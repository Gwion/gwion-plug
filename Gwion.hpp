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
typedef struct {
  m_uint length;
  m_uint depth;
  Type type;
  m_bool is_obj;
  m_bool is_ref;
  m_uint stack_offset;
} VM_Array_Info;

struct VM_Array_ {
  M_Vector* vector;
//  Map    map;
  Type type;
  m_bool is_obj;
};

m_uint    m_vector_size(M_Vector* v);
m_uint    i_vector_at  (M_Vector* v, m_uint i);
m_float   f_vector_at  (M_Vector* v, m_uint i);
/*
#ifdef USE_DOUBLE
complex double  c_vector_at  (M_Vector* v, m_uint i);
#else
complex float  c_vector_at  (M_Vector* v, m_uint i);
#endif
*/
m_vec3   v3_vector_at  (M_Vector* v, m_uint i);
m_vec4   v4_vector_at  (M_Vector* v, m_uint i);
m_uint*   i_vector_addr(M_Vector* v, m_uint i);
m_float*  f_vector_addr(M_Vector* v, m_uint i);

_Complex c_vector_addr(M_Vector* v, m_uint i);
m_vec3*  v3_vector_addr(M_Vector* v, m_uint i);
m_vec4*  v4_vector_addr(M_Vector* v, m_uint i);
void      i_vector_set (M_Vector* v, m_uint i, m_uint  data);
void      f_vector_set (M_Vector* v, m_uint i, m_float data);
void      c_vector_set (M_Vector* v, m_uint i, _Complex data);
void     v3_vector_set (M_Vector* v, m_uint i, m_vec3  data);
void     v4_vector_set (M_Vector* v, m_uint i, m_vec4  data);

#endif

#include "type.h"
#include "dl.h"
#include "import.h"
typedef struct VM_Shred_* VM_Shred;
#define import_var(a, b, c, d, e) import_var(a, (m_str)b, (m_str)c, (ae_flag)d, e)
#define import_fun(a, b, c)        import_fun(a, b, (ae_flag)c)
#define dl_func_add_arg(a, b, c)   dl_func_add_arg(a, (m_str)b , (m_str)c)
#define dl_func_init(a, b, c, d)   dl_func_init(a, (m_str)b, (m_str)c, (m_uint)d)
}

