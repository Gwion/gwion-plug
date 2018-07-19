extern "C"
{

#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"

#define gwi_mk_type(a, b, c, d) gwi_mk_type(a, (m_str)b, c, d)
#define gwi_item_ini(a, b, c) gwi_item_ini(a, (m_str)b, (m_str)c)
#define gwi_func_ini(a, b, c, d) gwi_func_ini(a, (m_str)b, (m_str)c, (f_xfun)d)
#define gwi_func_arg(a, b, c)  gwi_func_arg(a, (m_str)b , (m_str)c)

}

