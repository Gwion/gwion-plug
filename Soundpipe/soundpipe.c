#include <stdlib.h>
#include <soundpipe.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "err_msg.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "gwi.h"
#include "array.h"
#define FTBL(o) *((sp_ftbl**)((M_Object)o)->data)
#define CHECK_SIZE(size)  if(size <= 0){fprintf(stderr, "'gen_ftbl' size argument must be more than 0");return;}
#define sp_handle(a,c) { xfun_handle(a,c); return; }

static DTOR(ftbl_dtor) {
  if(FTBL(o))
    sp_ftbl_destroy(&FTBL(o));
}

typedef struct {
  sp_data* sp;
  sp_adsr* osc;
} GW_adsr;

static TICK(adsr_tick) {
  const GW_adsr* ug = (GW_adsr*)u->module.gen.data;
  sp_adsr_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(adsr_dtor) {
  GW_adsr* ug = UGEN(o)->module.gen.data;
  sp_adsr_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_adsr), ug);
}

static MFUN(adsr_init) {
  GW_adsr* ug = mp_malloc2(shred->info->mp, sizeof(GW_adsr));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), adsr_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_adsr_create(&ug->osc) == SP_NOT_OK || sp_adsr_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(adsr_get_atk) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(adsr_set_atk) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(adsr_get_dec) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(adsr_set_dec) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

static MFUN(adsr_get_sus) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->sus;
}

static MFUN(adsr_set_sus) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  m_float sus = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->sus = sus);
}

static MFUN(adsr_get_rel) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(adsr_set_rel) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
  sp_data* sp;
  sp_allpass* osc;
} GW_allpass;

static TICK(allpass_tick) {
  const GW_allpass* ug = (GW_allpass*)u->module.gen.data;
  sp_allpass_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(allpass_dtor) {
  GW_allpass* ug = UGEN(o)->module.gen.data;
  sp_allpass_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_allpass), ug);
}

static MFUN(allpass_init) {
  GW_allpass* ug = mp_malloc2(shred->info->mp, sizeof(GW_allpass));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), allpass_tick, ug, 0);
  m_float looptime = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_allpass_create(&ug->osc) == SP_NOT_OK || sp_allpass_init(ug->sp, ug->osc , looptime) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(allpass_get_revtime) {
  const GW_allpass* ug = (GW_allpass*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->revtime;
}

static MFUN(allpass_set_revtime) {
  const GW_allpass* ug = (GW_allpass*)UGEN(o)->module.gen.data;
  m_float revtime = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->revtime = revtime);
}

typedef struct {
  sp_data* sp;
  sp_atone* osc;
} GW_atone;

static TICK(atone_tick) {
  const GW_atone* ug = (GW_atone*)u->module.gen.data;
  sp_atone_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(atone_dtor) {
  GW_atone* ug = UGEN(o)->module.gen.data;
  sp_atone_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_atone), ug);
}

static MFUN(atone_init) {
  GW_atone* ug = mp_malloc2(shred->info->mp, sizeof(GW_atone));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), atone_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_atone_create(&ug->osc) == SP_NOT_OK || sp_atone_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(atone_get_hp) {
  const GW_atone* ug = (GW_atone*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->hp;
}

static MFUN(atone_set_hp) {
  const GW_atone* ug = (GW_atone*)UGEN(o)->module.gen.data;
  m_float hp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->hp = hp);
}

typedef struct {
  sp_data* sp;
  sp_autowah* osc;
} GW_autowah;

static TICK(autowah_tick) {
  const GW_autowah* ug = (GW_autowah*)u->module.gen.data;
  sp_autowah_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(autowah_dtor) {
  GW_autowah* ug = UGEN(o)->module.gen.data;
  sp_autowah_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_autowah), ug);
}

static MFUN(autowah_init) {
  GW_autowah* ug = mp_malloc2(shred->info->mp, sizeof(GW_autowah));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), autowah_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_autowah_create(&ug->osc) == SP_NOT_OK || sp_autowah_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(autowah_get_level) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->level;
}

static MFUN(autowah_set_level) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  m_float level = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->level = level);
}

static MFUN(autowah_get_wah) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->wah;
}

static MFUN(autowah_set_wah) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  m_float wah = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->wah = wah);
}

static MFUN(autowah_get_mix) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->mix;
}

static MFUN(autowah_set_mix) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  m_float mix = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->mix = mix);
}

typedef struct {
  sp_data* sp;
  sp_bal* osc;
} GW_bal;

static TICK(bal_tick) {
  const GW_bal* ug = (GW_bal*)u->module.gen.data;
  sp_bal_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);
}

static DTOR(bal_dtor) {
  GW_bal* ug = UGEN(o)->module.gen.data;
  sp_bal_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_bal), ug);
}

static MFUN(bal_init) {
  GW_bal* ug = mp_malloc2(shred->info->mp, sizeof(GW_bal));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), bal_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_bal_create(&ug->osc) == SP_NOT_OK || sp_bal_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_bar* osc;
} GW_bar;

static TICK(bar_tick) {
  const GW_bar* ug = (GW_bar*)u->module.gen.data;
  sp_bar_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(bar_dtor) {
  GW_bar* ug = UGEN(o)->module.gen.data;
  sp_bar_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_bar), ug);
}

static MFUN(bar_init) {
  GW_bar* ug = mp_malloc2(shred->info->mp, sizeof(GW_bar));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), bar_tick, ug, 1);
  m_float iK = *(m_float*)(shred->mem+SZ_INT);
  m_float ib = *(m_float*)(shred->mem+SZ_INT+SZ_FLOAT);
  *(M_Object*)RETURN = o;
  if(sp_bar_create(&ug->osc) == SP_NOT_OK || sp_bar_init(ug->sp, ug->osc , iK, ib) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(bar_get_bcL) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bcL;
}

static MFUN(bar_set_bcL) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float bcL = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bcL = bcL);
}

static MFUN(bar_get_bcR) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bcR;
}

static MFUN(bar_set_bcR) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float bcR = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bcR = bcR);
}

static MFUN(bar_get_T30) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->T30;
}

static MFUN(bar_set_T30) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float T30 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->T30 = T30);
}

static MFUN(bar_get_scan) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->scan;
}

static MFUN(bar_set_scan) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float scan = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->scan = scan);
}

static MFUN(bar_get_pos) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pos;
}

static MFUN(bar_set_pos) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float pos = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->pos = pos);
}

static MFUN(bar_get_vel) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->vel;
}

static MFUN(bar_set_vel) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float vel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->vel = vel);
}

static MFUN(bar_get_wid) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->wid;
}

static MFUN(bar_set_wid) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float wid = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->wid = wid);
}

typedef struct {
  sp_data* sp;
  sp_biquad* osc;
} GW_biquad;

static TICK(biquad_tick) {
  const GW_biquad* ug = (GW_biquad*)u->module.gen.data;
  sp_biquad_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(biquad_dtor) {
  GW_biquad* ug = UGEN(o)->module.gen.data;
  sp_biquad_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_biquad), ug);
}

static MFUN(biquad_init) {
  GW_biquad* ug = mp_malloc2(shred->info->mp, sizeof(GW_biquad));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), biquad_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_biquad_create(&ug->osc) == SP_NOT_OK || sp_biquad_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(biquad_get_b0) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b0;
}

static MFUN(biquad_set_b0) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float b0 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->b0 = b0);
}

static MFUN(biquad_get_b1) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b1;
}

static MFUN(biquad_set_b1) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float b1 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->b1 = b1);
}

static MFUN(biquad_get_b2) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b2;
}

static MFUN(biquad_set_b2) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float b2 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->b2 = b2);
}

static MFUN(biquad_get_a0) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a0;
}

static MFUN(biquad_set_a0) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float a0 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->a0 = a0);
}

static MFUN(biquad_get_a1) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a1;
}

static MFUN(biquad_set_a1) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float a1 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->a1 = a1);
}

static MFUN(biquad_get_a2) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a2;
}

static MFUN(biquad_set_a2) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float a2 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->a2 = a2);
}

typedef struct {
  sp_data* sp;
  sp_biscale* osc;
} GW_biscale;

static TICK(biscale_tick) {
  const GW_biscale* ug = (GW_biscale*)u->module.gen.data;
  sp_biscale_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(biscale_dtor) {
  GW_biscale* ug = UGEN(o)->module.gen.data;
  sp_biscale_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_biscale), ug);
}

static MFUN(biscale_init) {
  GW_biscale* ug = mp_malloc2(shred->info->mp, sizeof(GW_biscale));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), biscale_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_biscale_create(&ug->osc) == SP_NOT_OK || sp_biscale_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(biscale_get_min) {
  const GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(biscale_set_min) {
  const GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(biscale_get_max) {
  const GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(biscale_set_max) {
  const GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
  sp_data* sp;
  sp_bitcrush* osc;
} GW_bitcrush;

static TICK(bitcrush_tick) {
  const GW_bitcrush* ug = (GW_bitcrush*)u->module.gen.data;
  sp_bitcrush_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(bitcrush_dtor) {
  GW_bitcrush* ug = UGEN(o)->module.gen.data;
  sp_bitcrush_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_bitcrush), ug);
}

static MFUN(bitcrush_init) {
  GW_bitcrush* ug = mp_malloc2(shred->info->mp, sizeof(GW_bitcrush));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), bitcrush_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_bitcrush_create(&ug->osc) == SP_NOT_OK || sp_bitcrush_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(bitcrush_get_bitdepth) {
  const GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bitdepth;
}

static MFUN(bitcrush_set_bitdepth) {
  const GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
  m_float bitdepth = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bitdepth = bitdepth);
}

static MFUN(bitcrush_get_srate) {
  const GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->srate;
}

static MFUN(bitcrush_set_srate) {
  const GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
  m_float srate = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->srate = srate);
}

typedef struct {
  sp_data* sp;
  sp_blsaw* osc;
} GW_blsaw;

static TICK(blsaw_tick) {
  const GW_blsaw* ug = (GW_blsaw*)u->module.gen.data;
  sp_blsaw_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(blsaw_dtor) {
  GW_blsaw* ug = UGEN(o)->module.gen.data;
  sp_blsaw_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_blsaw), ug);
}

static MFUN(blsaw_init) {
  GW_blsaw* ug = mp_malloc2(shred->info->mp, sizeof(GW_blsaw));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), blsaw_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_blsaw_create(&ug->osc) == SP_NOT_OK || sp_blsaw_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(blsaw_get_freq) {
  const GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->freq;
}

static MFUN(blsaw_set_freq) {
  const GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->freq = freq);
}

static MFUN(blsaw_get_amp) {
  const GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->amp;
}

static MFUN(blsaw_set_amp) {
  const GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_blsquare* osc;
} GW_blsquare;

static TICK(blsquare_tick) {
  const GW_blsquare* ug = (GW_blsquare*)u->module.gen.data;
  sp_blsquare_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(blsquare_dtor) {
  GW_blsquare* ug = UGEN(o)->module.gen.data;
  sp_blsquare_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_blsquare), ug);
}

static MFUN(blsquare_init) {
  GW_blsquare* ug = mp_malloc2(shred->info->mp, sizeof(GW_blsquare));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), blsquare_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_blsquare_create(&ug->osc) == SP_NOT_OK || sp_blsquare_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(blsquare_get_freq) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->freq;
}

static MFUN(blsquare_set_freq) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->freq = freq);
}

static MFUN(blsquare_get_amp) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->amp;
}

static MFUN(blsquare_set_amp) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->amp = amp);
}

static MFUN(blsquare_get_width) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->width;
}

static MFUN(blsquare_set_width) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  m_float width = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->width = width);
}

typedef struct {
  sp_data* sp;
  sp_bltriangle* osc;
} GW_bltriangle;

static TICK(bltriangle_tick) {
  const GW_bltriangle* ug = (GW_bltriangle*)u->module.gen.data;
  sp_bltriangle_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(bltriangle_dtor) {
  GW_bltriangle* ug = UGEN(o)->module.gen.data;
  sp_bltriangle_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_bltriangle), ug);
}

static MFUN(bltriangle_init) {
  GW_bltriangle* ug = mp_malloc2(shred->info->mp, sizeof(GW_bltriangle));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), bltriangle_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_bltriangle_create(&ug->osc) == SP_NOT_OK || sp_bltriangle_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(bltriangle_get_freq) {
  const GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->freq;
}

static MFUN(bltriangle_set_freq) {
  const GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->freq = freq);
}

static MFUN(bltriangle_get_amp) {
  const GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->amp;
}

static MFUN(bltriangle_set_amp) {
  const GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_brown* osc;
} GW_brown;

static TICK(brown_tick) {
  const GW_brown* ug = (GW_brown*)u->module.gen.data;
  sp_brown_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(brown_dtor) {
  GW_brown* ug = UGEN(o)->module.gen.data;
  sp_brown_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_brown), ug);
}

static MFUN(brown_init) {
  GW_brown* ug = mp_malloc2(shred->info->mp, sizeof(GW_brown));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), brown_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_brown_create(&ug->osc) == SP_NOT_OK || sp_brown_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_butbp* osc;
} GW_butbp;

static TICK(butbp_tick) {
  const GW_butbp* ug = (GW_butbp*)u->module.gen.data;
  sp_butbp_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(butbp_dtor) {
  GW_butbp* ug = UGEN(o)->module.gen.data;
  sp_butbp_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_butbp), ug);
}

static MFUN(butbp_init) {
  GW_butbp* ug = mp_malloc2(shred->info->mp, sizeof(GW_butbp));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), butbp_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_butbp_create(&ug->osc) == SP_NOT_OK || sp_butbp_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(butbp_get_freq) {
  const GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(butbp_set_freq) {
  const GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(butbp_get_bw) {
  const GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bw;
}

static MFUN(butbp_set_bw) {
  const GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
  m_float bw = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bw = bw);
}

typedef struct {
  sp_data* sp;
  sp_butbr* osc;
} GW_butbr;

static TICK(butbr_tick) {
  const GW_butbr* ug = (GW_butbr*)u->module.gen.data;
  sp_butbr_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(butbr_dtor) {
  GW_butbr* ug = UGEN(o)->module.gen.data;
  sp_butbr_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_butbr), ug);
}

static MFUN(butbr_init) {
  GW_butbr* ug = mp_malloc2(shred->info->mp, sizeof(GW_butbr));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), butbr_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_butbr_create(&ug->osc) == SP_NOT_OK || sp_butbr_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(butbr_get_freq) {
  const GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(butbr_set_freq) {
  const GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(butbr_get_bw) {
  const GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bw;
}

static MFUN(butbr_set_bw) {
  const GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
  m_float bw = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bw = bw);
}

typedef struct {
  sp_data* sp;
  sp_buthp* osc;
} GW_buthp;

static TICK(buthp_tick) {
  const GW_buthp* ug = (GW_buthp*)u->module.gen.data;
  sp_buthp_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(buthp_dtor) {
  GW_buthp* ug = UGEN(o)->module.gen.data;
  sp_buthp_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_buthp), ug);
}

static MFUN(buthp_init) {
  GW_buthp* ug = mp_malloc2(shred->info->mp, sizeof(GW_buthp));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), buthp_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_buthp_create(&ug->osc) == SP_NOT_OK || sp_buthp_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(buthp_get_freq) {
  const GW_buthp* ug = (GW_buthp*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(buthp_set_freq) {
  const GW_buthp* ug = (GW_buthp*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
  sp_data* sp;
  sp_butlp* osc;
} GW_butlp;

static TICK(butlp_tick) {
  const GW_butlp* ug = (GW_butlp*)u->module.gen.data;
  sp_butlp_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(butlp_dtor) {
  GW_butlp* ug = UGEN(o)->module.gen.data;
  sp_butlp_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_butlp), ug);
}

static MFUN(butlp_init) {
  GW_butlp* ug = mp_malloc2(shred->info->mp, sizeof(GW_butlp));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), butlp_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_butlp_create(&ug->osc) == SP_NOT_OK || sp_butlp_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(butlp_get_freq) {
  const GW_butlp* ug = (GW_butlp*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(butlp_set_freq) {
  const GW_butlp* ug = (GW_butlp*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
  sp_data* sp;
  sp_clip* osc;
} GW_clip;

static TICK(clip_tick) {
  const GW_clip* ug = (GW_clip*)u->module.gen.data;
  sp_clip_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(clip_dtor) {
  GW_clip* ug = UGEN(o)->module.gen.data;
  sp_clip_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_clip), ug);
}

static MFUN(clip_init) {
  GW_clip* ug = mp_malloc2(shred->info->mp, sizeof(GW_clip));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), clip_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_clip_create(&ug->osc) == SP_NOT_OK || sp_clip_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(clip_get_lim) {
  const GW_clip* ug = (GW_clip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->lim;
}

static MFUN(clip_set_lim) {
  const GW_clip* ug = (GW_clip*)UGEN(o)->module.gen.data;
  m_float lim = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->lim = lim);
}

typedef struct {
  sp_data* sp;
  sp_clock* osc;
} GW_clock;

static TICK(clock_tick) {
  const GW_clock* ug = (GW_clock*)u->module.gen.data;
  sp_clock_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(clock_dtor) {
  GW_clock* ug = UGEN(o)->module.gen.data;
  sp_clock_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_clock), ug);
}

static MFUN(clock_init) {
  GW_clock* ug = mp_malloc2(shred->info->mp, sizeof(GW_clock));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), clock_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_clock_create(&ug->osc) == SP_NOT_OK || sp_clock_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(clock_get_bpm) {
  const GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bpm;
}

static MFUN(clock_set_bpm) {
  const GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
  m_float bpm = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bpm = bpm);
}

static MFUN(clock_get_subdiv) {
  const GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->subdiv;
}

static MFUN(clock_set_subdiv) {
  const GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
  m_float subdiv = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->subdiv = subdiv);
}

typedef struct {
  sp_data* sp;
  sp_comb* osc;
} GW_comb;

static TICK(comb_tick) {
  const GW_comb* ug = (GW_comb*)u->module.gen.data;
  sp_comb_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(comb_dtor) {
  GW_comb* ug = UGEN(o)->module.gen.data;
  sp_comb_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_comb), ug);
}

static MFUN(comb_init) {
  GW_comb* ug = mp_malloc2(shred->info->mp, sizeof(GW_comb));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), comb_tick, ug, 0);
  m_float looptime = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_comb_create(&ug->osc) == SP_NOT_OK || sp_comb_init(ug->sp, ug->osc , looptime) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(comb_get_revtime) {
  const GW_comb* ug = (GW_comb*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->revtime;
}

static MFUN(comb_set_revtime) {
  const GW_comb* ug = (GW_comb*)UGEN(o)->module.gen.data;
  m_float revtime = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->revtime = revtime);
}

typedef struct {
  sp_data* sp;
  sp_compressor* osc;
} GW_compressor;

static TICK(compressor_tick) {
  const GW_compressor* ug = (GW_compressor*)u->module.gen.data;
  sp_compressor_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(compressor_dtor) {
  GW_compressor* ug = UGEN(o)->module.gen.data;
  sp_compressor_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_compressor), ug);
}

static MFUN(compressor_init) {
  GW_compressor* ug = mp_malloc2(shred->info->mp, sizeof(GW_compressor));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), compressor_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_compressor_create(&ug->osc) == SP_NOT_OK || sp_compressor_init(ug->sp, ug->osc ) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(compressor_get_ratio) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->ratio;
}

static MFUN(compressor_set_ratio) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  m_float ratio = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->ratio = ratio);
}

static MFUN(compressor_get_thresh) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->thresh;
}

static MFUN(compressor_set_thresh) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  m_float thresh = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->thresh = thresh);
}

static MFUN(compressor_get_atk) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->atk;
}

static MFUN(compressor_set_atk) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->atk = atk);
}

static MFUN(compressor_get_rel) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->rel;
}

static MFUN(compressor_set_rel) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->rel = rel);
}

typedef struct {
  sp_data* sp;
  sp_conv* osc;
  M_Object ft_obj;

} GW_conv;

static TICK(conv_tick) {
  const GW_conv* ug = (GW_conv*)u->module.gen.data;
  sp_conv_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(conv_dtor) {
  GW_conv* ug = UGEN(o)->module.gen.data;
  release(ug->ft_obj, shred);

  sp_conv_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_conv), ug);
}

static MFUN(conv_init) {
  GW_conv* ug = mp_malloc2(shred->info->mp, sizeof(GW_conv));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), conv_tick, ug, 0);
  const M_Object ft_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* ft = FTBL(ft_obj);
  ++ft_obj->ref;
  m_float iPartLen = *(m_float*)(shred->mem+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_conv_create(&ug->osc) == SP_NOT_OK || sp_conv_init(ug->sp, ug->osc , ft, iPartLen) == SP_NOT_OK) {
    release(ft_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->ft_obj = ft_obj;
}

typedef struct {
  sp_data* sp;
  sp_count* osc;
} GW_count;

static TICK(count_tick) {
  const GW_count* ug = (GW_count*)u->module.gen.data;
  sp_count_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(count_dtor) {
  GW_count* ug = UGEN(o)->module.gen.data;
  sp_count_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_count), ug);
}

static MFUN(count_init) {
  GW_count* ug = mp_malloc2(shred->info->mp, sizeof(GW_count));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), count_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_count_create(&ug->osc) == SP_NOT_OK || sp_count_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(count_get_count) {
  const GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->count;
}

static MFUN(count_set_count) {
  const GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
  m_float count = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->count = count);
}

static MFUN(count_get_mode) {
  const GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mode;
}

static MFUN(count_set_mode) {
  const GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
  m_float mode = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
  sp_data* sp;
  sp_crossfade* osc;
} GW_crossfade;

static TICK(crossfade_tick) {
  const GW_crossfade* ug = (GW_crossfade*)u->module.gen.data;
  sp_crossfade_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);
}

static DTOR(crossfade_dtor) {
  GW_crossfade* ug = UGEN(o)->module.gen.data;
  sp_crossfade_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_crossfade), ug);
}

static MFUN(crossfade_init) {
  GW_crossfade* ug = mp_malloc2(shred->info->mp, sizeof(GW_crossfade));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), crossfade_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_crossfade_create(&ug->osc) == SP_NOT_OK || sp_crossfade_init(ug->sp, ug->osc ) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(crossfade_get_pos) {
  const GW_crossfade* ug = (GW_crossfade*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pos;
}

static MFUN(crossfade_set_pos) {
  const GW_crossfade* ug = (GW_crossfade*)UGEN(o)->module.gen.data;
  m_float pos = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->pos = pos);
}

typedef struct {
  sp_data* sp;
  sp_dcblock* osc;
} GW_dcblock;

static TICK(dcblock_tick) {
  const GW_dcblock* ug = (GW_dcblock*)u->module.gen.data;
  sp_dcblock_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(dcblock_dtor) {
  GW_dcblock* ug = UGEN(o)->module.gen.data;
  sp_dcblock_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_dcblock), ug);
}

static MFUN(dcblock_init) {
  GW_dcblock* ug = mp_malloc2(shred->info->mp, sizeof(GW_dcblock));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dcblock_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_dcblock_create(&ug->osc) == SP_NOT_OK || sp_dcblock_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_delay* osc;
} GW_delay;

static TICK(delay_tick) {
  const GW_delay* ug = (GW_delay*)u->module.gen.data;
  sp_delay_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(delay_dtor) {
  GW_delay* ug = UGEN(o)->module.gen.data;
  sp_delay_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_delay), ug);
}

static MFUN(delay_init) {
  GW_delay* ug = mp_malloc2(shred->info->mp, sizeof(GW_delay));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), delay_tick, ug, 0);
  m_float time = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_delay_create(&ug->osc) == SP_NOT_OK || sp_delay_init(ug->sp, ug->osc , time) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(delay_get_feedback) {
  const GW_delay* ug = (GW_delay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->feedback;
}

static MFUN(delay_set_feedback) {
  const GW_delay* ug = (GW_delay*)UGEN(o)->module.gen.data;
  m_float feedback = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->feedback = feedback);
}

typedef struct {
  sp_data* sp;
  sp_diode* osc;
} GW_diode;

static TICK(diode_tick) {
  const GW_diode* ug = (GW_diode*)u->module.gen.data;
  sp_diode_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(diode_dtor) {
  GW_diode* ug = UGEN(o)->module.gen.data;
  sp_diode_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_diode), ug);
}

static MFUN(diode_init) {
  GW_diode* ug = mp_malloc2(shred->info->mp, sizeof(GW_diode));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), diode_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_diode_create(&ug->osc) == SP_NOT_OK || sp_diode_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(diode_get_freq) {
  const GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(diode_set_freq) {
  const GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(diode_get_res) {
  const GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(diode_set_res) {
  const GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->res = res);
}

typedef struct {
  sp_data* sp;
  sp_diskin* osc;
} GW_diskin;

static TICK(diskin_tick) {
  const GW_diskin* ug = (GW_diskin*)u->module.gen.data;
  sp_diskin_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(diskin_dtor) {
  GW_diskin* ug = UGEN(o)->module.gen.data;
  sp_diskin_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_diskin), ug);
}

static MFUN(diskin_init) {
  GW_diskin* ug = mp_malloc2(shred->info->mp, sizeof(GW_diskin));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), diskin_tick, ug, 0);
  M_Object filename_obj = *(M_Object*)(shred->mem+SZ_INT);
  m_str filename = STRING(filename_obj);
  *(M_Object*)RETURN = o;
  if(sp_diskin_create(&ug->osc) == SP_NOT_OK || sp_diskin_init(ug->sp, ug->osc , filename) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_dist* osc;
} GW_dist;

static TICK(dist_tick) {
  const GW_dist* ug = (GW_dist*)u->module.gen.data;
  sp_dist_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(dist_dtor) {
  GW_dist* ug = UGEN(o)->module.gen.data;
  sp_dist_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_dist), ug);
}

static MFUN(dist_init) {
  GW_dist* ug = mp_malloc2(shred->info->mp, sizeof(GW_dist));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dist_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_dist_create(&ug->osc) == SP_NOT_OK || sp_dist_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(dist_get_pregain) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pregain;
}

static MFUN(dist_set_pregain) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  m_float pregain = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->pregain = pregain);
}

static MFUN(dist_get_postgain) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->postgain;
}

static MFUN(dist_set_postgain) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  m_float postgain = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->postgain = postgain);
}

static MFUN(dist_get_shape1) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->shape1;
}

static MFUN(dist_set_shape1) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  m_float shape1 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->shape1 = shape1);
}

static MFUN(dist_get_shape2) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->shape2;
}

static MFUN(dist_set_shape2) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  m_float shape2 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->shape2 = shape2);
}

typedef struct {
  sp_data* sp;
  sp_dmetro* osc;
} GW_dmetro;

static TICK(dmetro_tick) {
  const GW_dmetro* ug = (GW_dmetro*)u->module.gen.data;
  sp_dmetro_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(dmetro_dtor) {
  GW_dmetro* ug = UGEN(o)->module.gen.data;
  sp_dmetro_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_dmetro), ug);
}

static MFUN(dmetro_init) {
  GW_dmetro* ug = mp_malloc2(shred->info->mp, sizeof(GW_dmetro));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dmetro_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_dmetro_create(&ug->osc) == SP_NOT_OK || sp_dmetro_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(dmetro_get_time) {
  const GW_dmetro* ug = (GW_dmetro*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->time;
}

static MFUN(dmetro_set_time) {
  const GW_dmetro* ug = (GW_dmetro*)UGEN(o)->module.gen.data;
  m_float time = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->time = time);
}

typedef struct {
  sp_data* sp;
  sp_drip* osc;
} GW_drip;

static TICK(drip_tick) {
  const GW_drip* ug = (GW_drip*)u->module.gen.data;
  sp_drip_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(drip_dtor) {
  GW_drip* ug = UGEN(o)->module.gen.data;
  sp_drip_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_drip), ug);
}

static MFUN(drip_init) {
  GW_drip* ug = mp_malloc2(shred->info->mp, sizeof(GW_drip));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), drip_tick, ug, 1);
  m_float dettack = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_drip_create(&ug->osc) == SP_NOT_OK || sp_drip_init(ug->sp, ug->osc , dettack) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(drip_get_num_tubes) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->num_tubes;
}

static MFUN(drip_set_num_tubes) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float num_tubes = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->num_tubes = num_tubes);
}

static MFUN(drip_get_amp) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(drip_set_amp) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(drip_get_damp) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->damp;
}

static MFUN(drip_set_damp) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float damp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->damp = damp);
}

static MFUN(drip_get_shake_max) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->shake_max;
}

static MFUN(drip_set_shake_max) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float shake_max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->shake_max = shake_max);
}

static MFUN(drip_get_freq) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(drip_set_freq) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(drip_get_freq1) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq1;
}

static MFUN(drip_set_freq1) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float freq1 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq1 = freq1);
}

static MFUN(drip_get_freq2) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq2;
}

static MFUN(drip_set_freq2) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float freq2 = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq2 = freq2);
}

typedef struct {
  sp_data* sp;
  sp_dtrig* osc;
  M_Object ft_obj;

} GW_dtrig;

static TICK(dtrig_tick) {
  const GW_dtrig* ug = (GW_dtrig*)u->module.gen.data;
  sp_dtrig_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(dtrig_dtor) {
  GW_dtrig* ug = UGEN(o)->module.gen.data;
  release(ug->ft_obj, shred);

  sp_dtrig_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_dtrig), ug);
}

static MFUN(dtrig_init) {
  GW_dtrig* ug = mp_malloc2(shred->info->mp, sizeof(GW_dtrig));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dtrig_tick, ug, 1);
  const M_Object ft_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* ft = FTBL(ft_obj);
  ++ft_obj->ref;
  *(M_Object*)RETURN = o;
  if(sp_dtrig_create(&ug->osc) == SP_NOT_OK || sp_dtrig_init(ug->sp, ug->osc , ft) == SP_NOT_OK) {
    release(ft_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->ft_obj = ft_obj;
}

static MFUN(dtrig_get_loop) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->loop;
}

static MFUN(dtrig_set_loop) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  m_int loop = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->loop = loop);
}

static MFUN(dtrig_get_delay) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->delay;
}

static MFUN(dtrig_set_delay) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  m_float delay = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->delay = delay);
}

static MFUN(dtrig_get_scale) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->scale;
}

static MFUN(dtrig_set_scale) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  m_float scale = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->scale = scale);
}

typedef struct {
  sp_data* sp;
  sp_dust* osc;
} GW_dust;

static TICK(dust_tick) {
  const GW_dust* ug = (GW_dust*)u->module.gen.data;
  sp_dust_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(dust_dtor) {
  GW_dust* ug = UGEN(o)->module.gen.data;
  sp_dust_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_dust), ug);
}

static MFUN(dust_init) {
  GW_dust* ug = mp_malloc2(shred->info->mp, sizeof(GW_dust));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dust_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_dust_create(&ug->osc) == SP_NOT_OK || sp_dust_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(dust_get_amp) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(dust_set_amp) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(dust_get_density) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->density;
}

static MFUN(dust_set_density) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  m_float density = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->density = density);
}

static MFUN(dust_get_bipolar) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->bipolar;
}

static MFUN(dust_set_bipolar) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  m_int bipolar = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->bipolar = bipolar);
}

typedef struct {
  sp_data* sp;
  sp_eqfil* osc;
} GW_eqfil;

static TICK(eqfil_tick) {
  const GW_eqfil* ug = (GW_eqfil*)u->module.gen.data;
  sp_eqfil_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(eqfil_dtor) {
  GW_eqfil* ug = UGEN(o)->module.gen.data;
  sp_eqfil_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_eqfil), ug);
}

static MFUN(eqfil_init) {
  GW_eqfil* ug = mp_malloc2(shred->info->mp, sizeof(GW_eqfil));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), eqfil_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_eqfil_create(&ug->osc) == SP_NOT_OK || sp_eqfil_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(eqfil_get_freq) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(eqfil_set_freq) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(eqfil_get_bw) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bw;
}

static MFUN(eqfil_set_bw) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  m_float bw = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bw = bw);
}

static MFUN(eqfil_get_gain) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->gain;
}

