#include "map.h"
#include "vm.h"
#include "type.h"
#include "dl.h"
#include "err_msg.h"
#include "import.h"
#include "lang.h"
#include "vm.h"
#include "shreduler.h"
#include "ugen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <pthread.h>
#include "fmsynth.h"
static struct Type_ t_fmsynth = {"FMSynth", SZ_INT, &t_ugen };
static m_int o_fmsynth_data, o_fmsynth_name, o_fmsynth_author;
#define SYNTH(o) *(fmsynth_t**)(o->d.data + o_fmsynth_data)
#define NAME(o) *(M_Object*)(o->d.data + o_fmsynth_name)
#define AUTHOR(o) *(M_Object*)(o->d.data + o_fmsynth_author)
#define POLYPHONY 64

static m_bool fmsynth_tick(UGen u)
{
  float left;
  float right;
  fmsynth_render(u->ug, &left, &right, 1);
  u->channel[0]->ugen->out = left;
  u->channel[1]->ugen->out = right;
  u->out = left + right;
}


CTOR(ctor)
{
  NAME(o) = new_String(NULL, "name");
  AUTHOR(o) = new_String(NULL, "author");
  SYNTH(o) = fmsynth_new(shred->vm_ref->bbq->sp->sr, POLYPHONY);
  assign_ugen(o->ugen, 1, 2, 0, SYNTH(o));
  o->ugen->tick = fmsynth_tick;
}
DTOR(dtor)
{
  fmsynth_free(SYNTH(o));
}

MFUN(init)
{
  fmsynth_free(SYNTH(o));
  SYNTH(o) = fmsynth_new(shred->vm_ref->bbq->sp->sr, *(m_uint*)(shred->mem + SZ_INT));
  o->ugen->ug = SYNTH(o);
}
MFUN(parameter)
{
  fmsynth_set_parameter(SYNTH(o),
      *(m_uint*)(shred->mem + SZ_INT), *(m_uint*)(shred->mem + SZ_INT*2), *(m_float*)(shred->mem + SZ_INT*3));
}

MFUN(global_parameter)
{
  printf("%f\n", *(m_float*)(shred->mem + SZ_INT*2));
  fmsynth_set_global_parameter(SYNTH(o),
      *(m_uint*)(shred->mem + SZ_INT), *(m_float*)(shred->mem + SZ_INT*2));
}

MFUN(synth_reset)
{
 fmsynth_reset(SYNTH(o));
}

MFUN(noteon)
{
 fmsynth_note_on(SYNTH(o), *(m_uint*)(shred->mem + SZ_INT), *(m_uint*)(shred->mem + SZ_INT*2));

}

MFUN(noteoff)
{
 fmsynth_note_off(SYNTH(o), *(m_uint*)(shred->mem + SZ_INT));
}

MFUN(sustain)
{
 fmsynth_set_sustain(SYNTH(o), *(m_uint*)(shred->mem + SZ_INT));
}

MFUN(wheel)
{
 fmsynth_set_mod_wheel(SYNTH(o), *(m_uint*)(shred->mem + SZ_INT));
}

MFUN(bend)
{
 fmsynth_set_pitch_bend(SYNTH(o), *(m_uint*)(shred->mem + SZ_INT));
}

MFUN(release_all)
{
 fmsynth_release_all(SYNTH(o));
}

MFUN(load)
{
  fmsynth_free(SYNTH(o));
  SYNTH(o) = fmsynth_new(shred->vm_ref->bbq->sp->sr, POLYPHONY);
  m_str filename = STRING( *(M_Object*)(shred->mem + SZ_INT) );
  struct fmsynth_preset_metadata* metadata = malloc(sizeof(struct fmsynth_preset_metadata));
  FILE* file = fopen(filename, "r");
  if(!file)
  {
    RETURN->d.v_uint = -1;
    return;
  }
  void* buf;
  size_t len = fread(buf,fmsynth_preset_size(), 1, file);
  if(len != 1)
  {
    RETURN->d.v_uint = -1;
    return;
  }
  fclose(file);
  RETURN->d.v_uint = fmsynth_preset_load(SYNTH(o), metadata,
      buf, fmsynth_preset_size());
  free(STRING(NAME(o)));
  free(STRING(AUTHOR(o)));
  STRING(NAME(o)) = strdup(metadata->name);
  STRING(AUTHOR(o)) = strdup(metadata->author);
  free(metadata);
}

