#include <stdlib.h>
#include <soundpipe.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "gwi.h"
#include "array.h"
#include "func.h"
#define FTBL(o) *((sp_ftbl**)((M_Object)o)->data)
#define CHECK_SIZE(size)  if(size <= 0){fprintf(stderr, "'gen_ftbl' size argument must be more than 0");return;}

/*static*/ sp_data* sp;
__attribute__((destructor)) static void sp_end(void) {sp_destroy(&sp);}
static DTOR(sp_dtor) {
  sp_destroy(&sp);
}

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

static CTOR(adsr_ctor) {
  GW_adsr* ug = (GW_adsr*)xcalloc(1, sizeof(GW_adsr));
  ug->sp = sp;
  sp_adsr_create(&ug->osc);
  sp_adsr_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), adsr_tick, ug, 0);
}

static DTOR(adsr_dtor) {
  GW_adsr* ug = UGEN(o)->module.gen.data;
  sp_adsr_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(adsr_get_atk) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(adsr_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(adsr_get_dec) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(adsr_set_dec) {
  const m_uint gw_offset = SZ_INT;
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

static MFUN(adsr_get_sus) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->sus;
}

static MFUN(adsr_set_sus) {
  const m_uint gw_offset = SZ_INT;
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  m_float sus = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->sus = sus);
}

static MFUN(adsr_get_rel) {
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(adsr_set_rel) {
  const m_uint gw_offset = SZ_INT;
  const GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
  sp_data* sp;
  sp_allpass* osc;
  m_bool is_init;
} GW_allpass;

static TICK(allpass_tick) {
  const GW_allpass* ug = (GW_allpass*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_allpass_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(allpass_ctor) {
  GW_allpass* ug = (GW_allpass*)xcalloc(1, sizeof(GW_allpass));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), allpass_tick, ug, 0);
}

static DTOR(allpass_dtor) {
  GW_allpass* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_allpass_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(allpass_init) {
  const m_uint gw_offset = SZ_INT;
  GW_allpass* ug = (GW_allpass*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_allpass_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float looptime = *(m_float*)(shred->mem + gw_offset);
  if(sp_allpass_create(&ug->osc) == SP_NOT_OK || sp_allpass_init(ug->sp, ug->osc, looptime) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(allpass_get_revtime) {
  const GW_allpass* ug = (GW_allpass*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->revtime;
}

static MFUN(allpass_set_revtime) {
  const m_uint gw_offset = SZ_INT;
  const GW_allpass* ug = (GW_allpass*)UGEN(o)->module.gen.data;
  m_float revtime = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(atone_ctor) {
  GW_atone* ug = (GW_atone*)xcalloc(1, sizeof(GW_atone));
  ug->sp = sp;
  sp_atone_create(&ug->osc);
  sp_atone_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), atone_tick, ug, 0);
}

static DTOR(atone_dtor) {
  GW_atone* ug = UGEN(o)->module.gen.data;
  sp_atone_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(atone_get_hp) {
  const GW_atone* ug = (GW_atone*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->hp;
}

static MFUN(atone_set_hp) {
  const m_uint gw_offset = SZ_INT;
  const GW_atone* ug = (GW_atone*)UGEN(o)->module.gen.data;
  m_float hp = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(autowah_ctor) {
  GW_autowah* ug = (GW_autowah*)xcalloc(1, sizeof(GW_autowah));
  ug->sp = sp;
  sp_autowah_create(&ug->osc);
  sp_autowah_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), autowah_tick, ug, 0);
}

static DTOR(autowah_dtor) {
  GW_autowah* ug = UGEN(o)->module.gen.data;
  sp_autowah_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(autowah_get_level) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->level;
}

static MFUN(autowah_set_level) {
  const m_uint gw_offset = SZ_INT;
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  m_float level = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->level = level);
}

static MFUN(autowah_get_wah) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->wah;
}

static MFUN(autowah_set_wah) {
  const m_uint gw_offset = SZ_INT;
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  m_float wah = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->wah = wah);
}

static MFUN(autowah_get_mix) {
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->mix;
}

static MFUN(autowah_set_mix) {
  const m_uint gw_offset = SZ_INT;
  const GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
  m_float mix = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(bal_ctor) {
  GW_bal* ug = (GW_bal*)xcalloc(1, sizeof(GW_bal));
  ug->sp = sp;
  sp_bal_create(&ug->osc);
  sp_bal_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), bal_tick, ug, 0);
}

static DTOR(bal_dtor) {
  GW_bal* ug = UGEN(o)->module.gen.data;
  sp_bal_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_bar* osc;
  m_bool is_init;
} GW_bar;

static TICK(bar_tick) {
  const GW_bar* ug = (GW_bar*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_bar_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(bar_ctor) {
  GW_bar* ug = (GW_bar*)xcalloc(1, sizeof(GW_bar));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), bar_tick, ug, 1);
}

static DTOR(bar_dtor) {
  GW_bar* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_bar_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(bar_init) {
  m_uint gw_offset = SZ_INT;
  GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_bar_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float iK = *(m_float*)(shred->mem + gw_offset);
  gw_offset +=SZ_FLOAT;
  m_float ib = *(m_float*)(shred->mem + gw_offset);
  if(sp_bar_create(&ug->osc) == SP_NOT_OK || sp_bar_init(ug->sp, ug->osc, iK, ib) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(bar_get_bcL) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bcL;
}

static MFUN(bar_set_bcL) {
  const m_uint gw_offset = SZ_INT;
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float bcL = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->bcL = bcL);
}

static MFUN(bar_get_bcR) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bcR;
}

static MFUN(bar_set_bcR) {
  const m_uint gw_offset = SZ_INT;
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float bcR = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->bcR = bcR);
}

static MFUN(bar_get_T30) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->T30;
}

static MFUN(bar_set_T30) {
  const m_uint gw_offset = SZ_INT;
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float T30 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->T30 = T30);
}

static MFUN(bar_get_scan) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->scan;
}

static MFUN(bar_set_scan) {
  const m_uint gw_offset = SZ_INT;
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float scan = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->scan = scan);
}

static MFUN(bar_get_pos) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pos;
}

static MFUN(bar_set_pos) {
  const m_uint gw_offset = SZ_INT;
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float pos = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->pos = pos);
}

static MFUN(bar_get_vel) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->vel;
}

static MFUN(bar_set_vel) {
  const m_uint gw_offset = SZ_INT;
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float vel = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->vel = vel);
}

static MFUN(bar_get_wid) {
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->wid;
}

static MFUN(bar_set_wid) {
  const m_uint gw_offset = SZ_INT;
  const GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
  m_float wid = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(biquad_ctor) {
  GW_biquad* ug = (GW_biquad*)xcalloc(1, sizeof(GW_biquad));
  ug->sp = sp;
  sp_biquad_create(&ug->osc);
  sp_biquad_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), biquad_tick, ug, 0);
}

static DTOR(biquad_dtor) {
  GW_biquad* ug = UGEN(o)->module.gen.data;
  sp_biquad_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(biquad_get_b0) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b0;
}

static MFUN(biquad_set_b0) {
  const m_uint gw_offset = SZ_INT;
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float b0 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->b0 = b0);
}

static MFUN(biquad_get_b1) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b1;
}

static MFUN(biquad_set_b1) {
  const m_uint gw_offset = SZ_INT;
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float b1 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->b1 = b1);
}

static MFUN(biquad_get_b2) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b2;
}

static MFUN(biquad_set_b2) {
  const m_uint gw_offset = SZ_INT;
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float b2 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->b2 = b2);
}

static MFUN(biquad_get_a0) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a0;
}

static MFUN(biquad_set_a0) {
  const m_uint gw_offset = SZ_INT;
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float a0 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->a0 = a0);
}

static MFUN(biquad_get_a1) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a1;
}

static MFUN(biquad_set_a1) {
  const m_uint gw_offset = SZ_INT;
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float a1 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->a1 = a1);
}

static MFUN(biquad_get_a2) {
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a2;
}

static MFUN(biquad_set_a2) {
  const m_uint gw_offset = SZ_INT;
  const GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
  m_float a2 = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(biscale_ctor) {
  GW_biscale* ug = (GW_biscale*)xcalloc(1, sizeof(GW_biscale));
  ug->sp = sp;
  sp_biscale_create(&ug->osc);
  sp_biscale_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), biscale_tick, ug, 0);
}

static DTOR(biscale_dtor) {
  GW_biscale* ug = UGEN(o)->module.gen.data;
  sp_biscale_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(biscale_get_min) {
  const GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(biscale_set_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(biscale_get_max) {
  const GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(biscale_set_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(bitcrush_ctor) {
  GW_bitcrush* ug = (GW_bitcrush*)xcalloc(1, sizeof(GW_bitcrush));
  ug->sp = sp;
  sp_bitcrush_create(&ug->osc);
  sp_bitcrush_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), bitcrush_tick, ug, 0);
}

static DTOR(bitcrush_dtor) {
  GW_bitcrush* ug = UGEN(o)->module.gen.data;
  sp_bitcrush_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(bitcrush_get_bitdepth) {
  const GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bitdepth;
}

static MFUN(bitcrush_set_bitdepth) {
  const m_uint gw_offset = SZ_INT;
  const GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
  m_float bitdepth = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->bitdepth = bitdepth);
}

static MFUN(bitcrush_get_srate) {
  const GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->srate;
}

static MFUN(bitcrush_set_srate) {
  const m_uint gw_offset = SZ_INT;
  const GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
  m_float srate = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(blsaw_ctor) {
  GW_blsaw* ug = (GW_blsaw*)xcalloc(1, sizeof(GW_blsaw));
  ug->sp = sp;
  sp_blsaw_create(&ug->osc);
  sp_blsaw_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), blsaw_tick, ug, 0);
}

static DTOR(blsaw_dtor) {
  GW_blsaw* ug = UGEN(o)->module.gen.data;
  sp_blsaw_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(blsaw_get_freq) {
  const GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->freq;
}

static MFUN(blsaw_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->freq = freq);
}

static MFUN(blsaw_get_amp) {
  const GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->amp;
}

static MFUN(blsaw_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(blsquare_ctor) {
  GW_blsquare* ug = (GW_blsquare*)xcalloc(1, sizeof(GW_blsquare));
  ug->sp = sp;
  sp_blsquare_create(&ug->osc);
  sp_blsquare_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), blsquare_tick, ug, 0);
}

static DTOR(blsquare_dtor) {
  GW_blsquare* ug = UGEN(o)->module.gen.data;
  sp_blsquare_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(blsquare_get_freq) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->freq;
}

static MFUN(blsquare_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->freq = freq);
}

static MFUN(blsquare_get_amp) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->amp;
}

static MFUN(blsquare_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->amp = amp);
}

static MFUN(blsquare_get_width) {
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->width;
}

static MFUN(blsquare_set_width) {
  const m_uint gw_offset = SZ_INT;
  const GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
  m_float width = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(bltriangle_ctor) {
  GW_bltriangle* ug = (GW_bltriangle*)xcalloc(1, sizeof(GW_bltriangle));
  ug->sp = sp;
  sp_bltriangle_create(&ug->osc);
  sp_bltriangle_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), bltriangle_tick, ug, 0);
}

static DTOR(bltriangle_dtor) {
  GW_bltriangle* ug = UGEN(o)->module.gen.data;
  sp_bltriangle_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(bltriangle_get_freq) {
  const GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->freq;
}

static MFUN(bltriangle_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->freq = freq);
}

static MFUN(bltriangle_get_amp) {
  const GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->amp;
}

static MFUN(bltriangle_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(brown_ctor) {
  GW_brown* ug = (GW_brown*)xcalloc(1, sizeof(GW_brown));
  ug->sp = sp;
  sp_brown_create(&ug->osc);
  sp_brown_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), brown_tick, ug, 0);
}

static DTOR(brown_dtor) {
  GW_brown* ug = UGEN(o)->module.gen.data;
  sp_brown_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_butbp* osc;
} GW_butbp;

static TICK(butbp_tick) {
  const GW_butbp* ug = (GW_butbp*)u->module.gen.data;
  sp_butbp_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(butbp_ctor) {
  GW_butbp* ug = (GW_butbp*)xcalloc(1, sizeof(GW_butbp));
  ug->sp = sp;
  sp_butbp_create(&ug->osc);
  sp_butbp_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), butbp_tick, ug, 0);
}

