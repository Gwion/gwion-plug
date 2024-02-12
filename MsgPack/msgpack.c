#include "msgpack.h"
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "dict.h"
#include "gwi.h"
#include "traverse.h"

typedef struct ToMsg {
  msgpack_packer *ctx;
  Gwion gwion;
  union {
    m_bit *data;
    M_Object obj;
  };
} ToMsg;

ANN static void _tomsg(ToMsg *const msg, const Type t);
ANN static void tomsg_pp(ToMsg *const msg, const Type t, m_bit *data);

ANN static void tomsg_string(ToMsg *const msg, const char *str) {
  msgpack_pack_str_with_body(msg->ctx, str, strlen(str));
}

ANN static void tomsg_union(ToMsg *const msg, const Type t) {
  const m_uint   idx = **(m_uint**)msg->data;
  if(idx) {
    msgpack_pack_array(msg->ctx, 2);
    msgpack_pack_int64(msg->ctx, idx);
    const Value    val = (Value)map_at(&t->nspc->info->value->map, idx);
    if(val->type != msg->gwion->type[et_none])
      tomsg_pp(msg, val->type, *(m_bit**)msg->data + SZ_INT);
    else msgpack_pack_nil(msg->ctx);
  } else msgpack_pack_nil(msg->ctx);
}

typedef struct {
  m_uint index;
  bool deleted;
} dictpack;

ANN static void tomsg_dict(ToMsg *const msg, const Type t) {
  const HMapInfo *hinfo = (HMapInfo*)t->nspc->class_data;
  HMap *hmap = (struct HMap*)msg->obj->data;
  m_uint n = 0;
  m_uint count = 0;
  dictpack *dps = mp_calloc2(msg->gwion->mp, hmap->capacity * sizeof(dictpack));
  for(m_uint i = 0; i < hmap->capacity; i++) {
    const HState state = *(HState*)(hmap->state + i * sizeof(HState));
    if(state.set) {
      dps[n] = (dictpack){ .index = i, .deleted = state.deleted };
      count += 2 + !state.deleted;
      n++;
    }
  }
  msgpack_pack_array(msg->ctx, count + 2);
  msgpack_pack_int64(msg->ctx, hmap->capacity);
  msgpack_pack_int64(msg->ctx, n);
  for(m_uint i = 0; i < n; i++) {
    dictpack dp = dps[i];
    msgpack_pack_int64(msg->ctx, dp.index);
    m_bit *const data = hmap->data + hinfo->sz * dp.index;
    if(!dp.deleted) {
      tomsg_pp(msg, hinfo->key, data);
      tomsg_pp(msg, hinfo->val, data + hinfo->key->size);
    } else msgpack_pack_nil(msg->ctx);
  }
  mp_free2(msg->gwion->mp, hmap->capacity * sizeof(dictpack), dps);
}

ANN static inline bool is_base_array(const Type t) {
  return t->array_depth && !tflag(t, tflag_empty);
}

ANN static inline bool is_member(const Gwion gwion, const Value v) {
  return !(is_class(gwion, v->type) || is_func(gwion, v->type)) &&
           vflag(v, vflag_member);
}

ANN static bool tomsg_array(ToMsg *const msg, const Type t) {
  const M_Vector array = ARRAY(msg->obj);
  const Type base = t->array_depth == 1
    ? t->info->base_type
    : array_type(msg->gwion->env, array_base(t), t->array_depth - 1, t->info->cdef->base.tag.loc);
  msgpack_pack_array(msg->ctx, m_vector_size(array));
  m_bit *const ptr = array->ptr + ARRAY_OFFSET;
  for(m_uint i = 0; i < m_vector_size(array); i++)
    tomsg_pp(msg, base, ptr + i * base->size);
  return is_base_array(msg->obj->type_ref);
}

ANN static void tomsg_compound(ToMsg *const msg, const Type t) {
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value v = (Value)map_at(m, i);
    if(!is_member(msg->gwion, v)) continue;
    if(!tflag(v->type, tflag_struct))
      tomsg_pp(msg, v->type, msg->obj->data + v->from->offset);
    else
      tomsg_pp(msg, v->type, *(m_bit**)msg->data + v->from->offset);
  }
}

