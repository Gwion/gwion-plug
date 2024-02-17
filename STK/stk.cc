#include <utility>
#define SWIG_STD_MOVE(i) std::move(i)
#include "Gwion.hh"
extern "C" { ANEW UGen new_UGen(MemPool p);}
#include "Stk.h"
#include "Instrmnt.h"
#include "Generator.h"
#include "FM.h"
#include "Envelope.h"
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
#include "WvIn.h"
#include "FileWvIn.h"
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
#define GW_Stk(o) *(void**)(o->data)
static SFUN(gw_Stk_STK_SINT8_get) {
  stk::Stk::StkFormat const result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_SINT8;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_SINT16_get) {
  stk::Stk::StkFormat const result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_SINT16;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_SINT24_get) {
  stk::Stk::StkFormat const result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_SINT24;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_SINT32_get) {
  stk::Stk::StkFormat const result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_SINT32;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_FLOAT32_get) {
  stk::Stk::StkFormat const result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_FLOAT32;
  *(m_int*)RETURN = (m_int)result;
}

static SFUN(gw_Stk_STK_FLOAT64_get) {
  stk::Stk::StkFormat const result = (stk::Stk::StkFormat)(stk::Stk::StkFormat)stk::Stk::STK_FLOAT64;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Stk_sampleRate) {
  stk::StkFloat result = (stk::StkFloat)stk::Stk::sampleRate();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Stk_setSampleRate) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Stk::setSampleRate(arg1);
}

static MFUN(gw_Stk_ignoreSampleRateChange0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Stk * arg1 = *(stk::Stk **)(temp1->data);
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_Stk_ignoreSampleRateChange1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Stk * arg1 = *(stk::Stk **)(temp1->data);
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_Stk_clear_alertList) {
  stk::Stk::clear_alertList();
}

static MFUN(gw_Stk_rawwavePath) {
  std::string result = stk::Stk::rawwavePath();
  *(M_Object*)RETURN = (&result)->data() ? new_string(shred->info->vm->gwion, (m_str)(&result)->data()) : NULL;
}

static MFUN(gw_Stk_setRawwavePath) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  stk::Stk::setRawwavePath(SWIG_STD_MOVE(arg1));
}

static MFUN(gw_Stk_sleep) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  unsigned long arg1 = *(unsigned long*)(temp1->data);
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

static MFUN(gw_SRATE_get) {
  stk::StkFloat const result = (stk::StkFloat)(stk::StkFloat)stk::SRATE;
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_RT_BUFFER_SIZE_get) {
  unsigned int const result = (unsigned int)(unsigned int)stk::RT_BUFFER_SIZE;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_PI_get) {
  stk::StkFloat const result = (stk::StkFloat)(stk::StkFloat)stk::PI;
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_TWO_PI_get) {
  stk::StkFloat const result = (stk::StkFloat)(stk::StkFloat)stk::TWO_PI;
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_ONE_OVER_128_get) {
  stk::StkFloat const result = (stk::StkFloat)(stk::StkFloat)stk::ONE_OVER_128;
  *(m_float*)RETURN = (m_float)(double)result;
}

