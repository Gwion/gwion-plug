#include <ffi.h>
#include <complex.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "traverse.h"
#include "emit.h"
#include "gwi.h"
#include "gack.h"
#include "vararg.h"

static const m_str typenames[] = {
    "int", "uint", "int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64",
    "float", "double", "longdouble",
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
    "complex_float", "complex_double", "complex_longdouble",
#endif
    "pointer"
};

static ffi_type *const ffitypes[] = {
    &ffi_type_sint, &ffi_type_uint, &ffi_type_sint8, &ffi_type_uint8, &ffi_type_sint16, &ffi_type_uint16,
    &ffi_type_sint32, &ffi_type_uint32, &ffi_type_sint64, &ffi_type_uint64,
    &ffi_type_float, &ffi_type_double, &ffi_type_longdouble,
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
    &ffi_type_complex_float, &ffi_type_complex_double, &ffi_type_complex_longdouble,
#endif
      &ffi_type_pointer
};

static ffi_type* str2ffitype(const m_str name) {
  for(m_uint i = 0; i < sizeof(typenames)/sizeof(m_str); ++i) {
    if(!strcmp(name, typenames[i]))
      return ffitypes[i];
  }
  return NULL;
}

#define FFI_CIF(t)   (*(ffi_cif*)t->nspc->info->class_data)
#define FFI_DL(t)    (*(void**)(t->nspc->info->class_data + sizeof(ffi_cif)))
#define FFI_SZ(t)    (*(m_uint*)(t->nspc->info->class_data + sizeof(ffi_cif) + SZ_INT))
#define FFI_TYPES(t) ( (ffi_type**)(t->nspc->info->class_data + sizeof(ffi_cif) + SZ_INT * 2))

static MFUN(ffi_do_call) {
  const Type t = o->type_ref;
  ffi_cif cif = FFI_CIF(t);
  const void*  func = FFI_DL(t);
  void* const data = MEM(SZ_INT + FFI_SZ(t)), **tmp = data;
  m_uint sz = SZ_INT;
  for(unsigned i = 0; i < cif.nargs; ++i) {
    const ffi_type *type = cif.arg_types[i];
    *(void**)tmp = MEM(sz);
    sz += type->size;
    ++tmp;
  }
  ffi_call(&cif, FFI_FN(func), (m_bit*)RETURN, data);
}

static MFUN(ffivar_do_call) {
  const Type t = o->type_ref;
  ffi_cif cif = FFI_CIF(t);
  const void*  func = FFI_DL(t);
  const Func f = (Func)vector_front(&t->nspc->vtable);
  M_Object varobj  = *(M_Object*)MEM(f->code->stack_depth - SZ_INT);
  struct Vararg_ *v = *(struct Vararg_**)varobj->data;
  const m_uint nvariadic = vector_size(&v->t);
  void* const data = MEM(SZ_INT + FFI_SZ(t) + SZ_INT), **tmp = data;
  m_uint sz = SZ_INT;
  for(unsigned i = 0; i < cif.nargs; ++i) {
    const ffi_type *type = FFI_TYPES(t)[i];
    *(void**)tmp = MEM(sz);
    ++tmp;
  }
  sz = 0;
  const uint nargs = FFI_CIF(t).nargs + nvariadic;
  ffi_type *trash[nargs];// = { &ffi_type_pointer, &ffi_type_sint };
  ffi_type **tmp_types = trash + cif.nargs;
  memcpy(trash, FFI_TYPES(t), cif.nargs * SZ_INT);
  for(unsigned i = 0; i < nvariadic; ++i) {
    const Type type = ((Type)vector_at(&v->t, i));
    const m_str name = type->name;
    *tmp_types = str2ffitype(type->name);
    *(void**)tmp = (v->d + sz);
    sz += type->size;
    ++tmp;
    ++tmp_types;
  }
  if(ffi_prep_cif_var(&FFI_CIF(t), FFI_DEFAULT_ABI,
        FFI_CIF(t).nargs, FFI_CIF(t).nargs + nvariadic, &ffi_type_sint, trash) != FFI_OK) {
    handle(shred, "can't prepare variadic func");
    return;
  }
  ffi_call(&FFI_CIF(t), FFI_FN(FFI_DL(t)), (m_bit*)RETURN, data);
  FFI_CIF(t).nargs -= nvariadic;
}

