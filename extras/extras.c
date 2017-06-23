#include "type.h"
#include "dl.h"
#include "import.h"
#include "err_msg.h"
#include "lang.h"

static struct Type_ t_extras = { "extras", SZ_INT, &t_object };

CTOR(extras_ctor) { /*code here */ }

DTOR(extras_dtor) { /*code here */ }

m_int o_extras_member_data;
m_int o_extras_static_data;
m_int* o_extras_static_value;

static MFUN(mfun) { /*code here */ }
static SFUN(sfun) { /*code here */ }

IMPORT
{
  DL_Func fun;

  CHECK_BB(add_global_type(env, &t_extras))
  CHECK_BB(import_class_begin(env, &t_extras, env->global_nspc, extras_ctor, extras_dtor))

  o_extras_member_data = import_mvar(env, "int",  "member", 0, 0, "doc");

  o_extras_static_value = malloc(sizeof(m_int));
  o_extras_static_data = import_svar(env, "int", "static", 1, 0, o_extras_static_value, "doc.");

  dl_func_init(&fun, "int", "mfun",  (m_uint)mfun);
    dl_func_add_arg(&fun, "int", "arg");
  CHECK_BB(import_fun(env, &fun), 0))

  dl_func_init(&fun, "int", "sfun",  (m_uint)sfun);
    dl_func_add_arg(&fun, "int", "arg");
  CHECK_BB(import_fun(env, &fun), 0))

  CHECK_BB(import_class_end(env))
  return 1;
}
