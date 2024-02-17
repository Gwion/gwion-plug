#include <lo/lo.h>
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
#include "gwi.h"

#define BINIOU_ADDR(o) (*(lo_address*)((o)->data + SZ_INT))

static TICK(lebiniou_tick) {
  const lo_message msg = lo_message_new();
  const m_bit* data = u->module.gen.data;
  const m_float left = UGEN(u->connect.multi->channel[0])->in;
  const m_float right = UGEN(u->connect.multi->channel[1])->in;
  lo_message_add_double(msg, left);
  lo_message_add_double(msg, right);
  const lo_address addr = *(lo_message*)(data);
  lo_send_message(addr, "/lebiniou/audioinput", msg);
  lo_message_free(msg);
  UGEN(u->connect.multi->channel[0])->out = left;
  UGEN(u->connect.multi->channel[1])->out = right;
}

static DTOR(lebiniou_dtor) {
  lo_address_free(BINIOU_ADDR(o));
}

GWION_IMPORT(LeBiniou) {
  DECL_B(const Type, t_lebiniou, = gwi_class_ini(gwi, "@LeBiniou", "UGen"));
  gwi_class_xtor(gwi, NULL, lebiniou_dtor);
  t_lebiniou->nspc->offset += SZ_INT; // room for the lo_thread
  GWI_B(gwi_class_end(gwi))


  const M_Object o = new_object(gwi->gwion->mp, t_lebiniou);
  UGEN(o) = new_UGen(gwi->gwion->mp);
  ugen_ini(gwi->gwion, UGEN(o), 2, 2);
  ugen_gen(gwi->gwion, UGEN(o), lebiniou_tick,
           o->data + SZ_INT, 0);
  vector_add(&gwi->gwion->vm->ugen, (m_uint)UGEN(o));
  BINIOU_ADDR(o) = lo_address_new("localhost", "9999");
  gwi_item_ini(gwi, "@LeBiniou", "LeBiniou");
  gwi_item_end(gwi, ae_flag_const, obj, o);
  return true;
}

static const m_str deps[] = { "Lo", NULL };
GWDEPEND(LeBiniou){ return deps; }
