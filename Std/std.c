#include <stdlib.h>
#include <string.h>
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
#include <dirent.h>
#ifdef USE_DOUBLE
#define fabs fabs
#undef USE_DOUBLE
#else
#define fabs fabsf
#endif
#include <err_msg.h>

#define LOGTWO log(2)
#define LOGTEN log(10)

static double mtof(const double f) {
  if(f <= -1500)
    return 0.0;
  if(f > 1499)
    return mtof(1499);
// else return (8.17579891564 * exp(.0577622650 * f));
  return (pow(2, (f - 69) / 12.0) * 440.0);
}

static double ftom(const double f) {
  // return (f > 0 ? 17.3123405046 * log(.12231220585 * f) : -1500);
  return (f > 0 ? (log(f / 440.0) / LOGTWO) * 12.0 + 69 : -1500);
}
static double powtodb(const double f) {
  if(f <= 0)
    return (0);
  const double val = 100 + 10. / LOGTEN * log(f);
  return (val < 0 ? 0 : val);
}

static double rmstodb(const double f) {
  if(f <= 0)
    return (0);
  const double val = 100 + 20. / LOGTEN * log(f);
  return (val < 0 ? 0 : val);
}

static double dbtopow(double f) {
  if(f <= 0)
    return (0);
  if(f > 870) f = 870;
    return (exp((LOGTEN * 0.1) * (f - 100.)));
}
static double dbtorms(double f) {
  if(f <= 0)
    return (0);
  if(f > 485) f = 485;
    return (exp((LOGTEN * 0.05) * (f - 100.)));
}

