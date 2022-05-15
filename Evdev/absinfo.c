#include <libevdev/libevdev.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gw_evdev.h"

#define describe_absinfo(name)                  \
static MFUN(absinfo_get_##name) {               \
  const struct input_absinfo* abs = ABSINFO(o); \
  *(m_int*)RETURN = abs->name;                  \
}                                               \
static MFUN(absinfo_set_##name) {               \
  struct input_absinfo* abs = ABSINFO(o);       \
  const m_int i = *(m_int*)MEM(SZ_INT);         \
  *(m_int*)RETURN = abs->name = i;              \
}

describe_absinfo(value)
describe_absinfo(minimum)
describe_absinfo(maximum)
describe_absinfo(fuzz)
describe_absinfo(flat)
describe_absinfo(resolution)

#define _import_absinfo(name)                                   \
  GWI_BB(gwi_func_ini(gwi, "int", #name))                       \
  GWI_BB(gwi_func_end(gwi, absinfo_get_##name , ae_flag_none))  \
  GWI_BB(gwi_func_ini(gwi, "int", #name))                       \
  GWI_BB(gwi_func_arg(gwi, "int", "val"))                       \
  GWI_BB(gwi_func_end(gwi, absinfo_set_##name , ae_flag_none))

ANN m_bool import_absinfo(const Gwi gwi) {
  DECL_OB(const Type, t_abs, = gwi_class_ini(gwi, "AbsInfo", NULL));
  t_abs->nspc->offset += SZ_INT;
  GWI_BB(gwi_item_ini(gwi, "@internal", "@info"))
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0))
  _import_absinfo(value)
  _import_absinfo(minimum)
  _import_absinfo(maximum)
  _import_absinfo(fuzz)
  _import_absinfo(flat)
  _import_absinfo(resolution)
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
