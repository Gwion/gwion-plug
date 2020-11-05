#include <stdlib.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "array.h"
#include <string.h>

#define MAP_INFO(o) (*(struct Map_Info_**)(o->data + t_array->nspc->offset))
#define MAP_KEY(p) ((m_bit*)(p->data))
#define MAP_VAL(p, o) ((m_bit*)(p->data + MAP_INFO(o)->key_size))

typedef m_bool (*f_cmp)(const m_bit* restrict , const unsigned char* restrict , const m_uint);

struct Map_Info_ {
  Type t;
  m_uint key_size;
  m_uint val_size;
  f_cmp cmp;
};

static m_bool string_cmp(const m_bit* restrict a, const unsigned char* restrict b, const m_uint size __attribute__((unused))) {
  const M_Object o = (M_Object)b;
  if(!o && !a)
    return GW_OK;
  return o ? !strcmp((char*)a, STRING(o)) : 1;
}

static m_bool cmp(const m_bit* restrict a, const unsigned char*restrict b, const m_uint size) {
  return !memcmp(a, b, size);
}

static CTOR(map_ctor) {
  struct Map_Info_* info = mp_calloc(shred->info->mp, Map_Info);
  info->t = array_base(o->type_ref->e->parent);
  const Env env = shred->info->vm->gwion->env;
  const Nspc curr = env->curr;
  const m_str key_name = get_type_name(env, info->t, 1);
  struct loc_t_ pos = {};
  const Type key_type = str2type(env->gwion, key_name, &pos);
  info->key_size = key_type->size;
  const m_str val_name = get_type_name(env, info->t, 2);
  const Type val_type = str2type(env->gwion, val_name, &pos);
  info->val_size = val_type->size;
  MAP_INFO(o) = info;
  env->curr = curr;
  if(isa(key_type, t_string) > 0)
    MAP_INFO(o)->cmp = string_cmp;
  else
    MAP_INFO(o)->cmp = cmp;
  ADD_REF(o->type_ref->e->parent);
}

static DTOR(map_dtor) {
  mp_free(Map_Info, MAP_INFO(o));
}


static MFUN(gw_map_get) {
  const M_Vector v = ARRAY(o);
  const m_uint size = m_vector_size(v);
  for(m_uint i = 0; i < size; i++) {
    M_Object p;
    m_vector_get(v, i, &p);
    if(MAP_INFO(o)->cmp(MAP_KEY(p), MEM(SZ_INT), MAP_INFO(o)->key_size)) {
      memcpy((m_bit*)RETURN, MAP_VAL(p, o), MAP_INFO(o)->key_size);
      return;
    }
  }
  memset((m_bit*)RETURN, 0, MAP_INFO(o)->key_size);
}

static MFUN(gw_map_set) {
  const M_Vector v = ARRAY(o);
  const m_uint size = m_vector_size(v);
  memcpy((m_bit*)RETURN, MEM(SZ_INT + MAP_INFO(o)->key_size), MAP_INFO(o)->val_size);
  for(m_uint i = 0; i < size; i++) {
    M_Object p;
    m_vector_get(v, i, &p);
    if(MAP_INFO(o)->cmp(MAP_KEY(p), MEM(SZ_INT), MAP_INFO(o)->key_size)) {
      memcpy(MAP_VAL(p, o), MEM(SZ_INT + MAP_INFO(o)->key_size), MAP_INFO(o)->key_size);
      return;
    }
  }
  const M_Object pair = new_object(shred->info->mp, NULL, MAP_INFO(o)->t);
  memcpy(pair->data, MEM(SZ_INT), MAP_INFO(o)->key_size + MAP_INFO(o)->val_size);
  m_vector_add(v, (m_bit*)&pair);
}

GWION_IMPORT(map) {
/*
  const m_str types[] = { "A", "B" };
  const Type t_map = gwi_mk_type(gwi, "Map", SZ_INT, NULL);
  CHECK_BB(gwi_tmpl_ini(gwi, 2, types))
  CHECK_BB(gwi_class_ini(gwi, t_map, map_ctor, map_dtor))
  CHECK_BB(gwi_tmpl_end(gwi))
  Type_Decl* td  = new_type_decl(prepend_id_list(insert_symbol("Pair"), NULL, 0), 0);
  Type_Decl* td0 = new_type_decl(prepend_id_list(insert_symbol("A"), NULL, 0), 0);
  Type_Decl* td1 = new_type_decl(prepend_id_list(insert_symbol("B"), NULL, 0), 0);
  Type_List tl1  = new_type_list(td1, NULL);
  Type_List tl0  = new_type_list(td0, tl1);
  td->types = tl0;
  const Exp e = new_prim_int(0, 0);
  const Array_Sub array = new_array_sub(e, 0);
  td->array = array;
  CHECK_BB(gwi_class_ext(gwi, td))
  CHECK_BB(gwi_item_ini(gwi, "@internal", "@map_info"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL));
  CHECK_BB(gwi_func_ini(gwi, "B", "set", gw_map_set))
  CHECK_BB(gwi_func_arg(gwi, "A", "key"))
  CHECK_BB(gwi_func_arg(gwi, "B", "val"))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_func_ini(gwi, "B", "get", gw_map_get))
  CHECK_BB(gwi_func_arg(gwi, "A", "key"))
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
*/
  return GW_OK;
}
