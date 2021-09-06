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
#include "gack.h"

static GACK(termcolor_gack) {
  char static_str[1024];
  const M_Object obj = *(M_Object *)VALUE;
  const size_t sz = strlen(STRING(obj)) * 2;
  const bool not_too_big = sz <= 1024;
  char *const buf = not_too_big ?
    static_str : mp_malloc2(shred->info->mp, sz);
  const m_str str = STRING(obj);
  const enum term_color_error_t err = tcol_snprintf(buf, sz, str);
  INTERP_PRINTF("%s", err == TermColorErrorNone ? buf : str);
  if(!not_too_big)
    mp_free2(shred->info->mp, sz, buf);
}

GWION_IMPORT(TermColor) {

  DECL_OB(const Type, t_termcolor, = gwi_class_ini(gwi, "TermColor", "string"));
  SET_FLAG(t_termcolor, final | ae_flag_abstract);
  gwi_gack(gwi, t_termcolor, termcolor_gack);
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "string", "TermColor", "TermColor"));
  GWI_BB(gwi_oper_end(gwi, "@implicit", NoOp));
  GWI_BB(gwi_oper_end(gwi, "$", NoOp));

  return GW_OK;
}
