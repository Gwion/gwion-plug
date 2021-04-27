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

static SFUN(gw_getline) {


}

GWION_IMPORT(Getline) {
  DECL_OB(const Type, t_getline, = gwi_struct_ini(gwi, "getline"));

  GWI_BB(gwi_func_ini(gwi, "int", "line"))
  GWI_BB(gwi_func_arg(gwi, "Ref:[int]", "n"))
  GWI_BB(gwi_func_end(gwi, gw_getline, ae_flag_static))
/*
  GWI_BB(gwi_func_ini(gwi, "int", "sfun"))
  GWI_BB(gwi_func_arg(gwi, "int", "arg"))
  GWI_BB(gwi_func_end(gwi, sfun, ae_flag_static))
*/
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
