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
#define PLAYER_GWION(o) *(Gwion*)(o->data + SZ_INT*4)
#define PLAYER_NOW(o) *(m_uint*)(o->data + SZ_INT*5)
#define PLAYER_DUR(o) *(m_float*)(o->data + SZ_INT*6)

typedef struct {
    double real_offset;
    double real_duration;
    uint8_t volume;
    uint8_t octave;
    char name;
    bool is_sharp;
} ogh_adjusted_note_t;

#define ARRAY_NOTE(a, i)  ((ogh_adjusted_note_t*)(ARRAY_PTR((a)) + ((i) * sizeof(ogh_adjusted_note_t))))

#include <math.h>

static m_float note2freq(int note) {
    int a = 440; //frequency of A (coomon value is 440Hz)
    return pow(2, (note - 69) / 12.0) * a;
}

static CTOR(ogham_ctor) {
  OGH(o) = mp_calloc2(shred->info->vm->gwion->mp, sizeof(ogh_music_t));
  ogh_music_init(OGH(o));
}

static DTOR(ogham_dtor) {
  mp_free2(shred->info->vm->gwion->mp, sizeof(ogh_music_t) + (OGH(o)->length)*sizeof(ogh_note_t), OGH(o));
}

static DTOR(player_dtor) {
  free_m_vector(shred->info->vm->gwion->mp, PLAYER_NOTES(o));
}

typedef struct {
  M_Vector array;
  m_uint idx;
  m_float tempo;
} Decoder;
#define DEFAULT_TEMPO (1.0/64.0)

static inline void adjust(ogh_adjusted_note_t *dec, ogh_note_t* n, m_float tempo) {
  dec->real_offset = tempo *n->offset;
  dec->real_duration = tempo *n->duration;
  dec->volume = n->volume;
  dec->octave = n->octave;
  dec->name = n->name;
  dec->is_sharp = n->is_sharp;
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

static MFUN(ogh_tc) {
  ogh_tempo_change(OGH(o), *(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2));
}

#define NOTE(o) (*(ogh_note_t*)(o->data))


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

static INSTR(ogham_add_note) {
  POP_REG(shred, SZ_INT);
  M_Object ogh = *(M_Object*)REG(-SZ_INT);
  M_Object not = *(M_Object*)REG(0);
  const uint32_t len = OGH(ogh)->length;
  OGH(ogh) = mp_realloc(shred->info->vm->gwion->mp, OGH(ogh),
     sizeof(ogh_music_t) + len*sizeof(ogh_packed_t), sizeof(ogh_packed_t) + (len+1)*sizeof(ogh_note_t));
  ogh_music_add_notes(OGH(ogh), 1, &NOTE(not));
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

static int ogh_note_midi(ogh_adjusted_note_t *note) {
  switch(note->name) {
    case 'A': return 9;
    case 'B': return 11;
    case 'C': return 12;
    case 'D': return 14;
    case 'E': return 16;
    case 'F': return 18;
    case 'G': return 19;
  }
}

static INSTR(OghStop) {
  shreduler_remove(shred->info->vm->shreduler, shred, 0);
  vector_rem2(&shred->info->vm->shreduler->shreds, (vtype)shred->tick->self);
}

static VM_Code ogh_callback(MemPool mp, VM_Code base) {
  char name[strlen(base->name) + 11];
  sprintf(name, "%s(callback)", base->name);
  const Instr instr = (Instr)vector_back(base->instr);
  instr->opcode = eOP_MAX;
  instr->execute = OghStop;
  VM_Code code = new_vmcode(mp, base->instr, base->stack_depth, base->builtin, name);
  code->callback = 1;
  instr->opcode = eFuncReturn;
  return code;
}

static void launch_notes(const M_Object o, ogh_adjusted_note_t *note) {
  const Gwion gwion = PLAYER_GWION(o);
  const Type t = o->type_ref;
  const Func noteOn = (Func)vector_at((Vector)&t->nspc->info->vtable, 6);
  const uint midi = ogh_note_midi(note) + note->is_sharp + (note->octave + 1) *12;
  const VM_Code noteOn_code = vmcode_callback(gwion->mp, noteOn->code);
// this is really wasteful. rather have a VM member in Player
// shred could be reused too
  VM* vm = gwion_cpy(gwion->vm);
  const VM_Shred shred = new_vm_shred(gwion->mp, noteOn_code);
  *(M_Object*)(shred->mem) = o;
  *(m_float*)(shred->mem + SZ_INT) = note2freq(midi);
  *(m_float*)(shred->mem + SZ_INT + SZ_FLOAT) = ((m_float)note->volume)/128.0;
  vm_add_shred(vm, shred);
  vm_run(vm);
  free_vm(vm);
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
    }
    i++;
  }
  PLAYER_NOW(o) += 1;
}

