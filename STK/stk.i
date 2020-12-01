%module STK
%nspc_ini(stk);
%insert("init") {
gwi_typedef_ini(gwi, (m_str)"float", (m_str)"StkFloat");
gwi_typedef_end(gwi, ae_flag_none);
}
// We'll get a better way
%insert("header") {extern "C" { ANEW UGen new_UGen(MemPool p);}}

// StkFloat handling
//%insert("header") {#define StkFloat m_float}
%typemap(ffi) StkFloat "float";
%typemap(ffi) stk::StkFloat = float;
%typemap(ffi) stk::StkFloat& "Ptr:[float]";
%typemap(out) StkFloat = float;
%typemap(in) StkFloat = float;
%typemap(in) stk::StkFloat = float;
%typemap(in) stk::StkFloat& = float&;
//%typemap(out) stk::StkFloat& = float&;
%typemap(in) StkFloat*, stk::StkFloat*, stk::StkFloat[ANY] = float*;
%typemap(offset) StkFloat = float;
//%typemap(ffi) std::vector< stk::StkFloat > "float[]";
%typemap(ffi) std::vector< stk::StkFloat > "float[]";

// Samplerate
%insert("init") {stk::Stk::setSampleRate(gwi->gwion->vm->bbq->si->sr);}

%ignore *::swap16;
%ignore *::swap32;
%ignore *::swap64;
%ignore *::operator ();
%ignore *::operator[]; // we should import on of them

%ignore stk::StkError;
%ignore stk::Stk::handleError;
%ignore stk::StkFrames;

