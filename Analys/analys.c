#include <stdlib.h>
#include <math.h>
#include <soundpipe.h>
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

static Type t_ana;
typedef struct {
  unsigned int size;
  unsigned int pos;
  m_float* data;
  Vector curr;
} sp_buffer;

static m_int sp_buffer_create(sp_buffer** p, m_uint size) {
  *p = (sp_buffer*)xmalloc(sizeof(sp_buffer));
  (*p)->size = size;
  (*p)->pos = 0;
  (*p)->data = (m_float*)xcalloc(size, sizeof(m_float));
  return SP_OK;
}

static m_int sp_buffer_destroy(sp_buffer* buffer) {
  free(buffer->data);
  free(buffer);
  return SP_OK;
}

static m_int sp_buffer_add(sp_buffer* buffer, m_float f) {
  buffer->data[ buffer->pos ] = f;
  buffer->pos++;
  buffer->pos %= buffer->size;
  return SP_OK;
}

static m_float* sp_buffer_get(sp_buffer* buffer) {
  m_uint i;
  m_float* ret = (m_float*)xcalloc(buffer->size, sizeof(m_float));
  for(i = 0; i < buffer->size; i++)
    ret[i] = buffer->data[(buffer->pos + i) % buffer->size];
  return ret;
}

typedef struct {
  struct BBQ_* sp;
  sp_buffer*  buf;
  /*  m_float*  (*win)(m_float* buf, m_uint size);*/
  FFTFREQS*    frq;
  FFTwrapper* fft;
  m_uint last;
} Fft;


static TICK(fft_tick) {
  Fft* ana = (Fft*)u->module.gen.data;
  if(!ana->buf)
    return;
  sp_buffer_add(ana->buf, u->in);      // add them to buffer
  if(u->module.gen.trig) {
    if(ana->last == ana->sp->pos || u->module.gen.trig->out) {  // if trigged, compute fft
      m_float* smp = sp_buffer_get(ana->buf);
      /*    if(ana->win)*/                  // do windowing
      /*      ana->win(smp, ana->buf->size);*/
      smps2freqs(ana->fft, smp, ana->frq);
      free(smp);
      ana->last = ana->sp->pos;
    }
  }
}

static CTOR(fft_ctor) {
  Fft* fft = UGEN(o)->module.gen.data = (Fft*)xcalloc(1, sizeof(Fft));
  ugen_ini(shred->info->vm->gwion, UGEN(o), 1, 1);
  ugen_gen(shred->info->vm->gwion, UGEN(o), fft_tick, fft, 1);
  fft->sp = shred->info->vm->bbq;
}

static DTOR(fft_dtor) {
  Fft* ana = (Fft*)UGEN(o)->module.gen.data;
  if(ana->buf)
    sp_buffer_destroy(ana->buf);
  if(ana->frq) {
    deleteFFTFREQS(ana->frq);
    free(ana->frq);
  }
  if(ana->fft)
    FFTwrapper_destroy(&ana->fft);
  free(ana);
}

static MFUN(fft_init) {
  Fft* ana = (Fft*)UGEN(o)->module.gen.data;
  m_int size = *(m_int*)MEM(SZ_INT);
  if(size <= 0 || size % 2)Except(shred, "FftInvalidSizeException.")
    if(ana->buf)
      sp_buffer_destroy(ana->buf);
  if(ana->frq) {
    deleteFFTFREQS(ana->frq);
    free(ana->frq);
  }
  if(ana->fft)
    FFTwrapper_destroy(&ana->fft);
  sp_buffer_create(&ana->buf, size);
  ana->frq = (FFTFREQS*)xcalloc(size, sizeof(FFTFREQS));
  newFFTFREQS(ana->frq, size);
  FFTwrapper_create(&ana->fft, size);
  ana->frq->size = size;
  *(m_uint*)RETURN = size;
}