ANN static Exp decl_from_id(const Gwion gwion, const m_str type, const m_str name, const loc_t pos) {
  Type_Decl *td = new_type_decl(gwion->mp, insert_symbol(gwion->st, type), pos);
  const Var_Decl var = new_var_decl(gwion->mp, insert_symbol(gwion->st, name), NULL, pos);
  const Var_Decl_List vlist = new_var_decl_list(gwion->mp, var, NULL);
  SET_FLAG(td, static);
  return new_exp_decl(gwion->mp, td, vlist, pos);
}

ANN static inline Stmt_List stmt_list_from_id(const Gwion gwion, const m_str type, const m_str name, const loc_t pos) {
  const Exp exp = decl_from_id(gwion, type, name, pos);
  const Stmt stmt = new_stmt_exp(gwion->mp, ae_stmt_exp, exp, pos);
  return new_stmt_list(gwion->mp, stmt, NULL);
}

static OP_CHECK(ctor_as_call) {
  Exp_Call *const call = (Exp_Call*)data;
  Exp func = cpy_exp(env->gwion->mp, call->func), e = call->func;
  e->exp_type = ae_exp_dot;
  Exp_Dot *dot = &e->d.exp_dot;
  dot->base = func;
  dot->xid = insert_symbol(env->gwion->st, "call");
  return check_exp_call1(env, call) ?: env->gwion->type[et_error];
}

static inline m_bool _traverse_ffi(const Env env, const Class_Def cdef) {
  CHECK_BB(scan0_class_def(env, cdef));;
  return traverse_class_def(env, cdef);
}

static inline m_bool traverse_ffi(const Env env, const Type ffi, const Class_Def cdef) {
  const m_uint scope = env_push_type(env, ffi);
  const m_bool ret = _traverse_ffi(env, cdef);
  env_pop(env, scope);
  return ret;
}

static inline Type _check_ffi_types(const Env env, const Exp_Call *call) {
  if(call->args->next)
    CHECK_OO(check_exp(env, call->args->next));;
  return known_type(env, call->tmpl->call->td);
}

static inline Type check_ffi_types(const Env env, const Type ffi, const Exp_Call *call) {
  const m_uint scope = env_push_type(env, ffi);
  const Type t = _check_ffi_types(env, call);
  env_pop(env, scope);
  return t;
}

static OP_CHECK(ffi_var_cast) {
  Exp exp = (Exp)data;
  Exp_Call *call = &exp->d.exp_call;
  CHECK_ON(check_exp(env, call->args));
  struct loc_t_ loc = {};
  const Type ffi = str2type(env->gwion, "FFIBASE.@CFFI", loc);
  Exp arg = call->args->next;
  while(arg) {
    if(isa(arg->type, ffi) < 0)
      ERR_N(arg->pos, "FFI variadic arguments must be of FFI type");
    arg = arg->next;
  }
  return NULL;
}

