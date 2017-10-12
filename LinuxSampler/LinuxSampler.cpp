#include <linuxsampler/Sampler.h>
#include "Gwion.hpp"

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
  UGEN(u->channel[0])->out = 0;
  UGEN(u->channel[1])->out = 0;
  ls->tick(&UGEN(u->channel[0])->out, &UGEN(u->channel[1])->out);
  u->out = u->last = (UGEN(u->channel[0])->out + UGEN(u->channel[1])->out)/2;
}

static struct Type_ t_ls= { (m_str)"LinuxSampler", SZ_INT, &t_ugen };
CTOR(linuxsampler_ctor)
{
  if(!sampler)
    sampler = new LinuxSampler::Sampler();
  std::map<String,LinuxSampler::DeviceCreationParameter*> param;
  myLinuxSampler* ls = *(myLinuxSampler**)(o->data + o_ls_data) = new myLinuxSampler(param, shred->vm_ref->sp->sr);
  UGEN(o)->tick = tick;
  assign_ugen(UGEN(o), 0, 2, 0, (void*)ls);
}

DTOR(linuxsampler_dtor)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  delete ls_obj;
}

MFUN(linuxsampler_load)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data + o_ls_data);
  ls_obj->load(STRING(*(M_Object*)MEM(SZ_INT)), 0);
}


MFUN(linuxsampler_load_instrument)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data + o_ls_data);
  ls_obj->load(STRING(*(M_Object*)MEM(SZ_INT)), *(m_uint*)MEM(SZ_INT*2));
}

MFUN(linuxsampler_noteOn)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendNoteOn(
    (*(m_uint*)RETURN = *(m_int*)MEM(SZ_INT)), *(m_int*)MEM(SZ_INT*2), 0 );
}

MFUN(linuxsampler_noteOff)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendNoteOff(
    (*(m_uint*)RETURN = *(m_int*)MEM(SZ_INT)), *(m_int*)MEM(SZ_INT*2),0 );
}

MFUN(linuxsampler_pitchbend)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendPitchbend(
    *(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2), 0 );
}

MFUN(linuxsampler_status)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_uint*)RETURN = ls_obj->Engine() ? ls_obj->Engine()->InstrumentStatus() : 0;
}

MFUN(linuxsampler_getgain)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_float*)RETURN = ls_obj->Engine() ? ls_obj->Engine()->Volume() : 0;
}

MFUN(linuxsampler_setgain)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  if( ls_obj->Engine() ) ls_obj->Engine()->Volume(*(m_float*)RETURN);
}

MFUN(linuxsampler_getpan)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_float*)RETURN = ls_obj->Engine() ? ls_obj->Engine()->Pan() : 0;
}

MFUN(linuxsampler_setpan)
{
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  if(ls_obj->Engine())
    ls_obj->Engine()->Pan(*(m_float*)RETURN);
}

extern "C"
{
IMPORT
{
  CHECK_BB(importer_class_ini(importer, &t_ls, linuxsampler_ctor, linuxsampler_dtor))
	importer_item_ini(importer,(m_str)"int", (m_str)"@sampler");
  o_ls_data = importer_item_end(importer, ae_flag_member, NULL);
  importer_func_ini(importer, "void", "load", (m_uint)linuxsampler_load);
    importer_func_arg(importer, "string", "filename");
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  importer_func_ini(importer, "void", "load", (m_uint)linuxsampler_load_instrument);
    importer_func_arg(importer, "string", "filename");
    importer_func_arg(importer, "int", "index");
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  importer_func_ini(importer, "int", "noteOn", (m_uint)linuxsampler_noteOn);
    importer_func_arg(importer, "int", "note");
    importer_func_arg(importer, "int", "velocity");
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  importer_func_ini(importer, "int", "noteOff", (m_uint)linuxsampler_noteOff);
    importer_func_arg(importer, "int", "note");
    importer_func_arg(importer, "int", "velocity");
  importer_func_ini(importer, "void", "noteOff", (m_uint)linuxsampler_pitchbend);
    importer_func_arg(importer, "int", "pitch");
    importer_func_arg(importer, "int", "note");
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  importer_func_ini(importer, "int", "status", (m_uint)linuxsampler_status);
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  importer_func_ini(importer, "float", "gain", (m_uint)linuxsampler_getgain);
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  importer_func_ini(importer, "float", "gain", (m_uint)linuxsampler_setgain);
    importer_func_arg(importer, "float", "f");
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  importer_func_ini(importer, "float", "pan", (m_uint)linuxsampler_getpan);
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  importer_func_ini(importer, "float", "pan", (m_uint)linuxsampler_setpan);
    importer_func_arg(importer, "float", "f");
  CHECK_BB(importer_func_end(importer, ae_flag_member))
  CHECK_BB(importer_class_end(importer));
  return 1;
}
}
