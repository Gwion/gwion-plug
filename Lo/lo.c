#include <lo/lo.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "traverse.h"
#include "parse.h"
#include "emit.h"
#include "shreduler_private.h"

typedef struct LoArg_ {
  union {
    m_uint  i;
    m_float f;
    m_str   s;
  } data;
  volatile uint32_t   ref;
  char       t;
} * LoArg;

ANN static inline void release_loarg(const MemPool mp, const LoArg arg) {
  if (--arg->ref) return;
  if (arg->t == 's') free_mstr(mp, arg->data.s);
  mp_free(mp, LoArg, arg);
}

struct LoOut {
  struct Vector_ args;
  lo_address     addr;
};
#define LOOUT(o) (*(struct LoOut *)((o)->data))

static MFUN(osc_send) {
  struct LoOut *loout = &LOOUT(o);
  const lo_message msg = lo_message_new();
  for (m_uint i = 0; i < vector_size(&loout->args); i++) {
    const LoArg arg = (LoArg)vector_at(&loout->args, i);
    switch (arg->t) {
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
      handle(shred, "OscError");
      return;
    }
    release_loarg(shred->info->mp, arg);
  }
  vector_clear(&loout->args);
  *(m_uint *)RETURN =
      !!lo_send_message(loout->addr, STRING(*(M_Object *)MEM(SZ_INT)), msg);
  lo_message_free(msg);
}

ANN inline static LoArg new_arg(const MemPool mp) {
  const LoArg arg = (LoArg)mp_malloc(mp, LoArg);
  arg->ref      = 1;
  return arg;
}

//! function factory for setters
#define lo_setter(name, type, input, _data)                                    \
  static INSTR(oscsend_add_##name) {                                           \
    POP_REG(shred, SZ_INT);                                                    \
    const LoArg    arg   = new_arg(shred->info->mp);                           \
    struct LoOut loout = LOOUT((*(M_Object *)REG(0)));                       \
    vector_add(&loout.args, (m_uint)arg);                                      \
    arg->t          = type;                                                    \
    arg->data.input = _data;                                                   \
  }

lo_setter(int, 'i', i, *(m_int *)REG(-SZ_INT));
lo_setter(float, 'd', f, *(m_float *)REG(-SZ_FLOAT));
lo_setter(string, 's', s, mstrdup(shred->info->mp, STRING(*(M_Object *)REG(-SZ_INT))));

static DTOR(loout_dtor) {
  struct LoOut loout = LOOUT(o);
  vector_release(&loout.args);
  lo_address_free(loout.addr);
}

static MFUN(oscout_new0) {
  const m_str url  = STRING(*(M_Object *)MEM(SZ_INT));
  const lo_address addr = lo_address_new_from_url(url);
  if(!addr) {
    handle(shred, "wut?");
    return;
  }
  struct LoOut *  loout = &LOOUT(o);
  vector_init(&loout->args);
  loout->addr = addr;
  *(M_Object*)RETURN = o;
}

static MFUN(oscout_new1) {
  const m_str host  = STRING(*(M_Object *)MEM(SZ_INT));
  const m_int port  = *(m_uint *)MEM(SZ_INT*2);
  char c[256];
  snprintf(c, 256, "%" INT_F, port);
  const lo_address addr = lo_address_new(host, c);
  if(!addr) {
    handle(shred, "wut?");
    return;
  }
  struct LoOut *  loout = &LOOUT(o);
  vector_init(&loout->args);
  loout->addr = addr;
  *(M_Object*)RETURN = o;
}

static MFUN(oscout_new2) {
  const m_uint proto  = *(m_uint *)MEM(SZ_INT);
  const m_str  host   = STRING(*(M_Object *)MEM(SZ_INT*2));
  const m_int  port   = *(m_uint *)MEM(SZ_INT*3);
  char c[256];
  snprintf(c, 256, "%" INT_F, port);
  const lo_address addr = lo_address_new_with_proto(proto, host, c);
  if(!addr) {
    handle(shred, "wut?");
    return;
  }
  struct LoOut *  loout = &LOOUT(o);
  vector_init(&loout->args);
  loout->addr = addr;
  *(M_Object*)RETURN = o;
}

