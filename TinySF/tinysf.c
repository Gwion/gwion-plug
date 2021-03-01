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

#define TSF_IMPLEMENTATION
#include "tsf.h"

#define TSF(o) (*(tsf**)(o->data + SZ_INT))

static TICK(tsf_tick) {
#ifndef USE_DOUBLE
  m_float f[2];
  tsf_render_float(u->module.gen.data, f, 1, TSF_STEREO_INTERLEAVED);
  UGEN(u->connect.multi->channel[0])->out = f[0];
  UGEN(u->connect.multi->channel[1])->out = f[1];
#else
puts("not implemented yet");
exit(3);
#endif
}

static CTOR(tinysf_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tsf_tick, TSF(o), 0);
}

// check it with a constructor
static DTOR(tinysf_dtor) { if(TSF(o))tsf_close(TSF(o)); }

static m_int o_tinysf_member_data;
static m_int o_tinysf_static_data;
static m_int* tinysf_static_value;

static MFUN(load_filename) { 
// check if there is already one
// or ... ctor
  UGEN(o)->module.gen.data = TSF(o) = tsf_load_filename(STRING(*(M_Object*)MEM(SZ_INT)));
tsf_set_output(TSF(o), TSF_STEREO_INTERLEAVED, shred->info->vm->bbq->si->sr, 0);

}

static MFUN(note_on) {
  tsf_note_on(TSF(o), 1, *(m_int*)MEM(SZ_INT), *(m_float*)MEM(SZ_INT*2));
}

static MFUN(note_off) {
  tsf_note_off(TSF(o), 1, *(m_int*)MEM(SZ_INT));
}

static SFUN(sfun) { /*code here */ }

GWION_IMPORT(TinySF) {
  DECL_OB(const Type, t_tinysf, = gwi_class_ini(gwi, "TinySF", "UGen"))
  t_tinysf->nspc->info->offset += sizeof(struct tsf);
  gwi_class_xtor(gwi, tinysf_ctor, tinysf_dtor);


//  GWI_BB(gwi_item_ini(gwi, "int", "member"))
//  GWI_BB((o_tinysf_member_data = gwi_item_end(gwi, ae_flag_none, num, 0)))

//  GWI_BB(gwi_item_ini(gwi, "int", "static"))
//  GWI_BB((o_tinysf_static_data = gwi_item_end(gwi, ae_flag_static, num, 1234)))

  GWI_BB(gwi_func_ini(gwi, "int", "load_filename"))
  GWI_BB(gwi_func_arg(gwi, "string", "arg"))
  GWI_BB(gwi_func_end(gwi, load_filename, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "noteOn"))
  GWI_BB(gwi_func_arg(gwi, "int", "note"))
  GWI_BB(gwi_func_arg(gwi, "float", "vel"))
  GWI_BB(gwi_func_end(gwi, note_on, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "noteOff"))
  GWI_BB(gwi_func_arg(gwi, "int", "note"))
  GWI_BB(gwi_func_end(gwi, note_off, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "sfun"))
  GWI_BB(gwi_func_arg(gwi, "int", "arg"))
  GWI_BB(gwi_func_end(gwi, sfun, ae_flag_static))

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
