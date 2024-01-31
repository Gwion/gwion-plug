#include "simdjson.h"
#include "Gwion.hh"
#include "tojson.hh"
extern "C" {
#include "traverse.h"
#include "gwi.h"
#include "gack.h"
#include "looper.h"
#include "emit.h"
}

using namespace simdjson;
#include "hydrate.hh"
static struct Vector_ parser_vector; 
#define JSON_ELEMENT(a)     (*(dom::element*)((a)->data))
#define JSON_OBJECT(a) (*(dom::object*)((a)->data))
#define JSON_ARRAY(a) (*(dom::array*)((a)->data))

static DTOR(json_dtor) {
  // unref parser
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
  str->append("}");
}

static MFUN(json_elem_pp) {
  std::string str;
  element_pp(&str, JSON_ELEMENT(o), 0);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str.data());
}

static MFUN(json_object_pp) {
  std::string str;
  object_pp(&str, JSON_OBJECT(o), 0);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str.data());
}

static MFUN(json_array_pp) {
  std::string str;
  array_pp(&str, JSON_ARRAY(o), 0);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str.data());
}

static dom::parser *get_parser(const M_Object o) {
  auto parser = new dom::parser();
  *(uint32_t*)(o->data + sizeof(dom::element)) = vector_size(&parser_vector);
  vector_add(&parser_vector, (vtype)parser);
  return parser;
}

ANN static void load(void *data) {
  const VM_Shred shred = (VM_Shred)data;
  const M_Object arg = *(M_Object *)MEM(0);
  const VM_Code code = *(VM_Code *)REG(SZ_INT);
  const M_Object ret = new_object(shred->info->mp, code->ret_type);
  shred->reg += SZ_INT;
  *(M_Object *)(shred->reg - SZ_INT) = ret;
  try {
    auto parser = get_parser(ret);
    JSON_ELEMENT(ret) = parser->load(STRING(arg));
  } catch (const simdjson_error &e) {
    xfun_handle(shred, (m_str)"SimdJSONLoad");
    return;
  }
  shredule(shred->tick->shreduler, shred, 0);
}

static SFUN(simdjson_load) {
  shred_pool_run(shred, load, shred);
}

ANN static void _parse(void *data) {
  const VM_Shred shred = (VM_Shred)data;
  const M_Object arg = *(M_Object *)MEM(0);
  const VM_Code code = *(VM_Code *)REG(SZ_INT);
  const M_Object ret = new_object(shred->info->mp, code->ret_type);
  shred->reg += SZ_INT;
  *(M_Object *)(shred->reg - SZ_INT) = ret;
  try {
    auto parser = get_parser(ret);
    JSON_ELEMENT(ret) = parser->parse(STRING(arg), strlen(STRING(arg)));
  } catch (const simdjson_error &e) {
    xfun_handle(shred, (m_str)"SimdJSONParse");
    return;
  }
}

static SFUN(simdjson_parse) {
  shred_pool_run(shred, _parse, shred);
}

#define prim2gw(ctype, gwtype) \
ANN static gwtype ctype##2gw(const M_Object base NUSED, const VM_Shred shred NUSED, \
    dom::element e, const uint offset NUSED) {                                      \
  ctype value = e;                                                                  \
  return value;                                                                     \
}
prim2gw(bool, m_int);
prim2gw(int64_t, m_int);
prim2gw(double, m_float);

ANN static M_Object string2gw(const M_Object o NUSED, const VM_Shred shred,
    dom::element e, const uint offset NUSED) {
  std::string_view view = e;
  std::string value(view);
  return new_string(shred->info->vm->gwion, (m_str)value.data());
}
// works on the assumption object array and element are the same size
#define json2gw(name, type)                                                          \
ANN static M_Object name##2gw(const M_Object base, const VM_Shred shred,             \
    const dom::element e, const uint offset) {                                       \
  const VM_Code code = *(VM_Code*)REG(offset);                                       \
  const M_Object o = new_object(shred->info->mp, code->ret_type);                    \
  *(type*)o->data = e;                                                               \
  *(uint32_t*)(o->data + sizeof(type)) = *(uint32_t*)(base->data + sizeof(type));    \
  return o;                                                                          \
}
json2gw(obj, dom::object);
json2gw(arr, dom::array);
json2gw(element, dom::element);

#define anyx(def)                    \
def(bool, m_int, bool2gw)          \
def(int, m_int, int64_t2gw)        \
def(float, m_float, double2gw)     \
def(string, M_Object, string2gw)    \
def(object, M_Object, obj2gw)       \
def(array, M_Object, arr2gw)        \
def(element, M_Object, element2gw)

