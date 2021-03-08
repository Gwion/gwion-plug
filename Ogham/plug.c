#include <math.h>
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
#include "ugen.h"
#include "array.h"
#include "gwi.h"
#include "traverse.h"
#include "emit.h"
#include "specialid.h"
#include "shreduler_private.h"
#include "gwi.h"

#include "ogham.h"

#define OGH(o)  (*(ogh_music_t**)(o->data))

// maybe we could just use a pointer
#define PLAYER_POLYPHONY(o) *(m_uint*)(o->data + SZ_INT)
#define PLAYER_CURRENT(o) *(m_uint*)(o->data + SZ_INT*2)
#define PLAYER_NOTES(o) *(M_Vector*)(o->data + SZ_INT*3)
#define PLAYER_RUNTIME(o) *(Runtime**)(o->data + SZ_INT*4)
#define PLAYER_NOW(o) *(m_uint*)(o->data + SZ_INT*5)
#define PLAYER_DUR(o) *(m_float*)(o->data + SZ_INT*6)

typedef struct {
    m_uint  index;
    m_float freq;
    m_float volume;
    m_float real_duration;
    m_float real_offset;
} ogh_adjusted_note_t;

#define ARRAY_NOTE(a, i)  ((ogh_adjusted_note_t*)(ARRAY_PTR((a)) + ((i) * sizeof(ogh_adjusted_note_t))))

static double note2freq(int note) {
    int a = 440; //frequency of A (common value is 440Hz)
    return pow(2, (note - 69) / 12.0) * a;
}

static CTOR(ogham_ctor) {
  OGH(o) = mp_calloc2(shred->info->vm->gwion->mp, sizeof(ogh_music_t));
  ogh_music_init(OGH(o));
}

static DTOR(ogham_dtor) {
  mp_free2(shred->info->vm->gwion->mp, sizeof(ogh_music_t) + (OGH(o)->length)*sizeof(ogh_packed_t), OGH(o));
}

#define NOTE(o) (*(ogh_note_t*)(o->data))

struct Voice {
  VM_Shred shred;
  M_Object note;
};

typedef struct Runtime_{
  VM_Code code;
  struct Voice voice[];
} Runtime;

static DTOR(player_dtor) {
  free_m_vector(shred->info->vm->gwion->mp, PLAYER_NOTES(o));
  Runtime *runtime = PLAYER_RUNTIME(o);
  for(m_uint i = 0; i < PLAYER_POLYPHONY(o); i++) {
shreduler_remove(shred->tick->shreduler, runtime->voice[i].shred, 0);
    free_vm_shred(runtime->voice[i].shred);
    release(runtime->voice[i].note, shred);
  }
  vmcode_remref(runtime->code, shred->info->vm->gwion);
  mp_free2(shred->info->vm->gwion->mp, sizeof(Runtime) + PLAYER_POLYPHONY(o) * sizeof(struct Voice),
     PLAYER_RUNTIME(o));
}

typedef struct {
  M_Vector array;
  m_uint idx;
  m_float tempo;
} Decoder;
#define DEFAULT_TEMPO (1.0/64.0)

const uint note_table[] = { 21, 23, 12, 14, 16, 17, 19 };
static int ogh_note_midi(ogh_note_t *note) {
if(note->name < 'A')return 0;
  return note_table[note->name - 'A'];
}

static inline void adjust(ogh_adjusted_note_t *dec, ogh_note_t* n, m_float tempo) {
  dec->real_offset = tempo * n->offset;
  dec->real_duration = tempo * n->duration;
  const uint midi = ogh_note_midi(n) + n->is_sharp + (n->octave + 1) *12;
  dec->freq = note2freq(midi);
  dec->volume = n->volume / 128.0;
}

void dbg_on_note(void *data, ogh_note_t* n) {
  Decoder *dec = (Decoder*)data;
  adjust(ARRAY_NOTE(dec->array, dec->idx++), n, dec->tempo);
}

void dbg_on_tempo_change(void *data, ogh_tc_dir_t* tc_dir) {
  Decoder *dec = (Decoder*)data;
  dec->tempo = (double)tc_dir->numer / (double)tc_dir->denom;
}

static int compare(const void *a, const void *b) {
  return ((ogh_adjusted_note_t*)b)->real_offset <
         ((ogh_adjusted_note_t*)a)->real_offset;
}

static int compare_end(const void *a, const void *b) {
  return *(double*)b < *(double*)a;
}

