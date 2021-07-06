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

ANN /*static */gcc_jit_rvalue* jit_op_binary(GwGccJit *a, Exp_Binary *b, enum gcc_jit_binary_op op) {
  gcc_jit_rvalue *lhs = gwgcc_exp(a, b->lhs);
  gcc_jit_rvalue *rhs = gwgcc_exp(a, b->rhs);
  return gcc_jit_context_new_binary_op (a->ctx,
             NULL, //gcc_jit_location *loc,
             op,
             jit_type(a, exp_self(b)->type),
             lhs, rhs);
}

ANN /*static*/ gcc_jit_rvalue* jit_assign(GwGccJit *a, Exp_Binary *b) {
  gcc_jit_rvalue *lhs = gwgcc_exp(a, b->lhs);
  gcc_jit_rvalue *rhs = gwgcc_exp(a, b->rhs);
  /*check me*/gcc_jit_block_add_assignment(get_block(a),
             NULL, //gcc_jit_location *loc,
             (gcc_jit_lvalue*)rhs, lhs);
  return lhs;
}

ANN static gcc_jit_rvalue* jit_op_assign(GwGccJit *a, Exp_Binary *b, enum gcc_jit_binary_op op) {
  gcc_jit_rvalue *lhs = gwgcc_exp(a, b->lhs);
  gcc_jit_rvalue *rhs = gwgcc_exp(a, b->rhs);
  /*check me*/gcc_jit_block_add_assignment_op (get_block(a),
             NULL, //gcc_jit_location *loc,
             (gcc_jit_lvalue*)rhs, op, lhs);
  return lhs;
}

ANN /*static*/ gcc_jit_rvalue* jit_op_compare(GwGccJit *a, Exp_Binary *b, const enum gcc_jit_comparison op) {
  gcc_jit_rvalue *lhs = gwgcc_exp(a, b->lhs);
  gcc_jit_rvalue *rhs = gwgcc_exp(a, b->rhs);
  gcc_jit_block* block = (gcc_jit_block*)vector_back(&a->block);
  return gcc_jit_context_new_comparison (a->ctx, NULL/*loc*/,
        op, lhs, rhs);
}

ANN /*static */gcc_jit_rvalue* jit_op_unary(GwGccJit *a, Exp_Unary *b, const enum gcc_jit_unary_op op) {
  gcc_jit_rvalue *lhs = gwgcc_exp(a, b->exp);
  gcc_jit_block* block = (gcc_jit_block*)vector_back(&a->block);
  return gcc_jit_context_new_unary_op (a->ctx, NULL/*loc*/,
        op, jit_type(a, exp_self(b)->type), lhs);
}

ANN /*static */gcc_jit_rvalue* jit_unaryassign(GwGccJit *a, Exp_Unary *b, enum gcc_jit_binary_op op) {
  gcc_jit_rvalue *lhs = gwgcc_exp(a, b->exp);
  gcc_jit_rvalue *rhs = gcc_jit_context_one(a->ctx, jit_type(a, b->exp->type));
  /*check me*/gcc_jit_block_add_assignment_op (get_block(a),
             NULL, //gcc_jit_location *loc,
             (gcc_jit_lvalue*)lhs, op, rhs);
  return lhs;
}

ANN /*static */gcc_jit_rvalue* jit_postassign(GwGccJit *a, Exp_Postfix *b, enum gcc_jit_binary_op op) {
  gcc_jit_rvalue *lhs = gwgcc_exp(a, b->exp);
  gcc_jit_rvalue *rhs = gcc_jit_context_one(a->ctx, jit_type(a, b->exp->type));
  const pos_t pos = b->exp->pos.first;
  char name[256];
  sprintf(name, "%tmp_%u_%u", pos.line, pos.column);
  gcc_jit_lvalue *tmp = gcc_jit_function_new_local(a->func,
          NULL/*gcc_jit_location *loc*/,
          jit_type(a, b->exp->type), name);
  /*check me*/gcc_jit_block_add_assignment (get_block(a),
             NULL, //gcc_jit_location *loc,
             (gcc_jit_lvalue*)tmp, lhs);
  /*check me*/gcc_jit_block_add_assignment_op (get_block(a),
             NULL, //gcc_jit_location *loc,
             (gcc_jit_lvalue*)lhs, op, rhs);
  return gcc_jit_lvalue_as_rvalue(tmp);
}



