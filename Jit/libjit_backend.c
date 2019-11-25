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
  m_uint start, pc;
  m_bit *ctrl0;
  m_bit *ctrl;
  jit_label_t *label;
  jit_type_t signature;
};

struct JitBackend_* jb_ini(const Gwion gwion) {
  struct JitBackend_ *jb = (struct JitBackend_*)mp_calloc(gwion->mp, JitBackend);
  jb->context = jit_context_create();
  jb->gwion = gwion;
  return jb;
}

ANN static inline jit_function_t compilation_ini(struct JitBackend_ *jb) {
//puts(__func__);
  jit_context_build_start(jb->context);
  if(!jb->signature) {
   jit_type_t params[2] = { jit_type_void_ptr, jit_type_void_ptr };
   jb->signature = jit_type_create_signature
         (jit_abi_cdecl, jit_type_void, params, 2, 1);
  }
  return jit_function_create(jb->context, jb->signature);
}

ANN static inline void compilation_end(struct JitBackend_ *jb, jit_function_t fn) {
//puts(__func__);
  jit_insn_default_return(fn);
  jit_function_clear_recompilable(fn);
  jit_function_compile(fn);
  jit_function_clear_recompilable(fn);
  jit_function_compile(fn);
  jit_context_build_end(jb->context);
//  MUTEX_LOCK(jb->gwion->data->mutex);
//printf("in lock at %lu %lu\n", jb->start, jb->pc);
//  MUTEX_UNLOCK(jb->gwion->data->mutex);
}

#define JLOADR(a,b,c) jit_insn_load_relative(fn, (a), (b), jit_type_##c)
#define JOFF(a,b) __builtin_offsetof(struct a##_, b)
#define JCONST(a, b) jit_value_create_nint_constant(fn, jit_type_##a, (jit_nint)(b))
#define JSTORER(a,b,c) jit_insn_store_relative(fn, (a), (b), (c))
#define JADDR(a,b) jit_insn_add_relative(fn, (a), (b))

#ifdef USE_DOUBLE
#define jit_type_float jit_type_sys_double
#else
#define jit_type_float jit_type_sys_float
#endif

ANN m_bool same_block(struct JitBackend_ *jb, const m_uint target, m_uint i) {
  for(i; i < target; ++i) {
    if(jb->ctrl0[i])
      return 0;
  }
  return 1;
}

ANN void jb_ctrl(struct JitBackend_ *jb, const VM_Code code) {
  for(m_uint i = 0; i < vector_size(code->instr); ++i) {
    const Instr instr = (Instr)vector_at(code->instr, i);
//printf("OPCODE %lu\n", instr->opcode);
    switch(instr->opcode) {
      case eGoto:
if(!same_block(jb, instr->m_val, i))
        jb->ctrl[i] = 1;
//        jb->ctrl[instr->m_val] = 1;
//}
//else
//jb->label[instr->m_val] = jit_label_undefined;
//jb->label[instr->m_val] = 1;
 //       break;
      case eBranchEqInt:
if(!same_block(jb, instr->m_val, i))
        jb->ctrl[instr->m_val-1] = 1;
else
jb->label[instr->m_val] = 1;
        break;
      case eOverflow:
        jb->ctrl[i] = 1;
        break;
      default:;
    }
  }
}

ANN void jb_ctrl0(struct JitBackend_ *jb, const VM_Code code) {
  for(m_uint i = 0; i < vector_size(code->instr); ++i) {
    const Instr instr = (Instr)vector_at(code->instr, i);
    switch(instr->opcode) {
      case eOverflow:
        jb->ctrl0[i] = 1;
        break;
      default:;
    }
  }
}

static INSTR(jit_instr);
static inline m_bool vm_continue(const VM_Shred shred) {
  Instr instr = *(Instr*)(shred->code->bytecode + ((shred->pc-1) *SZ_INT*3)+ SZ_INT);
//   const Instr instr = (Instr)vector_at(shred->code->instr, shred->pc - 1);
   assert(instr->execute == jit_instr);
   return instr->execute == jit_instr;
}

