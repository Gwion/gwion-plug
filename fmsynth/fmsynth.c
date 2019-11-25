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
#include "object.h"
#include "gwion.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "gwi.h"
#include "gwion.h"
#include "fmsynth.h"

static m_int o_fmsynth_data, o_fmsynth_name, o_fmsynth_author;
#define SYNTH(o) *(fmsynth_t**)(o->data + o_fmsynth_data)
#define NAME(o) *(M_Object*)(o->data + o_fmsynth_name)
#define AUTHOR(o) *(M_Object*)(o->data + o_fmsynth_author)
#define POLYPHONY 64

static inline void set_synth(M_Object o, fmsynth_t* t) {
  UGEN(o)->module.gen.data = SYNTH(o) = t;
}

static TICK(fmsynth_tick) {
  float left = 0.;
  float right = 0.;
  fmsynth_render(u->module.gen.data, &left, &right, 1);
  UGEN(u->connect.multi->channel[0])->out = left;
  UGEN(u->connect.multi->channel[1])->out = right;
  u->out = (m_float)((left + right) / 2);
}

CTOR(ctor) {
  NAME(o) = new_string(shred->info->vm->gwion->mp, NULL, "name");
  AUTHOR(o) = new_string(shred->info->vm->gwion->mp, NULL, "author");
  SYNTH(o) = fmsynth_new(shred->info->vm->bbq->si->sr, POLYPHONY);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 2);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fmsynth_tick, SYNTH(o), 0);
}

DTOR(dtor) {
  fmsynth_free(SYNTH(o));
}

MFUN(init) {
  fmsynth_free(SYNTH(o));
  set_synth(o, fmsynth_new(shred->info->vm->bbq->si->sr, *(m_uint*)MEM(SZ_INT)));
//  UGEN(o)->module.gen.data = SYNTH(o);
}
MFUN(parameter) {
  fmsynth_set_parameter(SYNTH(o),
      *(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2), *(m_float*)MEM(SZ_INT*3));
}

MFUN(global_parameter) {
  fmsynth_set_global_parameter(SYNTH(o),
      *(m_uint*)MEM(SZ_INT), *(m_float*)MEM(SZ_INT*2));
}

MFUN(synth_reset) {
 fmsynth_reset(SYNTH(o));
}

MFUN(noteon) {
 fmsynth_note_on(SYNTH(o), *(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2));
}

