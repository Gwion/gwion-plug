#include "simdjson.h"
#include "Gwion.hh"
#include "tojson.hh"

ANN static inline bool is_base_array(const Type t) {
  return t->array_depth && !tflag(t, tflag_empty);
}

typedef struct ToJson {
  std::stringstream *const str;
  union {
    const M_Object obj;
    m_bit *data;
  };
  const Gwion    gwion;
  bool init;
} ToJson;

ANN static void tojson_pp(ToJson *const json, const Type t, const m_bit* data);
ANN void real_tojson(ToJson *const json, const Type t, m_bit *const data);

ANN static void tojson_union(ToJson *const json, const Type t) {
    const m_uint   idx = *(m_uint*)json->obj->data;
    if(idx) {
      const Value    val = (Value)map_at(&t->nspc->info->value->map, idx);
      *json->str << "\"" << val->name << "\":";
      tojson_pp(json, val->type, json->obj->data + SZ_INT);
  } else *json->str << "\"@unset\":null";
}

ANN static void tojson_pp(ToJson *const json, const Type t, const m_bit* data) {
  const Type *type = json->gwion->type;
  if(isa(t, type[et_int]) > 0)
    *json->str << *(m_int*)data;
  else if(tflag(t, tflag_float))
    *json->str << *(m_float*)data;
  else if(t == type[et_string])
    *json->str << "\"" << STRING(*(M_Object*)data) << "\"" ;
  else if(isa(t, type[et_function]) > 0) {
    const VM_Code code = *(VM_Code*)data;
    if(code)
      *json->str << "\"" << (*(VM_Code*)data)->name << "\"" ;
    else *json->str << "null";
  } else if(isa(t, type[et_compound]) > 0) {
    m_bit *const next = !tflag(t, tflag_struct)
      ? *(m_bit**)data : (m_bit*)&data;
    real_tojson(json, t, next);
  }
}

ANN static void _tojson_array(ToJson *const json, const Type t) {
  *json->str << "[";
  const M_Vector array = ARRAY(json->obj);
  const Type base = t->array_depth == 1
    ? t->info->base_type : array_type(json->gwion->env, array_base(t), t->array_depth - 1);
  bool init = false;
  const m_bit *ptr = array->ptr + ARRAY_OFFSET;
  for(m_uint i = 0; i < m_vector_size(array); i++) {
    if(is_func(json->gwion, base) && !is_fptr(json->gwion, base))continue;
    if(init) *json->str << ",";
    tojson_pp(json, base, ptr + i * base->size);
    init = true;
  }
  *json->str << "]";
}

ANN static bool tojson_array(ToJson *const json, const Type t) {
  const bool base = is_base_array(json->obj->type_ref);
  if(!base)
    *json->str << "\"array\":";
  _tojson_array(json, t);
  json->init = true;
  return base;
}

ANN static void tojson_compound(ToJson *const json, const Type t) {
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value value = (Value)map_at(m, i);
    if(is_class(json->gwion, value->type))continue;
    if(is_func(json->gwion, value->type) && !is_fptr(json->gwion, value->type))continue;
    if(!vflag(value, vflag_member))continue;
    if(json->init) *json->str << ",";
    *json->str << "\"" << value->name << "\":";
    if(!tflag(t, tflag_struct))
      tojson_pp(json, value->type, json->obj->data + value->from->offset);
    else
      tojson_pp(json, value->type, *(m_bit**)json->data + value->from->offset);
    json->init = true;
  }
}

ANN static void _tojson(ToJson *const json, const Type t) {
  if(!t->nspc)return;
  if(isa(t, json->gwion->type[et_union]) > 0) {
    tojson_union(json, t);
    return;
  }
  if(t->info->parent)
    _tojson(json, t->info->parent);
  if(t->array_depth && tojson_array(json, t))
    return;
  tojson_compound(json, t);
}

ANN void real_tojson(ToJson *const json, const Type t, m_bit *const data) {
  if(!is_base_array(t))
    *json->str << "{";
  ToJson next = { .str=json->str, .data=data, .gwion=json->gwion };
  _tojson(&next, t);
  if(!is_base_array(t))
    *json->str << "}";
}
ANN void tojson(const Gwion gwion, std::stringstream *str, const M_Object o) {
  ToJson json = { .str=str, .gwion=gwion };
  real_tojson(&json, o->type_ref, (m_bit*)o);
}
