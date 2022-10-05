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

static MFUN(tinysf_new) {
   tsf* tiny = tsf_load_filename(STRING(*(M_Object*)MEM(SZ_INT)));
   if(!tiny) {
     gw_err("file '%s' can't opened file for reading", STRING(*(M_Object*)MEM(SZ_INT)));
     xfun_handle(shred, "TinySFException");
     return;
   }
   tsf_set_output(tiny, TSF_STEREO_INTERLEAVED, shred->info->vm->bbq->si->sr, 0);
   UGEN(o) = new_UGen(shred->info->mp);
   UGEN(o)->module.gen.data = TSF(o) = tiny;
   vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
   ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 2);
   ugen_gen(shred->info->vm->gwion, UGEN(o), tsf_tick, tiny, 0);
   *(M_Object*)RETURN = o;
}

static DTOR(tinysf_dtor) { tsf_close(TSF(o)); }

static MFUN(note_on) {
  tsf_note_on(TSF(o), 1, *(m_int*)MEM(SZ_INT), *(m_float*)MEM(SZ_INT*2));
}

static MFUN(note_off) {
  tsf_note_off(TSF(o), 1, *(m_int*)MEM(SZ_INT));
}

GWION_IMPORT(TinySF) {
  DECL_OB(const Type, t_tinysf, = gwi_class_ini(gwi, "TinySF", "UGen"));
  t_tinysf->nspc->offset += sizeof(struct tsf);
  gwi_class_xtor(gwi, NULL, tinysf_dtor);

  GWI_BB(gwi_func_ini(gwi, "auto", "new"))
  GWI_BB(gwi_func_arg(gwi, "string", "file"))
  GWI_BB(gwi_func_end(gwi, tinysf_new, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "noteOn"))
  GWI_BB(gwi_func_arg(gwi, "int", "note"))
  GWI_BB(gwi_func_arg(gwi, "float", "vel"))
  GWI_BB(gwi_func_end(gwi, note_on, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "noteOff"))
  GWI_BB(gwi_func_arg(gwi, "int", "note"))
  GWI_BB(gwi_func_end(gwi, note_off, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

  return GW_OK;
}
