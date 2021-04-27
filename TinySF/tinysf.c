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
#include "traverse.h"
#include "parse.h"
#include "emit.h"

#define TSF_IMPLEMENTATION
#include "tsf.h"

#define TSF(o) (*(tsf**)(o->data + SZ_INT))

static TICK(tsf_tick) {
  m_float f[2];
  tsf_render_float(u->module.gen.data, f, 1, TSF_STEREO_INTERLEAVED);
  UGEN(u->connect.multi->channel[0])->out = f[0];
  UGEN(u->connect.multi->channel[1])->out = f[1];
}

static INSTR(TinySFCtor) {
   POP_REG(shred, SZ_INT);
   tsf* tiny = tsf_load_filename(STRING(*(M_Object*)REG(-SZ_INT)));
   if(!tiny) {
     gw_err("file '%s' can't opened file for reading", STRING(*(M_Object*)REG(-SZ_INT)));
     handle(shred, "TinySFException");
     return;
   }
   tsf_set_output(tiny, TSF_STEREO_INTERLEAVED, shred->info->vm->bbq->si->sr, 0);
   const M_Object o = new_object(shred->info->vm->gwion->mp, shred, (Type)instr->m_val);
   UGEN(o) = new_UGen(shred->info->mp);
   UGEN(o)->module.gen.data = TSF(o) = tiny;
   vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
   ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 2);
   ugen_gen(shred->info->vm->gwion, UGEN(o), tsf_tick, tiny, 0);
   *(M_Object*)REG(-SZ_INT) = o;
}

static OP_CHECK(opck_tinysf_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  if(!call->args || call->args->next ||
     !check_exp(env, call->args) || isa(call->args->type, env->gwion->type[et_string]) < 0)
    ERR_N(call->func->pos, "TinySF constructor requires one "
         "and only one 'string' argument")
  return actual_type(env->gwion, call->func->type);
}

static OP_EMIT(opem_tinysf_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  const Instr instr = emit_add_instr(emit, TinySFCtor);
  instr->m_val = (m_uint) exp_self(call)->type;
  return GW_OK;
}

static DTOR(tinysf_dtor) { tsf_close(TSF(o)); }

static m_int o_tinysf_member_data;
static m_int o_tinysf_static_data;
static m_int* tinysf_static_value;

static MFUN(note_on) {
  tsf_note_on(TSF(o), 1, *(m_int*)MEM(SZ_INT), *(m_float*)MEM(SZ_INT*2));
}

static MFUN(note_off) {
  tsf_note_off(TSF(o), 1, *(m_int*)MEM(SZ_INT));
}

GWION_IMPORT(TinySF) {
  DECL_OB(const Type, t_tinysf, = gwi_class_ini(gwi, "TinySF", "UGen"));
  t_tinysf->nspc->info->offset += sizeof(struct tsf);
  gwi_class_xtor(gwi, NULL, tinysf_dtor);
  SET_FLAG(t_tinysf, abstract);

  GWI_BB(gwi_func_ini(gwi, "void", "noteOn"))
  GWI_BB(gwi_func_arg(gwi, "int", "note"))
  GWI_BB(gwi_func_arg(gwi, "float", "vel"))
  GWI_BB(gwi_func_end(gwi, note_on, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "noteOff"))
  GWI_BB(gwi_func_arg(gwi, "int", "note"))
  GWI_BB(gwi_func_end(gwi, note_off, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, NULL, "TinySF", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_tinysf_ctor))
  GWI_BB(gwi_oper_emi(gwi, opem_tinysf_ctor))
  gwi_oper_eff(gwi, "TinySFException");
  GWI_BB(gwi_oper_end(gwi, "@ctor", TinySFCtor))

  return GW_OK;
}
