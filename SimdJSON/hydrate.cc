#include "simdjson.h"
#include "Gwion.hh"
#include "dict.h"

using namespace simdjson;

#include "hydrate.hh"

typedef struct {
  const Gwion gwion;
  const VM_Shred shred;
  union {
    M_Object obj;
    m_bit *data;
  };
} Hydrate;

ANN static void _hydrate(Hydrate *const h, dom::element elem, const Type t);

ANN static void hydrate_dict(Hydrate *const h, dom::element elem, const Type t) {
  HMap *const hmap = &*(struct HMap*)h->obj->data;
  const dom::object object = elem.get_object();
  const m_uint capacity = object["capacity"].get_uint64();
  const dom::element data_object = object["data"];
  const dom::array dom_array = data_object.get_array();
  const HMapInfo *hinfo = (HMapInfo*)t->nspc->class_data;
  hmap->data  = (m_bit*)mp_calloc2(h->gwion->mp, hinfo->sz * capacity);
  hmap->state = (m_bit*)mp_calloc2(h->gwion->mp, sizeof(HState) * capacity);
  hmap->capacity = capacity;
  for(auto e : dom_array) {
    const auto dom_slot = e["slot"];
    const m_uint slot = dom_slot.get_int64();
    HState *const state = (HState*)(hmap->state + slot * sizeof(HState));
    state->set = true;
    try {
      m_bit *const data = hmap->data + (hinfo->sz * slot);
      auto key = e["key"];
      hydrate(h->gwion, h->shred, key.value(), hinfo->key, data);
      auto val = e["val"];
      hydrate(h->gwion, h->shred, val.value(), hinfo->key, data + hinfo->key->size);
    } catch (const simdjson_error &e) {
      state->deleted = true;
    }
  }
}

ANN static void hydrate_union(Hydrate *const h, dom::element elem, const Type t) {
  const dom::object obj = elem.get_object();
  // should not iterate
  for(const auto key_value : obj) {
    const Map map = &t->nspc->info->value->map;
    for(m_uint i = 0; i < map_size(map); i++) {
      const Value val = (Value)map_at(map, i);
      if(!strcmp(val->name, key_value.key.data())) {
        *(m_uint*)h->obj->data = i;
        hydrate(h->gwion, h->shred, key_value.value, val->type, h->obj->data + SZ_INT);
        return;
      }
    }
  }
}

ANN static void hydrate_object(Hydrate *const h, dom::element elem, const Type t) {
  const M_Object tmp = new_object(h->gwion->mp, t);
  Hydrate next = { .gwion=h->gwion, .shred=h->shred, .obj=tmp };
  if(t == h->gwion->type[et_event])
    vector_init(&EV_SHREDS(tmp));
  else if(isa(t, h->gwion->type[et_union]) > 0)
    hydrate_union(&next, elem, t);
  else
    _hydrate(&next, elem, t);
  *(M_Object*)(h->data) = tmp;
}

ANN static void hydrate_struct(Hydrate *const h, dom::element elem, const Type t) {
  Hydrate next = { .gwion=h->gwion, .shred=h->shred, .data=h->data };
  _hydrate(&next, elem, t);
}

ANN static void hydrate_compound(Hydrate *const h, dom::element elem, const Type t) {
  if(!tflag(t, tflag_struct))
    hydrate_object(h, elem, t);
  else
    hydrate_struct(h, elem, t);
}

ANN void hydrate(const Gwion gwion, const VM_Shred shred, dom::element elem, const Type t, m_bit *data) {
  if(isa(t, gwion->type[et_int]) > 0)
    *(m_int*)data = elem.get_int64();
  else if(tflag(t, tflag_float))
    *(m_float*)data = elem.get_double();
  else if(t == gwion->type[et_string]) {
    if(elem.is_string()) {
      std::string_view s = elem.get_string();
      *(M_Object*)data = new_string(gwion, (m_str)s.data());
    }
  } else if(isa(t, gwion->type[et_function]) > 0) {
    std::string_view s = elem.get_string();
    char c[strlen(s.data()) + 1];
    strcpy(c, s.data());
    char *at = strchr(c, '@');
    *at = '\0';
    char c1[strlen(s.data()) + 1];
    strcpy(c1, s.data());
    char *at1 = strrchr(c1, '$');
    *at1 = '\0';
    const Symbol sym = insert_symbol(gwion->st, c);
    const Value v = nspc_lookup_value1(t->info->value->from->owner, sym);
    Func f = v->d.func_ref;
    do {
      if(!strcmp(f->name, c1)) {
        *(VM_Code*)data = f->code;
        break;
      }
    } while((f = f->next));
  } else if(isa(t, gwion->type[et_compound]) > 0) {
    if(!elem.is_null()) {
      Hydrate h = { .gwion=gwion, .shred=shred, .data=data};
      hydrate_compound(&h, elem, t);
    }
  }
}

ANN static void iterate_array(Hydrate *const h,
                          const dom::array dom_array, const Type t) {
  const Type btype = t->info->base_type;
  const M_Vector array = ARRAY(h->obj);
  m_vector_init(array, t->info->base_type->size, dom_array.size());
  m_uint i = 0;
  m_bit *const ptr = array->ptr + ARRAY_OFFSET;
  for(auto a : dom_array)
    hydrate(h->gwion, h->shred, a, btype, ptr + i++ * btype->size);
}

ANN static void iterate_object(Hydrate *const &h,
                          const dom::object dom_object, const Type t) {
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value value = (Value)map_at(m, i);
    if(is_class(h->gwion, value->type))continue;
    if(is_func(h->gwion, value->type) && !is_fptr(h->gwion, value->type))continue;
    if(!vflag(value, vflag_member))continue;
    m_bit *ptr = !tflag(t, tflag_struct) ? h->obj->data : (m_bit*)h->data;
    hydrate(h->gwion, h->shred, dom_object[value->name], value->type, ptr + value->from->offset);
  }
}

ANN static void _hydrate(Hydrate *const h, dom::element elem, /*const M_Object o, */const Type t) {
  if(!t->nspc)return;
  if(t->array_depth) {
    dom::array dom_array = elem.get_array();
    iterate_array(h, dom_array, t);
    if(t->info->parent == h->gwion->type[et_array])
      return;
  }
  if(t->info->parent)
    _hydrate(h, elem, t->info->parent);
  dom::object dom_object = elem.get_object();
  iterate_object(h, dom_object, t);
}