static MFUN(ogh_tc) {
  ogh_tempo_change(OGH(o), *(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2));
}


#define note_func(member, type) \
static MFUN(note_##member##_get) {\
  *(m_uint*)RETURN = NOTE(o).member; } \
static MFUN(note_##member##_set) { \
  NOTE(o).member = (type)(*(m_uint*)RETURN = *(m_int*)MEM(SZ_INT));\
}

// TODO: add range checking
note_func(offset, uint32_t)
note_func(duration, uint32_t)
note_func(volume, uint8_t)
note_func(octave, uint8_t)
note_func(name, char)
note_func(is_sharp, bool)

static MFUN(note_name_str_set) {
  const M_Object arg = *(M_Object*)MEM(SZ_INT);
  const m_str str = STRING(arg);
  const size_t sz = strlen(str);
  if(sz == 0 || sz > 2 || *str < 'A' || *str > 'G')
    Except (shred, "invalid note name")
  NOTE(o).name = *str;
  if(sz == 2) {
    if(str[1] == 'b')
      --NOTE(o).name;
    else if(str[1] != '#')
      Except (shred, "invalid note alteration")
    NOTE(o).is_sharp = true;
  }
  *(M_Object*)RETURN = arg;
}

static INSTR(ogham_add_notes) {
  POP_REG(shred, SZ_INT);
  M_Object ogh = *(M_Object*)REG(-SZ_INT);
  M_Object not = *(M_Object*)REG(0);
  const M_Vector array = ARRAY(not);
  const m_uint sz = m_vector_size(array);
  const uint32_t len = OGH(ogh)->length;
  OGH(ogh) = mp_realloc(shred->info->vm->gwion->mp, OGH(ogh),
     sizeof(ogh_music_t) + len*sizeof(ogh_packed_t), sizeof(ogh_music_t) + (len+sz)*sizeof(ogh_packed_t));
  ogh_note_t notes[sz];
  for(m_uint i = 0; i < sz; i++) {
    const M_Object o = *(M_Object*)(ARRAY_PTR((array)) + ((i) * SZ_INT));
    notes[i] = NOTE(o);
  }
  ogh_music_add_notes(OGH(ogh), sz, notes);
}

static Type get_player_type(const Gwion gwion, Type t) {
  const Type t_ugen = gwion->type[et_ugen];
  while(t->info->parent != t_ugen)t = t->info->parent;
  return t;
}

static OP_CHECK(opck_player_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  const Type t = actual_type(env->gwion, call->func->type);
  const Type player = get_player_type(env->gwion, t);
  if(!call->args || call->args->next ||
     !check_exp(env, call->args) || isa(call->args->type, player->info->owner_class) < 0)
    ERR_N(call->func->pos, "Ogham.Player constructor requires one "
         "and only one 'Ogham' argument")
  return t; // return event?
}

static void launch_notes(const M_Object o, ogh_adjusted_note_t *note) {
  const Runtime *runtime = PLAYER_RUNTIME(o);
  const VM_Shred shred = runtime->voice[note->index].shred;
//shred->code = runtime->code;
  shred->pc = 0;
  ++shred->info->me->ref;
  const M_Object player_note = runtime->voice[note->index].note;
  memcpy(player_note->data, note, sizeof(ogh_adjusted_note_t) - sizeof(m_float) * 2);
  (*(ogh_adjusted_note_t*)player_note->data).real_duration = note->real_duration * PLAYER_DUR(o) -1;
  *(M_Object*)(shred->mem) = o;
  *(M_Object*)(shred->mem + SZ_INT) = player_note;
  shredule(shred->info->vm->shreduler, shred, GWION_EPSILON);
}

static TICK(player_tick) {
  const M_Object o = u->module.gen.data;
  const M_Vector array = PLAYER_NOTES(o);
  const m_uint sz = m_vector_size(array);
  const m_float now = PLAYER_NOW(o);
  m_uint i = PLAYER_CURRENT(o);
  while(i < sz) {
    ogh_adjusted_note_t *note = ARRAY_NOTE(array, i);
    if((note->real_offset * PLAYER_DUR(o)) <= now) {
      ++PLAYER_CURRENT(o);
      launch_notes(o, note);
    } else
      break;
    i++;
  }
  PLAYER_NOW(o) += 1;
}

