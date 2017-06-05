/* TODO:
  put MSG only in midiin
*/

#include "defs.h"
#include "vm.h"
#include "lang.h"
#include "import.h"
#include "shreduler.h"

#include <portmidi.h>
#include <pthread.h>
#include <unistd.h>

struct Type_ t_portmidi = { "PortMidi", SZ_INT, &t_event };
struct Type_ t_midiout  = { "MidiOut",  SZ_INT, &t_portmidi };
struct Type_ t_midiin   = { "MidiIn",   SZ_INT, &t_portmidi };

static Map map = NULL;
typedef struct
{
  m_float last;
  PmStream* stream;
  VM* vm;
  Vector client;
  pthread_t thread;
} MidiInfo;

static MidiInfo* get(VM* vm, m_uint i)
{
  if(!map)
  {
    map = new_map();
    Pm_Initialize();
  }
  MidiInfo* info = (MidiInfo*)map_get(map, (vtype)i+1);
  if(!info)
  {
    info = malloc(sizeof(MidiInfo));
    info->stream = NULL;
    info->client = new_vector();
    info->thread = 0;
    info->vm = vm;
    map_set(map, i+1, (vtype)info);
  }
  return info;
}

static void release_info(m_uint i, M_Object o)
{
  MidiInfo* info = (MidiInfo*)map_get(map, (vtype)i+i);
  if(info)
  {
    vector_rem(info->client, vector_find(info->client, (vtype)o));
    if(!vector_size(info->client))
    {
      if(info->thread)
        pthread_cancel(info->thread);
      Pm_Close(info->stream);
      free_vector(info->client);
      free(info);
    }
  }
  if(!map_size(map))
  {
    free_map(map);
    map = NULL;
    Pm_Terminate();
  }
}

static m_uint active = 0;
static m_int o_pm_stream;
static m_int o_pm_id;
static m_int o_pm_status;
static m_int o_pm_data1;
static m_int o_pm_data2;
static m_int o_pm_msg;

#define STREAM(o) *(PmStream**)(o->d.data + o_pm_stream)
#define ID(o)     *(m_int*)    (o->d.data + o_pm_id)
#define STATUS(o) *(m_uint*)   (o->d.data + o_pm_status)
#define DATA1(o) *(m_uint*)    (o->d.data + o_pm_data1)
#define DATA2(o) *(m_uint*)    (o->d.data + o_pm_data2)
#define MSG(o) *(Vector*)      (o->d.data + o_pm_msg)

static MFUN(pm_name)
{
  const PmDeviceInfo* info = Pm_GetDeviceInfo(ID(o));
  if(!info)
    RETURN->d.v_uint = (m_uint)new_String(shred, "no device");
  else
    RETURN->d.v_uint = (m_uint)new_String(shred, (m_str)info->name);
}

static SFUN(pm_error)
{
  RETURN->d.v_uint = (m_uint)new_String(shred, (m_str)Pm_GetErrorText(*(m_int*)(shred->mem + SZ_INT)));
}

static MFUN(pm_close)
{
  release_info(ID(o), o);
  STREAM(o) = NULL;
  ID(o) = -1;
  RETURN->d.v_uint = 1;
}

static CTOR(pm_ctor)
{
  ID(o) = -1;
  MSG(o) = new_vector();
}

static DTOR(pm_dtor)
{
  free_vector(MSG(o));
  if(get(shred->vm_ref, ID(o)))
    release_info(ID(o), o);
}

static MFUN(midiout_open)
{
  if(ID(o) > -1)
    release_info(ID(o), o);
  ID(o) = *(m_uint*)(shred->mem + SZ_INT);
  MidiInfo* info = get(shred->vm_ref, ID(o));
  vector_add(info->client, (vtype)o);
  if(!info->stream)
    Pm_OpenOutput(&info->stream, ID(o), 0, 0, NULL, NULL, 0);
  STREAM(o) = info->stream;
  RETURN->d.v_uint = 1;
}

static MFUN(midiout_send_self)
{
  RETURN->d.v_uint = Pm_WriteShort(STREAM(o), 0, 
    Pm_Message(STATUS(o), DATA1(o), DATA2(o)));
}

