#include <stdlib.h>
#include "Gwion.hh"
#include "gwi.h"
#include "array.h"

//#include <string>

extern "C" { ANEW UGen new_UGen(MemPool p);}
#include "Stk.h"
#include "Instrmnt.h"
#include "Generator.h"
#include "FM.h"
#include "Filter.h"
#include "Effect.h"
#include "Function.h"
#include "Sampler.h"
#include "ADSR.h"
#include "Asymp.h"
#include "BandedWG.h"
#include "BiQuad.h"
#include "Blit.h"
#include "BlitSaw.h"
#include "BlitSquare.h"
#include "BeeThree.h"
#include "BlowBotl.h"
#include "BlowHole.h"
#include "Bowed.h"
#include "BowTable.h"
#include "Brass.h"
#include "Chorus.h"
#include "Clarinet.h"
#include "Cubic.h"
#include "Delay.h"
#include "DelayA.h"
#include "DelayL.h"
#include "Drummer.h"
#include "Echo.h"
#include "Flute.h"
#include "FMVoices.h"
#include "FormSwep.h"
#include "FreeVerb.h"
#include "Granulate.h"
#include "Guitar.h"
#include "HevyMetl.h"
#include "JCRev.h"
#include "JetTable.h"
#include "LentPitShift.h"
#include "Mandolin.h"
#include "Mesh2D.h"
#include "Modal.h"
#include "ModalBar.h"
#include "Modulate.h"
#include "Moog.h"
#include "Noise.h"
#include "NRev.h"
#include "OnePole.h"
#include "OneZero.h"
#include "PercFlut.h"
#include "Phonemes.h"
#include "PitShift.h"
#include "Plucked.h"
#include "PoleZero.h"
#include "PRCRev.h"
#include "Recorder.h"
#include "ReedTable.h"
#include "Resonate.h"
#include "Rhodey.h"
#include "Saxofony.h"
#include "Shakers.h"
#include "Simple.h"
#include "SineWave.h"
#include "SingWave.h"
#include "Sitar.h"
#include "Vector3D.h"
#include "StifKarp.h"
#include "TubeBell.h"
#include "Twang.h"
#include "TwoPole.h"
#include "TwoZero.h"
#include "Voicer.h"
#include "VoicForm.h"
#include "Whistle.h"
#include "Wurley.h"
static Type t_Stk;
static Type t_Instrmnt;
static Type t_Generator;
static Type t_FM;
static Type t_Filter;
static Type t_Effect;
static Type t_Function;
static Type t_Sampler;
static Type t_ADSR;
static Type t_Asymp;
static Type t_BandedWG;
static Type t_BiQuad;
static Type t_Blit;
static Type t_BlitSaw;
static Type t_BlitSquare;
static Type t_BeeThree;
static Type t_BlowBotl;
static Type t_BlowHole;
static Type t_Bowed;
static Type t_BowTable;
static Type t_Brass;
static Type t_Chorus;
static Type t_Clarinet;
static Type t_Cubic;
static Type t_Delay;
static Type t_DelayA;
static Type t_DelayL;
static Type t_Drummer;
static Type t_Echo;
static Type t_Flute;
static Type t_FMVoices;
static Type t_FormSwep;
static Type t_FreeVerb;
static Type t_Granulate;
static Type t_Guitar;
static Type t_HevyMetl;
static Type t_JCRev;
static Type t_JetTable;
static Type t_LentPitShift;
static Type t_Mandolin;
static Type t_Mesh2D;
static Type t_Modal;
static Type t_ModalBar;
static Type t_Modulate;
static Type t_Moog;
static Type t_Noise;
static Type t_NRev;
static Type t_OnePole;
static Type t_OneZero;
static Type t_PercFlut;
static Type t_Phonemes;
static Type t_PitShift;
static Type t_Plucked;
static Type t_PoleZero;
static Type t_PRCRev;
static Type t_Recorder;
static Type t_ReedTable;
static Type t_Resonate;
static Type t_Rhodey;
static Type t_Saxofony;
static Type t_Shakers;
static Type t_Simple;
static Type t_SineWave;
static Type t_SingWave;
static Type t_Sitar;
static Type t_Vector3D;
static Type t_StifKarp;
static Type t_TubeBell;
static Type t_Twang;
static Type t_TwoPole;
static Type t_TwoZero;
static Type t_Voicer;
static Type t_VoicForm;
static Type t_Whistle;
static Type t_Wurley;

#define GW_Stk(a) *(stk::Stk**)(a->data)
static SFUN(gw_Stk_STK_SINT8_get) {
  stk::Stk::StkFormat result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_SINT8;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_SINT16_get) {
  stk::Stk::StkFormat result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_SINT16;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_SINT24_get) {
  stk::Stk::StkFormat result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_SINT24;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_SINT32_get) {
  stk::Stk::StkFormat result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_SINT32;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_FLOAT32_get) {
  stk::Stk::StkFormat result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_FLOAT32;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_FLOAT64_get) {
  stk::Stk::StkFormat result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_FLOAT64;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Stk_sampleRate) {
  stk::StkFloat result = (stk::StkFloat)stk::Stk::sampleRate();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Stk_setSampleRate) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Stk::setSampleRate(arg1);
}

static MFUN(gw_Stk_ignoreSampleRateChange0) {
  stk::Stk * arg1 = (stk::Stk *)GW_Stk((*(M_Object*)MEM(0)));
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_Stk_ignoreSampleRateChange1) {
  stk::Stk * arg1 = (stk::Stk *)GW_Stk((*(M_Object*)MEM(0)));
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_Stk_rawwavePath) {
  std::string result = stk::Stk::rawwavePath();
  *(M_Object*)RETURN = (&result)->data() ? new_string(shred->info->vm->gwion, (m_str)(&result)->data()) : NULL;
}

static MFUN(gw_Stk_setRawwavePath) {
  std::string arg1(STRING(*(M_Object*)MEM(0))); // here arg1
  stk::Stk::setRawwavePath(arg1);
}

static MFUN(gw_Stk_sleep) {
  unsigned long arg1 = (unsigned long)*(m_int*)MEM(0);
  stk::Stk::sleep(arg1);
}

static MFUN(gw_Stk_inRange) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_FLOAT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_FLOAT+SZ_FLOAT);
  bool result = (bool)stk::Stk::inRange(arg1,arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Stk_showWarnings) {
  bool arg1 = (bool)*(m_int*)MEM(0);
  stk::Stk::showWarnings(arg1);
}

static MFUN(gw_Stk_printErrors) {
  bool arg1 = (bool)*(m_int*)MEM(0);
  stk::Stk::printErrors(arg1);
}

static SFUN(gw_SRATE_get) {
  stk::StkFloat result = (stk::StkFloat)(stk::StkFloat)stk::SRATE;
  *(m_float*)RETURN = (m_float)(float)result;
}

static SFUN(gw_RT_BUFFER_SIZE_get) {
  unsigned int result = (unsigned int)(unsigned int)stk::RT_BUFFER_SIZE;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_PI_get) {
  stk::StkFloat result = (stk::StkFloat)(stk::StkFloat)stk::PI;
  *(m_float*)RETURN = (m_float)(float)result;
}

static SFUN(gw_TWO_PI_get) {
  stk::StkFloat result = (stk::StkFloat)(stk::StkFloat)stk::TWO_PI;
  *(m_float*)RETURN = (m_float)(float)result;
}

static SFUN(gw_ONE_OVER_128_get) {
  stk::StkFloat result = (stk::StkFloat)(stk::StkFloat)stk::ONE_OVER_128;
  *(m_float*)RETURN = (m_float)(float)result;
}

static TICK(Instrmnt_tick) {
  stk::Instrmnt *s = (stk::Instrmnt*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Instrmnt(a) *(stk::Instrmnt**)(a->data)
static MFUN(gw_Instrmnt_clear) {
  stk::Instrmnt * arg1 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Instrmnt_noteOn) {
  stk::Instrmnt * arg1 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Instrmnt_noteOff) {
  stk::Instrmnt * arg1 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Instrmnt_setFrequency) {
  stk::Instrmnt * arg1 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Instrmnt_controlChange) {
  stk::Instrmnt * arg1 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static MFUN(gw_Instrmnt_channelsOut) {
  stk::Instrmnt * arg1 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0)));
  unsigned int result = (unsigned int)((stk::Instrmnt const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static DTOR(gw_Instrmnt_dtor) {
  if(GW_Instrmnt(o)) {
    delete (stk::Instrmnt*)GW_Instrmnt(o);
    GW_Instrmnt(o) = NULL;
  }
}

#define GW_Generator(a) *(stk::Generator**)(a->data)
static MFUN(gw_Generator_channelsOut) {
  stk::Generator * arg1 = (stk::Generator *)GW_Generator((*(M_Object*)MEM(0)));
  unsigned int result = (unsigned int)((stk::Generator const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static DTOR(gw_Generator_dtor) {
  if(GW_Generator(o)) {
    delete (stk::Generator*)GW_Generator(o);
    GW_Generator(o) = NULL;
  }
}

static TICK(FM_tick) {
  stk::FM *s = (stk::FM*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_FM(a) *(stk::FM**)(a->data)
static DTOR(gw_FM_dtor) {
  if(GW_FM(o)) {
    delete (stk::FM*)GW_FM(o);
    GW_FM(o) = NULL;
  }
}

static MFUN(gw_FM_loadWaves) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  char ** arg2 = (char**)m_vector_addr(ARRAY(*(M_Object*)MEM(0+SZ_INT)), 0);
  (arg1)->loadWaves((char const **)arg2);
}

static MFUN(gw_FM_setFrequency) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_FM_setRatio) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setRatio(arg2,arg3);
}

static MFUN(gw_FM_setGain) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setGain(arg2,arg3);
}

static MFUN(gw_FM_setModulationSpeed) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationSpeed(arg2);
}

static MFUN(gw_FM_setModulationDepth) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationDepth(arg2);
}

static MFUN(gw_FM_setControl1) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setControl1(arg2);
}

static MFUN(gw_FM_setControl2) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setControl2(arg2);
}

static MFUN(gw_FM_keyOn) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  (arg1)->keyOn();
}

static MFUN(gw_FM_keyOff) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  (arg1)->keyOff();
}

static MFUN(gw_FM_noteOff) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_FM_controlChange) {
  stk::FM * arg1 = (stk::FM *)GW_FM((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

#define GW_Filter(a) *(stk::Filter**)(a->data)
static MFUN(gw_Filter_channelsIn) {
  stk::Filter * arg1 = (stk::Filter *)GW_Filter((*(M_Object*)MEM(0)));
  unsigned int result = (unsigned int)((stk::Filter const *)arg1)->channelsIn();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Filter_channelsOut) {
  stk::Filter * arg1 = (stk::Filter *)GW_Filter((*(M_Object*)MEM(0)));
  unsigned int result = (unsigned int)((stk::Filter const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Filter_clear) {
  stk::Filter * arg1 = (stk::Filter *)GW_Filter((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Filter_setGain) {
  stk::Filter * arg1 = (stk::Filter *)GW_Filter((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setGain(arg2);
}

static MFUN(gw_Filter_getGain) {
  stk::Filter * arg1 = (stk::Filter *)GW_Filter((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)((stk::Filter const *)arg1)->getGain();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Filter_phaseDelay) {
  stk::Filter * arg1 = (stk::Filter *)GW_Filter((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)(arg1)->phaseDelay(arg2);
  *(m_float*)RETURN = (m_float)(float)result;
}

static DTOR(gw_Filter_dtor) {
  if(GW_Filter(o)) {
    delete (stk::Filter*)GW_Filter(o);
    GW_Filter(o) = NULL;
  }
}

#define GW_Effect(a) *(stk::Effect**)(a->data)
static MFUN(gw_Effect_channelsOut) {
  stk::Effect * arg1 = (stk::Effect *)GW_Effect((*(M_Object*)MEM(0)));
  unsigned int result = (unsigned int)((stk::Effect const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Effect_clear) {
  stk::Effect * arg1 = (stk::Effect *)GW_Effect((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Effect_setEffectMix) {
  stk::Effect * arg1 = (stk::Effect *)GW_Effect((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setEffectMix(arg2);
}

static DTOR(gw_Effect_dtor) {
  if(GW_Effect(o)) {
    delete (stk::Effect*)GW_Effect(o);
    GW_Effect(o) = NULL;
  }
}

#define GW_Function(a) *(stk::Function**)(a->data)
static MFUN(gw_Function_lastOut) {
  stk::Function * arg1 = (stk::Function *)GW_Function((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)((stk::Function const *)arg1)->lastOut();
  *(m_float*)RETURN = (m_float)(float)result;
}

static DTOR(gw_Function_dtor) {
  if(GW_Function(o)) {
    delete (stk::Function*)GW_Function(o);
    GW_Function(o) = NULL;
  }
}

static TICK(Sampler_tick) {
  stk::Sampler *s = (stk::Sampler*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Sampler(a) *(stk::Sampler**)(a->data)
static DTOR(gw_Sampler_dtor) {
  if(GW_Sampler(o)) {
    delete (stk::Sampler*)GW_Sampler(o);
    GW_Sampler(o) = NULL;
  }
}

static MFUN(gw_Sampler_setFrequency) {
  stk::Sampler * arg1 = (stk::Sampler *)GW_Sampler((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Sampler_keyOn) {
  stk::Sampler * arg1 = (stk::Sampler *)GW_Sampler((*(M_Object*)MEM(0)));
  (arg1)->keyOn();
}

static MFUN(gw_Sampler_keyOff) {
  stk::Sampler * arg1 = (stk::Sampler *)GW_Sampler((*(M_Object*)MEM(0)));
  (arg1)->keyOff();
}

static MFUN(gw_Sampler_noteOff) {
  stk::Sampler * arg1 = (stk::Sampler *)GW_Sampler((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Sampler_controlChange) {
  stk::Sampler * arg1 = (stk::Sampler *)GW_Sampler((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(ADSR_tick) {
  stk::ADSR *s = (stk::ADSR*)u->module.gen.data;
  u->out = s->tick();
}

#define GW_ADSR(a) *(stk::ADSR**)(a->data)
static CTOR(gw_ADSR_ctor) {
  if(o->type_ref == t_ADSR)
  GW_ADSR(o) = new stk::ADSR();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), ADSR_tick, GW_ADSR(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_ADSR_dtor) {
  if(GW_ADSR(o)) {
    delete (stk::ADSR*)GW_ADSR(o);
    GW_ADSR(o) = NULL;
  }
}

static MFUN(gw_ADSR_keyOn) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  (arg1)->keyOn();
}

static MFUN(gw_ADSR_keyOff) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  (arg1)->keyOff();
}

static MFUN(gw_ADSR_setAttackRate) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setAttackRate(arg2);
}

static MFUN(gw_ADSR_setAttackTarget) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setAttackTarget(arg2);
}

static MFUN(gw_ADSR_setDecayRate) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDecayRate(arg2);
}

static MFUN(gw_ADSR_setSustainLevel) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSustainLevel(arg2);
}

static MFUN(gw_ADSR_setReleaseRate) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setReleaseRate(arg2);
}

static MFUN(gw_ADSR_setAttackTime) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setAttackTime(arg2);
}

static MFUN(gw_ADSR_setDecayTime) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDecayTime(arg2);
}

static MFUN(gw_ADSR_setReleaseTime) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setReleaseTime(arg2);
}

static MFUN(gw_ADSR_setAllTimes) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg5 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setAllTimes(arg2,arg3,arg4,arg5);
}

static MFUN(gw_ADSR_setTarget) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTarget(arg2);
}

static MFUN(gw_ADSR_getState) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  int result = (int)((stk::ADSR const *)arg1)->getState();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_ADSR_setValue) {
  stk::ADSR * arg1 = (stk::ADSR *)GW_ADSR((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setValue(arg2);
}

static TICK(Asymp_tick) {
  stk::Asymp *s = (stk::Asymp*)u->module.gen.data;
  u->out = s->tick();
}

static SFUN(gw_TARGET_THRESHOLD_get) {
  stk::StkFloat result = (stk::StkFloat)(stk::StkFloat)stk::TARGET_THRESHOLD;
  *(m_float*)RETURN = (m_float)(float)result;
}

#define GW_Asymp(a) *(stk::Asymp**)(a->data)
static CTOR(gw_Asymp_ctor) {
  if(o->type_ref == t_Asymp)
  GW_Asymp(o) = new stk::Asymp();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Asymp_tick, GW_Asymp(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Asymp_dtor) {
  if(GW_Asymp(o)) {
    delete (stk::Asymp*)GW_Asymp(o);
    GW_Asymp(o) = NULL;
  }
}

static MFUN(gw_Asymp_keyOn) {
  stk::Asymp * arg1 = (stk::Asymp *)GW_Asymp((*(M_Object*)MEM(0)));
  (arg1)->keyOn();
}

static MFUN(gw_Asymp_keyOff) {
  stk::Asymp * arg1 = (stk::Asymp *)GW_Asymp((*(M_Object*)MEM(0)));
  (arg1)->keyOff();
}

static MFUN(gw_Asymp_setTau) {
  stk::Asymp * arg1 = (stk::Asymp *)GW_Asymp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTau(arg2);
}

static MFUN(gw_Asymp_setTime) {
  stk::Asymp * arg1 = (stk::Asymp *)GW_Asymp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTime(arg2);
}

static MFUN(gw_Asymp_setT60) {
  stk::Asymp * arg1 = (stk::Asymp *)GW_Asymp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setT60(arg2);
}

static MFUN(gw_Asymp_setTarget) {
  stk::Asymp * arg1 = (stk::Asymp *)GW_Asymp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTarget(arg2);
}

static MFUN(gw_Asymp_setValue) {
  stk::Asymp * arg1 = (stk::Asymp *)GW_Asymp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setValue(arg2);
}

static MFUN(gw_Asymp_getState) {
  stk::Asymp * arg1 = (stk::Asymp *)GW_Asymp((*(M_Object*)MEM(0)));
  int result = (int)((stk::Asymp const *)arg1)->getState();
  *(m_int*)RETURN = (m_int)result;
}

static TICK(BandedWG_tick) {
  stk::BandedWG *s = (stk::BandedWG*)u->module.gen.data;
  u->out = s->tick();
}

static SFUN(gw_MAX_BANDED_MODES_get) {
  int result = (int)(int)stk::MAX_BANDED_MODES;
  *(m_int*)RETURN = (m_int)result;
}

#define GW_BandedWG(a) *(stk::BandedWG**)(a->data)
static CTOR(gw_BandedWG_ctor) {
  if(o->type_ref == t_BandedWG)
  GW_BandedWG(o) = new stk::BandedWG();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BandedWG_tick, GW_BandedWG(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_BandedWG_dtor) {
  if(GW_BandedWG(o)) {
    delete (stk::BandedWG*)GW_BandedWG(o);
    GW_BandedWG(o) = NULL;
  }
}

static MFUN(gw_BandedWG_clear) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_BandedWG_setStrikePosition) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setStrikePosition(arg2);
}

static MFUN(gw_BandedWG_setPreset) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setPreset(arg2);
}

static MFUN(gw_BandedWG_setFrequency) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BandedWG_startBowing) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBowing(arg2,arg3);
}

static MFUN(gw_BandedWG_stopBowing) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBowing(arg2);
}

static MFUN(gw_BandedWG_pluck) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_BandedWG_noteOn) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_BandedWG_noteOff) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_BandedWG_controlChange) {
  stk::BandedWG * arg1 = (stk::BandedWG *)GW_BandedWG((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(BiQuad_tick) {
  stk::BiQuad *s = (stk::BiQuad*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_BiQuad(a) *(stk::BiQuad**)(a->data)
static CTOR(gw_BiQuad_ctor) {
  if(o->type_ref == t_BiQuad)
  GW_BiQuad(o) = new stk::BiQuad();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BiQuad_tick, GW_BiQuad(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_BiQuad_dtor) {
  if(GW_BiQuad(o)) {
    delete (stk::BiQuad*)GW_BiQuad(o);
    GW_BiQuad(o) = NULL;
  }
}

static MFUN(gw_BiQuad_ignoreSampleRateChange0) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_BiQuad_ignoreSampleRateChange1) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_BiQuad_setCoefficients0) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg5 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg6 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  bool arg7 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5,arg6,arg7);
}

static MFUN(gw_BiQuad_setCoefficients1) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg5 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg6 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5,arg6);
}

static MFUN(gw_BiQuad_setB0) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_BiQuad_setB1) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB1(arg2);
}

static MFUN(gw_BiQuad_setB2) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB2(arg2);
}

static MFUN(gw_BiQuad_setA1) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_BiQuad_setA2) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA2(arg2);
}

static MFUN(gw_BiQuad_setResonance0) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  bool arg4 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3,arg4);
}

static MFUN(gw_BiQuad_setResonance1) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3);
}

