#include <stdlib.h>
#include <string.h>
#include <lo/lo.h>
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

static Map map = NULL;
struct Arg {
  union {
    m_uint  i;
    m_float f;
    m_str   s;
  } data;
  m_uint ref;
  MUTEX_TYPE mutex;
  char t;
};

struct Method {
  m_uint ref;
  m_str path, type;
  Vector client;
  lo_method method;
  MemPool p;
};

struct Server {
  m_uint port, ref;
  lo_server_thread thread;
  Vector method;
  MemPool p;
};

void release_Arg(struct Arg* arg) {
  MUTEX_LOCK(arg->mutex);
  if(!--arg->ref) {
    if(arg->t == 's')
      free(arg->data.s);
    MUTEX_UNLOCK(arg->mutex);
    MUTEX_CLEANUP(arg->mutex);
    free(arg);
  } else
    MUTEX_UNLOCK(arg->mutex);
}

void release_method(struct Server* s, struct Method* m) {
  if(!--m->ref) {
    lo_server_thread_del_method(s->thread, m->path, m->type);
    vector_rem(s->method, vector_find(s->method, (vtype)m));
    free(m);
  }
}

void release_server(struct Server* s) {
  if(!--s->ref) {
    map_remove(map, (vtype)s->port);
    lo_server_thread_stop(s->thread);
    lo_server_thread_free(s->thread);
    if(!map_size(map))
      free_map(s->p, map);
  }
}

struct Arg* new_Arg(char t, char* stack) {
  struct Arg* arg = (struct Arg*)xcalloc(1, sizeof(struct Arg));
  arg->t = t;
  arg->ref = 1;
  switch(t) {
    case 'i':
      arg->data.i = *(m_int*)  (stack - SZ_INT);
      break;
    case 'd':
      arg->data.f = *(m_float*)(stack - SZ_INT);
      break;
    case 's':
      arg->data.s = STRING(*(M_Object*)  (stack - SZ_INT));
      break;
  }
  return arg;
}

static m_int o_lo_addr, o_lo_args, o_lo_serv, o_lo_meth, o_lo_curr, o_lo_mutex;
#define ADDR(o) *(lo_address*)    (o->data + o_lo_addr)
#define ARGS(o) *(Vector*)        (o->data + o_lo_args)
#define SERV(o) *(struct Server**)(o->data + o_lo_serv)
#define METH(o) *(Vector*)        (o->data + o_lo_meth)
#define CURR(o) *(Vector*)     (o->data + o_lo_curr)
#define CURR(o) *(Vector*)     (o->data + o_lo_curr)
#define MUTEX(o) *(MUTEX_TYPE*)     (o->data + o_lo_mutex)

static MFUN(osc_addr) {
  if(ADDR(o))
    lo_address_free(ADDR(o));
  m_str host = STRING(*(M_Object*)MEM(SZ_INT));
  m_str port = STRING(*(M_Object*)MEM(SZ_INT * 2));
  *(m_uint*)RETURN = (ADDR(o) = lo_address_new(host, port)) ? 1 : 0;
}

static MFUN(osc_send) {
  if(!ADDR(o))
  {
    gw_err("oscsend address not set. shred[%i] exiting.", shred->tick->xid);
    vm_shred_exit(shred);
    return;
  }
  lo_message msg = lo_message_new();
  for(m_uint i = 0; i < vector_size(ARGS(o)); i++)
  {
    struct Arg *arg = (struct Arg*)vector_at(ARGS(o), i);
    switch(arg->t)
    {
      case 'i':
        lo_message_add_int32(msg, arg->data.i);
        break;
      case 'd':
        lo_message_add_double(msg, arg->data.f);
        break;
      case 's':
        lo_message_add_string(msg, arg->data.s);
        break;
      default:
          gw_err("oscsend invalid type: '%c'  in arg '%i'\n", arg->t, i);
          vm_shred_exit(shred);
          return;
    }
    release_Arg(arg);
  }
  vector_clear(ARGS(o));
  *(m_uint*)RETURN = lo_send_message(ADDR(o), STRING(*(M_Object*)MEM(SZ_INT)), msg) ? 1 : 0;
}

