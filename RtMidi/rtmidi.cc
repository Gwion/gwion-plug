#include <rtmidi/RtMidi.h>
#include "Gwion.hh"

ANN static void error_callback(RtMidiError::Type type, const std::string &errorText, void *data NUSED) {
  switch(type) {
    case RtMidiError::Type::WARNING:
     gw_err("{+Y}[RtMidi] Warning{0}: ");
     break;
    case RtMidiError::Type::DEBUG_WARNING:
     gw_err("{+Y}[RtMidi] Debug Warning{0}: ");
     break;
    case RtMidiError::Type::UNSPECIFIED:
     gw_err("{+R}[RtMidi] UNSPECIFIED{0}: ");
     break;
    case RtMidiError::Type::NO_DEVICES_FOUND:
     gw_err("{+R}[RtMidi] No Device Found{0}: ");
     break;
    case RtMidiError::Type::INVALID_DEVICE:
     gw_err("{+R}[RtMidi] Invalid Device{0}: ");
     break;
    case RtMidiError::Type::MEMORY_ERROR:
     gw_err("{+R}[RtMidi] Memory Error{0}: ");
     break;
    case RtMidiError::Type::INVALID_PARAMETER:
     gw_err("{+R}[RtMidi] Invalid Parameter{0}: ");
     break;
    case RtMidiError::Type::INVALID_USE:
     gw_err("{+R}[RtMidi] Invalid Use{0}: ");
     break;
    case RtMidiError::Type::DRIVER_ERROR:
     gw_err("{+R}[RtMidi] Driver Error{0}: ");
     break;
    case RtMidiError::Type::SYSTEM_ERROR:
     gw_err("{+R}[RtMidi] System Error{0}: ");
     break;
    case RtMidiError::Type::THREAD_ERROR:
     gw_err("{+R}[RtMidi] Thread Error{0}: ");
     break;
  }
  gw_err("%s\n", errorText.data());
}

struct MidiIn {
  RtMidiIn *in;
  MUTEX_TYPE mutex;
  struct M_Vector_ msgs;
  struct Vector_   thru;
};

static DTOR(rtmidiin_dtor) {
  struct MidiIn *min = (MidiIn*)(o->data + SZ_INT);
  MUTEX_TYPE mutex = min->mutex;
  MUTEX_LOCK(mutex);
  m_vector_release(&min->msgs);
  if(min->thru.ptr) {
    for(m_uint i = 0; i < vector_size(&min->thru); i++)
      _release((M_Object)vector_at(&min->thru, i), shred);
  }
  delete min->in;
  MUTEX_UNLOCK(mutex);
  MUTEX_CLEANUP(mutex);
}

ANN static inline void min_add(struct MidiIn *min, unsigned char msg[3]) {
  MUTEX_LOCK(min->mutex);
  m_vector_add(&min->msgs, msg);
  MUTEX_UNLOCK(min->mutex);
}

ANN static void callback(double timeStamp NUSED, std::vector<unsigned char> *message, void *data) {
  const M_Object o = (M_Object)data;
  struct MidiIn *min = (MidiIn*)(o->data + SZ_INT);
  size_t sz = message->size(),
         i  = 0,
         j  = 0;
  unsigned char msg[3] = {};
  while(i < sz) {
    msg[j] = message->at(i);
    if(++j == 3) {
      min_add(min, msg);
      memset(msg, 0, 3);
      j = 0;
    }
    ++i;
  }
  if(sz % 3)
    min_add(min, msg);
  broadcast(o);
}

ANN static void thru_callback(double timeStamp , std::vector<unsigned char> *message, void *data) {
  callback(timeStamp, message, data);
  const M_Object o = (M_Object)data;
  struct MidiIn *min = (MidiIn*)(o->data + SZ_INT);
  for(m_uint i = 0; i < vector_size(&min->thru); i++) {
    RtMidiOut *mout = (RtMidiOut*)vector_at(&min->thru, i);
    mout->sendMessage(message);
  }
}

