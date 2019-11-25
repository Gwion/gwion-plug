#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "plug.h"
#include "driver.h"

#ifndef SP_ALSA_ACCESS
#define SP_ALSA_ACCESS SND_PCM_ACCESS_RW_INTERLEAVED
#endif

#ifdef USE_DOUBLE
#define ALSA_FORMAT SND_PCM_FORMAT_FLOAT64
#else
#define ALSA_FORMAT SND_PCM_FORMAT_FLOAT
#endif
/*
struct AlsaNi {
  void** buf;
  void* bufi;
};
*/
struct AlsaInfo {
  snd_pcm_t* handle;
  snd_pcm_t *pcm_in,       *pcm_out;
  m_float  **in_buf,  **out_buf;
  void     **_in_buf, **_out_buf;
  void *in_bufi, *out_bufi;
  snd_pcm_uframes_t bufsize;
  uint chan, bufnum;
};

static int sp_alsa_init(Driver* di, const char* device,
    int stream, int mode) {
  struct AlsaInfo* info = (struct AlsaInfo*)di->driver->data;
  static snd_pcm_t* handle;
  snd_pcm_hw_params_t* params;
  int dir = 0;

  if(snd_pcm_open(&handle, device, stream, mode) > 0 || !handle)
    return GW_ERROR;
  snd_pcm_hw_params_alloca(&params);

  if(snd_pcm_hw_params_any(handle, params) > 0) {
    snd_pcm_close(handle);
    return GW_ERROR;
  }
  if(!snd_pcm_hw_params_test_access(handle, params, SP_ALSA_ACCESS))
    snd_pcm_hw_params_set_access(handle, params, SP_ALSA_ACCESS);

  if(!snd_pcm_hw_params_test_format(handle, params, ALSA_FORMAT))
    snd_pcm_hw_params_set_format(handle, params, ALSA_FORMAT);

  if(!snd_pcm_hw_params_test_rate(handle, params, di->si->sr, dir))
    snd_pcm_hw_params_set_rate_near(handle, params, &di->si->sr, &dir);

  if(!snd_pcm_hw_params_test_channels(handle, params, (uint)info->chan))
    snd_pcm_hw_params_set_channels(handle, params, (uint)info->chan);
  else return GW_ERROR;

  if(snd_pcm_hw_params_set_period_size_near(handle, params, &info->bufsize, &dir))
    return GW_ERROR;
  if(snd_pcm_hw_params_set_periods_near(handle, params, &info->bufnum, &dir))
    return GW_ERROR;
  if(snd_pcm_hw_params(handle, params))
    return GW_ERROR;

  snd_pcm_hw_params_get_rate_max(params, &di->si->sr, &dir);
  snd_pcm_hw_params_set_rate_near(handle, params, &di->si->sr, &dir);
  info->handle = handle;
  return GW_OK;
}

static void alsa_run_init(Driver* d) {
  struct AlsaInfo* info = (struct AlsaInfo*)d->driver->data;
  snd_pcm_hwsync(info->pcm_out);
  snd_pcm_start(info->pcm_out);
  snd_pcm_hwsync(info->pcm_in);
  snd_pcm_start(info->pcm_in);
}

