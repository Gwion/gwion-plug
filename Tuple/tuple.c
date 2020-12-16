#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "traverse.h"
#include "object.h"
#include "parse.h"
#include "array.h"
#include "operator.h"

#define TUPLE_NAME "Tuple"

#undef insert_symbol

struct TupleEmit {
  Exp e;
  Vector v;
  m_uint obj_offset;
  m_uint tmp_offset;
  m_uint mem_offset;
  m_uint sz;
  m_uint idx;
};

struct UnpackInfo_ {
  m_uint obj_offset;
  m_uint mem_offset;
  m_uint size;
};

static INSTR(TupleUnpack) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT*2);
  struct UnpackInfo_ *info = (struct UnpackInfo_*)instr->m_val;
  memcpy(shred->mem + info->mem_offset, o->data + info->obj_offset, info->size);
}

ANN static inline Type tuple_base(Type t, const m_uint idx) {
  while(t->info->tuple->start > idx)
    t = t->info->parent;
  return t;
}

INSTR(TupleMember) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  const Type base = tuple_base(o->type_ref, instr->m_val);
  const m_bit* byte = shred->code->bytecode + shred->pc * BYTECODE_SZ;
  const m_uint idx = instr->m_val - base->info->tuple->start;
  const Type t = (Type)vector_at(&base->info->tuple->types, idx);
  const m_uint offset = vector_at(&base->info->tuple->offset, idx);
  *(m_uint*)(byte + SZ_INT) = offset;
  if(!instr->m_val2) {
    if(t->size == SZ_INT)
      *(m_uint*)(byte) = eDotMember;
    else if(t->size == SZ_FLOAT)
      *(m_uint*)(byte) = eDotMember2;
    else {
      *(m_uint*)(byte) = eDotMember3;
      *(m_uint*)(byte + SZ_INT*2) = t->size;
    }
  } else
    *(m_uint*)(byte) = eDotMember4;
}

static FREEARG(freearg_tuple_at) {
  mp_free(((Gwion)gwion)->mp, UnpackInfo, (struct UnpackInfo*)instr->m_val);
}

ANN static void emit_unpack_instr_inner(const Emitter emit, struct TupleEmit *te) {
  const Instr instr = emit_add_instr(emit, TupleUnpack);
  struct UnpackInfo_ *info = mp_malloc(emit->gwion->mp, UnpackInfo);
  info->obj_offset = te->obj_offset;
  info->mem_offset = te->mem_offset;
  info->size = te->sz;
  instr->m_val = (m_uint)info;
}

ANN static int tuple_continue(struct TupleEmit *te) {
  const m_bool ret = (te->e = te->e->next) &&
       ++te->idx < VLEN(te->v);
  if(!ret)
    te->e = NULL;
  return ret;
}

ANN static void unpack_instr_decl(const Emitter emit, struct TupleEmit *te) {
  m_uint sz = 0;
  te->sz = 0;
  te->obj_offset = te->tmp_offset;
  do {
    if(te->e->exp_type == ae_exp_decl) {
      const Value value = te->e->d.exp_decl.list->self->value;
      te->sz += value->type->size;
      sz += value->type->size;
      value->from->offset = emit_local(emit, value->type);
    } else {
      sz += ((Type)vector_at(te->v, te->idx))->size;
      break;
    }
  } while(tuple_continue(te));
  te->tmp_offset += sz;
}

ANN void emit_unpack_instr(const Emitter emit, struct TupleEmit *te) {
  te->mem_offset = emit_code_offset(emit);
  unpack_instr_decl(emit, te);
  if(te->sz)
    emit_unpack_instr_inner(emit, te);
  if(te->e && (te->e = te->e->next))
    emit_unpack_instr(emit, te);
}

static void get(const Vector v, Type t) {
  if(t->info->parent)
    get(v, t->info->parent);
  if(!t->info->tuple) // necessary ?
    return;
  for(m_uint i = 0; i < vector_size(&t->info->tuple->types); ++i)
    vector_add(v, (m_uint)vector_at(&t->info->tuple->types, i));
}

struct Matcher {
  struct Vector_ l;
  struct Vector_ r;
  const Type undef;
};

ANN static inline m_bool matcher_run(struct Matcher *m) {
  for(m_uint i = 0; i < vector_size(&m->r); ++i) {
    DECL_OB(const Type, l, = (Type)vector_at(&m->l, i))
    DECL_OB(const Type, r, = (Type)vector_at(&m->r, i))
    if(r != m->undef)
      CHECK_BB(isa(l, r))
  }
  return GW_OK;
}

