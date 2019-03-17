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
      return NULL;
	}
};

static TICK(tick) {
  myLinuxSampler* ls = (myLinuxSampler*)u->module.gen.data;
  UGEN(u->connect.multi->channel[0])->out = 0;
  UGEN(u->connect.multi->channel[1])->out = 0;
  ls->tick(&UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);
  u->out = (UGEN(u->connect.multi->channel[0])->out + UGEN(u->connect.multi->channel[1])->out)/2;
}

static CTOR(linuxsampler_ctor) {
  if(!sampler)
    sampler = new LinuxSampler::Sampler();
  std::map<String,LinuxSampler::DeviceCreationParameter*> param;
  myLinuxSampler* ls = *(myLinuxSampler**)(o->data + o_ls_data) = new 
    myLinuxSampler(param, shred->info->vm->bbq->si->sr);
  ugen_ini(UGEN(o), 0, 2);
  ugen_gen(UGEN(o), tick, (void*)ls, 0);
}

static DTOR(linuxsampler_dtor) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  delete ls_obj;
}

static MFUN(linuxsampler_load) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data + o_ls_data);
  ls_obj->load(STRING(*(M_Object*)MEM(SZ_INT)), 0);
}


static MFUN(linuxsampler_load_instrument) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data + o_ls_data);
  ls_obj->load(STRING(*(M_Object*)MEM(SZ_INT)), *(m_uint*)MEM(SZ_INT*2));
}

static MFUN(linuxsampler_noteOn) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendNoteOn(
    (*(m_uint*)RETURN = *(m_int*)MEM(SZ_INT)), *(m_int*)MEM(SZ_INT*2), 0 );
}

static MFUN(linuxsampler_noteOff) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendNoteOff(
    (*(m_uint*)RETURN = *(m_int*)MEM(SZ_INT)), *(m_int*)MEM(SZ_INT*2),0 );
}

static MFUN(linuxsampler_pitchbend) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  if( ls_obj->Engine() ) ls_obj->Engine()->SendPitchbend(
    *(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2), 0 );
}

static MFUN(linuxsampler_status) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_uint*)RETURN = ls_obj->Engine() ? ls_obj->Engine()->InstrumentStatus() : 0;
}

static MFUN(linuxsampler_getgain) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_float*)RETURN = ls_obj->Engine() ? ls_obj->Engine()->Volume() : 0;
}

static MFUN(linuxsampler_setgain) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  if( ls_obj->Engine() ) ls_obj->Engine()->Volume(*(m_float*)RETURN);
}

static MFUN(linuxsampler_getpan) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_float*)RETURN = ls_obj->Engine() ? ls_obj->Engine()->Pan() : 0;
}

static MFUN(linuxsampler_setpan) {
  myLinuxSampler * ls_obj = *(myLinuxSampler**) (o->data +o_ls_data);
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  if(ls_obj->Engine())
    ls_obj->Engine()->Pan(*(m_float*)RETURN);
}

extern "C"
{
GWION_IMPORT(linuxsampler) {
  Type t_ls;
  CHECK_OB((t_ls = gwi_mk_type(gwi, (m_str)"LinuxSampler", SZ_INT, t_ugen)))
  CHECK_BB(gwi_class_ini(gwi, t_ls, linuxsampler_ctor, linuxsampler_dtor))
	gwi_item_ini(gwi,(m_str)"int", (m_str)"@sampler");
  o_ls_data = gwi_item_end(gwi, ae_flag_member, NULL);
  gwi_func_ini(gwi, "void", "load", linuxsampler_load);
    gwi_func_arg(gwi, "string", "filename");
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  gwi_func_ini(gwi, "void", "load", linuxsampler_load_instrument);
    gwi_func_arg(gwi, "string", "filename");
    gwi_func_arg(gwi, "int", "index");
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  gwi_func_ini(gwi, "int", "noteOn", linuxsampler_noteOn);
    gwi_func_arg(gwi, "int", "note");
    gwi_func_arg(gwi, "int", "velocity");
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  gwi_func_ini(gwi, "int", "noteOff", linuxsampler_noteOff);
    gwi_func_arg(gwi, "int", "note");
    gwi_func_arg(gwi, "int", "velocity");
  gwi_func_ini(gwi, "void", "noteOff", linuxsampler_pitchbend);
    gwi_func_arg(gwi, "int", "pitch");
    gwi_func_arg(gwi, "int", "note");
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  gwi_func_ini(gwi, "int", "status", linuxsampler_status);
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  gwi_func_ini(gwi, "float", "gain", linuxsampler_getgain);
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  gwi_func_ini(gwi, "float", "gain", linuxsampler_setgain);
    gwi_func_arg(gwi, "float", "f");
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  gwi_func_ini(gwi, "float", "pan", linuxsampler_getpan);
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  gwi_func_ini(gwi, "float", "pan", linuxsampler_setpan);
    gwi_func_arg(gwi, "float", "f");
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))
  CHECK_BB(gwi_class_end(gwi));
  return 1;
}
}