#define json_access(target, func, prelude, name, type, element, offset) \
static MFUN(target##_##name) {                                          \
  prelude                                                               \
  try {                                                                 \
    *(type*)RETURN = func(o, shred, element, offset);                   \
  } catch (const simdjson_error &e) {                                   \
    xfun_handle(shred, (m_str)"SimdJSONInvalidAccess");                 \
  }                                                                     \
}

#define _asx(name, type, func) json_access(json_element, func,, as##name, type, JSON_ELEMENT(o), SZ_INT);
#define _getx(name, type, func) json_access(json_object, func, const char *str = STRING(*(M_Object*)MEM(SZ_INT));, get##name, type, JSON_OBJECT(o)[str], SZ_INT*2);
#define _atx(name, type, func) json_access(json_array, func, const m_int idx = *(m_int*)MEM(SZ_INT);, at##name, type, JSON_ARRAY(o).at(idx), SZ_INT*2);
anyx(_asx)
anyx(_getx)
anyx(_atx)

static MFUN(json_array_size) {
  size_t value = JSON_ARRAY(o).size();
  *(m_int*)RETURN = value;
}

static MFUN(json_element_type) {
  *(m_int*)RETURN = (m_int)JSON_ELEMENT(o).type();
}
static MFUN(Hydrate) {
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
  try {
    hydrate(shred->info->vm->gwion, shred, JSON_ELEMENT(o), code->ret_type, (m_bit*)RETURN);
  } catch (const simdjson_error &e) {
    xfun_handle(shred, (m_str)"JsonHydrateError");
  }
}

static SFUN(ToJson) {
  std::stringstream str;
  tojson(shred->info->vm->gwion, &str, *(M_Object*)MEM(0));
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str.str().data());
}

static INSTR(getoff) {
  const M_Object o = *(M_Object*)MEM(instr->m_val + SZ_INT);
  dom::array array = JSON_ARRAY(o);
  *(m_uint*)MEM(instr->m_val) = array.size();
  exit(21);
}

static OP_CHECK(opck_json_array_each_val) {
  Exp* exp = (Exp*)data;
  const Type t = str2type(env->gwion, (m_str)"SimdJSON.element", exp->loc);
  return t;
}

static OP_EMIT(opem_json_array_each_init) {
    Looper *loop = (Looper *)data;
  //const Instr instr = emit_add_instr(emit, AutoUnrollInit);
  const Instr instr = emit_add_instr(emit, getoff);
  instr->m_val = loop->offset;
  //loop->instr = instr;
  return GW_OK;
}

static INSTR(json_array_loop) {
  const M_Object o = *(M_Object*)MEM(instr->m_val2 - SZ_INT);
  const m_uint idx = ++*(m_uint*)MEM(instr->m_val2);
  const bool end = idx == JSON_ARRAY(o).size();
  if(end) {
    // shred->pc = instr->m_val;
  } else { 
    // should be mem counted
    // p
    const Type t = (Type) instr->m_val;
    M_Object ret = 
      *(M_Object*)MEM(instr->m_val2 + SZ_INT) = new_object(shred->info->vm->gwion->mp, t);
    JSON_ELEMENT(ret) = JSON_ARRAY(o).at(idx);
  }
  *(m_uint*)shred->reg = end;
  shred->reg += SZ_INT;
  // we should put end on the reg stack
  // and use goto
}

static OP_EMIT(opem_json_array_each) {
    Looper *loop = (Looper *)data;
   // pp
  const Instr instr = emit_add_instr(emit, json_array_loop);
  //puts(loop->exp->type->name);exit(12);
  const Type t = str2type(emit->gwion, (m_str)"SimdJSON.element", loop->exp->loc);
  instr->m_val = (m_uint)t;
  instr->m_val2 = loop->offset + SZ_INT;
  loop->instr = emit_add_instr(emit, BranchNeqInt);
  return GW_OK;

}

