#include <stdlib.h>
#include <tinysr.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"

static m_int o_tinysr_ev;
static m_int o_tinysr_idx;
static m_int o_tinysr_score;

#define TINY_SR(o) *(M_Object*)((o)->data + o_tinysr_sr)
#define TINY_EV(o) *(M_Object*)((o)->data + o_tinysr_ev)
#define TINY_IDX(o) *(m_int*)((o)->data + o_tinysr_idx)
#define TINY_SCORE(o) *(m_float*)((o)->data + o_tinysr_score)

#define READ_SAMPS 128

struct sr_data {
  M_Object obj;
  tinysr_ctx_t* ctx;
  m_uint idx;
  samp_t smp[READ_SAMPS];
  m_bool state;
  m_bool ini;
};

static struct sr_data* new_sr_data(M_Object obj, const m_uint rate) {
  struct sr_data* sr = malloc(sizeof(struct sr_data));
  sr->obj = obj;
  sr->ctx = tinysr_allocate_context();
  sr->ctx->input_sample_rate = rate;
  sr->ctx->utterance_mode = TINYSR_MODE_FREE_RUNNING;
  sr->idx = 0;
  sr->state = 0;
  sr->ini = 0;
}

static void free_sr_data(struct sr_data* sr) {
  tinysr_free_context(sr->ctx);
  free(sr);
}

static  const m_float range = (m_float)INT16_MAX - (m_float)INT16_MIN;

static TICK(tinysr_tick) {
  struct sr_data* sr = (struct sr_data*)u->module.gen.data;
  u->out = u->in;

  const m_float val = u->in;
  sr->smp[sr->idx] = (samp_t)((val * range)/2.0);

  if(++sr->idx == READ_SAMPS && sr->ini) {
    tinysr_recognize(sr->ctx, sr->smp, READ_SAMPS);
/*
    if(!sr->state && sr->ctx->utterance_state)
            printf("Utterance detected.\n");
    if(sr->state && !sr->ctx->utterance_state)
      printf("Utterance over.\n");
*/
    sr->state = sr->ctx->utterance_state;

    int word_index;
    float score;
    if(tinysr_get_result(sr->ctx, &word_index, &score)) {
      TINY_SCORE(sr->obj) = score;
      TINY_IDX(sr->obj) = word_index;
      broadcast(TINY_EV(sr->obj));
    }
    sr->idx = 0;
  }
}

static CTOR(tinysr_ctor) {
  struct sr_data* sr = new_sr_data(o, shred->info->vm->bbq->si->sr);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tinysr_tick, sr, 0);
  const M_Object ev = TINY_EV(o) = new_object(shred->info->vm->gwion->mp, NULL, shred->info->vm->gwion->type[et_event]);
  EV_SHREDS(ev) = new_vector(shred->info->vm->gwion->mp);
}

static DTOR(tinysr_dtor) {
  const UGen u = UGEN(o);
  struct sr_data* sr = (struct sr_data*)u->module.gen.data;
  free_sr_data(sr);
  _release(TINY_EV(o), shred );
}

static MFUN(load) {
  const UGen u = UGEN(o);
  struct sr_data* sr = (struct sr_data*)u->module.gen.data;
  const M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj)
    Except(shred, "NullPtrException");
  *(m_int*)RETURN = tinysr_load_model(sr->ctx, STRING(obj));
  _release(obj, shred);
  sr->ini = 1;
}

static MFUN(word) {
  const UGen u = UGEN(o);
  struct sr_data* sr = (struct sr_data*)u->module.gen.data;
  *(M_Object*)RETURN = sr->ini ?
    new_string(shred->info->vm->gwion->mp, shred, sr->ctx->word_names[TINY_IDX(o)]) : NULL;
}

static MFUN(word_index) {
  const m_int idx = *(m_int*)MEM(SZ_INT);
  const UGen u = UGEN(o);
  struct sr_data* sr = (struct sr_data*)u->module.gen.data;
  *(M_Object*)RETURN = idx >= 0 ?
    new_string(shred->info->vm->gwion->mp, shred, sr->ctx->word_names[idx]) : NULL;
}

static MFUN(state) {
  const m_int idx = *(m_int*)MEM(SZ_INT);
  const UGen u = UGEN(o);
  struct sr_data* sr = (struct sr_data*)u->module.gen.data;
  *(m_int*)RETURN = sr->state;
}

GWION_IMPORT(tinysr) {
  const Type t_tinysr = gwi_mk_type(gwi, "TinySR", SZ_INT, "UGen");

  GWI_BB(gwi_class_ini(gwi, "Tinysr", "UGen"))
  gwi_class_xtor(gwi, tinysr_ctor, tinysr_dtor);

  GWI_BB(gwi_item_ini(gwi, "Event", "ev"))
  GWI_BB((o_tinysr_ev = gwi_item_end(gwi, ae_flag_const, NULL)))

  GWI_BB(gwi_item_ini(gwi, "int", "index"))
  GWI_BB((o_tinysr_idx = gwi_item_end(gwi, ae_flag_const , NULL)))

  GWI_BB(gwi_item_ini(gwi, "float", "score"))
  GWI_BB((o_tinysr_score = gwi_item_end(gwi, ae_flag_const, NULL)))

  GWI_BB(gwi_func_ini(gwi, "int", "load"))
  GWI_BB(gwi_func_arg(gwi, "string", "file"))
  GWI_BB(gwi_func_end(gwi, load, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "string", "word"))
  GWI_BB(gwi_func_end(gwi, word, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "string", "word"))
  GWI_BB(gwi_func_arg(gwi, "int", "index"))
  GWI_BB(gwi_func_end(gwi, word_index, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "string", "state"))
  GWI_BB(gwi_func_end(gwi, state, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