static DTOR(butbp_dtor) {
  GW_butbp* ug = UGEN(o)->module.gen.data;
  sp_butbp_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(butbp_get_freq) {
  const GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(butbp_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(butbp_get_bw) {
  const GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bw;
}

static MFUN(butbp_set_bw) {
  const m_uint gw_offset = SZ_INT;
  const GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
  m_float bw = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(butbr_ctor) {
  GW_butbr* ug = (GW_butbr*)xcalloc(1, sizeof(GW_butbr));
  ug->sp = sp;
  sp_butbr_create(&ug->osc);
  sp_butbr_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), butbr_tick, ug, 0);
}

static DTOR(butbr_dtor) {
  GW_butbr* ug = UGEN(o)->module.gen.data;
  sp_butbr_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(butbr_get_freq) {
  const GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(butbr_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(butbr_get_bw) {
  const GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bw;
}

static MFUN(butbr_set_bw) {
  const m_uint gw_offset = SZ_INT;
  const GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
  m_float bw = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(buthp_ctor) {
  GW_buthp* ug = (GW_buthp*)xcalloc(1, sizeof(GW_buthp));
  ug->sp = sp;
  sp_buthp_create(&ug->osc);
  sp_buthp_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), buthp_tick, ug, 0);
}

static DTOR(buthp_dtor) {
  GW_buthp* ug = UGEN(o)->module.gen.data;
  sp_buthp_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(buthp_get_freq) {
  const GW_buthp* ug = (GW_buthp*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(buthp_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_buthp* ug = (GW_buthp*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(butlp_ctor) {
  GW_butlp* ug = (GW_butlp*)xcalloc(1, sizeof(GW_butlp));
  ug->sp = sp;
  sp_butlp_create(&ug->osc);
  sp_butlp_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), butlp_tick, ug, 0);
}

static DTOR(butlp_dtor) {
  GW_butlp* ug = UGEN(o)->module.gen.data;
  sp_butlp_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(butlp_get_freq) {
  const GW_butlp* ug = (GW_butlp*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(butlp_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_butlp* ug = (GW_butlp*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(clip_ctor) {
  GW_clip* ug = (GW_clip*)xcalloc(1, sizeof(GW_clip));
  ug->sp = sp;
  sp_clip_create(&ug->osc);
  sp_clip_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), clip_tick, ug, 0);
}

static DTOR(clip_dtor) {
  GW_clip* ug = UGEN(o)->module.gen.data;
  sp_clip_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(clip_get_lim) {
  const GW_clip* ug = (GW_clip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->lim;
}

static MFUN(clip_set_lim) {
  const m_uint gw_offset = SZ_INT;
  const GW_clip* ug = (GW_clip*)UGEN(o)->module.gen.data;
  m_float lim = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(clock_ctor) {
  GW_clock* ug = (GW_clock*)xcalloc(1, sizeof(GW_clock));
  ug->sp = sp;
  sp_clock_create(&ug->osc);
  sp_clock_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), clock_tick, ug, 1);
}

static DTOR(clock_dtor) {
  GW_clock* ug = UGEN(o)->module.gen.data;
  sp_clock_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(clock_get_bpm) {
  const GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bpm;
}

static MFUN(clock_set_bpm) {
  const m_uint gw_offset = SZ_INT;
  const GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
  m_float bpm = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->bpm = bpm);
}

static MFUN(clock_get_subdiv) {
  const GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->subdiv;
}

static MFUN(clock_set_subdiv) {
  const m_uint gw_offset = SZ_INT;
  const GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
  m_float subdiv = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->subdiv = subdiv);
}

typedef struct {
  sp_data* sp;
  sp_comb* osc;
  m_bool is_init;
} GW_comb;

static TICK(comb_tick) {
  const GW_comb* ug = (GW_comb*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_comb_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(comb_ctor) {
  GW_comb* ug = (GW_comb*)xcalloc(1, sizeof(GW_comb));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), comb_tick, ug, 0);
}

static DTOR(comb_dtor) {
  GW_comb* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_comb_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(comb_init) {
  const m_uint gw_offset = SZ_INT;
  GW_comb* ug = (GW_comb*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_comb_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float looptime = *(m_float*)(shred->mem + gw_offset);
  if(sp_comb_create(&ug->osc) == SP_NOT_OK || sp_comb_init(ug->sp, ug->osc, looptime) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(comb_get_revtime) {
  const GW_comb* ug = (GW_comb*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->revtime;
}

static MFUN(comb_set_revtime) {
  const m_uint gw_offset = SZ_INT;
  const GW_comb* ug = (GW_comb*)UGEN(o)->module.gen.data;
  m_float revtime = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(compressor_ctor) {
  GW_compressor* ug = (GW_compressor*)xcalloc(1, sizeof(GW_compressor));
  ug->sp = sp;
  sp_compressor_create(&ug->osc);
  sp_compressor_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), compressor_tick, ug, 0);
}

static DTOR(compressor_dtor) {
  GW_compressor* ug = UGEN(o)->module.gen.data;
  sp_compressor_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(compressor_get_ratio) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->ratio;
}

static MFUN(compressor_set_ratio) {
  const m_uint gw_offset = SZ_INT;
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  m_float ratio = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->ratio = ratio);
}

static MFUN(compressor_get_thresh) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->thresh;
}

static MFUN(compressor_set_thresh) {
  const m_uint gw_offset = SZ_INT;
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  m_float thresh = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->thresh = thresh);
}

static MFUN(compressor_get_atk) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->atk;
}

static MFUN(compressor_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->atk = atk);
}

static MFUN(compressor_get_rel) {
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->rel;
}

static MFUN(compressor_set_rel) {
  const m_uint gw_offset = SZ_INT;
  const GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->rel = rel);
}

typedef struct {
  sp_data* sp;
  sp_conv* osc;
  m_bool is_init;
} GW_conv;

static TICK(conv_tick) {
  const GW_conv* ug = (GW_conv*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_conv_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(conv_ctor) {
  GW_conv* ug = (GW_conv*)xcalloc(1, sizeof(GW_conv));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), conv_tick, ug, 0);
}

static DTOR(conv_dtor) {
  GW_conv* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_conv_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(conv_init) {
  m_uint gw_offset = SZ_INT;
  GW_conv* ug = (GW_conv*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_conv_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* ft = FTBL(ft_obj);
  release(ft_obj, shred);
  gw_offset +=SZ_INT;
  m_float iPartLen = *(m_float*)(shred->mem + gw_offset);
  if(sp_conv_create(&ug->osc) == SP_NOT_OK || sp_conv_init(ug->sp, ug->osc, ft, iPartLen) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_count* osc;
} GW_count;

static TICK(count_tick) {
  const GW_count* ug = (GW_count*)u->module.gen.data;
  sp_count_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(count_ctor) {
  GW_count* ug = (GW_count*)xcalloc(1, sizeof(GW_count));
  ug->sp = sp;
  sp_count_create(&ug->osc);
  sp_count_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), count_tick, ug, 1);
}

static DTOR(count_dtor) {
  GW_count* ug = UGEN(o)->module.gen.data;
  sp_count_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(count_get_count) {
  const GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->count;
}

static MFUN(count_set_count) {
  const m_uint gw_offset = SZ_INT;
  const GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
  m_float count = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->count = count);
}

static MFUN(count_get_mode) {
  const GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mode;
}

static MFUN(count_set_mode) {
  const m_uint gw_offset = SZ_INT;
  const GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
  m_float mode = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(crossfade_ctor) {
  GW_crossfade* ug = (GW_crossfade*)xcalloc(1, sizeof(GW_crossfade));
  ug->sp = sp;
  sp_crossfade_create(&ug->osc);
  sp_crossfade_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), crossfade_tick, ug, 0);
}

static DTOR(crossfade_dtor) {
  GW_crossfade* ug = UGEN(o)->module.gen.data;
  sp_crossfade_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(crossfade_get_pos) {
  const GW_crossfade* ug = (GW_crossfade*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pos;
}

static MFUN(crossfade_set_pos) {
  const m_uint gw_offset = SZ_INT;
  const GW_crossfade* ug = (GW_crossfade*)UGEN(o)->module.gen.data;
  m_float pos = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(dcblock_ctor) {
  GW_dcblock* ug = (GW_dcblock*)xcalloc(1, sizeof(GW_dcblock));
  ug->sp = sp;
  sp_dcblock_create(&ug->osc);
  sp_dcblock_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dcblock_tick, ug, 0);
}

static DTOR(dcblock_dtor) {
  GW_dcblock* ug = UGEN(o)->module.gen.data;
  sp_dcblock_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_delay* osc;
  m_bool is_init;
} GW_delay;

static TICK(delay_tick) {
  const GW_delay* ug = (GW_delay*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_delay_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(delay_ctor) {
  GW_delay* ug = (GW_delay*)xcalloc(1, sizeof(GW_delay));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), delay_tick, ug, 0);
}

static DTOR(delay_dtor) {
  GW_delay* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_delay_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(delay_init) {
  const m_uint gw_offset = SZ_INT;
  GW_delay* ug = (GW_delay*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_delay_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float time = *(m_float*)(shred->mem + gw_offset);
  if(sp_delay_create(&ug->osc) == SP_NOT_OK || sp_delay_init(ug->sp, ug->osc, time) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(delay_get_feedback) {
  const GW_delay* ug = (GW_delay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->feedback;
}

static MFUN(delay_set_feedback) {
  const m_uint gw_offset = SZ_INT;
  const GW_delay* ug = (GW_delay*)UGEN(o)->module.gen.data;
  m_float feedback = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(diode_ctor) {
  GW_diode* ug = (GW_diode*)xcalloc(1, sizeof(GW_diode));
  ug->sp = sp;
  sp_diode_create(&ug->osc);
  sp_diode_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), diode_tick, ug, 0);
}

static DTOR(diode_dtor) {
  GW_diode* ug = UGEN(o)->module.gen.data;
  sp_diode_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(diode_get_freq) {
  const GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(diode_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(diode_get_res) {
  const GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(diode_set_res) {
  const m_uint gw_offset = SZ_INT;
  const GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->res = res);
}

typedef struct {
  sp_data* sp;
  sp_diskin* osc;
  m_bool is_init;
} GW_diskin;

static TICK(diskin_tick) {
  const GW_diskin* ug = (GW_diskin*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_diskin_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(diskin_ctor) {
  GW_diskin* ug = (GW_diskin*)xcalloc(1, sizeof(GW_diskin));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), diskin_tick, ug, 0);
}

static DTOR(diskin_dtor) {
  GW_diskin* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_diskin_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(diskin_init) {
  const m_uint gw_offset = SZ_INT;
  GW_diskin* ug = (GW_diskin*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_diskin_destroy(&ug->osc);
    ug->osc = NULL;
  }
  M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str filename = STRING(filename_obj);
  release(filename_obj, shred);
  if(sp_diskin_create(&ug->osc) == SP_NOT_OK || sp_diskin_init(ug->sp, ug->osc, filename) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_dist* osc;
} GW_dist;

static TICK(dist_tick) {
  const GW_dist* ug = (GW_dist*)u->module.gen.data;
  sp_dist_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(dist_ctor) {
  GW_dist* ug = (GW_dist*)xcalloc(1, sizeof(GW_dist));
  ug->sp = sp;
  sp_dist_create(&ug->osc);
  sp_dist_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dist_tick, ug, 0);
}

static DTOR(dist_dtor) {
  GW_dist* ug = UGEN(o)->module.gen.data;
  sp_dist_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(dist_get_pregain) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pregain;
}

static MFUN(dist_set_pregain) {
  const m_uint gw_offset = SZ_INT;
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  m_float pregain = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->pregain = pregain);
}

static MFUN(dist_get_postgain) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->postgain;
}

static MFUN(dist_set_postgain) {
  const m_uint gw_offset = SZ_INT;
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  m_float postgain = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->postgain = postgain);
}

static MFUN(dist_get_shape1) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->shape1;
}

static MFUN(dist_set_shape1) {
  const m_uint gw_offset = SZ_INT;
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  m_float shape1 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->shape1 = shape1);
}

static MFUN(dist_get_shape2) {
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->shape2;
}

static MFUN(dist_set_shape2) {
  const m_uint gw_offset = SZ_INT;
  const GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
  m_float shape2 = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(dmetro_ctor) {
  GW_dmetro* ug = (GW_dmetro*)xcalloc(1, sizeof(GW_dmetro));
  ug->sp = sp;
  sp_dmetro_create(&ug->osc);
  sp_dmetro_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dmetro_tick, ug, 0);
}

static DTOR(dmetro_dtor) {
  GW_dmetro* ug = UGEN(o)->module.gen.data;
  sp_dmetro_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(dmetro_get_time) {
  const GW_dmetro* ug = (GW_dmetro*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->time;
}

static MFUN(dmetro_set_time) {
  const m_uint gw_offset = SZ_INT;
  const GW_dmetro* ug = (GW_dmetro*)UGEN(o)->module.gen.data;
  m_float time = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->time = time);
}

typedef struct {
  sp_data* sp;
  sp_drip* osc;
  m_bool is_init;
} GW_drip;

static TICK(drip_tick) {
  const GW_drip* ug = (GW_drip*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_drip_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(drip_ctor) {
  GW_drip* ug = (GW_drip*)xcalloc(1, sizeof(GW_drip));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), drip_tick, ug, 1);
}

static DTOR(drip_dtor) {
  GW_drip* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_drip_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(drip_init) {
  const m_uint gw_offset = SZ_INT;
  GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_drip_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float dettack = *(m_float*)(shred->mem + gw_offset);
  if(sp_drip_create(&ug->osc) == SP_NOT_OK || sp_drip_init(ug->sp, ug->osc, dettack) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(drip_get_num_tubes) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->num_tubes;
}

static MFUN(drip_set_num_tubes) {
  const m_uint gw_offset = SZ_INT;
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float num_tubes = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->num_tubes = num_tubes);
}

static MFUN(drip_get_amp) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(drip_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(drip_get_damp) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->damp;
}

static MFUN(drip_set_damp) {
  const m_uint gw_offset = SZ_INT;
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float damp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->damp = damp);
}

static MFUN(drip_get_shake_max) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->shake_max;
}

static MFUN(drip_set_shake_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float shake_max = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->shake_max = shake_max);
}

static MFUN(drip_get_freq) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(drip_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(drip_get_freq1) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq1;
}

static MFUN(drip_set_freq1) {
  const m_uint gw_offset = SZ_INT;
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float freq1 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq1 = freq1);
}

static MFUN(drip_get_freq2) {
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq2;
}

static MFUN(drip_set_freq2) {
  const m_uint gw_offset = SZ_INT;
  const GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
  m_float freq2 = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq2 = freq2);
}

typedef struct {
  sp_data* sp;
  sp_dtrig* osc;
  m_bool is_init;
} GW_dtrig;

static TICK(dtrig_tick) {
  const GW_dtrig* ug = (GW_dtrig*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_dtrig_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(dtrig_ctor) {
  GW_dtrig* ug = (GW_dtrig*)xcalloc(1, sizeof(GW_dtrig));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dtrig_tick, ug, 1);
}

static DTOR(dtrig_dtor) {
  GW_dtrig* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_dtrig_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(dtrig_init) {
  const m_uint gw_offset = SZ_INT;
  GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_dtrig_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* ft = FTBL(ft_obj);
  release(ft_obj, shred);
  if(sp_dtrig_create(&ug->osc) == SP_NOT_OK || sp_dtrig_init(ug->sp, ug->osc, ft) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(dtrig_get_loop) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->loop;
}

static MFUN(dtrig_set_loop) {
  const m_uint gw_offset = SZ_INT;
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  m_int loop = *(m_int*)(shred->mem + gw_offset);
  *(m_uint*)RETURN = (ug->osc->loop = loop);
}

static MFUN(dtrig_get_delay) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->delay;
}

static MFUN(dtrig_set_delay) {
  const m_uint gw_offset = SZ_INT;
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  m_float delay = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->delay = delay);
}

static MFUN(dtrig_get_scale) {
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->scale;
}

static MFUN(dtrig_set_scale) {
  const m_uint gw_offset = SZ_INT;
  const GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
  m_float scale = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(dust_ctor) {
  GW_dust* ug = (GW_dust*)xcalloc(1, sizeof(GW_dust));
  ug->sp = sp;
  sp_dust_create(&ug->osc);
  sp_dust_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), dust_tick, ug, 0);
}

static DTOR(dust_dtor) {
  GW_dust* ug = UGEN(o)->module.gen.data;
  sp_dust_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(dust_get_amp) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(dust_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(dust_get_density) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->density;
}

static MFUN(dust_set_density) {
  const m_uint gw_offset = SZ_INT;
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  m_float density = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->density = density);
}

static MFUN(dust_get_bipolar) {
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->bipolar;
}

static MFUN(dust_set_bipolar) {
  const m_uint gw_offset = SZ_INT;
  const GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
  m_int bipolar = *(m_int*)(shred->mem + gw_offset);
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

static CTOR(eqfil_ctor) {
  GW_eqfil* ug = (GW_eqfil*)xcalloc(1, sizeof(GW_eqfil));
  ug->sp = sp;
  sp_eqfil_create(&ug->osc);
  sp_eqfil_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), eqfil_tick, ug, 0);
}

static DTOR(eqfil_dtor) {
  GW_eqfil* ug = UGEN(o)->module.gen.data;
  sp_eqfil_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(eqfil_get_freq) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(eqfil_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(eqfil_get_bw) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bw;
}

static MFUN(eqfil_set_bw) {
  const m_uint gw_offset = SZ_INT;
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  m_float bw = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->bw = bw);
}

static MFUN(eqfil_get_gain) {
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->gain;
}

static MFUN(eqfil_set_gain) {
  const m_uint gw_offset = SZ_INT;
  const GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
  m_float gain = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(expon_ctor) {
  GW_expon* ug = (GW_expon*)xcalloc(1, sizeof(GW_expon));
  ug->sp = sp;
  sp_expon_create(&ug->osc);
  sp_expon_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), expon_tick, ug, 1);
}

static DTOR(expon_dtor) {
  GW_expon* ug = UGEN(o)->module.gen.data;
  sp_expon_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(expon_get_a) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a;
}

static MFUN(expon_set_a) {
  const m_uint gw_offset = SZ_INT;
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  m_float a = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->a = a);
}

static MFUN(expon_get_dur) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(expon_set_dur) {
  const m_uint gw_offset = SZ_INT;
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dur = dur);
}

static MFUN(expon_get_b) {
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b;
}

static MFUN(expon_set_b) {
  const m_uint gw_offset = SZ_INT;
  const GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
  m_float b = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->b = b);
}

typedef struct {
  sp_data* sp;
  sp_fof* osc;
  m_bool is_init;
} GW_fof;

static TICK(fof_tick) {
  const GW_fof* ug = (GW_fof*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_fof_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(fof_ctor) {
  GW_fof* ug = (GW_fof*)xcalloc(1, sizeof(GW_fof));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fof_tick, ug, 0);
}

static DTOR(fof_dtor) {
  GW_fof* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_fof_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(fof_init) {
  m_uint gw_offset = SZ_INT;
  GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_fof_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object sine_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* sine = FTBL(sine_obj);
  release(sine_obj, shred);
  gw_offset +=SZ_INT;
  const M_Object win_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* win = FTBL(win_obj);
  release(win_obj, shred);
  gw_offset +=SZ_INT;
  m_int iolaps = *(m_int*)(shred->mem + gw_offset);
  gw_offset +=SZ_INT;
  m_float iphs = *(m_float*)(shred->mem + gw_offset);
  if(sp_fof_create(&ug->osc) == SP_NOT_OK || sp_fof_init(ug->sp, ug->osc, sine, win, iolaps, iphs) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(fof_get_amp) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(fof_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(fof_get_fund) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->fund;
}

static MFUN(fof_set_fund) {
  const m_uint gw_offset = SZ_INT;
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float fund = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->fund = fund);
}

static MFUN(fof_get_form) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->form;
}

static MFUN(fof_set_form) {
  const m_uint gw_offset = SZ_INT;
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float form = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->form = form);
}

static MFUN(fof_get_oct) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->oct;
}

static MFUN(fof_set_oct) {
  const m_uint gw_offset = SZ_INT;
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float oct = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->oct = oct);
}

static MFUN(fof_get_band) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->band;
}

static MFUN(fof_set_band) {
  const m_uint gw_offset = SZ_INT;
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float band = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->band = band);
}

static MFUN(fof_get_ris) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->ris;
}

static MFUN(fof_set_ris) {
  const m_uint gw_offset = SZ_INT;
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float ris = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->ris = ris);
}

static MFUN(fof_get_dec) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(fof_set_dec) {
  const m_uint gw_offset = SZ_INT;
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

static MFUN(fof_get_dur) {
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(fof_set_dur) {
  const m_uint gw_offset = SZ_INT;
  const GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(fofilt_ctor) {
  GW_fofilt* ug = (GW_fofilt*)xcalloc(1, sizeof(GW_fofilt));
  ug->sp = sp;
  sp_fofilt_create(&ug->osc);
  sp_fofilt_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fofilt_tick, ug, 0);
}

static DTOR(fofilt_dtor) {
  GW_fofilt* ug = UGEN(o)->module.gen.data;
  sp_fofilt_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(fofilt_get_freq) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(fofilt_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(fofilt_get_atk) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(fofilt_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(fofilt_get_dec) {
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(fofilt_set_dec) {
  const m_uint gw_offset = SZ_INT;
  const GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

typedef struct {
  sp_data* sp;
  sp_fog* osc;
  m_bool is_init;
} GW_fog;

static TICK(fog_tick) {
  const GW_fog* ug = (GW_fog*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_fog_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(fog_ctor) {
  GW_fog* ug = (GW_fog*)xcalloc(1, sizeof(GW_fog));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fog_tick, ug, 0);
}

static DTOR(fog_dtor) {
  GW_fog* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_fog_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(fog_init) {
  m_uint gw_offset = SZ_INT;
  GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_fog_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object wav_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* wav = FTBL(wav_obj);
  release(wav_obj, shred);
  gw_offset +=SZ_INT;
  const M_Object win_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* win = FTBL(win_obj);
  release(win_obj, shred);
  gw_offset +=SZ_INT;
  m_int iolaps = *(m_int*)(shred->mem + gw_offset);
  gw_offset +=SZ_INT;
  m_float iphs = *(m_float*)(shred->mem + gw_offset);
  if(sp_fog_create(&ug->osc) == SP_NOT_OK || sp_fog_init(ug->sp, ug->osc, wav, win, iolaps, iphs) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(fog_get_amp) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(fog_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(fog_get_dens) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dens;
}

static MFUN(fog_set_dens) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float dens = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dens = dens);
}

static MFUN(fog_get_trans) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->trans;
}

static MFUN(fog_set_trans) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float trans = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->trans = trans);
}

static MFUN(fog_get_spd) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->spd;
}

static MFUN(fog_set_spd) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float spd = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->spd = spd);
}

static MFUN(fog_get_oct) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->oct;
}

static MFUN(fog_set_oct) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float oct = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->oct = oct);
}

static MFUN(fog_get_band) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->band;
}

static MFUN(fog_set_band) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float band = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->band = band);
}

static MFUN(fog_get_ris) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->ris;
}

static MFUN(fog_set_ris) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float ris = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->ris = ris);
}

static MFUN(fog_get_dec) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(fog_set_dec) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

static MFUN(fog_get_dur) {
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(fog_set_dur) {
  const m_uint gw_offset = SZ_INT;
  const GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(fold_ctor) {
  GW_fold* ug = (GW_fold*)xcalloc(1, sizeof(GW_fold));
  ug->sp = sp;
  sp_fold_create(&ug->osc);
  sp_fold_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fold_tick, ug, 0);
}

static DTOR(fold_dtor) {
  GW_fold* ug = UGEN(o)->module.gen.data;
  sp_fold_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(fold_get_incr) {
  const GW_fold* ug = (GW_fold*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->incr;
}

static MFUN(fold_set_incr) {
  const m_uint gw_offset = SZ_INT;
  const GW_fold* ug = (GW_fold*)UGEN(o)->module.gen.data;
  m_float incr = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->incr = incr);
}

typedef struct {
  sp_data* sp;
  sp_fosc* osc;
  m_bool is_init;
} GW_fosc;

static TICK(fosc_tick) {
  const GW_fosc* ug = (GW_fosc*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_fosc_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(fosc_ctor) {
  GW_fosc* ug = (GW_fosc*)xcalloc(1, sizeof(GW_fosc));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fosc_tick, ug, 0);
}

static DTOR(fosc_dtor) {
  GW_fosc* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_fosc_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(fosc_init) {
  const m_uint gw_offset = SZ_INT;
  GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_fosc_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object tbl_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* tbl = FTBL(tbl_obj);
  release(tbl_obj, shred);
  if(sp_fosc_create(&ug->osc) == SP_NOT_OK || sp_fosc_init(ug->sp, ug->osc, tbl) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(fosc_get_freq) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(fosc_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(fosc_get_amp) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(fosc_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(fosc_get_car) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->car;
}

static MFUN(fosc_set_car) {
  const m_uint gw_offset = SZ_INT;
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float car = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->car = car);
}

static MFUN(fosc_get_mod) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mod;
}

static MFUN(fosc_set_mod) {
  const m_uint gw_offset = SZ_INT;
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float mod = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->mod = mod);
}

static MFUN(fosc_get_indx) {
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->indx;
}

static MFUN(fosc_set_indx) {
  const m_uint gw_offset = SZ_INT;
  const GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
  m_float indx = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->indx = indx);
}

typedef struct {
  sp_data* sp;
  sp_gbuzz* osc;
  m_bool is_init;
} GW_gbuzz;

static TICK(gbuzz_tick) {
  const GW_gbuzz* ug = (GW_gbuzz*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_gbuzz_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(gbuzz_ctor) {
  GW_gbuzz* ug = (GW_gbuzz*)xcalloc(1, sizeof(GW_gbuzz));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), gbuzz_tick, ug, 0);
}

static DTOR(gbuzz_dtor) {
  GW_gbuzz* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_gbuzz_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(gbuzz_init) {
  m_uint gw_offset = SZ_INT;
  GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_gbuzz_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* ft = FTBL(ft_obj);
  release(ft_obj, shred);
  gw_offset +=SZ_INT;
  m_float iphs = *(m_float*)(shred->mem + gw_offset);
  if(sp_gbuzz_create(&ug->osc) == SP_NOT_OK || sp_gbuzz_init(ug->sp, ug->osc, ft, iphs) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(gbuzz_get_freq) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(gbuzz_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(gbuzz_get_amp) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(gbuzz_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(gbuzz_get_nharm) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->nharm;
}

static MFUN(gbuzz_set_nharm) {
  const m_uint gw_offset = SZ_INT;
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float nharm = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->nharm = nharm);
}

static MFUN(gbuzz_get_lharm) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->lharm;
}

static MFUN(gbuzz_set_lharm) {
  const m_uint gw_offset = SZ_INT;
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float lharm = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->lharm = lharm);
}

static MFUN(gbuzz_get_mul) {
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mul;
}

static MFUN(gbuzz_set_mul) {
  const m_uint gw_offset = SZ_INT;
  const GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
  m_float mul = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->mul = mul);
}

static MFUN(ftbl_gen_composite) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str argstring = STRING(argstring_obj);
  release(argstring_obj, shred);
  CHECK_SIZE(size);
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_composite(sp, ftbl, argstring);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_file) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str filename = STRING(filename_obj);
  release(filename_obj, shred);
  CHECK_SIZE(size);
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_file(sp, ftbl, filename);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_gauss) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  m_float scale = *(m_float*)(shred->mem + gw_offset);
  gw_offset +=SZ_FLOAT;
  m_int seed = *(m_int*)(shred->mem + gw_offset);
  CHECK_SIZE(size);
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_gauss(sp, ftbl, scale, seed);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_line) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str argstring = STRING(argstring_obj);
  release(argstring_obj, shred);
  CHECK_SIZE(size);
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_line(sp, ftbl, argstring);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_padsynth) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  const M_Object amps_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* amps = FTBL(amps_obj);
  release(amps_obj, shred);
  gw_offset +=SZ_INT;
  m_float f = *(m_float*)(shred->mem + gw_offset);
  gw_offset +=SZ_FLOAT;
  m_float bw = *(m_float*)(shred->mem + gw_offset);
  CHECK_SIZE(size);
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_padsynth(sp, ftbl, amps, f, bw);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_rand) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str argstring = STRING(argstring_obj);
  release(argstring_obj, shred);
  CHECK_SIZE(size);
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_rand(sp, ftbl, argstring);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_scrambler) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object dest_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl** dest = &FTBL(dest_obj);
  release(dest_obj, shred);
  CHECK_SIZE(size);
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
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_sine(sp, ftbl);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_sinesum) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str argstring = STRING(argstring_obj);
  release(argstring_obj, shred);
  CHECK_SIZE(size);
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
  sp_ftbl_create(sp, &ftbl, size);
  sp_gen_triangle(sp, ftbl);
  FTBL(o) = ftbl;
}