ANN static void _tomsg(ToMsg *const msg, const Type t) {
  if(!t->nspc) return;
  if(tflag(t, tflag_union))
    return tomsg_union(msg, t);
  if(t->info->parent)
    _tomsg(msg, t->info->parent);
  if(t == msg->gwion->type[et_string])
    return tomsg_string(msg, STRING(msg->obj));
  if(t->array_depth && tomsg_array(msg, t))
    return;
  tomsg_compound(msg, t);
}

ANN static m_uint nmember(const Gwion gwion, const Type t) {
  const Type parent = t->info->parent;
  m_uint n = parent && parent->nspc
    ? nmember(gwion, parent)
    : 0;
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value v = (Value)map_at(m, i);
    if(is_member(gwion, v)) n++;
  }
  return n;
}
ANN void real_tomsg(ToMsg *const msg, const Type t, m_bit *const data) {
  const M_Object o = (M_Object)data;
  ToMsg next = { .ctx=msg->ctx, .data=data, .gwion=msg->gwion };
  if(tflag(t, tflag_union))
    return tomsg_union(&next, t);
  if(isa(t, msg->gwion->type[et_dict]) > 0)
    return tomsg_dict(&next, t);
  // we can cache it
  int n = nmember(msg->gwion, t) + !!t->array_depth;
  if(isa(t, msg->gwion->type[et_string]) > 0) n++;
  msgpack_pack_array(msg->ctx, n + 1);
  const m_str name = type2str(msg->gwion, t, (loc_t){});
  tomsg_string(msg, name);
  free_mstr(msg->gwion->mp, name);
  if(!tflag(t, tflag_struct)) {
    if(o) _tomsg(&next, o->type_ref);
    else msgpack_pack_nil(msg->ctx);
  } else _tomsg(&next, t);
}

ANN static void tomsg_pp(ToMsg *const msg, const Type t, m_bit *data) {
  const Type *type = msg->gwion->type;
  if(isa(t, type[et_int]) > 0)
    msgpack_pack_int64(msg->ctx, *(m_int*)data);
  else if(tflag(t, tflag_float))
    msgpack_pack_double(msg->ctx, *(m_float*)data);
  else if(tflag(t, tflag_primitive)) {
    const uint n = t->size / sizeof(uint64_t);
    msgpack_pack_array(msg->ctx, n);
    for(uint i = 0; i < n; i++)
      msgpack_pack_int64(msg->ctx, *(uint64_t*)(data + SZ_INT));
  } else if(tflag(t, tflag_compound)) {
    m_bit *const next = !tflag(t, tflag_struct)
      ? *(m_bit**)data : (m_bit*)&data;
    real_tomsg(msg, t, next);
  }
}

static OP_CHECK(opck_pack_any) {
  Exp_Call *call = (Exp_Call *)data;
  Exp* name = call->args;
  if(!name || name->next)
    ERR_N(call->func->loc, "MsgPack.pack takes only one argument");
  CHECK_ON(check_exp(env, name));
  Exp* e = new_prim_int(env->gwion->mp, (m_uint)name->type, name->loc);
  e->type = env->gwion->type[et_int];
  e->next = name;
  call->args = e;
  const loc_t loc = call->func->loc;
  free_exp(env->gwion->mp, call->func);
  Exp* base = new_prim_id(env->gwion->mp, insert_symbol(env->gwion->st, "MsgPack"), loc);
  call->func = new_exp_dot(env->gwion->mp, base, insert_symbol(env->gwion->st, "pack_internal"), loc);
  CHECK_ON(check_exp(env, call->func));
  return call->func->type;
}

typedef struct Packer_{
  m_bit *reg;
  m_bit *mem;
  VM_Shred shred;
} Packer;

ANN static void pack_func(void *data) {
  Packer *packer = data;
  m_bit *reg = packer->reg;
  m_bit *mem = packer->mem;
  VM_Shred shred = packer->shred;
  mp_free(shred->info->vm->gwion->mp, Packer, packer);
  const Type t = *(Type*)mem;
  msgpack_packer pack;
  msgpack_sbuffer sbuf = {};
  msgpack_packer_init(&pack, &sbuf, msgpack_sbuffer_write);
  ToMsg msg = { .ctx=&pack,  .data = mem + SZ_INT, .gwion=shred->info->vm->gwion };
  tomsg_pp(&msg, t, mem + SZ_INT);
  sbuf.data[sbuf.size] = 0;
  *(M_Object*)(reg) = new_string(shred->info->vm->gwion, sbuf.data);
  shred->reg += SZ_INT;
  msgpack_sbuffer_destroy(&sbuf);
  shredule(shred->tick->shreduler, shred, GWION_EPSILON);
}