static ANN m_bool import_oscout(const Gwi gwi) {
  gwidoc(gwi, "A type to send Osc events");
  const Type t_loout = gwi_class_ini(gwi, "OscOut", "Object");
  gwi_class_xtor(gwi, NULL, loout_dtor);
  SET_FLAG(t_loout, final);
  t_loout->nspc->offset += sizeof(struct LoOut); // reserve
    gwidoc(gwi, "Send a message to `path`");

    GWI_BB(gwi_enum_ini(gwi, "Proto"));
    GWI_BB(gwi_enum_add(gwi, "UDP",  LO_UDP));
    GWI_BB(gwi_enum_add(gwi, "TCP",  LO_TCP));
    GWI_BB(gwi_enum_add(gwi, "UNIX", LO_UNIX));
    GWI_OB(gwi_enum_end(gwi));

    gwi_func_ini(gwi, "auto", "new");
      gwi_func_arg(gwi, "string", "url");
    GWI_BB(gwi_func_end(gwi, oscout_new0, ae_flag_none))

    gwi_func_ini(gwi, "auto", "new");
      gwi_func_arg(gwi, "string", "host");
      gwi_func_arg(gwi, "int", "port");
    GWI_BB(gwi_func_end(gwi, oscout_new1, ae_flag_none))

    gwi_func_ini(gwi, "auto", "new");
      gwi_func_arg(gwi, "Proto", "proto");
      gwi_func_arg(gwi, "string", "host");
      gwi_func_arg(gwi, "int", "port");
    GWI_BB(gwi_func_end(gwi, oscout_new2, ae_flag_none))

    gwi_func_ini(gwi, "bool", "send");
      gwi_func_arg(gwi, "string", "path");
    GWI_BB(gwi_func_end(gwi, osc_send, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

#define oscout_oper(name)                                                      \
  gwidoc(gwi, "Add an `" #name "` to the message");                            \
  GWI_BB(gwi_oper_ini(gwi, #name, "OscOut", #name))                            \
  GWI_BB(gwi_oper_end(gwi, "=>", oscsend_add_##name))

  oscout_oper(int) oscout_oper(float) oscout_oper(string)
#undef oscout_oper

      return GW_OK;
}

typedef struct LoMethod_ {
  m_uint         ref;
  m_str          path, type;
  struct Vector_ client;
  lo_method      method;
  MemPool        p;
} * LoMethod;

typedef struct LoServer_ {
  m_uint           port, ref;
  lo_server_thread thread;
  struct Vector_   method;
  MemPool          p;
} * LoServer;

ANN static void release_lomethod(const LoServer s, const LoMethod m) {
  if (--m->ref)
    return;
  lo_server_thread_del_method(s->thread, m->path, m->type);
  vector_release(&m->client);
  mp_free(s->p, LoMethod, m);
}

ANN static void release_loserver(const Map map, const LoServer s) {
  if (--s->ref)
    return;
  map_remove(map, (vtype)s->port);
  for (m_uint i = 0; i < vector_size(&s->method); i++)
    release_lomethod(s, (LoMethod)vector_at(&s->method, i));
  vector_release(&s->method);
  lo_server_thread_stop(s->thread);
  lo_server_thread_free(s->thread);
  mp_free(s->p, LoServer, s);
}

struct LoIn {
  struct Vector_ args;
  LoServer       server;
  struct Vector_ methods;
  struct Vector_ curr;
  MUTEX_TYPE     mutex;
  MUTEX_TYPE     bbq;
};
#define LOIN(o) (*(struct LoIn *)((o)->data + SZ_INT))

static void osc_error_handler(int num, const char *msg, const char *where) {
  gw_err("problem %i with osc: %s %s", num, msg, where);
}

static int osc_method_handler(const char *path NUSED, const char *type, lo_arg **argv,
                              int argc, lo_message msg NUSED, void *data) {
  const LoMethod   m = (LoMethod)data;
  struct Vector_ v;
  vector_init(&v);
  for (int i = 0; i < argc; i++) {
    const LoArg arg = new_arg(m->p);
    switch (type[i]) {
    case 'i':
      arg->data.i = argv[i]->i;
      arg->t      = 'i';
      break;
    case 'f':
      arg->data.f = argv[i]->f;
      arg->t      = 'f';
      break;
    case 'd':
      arg->data.f = argv[i]->d;
      arg->t      = 'f';
      break;
    case 's':
      arg->data.s = mstrdup(m->p, (m_str)argv[i]);
      break;
    default:
      for(m_uint i = 0; i < vector_size(&v); i++)
        release_loarg(m->p, (LoArg)vector_at(&v, i));
      vector_release(&v);
      gw_err("unhandled osc arg type '%c'", type[i]);
      return GW_OK;
    }
    vector_add(&v, (vtype)arg);
  }

  for (m_uint i = 0; i < vector_size(&m->client); i++) {
    for (int j = 1; j < argc; j++) {
      const LoArg arg = (LoArg)vector_at(&v, j);
      arg->ref++;
    }
    const M_Object o    = (M_Object)vector_at(&m->client, i);
    struct LoIn    *loin = &LOIN(o);
    struct Vector_ vec = {};
    vector_copy2(&v, &vec);
    MUTEX_LOCK(loin->mutex);
    vector_add(&loin->args, (m_uint)vec.ptr);
    MUTEX_UNLOCK(loin->mutex);
    MUTEX_LOCK(loin->bbq);
    broadcast(o);
    MUTEX_UNLOCK(loin->bbq);

  }

  vector_release(&v);
  return 0;
}

static MFUN(osc_get_port) {
  const LoServer s    = LOIN(o).server;
  *(m_uint *)RETURN = s->port;
}

ANN static inline LoMethod find_lomethod(const Vector v, const m_str path,
                                     const m_str type) {
  for (m_uint i = 0; i < vector_size(v); i++) {
    const LoMethod m = (LoMethod)vector_at(v, i);
    if (!strcmp(m->path, path) && !strcmp(m->type, type)) {
      m->ref++;
      return m;
    }
  }
  return NULL;
}

ANN static inline LoMethod new_lomethod(const LoServer s, const m_str path,
                                      const m_str type) {
  const LoMethod m = (LoMethod)mp_malloc(s->p, LoMethod);
  m->path          = path;
  m->type          = type;
  m->p             = s->p;
  m->ref           = 1;
  m->method =
      lo_server_thread_add_method(s->thread, path, type, osc_method_handler, m);
  vector_init(&m->client);
  vector_add(&s->method, (vtype)m);
  return m;
}

ANN static inline LoMethod get_lomethod(const LoServer s, const m_str path,
                                      const m_str type) {
  return find_lomethod(&s->method, path, type) ?: new_lomethod(s, path, type);
}

ANN static inline void add_lomethod(struct LoIn *const loin, const LoMethod m,
                                  const M_Object o) {
  if (vector_find(&m->client, (vtype)o) < 0) vector_add(&m->client, (vtype)o);
  if (vector_find(&loin->methods, (vtype)m) < 0)
    vector_add(&loin->methods, (vtype)m);
}

static MFUN(osc_add_method) {
  struct LoIn  loin = LOIN(o);
  const m_str  path = STRING(*(M_Object *)MEM(SZ_INT));
  const m_str  type = STRING(*(M_Object *)MEM(SZ_INT * 2));
  const LoMethod m    = get_lomethod(loin.server, path, type);
  add_lomethod(&loin, m, o);
}

ANN static inline void clear_curr(const MemPool mp, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); i++)
    release_loarg(mp, (LoArg)vector_at(v, i));
  vector_release(v);
}

static DTOR(loin_dtor) {
  struct LoIn loin = LOIN(o);
  vector_release(&loin.methods);
  if (loin.curr.ptr)
    clear_curr(shred->info->mp, &loin.curr);
  vector_release(&loin.args);
  const Map map = get_module(shred->info->vm->gwion, "Lo");
  release_loserver(map, loin.server);
  MUTEX_CLEANUP(loin.mutex);
}

static MFUN(osc_recv) {
  struct LoIn *loin = &LOIN(o);
  MUTEX_LOCK(loin->mutex);
  const Vector c_arg = &loin->args;
  if (!vector_size(c_arg)) {
    *(m_uint *)RETURN = false;
    MUTEX_UNLOCK(loin->mutex);
    return;
  }
  if (loin->curr.ptr)
    clear_curr(shred->info->mp, &loin->curr);
  loin->curr.ptr    = (m_uint*)vector_front(c_arg);
  *(m_uint *)RETURN = true;
  vector_rem(c_arg, 0);
  MUTEX_UNLOCK(loin->mutex);
}
/*
static MFUN(oscin_rem) {
  const m_str path = STRING(*(M_Object *)MEM(0));
  const m_str type = STRING(*(M_Object *)MEM(SZ_INT));
  struct LoIn loin = LOIN(o);
  for (m_int i = 0; i < (m_int)vector_size(&loin.methods); i++) {
    const LoMethod m = (LoMethod)vector_at(&loin.methods, i);
    if (!strcmp(m->path, path) && !strcmp(m->type, type)) {
      if ((i = vector_find(&m->client, (vtype)o)) >= 0)
        vector_rem(&m->client, (vtype)i);
      if ((i = vector_find(&loin.methods, (vtype)m)) >= 0)
        vector_rem(&loin.methods, (vtype)i);
      vector_rem(&loin.server->method, i);
      release_lomethod(loin.server, m);
      *(m_uint *)RETURN = 1;
      return;
    }
  }
}
*/
//! function factory for getters
#define lo_getter(name, type, ret)                                             \
  static INSTR(oscin_get_##name) {                                             \
    POP_REG(shred, SZ_INT*2 - sizeof(type));                                   \
    struct LoIn *loin = &LOIN(*(M_Object *)REG(-sizeof(type)));                \
    const Vector c_arg          = &loin->curr;                                 \
    const LoArg  arg            = (LoArg)vector_front(c_arg);                  \
    vector_rem(c_arg, 0);                                                      \
    **(type **)REG(SZ_INT - sizeof(type)) = ret;                               \
    *(type *)REG(-sizeof(type)) = ret;                                         \
    release_loarg(shred->info->mp, arg);                                       \
    if (!vector_size(c_arg)) { vector_release(c_arg); c_arg->ptr = NULL;}      \
  }

lo_getter(int, m_int, arg->data.i);
lo_getter(float, m_float, arg->data.f);
lo_getter(string, M_Object, new_string(shred->info->vm->gwion, arg->data.s));

ANEW ANN static LoServer new_loserver(const MemPool mp, const Map map, const m_int port) {
  char c[256];
  snprintf(c, 256, "%" INT_F, port);
  lo_server_thread lserv = lo_server_thread_new(c, osc_error_handler);
  if (!lserv) return NULL;
  if (lo_server_thread_start(lserv) < 0) {
    lo_server_thread_free(lserv);
    return NULL;
  }
  const LoServer s = (LoServer)mp_malloc(mp, LoServer);
  s->p           = mp;
  s->thread      = lserv;
  s->port = port;
  lo_server_enable_coercion(lo_server_thread_get_server(s->thread), 0);
  vector_init(&s->method);
  s->ref = 0;
  if (!map->ptr) map_init(map);
  map_set(map, (vtype)port, (vtype)s);
  return s;
}

ANN static inline LoServer get_server(const MemPool mp, const Map map, const m_int port) {
  if (map->ptr) {
    const LoServer s = (LoServer)map_get(map, (vtype)port);
    if (s) return s;
  }
  return new_loserver(mp, map, port);
}

static MFUN(osc_new) {
   struct LoIn *  loin = &LOIN(o);
   vector_init(&loin->args);
   vector_init(&loin->methods);
   MUTEX_SETUP(loin->mutex);
   loin->bbq = shred->info->vm->shreduler->mutex;
   const m_int  port = *(m_int *)MEM(SZ_INT);
   const Map _map = get_module(shred->info->vm->gwion, "Lo");
   const LoServer s  = get_server(shred->info->mp, _map, port);
   if (s) {
     *(M_Object *)RETURN = o;
     s->ref++;
     loin->server = s;
     return;
   }
   handle(shred, "OscError");
}

static ANN m_bool import_oscin(const Gwi gwi) {
  gwidoc(gwi, "A type to receive OSC events");
  const Type t_loin = gwi_class_ini(gwi, "OscIn", "Event");
  gwi_class_xtor(gwi, NULL, loin_dtor);
  SET_FLAG(t_loin, final | ae_flag_abstract);
  t_loin->nspc->offset += sizeof(struct LoIn);


  gwidoc(gwi, "constructor");
  gwi_func_ini(gwi, "auto", "new");
  gwi_func_arg(gwi, "int", "port");
  GWI_BB(gwi_func_end(gwi, osc_new, ae_flag_none))

  gwidoc(gwi, "add a new method");
  gwi_func_ini(gwi, "void", "add");
  gwi_func_arg(gwi, "string", "path");
  gwi_func_arg(gwi, "string", "type");
  GWI_BB(gwi_func_end(gwi, osc_add_method, ae_flag_none))

  gwidoc(gwi, "get the port");
  gwi_func_ini(gwi, "int", "port");
  GWI_BB(gwi_func_end(gwi, osc_get_port, ae_flag_none))

  gwidoc(gwi, "Receive an argument from the message");
  gwi_func_ini(gwi, "bool", "recv");
  GWI_BB(gwi_func_end(gwi, osc_recv, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

#define oscin_oper(name)                                                       \
  gwidoc(gwi, "Get an `" #name "` from the message");                          \
  GWI_BB(gwi_oper_ini(gwi, "OscIn", #name, #name))                             \
  GWI_BB(gwi_oper_add(gwi, opck_rassign))                             \
  GWI_BB(gwi_oper_end(gwi, "=>", oscin_get_##name))

  oscin_oper(int);
  oscin_oper(float);
  oscin_oper(string);
#undef oscin_oper

  return GW_OK;
}

GWMODINI(Lo) { return new_map(gwion->mp); }
GWMODEND(Lo) { return free_map(gwion->mp, (Map)self); }

GWION_IMPORT(lo) {
  GWI_BB(import_oscout(gwi));
  GWI_BB(import_oscin(gwi));
  if(!get_module(gwi->gwion, "Lo"))
    set_module(gwi->gwion, "Lo", GWMODINI_NAME(gwi->gwion, NULL));
  return GW_OK;
}