static inline void matcher_init(struct Matcher *m) {
  vector_init(&m->l);
  vector_init(&m->r);
}

static inline void matcher_release(struct Matcher *m) {
  vector_release(&m->l);
  vector_release(&m->r);
}

static m_bool tuple_match(const Env env, const Type lhs, const Type rhs) {
  struct Matcher m = { .undef= env->gwion->type[et_undefined] };
  matcher_init(&m);
  get(&m.l, lhs);
  get(&m.r, rhs);
  const m_bool ret = matcher_run(&m);
  matcher_release(&m);
  return ret;
}

static OP_CHECK(opck_at_object_tuple) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data, mut))
  CHECK_BN(tuple_match(env, bin->lhs->info->type, bin->rhs->info->type))
  exp_setvar(bin->rhs, 1);
  return bin->rhs->info->type;
}

static INSTR(TupleAt) {
  POP_REG(shred, SZ_INT);
  if(!*(M_Object*)REG(0)) {

  }
  exit(3);
}

static OP_EMIT(opem_at_object_tuple) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  const Instr instr = emit_add_instr(emit, TupleAt);
  instr->m_val = (m_uint)bin->rhs->info->type;
  return GW_OK;
}

static OP_CHECK(opck_at_tuple_object) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_NN(opck_rassign(env, data, mut))
  if(!bin->rhs->info->type->info->tuple)
    return bin->rhs->info->type;
  CHECK_BN(tuple_match(env, bin->rhs->info->type, bin->lhs->info->type))
  exp_setvar(bin->rhs, 1);
  set_decl_ref(bin->rhs);
  return bin->rhs->info->type;
}

static OP_CHECK(opck_cast_tuple_object) {
  const Exp_Cast *cast = (Exp_Cast*)data;
  CHECK_BN(tuple_match(env, cast->exp->info->type, exp_self(cast)->info->type))
  return exp_self(cast)->info->type;
}

static INSTR(Tuple2Object) {
  const M_Object o = *(M_Object*)(shred->reg - instr->m_val2);
  const Type t = (Type)instr->m_val;
  if(o && isa(o->type_ref, t) < 0)
  // TODO: pass position by m_val2
//    Except(shred, "can't cast %s to %s\n", o->type_ref->name, t->name);
    Except(shred, _("can't cast\n"));
}

// TODO: do not emit Tuple2Object if full match
static OP_EMIT(opem_cast_tuple_object) {
  const Exp_Cast* exp = (Exp_Cast*)data;
  const Instr instr = emit_add_instr(emit, Tuple2Object);
  instr->m_val = (m_uint)exp_self(exp)->info->type;
  instr->m_val2 = SZ_INT;
  return GW_OK;
}

static OP_CHECK(opck_cast_tuple) {
  const Exp_Cast *cast = (Exp_Cast*)data;
  CHECK_BN(tuple_match(env, exp_self(cast)->info->type, cast->exp->info->type))
  return exp_self(cast)->info->type;
}

static OP_CHECK(opck_impl_tuple) {
  struct Implicit *imp = (struct Implicit*)data;
  CHECK_BN(tuple_match(env, imp->e->info->type, imp->t))
  return imp->t;
}

INSTR(TupleCtor) {
// most of this could be expressed with regular instructions
  const Type t = (Type)instr->m_val;
  const M_Object o = new_object(shred->info->vm->gwion->mp, shred, t);
  const m_uint sz = t->nspc->info->offset;
  memcpy(o->data, shred->reg - sz - SZ_INT, sz);
  shred->reg -= (sz);
  *(M_Object*)(shred->reg - SZ_INT) = o;
}

ANN static Symbol tuple_sym(const Env env, const Vector v) {
  GwText text = { .mp=env->gwion->mp };
  text_add(&text, TUPLE_NAME);
  text_add(&text, ":[");
  for(m_uint i = 0; i < vector_size(v); ++i) {
    const Type t = (Type)vector_at(v, i);
    if(t != (Type)1) {
      struct loc_t_ pos = {};
      const m_str str = type2str(env->gwion, t, &pos);
      text_add(&text, str);
      free_mstr(env->gwion->mp, str);
    } else
    text_add(&text, "_");
    if(i+1 < vector_size(v))
      text_add(&text, ",");
  }
  text_add(&text, "]");
  const Symbol sym = insert_symbol(env->gwion->st, text.str);
  text_release(&text);
  return sym;
}

