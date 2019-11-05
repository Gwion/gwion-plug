extern "C"
{

#define restrict __restrict__

#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "type.h"
#include "object.h"
#include "instr.h"
#include "gwion.h"
#include "plug.h"
#include "value.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"

#define gwi_mk_type(a, b, c, d) gwi_mk_type(a, (m_str)b, c, (m_str)d)
#define gwi_class_ini(a, b, c) gwi_item_ini(a, (m_str)b, (m_str)c)
#define gwi_item_ini(a, b, c) gwi_item_ini(a, (m_str)b, (m_str)c)
#define gwi_func_ini(a, b, c) gwi_func_ini(a, (m_str)b, (m_str)c)
#define gwi_func_arg(a, b, c)  gwi_func_arg(a, (m_str)b , (m_str)c)
#define gwi_func_end(a, b, c)  gwi_func_end(a, (f_xfun)b , c)

}

