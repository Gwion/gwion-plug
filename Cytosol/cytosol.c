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
#define VALUE(o)   (*(struct cyt_value**)o->data)

static CTOR(cytosol_ctor) {
  PROG(o) = cyt_program_new();
  RUNNER(o) = cyt_driver_runner_new();
}

static DTOR(cytosol_dtor) {
  cyt_program_destroy(PROG(o));
  cyt_driver_runner_destroy(RUNNER(o));
}

static m_int o_cytosol_member_data;
static m_int o_cytosol_static_data;
static m_int* cytosol_static_value;

static MFUN(file_from_string) {
  cyt_driver_runner_add_file_from_string(RUNNER(o),
    STRING(*(M_Object*)MEM(SZ_INT)),
    STRING(*(M_Object*)MEM(SZ_INT*2)));
}

static MFUN(cytosol_compile) {
  cyt_driver_runner_compile(RUNNER(o), PROG(o));
}

static DTOR(value_dtor) {
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

GWION_IMPORT(Cytosol) {
  DECL_OB(const Type, t_cytosol, = gwi_class_ini(gwi, "Cytosol", "Object"))
  gwi_class_xtor(gwi, cytosol_ctor, cytosol_dtor);

  GWI_BB(gwi_item_ini(gwi, "@internal", "program"))
  GWI_BB((o_cytosol_member_data = gwi_item_end(gwi, ae_flag_none, num, 0)))

  GWI_BB(gwi_item_ini(gwi, "@internal", "runner"))
  GWI_BB((o_cytosol_static_data = gwi_item_end(gwi, ae_flag_static, num, 1234)))

  GWI_BB(gwi_func_ini(gwi, "void", "add_string"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_arg(gwi, "string", "source"))
  GWI_BB(gwi_func_end(gwi, file_from_string, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "bool", "compile"))
  GWI_BB(gwi_func_end(gwi, cytosol_compile, ae_flag_none))

    DECL_OB(const Type, t_value, = gwi_class_ini(gwi, "Value", "Object"))
    gwi_class_xtor(gwi, NULL, value_dtor);
    GWI_BB(gwi_item_ini(gwi, "@internal", "value"))
    GWI_BB((o_cytosol_member_data = gwi_item_end(gwi, ae_flag_none, num, 0)))
    GWI_BB(gwi_class_end(gwi))
    SET_FLAG(t_value, abstract);

    DECL_OB(const Type, t_int, = gwi_class_ini(gwi, "Int", "Value"))
    gwi_class_xtor(gwi, Int_ctor, NULL);
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_string, = gwi_class_ini(gwi, "String", "Value"))
    gwi_class_xtor(gwi, String_ctor, NULL);
    GWI_BB(gwi_class_end(gwi))

/*
    DECL_OB(const Type, t_record, = gwi_class_ini(gwi, "Record", "Value"))
    gwi_class_xtor(gwi, Record_ctor, NULL);
    GWI_BB(gwi_func_ini(gwi, "Option:[Value]", "field"))
    GWI_BB(gwi_func_arg(gwi, "int", "index"))
    GWI_BB(gwi_func_end(gwi, cytosol_compile, ae_flag_none))

    GWI_BB(gwi_func_ini(gwi, "bool", "field"))
    GWI_BB(gwi_func_arg(gwi, "Value", "new_field"))
    GWI_BB(gwi_func_end(gwi, cytosol_compile, ae_flag_none))
    GWI_BB(gwi_class_end(gwi))
*/
  GWI_BB(gwi_func_ini(gwi, "bool", "compile"))
  GWI_BB(gwi_func_end(gwi, cytosol_compile, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))


  // define operators at global scope
  GWI_BB(gwi_oper_ini(gwi, "int", "Cytosol.Int", "int"))
  GWI_BB(gwi_oper_end(gwi, "=>", int2value))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Int", "int", "int"))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "=>", value2int))

  GWI_BB(gwi_oper_ini(gwi, "string", "Cytosol.Int", "string"))
  GWI_BB(gwi_oper_end(gwi, "@=>", string2value))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Int", "string", "string"))
  GWI_BB(gwi_oper_end(gwi, "=>", value2string))

  return GW_OK;
}