static MFUN(midiout_send)
{
  RETURN->d.v_uint = Pm_WriteShort(STREAM(o), 0, 
    Pm_Message(*(m_uint*)(shred->mem + SZ_INT), *(m_uint*)(shred->mem + SZ_INT*2), *(m_uint*)(shred->mem + SZ_INT*3)));
}

static void* pm_recv(void* data)
{
  m_uint i;
  PmEvent event;
  MidiInfo* info = (MidiInfo*)data;
  m_float now = get_now(info->vm->shreduler);
  while(1)
  {
    while(Pm_Poll(info->stream) == 1)
    {
      Pm_Read(info->stream, &event, 1);
      for(i = 0; i < vector_size(info->client); i++)
      {
        M_Object o = (M_Object)vector_at(info->client, i);
        vector_add(MSG(o), (vtype)(m_uint)event.message);
        if(info->last != now);
        broadcast(o);
      }
    }
    info->last = now;
    usleep(100000); // one millisecond
  }
  return NULL;
}

static MFUN(midiin_open)
{
  ID(o) = *(m_uint*)(shred->mem + SZ_INT);
  MidiInfo* info = get(shred->vm_ref, ID(o));
  if(!info->thread)
  {
    Pm_OpenInput(&info->stream, ID(o), 0, 0, NULL, NULL);
    pthread_create(&info->thread, NULL, pm_recv, info);
  }
  vector_add(info->client, (vtype)o);
  STREAM(o) = info->stream;
}

static MFUN(midiin_recv)
{
  RETURN->d.v_uint = vector_size(MSG(o)) ? 1 : 0;
}

static MFUN(midiin_read)
{
  m_uint msg = (m_uint)vector_front(MSG(o));
  STATUS(o) = Pm_MessageStatus(msg);
  DATA1(o)  = Pm_MessageData1(msg);
  DATA2(o)  = Pm_MessageData2(msg);
  vector_rem(MSG(o), (vtype)0);
}

IMPORT
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_portmidi))
  CHECK_BB(import_class_begin(env, &t_portmidi, env->global_nspc, pm_ctor, pm_dtor))
   import_mvar(env, "int",  "@dummy",   0, 0, "place for midi stream");
  o_pm_stream = import_mvar(env, "int",  "@stream",   0, 0, "place for midi stream");
  CHECK_BB(o_pm_stream)
  o_pm_id = import_mvar(env, "int",  "id",   0, 0, "place for midi stream");
  CHECK_BB(o_pm_id)
  o_pm_status  = import_mvar(env, "int",  "status",   0, 0, "place for midi stream");
  CHECK_BB(o_pm_status)
  o_pm_data1 = import_mvar(env, "int",  "data1",   0, 0, "place for midi stream");
  CHECK_BB(o_pm_data1)
  o_pm_data2 = import_mvar(env, "int",  "data2",   0, 0, "place for midi stream");
  CHECK_BB(o_pm_data2)
  o_pm_msg = import_mvar(env, "int",  "@msg",   0, 0, "place for midi event vector");
  CHECK_BB(o_pm_msg)
  fun = new_dl_func("string", "name", (m_uint)pm_name);
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("string", "error", (m_uint)pm_error);
    dl_func_add_arg(fun, "int", "id");
  CHECK_OB(import_sfun(env, fun))
  fun = new_dl_func("int", "close", (m_uint)pm_close);
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_midiout))
  CHECK_BB(import_class_begin(env, &t_midiout, env->global_nspc, NULL, NULL))
  fun = new_dl_func("int", "open", (m_uint)midiout_open);
    dl_func_add_arg(fun, "int", "id");
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("int", "send", (m_uint)midiout_send_self);
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("int", "send", (m_uint)midiout_send);
    dl_func_add_arg(fun, "int", "status");
    dl_func_add_arg(fun, "int", "data1");
    dl_func_add_arg(fun, "int", "data2");
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_midiin))
  CHECK_BB(import_class_begin(env, &t_midiin, env->global_nspc, NULL, NULL))
  fun = new_dl_func("int", "open", (m_uint)midiin_open);
    dl_func_add_arg(fun, "int", "id");
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("int", "recv", (m_uint)midiin_recv);
  CHECK_OB(import_mfun(env, fun))
  fun = new_dl_func("int", "read", (m_uint)midiin_read);
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(import_class_end(env))

  return 1;
}