MFUN(save)
{
  m_str filename = STRING( *(M_Object*)(shred->mem + SZ_INT) );
  FILE* file = fopen(filename, "w");
  if(!file)
  {
    RETURN->d.v_uint = -1;
    return;
  }
  void* buf;
  struct fmsynth_preset_metadata* metadata = malloc(sizeof(struct fmsynth_preset_metadata));
  memset(metadata, 0, sizeof(metadata));
  strcat(metadata->name, STRING(NAME(o)));
  strcat(metadata->author, STRING(AUTHOR(o)));
  RETURN->d.v_uint = fmsynth_preset_save(SYNTH(o), metadata,
      buf, fmsynth_preset_size());
  fwrite(buf, fmsynth_preset_size(), 1, file);
  size_t len = fwrite(buf,fmsynth_preset_size(), 1, file);
  if(len != 1)
  {
    RETURN->d.v_uint = -1;
    return;
  }
  fclose(file);
  free(metadata);
}
// params
static m_int o_amp, o_pan, o_freq_mod, o_freq_offset,
  o_target0, o_target1, o_target2, o_delay0, o_delay1, o_delay2,
  o_rel, o_mid_point, o_low_fact, o_high_fact, o_velo_sens, o_wheel_sens,
  o_lfo_amp_sens, o_lfo_freq_mod, o_enable,
  o_carriers, o_carrier0,
  o_g_vol, o_g_lfo,
  o_ok, o_busy, o_small, o_nonul, o_format, o_unknown;
