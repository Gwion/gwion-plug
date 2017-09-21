#include "map.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "import.h"
#include "instr.h"
#include "lang.h"
#include "vm.h"
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
  SYNTH(o) = fmsynth_new(shred->vm_ref->sp->sr, POLYPHONY);
  UGEN(o)->tick = fmsynth_tick;
  assign_ugen(UGEN(o), 0, 2, 0, SYNTH(o));
}

DTOR(dtor)
{
  fmsynth_free(SYNTH(o));
}

MFUN(init)
{
  fmsynth_free(SYNTH(o));
  SYNTH(o) = fmsynth_new(shred->vm_ref->sp->sr, *(m_uint*)MEM(SZ_INT));
  UGEN(o)->ug = SYNTH(o);
}
MFUN(parameter)
{
  fmsynth_set_parameter(SYNTH(o),
      *(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2), *(m_float*)MEM(SZ_INT*3));
}

MFUN(global_parameter)
{
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
  m_uint size = fmsynth_preset_size();
  char* buf[size];
  struct fmsynth_preset_metadata metadata;
  fmsynth_free(SYNTH(o));
  m_str filename = STRING( *(M_Object*)MEM(SZ_INT) );
  FILE* file = fopen(filename, "r");

  SYNTH(o) = fmsynth_new(shred->vm_ref->sp->sr, POLYPHONY);
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
  *(m_uint*)RETURN = fmsynth_preset_load(SYNTH(o), &metadata,
      buf, fmsynth_preset_size());
  free(STRING(NAME(o)));
  free(STRING(AUTHOR(o)));
  STRING(NAME(o)) = strdup(metadata.name);
  STRING(AUTHOR(o)) = strdup(metadata.author);
}

