#include <soundpipe.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

GWMODSTR(Soundpipe)
GWMODINI(Soundpipe) {
  sp_data *sp;
  sp_createn(&sp, gwion->vm->bbq->si->out);
  sp->sr = gwion->vm->bbq->si->sr;
  return sp;
}
GWMODEND(Soundpipe) {
  sp_destroy((sp_data**)&self);
}
