#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "traverse.h"
#include "gwi.h"

typedef Type (*filemodule)(const Gwion);

static OP_CHECK(opck_file_scan) {
  struct TemplateScan *ts = (struct TemplateScan*)data;
  if(!ts->td->types || ts->td->types->len)
    ERR_N(ts->td->pos, _("File needs one template argument"));
  const Symbol sym = (*mp_vector_at(ts->td->types, Type_Decl*, 0))->xid;
  const m_str module = s_name(sym);
  char c[64];
  const size_t sz = strlen(module);
  strcpy(c, "@File");
  strcpy(c + 5, module);
  c[5 + sz] = '\0';
  return nspc_lookup_type1(ts->t->nspc, insert_symbol(env->gwion->st, c));
}

static OP_CHECK(opck_file_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  if(!call->tmpl->call || call->tmpl->call->len)
    ERR_N(exp_self(call)->pos, _("File construtor needs one template argument"));
  const Symbol sym = (*mp_vector_at(call->tmpl->call, Type_Decl*, 0))->xid;
  const m_str module = s_name(sym);
  char c[64];
  const size_t sz = strlen(module);
  strcpy(c, "@File");
  strcpy(c + 5, module);
  c[5 + sz] = '\0';
  call->tmpl = NULL; // free tmpl
  call->func->d.prim.d.var = insert_symbol(env->gwion->st, c);
  call->func->type = NULL;
  CHECK_BN(traverse_exp(env, exp_self(call)));
  const Type ret = actual_type(env->gwion, call->func->type);
  return ret;
}

GWMODINI(File) {
  return new_map(gwion->mp);
}

GWMODEND(File) {
  free_map(gwion->mp, self);
}

GWION_IMPORT(File) {
  const Map map = get_module(gwi->gwion, "File");
  if(!map)
    set_module(gwi->gwion, "File", new_map(gwi->gwion->mp));
  DECL_OB(const Type, t_file, = gwi_class_ini(gwi, "File:[module]", "Event"));
  SET_FLAG(t_file, abstract);
  t_file->nspc->offset += SZ_INT;
  GWI_BB(gwi_class_end(gwi))

  set_tflag(t_file, tflag_tmpl | tflag_ntmpl);
  GWI_BB(gwi_oper_ini(gwi, "File", NULL, NULL))
  GWI_BB(gwi_oper_add(gwi, opck_file_scan))
  GWI_BB(gwi_oper_end(gwi, "@scan", NULL))
  GWI_BB(gwi_oper_ini(gwi, NULL, "File", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_file_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  return GW_OK;
}
