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
#include "gwi.h"
#include "gack.h"
#include "traverse.h"
#include "emit.h"

static GACK(gack_dynclass) {
  INTERP_PRINTF("%s", (*(Type*)VALUE)->name);
}

static m_uint safe_map_size(const Map m) {
  return m ? map_size(m) : 0;
}

static m_uint fill_values(const VM_Shred shred, const Type base, const M_Vector v, const Type t, const M_Object data, const m_uint offset) {
  const Map m = base->nspc ? &base->nspc->info->value->map : NULL;
  const Gwion gwion = shred->info->vm->gwion;
  const m_uint new_offset = ((base->info->parent ? fill_values(shred, base->info->parent, v, t, data, offset) : 0)) + offset;
  const m_uint sz = safe_map_size(m);
  for(m_uint i = 0; i < sz; i++) {
    const Value value = (Value)map_at(m, i);
    M_Object o = *(M_Object*)(ARRAY_PTR(v) + (i + new_offset) * ARRAY_SIZE(v)) = new_object(shred->info->mp, NULL, t);
    *(Type*)o->data = type_class(gwion, value->type);
    *(M_Object*)(o->data + SZ_INT) = new_string2(gwion, NULL, value->type->name);
    *(M_Object*)(o->data + SZ_INT*2) = new_string2(gwion, NULL, value->name);
    *(m_uint*)(o->data + SZ_INT*3) = vflag(value, vflag_member);
    *(m_uint*)(o->data + SZ_INT*4) = is_func(gwion, value->type);

    if(data) {
      if(!is_func(gwion, value->type)) {
        if(vflag(value, vflag_member)) {
          *(m_bit**)(o->data + SZ_INT*5) = &*(m_bit*)(data->data + value->from->offset);
        } else {
          *(m_bit**)(o->data + SZ_INT*5) = &*(m_bit*)(t->nspc->class_data + value->from->offset);
        }
      } else
        *(m_uint**)(o->data + SZ_INT*5) = &*(m_uint*)(t->nspc->vtable.ptr + value->from->offset);
    }
  }
  return new_offset + sz;
}

ANN static m_uint count_values(const Type t, const m_uint base) {
  const Map m = t->nspc ? &t->nspc->info->value->map : NULL;
  const m_uint new_base = safe_map_size(m) + base;
  if(t->info->parent)
    return count_values(t->info->parent, new_base);
  return new_base;
}

static SFUN(reflection) {
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
  const Type t = (*(Type*)MEM(0));
  const Map m = t->nspc ? &t->nspc->info->value->map : NULL;
//  const m_uint sz = map_size(m);
  const m_uint sz = count_values(t, 0);
  const M_Object ret = new_array(shred->info->mp, code->ret_type, sz);
  vector_add(&shred->gc, (m_uint)ret);
  *(M_Object*)RETURN = ret;
  if(!sz)return;
  fill_values(shred, t, ARRAY(ret), code->ret_type->info->base_type, NULL, 0);
}

static SFUN(reflection_object) {
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
  const M_Object arg = *(M_Object*)MEM(0);
  const Type t = arg->type_ref;
  const Map m = t->nspc ? &t->nspc->info->value->map : NULL;
  const m_uint sz = count_values(t, 0);
//  const m_uint sz = map_size(m);
  const M_Object ret = new_array(shred->info->mp, code->ret_type, sz);
  vector_add(&shred->gc, (m_uint)ret);
  *(M_Object*)RETURN = ret;
  if(!sz)return;
  fill_values(shred, t, ARRAY(ret), code->ret_type->info->base_type, arg, 0);
}


static SFUN(reflection_typeof) {
  *(Type*)RETURN = o->type_ref;
}

static SFUN(reflection_typename) {
  *(M_Object*)RETURN = new_string(shred->info->mp, shred, (*(Type*)MEM(0))->name);
}

static OP_CHECK(opck_typeof) {
  const Exp_Call *call = (Exp_Call*)data;
  DECL_ON(const Type, ret, = check_exp(env, call->args));
  if(isa(ret, env->gwion->type[et_object]) > 0)
    return call->func->type->info->func->def->base->ret_type;
  call->func->d.prim.d.var = insert_symbol(env->gwion->st, "@typeof");
  free_exp(env->gwion->mp, call->func);
  free_exp(env->gwion->mp, call->args);
  const Exp e = exp_self(call);
  e->exp_type = ae_exp_td;
  Type_Decl *td = type2td(env->gwion, ret, e->pos);
  e->d.exp_td = td;
  return e->type = type_class(env->gwion, ret);
}

static OP_CHECK(opck_reflection_cast) {
  Exp_Cast *cast = (Exp_Cast*)data;
  return known_type(env, cast->td);
}

static INSTR(reflection_cast) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  if(isa((*(Type*)o->data)->info->base_type, ((Type)instr->m_val)->info->base_type) > 0) {
    const m_bit *base  = *(m_bit**)(o->data + SZ_INT*5);
    shred->reg += instr->m_val2 - SZ_INT;
    memcpy(shred->reg - instr->m_val2, base, instr->m_val2);
  } else
    handle(shred, "ReflectionCastExpression");
}

