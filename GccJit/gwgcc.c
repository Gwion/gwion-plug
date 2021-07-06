#include <libgccjit.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "array.h"
#include "emit.h"
#include "operator.h"
#include "import.h"
#include "traverse.h"
#include "parse.h"
#include "gwi.h"
#include "emit.h"

#include "escape.h"
#include "gwgcc.h"
#include "gccjit_pass.h"

#define EXPAND(v, t) (v)->ptr ? vector_size((v)) : 0, (t)((v)->ptr ? ((v)->ptr + OFFSET) : NULL)

ANN static void cmangle(const Func func, char name[256]) {
  sprintf(name, "%s%"UINT_F, s_name(func->def->base->xid), func->value_ref->from->offset);
}

ANN static gcc_jit_function* get_func(GwGccJit *a, const Func func) {
  char name[256];
  cmangle(func, name);
  const Env env = a->jitter->gwion->env;
  const Symbol sym = insert_symbol(name);
  const m_uint ret = map_get(&a->funcs, (m_uint)sym) ?:
             map_get(&a->jitter->funcs, (m_uint)sym);
  if(ret)
    return (gcc_jit_function*)ret;
/*
  // we should find the func and add it to the environment
  const struct ValueFrom_ *from = func->value_ref->from;
  const m_uint push = env_push(env, from->owner_class, from->owner);
  env_pop(env, push);
*/
  return NULL;
}

ANN static inline enum gcc_jit_types _jit_type(const Gwion gwion, const Type t) {
  if(!t->size)
    return GCC_JIT_TYPE_VOID;
  if(isa(t, gwion->type[et_object]) > 0 || is_func(gwion, t))
    return GCC_JIT_TYPE_VOID_PTR;
  if(isa(t, gwion->type[et_bool]) > 0)
    return GCC_JIT_TYPE_BOOL;
  if(isa(t, gwion->type[et_int]) > 0)
    return GCC_JIT_TYPE_LONG;
}

//! get a gcc_jit_type from a gwion type
ANN static inline gcc_jit_type* jit_type(GwGccJit *a, const Type t) {
  const enum gcc_jit_types jt = _jit_type(a->jitter->gwion, t);
  return gcc_jit_context_get_type(a->ctx, jt);
}


ANN static inline void add_value(GwGccJit *a, const Symbol sym, const void *value) {
  scope_add(&a->value, (m_uint)sym, (m_uint)value);
}

ANN static gcc_jit_block *new_block(GwGccJit *a, const m_str b) {
  gcc_jit_block *block = gcc_jit_function_new_block(a->func, b);
  vector_add(&a->block, (m_uint)block);
  return block;
}

ANN static inline void push_block(GwGccJit *a, gcc_jit_block *block) {
  vector_add(&a->block, (m_uint)block);
}

ANN static inline void pop_block(GwGccJit *a) {
  vector_pop(&a->block);
}

ANN static bool gwgcc_symbol(GwGccJit *a, Symbol b) {
}

ANN static bool gwgcc_array_sub(GwGccJit *a, Array_Sub b) {
  if(b->exp)
    gwgcc_exp(a, b->exp);
}

ANN static bool gwgcc_id_list(GwGccJit *a, ID_List b) {
  gwgcc_symbol(a, b->xid);
  if(b->next)
    gwgcc_id_list(a, b->next);
}

ANN static bool gwgcc_specialized_list(GwGccJit *a, Specialized_List b) {
  gwgcc_symbol(a, b->xid);
  if(b->traits)
    gwgcc_id_list(a, b->traits);
  if(b->next)
    gwgcc_specialized_list(a, b->next);
}

ANN static bool gwgcc_type_list(GwGccJit *a, Type_List b) {
  gwgcc_type_decl(a, b->td);
  if(b->next)
    gwgcc_type_list(a, b->next);
}

ANN static bool gwgcc_tmpl(GwGccJit *a, Tmpl *b) {
  if(b->list)
    gwgcc_specialized_list(a, b->list);
  if(b->call)
    gwgcc_type_list(a, b->call);
}

ANN static bool gwgcc_range(GwGccJit *a, Range *b) {
  if(b->start)
    gwgcc_exp(a, b->start);
  if(b->end)
    gwgcc_exp(a, b->end);
}

