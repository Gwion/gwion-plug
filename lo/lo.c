#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

#include <string.h>
#include <lo/lo.h>

static Map map = NULL;
struct Arg
{
  union
  {
    m_uint  i;
    m_float f;
    m_str   s;
  } data;
  char t;
  m_uint ref;
};

struct Method
{
  m_uint ref;
  m_str path, type;
  Vector client;
  lo_method method;
};

struct Server
{
  m_uint port, ref;
  lo_server_thread thread;
  Vector method;
};

void release_Arg(struct Arg* arg)
{
  if(!--arg->ref)
    free(arg);
}
void release_method(struct Server* s, struct Method* m)
{
  if(!--m->ref)
  {
    lo_server_thread_del_method(s->thread, m->path, m->type);
    vector_remove(s->method, vector_find(s->method, (vtype)m));
    free(m);
  }
}

void release_server(struct Server* s)
{
  if(!--s->ref)
  {
    map_remove(map, (vtype)s->port);
    lo_server_thread_stop(s->thread);
    lo_server_thread_free(s->thread);
    if(!map_size(map))
      free_Map(map);
  }
}

struct Arg* new_Arg(char t, char* stack)
{
  struct Arg* arg = calloc(1, sizeof(struct Arg));
  arg->t = t;
  arg->ref = 1;
  switch(t)
  {
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

static struct Type_ t_lo       = { "Lo",     SZ_INT, &t_event };
static struct Type_ t_loin     = { "OscIn",  SZ_INT, &t_lo };
static struct Type_ t_loout    = { "OscOut", SZ_INT, &t_lo };

static m_int o_lo_addr, o_lo_args, o_lo_serv, o_lo_meth, o_lo_curr;
#define ADDR(o) *(lo_address*)    (o->d.data + o_lo_addr)
#define ARGS(o) *(Vector*)        (o->d.data + o_lo_args)
#define SERV(o) *(struct Server**)(o->d.data + o_lo_serv)
#define METH(o) *(Vector*)        (o->d.data + o_lo_meth)
#define CURR(o) *(Vector*)     (o->d.data + o_lo_curr)

static MFUN(osc_addr)
{
  if(ADDR(o))
    lo_address_free(ADDR(o));
  m_str host = STRING(*(M_Object*)(shred->mem + SZ_INT)); 
  m_str port = STRING(*(M_Object*)(shred->mem + SZ_INT * 2));
  RETURN->d.v_uint = (ADDR(o) = lo_address_new(host, port)) ? 1 : 0;
}

static MFUN(osc_send)
{
  m_uint i;
  struct Arg* arg;
  lo_message msg;
  if(!ADDR(o))
  {
    err_msg(INSTR_, 0, "oscsend address not set. shred[%i] exiting.", shred->xid);
    shred->is_running = 0;
    shred->is_done = 1;
    return;
  }
  msg = lo_message_new();
  for(i = 0; i < vector_size(ARGS(o)); i++)
  {
    arg = (struct Arg*)vector_at(ARGS(o), i);
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
          err_msg(INSTR_, 0, "oscsend invalid type: '%c'  in arg '%i'\n", arg->t, i);
          shred->is_done = 1;
          shred->is_running = 0;
          return;
    }
    release_Arg(arg);
  }
  vector_clear(ARGS(o));
  RETURN->d.v_uint = lo_send_message(ADDR(o), STRING(*(M_Object*)(shred->mem + SZ_INT)), msg) ? 1 : 0;
}

static INSTR(oscsend_add_int)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int => oscsend");
#endif
  shred->reg -= SZ_INT;
  vector_append(ARGS((**(M_Object**)shred->reg)), (vtype)new_Arg('i', shred->reg));
  release(**(M_Object**)shred->reg, shred);
}

static INSTR(oscsend_add_float)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "float => oscsend");
#endif
  shred->reg -= SZ_INT;
  vector_append(ARGS((**(M_Object**)shred->reg)), (vtype)new_Arg('d', shred->reg));
  release(**(M_Object**)shred->reg, shred);
}

static INSTR(oscsend_add_string)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "string => oscsend");
#endif
  shred->reg -= SZ_INT;
  vector_append(ARGS((**(M_Object**)shred->reg)), (vtype)new_Arg('s', shred->reg));
  release(**(M_Object**)shred->reg, shred);
}

