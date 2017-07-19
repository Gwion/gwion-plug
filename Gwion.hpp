extern "C"
{
#include "defs.h"
#include "err_msg.h"
//typedef _Complex complex;
extern m_int o_string_data;
#ifndef __ARRAY
#define __ARRAY
#include <complex.h>
#include "map.h"

typedef struct VM_Vector_* VM_Vector;

struct VM_Array_ {
  M_Vector* vector;
//  Map    map;
  Type type;
  m_bool is_obj;
};
#endif

#include "type.h"
#include "instr.h"
#include "import.h"
typedef struct VM_Shred_* VM_Shred;
#define import_var(a, b, c, d, e) import_var(a, (m_str)b, (m_str)c, (ae_flag)d, e)
#define import_fun(a, b, c)        import_fun(a, b, (ae_flag)c)
#define dl_func_add_arg(a, b, c)   dl_func_add_arg(a, (m_str)b , (m_str)c)
#define dl_func_init(a, b, c, d)   dl_func_init(a, (m_str)b, (m_str)c, (m_uint)d)
}