ANN static bool gwgcc_type_decl(GwGccJit *a, Type_Decl *b) {
  gwgcc_symbol(a, b->xid);
  if(b->array)
    gwgcc_array_sub(a, b->array);
  if(b->types)
    gwgcc_type_list(a, b->types);
  if(b->next)
    gwgcc_type_decl(a, b->next);
}

ANN static inline gcc_jit_rvalue* gwgcc_prim_id(GwGccJit *a, Symbol *b) {
  const Symbol sym = *b;
  const m_uint ret = scope_lookup1(&a->value, (m_uint)sym) ?:
               map_get(&a->jitter->value, (m_uint)sym);
  return (gcc_jit_rvalue*)ret;
}

ANN static gcc_jit_rvalue* gwgcc_prim_num(GwGccJit *a, m_uint *b) {
  return gcc_jit_context_new_rvalue_from_long (a->ctx,
              gcc_jit_context_get_type (a->ctx, GCC_JIT_TYPE_LONG), *(m_int*)b);
}

ANN static gcc_jit_rvalue* gwgcc_prim_float(GwGccJit *a, m_float *b) {
  return gcc_jit_context_new_rvalue_from_long (a->ctx,
              gcc_jit_context_get_type (a->ctx, GCC_JIT_TYPE_FLOAT), *(m_float*)b);
}

ANN static gcc_jit_rvalue* gwgcc_prim_str(GwGccJit *a, m_str *b) {
  return gcc_jit_context_new_rvalue_from_ptr (a->ctx,
              gcc_jit_context_get_type (a->ctx, GCC_JIT_TYPE_CONST_CHAR_PTR), *(m_str*)b);
}

ANN static gcc_jit_rvalue* gwgcc_prim_array(GwGccJit *a, Array_Sub *b) {
  gwgcc_array_sub(a, *b);
}

ANN static gcc_jit_rvalue* gwgcc_prim_range(GwGccJit *a, Range* *b) {
  gwgcc_range(a, *b);
}

ANN static gcc_jit_rvalue* gwgcc_prim_hack(GwGccJit *a, Exp *b) {
/*
    gcc_jit_function *printf_func =
    gcc_jit_context_new_function (a->ctx, NULL,//loc
				  GCC_JIT_FUNCTION_IMPORTED,
				  gcc_jit_context_get_type (a->ctx, GCC_JIT_TYPE_INT),
				  "printf",
				  2, args,
				  1);
*/
//  gwgcc_exp(a, *b);
}

ANN static gcc_jit_rvalue* gwgcc_prim_typeof(GwGccJit *a, Exp *b) {
  gwgcc_exp(a, *b);
}

ANN static gcc_jit_rvalue* gwgcc_prim_interp(GwGccJit *a, Exp *b) {
  gwgcc_exp(a, *b);
}

ANN static gcc_jit_rvalue* gwgcc_prim_char(GwGccJit *a, m_str *b) {
  const char c = str2char(a->jitter->gwion->emit, *b, prim_pos(b));
  return gcc_jit_context_new_rvalue_from_int (a->ctx,
              gcc_jit_context_get_type (a->ctx, GCC_JIT_TYPE_CHAR), c);
}

ANN static gcc_jit_rvalue* gwgcc_prim_nil(GwGccJit *a, bool *b) {
  return NULL;
}

ANN static gcc_jit_rvalue* gwgcc_prim_perform(GwGccJit *a, Symbol b) {
}

DECL_PRIM_FUNC(gwgcc, gcc_jit_rvalue* , GwGccJit*)
ANN static gcc_jit_rvalue* gwgcc_prim(GwGccJit *a, Exp_Primary *b) {
  return gwgcc_prim_func[b->prim_type](a, &b->d);
}

ANN static bool gwgcc_var_decl(GwGccJit *a, Var_Decl b) {
  if(b->xid)
    gwgcc_symbol(a, b->xid);
  if(b->array)
    gwgcc_array_sub(a, b->array);
}

ANN static bool gwgcc_var_decl_list(GwGccJit *a, Var_Decl_List b) {
  gwgcc_var_decl(a, b->self);
  if(b->next)
    gwgcc_var_decl_list(a, b->next);
}

