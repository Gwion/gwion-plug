#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <pthread.h>
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
#include "gwi.h"
#include "gwion.h"
#include "fmsynth.h"

#define SYNTH(o) UGEN(o)->module.gen.data
#define NAME(o) *(M_Object*)(o->data + SZ_INT)
#define AUTHOR(o) *(M_Object*)(o->data + SZ_INT*2)
#define POLYPHONY 64

static TICK(fmsynth_tick) {
  float left = 0.;
  float right = 0.;
  fmsynth_render(u->module.gen.data, &left, &right, 1);
  UGEN(u->connect.multi->channel[0])->out = left;
  UGEN(u->connect.multi->channel[1])->out = right;
  u->out = (m_float)((left + right) / 2);
}

static DTOR(dtor) {
  fmsynth_free(SYNTH(o));
}

static MFUN(parameter) {
  fmsynth_set_parameter(SYNTH(o),
      *(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2), *(m_float*)MEM(SZ_INT*3));
}

static MFUN(global_parameter) {
  fmsynth_set_global_parameter(SYNTH(o),
      *(m_uint*)MEM(SZ_INT), *(m_float*)MEM(SZ_INT*2));
}

static MFUN(synth_reset) {
 fmsynth_reset(SYNTH(o));
}

static MFUN(noteon) {
 fmsynth_note_on(SYNTH(o), *(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2));
}