static TICK(Instrmnt_tick) {
  stk::Instrmnt *s = (stk::Instrmnt*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Instrmnt_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Instrmnt * arg1 = (stk::Instrmnt *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Instrmnt_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Instrmnt * arg1 = (stk::Instrmnt *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Instrmnt_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Instrmnt * arg1 = (stk::Instrmnt *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Instrmnt_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Instrmnt * arg1 = (stk::Instrmnt *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Instrmnt_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Instrmnt * arg1 = (stk::Instrmnt *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static MFUN(gw_Instrmnt_channelsOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Instrmnt * arg1 = (stk::Instrmnt *)UGEN(temp1)->module.gen.data;
  unsigned int result = (unsigned int)((stk::Instrmnt const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static DTOR(gw_Instrmnt_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Instrmnt*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

#define GW_Generator(o) *(void**)(o->data)
static MFUN(gw_Generator_channelsOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Generator * arg1 = *(stk::Generator **)(temp1->data);
  unsigned int result = (unsigned int)((stk::Generator const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static DTOR(gw_Generator_dtor) {
  if(GW_Generator(o)) delete (stk::Generator*)GW_Generator(o);
  GW_Generator(o) = NULL;
}

static TICK(FM_tick) {
  stk::FM *s = (stk::FM*)u->module.gen.data;
  u->out = s->tick(1);
}

static DTOR(gw_FM_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::FM*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_FM_loadWaves) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  char ** arg2 = *(char ***)(temp2->data);
  (arg1)->loadWaves((char const **)arg2);
}

static MFUN(gw_FM_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_FM_setRatio) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setRatio(arg2,arg3);
}

static MFUN(gw_FM_setGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setGain(arg2,arg3);
}

static MFUN(gw_FM_setModulationSpeed) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationSpeed(arg2);
}

static MFUN(gw_FM_setModulationDepth) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationDepth(arg2);
}

static MFUN(gw_FM_setControl1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setControl1(arg2);
}

static MFUN(gw_FM_setControl2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setControl2(arg2);
}

static MFUN(gw_FM_keyOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOn();
}

static MFUN(gw_FM_keyOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOff();
}

static MFUN(gw_FM_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_FM_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FM * arg1 = (stk::FM *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Envelope_tick) {
  stk::Envelope *s = (stk::Envelope*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_Envelope_ctor) {
  stk::Envelope * result = (stk::Envelope *)new stk::Envelope();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Envelope_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Envelope_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Envelope*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Envelope_keyOn0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->keyOn(arg2);
}

static MFUN(gw_Envelope_keyOn1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOn();
}

static MFUN(gw_Envelope_keyOff0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->keyOff(arg2);
}

static MFUN(gw_Envelope_keyOff1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOff();
}

static MFUN(gw_Envelope_setRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRate(arg2);
}

static MFUN(gw_Envelope_setTime) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTime(arg2);
}

static MFUN(gw_Envelope_setTarget) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTarget(arg2);
}

static MFUN(gw_Envelope_setValue) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setValue(arg2);
}

static MFUN(gw_Envelope_getState) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Envelope * arg1 = (stk::Envelope *)UGEN(temp1)->module.gen.data;
  int result = (int)((stk::Envelope const *)arg1)->getState();
  *(m_int*)RETURN = (m_int)result;
}

#define GW_Filter(o) *(void**)(o->data)
static MFUN(gw_Filter_channelsIn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Filter * arg1 = *(stk::Filter **)(temp1->data);
  unsigned int result = (unsigned int)((stk::Filter const *)arg1)->channelsIn();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Filter_channelsOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Filter * arg1 = *(stk::Filter **)(temp1->data);
  unsigned int result = (unsigned int)((stk::Filter const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Filter_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Filter * arg1 = *(stk::Filter **)(temp1->data);
  (arg1)->clear();
}

static MFUN(gw_Filter_setGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Filter * arg1 = *(stk::Filter **)(temp1->data);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setGain(arg2);
}

static MFUN(gw_Filter_getGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Filter * arg1 = *(stk::Filter **)(temp1->data);
  stk::StkFloat result = (stk::StkFloat)((stk::Filter const *)arg1)->getGain();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Filter_phaseDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Filter * arg1 = *(stk::Filter **)(temp1->data);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)(arg1)->phaseDelay(arg2);
  *(m_float*)RETURN = (m_float)(double)result;
}

static DTOR(gw_Filter_dtor) {
  if(GW_Filter(o)) delete (stk::Filter*)GW_Filter(o);
  GW_Filter(o) = NULL;
}

#define GW_Effect(o) *(void**)(o->data)
static MFUN(gw_Effect_channelsOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Effect * arg1 = *(stk::Effect **)(temp1->data);
  unsigned int result = (unsigned int)((stk::Effect const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Effect_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Effect * arg1 = *(stk::Effect **)(temp1->data);
  (arg1)->clear();
}

static MFUN(gw_Effect_setEffectMix) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Effect * arg1 = *(stk::Effect **)(temp1->data);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setEffectMix(arg2);
}

static DTOR(gw_Effect_dtor) {
  if(GW_Effect(o)) delete (stk::Effect*)GW_Effect(o);
  GW_Effect(o) = NULL;
}

#define GW_Function(o) *(void**)(o->data)
static MFUN(gw_Function_lastOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Function * arg1 = *(stk::Function **)(temp1->data);
  stk::StkFloat result = (stk::StkFloat)((stk::Function const *)arg1)->lastOut();
  *(m_float*)RETURN = (m_float)(double)result;
}

static DTOR(gw_Function_dtor) {
  if(GW_Function(o)) delete (stk::Function*)GW_Function(o);
  GW_Function(o) = NULL;
}

static TICK(Sampler_tick) {
  stk::Sampler *s = (stk::Sampler*)u->module.gen.data;
  u->out = s->tick(1);
}

static DTOR(gw_Sampler_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Sampler*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Sampler_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sampler * arg1 = (stk::Sampler *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Sampler_keyOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sampler * arg1 = (stk::Sampler *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOn();
}

static MFUN(gw_Sampler_keyOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sampler * arg1 = (stk::Sampler *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOff();
}

static MFUN(gw_Sampler_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sampler * arg1 = (stk::Sampler *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Sampler_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sampler * arg1 = (stk::Sampler *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(ADSR_tick) {
  stk::ADSR *s = (stk::ADSR*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_ADSR_ctor) {
  stk::ADSR * result = (stk::ADSR *)new stk::ADSR();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), ADSR_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_ADSR_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::ADSR*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_ADSR_keyOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOn();
}

static MFUN(gw_ADSR_keyOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOff();
}

static MFUN(gw_ADSR_setAttackRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setAttackRate(arg2);
}

static MFUN(gw_ADSR_setAttackTarget) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setAttackTarget(arg2);
}

static MFUN(gw_ADSR_setDecayRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDecayRate(arg2);
}

static MFUN(gw_ADSR_setSustainLevel) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSustainLevel(arg2);
}

static MFUN(gw_ADSR_setReleaseRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setReleaseRate(arg2);
}

static MFUN(gw_ADSR_setAttackTime) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setAttackTime(arg2);
}

static MFUN(gw_ADSR_setDecayTime) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDecayTime(arg2);
}

static MFUN(gw_ADSR_setReleaseTime) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setReleaseTime(arg2);
}

static MFUN(gw_ADSR_setAllTimes) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg5 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setAllTimes(arg2,arg3,arg4,arg5);
}

static MFUN(gw_ADSR_setTarget) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTarget(arg2);
}

static MFUN(gw_ADSR_getState) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  int result = (int)((stk::ADSR const *)arg1)->getState();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_ADSR_setValue) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ADSR * arg1 = (stk::ADSR *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setValue(arg2);
}

static TICK(Asymp_tick) {
  stk::Asymp *s = (stk::Asymp*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_TARGET_THRESHOLD_get) {
  stk::StkFloat const result = (stk::StkFloat)(stk::StkFloat)stk::TARGET_THRESHOLD;
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Asymp_ctor) {
  stk::Asymp * result = (stk::Asymp *)new stk::Asymp();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Asymp_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Asymp_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Asymp*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Asymp_keyOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Asymp * arg1 = (stk::Asymp *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOn();
}

static MFUN(gw_Asymp_keyOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Asymp * arg1 = (stk::Asymp *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOff();
}

static MFUN(gw_Asymp_setTau) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Asymp * arg1 = (stk::Asymp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTau(arg2);
}

static MFUN(gw_Asymp_setTime) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Asymp * arg1 = (stk::Asymp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTime(arg2);
}

static MFUN(gw_Asymp_setT60) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Asymp * arg1 = (stk::Asymp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setT60(arg2);
}

static MFUN(gw_Asymp_setTarget) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Asymp * arg1 = (stk::Asymp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTarget(arg2);
}

static MFUN(gw_Asymp_setValue) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Asymp * arg1 = (stk::Asymp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setValue(arg2);
}

static MFUN(gw_Asymp_getState) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Asymp * arg1 = (stk::Asymp *)UGEN(temp1)->module.gen.data;
  int result = (int)((stk::Asymp const *)arg1)->getState();
  *(m_int*)RETURN = (m_int)result;
}

static TICK(BandedWG_tick) {
  stk::BandedWG *s = (stk::BandedWG*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_MAX_BANDED_MODES_get) {
  int const result = (int)(int)stk::MAX_BANDED_MODES;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_BandedWG_ctor) {
  stk::BandedWG * result = (stk::BandedWG *)new stk::BandedWG();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BandedWG_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_BandedWG_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::BandedWG*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_BandedWG_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_BandedWG_setStrikePosition) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setStrikePosition(arg2);
}

static MFUN(gw_BandedWG_setPreset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setPreset(arg2);
}

static MFUN(gw_BandedWG_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BandedWG_startBowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBowing(arg2,arg3);
}

static MFUN(gw_BandedWG_stopBowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBowing(arg2);
}

static MFUN(gw_BandedWG_pluck) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_BandedWG_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_BandedWG_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_BandedWG_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BandedWG * arg1 = (stk::BandedWG *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(BiQuad_tick) {
  stk::BiQuad *s = (stk::BiQuad*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_RECIP_SQRT_2_get) {
  stk::StkFloat const result = (stk::StkFloat)(stk::StkFloat)stk::RECIP_SQRT_2;
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_BiQuad_ctor) {
  stk::BiQuad * result = (stk::BiQuad *)new stk::BiQuad();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BiQuad_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_BiQuad_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::BiQuad*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_BiQuad_ignoreSampleRateChange0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_BiQuad_ignoreSampleRateChange1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_BiQuad_setCoefficients0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg5 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg6 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  bool arg7 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5,arg6,arg7);
}

static MFUN(gw_BiQuad_setCoefficients1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg5 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  stk::StkFloat arg6 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5,arg6);
}

static MFUN(gw_BiQuad_setB0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_BiQuad_setB1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB1(arg2);
}

static MFUN(gw_BiQuad_setB2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB2(arg2);
}

static MFUN(gw_BiQuad_setA1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_BiQuad_setA2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA2(arg2);
}

static MFUN(gw_BiQuad_setResonance0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  bool arg4 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3,arg4);
}

static MFUN(gw_BiQuad_setResonance1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3);
}

static MFUN(gw_BiQuad_setNotch) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setNotch(arg2,arg3);
}

static MFUN(gw_BiQuad_setLowPass0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setLowPass(arg2,arg3);
}

static MFUN(gw_BiQuad_setLowPass1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLowPass(arg2);
}

static MFUN(gw_BiQuad_setHighPass0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setHighPass(arg2,arg3);
}

static MFUN(gw_BiQuad_setHighPass1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setHighPass(arg2);
}

static MFUN(gw_BiQuad_setBandPass) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setBandPass(arg2,arg3);
}

static MFUN(gw_BiQuad_setBandReject) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setBandReject(arg2,arg3);
}

static MFUN(gw_BiQuad_setAllPass) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setAllPass(arg2,arg3);
}

static MFUN(gw_BiQuad_setEqualGainZeroes) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BiQuad * arg1 = (stk::BiQuad *)UGEN(temp1)->module.gen.data;
  (arg1)->setEqualGainZeroes();
}

static TICK(Blit_tick) {
  stk::Blit *s = (stk::Blit*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_Blit_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Blit * result = (stk::Blit *)new stk::Blit(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Blit_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Blit_ctor1) {
  stk::Blit * result = (stk::Blit *)new stk::Blit();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Blit_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Blit_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Blit*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Blit_reset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Blit * arg1 = (stk::Blit *)UGEN(temp1)->module.gen.data;
  (arg1)->reset();
}

static MFUN(gw_Blit_setPhase) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Blit * arg1 = (stk::Blit *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPhase(arg2);
}

static MFUN(gw_Blit_getPhase) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Blit * arg1 = (stk::Blit *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)((stk::Blit const *)arg1)->getPhase();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Blit_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Blit * arg1 = (stk::Blit *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Blit_setHarmonics0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Blit * arg1 = (stk::Blit *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setHarmonics(arg2);
}

static MFUN(gw_Blit_setHarmonics1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Blit * arg1 = (stk::Blit *)UGEN(temp1)->module.gen.data;
  (arg1)->setHarmonics();
}

static TICK(BlitSaw_tick) {
  stk::BlitSaw *s = (stk::BlitSaw*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_BlitSaw_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::BlitSaw * result = (stk::BlitSaw *)new stk::BlitSaw(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlitSaw_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_BlitSaw_ctor1) {
  stk::BlitSaw * result = (stk::BlitSaw *)new stk::BlitSaw();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlitSaw_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_BlitSaw_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::BlitSaw*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_BlitSaw_reset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSaw * arg1 = (stk::BlitSaw *)UGEN(temp1)->module.gen.data;
  (arg1)->reset();
}

static MFUN(gw_BlitSaw_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSaw * arg1 = (stk::BlitSaw *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BlitSaw_setHarmonics0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSaw * arg1 = (stk::BlitSaw *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setHarmonics(arg2);
}

static MFUN(gw_BlitSaw_setHarmonics1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSaw * arg1 = (stk::BlitSaw *)UGEN(temp1)->module.gen.data;
  (arg1)->setHarmonics();
}

static TICK(BlitSquare_tick) {
  stk::BlitSquare *s = (stk::BlitSquare*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_BlitSquare_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::BlitSquare * result = (stk::BlitSquare *)new stk::BlitSquare(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlitSquare_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_BlitSquare_ctor1) {
  stk::BlitSquare * result = (stk::BlitSquare *)new stk::BlitSquare();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlitSquare_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_BlitSquare_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::BlitSquare*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_BlitSquare_reset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSquare * arg1 = (stk::BlitSquare *)UGEN(temp1)->module.gen.data;
  (arg1)->reset();
}

static MFUN(gw_BlitSquare_setPhase) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSquare * arg1 = (stk::BlitSquare *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPhase(arg2);
}

static MFUN(gw_BlitSquare_getPhase) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSquare * arg1 = (stk::BlitSquare *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)((stk::BlitSquare const *)arg1)->getPhase();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_BlitSquare_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSquare * arg1 = (stk::BlitSquare *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BlitSquare_setHarmonics0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSquare * arg1 = (stk::BlitSquare *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setHarmonics(arg2);
}

static MFUN(gw_BlitSquare_setHarmonics1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlitSquare * arg1 = (stk::BlitSquare *)UGEN(temp1)->module.gen.data;
  (arg1)->setHarmonics();
}

static TICK(BeeThree_tick) {
  stk::BeeThree *s = (stk::BeeThree*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_BeeThree_ctor) {
  stk::BeeThree * result = (stk::BeeThree *)new stk::BeeThree();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BeeThree_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_BeeThree_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::BeeThree*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_BeeThree_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BeeThree * arg1 = (stk::BeeThree *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static TICK(BlowBotl_tick) {
  stk::BlowBotl *s = (stk::BlowBotl*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_BlowBotl_ctor) {
  stk::BlowBotl * result = (stk::BlowBotl *)new stk::BlowBotl();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlowBotl_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_BlowBotl_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::BlowBotl*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_BlowBotl_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowBotl * arg1 = (stk::BlowBotl *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_BlowBotl_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowBotl * arg1 = (stk::BlowBotl *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BlowBotl_startBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowBotl * arg1 = (stk::BlowBotl *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_BlowBotl_stopBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowBotl * arg1 = (stk::BlowBotl *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_BlowBotl_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowBotl * arg1 = (stk::BlowBotl *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_BlowBotl_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowBotl * arg1 = (stk::BlowBotl *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_BlowBotl_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowBotl * arg1 = (stk::BlowBotl *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(BlowHole_tick) {
  stk::BlowHole *s = (stk::BlowHole*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_BlowHole_ctor) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::BlowHole * result = (stk::BlowHole *)new stk::BlowHole(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BlowHole_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_BlowHole_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::BlowHole*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_BlowHole_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_BlowHole_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_BlowHole_setTonehole) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setTonehole(arg2);
}

static MFUN(gw_BlowHole_setVent) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVent(arg2);
}

static MFUN(gw_BlowHole_startBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_BlowHole_stopBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_BlowHole_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_BlowHole_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_BlowHole_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BlowHole * arg1 = (stk::BlowHole *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Bowed_tick) {
  stk::Bowed *s = (stk::Bowed*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Bowed_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Bowed * result = (stk::Bowed *)new stk::Bowed(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Bowed_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Bowed_ctor1) {
  stk::Bowed * result = (stk::Bowed *)new stk::Bowed();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Bowed_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Bowed_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Bowed*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Bowed_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Bowed * arg1 = (stk::Bowed *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Bowed_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Bowed * arg1 = (stk::Bowed *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Bowed_setVibrato) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Bowed * arg1 = (stk::Bowed *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibrato(arg2);
}

static MFUN(gw_Bowed_startBowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Bowed * arg1 = (stk::Bowed *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBowing(arg2,arg3);
}

static MFUN(gw_Bowed_stopBowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Bowed * arg1 = (stk::Bowed *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBowing(arg2);
}

static MFUN(gw_Bowed_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Bowed * arg1 = (stk::Bowed *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Bowed_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Bowed * arg1 = (stk::Bowed *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Bowed_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Bowed * arg1 = (stk::Bowed *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(BowTable_tick) {
  stk::BowTable *s = (stk::BowTable*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_BowTable_ctor) {
  stk::BowTable * result = (stk::BowTable *)new stk::BowTable();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), BowTable_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_BowTable_setOffset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BowTable * arg1 = (stk::BowTable *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setOffset(arg2);
}

static MFUN(gw_BowTable_setSlope) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BowTable * arg1 = (stk::BowTable *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSlope(arg2);
}

static MFUN(gw_BowTable_setMinOutput) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BowTable * arg1 = (stk::BowTable *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setMinOutput(arg2);
}

static MFUN(gw_BowTable_setMaxOutput) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::BowTable * arg1 = (stk::BowTable *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setMaxOutput(arg2);
}

static DTOR(gw_BowTable_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::BowTable*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(Brass_tick) {
  stk::Brass *s = (stk::Brass*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Brass_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Brass * result = (stk::Brass *)new stk::Brass(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Brass_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Brass_ctor1) {
  stk::Brass * result = (stk::Brass *)new stk::Brass();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Brass_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Brass_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Brass*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Brass_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Brass * arg1 = (stk::Brass *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Brass_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Brass * arg1 = (stk::Brass *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Brass_setLip) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Brass * arg1 = (stk::Brass *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLip(arg2);
}

static MFUN(gw_Brass_startBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Brass * arg1 = (stk::Brass *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Brass_stopBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Brass * arg1 = (stk::Brass *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Brass_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Brass * arg1 = (stk::Brass *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Brass_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Brass * arg1 = (stk::Brass *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Brass_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Brass * arg1 = (stk::Brass *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Chorus_tick) {
  stk::Chorus *s = (stk::Chorus*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Chorus_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Chorus * result = (stk::Chorus *)new stk::Chorus(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Chorus_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Chorus_ctor1) {
  stk::Chorus * result = (stk::Chorus *)new stk::Chorus();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Chorus_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Chorus_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Chorus * arg1 = (stk::Chorus *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Chorus_setModDepth) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Chorus * arg1 = (stk::Chorus *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModDepth(arg2);
}

static MFUN(gw_Chorus_setModFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Chorus * arg1 = (stk::Chorus *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModFrequency(arg2);
}

static DTOR(gw_Chorus_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Chorus*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(Clarinet_tick) {
  stk::Clarinet *s = (stk::Clarinet*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Clarinet_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Clarinet * result = (stk::Clarinet *)new stk::Clarinet(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Clarinet_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Clarinet_ctor1) {
  stk::Clarinet * result = (stk::Clarinet *)new stk::Clarinet();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Clarinet_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Clarinet_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Clarinet*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Clarinet_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Clarinet * arg1 = (stk::Clarinet *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Clarinet_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Clarinet * arg1 = (stk::Clarinet *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Clarinet_startBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Clarinet * arg1 = (stk::Clarinet *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Clarinet_stopBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Clarinet * arg1 = (stk::Clarinet *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Clarinet_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Clarinet * arg1 = (stk::Clarinet *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Clarinet_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Clarinet * arg1 = (stk::Clarinet *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Clarinet_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Clarinet * arg1 = (stk::Clarinet *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Cubic_tick) {
  stk::Cubic *s = (stk::Cubic*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Cubic_ctor) {
  stk::Cubic * result = (stk::Cubic *)new stk::Cubic();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Cubic_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Cubic_setA1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Cubic * arg1 = (stk::Cubic *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_Cubic_setA2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Cubic * arg1 = (stk::Cubic *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA2(arg2);
}

static MFUN(gw_Cubic_setA3) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Cubic * arg1 = (stk::Cubic *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA3(arg2);
}

static MFUN(gw_Cubic_setGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Cubic * arg1 = (stk::Cubic *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setGain(arg2);
}

static MFUN(gw_Cubic_setThreshold) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Cubic * arg1 = (stk::Cubic *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setThreshold(arg2);
}

static DTOR(gw_Cubic_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Cubic*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(Delay_tick) {
  stk::Delay *s = (stk::Delay*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Delay_ctor0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  unsigned long arg1 = *(unsigned long*)(temp1->data);
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  stk::Delay * result = (stk::Delay *)new stk::Delay(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Delay_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Delay_ctor1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  unsigned long arg1 = *(unsigned long*)(temp1->data);
  stk::Delay * result = (stk::Delay *)new stk::Delay(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Delay_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Delay_ctor2) {
  stk::Delay * result = (stk::Delay *)new stk::Delay();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Delay_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Delay_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Delay*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Delay_getMaximumDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  unsigned long result = (unsigned long)(arg1)->getMaximumDelay();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Delay_setMaximumDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  (arg1)->setMaximumDelay(arg2);
}

static MFUN(gw_Delay_setDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  (arg1)->setDelay(arg2);
}

static MFUN(gw_Delay_getDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  unsigned long result = (unsigned long)((stk::Delay const *)arg1)->getDelay();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Delay_tapOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  stk::StkFloat result = (stk::StkFloat)(arg1)->tapOut(arg2);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Delay_tapIn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  const M_Object temp3 = *(M_Object*)MEM(0+SZ_INT+SZ_FLOAT);
  unsigned long arg3 = *(unsigned long*)(temp3->data);
  (arg1)->tapIn(arg2,arg3);
}

static MFUN(gw_Delay_addTo) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  const M_Object temp3 = *(M_Object*)MEM(0+SZ_INT+SZ_FLOAT);
  unsigned long arg3 = *(unsigned long*)(temp3->data);
  stk::StkFloat result = (stk::StkFloat)(arg1)->addTo(arg2,arg3);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Delay_nextOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)(arg1)->nextOut();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Delay_energy) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Delay * arg1 = (stk::Delay *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)((stk::Delay const *)arg1)->energy();
  *(m_float*)RETURN = (m_float)(double)result;
}

static TICK(DelayA_tick) {
  stk::DelayA *s = (stk::DelayA*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_DelayA_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_FLOAT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  stk::DelayA * result = (stk::DelayA *)new stk::DelayA(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), DelayA_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_DelayA_ctor1) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::DelayA * result = (stk::DelayA *)new stk::DelayA(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), DelayA_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_DelayA_ctor2) {
  stk::DelayA * result = (stk::DelayA *)new stk::DelayA();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), DelayA_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_DelayA_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::DelayA*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_DelayA_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayA * arg1 = (stk::DelayA *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_DelayA_getMaximumDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayA * arg1 = (stk::DelayA *)UGEN(temp1)->module.gen.data;
  unsigned long result = (unsigned long)(arg1)->getMaximumDelay();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_DelayA_setMaximumDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayA * arg1 = (stk::DelayA *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  (arg1)->setMaximumDelay(arg2);
}

static MFUN(gw_DelayA_setDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayA * arg1 = (stk::DelayA *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDelay(arg2);
}

static MFUN(gw_DelayA_getDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayA * arg1 = (stk::DelayA *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)((stk::DelayA const *)arg1)->getDelay();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_DelayA_tapOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayA * arg1 = (stk::DelayA *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  stk::StkFloat result = (stk::StkFloat)(arg1)->tapOut(arg2);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_DelayA_tapIn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayA * arg1 = (stk::DelayA *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  const M_Object temp3 = *(M_Object*)MEM(0+SZ_INT+SZ_FLOAT);
  unsigned long arg3 = *(unsigned long*)(temp3->data);
  (arg1)->tapIn(arg2,arg3);
}

static MFUN(gw_DelayA_nextOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayA * arg1 = (stk::DelayA *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)(arg1)->nextOut();
  *(m_float*)RETURN = (m_float)(double)result;
}

static TICK(DelayL_tick) {
  stk::DelayL *s = (stk::DelayL*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_DelayL_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_FLOAT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  stk::DelayL * result = (stk::DelayL *)new stk::DelayL(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), DelayL_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_DelayL_ctor1) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::DelayL * result = (stk::DelayL *)new stk::DelayL(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), DelayL_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_DelayL_ctor2) {
  stk::DelayL * result = (stk::DelayL *)new stk::DelayL();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), DelayL_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_DelayL_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::DelayL*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_DelayL_getMaximumDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayL * arg1 = (stk::DelayL *)UGEN(temp1)->module.gen.data;
  unsigned long result = (unsigned long)(arg1)->getMaximumDelay();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_DelayL_setMaximumDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayL * arg1 = (stk::DelayL *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  (arg1)->setMaximumDelay(arg2);
}

static MFUN(gw_DelayL_setDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayL * arg1 = (stk::DelayL *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDelay(arg2);
}

static MFUN(gw_DelayL_getDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayL * arg1 = (stk::DelayL *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)((stk::DelayL const *)arg1)->getDelay();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_DelayL_tapOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayL * arg1 = (stk::DelayL *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  stk::StkFloat result = (stk::StkFloat)(arg1)->tapOut(arg2);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_DelayL_tapIn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayL * arg1 = (stk::DelayL *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  const M_Object temp3 = *(M_Object*)MEM(0+SZ_INT+SZ_FLOAT);
  unsigned long arg3 = *(unsigned long*)(temp3->data);
  (arg1)->tapIn(arg2,arg3);
}

static MFUN(gw_DelayL_nextOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::DelayL * arg1 = (stk::DelayL *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)(arg1)->nextOut();
  *(m_float*)RETURN = (m_float)(double)result;
}

static TICK(Drummer_tick) {
  stk::Drummer *s = (stk::Drummer*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_DRUM_NUMWAVES_get) {
  int const result = (int)(int)stk::DRUM_NUMWAVES;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_DRUM_POLYPHONY_get) {
  int const result = (int)(int)stk::DRUM_POLYPHONY;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Drummer_ctor) {
  stk::Drummer * result = (stk::Drummer *)new stk::Drummer();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Drummer_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Drummer_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Drummer*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Drummer_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Drummer * arg1 = (stk::Drummer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Drummer_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Drummer * arg1 = (stk::Drummer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static TICK(Echo_tick) {
  stk::Echo *s = (stk::Echo*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Echo_ctor0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  unsigned long arg1 = *(unsigned long*)(temp1->data);
  stk::Echo * result = (stk::Echo *)new stk::Echo(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Echo_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Echo_ctor1) {
  stk::Echo * result = (stk::Echo *)new stk::Echo();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Echo_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Echo_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Echo * arg1 = (stk::Echo *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Echo_setMaximumDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Echo * arg1 = (stk::Echo *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  (arg1)->setMaximumDelay(arg2);
}

static MFUN(gw_Echo_setDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Echo * arg1 = (stk::Echo *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned long arg2 = *(unsigned long*)(temp2->data);
  (arg1)->setDelay(arg2);
}

static DTOR(gw_Echo_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Echo*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

#define GW_WvIn(o) *(void**)(o->data)
static MFUN(gw_WvIn_channelsOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::WvIn * arg1 = *(stk::WvIn **)(temp1->data);
  unsigned int result = (unsigned int)((stk::WvIn const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static DTOR(gw_WvIn_dtor) {
  if(GW_WvIn(o)) delete (stk::WvIn*)GW_WvIn(o);
  GW_WvIn(o) = NULL;
}

static TICK(FileWvIn_tick) {
  stk::FileWvIn *s = (stk::FileWvIn*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_FileWvIn_ctor0) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  bool arg3 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  const M_Object temp4 = *(M_Object*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  unsigned long arg4 = *(unsigned long*)(temp4->data);
  const M_Object temp5 = *(M_Object*)MEM(0+SZ_INT+SZ_INT+SZ_INT+SZ_INT);
  unsigned long arg5 = *(unsigned long*)(temp5->data);
  bool arg6 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_INT+SZ_INT+SZ_INT);
  stk::FileWvIn * result = (stk::FileWvIn *)new stk::FileWvIn(arg1,arg2,arg3,arg4,arg5,arg6);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, result->channelsOut());
  ugen_gen(shred->info->vm->gwion, UGEN(o), FileWvIn_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_FileWvIn_ctor1) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  bool arg3 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  const M_Object temp4 = *(M_Object*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  unsigned long arg4 = *(unsigned long*)(temp4->data);
  const M_Object temp5 = *(M_Object*)MEM(0+SZ_INT+SZ_INT+SZ_INT+SZ_INT);
  unsigned long arg5 = *(unsigned long*)(temp5->data);
  stk::FileWvIn * result = (stk::FileWvIn *)new stk::FileWvIn(arg1,arg2,arg3,arg4,arg5);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, result->channelsOut());
  ugen_gen(shred->info->vm->gwion, UGEN(o), FileWvIn_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_FileWvIn_ctor2) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  bool arg3 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  const M_Object temp4 = *(M_Object*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  unsigned long arg4 = *(unsigned long*)(temp4->data);
  stk::FileWvIn * result = (stk::FileWvIn *)new stk::FileWvIn(arg1,arg2,arg3,arg4);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, result->channelsOut());
  ugen_gen(shred->info->vm->gwion, UGEN(o), FileWvIn_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_FileWvIn_ctor3) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  bool arg3 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  stk::FileWvIn * result = (stk::FileWvIn *)new stk::FileWvIn(arg1,arg2,arg3);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, result->channelsOut());
  ugen_gen(shred->info->vm->gwion, UGEN(o), FileWvIn_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_FileWvIn_ctor4) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  stk::FileWvIn * result = (stk::FileWvIn *)new stk::FileWvIn(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, result->channelsOut());
  ugen_gen(shred->info->vm->gwion, UGEN(o), FileWvIn_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_FileWvIn_ctor5) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  stk::FileWvIn * result = (stk::FileWvIn *)new stk::FileWvIn(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, result->channelsOut());
  ugen_gen(shred->info->vm->gwion, UGEN(o), FileWvIn_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_FileWvIn_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::FileWvIn*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_FileWvIn_reset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  (arg1)->reset();
}

static MFUN(gw_FileWvIn_normalize0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  (arg1)->normalize();
}

static MFUN(gw_FileWvIn_normalize1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->normalize(arg2);
}

static MFUN(gw_FileWvIn_getSize) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  unsigned long result = (unsigned long)((stk::FileWvIn const *)arg1)->getSize();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_FileWvIn_getFileRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)((stk::FileWvIn const *)arg1)->getFileRate();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_FileWvIn_isOpen) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  bool result = (bool)(arg1)->isOpen();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_FileWvIn_isFinished) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  bool result = (bool)((stk::FileWvIn const *)arg1)->isFinished();
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_FileWvIn_setRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRate(arg2);
}

static MFUN(gw_FileWvIn_addTime) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->addTime(arg2);
}

static MFUN(gw_FileWvIn_setInterpolate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FileWvIn * arg1 = (stk::FileWvIn *)UGEN(temp1)->module.gen.data;
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setInterpolate(arg2);
}

static TICK(Flute_tick) {
  stk::Flute *s = (stk::Flute*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Flute_ctor) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Flute * result = (stk::Flute *)new stk::Flute(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Flute_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Flute_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Flute*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Flute_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Flute_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Flute_setJetReflection) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setJetReflection(arg2);
}

static MFUN(gw_Flute_setEndReflection) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setEndReflection(arg2);
}

static MFUN(gw_Flute_setJetDelay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setJetDelay(arg2);
}

static MFUN(gw_Flute_startBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Flute_stopBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Flute_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Flute_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Flute_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Flute * arg1 = (stk::Flute *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(FMVoices_tick) {
  stk::FMVoices *s = (stk::FMVoices*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_FMVoices_ctor) {
  stk::FMVoices * result = (stk::FMVoices *)new stk::FMVoices();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), FMVoices_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_FMVoices_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::FMVoices*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_FMVoices_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FMVoices * arg1 = (stk::FMVoices *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_FMVoices_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FMVoices * arg1 = (stk::FMVoices *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_FMVoices_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FMVoices * arg1 = (stk::FMVoices *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(FormSwep_tick) {
  stk::FormSwep *s = (stk::FormSwep*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_FormSwep_ctor) {
  stk::FormSwep * result = (stk::FormSwep *)new stk::FormSwep();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), FormSwep_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_FormSwep_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::FormSwep*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_FormSwep_ignoreSampleRateChange0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_FormSwep_ignoreSampleRateChange1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_FormSwep_setResonance) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3);
}

static MFUN(gw_FormSwep_setStates0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setStates(arg2,arg3,arg4);
}

static MFUN(gw_FormSwep_setStates1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setStates(arg2,arg3);
}

static MFUN(gw_FormSwep_setTargets0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setTargets(arg2,arg3,arg4);
}

static MFUN(gw_FormSwep_setTargets1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setTargets(arg2,arg3);
}

static MFUN(gw_FormSwep_setSweepRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSweepRate(arg2);
}

static MFUN(gw_FormSwep_setSweepTime) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FormSwep * arg1 = (stk::FormSwep *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSweepTime(arg2);
}

static TICK(FreeVerb_tick) {
  stk::FreeVerb *s = (stk::FreeVerb*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_FreeVerb_ctor) {
  stk::FreeVerb * result = (stk::FreeVerb *)new stk::FreeVerb();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), FreeVerb_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_FreeVerb_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::FreeVerb*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_FreeVerb_setEffectMix) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setEffectMix(arg2);
}

static MFUN(gw_FreeVerb_setRoomSize) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRoomSize(arg2);
}

static MFUN(gw_FreeVerb_getRoomSize) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)(arg1)->getRoomSize();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_FreeVerb_setDamping) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDamping(arg2);
}

static MFUN(gw_FreeVerb_getDamping) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)(arg1)->getDamping();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_FreeVerb_setWidth) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setWidth(arg2);
}

static MFUN(gw_FreeVerb_getWidth) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)(arg1)->getWidth();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_FreeVerb_setMode) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setMode(arg2);
}

static MFUN(gw_FreeVerb_getMode) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)(arg1)->getMode();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_FreeVerb_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::FreeVerb * arg1 = (stk::FreeVerb *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static TICK(Granulate_tick) {
  stk::Granulate *s = (stk::Granulate*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Granulate_ctor0) {
  stk::Granulate * result = (stk::Granulate *)new stk::Granulate();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Granulate_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Granulate_ctor1) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  std::string arg2 = (std::string)STRING(temp2);
  bool arg3 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  stk::Granulate * result = (stk::Granulate *)new stk::Granulate(arg1,arg2,arg3);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Granulate_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Granulate_ctor2) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  std::string arg2 = (std::string)STRING(temp2);
  stk::Granulate * result = (stk::Granulate *)new stk::Granulate(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Granulate_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Granulate_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Granulate*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Granulate_openFile0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  std::string arg2 = (std::string)STRING(temp2);
  bool arg3 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->openFile(arg2,arg3);
}

static MFUN(gw_Granulate_openFile1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  std::string arg2 = (std::string)STRING(temp2);
  (arg1)->openFile(arg2);
}

static MFUN(gw_Granulate_reset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  (arg1)->reset();
}

static MFUN(gw_Granulate_setVoices0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setVoices(arg2);
}

static MFUN(gw_Granulate_setVoices1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  (arg1)->setVoices();
}

static MFUN(gw_Granulate_setStretch0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setStretch(arg2);
}

static MFUN(gw_Granulate_setStretch1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  (arg1)->setStretch();
}

static MFUN(gw_Granulate_setGrainParameters0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  unsigned int arg5 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_INT+SZ_INT);
  (arg1)->setGrainParameters(arg2,arg3,arg4,arg5);
}

static MFUN(gw_Granulate_setGrainParameters1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  (arg1)->setGrainParameters(arg2,arg3,arg4);
}

static MFUN(gw_Granulate_setGrainParameters2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setGrainParameters(arg2,arg3);
}

static MFUN(gw_Granulate_setGrainParameters3) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setGrainParameters(arg2);
}

static MFUN(gw_Granulate_setGrainParameters4) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  (arg1)->setGrainParameters();
}

static MFUN(gw_Granulate_setRandomFactor0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRandomFactor(arg2);
}

static MFUN(gw_Granulate_setRandomFactor1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Granulate * arg1 = (stk::Granulate *)UGEN(temp1)->module.gen.data;
  (arg1)->setRandomFactor();
}

static TICK(Guitar_tick) {
  stk::Guitar *s = (stk::Guitar*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Guitar_ctor0) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  std::string arg2 = (std::string)STRING(temp2);
  stk::Guitar * result = (stk::Guitar *)new stk::Guitar(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Guitar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Guitar_ctor1) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  stk::Guitar * result = (stk::Guitar *)new stk::Guitar(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Guitar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Guitar_ctor2) {
  stk::Guitar * result = (stk::Guitar *)new stk::Guitar();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Guitar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Guitar_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Guitar_setBodyFile0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  std::string arg2 = (std::string)STRING(temp2);
  (arg1)->setBodyFile(arg2);
}

static MFUN(gw_Guitar_setBodyFile1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  (arg1)->setBodyFile();
}

static MFUN(gw_Guitar_setPluckPosition0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setPluckPosition(arg2,arg3);
}

static MFUN(gw_Guitar_setPluckPosition1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPluckPosition(arg2);
}

static MFUN(gw_Guitar_setLoopGain0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setLoopGain(arg2,arg3);
}

static MFUN(gw_Guitar_setLoopGain1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLoopGain(arg2);
}

static MFUN(gw_Guitar_setFrequency0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setFrequency(arg2,arg3);
}

static MFUN(gw_Guitar_setFrequency1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Guitar_noteOn0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  unsigned int arg4 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3,arg4);
}

static MFUN(gw_Guitar_noteOn1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Guitar_noteOff0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOff(arg2,arg3);
}

static MFUN(gw_Guitar_noteOff1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Guitar_controlChange0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_FLOAT);
  (arg1)->controlChange(arg2,arg3,arg4);
}

static MFUN(gw_Guitar_controlChange1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Guitar * arg1 = (stk::Guitar *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static DTOR(gw_Guitar_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Guitar*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(HevyMetl_tick) {
  stk::HevyMetl *s = (stk::HevyMetl*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_HevyMetl_ctor) {
  stk::HevyMetl * result = (stk::HevyMetl *)new stk::HevyMetl();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), HevyMetl_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_HevyMetl_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::HevyMetl*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_HevyMetl_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::HevyMetl * arg1 = (stk::HevyMetl *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static TICK(JCRev_tick) {
  stk::JCRev *s = (stk::JCRev*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_JCRev_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::JCRev * result = (stk::JCRev *)new stk::JCRev(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), JCRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_JCRev_ctor1) {
  stk::JCRev * result = (stk::JCRev *)new stk::JCRev();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), JCRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_JCRev_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::JCRev * arg1 = (stk::JCRev *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_JCRev_setT60) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::JCRev * arg1 = (stk::JCRev *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setT60(arg2);
}

static DTOR(gw_JCRev_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::JCRev*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(JetTable_tick) {
  stk::JetTable *s = (stk::JetTable*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_JetTable_ctor) {
  stk::JetTable * result = (stk::JetTable *)new stk::JetTable();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), JetTable_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_JetTable_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::JetTable*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(LentPitShift_tick) {
  stk::LentPitShift *s = (stk::LentPitShift*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_LentPitShift_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  int arg2 = (int)*(m_int*)MEM(0+SZ_FLOAT);
  stk::LentPitShift * result = (stk::LentPitShift *)new stk::LentPitShift(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), LentPitShift_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_LentPitShift_ctor1) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::LentPitShift * result = (stk::LentPitShift *)new stk::LentPitShift(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), LentPitShift_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_LentPitShift_ctor2) {
  stk::LentPitShift * result = (stk::LentPitShift *)new stk::LentPitShift();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), LentPitShift_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_LentPitShift_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::LentPitShift*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_LentPitShift_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::LentPitShift * arg1 = (stk::LentPitShift *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_LentPitShift_setShift) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::LentPitShift * arg1 = (stk::LentPitShift *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setShift(arg2);
}

static TICK(Mandolin_tick) {
  stk::Mandolin *s = (stk::Mandolin*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Mandolin_ctor) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Mandolin * result = (stk::Mandolin *)new stk::Mandolin(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Mandolin_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Mandolin_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Mandolin*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Mandolin_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Mandolin_setDetune) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDetune(arg2);
}

static MFUN(gw_Mandolin_setBodySize) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBodySize(arg2);
}

static MFUN(gw_Mandolin_setPluckPosition) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPluckPosition(arg2);
}

static MFUN(gw_Mandolin_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Mandolin_pluck0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_Mandolin_pluck1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->pluck(arg2,arg3);
}

static MFUN(gw_Mandolin_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Mandolin_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Mandolin_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mandolin * arg1 = (stk::Mandolin *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Mesh2D_tick) {
  stk::Mesh2D *s = (stk::Mesh2D*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_NXMAX_get) {
  unsigned short const result = (unsigned short)(unsigned short)stk::NXMAX;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_NYMAX_get) {
  unsigned short const result = (unsigned short)(unsigned short)stk::NYMAX;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Mesh2D_ctor) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  unsigned short arg1 = *(unsigned short*)(temp1->data);
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned short arg2 = *(unsigned short*)(temp2->data);
  stk::Mesh2D * result = (stk::Mesh2D *)new stk::Mesh2D(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Mesh2D_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Mesh2D_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Mesh2D*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Mesh2D_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Mesh2D_setNX) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned short arg2 = *(unsigned short*)(temp2->data);
  (arg1)->setNX(arg2);
}

static MFUN(gw_Mesh2D_setNY) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  unsigned short arg2 = *(unsigned short*)(temp2->data);
  (arg1)->setNY(arg2);
}

static MFUN(gw_Mesh2D_setInputPosition) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setInputPosition(arg2,arg3);
}

static MFUN(gw_Mesh2D_setDecay) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDecay(arg2);
}

static MFUN(gw_Mesh2D_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Mesh2D_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Mesh2D_energy) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  stk::StkFloat result = (stk::StkFloat)(arg1)->energy();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Mesh2D_inputTick) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)(arg1)->inputTick(arg2);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Mesh2D_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Mesh2D * arg1 = (stk::Mesh2D *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Modal_tick) {
  stk::Modal *s = (stk::Modal*)u->module.gen.data;
  u->out = s->tick(1);
}

static DTOR(gw_Modal_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Modal*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Modal_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Modal_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Modal_setRatioAndRadius) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT+SZ_FLOAT);
  (arg1)->setRatioAndRadius(arg2,arg3,arg4);
}

static MFUN(gw_Modal_setMasterGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setMasterGain(arg2);
}

static MFUN(gw_Modal_setDirectGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setDirectGain(arg2);
}

static MFUN(gw_Modal_setModeGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setModeGain(arg2,arg3);
}

static MFUN(gw_Modal_strike) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->strike(arg2);
}

static MFUN(gw_Modal_damp) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->damp(arg2);
}

static MFUN(gw_Modal_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Modal_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Modal_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modal * arg1 = (stk::Modal *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(ModalBar_tick) {
  stk::ModalBar *s = (stk::ModalBar*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_ModalBar_ctor) {
  stk::ModalBar * result = (stk::ModalBar *)new stk::ModalBar();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), ModalBar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_ModalBar_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::ModalBar*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_ModalBar_setStickHardness) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ModalBar * arg1 = (stk::ModalBar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setStickHardness(arg2);
}

static MFUN(gw_ModalBar_setStrikePosition) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ModalBar * arg1 = (stk::ModalBar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setStrikePosition(arg2);
}

static MFUN(gw_ModalBar_setPreset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ModalBar * arg1 = (stk::ModalBar *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setPreset(arg2);
}

static MFUN(gw_ModalBar_setModulationDepth) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ModalBar * arg1 = (stk::ModalBar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationDepth(arg2);
}

static MFUN(gw_ModalBar_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ModalBar * arg1 = (stk::ModalBar *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Modulate_tick) {
  stk::Modulate *s = (stk::Modulate*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_Modulate_ctor) {
  stk::Modulate * result = (stk::Modulate *)new stk::Modulate();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Modulate_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Modulate_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Modulate*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Modulate_reset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modulate * arg1 = (stk::Modulate *)UGEN(temp1)->module.gen.data;
  (arg1)->reset();
}

static MFUN(gw_Modulate_setVibratoRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modulate * arg1 = (stk::Modulate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoRate(arg2);
}

static MFUN(gw_Modulate_setVibratoGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modulate * arg1 = (stk::Modulate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoGain(arg2);
}

static MFUN(gw_Modulate_setRandomRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modulate * arg1 = (stk::Modulate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRandomRate(arg2);
}

static MFUN(gw_Modulate_setRandomGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Modulate * arg1 = (stk::Modulate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRandomGain(arg2);
}

static TICK(Moog_tick) {
  stk::Moog *s = (stk::Moog*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Moog_ctor) {
  stk::Moog * result = (stk::Moog *)new stk::Moog();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Moog_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Moog_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Moog*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Moog_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Moog * arg1 = (stk::Moog *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Moog_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Moog * arg1 = (stk::Moog *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Moog_setModulationSpeed) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Moog * arg1 = (stk::Moog *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationSpeed(arg2);
}

static MFUN(gw_Moog_setModulationDepth) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Moog * arg1 = (stk::Moog *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setModulationDepth(arg2);
}

static MFUN(gw_Moog_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Moog * arg1 = (stk::Moog *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Noise_tick) {
  stk::Noise *s = (stk::Noise*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_Noise_ctor0) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  stk::Noise * result = (stk::Noise *)new stk::Noise(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Noise_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Noise_ctor1) {
  stk::Noise * result = (stk::Noise *)new stk::Noise();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Noise_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Noise_setSeed0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Noise * arg1 = (stk::Noise *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  (arg1)->setSeed(arg2);
}

static MFUN(gw_Noise_setSeed1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Noise * arg1 = (stk::Noise *)UGEN(temp1)->module.gen.data;
  (arg1)->setSeed();
}

static DTOR(gw_Noise_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Noise*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(NRev_tick) {
  stk::NRev *s = (stk::NRev*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_NRev_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::NRev * result = (stk::NRev *)new stk::NRev(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), NRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_NRev_ctor1) {
  stk::NRev * result = (stk::NRev *)new stk::NRev();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), NRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_NRev_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::NRev * arg1 = (stk::NRev *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_NRev_setT60) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::NRev * arg1 = (stk::NRev *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setT60(arg2);
}

static DTOR(gw_NRev_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::NRev*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(OnePole_tick) {
  stk::OnePole *s = (stk::OnePole*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_OnePole_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::OnePole * result = (stk::OnePole *)new stk::OnePole(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), OnePole_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_OnePole_ctor1) {
  stk::OnePole * result = (stk::OnePole *)new stk::OnePole();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), OnePole_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_OnePole_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::OnePole*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_OnePole_setB0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OnePole * arg1 = (stk::OnePole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_OnePole_setA1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OnePole * arg1 = (stk::OnePole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_OnePole_setCoefficients0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OnePole * arg1 = (stk::OnePole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  bool arg4 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_OnePole_setCoefficients1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OnePole * arg1 = (stk::OnePole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3);
}

static MFUN(gw_OnePole_setPole) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OnePole * arg1 = (stk::OnePole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPole(arg2);
}

static TICK(OneZero_tick) {
  stk::OneZero *s = (stk::OneZero*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_OneZero_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::OneZero * result = (stk::OneZero *)new stk::OneZero(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), OneZero_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_OneZero_ctor1) {
  stk::OneZero * result = (stk::OneZero *)new stk::OneZero();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), OneZero_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_OneZero_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::OneZero*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_OneZero_setB0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OneZero * arg1 = (stk::OneZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_OneZero_setB1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OneZero * arg1 = (stk::OneZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB1(arg2);
}

static MFUN(gw_OneZero_setCoefficients0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OneZero * arg1 = (stk::OneZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  bool arg4 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_OneZero_setCoefficients1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OneZero * arg1 = (stk::OneZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3);
}

static MFUN(gw_OneZero_setZero) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::OneZero * arg1 = (stk::OneZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setZero(arg2);
}

static TICK(PercFlut_tick) {
  stk::PercFlut *s = (stk::PercFlut*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_PercFlut_ctor) {
  stk::PercFlut * result = (stk::PercFlut *)new stk::PercFlut();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PercFlut_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_PercFlut_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::PercFlut*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_PercFlut_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PercFlut * arg1 = (stk::PercFlut *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_PercFlut_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PercFlut * arg1 = (stk::PercFlut *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

#define GW_Phonemes(o) *(void**)(o->data)
static MFUN(gw_Phonemes_ctor) {
  stk::Phonemes * result = (stk::Phonemes *)new stk::Phonemes();
  // VM_Code code = *(VM_Code*)REG(0);
  // M_Object o = new_object(shred->info->mp, code->ret_type);
  *(stk::Phonemes **)o->data = result; // TODO: handle offset
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Phonemes_dtor) {
  if(GW_Phonemes(o)) delete (stk::Phonemes*)GW_Phonemes(o);
  GW_Phonemes(o) = NULL;
}

static MFUN(gw_Phonemes_name) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  char const * result = (char *)stk::Phonemes::name(arg1);
  *(M_Object*)RETURN = result ? new_string(shred->info->vm->gwion, (m_str)result) : NULL;
}

static MFUN(gw_Phonemes_voiceGain) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::voiceGain(arg1);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Phonemes_noiseGain) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::noiseGain(arg1);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Phonemes_formantFrequency) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::formantFrequency(arg1,arg2);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Phonemes_formantRadius) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::formantRadius(arg1,arg2);
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Phonemes_formantGain) {
  unsigned int arg1 = (unsigned int)*(m_int*)MEM(0);
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat result = (stk::StkFloat)stk::Phonemes::formantGain(arg1,arg2);
  *(m_float*)RETURN = (m_float)(double)result;
}

static TICK(PitShift_tick) {
  stk::PitShift *s = (stk::PitShift*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_maxDelay_get) {
  int const result = (int)(int)stk::maxDelay;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_PitShift_ctor) {
  stk::PitShift * result = (stk::PitShift *)new stk::PitShift();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PitShift_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_PitShift_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PitShift * arg1 = (stk::PitShift *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_PitShift_setShift) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PitShift * arg1 = (stk::PitShift *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setShift(arg2);
}

static DTOR(gw_PitShift_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::PitShift*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(Plucked_tick) {
  stk::Plucked *s = (stk::Plucked*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Plucked_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Plucked * result = (stk::Plucked *)new stk::Plucked(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Plucked_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Plucked_ctor1) {
  stk::Plucked * result = (stk::Plucked *)new stk::Plucked();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Plucked_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Plucked_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Plucked*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Plucked_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Plucked * arg1 = (stk::Plucked *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Plucked_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Plucked * arg1 = (stk::Plucked *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Plucked_pluck) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Plucked * arg1 = (stk::Plucked *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_Plucked_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Plucked * arg1 = (stk::Plucked *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Plucked_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Plucked * arg1 = (stk::Plucked *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static TICK(PoleZero_tick) {
  stk::PoleZero *s = (stk::PoleZero*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_PoleZero_ctor) {
  stk::PoleZero * result = (stk::PoleZero *)new stk::PoleZero();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PoleZero_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_PoleZero_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::PoleZero*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_PoleZero_setB0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PoleZero * arg1 = (stk::PoleZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_PoleZero_setB1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PoleZero * arg1 = (stk::PoleZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB1(arg2);
}

static MFUN(gw_PoleZero_setA1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PoleZero * arg1 = (stk::PoleZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_PoleZero_setCoefficients0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PoleZero * arg1 = (stk::PoleZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  bool arg5 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5);
}

static MFUN(gw_PoleZero_setCoefficients1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PoleZero * arg1 = (stk::PoleZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_PoleZero_setAllpass) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PoleZero * arg1 = (stk::PoleZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setAllpass(arg2);
}

static MFUN(gw_PoleZero_setBlockZero0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PoleZero * arg1 = (stk::PoleZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBlockZero(arg2);
}

static MFUN(gw_PoleZero_setBlockZero1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PoleZero * arg1 = (stk::PoleZero *)UGEN(temp1)->module.gen.data;
  (arg1)->setBlockZero();
}

static TICK(PRCRev_tick) {
  stk::PRCRev *s = (stk::PRCRev*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_PRCRev_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::PRCRev * result = (stk::PRCRev *)new stk::PRCRev(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PRCRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_PRCRev_ctor1) {
  stk::PRCRev * result = (stk::PRCRev *)new stk::PRCRev();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), PRCRev_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_PRCRev_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PRCRev * arg1 = (stk::PRCRev *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_PRCRev_setT60) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::PRCRev * arg1 = (stk::PRCRev *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setT60(arg2);
}

static DTOR(gw_PRCRev_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::PRCRev*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(Recorder_tick) {
  stk::Recorder *s = (stk::Recorder*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Recorder_ctor) {
  stk::Recorder * result = (stk::Recorder *)new stk::Recorder();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Recorder_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Recorder_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Recorder*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Recorder_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Recorder_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Recorder_startBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Recorder_stopBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Recorder_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Recorder_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Recorder_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static MFUN(gw_Recorder_setBlowPressure) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBlowPressure(arg2);
}

static MFUN(gw_Recorder_setVibratoGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoGain(arg2);
}

static MFUN(gw_Recorder_setVibratoFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoFrequency(arg2);
}

static MFUN(gw_Recorder_setNoiseGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setNoiseGain(arg2);
}

static MFUN(gw_Recorder_setBreathCutoff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBreathCutoff(arg2);
}

static MFUN(gw_Recorder_setSoftness) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Recorder * arg1 = (stk::Recorder *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSoftness(arg2);
}

static TICK(ReedTable_tick) {
  stk::ReedTable *s = (stk::ReedTable*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_ReedTable_ctor) {
  stk::ReedTable * result = (stk::ReedTable *)new stk::ReedTable();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), ReedTable_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_ReedTable_setOffset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ReedTable * arg1 = (stk::ReedTable *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setOffset(arg2);
}

static MFUN(gw_ReedTable_setSlope) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::ReedTable * arg1 = (stk::ReedTable *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSlope(arg2);
}

static DTOR(gw_ReedTable_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::ReedTable*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(Resonate_tick) {
  stk::Resonate *s = (stk::Resonate*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Resonate_ctor) {
  stk::Resonate * result = (stk::Resonate *)new stk::Resonate();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Resonate_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Resonate_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Resonate*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Resonate_setResonance) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Resonate * arg1 = (stk::Resonate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3);
}

static MFUN(gw_Resonate_setNotch) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Resonate * arg1 = (stk::Resonate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setNotch(arg2,arg3);
}

static MFUN(gw_Resonate_setEqualGainZeroes) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Resonate * arg1 = (stk::Resonate *)UGEN(temp1)->module.gen.data;
  (arg1)->setEqualGainZeroes();
}

static MFUN(gw_Resonate_keyOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Resonate * arg1 = (stk::Resonate *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOn();
}

static MFUN(gw_Resonate_keyOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Resonate * arg1 = (stk::Resonate *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOff();
}

static MFUN(gw_Resonate_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Resonate * arg1 = (stk::Resonate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Resonate_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Resonate * arg1 = (stk::Resonate *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Resonate_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Resonate * arg1 = (stk::Resonate *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Rhodey_tick) {
  stk::Rhodey *s = (stk::Rhodey*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Rhodey_ctor) {
  stk::Rhodey * result = (stk::Rhodey *)new stk::Rhodey();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Rhodey_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Rhodey_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Rhodey*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Rhodey_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Rhodey * arg1 = (stk::Rhodey *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Rhodey_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Rhodey * arg1 = (stk::Rhodey *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static TICK(Saxofony_tick) {
  stk::Saxofony *s = (stk::Saxofony*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Saxofony_ctor) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Saxofony * result = (stk::Saxofony *)new stk::Saxofony(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Saxofony_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Saxofony_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Saxofony*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Saxofony_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Saxofony * arg1 = (stk::Saxofony *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Saxofony_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Saxofony * arg1 = (stk::Saxofony *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Saxofony_setBlowPosition) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Saxofony * arg1 = (stk::Saxofony *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBlowPosition(arg2);
}

static MFUN(gw_Saxofony_startBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Saxofony * arg1 = (stk::Saxofony *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Saxofony_stopBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Saxofony * arg1 = (stk::Saxofony *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Saxofony_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Saxofony * arg1 = (stk::Saxofony *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Saxofony_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Saxofony * arg1 = (stk::Saxofony *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Saxofony_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Saxofony * arg1 = (stk::Saxofony *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Shakers_tick) {
  stk::Shakers *s = (stk::Shakers*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Shakers_ctor0) {
  int arg1 = (int)*(m_int*)MEM(0);
  stk::Shakers * result = (stk::Shakers *)new stk::Shakers(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Shakers_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Shakers_ctor1) {
  stk::Shakers * result = (stk::Shakers *)new stk::Shakers();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Shakers_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Shakers_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Shakers * arg1 = (stk::Shakers *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Shakers_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Shakers * arg1 = (stk::Shakers *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Shakers_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Shakers * arg1 = (stk::Shakers *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static DTOR(gw_Shakers_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Shakers*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_MIN_ENERGY_get) {
  stk::StkFloat const result = (stk::StkFloat)(stk::StkFloat)stk::MIN_ENERGY;
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_WATER_FREQ_SWEEP_get) {
  stk::StkFloat const result = (stk::StkFloat)(stk::StkFloat)stk::WATER_FREQ_SWEEP;
  *(m_float*)RETURN = (m_float)(double)result;
}

static TICK(Simple_tick) {
  stk::Simple *s = (stk::Simple*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_Simple_ctor) {
  stk::Simple * result = (stk::Simple *)new stk::Simple();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Simple_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Simple_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Simple*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Simple_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Simple * arg1 = (stk::Simple *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Simple_keyOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Simple * arg1 = (stk::Simple *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOn();
}

static MFUN(gw_Simple_keyOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Simple * arg1 = (stk::Simple *)UGEN(temp1)->module.gen.data;
  (arg1)->keyOff();
}

static MFUN(gw_Simple_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Simple * arg1 = (stk::Simple *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Simple_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Simple * arg1 = (stk::Simple *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Simple_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Simple * arg1 = (stk::Simple *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(SineWave_tick) {
  stk::SineWave *s = (stk::SineWave*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_TABLE_SIZE_get) {
  unsigned long const result = (unsigned long)(unsigned long)TABLE_SIZE;
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_SineWave_ctor) {
  stk::SineWave * result = (stk::SineWave *)new stk::SineWave();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), SineWave_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_SineWave_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::SineWave*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_SineWave_reset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SineWave * arg1 = (stk::SineWave *)UGEN(temp1)->module.gen.data;
  (arg1)->reset();
}

static MFUN(gw_SineWave_setRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SineWave * arg1 = (stk::SineWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRate(arg2);
}

static MFUN(gw_SineWave_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SineWave * arg1 = (stk::SineWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_SineWave_addTime) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SineWave * arg1 = (stk::SineWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->addTime(arg2);
}

static MFUN(gw_SineWave_addPhase) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SineWave * arg1 = (stk::SineWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->addPhase(arg2);
}

static MFUN(gw_SineWave_addPhaseOffset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SineWave * arg1 = (stk::SineWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->addPhaseOffset(arg2);
}

static TICK(SingWave_tick) {
  stk::SingWave *s = (stk::SingWave*)u->module.gen.data;
  u->out = s->tick();
}

static MFUN(gw_SingWave_ctor0) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  stk::SingWave * result = (stk::SingWave *)new stk::SingWave(arg1,arg2);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), SingWave_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_SingWave_ctor1) {
  M_Object temp1 = *(M_Object*)MEM(0);
  std::string arg1 = (std::string)STRING(temp1);
  stk::SingWave * result = (stk::SingWave *)new stk::SingWave(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), SingWave_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_SingWave_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::SingWave*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_SingWave_reset) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  (arg1)->reset();
}

static MFUN(gw_SingWave_normalize0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  (arg1)->normalize();
}

static MFUN(gw_SingWave_normalize1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->normalize(arg2);
}

static MFUN(gw_SingWave_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_SingWave_setVibratoRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoRate(arg2);
}

static MFUN(gw_SingWave_setVibratoGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVibratoGain(arg2);
}

static MFUN(gw_SingWave_setRandomGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setRandomGain(arg2);
}

static MFUN(gw_SingWave_setSweepRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setSweepRate(arg2);
}

static MFUN(gw_SingWave_setGainRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setGainRate(arg2);
}

static MFUN(gw_SingWave_setGainTarget) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setGainTarget(arg2);
}

static MFUN(gw_SingWave_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  (arg1)->noteOn();
}

static MFUN(gw_SingWave_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::SingWave * arg1 = (stk::SingWave *)UGEN(temp1)->module.gen.data;
  (arg1)->noteOff();
}

static TICK(Sitar_tick) {
  stk::Sitar *s = (stk::Sitar*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Sitar_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Sitar * result = (stk::Sitar *)new stk::Sitar(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Sitar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Sitar_ctor1) {
  stk::Sitar * result = (stk::Sitar *)new stk::Sitar();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Sitar_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Sitar_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Sitar*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Sitar_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sitar * arg1 = (stk::Sitar *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Sitar_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sitar * arg1 = (stk::Sitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Sitar_pluck) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sitar * arg1 = (stk::Sitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_Sitar_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sitar * arg1 = (stk::Sitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Sitar_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Sitar * arg1 = (stk::Sitar *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

#define GW_Vector3D(o) *(void**)(o->data)
static MFUN(gw_Vector3D_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_FLOAT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_FLOAT+SZ_FLOAT);
  stk::Vector3D * result = (stk::Vector3D *)new stk::Vector3D(arg1,arg2,arg3);
  // VM_Code code = *(VM_Code*)REG(0+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  // M_Object o = new_object(shred->info->mp, code->ret_type);
  *(stk::Vector3D **)o->data = result; // TODO: handle offset
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Vector3D_ctor1) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_FLOAT);
  stk::Vector3D * result = (stk::Vector3D *)new stk::Vector3D(arg1,arg2);
  // VM_Code code = *(VM_Code*)REG(0+SZ_FLOAT+SZ_FLOAT);
  // M_Object o = new_object(shred->info->mp, code->ret_type);
  *(stk::Vector3D **)o->data = result; // TODO: handle offset
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Vector3D_ctor2) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Vector3D * result = (stk::Vector3D *)new stk::Vector3D(arg1);
  // VM_Code code = *(VM_Code*)REG(0+SZ_FLOAT);
  // M_Object o = new_object(shred->info->mp, code->ret_type);
  *(stk::Vector3D **)o->data = result; // TODO: handle offset
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Vector3D_ctor3) {
  stk::Vector3D * result = (stk::Vector3D *)new stk::Vector3D();
  // VM_Code code = *(VM_Code*)REG(0);
  // M_Object o = new_object(shred->info->mp, code->ret_type);
  *(stk::Vector3D **)o->data = result; // TODO: handle offset
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Vector3D_getX) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Vector3D * arg1 = *(stk::Vector3D **)(temp1->data);
  stk::StkFloat result = (stk::StkFloat)(arg1)->getX();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Vector3D_getY) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Vector3D * arg1 = *(stk::Vector3D **)(temp1->data);
  stk::StkFloat result = (stk::StkFloat)(arg1)->getY();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Vector3D_getZ) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Vector3D * arg1 = *(stk::Vector3D **)(temp1->data);
  stk::StkFloat result = (stk::StkFloat)(arg1)->getZ();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Vector3D_getLength) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Vector3D * arg1 = *(stk::Vector3D **)(temp1->data);
  stk::StkFloat result = (stk::StkFloat)(arg1)->getLength();
  *(m_float*)RETURN = (m_float)(double)result;
}

static MFUN(gw_Vector3D_setXYZ) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Vector3D * arg1 = *(stk::Vector3D **)(temp1->data);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setXYZ(arg2,arg3,arg4);
}

static MFUN(gw_Vector3D_setX) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Vector3D * arg1 = *(stk::Vector3D **)(temp1->data);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setX(arg2);
}

static MFUN(gw_Vector3D_setY) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Vector3D * arg1 = *(stk::Vector3D **)(temp1->data);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setY(arg2);
}

static MFUN(gw_Vector3D_setZ) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Vector3D * arg1 = *(stk::Vector3D **)(temp1->data);
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setZ(arg2);
}

static DTOR(gw_Vector3D_dtor) {
  if(GW_Vector3D(o)) delete (stk::Vector3D*)GW_Vector3D(o);
  GW_Vector3D(o) = NULL;
}

static TICK(StifKarp_tick) {
  stk::StifKarp *s = (stk::StifKarp*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_StifKarp_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::StifKarp * result = (stk::StifKarp *)new stk::StifKarp(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), StifKarp_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_StifKarp_ctor1) {
  stk::StifKarp * result = (stk::StifKarp *)new stk::StifKarp();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), StifKarp_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_StifKarp_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::StifKarp*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_StifKarp_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_StifKarp_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_StifKarp_setStretch) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setStretch(arg2);
}

static MFUN(gw_StifKarp_setPickupPosition) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPickupPosition(arg2);
}

static MFUN(gw_StifKarp_setBaseLoopGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setBaseLoopGain(arg2);
}

static MFUN(gw_StifKarp_pluck) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pluck(arg2);
}

static MFUN(gw_StifKarp_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_StifKarp_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_StifKarp_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::StifKarp * arg1 = (stk::StifKarp *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(TubeBell_tick) {
  stk::TubeBell *s = (stk::TubeBell*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_TubeBell_ctor) {
  stk::TubeBell * result = (stk::TubeBell *)new stk::TubeBell();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), TubeBell_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_TubeBell_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::TubeBell*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_TubeBell_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TubeBell * arg1 = (stk::TubeBell *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static TICK(Twang_tick) {
  stk::Twang *s = (stk::Twang*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Twang_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Twang * result = (stk::Twang *)new stk::Twang(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Twang_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Twang_ctor1) {
  stk::Twang * result = (stk::Twang *)new stk::Twang();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Twang_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Twang_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Twang * arg1 = (stk::Twang *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Twang_setLowestFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Twang * arg1 = (stk::Twang *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLowestFrequency(arg2);
}

static MFUN(gw_Twang_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Twang * arg1 = (stk::Twang *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Twang_setPluckPosition) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Twang * arg1 = (stk::Twang *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPluckPosition(arg2);
}

static MFUN(gw_Twang_setLoopGain) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Twang * arg1 = (stk::Twang *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setLoopGain(arg2);
}

static MFUN(gw_Twang_setLoopFilter) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Twang * arg1 = (stk::Twang *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  std::vector< stk::StkFloat > arg2 = *(std::vector< stk::StkFloat >*)(temp2->data);
  (arg1)->setLoopFilter(arg2);
}

static DTOR(gw_Twang_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Twang*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(TwoPole_tick) {
  stk::TwoPole *s = (stk::TwoPole*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_TwoPole_ctor) {
  stk::TwoPole * result = (stk::TwoPole *)new stk::TwoPole();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), TwoPole_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_TwoPole_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::TwoPole*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_TwoPole_ignoreSampleRateChange0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_TwoPole_ignoreSampleRateChange1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_TwoPole_setB0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_TwoPole_setA1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA1(arg2);
}

static MFUN(gw_TwoPole_setA2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setA2(arg2);
}

static MFUN(gw_TwoPole_setCoefficients0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  bool arg5 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5);
}

static MFUN(gw_TwoPole_setCoefficients1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_TwoPole_setResonance0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  bool arg4 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3,arg4);
}

static MFUN(gw_TwoPole_setResonance1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoPole * arg1 = (stk::TwoPole *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setResonance(arg2,arg3);
}

static TICK(TwoZero_tick) {
  stk::TwoZero *s = (stk::TwoZero*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_TwoZero_ctor) {
  stk::TwoZero * result = (stk::TwoZero *)new stk::TwoZero();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), TwoZero_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_TwoZero_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::TwoZero*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_TwoZero_ignoreSampleRateChange0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoZero * arg1 = (stk::TwoZero *)UGEN(temp1)->module.gen.data;
  bool arg2 = (bool)*(m_int*)MEM(0+SZ_INT);
  (arg1)->ignoreSampleRateChange(arg2);
}

static MFUN(gw_TwoZero_ignoreSampleRateChange1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoZero * arg1 = (stk::TwoZero *)UGEN(temp1)->module.gen.data;
  (arg1)->ignoreSampleRateChange();
}

static MFUN(gw_TwoZero_setB0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoZero * arg1 = (stk::TwoZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB0(arg2);
}

static MFUN(gw_TwoZero_setB1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoZero * arg1 = (stk::TwoZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB1(arg2);
}

static MFUN(gw_TwoZero_setB2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoZero * arg1 = (stk::TwoZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setB2(arg2);
}

static MFUN(gw_TwoZero_setCoefficients0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoZero * arg1 = (stk::TwoZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  bool arg5 = (bool)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4,arg5);
}

static MFUN(gw_TwoZero_setCoefficients1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoZero * arg1 = (stk::TwoZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->setCoefficients(arg2,arg3,arg4);
}

static MFUN(gw_TwoZero_setNotch) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::TwoZero * arg1 = (stk::TwoZero *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setNotch(arg2,arg3);
}

static TICK(Voicer_tick) {
  stk::Voicer *s = (stk::Voicer*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Voicer_ctor0) {
  stk::StkFloat arg1 = (stk::StkFloat)*(m_float*)MEM(0);
  stk::Voicer * result = (stk::Voicer *)new stk::Voicer(arg1);
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Voicer_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Voicer_ctor1) {
  stk::Voicer * result = (stk::Voicer *)new stk::Voicer();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Voicer_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static MFUN(gw_Voicer_addInstrument0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  stk::Instrmnt * arg2 = (stk::Instrmnt *)UGEN(temp2)->module.gen.data;
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->addInstrument(arg2,arg3);
}

static MFUN(gw_Voicer_addInstrument1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  stk::Instrmnt * arg2 = (stk::Instrmnt *)UGEN(temp2)->module.gen.data;
  (arg1)->addInstrument(arg2);
}

static MFUN(gw_Voicer_removeInstrument) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  const M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  stk::Instrmnt * arg2 = (stk::Instrmnt *)UGEN(temp2)->module.gen.data;
  (arg1)->removeInstrument(arg2);
}

static MFUN(gw_Voicer_noteOn0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  long result = (long)(arg1)->noteOn(arg2,arg3,arg4);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Voicer_noteOn1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  long result = (long)(arg1)->noteOn(arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_Voicer_noteOff0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT+SZ_FLOAT);
  (arg1)->noteOff(arg2,arg3,arg4);
}

static MFUN(gw_Voicer_noteOff1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOff(arg2,arg3);
}

static MFUN(gw_Voicer_noteOff2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  long arg2 = (long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->noteOff(arg2,arg3);
}

static MFUN(gw_Voicer_setFrequency0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->setFrequency(arg2,arg3);
}

static MFUN(gw_Voicer_setFrequency1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Voicer_setFrequency2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  long arg2 = (long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setFrequency(arg2,arg3);
}

static MFUN(gw_Voicer_pitchBend0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->pitchBend(arg2,arg3);
}

static MFUN(gw_Voicer_pitchBend1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->pitchBend(arg2);
}

static MFUN(gw_Voicer_pitchBend2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  long arg2 = (long)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->pitchBend(arg2,arg3);
}

static MFUN(gw_Voicer_controlChange0) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  int arg4 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT+SZ_FLOAT);
  (arg1)->controlChange(arg2,arg3,arg4);
}

static MFUN(gw_Voicer_controlChange1) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static MFUN(gw_Voicer_controlChange2) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  long arg2 = (long)*(m_int*)MEM(0+SZ_INT);
  int arg3 = (int)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  stk::StkFloat arg4 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3,arg4);
}

static MFUN(gw_Voicer_silence) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  (arg1)->silence();
}

static MFUN(gw_Voicer_channelsOut) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Voicer * arg1 = (stk::Voicer *)UGEN(temp1)->module.gen.data;
  unsigned int result = (unsigned int)((stk::Voicer const *)arg1)->channelsOut();
  *(m_int*)RETURN = (m_int)result;
}

static DTOR(gw_Voicer_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Voicer*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static TICK(VoicForm_tick) {
  stk::VoicForm *s = (stk::VoicForm*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_VoicForm_ctor) {
  stk::VoicForm * result = (stk::VoicForm *)new stk::VoicForm();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), VoicForm_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_VoicForm_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::VoicForm*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_VoicForm_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_VoicForm_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_VoicForm_setPhoneme) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  M_Object temp2 = *(M_Object*)MEM(0+SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  bool result = (bool)(arg1)->setPhoneme((char const *)arg2);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_VoicForm_setVoiced) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setVoiced(arg2);
}

static MFUN(gw_VoicForm_setUnVoiced) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setUnVoiced(arg2);
}

static MFUN(gw_VoicForm_setFilterSweepRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->setFilterSweepRate(arg2,arg3);
}

static MFUN(gw_VoicForm_setPitchSweepRate) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setPitchSweepRate(arg2);
}

static MFUN(gw_VoicForm_speak) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  (arg1)->speak();
}

static MFUN(gw_VoicForm_quiet) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  (arg1)->quiet();
}

static MFUN(gw_VoicForm_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_VoicForm_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_VoicForm_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::VoicForm * arg1 = (stk::VoicForm *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Whistle_tick) {
  stk::Whistle *s = (stk::Whistle*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Whistle_ctor) {
  stk::Whistle * result = (stk::Whistle *)new stk::Whistle();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Whistle_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Whistle_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Whistle*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Whistle_clear) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Whistle * arg1 = (stk::Whistle *)UGEN(temp1)->module.gen.data;
  (arg1)->clear();
}

static MFUN(gw_Whistle_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Whistle * arg1 = (stk::Whistle *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Whistle_startBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Whistle * arg1 = (stk::Whistle *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->startBlowing(arg2,arg3);
}

static MFUN(gw_Whistle_stopBlowing) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Whistle * arg1 = (stk::Whistle *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->stopBlowing(arg2);
}

static MFUN(gw_Whistle_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Whistle * arg1 = (stk::Whistle *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

static MFUN(gw_Whistle_noteOff) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Whistle * arg1 = (stk::Whistle *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->noteOff(arg2);
}

static MFUN(gw_Whistle_controlChange) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Whistle * arg1 = (stk::Whistle *)UGEN(temp1)->module.gen.data;
  int arg2 = (int)*(m_int*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_INT);
  (arg1)->controlChange(arg2,arg3);
}

static TICK(Wurley_tick) {
  stk::Wurley *s = (stk::Wurley*)u->module.gen.data;
  u->out = s->tick(1);
}

static MFUN(gw_Wurley_ctor) {
  stk::Wurley * result = (stk::Wurley *)new stk::Wurley();
  UGEN(o) = new_UGen(shred->info->mp);
  ugen_ini(shred->info->vm->gwion, UGEN(o), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), Wurley_tick, result, 0);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(o));
  *(M_Object*)RETURN = o;
}

static DTOR(gw_Wurley_dtor) {
  if(UGEN(o)->module.gen.data) delete (stk::Wurley*)UGEN(o)->module.gen.data;
  UGEN(o)->module.gen.data = NULL;
}

static MFUN(gw_Wurley_setFrequency) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Wurley * arg1 = (stk::Wurley *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  (arg1)->setFrequency(arg2);
}

static MFUN(gw_Wurley_noteOn) {
  const M_Object temp1 = *(M_Object*)MEM(0);
  stk::Wurley * arg1 = (stk::Wurley *)UGEN(temp1)->module.gen.data;
  stk::StkFloat arg2 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT);
  stk::StkFloat arg3 = (stk::StkFloat)*(m_float*)MEM(0+SZ_INT+SZ_FLOAT);
  (arg1)->noteOn(arg2,arg3);
}

  class SwigWrapper {
  public: static bool CPPIMPORT(Gwi gwi) {
  gwi_struct_ini(gwi, (m_str)"stk");

gwi_typedef_ini(gwi, (m_str)"float", (m_str)"StkFloat");
gwi_typedef_end(gwi, ae_flag_none);

stk::Stk::setSampleRate(gwi->gwion->vm->bbq->si->sr);
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK base class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Nearly all STK classes inherit from this class. The global sample rate and");
  gwinote(gwi, "rawwave path variables can be queried and modified via Stk. In addition, this");
  gwinote(gwi, "class provides error handling and byte-swapping functions.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The Synthesis ToolKit in C++ (STK) is a set of open source audio signal");
  gwinote(gwi, "processing and algorithmic synthesis classes written in the C++ programming");
  gwinote(gwi, "language. STK was designed to facilitate rapid development of music synthesis");
  gwinote(gwi, "and audio processing software, with an emphasis on cross-platform functionality,");
  gwinote(gwi, "realtime control, ease of use, and educational example code. STK currently runs");
  gwinote(gwi, "with realtime support (audio and MIDI) on Linux, Macintosh OS X, and Windows");
  gwinote(gwi, "computer platforms. Generic, non-realtime support has been tested under");
  gwinote(gwi, "NeXTStep, Sun, and other platforms and should work with any standard C++");
  gwinote(gwi, "compiler.  ");
  gwinote(gwi, "");
  gwinote(gwi, "STK WWW site: http://ccrma.stanford.edu/software/stk/  ");
  gwinote(gwi, "");
  gwinote(gwi, "The Synthesis ToolKit in C++ (STK) Copyright (c) 1995--2021 Perry R. Cook and");
  gwinote(gwi, "Gary P. Scavone  ");
  gwinote(gwi, "");
  gwinote(gwi, "Permission is hereby granted, free of charge, to any person obtaining a copy of");
  gwinote(gwi, "this software and associated documentation files (the \"Software\"), to deal in");
  gwinote(gwi, "the Software without restriction, including without limitation the rights to");
  gwinote(gwi, "use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of");
  gwinote(gwi, "the Software, and to permit persons to whom the Software is furnished to do so,");
  gwinote(gwi, "subject to the following conditions:  ");
  gwinote(gwi, "");
  gwinote(gwi, "The above copyright notice and this permission notice shall be included in all");
  gwinote(gwi, "copies or substantial portions of the Software.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Any person wishing to distribute modifications to the Software is asked to send");
  gwinote(gwi, "the modifications to the original developer so that they can be incorporated");
  gwinote(gwi, "into the canonical version. This is, however, not a binding provision of this");
  gwinote(gwi, "license.  ");
  gwinote(gwi, "");
  gwinote(gwi, "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
  gwinote(gwi, "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS");
  gwinote(gwi, "FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR");
  gwinote(gwi, "COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER");
  gwinote(gwi, "IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN");
  gwinote(gwi, "CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Attributes");
  gwinote(gwi, "----------");
  gwinote(gwi, "* `STK_SINT8` : `const StkFormat`  ");
  gwinote(gwi, "    -128 to +127  ");
  gwinote(gwi, "");
  gwinote(gwi, "* `STK_SINT16` : `const StkFormat`  ");
  gwinote(gwi, "    -32768 to +32767  ");
  gwinote(gwi, "");
  gwinote(gwi, "* `STK_SINT24` : `const StkFormat`  ");
  gwinote(gwi, "    Lower 3 bytes of 32-bit signed integer.  ");
  gwinote(gwi, "");
  gwinote(gwi, "* `STK_SINT32` : `const StkFormat`  ");
  gwinote(gwi, "    -2147483648 to +2147483647.  ");
  gwinote(gwi, "");
  gwinote(gwi, "* `STK_FLOAT32` : `const StkFormat`  ");
  gwinote(gwi, "    Normalized between plus/minus 1.0.  ");
  gwinote(gwi, "");
  gwinote(gwi, "* `STK_FLOAT64` : `const StkFormat`  ");
  gwinote(gwi, "    Normalized between plus/minus 1.0.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Stk.h");
  gwinote(gwi, "");
  const Type t_Stk = gwi_class_ini(gwi, "Stk", "UGen");
  t_Stk->nspc->offset += SZ_INT;
  GWI_B(gwi_func_ini(gwi, "int", "STK_SINT8"));
  GWI_B(gwi_func_end(gwi, gw_Stk_STK_SINT8_get, ae_flag_static));
  GWI_B(gwi_func_ini(gwi, "int", "STK_SINT16"));
  GWI_B(gwi_func_end(gwi, gw_Stk_STK_SINT16_get, ae_flag_static));
  GWI_B(gwi_func_ini(gwi, "int", "STK_SINT24"));
  GWI_B(gwi_func_end(gwi, gw_Stk_STK_SINT24_get, ae_flag_static));
  GWI_B(gwi_func_ini(gwi, "int", "STK_SINT32"));
  GWI_B(gwi_func_end(gwi, gw_Stk_STK_SINT32_get, ae_flag_static));
  GWI_B(gwi_func_ini(gwi, "int", "STK_FLOAT32"));
  GWI_B(gwi_func_end(gwi, gw_Stk_STK_FLOAT32_get, ae_flag_static));
  GWI_B(gwi_func_ini(gwi, "int", "STK_FLOAT64"));
  GWI_B(gwi_func_end(gwi, gw_Stk_STK_FLOAT64_get, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::sampleRate");
  gwinote(gwi, "Static method that returns the current STK sample rate.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "sampleRate"));
  GWI_B(gwi_func_end(gwi, gw_Stk_sampleRate, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::setSampleRate");
  gwinote(gwi, "Static method that sets the STK sample rate.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The sample rate set using this method is queried by all STK classes that depend");
  gwinote(gwi, "on its value. It is initialized to the default SRATE set in Stk.h. Many STK");
  gwinote(gwi, "classes use the sample rate during instantiation. Therefore, if you wish to use");
  gwinote(gwi, "a rate that is different from the default rate, it is imperative that it be set");
  gwinote(gwi, "*BEFORE* STK objects are instantiated. A few classes that make use of the global");
  gwinote(gwi, "STK sample rate are automatically notified when the rate changes so that");
  gwinote(gwi, "internal class data can be appropriately updated. However, this has not been");
  gwinote(gwi, "fully implemented. Specifically, classes that appropriately update their own");
  gwinote(gwi, "data when either a setFrequency() or noteOn() function is called do not");
  gwinote(gwi, "currently receive the automatic notification of rate change. If the user wants a");
  gwinote(gwi, "specific class instance to ignore such notifications, perhaps in a multi-rate");
  gwinote(gwi, "context, the function Stk::ignoreSampleRateChange() should be called.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSampleRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Stk_setSampleRate, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function allows the user to enable or disable class data updates in");
  gwinote(gwi, "response to global sample rate changes on a class by class basis.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Stk_ignoreSampleRateChange0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function allows the user to enable or disable class data updates in");
  gwinote(gwi, "response to global sample rate changes on a class by class basis.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_end(gwi, gw_Stk_ignoreSampleRateChange1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::clear_alertList");
  gwinote(gwi, "Static method that frees memory from alertList_.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear_alertList"));
  GWI_B(gwi_func_end(gwi, gw_Stk_clear_alertList, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::rawwavePath");
  gwinote(gwi, "Static method that returns the current rawwave path.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "string", "rawwavePath"));
  GWI_B(gwi_func_end(gwi, gw_Stk_rawwavePath, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::setRawwavePath");
  gwinote(gwi, "Static method that sets the STK rawwave path.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRawwavePath"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Stk_setRawwavePath, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::sleep");
  gwinote(gwi, "Static cross-platform method to sleep for a number of milliseconds.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "sleep"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Stk_sleep, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::inRange");
  gwinote(gwi, "Static method to check whether a value is within a specified range.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "bool", "inRange"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Stk_inRange, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::showWarnings");
  gwinote(gwi, "Toggle display of WARNING and STATUS messages.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "showWarnings"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Stk_showWarnings, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Stk::printErrors");
  gwinote(gwi, "Toggle display of error messages before throwing exceptions.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "printErrors"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Stk_printErrors, ae_flag_static));
  GWI_B(gwi_class_end(gwi)); // Stk

  GWI_B(gwi_func_ini(gwi, "float", "SRATE"));
  GWI_B(gwi_func_end(gwi, gw_SRATE_get, ae_flag_none));
  GWI_B(gwi_func_ini(gwi, "int", "RT_BUFFER_SIZE"));
  GWI_B(gwi_func_end(gwi, gw_RT_BUFFER_SIZE_get, ae_flag_none));
  const M_Object RAWWAVE_PATH_value = new_string(gwi->gwion, (m_str)"../../rawwaves/");
  GWI_B(gwi_item_ini(gwi, "string", "RAWWAVE_PATH"));
  GWI_B(gwi_item_end(gwi, ae_flag_const, obj, RAWWAVE_PATH_value));

  GWI_B(gwi_func_ini(gwi, "float", "PI"));
  GWI_B(gwi_func_end(gwi, gw_PI_get, ae_flag_none));
  GWI_B(gwi_func_ini(gwi, "float", "TWO_PI"));
  GWI_B(gwi_func_end(gwi, gw_TWO_PI_get, ae_flag_none));
  GWI_B(gwi_func_ini(gwi, "float", "ONE_OVER_128"));
  GWI_B(gwi_func_end(gwi, gw_ONE_OVER_128_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK instrument abstract base class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class provides a common interface for all STK instruments.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Instrmnt.h");
  gwinote(gwi, "");
  const Type t_Instrmnt = gwi_class_ini(gwi, "Instrmnt", "stk.Stk");
  gwi_class_xtor(gwi, NULL, gw_Instrmnt_dtor);
  SET_FLAG(t_Instrmnt, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Instrmnt::clear");
  gwinote(gwi, "Reset and clear all internal state (for subclasses).  ");
  gwinote(gwi, "");
  gwinote(gwi, "Not all subclasses implement a clear() function.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Instrmnt_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Instrmnt::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Instrmnt_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Instrmnt::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Instrmnt_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Instrmnt::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Instrmnt_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Instrmnt::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Instrmnt_controlChange, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Instrmnt::channelsOut");
  gwinote(gwi, "Return the number of output channels for the class.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "channelsOut"));
  GWI_B(gwi_func_end(gwi, gw_Instrmnt_channelsOut, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Instrmnt

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK abstract unit generator parent class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class provides limited common functionality for STK unit generator sample-");
  gwinote(gwi, "source subclasses. It is general enough to support both monophonic and");
  gwinote(gwi, "polyphonic output classes.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Generator.h");
  gwinote(gwi, "");
  const Type t_Generator = gwi_class_ini(gwi, "Generator", "stk.Stk");
  gwi_class_xtor(gwi, NULL, gw_Generator_dtor);
  SET_FLAG(t_Generator, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Generator::channelsOut");
  gwinote(gwi, "Return the number of output channels for the class.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "channelsOut"));
  GWI_B(gwi_func_end(gwi, gw_Generator_channelsOut, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Generator

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK abstract FM synthesis base class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class controls an arbitrary number of waves and envelopes, determined via a");
  gwinote(gwi, "constructor argument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Control One = 2  ");
  gwinote(gwi, "*   Control Two = 4  ");
  gwinote(gwi, "*   LFO Speed = 11  ");
  gwinote(gwi, "*   LFO Depth = 1  ");
  gwinote(gwi, "*   ADSR 2 & 4 Target = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "The basic Chowning/Stanford FM patent expired in 1995, but there exist follow-on");
  gwinote(gwi, "patents, mostly assigned to Yamaha. If you are of the type who should worry");
  gwinote(gwi, "about this (making money) worry away.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: FM.h");
  gwinote(gwi, "");
  const Type t_FM = gwi_class_ini(gwi, "FM", "stk.Instrmnt");
  gwi_class_xtor(gwi, NULL, gw_FM_dtor);
  SET_FLAG(t_FM, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::loadWaves");
  gwinote(gwi, "Load the rawwave filenames in waves.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "loadWaves"));
  GWI_B(gwi_func_arg(gwi, "string[]", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FM_loadWaves, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FM_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::setRatio");
  gwinote(gwi, "Set the frequency ratio for the specified wave.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRatio"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FM_setRatio, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::setGain");
  gwinote(gwi, "Set the gain for the specified wave.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGain"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FM_setGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::setModulationSpeed");
  gwinote(gwi, "Set the modulation speed in Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setModulationSpeed"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FM_setModulationSpeed, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::setModulationDepth");
  gwinote(gwi, "Set the modulation depth.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setModulationDepth"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FM_setModulationDepth, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::setControl1");
  gwinote(gwi, "Set the value of control1.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setControl1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FM_setControl1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::setControl2");
  gwinote(gwi, "Set the value of control1.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setControl2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FM_setControl2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::keyOn");
  gwinote(gwi, "Start envelopes toward \"on\" targets.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOn"));
  GWI_B(gwi_func_end(gwi, gw_FM_keyOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::keyOff");
  gwinote(gwi, "Start envelopes toward \"off\" targets.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOff"));
  GWI_B(gwi_func_end(gwi, gw_FM_keyOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FM_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FM::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FM_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // FM

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK linear line envelope class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple linear line envelope generator which is capable");
  gwinote(gwi, "of ramping to an arbitrary target value by a specified *rate*. It also responds");
  gwinote(gwi, "to simple *keyOn* and *keyOff* messages, ramping to a specified target (default");
  gwinote(gwi, "= 1.0) on keyOn and to a specified target (default = 0.0) on keyOff.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Envelope.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Envelope", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_Envelope_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::Envelope");
  gwinote(gwi, "Default constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::keyOn");
  gwinote(gwi, "Start ramping to specified target (default = 1).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_keyOn0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::keyOn");
  gwinote(gwi, "Start ramping to specified target (default = 1).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOn"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_keyOn1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::keyOff");
  gwinote(gwi, "Start ramping to specified target (default = 0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_keyOff0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::keyOff");
  gwinote(gwi, "Start ramping to specified target (default = 0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOff"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_keyOff1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::setRate");
  gwinote(gwi, "Set the *rate*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The *rate* must be positive (though a value of 0.0 is allowed).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_setRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::setTime");
  gwinote(gwi, "Set the *rate* based on a positive time duration (seconds).  ");
  gwinote(gwi, "");
  gwinote(gwi, "The *rate* is calculated such that the envelope will ramp from the current value");
  gwinote(gwi, "to the current target in the specified time duration.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTime"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_setTime, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::setTarget");
  gwinote(gwi, "Set the target value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTarget"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_setTarget, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::setValue");
  gwinote(gwi, "Set current and target values to *value*.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setValue"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_setValue, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Envelope::getState");
  gwinote(gwi, "Return the current envelope *state* (0 = at target, 1 otherwise).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "getState"));
  GWI_B(gwi_func_end(gwi, gw_Envelope_getState, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Envelope

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK abstract filter class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class provides limited common functionality for STK digital filter");
  gwinote(gwi, "subclasses. It is general enough to support both monophonic and polyphonic");
  gwinote(gwi, "input/output classes.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Filter.h");
  gwinote(gwi, "");
  const Type t_Filter = gwi_class_ini(gwi, "Filter", "stk.Stk");
  gwi_class_xtor(gwi, NULL, gw_Filter_dtor);
  SET_FLAG(t_Filter, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Filter::channelsIn");
  gwinote(gwi, "Return the number of input channels for the class.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "channelsIn"));
  GWI_B(gwi_func_end(gwi, gw_Filter_channelsIn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Filter::channelsOut");
  gwinote(gwi, "Return the number of output channels for the class.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "channelsOut"));
  GWI_B(gwi_func_end(gwi, gw_Filter_channelsOut, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Filter::clear");
  gwinote(gwi, "Clears all internal states of the filter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Filter_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Filter::setGain");
  gwinote(gwi, "Set the filter gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The gain is applied at the filter input and does not affect the coefficient");
  gwinote(gwi, "values. The default gain value is 1.0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Filter_setGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Filter::getGain");
  gwinote(gwi, "Return the current filter gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getGain"));
  GWI_B(gwi_func_end(gwi, gw_Filter_getGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Filter::phaseDelay");
  gwinote(gwi, "Return the filter phase delay at the specified frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Note that the phase delay calculation accounts for the filter gain. The");
  gwinote(gwi, "frequency value should be greater than 0.0 and less than or equal to one-half");
  gwinote(gwi, "the sample rate.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "phaseDelay"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Filter_phaseDelay, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Filter

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK abstract effects parent class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class provides common functionality for STK effects subclasses. It is");
  gwinote(gwi, "general enough to support both monophonic and polyphonic input/output classes.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Effect.h");
  gwinote(gwi, "");
  const Type t_Effect = gwi_class_ini(gwi, "Effect", "stk.Stk");
  gwi_class_xtor(gwi, NULL, gw_Effect_dtor);
  SET_FLAG(t_Effect, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Effect::channelsOut");
  gwinote(gwi, "Return the number of output channels for the class.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "channelsOut"));
  GWI_B(gwi_func_end(gwi, gw_Effect_channelsOut, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Effect::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Effect_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Effect::setEffectMix");
  gwinote(gwi, "Set the mixture of input and \"effected\" levels in the output (0.0 = input");
  gwinote(gwi, "only, 1.0 = effect only).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setEffectMix"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Effect_setEffectMix, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Effect

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK abstract function parent class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class provides common functionality for STK classes that implement tables");
  gwinote(gwi, "or other types of input to output function mappings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Function.h");
  gwinote(gwi, "");
  const Type t_Function = gwi_class_ini(gwi, "Function", "stk.Stk");
  gwi_class_xtor(gwi, NULL, gw_Function_dtor);
  SET_FLAG(t_Function, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Function::lastOut");
  gwinote(gwi, "Return the last computed output sample.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "lastOut"));
  GWI_B(gwi_func_end(gwi, gw_Function_lastOut, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Function

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK sampling synthesis abstract base class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This instrument provides an ADSR envelope, a one-pole filter, and structures for");
  gwinote(gwi, "an arbitrary number of attack and looped files.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Sampler.h");
  gwinote(gwi, "");
  const Type t_Sampler = gwi_class_ini(gwi, "Sampler", "stk.Instrmnt");
  gwi_class_xtor(gwi, NULL, gw_Sampler_dtor);
  SET_FLAG(t_Sampler, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sampler::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Sampler_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sampler::keyOn");
  gwinote(gwi, "Initiate the envelopes with a key-on event and reset the attack waves.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOn"));
  GWI_B(gwi_func_end(gwi, gw_Sampler_keyOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sampler::keyOff");
  gwinote(gwi, "Signal a key-off event to the envelopes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOff"));
  GWI_B(gwi_func_end(gwi, gw_Sampler_keyOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sampler::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Sampler_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sampler::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Sampler_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Sampler

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK ADSR envelope class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a traditional ADSR (Attack, Decay, Sustain, Release)");
  gwinote(gwi, "envelope. It responds to simple keyOn and keyOff messages, keeping track of its");
  gwinote(gwi, "state. The *state* = ADSR::IDLE before being triggered and after the envelope");
  gwinote(gwi, "value reaches 0.0 in the ADSR::RELEASE state. All rate, target and level");
  gwinote(gwi, "settings must be non-negative. All time settings are in seconds and must be");
  gwinote(gwi, "positive.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: ADSR.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "ADSR", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_ADSR_dtor);
  GWI_B(gwi_enum_ini(gwi, (m_str)"enum0"));
  GWI_B(gwi_enum_add(gwi, (m_str)"ATTACK", (m_uint)stk::ADSR::ATTACK));
  GWI_B(gwi_enum_add(gwi, (m_str)"DECAY", (m_uint)stk::ADSR::DECAY));
  GWI_B(gwi_enum_add(gwi, (m_str)"SUSTAIN", (m_uint)stk::ADSR::SUSTAIN));
  GWI_B(gwi_enum_add(gwi, (m_str)"RELEASE", (m_uint)stk::ADSR::RELEASE));
  GWI_B(gwi_enum_add(gwi, (m_str)"IDLE", (m_uint)stk::ADSR::IDLE));
  GWI_B(gwi_enum_end(gwi));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::ADSR");
  gwinote(gwi, "Default constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::keyOn");
  gwinote(gwi, "Set target = 1, state = *ADSR::ATTACK*.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOn"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_keyOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::keyOff");
  gwinote(gwi, "Set target = 0, state = *ADSR::RELEASE*.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOff"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_keyOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setAttackRate");
  gwinote(gwi, "Set the attack rate (gain / sample).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setAttackRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setAttackRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setAttackTarget");
  gwinote(gwi, "Set the target value for the attack (default = 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setAttackTarget"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setAttackTarget, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setDecayRate");
  gwinote(gwi, "Set the decay rate (gain / sample).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDecayRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setDecayRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setSustainLevel");
  gwinote(gwi, "Set the sustain level.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSustainLevel"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setSustainLevel, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setReleaseRate");
  gwinote(gwi, "Set the release rate (gain / sample).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setReleaseRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setReleaseRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setAttackTime");
  gwinote(gwi, "Set the attack rate based on a time duration (seconds).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setAttackTime"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setAttackTime, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setDecayTime");
  gwinote(gwi, "Set the decay rate based on a time duration (seconds).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDecayTime"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setDecayTime, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setReleaseTime");
  gwinote(gwi, "Set the release rate based on a time duration (seconds).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setReleaseTime"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setReleaseTime, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setAllTimes");
  gwinote(gwi, "Set sustain level and attack, decay, and release time durations (seconds).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setAllTimes"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_arg(gwi, "float", "arg5"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setAllTimes, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setTarget");
  gwinote(gwi, "Set a sustain target value and attack or decay from current value to target.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTarget"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setTarget, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::getState");
  gwinote(gwi, "Return the current envelope *state* (ATTACK, DECAY, SUSTAIN, RELEASE, IDLE).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "getState"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_getState, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ADSR::setValue");
  gwinote(gwi, "Set to state = ADSR::SUSTAIN with current and target values of *value*.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setValue"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ADSR_setValue, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // ADSR

  GWI_B(gwi_func_ini(gwi, "float", "TARGET_THRESHOLD"));
  GWI_B(gwi_func_end(gwi, gw_TARGET_THRESHOLD_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK asymptotic curve envelope class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple envelope generator which asymptotically");
  gwinote(gwi, "approaches a target value. The algorithm used is of the form:  ");
  gwinote(gwi, "");
  gwinote(gwi, "y[n] = a y[n-1] + (1-a) target,  ");
  gwinote(gwi, "");
  gwinote(gwi, "where a = exp(-T/tau), T is the sample period, and tau is a time constant. The");
  gwinote(gwi, "user can set the time constant (default value = 0.3) and target value.");
  gwinote(gwi, "Theoretically, this recursion never reaches its target, though the calculations");
  gwinote(gwi, "in this class are stopped when the current value gets within a small threshold");
  gwinote(gwi, "value of the target (at which time the current value is set to the target). It");
  gwinote(gwi, "responds to *keyOn* and *keyOff* messages by ramping to 1.0 on keyOn and to 0.0");
  gwinote(gwi, "on keyOff.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Asymp.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Asymp", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_Asymp_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::Asymp");
  gwinote(gwi, "Default constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::keyOn");
  gwinote(gwi, "Set target = 1.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOn"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_keyOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::keyOff");
  gwinote(gwi, "Set target = 0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOff"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_keyOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::setTau");
  gwinote(gwi, "Set the asymptotic rate via the time factor *tau* (must be > 0).  ");
  gwinote(gwi, "");
  gwinote(gwi, "The rate is computed as described above. The value of *tau* must be greater than");
  gwinote(gwi, "zero. Values of *tau* close to zero produce fast approach rates, while values");
  gwinote(gwi, "greater than 1.0 produce rather slow rates.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTau"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_setTau, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::setTime");
  gwinote(gwi, "Set the asymptotic rate based on a time duration (must be > 0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTime"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_setTime, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::setT60");
  gwinote(gwi, "Set the asymptotic rate such that the target value is perceptually reached (to");
  gwinote(gwi, "within -60dB of the target) in *t60* seconds.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setT60"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_setT60, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::setTarget");
  gwinote(gwi, "Set the target value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTarget"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_setTarget, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::setValue");
  gwinote(gwi, "Set current and target values to *value*.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setValue"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_setValue, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Asymp::getState");
  gwinote(gwi, "Return the current envelope *state* (0 = at target, 1 otherwise).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "getState"));
  GWI_B(gwi_func_end(gwi, gw_Asymp_getState, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Asymp

  GWI_B(gwi_func_ini(gwi, "int", "MAX_BANDED_MODES"));
  GWI_B(gwi_func_end(gwi, gw_MAX_BANDED_MODES_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Banded waveguide modeling class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class uses banded waveguide techniques to model a variety of sounds,");
  gwinote(gwi, "including bowed bars, glasses, and bowls. For more information, see Essl, G. and");
  gwinote(gwi, "Cook, P. \"Banded");
  gwinote(gwi, "Waveguides: Towards Physical Modelling of Bar");
  gwinote(gwi, "Percussion Instruments\", Proceedings of the 1999 International Computer Music");
  gwinote(gwi, "Conference.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Bow Pressure = 2  ");
  gwinote(gwi, "*   Bow Motion = 4  ");
  gwinote(gwi, "*   Strike Position = 8 (not implemented)  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Gain = 1  ");
  gwinote(gwi, "*   Bow Velocity = 128  ");
  gwinote(gwi, "*   Set Striking = 64  ");
  gwinote(gwi, "*   Instrument Presets = 16");
  gwinote(gwi, "    -   Uniform Bar = 0  ");
  gwinote(gwi, "    -   Tuned Bar = 1  ");
  gwinote(gwi, "    -   Glass Harmonica = 2  ");
  gwinote(gwi, "    -   Tibetan Bowl = 3  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Georg Essl, 1999--2004. Modified for STK 4.0 by Gary Scavone.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: BandedWG.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "BandedWG", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_BandedWG_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::BandedWG");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::setStrikePosition");
  gwinote(gwi, "Set strike position (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setStrikePosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_setStrikePosition, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::setPreset");
  gwinote(gwi, "Select a preset.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPreset"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_setPreset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::startBowing");
  gwinote(gwi, "Apply bow velocity/pressure to instrument with given amplitude and rate of");
  gwinote(gwi, "increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_startBowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::stopBowing");
  gwinote(gwi, "Decrease bow velocity/breath pressure with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_stopBowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::pluck");
  gwinote(gwi, "Pluck the instrument with given amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pluck"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_pluck, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BandedWG::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BandedWG_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // BandedWG

  GWI_B(gwi_func_ini(gwi, "float", "RECIP_SQRT_2"));
  GWI_B(gwi_func_end(gwi, gw_RECIP_SQRT_2_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK biquad (two-pole, two-zero) filter class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a two-pole, two-zero digital filter. Methods are provided");
  gwinote(gwi, "for creating a resonance or notch in the frequency response while maintaining a");
  gwinote(gwi, "constant filter gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Formulae used calculate coefficients for lowpass, highpass, bandpass, bandreject");
  gwinote(gwi, "and allpass are found on pg. 55 of Udo Zölzer's \"DAFX - Digital Audio Effects\"");
  gwinote(gwi, "(2011 2nd ed).  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: BiQuad.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "BiQuad", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_BiQuad_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::BiQuad");
  gwinote(gwi, "Default constructor creates a second-order pass-through filter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_ignoreSampleRateChange0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_ignoreSampleRateChange1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_arg(gwi, "float", "arg5"));
  GWI_B(gwi_func_arg(gwi, "float", "arg6"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg7"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setCoefficients0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_arg(gwi, "float", "arg5"));
  GWI_B(gwi_func_arg(gwi, "float", "arg6"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setCoefficients1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setB0");
  gwinote(gwi, "Set the b[0] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB0"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setB0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setB1");
  gwinote(gwi, "Set the b[1] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setB1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setB2");
  gwinote(gwi, "Set the b[2] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setB2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setA1");
  gwinote(gwi, "Set the a[1] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setA1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setA2");
  gwinote(gwi, "Set the a[2] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setA2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setResonance");
  gwinote(gwi, "Sets the filter coefficients for a resonance at *frequency* (in Hz).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to two complex-");
  gwinote(gwi, "conjugate poles with the given *frequency* (in Hz) and *radius* from the z-plane");
  gwinote(gwi, "origin. If *normalize* is true, the filter zeros are placed at z = 1, z = -1,");
  gwinote(gwi, "and the coefficients are then normalized to produce a constant unity peak gain");
  gwinote(gwi, "(independent of the filter *gain* parameter). The resulting filter frequency");
  gwinote(gwi, "response has a resonance at the given *frequency*. The closer the poles are to");
  gwinote(gwi, "the unit-circle (*radius* close to one), the narrower the resulting resonance");
  gwinote(gwi, "width. An unstable filter will result for *radius* >= 1.0. The *frequency* value");
  gwinote(gwi, "should be between zero and half the sample rate.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setResonance"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setResonance0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setResonance");
  gwinote(gwi, "Sets the filter coefficients for a resonance at *frequency* (in Hz).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to two complex-");
  gwinote(gwi, "conjugate poles with the given *frequency* (in Hz) and *radius* from the z-plane");
  gwinote(gwi, "origin. If *normalize* is true, the filter zeros are placed at z = 1, z = -1,");
  gwinote(gwi, "and the coefficients are then normalized to produce a constant unity peak gain");
  gwinote(gwi, "(independent of the filter *gain* parameter). The resulting filter frequency");
  gwinote(gwi, "response has a resonance at the given *frequency*. The closer the poles are to");
  gwinote(gwi, "the unit-circle (*radius* close to one), the narrower the resulting resonance");
  gwinote(gwi, "width. An unstable filter will result for *radius* >= 1.0. The *frequency* value");
  gwinote(gwi, "should be between zero and half the sample rate.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setResonance"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setResonance1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setNotch");
  gwinote(gwi, "Set the filter coefficients for a notch at *frequency* (in Hz).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to two complex-");
  gwinote(gwi, "conjugate zeros with the given *frequency* (in Hz) and *radius* from the z-plane");
  gwinote(gwi, "origin. No filter normalization is attempted. The *frequency* value should be");
  gwinote(gwi, "between zero and half the sample rate. The *radius* value should be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setNotch"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setNotch, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setLowPass");
  gwinote(gwi, "Set the filter coefficients for a low-pass with cutoff frequency *fc* (in Hz)");
  gwinote(gwi, "and Q-factor *Q*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to a low-pass");
  gwinote(gwi, "filter with cutoff placed at *fc*, where sloping behaviour and resonance are");
  gwinote(gwi, "determined by *Q*. The default value for *Q* is 1/sqrt(2), resulting in a");
  gwinote(gwi, "gradual attenuation of frequencies higher than *fc* without added resonance.");
  gwinote(gwi, "Values greater than this will more aggressively attenuate frequencies above *fc*");
  gwinote(gwi, "while also adding a resonance at *fc*. Values less than this will result in a");
  gwinote(gwi, "more gradual attenuation of frequencies above *fc*, but will also attenuate");
  gwinote(gwi, "frequencies below *fc* as well. Both *fc* and *Q* must be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setLowPass"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setLowPass0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setLowPass");
  gwinote(gwi, "Set the filter coefficients for a low-pass with cutoff frequency *fc* (in Hz)");
  gwinote(gwi, "and Q-factor *Q*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to a low-pass");
  gwinote(gwi, "filter with cutoff placed at *fc*, where sloping behaviour and resonance are");
  gwinote(gwi, "determined by *Q*. The default value for *Q* is 1/sqrt(2), resulting in a");
  gwinote(gwi, "gradual attenuation of frequencies higher than *fc* without added resonance.");
  gwinote(gwi, "Values greater than this will more aggressively attenuate frequencies above *fc*");
  gwinote(gwi, "while also adding a resonance at *fc*. Values less than this will result in a");
  gwinote(gwi, "more gradual attenuation of frequencies above *fc*, but will also attenuate");
  gwinote(gwi, "frequencies below *fc* as well. Both *fc* and *Q* must be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setLowPass"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setLowPass1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setHighPass");
  gwinote(gwi, "Set the filter coefficients for a high-pass with cutoff frequency *fc* (in Hz)");
  gwinote(gwi, "and Q-factor *Q*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to a high-pass");
  gwinote(gwi, "filter with cutoff placed at *fc*, where sloping behaviour and resonance are");
  gwinote(gwi, "determined by *Q*. The default value for *Q* is 1/sqrt(2), resulting in a");
  gwinote(gwi, "gradual attenuation of frequencies lower than *fc* without added resonance.");
  gwinote(gwi, "Values greater than this will more aggressively attenuate frequencies below *fc*");
  gwinote(gwi, "while also adding a resonance at *fc*. Values less than this will result in a");
  gwinote(gwi, "more gradual attenuation of frequencies below *fc*, but will also attenuate");
  gwinote(gwi, "frequencies above *fc* as well. Both *fc* and *Q* must be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setHighPass"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setHighPass0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setHighPass");
  gwinote(gwi, "Set the filter coefficients for a high-pass with cutoff frequency *fc* (in Hz)");
  gwinote(gwi, "and Q-factor *Q*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to a high-pass");
  gwinote(gwi, "filter with cutoff placed at *fc*, where sloping behaviour and resonance are");
  gwinote(gwi, "determined by *Q*. The default value for *Q* is 1/sqrt(2), resulting in a");
  gwinote(gwi, "gradual attenuation of frequencies lower than *fc* without added resonance.");
  gwinote(gwi, "Values greater than this will more aggressively attenuate frequencies below *fc*");
  gwinote(gwi, "while also adding a resonance at *fc*. Values less than this will result in a");
  gwinote(gwi, "more gradual attenuation of frequencies below *fc*, but will also attenuate");
  gwinote(gwi, "frequencies above *fc* as well. Both *fc* and *Q* must be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setHighPass"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setHighPass1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setBandPass");
  gwinote(gwi, "Set the filter coefficients for a band-pass centered at *fc* (in Hz) with");
  gwinote(gwi, "Q-factor *Q*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to a band-pass");
  gwinote(gwi, "filter with pass-band centered at *fc*, where band width and slope a determined");
  gwinote(gwi, "by *Q*. Values for *Q* that are less than 1.0 will attenuate frequencies above");
  gwinote(gwi, "and below *fc* more gradually, resulting in a convex slope and a wider band.");
  gwinote(gwi, "Values for *Q* greater than 1.0 will attenuate frequencies above and below *fc*");
  gwinote(gwi, "more aggressively, resulting in a concave slope and a narrower band. Both *fc*");
  gwinote(gwi, "and *Q* must be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBandPass"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setBandPass, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setBandReject");
  gwinote(gwi, "Set the filter coefficients for a band-reject centered at *fc* (in Hz) with");
  gwinote(gwi, "Q-factor *Q*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to a band-reject");
  gwinote(gwi, "filter with stop-band centered at *fc*, where band width and slope are");
  gwinote(gwi, "determined by *Q*. Values for *Q* that are less than 1.0 will yield a wider band");
  gwinote(gwi, "with greater attenuation of *fc*. Values for *Q* greater than 1.0 will yield a");
  gwinote(gwi, "narrower band with less attenuation of *fc*. Both *fc* and *Q* must be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBandReject"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setBandReject, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setAllPass");
  gwinote(gwi, "Set the filter coefficients for an all-pass centered at *fc* (in Hz) with");
  gwinote(gwi, "Q-factor *Q*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to an all-pass");
  gwinote(gwi, "filter whose phase response crosses -pi radians at *fc*. High values for *Q*");
  gwinote(gwi, "will result in a more instantaenous shift in phase response at *fc*. Lower");
  gwinote(gwi, "values will result in a more gradual shift in phase response around *fc*. Both");
  gwinote(gwi, "*fc* and *Q* must be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setAllPass"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setAllPass, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BiQuad::setEqualGainZeroes");
  gwinote(gwi, "Sets the filter zeroes for equal resonance gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "When using the filter as a resonator, zeroes places at z = 1, z = -1 will result");
  gwinote(gwi, "in a constant gain at resonance of 1 / (1 - R), where R is the pole radius");
  gwinote(gwi, "setting.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setEqualGainZeroes"));
  GWI_B(gwi_func_end(gwi, gw_BiQuad_setEqualGainZeroes, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // BiQuad

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK band-limited impulse train class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class generates a band-limited impulse train using a closed-form algorithm");
  gwinote(gwi, "reported by Stilson and Smith in \"Alias-Free");
  gwinote(gwi, "Digital Synthesis of Classic Analog Waveforms\", 1996. The user can specify both");
  gwinote(gwi, "the fundamental frequency of the impulse train and the number of harmonics");
  gwinote(gwi, "contained in the resulting signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The signal is normalized so that the peak value is +/-1.0.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If nHarmonics is 0, then the signal will contain all harmonics up to half the");
  gwinote(gwi, "sample rate. Note, however, that this setting may produce aliasing in the signal");
  gwinote(gwi, "when the frequency is changing (no automatic modification of the number of");
  gwinote(gwi, "harmonics is performed by the setFrequency() function).  ");
  gwinote(gwi, "");
  gwinote(gwi, "Original code by Robin Davies, 2005. Revisions by Gary Scavone for STK, 2005.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Blit.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Blit", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_Blit_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Blit::Blit");
  gwinote(gwi, "Default constructor that initializes BLIT frequency to 220 Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Blit_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Blit::Blit");
  gwinote(gwi, "Default constructor that initializes BLIT frequency to 220 Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Blit_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Blit::reset");
  gwinote(gwi, "Resets the oscillator state and phase to 0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "reset"));
  GWI_B(gwi_func_end(gwi, gw_Blit_reset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Blit::setPhase");
  gwinote(gwi, "Set the phase of the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Set the phase of the signal, in the range 0 to 1.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPhase"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Blit_setPhase, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Blit::getPhase");
  gwinote(gwi, "Get the current phase of the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Get the phase of the signal, in the range [0 to 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getPhase"));
  GWI_B(gwi_func_end(gwi, gw_Blit_getPhase, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Blit::setFrequency");
  gwinote(gwi, "Set the impulse train rate in terms of a frequency in Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Blit_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Blit::setHarmonics");
  gwinote(gwi, "Set the number of harmonics generated in the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function sets the number of harmonics contained in the resulting signal. It");
  gwinote(gwi, "is equivalent to (2 * M) + 1 in the BLIT algorithm. The default value of 0 sets");
  gwinote(gwi, "the algorithm for maximum harmonic content (harmonics up to half the sample");
  gwinote(gwi, "rate). This parameter is not checked against the current sample rate and");
  gwinote(gwi, "fundamental frequency. Thus, aliasing can result if one or more harmonics for a");
  gwinote(gwi, "given fundamental frequency exceeds fs / 2. This behavior was chosen over the");
  gwinote(gwi, "potentially more problematic solution of automatically modifying the M");
  gwinote(gwi, "parameter, which can produce audible clicks in the signal.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setHarmonics"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Blit_setHarmonics0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Blit::setHarmonics");
  gwinote(gwi, "Set the number of harmonics generated in the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function sets the number of harmonics contained in the resulting signal. It");
  gwinote(gwi, "is equivalent to (2 * M) + 1 in the BLIT algorithm. The default value of 0 sets");
  gwinote(gwi, "the algorithm for maximum harmonic content (harmonics up to half the sample");
  gwinote(gwi, "rate). This parameter is not checked against the current sample rate and");
  gwinote(gwi, "fundamental frequency. Thus, aliasing can result if one or more harmonics for a");
  gwinote(gwi, "given fundamental frequency exceeds fs / 2. This behavior was chosen over the");
  gwinote(gwi, "potentially more problematic solution of automatically modifying the M");
  gwinote(gwi, "parameter, which can produce audible clicks in the signal.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setHarmonics"));
  GWI_B(gwi_func_end(gwi, gw_Blit_setHarmonics1, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Blit

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK band-limited sawtooth wave class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class generates a band-limited sawtooth waveform using a closed-form");
  gwinote(gwi, "algorithm reported by Stilson and Smith in \"Alias-Free");
  gwinote(gwi, "Digital Synthesis of Classic Analog Waveforms\", 1996. The user can specify both");
  gwinote(gwi, "the fundamental frequency of the sawtooth and the number of harmonics contained");
  gwinote(gwi, "in the resulting signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If nHarmonics is 0, then the signal will contain all harmonics up to half the");
  gwinote(gwi, "sample rate. Note, however, that this setting may produce aliasing in the signal");
  gwinote(gwi, "when the frequency is changing (no automatic modification of the number of");
  gwinote(gwi, "harmonics is performed by the setFrequency() function).  ");
  gwinote(gwi, "");
  gwinote(gwi, "Based on initial code of Robin Davies, 2005. Modified algorithm code by Gary");
  gwinote(gwi, "Scavone, 2005.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: BlitSaw.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "BlitSaw", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_BlitSaw_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSaw::BlitSaw");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_BlitSaw_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSaw::BlitSaw");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_BlitSaw_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSaw::reset");
  gwinote(gwi, "Resets the oscillator state and phase to 0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "reset"));
  GWI_B(gwi_func_end(gwi, gw_BlitSaw_reset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSaw::setFrequency");
  gwinote(gwi, "Set the sawtooth oscillator rate in terms of a frequency in Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlitSaw_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSaw::setHarmonics");
  gwinote(gwi, "Set the number of harmonics generated in the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function sets the number of harmonics contained in the resulting signal. It");
  gwinote(gwi, "is equivalent to (2 * M) + 1 in the BLIT algorithm. The default value of 0 sets");
  gwinote(gwi, "the algorithm for maximum harmonic content (harmonics up to half the sample");
  gwinote(gwi, "rate). This parameter is not checked against the current sample rate and");
  gwinote(gwi, "fundamental frequency. Thus, aliasing can result if one or more harmonics for a");
  gwinote(gwi, "given fundamental frequency exceeds fs / 2. This behavior was chosen over the");
  gwinote(gwi, "potentially more problematic solution of automatically modifying the M");
  gwinote(gwi, "parameter, which can produce audible clicks in the signal.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setHarmonics"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlitSaw_setHarmonics0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSaw::setHarmonics");
  gwinote(gwi, "Set the number of harmonics generated in the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function sets the number of harmonics contained in the resulting signal. It");
  gwinote(gwi, "is equivalent to (2 * M) + 1 in the BLIT algorithm. The default value of 0 sets");
  gwinote(gwi, "the algorithm for maximum harmonic content (harmonics up to half the sample");
  gwinote(gwi, "rate). This parameter is not checked against the current sample rate and");
  gwinote(gwi, "fundamental frequency. Thus, aliasing can result if one or more harmonics for a");
  gwinote(gwi, "given fundamental frequency exceeds fs / 2. This behavior was chosen over the");
  gwinote(gwi, "potentially more problematic solution of automatically modifying the M");
  gwinote(gwi, "parameter, which can produce audible clicks in the signal.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setHarmonics"));
  GWI_B(gwi_func_end(gwi, gw_BlitSaw_setHarmonics1, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // BlitSaw

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK band-limited square wave class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class generates a band-limited square wave signal. It is derived in part");
  gwinote(gwi, "from the approach reported by Stilson and Smith in \"Alias-Free Digital");
  gwinote(gwi, "Synthesis of Classic Analog Waveforms\", 1996. The algorithm implemented in this");
  gwinote(gwi, "class uses a SincM function with an even M value to achieve a bipolar");
  gwinote(gwi, "bandlimited impulse train. This signal is then integrated to achieve a square");
  gwinote(gwi, "waveform. The integration process has an associated DC offset so a DC blocking");
  gwinote(gwi, "filter is applied at the output.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The user can specify both the fundamental frequency of the waveform and the");
  gwinote(gwi, "number of harmonics contained in the resulting signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If nHarmonics is 0, then the signal will contain all harmonics up to half the");
  gwinote(gwi, "sample rate. Note, however, that this setting may produce aliasing in the signal");
  gwinote(gwi, "when the frequency is changing (no automatic modification of the number of");
  gwinote(gwi, "harmonics is performed by the setFrequency() function). Also note that the");
  gwinote(gwi, "harmonics of a square wave fall at odd integer multiples of the fundamental, so");
  gwinote(gwi, "aliasing will happen with a lower fundamental than with the other Blit");
  gwinote(gwi, "waveforms. This class is not guaranteed to be well behaved in the presence of");
  gwinote(gwi, "significant aliasing.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Based on initial code of Robin Davies, 2005. Modified algorithm code by Gary");
  gwinote(gwi, "Scavone, 2005--2006.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: BlitSquare.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "BlitSquare", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_BlitSquare_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSquare::BlitSquare");
  gwinote(gwi, "Default constructor that initializes BLIT frequency to 220 Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_BlitSquare_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSquare::BlitSquare");
  gwinote(gwi, "Default constructor that initializes BLIT frequency to 220 Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_BlitSquare_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSquare::reset");
  gwinote(gwi, "Resets the oscillator state and phase to 0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "reset"));
  GWI_B(gwi_func_end(gwi, gw_BlitSquare_reset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSquare::setPhase");
  gwinote(gwi, "Set the phase of the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Set the phase of the signal, in the range 0 to 1.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPhase"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlitSquare_setPhase, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSquare::getPhase");
  gwinote(gwi, "Get the current phase of the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Get the phase of the signal, in the range [0 to 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getPhase"));
  GWI_B(gwi_func_end(gwi, gw_BlitSquare_getPhase, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSquare::setFrequency");
  gwinote(gwi, "Set the impulse train rate in terms of a frequency in Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlitSquare_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSquare::setHarmonics");
  gwinote(gwi, "Set the number of harmonics generated in the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function sets the number of harmonics contained in the resulting signal. It");
  gwinote(gwi, "is equivalent to (2 * M) + 1 in the BLIT algorithm. The default value of 0 sets");
  gwinote(gwi, "the algorithm for maximum harmonic content (harmonics up to half the sample");
  gwinote(gwi, "rate). This parameter is not checked against the current sample rate and");
  gwinote(gwi, "fundamental frequency. Thus, aliasing can result if one or more harmonics for a");
  gwinote(gwi, "given fundamental frequency exceeds fs / 2. This behavior was chosen over the");
  gwinote(gwi, "potentially more problematic solution of automatically modifying the M");
  gwinote(gwi, "parameter, which can produce audible clicks in the signal.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setHarmonics"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlitSquare_setHarmonics0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlitSquare::setHarmonics");
  gwinote(gwi, "Set the number of harmonics generated in the signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function sets the number of harmonics contained in the resulting signal. It");
  gwinote(gwi, "is equivalent to (2 * M) + 1 in the BLIT algorithm. The default value of 0 sets");
  gwinote(gwi, "the algorithm for maximum harmonic content (harmonics up to half the sample");
  gwinote(gwi, "rate). This parameter is not checked against the current sample rate and");
  gwinote(gwi, "fundamental frequency. Thus, aliasing can result if one or more harmonics for a");
  gwinote(gwi, "given fundamental frequency exceeds fs / 2. This behavior was chosen over the");
  gwinote(gwi, "potentially more problematic solution of automatically modifying the M");
  gwinote(gwi, "parameter, which can produce audible clicks in the signal.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setHarmonics"));
  GWI_B(gwi_func_end(gwi, gw_BlitSquare_setHarmonics1, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // BlitSquare

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK Hammond-oid organ FM synthesis instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple 4 operator topology, also referred to as");
  gwinote(gwi, "algorithm 8 of the TX81Z.  ");
  gwinote(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Operator 4 (feedback) Gain = 2  ");
  gwinote(gwi, "*   Operator 3 Gain = 4  ");
  gwinote(gwi, "*   LFO Speed = 11  ");
  gwinote(gwi, "*   LFO Depth = 1  ");
  gwinote(gwi, "*   ADSR 2 & 4 Target = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "The basic Chowning/Stanford FM patent expired in 1995, but there exist follow-on");
  gwinote(gwi, "patents, mostly assigned to Yamaha. If you are of the type who should worry");
  gwinote(gwi, "about this (making money) worry away.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: BeeThree.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "BeeThree", "stk.FM"));
  gwi_class_xtor(gwi, NULL, gw_BeeThree_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BeeThree::BeeThree");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_BeeThree_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BeeThree::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BeeThree_noteOn, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // BeeThree

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK blown bottle instrument class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a helmholtz resonator (biquad filter) with a polynomial");
  gwinote(gwi, "jet excitation (a la Cook).  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Noise Gain = 4  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Volume = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: BlowBotl.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "BlowBotl", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_BlowBotl_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowBotl::BlowBotl");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_BlowBotl_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowBotl::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_BlowBotl_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowBotl::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlowBotl_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowBotl::startBlowing");
  gwinote(gwi, "Apply breath velocity to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BlowBotl_startBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowBotl::stopBlowing");
  gwinote(gwi, "Decrease breath velocity with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlowBotl_stopBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowBotl::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BlowBotl_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowBotl::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlowBotl_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowBotl::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BlowBotl_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // BlowBotl

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK clarinet physical model with one register hole and one tonehole.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class is based on the clarinet model, with the addition of a two-port");
  gwinote(gwi, "register hole and a three-port dynamic tonehole implementation, as discussed by");
  gwinote(gwi, "Scavone and Cook (1998).  ");
  gwinote(gwi, "");
  gwinote(gwi, "In this implementation, the distances between the reed/register hole and");
  gwinote(gwi, "tonehole/bell are fixed. As a result, both the tonehole and register hole will");
  gwinote(gwi, "have variable influence on the playing frequency, which is dependent on the");
  gwinote(gwi, "length of the air column. In addition, the highest playing freqeuency is limited");
  gwinote(gwi, "by these fixed lengths.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Reed Stiffness = 2  ");
  gwinote(gwi, "*   Noise Gain = 4  ");
  gwinote(gwi, "*   Tonehole State = 11  ");
  gwinote(gwi, "*   Register State = 1  ");
  gwinote(gwi, "*   Breath Pressure = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: BlowHole.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "BlowHole", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_BlowHole_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::BlowHole");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::setTonehole");
  gwinote(gwi, "Set the tonehole state (0.0 = closed, 1.0 = fully open).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTonehole"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_setTonehole, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::setVent");
  gwinote(gwi, "Set the register hole state (0.0 = closed, 1.0 = fully open).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVent"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_setVent, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::startBlowing");
  gwinote(gwi, "Apply breath pressure to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_startBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::stopBlowing");
  gwinote(gwi, "Decrease breath pressure with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_stopBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BlowHole::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_BlowHole_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // BlowHole

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK bowed string instrument class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a bowed string model, a la Smith (1986), after McIntyre,");
  gwinote(gwi, "Schumacher, Woodhouse (1983).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Bow Pressure = 2  ");
  gwinote(gwi, "*   Bow Position = 4  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Bow Velocity = 100  ");
  gwinote(gwi, "*   Frequency = 101  ");
  gwinote(gwi, "*   Volume = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021. Contributions by Esteban");
  gwinote(gwi, "Maestre, 2011.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Bowed.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Bowed", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Bowed_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::Bowed");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::Bowed");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::setVibrato");
  gwinote(gwi, "Set vibrato gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVibrato"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_setVibrato, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::startBowing");
  gwinote(gwi, "Apply breath pressure to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_startBowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::stopBowing");
  gwinote(gwi, "Decrease breath pressure with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_stopBowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Bowed::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Bowed_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Bowed

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK bowed string table class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple bowed string non-linear function, as described by");
  gwinote(gwi, "Smith (1986). The output is an instantaneous reflection coefficient value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: BowTable.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "BowTable", "stk.Function"));
  gwi_class_xtor(gwi, NULL, gw_BowTable_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BowTable::BowTable");
  gwinote(gwi, "Default constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_BowTable_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BowTable::setOffset");
  gwinote(gwi, "Set the table offset value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The table offset is a bias which controls the symmetry of the friction. If you");
  gwinote(gwi, "want the friction to vary with direction, use a non-zero value for the offset.");
  gwinote(gwi, "The default value is zero.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setOffset"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BowTable_setOffset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BowTable::setSlope");
  gwinote(gwi, "Set the table slope value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The table slope controls the width of the friction pulse, which is related to");
  gwinote(gwi, "bow force.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSlope"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BowTable_setSlope, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BowTable::setMinOutput");
  gwinote(gwi, "Set the minimum table output value (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setMinOutput"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BowTable_setMinOutput, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::BowTable::setMaxOutput");
  gwinote(gwi, "Set the maximum table output value (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setMaxOutput"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_BowTable_setMaxOutput, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // BowTable

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK simple brass instrument class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple brass instrument waveguide model, a la Cook");
  gwinote(gwi, "(TBone, HosePlayer).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Lip Tension = 2  ");
  gwinote(gwi, "*   Slide Length = 4  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Volume = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Brass.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Brass", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Brass_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::Brass");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Brass_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::Brass");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Brass_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Brass_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Brass_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::setLip");
  gwinote(gwi, "Set the lips frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setLip"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Brass_setLip, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::startBlowing");
  gwinote(gwi, "Apply breath pressure to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Brass_startBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::stopBlowing");
  gwinote(gwi, "Decrease breath pressure with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Brass_stopBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Brass_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Brass_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Brass::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Brass_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Brass

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK chorus effect class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a chorus effect. It takes a monophonic input signal and");
  gwinote(gwi, "produces a stereo output signal.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Chorus.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Chorus", "stk.Effect"));
  gwi_class_xtor(gwi, NULL, gw_Chorus_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Chorus::Chorus");
  gwinote(gwi, "Class constructor, taking the median desired delay length.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError can be thrown if the rawwave path is incorrect.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Chorus_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Chorus::Chorus");
  gwinote(gwi, "Class constructor, taking the median desired delay length.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError can be thrown if the rawwave path is incorrect.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Chorus_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Chorus::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Chorus_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Chorus::setModDepth");
  gwinote(gwi, "Set modulation depth in range 0.0 - 1.0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setModDepth"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Chorus_setModDepth, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Chorus::setModFrequency");
  gwinote(gwi, "Set modulation frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setModFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Chorus_setModFrequency, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Chorus

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK clarinet physical model class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple clarinet physical model, as discussed by Smith");
  gwinote(gwi, "(1986), McIntyre, Schumacher, Woodhouse (1983), and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Reed Stiffness = 2  ");
  gwinote(gwi, "*   Noise Gain = 4  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Breath Pressure = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Clarinet.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Clarinet", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Clarinet_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::Clarinet");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::Clarinet");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::startBlowing");
  gwinote(gwi, "Apply breath pressure to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_startBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::stopBlowing");
  gwinote(gwi, "Decrease breath pressure with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_stopBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Clarinet::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Clarinet_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Clarinet

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK cubic non-linearity class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements the cubic non-linearity that was used in SynthBuilder.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The formula implemented is:  ");
  gwinote(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "followed by a limiter for values outside +-threshold.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Ported to STK by Nick Porcaro, 2007. Updated for inclusion in STK distribution");
  gwinote(gwi, "by Gary Scavone, 2011.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Cubic.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Cubic", "stk.Function"));
  gwi_class_xtor(gwi, NULL, gw_Cubic_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Cubic::Cubic");
  gwinote(gwi, "Default constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Cubic_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Cubic::setA1");
  gwinote(gwi, "Set the a1 coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Cubic_setA1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Cubic::setA2");
  gwinote(gwi, "Set the a2 coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Cubic_setA2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Cubic::setA3");
  gwinote(gwi, "Set the a3 coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Cubic_setA3, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Cubic::setGain");
  gwinote(gwi, "Set the gain value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Cubic_setGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Cubic::setThreshold");
  gwinote(gwi, "Set the threshold value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setThreshold"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Cubic_setThreshold, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Cubic

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK non-interpolating delay line class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a non-interpolating digital delay-line. If the delay and");
  gwinote(gwi, "maximum length are not specified during instantiation, a fixed maximum length of");
  gwinote(gwi, "4095 and a delay of zero is set.  ");
  gwinote(gwi, "");
  gwinote(gwi, "A non-interpolating delay line is typically used in fixed delay-length");
  gwinote(gwi, "applications, such as for reverberation.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Delay.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Delay", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_Delay_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::Delay");
  gwinote(gwi, "The default constructor creates a delay-line with maximum length of 4095 samples");
  gwinote(gwi, "and zero delay.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Delay_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::Delay");
  gwinote(gwi, "The default constructor creates a delay-line with maximum length of 4095 samples");
  gwinote(gwi, "and zero delay.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Delay_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::Delay");
  gwinote(gwi, "The default constructor creates a delay-line with maximum length of 4095 samples");
  gwinote(gwi, "and zero delay.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Delay_ctor2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::getMaximumDelay");
  gwinote(gwi, "Get the maximum delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "getMaximumDelay"));
  GWI_B(gwi_func_end(gwi, gw_Delay_getMaximumDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::setMaximumDelay");
  gwinote(gwi, "Set the maximum delay-line length.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method should generally only be used during initial setup of the delay");
  gwinote(gwi, "line. If it is used between calls to the tick() function, without a call to");
  gwinote(gwi, "clear(), a signal discontinuity will likely occur. If the current maximum length");
  gwinote(gwi, "is greater than the new length, no memory allocation change is made.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setMaximumDelay"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Delay_setMaximumDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::setDelay");
  gwinote(gwi, "Set the delay-line length.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The valid range for *delay* is from 0 to the maximum delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDelay"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Delay_setDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::getDelay");
  gwinote(gwi, "Return the current delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "getDelay"));
  GWI_B(gwi_func_end(gwi, gw_Delay_getDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::tapOut");
  gwinote(gwi, "Return the value at *tapDelay* samples from the delay-line input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The tap point is determined modulo the delay-line length and is relative to the");
  gwinote(gwi, "last input value (i.e., a tapDelay of zero returns the last input value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "tapOut"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Delay_tapOut, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::tapIn");
  gwinote(gwi, "Set the *value* at *tapDelay* samples from the delay-line input.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "tapIn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Delay_tapIn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::addTo");
  gwinote(gwi, "Sum the provided *value* into the delay line at *tapDelay* samples from the");
  gwinote(gwi, "input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The new value is returned. The tap point is determined modulo the delay-line");
  gwinote(gwi, "length and is relative to the last input value (i.e., a tapDelay of zero sums");
  gwinote(gwi, "into the last input value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "addTo"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Delay_addTo, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::nextOut");
  gwinote(gwi, "Return the value that will be output by the next call to tick().  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method is valid only for delay settings greater than zero!  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "nextOut"));
  GWI_B(gwi_func_end(gwi, gw_Delay_nextOut, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Delay::energy");
  gwinote(gwi, "Calculate and return the signal energy in the delay-line.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "energy"));
  GWI_B(gwi_func_end(gwi, gw_Delay_energy, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Delay

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK allpass interpolating delay line class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a fractional-length digital delay-line using a first-order");
  gwinote(gwi, "allpass filter. If the delay and maximum length are not specified during");
  gwinote(gwi, "instantiation, a fixed maximum length of 4095 and a delay of 0.5 is set.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An allpass filter has unity magnitude gain but variable phase delay properties,");
  gwinote(gwi, "making it useful in achieving fractional delays without affecting a signal's");
  gwinote(gwi, "frequency magnitude response. In order to achieve a maximally flat phase delay");
  gwinote(gwi, "response, the minimum delay possible in this implementation is limited to a");
  gwinote(gwi, "value of 0.5.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: DelayA.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "DelayA", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_DelayA_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::DelayA");
  gwinote(gwi, "Default constructor creates a delay-line with maximum length of 4095 samples and");
  gwinote(gwi, "delay = 0.5.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::DelayA");
  gwinote(gwi, "Default constructor creates a delay-line with maximum length of 4095 samples and");
  gwinote(gwi, "delay = 0.5.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::DelayA");
  gwinote(gwi, "Default constructor creates a delay-line with maximum length of 4095 samples and");
  gwinote(gwi, "delay = 0.5.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_ctor2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::clear");
  gwinote(gwi, "Clears all internal states of the delay line.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::getMaximumDelay");
  gwinote(gwi, "Get the maximum delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "getMaximumDelay"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_getMaximumDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::setMaximumDelay");
  gwinote(gwi, "Set the maximum delay-line length.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method should generally only be used during initial setup of the delay");
  gwinote(gwi, "line. If it is used between calls to the tick() function, without a call to");
  gwinote(gwi, "clear(), a signal discontinuity will likely occur. If the current maximum length");
  gwinote(gwi, "is greater than the new length, no memory allocation change is made.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setMaximumDelay"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_setMaximumDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::setDelay");
  gwinote(gwi, "Set the delay-line length.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The valid range for *delay* is from 0.5 to the maximum delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDelay"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_setDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::getDelay");
  gwinote(gwi, "Return the current delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getDelay"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_getDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::tapOut");
  gwinote(gwi, "Return the value at *tapDelay* samples from the delay-line input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The tap point is determined modulo the delay-line length and is relative to the");
  gwinote(gwi, "last input value (i.e., a tapDelay of zero returns the last input value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "tapOut"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_tapOut, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::tapIn");
  gwinote(gwi, "Set the *value* at *tapDelay* samples from the delay-line input.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "tapIn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_tapIn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayA::nextOut");
  gwinote(gwi, "Return the value which will be output by the next call to tick().  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method is valid only for delay settings greater than zero!  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "nextOut"));
  GWI_B(gwi_func_end(gwi, gw_DelayA_nextOut, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // DelayA

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK linear interpolating delay line class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a fractional-length digital delay-line using first-order");
  gwinote(gwi, "linear interpolation. If the delay and maximum length are not specified during");
  gwinote(gwi, "instantiation, a fixed maximum length of 4095 and a delay of zero is set.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Linear interpolation is an efficient technique for achieving fractional delay");
  gwinote(gwi, "lengths, though it does introduce high-frequency signal attenuation to varying");
  gwinote(gwi, "degrees depending on the fractional delay setting. The use of higher order");
  gwinote(gwi, "Lagrange interpolators can typically improve (minimize) this attenuation");
  gwinote(gwi, "characteristic.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: DelayL.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "DelayL", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_DelayL_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::DelayL");
  gwinote(gwi, "Default constructor creates a delay-line with maximum length of 4095 samples and");
  gwinote(gwi, "zero delay.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::DelayL");
  gwinote(gwi, "Default constructor creates a delay-line with maximum length of 4095 samples and");
  gwinote(gwi, "zero delay.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::DelayL");
  gwinote(gwi, "Default constructor creates a delay-line with maximum length of 4095 samples and");
  gwinote(gwi, "zero delay.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the delay parameter is less than zero, the maximum");
  gwinote(gwi, "delay parameter is less than one, or the delay parameter is greater than the");
  gwinote(gwi, "maxDelay value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_ctor2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::getMaximumDelay");
  gwinote(gwi, "Get the maximum delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "getMaximumDelay"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_getMaximumDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::setMaximumDelay");
  gwinote(gwi, "Set the maximum delay-line length.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method should generally only be used during initial setup of the delay");
  gwinote(gwi, "line. If it is used between calls to the tick() function, without a call to");
  gwinote(gwi, "clear(), a signal discontinuity will likely occur. If the current maximum length");
  gwinote(gwi, "is greater than the new length, no memory allocation change is made.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setMaximumDelay"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_setMaximumDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::setDelay");
  gwinote(gwi, "Set the delay-line length.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The valid range for *delay* is from 0 to the maximum delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDelay"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_setDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::getDelay");
  gwinote(gwi, "Return the current delay-line length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getDelay"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_getDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::tapOut");
  gwinote(gwi, "Return the value at *tapDelay* samples from the delay-line input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The tap point is determined modulo the delay-line length and is relative to the");
  gwinote(gwi, "last input value (i.e., a tapDelay of zero returns the last input value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "tapOut"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_tapOut, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::tapIn");
  gwinote(gwi, "Set the *value* at *tapDelay* samples from the delay-line input.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "tapIn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_tapIn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::DelayL::nextOut");
  gwinote(gwi, "Return the value which will be output by the next call to tick().  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method is valid only for delay settings greater than zero!  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "nextOut"));
  GWI_B(gwi_func_end(gwi, gw_DelayL_nextOut, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // DelayL

  GWI_B(gwi_func_ini(gwi, "int", "DRUM_NUMWAVES"));
  GWI_B(gwi_func_end(gwi, gw_DRUM_NUMWAVES_get, ae_flag_none));
  GWI_B(gwi_func_ini(gwi, "int", "DRUM_POLYPHONY"));
  GWI_B(gwi_func_end(gwi, gw_DRUM_POLYPHONY_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK drum sample player class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a drum sampling synthesizer using WvIn objects and one-");
  gwinote(gwi, "pole filters. The drum rawwave files are sampled at 22050 Hz, but will be");
  gwinote(gwi, "appropriately interpolated for other sample rates. You can specify the maximum");
  gwinote(gwi, "polyphony (maximum number of simultaneous voices) via a #define in the");
  gwinote(gwi, "Drummer.h.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Drummer.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Drummer", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Drummer_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Drummer::Drummer");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Drummer_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Drummer::noteOn");
  gwinote(gwi, "Start a note with the given drum type and amplitude.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Use general MIDI drum instrument numbers, converted to frequency values as if");
  gwinote(gwi, "MIDI note numbers, to select a particular instrument. An StkError will be thrown");
  gwinote(gwi, "if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Drummer_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Drummer::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Drummer_noteOff, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Drummer

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK echo effect class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements an echo effect.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Echo.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Echo", "stk.Effect"));
  gwi_class_xtor(gwi, NULL, gw_Echo_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Echo::Echo");
  gwinote(gwi, "Class constructor, taking the longest desired delay length (one second default");
  gwinote(gwi, "value).  ");
  gwinote(gwi, "");
  gwinote(gwi, "The default delay value is set to 1/2 the maximum delay length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Echo_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Echo::Echo");
  gwinote(gwi, "Class constructor, taking the longest desired delay length (one second default");
  gwinote(gwi, "value).  ");
  gwinote(gwi, "");
  gwinote(gwi, "The default delay value is set to 1/2 the maximum delay length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Echo_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Echo::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Echo_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Echo::setMaximumDelay");
  gwinote(gwi, "Set the maximum delay line length in samples.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setMaximumDelay"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Echo_setMaximumDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Echo::setDelay");
  gwinote(gwi, "Set the delay line length in samples.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDelay"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Echo_setDelay, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Echo

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK audio input abstract base class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class provides common functionality for a variety of audio data input");
  gwinote(gwi, "subclasses.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: WvIn.h");
  gwinote(gwi, "");
  const Type t_WvIn = gwi_class_ini(gwi, "WvIn", "stk.Stk");
  gwi_class_xtor(gwi, NULL, gw_WvIn_dtor);
  SET_FLAG(t_WvIn, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::WvIn::channelsOut");
  gwinote(gwi, "Return the number of audio channels in the data or stream.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "channelsOut"));
  GWI_B(gwi_func_end(gwi, gw_WvIn_channelsOut, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // WvIn

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK audio file input class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class inherits from WvIn. It provides a \"tick-level\" interface to the");
  gwinote(gwi, "FileRead class. It also provides variable-rate playback functionality. Audio");
  gwinote(gwi, "file support is provided by the FileRead class. Linear interpolation is used for");
  gwinote(gwi, "fractional read rates.  ");
  gwinote(gwi, "");
  gwinote(gwi, "FileWvIn supports multi-channel data. It is important to distinguish the tick()");
  gwinote(gwi, "method that computes a single frame (and returns only the specified sample of a");
  gwinote(gwi, "multi-channel frame) from the overloaded one that takes an StkFrames object for");
  gwinote(gwi, "multi-channel and/or multi-frame data.  ");
  gwinote(gwi, "");
  gwinote(gwi, "FileWvIn will either load the entire content of an audio file into local memory");
  gwinote(gwi, "or incrementally read file data from disk in chunks. This behavior is controlled");
  gwinote(gwi, "by the optional constructor arguments *chunkThreshold* and *chunkSize*. File");
  gwinote(gwi, "sizes greater than *chunkThreshold* (in sample frames) will be read");
  gwinote(gwi, "incrementally in chunks of *chunkSize* each (also in sample frames).  ");
  gwinote(gwi, "");
  gwinote(gwi, "For file data read completely into local memory, the *doNormalize* flag can be");
  gwinote(gwi, "used to normalize all values with respect to the maximum absolute value of the");
  gwinote(gwi, "data.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the file data format is fixed point, the flag *doInt2FloatScaling* can be");
  gwinote(gwi, "used to control whether the values are scaled with respect to the corresponding");
  gwinote(gwi, "fixed-point maximum. For example, if reading 16-bit signed integers, the input");
  gwinote(gwi, "values will be scaled by 1 / 32768.0. This scaling will not happen for floating-");
  gwinote(gwi, "point file data formats.  ");
  gwinote(gwi, "");
  gwinote(gwi, "When the file end is reached, subsequent calls to the tick() functions return");
  gwinote(gwi, "zeros and isFinished() returns *true*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "See the FileRead class for a description of the supported audio file formats.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: FileWvIn.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "FileWvIn", "stk.WvIn"));
  gwi_class_xtor(gwi, NULL, gw_FileWvIn_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::FileWvIn");
  gwinote(gwi, "Overloaded constructor for file input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown, or a");
  gwinote(gwi, "read error occurs.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_arg(gwi, "int", "arg5"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg6"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::FileWvIn");
  gwinote(gwi, "Overloaded constructor for file input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown, or a");
  gwinote(gwi, "read error occurs.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_arg(gwi, "int", "arg5"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::FileWvIn");
  gwinote(gwi, "Overloaded constructor for file input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown, or a");
  gwinote(gwi, "read error occurs.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_ctor2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::FileWvIn");
  gwinote(gwi, "Overloaded constructor for file input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown, or a");
  gwinote(gwi, "read error occurs.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_ctor3, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::FileWvIn");
  gwinote(gwi, "Overloaded constructor for file input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown, or a");
  gwinote(gwi, "read error occurs.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_ctor4, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::FileWvIn");
  gwinote(gwi, "Overloaded constructor for file input.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown, or a");
  gwinote(gwi, "read error occurs.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_ctor5, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::reset");
  gwinote(gwi, "Clear outputs and reset time (file) pointer to zero.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "reset"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_reset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::normalize");
  gwinote(gwi, "Normalize data to a maximum of *+-peak*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function has no effect when data is incrementally loaded from disk.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "normalize"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_normalize0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::normalize");
  gwinote(gwi, "Normalize data to a maximum of *+-peak*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function has no effect when data is incrementally loaded from disk.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "normalize"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_normalize1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::getSize");
  gwinote(gwi, "Return the file size in sample frames.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "getSize"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_getSize, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::getFileRate");
  gwinote(gwi, "Return the input file sample rate in Hz (not the data read rate).  ");
  gwinote(gwi, "");
  gwinote(gwi, "WAV, SND, and AIF formatted files specify a sample rate in their headers. STK");
  gwinote(gwi, "RAW files have a sample rate of 22050 Hz by definition. MAT-files are assumed to");
  gwinote(gwi, "have a rate of 44100 Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getFileRate"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_getFileRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::isOpen");
  gwinote(gwi, "Query whether a file is open.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "bool", "isOpen"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_isOpen, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::isFinished");
  gwinote(gwi, "Query whether reading is complete.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "bool", "isFinished"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_isFinished, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::setRate");
  gwinote(gwi, "Set the data read rate in samples. The rate can be negative.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the rate value is negative, the data is read in reverse order.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_setRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::addTime");
  gwinote(gwi, "Increment the read pointer by *time* samples.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Note that this function will not modify the interpolation flag status.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "addTime"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_addTime, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FileWvIn::setInterpolate");
  gwinote(gwi, "Turn linear interpolation on/off.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Interpolation is automatically off when the read rate is an integer value. If");
  gwinote(gwi, "interpolation is turned off for a fractional rate, the time index is truncated");
  gwinote(gwi, "to an integer value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setInterpolate"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FileWvIn_setInterpolate, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // FileWvIn

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK flute physical model class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple flute physical model, as discussed by");
  gwinote(gwi, "Karjalainen, Smith, Waryznyk, etc. The jet model uses a polynomial, a la Cook.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Jet Delay = 2  ");
  gwinote(gwi, "*   Noise Gain = 4  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Breath Pressure = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Flute.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Flute", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Flute_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::Flute");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Flute_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Flute_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Flute_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::setJetReflection");
  gwinote(gwi, "Set the reflection coefficient for the jet delay (-1.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setJetReflection"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Flute_setJetReflection, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::setEndReflection");
  gwinote(gwi, "Set the reflection coefficient for the air column delay (-1.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setEndReflection"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Flute_setEndReflection, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::setJetDelay");
  gwinote(gwi, "Set the length of the jet delay in terms of a ratio of jet delay to air column");
  gwinote(gwi, "delay lengths.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setJetDelay"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Flute_setJetDelay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::startBlowing");
  gwinote(gwi, "Apply breath velocity to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Flute_startBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::stopBlowing");
  gwinote(gwi, "Decrease breath velocity with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Flute_stopBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Flute_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Flute_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Flute::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Flute_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Flute

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK singing FM synthesis instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements 3 carriers and a common modulator, also referred to as");
  gwinote(gwi, "algorithm 6 of the TX81Z.  ");
  gwinote(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Vowel = 2  ");
  gwinote(gwi, "*   Spectral Tilt = 4  ");
  gwinote(gwi, "*   LFO Speed = 11  ");
  gwinote(gwi, "*   LFO Depth = 1  ");
  gwinote(gwi, "*   ADSR 2 & 4 Target = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "The basic Chowning/Stanford FM patent expired in 1995, but there exist follow-on");
  gwinote(gwi, "patents, mostly assigned to Yamaha. If you are of the type who should worry");
  gwinote(gwi, "about this (making money) worry away.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: FMVoices.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "FMVoices", "stk.FM"));
  gwi_class_xtor(gwi, NULL, gw_FMVoices_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FMVoices::FMVoices");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_FMVoices_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FMVoices::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FMVoices_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FMVoices::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FMVoices_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FMVoices::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FMVoices_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // FMVoices

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK sweepable formant filter class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a formant (resonance) which can be \"swept\" over time");
  gwinote(gwi, "from one frequency setting to another. It provides methods for controlling the");
  gwinote(gwi, "sweep rate and target frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: FormSwep.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "FormSwep", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_FormSwep_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::FormSwep");
  gwinote(gwi, "Default constructor creates a second-order pass-through filter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_ignoreSampleRateChange0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_ignoreSampleRateChange1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::setResonance");
  gwinote(gwi, "Sets the filter coefficients for a resonance at *frequency* (in Hz).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to two complex-");
  gwinote(gwi, "conjugate poles with the given *frequency* (in Hz) and *radius* from the z-plane");
  gwinote(gwi, "origin. The filter zeros are placed at z = 1, z = -1, and the coefficients are");
  gwinote(gwi, "then normalized to produce a constant unity gain (independent of the filter");
  gwinote(gwi, "*gain* parameter). The resulting filter frequency response has a resonance at");
  gwinote(gwi, "the given *frequency*. The closer the poles are to the unit-circle (*radius*");
  gwinote(gwi, "close to one), the narrower the resulting resonance width. An unstable filter");
  gwinote(gwi, "will result for *radius* >= 1.0. The *frequency* value should be between zero");
  gwinote(gwi, "and half the sample rate.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setResonance"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_setResonance, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::setStates");
  gwinote(gwi, "Set both the current and target resonance parameters.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setStates"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_setStates0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::setStates");
  gwinote(gwi, "Set both the current and target resonance parameters.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setStates"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_setStates1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::setTargets");
  gwinote(gwi, "Set target resonance parameters.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTargets"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_setTargets0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::setTargets");
  gwinote(gwi, "Set target resonance parameters.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setTargets"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_setTargets1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::setSweepRate");
  gwinote(gwi, "Set the sweep rate (between 0.0 - 1.0).  ");
  gwinote(gwi, "");
  gwinote(gwi, "The formant parameters are varied in increments of the sweep rate between their");
  gwinote(gwi, "current and target values. A sweep rate of 1.0 will produce an immediate change");
  gwinote(gwi, "in resonance parameters from their current values to the target values. A sweep");
  gwinote(gwi, "rate of 0.0 will produce no change in resonance parameters.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSweepRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_setSweepRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FormSwep::setSweepTime");
  gwinote(gwi, "Set the sweep rate in terms of a time value in seconds.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method adjusts the sweep rate based on a given time for the formant");
  gwinote(gwi, "parameters to reach their target values.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSweepTime"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FormSwep_setSweepTime, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // FormSwep

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Jezar at Dreampoint's FreeVerb, implemented in STK.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Freeverb is a free and open-source Schroeder reverberator originally implemented");
  gwinote(gwi, "in C++. The parameters of the reverberation model are exceptionally well tuned.");
  gwinote(gwi, "FreeVerb uses 8 lowpass-feedback-comb-filters in parallel, followed by 4");
  gwinote(gwi, "Schroeder allpass filters in series. The input signal can be either mono or");
  gwinote(gwi, "stereo, and the output signal is stereo. The delay lengths are optimized for a");
  gwinote(gwi, "sample rate of 44100 Hz.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Ported to STK by Gregory Burlet, 2012.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: FreeVerb.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "FreeVerb", "stk.Effect"));
  gwi_class_xtor(gwi, NULL, gw_FreeVerb_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::FreeVerb");
  gwinote(gwi, "FreeVerb Constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Initializes the effect with default parameters. Note that these defaults are");
  gwinote(gwi, "slightly different than those in the original implementation of FreeVerb [Effect");
  gwinote(gwi, "Mix: 0.75; Room Size: 0.75; Damping: 0.25; Width: 1.0; Mode: freeze mode off].  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::setEffectMix");
  gwinote(gwi, "Set the effect mix [0 = mostly dry, 1 = mostly wet].  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setEffectMix"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_setEffectMix, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::setRoomSize");
  gwinote(gwi, "Set the room size (comb filter feedback gain) parameter [0,1].  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRoomSize"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_setRoomSize, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::getRoomSize");
  gwinote(gwi, "Get the room size (comb filter feedback gain) parameter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getRoomSize"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_getRoomSize, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::setDamping");
  gwinote(gwi, "Set the damping parameter [0=low damping, 1=higher damping].  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDamping"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_setDamping, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::getDamping");
  gwinote(gwi, "Get the damping parameter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getDamping"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_getDamping, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::setWidth");
  gwinote(gwi, "Set the width (left-right mixing) parameter [0,1].  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setWidth"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_setWidth, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::getWidth");
  gwinote(gwi, "Get the width (left-right mixing) parameter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getWidth"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_getWidth, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::setMode");
  gwinote(gwi, "Set the mode [frozen = 1, unfrozen = 0].  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setMode"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_setMode, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::getMode");
  gwinote(gwi, "Get the current freeze mode [frozen = 1, unfrozen = 0].  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getMode"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_getMode, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::FreeVerb::clear");
  gwinote(gwi, "Clears delay lines, etc.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_FreeVerb_clear, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // FreeVerb

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK granular synthesis class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a real-time granular synthesis algorithm that operates on");
  gwinote(gwi, "an input soundfile. Multi-channel files are supported. Various functions are");
  gwinote(gwi, "provided to allow control over voice and grain parameters.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The functionality of this class is based on the program MacPod by Chris Rolfe");
  gwinote(gwi, "and Damian Keller, though there are likely to be a number of differences in the");
  gwinote(gwi, "actual implementation.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Gary Scavone, 2005--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Granulate.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Granulate", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_Granulate_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::Granulate");
  gwinote(gwi, "Constructor taking input audio file and number of voices arguments.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::Granulate");
  gwinote(gwi, "Constructor taking input audio file and number of voices arguments.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_arg(gwi, "string", "arg2"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::Granulate");
  gwinote(gwi, "Constructor taking input audio file and number of voices arguments.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_arg(gwi, "string", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_ctor2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::openFile");
  gwinote(gwi, "Load a monophonic soundfile to be \"granulated\".  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown or");
  gwinote(gwi, "unsupported, or the file has more than one channel.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "openFile"));
  GWI_B(gwi_func_arg(gwi, "string", "arg2"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_openFile0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::openFile");
  gwinote(gwi, "Load a monophonic soundfile to be \"granulated\".  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown or");
  gwinote(gwi, "unsupported, or the file has more than one channel.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "openFile"));
  GWI_B(gwi_func_arg(gwi, "string", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_openFile1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::reset");
  gwinote(gwi, "Reset the file pointer and all existing grains to the file start.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Multiple grains are offset from one another in time by grain duration / nVoices.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "reset"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_reset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setVoices");
  gwinote(gwi, "Set the number of simultaneous grain \"voices\" to use.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Multiple grains are offset from one another in time by grain duration / nVoices.");
  gwinote(gwi, "For this reason, it is best to set the grain parameters before calling this");
  gwinote(gwi, "function (during initialization).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVoices"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setVoices0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setVoices");
  gwinote(gwi, "Set the number of simultaneous grain \"voices\" to use.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Multiple grains are offset from one another in time by grain duration / nVoices.");
  gwinote(gwi, "For this reason, it is best to set the grain parameters before calling this");
  gwinote(gwi, "function (during initialization).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVoices"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setVoices1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setStretch");
  gwinote(gwi, "Set the stretch factor used for grain playback (1 - 1000).  ");
  gwinote(gwi, "");
  gwinote(gwi, "Granular synthesis allows for time-stetching without affecting the original");
  gwinote(gwi, "pitch of a sound. A stretch factor of 4 will produce a resulting sound of length");
  gwinote(gwi, "4 times the orignal sound. The default parameter of 1 produces no stretching.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setStretch"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setStretch0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setStretch");
  gwinote(gwi, "Set the stretch factor used for grain playback (1 - 1000).  ");
  gwinote(gwi, "");
  gwinote(gwi, "Granular synthesis allows for time-stetching without affecting the original");
  gwinote(gwi, "pitch of a sound. A stretch factor of 4 will produce a resulting sound of length");
  gwinote(gwi, "4 times the orignal sound. The default parameter of 1 produces no stretching.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setStretch"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setStretch1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setGrainParameters");
  gwinote(gwi, "Set global grain parameters used to determine individual grain settings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Each grain is defined as having a length of *duration* milliseconds which must");
  gwinote(gwi, "be greater than zero. For values of *rampPercent* (0 - 100) greater than zero, a");
  gwinote(gwi, "linear envelope will be applied to each grain. If *rampPercent* = 100, the");
  gwinote(gwi, "resultant grain \"window\" is triangular while *rampPercent* = 50 produces a");
  gwinote(gwi, "trapezoidal window. In addition, each grain can have a time delay of length");
  gwinote(gwi, "*delay* and a grain pointer increment of length *offset*, which can be negative,");
  gwinote(gwi, "before the next ramp onset (in milliseconds). The *offset* parameter controls");
  gwinote(gwi, "grain pointer jumps between enveloped grain segments, while the *delay*");
  gwinote(gwi, "parameter causes grain calculations to pause between grains. The actual values");
  gwinote(gwi, "calculated for each grain will be randomized by a factor set using the");
  gwinote(gwi, "setRandomFactor() function.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGrainParameters"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_arg(gwi, "int", "arg5"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setGrainParameters0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setGrainParameters");
  gwinote(gwi, "Set global grain parameters used to determine individual grain settings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Each grain is defined as having a length of *duration* milliseconds which must");
  gwinote(gwi, "be greater than zero. For values of *rampPercent* (0 - 100) greater than zero, a");
  gwinote(gwi, "linear envelope will be applied to each grain. If *rampPercent* = 100, the");
  gwinote(gwi, "resultant grain \"window\" is triangular while *rampPercent* = 50 produces a");
  gwinote(gwi, "trapezoidal window. In addition, each grain can have a time delay of length");
  gwinote(gwi, "*delay* and a grain pointer increment of length *offset*, which can be negative,");
  gwinote(gwi, "before the next ramp onset (in milliseconds). The *offset* parameter controls");
  gwinote(gwi, "grain pointer jumps between enveloped grain segments, while the *delay*");
  gwinote(gwi, "parameter causes grain calculations to pause between grains. The actual values");
  gwinote(gwi, "calculated for each grain will be randomized by a factor set using the");
  gwinote(gwi, "setRandomFactor() function.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGrainParameters"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setGrainParameters1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setGrainParameters");
  gwinote(gwi, "Set global grain parameters used to determine individual grain settings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Each grain is defined as having a length of *duration* milliseconds which must");
  gwinote(gwi, "be greater than zero. For values of *rampPercent* (0 - 100) greater than zero, a");
  gwinote(gwi, "linear envelope will be applied to each grain. If *rampPercent* = 100, the");
  gwinote(gwi, "resultant grain \"window\" is triangular while *rampPercent* = 50 produces a");
  gwinote(gwi, "trapezoidal window. In addition, each grain can have a time delay of length");
  gwinote(gwi, "*delay* and a grain pointer increment of length *offset*, which can be negative,");
  gwinote(gwi, "before the next ramp onset (in milliseconds). The *offset* parameter controls");
  gwinote(gwi, "grain pointer jumps between enveloped grain segments, while the *delay*");
  gwinote(gwi, "parameter causes grain calculations to pause between grains. The actual values");
  gwinote(gwi, "calculated for each grain will be randomized by a factor set using the");
  gwinote(gwi, "setRandomFactor() function.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGrainParameters"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setGrainParameters2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setGrainParameters");
  gwinote(gwi, "Set global grain parameters used to determine individual grain settings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Each grain is defined as having a length of *duration* milliseconds which must");
  gwinote(gwi, "be greater than zero. For values of *rampPercent* (0 - 100) greater than zero, a");
  gwinote(gwi, "linear envelope will be applied to each grain. If *rampPercent* = 100, the");
  gwinote(gwi, "resultant grain \"window\" is triangular while *rampPercent* = 50 produces a");
  gwinote(gwi, "trapezoidal window. In addition, each grain can have a time delay of length");
  gwinote(gwi, "*delay* and a grain pointer increment of length *offset*, which can be negative,");
  gwinote(gwi, "before the next ramp onset (in milliseconds). The *offset* parameter controls");
  gwinote(gwi, "grain pointer jumps between enveloped grain segments, while the *delay*");
  gwinote(gwi, "parameter causes grain calculations to pause between grains. The actual values");
  gwinote(gwi, "calculated for each grain will be randomized by a factor set using the");
  gwinote(gwi, "setRandomFactor() function.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGrainParameters"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setGrainParameters3, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setGrainParameters");
  gwinote(gwi, "Set global grain parameters used to determine individual grain settings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Each grain is defined as having a length of *duration* milliseconds which must");
  gwinote(gwi, "be greater than zero. For values of *rampPercent* (0 - 100) greater than zero, a");
  gwinote(gwi, "linear envelope will be applied to each grain. If *rampPercent* = 100, the");
  gwinote(gwi, "resultant grain \"window\" is triangular while *rampPercent* = 50 produces a");
  gwinote(gwi, "trapezoidal window. In addition, each grain can have a time delay of length");
  gwinote(gwi, "*delay* and a grain pointer increment of length *offset*, which can be negative,");
  gwinote(gwi, "before the next ramp onset (in milliseconds). The *offset* parameter controls");
  gwinote(gwi, "grain pointer jumps between enveloped grain segments, while the *delay*");
  gwinote(gwi, "parameter causes grain calculations to pause between grains. The actual values");
  gwinote(gwi, "calculated for each grain will be randomized by a factor set using the");
  gwinote(gwi, "setRandomFactor() function.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGrainParameters"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setGrainParameters4, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setRandomFactor");
  gwinote(gwi, "This factor is used when setting individual grain parameters (0.0 - 1.0).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This random factor is applied when all grain state durations are calculated. If");
  gwinote(gwi, "set to 0.0, no randomness occurs. When randomness = 1.0, a grain segment of");
  gwinote(gwi, "length *duration* will be randomly augmented by up to +- *duration* seconds");
  gwinote(gwi, "(i.e., a 30 millisecond length will be augmented by an extra length of up to +30");
  gwinote(gwi, "or -30 milliseconds).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRandomFactor"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setRandomFactor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Granulate::setRandomFactor");
  gwinote(gwi, "This factor is used when setting individual grain parameters (0.0 - 1.0).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This random factor is applied when all grain state durations are calculated. If");
  gwinote(gwi, "set to 0.0, no randomness occurs. When randomness = 1.0, a grain segment of");
  gwinote(gwi, "length *duration* will be randomly augmented by up to +- *duration* seconds");
  gwinote(gwi, "(i.e., a 30 millisecond length will be augmented by an extra length of up to +30");
  gwinote(gwi, "or -30 milliseconds).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRandomFactor"));
  GWI_B(gwi_func_end(gwi, gw_Granulate_setRandomFactor1, ae_flag_none));
  GWI_B(gwi_enum_ini(gwi, (m_str)"GrainState"));
  GWI_B(gwi_enum_add(gwi, (m_str)"GRAIN_STOPPED", (m_uint)stk::Granulate::GRAIN_STOPPED));
  GWI_B(gwi_enum_add(gwi, (m_str)"GRAIN_FADEIN", (m_uint)stk::Granulate::GRAIN_FADEIN));
  GWI_B(gwi_enum_add(gwi, (m_str)"GRAIN_SUSTAIN", (m_uint)stk::Granulate::GRAIN_SUSTAIN));
  GWI_B(gwi_enum_add(gwi, (m_str)"GRAIN_FADEOUT", (m_uint)stk::Granulate::GRAIN_FADEOUT));
  GWI_B(gwi_enum_end(gwi));
  GWI_B(gwi_class_end(gwi)); // Granulate

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK guitar model class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a guitar model with an arbitrary number of strings");
  gwinote(gwi, "(specified during instantiation). Each string is represented by an stk::Twang");
  gwinote(gwi, "object. The model supports commuted synthesis, as discussed by Smith and");
  gwinote(gwi, "Karjalainen. It also includes a basic body coupling model and supports feedback.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class does not attempt voice management. Rather, most functions support a");
  gwinote(gwi, "parameter to specify a particular string number and string (voice) management is");
  gwinote(gwi, "assumed to occur externally. Note that this class does not inherit from");
  gwinote(gwi, "stk::Instrmnt because of API inconsistencies.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Bridge Coupling Gain = 2  ");
  gwinote(gwi, "*   Pluck Position = 4  ");
  gwinote(gwi, "*   Loop Gain = 11  ");
  gwinote(gwi, "*   Coupling Filter Pole = 1  ");
  gwinote(gwi, "*   Pick Filter Pole = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Gary P. Scavone, 2012.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Guitar.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Guitar", "stk.Stk"));
  gwi_class_xtor(gwi, NULL, gw_Guitar_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::Guitar");
  gwinote(gwi, "Class constructor, specifying an arbitrary number of strings (default = 6).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_arg(gwi, "string", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::Guitar");
  gwinote(gwi, "Class constructor, specifying an arbitrary number of strings (default = 6).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::Guitar");
  gwinote(gwi, "Class constructor, specifying an arbitrary number of strings (default = 6).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_ctor2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::setBodyFile");
  gwinote(gwi, "Set the string excitation, using either a soundfile or computed noise.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If no argument is provided, the std::string is empty, or an error occurs reading");
  gwinote(gwi, "the file data, an enveloped noise signal will be generated for use as the pluck");
  gwinote(gwi, "excitation.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBodyFile"));
  GWI_B(gwi_func_arg(gwi, "string", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_setBodyFile0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::setBodyFile");
  gwinote(gwi, "Set the string excitation, using either a soundfile or computed noise.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If no argument is provided, the std::string is empty, or an error occurs reading");
  gwinote(gwi, "the file data, an enveloped noise signal will be generated for use as the pluck");
  gwinote(gwi, "excitation.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBodyFile"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_setBodyFile1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::setPluckPosition");
  gwinote(gwi, "Set the pluck position for one or all strings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the `string` argument is < 0, the pluck position is set for all strings.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPluckPosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_setPluckPosition0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::setPluckPosition");
  gwinote(gwi, "Set the pluck position for one or all strings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the `string` argument is < 0, the pluck position is set for all strings.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPluckPosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_setPluckPosition1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::setLoopGain");
  gwinote(gwi, "Set the loop gain for one or all strings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the `string` argument is < 0, the loop gain is set for all strings.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setLoopGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_setLoopGain0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::setLoopGain");
  gwinote(gwi, "Set the loop gain for one or all strings.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the `string` argument is < 0, the loop gain is set for all strings.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setLoopGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_setLoopGain1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_setFrequency0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_setFrequency1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the `amplitude` parameter is less than 0.2, the string will be undamped but");
  gwinote(gwi, "it will not be \"plucked.\"  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_noteOn0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the `amplitude` parameter is less than 0.2, the string will be undamped but");
  gwinote(gwi, "it will not be \"plucked.\"  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_noteOn1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_noteOff0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_noteOff1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the `string` argument is < 0, then the control change is applied to all");
  gwinote(gwi, "strings (if appropriate).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_controlChange0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Guitar::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the `string` argument is < 0, then the control change is applied to all");
  gwinote(gwi, "strings (if appropriate).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Guitar_controlChange1, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Guitar

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK heavy metal FM synthesis instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements 3 cascade operators with feedback modulation, also");
  gwinote(gwi, "referred to as algorithm 3 of the TX81Z.  ");
  gwinote(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Total Modulator Index = 2  ");
  gwinote(gwi, "*   Modulator Crossfade = 4  ");
  gwinote(gwi, "*   LFO Speed = 11  ");
  gwinote(gwi, "*   LFO Depth = 1  ");
  gwinote(gwi, "*   ADSR 2 & 4 Target = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "The basic Chowning/Stanford FM patent expired in 1995, but there exist follow-on");
  gwinote(gwi, "patents, mostly assigned to Yamaha. If you are of the type who should worry");
  gwinote(gwi, "about this (making money) worry away.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: HevyMetl.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "HevyMetl", "stk.FM"));
  gwi_class_xtor(gwi, NULL, gw_HevyMetl_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::HevyMetl::HevyMetl");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_HevyMetl_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::HevyMetl::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_HevyMetl_noteOn, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // HevyMetl

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "John Chowning's reverberator class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class takes a monophonic input signal and produces a stereo output signal.");
  gwinote(gwi, "It is derived from the CLM JCRev function, which is based on the use of networks");
  gwinote(gwi, "of simple allpass and comb delay filters. This class implements three series");
  gwinote(gwi, "allpass units, followed by four parallel comb filters, and two decorrelation");
  gwinote(gwi, "delay lines in parallel at the output.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Although not in the original JC reverberator, one-pole lowpass filters have been");
  gwinote(gwi, "added inside the feedback comb filters.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: JCRev.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "JCRev", "stk.Effect"));
  gwi_class_xtor(gwi, NULL, gw_JCRev_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::JCRev::JCRev");
  gwinote(gwi, "Class constructor taking a T60 decay time argument (one second default value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_JCRev_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::JCRev::JCRev");
  gwinote(gwi, "Class constructor taking a T60 decay time argument (one second default value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_JCRev_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::JCRev::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_JCRev_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::JCRev::setT60");
  gwinote(gwi, "Set the reverberation T60 decay time.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setT60"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_JCRev_setT60, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // JCRev

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK jet table class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a flue jet non-linear function, computed by a polynomial");
  gwinote(gwi, "calculation. Contrary to the name, this is not a \"table\".  ");
  gwinote(gwi, "");
  gwinote(gwi, "Consult Fletcher and Rossing, Karjalainen, Cook, and others for more");
  gwinote(gwi, "information.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: JetTable.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "JetTable", "stk.Function"));
  gwi_class_xtor(gwi, NULL, gw_JetTable_dtor);
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_JetTable_ctor, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // JetTable

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Pitch shifter effect class based on the Lent algorithm.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a pitch shifter using pitch tracking and sample windowing");
  gwinote(gwi, "and shifting.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Francois Germain, 2009.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: LentPitShift.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "LentPitShift", "stk.Effect"));
  gwi_class_xtor(gwi, NULL, gw_LentPitShift_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::LentPitShift::LentPitShift");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_LentPitShift_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::LentPitShift::LentPitShift");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_LentPitShift_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::LentPitShift::LentPitShift");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_LentPitShift_ctor2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::LentPitShift::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_LentPitShift_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::LentPitShift::setShift");
  gwinote(gwi, "Set the pitch shift factor (1.0 produces no shift).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setShift"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_LentPitShift_setShift, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // LentPitShift

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK mandolin instrument model class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class uses two \"twang\" models and \"commuted");
  gwinote(gwi, "synthesis\" techniques to model a mandolin instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others. Commuted Synthesis, in");
  gwinote(gwi, "particular, is covered by patents, granted, pending, and/or applied-for. All are");
  gwinote(gwi, "assigned to the Board of Trustees, Stanford University. For information, contact");
  gwinote(gwi, "the Office of Technology Licensing, Stanford University.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Body Size = 2  ");
  gwinote(gwi, "*   Pluck Position = 4  ");
  gwinote(gwi, "*   String Sustain = 11  ");
  gwinote(gwi, "*   String Detuning = 1  ");
  gwinote(gwi, "*   Microphone Position = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Mandolin.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Mandolin", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Mandolin_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::Mandolin");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::setDetune");
  gwinote(gwi, "Detune the two strings by the given factor. A value of 1.0 produces unison");
  gwinote(gwi, "strings.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDetune"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_setDetune, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::setBodySize");
  gwinote(gwi, "Set the body size (a value of 1.0 produces the \"default\" size).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBodySize"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_setBodySize, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::setPluckPosition");
  gwinote(gwi, "Set the pluck or \"excitation\" position along the string (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPluckPosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_setPluckPosition, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::pluck");
  gwinote(gwi, "Pluck the strings with the given amplitude (0.0 - 1.0) and position (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pluck"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_pluck0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::pluck");
  gwinote(gwi, "Pluck the strings with the given amplitude (0.0 - 1.0) and position (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pluck"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_pluck1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mandolin::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Mandolin_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Mandolin

  GWI_B(gwi_func_ini(gwi, "int", "NXMAX"));
  GWI_B(gwi_func_end(gwi, gw_NXMAX_get, ae_flag_none));
  GWI_B(gwi_func_ini(gwi, "int", "NYMAX"));
  GWI_B(gwi_func_end(gwi, gw_NYMAX_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Two-dimensional rectilinear waveguide mesh class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a rectilinear, two-dimensional digital waveguide mesh");
  gwinote(gwi, "structure. For details, see Van Duyne and Smith, \"Physical Modeling with the");
  gwinote(gwi, "2-D Digital");
  gwinote(gwi, "Waveguide Mesh\", Proceedings of the 1993 International Computer Music");
  gwinote(gwi, "Conference.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   X Dimension = 2  ");
  gwinote(gwi, "*   Y Dimension = 4  ");
  gwinote(gwi, "*   Mesh Decay = 11  ");
  gwinote(gwi, "*   X-Y Input Position = 1  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Julius Smith, 2000--2002. Revised by Gary Scavone for STK, 2002.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Mesh2D.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Mesh2D", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Mesh2D_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::Mesh2D");
  gwinote(gwi, "Class constructor, taking the x and y dimensions in samples.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::setNX");
  gwinote(gwi, "Set the x dimension size in samples.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setNX"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_setNX, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::setNY");
  gwinote(gwi, "Set the y dimension size in samples.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setNY"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_setNY, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::setInputPosition");
  gwinote(gwi, "Set the x, y input position on a 0.0 - 1.0 scale.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setInputPosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_setInputPosition, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::setDecay");
  gwinote(gwi, "Set the loss filters gains (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDecay"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_setDecay, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::noteOn");
  gwinote(gwi, "Impulse the mesh with the given amplitude (frequency ignored).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay) ... currently ignored.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::energy");
  gwinote(gwi, "Calculate and return the signal energy stored in the mesh.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "energy"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_energy, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::inputTick");
  gwinote(gwi, "Input a sample to the mesh and compute one output sample.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "inputTick"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_inputTick, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Mesh2D::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Mesh2D_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Mesh2D

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK resonance model abstract base class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class contains an excitation wavetable, an envelope, an oscillator, and N");
  gwinote(gwi, "resonances (non-sweeping BiQuad filters), where N is set during instantiation.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Modal.h");
  gwinote(gwi, "");
  const Type t_Modal = gwi_class_ini(gwi, "Modal", "stk.Instrmnt");
  gwi_class_xtor(gwi, NULL, gw_Modal_dtor);
  SET_FLAG(t_Modal, abstract);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Modal_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modal_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::setRatioAndRadius");
  gwinote(gwi, "Set the ratio and radius for a specified mode filter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRatioAndRadius"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Modal_setRatioAndRadius, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::setMasterGain");
  gwinote(gwi, "Set the master gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setMasterGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modal_setMasterGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::setDirectGain");
  gwinote(gwi, "Set the direct gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setDirectGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modal_setDirectGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::setModeGain");
  gwinote(gwi, "Set the gain for a specified mode filter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setModeGain"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Modal_setModeGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::strike");
  gwinote(gwi, "Initiate a strike with the given amplitude (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "strike"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modal_strike, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::damp");
  gwinote(gwi, "Damp modes with a given decay factor (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "damp"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modal_damp, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Modal_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modal_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modal::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Modal_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Modal

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK resonant bar instrument class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a number of different struck bar instruments. It inherits");
  gwinote(gwi, "from the Modal class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Stick Hardness = 2  ");
  gwinote(gwi, "*   Stick Position = 4  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Direct Stick Mix = 8  ");
  gwinote(gwi, "*   Volume = 128  ");
  gwinote(gwi, "*   Modal Presets = 16");
  gwinote(gwi, "    -   Marimba = 0  ");
  gwinote(gwi, "    -   Vibraphone = 1  ");
  gwinote(gwi, "    -   Agogo = 2  ");
  gwinote(gwi, "    -   Wood1 = 3  ");
  gwinote(gwi, "    -   Reso = 4  ");
  gwinote(gwi, "    -   Wood2 = 5  ");
  gwinote(gwi, "    -   Beats = 6  ");
  gwinote(gwi, "    -   Two Fixed = 7  ");
  gwinote(gwi, "    -   Clump = 8  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: ModalBar.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "ModalBar", "stk.Modal"));
  gwi_class_xtor(gwi, NULL, gw_ModalBar_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ModalBar::ModalBar");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_ModalBar_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ModalBar::setStickHardness");
  gwinote(gwi, "Set stick hardness (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setStickHardness"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ModalBar_setStickHardness, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ModalBar::setStrikePosition");
  gwinote(gwi, "Set stick position (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setStrikePosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ModalBar_setStrikePosition, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ModalBar::setPreset");
  gwinote(gwi, "Select a bar preset (currently modulo 9).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPreset"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ModalBar_setPreset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ModalBar::setModulationDepth");
  gwinote(gwi, "Set the modulation (vibrato) depth (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setModulationDepth"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ModalBar_setModulationDepth, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ModalBar::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_ModalBar_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // ModalBar

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK periodic/random modulator.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class combines random and periodic modulations to give a nice, natural");
  gwinote(gwi, "human modulation function.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Modulate.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Modulate", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_Modulate_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modulate::Modulate");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError can be thrown if the rawwave path is incorrect.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Modulate_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modulate::reset");
  gwinote(gwi, "Reset internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "reset"));
  GWI_B(gwi_func_end(gwi, gw_Modulate_reset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modulate::setVibratoRate");
  gwinote(gwi, "Set the periodic (vibrato) rate or frequency in Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVibratoRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modulate_setVibratoRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modulate::setVibratoGain");
  gwinote(gwi, "Set the periodic (vibrato) gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVibratoGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modulate_setVibratoGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modulate::setRandomRate");
  gwinote(gwi, "Set the periodic (vibrato) rate or frequency in Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRandomRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modulate_setRandomRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Modulate::setRandomGain");
  gwinote(gwi, "Set the random modulation gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRandomGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Modulate_setRandomGain, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Modulate

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK moog-like swept filter sampling synthesis class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This instrument uses one attack wave, one looped wave, and an ADSR envelope");
  gwinote(gwi, "(inherited from the Sampler class) and adds two sweepable formant (FormSwep)");
  gwinote(gwi, "filters.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Filter Q = 2  ");
  gwinote(gwi, "*   Filter Sweep Rate = 4  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Gain = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Moog.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Moog", "stk.Sampler"));
  gwi_class_xtor(gwi, NULL, gw_Moog_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Moog::Moog");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Moog_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Moog::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Moog_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Moog::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Moog_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Moog::setModulationSpeed");
  gwinote(gwi, "Set the modulation (vibrato) speed in Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setModulationSpeed"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Moog_setModulationSpeed, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Moog::setModulationDepth");
  gwinote(gwi, "Set the modulation (vibrato) depth.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setModulationDepth"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Moog_setModulationDepth, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Moog::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Moog_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Moog

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK noise generator.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Generic random number generation using the C rand() function. The quality of the");
  gwinote(gwi, "rand() function varies from one OS to another.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Noise.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Noise", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_Noise_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Noise::Noise");
  gwinote(gwi, "Default constructor that can also take a specific seed value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the seed value is zero (the default value), the random number generator is");
  gwinote(gwi, "seeded with the system time.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Noise_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Noise::Noise");
  gwinote(gwi, "Default constructor that can also take a specific seed value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the seed value is zero (the default value), the random number generator is");
  gwinote(gwi, "seeded with the system time.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Noise_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Noise::setSeed");
  gwinote(gwi, "Seed the random number generator with a specific seed value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If no seed is provided or the seed value is zero, the random number generator is");
  gwinote(gwi, "seeded with the current system time.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSeed"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Noise_setSeed0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Noise::setSeed");
  gwinote(gwi, "Seed the random number generator with a specific seed value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If no seed is provided or the seed value is zero, the random number generator is");
  gwinote(gwi, "seeded with the current system time.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSeed"));
  GWI_B(gwi_func_end(gwi, gw_Noise_setSeed1, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Noise

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "CCRMA's NRev reverberator class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class takes a monophonic input signal and produces a stereo output signal.");
  gwinote(gwi, "It is derived from the CLM NRev function, which is based on the use of networks");
  gwinote(gwi, "of simple allpass and comb delay filters. This particular arrangement consists");
  gwinote(gwi, "of 6 comb filters in parallel, followed by 3 allpass filters, a lowpass filter,");
  gwinote(gwi, "and another allpass in series, followed by two allpass filters in parallel with");
  gwinote(gwi, "corresponding right and left outputs.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: NRev.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "NRev", "stk.Effect"));
  gwi_class_xtor(gwi, NULL, gw_NRev_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::NRev::NRev");
  gwinote(gwi, "Class constructor taking a T60 decay time argument (one second default value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_NRev_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::NRev::NRev");
  gwinote(gwi, "Class constructor taking a T60 decay time argument (one second default value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_NRev_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::NRev::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_NRev_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::NRev::setT60");
  gwinote(gwi, "Set the reverberation T60 decay time.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setT60"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_NRev_setT60, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // NRev

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK one-pole filter class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a one-pole digital filter. A method is provided for");
  gwinote(gwi, "setting the pole position along the real axis of the z-plane while maintaining a");
  gwinote(gwi, "constant peak filter gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: OnePole.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "OnePole", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_OnePole_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OnePole::OnePole");
  gwinote(gwi, "The default constructor creates a low-pass filter (pole at z = 0.9).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_OnePole_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OnePole::OnePole");
  gwinote(gwi, "The default constructor creates a low-pass filter (pole at z = 0.9).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_OnePole_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OnePole::setB0");
  gwinote(gwi, "Set the b[0] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB0"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_OnePole_setB0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OnePole::setA1");
  gwinote(gwi, "Set the a[1] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_OnePole_setA1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OnePole::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_OnePole_setCoefficients0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OnePole::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_OnePole_setCoefficients1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OnePole::setPole");
  gwinote(gwi, "Set the pole position in the z-plane.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method sets the pole position along the real-axis of the z-plane and");
  gwinote(gwi, "normalizes the coefficients for a maximum gain of one. A positive pole value");
  gwinote(gwi, "produces a low-pass filter, while a negative pole value produces a high-pass");
  gwinote(gwi, "filter. This method does not affect the filter *gain* value. The argument");
  gwinote(gwi, "magnitude should be less than one to maintain filter stability.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPole"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_OnePole_setPole, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // OnePole

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK one-zero filter class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a one-zero digital filter. A method is provided for");
  gwinote(gwi, "setting the zero position along the real axis of the z-plane while maintaining a");
  gwinote(gwi, "constant filter gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: OneZero.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "OneZero", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_OneZero_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OneZero::OneZero");
  gwinote(gwi, "The default constructor creates a low-pass filter (zero at z = -1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_OneZero_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OneZero::OneZero");
  gwinote(gwi, "The default constructor creates a low-pass filter (zero at z = -1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_OneZero_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OneZero::setB0");
  gwinote(gwi, "Set the b[0] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB0"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_OneZero_setB0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OneZero::setB1");
  gwinote(gwi, "Set the b[1] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_OneZero_setB1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OneZero::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_OneZero_setCoefficients0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OneZero::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_OneZero_setCoefficients1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::OneZero::setZero");
  gwinote(gwi, "Set the zero position in the z-plane.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method sets the zero position along the real-axis of the z-plane and");
  gwinote(gwi, "normalizes the coefficients for a maximum gain of one. A positive zero value");
  gwinote(gwi, "produces a high-pass filter, while a negative zero value produces a low-pass");
  gwinote(gwi, "filter. This method does not affect the filter *gain* value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setZero"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_OneZero_setZero, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // OneZero

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK percussive flute FM synthesis instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements algorithm 4 of the TX81Z.  ");
  gwinote(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Total Modulator Index = 2  ");
  gwinote(gwi, "*   Modulator Crossfade = 4  ");
  gwinote(gwi, "*   LFO Speed = 11  ");
  gwinote(gwi, "*   LFO Depth = 1  ");
  gwinote(gwi, "*   ADSR 2 & 4 Target = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "The basic Chowning/Stanford FM patent expired in 1995, but there exist follow-on");
  gwinote(gwi, "patents, mostly assigned to Yamaha. If you are of the type who should worry");
  gwinote(gwi, "about this (making money) worry away.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: PercFlut.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "PercFlut", "stk.FM"));
  gwi_class_xtor(gwi, NULL, gw_PercFlut_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PercFlut::PercFlut");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_PercFlut_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PercFlut::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_PercFlut_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PercFlut::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_PercFlut_noteOn, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // PercFlut

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK phonemes table.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class does nothing other than declare a set of 32 static phoneme formant");
  gwinote(gwi, "parameters and provide access to those values.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Phonemes.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Phonemes", "stk.Stk"));
  gwi_class_xtor(gwi, NULL, gw_Phonemes_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Phonemes::Phonemes");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Phonemes_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Phonemes::name");
  gwinote(gwi, "Returns the phoneme name for the given index (0-31).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "string", "name"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Phonemes_name, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Phonemes::voiceGain");
  gwinote(gwi, "Returns the voiced component gain for the given phoneme index (0-31).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "voiceGain"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Phonemes_voiceGain, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Phonemes::noiseGain");
  gwinote(gwi, "Returns the unvoiced component gain for the given phoneme index (0-31).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "noiseGain"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Phonemes_noiseGain, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Phonemes::formantFrequency");
  gwinote(gwi, "Returns the formant frequency for the given phoneme index (0-31) and partial");
  gwinote(gwi, "(0-3).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "formantFrequency"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Phonemes_formantFrequency, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Phonemes::formantRadius");
  gwinote(gwi, "Returns the formant radius for the given phoneme index (0-31) and partial (0-3).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "formantRadius"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Phonemes_formantRadius, ae_flag_static));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Phonemes::formantGain");
  gwinote(gwi, "Returns the formant gain for the given phoneme index (0-31) and partial (0-3).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "formantGain"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Phonemes_formantGain, ae_flag_static));
  GWI_B(gwi_class_end(gwi)); // Phonemes

  GWI_B(gwi_func_ini(gwi, "int", "maxDelay"));
  GWI_B(gwi_func_end(gwi, gw_maxDelay_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK simple pitch shifter effect class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple pitch shifter using delay lines.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: PitShift.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "PitShift", "stk.Effect"));
  gwi_class_xtor(gwi, NULL, gw_PitShift_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PitShift::PitShift");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_PitShift_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PitShift::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_PitShift_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PitShift::setShift");
  gwinote(gwi, "Set the pitch shift factor (1.0 produces no shift).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setShift"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_PitShift_setShift, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // PitShift

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK basic plucked string class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple plucked string physical model based on the");
  gwinote(gwi, "Karplus-Strong algorithm.  ");
  gwinote(gwi, "");
  gwinote(gwi, "For a more advanced plucked string implementation, see the stk::Twang class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others. There exist at least two");
  gwinote(gwi, "patents, assigned to Stanford, bearing the names of Karplus and/or Strong.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Plucked.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Plucked", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Plucked_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Plucked::Plucked");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Plucked_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Plucked::Plucked");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Plucked_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Plucked::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Plucked_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Plucked::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Plucked_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Plucked::pluck");
  gwinote(gwi, "Pluck the string with the given amplitude using the current frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pluck"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Plucked_pluck, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Plucked::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Plucked_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Plucked::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Plucked_noteOff, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Plucked

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK one-pole, one-zero filter class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a one-pole, one-zero digital filter. A method is provided");
  gwinote(gwi, "for creating an allpass filter with a given coefficient. Another method is");
  gwinote(gwi, "provided to create a DC blocking filter.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: PoleZero.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "PoleZero", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_PoleZero_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::PoleZero");
  gwinote(gwi, "Default constructor creates a first-order pass-through filter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::setB0");
  gwinote(gwi, "Set the b[0] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB0"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_setB0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::setB1");
  gwinote(gwi, "Set the b[1] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_setB1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::setA1");
  gwinote(gwi, "Set the a[1] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_setA1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg5"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_setCoefficients0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_setCoefficients1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::setAllpass");
  gwinote(gwi, "Set the filter for allpass behavior using *coefficient*.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method uses *coefficient* to create an allpass filter, which has unity gain");
  gwinote(gwi, "at all frequencies. Note that the *coefficient* magnitude must be less than one");
  gwinote(gwi, "to maintain filter stability.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setAllpass"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_setAllpass, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::setBlockZero");
  gwinote(gwi, "Create a DC blocking filter with the given pole position in the z-plane.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method sets the given pole position, together with a zero at z=1, to create");
  gwinote(gwi, "a DC blocking filter. The argument magnitude should be close to (but less than)");
  gwinote(gwi, "one to minimize low-frequency attenuation.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBlockZero"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_setBlockZero0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PoleZero::setBlockZero");
  gwinote(gwi, "Create a DC blocking filter with the given pole position in the z-plane.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method sets the given pole position, together with a zero at z=1, to create");
  gwinote(gwi, "a DC blocking filter. The argument magnitude should be close to (but less than)");
  gwinote(gwi, "one to minimize low-frequency attenuation.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBlockZero"));
  GWI_B(gwi_func_end(gwi, gw_PoleZero_setBlockZero1, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // PoleZero

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Perry's simple reverberator class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class takes a monophonic input signal and produces a stereo output signal.");
  gwinote(gwi, "It is based on some of the famous Stanford/CCRMA reverbs (NRev, KipRev), which");
  gwinote(gwi, "were based on the Chowning/Moorer/Schroeder reverberators using networks of");
  gwinote(gwi, "simple allpass and comb delay filters. This class implements two series allpass");
  gwinote(gwi, "units and two parallel comb filters.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: PRCRev.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "PRCRev", "stk.Effect"));
  gwi_class_xtor(gwi, NULL, gw_PRCRev_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PRCRev::PRCRev");
  gwinote(gwi, "Class constructor taking a T60 decay time argument (one second default value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_PRCRev_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PRCRev::PRCRev");
  gwinote(gwi, "Class constructor taking a T60 decay time argument (one second default value).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_PRCRev_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PRCRev::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_PRCRev_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::PRCRev::setT60");
  gwinote(gwi, "Set the reverberation T60 decay time.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setT60"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_PRCRev_setT60, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // PRCRev

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "A recorder / flute physical model.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a physical model of a recorder / flute instrument, based");
  gwinote(gwi, "on the paper \"Sound production");
  gwinote(gwi, "in recorderlike instruments. II. A simulation model.\" by M.P. Verge, A.");
  gwinote(gwi, "Hirschberg and R. Causse, Journal of the Acoustical Society of America, 1997.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Softness = 2  ");
  gwinote(gwi, "*   Noise Gain = 4  ");
  gwinote(gwi, "*   Noise Cutoff = 16  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Breath Pressure = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Mathias Bredholt, McGill University. Formatted for STK by Gary Scavone, 2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Recorder.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Recorder", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Recorder_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::Recorder");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::startBlowing");
  gwinote(gwi, "Apply breath velocity to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_startBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::stopBlowing");
  gwinote(gwi, "Decrease breath velocity with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_stopBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_controlChange, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::setBlowPressure");
  GWI_B(gwi_func_ini(gwi, "void", "setBlowPressure"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_setBlowPressure, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::setVibratoGain");
  GWI_B(gwi_func_ini(gwi, "void", "setVibratoGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_setVibratoGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::setVibratoFrequency");
  GWI_B(gwi_func_ini(gwi, "void", "setVibratoFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_setVibratoFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::setNoiseGain");
  GWI_B(gwi_func_ini(gwi, "void", "setNoiseGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_setNoiseGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::setBreathCutoff");
  GWI_B(gwi_func_ini(gwi, "void", "setBreathCutoff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_setBreathCutoff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Recorder::setSoftness");
  GWI_B(gwi_func_ini(gwi, "void", "setSoftness"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Recorder_setSoftness, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Recorder

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK reed table class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple one breakpoint, non-linear reed function, as");
  gwinote(gwi, "described by Smith (1986). This function is based on a memoryless non-linear");
  gwinote(gwi, "spring model of the reed (the reed mass is ignored) which saturates when the");
  gwinote(gwi, "reed collides with the mouthpiece facing.  ");
  gwinote(gwi, "");
  gwinote(gwi, "See McIntyre, Schumacher, & Woodhouse (1983), Smith (1986), Hirschman, Cook,");
  gwinote(gwi, "Scavone, and others for more information.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: ReedTable.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "ReedTable", "stk.Function"));
  gwi_class_xtor(gwi, NULL, gw_ReedTable_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ReedTable::ReedTable");
  gwinote(gwi, "Default constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_ReedTable_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ReedTable::setOffset");
  gwinote(gwi, "Set the table offset value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The table offset roughly corresponds to the size of the initial reed tip opening");
  gwinote(gwi, "(a greater offset represents a smaller opening).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setOffset"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ReedTable_setOffset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::ReedTable::setSlope");
  gwinote(gwi, "Set the table slope value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The table slope roughly corresponds to the reed stiffness (a greater slope");
  gwinote(gwi, "represents a harder reed).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSlope"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_ReedTable_setSlope, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // ReedTable

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK noise driven formant filter.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This instrument contains a noise source, which excites a biquad resonance");
  gwinote(gwi, "filter, with volume controlled by an ADSR.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Resonance Frequency (0-Nyquist) = 2  ");
  gwinote(gwi, "*   Pole Radii = 4  ");
  gwinote(gwi, "*   Notch Frequency (0-Nyquist) = 11  ");
  gwinote(gwi, "*   Zero Radii = 1  ");
  gwinote(gwi, "*   Envelope Gain = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Resonate.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Resonate", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Resonate_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::Resonate");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::setResonance");
  gwinote(gwi, "Set the filter for a resonance at the given frequency (Hz) and radius.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setResonance"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_setResonance, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::setNotch");
  gwinote(gwi, "Set the filter for a notch at the given frequency (Hz) and radius.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setNotch"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_setNotch, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::setEqualGainZeroes");
  gwinote(gwi, "Set the filter zero coefficients for contant resonance gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setEqualGainZeroes"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_setEqualGainZeroes, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::keyOn");
  gwinote(gwi, "Initiate the envelope with a key-on event.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOn"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_keyOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::keyOff");
  gwinote(gwi, "Signal a key-off event to the envelope.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOff"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_keyOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Resonate::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Resonate_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Resonate

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK Fender Rhodes electric piano FM synthesis instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements two simple FM Pairs summed together, also referred to as");
  gwinote(gwi, "algorithm 5 of the TX81Z.  ");
  gwinote(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Modulator Index One = 2  ");
  gwinote(gwi, "*   Crossfade of Outputs = 4  ");
  gwinote(gwi, "*   LFO Speed = 11  ");
  gwinote(gwi, "*   LFO Depth = 1  ");
  gwinote(gwi, "*   ADSR 2 & 4 Target = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "The basic Chowning/Stanford FM patent expired in 1995, but there exist follow-on");
  gwinote(gwi, "patents, mostly assigned to Yamaha. If you are of the type who should worry");
  gwinote(gwi, "about this (making money) worry away.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Rhodey.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Rhodey", "stk.FM"));
  gwi_class_xtor(gwi, NULL, gw_Rhodey_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Rhodey::Rhodey");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Rhodey_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Rhodey::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Rhodey_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Rhodey::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Rhodey_noteOn, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Rhodey

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK faux conical bore reed instrument class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a \"hybrid\" digital waveguide instrument that can");
  gwinote(gwi, "generate a variety of wind-like sounds. It has also been referred to as the");
  gwinote(gwi, "\"blowed string\" model. The waveguide section is essentially that of a string,");
  gwinote(gwi, "with one rigid and one lossy termination. The non-linear function is a reed");
  gwinote(gwi, "table. The string can be \"blown\" at any point between the terminations, though");
  gwinote(gwi, "just as with strings, it is impossible to excite the system at either end. If");
  gwinote(gwi, "the excitation is placed at the string mid-point, the sound is that of a");
  gwinote(gwi, "clarinet. At points closer to the \"bridge\", the sound is closer to that of a");
  gwinote(gwi, "saxophone. See Scavone (2002) for more details.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Reed Stiffness = 2  ");
  gwinote(gwi, "*   Reed Aperture = 26  ");
  gwinote(gwi, "*   Noise Gain = 4  ");
  gwinote(gwi, "*   Blow Position = 11  ");
  gwinote(gwi, "*   Vibrato Frequency = 29  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Breath Pressure = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Saxofony.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Saxofony", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Saxofony_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::Saxofony");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::setBlowPosition");
  gwinote(gwi, "Set the \"blowing\" position between the air column terminations (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBlowPosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_setBlowPosition, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::startBlowing");
  gwinote(gwi, "Apply breath pressure to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_startBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::stopBlowing");
  gwinote(gwi, "Decrease breath pressure with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_stopBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Saxofony::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Saxofony_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Saxofony

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "PhISEM and PhOLIES class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "PhISEM (Physically Informed Stochastic Event Modeling) is an algorithmic");
  gwinote(gwi, "approach for simulating collisions of multiple independent sound producing");
  gwinote(gwi, "objects. This class is a meta-model that can simulate a Maraca, Sekere, Cabasa,");
  gwinote(gwi, "Bamboo Wind Chimes, Water Drops, Tambourine, Sleighbells, and a Guiro.  ");
  gwinote(gwi, "");
  gwinote(gwi, "PhOLIES (Physically-Oriented Library of Imitated Environmental Sounds) is a");
  gwinote(gwi, "similar approach for the synthesis of environmental sounds. This class");
  gwinote(gwi, "implements simulations of breaking sticks, crunchy snow (or not), a wrench,");
  gwinote(gwi, "sandpaper, and more.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Shake Energy = 2  ");
  gwinote(gwi, "*   System Decay = 4  ");
  gwinote(gwi, "*   Number Of Objects = 11  ");
  gwinote(gwi, "*   Resonance Frequency = 1  ");
  gwinote(gwi, "*   Shake Energy = 128  ");
  gwinote(gwi, "*   Instrument Selection = 1071  ");
  gwinote(gwi, "*   Maraca = 0  ");
  gwinote(gwi, "*   Cabasa = 1  ");
  gwinote(gwi, "*   Sekere = 2  ");
  gwinote(gwi, "*   Tambourine = 3  ");
  gwinote(gwi, "*   Sleigh Bells = 4  ");
  gwinote(gwi, "*   Bamboo Chimes = 5  ");
  gwinote(gwi, "*   Sand Paper = 6  ");
  gwinote(gwi, "*   Coke Can = 7  ");
  gwinote(gwi, "*   Sticks = 8  ");
  gwinote(gwi, "*   Crunch = 9  ");
  gwinote(gwi, "*   Big Rocks = 10  ");
  gwinote(gwi, "*   Little Rocks = 11  ");
  gwinote(gwi, "*   Next Mug = 12  ");
  gwinote(gwi, "*   Penny + Mug = 13  ");
  gwinote(gwi, "*   Nickle + Mug = 14  ");
  gwinote(gwi, "*   Dime + Mug = 15  ");
  gwinote(gwi, "*   Quarter + Mug = 16  ");
  gwinote(gwi, "*   Franc + Mug = 17  ");
  gwinote(gwi, "*   Peso + Mug = 18  ");
  gwinote(gwi, "*   Guiro = 19  ");
  gwinote(gwi, "*   Wrench = 20  ");
  gwinote(gwi, "*   Water Drops = 21  ");
  gwinote(gwi, "*   Tuned Bamboo Chimes = 22  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook with updates by Gary Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Shakers.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Shakers", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Shakers_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Shakers::Shakers");
  gwinote(gwi, "Class constructor taking instrument type argument.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Shakers_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Shakers::Shakers");
  gwinote(gwi, "Class constructor taking instrument type argument.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Shakers_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Shakers::noteOn");
  gwinote(gwi, "Start a note with the given instrument and amplitude.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Use the instrument numbers above, converted to frequency values as if MIDI note");
  gwinote(gwi, "numbers, to select a particular instrument.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Shakers_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Shakers::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Shakers_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Shakers::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Shakers_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Shakers

  GWI_B(gwi_func_ini(gwi, "float", "MIN_ENERGY"));
  GWI_B(gwi_func_end(gwi, gw_MIN_ENERGY_get, ae_flag_none));
  GWI_B(gwi_func_ini(gwi, "float", "WATER_FREQ_SWEEP"));
  GWI_B(gwi_func_end(gwi, gw_WATER_FREQ_SWEEP_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK wavetable/noise instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class combines a looped wave, a noise source, a biquad resonance filter, a");
  gwinote(gwi, "one-pole filter, and an ADSR envelope to create some interesting sounds.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Filter Pole Position = 2  ");
  gwinote(gwi, "*   Noise/Pitched Cross-Fade = 4  ");
  gwinote(gwi, "*   Envelope Rate = 11  ");
  gwinote(gwi, "*   Gain = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Simple.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Simple", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Simple_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Simple::Simple");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Simple_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Simple::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Simple_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Simple::keyOn");
  gwinote(gwi, "Start envelope toward \"on\" target.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOn"));
  GWI_B(gwi_func_end(gwi, gw_Simple_keyOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Simple::keyOff");
  gwinote(gwi, "Start envelope toward \"off\" target.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "keyOff"));
  GWI_B(gwi_func_end(gwi, gw_Simple_keyOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Simple::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Simple_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Simple::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Simple_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Simple::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Simple_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Simple

  GWI_B(gwi_func_ini(gwi, "int", "TABLE_SIZE"));
  GWI_B(gwi_func_end(gwi, gw_TABLE_SIZE_get, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK sinusoid oscillator class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class computes and saves a static sine \"table\" that can be shared by");
  gwinote(gwi, "multiple instances. It has an interface similar to the WaveLoop class but");
  gwinote(gwi, "inherits from the Generator class. Output values are computed using linear");
  gwinote(gwi, "interpolation.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The \"table\" length, set in SineWave.h, is 2048 samples by default.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: SineWave.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "SineWave", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_SineWave_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SineWave::SineWave");
  gwinote(gwi, "Default constructor.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_SineWave_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SineWave::reset");
  gwinote(gwi, "Clear output and reset time pointer to zero.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "reset"));
  GWI_B(gwi_func_end(gwi, gw_SineWave_reset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SineWave::setRate");
  gwinote(gwi, "Set the data read rate in samples. The rate can be negative.  ");
  gwinote(gwi, "");
  gwinote(gwi, "If the rate value is negative, the data is read in reverse order.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SineWave_setRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SineWave::setFrequency");
  gwinote(gwi, "Set the data interpolation rate based on a looping frequency.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function determines the interpolation rate based on the file size and the");
  gwinote(gwi, "current Stk::sampleRate. The *frequency* value corresponds to file cycles per");
  gwinote(gwi, "second. The frequency can be negative, in which case the loop is read in reverse");
  gwinote(gwi, "order.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SineWave_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SineWave::addTime");
  gwinote(gwi, "Increment the read pointer by *time* in samples, modulo the table size.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "addTime"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SineWave_addTime, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SineWave::addPhase");
  gwinote(gwi, "Increment the read pointer by a normalized *phase* value.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This function increments the read pointer by a normalized phase value, such that");
  gwinote(gwi, "*phase* = 1.0 corresponds to a 360 degree phase shift. Positive or negative");
  gwinote(gwi, "values are possible.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "addPhase"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SineWave_addPhase, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SineWave::addPhaseOffset");
  gwinote(gwi, "Add a normalized phase offset to the read pointer.  ");
  gwinote(gwi, "");
  gwinote(gwi, "A *phaseOffset* = 1.0 corresponds to a 360 degree phase offset. Positive or");
  gwinote(gwi, "negative values are possible.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "addPhaseOffset"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SineWave_addPhaseOffset, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // SineWave

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK \"singing\" looped soundfile class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class loops a specified soundfile and modulates it both periodically and");
  gwinote(gwi, "randomly to produce a pitched musical sound, like a simple voice or violin. In");
  gwinote(gwi, "general, it is not be used alone because of \"munchkinification\" effects from");
  gwinote(gwi, "pitch shifting. Within STK, it is used as an excitation source for other");
  gwinote(gwi, "instruments.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: SingWave.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "SingWave", "stk.Generator"));
  gwi_class_xtor(gwi, NULL, gw_SingWave_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::SingWave");
  gwinote(gwi, "Class constructor taking filename argument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown, or a");
  gwinote(gwi, "read error occurs. If the soundfile has no header, the second argument should be");
  gwinote(gwi, "*true* and the file data will be assumed to consist of 16-bit signed integers in");
  gwinote(gwi, "big-endian byte order at a sample rate of 22050 Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::SingWave");
  gwinote(gwi, "Class constructor taking filename argument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the file is not found, its format is unknown, or a");
  gwinote(gwi, "read error occurs. If the soundfile has no header, the second argument should be");
  gwinote(gwi, "*true* and the file data will be assumed to consist of 16-bit signed integers in");
  gwinote(gwi, "big-endian byte order at a sample rate of 22050 Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "string", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::reset");
  gwinote(gwi, "Reset file to beginning.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "reset"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_reset, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::normalize");
  gwinote(gwi, "Normalize the file to a maximum of *+-* peak.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "normalize"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_normalize0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::normalize");
  gwinote(gwi, "Normalize the file to a maximum of *+-* peak.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "normalize"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_normalize1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::setFrequency");
  gwinote(gwi, "Set looping parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::setVibratoRate");
  gwinote(gwi, "Set the vibrato frequency in Hz.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVibratoRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_setVibratoRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::setVibratoGain");
  gwinote(gwi, "Set the vibrato gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVibratoGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_setVibratoGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::setRandomGain");
  gwinote(gwi, "Set the random-ness amount.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setRandomGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_setRandomGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::setSweepRate");
  gwinote(gwi, "Set the sweep rate.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setSweepRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_setSweepRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::setGainRate");
  gwinote(gwi, "Set the gain rate.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGainRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_setGainRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::setGainTarget");
  gwinote(gwi, "Set the gain target value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setGainTarget"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_setGainTarget, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::noteOn");
  gwinote(gwi, "Start a note.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::SingWave::noteOff");
  gwinote(gwi, "Stop a note.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_end(gwi, gw_SingWave_noteOff, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // SingWave

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK sitar string model class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a sitar plucked string physical model based on the");
  gwinote(gwi, "Karplus-Strong algorithm.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others. There exist at least two");
  gwinote(gwi, "patents, assigned to Stanford, bearing the names of Karplus and/or Strong.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Sitar.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Sitar", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Sitar_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sitar::Sitar");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Sitar_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sitar::Sitar");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Sitar_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sitar::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Sitar_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sitar::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Sitar_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sitar::pluck");
  gwinote(gwi, "Pluck the string with the given amplitude using the current frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pluck"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Sitar_pluck, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sitar::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Sitar_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Sitar::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Sitar_noteOff, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Sitar

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK 3D vector class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a three-dimensional vector.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Vector3D.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Vector3D", "stk.Stk"));
  gwi_class_xtor(gwi, NULL, gw_Vector3D_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::Vector3D");
  gwinote(gwi, "Default constructor taking optional initial X, Y, and Z values.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::Vector3D");
  gwinote(gwi, "Default constructor taking optional initial X, Y, and Z values.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::Vector3D");
  gwinote(gwi, "Default constructor taking optional initial X, Y, and Z values.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_ctor2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::Vector3D");
  gwinote(gwi, "Default constructor taking optional initial X, Y, and Z values.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_ctor3, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::getX");
  gwinote(gwi, "Get the current X value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getX"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_getX, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::getY");
  gwinote(gwi, "Get the current Y value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getY"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_getY, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::getZ");
  gwinote(gwi, "Get the current Z value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getZ"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_getZ, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::getLength");
  gwinote(gwi, "Calculate the vector length.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "float", "getLength"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_getLength, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::setXYZ");
  gwinote(gwi, "Set the X, Y, and Z values simultaniously.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setXYZ"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_setXYZ, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::setX");
  gwinote(gwi, "Set the X value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setX"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_setX, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::setY");
  gwinote(gwi, "Set the Y value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setY"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_setY, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Vector3D::setZ");
  gwinote(gwi, "Set the Z value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setZ"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Vector3D_setZ, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Vector3D

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK plucked stiff string instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a simple plucked string algorithm (Karplus Strong) with");
  gwinote(gwi, "enhancements (Jaffe-Smith, Smith, and others), including string stiffness and");
  gwinote(gwi, "pluck position controls. The stiffness is modeled with allpass filters.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Pickup Position = 4  ");
  gwinote(gwi, "*   String Sustain = 11  ");
  gwinote(gwi, "*   String Stretch = 1  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: StifKarp.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "StifKarp", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_StifKarp_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::StifKarp");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::StifKarp");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::setStretch");
  gwinote(gwi, "Set the stretch \"factor\" of the string (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setStretch"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_setStretch, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::setPickupPosition");
  gwinote(gwi, "Set the pluck or \"excitation\" position along the string (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPickupPosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_setPickupPosition, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::setBaseLoopGain");
  gwinote(gwi, "Set the base loop gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The actual loop gain is set according to the frequency. Because of high-");
  gwinote(gwi, "frequency loop filter roll-off, higher frequency settings have greater loop");
  gwinote(gwi, "gains.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setBaseLoopGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_setBaseLoopGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::pluck");
  gwinote(gwi, "Pluck the string with the given amplitude using the current frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pluck"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_pluck, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::StifKarp::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_StifKarp_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // StifKarp

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK tubular bell (orchestral chime) FM synthesis instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements two simple FM Pairs summed together, also referred to as");
  gwinote(gwi, "algorithm 5 of the TX81Z.  ");
  gwinote(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Modulator Index One = 2  ");
  gwinote(gwi, "*   Crossfade of Outputs = 4  ");
  gwinote(gwi, "*   LFO Speed = 11  ");
  gwinote(gwi, "*   LFO Depth = 1  ");
  gwinote(gwi, "*   ADSR 2 & 4 Target = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "The basic Chowning/Stanford FM patent expired in 1995, but there exist follow-on");
  gwinote(gwi, "patents, mostly assigned to Yamaha. If you are of the type who should worry");
  gwinote(gwi, "about this (making money) worry away.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: TubeBell.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "TubeBell", "stk.FM"));
  gwi_class_xtor(gwi, NULL, gw_TubeBell_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TubeBell::TubeBell");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_TubeBell_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TubeBell::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_TubeBell_noteOn, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // TubeBell

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK enhanced plucked string class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements an enhanced plucked-string physical model, a la Jaffe-");
  gwinote(gwi, "Smith, Smith, Karjalainen and others. It includes a comb filter to simulate");
  gwinote(gwi, "pluck position. The tick() function takes an input sample, which is added to the");
  gwinote(gwi, "delayline input. This can be used to implement commuted synthesis (if the input");
  gwinote(gwi, "samples are derived from the impulse response of a body filter) and/or feedback");
  gwinote(gwi, "(as in an electric guitar model).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This is a digital waveguide model, making its use possibly subject to patents");
  gwinote(gwi, "held by Stanford University, Yamaha, and others.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Twang.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Twang", "stk.Stk"));
  gwi_class_xtor(gwi, NULL, gw_Twang_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Twang::Twang");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Twang_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Twang::Twang");
  gwinote(gwi, "Class constructor, taking the lowest desired playing frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Twang_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Twang::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Twang_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Twang::setLowestFrequency");
  gwinote(gwi, "Set the delayline parameters to allow frequencies as low as specified.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setLowestFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Twang_setLowestFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Twang::setFrequency");
  gwinote(gwi, "Set the delayline parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Twang_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Twang::setPluckPosition");
  gwinote(gwi, "Set the pluck or \"excitation\" position along the string (0.0 - 1.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPluckPosition"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Twang_setPluckPosition, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Twang::setLoopGain");
  gwinote(gwi, "Set the nominal loop gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The actual loop gain is based on the value set with this function, but scaled");
  gwinote(gwi, "slightly according to the frequency. Higher frequency settings have greater loop");
  gwinote(gwi, "gains because of high-frequency loop-filter roll-off.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setLoopGain"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Twang_setLoopGain, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Twang::setLoopFilter");
  gwinote(gwi, "Set the loop filter coefficients.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The loop filter can be any arbitrary FIR filter. By default, the coefficients");
  gwinote(gwi, "are set for a first-order lowpass filter with coefficients b = [0.5 0.5].  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setLoopFilter"));
  GWI_B(gwi_func_arg(gwi, "float[]", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Twang_setLoopFilter, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Twang

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK two-pole filter class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a two-pole digital filter. A method is provided for");
  gwinote(gwi, "creating a resonance in the frequency response while maintaining a nearly");
  gwinote(gwi, "constant filter gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: TwoPole.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "TwoPole", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_TwoPole_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::TwoPole");
  gwinote(gwi, "Default constructor creates a second-order pass-through filter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_ignoreSampleRateChange0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_ignoreSampleRateChange1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::setB0");
  gwinote(gwi, "Set the b[0] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB0"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_setB0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::setA1");
  gwinote(gwi, "Set the a[1] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_setA1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::setA2");
  gwinote(gwi, "Set the a[2] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setA2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_setA2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg5"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_setCoefficients0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_setCoefficients1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::setResonance");
  gwinote(gwi, "Sets the filter coefficients for a resonance at *frequency* (in Hz).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to two complex-");
  gwinote(gwi, "conjugate poles with the given *frequency* (in Hz) and *radius* from the z-plane");
  gwinote(gwi, "origin. If *normalize* is true, the coefficients are then normalized to produce");
  gwinote(gwi, "unity gain at *frequency* (the actual maximum filter gain tends to be slightly");
  gwinote(gwi, "greater than unity when *radius* is not close to one). The resulting filter");
  gwinote(gwi, "frequency response has a resonance at the given *frequency*. The closer the");
  gwinote(gwi, "poles are to the unit-circle (*radius* close to one), the narrower the resulting");
  gwinote(gwi, "resonance width. An unstable filter will result for *radius* >= 1.0. The");
  gwinote(gwi, "*frequency* value should be between zero and half the sample rate. For a better");
  gwinote(gwi, "resonance filter, use a BiQuad filter.  ");
  gwinote(gwi, "");
  gwinote(gwi, "See also: BiQuad filter class  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setResonance"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_setResonance0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoPole::setResonance");
  gwinote(gwi, "Sets the filter coefficients for a resonance at *frequency* (in Hz).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to two complex-");
  gwinote(gwi, "conjugate poles with the given *frequency* (in Hz) and *radius* from the z-plane");
  gwinote(gwi, "origin. If *normalize* is true, the coefficients are then normalized to produce");
  gwinote(gwi, "unity gain at *frequency* (the actual maximum filter gain tends to be slightly");
  gwinote(gwi, "greater than unity when *radius* is not close to one). The resulting filter");
  gwinote(gwi, "frequency response has a resonance at the given *frequency*. The closer the");
  gwinote(gwi, "poles are to the unit-circle (*radius* close to one), the narrower the resulting");
  gwinote(gwi, "resonance width. An unstable filter will result for *radius* >= 1.0. The");
  gwinote(gwi, "*frequency* value should be between zero and half the sample rate. For a better");
  gwinote(gwi, "resonance filter, use a BiQuad filter.  ");
  gwinote(gwi, "");
  gwinote(gwi, "See also: BiQuad filter class  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setResonance"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_TwoPole_setResonance1, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // TwoPole

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK two-zero filter class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a two-zero digital filter. A method is provided for");
  gwinote(gwi, "creating a \"notch\" in the frequency response while maintaining a constant");
  gwinote(gwi, "filter gain.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: TwoZero.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "TwoZero", "stk.Filter"));
  gwi_class_xtor(gwi, NULL, gw_TwoZero_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::TwoZero");
  gwinote(gwi, "Default constructor creates a second-order pass-through filter.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_ignoreSampleRateChange0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::ignoreSampleRateChange");
  gwinote(gwi, "A function to enable/disable the automatic updating of class data when the STK");
  gwinote(gwi, "sample rate changes.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "ignoreSampleRateChange"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_ignoreSampleRateChange1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::setB0");
  gwinote(gwi, "Set the b[0] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB0"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_setB0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::setB1");
  gwinote(gwi, "Set the b[1] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB1"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_setB1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::setB2");
  gwinote(gwi, "Set the b[2] coefficient value.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setB2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_setB2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_arg(gwi, "bool", "arg5"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_setCoefficients0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::setCoefficients");
  gwinote(gwi, "Set all filter coefficients.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setCoefficients"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_setCoefficients1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::TwoZero::setNotch");
  gwinote(gwi, "Sets the filter coefficients for a \"notch\" at *frequency* (in Hz).  ");
  gwinote(gwi, "");
  gwinote(gwi, "This method determines the filter coefficients corresponding to two complex-");
  gwinote(gwi, "conjugate zeros with the given *frequency* (in Hz) and *radius* from the z-plane");
  gwinote(gwi, "origin. The coefficients are then normalized to produce a maximum filter gain of");
  gwinote(gwi, "one (independent of the filter *gain* parameter). The resulting filter frequency");
  gwinote(gwi, "response has a \"notch\" or anti-resonance at the given *frequency*. The closer");
  gwinote(gwi, "the zeros are to the unit-circle (*radius* close to or equal to one), the");
  gwinote(gwi, "narrower the resulting notch width. The *frequency* value should be between zero");
  gwinote(gwi, "and half the sample rate. The *radius* value should be positive.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setNotch"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_TwoZero_setNotch, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // TwoZero

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK voice manager class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class can be used to manage a group of STK instrument classes. Individual");
  gwinote(gwi, "voices can be controlled via unique note tags. Instrument groups can be");
  gwinote(gwi, "controlled by group number.  ");
  gwinote(gwi, "");
  gwinote(gwi, "A previously constructed STK instrument class is linked with a voice manager");
  gwinote(gwi, "using the addInstrument() function. An optional group number argument can be");
  gwinote(gwi, "specified to the addInstrument() function as well (default group = 0). The voice");
  gwinote(gwi, "manager does not delete any instrument instances ... it is the responsibility of");
  gwinote(gwi, "the user to allocate and deallocate all instruments.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The tick() function returns the mix of all sounding voices. Each noteOn returns");
  gwinote(gwi, "a unique tag (credits to the NeXT MusicKit), so you can send control changes to");
  gwinote(gwi, "specific voices within an ensemble. Alternately, control changes can be sent to");
  gwinote(gwi, "all voices in a given group.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Voicer.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Voicer", "stk.Stk"));
  gwi_class_xtor(gwi, NULL, gw_Voicer_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::Voicer");
  gwinote(gwi, "Class constructor taking an optional note decay time (in seconds).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "float", "arg1"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_ctor0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::Voicer");
  gwinote(gwi, "Class constructor taking an optional note decay time (in seconds).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_ctor1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::addInstrument");
  gwinote(gwi, "Add an instrument with an optional group number to the voice manager.  ");
  gwinote(gwi, "");
  gwinote(gwi, "A set of instruments can be grouped by group number and controlled via the");
  gwinote(gwi, "functions that take a group number argument.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "addInstrument"));
  GWI_B(gwi_func_arg(gwi, "stk.Instrmnt", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_addInstrument0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::addInstrument");
  gwinote(gwi, "Add an instrument with an optional group number to the voice manager.  ");
  gwinote(gwi, "");
  gwinote(gwi, "A set of instruments can be grouped by group number and controlled via the");
  gwinote(gwi, "functions that take a group number argument.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "addInstrument"));
  GWI_B(gwi_func_arg(gwi, "stk.Instrmnt", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_addInstrument1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::removeInstrument");
  gwinote(gwi, "Remove the given instrument pointer from the voice manager's control.  ");
  gwinote(gwi, "");
  gwinote(gwi, "It is important that any instruments which are to be deleted by the user while");
  gwinote(gwi, "the voice manager is running be first removed from the manager's control via");
  gwinote(gwi, "this function!!  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "removeInstrument"));
  GWI_B(gwi_func_arg(gwi, "stk.Instrmnt", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_removeInstrument, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::noteOn");
  gwinote(gwi, "Initiate a noteOn event with the given note number and amplitude and return a");
  gwinote(gwi, "unique note tag.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Send the noteOn message to the first available unused voice. If all voices are");
  gwinote(gwi, "sounding, the oldest voice is interrupted and sent the noteOn message. If the");
  gwinote(gwi, "optional group argument is non-zero, only voices in that group are used. If no");
  gwinote(gwi, "voices are found for a specified non-zero group value, the function returns -1.");
  gwinote(gwi, "The amplitude value should be in the range 0.0 - 128.0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_noteOn0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::noteOn");
  gwinote(gwi, "Initiate a noteOn event with the given note number and amplitude and return a");
  gwinote(gwi, "unique note tag.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Send the noteOn message to the first available unused voice. If all voices are");
  gwinote(gwi, "sounding, the oldest voice is interrupted and sent the noteOn message. If the");
  gwinote(gwi, "optional group argument is non-zero, only voices in that group are used. If no");
  gwinote(gwi, "voices are found for a specified non-zero group value, the function returns -1.");
  gwinote(gwi, "The amplitude value should be in the range 0.0 - 128.0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_noteOn1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::noteOff");
  gwinote(gwi, "Send a noteOff to the voice with the given note tag.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The amplitude value should be in the range 0.0 - 128.0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_noteOff0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::noteOff");
  gwinote(gwi, "Send a noteOff to the voice with the given note tag.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The amplitude value should be in the range 0.0 - 128.0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_noteOff1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::noteOff");
  gwinote(gwi, "Send a noteOff to the voice with the given note tag.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The amplitude value should be in the range 0.0 - 128.0.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_noteOff2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::setFrequency");
  gwinote(gwi, "Send a frequency update message to the voice with the given note tag.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The *noteNumber* argument corresponds to a MIDI note number, though it is a");
  gwinote(gwi, "floating-point value and can range beyond the normal 0-127 range.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_setFrequency0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::setFrequency");
  gwinote(gwi, "Send a frequency update message to the voice with the given note tag.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The *noteNumber* argument corresponds to a MIDI note number, though it is a");
  gwinote(gwi, "floating-point value and can range beyond the normal 0-127 range.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_setFrequency1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::setFrequency");
  gwinote(gwi, "Send a frequency update message to the voice with the given note tag.  ");
  gwinote(gwi, "");
  gwinote(gwi, "The *noteNumber* argument corresponds to a MIDI note number, though it is a");
  gwinote(gwi, "floating-point value and can range beyond the normal 0-127 range.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_setFrequency2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::pitchBend");
  gwinote(gwi, "Send a pitchBend message to the voice with the given note tag.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pitchBend"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_pitchBend0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::pitchBend");
  gwinote(gwi, "Send a pitchBend message to the voice with the given note tag.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pitchBend"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_pitchBend1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::pitchBend");
  gwinote(gwi, "Send a pitchBend message to the voice with the given note tag.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "pitchBend"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_pitchBend2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::controlChange");
  gwinote(gwi, "Send a controlChange to the voice with the given note tag.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_arg(gwi, "int", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_controlChange0, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::controlChange");
  gwinote(gwi, "Send a controlChange to the voice with the given note tag.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_controlChange1, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::controlChange");
  gwinote(gwi, "Send a controlChange to the voice with the given note tag.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "int", "arg3"));
  GWI_B(gwi_func_arg(gwi, "float", "arg4"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_controlChange2, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::silence");
  gwinote(gwi, "Send a noteOff message to all existing voices.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "silence"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_silence, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Voicer::channelsOut");
  gwinote(gwi, "Return the current number of output channels.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "int", "channelsOut"));
  GWI_B(gwi_func_end(gwi, gw_Voicer_channelsOut, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Voicer

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Four formant synthesis instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This instrument contains an excitation singing wavetable (looping wave with");
  gwinote(gwi, "random and periodic vibrato, smoothing on frequency, etc.), excitation noise,");
  gwinote(gwi, "and four sweepable complex resonances.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Measured formant data is included, and enough data is there to support either");
  gwinote(gwi, "parallel or cascade synthesis. In the floating point case cascade synthesis is");
  gwinote(gwi, "the most natural so that's what you'll find here.  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Voiced/Unvoiced Mix = 2  ");
  gwinote(gwi, "*   Vowel/Phoneme Selection = 4  ");
  gwinote(gwi, "*   Vibrato Frequency = 11  ");
  gwinote(gwi, "*   Vibrato Gain = 1  ");
  gwinote(gwi, "*   Loudness (Spectral Tilt) = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: VoicForm.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "VoicForm", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_VoicForm_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::VoicForm");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::setPhoneme");
  gwinote(gwi, "Set instrument parameters for the given phoneme. Returns false if phoneme not");
  gwinote(gwi, "found.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "bool", "setPhoneme"));
  GWI_B(gwi_func_arg(gwi, "string", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_setPhoneme, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::setVoiced");
  gwinote(gwi, "Set the voiced component gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setVoiced"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_setVoiced, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::setUnVoiced");
  gwinote(gwi, "Set the unvoiced component gain.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setUnVoiced"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_setUnVoiced, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::setFilterSweepRate");
  gwinote(gwi, "Set the sweep rate for a particular formant filter (0-3).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFilterSweepRate"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_setFilterSweepRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::setPitchSweepRate");
  gwinote(gwi, "Set voiced component pitch sweep rate.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setPitchSweepRate"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_setPitchSweepRate, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::speak");
  gwinote(gwi, "Start the voice.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "speak"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_speak, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::quiet");
  gwinote(gwi, "Stop the voice.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "quiet"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_quiet, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::VoicForm::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_VoicForm_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // VoicForm

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK police/referee whistle instrument class.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements a hybrid physical/spectral model of a police whistle (a la");
  gwinote(gwi, "Cook).  ");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Noise Gain = 4  ");
  gwinote(gwi, "*   Fipple Modulation Frequency = 11  ");
  gwinote(gwi, "*   Fipple Modulation Gain = 1  ");
  gwinote(gwi, "*   Blowing Frequency Modulation = 2  ");
  gwinote(gwi, "*   Volume = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Whistle.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Whistle", "stk.Instrmnt"));
  gwi_class_xtor(gwi, NULL, gw_Whistle_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Whistle::Whistle");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Whistle_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Whistle::clear");
  gwinote(gwi, "Reset and clear all internal state.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "clear"));
  GWI_B(gwi_func_end(gwi, gw_Whistle_clear, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Whistle::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Whistle_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Whistle::startBlowing");
  gwinote(gwi, "Apply breath velocity to instrument with given amplitude and rate of increase.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "startBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Whistle_startBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Whistle::stopBlowing");
  gwinote(gwi, "Decrease breath velocity with given rate of decrease.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "stopBlowing"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Whistle_stopBlowing, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Whistle::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Whistle_noteOn, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Whistle::noteOff");
  gwinote(gwi, "Stop a note with the given amplitude (speed of decay).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOff"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Whistle_noteOff, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Whistle::controlChange");
  gwinote(gwi, "Perform the control change specified by *number* and *value* (0.0 - 128.0).  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "controlChange"));
  GWI_B(gwi_func_arg(gwi, "int", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Whistle_controlChange, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Whistle

  gwidoc(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "STK Wurlitzer electric piano FM synthesis instrument.  ");
  gwinote(gwi, "");
  gwinote(gwi, "This class implements two simple FM Pairs summed together, also referred to as");
  gwinote(gwi, "algorithm 5 of the TX81Z.  ");
  gwinote(gwi, "");
  gwinote(gwi, "");
  gwinote(gwi, "Control Change Numbers:  ");
  gwinote(gwi, "");
  gwinote(gwi, "*   Modulator Index One = 2  ");
  gwinote(gwi, "*   Crossfade of Outputs = 4  ");
  gwinote(gwi, "*   LFO Speed = 11  ");
  gwinote(gwi, "*   LFO Depth = 1  ");
  gwinote(gwi, "*   ADSR 2 & 4 Target = 128  ");
  gwinote(gwi, "");
  gwinote(gwi, "The basic Chowning/Stanford FM patent expired in 1995, but there exist follow-on");
  gwinote(gwi, "patents, mostly assigned to Yamaha. If you are of the type who should worry");
  gwinote(gwi, "about this (making money) worry away.  ");
  gwinote(gwi, "");
  gwinote(gwi, "by Perry R. Cook and Gary P. Scavone, 1995--2021.  ");
  gwinote(gwi, "");
  gwinote(gwi, "C++ includes: Wurley.h");
  gwinote(gwi, "");
 GWI_B(gwi_class_ini(gwi, "Wurley", "stk.FM"));
  gwi_class_xtor(gwi, NULL, gw_Wurley_dtor);
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Wurley::Wurley");
  gwinote(gwi, "Class constructor.  ");
  gwinote(gwi, "");
  gwinote(gwi, "An StkError will be thrown if the rawwave path is incorrectly set.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, gw_Wurley_ctor, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Wurley::setFrequency");
  gwinote(gwi, "Set instrument parameters for a particular frequency.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "setFrequency"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_end(gwi, gw_Wurley_setFrequency, ae_flag_none));
  gwidoc(gwi, "");
  gwinote(gwi, "stk::Wurley::noteOn");
  gwinote(gwi, "Start a note with the given frequency and amplitude.  ");
  gwinote(gwi, "");
  GWI_B(gwi_func_ini(gwi, "void", "noteOn"));
  GWI_B(gwi_func_arg(gwi, "float", "arg2"));
  GWI_B(gwi_func_arg(gwi, "float", "arg3"));
  GWI_B(gwi_func_end(gwi, gw_Wurley_noteOn, ae_flag_none));
  GWI_B(gwi_class_end(gwi)); // Wurley

  gwi_struct_end(gwi);
  return true;
}
};
extern "C" GWION_IMPORT(STK){ return SwigWrapper::CPPIMPORT(gwi);}
