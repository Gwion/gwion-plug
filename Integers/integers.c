#include <stdlib.h>
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

GWION_IMPORT(Integers) {
  const Type t_uint = gwi_mk_type(gwi, "uint", SZ_INT, NULL);
  return GW_OK;
}