static OP_EMIT(opem_reflection_cast) {
  Exp_Cast *cast = (Exp_Cast*)data;
  const Type t = exp_self(cast)->type;
  const Instr instr = emit_add_instr(emit, reflection_cast);
  instr->m_val  = (m_uint)type_class(emit->gwion, t);
  instr->m_val2 = t->size;
  return GW_OK;
}
GWION_IMPORT(Reflection) {
  const Type t_dynclass = gwi_mk_type(gwi, "DynClass", SZ_INT, NULL);
  gwi_add_type(gwi, t_dynclass);
  GWI_BB(gwi_gack(gwi, t_dynclass, gack_dynclass))

  DECL_OB(const Type, t_reflection, = gwi_struct_ini(gwi, "Reflection"));
    DECL_OB(const Type, t_value, = gwi_class_ini(gwi, "Value", NULL));
    CHECK_BB(gwi_item_ini(gwi, "DynClass", "type"));
    CHECK_BB(gwi_item_end(gwi, ae_flag_none, num, 0));
    CHECK_BB(gwi_item_ini(gwi, "string", "typename"));
    CHECK_BB(gwi_item_end(gwi, ae_flag_none, num, 0));
    CHECK_BB(gwi_item_ini(gwi, "string", "name"));
    CHECK_BB(gwi_item_end(gwi, ae_flag_none, num, 0));
    CHECK_BB(gwi_item_ini(gwi, "bool", "member"));
    CHECK_BB(gwi_item_end(gwi, ae_flag_none, num, 0));
    CHECK_BB(gwi_item_ini(gwi, "bool", "func"));
    CHECK_BB(gwi_item_end(gwi, ae_flag_none, num, 0));
    CHECK_BB(gwi_class_end(gwi));

    DECL_OB(const Type, t_instance, = gwi_class_ini(gwi, "Instance", "Value"));
    t_instance->nspc->offset += SZ_INT; // hold a pointer to the data
    SET_FLAG(t_instance, abstract | ae_flag_final);
    CHECK_BB(gwi_class_end(gwi));
    SET_FLAG(t_value, abstract | ae_flag_final);

  CHECK_BB(gwi_struct_end(gwi));

  gwidoc(gwi, "Reflect a class");
  CHECK_BB(gwi_func_ini(gwi, "Reflection.Value[]", "Reflect"));
  CHECK_BB(gwi_func_arg(gwi, "Class", "t"));
  CHECK_BB(gwi_func_end(gwi, reflection, ae_flag_none));

  gwidoc(gwi, "Reflect a class");
  CHECK_BB(gwi_func_ini(gwi, "Reflection.Value[]", "Reflect"));
  CHECK_BB(gwi_func_arg(gwi, "DynClass", "t"));
  CHECK_BB(gwi_func_end(gwi, reflection, ae_flag_none));

  gwidoc(gwi, "Reflect and object");
  CHECK_BB(gwi_func_ini(gwi, "Reflection.Instance[]", "Reflect"));
  CHECK_BB(gwi_func_arg(gwi, "Object", "o"));
  CHECK_BB(gwi_func_end(gwi, reflection_object, ae_flag_none));

  gwidoc(gwi, "typeof function");
  CHECK_BB(gwi_func_ini(gwi, "DynClass", "typeof:[T]"));
  CHECK_BB(gwi_func_arg(gwi, "T", "t"));
  CHECK_BB(gwi_func_end(gwi, reflection_typeof, ae_flag_none));

  set_tflag(t_dynclass, tflag_infer);
  SET_FLAG(t_dynclass, abstract);
  const Func             f      = nspc_lookup_func1(gwi->gwion->env->curr, insert_symbol(gwi->gwion->st, "typeof"));
  const struct Op_Func   opfunc = {.ck = opck_typeof};
  const struct Op_Import opi    = {
      .lhs  = NULL,
      .rhs  = f->value_ref->type,
      .ret = NULL,
      .func = &opfunc,
      .data = (uintptr_t)f,
      .nspc = NULL,
      .op   = insert_symbol(gwi->gwion->st, (m_str)"@func_check"),
      .pos  = gwi->loc
  };
  CHECK_BB(add_op(gwi->gwion, &opi));

  gwidoc(gwi, "typename function");
  CHECK_BB(gwi_func_ini(gwi, "string", "typename"));
  CHECK_BB(gwi_func_arg(gwi, "Class", "t"));
  CHECK_BB(gwi_func_end(gwi, reflection_typename, ae_flag_none));

  CHECK_BB(gwi_func_ini(gwi, "string", "typename"));
  CHECK_BB(gwi_func_arg(gwi, "DynClass", "t"));
  CHECK_BB(gwi_func_end(gwi, reflection_typename, ae_flag_none));

  gwidoc(gwi, "compare DynClass and DynClass");
  GWI_BB(gwi_oper_ini(gwi, "DynClass", "DynClass", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==", int_eq))

  gwidoc(gwi, "compare DynClass and Class");
  GWI_BB(gwi_oper_ini(gwi, "DynClass", "Class", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==", int_eq))

  gwidoc(gwi, "compare Class and DynClass");
  GWI_BB(gwi_oper_ini(gwi, "Class", "DynClass", "bool"))
  GWI_BB(gwi_oper_end(gwi, "==", int_eq))

  gwidoc(gwi, "allow casting a Reflection.Instance to any type");
  GWI_BB(gwi_oper_ini(gwi, "Reflection.Instance", (m_str)OP_ANY_TYPE, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_reflection_cast));
  GWI_BB(gwi_oper_emi(gwi, opem_reflection_cast));
  GWI_BB(gwi_oper_end(gwi, "$", NULL))

  GWI_BB(gwi_oper_ini(gwi, "Reflection.Instance", (m_str)"Object", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_reflection_cast));
  GWI_BB(gwi_oper_emi(gwi, opem_reflection_cast));
  GWI_BB(gwi_oper_end(gwi, "$", NULL))

  return GW_OK;
}