static MFUN(gw_BiQuad_setNotch) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setNotch(arg2,arg3);
}

static MFUN(gw_BiQuad_setEqualGainZeroes) {
  stk::BiQuad * arg1 = (stk::BiQuad *)GW_BiQuad((*(M_Object*)MEM(0)));
  (arg1)->setEqualGainZeroes();
}

static TICK(Blit_tick) {
  stk::Blit *s = (stk::Blit*)u->module.gen.data;
  u->out = s->tick();
}

#define GW_Blit(a) *(stk::Blit**)(a->data)
static SFUN(gw_Blit_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Blit * result = (stk::Blit *)new stk::Blit(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Blit);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Blit");
  GW_Blit(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Blit_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Blit_ctor1) {
  if(o->type_ref == t_Blit)
  GW_Blit(o) = new stk::Blit();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Blit_tick, GW_Blit(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Blit_dtor) {
  if(GW_Blit(o)) {
    delete (stk::Blit*)GW_Blit(o);
    GW_Blit(o) = NULL;
  }
}

static MFUN(gw_Blit_reset) {
  stk::Blit * arg1 = (stk::Blit *)GW_Blit((*(M_Object*)MEM(0)));
  (arg1)->reset();
}

static MFUN(gw_Blit_setPhase) {
  stk::Blit * arg1 = (stk::Blit *)GW_Blit((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPhase(arg2);
}

static MFUN(gw_Blit_getPhase) {
  stk::Blit * arg1 = (stk::Blit *)GW_Blit((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)((stk::Blit const *)arg1)->getPhase();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Blit_setFrequency) {
  stk::Blit * arg1 = (stk::Blit *)GW_Blit((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Blit_setHarmonics0) {
  stk::Blit * arg1 = (stk::Blit *)GW_Blit((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setHarmonics(arg2);
}

static MFUN(gw_Blit_setHarmonics1) {
  stk::Blit * arg1 = (stk::Blit *)GW_Blit((*(M_Object*)MEM(0)));
  (arg1)->setHarmonics();
}

static TICK(BlitSaw_tick) {
  stk::BlitSaw *s = (stk::BlitSaw*)u->module.gen.data;
  u->out = s->tick();
}

#define GW_BlitSaw(a) *(stk::BlitSaw**)(a->data)
static SFUN(gw_BlitSaw_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::BlitSaw * result = (stk::BlitSaw *)new stk::BlitSaw(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_BlitSaw);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.BlitSaw");
  GW_BlitSaw(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), BlitSaw_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_BlitSaw_ctor1) {
  if(o->type_ref == t_BlitSaw)
  GW_BlitSaw(o) = new stk::BlitSaw();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlitSaw_tick, GW_BlitSaw(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_BlitSaw_dtor) {
  if(GW_BlitSaw(o)) {
    delete (stk::BlitSaw*)GW_BlitSaw(o);
    GW_BlitSaw(o) = NULL;
  }
}

static MFUN(gw_BlitSaw_reset) {
  stk::BlitSaw * arg1 = (stk::BlitSaw *)GW_BlitSaw((*(M_Object*)MEM(0)));
  (arg1)->reset();
}

static MFUN(gw_BlitSaw_setFrequency) {
  stk::BlitSaw * arg1 = (stk::BlitSaw *)GW_BlitSaw((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BlitSaw_setHarmonics0) {
  stk::BlitSaw * arg1 = (stk::BlitSaw *)GW_BlitSaw((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setHarmonics(arg2);
}

static MFUN(gw_BlitSaw_setHarmonics1) {
  stk::BlitSaw * arg1 = (stk::BlitSaw *)GW_BlitSaw((*(M_Object*)MEM(0)));
  (arg1)->setHarmonics();
}

static TICK(BlitSquare_tick) {
  stk::BlitSquare *s = (stk::BlitSquare*)u->module.gen.data;
  u->out = s->tick();
}

#define GW_BlitSquare(a) *(stk::BlitSquare**)(a->data)
static SFUN(gw_BlitSquare_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::BlitSquare * result = (stk::BlitSquare *)new stk::BlitSquare(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_BlitSquare);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.BlitSquare");
  GW_BlitSquare(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), BlitSquare_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_BlitSquare_ctor1) {
  if(o->type_ref == t_BlitSquare)
  GW_BlitSquare(o) = new stk::BlitSquare();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlitSquare_tick, GW_BlitSquare(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_BlitSquare_dtor) {
  if(GW_BlitSquare(o)) {
    delete (stk::BlitSquare*)GW_BlitSquare(o);
    GW_BlitSquare(o) = NULL;
  }
}

static MFUN(gw_BlitSquare_reset) {
  stk::BlitSquare * arg1 = (stk::BlitSquare *)GW_BlitSquare((*(M_Object*)MEM(0)));
  (arg1)->reset();
}

static MFUN(gw_BlitSquare_setPhase) {
  stk::BlitSquare * arg1 = (stk::BlitSquare *)GW_BlitSquare((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPhase(arg2);
}

static MFUN(gw_BlitSquare_getPhase) {
  stk::BlitSquare * arg1 = (stk::BlitSquare *)GW_BlitSquare((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)((stk::BlitSquare const *)arg1)->getPhase();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_BlitSquare_setFrequency) {
  stk::BlitSquare * arg1 = (stk::BlitSquare *)GW_BlitSquare((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BlitSquare_setHarmonics0) {
  stk::BlitSquare * arg1 = (stk::BlitSquare *)GW_BlitSquare((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setHarmonics(arg2);
}

static MFUN(gw_BlitSquare_setHarmonics1) {
  stk::BlitSquare * arg1 = (stk::BlitSquare *)GW_BlitSquare((*(M_Object*)MEM(0)));
  (arg1)->setHarmonics();
}

static TICK(BeeThree_tick) {
  stk::BeeThree *s = (stk::BeeThree*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_BeeThree(a) *(stk::BeeThree**)(a->data)
static CTOR(gw_BeeThree_ctor) {
  if(o->type_ref == t_BeeThree)
  GW_BeeThree(o) = new stk::BeeThree();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BeeThree_tick, GW_BeeThree(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_BeeThree_dtor) {
  if(GW_BeeThree(o)) {
    delete (stk::BeeThree*)GW_BeeThree(o);
    GW_BeeThree(o) = NULL;
  }
}

static MFUN(gw_BeeThree_noteOn) {
  stk::BeeThree * arg1 = (stk::BeeThree *)GW_BeeThree((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static TICK(BlowBotl_tick) {
  stk::BlowBotl *s = (stk::BlowBotl*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_BlowBotl(a) *(stk::BlowBotl**)(a->data)
static CTOR(gw_BlowBotl_ctor) {
  if(o->type_ref == t_BlowBotl)
  GW_BlowBotl(o) = new stk::BlowBotl();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlowBotl_tick, GW_BlowBotl(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_BlowBotl_dtor) {
  if(GW_BlowBotl(o)) {
    delete (stk::BlowBotl*)GW_BlowBotl(o);
    GW_BlowBotl(o) = NULL;
  }
}

static MFUN(gw_BlowBotl_clear) {
  stk::BlowBotl * arg1 = (stk::BlowBotl *)GW_BlowBotl((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_BlowBotl_setFrequency) {
  stk::BlowBotl * arg1 = (stk::BlowBotl *)GW_BlowBotl((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BlowBotl_startBlowing) {
  stk::BlowBotl * arg1 = (stk::BlowBotl *)GW_BlowBotl((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_BlowBotl_stopBlowing) {
  stk::BlowBotl * arg1 = (stk::BlowBotl *)GW_BlowBotl((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_BlowBotl_noteOn) {
  stk::BlowBotl * arg1 = (stk::BlowBotl *)GW_BlowBotl((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_BlowBotl_noteOff) {
  stk::BlowBotl * arg1 = (stk::BlowBotl *)GW_BlowBotl((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_BlowBotl_controlChange) {
  stk::BlowBotl * arg1 = (stk::BlowBotl *)GW_BlowBotl((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(BlowHole_tick) {
  stk::BlowHole *s = (stk::BlowHole*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_BlowHole(a) *(stk::BlowHole**)(a->data)
static SFUN(gw_BlowHole_ctor) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::BlowHole * result = (stk::BlowHole *)new stk::BlowHole(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_BlowHole);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.BlowHole");
  GW_BlowHole(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), BlowHole_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static DTOR(gw_BlowHole_dtor) {
  if(GW_BlowHole(o)) {
    delete (stk::BlowHole*)GW_BlowHole(o);
    GW_BlowHole(o) = NULL;
  }
}

static MFUN(gw_BlowHole_clear) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_BlowHole_setFrequency) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BlowHole_setTonehole) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTonehole(arg2);
}

static MFUN(gw_BlowHole_setVent) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVent(arg2);
}

static MFUN(gw_BlowHole_startBlowing) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_BlowHole_stopBlowing) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_BlowHole_noteOn) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_BlowHole_noteOff) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_BlowHole_controlChange) {
  stk::BlowHole * arg1 = (stk::BlowHole *)GW_BlowHole((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Bowed_tick) {
  stk::Bowed *s = (stk::Bowed*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Bowed(a) *(stk::Bowed**)(a->data)
static SFUN(gw_Bowed_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Bowed * result = (stk::Bowed *)new stk::Bowed(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Bowed);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Bowed");
  GW_Bowed(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Bowed_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Bowed_ctor1) {
  if(o->type_ref == t_Bowed)
  GW_Bowed(o) = new stk::Bowed();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Bowed_tick, GW_Bowed(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Bowed_dtor) {
  if(GW_Bowed(o)) {
    delete (stk::Bowed*)GW_Bowed(o);
    GW_Bowed(o) = NULL;
  }
}

static MFUN(gw_Bowed_clear) {
  stk::Bowed * arg1 = (stk::Bowed *)GW_Bowed((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Bowed_setFrequency) {
  stk::Bowed * arg1 = (stk::Bowed *)GW_Bowed((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Bowed_setVibrato) {
  stk::Bowed * arg1 = (stk::Bowed *)GW_Bowed((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibrato(arg2);
}

static MFUN(gw_Bowed_startBowing) {
  stk::Bowed * arg1 = (stk::Bowed *)GW_Bowed((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBowing(arg2,arg3);
}

static MFUN(gw_Bowed_stopBowing) {
  stk::Bowed * arg1 = (stk::Bowed *)GW_Bowed((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBowing(arg2);
}

static MFUN(gw_Bowed_noteOn) {
  stk::Bowed * arg1 = (stk::Bowed *)GW_Bowed((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Bowed_noteOff) {
  stk::Bowed * arg1 = (stk::Bowed *)GW_Bowed((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Bowed_controlChange) {
  stk::Bowed * arg1 = (stk::Bowed *)GW_Bowed((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(BowTable_tick) {
  stk::BowTable *s = (stk::BowTable*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_BowTable(a) *(stk::BowTable**)(a->data)
static CTOR(gw_BowTable_ctor) {
  if(o->type_ref == t_BowTable)
  GW_BowTable(o) = new stk::BowTable();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BowTable_tick, GW_BowTable(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_BowTable_setOffset) {
  stk::BowTable * arg1 = (stk::BowTable *)GW_BowTable((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setOffset(arg2);
}

static MFUN(gw_BowTable_setSlope) {
  stk::BowTable * arg1 = (stk::BowTable *)GW_BowTable((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSlope(arg2);
}

static MFUN(gw_BowTable_setMinOutput) {
  stk::BowTable * arg1 = (stk::BowTable *)GW_BowTable((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setMinOutput(arg2);
}

static MFUN(gw_BowTable_setMaxOutput) {
  stk::BowTable * arg1 = (stk::BowTable *)GW_BowTable((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setMaxOutput(arg2);
}

static DTOR(gw_BowTable_dtor) {
  if(GW_BowTable(o)) {
    delete (stk::BowTable*)GW_BowTable(o);
    GW_BowTable(o) = NULL;
  }
}

static TICK(Brass_tick) {
  stk::Brass *s = (stk::Brass*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Brass(a) *(stk::Brass**)(a->data)
static SFUN(gw_Brass_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Brass * result = (stk::Brass *)new stk::Brass(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Brass);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Brass");
  GW_Brass(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Brass_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Brass_ctor1) {
  if(o->type_ref == t_Brass)
  GW_Brass(o) = new stk::Brass();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Brass_tick, GW_Brass(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Brass_dtor) {
  if(GW_Brass(o)) {
    delete (stk::Brass*)GW_Brass(o);
    GW_Brass(o) = NULL;
  }
}

static MFUN(gw_Brass_clear) {
  stk::Brass * arg1 = (stk::Brass *)GW_Brass((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Brass_setFrequency) {
  stk::Brass * arg1 = (stk::Brass *)GW_Brass((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Brass_setLip) {
  stk::Brass * arg1 = (stk::Brass *)GW_Brass((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLip(arg2);
}

static MFUN(gw_Brass_startBlowing) {
  stk::Brass * arg1 = (stk::Brass *)GW_Brass((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Brass_stopBlowing) {
  stk::Brass * arg1 = (stk::Brass *)GW_Brass((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Brass_noteOn) {
  stk::Brass * arg1 = (stk::Brass *)GW_Brass((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Brass_noteOff) {
  stk::Brass * arg1 = (stk::Brass *)GW_Brass((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Brass_controlChange) {
  stk::Brass * arg1 = (stk::Brass *)GW_Brass((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Chorus_tick) {
  stk::Chorus *s = (stk::Chorus*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Chorus(a) *(stk::Chorus**)(a->data)
static SFUN(gw_Chorus_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Chorus * result = (stk::Chorus *)new stk::Chorus(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Chorus);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Chorus");
  GW_Chorus(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Chorus_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Chorus_ctor1) {
  if(o->type_ref == t_Chorus)
  GW_Chorus(o) = new stk::Chorus();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Chorus_tick, GW_Chorus(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_Chorus_clear) {
  stk::Chorus * arg1 = (stk::Chorus *)GW_Chorus((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Chorus_setModDepth) {
  stk::Chorus * arg1 = (stk::Chorus *)GW_Chorus((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModDepth(arg2);
}

static MFUN(gw_Chorus_setModFrequency) {
  stk::Chorus * arg1 = (stk::Chorus *)GW_Chorus((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModFrequency(arg2);
}

static DTOR(gw_Chorus_dtor) {
  if(GW_Chorus(o)) {
    delete (stk::Chorus*)GW_Chorus(o);
    GW_Chorus(o) = NULL;
  }
}

static TICK(Clarinet_tick) {
  stk::Clarinet *s = (stk::Clarinet*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Clarinet(a) *(stk::Clarinet**)(a->data)
static SFUN(gw_Clarinet_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Clarinet * result = (stk::Clarinet *)new stk::Clarinet(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Clarinet);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Clarinet");
  GW_Clarinet(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Clarinet_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Clarinet_ctor1) {
  if(o->type_ref == t_Clarinet)
  GW_Clarinet(o) = new stk::Clarinet();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Clarinet_tick, GW_Clarinet(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Clarinet_dtor) {
  if(GW_Clarinet(o)) {
    delete (stk::Clarinet*)GW_Clarinet(o);
    GW_Clarinet(o) = NULL;
  }
}

static MFUN(gw_Clarinet_clear) {
  stk::Clarinet * arg1 = (stk::Clarinet *)GW_Clarinet((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Clarinet_setFrequency) {
  stk::Clarinet * arg1 = (stk::Clarinet *)GW_Clarinet((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Clarinet_startBlowing) {
  stk::Clarinet * arg1 = (stk::Clarinet *)GW_Clarinet((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Clarinet_stopBlowing) {
  stk::Clarinet * arg1 = (stk::Clarinet *)GW_Clarinet((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Clarinet_noteOn) {
  stk::Clarinet * arg1 = (stk::Clarinet *)GW_Clarinet((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Clarinet_noteOff) {
  stk::Clarinet * arg1 = (stk::Clarinet *)GW_Clarinet((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Clarinet_controlChange) {
  stk::Clarinet * arg1 = (stk::Clarinet *)GW_Clarinet((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Cubic_tick) {
  stk::Cubic *s = (stk::Cubic*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Cubic(a) *(stk::Cubic**)(a->data)
static CTOR(gw_Cubic_ctor) {
  if(o->type_ref == t_Cubic)
  GW_Cubic(o) = new stk::Cubic();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Cubic_tick, GW_Cubic(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_Cubic_setA1) {
  stk::Cubic * arg1 = (stk::Cubic *)GW_Cubic((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_Cubic_setA2) {
  stk::Cubic * arg1 = (stk::Cubic *)GW_Cubic((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA2(arg2);
}

static MFUN(gw_Cubic_setA3) {
  stk::Cubic * arg1 = (stk::Cubic *)GW_Cubic((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA3(arg2);
}

static MFUN(gw_Cubic_setGain) {
  stk::Cubic * arg1 = (stk::Cubic *)GW_Cubic((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setGain(arg2);
}

static MFUN(gw_Cubic_setThreshold) {
  stk::Cubic * arg1 = (stk::Cubic *)GW_Cubic((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setThreshold(arg2);
}

static DTOR(gw_Cubic_dtor) {
  if(GW_Cubic(o)) {
    delete (stk::Cubic*)GW_Cubic(o);
    GW_Cubic(o) = NULL;
  }
}

static TICK(Delay_tick) {
  stk::Delay *s = (stk::Delay*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Delay(a) *(stk::Delay**)(a->data)
static SFUN(gw_Delay_ctor0) {
  if(!*(M_Object*)MEM(0))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg1 = (unsigned long)*(m_int*)MEM(0);
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  stk::Delay * result = (stk::Delay *)new stk::Delay(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_Delay);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Delay");
  GW_Delay(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Delay_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static SFUN(gw_Delay_ctor1) {
  if(!*(M_Object*)MEM(0))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg1 = (unsigned long)*(m_int*)MEM(0);
  stk::Delay * result = (stk::Delay *)new stk::Delay(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Delay);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Delay");
  GW_Delay(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Delay_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Delay_ctor2) {
  if(o->type_ref == t_Delay)
  GW_Delay(o) = new stk::Delay();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Delay_tick, GW_Delay(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Delay_dtor) {
  if(GW_Delay(o)) {
    delete (stk::Delay*)GW_Delay(o);
    GW_Delay(o) = NULL;
  }
}

static MFUN(gw_Delay_getMaximumDelay) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  unsigned long result = (unsigned long)(arg1)->getMaximumDelay();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Delay_setMaximumDelay) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setMaximumDelay(arg2);
}

static MFUN(gw_Delay_setDelay) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setDelay(arg2);
}

static MFUN(gw_Delay_getDelay) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  unsigned long result = (unsigned long)((stk::Delay const *)arg1)->getDelay();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Delay_tapOut) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)(arg1)->tapOut(arg2);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Delay_tapIn) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  if(!*(M_Object*)MEM(0+SZ_INT+SZ_FLOAT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg3 = (unsigned long)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->tapIn(arg2,arg3);
}

static MFUN(gw_Delay_addTo) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  if(!*(M_Object*)MEM(0+SZ_INT+SZ_FLOAT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg3 = (unsigned long)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat result = (stk::StkFloat)(arg1)->addTo(arg2,arg3);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Delay_nextOut) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->nextOut();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Delay_energy) {
  stk::Delay * arg1 = (stk::Delay *)GW_Delay((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)((stk::Delay const *)arg1)->energy();
  *(m_float*)RETURN = (m_float)(float)result;
}

static TICK(DelayA_tick) {
  stk::DelayA *s = (stk::DelayA*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_DelayA(a) *(stk::DelayA**)(a->data)
static SFUN(gw_DelayA_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  if(!*(M_Object*)MEM(0+SZ_FLOAT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_FLOAT);
  stk::DelayA * result = (stk::DelayA *)new stk::DelayA(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_DelayA);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.DelayA");
  GW_DelayA(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), DelayA_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static SFUN(gw_DelayA_ctor1) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::DelayA * result = (stk::DelayA *)new stk::DelayA(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_DelayA);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.DelayA");
  GW_DelayA(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), DelayA_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_DelayA_ctor2) {
  if(o->type_ref == t_DelayA)
  GW_DelayA(o) = new stk::DelayA();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), DelayA_tick, GW_DelayA(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_DelayA_dtor) {
  if(GW_DelayA(o)) {
    delete (stk::DelayA*)GW_DelayA(o);
    GW_DelayA(o) = NULL;
  }
}

static MFUN(gw_DelayA_clear) {
  stk::DelayA * arg1 = (stk::DelayA *)GW_DelayA((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_DelayA_getMaximumDelay) {
  stk::DelayA * arg1 = (stk::DelayA *)GW_DelayA((*(M_Object*)MEM(0)));
  unsigned long result = (unsigned long)(arg1)->getMaximumDelay();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_DelayA_setMaximumDelay) {
  stk::DelayA * arg1 = (stk::DelayA *)GW_DelayA((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setMaximumDelay(arg2);
}

static MFUN(gw_DelayA_setDelay) {
  stk::DelayA * arg1 = (stk::DelayA *)GW_DelayA((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDelay(arg2);
}

static MFUN(gw_DelayA_getDelay) {
  stk::DelayA * arg1 = (stk::DelayA *)GW_DelayA((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)((stk::DelayA const *)arg1)->getDelay();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_DelayA_tapOut) {
  stk::DelayA * arg1 = (stk::DelayA *)GW_DelayA((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)(arg1)->tapOut(arg2);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_DelayA_tapIn) {
  stk::DelayA * arg1 = (stk::DelayA *)GW_DelayA((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  if(!*(M_Object*)MEM(0+SZ_INT+SZ_FLOAT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg3 = (unsigned long)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->tapIn(arg2,arg3);
}

static MFUN(gw_DelayA_nextOut) {
  stk::DelayA * arg1 = (stk::DelayA *)GW_DelayA((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->nextOut();
  *(m_float*)RETURN = (m_float)(float)result;
}

static TICK(DelayL_tick) {
  stk::DelayL *s = (stk::DelayL*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_DelayL(a) *(stk::DelayL**)(a->data)
static SFUN(gw_DelayL_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  if(!*(M_Object*)MEM(0+SZ_FLOAT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_FLOAT);
  stk::DelayL * result = (stk::DelayL *)new stk::DelayL(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_DelayL);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.DelayL");
  GW_DelayL(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), DelayL_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static SFUN(gw_DelayL_ctor1) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::DelayL * result = (stk::DelayL *)new stk::DelayL(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_DelayL);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.DelayL");
  GW_DelayL(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), DelayL_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_DelayL_ctor2) {
  if(o->type_ref == t_DelayL)
  GW_DelayL(o) = new stk::DelayL();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), DelayL_tick, GW_DelayL(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_DelayL_dtor) {
  if(GW_DelayL(o)) {
    delete (stk::DelayL*)GW_DelayL(o);
    GW_DelayL(o) = NULL;
  }
}

static MFUN(gw_DelayL_getMaximumDelay) {
  stk::DelayL * arg1 = (stk::DelayL *)GW_DelayL((*(M_Object*)MEM(0)));
  unsigned long result = (unsigned long)(arg1)->getMaximumDelay();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_DelayL_setMaximumDelay) {
  stk::DelayL * arg1 = (stk::DelayL *)GW_DelayL((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setMaximumDelay(arg2);
}

static MFUN(gw_DelayL_setDelay) {
  stk::DelayL * arg1 = (stk::DelayL *)GW_DelayL((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDelay(arg2);
}

static MFUN(gw_DelayL_getDelay) {
  stk::DelayL * arg1 = (stk::DelayL *)GW_DelayL((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)((stk::DelayL const *)arg1)->getDelay();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_DelayL_tapOut) {
  stk::DelayL * arg1 = (stk::DelayL *)GW_DelayL((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)(arg1)->tapOut(arg2);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_DelayL_tapIn) {
  stk::DelayL * arg1 = (stk::DelayL *)GW_DelayL((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  if(!*(M_Object*)MEM(0+SZ_INT+SZ_FLOAT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg3 = (unsigned long)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->tapIn(arg2,arg3);
}

static MFUN(gw_DelayL_nextOut) {
  stk::DelayL * arg1 = (stk::DelayL *)GW_DelayL((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->nextOut();
  *(m_float*)RETURN = (m_float)(float)result;
}

static TICK(Drummer_tick) {
  stk::Drummer *s = (stk::Drummer*)u->module.gen.data;
  u->out = s->tick(1);
}

static SFUN(gw_DRUM_NUMWAVES_get) {
  int result = (int)(int)stk::DRUM_NUMWAVES;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_DRUM_POLYPHONY_get) {
  int result = (int)(int)stk::DRUM_POLYPHONY;
  *(m_int*)RETURN = (m_int)result;
}

#define GW_Drummer(a) *(stk::Drummer**)(a->data)
static CTOR(gw_Drummer_ctor) {
  if(o->type_ref == t_Drummer)
  GW_Drummer(o) = new stk::Drummer();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Drummer_tick, GW_Drummer(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Drummer_dtor) {
  if(GW_Drummer(o)) {
    delete (stk::Drummer*)GW_Drummer(o);
    GW_Drummer(o) = NULL;
  }
}

static MFUN(gw_Drummer_noteOn) {
  stk::Drummer * arg1 = (stk::Drummer *)GW_Drummer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Drummer_noteOff) {
  stk::Drummer * arg1 = (stk::Drummer *)GW_Drummer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static TICK(Echo_tick) {
  stk::Echo *s = (stk::Echo*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Echo(a) *(stk::Echo**)(a->data)
static SFUN(gw_Echo_ctor0) {
  unsigned long arg1 = (unsigned long)*(m_int*)MEM(0);
  stk::Echo * result = (stk::Echo *)new stk::Echo(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Echo);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Echo");
  GW_Echo(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Echo_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Echo_ctor1) {
  if(o->type_ref == t_Echo)
  GW_Echo(o) = new stk::Echo();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Echo_tick, GW_Echo(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_Echo_clear) {
  stk::Echo * arg1 = (stk::Echo *)GW_Echo((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Echo_setMaximumDelay) {
  stk::Echo * arg1 = (stk::Echo *)GW_Echo((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setMaximumDelay(arg2);
}

static MFUN(gw_Echo_setDelay) {
  stk::Echo * arg1 = (stk::Echo *)GW_Echo((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned long arg2 = (unsigned long)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setDelay(arg2);
}

static DTOR(gw_Echo_dtor) {
  if(GW_Echo(o)) {
    delete (stk::Echo*)GW_Echo(o);
    GW_Echo(o) = NULL;
  }
}

static TICK(Flute_tick) {
  stk::Flute *s = (stk::Flute*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Flute(a) *(stk::Flute**)(a->data)
static SFUN(gw_Flute_ctor) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Flute * result = (stk::Flute *)new stk::Flute(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Flute);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Flute");
  GW_Flute(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Flute_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static DTOR(gw_Flute_dtor) {
  if(GW_Flute(o)) {
    delete (stk::Flute*)GW_Flute(o);
    GW_Flute(o) = NULL;
  }
}

static MFUN(gw_Flute_clear) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Flute_setFrequency) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Flute_setJetReflection) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setJetReflection(arg2);
}

static MFUN(gw_Flute_setEndReflection) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setEndReflection(arg2);
}

static MFUN(gw_Flute_setJetDelay) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setJetDelay(arg2);
}

static MFUN(gw_Flute_startBlowing) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Flute_stopBlowing) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Flute_noteOn) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Flute_noteOff) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Flute_controlChange) {
  stk::Flute * arg1 = (stk::Flute *)GW_Flute((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(FMVoices_tick) {
  stk::FMVoices *s = (stk::FMVoices*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_FMVoices(a) *(stk::FMVoices**)(a->data)
static CTOR(gw_FMVoices_ctor) {
  if(o->type_ref == t_FMVoices)
  GW_FMVoices(o) = new stk::FMVoices();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), FMVoices_tick, GW_FMVoices(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_FMVoices_dtor) {
  if(GW_FMVoices(o)) {
    delete (stk::FMVoices*)GW_FMVoices(o);
    GW_FMVoices(o) = NULL;
  }
}

static MFUN(gw_FMVoices_setFrequency) {
  stk::FMVoices * arg1 = (stk::FMVoices *)GW_FMVoices((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_FMVoices_noteOn) {
  stk::FMVoices * arg1 = (stk::FMVoices *)GW_FMVoices((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_FMVoices_controlChange) {
  stk::FMVoices * arg1 = (stk::FMVoices *)GW_FMVoices((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(FormSwep_tick) {
  stk::FormSwep *s = (stk::FormSwep*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_FormSwep(a) *(stk::FormSwep**)(a->data)
static CTOR(gw_FormSwep_ctor) {
  if(o->type_ref == t_FormSwep)
  GW_FormSwep(o) = new stk::FormSwep();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), FormSwep_tick, GW_FormSwep(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_FormSwep_dtor) {
  if(GW_FormSwep(o)) {
    delete (stk::FormSwep*)GW_FormSwep(o);
    GW_FormSwep(o) = NULL;
  }
}

static MFUN(gw_FormSwep_ignoreSampleRateChange0) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_FormSwep_ignoreSampleRateChange1) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_FormSwep_setResonance) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3);
}

static MFUN(gw_FormSwep_setStates0) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setStates(arg2,arg3,arg4);
}

static MFUN(gw_FormSwep_setStates1) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setStates(arg2,arg3);
}

static MFUN(gw_FormSwep_setTargets0) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setTargets(arg2,arg3,arg4);
}

static MFUN(gw_FormSwep_setTargets1) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setTargets(arg2,arg3);
}

static MFUN(gw_FormSwep_setSweepRate) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSweepRate(arg2);
}

static MFUN(gw_FormSwep_setSweepTime) {
  stk::FormSwep * arg1 = (stk::FormSwep *)GW_FormSwep((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSweepTime(arg2);
}

static TICK(FreeVerb_tick) {
  stk::FreeVerb *s = (stk::FreeVerb*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_FreeVerb(a) *(stk::FreeVerb**)(a->data)
static CTOR(gw_FreeVerb_ctor) {
  if(o->type_ref == t_FreeVerb)
  GW_FreeVerb(o) = new stk::FreeVerb();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), FreeVerb_tick, GW_FreeVerb(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_FreeVerb_dtor) {
  if(GW_FreeVerb(o)) {
    delete (stk::FreeVerb*)GW_FreeVerb(o);
    GW_FreeVerb(o) = NULL;
  }
}

static MFUN(gw_FreeVerb_setEffectMix) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setEffectMix(arg2);
}

static MFUN(gw_FreeVerb_setRoomSize) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRoomSize(arg2);
}

static MFUN(gw_FreeVerb_getRoomSize) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->getRoomSize();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_FreeVerb_setDamping) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDamping(arg2);
}

static MFUN(gw_FreeVerb_getDamping) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->getDamping();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_FreeVerb_setWidth) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setWidth(arg2);
}

static MFUN(gw_FreeVerb_getWidth) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->getWidth();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_FreeVerb_setMode) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setMode(arg2);
}

static MFUN(gw_FreeVerb_getMode) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->getMode();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_FreeVerb_clear) {
  stk::FreeVerb * arg1 = (stk::FreeVerb *)GW_FreeVerb((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static TICK(Granulate_tick) {
  stk::Granulate *s = (stk::Granulate*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Granulate(a) *(stk::Granulate**)(a->data)
static CTOR(gw_Granulate_ctor0) {
  if(o->type_ref == t_Granulate)
  GW_Granulate(o) = new stk::Granulate();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Granulate_tick, GW_Granulate(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static SFUN(gw_Granulate_ctor1) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  std::string arg2(STRING(*(M_Object*)MEM(0+SZ_INT))); // here arg2
  bool arg3 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  stk::Granulate * result = (stk::Granulate *)new stk::Granulate(arg1,arg2,arg3);
  //M_Object ret_obj = new_object(shred->info->mp, t_Granulate);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Granulate");
  GW_Granulate(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Granulate_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static SFUN(gw_Granulate_ctor2) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  std::string arg2(STRING(*(M_Object*)MEM(0+SZ_INT))); // here arg2
  stk::Granulate * result = (stk::Granulate *)new stk::Granulate(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_Granulate);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Granulate");
  GW_Granulate(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Granulate_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static DTOR(gw_Granulate_dtor) {
  if(GW_Granulate(o)) {
    delete (stk::Granulate*)GW_Granulate(o);
    GW_Granulate(o) = NULL;
  }
}

static MFUN(gw_Granulate_openFile0) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  std::string arg2(STRING(*(M_Object*)MEM(0+SZ_INT))); // here arg2
  bool arg3 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->openFile(arg2,arg3);
}

static MFUN(gw_Granulate_openFile1) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  std::string arg2(STRING(*(M_Object*)MEM(0+SZ_INT))); // here arg2
  (arg1)->openFile(arg2);
}

static MFUN(gw_Granulate_reset) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  (arg1)->reset();
}

static MFUN(gw_Granulate_setVoices0) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setVoices(arg2);
}

static MFUN(gw_Granulate_setVoices1) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  (arg1)->setVoices();
}

static MFUN(gw_Granulate_setStretch0) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setStretch(arg2);
}

static MFUN(gw_Granulate_setStretch1) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  (arg1)->setStretch();
}

static MFUN(gw_Granulate_setGrainParameters0) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  unsigned int arg5 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_INT+SZ_INT);
  (arg1)->setGrainParameters(arg2,arg3,arg4,arg5);
}

static MFUN(gw_Granulate_setGrainParameters1) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  (arg1)->setGrainParameters(arg2,arg3,arg4);
}

static MFUN(gw_Granulate_setGrainParameters2) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setGrainParameters(arg2,arg3);
}

static MFUN(gw_Granulate_setGrainParameters3) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setGrainParameters(arg2);
}

static MFUN(gw_Granulate_setGrainParameters4) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  (arg1)->setGrainParameters();
}

static MFUN(gw_Granulate_setRandomFactor0) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRandomFactor(arg2);
}

static MFUN(gw_Granulate_setRandomFactor1) {
  stk::Granulate * arg1 = (stk::Granulate *)GW_Granulate((*(M_Object*)MEM(0)));
  (arg1)->setRandomFactor();
}

static TICK(Guitar_tick) {
  stk::Guitar *s = (stk::Guitar*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Guitar(a) *(stk::Guitar**)(a->data)
static SFUN(gw_Guitar_ctor0) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  std::string arg2(STRING(*(M_Object*)MEM(0+SZ_INT))); // here arg2
  stk::Guitar * result = (stk::Guitar *)new stk::Guitar(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_Guitar);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Guitar");
  GW_Guitar(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Guitar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static SFUN(gw_Guitar_ctor1) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  stk::Guitar * result = (stk::Guitar *)new stk::Guitar(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Guitar);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Guitar");
  GW_Guitar(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Guitar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Guitar_ctor2) {
  if(o->type_ref == t_Guitar)
  GW_Guitar(o) = new stk::Guitar();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Guitar_tick, GW_Guitar(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_Guitar_clear) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Guitar_setBodyFile0) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  std::string arg2(STRING(*(M_Object*)MEM(0+SZ_INT))); // here arg2
  (arg1)->setBodyFile(arg2);
}

static MFUN(gw_Guitar_setBodyFile1) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  (arg1)->setBodyFile();
}

static MFUN(gw_Guitar_setPluckPosition0) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setPluckPosition(arg2,arg3);
}

static MFUN(gw_Guitar_setPluckPosition1) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPluckPosition(arg2);
}

static MFUN(gw_Guitar_setLoopGain0) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setLoopGain(arg2,arg3);
}

static MFUN(gw_Guitar_setLoopGain1) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLoopGain(arg2);
}

static MFUN(gw_Guitar_setFrequency0) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setFrequency(arg2,arg3);
}

static MFUN(gw_Guitar_setFrequency1) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Guitar_noteOn0) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  unsigned int arg4 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3,arg4);
}

static MFUN(gw_Guitar_noteOn1) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Guitar_noteOff0) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOff(arg2,arg3);
}

static MFUN(gw_Guitar_noteOff1) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Guitar_controlChange0) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_FLOAT);
  (arg1)->controlChange(arg2,arg3,arg4);
}

static MFUN(gw_Guitar_controlChange1) {
  stk::Guitar * arg1 = (stk::Guitar *)GW_Guitar((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static DTOR(gw_Guitar_dtor) {
  if(GW_Guitar(o)) {
    delete (stk::Guitar*)GW_Guitar(o);
    GW_Guitar(o) = NULL;
  }
}

static TICK(HevyMetl_tick) {
  stk::HevyMetl *s = (stk::HevyMetl*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_HevyMetl(a) *(stk::HevyMetl**)(a->data)
static CTOR(gw_HevyMetl_ctor) {
  if(o->type_ref == t_HevyMetl)
  GW_HevyMetl(o) = new stk::HevyMetl();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), HevyMetl_tick, GW_HevyMetl(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_HevyMetl_dtor) {
  if(GW_HevyMetl(o)) {
    delete (stk::HevyMetl*)GW_HevyMetl(o);
    GW_HevyMetl(o) = NULL;
  }
}

static MFUN(gw_HevyMetl_noteOn) {
  stk::HevyMetl * arg1 = (stk::HevyMetl *)GW_HevyMetl((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static TICK(JCRev_tick) {
  stk::JCRev *s = (stk::JCRev*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_JCRev(a) *(stk::JCRev**)(a->data)
static SFUN(gw_JCRev_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::JCRev * result = (stk::JCRev *)new stk::JCRev(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_JCRev);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.JCRev");
  GW_JCRev(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), JCRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_JCRev_ctor1) {
  if(o->type_ref == t_JCRev)
  GW_JCRev(o) = new stk::JCRev();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), JCRev_tick, GW_JCRev(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_JCRev_clear) {
  stk::JCRev * arg1 = (stk::JCRev *)GW_JCRev((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_JCRev_setT60) {
  stk::JCRev * arg1 = (stk::JCRev *)GW_JCRev((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setT60(arg2);
}

static DTOR(gw_JCRev_dtor) {
  if(GW_JCRev(o)) {
    delete (stk::JCRev*)GW_JCRev(o);
    GW_JCRev(o) = NULL;
  }
}

static TICK(JetTable_tick) {
  stk::JetTable *s = (stk::JetTable*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_JetTable(a) *(stk::JetTable**)(a->data)
static CTOR(gw_JetTable_ctor) {
  if(o->type_ref == t_JetTable)
  GW_JetTable(o) = new stk::JetTable();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), JetTable_tick, GW_JetTable(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_JetTable_dtor) {
  if(GW_JetTable(o)) {
    delete (stk::JetTable*)GW_JetTable(o);
    GW_JetTable(o) = NULL;
  }
}

static TICK(LentPitShift_tick) {
  stk::LentPitShift *s = (stk::LentPitShift*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_LentPitShift(a) *(stk::LentPitShift**)(a->data)
static SFUN(gw_LentPitShift_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  int arg2 = (int)*(m_int*)MEM(0+SZ_FLOAT);
  stk::LentPitShift * result = (stk::LentPitShift *)new stk::LentPitShift(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_LentPitShift);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.LentPitShift");
  GW_LentPitShift(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), LentPitShift_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static SFUN(gw_LentPitShift_ctor1) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::LentPitShift * result = (stk::LentPitShift *)new stk::LentPitShift(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_LentPitShift);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.LentPitShift");
  GW_LentPitShift(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), LentPitShift_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_LentPitShift_ctor2) {
  if(o->type_ref == t_LentPitShift)
  GW_LentPitShift(o) = new stk::LentPitShift();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), LentPitShift_tick, GW_LentPitShift(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_LentPitShift_dtor) {
  if(GW_LentPitShift(o)) {
    delete (stk::LentPitShift*)GW_LentPitShift(o);
    GW_LentPitShift(o) = NULL;
  }
}

static MFUN(gw_LentPitShift_clear) {
  stk::LentPitShift * arg1 = (stk::LentPitShift *)GW_LentPitShift((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_LentPitShift_setShift) {
  stk::LentPitShift * arg1 = (stk::LentPitShift *)GW_LentPitShift((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setShift(arg2);
}

static TICK(Mandolin_tick) {
  stk::Mandolin *s = (stk::Mandolin*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Mandolin(a) *(stk::Mandolin**)(a->data)
static SFUN(gw_Mandolin_ctor) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Mandolin * result = (stk::Mandolin *)new stk::Mandolin(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Mandolin);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Mandolin");
  GW_Mandolin(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Mandolin_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static DTOR(gw_Mandolin_dtor) {
  if(GW_Mandolin(o)) {
    delete (stk::Mandolin*)GW_Mandolin(o);
    GW_Mandolin(o) = NULL;
  }
}

static MFUN(gw_Mandolin_clear) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Mandolin_setDetune) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDetune(arg2);
}

static MFUN(gw_Mandolin_setBodySize) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBodySize(arg2);
}

static MFUN(gw_Mandolin_setPluckPosition) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPluckPosition(arg2);
}

static MFUN(gw_Mandolin_setFrequency) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Mandolin_pluck0) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_Mandolin_pluck1) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->pluck(arg2,arg3);
}

static MFUN(gw_Mandolin_noteOn) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Mandolin_noteOff) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Mandolin_controlChange) {
  stk::Mandolin * arg1 = (stk::Mandolin *)GW_Mandolin((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Mesh2D_tick) {
  stk::Mesh2D *s = (stk::Mesh2D*)u->module.gen.data;
  u->out = s->tick(1);
}

static SFUN(gw_NXMAX_get) {
  unsigned short result = (unsigned short)(unsigned short)stk::NXMAX;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_NYMAX_get) {
  unsigned short result = (unsigned short)(unsigned short)stk::NYMAX;
  *(m_int*)RETURN = (m_int)result;
}

#define GW_Mesh2D(a) *(stk::Mesh2D**)(a->data)
static SFUN(gw_Mesh2D_ctor) {
  if(!*(M_Object*)MEM(0))
  handle(shred, (m_str)"NullPtrException");
  unsigned short arg1 = (unsigned short)*(m_int*)MEM(0);
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned short arg2 = (unsigned short)*(m_int*)MEM(0+SZ_INT);
  stk::Mesh2D * result = (stk::Mesh2D *)new stk::Mesh2D(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_Mesh2D);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Mesh2D");
  GW_Mesh2D(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Mesh2D_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static DTOR(gw_Mesh2D_dtor) {
  if(GW_Mesh2D(o)) {
    delete (stk::Mesh2D*)GW_Mesh2D(o);
    GW_Mesh2D(o) = NULL;
  }
}

static MFUN(gw_Mesh2D_clear) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Mesh2D_setNX) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned short arg2 = (unsigned short)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setNX(arg2);
}

static MFUN(gw_Mesh2D_setNY) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  unsigned short arg2 = (unsigned short)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setNY(arg2);
}

static MFUN(gw_Mesh2D_setInputPosition) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setInputPosition(arg2,arg3);
}

static MFUN(gw_Mesh2D_setDecay) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDecay(arg2);
}

static MFUN(gw_Mesh2D_noteOn) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Mesh2D_noteOff) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Mesh2D_energy) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->energy();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Mesh2D_inputTick) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)(arg1)->inputTick(arg2);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Mesh2D_controlChange) {
  stk::Mesh2D * arg1 = (stk::Mesh2D *)GW_Mesh2D((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Modal_tick) {
  stk::Modal *s = (stk::Modal*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Modal(a) *(stk::Modal**)(a->data)
static DTOR(gw_Modal_dtor) {
  if(GW_Modal(o)) {
    delete (stk::Modal*)GW_Modal(o);
    GW_Modal(o) = NULL;
  }
}

static MFUN(gw_Modal_clear) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Modal_setFrequency) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Modal_setRatioAndRadius) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT+SZ_FLOAT);
  (arg1)->setRatioAndRadius(arg2,arg3,arg4);
}

static MFUN(gw_Modal_setMasterGain) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setMasterGain(arg2);
}

static MFUN(gw_Modal_setDirectGain) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDirectGain(arg2);
}

static MFUN(gw_Modal_setModeGain) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setModeGain(arg2,arg3);
}

static MFUN(gw_Modal_strike) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->strike(arg2);
}

static MFUN(gw_Modal_damp) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->damp(arg2);
}

static MFUN(gw_Modal_noteOn) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Modal_noteOff) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Modal_controlChange) {
  stk::Modal * arg1 = (stk::Modal *)GW_Modal((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(ModalBar_tick) {
  stk::ModalBar *s = (stk::ModalBar*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_ModalBar(a) *(stk::ModalBar**)(a->data)
static CTOR(gw_ModalBar_ctor) {
  if(o->type_ref == t_ModalBar)
  GW_ModalBar(o) = new stk::ModalBar();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), ModalBar_tick, GW_ModalBar(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_ModalBar_dtor) {
  if(GW_ModalBar(o)) {
    delete (stk::ModalBar*)GW_ModalBar(o);
    GW_ModalBar(o) = NULL;
  }
}

static MFUN(gw_ModalBar_setStickHardness) {
  stk::ModalBar * arg1 = (stk::ModalBar *)GW_ModalBar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setStickHardness(arg2);
}

static MFUN(gw_ModalBar_setStrikePosition) {
  stk::ModalBar * arg1 = (stk::ModalBar *)GW_ModalBar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setStrikePosition(arg2);
}

static MFUN(gw_ModalBar_setPreset) {
  stk::ModalBar * arg1 = (stk::ModalBar *)GW_ModalBar((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setPreset(arg2);
}

static MFUN(gw_ModalBar_setModulationDepth) {
  stk::ModalBar * arg1 = (stk::ModalBar *)GW_ModalBar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationDepth(arg2);
}

static MFUN(gw_ModalBar_controlChange) {
  stk::ModalBar * arg1 = (stk::ModalBar *)GW_ModalBar((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Modulate_tick) {
  stk::Modulate *s = (stk::Modulate*)u->module.gen.data;
  u->out = s->tick();
}

#define GW_Modulate(a) *(stk::Modulate**)(a->data)
static CTOR(gw_Modulate_ctor) {
  if(o->type_ref == t_Modulate)
  GW_Modulate(o) = new stk::Modulate();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Modulate_tick, GW_Modulate(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Modulate_dtor) {
  if(GW_Modulate(o)) {
    delete (stk::Modulate*)GW_Modulate(o);
    GW_Modulate(o) = NULL;
  }
}

static MFUN(gw_Modulate_reset) {
  stk::Modulate * arg1 = (stk::Modulate *)GW_Modulate((*(M_Object*)MEM(0)));
  (arg1)->reset();
}

static MFUN(gw_Modulate_setVibratoRate) {
  stk::Modulate * arg1 = (stk::Modulate *)GW_Modulate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoRate(arg2);
}

static MFUN(gw_Modulate_setVibratoGain) {
  stk::Modulate * arg1 = (stk::Modulate *)GW_Modulate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoGain(arg2);
}

static MFUN(gw_Modulate_setRandomRate) {
  stk::Modulate * arg1 = (stk::Modulate *)GW_Modulate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRandomRate(arg2);
}

static MFUN(gw_Modulate_setRandomGain) {
  stk::Modulate * arg1 = (stk::Modulate *)GW_Modulate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRandomGain(arg2);
}

static TICK(Moog_tick) {
  stk::Moog *s = (stk::Moog*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Moog(a) *(stk::Moog**)(a->data)
static CTOR(gw_Moog_ctor) {
  if(o->type_ref == t_Moog)
  GW_Moog(o) = new stk::Moog();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Moog_tick, GW_Moog(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Moog_dtor) {
  if(GW_Moog(o)) {
    delete (stk::Moog*)GW_Moog(o);
    GW_Moog(o) = NULL;
  }
}

static MFUN(gw_Moog_setFrequency) {
  stk::Moog * arg1 = (stk::Moog *)GW_Moog((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Moog_noteOn) {
  stk::Moog * arg1 = (stk::Moog *)GW_Moog((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Moog_setModulationSpeed) {
  stk::Moog * arg1 = (stk::Moog *)GW_Moog((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationSpeed(arg2);
}

static MFUN(gw_Moog_setModulationDepth) {
  stk::Moog * arg1 = (stk::Moog *)GW_Moog((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationDepth(arg2);
}

static MFUN(gw_Moog_controlChange) {
  stk::Moog * arg1 = (stk::Moog *)GW_Moog((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Noise_tick) {
  stk::Noise *s = (stk::Noise*)u->module.gen.data;
  u->out = s->tick();
}

#define GW_Noise(a) *(stk::Noise**)(a->data)
static SFUN(gw_Noise_ctor0) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  stk::Noise * result = (stk::Noise *)new stk::Noise(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Noise);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Noise");
  GW_Noise(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Noise_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Noise_ctor1) {
  if(o->type_ref == t_Noise)
  GW_Noise(o) = new stk::Noise();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Noise_tick, GW_Noise(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_Noise_setSeed0) {
  stk::Noise * arg1 = (stk::Noise *)GW_Noise((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setSeed(arg2);
}

static MFUN(gw_Noise_setSeed1) {
  stk::Noise * arg1 = (stk::Noise *)GW_Noise((*(M_Object*)MEM(0)));
  (arg1)->setSeed();
}

static DTOR(gw_Noise_dtor) {
  if(GW_Noise(o)) {
    delete (stk::Noise*)GW_Noise(o);
    GW_Noise(o) = NULL;
  }
}

static TICK(NRev_tick) {
  stk::NRev *s = (stk::NRev*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_NRev(a) *(stk::NRev**)(a->data)
static SFUN(gw_NRev_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::NRev * result = (stk::NRev *)new stk::NRev(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_NRev);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.NRev");
  GW_NRev(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), NRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_NRev_ctor1) {
  if(o->type_ref == t_NRev)
  GW_NRev(o) = new stk::NRev();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), NRev_tick, GW_NRev(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_NRev_clear) {
  stk::NRev * arg1 = (stk::NRev *)GW_NRev((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_NRev_setT60) {
  stk::NRev * arg1 = (stk::NRev *)GW_NRev((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setT60(arg2);
}

static DTOR(gw_NRev_dtor) {
  if(GW_NRev(o)) {
    delete (stk::NRev*)GW_NRev(o);
    GW_NRev(o) = NULL;
  }
}

static TICK(OnePole_tick) {
  stk::OnePole *s = (stk::OnePole*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_OnePole(a) *(stk::OnePole**)(a->data)
static SFUN(gw_OnePole_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::OnePole * result = (stk::OnePole *)new stk::OnePole(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_OnePole);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.OnePole");
  GW_OnePole(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), OnePole_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_OnePole_ctor1) {
  if(o->type_ref == t_OnePole)
  GW_OnePole(o) = new stk::OnePole();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), OnePole_tick, GW_OnePole(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_OnePole_dtor) {
  if(GW_OnePole(o)) {
    delete (stk::OnePole*)GW_OnePole(o);
    GW_OnePole(o) = NULL;
  }
}

static MFUN(gw_OnePole_setB0) {
  stk::OnePole * arg1 = (stk::OnePole *)GW_OnePole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_OnePole_setA1) {
  stk::OnePole * arg1 = (stk::OnePole *)GW_OnePole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_OnePole_setCoefficients0) {
  stk::OnePole * arg1 = (stk::OnePole *)GW_OnePole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  bool arg4 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_OnePole_setCoefficients1) {
  stk::OnePole * arg1 = (stk::OnePole *)GW_OnePole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3);
}

static MFUN(gw_OnePole_setPole) {
  stk::OnePole * arg1 = (stk::OnePole *)GW_OnePole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPole(arg2);
}

static TICK(OneZero_tick) {
  stk::OneZero *s = (stk::OneZero*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_OneZero(a) *(stk::OneZero**)(a->data)
static SFUN(gw_OneZero_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::OneZero * result = (stk::OneZero *)new stk::OneZero(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_OneZero);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.OneZero");
  GW_OneZero(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), OneZero_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_OneZero_ctor1) {
  if(o->type_ref == t_OneZero)
  GW_OneZero(o) = new stk::OneZero();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), OneZero_tick, GW_OneZero(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_OneZero_dtor) {
  if(GW_OneZero(o)) {
    delete (stk::OneZero*)GW_OneZero(o);
    GW_OneZero(o) = NULL;
  }
}

static MFUN(gw_OneZero_setB0) {
  stk::OneZero * arg1 = (stk::OneZero *)GW_OneZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_OneZero_setB1) {
  stk::OneZero * arg1 = (stk::OneZero *)GW_OneZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB1(arg2);
}

static MFUN(gw_OneZero_setCoefficients0) {
  stk::OneZero * arg1 = (stk::OneZero *)GW_OneZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  bool arg4 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_OneZero_setCoefficients1) {
  stk::OneZero * arg1 = (stk::OneZero *)GW_OneZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3);
}

static MFUN(gw_OneZero_setZero) {
  stk::OneZero * arg1 = (stk::OneZero *)GW_OneZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setZero(arg2);
}

static TICK(PercFlut_tick) {
  stk::PercFlut *s = (stk::PercFlut*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_PercFlut(a) *(stk::PercFlut**)(a->data)
static CTOR(gw_PercFlut_ctor) {
  if(o->type_ref == t_PercFlut)
  GW_PercFlut(o) = new stk::PercFlut();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PercFlut_tick, GW_PercFlut(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_PercFlut_dtor) {
  if(GW_PercFlut(o)) {
    delete (stk::PercFlut*)GW_PercFlut(o);
    GW_PercFlut(o) = NULL;
  }
}

static MFUN(gw_PercFlut_setFrequency) {
  stk::PercFlut * arg1 = (stk::PercFlut *)GW_PercFlut((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_PercFlut_noteOn) {
  stk::PercFlut * arg1 = (stk::PercFlut *)GW_PercFlut((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

#define GW_Phonemes(a) *(stk::Phonemes**)(a->data)
static CTOR(gw_Phonemes_ctor) {
  if(o->type_ref == t_Phonemes)
  GW_Phonemes(o) = new stk::Phonemes();
}

static DTOR(gw_Phonemes_dtor) {
  if(GW_Phonemes(o)) {
    delete (stk::Phonemes*)GW_Phonemes(o);
    GW_Phonemes(o) = NULL;
  }
}

static MFUN(gw_Phonemes_name) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  char * result = (char *)stk::Phonemes::name(arg1);
  *(M_Object*)RETURN = result ? new_string(shred->info->vm->gwion, (m_str)result) : NULL;
}

static MFUN(gw_Phonemes_voiceGain) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::voiceGain(arg1);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Phonemes_noiseGain) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::noiseGain(arg1);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Phonemes_formantFrequency) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::formantFrequency(arg1,arg2);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Phonemes_formantRadius) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::formantRadius(arg1,arg2);
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Phonemes_formantGain) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::formantGain(arg1,arg2);
  *(m_float*)RETURN = (m_float)(float)result;
}

static TICK(PitShift_tick) {
  stk::PitShift *s = (stk::PitShift*)u->module.gen.data;
  u->out = s->tick(1);
}

static SFUN(gw_maxDelay_get) {
  int result = (int)(int)stk::maxDelay;
  *(m_int*)RETURN = (m_int)result;
}

#define GW_PitShift(a) *(stk::PitShift**)(a->data)
static CTOR(gw_PitShift_ctor) {
  if(o->type_ref == t_PitShift)
  GW_PitShift(o) = new stk::PitShift();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PitShift_tick, GW_PitShift(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_PitShift_clear) {
  stk::PitShift * arg1 = (stk::PitShift *)GW_PitShift((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_PitShift_setShift) {
  stk::PitShift * arg1 = (stk::PitShift *)GW_PitShift((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setShift(arg2);
}

static DTOR(gw_PitShift_dtor) {
  if(GW_PitShift(o)) {
    delete (stk::PitShift*)GW_PitShift(o);
    GW_PitShift(o) = NULL;
  }
}

static TICK(Plucked_tick) {
  stk::Plucked *s = (stk::Plucked*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Plucked(a) *(stk::Plucked**)(a->data)
static SFUN(gw_Plucked_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Plucked * result = (stk::Plucked *)new stk::Plucked(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Plucked);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Plucked");
  GW_Plucked(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Plucked_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Plucked_ctor1) {
  if(o->type_ref == t_Plucked)
  GW_Plucked(o) = new stk::Plucked();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Plucked_tick, GW_Plucked(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Plucked_dtor) {
  if(GW_Plucked(o)) {
    delete (stk::Plucked*)GW_Plucked(o);
    GW_Plucked(o) = NULL;
  }
}

static MFUN(gw_Plucked_clear) {
  stk::Plucked * arg1 = (stk::Plucked *)GW_Plucked((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Plucked_setFrequency) {
  stk::Plucked * arg1 = (stk::Plucked *)GW_Plucked((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Plucked_pluck) {
  stk::Plucked * arg1 = (stk::Plucked *)GW_Plucked((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_Plucked_noteOn) {
  stk::Plucked * arg1 = (stk::Plucked *)GW_Plucked((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Plucked_noteOff) {
  stk::Plucked * arg1 = (stk::Plucked *)GW_Plucked((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static TICK(PoleZero_tick) {
  stk::PoleZero *s = (stk::PoleZero*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_PoleZero(a) *(stk::PoleZero**)(a->data)
static CTOR(gw_PoleZero_ctor) {
  if(o->type_ref == t_PoleZero)
  GW_PoleZero(o) = new stk::PoleZero();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PoleZero_tick, GW_PoleZero(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_PoleZero_dtor) {
  if(GW_PoleZero(o)) {
    delete (stk::PoleZero*)GW_PoleZero(o);
    GW_PoleZero(o) = NULL;
  }
}

static MFUN(gw_PoleZero_setB0) {
  stk::PoleZero * arg1 = (stk::PoleZero *)GW_PoleZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_PoleZero_setB1) {
  stk::PoleZero * arg1 = (stk::PoleZero *)GW_PoleZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB1(arg2);
}

static MFUN(gw_PoleZero_setA1) {
  stk::PoleZero * arg1 = (stk::PoleZero *)GW_PoleZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_PoleZero_setCoefficients0) {
  stk::PoleZero * arg1 = (stk::PoleZero *)GW_PoleZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  bool arg5 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5);
}

static MFUN(gw_PoleZero_setCoefficients1) {
  stk::PoleZero * arg1 = (stk::PoleZero *)GW_PoleZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_PoleZero_setAllpass) {
  stk::PoleZero * arg1 = (stk::PoleZero *)GW_PoleZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setAllpass(arg2);
}

static MFUN(gw_PoleZero_setBlockZero0) {
  stk::PoleZero * arg1 = (stk::PoleZero *)GW_PoleZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBlockZero(arg2);
}

static MFUN(gw_PoleZero_setBlockZero1) {
  stk::PoleZero * arg1 = (stk::PoleZero *)GW_PoleZero((*(M_Object*)MEM(0)));
  (arg1)->setBlockZero();
}

static TICK(PRCRev_tick) {
  stk::PRCRev *s = (stk::PRCRev*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_PRCRev(a) *(stk::PRCRev**)(a->data)
static SFUN(gw_PRCRev_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::PRCRev * result = (stk::PRCRev *)new stk::PRCRev(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_PRCRev);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.PRCRev");
  GW_PRCRev(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), PRCRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_PRCRev_ctor1) {
  if(o->type_ref == t_PRCRev)
  GW_PRCRev(o) = new stk::PRCRev();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PRCRev_tick, GW_PRCRev(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_PRCRev_clear) {
  stk::PRCRev * arg1 = (stk::PRCRev *)GW_PRCRev((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_PRCRev_setT60) {
  stk::PRCRev * arg1 = (stk::PRCRev *)GW_PRCRev((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setT60(arg2);
}

static DTOR(gw_PRCRev_dtor) {
  if(GW_PRCRev(o)) {
    delete (stk::PRCRev*)GW_PRCRev(o);
    GW_PRCRev(o) = NULL;
  }
}

static TICK(Recorder_tick) {
  stk::Recorder *s = (stk::Recorder*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Recorder(a) *(stk::Recorder**)(a->data)
static CTOR(gw_Recorder_ctor) {
  if(o->type_ref == t_Recorder)
  GW_Recorder(o) = new stk::Recorder();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Recorder_tick, GW_Recorder(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Recorder_dtor) {
  if(GW_Recorder(o)) {
    delete (stk::Recorder*)GW_Recorder(o);
    GW_Recorder(o) = NULL;
  }
}

static MFUN(gw_Recorder_clear) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Recorder_setFrequency) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Recorder_startBlowing) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Recorder_stopBlowing) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Recorder_noteOn) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Recorder_noteOff) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Recorder_controlChange) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static MFUN(gw_Recorder_setBlowPressure) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBlowPressure(arg2);
}

static MFUN(gw_Recorder_setVibratoGain) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoGain(arg2);
}

static MFUN(gw_Recorder_setVibratoFrequency) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoFrequency(arg2);
}

static MFUN(gw_Recorder_setNoiseGain) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setNoiseGain(arg2);
}

static MFUN(gw_Recorder_setBreathCutoff) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBreathCutoff(arg2);
}

static MFUN(gw_Recorder_setSoftness) {
  stk::Recorder * arg1 = (stk::Recorder *)GW_Recorder((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSoftness(arg2);
}

static TICK(ReedTable_tick) {
  stk::ReedTable *s = (stk::ReedTable*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_ReedTable(a) *(stk::ReedTable**)(a->data)
static CTOR(gw_ReedTable_ctor) {
  if(o->type_ref == t_ReedTable)
  GW_ReedTable(o) = new stk::ReedTable();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), ReedTable_tick, GW_ReedTable(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_ReedTable_setOffset) {
  stk::ReedTable * arg1 = (stk::ReedTable *)GW_ReedTable((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setOffset(arg2);
}

static MFUN(gw_ReedTable_setSlope) {
  stk::ReedTable * arg1 = (stk::ReedTable *)GW_ReedTable((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSlope(arg2);
}

static DTOR(gw_ReedTable_dtor) {
  if(GW_ReedTable(o)) {
    delete (stk::ReedTable*)GW_ReedTable(o);
    GW_ReedTable(o) = NULL;
  }
}

static TICK(Resonate_tick) {
  stk::Resonate *s = (stk::Resonate*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Resonate(a) *(stk::Resonate**)(a->data)
static CTOR(gw_Resonate_ctor) {
  if(o->type_ref == t_Resonate)
  GW_Resonate(o) = new stk::Resonate();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Resonate_tick, GW_Resonate(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Resonate_dtor) {
  if(GW_Resonate(o)) {
    delete (stk::Resonate*)GW_Resonate(o);
    GW_Resonate(o) = NULL;
  }
}

static MFUN(gw_Resonate_setResonance) {
  stk::Resonate * arg1 = (stk::Resonate *)GW_Resonate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3);
}

static MFUN(gw_Resonate_setNotch) {
  stk::Resonate * arg1 = (stk::Resonate *)GW_Resonate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setNotch(arg2,arg3);
}

static MFUN(gw_Resonate_setEqualGainZeroes) {
  stk::Resonate * arg1 = (stk::Resonate *)GW_Resonate((*(M_Object*)MEM(0)));
  (arg1)->setEqualGainZeroes();
}

static MFUN(gw_Resonate_keyOn) {
  stk::Resonate * arg1 = (stk::Resonate *)GW_Resonate((*(M_Object*)MEM(0)));
  (arg1)->keyOn();
}

static MFUN(gw_Resonate_keyOff) {
  stk::Resonate * arg1 = (stk::Resonate *)GW_Resonate((*(M_Object*)MEM(0)));
  (arg1)->keyOff();
}

static MFUN(gw_Resonate_noteOn) {
  stk::Resonate * arg1 = (stk::Resonate *)GW_Resonate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Resonate_noteOff) {
  stk::Resonate * arg1 = (stk::Resonate *)GW_Resonate((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Resonate_controlChange) {
  stk::Resonate * arg1 = (stk::Resonate *)GW_Resonate((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Rhodey_tick) {
  stk::Rhodey *s = (stk::Rhodey*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Rhodey(a) *(stk::Rhodey**)(a->data)
static CTOR(gw_Rhodey_ctor) {
  if(o->type_ref == t_Rhodey)
  GW_Rhodey(o) = new stk::Rhodey();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Rhodey_tick, GW_Rhodey(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Rhodey_dtor) {
  if(GW_Rhodey(o)) {
    delete (stk::Rhodey*)GW_Rhodey(o);
    GW_Rhodey(o) = NULL;
  }
}

static MFUN(gw_Rhodey_setFrequency) {
  stk::Rhodey * arg1 = (stk::Rhodey *)GW_Rhodey((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Rhodey_noteOn) {
  stk::Rhodey * arg1 = (stk::Rhodey *)GW_Rhodey((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static TICK(Saxofony_tick) {
  stk::Saxofony *s = (stk::Saxofony*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Saxofony(a) *(stk::Saxofony**)(a->data)
static SFUN(gw_Saxofony_ctor) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Saxofony * result = (stk::Saxofony *)new stk::Saxofony(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Saxofony);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Saxofony");
  GW_Saxofony(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Saxofony_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static DTOR(gw_Saxofony_dtor) {
  if(GW_Saxofony(o)) {
    delete (stk::Saxofony*)GW_Saxofony(o);
    GW_Saxofony(o) = NULL;
  }
}

static MFUN(gw_Saxofony_clear) {
  stk::Saxofony * arg1 = (stk::Saxofony *)GW_Saxofony((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Saxofony_setFrequency) {
  stk::Saxofony * arg1 = (stk::Saxofony *)GW_Saxofony((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Saxofony_setBlowPosition) {
  stk::Saxofony * arg1 = (stk::Saxofony *)GW_Saxofony((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBlowPosition(arg2);
}

static MFUN(gw_Saxofony_startBlowing) {
  stk::Saxofony * arg1 = (stk::Saxofony *)GW_Saxofony((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Saxofony_stopBlowing) {
  stk::Saxofony * arg1 = (stk::Saxofony *)GW_Saxofony((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Saxofony_noteOn) {
  stk::Saxofony * arg1 = (stk::Saxofony *)GW_Saxofony((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Saxofony_noteOff) {
  stk::Saxofony * arg1 = (stk::Saxofony *)GW_Saxofony((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Saxofony_controlChange) {
  stk::Saxofony * arg1 = (stk::Saxofony *)GW_Saxofony((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Shakers_tick) {
  stk::Shakers *s = (stk::Shakers*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Shakers(a) *(stk::Shakers**)(a->data)
static SFUN(gw_Shakers_ctor0) {
  int arg1 = (int)*(m_int*)MEM(0);
  stk::Shakers * result = (stk::Shakers *)new stk::Shakers(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Shakers);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Shakers");
  GW_Shakers(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Shakers_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Shakers_ctor1) {
  if(o->type_ref == t_Shakers)
  GW_Shakers(o) = new stk::Shakers();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Shakers_tick, GW_Shakers(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_Shakers_noteOn) {
  stk::Shakers * arg1 = (stk::Shakers *)GW_Shakers((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Shakers_noteOff) {
  stk::Shakers * arg1 = (stk::Shakers *)GW_Shakers((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Shakers_controlChange) {
  stk::Shakers * arg1 = (stk::Shakers *)GW_Shakers((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static DTOR(gw_Shakers_dtor) {
  if(GW_Shakers(o)) {
    delete (stk::Shakers*)GW_Shakers(o);
    GW_Shakers(o) = NULL;
  }
}

static SFUN(gw_MIN_ENERGY_get) {
  stk::StkFloat result = (stk::StkFloat)(stk::StkFloat)stk::MIN_ENERGY;
  *(m_float*)RETURN = (m_float)(float)result;
}

static SFUN(gw_WATER_FREQ_SWEEP_get) {
  stk::StkFloat result = (stk::StkFloat)(stk::StkFloat)stk::WATER_FREQ_SWEEP;
  *(m_float*)RETURN = (m_float)(float)result;
}

static TICK(Simple_tick) {
  stk::Simple *s = (stk::Simple*)u->module.gen.data;
  u->out = s->tick();
}

#define GW_Simple(a) *(stk::Simple**)(a->data)
static CTOR(gw_Simple_ctor) {
  if(o->type_ref == t_Simple)
  GW_Simple(o) = new stk::Simple();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Simple_tick, GW_Simple(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Simple_dtor) {
  if(GW_Simple(o)) {
    delete (stk::Simple*)GW_Simple(o);
    GW_Simple(o) = NULL;
  }
}

static MFUN(gw_Simple_setFrequency) {
  stk::Simple * arg1 = (stk::Simple *)GW_Simple((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Simple_keyOn) {
  stk::Simple * arg1 = (stk::Simple *)GW_Simple((*(M_Object*)MEM(0)));
  (arg1)->keyOn();
}

static MFUN(gw_Simple_keyOff) {
  stk::Simple * arg1 = (stk::Simple *)GW_Simple((*(M_Object*)MEM(0)));
  (arg1)->keyOff();
}

static MFUN(gw_Simple_noteOn) {
  stk::Simple * arg1 = (stk::Simple *)GW_Simple((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Simple_noteOff) {
  stk::Simple * arg1 = (stk::Simple *)GW_Simple((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Simple_controlChange) {
  stk::Simple * arg1 = (stk::Simple *)GW_Simple((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(SineWave_tick) {
  stk::SineWave *s = (stk::SineWave*)u->module.gen.data;
  u->out = s->tick();
}

static SFUN(gw_TABLE_SIZE_get) {
  unsigned long result = (unsigned long)(unsigned long)TABLE_SIZE;
  *(m_int*)RETURN = (m_int)result;
}

#define GW_SineWave(a) *(stk::SineWave**)(a->data)
static CTOR(gw_SineWave_ctor) {
  if(o->type_ref == t_SineWave)
  GW_SineWave(o) = new stk::SineWave();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), SineWave_tick, GW_SineWave(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_SineWave_dtor) {
  if(GW_SineWave(o)) {
    delete (stk::SineWave*)GW_SineWave(o);
    GW_SineWave(o) = NULL;
  }
}

static MFUN(gw_SineWave_reset) {
  stk::SineWave * arg1 = (stk::SineWave *)GW_SineWave((*(M_Object*)MEM(0)));
  (arg1)->reset();
}

static MFUN(gw_SineWave_setRate) {
  stk::SineWave * arg1 = (stk::SineWave *)GW_SineWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRate(arg2);
}

static MFUN(gw_SineWave_setFrequency) {
  stk::SineWave * arg1 = (stk::SineWave *)GW_SineWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_SineWave_addTime) {
  stk::SineWave * arg1 = (stk::SineWave *)GW_SineWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->addTime(arg2);
}

static MFUN(gw_SineWave_addPhase) {
  stk::SineWave * arg1 = (stk::SineWave *)GW_SineWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->addPhase(arg2);
}

static MFUN(gw_SineWave_addPhaseOffset) {
  stk::SineWave * arg1 = (stk::SineWave *)GW_SineWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->addPhaseOffset(arg2);
}

static TICK(SingWave_tick) {
  stk::SingWave *s = (stk::SingWave*)u->module.gen.data;
  u->out = s->tick();
}

#define GW_SingWave(a) *(stk::SingWave**)(a->data)
static SFUN(gw_SingWave_ctor0) {
  if(!*(M_Object*)MEM(0))
  handle(shred, (m_str)"NullPtrException");
  std::string arg1(STRING(*(M_Object*)MEM(0))); // here arg1
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  stk::SingWave * result = (stk::SingWave *)new stk::SingWave(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_SingWave);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.SingWave");
  GW_SingWave(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), SingWave_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static SFUN(gw_SingWave_ctor1) {
  if(!*(M_Object*)MEM(0))
  handle(shred, (m_str)"NullPtrException");
  std::string arg1(STRING(*(M_Object*)MEM(0))); // here arg1
  stk::SingWave * result = (stk::SingWave *)new stk::SingWave(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_SingWave);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.SingWave");
  GW_SingWave(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), SingWave_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static DTOR(gw_SingWave_dtor) {
  if(GW_SingWave(o)) {
    delete (stk::SingWave*)GW_SingWave(o);
    GW_SingWave(o) = NULL;
  }
}

static MFUN(gw_SingWave_reset) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  (arg1)->reset();
}

static MFUN(gw_SingWave_normalize0) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  (arg1)->normalize();
}

static MFUN(gw_SingWave_normalize1) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->normalize(arg2);
}

static MFUN(gw_SingWave_setFrequency) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_SingWave_setVibratoRate) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoRate(arg2);
}

static MFUN(gw_SingWave_setVibratoGain) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoGain(arg2);
}

static MFUN(gw_SingWave_setRandomGain) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRandomGain(arg2);
}

static MFUN(gw_SingWave_setSweepRate) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSweepRate(arg2);
}

static MFUN(gw_SingWave_setGainRate) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setGainRate(arg2);
}

static MFUN(gw_SingWave_setGainTarget) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setGainTarget(arg2);
}

static MFUN(gw_SingWave_noteOn) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  (arg1)->noteOn();
}

static MFUN(gw_SingWave_noteOff) {
  stk::SingWave * arg1 = (stk::SingWave *)GW_SingWave((*(M_Object*)MEM(0)));
  (arg1)->noteOff();
}

static TICK(Sitar_tick) {
  stk::Sitar *s = (stk::Sitar*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Sitar(a) *(stk::Sitar**)(a->data)
static SFUN(gw_Sitar_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Sitar * result = (stk::Sitar *)new stk::Sitar(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Sitar);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Sitar");
  GW_Sitar(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Sitar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Sitar_ctor1) {
  if(o->type_ref == t_Sitar)
  GW_Sitar(o) = new stk::Sitar();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Sitar_tick, GW_Sitar(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Sitar_dtor) {
  if(GW_Sitar(o)) {
    delete (stk::Sitar*)GW_Sitar(o);
    GW_Sitar(o) = NULL;
  }
}

static MFUN(gw_Sitar_clear) {
  stk::Sitar * arg1 = (stk::Sitar *)GW_Sitar((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Sitar_setFrequency) {
  stk::Sitar * arg1 = (stk::Sitar *)GW_Sitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Sitar_pluck) {
  stk::Sitar * arg1 = (stk::Sitar *)GW_Sitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_Sitar_noteOn) {
  stk::Sitar * arg1 = (stk::Sitar *)GW_Sitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Sitar_noteOff) {
  stk::Sitar * arg1 = (stk::Sitar *)GW_Sitar((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

#define GW_Vector3D(a) *(stk::Vector3D**)(a->data)
static SFUN(gw_Vector3D_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_FLOAT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_FLOAT+SZ_FLOAT);
  stk::Vector3D * result = (stk::Vector3D *)new stk::Vector3D(arg1,arg2,arg3);
  //M_Object ret_obj = new_object(shred->info->mp, t_Vector3D);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Vector3D");
  GW_Vector3D(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;
}

static SFUN(gw_Vector3D_ctor1) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_FLOAT);
  stk::Vector3D * result = (stk::Vector3D *)new stk::Vector3D(arg1,arg2);
  //M_Object ret_obj = new_object(shred->info->mp, t_Vector3D);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Vector3D");
  GW_Vector3D(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;
}

static SFUN(gw_Vector3D_ctor2) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Vector3D * result = (stk::Vector3D *)new stk::Vector3D(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Vector3D);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Vector3D");
  GW_Vector3D(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;
}

static CTOR(gw_Vector3D_ctor3) {
  if(o->type_ref == t_Vector3D)
  GW_Vector3D(o) = new stk::Vector3D();
}

static MFUN(gw_Vector3D_getX) {
  stk::Vector3D * arg1 = (stk::Vector3D *)GW_Vector3D((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->getX();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Vector3D_getY) {
  stk::Vector3D * arg1 = (stk::Vector3D *)GW_Vector3D((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->getY();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Vector3D_getZ) {
  stk::Vector3D * arg1 = (stk::Vector3D *)GW_Vector3D((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->getZ();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Vector3D_getLength) {
  stk::Vector3D * arg1 = (stk::Vector3D *)GW_Vector3D((*(M_Object*)MEM(0)));
  stk::StkFloat result = (stk::StkFloat)(arg1)->getLength();
  *(m_float*)RETURN = (m_float)(float)result;
}

static MFUN(gw_Vector3D_setXYZ) {
  stk::Vector3D * arg1 = (stk::Vector3D *)GW_Vector3D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setXYZ(arg2,arg3,arg4);
}

static MFUN(gw_Vector3D_setX) {
  stk::Vector3D * arg1 = (stk::Vector3D *)GW_Vector3D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setX(arg2);
}

static MFUN(gw_Vector3D_setY) {
  stk::Vector3D * arg1 = (stk::Vector3D *)GW_Vector3D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setY(arg2);
}

static MFUN(gw_Vector3D_setZ) {
  stk::Vector3D * arg1 = (stk::Vector3D *)GW_Vector3D((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setZ(arg2);
}

static DTOR(gw_Vector3D_dtor) {
  if(GW_Vector3D(o)) {
    delete (stk::Vector3D*)GW_Vector3D(o);
    GW_Vector3D(o) = NULL;
  }
}

static TICK(StifKarp_tick) {
  stk::StifKarp *s = (stk::StifKarp*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_StifKarp(a) *(stk::StifKarp**)(a->data)
static SFUN(gw_StifKarp_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::StifKarp * result = (stk::StifKarp *)new stk::StifKarp(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_StifKarp);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.StifKarp");
  GW_StifKarp(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), StifKarp_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_StifKarp_ctor1) {
  if(o->type_ref == t_StifKarp)
  GW_StifKarp(o) = new stk::StifKarp();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), StifKarp_tick, GW_StifKarp(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_StifKarp_dtor) {
  if(GW_StifKarp(o)) {
    delete (stk::StifKarp*)GW_StifKarp(o);
    GW_StifKarp(o) = NULL;
  }
}

static MFUN(gw_StifKarp_clear) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_StifKarp_setFrequency) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_StifKarp_setStretch) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setStretch(arg2);
}

static MFUN(gw_StifKarp_setPickupPosition) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPickupPosition(arg2);
}

static MFUN(gw_StifKarp_setBaseLoopGain) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBaseLoopGain(arg2);
}

static MFUN(gw_StifKarp_pluck) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_StifKarp_noteOn) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_StifKarp_noteOff) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_StifKarp_controlChange) {
  stk::StifKarp * arg1 = (stk::StifKarp *)GW_StifKarp((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(TubeBell_tick) {
  stk::TubeBell *s = (stk::TubeBell*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_TubeBell(a) *(stk::TubeBell**)(a->data)
static CTOR(gw_TubeBell_ctor) {
  if(o->type_ref == t_TubeBell)
  GW_TubeBell(o) = new stk::TubeBell();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), TubeBell_tick, GW_TubeBell(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_TubeBell_dtor) {
  if(GW_TubeBell(o)) {
    delete (stk::TubeBell*)GW_TubeBell(o);
    GW_TubeBell(o) = NULL;
  }
}

static MFUN(gw_TubeBell_noteOn) {
  stk::TubeBell * arg1 = (stk::TubeBell *)GW_TubeBell((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static TICK(Twang_tick) {
  stk::Twang *s = (stk::Twang*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Twang(a) *(stk::Twang**)(a->data)
static SFUN(gw_Twang_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Twang * result = (stk::Twang *)new stk::Twang(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Twang);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Twang");
  GW_Twang(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Twang_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Twang_ctor1) {
  if(o->type_ref == t_Twang)
  GW_Twang(o) = new stk::Twang();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Twang_tick, GW_Twang(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_Twang_clear) {
  stk::Twang * arg1 = (stk::Twang *)GW_Twang((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Twang_setLowestFrequency) {
  stk::Twang * arg1 = (stk::Twang *)GW_Twang((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLowestFrequency(arg2);
}

static MFUN(gw_Twang_setFrequency) {
  stk::Twang * arg1 = (stk::Twang *)GW_Twang((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Twang_setPluckPosition) {
  stk::Twang * arg1 = (stk::Twang *)GW_Twang((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPluckPosition(arg2);
}

static MFUN(gw_Twang_setLoopGain) {
  stk::Twang * arg1 = (stk::Twang *)GW_Twang((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLoopGain(arg2);
}

static MFUN(gw_Twang_setLoopFilter) {
  stk::Twang * arg1 = (stk::Twang *)GW_Twang((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  const M_Vector arg2_array = ARRAY(*(M_Object*)MEM(0+SZ_INT));
  std::vector< stk::StkFloat > arg2;
  for (m_int i=0; i < m_vector_size(arg2_array); i++) {
    m_float f;
    m_vector_get(arg2_array, i, &f);
    (&arg2)->push_back(f);
  }
  (arg1)->setLoopFilter(arg2);
}

static DTOR(gw_Twang_dtor) {
  if(GW_Twang(o)) {
    delete (stk::Twang*)GW_Twang(o);
    GW_Twang(o) = NULL;
  }
}

static TICK(TwoPole_tick) {
  stk::TwoPole *s = (stk::TwoPole*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_TwoPole(a) *(stk::TwoPole**)(a->data)
static CTOR(gw_TwoPole_ctor) {
  if(o->type_ref == t_TwoPole)
  GW_TwoPole(o) = new stk::TwoPole();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), TwoPole_tick, GW_TwoPole(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_TwoPole_dtor) {
  if(GW_TwoPole(o)) {
    delete (stk::TwoPole*)GW_TwoPole(o);
    GW_TwoPole(o) = NULL;
  }
}

static MFUN(gw_TwoPole_ignoreSampleRateChange0) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_TwoPole_ignoreSampleRateChange1) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_TwoPole_setB0) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_TwoPole_setA1) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_TwoPole_setA2) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA2(arg2);
}

static MFUN(gw_TwoPole_setCoefficients0) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  bool arg5 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5);
}

static MFUN(gw_TwoPole_setCoefficients1) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_TwoPole_setResonance0) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  bool arg4 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3,arg4);
}

static MFUN(gw_TwoPole_setResonance1) {
  stk::TwoPole * arg1 = (stk::TwoPole *)GW_TwoPole((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3);
}

static TICK(TwoZero_tick) {
  stk::TwoZero *s = (stk::TwoZero*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_TwoZero(a) *(stk::TwoZero**)(a->data)
static CTOR(gw_TwoZero_ctor) {
  if(o->type_ref == t_TwoZero)
  GW_TwoZero(o) = new stk::TwoZero();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), TwoZero_tick, GW_TwoZero(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_TwoZero_dtor) {
  if(GW_TwoZero(o)) {
    delete (stk::TwoZero*)GW_TwoZero(o);
    GW_TwoZero(o) = NULL;
  }
}

static MFUN(gw_TwoZero_ignoreSampleRateChange0) {
  stk::TwoZero * arg1 = (stk::TwoZero *)GW_TwoZero((*(M_Object*)MEM(0)));
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_TwoZero_ignoreSampleRateChange1) {
  stk::TwoZero * arg1 = (stk::TwoZero *)GW_TwoZero((*(M_Object*)MEM(0)));
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_TwoZero_setB0) {
  stk::TwoZero * arg1 = (stk::TwoZero *)GW_TwoZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_TwoZero_setB1) {
  stk::TwoZero * arg1 = (stk::TwoZero *)GW_TwoZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB1(arg2);
}

static MFUN(gw_TwoZero_setB2) {
  stk::TwoZero * arg1 = (stk::TwoZero *)GW_TwoZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB2(arg2);
}

static MFUN(gw_TwoZero_setCoefficients0) {
  stk::TwoZero * arg1 = (stk::TwoZero *)GW_TwoZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  bool arg5 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5);
}

static MFUN(gw_TwoZero_setCoefficients1) {
  stk::TwoZero * arg1 = (stk::TwoZero *)GW_TwoZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_TwoZero_setNotch) {
  stk::TwoZero * arg1 = (stk::TwoZero *)GW_TwoZero((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setNotch(arg2,arg3);
}

static TICK(Voicer_tick) {
  stk::Voicer *s = (stk::Voicer*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Voicer(a) *(stk::Voicer**)(a->data)
static SFUN(gw_Voicer_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Voicer * result = (stk::Voicer *)new stk::Voicer(arg1);
  //M_Object ret_obj = new_object(shred->info->mp, t_Voicer);
  M_Object ret_obj = new_object_str(shred->info->vm->gwion, (m_str)"stk.Voicer");
  GW_Voicer(ret_obj) = result;
  *(M_Object*)RETURN = ret_obj;UGEN(ret_obj) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(ret_obj), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret_obj), Voicer_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret_obj));
}

static CTOR(gw_Voicer_ctor1) {
  if(o->type_ref == t_Voicer)
  GW_Voicer(o) = new stk::Voicer();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Voicer_tick, GW_Voicer(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static MFUN(gw_Voicer_addInstrument0) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  stk::Instrmnt * arg2 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0+SZ_INT)));
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->addInstrument(arg2,arg3);
}

static MFUN(gw_Voicer_addInstrument1) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  stk::Instrmnt * arg2 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0+SZ_INT)));
  (arg1)->addInstrument(arg2);
}

static MFUN(gw_Voicer_removeInstrument) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  stk::Instrmnt * arg2 = (stk::Instrmnt *)GW_Instrmnt((*(M_Object*)MEM(0+SZ_INT)));
  (arg1)->removeInstrument(arg2);
}

static MFUN(gw_Voicer_noteOn0) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  long result = (long)(arg1)->noteOn(arg2,arg3,arg4);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Voicer_noteOn1) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  long result = (long)(arg1)->noteOn(arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Voicer_noteOff0) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->noteOff(arg2,arg3,arg4);
}

static MFUN(gw_Voicer_noteOff1) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOff(arg2,arg3);
}

static MFUN(gw_Voicer_noteOff2) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  long arg2 = (long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->noteOff(arg2,arg3);
}

static MFUN(gw_Voicer_setFrequency0) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setFrequency(arg2,arg3);
}

static MFUN(gw_Voicer_setFrequency1) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Voicer_setFrequency2) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  long arg2 = (long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setFrequency(arg2,arg3);
}

static MFUN(gw_Voicer_pitchBend0) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->pitchBend(arg2,arg3);
}

static MFUN(gw_Voicer_pitchBend1) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pitchBend(arg2);
}

static MFUN(gw_Voicer_pitchBend2) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  long arg2 = (long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->pitchBend(arg2,arg3);
}

static MFUN(gw_Voicer_controlChange0) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_FLOAT);
  (arg1)->controlChange(arg2,arg3,arg4);
}

static MFUN(gw_Voicer_controlChange1) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static MFUN(gw_Voicer_controlChange2) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  long arg2 = (long)*(m_int*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3,arg4);
}

static MFUN(gw_Voicer_silence) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  (arg1)->silence();
}

static MFUN(gw_Voicer_channelsOut) {
  stk::Voicer * arg1 = (stk::Voicer *)GW_Voicer((*(M_Object*)MEM(0)));
  unsigned int result = (unsigned int)((stk::Voicer const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static DTOR(gw_Voicer_dtor) {
  if(GW_Voicer(o)) {
    delete (stk::Voicer*)GW_Voicer(o);
    GW_Voicer(o) = NULL;
  }
}

static TICK(VoicForm_tick) {
  stk::VoicForm *s = (stk::VoicForm*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_VoicForm(a) *(stk::VoicForm**)(a->data)
static CTOR(gw_VoicForm_ctor) {
  if(o->type_ref == t_VoicForm)
  GW_VoicForm(o) = new stk::VoicForm();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), VoicForm_tick, GW_VoicForm(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_VoicForm_dtor) {
  if(GW_VoicForm(o)) {
    delete (stk::VoicForm*)GW_VoicForm(o);
    GW_VoicForm(o) = NULL;
  }
}

static MFUN(gw_VoicForm_clear) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_VoicForm_setFrequency) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_VoicForm_setPhoneme) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  if(!*(M_Object*)MEM(0+SZ_INT))
  handle(shred, (m_str)"NullPtrException");
  char * arg2 = (char*)STRING(*(M_Object*)MEM(0+SZ_INT));
  bool result = (bool)(arg1)->setPhoneme((char const *)arg2);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_VoicForm_setVoiced) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVoiced(arg2);
}

static MFUN(gw_VoicForm_setUnVoiced) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setUnVoiced(arg2);
}

static MFUN(gw_VoicForm_setFilterSweepRate) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setFilterSweepRate(arg2,arg3);
}

static MFUN(gw_VoicForm_setPitchSweepRate) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPitchSweepRate(arg2);
}

static MFUN(gw_VoicForm_speak) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  (arg1)->speak();
}

static MFUN(gw_VoicForm_quiet) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  (arg1)->quiet();
}

static MFUN(gw_VoicForm_noteOn) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_VoicForm_noteOff) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_VoicForm_controlChange) {
  stk::VoicForm * arg1 = (stk::VoicForm *)GW_VoicForm((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Whistle_tick) {
  stk::Whistle *s = (stk::Whistle*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Whistle(a) *(stk::Whistle**)(a->data)
static CTOR(gw_Whistle_ctor) {
  if(o->type_ref == t_Whistle)
  GW_Whistle(o) = new stk::Whistle();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Whistle_tick, GW_Whistle(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Whistle_dtor) {
  if(GW_Whistle(o)) {
    delete (stk::Whistle*)GW_Whistle(o);
    GW_Whistle(o) = NULL;
  }
}

static MFUN(gw_Whistle_clear) {
  stk::Whistle * arg1 = (stk::Whistle *)GW_Whistle((*(M_Object*)MEM(0)));
  (arg1)->clear();
}

static MFUN(gw_Whistle_setFrequency) {
  stk::Whistle * arg1 = (stk::Whistle *)GW_Whistle((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Whistle_startBlowing) {
  stk::Whistle * arg1 = (stk::Whistle *)GW_Whistle((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Whistle_stopBlowing) {
  stk::Whistle * arg1 = (stk::Whistle *)GW_Whistle((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Whistle_noteOn) {
  stk::Whistle * arg1 = (stk::Whistle *)GW_Whistle((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Whistle_noteOff) {
  stk::Whistle * arg1 = (stk::Whistle *)GW_Whistle((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Whistle_controlChange) {
  stk::Whistle * arg1 = (stk::Whistle *)GW_Whistle((*(M_Object*)MEM(0)));
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Wurley_tick) {
  stk::Wurley *s = (stk::Wurley*)u->module.gen.data;
  u->out = s->tick(1);
}

#define GW_Wurley(a) *(stk::Wurley**)(a->data)
static CTOR(gw_Wurley_ctor) {
  if(o->type_ref == t_Wurley)
  GW_Wurley(o) = new stk::Wurley();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Wurley_tick, GW_Wurley(o), 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
}

static DTOR(gw_Wurley_dtor) {
  if(GW_Wurley(o)) {
    delete (stk::Wurley*)GW_Wurley(o);
    GW_Wurley(o) = NULL;
  }
}

static MFUN(gw_Wurley_setFrequency) {
  stk::Wurley * arg1 = (stk::Wurley *)GW_Wurley((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Wurley_noteOn) {
  stk::Wurley * arg1 = (stk::Wurley *)GW_Wurley((*(M_Object*)MEM(0)));
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

m_bool CPPIMPORT(Gwi gwi) {
  CHECK_OB(gwi_struct_ini(gwi, (m_str)"stk"));

  gwi_typedef_ini(gwi, (m_str)"float", (m_str)"StkFloat");
  gwi_typedef_end(gwi, ae_flag_none);

  stk::Stk::setSampleRate(gwi->gwion->vm->bbq->si->sr);
  const Type t_Stk = gwi_class_ini(gwi, "Stk", "UGen");
  SET_FLAG(t_Stk, abstract);
  t_Stk->nspc->offset += SZ_INT;

  CHECK_BB(gwi_func_ini(gwi, "int", "STK_SINT8"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_STK_SINT8_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "STK_SINT16"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_STK_SINT16_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "STK_SINT24"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_STK_SINT24_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "STK_SINT32"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_STK_SINT32_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "STK_FLOAT32"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_STK_FLOAT32_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "STK_FLOAT64"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_STK_FLOAT64_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "float", "sampleRate"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_sampleRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSampleRate"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_setSampleRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_ignoreSampleRateChange0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_ignoreSampleRateChange1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "string", "rawwavePath"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_rawwavePath, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRawwavePath"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_setRawwavePath, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "sleep"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_sleep, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "inRange"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_inRange, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "showWarnings"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_showWarnings, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "printErrors"));
  CHECK_BB(gwi_func_end(gwi, gw_Stk_printErrors, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Stk;
  
  
  CHECK_BB(gwi_func_ini(gwi, "float", "SRATE"));
  CHECK_BB(gwi_func_end(gwi, gw_SRATE_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "RT_BUFFER_SIZE"));
  CHECK_BB(gwi_func_end(gwi, gw_RT_BUFFER_SIZE_get, ae_flag_static));
  CHECK_BB(gwi_item_ini(gwi, "string", "RAWWAVE_PATH"));
  const M_Object rawpath = new_string(gwi->gwion, s_name(insert_symbol(gwi->gwion->st, (m_str)"../../rawwaves/")));
  CHECK_BB(gwi_item_end(gwi, ae_flag_const | ae_flag_static, obj, rawpath));
  CHECK_BB(gwi_func_ini(gwi, "float", "PI"));
  CHECK_BB(gwi_func_end(gwi, gw_PI_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "float", "TWO_PI"));
  CHECK_BB(gwi_func_end(gwi, gw_TWO_PI_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "float", "ONE_OVER_128"));
  CHECK_BB(gwi_func_end(gwi, gw_ONE_OVER_128_get, ae_flag_static));
  /*const Type*/ t_Instrmnt = gwi_class_ini(gwi, "Instrmnt", "Stk");
  SET_FLAG(t_Instrmnt, abstract);
  gwi_class_xtor(gwi, NULL, gw_Instrmnt_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Instrmnt_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Instrmnt_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Instrmnt_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Instrmnt_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Instrmnt_controlChange, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "channelsOut"));
  CHECK_BB(gwi_func_end(gwi, gw_Instrmnt_channelsOut, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Instrmnt;
  
  /*const Type*/ t_Generator = gwi_class_ini(gwi, "Generator", "Stk");
  SET_FLAG(t_Generator, abstract);
  gwi_class_xtor(gwi, NULL, gw_Generator_dtor);
  CHECK_BB(gwi_func_ini(gwi, "int", "channelsOut"));
  CHECK_BB(gwi_func_end(gwi, gw_Generator_channelsOut, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Generator;
  
  /*const Type*/ t_FM = gwi_class_ini(gwi, "FM", "Instrmnt");
  SET_FLAG(t_FM, abstract);
  gwi_class_xtor(gwi, NULL, gw_FM_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "loadWaves"));
  CHECK_BB(gwi_func_arg(gwi, "string[]", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_loadWaves, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRatio"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_setRatio, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGain"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_setGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setModulationSpeed"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_setModulationSpeed, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setModulationDepth"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_setModulationDepth, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setControl1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_setControl1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setControl2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_setControl2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOn"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_keyOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOff"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_keyOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_FM_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// FM;
  
  /*const Type*/ t_Filter = gwi_class_ini(gwi, "Filter", "Stk");
  SET_FLAG(t_Filter, abstract);
  gwi_class_xtor(gwi, NULL, gw_Filter_dtor);
  CHECK_BB(gwi_func_ini(gwi, "int", "channelsIn"));
  CHECK_BB(gwi_func_end(gwi, gw_Filter_channelsIn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "channelsOut"));
  CHECK_BB(gwi_func_end(gwi, gw_Filter_channelsOut, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Filter_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Filter_setGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getGain"));
  CHECK_BB(gwi_func_end(gwi, gw_Filter_getGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "phaseDelay"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Filter_phaseDelay, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));;
  
  /*const Type*/ t_Effect = gwi_class_ini(gwi, "Effect", "Stk");
  SET_FLAG(t_Effect, abstract);
  gwi_class_xtor(gwi, NULL, gw_Effect_dtor);
  CHECK_BB(gwi_func_ini(gwi, "int", "channelsOut"));
  CHECK_BB(gwi_func_end(gwi, gw_Effect_channelsOut, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Effect_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setEffectMix"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Effect_setEffectMix, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));;// Effect
  
  /*const Type*/ t_Function = gwi_class_ini(gwi, "Function", "Stk");
  SET_FLAG(t_Function, abstract);
  gwi_class_xtor(gwi, NULL, gw_Function_dtor);
  CHECK_BB(gwi_func_ini(gwi, "float", "lastOut"));
  CHECK_BB(gwi_func_end(gwi, gw_Function_lastOut, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));;// Function;
  
  /*const Type*/ t_Sampler = gwi_class_ini(gwi, "Sampler", "Instrmnt");
  SET_FLAG(t_Sampler, abstract);
  gwi_class_xtor(gwi, NULL, gw_Sampler_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Sampler_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOn"));
  CHECK_BB(gwi_func_end(gwi, gw_Sampler_keyOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOff"));
  CHECK_BB(gwi_func_end(gwi, gw_Sampler_keyOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Sampler_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Sampler_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));;// Sampler;
  
  /*const Type*/ t_ADSR = gwi_class_ini(gwi, "ADSR", "Generator");
  gwi_class_xtor(gwi, gw_ADSR_ctor, gw_ADSR_dtor);
  CHECK_BB(gwi_enum_ini(gwi, (m_str)"@0"));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"ATTACK", (m_uint)stk::ADSR::ATTACK));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"DECAY", (m_uint)stk::ADSR::DECAY));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"SUSTAIN", (m_uint)stk::ADSR::SUSTAIN));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"RELEASE", (m_uint)stk::ADSR::RELEASE));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"IDLE", (m_uint)stk::ADSR::IDLE));
  CHECK_OB(gwi_enum_end(gwi));
  
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOn"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_keyOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOff"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_keyOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setAttackRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setAttackRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setAttackTarget"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setAttackTarget, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDecayRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setDecayRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSustainLevel"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setSustainLevel, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setReleaseRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setReleaseRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setAttackTime"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setAttackTime, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDecayTime"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setDecayTime, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setReleaseTime"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setReleaseTime, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setAllTimes"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg5"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setAllTimes, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setTarget"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setTarget, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "getState"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_getState, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setValue"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ADSR_setValue, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));;// ADSR;
  
  
  CHECK_BB(gwi_func_ini(gwi, "float", "TARGET_THRESHOLD"));
  CHECK_BB(gwi_func_end(gwi, gw_TARGET_THRESHOLD_get, ae_flag_static));
  /*const Type*/ t_Asymp = gwi_class_ini(gwi, "Asymp", "Generator");;
  gwi_class_xtor(gwi, gw_Asymp_ctor, gw_Asymp_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOn"));
  CHECK_BB(gwi_func_end(gwi, gw_Asymp_keyOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOff"));
  CHECK_BB(gwi_func_end(gwi, gw_Asymp_keyOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setTau"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Asymp_setTau, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setTime"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Asymp_setTime, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setT60"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Asymp_setT60, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setTarget"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Asymp_setTarget, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setValue"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Asymp_setValue, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "getState"));
  CHECK_BB(gwi_func_end(gwi, gw_Asymp_getState, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Asymp;
  
  
  CHECK_BB(gwi_func_ini(gwi, "int", "MAX_BANDED_MODES"));
  CHECK_BB(gwi_func_end(gwi, gw_MAX_BANDED_MODES_get, ae_flag_static));
  /*const Type*/ t_BandedWG = gwi_class_ini(gwi, "BandedWG", "Instrmnt");;
  gwi_class_xtor(gwi, gw_BandedWG_ctor, gw_BandedWG_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setStrikePosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_setStrikePosition, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPreset"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_setPreset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_startBowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_stopBowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pluck"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_pluck, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BandedWG_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// BandedWG;
  
  /*const Type*/ t_BiQuad = gwi_class_ini(gwi, "BiQuad", "Filter");
  gwi_class_xtor(gwi, gw_BiQuad_ctor, gw_BiQuad_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_ignoreSampleRateChange0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_ignoreSampleRateChange1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg5"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg6"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg7"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setCoefficients0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg5"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg6"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setCoefficients1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB0"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setB0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setB1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setB2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setA1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setA1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setA2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setA2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setResonance"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setResonance0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setResonance"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setResonance1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setNotch"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setNotch, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setEqualGainZeroes"));
  CHECK_BB(gwi_func_end(gwi, gw_BiQuad_setEqualGainZeroes, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// BiQuad;
  
  /*const Type*/ t_Blit = gwi_class_ini(gwi, "Blit", "Generator");
  gwi_class_xtor(gwi, gw_Blit_ctor1, gw_Blit_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Blit", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Blit_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "reset"));
  CHECK_BB(gwi_func_end(gwi, gw_Blit_reset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPhase"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Blit_setPhase, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getPhase"));
  CHECK_BB(gwi_func_end(gwi, gw_Blit_getPhase, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Blit_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setHarmonics"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Blit_setHarmonics0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setHarmonics"));
  CHECK_BB(gwi_func_end(gwi, gw_Blit_setHarmonics1, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Blit;
  
  /*const Type*/ t_BlitSaw = gwi_class_ini(gwi, "BlitSaw", "Generator");
  gwi_class_xtor(gwi, gw_BlitSaw_ctor1, gw_BlitSaw_dtor);
  CHECK_BB(gwi_func_ini(gwi, "BlitSaw", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSaw_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "reset"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSaw_reset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSaw_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setHarmonics"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSaw_setHarmonics0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setHarmonics"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSaw_setHarmonics1, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// BlitSaw;
  
  /*const Type*/ t_BlitSquare = gwi_class_ini(gwi, "BlitSquare", "Generator");
  gwi_class_xtor(gwi, gw_BlitSquare_ctor1, gw_BlitSquare_dtor);
  CHECK_BB(gwi_func_ini(gwi, "BlitSquare", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSquare_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "reset"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSquare_reset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPhase"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSquare_setPhase, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getPhase"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSquare_getPhase, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSquare_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setHarmonics"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSquare_setHarmonics0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setHarmonics"));
  CHECK_BB(gwi_func_end(gwi, gw_BlitSquare_setHarmonics1, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// BlitSquare;
  
  /*const Type*/ t_BeeThree = gwi_class_ini(gwi, "BeeThree", "FM");
  gwi_class_xtor(gwi, gw_BeeThree_ctor, gw_BeeThree_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BeeThree_noteOn, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// BeeThree;
  
  /*const Type*/ t_BlowBotl = gwi_class_ini(gwi, "BlowBotl", "Instrmnt");
  gwi_class_xtor(gwi, gw_BlowBotl_ctor, gw_BlowBotl_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowBotl_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowBotl_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowBotl_startBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowBotl_stopBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowBotl_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowBotl_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowBotl_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// BlowBotl;
  
  /*const Type*/ t_BlowHole = gwi_class_ini(gwi, "BlowHole", "Instrmnt");
  SET_FLAG(t_BlowHole, abstract);
  gwi_class_xtor(gwi, NULL, gw_BlowHole_dtor);
  CHECK_BB(gwi_func_ini(gwi, "BlowHole", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_ctor, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setTonehole"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_setTonehole, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVent"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_setVent, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_startBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_stopBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_BlowHole_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// BlowHole;
  
  /*const Type*/ t_Bowed = gwi_class_ini(gwi, "Bowed", "Instrmnt");
  gwi_class_xtor(gwi, gw_Bowed_ctor1, gw_Bowed_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Bowed", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVibrato"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_setVibrato, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_startBowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_stopBowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Bowed_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Bowed;
  
  /*const Type*/ t_BowTable = gwi_class_ini(gwi, "BowTable", "Function");
  gwi_class_xtor(gwi, gw_BowTable_ctor, gw_BowTable_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setOffset"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BowTable_setOffset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSlope"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BowTable_setSlope, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setMinOutput"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BowTable_setMinOutput, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setMaxOutput"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_BowTable_setMaxOutput, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// BowTable;
  
  /*const Type*/ t_Brass = gwi_class_ini(gwi, "Brass", "Instrmnt");
  gwi_class_xtor(gwi, gw_Brass_ctor1, gw_Brass_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Brass", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setLip"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_setLip, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_startBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_stopBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Brass_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Brass;
  
  /*const Type*/ t_Chorus = gwi_class_ini(gwi, "Chorus", "Effect");
  gwi_class_xtor(gwi, gw_Chorus_ctor1, gw_Chorus_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Chorus", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Chorus_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Chorus_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setModDepth"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Chorus_setModDepth, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setModFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Chorus_setModFrequency, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Chorus;
  
  /*const Type*/ t_Clarinet = gwi_class_ini(gwi, "Clarinet", "Instrmnt");
  gwi_class_xtor(gwi, gw_Clarinet_ctor1, gw_Clarinet_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Clarinet", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Clarinet_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Clarinet_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Clarinet_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Clarinet_startBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Clarinet_stopBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Clarinet_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Clarinet_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Clarinet_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Clarinet;
  
  /*const Type*/ t_Cubic = gwi_class_ini(gwi, "Cubic", "Function");
  gwi_class_xtor(gwi, gw_Cubic_ctor, gw_Cubic_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setA1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Cubic_setA1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setA2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Cubic_setA2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setA3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Cubic_setA3, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Cubic_setGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setThreshold"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Cubic_setThreshold, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Cubic;
  
  /*const Type*/ t_Delay = gwi_class_ini(gwi, "Delay", "Filter");
  gwi_class_xtor(gwi, gw_Delay_ctor2, gw_Delay_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Delay", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "Delay", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_ctor1, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "getMaximumDelay"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_getMaximumDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setMaximumDelay"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_setMaximumDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDelay"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_setDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "getDelay"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_getDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "tapOut"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_tapOut, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "tapIn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_tapIn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "addTo"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_addTo, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "nextOut"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_nextOut, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "energy"));
  CHECK_BB(gwi_func_end(gwi, gw_Delay_energy, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Delay;
  
  /*const Type*/ t_DelayA = gwi_class_ini(gwi, "DelayA", "Filter");
  gwi_class_xtor(gwi, gw_DelayA_ctor2, gw_DelayA_dtor);
  CHECK_BB(gwi_func_ini(gwi, "DelayA", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "DelayA", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_ctor1, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "getMaximumDelay"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_getMaximumDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setMaximumDelay"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_setMaximumDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDelay"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_setDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getDelay"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_getDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "tapOut"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_tapOut, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "tapIn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_tapIn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "nextOut"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayA_nextOut, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// DelayA;
  
  /*const Type*/ t_DelayL = gwi_class_ini(gwi, "DelayL", "Filter");
  gwi_class_xtor(gwi, gw_DelayL_ctor2, gw_DelayL_dtor);
  CHECK_BB(gwi_func_ini(gwi, "DelayL", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "DelayL", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_ctor1, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "getMaximumDelay"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_getMaximumDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setMaximumDelay"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_setMaximumDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDelay"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_setDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getDelay"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_getDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "tapOut"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_tapOut, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "tapIn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_tapIn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "nextOut"));
  CHECK_BB(gwi_func_end(gwi, gw_DelayL_nextOut, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// DelayL;
  
  
  CHECK_BB(gwi_func_ini(gwi, "int", "DRUM_NUMWAVES"));
  CHECK_BB(gwi_func_end(gwi, gw_DRUM_NUMWAVES_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "DRUM_POLYPHONY"));
  CHECK_BB(gwi_func_end(gwi, gw_DRUM_POLYPHONY_get, ae_flag_static));
/*const Type*/ t_Drummer = gwi_class_ini(gwi, "Drummer", "Instrmnt");;
  gwi_class_xtor(gwi, gw_Drummer_ctor, gw_Drummer_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Drummer_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Drummer_noteOff, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Drummer;
  
  /*const Type*/ t_Echo = gwi_class_ini(gwi, "Echo", "Effect");
  gwi_class_xtor(gwi, gw_Echo_ctor1, gw_Echo_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Echo", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Echo_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Echo_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setMaximumDelay"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Echo_setMaximumDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDelay"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Echo_setDelay, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Echo;
  
  /*const Type*/ t_Flute = gwi_class_ini(gwi, "Flute", "Instrmnt");
  SET_FLAG(t_Flute, abstract);
  gwi_class_xtor(gwi, NULL, gw_Flute_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Flute", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_ctor, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setJetReflection"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_setJetReflection, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setEndReflection"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_setEndReflection, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setJetDelay"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_setJetDelay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_startBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_stopBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Flute_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Flute;
  
  /*const Type*/ t_FMVoices = gwi_class_ini(gwi, "FMVoices", "FM");
  gwi_class_xtor(gwi, gw_FMVoices_ctor, gw_FMVoices_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FMVoices_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_FMVoices_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_FMVoices_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// FMVoices;
  
  /*const Type*/ t_FormSwep = gwi_class_ini(gwi, "FormSwep", "Filter");
  gwi_class_xtor(gwi, gw_FormSwep_ctor, gw_FormSwep_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_ignoreSampleRateChange0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_ignoreSampleRateChange1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setResonance"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_setResonance, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setStates"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_setStates0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setStates"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_setStates1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setTargets"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_setTargets0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setTargets"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_setTargets1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSweepRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_setSweepRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSweepTime"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FormSwep_setSweepTime, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// FormSwep;
  
  /*const Type*/ t_FreeVerb = gwi_class_ini(gwi, "FreeVerb", "Effect");
  gwi_class_xtor(gwi, gw_FreeVerb_ctor, gw_FreeVerb_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setEffectMix"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_setEffectMix, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRoomSize"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_setRoomSize, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getRoomSize"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_getRoomSize, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDamping"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_setDamping, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getDamping"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_getDamping, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setWidth"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_setWidth, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getWidth"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_getWidth, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setMode"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_setMode, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getMode"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_getMode, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_FreeVerb_clear, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// FreeVerb;
  
  /*const Type*/ t_Granulate = gwi_class_ini(gwi, "Granulate", "Generator");
  gwi_class_xtor(gwi, gw_Granulate_ctor0, gw_Granulate_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Granulate", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_ctor1, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "Granulate", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_ctor2, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "openFile"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_openFile0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "openFile"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_openFile1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "reset"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_reset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVoices"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setVoices0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVoices"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setVoices1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setStretch"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setStretch0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setStretch"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setStretch1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGrainParameters"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg5"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setGrainParameters0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGrainParameters"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setGrainParameters1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGrainParameters"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setGrainParameters2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGrainParameters"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setGrainParameters3, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGrainParameters"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setGrainParameters4, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRandomFactor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setRandomFactor0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRandomFactor"));
  CHECK_BB(gwi_func_end(gwi, gw_Granulate_setRandomFactor1, ae_flag_none));
  CHECK_BB(gwi_enum_ini(gwi, (m_str)"GrainState"));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"GRAIN_STOPPED", (m_uint)stk::Granulate::GRAIN_STOPPED));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"GRAIN_FADEIN", (m_uint)stk::Granulate::GRAIN_FADEIN));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"GRAIN_SUSTAIN", (m_uint)stk::Granulate::GRAIN_SUSTAIN));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"GRAIN_FADEOUT", (m_uint)stk::Granulate::GRAIN_FADEOUT));
  CHECK_OB(gwi_enum_end(gwi));
  
  CHECK_BB(gwi_class_end(gwi));// Granulate;
  
  /*const Type*/ t_Guitar = gwi_class_ini(gwi, "Guitar", "Stk");
  gwi_class_xtor(gwi, gw_Guitar_ctor2, gw_Guitar_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Guitar", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "Guitar", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_ctor1, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBodyFile"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_setBodyFile0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBodyFile"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_setBodyFile1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPluckPosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_setPluckPosition0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPluckPosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_setPluckPosition1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setLoopGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_setLoopGain0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setLoopGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_setLoopGain1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_setFrequency0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_setFrequency1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_noteOn0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_noteOn1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_noteOff0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_noteOff1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_controlChange0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Guitar_controlChange1, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Guitar;
  
  /*const Type*/ t_HevyMetl = gwi_class_ini(gwi, "HevyMetl", "FM");
  gwi_class_xtor(gwi, gw_HevyMetl_ctor, gw_HevyMetl_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_HevyMetl_noteOn, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// HevyMetl;
  
  /*const Type*/ t_JCRev = gwi_class_ini(gwi, "JCRev", "Effect");
  gwi_class_xtor(gwi, gw_JCRev_ctor1, gw_JCRev_dtor);
  CHECK_BB(gwi_func_ini(gwi, "JCRev", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_JCRev_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_JCRev_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setT60"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_JCRev_setT60, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// JCRev;
  
  /*const Type*/ t_JetTable = gwi_class_ini(gwi, "JetTable", "Function");
  gwi_class_xtor(gwi, gw_JetTable_ctor, gw_JetTable_dtor);
  CHECK_BB(gwi_class_end(gwi));// JetTable;
  
  /*const Type*/ t_LentPitShift = gwi_class_ini(gwi, "LentPitShift", "Effect");
  gwi_class_xtor(gwi, gw_LentPitShift_ctor2, gw_LentPitShift_dtor);
  CHECK_BB(gwi_func_ini(gwi, "LentPitShift", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_LentPitShift_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "LentPitShift", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_LentPitShift_ctor1, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_LentPitShift_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setShift"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_LentPitShift_setShift, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// LentPitShift;
  
  /*const Type*/ t_Mandolin = gwi_class_ini(gwi, "Mandolin", "Instrmnt");
  SET_FLAG(t_Mandolin, abstract);
  gwi_class_xtor(gwi, NULL, gw_Mandolin_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Mandolin", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_ctor, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDetune"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_setDetune, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBodySize"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_setBodySize, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPluckPosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_setPluckPosition, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pluck"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_pluck0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pluck"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_pluck1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Mandolin_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Mandolin;
  
  
  CHECK_BB(gwi_func_ini(gwi, "int", "NXMAX"));
  CHECK_BB(gwi_func_end(gwi, gw_NXMAX_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "int", "NYMAX"));
  CHECK_BB(gwi_func_end(gwi, gw_NYMAX_get, ae_flag_static));
/*const Type*/ t_Mesh2D = gwi_class_ini(gwi, "Mesh2D", "Instrmnt");;
  SET_FLAG(t_Mesh2D, abstract);
  gwi_class_xtor(gwi, NULL, gw_Mesh2D_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Mesh2D", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_ctor, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setNX"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_setNX, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setNY"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_setNY, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setInputPosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_setInputPosition, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDecay"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_setDecay, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "energy"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_energy, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "inputTick"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_inputTick, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Mesh2D_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Mesh2D;
  
  /*const Type*/ t_Modal = gwi_class_ini(gwi, "Modal", "Instrmnt");
  SET_FLAG(t_Modal, abstract);
  gwi_class_xtor(gwi, NULL, gw_Modal_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRatioAndRadius"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_setRatioAndRadius, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setMasterGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_setMasterGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setDirectGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_setDirectGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setModeGain"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_setModeGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "strike"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_strike, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "damp"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_damp, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Modal_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Modal;
  
  /*const Type*/ t_ModalBar = gwi_class_ini(gwi, "ModalBar", "Modal");
  gwi_class_xtor(gwi, gw_ModalBar_ctor, gw_ModalBar_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setStickHardness"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ModalBar_setStickHardness, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setStrikePosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ModalBar_setStrikePosition, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPreset"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ModalBar_setPreset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setModulationDepth"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ModalBar_setModulationDepth, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_ModalBar_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// ModalBar;
  
  /*const Type*/ t_Modulate = gwi_class_ini(gwi, "Modulate", "Generator");
  gwi_class_xtor(gwi, gw_Modulate_ctor, gw_Modulate_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "reset"));
  CHECK_BB(gwi_func_end(gwi, gw_Modulate_reset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVibratoRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modulate_setVibratoRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVibratoGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modulate_setVibratoGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRandomRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modulate_setRandomRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRandomGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Modulate_setRandomGain, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Modulate;
  
  /*const Type*/ t_Moog = gwi_class_ini(gwi, "Moog", "Sampler");
  gwi_class_xtor(gwi, gw_Moog_ctor, gw_Moog_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Moog_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Moog_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setModulationSpeed"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Moog_setModulationSpeed, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setModulationDepth"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Moog_setModulationDepth, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Moog_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Moog;
  
  /*const Type*/ t_Noise = gwi_class_ini(gwi, "Noise", "Generator");
  gwi_class_xtor(gwi, gw_Noise_ctor1, gw_Noise_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Noise", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Noise_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSeed"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Noise_setSeed0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSeed"));
  CHECK_BB(gwi_func_end(gwi, gw_Noise_setSeed1, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Noise;
  
  /*const Type*/ t_NRev = gwi_class_ini(gwi, "NRev", "Effect");
  gwi_class_xtor(gwi, gw_NRev_ctor1, gw_NRev_dtor);
  CHECK_BB(gwi_func_ini(gwi, "NRev", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_NRev_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_NRev_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setT60"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_NRev_setT60, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// NRev;
  
  /*const Type*/ t_OnePole = gwi_class_ini(gwi, "OnePole", "Filter");
  gwi_class_xtor(gwi, gw_OnePole_ctor1, gw_OnePole_dtor);
  CHECK_BB(gwi_func_ini(gwi, "OnePole", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_OnePole_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB0"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_OnePole_setB0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setA1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_OnePole_setA1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_OnePole_setCoefficients0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_OnePole_setCoefficients1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPole"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_OnePole_setPole, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// OnePole;
  
  /*const Type*/ t_OneZero = gwi_class_ini(gwi, "OneZero", "Filter");
  gwi_class_xtor(gwi, gw_OneZero_ctor1, gw_OneZero_dtor);
  CHECK_BB(gwi_func_ini(gwi, "OneZero", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_OneZero_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB0"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_OneZero_setB0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_OneZero_setB1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_OneZero_setCoefficients0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_OneZero_setCoefficients1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setZero"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_OneZero_setZero, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// OneZero;
  
  /*const Type*/ t_PercFlut = gwi_class_ini(gwi, "PercFlut", "FM");
  gwi_class_xtor(gwi, gw_PercFlut_ctor, gw_PercFlut_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_PercFlut_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_PercFlut_noteOn, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// PercFlut;
  
  /*const Type*/ t_Phonemes = gwi_class_ini(gwi, "Phonemes", "Stk");
  gwi_class_xtor(gwi, gw_Phonemes_ctor, gw_Phonemes_dtor);
  CHECK_BB(gwi_func_ini(gwi, "string", "name"));
  CHECK_BB(gwi_func_end(gwi, gw_Phonemes_name, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "voiceGain"));
  CHECK_BB(gwi_func_end(gwi, gw_Phonemes_voiceGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "noiseGain"));
  CHECK_BB(gwi_func_end(gwi, gw_Phonemes_noiseGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "formantFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Phonemes_formantFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "formantRadius"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Phonemes_formantRadius, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "formantGain"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Phonemes_formantGain, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Phonemes;
  
  
  CHECK_BB(gwi_func_ini(gwi, "int", "maxDelay"));
  CHECK_BB(gwi_func_end(gwi, gw_maxDelay_get, ae_flag_static));
/*const Type*/ t_PitShift = gwi_class_ini(gwi, "PitShift", "Effect");;
  gwi_class_xtor(gwi, gw_PitShift_ctor, gw_PitShift_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_PitShift_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setShift"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_PitShift_setShift, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// PitShift;
  
  /*const Type*/ t_Plucked = gwi_class_ini(gwi, "Plucked", "Instrmnt");
  gwi_class_xtor(gwi, gw_Plucked_ctor1, gw_Plucked_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Plucked", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Plucked_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Plucked_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Plucked_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pluck"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Plucked_pluck, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Plucked_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Plucked_noteOff, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Plucked;
  
  /*const Type*/ t_PoleZero = gwi_class_ini(gwi, "PoleZero", "Filter");
  gwi_class_xtor(gwi, gw_PoleZero_ctor, gw_PoleZero_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setB0"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_PoleZero_setB0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_PoleZero_setB1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setA1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_PoleZero_setA1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg5"));
  CHECK_BB(gwi_func_end(gwi, gw_PoleZero_setCoefficients0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_PoleZero_setCoefficients1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setAllpass"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_PoleZero_setAllpass, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBlockZero"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_PoleZero_setBlockZero0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBlockZero"));
  CHECK_BB(gwi_func_end(gwi, gw_PoleZero_setBlockZero1, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// PoleZero;
  
  /*const Type*/ t_PRCRev = gwi_class_ini(gwi, "PRCRev", "Effect");
  gwi_class_xtor(gwi, gw_PRCRev_ctor1, gw_PRCRev_dtor);
  CHECK_BB(gwi_func_ini(gwi, "PRCRev", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_PRCRev_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_PRCRev_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setT60"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_PRCRev_setT60, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// PRCRev;
  
  /*const Type*/ t_Recorder = gwi_class_ini(gwi, "Recorder", "Instrmnt");
  gwi_class_xtor(gwi, gw_Recorder_ctor, gw_Recorder_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_startBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_stopBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_controlChange, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBlowPressure"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_setBlowPressure, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVibratoGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_setVibratoGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVibratoFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_setVibratoFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setNoiseGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_setNoiseGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBreathCutoff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_setBreathCutoff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSoftness"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Recorder_setSoftness, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Recorder;
  
  /*const Type*/ t_ReedTable = gwi_class_ini(gwi, "ReedTable", "Function");
  gwi_class_xtor(gwi, gw_ReedTable_ctor, gw_ReedTable_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setOffset"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ReedTable_setOffset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSlope"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_ReedTable_setSlope, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// ReedTable;
  
  /*const Type*/ t_Resonate = gwi_class_ini(gwi, "Resonate", "Instrmnt");
  gwi_class_xtor(gwi, gw_Resonate_ctor, gw_Resonate_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setResonance"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Resonate_setResonance, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setNotch"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Resonate_setNotch, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setEqualGainZeroes"));
  CHECK_BB(gwi_func_end(gwi, gw_Resonate_setEqualGainZeroes, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOn"));
  CHECK_BB(gwi_func_end(gwi, gw_Resonate_keyOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOff"));
  CHECK_BB(gwi_func_end(gwi, gw_Resonate_keyOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Resonate_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Resonate_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Resonate_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Resonate;
  
  /*const Type*/ t_Rhodey = gwi_class_ini(gwi, "Rhodey", "FM");
  gwi_class_xtor(gwi, gw_Rhodey_ctor, gw_Rhodey_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Rhodey_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Rhodey_noteOn, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Rhodey;
  
  /*const Type*/ t_Saxofony = gwi_class_ini(gwi, "Saxofony", "Instrmnt");
  SET_FLAG(t_Saxofony, abstract);
  gwi_class_xtor(gwi, NULL, gw_Saxofony_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Saxofony", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_ctor, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBlowPosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_setBlowPosition, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_startBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_stopBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Saxofony_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Saxofony;
  
  /*const Type*/ t_Shakers = gwi_class_ini(gwi, "Shakers", "Instrmnt");
  gwi_class_xtor(gwi, gw_Shakers_ctor1, gw_Shakers_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Shakers", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Shakers_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Shakers_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Shakers_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Shakers_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Shakers;
  
  
  CHECK_BB(gwi_func_ini(gwi, "float", "MIN_ENERGY"));
  CHECK_BB(gwi_func_end(gwi, gw_MIN_ENERGY_get, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "float", "WATER_FREQ_SWEEP"));
  CHECK_BB(gwi_func_end(gwi, gw_WATER_FREQ_SWEEP_get, ae_flag_static));
/*const Type*/ t_Simple = gwi_class_ini(gwi, "Simple", "Instrmnt");;
  gwi_class_xtor(gwi, gw_Simple_ctor, gw_Simple_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Simple_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOn"));
  CHECK_BB(gwi_func_end(gwi, gw_Simple_keyOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "keyOff"));
  CHECK_BB(gwi_func_end(gwi, gw_Simple_keyOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Simple_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Simple_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Simple_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Simple;
  
  
  CHECK_BB(gwi_func_ini(gwi, "int", "TABLE_SIZE"));
  CHECK_BB(gwi_func_end(gwi, gw_TABLE_SIZE_get, ae_flag_static));
/*const Type*/ t_SineWave = gwi_class_ini(gwi, "SineWave", "Generator");;
  gwi_class_xtor(gwi, gw_SineWave_ctor, gw_SineWave_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "reset"));
  CHECK_BB(gwi_func_end(gwi, gw_SineWave_reset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SineWave_setRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SineWave_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "addTime"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SineWave_addTime, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "addPhase"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SineWave_addPhase, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "addPhaseOffset"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SineWave_addPhaseOffset, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// SineWave;
  
  /*const Type*/ t_SingWave = gwi_class_ini(gwi, "SingWave", "Generator");
  SET_FLAG(t_SingWave, abstract);
  gwi_class_xtor(gwi, NULL, gw_SingWave_dtor);
  CHECK_BB(gwi_func_ini(gwi, "SingWave", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "SingWave", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_ctor1, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "reset"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_reset, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "normalize"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_normalize0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "normalize"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_normalize1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVibratoRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_setVibratoRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVibratoGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_setVibratoGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setRandomGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_setRandomGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setSweepRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_setSweepRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGainRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_setGainRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setGainTarget"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_setGainTarget, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_end(gwi, gw_SingWave_noteOff, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// SingWave;
  
  /*const Type*/ t_Sitar = gwi_class_ini(gwi, "Sitar", "Instrmnt");
  gwi_class_xtor(gwi, gw_Sitar_ctor1, gw_Sitar_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Sitar", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Sitar_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Sitar_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Sitar_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pluck"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Sitar_pluck, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Sitar_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Sitar_noteOff, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Sitar;
  
  /*const Type*/ t_Vector3D = gwi_class_ini(gwi, "Vector3D", "Stk");
  gwi_class_xtor(gwi, gw_Vector3D_ctor3, gw_Vector3D_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Vector3D", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "Vector3D", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_ctor1, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "Vector3D", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_ctor2, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "float", "getX"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_getX, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getY"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_getY, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getZ"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_getZ, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "float", "getLength"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_getLength, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setXYZ"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_setXYZ, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setX"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_setX, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setY"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_setY, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setZ"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Vector3D_setZ, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Vector3D;
  
  /*const Type*/ t_StifKarp = gwi_class_ini(gwi, "StifKarp", "Instrmnt");
  gwi_class_xtor(gwi, gw_StifKarp_ctor1, gw_StifKarp_dtor);
  CHECK_BB(gwi_func_ini(gwi, "StifKarp", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setStretch"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_setStretch, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPickupPosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_setPickupPosition, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setBaseLoopGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_setBaseLoopGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pluck"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_pluck, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_StifKarp_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// StifKarp;
  
  /*const Type*/ t_TubeBell = gwi_class_ini(gwi, "TubeBell", "FM");
  gwi_class_xtor(gwi, gw_TubeBell_ctor, gw_TubeBell_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_TubeBell_noteOn, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// TubeBell;
  
  /*const Type*/ t_Twang = gwi_class_ini(gwi, "Twang", "Stk");
  gwi_class_xtor(gwi, gw_Twang_ctor1, gw_Twang_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Twang", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Twang_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Twang_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setLowestFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Twang_setLowestFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Twang_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPluckPosition"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Twang_setPluckPosition, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setLoopGain"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Twang_setLoopGain, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setLoopFilter"));
  CHECK_BB(gwi_func_arg(gwi, "float[]", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Twang_setLoopFilter, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Twang;
  
  /*const Type*/ t_TwoPole = gwi_class_ini(gwi, "TwoPole", "Filter");
  gwi_class_xtor(gwi, gw_TwoPole_ctor, gw_TwoPole_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_ignoreSampleRateChange0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_ignoreSampleRateChange1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB0"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_setB0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setA1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_setA1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setA2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_setA2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg5"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_setCoefficients0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_setCoefficients1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setResonance"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_setResonance0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setResonance"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoPole_setResonance1, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// TwoPole;
  
  /*const Type*/ t_TwoZero = gwi_class_ini(gwi, "TwoZero", "Filter");
  gwi_class_xtor(gwi, gw_TwoZero_ctor, gw_TwoZero_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoZero_ignoreSampleRateChange0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoZero_ignoreSampleRateChange1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB0"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoZero_setB0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB1"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoZero_setB1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setB2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoZero_setB2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg5"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoZero_setCoefficients0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setCoefficients"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoZero_setCoefficients1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setNotch"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_TwoZero_setNotch, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// TwoZero;
  
  /*const Type*/ t_Voicer = gwi_class_ini(gwi, "Voicer", "Stk");
  gwi_class_xtor(gwi, gw_Voicer_ctor1, gw_Voicer_dtor);
  CHECK_BB(gwi_func_ini(gwi, "Voicer", "ctor"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg1"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_ctor0, ae_flag_static));
  CHECK_BB(gwi_func_ini(gwi, "void", "addInstrument"));
  CHECK_BB(gwi_func_arg(gwi, "stk.Instrmnt", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_addInstrument0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "addInstrument"));
  CHECK_BB(gwi_func_arg(gwi, "stk.Instrmnt", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_addInstrument1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "removeInstrument"));
  CHECK_BB(gwi_func_arg(gwi, "stk.Instrmnt", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_removeInstrument, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_noteOn0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_noteOn1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_noteOff0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_noteOff1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_noteOff2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_setFrequency0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_setFrequency1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_setFrequency2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pitchBend"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_pitchBend0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pitchBend"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_pitchBend1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "pitchBend"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_pitchBend2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_controlChange0, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_controlChange1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg3"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg4"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_controlChange2, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "silence"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_silence, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "channelsOut"));
  CHECK_BB(gwi_func_end(gwi, gw_Voicer_channelsOut, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Voicer;
  
  /*const Type*/ t_VoicForm = gwi_class_ini(gwi, "VoicForm", "Instrmnt");
  gwi_class_xtor(gwi, gw_VoicForm_ctor, gw_VoicForm_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "setPhoneme"));
  CHECK_BB(gwi_func_arg(gwi, "string", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_setPhoneme, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setVoiced"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_setVoiced, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setUnVoiced"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_setUnVoiced, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFilterSweepRate"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_setFilterSweepRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setPitchSweepRate"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_setPitchSweepRate, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "speak"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_speak, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "quiet"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_quiet, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_VoicForm_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// VoicForm;
  
  /*const Type*/ t_Whistle = gwi_class_ini(gwi, "Whistle", "Instrmnt");
  gwi_class_xtor(gwi, gw_Whistle_ctor, gw_Whistle_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "clear"));
  CHECK_BB(gwi_func_end(gwi, gw_Whistle_clear, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Whistle_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "startBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Whistle_startBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "stopBlowing"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Whistle_stopBlowing, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Whistle_noteOn, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOff"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Whistle_noteOff, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "controlChange"));
  CHECK_BB(gwi_func_arg(gwi, "int", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Whistle_controlChange, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Whistle;
  
  /*const Type*/ t_Wurley = gwi_class_ini(gwi, "Wurley", "FM");
  gwi_class_xtor(gwi, gw_Wurley_ctor, gw_Wurley_dtor);
  CHECK_BB(gwi_func_ini(gwi, "void", "setFrequency"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_end(gwi, gw_Wurley_setFrequency, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "noteOn"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg2"));
  CHECK_BB(gwi_func_arg(gwi, "float", "arg3"));
  CHECK_BB(gwi_func_end(gwi, gw_Wurley_noteOn, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi));// Wurley;
  
  CHECK_BB(gwi_class_end(gwi));;
  return GW_OK;
}
extern "C" GWION_IMPORT(STK){
  return CPPIMPORT(gwi);
}