ANN static gcc_jit_rvalue* gwgcc_exp_decl(GwGccJit *a, Exp_Decl *b) {
  if(b->td)
    gwgcc_type_decl(a, b->td);
  gwgcc_var_decl_list(a, b->list);
}

void *op_get(const Env, struct Op_Import*);
typedef gcc_jit_rvalue*  (*jit_op_func_t )(GwGccJit*, void*);

ANN static gcc_jit_rvalue* gwgcc_exp_binary(GwGccJit *a, Exp_Binary *b) {
  struct Op_Import opi = { .lhs = b->lhs->type, .rhs = b->rhs->type, .op = b->op };
  void *mo = op_get(a->jitter->gwion->env, &opi);
  jit_op_func_t func = (jit_op_func_t)map_get(&a->jitter->op, (m_uint)mo);
  return func ? func(a, b) :NULL;
}

ANN static gcc_jit_rvalue* gwgcc_exp_unary(GwGccJit *a, Exp_Unary *b) {
// assume b->exp for now
  struct Op_Import opi = { .lhs = b->exp->type, .op = b->op };
  void *mo = op_get(a->jitter->gwion->env, &opi);
  jit_op_func_t func = (jit_op_func_t)map_get(&a->jitter->op, (m_uint)mo);
  return func ? func(a, b) :NULL;
/*
  gwgcc_symbol(a, b->op);
  if(b->exp)
    gwgcc_exp(a, b->exp);
  if(b->td)
    gwgcc_type_decl(a, b->td);
  if(b->code)
    gwgcc_stmt(a, b->code);
*/
}

ANN static gcc_jit_rvalue* gwgcc_exp_cast(GwGccJit *a, Exp_Cast *b) {
  gwgcc_type_decl(a, b->td);
  gwgcc_exp(a, b->exp);
}

ANN static gcc_jit_rvalue* gwgcc_exp_post(GwGccJit *a, Exp_Postfix *b) {
  struct Op_Import opi = { .rhs = b->exp->type, .op = b->op };
  void *mo = op_get(a->jitter->gwion->env, &opi);
  jit_op_func_t func = (jit_op_func_t)map_get(&a->jitter->op, (m_uint)mo);
  return func ? func(a, b) :NULL;
}

ANN static void func_call_args(GwGccJit *a, Exp arg, const Vector v) {
  if(!arg)
    return;
  vector_init(v);
  do {
    gcc_jit_rvalue *jit_arg = gwgcc_exp1(a, arg);
    vector_add(v, (m_uint)jit_arg);
  } while ((arg = arg->next));
}

ANN static gcc_jit_rvalue* gwgcc_exp_call(GwGccJit *a, Exp_Call *b) {
  gcc_jit_function *func = get_func(a, b->func->type->info->func);
  gcc_jit_block* block = (gcc_jit_block*)vector_back(&a->block);
  gcc_jit_location *loc = gcc_jit_context_new_location(a->ctx, a->jitter->gwion->env->name,
     b->func->pos.first.line, b->func->pos.first.column);
  struct Vector_ args;
  func_call_args(a, b->args, &args);
  gcc_jit_rvalue* ret = gcc_jit_context_new_call(a->ctx, loc, func, EXPAND(&args, gcc_jit_rvalue**));
  if(args.ptr)
    vector_release(&args);
  return ret;
}

ANN static gcc_jit_rvalue* gwgcc_exp_array(GwGccJit *a, Exp_Array *b) {
  gwgcc_exp(a, b->base);
  gwgcc_array_sub(a, b->array);
}

ANN static gcc_jit_rvalue* gwgcc_exp_slice(GwGccJit *a, Exp_Slice *b) {
  gwgcc_exp(a, b->base);
  gwgcc_range(a, b->range);
}

ANN static gcc_jit_rvalue* gwgcc_exp_if(GwGccJit *a, Exp_If *b) {
  gwgcc_exp(a, b->cond);
  if(b->if_exp)
    gwgcc_exp(a, b->if_exp);
  gwgcc_exp(a, b->else_exp);
}

