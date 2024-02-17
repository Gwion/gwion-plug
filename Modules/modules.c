#include <math.h>
#include <soundpipe.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"

typedef struct SP_osc_ {
  sp_data* sp;
  sp_osc*  osc;
  sp_ftbl* tbl;
  m_float  phz;
} SP_osc; // copied from generated osc.c

static TICK(sinosc_tick) {
  const SP_osc* ug = (SP_osc*)u->module.gen.data;
  sp_osc_compute(ug->sp, ug->osc, NULL, &u->out);
}

ANN static void refresh_sine(const VM* vm, SP_osc* ug, const m_int sz, const m_float phz) {
  if(sz <= 0) {
    gw_err("%s size requested for sinosc. doing nothing",
            sz < 0 ? "negative" : "zero");
    return;
  }
  sp_data *sp=(sp_data*)get_module(vm->gwion, "Soundpipe");
  sp_ftbl_destroy(&ug->tbl);
  sp_osc_destroy(&ug->osc);
  sp_osc_create(&ug->osc);
  sp_ftbl_create(sp, &ug->tbl, sz);
  sp_gen_sine(sp, ug->tbl);
  sp_osc_init(sp, (sp_osc*)ug->osc, ug->tbl, phz);
}

static CTOR(sinosc_ctor) {
  sp_data *sp=(sp_data*)get_module(shred->info->vm->gwion, "Soundpipe");
  SP_osc* ug = (SP_osc*)xmalloc(sizeof(SP_osc));
  sp_osc_create(&ug->osc);
  sp_ftbl_create(sp, &ug->tbl, 2048);
  sp_gen_sine(sp, ug->tbl);
  sp_osc_init(sp, ug->osc, ug->tbl, 0.);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), sinosc_tick, ug, 0);
}

static DTOR(sinosc_dtor) {
  SP_osc* ug = (SP_osc*)UGEN(o)->module.gen.data;
  sp_osc_destroy(&ug->osc);
  sp_ftbl_destroy(&ug->tbl);
  free(ug);
}

static MFUN(sinosc_size) {
  const m_int size = *(m_int*)(shred->mem + SZ_INT);
  SP_osc* ug = (SP_osc*)UGEN(o)->module.gen.data;
  refresh_sine(shred->info->vm, ug, size, 0);
  *(M_Object*)RETURN = o;
}

static MFUN(sinosc_phase) {
  const m_float phase = *(m_float*)(shred->mem + SZ_INT);
  SP_osc* ug = (SP_osc*)UGEN(o)->module.gen.data;
  refresh_sine(shred->info->vm, ug, 2048, phase);
  *(M_Object*)RETURN = o;
}

static MFUN(sinosc_size_phase) {
  const m_int size    = *(m_int*)(shred->mem + SZ_INT);
  const m_float phase = *(m_float*)(shred->mem + SZ_INT * 2);
  SP_osc* ug = (SP_osc*)UGEN(o)->module.gen.data;
  refresh_sine(shred->info->vm, ug, size, phase);
  *(M_Object*)RETURN = o;
}