static INSTR(oscsend_add_int) {
  POP_REG(shred, SZ_INT);
  vector_add(ARGS((**(M_Object**)REG(0))), (vtype)new_Arg('i', REG(0)));
}

static INSTR(oscsend_add_float) {
  POP_REG(shred, SZ_INT);
  vector_add(ARGS((**(M_Object**)REG(0))), (vtype)new_Arg('d', REG(0)));
}

static INSTR(oscsend_add_string) {
  POP_REG(shred, SZ_INT);
  vector_add(ARGS((**(M_Object**)REG(0))), (vtype)new_Arg('s', REG(0)));
}

static void osc_error_handler(int num, const char *msg, const char *where)
{ gw_err("problem %i with osc: %s %s", num, msg, where); }

static int osc_method_handler(const char* path, const char* type,
  lo_arg **argv, int argc, lo_message msg, void *data) {
  struct Method* m = (struct Method*)data;
  const Vector v = new_vector(m->p);
  for(m_uint i = 0; i < argc; i++) {
    struct Arg* arg = (struct Arg*)xcalloc(1, sizeof(struct Arg));
    arg->ref = 1;
    MUTEX_SETUP(arg->mutex);
    switch(type[i]) {
      case 'i':
        arg->data.i = argv[i]->i;
        arg->t = 'i';
        break;
      case 'f':
        arg->data.f = argv[i]->f;
        arg->t = 'f';
        break;
      case 'd':
        arg->data.f = argv[i]->d;
        arg->t = 'f';
        break;
      case 's':
        arg->data.s = strdup((m_str)argv[i]);
        break;
      default:
        gw_err("unhandled osc arg type '%c'", type[i]);
// leaks vector, at least
        return GW_OK;
    }
    arg->t = type[i];
    vector_add(v, (vtype)arg);
  }
  for(m_uint i = 0; i < vector_size(m->client); i++) {
    for(m_uint j = 0; j < argc; j++) {
      struct Arg* arg = (struct Arg*)vector_at(v, j);
      arg->ref++;
      MUTEX_LOCK(arg->mutex);
    }
    const M_Object o = (M_Object)vector_at(m->client, i);
    MUTEX_LOCK(MUTEX(o));
    const Vector vec = vector_copy(m->p, v);
    vector_add(ARGS(o), (vtype)vec);
    broadcast(o);
    MUTEX_UNLOCK(MUTEX(o));
    for(m_uint j = 0; j < argc; j++) {
      struct Arg* arg = (struct Arg*)vector_at(v, j);
      MUTEX_UNLOCK(arg->mutex);
    }
  }
  for(m_uint j = 0; j < argc; j++) {
    struct Arg* arg = (struct Arg*)vector_at(v, j);
    release_Arg(arg);
  }
  free_vector(m->p, v);
  return 0;
}

static MFUN(osc_get_port) {
  struct Server* s = SERV(o);
  *(m_uint*)RETURN = s ? s->port : -1;
}

static MFUN(osc_port) {
  struct Server* s = NULL;
  /*if(!o)*/
  /*{*/
  /*handle(shred)*/
  /*return;*/
  /*}*/
  m_int port = *(m_int*)MEM(SZ_INT);
  char c[256];
  if(SERV(o))
    release_server(SERV(o));
  if(map)
    s = (struct Server*)map_get(map, (vtype)port);
  if(!s)
  {
    sprintf(c, "%i", port);
    s = (struct Server*)xcalloc(1, sizeof(struct Server));
    s->thread = lo_server_thread_new(c, osc_error_handler);
    s->p = shred->info->mp;
    if(!s->thread)
    {
      free(s);
      vm_shred_exit(shred);
      return;
    }
    lo_server_enable_coercion(lo_server_thread_get_server(s->thread), 0);
    s->method = new_vector(s->p);
    s->ref = 0;
    if(!map)
      map = new_map(s->p);
    map_set(map, (vtype)port, (vtype)s);
  }
  s->ref++;
  SERV(o) = s;
  *(m_uint*)RETURN = s->port = port;
}

