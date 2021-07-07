typedef struct Jitter_ {
  Gwion gwion;
  struct Map_ funcs;
  struct Map_ value;
  struct Map_ op;
  bool init;
} Jitter;

typedef struct {
  Jitter *const jitter;
  gcc_jit_context *ctx;
  gcc_jit_function *func;
  struct Vector_ block;
  struct Scope_ value;
  struct Map_ funcs;
} GwGccJit;

ANN gcc_jit_rvalue* gwgcc_exp(GwGccJit *a, Exp b);
ANN bool gwgcc_ast(GwGccJit *a, Ast b);


ANN gcc_jit_rvalue* jit_op_binary(GwGccJit *a, Exp_Binary *b, enum gcc_jit_binary_op op);
#define jit_binary(name, op)                                            \
ANN static gcc_jit_rvalue* jit_op_##name(GwGccJit *a, Exp_Binary *b) { \
  return jit_op_binary(a, b, GCC_JIT_BINARY_##op);                      \
}

ANN gcc_jit_rvalue* jit_assign(GwGccJit *a, Exp_Binary *b);
#define jit_assignop(name, op)                                              \
ANN static gcc_jit_rvalue* jit_assign_##name(GwGccJit *a, Exp_Binary *b) { \
  return jit_op_binary(a, b, GCC_JIT_BINARY_##op);                          \
}

ANN gcc_jit_rvalue* jit_op_compare(GwGccJit *a, Exp_Binary *b, const enum gcc_jit_comparison op);
#define jit_comparison(name, op)                                        \
ANN static gcc_jit_rvalue* jit_op_##name(GwGccJit *a, Exp_Binary *b) { \
  return jit_op_compare(a, b, GCC_JIT_COMPARISON_##op);                 \
}

ANN gcc_jit_rvalue* jit_op_unary(GwGccJit *a, Exp_Unary *b, const enum gcc_jit_unary_op op);
#define jit_unary(name, op)                                               \
ANN static gcc_jit_rvalue* jit_unary_##name(GwGccJit *a, Exp_Unary *b) { \
  return jit_op_unary(a, b, GCC_JIT_UNARY_##op);                          \
}

ANN gcc_jit_rvalue* jit_unaryassign(GwGccJit *a, Exp_Unary *b, enum gcc_jit_binary_op op);
#define jit_assignunary(name, op)                                         \
ANN static gcc_jit_rvalue* jit_unary_##name(GwGccJit *a, Exp_Unary *b) { \
  return jit_unaryassign(a, b, GCC_JIT_BINARY_##op);                      \
}

ANN gcc_jit_rvalue* jit_postassign(GwGccJit *a, Exp_Postfix *b, enum gcc_jit_binary_op op);
#define jit_post(name, op)                                                 \
ANN static gcc_jit_rvalue* jit_post_##name(GwGccJit *a, Exp_Postfix *b) { \
  return jit_postassign(a, b, GCC_JIT_BINARY_##op);                        \
}

ANN2(1,3,5) void jit_add_op(Jitter *jitter, const m_str slhs, const m_str sop, const m_str srhs, void *func);

ANN static inline gcc_jit_block *get_block(GwGccJit *a) {
  return (gcc_jit_block *)vector_back(&a->block);
}

//! get a gcc_jit_type from a gwion type
ANN gcc_jit_type* jit_type(GwGccJit *a, const Type t);