ANN static void min_init(struct MidiIn *min, const M_Object o) {
  min->in = new RtMidiIn(RtMidi::Api::UNSPECIFIED, "Gwion In");
  min->in->setCallback(callback, o);
  min->in->setErrorCallback(error_callback, o);
  MUTEX_SETUP(min->mutex);
  m_vector_init(&min->msgs, 4 * sizeof(m_bit), 0);
}

#define MIN_INIT(name, open)                        \
static MFUN(rtmidiin_##name) {                      \
  *(M_Object*)RETURN = o;                           \
  struct MidiIn *min = (MidiIn*)(o->data + SZ_INT); \
  try {                                             \
    min_init(min, o);                               \
    min->in->open;                                  \
  } catch (RtMidiError &e) {                        \
    handle(shred, (m_str)"MidiInException");        \
  }                                                 \
}

MIN_INIT(new0, openPort())
MIN_INIT(new1, openPort(*(m_int*)MEM(SZ_INT)))
MIN_INIT(new2, openPort(*(m_int*)MEM(SZ_INT),
               STRING(*(M_Object*)MEM(SZ_INT*2))))
MIN_INIT(new3, openVirtualPort(STRING(*(M_Object*)MEM(SZ_INT))))

#undef MIN_INIT

static MFUN(rtmidiin_connect) {
  struct MidiIn *min = (MidiIn*)(o->data + SZ_INT);
  const M_Object mout = *(M_Object*)MEM(SZ_INT);
  vector_add(&min->thru, (m_uint)mout);
  mout->ref++;
  if(!min->thru.ptr) {
    vector_init(&min->thru);
    min->in->setCallback(thru_callback, o);
  }
}

static MFUN(rtmidiin_disconnect) {
  struct MidiIn *min = (MidiIn*)(o->data + SZ_INT);
  const M_Object mout = *(M_Object*)MEM(SZ_INT);
  const m_int idx = vector_find(&min->thru, (m_uint)mout);
  if(idx != -1) {
    vector_rem(&min->thru, idx);
    release(mout, shred);
    if(!vector_size(&min->thru))
      min->in->setCallback(callback, o);
  }
}

static MFUN(rtmidiin_read) {
  struct MidiIn *min = (MidiIn*)(o->data + SZ_INT);
  MUTEX_LOCK(min->mutex);
  const m_uint sz = m_vector_size(&min->msgs);
  if(sz) {
    const unsigned char *msg = (unsigned char*)(min->msgs.ptr + ARRAY_OFFSET);
    **(m_int**)MEM(SZ_INT)   = msg[0];
    **(m_int**)MEM(SZ_INT*2) = msg[1];
    **(m_int**)MEM(SZ_INT*3) = msg[2];
    m_vector_rem(&min->msgs, 0);
    *(m_uint*)RETURN = true;
  } else
    *(m_uint*)RETURN = false;
  MUTEX_UNLOCK(min->mutex);
}

static MFUN(rtmidiin_ignore) {
  struct MidiIn *min = (MidiIn*)(o->data + SZ_INT);
  min->in->ignoreTypes(*(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2), *(m_uint*)MEM(SZ_INT*3));
}
#define static_func(IO, io)\
static SFUN(rtmidi##io##_count) {\
  try{\
    RtMidi##IO io(RtMidi::Api::UNSPECIFIED, "Gwion " #IO);\
   *(m_int*)RETURN = io.getPortCount();\
  } catch (RtMidiError &e) {\
    handle(shred, (m_str)"MidiOutException");\
  }\
}\
\
static SFUN(rtmidi##io##_name) {                                                   \
  try{\
    RtMidi##IO io(RtMidi::Api::UNSPECIFIED, "Gwion " #IO);                        \
    std::string str = io.getPortName(*(m_int*)MEM(0));                           \
    *(M_Object*)RETURN = new_string(shred->info->vm->gwion, (m_str)str.data()); \
  } catch (RtMidiError &e) {\
    handle(shred, (m_str)"MidiOutException");\
  }\
	}                                                                              \