static MFUN(eqfil_set_gain) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  m_float gain = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->gain = gain);
}

typedef struct {
  sp_data* sp;
  sp_expon* osc;
} GW_expon;

static TICK(expon_tick) {
  const GW_expon* ug = (GW_expon*)u->module.gen.data;
  sp_expon_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(expon_dtor) {
  GW_expon* ug = UGEN(o)->module.gen.data;
  sp_expon_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_expon), ug);
}

static MFUN(expon_init) {
  GW_expon* ug = mp_malloc2(shred->info->mp, sizeof(GW_expon));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), expon_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_expon_create(&ug->osc) == SP_NOT_OK || sp_expon_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(expon_get_a) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a;
}

static MFUN(expon_set_a) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  m_float a = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->a = a);
}

static MFUN(expon_get_dur) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(expon_set_dur) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dur = dur);
}

static MFUN(expon_get_b) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b;
}

static MFUN(expon_set_b) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  m_float b = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->b = b);
}

typedef struct {
  sp_data* sp;
  sp_fof* osc;
  M_Object sine_obj;

  M_Object win_obj;

} GW_fof;

static TICK(fof_tick) {
  const GW_fof* ug = (GW_fof*)u->module.gen.data;
  sp_fof_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(fof_dtor) {
  GW_fof* ug = UGEN(o)->module.gen.data;
  release(ug->sine_obj, shred);

  release(ug->win_obj, shred);

  sp_fof_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_fof), ug);
}

static MFUN(fof_init) {
  GW_fof* ug = mp_malloc2(shred->info->mp, sizeof(GW_fof));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fof_tick, ug, 0);
  const M_Object sine_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* sine = FTBL(sine_obj);
  ++sine_obj->ref;
  const M_Object win_obj = *(M_Object*)(shred->mem+SZ_INT+SZ_INT);
  sp_ftbl* win = FTBL(win_obj);
  ++win_obj->ref;
  m_int iolaps = *(m_int*)(shred->mem+SZ_INT+SZ_INT+SZ_INT);
  m_float iphs = *(m_float*)(shred->mem+SZ_INT+SZ_INT+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_fof_create(&ug->osc) == SP_NOT_OK || sp_fof_init(ug->sp, ug->osc , sine, win, iolaps, iphs) == SP_NOT_OK) {
    release(sine_obj, shred); // LCOV_EXCL_LINE
    release(win_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->sine_obj = sine_obj;
  ug->win_obj = win_obj;
}

static MFUN(fof_get_amp) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(fof_set_amp) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(fof_get_fund) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->fund;
}

static MFUN(fof_set_fund) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float fund = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->fund = fund);
}

static MFUN(fof_get_form) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->form;
}

static MFUN(fof_set_form) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float form = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->form = form);
}

static MFUN(fof_get_oct) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->oct;
}

static MFUN(fof_set_oct) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float oct = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->oct = oct);
}

static MFUN(fof_get_band) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->band;
}

static MFUN(fof_set_band) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float band = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->band = band);
}

static MFUN(fof_get_ris) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->ris;
}

static MFUN(fof_set_ris) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float ris = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->ris = ris);
}

static MFUN(fof_get_dec) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(fof_set_dec) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

static MFUN(fof_get_dur) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(fof_set_dur) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dur = dur);
}

typedef struct {
  sp_data* sp;
  sp_fofilt* osc;
} GW_fofilt;

static TICK(fofilt_tick) {
  const GW_fofilt* ug = (GW_fofilt*)u->module.gen.data;
  sp_fofilt_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(fofilt_dtor) {
  GW_fofilt* ug = UGEN(o)->module.gen.data;
  sp_fofilt_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_fofilt), ug);
}

static MFUN(fofilt_init) {
  GW_fofilt* ug = mp_malloc2(shred->info->mp, sizeof(GW_fofilt));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fofilt_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_fofilt_create(&ug->osc) == SP_NOT_OK || sp_fofilt_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(fofilt_get_freq) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(fofilt_set_freq) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(fofilt_get_atk) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(fofilt_set_atk) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(fofilt_get_dec) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(fofilt_set_dec) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

typedef struct {
  sp_data* sp;
  sp_fog* osc;
  M_Object wav_obj;

  M_Object win_obj;

} GW_fog;

static TICK(fog_tick) {
  const GW_fog* ug = (GW_fog*)u->module.gen.data;
  sp_fog_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(fog_dtor) {
  GW_fog* ug = UGEN(o)->module.gen.data;
  release(ug->wav_obj, shred);

  release(ug->win_obj, shred);

  sp_fog_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_fog), ug);
}

static MFUN(fog_init) {
  GW_fog* ug = mp_malloc2(shred->info->mp, sizeof(GW_fog));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fog_tick, ug, 0);
  const M_Object wav_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* wav = FTBL(wav_obj);
  ++wav_obj->ref;
  const M_Object win_obj = *(M_Object*)(shred->mem+SZ_INT+SZ_INT);
  sp_ftbl* win = FTBL(win_obj);
  ++win_obj->ref;
  m_int iolaps = *(m_int*)(shred->mem+SZ_INT+SZ_INT+SZ_INT);
  m_float iphs = *(m_float*)(shred->mem+SZ_INT+SZ_INT+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_fog_create(&ug->osc) == SP_NOT_OK || sp_fog_init(ug->sp, ug->osc , wav, win, iolaps, iphs) == SP_NOT_OK) {
    release(wav_obj, shred); // LCOV_EXCL_LINE
    release(win_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->wav_obj = wav_obj;
  ug->win_obj = win_obj;
}

static MFUN(fog_get_amp) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(fog_set_amp) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(fog_get_dens) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dens;
}

static MFUN(fog_set_dens) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float dens = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dens = dens);
}

static MFUN(fog_get_trans) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->trans;
}

static MFUN(fog_set_trans) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float trans = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->trans = trans);
}

static MFUN(fog_get_spd) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->spd;
}

static MFUN(fog_set_spd) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float spd = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->spd = spd);
}

static MFUN(fog_get_oct) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->oct;
}

static MFUN(fog_set_oct) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float oct = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->oct = oct);
}

static MFUN(fog_get_band) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->band;
}

static MFUN(fog_set_band) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float band = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->band = band);
}

static MFUN(fog_get_ris) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->ris;
}

static MFUN(fog_set_ris) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float ris = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->ris = ris);
}

static MFUN(fog_get_dec) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(fog_set_dec) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

static MFUN(fog_get_dur) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(fog_set_dur) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dur = dur);
}

typedef struct {
  sp_data* sp;
  sp_fold* osc;
} GW_fold;

static TICK(fold_tick) {
  const GW_fold* ug = (GW_fold*)u->module.gen.data;
  sp_fold_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(fold_dtor) {
  GW_fold* ug = UGEN(o)->module.gen.data;
  sp_fold_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_fold), ug);
}

static MFUN(fold_init) {
  GW_fold* ug = mp_malloc2(shred->info->mp, sizeof(GW_fold));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fold_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_fold_create(&ug->osc) == SP_NOT_OK || sp_fold_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(fold_get_incr) {
  const GW_fold* ug = (GW_fold*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->incr;
}

static MFUN(fold_set_incr) {
  const GW_fold* ug = (GW_fold*)UGEN(o)->module.gen.data;
  m_float incr = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->incr = incr);
}

typedef struct {
  sp_data* sp;
  sp_fosc* osc;
  M_Object tbl_obj;

} GW_fosc;

static TICK(fosc_tick) {
  const GW_fosc* ug = (GW_fosc*)u->module.gen.data;
  sp_fosc_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(fosc_dtor) {
  GW_fosc* ug = UGEN(o)->module.gen.data;
  release(ug->tbl_obj, shred);

  sp_fosc_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_fosc), ug);
}

static MFUN(fosc_init) {
  GW_fosc* ug = mp_malloc2(shred->info->mp, sizeof(GW_fosc));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fosc_tick, ug, 0);
  const M_Object tbl_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* tbl = FTBL(tbl_obj);
  ++tbl_obj->ref;
  *(M_Object*)RETURN = o;
  if(sp_fosc_create(&ug->osc) == SP_NOT_OK || sp_fosc_init(ug->sp, ug->osc , tbl) == SP_NOT_OK) {
    release(tbl_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->tbl_obj = tbl_obj;
}

static MFUN(fosc_get_freq) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(fosc_set_freq) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(fosc_get_amp) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(fosc_set_amp) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(fosc_get_car) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->car;
}

static MFUN(fosc_set_car) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float car = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->car = car);
}

static MFUN(fosc_get_mod) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mod;
}

static MFUN(fosc_set_mod) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float mod = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->mod = mod);
}

static MFUN(fosc_get_indx) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->indx;
}

static MFUN(fosc_set_indx) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float indx = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->indx = indx);
}

typedef struct {
  sp_data* sp;
  sp_gbuzz* osc;
  M_Object ft_obj;

} GW_gbuzz;

static TICK(gbuzz_tick) {
  const GW_gbuzz* ug = (GW_gbuzz*)u->module.gen.data;
  sp_gbuzz_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(gbuzz_dtor) {
  GW_gbuzz* ug = UGEN(o)->module.gen.data;
  release(ug->ft_obj, shred);

  sp_gbuzz_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_gbuzz), ug);
}

static MFUN(gbuzz_init) {
  GW_gbuzz* ug = mp_malloc2(shred->info->mp, sizeof(GW_gbuzz));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), gbuzz_tick, ug, 0);
  const M_Object ft_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* ft = FTBL(ft_obj);
  ++ft_obj->ref;
  m_float iphs = *(m_float*)(shred->mem+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_gbuzz_create(&ug->osc) == SP_NOT_OK || sp_gbuzz_init(ug->sp, ug->osc , ft, iphs) == SP_NOT_OK) {
    release(ft_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->ft_obj = ft_obj;
}

static MFUN(gbuzz_get_freq) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(gbuzz_set_freq) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(gbuzz_get_amp) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(gbuzz_set_amp) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(gbuzz_get_nharm) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->nharm;
}

static MFUN(gbuzz_set_nharm) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float nharm = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->nharm = nharm);
}

static MFUN(gbuzz_get_lharm) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->lharm;
}

static MFUN(gbuzz_set_lharm) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float lharm = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->lharm = lharm);
}

static MFUN(gbuzz_get_mul) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mul;
}

static MFUN(gbuzz_set_mul) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float mul = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->mul = mul);
}

static MFUN(ftbl_gen_composite) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem+SZ_INT*2);
  m_str argstring = STRING(argstring_obj);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_composite(sp, ftbl, argstring);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_file) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object filename_obj = *(M_Object*)(shred->mem+SZ_INT*2);
  m_str filename = STRING(filename_obj);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_file(sp, ftbl, filename);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_gauss) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  m_float scale = *(m_float*)(shred->mem+SZ_INT*2);
  m_int seed = *(m_int*)(shred->mem+SZ_INT*2+SZ_FLOAT);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_gauss(sp, ftbl, scale, seed);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_line) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem+SZ_INT*2);
  m_str argstring = STRING(argstring_obj);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_line(sp, ftbl, argstring);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_padsynth) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  const M_Object amps_obj = *(M_Object*)(shred->mem+SZ_INT*2);
  sp_ftbl* amps = FTBL(amps_obj);
  ++amps_obj->ref;
  m_float f = *(m_float*)(shred->mem+SZ_INT*2+SZ_INT);
  m_float bw = *(m_float*)(shred->mem+SZ_INT*2+SZ_INT+SZ_FLOAT);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_padsynth(sp, ftbl, amps, f, bw);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_rand) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem+SZ_INT*2);
  m_str argstring = STRING(argstring_obj);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_rand(sp, ftbl, argstring);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_scrambler) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object dest_obj = *(M_Object*)(shred->mem+SZ_INT*2);
  sp_ftbl** dest = &FTBL(dest_obj);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_scrambler(sp, ftbl, dest);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_sine) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_sine(sp, ftbl);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_sinesum) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem+SZ_INT*2);
  m_str argstring = STRING(argstring_obj);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_sinesum(sp, ftbl, argstring);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_triangle) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_triangle(sp, ftbl);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_xline) {
  sp_ftbl* ftbl = FTBL(o);
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem+SZ_INT*2);
  m_str argstring = STRING(argstring_obj);
  CHECK_SIZE(size);
  sp_data *sp = get_module(shred->info->vm->gwion, "Soundpipe");
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_xline(sp, ftbl, argstring);
  FTBL(o) = ftbl;
}

typedef struct {
  sp_data* sp;
  sp_hilbert* osc;
} GW_hilbert;

static TICK(hilbert_tick) {
  const GW_hilbert* ug = (GW_hilbert*)u->module.gen.data;
  sp_hilbert_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
}

static DTOR(hilbert_dtor) {
  GW_hilbert* ug = UGEN(o)->module.gen.data;
  sp_hilbert_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_hilbert), ug);
}

static MFUN(hilbert_init) {
  GW_hilbert* ug = mp_malloc2(shred->info->mp, sizeof(GW_hilbert));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), hilbert_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_hilbert_create(&ug->osc) == SP_NOT_OK || sp_hilbert_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_in* osc;
} GW_in;

static TICK(in_tick) {
  const GW_in* ug = (GW_in*)u->module.gen.data;
  sp_in_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(in_dtor) {
  GW_in* ug = UGEN(o)->module.gen.data;
  sp_in_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_in), ug);
}

static MFUN(in_init) {
  GW_in* ug = mp_malloc2(shred->info->mp, sizeof(GW_in));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), in_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_in_create(&ug->osc) == SP_NOT_OK || sp_in_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_incr* osc;
} GW_incr;

static TICK(incr_tick) {
  const GW_incr* ug = (GW_incr*)u->module.gen.data;
  sp_incr_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(incr_dtor) {
  GW_incr* ug = UGEN(o)->module.gen.data;
  sp_incr_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_incr), ug);
}

static MFUN(incr_init) {
  GW_incr* ug = mp_malloc2(shred->info->mp, sizeof(GW_incr));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), incr_tick, ug, 1);
  m_float val = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_incr_create(&ug->osc) == SP_NOT_OK || sp_incr_init(ug->sp, ug->osc , val) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(incr_get_step) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->step;
}

static MFUN(incr_set_step) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  m_float step = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->step = step);
}

static MFUN(incr_get_min) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(incr_set_min) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(incr_get_max) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(incr_set_max) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
  sp_data* sp;
  sp_jcrev* osc;
} GW_jcrev;

static TICK(jcrev_tick) {
  const GW_jcrev* ug = (GW_jcrev*)u->module.gen.data;
  sp_jcrev_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(jcrev_dtor) {
  GW_jcrev* ug = UGEN(o)->module.gen.data;
  sp_jcrev_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_jcrev), ug);
}

static MFUN(jcrev_init) {
  GW_jcrev* ug = mp_malloc2(shred->info->mp, sizeof(GW_jcrev));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), jcrev_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_jcrev_create(&ug->osc) == SP_NOT_OK || sp_jcrev_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_jitter* osc;
} GW_jitter;

static TICK(jitter_tick) {
  const GW_jitter* ug = (GW_jitter*)u->module.gen.data;
  sp_jitter_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(jitter_dtor) {
  GW_jitter* ug = UGEN(o)->module.gen.data;
  sp_jitter_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_jitter), ug);
}

static MFUN(jitter_init) {
  GW_jitter* ug = mp_malloc2(shred->info->mp, sizeof(GW_jitter));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), jitter_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_jitter_create(&ug->osc) == SP_NOT_OK || sp_jitter_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(jitter_get_amp) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(jitter_set_amp) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(jitter_get_cpsMin) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cpsMin;
}

static MFUN(jitter_set_cpsMin) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  m_float cpsMin = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->cpsMin = cpsMin);
}

static MFUN(jitter_get_cpsMax) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cpsMax;
}

static MFUN(jitter_set_cpsMax) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  m_float cpsMax = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->cpsMax = cpsMax);
}

typedef struct {
  sp_data* sp;
  sp_line* osc;
} GW_line;

static TICK(line_tick) {
  const GW_line* ug = (GW_line*)u->module.gen.data;
  sp_line_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(line_dtor) {
  GW_line* ug = UGEN(o)->module.gen.data;
  sp_line_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_line), ug);
}

static MFUN(line_init) {
  GW_line* ug = mp_malloc2(shred->info->mp, sizeof(GW_line));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), line_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_line_create(&ug->osc) == SP_NOT_OK || sp_line_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(line_get_a) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a;
}

static MFUN(line_set_a) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  m_float a = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->a = a);
}

static MFUN(line_get_dur) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(line_set_dur) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dur = dur);
}

static MFUN(line_get_b) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b;
}

static MFUN(line_set_b) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  m_float b = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->b = b);
}

typedef struct {
  sp_data* sp;
  sp_lpc* osc;
} GW_lpc;

static TICK(lpc_tick) {
  const GW_lpc* ug = (GW_lpc*)u->module.gen.data;
  sp_lpc_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(lpc_dtor) {
  GW_lpc* ug = UGEN(o)->module.gen.data;
  sp_lpc_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_lpc), ug);
}

static MFUN(lpc_init) {
  GW_lpc* ug = mp_malloc2(shred->info->mp, sizeof(GW_lpc));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), lpc_tick, ug, 0);
  m_int framesize = *(m_int*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_lpc_create(&ug->osc) == SP_NOT_OK || sp_lpc_init(ug->sp, ug->osc , framesize) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_lpf18* osc;
} GW_lpf18;

static TICK(lpf18_tick) {
  const GW_lpf18* ug = (GW_lpf18*)u->module.gen.data;
  sp_lpf18_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(lpf18_dtor) {
  GW_lpf18* ug = UGEN(o)->module.gen.data;
  sp_lpf18_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_lpf18), ug);
}

static MFUN(lpf18_init) {
  GW_lpf18* ug = mp_malloc2(shred->info->mp, sizeof(GW_lpf18));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), lpf18_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_lpf18_create(&ug->osc) == SP_NOT_OK || sp_lpf18_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(lpf18_get_cutoff) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cutoff;
}

static MFUN(lpf18_set_cutoff) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  m_float cutoff = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->cutoff = cutoff);
}

static MFUN(lpf18_get_res) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(lpf18_set_res) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->res = res);
}

static MFUN(lpf18_get_dist) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dist;
}

static MFUN(lpf18_set_dist) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  m_float dist = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dist = dist);
}

typedef struct {
  sp_data* sp;
  sp_maygate* osc;
} GW_maygate;

static TICK(maygate_tick) {
  const GW_maygate* ug = (GW_maygate*)u->module.gen.data;
  sp_maygate_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(maygate_dtor) {
  GW_maygate* ug = UGEN(o)->module.gen.data;
  sp_maygate_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_maygate), ug);
}

static MFUN(maygate_init) {
  GW_maygate* ug = mp_malloc2(shred->info->mp, sizeof(GW_maygate));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), maygate_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_maygate_create(&ug->osc) == SP_NOT_OK || sp_maygate_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(maygate_get_prob) {
  const GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->prob;
}

static MFUN(maygate_set_prob) {
  const GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
  m_float prob = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->prob = prob);
}

static MFUN(maygate_get_mode) {
  const GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->mode;
}

static MFUN(maygate_set_mode) {
  const GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
  m_int mode = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
  sp_data* sp;
  sp_metro* osc;
} GW_metro;

static TICK(metro_tick) {
  const GW_metro* ug = (GW_metro*)u->module.gen.data;
  sp_metro_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(metro_dtor) {
  GW_metro* ug = UGEN(o)->module.gen.data;
  sp_metro_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_metro), ug);
}

static MFUN(metro_init) {
  GW_metro* ug = mp_malloc2(shred->info->mp, sizeof(GW_metro));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), metro_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_metro_create(&ug->osc) == SP_NOT_OK || sp_metro_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(metro_get_freq) {
  const GW_metro* ug = (GW_metro*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(metro_set_freq) {
  const GW_metro* ug = (GW_metro*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
  sp_data* sp;
  sp_mincer* osc;
  M_Object ft_obj;

} GW_mincer;

static TICK(mincer_tick) {
  const GW_mincer* ug = (GW_mincer*)u->module.gen.data;
  sp_mincer_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(mincer_dtor) {
  GW_mincer* ug = UGEN(o)->module.gen.data;
  release(ug->ft_obj, shred);

  sp_mincer_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_mincer), ug);
}

static MFUN(mincer_init) {
  GW_mincer* ug = mp_malloc2(shred->info->mp, sizeof(GW_mincer));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), mincer_tick, ug, 0);
  const M_Object ft_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* ft = FTBL(ft_obj);
  ++ft_obj->ref;
  m_int winsize = *(m_int*)(shred->mem+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_mincer_create(&ug->osc) == SP_NOT_OK || sp_mincer_init(ug->sp, ug->osc , ft, winsize) == SP_NOT_OK) {
    release(ft_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->ft_obj = ft_obj;
}

static MFUN(mincer_get_time) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->time;
}

static MFUN(mincer_set_time) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  m_float time = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->time = time);
}

static MFUN(mincer_get_amp) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(mincer_set_amp) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(mincer_get_pitch) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pitch;
}

static MFUN(mincer_set_pitch) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  m_float pitch = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->pitch = pitch);
}

typedef struct {
  sp_data* sp;
  sp_mode* osc;
} GW_mode;

static TICK(mode_tick) {
  const GW_mode* ug = (GW_mode*)u->module.gen.data;
  sp_mode_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(mode_dtor) {
  GW_mode* ug = UGEN(o)->module.gen.data;
  sp_mode_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_mode), ug);
}

static MFUN(mode_init) {
  GW_mode* ug = mp_malloc2(shred->info->mp, sizeof(GW_mode));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), mode_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_mode_create(&ug->osc) == SP_NOT_OK || sp_mode_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(mode_get_freq) {
  const GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(mode_set_freq) {
  const GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(mode_get_q) {
  const GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->q;
}

static MFUN(mode_set_q) {
  const GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
  m_float q = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->q = q);
}

typedef struct {
  sp_data* sp;
  sp_moogladder* osc;
} GW_moogladder;

static TICK(moogladder_tick) {
  const GW_moogladder* ug = (GW_moogladder*)u->module.gen.data;
  sp_moogladder_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(moogladder_dtor) {
  GW_moogladder* ug = UGEN(o)->module.gen.data;
  sp_moogladder_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_moogladder), ug);
}

static MFUN(moogladder_init) {
  GW_moogladder* ug = mp_malloc2(shred->info->mp, sizeof(GW_moogladder));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), moogladder_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_moogladder_create(&ug->osc) == SP_NOT_OK || sp_moogladder_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(moogladder_get_freq) {
  const GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(moogladder_set_freq) {
  const GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(moogladder_get_res) {
  const GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(moogladder_set_res) {
  const GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->res = res);
}

typedef struct {
  sp_data* sp;
  sp_noise* osc;
} GW_noise;

static TICK(noise_tick) {
  const GW_noise* ug = (GW_noise*)u->module.gen.data;
  sp_noise_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(noise_dtor) {
  GW_noise* ug = UGEN(o)->module.gen.data;
  sp_noise_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_noise), ug);
}

static MFUN(noise_init) {
  GW_noise* ug = mp_malloc2(shred->info->mp, sizeof(GW_noise));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), noise_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_noise_create(&ug->osc) == SP_NOT_OK || sp_noise_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(noise_get_amp) {
  const GW_noise* ug = (GW_noise*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(noise_set_amp) {
  const GW_noise* ug = (GW_noise*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_nsmp* osc;
} GW_nsmp;

static TICK(nsmp_tick) {
  const GW_nsmp* ug = (GW_nsmp*)u->module.gen.data;
  sp_nsmp_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(nsmp_dtor) {
  GW_nsmp* ug = UGEN(o)->module.gen.data;
  sp_nsmp_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_nsmp), ug);
}

static MFUN(nsmp_init) {
  GW_nsmp* ug = mp_malloc2(shred->info->mp, sizeof(GW_nsmp));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), nsmp_tick, ug, 1);
  const M_Object ft_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* ft = FTBL(ft_obj);
  ++ft_obj->ref;
  m_int sr = *(m_int*)(shred->mem+SZ_INT+SZ_INT);
  M_Object init_obj = *(M_Object*)(shred->mem+SZ_INT+SZ_INT+SZ_INT);
  m_str init = STRING(init_obj);
  *(M_Object*)RETURN = o;
  if(sp_nsmp_create(&ug->osc) == SP_NOT_OK || sp_nsmp_init(ug->sp, ug->osc , ft, sr, init) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(nsmp_get_index) {
  const GW_nsmp* ug = (GW_nsmp*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->index;
}

static MFUN(nsmp_set_index) {
  const GW_nsmp* ug = (GW_nsmp*)UGEN(o)->module.gen.data;
  m_int index = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->index = index);
}

typedef struct {
  sp_data* sp;
  sp_osc* osc;
  M_Object tbl_obj;

} GW_osc;

static TICK(osc_tick) {
  const GW_osc* ug = (GW_osc*)u->module.gen.data;
  sp_osc_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(osc_dtor) {
  GW_osc* ug = UGEN(o)->module.gen.data;
  release(ug->tbl_obj, shred);

  sp_osc_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_osc), ug);
}

static MFUN(osc_init) {
  GW_osc* ug = mp_malloc2(shred->info->mp, sizeof(GW_osc));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), osc_tick, ug, 0);
  const M_Object tbl_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* tbl = FTBL(tbl_obj);
  ++tbl_obj->ref;
  m_float phase = *(m_float*)(shred->mem+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_osc_create(&ug->osc) == SP_NOT_OK || sp_osc_init(ug->sp, ug->osc , tbl, phase) == SP_NOT_OK) {
    release(tbl_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->tbl_obj = tbl_obj;
}

static MFUN(osc_get_freq) {
  const GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(osc_set_freq) {
  const GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(osc_get_amp) {
  const GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(osc_set_amp) {
  const GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_oscmorph* osc;
  sp_ftbl** tbl;

  M_Object tbl_ptr;

} GW_oscmorph;

static TICK(oscmorph_tick) {
  const GW_oscmorph* ug = (GW_oscmorph*)u->module.gen.data;
  sp_oscmorph_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(oscmorph_dtor) {
  GW_oscmorph* ug = UGEN(o)->module.gen.data;
  xfree(ug->osc->tbl);

  release(ug->tbl_ptr, shred);

  sp_oscmorph_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_oscmorph), ug);
}

static MFUN(oscmorph_init) {
  GW_oscmorph* ug = mp_malloc2(shred->info->mp, sizeof(GW_oscmorph));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), oscmorph_tick, ug, 0);
  M_Object tbl_ptr = *(M_Object*)(shred->mem+SZ_INT);
  m_uint tbl_iter;
  sp_ftbl** tbl = (sp_ftbl**)xmalloc(m_vector_size(ARRAY(tbl_ptr)) * SZ_INT);
  for(tbl_iter = 0; tbl_iter < m_vector_size(ARRAY(tbl_ptr)); tbl_iter++) {
      M_Object tbl_ftl_obj;
      m_vector_get(ARRAY(tbl_ptr), tbl_iter, &tbl_ftl_obj);
      tbl[tbl_iter] = FTBL(tbl_ftl_obj);
  }
  ++tbl_ptr->ref;
  m_int nft = *(m_int*)(shred->mem+SZ_INT+SZ_INT);
  m_float phase = *(m_float*)(shred->mem+SZ_INT+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_oscmorph_create(&ug->osc) == SP_NOT_OK || sp_oscmorph_init(ug->sp, ug->osc , tbl, nft, phase) == SP_NOT_OK) {
    xfree(tbl); // LCOV_EXCL_LINE
    release(tbl_ptr, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->tbl = tbl;
  ug->tbl_ptr = tbl_ptr;
}

static MFUN(oscmorph_get_freq) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(oscmorph_set_freq) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(oscmorph_get_amp) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(oscmorph_set_amp) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(oscmorph_get_wtpos) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->wtpos;
}

static MFUN(oscmorph_set_wtpos) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  m_float wtpos = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->wtpos = wtpos);
}

typedef struct {
  sp_data* sp;
  sp_pan2* osc;
} GW_pan2;

static TICK(pan2_tick) {
  const GW_pan2* ug = (GW_pan2*)u->module.gen.data;
  sp_pan2_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
}

static DTOR(pan2_dtor) {
  GW_pan2* ug = UGEN(o)->module.gen.data;
  sp_pan2_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_pan2), ug);
}

static MFUN(pan2_init) {
  GW_pan2* ug = mp_malloc2(shred->info->mp, sizeof(GW_pan2));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pan2_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_pan2_create(&ug->osc) == SP_NOT_OK || sp_pan2_init(ug->sp, ug->osc ) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(pan2_get_type) {
  const GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->type;
}

static MFUN(pan2_set_type) {
  const GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
  m_int type = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->type = type);
}

static MFUN(pan2_get_pan) {
  const GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pan;
}

static MFUN(pan2_set_pan) {
  const GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
  m_float pan = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->pan = pan);
}

typedef struct {
  sp_data* sp;
  sp_panst* osc;
} GW_panst;

static TICK(panst_tick) {
  const GW_panst* ug = (GW_panst*)u->module.gen.data;
  sp_panst_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
}

static DTOR(panst_dtor) {
  GW_panst* ug = UGEN(o)->module.gen.data;
  sp_panst_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_panst), ug);
}

static MFUN(panst_init) {
  GW_panst* ug = mp_malloc2(shred->info->mp, sizeof(GW_panst));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), panst_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_panst_create(&ug->osc) == SP_NOT_OK || sp_panst_init(ug->sp, ug->osc ) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(panst_get_type) {
  const GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->type;
}

static MFUN(panst_set_type) {
  const GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
  m_int type = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->type = type);
}

static MFUN(panst_get_pan) {
  const GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pan;
}

static MFUN(panst_set_pan) {
  const GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
  m_float pan = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->pan = pan);
}

typedef struct {
  sp_data* sp;
  sp_pareq* osc;
} GW_pareq;

static TICK(pareq_tick) {
  const GW_pareq* ug = (GW_pareq*)u->module.gen.data;
  sp_pareq_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(pareq_dtor) {
  GW_pareq* ug = UGEN(o)->module.gen.data;
  sp_pareq_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_pareq), ug);
}

static MFUN(pareq_init) {
  GW_pareq* ug = mp_malloc2(shred->info->mp, sizeof(GW_pareq));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pareq_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_pareq_create(&ug->osc) == SP_NOT_OK || sp_pareq_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(pareq_get_fc) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->fc;
}

static MFUN(pareq_set_fc) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  m_float fc = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->fc = fc);
}

static MFUN(pareq_get_v) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->v;
}

static MFUN(pareq_set_v) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  m_float v = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->v = v);
}

static MFUN(pareq_get_q) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->q;
}

static MFUN(pareq_set_q) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  m_float q = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->q = q);
}

static MFUN(pareq_get_mode) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mode;
}

static MFUN(pareq_set_mode) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  m_float mode = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
  sp_data* sp;
  sp_paulstretch* osc;
  M_Object ft_obj;

} GW_paulstretch;

static TICK(paulstretch_tick) {
  const GW_paulstretch* ug = (GW_paulstretch*)u->module.gen.data;
  sp_paulstretch_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(paulstretch_dtor) {
  GW_paulstretch* ug = UGEN(o)->module.gen.data;
  release(ug->ft_obj, shred);

  sp_paulstretch_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_paulstretch), ug);
}

static MFUN(paulstretch_init) {
  GW_paulstretch* ug = mp_malloc2(shred->info->mp, sizeof(GW_paulstretch));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), paulstretch_tick, ug, 0);
  const M_Object ft_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* ft = FTBL(ft_obj);
  ++ft_obj->ref;
  m_float windowsize = *(m_float*)(shred->mem+SZ_INT+SZ_INT);
  m_float stretch = *(m_float*)(shred->mem+SZ_INT+SZ_INT+SZ_FLOAT);
  *(M_Object*)RETURN = o;
  if(sp_paulstretch_create(&ug->osc) == SP_NOT_OK || sp_paulstretch_init(ug->sp, ug->osc , ft, windowsize, stretch) == SP_NOT_OK) {
    release(ft_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->ft_obj = ft_obj;
}

typedef struct {
  sp_data* sp;
  sp_pdhalf* osc;
} GW_pdhalf;

static TICK(pdhalf_tick) {
  const GW_pdhalf* ug = (GW_pdhalf*)u->module.gen.data;
  sp_pdhalf_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(pdhalf_dtor) {
  GW_pdhalf* ug = UGEN(o)->module.gen.data;
  sp_pdhalf_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_pdhalf), ug);
}

