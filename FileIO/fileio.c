#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <complex.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "array.h"
#include "gwi.h"

m_int o_fileio_file;

static DTOR(fileio_dtor) {
  if(IO_FILE(o))
    fclose(IO_FILE(o));
}

static DTOR(static_fileio_dtor) {
  IO_FILE(o) = NULL;
}

#define describe_2file(name, type, offset, pop, format, ...)  \
static INSTR(name##_to_file) {\
  POP_REG(shred, SZ_INT)                                      \
  const type lhs = *(type*)REG(-offset);                      \
  const M_Object o = *(M_Object*)REG(0);                      \
  if(!IO_FILE(o)) {                                           \
    handle(shred, "EmptyFilehandleion");                      \
    return;                                                   \
  }                                                           \
  fprintf(IO_FILE(o), format, __VA_ARGS__);                   \
  pop;                                                        \
  *(M_Object*)REG(-SZ_INT) = o;                               \
}
describe_2file(int, m_int, SZ_INT,, "%" INT_F "", lhs)
describe_2file(float, m_float, SZ_FLOAT,
 PUSH_REG(shred, SZ_INT - SZ_FLOAT), "%f", lhs)
describe_2file(string, M_Object, SZ_INT,
  , "%s", lhs ? STRING(lhs) : NULL)
describe_2file(object, M_Object, SZ_INT,
  , "%p", (void*)lhs)
describe_2file(complex, m_complex, SZ_COMPLEX,
  PUSH_REG(shred, SZ_INT - SZ_COMPLEX), "#(%f, %f)", creal(lhs), cimag(lhs))
describe_2file(polar, m_complex, SZ_COMPLEX,
  PUSH_REG(shred, SZ_INT - SZ_COMPLEX), "%%(%f, %f)", creal(lhs), cimag(lhs)/M_PI)
describe_2file(vec3, m_vec3, SZ_VEC3,
  PUSH_REG(shred, SZ_INT - SZ_VEC3), "@(%f, %f, %f)", lhs.x, lhs.y, lhs.z)
describe_2file(vec4, m_vec4, SZ_VEC4,
  POP_REG(shred, SZ_INT - SZ_VEC4), "@(%f, %f, %f, %f)", lhs.x, lhs.y, lhs.z, lhs.w)

static INSTR(file_to_int) {
  POP_REG(shred, SZ_INT)
  m_int* ret = *(m_int**)REG(0);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(!o) {
    handle(shred, "EmptyFilehandleion");
    return;
  }
  if(IO_FILE(o)) {
    if(fscanf(IO_FILE(o), "%"INT_F, ret) < 0) {
      handle(shred, "FileReadhandleion");
      return;
    }
    *(m_uint*)REG(-SZ_INT) = *ret;
  } else
    handle(shred, "EmptyFilehandleion");
}

static INSTR(file_to_float) {
  POP_REG(shred, SZ_INT)
  float ret;
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(!o) {
    handle(shred, "EmptyFilehandleion");
    return;
  }
  if(IO_FILE(o)) {
    if(fscanf(IO_FILE(o), "%f", &ret) < 0) {
      handle(shred, "FileReadhandleion");
      return;
    }                                  // LCOV_EXCL_LINE
    *(m_float*)REG(- SZ_FLOAT) = (**(m_float**)REG(0) = ret);
  } else {
    handle(shred, "EmptyFilehandleion");
    return;
  }
  POP_REG(shred, SZ_FLOAT)
}

static INSTR(file_to_string) {
  POP_REG(shred, SZ_INT)
  const M_Object o    = *(M_Object*)REG(- SZ_INT);
  const M_Object s    = *(M_Object*)REG(0);
  if(!o) {
    handle(shred, "EmptyFilehandle");
    return;
  }
  if(!s) {
    handle(shred, "EmptyStringhandle");
    return;
  }
  if(IO_FILE(o)) {
    char *line = NULL;
    size_t sz = 0;
    const ssize_t ret = getline(&line, &sz, IO_FILE(o));
    if(ret > 0) {
      line[ret-1] = '\0';
      STRING(s) = s_name(insert_symbol(shred->info->vm->gwion->st, line));
      xfree(line);
    }
    *(M_Object*)REG(- SZ_INT) = s;
  }
}

static MFUN(file_eof) {
  if(!o || !IO_FILE(o)) {
    handle(shred, "EmptyFilehandle");
    return;
  }
  *(m_int*)RETURN = feof(IO_FILE(o));
}

static MFUN(file_nl) {
  *(m_uint*)RETURN = fwrite("\n",  strlen("\n"), 1, IO_FILE(o));
}

static MFUN(file_open) {
  const M_Object lhs = *(M_Object*)MEM(SZ_INT * 2);
  const M_Object rhs = *(M_Object*)MEM(SZ_INT);
  if(!lhs || !rhs) {
    handle(shred, "invalid arguments to FileIO.open()");
    return;
  }
  const m_str filename = STRING(rhs);
  const m_str mode = STRING(lhs);
  if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
  }
  IO_FILE(o) = fopen(filename, mode);
  *(m_uint*)RETURN = IO_FILE(o) ? 1 : 0;
}