static m_uint get_polyphony(MemPool mp, M_Vector notes) {
  m_int count = 1, max = 0;
  const m_uint sz = m_vector_size(notes);
  if(sz)
  for(m_uint i = 0; i < sz-1; i++) {
    ogh_adjusted_note_t *const note = ARRAY_NOTE(notes, i);
    const double end = note->real_offset + note->real_duration;
    for(m_uint j = 0; j < sz; j++) {
      const ogh_adjusted_note_t *other = ARRAY_NOTE(notes, j);
      if(note == other)
        continue;
      if(end <= other->real_offset) {
        --count;
        break;
      }
    }
    note->index = count;
    if(++count > max)
      max = count;
  }
  return max;
}

static INSTR(PlayerCtor) {
  POP_REG(shred, SZ_INT);
  const Gwion gwion = shred->info->vm->gwion;
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const M_Vector array = new_m_vector(gwion->mp, sizeof(ogh_adjusted_note_t), OGH(o)->length);
  Decoder dec = { .array=array, .tempo=DEFAULT_TEMPO };
  ogh_music_decode(OGH(o), dbg_on_note, NULL, &dec);
  qsort(ARRAY_PTR(dec.array), m_vector_size(dec.array), sizeof(ogh_adjusted_note_t), compare);
  const Type t = (Type)instr->m_val;
  const M_Object ret = new_object(gwion->mp, shred, t);
  UGEN(ret) = new_UGen(shred->info->mp);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret));
  ugen_ini(shred->info->vm->gwion, UGEN(ret), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret), player_tick, ret, 0);
  PLAYER_NOTES(ret) = array;
  const m_uint polyphony = PLAYER_POLYPHONY(ret) = get_polyphony(gwion->mp, array);
  PLAYER_DUR(ret) = shred->info->vm->bbq->si->sr;
  PLAYER_NOW(ret) = shred->tick->shreduler->bbq->pos;

  Runtime * runtime = PLAYER_RUNTIME(ret) = (Runtime*)_mp_malloc(gwion->mp, sizeof(Runtime) +
    polyphony * sizeof(struct Voice));
  const Func play = (Func)vector_at((Vector)&t->nspc->info->vtable, 6);
  runtime->code = vmcode_callback(gwion->mp, play->code);
  const Type note_type = (Type)map_at(&((Type)instr->m_val2)->nspc->info->type->map, 0);
  for(m_uint i = 0; i < polyphony; i++) {
    runtime->voice[i].shred = new_vm_shred(gwion->mp, runtime->code);
    vm_add_shred(gwion->vm, runtime->voice[i].shred);
    shreduler_remove(gwion->vm->shreduler, runtime->voice[i].shred, 0);
    runtime->voice[i].note = new_object(gwion->mp, shred, note_type);
  }
  *(M_Object*)REG(-SZ_INT) = ret;
  const VM_Code ctor_code = t->nspc->pre_ctor;
  register const m_uint push = SZ_INT + *(m_uint*)(shred->mem-SZ_INT);
  shred->mem += push;
  *(m_uint*)  shred->mem = push; shred->mem += SZ_INT;
  *(VM_Code*) shred->mem = shred->code; shred->mem += SZ_INT;
  *(m_uint*)  shred->mem = shred->pc; shred->mem += SZ_INT;
  *(m_uint*) shred->mem = ctor_code->stack_depth; shred->mem += SZ_INT;
  shred->code = ctor_code;
  shred->pc = 0;
  *(M_Object*)(shred->mem) = ret;
}

static OP_EMIT(opem_player_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  const Instr instr = emit_add_instr(emit, PlayerCtor);
  const Type t = exp_self(call)->type;
  instr->m_val = (m_uint)t;
  instr->m_val2 = (m_uint)get_player_type(emit->gwion, t);
  return GW_OK;
}

static SFUN(ogh_open) {
  const m_str filename = STRING(*(M_Object*)MEM(0));
  FILE *file = fopen(filename, "r");
  if(!file)
    Except(shred, "Ogham can't open file for reading")
  fseek(file, 0, SEEK_END);
  const size_t length = ftell(file);
  ogh_music_t* ogh = _mp_malloc(shred->info->vm->gwion->mp, length);
  rewind(file);
  fread(ogh, length, 1, file);
  fclose(file);
  const M_Object ret = new_object_str(shred->info->vm->gwion, shred, "Ogham");
  OGH(ret) = ogh;
  *(M_Object*)RETURN = ret;
}