ANN static Exp decl_from_id(const Gwion gwion, const Type type, Symbol name, const loc_t pos) {
  Type_Decl *td = type != (Type)1 ?
      type2td(gwion, type, pos) :
      new_type_decl(gwion->mp, insert_symbol(gwion->st, "@Undefined"), loc_cpy(gwion->mp, pos));
  td->flag -= ae_flag_late;
  const Var_Decl var = new_var_decl(gwion->mp, name, NULL, loc_cpy(gwion->mp, pos));
  const Var_Decl_List vlist = new_var_decl_list(gwion->mp, var, NULL);
  return new_exp_decl(gwion->mp, td, vlist, loc_cpy(gwion->mp, pos));
}

ANN Type tuple_type(const Env env, const Vector v, const loc_t pos) {
  const Symbol sym = tuple_sym(env, v);
  const Type exists = nspc_lookup_type0(env->curr, sym);
  if(exists)
    return exists;
  Stmt_List base = NULL, curr = NULL;
  for(m_uint i = 0; i < vector_size(v); ++i) {
    char name[num_digit(i) + 16];
    sprintf(name, "@tuple member %"UINT_F, i);
    const Symbol sym = insert_symbol(env->gwion->st, name);
    const Type t = (Type)vector_at(v, i);
//    const Symbol tsym = insert_symbol(t != (Type)1 ? t->name : "@Undefined");
    Exp decl = decl_from_id(env->gwion, t, sym, pos);
    const Stmt stmt = new_stmt_exp(env->gwion->mp, ae_stmt_exp, decl, loc_cpy(env->gwion->mp, pos));
    const Stmt_List slist = new_stmt_list(env->gwion->mp, stmt, NULL);
    if(curr)
      curr->next = slist;
    if(!base)
      base = slist;
    curr = slist;
  }
  Section * section = new_section_stmt_list(env->gwion->mp, base);
  Ast body = new_ast(env->gwion->mp, section, NULL);
  Type_Decl *td = new_type_decl(env->gwion->mp, insert_symbol(env->gwion->st, TUPLE_NAME), loc_cpy(env->gwion->mp, pos));
  Class_Def cdef = new_class_def(env->gwion->mp, ae_flag_none,
        sym, td, body, loc_cpy(env->gwion->mp, pos));
  SET_FLAG(cdef, abstract | ae_flag_final);
  CHECK_BO(scan0_class_def(env, cdef))
//  SET_FLAG(cdef->base.type, abstract | ae_flag_final | ae_flag_late);
//  set_tflag(cdef->base.type, tflag_empty);
  CHECK_BO(traverse_class_def(env, cdef))
  return cdef->base.type;
}

ANN Type check_array_access(const Env env, const Array_Sub array);

static OP_CHECK(opck_tuple) {
  const Array_Sub array = (Array_Sub)data;
  const Exp exp = array->exp;
  if(exp->exp_type != ae_exp_primary ||
     exp->d.prim.prim_type != ae_prim_num)
    ERR_O(exp->pos, _("tuple subscripts must be litteral"))
  const m_uint idx = exp->d.prim.d.num;
  const Vector v = array->type->info->tuple ?
    &array->type->info->tuple->types : NULL;
  if(!v || idx >= vector_size(v))
    ERR_O(exp->pos, _("tuple subscripts too big"))
  const Type type = (Type)vector_at(v, idx);
  if(type == env->gwion->type[et_undefined])
    ERR_O(exp->pos, _("tuple subscripts is undefined at index %"UINT_F), idx)
  if(!exp->next)
    return type;
  struct Array_Sub_ next = { exp->next, type, array->depth - 1 };
  return check_array_access(env, &next);
}

static OP_CHECK(opck_tuple_ctor) {
  const Exp_Call *call = (Exp_Call*)data;
  const Exp exp = call->args;
  if(exp)
    CHECK_OO(check_exp(env, exp))
  struct Vector_ v;
  vector_init(&v);
  Exp e = exp;
  do vector_add(&v, (m_uint)e->info->type);
  while((e = e->next));
  const Type ret = tuple_type(env, &v, exp_self(call)->pos);
  vector_release(&v);
  return ret;
}

static OP_EMIT(opem_tuple_ctor) {
  const Exp_Call *call = (Exp_Call*)data;
  const Instr instr = emit_add_instr(emit, TupleCtor);
  instr->m_val = (m_uint)exp_self(call)->info->type;
  return GW_OK;
}

