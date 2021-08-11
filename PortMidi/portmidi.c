#include <unistd.h>
#include <portmidi.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"

typedef struct {
  PmStream* stream;
  THREAD_TYPE thread; // in only
  MUTEX_TYPE mutex; // in only
  MUTEX_TYPE bbq; // only needed for in
  struct M_Vector_ msg;
} MidiInfo;

typedef struct {
  m_uint status;
  m_uint data1;
  m_uint data2;
} MidiMsg;

ANN static void release_info(MemPool p, MidiInfo *info) {
  if(info->thread)
    pthread_cancel(info->thread);
  Pm_Close(info->stream);
}

static m_int o_pm_id;
static m_int o_pm_status;
static m_int o_pm_data1;
static m_int o_pm_data2;

//#define OUT(o)   (&*(MidiInfo*)(o->data + SZ_INT))
#define IN_INFO(o)   (&*(MidiInfo*)(o->data + SZ_INT))
#define ID(o)     *(m_int*)    (o->data + o_pm_id)
#define MSG(o) *(MidiMsg*)   (o->data + o_pm_status)
#define STATUS(o) *(m_uint*)   (o->data + o_pm_status)
#define DATA1(o) *(m_uint*)    (o->data + o_pm_data1)
#define DATA2(o) *(m_uint*)    (o->data + o_pm_data2)


// should be static
// and return deviceInfo
static MFUN(pm_name) {
  const PmDeviceInfo* info = Pm_GetDeviceInfo(ID(o));
  if(!info)
    *(m_uint*)RETURN = (m_uint)new_string(shred->info->mp, shred, "no device");
  else
    *(m_uint*)RETURN = (m_uint)new_string(shred->info->mp, shred, (m_str)info->name);
}

static SFUN(pm_count) {
  *(m_uint*)RETURN = Pm_CountDevices();
}

static SFUN(pm_error) {
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->mp, shred, (m_str)Pm_GetErrorText(*(m_int*)MEM(0)));
}

static MFUN(pm_close) {
  release_info(shred->info->mp, IN_INFO(o));
  ID(o) = -1;
  *(m_uint*)RETURN = 1;
}

static CTOR(pm_ctor) {
  ID(o) = -1;
}

static CTOR(pmin_ctor) {
  const MidiInfo *info = IN_INFO(o);
  m_vector_init(&info->msg, sizeof(PmMessage), 0);
}

static DTOR(pm_dtor) {
  if (IN_INFO(o)) // beware
    release_info(shred->info->mp,IN_INFO(o));
}

static DTOR(pmin_dtor) {
  const MidiInfo *info = IN_INFO(o);
  m_vector_release(&info->msg);
}

#include "shreduler_private.h"
static MFUN(midiout_open) {
  if(ID(o) > -1)
    release_info(shred->info->mp, IN_INFO(o));
  ID(o) = *(m_uint*)MEM(SZ_INT);
  MidiInfo* info = IN_INFO(o);
  Pm_OpenOutput(&info->stream, ID(o), 0, 0, NULL, NULL, 0);
  *(m_uint*)RETURN = 1;
}

// remove this?
static MFUN(midiout_send_self) {
  MidiInfo* info = IN_INFO(o);
  MidiMsg msg = MSG(o);
  *(m_uint*)RETURN = Pm_WriteShort(info->stream, 0,
    Pm_Message(msg.status, msg.data1, msg.data2));
}

static MFUN(midiout_send) {
  MidiInfo* info = IN_INFO(o);
  MidiMsg msg = *(MidiMsg*)MEM(SZ_INT);
  *(m_uint*)RETURN = Pm_WriteShort(info->stream, 0,
    Pm_Message(msg.status, msg.data1, msg.data2));
}

ANN static void* pm_recv(void* data) {
  const M_Object o = (M_Object)data;
  MidiInfo* info = IN_INFO(o);

  while(true) {
    while(Pm_Poll(info->stream) == 1) {

      PmEvent event;
      Pm_Read(info->stream, &event, 1);

      MUTEX_LOCK(info->mutex);
      m_vector_add(&info->msg, &event.message);
      MUTEX_UNLOCK(info->mutex);

      MUTEX_LOCK(info->bbq);
      broadcast(o);
      MUTEX_UNLOCK(info->bbq);

    }

    usleep(100000); // one millisecond
  }
  return NULL;
}

