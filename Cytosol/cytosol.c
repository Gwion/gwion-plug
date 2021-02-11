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

#define PROG(o)      (*(struct cyt_program**)o->data)
#define RUNNER(o)    (*(struct cyt_driver_runner**)(o->data + SZ_INT))
#define CELLENV(o)   (*(struct cyt_cell_env**)(o->data + SZ_INT*2))
#define EXECSTATE(o) (*(struct cyt_exec_state**)(o->data + SZ_INT*3))
#define FUNCVEC(o)   (*(struct Vector_*)(o->data + SZ_INT*4))

#define VALUE(o)   (*(struct cyt_value**)o->data)
#define BORROWED(o)   (*(m_int*)(o->data + SZ_INT))

/*
static void cytosol_print_line(void* data, size_t n, const struct cyt_value*const *values) {
  for(size_t i = 0; i < n; i++) {
    const cyt_value_type type = cyt_value_get_type(values[i]);
    switch(type) {
      case CYT_VALUE_TYPE_INTEGER: {
        ptrdiff_t integer;
        cyt_value_get_integer(values[i], &integer);
        printf("%li ", integer);
        break;
      }
      case CYT_VALUE_TYPE_STRING: {
        const char *str;
        size_t len;
        cyt_value_get_string(values[i], &str, &len);
        printf("%s ", str);
        break;
      }
      case CYT_VALUE_TYPE_RECORD:
        printf("%p ", values[i]);
        break;
    }
  }
  printf("\n");
}
*/
struct CytosolClosure_ {
  VM_Shred shred;
  VM_Code  code;
};

static CTOR(cytosol_ctor) {
  PROG(o) = cyt_program_new();
  RUNNER(o) = cyt_driver_runner_new();
  CELLENV(o) = cyt_cellenv_new();
  EXECSTATE(o) = cyt_exec_state_new();
/*  cyt_exec_state_set_extern_function(EXECSTATE(o),
    "test", cytosol_print_line, shred->info->vm->gwion);*/
  vector_init(&FUNCVEC(o));
}

static DTOR(cytosol_dtor) {
  cyt_program_destroy(PROG(o));
  cyt_driver_runner_destroy(RUNNER(o));
  cyt_cellenv_destroy(CELLENV(o));
  cyt_exec_state_destroy(EXECSTATE(o));

  for(m_uint i = 0; i < vector_size(&FUNCVEC(o)); i++) {
    struct CytosolClosure_ *closure = (struct CytosolClosure_*)vector_at(&FUNCVEC(o), i);
    _release(closure->shred->info->me, shred);
    vmcode_remref(closure->code, shred->info->vm->gwion);
    mp_free(shred->info->vm->gwion->mp, CytosolClosure, closure);
  }

  vector_release(&FUNCVEC(o));
}

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
  *(m_uint*)cyt_driver_runner_compile(RUNNER(o), PROG(o));
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

static MFUN(cytosol_count) {
  struct cyt_record_id out_id;
  const M_Object name = *(M_Object*)MEM(SZ_INT);
  if(cyt_program_record_by_name(PROG(o), STRING(name), &out_id)) {
    *(m_int*)RETURN = cyt_cellenv_count_records(CELLENV(o), out_id);
  } else
    *(m_int*)RETURN = 0;
}

static VM_Code mk_callback(MemPool mp, VM_Code base) {
  char name[strlen(base->name) + 11];
  sprintf(name, "%s(callback)", base->name);
  const Instr instr = (Instr)vector_back(base->instr);
  instr->opcode = eEOC;
  VM_Code code = new_vmcode(mp, base->instr, base->stack_depth, base->builtin, name);
  code->callback = 1;
  instr->opcode = eFuncReturn;
  return code;
}

ANN static inline m_str cytosol_type(const Gwion gwion, const struct cyt_value *value) {
  const enum cyt_value_type type = cyt_value_get_type(value);
  return type == CYT_VALUE_TYPE_INTEGER ?
     "Cytosol.Int" :  type == CYT_VALUE_TYPE_STRING ?
     "Cytosol.String" : "Cytosol.Record";
}

ANN static struct cyt_value* cytosol_cpy(const struct cyt_value *value) {
  const enum cyt_value_type type = cyt_value_get_type(value);
  switch(type) {
    case CYT_VALUE_TYPE_INTEGER: {
      ptrdiff_t n;
      cyt_value_get_integer(value, &n);
      return cyt_value_new_integer(n);
      break;
    }
    case CYT_VALUE_TYPE_STRING: {
      const char *str;size_t outlen;
      cyt_value_get_string(value, &str, &outlen);
      char name[outlen + 1];
      strncpy(name, str, outlen);
      name[outlen] = '\0';
      return cyt_value_new_string(name);
    }
    case CYT_VALUE_TYPE_RECORD: {
      struct cyt_value *ret = cyt_value_new_record();
      const struct cyt_value *v;
      size_t index = 0;
      while(cyt_value_get_record_field(value, index++, &v)) {
        struct cyt_value *field = cytosol_cpy(v);
        cyt_value_record_add_field(ret, field);
      }
      return ret;
    }
  }
}