static DRVINI(alsa_ini) {
  struct AlsaInfo* info = (struct AlsaInfo*)xcalloc(1, sizeof(struct AlsaInfo));
di->si->arg = "default:CARD=CODEC";
info->chan = di->si->out;
  di->driver->data = info;
  if(sp_alsa_init(di, di->si->arg, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
    gw_err("problem with playback");
    return GW_ERROR;
  }
  info->pcm_out = info->handle;
  di->si->out = info->chan;
info->chan = di->si->in;
  if(sp_alsa_init(di, di->si->arg, SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC |
      SND_PCM_NONBLOCK) < 0) {
    gw_err("problem with capture");
    return GW_ERROR;
  }
  info->pcm_in = info->handle;
  di->si->in = info->chan;
  return GW_OK;
}

static void alsa_run_init_non_interleaved(Driver* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->driver->data;
  info->in_buf  = (m_float**)xcalloc(di->si->in, SZ_FLOAT);
  info->out_buf = (m_float**)xcalloc(di->si->in, SZ_FLOAT);
  info->_out_buf   = (void**)xcalloc(di->si->out, SZ_INT);
  info->_in_buf    = (void**)xcalloc(di->si->out, SZ_INT);
  for(m_uint i = 0; i < (m_uint)di->si->out; i++) {
    info->out_buf[i]  = (m_float*)xcalloc(info->bufsize, SZ_FLOAT);
    info->_out_buf[i] = info->out_buf[i];
  }
  for(m_uint i = 0; i < (m_uint)di->si->in; i++) {
    info->in_buf[i]   = (m_float*)xcalloc(info->bufsize, SZ_FLOAT);
    info->_in_buf[i]  = info->in_buf[i];
  }
}

static void alsa_run_non_interleaved(VM *vm, Driver *di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->driver->data;
  while(di->is_running) {
    snd_pcm_readn(info->pcm_in, info->_in_buf, info->bufsize);
    LOOP_OPTIM
    for(m_uint i = 0; i < info->bufsize; i++) {
      for(m_uint chan = 0; chan < (m_uint)di->si->in; chan++)
        di->in[chan] = ((m_float**)(info->_in_buf))[chan][i];
      di->run(vm);
      for(m_uint chan = 0; chan < (m_uint)di->si->out; chan++)
        info->out_buf[chan][i] = di->out[chan];
      ++di->pos;
    }
    if(snd_pcm_writen(info->pcm_out, info->_out_buf, info->bufsize) < 0)
      snd_pcm_prepare(info->pcm_out);
  }
}

static void alsa_run_interleaved(VM* vm, Driver* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->driver->data;
  while(di->is_running) {
    m_int j = 0, k = 0;
    snd_pcm_readi(info->pcm_in, info->in_bufi, info->bufsize);
    LOOP_OPTIM
    for(m_uint i = 0; i < info->bufsize; i++) {
      LOOP_OPTIM
      for(m_uint chan = 0; chan < (m_uint)di->si->in; chan++)
        di->in[chan] = ((m_float*)(info->in_bufi))[j++];
      di->run(vm);
      LOOP_OPTIM
      for(m_uint chan = 0; chan < (m_uint)di->si->out; chan++)
        ((m_float*)info->out_bufi)[k++] = di->out[chan];
      ++di->pos;
    }
    if(snd_pcm_writei(info->pcm_out, info->out_bufi, info->bufsize) < 0)
      snd_pcm_prepare(info->pcm_out);
  }
}

static DRVRUN(alsa_run) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->driver->data;
  alsa_run_init(di);
  if(SP_ALSA_ACCESS == SND_PCM_ACCESS_RW_NONINTERLEAVED) {
    alsa_run_init_non_interleaved(di);
    alsa_run_non_interleaved(vm, di);
  } else {
    info->in_bufi  = (void*)xcalloc(di->si->in * info->bufsize, SZ_FLOAT);
    info->out_bufi = (void*)xcalloc(di->si->out * info->bufsize, SZ_FLOAT);
    alsa_run_interleaved(vm, di);
  }
}

static void alsa_del_non_interleaved(Driver *di, struct AlsaInfo* info) {
  if(info->in_buf && info->out_buf) {
    for(m_uint chan = 0; chan < (m_uint)di->si->in; chan++)
      free(info->in_buf[chan]);
    for(m_uint chan = 0; chan < (m_uint)di->si->out; chan++)
      free(info->out_buf[chan]);
    free(info->in_buf);
    free(info->out_buf);
  }
  if(info->_in_buf)
    free(info->_in_buf);
  if(info->_out_buf)
    free(info->_out_buf);
}

static DRVDEL(alsa_del) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->driver->data;
  if(info->pcm_in)
    snd_pcm_close(info->pcm_in);
  if(info->pcm_out)
    snd_pcm_close(info->pcm_out);
  snd_config_update_free_global();
  if(SP_ALSA_ACCESS == SND_PCM_ACCESS_RW_NONINTERLEAVED)
    alsa_del_non_interleaved(di, info);
  else {
    if(info->in_bufi)
      free(info->in_bufi);
    if(info->out_bufi)
      free(info->out_bufi);
  }
  free(info);
}
GWMODSTR(alsa)
GWDRIVER(alsa) {
  d->ini = alsa_ini;
  d->run = alsa_run;
  d->del = alsa_del;
}
