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

typedef struct {
  lsys_d       lsys;
  lsys_list    lst;
  lsys_entry * ent;
  m_uint pos;
} Lsys;

#define LSYS(o) ((Lsys*)UGEN(o)->module.gen.data)

static int tochar(int c) {
  if(c >= 10) return c + 87;
  else return c + 48;
}

static TICK(tick) {
  Lsys* ptr = (Lsys*)u->module.gen.data;
  u->out = 0;
  if(u->module.gen.trig->out) {
    ptr->pos = lsys_list_iter(&ptr->lst, &ptr->ent, ptr->pos);
    u->out = ptr->ent->val + 1;
  }
}

static DTOR(dtor) {
  Lsys* l = LSYS(o);
  lsys_list_destroy(&l->lst);
  mp_free2(shred->info->mp, sizeof(Lsys), l);
}

static MFUN(gw_lsys_parse) {
  Lsys* l = (Lsys*) mp_malloc2(shred->info->mp, sizeof(Lsys));
  lsys_init(&l->lsys);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), tick, l, 1);

  m_uint ord = *(m_uint*)MEM(SZ_INT);
  m_str  str = STRING(*(M_Object*)MEM(SZ_INT*2));

  lsys_parse(&l->lsys, str, str, strlen(str));
  lsys_list_init(&l->lst);
  lsys_make_list(&l->lsys, &l->lst, str, 0, ord);
  lsys_list_reset(&l->lst);
  l->pos     = 0;
  *(M_Object*) RETURN = o;
}

static MFUN(gw_lsys_reset) {
  Lsys*  ptr = LSYS(o);
  lsys_list_reset(&ptr->lst);
}

static MFUN(gw_lsys_size) {
  Lsys*  ptr = LSYS(o);
  *(m_int*)RETURN = ptr->lst.size;
}

static MFUN(gw_lsys_get) {
  Lsys*  ptr   = LSYS(o);
  char str[ptr->lst.size + 1];
  for(uint i = 0; i < ptr->lst.size; i++) {
    ptr->pos = lsys_list_iter(&ptr->lst, &ptr->ent, ptr->pos);
    str[i] = tochar(ptr->ent->val + 1);
  }
  str[ptr->lst.size] = '\0';
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->vm->gwion, str);
}

GWION_IMPORT(lsys) {
  GWI_OB(gwi_class_ini(gwi, "Lsys", "UGen"))
  gwi_class_xtor(gwi, NULL, dtor);

  GWI_BB(gwi_func_ini(gwi, "auto", "new"))
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
