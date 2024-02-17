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
#include "gwi.h"
#include "shreduler_private.h"

static MFUN(pm_info) {
  const PmDeviceInfo* info = Pm_GetDeviceInfo(*(m_int*)MEM(SZ_INT));
  if(!info) {
    xfun_handle(shred, "InvalidPortMidiInfo");
    return;
  }
  *(m_int*)   (o->data)            = info->structVersion;
  *(M_Object*)(o->data + SZ_INT)   = new_string(shred->info->vm->gwion, (m_str)info->interf);
  *(M_Object*)(o->data + SZ_INT*2) = new_string(shred->info->vm->gwion, (m_str)info->name);
  *(m_int*)   (o->data + SZ_INT*3) = info->input;
  *(m_int*)   (o->data + SZ_INT*4) = info->output;
  *(m_int*)   (o->data + SZ_INT*5) = info->opened;
  *(M_Object*)RETURN = o;
}


static SFUN(pm_count) {
  *(m_uint*)RETURN = Pm_CountDevices();
}

static MFUN(pm_channel) {
  Pm_SetChannelMask(*(PortMidiStream **)(o->data + SZ_INT), Pm_Channel(*(m_uint*)MEM(SZ_INT)));
}

static MFUN(pm_channels) {
  int chan = 0;
  M_Object arg = *(M_Object*)MEM(SZ_INT);
  for(m_uint i = 0; i < m_vector_size(ARRAY(arg)); i++) {
    chan |= Pm_Channel(*(m_uint*)(ARRAY(arg)->ptr + ARRAY_OFFSET + SZ_INT * i));
  }
  Pm_SetChannelMask(*(PortMidiStream **)(o->data + SZ_INT), chan);
}

#define OUT_INFO(o)  (*(PmStream**)(o->data + SZ_INT))

static DTOR(pmout_dtor) {
  Pm_Close(OUT_INFO(o));
}

static MFUN(midiout_new) {
  if(Pm_OpenOutput(&OUT_INFO(o), *(m_uint*)MEM(SZ_INT), 0, 0, NULL, NULL, 0)) {
    xfun_handle(shred, "InvalidMidiOut");
    return;
  }
  *(M_Object*)RETURN = o;
}

static MFUN(midiout_send) {
  *(m_uint*)RETURN = Pm_WriteShort(OUT_INFO(o), 0,
    Pm_Message(*(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2), *(m_int*)MEM(SZ_INT*3)));
}

typedef struct {
  PmStream* stream;
  THREAD_TYPE thread;
  gwtlock_t mutex;
  struct M_Vector_ msg;
} MidiIn;

#define IN_INFO(o)   (&*(MidiIn*)(o->data + SZ_INT))

ANN static void* pm_recv(void* data) {
  const M_Object o = (M_Object)data;
  MidiIn *const info = IN_INFO(o);

  while(true) {
    while(Pm_Poll(info->stream) == 1) {
      PmEvent event;
      Pm_Read(info->stream, &event, 1);
      gwt_lock(&info->mutex);
      m_vector_add(&info->msg, &event.message);
      gwt_unlock(&info->mutex);
      broadcast(o);
    }

  }
  return NULL;
}

static MFUN(midiin_new) {
  MidiIn* info = IN_INFO(o);
  if(Pm_OpenInput(&info->stream, *(m_uint*)MEM(SZ_INT), 0, 0, NULL, NULL)) {
    xfun_handle(shred, "InvalidMidiIn");
    return;
  }
  m_vector_init(&info->msg, sizeof(PmMessage), 0);
  gwt_lock_ini(&info->mutex);
  THREAD_CREATE(info->thread, pm_recv, o);
  *(M_Object*)RETURN = o;
}

static DTOR(pmin_dtor) {
  MidiIn *const info = IN_INFO(o);
  m_vector_release(&info->msg);
  if(info->thread) // find a better way
    pthread_cancel(info->thread);
  gwt_lock_end(&info->mutex);
  Pm_Close(info->stream);
}

static MFUN(midiin_read) {
  MidiIn *const info = IN_INFO(o);
  gwt_lock(&info->mutex);
  const m_uint sz = m_vector_size(&info->msg);
  const PmMessage pmsg = *(PmMessage*)(info->msg.ptr + ARRAY_OFFSET);
  if(sz) m_vector_rem(&info->msg, 0);
  gwt_unlock(&info->mutex);
  if(sz) {
    **(m_int**)MEM(SZ_INT)   = Pm_MessageStatus(pmsg);
    **(m_int**)MEM(SZ_INT*2) = Pm_MessageData1(pmsg);
    **(m_int**)MEM(SZ_INT*3) = Pm_MessageData2(pmsg);
    *(m_uint*)RETURN = true;
  } else
    *(m_uint*)RETURN = false;
}


