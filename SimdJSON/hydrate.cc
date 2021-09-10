#include "simdjson.h"
#include "Gwion.hh"

using namespace simdjson;

#include "hydrate.hh"

ANN static void _hydrate(const Gwion gwion, const VM_Shred shred, dom::element elem, const M_Object o, const Type t);

ANN static void hydrate_union(const Gwion gwion, const VM_Shred shred, dom::element elem, const Type t, const M_Object tmp) {
  const dom::object obj = elem.get_object();
  // should not iterate
  for(const auto key_value : obj) {
    const Map map = &t->nspc->info->value->map;
    for(m_uint i = 0; i < map_size(map); i++) {
      const Value val = (Value)map_at(map, i);
      if(!strcmp(val->name, key_value.key.data())) {
        *(m_uint*)tmp->data = i + 1;
        hydrate(gwion, shred, key_value.value, val->type, tmp->data + SZ_INT);
        break;
      }
    }
  }
}

ANN static void hydrate_object(const Gwion gwion, const VM_Shred shred, dom::element elem, const Type t, m_bit *data) {
  const M_Object tmp = new_object(gwion->mp, NULL, t);
  if(isa(t, gwion->type[et_event]) > 0)
    vector_init(&EV_SHREDS(tmp));
  else if(isa(t, gwion->type[et_union]) > 0)
    hydrate_union(gwion, shred, elem, t, tmp);
  else
    _hydrate(gwion, shred, elem, tmp, t);
  *(M_Object*)(data) = tmp;
}

ANN void hydrate(const Gwion gwion, const VM_Shred shred, dom::element elem, const Type t, m_bit *data) {
  if(isa(t, gwion->type[et_int]) > 0)
    *(m_int*)data = elem.get_int64();
  else if(tflag(t, tflag_float))
    *(m_float*)data = elem.get_double();
  else if(t == gwion->type[et_string]) {
    if(elem.is_string()) {
      std::string_view s = elem.get_string();
      *(M_Object*)data = new_string2(gwion, NULL, (m_str)s.data());
    }
  } else if(isa(t, gwion->type[et_object]) > 0) {
    if(!elem.is_null())
      hydrate_object(gwion, shred, elem, t, data);
  }
}

ANN static void iterate_array(const Gwion gwion, const VM_Shred shred,
                          const dom::array dom_array, const Type t, const M_Object o) {
  const Type btype = t->info->base_type;
  const M_Vector array = ARRAY(o);
  m_vector_init(ARRAY(o), t->info->base_type->size, dom_array.size());
  m_uint i = 0;
  for(auto a : dom_array)
    hydrate(gwion, shred, a, btype, array->ptr + ARRAY_OFFSET + i++ * btype->size);
}

ANN static void iterate_object(const Gwion gwion, const VM_Shred shred,
                          const dom::object dom_object, const Type t, const M_Object o) {
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value value = (Value)map_at(m, i);
    if(is_func(gwion, value->type))continue;
    if(!vflag(value, vflag_member))continue;
    hydrate(gwion, shred, dom_object[value->name], value->type, o->data + value->from->offset);
  }
}

ANN static void _hydrate(const Gwion gwion, const VM_Shred shred, dom::element elem, const M_Object o, const Type t) {
  if(!t->nspc)return;
  if(t->array_depth) {
    dom::array dom_array = elem.get_array();
    iterate_array(gwion, shred, dom_array, t, o);
    return;
  }
  if(t->info->parent)
    _hydrate(gwion, shred, elem, o, t->info->parent);
  dom::object dom_object = elem.get_object();
  iterate_object(gwion, shred, dom_object, t, o);
}