\
static SFUN(rtmidi##io##_names) {\
  try{\
    RtMidi##IO io(RtMidi::Api::UNSPECIFIED, "Gwion " #IO);\
    const unsigned int count = io.getPortCount();\
    const VM_Code code = *(VM_Code*)REG(0);\
    const M_Object ret = new_array(shred->info->mp, code->ret_type, count);\
    for(m_uint i = 0; i < count; i++) {\
      std::string str = io.getPortName(i);\
      *(M_Object*)(ARRAY(ret)->ptr + ARRAY_OFFSET + i * SZ_INT) =\
             new_string(shred->info->vm->gwion, (m_str)str.data());\
    }\
    *(M_Object*)RETURN = ret;\
  } catch (RtMidiError &e) {\
    handle(shred, (m_str)"MidiOutException");\
  }\
}\

static_func(In,  in);
static_func(Out, out);

static DTOR(rtmidiout_dtor) {
  RtMidiOut *out = *(RtMidiOut**)(o->data + SZ_INT*2);
  delete out;
}

static MFUN(rtmidiout_send) {
  RtMidiOut *out = *(RtMidiOut**)(o->data + SZ_INT*2);
  M_Vector array = ARRAY(o);
  const m_uint sz = m_vector_size(array);
  if(!sz) return;
  std::vector<unsigned char> msg(sz);
  for(m_uint i = 0; i < sz; i++)
    msg[i] = *(m_uint*)(array->ptr + ARRAY_OFFSET + i*SZ_INT);
  try {
    out->sendMessage(&msg);
  } catch (RtMidiError &e) {
    handle(shred, (m_str)"MidiOutException");
  }
  ARRAY_LEN(array) = 0;
}

#define MOUT_INIT(name, open)                                  \
static MFUN(rtmidiout_##name) {                                \
  *(M_Object*)RETURN = o;                                      \
  try {                                                        \
    RtMidiOut *mout = *(RtMidiOut**)(o->data + SZ_INT*2) =     \
      new RtMidiOut(RtMidi::Api::UNSPECIFIED, "Gwion Output"); \
    mout->setErrorCallback(error_callback, o);                 \
    mout->open;                                                \
  } catch (RtMidiError &e) {                                   \
    handle(shred, (m_str)"MidiOutException");                  \
  }                                                            \
}

MOUT_INIT(new0, openPort())
MOUT_INIT(new1, openPort(*(m_int*)MEM(SZ_INT)))
MOUT_INIT(new2, openPort(*(m_int*)MEM(SZ_INT),
               STRING(*(M_Object*)MEM(SZ_INT*2))))
MOUT_INIT(new3, openVirtualPort(STRING(*(M_Object*)MEM(SZ_INT))))

#undef MOUT_INIT

