#include <jit/jit.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "jitbackend.h"

struct JitBackend_ {
  jit_context_t context;
  Gwion gwion;
  m_uint pc;
};

struct JitBackend_* jb_ini(const Gwion gwion) {
  struct JitBackend_ *jb = (struct JitBackend_*)mp_calloc(gwion->mp, JitBackend);
  jb->context = jit_context_create();
  jb->gwion = gwion;
  return jb;
}

ANN static inline jit_function_t compilation_ini(struct JitBackend_ *jb) {
  jit_context_build_start(jb->context);
   jit_type_t params[2] = { jit_type_void_ptr, jit_type_void_ptr };
   jit_type_t signature = jit_type_create_signature
         (jit_abi_cdecl, jit_type_void, params, 2, 1);
  return jit_function_create(jb->context, signature);
}

ANN static inline void compilation_end(struct JitBackend_ *jb, jit_function_t fn) {
  jit_function_compile(fn);
  jit_context_build_end(jb->context);
}




ANN m_uint jb_block(struct JitBackend_ *jb, const VM_Code code) {
  m_uint i;
  for(i = jb->pc; i < vector_size(code->instr); ++i) {
    const Instr instr = (Instr)vector_at(code->instr, i);
//    printf("%lu %lu %lu %lu\n", instr->opcode, instr->m_val, instr->m_val2, instr->execute);
    
  }
  return i;
}

ANN m_uint jb_compile(struct JitBackend_ *jb, const VM_Code code) {
  jit_function_t fn = compilation_ini(jb);
  jb->pc = jb_block(jb, code);
  compilation_end(jb, fn);
// set code
  return jb->pc;
}

ANN void jb_run(struct JitBackend_ *jb, const VM_Code code) {
puts("here");
  jb->pc = 0;
  while(jb_compile(jb, code) < vector_size(code->instr));
}

ANN void jb_end(const Gwion gwion, struct JitBackend_ *jb) {
  jit_context_destroy(jb->context);
  mp_free(gwion->mp, JitBackend, jb);
}

static INSTR(libjit_instr) {
  VM_Shred s = shred;
  Instr i = instr;
  void* args[2] = { &s, &i };
  jit_function_t jit_fn = (jit_function_t)instr->m_val;
  jit_function_apply(jit_fn, args, NULL);
}
