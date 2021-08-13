#include "simdjson.h"
#include "Gwion.hh"
extern "C" {
#include "traverse.h"
#include "gwi.h"
#include "gack.h"
}

using namespace simdjson;

struct GwSimdJson {
  ondemand::parser *parser;
  ondemand::document document;
  padded_string *str;
};
#define SIMDJSON(a)     (*(struct GwSimdJson*)((a)->data))
#define SIMDJSON_OBJ(a) (**(ondemand::object**)((a)->data))
#define SIMDJSON_ARR(a) ( *(ondemand::array**)((a)->data))

static DTOR(simdjson_dtor) {
  delete SIMDJSON(o).parser;
  delete SIMDJSON(o).str;
}

static DTOR(simdjson_obj_dtor) {
  delete *(ondemand::object**)o->data;
}

static GACK(simdjson_obj_gack) {
  const M_Object o = *(M_Object*)VALUE;
  ondemand::object obj = **(ondemand::object**)o->data;
  INTERP_PRINTF("json object");
}
static SFUN(simdjson_load) {
  const M_Object arg = *(M_Object *)MEM(0);
  const VM_Code code = *(VM_Code *)REG(SZ_INT);
  const M_Object ret = new_object(shred->info->mp, shred, code->ret_type);
  *(M_Object *)RETURN = ret;
  vector_add(&shred->gc, (m_uint)ret);
  try {
    auto s = &SIMDJSON(ret);
    s->parser = new ondemand::parser();
    padded_string base = padded_string::load(STRING(arg));
    s->str = new padded_string(std::string_view(base));
    s->document = s->parser->iterate(*s->str);
  } catch (const simdjson_error &e) {
    handle(shred, (m_str)"SimdJSONOpen");
  }
}

static MFUN(simdjson_new) {
  const M_Object arg = *(M_Object *)MEM(SZ_INT);
  *(M_Object *)RETURN = o;
  try {
    auto s = &SIMDJSON(o);
    s->parser = new ondemand::parser();
    s->str = new padded_string(std::string_view(STRING(arg)));
    s->document = s->parser->iterate(*s->str);
  } catch (const simdjson_error &e) {
    handle(shred, (m_str)"SimdJSONOpen");
  }
}

#define getx(name, body)                         \
static MFUN(simdjson_get##name) {                \
  const M_Object arg = *(M_Object *)MEM(SZ_INT); \
  const char *str = STRING(arg);                 \
  try {                                          \
    body                                         \
  } catch (const simdjson_error &e) {            \
    handle(shred, (m_str)"SimdJSONInvalid");     \
  }                                              \
}

getx(i,
    int64_t value = SIMDJSON(o).document[str];
    *(m_int*)RETURN = value;);
getx(b,
    bool value = SIMDJSON(o).document[str];
    *(m_int*)RETURN = value;);
getx(f,
    double value = SIMDJSON(o).document[str];
    *(m_float*)RETURN = value;);
getx(s,
    std::string_view view = SIMDJSON(o).document[str];
    std::string value(view);
    *(M_Object*)RETURN = new_string2(shred->info->vm->gwion, shred, (m_str)value.data()););