GWMODINI(PortMidi) { Pm_Initialize(); return (void*)1; }
GWMODEND(PortMidi) { Pm_Terminate(); }

GWION_IMPORT(PortMidi) {
  gwidoc(gwi, "base MIDI class");
  const Type t_portmidi = gwi_class_ini(gwi, "PortMidi", "Event");
  SET_FLAG(t_portmidi, abstract);

  gwidoc(gwi, "set channel for an object");
  GWI_B(gwi_func_ini(gwi, "void", "channel"));
  GWI_B(gwi_func_arg(gwi, "int", "channels"));
  GWI_B(gwi_func_end(gwi, pm_channel, ae_flag_none));

  gwidoc(gwi, "set channels for an object");
  GWI_B(gwi_func_ini(gwi, "void", "channel"));
  GWI_B(gwi_func_arg(gwi, "int[]", "channels"));
  GWI_B(gwi_func_end(gwi, pm_channels, ae_flag_none));

  gwidoc(gwi, "count number of devices");
  GWI_B(gwi_func_ini(gwi, "int", "count"));
  GWI_B(gwi_func_end(gwi, pm_count, ae_flag_static));

    gwidoc(gwi, "Handle MIDI output");
    const Type t_info = gwi_class_ini(gwi, "Info", "Object");
    SET_FLAG(t_info, abstract);
    GWI_B(gwi_item_ini(gwi,"int",  "structVersion"));
    GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0));
    GWI_B(gwi_item_ini(gwi,"string",  "interf"));
    GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0));
    GWI_B(gwi_item_ini(gwi,"string",  "name"));
    GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0));
    GWI_B(gwi_item_ini(gwi,"int",  "input"));
    GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0));
    GWI_B(gwi_item_ini(gwi,"int",  "output"));
    GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0));
    GWI_B(gwi_item_ini(gwi,"int",  "opened"));
    GWI_B(gwi_item_end(gwi, ae_flag_const, num, 0));

    gwidoc(gwi, "constructor");
    GWI_B(gwi_func_ini(gwi, "auto", "new"));
    GWI_B(gwi_func_arg(gwi, "int", "id"));
    GWI_B(gwi_func_end(gwi, pm_info, ae_flag_none))

    GWI_B(gwi_class_end(gwi))

  GWI_B(gwi_class_end(gwi))

  gwidoc(gwi, "Handle MIDI output");
  DECL_B(const Type, t_midiout, = gwi_class_ini(gwi, "MidiOut", "PortMidi"));
  gwi_class_xtor(gwi, NULL, pmout_dtor);
  t_midiout->nspc->offset += sizeof(PmStream*);

    gwidoc(gwi, "constructor");
    GWI_B(gwi_func_ini(gwi, "auto", "new"));
    GWI_B(gwi_func_arg(gwi, "int", "device"));
    GWI_B(gwi_func_end(gwi, midiout_new, ae_flag_none))

    gwidoc(gwi, "send a MIDI message");
    GWI_B(gwi_func_ini(gwi, "int", "send"));
    GWI_B(gwi_func_arg(gwi, "int", "status"));
    GWI_B(gwi_func_arg(gwi, "int", "data1"));
    GWI_B(gwi_func_arg(gwi, "int", "data2"));
    GWI_B(gwi_func_end(gwi, midiout_send, ae_flag_none))

  GWI_B(gwi_class_end(gwi))

  gwidoc(gwi, "Handle MIDI input");
  DECL_B(const Type, t_midiin, = gwi_class_ini(gwi, "MidiIn", "PortMidi"));
  t_midiin->nspc->offset += sizeof(MidiIn);
  gwi_class_xtor(gwi, NULL, pmin_dtor);
  SET_FLAG(t_midiin, abstract);

  gwidoc(gwi, "constructor");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_arg(gwi, "int", "device"));
  GWI_B(gwi_func_end(gwi, midiin_new, ae_flag_none))

  gwidoc(gwi, "read a MIDI message");
  gwi_func_ini(gwi, "bool", "read");
    gwi_func_arg(gwi, "&int", "status");
    gwi_func_arg(gwi, "&int", "data1");
    gwi_func_arg(gwi, "&int", "data2");
  GWI_B(gwi_func_end(gwi, midiin_read, ae_flag_none))
  GWI_B(gwi_class_end(gwi))

  if(!get_module(gwi->gwion, "PortMidi"))
    CHECK_B(set_module(gwi->gwion, "PortMidi", GWMODINI_NAME(PortMidi)(gwi->gwion, NULL)));
  return true;
}

// channels
