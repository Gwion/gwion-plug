extern "C"
{

#define restrict __restrict__

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

#define gwi_mk_type(a, b, c, d)       gwi_mk_type(a, (m_str)b, c, (m_str)d)
#define gwi_class_ini(a, b, c)        gwi_class_ini(a, (m_str)b, (m_str)c)
#define gwi_item_ini(a, b, c)         gwi_item_ini(a, (m_str)b, (m_str)c)
#define gwi_func_ini(a, b, c)         gwi_func_ini(a, (m_str)b, (m_str)c)
#define gwi_func_arg(a, b, c)         gwi_func_arg(a, (m_str)b , (m_str)c)
#define gwi_func_end(a, b, c)         gwi_func_end(a, (f_xfun)b , c)

}

