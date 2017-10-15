#include <string.h>
#include "defs.h"
#include "type.h"
#include "lang.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"
#include "sporth.h"

typedef struct sporthData {
  m_float var;
  int parsed;
  sp_data *sp;
  plumber_data pd;
  SPFLOAT in;
} sporthData;

struct Type_ t_gworth = { "Sporth", SZ_INT, &t_ugen };

static int sporth_chuck_in(sporth_stack *stack, void *ud)
{
  plumber_data *pd = (plumber_data *) ud;
  sporthData * data = (sporthData *) pd->ud;
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
      fprintf(stderr, "Unknown mode!\n");
      break;
  }
  return PLUMBER_OK;
}

TICK(sporth_tick)
{
  sporthData * data = u->ug;
  data->in= u->in;
  plumber_compute(&data->pd, PLUMBER_COMPUTE);
  u->out = sporth_stack_pop_float(&data->pd.sporth.stack);
  return 1;
}
CTOR(sporth_ctor)
{
  sporthData * data = malloc(sizeof(sporthData));
  data->parsed = 0;
  data->in = 0;
  data->sp = shred->vm_ref->sp;
  plumber_register(&data->pd);
  data->pd.sporth.flist[SPORTH_IN - SPORTH_FOFFSET].func = sporth_chuck_in;
  plumber_init(&data->pd);
  data->pd.sp = data->sp;
  data->pd.ud = data;
  assign_ugen(UGEN(o), 1, 1, 0, data);
  UGEN(o)->tick = sporth_tick;
}

DTOR(sporth_dtor)
{
  sporthData * data = (sporthData*)UGEN(o)->ug;
  /*
     if(data)
     {
plumber_clean(&data->pd);
sp_destroy(&data->sp);
free(data);
}
*/
  }

MFUN(sporth_setp)
{
  sporthData * data = (sporthData*)UGEN(o)->ug;
  int i = *(m_uint*)MEM(SZ_INT);
  m_float val = *(m_float*)MEM(SZ_INT*2);
  data->pd.p[i] = val;
  *(m_float*)RETURN = val;
}

MFUN(sporth_set_table)
{
  sporthData * data = (sporthData*)UGEN(o)->ug;
  int i = *(m_uint*)MEM(SZ_INT);
  m_float val = *(m_float*)MEM(SZ_INT*2);
  const char * cstr = STRING(*(M_Object*)MEM(SZ_INT*2 + SZ_FLOAT));
  char* ftname = strndup(cstr, (strlen(cstr) + 1));

  int err = 0;
  sp_ftbl *ft;

  if(plumber_ftmap_search(&data->pd, ftname, &ft) == PLUMBER_NOTOK) {
    fprintf(stderr, "Could not find ftable %s", ftname);
    err++;
  }
  if(i > ft->size) {
    fprintf(stderr, "Stack overflow! %d exceeds table size %d\n", i, ft->size);
    err++;
  }
  if(!err) {
    ft->tbl[i] = val;
  }
  free(ftname);
  *(m_float*)RETURN = val;
}

MFUN(sporth_get_table)
{
  sporthData * data = (sporthData*)UGEN(o)->ug;
  int i = *(m_uint*)MEM(SZ_INT);
  sp_ftbl *ft;
  const char * cstr = STRING(*(M_Object*)MEM(SZ_INT));
  char* ftname = strndup(cstr, (strlen(cstr) + 1));
  int err = 0;

  if(plumber_ftmap_search(&data->pd, ftname, &ft) == PLUMBER_NOTOK) {
    fprintf(stderr, "Could not find ftable %s", ftname);
    err++;
  }
  if(i > ft->size) {
    fprintf(stderr, "Stack overflow! %d exceeds table size %d\n", i, ft->size);
    err++;
  }
  free(ftname);
  if(!err) {
    *(m_float*)RETURN = ft->tbl[i];
  } else {
    *(m_float*)RETURN = 0;
  }
}

MFUN(sporth_getp)
{
  sporthData * data = (sporthData*)UGEN(o)->ug;
  int i = *(m_uint*)MEM(SZ_INT);
  *(m_float*)RETURN = data->pd.p[i];
}

MFUN(sporth_parse_string)
{
  sporthData * data = (sporthData*)UGEN(o)->ug;
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
  *(m_float*)RETURN = data->var;
}

MFUN(sporth_parse_file)
{
  sporthData * data = (sporthData*)UGEN(o)->ug;
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
    *(m_float*)RETURN = data->var;
  }
}

IMPORT
{
  CHECK_BB(importer_class_ini(importer, &t_gworth, sporth_ctor, sporth_dtor))

    importer_func_ini(importer, "float", "p", (m_uint)sporth_setp);
  importer_func_arg(importer, "int", "index");
  importer_func_arg(importer, "float", "val");
  CHECK_BB(importer_func_end(importer, ae_flag_member))

    importer_func_ini(importer, "float", "p", (m_uint)sporth_getp);
  importer_func_arg(importer, "int", "index");
  CHECK_BB(importer_func_end(importer, ae_flag_member))

    importer_func_ini(importer, "float", "t", (m_uint)sporth_set_table);
  importer_func_arg(importer, "int", "index");
  importer_func_arg(importer, "float", "val");
  importer_func_arg(importer, "string", "table");
  CHECK_BB(importer_func_end(importer, ae_flag_member))

    importer_func_ini(importer, "float", "t", (m_uint)sporth_get_table);
  importer_func_arg(importer, "int", "index");
  importer_func_arg(importer, "string", "table");
  CHECK_BB(importer_func_end(importer, ae_flag_member))

    importer_func_ini(importer, "string", "parse", (m_uint)sporth_parse_string);
  importer_func_arg(importer, "string", "arg");
  CHECK_BB(importer_func_end(importer, ae_flag_member))

    importer_func_ini(importer, "string", "parsefile", (m_uint)sporth_parse_file);
  importer_func_arg(importer, "string", "arg");
  CHECK_BB(importer_func_end(importer, ae_flag_member))

    CHECK_BB(importer_class_end(importer))
    return 1;
}
