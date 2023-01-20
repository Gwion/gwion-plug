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
#include "gwi.h"
#include "threads.h"

static DTOR(fileio_dtor) {
  fclose(IO_FILE(o));
}

struct FileIOInfo {
  m_str str;
  FILE * file;
  VM_Shred shred;
};

static int fileio_read(void *data) {
  VM_Shred shred = (VM_Shred)data;
  const m_uint offset = *(m_uint*)(shred->reg + SZ_INT*2);
  const M_Object o = *(M_Object*)(shred->mem + offset);
  char *c = NULL;
  size_t n;
  GwText txt;
  text_init(&txt, shred->info->mp);
  if(getline(&c, &n, IO_FILE(o)) > 0 ) {
    text_add(&txt, c);
    xfree(c);
 //   txt.str[--txt.len] = '\0'; // remove last '\n'
    const M_Object s = new_string(shred->info->vm->gwion, txt.str);
    shred->reg += SZ_INT;
    *(M_Object*)(shred->reg - SZ_INT) = s;
    shredule(shred->tick->shreduler, shred, GWION_EPSILON);
  } else
    xfun_handle(shred, "FileReadException");
  return 0;
}

static int fileio_write(void *data) {
  struct FileIOInfo *info = data;
  const size_t sz = strlen(info->str);
  const VM_Shred shred = info->shred;
  const m_str str = info->str;
  FILE *file = info->file;
  mp_free2(shred->info->mp, sizeof(struct FileIOInfo), info);
  if(fwrite(str, sz, 1,  file) > 0)
    shredule(shred->tick->shreduler, shred, 1);
  else
    xfun_handle(shred, "FileWriteException");
  return 0;
}

static MFUN(file_eof) {
  *(m_int*)RETURN = feof(IO_FILE(o));
}

static MFUN(file_flush) {
  *(m_int*)RETURN = fflush(IO_FILE(o));
}

static MFUN(file2string) {
  shreduler_remove(shred->tick->shreduler, shred, 0);
//  fileio_read(shred);
  thrd_t thrd;
  thrd_create(&thrd, fileio_read, shred);
  thrd_detach(thrd);
}

static MFUN(string2file) {
  shreduler_remove(shred->tick->shreduler, shred, 0);
  struct FileIOInfo *info = mp_calloc2(shred->info->mp, sizeof(struct FileIOInfo));
  info->shred = shred;
  info->file = IO_FILE(o);
  const M_Object arg = *(M_Object*)(shred->mem + SZ_INT);
  info->str = STRING(arg);
  thrd_t thrd;
  thrd_create(&thrd, fileio_write, info);
  thrd_detach(thrd);
}

static MFUN(file_open) {
  const M_Object file = *(M_Object*)MEM(SZ_INT);
  const M_Object mode = *(M_Object*)MEM(SZ_INT * 2);
  if((IO_FILE(o) = fopen(STRING(file), STRING(mode))))
    *(M_Object*)RETURN = o;
  else
    xfun_handle(shred, "FileCtorException");
}

static void cfile(const Gwi gwi, const Type t, const m_str name, FILE *const file) {
  const M_Object o = new_object(gwi->gwion->mp, t);
  IO_FILE(o) = file;
  gwi_item_ini(gwi, "FileIO", name);
  gwi_item_end(gwi, ae_flag_const, obj, o);
}

GWION_IMPORT(fileio) {
  gwidoc(gwi, "Reading and writing text from/to a file.");
  DECL_OB(const Type, t_fileio, = gwi_class_ini(gwi, "FileIO", "Object"));
  gwi_class_xtor(gwi, NULL, fileio_dtor);
  SET_FLAG(t_fileio, abstract | ae_flag_final);
  t_fileio->nspc->offset += SZ_INT;

  gwidoc(gwi, "The constructor. very similar to fopen");
  gwi_func_ini(gwi, "auto", "new");
  gwi_func_arg(gwi, "string", "filename");
  gwi_func_arg(gwi, "string", "mode");
  GWI_BB(gwi_func_end(gwi, file_open, ae_flag_none))

  gwidoc(gwi, "Read a line");
  gwi_func_ini(gwi, "string", "read");
  GWI_BB(gwi_func_end(gwi, file2string, ae_flag_none))

  gwidoc(gwi, "Write a string");
  gwi_func_ini(gwi, "void", "write");
  gwi_func_arg(gwi, "string", "txt");
  GWI_BB(gwi_func_end(gwi, string2file, ae_flag_none))

  gwidoc(gwi, "Check for EOF");
  gwi_func_ini(gwi, "int", "eof");
  GWI_BB(gwi_func_end(gwi, file_eof, ae_flag_none))

  gwidoc(gwi, "flush the file");
  gwi_func_ini(gwi, "int", "flush");
  GWI_BB(gwi_func_end(gwi, file_flush, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

  gwidoc(gwi, "stdin, stderr and stdout");
  cfile(gwi, t_fileio, "stdin", stdin);
  cfile(gwi, t_fileio, "stdout", stdout);
  cfile(gwi, t_fileio, "stderr", stderr);
  return GW_OK;
}