static Packer *new_packer(MemPool mp, const VM_Shred shred) {
  Packer *packer = mp_malloc(mp, Packer);
  packer->reg = shred->reg;
  packer->mem = shred->mem;
  packer->shred = shred;
  return packer;
}

static SFUN(gwcw_pack) {
  Packer *packer = new_packer(shred->info->mp, shred);
  shred_pool_run(shred, pack_func, packer);
}

typedef struct FromMsg {
  const Gwion gwion;
  const Type owner_class;
  const Nspc owner;
  const VM_Shred shred;
} FromMsg;

static bool unpack(const FromMsg *msg, const Type t, msgpack_object o, m_bit *data);

static bool unpack_compound(const FromMsg *msg, const Type t, msgpack_object *o, m_bit *data) {
  const Map m = &t->nspc->info->value->map;
  for(m_uint i = 0; i < map_size(m); i++) {
    const Value value = (Value)map_at(m, i);
    if(is_class(msg->gwion, value->type))continue;
    if(is_func(msg->gwion, value->type))continue; // is_func
    if(!vflag(value, vflag_member))continue;
    if(!unpack(msg, value->type, *o, data + value->from->offset))
      return false;
    o++;
  }
  return true;
}

static bool unpack_union(const FromMsg *msg, const Type t, msgpack_object o, m_bit *data) {
  if(unlikely(o.type == MSGPACK_OBJECT_NIL)) {
    memset(data, 0, t->size);
    return true;
  }
  msgpack_object *p = o.via.array.ptr;
  const uint32_t idx = *(m_uint*)data = p->via.i64;
  const Type base = (Type)vector_at(&t->info->tuple->contains, idx-1);
  p++;
  return unpack(msg, base, *p, data + SZ_INT);
}

ANN static bool unpack_dict(const FromMsg *msg, const Type t, msgpack_object o, m_bit *data) {
  if(o.type != MSGPACK_OBJECT_ARRAY)
    return false;
  msgpack_object *p = o.via.array.ptr;
  if(p->type != MSGPACK_OBJECT_POSITIVE_INTEGER)
    return false;
  const M_Object ret = *(M_Object*)data = new_object(msg->gwion->mp, t);
  const m_uint capacity = p->via.i64;
  const HMapInfo *hinfo = (HMapInfo*)t->nspc->class_data;
  dict_alloc(msg->gwion->mp, ret, hinfo->sz, capacity);
  p++;
  if(p->type != MSGPACK_OBJECT_POSITIVE_INTEGER)
    return false;
  const uint32_t sz = p->via.i64;
  p++;
  for(uint32_t i = 0; i < sz; i++) {
    if(p->type != MSGPACK_OBJECT_POSITIVE_INTEGER)
      return false;
    const m_uint idx = p->via.i64;
    p++;
    HMap *hmap = &*(struct HMap*)ret->data;
    HState *const state = (HState*)(hmap->state + idx * sizeof(HState));
    m_bit *const data = hmap->data + hinfo->sz * idx;
    state->set = true;
    if(p->type == MSGPACK_OBJECT_NIL) state->deleted = true;
    else {
      if(!unpack(msg, hinfo->key, *p, data))
        return false;
      p++;
      if(!unpack(msg, hinfo->val, *p, data + hinfo->key->size))
        return false;
    }
    p++;
  }
  return true;
}

ANN static m_str unpack_string(MemPool mp, msgpack_object *o) {
  if(o->type != MSGPACK_OBJECT_STR)
    return NULL;
  const uint32_t sz = o->via.str.size;
  m_str str = mp_malloc2(mp, sz + 1);
  memcpy(str, o->via.str.ptr, sz);
  str[sz] = '\0';
  return str;
}

