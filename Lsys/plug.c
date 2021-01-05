#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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
#include "vm.h"
#include "ugen.h"

#include "lsys.h"

typedef struct
{
  lsys_d       lsys;
  lsys_list    lst;
  lsys_entry * ent;
  m_uint pos;
  m_bool is_init;
} Lsys;

#define LSYS(o) ((Lsys*)UGEN(o)->module.gen.data)

static int tochar(int c) {
    if(c >= 10) {
        return c + 87;
    } else {
        return c + 48;
    }
}

static TICK(tick) {
  Lsys* ptr = (Lsys*)u->module.gen.data;
  u->out = 0;
  if(!ptr->is_init)
    return;
  if(u->module.gen.trig->out) {
    ptr->pos = lsys_list_iter(&ptr->lst, &ptr->ent, ptr->pos);
    u->out = ptr->ent->val + 1;
  }
}

static CTOR(ctor) {
  Lsys* l = (Lsys*) xmalloc(sizeof(Lsys));
  lsys_init(&l->lsys);
  l->is_init = 0;
  l->pos = 0;
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tick, l, 1);
}

static DTOR(dtor)
{
  Lsys* l = LSYS(o);
  if(l->is_init)
    lsys_list_destroy(&l->lst);
  free(l);
}

static MFUN(gw_lsys_parse)
{
  Lsys*  ptr = LSYS(o);
  m_uint ord = *(m_uint*)MEM(SZ_INT);
  m_str  str = STRING(*(M_Object*)MEM(SZ_INT*2));

  lsys_parse(&ptr->lsys, str, str, strlen(str));
  lsys_list_init(&ptr->lst);
  lsys_make_list(&ptr->lsys, &ptr->lst, str, 0, ord);
  lsys_list_reset(&ptr->lst);

  ptr->is_init = 1;
  ptr->pos     = 0;
}

static MFUN(gw_lsys_reset)
{
  Lsys*  ptr = LSYS(o);
  if(ptr->is_init);
    lsys_list_reset(&ptr->lst);
}

static MFUN(gw_lsys_size)
{
  Lsys*  ptr = LSYS(o);
  *(m_uint*)RETURN = ptr->is_init ? ptr->lst.size : -1;
}

static MFUN(gw_lsys_get) {
  int i;
  Lsys*  ptr   = LSYS(o);
  if(!ptr->is_init)
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  char str[ptr->lst.size];
  for(i = 0; i < ptr->lst.size; i++) {
    ptr->pos = lsys_list_iter(&ptr->lst, &ptr->ent, ptr->pos);
    str[i] = tochar(ptr->ent->val + 1);
  }
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->vm->gwion->mp, shred, str);
}

GWION_IMPORT(lsys) {
  GWI_BB(gwi_class_ini(gwi, "Lsys", NULL))
  gwi_class_xtor(gwi, ctor, dtor);

  GWI_BB(gwi_func_ini(gwi, "void", "parse"))
    GWI_BB(gwi_func_arg(gwi, "int",    "ord"))
    GWI_BB(gwi_func_arg(gwi, "string", "str"))
  GWI_BB(gwi_func_end(gwi, gw_lsys_parse, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "reset"))
  GWI_BB(gwi_func_end(gwi, gw_lsys_reset, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "size"))
  GWI_BB(gwi_func_end(gwi, gw_lsys_size, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "string", "get"))
  GWI_BB(gwi_func_end(gwi, gw_lsys_get, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
