#include <stdlib.h>
#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "shreduler_private.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "compile.h"
#include "traverse.h"
#include "emit.h"
#include "array.h"
#include "pass.h"

static SFUN(machine_add) {
  const M_Object obj = *(M_Object*)MEM(0);
  if(!obj)
    return;
  const m_str str = STRING(obj);
  if(!str)
    return;
  *(m_uint*)RETURN = compile_filename(shred->info->vm->gwion, str);
}

static SFUN(machine_remove) {
  const VM* vm = shred->info->vm;
  const m_int xid = *(m_int*)MEM(0);
  *(m_int*)RETURN = vm_remove(vm, xid);
}

static SFUN(machine_replace) {
  const VM* vm = shred->info->vm;
  const m_int xid = *(m_int*)MEM(0);
  *(m_int*)RETURN = vm_remove(vm, xid);
  const M_Object str = *(M_Object*)MEM(SZ_INT);
  compile_filename_xid(shred->info->vm->gwion, STRING(str), xid);
}

static SFUN(machine_check) {
  *(m_uint*)RETURN = -1;
  const M_Object code_obj = *(M_Object*)MEM(0);
  const m_str line = code_obj ? STRING(code_obj) : NULL;
  if(!line)return;
  struct Vector_ v;
  vector_init(&v);
  vector_add(&v, (vtype)"check");
  pass_set(shred->info->vm->gwion, &v);
  vector_release(&v);
  *(m_uint*)RETURN = compile_string(shred->info->vm->gwion, "Machine.check", line);
}

static SFUN(machine_compile) {
  *(m_uint*)RETURN = -1;
  const M_Object code_obj = *(M_Object*)MEM(0);
  const m_str line = code_obj ? STRING(code_obj) : NULL;
  if(!line)return;
  *(m_uint*)RETURN = compile_string(shred->info->vm->gwion, "Machine.compile", line);
}

static SFUN(machine_compile_replace) {
  *(m_uint*)RETURN = -1;
  machine_remove(o, RETURN, shred);
  const m_int xid = *(m_int*)MEM(0);
  const M_Object code_obj = *(M_Object*)MEM(SZ_INT);
  const m_str line = code_obj ? STRING(code_obj) : NULL;
  if(!line)return;
  *(m_uint*)RETURN = compile_string_xid(shred->info->vm->gwion, "Machine.compile", line, xid);
}

static SFUN(machine_shreds) {
  VM* vm = shred->info->vm;
  const Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_int], 1);
  const Vector v = &vm->shreduler->active_shreds;
  const M_Object obj = new_array(shred->info->mp, t, vector_size(v));
  for(m_uint i = 0; i < vector_size(v); i++) {
    const VM_Shred sh = (VM_Shred)vector_at(v, i);
    m_vector_set(ARRAY(obj), i, &sh->tick->xid);
  }
  *(M_Object*)RETURN = obj;
}

static SFUN(machine_pass) {
  const M_Object array_obj = *(M_Object*)MEM(0);
  M_Vector array = ARRAY(array_obj);
  struct Vector_ v;
  for(m_uint i = 0; i < m_vector_size(array); ++i) {
    m_str str;
    m_vector_get(array, i, &str);
    vector_add(&v, (vtype)str);
  }
  *(m_uint*)RETURN = pass_set(shred->info->vm->gwion, &v);
  vector_release(&v);
}

GWION_IMPORT(machine) {
  GWI_OB(gwi_struct_ini(gwi, "Machine"))
  gwi_func_ini(gwi, "int",  "add");
  gwi_func_arg(gwi, "string",  "filename");
  GWI_BB(gwi_func_end(gwi, machine_add, ae_flag_static))

  gwi_func_ini(gwi, "int",  "remove");
  gwi_func_arg(gwi, "int", "id");
  GWI_BB(gwi_func_end(gwi, machine_remove, ae_flag_static))

  gwi_func_ini(gwi, "int",  "replace");
  gwi_func_arg(gwi, "int", "id");
  gwi_func_arg(gwi, "string", "filename");
  GWI_BB(gwi_func_end(gwi, machine_replace, ae_flag_static))

  gwi_func_ini(gwi, "int[]", "shreds");
  GWI_BB(gwi_func_end(gwi, machine_shreds, ae_flag_static))

  gwi_func_ini(gwi, "int",  "check");
  gwi_func_arg(gwi, "string", "code");
  GWI_BB(gwi_func_end(gwi, machine_check, ae_flag_static))

  gwi_func_ini(gwi, "int", "compile");
  gwi_func_arg(gwi, "string", "code");
  GWI_BB(gwi_func_end(gwi, machine_compile, ae_flag_static))

  gwi_func_ini(gwi, "int", "compile_replace");
  gwi_func_arg(gwi, "string", "code");
  gwi_func_arg(gwi, "string", "filename");
  GWI_BB(gwi_func_end(gwi, machine_compile_replace, ae_flag_static))

  gwi_func_ini(gwi, "int", "pass");
  gwi_func_arg(gwi, "string[]", "passes");
  GWI_BB(gwi_func_end(gwi, machine_pass, ae_flag_static))

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