static MFUN(fft_compute) {
  m_float* smp;
  Fft* ana = (Fft*)UGEN(o)->module.gen.data;
  if(!ana || ana->sp->pos == ana->last || !ana->buf) {
    *(m_uint*)RETURN = 0;
    return;
  }
  smp = sp_buffer_get(ana->buf);
  if(smp)
    smps2freqs(ana->fft, smp, ana->frq);
  free(smp);
  ana->last = ana->sp->pos;
}

static m_bool import_fft(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "FFT", "UGen"))
  gwi_class_xtor(gwi, fft_ctor, fft_dtor);
  gwi_func_ini(gwi, "int", "init");
  gwi_func_arg(gwi, "int", "size");
  GWI_BB(gwi_func_end(gwi, fft_init, ae_flag_none))
  /*  gwi_func_ini(gwi, "int", "init");
  /*    gwi_func_arg(gwi, "int", "size");*/
  /*    gwi_func_arg(gwi, "float[]", "window");*/
  /*  gwi_func_ini(gwi, "int", "init");
  /*    gwi_func_arg(gwi, "int", "size");*/
  /*    gwi_func_arg(gwi, "string", "window");*/
  /*  gwi_func_ini(gwi, "int", "window");
  /*    gwi_func_arg(gwi, "float[]", "window");*/
  /*  gwi_func_ini(gwi, "int", "window");
  /*    gwi_func_arg(gwi, "string", "name");*/
  /*  gwi_func_ini(gwi, "complex[]", "compute");*/
  gwi_func_ini(gwi, "void", "compute");
  GWI_BB(gwi_func_end(gwi, fft_compute, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

typedef struct Ana {
  m_uint size, sr;
  m_float* fval;
  m_float  percent;     // rollof
  /*  m_float* norm, *prev; // flux*/
  /*  m_float* cval[2];*/ // corr
  struct BBQ_* sp;
  m_uint last;
} Ana;

typedef double (*f_analys)(Ana* fft);
/*
m_float array_max(m_float* f, unsigned int size, unsigned int* index)
{
  unsigned int i;
  m_float max = -INFINITY;
  for(i = 0; i < size; i++) {
    if(f[i] > max) {
      max = f[i];
      *index = i;
    }
  }
  return max;
}
*/
/* from chuck ;-) */
m_float compute_centroid(Ana* fft) {
  m_float m0 = 0.0;
  m_float m1 = 0.0;
  m_float centroid = 0.0;
  unsigned int i;
  /* Compute centroid using moments */
  for(i = 0; i < fft->size / 2; i++) {
    m1 += i * fft->fval[i];
    m0 += fft->fval[i];
  }
  if(m0 != 0.0)
    centroid = m1 / m0;
  else
    centroid = fft->size / 2.0; /* Perfectly balanced */
  return centroid / fft->size;
}

m_float compute_spread(Ana* fft) {
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ((i - mu) * (i - mu)) * fft->fval[i];
  return ret / fft->size;
}

m_float compute_skewness(Ana* fft) {
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ((i - mu) * (i - mu) * (i - mu)) * fft->fval[i];
  return ret / fft->size;
}

m_float compute_kurtosis(Ana* fft) {
  unsigned int i;
  m_float ret = 0;
  m_float mu = compute_centroid(fft);
  for(i = 0; i < fft->size / 2; i++)
    ret += ((i - mu) * (i - mu) * (i - mu) * (i - mu)) * fft->fval[i];
  return ret / fft->size;
}


m_float compute_rms(Ana* fft) {
  m_float rms = 0.0;
  m_uint  i;
  /* get sum of squares */
  for(i = 0; i < fft->size / 2; i++)
    rms += (fft->fval[i] * fft->fval[i]);
  rms /= fft->size;
  rms = sqrt(rms);
  return rms;
}

m_float compute_rolloff(Ana* fft) {
  m_float sum = 0.0, target;
  unsigned int i;
  /* sanity check */
  /* assert(percent >= 0 && percent <= 1); */
  /* iterate */
  for(i = 0; i < fft->size / 2; i++)
    sum += fft->fval[i];
  /* the target */
  target = sum * fft->percent / 100.;
  sum = 0.0;
  /* iterate */
  for(i = 0; i < fft->size; i++) {
    sum += fft->fval[i];
    if(sum >= target)
      break;
  }
  return i / (m_float)fft->size;
}

m_float compute_freq(Ana* fft) {
  unsigned int i;
  m_float max = -0;
  m_float where = 0;
  for(i = 0; i < fft->size / 2; i++) {
    /*    if(fft->cval[i][0] > max)*/
    if(fft->fval[i] > max) {
      /*      max = fft->cval[i][0];*/
      max = fft->fval[i];
      where = i;
    }
  }
  return where / fft->size * fft->sr;
}

m_float compute_asc(Ana* fft) {
  unsigned int i;
  m_float ret = 0.0;
  m_float sum = 0.0;
  for(i = 0; i < fft->size / 2; i++) {
    ret += log2((compute_freq(fft) / 1000.)) * fft->fval[i];
    sum += fft->fval[i];
  }
  return ret / sum;
}

m_float compute_ass(Ana* fft) {
  unsigned int i;
  m_float ret = 0.0;
  m_float sum = 0.0;
  m_float asc = compute_asc(fft);
  for(i = 0; i < fft->size / 2; i++) {
    m_float f;
    f = log2(compute_freq(fft) / 1000.) - asc;
    f *= f;
    f *= fft->fval[i];
    ret += f;
    sum += fft->fval[i];
  }
  return ret / sum;
}

/*
m_float compute_normrms(Ana* fft)
{
  unsigned int i;
  m_float energy = 0.0;

  // get energy
  for(i = 0; i < fft->size/2; i++)
      energy += (fft->fval[i]*fft->fval[i]);

  // check energy
  if (energy == 0.0)
  {
    for(i = 0; i < fft->size/2; i++)
      fft->norm[i] = 0;
    return 0;
  }
  else
      energy = sqrt(energy);

  for(i = 0; i < fft->size/2; i++)
  {
      if(fft->fval[i] > 0.0)
        fft->norm[i] = (fft->fval[i] / energy);
      else
          fft->norm[i] = 0.0;
  }
  return array_max(fft->norm, fft->size/2, i);
}
*/
/*
m_float compute_flux(Ana* fft)
{
  unsigned int i;
  m_float result = 0.0;

  if(fft->prev)
  {
    // compute normalize rms
    compute_normrms(fft);
    // compute flux
    for(i = 0; i < fft->size; i++)
      result += (fft->fval[i] - fft->prev[i])*(fft->fval[i] - fft->prev[i]);
    // take square root of flux
    result = sqrt(result);
  }
  else
  {
    fft->prev = xmalloc(fft->size * sizeof(m_float));
    fft->norm = xmalloc(fft->size * sizeof(m_float));
  }
  // copy curr to prev
  for(i = 0; i < fft->size; i++)
    fft->prev[i] = fft->norm[i];
  return result;
}
*/
/*
m_float compute_corr(Ana* a,Ana* b)
{
  unsigned int i;
  fftw_complex *spec, *a_spec, *b_spec;
  if(a->size != b->size)
    return NAN;
  if(!a->corr)
    a->corr = fftw_xmalloc(a->size * sizeof(m_float));
  // forward fft both input spectrum
  spec    = xmalloc(a->size * sizeof(m_float));
  a_spec  = xmalloc(a->size * sizeof(m_float));
  b_spec  = xmalloc(b->size * sizeof(m_float));
  fftw_execute_dft_r2c(a->plan, a->fval, a_spec);
  fftw_execute_dft_r2c(b->plan, b->fval, b_spec);

  // conjugate a_spec and multiply a and b
  for(i = 0; i < a->size/2; i++)
  {
    a_spec[i][1] *= -1;
    spec[i][0] = (a_spec[i][0]*b_spec[i][0] - a_spec[i][1]*b_spec[i][1]);
    spec[i][1] = (a_spec[i][0]*b_spec[i][1] - a_spec[i][1]*b_spec[i][0]);
  }
  // take the ifft of spec
  if(!a->iplan)
    a->iplan = fftw_plan_dft_c2r_1d(a->size, spec, a->corr, FFTW_PATIENT);
  else
    fftw_execute_dft_c2r(a->iplan, b_spec, a->corr);

  fftw_free(spec);
  fftw_free(a_spec);
  fftw_free(b_spec);
  return array_max(a->corr, a->size/2, i) / a->size;
}
*/
/*
#define __SGN(x)  (x >= 0.0f ? 1.0f : -1.0f)

m_float compute_zerox(Ana* fft, m_float* buffer)
{
  unsigned int i, xings = 0;
  m_float v = 0, p = 0;
  p = buffer[0];
  for(i = 0; i < fft->size; i++)
  {
    v = buffer[i];
    xings += __SGN(v) != __SGN(p);
    p = v;
  }
  return xings/fft->size;
}
*/
m_int o_ana_ana;
m_int o_ana_fft;
m_int o_ana_fn;

static m_float ana_dummy(Fft* fft) {
  return 0.0;
}
static MFUN(ana_compute) {
  M_Object   fft = *(M_Object*)(o->data + o_ana_fft);
  Ana* ana = *(Ana**)(o->data + o_ana_ana);
  f_analys f = *(f_analys*)(o->data + o_ana_fn);
  if(!fft || ana->last == ana->sp->pos)
    return;
  *(m_float*)RETURN = f(ana);
  ana->last = ana->sp->pos;
}

static MFUN(ana_get_fft) {
  *(m_uint*)RETURN = (m_uint) * (M_Object*)(o->data + o_ana_fft);
}

static MFUN(ana_set_fft) {
  Fft* fft;
  M_Object obj = *(M_Object*)(o->data + o_ana_fft);
  Ana* ana = *(Ana**)(o->data + o_ana_ana);
  if(obj)
    release(obj, shred);
  obj = *(M_Object*)MEM(SZ_INT);
  if(!obj) {
    ana->size = 0;
    ana->fval = NULL;
    *(m_uint*)RETURN = 0;
    return;
  }
  fft = (Fft*)UGEN(obj)->module.gen.data;
  if(!fft || !fft->buf) {
    gw_err("FFT probably not initialised.");
    release(obj, shred);
    return;
  }
  ++obj->ref;
  ana->size = fft->fft->fftsize;
  ana->fval = fft->frq->s;
  *(M_Object*)RETURN = *(M_Object*)(o->data + o_ana_fft) = obj;
}

static CTOR(ana_ctor) {
  Ana* ana = *(Ana**)(o->data + o_ana_ana) = (Ana*)xmalloc(sizeof(Ana));
  ana->sr = shred->info->vm->bbq->si->sr;
  ana->percent = 50; // rolloff;
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)ana_dummy;
  ana->sp = shred->info->vm->bbq;
  ana->last = 0;
}

static DTOR(ana_dtor) {
  free(*(Ana**)(o->data + o_ana_ana));
}

static m_bool import_ana(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "ANA", NULL))
  gwi_class_xtor(gwi, ana_ctor, ana_dtor);
  gwi_item_ini(gwi, "@internal", "@_fft");
  o_ana_ana = gwi_item_end(gwi, ae_flag_none, NULL);
  GWI_BB(o_ana_ana)
  gwi_item_ini(gwi,"FFT", "@fft");
  o_ana_fft = gwi_item_end(gwi,  ae_flag_late, NULL);
  GWI_BB(o_ana_fft)
  gwi_item_ini(gwi, "@internal", "@fn");
  o_ana_fn = gwi_item_end(gwi, ae_flag_none, NULL);
  GWI_BB(o_ana_fn)
  gwi_func_ini(gwi, "float", "compute");
  GWI_BB(gwi_func_end(gwi, ana_compute, ae_flag_none))
  gwi_func_ini(gwi, "FFT", "fft");
  GWI_BB(gwi_func_end(gwi, ana_get_fft, ae_flag_none))
  gwi_func_ini(gwi, "FFT", "fft");
  gwi_func_arg(gwi, "FFT", "arg");
  GWI_BB(gwi_func_end(gwi, ana_set_fft, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(centroid_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_centroid;
}
static m_bool import_centroid(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "Centroid", "ANA"))
  gwi_class_xtor(gwi, centroid_ctor, NULL);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(spread_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_spread;
}
static m_bool import_spread(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "Spread", "ANA"))
  gwi_class_xtor(gwi, spread_ctor, NULL);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(skewness_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_skewness;
}
static m_bool import_skewness(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "Skewness", "ANA"))
  gwi_class_xtor(gwi, skewness_ctor, NULL);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(kurtosis_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_kurtosis;
}
static m_bool import_kurtosis(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "Kurtosis", "ANA"))
  gwi_class_xtor(gwi, kurtosis_ctor, NULL);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(rms_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_rms;
}
static m_bool import_rms(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "RMS", "ANA"))
  gwi_class_xtor(gwi, rms_ctor, NULL);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(rolloff_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_rolloff;
}
static MFUN(rolloff_get_percent) {
  Ana* ana = *(Ana**)(o->data + o_ana_ana);
  *(m_float*)RETURN = ana->percent;
}
static MFUN(rolloff_set_percent) {
  Ana* ana = *(Ana**)(o->data + o_ana_ana);
  *(m_float*)RETURN = (ana->percent = *(m_float*)MEM(SZ_INT));
}
static m_bool import_rolloff(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "Rolloff", "ANA"))
  gwi_class_xtor(gwi, rolloff_ctor, NULL);
  gwi_func_ini(gwi, "float", "percent");
  GWI_BB(gwi_func_end(gwi, rolloff_get_percent, ae_flag_none))
  gwi_func_ini(gwi, "float", "percent");
  gwi_func_arg(gwi, "float", "arg");
  GWI_BB(gwi_func_end(gwi, rolloff_set_percent, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(freq_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_freq;
}
static m_bool import_freq(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "Freq", "ANA"))
  gwi_class_xtor(gwi, freq_ctor, NULL);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(asc_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_asc;
}
static m_bool import_asc(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "ASC", "ANA"))
  gwi_class_xtor(gwi, asc_ctor, NULL);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static CTOR(ass_ctor) {
  *(f_analys*)(o->data + o_ana_fn) = (f_analys)compute_ass;
}
static m_bool import_ass(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "ASS", "ANA"))
  gwi_class_xtor(gwi, ass_ctor, NULL);
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

