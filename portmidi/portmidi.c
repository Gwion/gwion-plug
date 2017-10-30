/* TODO:
  put MSG only in midiin
*/

#include "defs.h"
//#include "vm.h"
#include "type.h"
//#include "lang.h"
#include "instr.h"
#include "import.h"

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
  pthread_mutex_t mutex;
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

#define STREAM(o) *(PmStream**)(o->data + o_pm_stream)
#define ID(o)     *(m_int*)    (o->data + o_pm_id)
#define STATUS(o) *(m_uint*)   (o->data + o_pm_status)
#define DATA1(o) *(m_uint*)    (o->data + o_pm_data1)
#define DATA2(o) *(m_uint*)    (o->data + o_pm_data2)
#define MSG(o) *(Vector*)      (o->data + o_pm_msg)

static MFUN(pm_name)
{
  const PmDeviceInfo* info = Pm_GetDeviceInfo(ID(o));
  if(!info)
    *(m_uint*)RETURN = (m_uint)new_String(shred, "no device");
  else
    *(m_uint*)RETURN = (m_uint)new_String(shred, (m_str)info->name);
}

static SFUN(pm_error)
{
  *(m_uint*)RETURN = (m_uint)new_String(shred, (m_str)Pm_GetErrorText(*(m_int*)MEM(SZ_INT)));
}

static MFUN(pm_close)
{
  release_info(ID(o), o);
  STREAM(o) = NULL;
  ID(o) = -1;
  *(m_uint*)RETURN = 1;
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
  ID(o) = *(m_uint*)MEM(SZ_INT);
  MidiInfo* info = get(shred->vm_ref, ID(o));
  vector_add(info->client, (vtype)o);
  if(!info->stream)
    Pm_OpenOutput(&info->stream, ID(o), 0, 0, NULL, NULL, 0);
  STREAM(o) = info->stream;
  *(m_uint*)RETURN = 1;
}

static MFUN(midiout_send_self)
{
  *(m_uint*)RETURN = Pm_WriteShort(STREAM(o), 0, 
    Pm_Message(STATUS(o), DATA1(o), DATA2(o)));
}

static MFUN(midiout_send)
{
  *(m_uint*)RETURN = Pm_WriteShort(STREAM(o), 0, 
    Pm_Message(*(m_uint*)MEM(SZ_INT), *(m_uint*)MEM(SZ_INT*2), *(m_uint*)MEM(SZ_INT*3)));
}

static void* pm_recv(void* data)
{
  m_uint i;
  PmEvent event;
  MidiInfo* info = (MidiInfo*)data;
  m_float now = info->vm->sp->pos;
  while(1)
  {
    while(Pm_Poll(info->stream) == 1)
    {
      Pm_Read(info->stream, &event, 1);
      for(i = 0; i < vector_size(info->client); i++)
      {
        M_Object o = (M_Object)vector_at(info->client, i);
        pthread_mutex_lock(&info->mutex);
        vector_add(MSG(o), (vtype)(m_uint)event.message);
        pthread_mutex_unlock(&info->mutex);
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
  ID(o) = *(m_uint*)MEM(SZ_INT);
  MidiInfo* info = get(shred->vm_ref, ID(o));
  if(!info->thread)
  {
    Pm_OpenInput(&info->stream, ID(o), 0, 0, NULL, NULL);
    pthread_mutex_init(&info->mutex, NULL);
    pthread_create(&info->thread, NULL, pm_recv, info);
  }
  vector_add(info->client, (vtype)o);
  STREAM(o) = info->stream;
}

static MFUN(midiin_recv)
{
  MidiInfo* info = get(shred->vm_ref, ID(o));
  pthread_mutex_lock(&info->mutex);
  *(m_uint*)RETURN = vector_size(MSG(o)) ? 1 : 0;
  pthread_mutex_unlock(&info->mutex);
}

static MFUN(midiin_read)
{
  MidiInfo* info = get(shred->vm_ref, ID(o));
  pthread_mutex_lock(&info->mutex);
  m_uint msg = (m_uint)vector_front(MSG(o));
  STATUS(o) = Pm_MessageStatus(msg);
  DATA1(o)  = Pm_MessageData1(msg);
  DATA2(o)  = Pm_MessageData2(msg);
  vector_rem(MSG(o), (vtype)0);
  pthread_mutex_unlock(&info->mutex);
}

IMPORT
{
  CHECK_BB(importer_class_ini(importer, &t_portmidi, pm_ctor, pm_dtor))
	importer_item_ini(importer,"int",  "@dummy");
   importer_item_end(importer, ae_flag_member, NULL);
	importer_item_ini(importer,"int",  "@stream");
  o_pm_stream = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_pm_stream)
	importer_item_ini(importer,"int",  "id");
  o_pm_id = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_pm_id)
	importer_item_ini(importer,"int",  "status");
  o_pm_status  = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_pm_status)
	importer_item_ini(importer,"int",  "data1");
  o_pm_data1 = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_pm_data1)
	importer_item_ini(importer,"int",  "data2");
  o_pm_data2 = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_pm_data2)
	importer_item_ini(importer,"int",  "@msg");
  o_pm_msg = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_pm_msg)
  importer_func_ini(importer, "string", "name", (m_uint)pm_name);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "string", "error", (m_uint)pm_error);
    importer_func_arg(importer, "int", "id");
  CHECK_BB(importer_func_end(importer, ae_flag_static))
  importer_func_ini(importer, "int", "close", (m_uint)pm_close);
  CHECK_BB(importer_func_end(importer, 0))
  CHECK_BB(importer_class_end(importer))

  CHECK_BB(importer_class_ini(importer, &t_midiout, NULL, NULL))
  importer_func_ini(importer, "int", "open", (m_uint)midiout_open);
    importer_func_arg(importer, "int", "id");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "send", (m_uint)midiout_send_self);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "send", (m_uint)midiout_send);
    importer_func_arg(importer, "int", "status");
    importer_func_arg(importer, "int", "data1");
    importer_func_arg(importer, "int", "data2");
  CHECK_BB(importer_func_end(importer, 0))
  CHECK_BB(importer_class_end(importer))

  CHECK_BB(importer_class_ini(importer, &t_midiin, NULL, NULL))
  importer_func_ini(importer, "int", "open", (m_uint)midiin_open);
    importer_func_arg(importer, "int", "id");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "recv", (m_uint)midiin_recv);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "read", (m_uint)midiin_read);
  CHECK_BB(importer_func_end(importer, 0))
  CHECK_BB(importer_class_end(importer))

  return 1;
}