static OP_CHECK(opck_ffi_ctor) {
  const MemPool mp = env->gwion->mp;
  Exp_Call *call = (Exp_Call*)data;
  if(!call->tmpl)
    ERR_N(exp_self(call)->pos, "'FFI' needs a return type as template");
  CHECK_ON(check_exp(env, call->func)); /* doesn't seem needed */
  if(!call->args)
    ERR_N(exp_self(call)->pos, "'FFI' needs at least function name");
  struct loc_t_ loc = {};
  const Type ffi = str2type(env->gwion, "FFIBASE", loc);
  const Type ffivar = str2type(env->gwion, "FFIvar", loc);
  const m_bool variadic = isa(actual_type(env->gwion, call->func->type), ffivar) > 0;
  DECL_ON(const Type, ret_type, = check_ffi_types(env, ffi, call));
  Exp exp = call->args;
  if(exp->exp_type != ae_exp_primary || exp->d.prim.prim_type != ae_prim_id)
    ERR_N(exp_self(call)->pos, "'FFI' first argument must be the name of a function");

  const Symbol func_sym = exp->d.prim.d.var;
  const m_str func_name = s_name(func_sym);
  const Type exists = nspc_lookup_type0(ffi->nspc, func_sym);
  if(exists)
    return exists;
  void *dl = DLOPEN(NULL, RTLD_LAZY | RTLD_GLOBAL);
#ifndef BUILD_ON_WINDOWS
  void* dlfunc = dlsym(dl, func_name);
#else
  void* dlfunc = GetProcAddress(dl, func_name);
#endif
  if(!dlfunc)
    ERR_N(exp->pos, "can't open func '%s'", func_name);

  exp = exp->next;
  Arg_List base = NULL, list = NULL;
  const Type cffi = nspc_lookup_type0(ffi->nspc, insert_symbol(env->gwion->st, "@CFFI"));
  while(exp) {
    const Type actual = actual_type(env->gwion, exp->type);
    if(is_class(env->gwion, exp->type) < 0 ||
        isa(actual, cffi) < 0)
      ERR_N(exp->pos, "Argument is not a FFI type");
    char name[64];
    sprintf(name, "FFIBASE.%s", actual->name);
    const loc_t pos = exp->pos;
    Type_Decl *td = str2td(env->gwion, name, pos);
    Var_Decl var = new_var_decl(mp, NULL, NULL, pos);
    Arg_List tmp = new_arg_list(mp, td, var, NULL);
    if(!base)
      base = tmp;
    if(list)
      list->next = tmp;
    list = tmp;
    exp = exp->next;
  }
  char _ret_name[64];
  sprintf(_ret_name, "FFIBASE.%s", ret_type->name);
  Type_Decl *td = str2td(env->gwion, _ret_name, call->func->pos);
  Func_Base *fb = new_func_base(mp, td, insert_symbol(env->gwion->st, "call"), base, ae_flag_none, call->func->pos);
  if(variadic)
    set_fbflag(fb, fbflag_variadic);
  Func_Def fdef = new_func_def(mp, fb, NULL);
  Section * section = new_section_func_def(env->gwion->mp, fdef);
  Ast body = new_ast(env->gwion->mp, section, NULL);
{
  const Stmt_List ssz = stmt_list_from_id(env->gwion, "int", "@sz", exp_self(call)->pos);
  const Stmt_List sfunc = stmt_list_from_id(env->gwion, "@internal", "@func", exp_self(call)->pos);
  sfunc->next = ssz;
  const Stmt_List scif = stmt_list_from_id(env->gwion, "@cif", "@ffi_cif", exp_self(call)->pos);
  scif->next = sfunc;
  Section *section = new_section_stmt_list(env->gwion->mp, scif);
  body->next = new_ast(mp, section, NULL);
}
  char ext_name[64];
  sprintf(ext_name, "FFI:[FFIBASE.%s]", ret_type->name);
  Type_Decl *const ext = str2td(env->gwion, ext_name, call->func->pos);
  const Class_Def cdef = new_class_def(mp, ae_flag_abstract | ae_flag_final, func_sym, ext, body, call->func->pos);
  CHECK_BN(traverse_ffi(env, ffi, cdef));
  const Type t = cdef->base.type;
  const Func func = (Func)vector_front(&t->nspc->vtable);
  builtin_func(mp, func, !variadic ? ffi_do_call : ffivar_do_call);
  const struct Op_Func opfunc = { .ck=ctor_as_call };
  const struct Op_Import opi = { .rhs=t, .ret=ret_type,
    .func=&opfunc, .data=(uintptr_t)func, .pos=call->func->pos, .op=insert_symbol(env->gwion->st, "@ctor") };
  CHECK_BN(add_op(env->gwion, &opi));
  if(variadic) {
    const struct Op_Func opfunc = { .ck=ffi_var_cast };
    const struct Op_Import opi = { .rhs=func->value_ref->type,
      .func=&opfunc, .data=(uintptr_t)func, .pos=call->func->pos, .op=insert_symbol(env->gwion->st, "@func_check") };
  CHECK_BN(add_op(env->gwion, &opi));
  }
  uint n = 0;
  Exp e = call->args->next;
  size_t arg_sz = 0;
  do {
    ++n;
    arg_sz += e->type->size;
  } while((e = e->next));
  t->nspc->info->class_data_size += n * SZ_INT;
  nspc_allocdata(mp, t->nspc);
{
  ffi_type** types = FFI_TYPES(t);
  Exp exp = call->args->next;
  uint i = 0;
  while(exp) {
    const m_str name = actual_type(env->gwion, exp->type)->name;
    types[i] = str2ffitype(name);
    ++i;
    exp = exp->next;
  }
}
  free_exp(env->gwion->mp, call->args);
  call->args = NULL;
  FFI_SZ(t) = arg_sz;
  FFI_DL(t) = dlfunc;
  if(!variadic) {
    if(ffi_prep_cif(&FFI_CIF(t), FFI_DEFAULT_ABI,
        n, &ffi_type_sint, FFI_TYPES(t)) != FFI_OK)
    ERR_N(exp_self(call)->pos, "can't prepare func '%s'", func_name);
  } else FFI_CIF(t).nargs = n;
  return t;
}

