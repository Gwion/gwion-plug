#include "simdjson.h"
#include "Gwion.hh"
#include "tojson.hh"
extern "C" {
#include "traverse.h"
#include "gwi.h"
#include "gack.h"
}

using namespace simdjson;
#include "hydrate.hh"

struct GwSimdJson {
  dom::parser *parser;
  dom::element element;
};
#define SIMDJSON(a)     (&*(struct GwSimdJson*)((a)->data))
#define SIMDJSON_OBJ(a) (**(dom::object**)((a)->data))
#define SIMDJSON_ARR(a) (**(dom::array**)((a)->data))

static DTOR(simdjson_dtor) {
  delete SIMDJSON(o)->parser;
}

static DTOR(simdjson_obj_dtor) {
  delete *(dom::object**)o->data;
}

static DTOR(simdjson_arr_dtor) {
  delete *(dom::array**)o->data;
}

ANN static void object_pp(std::string *str, dom::object elem, uint indent);
ANN static void array_pp(std::string *str, dom::array elem, uint indent);

ANN static void element_pp(std::string *str, dom::element value, uint indent) {
    switch (value.type()) {
      case dom::element_type::INT64:
      case dom::element_type::UINT64:
        str->append(std::to_string(value.get_int64()));
        break;
      case dom::element_type::BOOL:
        str->append(value.get_bool() ? "true" : "false");
        break;
      case dom::element_type::DOUBLE:
        str->append(std::to_string(value.get_double()));
        break;
      case dom::element_type::STRING:
        str->append(value.get_c_str());
        break;
      case dom::element_type::ARRAY:
        array_pp(str, value, indent);
        break;
      case dom::element_type::OBJECT:
        object_pp(str, value, indent);
        break;
      case dom::element_type::NULL_VALUE:
        str->append("null");
        break;
     }
}

ANN static void do_indent(std::string *str, const uint indent) {
  str->append(indent * 2, ' ');
}

ANN static void array_pp(std::string *str, dom::array elem, uint indent) {
  do_indent(str, indent);
  str->append("[\n");
  indent++;
  const size_t sz = elem.size();
  size_t i = 0;
  for(auto a : elem) {
    do_indent(str, indent);
    element_pp(str, a, indent);
    if(++i < sz)
      str->append(",");
    str->append("\n");
  }
  indent--;
  do_indent(str, indent);
  str->append("]");
}

ANN static void object_pp(std::string *str, dom::object elem, uint indent) {
  if(indent > 1)
    do_indent(str, indent);
  str->append("{\n");
  indent++;
  const size_t sz = elem.size();
  size_t i = 0;
  for(auto a : elem) {
    do_indent(str, indent);
    str->append("\"");
    str->append(a.key);
    str->append("\": ");
    element_pp(str, a.value, indent);
     if(++i < sz)
       str->append(",");
     str->append("\n");
  }
  indent--;
  do_indent(str, indent);
//  str->append("}\n");
  str->append("}");
}

static MFUN(simdjson_pp) {
  std::string str;
  element_pp(&str, SIMDJSON(o)->element, 0);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str.data());
}

static MFUN(simdjson_obj_pp) {
  std::string str;
  object_pp(&str, SIMDJSON_OBJ(o), 0);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str.data());
}

static MFUN(simdjson_arr_pp) {
  std::string str;
  array_pp(&str, SIMDJSON_ARR(o), 0);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str.data());
}

static SFUN(simdjson_load) {
  const M_Object arg = *(M_Object *)MEM(0);
  const VM_Code code = *(VM_Code *)REG(SZ_INT);
  const M_Object ret = new_object(shred->info->mp, code->ret_type);
  *(M_Object *)RETURN = ret;
  try {
    auto s = SIMDJSON(ret);
    s->parser = new dom::parser();
    s->element = s->parser->load(STRING(arg));
  } catch (const simdjson_error &e) {
    handle(shred, (m_str)"SimdJSONLoad");
  }
}