static OP_CHECK(unpack_ck) {
  const Exp_Call *call = (Exp_Call*)data;
  const Symbol decl = insert_symbol(env->gwion->st, "auto");
  const Symbol skip = insert_symbol(env->gwion->st, "_");
  Exp e = call->args;
//  const Value v = nspc_lookup_value1(env->global_nspc, insert_symbol("false"));
  while(e) {
    if(e->exp_type != ae_exp_primary || e->d.prim.prim_type != ae_prim_id)
      ERR_O(e->pos, _("invalid expression for unpack"))
    if(e->d.prim.d.var != skip) {
      const Symbol var = e->d.prim.d.var;
      memset(&e->d, 0, sizeof(union exp_data));
      e->info->type = env->gwion->type[et_auto];
      e->d.exp_decl.type = env->gwion->type[et_auto];
      e->exp_type = ae_exp_decl;
//      e->d.exp_decl.td = new_type_decl(env->gwion->mp, new_id_list(env->gwion->mp, decl, loc_cpy(env->gwion->mp, e->pos)));
      e->d.exp_decl.td = new_type_decl(env->gwion->mp, decl, loc_cpy(env->gwion->mp, e->pos));
      e->d.exp_decl.list = new_var_decl_list(env->gwion->mp,
        new_var_decl(env->gwion->mp, var, NULL, loc_cpy(env->gwion->mp, e->pos)), NULL);
//      e->d.exp_decl.list->self->value = v;
    } else {
      e->d.prim.prim_type = ae_prim_nil;
      e->info->type = env->gwion->type[et_undefined];
    }
    e = e->next;
  }
  exp_setmeta(exp_self(call), 0);
  return call->func->info->type->info->base_type;
}

static OP_EMIT(unpack_em) {
  const Exp_Call *call = (Exp_Call*)data;
  if(exp_getmeta(exp_self(call)))
    return GW_OK;
  env_err(emit->env, exp_self(call)->pos, _("unused Tuple unpack"));
  return GW_ERROR;
}

static void parents(const Env env, const Type t, const Vector v) {
  const Nspc parent = t->info->owner;
  if(parent->parent && parent != env->context->nspc && parent != env->global_nspc) {
    const Type older = nspc_lookup_type1(parent->parent, insert_symbol(env->gwion->st, parent->name));
    parents(env, older, v);
  }
  vector_add(v, (vtype)insert_symbol(env->gwion->st, t->name));
}

static OP_CHECK(opck_at_unpack) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  Exp e = bin->rhs->d.exp_call.args;
  int i = 0;
  while(e) {
    if(e->exp_type == ae_exp_decl) {
      DECL_OO(const Type, t, = (Type)VPTR(&bin->lhs->info->type->info->tuple->types, i))
      struct Vector_ v; // hoist?
      vector_init(&v);
      parents(env, t, &v);
//      ID_List id = e->d.exp_decl.td->xid;
//      id->xid = (Symbol)vector_front(&v);
//      for(m_uint i = 1; i < vector_size(&v); ++i)
//        id = (id->next = new_id_list(env->gwion->mp, (Symbol)vector_at(&v, i), loc_cpy(env->gwion->mp, e->pos)));
      Type_Decl *td = e->d.exp_decl.td;
      td->xid = (Symbol)vector_front(&v);
      for(m_uint i = 1; i < vector_size(&v); ++i)
        td = (td->next = new_type_decl(env->gwion->mp, (Symbol)vector_at(&v, i), loc_cpy(env->gwion->mp, e->pos)));
      vector_release(&v);
      const Exp next = e->next;
      e->d.exp_decl.type = NULL;
      e->next = NULL;
      const m_bool ret = traverse_exp(env, e);
      e->next = next;
      CHECK_BO(ret)
      exp_setmeta(bin->rhs, 1);
    }
    ++i;
    e = e->next;
  }
  return bin->lhs->info->type;
}

static OP_EMIT(opem_at_unpack) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  if(bin->rhs->d.exp_call.args) {
    const Type t_undef = emit->gwion->type[et_undefined];
    Exp e = bin->rhs->d.exp_call.args;
    m_uint sz = 0;
    do if(e->info->type != t_undef)
      sz += e->info->type->size;
    while((e = e->next));
    const Instr pop = emit_add_instr(emit, RegPop);
    pop->m_val = sz;
    const Vector v = &bin->lhs->info->type->info->tuple->types;
    struct TupleEmit te = { .e=bin->rhs->d.exp_call.args, .v=v };
    emit_unpack_instr(emit, &te);
    const Instr pop2 = emit_add_instr(emit, RegPop);
    pop2->m_val = SZ_INT;

  }
  return GW_OK;
}

static ANN Type scan_tuple(const Env env, const Type_Decl *td) {
  struct Vector_ v;
  vector_init(&v);
  Type_List tl = td->types;
  do {
    const Type t = tl->td->xid != insert_symbol(env->gwion->st, "_") ?
       known_type(env, tl->td) : (Type)1;
    if(t)
      vector_add(&v, (m_uint)t);
    else {
      vector_release(&v);
      return env->gwion->type[et_error];
    }
  } while((tl = tl->next));
  const Type ret = tuple_type(env, &v, td_pos(td));
  vector_release(&v);
  return ret;
}

