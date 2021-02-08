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
#include "ugen.h"
#include "array.h"

#include "cytosol.h"

#define PROG(o)   (*(struct cyt_program**)o->data)
#define RUNNER(o) (*(struct cyt_driver_runner**)(o->data + SZ_INT))
#define CELLENV(o) (*(struct cyt_cell_env**)(o->data + SZ_INT*2))
#define EXECSTATE(o) (*(struct cyt_exec_state**)(o->data + SZ_INT*3))
#define VALUE(o)   (*(struct cyt_value**)o->data)
#define BORROWED(o)   (*(m_int*)(o->data + SZ_INT))

static CTOR(cytosol_ctor) {
  PROG(o) = cyt_program_new();
  RUNNER(o) = cyt_driver_runner_new();
  CELLENV(o) = cyt_cellenv_new();
  EXECSTATE(o) = cyt_exec_state_new();
}

static DTOR(cytosol_dtor) {
  cyt_program_destroy(PROG(o));
  cyt_driver_runner_destroy(RUNNER(o));
  cyt_cellenv_destroy(CELLENV(o));
  cyt_exec_state_destroy(EXECSTATE(o));
}

static m_int o_cytosol_member_data;
static m_int o_cytosol_static_data;
static m_int* cytosol_static_value;

static MFUN(file_from_string) {
  cyt_driver_runner_add_file_from_string(RUNNER(o),
    STRING(*(M_Object*)MEM(SZ_INT)),
    STRING(*(M_Object*)MEM(SZ_INT*2)));
}

static MFUN(file_from_path) {
  cyt_driver_runner_add_file_from_path(RUNNER(o),
    STRING(*(M_Object*)MEM(SZ_INT)));
}

static MFUN(cytosol_compile) {
  cyt_driver_runner_compile(RUNNER(o), PROG(o));
}

static MFUN(cytosol_add_record) {
  struct cyt_record_id out_id;
  const M_Object name = *(M_Object*)MEM(SZ_INT);
  const m_int quantity = *(m_int*)MEM(SZ_INT*2);
  if(quantity < 0)
    Except(shred, "Negative quantity requested");
  if(!cyt_program_record_by_name(PROG(o), STRING(name), &out_id))
    Except(shred, "Invalid record Creation");
  const M_Object fields = *(M_Object*)MEM(SZ_INT*3);
  const M_Vector array = ARRAY(fields);
  cyt_cellenv_add_record(CELLENV(o), quantity, out_id, m_vector_size(array), (const struct cyt_value * const*)ARRAY_PTR(array));
}

static MFUN(cytosol_run) {
  const m_int bound = *(m_int*)MEM(SZ_INT);
  if(bound < 0)
    Except(shred, "Negative iteration requested");
   cyt_driver_runner_run(
    RUNNER(o), PROG(o), EXECSTATE(o), CELLENV(o), bound);
}

static MFUN(cytosol_run_single) {
  *(m_int*)RETURN = !cyt_driver_runner_run_single_iteration(
    RUNNER(o), PROG(o), EXECSTATE(o), CELLENV(o));
}

static DTOR(value_dtor) {
  if(!BORROWED(o))
    cyt_value_destroy(VALUE(o));
}

static CTOR(Int_ctor) {
  VALUE(o) = cyt_value_new_integer(0);
}

static CTOR(String_ctor) {
  VALUE(o) = cyt_value_new_string("");
}

static CTOR(Record_ctor) {
  VALUE(o) = cyt_value_new_record();
}

static INSTR(int2value) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(0);
  cyt_value_destroy(VALUE(o));
  VALUE(o) = cyt_value_new_integer(*(m_int*)REG(-SZ_INT));
}

static INSTR(value2int) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  ptrdiff_t i;
  cyt_value_get_integer(VALUE(o), &i);
  *(m_int*)REG(-SZ_INT) = **(m_int**)REG(0) = i;
}

static INSTR(string2value) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(0);
  cyt_value_destroy(VALUE(o));
  const M_Object str = *(M_Object*)REG(-SZ_INT);
  VALUE(o) = cyt_value_new_string(STRING(str));
}

static INSTR(value2string) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const M_Object str = *(M_Object*)REG(-SZ_INT);
  const char* data;
  size_t outlen;
  cyt_value_get_string(VALUE(o), &data, &outlen);
  *(M_Object*)REG(-SZ_INT) = new_string(shred->info->vm->gwion->mp, shred, (const m_str)data);
}


static inline m_str cytosol_type(const Gwion gwion, const struct cyt_value *value) {
  enum cyt_value_type type = cyt_value_get_type(value);
  return type == CYT_VALUE_TYPE_INTEGER ?
     "Cytosol.Int" :  type == CYT_VALUE_TYPE_STRING ?
     "Cytosol.String" : "Cytosol.Record";
}

