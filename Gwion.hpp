extern "C"
{

#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"

#define importer_item_ini(a, b, c) importer_item_ini(a, (m_str)b, (m_str)c)
#define importer_item_end(a, b, c) importer_item_end(a, (ae_flag)(b), (m_uint*)c)
#define importer_func_ini(a, b, c, d) importer_func_ini(a, (m_str)b, (m_str)c, (m_uint)d)
#define importer_func_arg(a, b, c)  importer_func_arg(a, (m_str)b , (m_str)c)

}

