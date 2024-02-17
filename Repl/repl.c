#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "operator.h"
#include "emit.h"
#include "compile.h"
#include "repl.h"
#include "shreduler_private.h"
#include <readline/readline.h>
#include <readline/history.h>

#define PROMPT_SZ 128

static pthread_t repl_thread;
static bool accept, cancel, chctx, fork, add, sys;

static inline int _bind_cr(int count __attribute__((unused)), int key __attribute__((unused))) {
  if(accept) {
    printf("\n");
    return rl_done = true;
  }
  printf("\n(...)");
  return 0;
}

static inline int _bind_accept(int count __attribute__((unused)), int key __attribute__((unused))) {
  accept = true;
  return 0;
}

static inline int _bind_cancel(int count __attribute__((unused)), int key __attribute__((unused))) {
  cancel = true;
  accept = true;
  return 0;
}

static inline int _bind_add(int count __attribute__((unused)), int key __attribute__((unused))) {
  accept = (rl_done = true);
  add = true;
  return 0;
}

static inline int _bind_rem(int count __attribute__((unused)), int key __attribute__((unused))) {
  accept = (rl_done = true);
  add = false;
  return 0;
}

static inline int _bind_ctx(int count __attribute__((unused)), int key __attribute__((unused))) {
  printf("\n");
  chctx = 1;
  return rl_done = 1;
}

static inline int _bind_fork(int count __attribute__((unused)), int key __attribute__((unused))) {
  fork = 1;
  return 0;
}

static inline int _bind_sys(int count __attribute__((unused)), int key __attribute__((unused))) {
  printf("\n");
  sys = 1;
  return accept = (rl_done = 1);
}

static inline VM_Shred repl_shred(MemPool p) {
  const VM_Code code = new_vmcode(p, NULL, NULL, "repl", 0, false, false);
  const VM_Shred shred = new_vm_shred(p, code);
  return shred;
}

ANN static bool eval(const VM* vm, const VM_Shred shred, const m_str line) {
  if(vm->shreduler->list && shred == vm->shreduler->list->self) {
    gw_err("shred[%"UINT_F"] is running.please use '\\C-f' to spork it\n", shred->tick->xid);
    return false;
  }
  return compile_string(vm->gwion, "repl", line);
}

struct Repl {
  Context ctx;
  VM_Shred shred;
  bool is_loaded;
};

ANN static struct Repl* new_repl(MemPool p, const m_str name) {
  struct Repl* repl = (struct Repl*)xmalloc(sizeof(struct Repl));
  repl->shred = repl_shred(p);
  repl->ctx = new_context(p, NULL, name);
  return repl;
}

ANN static void free_repl(struct Repl* repl, const VM* vm) {
/*
  if(repl->shred->code->instr) {
    ((Instr)vector_back(repl->shred->code->instr))->execute = EOC;
    repl->shred->pc = vector_size(repl->shred->code->instr) - 2;
    vm_add_shred(vm, repl->shred);
  } else if(repl->shred->tick)
    shreduler_remove(vm->shreduler, repl->shred, true);
*/
  context_remref(repl->ctx, vm->gwion);
  free(repl);
}

ANN static struct Repl* repl_ctx(struct Repl* repl, const Vector v, const VM* vm) {
  struct Repl* r = NULL;
  accept = 1;
  const m_str ln = readline("\033[1mcontext:\033[0m ");
  for(m_uint i = vector_size(v) + 1; --i;) {
    struct Repl* s = (struct Repl*)vector_at(v, i-1);
    if(!strcmp(ln, s->ctx->name)) {
       r = s;
       break;
    }
  }
  if(!r) {
    printf("creating new context \033[1m'%s'\033[0m\n", ln);
    r = new_repl(vm->gwion->mp, ln);
    vector_add(v, (vtype)r);
  }
  if(repl != r) {
    unload_context(repl->ctx, vm->gwion->env);
    repl->is_loaded = 0;
    repl = r;
    load_context(repl->ctx, vm->gwion->env);
    repl->is_loaded = 1;
  }
  chctx = 0;
  return repl;
}

ANN static void repl_fork(MemPool p, struct Repl* repl) {
  printf("fork shred [%"UINT_F"]\n", repl->shred->tick->xid);
  const VM_Shred old = repl->shred;
  repl->shred = repl_shred(p);
  old->tick->parent = repl->shred->tick;
  if(!repl->shred->tick->child.ptr)
    vector_init(&repl->shred->tick->child);
  vector_add(&repl->shred->tick->child, (vtype)old);
  memcpy((m_bit*)repl->shred + sizeof(struct VM_Shred_) + SIZEOF_REG, (m_bit*)old + sizeof(struct VM_Shred_)+ SIZEOF_REG, SIZEOF_MEM);
  memcpy((m_bit*)repl->shred + sizeof(struct VM_Shred_), (m_bit*)old + sizeof(struct VM_Shred_), SIZEOF_REG);
  fork = 0;
}

