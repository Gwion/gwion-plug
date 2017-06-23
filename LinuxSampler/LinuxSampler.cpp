#include "Gwion.hpp"
#include "ugen.h"
#include <linuxsampler/Sampler.h>

m_int o_ls_data = 0;
static LinuxSampler::Sampler * sampler = NULL;

class ChuckOutput : public LinuxSampler::AudioOutputDevice
{
public:
  ChuckOutput(std::map<String,LinuxSampler::DeviceCreationParameter*> param, m_float fs, size_t size = 1204) :
		LinuxSampler::AudioOutputDevice(param) , fs(fs), size(size) {}
  virtual bool isAutonomousDevice(){ return false; }
  virtual bool isAutonomousDeviceDriver(){ return false; }
  virtual void Play() { play = 1; }
  virtual bool IsPlaying() { return play; }
  virtual void Stop() { play = 0; }
  virtual uint MaxSamplesPerCycle() { return size; }
  virtual uint SampleRate() { return fs; }
  virtual String Driver() { return "gwion"; }
  virtual LinuxSampler::AudioChannel* CreateChannel(uint ChannelNr) { return new LinuxSampler::AudioChannel(ChannelNr, size); }
  size_t size;
private:
  bool play;
  m_float fs;
};

class myLinuxSampler : public ChuckOutput
{
public:
  myLinuxSampler(std::map<String,LinuxSampler::DeviceCreationParameter*> param, m_float fs) : ChuckOutput(param, fs, 512), count(0)
  {
    channel = sampler->AddSamplerChannel();
    channel->SetAudioOutputDevice(this);
  }

  ~myLinuxSampler()
  {
    if(thread)
      pthread_join(thread, NULL);
    sampler->RemoveSamplerChannel(channel);
  }

  void tick(m_float* l, m_float* r)
  {
    if(!Engine())
    	return;
    if(!(count%size))
      RenderAudio(size);
    *l = Channel(0)->Buffer()[count%size];
    *r = Channel(1)->Buffer()[count%size];
    count++;
  }

	void load(std::string s, int instrument = 0)
	{
		std::string suffix = s.substr(s.length()-3, s.length() );
		if( suffix == "gig" || suffix == "GIG" )
			channel->SetEngineType("GIG");
		else if( suffix == "sf2" || suffix == "SF2" )
			channel->SetEngineType("SF2");
		else if( suffix == "sfz" || suffix == "SFZ" )
			channel->SetEngineType("SFZ");
		this->s = s;
		pthread_create(&thread, NULL, &run, this);
	}

	LinuxSampler::EngineChannel* Engine() { return channel->GetEngineChannel(); }
private:
  m_uint count;
  std::string s;
	pthread_t thread = 0;
  LinuxSampler::SamplerChannel* channel;
	static void* run(void *arg)
	{
		myLinuxSampler * ls = (myLinuxSampler *) arg;
		ls->Engine()->PrepareLoadInstrument(ls->s.data(), 0);
		ls->Engine()->LoadInstrument();
	}
};


m_bool tick(UGen u)
{
  myLinuxSampler* ls = (myLinuxSampler*)u->ug;
u->channel[0]->ugen->out = 0;
u->channel[1]->ugen->out = 0;
  ls->tick(&u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
  u->out = u->last = (u->channel[0]->ugen->out + u->channel[1]->ugen->out)/2;
}

static struct Type_ t_ls= { (m_str)"LinuxSampler", SZ_INT, &t_ugen };
CTOR(linuxsampler_ctor)
{
  if(!sampler)
    sampler = new LinuxSampler::Sampler();
	std::map<String,LinuxSampler::DeviceCreationParameter*> param;
  myLinuxSampler* ls = *(myLinuxSampler**)(o->d.data + o_ls_data) = new myLinuxSampler(param, shred->vm_ref->bbq->sp->sr);
  o->ugen->ug = ls;
  o->ugen->n_in = 0;
  o->ugen->n_out = 2;
  o->ugen->tick = tick;
  vector_init(&o->ugen->ugen);
  o->ugen->channel = (M_Object*)calloc(2, sizeof(struct M_Object_));
  o->ugen->channel[0] = new_M_UGen();
  vector_init(&o->ugen->channel[0]->ugen->ugen);
  o->ugen->channel[0]->ugen->ref = o->ugen;
  o->ugen->channel[1] = new_M_UGen();
  vector_init(&o->ugen->channel[1]->ugen->ugen);
  o->ugen->channel[1]->ugen->ref = o->ugen;
}

DTOR(linuxsampler_dtor)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  delete ls_obj;
}