#define GETSTRING(a, c, b)                       \
  const M_Object a##_obj = *(M_Object*)MEM((b)); \
  if(!a##_obj) {                                 \
    xfun_handle(shred, "NullPtrException");   \
    return;                                      \
  }                                              \
  const m_str a = STRING(a##_obj);               \

static SFUN(std_system) {
  GETSTRING(str, 0, SZ_INT);
  *(m_uint*)RETURN = system(str);
}

#define STDGET                                \
  const m_int ret = *(m_int*)MEM(0);          \
  const m_int min = *(m_int*)MEM(SZ_INT);     \
  const m_int max = *(m_int*)MEM(SZ_INT * 2); \

static SFUN(std_clamp) {
  STDGET
  *(m_uint*)RETURN = ret < min ? min : ret > max ? max : ret;
}

static SFUN(std_clampf) {
  STDGET
  *(m_float*)RETURN = ret < min ? min : ret > max ? max : ret;
}

static SFUN(std_scale) {
  STDGET
  const m_float min2 = *(m_float*)MEM(SZ_FLOAT * 3);
  const m_float max2 = *(m_float*)MEM(SZ_FLOAT * 4);
  *(m_float*)RETURN = min2 + (max2 - min2) * ((ret - min) / (max - min));
}

static SFUN(std_getenv) {
  GETSTRING(env, 0, SZ_INT);
  const m_str str = getenv(env);
  *(M_Object*)RETURN = str ? new_string(shred->info->vm->gwion, str) : 0;
}

#ifdef BUILD_ON_WINDOWS
#define setenv(a,b,c) _putenv_s(a,b)
#endif
static SFUN(std_setenv) {
  GETSTRING(key, 0, SZ_INT*2);
  GETSTRING(val, SZ_INT, SZ_INT*2);
  *(m_uint*)RETURN = setenv(key, val, 1);
}
#ifdef BUILD_ON_WINDOWS
#undef setenv
#endif

static SFUN(std_atoi) {
  GETSTRING(key, 0, SZ_INT);
  *(m_int*)RETURN = atoi(key);
}

static SFUN(std_atof) {
  GETSTRING(key, 0, SZ_INT);
  *(m_float*)RETURN = (m_float)atof(key);
}

static SFUN(std_atoi2) {
  GETSTRING(key, 0, SZ_INT);
  char *endptr;
  *(m_int*)RETURN = strtol(key, &endptr, 10);
  **(m_uint**)MEM(SZ_INT) = endptr - key;
}

#ifdef USE_DOUBLE
#define str2float strtod
#else
#define str2float strtof
#endif

static SFUN(std_atof2) {
  GETSTRING(key, 0, SZ_INT*2);
  char *endptr;
  *(m_float*)RETURN = (m_float)str2float(key, &endptr);
  **(m_uint**)MEM(SZ_INT) = endptr - key;
}

#define pow10(a) pow(10.0, (a) / 20.0)
#define std(name, func)\
static SFUN(std_##name) {\
  *(m_float*)RETURN = func(*(m_float*)MEM(0));\
}
std(mtof, mtof)
std(ftom, ftom)
std(powtodb, powtodb)
std(rmstodb, rmstodb)
std(dbtopow, dbtopow)
std(dbtorms, dbtorms)
std(dbtolin, pow10)
std(lintodb, 20.0 * log10)

#define import_stdx(name, t1, t2)             \
  gwi_func_ini(gwi, #t1, #name);\
  gwi_func_arg(gwi, #t2, "value");            \
  GWI_BB(gwi_func_end(gwi, std_##name  , ae_flag_static)) \

static SFUN(std_classname) {
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, o->type_ref->name);
}

GWION_IMPORT(std) {
  GWI_OB(gwi_struct_ini(gwi, "Std"))

  gwi_func_ini(gwi, "int", "clamp");
  gwi_func_arg(gwi, "int", "value");
  gwi_func_arg(gwi, "int", "min");
  gwi_func_arg(gwi, "int", "max");
  GWI_BB(gwi_func_end(gwi, std_clamp, ae_flag_static))

  gwi_func_ini(gwi, "float", "clampf");
  gwi_func_arg(gwi, "float", "value");
  gwi_func_arg(gwi, "float", "min");
  gwi_func_arg(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, std_clampf, ae_flag_static))

  gwi_func_ini(gwi, "float", "scale");
  gwi_func_arg(gwi, "float", "value");
  gwi_func_arg(gwi, "float", "srcmin");
  gwi_func_arg(gwi, "float", "srcmax");
  gwi_func_arg(gwi, "float", "dstmin");
  gwi_func_arg(gwi, "float", "dstmax");
  GWI_BB(gwi_func_end(gwi, std_scale, ae_flag_static))

  gwi_func_ini(gwi, "int", "setenv");
  gwi_func_arg(gwi, "string", "key");
  gwi_func_arg(gwi, "string", "value");
  GWI_BB(gwi_func_end(gwi, std_setenv, ae_flag_static))

  gwi_func_ini(gwi, "int", "atoi");
  gwi_func_arg(gwi, "string", "key");
  gwi_func_arg(gwi, "Ref:[int]", "idx");
  GWI_BB(gwi_func_end(gwi, std_atoi2, ae_flag_static))

  gwi_func_ini(gwi, "float", "atof");
  gwi_func_arg(gwi, "string", "key");
  gwi_func_arg(gwi, "Ref:[int]", "idx");
  GWI_BB(gwi_func_end(gwi, std_atof2, ae_flag_static))

  gwi_func_ini(gwi, "int", "atoi");
  gwi_func_arg(gwi, "string", "key");
  GWI_BB(gwi_func_end(gwi, std_atoi, ae_flag_static))

  gwi_func_ini(gwi, "float", "atof");
  gwi_func_arg(gwi, "string", "key");
  GWI_BB(gwi_func_end(gwi, std_atof, ae_flag_static))

  import_stdx(system , int,    string)
  import_stdx(getenv , string, string)
  import_stdx(mtof   , float,  float)
  import_stdx(ftom   , float,  float)
  import_stdx(powtodb, float,  float)
  import_stdx(rmstodb, float,  float)
  import_stdx(dbtopow, float,  float)
  import_stdx(dbtorms, float,  float)
  import_stdx(dbtolin, float,  float)
  import_stdx(lintodb, float,  float)

  GWI_BB(gwi_func_ini(gwi, "string", "classname"));
  GWI_BB(gwi_func_arg(gwi, "Object", "arg"));
  GWI_BB(gwi_func_end(gwi, std_classname, ae_flag_static));

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