static MFUN(sinosc_get_freq) {
  const SP_osc* ug = (SP_osc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(sinosc_set_freq) {
  const SP_osc* ug = (SP_osc*)UGEN(o)->module.gen.data;
  const m_float freq = *(m_float*)(shred->mem + SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(sinosc_get_amp) {
  const SP_osc* ug = (SP_osc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(sinosc_set_amp) {
  const SP_osc* ug = (SP_osc*)UGEN(o)->module.gen.data;
  const m_float amp = *(m_float*)(shred->mem + SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static GWION_IMPORT(sinosc) {
  const Type t_sinosc = gwi_class_ini(gwi, "SinOsc", "UGen");
  gwi_class_xtor(gwi, sinosc_ctor, sinosc_dtor);
  gwi_func_ini(gwi, "auto", "new");
  gwi_func_arg(gwi, "int", "size");
  GWI_B(gwi_func_end(gwi, sinosc_size, ae_flag_none))
  gwi_func_ini(gwi, "auto", "new");
  gwi_func_arg(gwi, "float", "phase");
  GWI_B(gwi_func_end(gwi, sinosc_phase, ae_flag_none))
  gwi_func_ini(gwi, "auto", "new");
  gwi_func_arg(gwi, "int", "size");
  gwi_func_arg(gwi, "float", "phase");
  GWI_B(gwi_func_end(gwi, sinosc_size_phase, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_B(gwi_func_end(gwi, sinosc_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  gwi_func_arg(gwi, "float", "freq");
  GWI_B(gwi_func_end(gwi, sinosc_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_B(gwi_func_end(gwi, sinosc_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  gwi_func_arg(gwi, "float", "amp");
  GWI_B(gwi_func_end(gwi, sinosc_set_amp, ae_flag_none))
  GWI_B(gwi_class_end(gwi))
  UNSET_FLAG(t_sinosc, abstract);
  return 1;
}

static DTOR(basic_dtor) {
  free(UGEN(o)->module.gen.data);
}

static TICK(gain_tick) {
  u->out = (u->in * *(m_float*)u->module.gen.data);
}

static CTOR(gain_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), gain_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  UGEN(o)->module.gen.tick = gain_tick;
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static MFUN(gain_get_gain) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data;
}

static MFUN(gain_set_gain) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data = *(m_float*)MEM(SZ_INT);
}

static GWION_IMPORT(gain) {
  GWI_B(gwi_class_ini(gwi, "Gain", "UGen"))
  gwi_class_xtor(gwi, gain_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "gain");
  GWI_B(gwi_func_end(gwi, gain_get_gain, ae_flag_none))
  gwi_func_ini(gwi, "float", "gain");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_B(gwi_func_end(gwi, gain_set_gain, ae_flag_none))
  GWI_B(gwi_class_end(gwi))
  return 1;
}

static TICK(impulse_tick) {
  u->out = *(m_float*)u->module.gen.data;
  *(m_float*)u->module.gen.data = 0;
}

static CTOR(impulse_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), impulse_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 0;
}

static MFUN(impulse_get_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data;
}

static MFUN(impulse_set_next) {
  *(m_float*)RETURN = (*(m_float*)UGEN(o)->module.gen.data = *(m_float*)MEM(SZ_INT));
}

static GWION_IMPORT(impulse) {
  GWI_B(gwi_class_ini(gwi, "Impulse", "UGen"))
  gwi_class_xtor(gwi, impulse_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "next");
  GWI_B(gwi_func_end(gwi, impulse_get_next, ae_flag_none))
  gwi_func_ini(gwi, "float", "next");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_B(gwi_func_end(gwi, impulse_set_next, ae_flag_none))
  GWI_B(gwi_class_end(gwi))
  return 1;
}

static TICK(fullrect_tick) {
  u->out = fabs(u->in);
}

static CTOR(fullrect_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fullrect_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(fullrect) {
  GWI_B(gwi_class_ini(gwi, "FullRect", "UGen"))
  gwi_class_xtor(gwi, fullrect_ctor, basic_dtor);
  GWI_B(gwi_class_end(gwi))
  return 1;
}

static TICK(halfrect_tick) {
  if(u->in > 0)
    u->out = u->in;
  else
    u->out = 0;
}

static CTOR(halfrect_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), halfrect_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(halfrect) {
  GWI_B(gwi_class_ini(gwi,  "HalfRect", "UGen"))
  gwi_class_xtor(gwi, halfrect_ctor, basic_dtor);
  GWI_B(gwi_class_end(gwi))
  return 1;
}

static TICK(step_tick) {
  u->out = *(m_float*)u->module.gen.data;
}

static CTOR(step_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), step_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 0;
}

static MFUN(step_get_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data;
}

static MFUN(step_set_next) {
  *(m_float*)RETURN = *(m_float*)UGEN(o)->module.gen.data = *(m_float*)(shred->mem + SZ_INT);
}

static GWION_IMPORT(step) {
  GWI_B(gwi_class_ini(gwi, "Step", "UGen"))
  gwi_class_xtor(gwi, step_ctor, basic_dtor);
  gwi_func_ini(gwi, "float", "next");
  GWI_B(gwi_func_end(gwi, step_get_next, ae_flag_none))
  gwi_func_ini(gwi, "float", "next");
  gwi_func_arg(gwi, "float", "arg0");
  GWI_B(gwi_func_end(gwi, step_set_next, ae_flag_none))
  GWI_B(gwi_class_end(gwi))
  return 1;
}

static TICK(zerox_tick) {
  m_float in = (u->in < 0) ? -1 : (u->in > 0);
  m_float f = *(m_float*)u->module.gen.data;
  u->out = f == in ? 1 : 0;
  *(m_float*) u->module.gen.data = in;
}

static CTOR(zerox_ctor) {
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), zerox_tick, (m_float*)xmalloc(SZ_FLOAT), 0);
  *(m_float*)UGEN(o)->module.gen.data = 1;
}

static GWION_IMPORT(zerox) {
  GWI_B(gwi_class_ini(gwi, "ZeroX", "UGen"))
  gwi_class_xtor(gwi, zerox_ctor, basic_dtor);
  GWI_B(gwi_class_end(gwi))
  return 1;
}

GWION_IMPORT(Modules) {
  GWI_B(import_sinosc(gwi))
  GWI_B(import_gain(gwi))
  GWI_B(import_impulse(gwi))
  GWI_B(import_fullrect(gwi))
  GWI_B(import_halfrect(gwi))
  GWI_B(import_step(gwi))
  GWI_B(import_zerox(gwi))

  GWI_B(gwi_typedef_ini(gwi, "Bltriangle", "TriOsc"))
  GWI_B(gwi_typedef_end(gwi, ae_flag_none))
  GWI_B(gwi_typedef_ini(gwi, "Blsquare", "SquareOsc"))
  GWI_B(gwi_typedef_end(gwi, ae_flag_none))
  GWI_B(gwi_typedef_ini(gwi, "Blsaw", "SawOsc"))
  GWI_B(gwi_typedef_end(gwi, ae_flag_none))
  return true;
}

static m_str deps[] = { "Soundpipe", NULL };
GWDEPEND(Modules){ return deps; }
