#include <libevdev/libevdev.h>
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
#include "gw_evdev.h"

ANN static void to_ev(struct input_event *ev, const gw_input_event *e) {
  ev->type         = e->type;
  ev->code         = e->code;
  ev->value        = e->value;
  ev->time.tv_sec  = e->sec;
  ev->time.tv_usec = e->usec;
}

static MFUN(evdevev_is_type) {
  struct input_event ev;
  to_ev(&ev, EVDEVEV(o));
  const m_int type = *(m_int*)MEM(SZ_INT);
  *(m_int*)RETURN = libevdev_event_is_type(&ev, type);
}

static MFUN(evdevev_is_code) {
  struct input_event ev;
  to_ev(&ev, EVDEVEV(o));
  const m_int type = *(m_int*)MEM(SZ_INT);
  const m_int code = *(m_int*)MEM(SZ_INT*2);
  *(m_int*)RETURN = libevdev_event_is_code(&ev, type, code);
}

GWION_IMPORT(evdevev) {
  GWI_B(gwi_class_ini(gwi, "Ev", NULL))
  GWI_B(gwi_item_ini(gwi, "int", "type"))
  GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0))
  GWI_B(gwi_item_ini(gwi, "int", "code"))
  GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0))
  GWI_B(gwi_item_ini(gwi, "int", "value"))
  GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0))
  GWI_B(gwi_item_ini(gwi, "int", "sec"))
  GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0))
  GWI_B(gwi_item_ini(gwi, "int", "usec"))
  GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0))
  GWI_B(gwi_func_ini(gwi, "int", "is_type"))
  GWI_B(gwi_func_arg(gwi, "int", "type"))
  GWI_B(gwi_func_end(gwi, evdevev_is_type, ae_flag_none))
  GWI_B(gwi_func_ini(gwi, "int", "is_code"))
  GWI_B(gwi_func_arg(gwi, "int", "type"))
  GWI_B(gwi_func_arg(gwi, "int", "code"))
  GWI_B(gwi_func_end(gwi, evdevev_is_code, ae_flag_none))
  GWI_B(gwi_class_end(gwi))
  return true;
}