static MFUN(ogh_write) {
  const m_str filename = STRING(*(M_Object*)MEM(SZ_INT));
  FILE *file = fopen(filename, "w");
  if(!file)
    Except(shred, "Ogham can't open file for writing")
  const size_t length = sizeof(ogh_music_t) + OGH(o)->length * sizeof(ogh_packed_t);
  *(m_int*)RETURN = fwrite(OGH(o), length, 1, file);
  fclose(file);
}

#define note_import(member, type) \
  GWI_BB(gwi_func_ini(gwi, #type, #member)) \
  GWI_BB(gwi_func_arg(gwi, #type, "arg")) \
  GWI_BB(gwi_func_end(gwi, note_##member##_set, ae_flag_none)) \
  GWI_BB(gwi_func_ini(gwi, #type, #member)) \
  GWI_BB(gwi_func_end(gwi, note_##member##_get, ae_flag_none))

GWION_IMPORT(Ogham) {
  DECL_OB(const Type, t_ogham, = gwi_class_ini(gwi, "Ogham", "Object"))
  t_ogham->nspc->info->offset += SZ_INT;

    DECL_OB(const Type, t_player, = gwi_class_ini(gwi, "Player", "UGen"))
    gwi_class_xtor(gwi, NULL, player_dtor);

      GWI_BB(gwi_class_ini(gwi, "Note", "Object"))
      GWI_BB(gwi_item_ini(gwi, "int", "index"))
      GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
      GWI_BB(gwi_item_ini(gwi, "float", "frequency"))
      GWI_BB(gwi_item_end(gwi, ae_flag_none, fnum, 0))
      GWI_BB(gwi_item_ini(gwi, "float", "velocity"))
      GWI_BB(gwi_item_end(gwi, ae_flag_none, fnum, 0))
      GWI_BB(gwi_item_ini(gwi, "dur", "duration"))
      GWI_BB(gwi_item_end(gwi, ae_flag_none, fnum, 0))
      GWI_BB(gwi_class_end(gwi))

    GWI_BB(gwi_item_ini(gwi, "int", "polyphony"))
    GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0))
    t_player->nspc->info->offset += SZ_INT*4;//6
    GWI_BB(gwi_item_ini(gwi, "dur", "speed"))
    GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

    GWI_BB(gwi_func_ini(gwi, "void", "connect"))
    GWI_BB(gwi_func_arg(gwi, "UGen", "u"))
    GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))
    GWI_BB(gwi_func_ini(gwi, "void", "disconnect"))
    GWI_BB(gwi_func_arg(gwi, "UGen", "u"))
    GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))
    GWI_BB(gwi_func_ini(gwi, "void", "play"))
    GWI_BB(gwi_func_arg(gwi, "Note", "note"))
    GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))

    SET_FLAG(t_player, abstract);
    set_tflag(t_player, tflag_infer);
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_note, = gwi_class_ini(gwi, "Note", "Object"))
    t_note->nspc->info->offset += sizeof(ogh_note_t);
    note_import(offset, int)
    note_import(duration, int)
    note_import(volume, int)
    note_import(octave, int)
    note_import(name, char)
    note_import(is_sharp, bool)
    GWI_BB(gwi_func_ini(gwi, "string", "name"))
    GWI_BB(gwi_func_arg(gwi, "string", "note"))
    GWI_BB(gwi_func_end(gwi, note_name_str_set, ae_flag_none))
    GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_func_ini(gwi, "void", "tempo_change"))
  GWI_BB(gwi_func_arg(gwi, "int", "numer"))
  GWI_BB(gwi_func_arg(gwi, "int", "denum"))
  GWI_BB(gwi_func_end(gwi, ogh_tc, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "write"))
  GWI_BB(gwi_func_arg(gwi, "string", "filename"))
  GWI_BB(gwi_func_end(gwi, ogh_write, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "Ogham", "read"))
  GWI_BB(gwi_func_arg(gwi, "string", "filename"))
  GWI_BB(gwi_func_end(gwi, ogh_open, ae_flag_static))

  gwi_class_xtor(gwi, ogham_ctor, ogham_dtor);
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "Ogham", "Array:[Ogham.Note]", "Ogham"))
  GWI_BB(gwi_oper_end(gwi, "<<", ogham_add_notes))

  GWI_BB(gwi_oper_ini(gwi, NULL, "Ogham.Player", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_player_ctor))
  GWI_BB(gwi_oper_emi(gwi, opem_player_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  return GW_OK;
}
