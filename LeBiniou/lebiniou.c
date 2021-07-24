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

static CTOR(lebiniou_ctor) {
  BINIOU_ADDR(o) = lo_address_new("localhost", "9999");
  ugen_ini(shred->info->vm->gwion, UGEN(o), 2, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), lebiniou_tick,
           o->data + SZ_INT, 0);
}

static DTOR(lebiniou_dtor) {
  lo_address_free(BINIOU_ADDR(o));
}

GWION_IMPORT(LeBiniou) {
  DECL_OB(const Type, t_lebiniou, = gwi_class_ini(gwi, "LeBiniou", "UGen"));
  gwi_class_xtor(gwi, lebiniou_ctor, lebiniou_dtor);
  SET_FLAG(t_lebiniou, abstract);
  t_lebiniou->nspc->info->offset += SZ_INT; // room for the lo_thread
  GWI_BB(gwi_class_end(gwi))
  const M_Object o = new_object(gwi->gwion->mp, NULL, t_lebiniou);
  GWI_BB(gwi_item_ini(gwi, "LeBiniou", "LeBiniou"));
  GWI_BB(gwi_item_end(gwi, ae_flag_const, obj, o));
  return GW_OK;
}

static const m_str deps[] = { "Lo", NULL };
GWDEPEND{ return deps; }