static void osc_error_handler(int num, const char *msg, const char *where)
{ err_msg(INSTR_, 0, "problem %i with osc: %s %s", num, msg, where); }

static int osc_method_handler(const char* path, const char* type,
  lo_arg **argv, int argc, lo_message msg, void *data)
{
  m_uint i, j;
  struct Method* m = (struct Method*)data;
  M_Object o;
  Vector v;
  
  v = new_Vector_fixed(argc);
  for(i = 0; i < argc; i++)
  {
    struct Arg* arg = malloc(sizeof(struct Arg));
    arg->ref = 1;
    switch(type[i])
    {
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
        err_msg(INSTR_, 0, "unhandled osc arg type '%c'", type[i]);
        return 1;
    }
    arg->t = type[i];
    vector_set(v, i, (vtype)arg);
  }
  for(i = 0; i < vector_size(m->client); i++)
  {
    for(j = 0; j < argc; j++)
      ((struct Arg*)vector_at(v, j))->ref++;
    o = (M_Object)vector_at(m->client, i);
    broadcast(o);
    vector_append(ARGS(o), (vtype)vector_copy(v));
  }
  for(i = 0; i < argc; i++)
    release_Arg((struct Arg*)vector_at(v, i));
  free_Vector(v);
  return 0;
}

static MFUN(osc_get_port)
{
  struct Server* s = SERV(o);
  RETURN->d.v_uint = s ? s->port : -1;
}

static MFUN(osc_port)
{
  struct Server* s = NULL;
  if(!o)
  {
    Except(shred)
    return;
  }
  m_int port = *(m_int*)(shred->mem + SZ_INT);
  char c[256];
  if(SERV(o))
    release_server(SERV(o));
  if(map)
    s = (struct Server*)map_get(map, (vtype)port);
  if(!s)
  {
    sprintf(c, "%i", port);
    s = calloc(1, sizeof(struct Server));
    s->thread = lo_server_thread_new(c, osc_error_handler);
    if(!s->thread)
    {
      free(s);
      shred->is_done = 1;
      shred->is_running= 0;
      return;
    }
    lo_server_enable_coercion(lo_server_thread_get_server(s->thread), 0);
    s->method = new_Vector();
    s->ref = 0;
    if(!map)
      map = new_Map();
    map_set(map, (vtype)port, (vtype)s);
  }
  s->ref++;
  SERV(o) = s;
  RETURN->d.v_uint = s->port = port;
}

static MFUN(osc_add_method)
{
  m_uint i;
  struct Method* m = NULL;
  struct Server* s = SERV(o);
  m_str path = STRING(*(M_Object*)(shred->mem + SZ_INT));
  m_str type = STRING(*(M_Object*)(shred->mem + SZ_INT * 2));
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
  m = (struct Method*)calloc(1, sizeof(struct Method));
  m->path = path;
  m->type = type;
  m->client = new_Vector();
  m->method = lo_server_thread_add_method(s->thread, path, type, osc_method_handler, m);
  vector_append(s->method, (vtype)m);
  m->ref = 0;
found:
  m->ref++;
  if(vector_find(m->client, (vtype)o) < 0)
    vector_append(m->client, (vtype)o);
  if(vector_find(METH(o), (vtype)m) < 0)
    vector_append(METH(o), (vtype)m);
  RETURN->d.v_uint = 1;
}

CTOR(lo_ctor){ ARGS(o) = new_Vector(); }
CTOR(loin_ctor){ METH(o) = new_Vector(); CURR(o) = new_Vector();}
CTOR(lo_dtor) { free_Vector(ARGS(o)); }
CTOR(loout_dtor){ if(ADDR(o))lo_address_free(ADDR(o));}
CTOR(loin_dtor){ free_Vector(METH(o)); free_Vector(CURR(o));}

static MFUN(oscin_stop)
{
  struct Server* s = SERV(o);
  RETURN->d.v_uint = lo_server_thread_stop(s->thread);
}

static MFUN(oscin_start)
{
  struct Server* s = SERV(o);
  if(lo_server_thread_start(s->thread) < 0)
  {
    release_server(s);
    RETURN->d.v_uint = -1;
    return;
  }
  RETURN->d.v_uint = 1;
}