static MFUN(ftbl_gen_xline) {
  sp_ftbl* ftbl = FTBL(o);
  m_uint gw_offset = SZ_INT*2;
  if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
  m_int size = *(m_int*)(shred->mem + SZ_INT);
  M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str argstring = STRING(argstring_obj);
  release(argstring_obj, shred);
  CHECK_SIZE(size);
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

static CTOR(hilbert_ctor) {
  GW_hilbert* ug = (GW_hilbert*)xcalloc(1, sizeof(GW_hilbert));
  ug->sp = sp;
  sp_hilbert_create(&ug->osc);
  sp_hilbert_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), hilbert_tick, ug, 0);
}

static DTOR(hilbert_dtor) {
  GW_hilbert* ug = UGEN(o)->module.gen.data;
  sp_hilbert_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_in* osc;
} GW_in;

static TICK(in_tick) {
  const GW_in* ug = (GW_in*)u->module.gen.data;
  sp_in_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(in_ctor) {
  GW_in* ug = (GW_in*)xcalloc(1, sizeof(GW_in));
  ug->sp = sp;
  sp_in_create(&ug->osc);
  sp_in_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), in_tick, ug, 0);
}

static DTOR(in_dtor) {
  GW_in* ug = UGEN(o)->module.gen.data;
  sp_in_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_incr* osc;
  m_bool is_init;
} GW_incr;

static TICK(incr_tick) {
  const GW_incr* ug = (GW_incr*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_incr_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(incr_ctor) {
  GW_incr* ug = (GW_incr*)xcalloc(1, sizeof(GW_incr));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), incr_tick, ug, 1);
}

static DTOR(incr_dtor) {
  GW_incr* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_incr_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(incr_init) {
  const m_uint gw_offset = SZ_INT;
  GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_incr_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float val = *(m_float*)(shred->mem + gw_offset);
  if(sp_incr_create(&ug->osc) == SP_NOT_OK || sp_incr_init(ug->sp, ug->osc, val) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(incr_get_step) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->step;
}

static MFUN(incr_set_step) {
  const m_uint gw_offset = SZ_INT;
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  m_float step = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->step = step);
}

static MFUN(incr_get_min) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(incr_set_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(incr_get_max) {
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(incr_set_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(jcrev_ctor) {
  GW_jcrev* ug = (GW_jcrev*)xcalloc(1, sizeof(GW_jcrev));
  ug->sp = sp;
  sp_jcrev_create(&ug->osc);
  sp_jcrev_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), jcrev_tick, ug, 0);
}

static DTOR(jcrev_dtor) {
  GW_jcrev* ug = UGEN(o)->module.gen.data;
  sp_jcrev_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_jitter* osc;
} GW_jitter;

static TICK(jitter_tick) {
  const GW_jitter* ug = (GW_jitter*)u->module.gen.data;
  sp_jitter_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(jitter_ctor) {
  GW_jitter* ug = (GW_jitter*)xcalloc(1, sizeof(GW_jitter));
  ug->sp = sp;
  sp_jitter_create(&ug->osc);
  sp_jitter_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), jitter_tick, ug, 0);
}

static DTOR(jitter_dtor) {
  GW_jitter* ug = UGEN(o)->module.gen.data;
  sp_jitter_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(jitter_get_amp) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(jitter_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(jitter_get_cpsMin) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cpsMin;
}

static MFUN(jitter_set_cpsMin) {
  const m_uint gw_offset = SZ_INT;
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  m_float cpsMin = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->cpsMin = cpsMin);
}

static MFUN(jitter_get_cpsMax) {
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cpsMax;
}

static MFUN(jitter_set_cpsMax) {
  const m_uint gw_offset = SZ_INT;
  const GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
  m_float cpsMax = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(line_ctor) {
  GW_line* ug = (GW_line*)xcalloc(1, sizeof(GW_line));
  ug->sp = sp;
  sp_line_create(&ug->osc);
  sp_line_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), line_tick, ug, 1);
}

static DTOR(line_dtor) {
  GW_line* ug = UGEN(o)->module.gen.data;
  sp_line_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(line_get_a) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->a;
}

static MFUN(line_set_a) {
  const m_uint gw_offset = SZ_INT;
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  m_float a = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->a = a);
}

static MFUN(line_get_dur) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(line_set_dur) {
  const m_uint gw_offset = SZ_INT;
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dur = dur);
}

static MFUN(line_get_b) {
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->b;
}

static MFUN(line_set_b) {
  const m_uint gw_offset = SZ_INT;
  const GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
  m_float b = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->b = b);
}

typedef struct {
  sp_data* sp;
  sp_lpc* osc;
  m_bool is_init;
} GW_lpc;

static TICK(lpc_tick) {
  const GW_lpc* ug = (GW_lpc*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_lpc_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(lpc_ctor) {
  GW_lpc* ug = (GW_lpc*)xcalloc(1, sizeof(GW_lpc));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), lpc_tick, ug, 0);
}

static DTOR(lpc_dtor) {
  GW_lpc* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_lpc_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(lpc_init) {
  const m_uint gw_offset = SZ_INT;
  GW_lpc* ug = (GW_lpc*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_lpc_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_int framesize = *(m_int*)(shred->mem + gw_offset);
  if(sp_lpc_create(&ug->osc) == SP_NOT_OK || sp_lpc_init(ug->sp, ug->osc, framesize) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_lpf18* osc;
} GW_lpf18;

static TICK(lpf18_tick) {
  const GW_lpf18* ug = (GW_lpf18*)u->module.gen.data;
  sp_lpf18_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(lpf18_ctor) {
  GW_lpf18* ug = (GW_lpf18*)xcalloc(1, sizeof(GW_lpf18));
  ug->sp = sp;
  sp_lpf18_create(&ug->osc);
  sp_lpf18_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), lpf18_tick, ug, 0);
}

static DTOR(lpf18_dtor) {
  GW_lpf18* ug = UGEN(o)->module.gen.data;
  sp_lpf18_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(lpf18_get_cutoff) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cutoff;
}

static MFUN(lpf18_set_cutoff) {
  const m_uint gw_offset = SZ_INT;
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  m_float cutoff = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->cutoff = cutoff);
}

static MFUN(lpf18_get_res) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(lpf18_set_res) {
  const m_uint gw_offset = SZ_INT;
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->res = res);
}

static MFUN(lpf18_get_dist) {
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dist;
}

static MFUN(lpf18_set_dist) {
  const m_uint gw_offset = SZ_INT;
  const GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
  m_float dist = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(maygate_ctor) {
  GW_maygate* ug = (GW_maygate*)xcalloc(1, sizeof(GW_maygate));
  ug->sp = sp;
  sp_maygate_create(&ug->osc);
  sp_maygate_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), maygate_tick, ug, 1);
}

static DTOR(maygate_dtor) {
  GW_maygate* ug = UGEN(o)->module.gen.data;
  sp_maygate_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(maygate_get_prob) {
  const GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->prob;
}

static MFUN(maygate_set_prob) {
  const m_uint gw_offset = SZ_INT;
  const GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
  m_float prob = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->prob = prob);
}

static MFUN(maygate_get_mode) {
  const GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->mode;
}

static MFUN(maygate_set_mode) {
  const m_uint gw_offset = SZ_INT;
  const GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
  m_int mode = *(m_int*)(shred->mem + gw_offset);
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

static CTOR(metro_ctor) {
  GW_metro* ug = (GW_metro*)xcalloc(1, sizeof(GW_metro));
  ug->sp = sp;
  sp_metro_create(&ug->osc);
  sp_metro_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), metro_tick, ug, 0);
}

