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
#include "array.h"

#include "cmojify.h"
#include "traverse.h"
#include "parse.h"

static OP_CHECK(opck_emoji_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  const Type t_string = env->gwion->type[et_string];
  if(!call->args || call->args->next ||
    !check_exp(env, call->args) || isa(call->args->type, t_string) < 0)
      ERR_N(exp_self(call)->pos, _("Emoji require one string argument"))
  call->func->d.prim.d.var = insert_symbol("@Emojify");
  call->func->type = NULL;
  CHECK_ON(check_exp(env, call->func))
  return t_string;
}

static SFUN(sfun) {
  const m_str in = STRING(*(M_Object*)MEM(0));
  const size_t sz = strlen(in);
  char out[sz + 1];
  cmojify(out, sz, in);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, out);
}

GWION_IMPORT(Emoji) {
  DECL_OB(const Type, t_emoji, = gwi_class_ini(gwi, "Emoji", "string"));
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_func_ini(gwi, "string", "@Emojify"))
  GWI_BB(gwi_func_arg(gwi, "string", "arg"))
  GWI_BB(gwi_func_end(gwi, sfun, ae_flag_none))

  GWI_BB(gwi_oper_ini(gwi, NULL, "Emoji", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_emoji_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  return GW_OK;
}
