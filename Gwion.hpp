extern "C"
{

#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"

#define importer_add_var(a, b, c, d, e) importer_add_var(a, (m_str)b, (m_str)c, (ae_flag)d, e)
#define importer_add_arg(a, b, c)  importer_add_arg(a, (m_str)b , (m_str)c)
#define importer_func_begin(a, b, c, d) importer_func_begin(a, (m_str)b, (m_str)c, (m_uint)d)

}