ANN static bool unpack_object(const FromMsg *msg, const Type t, msgpack_object *o, M_Object data) {
  const Type parent = t->info->parent;
  if(parent && parent->nspc) unpack_object(msg, parent, o, data);
  if(vflag(t->info->value, vflag_builtin) && tflag(t, tflag_ctor)) {
    const Func f = (Func)vector_front(&t->nspc->vtable);
    f_xtor ctor = (f_xtor)f->code->native_func;
    ctor(data, msg->shred->mem, msg->shred); // beware mem
  }
  if(t == msg->gwion->type[et_string]) {
    STRING(data) = unpack_string(msg->gwion->mp, o);
    return !!STRING(data);
  }
  if(is_base_array(t)) {
    const Type base = array_base(t);
    const uint32_t sz = o->via.array.size;
    msgpack_object *p = o->via.array.ptr;
    M_Vector array = ARRAY(data);
    m_vector_init(array, base->size, sz);
    m_bit* ptr = ARRAY_PTR(array);
    for(uint32_t i = 0; i < sz; i++) {
      if(!unpack(msg, base, *p, ptr)) return false;
      p++;
      ptr += base->actual_size ?: base->size;
    }
    return true;
  }
  return unpack_compound(msg, t, o, data->data);
}

ANN static bool unpack(const FromMsg *msg, const Type t, msgpack_object o, m_bit *data) {
  if(isa(t, msg->gwion->type[et_int]) > 0) {
    if(o.type != MSGPACK_OBJECT_POSITIVE_INTEGER &&
       o.type != MSGPACK_OBJECT_NEGATIVE_INTEGER)
      return false;
    *(m_int*)data = o.via.i64;
    return true;
  }
  if(tflag(t, tflag_float) > 0) {
    if(o.type != MSGPACK_OBJECT_FLOAT64)
      return false;
    *(m_float*)data = o.via.f64;
    return true;
  }
  if(tflag(t, tflag_primitive)) {
    if(o.type != MSGPACK_OBJECT_ARRAY)
      return false;
    const uint32_t n = o.via.array.size;
    msgpack_object *p = o.via.array.ptr;
    for(uint32_t i = 0; i < n; i++) {
      if(p->type != MSGPACK_OBJECT_POSITIVE_INTEGER)
        return false;
      *(m_uint*)(data + i * SZ_INT) = p->via.i64;
      p++;
    }
    return true;
  }
  if(!tflag(t, tflag_struct)) {
    if(o.type == MSGPACK_OBJECT_NIL) {
      *(M_Object*)data = NULL;
      return true;
    }
    if(o.type != MSGPACK_OBJECT_ARRAY)
      return false;
    if(isa(t, msg->gwion->type[et_dict]) > 0)
      return unpack_dict(msg, t, o, data);
    msgpack_object *p = o.via.array.ptr;
    m_str type_name = unpack_string(msg->gwion->mp, p);
// envset?
    const uint32_t scope = env_push(msg->gwion->env, msg->owner_class, msg->owner);
    const Type base = str2type(msg->gwion, type_name, (loc_t){});
    env_pop(msg->gwion->env, scope);
    free_mstr(msg->gwion->mp, type_name);
    if(!base)return false;
    p++;
    if(p->type != MSGPACK_OBJECT_ARRAY) {
      if(p->type == MSGPACK_OBJECT_STR) {
        const m_str str = unpack_string(msg->gwion->mp, p);
        if(!str) return false;
        *(M_Object*)data = new_string(msg->gwion, str);
        return true;
      }
      return false;
    }
    const M_Object ret = *(M_Object*)data = new_object(msg->gwion->mp, base);
    return unpack_object(msg, base, p, ret);
  }
  if(tflag(t, tflag_union))
    return unpack_union(msg, t, o, data);
  if(o.type != MSGPACK_OBJECT_ARRAY)
    return false;
  msgpack_object *p = o.via.array.ptr;
  return unpack_compound(msg, t, p, data);
}

ANN static bool unpack_str(const FromMsg *msg, const Type t, const char *str, m_bit *const data) {
  msgpack_object o;
  msgpack_zone z;
  msgpack_zone_init(&z, 1024);
  msgpack_unpack_return uret = msgpack_unpack(str, strlen(str), NULL, &z, &o);
  const bool ret = uret == MSGPACK_UNPACK_SUCCESS
    ? unpack(msg, t, o, data)
    : false;
  msgpack_zone_destroy(&z);
  return ret;
}