MFUN(linuxsampler_load)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data + o_ls_data);
  ls_obj->load(STRING(*(M_Object*)(shred->mem + SZ_INT)), 0);
}


MFUN(linuxsampler_load_instrument)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data + o_ls_data);
  ls_obj->load(STRING(*(M_Object*)(shred->mem + SZ_INT)), *(m_uint*)(shred->mem + SZ_INT*2));
}

MFUN(linuxsampler_noteOn)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendNoteOn(
    (RETURN->d.v_uint = *(m_int*)(shred->mem + SZ_INT)), *(m_int*)(shred->mem + SZ_INT*2), 0 );
}

MFUN(linuxsampler_noteOff)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendNoteOff(
    (RETURN->d.v_uint = *(m_int*)(shred->mem + SZ_INT)), *(m_int*)(shred->mem + SZ_INT*2),0 );
}

MFUN(linuxsampler_pitchbend)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendPitchbend(
    *(m_int*)(shred->mem + SZ_INT), *(m_int*)(shred->mem + SZ_INT*2), 0 );
}

MFUN(linuxsampler_status)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  RETURN->d.v_uint = ls_obj->Engine() ? ls_obj->Engine()->InstrumentStatus() : 0;
}

MFUN(linuxsampler_getgain)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  RETURN->d.v_float = ls_obj->Engine() ? ls_obj->Engine()->Volume() : 0;
}

MFUN(linuxsampler_setgain)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  RETURN->d.v_float = *(m_float*)(shred->mem + SZ_INT);
  if( ls_obj->Engine() ) ls_obj->Engine()->Volume(RETURN->d.v_float);
}

MFUN(linuxsampler_getpan)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  RETURN->d.v_float = ls_obj->Engine() ? ls_obj->Engine()->Pan() : 0;
}

MFUN(linuxsampler_setpan)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->d.data +o_ls_data);
  RETURN->d.v_float = *(m_float*)(shred->mem + SZ_INT);
  if(ls_obj->Engine())
    ls_obj->Engine()->Pan(RETURN->d.v_float);
}

extern "C"
{
m_bool import(Env env)
{
  DL_Func fun;
  DL_Value* arg;
 	Func f;

  CHECK_BB(import_class_begin(env, &t_ls, env->global_nspc, linuxsampler_ctor, linuxsampler_dtor))
  o_ls_data = import_mvar(env, (m_str)"int", (m_str)"@sampler", 0, 0);
  dl_func_init(&fun, "void", "load", (m_uint)linuxsampler_load);
    dl_func_add_arg(&fun, "string", "filename");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "void", "load", (m_uint)linuxsampler_load_instrument);
    dl_func_add_arg(&fun, "string", "filename");
    dl_func_add_arg(&fun, "int", "index");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "int", "noteOn", (m_uint)linuxsampler_noteOn);
    dl_func_add_arg(&fun, "int", "note");
    dl_func_add_arg(&fun, "int", "velocity");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "int", "noteOff", (m_uint)linuxsampler_noteOff);
    dl_func_add_arg(&fun, "int", "note");
    dl_func_add_arg(&fun, "int", "velocity");
  dl_func_init(&fun, "void", "noteOff", (m_uint)linuxsampler_pitchbend);
    dl_func_add_arg(&fun, "int", "pitch");
    dl_func_add_arg(&fun, "int", "note");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "int", "status", (m_uint)linuxsampler_status);
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "float", "gain", (m_uint)linuxsampler_getgain);
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "float", "gain", (m_uint)linuxsampler_setgain);
    dl_func_add_arg(&fun, "float", "f");
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "float", "pan", (m_uint)linuxsampler_getpan);
  CHECK_BB(import_mfun(env, &fun))
  dl_func_init(&fun, "float", "pan", (m_uint)linuxsampler_setpan);
    dl_func_add_arg(&fun, "float", "f");
  CHECK_BB(import_mfun(env, &fun))
  CHECK_BB(import_class_end(env));
  return 1;
}
}
