//       #include <stdio.h>
//       #include <readline/readline.h>
//       #include <readline/history.h>
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


extern char *readline (const char *prompt);

static SFUN(gw_readline0) {
  const m_str str = readline(NULL);
  if(str) *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str);
  else handle(shred, "ReadlineError");
}


static SFUN(gw_readline1) {
  const M_Object prompt = *(M_Object*)MEM(0);
  const m_str str = readline(STRING(prompt));
  if(str) *(M_Object*)RETURN = new_string(shred->info->vm->gwion, str);
  else handle(shred, "ReadlineError");
}

GWION_IMPORT(Readline) {
  DECL_OB(const Type, t_readline, = gwi_struct_ini(gwi, "Readline"));

  GWI_BB(gwi_func_ini(gwi, "string", "run"));
  GWI_BB(gwi_func_end(gwi, gw_readline0, ae_flag_static));

  GWI_BB(gwi_func_ini(gwi, "string", "run"));
  GWI_BB(gwi_func_arg(gwi, "string", "prompt"));
  GWI_BB(gwi_func_end(gwi, gw_readline1, ae_flag_static));

  GWI_BB(gwi_struct_end(gwi));
  return GW_OK;
}