static OP_CHECK(opck_tuple_scan) {
  struct TemplateScan *ts = (struct TemplateScan*)data;
  return ts->td->types ? scan_tuple(env, ts->td) : ts->t;
}

ANN static void tuple_access(const  Emitter emit, const m_uint idx,
        const m_bool is_var) {
  const Instr instr = emit_add_instr(emit, TupleMember);
  instr->m_val = idx;
  instr->m_val2 = is_var;
  emit_add_instr(emit, DotMember); // just a place holder.
}

static OP_EMIT(opem_tuple_access) {
  struct ArrayAccessInfo *info = (struct ArrayAccessInfo*)data;
  const m_uint idx = info->array.exp->d.prim.d.num;
  tuple_access(emit, idx, (info->array.depth -1)? 0 : info->is_var);
  if(!info->array.exp->next)
    return GW_OK;
  const Type type = (Type)vector_at(&info->array.type->info->tuple->types, idx);
  struct Array_Sub_ next = { info->array.exp->next, type, info->array.depth - 1 };
  info->array = next;
  return emit_array_access(emit, info);
}

GWION_IMPORT(tuple) {
  const Type t_tuple = gwi_mk_type(gwi, TUPLE_NAME, SZ_INT, "Object");
  gwi_add_type(gwi, t_tuple);
  SET_FLAG(t_tuple, abstract);
  set_tflag(t_tuple, tflag_ntmpl | tflag_tmpl);
  GWI_BB(gwi_oper_ini(gwi, NULL, TUPLE_NAME, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_tuple_ctor))
  GWI_BB(gwi_oper_emi(gwi, opem_tuple_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))
  GWI_BB(gwi_oper_ini(gwi, TUPLE_NAME, NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_tuple_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))

  GWI_BB(gwi_oper_ini(gwi, "Object", TUPLE_NAME, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_at_object_tuple))
//  GWI_BB(gwi_oper_emi(gwi, opem_at_object_tuple))
  GWI_BB(gwi_oper_end(gwi, "@=>", ObjectAssign))
  GWI_BB(gwi_oper_add(gwi, opck_cast_tuple))
  GWI_BB(gwi_oper_end(gwi, "$", NoOp))
  GWI_BB(gwi_oper_add(gwi, opck_impl_tuple))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
//  GWI_BB(gwi_oper_ini(gwi, TUPLE_NAME, "Object", NULL))
//  GWI_BB(gwi_oper_add(gwi, opck_at_tuple_object))
//  GWI_BB(gwi_oper_emi(gwi, opem_at_tuple_object))
//  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
//  GWI_BB(gwi_oper_add(gwi, opck_cast_tuple_object))
//  GWI_BB(gwi_oper_emi(gwi, opem_cast_tuple_object))
//  GWI_BB(gwi_oper_end(gwi, "$", NULL))
//  GWI_BB(gwi_oper_add(gwi, opck_impl_tuple))
//  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
//  GWI_BB(gwi_oper_ini(gwi, TUPLE_NAME, "Tuple", NULL))
//  GWI_BB(gwi_oper_add(gwi, opck_at_object_tuple))
//  GWI_BB(gwi_oper_end(gwi, "@=>", ObjectAssign))
  GWI_BB(gwi_oper_ini(gwi, "int", TUPLE_NAME, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_tuple))
  GWI_BB(gwi_oper_emi(gwi, opem_tuple_access))
  GWI_BB(gwi_oper_end(gwi, "@array", NULL))
  gwi_register_freearg(gwi, TupleUnpack, freearg_tuple_at);

  const Type t_unpack = gwi_mk_type(gwi, "Unpack", 0, NULL);
  SET_FLAG(t_unpack, abstract | ae_flag_final);
  gwi_add_type(gwi, t_unpack);
  GWI_BB(gwi_oper_ini(gwi, NULL, "Unpack", NULL))
  GWI_BB(gwi_oper_add(gwi, unpack_ck))
  GWI_BB(gwi_oper_emi(gwi, unpack_em))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Object", "Unpack", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_at_unpack))
  GWI_BB(gwi_oper_emi(gwi, opem_at_unpack))
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
//  GWI_BB(gwi_oper_add(gwi, opck_at_unpack))
//  GWI_BB(gwi_oper_emi(gwi, opem_at_unpack))
//  GWI_BB(gwi_oper_end(gwi, "==", NULL))
  return GW_OK;
}
