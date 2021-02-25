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
#include "traverse.h"
#include "emit.h"

#include "cytosol.h"

#define PROG(o)      (*(struct cyt_program**)o->data)
#define RUNNER(o)    (*(struct cyt_driver_runner**)(o->data + SZ_INT))
#define CELLENV(o)   (*(struct cyt_cell_env**)(o->data + SZ_INT*2))
#define EXECSTATE(o) (*(struct cyt_exec_state**)(o->data + SZ_INT*3))
#define FUNCVEC(o)   (*(struct Vector_*)(o->data + SZ_INT*4))

#define BUFFER(o)     (*(struct cyt_value_buffer**)o->data)
#define INDEX(o)      (*(m_uint*)(o->data + SZ_INT))
#define BORROWED(o)   (*(m_int*)(o->data + SZ_INT*2))

struct CytosolClosure_ {
  VM_Shred shred;
  VM_Code  code;
};

static CTOR(cytosol_ctor) {
  PROG(o) = cyt_program_new();
  RUNNER(o) = cyt_driver_runner_new();
  CELLENV(o) = cyt_cellenv_new();
  EXECSTATE(o) = cyt_exec_state_new();
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

ANN static void cytosol_cpy(
          const struct cyt_value_buffer *inbuf,
          const size_t inidx,
          struct cyt_value_buffer *outbuf,
          const size_t outidx) {
  const enum cyt_value_type type = cyt_value_get_type(inbuf, inidx);
  switch(type) {
    case CYT_VALUE_TYPE_INTEGER: {
      ptrdiff_t n;
      cyt_value_buffer_get_integer(inbuf, inidx, &n);
      cyt_value_buffer_set_int(outbuf, outidx, n);
      break;
    }
    case CYT_VALUE_TYPE_STRING: {
      const char *str;size_t outlen;
      cyt_value_buffer_get_string(inbuf, inidx, &str, &outlen);
      char name[outlen + 1];
      strncpy(name, str, outlen);
      name[outlen] = '\0';
      cyt_value_buffer_set_string(outbuf, outidx, name);
      break;
    }
    case CYT_VALUE_TYPE_RECORD: {
      struct cyt_value_buffer *_buf;
      cyt_value_buffer_get_record_fields(inbuf, inidx, &_buf);
      cyt_value_buffer_set_record(outbuf, outidx, _buf);
      break;
    }
  }
}

static cyt_value_buffer* cytosol_buffer(Type *types, const M_Object o) {
  const Map map = &o->type_ref->nspc->info->value->map;
  const m_uint sz = map_size(map);
  cyt_value_buffer *buf = cyt_value_buffer_new(sz);
  for(m_uint i = 0; i < sz; i++) {
    const Value value = (Value)map_at(map, i);
    if(isa(value->type, types[et_int]) > 0)
      cyt_value_buffer_set_int(buf, i, *(m_int*)(o->data + i*SZ_INT));
    else if(isa(value->type, types[et_string]) > 0)
      cyt_value_buffer_set_string(buf, i, STRING(*(M_Object*)(o->data + i*SZ_INT)));
    else {
      cyt_value_buffer *_buf = cytosol_buffer(types, *(M_Object*)(o->data + i*SZ_INT));
      cyt_value_buffer_set_record(buf, i, _buf);
    }
  }
  return buf;
}

static MFUN(cytosol_add_record) {
  const m_int quantity = *(m_int*)MEM(SZ_INT);
  const M_Object record = *(M_Object*)MEM(SZ_INT*2);
  struct cyt_record_id out_id = {};
  if(!cyt_program_record_by_name(PROG(o), record->type_ref->name, &out_id))
    Except(shred, "Invalid record Creation");
  cyt_value_buffer *buf = cytosol_buffer(shred->info->vm->gwion->type, record);
  cyt_cellenv_add_record(CELLENV(o), quantity, out_id, buf);
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

ANN static inline m_str cytosol_type(const Gwion gwion, const struct cyt_value_buffer *buf, const size_t i) {
  const enum cyt_value_type type = cyt_value_get_type(buf, i);
  return type == CYT_VALUE_TYPE_INTEGER ?
     "Cytosol.Int" :  type == CYT_VALUE_TYPE_STRING ?
     "Cytosol.String" : "Cytosol.Record";
}

static MFUN(cytosol_value_type) {
  *(enum cyt_value_type*)RETURN = cyt_value_get_type(BUFFER(o), INDEX(o));
}

// we could use Cyt types instead for searching using strings
static M_Object cytosol_array(const Gwion gwion, const VM_Shred shred, const struct cyt_value_buffer* buf) {
  const Type base = str2type(gwion, "Cytosol.Value", (loc_t){});
  const Type t = array_type(gwion->env, base, 1);
  const M_Object array = new_object(gwion->mp, shred, t);
  const size_t n = cyt_value_buffer_get_size(buf);
  M_Vector vec = ARRAY(array) = new_m_vector(gwion->mp, SZ_INT, n);
  cyt_value_buffer *_buf = cyt_value_buffer_new(n);
  for(m_uint i = 0; i < n; i++) {
    const m_str type_name = cytosol_type(gwion, _buf, i);
    const M_Object obj = new_object_str(gwion, NULL, type_name);
    cytosol_cpy(buf, i, _buf, i);
    BUFFER(obj) = _buf;
    INDEX(obj) = i;
    m_vector_set(vec, i, &obj); // beware type
  }
  return array;
}

static M_Object cytosol_fields(const VM_Shred shred, const Gwion gwion, const M_Object record, const struct cyt_value_buffer* buf) {
  const size_t n = cyt_value_buffer_get_size(buf);
  const Map map = &record->type_ref->nspc->info->value->map;
  for(m_uint i = 0; i < n; i++) {
    const enum cyt_value_type type = cyt_value_get_type(buf, i);
    const Value value = (Value)map_at(map, i);
    if(type == CYT_VALUE_TYPE_INTEGER) {
      if(isa(value->type,gwion->type[et_int]) < 0)
        return NULL;
      cyt_value_buffer_get_integer(buf, i, (m_int*)(record->data + i*SZ_INT));
    }
    else if(type == CYT_VALUE_TYPE_STRING) {
      if(isa(value->type,gwion->type[et_string]) < 0)
        return NULL;
      const char* data;
      size_t outlen;
      cyt_value_buffer_get_string(buf, i , &data, &outlen);
      char c[outlen + 1];
      strncpy(c, data, outlen);
      c[outlen] = '\0';
      *(M_Object*)(record->data + i*SZ_INT) = new_string(gwion->mp, NULL, c);
    } else {
      const M_Object o = new_object(gwion->mp, NULL, value->type);
      cyt_value_buffer *out;
      cyt_value_buffer_get_record_fields(buf, i, &out);
      if(!cytosol_fields(shred, gwion, o, out)) {
        _release(o, shred);
        return NULL;
      }
      *(M_Object*)(record->data + i*SZ_INT) = o;
    }
  }
}

static void cytosol_fun(void* data, const struct cyt_value_buffer* buf) {
  struct CytosolClosure_ *closure = (struct CytosolClosure_*)data;
  vmcode_addref(closure->code);
  const VM_Shred shred = new_vm_shred(closure->shred->info->vm->gwion->mp, closure->code);
  shred->base = closure->shred->base;
  *(M_Object*)shred->mem = cytosol_array(closure->shred->info->vm->gwion, shred, buf);
  vm_add_shred(closure->shred->info->vm, shred);
  shredule(closure->shred->tick->shreduler, closure->shred, 0);
}

// C defined func won't work atm
static MFUN(cytosol_set_fun) {
  struct CytosolClosure_ *closure = mp_malloc(shred->info->vm->gwion->mp, CytosolClosure);
  const M_Object name = *(M_Object*)MEM(SZ_INT);
  const VM_Code code = *(VM_Code*)MEM(SZ_INT*2);
  closure->shred = shred;
  closure->code = vmcode_callback(shred->info->vm->gwion->mp, code);
  ++closure->shred->info->me->ref;
  vector_add(&FUNCVEC(o), (m_uint)closure);
  cyt_exec_state_set_extern_function(EXECSTATE(o),
    STRING(name), cytosol_fun, closure);
}

static DTOR(value_dtor) {
  if(!INDEX(o) && !BORROWED(o))
    cyt_value_buffer_destroy(BUFFER(o));
}

static CTOR(value_ctor) {
  BUFFER(o) = cyt_value_buffer_new(1);
}

static CTOR(String_ctor) {
  cyt_value_buffer_set_string(BUFFER(o), 0, "");
}

static CTOR(Record_ctor) {
  cyt_value_buffer_set_record(BUFFER(o), 0, 0);
}

static INSTR(int2value) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(0);
  cyt_value_buffer_get_integer(BUFFER(o), INDEX(o), *(ptrdiff_t**)REG(-SZ_INT));
}

static INSTR(value2int) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  ptrdiff_t i;
  cyt_value_buffer_get_integer(BUFFER(o), INDEX(o), &i);
  *(m_int*)REG(-SZ_INT) = **(m_int**)REG(0) = i;
}

