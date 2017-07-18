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
#define SYNTH(o) *(fmsynth_t**)(o->data + o_fmsynth_data)
#define NAME(o) *(M_Object*)(o->data + o_fmsynth_name)
#define AUTHOR(o) *(M_Object*)(o->data + o_fmsynth_author)
#define POLYPHONY 64

static m_bool fmsynth_tick(UGen u)
{
  float left;
  float right;
  fmsynth_render(u->ug, &left, &right, 1);
  UGEN(u->channel[0])->out = left;
  UGEN(u->channel[1])->out = right;
  u->out = left + right;
}


CTOR(ctor)
{
  NAME(o) = new_String(NULL, "name");
  AUTHOR(o) = new_String(NULL, "author");
  SYNTH(o) = fmsynth_new(shred->vm_ref->bbq->sp->sr, POLYPHONY);
  assign_ugen(UGEN(o), 1, 2, 0, SYNTH(o));
  UGEN(o)->tick = fmsynth_tick;
}
DTOR(dtor)
{
  fmsynth_free(SYNTH(o));
}

MFUN(init)
{
  fmsynth_free(SYNTH(o));
  SYNTH(o) = fmsynth_new(shred->vm_ref->bbq->sp->sr, *(m_uint*)MEM(SZ_INT));
  UGEN(o)->ug = SYNTH(o);
}
MFUN(parameter)
{
  fmsynth_set_parameter(SYNTH(o),
      *(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2), *(m_float*)MEM(SZ_INT*3));
}

MFUN(global_parameter)
{
  printf("%f\n", *(m_float*)MEM(SZ_INT*2));
  fmsynth_set_global_parameter(SYNTH(o),
      *(m_uint*)MEM(SZ_INT), *(m_float*)MEM(SZ_INT*2));
}

MFUN(synth_reset)
{
 fmsynth_reset(SYNTH(o));
}

MFUN(noteon)
{
 fmsynth_note_on(SYNTH(o), *(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2));

}

