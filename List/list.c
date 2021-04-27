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
#include "ugen.h"
#include "array.h"
#include "traverse.h"
#include "parse.h"
#include "emit.h"

static DTOR(list_dtor) { /*code here */ }

static m_int o_list_val_data;
static m_int o_list_next_data;
static m_int* list_static_value;

static SFUN(sfun) { /*code here */ }

static OP_CHECK(opck_list_array_access) {
  const Array_Sub array = (Array_Sub)data;
//  const Exp e = array->exp;
//  const Type t = (Type)vector_front(&array->type->info->tuple->contains);
//  if(!e->next)
    return array->type;
//  struct Array_Sub_ next = { e->next, t, t->array_depth };
//  return check_array_access(env, &next) ?: env->gwion->type[et_error];
}

static INSTR(ListAccess) {
  const Type t  = (Type)instr->m_val;
  shred->reg -= SZ_INT*2 - t->size;
  const m_int idx = *(m_uint*)REG(SZ_INT - t->size);
  if(idx < 0) {
    handle(shred, "NegativeListIndex");
    return;
  }
  M_Object o = *(M_Object*)REG(-t->size);
  for(m_uint i = 0; i < idx; ++i) {
    if(!(o = *(M_Object*)(o->data + t->size))) {
      handle(shred, "EmptyList");
      return;
    }
  }
  *(M_Object*)REG(-SZ_INT) = o;
//  memcpy(shred->reg - t->size, o->data, t->size);
}

static OP_EMIT(opem_list_array_access) {
  const Array_Sub array = (Array_Sub)data;
  CHECK_BB(emit_exp(emit, array->exp));;// emit_exp_pop_next
  const Instr access = emit_add_instr(emit, ListAccess);
  access->m_val = vector_front(&array->type->info->tuple->contains);
  return GW_OK;
}

static OP_CHECK(opck_list_ctor) {
  const Exp_Call *call = (Exp_Call*)data;
  if(!call->tmpl)
    ERR_N(call->func->pos, _("List needs template"));
  CHECK_BN(check_exp(env, call->func))
  DECL_ON(const Type, t, = known_type(env, call->tmpl->call->td));
  Exp e = call->args;
  if(!e)
    ERR_N(call->func->pos, _("List needs argument"));
  CHECK_BN(check_exp(env, call->args))
  do CHECK_BN(check_implicit(env, e, t))
  while((e = e->next));
  Type_Decl td = { .xid=insert_symbol("List"), .types=call->tmpl->call };
  const Type ret = known_type(env, &td);
  CHECK_BN(traverse_class_def(env, ret->info->cdef))
  return ret;
}

static M_Object new_list(const VM_Shred shred, const Type base, const Type t, m_bit *const data) {
  M_Object o = new_object(shred->info->vm->gwion->mp, shred, base);
  memcpy(o->data, data, t->size);
  return o;
}

static INSTR(ListCtor) {
  const Type tbase = (Type)instr->m_val;
  const Type t = (Type)instr->m_val2;
  const m_uint n = *(m_int*)REG(-SZ_INT);
  POP_REG(shred, (n * t->size) + SZ_INT);
  M_Object base = new_list(shred, tbase, t, REG(-t->size)),
    o = base;
  for(m_uint i = 1; i < n; ++i) {
    const M_Object tmp = new_list(shred, tbase, t, REG((i-1)*t->size));
    *(M_Object*)(o->data + t->size) = tmp;
    o = tmp;
  }
  *(M_Object*)REG(-SZ_INT) = base;
}

static OP_EMIT(opem_list_ctor) {
  const Exp_Call *call = (Exp_Call*)data;
  m_uint n = 0;
  Exp e = call->args;
  do ++n;
  while((e = e->next));
  const Type t = known_type(emit->env, call->tmpl->call->td);
  if(isa(t, emit->gwion->type[et_compound]) > 0) {
    for(m_uint i = 0; i < n; ++i)
      emit_compound_addref(emit, t, -(i+1)*t->size, 0);
  }
  const Instr ninstr = emit_add_instr(emit, RegPushImm);
  ninstr->m_val = n;
  const Instr instr = emit_add_instr(emit, ListCtor);
  instr->m_val = (m_uint)exp_self(call)->type;
  instr->m_val2 = (m_uint)t;
  return GW_OK;
}

static OP_CHECK(opck_list_append_front) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type t = (Type)vector_front(&bin->rhs->type->info->tuple->contains);
  CHECK_BN(check_implicit(env, bin->lhs, t))
  exp_setvar(bin->rhs, 1);
  return bin->rhs->type;
}

static INSTR(ListAppendFront) {
  const Type tbase = (Type)instr->m_val;
  const Type t = (Type)instr->m_val2;
  POP_REG(shred, t->size);
  const M_Object o = **(M_Object**)REG(t->size -SZ_INT);
  const M_Object list = new_list(shred, tbase, tbase, REG(-t->size));
  *(M_Object*)REG(-SZ_INT) = **(M_Object**)REG(t->size-SZ_INT) = list;
  *(M_Object*)(list->data + t->size) = o;
}

static OP_EMIT(opem_list_append_front) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type t = (Type)vector_front(&bin->rhs->type->info->tuple->contains);
  const Instr instr = emit_add_instr(emit, ListAppendFront);
  instr->m_val = (m_uint)bin->rhs->type;
  instr->m_val2 = (m_uint)bin->lhs->type;
  return GW_OK;
}