static MFUN(pdhalf_init) {
  GW_pdhalf* ug = mp_malloc2(shred->info->mp, sizeof(GW_pdhalf));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pdhalf_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_pdhalf_create(&ug->osc) == SP_NOT_OK || sp_pdhalf_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(pdhalf_get_amount) {
  const GW_pdhalf* ug = (GW_pdhalf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amount;
}

static MFUN(pdhalf_set_amount) {
  const GW_pdhalf* ug = (GW_pdhalf*)UGEN(o)->module.gen.data;
  m_float amount = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amount = amount);
}

typedef struct {
  sp_data* sp;
  sp_peaklim* osc;
} GW_peaklim;

static TICK(peaklim_tick) {
  const GW_peaklim* ug = (GW_peaklim*)u->module.gen.data;
  sp_peaklim_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(peaklim_dtor) {
  GW_peaklim* ug = UGEN(o)->module.gen.data;
  sp_peaklim_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_peaklim), ug);
}

static MFUN(peaklim_init) {
  GW_peaklim* ug = mp_malloc2(shred->info->mp, sizeof(GW_peaklim));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), peaklim_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_peaklim_create(&ug->osc) == SP_NOT_OK || sp_peaklim_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(peaklim_get_atk) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(peaklim_set_atk) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(peaklim_get_rel) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(peaklim_set_rel) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->rel = rel);
}

static MFUN(peaklim_get_thresh) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->thresh;
}

static MFUN(peaklim_set_thresh) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  m_float thresh = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->thresh = thresh);
}

typedef struct {
  sp_data* sp;
  sp_phaser* osc;
} GW_phaser;

static TICK(phaser_tick) {
  const GW_phaser* ug = (GW_phaser*)u->module.gen.data;
  sp_phaser_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
}

static DTOR(phaser_dtor) {
  GW_phaser* ug = UGEN(o)->module.gen.data;
  sp_phaser_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_phaser), ug);
}

static MFUN(phaser_init) {
  GW_phaser* ug = mp_malloc2(shred->info->mp, sizeof(GW_phaser));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), phaser_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_phaser_create(&ug->osc) == SP_NOT_OK || sp_phaser_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(phaser_get_MaxNotch1Freq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->MaxNotch1Freq;
}

static MFUN(phaser_set_MaxNotch1Freq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float MaxNotch1Freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->MaxNotch1Freq = MaxNotch1Freq);
}

static MFUN(phaser_get_MinNotch1Freq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->MinNotch1Freq;
}

static MFUN(phaser_set_MinNotch1Freq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float MinNotch1Freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->MinNotch1Freq = MinNotch1Freq);
}

static MFUN(phaser_get_Notch_width) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->Notch_width;
}

static MFUN(phaser_set_Notch_width) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float Notch_width = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->Notch_width = Notch_width);
}

static MFUN(phaser_get_NotchFreq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->NotchFreq;
}

static MFUN(phaser_set_NotchFreq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float NotchFreq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->NotchFreq = NotchFreq);
}

static MFUN(phaser_get_VibratoMode) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->VibratoMode;
}

static MFUN(phaser_set_VibratoMode) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float VibratoMode = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->VibratoMode = VibratoMode);
}

static MFUN(phaser_get_depth) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->depth;
}

static MFUN(phaser_set_depth) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float depth = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->depth = depth);
}

static MFUN(phaser_get_feedback_gain) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->feedback_gain;
}

static MFUN(phaser_set_feedback_gain) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float feedback_gain = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->feedback_gain = feedback_gain);
}

static MFUN(phaser_get_invert) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->invert;
}

static MFUN(phaser_set_invert) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float invert = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->invert = invert);
}

static MFUN(phaser_get_level) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->level;
}

static MFUN(phaser_set_level) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float level = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->level = level);
}

static MFUN(phaser_get_lfobpm) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->lfobpm;
}

static MFUN(phaser_set_lfobpm) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float lfobpm = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->lfobpm = lfobpm);
}

typedef struct {
  sp_data* sp;
  sp_phasor* osc;
} GW_phasor;

static TICK(phasor_tick) {
  const GW_phasor* ug = (GW_phasor*)u->module.gen.data;
  sp_phasor_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(phasor_dtor) {
  GW_phasor* ug = UGEN(o)->module.gen.data;
  sp_phasor_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_phasor), ug);
}

static MFUN(phasor_init) {
  GW_phasor* ug = mp_malloc2(shred->info->mp, sizeof(GW_phasor));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), phasor_tick, ug, 0);
  m_float iphs = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_phasor_create(&ug->osc) == SP_NOT_OK || sp_phasor_init(ug->sp, ug->osc , iphs) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(phasor_get_freq) {
  const GW_phasor* ug = (GW_phasor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(phasor_set_freq) {
  const GW_phasor* ug = (GW_phasor*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
  sp_data* sp;
  sp_pinknoise* osc;
} GW_pinknoise;

static TICK(pinknoise_tick) {
  const GW_pinknoise* ug = (GW_pinknoise*)u->module.gen.data;
  sp_pinknoise_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(pinknoise_dtor) {
  GW_pinknoise* ug = UGEN(o)->module.gen.data;
  sp_pinknoise_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_pinknoise), ug);
}

static MFUN(pinknoise_init) {
  GW_pinknoise* ug = mp_malloc2(shred->info->mp, sizeof(GW_pinknoise));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pinknoise_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_pinknoise_create(&ug->osc) == SP_NOT_OK || sp_pinknoise_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(pinknoise_get_amp) {
  const GW_pinknoise* ug = (GW_pinknoise*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(pinknoise_set_amp) {
  const GW_pinknoise* ug = (GW_pinknoise*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_pitchamdf* osc;
} GW_pitchamdf;

static TICK(pitchamdf_tick) {
  const GW_pitchamdf* ug = (GW_pitchamdf*)u->module.gen.data;
  sp_pitchamdf_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
}

static DTOR(pitchamdf_dtor) {
  GW_pitchamdf* ug = UGEN(o)->module.gen.data;
  sp_pitchamdf_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_pitchamdf), ug);
}

static MFUN(pitchamdf_init) {
  GW_pitchamdf* ug = mp_malloc2(shred->info->mp, sizeof(GW_pitchamdf));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pitchamdf_tick, ug, 0);
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  m_float max = *(m_float*)(shred->mem+SZ_INT+SZ_FLOAT);
  *(M_Object*)RETURN = o;
  if(sp_pitchamdf_create(&ug->osc) == SP_NOT_OK || sp_pitchamdf_init(ug->sp, ug->osc , min, max) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_pluck* osc;
} GW_pluck;

static TICK(pluck_tick) {
  const GW_pluck* ug = (GW_pluck*)u->module.gen.data;
  sp_pluck_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(pluck_dtor) {
  GW_pluck* ug = UGEN(o)->module.gen.data;
  sp_pluck_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_pluck), ug);
}

static MFUN(pluck_init) {
  GW_pluck* ug = mp_malloc2(shred->info->mp, sizeof(GW_pluck));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pluck_tick, ug, 1);
  m_float ifreq = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_pluck_create(&ug->osc) == SP_NOT_OK || sp_pluck_init(ug->sp, ug->osc , ifreq) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(pluck_get_freq) {
  const GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(pluck_set_freq) {
  const GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(pluck_get_amp) {
  const GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(pluck_set_amp) {
  const GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_port* osc;
} GW_port;

static TICK(port_tick) {
  const GW_port* ug = (GW_port*)u->module.gen.data;
  sp_port_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(port_dtor) {
  GW_port* ug = UGEN(o)->module.gen.data;
  sp_port_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_port), ug);
}

static MFUN(port_init) {
  GW_port* ug = mp_malloc2(shred->info->mp, sizeof(GW_port));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), port_tick, ug, 0);
  m_float htime = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_port_create(&ug->osc) == SP_NOT_OK || sp_port_init(ug->sp, ug->osc , htime) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(port_get_htime) {
  const GW_port* ug = (GW_port*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->htime;
}

static MFUN(port_set_htime) {
  const GW_port* ug = (GW_port*)UGEN(o)->module.gen.data;
  m_float htime = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->htime = htime);
}

typedef struct {
  sp_data* sp;
  sp_posc3* osc;
  M_Object tbl_obj;

} GW_posc3;

static TICK(posc3_tick) {
  const GW_posc3* ug = (GW_posc3*)u->module.gen.data;
  sp_posc3_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(posc3_dtor) {
  GW_posc3* ug = UGEN(o)->module.gen.data;
  release(ug->tbl_obj, shred);

  sp_posc3_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_posc3), ug);
}

static MFUN(posc3_init) {
  GW_posc3* ug = mp_malloc2(shred->info->mp, sizeof(GW_posc3));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), posc3_tick, ug, 0);
  const M_Object tbl_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* tbl = FTBL(tbl_obj);
  ++tbl_obj->ref;
  *(M_Object*)RETURN = o;
  if(sp_posc3_create(&ug->osc) == SP_NOT_OK || sp_posc3_init(ug->sp, ug->osc , tbl) == SP_NOT_OK) {
    release(tbl_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->tbl_obj = tbl_obj;
}

static MFUN(posc3_get_freq) {
  const GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(posc3_set_freq) {
  const GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(posc3_get_amp) {
  const GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(posc3_set_amp) {
  const GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_progress* osc;
} GW_progress;

static TICK(progress_tick) {
  const GW_progress* ug = (GW_progress*)u->module.gen.data;
  sp_progress_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(progress_dtor) {
  GW_progress* ug = UGEN(o)->module.gen.data;
  sp_progress_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_progress), ug);
}

static MFUN(progress_init) {
  GW_progress* ug = mp_malloc2(shred->info->mp, sizeof(GW_progress));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), progress_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_progress_create(&ug->osc) == SP_NOT_OK || sp_progress_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(progress_get_nbars) {
  const GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->nbars;
}

static MFUN(progress_set_nbars) {
  const GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
  m_int nbars = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->nbars = nbars);
}

static MFUN(progress_get_skip) {
  const GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->skip;
}

static MFUN(progress_set_skip) {
  const GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
  m_int skip = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->skip = skip);
}

typedef struct {
  sp_data* sp;
  sp_prop* osc;
} GW_prop;

static TICK(prop_tick) {
  const GW_prop* ug = (GW_prop*)u->module.gen.data;
  sp_prop_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(prop_dtor) {
  GW_prop* ug = UGEN(o)->module.gen.data;
  sp_prop_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_prop), ug);
}

static MFUN(prop_init) {
  GW_prop* ug = mp_malloc2(shred->info->mp, sizeof(GW_prop));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), prop_tick, ug, 0);
  M_Object str_obj = *(M_Object*)(shred->mem+SZ_INT);
  m_str str = STRING(str_obj);
  *(M_Object*)RETURN = o;
  if(sp_prop_create(&ug->osc) == SP_NOT_OK || sp_prop_init(ug->sp, ug->osc , str) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(prop_get_bpm) {
  const GW_prop* ug = (GW_prop*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bpm;
}

static MFUN(prop_set_bpm) {
  const GW_prop* ug = (GW_prop*)UGEN(o)->module.gen.data;
  m_float bpm = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bpm = bpm);
}

typedef struct {
  sp_data* sp;
  sp_pshift* osc;
} GW_pshift;

static TICK(pshift_tick) {
  const GW_pshift* ug = (GW_pshift*)u->module.gen.data;
  sp_pshift_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(pshift_dtor) {
  GW_pshift* ug = UGEN(o)->module.gen.data;
  sp_pshift_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_pshift), ug);
}

static MFUN(pshift_init) {
  GW_pshift* ug = mp_malloc2(shred->info->mp, sizeof(GW_pshift));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pshift_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_pshift_create(&ug->osc) == SP_NOT_OK || sp_pshift_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(pshift_get_shift) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->shift;
}

static MFUN(pshift_set_shift) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  m_float shift = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->shift = shift);
}

static MFUN(pshift_get_window) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->window;
}

static MFUN(pshift_set_window) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  m_float window = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->window = window);
}

static MFUN(pshift_get_xfade) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->xfade;
}

static MFUN(pshift_set_xfade) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  m_float xfade = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->xfade = xfade);
}

typedef struct {
  sp_data* sp;
  sp_ptrack* osc;
} GW_ptrack;

static TICK(ptrack_tick) {
  const GW_ptrack* ug = (GW_ptrack*)u->module.gen.data;
  sp_ptrack_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
}

static DTOR(ptrack_dtor) {
  GW_ptrack* ug = UGEN(o)->module.gen.data;
  sp_ptrack_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_ptrack), ug);
}

static MFUN(ptrack_init) {
  GW_ptrack* ug = mp_malloc2(shred->info->mp, sizeof(GW_ptrack));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), ptrack_tick, ug, 0);
  m_int ihopsize = *(m_int*)(shred->mem+SZ_INT);
  m_int ipeaks = *(m_int*)(shred->mem+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_ptrack_create(&ug->osc) == SP_NOT_OK || sp_ptrack_init(ug->sp, ug->osc , ihopsize, ipeaks) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_randh* osc;
} GW_randh;

static TICK(randh_tick) {
  const GW_randh* ug = (GW_randh*)u->module.gen.data;
  sp_randh_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(randh_dtor) {
  GW_randh* ug = UGEN(o)->module.gen.data;
  sp_randh_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_randh), ug);
}

static MFUN(randh_init) {
  GW_randh* ug = mp_malloc2(shred->info->mp, sizeof(GW_randh));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), randh_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_randh_create(&ug->osc) == SP_NOT_OK || sp_randh_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(randh_get_min) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(randh_set_min) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(randh_get_max) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(randh_set_max) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->max = max);
}

static MFUN(randh_get_freq) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(randh_set_freq) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
  sp_data* sp;
  sp_randi* osc;
} GW_randi;

static TICK(randi_tick) {
  const GW_randi* ug = (GW_randi*)u->module.gen.data;
  sp_randi_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(randi_dtor) {
  GW_randi* ug = UGEN(o)->module.gen.data;
  sp_randi_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_randi), ug);
}

static MFUN(randi_init) {
  GW_randi* ug = mp_malloc2(shred->info->mp, sizeof(GW_randi));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), randi_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_randi_create(&ug->osc) == SP_NOT_OK || sp_randi_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(randi_get_min) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(randi_set_min) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(randi_get_max) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(randi_set_max) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->max = max);
}

static MFUN(randi_get_cps) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cps;
}

static MFUN(randi_set_cps) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  m_float cps = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->cps = cps);
}

static MFUN(randi_get_mode) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mode;
}

static MFUN(randi_set_mode) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  m_float mode = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
  sp_data* sp;
  sp_random* osc;
} GW_random;

static TICK(random_tick) {
  const GW_random* ug = (GW_random*)u->module.gen.data;
  sp_random_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(random_dtor) {
  GW_random* ug = UGEN(o)->module.gen.data;
  sp_random_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_random), ug);
}

static MFUN(random_init) {
  GW_random* ug = mp_malloc2(shred->info->mp, sizeof(GW_random));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), random_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_random_create(&ug->osc) == SP_NOT_OK || sp_random_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(random_get_min) {
  const GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(random_set_min) {
  const GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(random_get_max) {
  const GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(random_set_max) {
  const GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
  sp_data* sp;
  sp_reson* osc;
} GW_reson;

static TICK(reson_tick) {
  const GW_reson* ug = (GW_reson*)u->module.gen.data;
  sp_reson_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(reson_dtor) {
  GW_reson* ug = UGEN(o)->module.gen.data;
  sp_reson_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_reson), ug);
}

static MFUN(reson_init) {
  GW_reson* ug = mp_malloc2(shred->info->mp, sizeof(GW_reson));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), reson_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_reson_create(&ug->osc) == SP_NOT_OK || sp_reson_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(reson_get_freq) {
  const GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(reson_set_freq) {
  const GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(reson_get_bw) {
  const GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bw;
}

static MFUN(reson_set_bw) {
  const GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
  m_float bw = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->bw = bw);
}

typedef struct {
  sp_data* sp;
  sp_reverse* osc;
} GW_reverse;

static TICK(reverse_tick) {
  const GW_reverse* ug = (GW_reverse*)u->module.gen.data;
  sp_reverse_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(reverse_dtor) {
  GW_reverse* ug = UGEN(o)->module.gen.data;
  sp_reverse_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_reverse), ug);
}

static MFUN(reverse_init) {
  GW_reverse* ug = mp_malloc2(shred->info->mp, sizeof(GW_reverse));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), reverse_tick, ug, 0);
  m_float delay = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_reverse_create(&ug->osc) == SP_NOT_OK || sp_reverse_init(ug->sp, ug->osc , delay) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_revsc* osc;
} GW_revsc;

static TICK(revsc_tick) {
  const GW_revsc* ug = (GW_revsc*)u->module.gen.data;
  sp_revsc_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
}

static DTOR(revsc_dtor) {
  GW_revsc* ug = UGEN(o)->module.gen.data;
  sp_revsc_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_revsc), ug);
}

static MFUN(revsc_init) {
  GW_revsc* ug = mp_malloc2(shred->info->mp, sizeof(GW_revsc));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), revsc_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_revsc_create(&ug->osc) == SP_NOT_OK || sp_revsc_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(revsc_get_feedback) {
  const GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->feedback;
}

static MFUN(revsc_set_feedback) {
  const GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
  m_float feedback = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->feedback = feedback);
}

static MFUN(revsc_get_lpfreq) {
  const GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->lpfreq;
}

static MFUN(revsc_set_lpfreq) {
  const GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
  m_float lpfreq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->lpfreq = lpfreq);
}

typedef struct {
  sp_data* sp;
  sp_rms* osc;
} GW_rms;

static TICK(rms_tick) {
  const GW_rms* ug = (GW_rms*)u->module.gen.data;
  sp_rms_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(rms_dtor) {
  GW_rms* ug = UGEN(o)->module.gen.data;
  sp_rms_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_rms), ug);
}

static MFUN(rms_init) {
  GW_rms* ug = mp_malloc2(shred->info->mp, sizeof(GW_rms));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), rms_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_rms_create(&ug->osc) == SP_NOT_OK || sp_rms_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(rms_get_ihp) {
  const GW_rms* ug = (GW_rms*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->ihp;
}

static MFUN(rms_set_ihp) {
  const GW_rms* ug = (GW_rms*)UGEN(o)->module.gen.data;
  m_float ihp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->ihp = ihp);
}

typedef struct {
  sp_data* sp;
  sp_rpt* osc;
} GW_rpt;

static TICK(rpt_tick) {
  const GW_rpt* ug = (GW_rpt*)u->module.gen.data;
  sp_rpt_compute(ug->sp, ug->osc, &u->in, &u->module.gen.trig->in, &u->out);
}

static DTOR(rpt_dtor) {
  GW_rpt* ug = UGEN(o)->module.gen.data;
  sp_rpt_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_rpt), ug);
}

static MFUN(rpt_init) {
  GW_rpt* ug = mp_malloc2(shred->info->mp, sizeof(GW_rpt));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), rpt_tick, ug, 1);
  m_float maxdur = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_rpt_create(&ug->osc) == SP_NOT_OK || sp_rpt_init(ug->sp, ug->osc , maxdur) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_rspline* osc;
} GW_rspline;

static TICK(rspline_tick) {
  const GW_rspline* ug = (GW_rspline*)u->module.gen.data;
  sp_rspline_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(rspline_dtor) {
  GW_rspline* ug = UGEN(o)->module.gen.data;
  sp_rspline_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_rspline), ug);
}

static MFUN(rspline_init) {
  GW_rspline* ug = mp_malloc2(shred->info->mp, sizeof(GW_rspline));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), rspline_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_rspline_create(&ug->osc) == SP_NOT_OK || sp_rspline_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(rspline_get_min) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(rspline_set_min) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(rspline_get_max) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(rspline_set_max) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->max = max);
}

static MFUN(rspline_get_cps_min) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cps_min;
}

static MFUN(rspline_set_cps_min) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  m_float cps_min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->cps_min = cps_min);
}

static MFUN(rspline_get_cps_max) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cps_max;
}

static MFUN(rspline_set_cps_max) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  m_float cps_max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->cps_max = cps_max);
}

typedef struct {
  sp_data* sp;
  sp_samphold* osc;
} GW_samphold;

static TICK(samphold_tick) {
  const GW_samphold* ug = (GW_samphold*)u->module.gen.data;
  sp_samphold_compute(ug->sp, ug->osc, &u->in, &u->module.gen.trig->in, &u->out);
}

static DTOR(samphold_dtor) {
  GW_samphold* ug = UGEN(o)->module.gen.data;
  sp_samphold_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_samphold), ug);
}

static MFUN(samphold_init) {
  GW_samphold* ug = mp_malloc2(shred->info->mp, sizeof(GW_samphold));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), samphold_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_samphold_create(&ug->osc) == SP_NOT_OK || sp_samphold_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_saturator* osc;
} GW_saturator;

static TICK(saturator_tick) {
  const GW_saturator* ug = (GW_saturator*)u->module.gen.data;
  sp_saturator_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(saturator_dtor) {
  GW_saturator* ug = UGEN(o)->module.gen.data;
  sp_saturator_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_saturator), ug);
}

static MFUN(saturator_init) {
  GW_saturator* ug = mp_malloc2(shred->info->mp, sizeof(GW_saturator));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), saturator_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_saturator_create(&ug->osc) == SP_NOT_OK || sp_saturator_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(saturator_get_drive) {
  const GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->drive;
}

static MFUN(saturator_set_drive) {
  const GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
  m_float drive = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->drive = drive);
}

static MFUN(saturator_get_dcoffset) {
  const GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dcoffset;
}

static MFUN(saturator_set_dcoffset) {
  const GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
  m_float dcoffset = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dcoffset = dcoffset);
}

typedef struct {
  sp_data* sp;
  sp_scale* osc;
} GW_scale;

static TICK(scale_tick) {
  const GW_scale* ug = (GW_scale*)u->module.gen.data;
  sp_scale_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(scale_dtor) {
  GW_scale* ug = UGEN(o)->module.gen.data;
  sp_scale_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_scale), ug);
}

static MFUN(scale_init) {
  GW_scale* ug = mp_malloc2(shred->info->mp, sizeof(GW_scale));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), scale_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_scale_create(&ug->osc) == SP_NOT_OK || sp_scale_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(scale_get_min) {
  const GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(scale_set_min) {
  const GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(scale_get_max) {
  const GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(scale_set_max) {
  const GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
  sp_data* sp;
  sp_sdelay* osc;
} GW_sdelay;

static TICK(sdelay_tick) {
  const GW_sdelay* ug = (GW_sdelay*)u->module.gen.data;
  sp_sdelay_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(sdelay_dtor) {
  GW_sdelay* ug = UGEN(o)->module.gen.data;
  sp_sdelay_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_sdelay), ug);
}

static MFUN(sdelay_init) {
  GW_sdelay* ug = mp_malloc2(shred->info->mp, sizeof(GW_sdelay));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), sdelay_tick, ug, 0);
  m_float size = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_sdelay_create(&ug->osc) == SP_NOT_OK || sp_sdelay_init(ug->sp, ug->osc , size) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_slice* osc;
  M_Object vals_obj;

  M_Object buf_obj;

} GW_slice;

static TICK(slice_tick) {
  const GW_slice* ug = (GW_slice*)u->module.gen.data;
  sp_slice_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(slice_dtor) {
  GW_slice* ug = UGEN(o)->module.gen.data;
  release(ug->vals_obj, shred);

  release(ug->buf_obj, shred);

  sp_slice_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_slice), ug);
}

static MFUN(slice_init) {
  GW_slice* ug = mp_malloc2(shred->info->mp, sizeof(GW_slice));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), slice_tick, ug, 1);
  const M_Object vals_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* vals = FTBL(vals_obj);
  ++vals_obj->ref;
  const M_Object buf_obj = *(M_Object*)(shred->mem+SZ_INT+SZ_INT);
  sp_ftbl* buf = FTBL(buf_obj);
  ++buf_obj->ref;
  *(M_Object*)RETURN = o;
  if(sp_slice_create(&ug->osc) == SP_NOT_OK || sp_slice_init(ug->sp, ug->osc , vals, buf) == SP_NOT_OK) {
    release(vals_obj, shred); // LCOV_EXCL_LINE
    release(buf_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->vals_obj = vals_obj;
  ug->buf_obj = buf_obj;
}

static MFUN(slice_get_id) {
  const GW_slice* ug = (GW_slice*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->id;
}

static MFUN(slice_set_id) {
  const GW_slice* ug = (GW_slice*)UGEN(o)->module.gen.data;
  m_float id = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->id = id);
}

typedef struct {
  sp_data* sp;
  sp_smoothdelay* osc;
} GW_smoothdelay;

static TICK(smoothdelay_tick) {
  const GW_smoothdelay* ug = (GW_smoothdelay*)u->module.gen.data;
  sp_smoothdelay_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(smoothdelay_dtor) {
  GW_smoothdelay* ug = UGEN(o)->module.gen.data;
  sp_smoothdelay_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_smoothdelay), ug);
}

static MFUN(smoothdelay_init) {
  GW_smoothdelay* ug = mp_malloc2(shred->info->mp, sizeof(GW_smoothdelay));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), smoothdelay_tick, ug, 0);
  m_float maxdel = *(m_float*)(shred->mem+SZ_INT);
  m_int interp = *(m_int*)(shred->mem+SZ_INT+SZ_FLOAT);
  *(M_Object*)RETURN = o;
  if(sp_smoothdelay_create(&ug->osc) == SP_NOT_OK || sp_smoothdelay_init(ug->sp, ug->osc , maxdel, interp) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(smoothdelay_get_feedback) {
  const GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->feedback;
}

static MFUN(smoothdelay_set_feedback) {
  const GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  m_float feedback = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->feedback = feedback);
}

static MFUN(smoothdelay_get_del) {
  const GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->del;
}

static MFUN(smoothdelay_set_del) {
  const GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  m_float del = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->del = del);
}

typedef struct {
  sp_data* sp;
  sp_spa* osc;
} GW_spa;

static TICK(spa_tick) {
  const GW_spa* ug = (GW_spa*)u->module.gen.data;
  sp_spa_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(spa_dtor) {
  GW_spa* ug = UGEN(o)->module.gen.data;
  sp_spa_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_spa), ug);
}

static MFUN(spa_init) {
  GW_spa* ug = mp_malloc2(shred->info->mp, sizeof(GW_spa));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), spa_tick, ug, 0);
  M_Object filename_obj = *(M_Object*)(shred->mem+SZ_INT);
  m_str filename = STRING(filename_obj);
  *(M_Object*)RETURN = o;
  if(sp_spa_create(&ug->osc) == SP_NOT_OK || sp_spa_init(ug->sp, ug->osc , filename) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_sparec* osc;
} GW_sparec;

static TICK(sparec_tick) {
  const GW_sparec* ug = (GW_sparec*)u->module.gen.data;
  sp_sparec_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(sparec_dtor) {
  GW_sparec* ug = UGEN(o)->module.gen.data;
  sp_sparec_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_sparec), ug);
}

static MFUN(sparec_init) {
  GW_sparec* ug = mp_malloc2(shred->info->mp, sizeof(GW_sparec));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), sparec_tick, ug, 0);
  M_Object filename_obj = *(M_Object*)(shred->mem+SZ_INT);
  m_str filename = STRING(filename_obj);
  *(M_Object*)RETURN = o;
  if(sp_sparec_create(&ug->osc) == SP_NOT_OK || sp_sparec_init(ug->sp, ug->osc , filename) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_streson* osc;
} GW_streson;

static TICK(streson_tick) {
  const GW_streson* ug = (GW_streson*)u->module.gen.data;
  sp_streson_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(streson_dtor) {
  GW_streson* ug = UGEN(o)->module.gen.data;
  sp_streson_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_streson), ug);
}

static MFUN(streson_init) {
  GW_streson* ug = mp_malloc2(shred->info->mp, sizeof(GW_streson));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), streson_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_streson_create(&ug->osc) == SP_NOT_OK || sp_streson_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(streson_get_freq) {
  const GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(streson_set_freq) {
  const GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(streson_get_fdbgain) {
  const GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->fdbgain;
}

static MFUN(streson_set_fdbgain) {
  const GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
  m_float fdbgain = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->fdbgain = fdbgain);
}

typedef struct {
  sp_data* sp;
  sp_switch* osc;
} GW_switch;

static TICK(switch_tick) {
  const GW_switch* ug = (GW_switch*)u->module.gen.data;
  sp_switch_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->module.gen.trig->in, &u->out);
}

static DTOR(switch_dtor) {
  GW_switch* ug = UGEN(o)->module.gen.data;
  sp_switch_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_switch), ug);
}

static MFUN(switch_init) {
  GW_switch* ug = mp_malloc2(shred->info->mp, sizeof(GW_switch));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 3, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), switch_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_switch_create(&ug->osc) == SP_NOT_OK || sp_switch_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_tabread* osc;
  M_Object ft_obj;

} GW_tabread;

static TICK(tabread_tick) {
  const GW_tabread* ug = (GW_tabread*)u->module.gen.data;
  sp_tabread_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(tabread_dtor) {
  GW_tabread* ug = UGEN(o)->module.gen.data;
  release(ug->ft_obj, shred);

  sp_tabread_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tabread), ug);
}

static MFUN(tabread_init) {
  GW_tabread* ug = mp_malloc2(shred->info->mp, sizeof(GW_tabread));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tabread_tick, ug, 0);
  const M_Object ft_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* ft = FTBL(ft_obj);
  ++ft_obj->ref;
  m_float mode = *(m_float*)(shred->mem+SZ_INT+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_tabread_create(&ug->osc) == SP_NOT_OK || sp_tabread_init(ug->sp, ug->osc , ft, mode) == SP_NOT_OK) {
    release(ft_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->ft_obj = ft_obj;
}

static MFUN(tabread_get_index) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->index;
}

static MFUN(tabread_set_index) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  m_float index = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->index = index);
}

static MFUN(tabread_get_offset) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->offset;
}

static MFUN(tabread_set_offset) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  m_float offset = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->offset = offset);
}

static MFUN(tabread_get_wrap) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->wrap;
}

static MFUN(tabread_set_wrap) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  m_float wrap = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->wrap = wrap);
}

typedef struct {
  sp_data* sp;
  sp_tadsr* osc;
} GW_tadsr;

static TICK(tadsr_tick) {
  const GW_tadsr* ug = (GW_tadsr*)u->module.gen.data;
  sp_tadsr_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tadsr_dtor) {
  GW_tadsr* ug = UGEN(o)->module.gen.data;
  sp_tadsr_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tadsr), ug);
}

static MFUN(tadsr_init) {
  GW_tadsr* ug = mp_malloc2(shred->info->mp, sizeof(GW_tadsr));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tadsr_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_tadsr_create(&ug->osc) == SP_NOT_OK || sp_tadsr_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tadsr_get_atk) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(tadsr_set_atk) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(tadsr_get_dec) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(tadsr_set_dec) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

static MFUN(tadsr_get_sus) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->sus;
}

static MFUN(tadsr_set_sus) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  m_float sus = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->sus = sus);
}

static MFUN(tadsr_get_rel) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(tadsr_set_rel) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
  sp_data* sp;
  sp_talkbox* osc;
} GW_talkbox;

static TICK(talkbox_tick) {
  const GW_talkbox* ug = (GW_talkbox*)u->module.gen.data;
  sp_talkbox_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);
}

static DTOR(talkbox_dtor) {
  GW_talkbox* ug = UGEN(o)->module.gen.data;
  sp_talkbox_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_talkbox), ug);
}