static INSTR(string2value) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(0);
  const M_Object str = *(M_Object*)REG(-SZ_INT);
  cyt_value_buffer_set_string(BUFFER(o), INDEX(o), STRING(str));
}

static INSTR(value2string) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const char* data;
  size_t outlen;
  cyt_value_buffer_get_string(BUFFER(o), INDEX(o), &data, &outlen);
  char c[outlen + 1];
  strncpy(c, data, outlen);
  c[outlen] = '\0';
  *(M_Object*)REG(-SZ_INT) = new_string(shred->info->vm->gwion->mp, shred, c);
}



static INSTR(record2field) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  cyt_value_buffer *buf;
  if(!cyt_value_buffer_get_record_fields(BUFFER(o), INDEX(o), &buf)) {
    cyt_value_buffer_destroy(buf);
    Except(shred, "invalid field index requested");
  }
  // use object if any
  const M_Object record = *(M_Object*)REG(0) ?: new_object(shred->info->vm->gwion->mp, shred, (Type)instr->m_val);
  if(!(*(M_Object*)REG(-SZ_INT) = cytosol_fields(shred, shred->info->vm->gwion, record, buf)))
    Except(shred, "invalid type in field assignment");
}

static OP_EMIT(opem_record2field) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  const Instr pop = emit_add_instr(emit, RegMove);
  pop->m_val = -SZ_INT;
  const Instr instr = emit_add_instr(emit, record2field);
  instr->m_val = (m_uint)bin->rhs->type;
  return GW_OK;
}