static m_int o_fc_vector;
static CTOR(fc_ctor) {
  *(Vector*)(o->data + o_fc_vector) = new_vector(shred->info->vm->gwion->mp);
}
static DTOR(fc_dtor) {
  free_vector(shred->info->vm->gwion->mp, *(Vector*)(o->data + o_fc_vector));
}

static MFUN(fc_compute) {
  m_uint i;
  M_Object ret;
  Vector v = *(Vector*)(o->data + o_fc_vector);
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_float], 1);
  ret = new_array(shred->info->vm->gwion->mp, t, vector_size(v));
  vector_add(&shred->gc, (vtype)ret);
  for(i = 0; i < vector_size(v); i++) {
    M_Object obj = (M_Object)vector_at(v, i);
//    if(!obj) continue; // prevented in fc.add
    Ana* ana   = *(Ana**)(obj->data + o_ana_ana);
//    if(!_fft) continue; // seems prevented somehow. (this is unclear)
    Fft* fft   = *(Fft**)(obj->data + o_ana_fft);
    if(!fft)
      continue;
    f_analys fn  = *(f_analys*)(obj->data + o_ana_fn);
    m_float f = fn(ana);
    m_vector_set(ARRAY(ret), i, (char*)&f);
  }
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(fc_add) {
  Vector v = *(Vector*)(o->data + o_fc_vector);
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(obj) {
    vector_add(v, (vtype)obj);
    release(obj, shred);
  }
  *(m_uint*)RETURN = (m_uint)obj;
}