static DTOR(metro_dtor) {
  GW_metro* ug = UGEN(o)->module.gen.data;
  sp_metro_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(metro_get_freq) {
  const GW_metro* ug = (GW_metro*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(metro_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_metro* ug = (GW_metro*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
  sp_data* sp;
  sp_mincer* osc;
  m_bool is_init;
} GW_mincer;

static TICK(mincer_tick) {
  const GW_mincer* ug = (GW_mincer*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_mincer_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(mincer_ctor) {
  GW_mincer* ug = (GW_mincer*)xcalloc(1, sizeof(GW_mincer));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), mincer_tick, ug, 0);
}

static DTOR(mincer_dtor) {
  GW_mincer* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_mincer_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(mincer_init) {
  m_uint gw_offset = SZ_INT;
  GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_mincer_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* ft = FTBL(ft_obj);
  release(ft_obj, shred);
  gw_offset +=SZ_INT;
  m_int winsize = *(m_int*)(shred->mem + gw_offset);
  if(sp_mincer_create(&ug->osc) == SP_NOT_OK || sp_mincer_init(ug->sp, ug->osc, ft, winsize) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(mincer_get_time) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->time;
}

static MFUN(mincer_set_time) {
  const m_uint gw_offset = SZ_INT;
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  m_float time = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->time = time);
}

static MFUN(mincer_get_amp) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(mincer_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(mincer_get_pitch) {
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pitch;
}

static MFUN(mincer_set_pitch) {
  const m_uint gw_offset = SZ_INT;
  const GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
  m_float pitch = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(mode_ctor) {
  GW_mode* ug = (GW_mode*)xcalloc(1, sizeof(GW_mode));
  ug->sp = sp;
  sp_mode_create(&ug->osc);
  sp_mode_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), mode_tick, ug, 0);
}

static DTOR(mode_dtor) {
  GW_mode* ug = UGEN(o)->module.gen.data;
  sp_mode_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(mode_get_freq) {
  const GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(mode_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(mode_get_q) {
  const GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->q;
}

static MFUN(mode_set_q) {
  const m_uint gw_offset = SZ_INT;
  const GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
  m_float q = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(moogladder_ctor) {
  GW_moogladder* ug = (GW_moogladder*)xcalloc(1, sizeof(GW_moogladder));
  ug->sp = sp;
  sp_moogladder_create(&ug->osc);
  sp_moogladder_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), moogladder_tick, ug, 0);
}

static DTOR(moogladder_dtor) {
  GW_moogladder* ug = UGEN(o)->module.gen.data;
  sp_moogladder_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(moogladder_get_freq) {
  const GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(moogladder_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(moogladder_get_res) {
  const GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(moogladder_set_res) {
  const m_uint gw_offset = SZ_INT;
  const GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(noise_ctor) {
  GW_noise* ug = (GW_noise*)xcalloc(1, sizeof(GW_noise));
  ug->sp = sp;
  sp_noise_create(&ug->osc);
  sp_noise_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), noise_tick, ug, 0);
}

static DTOR(noise_dtor) {
  GW_noise* ug = UGEN(o)->module.gen.data;
  sp_noise_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(noise_get_amp) {
  const GW_noise* ug = (GW_noise*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(noise_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_noise* ug = (GW_noise*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_nsmp* osc;
  m_bool is_init;
} GW_nsmp;

static TICK(nsmp_tick) {
  const GW_nsmp* ug = (GW_nsmp*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_nsmp_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(nsmp_ctor) {
  GW_nsmp* ug = (GW_nsmp*)xcalloc(1, sizeof(GW_nsmp));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), nsmp_tick, ug, 1);
}

static DTOR(nsmp_dtor) {
  GW_nsmp* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_nsmp_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(nsmp_init) {
  m_uint gw_offset = SZ_INT;
  GW_nsmp* ug = (GW_nsmp*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_nsmp_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* ft = FTBL(ft_obj);
  release(ft_obj, shred);
  gw_offset +=SZ_INT;
  m_int sr = *(m_int*)(shred->mem + gw_offset);
  gw_offset +=SZ_INT;
  M_Object init_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str init = STRING(init_obj);
  release(init_obj, shred);
  if(sp_nsmp_create(&ug->osc) == SP_NOT_OK || sp_nsmp_init(ug->sp, ug->osc, ft, sr, init) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(nsmp_get_index) {
  const GW_nsmp* ug = (GW_nsmp*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->index;
}

static MFUN(nsmp_set_index) {
  const m_uint gw_offset = SZ_INT;
  const GW_nsmp* ug = (GW_nsmp*)UGEN(o)->module.gen.data;
  m_int index = *(m_int*)(shred->mem + gw_offset);
  *(m_uint*)RETURN = (ug->osc->index = index);
}

typedef struct {
  sp_data* sp;
  sp_osc* osc;
  m_bool is_init;
} GW_osc;

static TICK(osc_tick) {
  const GW_osc* ug = (GW_osc*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_osc_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(osc_ctor) {
  GW_osc* ug = (GW_osc*)xcalloc(1, sizeof(GW_osc));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), osc_tick, ug, 0);
}

static DTOR(osc_dtor) {
  GW_osc* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_osc_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(osc_init) {
  m_uint gw_offset = SZ_INT;
  GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_osc_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object tbl_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* tbl = FTBL(tbl_obj);
  release(tbl_obj, shred);
  gw_offset +=SZ_INT;
  m_float phase = *(m_float*)(shred->mem + gw_offset);
  if(sp_osc_create(&ug->osc) == SP_NOT_OK || sp_osc_init(ug->sp, ug->osc, tbl, phase) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(osc_get_freq) {
  const GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(osc_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(osc_get_amp) {
  const GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(osc_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_oscmorph* osc;
  m_bool is_init;
  sp_ftbl** tbl;

} GW_oscmorph;

static TICK(oscmorph_tick) {
  const GW_oscmorph* ug = (GW_oscmorph*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_oscmorph_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(oscmorph_ctor) {
  GW_oscmorph* ug = (GW_oscmorph*)xcalloc(1, sizeof(GW_oscmorph));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), oscmorph_tick, ug, 0);
}

static DTOR(oscmorph_dtor) {
  GW_oscmorph* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    xfree(ug->osc->tbl);

    sp_oscmorph_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(oscmorph_init) {
  m_uint gw_offset = SZ_INT;
  GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_oscmorph_destroy(&ug->osc);
    xfree(ug->tbl);
    ug->osc = NULL;
  }
  M_Object tbl_ptr = *(M_Object*)(shred->mem + gw_offset);
  m_uint tbl_iter;
  sp_ftbl** tbl = (sp_ftbl**)xmalloc(m_vector_size(ARRAY(tbl_ptr)) * SZ_INT);
  for(tbl_iter = 0; tbl_iter < m_vector_size(ARRAY(tbl_ptr)); tbl_iter++) {
      M_Object tbl_ftl_obj;
      m_vector_get(ARRAY(tbl_ptr), tbl_iter, &tbl_ftl_obj);
      tbl[tbl_iter] = FTBL(tbl_ftl_obj);
  }
  release(tbl_ptr, shred);
  gw_offset +=SZ_INT;
  m_int nft = *(m_int*)(shred->mem + gw_offset);
  gw_offset +=SZ_INT;
  m_float phase = *(m_float*)(shred->mem + gw_offset);
  if(sp_oscmorph_create(&ug->osc) == SP_NOT_OK || sp_oscmorph_init(ug->sp, ug->osc, tbl, nft, phase) == SP_NOT_OK) {
    xfree(tbl); // LCOV_EXCL_LINE
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->tbl = tbl;
  ug->is_init = 1;
}

static MFUN(oscmorph_get_freq) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(oscmorph_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(oscmorph_get_amp) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(oscmorph_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

static MFUN(oscmorph_get_wtpos) {
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->wtpos;
}

static MFUN(oscmorph_set_wtpos) {
  const m_uint gw_offset = SZ_INT;
  const GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
  m_float wtpos = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(pan2_ctor) {
  GW_pan2* ug = (GW_pan2*)xcalloc(1, sizeof(GW_pan2));
  ug->sp = sp;
  sp_pan2_create(&ug->osc);
  sp_pan2_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pan2_tick, ug, 0);
}

static DTOR(pan2_dtor) {
  GW_pan2* ug = UGEN(o)->module.gen.data;
  sp_pan2_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(pan2_get_type) {
  const GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->type;
}

static MFUN(pan2_set_type) {
  const m_uint gw_offset = SZ_INT;
  const GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
  m_int type = *(m_int*)(shred->mem + gw_offset);
  *(m_uint*)RETURN = (ug->osc->type = type);
}

static MFUN(pan2_get_pan) {
  const GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pan;
}

static MFUN(pan2_set_pan) {
  const m_uint gw_offset = SZ_INT;
  const GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
  m_float pan = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(panst_ctor) {
  GW_panst* ug = (GW_panst*)xcalloc(1, sizeof(GW_panst));
  ug->sp = sp;
  sp_panst_create(&ug->osc);
  sp_panst_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), panst_tick, ug, 0);
}

static DTOR(panst_dtor) {
  GW_panst* ug = UGEN(o)->module.gen.data;
  sp_panst_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(panst_get_type) {
  const GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->type;
}

static MFUN(panst_set_type) {
  const m_uint gw_offset = SZ_INT;
  const GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
  m_int type = *(m_int*)(shred->mem + gw_offset);
  *(m_uint*)RETURN = (ug->osc->type = type);
}

static MFUN(panst_get_pan) {
  const GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->pan;
}

static MFUN(panst_set_pan) {
  const m_uint gw_offset = SZ_INT;
  const GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
  m_float pan = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(pareq_ctor) {
  GW_pareq* ug = (GW_pareq*)xcalloc(1, sizeof(GW_pareq));
  ug->sp = sp;
  sp_pareq_create(&ug->osc);
  sp_pareq_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pareq_tick, ug, 0);
}

static DTOR(pareq_dtor) {
  GW_pareq* ug = UGEN(o)->module.gen.data;
  sp_pareq_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(pareq_get_fc) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->fc;
}

static MFUN(pareq_set_fc) {
  const m_uint gw_offset = SZ_INT;
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  m_float fc = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->fc = fc);
}

static MFUN(pareq_get_v) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->v;
}

static MFUN(pareq_set_v) {
  const m_uint gw_offset = SZ_INT;
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  m_float v = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->v = v);
}

static MFUN(pareq_get_q) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->q;
}

static MFUN(pareq_set_q) {
  const m_uint gw_offset = SZ_INT;
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  m_float q = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->q = q);
}

static MFUN(pareq_get_mode) {
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mode;
}

static MFUN(pareq_set_mode) {
  const m_uint gw_offset = SZ_INT;
  const GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
  m_float mode = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
  sp_data* sp;
  sp_paulstretch* osc;
  m_bool is_init;
} GW_paulstretch;

static TICK(paulstretch_tick) {
  const GW_paulstretch* ug = (GW_paulstretch*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_paulstretch_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(paulstretch_ctor) {
  GW_paulstretch* ug = (GW_paulstretch*)xcalloc(1, sizeof(GW_paulstretch));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), paulstretch_tick, ug, 0);
}

static DTOR(paulstretch_dtor) {
  GW_paulstretch* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_paulstretch_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(paulstretch_init) {
  m_uint gw_offset = SZ_INT;
  GW_paulstretch* ug = (GW_paulstretch*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_paulstretch_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* ft = FTBL(ft_obj);
  release(ft_obj, shred);
  gw_offset +=SZ_INT;
  m_float windowsize = *(m_float*)(shred->mem + gw_offset);
  gw_offset +=SZ_FLOAT;
  m_float stretch = *(m_float*)(shred->mem + gw_offset);
  if(sp_paulstretch_create(&ug->osc) == SP_NOT_OK || sp_paulstretch_init(ug->sp, ug->osc, ft, windowsize, stretch) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_pdhalf* osc;
} GW_pdhalf;

static TICK(pdhalf_tick) {
  const GW_pdhalf* ug = (GW_pdhalf*)u->module.gen.data;
  sp_pdhalf_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(pdhalf_ctor) {
  GW_pdhalf* ug = (GW_pdhalf*)xcalloc(1, sizeof(GW_pdhalf));
  ug->sp = sp;
  sp_pdhalf_create(&ug->osc);
  sp_pdhalf_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pdhalf_tick, ug, 0);
}

static DTOR(pdhalf_dtor) {
  GW_pdhalf* ug = UGEN(o)->module.gen.data;
  sp_pdhalf_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(pdhalf_get_amount) {
  const GW_pdhalf* ug = (GW_pdhalf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amount;
}

static MFUN(pdhalf_set_amount) {
  const m_uint gw_offset = SZ_INT;
  const GW_pdhalf* ug = (GW_pdhalf*)UGEN(o)->module.gen.data;
  m_float amount = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(peaklim_ctor) {
  GW_peaklim* ug = (GW_peaklim*)xcalloc(1, sizeof(GW_peaklim));
  ug->sp = sp;
  sp_peaklim_create(&ug->osc);
  sp_peaklim_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), peaklim_tick, ug, 0);
}

static DTOR(peaklim_dtor) {
  GW_peaklim* ug = UGEN(o)->module.gen.data;
  sp_peaklim_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(peaklim_get_atk) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(peaklim_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(peaklim_get_rel) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(peaklim_set_rel) {
  const m_uint gw_offset = SZ_INT;
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->rel = rel);
}

static MFUN(peaklim_get_thresh) {
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->thresh;
}

static MFUN(peaklim_set_thresh) {
  const m_uint gw_offset = SZ_INT;
  const GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
  m_float thresh = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(phaser_ctor) {
  GW_phaser* ug = (GW_phaser*)xcalloc(1, sizeof(GW_phaser));
  ug->sp = sp;
  sp_phaser_create(&ug->osc);
  sp_phaser_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), phaser_tick, ug, 0);
}

static DTOR(phaser_dtor) {
  GW_phaser* ug = UGEN(o)->module.gen.data;
  sp_phaser_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(phaser_get_MaxNotch1Freq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->MaxNotch1Freq;
}

static MFUN(phaser_set_MaxNotch1Freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float MaxNotch1Freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->MaxNotch1Freq = MaxNotch1Freq);
}

static MFUN(phaser_get_MinNotch1Freq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->MinNotch1Freq;
}

static MFUN(phaser_set_MinNotch1Freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float MinNotch1Freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->MinNotch1Freq = MinNotch1Freq);
}

static MFUN(phaser_get_Notch_width) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->Notch_width;
}

static MFUN(phaser_set_Notch_width) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float Notch_width = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->Notch_width = Notch_width);
}

static MFUN(phaser_get_NotchFreq) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->NotchFreq;
}

static MFUN(phaser_set_NotchFreq) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float NotchFreq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->NotchFreq = NotchFreq);
}

static MFUN(phaser_get_VibratoMode) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->VibratoMode;
}

static MFUN(phaser_set_VibratoMode) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float VibratoMode = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->VibratoMode = VibratoMode);
}

static MFUN(phaser_get_depth) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->depth;
}

static MFUN(phaser_set_depth) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float depth = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->depth = depth);
}

static MFUN(phaser_get_feedback_gain) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->feedback_gain;
}

static MFUN(phaser_set_feedback_gain) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float feedback_gain = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->feedback_gain = feedback_gain);
}

static MFUN(phaser_get_invert) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->invert;
}

static MFUN(phaser_set_invert) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float invert = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->invert = invert);
}

static MFUN(phaser_get_level) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->level;
}

static MFUN(phaser_set_level) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float level = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->level = level);
}

static MFUN(phaser_get_lfobpm) {
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->lfobpm;
}

static MFUN(phaser_set_lfobpm) {
  const m_uint gw_offset = SZ_INT;
  const GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
  m_float lfobpm = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->lfobpm = lfobpm);
}

typedef struct {
  sp_data* sp;
  sp_phasor* osc;
  m_bool is_init;
} GW_phasor;

static TICK(phasor_tick) {
  const GW_phasor* ug = (GW_phasor*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_phasor_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(phasor_ctor) {
  GW_phasor* ug = (GW_phasor*)xcalloc(1, sizeof(GW_phasor));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), phasor_tick, ug, 0);
}

static DTOR(phasor_dtor) {
  GW_phasor* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_phasor_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(phasor_init) {
  const m_uint gw_offset = SZ_INT;
  GW_phasor* ug = (GW_phasor*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_phasor_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float iphs = *(m_float*)(shred->mem + gw_offset);
  if(sp_phasor_create(&ug->osc) == SP_NOT_OK || sp_phasor_init(ug->sp, ug->osc, iphs) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(phasor_get_freq) {
  const GW_phasor* ug = (GW_phasor*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(phasor_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_phasor* ug = (GW_phasor*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(pinknoise_ctor) {
  GW_pinknoise* ug = (GW_pinknoise*)xcalloc(1, sizeof(GW_pinknoise));
  ug->sp = sp;
  sp_pinknoise_create(&ug->osc);
  sp_pinknoise_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pinknoise_tick, ug, 0);
}

static DTOR(pinknoise_dtor) {
  GW_pinknoise* ug = UGEN(o)->module.gen.data;
  sp_pinknoise_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(pinknoise_get_amp) {
  const GW_pinknoise* ug = (GW_pinknoise*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(pinknoise_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_pinknoise* ug = (GW_pinknoise*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_pitchamdf* osc;
  m_bool is_init;
} GW_pitchamdf;

static TICK(pitchamdf_tick) {
  const GW_pitchamdf* ug = (GW_pitchamdf*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_pitchamdf_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

static CTOR(pitchamdf_ctor) {
  GW_pitchamdf* ug = (GW_pitchamdf*)xcalloc(1, sizeof(GW_pitchamdf));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pitchamdf_tick, ug, 0);
}

static DTOR(pitchamdf_dtor) {
  GW_pitchamdf* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_pitchamdf_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(pitchamdf_init) {
  m_uint gw_offset = SZ_INT;
  GW_pitchamdf* ug = (GW_pitchamdf*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_pitchamdf_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float min = *(m_float*)(shred->mem + gw_offset);
  gw_offset +=SZ_FLOAT;
  m_float max = *(m_float*)(shred->mem + gw_offset);
  if(sp_pitchamdf_create(&ug->osc) == SP_NOT_OK || sp_pitchamdf_init(ug->sp, ug->osc, min, max) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_pluck* osc;
  m_bool is_init;
} GW_pluck;

static TICK(pluck_tick) {
  const GW_pluck* ug = (GW_pluck*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_pluck_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(pluck_ctor) {
  GW_pluck* ug = (GW_pluck*)xcalloc(1, sizeof(GW_pluck));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pluck_tick, ug, 1);
}

static DTOR(pluck_dtor) {
  GW_pluck* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_pluck_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(pluck_init) {
  const m_uint gw_offset = SZ_INT;
  GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_pluck_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float ifreq = *(m_float*)(shred->mem + gw_offset);
  if(sp_pluck_create(&ug->osc) == SP_NOT_OK || sp_pluck_init(ug->sp, ug->osc, ifreq) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(pluck_get_freq) {
  const GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(pluck_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(pluck_get_amp) {
  const GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(pluck_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
  sp_data* sp;
  sp_port* osc;
  m_bool is_init;
} GW_port;

static TICK(port_tick) {
  const GW_port* ug = (GW_port*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_port_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(port_ctor) {
  GW_port* ug = (GW_port*)xcalloc(1, sizeof(GW_port));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), port_tick, ug, 0);
}

static DTOR(port_dtor) {
  GW_port* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_port_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(port_init) {
  const m_uint gw_offset = SZ_INT;
  GW_port* ug = (GW_port*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_port_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float htime = *(m_float*)(shred->mem + gw_offset);
  if(sp_port_create(&ug->osc) == SP_NOT_OK || sp_port_init(ug->sp, ug->osc, htime) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(port_get_htime) {
  const GW_port* ug = (GW_port*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->htime;
}

static MFUN(port_set_htime) {
  const m_uint gw_offset = SZ_INT;
  const GW_port* ug = (GW_port*)UGEN(o)->module.gen.data;
  m_float htime = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->htime = htime);
}

typedef struct {
  sp_data* sp;
  sp_posc3* osc;
  m_bool is_init;
} GW_posc3;

static TICK(posc3_tick) {
  const GW_posc3* ug = (GW_posc3*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_posc3_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(posc3_ctor) {
  GW_posc3* ug = (GW_posc3*)xcalloc(1, sizeof(GW_posc3));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), posc3_tick, ug, 0);
}

static DTOR(posc3_dtor) {
  GW_posc3* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_posc3_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(posc3_init) {
  const m_uint gw_offset = SZ_INT;
  GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_posc3_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object tbl_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* tbl = FTBL(tbl_obj);
  release(tbl_obj, shred);
  if(sp_posc3_create(&ug->osc) == SP_NOT_OK || sp_posc3_init(ug->sp, ug->osc, tbl) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(posc3_get_freq) {
  const GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(posc3_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(posc3_get_amp) {
  const GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->amp;
}

static MFUN(posc3_set_amp) {
  const m_uint gw_offset = SZ_INT;
  const GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
  m_float amp = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(progress_ctor) {
  GW_progress* ug = (GW_progress*)xcalloc(1, sizeof(GW_progress));
  ug->sp = sp;
  sp_progress_create(&ug->osc);
  sp_progress_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), progress_tick, ug, 0);
}

static DTOR(progress_dtor) {
  GW_progress* ug = UGEN(o)->module.gen.data;
  sp_progress_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(progress_get_nbars) {
  const GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->nbars;
}

static MFUN(progress_set_nbars) {
  const m_uint gw_offset = SZ_INT;
  const GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
  m_int nbars = *(m_int*)(shred->mem + gw_offset);
  *(m_uint*)RETURN = (ug->osc->nbars = nbars);
}

static MFUN(progress_get_skip) {
  const GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->skip;
}

static MFUN(progress_set_skip) {
  const m_uint gw_offset = SZ_INT;
  const GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
  m_int skip = *(m_int*)(shred->mem + gw_offset);
  *(m_uint*)RETURN = (ug->osc->skip = skip);
}

typedef struct {
  sp_data* sp;
  sp_prop* osc;
  m_bool is_init;
} GW_prop;

static TICK(prop_tick) {
  const GW_prop* ug = (GW_prop*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_prop_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(prop_ctor) {
  GW_prop* ug = (GW_prop*)xcalloc(1, sizeof(GW_prop));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), prop_tick, ug, 0);
}

static DTOR(prop_dtor) {
  GW_prop* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_prop_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(prop_init) {
  const m_uint gw_offset = SZ_INT;
  GW_prop* ug = (GW_prop*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_prop_destroy(&ug->osc);
    ug->osc = NULL;
  }
  M_Object str_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str str = STRING(str_obj);
  release(str_obj, shred);
  if(sp_prop_create(&ug->osc) == SP_NOT_OK || sp_prop_init(ug->sp, ug->osc, str) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(prop_get_bpm) {
  const GW_prop* ug = (GW_prop*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bpm;
}

static MFUN(prop_set_bpm) {
  const m_uint gw_offset = SZ_INT;
  const GW_prop* ug = (GW_prop*)UGEN(o)->module.gen.data;
  m_float bpm = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(pshift_ctor) {
  GW_pshift* ug = (GW_pshift*)xcalloc(1, sizeof(GW_pshift));
  ug->sp = sp;
  sp_pshift_create(&ug->osc);
  sp_pshift_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), pshift_tick, ug, 0);
}

static DTOR(pshift_dtor) {
  GW_pshift* ug = UGEN(o)->module.gen.data;
  sp_pshift_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(pshift_get_shift) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->shift;
}

static MFUN(pshift_set_shift) {
  const m_uint gw_offset = SZ_INT;
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  m_float shift = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->shift = shift);
}

static MFUN(pshift_get_window) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->window;
}

static MFUN(pshift_set_window) {
  const m_uint gw_offset = SZ_INT;
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  m_float window = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->window = window);
}

static MFUN(pshift_get_xfade) {
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->xfade;
}

static MFUN(pshift_set_xfade) {
  const m_uint gw_offset = SZ_INT;
  const GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
  m_float xfade = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->xfade = xfade);
}

typedef struct {
  sp_data* sp;
  sp_ptrack* osc;
  m_bool is_init;
} GW_ptrack;

static TICK(ptrack_tick) {
  const GW_ptrack* ug = (GW_ptrack*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_ptrack_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

static CTOR(ptrack_ctor) {
  GW_ptrack* ug = (GW_ptrack*)xcalloc(1, sizeof(GW_ptrack));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), ptrack_tick, ug, 0);
}

static DTOR(ptrack_dtor) {
  GW_ptrack* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_ptrack_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(ptrack_init) {
  m_uint gw_offset = SZ_INT;
  GW_ptrack* ug = (GW_ptrack*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_ptrack_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_int ihopsize = *(m_int*)(shred->mem + gw_offset);
  gw_offset +=SZ_INT;
  m_int ipeaks = *(m_int*)(shred->mem + gw_offset);
  if(sp_ptrack_create(&ug->osc) == SP_NOT_OK || sp_ptrack_init(ug->sp, ug->osc, ihopsize, ipeaks) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_randh* osc;
} GW_randh;

static TICK(randh_tick) {
  const GW_randh* ug = (GW_randh*)u->module.gen.data;
  sp_randh_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(randh_ctor) {
  GW_randh* ug = (GW_randh*)xcalloc(1, sizeof(GW_randh));
  ug->sp = sp;
  sp_randh_create(&ug->osc);
  sp_randh_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), randh_tick, ug, 0);
}

static DTOR(randh_dtor) {
  GW_randh* ug = UGEN(o)->module.gen.data;
  sp_randh_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(randh_get_min) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(randh_set_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(randh_get_max) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(randh_set_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->max = max);
}

static MFUN(randh_get_freq) {
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(randh_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(randi_ctor) {
  GW_randi* ug = (GW_randi*)xcalloc(1, sizeof(GW_randi));
  ug->sp = sp;
  sp_randi_create(&ug->osc);
  sp_randi_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), randi_tick, ug, 0);
}

static DTOR(randi_dtor) {
  GW_randi* ug = UGEN(o)->module.gen.data;
  sp_randi_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(randi_get_min) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(randi_set_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(randi_get_max) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(randi_set_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->max = max);
}

static MFUN(randi_get_cps) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cps;
}

static MFUN(randi_set_cps) {
  const m_uint gw_offset = SZ_INT;
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  m_float cps = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->cps = cps);
}

static MFUN(randi_get_mode) {
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->mode;
}

static MFUN(randi_set_mode) {
  const m_uint gw_offset = SZ_INT;
  const GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
  m_float mode = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(random_ctor) {
  GW_random* ug = (GW_random*)xcalloc(1, sizeof(GW_random));
  ug->sp = sp;
  sp_random_create(&ug->osc);
  sp_random_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), random_tick, ug, 0);
}

static DTOR(random_dtor) {
  GW_random* ug = UGEN(o)->module.gen.data;
  sp_random_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(random_get_min) {
  const GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(random_set_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(random_get_max) {
  const GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(random_set_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(reson_ctor) {
  GW_reson* ug = (GW_reson*)xcalloc(1, sizeof(GW_reson));
  ug->sp = sp;
  sp_reson_create(&ug->osc);
  sp_reson_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), reson_tick, ug, 0);
}

static DTOR(reson_dtor) {
  GW_reson* ug = UGEN(o)->module.gen.data;
  sp_reson_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(reson_get_freq) {
  const GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(reson_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(reson_get_bw) {
  const GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->bw;
}

static MFUN(reson_set_bw) {
  const m_uint gw_offset = SZ_INT;
  const GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
  m_float bw = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->bw = bw);
}

typedef struct {
  sp_data* sp;
  sp_reverse* osc;
  m_bool is_init;
} GW_reverse;

static TICK(reverse_tick) {
  const GW_reverse* ug = (GW_reverse*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_reverse_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(reverse_ctor) {
  GW_reverse* ug = (GW_reverse*)xcalloc(1, sizeof(GW_reverse));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), reverse_tick, ug, 0);
}

static DTOR(reverse_dtor) {
  GW_reverse* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_reverse_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(reverse_init) {
  const m_uint gw_offset = SZ_INT;
  GW_reverse* ug = (GW_reverse*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_reverse_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float delay = *(m_float*)(shred->mem + gw_offset);
  if(sp_reverse_create(&ug->osc) == SP_NOT_OK || sp_reverse_init(ug->sp, ug->osc, delay) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_revsc* osc;
} GW_revsc;

static TICK(revsc_tick) {
  const GW_revsc* ug = (GW_revsc*)u->module.gen.data;
  sp_revsc_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

static CTOR(revsc_ctor) {
  GW_revsc* ug = (GW_revsc*)xcalloc(1, sizeof(GW_revsc));
  ug->sp = sp;
  sp_revsc_create(&ug->osc);
  sp_revsc_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), revsc_tick, ug, 0);
}

static DTOR(revsc_dtor) {
  GW_revsc* ug = UGEN(o)->module.gen.data;
  sp_revsc_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(revsc_get_feedback) {
  const GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->feedback;
}

static MFUN(revsc_set_feedback) {
  const m_uint gw_offset = SZ_INT;
  const GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
  m_float feedback = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->feedback = feedback);
}

static MFUN(revsc_get_lpfreq) {
  const GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->lpfreq;
}

static MFUN(revsc_set_lpfreq) {
  const m_uint gw_offset = SZ_INT;
  const GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
  m_float lpfreq = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(rms_ctor) {
  GW_rms* ug = (GW_rms*)xcalloc(1, sizeof(GW_rms));
  ug->sp = sp;
  sp_rms_create(&ug->osc);
  sp_rms_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), rms_tick, ug, 0);
}

static DTOR(rms_dtor) {
  GW_rms* ug = UGEN(o)->module.gen.data;
  sp_rms_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(rms_get_ihp) {
  const GW_rms* ug = (GW_rms*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->ihp;
}

static MFUN(rms_set_ihp) {
  const m_uint gw_offset = SZ_INT;
  const GW_rms* ug = (GW_rms*)UGEN(o)->module.gen.data;
  m_float ihp = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->ihp = ihp);
}

typedef struct {
  sp_data* sp;
  sp_rpt* osc;
  m_bool is_init;
} GW_rpt;

static TICK(rpt_tick) {
  const GW_rpt* ug = (GW_rpt*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_rpt_compute(ug->sp, ug->osc, &u->in, &u->module.gen.trig->in, &u->out);

}

static CTOR(rpt_ctor) {
  GW_rpt* ug = (GW_rpt*)xcalloc(1, sizeof(GW_rpt));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), rpt_tick, ug, 1);
}

static DTOR(rpt_dtor) {
  GW_rpt* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_rpt_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(rpt_init) {
  const m_uint gw_offset = SZ_INT;
  GW_rpt* ug = (GW_rpt*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_rpt_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float maxdur = *(m_float*)(shred->mem + gw_offset);
  if(sp_rpt_create(&ug->osc) == SP_NOT_OK || sp_rpt_init(ug->sp, ug->osc, maxdur) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_rspline* osc;
} GW_rspline;

static TICK(rspline_tick) {
  const GW_rspline* ug = (GW_rspline*)u->module.gen.data;
  sp_rspline_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(rspline_ctor) {
  GW_rspline* ug = (GW_rspline*)xcalloc(1, sizeof(GW_rspline));
  ug->sp = sp;
  sp_rspline_create(&ug->osc);
  sp_rspline_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), rspline_tick, ug, 0);
}

static DTOR(rspline_dtor) {
  GW_rspline* ug = UGEN(o)->module.gen.data;
  sp_rspline_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(rspline_get_min) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(rspline_set_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(rspline_get_max) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(rspline_set_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->max = max);
}

static MFUN(rspline_get_cps_min) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cps_min;
}

static MFUN(rspline_set_cps_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  m_float cps_min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->cps_min = cps_min);
}

static MFUN(rspline_get_cps_max) {
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cps_max;
}

static MFUN(rspline_set_cps_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
  m_float cps_max = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(samphold_ctor) {
  GW_samphold* ug = (GW_samphold*)xcalloc(1, sizeof(GW_samphold));
  ug->sp = sp;
  sp_samphold_create(&ug->osc);
  sp_samphold_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), samphold_tick, ug, 1);
}

static DTOR(samphold_dtor) {
  GW_samphold* ug = UGEN(o)->module.gen.data;
  sp_samphold_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_saturator* osc;
} GW_saturator;

static TICK(saturator_tick) {
  const GW_saturator* ug = (GW_saturator*)u->module.gen.data;
  sp_saturator_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(saturator_ctor) {
  GW_saturator* ug = (GW_saturator*)xcalloc(1, sizeof(GW_saturator));
  ug->sp = sp;
  sp_saturator_create(&ug->osc);
  sp_saturator_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), saturator_tick, ug, 0);
}

static DTOR(saturator_dtor) {
  GW_saturator* ug = UGEN(o)->module.gen.data;
  sp_saturator_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(saturator_get_drive) {
  const GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->drive;
}

static MFUN(saturator_set_drive) {
  const m_uint gw_offset = SZ_INT;
  const GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
  m_float drive = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->drive = drive);
}

static MFUN(saturator_get_dcoffset) {
  const GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dcoffset;
}

static MFUN(saturator_set_dcoffset) {
  const m_uint gw_offset = SZ_INT;
  const GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
  m_float dcoffset = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(scale_ctor) {
  GW_scale* ug = (GW_scale*)xcalloc(1, sizeof(GW_scale));
  ug->sp = sp;
  sp_scale_create(&ug->osc);
  sp_scale_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), scale_tick, ug, 0);
}

static DTOR(scale_dtor) {
  GW_scale* ug = UGEN(o)->module.gen.data;
  sp_scale_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(scale_get_min) {
  const GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(scale_set_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(scale_get_max) {
  const GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(scale_set_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
  sp_data* sp;
  sp_sdelay* osc;
  m_bool is_init;
} GW_sdelay;

static TICK(sdelay_tick) {
  const GW_sdelay* ug = (GW_sdelay*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_sdelay_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(sdelay_ctor) {
  GW_sdelay* ug = (GW_sdelay*)xcalloc(1, sizeof(GW_sdelay));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), sdelay_tick, ug, 0);
}

static DTOR(sdelay_dtor) {
  GW_sdelay* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_sdelay_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(sdelay_init) {
  const m_uint gw_offset = SZ_INT;
  GW_sdelay* ug = (GW_sdelay*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_sdelay_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float size = *(m_float*)(shred->mem + gw_offset);
  if(sp_sdelay_create(&ug->osc) == SP_NOT_OK || sp_sdelay_init(ug->sp, ug->osc, size) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_slice* osc;
  m_bool is_init;
} GW_slice;

static TICK(slice_tick) {
  const GW_slice* ug = (GW_slice*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_slice_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(slice_ctor) {
  GW_slice* ug = (GW_slice*)xcalloc(1, sizeof(GW_slice));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), slice_tick, ug, 1);
}

static DTOR(slice_dtor) {
  GW_slice* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_slice_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(slice_init) {
  m_uint gw_offset = SZ_INT;
  GW_slice* ug = (GW_slice*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_slice_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object vals_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* vals = FTBL(vals_obj);
  release(vals_obj, shred);
  gw_offset +=SZ_INT;
  const M_Object buf_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* buf = FTBL(buf_obj);
  release(buf_obj, shred);
  if(sp_slice_create(&ug->osc) == SP_NOT_OK || sp_slice_init(ug->sp, ug->osc, vals, buf) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(slice_get_id) {
  const GW_slice* ug = (GW_slice*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->id;
}

static MFUN(slice_set_id) {
  const m_uint gw_offset = SZ_INT;
  const GW_slice* ug = (GW_slice*)UGEN(o)->module.gen.data;
  m_float id = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->id = id);
}

typedef struct {
  sp_data* sp;
  sp_smoothdelay* osc;
  m_bool is_init;
} GW_smoothdelay;

static TICK(smoothdelay_tick) {
  const GW_smoothdelay* ug = (GW_smoothdelay*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_smoothdelay_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(smoothdelay_ctor) {
  GW_smoothdelay* ug = (GW_smoothdelay*)xcalloc(1, sizeof(GW_smoothdelay));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), smoothdelay_tick, ug, 0);
}

static DTOR(smoothdelay_dtor) {
  GW_smoothdelay* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_smoothdelay_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(smoothdelay_init) {
  m_uint gw_offset = SZ_INT;
  GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_smoothdelay_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float maxdel = *(m_float*)(shred->mem + gw_offset);
  gw_offset +=SZ_FLOAT;
  m_int interp = *(m_int*)(shred->mem + gw_offset);
  if(sp_smoothdelay_create(&ug->osc) == SP_NOT_OK || sp_smoothdelay_init(ug->sp, ug->osc, maxdel, interp) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(smoothdelay_get_feedback) {
  const GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->feedback;
}

static MFUN(smoothdelay_set_feedback) {
  const m_uint gw_offset = SZ_INT;
  const GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  m_float feedback = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->feedback = feedback);
}

static MFUN(smoothdelay_get_del) {
  const GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->del;
}

static MFUN(smoothdelay_set_del) {
  const m_uint gw_offset = SZ_INT;
  const GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
  m_float del = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->del = del);
}

typedef struct {
  sp_data* sp;
  sp_spa* osc;
  m_bool is_init;
} GW_spa;

static TICK(spa_tick) {
  const GW_spa* ug = (GW_spa*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_spa_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(spa_ctor) {
  GW_spa* ug = (GW_spa*)xcalloc(1, sizeof(GW_spa));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), spa_tick, ug, 0);
}

static DTOR(spa_dtor) {
  GW_spa* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_spa_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(spa_init) {
  const m_uint gw_offset = SZ_INT;
  GW_spa* ug = (GW_spa*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_spa_destroy(&ug->osc);
    ug->osc = NULL;
  }
  M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str filename = STRING(filename_obj);
  release(filename_obj, shred);
  if(sp_spa_create(&ug->osc) == SP_NOT_OK || sp_spa_init(ug->sp, ug->osc, filename) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_sparec* osc;
  m_bool is_init;
} GW_sparec;

static TICK(sparec_tick) {
  const GW_sparec* ug = (GW_sparec*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_sparec_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(sparec_ctor) {
  GW_sparec* ug = (GW_sparec*)xcalloc(1, sizeof(GW_sparec));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), sparec_tick, ug, 0);
}

static DTOR(sparec_dtor) {
  GW_sparec* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_sparec_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(sparec_init) {
  const m_uint gw_offset = SZ_INT;
  GW_sparec* ug = (GW_sparec*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_sparec_destroy(&ug->osc);
    ug->osc = NULL;
  }
  M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str filename = STRING(filename_obj);
  release(filename_obj, shred);
  if(sp_sparec_create(&ug->osc) == SP_NOT_OK || sp_sparec_init(ug->sp, ug->osc, filename) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_streson* osc;
} GW_streson;

static TICK(streson_tick) {
  const GW_streson* ug = (GW_streson*)u->module.gen.data;
  sp_streson_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(streson_ctor) {
  GW_streson* ug = (GW_streson*)xcalloc(1, sizeof(GW_streson));
  ug->sp = sp;
  sp_streson_create(&ug->osc);
  sp_streson_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), streson_tick, ug, 0);
}

static DTOR(streson_dtor) {
  GW_streson* ug = UGEN(o)->module.gen.data;
  sp_streson_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(streson_get_freq) {
  const GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->freq;
}

static MFUN(streson_set_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
  m_float freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->freq = freq);
}

static MFUN(streson_get_fdbgain) {
  const GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->fdbgain;
}

static MFUN(streson_set_fdbgain) {
  const m_uint gw_offset = SZ_INT;
  const GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
  m_float fdbgain = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(switch_ctor) {
  GW_switch* ug = (GW_switch*)xcalloc(1, sizeof(GW_switch));
  ug->sp = sp;
  sp_switch_create(&ug->osc);
  sp_switch_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 3, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), switch_tick, ug, 1);
}

static DTOR(switch_dtor) {
  GW_switch* ug = UGEN(o)->module.gen.data;
  sp_switch_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_tabread* osc;
  m_bool is_init;
} GW_tabread;

static TICK(tabread_tick) {
  const GW_tabread* ug = (GW_tabread*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_tabread_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(tabread_ctor) {
  GW_tabread* ug = (GW_tabread*)xcalloc(1, sizeof(GW_tabread));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tabread_tick, ug, 0);
}

static DTOR(tabread_dtor) {
  GW_tabread* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_tabread_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(tabread_init) {
  m_uint gw_offset = SZ_INT;
  GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_tabread_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* ft = FTBL(ft_obj);
  release(ft_obj, shred);
  gw_offset +=SZ_INT;
  m_float mode = *(m_float*)(shred->mem + gw_offset);
  if(sp_tabread_create(&ug->osc) == SP_NOT_OK || sp_tabread_init(ug->sp, ug->osc, ft, mode) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(tabread_get_index) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->index;
}

static MFUN(tabread_set_index) {
  const m_uint gw_offset = SZ_INT;
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  m_float index = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->index = index);
}

static MFUN(tabread_get_offset) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->offset;
}

static MFUN(tabread_set_offset) {
  const m_uint gw_offset = SZ_INT;
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  m_float offset = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->offset = offset);
}

static MFUN(tabread_get_wrap) {
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->wrap;
}

static MFUN(tabread_set_wrap) {
  const m_uint gw_offset = SZ_INT;
  const GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
  m_float wrap = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(tadsr_ctor) {
  GW_tadsr* ug = (GW_tadsr*)xcalloc(1, sizeof(GW_tadsr));
  ug->sp = sp;
  sp_tadsr_create(&ug->osc);
  sp_tadsr_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tadsr_tick, ug, 1);
}

static DTOR(tadsr_dtor) {
  GW_tadsr* ug = UGEN(o)->module.gen.data;
  sp_tadsr_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(tadsr_get_atk) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(tadsr_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(tadsr_get_dec) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dec;
}

static MFUN(tadsr_set_dec) {
  const m_uint gw_offset = SZ_INT;
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  m_float dec = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dec = dec);
}

static MFUN(tadsr_get_sus) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->sus;
}

static MFUN(tadsr_set_sus) {
  const m_uint gw_offset = SZ_INT;
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  m_float sus = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->sus = sus);
}

static MFUN(tadsr_get_rel) {
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(tadsr_set_rel) {
  const m_uint gw_offset = SZ_INT;
  const GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(talkbox_ctor) {
  GW_talkbox* ug = (GW_talkbox*)xcalloc(1, sizeof(GW_talkbox));
  ug->sp = sp;
  sp_talkbox_create(&ug->osc);
  sp_talkbox_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), talkbox_tick, ug, 0);
}

static DTOR(talkbox_dtor) {
  GW_talkbox* ug = UGEN(o)->module.gen.data;
  sp_talkbox_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(talkbox_get_quality) {
  const GW_talkbox* ug = (GW_talkbox*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->quality;
}

static MFUN(talkbox_set_quality) {
  const m_uint gw_offset = SZ_INT;
  const GW_talkbox* ug = (GW_talkbox*)UGEN(o)->module.gen.data;
  m_float quality = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->quality = quality);
}

typedef struct {
  sp_data* sp;
  sp_tblrec* osc;
  m_bool is_init;
} GW_tblrec;

static TICK(tblrec_tick) {
  const GW_tblrec* ug = (GW_tblrec*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_tblrec_compute(ug->sp, ug->osc, &u->in, &u->module.gen.trig->in, &u->out);

}

static CTOR(tblrec_ctor) {
  GW_tblrec* ug = (GW_tblrec*)xcalloc(1, sizeof(GW_tblrec));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tblrec_tick, ug, 1);
}

static DTOR(tblrec_dtor) {
  GW_tblrec* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_tblrec_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(tblrec_init) {
  const m_uint gw_offset = SZ_INT;
  GW_tblrec* ug = (GW_tblrec*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_tblrec_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object bar_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* bar = FTBL(bar_obj);
  release(bar_obj, shred);
  if(sp_tblrec_create(&ug->osc) == SP_NOT_OK || sp_tblrec_init(ug->sp, ug->osc, bar) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_tbvcf* osc;
} GW_tbvcf;

static TICK(tbvcf_tick) {
  const GW_tbvcf* ug = (GW_tbvcf*)u->module.gen.data;
  sp_tbvcf_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(tbvcf_ctor) {
  GW_tbvcf* ug = (GW_tbvcf*)xcalloc(1, sizeof(GW_tbvcf));
  ug->sp = sp;
  sp_tbvcf_create(&ug->osc);
  sp_tbvcf_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tbvcf_tick, ug, 0);
}

static DTOR(tbvcf_dtor) {
  GW_tbvcf* ug = UGEN(o)->module.gen.data;
  sp_tbvcf_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(tbvcf_get_fco) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->fco;
}

static MFUN(tbvcf_set_fco) {
  const m_uint gw_offset = SZ_INT;
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  m_float fco = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->fco = fco);
}

static MFUN(tbvcf_get_res) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(tbvcf_set_res) {
  const m_uint gw_offset = SZ_INT;
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->res = res);
}

static MFUN(tbvcf_get_dist) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dist;
}

static MFUN(tbvcf_set_dist) {
  const m_uint gw_offset = SZ_INT;
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  m_float dist = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dist = dist);
}

static MFUN(tbvcf_get_asym) {
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->asym;
}

static MFUN(tbvcf_set_asym) {
  const m_uint gw_offset = SZ_INT;
  const GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
  m_float asym = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(tdiv_ctor) {
  GW_tdiv* ug = (GW_tdiv*)xcalloc(1, sizeof(GW_tdiv));
  ug->sp = sp;
  sp_tdiv_create(&ug->osc);
  sp_tdiv_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tdiv_tick, ug, 1);
}

static DTOR(tdiv_dtor) {
  GW_tdiv* ug = UGEN(o)->module.gen.data;
  sp_tdiv_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(tdiv_get_num) {
  const GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->num;
}

static MFUN(tdiv_set_num) {
  const m_uint gw_offset = SZ_INT;
  const GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
  m_float num = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->num = num);
}

static MFUN(tdiv_get_offset) {
  const GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->offset;
}

static MFUN(tdiv_set_offset) {
  const m_uint gw_offset = SZ_INT;
  const GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
  m_float offset = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(tenv_ctor) {
  GW_tenv* ug = (GW_tenv*)xcalloc(1, sizeof(GW_tenv));
  ug->sp = sp;
  sp_tenv_create(&ug->osc);
  sp_tenv_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tenv_tick, ug, 1);
}

static DTOR(tenv_dtor) {
  GW_tenv* ug = UGEN(o)->module.gen.data;
  sp_tenv_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(tenv_get_atk) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(tenv_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(tenv_get_hold) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->hold;
}

static MFUN(tenv_set_hold) {
  const m_uint gw_offset = SZ_INT;
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  m_float hold = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->hold = hold);
}

static MFUN(tenv_get_rel) {
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(tenv_set_rel) {
  const m_uint gw_offset = SZ_INT;
  const GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(tenv2_ctor) {
  GW_tenv2* ug = (GW_tenv2*)xcalloc(1, sizeof(GW_tenv2));
  ug->sp = sp;
  sp_tenv2_create(&ug->osc);
  sp_tenv2_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tenv2_tick, ug, 1);
}

static DTOR(tenv2_dtor) {
  GW_tenv2* ug = UGEN(o)->module.gen.data;
  sp_tenv2_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(tenv2_get_atk) {
  const GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(tenv2_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(tenv2_get_rel) {
  const GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(tenv2_set_rel) {
  const m_uint gw_offset = SZ_INT;
  const GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(tenvx_ctor) {
  GW_tenvx* ug = (GW_tenvx*)xcalloc(1, sizeof(GW_tenvx));
  ug->sp = sp;
  sp_tenvx_create(&ug->osc);
  sp_tenvx_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tenvx_tick, ug, 1);
}

static DTOR(tenvx_dtor) {
  GW_tenvx* ug = UGEN(o)->module.gen.data;
  sp_tenvx_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(tenvx_get_atk) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->atk;
}

static MFUN(tenvx_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->atk = atk);
}

static MFUN(tenvx_get_hold) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->hold;
}

static MFUN(tenvx_set_hold) {
  const m_uint gw_offset = SZ_INT;
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  m_float hold = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->hold = hold);
}

static MFUN(tenvx_get_rel) {
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rel;
}

static MFUN(tenvx_set_rel) {
  const m_uint gw_offset = SZ_INT;
  const GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(tgate_ctor) {
  GW_tgate* ug = (GW_tgate*)xcalloc(1, sizeof(GW_tgate));
  ug->sp = sp;
  sp_tgate_create(&ug->osc);
  sp_tgate_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tgate_tick, ug, 1);
}

static DTOR(tgate_dtor) {
  GW_tgate* ug = UGEN(o)->module.gen.data;
  sp_tgate_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(tgate_get_time) {
  const GW_tgate* ug = (GW_tgate*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->time;
}

static MFUN(tgate_set_time) {
  const m_uint gw_offset = SZ_INT;
  const GW_tgate* ug = (GW_tgate*)UGEN(o)->module.gen.data;
  m_float time = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(thresh_ctor) {
  GW_thresh* ug = (GW_thresh*)xcalloc(1, sizeof(GW_thresh));
  ug->sp = sp;
  sp_thresh_create(&ug->osc);
  sp_thresh_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), thresh_tick, ug, 0);
}

static DTOR(thresh_dtor) {
  GW_thresh* ug = UGEN(o)->module.gen.data;
  sp_thresh_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(thresh_get_thresh) {
  const GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->thresh;
}

static MFUN(thresh_set_thresh) {
  const m_uint gw_offset = SZ_INT;
  const GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
  m_float thresh = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->thresh = thresh);
}

static MFUN(thresh_get_mode) {
  const GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->mode;
}

static MFUN(thresh_set_mode) {
  const m_uint gw_offset = SZ_INT;
  const GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
  m_int mode = *(m_int*)(shred->mem + gw_offset);
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

static CTOR(timer_ctor) {
  GW_timer* ug = (GW_timer*)xcalloc(1, sizeof(GW_timer));
  ug->sp = sp;
  sp_timer_create(&ug->osc);
  sp_timer_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), timer_tick, ug, 0);
}

static DTOR(timer_dtor) {
  GW_timer* ug = UGEN(o)->module.gen.data;
  sp_timer_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_tin* osc;
} GW_tin;

static TICK(tin_tick) {
  const GW_tin* ug = (GW_tin*)u->module.gen.data;
  sp_tin_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(tin_ctor) {
  GW_tin* ug = (GW_tin*)xcalloc(1, sizeof(GW_tin));
  ug->sp = sp;
  sp_tin_create(&ug->osc);
  sp_tin_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tin_tick, ug, 1);
}

static DTOR(tin_dtor) {
  GW_tin* ug = UGEN(o)->module.gen.data;
  sp_tin_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_tone* osc;
} GW_tone;

static TICK(tone_tick) {
  const GW_tone* ug = (GW_tone*)u->module.gen.data;
  sp_tone_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(tone_ctor) {
  GW_tone* ug = (GW_tone*)xcalloc(1, sizeof(GW_tone));
  ug->sp = sp;
  sp_tone_create(&ug->osc);
  sp_tone_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tone_tick, ug, 0);
}

static DTOR(tone_dtor) {
  GW_tone* ug = UGEN(o)->module.gen.data;
  sp_tone_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(tone_get_hp) {
  const GW_tone* ug = (GW_tone*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->hp;
}

static MFUN(tone_set_hp) {
  const m_uint gw_offset = SZ_INT;
  const GW_tone* ug = (GW_tone*)UGEN(o)->module.gen.data;
  m_float hp = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(trand_ctor) {
  GW_trand* ug = (GW_trand*)xcalloc(1, sizeof(GW_trand));
  ug->sp = sp;
  sp_trand_create(&ug->osc);
  sp_trand_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), trand_tick, ug, 1);
}

static DTOR(trand_dtor) {
  GW_trand* ug = UGEN(o)->module.gen.data;
  sp_trand_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(trand_get_min) {
  const GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->min;
}

static MFUN(trand_set_min) {
  const m_uint gw_offset = SZ_INT;
  const GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
  m_float min = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->min = min);
}

static MFUN(trand_get_max) {
  const GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->max;
}

static MFUN(trand_set_max) {
  const m_uint gw_offset = SZ_INT;
  const GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
  m_float max = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
  sp_data* sp;
  sp_tseg* osc;
  m_bool is_init;
} GW_tseg;

static TICK(tseg_tick) {
  const GW_tseg* ug = (GW_tseg*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_tseg_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(tseg_ctor) {
  GW_tseg* ug = (GW_tseg*)xcalloc(1, sizeof(GW_tseg));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tseg_tick, ug, 1);
}

static DTOR(tseg_dtor) {
  GW_tseg* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_tseg_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(tseg_init) {
  const m_uint gw_offset = SZ_INT;
  GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_tseg_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float ibeg = *(m_float*)(shred->mem + gw_offset);
  if(sp_tseg_create(&ug->osc) == SP_NOT_OK || sp_tseg_init(ug->sp, ug->osc, ibeg) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(tseg_get_end) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->end;
}

static MFUN(tseg_set_end) {
  const m_uint gw_offset = SZ_INT;
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  m_float end = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->end = end);
}

static MFUN(tseg_get_dur) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->dur;
}

static MFUN(tseg_set_dur) {
  const m_uint gw_offset = SZ_INT;
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  m_float dur = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->dur = dur);
}

static MFUN(tseg_get_type) {
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->type;
}

static MFUN(tseg_set_type) {
  const m_uint gw_offset = SZ_INT;
  const GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
  m_float type = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->type = type);
}

typedef struct {
  sp_data* sp;
  sp_tseq* osc;
  m_bool is_init;
} GW_tseq;

static TICK(tseq_tick) {
  const GW_tseq* ug = (GW_tseq*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_tseq_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

static CTOR(tseq_ctor) {
  GW_tseq* ug = (GW_tseq*)xcalloc(1, sizeof(GW_tseq));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tseq_tick, ug, 1);
}

static DTOR(tseq_dtor) {
  GW_tseq* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_tseq_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(tseq_init) {
  const m_uint gw_offset = SZ_INT;
  GW_tseq* ug = (GW_tseq*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_tseq_destroy(&ug->osc);
    ug->osc = NULL;
  }
  const M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
  sp_ftbl* ft = FTBL(ft_obj);
  release(ft_obj, shred);
  if(sp_tseq_create(&ug->osc) == SP_NOT_OK || sp_tseq_init(ug->sp, ug->osc, ft) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(tseq_get_shuf) {
  const GW_tseq* ug = (GW_tseq*)UGEN(o)->module.gen.data;
  *(m_uint*)RETURN = ug->osc->shuf;
}

static MFUN(tseq_set_shuf) {
  const m_uint gw_offset = SZ_INT;
  const GW_tseq* ug = (GW_tseq*)UGEN(o)->module.gen.data;
  m_int shuf = *(m_int*)(shred->mem + gw_offset);
  *(m_uint*)RETURN = (ug->osc->shuf = shuf);
}

typedef struct {
  sp_data* sp;
  sp_vdelay* osc;
  m_bool is_init;
} GW_vdelay;

static TICK(vdelay_tick) {
  const GW_vdelay* ug = (GW_vdelay*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_vdelay_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(vdelay_ctor) {
  GW_vdelay* ug = (GW_vdelay*)xcalloc(1, sizeof(GW_vdelay));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), vdelay_tick, ug, 0);
}

static DTOR(vdelay_dtor) {
  GW_vdelay* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_vdelay_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(vdelay_init) {
  const m_uint gw_offset = SZ_INT;
  GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_vdelay_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float maxdel = *(m_float*)(shred->mem + gw_offset);
  if(sp_vdelay_create(&ug->osc) == SP_NOT_OK || sp_vdelay_init(ug->sp, ug->osc, maxdel) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(vdelay_get_del) {
  const GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->del;
}

static MFUN(vdelay_set_del) {
  const m_uint gw_offset = SZ_INT;
  const GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  m_float del = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->del = del);
}

static MFUN(vdelay_get_feedback) {
  const GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->feedback;
}

static MFUN(vdelay_set_feedback) {
  const m_uint gw_offset = SZ_INT;
  const GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
  m_float feedback = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(voc_ctor) {
  GW_voc* ug = (GW_voc*)xcalloc(1, sizeof(GW_voc));
  ug->sp = sp;
  sp_voc_create(&ug->osc);
  sp_voc_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), voc_tick, ug, 0);
}

static DTOR(voc_dtor) {
  GW_voc* ug = UGEN(o)->module.gen.data;
  sp_voc_destroy(&ug->osc);
  xfree(ug);
}

typedef struct {
  sp_data* sp;
  sp_vocoder* osc;
} GW_vocoder;

static TICK(vocoder_tick) {
  const GW_vocoder* ug = (GW_vocoder*)u->module.gen.data;
  sp_vocoder_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);

}

static CTOR(vocoder_ctor) {
  GW_vocoder* ug = (GW_vocoder*)xcalloc(1, sizeof(GW_vocoder));
  ug->sp = sp;
  sp_vocoder_create(&ug->osc);
  sp_vocoder_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), vocoder_tick, ug, 0);
}

static DTOR(vocoder_dtor) {
  GW_vocoder* ug = UGEN(o)->module.gen.data;
  sp_vocoder_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(vocoder_get_atk) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->atk;
}

static MFUN(vocoder_set_atk) {
  const m_uint gw_offset = SZ_INT;
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  m_float atk = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->atk = atk);
}

static MFUN(vocoder_get_rel) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->rel;
}

static MFUN(vocoder_set_rel) {
  const m_uint gw_offset = SZ_INT;
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  m_float rel = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->rel = rel);
}

static MFUN(vocoder_get_bwratio) {
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->bwratio;
}

static MFUN(vocoder_set_bwratio) {
  const m_uint gw_offset = SZ_INT;
  const GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
  m_float bwratio = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->bwratio = bwratio);
}

typedef struct {
  sp_data* sp;
  sp_waveset* osc;
  m_bool is_init;
} GW_waveset;

static TICK(waveset_tick) {
  const GW_waveset* ug = (GW_waveset*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_waveset_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(waveset_ctor) {
  GW_waveset* ug = (GW_waveset*)xcalloc(1, sizeof(GW_waveset));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), waveset_tick, ug, 0);
}

static DTOR(waveset_dtor) {
  GW_waveset* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_waveset_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(waveset_init) {
  const m_uint gw_offset = SZ_INT;
  GW_waveset* ug = (GW_waveset*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_waveset_destroy(&ug->osc);
    ug->osc = NULL;
  }
  m_float ilen = *(m_float*)(shred->mem + gw_offset);
  if(sp_waveset_create(&ug->osc) == SP_NOT_OK || sp_waveset_init(ug->sp, ug->osc, ilen) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

static MFUN(waveset_get_rep) {
  const GW_waveset* ug = (GW_waveset*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->rep;
}

static MFUN(waveset_set_rep) {
  const m_uint gw_offset = SZ_INT;
  const GW_waveset* ug = (GW_waveset*)UGEN(o)->module.gen.data;
  m_float rep = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->rep = rep);
}

typedef struct {
  sp_data* sp;
  sp_wavin* osc;
  m_bool is_init;
} GW_wavin;

static TICK(wavin_tick) {
  const GW_wavin* ug = (GW_wavin*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_wavin_compute(ug->sp, ug->osc, NULL, &u->out);

}

static CTOR(wavin_ctor) {
  GW_wavin* ug = (GW_wavin*)xcalloc(1, sizeof(GW_wavin));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), wavin_tick, ug, 0);
}

static DTOR(wavin_dtor) {
  GW_wavin* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_wavin_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(wavin_init) {
  const m_uint gw_offset = SZ_INT;
  GW_wavin* ug = (GW_wavin*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_wavin_destroy(&ug->osc);
    ug->osc = NULL;
  }
  M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str filename = STRING(filename_obj);
  release(filename_obj, shred);
  if(sp_wavin_create(&ug->osc) == SP_NOT_OK || sp_wavin_init(ug->sp, ug->osc, filename) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_wavout* osc;
  m_bool is_init;
} GW_wavout;

static TICK(wavout_tick) {
  const GW_wavout* ug = (GW_wavout*)u->module.gen.data;
  if(!ug->is_init) { // LCOV_EXCL_START
    u->out = 0;
    return;
  } // LCOV_EXCL_STOP
  sp_wavout_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(wavout_ctor) {
  GW_wavout* ug = (GW_wavout*)xcalloc(1, sizeof(GW_wavout));
  ug->sp = sp;
  ug->is_init = 0;
  ug->osc = NULL;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), wavout_tick, ug, 0);
}

static DTOR(wavout_dtor) {
  GW_wavout* ug = UGEN(o)->module.gen.data;
  if(ug->is_init) {

    sp_wavout_destroy(&ug->osc);
  }
  xfree(ug);
}

static MFUN(wavout_init) {
  const m_uint gw_offset = SZ_INT;
  GW_wavout* ug = (GW_wavout*)UGEN(o)->module.gen.data;
  if(ug->osc) {
    sp_wavout_destroy(&ug->osc);
    ug->osc = NULL;
  }
  M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
  m_str filename = STRING(filename_obj);
  release(filename_obj, shred);
  if(sp_wavout_create(&ug->osc) == SP_NOT_OK || sp_wavout_init(ug->sp, ug->osc, filename) == SP_NOT_OK) {
    Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
  }
  ug->is_init = 1;
}

typedef struct {
  sp_data* sp;
  sp_wpkorg35* osc;
} GW_wpkorg35;

static TICK(wpkorg35_tick) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)u->module.gen.data;
  sp_wpkorg35_compute(ug->sp, ug->osc, &u->in, &u->out);

}

static CTOR(wpkorg35_ctor) {
  GW_wpkorg35* ug = (GW_wpkorg35*)xcalloc(1, sizeof(GW_wpkorg35));
  ug->sp = sp;
  sp_wpkorg35_create(&ug->osc);
  sp_wpkorg35_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), wpkorg35_tick, ug, 0);
}

static DTOR(wpkorg35_dtor) {
  GW_wpkorg35* ug = UGEN(o)->module.gen.data;
  sp_wpkorg35_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(wpkorg35_get_cutoff) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->cutoff;
}

static MFUN(wpkorg35_set_cutoff) {
  const m_uint gw_offset = SZ_INT;
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  m_float cutoff = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->cutoff = cutoff);
}

static MFUN(wpkorg35_get_res) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->res;
}

static MFUN(wpkorg35_set_res) {
  const m_uint gw_offset = SZ_INT;
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  m_float res = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (ug->osc->res = res);
}

static MFUN(wpkorg35_get_saturation) {
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = ug->osc->saturation;
}

static MFUN(wpkorg35_set_saturation) {
  const m_uint gw_offset = SZ_INT;
  const GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
  m_float saturation = *(m_float*)(shred->mem + gw_offset);
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

static CTOR(zitarev_ctor) {
  GW_zitarev* ug = (GW_zitarev*)xcalloc(1, sizeof(GW_zitarev));
  ug->sp = sp;
  sp_zitarev_create(&ug->osc);
  sp_zitarev_init(ug->sp, ug->osc);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), zitarev_tick, ug, 0);
}

static DTOR(zitarev_dtor) {
  GW_zitarev* ug = UGEN(o)->module.gen.data;
  sp_zitarev_destroy(&ug->osc);
  xfree(ug);
}

static MFUN(zitarev_get_in_delay) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->in_delay;
}

static MFUN(zitarev_set_in_delay) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float in_delay = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->in_delay = in_delay);
}

static MFUN(zitarev_get_lf_x) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->lf_x;
}

static MFUN(zitarev_set_lf_x) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float lf_x = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->lf_x = lf_x);
}

static MFUN(zitarev_get_rt60_low) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->rt60_low;
}

static MFUN(zitarev_set_rt60_low) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float rt60_low = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->rt60_low = rt60_low);
}

static MFUN(zitarev_get_rt60_mid) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->rt60_mid;
}

static MFUN(zitarev_set_rt60_mid) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float rt60_mid = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->rt60_mid = rt60_mid);
}

static MFUN(zitarev_get_hf_damping) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->hf_damping;
}

static MFUN(zitarev_set_hf_damping) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float hf_damping = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->hf_damping = hf_damping);
}

static MFUN(zitarev_get_eq1_freq) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->eq1_freq;
}

static MFUN(zitarev_set_eq1_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float eq1_freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->eq1_freq = eq1_freq);
}

static MFUN(zitarev_get_eq1_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->eq1_level;
}

static MFUN(zitarev_set_eq1_level) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float eq1_level = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->eq1_level = eq1_level);
}

static MFUN(zitarev_get_eq2_freq) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->eq2_freq;
}

static MFUN(zitarev_set_eq2_freq) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float eq2_freq = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->eq2_freq = eq2_freq);
}

static MFUN(zitarev_get_eq2_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->eq2_level;
}

static MFUN(zitarev_set_eq2_level) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float eq2_level = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->eq2_level = eq2_level);
}

static MFUN(zitarev_get_mix) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->mix;
}

static MFUN(zitarev_set_mix) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float mix = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->mix = mix);
}

static MFUN(zitarev_get_level) {
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  *(m_float*)RETURN = *ug->osc->level;
}

static MFUN(zitarev_set_level) {
  const m_uint gw_offset = SZ_INT;
  const GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
  m_float level = *(m_float*)(shred->mem + gw_offset);
  *(m_float*)RETURN = (*ug->osc->level = level);
}

static TICK(sp_tick) {
  ++((sp_data*)u->module.gen.data)->pos; }

GWION_IMPORT(soundpipe) {

  VM* vm = gwi_vm(gwi);
  const uint8_t nchan = vm->bbq->si->out;
  sp_createn(&sp, nchan);
  sp->sr = vm->bbq->si->sr;
  Type t_ftbl = gwi_mk_type(gwi, "ftbl", SZ_INT, "Object");
  CHECK_BB(gwi_class_ini(gwi, t_ftbl, NULL, ftbl_dtor))
  CHECK_BB(gwi_item_ini(gwi, "int", "@ftbl"))
  gwi_item_end(gwi, 0, NULL);
  gwi_func_ini(gwi, "void", "gen_composite", ftbl_gen_composite);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "string", "argstring");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_file", ftbl_gen_file);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_gauss", ftbl_gen_gauss);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "float", "scale");
     gwi_func_arg(gwi, "int", "seed");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_line", ftbl_gen_line);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "string", "argstring");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_padsynth", ftbl_gen_padsynth);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "ftbl", "amps");
     gwi_func_arg(gwi, "float", "f");
     gwi_func_arg(gwi, "float", "bw");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_rand", ftbl_gen_rand);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "string", "argstring");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_scrambler", ftbl_gen_scrambler);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "ftbl", "dest");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_sine", ftbl_gen_sine);
  gwi_func_arg(gwi, "int", "size");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_sinesum", ftbl_gen_sinesum);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "string", "argstring");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_triangle", ftbl_gen_triangle);
  gwi_func_arg(gwi, "int", "size");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "void", "gen_xline", ftbl_gen_xline);
  gwi_func_arg(gwi, "int", "size");
     gwi_func_arg(gwi, "string", "argstring");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_adsr = gwi_mk_type(gwi, "Adsr", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_adsr, adsr_ctor, adsr_dtor))
  gwi_func_ini(gwi, "float", "atk", adsr_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", adsr_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", adsr_get_dec);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", adsr_set_dec);
     gwi_func_arg(gwi, "float", "dec");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "sus", adsr_get_sus);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "sus", adsr_set_sus);
     gwi_func_arg(gwi, "float", "sus");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", adsr_get_rel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", adsr_set_rel);
     gwi_func_arg(gwi, "float", "rel");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_allpass = gwi_mk_type(gwi, "Allpass", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_allpass, allpass_ctor, allpass_dtor))
  gwi_func_ini(gwi, "void", "init", allpass_init);
     gwi_func_arg(gwi, "float", "looptime");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "revtime", allpass_get_revtime);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "revtime", allpass_set_revtime);
     gwi_func_arg(gwi, "float", "revtime");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_atone = gwi_mk_type(gwi, "Atone", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_atone, atone_ctor, atone_dtor))
  gwi_func_ini(gwi, "float", "hp", atone_get_hp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "hp", atone_set_hp);
     gwi_func_arg(gwi, "float", "hp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_autowah = gwi_mk_type(gwi, "Autowah", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_autowah, autowah_ctor, autowah_dtor))
  gwi_func_ini(gwi, "float", "level", autowah_get_level);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "level", autowah_set_level);
     gwi_func_arg(gwi, "float", "level");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "wah", autowah_get_wah);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "wah", autowah_set_wah);
     gwi_func_arg(gwi, "float", "wah");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mix", autowah_get_mix);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mix", autowah_set_mix);
     gwi_func_arg(gwi, "float", "mix");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_bal = gwi_mk_type(gwi, "Bal", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_bal, bal_ctor, bal_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_bar = gwi_mk_type(gwi, "Bar", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_bar, bar_ctor, bar_dtor))
  gwi_func_ini(gwi, "void", "init", bar_init);
     gwi_func_arg(gwi, "float", "iK");
     gwi_func_arg(gwi, "float", "ib");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bcL", bar_get_bcL);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bcL", bar_set_bcL);
     gwi_func_arg(gwi, "float", "bcL");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bcR", bar_get_bcR);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bcR", bar_set_bcR);
     gwi_func_arg(gwi, "float", "bcR");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "T30", bar_get_T30);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "T30", bar_set_T30);
     gwi_func_arg(gwi, "float", "T30");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "scan", bar_get_scan);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "scan", bar_set_scan);
     gwi_func_arg(gwi, "float", "scan");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pos", bar_get_pos);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pos", bar_set_pos);
     gwi_func_arg(gwi, "float", "pos");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "vel", bar_get_vel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "vel", bar_set_vel);
     gwi_func_arg(gwi, "float", "vel");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "wid", bar_get_wid);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "wid", bar_set_wid);
     gwi_func_arg(gwi, "float", "wid");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_biquad = gwi_mk_type(gwi, "Biquad", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_biquad, biquad_ctor, biquad_dtor))
  gwi_func_ini(gwi, "float", "b0", biquad_get_b0);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b0", biquad_set_b0);
     gwi_func_arg(gwi, "float", "b0");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b1", biquad_get_b1);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b1", biquad_set_b1);
     gwi_func_arg(gwi, "float", "b1");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b2", biquad_get_b2);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b2", biquad_set_b2);
     gwi_func_arg(gwi, "float", "b2");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "a0", biquad_get_a0);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "a0", biquad_set_a0);
     gwi_func_arg(gwi, "float", "a0");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "a1", biquad_get_a1);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "a1", biquad_set_a1);
     gwi_func_arg(gwi, "float", "a1");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "a2", biquad_get_a2);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "a2", biquad_set_a2);
     gwi_func_arg(gwi, "float", "a2");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_biscale = gwi_mk_type(gwi, "Biscale", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_biscale, biscale_ctor, biscale_dtor))
  gwi_func_ini(gwi, "float", "min", biscale_get_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", biscale_set_min);
     gwi_func_arg(gwi, "float", "min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", biscale_get_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", biscale_set_max);
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_bitcrush = gwi_mk_type(gwi, "Bitcrush", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_bitcrush, bitcrush_ctor, bitcrush_dtor))
  gwi_func_ini(gwi, "float", "bitdepth", bitcrush_get_bitdepth);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bitdepth", bitcrush_set_bitdepth);
     gwi_func_arg(gwi, "float", "bitdepth");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "srate", bitcrush_get_srate);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "srate", bitcrush_set_srate);
     gwi_func_arg(gwi, "float", "srate");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_blsaw = gwi_mk_type(gwi, "Blsaw", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_blsaw, blsaw_ctor, blsaw_dtor))
  gwi_func_ini(gwi, "float", "freq", blsaw_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", blsaw_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", blsaw_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", blsaw_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_blsquare = gwi_mk_type(gwi, "Blsquare", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_blsquare, blsquare_ctor, blsquare_dtor))
  gwi_func_ini(gwi, "float", "freq", blsquare_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", blsquare_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", blsquare_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", blsquare_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "width", blsquare_get_width);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "width", blsquare_set_width);
     gwi_func_arg(gwi, "float", "width");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_bltriangle = gwi_mk_type(gwi, "Bltriangle", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_bltriangle, bltriangle_ctor, bltriangle_dtor))
  gwi_func_ini(gwi, "float", "freq", bltriangle_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", bltriangle_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", bltriangle_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", bltriangle_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_brown = gwi_mk_type(gwi, "Brown", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_brown, brown_ctor, brown_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_butbp = gwi_mk_type(gwi, "Butbp", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_butbp, butbp_ctor, butbp_dtor))
  gwi_func_ini(gwi, "float", "freq", butbp_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", butbp_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bw", butbp_get_bw);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bw", butbp_set_bw);
     gwi_func_arg(gwi, "float", "bw");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_butbr = gwi_mk_type(gwi, "Butbr", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_butbr, butbr_ctor, butbr_dtor))
  gwi_func_ini(gwi, "float", "freq", butbr_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", butbr_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bw", butbr_get_bw);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bw", butbr_set_bw);
     gwi_func_arg(gwi, "float", "bw");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_buthp = gwi_mk_type(gwi, "Buthp", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_buthp, buthp_ctor, buthp_dtor))
  gwi_func_ini(gwi, "float", "freq", buthp_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", buthp_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_butlp = gwi_mk_type(gwi, "Butlp", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_butlp, butlp_ctor, butlp_dtor))
  gwi_func_ini(gwi, "float", "freq", butlp_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", butlp_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_clip = gwi_mk_type(gwi, "Clip", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_clip, clip_ctor, clip_dtor))
  gwi_func_ini(gwi, "float", "lim", clip_get_lim);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lim", clip_set_lim);
     gwi_func_arg(gwi, "float", "lim");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_clock = gwi_mk_type(gwi, "Clock", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_clock, clock_ctor, clock_dtor))
  gwi_func_ini(gwi, "float", "bpm", clock_get_bpm);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bpm", clock_set_bpm);
     gwi_func_arg(gwi, "float", "bpm");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "subdiv", clock_get_subdiv);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "subdiv", clock_set_subdiv);
     gwi_func_arg(gwi, "float", "subdiv");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_comb = gwi_mk_type(gwi, "Comb", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_comb, comb_ctor, comb_dtor))
  gwi_func_ini(gwi, "void", "init", comb_init);
     gwi_func_arg(gwi, "float", "looptime");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "revtime", comb_get_revtime);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "revtime", comb_set_revtime);
     gwi_func_arg(gwi, "float", "revtime");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_compressor = gwi_mk_type(gwi, "Compressor", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_compressor, compressor_ctor, compressor_dtor))
  gwi_func_ini(gwi, "float", "ratio", compressor_get_ratio);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "ratio", compressor_set_ratio);
     gwi_func_arg(gwi, "float", "ratio");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "thresh", compressor_get_thresh);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "thresh", compressor_set_thresh);
     gwi_func_arg(gwi, "float", "thresh");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", compressor_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", compressor_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", compressor_get_rel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", compressor_set_rel);
     gwi_func_arg(gwi, "float", "rel");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_conv = gwi_mk_type(gwi, "Conv", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_conv, conv_ctor, conv_dtor))
  gwi_func_ini(gwi, "void", "init", conv_init);
     gwi_func_arg(gwi, "ftbl", "ft");
     gwi_func_arg(gwi, "float", "iPartLen");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_count = gwi_mk_type(gwi, "Count", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_count, count_ctor, count_dtor))
  gwi_func_ini(gwi, "float", "count", count_get_count);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "count", count_set_count);
     gwi_func_arg(gwi, "float", "count");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mode", count_get_mode);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mode", count_set_mode);
     gwi_func_arg(gwi, "float", "mode");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_crossfade = gwi_mk_type(gwi, "Crossfade", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_crossfade, crossfade_ctor, crossfade_dtor))
  gwi_func_ini(gwi, "float", "pos", crossfade_get_pos);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pos", crossfade_set_pos);
     gwi_func_arg(gwi, "float", "pos");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_dcblock = gwi_mk_type(gwi, "Dcblock", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_dcblock, dcblock_ctor, dcblock_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_delay = gwi_mk_type(gwi, "Delay", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_delay, delay_ctor, delay_dtor))
  gwi_func_ini(gwi, "void", "init", delay_init);
     gwi_func_arg(gwi, "float", "time");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback", delay_get_feedback);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback", delay_set_feedback);
     gwi_func_arg(gwi, "float", "feedback");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_diode = gwi_mk_type(gwi, "Diode", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_diode, diode_ctor, diode_dtor))
  gwi_func_ini(gwi, "float", "freq", diode_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", diode_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", diode_get_res);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", diode_set_res);
     gwi_func_arg(gwi, "float", "res");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_diskin = gwi_mk_type(gwi, "Diskin", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_diskin, diskin_ctor, diskin_dtor))
  gwi_func_ini(gwi, "void", "init", diskin_init);
     gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_dist = gwi_mk_type(gwi, "Dist", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_dist, dist_ctor, dist_dtor))
  gwi_func_ini(gwi, "float", "pregain", dist_get_pregain);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pregain", dist_set_pregain);
     gwi_func_arg(gwi, "float", "pregain");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "postgain", dist_get_postgain);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "postgain", dist_set_postgain);
     gwi_func_arg(gwi, "float", "postgain");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "shape1", dist_get_shape1);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "shape1", dist_set_shape1);
     gwi_func_arg(gwi, "float", "shape1");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "shape2", dist_get_shape2);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "shape2", dist_set_shape2);
     gwi_func_arg(gwi, "float", "shape2");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_dmetro = gwi_mk_type(gwi, "Dmetro", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_dmetro, dmetro_ctor, dmetro_dtor))
  gwi_func_ini(gwi, "float", "time", dmetro_get_time);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "time", dmetro_set_time);
     gwi_func_arg(gwi, "float", "time");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_drip = gwi_mk_type(gwi, "Drip", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_drip, drip_ctor, drip_dtor))
  gwi_func_ini(gwi, "void", "init", drip_init);
     gwi_func_arg(gwi, "float", "dettack");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "num_tubes", drip_get_num_tubes);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "num_tubes", drip_set_num_tubes);
     gwi_func_arg(gwi, "float", "num_tubes");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", drip_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", drip_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "damp", drip_get_damp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "damp", drip_set_damp);
     gwi_func_arg(gwi, "float", "damp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "shake_max", drip_get_shake_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "shake_max", drip_set_shake_max);
     gwi_func_arg(gwi, "float", "shake_max");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", drip_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", drip_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq1", drip_get_freq1);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq1", drip_set_freq1);
     gwi_func_arg(gwi, "float", "freq1");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq2", drip_get_freq2);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq2", drip_set_freq2);
     gwi_func_arg(gwi, "float", "freq2");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_dtrig = gwi_mk_type(gwi, "Dtrig", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_dtrig, dtrig_ctor, dtrig_dtor))
  gwi_func_ini(gwi, "void", "init", dtrig_init);
     gwi_func_arg(gwi, "ftbl", "ft");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "loop", dtrig_get_loop);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "loop", dtrig_set_loop);
     gwi_func_arg(gwi, "int", "loop");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "delay", dtrig_get_delay);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "delay", dtrig_set_delay);
     gwi_func_arg(gwi, "float", "delay");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "scale", dtrig_get_scale);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "scale", dtrig_set_scale);
     gwi_func_arg(gwi, "float", "scale");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_dust = gwi_mk_type(gwi, "Dust", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_dust, dust_ctor, dust_dtor))
  gwi_func_ini(gwi, "float", "amp", dust_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", dust_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "density", dust_get_density);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "density", dust_set_density);
     gwi_func_arg(gwi, "float", "density");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "bipolar", dust_get_bipolar);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "bipolar", dust_set_bipolar);
     gwi_func_arg(gwi, "int", "bipolar");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_eqfil = gwi_mk_type(gwi, "Eqfil", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_eqfil, eqfil_ctor, eqfil_dtor))
  gwi_func_ini(gwi, "float", "freq", eqfil_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", eqfil_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bw", eqfil_get_bw);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bw", eqfil_set_bw);
     gwi_func_arg(gwi, "float", "bw");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "gain", eqfil_get_gain);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "gain", eqfil_set_gain);
     gwi_func_arg(gwi, "float", "gain");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_expon = gwi_mk_type(gwi, "Expon", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_expon, expon_ctor, expon_dtor))
  gwi_func_ini(gwi, "float", "a", expon_get_a);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "a", expon_set_a);
     gwi_func_arg(gwi, "float", "a");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", expon_get_dur);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", expon_set_dur);
     gwi_func_arg(gwi, "float", "dur");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b", expon_get_b);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b", expon_set_b);
     gwi_func_arg(gwi, "float", "b");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_fof = gwi_mk_type(gwi, "Fof", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_fof, fof_ctor, fof_dtor))
  gwi_func_ini(gwi, "void", "init", fof_init);
     gwi_func_arg(gwi, "ftbl", "sine");
     gwi_func_arg(gwi, "ftbl", "win");
     gwi_func_arg(gwi, "int", "iolaps");
     gwi_func_arg(gwi, "float", "iphs");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", fof_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", fof_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "fund", fof_get_fund);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "fund", fof_set_fund);
     gwi_func_arg(gwi, "float", "fund");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "form", fof_get_form);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "form", fof_set_form);
     gwi_func_arg(gwi, "float", "form");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "oct", fof_get_oct);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "oct", fof_set_oct);
     gwi_func_arg(gwi, "float", "oct");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "band", fof_get_band);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "band", fof_set_band);
     gwi_func_arg(gwi, "float", "band");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "ris", fof_get_ris);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "ris", fof_set_ris);
     gwi_func_arg(gwi, "float", "ris");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", fof_get_dec);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", fof_set_dec);
     gwi_func_arg(gwi, "float", "dec");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", fof_get_dur);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", fof_set_dur);
     gwi_func_arg(gwi, "float", "dur");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_fofilt = gwi_mk_type(gwi, "Fofilt", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_fofilt, fofilt_ctor, fofilt_dtor))
  gwi_func_ini(gwi, "float", "freq", fofilt_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", fofilt_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", fofilt_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", fofilt_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", fofilt_get_dec);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", fofilt_set_dec);
     gwi_func_arg(gwi, "float", "dec");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_fog = gwi_mk_type(gwi, "Fog", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_fog, fog_ctor, fog_dtor))
  gwi_func_ini(gwi, "void", "init", fog_init);
     gwi_func_arg(gwi, "ftbl", "wav");
     gwi_func_arg(gwi, "ftbl", "win");
     gwi_func_arg(gwi, "int", "iolaps");
     gwi_func_arg(gwi, "float", "iphs");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", fog_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", fog_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dens", fog_get_dens);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dens", fog_set_dens);
     gwi_func_arg(gwi, "float", "dens");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "trans", fog_get_trans);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "trans", fog_set_trans);
     gwi_func_arg(gwi, "float", "trans");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "spd", fog_get_spd);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "spd", fog_set_spd);
     gwi_func_arg(gwi, "float", "spd");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "oct", fog_get_oct);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "oct", fog_set_oct);
     gwi_func_arg(gwi, "float", "oct");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "band", fog_get_band);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "band", fog_set_band);
     gwi_func_arg(gwi, "float", "band");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "ris", fog_get_ris);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "ris", fog_set_ris);
     gwi_func_arg(gwi, "float", "ris");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", fog_get_dec);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", fog_set_dec);
     gwi_func_arg(gwi, "float", "dec");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", fog_get_dur);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", fog_set_dur);
     gwi_func_arg(gwi, "float", "dur");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_fold = gwi_mk_type(gwi, "Fold", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_fold, fold_ctor, fold_dtor))
  gwi_func_ini(gwi, "float", "incr", fold_get_incr);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "incr", fold_set_incr);
     gwi_func_arg(gwi, "float", "incr");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_fosc = gwi_mk_type(gwi, "Fosc", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_fosc, fosc_ctor, fosc_dtor))
  gwi_func_ini(gwi, "void", "init", fosc_init);
     gwi_func_arg(gwi, "ftbl", "tbl");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", fosc_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", fosc_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", fosc_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", fosc_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "car", fosc_get_car);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "car", fosc_set_car);
     gwi_func_arg(gwi, "float", "car");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mod", fosc_get_mod);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mod", fosc_set_mod);
     gwi_func_arg(gwi, "float", "mod");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "indx", fosc_get_indx);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "indx", fosc_set_indx);
     gwi_func_arg(gwi, "float", "indx");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_gbuzz = gwi_mk_type(gwi, "Gbuzz", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_gbuzz, gbuzz_ctor, gbuzz_dtor))
  gwi_func_ini(gwi, "void", "init", gbuzz_init);
     gwi_func_arg(gwi, "ftbl", "ft");
     gwi_func_arg(gwi, "float", "iphs");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", gbuzz_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", gbuzz_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", gbuzz_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", gbuzz_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "nharm", gbuzz_get_nharm);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "nharm", gbuzz_set_nharm);
     gwi_func_arg(gwi, "float", "nharm");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lharm", gbuzz_get_lharm);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lharm", gbuzz_set_lharm);
     gwi_func_arg(gwi, "float", "lharm");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mul", gbuzz_get_mul);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mul", gbuzz_set_mul);
     gwi_func_arg(gwi, "float", "mul");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_hilbert = gwi_mk_type(gwi, "Hilbert", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_hilbert, hilbert_ctor, hilbert_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_in = gwi_mk_type(gwi, "In", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_in, in_ctor, in_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_incr = gwi_mk_type(gwi, "Incr", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_incr, incr_ctor, incr_dtor))
  gwi_func_ini(gwi, "void", "init", incr_init);
     gwi_func_arg(gwi, "float", "val");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "step", incr_get_step);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "step", incr_set_step);
     gwi_func_arg(gwi, "float", "step");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", incr_get_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", incr_set_min);
     gwi_func_arg(gwi, "float", "min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", incr_get_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", incr_set_max);
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_jcrev = gwi_mk_type(gwi, "Jcrev", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_jcrev, jcrev_ctor, jcrev_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_jitter = gwi_mk_type(gwi, "Jitter", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_jitter, jitter_ctor, jitter_dtor))
  gwi_func_ini(gwi, "float", "amp", jitter_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", jitter_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cpsMin", jitter_get_cpsMin);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cpsMin", jitter_set_cpsMin);
     gwi_func_arg(gwi, "float", "cpsMin");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cpsMax", jitter_get_cpsMax);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cpsMax", jitter_set_cpsMax);
     gwi_func_arg(gwi, "float", "cpsMax");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_line = gwi_mk_type(gwi, "Line", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_line, line_ctor, line_dtor))
  gwi_func_ini(gwi, "float", "a", line_get_a);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "a", line_set_a);
     gwi_func_arg(gwi, "float", "a");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", line_get_dur);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", line_set_dur);
     gwi_func_arg(gwi, "float", "dur");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b", line_get_b);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "b", line_set_b);
     gwi_func_arg(gwi, "float", "b");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_lpc = gwi_mk_type(gwi, "Lpc", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_lpc, lpc_ctor, lpc_dtor))
  gwi_func_ini(gwi, "void", "init", lpc_init);
     gwi_func_arg(gwi, "int", "framesize");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_lpf18 = gwi_mk_type(gwi, "Lpf18", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_lpf18, lpf18_ctor, lpf18_dtor))
  gwi_func_ini(gwi, "float", "cutoff", lpf18_get_cutoff);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cutoff", lpf18_set_cutoff);
     gwi_func_arg(gwi, "float", "cutoff");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", lpf18_get_res);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", lpf18_set_res);
     gwi_func_arg(gwi, "float", "res");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dist", lpf18_get_dist);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dist", lpf18_set_dist);
     gwi_func_arg(gwi, "float", "dist");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_maygate = gwi_mk_type(gwi, "Maygate", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_maygate, maygate_ctor, maygate_dtor))
  gwi_func_ini(gwi, "float", "prob", maygate_get_prob);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "prob", maygate_set_prob);
     gwi_func_arg(gwi, "float", "prob");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "mode", maygate_get_mode);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "mode", maygate_set_mode);
     gwi_func_arg(gwi, "int", "mode");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_metro = gwi_mk_type(gwi, "Metro", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_metro, metro_ctor, metro_dtor))
  gwi_func_ini(gwi, "float", "freq", metro_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", metro_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_mincer = gwi_mk_type(gwi, "Mincer", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_mincer, mincer_ctor, mincer_dtor))
  gwi_func_ini(gwi, "void", "init", mincer_init);
     gwi_func_arg(gwi, "ftbl", "ft");
     gwi_func_arg(gwi, "int", "winsize");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "time", mincer_get_time);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "time", mincer_set_time);
     gwi_func_arg(gwi, "float", "time");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", mincer_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", mincer_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pitch", mincer_get_pitch);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pitch", mincer_set_pitch);
     gwi_func_arg(gwi, "float", "pitch");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_mode = gwi_mk_type(gwi, "Mode", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_mode, mode_ctor, mode_dtor))
  gwi_func_ini(gwi, "float", "freq", mode_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", mode_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "q", mode_get_q);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "q", mode_set_q);
     gwi_func_arg(gwi, "float", "q");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_moogladder = gwi_mk_type(gwi, "Moogladder", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_moogladder, moogladder_ctor, moogladder_dtor))
  gwi_func_ini(gwi, "float", "freq", moogladder_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", moogladder_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", moogladder_get_res);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", moogladder_set_res);
     gwi_func_arg(gwi, "float", "res");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_noise = gwi_mk_type(gwi, "Noise", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_noise, noise_ctor, noise_dtor))
  gwi_func_ini(gwi, "float", "amp", noise_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", noise_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_nsmp = gwi_mk_type(gwi, "Nsmp", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_nsmp, nsmp_ctor, nsmp_dtor))
  gwi_func_ini(gwi, "void", "init", nsmp_init);
     gwi_func_arg(gwi, "ftbl", "ft");
     gwi_func_arg(gwi, "int", "sr");
     gwi_func_arg(gwi, "string", "init");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "index", nsmp_get_index);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "index", nsmp_set_index);
     gwi_func_arg(gwi, "int", "index");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_osc = gwi_mk_type(gwi, "Osc", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_osc, osc_ctor, osc_dtor))
  gwi_func_ini(gwi, "void", "init", osc_init);
     gwi_func_arg(gwi, "ftbl", "tbl");
     gwi_func_arg(gwi, "float", "phase");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", osc_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", osc_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", osc_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", osc_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_oscmorph = gwi_mk_type(gwi, "Oscmorph", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_oscmorph, oscmorph_ctor, oscmorph_dtor))
  gwi_func_ini(gwi, "void", "init", oscmorph_init);
     gwi_func_arg(gwi, "ftbl[]", "tbl");
     gwi_func_arg(gwi, "int", "nft");
     gwi_func_arg(gwi, "float", "phase");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", oscmorph_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", oscmorph_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", oscmorph_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", oscmorph_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "wtpos", oscmorph_get_wtpos);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "wtpos", oscmorph_set_wtpos);
     gwi_func_arg(gwi, "float", "wtpos");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_pan2 = gwi_mk_type(gwi, "Pan2", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_pan2, pan2_ctor, pan2_dtor))
  gwi_func_ini(gwi, "int", "type", pan2_get_type);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "type", pan2_set_type);
     gwi_func_arg(gwi, "int", "type");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pan", pan2_get_pan);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pan", pan2_set_pan);
     gwi_func_arg(gwi, "float", "pan");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_panst = gwi_mk_type(gwi, "Panst", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_panst, panst_ctor, panst_dtor))
  gwi_func_ini(gwi, "int", "type", panst_get_type);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "type", panst_set_type);
     gwi_func_arg(gwi, "int", "type");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pan", panst_get_pan);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "pan", panst_set_pan);
     gwi_func_arg(gwi, "float", "pan");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_pareq = gwi_mk_type(gwi, "Pareq", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_pareq, pareq_ctor, pareq_dtor))
  gwi_func_ini(gwi, "float", "fc", pareq_get_fc);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "fc", pareq_set_fc);
     gwi_func_arg(gwi, "float", "fc");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "v", pareq_get_v);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "v", pareq_set_v);
     gwi_func_arg(gwi, "float", "v");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "q", pareq_get_q);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "q", pareq_set_q);
     gwi_func_arg(gwi, "float", "q");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mode", pareq_get_mode);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mode", pareq_set_mode);
     gwi_func_arg(gwi, "float", "mode");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_paulstretch = gwi_mk_type(gwi, "Paulstretch", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_paulstretch, paulstretch_ctor, paulstretch_dtor))
  gwi_func_ini(gwi, "void", "init", paulstretch_init);
     gwi_func_arg(gwi, "ftbl", "ft");
     gwi_func_arg(gwi, "float", "windowsize");
     gwi_func_arg(gwi, "float", "stretch");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_pdhalf = gwi_mk_type(gwi, "Pdhalf", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_pdhalf, pdhalf_ctor, pdhalf_dtor))
  gwi_func_ini(gwi, "float", "amount", pdhalf_get_amount);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amount", pdhalf_set_amount);
     gwi_func_arg(gwi, "float", "amount");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_peaklim = gwi_mk_type(gwi, "Peaklim", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_peaklim, peaklim_ctor, peaklim_dtor))
  gwi_func_ini(gwi, "float", "atk", peaklim_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", peaklim_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", peaklim_get_rel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", peaklim_set_rel);
     gwi_func_arg(gwi, "float", "rel");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "thresh", peaklim_get_thresh);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "thresh", peaklim_set_thresh);
     gwi_func_arg(gwi, "float", "thresh");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_phaser = gwi_mk_type(gwi, "Phaser", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_phaser, phaser_ctor, phaser_dtor))
  gwi_func_ini(gwi, "float", "MaxNotch1Freq", phaser_get_MaxNotch1Freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "MaxNotch1Freq", phaser_set_MaxNotch1Freq);
     gwi_func_arg(gwi, "float", "MaxNotch1Freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "MinNotch1Freq", phaser_get_MinNotch1Freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "MinNotch1Freq", phaser_set_MinNotch1Freq);
     gwi_func_arg(gwi, "float", "MinNotch1Freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "Notch_width", phaser_get_Notch_width);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "Notch_width", phaser_set_Notch_width);
     gwi_func_arg(gwi, "float", "Notch_width");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "NotchFreq", phaser_get_NotchFreq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "NotchFreq", phaser_set_NotchFreq);
     gwi_func_arg(gwi, "float", "NotchFreq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "VibratoMode", phaser_get_VibratoMode);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "VibratoMode", phaser_set_VibratoMode);
     gwi_func_arg(gwi, "float", "VibratoMode");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "depth", phaser_get_depth);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "depth", phaser_set_depth);
     gwi_func_arg(gwi, "float", "depth");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback_gain", phaser_get_feedback_gain);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback_gain", phaser_set_feedback_gain);
     gwi_func_arg(gwi, "float", "feedback_gain");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "invert", phaser_get_invert);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "invert", phaser_set_invert);
     gwi_func_arg(gwi, "float", "invert");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "level", phaser_get_level);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "level", phaser_set_level);
     gwi_func_arg(gwi, "float", "level");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lfobpm", phaser_get_lfobpm);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lfobpm", phaser_set_lfobpm);
     gwi_func_arg(gwi, "float", "lfobpm");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_phasor = gwi_mk_type(gwi, "Phasor", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_phasor, phasor_ctor, phasor_dtor))
  gwi_func_ini(gwi, "void", "init", phasor_init);
     gwi_func_arg(gwi, "float", "iphs");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", phasor_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", phasor_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_pinknoise = gwi_mk_type(gwi, "Pinknoise", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_pinknoise, pinknoise_ctor, pinknoise_dtor))
  gwi_func_ini(gwi, "float", "amp", pinknoise_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", pinknoise_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_pitchamdf = gwi_mk_type(gwi, "Pitchamdf", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_pitchamdf, pitchamdf_ctor, pitchamdf_dtor))
  gwi_func_ini(gwi, "void", "init", pitchamdf_init);
     gwi_func_arg(gwi, "float", "min");
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_pluck = gwi_mk_type(gwi, "Pluck", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_pluck, pluck_ctor, pluck_dtor))
  gwi_func_ini(gwi, "void", "init", pluck_init);
     gwi_func_arg(gwi, "float", "ifreq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", pluck_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", pluck_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", pluck_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", pluck_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_port = gwi_mk_type(gwi, "Port", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_port, port_ctor, port_dtor))
  gwi_func_ini(gwi, "void", "init", port_init);
     gwi_func_arg(gwi, "float", "htime");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "htime", port_get_htime);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "htime", port_set_htime);
     gwi_func_arg(gwi, "float", "htime");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_posc3 = gwi_mk_type(gwi, "Posc3", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_posc3, posc3_ctor, posc3_dtor))
  gwi_func_ini(gwi, "void", "init", posc3_init);
     gwi_func_arg(gwi, "ftbl", "tbl");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", posc3_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", posc3_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", posc3_get_amp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "amp", posc3_set_amp);
     gwi_func_arg(gwi, "float", "amp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_progress = gwi_mk_type(gwi, "Progress", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_progress, progress_ctor, progress_dtor))
  gwi_func_ini(gwi, "int", "nbars", progress_get_nbars);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "nbars", progress_set_nbars);
     gwi_func_arg(gwi, "int", "nbars");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "skip", progress_get_skip);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "skip", progress_set_skip);
     gwi_func_arg(gwi, "int", "skip");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_prop = gwi_mk_type(gwi, "Prop", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_prop, prop_ctor, prop_dtor))
  gwi_func_ini(gwi, "void", "init", prop_init);
     gwi_func_arg(gwi, "string", "str");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bpm", prop_get_bpm);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bpm", prop_set_bpm);
     gwi_func_arg(gwi, "float", "bpm");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_pshift = gwi_mk_type(gwi, "Pshift", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_pshift, pshift_ctor, pshift_dtor))
  gwi_func_ini(gwi, "float", "shift", pshift_get_shift);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "shift", pshift_set_shift);
     gwi_func_arg(gwi, "float", "shift");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "window", pshift_get_window);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "window", pshift_set_window);
     gwi_func_arg(gwi, "float", "window");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "xfade", pshift_get_xfade);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "xfade", pshift_set_xfade);
     gwi_func_arg(gwi, "float", "xfade");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_ptrack = gwi_mk_type(gwi, "Ptrack", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_ptrack, ptrack_ctor, ptrack_dtor))
  gwi_func_ini(gwi, "void", "init", ptrack_init);
     gwi_func_arg(gwi, "int", "ihopsize");
     gwi_func_arg(gwi, "int", "ipeaks");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_randh = gwi_mk_type(gwi, "Randh", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_randh, randh_ctor, randh_dtor))
  gwi_func_ini(gwi, "float", "min", randh_get_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", randh_set_min);
     gwi_func_arg(gwi, "float", "min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", randh_get_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", randh_set_max);
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", randh_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", randh_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_randi = gwi_mk_type(gwi, "Randi", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_randi, randi_ctor, randi_dtor))
  gwi_func_ini(gwi, "float", "min", randi_get_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", randi_set_min);
     gwi_func_arg(gwi, "float", "min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", randi_get_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", randi_set_max);
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cps", randi_get_cps);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cps", randi_set_cps);
     gwi_func_arg(gwi, "float", "cps");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mode", randi_get_mode);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mode", randi_set_mode);
     gwi_func_arg(gwi, "float", "mode");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_random = gwi_mk_type(gwi, "Random", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_random, random_ctor, random_dtor))
  gwi_func_ini(gwi, "float", "min", random_get_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", random_set_min);
     gwi_func_arg(gwi, "float", "min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", random_get_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", random_set_max);
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_reson = gwi_mk_type(gwi, "Reson", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_reson, reson_ctor, reson_dtor))
  gwi_func_ini(gwi, "float", "freq", reson_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", reson_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bw", reson_get_bw);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bw", reson_set_bw);
     gwi_func_arg(gwi, "float", "bw");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_reverse = gwi_mk_type(gwi, "Reverse", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_reverse, reverse_ctor, reverse_dtor))
  gwi_func_ini(gwi, "void", "init", reverse_init);
     gwi_func_arg(gwi, "float", "delay");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_revsc = gwi_mk_type(gwi, "Revsc", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_revsc, revsc_ctor, revsc_dtor))
  gwi_func_ini(gwi, "float", "feedback", revsc_get_feedback);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback", revsc_set_feedback);
     gwi_func_arg(gwi, "float", "feedback");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lpfreq", revsc_get_lpfreq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lpfreq", revsc_set_lpfreq);
     gwi_func_arg(gwi, "float", "lpfreq");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_rms = gwi_mk_type(gwi, "Rms", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_rms, rms_ctor, rms_dtor))
  gwi_func_ini(gwi, "float", "ihp", rms_get_ihp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "ihp", rms_set_ihp);
     gwi_func_arg(gwi, "float", "ihp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_rpt = gwi_mk_type(gwi, "Rpt", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_rpt, rpt_ctor, rpt_dtor))
  gwi_func_ini(gwi, "void", "init", rpt_init);
     gwi_func_arg(gwi, "float", "maxdur");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_rspline = gwi_mk_type(gwi, "Rspline", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_rspline, rspline_ctor, rspline_dtor))
  gwi_func_ini(gwi, "float", "min", rspline_get_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", rspline_set_min);
     gwi_func_arg(gwi, "float", "min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", rspline_get_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", rspline_set_max);
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cps_min", rspline_get_cps_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cps_min", rspline_set_cps_min);
     gwi_func_arg(gwi, "float", "cps_min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cps_max", rspline_get_cps_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cps_max", rspline_set_cps_max);
     gwi_func_arg(gwi, "float", "cps_max");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_samphold = gwi_mk_type(gwi, "Samphold", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_samphold, samphold_ctor, samphold_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_saturator = gwi_mk_type(gwi, "Saturator", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_saturator, saturator_ctor, saturator_dtor))
  gwi_func_ini(gwi, "float", "drive", saturator_get_drive);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "drive", saturator_set_drive);
     gwi_func_arg(gwi, "float", "drive");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dcoffset", saturator_get_dcoffset);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dcoffset", saturator_set_dcoffset);
     gwi_func_arg(gwi, "float", "dcoffset");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_scale = gwi_mk_type(gwi, "Scale", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_scale, scale_ctor, scale_dtor))
  gwi_func_ini(gwi, "float", "min", scale_get_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", scale_set_min);
     gwi_func_arg(gwi, "float", "min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", scale_get_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", scale_set_max);
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_sdelay = gwi_mk_type(gwi, "Sdelay", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_sdelay, sdelay_ctor, sdelay_dtor))
  gwi_func_ini(gwi, "void", "init", sdelay_init);
     gwi_func_arg(gwi, "float", "size");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_slice = gwi_mk_type(gwi, "Slice", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_slice, slice_ctor, slice_dtor))
  gwi_func_ini(gwi, "void", "init", slice_init);
     gwi_func_arg(gwi, "ftbl", "vals");
     gwi_func_arg(gwi, "ftbl", "buf");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "id", slice_get_id);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "id", slice_set_id);
     gwi_func_arg(gwi, "float", "id");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_smoothdelay = gwi_mk_type(gwi, "Smoothdelay", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_smoothdelay, smoothdelay_ctor, smoothdelay_dtor))
  gwi_func_ini(gwi, "void", "init", smoothdelay_init);
     gwi_func_arg(gwi, "float", "maxdel");
     gwi_func_arg(gwi, "int", "interp");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback", smoothdelay_get_feedback);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback", smoothdelay_set_feedback);
     gwi_func_arg(gwi, "float", "feedback");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "del", smoothdelay_get_del);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "del", smoothdelay_set_del);
     gwi_func_arg(gwi, "float", "del");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_spa = gwi_mk_type(gwi, "Spa", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_spa, spa_ctor, spa_dtor))
  gwi_func_ini(gwi, "void", "init", spa_init);
     gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_sparec = gwi_mk_type(gwi, "Sparec", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_sparec, sparec_ctor, sparec_dtor))
  gwi_func_ini(gwi, "void", "init", sparec_init);
     gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_streson = gwi_mk_type(gwi, "Streson", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_streson, streson_ctor, streson_dtor))
  gwi_func_ini(gwi, "float", "freq", streson_get_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "freq", streson_set_freq);
     gwi_func_arg(gwi, "float", "freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "fdbgain", streson_get_fdbgain);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "fdbgain", streson_set_fdbgain);
     gwi_func_arg(gwi, "float", "fdbgain");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_switch = gwi_mk_type(gwi, "Switch", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_switch, switch_ctor, switch_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tabread = gwi_mk_type(gwi, "Tabread", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tabread, tabread_ctor, tabread_dtor))
  gwi_func_ini(gwi, "void", "init", tabread_init);
     gwi_func_arg(gwi, "ftbl", "ft");
     gwi_func_arg(gwi, "float", "mode");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "index", tabread_get_index);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "index", tabread_set_index);
     gwi_func_arg(gwi, "float", "index");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "offset", tabread_get_offset);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "offset", tabread_set_offset);
     gwi_func_arg(gwi, "float", "offset");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "wrap", tabread_get_wrap);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "wrap", tabread_set_wrap);
     gwi_func_arg(gwi, "float", "wrap");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tadsr = gwi_mk_type(gwi, "Tadsr", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tadsr, tadsr_ctor, tadsr_dtor))
  gwi_func_ini(gwi, "float", "atk", tadsr_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", tadsr_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", tadsr_get_dec);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dec", tadsr_set_dec);
     gwi_func_arg(gwi, "float", "dec");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "sus", tadsr_get_sus);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "sus", tadsr_set_sus);
     gwi_func_arg(gwi, "float", "sus");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", tadsr_get_rel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", tadsr_set_rel);
     gwi_func_arg(gwi, "float", "rel");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_talkbox = gwi_mk_type(gwi, "Talkbox", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_talkbox, talkbox_ctor, talkbox_dtor))
  gwi_func_ini(gwi, "float", "quality", talkbox_get_quality);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "quality", talkbox_set_quality);
     gwi_func_arg(gwi, "float", "quality");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tblrec = gwi_mk_type(gwi, "Tblrec", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tblrec, tblrec_ctor, tblrec_dtor))
  gwi_func_ini(gwi, "void", "init", tblrec_init);
     gwi_func_arg(gwi, "ftbl", "bar");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tbvcf = gwi_mk_type(gwi, "Tbvcf", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tbvcf, tbvcf_ctor, tbvcf_dtor))
  gwi_func_ini(gwi, "float", "fco", tbvcf_get_fco);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "fco", tbvcf_set_fco);
     gwi_func_arg(gwi, "float", "fco");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", tbvcf_get_res);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", tbvcf_set_res);
     gwi_func_arg(gwi, "float", "res");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dist", tbvcf_get_dist);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dist", tbvcf_set_dist);
     gwi_func_arg(gwi, "float", "dist");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "asym", tbvcf_get_asym);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "asym", tbvcf_set_asym);
     gwi_func_arg(gwi, "float", "asym");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tdiv = gwi_mk_type(gwi, "Tdiv", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tdiv, tdiv_ctor, tdiv_dtor))
  gwi_func_ini(gwi, "float", "num", tdiv_get_num);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "num", tdiv_set_num);
     gwi_func_arg(gwi, "float", "num");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "offset", tdiv_get_offset);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "offset", tdiv_set_offset);
     gwi_func_arg(gwi, "float", "offset");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tenv = gwi_mk_type(gwi, "Tenv", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tenv, tenv_ctor, tenv_dtor))
  gwi_func_ini(gwi, "float", "atk", tenv_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", tenv_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "hold", tenv_get_hold);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "hold", tenv_set_hold);
     gwi_func_arg(gwi, "float", "hold");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", tenv_get_rel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", tenv_set_rel);
     gwi_func_arg(gwi, "float", "rel");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tenv2 = gwi_mk_type(gwi, "Tenv2", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tenv2, tenv2_ctor, tenv2_dtor))
  gwi_func_ini(gwi, "float", "atk", tenv2_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", tenv2_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", tenv2_get_rel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", tenv2_set_rel);
     gwi_func_arg(gwi, "float", "rel");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tenvx = gwi_mk_type(gwi, "Tenvx", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tenvx, tenvx_ctor, tenvx_dtor))
  gwi_func_ini(gwi, "float", "atk", tenvx_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", tenvx_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "hold", tenvx_get_hold);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "hold", tenvx_set_hold);
     gwi_func_arg(gwi, "float", "hold");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", tenvx_get_rel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", tenvx_set_rel);
     gwi_func_arg(gwi, "float", "rel");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tgate = gwi_mk_type(gwi, "Tgate", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tgate, tgate_ctor, tgate_dtor))
  gwi_func_ini(gwi, "float", "time", tgate_get_time);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "time", tgate_set_time);
     gwi_func_arg(gwi, "float", "time");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_thresh = gwi_mk_type(gwi, "Thresh", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_thresh, thresh_ctor, thresh_dtor))
  gwi_func_ini(gwi, "float", "thresh", thresh_get_thresh);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "thresh", thresh_set_thresh);
     gwi_func_arg(gwi, "float", "thresh");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "mode", thresh_get_mode);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "mode", thresh_set_mode);
     gwi_func_arg(gwi, "int", "mode");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_timer = gwi_mk_type(gwi, "Timer", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_timer, timer_ctor, timer_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tin = gwi_mk_type(gwi, "Tin", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tin, tin_ctor, tin_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tone = gwi_mk_type(gwi, "Tone", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tone, tone_ctor, tone_dtor))
  gwi_func_ini(gwi, "float", "hp", tone_get_hp);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "hp", tone_set_hp);
     gwi_func_arg(gwi, "float", "hp");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_trand = gwi_mk_type(gwi, "Trand", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_trand, trand_ctor, trand_dtor))
  gwi_func_ini(gwi, "float", "min", trand_get_min);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "min", trand_set_min);
     gwi_func_arg(gwi, "float", "min");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", trand_get_max);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "max", trand_set_max);
     gwi_func_arg(gwi, "float", "max");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tseg = gwi_mk_type(gwi, "Tseg", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tseg, tseg_ctor, tseg_dtor))
  gwi_func_ini(gwi, "void", "init", tseg_init);
     gwi_func_arg(gwi, "float", "ibeg");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "end", tseg_get_end);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "end", tseg_set_end);
     gwi_func_arg(gwi, "float", "end");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", tseg_get_dur);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "dur", tseg_set_dur);
     gwi_func_arg(gwi, "float", "dur");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "type", tseg_get_type);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "type", tseg_set_type);
     gwi_func_arg(gwi, "float", "type");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_tseq = gwi_mk_type(gwi, "Tseq", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_tseq, tseq_ctor, tseq_dtor))
  gwi_func_ini(gwi, "void", "init", tseq_init);
     gwi_func_arg(gwi, "ftbl", "ft");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "shuf", tseq_get_shuf);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "shuf", tseq_set_shuf);
     gwi_func_arg(gwi, "int", "shuf");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_vdelay = gwi_mk_type(gwi, "Vdelay", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_vdelay, vdelay_ctor, vdelay_dtor))
  gwi_func_ini(gwi, "void", "init", vdelay_init);
     gwi_func_arg(gwi, "float", "maxdel");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "del", vdelay_get_del);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "del", vdelay_set_del);
     gwi_func_arg(gwi, "float", "del");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback", vdelay_get_feedback);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "feedback", vdelay_set_feedback);
     gwi_func_arg(gwi, "float", "feedback");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_voc = gwi_mk_type(gwi, "Voc", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_voc, voc_ctor, voc_dtor))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_vocoder = gwi_mk_type(gwi, "Vocoder", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_vocoder, vocoder_ctor, vocoder_dtor))
  gwi_func_ini(gwi, "float", "atk", vocoder_get_atk);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "atk", vocoder_set_atk);
     gwi_func_arg(gwi, "float", "atk");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", vocoder_get_rel);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rel", vocoder_set_rel);
     gwi_func_arg(gwi, "float", "rel");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bwratio", vocoder_get_bwratio);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "bwratio", vocoder_set_bwratio);
     gwi_func_arg(gwi, "float", "bwratio");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_waveset = gwi_mk_type(gwi, "Waveset", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_waveset, waveset_ctor, waveset_dtor))
  gwi_func_ini(gwi, "void", "init", waveset_init);
     gwi_func_arg(gwi, "float", "ilen");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rep", waveset_get_rep);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rep", waveset_set_rep);
     gwi_func_arg(gwi, "float", "rep");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_wavin = gwi_mk_type(gwi, "Wavin", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_wavin, wavin_ctor, wavin_dtor))
  gwi_func_ini(gwi, "void", "init", wavin_init);
     gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_wavout = gwi_mk_type(gwi, "Wavout", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_wavout, wavout_ctor, wavout_dtor))
  gwi_func_ini(gwi, "void", "init", wavout_init);
     gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_wpkorg35 = gwi_mk_type(gwi, "Wpkorg35", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_wpkorg35, wpkorg35_ctor, wpkorg35_dtor))
  gwi_func_ini(gwi, "float", "cutoff", wpkorg35_get_cutoff);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "cutoff", wpkorg35_set_cutoff);
     gwi_func_arg(gwi, "float", "cutoff");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", wpkorg35_get_res);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "res", wpkorg35_set_res);
     gwi_func_arg(gwi, "float", "res");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "saturation", wpkorg35_get_saturation);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "saturation", wpkorg35_set_saturation);
     gwi_func_arg(gwi, "float", "saturation");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  const Type t_zitarev = gwi_mk_type(gwi, "Zitarev", SZ_INT, "UGen");
  CHECK_BB(gwi_class_ini(gwi, t_zitarev, zitarev_ctor, zitarev_dtor))
  gwi_func_ini(gwi, "float", "in_delay", zitarev_get_in_delay);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "in_delay", zitarev_set_in_delay);
     gwi_func_arg(gwi, "float", "in_delay");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lf_x", zitarev_get_lf_x);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "lf_x", zitarev_set_lf_x);
     gwi_func_arg(gwi, "float", "lf_x");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rt60_low", zitarev_get_rt60_low);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rt60_low", zitarev_set_rt60_low);
     gwi_func_arg(gwi, "float", "rt60_low");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rt60_mid", zitarev_get_rt60_mid);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "rt60_mid", zitarev_set_rt60_mid);
     gwi_func_arg(gwi, "float", "rt60_mid");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "hf_damping", zitarev_get_hf_damping);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "hf_damping", zitarev_set_hf_damping);
     gwi_func_arg(gwi, "float", "hf_damping");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "eq1_freq", zitarev_get_eq1_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "eq1_freq", zitarev_set_eq1_freq);
     gwi_func_arg(gwi, "float", "eq1_freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "eq1_level", zitarev_get_eq1_level);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "eq1_level", zitarev_set_eq1_level);
     gwi_func_arg(gwi, "float", "eq1_level");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "eq2_freq", zitarev_get_eq2_freq);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "eq2_freq", zitarev_set_eq2_freq);
     gwi_func_arg(gwi, "float", "eq2_freq");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "eq2_level", zitarev_get_eq2_level);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "eq2_level", zitarev_set_eq2_level);
     gwi_func_arg(gwi, "float", "eq2_level");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mix", zitarev_get_mix);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "mix", zitarev_set_mix);
     gwi_func_arg(gwi, "float", "mix");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "level", zitarev_get_level);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "level", zitarev_set_level);
     gwi_func_arg(gwi, "float", "level");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  return 1;
}
