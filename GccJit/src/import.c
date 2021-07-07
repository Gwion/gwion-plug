#include <libgccjit.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "gwgcc.h"

extern ANN void import_int(Jitter *jitter);
extern ANN void import_float(Jitter *jitter);

ANN static m_bool gwgcc_pass(const Env env,  Ast b) {
  Jitter *const jitter = (Jitter*)get_module(env->gwion, "GccJit");
  GwGccJit a = { .jitter=jitter, .ctx = gcc_jit_context_acquire() };
  gcc_jit_context_set_int_option (a.ctx, GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL, 3);
  vector_init(&a.block);
  scope_init(env->gwion->mp, &a.value);
  map_init(&a.funcs);
  if(!jitter->init) {
    import_int(jitter);
    import_float(jitter);
    jitter->init = true;
  }
  gwgcc_ast(&a, b);

  gcc_jit_result *  result = gcc_jit_context_compile (a.ctx);
  if (!result) {
    fprintf (stderr, "NULL result");
    return GW_ERROR;
  }
/*
  // dump
  char c[strlen(env->name) + 3];
  sprintf(c, "%s.c", env->name);
  gcc_jit_context_dump_to_file (a.ctx, c, 1);
*/
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