getx(o,
   ondemand::object value = value = SIMDJSON(o).document[str];
   ondemand::object *val = new ondemand::object(value);
   M_Object ret = new_object(shred->info->mp, shred, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(ondemand::object**)ret->data = val;
   *(M_Object*)RETURN = ret;);
getx(a,
   ondemand::array value = value = SIMDJSON(o).document[str];
   ondemand::array *val = new ondemand::array(value);
   M_Object ret = new_object(shred->info->mp, shred, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(ondemand::array**)ret->data = val;
   *(M_Object*)RETURN = ret;);

#define obj_getx(name, body)                     \
static MFUN(simdjson_obj_get##name) {            \
  const M_Object arg = *(M_Object *)MEM(SZ_INT); \
  const char *str = STRING(arg);                 \
  try {                                          \
    body                                         \
  } catch (const simdjson_error &e) {            \
    handle(shred, (m_str)"SimdJSONInvalid");     \
  }                                              \
}

obj_getx(i,
    int64_t value = SIMDJSON_OBJ(o)[str];
    *(m_int*)RETURN = value;);
obj_getx(b,
    bool value = SIMDJSON_OBJ(o)[str];
    *(m_int*)RETURN = value;);
obj_getx(f,
    double value = SIMDJSON_OBJ(o)[str];
    *(m_float*)RETURN = value;);
obj_getx(s,
    std::string_view view = SIMDJSON_OBJ(o)[str];
    std::string value(view);
    *(M_Object*)RETURN = new_string2(shred->info->vm->gwion, shred, (m_str)value.data()););
obj_getx(o,
   ondemand::object value = value = SIMDJSON_OBJ(o)[str];
   ondemand::object *val = new ondemand::object(value);
   M_Object ret = new_object(shred->info->mp, shred, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(ondemand::object**)ret->data = val;
   *(M_Object*)RETURN = ret;);
obj_getx(a,
   ondemand::array value = value = SIMDJSON_OBJ(o)[str];
   ondemand::array *val = new ondemand::array(value);
   M_Object ret = new_object(shred->info->mp, shred, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(ondemand::array**)ret->data = val;
   *(M_Object*)RETURN = ret;);

/*
#define arr_getx(name, body)                     \
static MFUN(simdjson_arr_get##name) {            \
  const size_t idx = *(m_int *)MEM(SZ_INT);      \
  try {                                          \
    body                                         \
  } catch (const simdjson_error &e) {            \
    handle(shred, (m_str)"SimdJSONInvalid");     \
  }                                              \
}

arr_getx(i,
   for(auto a : SIMDJSON_ARR(o)) {
//     if()
   }
//    int64_t value = SIMDJSON_ARR(o)[idx];
    *(m_int*)RETURN = value;);
arr_getx(b,
    bool value = SIMDJSON_ARR(o)->at(idx);
    *(m_int*)RETURN = value;);
arr_getx(f,
    double value = SIMDJSON_ARR(o)->at(idx);
    *(m_float*)RETURN = value;);
arr_getx(s,
    std::string_view view = SIMDJSON_ARR(o).at(idx);
    std::string value(view);
    *(M_Object*)RETURN = new_string2(shred->info->vm->gwion, shred, (m_str)value.data()););
arr_getx(o,
   ondemand::object value = value = SIMDJSON_ARR(o)[idx];
   ondemand::object *val = new ondemand::object(value);
   M_Object ret = new_object(shred->info->mp, shred, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(ondemand::object**)ret->data = val;
   *(M_Object*)RETURN = ret;);
arr_getx(a,
//   ondemand::array value = value = SIMDJSON_ARR(o)[idx];
   ondemand::array value = value = SIMDJSON_ARR(o)->at(idx);
   ondemand::array *val = new ondemand::array(value);
   M_Object ret = new_object(shred->info->mp, shred, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(ondemand::array**)ret->data = val;
   *(M_Object*)RETURN = ret;);
*/
extern "C" {
static OP_CHECK(opck_simdjson_get_at) {
  Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data));
  // check_the expression has the good type
  if (bin->lhs->exp_type != ae_exp_call)exit(12);
  if (bin->lhs->d.exp_call.func->exp_type != ae_exp_dot)exit(13);
  bin->lhs->d.exp_call.func->type = NULL;
  Exp_Dot *const dot = &bin->lhs->d.exp_call.func->d.exp_dot;
  if(isa(bin->rhs->type, env->gwion->type[et_bool]) > 0)
    dot->xid = insert_symbol(env->gwion->st, (m_str)"get_b");
  else if(isa(bin->rhs->type, env->gwion->type[et_int]) > 0)
    dot->xid = insert_symbol(env->gwion->st, (m_str)"get_i");
  else if(isa(bin->rhs->type, env->gwion->type[et_float]) > 0)
    dot->xid = insert_symbol(env->gwion->st, (m_str)"get_f");
  else if(isa(bin->rhs->type, env->gwion->type[et_string]) > 0)
    dot->xid = insert_symbol(env->gwion->st, (m_str)"get_s");
  else if(!strcmp(bin->rhs->type->name, "object"))
    dot->xid = insert_symbol(env->gwion->st, (m_str)"get_o");
  else if(!strcmp(bin->rhs->type->name, "array"))
    dot->xid = insert_symbol(env->gwion->st, (m_str)"get_a");
  else exit(12);
  CHECK_ON(check_exp(env, bin->lhs->d.exp_call.func));
  return bin->lhs->type = bin->lhs->d.exp_call.func->type->info->func->def->base->ret_type;
}

GWION_IMPORT(SimdJSON) {
  DECL_OB(const Type, t_simdjson, = gwi_class_ini(gwi, "SimdJSON", "Object"));
  t_simdjson->nspc->offset += sizeof(GwSimdJson);
  gwi_class_xtor(gwi, NULL, simdjson_dtor);

    DECL_OB(const Type, t_simdjson_get, = gwi_class_ini(gwi, "Get", "Object"));
    SET_FLAG(t_simdjson_get, abstract);
    set_tflag(t_simdjson_get, tflag_infer);
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_simdjson_array, = gwi_class_ini(gwi, "array", "Object"));
    set_tflag(t_simdjson_array, tflag_infer);
    GWI_BB(gwi_class_end(gwi))

// make it a struct or a type
    DECL_OB(const Type, t_simdjson_obj, = gwi_class_ini(gwi, "object", "Object"));
  t_simdjson_obj->nspc->offset += SZ_INT;
    SET_FLAG(t_simdjson_obj, abstract);
//    set_tflag(t_simdjson_obj, tflag_infer);
  gwi_class_xtor(gwi, NULL, simdjson_obj_dtor);
  gwi_gack(gwi, t_simdjson_obj, simdjson_obj_gack);
  GWI_BB(gwi_func_ini(gwi, "Get", "get"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "bool", "get_b"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_obj_getb, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "get_i"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_obj_geti, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "float", "get_f"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_obj_getf, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "string", "get_s"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_obj_gets, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "object", "get_o"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_obj_geto, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "array", "get_a"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_obj_geta, ae_flag_none))

    GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, (m_str)"SimdJSON.Get", (m_str)OP_ANY_TYPE, NULL));
  GWI_BB(gwi_oper_add(gwi, opck_simdjson_get_at));
  GWI_BB(gwi_oper_end(gwi, (m_str)"=>", NULL));

  GWI_BB(gwi_func_ini(gwi, "Get", "get"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "bool", "get_b"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_getb, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "get_i"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_geti, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "float", "get_f"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_getf, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "string", "get_s"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_gets, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "object", "get_o"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_geto, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "array", "get_a"))
  GWI_BB(gwi_func_arg(gwi, "string", "name"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_geta, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "SimdJSON", "load"))
  GWI_BB(gwi_func_arg(gwi, "string", "json_file"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_load, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "SimdJSON", "new"))
  GWI_BB(gwi_func_arg(gwi, "string", "data"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_new, ae_flag_none))
/*
  // now extending array
  const m_uint scope = env_push_type(gwi->gwion->env, t_simdjson_array);
  GWI_BB(gwi_func_ini(gwi, "Get", "get"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "bool", "get_b"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_getb, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "get_i"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_geti, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "float", "get_f"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_getf, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "string", "get_s"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_gets, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "object", "get_o"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_geto, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "array", "get_a"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_geta, ae_flag_none))

  env_pop(gwi->gwion->env, scope);
*/
  GWI_BB(gwi_class_end(gwi))
// could be a type

/*
// maybe we can use it to check get()B
  const Func             f      = (Func)vector_front(&t_simdjson->nspc->vtable);
  const struct Op_Func   opfunc = {.ck = opck_simdjson_get};
  const struct Op_Import opi    = {
      .lhs  = NULL,
      .rhs  = f->value_ref->type,
      .ret = NULL,
      .func = &opfunc,
      .data = (uintptr_t)f,
      .nspc = NULL,
      .op   = insert_symbol(gwi->gwion->st, (m_str)"@func_check"),
      .pos  = gwi->loc
  };
  CHECK_BB(add_op(gwi->gwion, &opi));
*/
  GWI_BB(gwi_oper_ini(gwi, (m_str)"SimdJSON.Get", (m_str)OP_ANY_TYPE, NULL));
  GWI_BB(gwi_oper_add(gwi, opck_simdjson_get_at));
  GWI_BB(gwi_oper_end(gwi, (m_str)"=>", NULL));
  return GW_OK;
}
}

// error messages
// effects