MFUN(noteoff)
{
 fmsynth_note_off(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

MFUN(sustain)
{
 fmsynth_set_sustain(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

MFUN(wheel)
{
 fmsynth_set_mod_wheel(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

MFUN(bend)
{
 fmsynth_set_pitch_bend(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

MFUN(release_all)
{
 fmsynth_release_all(SYNTH(o));
}

MFUN(load)
{
  fmsynth_free(SYNTH(o));
  SYNTH(o) = fmsynth_new(shred->vm_ref->bbq->sp->sr, POLYPHONY);
  m_str filename = STRING( *(M_Object*)MEM(SZ_INT) );
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
  m_str filename = STRING( *(M_Object*)MEM(SZ_INT) );
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
  o_fmsynth_data = import_var(env, "int", "&synth", ae_flag_member, NULL);
  CHECK_BB(o_fmsynth_data)
  o_fmsynth_name = import_var(env, "string", "name", ae_flag_member, NULL);
  CHECK_BB(o_fmsynth_name)
  o_fmsynth_author = import_var(env, "string", "author", ae_flag_member, NULL);
  CHECK_BB(o_fmsynth_author)

  // params
  o_amp = import_var(env, "int", "AMP",      ae_flag_static | ae_flag_const, amp);
  CHECK_BB(o_pan)
  o_pan = import_var(env, "int", "PAN",      ae_flag_static | ae_flag_const, pan);
  CHECK_BB(o_pan)

  o_freq_mod = import_var(env, "int", "FREQ_MOD",      ae_flag_static | ae_flag_const, freq_mod);
  CHECK_BB(o_freq_offset)
  o_freq_offset = import_var(env, "int", "FREQ_OFFSET",      ae_flag_static | ae_flag_const, freq_offset);
  CHECK_BB(o_freq_mod)
  o_target0 = import_var(env, "int", "TARGET0",      ae_flag_static | ae_flag_const, target0);
  CHECK_BB(o_target0)
  o_target1 = import_var(env, "int", "TARGET1",      ae_flag_static | ae_flag_const, target1);
  CHECK_BB(o_target1)
  o_target2 = import_var(env, "int", "TARGET2",      ae_flag_static | ae_flag_const, target2);
  CHECK_BB(o_target2)
  o_delay0 = import_var(env, "int", "DELAY0",      ae_flag_static | ae_flag_const, delay0);
  CHECK_BB(o_delay0)
  o_delay1 = import_var(env, "int", "DELAY1",      ae_flag_static | ae_flag_const, delay1);
  CHECK_BB(o_delay0)
  o_delay2 = import_var(env, "int", "DELAY2",      ae_flag_static | ae_flag_const, delay2);
  CHECK_BB(o_delay2)
  o_rel = import_var(env, "int", "RELEASE",      ae_flag_static | ae_flag_const, rel);
  CHECK_BB(o_rel)
  o_mid_point = import_var(env, "int", "MID_POINT",      ae_flag_static | ae_flag_const, mid_point);
  CHECK_BB(o_mid_point)
  o_low_fact = import_var(env, "int", "LOW_FACTOR",      ae_flag_static | ae_flag_const, low_fact);
  CHECK_BB(o_low_fact)
  o_high_fact = import_var(env, "int", "HIGH_FACTOR",      ae_flag_static | ae_flag_const, high_fact);
  CHECK_BB(o_high_fact)
  o_velo_sens = import_var(env, "int", "VELO_SENS",      ae_flag_static | ae_flag_const, velo_sens);
  CHECK_BB(o_velo_sens)
  o_wheel_sens = import_var(env, "int", "WHEEL_SENS",      ae_flag_static | ae_flag_const, wheel_sens);
  CHECK_BB(o_wheel_sens)
  o_lfo_amp_sens = import_var(env, "int", "LFO_AMP_SENS",      ae_flag_static | ae_flag_const, lfo_amp_sens);
  CHECK_BB(o_lfo_amp_sens)
  o_lfo_freq_mod = import_var(env, "int", "LFO_FREQ_MOD",      ae_flag_static | ae_flag_const, lfo_freq_mod);
  CHECK_BB(o_lfo_freq_mod)
  o_enable = import_var(env, "int", "ENABLE",      ae_flag_static | ae_flag_const, enable);
  CHECK_BB(o_enable)
  o_carriers = import_var(env, "int", "CARRIERS",      ae_flag_static | ae_flag_const, carriers);
  CHECK_BB(o_carriers)
  o_carrier0 = import_var(env, "int", "CARRIER0",      ae_flag_static | ae_flag_const, carrier0);
  CHECK_BB(o_carrier0)

  // global
  o_g_vol    = import_var(env, "int", "GVOL", ae_flag_static | ae_flag_const, g_vol);
  CHECK_BB(o_g_vol)

  o_g_lfo    = import_var(env, "int", "GLFO",      ae_flag_static | ae_flag_const, g_lfo);
  CHECK_BB(o_g_lfo)
  // status
  o_ok    = import_var(env, "int", "OK",      ae_flag_static | ae_flag_const, ok);
  CHECK_BB(o_ok)
  o_busy   = import_var(env, "int", "BUSY",   ae_flag_static | ae_flag_const, busy);
  CHECK_BB(o_busy)
  o_small  = import_var(env, "int", "SMALL",   ae_flag_static | ae_flag_const, small);
  CHECK_BB(o_small)
  o_nonul  = import_var(env, "int", "NONUL",   ae_flag_static | ae_flag_const, nonul);
  CHECK_BB(o_nonul)
  o_format = import_var(env, "int", "FORMAT",   ae_flag_static | ae_flag_const, format);
  CHECK_BB(o_format)
  o_unknown = import_var(env, "int", "UNKNOWN",   ae_flag_static | ae_flag_const, unknown);
  CHECK_BB(o_unknown)

  dl_func_init(&fun, "void", "init", (m_uint)init);
    dl_func_add_arg(&fun, "int", "plyphony");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "parameter", (m_uint)parameter);
    dl_func_add_arg(&fun, "int", "parameter");
    dl_func_add_arg(&fun, "int", "operator_index");
    dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "parameter", (m_uint)global_parameter);
    dl_func_add_arg(&fun, "int", "parameter");
    dl_func_add_arg(&fun, "float", "value");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "noteon", (m_uint)noteon);
    dl_func_add_arg(&fun, "int", "note");
    dl_func_add_arg(&fun, "int", "velocity");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "reset", (m_uint)synth_reset);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "noteoff", (m_uint)noteoff);
    dl_func_add_arg(&fun, "int", "note");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "sustain", (m_uint)sustain);
    dl_func_add_arg(&fun, "int", "enable");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "wheel", (m_uint)wheel);
    dl_func_add_arg(&fun, "int", "value");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "bend", (m_uint)bend);
    dl_func_add_arg(&fun, "int", "value");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "void", "release", (m_uint)release_all);
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "int", "load", (m_uint)load);
    dl_func_add_arg(&fun, "string", "filename");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "int", "save", (m_uint)save);
    dl_func_add_arg(&fun, "string", "filename");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}

// MISS: reset
