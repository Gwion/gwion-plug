#include <pulse/simple.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "plug.h"
#include "driver.h"

#define BUFSIZE 1024

struct PaInfo {
  pa_simple* in;
  pa_simple* out;
};

static pa_simple* pa_open(m_uint direction, pa_sample_spec* ss) {
  return pa_simple_new(NULL, "Gwion", direction,
    NULL, "Gwion", ss, NULL, NULL, NULL);
}

#ifdef USE_DOUBLE
#define PA_FLOAT PA_SAMPLE_FLOAT64NE
#else
#define PA_FLOAT PA_SAMPLE_FLOAT32NE
#endif

static DRVINI(pa_ini) {
  struct PaInfo* info = (struct PaInfo*)xmalloc(sizeof(struct PaInfo));
  bool error = false;
  if(di->si->in) {
    pa_sample_spec ss_in = { PA_FLOAT, di->si->sr, di->si->in};
    if(!(info->in = pa_open(PA_STREAM_RECORD, &ss_in))) {
      gw_err("{-}[{0}{+R}pulseaudio input error{0}{-}]{0}\n");
      error = true;
    }
  } else
    info->in = NULL;
  if(di->si->out) {
    pa_sample_spec ss_out = { PA_FLOAT, di->si->sr, di->si->out};
    if(!(info->out  = pa_open(PA_STREAM_PLAYBACK,   &ss_out))) {
      gw_err("{-}[{0}{+R}pulseaudio output error{0}{-}]{0}\n");
      error = true;
    }
  } else
    info->out = NULL;
  di->driver->data = info;
  return !error ? GW_OK : GW_ERROR;
}

static DRVRUN(pa_run) {
  int error;
  struct PaInfo* info = (struct PaInfo*)di->driver->data;
  size_t in_sz = BUFSIZE * vm->bbq->si->in * sizeof(m_float);
  size_t out_sz = BUFSIZE * vm->bbq->si->out * sizeof(m_float);
  float  *in_data = xmalloc(out_sz);
  float *out_data = xmalloc(out_sz);
  while(vm->bbq->is_running) {
    if(info->in && pa_simple_read(info->in, in_data, in_sz, &error) < 0)
      return;
    for(uint frame = 0; frame < BUFSIZE; frame++) {
      for(uint chan = 0; chan < (m_uint)vm->bbq->si->in; chan++)
        vm->bbq->in[chan] = in_data[frame * vm->bbq->si->in + chan];
      di->run(vm);
      for(uint chan = 0; chan < (m_uint)vm->bbq->si->out; chan++)
        out_data[frame * vm->bbq->si->out + chan] = vm->bbq->out[chan];
      next_bbq_pos(vm);
    }
    if(info->out && pa_simple_write(info->out, out_data, out_sz, &error) < 0)
      break;
  }
  if(info->out)
    free(in_data);
  if(info->out) {
    (void)pa_simple_drain(info->out, &error);
    free(out_data);
  }
}

static DRVDEL(pa_del) {
  struct PaInfo* info = (struct PaInfo*)di->driver->data;
  if(!info)
    return;
  if(info->in)
    pa_simple_free(info->in);
  if(info->out)
    pa_simple_free(info->out);
  xfree(info);
}

GWDRIVER(pulseaudio) {
  d->ini = pa_ini;
  d->run = pa_run;
  d->del = pa_del;
}