static MFUN(fc_rem) {
  Vector v = *(Vector*)(o->data + o_fc_vector);
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(obj) {
    vector_rem(v, vector_find(v, (vtype)obj));
    release(obj, shred);
  }
  *(m_uint*)RETURN = (m_uint)obj;
}

INSTR(fc_connect) {
  POP_REG(shred, SZ_INT * 2);
  M_Object o   = *(M_Object*)REG(0);
  M_Object obj = **(M_Object**)REG(SZ_INT);
  if(o) {
    if(obj) {
      Vector v = *(Vector*)(obj->data + o_fc_vector);
      vector_add(v, (vtype)o);
      release(obj, shred);
    }
    release(o, shred);
  }
  *(M_Object*)REG(0) = obj;
  PUSH_REG(shred, SZ_INT);
}

INSTR(fc_disconnect) {
  POP_REG(shred, SZ_INT * 2);
  M_Object o   = *(M_Object*)REG(0);
  M_Object obj = *(M_Object*)REG(SZ_INT); // WARN inconsistency
  if(o) {
    if(obj) {
      Vector v = *(Vector*)(obj->data + o_fc_vector);
      vector_rem(v, vector_find(v, (vtype)o));
      release(obj, shred);
    }
    release(o, shred);
  }
  *(M_Object*)REG(0) = obj;
  PUSH_REG(shred, SZ_INT);
}