static INSTR(FFICtor) {
  const Type t = (Type)instr->m_val;
  *(M_Object*)REG(-SZ_INT) = new_object(shred->info->mp, shred, t);
}

static OP_EMIT(opem_ffi_ctor) {
  const Exp_Call *call = (Exp_Call*)data;
  const Type t = exp_self(call)->type;
  const Instr instr = emit_add_instr(emit, FFICtor);
  instr->m_val = (m_uint)t;
  return GW_OK;
}

#define FFI_GACK(name, type, fmt)   \
static GACK(gack_ffi_##name) {      \
  INTERP_PRINTF(fmt, *(type*)VALUE) \
}

#define FFI_INSTR(name, type, c, d)                        \
static INSTR(name##2ffi) {                                 \
  POP_REG(shred, d - sizeof(type));                        \
  *(type*)REG(-sizeof(type)) = *(c*)REG(sizeof(type) - d); \
}                                                          \
static INSTR(ffi2##name) {                                 \
  POP_REG(shred, sizeof(type) - d);                        \
  *(c*)REG(-d) = *(type*)REG(d - sizeof(type));            \
}

static INSTR(object2ffi) {
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  *(void**)REG(-SZ_INT) = o ? *(void**)o->data : NULL;
}

#define FFI_FUNC(name, type, gwtype, sz, fmt)              \
FFI_GACK(name, type, fmt)                                  \
FFI_INSTR(name, type, gwtype, sz)

FFI_FUNC(int, int, m_int, SZ_INT, "%i")
FFI_FUNC(uint, uint, m_int, SZ_INT, "%u")
FFI_FUNC(int8, int8_t, m_int, SZ_INT, "%si")
FFI_FUNC(uint8, uint8_t, m_uint, SZ_INT, "%su")
FFI_FUNC(int16, int16_t, m_int, SZ_INT, "%i")
FFI_FUNC(uint16, uint16_t, m_uint, SZ_INT, "%u")
FFI_FUNC(int32, int32_t, m_int, SZ_INT, "%i")
FFI_FUNC(uint32, uint32_t, m_uint, SZ_INT, "%u")
FFI_FUNC(int64, int64_t, m_int, SZ_INT, "%li")
FFI_FUNC(uint64, uint64_t, m_uint, SZ_INT, "%lu")
FFI_FUNC(float, float, m_float, SZ_FLOAT, "%f")
FFI_FUNC(double, double, m_float, SZ_FLOAT, "%lf")
FFI_FUNC(longdouble, long double, m_float, SZ_FLOAT, "%llf")
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
FFI_GACK(complex_float, complex float, "%p")
FFI_GACK(complex_double, complex double, "%p")
FFI_GACK(complex_longdouble, complex long double, "%p")
#endif
FFI_FUNC(pointer, void*, void*, SZ_INT, "%p")

