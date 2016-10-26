extern "C"
{
#include "defs.h"
#include "err_msg.h"
typedef _Complex complex;
extern m_int o_string_data;

#include "type.h"
#include "dl.h"
#include "bbq.h"
extern m_bool initialize_object(M_Object o, Type t);
extern Type import_class_begin(Env env, Type type, NameSpace where, f_ctor pre_ctor, f_dtor dtor);
extern m_bool import_class_end(Env env);
extern m_bool add_global_type(Env env, Type t);
m_int import_mvar(Env env, const m_str type,
  const m_str name, m_bool is_const, m_bool is_ref, const m_str doc );
extern Func import_mfun(Env env, DL_Func * mfun);
}
struct Env_
{
  NameSpace curr;
  NameSpace global_nspc;
  NameSpace user_nspc;
  m_uint    class_scope;
  Context   global_context;
  Context   context;
  Vector    contexts;
  Vector    nspc_stack;
  Vector    class_stack;
  Vector    breaks;
  Vector    conts;
  Type      class_def;
  Func      func;
  VM_Object obj;
  Map known_ctx;
};

typedef struct VM_Shred_* VM_Shred;
#define import_mvar(a, b, c, d, e, f) import_mvar(a, (m_str)b , (m_str)c, d, e, (m_str)f)
#define dl_func_add_arg(a, b, c)      dl_func_add_arg(a, (m_str)b , (m_str)c) 
#define new_DL_Func(a, b, c)          new_DL_Func((m_str)a, (m_str)b, (m_uint)c)               