static MFUN(talkbox_init) {
  GW_talkbox* ug = mp_malloc2(shred->info->mp, sizeof(GW_talkbox));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), talkbox_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_talkbox_create(&ug->osc) == SP_NOT_OK || sp_talkbox_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(talkbox_get_quality) {
  const GW_talkbox* ug = (GW_talkbox*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->quality;
}

static MFUN(talkbox_set_quality) {
  const GW_talkbox* ug = (GW_talkbox*)UGEN(o)->module.gen.data;
  m_float quality = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->quality = quality);
}

typedef struct {
  sp_data* sp;
  sp_tblrec* osc;
  M_Object bar_obj;

} GW_tblrec;

static TICK(tblrec_tick) {
  const GW_tblrec* ug = (GW_tblrec*)u->module.gen.data;
  sp_tblrec_compute(ug->sp, ug->osc, &u->in, &u->module.gen.trig->in, &u->out);
}

static DTOR(tblrec_dtor) {
  GW_tblrec* ug = UGEN(o)->module.gen.data;
  release(ug->bar_obj, shred);

  sp_tblrec_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tblrec), ug);
}

static MFUN(tblrec_init) {
  GW_tblrec* ug = mp_malloc2(shred->info->mp, sizeof(GW_tblrec));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tblrec_tick, ug, 1);
  const M_Object bar_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* bar = FTBL(bar_obj);
  ++bar_obj->ref;
  *(M_Object*)RETURN = o;
  if(sp_tblrec_create(&ug->osc) == SP_NOT_OK || sp_tblrec_init(ug->sp, ug->osc , bar) == SP_NOT_OK) {
    release(bar_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->bar_obj = bar_obj;
}

typedef struct {
  sp_data* sp;
  sp_tbvcf* osc;
} GW_tbvcf;

static TICK(tbvcf_tick) {
  const GW_tbvcf* ug = (GW_tbvcf*)u->module.gen.data;
  sp_tbvcf_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(tbvcf_dtor) {
  GW_tbvcf* ug = UGEN(o)->module.gen.data;
  sp_tbvcf_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tbvcf), ug);
}

static MFUN(tbvcf_init) {
  GW_tbvcf* ug = mp_malloc2(shred->info->mp, sizeof(GW_tbvcf));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tbvcf_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_tbvcf_create(&ug->osc) == SP_NOT_OK || sp_tbvcf_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tbvcf_get_fco) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->fco;
}

static MFUN(tbvcf_set_fco) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  m_float fco = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->fco = fco);
}

static MFUN(tbvcf_get_res) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(tbvcf_set_res) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->res = res);
}

static MFUN(tbvcf_get_dist) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dist;
}

static MFUN(tbvcf_set_dist) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  m_float dist = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dist = dist);
}

static MFUN(tbvcf_get_asym) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->asym;
}

static MFUN(tbvcf_set_asym) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  m_float asym = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->asym = asym);
}

typedef struct {
  sp_data* sp;
  sp_tdiv* osc;
} GW_tdiv;

static TICK(tdiv_tick) {
  const GW_tdiv* ug = (GW_tdiv*)u->module.gen.data;
  sp_tdiv_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tdiv_dtor) {
  GW_tdiv* ug = UGEN(o)->module.gen.data;
  sp_tdiv_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tdiv), ug);
}

static MFUN(tdiv_init) {
  GW_tdiv* ug = mp_malloc2(shred->info->mp, sizeof(GW_tdiv));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tdiv_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_tdiv_create(&ug->osc) == SP_NOT_OK || sp_tdiv_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tdiv_get_num) {
  const GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->num;
}

static MFUN(tdiv_set_num) {
  const GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
  m_float num = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->num = num);
}

static MFUN(tdiv_get_offset) {
  const GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->offset;
}

static MFUN(tdiv_set_offset) {
  const GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
  m_float offset = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->offset = offset);
}

typedef struct {
  sp_data* sp;
  sp_tenv* osc;
} GW_tenv;

static TICK(tenv_tick) {
  const GW_tenv* ug = (GW_tenv*)u->module.gen.data;
  sp_tenv_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tenv_dtor) {
  GW_tenv* ug = UGEN(o)->module.gen.data;
  sp_tenv_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tenv), ug);
}

static MFUN(tenv_init) {
  GW_tenv* ug = mp_malloc2(shred->info->mp, sizeof(GW_tenv));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tenv_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_tenv_create(&ug->osc) == SP_NOT_OK || sp_tenv_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tenv_get_atk) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(tenv_set_atk) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(tenv_get_hold) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->hold;
}

static MFUN(tenv_set_hold) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  m_float hold = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->hold = hold);
}

static MFUN(tenv_get_rel) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(tenv_set_rel) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
  sp_data* sp;
  sp_tenv2* osc;
} GW_tenv2;

static TICK(tenv2_tick) {
  const GW_tenv2* ug = (GW_tenv2*)u->module.gen.data;
  sp_tenv2_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tenv2_dtor) {
  GW_tenv2* ug = UGEN(o)->module.gen.data;
  sp_tenv2_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tenv2), ug);
}

static MFUN(tenv2_init) {
  GW_tenv2* ug = mp_malloc2(shred->info->mp, sizeof(GW_tenv2));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tenv2_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_tenv2_create(&ug->osc) == SP_NOT_OK || sp_tenv2_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tenv2_get_atk) {
  const GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(tenv2_set_atk) {
  const GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(tenv2_get_rel) {
  const GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(tenv2_set_rel) {
  const GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
  sp_data* sp;
  sp_tenvx* osc;
} GW_tenvx;

static TICK(tenvx_tick) {
  const GW_tenvx* ug = (GW_tenvx*)u->module.gen.data;
  sp_tenvx_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tenvx_dtor) {
  GW_tenvx* ug = UGEN(o)->module.gen.data;
  sp_tenvx_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tenvx), ug);
}

static MFUN(tenvx_init) {
  GW_tenvx* ug = mp_malloc2(shred->info->mp, sizeof(GW_tenvx));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tenvx_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_tenvx_create(&ug->osc) == SP_NOT_OK || sp_tenvx_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tenvx_get_atk) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(tenvx_set_atk) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(tenvx_get_hold) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->hold;
}

static MFUN(tenvx_set_hold) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  m_float hold = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->hold = hold);
}

static MFUN(tenvx_get_rel) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(tenvx_set_rel) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
  sp_data* sp;
  sp_tgate* osc;
} GW_tgate;

static TICK(tgate_tick) {
  const GW_tgate* ug = (GW_tgate*)u->module.gen.data;
  sp_tgate_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tgate_dtor) {
  GW_tgate* ug = UGEN(o)->module.gen.data;
  sp_tgate_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tgate), ug);
}

static MFUN(tgate_init) {
  GW_tgate* ug = mp_malloc2(shred->info->mp, sizeof(GW_tgate));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tgate_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_tgate_create(&ug->osc) == SP_NOT_OK || sp_tgate_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tgate_get_time) {
  const GW_tgate* ug = (GW_tgate*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->time;
}

static MFUN(tgate_set_time) {
  const GW_tgate* ug = (GW_tgate*)UGEN(o)->module.gen.data;
  m_float time = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->time = time);
}

typedef struct {
  sp_data* sp;
  sp_thresh* osc;
} GW_thresh;

static TICK(thresh_tick) {
  const GW_thresh* ug = (GW_thresh*)u->module.gen.data;
  sp_thresh_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(thresh_dtor) {
  GW_thresh* ug = UGEN(o)->module.gen.data;
  sp_thresh_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_thresh), ug);
}

static MFUN(thresh_init) {
  GW_thresh* ug = mp_malloc2(shred->info->mp, sizeof(GW_thresh));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), thresh_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_thresh_create(&ug->osc) == SP_NOT_OK || sp_thresh_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(thresh_get_thresh) {
  const GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->thresh;
}

static MFUN(thresh_set_thresh) {
  const GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
  m_float thresh = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->thresh = thresh);
}

static MFUN(thresh_get_mode) {
  const GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->mode;
}

static MFUN(thresh_set_mode) {
  const GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
  m_int mode = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
  sp_data* sp;
  sp_timer* osc;
} GW_timer;

static TICK(timer_tick) {
  const GW_timer* ug = (GW_timer*)u->module.gen.data;
  sp_timer_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(timer_dtor) {
  GW_timer* ug = UGEN(o)->module.gen.data;
  sp_timer_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_timer), ug);
}

static MFUN(timer_init) {
  GW_timer* ug = mp_malloc2(shred->info->mp, sizeof(GW_timer));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), timer_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_timer_create(&ug->osc) == SP_NOT_OK || sp_timer_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_tin* osc;
} GW_tin;

static TICK(tin_tick) {
  const GW_tin* ug = (GW_tin*)u->module.gen.data;
  sp_tin_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tin_dtor) {
  GW_tin* ug = UGEN(o)->module.gen.data;
  sp_tin_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tin), ug);
}

static MFUN(tin_init) {
  GW_tin* ug = mp_malloc2(shred->info->mp, sizeof(GW_tin));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tin_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_tin_create(&ug->osc) == SP_NOT_OK || sp_tin_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_tone* osc;
} GW_tone;

static TICK(tone_tick) {
  const GW_tone* ug = (GW_tone*)u->module.gen.data;
  sp_tone_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(tone_dtor) {
  GW_tone* ug = UGEN(o)->module.gen.data;
  sp_tone_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tone), ug);
}

static MFUN(tone_init) {
  GW_tone* ug = mp_malloc2(shred->info->mp, sizeof(GW_tone));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tone_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_tone_create(&ug->osc) == SP_NOT_OK || sp_tone_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tone_get_hp) {
  const GW_tone* ug = (GW_tone*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->hp;
}

static MFUN(tone_set_hp) {
  const GW_tone* ug = (GW_tone*)UGEN(o)->module.gen.data;
  m_float hp = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->hp = hp);
}

typedef struct {
  sp_data* sp;
  sp_trand* osc;
} GW_trand;

static TICK(trand_tick) {
  const GW_trand* ug = (GW_trand*)u->module.gen.data;
  sp_trand_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(trand_dtor) {
  GW_trand* ug = UGEN(o)->module.gen.data;
  sp_trand_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_trand), ug);
}

static MFUN(trand_init) {
  GW_trand* ug = mp_malloc2(shred->info->mp, sizeof(GW_trand));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), trand_tick, ug, 1);
  *(M_Object*)RETURN = o;
  if(sp_trand_create(&ug->osc) == SP_NOT_OK || sp_trand_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(trand_get_min) {
  const GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(trand_set_min) {
  const GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(trand_get_max) {
  const GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(trand_set_max) {
  const GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
  sp_data* sp;
  sp_tseg* osc;
} GW_tseg;

static TICK(tseg_tick) {
  const GW_tseg* ug = (GW_tseg*)u->module.gen.data;
  sp_tseg_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tseg_dtor) {
  GW_tseg* ug = UGEN(o)->module.gen.data;
  sp_tseg_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tseg), ug);
}

static MFUN(tseg_init) {
  GW_tseg* ug = mp_malloc2(shred->info->mp, sizeof(GW_tseg));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tseg_tick, ug, 1);
  m_float ibeg = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_tseg_create(&ug->osc) == SP_NOT_OK || sp_tseg_init(ug->sp, ug->osc , ibeg) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(tseg_get_end) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->end;
}

static MFUN(tseg_set_end) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  m_float end = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->end = end);
}

static MFUN(tseg_get_dur) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(tseg_set_dur) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->dur = dur);
}

static MFUN(tseg_get_type) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->type;
}

static MFUN(tseg_set_type) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  m_float type = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->type = type);
}

typedef struct {
  sp_data* sp;
  sp_tseq* osc;
  M_Object ft_obj;

} GW_tseq;

static TICK(tseq_tick) {
  const GW_tseq* ug = (GW_tseq*)u->module.gen.data;
  sp_tseq_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);
}

static DTOR(tseq_dtor) {
  GW_tseq* ug = UGEN(o)->module.gen.data;
  release(ug->ft_obj, shred);

  sp_tseq_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_tseq), ug);
}

static MFUN(tseq_init) {
  GW_tseq* ug = mp_malloc2(shred->info->mp, sizeof(GW_tseq));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tseq_tick, ug, 1);
  const M_Object ft_obj = *(M_Object*)(shred->mem+SZ_INT);
  sp_ftbl* ft = FTBL(ft_obj);
  ++ft_obj->ref;
  *(M_Object*)RETURN = o;
  if(sp_tseq_create(&ug->osc) == SP_NOT_OK || sp_tseq_init(ug->sp, ug->osc , ft) == SP_NOT_OK) {
    release(ft_obj, shred); // LCOV_EXCL_LINE
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->ft_obj = ft_obj;
}

static MFUN(tseq_get_shuf) {
  const GW_tseq* ug = (GW_tseq*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->shuf;
}

static MFUN(tseq_set_shuf) {
  const GW_tseq* ug = (GW_tseq*)UGEN(o)->module.gen.data;
  m_int shuf = *(m_int*)(shred->mem+SZ_INT);
  *(m_uint*)RETURN = (ug->osc->shuf = shuf);
}

typedef struct {
  sp_data* sp;
  sp_vdelay* osc;
} GW_vdelay;

static TICK(vdelay_tick) {
  const GW_vdelay* ug = (GW_vdelay*)u->module.gen.data;
  sp_vdelay_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(vdelay_dtor) {
  GW_vdelay* ug = UGEN(o)->module.gen.data;
  sp_vdelay_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_vdelay), ug);
}

static MFUN(vdelay_init) {
  GW_vdelay* ug = mp_malloc2(shred->info->mp, sizeof(GW_vdelay));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), vdelay_tick, ug, 0);
  m_float maxdel = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_vdelay_create(&ug->osc) == SP_NOT_OK || sp_vdelay_init(ug->sp, ug->osc , maxdel) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(vdelay_get_del) {
  const GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->del;
}

static MFUN(vdelay_set_del) {
  const GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  m_float del = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->del = del);
}

static MFUN(vdelay_get_feedback) {
  const GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->feedback;
}

static MFUN(vdelay_set_feedback) {
  const GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  m_float feedback = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->feedback = feedback);
}

typedef struct {
  sp_data* sp;
  sp_voc* osc;
} GW_voc;

static TICK(voc_tick) {
  const GW_voc* ug = (GW_voc*)u->module.gen.data;
  sp_voc_compute(ug->sp, ug->osc, &u->out);
}

static DTOR(voc_dtor) {
  GW_voc* ug = UGEN(o)->module.gen.data;
  sp_voc_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_voc), ug);
}

static MFUN(voc_init) {
  GW_voc* ug = mp_malloc2(shred->info->mp, sizeof(GW_voc));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), voc_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_voc_create(&ug->osc) == SP_NOT_OK || sp_voc_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_vocoder* osc;
} GW_vocoder;

static TICK(vocoder_tick) {
  const GW_vocoder* ug = (GW_vocoder*)u->module.gen.data;
  sp_vocoder_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);
}

static DTOR(vocoder_dtor) {
  GW_vocoder* ug = UGEN(o)->module.gen.data;
  sp_vocoder_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_vocoder), ug);
}

static MFUN(vocoder_init) {
  GW_vocoder* ug = mp_malloc2(shred->info->mp, sizeof(GW_vocoder));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), vocoder_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_vocoder_create(&ug->osc) == SP_NOT_OK || sp_vocoder_init(ug->sp, ug->osc ) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(vocoder_get_atk) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->atk;
}

static MFUN(vocoder_set_atk) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->atk = atk);
}

static MFUN(vocoder_get_rel) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->rel;
}

static MFUN(vocoder_set_rel) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->rel = rel);
}

static MFUN(vocoder_get_bwratio) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->bwratio;
}

static MFUN(vocoder_set_bwratio) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  m_float bwratio = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->bwratio = bwratio);
}

typedef struct {
  sp_data* sp;
  sp_waveset* osc;
} GW_waveset;

static TICK(waveset_tick) {
  const GW_waveset* ug = (GW_waveset*)u->module.gen.data;
  sp_waveset_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(waveset_dtor) {
  GW_waveset* ug = UGEN(o)->module.gen.data;
  sp_waveset_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_waveset), ug);
}

static MFUN(waveset_init) {
  GW_waveset* ug = mp_malloc2(shred->info->mp, sizeof(GW_waveset));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), waveset_tick, ug, 0);
  m_float ilen = *(m_float*)(shred->mem+SZ_INT);
  *(M_Object*)RETURN = o;
  if(sp_waveset_create(&ug->osc) == SP_NOT_OK || sp_waveset_init(ug->sp, ug->osc , ilen) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(waveset_get_rep) {
  const GW_waveset* ug = (GW_waveset*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rep;
}

static MFUN(waveset_set_rep) {
  const GW_waveset* ug = (GW_waveset*)UGEN(o)->module.gen.data;
  m_float rep = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->rep = rep);
}

typedef struct {
  sp_data* sp;
  sp_wavin* osc;
} GW_wavin;

static TICK(wavin_tick) {
  const GW_wavin* ug = (GW_wavin*)u->module.gen.data;
  sp_wavin_compute(ug->sp, ug->osc, NULL, &u->out);
}

static DTOR(wavin_dtor) {
  GW_wavin* ug = UGEN(o)->module.gen.data;
  sp_wavin_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_wavin), ug);
}

static MFUN(wavin_init) {
  GW_wavin* ug = mp_malloc2(shred->info->mp, sizeof(GW_wavin));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), wavin_tick, ug, 0);
  M_Object filename_obj = *(M_Object*)(shred->mem+SZ_INT);
  m_str filename = STRING(filename_obj);
  *(M_Object*)RETURN = o;
  if(sp_wavin_create(&ug->osc) == SP_NOT_OK || sp_wavin_init(ug->sp, ug->osc , filename) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_wavout* osc;
} GW_wavout;

static TICK(wavout_tick) {
  const GW_wavout* ug = (GW_wavout*)u->module.gen.data;
  sp_wavout_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(wavout_dtor) {
  GW_wavout* ug = UGEN(o)->module.gen.data;
  sp_wavout_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_wavout), ug);
}

static MFUN(wavout_init) {
  GW_wavout* ug = mp_malloc2(shred->info->mp, sizeof(GW_wavout));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), wavout_tick, ug, 0);
  M_Object filename_obj = *(M_Object*)(shred->mem+SZ_INT);
  m_str filename = STRING(filename_obj);
  *(M_Object*)RETURN = o;
  if(sp_wavout_create(&ug->osc) == SP_NOT_OK || sp_wavout_init(ug->sp, ug->osc , filename) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

typedef struct {
  sp_data* sp;
  sp_wpkorg35* osc;
} GW_wpkorg35;

static TICK(wpkorg35_tick) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)u->module.gen.data;
  sp_wpkorg35_compute(ug->sp, ug->osc, &u->in, &u->out);
}

static DTOR(wpkorg35_dtor) {
  GW_wpkorg35* ug = UGEN(o)->module.gen.data;
  sp_wpkorg35_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_wpkorg35), ug);
}

static MFUN(wpkorg35_init) {
  GW_wpkorg35* ug = mp_malloc2(shred->info->mp, sizeof(GW_wpkorg35));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), wpkorg35_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_wpkorg35_create(&ug->osc) == SP_NOT_OK || sp_wpkorg35_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(wpkorg35_get_cutoff) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cutoff;
}

static MFUN(wpkorg35_set_cutoff) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  m_float cutoff = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->cutoff = cutoff);
}

static MFUN(wpkorg35_get_res) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(wpkorg35_set_res) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->res = res);
}

static MFUN(wpkorg35_get_saturation) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->saturation;
}

static MFUN(wpkorg35_set_saturation) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  m_float saturation = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (ug->osc->saturation = saturation);
}

typedef struct {
  sp_data* sp;
  sp_zitarev* osc;
} GW_zitarev;

static TICK(zitarev_tick) {
  const GW_zitarev* ug = (GW_zitarev*)u->module.gen.data;
  sp_zitarev_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
}

static DTOR(zitarev_dtor) {
  GW_zitarev* ug = UGEN(o)->module.gen.data;
  sp_zitarev_destroy(&ug->osc);
  mp_free2(shred->info->mp, sizeof(GW_zitarev), ug);
}

static MFUN(zitarev_init) {
  GW_zitarev* ug = mp_malloc2(shred->info->mp, sizeof(GW_zitarev));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), zitarev_tick, ug, 0);
  *(M_Object*)RETURN = o;
  if(sp_zitarev_create(&ug->osc) == SP_NOT_OK || sp_zitarev_init(ug->sp, ug->osc) == SP_NOT_OK) {
    sp_handle(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
}

static MFUN(zitarev_get_in_delay) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->in_delay;
}

static MFUN(zitarev_set_in_delay) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float in_delay = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->in_delay = in_delay);
}

static MFUN(zitarev_get_lf_x) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->lf_x;
}

static MFUN(zitarev_set_lf_x) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float lf_x = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->lf_x = lf_x);
}

static MFUN(zitarev_get_rt60_low) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->rt60_low;
}

static MFUN(zitarev_set_rt60_low) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float rt60_low = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->rt60_low = rt60_low);
}

static MFUN(zitarev_get_rt60_mid) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->rt60_mid;
}

static MFUN(zitarev_set_rt60_mid) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float rt60_mid = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->rt60_mid = rt60_mid);
}

static MFUN(zitarev_get_hf_damping) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->hf_damping;
}

static MFUN(zitarev_set_hf_damping) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float hf_damping = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->hf_damping = hf_damping);
}

static MFUN(zitarev_get_eq1_freq) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->eq1_freq;
}

static MFUN(zitarev_set_eq1_freq) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float eq1_freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->eq1_freq = eq1_freq);
}

static MFUN(zitarev_get_eq1_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->eq1_level;
}

static MFUN(zitarev_set_eq1_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float eq1_level = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->eq1_level = eq1_level);
}

static MFUN(zitarev_get_eq2_freq) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->eq2_freq;
}

static MFUN(zitarev_set_eq2_freq) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float eq2_freq = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->eq2_freq = eq2_freq);
}

static MFUN(zitarev_get_eq2_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->eq2_level;
}

static MFUN(zitarev_set_eq2_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float eq2_level = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->eq2_level = eq2_level);
}

static MFUN(zitarev_get_mix) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->mix;
}

static MFUN(zitarev_set_mix) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float mix = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->mix = mix);
}

static MFUN(zitarev_get_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->level;
}

static MFUN(zitarev_set_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float level = *(m_float*)(shred->mem+SZ_INT);
  *(m_float*)RETURN = (*ug->osc->level = level);
}

static TICK(sp_tick) {
  ++((sp_data*)u->module.gen.data)->pos; }

GWMODINI(Soundpipe) {
  sp_data *sp;
  sp_createn(&sp, gwion->vm->bbq->si->out);
  sp->sr = gwion->vm->bbq->si->sr;
  return sp;
}

