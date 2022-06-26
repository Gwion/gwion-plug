#include <spa/param/audio/format-utils.h>
#include <spa/param/latency-utils.h>
#include <pipewire/pipewire.h>

#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "shreduler_private.h"

#define BUFFER_SIZE 124

#ifndef USE_DOUBLE
#define GW_FORMAT SPA_AUDIO_FORMAT_F32
#else
#define GW_FORMAT SPA_AUDIO_FORMAT_F64
#endif

struct port {
  struct data *data;
};

struct data {
  struct pw_main_loop *loop;
  struct pw_filter *filter;
  struct port **in_port;
  struct port **out_port;
  m_float **in;
  m_float **out;
  VM* vm;
  uint8_t buffer[];
};


static void do_quit(void *userdata, int signal_number NUSED) {
  struct data* data = (struct data*)userdata;
  data->vm->bbq->is_running = 0;
  pw_main_loop_quit(data->loop);
}

static void on_process(void *userdata, struct spa_io_position *position) {
  struct data *data = userdata;
  uint32_t n_samples = position->clock.duration;
  const VM *vm = data->vm;
  const Driver* bbq = vm->bbq;
  if(!bbq->is_running) {
    pw_main_loop_quit(data->loop);
    return;
  }
  for(uint8_t j = 0; j < bbq->si->in; j++)
    data->in[j] = pw_filter_get_dsp_buffer(data->in_port[j], n_samples);
  for(uint8_t j = 0; j < bbq->si->out; j++)
    data->out[j] = pw_filter_get_dsp_buffer(data->out_port[j], n_samples);
  for(uint32_t i = 0; i < n_samples; i++) {
    for(uint8_t j = 0; j < bbq->si->in; j++)
      bbq->in[j] = data->in[j] ? data->in[j][i] : 0.0;
    vm->bbq->run(vm);
    for(uint8_t j = 0; j < bbq->si->out; j++)
      if(data->out[j]) data->out[j][i] = bbq->out[j];
    next_bbq_pos(vm);
  }
}

static const struct pw_filter_events filter_events = {
        PW_VERSION_FILTER_EVENTS,
        .process = on_process,
};

static m_bool pw_ini(VM* vm, Driver* di) {
  int argc; char **argv;
  pw_init(&argc, &argv);
  struct data *data = mp_malloc2(vm->gwion->mp, sizeof(struct data) + BUFFER_SIZE);
  data->loop = pw_main_loop_new(NULL);
  data->vm = vm;
  pw_loop_add_signal(pw_main_loop_get_loop(data->loop), SIGINT, do_quit, data);
  pw_loop_add_signal(pw_main_loop_get_loop(data->loop), SIGTERM, do_quit, data);
  data->filter = pw_filter_new_simple(
                   pw_main_loop_get_loop(data->loop),
                   "audio-filter",
                   pw_properties_new(
                     PW_KEY_MEDIA_TYPE, "Audio",
                     PW_KEY_MEDIA_CATEGORY, "Filter",
                     PW_KEY_MEDIA_ROLE, "DSP",
                     PW_KEY_APP_NAME, "Gwion",
                     NULL),
                   &filter_events,
                   data);
  di->driver->data = data;
  return GW_OK;
}

ANN static struct port** mk_port(MemPool mp, struct pw_filter *filter, const m_str name, const uint8_t n, const enum spa_direction dir) {
  struct port **ports = mp_calloc2(mp, n * sizeof(struct port*));
  char c[256];
  for(uint8_t i = 0; i < n; i++) {
    sprintf(c, "%s%u\n", name, i);
    ports[i] = pw_filter_add_port(filter, dir,
        PW_FILTER_PORT_FLAG_MAP_BUFFERS,
        sizeof(struct port),
        pw_properties_new(
#ifndef USE_DOUBLE
          PW_KEY_FORMAT_DSP, "32 bit float mono audio",
#else
          PW_KEY_FORMAT_DSP, "64 bit float mono audio",
#endif
          PW_KEY_PORT_NAME, c,
          NULL),
        NULL, 0);
  }
  return ports;
}

static void pw_run(VM* vm, Driver* di) {
  struct data *data = (struct data*)di->driver->data;
  const MemPool mp = vm->gwion->mp;
  data->in_port = mk_port(mp, data->filter, "input", di->si->in, PW_DIRECTION_INPUT);
  data->out_port = mk_port(mp, data->filter, "output", di->si->out, PW_DIRECTION_OUTPUT);
  data->in = mp_calloc2(mp, di->si->in * sizeof(m_float*));
  data->out = mp_calloc2(mp, di->si->out * sizeof(m_float*));
  struct spa_pod_builder b = SPA_POD_BUILDER_INIT(data->buffer, BUFFER_SIZE);
  const struct spa_pod *params[1];
  params[0] = spa_process_latency_build(&b,
                  SPA_PARAM_ProcessLatency,
                  &SPA_PROCESS_LATENCY_INFO_INIT(
                  .ns = 10 * SPA_NSEC_PER_MSEC
                  ));

  if (pw_filter_connect(data->filter,
                        PW_FILTER_FLAG_RT_PROCESS,
                        params, 1) < 0) {
    fprintf(stderr, "can't connect\n");
    return;
  }
  pw_main_loop_run(data->loop);
}

static void pw_del(VM* vm __attribute__((unused)), Driver* di) {
  struct data* data = (struct data*)di->driver->data;
  if(!data) return;
  pw_filter_destroy(data->filter);
  pw_main_loop_destroy(data->loop);
  mp_free2(vm->gwion->mp, di->si->in * sizeof(struct port*) + BUFFER_SIZE, data->in_port);
  mp_free2(vm->gwion->mp, di->si->out * sizeof(struct port*) + BUFFER_SIZE, data->out_port);
  mp_free2(vm->gwion->mp, di->si->in * sizeof(m_float*) + BUFFER_SIZE, data->in);
  mp_free2(vm->gwion->mp, di->si->out * sizeof(m_float*) + BUFFER_SIZE, data->out);
  mp_free2(vm->gwion->mp, sizeof(struct data) + BUFFER_SIZE, data);
  pw_deinit();
}

GWDRIVER(pipewire) {
  d->ini = pw_ini;
  d->run = pw_run;
  d->del = pw_del;
}
