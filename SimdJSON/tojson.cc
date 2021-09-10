#include "simdjson.h"
#include "Gwion.hh"
#include "tojson.hh"

ANN static inline bool is_base_array(const Type t) {
  return t->array_depth && !tflag(t, tflag_empty);
}

typedef struct ToJson {
  const Gwion    gwion;
  union {
    const M_Object obj;
    m_bit *data;
  };
  bool init;
} ToJson;

ANN static void tojson_pp(const Gwion gwion, std::stringstream *str, const Type t, const m_bit* data);

ANN static void tojson_union(ToJson *const json, std::stringstream *str, const Type t, const m_bit* data) {
    const M_Object obj = (M_Object)data;
    const m_uint   idx = *(m_uint*)obj->data;
    if(idx) {
      const Value    val = (Value)map_at(&t->nspc->info->value->map, idx);
      *str << "\"" << val->name << "\":";
      tojson_pp(json->gwion, str, val->type, obj->data + SZ_INT);
  } else *str << "\"@unset\":null";
}

ANN static void tojson_pp(const Gwion gwion, std::stringstream *str, const Type t, const m_bit* data) {
  const Type *type = gwion->type;
  if(isa(t, type[et_int]) > 0)
    *str << *(m_int*)data;
  else if(tflag(t, tflag_float))
    *str << *(m_float*)data;
  else if(t == type[et_string])
    *str << "\"" << STRING(*(M_Object*)data) << "\"" ;
  else if(isa(t, type[et_compound]) > 0) {
    const M_Object obj = !tflag(t, tflag_struct)
      ? *(M_Object*)data : (M_Object)&data;
    tojson(gwion, str, obj);
  }
}

ANN static void _tojson_array(ToJson *const json, std::stringstream *str, const Type t) {
  *str << "[";
  const M_Vector array = ARRAY(json->obj);
  const Type base = t->array_depth == 1
    ? t->info->base_type : array_type(json->gwion->env, array_base(t), t->array_depth - 1);
  bool init = false;
  const m_bit *ptr = array->ptr + ARRAY_OFFSET;
  for(m_uint i = 0; i < m_vector_size(array); i++) {
    if(is_func(json->gwion, base))continue;
    if(init) *str << ",";
    tojson_pp(json->gwion, str, base, ptr + i * base->size);
    init = true;
  }
  *str << "]";
}

ANN static bool tojson_array(ToJson *const json, std::stringstream *str, const Type t) {
  const bool base = is_base_array(json->obj->type_ref);
  if(!base)
    *str << "\"array\":";
  _tojson_array(json, str, t);
  json->init = true;
  return base;
}

ANN static void tojson_object(ToJson *const json, std::stringstream *str, const Type t) {
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value value = (Value)map_at(m, i);
    if(is_func(json->gwion, value->type))continue;
    if(!vflag(value, vflag_member))continue;
    if(json->init) *str << ",";
    *str << "\"" << value->name << "\":";
    if(!tflag(t, tflag_struct))
      tojson_pp(json->gwion, str, value->type, json->obj->data + value->from->offset);
    else
      tojson_pp(json->gwion, str, value->type, *(m_bit**)json->data + value->from->offset);
    json->init = true;
  }
}

ANN static void _tojson(ToJson *const json, std::stringstream *str, const Type t) {
  if(!t->nspc)return;
  if(isa(t, json->gwion->type[et_union]) > 0) {
    tojson_union(json, str, t, (const m_bit*)json->obj);
    return;
  }
  if(t->info->parent)
    _tojson(json, str, t->info->parent);
  if(t->array_depth && tojson_array(json, str, t))
    return;
  tojson_object(json, str, t);
}

ANN void tojson(const Gwion gwion, std::stringstream *str, const M_Object o) {
  const Type t = o->type_ref;
  if(!is_base_array(t))
    *str << "{";
  ToJson json = { gwion, o };
  _tojson(&json, str, t);
  if(!is_base_array(t))
    *str << "}";
}