static void jit_vm(const VM_Shred shred) {
  VM_Shred s = shred;
  void* args[1] = { &s };
  do  {
    const m_bit *bytecode = shred->code->bytecode + (shred->pc -1)*SZ_INT*3;
    Instr instr = *(Instr*)(bytecode + SZ_INT);
    jit_function_t fn = (jit_function_t)instr->m_val;
//    jit_function_t fn = (jit_function_t)instr->m_val;
f_instr _fn = (f_instr)instr->m_val2;
_fn(shred, instr);
// assert
//puts("here 2");
//    jit_function_apply(fn, args, NULL);
//puts("here 3");
//}while(1);
//  }while(0);
  } while(vm_continue(shred));
}

static INSTR(jit_instr) {
   const Instr _instr = (Instr)vector_at(shred->code->instr, shred->pc - 1);
  VM_Shred s = shred;
  void* args[2] = { &s };
//  jit_function_t fn = (jit_function_t)instr->m_val;
  f_instr fn = (f_instr)instr->m_val;
//--shred->pc;
//  jit_function_apply(fn, args, NULL);
}

ANN static void mk_instr(jit_function_t fn, const m_bit *data, const Instr instr) {
//  instr->opcode = eOP_MAX;
//  instr->m_val = (m_uint)fn;
//  instr->m_val2 = (m_uint)jit_function_to_closure;
//  instr->execute = jit_instr;
  *(m_uint*)data = eOP_MAX;
//  *(jit_function_t*)(data + SZ_INT) = //fn; // ws instr
//  *(f_instr*)(data + SZ_INT*2) = jit_instr;
  *(f_instr*)(data + SZ_INT*2) = jit_function_to_closure(fn);
}

ANN m_uint ctrl_next(const struct JitBackend_ *jb, const m_uint len) {
  for(m_uint i = jb->start; i < len; ++i) {
    if(jb->ctrl[i])
      return i;
  }
  return len;
}