static MFUN(noteoff) {
 fmsynth_note_off(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

static MFUN(sustain) {
 fmsynth_set_sustain(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

static MFUN(wheel) {
 fmsynth_set_mod_wheel(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

static MFUN(bend) {
 fmsynth_set_pitch_bend(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

static MFUN(release_all) {
 fmsynth_release_all(SYNTH(o));
}

static MFUN(load) {
  const m_uint size = fmsynth_preset_size();
  char* buf[size];
  struct fmsynth_preset_metadata metadata;
  const m_str filename = STRING(*(M_Object*)MEM(SZ_INT));
  FILE* file = fopen(filename, "r");

  if(!file) {
    *(m_uint*)RETURN = -1;
    return;
  }
  size_t len = fread(buf, size, 1, file);
  if(len != 1) {
    *(m_uint*)RETURN = -1;
    return;
  }
  fclose(file);
  SYNTH(o) = fmsynth_new(shred->info->vm->bbq->si->sr, POLYPHONY);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fmsynth_tick, SYNTH(o), 0);
  *(m_uint*)RETURN = fmsynth_preset_load(SYNTH(o), &metadata,
      buf, fmsynth_preset_size());
  NAME(o) = new_string(shred->info->vm->gwion, metadata.name);
  AUTHOR(o) = new_string(shred->info->vm->gwion, metadata.author);
  STRING(NAME(o)) = metadata.name;
  STRING(AUTHOR(o)) = metadata.author;
}

static MFUN(save) {
  const m_str filename = STRING( *(M_Object*)MEM(SZ_INT) );
  FILE* file = fopen(filename, "w");
  if(!file)
  {
    *(m_uint*)RETURN = -1;
    return;
  }
  char buf[1024];
  struct fmsynth_preset_metadata metadata = {};
  strcpy(metadata.name, STRING(NAME(o)));
  strcpy(metadata.author, STRING(AUTHOR(o)));
  *(m_uint*)RETURN = fmsynth_preset_save(SYNTH(o), &metadata,
      buf, fmsynth_preset_size());
  fwrite(buf, fmsynth_preset_size(), 1, file);
  size_t len = fwrite(buf,fmsynth_preset_size(), 1, file);
  if(len != 1) {
    *(m_uint*)RETURN = -1;
    return;
  }
  fclose(file);
}

GWION_IMPORT(FMSynth) {
  DECL_B(const Type, t_fmsynth, = gwi_class_ini(gwi, "FMSynth", "UGen"));
  gwi_class_xtor(gwi, NULL, dtor);
  t_fmsynth->nspc->offset += SZ_INT*2;
  gwi_item_ini(gwi,"string", "name");
  gwi_item_end(gwi, ae_flag_none, num, 0);
  gwi_item_ini(gwi,"string", "author");
  gwi_item_end(gwi, ae_flag_none, num, 0);

  // params
  gwi_item_ini(gwi,"int", "AMP");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_AMP);
  gwi_item_ini(gwi,"int", "PAN");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_PAN);

  gwi_item_ini(gwi,"int", "FREQ_MOD");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_FREQ_MOD);
  gwi_item_ini(gwi,"int", "FREQ_OFFSET");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_FREQ_OFFSET);
  gwi_item_ini(gwi,"int", "TARGET0");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_ENVELOPE_TARGET0);
  gwi_item_ini(gwi,"int", "TARGET1");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_ENVELOPE_TARGET1);
  gwi_item_ini(gwi,"int", "TARGET2");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_ENVELOPE_TARGET2);
  gwi_item_ini(gwi,"int", "DELAY0");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_DELAY0);
  gwi_item_ini(gwi,"int", "DELAY1");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_DELAY1);
  gwi_item_ini(gwi,"int", "DELAY2");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_DELAY2);
  gwi_item_ini(gwi,"int", "RELEASE");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_RELEASE_TIME);
  gwi_item_ini(gwi,"int", "MID_POINT");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_KEYBOARD_SCALING_MID_POINT);
  gwi_item_ini(gwi,"int", "LOW_FACTOR");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_KEYBOARD_SCALING_LOW_FACTOR);
  gwi_item_ini(gwi,"int", "HIGH_FACTOR");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_KEYBOARD_SCALING_HIGH_FACTOR);
  gwi_item_ini(gwi,"int", "VELO_SENS");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_VELOCITY_SENSITIVITY);
  gwi_item_ini(gwi,"int", "WHEEL_SENS");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_MOD_WHEEL_SENSITIVITY);
  gwi_item_ini(gwi,"int", "LFO_AMP_SENS");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_LFO_AMP_SENSITIVITY);
  gwi_item_ini(gwi,"int", "LFO_FREQ_MOD");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_LFO_FREQ_MOD_DEPTH);
  gwi_item_ini(gwi,"int", "ENABLE");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_ENABLE);
  gwi_item_ini(gwi,"int", "CARRIERS");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_CARRIERS);
  gwi_item_ini(gwi,"int", "CARRIER0");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_PARAM_MOD_TO_CARRIERS0);

  // global
  gwi_item_ini(gwi,"int", "GVOL");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_GLOBAL_PARAM_VOLUME);

  gwi_item_ini(gwi,"int", "GLFO");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_GLOBAL_PARAM_LFO_FREQ);
  // status
  gwi_item_ini(gwi,"int", "OK");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_STATUS_OK);
  gwi_item_ini(gwi,"int", "BUSY");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_STATUS_BUSY);
  gwi_item_ini(gwi,"int", "SMALL");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_STATUS_BUFFER_TOO_SMALL);
  gwi_item_ini(gwi,"int", "NONUL");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_STATUS_NO_NUL_TERMINATE);
  gwi_item_ini(gwi,"int", "FORMAT");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_STATUS_INVALID_FORMAT);
  gwi_item_ini(gwi,"int", "UNKNOWN");
  gwi_item_end(gwi, ae_flag_static | ae_flag_const, num, FMSYNTH_STATUS_MESSAGE_UNKNOWN);

  gwi_func_ini(gwi, "void", "parameter");
    gwi_func_arg(gwi, "int", "parameter");
    gwi_func_arg(gwi, "int", "operator_index");
    gwi_func_arg(gwi, "float", "value");
  GWI_B(gwi_func_end(gwi, parameter, ae_flag_none));
  gwi_func_ini(gwi, "void", "parameter");
    gwi_func_arg(gwi, "int", "parameter");
    gwi_func_arg(gwi, "float", "value");
  GWI_B(gwi_func_end(gwi, global_parameter, ae_flag_none));
  gwi_func_ini(gwi, "void", "noteon");
    gwi_func_arg(gwi, "int", "note");
    gwi_func_arg(gwi, "int", "velocity");
  GWI_B(gwi_func_end(gwi, noteon, ae_flag_none));
  gwi_func_ini(gwi, "void", "reset");
  GWI_B(gwi_func_end(gwi, synth_reset, ae_flag_none));
  gwi_func_ini(gwi, "void", "noteoff");
    gwi_func_arg(gwi, "int", "note");
  GWI_B(gwi_func_end(gwi, noteoff, ae_flag_none));
  gwi_func_ini(gwi, "void", "sustain");
    gwi_func_arg(gwi, "int", "enable");
  GWI_B(gwi_func_end(gwi, sustain, ae_flag_none));
  gwi_func_ini(gwi, "void", "wheel");
    gwi_func_arg(gwi, "int", "value");
  GWI_B(gwi_func_end(gwi, wheel, ae_flag_none));
  gwi_func_ini(gwi, "void", "bend");
    gwi_func_arg(gwi, "int", "value");
  GWI_B(gwi_func_end(gwi, bend, ae_flag_none));
  gwi_func_ini(gwi, "void", "release");
  GWI_B(gwi_func_end(gwi, release_all, ae_flag_none));
  gwi_func_ini(gwi, "auto", "new");
    gwi_func_arg(gwi, "string", "filename");
  GWI_B(gwi_func_end(gwi, load, ae_flag_none));
  gwi_func_ini(gwi, "int", "save");
    gwi_func_arg(gwi, "string", "filename");
  GWI_B(gwi_func_end(gwi, save, ae_flag_none));
  GWI_B(gwi_class_end(gwi));
  return true;
}

// MISS: reset
// metadata can be heap