static INSTR(PlayerCtor) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  const M_Vector array = new_m_vector(shred->info->vm->gwion->mp, sizeof(ogh_adjusted_note_t), OGH(o)->length);
  Decoder dec = { .array=array, .tempo=DEFAULT_TEMPO };
  ogh_music_decode(OGH(o), dbg_on_note, NULL, &dec);
  qsort(ARRAY_PTR(dec.array), m_vector_size(dec.array), sizeof(ogh_adjusted_note_t), compare);
  const Type t = (Type)instr->m_val;
  const M_Object ret = new_object(shred->info->vm->gwion->mp, shred, t);
  UGEN(ret) = new_UGen(shred->info->mp);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(ret));
  ugen_ini(shred->info->vm->gwion, UGEN(ret), 0, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(ret), player_tick, ret, 0);
  PLAYER_NOTES(ret) = array;
  PLAYER_POLYPHONY(ret) = 1;
// this one should be made public
  PLAYER_DUR(ret) = shred->info->vm->bbq->si->sr;
  PLAYER_NOW(ret) = shred->tick->shreduler->bbq->pos;
  PLAYER_GWION(ret) = shred->info->vm->gwion;
  *(M_Object*)REG(-SZ_INT) = ret;
  ++ret->ref;
  const VM_Code ctor_code = ogh_callback(shred->info->vm->gwion->mp, t->nspc->pre_ctor);
  const VM_Shred ctor_sh = new_vm_shred(shred->info->vm->gwion->mp, ctor_code);
  *(M_Object*)(ctor_sh->mem) = ret;
  shreduler_remove(shred->info->vm->shreduler, shred, 0);
  vm_add_shred(shred->info->vm, ctor_sh);
  shredule(shred->info->vm->shreduler, shred, GWION_EPSILON);
}

static OP_EMIT(opem_player_ctor) {
  Exp_Call *call = (Exp_Call*)data;
  const Instr instr = emit_add_instr(emit, PlayerCtor);
  instr->m_val = (m_uint)exp_self(call)->type;
  return GW_OK;
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

    DECL_OB(const Type, t_note, = gwi_class_ini(gwi, "Note", "Object"))
    t_note->nspc->info->offset += sizeof(ogh_note_t);
    note_import(offset, int)
    note_import(duration, int)
    note_import(volume, int)
    note_import(octave, int)
    note_import(name, char)
    note_import(is_sharp, bool)
    GWI_BB(gwi_class_end(gwi))

    DECL_OB(const Type, t_player, = gwi_class_ini(gwi, "Player", "UGen"))
    gwi_class_xtor(gwi, NULL, player_dtor);
    GWI_BB(gwi_item_ini(gwi, "int", "polyphony"))
    GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0))
    t_player->nspc->info->offset += SZ_INT*6 + SZ_FLOAT;

    GWI_BB(gwi_func_ini(gwi, "void", "connect"))
    GWI_BB(gwi_func_arg(gwi, "UGen", "u"))
    GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))
    GWI_BB(gwi_func_ini(gwi, "void", "disconnect"))
    GWI_BB(gwi_func_arg(gwi, "UGen", "u"))
    GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))
    GWI_BB(gwi_func_ini(gwi, "void", "noteOn"))
    GWI_BB(gwi_func_arg(gwi, "float", "note"))
    GWI_BB(gwi_func_arg(gwi, "float", "velocity"))
    GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))
    GWI_BB(gwi_func_ini(gwi, "void", "noteOff"))
    GWI_BB(gwi_func_arg(gwi, "float", "note"))
    GWI_BB(gwi_func_arg(gwi, "float", "velocity"))
    GWI_BB(gwi_func_end(gwi, (f_xfun)1, ae_flag_abstract))

    SET_FLAG(t_player, abstract);
    set_tflag(t_player, tflag_infer);
    GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_func_ini(gwi, "void", "tempo_change"))
  GWI_BB(gwi_func_arg(gwi, "int", "numer"))
  GWI_BB(gwi_func_arg(gwi, "int", "denum"))
  GWI_BB(gwi_func_end(gwi, ogh_tc, ae_flag_none))

  gwi_class_xtor(gwi, ogham_ctor, ogham_dtor);
  GWI_BB(gwi_class_end(gwi))

// TODO: add Ogham << Note[]
  GWI_BB(gwi_oper_ini(gwi, "Ogham", "Ogham.Note", "Ogham"))
  GWI_BB(gwi_oper_end(gwi, "<<", ogham_add_note))

  GWI_BB(gwi_oper_ini(gwi, NULL, "Ogham.Player", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_player_ctor))
  GWI_BB(gwi_oper_emi(gwi, opem_player_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  return GW_OK;
}
