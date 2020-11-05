#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "gwion.h"
#include "jitbackend.h"

struct JitEngine_ {
  struct JitBackend_ *jb;
  VM_Code (*emit_code)(const Emitter);// base emit func
  struct Vector_ code_list;
  THREAD_TYPE thread;
  MUTEX_TYPE mutex;
  THREAD_COND_TYPE cond;
  uint cancel;
};

ANN static void add2queue(struct JitEngine_* jit, const VM_Code code) {
{
  const Instr last = (Instr)vector_back(code->instr);
  if(last->opcode == eEOC)
    return;


}
  MUTEX_LOCK(jit->mutex);
  if(jit->cancel)
    return;
  vector_add(&jit->code_list, (vtype)code);
  ADD_REF(code);
  THREAD_COND_SIGNAL(jit->cond);
  MUTEX_UNLOCK(jit->mutex);
}

static VM_Code jit_emit(const Emitter emit) {
  struct JitEngine_ *jit = get_module(emit->gwion, "jit");
  const VM_Code code = jit->emit_code(emit);
  add2queue(jit, code);
  return code;
}

ANN static void* process_queue(void *data) {
  struct JitEngine_ *jit = (struct JitEngine_ *)data;
  while(1) {
    MUTEX_LOCK(jit->mutex);
    while (!vector_size(&jit->code_list) && !jit->cancel)
      THREAD_COND_WAIT(jit->cond, jit->mutex);
    if(!jit->cancel) {
      const VM_Code code = (VM_Code)vector_front(&jit->code_list);
      vector_rem(&jit->code_list, 0);
      jb_run(jit->jb, code);
    } else {
      THREAD_COND_SIGNAL(jit->cond);
      MUTEX_UNLOCK(jit->mutex);
//THREAD_RETURN(NULL);
      return NULL;
    }
    MUTEX_UNLOCK(jit->mutex);
  }
THREAD_RETURN(NULL);
//  return NULL;
}

GWMODINI(jit) {
  struct JitEngine_ *jit = mp_calloc(gwion->mp, JitEngine);
  jit->jb = jb_ini(gwion);
  vector_init(&jit->code_list);
  MUTEX_SETUP(jit->mutex)
  THREAD_COND_SETUP(jit->cond)
  jit->emit_code = gwion->emit->info->emit_code;
  gwion->emit->info->emit_code = jit_emit;
  THREAD_CREATE(jit->thread, process_queue, jit);
  return jit;
}

GWMODEND(jit) {
  struct JitEngine_ *jit = self;
  MUTEX_LOCK(jit->mutex);
  jit->cancel = 1;
  THREAD_COND_SIGNAL(jit->cond);
  MUTEX_UNLOCK(jit->mutex);
  MUTEX_LOCK(jit->mutex);
  THREAD_COND_WAIT(jit->cond, jit->mutex);
  MUTEX_UNLOCK(jit->mutex);
  vector_release(&jit->code_list);
  THREAD_JOIN(jit->thread);
  MUTEX_CLEANUP(jit->mutex)
  THREAD_COND_CLEANUP(jit->cond);
  jb_end(gwion, jit->jb);
  mp_free(gwion->mp, JitEngine, jit);
}
