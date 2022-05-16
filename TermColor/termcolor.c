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

// Define static function `TermColor`.
static SFUN(TermColor) {
  const M_Object obj = *(M_Object *)MEM(0);  // Fetch first argument.
  m_str str = STRING(obj);  // Access string data.

  size_t sz = strlen(STRING(obj)) * 4;  // Grow by factor of 4x
  char* buf = mp_malloc2(shred->info->mp, sz);  // Allocate memory in pool.

  // Hand the string to tcol, it will put the result in `buf`.
  /*enum term_color_error_t err = */
  (void)tcol_snprintf(buf, sz, str);

  // Set the return value to the new string (the string constructor implicitly copies buf).
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, buf);

  // Free the buffer from the memory pool.
  mp_free2(shred->info->mp, sz, buf);
}

GWION_IMPORT(TermColor) {
  // Declare function `TermColor` that accepts a string argument and returns a
  // a new formatted string.
  GWI_BB(gwi_func_ini(gwi, "string", "TermColor"));
  GWI_BB(gwi_func_arg(gwi, "string", "str"));
  GWI_BB(gwi_func_end(gwi, TermColor, ae_flag_none));

  return GW_OK;
}