ANN static gcc_jit_rvalue* gwgcc_exp_dot(GwGccJit *a, Exp_Dot *b) {
  gwgcc_exp(a, b->base);
  gwgcc_symbol(a, b->xid);
}

ANN static gcc_jit_rvalue* gwgcc_exp_lambda(GwGccJit *a, Exp_Lambda *b) {
  gwgcc_func_def(a, b->def);
//  char c[256];
//  cmangle(b->def->base->func, c);
//  return get_func(a, c);
}

ANN static gcc_jit_rvalue* gwgcc_exp_td(GwGccJit *a, Type_Decl *b) {
  return gcc_jit_context_new_rvalue_from_ptr (a->ctx,
              gcc_jit_context_get_type (a->ctx, GCC_JIT_TYPE_VOID_PTR), exp_self(b)->type);
}

DECL_EXP_FUNC(gwgcc, gcc_jit_rvalue*, GwGccJit*)
ANN static gcc_jit_rvalue* gwgcc_exp1(GwGccJit *a, Exp b) {
  return gwgcc_exp_func[b->exp_type](a, &b->d);
}

ANN gcc_jit_rvalue* gwgcc_exp(GwGccJit *a, Exp b) {
  gcc_jit_rvalue* ret = gwgcc_exp_func[b->exp_type](a, &b->d);
  if(b->next)
    gwgcc_exp(a, b->next);
  return ret;
}

ANN static bool gwgcc_stmt_exp(GwGccJit *a, Stmt_Exp b) {
  if(b->val) {
    gwgcc_exp(a, b->val);
  }
}

ANN static bool gwgcc_stmt_while(GwGccJit *a, Stmt_Flow b) {
  gwgcc_exp(a, b->cond);
  gwgcc_stmt(a, b->body);
}

ANN static bool gwgcc_stmt_until(GwGccJit *a, Stmt_Flow b) {
  gwgcc_exp(a, b->cond);
  gwgcc_stmt(a, b->body);
}

ANN static bool gwgcc_stmt_for(GwGccJit *a, Stmt_For b) {
  gwgcc_stmt(a, b->c1);
  if(b->c2)
    gwgcc_stmt(a, b->c2);
  if(b->c3)
    gwgcc_exp(a, b->c3);
  gwgcc_stmt(a, b->body);
}

ANN static bool gwgcc_stmt_each(GwGccJit *a, Stmt_Each b) {
  gwgcc_symbol(a, b->sym);
  gwgcc_exp(a, b->exp);
  gwgcc_stmt(a, b->body);
}

ANN static bool gwgcc_stmt_loop(GwGccJit *a, Stmt_Loop b) {
  gwgcc_exp(a, b->cond);
  gwgcc_stmt(a, b->body);
}

ANN static bool gwgcc_stmt_if(GwGccJit *a, Stmt_If b) {
  gcc_jit_block* b_cond = gcc_jit_function_new_block(a->func, "if_cond");
  gcc_jit_block* b_if = gcc_jit_function_new_block(a->func, "if_body");
  gcc_jit_block* b_else = b->else_body ? gcc_jit_function_new_block(a->func, "else_body") : NULL;
  gcc_jit_block_end_with_jump(get_block(a), NULL, b_cond);
  gcc_jit_block* b_next = gcc_jit_function_new_block(a->func, "next");
  push_block(a, b_cond);
  gcc_jit_rvalue *cond = gwgcc_exp(a, b->cond);
  pop_block(a);
  gcc_jit_block_end_with_conditional(b_cond, NULL, cond, b_if, b_else ?: b_next);
  push_block(a, b_if);
  gwgcc_stmt(a, b->if_body);
  pop_block(a);
  if(b->else_body) {
    push_block(a, b_else);
    gwgcc_stmt(a, b->else_body);
    pop_block(a);
    push_block(a, b_next);
    gcc_jit_block_end_with_jump(b_else, NULL, b_next);
  }
  push_block(a, b_next);
}

ANN static bool gwgcc_stmt_code(GwGccJit *a, Stmt_Code b) {
  if(b->stmt_list)
    gwgcc_stmt_list(a, b->stmt_list);
}