ANN static void unpack_func(void *data) {
  Packer *packer = data;
  m_bit *reg = packer->reg;
  m_bit *mem = packer->mem;
  VM_Shred shred = packer->shred;
  Gwion gwion = shred->info->vm->gwion;
  mp_free(gwion->mp, Packer, packer);
  const M_Object arg = *(M_Object*)(mem + SZ_INT*2);
  VM_Code code = *(VM_Code*)(reg + SZ_INT*3);
  const FromMsg msg = {
    .gwion = gwion,
    .owner_class = *(Type*)mem,
    .owner = *(Nspc*)(mem + SZ_INT),
    .shred = shred
  };
  msgpack_unpacked result;
  msgpack_unpacked_init(&result);
  const bool ret = unpack_str(&msg, code->ret_type, STRING(arg), reg);
  msgpack_unpacked_destroy(&result);
  shred->reg += code->ret_type->size;
  if(ret) shredule(shred->tick->shreduler, shred, GWION_EPSILON);
  else xfun_handle(shred, "MsgPackUnpack");
}

static SFUN(gwcw_unpack) {
  Packer *packer = new_packer(shred->info->mp, shred);
  shred_pool_run(shred, unpack_func, packer);
}

static OP_CHECK(opck_unpack_any) {
  Exp_Call *call = (Exp_Call *)data;
  Exp* name = call->args;
  if(!name || name->next || check_exp(env, name) != env->gwion->type[et_string])
    ERR_N(call->func->loc, "MsgPack.pack takes only one argument");
  Exp* e = new_prim_int(env->gwion->mp, (m_uint)env->class_def, name->loc);
  e->type = env->gwion->type[et_int];
  Exp* e2 = new_prim_int(env->gwion->mp, (m_uint)env->curr, name->loc);
  e2->type = env->gwion->type[et_int];
  e->next = e2;
  e2->next = name;
  call->args = e;
  const loc_t loc = call->func->loc;
  free_exp(env->gwion->mp, call->func);
  Exp* base = new_prim_id(env->gwion->mp, insert_symbol(env->gwion->st, "MsgPack"), loc);
  call->func = new_exp_dot(env->gwion->mp, base, insert_symbol(env->gwion->st, "unpack_internal"), loc);
  CHECK_ON(check_exp(env, call->func));
  return call->func->type;
}

GWION_IMPORT(MsgPack) {
  DECL_OB(const Type, t_msgpack, = gwi_class_ini(gwi, "MsgPack", "Object"));

  GWI_BB(gwi_func_ini(gwi, "string", "pack"));
  GWI_BB(gwi_func_end(gwi, gwcw_pack, ae_flag_static));

  GWI_BB(gwi_func_ini(gwi, "string", "unpack"));
  GWI_BB(gwi_func_end(gwi, gwcw_pack, ae_flag_static));

  GWI_BB(gwi_func_ini(gwi, "string", "pack_internal:[T]"));
  GWI_BB(gwi_func_arg(gwi, "int", "type"));
  GWI_BB(gwi_func_arg(gwi, "T", "arg"));
  GWI_BB(gwi_func_end(gwi, gwcw_pack, ae_flag_static));

  GWI_BB(gwi_func_ini(gwi, "T", "unpack_internal:[T]"));
  GWI_BB(gwi_func_arg(gwi, "int", "owner_class"));
  GWI_BB(gwi_func_arg(gwi, "int", "owner"));
  GWI_BB(gwi_func_arg(gwi, "string", "arg"));
  GWI_BB(gwi_func_end(gwi, gwcw_unpack, ae_flag_static));

  GWI_BB(gwi_class_end(gwi));

  const struct Op_Func   opfunc0 = {.ck = opck_pack_any};
  CHECK_B(add_op_func_check(gwi->gwion->env, t_msgpack, &opfunc0, 0));
  const struct Op_Func   opfunc1 = {.ck = opck_unpack_any};
  CHECK_B(add_op_func_check(gwi->gwion->env, t_msgpack, &opfunc1, 1));

  return GW_OK;
}