GWMODEND(Soundpipe) {
  sp_destroy((sp_data**)&self);
}
GWION_IMPORT(Soundpipe) {

  VM* vm = gwi_vm(gwi);
  sp_data *sp = get_module(vm->gwion, "Soundpipe");
  if(!sp) {
    sp_createn(&sp, gwi->gwion->vm->bbq->si->out); 
    sp->sr = gwi->gwion->vm->bbq->si->sr;
    set_module(vm->gwion, "Soundpipe", sp);
  }
  const M_Object o = new_M_UGen(gwi->gwion);
  ugen_ini(gwi->gwion, UGEN(o), 1, 1);
  ugen_gen(gwi->gwion, UGEN(o), sp_tick, sp, 0);
  vector_add(&vm->ugen, (vtype)UGEN(o));
  gwi_item_ini(gwi, "UGen", "@soundpipe main ugen");
  gwi_item_end(gwi, ae_flag_late | ae_flag_const, obj, o);
  ugen_connect(UGEN(o), (UGen)vector_front(&vm->ugen));
  DECL_OB(const Type, t_ftbl, = gwi_class_ini(gwi, "ftbl", NULL));
  gwi_class_xtor(gwi, NULL, ftbl_dtor);
  t_ftbl->nspc->offset += SZ_INT;
  gwi_func_ini(gwi, "void", "_gen_composite");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "a string of space-separated parameters, in groups of four:");
     gwinote(gwi, "arg 1 is the partial number. must be positive, but it doesn't need to be a whole number.");
     gwinote(gwi, "arg 2 is the strength.");
     gwinote(gwi, "arg 3 is the initial phase (expressed in degrees)");
     gwinote(gwi, "arg 4 is the dc offset. A dc offset of 2 will put a 2-strength sinusoid in the range");
     gwinote(gwi, "from (-2,2) to (0, 4)");
     gwi_func_arg(gwi, "string", "_argstring");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_composite, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_file");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "filename");
     gwi_func_arg(gwi, "string", "_filename");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_file, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_gauss");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "The scale of the distribution, in the range of -/+scale");
     gwi_func_arg(gwi, "float", "_scale");
     gwinote(gwi, "Random seed.");
     gwi_func_arg(gwi, "int", "_seed");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_gauss, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_line");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "A list of ordered xy pairs. X expects whole number integers, as they correlate to index positions in the ftable.");
     gwi_func_arg(gwi, "string", "_argstring");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_line, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_padsynth");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "ftable of amplitudes to use");
     gwi_func_arg(gwi, "ftbl", "_amps");
     gwinote(gwi, "Base frequency.");
     gwi_func_arg(gwi, "float", "_f");
     gwinote(gwi, "Bandwidth.");
     gwi_func_arg(gwi, "float", "_bw");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_padsynth, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_rand");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "A string of value pairs. The first value is the value, then the probability.");
     gwi_func_arg(gwi, "string", "_argstring");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_rand, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_scrambler");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "destination ftable");
     gwi_func_arg(gwi, "ftbl", "_dest");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_scrambler, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_sine");
  gwi_func_arg(gwi, "int", "Size");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_sine, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_sinesum");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "A list of amplitudes, in the range 0-1, separated by spaces.Each position coordinates to their partial number. Position 1 is the fundamental amplitude (1 * freq). Position 2 is the first overtone (2 * freq), 3 is the second (3 * freq), etc...");
     gwi_func_arg(gwi, "string", "_argstring");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_sinesum, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_triangle");
  gwi_func_arg(gwi, "int", "Size");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_triangle, ae_flag_none))
  gwi_func_ini(gwi, "void", "_gen_xline");
  gwi_func_arg(gwi, "int", "Size");
     gwinote(gwi, "A list of ordered xy pairs. X expects whole number integers, as they correlate to index positions in the ftable.");
     gwi_func_arg(gwi, "string", "_argstring");
  GWI_BB(gwi_func_end(gwi, ftbl_gen_xline, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "ADSR generator");
     gwinote(gwi, "This is an ADSR generator whose curves are created using a one-pole low pass filter.");
  DECL_OB(const Type, t_adsr, = gwi_class_ini(gwi, "Adsr", "UGen"));
  SET_FLAG(t_adsr, final);
  gwi_class_xtor(gwi, NULL, adsr_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, adsr_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, adsr_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Attack time (in seconds)");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, adsr_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
  GWI_BB(gwi_func_end(gwi, adsr_get_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
     gwinote(gwi, "Decay time (in seconds)");
     gwi_func_arg(gwi, "float", "_dec");
  GWI_BB(gwi_func_end(gwi, adsr_set_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "sus");
  GWI_BB(gwi_func_end(gwi, adsr_get_sus, ae_flag_none))
  gwi_func_ini(gwi, "float", "sus");
     gwinote(gwi, "Sustain (in range 0-1)");
     gwi_func_arg(gwi, "float", "_sus");
  GWI_BB(gwi_func_end(gwi, adsr_set_sus, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
  GWI_BB(gwi_func_end(gwi, adsr_get_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
     gwinote(gwi, "Release time (in seconds)");
     gwi_func_arg(gwi, "float", "_rel");
  GWI_BB(gwi_func_end(gwi, adsr_set_rel, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Allpass filter");
     gwinote(gwi, "    Often used for the creation of reverb modules.");
  DECL_OB(const Type, t_allpass, = gwi_class_ini(gwi, "Allpass", "UGen"));
  SET_FLAG(t_allpass, final);
  gwi_class_xtor(gwi, NULL, allpass_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "The loop time of the filter, in seconds. This can also be thought of as the delay time.");
     gwi_func_arg(gwi, "float", "_looptime");
  GWI_BB(gwi_func_end(gwi, allpass_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "revtime");
  GWI_BB(gwi_func_end(gwi, allpass_get_revtime, ae_flag_none))
  gwi_func_ini(gwi, "float", "revtime");
     gwinote(gwi, "The reverberation time, in seconds (RT-60).");
     gwi_func_arg(gwi, "float", "_revtime");
  GWI_BB(gwi_func_end(gwi, allpass_set_revtime, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "First-order recursive highpass filter");
     gwinote(gwi, "	This is the complement to the tone module.");
  DECL_OB(const Type, t_atone, = gwi_class_ini(gwi, "Atone", "UGen"));
  SET_FLAG(t_atone, final);
  gwi_class_xtor(gwi, NULL, atone_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, atone_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "hp");
  GWI_BB(gwi_func_end(gwi, atone_get_hp, ae_flag_none))
  gwi_func_ini(gwi, "float", "hp");
     gwinote(gwi, "The response curve's half power point (cutoff frequency).");
     gwi_func_arg(gwi, "float", "_hp");
  GWI_BB(gwi_func_end(gwi, atone_set_hp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Automatic wah pedal");
     gwinote(gwi, "	An automatic wah effect, ported from Guitarix via Faust. ");
  DECL_OB(const Type, t_autowah, = gwi_class_ini(gwi, "Autowah", "UGen"));
  SET_FLAG(t_autowah, final);
  gwi_class_xtor(gwi, NULL, autowah_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, autowah_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "level");
  GWI_BB(gwi_func_end(gwi, autowah_get_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "level");
     gwinote(gwi, "Overall level (between 0 and 1)");
     gwi_func_arg(gwi, "float", "_level");
  GWI_BB(gwi_func_end(gwi, autowah_set_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "wah");
  GWI_BB(gwi_func_end(gwi, autowah_get_wah, ae_flag_none))
  gwi_func_ini(gwi, "float", "wah");
     gwinote(gwi, "wah amount");
     gwi_func_arg(gwi, "float", "_wah");
  GWI_BB(gwi_func_end(gwi, autowah_set_wah, ae_flag_none))
  gwi_func_ini(gwi, "float", "mix");
  GWI_BB(gwi_func_end(gwi, autowah_get_mix, ae_flag_none))
  gwi_func_ini(gwi, "float", "mix");
     gwinote(gwi, "Wet/dry amount (100 = wet, 0 = dry)");
     gwi_func_arg(gwi, "float", "_mix");
  GWI_BB(gwi_func_end(gwi, autowah_set_mix, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Balance the gain of one signal based on another signal");
     gwinote(gwi, "    This is often used to restore gain lost in the output of a filter.");
     gwinote(gwi, "In the source code, the value `ihp` is set to a static 10hz. This is the default value in Csound, and should not often need to be changed.");
  DECL_OB(const Type, t_bal, = gwi_class_ini(gwi, "Bal", "UGen"));
  SET_FLAG(t_bal, final);
  gwi_class_xtor(gwi, NULL, bal_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, bal_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Physical model approximating the sound of a struck metal bar");
  DECL_OB(const Type, t_bar, = gwi_class_ini(gwi, "Bar", "UGen"));
  SET_FLAG(t_bar, final);
  gwi_class_xtor(gwi, NULL, bar_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Dimensionless stiffness parameter");
     gwi_func_arg(gwi, "float", "_iK");
     gwinote(gwi, "High-frequency loss parameter. Keep this small");
     gwi_func_arg(gwi, "float", "_ib");
  GWI_BB(gwi_func_end(gwi, bar_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "bcL");
  GWI_BB(gwi_func_end(gwi, bar_get_bcL, ae_flag_none))
  gwi_func_ini(gwi, "float", "bcL");
     gwinote(gwi, "Boundary condition at left end of bar. 1 = clamped, 2 = pivoting, 3 = free");
     gwi_func_arg(gwi, "float", "_bcL");
  GWI_BB(gwi_func_end(gwi, bar_set_bcL, ae_flag_none))
  gwi_func_ini(gwi, "float", "bcR");
  GWI_BB(gwi_func_end(gwi, bar_get_bcR, ae_flag_none))
  gwi_func_ini(gwi, "float", "bcR");
     gwinote(gwi, "Boundary condition at right end of bar. 1 = clamped, 2 = pivoting, 3 = free");
     gwi_func_arg(gwi, "float", "_bcR");
  GWI_BB(gwi_func_end(gwi, bar_set_bcR, ae_flag_none))
  gwi_func_ini(gwi, "float", "T30");
  GWI_BB(gwi_func_end(gwi, bar_get_T30, ae_flag_none))
  gwi_func_ini(gwi, "float", "T30");
     gwinote(gwi, "30db decay time (in seconds).");
     gwi_func_arg(gwi, "float", "_T30");
  GWI_BB(gwi_func_end(gwi, bar_set_T30, ae_flag_none))
  gwi_func_ini(gwi, "float", "scan");
  GWI_BB(gwi_func_end(gwi, bar_get_scan, ae_flag_none))
  gwi_func_ini(gwi, "float", "scan");
     gwinote(gwi, "Speed of scanning the output location.");
     gwi_func_arg(gwi, "float", "_scan");
  GWI_BB(gwi_func_end(gwi, bar_set_scan, ae_flag_none))
  gwi_func_ini(gwi, "float", "pos");
  GWI_BB(gwi_func_end(gwi, bar_get_pos, ae_flag_none))
  gwi_func_ini(gwi, "float", "pos");
     gwinote(gwi, "Position along bar that strike occurs.");
     gwi_func_arg(gwi, "float", "_pos");
  GWI_BB(gwi_func_end(gwi, bar_set_pos, ae_flag_none))
  gwi_func_ini(gwi, "float", "vel");
  GWI_BB(gwi_func_end(gwi, bar_get_vel, ae_flag_none))
  gwi_func_ini(gwi, "float", "vel");
     gwinote(gwi, "Normalized strike velocity");
     gwi_func_arg(gwi, "float", "_vel");
  GWI_BB(gwi_func_end(gwi, bar_set_vel, ae_flag_none))
  gwi_func_ini(gwi, "float", "wid");
  GWI_BB(gwi_func_end(gwi, bar_get_wid, ae_flag_none))
  gwi_func_ini(gwi, "float", "wid");
     gwinote(gwi, "Spatial width of strike.");
     gwi_func_arg(gwi, "float", "_wid");
  GWI_BB(gwi_func_end(gwi, bar_set_wid, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A sweepable biquadratic general purpose filter");
     gwinote(gwi, "    ");
  DECL_OB(const Type, t_biquad, = gwi_class_ini(gwi, "Biquad", "UGen"));
  SET_FLAG(t_biquad, final);
  gwi_class_xtor(gwi, NULL, biquad_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, biquad_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "b0");
  GWI_BB(gwi_func_end(gwi, biquad_get_b0, ae_flag_none))
  gwi_func_ini(gwi, "float", "b0");
     gwinote(gwi, "biquad coefficient.");
     gwi_func_arg(gwi, "float", "_b0");
  GWI_BB(gwi_func_end(gwi, biquad_set_b0, ae_flag_none))
  gwi_func_ini(gwi, "float", "b1");
  GWI_BB(gwi_func_end(gwi, biquad_get_b1, ae_flag_none))
  gwi_func_ini(gwi, "float", "b1");
     gwinote(gwi, "biquad coefficient.");
     gwi_func_arg(gwi, "float", "_b1");
  GWI_BB(gwi_func_end(gwi, biquad_set_b1, ae_flag_none))
  gwi_func_ini(gwi, "float", "b2");
  GWI_BB(gwi_func_end(gwi, biquad_get_b2, ae_flag_none))
  gwi_func_ini(gwi, "float", "b2");
     gwinote(gwi, "biquad coefficient.");
     gwi_func_arg(gwi, "float", "_b2");
  GWI_BB(gwi_func_end(gwi, biquad_set_b2, ae_flag_none))
  gwi_func_ini(gwi, "float", "a0");
  GWI_BB(gwi_func_end(gwi, biquad_get_a0, ae_flag_none))
  gwi_func_ini(gwi, "float", "a0");
     gwinote(gwi, "biquad coefficient.");
     gwi_func_arg(gwi, "float", "_a0");
  GWI_BB(gwi_func_end(gwi, biquad_set_a0, ae_flag_none))
  gwi_func_ini(gwi, "float", "a1");
  GWI_BB(gwi_func_end(gwi, biquad_get_a1, ae_flag_none))
  gwi_func_ini(gwi, "float", "a1");
     gwinote(gwi, "biquad coefficient.");
     gwi_func_arg(gwi, "float", "_a1");
  GWI_BB(gwi_func_end(gwi, biquad_set_a1, ae_flag_none))
  gwi_func_ini(gwi, "float", "a2");
  GWI_BB(gwi_func_end(gwi, biquad_get_a2, ae_flag_none))
  gwi_func_ini(gwi, "float", "a2");
     gwinote(gwi, "biquad coefficient.");
     gwi_func_arg(gwi, "float", "_a2");
  GWI_BB(gwi_func_end(gwi, biquad_set_a2, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Bipolar Scale");
     gwinote(gwi, "    This module scales from bipolar [-1, 1] to another range defined by min and max.");
  DECL_OB(const Type, t_biscale, = gwi_class_ini(gwi, "Biscale", "UGen"));
  SET_FLAG(t_biscale, final);
  gwi_class_xtor(gwi, NULL, biscale_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, biscale_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
  GWI_BB(gwi_func_end(gwi, biscale_get_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
     gwinote(gwi, "Minimum value to scale to.");
     gwi_func_arg(gwi, "float", "_min");
  GWI_BB(gwi_func_end(gwi, biscale_set_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, biscale_get_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
     gwinote(gwi, "Maximum value to scale to.");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, biscale_set_max, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Digital signal degradation");
     gwinote(gwi, "    Bitcrusher will digitally degrade a signal by altering the bitdepth and sampling-rate. ");
  DECL_OB(const Type, t_bitcrush, = gwi_class_ini(gwi, "Bitcrush", "UGen"));
  SET_FLAG(t_bitcrush, final);
  gwi_class_xtor(gwi, NULL, bitcrush_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, bitcrush_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "bitdepth");
  GWI_BB(gwi_func_end(gwi, bitcrush_get_bitdepth, ae_flag_none))
  gwi_func_ini(gwi, "float", "bitdepth");
     gwinote(gwi, "Bit depth. Expects an integer in the range of 1-16. Fractional values will be truncated.");
     gwi_func_arg(gwi, "float", "_bitdepth");
  GWI_BB(gwi_func_end(gwi, bitcrush_set_bitdepth, ae_flag_none))
  gwi_func_ini(gwi, "float", "srate");
  GWI_BB(gwi_func_end(gwi, bitcrush_get_srate, ae_flag_none))
  gwi_func_ini(gwi, "float", "srate");
     gwinote(gwi, "Sampling rate.");
     gwi_func_arg(gwi, "float", "_srate");
  GWI_BB(gwi_func_end(gwi, bitcrush_set_srate, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Bandlimited sawtooth oscillator");
     gwinote(gwi, "This is a bandlimited sawtooth oscillator ported from the `sawtooth` function from the Faust");
     gwinote(gwi, "programming language.");
  DECL_OB(const Type, t_blsaw, = gwi_class_ini(gwi, "Blsaw", "UGen"));
  SET_FLAG(t_blsaw, final);
  gwi_class_xtor(gwi, NULL, blsaw_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, blsaw_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, blsaw_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency, (range 0-20000)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, blsaw_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, blsaw_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude (range 0-1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, blsaw_set_amp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Bandlimited square oscillator");
     gwinote(gwi, "This is a bandlimited square oscillator ported from the `squaretooth` function from the Faust");
     gwinote(gwi, "programming language.");
  DECL_OB(const Type, t_blsquare, = gwi_class_ini(gwi, "Blsquare", "UGen"));
  SET_FLAG(t_blsquare, final);
  gwi_class_xtor(gwi, NULL, blsquare_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, blsquare_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, blsquare_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency, (range 0-20000)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, blsquare_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, blsquare_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude (range 0-1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, blsquare_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "width");
  GWI_BB(gwi_func_end(gwi, blsquare_get_width, ae_flag_none))
  gwi_func_ini(gwi, "float", "width");
     gwinote(gwi, "Duty cycle width (range 0-1).");
     gwi_func_arg(gwi, "float", "_width");
  GWI_BB(gwi_func_end(gwi, blsquare_set_width, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Bandlimited triangle oscillator");
     gwinote(gwi, "This is a bandlimited triangle oscillator ported from the `triangletooth` function from the Faust");
     gwinote(gwi, "programming language.");
  DECL_OB(const Type, t_bltriangle, = gwi_class_ini(gwi, "Bltriangle", "UGen"));
  SET_FLAG(t_bltriangle, final);
  gwi_class_xtor(gwi, NULL, bltriangle_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, bltriangle_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, bltriangle_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency, (range 0-20000)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, bltriangle_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, bltriangle_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude (range 0-1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, bltriangle_set_amp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Brownian noise generator.");
  DECL_OB(const Type, t_brown, = gwi_class_ini(gwi, "Brown", "UGen"));
  SET_FLAG(t_brown, final);
  gwi_class_xtor(gwi, NULL, brown_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, brown_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Bandpass Butterworth filter");
  DECL_OB(const Type, t_butbp, = gwi_class_ini(gwi, "Butbp", "UGen"));
  SET_FLAG(t_butbp, final);
  gwi_class_xtor(gwi, NULL, butbp_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, butbp_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, butbp_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Center Frequency. (in Hertz)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, butbp_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "bw");
  GWI_BB(gwi_func_end(gwi, butbp_get_bw, ae_flag_none))
  gwi_func_ini(gwi, "float", "bw");
     gwinote(gwi, "Bandwidth. (in Hertz)");
     gwi_func_arg(gwi, "float", "_bw");
  GWI_BB(gwi_func_end(gwi, butbp_set_bw, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Band-reject Butterworth filter");
  DECL_OB(const Type, t_butbr, = gwi_class_ini(gwi, "Butbr", "UGen"));
  SET_FLAG(t_butbr, final);
  gwi_class_xtor(gwi, NULL, butbr_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, butbr_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, butbr_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Center Frequency. (in Hertz)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, butbr_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "bw");
  GWI_BB(gwi_func_end(gwi, butbr_get_bw, ae_flag_none))
  gwi_func_ini(gwi, "float", "bw");
     gwinote(gwi, "Bandwidth. (in Hertz)");
     gwi_func_arg(gwi, "float", "_bw");
  GWI_BB(gwi_func_end(gwi, butbr_set_bw, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Highpass Butterworth filter");
  DECL_OB(const Type, t_buthp, = gwi_class_ini(gwi, "Buthp", "UGen"));
  SET_FLAG(t_buthp, final);
  gwi_class_xtor(gwi, NULL, buthp_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, buthp_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, buthp_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Cutoff Frequency.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, buthp_set_freq, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Lowpass Butterworth filter");
  DECL_OB(const Type, t_butlp, = gwi_class_ini(gwi, "Butlp", "UGen"));
  SET_FLAG(t_butlp, final);
  gwi_class_xtor(gwi, NULL, butlp_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, butlp_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, butlp_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Cutoff Frequency.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, butlp_set_freq, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Applies clip-limiting to a signal");
  DECL_OB(const Type, t_clip, = gwi_class_ini(gwi, "Clip", "UGen"));
  SET_FLAG(t_clip, final);
  gwi_class_xtor(gwi, NULL, clip_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, clip_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "lim");
  GWI_BB(gwi_func_end(gwi, clip_get_lim, ae_flag_none))
  gwi_func_ini(gwi, "float", "lim");
     gwinote(gwi, "threshold / limiting value.");
     gwi_func_arg(gwi, "float", "_lim");
  GWI_BB(gwi_func_end(gwi, clip_set_lim, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Resettable clock with subdivisions");
  DECL_OB(const Type, t_clock, = gwi_class_ini(gwi, "Clock", "UGen"));
  SET_FLAG(t_clock, final);
  gwi_class_xtor(gwi, NULL, clock_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, clock_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "bpm");
  GWI_BB(gwi_func_end(gwi, clock_get_bpm, ae_flag_none))
  gwi_func_ini(gwi, "float", "bpm");
     gwinote(gwi, "Clock tempo, in beats per minute.");
     gwi_func_arg(gwi, "float", "_bpm");
  GWI_BB(gwi_func_end(gwi, clock_set_bpm, ae_flag_none))
  gwi_func_ini(gwi, "float", "subdiv");
  GWI_BB(gwi_func_end(gwi, clock_get_subdiv, ae_flag_none))
  gwi_func_ini(gwi, "float", "subdiv");
     gwinote(gwi, "Clock subdivision. 2 = eighths, 4 = 16ths, etc.");
     gwi_func_arg(gwi, "float", "_subdiv");
  GWI_BB(gwi_func_end(gwi, clock_set_subdiv, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Comb filter");
  DECL_OB(const Type, t_comb, = gwi_class_ini(gwi, "Comb", "UGen"));
  SET_FLAG(t_comb, final);
  gwi_class_xtor(gwi, NULL, comb_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "The loop time of the filter, in seconds. This can also be thought of as the delay time.");
     gwi_func_arg(gwi, "float", "_looptime");
  GWI_BB(gwi_func_end(gwi, comb_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "revtime");
  GWI_BB(gwi_func_end(gwi, comb_get_revtime, ae_flag_none))
  gwi_func_ini(gwi, "float", "revtime");
     gwinote(gwi, "Reverberation time, in seconds (RT-60).");
     gwi_func_arg(gwi, "float", "_revtime");
  GWI_BB(gwi_func_end(gwi, comb_set_revtime, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Compressor");
  DECL_OB(const Type, t_compressor, = gwi_class_ini(gwi, "Compressor", "UGen"));
  SET_FLAG(t_compressor, final);
  gwi_class_xtor(gwi, NULL, compressor_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, compressor_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "ratio");
  GWI_BB(gwi_func_end(gwi, compressor_get_ratio, ae_flag_none))
  gwi_func_ini(gwi, "float", "ratio");
     gwinote(gwi, "Ratio to compress with, a value > 1 will compress");
     gwi_func_arg(gwi, "float", "_ratio");
  GWI_BB(gwi_func_end(gwi, compressor_set_ratio, ae_flag_none))
  gwi_func_ini(gwi, "float", "thresh");
  GWI_BB(gwi_func_end(gwi, compressor_get_thresh, ae_flag_none))
  gwi_func_ini(gwi, "float", "thresh");
     gwinote(gwi, "Threshold (in dB) 0 = max");
     gwi_func_arg(gwi, "float", "_thresh");
  GWI_BB(gwi_func_end(gwi, compressor_set_thresh, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, compressor_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Compressor attack");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, compressor_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
  GWI_BB(gwi_func_end(gwi, compressor_get_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
     gwinote(gwi, "Compressor release");
     gwi_func_arg(gwi, "float", "_rel");
  GWI_BB(gwi_func_end(gwi, compressor_set_rel, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Partioned convolution. ");
     gwinote(gwi, "This module will perform partitioned convolution on an input signal using");
     gwinote(gwi, "an ftable as an impulse response.");
  DECL_OB(const Type, t_conv, = gwi_class_ini(gwi, "Conv", "UGen"));
  SET_FLAG(t_conv, final);
  gwi_class_xtor(gwi, NULL, conv_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Ftable used as the impulse response. ");
     gwi_func_arg(gwi, "ftbl", "_ft");
     gwinote(gwi, "Partition length (in samples). ");
     gwinote(gwi, "Must be a power of 2. Lower values will add less latency, at the cost ");
     gwinote(gwi, "of requiring more CPU power. ");
     gwi_func_arg(gwi, "float", "_iPartLen");
  GWI_BB(gwi_func_end(gwi, conv_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Trigger-based fixed counter");
     gwinote(gwi, "    The signal output will count from 0 to [N-1], and then");
     gwinote(gwi, "repeat itself. Count will start when it has been triggered, otherwise it will be -1.");
  DECL_OB(const Type, t_count, = gwi_class_ini(gwi, "Count", "UGen"));
  SET_FLAG(t_count, final);
  gwi_class_xtor(gwi, NULL, count_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, count_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "count");
  GWI_BB(gwi_func_end(gwi, count_get_count, ae_flag_none))
  gwi_func_ini(gwi, "float", "count");
     gwinote(gwi, "Number to count up to (count - 1). Decimal points will be truncated.");
     gwi_func_arg(gwi, "float", "_count");
  GWI_BB(gwi_func_end(gwi, count_set_count, ae_flag_none))
  gwi_func_ini(gwi, "float", "mode");
  GWI_BB(gwi_func_end(gwi, count_get_mode, ae_flag_none))
  gwi_func_ini(gwi, "float", "mode");
     gwinote(gwi, "Counting mode. 0 = wrap-around, 1 = count up to N -1, then stop and spit out -1");
     gwi_func_arg(gwi, "float", "_mode");
  GWI_BB(gwi_func_end(gwi, count_set_mode, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Crossfade two signals. ");
     gwinote(gwi, "This module will perform a linear crossfade between two input signals.");
  DECL_OB(const Type, t_crossfade, = gwi_class_ini(gwi, "Crossfade", "UGen"));
  SET_FLAG(t_crossfade, final);
  gwi_class_xtor(gwi, NULL, crossfade_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, crossfade_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "pos");
  GWI_BB(gwi_func_end(gwi, crossfade_get_pos, ae_flag_none))
  gwi_func_ini(gwi, "float", "pos");
     gwinote(gwi, "Crossfade position. 0 = all signal 1, 1 = all signal 2");
     gwi_func_arg(gwi, "float", "_pos");
  GWI_BB(gwi_func_end(gwi, crossfade_set_pos, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A simple DC block filter");
  DECL_OB(const Type, t_dcblock, = gwi_class_ini(gwi, "Dcblock", "UGen"));
  SET_FLAG(t_dcblock, final);
  gwi_class_xtor(gwi, NULL, dcblock_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, dcblock_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Adds a delay to an incoming signal with optional feedback.");
  DECL_OB(const Type, t_delay, = gwi_class_ini(gwi, "Delay", "UGen"));
  SET_FLAG(t_delay, final);
  gwi_class_xtor(gwi, NULL, delay_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Delay time, in seconds.");
     gwi_func_arg(gwi, "float", "_time");
  GWI_BB(gwi_func_end(gwi, delay_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback");
  GWI_BB(gwi_func_end(gwi, delay_get_feedback, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback");
     gwinote(gwi, "Feedback amount. Should be a value between 0-1.");
     gwi_func_arg(gwi, "float", "_feedback");
  GWI_BB(gwi_func_end(gwi, delay_set_feedback, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Diode-ladder virtual analogue low-pass filter");
     gwinote(gwi, "This is a diode-ladder filter designed by Will Pirkle. ");
     gwinote(gwi, "More information can be found in this paper here: ");
     gwinote(gwi, "http://www.willpirkle.com/Downloads/AN-6DiodeLadderFilter.pdf");
  DECL_OB(const Type, t_diode, = gwi_class_ini(gwi, "Diode", "UGen"));
  SET_FLAG(t_diode, final);
  gwi_class_xtor(gwi, NULL, diode_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, diode_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, diode_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, diode_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
  GWI_BB(gwi_func_end(gwi, diode_get_res, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
     gwi_func_arg(gwi, "float", "_res");
  GWI_BB(gwi_func_end(gwi, diode_set_res, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Read from an audio file");
     gwinote(gwi, "    Expects a 1-channel file matching the project samplerate. Diskin should be able to read any file format that libsndfile supports.");
  DECL_OB(const Type, t_diskin, = gwi_class_ini(gwi, "Diskin", "UGen"));
  SET_FLAG(t_diskin, final);
  gwi_class_xtor(gwi, NULL, diskin_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Filename of the audio file.");
     gwi_func_arg(gwi, "string", "_filename");
  GWI_BB(gwi_func_end(gwi, diskin_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Distortion using a modified hyperbolic tangent function");
  DECL_OB(const Type, t_dist, = gwi_class_ini(gwi, "Dist", "UGen"));
  SET_FLAG(t_dist, final);
  gwi_class_xtor(gwi, NULL, dist_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, dist_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "pregain");
  GWI_BB(gwi_func_end(gwi, dist_get_pregain, ae_flag_none))
  gwi_func_ini(gwi, "float", "pregain");
     gwinote(gwi, "Gain applied before waveshaping.");
     gwi_func_arg(gwi, "float", "_pregain");
  GWI_BB(gwi_func_end(gwi, dist_set_pregain, ae_flag_none))
  gwi_func_ini(gwi, "float", "postgain");
  GWI_BB(gwi_func_end(gwi, dist_get_postgain, ae_flag_none))
  gwi_func_ini(gwi, "float", "postgain");
     gwinote(gwi, "Gain applied after waveshaping");
     gwi_func_arg(gwi, "float", "_postgain");
  GWI_BB(gwi_func_end(gwi, dist_set_postgain, ae_flag_none))
  gwi_func_ini(gwi, "float", "shape1");
  GWI_BB(gwi_func_end(gwi, dist_get_shape1, ae_flag_none))
  gwi_func_ini(gwi, "float", "shape1");
     gwinote(gwi, "Shape of the positive part of the signal. A value of 0 gets a flat clip.");
     gwi_func_arg(gwi, "float", "_shape1");
  GWI_BB(gwi_func_end(gwi, dist_set_shape1, ae_flag_none))
  gwi_func_ini(gwi, "float", "shape2");
  GWI_BB(gwi_func_end(gwi, dist_get_shape2, ae_flag_none))
  gwi_func_ini(gwi, "float", "shape2");
     gwinote(gwi, "Like shape1, only for the negative part of the signal.");
     gwi_func_arg(gwi, "float", "_shape2");
  GWI_BB(gwi_func_end(gwi, dist_set_shape2, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Delta Metro");
     gwinote(gwi, "    Produce a set of triggers spaced apart by time.");
  DECL_OB(const Type, t_dmetro, = gwi_class_ini(gwi, "Dmetro", "UGen"));
  SET_FLAG(t_dmetro, final);
  gwi_class_xtor(gwi, NULL, dmetro_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, dmetro_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "time");
  GWI_BB(gwi_func_end(gwi, dmetro_get_time, ae_flag_none))
  gwi_func_ini(gwi, "float", "time");
     gwinote(gwi, "Time between triggers (in seconds). This will update at the start of each trigger.");
     gwi_func_arg(gwi, "float", "_time");
  GWI_BB(gwi_func_end(gwi, dmetro_set_time, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Water drop physical model");
     gwinote(gwi, "    Physical model of the sound of dripping water. When triggered, it will produce a droplet of water.");
  DECL_OB(const Type, t_drip, = gwi_class_ini(gwi, "Drip", "UGen"));
  SET_FLAG(t_drip, final);
  gwi_class_xtor(gwi, NULL, drip_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Period of time over which all sound is stopped.");
     gwi_func_arg(gwi, "float", "_dettack");
  GWI_BB(gwi_func_end(gwi, drip_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "num_tubes");
  GWI_BB(gwi_func_end(gwi, drip_get_num_tubes, ae_flag_none))
  gwi_func_ini(gwi, "float", "num_tubes");
     gwinote(gwi, "Number of units.");
     gwi_func_arg(gwi, "float", "_num_tubes");
  GWI_BB(gwi_func_end(gwi, drip_set_num_tubes, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, drip_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude.");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, drip_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "damp");
  GWI_BB(gwi_func_end(gwi, drip_get_damp, ae_flag_none))
  gwi_func_ini(gwi, "float", "damp");
     gwinote(gwi, "The damping factor. Maximum value is 2.0.");
     gwi_func_arg(gwi, "float", "_damp");
  GWI_BB(gwi_func_end(gwi, drip_set_damp, ae_flag_none))
  gwi_func_ini(gwi, "float", "shake_max");
  GWI_BB(gwi_func_end(gwi, drip_get_shake_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "shake_max");
     gwinote(gwi, "The amount of energy to add back into the system.");
     gwi_func_arg(gwi, "float", "_shake_max");
  GWI_BB(gwi_func_end(gwi, drip_set_shake_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, drip_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Main resonant frequency.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, drip_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq1");
  GWI_BB(gwi_func_end(gwi, drip_get_freq1, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq1");
     gwinote(gwi, "The first resonant frequency.");
     gwi_func_arg(gwi, "float", "_freq1");
  GWI_BB(gwi_func_end(gwi, drip_set_freq1, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq2");
  GWI_BB(gwi_func_end(gwi, drip_get_freq2, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq2");
     gwinote(gwi, "The second resonant frequency.");
     gwi_func_arg(gwi, "float", "_freq2");
  GWI_BB(gwi_func_end(gwi, drip_set_freq2, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, " Delta trigger");
     gwinote(gwi, "    This is able to create spaced out triggers. It is set off by a single trigger.");
  DECL_OB(const Type, t_dtrig, = gwi_class_ini(gwi, "Dtrig", "UGen"));
  SET_FLAG(t_dtrig, final);
  gwi_class_xtor(gwi, NULL, dtrig_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "An ftable containing times in seconds.");
     gwi_func_arg(gwi, "ftbl", "_ft");
  GWI_BB(gwi_func_end(gwi, dtrig_init, ae_flag_none))
  gwi_func_ini(gwi, "int", "loop");
  GWI_BB(gwi_func_end(gwi, dtrig_get_loop, ae_flag_none))
  gwi_func_ini(gwi, "int", "loop");
     gwinote(gwi, "When set to 1, dtrig will wrap around and loop again.");
     gwi_func_arg(gwi, "int", "_loop");
  GWI_BB(gwi_func_end(gwi, dtrig_set_loop, ae_flag_none))
  gwi_func_ini(gwi, "float", "delay");
  GWI_BB(gwi_func_end(gwi, dtrig_get_delay, ae_flag_none))
  gwi_func_ini(gwi, "float", "delay");
     gwinote(gwi, "This sets a delay (in seconds) on the triggered output when it is initially triggered. This is useful for rhythmic sequences with rests in the beginnings.");
     gwi_func_arg(gwi, "float", "_delay");
  GWI_BB(gwi_func_end(gwi, dtrig_set_delay, ae_flag_none))
  gwi_func_ini(gwi, "float", "scale");
  GWI_BB(gwi_func_end(gwi, dtrig_get_scale, ae_flag_none))
  gwi_func_ini(gwi, "float", "scale");
     gwinote(gwi, "Scales the timing signal. A scale of 1 is normal, a scale of 2 will double the duration, and a scale of 0.5 will halve it.");
     gwi_func_arg(gwi, "float", "_scale");
  GWI_BB(gwi_func_end(gwi, dtrig_set_scale, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A series of random impulses");
  DECL_OB(const Type, t_dust, = gwi_class_ini(gwi, "Dust", "UGen"));
  SET_FLAG(t_dust, final);
  gwi_class_xtor(gwi, NULL, dust_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, dust_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, dust_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, dust_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "density");
  GWI_BB(gwi_func_end(gwi, dust_get_density, ae_flag_none))
  gwi_func_ini(gwi, "float", "density");
     gwi_func_arg(gwi, "float", "_density");
  GWI_BB(gwi_func_end(gwi, dust_set_density, ae_flag_none))
  gwi_func_ini(gwi, "int", "bipolar");
  GWI_BB(gwi_func_end(gwi, dust_get_bipolar, ae_flag_none))
  gwi_func_ini(gwi, "int", "bipolar");
     gwinote(gwi, "Bipolar flag. A non-zero makes the signal bipolar as opposed to unipolar. ");
     gwi_func_arg(gwi, "int", "_bipolar");
  GWI_BB(gwi_func_end(gwi, dust_set_bipolar, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "2nd order tunable equalization filter");
     gwinote(gwi, "    This provides a peak/notch filter for building parametric/graphic equalizers. With gain above 1, there will be a peak at the center frequency with a width dependent on bw. If gain is less than 1, a notch is formed around the center frequency (freq).");
     gwinote(gwi, "    ");
  DECL_OB(const Type, t_eqfil, = gwi_class_ini(gwi, "Eqfil", "UGen"));
  SET_FLAG(t_eqfil, final);
  gwi_class_xtor(gwi, NULL, eqfil_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, eqfil_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, eqfil_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "The center frequency of the filter");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, eqfil_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "bw");
  GWI_BB(gwi_func_end(gwi, eqfil_get_bw, ae_flag_none))
  gwi_func_ini(gwi, "float", "bw");
     gwinote(gwi, "The peak/notch bandwidth in Hertz");
     gwi_func_arg(gwi, "float", "_bw");
  GWI_BB(gwi_func_end(gwi, eqfil_set_bw, ae_flag_none))
  gwi_func_ini(gwi, "float", "gain");
  GWI_BB(gwi_func_end(gwi, eqfil_get_gain, ae_flag_none))
  gwi_func_ini(gwi, "float", "gain");
     gwinote(gwi, "The peak/notch gain");
     gwi_func_arg(gwi, "float", "_gain");
  GWI_BB(gwi_func_end(gwi, eqfil_set_gain, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Produce a line segment with exponential slope");
     gwinote(gwi, "This will generate a line from value A to value B in given amount of time. ");
     gwinote(gwi, "When it reaches it's target, it will stay at that value. ");
  DECL_OB(const Type, t_expon, = gwi_class_ini(gwi, "Expon", "UGen"));
  SET_FLAG(t_expon, final);
  gwi_class_xtor(gwi, NULL, expon_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, expon_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "a");
  GWI_BB(gwi_func_end(gwi, expon_get_a, ae_flag_none))
  gwi_func_ini(gwi, "float", "a");
     gwinote(gwi, "Inital point.");
     gwi_func_arg(gwi, "float", "_a");
  GWI_BB(gwi_func_end(gwi, expon_set_a, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
  GWI_BB(gwi_func_end(gwi, expon_get_dur, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
     gwinote(gwi, "Duration (in seconds)");
     gwi_func_arg(gwi, "float", "_dur");
  GWI_BB(gwi_func_end(gwi, expon_set_dur, ae_flag_none))
  gwi_func_ini(gwi, "float", "b");
  GWI_BB(gwi_func_end(gwi, expon_get_b, ae_flag_none))
  gwi_func_ini(gwi, "float", "b");
     gwinote(gwi, "End point");
     gwi_func_arg(gwi, "float", "_b");
  GWI_BB(gwi_func_end(gwi, expon_set_b, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Produces sinusoid bursts for granular and formant synthesis");
  DECL_OB(const Type, t_fof, = gwi_class_ini(gwi, "Fof", "UGen"));
  SET_FLAG(t_fof, final);
  gwi_class_xtor(gwi, NULL, fof_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "ftable for sine wave.");
     gwi_func_arg(gwi, "ftbl", "_sine");
     gwinote(gwi, "Ftable for envelope function (use either gen_line or gen_sinecomp)");
     gwi_func_arg(gwi, "ftbl", "_win");
     gwinote(gwi, "Maximum number of foflet overlaps.");
     gwi_func_arg(gwi, "int", "_iolaps");
     gwinote(gwi, "Phase");
     gwi_func_arg(gwi, "float", "_iphs");
  GWI_BB(gwi_func_end(gwi, fof_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, fof_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Overall amplitude");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, fof_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "fund");
  GWI_BB(gwi_func_end(gwi, fof_get_fund, ae_flag_none))
  gwi_func_ini(gwi, "float", "fund");
     gwinote(gwi, "Fundamental frequency");
     gwi_func_arg(gwi, "float", "_fund");
  GWI_BB(gwi_func_end(gwi, fof_set_fund, ae_flag_none))
  gwi_func_ini(gwi, "float", "form");
  GWI_BB(gwi_func_end(gwi, fof_get_form, ae_flag_none))
  gwi_func_ini(gwi, "float", "form");
     gwinote(gwi, "Formant frequency.");
     gwi_func_arg(gwi, "float", "_form");
  GWI_BB(gwi_func_end(gwi, fof_set_form, ae_flag_none))
  gwi_func_ini(gwi, "float", "oct");
  GWI_BB(gwi_func_end(gwi, fof_get_oct, ae_flag_none))
  gwi_func_ini(gwi, "float", "oct");
     gwinote(gwi, "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.");
     gwi_func_arg(gwi, "float", "_oct");
  GWI_BB(gwi_func_end(gwi, fof_set_oct, ae_flag_none))
  gwi_func_ini(gwi, "float", "band");
  GWI_BB(gwi_func_end(gwi, fof_get_band, ae_flag_none))
  gwi_func_ini(gwi, "float", "band");
     gwinote(gwi, "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.");
     gwi_func_arg(gwi, "float", "_band");
  GWI_BB(gwi_func_end(gwi, fof_set_band, ae_flag_none))
  gwi_func_ini(gwi, "float", "ris");
  GWI_BB(gwi_func_end(gwi, fof_get_ris, ae_flag_none))
  gwi_func_ini(gwi, "float", "ris");
     gwinote(gwi, "Rise of sinusoid burst (in seconds)");
     gwi_func_arg(gwi, "float", "_ris");
  GWI_BB(gwi_func_end(gwi, fof_set_ris, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
  GWI_BB(gwi_func_end(gwi, fof_get_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
     gwinote(gwi, "Decay of the sinusoid burst (in seconds).");
     gwi_func_arg(gwi, "float", "_dec");
  GWI_BB(gwi_func_end(gwi, fof_set_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
  GWI_BB(gwi_func_end(gwi, fof_get_dur, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
     gwinote(gwi, "OVerall duration of sinusoid burst (in seconds).");
     gwi_func_arg(gwi, "float", "_dur");
  GWI_BB(gwi_func_end(gwi, fof_set_dur, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Formant filter");
     gwinote(gwi, "    When fed with a pulse train, it will generate a series of overlapping grains. Overlapping will occur when 1/freq < dec, but there is no upper limit on the number of overlaps. (cited from www.csounds.com/manual/html/fofilter.html)");
  DECL_OB(const Type, t_fofilt, = gwi_class_ini(gwi, "Fofilt", "UGen"));
  SET_FLAG(t_fofilt, final);
  gwi_class_xtor(gwi, NULL, fofilt_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, fofilt_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, fofilt_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Center frequency.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, fofilt_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, fofilt_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Impulse response attack time (in seconds).");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, fofilt_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
  GWI_BB(gwi_func_end(gwi, fofilt_get_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
     gwinote(gwi, "Impulse reponse decay time (in seconds)");
     gwi_func_arg(gwi, "float", "_dec");
  GWI_BB(gwi_func_end(gwi, fofilt_set_dec, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Sucession of grains from data in a stored function table");
  DECL_OB(const Type, t_fog, = gwi_class_ini(gwi, "Fog", "UGen"));
  SET_FLAG(t_fog, final);
  gwi_class_xtor(gwi, NULL, fog_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "ftable for sample.");
     gwi_func_arg(gwi, "ftbl", "_wav");
     gwinote(gwi, "Ftable for envelope function (use either gen_line or gen_sinecomp)");
     gwi_func_arg(gwi, "ftbl", "_win");
     gwinote(gwi, "Maximum number of foglet overlaps.");
     gwi_func_arg(gwi, "int", "_iolaps");
     gwinote(gwi, "Phase");
     gwi_func_arg(gwi, "float", "_iphs");
  GWI_BB(gwi_func_end(gwi, fog_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, fog_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Overall amplitude");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, fog_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "dens");
  GWI_BB(gwi_func_end(gwi, fog_get_dens, ae_flag_none))
  gwi_func_ini(gwi, "float", "dens");
     gwinote(gwi, "Density. The frequency of grains/second.");
     gwi_func_arg(gwi, "float", "_dens");
  GWI_BB(gwi_func_end(gwi, fog_set_dens, ae_flag_none))
  gwi_func_ini(gwi, "float", "trans");
  GWI_BB(gwi_func_end(gwi, fog_get_trans, ae_flag_none))
  gwi_func_ini(gwi, "float", "trans");
     gwinote(gwi, "Transposition, in terms of playback speed");
     gwi_func_arg(gwi, "float", "_trans");
  GWI_BB(gwi_func_end(gwi, fog_set_trans, ae_flag_none))
  gwi_func_ini(gwi, "float", "spd");
  GWI_BB(gwi_func_end(gwi, fog_get_spd, ae_flag_none))
  gwi_func_ini(gwi, "float", "spd");
     gwinote(gwi, "Starting sample index, normalized 0-1.");
     gwi_func_arg(gwi, "float", "_spd");
  GWI_BB(gwi_func_end(gwi, fog_set_spd, ae_flag_none))
  gwi_func_ini(gwi, "float", "oct");
  GWI_BB(gwi_func_end(gwi, fog_get_oct, ae_flag_none))
  gwi_func_ini(gwi, "float", "oct");
     gwinote(gwi, "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.");
     gwi_func_arg(gwi, "float", "_oct");
  GWI_BB(gwi_func_end(gwi, fog_set_oct, ae_flag_none))
  gwi_func_ini(gwi, "float", "band");
  GWI_BB(gwi_func_end(gwi, fog_get_band, ae_flag_none))
  gwi_func_ini(gwi, "float", "band");
     gwinote(gwi, "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.");
     gwi_func_arg(gwi, "float", "_band");
  GWI_BB(gwi_func_end(gwi, fog_set_band, ae_flag_none))
  gwi_func_ini(gwi, "float", "ris");
  GWI_BB(gwi_func_end(gwi, fog_get_ris, ae_flag_none))
  gwi_func_ini(gwi, "float", "ris");
     gwinote(gwi, "Rise of sinusoid burst (in seconds)");
     gwi_func_arg(gwi, "float", "_ris");
  GWI_BB(gwi_func_end(gwi, fog_set_ris, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
  GWI_BB(gwi_func_end(gwi, fog_get_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
     gwinote(gwi, "Decay of the sinusoid burst (in seconds).");
     gwi_func_arg(gwi, "float", "_dec");
  GWI_BB(gwi_func_end(gwi, fog_set_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
  GWI_BB(gwi_func_end(gwi, fog_get_dur, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
     gwinote(gwi, "OVerall duration of sinusoid burst (in seconds).");
     gwi_func_arg(gwi, "float", "_dur");
  GWI_BB(gwi_func_end(gwi, fog_set_dur, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Adds artificial foldover to an audio signal");
     gwinote(gwi, "    Primarily created for use with Decimator.");
  DECL_OB(const Type, t_fold, = gwi_class_ini(gwi, "Fold", "UGen"));
  SET_FLAG(t_fold, final);
  gwi_class_xtor(gwi, NULL, fold_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, fold_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "incr");
  GWI_BB(gwi_func_end(gwi, fold_get_incr, ae_flag_none))
  gwi_func_ini(gwi, "float", "incr");
     gwinote(gwi, "Increment");
     gwi_func_arg(gwi, "float", "_incr");
  GWI_BB(gwi_func_end(gwi, fold_set_incr, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A short title describing the module");
     gwinote(gwi, "    This is a description of the entire module.  This is not a real module. This description should be a comprehensive sumary of what this function does.");
     gwinote(gwi, "Inside the Lua table, this is expressed as a multiline string, however it does not adhere to the tradtional 80 column rule found in programming.");
     gwinote(gwi, "Write as much text as needed here...");
     gwinote(gwi, "FM oscilator pair with linear interpolation");
  DECL_OB(const Type, t_fosc, = gwi_class_ini(gwi, "Fosc", "UGen"));
  SET_FLAG(t_fosc, final);
  gwi_class_xtor(gwi, NULL, fosc_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Wavetable to read from. Note: the size of this table must be a power of 2.");
     gwi_func_arg(gwi, "ftbl", "_tbl");
  GWI_BB(gwi_func_end(gwi, fosc_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, fosc_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency (in Hz)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, fosc_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, fosc_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude (typically a value between 0 and 1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, fosc_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "car");
  GWI_BB(gwi_func_end(gwi, fosc_get_car, ae_flag_none))
  gwi_func_ini(gwi, "float", "car");
     gwinote(gwi, "Carrier frequency, expressed as a ratio number in C:M ratio. Typically an integer.");
     gwi_func_arg(gwi, "float", "_car");
  GWI_BB(gwi_func_end(gwi, fosc_set_car, ae_flag_none))
  gwi_func_ini(gwi, "float", "mod");
  GWI_BB(gwi_func_end(gwi, fosc_get_mod, ae_flag_none))
  gwi_func_ini(gwi, "float", "mod");
     gwinote(gwi, "Modulator frequency, expressed as a ratio number in C:M ratio. Typically an integer.");
     gwi_func_arg(gwi, "float", "_mod");
  GWI_BB(gwi_func_end(gwi, fosc_set_mod, ae_flag_none))
  gwi_func_ini(gwi, "float", "indx");
  GWI_BB(gwi_func_end(gwi, fosc_get_indx, ae_flag_none))
  gwi_func_ini(gwi, "float", "indx");
     gwinote(gwi, "Modulation index. Most commercial synthesizers have a range from 0-8, but there's nothing stopping you from going beyond that.");
     gwi_func_arg(gwi, "float", "_indx");
  GWI_BB(gwi_func_end(gwi, fosc_set_indx, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Series of partials from the harmonic series");
     gwinote(gwi, "    GBuzz comes from the `buzz` family of Csound opcodes, and is capable of producing a rich spectrum of harmonic content, useful for subtractive synthesis implementation.");
  DECL_OB(const Type, t_gbuzz, = gwi_class_ini(gwi, "Gbuzz", "UGen"));
  SET_FLAG(t_gbuzz, final);
  gwi_class_xtor(gwi, NULL, gbuzz_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Soundpipe function table used internally. This should be a sine wave.");
     gwi_func_arg(gwi, "ftbl", "_ft");
     gwinote(gwi, "Phase to start on (in the range 0-1)");
     gwi_func_arg(gwi, "float", "_iphs");
  GWI_BB(gwi_func_end(gwi, gbuzz_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, gbuzz_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency, in Hertz.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, gbuzz_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, gbuzz_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude (Typically a value between 0 and 1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, gbuzz_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "nharm");
  GWI_BB(gwi_func_end(gwi, gbuzz_get_nharm, ae_flag_none))
  gwi_func_ini(gwi, "float", "nharm");
     gwinote(gwi, "Number of harmonics.");
     gwi_func_arg(gwi, "float", "_nharm");
  GWI_BB(gwi_func_end(gwi, gbuzz_set_nharm, ae_flag_none))
  gwi_func_ini(gwi, "float", "lharm");
  GWI_BB(gwi_func_end(gwi, gbuzz_get_lharm, ae_flag_none))
  gwi_func_ini(gwi, "float", "lharm");
     gwinote(gwi, "Lowest harmonic present. This should be a whole number integer.");
     gwi_func_arg(gwi, "float", "_lharm");
  GWI_BB(gwi_func_end(gwi, gbuzz_set_lharm, ae_flag_none))
  gwi_func_ini(gwi, "float", "mul");
  GWI_BB(gwi_func_end(gwi, gbuzz_get_mul, ae_flag_none))
  gwi_func_ini(gwi, "float", "mul");
     gwinote(gwi, "Multiplier. This determines the relative strength of each harmonic.");
     gwi_func_arg(gwi, "float", "_mul");
  GWI_BB(gwi_func_end(gwi, gbuzz_set_mul, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Generate a composite waveform of sinusoids");
     gwinote(gwi, "Reads from a wav file");
     gwinote(gwi, "    This will only load as many samples as the length of the ftable.");
     gwinote(gwi, "Gaussian distribution");
     gwinote(gwi, "A series of line segments");
     gwinote(gwi, "An implementation of the Padsynth Algorithm by Paul Nasca. ");
     gwinote(gwi, "This is a basic implemenation of the padsynth algorithm. More information can be found here:");
     gwinote(gwi, "http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm");
     gwinote(gwi, "This gen routine requires libfftw, and is not compiled by default. See config.mk for more info.");
     gwinote(gwi, "    ");
     gwinote(gwi, "    ");
     gwinote(gwi, "Generates a user defined random number distribution.");
     gwinote(gwi, "Scrambles phase of ftable.");
     gwinote(gwi, "This gen routine will copy the ftable, apply an FFT, apply");
     gwinote(gwi, "a random phase, and then do an inverse FFT. This effect ");
     gwinote(gwi, "is ideal for creating pad-like sounds. ");
     gwinote(gwi, "generates a sampled sinusoid");
     gwinote(gwi, "Waveform as a sum of harmonically related sine waves ");
     gwinote(gwi, "generates a sampled triangle wave");
     gwinote(gwi, "A series of exponential segments");
     gwinote(gwi, "Hilbert transform");
  DECL_OB(const Type, t_hilbert, = gwi_class_ini(gwi, "Hilbert", "UGen"));
  SET_FLAG(t_hilbert, final);
  gwi_class_xtor(gwi, NULL, hilbert_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, hilbert_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Reads from standard input.");
     gwinote(gwi, "    Expects type of SPFLOAT, which by default is a float. If the input data is larger than the number of samples, you will get a complaint about a broken pipe (but it will still work). If there is no input data from STDIN, it will hang.");
     gwinote(gwi, "The expected use case of sp_in is to utilize pipes from the commandline, like so:");
     gwinote(gwi, "cat /dev/urandom | ./my_program");
     gwinote(gwi, "Assuming my_program is using sp_in, this will write /dev/urandom (essentially white noise) to an audio file.");
  DECL_OB(const Type, t_in, = gwi_class_ini(gwi, "In", "UGen"));
  SET_FLAG(t_in, final);
  gwi_class_xtor(gwi, NULL, in_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, in_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Trigger-based Incrementor");
     gwinote(gwi, "When triggered, this module will increment and decrement a value bounded between a min");
     gwinote(gwi, "and max. Initially, this was designed for the specific use case of interfacing with the");
     gwinote(gwi, "griffin knob. ");
  DECL_OB(const Type, t_incr, = gwi_class_ini(gwi, "Incr", "UGen"));
  SET_FLAG(t_incr, final);
  gwi_class_xtor(gwi, NULL, incr_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Initial value");
     gwi_func_arg(gwi, "float", "_val");
  GWI_BB(gwi_func_end(gwi, incr_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "step");
  GWI_BB(gwi_func_end(gwi, incr_get_step, ae_flag_none))
  gwi_func_ini(gwi, "float", "step");
     gwinote(gwi, "Step value to increment by.");
     gwi_func_arg(gwi, "float", "_step");
  GWI_BB(gwi_func_end(gwi, incr_set_step, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
  GWI_BB(gwi_func_end(gwi, incr_get_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
     gwinote(gwi, "Minimum value");
     gwi_func_arg(gwi, "float", "_min");
  GWI_BB(gwi_func_end(gwi, incr_set_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, incr_get_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
     gwinote(gwi, "Maximum value");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, incr_set_max, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "John Chowning reverberator");
     gwinote(gwi, "    This is was built using the JC reverb implentation found in FAUST. According to the source code, the specifications for");
     gwinote(gwi, "this implementation were found on an old SAIL DART backup tape.");
     gwinote(gwi, "  This class is derived from the CLM JCRev function, which is based on the use of");
     gwinote(gwi, "  networks of simple allpass and comb delay filters.  This class implements three series");
     gwinote(gwi, "  allpass units, followed by four parallel comb filters, and two decorrelation delay lines in");
     gwinote(gwi, "  parallel at the output.");
  DECL_OB(const Type, t_jcrev, = gwi_class_ini(gwi, "Jcrev", "UGen"));
  SET_FLAG(t_jcrev, final);
  gwi_class_xtor(gwi, NULL, jcrev_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, jcrev_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A signal with random fluctuations");
     gwinote(gwi, "     This is useful for emulating jitter found in analogue equipment. ");
  DECL_OB(const Type, t_jitter, = gwi_class_ini(gwi, "Jitter", "UGen"));
  SET_FLAG(t_jitter, final);
  gwi_class_xtor(gwi, NULL, jitter_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, jitter_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, jitter_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "The amplitude of the line. Will produce values in the range of (+/-)amp.");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, jitter_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "cpsMin");
  GWI_BB(gwi_func_end(gwi, jitter_get_cpsMin, ae_flag_none))
  gwi_func_ini(gwi, "float", "cpsMin");
     gwinote(gwi, "The minimum frequency of change in Hz.");
     gwi_func_arg(gwi, "float", "_cpsMin");
  GWI_BB(gwi_func_end(gwi, jitter_set_cpsMin, ae_flag_none))
  gwi_func_ini(gwi, "float", "cpsMax");
  GWI_BB(gwi_func_end(gwi, jitter_get_cpsMax, ae_flag_none))
  gwi_func_ini(gwi, "float", "cpsMax");
     gwinote(gwi, "The maximum frequency of change in Hz.");
     gwi_func_arg(gwi, "float", "_cpsMax");
  GWI_BB(gwi_func_end(gwi, jitter_set_cpsMax, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Produce a line segment with linear slope");
     gwinote(gwi, "This will generate a line from value A to value B in given amount of time. ");
     gwinote(gwi, "When it reaches it's target, it will stay at that value. ");
  DECL_OB(const Type, t_line, = gwi_class_ini(gwi, "Line", "UGen"));
  SET_FLAG(t_line, final);
  gwi_class_xtor(gwi, NULL, line_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, line_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "a");
  GWI_BB(gwi_func_end(gwi, line_get_a, ae_flag_none))
  gwi_func_ini(gwi, "float", "a");
     gwinote(gwi, "Inital point.");
     gwi_func_arg(gwi, "float", "_a");
  GWI_BB(gwi_func_end(gwi, line_set_a, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
  GWI_BB(gwi_func_end(gwi, line_get_dur, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
     gwinote(gwi, "Duration (in seconds)");
     gwi_func_arg(gwi, "float", "_dur");
  GWI_BB(gwi_func_end(gwi, line_set_dur, ae_flag_none))
  gwi_func_ini(gwi, "float", "b");
  GWI_BB(gwi_func_end(gwi, line_get_b, ae_flag_none))
  gwi_func_ini(gwi, "float", "b");
     gwinote(gwi, "End point");
     gwi_func_arg(gwi, "float", "_b");
  GWI_BB(gwi_func_end(gwi, line_set_b, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A linear predictive coding filter.");
     gwinote(gwi, "This module is a wrapper for the open source library openlpc, which implements");
     gwinote(gwi, "the LPC10 audio codec optimized for speech signals. This module takes in an");
     gwinote(gwi, "input signal, downsamples it, and produces a decoded LPC10 audio signal, which");
     gwinote(gwi, "has a similar sound to that of a speak and spell. In this context, the LPC");
     gwinote(gwi, "signal is meant to be more of a audio effect rather than a utility for");
     gwinote(gwi, "communication. ");
     gwinote(gwi, "Because the LPC10 encoder");
     gwinote(gwi, "relies on frames for encoding, the output signal has a few milliseconds of");
     gwinote(gwi, "delay. The delay can be calculated in seconds as (framesize * 4) / samplerate.");
     gwinote(gwi, "In addition to using the LPC as a decoder/encoder, this module can also be ");
     gwinote(gwi, "set to synth mode. Instead of reading from an input signal, the LPC can");
     gwinote(gwi, "instead read from parameters set directly in a scaled ftable. ");
  DECL_OB(const Type, t_lpc, = gwi_class_ini(gwi, "Lpc", "UGen"));
  SET_FLAG(t_lpc, final);
  gwi_class_xtor(gwi, NULL, lpc_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Sets the frame size for the encoder.");
     gwi_func_arg(gwi, "int", "_framesize");
  GWI_BB(gwi_func_end(gwi, lpc_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "3-pole (18 db/oct slope) Low-Pass filter with resonance and tanh distortion");
  DECL_OB(const Type, t_lpf18, = gwi_class_ini(gwi, "Lpf18", "UGen"));
  SET_FLAG(t_lpf18, final);
  gwi_class_xtor(gwi, NULL, lpf18_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, lpf18_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "cutoff");
  GWI_BB(gwi_func_end(gwi, lpf18_get_cutoff, ae_flag_none))
  gwi_func_ini(gwi, "float", "cutoff");
     gwinote(gwi, "Filter cutoff frequency, in Hertz");
     gwi_func_arg(gwi, "float", "_cutoff");
  GWI_BB(gwi_func_end(gwi, lpf18_set_cutoff, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
  GWI_BB(gwi_func_end(gwi, lpf18_get_res, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
     gwinote(gwi, "Resonance. Expects a value in the range 0-1. A value of 1.0 will self oscillate at the cutoff frequency.");
     gwi_func_arg(gwi, "float", "_res");
  GWI_BB(gwi_func_end(gwi, lpf18_set_res, ae_flag_none))
  gwi_func_ini(gwi, "float", "dist");
  GWI_BB(gwi_func_end(gwi, lpf18_get_dist, ae_flag_none))
  gwi_func_ini(gwi, "float", "dist");
     gwinote(gwi, "Distortion amount.");
     gwi_func_arg(gwi, "float", "_dist");
  GWI_BB(gwi_func_end(gwi, lpf18_set_dist, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A randomly open or closed `maybe gate`");
     gwinote(gwi, "    It takes in a trigger, and then it will randomly decide to turn the gate on or not. One particular application for maygate is to arbitrarily turn on/off sends to effects. One specific example of this could be a randomized reverb throw on a snare.");
  DECL_OB(const Type, t_maygate, = gwi_class_ini(gwi, "Maygate", "UGen"));
  SET_FLAG(t_maygate, final);
  gwi_class_xtor(gwi, NULL, maygate_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, maygate_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "prob");
  GWI_BB(gwi_func_end(gwi, maygate_get_prob, ae_flag_none))
  gwi_func_ini(gwi, "float", "prob");
     gwinote(gwi, "Probability of maygate. This is a value between 0-1. The closer to 1, the more likely the maygate will let a signal through.");
     gwi_func_arg(gwi, "float", "_prob");
  GWI_BB(gwi_func_end(gwi, maygate_set_prob, ae_flag_none))
  gwi_func_ini(gwi, "int", "mode");
  GWI_BB(gwi_func_end(gwi, maygate_get_mode, ae_flag_none))
  gwi_func_ini(gwi, "int", "mode");
     gwinote(gwi, "If mode is nonzero, maygate will output one sample triggers instead of a gate signal.");
     gwi_func_arg(gwi, "int", "_mode");
  GWI_BB(gwi_func_end(gwi, maygate_set_mode, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Metronome");
     gwinote(gwi, "    Metro produces a series of 1-sample ticks at a regular rate. Typically, this is used alongside trigger-driven modules.");
  DECL_OB(const Type, t_metro, = gwi_class_ini(gwi, "Metro", "UGen"));
  SET_FLAG(t_metro, final);
  gwi_class_xtor(gwi, NULL, metro_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, metro_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, metro_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "The frequency to repeat.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, metro_set_freq, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Phase-locked vocoder.");
     gwinote(gwi, "    Mincer is a phase-locked vocoder. It has the ability to play back an audio ");
     gwinote(gwi, "file loaded into an ftable like a sampler would. Unlike a typical sampler, mincer allows");
     gwinote(gwi, "time and pitch to be controlled separately. ");
  DECL_OB(const Type, t_mincer, = gwi_class_ini(gwi, "Mincer", "UGen"));
  SET_FLAG(t_mincer, final);
  gwi_class_xtor(gwi, NULL, mincer_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "ftable containing an audio file.");
     gwi_func_arg(gwi, "ftbl", "_ft");
     gwinote(gwi, "FFT window size. Should be a power of 2.");
     gwi_func_arg(gwi, "int", "_winsize");
  GWI_BB(gwi_func_end(gwi, mincer_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "time");
  GWI_BB(gwi_func_end(gwi, mincer_get_time, ae_flag_none))
  gwi_func_ini(gwi, "float", "time");
     gwinote(gwi, "Position in time. When non-changing it will do a spectral freeze of a the current point in time.");
     gwi_func_arg(gwi, "float", "_time");
  GWI_BB(gwi_func_end(gwi, mincer_set_time, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, mincer_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude.");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, mincer_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "pitch");
  GWI_BB(gwi_func_end(gwi, mincer_get_pitch, ae_flag_none))
  gwi_func_ini(gwi, "float", "pitch");
     gwinote(gwi, "Pitch ratio. A value of. 1  normal, 2 is double speed, 0.5 is halfspeed, etc.");
     gwi_func_arg(gwi, "float", "_pitch");
  GWI_BB(gwi_func_end(gwi, mincer_set_pitch, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Resonance filter used for modal synthesis");
     gwinote(gwi, "    Plucked and bell sounds can be created by passing an impulse through a combination of modal filters. ");
  DECL_OB(const Type, t_mode, = gwi_class_ini(gwi, "Mode", "UGen"));
  SET_FLAG(t_mode, final);
  gwi_class_xtor(gwi, NULL, mode_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, mode_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, mode_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Resonant frequency of the filter.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, mode_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "q");
  GWI_BB(gwi_func_end(gwi, mode_get_q, ae_flag_none))
  gwi_func_ini(gwi, "float", "q");
     gwinote(gwi, "Quality factor of the filter. Roughly equal to q/freq.");
     gwi_func_arg(gwi, "float", "_q");
  GWI_BB(gwi_func_end(gwi, mode_set_q, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Low pass resonant filter based on the Moogladder filter");
  DECL_OB(const Type, t_moogladder, = gwi_class_ini(gwi, "Moogladder", "UGen"));
  SET_FLAG(t_moogladder, final);
  gwi_class_xtor(gwi, NULL, moogladder_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, moogladder_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, moogladder_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Filter cutoff frequency.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, moogladder_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
  GWI_BB(gwi_func_end(gwi, moogladder_get_res, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
     gwinote(gwi, "Filter resonance");
     gwi_func_arg(gwi, "float", "_res");
  GWI_BB(gwi_func_end(gwi, moogladder_set_res, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "White noise generator");
  DECL_OB(const Type, t_noise, = gwi_class_ini(gwi, "Noise", "UGen"));
  SET_FLAG(t_noise, final);
  gwi_class_xtor(gwi, NULL, noise_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, noise_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, noise_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude. (Value between 0-1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, noise_set_amp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Nanosamp: a tiny sampler built for Soundpipe");
     gwinote(gwi, "    A nanosamp file is comprised of a mono audio file and an ini file. Nanosamp is geared towards percussive and found sound sample players, and is intended to be combined with soundpipe modules.");
     gwinote(gwi, "The ini file contains mappings that correspond to the audio file. Such an entry would look like this:");
     gwinote(gwi, "[keyword]");
     gwinote(gwi, "pos = 2.3");
     gwinote(gwi, "size = 3");
     gwinote(gwi, "</pre>");
     gwinote(gwi, "In this instance, an entry called `keyword` has been made, starting at 2.3 seconds in the");
     gwinote(gwi, "audio file, with a length of 3 seconds. An example file oneart.ini has been created in the");
     gwinote(gwi, "examples folder.");
     gwinote(gwi, "The SoundPipe implementation of nanosamp will automatically index the entries");
     gwinote(gwi, "in the order they appear in the INI file and must be selected this way by changing the index");
     gwinote(gwi, "parameter. Soundpipe will only select the new entry when the trigger input is a non-zero value.");
  DECL_OB(const Type, t_nsmp, = gwi_class_ini(gwi, "Nsmp", "UGen"));
  SET_FLAG(t_nsmp, final);
  gwi_class_xtor(gwi, NULL, nsmp_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "ftbl of the audio file. It should be mono.");
     gwi_func_arg(gwi, "ftbl", "_ft");
     gwinote(gwi, "samplerate.");
     gwi_func_arg(gwi, "int", "_sr");
     gwinote(gwi, "ini file.");
     gwi_func_arg(gwi, "string", "_init");
  GWI_BB(gwi_func_end(gwi, nsmp_init, ae_flag_none))
  gwi_func_ini(gwi, "int", "index");
  GWI_BB(gwi_func_end(gwi, nsmp_get_index, ae_flag_none))
  gwi_func_ini(gwi, "int", "index");
     gwinote(gwi, "This is an optional parameter. These values are always set to a value by default, and can be set after the init function has been called.");
     gwi_func_arg(gwi, "int", "_index");
  GWI_BB(gwi_func_end(gwi, nsmp_set_index, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, " Table-lookup oscilator with linear interpolation");
  DECL_OB(const Type, t_osc, = gwi_class_ini(gwi, "Osc", "UGen"));
  SET_FLAG(t_osc, final);
  gwi_class_xtor(gwi, NULL, osc_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Wavetable to read from. Note: the size of this table must be a power of 2.");
     gwi_func_arg(gwi, "ftbl", "_tbl");
     gwinote(gwi, "Initial phase of waveform, expects a value 0-1");
     gwi_func_arg(gwi, "float", "_phase");
  GWI_BB(gwi_func_end(gwi, osc_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, osc_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency (in Hz)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, osc_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, osc_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude (typically a value between 0 and 1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, osc_set_amp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Wavetable morphing oscillator");
     gwinote(gwi, "This is an oscillator with linear interpolation that is capable of morphing ");
     gwinote(gwi, "between an arbitrary number of wavetables. ");
  DECL_OB(const Type, t_oscmorph, = gwi_class_ini(gwi, "Oscmorph", "UGen"));
  SET_FLAG(t_oscmorph, final);
  gwi_class_xtor(gwi, NULL, oscmorph_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "An array of ftables to read from. Note: the size of these tables must be a power of 2 (and the same size as well).");
     gwi_func_arg(gwi, "ftbl[]", "_tbl");
     gwinote(gwi, "Number of ftbls");
     gwi_func_arg(gwi, "int", "_nft");
     gwinote(gwi, "Initial phase of waveform, expects a value 0-1");
     gwi_func_arg(gwi, "float", "_phase");
  GWI_BB(gwi_func_end(gwi, oscmorph_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, oscmorph_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency (in Hz)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, oscmorph_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, oscmorph_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude (typically a value between 0 and 1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, oscmorph_set_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "wtpos");
  GWI_BB(gwi_func_end(gwi, oscmorph_get_wtpos, ae_flag_none))
  gwi_func_ini(gwi, "float", "wtpos");
     gwinote(gwi, "Wavetable position. (scaled 0-1)");
     gwi_func_arg(gwi, "float", "_wtpos");
  GWI_BB(gwi_func_end(gwi, oscmorph_set_wtpos, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Panner");
  DECL_OB(const Type, t_pan2, = gwi_class_ini(gwi, "Pan2", "UGen"));
  SET_FLAG(t_pan2, final);
  gwi_class_xtor(gwi, NULL, pan2_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, pan2_init, ae_flag_none))
  gwi_func_ini(gwi, "int", "type");
  GWI_BB(gwi_func_end(gwi, pan2_get_type, ae_flag_none))
  gwi_func_ini(gwi, "int", "type");
     gwinote(gwi, "Panning type. 0 = equal power, 1 = square root, 2 = linear,");
     gwinote(gwi, "3 = alternative equal power. Values outside this range will wrap. ");
     gwi_func_arg(gwi, "int", "_type");
  GWI_BB(gwi_func_end(gwi, pan2_set_type, ae_flag_none))
  gwi_func_ini(gwi, "float", "pan");
  GWI_BB(gwi_func_end(gwi, pan2_get_pan, ae_flag_none))
  gwi_func_ini(gwi, "float", "pan");
     gwinote(gwi, "Panning. A value of -1 is hard left, and a value of 1 is hard right, and 0 is center.");
     gwi_func_arg(gwi, "float", "_pan");
  GWI_BB(gwi_func_end(gwi, pan2_set_pan, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Stereo Panner");
  DECL_OB(const Type, t_panst, = gwi_class_ini(gwi, "Panst", "UGen"));
  SET_FLAG(t_panst, final);
  gwi_class_xtor(gwi, NULL, panst_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, panst_init, ae_flag_none))
  gwi_func_ini(gwi, "int", "type");
  GWI_BB(gwi_func_end(gwi, panst_get_type, ae_flag_none))
  gwi_func_ini(gwi, "int", "type");
     gwinote(gwi, "Panning type. 0 = equal power, 1 = square root, 2 = linear,");
     gwinote(gwi, "3 = alternative equal power. Values outside this range will wrap. ");
     gwi_func_arg(gwi, "int", "_type");
  GWI_BB(gwi_func_end(gwi, panst_set_type, ae_flag_none))
  gwi_func_ini(gwi, "float", "pan");
  GWI_BB(gwi_func_end(gwi, panst_get_pan, ae_flag_none))
  gwi_func_ini(gwi, "float", "pan");
     gwinote(gwi, "Panning. A value of -1 is hard left, and a value of 1 is hard right.");
     gwi_func_arg(gwi, "float", "_pan");
  GWI_BB(gwi_func_end(gwi, panst_set_pan, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Parametric Equalizer");
     gwinote(gwi, "This is an implementation of Zoelzer's parametric equalizer filter.");
  DECL_OB(const Type, t_pareq, = gwi_class_ini(gwi, "Pareq", "UGen"));
  SET_FLAG(t_pareq, final);
  gwi_class_xtor(gwi, NULL, pareq_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, pareq_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "fc");
  GWI_BB(gwi_func_end(gwi, pareq_get_fc, ae_flag_none))
  gwi_func_ini(gwi, "float", "fc");
     gwinote(gwi, "Center frequency in peak mode, corner frequency in shelving mode.");
     gwi_func_arg(gwi, "float", "_fc");
  GWI_BB(gwi_func_end(gwi, pareq_set_fc, ae_flag_none))
  gwi_func_ini(gwi, "float", "v");
  GWI_BB(gwi_func_end(gwi, pareq_get_v, ae_flag_none))
  gwi_func_ini(gwi, "float", "v");
     gwinote(gwi, "Amount at which the center frequency value shall be increased or decreased. A value of 1 is a flat response.");
     gwi_func_arg(gwi, "float", "_v");
  GWI_BB(gwi_func_end(gwi, pareq_set_v, ae_flag_none))
  gwi_func_ini(gwi, "float", "q");
  GWI_BB(gwi_func_end(gwi, pareq_get_q, ae_flag_none))
  gwi_func_ini(gwi, "float", "q");
     gwinote(gwi, "Q of the filter. sqrt(0.5) is no resonance.");
     gwi_func_arg(gwi, "float", "_q");
  GWI_BB(gwi_func_end(gwi, pareq_set_q, ae_flag_none))
  gwi_func_ini(gwi, "float", "mode");
  GWI_BB(gwi_func_end(gwi, pareq_get_mode, ae_flag_none))
  gwi_func_ini(gwi, "float", "mode");
     gwinote(gwi, "EQ mode. 0 = peak, 1 = low shelving, 2 = high shelving");
     gwi_func_arg(gwi, "float", "_mode");
  GWI_BB(gwi_func_end(gwi, pareq_set_mode, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "An extreme time-stretching algorithm by Paul Nasca Octavian");
     gwinote(gwi, "This is an implementation of the popular paulstretch algorithm used for time");
     gwinote(gwi, "stretching an audio signal to create ambient textures. Ideally, this algorithm ");
     gwinote(gwi, "is best used for stretching signals by very very long amounts. ");
     gwinote(gwi, "This version of paulstretch will take an ftable and loop through it, make");
     gwinote(gwi, "it an ideal means for creating sustained pads. ");
  DECL_OB(const Type, t_paulstretch, = gwi_class_ini(gwi, "Paulstretch", "UGen"));
  SET_FLAG(t_paulstretch, final);
  gwi_class_xtor(gwi, NULL, paulstretch_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "ftable containing audio data");
     gwi_func_arg(gwi, "ftbl", "_ft");
     gwinote(gwi, "Window size, in seconds.");
     gwi_func_arg(gwi, "float", "_windowsize");
     gwinote(gwi, "Stretch factor, 1.0 is no stretch.");
     gwi_func_arg(gwi, "float", "_stretch");
  GWI_BB(gwi_func_end(gwi, paulstretch_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Casio-style phase distortion with `pivot point` on the X axis");
     gwinote(gwi, "This module is designed to emulate the classic phase distortion synthesis technique.");
     gwinote(gwi, "From the mid 90's. The technique reads the first and second halves of the ftbl");
     gwinote(gwi, "at different rates in order to warp the waveform. For example, pdhalf can ");
     gwinote(gwi, "smoothly transition a sinewave into something approximating a sawtooth wave.");
  DECL_OB(const Type, t_pdhalf, = gwi_class_ini(gwi, "Pdhalf", "UGen"));
  SET_FLAG(t_pdhalf, final);
  gwi_class_xtor(gwi, NULL, pdhalf_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, pdhalf_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "amount");
  GWI_BB(gwi_func_end(gwi, pdhalf_get_amount, ae_flag_none))
  gwi_func_ini(gwi, "float", "amount");
     gwinote(gwi, "Amount of distortion, within the range [-1, 1]. 0 is no distortion.");
     gwi_func_arg(gwi, "float", "_amount");
  GWI_BB(gwi_func_end(gwi, pdhalf_set_amount, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Peak limiter ");
     gwinote(gwi, "This is a simple peak limiting algorithm, based off code from the Stanford");
     gwinote(gwi, "Music-424 class.");
  DECL_OB(const Type, t_peaklim, = gwi_class_ini(gwi, "Peaklim", "UGen"));
  SET_FLAG(t_peaklim, final);
  gwi_class_xtor(gwi, NULL, peaklim_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, peaklim_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, peaklim_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Attack time, in seconds");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, peaklim_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
  GWI_BB(gwi_func_end(gwi, peaklim_get_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
     gwinote(gwi, "Release time, in seconds");
     gwi_func_arg(gwi, "float", "_rel");
  GWI_BB(gwi_func_end(gwi, peaklim_set_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "thresh");
  GWI_BB(gwi_func_end(gwi, peaklim_get_thresh, ae_flag_none))
  gwi_func_ini(gwi, "float", "thresh");
     gwinote(gwi, "Threshold, in dB");
     gwi_func_arg(gwi, "float", "_thresh");
  GWI_BB(gwi_func_end(gwi, peaklim_set_thresh, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A stereo phaser");
     gwinote(gwi, "	This is a stereo phaser, generated from Faust code taken from the ");
     gwinote(gwi, "Guitarix project.");
  DECL_OB(const Type, t_phaser, = gwi_class_ini(gwi, "Phaser", "UGen"));
  SET_FLAG(t_phaser, final);
  gwi_class_xtor(gwi, NULL, phaser_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, phaser_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "MaxNotch1Freq");
  GWI_BB(gwi_func_end(gwi, phaser_get_MaxNotch1Freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "MaxNotch1Freq");
     gwinote(gwi, "Between 20 and 10000");
     gwi_func_arg(gwi, "float", "_MaxNotch1Freq");
  GWI_BB(gwi_func_end(gwi, phaser_set_MaxNotch1Freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "MinNotch1Freq");
  GWI_BB(gwi_func_end(gwi, phaser_get_MinNotch1Freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "MinNotch1Freq");
     gwinote(gwi, "Between 20 and 5000");
     gwi_func_arg(gwi, "float", "_MinNotch1Freq");
  GWI_BB(gwi_func_end(gwi, phaser_set_MinNotch1Freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "Notch_width");
  GWI_BB(gwi_func_end(gwi, phaser_get_Notch_width, ae_flag_none))
  gwi_func_ini(gwi, "float", "Notch_width");
     gwinote(gwi, "Between 10 and 5000");
     gwi_func_arg(gwi, "float", "_Notch_width");
  GWI_BB(gwi_func_end(gwi, phaser_set_Notch_width, ae_flag_none))
  gwi_func_ini(gwi, "float", "NotchFreq");
  GWI_BB(gwi_func_end(gwi, phaser_get_NotchFreq, ae_flag_none))
  gwi_func_ini(gwi, "float", "NotchFreq");
     gwinote(gwi, "Between 1.1 and 4");
     gwi_func_arg(gwi, "float", "_NotchFreq");
  GWI_BB(gwi_func_end(gwi, phaser_set_NotchFreq, ae_flag_none))
  gwi_func_ini(gwi, "float", "VibratoMode");
  GWI_BB(gwi_func_end(gwi, phaser_get_VibratoMode, ae_flag_none))
  gwi_func_ini(gwi, "float", "VibratoMode");
     gwinote(gwi, "1 or 0");
     gwi_func_arg(gwi, "float", "_VibratoMode");
  GWI_BB(gwi_func_end(gwi, phaser_set_VibratoMode, ae_flag_none))
  gwi_func_ini(gwi, "float", "depth");
  GWI_BB(gwi_func_end(gwi, phaser_get_depth, ae_flag_none))
  gwi_func_ini(gwi, "float", "depth");
     gwinote(gwi, "Between 0 and 1");
     gwi_func_arg(gwi, "float", "_depth");
  GWI_BB(gwi_func_end(gwi, phaser_set_depth, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback_gain");
  GWI_BB(gwi_func_end(gwi, phaser_get_feedback_gain, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback_gain");
     gwinote(gwi, "Between 0 and 1");
     gwi_func_arg(gwi, "float", "_feedback_gain");
  GWI_BB(gwi_func_end(gwi, phaser_set_feedback_gain, ae_flag_none))
  gwi_func_ini(gwi, "float", "invert");
  GWI_BB(gwi_func_end(gwi, phaser_get_invert, ae_flag_none))
  gwi_func_ini(gwi, "float", "invert");
     gwinote(gwi, "1 or 0");
     gwi_func_arg(gwi, "float", "_invert");
  GWI_BB(gwi_func_end(gwi, phaser_set_invert, ae_flag_none))
  gwi_func_ini(gwi, "float", "level");
  GWI_BB(gwi_func_end(gwi, phaser_get_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "level");
     gwinote(gwi, "-60 to 10 dB");
     gwi_func_arg(gwi, "float", "_level");
  GWI_BB(gwi_func_end(gwi, phaser_set_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "lfobpm");
  GWI_BB(gwi_func_end(gwi, phaser_get_lfobpm, ae_flag_none))
  gwi_func_ini(gwi, "float", "lfobpm");
     gwinote(gwi, "Between 24 and 360");
     gwi_func_arg(gwi, "float", "_lfobpm");
  GWI_BB(gwi_func_end(gwi, phaser_set_lfobpm, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Normalized sawtooth wave from 0 to 1");
     gwinote(gwi, "    Phasors are often used when building table-lookup oscillators.");
  DECL_OB(const Type, t_phasor, = gwi_class_ini(gwi, "Phasor", "UGen"));
  SET_FLAG(t_phasor, final);
  gwi_class_xtor(gwi, NULL, phasor_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "initial phase");
     gwi_func_arg(gwi, "float", "_iphs");
  GWI_BB(gwi_func_end(gwi, phasor_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, phasor_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, phasor_set_freq, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Pink pinknoise generator");
  DECL_OB(const Type, t_pinknoise, = gwi_class_ini(gwi, "Pinknoise", "UGen"));
  SET_FLAG(t_pinknoise, final);
  gwi_class_xtor(gwi, NULL, pinknoise_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, pinknoise_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, pinknoise_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude. (Value between 0-1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, pinknoise_set_amp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Pitch detection using AMDF method.");
     gwinote(gwi, "Pitchamdf tracks the pitch of signal using the AMDF (Average Magnitude Difference Function) method of ");
     gwinote(gwi, "Pitch following. ");
  DECL_OB(const Type, t_pitchamdf, = gwi_class_ini(gwi, "Pitchamdf", "UGen"));
  SET_FLAG(t_pitchamdf, final);
  gwi_class_xtor(gwi, NULL, pitchamdf_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Minimum expected frequency to detect");
     gwi_func_arg(gwi, "float", "_min");
     gwinote(gwi, "Maximum expected frequency to detect");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, pitchamdf_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Karplus-Strong plucked string instrument.");
  DECL_OB(const Type, t_pluck, = gwi_class_ini(gwi, "Pluck", "UGen"));
  SET_FLAG(t_pluck, final);
  gwi_class_xtor(gwi, NULL, pluck_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Sets the initial frequency. This frequency is used to allocate all the buffers needed for the delay. This should be the lowest frequency you plan on using.");
     gwi_func_arg(gwi, "float", "_ifreq");
  GWI_BB(gwi_func_end(gwi, pluck_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, pluck_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Variable frequency. Values less than the initial ");
     gwinote(gwi, "frequency (ifreq) will be doubled until it is greater than or equal to ifreq.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, pluck_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, pluck_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, pluck_set_amp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, " Portamento-style control signal smoothing");
     gwinote(gwi, "    Useful for smoothing out low-resolution signals and applying glissando to filters.");
  DECL_OB(const Type, t_port, = gwi_class_ini(gwi, "Port", "UGen"));
  SET_FLAG(t_port, final);
  gwi_class_xtor(gwi, NULL, port_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwi_func_arg(gwi, "float", "_htime");
  GWI_BB(gwi_func_end(gwi, port_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "htime");
  GWI_BB(gwi_func_end(gwi, port_get_htime, ae_flag_none))
  gwi_func_ini(gwi, "float", "htime");
     gwi_func_arg(gwi, "float", "_htime");
  GWI_BB(gwi_func_end(gwi, port_set_htime, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, " High-precision table-lookup posc3ilator with cubic interpolation");
  DECL_OB(const Type, t_posc3, = gwi_class_ini(gwi, "Posc3", "UGen"));
  SET_FLAG(t_posc3, final);
  gwi_class_xtor(gwi, NULL, posc3_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Wavetable to read from. Note: the size of this table must be a power of 2.");
     gwi_func_arg(gwi, "ftbl", "_tbl");
  GWI_BB(gwi_func_end(gwi, posc3_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, posc3_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency (in Hz)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, posc3_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
  GWI_BB(gwi_func_end(gwi, posc3_get_amp, ae_flag_none))
  gwi_func_ini(gwi, "float", "amp");
     gwinote(gwi, "Amplitude (typically a value between 0 and 1).");
     gwi_func_arg(gwi, "float", "_amp");
  GWI_BB(gwi_func_end(gwi, posc3_set_amp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A simple progress bar for the commandline");
     gwinote(gwi, "    Mostly ideal for offline renderings and programs with finite length. Escape characters are used to show/hide the cursor. Interruption before finishing may cause the cursor to disappear.");
  DECL_OB(const Type, t_progress, = gwi_class_ini(gwi, "Progress", "UGen"));
  SET_FLAG(t_progress, final);
  gwi_class_xtor(gwi, NULL, progress_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, progress_init, ae_flag_none))
  gwi_func_ini(gwi, "int", "nbars");
  GWI_BB(gwi_func_end(gwi, progress_get_nbars, ae_flag_none))
  gwi_func_ini(gwi, "int", "nbars");
     gwi_func_arg(gwi, "int", "_nbars");
  GWI_BB(gwi_func_end(gwi, progress_set_nbars, ae_flag_none))
  gwi_func_ini(gwi, "int", "skip");
  GWI_BB(gwi_func_end(gwi, progress_get_skip, ae_flag_none))
  gwi_func_ini(gwi, "int", "skip");
     gwinote(gwi, "How many samples to skip before checking");
     gwi_func_arg(gwi, "int", "_skip");
  GWI_BB(gwi_func_end(gwi, progress_set_skip, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Simple rhythmic notation gate generator");
     gwinote(gwi, "    Creates a gate using a simple rhythmic notation system called prop. When it reaches the end of the prop string, it will loop back to the beginning.");
     gwinote(gwi, "Prop has a few basic rules:");
     gwinote(gwi, "1. A '+' denotes a on. A '-' denotes an off (rest). They each have an equal duration of a quarter note.");
     gwinote(gwi, "2. On and off values can be strung together to create equally spaced gates: +-+--");
     gwinote(gwi, "3. When notes are enclosed in parantheses '()' following a positive integer N, their duration is reduced N times: ++2(+-)");
     gwinote(gwi, "4. When notes are enclosed in brackets '[]' following a positive integer N, their duration is scaled by a factor of N: ++2[++]");
     gwinote(gwi, "5. Parenthesis and brackets can be nested: +- 2[3(+2(++)+)]2(++)");
  DECL_OB(const Type, t_prop, = gwi_class_ini(gwi, "Prop", "UGen"));
  SET_FLAG(t_prop, final);
  gwi_class_xtor(gwi, NULL, prop_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Prop string to be parsed.");
     gwi_func_arg(gwi, "string", "_str");
  GWI_BB(gwi_func_end(gwi, prop_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "bpm");
  GWI_BB(gwi_func_end(gwi, prop_get_bpm, ae_flag_none))
  gwi_func_ini(gwi, "float", "bpm");
     gwinote(gwi, "Beats per minute of the prop string.");
     gwi_func_arg(gwi, "float", "_bpm");
  GWI_BB(gwi_func_end(gwi, prop_set_bpm, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Time-domain pitch shifter.");
  DECL_OB(const Type, t_pshift, = gwi_class_ini(gwi, "Pshift", "UGen"));
  SET_FLAG(t_pshift, final);
  gwi_class_xtor(gwi, NULL, pshift_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, pshift_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "shift");
  GWI_BB(gwi_func_end(gwi, pshift_get_shift, ae_flag_none))
  gwi_func_ini(gwi, "float", "shift");
     gwinote(gwi, "Pitch shift (in semitones), range -24/24.");
     gwi_func_arg(gwi, "float", "_shift");
  GWI_BB(gwi_func_end(gwi, pshift_set_shift, ae_flag_none))
  gwi_func_ini(gwi, "float", "window");
  GWI_BB(gwi_func_end(gwi, pshift_get_window, ae_flag_none))
  gwi_func_ini(gwi, "float", "window");
     gwinote(gwi, "Window size (in samples), max 10000");
     gwi_func_arg(gwi, "float", "_window");
  GWI_BB(gwi_func_end(gwi, pshift_set_window, ae_flag_none))
  gwi_func_ini(gwi, "float", "xfade");
  GWI_BB(gwi_func_end(gwi, pshift_get_xfade, ae_flag_none))
  gwi_func_ini(gwi, "float", "xfade");
     gwinote(gwi, "Crossfade (in samples), max 10000");
     gwi_func_arg(gwi, "float", "_xfade");
  GWI_BB(gwi_func_end(gwi, pshift_set_xfade, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Track pitch of a signal.");
     gwinote(gwi, "Ptrack is a pitch tracker, based on an algorithm originally created by");
     gwinote(gwi, "Miller Puckette.");
  DECL_OB(const Type, t_ptrack, = gwi_class_ini(gwi, "Ptrack", "UGen"));
  SET_FLAG(t_ptrack, final);
  gwi_class_xtor(gwi, NULL, ptrack_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "hop size.");
     gwi_func_arg(gwi, "int", "_ihopsize");
     gwinote(gwi, "Number of peaks.");
     gwi_func_arg(gwi, "int", "_ipeaks");
  GWI_BB(gwi_func_end(gwi, ptrack_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Random number generator with hold time.");
     gwinote(gwi, "Randh is loosely based off of the Csound opcode randomh. The design is equivalent");
     gwinote(gwi, "to scaled noise sent through a classic sample and hold module.");
  DECL_OB(const Type, t_randh, = gwi_class_ini(gwi, "Randh", "UGen"));
  SET_FLAG(t_randh, final);
  gwi_class_xtor(gwi, NULL, randh_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, randh_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
  GWI_BB(gwi_func_end(gwi, randh_get_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
     gwinote(gwi, "Minimum value to use.");
     gwi_func_arg(gwi, "float", "_min");
  GWI_BB(gwi_func_end(gwi, randh_set_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, randh_get_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
     gwinote(gwi, "Maximum value to use.");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, randh_set_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, randh_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Frequency of randomization (in Hz)");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, randh_set_freq, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Line segments between random values within a range");
  DECL_OB(const Type, t_randi, = gwi_class_ini(gwi, "Randi", "UGen"));
  SET_FLAG(t_randi, final);
  gwi_class_xtor(gwi, NULL, randi_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, randi_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
  GWI_BB(gwi_func_end(gwi, randi_get_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
     gwinote(gwi, "Minimum value");
     gwi_func_arg(gwi, "float", "_min");
  GWI_BB(gwi_func_end(gwi, randi_set_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, randi_get_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
     gwinote(gwi, "Maximum value");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, randi_set_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "cps");
  GWI_BB(gwi_func_end(gwi, randi_get_cps, ae_flag_none))
  gwi_func_ini(gwi, "float", "cps");
     gwinote(gwi, "Frequency to change values.");
     gwi_func_arg(gwi, "float", "_cps");
  GWI_BB(gwi_func_end(gwi, randi_set_cps, ae_flag_none))
  gwi_func_ini(gwi, "float", "mode");
  GWI_BB(gwi_func_end(gwi, randi_get_mode, ae_flag_none))
  gwi_func_ini(gwi, "float", "mode");
     gwinote(gwi, "Randi mode (not yet implemented yet.)");
     gwi_func_arg(gwi, "float", "_mode");
  GWI_BB(gwi_func_end(gwi, randi_set_mode, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Random values within a range");
  DECL_OB(const Type, t_random, = gwi_class_ini(gwi, "Random", "UGen"));
  SET_FLAG(t_random, final);
  gwi_class_xtor(gwi, NULL, random_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, random_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
  GWI_BB(gwi_func_end(gwi, random_get_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
     gwinote(gwi, "Minimum value.");
     gwi_func_arg(gwi, "float", "_min");
  GWI_BB(gwi_func_end(gwi, random_set_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, random_get_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
     gwinote(gwi, "Maximum value.");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, random_set_max, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A second-order resonant filter. NOTE: The output for reson appears to be very hot, so take caution when using this module.");
  DECL_OB(const Type, t_reson, = gwi_class_ini(gwi, "Reson", "UGen"));
  SET_FLAG(t_reson, final);
  gwi_class_xtor(gwi, NULL, reson_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, reson_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, reson_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Center frequency of the filter, or frequency position of the peak response.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, reson_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "bw");
  GWI_BB(gwi_func_end(gwi, reson_get_bw, ae_flag_none))
  gwi_func_ini(gwi, "float", "bw");
     gwinote(gwi, "Bandwidth of the filter.");
     gwi_func_arg(gwi, "float", "_bw");
  GWI_BB(gwi_func_end(gwi, reson_set_bw, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Signal reverser");
     gwinote(gwi, "	Reverse will store a signal inside a buffer and play it back reversed.");
  DECL_OB(const Type, t_reverse, = gwi_class_ini(gwi, "Reverse", "UGen"));
  SET_FLAG(t_reverse, final);
  gwi_class_xtor(gwi, NULL, reverse_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Delay time in seconds.");
     gwi_func_arg(gwi, "float", "_delay");
  GWI_BB(gwi_func_end(gwi, reverse_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, " 8 FDN stereo reverb");
  DECL_OB(const Type, t_revsc, = gwi_class_ini(gwi, "Revsc", "UGen"));
  SET_FLAG(t_revsc, final);
  gwi_class_xtor(gwi, NULL, revsc_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, revsc_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback");
  GWI_BB(gwi_func_end(gwi, revsc_get_feedback, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback");
     gwinote(gwi, "Value between 0-1 that sets feedback value. The larger the value, the longer the decay.");
     gwi_func_arg(gwi, "float", "_feedback");
  GWI_BB(gwi_func_end(gwi, revsc_set_feedback, ae_flag_none))
  gwi_func_ini(gwi, "float", "lpfreq");
  GWI_BB(gwi_func_end(gwi, revsc_get_lpfreq, ae_flag_none))
  gwi_func_ini(gwi, "float", "lpfreq");
     gwinote(gwi, "low pass cutoff frequency.");
     gwi_func_arg(gwi, "float", "_lpfreq");
  GWI_BB(gwi_func_end(gwi, revsc_set_lpfreq, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "RMS-averaged signal amplitude");
     gwinote(gwi, "    Perform `root-mean-square` on a signal to get overall amplitude of a signal. The output signal looks similar to that of a classic VU meter.");
  DECL_OB(const Type, t_rms, = gwi_class_ini(gwi, "Rms", "UGen"));
  SET_FLAG(t_rms, final);
  gwi_class_xtor(gwi, NULL, rms_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, rms_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "ihp");
  GWI_BB(gwi_func_end(gwi, rms_get_ihp, ae_flag_none))
  gwi_func_ini(gwi, "float", "ihp");
     gwinote(gwi, "Half-power point (in Hz) of internal lowpass filter. This parameter is fixed at 10Hz and is not yet mutable.");
     gwi_func_arg(gwi, "float", "_ihp");
  GWI_BB(gwi_func_end(gwi, rms_set_ihp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Trigger based beat-repeat stuttering effect");
     gwinote(gwi, "    When the input is a non-zero value, rpt will load up the buffer and loop a certain number of times. Speed and repeat amounts can be set with the sp_rpt_set function.");
  DECL_OB(const Type, t_rpt, = gwi_class_ini(gwi, "Rpt", "UGen"));
  SET_FLAG(t_rpt, final);
  gwi_class_xtor(gwi, NULL, rpt_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Maximum delay duration in seconds. This will set the buffer size.");
     gwi_func_arg(gwi, "float", "_maxdur");
  GWI_BB(gwi_func_end(gwi, rpt_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Random spline curve generator");
     gwinote(gwi, "This is a function that generates random spline curves. This signal generator");
     gwinote(gwi, "is ideal for control signals. Curves are quite smooth when cps_min is not");
     gwinote(gwi, "too different from cps_max. When the range of these two is big, ");
     gwinote(gwi, "some discontinuity may occur. Due to the interpolation, the output ");
     gwinote(gwi, "may be greater than the range values. ");
  DECL_OB(const Type, t_rspline, = gwi_class_ini(gwi, "Rspline", "UGen"));
  SET_FLAG(t_rspline, final);
  gwi_class_xtor(gwi, NULL, rspline_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, rspline_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
  GWI_BB(gwi_func_end(gwi, rspline_get_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
     gwinote(gwi, "Minimum range.");
     gwi_func_arg(gwi, "float", "_min");
  GWI_BB(gwi_func_end(gwi, rspline_set_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, rspline_get_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
     gwinote(gwi, "Maximum range");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, rspline_set_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "cps_min");
  GWI_BB(gwi_func_end(gwi, rspline_get_cps_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "cps_min");
     gwi_func_arg(gwi, "float", "_cps_min");
  GWI_BB(gwi_func_end(gwi, rspline_set_cps_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "cps_max");
  GWI_BB(gwi_func_end(gwi, rspline_get_cps_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "cps_max");
     gwi_func_arg(gwi, "float", "_cps_max");
  GWI_BB(gwi_func_end(gwi, rspline_set_cps_max, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Classic sample and hold");
  DECL_OB(const Type, t_samphold, = gwi_class_ini(gwi, "Samphold", "UGen"));
  SET_FLAG(t_samphold, final);
  gwi_class_xtor(gwi, NULL, samphold_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, samphold_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Soft clip saturating distortion, based on examples from Abel/Berners' Music 424 course at Stanford.");
  DECL_OB(const Type, t_saturator, = gwi_class_ini(gwi, "Saturator", "UGen"));
  SET_FLAG(t_saturator, final);
  gwi_class_xtor(gwi, NULL, saturator_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, saturator_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "drive");
  GWI_BB(gwi_func_end(gwi, saturator_get_drive, ae_flag_none))
  gwi_func_ini(gwi, "float", "drive");
     gwinote(gwi, "Input gain into the distortion section, in decibels. Controls overall amount of distortion.");
     gwi_func_arg(gwi, "float", "_drive");
  GWI_BB(gwi_func_end(gwi, saturator_set_drive, ae_flag_none))
  gwi_func_ini(gwi, "float", "dcoffset");
  GWI_BB(gwi_func_end(gwi, saturator_get_dcoffset, ae_flag_none))
  gwi_func_ini(gwi, "float", "dcoffset");
     gwinote(gwi, "Constant linear offset applied to the signal. A small offset will introduce odd harmonics into the distoration spectrum, whereas a zero offset will have only even harmonics.");
     gwi_func_arg(gwi, "float", "_dcoffset");
  GWI_BB(gwi_func_end(gwi, saturator_set_dcoffset, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Bipolar Scale");
     gwinote(gwi, "    This module scales from unipolar [0, 1] to another range defined by min and max.");
  DECL_OB(const Type, t_scale, = gwi_class_ini(gwi, "Scale", "UGen"));
  SET_FLAG(t_scale, final);
  gwi_class_xtor(gwi, NULL, scale_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, scale_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
  GWI_BB(gwi_func_end(gwi, scale_get_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
     gwinote(gwi, "Minimum value to scale to.");
     gwi_func_arg(gwi, "float", "_min");
  GWI_BB(gwi_func_end(gwi, scale_set_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, scale_get_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
     gwinote(gwi, "Maximum value to scale to.");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, scale_set_max, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Delays a signal by a number of samples.");
  DECL_OB(const Type, t_sdelay, = gwi_class_ini(gwi, "Sdelay", "UGen"));
  SET_FLAG(t_sdelay, final);
  gwi_class_xtor(gwi, NULL, sdelay_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Size of delay (in samples)");
     gwi_func_arg(gwi, "float", "_size");
  GWI_BB(gwi_func_end(gwi, sdelay_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Yet another slice-based sample player.");
     gwinote(gwi, "This module takes in an audio buffer and a table with slice points. ");
  DECL_OB(const Type, t_slice, = gwi_class_ini(gwi, "Slice", "UGen"));
  SET_FLAG(t_slice, final);
  gwi_class_xtor(gwi, NULL, slice_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "A table containing slice points, in samples");
     gwi_func_arg(gwi, "ftbl", "_vals");
     gwinote(gwi, "The buffer containing the audio samples.");
     gwi_func_arg(gwi, "ftbl", "_buf");
  GWI_BB(gwi_func_end(gwi, slice_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "id");
  GWI_BB(gwi_func_end(gwi, slice_get_id, ae_flag_none))
  gwi_func_ini(gwi, "float", "id");
     gwinote(gwi, "Value position.");
     gwi_func_arg(gwi, "float", "_id");
  GWI_BB(gwi_func_end(gwi, slice_set_id, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Smooth variable delay line without varispeed pitch.");
     gwinote(gwi, "Smooth delay is based off the sdelay module in Faust. The smooth delay ");
     gwinote(gwi, "algorithm involves a double delay line. Any time the delay time changes, ");
     gwinote(gwi, "the delay time of buffer not heard changes, then is crossfaded to ");
     gwinote(gwi, "that buffer.");
  DECL_OB(const Type, t_smoothdelay, = gwi_class_ini(gwi, "Smoothdelay", "UGen"));
  SET_FLAG(t_smoothdelay, final);
  gwi_class_xtor(gwi, NULL, smoothdelay_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Maximum delay time (in seconds)");
     gwi_func_arg(gwi, "float", "_maxdel");
     gwinote(gwi, "interpolation time (in samples)");
     gwi_func_arg(gwi, "int", "_interp");
  GWI_BB(gwi_func_end(gwi, smoothdelay_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback");
  GWI_BB(gwi_func_end(gwi, smoothdelay_get_feedback, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback");
     gwi_func_arg(gwi, "float", "_feedback");
  GWI_BB(gwi_func_end(gwi, smoothdelay_set_feedback, ae_flag_none))
  gwi_func_ini(gwi, "float", "del");
  GWI_BB(gwi_func_end(gwi, smoothdelay_get_del, ae_flag_none))
  gwi_func_ini(gwi, "float", "del");
     gwi_func_arg(gwi, "float", "_del");
  GWI_BB(gwi_func_end(gwi, smoothdelay_set_del, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Stream a Soundpipe Audio File");
     gwinote(gwi, "Similar to sp_diskin, sp_spa will stream a file in the internal soundpipe");
     gwinote(gwi, "audio format. Such a format is useful for instances where you need to read");
     gwinote(gwi, "audio files, but can't use libsndfile. ");
  DECL_OB(const Type, t_spa, = gwi_class_ini(gwi, "Spa", "UGen"));
  SET_FLAG(t_spa, final);
  gwi_class_xtor(gwi, NULL, spa_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Filename of SPA file");
     gwi_func_arg(gwi, "string", "_filename");
  GWI_BB(gwi_func_end(gwi, spa_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Writes signal to spa file.");
  DECL_OB(const Type, t_sparec, = gwi_class_ini(gwi, "Sparec", "UGen"));
  SET_FLAG(t_sparec, final);
  gwi_class_xtor(gwi, NULL, sparec_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Filename to write to");
     gwi_func_arg(gwi, "string", "_filename");
  GWI_BB(gwi_func_end(gwi, sparec_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "String resonator filter");
  DECL_OB(const Type, t_streson, = gwi_class_ini(gwi, "Streson", "UGen"));
  SET_FLAG(t_streson, final);
  gwi_class_xtor(gwi, NULL, streson_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, streson_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
  GWI_BB(gwi_func_end(gwi, streson_get_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "freq");
     gwinote(gwi, "Fundamental frequency of string.");
     gwi_func_arg(gwi, "float", "_freq");
  GWI_BB(gwi_func_end(gwi, streson_set_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "fdbgain");
  GWI_BB(gwi_func_end(gwi, streson_get_fdbgain, ae_flag_none))
  gwi_func_ini(gwi, "float", "fdbgain");
     gwinote(gwi, "Feedback amount (value between 0-1)");
     gwi_func_arg(gwi, "float", "_fdbgain");
  GWI_BB(gwi_func_end(gwi, streson_set_fdbgain, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Switch between two signals");
     gwinote(gwi, "    By default, the incoming first signal is selected. When triggered, the output signal will switch to the other signal.");
  DECL_OB(const Type, t_switch, = gwi_class_ini(gwi, "Switch", "UGen"));
  SET_FLAG(t_switch, final);
  gwi_class_xtor(gwi, NULL, switch_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, switch_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Table ");
     gwinote(gwi, "        Read through an sp_ftbl with linear interpolation.");
     gwinote(gwi, "    ");
  DECL_OB(const Type, t_tabread, = gwi_class_ini(gwi, "Tabread", "UGen"));
  SET_FLAG(t_tabread, final);
  gwi_class_xtor(gwi, NULL, tabread_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "A properly allocated table (using a function like sp_gen_file).");
     gwi_func_arg(gwi, "ftbl", "_ft");
     gwinote(gwi, "1 = scaled index, 0 = unscaled index");
     gwi_func_arg(gwi, "float", "_mode");
  GWI_BB(gwi_func_end(gwi, tabread_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "index");
  GWI_BB(gwi_func_end(gwi, tabread_get_index, ae_flag_none))
  gwi_func_ini(gwi, "float", "index");
     gwinote(gwi, "index position, either scaled or unscaled with mode parameter");
     gwi_func_arg(gwi, "float", "_index");
  GWI_BB(gwi_func_end(gwi, tabread_set_index, ae_flag_none))
  gwi_func_ini(gwi, "float", "offset");
  GWI_BB(gwi_func_end(gwi, tabread_get_offset, ae_flag_none))
  gwi_func_ini(gwi, "float", "offset");
     gwinote(gwi, "Offset from beginning of ftable. If the mode is scaled, then it is in range 0-1, other wise it is the index position.");
     gwi_func_arg(gwi, "float", "_offset");
  GWI_BB(gwi_func_end(gwi, tabread_set_offset, ae_flag_none))
  gwi_func_ini(gwi, "float", "wrap");
  GWI_BB(gwi_func_end(gwi, tabread_get_wrap, ae_flag_none))
  gwi_func_ini(gwi, "float", "wrap");
     gwinote(gwi, "Enable wraparound. 1 = on; 0 = 0ff.");
     gwi_func_arg(gwi, "float", "_wrap");
  GWI_BB(gwi_func_end(gwi, tabread_set_wrap, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Triggerable classic ADSR envelope");
  DECL_OB(const Type, t_tadsr, = gwi_class_ini(gwi, "Tadsr", "UGen"));
  SET_FLAG(t_tadsr, final);
  gwi_class_xtor(gwi, NULL, tadsr_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tadsr_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, tadsr_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Attack time");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, tadsr_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
  GWI_BB(gwi_func_end(gwi, tadsr_get_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "dec");
     gwinote(gwi, "Decay time");
     gwi_func_arg(gwi, "float", "_dec");
  GWI_BB(gwi_func_end(gwi, tadsr_set_dec, ae_flag_none))
  gwi_func_ini(gwi, "float", "sus");
  GWI_BB(gwi_func_end(gwi, tadsr_get_sus, ae_flag_none))
  gwi_func_ini(gwi, "float", "sus");
     gwinote(gwi, "Sustain Level");
     gwi_func_arg(gwi, "float", "_sus");
  GWI_BB(gwi_func_end(gwi, tadsr_set_sus, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
  GWI_BB(gwi_func_end(gwi, tadsr_get_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
     gwinote(gwi, "release");
     gwi_func_arg(gwi, "float", "_rel");
  GWI_BB(gwi_func_end(gwi, tadsr_set_rel, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A high talkbox emulation, simular to a vocoder.");
     gwinote(gwi, "This is the talkbox plugin ported from the MDA plugin suite. In many ways,");
     gwinote(gwi, "this Talkbox functions like a vocoder: it takes in a source signal (usually");
     gwinote(gwi, "speech), which then excites an excitation signal ");
     gwinote(gwi, "(usually a harmonically rich signal like a saw wave). This particular algorithm");
     gwinote(gwi, "uses linear-predictive coding (LPC), making speech intelligibility better ");
     gwinote(gwi, "than most vocoder implementations.");
  DECL_OB(const Type, t_talkbox, = gwi_class_ini(gwi, "Talkbox", "UGen"));
  SET_FLAG(t_talkbox, final);
  gwi_class_xtor(gwi, NULL, talkbox_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, talkbox_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "quality");
  GWI_BB(gwi_func_end(gwi, talkbox_get_quality, ae_flag_none))
  gwi_func_ini(gwi, "float", "quality");
     gwinote(gwi, "Quality of the talkbox sound. 0=lowest fidelity. 1=highest fidelity");
     gwi_func_arg(gwi, "float", "_quality");
  GWI_BB(gwi_func_end(gwi, talkbox_set_quality, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Record a signal to an ftable.");
     gwinote(gwi, "This module will write audio-rate signals to a preallocated soundpipe ftable. ");
     gwinote(gwi, "Every time record is enabled, it will got back to index 0 overwrite any");
     gwinote(gwi, "previous information that was on it. ");
  DECL_OB(const Type, t_tblrec, = gwi_class_ini(gwi, "Tblrec", "UGen"));
  SET_FLAG(t_tblrec, final);
  gwi_class_xtor(gwi, NULL, tblrec_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwi_func_arg(gwi, "ftbl", "_bar");
  GWI_BB(gwi_func_end(gwi, tblrec_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Emulation of the Roland TB-303 filter");
  DECL_OB(const Type, t_tbvcf, = gwi_class_ini(gwi, "Tbvcf", "UGen"));
  SET_FLAG(t_tbvcf, final);
  gwi_class_xtor(gwi, NULL, tbvcf_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tbvcf_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "fco");
  GWI_BB(gwi_func_end(gwi, tbvcf_get_fco, ae_flag_none))
  gwi_func_ini(gwi, "float", "fco");
     gwinote(gwi, "Filter cutoff frequency");
     gwi_func_arg(gwi, "float", "_fco");
  GWI_BB(gwi_func_end(gwi, tbvcf_set_fco, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
  GWI_BB(gwi_func_end(gwi, tbvcf_get_res, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
     gwinote(gwi, "Resonance");
     gwi_func_arg(gwi, "float", "_res");
  GWI_BB(gwi_func_end(gwi, tbvcf_set_res, ae_flag_none))
  gwi_func_ini(gwi, "float", "dist");
  GWI_BB(gwi_func_end(gwi, tbvcf_get_dist, ae_flag_none))
  gwi_func_ini(gwi, "float", "dist");
     gwinote(gwi, "Distortion. Value is typically 2.0; deviation from this can cause stability issues. ");
     gwi_func_arg(gwi, "float", "_dist");
  GWI_BB(gwi_func_end(gwi, tbvcf_set_dist, ae_flag_none))
  gwi_func_ini(gwi, "float", "asym");
  GWI_BB(gwi_func_end(gwi, tbvcf_get_asym, ae_flag_none))
  gwi_func_ini(gwi, "float", "asym");
     gwinote(gwi, "Asymmetry of resonance. Value is between 0-1");
     gwi_func_arg(gwi, "float", "_asym");
  GWI_BB(gwi_func_end(gwi, tbvcf_set_asym, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Trigger divider.");
     gwinote(gwi, "This module will take in a trigger signal, and output a trigger signal");
     gwinote(gwi, "every N times.");
     gwinote(gwi, "For instance, when N = 3:");
     gwinote(gwi, "in: * * * * * * * * *");
     gwinote(gwi, "out *     *     *   ");
  DECL_OB(const Type, t_tdiv, = gwi_class_ini(gwi, "Tdiv", "UGen"));
  SET_FLAG(t_tdiv, final);
  gwi_class_xtor(gwi, NULL, tdiv_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tdiv_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "num");
  GWI_BB(gwi_func_end(gwi, tdiv_get_num, ae_flag_none))
  gwi_func_ini(gwi, "float", "num");
     gwinote(gwi, "Triggers every N times.");
     gwi_func_arg(gwi, "float", "_num");
  GWI_BB(gwi_func_end(gwi, tdiv_set_num, ae_flag_none))
  gwi_func_ini(gwi, "float", "offset");
  GWI_BB(gwi_func_end(gwi, tdiv_get_offset, ae_flag_none))
  gwi_func_ini(gwi, "float", "offset");
     gwinote(gwi, "Offset amoutn.");
     gwi_func_arg(gwi, "float", "_offset");
  GWI_BB(gwi_func_end(gwi, tdiv_set_offset, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Trigger based linear AHD envelope generator");
  DECL_OB(const Type, t_tenv, = gwi_class_ini(gwi, "Tenv", "UGen"));
  SET_FLAG(t_tenv, final);
  gwi_class_xtor(gwi, NULL, tenv_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tenv_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, tenv_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Attack time, in seconds.");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, tenv_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "hold");
  GWI_BB(gwi_func_end(gwi, tenv_get_hold, ae_flag_none))
  gwi_func_ini(gwi, "float", "hold");
     gwinote(gwi, "Hold time, in seconds.");
     gwi_func_arg(gwi, "float", "_hold");
  GWI_BB(gwi_func_end(gwi, tenv_set_hold, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
  GWI_BB(gwi_func_end(gwi, tenv_get_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
     gwinote(gwi, "Release time, in seconds.");
     gwi_func_arg(gwi, "float", "_rel");
  GWI_BB(gwi_func_end(gwi, tenv_set_rel, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Linear 2-stage Attack/Release envelope generator");
     gwinote(gwi, "    This envelope takes 2 triggers. When triggered once,");
     gwinote(gwi, "the envelope will rise to 1 according to the attack time. When triggered again, it will decay to 0 according to");
     gwinote(gwi, "the decay time.");
  DECL_OB(const Type, t_tenv2, = gwi_class_ini(gwi, "Tenv2", "UGen"));
  SET_FLAG(t_tenv2, final);
  gwi_class_xtor(gwi, NULL, tenv2_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tenv2_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, tenv2_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Attack time (in seconds).");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, tenv2_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
  GWI_BB(gwi_func_end(gwi, tenv2_get_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
     gwinote(gwi, "Release time (in seconds).");
     gwi_func_arg(gwi, "float", "_rel");
  GWI_BB(gwi_func_end(gwi, tenv2_set_rel, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Trigger based exponential AHD envelope generator.");
     gwinote(gwi, "    This envelope generator emulates the exponential behavior of analogue ");
     gwinote(gwi, "envelope generators by passing a gate signal (whose duration is specified via");
     gwinote(gwi, "the hold parameter) through a one-pole filter, whose filter coefficeints are");
     gwinote(gwi, "calculated in terms of tau.  ");
     gwinote(gwi, "    ");
  DECL_OB(const Type, t_tenvx, = gwi_class_ini(gwi, "Tenvx", "UGen"));
  SET_FLAG(t_tenvx, final);
  gwi_class_xtor(gwi, NULL, tenvx_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tenvx_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, tenvx_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Tau attack time, in seconds. Must be non-zero.");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, tenvx_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "hold");
  GWI_BB(gwi_func_end(gwi, tenvx_get_hold, ae_flag_none))
  gwi_func_ini(gwi, "float", "hold");
     gwinote(gwi, "Hold time, in seconds. The duration of the gate signal.");
     gwi_func_arg(gwi, "float", "_hold");
  GWI_BB(gwi_func_end(gwi, tenvx_set_hold, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
  GWI_BB(gwi_func_end(gwi, tenvx_get_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
     gwinote(gwi, "Tau release time, in seconds. Must be non-zero.");
     gwi_func_arg(gwi, "float", "_rel");
  GWI_BB(gwi_func_end(gwi, tenvx_set_rel, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A triggerable gate.");
  DECL_OB(const Type, t_tgate, = gwi_class_ini(gwi, "Tgate", "UGen"));
  SET_FLAG(t_tgate, final);
  gwi_class_xtor(gwi, NULL, tgate_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tgate_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "time");
  GWI_BB(gwi_func_end(gwi, tgate_get_time, ae_flag_none))
  gwi_func_ini(gwi, "float", "time");
     gwinote(gwi, "Duration of the gate (in seconds)");
     gwi_func_arg(gwi, "float", "_time");
  GWI_BB(gwi_func_end(gwi, tgate_set_time, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Trigger generator for signals that cross a given threshold. ");
  DECL_OB(const Type, t_thresh, = gwi_class_ini(gwi, "Thresh", "UGen"));
  SET_FLAG(t_thresh, final);
  gwi_class_xtor(gwi, NULL, thresh_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, thresh_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "thresh");
  GWI_BB(gwi_func_end(gwi, thresh_get_thresh, ae_flag_none))
  gwi_func_ini(gwi, "float", "thresh");
     gwinote(gwi, "Threshold level");
     gwi_func_arg(gwi, "float", "_thresh");
  GWI_BB(gwi_func_end(gwi, thresh_set_thresh, ae_flag_none))
  gwi_func_ini(gwi, "int", "mode");
  GWI_BB(gwi_func_end(gwi, thresh_get_mode, ae_flag_none))
  gwi_func_ini(gwi, "int", "mode");
     gwinote(gwi, "Sets when to detect theshold crossings. 0 = from below. 1 = from above. 2 = from above/below");
     gwi_func_arg(gwi, "int", "_mode");
  GWI_BB(gwi_func_end(gwi, thresh_set_mode, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Tap-tempo like timer");
     gwinote(gwi, "When triggered, timer will begin an internal stopwatch until it is triggered again.");
     gwinote(gwi, "The output of the timer will be the time elapsed in seconds.");
  DECL_OB(const Type, t_timer, = gwi_class_ini(gwi, "Timer", "UGen"));
  SET_FLAG(t_timer, final);
  gwi_class_xtor(gwi, NULL, timer_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, timer_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Similar to in, tin reads SPFLOATs (by default, this is a 4 byte binary float) from standard input every time it is triggered. behaves like a sample and hold, retaining the previous value (initial set to 0) until triggered. ");
  DECL_OB(const Type, t_tin, = gwi_class_ini(gwi, "Tin", "UGen"));
  SET_FLAG(t_tin, final);
  gwi_class_xtor(gwi, NULL, tin_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tin_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "First-order recursive lowpass filter");
  DECL_OB(const Type, t_tone, = gwi_class_ini(gwi, "Tone", "UGen"));
  SET_FLAG(t_tone, final);
  gwi_class_xtor(gwi, NULL, tone_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, tone_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "hp");
  GWI_BB(gwi_func_end(gwi, tone_get_hp, ae_flag_none))
  gwi_func_ini(gwi, "float", "hp");
     gwinote(gwi, "The response curve's half power point (aka cutoff frequency).");
     gwi_func_arg(gwi, "float", "_hp");
  GWI_BB(gwi_func_end(gwi, tone_set_hp, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Triggered random number generator.");
  DECL_OB(const Type, t_trand, = gwi_class_ini(gwi, "Trand", "UGen"));
  SET_FLAG(t_trand, final);
  gwi_class_xtor(gwi, NULL, trand_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, trand_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
  GWI_BB(gwi_func_end(gwi, trand_get_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "min");
     gwinote(gwi, "Minimum value to use.");
     gwi_func_arg(gwi, "float", "_min");
  GWI_BB(gwi_func_end(gwi, trand_set_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, trand_get_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "max");
     gwinote(gwi, "Maximum value to use.");
     gwi_func_arg(gwi, "float", "_max");
  GWI_BB(gwi_func_end(gwi, trand_set_max, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "This module creates a series of line segments. ");
  DECL_OB(const Type, t_tseg, = gwi_class_ini(gwi, "Tseg", "UGen"));
  SET_FLAG(t_tseg, final);
  gwi_class_xtor(gwi, NULL, tseg_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Beginning value.");
     gwi_func_arg(gwi, "float", "_ibeg");
  GWI_BB(gwi_func_end(gwi, tseg_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "end");
  GWI_BB(gwi_func_end(gwi, tseg_get_end, ae_flag_none))
  gwi_func_ini(gwi, "float", "end");
     gwinote(gwi, "End parameter to go to.");
     gwi_func_arg(gwi, "float", "_end");
  GWI_BB(gwi_func_end(gwi, tseg_set_end, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
  GWI_BB(gwi_func_end(gwi, tseg_get_dur, ae_flag_none))
  gwi_func_ini(gwi, "float", "dur");
     gwinote(gwi, "duration to rise to (in seconds).");
     gwi_func_arg(gwi, "float", "_dur");
  GWI_BB(gwi_func_end(gwi, tseg_set_dur, ae_flag_none))
  gwi_func_ini(gwi, "float", "type");
  GWI_BB(gwi_func_end(gwi, tseg_get_type, ae_flag_none))
  gwi_func_ini(gwi, "float", "type");
     gwinote(gwi, "The type of line, which determines slope of line");
     gwi_func_arg(gwi, "float", "_type");
  GWI_BB(gwi_func_end(gwi, tseg_set_type, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Function table looper");
     gwinote(gwi, "    TSeq runs through values in an ftable. It will change values when the trigger input is a non-zero value, and wrap around when it reaches the end.");
  DECL_OB(const Type, t_tseq, = gwi_class_ini(gwi, "Tseq", "UGen"));
  SET_FLAG(t_tseq, final);
  gwi_class_xtor(gwi, NULL, tseq_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "An ftable of values");
     gwi_func_arg(gwi, "ftbl", "_ft");
  GWI_BB(gwi_func_end(gwi, tseq_init, ae_flag_none))
  gwi_func_ini(gwi, "int", "shuf");
  GWI_BB(gwi_func_end(gwi, tseq_get_shuf, ae_flag_none))
  gwi_func_ini(gwi, "int", "shuf");
     gwinote(gwi, "When shuf is non-zero, randomly pick a value rather than cycle through sequentially.");
     gwi_func_arg(gwi, "int", "_shuf");
  GWI_BB(gwi_func_end(gwi, tseq_set_shuf, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Delay line with cubic interpolation");
  DECL_OB(const Type, t_vdelay, = gwi_class_ini(gwi, "Vdelay", "UGen"));
  SET_FLAG(t_vdelay, final);
  gwi_class_xtor(gwi, NULL, vdelay_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "The maximum delay time, in seconds.");
     gwi_func_arg(gwi, "float", "_maxdel");
  GWI_BB(gwi_func_end(gwi, vdelay_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "del");
  GWI_BB(gwi_func_end(gwi, vdelay_get_del, ae_flag_none))
  gwi_func_ini(gwi, "float", "del");
     gwinote(gwi, "Delay time (in seconds) that can be changed during performance. This value must not exceed the maximum delay time.");
     gwi_func_arg(gwi, "float", "_del");
  GWI_BB(gwi_func_end(gwi, vdelay_set_del, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback");
  GWI_BB(gwi_func_end(gwi, vdelay_get_feedback, ae_flag_none))
  gwi_func_ini(gwi, "float", "feedback");
     gwinote(gwi, "Amount of feedback in delay line. Should be in range 0-1.");
     gwi_func_arg(gwi, "float", "_feedback");
  GWI_BB(gwi_func_end(gwi, vdelay_set_feedback, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "A vocal tract physical model.");
     gwinote(gwi, "Based on the Pink Trombone algorithm by Neil Thapen, Voc implements a physical ");
     gwinote(gwi, "model of the vocal tract glottal pulse wave. The tract model is based on the ");
     gwinote(gwi, "classic Kelly-Lochbaum");
     gwinote(gwi, "segmented cylindrical 1d waveguide model, and the glottal pulse wave is a");
     gwinote(gwi, "LF glottal pulse model. ");
     gwinote(gwi, "The soundpipe source code for Voc is generated via ctangle, one half of the");
     gwinote(gwi, "literate documentation system known CWEB. The CWEB are maintained in a ");
     gwinote(gwi, "separate repository. They are hosted on github here: ");
     gwinote(gwi, "http://www.github.com/paulbatchelor/voc");
     gwinote(gwi, "This documentation is a stub. For a full overview on proper usage, consult");
     gwinote(gwi, "the `Top-level functions` section of the documented code, a copy of which");
     gwinote(gwi, "can be found at the Voc project page pbat.ch/proj/voc, or generate the PDF");
     gwinote(gwi, "from the github page described above.");
  DECL_OB(const Type, t_voc, = gwi_class_ini(gwi, "Voc", "UGen"));
  SET_FLAG(t_voc, final);
  gwi_class_xtor(gwi, NULL, voc_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, voc_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "16-band channel vocoder");
  DECL_OB(const Type, t_vocoder, = gwi_class_ini(gwi, "Vocoder", "UGen"));
  SET_FLAG(t_vocoder, final);
  gwi_class_xtor(gwi, NULL, vocoder_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, vocoder_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
  GWI_BB(gwi_func_end(gwi, vocoder_get_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "atk");
     gwinote(gwi, "Attack time . (Range 0.001 and 0.5 seconds)");
     gwi_func_arg(gwi, "float", "_atk");
  GWI_BB(gwi_func_end(gwi, vocoder_set_atk, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
  GWI_BB(gwi_func_end(gwi, vocoder_get_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "rel");
     gwinote(gwi, "Release time");
     gwi_func_arg(gwi, "float", "_rel");
  GWI_BB(gwi_func_end(gwi, vocoder_set_rel, ae_flag_none))
  gwi_func_ini(gwi, "float", "bwratio");
  GWI_BB(gwi_func_end(gwi, vocoder_get_bwratio, ae_flag_none))
  gwi_func_ini(gwi, "float", "bwratio");
     gwinote(gwi, "Coeffecient to adjust the bandwidth of each band (0.1 - 2)");
     gwi_func_arg(gwi, "float", "_bwratio");
  GWI_BB(gwi_func_end(gwi, vocoder_set_bwratio, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Simple Time-stretching from repeating wavecyles");
     gwinote(gwi, "This module looks for zero-crossings and repeats them by a integer factor.");
     gwinote(gwi, "While a crude means for time stretching, it is a very aesthetically pleasing");
     gwinote(gwi, "effect to use on sounds and often produces a `wet` sound.");
     gwinote(gwi, "The waveset algorithm was originally created by Trevor Wishart for the Composer");
     gwinote(gwi, "Desktop Project (CDP), and was then ported to Csound. ");
  DECL_OB(const Type, t_waveset, = gwi_class_ini(gwi, "Waveset", "UGen"));
  SET_FLAG(t_waveset, final);
  gwi_class_xtor(gwi, NULL, waveset_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Length of buffer (in seconds).");
     gwi_func_arg(gwi, "float", "_ilen");
  GWI_BB(gwi_func_end(gwi, waveset_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "rep");
  GWI_BB(gwi_func_end(gwi, waveset_get_rep, ae_flag_none))
  gwi_func_ini(gwi, "float", "rep");
     gwinote(gwi, "Number of repeats.");
     gwi_func_arg(gwi, "float", "_rep");
  GWI_BB(gwi_func_end(gwi, waveset_set_rep, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Reads a mono WAV file.");
     gwinote(gwi, "This module reads a mono WAV file from disk. It uses the public-domain ");
     gwinote(gwi, "dr_wav library for decoding, so it can be a good substitute for libsndfile.");
  DECL_OB(const Type, t_wavin, = gwi_class_ini(gwi, "Wavin", "UGen"));
  SET_FLAG(t_wavin, final);
  gwi_class_xtor(gwi, NULL, wavin_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "Filename");
     gwi_func_arg(gwi, "string", "_filename");
  GWI_BB(gwi_func_end(gwi, wavin_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "Writes a mono signal to a WAV file.");
     gwinote(gwi, "This module uses the public-domain dr_wav library to write WAV files");
     gwinote(gwi, "to disk. This module is ideal for instances where GPL-licensed libsndfile ");
     gwinote(gwi, "cannot be used for legal reasons.");
  DECL_OB(const Type, t_wavout, = gwi_class_ini(gwi, "Wavout", "UGen"));
  SET_FLAG(t_wavout, final);
  gwi_class_xtor(gwi, NULL, wavout_dtor);
  gwi_func_ini(gwi, "auto", "new");
     gwinote(gwi, "The filename of the output file.");
     gwi_func_arg(gwi, "string", "_filename");
  GWI_BB(gwi_func_end(gwi, wavout_init, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, " Analogue model of the Korg 35 Lowpass Filter");
     gwinote(gwi, "Original port done by Will Pirkle:");
     gwinote(gwi, "http://www.willpirkle.com/Downloads/AN-5Korg35_V3.pdf");
  DECL_OB(const Type, t_wpkorg35, = gwi_class_ini(gwi, "Wpkorg35", "UGen"));
  SET_FLAG(t_wpkorg35, final);
  gwi_class_xtor(gwi, NULL, wpkorg35_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, wpkorg35_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "cutoff");
  GWI_BB(gwi_func_end(gwi, wpkorg35_get_cutoff, ae_flag_none))
  gwi_func_ini(gwi, "float", "cutoff");
     gwinote(gwi, "Filter cutoff");
     gwi_func_arg(gwi, "float", "_cutoff");
  GWI_BB(gwi_func_end(gwi, wpkorg35_set_cutoff, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
  GWI_BB(gwi_func_end(gwi, wpkorg35_get_res, ae_flag_none))
  gwi_func_ini(gwi, "float", "res");
     gwinote(gwi, "Filter resonance (should be between 0-2)");
     gwi_func_arg(gwi, "float", "_res");
  GWI_BB(gwi_func_end(gwi, wpkorg35_set_res, ae_flag_none))
  gwi_func_ini(gwi, "float", "saturation");
  GWI_BB(gwi_func_end(gwi, wpkorg35_get_saturation, ae_flag_none))
  gwi_func_ini(gwi, "float", "saturation");
     gwinote(gwi, "Filter saturation.");
     gwi_func_arg(gwi, "float", "_saturation");
  GWI_BB(gwi_func_end(gwi, wpkorg35_set_saturation, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

     gwinote(gwi, "8 FDN stereo zitareverb algorithm, imported from Faust.");
  DECL_OB(const Type, t_zitarev, = gwi_class_ini(gwi, "Zitarev", "UGen"));
  SET_FLAG(t_zitarev, final);
  gwi_class_xtor(gwi, NULL, zitarev_dtor);
  gwi_func_ini(gwi, "auto", "new");
  GWI_BB(gwi_func_end(gwi, zitarev_init, ae_flag_none))
  gwi_func_ini(gwi, "float", "in_delay");
  GWI_BB(gwi_func_end(gwi, zitarev_get_in_delay, ae_flag_none))
  gwi_func_ini(gwi, "float", "in_delay");
     gwinote(gwi, "Delay in ms before reverberation begins.");
     gwi_func_arg(gwi, "float", "_in_delay");
  GWI_BB(gwi_func_end(gwi, zitarev_set_in_delay, ae_flag_none))
  gwi_func_ini(gwi, "float", "lf_x");
  GWI_BB(gwi_func_end(gwi, zitarev_get_lf_x, ae_flag_none))
  gwi_func_ini(gwi, "float", "lf_x");
     gwinote(gwi, "Crossover frequency separating low and middle frequencies (Hz).");
     gwi_func_arg(gwi, "float", "_lf_x");
  GWI_BB(gwi_func_end(gwi, zitarev_set_lf_x, ae_flag_none))
  gwi_func_ini(gwi, "float", "rt60_low");
  GWI_BB(gwi_func_end(gwi, zitarev_get_rt60_low, ae_flag_none))
  gwi_func_ini(gwi, "float", "rt60_low");
     gwinote(gwi, "Time (in seconds) to decay 60db in low-frequency band.");
     gwi_func_arg(gwi, "float", "_rt60_low");
  GWI_BB(gwi_func_end(gwi, zitarev_set_rt60_low, ae_flag_none))
  gwi_func_ini(gwi, "float", "rt60_mid");
  GWI_BB(gwi_func_end(gwi, zitarev_get_rt60_mid, ae_flag_none))
  gwi_func_ini(gwi, "float", "rt60_mid");
     gwinote(gwi, "Time (in seconds) to decay 60db in mid-frequency band.");
     gwi_func_arg(gwi, "float", "_rt60_mid");
  GWI_BB(gwi_func_end(gwi, zitarev_set_rt60_mid, ae_flag_none))
  gwi_func_ini(gwi, "float", "hf_damping");
  GWI_BB(gwi_func_end(gwi, zitarev_get_hf_damping, ae_flag_none))
  gwi_func_ini(gwi, "float", "hf_damping");
     gwinote(gwi, "Frequency (Hz) at which the high-frequency T60 is half the middle-band's T60.");
     gwi_func_arg(gwi, "float", "_hf_damping");
  GWI_BB(gwi_func_end(gwi, zitarev_set_hf_damping, ae_flag_none))
  gwi_func_ini(gwi, "float", "eq1_freq");
  GWI_BB(gwi_func_end(gwi, zitarev_get_eq1_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "eq1_freq");
     gwinote(gwi, "Center frequency of second-order Regalia Mitra peaking equalizer section 1.");
     gwi_func_arg(gwi, "float", "_eq1_freq");
  GWI_BB(gwi_func_end(gwi, zitarev_set_eq1_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "eq1_level");
  GWI_BB(gwi_func_end(gwi, zitarev_get_eq1_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "eq1_level");
     gwinote(gwi, "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 1");
     gwi_func_arg(gwi, "float", "_eq1_level");
  GWI_BB(gwi_func_end(gwi, zitarev_set_eq1_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "eq2_freq");
  GWI_BB(gwi_func_end(gwi, zitarev_get_eq2_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "eq2_freq");
     gwinote(gwi, "Center frequency of second-order Regalia Mitra peaking equalizer section 2.");
     gwi_func_arg(gwi, "float", "_eq2_freq");
  GWI_BB(gwi_func_end(gwi, zitarev_set_eq2_freq, ae_flag_none))
  gwi_func_ini(gwi, "float", "eq2_level");
  GWI_BB(gwi_func_end(gwi, zitarev_get_eq2_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "eq2_level");
     gwinote(gwi, "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 2");
     gwi_func_arg(gwi, "float", "_eq2_level");
  GWI_BB(gwi_func_end(gwi, zitarev_set_eq2_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "mix");
  GWI_BB(gwi_func_end(gwi, zitarev_get_mix, ae_flag_none))
  gwi_func_ini(gwi, "float", "mix");
     gwinote(gwi, "0 = all dry, 1 = all wet");
     gwi_func_arg(gwi, "float", "_mix");
  GWI_BB(gwi_func_end(gwi, zitarev_set_mix, ae_flag_none))
  gwi_func_ini(gwi, "float", "level");
  GWI_BB(gwi_func_end(gwi, zitarev_get_level, ae_flag_none))
  gwi_func_ini(gwi, "float", "level");
     gwinote(gwi, "Output scale factor (in dB).");
     gwi_func_arg(gwi, "float", "_level");
  GWI_BB(gwi_func_end(gwi, zitarev_set_level, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  return GW_OK;
}