ANN static bool gwgcc_stmt_varloop(GwGccJit *a, Stmt_VarLoop b) {
  gwgcc_exp(a, b->exp);
  gwgcc_stmt(a, b->body);
}

ANN static bool gwgcc_stmt_break(GwGccJit *a, Stmt_Exp b) {
}

ANN static bool gwgcc_stmt_continue(GwGccJit *a, Stmt_Exp b) {
}

ANN static bool gwgcc_stmt_return(GwGccJit *a, Stmt_Exp b) {
  if(b->val) {
    gcc_jit_rvalue *ret = gwgcc_exp(a, b->val);
    gcc_jit_block* block = (gcc_jit_block*)vector_back(&a->block);
    gcc_jit_block_end_with_return (block, NULL/*gcc_jit_location *loc*/, ret);
  }
}

ANN static bool gwgcc_case_list(GwGccJit *a, Stmt_List b) {
  gwgcc_stmt_case(a, &b->stmt->d.stmt_match);
  if(b->next)
    gwgcc_case_list(a, b->next);
}

ANN static bool gwgcc_stmt_match(GwGccJit *a, Stmt_Match b) {
  gwgcc_exp(a, b->cond);
  gwgcc_case_list(a, b->list);
  if(b->where)
    gwgcc_stmt(a, b->where);
}

ANN static bool gwgcc_stmt_case(GwGccJit *a, Stmt_Match b) {
  gwgcc_exp(a, b->cond);
  gwgcc_stmt_list(a, b->list);
  if(b->when)
    gwgcc_exp(a, b->when);
}

ANN static bool gwgcc_stmt_index(GwGccJit *a, Stmt_Index b) {
}

ANN static bool gwgcc_stmt_pp(GwGccJit *a, Stmt_PP b) {
}

ANN static bool gwgcc_stmt_retry(GwGccJit *a, Stmt_Exp b) {
}

ANN static bool gwgcc_stmt_try(GwGccJit *a, Stmt_Try b) {
  gwgcc_stmt(a, b->stmt);
}

ANN static bool gwgcc_stmt_defer(GwGccJit *a, Stmt_Defer b) {
  gwgcc_stmt(a, b->stmt);
}

DECL_STMT_FUNC(gwgcc, void, GwGccJit*)
ANN static bool gwgcc_stmt(GwGccJit *a, Stmt b) {
  gwgcc_stmt_func[b->stmt_type](a, &b->d);
}

ANN static bool gwgcc_arg_list(GwGccJit *a, Arg_List b) {
  if(b->td)
    gwgcc_type_decl(a, b->td);
  gwgcc_var_decl(a, b->var_decl);
  if(b->next)
    gwgcc_arg_list(a, b->next);
}

ANN static bool gwgcc_union_list(GwGccJit *a, Union_List b) {
  gwgcc_type_decl(a, b->td);
  gwgcc_symbol(a, b->xid);
}

ANN static bool gwgcc_stmt_list(GwGccJit *a, Stmt_List b) {
  gwgcc_stmt(a, b->stmt);
  if(b->next)
    gwgcc_stmt_list(a, b->next);
}

ANN static bool gwgcc_func_base(GwGccJit *a, Func_Base *b) {
  if(b->td)
    gwgcc_type_decl(a, b->td);
//  gwgcc_symbol(a, b->xid);
//  if(b->tmpl)
//    gwgcc_tmpl(a, b->tmpl);
  if(b->args)
    gwgcc_arg_list(a, b->args);
}

ANN static enum gcc_jit_function_kind jit_func_kind(const Value value) {
//  if(GET_FLAG(value, global))
    return GCC_JIT_FUNCTION_EXPORTED;
//  return GCC_JIT_FUNCTION_INTERNAL;
}

ANN static void func_def_args(GwGccJit *a, const Func_Base *base, const Vector v) {
  if(!base->args)
    return;
  vector_init(v);
  Arg_List arg = base->args;
  do {
    gcc_jit_type *arg_type = jit_type(a, arg->type);
    gcc_jit_param *arg0 = gcc_jit_context_new_param (a->ctx, NULL, arg_type, s_name(arg->var_decl->xid));
    add_value(a, arg->var_decl->xid, arg0);
    vector_add(v, (m_uint)arg0);
  } while ((arg = arg->next));
}