MFUN(save)
{
  m_str filename = STRING( *(M_Object*)MEM(SZ_INT) );
  FILE* file = fopen(filename, "w");
  if(!file)
  {
    *(m_uint*)RETURN = -1;
    return;
  }
  void* buf;
  struct fmsynth_preset_metadata* metadata = malloc(sizeof(struct fmsynth_preset_metadata));
  memset(metadata, 0, sizeof(metadata));
  strcat(metadata->name, STRING(NAME(o)));
  strcat(metadata->author, STRING(AUTHOR(o)));
  *(m_uint*)RETURN = fmsynth_preset_save(SYNTH(o), metadata,
      buf, fmsynth_preset_size());
  fwrite(buf, fmsynth_preset_size(), 1, file);
  size_t len = fwrite(buf,fmsynth_preset_size(), 1, file);
  if(len != 1) {
    *(m_uint*)RETURN = -1;
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

IMPORT
{
  ALLOC_PTR(amp, m_uint, FMSYNTH_PARAM_AMP);
  ALLOC_PTR(pan, m_uint, FMSYNTH_PARAM_PAN);
  ALLOC_PTR(freq_mod, m_uint, FMSYNTH_PARAM_FREQ_MOD);
  ALLOC_PTR(freq_offset, m_uint, FMSYNTH_PARAM_FREQ_OFFSET);
  ALLOC_PTR(target0, m_uint, FMSYNTH_PARAM_ENVELOPE_TARGET0);
  ALLOC_PTR(target1, m_uint, FMSYNTH_PARAM_ENVELOPE_TARGET1);
  ALLOC_PTR(target2, m_uint, FMSYNTH_PARAM_ENVELOPE_TARGET2);
  ALLOC_PTR(delay0, m_uint, FMSYNTH_PARAM_DELAY0);
  ALLOC_PTR(delay1, m_uint, FMSYNTH_PARAM_DELAY1);
  ALLOC_PTR(delay2, m_uint, FMSYNTH_PARAM_DELAY2);
  ALLOC_PTR(rel, m_uint, FMSYNTH_PARAM_RELEASE_TIME);
  ALLOC_PTR(mid_point, m_uint, FMSYNTH_PARAM_KEYBOARD_SCALING_MID_POINT);
  ALLOC_PTR(low_fact, m_uint, FMSYNTH_PARAM_KEYBOARD_SCALING_LOW_FACTOR);
  ALLOC_PTR(high_fact, m_uint, FMSYNTH_PARAM_KEYBOARD_SCALING_HIGH_FACTOR);
  ALLOC_PTR(velo_sens, m_uint, FMSYNTH_PARAM_VELOCITY_SENSITIVITY);
  ALLOC_PTR(wheel_sens, m_uint, FMSYNTH_PARAM_MOD_WHEEL_SENSITIVITY);
  ALLOC_PTR(lfo_amp_sens, m_uint, FMSYNTH_PARAM_LFO_AMP_SENSITIVITY);
  ALLOC_PTR(lfo_freq_mod, m_uint, FMSYNTH_PARAM_LFO_FREQ_MOD_DEPTH);
  ALLOC_PTR(enable, m_uint, FMSYNTH_PARAM_ENABLE);
  ALLOC_PTR(carriers, m_uint, FMSYNTH_PARAM_CARRIERS);
  ALLOC_PTR(carrier0, m_uint, FMSYNTH_PARAM_MOD_TO_CARRIERS0);
  ALLOC_PTR(g_vol, m_uint, FMSYNTH_GLOBAL_PARAM_VOLUME);
  ALLOC_PTR(g_lfo, m_uint, FMSYNTH_GLOBAL_PARAM_LFO_FREQ);
  ALLOC_PTR(ok, m_uint, FMSYNTH_STATUS_OK);
  ALLOC_PTR(busy, m_uint, FMSYNTH_STATUS_BUSY);
  ALLOC_PTR(small, m_uint, FMSYNTH_STATUS_BUFFER_TOO_SMALL);
  ALLOC_PTR(nonul, m_uint, FMSYNTH_STATUS_NO_NUL_TERMINATE);
  ALLOC_PTR(format, m_uint, FMSYNTH_STATUS_INVALID_FORMAT);
  ALLOC_PTR(unknown, m_uint, FMSYNTH_STATUS_MESSAGE_UNKNOWN);

  CHECK_BB(importer_class_begin(importer, &t_fmsynth, ctor, dtor))
  o_fmsynth_data = importer_add_var(importer, "int", "&synth", ae_flag_member, NULL);
  CHECK_BB(o_fmsynth_data)
  o_fmsynth_name = importer_add_var(importer, "string", "name", ae_flag_member, NULL);
  CHECK_BB(o_fmsynth_name)
  o_fmsynth_author = importer_add_var(importer, "string", "author", ae_flag_member, NULL);
  CHECK_BB(o_fmsynth_author)

  // params
  o_amp = importer_add_var(importer, "int", "AMP", ae_flag_static | ae_flag_const, amp);
  CHECK_BB(o_pan)
  o_pan = importer_add_var(importer, "int", "PAN", ae_flag_static | ae_flag_const, pan);
  CHECK_BB(o_pan)

  o_freq_mod = importer_add_var(importer, "int", "FREQ_MOD", ae_flag_static | ae_flag_const, freq_mod);
  CHECK_BB(o_freq_offset)
  o_freq_offset = importer_add_var(importer, "int", "FREQ_OFFSET", ae_flag_static | ae_flag_const, freq_offset);
  CHECK_BB(o_freq_mod)
  o_target0 = importer_add_var(importer, "int", "TARGET0", ae_flag_static | ae_flag_const, target0);
  CHECK_BB(o_target0)
  o_target1 = importer_add_var(importer, "int", "TARGET1", ae_flag_static | ae_flag_const, target1);
  CHECK_BB(o_target1)
  o_target2 = importer_add_var(importer, "int", "TARGET2", ae_flag_static | ae_flag_const, target2);
  CHECK_BB(o_target2)
  o_delay0 = importer_add_var(importer, "int", "DELAY0", ae_flag_static | ae_flag_const, delay0);
  CHECK_BB(o_delay0)
  o_delay1 = importer_add_var(importer, "int", "DELAY1", ae_flag_static | ae_flag_const, delay1);
  CHECK_BB(o_delay0)
  o_delay2 = importer_add_var(importer, "int", "DELAY2", ae_flag_static | ae_flag_const, delay2);
  CHECK_BB(o_delay2)
  o_rel = importer_add_var(importer, "int", "RELEASE", ae_flag_static | ae_flag_const, rel);
  CHECK_BB(o_rel)
  o_mid_point = importer_add_var(importer, "int", "MID_POINT", ae_flag_static | ae_flag_const, mid_point);
  CHECK_BB(o_mid_point)
  o_low_fact = importer_add_var(importer, "int", "LOW_FACTOR", ae_flag_static | ae_flag_const, low_fact);
  CHECK_BB(o_low_fact)
  o_high_fact = importer_add_var(importer, "int", "HIGH_FACTOR", ae_flag_static | ae_flag_const, high_fact);
  CHECK_BB(o_high_fact)
  o_velo_sens = importer_add_var(importer, "int", "VELO_SENS", ae_flag_static | ae_flag_const, velo_sens);
  CHECK_BB(o_velo_sens)
  o_wheel_sens = importer_add_var(importer, "int", "WHEEL_SENS", ae_flag_static | ae_flag_const, wheel_sens);
  CHECK_BB(o_wheel_sens)
  o_lfo_amp_sens = importer_add_var(importer, "int", "LFO_AMP_SENS", ae_flag_static | ae_flag_const, lfo_amp_sens);
  CHECK_BB(o_lfo_amp_sens)
  o_lfo_freq_mod = importer_add_var(importer, "int", "LFO_FREQ_MOD", ae_flag_static | ae_flag_const, lfo_freq_mod);
  CHECK_BB(o_lfo_freq_mod)
  o_enable = importer_add_var(importer, "int", "ENABLE", ae_flag_static | ae_flag_const, enable);
  CHECK_BB(o_enable)
  o_carriers = importer_add_var(importer, "int", "CARRIERS", ae_flag_static | ae_flag_const, carriers);
  CHECK_BB(o_carriers)
  o_carrier0 = importer_add_var(importer, "int", "CARRIER0", ae_flag_static | ae_flag_const, carrier0);
  CHECK_BB(o_carrier0)

  // global
  o_g_vol    = importer_add_var(importer, "int", "GVOL", ae_flag_static | ae_flag_const, g_vol);
  CHECK_BB(o_g_vol)

  o_g_lfo    = importer_add_var(importer, "int", "GLFO", ae_flag_static | ae_flag_const, g_lfo);
  CHECK_BB(o_g_lfo)
  // status
  o_ok    = importer_add_var(importer, "int", "OK", ae_flag_static | ae_flag_const, ok);
  CHECK_BB(o_ok)
  o_busy   = importer_add_var(importer, "int", "BUSY", ae_flag_static | ae_flag_const, busy);
  CHECK_BB(o_busy)
  o_small  = importer_add_var(importer, "int", "SMALL", ae_flag_static | ae_flag_const, small);
  CHECK_BB(o_small)
  o_nonul  = importer_add_var(importer, "int", "NONUL", ae_flag_static | ae_flag_const, nonul);
  CHECK_BB(o_nonul)
  o_format = importer_add_var(importer, "int", "FORMAT", ae_flag_static | ae_flag_const, format);
  CHECK_BB(o_format)
  o_unknown = importer_add_var(importer, "int", "UNKNOWN", ae_flag_static | ae_flag_const, unknown);
  CHECK_BB(o_unknown)
 
  importer_func_begin(importer, "void", "init", (m_uint)init);
    importer_add_arg(importer, "int", "plyphony");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "parameter", (m_uint)parameter);
    importer_add_arg(importer, "int", "parameter");
    importer_add_arg(importer, "int", "operator_index");
    importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "parameter", (m_uint)global_parameter);
    importer_add_arg(importer, "int", "parameter");
    importer_add_arg(importer, "float", "value");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "noteon", (m_uint)noteon);
    importer_add_arg(importer, "int", "note");
    importer_add_arg(importer, "int", "velocity");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "reset", (m_uint)synth_reset);
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "noteoff", (m_uint)noteoff);
    importer_add_arg(importer, "int", "note");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "sustain", (m_uint)sustain);
    importer_add_arg(importer, "int", "enable");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "wheel", (m_uint)wheel);
    importer_add_arg(importer, "int", "value");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "bend", (m_uint)bend);
    importer_add_arg(importer, "int", "value");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "void", "release", (m_uint)release_all);
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "int", "load", (m_uint)load);
    importer_add_arg(importer, "string", "filename");
  CHECK_BB(importer_add_fun(importer, 0))
  importer_func_begin(importer, "int", "save", (m_uint)save);
    importer_add_arg(importer, "string", "filename");
  CHECK_BB(importer_add_fun(importer, 0))
  CHECK_BB(importer_class_end(importer))
  return 1;
}

// MISS: reset
// metadata can be heap