ANN m_uint jb_compile(struct JitBackend_ *jb, const VM_Code code) {
//puts(__func__);


  jb->start = jb->pc;
const m_uint next = ctrl_next(jb, vector_size(code->instr));
printf("next %lu\n", next);
if((next - jb->start) < 3) {
  puts("skip");
  return next;
}

  jit_function_t fn = compilation_ini(jb);
//  m_uint i;
    jit_value_t shred = jit_value_get_param(fn, 0);
  jit_value_t reg  = JLOADR(shred, JOFF(VM_Shred, reg),  void_ptr);
  jit_value_t mem  = JLOADR(shred, JOFF(VM_Shred, mem),  void_ptr);

  for(jb->start = jb->pc; jb->pc < vector_size(code->instr)
; ++jb->pc) {
    const Instr instr = (Instr)vector_at(code->instr, jb->pc);


//if(jb->label[jb->pc] == jit_label_undefined)
if(jb->label[jb->pc] == 1) {
  jb->label[jb->pc] = jit_label_undefined;
  jit_insn_label(fn, &jb->label[jb->pc]);                  \
}

    jit_value_t base = JLOADR(shred, JOFF(VM_Shred, base), void_ptr);

//jit_value_t pc = NULL;

static m_uint next = 0;
jit_value_t next_code;
#define regset(type, val, off) \
 { jit_value_t v = JCONST(type, val); JSTORER(reg, off, v); }
#define memset(type, val, off) \
 { jit_value_t v = JCONST(type, val); JSTORER(mem, off, v); }
#define regmem(type, val, off) \
 { jit_value_t v = JLOADR(mem, val, type); JSTORER(reg, off, v); }
#define regbase(type, val, off) \
 { jit_value_t v = JLOADR(base, val, type); JSTORER(reg, off, v); }

switch(instr->opcode) {
  
case eRegSetImm:
  regset(nuint, instr->m_val, instr->m_val2);
  break;
#define regpush(name, type, val, size) \
case eRegPush##name:                   \
    regset(type, val, 0);              \
    reg = JADDR(reg, size);            \
    break;\

regpush(Imm, nuint, instr->m_val, SZ_INT)
regpush(Imm2, float, instr->f, SZ_FLOAT)
case eRegPushImm3:
    for(m_uint i = 0; i <= instr->m_val2; i+= SZ_INT)
      regset(nuint, instr->m_val + i, i);
    reg = JADDR(reg, instr->m_val2);
    break;
regpush(Imm4, void_ptr, &instr->m_val, SZ_INT)

case eRegPushMem:
    regmem(nuint, instr->m_val, 0);
    reg = JADDR(reg, SZ_INT);
    break;
case eRegPushMem2:
    regmem(float, instr->m_val, 0);
    reg = JADDR(reg, SZ_FLOAT);
    break;
case eRegPushMem3:
    for(m_uint i = 0; i <= instr->m_val2; i+= SZ_INT)
      regmem(float, instr->m_val + i, i);
    reg = JADDR(reg, SZ_INT);
    break;
case eRegPushMem4:
 { jit_value_t v = JADDR(mem, instr->m_val); JSTORER(reg, 0, v); }
    reg = JADDR(reg, instr->m_val2);
    break;

case eRegPushBase:
    regbase(nuint, instr->m_val, 0);
    reg = JADDR(reg, SZ_INT);
    break;
case eRegPushBase2:
    regbase(float, instr->m_val, 0);
    reg = JADDR(reg, SZ_FLOAT);
    break;
case eRegPushBase3:
    for(m_uint i = 0; i <= instr->m_val2; i+= SZ_INT)
      regbase(float, instr->m_val + i, i);
    reg = JADDR(reg, SZ_INT);
    break;
case eRegPushBase4:
 { jit_value_t v = JADDR(base, instr->m_val); JSTORER(reg, 0, v); }
    reg = JADDR(reg, instr->m_val2);
    break;
// ... regtoreg ...
case eMemSetImm:
//  memset(nuint, instr->m_val, instr->m_val2);
  memset(nuint, instr->m_val2, instr->m_val);
//  break;

//

case eFuncReturn:
{
  jit_value_t pc   = JLOADR(mem, -SZ_INT*2, nuint);
  jit_value_t code = JLOADR(mem, -SZ_INT*3, void_ptr);
  jit_value_t off0 = JLOADR(mem, -SZ_INT*4, nuint);
  jit_value_t off1 = JCONST(nuint, SZ_INT*4);
  jit_value_t off  = jit_insn_add(fn, off0, off1);
  mem = jit_insn_sub(fn, mem, off);
  JSTORER(shred, JOFF(VM_Shred, pc), pc);
  JSTORER(shred, JOFF(VM_Shred, code), code);
}
 break;
//  compilation_end(jb, fn);
//mk_instr(fn, code->bytecode + (jb->start * SZ_INT*3),
//(Instr)vector_at(code->instr, jb->start));
//  return jb->pc = vector_size(code->instr);

case eGoto:
{
//  printf("GOTO instr->m_val %lu\n", instr->m_val);
  jit_value_t pc = JCONST(nuint, instr->m_val);
//  JSTORER(shred, JOFF(VM_Shred, code), JCONST(void_ptr, code));
  JSTORER(shred, JOFF(VM_Shred, pc), pc);
if(same_block(jb, instr->m_val, jb->pc))
//  jit_insn_branch(fn, &jb->label[instr->m_val]);
  jb->pc = instr->m_val -1;

break;
}


#define OP(t, sz, op, ...)                       \
{                                                \
  reg = JADDR(reg, -sz);                         \
  __VA_ARGS__                                    \
 jit_value_t lhs = JLOADR(reg, -sz, t);         \
 jit_value_t rhs = JLOADR(reg, 0, t);           \
 jit_value_t ret = jit_insn_##op(fn, lhs, rhs);\
  JSTORER(reg, -sz, ret);                        \
  break;                                         \
}

#define INT_OP(op, ...) OP(nint, SZ_INT, op, __VA_ARGS__)
case eint_plus:
  INT_OP(add)
case eint_minus:
  INT_OP(sub)

#define LOGICAL(t, sz0, sz, op)                  \
{                                                \
  reg = JADDR(reg, -sz0);                        \
  jit_value_t lhs = JLOADR(reg, -SZ_INT, t);     \
  jit_value_t rhs = JLOADR(reg, sz, t);          \
  jit_value_t ret = jit_insn_##op(fn, lhs, rhs); \
  JSTORER(reg, -SZ_INT, ret);                    \
  break;                                         \
}

#define INT_LOGICAL(op) LOGICAL(nint, SZ_INT, 0, op)

case eint_lt:
  INT_LOGICAL(lt)

case eint_gt:
  INT_LOGICAL(gt)



case eFuncUsr:
{
  reg = JADDR(reg, -SZ_INT);
  next_code = JLOADR(reg, 0, void_ptr);
  jit_value_t push_reg = JLOADR(reg, SZ_INT, nuint);
  jit_value_t push_mem = JLOADR(mem, -SZ_INT, nuint);
  jit_value_t push = jit_insn_add(fn, push_reg, push_mem);
  mem = jit_insn_add(fn, mem, push);
  JSTORER(mem, 0,        push);
  jit_value_t curr_code = JLOADR(shred, JOFF(VM_Shred, code), void_ptr);
  JSTORER(mem, SZ_INT,   curr_code);
  jit_value_t next_pc = JCONST(nuint, jb->pc + instr->m_val2 + 1);
  JSTORER(mem, SZ_INT*2, next_pc);
  jit_value_t stack_depth = JLOADR(next_code, JOFF(VM_Code, stack_depth), nuint);
  JSTORER(mem, SZ_INT*3, stack_depth);
  mem = JADDR(mem, SZ_INT*4);
  reg = JADDR(reg, -instr->m_val);
// TODO
next = eFuncUsrEnd;
//  JSTORER(next
  break;
}


case eReg2Mem:
{
//puts("regtomem");
  jit_value_t val = JLOADR(reg, instr->m_val2, nuint);
  JSTORER(mem, instr->m_val, val);
  break;
}
case eOverflow:
{
// TODO: actual overflow check
//if(next == eFuncUsrEnd)

// check nextcode

  JSTORER(shred, JOFF(VM_Shred, code), next_code);
  jit_value_t pc = JCONST(nuint, 0);
  JSTORER(shred, JOFF(VM_Shred, pc), pc);
  JSTORER(shred, JOFF(VM_Shred, reg), reg);
  JSTORER(shred, JOFF(VM_Shred, mem), mem);
break;

  jit_value_t bytecode = JLOADR(next_code, JOFF(VM_Code, bytecode), void_ptr);
  jit_value_t _instr = JLOADR(bytecode, SZ_INT*1, void_ptr);
//  jit_function_t _fn = JLOADR(bytecode, SZ_INT*1, void_ptr);
  jit_value_t closure = JLOADR(bytecode, SZ_INT*2, void_ptr);
//assert(_instr == closure);
//  jit_function_t _fn = jit_function_from_closure(jb->context, closure);
  jit_value_t args[] = { shred, _instr };
  jit_insn_call_indirect(fn, closure, jb->signature, args, 2, JIT_CALL_NOTHROW);
//  jit_insn_call(fn, NULL, _fn, 0, args, 2, JIT_CALL_NOTHROW);
  break;
}

#define BRANCH_DISPATCH(type, op)               \
  jit_value_t val = JLOADR(reg, 0, type);       \
  jit_label_t label1 = jit_label_undefined;     \
  jit_insn_branch_if##op(fn, val, &label1);    \
  if(!same_block(jb, instr->m_val, jb->pc)) \
    jit_insn_branch(fn, &jb->label[instr->m_val]);\
   else {\
    jit_value_t pc = JCONST(nuint, instr->m_val); \
    JSTORER(shred, JOFF(VM_Shred, pc), pc);       \
    jit_insn_default_return(fn);                  \
  }\
  jit_insn_label(fn, &label1);                  \
  break;

case eBranchEqInt:
  reg = JADDR(reg, -SZ_INT);
//  BRANCH_DISPATCH(nuint, _not);
  BRANCH_DISPATCH(nuint, );


case eRegPop:
  reg = JADDR(reg, -instr->m_val);
  break;
case eNoOp:
  break;

case eOP_MAX:
{
// code ?
  JSTORER(shred, JOFF(VM_Shred, reg), reg);
  JSTORER(shred, JOFF(VM_Shred, mem), mem);
  JSTORER(shred, JOFF(VM_Shred, pc), JCONST(nuint, jb->pc));
  jit_value_t closure = JCONST(void_ptr, instr->m_val);
  jit_value_t _instr = JCONST(void_ptr, instr);
  jit_value_t args[] = { shred, _instr };
  jit_insn_call_indirect(fn, closure, jb->signature,
    args, 2, JIT_CALL_NOTHROW);
//  code = JSTORER(shred, JOFF(VM_Shred, reg), reg);
  reg = JLOADR(shred, JOFF(VM_Shred, reg), void_ptr);
  mem = JLOADR(shred, JOFF(VM_Shred, mem), void_ptr);
//  pc  = JLOADER(shred, JOFF(VM_Shred, pc), nuint);
  break;
}


default:
printf("\033[31mERROR %lu\033[0m\n", instr->opcode);
return vector_size(code->instr);
  break;
}
  if(jb->ctrl[jb->pc]) {
printf("start %lu end %lu %lu\n", jb->start, jb->pc, vector_size(code->instr));
    JSTORER(shred, JOFF(VM_Shred, reg), reg);
    JSTORER(shred, JOFF(VM_Shred, mem), mem);
//    printf(":!::::break %lu %lu\n", jb->ctrl[jb->pc], jb->pc);
    compilation_end(jb, fn);
    mk_instr(fn, code->bytecode + (jb->start * SZ_INT*3), 
    (Instr)vector_at(code->instr, jb->start));
    return ++jb->pc;
  }

  }
printf("[]start %lu end %lu %lu\n", jb->start, jb->pc, vector_size(code->instr));
  JSTORER(shred, JOFF(VM_Shred, reg), reg);
  JSTORER(shred, JOFF(VM_Shred, mem), mem);
    compilation_end(jb, fn);
    mk_instr(fn, code->bytecode + (jb->start * SZ_INT*3), 
    (Instr)vector_at(code->instr, jb->start));
  return vector_size(code->instr);
}

ANN void jb_run(struct JitBackend_ *jb, const VM_Code code) {
  jb->pc = 0;
m_bit ctrl[vector_size(code->instr)];
m_bit ctrl0[vector_size(code->instr)];
jit_label_t label[vector_size(code->instr)];
__builtin_memset(ctrl , 0, vector_size(code->instr));
__builtin_memset(ctrl0 , 0, vector_size(code->instr));
__builtin_memset(label , 0, sizeof(jit_label_t) *vector_size(code->instr));
  jb->ctrl0 = ctrl0;
  jb->ctrl = ctrl;
  jb->label = label;
  jb_ctrl0(jb, code);
  jb_ctrl(jb, code);
  while(jb_compile(jb, code) < vector_size(code->instr));
  REM_REF(code, jb->gwion);
}

ANN void jb_end(const Gwion gwion, struct JitBackend_ *jb) {
  jit_context_destroy(jb->context);
  mp_free(gwion->mp, JitBackend, jb);
}