/*
static m_uint amp            = FMSYNTH_PARAM_AMP;
static m_uint pan            = FMSYNTH_PARAM_PAN;
static m_uint freq_mod       = FMSYNTH_PARAM_FREQ_MOD;
static m_uint freq_offset    = FMSYNTH_PARAM_FREQ_OFFSET;
static m_uint target0        = FMSYNTH_PARAM_ENVELOPE_TARGET0;
static m_uint target1        = FMSYNTH_PARAM_ENVELOPE_TARGET1;
static m_uint target2        = FMSYNTH_PARAM_ENVELOPE_TARGET2;
static m_uint delay0         = FMSYNTH_PARAM_DELAY0;
static m_uint delay1         = FMSYNTH_PARAM_DELAY1;
static m_uint delay2         = FMSYNTH_PARAM_DELAY2;
static m_uint rel            = FMSYNTH_PARAM_RELEASE_TIME;
static m_uint mid_point      = FMSYNTH_PARAM_KEYBOARD_SCALING_MID_POINT;
static m_uint low_fact       = FMSYNTH_PARAM_KEYBOARD_SCALING_LOW_FACTOR;
static m_uint high_fact      = FMSYNTH_PARAM_KEYBOARD_SCALING_HIGH_FACTOR;
static m_uint velo_sens      = FMSYNTH_PARAM_VELOCITY_SENSITIVITY;
static m_uint wheel_sens     = FMSYNTH_PARAM_MOD_WHEEL_SENSITIVITY;
static m_uint lfo_amp_sens   = FMSYNTH_PARAM_LFO_AMP_SENSITIVITY;
static m_uint lfo_freq_mod   = FMSYNTH_PARAM_LFO_FREQ_MOD_DEPTH;
static m_uint enable         = FMSYNTH_PARAM_ENABLE;
static m_uint carriers       = FMSYNTH_PARAM_CARRIERS;
static m_uint carrier0       = FMSYNTH_PARAM_MOD_TO_CARRIERS0;
// global paramters
static m_int o_g_vol, o_g_lfo;
static m_uint g_vol = FMSYNTH_GLOBAL_PARAM_VOLUME;
static m_uint g_lfo = FMSYNTH_GLOBAL_PARAM_LFO_FREQ;

static m_int o_ok, o_busy, o_small, o_nonul, o_format, o_unknown;
static m_uint ok      = FMSYNTH_STATUS_OK;
static m_uint busy    = FMSYNTH_STATUS_BUSY;
static m_uint small   = FMSYNTH_STATUS_BUFFER_TOO_SMALL;
static m_uint nonul   = FMSYNTH_STATUS_NO_NUL_TERMINATE;
static m_uint format  = FMSYNTH_STATUS_INVALID_FORMAT;
static m_uint unknown = FMSYNTH_STATUS_MESSAGE_UNKNOWN;
*/
IMPORT
{
  DL_Func  fun;
  DL_Value* arg;

  m_uint* amp            = malloc(SZ_INT);
  m_uint* pan            = malloc(SZ_INT);
  m_uint* freq_mod       = malloc(SZ_INT);
  m_uint* freq_offset    = malloc(SZ_INT);
  m_uint* target0        = malloc(SZ_INT);
  m_uint* target1        = malloc(SZ_INT);
  m_uint* target2        = malloc(SZ_INT);
  m_uint* delay0         = malloc(SZ_INT);
  m_uint* delay1         = malloc(SZ_INT);
  m_uint* delay2         = malloc(SZ_INT);
  m_uint* rel            = malloc(SZ_INT);
  m_uint* mid_point      = malloc(SZ_INT);
  m_uint* low_fact       = malloc(SZ_INT);
  m_uint* high_fact      = malloc(SZ_INT);
  m_uint* velo_sens      = malloc(SZ_INT);
  m_uint* wheel_sens     = malloc(SZ_INT);
  m_uint* lfo_amp_sens   = malloc(SZ_INT);
  m_uint* lfo_freq_mod   = malloc(SZ_INT);
  m_uint* enable         = malloc(SZ_INT);
  m_uint* carriers       = malloc(SZ_INT);
  m_uint* carrier0       = malloc(SZ_INT);
  m_uint* g_vol          = malloc(SZ_INT);
  m_uint* g_lfo          = malloc(SZ_INT);
  m_uint* ok             = malloc(SZ_INT);
  m_uint* busy           = malloc(SZ_INT);
  m_uint* small          = malloc(SZ_INT);
  m_uint* nonul          = malloc(SZ_INT);
  m_uint* format         = malloc(SZ_INT);
  m_uint* unknown        = malloc(SZ_INT);

  *amp            = FMSYNTH_PARAM_AMP;
  *pan            = FMSYNTH_PARAM_PAN;
  *freq_mod       = FMSYNTH_PARAM_FREQ_MOD;
  *freq_offset    = FMSYNTH_PARAM_FREQ_OFFSET;
  *target0        = FMSYNTH_PARAM_ENVELOPE_TARGET0;
  *target1        = FMSYNTH_PARAM_ENVELOPE_TARGET1;
  *target2        = FMSYNTH_PARAM_ENVELOPE_TARGET2;
  *delay0         = FMSYNTH_PARAM_DELAY0;
  *delay1         = FMSYNTH_PARAM_DELAY1;
  *delay2         = FMSYNTH_PARAM_DELAY2;
  *rel            = FMSYNTH_PARAM_RELEASE_TIME;
  *mid_point      = FMSYNTH_PARAM_KEYBOARD_SCALING_MID_POINT;
  *low_fact       = FMSYNTH_PARAM_KEYBOARD_SCALING_LOW_FACTOR;
  *high_fact      = FMSYNTH_PARAM_KEYBOARD_SCALING_HIGH_FACTOR;
  *velo_sens      = FMSYNTH_PARAM_VELOCITY_SENSITIVITY;
  *wheel_sens     = FMSYNTH_PARAM_MOD_WHEEL_SENSITIVITY;
  *lfo_amp_sens   = FMSYNTH_PARAM_LFO_AMP_SENSITIVITY;
  *lfo_freq_mod   = FMSYNTH_PARAM_LFO_FREQ_MOD_DEPTH;
  *enable         = FMSYNTH_PARAM_ENABLE;
  *carriers       = FMSYNTH_PARAM_CARRIERS;
  *carrier0       = FMSYNTH_PARAM_MOD_TO_CARRIERS0;
  *g_vol          = FMSYNTH_GLOBAL_PARAM_VOLUME;
  *g_lfo          = FMSYNTH_GLOBAL_PARAM_LFO_FREQ;
  *ok             = FMSYNTH_STATUS_OK;
  *busy           = FMSYNTH_STATUS_BUSY;
  *small          = FMSYNTH_STATUS_BUFFER_TOO_SMALL;
  *nonul          = FMSYNTH_STATUS_NO_NUL_TERMINATE;
  *format         = FMSYNTH_STATUS_INVALID_FORMAT;
  *unknown        = FMSYNTH_STATUS_MESSAGE_UNKNOWN;

  CHECK_BB(import_class_begin(env, &t_fmsynth, env->global_nspc, ctor, dtor))
  o_fmsynth_data = import_mvar(env, "int", "&synth",   0, 0);
  CHECK_BB(o_fmsynth_data)
  o_fmsynth_name = import_mvar(env, "string", "name",   0, 0);
  CHECK_BB(o_fmsynth_name)
  o_fmsynth_author = import_mvar(env, "string", "author",   0, 0);
  CHECK_BB(o_fmsynth_author)

  // params
  o_amp = import_svar(env, "int", "AMP",      1, 0, amp);
  CHECK_BB(o_pan)
  o_pan = import_svar(env, "int", "PAN",      1, 0, pan);
  CHECK_BB(o_pan)

  o_freq_mod = import_svar(env, "int", "FREQ_MOD",      1, 0, freq_mod);
  CHECK_BB(o_freq_offset)
  o_freq_offset = import_svar(env, "int", "FREQ_OFFSET",      1, 0, freq_offset);
  CHECK_BB(o_freq_mod)
  o_target0 = import_svar(env, "int", "TARGET0",      1, 0, target0);
  CHECK_BB(o_target0)
  o_target1 = import_svar(env, "int", "TARGET1",      1, 0, target1);
  CHECK_BB(o_target1)
  o_target2 = import_svar(env, "int", "TARGET2",      1, 0, target2);
  CHECK_BB(o_target2)
  o_delay0 = import_svar(env, "int", "DELAY0",      1, 0, delay0);
  CHECK_BB(o_delay0)
  o_delay1 = import_svar(env, "int", "DELAY1",      1, 0, delay1);
  CHECK_BB(o_delay0)
  o_delay2 = import_svar(env, "int", "DELAY2",      1, 0, delay2);
  CHECK_BB(o_delay2)
  o_rel = import_svar(env, "int", "RELEASE",      1, 0, rel);
  CHECK_BB(o_rel)
  o_mid_point = import_svar(env, "int", "MID_POINT",      1, 0, mid_point);
  CHECK_BB(o_mid_point)
  o_low_fact = import_svar(env, "int", "LOW_FACTOR",      1, 0, low_fact);
  CHECK_BB(o_low_fact)
  o_high_fact = import_svar(env, "int", "HIGH_FACTOR",      1, 0, high_fact);
  CHECK_BB(o_high_fact)
  o_velo_sens = import_svar(env, "int", "VELO_SENS",      1, 0, velo_sens);
  CHECK_BB(o_velo_sens)
  o_wheel_sens = import_svar(env, "int", "WHEEL_SENS",      1, 0, wheel_sens);
  CHECK_BB(o_wheel_sens)
  o_lfo_amp_sens = import_svar(env, "int", "LFO_AMP_SENS",      1, 0, lfo_amp_sens);
  CHECK_BB(o_lfo_amp_sens)
  o_lfo_freq_mod = import_svar(env, "int", "LFO_FREQ_MOD",      1, 0, lfo_freq_mod);
  CHECK_BB(o_lfo_freq_mod)
  o_enable = import_svar(env, "int", "ENABLE",      1, 0, enable);
  CHECK_BB(o_enable)
  o_carriers = import_svar(env, "int", "CARRIERS",      1, 0, carriers);
  CHECK_BB(o_carriers)
  o_carrier0 = import_svar(env, "int", "CARRIER0",      1, 0, carrier0);
  CHECK_BB(o_carrier0)

  // global
  o_g_vol    = import_svar(env, "int", "GVOL", 1, 0, g_vol);
  CHECK_BB(o_g_vol)

  o_g_lfo    = import_svar(env, "int", "GLFO",      1, 0, g_lfo);
  CHECK_BB(o_g_lfo)
  // status
  o_ok    = import_svar(env, "int", "OK",      1, 0, ok);
  CHECK_BB(o_ok)
  o_busy   = import_svar(env, "int", "BUSY",   1, 0, busy);
  CHECK_BB(o_busy)
  o_small  = import_svar(env, "int", "SMALL",   1, 0, small);
  CHECK_BB(o_small)
  o_nonul  = import_svar(env, "int", "NONUL",   1, 0, nonul);
  CHECK_BB(o_nonul)
  o_format = import_svar(env, "int", "FORMAT",   1, 0, format);
  CHECK_BB(o_format)
  o_unknown = import_svar(env, "int", "UNKNOWN",   1, 0, unknown);
  CHECK_BB(o_unknown)

  dl_func_init(&fun, "void", "init", (m_uint)init);
    dl_func_add_arg(&fun, "int", "plyphony");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "parameter", (m_uint)parameter);
    dl_func_add_arg(&fun, "int", "parameter");
    dl_func_add_arg(&fun, "int", "operator_index");
    dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "parameter", (m_uint)global_parameter);
    dl_func_add_arg(&fun, "int", "parameter");
    dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "noteon", (m_uint)noteon);
    dl_func_add_arg(&fun, "int", "note");
    dl_func_add_arg(&fun, "int", "velocity");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "reset", (m_uint)synth_reset);
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "noteoff", (m_uint)noteoff);
    dl_func_add_arg(&fun, "int", "note");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "sustain", (m_uint)sustain);
    dl_func_add_arg(&fun, "int", "enable");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "wheel", (m_uint)wheel);
    dl_func_add_arg(&fun, "int", "value");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "bend", (m_uint)bend);
    dl_func_add_arg(&fun, "int", "value");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "release", (m_uint)release_all);
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "int", "load", (m_uint)load);
    dl_func_add_arg(&fun, "string", "filename");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "int", "save", (m_uint)save);
    dl_func_add_arg(&fun, "string", "filename");
  CHECK_BB(import_mfun(env, &fun))
  CHECK_BB(import_class_end(env))
  return 1;
}

// MISS: reset
