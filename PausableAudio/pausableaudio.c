#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "instr.h"
#include "import.h"

typedef struct Pausable_ {
  DriverData dd;
  bool play;
  bool suspend;
} Pausable;

static DRVRUN(pausable_run) {
  Pausable *pausable = di->driver->data;
  Driver drv = *di;
  drv.driver = &pausable->dd;
  while (di->is_running) {
    if(!pausable->play) {
      di->run(vm);
      next_bbq_pos(vm);
    } else {
      pausable->dd.run(vm, &drv);
      if(pausable->suspend)
        di->is_running = true;
      pausable->suspend = false;
    }
  }
}

static DRVDEL(pausable_del) {
  Pausable *pausable = di->driver->data;
  Driver drv = *di;
  drv.driver = &pausable->dd;
  if(pausable->dd.del) pausable->dd.del(vm, &drv);
  mp_free(vm->gwion->mp, Pausable, pausable);
}

ANN static bool child_init(VM *vm, Driver *drv) {
  DECL_B(gwdriver_t, func, = driver_ini(vm->gwion, drv->si));
  func(drv->driver);
  return drv->driver->ini(vm, drv);
}

static DRVINI(pausable_ini) {
  char *const old_arg = di->si->arg;
  char *arg = strchr(old_arg, '=');
  if(!arg) {
    gw_err("{R}PausableAudio{0} driver needs an argument\n");
    return false;
  }
  arg++;
  struct SoundInfo_ si = *di->si;
  si.arg = arg;
  Pausable *pausable = di->driver->data =
    mp_calloc(vm->gwion->mp, Pausable);
  Driver drv = {
    .driver = &pausable->dd,
    .si = &si
  };
  if(child_init(vm, &drv)) return true;
  gw_err("{R}PausableAudio{0} error while initializing child driver\n");
  return false;
}

GWDRIVER(PausableAudio) {
  d->ini = pausable_ini;
  d->run = pausable_run;
  d->del = pausable_del;
}

static SFUN(get) {
  Pausable *pausable = shred->info->vm->bbq->driver->data;
  *(m_uint*)RETURN = pausable->play;
}

static SFUN(toggle) {
  Pausable *pausable = shred->info->vm->bbq->driver->data;
  pausable->play = *(m_uint*)MEM(0);
  if(!pausable->play) {
    shred->info->vm->bbq->is_running = false;
    shredule(shred->tick->shreduler, shred, GWION_EPSILON);
  } else pausable->suspend = true;
}

GWION_IMPORT(PausableAudio) {
  GWI_B(gwi_struct_ini(gwi, "PausableAudio"));
  GWI_B(gwi_func_ini(gwi, "bool", "play"));
  GWI_B(gwi_func_end(gwi, get, ae_flag_static));
  GWI_B(gwi_func_ini(gwi, "void", "play"));
  GWI_B(gwi_func_arg(gwi, "bool", "p"));
  GWI_B(gwi_func_end(gwi, toggle, ae_flag_static));
  GWI_B(gwi_class_end(gwi));
  return true;
}
