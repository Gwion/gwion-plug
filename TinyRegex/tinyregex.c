#include "re.h"

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

#define RE(o) (*(re_t*)(o)->data)

static MFUN(regex_new) {
  const M_Object pattern = *(M_Object*)MEM(SZ_INT);
  RE(o) = re_compile(STRING(pattern));
  *(M_Object*)RETURN = o;
}

static MFUN(regex_run) {
  const M_Object text    = *(M_Object*)MEM(SZ_INT);
  m_int *const len       = *(m_int**)MEM(SZ_INT *2);
  int matchlength;
  *(m_int*)RETURN = re_matchp(RE(o), STRING(text), &matchlength);
  *len = matchlength;
}

static SFUN(regex_get) {
  const M_Object pattern = *(M_Object*)MEM(0);
  const M_Object text    = *(M_Object*)MEM(SZ_INT);
  m_int *const len       = *(m_int**)MEM(SZ_INT *2);
  int matchlength;
  *(m_int*)RETURN = re_match(STRING(pattern), STRING(text), &matchlength);
  *len = matchlength;
}

GWION_IMPORT(TinyRegex) {
  DECL_B(const Type, t_tinyregex, = gwi_class_ini(gwi, "TinyRegex", "Object"));

  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "pattern"));
  GWI_B(gwi_func_end(gwi, regex_new, ae_flag_none));

  GWI_B(gwi_func_ini(gwi, "int", "run"));
  GWI_B(gwi_func_arg(gwi, "string", "text"));
  GWI_B(gwi_func_arg(gwi, "&int", "lenght"));
  GWI_B(gwi_func_end(gwi, regex_run, ae_flag_none));

  GWI_B(gwi_func_ini(gwi, "int", "get"));
  GWI_B(gwi_func_arg(gwi, "string", "pattern"));
  GWI_B(gwi_func_arg(gwi, "string", "text"));
  GWI_B(gwi_func_arg(gwi, "&int", "lenght"));
  GWI_B(gwi_func_end(gwi, regex_get, ae_flag_static));

  GWI_B(gwi_class_end(gwi));
  return true;
}
