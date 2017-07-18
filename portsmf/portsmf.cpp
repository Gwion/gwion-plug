#include <stdlib.h>
#include <string.h>
#include <portsmf/allegro.h>
#include "Gwion.hpp"

#define TYPE(o)  *(m_uint*)(o->data + o_midiev_type)
#define START(o) *(m_float*)(o->data + o_midiev_start)
#define PITCH(o) *(m_float*)(o->data + o_midiev_pitch)
#define LOUD(o)  *(m_float*)(o->data + o_midiev_loud)
#define END(o)   *(m_float*)(o->data + o_midiev_end)
#define DUR(o)   *(m_float*)(o->data + o_midiev_dur)
#define SEQ(o)   *(Alg_seq**)(o->data + o_midifile_seq)
struct Type_ t_midifileev = { (m_str)"MidiFileEv", SZ_INT, &t_object };
struct Type_ t_midifile = { (m_str)"MidiFile",  SZ_INT, &t_object };
m_int o_midifile_seq;
m_int o_midiev_type;
m_int o_midiev_start;
m_int o_midiev_end;
m_int o_midiev_dur;
m_int o_midiev_pitch;
m_int o_midiev_loud;
static void ctor(M_Object o, VM_Shred shred);
static void dtor(M_Object o, VM_Shred shred);

extern "C"
{
MFUN(midifile_open);
MFUN(midifile_tracks);
MFUN(midifile_track_len);
MFUN(midifile_event);
MFUN(midifile_add_track);
MFUN(midifile_add_note);
MFUN(midifile_write);

m_bool import(Env env)
{
  DL_Func fun;
  CHECK_BB(import_class_begin(env, &t_midifileev, env->global_nspc, NULL, NULL))
  o_midiev_type = import_var(env, "int", "type",   ae_flag_const, NULL);
  CHECK_BB(o_midiev_type);
  o_midiev_pitch = import_var(env, "float", "pitch", ae_flag_const, NULL);
  CHECK_BB(o_midiev_pitch);
  o_midiev_loud  = import_var(env, "float", "loud",  ae_flag_const, NULL);
  CHECK_BB(o_midiev_loud);
  o_midiev_start = import_var(env, "float", "start", ae_flag_const, NULL);
  CHECK_BB(o_midiev_start);
  o_midiev_end   = import_var(env, "float", "end",   ae_flag_const, NULL);
  CHECK_BB(o_midiev_start);
  o_midiev_dur   = import_var(env, "float", "dur",   ae_flag_const, NULL);
  CHECK_BB(o_midiev_dur);
  CHECK_BB(import_class_end(env))

  CHECK_BB(import_class_begin(env, &t_midifile, env->global_nspc, ctor, dtor))
  o_midifile_seq = import_var(env, "int", "@seq", ae_flag_member, NULL);
  CHECK_BB(o_midifile_seq);
  dl_func_init(&fun, "void", "open", (m_uint)midifile_open);
    dl_func_add_arg(&fun, "string", "filename");
    dl_func_add_arg(&fun, "int", "smf");
  CHECK_BB(import_fun(env, &fun, 0))
  dl_func_init(&fun, "int", "tracks", (m_uint)midifile_tracks);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "len", (m_uint)midifile_track_len);
    dl_func_add_arg(&fun, "int", "track");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "MidiFileEv", "event", (m_uint)midifile_event);
    dl_func_add_arg(&fun, "int", "track");
    dl_func_add_arg(&fun, "int", "event_number");
//    dl_func_add_arg(&fun, "MidiFileEv", "event");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "void", "add_track", (m_uint)midifile_add_track);
    dl_func_add_arg(&fun, "int", "number");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "add_note", (m_uint)midifile_add_note);
    dl_func_add_arg(&fun, "int", "track");
    dl_func_add_arg(&fun, "MidiFileEv", "note");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "void", "write", (m_uint)midifile_write);
    dl_func_add_arg(&fun, "string", "filename");
  CHECK_BB(import_fun(env, &fun, 0))
  CHECK_BB(import_class_end(env))
  return 1;
}
}


static void ctor(M_Object o, VM_Shred shred)
{
  SEQ(o) = new Alg_seq();
}

static void dtor(M_Object o, VM_Shred shred)
{
  delete SEQ(o);
}

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
  m_uint track = *(m_uint*)MEM(SZ_INT);
  m_uint n     = *(m_uint*)MEM(SZ_INT*2);
//  if(track < 0 || track >= seq->tracks())
//    exit(12);
  Alg_track* tr = seq->track(track);
//  M_Object obj = *(M_Object*)MEM(SZ_INT*3);
  M_Object obj = new_M_Object(shred);
  initialize_object(obj, &t_midifileev);
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
      START(obj) = ev->get_start_time() * shred->vm_ref->bbq->sp->sr;
      END(obj)   = ev->get_end_time()   * shred->vm_ref->bbq->sp->sr;
      DUR(obj)   = ev->get_duration()   * shred->vm_ref->bbq->sp->sr;
    }
  }
  *(m_uint*)RETURN = (m_uint)obj;
}

MFUN(midifile_add_note)
{
  Alg_seq* seq = SEQ(o);
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(TYPE(obj) != 'n')
  {
    err_msg(INSTR_, 0, "not a note.");
    shred->me = NULL;
    return;
  }
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