static MFUN(osc_add_method) {
  m_uint i;
  struct Method* m = NULL;
  struct Server* s = SERV(o);
  m_str path = STRING(*(M_Object*)MEM(SZ_INT));
  m_str type = STRING(*(M_Object*)MEM(SZ_INT * 2));
  if(!s)
    return;
  for(i = 0; i < vector_size(s->method); i++)
  {
    m = (struct Method*)vector_at(s->method, i);
    if(strcmp(m->path, path))
      continue;
    if(!strcmp(m->type, type))
      goto found;
  }
  m = (struct Method*)xcalloc(1, sizeof(struct Method));
  m->path = path;
  m->type = type;
  m->p = shred->info->mp;
  m->client = new_vector(m->p);
  m->method = lo_server_thread_add_method(s->thread, path, type, osc_method_handler, m);
  vector_add(s->method, (vtype)m);
  m->ref = 0;
found:
  m->ref++;
  if(vector_find(m->client, (vtype)o) < 0)
    vector_add(m->client, (vtype)o);
  if(vector_find(METH(o), (vtype)m) < 0)
    vector_add(METH(o), (vtype)m);
  *(m_uint*)RETURN = 1;
}

static CTOR(lo_ctor){ ARGS(o) = new_vector(shred->info->mp); }
static CTOR(loin_ctor){ METH(o) = new_vector(shred->info->mp); CURR(o) = new_vector(shred->info->mp); MUTEX_SETUP(MUTEX(o)); }
static CTOR(lo_dtor) { free_vector(shred->info->mp, ARGS(o)); }
static CTOR(loout_dtor){ if(ADDR(o))lo_address_free(ADDR(o));}
static CTOR(loin_dtor){ free_vector(shred->info->mp, METH(o)); free_vector(shred->info->mp, CURR(o)); MUTEX_CLEANUP(MUTEX(o)); }

static MFUN(oscin_stop) {
  struct Server* s = SERV(o);
  *(m_uint*)RETURN = lo_server_thread_stop(s->thread);
}

static MFUN(oscin_start) {
  struct Server* s = SERV(o);
  const int ret = lo_server_thread_start(s->thread);
  if(ret < 0) {
    release_server(s);
    *(m_uint*)RETURN = -1;
    return;
  }
  *(m_uint*)RETURN = 1;
}

static MFUN(osc_recv) {
  MUTEX_LOCK(MUTEX(o));
  Vector c_arg = ARGS(o);
  if(!c_arg) {
    *(m_uint*)RETURN = 0;
    MUTEX_UNLOCK(MUTEX(o));
    return;
  }
  if(!vector_size(c_arg)) {
    *(m_uint*)RETURN = 0;
    MUTEX_UNLOCK(MUTEX(o));
    return;
  }
  CURR(o) = (Vector)vector_at(c_arg, 0);
  *(m_uint*)RETURN = !!vector_size(CURR(o));
  vector_rem(ARGS(o), 0);
  MUTEX_UNLOCK(MUTEX(o));
}

static MFUN(oscin_rem) {
  m_str path = STRING(*(M_Object*)MEM(0));
  m_str type = STRING(*(M_Object*)MEM(SZ_INT));
  if(!SERV(o))
    return;
  for(m_uint i = 0; i < vector_size(METH(o)); i++) {
    struct Method *m = (struct Method*)vector_at(METH(o), i);
    if(strcmp(m->path, path))
      continue;
    if(!strcmp(m->type, type)) {
      if((i = vector_find(m->client, (vtype)o)) >= 0)
        vector_rem(m->client, (vtype)i);
      if((i = vector_find(METH(o), (vtype)m)) >= 0)
        vector_rem(METH(o), (vtype)i);
      release_method(SERV(o), m);
      *(m_uint*)RETURN = 1;
      return;
    }
  }
}

static MFUN(oscin_get_i) {
  MUTEX_LOCK(MUTEX(o));
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  *(m_uint*)RETURN = arg->data.i;
  release_Arg(arg);
  vector_rem(c_arg, 0);
  MUTEX_UNLOCK(MUTEX(o));
}

