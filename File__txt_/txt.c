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
#include "gwi.h"
#include "emit.h"
#include "traverse.h"

static const m_str deps[] = { "File", NULL };
GWDEPEND{ return deps; }

static DTOR(filetxt_dtor) {
  fclose(*(FILE**)(o->data + SZ_INT));
}

static INSTR(TxtWrite) {
  POP_REG(shred, SZ_INT*2);
  const m_str str = *(m_str*)REG(SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  fprintf(*(FILE**)(o->data + SZ_INT), str);
}

m_bool emit_interp(const Emitter, const Exp);
static OP_EMIT(opem_txt_write) {
  Exp_Binary *bin = (Exp_Binary*)data;
  CHECK_BB(emit_interp(emit, bin->rhs))
  emit_add_instr(emit, TxtWrite);
  return GW_OK;
}

static OP_CHECK(opck_txt_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  if(!call->args || !call->args->next || call->args->next->next)
    ERR_N(exp_self(call)->pos, _("File:[txt] construtor needs two arguments"))
  CHECK_BN(check_exp(env, call->args))
  if(isa(call->args->type, env->gwion->type[et_string]) < 0 ||
     isa(call->args->next->type, env->gwion->type[et_string]) < 0)
    ERR_N(exp_self(call)->pos, _("File:[txt] construtor arguments must be of type string"))
  return actual_type(env->gwion, call->func->type);
}

static MFUN(fileread) {
  FILE *f = *(FILE**)(o->data + SZ_INT);
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *string = _mp_malloc(shred->info->vm->gwion->mp, fsize + 1);
  fread(string, 1, fsize, f);
  string[fsize] = 0;
  const M_Object ret = new_object(shred->info->vm->gwion->mp, shred, shred->info->vm->gwion->type[et_string]);
  STRING(ret) = string;
  *(M_Object*)RETURN = ret;
}

static INSTR(FileTxtCtor) {
  POP_REG(shred, SZ_INT*2);
  const Type t = *(Type*)REG(SZ_INT);
  const M_Object mode = *(M_Object*)REG(0);
  const M_Object filename = *(M_Object*)REG(-SZ_INT);
  FILE *f = fopen(STRING(filename), STRING(mode));
  if(!f)
    Except(shred, _("can't open file"));
  const M_Object o = new_object(shred->info->vm->gwion->mp, shred, t);
  *(Vector*)o->data = new_vector(shred->info->vm->gwion->mp);
  *(FILE**)(o->data + SZ_INT) = f;
  *(M_Object*)REG(-SZ_INT) = o;
}

GWION_IMPORT(File:[text]) {
  const Type t = gwi_class_ini(gwi, "@Filetxt", "File:[@internal]");
  gwi_class_xtor(gwi, NULL, filetxt_dtor);
  GWI_BB(gwi_func_ini(gwi, "string", "read"))
  GWI_BB(gwi_func_end(gwi, fileread, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, NULL, "@Filetxt", "@Filetxt"))
  GWI_BB(gwi_oper_add(gwi, opck_txt_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", FileTxtCtor))

  GWI_BB(gwi_oper_ini(gwi, "@Filetxt", (m_str)OP_ANY_TYPE, "@Filetxt"))
  GWI_BB(gwi_oper_emi(gwi, opem_txt_write))
  GWI_BB(gwi_oper_end(gwi, "<<", NULL))

  return GW_OK;
}
