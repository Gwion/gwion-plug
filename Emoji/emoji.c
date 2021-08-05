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

#include "cmojify.h"
#include "traverse.h"
#include "parse.h"

static MFUN(mfun) {
  const m_str in = STRING(*(M_Object*)MEM(SZ_INT));
  const size_t sz = strlen(in);
  char out[sz + 1];
  cmojify(out, sz, in);
  *(M_Object*)RETURN = new_string(shred->info->mp, shred, out);
}

GWION_IMPORT(Emoji) {
  DECL_OB(const Type, t_emoji, = gwi_class_ini(gwi, "Emoji", "string"));
  GWI_BB(gwi_func_ini(gwi, "auto", "new"))
  GWI_BB(gwi_func_arg(gwi, "string", "arg"))
  GWI_BB(gwi_func_end(gwi, mfun, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
