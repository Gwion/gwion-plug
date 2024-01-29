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
#include "gwi.h"
#include "gwion.h"
#include "shreduler_private.h"

#define ANNA(o) ((Anna*)(UGEN(o)->module.gen.data))

typedef struct Anna {
  m_float phase;
  m_float num;
} Anna;

static TICK(anna_tick) {
  Anna *anna = u->module.gen.data;
  u->out = sin( anna->phase * (M_PI*2) );
  anna->phase += anna->num;
  // keep the phase between 0 and 1
  if( anna->phase > 1.0 ) anna->phase -= 1.0;
  else if( anna->phase < 0.0 ) anna->phase += 1.0;
}

static DTOR(dtor) {
  // should use the memory pool
  free(ANNA(o));
}

static CTOR(ctor) {
  // should use the memory pool
  UGEN(o)->module.gen.data= malloc(sizeof(Anna));
  ANNA(o)->num = 440 / shred->tick->shreduler->bbq->si->sr;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), anna_tick, ANNA(o), 0);
}

static MFUN(anna_set_freq) {
  // the object is in MEM(0)
  const m_float freq = *(m_float*)MEM(SZ_INT);
  ANNA(o)->num = freq / shred->tick->shreduler->bbq->si->sr;
}

GWION_IMPORT(Anna) {
  DECL_OB(const Type, t_anna, = gwi_class_ini(gwi, "Anna", "UGen"));
  gwi_class_xtor(gwi, ctor, dtor);
  t_anna->nspc->offset += SZ_INT; // room for the synth

  GWI_BB(gwi_func_ini(gwi, "float", "freq"));
  GWI_BB(gwi_func_arg(gwi, "float", "arg"));
  GWI_BB(gwi_func_end(gwi, anna_set_freq, ae_flag_none));

  GWI_BB(gwi_class_end(gwi));
  return GW_OK;
}