#define FFI_DECL(name, type)                                                      \
  DECL_OB(const Type, t_##name, = gwi_mk_type(gwi, #name, sizeof(type), "@CFFI")); \
  gwi_add_type(gwi, t_##name);                                                    \
  gwi_gack(gwi, t_##name, gack_ffi_##name);

static OP_CHECK(opck2ffi) {
  struct Implicit* imp = (struct Implicit*)data;
  return imp->e->cast_to = imp->t;
}

#define FFI_OPER2(name, gwname, ffiname)                                          \
  GWI_BB(gwi_oper_ini(gwi, #gwname, "FFIBASE." #ffiname, "FFIBASE." #ffiname))    \
  GWI_BB(gwi_oper_end(gwi, "$", name##2ffi))                                      \
  GWI_BB(gwi_oper_add(gwi, opck2ffi))                                             \
  GWI_BB(gwi_oper_end(gwi, "@implicit", name##2ffi))                              \
  GWI_BB(gwi_oper_ini(gwi, "FFIBASE." #ffiname, #gwname, #gwname))                \
  GWI_BB(gwi_oper_end(gwi, "$", ffi2##name))                                      \
  GWI_BB(gwi_oper_end(gwi, "@implicit", ffi2##name))

#define FFI_OPER(gwname, ffiname) FFI_OPER2(gwname, gwname, ffiname)

GWION_IMPORT(FFI) {
  DECL_OB(const Type, t_ffib, = gwi_class_ini(gwi, "FFIBASE", "Object"));
    DECL_OB(const Type, t_cif, = gwi_mk_type(gwi, "@cif", sizeof(ffi_cif), NULL));
    gwi_add_type(gwi, t_cif);
    DECL_OB(const Type, t_cffi, = gwi_mk_type(gwi, "@CFFI", 0, NULL));
    gwi_add_type(gwi, t_cffi);

    FFI_DECL(int, int)
    FFI_DECL(uint, uint)
    FFI_DECL(int8, int8_t)
    FFI_DECL(uint8, uint8_t)
    FFI_DECL(int16, int16_t)
    FFI_DECL(uint16, uint16_t)
    FFI_DECL(int32, int32_t)
    FFI_DECL(uint32, uint32_t)
    FFI_DECL(int64, int64_t)
    FFI_DECL(uint64, uint64_t)
    FFI_DECL(float, float)
    FFI_DECL(double, double)
    FFI_DECL(longdouble, long double)
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
    FFI_DECL(complex_float, complex float)
    FFI_DECL(complex_double, complex double)
    FFI_DECL(complex_longdouble, complex long double)
#endif
    FFI_DECL(pointer, void*)
  GWI_BB(gwi_class_end(gwi))

  GWI_OB(gwi_class_ini(gwi, "FFI:[A]", "FFIBASE"))
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, NULL, "FFIBASE", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_ffi_ctor))
  GWI_BB(gwi_oper_emi(gwi, opem_ffi_ctor))
  GWI_BB(gwi_oper_end(gwi, "@ctor", NULL))

  DECL_OB(const Type, t_ffivar, = gwi_class_ini(gwi, "FFIvar:[A]", "FFIBASE"));
  GWI_BB(gwi_class_end(gwi))

  FFI_OPER(int, int)
  FFI_OPER(int, uint)
  FFI_OPER(int, int8)
  FFI_OPER(int, uint8)
  FFI_OPER(int, int16)
  FFI_OPER(int, uint16)
  FFI_OPER(int, int32)
  FFI_OPER(int, uint32)
  FFI_OPER(int, int64)
  FFI_OPER(int, uint64)
  FFI_OPER(float, float)
  FFI_OPER(float, double)
  FFI_OPER(float, longdouble)
  FFI_OPER2(pointer, @internal, pointer)

  GWI_BB(gwi_oper_ini(gwi, "Object", "FFIBASE.pointer", "FFIBASE.pointer"))
  GWI_BB(gwi_oper_end(gwi, "$", object2ffi))
  GWI_BB(gwi_oper_end(gwi, "@implicit", object2ffi))
//  GWI_BB(gwi_oper_ini(gwi, "FFI." #ffiname, #gwname, #gwname))
//  GWI_BB(gwi_oper_end(gwi, "$", ffi2##name))
//  GWI_BB(gwi_oper_end(gwi, "@implicit", ffi2##name))
  return GW_OK;
}
