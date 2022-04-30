#include <stdlib.h>
#include <string.h>
#include <portsmf/allegro.h>
#include "Gwion.hh"

static Type t_midifileev;
#define TYPE(o)  *(m_uint*)(o->data + o_midiev_type)
#define START(o) *(m_float*)(o->data + o_midiev_start)
#define PITCH(o) *(m_float*)(o->data + o_midiev_pitch)
#define LOUD(o)  *(m_float*)(o->data + o_midiev_loud)
#define END(o)   *(m_float*)(o->data + o_midiev_end)
#define DUR(o)   *(m_float*)(o->data + o_midiev_dur)
#define SEQ(o)   *(Alg_seq**)(o->data + o_midifile_seq)
m_int o_midifile_seq;
m_int o_midiev_type;
m_int o_midiev_start;
m_int o_midiev_end;
m_int o_midiev_dur;
m_int o_midiev_pitch;
m_int o_midiev_loud;
//static void ctor(M_Object o, VM_Shred shred);
//static void dtor(M_Object o, VM_Shred shred);
static CTOR(ctor);
static DTOR(dtor);

extern "C"
{
MFUN(midifile_open);
MFUN(midifile_tracks);
MFUN(midifile_track_len);
MFUN(midifile_event);
MFUN(midifile_add_track);
MFUN(midifile_add_note);
MFUN(midifile_write);
GWION_IMPORT(portsmf) {
  GWI_OB(gwi_class_ini(gwi, "MidiFileEv", "Event"))
	gwi_item_ini(gwi,"int", "type");
  o_midiev_type = gwi_item_end(gwi, ae_flag_const, num, 0);
  GWI_BB(o_midiev_type);
	gwi_item_ini(gwi,"float", "pitch");
  o_midiev_pitch = gwi_item_end(gwi, ae_flag_const, num, 0);
  GWI_BB(o_midiev_pitch);
	gwi_item_ini(gwi,"float", "loud");
  o_midiev_loud  = gwi_item_end(gwi,  ae_flag_const, num, 0);
  GWI_BB(o_midiev_loud);
	gwi_item_ini(gwi,"float", "start");
  o_midiev_start = gwi_item_end(gwi, ae_flag_const, num, 0);
  GWI_BB(o_midiev_start);
	gwi_item_ini(gwi,"float", "end");
  o_midiev_end   = gwi_item_end(gwi,   ae_flag_const, num, 0);
  GWI_BB(o_midiev_start);
	gwi_item_ini(gwi,"float", "dur");
  o_midiev_dur   = gwi_item_end(gwi,   ae_flag_const, num, 0);
  GWI_BB(o_midiev_dur);
  GWI_BB(gwi_class_end(gwi))

  GWI_OB(gwi_class_ini(gwi, "MidiFile", NULL))
  gwi_class_xtor(gwi, ctor, dtor);
	gwi_item_ini(gwi,"int", "@seq");
  o_midifile_seq = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_midifile_seq);
  gwi_func_ini(gwi, "void", "open");
    gwi_func_arg(gwi, "string", "filename");
    gwi_func_arg(gwi, "int", "smf");
  GWI_BB(gwi_func_end(gwi, midifile_open, ae_flag_none))
  gwi_func_ini(gwi, "int", "tracks");
  GWI_BB(gwi_func_end(gwi, midifile_tracks, ae_flag_none))

  gwi_func_ini(gwi, "int", "len");
    gwi_func_arg(gwi, "int", "track");
  GWI_BB(gwi_func_end(gwi, midifile_track_len, ae_flag_none))

  gwi_func_ini(gwi, "MidiFileEv", "event");
    gwi_func_arg(gwi, "int", "track");
    gwi_func_arg(gwi, "int", "event_number");
//    gwi_func_arg(gwi, "MidiFileEv", "event");
  GWI_BB(gwi_func_end(gwi, midifile_event, ae_flag_none))

  gwi_func_ini(gwi, "void", "add_track");
    gwi_func_arg(gwi, "int", "number");
  GWI_BB(gwi_func_end(gwi, midifile_add_track, ae_flag_none))

  gwi_func_ini(gwi, "int", "add_note");
    gwi_func_arg(gwi, "int", "track");
    gwi_func_arg(gwi, "MidiFileEv", "note");
  GWI_BB(gwi_func_end(gwi, midifile_add_note, ae_flag_none))

  gwi_func_ini(gwi, "void", "write");
    gwi_func_arg(gwi, "string", "filename");
  GWI_BB(gwi_func_end(gwi, midifile_write, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return 1;
}
}


static CTOR(ctor) { SEQ(o) = new Alg_seq(); }

static DTOR(dtor) { delete SEQ(o); }

MFUN(midifile_open)
{
  delete SEQ(o);
  SEQ(o) = new Alg_seq(STRING(*(M_Object*)MEM(SZ_INT)), *(m_uint*)MEM(SZ_INT), 0);
  Alg_seq* seq = SEQ(o);
  seq->convert_to_seconds();
}

MFUN(midifile_tracks)
{
  Alg_seq* seq = SEQ(o);
  *(m_uint*)RETURN = seq->tracks();
}

MFUN(midifile_add_track)
{
  Alg_seq* seq = SEQ(o);
  seq->add_track(*(m_uint*)MEM(SZ_INT));
}
MFUN(midifile_track_len)
{
  Alg_seq* seq = SEQ(o);
  *(m_uint*)RETURN = seq->track(*(m_uint*)MEM(SZ_INT))->length();

}
MFUN(midifile_event)
{
  Alg_seq* seq = SEQ(o);
  m_int track = *(m_uint*)MEM(SZ_INT);
  m_uint n     = *(m_uint*)MEM(SZ_INT*2);
  Alg_track* tr = seq->track(track);
  if(track < 0 || track >= seq->tracks()) {
    handle(shred, "PortSmfTrackError");
  }
  M_Object obj = new_object(shred->info->mp, t_midifileev);
  if(n < 0 || n >= tr->length())
      TYPE(obj) = 'e'; // error
  else
  {
    Alg_event* ev = tr[0][*(m_uint*)MEM(SZ_INT*2)];
    TYPE(obj) = ev->get_type();
    if(ev->get_type() == 'n')
    {
      PITCH(obj) = ev->get_pitch();
      LOUD(obj)  = ev->get_loud();
      START(obj) = ev->get_start_time() * shred->info->vm->bbq->si->sr;
      END(obj)   = ev->get_end_time()   * shred->info->vm->bbq->si->sr;
      DUR(obj)   = ev->get_duration()   * shred->info->vm->bbq->si->sr;
    }
  }
  *(m_uint*)RETURN = (m_uint)obj;
}

MFUN(midifile_add_note)
{
  Alg_seq* seq = SEQ(o);
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(TYPE(obj) != 'n')
    handle(0, (m_str)"not a note.");
  Alg_note* ev = new Alg_note();
  ev->pitch = PITCH(obj);
  ev->loud  = LOUD(obj);
  ev->dur   = DUR(obj);
  seq->add_event(ev, *(m_uint*)MEM(SZ_INT*2));
  Alg_track* tr = seq->track(*(m_uint*)MEM(SZ_INT*2));
  tr->set_start_time(ev, START(obj));
}

MFUN(midifile_write)
{
  Alg_seq* seq = SEQ(o);
  seq->write(STRING(*(M_Object*)MEM(SZ_INT)));

}
