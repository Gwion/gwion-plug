#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"
#include "sporth.h"

typedef struct sporthData {
  sp_data *sp;
  plumber_data pd;
  SPFLOAT in;
  m_float var;
  m_bool parsed;
} sporthData;

static int sporth_chuck_in(sporth_stack *stack, void *ud) {
  plumber_data *pd = (plumber_data *) ud;
  const sporthData * data = (sporthData *) pd->ud;
  switch(pd->mode) {
    case PLUMBER_CREATE:
      plumber_add_ugen(pd, SPORTH_IN, NULL);
      sporth_stack_push_float(stack, 0);
      break;

    case PLUMBER_INIT:
      sporth_stack_push_float(stack, 0);
      break;

    case PLUMBER_COMPUTE:
      sporth_stack_push_float(stack, data->in);
      break;

    case PLUMBER_DESTROY:
      break;

    default:
      gw_err("Unknown mode!\n");
      break;
  }
  return PLUMBER_OK;
}

static TICK(gwsporth_tick) {
  sporthData * data = u->module.gen.data;
  data->in= u->in;
  plumber_compute(&data->pd, PLUMBER_COMPUTE);
  u->out = sporth_stack_pop_float(&data->pd.sporth.stack);
}

static CTOR(sporth_ctor) {
  sporthData * data = (sporthData*)xmalloc(sizeof(sporthData));
  data->parsed = 0;
  data->in = 0;
  data->sp = shred->vm_ref->sp;
  plumber_register(&data->pd);
  data->pd.sporth.flist[SPORTH_IN - SPORTH_FOFFSET].func = sporth_chuck_in;
  plumber_init(&data->pd);
  data->pd.sp = data->sp;
  data->pd.ud = data;
  ugen_ini(UGEN(o), 1, 1);
  ugen_gen(UGEN(o), gwsporth_tick, data, 0);
}

static DTOR(sporth_dtor) {
  sporthData * data = (sporthData*)UGEN(o)->module.gen.data;
  if(data) {
    plumber_clean(&data->pd);
    free(data);
  }
}

static MFUN(sporth_setp) {
  sporthData * data = (sporthData*)UGEN(o)->module.gen.data;
  const m_uint i = *(m_uint*)MEM(SZ_INT);
  m_float val = *(m_float*)MEM(SZ_INT*2);
  data->pd.p[i] = val;
  *(m_float*)RETURN = val;
}

static MFUN(sporth_set_table) {
  sporthData * data = (sporthData*)UGEN(o)->module.gen.data;
  const m_uint i = *(m_uint*)MEM(SZ_INT);
  const m_float val = *(m_float*)MEM(SZ_INT*2);
  const char * cstr = STRING(*(M_Object*)MEM(SZ_INT*2 + SZ_FLOAT));
  char* ftname = strndup(cstr, (strlen(cstr) + 1));

  int err = 0;
  sp_ftbl *ft;

  if(plumber_ftmap_search(&data->pd, ftname, &ft) == PLUMBER_NOTOK) {
    gw_err("Could not find ftable %s", ftname);
    err++;
  }
  if(i > ft->size) {
    gw_err("Stack overflow! %d exceeds table size %d\n", i, ft->size);
    err++;
  }
  if(!err) {
    ft->tbl[i] = val;
  }
  free(ftname);
  *(m_float*)RETURN = val;
}

MFUN(sporth_get_table) {
  sporthData * data = (sporthData*)UGEN(o)->module.gen.data;
  const m_uint i = *(m_uint*)MEM(SZ_INT);
  sp_ftbl *ft;
  const char * cstr = STRING(*(M_Object*)MEM(SZ_INT));
  char* ftname = strndup(cstr, (strlen(cstr) + 1));
  int err = 0;

  if(plumber_ftmap_search(&data->pd, ftname, &ft) == PLUMBER_NOTOK) {
    gw_err("Could not find ftable %s", ftname);
    ++err;
  }
  if(i > ft->size) {
    gw_err("Stack overflow! %d exceeds table size %d\n", i, ft->size);
    ++err;
  }
  free(ftname);
  if(!err)
    *(m_float*)RETURN = ft->tbl[i];
  else
    *(m_float*)RETURN = 0;
}

static MFUN(sporth_getp) {
  const sporthData * data = (sporthData*)UGEN(o)->module.gen.data;
  const int i = *(m_uint*)MEM(SZ_INT);
  *(m_float*)RETURN = data->pd.p[i];
}

static MFUN(sporth_parse_string)
{
  sporthData * data = (sporthData*)UGEN(o)->module.gen.data;
  const char * cstr = STRING(*(M_Object*)MEM(SZ_INT));
  char* str = strndup(cstr, (strlen(cstr) + 1));
  if(!data->parsed) {
    data->parsed = 1;
    plumber_parse_string(&data->pd, str);
    plumber_compute(&data->pd, PLUMBER_INIT);
  } else {
    plumber_recompile_string(&data->pd, str);
  }
  free(str);
  *(M_Object*)RETURN = *(M_Object*)MEM(SZ_INT);
  release(*(M_Object*)RETURN, shred);
}

static MFUN(sporth_parse_file) {
  sporthData * data = (sporthData*)UGEN(o)->module.gen.data;
  const char * cstr = STRING(*(M_Object*)MEM(SZ_INT));
  char* str = strndup(cstr, (strlen(cstr) + 1));

  if(plumber_open_file(&data->pd, str) == PLUMBER_OK) {
    if(!data->parsed) {
      data->parsed = 1;
      plumber_parse(&data->pd);
      plumber_compute(&data->pd, PLUMBER_INIT);
    } else {
      plumber_recompile(&data->pd);
    }
    plumber_close_file(&data->pd);
    free(str);
    *(M_Object*)RETURN = *(M_Object*)MEM(SZ_INT);
    release(*(M_Object*)RETURN, shred);
  }
}

IMPORT {
  Type t_gworth;
  CHECK_BB((t_gworth = gwi_mk_type(gwi, "Sporth", SZ_INT, t_ugen)))

  CHECK_BB(gwi_class_ini(gwi, t_gworth, sporth_ctor, sporth_dtor))

  CHECK_BB(gwi_func_ini(gwi, "float", "p", sporth_setp))
  CHECK_BB(gwi_func_arg(gwi, "int", "index"))
  CHECK_BB(gwi_func_arg(gwi, "float", "val"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "float", "p", sporth_getp))
  CHECK_BB(gwi_func_arg(gwi, "int", "index"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "float", "t", sporth_set_table))
  CHECK_BB(gwi_func_arg(gwi, "int", "index"))
  CHECK_BB(gwi_func_arg(gwi, "float", "val"))
  CHECK_BB(gwi_func_arg(gwi, "string", "table"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "float", "t", sporth_get_table))
  CHECK_BB(gwi_func_arg(gwi, "int", "index"))
  CHECK_BB(gwi_func_arg(gwi, "string", "table"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "string", "parse", sporth_parse_string))
  CHECK_BB(gwi_func_arg(gwi, "string", "arg"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "string", "parsefile", sporth_parse_file))
  CHECK_BB(gwi_func_arg(gwi, "string", "arg"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