// should be the constructor
static MFUN(midiin_open) {
  ID(o) = *(m_uint*)MEM(SZ_INT);
  MidiInfo* info = IN_INFO(o);
  info->bbq = shred->info->vm->shreduler->mutex;
  Pm_OpenInput(&info->stream, ID(o), 0, 0, NULL, NULL);
  MUTEX_SETUP(info->mutex);
  THREAD_CREATE(info->thread, pm_recv, o);
}

static MFUN(midiin_recv) {
  MidiInfo* info = IN_INFO(o);
  MUTEX_LOCK(info->mutex);
  *(m_uint*)RETURN = m_vector_size(&info->msg) ? 1 : 0;
  MUTEX_UNLOCK(info->mutex);
}

static MFUN(midiin_read) {
  MidiInfo* info = IN_INFO(o);
  MUTEX_LOCK(info->mutex);
  const PmMessage pmsg = *(PmMessage*)(info->msg.ptr + ARRAY_OFFSET);
  m_vector_rem(&info->msg, 0);
  MUTEX_UNLOCK(info->mutex);
  MidiMsg *mmsg = &MSG(o);
  mmsg->status = Pm_MessageStatus(pmsg);
  mmsg->data1  = Pm_MessageData1(pmsg);
  mmsg->data2  = Pm_MessageData2(pmsg);
}

GWION_IMPORT(portmidi) {
  const Type t_portmidi = gwi_class_ini(gwi, "PortMidi", "Event");
  SET_FLAG(t_portmidi, abstract);
  gwi_class_xtor(gwi, pm_ctor, pm_dtor);
  t_portmidi->nspc->offset += sizeof(MidiInfo);

  gwi_item_ini(gwi,"int",  "id");
  o_pm_id = gwi_item_end(gwi, ae_flag_const, num, 0);

  GWI_BB(o_pm_id)
  gwi_item_ini(gwi,"int",  "status");
  o_pm_status  = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_pm_status)
  gwi_item_ini(gwi,"int",  "data1");
  o_pm_data1 = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_pm_data1)
  gwi_item_ini(gwi,"int",  "data2");
  o_pm_data2 = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_pm_data2)

  gwi_func_ini(gwi, "int", "count");
  GWI_BB(gwi_func_end(gwi, pm_count, ae_flag_static))

  gwi_func_ini(gwi, "string", "name");
  GWI_BB(gwi_func_end(gwi, pm_name, ae_flag_none))

  gwi_func_ini(gwi, "string", "error");
    gwi_func_arg(gwi, "int", "id");
  GWI_BB(gwi_func_end(gwi, pm_error, ae_flag_static))
  gwi_func_ini(gwi, "int", "close");
  GWI_BB(gwi_func_end(gwi, pm_close, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_class_ini(gwi, "MidiOut", "PortMidi"))
  gwi_func_ini(gwi, "int", "open");
    gwi_func_arg(gwi, "int", "id");
  GWI_BB(gwi_func_end(gwi, midiout_open, ae_flag_none))
  gwi_func_ini(gwi, "int", "send");
  GWI_BB(gwi_func_end(gwi, midiout_send_self, ae_flag_none))
  gwi_func_ini(gwi, "int", "send");
    gwi_func_arg(gwi, "int", "status");
    gwi_func_arg(gwi, "int", "data1");
    gwi_func_arg(gwi, "int", "data2");
  GWI_BB(gwi_func_end(gwi, midiout_send, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_class_ini(gwi, "MidiIn", "PortMidi"))
  gwi_class_xtor(gwi, pmin_ctor, pmin_dtor);
  gwi_func_ini(gwi, "int", "open");
    gwi_func_arg(gwi, "int", "id");
  GWI_BB(gwi_func_end(gwi, midiin_open, ae_flag_none))
  gwi_func_ini(gwi, "int", "recv");
  GWI_BB(gwi_func_end(gwi, midiin_recv, ae_flag_none))
  gwi_func_ini(gwi, "int", "read");
  GWI_BB(gwi_func_end(gwi, midiin_read, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

Pm_Initialize(); // to put in module
  return GW_OK;
}

// channels
// modules
// device info
// new
// remove msg