%define %StkInit(TYPE)
%feature("extends", "UGen") TYPE;
%insert("header") %{#include "TYPE.h"%}
%include "TYPE.h";
%ignore stk::TYPE::lastFrame;
%ignore stk::TYPE::tick;
%enddef

%define %StkUGen(TYPE, NIN, NOUT)
%feature("extends", "UGen") TYPE;
%feature("in", "NIN") TYPE;
%feature("out", "NOUT") TYPE;
%ignore stk::TYPE::tick;
%ignore stk::TYPE::lastOut;
%ignore stk::TYPE::lastFrame;
%insert("wrapper") {static TICK(TYPE##_tick) {
  stk::TYPE *s = (stk::TYPE*)u->module.gen.data;
  u->out = s->tick(1);
}
}
%insert("header") %{#include "TYPE.h"%}
%include "TYPE.h";
%enddef

%define %StkUGenVoid(TYPE, NIN, NOUT)
%feature("extends", "UGen") TYPE;
%feature("in", "NIN") TYPE;
%feature("out", "NOUT") TYPE;
%ignore stk::TYPE::tick;
%ignore stk::TYPE::lastOut;
%ignore stk::TYPE::lastFrame;
%insert("wrapper") {static TICK(TYPE##_tick) {
  stk::TYPE *s = (stk::TYPE*)u->module.gen.data;
  u->out = s->tick();
}
}
%insert("header") %{#include "TYPE.h"%}
%include "TYPE.h";
%enddef

%StkInit(Stk);
%StkUGen(Instrmnt, 0, 1);
%StkInit(Generator);
%StkUGen(FM, 0, 1);
%StkUGenVoid(Envelope, 0, 1);
%StkInit(Filter);
%StkInit(Effect);
%StkInit(Function);
%StkUGen(Sampler, 0, 1);
//%gwinclude(Socket.h);  // void sized variable?
%StkUGenVoid(ADSR, 0, 1);
%StkUGenVoid(Asymp, 0, 1);
%StkUGenVoid(BandedWG, 0, 1);
%StkUGen(BiQuad, 1, 1);
%StkUGenVoid(Blit, 0, 1);
%StkUGenVoid(BlitSaw, 0, 1);
%StkUGenVoid(BlitSquare, 0, 1);
%StkUGen(BeeThree, 0, 1);
%StkUGen(BlowBotl, 0, 1);
%StkUGen(BlowHole, 0, 1);
%StkUGen(Bowed, 0, 1);
%StkUGen(BowTable, 0, 1);
%StkUGen(Brass, 0, 1);
%StkUGen(Chorus, 1, 1);
%StkUGen(Clarinet, 0, 1);
%StkUGen(Cubic, 1, 1);
%StkUGen(Delay, 1, 1);
%StkUGen(DelayA, 1, 1);
%StkUGen(DelayL, 1, 1);
%StkUGen(Drummer, 0, 1);
%StkUGen(Echo, 1, 1);
//%StkUGen(FileRead, 0, result->channels()); // TODO: fix output channel number
//%StkUGen(FileWvIn, 0, result->channels()); // TODO: fix output channel number
//%StkUGen(FileWvOut, 0, result->channels()); // TODO: fix output channel number
//%StkUGen(FileLoop, 0, 1); // "
//%StkUGen(Fir, 0, 1); // excedental < ?
%StkUGen(Flute, 0, 1);
%StkUGen(FMVoices, 0, 1);
%StkUGen(FormSwep, 0, 1);
%StkUGen(FreeVerb, 0, 1);
%StkUGen(Granulate, 0, 1); // Grain ignored
%StkUGen(Guitar, 0, 1);
%StkUGen(HevyMetl, 0, 1);
//%StkUGen(Iir, 0, 1); // excedental < ?
//%StkUGen(InetWvIn, 0, 1);
//%StkUGen(InetWvOut, 0, 1);
%StkUGen(JCRev, 1, 1);
%StkUGen(JetTable, 0, 1);
%StkUGen(LentPitShift, 0, 1);
%StkUGen(Mandolin, 0, 1);
%StkUGen(Mesh2D, 0, 1);
//%gwinclude(Skini.h);
//%StkInit(SKINImsg);
//%StkInit(SKINItbl);
//%StkUGen(Messager, 0, 1);
//%gwinclude(MidiFileIn.h); // excedental
%StkUGen(Modal, 0, 1);
%StkUGen(ModalBar, 0, 1);
%StkUGenVoid(Modulate, 0, 1);
%StkUGen(Moog, 0, 1);
//%StkUGen(Mutex, 0, 1);
%StkUGenVoid(Noise, 0, 1);
%StkUGen(NRev, 1, 1);
%StkUGen(OnePole, 1, 1);
%StkUGen(OneZero, 1, 1);
%StkUGen(PercFlut, 0, 1);
%gwinclude(Phonemes.h);
%StkUGen(PitShift, 1, 1);
%StkUGen(Plucked, 0, 1);
%StkUGen(PoleZero, 1, 1);
%StkUGen(PRCRev, 1, 1);
%StkUGen(Recorder, 1, 1);
%StkUGen(ReedTable, 0, 1);
%StkUGen(Resonate, 1, 1);
%StkUGen(Rhodey, 0, 1);
//%StkUGen(RtAudio, 0, 1);
//%StkUGen(RtMidi, 0, 1);
//%StkUGen(RtWvIn, 0, 1);
//%StkUGen(RtWvout, 0, 1);
%StkUGen(Saxofony, 0, 1);
//%rename stk::Shakers::BiQuad ShakersBiQuad;
%ignore stk::Shakers::BiQuad;
//%ignore ShakersBiQuad;
%StkUGen(Shakers, 0, 1);
%StkUGenVoid(Simple, 0, 1);
%StkUGenVoid(SineWave, 0, 1);
%StkUGenVoid(SingWave, 0, 1);
%StkUGen(Sitar, 0, 1);
%gwinclude(Vector3D.h);
//%StkUGenVoid(Sphere, 0, 1);
%StkUGen(StifKarp, 0, 1);
//%StkUGen(TapDelay, 1, 1);
//%StkInit(TcpClient);
//%StkInit(TcpServer);
//%StkInit(Thread);
%StkUGen(TubeBell, 0, 1);
%StkUGen(Twang, 0, 1);
%StkUGen(TwoPole, 1, 1);
%StkUGen(TwoZero, 1, 1);
//%StkUGen(UdpSocket, 1, 1);
%StkUGen(Voicer, 0, 1);
%StkUGen(VoicForm, 0, 1);
%StkUGen(Whistle, 0, 1);
%StkUGen(Wurley, 0, 1);
//%StkUGen(WvIn, 1, 1);// channels
//%StkUGen(WvOut, 1, 1); // TODO: fix channels
%nspc_end();
