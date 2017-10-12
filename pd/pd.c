#include <libpd/z_libpd.h>
#include "gw_type.h"
#include "instr.h"
#include "import.h"

static struct Type_ t_gwpd = { "PD", SZ_INT, &t_ugen };

static m_int o_pd_file;
static m_bool pd_init;

#define PDFILE(o) (*(void**)(o->data + o_pd_file))

CTOR(pd_ctor) {
  if(!pd_init++) {
    libpd_init();
    libpd_init_audio(vm->n_in , vm->sp->nchan, vm->sp->sr);
    libpd_start_message(256);
    libpd_add_float(1.0);
    libpd_finish_message ("pd" , "dsp");
  }
  PDFILE(o) = NULL;
}

DTOR(pd_dtor) {
  if(PDFILE(o))
    libpd_closefile(PDFILE(o));
}

m_int o_pd_member_data;
m_int o_pd_static_data;
m_int* pd_static_value;

static MFUN(gwpd_open) {
  M_Object f_obj = *(M_Object*)MEM(SZ_INT);
  M_Object d_obj = *(M_Object*)MEM(SZ_INT*2);
  PDFILE(o) = libpd_openfile(STRING(f_obj), STRING(d_obj));
  release(f_obj, shred);
  release(d_obj, shred);
}

static MFUN(gwpd_close) {
  if(PDFILE(o))
    libpd_closefile(PDFILE(o));
}

IMPORT
{
  CHECK_BB(importer_class_ini(importer, &t_gwpd, pd_ctor, pd_dtor))

	importer_item_ini(importer,"int",  "@file");
  CHECK_BB((o_pd_file = importer_item_end(importer, ae_flag_member, NULL)))

    /*pd_static_value = malloc(sizeof(m_int));*/
    /*o_pd_static_data = import_var(env, "int", "static", ae_flag_static, pd_static_value);*/

    /*dl_func_init(&fun, "int", "mfun",  (m_uint)mfun);*/
    /*dl_func_func_arg(&fun, "int", "arg");*/
    /*CHECK_OB(import_fun(env, &fun, ae_flag_member))*/

  importer_func_ini(importer, "int", "open",  (m_uint)gwpd_open);
  importer_func_arg(importer, "string", "basename");
  importer_func_arg(importer, "string", "dirname");
  CHECK_OB(importer_func_end(importer, ae_flag_member))

  importer_func_ini(importer, "int", "close",  (m_uint)gwpd_close);
  CHECK_OB(importer_func_end(importer, ae_flag_member))

  CHECK_BB(importer_class_end(importer))
  return 1;
}
