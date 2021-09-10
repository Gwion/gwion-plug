#include "simdjson.h"
#include "Gwion.hh"
#include "tojson.hh"

ANN static inline bool is_base_array(const Type t) {
  return t->array_depth && !tflag(t, tflag_empty);
}

ANN static void tojson_pp(const Gwion gwion, std::stringstream *str, const Type t, const m_bit* data) {
  if(isa(t, gwion->type[et_int]) > 0)
    *str << *(m_int*)data;
  else if(tflag(t, tflag_float) > 0)
    *str << *(m_float*)data;
  else if(t == gwion->type[et_string])
    *str << "\"" << STRING(*(M_Object*)data) << "\"" ;
  else if(isa(t, gwion->type[et_union]) > 0) {
    const M_Object obj = *(M_Object*)data;
    const m_uint   idx = *(m_uint*)obj->data;
    if(idx) {
      const Value    val = (Value)map_at(&t->nspc->info->value->map, idx-1);
      tojson_pp(gwion, str, val->type, obj->data + SZ_INT);
    } else *str << "null";
  } else if(isa(t, gwion->type[et_object]) > 0) {
    const M_Object obj = *(M_Object*)data;
    bool _init = false;
    tojson(gwion, str, obj, obj->type_ref, &_init);
  }
}

ANN static void _tojson_array(const Gwion gwion, std::stringstream *str, const M_Object o, const Type t) {
  *str << "[";
  const M_Vector array = ARRAY(o);
  const Type base = o->type_ref->info->base_type;
  bool _init = false;
  for(m_uint i = 0; i < m_vector_size(array); i++) {
    if(is_func(gwion, base))continue;
    if(_init)*str << ",";
    tojson_pp(gwion, str, base, array->ptr + ARRAY_OFFSET + i * base->size);
    _init = true;
  }
  *str << "]";
}

ANN static bool tojson_array(const Gwion gwion, std::stringstream *str, const M_Object o, const Type t, bool *init) {
  const bool base = is_base_array(o->type_ref);
  if(!base)
    *str << "\"array\":";
  _tojson_array(gwion, str, o, t);
  *init = true;
  return base;
}

ANN static void tojson_object(const Gwion gwion, std::stringstream *str, const M_Object o, const Type t, bool *init) {
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value value = (Value)map_at(m, i);
    if(is_func(gwion, value->type))continue;
    if(!vflag(value, vflag_member))continue;
    if(*init)
      *str << ",";
    *str << "\"" << value->name << "\":";
    tojson_pp(gwion, str, value->type, o->data + value->from->offset);
    *init = true;
  }
}

ANN static void _tojson(const Gwion gwion, std::stringstream *str, const M_Object o, const Type t, bool *init) {
  if(!t->nspc)return;
  if(t->info->parent)
    _tojson(gwion, str, o, t->info->parent, init);
  if(t->array_depth && tojson_array(gwion, str, o, t, init))
    return;
  tojson_object(gwion, str, o, t, init);
}

ANN void tojson(const Gwion gwion, std::stringstream *str, const M_Object o, const Type t, bool *init) {
  if(!is_base_array(o->type_ref))
    *str << "{";
  _tojson(gwion, str, o, t, init);
  if(!is_base_array(o->type_ref))
    *str << "}";
}
