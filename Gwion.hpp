extern "C"
{

#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

#define import_var(a, b, c, d, e) import_var(a, (m_str)b, (m_str)c, (ae_flag)d, e)
#define import_fun(a, b, c)        import_fun(a, b, (ae_flag)c)
#define dl_func_add_arg(a, b, c)   dl_func_add_arg(a, (m_str)b , (m_str)c)
#define dl_func_init(a, b, c, d)   dl_func_init(a, (m_str)b, (m_str)c, (m_uint)d)

}