static INSTR(field2record) {
//  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const M_Object record = *(M_Object*)REG(0);
  cyt_value_buffer *buf = cytosol_buffer(shred->info->vm->gwion->type, record);
  cyt_value_buffer_set_record(BUFFER(o), INDEX(o), buf);
  *(M_Object*)REG(-SZ_INT) = record;
}

static OP_EMIT(opem_field2record) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  const Instr pop = emit_add_instr(emit, RegMove);
  pop->m_val = -SZ_INT;
  const Instr instr = emit_add_instr(emit, field2record);
  instr->m_val = (m_uint)bin->rhs->type;
  return GW_OK;
}

static INSTR(IntCast) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(cyt_value_get_type(BUFFER(o), INDEX(o)) != CYT_VALUE_TYPE_INTEGER)
    Except(shred, "Invalid cytosol value cast to Int")
}

static INSTR(StringCast) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(cyt_value_get_type(BUFFER(o), INDEX(o)) != CYT_VALUE_TYPE_STRING)
    Except(shred, "Invalid cytosol value cast to String")
}

static INSTR(RecordCast) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(cyt_value_get_type(BUFFER(o), INDEX(o)) != CYT_VALUE_TYPE_RECORD)
    Except(shred, "Invalid cytosol value cast to Record")
}

static Type cytosol_stmt_list(const Env env, const Type fields, Stmt_List list) {
  while(list) {
    Stmt stmt = list->stmt;
    if (stmt->stmt_type == ae_stmt_exp &&
        stmt->d.stmt_exp.val->exp_type == ae_exp_decl &&
        !stmt->d.stmt_exp.val->d.exp_decl.list->next) {
      const Type type = stmt->d.stmt_exp.val->d.exp_decl.list->self->value->type;
      if(isa(type, env->gwion->type[et_int]) < 0 &&
        isa(type, env->gwion->type[et_string]) < 0 &&
        isa(type, fields) < 0)
      ERR_N(stmt->d.stmt_exp.val->pos, "invalid type '%s' in Cytosol.field", type->name)
   } else
      ERR_N(stmt->pos, "invalid stmt in Cytosol.field")
    list = list->next;
  }
}

static OP_CHECK(opck_fields_check) {
  const Class_Def cdef = (Class_Def)data;
  const Map map = &cdef->base.type->nspc->info->value->map;
  Ast ast = cdef->body;
  const Section * section = ast->section;
  if (ast->next || section->section_type != ae_section_stmt)
    ERR_N(cdef->pos, "Invalid section in Cytosol.Field");
  const Type fields = cdef->base.type->info->parent;
  CHECK_NN(cytosol_stmt_list(env, fields, section->d.stmt_list))
  SET_FLAG(cdef->base.type, abstract | ae_flag_final);
  return cdef->base.type;
}

static OP_CHECK(opck_fields_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  Exp arg = call->args;
  CHECK_NN(check_exp(env, arg))
  const Map map = &actual_type(env->gwion, call->func->type)->nspc->info->value->map;
  m_uint i = 0;
  while(arg) {
    const Value value = (Value)map_at(map, i);
    if(isa(arg->type , value->type) < 0)
       ERR_N(arg->pos, _("invalid type '%s' in '%s' construtor (expected '%s')"),
            arg->type->name, call->func->type->name, value->type->name)
    i++;
    arg = arg->next;
  }
  return actual_type(env->gwion, call->func->type);
}