ANN static bool gwgcc_func_def(GwGccJit *a, Func_Def b) {
  gcc_jit_function *former = a->func;
  gcc_jit_type *ret_type = jit_type(a, b->base->ret_type);
  struct Vector_ args;
  func_def_args(a, b->base, &args);
  const Value value = b->base->func->value_ref;
  enum gcc_jit_function_kind kind = jit_func_kind(value);

  char c[256];
  cmangle(b->base->func, c);
  gcc_jit_location *loc = gcc_jit_context_new_location(a->ctx, value->from->filename,
     value->from->loc.first.line, value->from->loc.first.column);
  a->func = gcc_jit_context_new_function (a->ctx, loc, kind, ret_type,
    c, EXPAND(&args, gcc_jit_param**), 0);
  if(args.ptr)vector_release(&args);
  const Env env = a->jitter->gwion->env; // we could use the mpool instead of symbols here
  map_set(&a->funcs, (m_uint)insert_symbol(c), (m_uint)a->func);
  gcc_jit_block *block = gcc_jit_function_new_block(a->func, "body");
  vector_add(&a->block, (m_uint)block);
  gwgcc_func_base(a, b->base);
  if(b->d.code) {
    gwgcc_stmt(a, b->d.code);
  }
  a->func = former;
}

ANN static bool gwgcc_class_def(GwGccJit *a, Class_Def b) {
  gwgcc_type_def(a, &b->base);
  if(b->body)
    gwgcc_ast(a, b->body);
}

ANN static bool gwgcc_trait_def(GwGccJit *a, Trait_Def b) {
  if(b->body)
    gwgcc_ast(a, b->body);
}

ANN static bool gwgcc_enum_def(GwGccJit *a, Enum_Def b) {
  gwgcc_id_list(a, b->list);
  if(b->xid)
    gwgcc_symbol(a, b->xid);
}

ANN static bool gwgcc_union_def(GwGccJit *a, Union_Def b) {
  gwgcc_union_list(a, b->l);
  if(b->xid)
    gwgcc_symbol(a, b->xid);
  if(b->tmpl)
    gwgcc_tmpl(a, b->tmpl);
}

ANN static bool gwgcc_fptr_def(GwGccJit *a, Fptr_Def b) {
  gwgcc_func_base(a, b->base);
}

ANN static bool gwgcc_type_def(GwGccJit *a, Type_Def b) {
  if(b->ext)
    gwgcc_type_decl(a, b->ext);
  gwgcc_symbol(a, b->xid);
  if(b->tmpl)
    gwgcc_tmpl(a, b->tmpl);
}

ANN static bool gwgcc_extend_def(GwGccJit *a, Extend_Def b) {
  gwgcc_ast(a, b->body);
  gwgcc_type_decl(a, b->td);
}

DECL_SECTION_FUNC(gwgcc, void, GwGccJit*)
ANN static bool gwgcc_section(GwGccJit *a, Section *b) {
  gwgcc_section_func[b->section_type](a, *(void**)&b->d);
}

ANN static bool gwgcc_main_section(GwGccJit *a, Section *b) {
  if(b->section_type != ae_section_stmt)
    gwgcc_section_func[b->section_type](a, *(void**)&b->d);
}

ANN bool gwgcc_ast(GwGccJit *a, Ast b) {
// Warning: will break classes
  gwgcc_main_section(a, b->section);
  if(b->next)
    gwgcc_ast(a, b->next);
}

ANN2(1,3,5) void jit_add_op(Jitter *jitter, const m_str slhs, const m_str sop, const m_str srhs, void *func) {
  const Env env = jitter->gwion->env;
  const Symbol op = insert_symbol(sop);
  const Type lhs = slhs ? str2type(jitter->gwion, slhs, (loc_t){}) : NULL;
  const Type rhs = srhs ? str2type(jitter->gwion, srhs, (loc_t){}) : NULL;
  struct Op_Import opi = { .lhs = lhs, .rhs = rhs, .op = op };
  void* mo = op_get(jitter->gwion->env, &opi);
  map_set(&jitter->op, (m_uint)mo, (m_uint)func);
}
