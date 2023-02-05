#include "simdjson.h"
#include "Gwion.hh"
#include "tojson.hh"
#include "dict.h"

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

ANN static void tojson_dict(ToJson *const json, const Type t) {
  const HMapInfo *hinfo = (HMapInfo*)t->nspc->class_data;
  HMap *hmap = &*(struct HMap*)json->obj->data;
  m_uint count = 0;
  *json->str << "\"capacity\":" << hmap->capacity << ",\"data\":[";
  for(m_uint i = 0; i < hmap->capacity; i++) {
    const HState state = *(HState*)(hmap->state + i * sizeof(HState));
    m_bit *const data = hmap->data + hinfo->sz * i;
    if(state.set) {
      if(count++) *json->str << ",";
      *json->str << "{\"slot\":" << i;
      if(!state.deleted) {
       *json->str << ",\"key\":";
        tojson_pp(json, hinfo->key, data);
        *json->str << ",\"val\":";
        tojson_pp(json, hinfo->val, data + hinfo->key->size);
      } else *json->str << ",\"deleted\":true";
      *json->str << "}";
    }
  }
  *json->str << "]";
}

ANN static void tojson_union(ToJson *const json, const Type t) {
    const m_uint   idx = **(m_uint**)json->data;
    if(idx) {
      const Value    val = (Value)map_at(&t->nspc->info->value->map, idx);
      *json->str << "\"" << val->name << "\":";
      tojson_pp(json, val->type, *(m_bit**)json->data + SZ_INT);
  } else *json->str << "\"@unset\":null";
}

ANN static void tojson_pp(ToJson *const json, const Type t, const m_bit* data) {
  const Type *type = json->gwion->type;
  if(isa(t, type[et_int]) > 0)
    *json->str << *(m_int*)data;
  else if(tflag(t, tflag_float))
    *json->str << *(m_float*)data;
  else if(t == type[et_string]) {
    const M_Object o = *(M_Object*)data;
    if(o)
      *json->str << "\"" << STRING(o) << "\"" ;
  } else if(is_func(json->gwion, t)) { // is_callable?
    const VM_Code code = *(VM_Code*)data;
    if(code)
      *json->str << "\"" << (*(VM_Code*)data)->name << "\"" ;
    else *json->str << "null";
  } else if(tflag(t, tflag_compound)) {
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
    if(is_func(json->gwion, base))continue; // is_func
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
    if(is_func(json->gwion, value->type))continue; // is_func
    if(!vflag(value, vflag_member))continue;
    if(json->init) *json->str << ",";
    *json->str << "\"" << value->name << "\":";
    if(!tflag(t, tflag_struct))
      tojson_pp(json, value->type, json->obj->data + value->from->offset);
    else //if ((*(m_bit**)json->data + value->from->offset))
      tojson_pp(json, value->type, *(m_bit**)json->data + value->from->offset);
//    else *json->str << "null";
    json->init = true;
  }
}

ANN static void _tojson(ToJson *const json, const Type t) {
  if(!t->nspc)return;
  if(tflag(t, tflag_union)) {
    tojson_union(json, t);
    return;
  }
  if(isa(t, json->gwion->type[et_dict]) > 0) {
    tojson_dict(json, t);
    return;
  }
  if(t->info->parent)
    _tojson(json, t->info->parent);
  if(t->array_depth && tojson_array(json, t))
    return;
  tojson_compound(json, t);
}

ANN void real_tojson(ToJson *const json, const Type t, m_bit *const data) {
  const M_Object o = (M_Object)data;
  if(!is_base_array(t) && o )
    *json->str << "{";
  ToJson next = { .str=json->str, .data=data, .gwion=json->gwion, .init = false };
  if(!tflag(t, tflag_struct)) {
    if(o) _tojson(&next, o->type_ref);
    else *json->str << "null";
  } else _tojson(&next, t);
  if(!is_base_array(t) && o)
    *json->str << "}";
}

ANN void tojson(const Gwion gwion, std::stringstream *str, const M_Object o) {
  ToJson json = { .str=str, .data = NULL, .gwion=gwion, .init = false };
  real_tojson(&json, o->type_ref, (m_bit*)o);
}
