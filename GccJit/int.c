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

jit_binary(add, OP_PLUS);
jit_binary(sub, OP_MINUS);
jit_binary(mul, OP_MULT);
jit_binary(div, OP_DIVIDE);
jit_binary(mod, OP_MODULO);
jit_binary(band, OP_BITWISE_AND);
jit_binary(bxor, OP_BITWISE_XOR);
jit_binary(bor, OP_BITWISE_OR);
jit_binary(and, OP_LOGICAL_AND);
jit_binary(or, OP_LOGICAL_OR);
jit_binary(ls, OP_LSHIFT);
jit_binary(rs, OP_RSHIFT);

jit_assignop(add, OP_PLUS);
jit_assignop(sub, OP_MINUS);
jit_assignop(mul, OP_MULT);
jit_assignop(div, OP_DIVIDE);
jit_assignop(mod, OP_MODULO);
jit_assignop(band, OP_BITWISE_AND);
jit_assignop(bxor, OP_BITWISE_XOR);
jit_assignop(bor, OP_BITWISE_OR);
jit_assignop(and, OP_LOGICAL_AND);
jit_assignop(or, OP_LOGICAL_OR);
jit_assignop(ls, OP_LSHIFT);
jit_assignop(rs, OP_RSHIFT);

jit_comparison(eq, EQ);
jit_comparison(ne, NE);
jit_comparison(lt, LT);
jit_comparison(le, LE);
jit_comparison(gt, GT);
jit_comparison(ge, GE);

jit_unary(sub, OP_MINUS);
jit_unary(cmp, OP_BITWISE_NEGATE);
jit_unary(neg, OP_LOGICAL_NEGATE);
//jit_unary(abs, OP_BITWISE_ABS);

jit_assignunary(addadd, OP_PLUS);
jit_assignunary(subsub, OP_MINUS);

jit_post(addadd, OP_PLUS);
jit_post(subsub, OP_MINUS);

ANN void import_int(Jitter *jitter) {
  jit_add_op(jitter, "int", "+", "int", jit_op_add);
  jit_add_op(jitter, "int", "-", "int", jit_op_sub);
  jit_add_op(jitter, "int", "*", "int", jit_op_mul);
  jit_add_op(jitter, "int", "/", "int", jit_op_div);
  jit_add_op(jitter, "int", "%", "int", jit_op_mod);

  jit_add_op(jitter, "int", "&", "int", jit_op_band);
  jit_add_op(jitter, "int", "^", "int", jit_op_bxor);
  jit_add_op(jitter, "int", "|", "int", jit_op_bor);

  jit_add_op(jitter, "int", "&&", "int", jit_op_and);
  jit_add_op(jitter, "int", "||", "int", jit_op_or);

  jit_add_op(jitter, "int", "<<", "int", jit_op_ls);
  jit_add_op(jitter, "int", ">>", "int", jit_op_rs);


  jit_add_op(jitter, "int", "==", "int", jit_op_eq);
  jit_add_op(jitter, "int", "!=", "int", jit_op_ne);
  jit_add_op(jitter, "int", "<",  "int", jit_op_lt);
  jit_add_op(jitter, "int", ">=", "int", jit_op_le);
  jit_add_op(jitter, "int", ">",  "int", jit_op_gt);
  jit_add_op(jitter, "int", ">=", "int", jit_op_ge);

  jit_add_op(jitter, "int", "=>", "int", jit_assign);
  jit_add_op(jitter, "int", "+=>", "int", jit_assign_add);
  jit_add_op(jitter, "int", "-=>", "int", jit_assign_sub);
  jit_add_op(jitter, "int", "*=>", "int", jit_assign_mul);
  jit_add_op(jitter, "int", "/=>", "int", jit_assign_div);
  jit_add_op(jitter, "int", "%=>", "int", jit_assign_mod);

  jit_add_op(jitter, "int", "&=>", "int", jit_assign_band);
  jit_add_op(jitter, "int", "^=>", "int", jit_assign_bxor);
  jit_add_op(jitter, "int", "|=>", "int", jit_assign_bor);

  jit_add_op(jitter, "int", "&&=>", "int", jit_assign_and);
  jit_add_op(jitter, "int", "||=>", "int", jit_assign_or);

  jit_add_op(jitter, "int", "<<=>", "int", jit_assign_ls);
  jit_add_op(jitter, "int", ">>=>", "int", jit_assign_rs);

  jit_add_op(jitter, "int", "-",  NULL, jit_unary_sub);
  jit_add_op(jitter, "int", "~",  NULL, jit_unary_cmp);
  jit_add_op(jitter, "int", "!",  NULL, jit_unary_neg);
  jit_add_op(jitter, "int", "++", NULL, jit_unary_addadd);
  jit_add_op(jitter, "int", "--", NULL, jit_unary_subsub);
  jit_add_op(jitter, NULL,  "++", "int", jit_post_addadd);
  jit_add_op(jitter, NULL,  "--", "int", jit_post_subsub);
}

ANN static m_bool gwgcc_pass(const Env env,  Ast b) {
  Jitter *const jitter = (Jitter*)get_module(env->gwion, "GccJit");
  GwGccJit a = { .jitter=jitter, .ctx = gcc_jit_context_acquire() };
  gcc_jit_context_set_int_option (a.ctx, GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL, 3);
  vector_init(&a.block);
  scope_init(env->gwion->mp, &a.value);
  map_init(&a.funcs);
  if(!jitter->init) {
    import_int(jitter);
    jitter->init = true;
  }
  gwgcc_ast(&a, b);

  gcc_jit_result *  result = gcc_jit_context_compile (a.ctx);
  if (!result) {
    fprintf (stderr, "NULL result");
    return GW_ERROR;
  }

  // dump
  char c[strlen(env->name) + 3];
  sprintf(c, "%s.c", env->name);
  gcc_jit_context_dump_to_file (a.ctx, c, 1);

  // release context
  gcc_jit_context_release (a.ctx);

  typedef long (*loop_test_fn_type) (long);
  loop_test_fn_type loop_test = (loop_test_fn_type)gcc_jit_result_get_code(result, "fib0");
  printf ("result: %li", loop_test (40));

  gcc_jit_result_release(result);

  vector_release(&a.block);
  scope_release(&a.value);
  map_release(&a.funcs);
  return GW_OK;
}

ANN static inline void add_pass(const Vector v, const m_str name) {
  vector_add(v, (m_uint)name);
}

GWMODINI(gwgcc) {
  Jitter *const jitter = (Jitter*)mp_malloc(gwion->mp, Jitter);
  jitter->gwion = gwion;
  map_init(&jitter->funcs);
  map_init(&jitter->value);
  map_init(&jitter->op);
  pass_register(gwion, "gwgcc", gwgcc_pass);
  struct Vector_ v;
  vector_init(&v);
  add_pass(&v, "check");
//  add_pass(&v, "emit");
  add_pass(&v, "gwgcc");
  pass_set(gwion, &v);
  vector_release(&v);
  return jitter;
}

GWMODEND(gwgcc) {
  Jitter *const jitter = (Jitter*)self;
  map_release(&jitter->funcs);
  map_release(&jitter->value);
  map_release(&jitter->op);
}