static SFUN(simdjson_parse) {
  const M_Object arg = *(M_Object *)MEM(0);
  const VM_Code code = *(VM_Code *)REG(SZ_INT);
  const M_Object ret = new_object(shred->info->mp, code->ret_type);
  *(M_Object *)RETURN = ret;
  try {
    auto s = SIMDJSON(ret);
    s->parser = new dom::parser();
    s->element = s->parser->parse(STRING(arg), strlen(STRING(arg)));
  } catch (const simdjson_error &e) {
    handle(shred, (m_str)"SimdJSONParse");
  }
}

static MFUN(simdjson_new) {
  const M_Object arg = *(M_Object *)MEM(SZ_INT);
  *(M_Object *)RETURN = o;
  try {
    auto s = SIMDJSON(o);
    s->parser = new dom::parser();
    s->element = s->parser->parse(STRING(arg), strlen(STRING(arg)));
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
    handle(shred, (m_str)"SimdJSONDocumentInvalid");     \
  }                                              \
}

getx(i,
    int64_t value = SIMDJSON(o)->element[str];
    *(m_int*)RETURN = value;)
getx(b,
    bool value = SIMDJSON(o)->element[str];
    *(m_int*)RETURN = value;)
getx(f,
    double value = SIMDJSON(o)->element[str];
    *(m_float*)RETURN = value;)
getx(s,
    std::string_view view = SIMDJSON(o)->element[str];
    std::string value(view);
    *(M_Object*)RETURN = new_string(shred->info->vm->gwion, (m_str)value.data());)
