#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "shreduler_private.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "plug.h"
#include "import.h"
#include "compile.h"
#include "traverse.h"
#include "emit.h"
#include "array.h"

#ifdef JIT
#include "jitter.h"
#endif
static SFUN(machine_add) {
  const M_Object obj = *(M_Object*)MEM(0);
  if(!obj)
    return;
  const m_str str = STRING(obj);
  _release(obj, shred);
  if(!str)
    return;
  *(m_uint*)RETURN = compile_filename(shred->info->vm->gwion, str);
}

static SFUN(machine_check) {
  *(m_uint*)RETURN = -1;
  const M_Object code_obj = *(M_Object*)MEM(0);
  const m_str line = code_obj ? STRING(code_obj) : NULL;
  release(code_obj, shred);
  if(!line)return;
  *(m_uint*)RETURN = check_string(shred->info->vm->gwion, "Machine.check", line);
/*
  FILE* f = fmemopen(line, strlen(line), "r");
  const Ast ast = parse(shred->vm->gwion->scan, "Machine.check", f);
  if(!ast)
    goto close;
  *(m_uint*)RETURN = traverse_ast(shred->vm->gwion->emit->env, ast);
close:
  if(ast)
    free_ast(ast);
  fclose(f);
*/
}

static SFUN(machine_compile) {
  *(m_uint*)RETURN = -1;
  const M_Object code_obj = *(M_Object*)MEM(0);
  const m_str line = code_obj ? STRING(code_obj) : NULL;
  release(code_obj, shred);
  if(!line)return;
  *(m_uint*)RETURN = compile_string(shred->info->vm->gwion, "Machine.compile", line);
}

static SFUN(machine_shreds) {
  VM* vm = shred->info->vm;
  const Type t = array_type(t_int, 1);
  const M_Object obj = new_array(t, vector_size(&vm->shreduler->shreds));
  for(m_uint i = 0; i < vector_size(&vm->shreduler->shreds); i++) {
    const VM_Shred sh = (VM_Shred)vector_at(&vm->shreduler->shreds, i);
    m_vector_set(ARRAY(obj), i, &sh->tick->xid);
  }
  vector_add(&shred->gc, (vtype)obj);
  *(M_Object*)RETURN = obj;
}

GWION_IMPORT(machine) {
  Type t_machine;
  CHECK_OB((t_machine = gwi_mk_type(gwi, "Machine", 0, NULL)))
  CHECK_BB(gwi_class_ini(gwi,  t_machine, NULL, NULL))
  gwi_func_ini(gwi, "void",  "add", machine_add);
  gwi_func_arg(gwi,       "string",  "filename");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int[]", "shreds", machine_shreds);
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int",  "check", machine_check);
  gwi_func_arg(gwi,      "string",  "code");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "void", "compile", machine_compile);
  gwi_func_arg(gwi,      "string",  "filename");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  CHECK_BB(gwi_class_end(gwi))
  return GW_OK;
}
