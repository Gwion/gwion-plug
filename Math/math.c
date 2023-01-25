#include <stdlib.h>
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

#ifndef USE_DOUBLE
#define modf fmodf
#endif

static SFUN(math_abs) {
  *(m_uint*)RETURN = labs(*(m_int*)MEM(0));
}

static SFUN(math_rand) {
  *(m_uint*)RETURN = gw_rand(shred->info->vm->rand);
}

static SFUN(math_rand2) {
  const m_int min = *(m_int*)MEM(0);
  const m_int max = *(m_int*)MEM(SZ_INT);
  const m_int range = max - min;
  if(range == 0)
    *(m_uint*)RETURN = min;
  else {
    if(range > 0)
      *(m_uint*)RETURN = min + (m_int)((1.0 + range) * (gw_rand(shred->info->vm->rand) / (UINT32_MAX + 1.0)));
    else
      *(m_uint*)RETURN = min - (m_int)((-range + 1.0) * (gw_rand(shred->info->vm->rand) / (UINT32_MAX + 1.0)));
  }
}

static SFUN(math_randf) {
  *(m_float*)RETURN = 2.0 * gw_rand(shred->info->vm->rand) / UINT32_MAX - 1.0;
}

static SFUN(math_rand2f) {
  const m_float min = *(m_float*)MEM(0);
  const m_float max = *(m_float*)MEM(SZ_FLOAT);
  *(m_float*)RETURN = min + (max - min) * (gw_rand(shred->info->vm->rand) / (m_float)UINT32_MAX);
}

static SFUN(math_srand) {
  const m_float ret = *(m_float*)MEM(0);
  gw_seed(shred->info->vm->rand, ret);
  *(m_float*)RETURN = ret;
}

static SFUN(math_sgn) {
  const m_float ret = *(m_float*)MEM(0);
  *(m_uint*)RETURN = ret < 0. ? -1 : ret > 0. ? 1 : 0;
}

#define math1(func)                      \
static SFUN(math_##func) {               \
  const m_float ret = *(m_float*)MEM(0); \
  *(m_float*)RETURN = func(ret);         \
}

math1(fabs)
math1(sin)
math1(cos)
math1(tan)
math1(asin)
math1(acos)
math1(atan)
math1(sinh)
math1(cosh)
math1(tanh)
math1(asinh)
math1(acosh)
math1(atanh)
math1(sqrt)
math1(exp)
math1(log)
math1(log2)
math1(log10)
math1(floor)
math1(ceil)
math1(round)
math1(trunc)
math1(isinf)
math1(isnan)

static m_float _min(m_float _f1, m_float _f2) { return _f1 < _f2 ? _f1 : _f2; }
static m_float _max(m_float _f1, m_float _f2) { return _f1 > _f2 ? _f1 : _f2; }

#define math2(func)                              \
static SFUN(math_##func) {                       \
  const m_float ret1 = *(m_float*)MEM(0);        \
  const m_float ret2 = *(m_float*)MEM(SZ_FLOAT); \
  *(m_float*)RETURN = func(ret1, ret2);          \
}
math2(atan2)
math2(hypot)
math2(pow)
math2(fmod)
math2(modf)
math2(remainder)
math2(_min)
math2(_max)

#define decl_math1(name, func)                   \
  gwi_func_ini(gwi, "float", name);              \
  gwi_func_arg(gwi, "float", "value");           \
  GWI_BB(gwi_func_end(gwi, math_##func , ae_flag_static))    \

#define decl_math2(name, func)                   \
  gwi_func_ini(gwi, "float", name);              \
  gwi_func_arg(gwi, "float", "value1");          \
  gwi_func_arg(gwi, "float", "value2");          \
  GWI_BB(gwi_func_end(gwi, math_##func , ae_flag_static))    \

static INSTR(op_ffpower) {
  POP_REG(shred, SZ_FLOAT);
  *(m_float*)REG(-SZ_FLOAT) = pow(*(m_float*)REG(-SZ_FLOAT), *(m_float*)REG(0));
}

static INSTR(op_fipower) {
  POP_REG(shred, SZ_INT);
  *(m_float*)REG(-SZ_FLOAT) = pow(*(m_float*)REG(-SZ_FLOAT), (m_float)*(m_int*)REG(0));
}

static INSTR(op_iipower) {
  POP_REG(shred, SZ_INT);
  *(m_int*)REG(-SZ_INT) = (m_int)pow((m_float)*(m_int*)REG(-SZ_INT), (m_float)*(m_int*)REG(0));
}

static INSTR(op_ifpower) {
  POP_REG(shred, SZ_FLOAT + SZ_INT - SZ_FLOAT);
  *(m_float*)REG(-SZ_FLOAT) = pow((m_float)*(m_int*)REG(SZ_FLOAT - SZ_INT), *(m_float*)REG(SZ_INT-SZ_FLOAT));
}

GWION_IMPORT(Math) {
  GWI_OB(gwi_struct_ini(gwi, "Math"))

  gwi_func_ini(gwi, "int", "abs");
  gwi_func_arg(gwi, "int", "value");
  GWI_BB(gwi_func_end(gwi, math_abs, ae_flag_static))

  gwi_func_ini(gwi, "int", "rand");
  GWI_BB(gwi_func_end(gwi, math_rand, ae_flag_static))

  gwi_func_ini(gwi, "int", "rand2");
  gwi_func_arg(gwi, "int", "min");
  gwi_func_arg(gwi, "int", "max");
  GWI_BB(gwi_func_end(gwi, math_rand2, ae_flag_static))

  gwi_func_ini(gwi, "float", "randf");
  GWI_BB(gwi_func_end(gwi, math_randf, ae_flag_static))

  decl_math2("rand2f", rand2f)

  decl_math1("fabs",  fabs)
  decl_math1("srand", srand)
  decl_math1("sgn",   sgn)
  decl_math1("sin",   sin)
  decl_math1("cos",   cos)
  decl_math1("tan",   tan)
  decl_math1("asin",  asin)
  decl_math1("acos",  acos)
  decl_math1("atan",  atan)
  decl_math1("sinh",  sinh)
  decl_math1("cosh",  cosh)
  decl_math1("tanh",  tanh)
  decl_math1("asinh", asinh)
  decl_math1("acosh", acosh)
  decl_math1("atanh", atanh)
  decl_math1("sqrt",  sqrt)
  decl_math1("exp",   exp)
  decl_math1("log",   log)
  decl_math1("log2",  log2)
  decl_math1("log10", log10)
  decl_math1("floor", floor)
  decl_math1("ceil",  ceil)
  decl_math1("round", round)
  decl_math1("trunc", trunc)
  decl_math1("isinf", isinf)
  decl_math1("isnan", isnan)

  decl_math2("hypot",     hypot)
  decl_math2("atan2",     atan2)
  decl_math2("pow",       pow)
  decl_math2("fmod",      fmod)
  decl_math2("modf",      modf)
  decl_math2("remainder", remainder)
  decl_math2("min",       _min)
  decl_math2("max",       _max)
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "float", "float", "float"))
  GWI_BB(gwi_oper_end(gwi, "**", op_ffpower))

  GWI_BB(gwi_oper_ini(gwi, "int", "float", "float"))
  GWI_BB(gwi_oper_end(gwi, "**", op_ifpower))

  GWI_BB(gwi_oper_ini(gwi, "float", "int", "float"))
  GWI_BB(gwi_oper_end(gwi, "**", op_fipower))

  GWI_BB(gwi_oper_ini(gwi, "int", "int", "int"))
  GWI_BB(gwi_oper_end(gwi, "**", op_iipower))

  return GW_OK;
}