static OP_CHECK(opck_list_append_back) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type t = (Type)vector_front(&bin->lhs->type->info->tuple->contains);
  CHECK_BN(check_implicit(env, bin->rhs, t))
  return bin->lhs->type;
}

static INSTR(ListAppendBack) {
  const Type tbase = (Type)instr->m_val;
  const Type t = (Type)instr->m_val2;
  POP_REG(shred, t->size);
  M_Object o = *(M_Object*)REG(-SZ_INT), next;
  while((next = *(M_Object*)(o->data + t->size)))
    o = next;
  *(M_Object*)(o->data + t->size) = new_list(shred, tbase, tbase, REG(0));
}

static OP_EMIT(opem_list_append_back) {
  Exp_Binary *bin = (Exp_Binary*)data;
  const Type t = (Type)vector_front(&bin->lhs->type->info->tuple->contains);
  const Instr instr = emit_add_instr(emit, ListAppendBack);
  instr->m_val = (m_uint)bin->lhs->type;
  instr->m_val2 = (m_uint)bin->rhs->type;
  return GW_OK;
}

static OP_CHECK(opck_list_slice) {
  Exp_Binary *bin = (Exp_Binary*)data;
  return bin->lhs->type;
}

static OP_EMIT(opem_list_slice) {
/*
  M_Object tmp = o;
  m_int i = 0;
  for(m_uint i = 0; i < idx; ++i) {
    if(!(tmp = *(M_Object*)(tmp->data + t->size))) {
      *(m_uint*)RETURN = 0;
      return;
    }
*/
}

static MFUN(list_size) {
  m_uint i = 1;
  M_Object tmp = o;
  const Type t = (Type)vector_front(&o->type_ref->info->tuple->contains);
  const size_t sz = t->size;
  while((tmp = *(M_Object*)(tmp->data + sz)))
    i++;
  *(m_uint*)RETURN = i;
}

static MFUN(list_remove_at) {
  const m_int idx = *(m_int*)MEM(SZ_INT);
  const Type t = (Type)vector_front(&o->type_ref->info->tuple->contains);
  if(idx > 0) {
    M_Object tmp = o;
    m_int i = 0;
    for(m_uint i = 0; i < idx; ++i) {
      if(!(tmp = *(M_Object*)(tmp->data + t->size))) {
        *(m_uint*)RETURN = 0;
        return;
      }
    }
    const M_Object remove = *(M_Object*)(tmp->data + t->size);
    if(remove) {
      const M_Object next = *(M_Object*)(remove->data + t->size);
      *(M_Object*)(tmp->data + t->size) = next;
    }
    release(remove, shred);
    *(m_uint*)RETURN = 1;
  } else
  *(m_uint*)RETURN = 0;
}

static MFUN(list_insert) {
exit(3);
}

GWION_IMPORT(List) {
  DECL_OB(const Type, t_blist, = gwi_class_ini(gwi, "@List", "Object"));
  CHECK_BB(gwi_func_ini(gwi, "bool", "removeAt"));;
  CHECK_BB(gwi_func_arg(gwi, "int", "index"));;
  CHECK_BB(gwi_func_end(gwi, list_remove_at, ae_flag_none));;
  CHECK_BB(gwi_func_ini(gwi, "int", "size"));;
  CHECK_BB(gwi_func_end(gwi, list_size, ae_flag_none));;
  GWI_BB(gwi_class_end(gwi))

  DECL_OB(const Type, t_list, = gwi_class_ini(gwi, "List:[T]", "@List"));
  SET_FLAG(t_list, abstract);
    GWI_BB(gwi_item_ini(gwi, "T", "val"))
    CHECK_BB((o_list_val_data = gwi_item_end(gwi, ae_flag_none, num, 0)));;
    GWI_BB(gwi_item_ini(gwi, "List:[T]", "@next"))
    CHECK_BB((o_list_next_data = gwi_item_end(gwi, ae_flag_late, num, 0)));;

  GWI_BB(gwi_func_ini(gwi, "bool", "insert"))
  GWI_BB(gwi_func_arg(gwi, "T", "data"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, list_insert, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "int", "@List", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_list_array_access))
  GWI_BB(gwi_oper_emi(gwi, opem_list_array_access))
  gwi_oper_eff(gwi, "NegativeListIndex");
  gwi_oper_eff(gwi, "EmptyList");
  GWI_BB(gwi_oper_end(gwi, "@array", NULL))

  GWI_BB(gwi_oper_ini(gwi, NULL, "@List", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_list_ctor))
  GWI_BB(gwi_oper_emi(gwi, opem_list_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  GWI_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@List", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_list_append_front))
  GWI_BB(gwi_oper_emi(gwi, opem_list_append_front))
  GWI_BB(gwi_oper_end(gwi, ">>", NULL))

  GWI_BB(gwi_oper_ini(gwi, "@List", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_list_append_back))
  GWI_BB(gwi_oper_emi(gwi, opem_list_append_back))
  GWI_BB(gwi_oper_end(gwi, "<<", NULL))

  GWI_BB(gwi_oper_ini(gwi, "int", "@List", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_list_slice))
  GWI_BB(gwi_oper_emi(gwi, opem_list_slice))
  GWI_BB(gwi_oper_end(gwi, "@slice", NULL))

  return GW_OK;
}