static MFUN(osc_recv)
{
  Vector c_arg = ARGS(o);
  CURR(o) = (Vector)vector_at(c_arg, 0);
  RETURN->d.v_uint = CURR(o) ? 1 : 0;
  vector_remove(ARGS(o), 0);
}
static MFUN(oscin_rem)
{
  m_uint i;
  struct Method* m = NULL;
  shred->reg -= SZ_INT*2;
  m_str path = STRING(*(M_Object*)(shred->mem));
  m_str type = STRING(*(M_Object*)(shred->mem + SZ_INT));
  if(!SERV(o)) // no server
    return;
  for(i = 0; i < vector_size(METH(o)); i++)
  {
    m = (struct Method*)vector_at(METH(o), i);
    if(strcmp(m->path, path))
      continue;
    if(!strcmp(m->type, type))
    {
      if((i = vector_find(m->client, (vtype)o)) >= 0)
        vector_remove(m->client, (vtype)i);
      if((i = vector_find(METH(o), (vtype)m)) >= 0)
        vector_remove(METH(o), (vtype)i);
      release_method(SERV(o), m);
      RETURN->d.v_uint = 1;
      return;
    }
  }
}

static MFUN(oscin_get_i)
{
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  vector_remove(c_arg, 0);
  RETURN->d.v_uint = arg->data.i;
  release_Arg(arg);
}

static MFUN(oscin_get_f)
{
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  vector_remove(c_arg, 0);
  RETURN->d.v_float = arg->data.f;
  release_Arg(arg);
}

static MFUN(oscin_get_s)
{
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  vector_remove(c_arg, 0);
  RETURN->d.v_uint = (m_uint)new_String(shred, arg->data.s);
  release_Arg(arg);
}

IMPORT
{
  DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_lo))
  CHECK_BB(import_class_begin(env, &t_lo, env->global_nspc, lo_ctor, NULL))
  o_lo_args = import_mvar(env, "int",  "@args", 0, 0, "Arg vector");
  CHECK_BB(o_lo_args)
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_loout))
  CHECK_BB(import_class_begin(env, &t_loout, env->global_nspc, NULL, loout_dtor))
  o_lo_addr = import_mvar(env, "int",  "@addr", 0, 0, "lo adress");
  CHECK_BB(o_lo_addr)
  fun = new_DL_Func("int", "addr", (m_uint)osc_addr);
    dl_func_add_arg(fun, "string", "host");
    dl_func_add_arg(fun, "string", "port");
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "send", (m_uint)osc_send);
    dl_func_add_arg(fun, "string", "path");
  CHECK_OB(import_mfun(env, fun))
  CHECK_BB(import_class_end(env))

  CHECK_BB(add_global_type(env, &t_loin))
  CHECK_BB(import_class_begin(env, &t_loin, env->global_nspc, loin_ctor, NULL))
  o_lo_serv = import_mvar(env, "int",  "@serv", 0, 0, "Server ref");
  CHECK_BB(o_lo_serv)
  o_lo_meth = import_mvar(env, "int",  "@meth", 0, 0, "Method Vector");
  CHECK_BB(o_lo_meth)
  o_lo_curr = import_mvar(env, "int",  "@curr", 0, 0, "Arg Vector");
  CHECK_BB(o_lo_curr)
  fun = new_DL_Func("int", "add", (m_uint)osc_add_method);
    dl_func_add_arg(fun, "string", "path");
    dl_func_add_arg(fun, "string", "type");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "port", (m_uint)osc_get_port);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "port", (m_uint)osc_port);
    dl_func_add_arg(fun, "int", "port");
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "start", (m_uint)oscin_start);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "stop", (m_uint)oscin_stop);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "recv", (m_uint)osc_recv);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("int", "get_i", (m_uint)oscin_get_i);
  CHECK_OB(import_mfun(env, fun))
  fun = new_DL_Func("float", "get_f", (m_uint)oscin_get_f);
  CHECK_OB(import_mfun(env, fun))  
  fun = new_DL_Func("string", "get_s", (m_uint)oscin_get_s);
  CHECK_OB(import_mfun(env, fun))


  CHECK_BB(import_class_end(env))

  CHECK_BB(add_binary_op(env, op_chuck, &t_int,    &t_loout, &t_int,    oscsend_add_int,    1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_float,  &t_loout, &t_float,  oscsend_add_float,  1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_string, &t_loout, &t_string, oscsend_add_string, 1))

  return 1;
}