static MFUN(file_close) {
  if(IO_FILE(o)) {
    fclose(IO_FILE(o));
    IO_FILE(o) = NULL;
    *(m_uint*)RETURN = 1;
  } else
    *(m_uint*)RETURN = !IO_FILE(o) ? 1 : 0;
}

static MFUN(file_fileno) {
  FILE* file = IO_FILE(o);
  *(m_int*)RETURN = file ? fileno(file) : -1;
}

static SFUN(file_remove) {
  const M_Object obj = *(M_Object*)MEM(0);
  if(!obj)
    return;
  *(m_uint*)RETURN = remove(STRING(*(M_Object*)MEM(0)));
}

static SFUN(file_list) {
  struct dirent **namelist;
  const M_Object obj = *(M_Object*)MEM(0);
  if(!obj)
    return;
  const m_str str = STRING(obj);
  if(!str)
    return;
  const m_int n = scandir(str, &namelist, NULL, alphasort);
  if(n < 0) {
    const Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_string], 0);
    const M_Object ret = new_array(shred->info->mp, t, n);
    *(M_Object*)RETURN = ret;
    return;
  }
  const Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_string], 1);
  const M_Object ret = new_array(shred->info->mp, t, n);
  vector_add(&shred->gc, (vtype)ret);
  for(m_uint i = 0; i < (m_uint)n; i++) {
    const M_Object string = new_string(shred->info->mp, NULL, namelist[i]->d_name);
    m_vector_set(ARRAY(ret), i, &string);
    free(namelist[i]);
  }
  free(namelist);
  *(M_Object*)RETURN = ret;
}

GWION_IMPORT(fileio) {
  DECL_OB(const Type, t_fileio, = gwi_class_ini(gwi, "FileIO", "Event"));
  gwi_class_xtor(gwi, NULL, fileio_dtor);
  o_fileio_file = t_fileio->nspc->offset;
  t_fileio->nspc->offset += SZ_INT;
  // import vars

  // import funcs
  gwi_func_ini(gwi, "int", "nl");
  GWI_BB(gwi_func_end(gwi, file_nl, ae_flag_none))
  gwi_func_ini(gwi, "int", "open");
  gwi_func_arg(gwi, "string", "filename");
  gwi_func_arg(gwi, "string", "mode");
  GWI_BB(gwi_func_end(gwi, file_open, ae_flag_none))
  gwi_func_ini(gwi, "int", "close");
  GWI_BB(gwi_func_end(gwi, file_close, ae_flag_none))
  gwi_func_ini(gwi, "int", "fileno");
  GWI_BB(gwi_func_end(gwi, file_fileno, ae_flag_none))
  gwi_func_ini(gwi, "int", "eof");
  GWI_BB(gwi_func_end(gwi, file_eof, ae_flag_none))
  gwi_func_ini(gwi, "int", "remove");
  gwi_func_arg(gwi, "string", "filename");
  GWI_BB(gwi_func_end(gwi, file_remove, ae_flag_static))
  gwi_func_ini(gwi, "string[]", "list");
  gwi_func_arg(gwi, "string", "filename");
  GWI_BB(gwi_func_end(gwi, file_list, ae_flag_static))

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi,"string", "FileIO", "FileIO"))
  GWI_BB(gwi_oper_end(gwi, "=>", string_to_file))
  GWI_BB(gwi_oper_ini(gwi, "FileIO", "string", "string"))
  GWI_BB(gwi_oper_add(gwi, opck_const_rhs))
  GWI_BB(gwi_oper_end(gwi, "=>", file_to_string))
  GWI_BB(gwi_oper_ini(gwi, "FileIO", "int",    "int"))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "=>", file_to_int))
  GWI_BB(gwi_oper_ini(gwi, "FileIO", "float",  "float"))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "=>", file_to_float))

  const Type t_internal = gwi_class_ini(gwi, "@FileIOInternal", "FileIO");
  gwi_class_xtor(gwi,  NULL, static_fileio_dtor);
  GWI_BB(gwi_class_end(gwi))

  const M_Object gw_cin = new_object(gwi->gwion->mp, NULL, t_internal);
  IO_FILE(gw_cin) = stdin;
  vector_init(&EV_SHREDS(gw_cin));
  const M_Object gw_cout = new_object(gwi->gwion->mp, NULL, t_internal);
  IO_FILE(gw_cout) = stdout;
  vector_init(&EV_SHREDS(gw_cout));
  const M_Object gw_cerr = new_object(gwi->gwion->mp, NULL, t_internal);
  IO_FILE(gw_cerr) = stderr;
  vector_init(&EV_SHREDS(gw_cerr));
  gwi_item_ini(gwi, "@FileIOInternal", "cin");
  gwi_item_end(gwi, ae_flag_const, obj, gw_cin);
  gwi_item_ini(gwi, "@FileIOInternal", "cout");
  gwi_item_end(gwi, ae_flag_const, obj, gw_cout);
  gwi_item_ini(gwi, "@FileIOInternal", "cerr");
  gwi_item_end(gwi, ae_flag_const, obj, gw_cerr);
  return GW_OK;
}

#ifdef JIT
#include "ctrl/fileio.h"
#include "code/fileio.h"
#endif