static m_bool import_fc(Gwi gwi) {
  GWI_BB(gwi_class_ini(gwi, "FC", NULL))
  gwi_class_xtor(gwi, fc_ctor, fc_dtor);
  gwi_item_ini(gwi, "@internal", "@vector");
  o_fc_vector = gwi_item_end(gwi, ae_flag_none, NULL);
  GWI_BB(o_fc_vector)
  gwi_func_ini(gwi, "float[]", "compute");
  GWI_BB(gwi_func_end(gwi, fc_compute, ae_flag_none))
  gwi_func_ini(gwi, "ANA", "add");
  gwi_func_arg(gwi, "ANA", "arg");
  GWI_BB(gwi_func_end(gwi, fc_add, ae_flag_none))
  gwi_func_ini(gwi, "ANA", "rem");
  gwi_func_arg(gwi, "ANA", "arg");
  GWI_BB(gwi_func_end(gwi, fc_rem, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}

GWION_IMPORT(analys) {
  GWI_BB(import_fft(gwi))
  GWI_BB(import_ana(gwi))
  GWI_BB(import_centroid(gwi))
  GWI_BB(import_spread(gwi))
  GWI_BB(import_skewness(gwi))
  GWI_BB(import_kurtosis(gwi))
  GWI_BB(import_rms(gwi))
  GWI_BB(import_rolloff(gwi))
  GWI_BB(import_freq(gwi))
  GWI_BB(import_asc(gwi))
  GWI_BB(import_ass(gwi))
  GWI_BB(import_fc(gwi))
  GWI_BB(gwi_oper_ini(gwi, "ANA", "FC", "FC"))
  GWI_BB(gwi_oper_end(gwi, "=>", fc_connect))
  GWI_BB(gwi_oper_ini(gwi, "ANA", "FC", "FC"))
  GWI_BB(gwi_oper_end(gwi, "=<", fc_disconnect))
  return GW_OK;
}