extern "C" {

GWION_IMPORT(SimdJSON) {
vector_init(&parser_vector);
  GWI_OB(gwi_struct_ini(gwi, (m_str)"SimdJSON"));

  DECL_OB(const Type, t_json_array, = gwi_class_ini(gwi, "array", "Object"));
  t_json_array->nspc->offset += sizeof(dom::array) + sizeof(uint32_t);
  SET_FLAG(t_json_array, abstract);
  gwi_class_xtor(gwi, NULL, json_dtor);
  GWI_BB(gwi_func_ini(gwi, "int", "size"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)json_array_size, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "string", "pp"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)json_array_pp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  DECL_OB(const Type, t_json_object, = gwi_class_ini(gwi, "object", "Object"));
  t_json_object->nspc->offset += sizeof(dom::object) + sizeof(uint32_t);
  SET_FLAG(t_json_object, abstract);
  gwi_class_xtor(gwi, NULL, json_dtor);
  GWI_BB(gwi_func_ini(gwi, "string", "pp"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)json_object_pp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  DECL_OB(const Type, t_json_element, = gwi_class_ini(gwi, "element", "Object"));
  t_json_element->nspc->offset += sizeof(dom::element) + sizeof(uint32_t);
  SET_FLAG(t_json_element, abstract);
  gwi_class_xtor(gwi, NULL, json_dtor);

  GWI_BB(gwi_enum_ini(gwi, (m_str)"type"));
  GWI_BB(gwi_enum_add(gwi, (m_str)"ARRAY", (m_uint)dom::element_type::ARRAY));
  GWI_BB(gwi_enum_add(gwi, (m_str)"OBJECT", (m_uint)dom::element_type::OBJECT));
  GWI_BB(gwi_enum_add(gwi, (m_str)"INT64", (m_uint)dom::element_type::INT64));
  GWI_BB(gwi_enum_add(gwi, (m_str)"UINT64", (m_uint)dom::element_type::UINT64));
  GWI_BB(gwi_enum_add(gwi, (m_str)"DOUBLE", (m_uint)dom::element_type::DOUBLE));
  GWI_BB(gwi_enum_add(gwi, (m_str)"OBJECT", (m_uint)dom::element_type::OBJECT));
  GWI_BB(gwi_enum_add(gwi, (m_str)"STRING", (m_uint)dom::element_type::STRING));
  GWI_BB(gwi_enum_add(gwi, (m_str)"BOOL", (m_uint)dom::element_type::BOOL));
  GWI_BB(gwi_enum_add(gwi, (m_str)"NULL_VALUE", (m_uint)dom::element_type::NULL_VALUE));
  GWI_OB(gwi_enum_end(gwi));

  GWI_BB(gwi_func_ini(gwi, "type", "get_type"));
  GWI_BB(gwi_func_end(gwi, json_element_type, ae_flag_none));
  GWI_BB(gwi_func_ini(gwi, "string", "pp"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)json_elem_pp, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "T", "hydrate:[T]"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)Hydrate, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))
#define import_arg(type, name)          \
  GWI_BB(gwi_func_arg(gwi, type, name))

#define _import_args(target, action, name, args)          \
  GWI_BB(gwi_func_ini(gwi, #name, #action "_" #name) )                  \
  args \
  GWI_BB(gwi_func_end(gwi, (f_xfun)target##_##action##name, ae_flag_none))

#define _import_as(name) _import_args(json_element, as, name,)
#define _import_get(name) _import_args(json_object, get, name, import_arg("string", "key"))
#define _import_at(name) _import_args(json_array, at, name, import_arg("int", "index"))

#define import(def, func) \
def(bool);\
def(int);\
def(float);\
def(string);\
def(object);\
def(array);\
def(element);\

#define import_all(type, func) { \
    const m_uint scope = env_push_type(gwi->gwion->env, t_##type); \
    import(func, type);  \
    env_pop(gwi->gwion->env, scope); \
  }
  import_all(json_array, _import_at);
  import_all(json_object, _import_get);
  import_all(json_element, _import_as);

  GWI_BB(gwi_func_ini(gwi, "element", "load"))
  GWI_BB(gwi_func_arg(gwi, "string", "path"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_load, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "element", "parse"))
  GWI_BB(gwi_func_arg(gwi, "string", "data"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)simdjson_parse, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "string", "tojson"))
  GWI_BB(gwi_func_arg(gwi, "Object", "o"))
  GWI_BB(gwi_func_end(gwi, (f_xfun)ToJson, ae_flag_static))

  GWI_BB(gwi_struct_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "SimdJSON.array", NULL, "void"))
  GWI_BB(gwi_oper_emi(gwi, opem_json_array_each_init));
  GWI_BB(gwi_oper_end(gwi, "@each_init", NULL));
  GWI_BB(gwi_oper_ini(gwi, "SimdJSON.array", NULL, "int"))
  GWI_BB(gwi_oper_emi(gwi, opem_json_array_each));
  GWI_BB(gwi_oper_end(gwi, "@each", NULL));
  GWI_BB(gwi_oper_ini(gwi, "SimdJSON.array", NULL, "SimdJSON.element"))
  GWI_BB(gwi_oper_add(gwi, opck_json_array_each_val));
  GWI_BB(gwi_oper_end(gwi, "@each_val", NULL));
  GWI_BB(gwi_oper_ini(gwi, "SimdJSON.array", NULL, "int"))
  GWI_BB(gwi_oper_end(gwi, "@each_idx", NULL));

  return GW_OK;
}
}
// T hydrate:[T : Codable]() or smth
//   then we need a @trait_check operator
//   could be used in Cytosol too
// error messages
// effects
// use func_check_post?
