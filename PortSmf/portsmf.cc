#include <stdlib.h>
#include <string.h>
#include <portsmf/allegro.h>
#include "Gwion.hh"

static Type t_midifileev;
#define TYPE(o)  *(m_uint*)(o->data + SZ_INT)
#define PITCH(o) *(m_float*)(o->data + SZ_INT*2)
#define LOUD(o)  *(m_float*)(o->data + SZ_INT*3)
#define START(o) *(m_float*)(o->data + SZ_INT*4)
#define END(o)   *(m_float*)(o->data + SZ_INT*5)
#define DUR(o)   *(m_float*)(o->data + SZ_INT*6)
#define SEQ(o)   *(Alg_seq**)(o->data)
static CTOR(ctor);
static DTOR(dtor);

extern "C"
{
static MFUN(midifile_open);
static MFUN(midifile_tracks);
static MFUN(midifile_track_len);
static MFUN(midifile_event);
static MFUN(midifile_add_track);
static MFUN(midifile_add_note);
static MFUN(midifile_write);
GWION_IMPORT(portsmf) {
  GWI_OB(gwi_class_ini(gwi, "MidiFileEv", "Event"))
	gwi_item_ini(gwi,"int", "type");
  gwi_item_end(gwi, ae_flag_const, num, 0);
	gwi_item_ini(gwi,"float", "pitch");
  gwi_item_end(gwi, ae_flag_const, num, 0);
	gwi_item_ini(gwi,"float", "loud");
  gwi_item_end(gwi,  ae_flag_const, num, 0);
	gwi_item_ini(gwi,"float", "start");
  gwi_item_end(gwi, ae_flag_const, num, 0);
	gwi_item_ini(gwi,"float", "end");
  gwi_item_end(gwi,   ae_flag_const, num, 0);
	gwi_item_ini(gwi,"float", "dur");
  gwi_item_end(gwi,   ae_flag_const, num, 0);
  GWI_BB(gwi_class_end(gwi))

  GWI_OB(gwi_class_ini(gwi, "MidiFile", NULL))
  gwi_class_xtor(gwi, ctor, dtor);
	gwi_item_ini(gwi,"int", "@seq");
  gwi_item_end(gwi, ae_flag_none, num, 0);
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

static MFUN(midifile_open)
{
  delete SEQ(o);
  SEQ(o) = new Alg_seq(STRING(*(M_Object*)MEM(SZ_INT)), *(m_uint*)MEM(SZ_INT), 0);
  Alg_seq* seq = SEQ(o);
  seq->convert_to_seconds();
}

static MFUN(midifile_tracks)
{
  Alg_seq* seq = SEQ(o);
  *(m_uint*)RETURN = seq->tracks();
}

static MFUN(midifile_add_track)
{
  Alg_seq* seq = SEQ(o);
  seq->add_track(*(m_uint*)MEM(SZ_INT));
}
static MFUN(midifile_track_len)
{
  Alg_seq* seq = SEQ(o);
  *(m_uint*)RETURN = seq->track(*(m_uint*)MEM(SZ_INT))->length();

}
static MFUN(midifile_event)
{
  Alg_seq* seq = SEQ(o);
  m_int track = *(m_uint*)MEM(SZ_INT);
  m_int n     = *(m_int*)MEM(SZ_INT*2);
  Alg_track* tr = seq->track(track);
  if(track < 0 || track >= seq->tracks()) {
    xfun_handle(shred, SZ_INT*3, (m_str)"PortSmfTrackError");
    return;
  }
  M_Object obj = new_object(shred->info->mp, t_midifileev);
  if(n < 0 || (int)n >= tr->length())
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

static MFUN(midifile_add_note)
{
  Alg_seq* seq = SEQ(o);
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(TYPE(obj) != 'n')
    xfun_handle(shred, SZ_INT*3, (m_str)"not a note.");
  Alg_note* ev = new Alg_note();
  ev->pitch = PITCH(obj);
  ev->loud  = LOUD(obj);
  ev->dur   = DUR(obj);
  seq->add_event(ev, *(m_uint*)MEM(SZ_INT*2));
  Alg_track* tr = seq->track(*(m_uint*)MEM(SZ_INT*2));
  tr->set_start_time(ev, START(obj));
}

static MFUN(midifile_write)
{
  Alg_seq* seq = SEQ(o);
  seq->write(STRING(*(M_Object*)MEM(SZ_INT)));

}