static MFUN(oscin_get_f) {
  MUTEX_LOCK(MUTEX(o));
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  *(m_float*)RETURN = arg->data.f;
  release_Arg(arg);
  vector_rem(c_arg, 0);
  MUTEX_UNLOCK(MUTEX(o));
}

static MFUN(oscin_get_s) {
  MUTEX_LOCK(MUTEX(o));
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->vm->gwion->mp, shred, arg->data.s);
  release_Arg(arg);
  vector_rem(c_arg, 0);
  MUTEX_UNLOCK(MUTEX(o));
}

GWION_IMPORT(lo) {
  const Type t_lo = gwi_class_ini(gwi, "Lo", "Event");
  SET_FLAG(t_lo, abstract);
  gwi_class_xtor(gwi, lo_ctor, lo_dtor);
  gwi_item_ini(gwi, "@internal", "@args");
  o_lo_args = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_lo_args)
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_class_ini(gwi, "OscOut", "Lo"))
  gwi_class_xtor(gwi, NULL, loout_dtor);
  gwi_item_ini(gwi, "@internal", "@addr");
  o_lo_addr = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_lo_addr)
  gwi_func_ini(gwi, "int", "addr");
    gwi_func_arg(gwi, "string", "host");
    gwi_func_arg(gwi, "string", "port");
  GWI_BB(gwi_func_end(gwi, osc_addr, ae_flag_none))
  gwi_func_ini(gwi, "int", "send");
    gwi_func_arg(gwi, "string", "path");
  GWI_BB(gwi_func_end(gwi, osc_send, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_class_ini(gwi, "OscIn", "Lo"))
  gwi_class_xtor(gwi, loin_ctor, NULL);
  gwi_item_ini(gwi, "@internal", "@serv");
  o_lo_serv = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_lo_serv)
  gwi_item_ini(gwi, "@internal", "@meth");
  o_lo_meth = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_lo_meth)
  gwi_item_ini(gwi, "@internal", "@curr");
  o_lo_curr = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_lo_curr)
  gwi_item_ini(gwi, "@internal", "@mutex");
  o_lo_mutex = gwi_item_end(gwi, ae_flag_none, num, 0);
  GWI_BB(o_lo_mutex)
  gwi_func_ini(gwi, "int", "add");
    gwi_func_arg(gwi, "string", "path");
    gwi_func_arg(gwi, "string", "type");
  GWI_BB(gwi_func_end(gwi, osc_add_method, ae_flag_none))

  gwi_func_ini(gwi, "int", "port");
  GWI_BB(gwi_func_end(gwi, osc_get_port, ae_flag_none))
  gwi_func_ini(gwi, "int", "port");
    gwi_func_arg(gwi, "int", "port");
  GWI_BB(gwi_func_end(gwi, osc_port, ae_flag_none))
  gwi_func_ini(gwi, "int", "start");
  GWI_BB(gwi_func_end(gwi, oscin_start, ae_flag_none))
  gwi_func_ini(gwi, "int", "stop");
  GWI_BB(gwi_func_end(gwi, oscin_stop, ae_flag_none))
  gwi_func_ini(gwi, "int", "recv");
  GWI_BB(gwi_func_end(gwi, osc_recv, ae_flag_none))
  gwi_func_ini(gwi, "int", "get_i");
  GWI_BB(gwi_func_end(gwi, oscin_get_i, ae_flag_none))
  gwi_func_ini(gwi, "float", "get_f");
  GWI_BB(gwi_func_end(gwi, oscin_get_f, ae_flag_none))
  gwi_func_ini(gwi, "string", "get_s");
  GWI_BB(gwi_func_end(gwi, oscin_get_s, ae_flag_none))


  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "int", "OscOut", "int"))
  GWI_BB(gwi_oper_end(gwi, "=>", oscsend_add_int))
  GWI_BB(gwi_oper_ini(gwi,"float",  "OscOut", "float"))
  GWI_BB(gwi_oper_end(gwi, "=>", oscsend_add_float))
  GWI_BB(gwi_oper_ini(gwi, "string", "OscOut", "string"))
  GWI_BB(gwi_oper_end(gwi, "=>", oscsend_add_string))

  return GW_OK;
}