MFUN(noteoff) {
 fmsynth_note_off(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

MFUN(sustain) {
 fmsynth_set_sustain(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

MFUN(wheel) {
 fmsynth_set_mod_wheel(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

MFUN(bend) {
 fmsynth_set_pitch_bend(SYNTH(o), *(m_uint*)MEM(SZ_INT));
}

MFUN(release_all)
{
 fmsynth_release_all(SYNTH(o));
}

MFUN(load) {
  m_uint size = fmsynth_preset_size();
  char* buf[size];
  struct fmsynth_preset_metadata metadata;
  fmsynth_free(SYNTH(o));
  m_str filename = STRING( *(M_Object*)MEM(SZ_INT) );
  FILE* file = fopen(filename, "r");

  set_synth(o, fmsynth_new(shred->info->vm->bbq->si->sr, POLYPHONY));
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
//  free(STRING(NAME(o)));
//  free(STRING(AUTHOR(o)));
//  STRING(NAME(o)) = s_name(insert_symbol(strdup(metadata.name)));
//  STRING(AUTHOR(o)) = strdup(metadata.author);
  STRING(NAME(o)) = metadata.name;
  STRING(AUTHOR(o)) = metadata.author;
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
  struct fmsynth_preset_metadata* metadata = (struct fmsynth_preset_metadata*)xmalloc(sizeof(struct fmsynth_preset_metadata));
  memset(metadata, 0, sizeof(metadata));
  strcpy(metadata->name, STRING(NAME(o)));
  strcpy(metadata->author, STRING(AUTHOR(o)));
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

GWION_IMPORT(fmsynth) {
  ALLOC_PTR(gwi->gwion->mp, amp, m_uint, FMSYNTH_PARAM_AMP);
  ALLOC_PTR(gwi->gwion->mp, pan, m_uint, FMSYNTH_PARAM_PAN);
  ALLOC_PTR(gwi->gwion->mp, freq_mod, m_uint, FMSYNTH_PARAM_FREQ_MOD);
  ALLOC_PTR(gwi->gwion->mp, freq_offset, m_uint, FMSYNTH_PARAM_FREQ_OFFSET);
  ALLOC_PTR(gwi->gwion->mp, target0, m_uint, FMSYNTH_PARAM_ENVELOPE_TARGET0);
  ALLOC_PTR(gwi->gwion->mp, target1, m_uint, FMSYNTH_PARAM_ENVELOPE_TARGET1);
  ALLOC_PTR(gwi->gwion->mp, target2, m_uint, FMSYNTH_PARAM_ENVELOPE_TARGET2);
  ALLOC_PTR(gwi->gwion->mp, delay0, m_uint, FMSYNTH_PARAM_DELAY0);
  ALLOC_PTR(gwi->gwion->mp, delay1, m_uint, FMSYNTH_PARAM_DELAY1);
  ALLOC_PTR(gwi->gwion->mp, delay2, m_uint, FMSYNTH_PARAM_DELAY2);
  ALLOC_PTR(gwi->gwion->mp, rel, m_uint, FMSYNTH_PARAM_RELEASE_TIME);
  ALLOC_PTR(gwi->gwion->mp, mid_point, m_uint, FMSYNTH_PARAM_KEYBOARD_SCALING_MID_POINT);
  ALLOC_PTR(gwi->gwion->mp, low_fact, m_uint, FMSYNTH_PARAM_KEYBOARD_SCALING_LOW_FACTOR);
  ALLOC_PTR(gwi->gwion->mp, high_fact, m_uint, FMSYNTH_PARAM_KEYBOARD_SCALING_HIGH_FACTOR);
  ALLOC_PTR(gwi->gwion->mp, velo_sens, m_uint, FMSYNTH_PARAM_VELOCITY_SENSITIVITY);
  ALLOC_PTR(gwi->gwion->mp, wheel_sens, m_uint, FMSYNTH_PARAM_MOD_WHEEL_SENSITIVITY);
  ALLOC_PTR(gwi->gwion->mp, lfo_amp_sens, m_uint, FMSYNTH_PARAM_LFO_AMP_SENSITIVITY);
  ALLOC_PTR(gwi->gwion->mp, lfo_freq_mod, m_uint, FMSYNTH_PARAM_LFO_FREQ_MOD_DEPTH);
  ALLOC_PTR(gwi->gwion->mp, enable, m_uint, FMSYNTH_PARAM_ENABLE);
  ALLOC_PTR(gwi->gwion->mp, carriers, m_uint, FMSYNTH_PARAM_CARRIERS);
  ALLOC_PTR(gwi->gwion->mp, carrier0, m_uint, FMSYNTH_PARAM_MOD_TO_CARRIERS0);
  ALLOC_PTR(gwi->gwion->mp, g_vol, m_uint, FMSYNTH_GLOBAL_PARAM_VOLUME);
  ALLOC_PTR(gwi->gwion->mp, g_lfo, m_uint, FMSYNTH_GLOBAL_PARAM_LFO_FREQ);
  ALLOC_PTR(gwi->gwion->mp, ok, m_uint, FMSYNTH_STATUS_OK);
  ALLOC_PTR(gwi->gwion->mp, busy, m_uint, FMSYNTH_STATUS_BUSY);
  ALLOC_PTR(gwi->gwion->mp, small, m_uint, FMSYNTH_STATUS_BUFFER_TOO_SMALL);
  ALLOC_PTR(gwi->gwion->mp, nonul, m_uint, FMSYNTH_STATUS_NO_NUL_TERMINATE);
  ALLOC_PTR(gwi->gwion->mp, format, m_uint, FMSYNTH_STATUS_INVALID_FORMAT);
  ALLOC_PTR(gwi->gwion->mp, unknown, m_uint, FMSYNTH_STATUS_MESSAGE_UNKNOWN);

  Type t_fmsynth;
  GWI_BB(gwi_class_ini(gwi, "FMsynth", "UGen"))
  gwi_class_xtor(gwi, ctor, dtor);
  gwi_item_ini(gwi,"int", "&synth");
  o_fmsynth_data = gwi_item_end(gwi, ae_flag_member, NULL);
  GWI_BB(o_fmsynth_data)
  gwi_item_ini(gwi,"string", "name");
  o_fmsynth_name = gwi_item_end(gwi, ae_flag_member, NULL);
  GWI_BB(o_fmsynth_name)
  gwi_item_ini(gwi,"string", "author");
  o_fmsynth_author = gwi_item_end(gwi, ae_flag_member, NULL);
  GWI_BB(o_fmsynth_author)

  // params
  gwi_item_ini(gwi,"int", "AMP");
  o_amp = gwi_item_end(gwi, ae_flag_static | ae_flag_const, amp);
  GWI_BB(o_pan)
  gwi_item_ini(gwi,"int", "PAN");
  o_pan = gwi_item_end(gwi, ae_flag_static | ae_flag_const, pan);
  GWI_BB(o_pan)

  gwi_item_ini(gwi,"int", "FREQ_MOD");
  o_freq_mod = gwi_item_end(gwi, ae_flag_static | ae_flag_const, freq_mod);
  GWI_BB(o_freq_offset)
  gwi_item_ini(gwi,"int", "FREQ_OFFSET");
  o_freq_offset = gwi_item_end(gwi, ae_flag_static | ae_flag_const, freq_offset);
  GWI_BB(o_freq_mod)
  gwi_item_ini(gwi,"int", "TARGET0");
  o_target0 = gwi_item_end(gwi, ae_flag_static | ae_flag_const, target0);
  GWI_BB(o_target0)
  gwi_item_ini(gwi,"int", "TARGET1");
  o_target1 = gwi_item_end(gwi, ae_flag_static | ae_flag_const, target1);
  GWI_BB(o_target1)
  gwi_item_ini(gwi,"int", "TARGET2");
  o_target2 = gwi_item_end(gwi, ae_flag_static | ae_flag_const, target2);
  GWI_BB(o_target2)
  gwi_item_ini(gwi,"int", "DELAY0");
  o_delay0 = gwi_item_end(gwi, ae_flag_static | ae_flag_const, delay0);
  GWI_BB(o_delay0)
  gwi_item_ini(gwi,"int", "DELAY1");
  o_delay1 = gwi_item_end(gwi, ae_flag_static | ae_flag_const, delay1);
  GWI_BB(o_delay0)
  gwi_item_ini(gwi,"int", "DELAY2");
  o_delay2 = gwi_item_end(gwi, ae_flag_static | ae_flag_const, delay2);
  GWI_BB(o_delay2)
  gwi_item_ini(gwi,"int", "RELEASE");
  o_rel = gwi_item_end(gwi, ae_flag_static | ae_flag_const, rel);
  GWI_BB(o_rel)
  gwi_item_ini(gwi,"int", "MID_POINT");
  o_mid_point = gwi_item_end(gwi, ae_flag_static | ae_flag_const, mid_point);
  GWI_BB(o_mid_point)
  gwi_item_ini(gwi,"int", "LOW_FACTOR");
  o_low_fact = gwi_item_end(gwi, ae_flag_static | ae_flag_const, low_fact);
  GWI_BB(o_low_fact)
  gwi_item_ini(gwi,"int", "HIGH_FACTOR");
  o_high_fact = gwi_item_end(gwi, ae_flag_static | ae_flag_const, high_fact);
  GWI_BB(o_high_fact)
  gwi_item_ini(gwi,"int", "VELO_SENS");
  o_velo_sens = gwi_item_end(gwi, ae_flag_static | ae_flag_const, velo_sens);
  GWI_BB(o_velo_sens)
  gwi_item_ini(gwi,"int", "WHEEL_SENS");
  o_wheel_sens = gwi_item_end(gwi, ae_flag_static | ae_flag_const, wheel_sens);
  GWI_BB(o_wheel_sens)
  gwi_item_ini(gwi,"int", "LFO_AMP_SENS");
  o_lfo_amp_sens = gwi_item_end(gwi, ae_flag_static | ae_flag_const, lfo_amp_sens);
  GWI_BB(o_lfo_amp_sens)
  gwi_item_ini(gwi,"int", "LFO_FREQ_MOD");
  o_lfo_freq_mod = gwi_item_end(gwi, ae_flag_static | ae_flag_const, lfo_freq_mod);
  GWI_BB(o_lfo_freq_mod)
  gwi_item_ini(gwi,"int", "ENABLE");
  o_enable = gwi_item_end(gwi, ae_flag_static | ae_flag_const, enable);
  GWI_BB(o_enable)
  gwi_item_ini(gwi,"int", "CARRIERS");
  o_carriers = gwi_item_end(gwi, ae_flag_static | ae_flag_const, carriers);
  GWI_BB(o_carriers)
  gwi_item_ini(gwi,"int", "CARRIER0");
  o_carrier0 = gwi_item_end(gwi, ae_flag_static | ae_flag_const, carrier0);
  GWI_BB(o_carrier0)

  // global
  gwi_item_ini(gwi,"int", "GVOL");
  o_g_vol    = gwi_item_end(gwi, ae_flag_static | ae_flag_const, g_vol);
  GWI_BB(o_g_vol)

  gwi_item_ini(gwi,"int", "GLFO");
  o_g_lfo    = gwi_item_end(gwi, ae_flag_static | ae_flag_const, g_lfo);
  GWI_BB(o_g_lfo)
  // status
  gwi_item_ini(gwi,"int", "OK");
  o_ok    = gwi_item_end(gwi, ae_flag_static | ae_flag_const, ok);
  GWI_BB(o_ok)
  gwi_item_ini(gwi,"int", "BUSY");
  o_busy   = gwi_item_end(gwi, ae_flag_static | ae_flag_const, busy);
  GWI_BB(o_busy)
  gwi_item_ini(gwi,"int", "SMALL");
  o_small  = gwi_item_end(gwi, ae_flag_static | ae_flag_const, small);
  GWI_BB(o_small)
  gwi_item_ini(gwi,"int", "NONUL");
  o_nonul  = gwi_item_end(gwi, ae_flag_static | ae_flag_const, nonul);
  GWI_BB(o_nonul)
  gwi_item_ini(gwi,"int", "FORMAT");
  o_format = gwi_item_end(gwi, ae_flag_static | ae_flag_const, format);
  GWI_BB(o_format)
  gwi_item_ini(gwi,"int", "UNKNOWN");
  o_unknown = gwi_item_end(gwi, ae_flag_static | ae_flag_const, unknown);
  GWI_BB(o_unknown)
 
  gwi_func_ini(gwi, "void", "init");
    gwi_func_arg(gwi, "int", "plyphony");
  GWI_BB(gwi_func_end(gwi, init, ae_flag_none))
  gwi_func_ini(gwi, "void", "parameter");
    gwi_func_arg(gwi, "int", "parameter");
    gwi_func_arg(gwi, "int", "operator_index");
    gwi_func_arg(gwi, "float", "value");
  GWI_BB(gwi_func_end(gwi, parameter, ae_flag_none))
  gwi_func_ini(gwi, "void", "parameter");
    gwi_func_arg(gwi, "int", "parameter");
    gwi_func_arg(gwi, "float", "value");
  GWI_BB(gwi_func_end(gwi, global_parameter, ae_flag_none))
  gwi_func_ini(gwi, "void", "noteon");
    gwi_func_arg(gwi, "int", "note");
    gwi_func_arg(gwi, "int", "velocity");
  GWI_BB(gwi_func_end(gwi, noteon, ae_flag_none))
  gwi_func_ini(gwi, "void", "reset");
  GWI_BB(gwi_func_end(gwi, synth_reset, ae_flag_none))
  gwi_func_ini(gwi, "void", "noteoff");
    gwi_func_arg(gwi, "int", "note");
  GWI_BB(gwi_func_end(gwi, noteoff, ae_flag_none))
  gwi_func_ini(gwi, "void", "sustain");
    gwi_func_arg(gwi, "int", "enable");
  GWI_BB(gwi_func_end(gwi, sustain, ae_flag_none))
  gwi_func_ini(gwi, "void", "wheel");
    gwi_func_arg(gwi, "int", "value");
  GWI_BB(gwi_func_end(gwi, wheel, ae_flag_none))
  gwi_func_ini(gwi, "void", "bend");
    gwi_func_arg(gwi, "int", "value");
  GWI_BB(gwi_func_end(gwi, bend, ae_flag_none))
  gwi_func_ini(gwi, "void", "release");
  GWI_BB(gwi_func_end(gwi, release_all, ae_flag_none))
  gwi_func_ini(gwi, "int", "load");
    gwi_func_arg(gwi, "string", "filename");
  GWI_BB(gwi_func_end(gwi, load, ae_flag_none))
  gwi_func_ini(gwi, "int", "save");
    gwi_func_arg(gwi, "string", "filename");
  GWI_BB(gwi_func_end(gwi, save, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

// MISS: reset
// metadata can be heap
