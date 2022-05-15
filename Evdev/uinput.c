#include <fcntl.h>
#include <unistd.h>
#include <libevdev/libevdev-uinput.h>
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

#define UINPUT(o) *(struct libevdev_uinput**)(o->data)

static DTOR(uinput_dtor) {
  struct libevdev_uinput *uidev = UINPUT(o);
  if(uidev) {
    const int fd = libevdev_uinput_get_fd(uidev);
    libevdev_uinput_destroy(uidev);
    close(fd);
  }
}

static MFUN(uinput_fd) {
  struct libevdev_uinput* uidev = UINPUT(o);
  *(m_int*)RETURN = uidev ? libevdev_uinput_get_fd(uidev): -1;
}

#define describe_uinput(func)                                                \
static MFUN(uinput_##func) {                                                 \
  struct libevdev_uinput* uidev = UINPUT(o);                                 \
  *(M_Object*)RETURN = uidev ?                                               \
    new_string(shred->info->vm->gwion, (const m_str)libevdev_uinput_get_##func(uidev)): NULL; \
}
describe_uinput(syspath)
describe_uinput(devnode)

static MFUN(uinput_write) {
  struct libevdev_uinput* uidev = UINPUT(o);
  const m_int type  = *(m_int*)MEM(SZ_INT);
  const m_int code  = *(m_int*)MEM(SZ_INT);
  const m_int value = *(m_int*)MEM(SZ_INT*2);
  *(m_int*)RETURN = uidev ?
    libevdev_uinput_write_event(uidev, type, code, value): -1;
}

ANN m_bool import_uinput(const Gwi gwi) {
  DECL_OB(const Type, t_uinput, = gwi_class_ini(gwi, "Uinput", NULL));
  gwi_class_xtor(gwi, NULL, uinput_dtor);
  SET_FLAG(t_uinput, abstract);
  t_uinput->nspc->offset += SZ_INT;
  GWI_BB(gwi_func_ini(gwi, "int", "fd"))
  GWI_BB(gwi_func_end(gwi, uinput_fd, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "string", "syspath"))
  GWI_BB(gwi_func_end(gwi, uinput_syspath, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "string", "devnode"))
  GWI_BB(gwi_func_end(gwi, uinput_devnode, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "string", "write"))
  GWI_BB(gwi_func_arg(gwi, "int", "type"))
  GWI_BB(gwi_func_arg(gwi, "int", "code"))
  GWI_BB(gwi_func_arg(gwi, "int", "value"))
  GWI_BB(gwi_func_end(gwi, uinput_write, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