getx(o,
   dom::object value = SIMDJSON(o)->element[str];
   dom::object *val = new dom::object(value);
   M_Object ret = new_object(shred->info->mp, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(dom::object**)ret->data = val;
   *(M_Object*)RETURN = ret;)
getx(a,
   dom::array value = SIMDJSON(o)->element[str];
   dom::array *val = new dom::array(value);
   M_Object ret = new_object(shred->info->mp, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(dom::array**)ret->data = val;
   *(M_Object*)RETURN = ret;)

#define obj_getx(name, body)                     \
static MFUN(simdjson_obj_get##name) {            \
  const M_Object arg = *(M_Object *)MEM(SZ_INT); \
  const char *str = STRING(arg);                 \
  try {                                          \
    body                                         \
  } catch (const simdjson_error &e) {            \
    handle(shred, (m_str)"SimdJSONObjectInvalid");     \
  }                                              \
}

obj_getx(i,
    int64_t value = SIMDJSON_OBJ(o)[str];
    *(m_int*)RETURN = value;)
obj_getx(b,
    bool value = SIMDJSON_OBJ(o)[str];
    *(m_int*)RETURN = value;)
obj_getx(f,
    double value = SIMDJSON_OBJ(o)[str];
    *(m_float*)RETURN = value;)
obj_getx(s,
    std::string_view view = SIMDJSON_OBJ(o)[str];
    std::string value(view);
    *(M_Object*)RETURN = new_string(shred->info->vm->gwion, (m_str)value.data());)
obj_getx(o,
   dom::object value = SIMDJSON_OBJ(o)[str];
   dom::object *val = new dom::object(value);
   M_Object ret = new_object(shred->info->mp, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(dom::object**)ret->data = val;
   *(M_Object*)RETURN = ret;)
obj_getx(a,
   dom::array value = SIMDJSON_OBJ(o)[str];
   dom::array *val = new dom::array(value);
   M_Object ret = new_object(shred->info->mp, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(dom::array**)ret->data = val;
   *(M_Object*)RETURN = ret;)

#define arr_getx(name, body)                      \
static MFUN(simdjson_arr_get##name) {             \
  const size_t idx = *(m_int *)MEM(SZ_INT);       \
  try {                                           \
    auto iter = SIMDJSON_ARR(o).begin();          \
    m_uint i = 0;                                 \
    while(iter != SIMDJSON_ARR(o).end()) {        \
      if(i == idx) {                              \
        body                                      \
      }                                           \
      i++;                                        \
      ++iter;                                     \
    }                                             \
  } catch (const simdjson_error &e) {             \
    handle(shred, (m_str)"SimdJSONArrayInvalid"); \
  }                                               \
}

arr_getx(i,
    const int64_t value = *iter;
    *(m_int*)RETURN = value;)
arr_getx(b,
    const bool value = *iter;
    *(m_int*)RETURN = value;)
arr_getx(f,
    const double value = *iter;
    *(m_float*)RETURN = value;)
arr_getx(s,
    std::string_view view = *iter;
    std::string value(view);
    *(M_Object*)RETURN = new_string(shred->info->vm->gwion, (m_str)value.data());)
arr_getx(o,
   dom::object value = *iter;
   dom::object *val = new dom::object(value);
   M_Object ret = new_object(shred->info->mp, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(dom::object**)ret->data = val;
   *(M_Object*)RETURN = ret;)
arr_getx(a,
   dom::array value = *iter;
   dom::array *val = new dom::array(value);
   M_Object ret = new_object(shred->info->mp, (*(VM_Code*)REG(SZ_INT*2))->ret_type);
   *(dom::array**)ret->data = val;
   *(M_Object*)RETURN = ret;)

static MFUN(Hydrate) {
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
  try {
    hydrate(shred->info->vm->gwion, shred, SIMDJSON(o)->element, code->ret_type, (m_bit*)RETURN);
  } catch (const simdjson_error &e) {
    handle(shred, (m_str)"JsonHydrateError");
  }
}

static SFUN(ToJson) {
  std::stringstream str;
  tojson(shred->info->vm->gwion, &str, *(M_Object*)MEM(0));
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str.str().data());
}

// return an Event:[T]
// we need to shreduler_remove false
// when creating an object
// get the constructor code (emit all that?)
// reshredule shred at the end of code
// when back to original shred broadcast the event
// what to do about the result?
// it should only be available at the end of operations

extern "C" {

static OP_CHECK(opck_simdjson_get_at) {
  Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data));
  // check_the expression has the good type
  if (bin->lhs->exp_type != ae_exp_call ||
      bin->lhs->d.exp_call.func->exp_type != ae_exp_dot)
    ERR_N(bin->lhs->pos, (m_str)_("invalid expression for SimdJSON.get"));
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
  else ERR_N(bin->lhs->pos, (m_str)_("unhandled type for SimdJSON.get"));
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

    DECL_OB(const Type, t_simdjson_arr, = gwi_class_ini(gwi, "array", "Object"));
    t_simdjson_arr->nspc->offset += SZ_INT;
    SET_FLAG(t_simdjson_arr, abstract);
  gwi_class_xtor(gwi, NULL, simdjson_arr_dtor);
    GWI_BB(gwi_class_end(gwi))

// make it a struct or a type
    DECL_OB(const Type, t_simdjson_obj, = gwi_class_ini(gwi, "object", "Object"));
    t_simdjson_obj->nspc->offset += SZ_INT;
    SET_FLAG(t_simdjson_obj, abstract);

//    set_tflag(t_simdjson_obj, tflag_infer);
  gwi_class_xtor(gwi, NULL, simdjson_obj_dtor);
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

  GWI_BB(gwi_func_ini(gwi, "string", "pp"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_obj_pp, ae_flag_none))

    GWI_BB(gwi_class_end(gwi))

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

  GWI_BB(gwi_func_ini(gwi, "SimdJSON", "parse"))
  GWI_BB(gwi_func_arg(gwi, "string", "json_string"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_parse, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "SimdJSON", "new"))
  GWI_BB(gwi_func_arg(gwi, "string", "data"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_new, ae_flag_none))

  // now extending array
  const m_uint scope = env_push_type(gwi->gwion->env, t_simdjson_arr);
  GWI_BB(gwi_func_ini(gwi, "Get", "get"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "get_i"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_geti, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "bool", "get_b"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_getb, ae_flag_none))

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

  GWI_BB(gwi_func_ini(gwi, "string", "pp"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_arr_pp, ae_flag_none))

  env_pop(gwi->gwion->env, scope);

  GWI_BB(gwi_func_ini(gwi, "string", "pp"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_pp, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "T", "hydrate:[T]"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)Hydrate, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "string", "tojson"))
  GWI_BB(gwi_func_arg(gwi, "Object", "o"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)ToJson, ae_flag_static))

  GWI_BB(gwi_class_end(gwi))
// could be a type

/*
// maybe we can use it to check get()
// where it is incorrect
// or not assigned to smth
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
// T hydrate:[T : Codable]() or smth
//   then we need a @trait_check operator
//   could be used in Cytosol too
// error messages
// effects
// use func_check_post?