static void repl_sys(void) {
  const pid_t cpid = fork;
  int status;
  const m_str cmd = readline("command: ");
  system(cmd);
  xfree(cmd);
  waitpid(cpid, &status, 0);
  sys = 0;
}

ANN static void repl_add(VM* vm) {
  const m_str line = readline(add ? "add file: " : "rem file:");
  if(add) {
    // should we strip in the core?
    line[strlen(line)-1] = '\0';
    compile_string(vm->gwion, "repl", line);
    compile_filename(vm->gwion, line);
  } else {
    const m_uint index = strtol(line, NULL, 10);
    vm_remove(vm, index);
  }
  free(line);
  add = 0;
}

ANN static m_str repl_prompt(const struct Repl* repl) {
  char prompt[PROMPT_SZ];
  if(repl->shred->tick)
    snprintf(prompt, PROMPT_SZ, "'%s'\033[30;3;1m[%"UINT_F"]\033[32m=>\033[0m ", repl->ctx->name, repl->shred->tick->xid);
  else
    snprintf(prompt, PROMPT_SZ, "'%s'\033[30;3;1m[!]\033[32m=>\033[0m ", repl->ctx->name);
  accept = 0;
  char* ret = readline(prompt);
  if(cancel) {
    cancel = 0;
    xfree(ret);
    return (m_str)1;
  }
  return ret;
}

ANN static void repl_cmd(struct Repl* repl, VM* vm, const Vector v) {
    if(chctx)
      repl = repl_ctx(repl, v, vm);
    if(sys)
      repl_sys();
    if(add)
      repl_add(vm);
    if(fork)
      repl_fork(vm->gwion->mp, repl);
}

ANN static struct Repl* repl_ini(const VM* vm, const Vector v) {
  struct Repl* repl = new_repl(vm->gwion->mp, "repl");
  vector_init(v);
  vector_add(v, (vtype)repl);
  load_context(repl->ctx, vm->gwion->env);
  repl->is_loaded = 1;
  read_history(NULL);
  rl_bind_key('\n', _bind_cr);\
  rl_bind_key('\r', _bind_cr);\
  rl_bind_keyseq("\\M-g", _bind_accept);\
  rl_bind_keyseq("\\M-f", _bind_fork);\
  rl_bind_keyseq("\\M-c", _bind_ctx);\
  rl_bind_keyseq("\\M-z", _bind_cancel);\
  rl_bind_keyseq("\\M-a", _bind_add);\
  rl_bind_keyseq("\\M-r", _bind_rem);\
  rl_bind_keyseq("\\M-s", _bind_sys);\
  return repl;
}

ANN static void repl_end(struct Repl* repl, VM* vm, const Vector v) {
  if(!repl->is_loaded)
    unload_context(repl->ctx, vm->gwion->env);
  for(m_uint i = vector_size(v) + 1; --i;)
    free_repl((struct Repl*)vector_at(v, i-1), vm);
  vector_release(v);
  write_history(NULL);
  vm->bbq->is_running = 0;
}

static void please_wait(void) {
 enum {SECS_TO_SLEEP = 0, NSEC_TO_SLEEP = 50000000};
 struct timespec remaining, request = {SECS_TO_SLEEP, NSEC_TO_SLEEP};
  nanosleep(&request, &remaining);
}

ANN static void* repl_process(void* data) {
  please_wait();
  VM* vm = (VM*)data;
  struct Vector_ v;
  struct Repl* repl = repl_ini(vm, &v);
  while(vm->bbq->is_running) {
    char* line = repl_prompt(repl);
    if(!line)
      break;
    if(line == (m_str) 1)
      continue;
    if(strlen(line)) {
      eval(vm, repl->shred, line);
      add_history(line);
    }
    repl_cmd(repl, vm, &v);
    xfree(line);
  }
  repl_end(repl, vm, &v);
  return NULL;
}

GWMODINI(Repl) {
  shreduler_set_loop(gwion->vm->shreduler, 1);
  pthread_create(&repl_thread, NULL, repl_process, gwion->vm);
  return (void*)true;
}

GWMODEND(Repl) {
  pthread_join(repl_thread, NULL);
}

