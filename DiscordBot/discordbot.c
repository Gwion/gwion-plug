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

static SFUN(sfun1) {
  printf("\\e[gwi1%s\\e[gwi",
    STRING(*(M_Object*)MEM(0)));
}

static SFUN(sfun2) {
  printf("\\e[gwi2%s\\e%s\\e[gwi",
    STRING(*(M_Object*)MEM(0)),
    STRING(*(M_Object*)MEM(SZ_INT)));
}

static SFUN(sfun3) {
  printf("\\e[gwi3%s\\e%s\\e%s\\e[gwi",
    STRING(*(M_Object*)MEM(0)),
    STRING(*(M_Object*)MEM(SZ_INT)),
    STRING(*(M_Object*)MEM(SZ_INT*2)));
}


GWION_IMPORT(DiscordBot) {
  DECL_OB(const Type, t_discordbot, = gwi_class_ini(gwi, "DiscordBot", "Object"))
  SET_FLAG(t_discordbot, abstract);

  GWI_BB(gwi_func_ini(gwi, "void", "send"))
  GWI_BB(gwi_func_arg(gwi, "string", "message"))
  GWI_BB(gwi_func_end(gwi, sfun1, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "void", "send"))
  GWI_BB(gwi_func_arg(gwi, "string", "title"))
  GWI_BB(gwi_func_arg(gwi, "string", "message"))
  GWI_BB(gwi_func_end(gwi, sfun2, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "void", "send"))
  GWI_BB(gwi_func_arg(gwi, "string", "title"))
  GWI_BB(gwi_func_arg(gwi, "string", "message"))
  GWI_BB(gwi_func_arg(gwi, "string", "color"))
  GWI_BB(gwi_func_end(gwi, sfun3, ae_flag_static))

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