static void cytosol_fun(void* data, size_t n, const struct cyt_value*const *values) {
  struct CytosolClosure_ *closure = (struct CytosolClosure_*)data;
  vmcode_addref(closure->code);
  const VM_Shred shred = new_vm_shred(closure->shred->info->vm->gwion->mp, closure->code);
  shred->base = closure->shred->base;
  const Type base = str2type(closure->shred->info->vm->gwion, "Cytosol.Value[]", (loc_t){});
  const Type t = array_type(closure->shred->info->vm->gwion->env, base, 1);
  const M_Object array = new_object(closure->shred->info->vm->gwion->mp, closure->shred, t);
  M_Vector vec = ARRAY(array) = new_m_vector(closure->shred->info->vm->gwion->mp, SZ_INT, n);
  for(m_uint i = 0; i < n; i++) {
    const m_str type_name = cytosol_type(closure->shred->info->vm->gwion, values[i]);
    struct cyt_value* const value = cytosol_cpy(values[i]);
    const M_Object obj = new_object_str(closure->shred->info->vm->gwion, NULL, type_name);
    VALUE(obj) = value; // beware type
    m_vector_set(vec, i, &obj); // beware type
  }
  *(M_Object*)shred->mem = array;
  vm_add_shred(closure->shred->info->vm, shred);
  shredule(closure->shred->tick->shreduler, closure->shred, 0);
}

// C defined func won't work atm
static MFUN(cytosol_set_fun) {
  struct CytosolClosure_ *closure = mp_malloc(shred->info->vm->gwion->mp, CytosolClosure);
  const M_Object name = *(M_Object*)MEM(SZ_INT);
  const VM_Code code = *(VM_Code*)MEM(SZ_INT*2);
  closure->shred = shred;
  closure->code = mk_callback(shred->info->vm->gwion->mp, code);
  ++closure->shred->info->me->ref;
  vector_add(&FUNCVEC(o), (m_uint)closure);
  cyt_exec_state_set_extern_function(EXECSTATE(o),
    STRING(name), cytosol_fun, closure);
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
  char c[outlen + 1];
  strncpy(c, data, outlen);
  c[outlen] = '\0';
  *(M_Object*)REG(-SZ_INT) = new_string(shred->info->vm->gwion->mp, shred, c);
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

static INSTR(IntCast) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(cyt_value_get_type(VALUE(o)) != CYT_VALUE_TYPE_INTEGER)
    Except(shred, "Invalid cytosol value cast to Int")
}

static INSTR(StringCast) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
//  if(cyt_value_get_type(VALUE(o)) != CYT_VALUE_TYPE_STRING)
//    Except(shred, "Invalid cytosol value cast to String")
}

static INSTR(RecordCast) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(cyt_value_get_type(VALUE(o)) != CYT_VALUE_TYPE_RECORD)
    Except(shred, "Invalid cytosol value cast to Record")
}

GWION_IMPORT(Cytosol) {
//  GWI_BB(gwi_fptr_ini(gwi, "void", "@CytosolFunType:[T]"))
//  GWI_BB(gwi_func_arg(gwi, "T", "values"))
//  GWI_BB(gwi_fptr_end(gwi, ae_flag_none))

  DECL_OB(const Type, t_cytosol, = gwi_class_ini(gwi, "Cytosol", "Object"))
  gwi_class_xtor(gwi, cytosol_ctor, cytosol_dtor);

  GWI_BB(gwi_item_ini(gwi, "@internal", "program"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_item_ini(gwi, "@internal", "runner"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_item_ini(gwi, "@internal", "cell_env"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_item_ini(gwi, "@internal", "exec_state"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_item_ini(gwi, "@internal", "func_vector"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

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
    gwi_class_xtor(gwi, Record_ctor, NULL);

    GWI_BB(gwi_func_ini(gwi, "Value", "field"))
    GWI_BB(gwi_func_arg(gwi, "int", "index"))
    GWI_BB(gwi_func_end(gwi, record_get_field, ae_flag_none))

    GWI_BB(gwi_func_ini(gwi, "bool", "field"))
    GWI_BB(gwi_func_arg(gwi, "Value", "new_field"))
    GWI_BB(gwi_func_end(gwi, record_add_field, ae_flag_none))

    GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_func_ini(gwi, "bool", "compile"))
  GWI_BB(gwi_func_end(gwi, cytosol_compile, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "count"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, cytosol_count, ae_flag_none))

  GWI_BB(gwi_fptr_ini(gwi, "void", "FunType"))
  GWI_BB(gwi_func_arg(gwi, "Cytosol.Value[]", "values"))
  GWI_BB(gwi_fptr_end(gwi, ae_flag_global))

  GWI_BB(gwi_func_ini(gwi, "void", "set_fun"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_arg(gwi, "FunType", "fun"))
  GWI_BB(gwi_func_end(gwi, cytosol_set_fun, ae_flag_none))

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

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Value", "Cytosol.Int", "Cytosol.Int"))
  GWI_BB(gwi_oper_end(gwi, "$", IntCast))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Value", "Cytosol.String", "Cytosol.String"))
  GWI_BB(gwi_oper_end(gwi, "$", StringCast))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Value", "Cytosol.Record", "Cytosol.Record"))
  GWI_BB(gwi_oper_end(gwi, "$", RecordCast))

  return GW_OK;
}