static INSTR(FieldCtor) {
  POP_REG(shred, instr->m_val);
  const M_Object o = new_object(shred->info->vm->gwion->mp, shred, (Type)instr->m_val2);
  memcpy(o->data, REG(-SZ_INT), instr->m_val);
  *(M_Object*)REG(-SZ_INT) = o;
}

static OP_EMIT(opem_fields_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  const Map map = &actual_type(emit->gwion, call->func->type)->nspc->info->value->map;
  const size_t sz = map_size(map);
  for(m_uint i = 0; i < sz; i++) {
    const Value value = (Value)map_at(map, i);
    if(isa(value->type, emit->gwion->type[et_object]) > 0)
      emit_object_addref(emit, (-sz + i - 1) * SZ_INT, 0);
  }
  const Instr instr = emit_add_instr(emit, FieldCtor);
  instr->m_val = sz * SZ_INT;
  instr->m_val2 = (m_uint)actual_type(emit->gwion, call->func->type);
  return GW_OK;
}

GWION_IMPORT(Cytosol) {
  DECL_OB(const Type, t_cytosol, = gwi_class_ini(gwi, "Cytosol", "Object"))
  gwi_class_xtor(gwi, cytosol_ctor, cytosol_dtor);

  GWI_BB(gwi_enum_ini(gwi, "Type"))
  GWI_BB(gwi_enum_add(gwi, "int_t", 0))
  GWI_BB(gwi_enum_add(gwi, "string_t", 0))
  GWI_BB(gwi_enum_add(gwi, "record_t", 0))
  GWI_BB(gwi_enum_end(gwi))
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
    gwi_class_xtor(gwi, value_ctor, value_dtor);
    GWI_BB(gwi_item_ini(gwi, "@internal", "value"))
    GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
    GWI_BB(gwi_item_ini(gwi, "@internal", "@index"))
    GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
    GWI_BB(gwi_item_ini(gwi, "@internal", "@borrowed"))
    GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
    GWI_BB(gwi_func_ini(gwi, "Cytosol.Type", "type"))
    GWI_BB(gwi_func_end(gwi, cytosol_value_type, ae_flag_none))
    GWI_BB(gwi_class_end(gwi))
    SET_FLAG(t_value, abstract);

    DECL_OB(const Type, t_int, = gwi_class_ini(gwi, "Int", "Value"))
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_string, = gwi_class_ini(gwi, "String", "Value"))
    gwi_class_xtor(gwi, String_ctor, NULL);
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_record, = gwi_class_ini(gwi, "Record", "Value"))
    gwi_class_xtor(gwi, Record_ctor, NULL);
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_fields, = gwi_class_ini(gwi, "Fields", "Object"))
    SET_FLAG(t_fields, abstract);
    GWI_BB(gwi_class_end(gwi))

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
  GWI_BB(gwi_func_arg(gwi, "int", "quantity"))
  GWI_BB(gwi_func_arg(gwi, "Fields", "fields"))
  GWI_BB(gwi_func_end(gwi, cytosol_add_record, ae_flag_none))

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
  GWI_BB(gwi_oper_end(gwi, "=>", int2value))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Int", "int", "int"))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "=>", value2int))

  GWI_BB(gwi_oper_ini(gwi, "string", "Cytosol.String", "string"))
  GWI_BB(gwi_oper_end(gwi, "=>", string2value))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.String", "string", "string"))
  GWI_BB(gwi_oper_end(gwi, "=>", value2string))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Fields", "Cytosol.Record", "Cytosol.Fields"))
  GWI_BB(gwi_oper_emi(gwi, opem_field2record))
  GWI_BB(gwi_oper_end(gwi, "=>", field2record))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Record", "Cytosol.Fields", "Cytosol.Fields"))
  GWI_BB(gwi_oper_emi(gwi, opem_record2field))
  GWI_BB(gwi_oper_end(gwi, "=>", record2field))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Value", "Cytosol.Int", "Cytosol.Int"))
  GWI_BB(gwi_oper_end(gwi, "$", IntCast))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Value", "Cytosol.String", "Cytosol.String"))
  GWI_BB(gwi_oper_end(gwi, "$", StringCast))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Value", "Cytosol.Record", "Cytosol.Record"))
  GWI_BB(gwi_oper_end(gwi, "$", RecordCast))

  GWI_BB(gwi_oper_ini(gwi, "Cytosol.Fields", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fields_check))
  GWI_BB(gwi_oper_end(gwi, "@class_check", NULL))

  GWI_BB(gwi_oper_ini(gwi, NULL, "Cytosol.Fields", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_fields_ctor))
  GWI_BB(gwi_oper_emi(gwi, opem_fields_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  return GW_OK;
}
