#define _GNU_SOURCE
#include <spa/param/audio/format-utils.h>
#include <spa/param/latency-utils.h>
#include <pipewire/pipewire.h>

#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "shreduler_private.h"

#include "instr.h"
#include "operator.h"
#include "object.h"
#include "import.h"

#include "gack.h"

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
    sprintf(c, "%s%u", name, i);
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

struct roundtrip_data {
  int pending;
  struct pw_main_loop *loop;
};

static void on_core_done(void *data, uint32_t id, int seq) {
  struct roundtrip_data *d = data;
  if (id == PW_ID_CORE && seq == d->pending)
    pw_main_loop_quit(d->loop);
}

static void roundtrip(struct pw_core *core, struct pw_main_loop *loop) {
  static const struct pw_core_events core_events = {
    PW_VERSION_CORE_EVENTS,
    .done = on_core_done,
  };

  struct roundtrip_data d = { .loop = loop };
  struct spa_hook core_listener;
  pw_core_add_listener(core, &core_listener, &core_events, &d);
  d.pending = pw_core_sync(core, PW_ID_CORE, 0);
  pw_main_loop_run(loop);
  spa_hook_remove(&core_listener);
}

typedef struct {
  const Type t_node;
  const Type t_props;
  M_Object ret;
  Gwion gwion;
} PwArg;

typedef struct {
  M_Object key;
  M_Object val;
} PwProp;

typedef struct {
  struct pw_main_loop *loop;
  struct pw_context *context;
  struct pw_core *core;
  struct pw_registry *registry;
  struct remote_data *current;
  struct pw_map vars;
} Pw;

static void registry_event_global(void *data, uint32_t id,
                uint32_t permissions, const char *type, uint32_t version,
                const struct spa_dict *props) {
  PwArg *pwarg = data;
  const M_Object o = new_object(pwarg->gwion->mp, pwarg->t_node);
  *(m_int*)(o->data) = id;
  *(m_int*)(o->data + SZ_INT) = permissions;
  *(M_Object*)(o->data + SZ_INT*2) = new_string(pwarg->gwion, (m_str)type);
  *(m_int*)(o->data + SZ_INT*3) = version;
  const M_Object gwprops = new_array(pwarg->gwion->mp, pwarg->t_props, props->n_items);
  *(M_Object*)(o->data + SZ_INT*4) = gwprops;
  const struct spa_dict_item *item;
  uint32_t i = 0;
  spa_dict_for_each(item, props) {
    PwProp prop = {
      .key = new_string(pwarg->gwion, (m_str)item->key),
      .val = new_string(pwarg->gwion, (m_str)item->value)
    };
    m_vector_set(ARRAY(gwprops), i++, &prop);
  }
o->ref++; //???
  m_vector_add(ARRAY(pwarg->ret), &o);
}

ANN static int run_list(void *data) {
  VM_Shred shred = (VM_Shred)data;
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
  const M_Object o = *(M_Object*)MEM(SZ_INT);
  Pw *pw = (Pw*)o->data;
  const M_Object ret = new_array(shred->info->mp, code->ret_type, 0);
  const Type t_node = array_base(code->ret_type);
  PwArg pwarg = {
    .t_node = t_node,
    .t_props = (Type)vector_at(&t_node->info->tuple->contains, 2),
    .ret = ret,
    .gwion = shred->info->vm->gwion
  };
  struct spa_hook registry_listener;
  static const struct pw_registry_events registry_events = {
    PW_VERSION_REGISTRY_EVENTS,
    .global = registry_event_global,
  };
  pw_registry_add_listener(pw->registry, &registry_listener, &registry_events, &pwarg);
  roundtrip(pw->core, pw->loop);
  spa_hook_remove(&registry_listener);
  shredule(shred->tick->shreduler, shred, GWION_EPSILON);
  *(M_Object*)REG(0) = ret;
  shred->reg += SZ_INT;
  return 0;
}

#include "threads.h"
static MFUN(pw_list) {
  thrd_t thrd;
  thrd_create(&thrd, run_list, shred);
  thrd_detach(thrd);
  shreduler_remove(shred->tick->shreduler, shred, 0);
}

static SFUN(pw_connect) {
  char *cmd;
  gw_asprintf(shred->info->mp, &cmd, "/usr/bin/pw-link %s 2>&1", STRING(*(M_Object*)MEM(0)));
  FILE *fp = popen(cmd, "r");
  if (!fp) {
    handle(shred, "Failed to run pw-link command" );
    return;
  }

  struct GwText_ txt = { .mp = shred->info->mp };
  char path[1035];
  while (fgets(path, sizeof(path), fp) != NULL)
    text_add(&txt, path);
  pclose(fp);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, txt.str);
  free_mstr(shred->info->mp, cmd);
}

static CTOR(pw_ctor) {
  int argc;
  char **argv;
  pw_init(&argc, &argv);
  Pw *pw = (Pw*)o->data;
  pw->loop = pw_main_loop_new(NULL);
  pw->context = pw_context_new(pw_main_loop_get_loop(pw->loop), NULL, 0);
  pw->core = pw_context_connect(pw->context, NULL, 0);
  pw->registry = pw_core_get_registry(pw->core, PW_VERSION_REGISTRY, sizeof(PwArg));
}

static DTOR(pw_dtor) {
  Pw *pw = (Pw*)o->data;
  pw_proxy_destroy((struct pw_proxy*)pw->registry);
  pw_core_disconnect(pw->core);
  pw_context_destroy(pw->context);
  pw_main_loop_destroy(pw->loop);
}

GWION_IMPORT(pipewire) {
  const Type t_pw = gwi_class_ini(gwi, "Pipewire", "Object");
  gwi_class_xtor(gwi, pw_ctor, pw_dtor);
  t_pw->nspc->offset += sizeof(Pw);
  GWI_OB(gwi_struct_ini(gwi, "Prop"));
  GWI_BB(gwi_item_ini(gwi, "string", "key"));
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0));
  GWI_BB(gwi_item_ini(gwi, "string", "value"));
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0));
  GWI_OB(gwi_struct_end(gwi));
  const Type t_node = gwi_class_ini(gwi, "Node", "Object");
  SET_FLAG(t_node, abstract);
  GWI_BB(gwi_item_ini(gwi, "int", "id"));
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0));
  GWI_BB(gwi_item_ini(gwi, "int", "permissions"));
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0));
  GWI_BB(gwi_item_ini(gwi, "string", "type"));
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0));
  GWI_BB(gwi_item_ini(gwi, "int", "version"));
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0));
  GWI_BB(gwi_item_ini(gwi, "Prop[0]", "props"));
  GWI_BB(gwi_item_end(gwi, ae_flag_const, num, 0));
  GWI_OB(gwi_class_end(gwi));
  GWI_BB(gwi_func_ini(gwi, "Node[]", "list"));
  GWI_BB(gwi_func_end(gwi, pw_list, ae_flag_none));
  GWI_BB(gwi_func_ini(gwi, "string", "connect"));
  GWI_BB(gwi_func_arg(gwi, "string", "argument"));
  GWI_BB(gwi_func_end(gwi, pw_connect, ae_flag_static));
  GWI_OB(gwi_class_end(gwi));
  return GW_OK;
}