extern "C" {
GWION_IMPORT(RtMidi) {
  #define import_ctor(io)                                     \
  gwidoc(gwi, "open default device");                         \
  GWI_BB(gwi_func_ini(gwi, "auto", "new"))                    \
  GWI_BB(gwi_func_end(gwi, rtmidi##io##_new0, ae_flag_none))  \
                                                              \
  gwidoc(gwi, "open device at port n");                       \
  GWI_BB(gwi_func_ini(gwi, "auto", "new"))                    \
  GWI_BB(gwi_func_arg(gwi, "int", "n"))                       \
  GWI_BB(gwi_func_end(gwi, rtmidi##io##_new1, ae_flag_none))  \
                                                              \
  gwidoc(gwi, "open device at port n and set port name");     \
  GWI_BB(gwi_func_ini(gwi, "auto", "new"))                    \
  GWI_BB(gwi_func_arg(gwi, "int", "n"))                       \
  GWI_BB(gwi_func_arg(gwi, "string", "name"))                 \
  GWI_BB(gwi_func_end(gwi, rtmidi##io##_new2, ae_flag_none))  \
                                                              \
  gwidoc(gwi, "open virtual device");                         \
  GWI_BB(gwi_func_ini(gwi, "auto", "new"))                    \
  GWI_BB(gwi_func_arg(gwi, "string", "name"))                 \
  GWI_BB(gwi_func_end(gwi, rtmidi##io##_new3, ae_flag_none))  \

  #define import_static(io)                                     \
  gwidoc(gwi, "returns device name at port n");                 \
  GWI_BB(gwi_func_ini(gwi, "string", "name"))                   \
  GWI_BB(gwi_func_arg(gwi, "int", "port"))                      \
  GWI_BB(gwi_func_end(gwi, rtmidi##io##_name, ae_flag_static))  \
                                                                \
  gwidoc(gwi, "return a list of device names");                 \
  GWI_BB(gwi_func_ini(gwi, "string[]", "names"))                \
  GWI_BB(gwi_func_end(gwi, rtmidi##io##_names, ae_flag_static)) \
                                                                \
  gwidoc(gwi, "count devices");                                 \
  GWI_BB(gwi_func_ini(gwi, "int", "count"))                     \
  GWI_BB(gwi_func_end(gwi, rtmidi##io##_count, ae_flag_static)) \

  #define import_common(io) import_ctor(io); import_static(io);

  GWI_OB(gwi_struct_ini(gwi, (m_str)"RtMidi"));

  DECL_OB(const Type, t_rtmidiout, = gwi_class_ini(gwi, "Out", "int[0]"));
  SET_FLAG(t_rtmidiout, abstract | ae_flag_final);
  gwi_class_xtor(gwi, NULL, rtmidiout_dtor);
  t_rtmidiout->nspc->offset += SZ_INT;
  import_common(out);

  gwidoc(gwi, "send a midi message");
  GWI_BB(gwi_func_ini(gwi, "void", "send"))
  GWI_BB(gwi_func_end(gwi, rtmidiout_send, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

  DECL_OB(const Type, t_rtmidiin, = gwi_class_ini(gwi, "In", "Event"));
  SET_FLAG(t_rtmidiin, abstract | ae_flag_final);
  gwi_class_xtor(gwi, NULL, rtmidiin_dtor);
  t_rtmidiin->nspc->offset += sizeof(struct MidiIn);
  import_common(in);

  gwidoc(gwi, "read a midi message, return true if there's messages left");
  GWI_BB(gwi_func_ini(gwi, "bool", "read"))
  GWI_BB(gwi_func_arg(gwi, "&int", "status"))
  GWI_BB(gwi_func_arg(gwi, "&int", "data1"))
  GWI_BB(gwi_func_arg(gwi, "&int", "data2"))
  GWI_BB(gwi_func_end(gwi, rtmidiin_read, ae_flag_none))

  gwidoc(gwi, "ignore (or not) sysex, midiTime and MidiSense");
  GWI_BB(gwi_func_ini(gwi, "void", "ignore"))
  GWI_BB(gwi_func_arg(gwi, "bool", "sysex"))
  GWI_BB(gwi_func_arg(gwi, "bool", "midiTime"))
  GWI_BB(gwi_func_arg(gwi, "bool", "midiSense"))
  GWI_BB(gwi_func_end(gwi, rtmidiin_ignore, ae_flag_none))

  gwidoc(gwi, "set midi-thru");
  GWI_BB(gwi_func_ini(gwi, "void", "connect"))
  GWI_BB(gwi_func_arg(gwi, "Out", "out"))
  GWI_BB(gwi_func_end(gwi, rtmidiin_connect, ae_flag_none))

  gwidoc(gwi, "unset midi-thru");
  GWI_BB(gwi_func_ini(gwi, "void", "disconnect"))
  GWI_BB(gwi_func_arg(gwi, "Out", "out"))
  GWI_BB(gwi_func_end(gwi, rtmidiin_disconnect, ae_flag_none))


  GWI_BB(gwi_class_end(gwi))

  GWI_OB(gwi_struct_end(gwi));

  #undef import_static
  #undef import_ctor
  #undef import_common

  return GW_OK;
}
}