static MFUN(record_get_field) {
  const M_Object record = *(M_Object*)MEM(0);
  const size_t index = *(m_uint*)MEM(SZ_INT); // taking arg as uint, cytosol will error by itself anyway
  const struct cyt_value *out_value;
  if(!cyt_value_get_record_field(VALUE(record), index, &out_value))
    Except(shred, "invalid field index requested");
  const m_str type = cytosol_type(shred->info->vm->gwion, out_value);
  *(M_Object*)RETURN = new_object_str(shred->info->vm->gwion, shred, type);
}

static MFUN(record_add_field) {
  const M_Object new_field = *(M_Object*)MEM(SZ_INT); // taking arg as uint, cytosol will error by itself anyway
  cyt_value_record_add_field(VALUE(o), VALUE(new_field));
  BORROWED(new_field) = 1;
}

GWION_IMPORT(Cytosol) {
  DECL_OB(const Type, t_cytosol, = gwi_class_ini(gwi, "Cytosol", "Object"))
  gwi_class_xtor(gwi, cytosol_ctor, cytosol_dtor);

  GWI_BB(gwi_item_ini(gwi, "@internal", "program"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_item_ini(gwi, "@internal", "runner"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, 0))

  GWI_BB(gwi_item_ini(gwi, "@internal", "cell_env"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, 0))

  GWI_BB(gwi_item_ini(gwi, "@internal", "exec_state"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, 0))

    DECL_OB(const Type, t_value, = gwi_class_ini(gwi, "Value", "Object"))
    gwi_class_xtor(gwi, NULL, value_dtor);
    GWI_BB(gwi_item_ini(gwi, "@internal", "value"))
    GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
    GWI_BB(gwi_item_ini(gwi, "@internal", "@borrowed"))
    GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
    GWI_BB(gwi_class_end(gwi))
    SET_FLAG(t_value, abstract);

  GWI_BB(gwi_func_ini(gwi, "void", "add_string"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_arg(gwi, "string", "source"))
  GWI_BB(gwi_func_end(gwi, file_from_string, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "add_path"))
  GWI_BB(gwi_func_arg(gwi, "string", "path"))
  GWI_BB(gwi_func_end(gwi, file_from_path, ae_flag_none))

  // add enum?
  GWI_BB(gwi_func_ini(gwi, "void", "run"))
  GWI_BB(gwi_func_arg(gwi, "int", "bound"))
  GWI_BB(gwi_func_end(gwi, cytosol_run, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "bool", "run_single"))
  GWI_BB(gwi_func_end(gwi, cytosol_run_single, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "bool", "compile"))
  GWI_BB(gwi_func_end(gwi, cytosol_compile, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "add_record"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_arg(gwi, "int", "quantity"))
  GWI_BB(gwi_func_arg(gwi, "Value[]", "fields"))
  GWI_BB(gwi_func_end(gwi, cytosol_add_record, ae_flag_none))

    DECL_OB(const Type, t_int, = gwi_class_ini(gwi, "Int", "Value"))
    gwi_class_xtor(gwi, Int_ctor, NULL);
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_string, = gwi_class_ini(gwi, "String", "Value"))
    gwi_class_xtor(gwi, String_ctor, NULL);
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_record, = gwi_class_ini(gwi, "Record", "Value"))
//    GWI_BB(gwi_func_arg(gwi, "Value[]", "fields"))
    gwi_class_xtor(gwi, Record_ctor, NULL);

    GWI_BB(gwi_func_ini(gwi, "Value", "field"))
    GWI_BB(gwi_func_arg(gwi, "int", "index"))
    GWI_BB(gwi_func_end(gwi, record_get_field, ae_flag_none))

    GWI_BB(gwi_func_ini(gwi, "bool", "field"))
    GWI_BB(gwi_func_arg(gwi, "Value", "new_field"))
    GWI_BB(gwi_func_end(gwi, cytosol_compile, ae_flag_none))

    GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_func_ini(gwi, "bool", "compile"))
  GWI_BB(gwi_func_end(gwi, cytosol_compile, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))


  // define operators at global scope
  GWI_BB(gwi_oper_ini(gwi, "int", "Cytosol.Int", "int"))
  GWI_BB(gwi_oper_end(gwi, "@=>", int2value))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Int", "int", "int"))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "@=>", value2int))

  GWI_BB(gwi_oper_ini(gwi, "string", "Cytosol.String", "string"))
  GWI_BB(gwi_oper_end(gwi, "@=>", string2value))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.String", "string", "string"))
  GWI_BB(gwi_oper_end(gwi, "@=>", value2string))

  return GW_OK;
}
