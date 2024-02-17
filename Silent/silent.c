#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "driver.h"

static DRVINI(Silent_ini) {
  return true;
}

static DRVRUN(Silent_run) {
  while(true) {
    for(uint32_t i = 0; i < di->si->sr; i++){
      di->run(vm);
      next_bbq_pos(vm);
    }
    if(!di->is_running) break;
    sleep(1);
  }
}

GWDRIVER(Silent) {
  d->ini = Silent_ini;
  d->run = Silent_run;
}

