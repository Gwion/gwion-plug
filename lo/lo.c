#include <stdlib.h>
#include <string.h>
#include <lo/lo.h>
#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "instr.h"
#include "import.h"

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
    vector_rem(s->method, vector_find(s->method, (vtype)m));
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
      free_map(map);
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

static m_int o_lo_addr, o_lo_args, o_lo_serv, o_lo_meth, o_lo_curr;
#define ADDR(o) *(lo_address*)    (o->data + o_lo_addr)
#define ARGS(o) *(Vector*)        (o->data + o_lo_args)
#define SERV(o) *(struct Server**)(o->data + o_lo_serv)
#define METH(o) *(Vector*)        (o->data + o_lo_meth)
#define CURR(o) *(Vector*)     (o->data + o_lo_curr)

static MFUN(osc_addr)
{
  if(ADDR(o))
    lo_address_free(ADDR(o));
  m_str host = STRING(*(M_Object*)MEM(SZ_INT)); 
  m_str port = STRING(*(M_Object*)MEM(SZ_INT * 2));
  *(m_uint*)RETURN = (ADDR(o) = lo_address_new(host, port)) ? 1 : 0;
}

static MFUN(osc_send)
{
  m_uint i;
  struct Arg* arg;
  lo_message msg;
  if(!ADDR(o))
  {
    err_msg(INSTR_, 0, "oscsend address not set. shred[%i] exiting.", shred->xid);
    vm_shred_exit(shred);
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
          vm_shred_exit(shred);
          return;
    }
    release_Arg(arg);
  }
  vector_clear(ARGS(o));
  *(m_uint*)RETURN = lo_send_message(ADDR(o), STRING(*(M_Object*)MEM(SZ_INT)), msg) ? 1 : 0;
}

static INSTR(oscsend_add_int) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  vector_add(ARGS((**(M_Object**)REG(0))), (vtype)new_Arg('i', REG(0)));
  release(**(M_Object**)REG(0), shred);
}

static INSTR(oscsend_add_float) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  vector_add(ARGS((**(M_Object**)REG(0))), (vtype)new_Arg('d', REG(0)));
  release(**(M_Object**)REG(0), shred);
}

static INSTR(oscsend_add_string) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  vector_add(ARGS((**(M_Object**)REG(0))), (vtype)new_Arg('s', REG(0)));
  release(**(M_Object**)REG(0), shred);
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

  v = new_vector();
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
    vector_add(v, (vtype)arg);
  }
  for(i = 0; i < vector_size(m->client); i++)
  {
    for(j = 0; j < argc; j++)
      ((struct Arg*)vector_at(v, j))->ref++;
    o = (M_Object)vector_at(m->client, i);
    broadcast(o);
    vector_add(ARGS(o), (vtype)vector_copy(v));
  }
  for(i = 0; i < argc; i++)
    release_Arg((struct Arg*)vector_at(v, i));
  free_vector(v);
  return 0;
}

static MFUN(osc_get_port)
{
  struct Server* s = SERV(o);
  *(m_uint*)RETURN = s ? s->port : -1;
}

static MFUN(osc_port)
{
  struct Server* s = NULL;
  /*if(!o)*/
  /*{*/
  /*Except(shred)*/
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
    s = calloc(1, sizeof(struct Server));
    s->thread = lo_server_thread_new(c, osc_error_handler);
    if(!s->thread)
    {
      free(s);
      vm_shred_exit(shred);
      return;
    }
    lo_server_enable_coercion(lo_server_thread_get_server(s->thread), 0);
    s->method = new_vector();
    s->ref = 0;
    if(!map)
      map = new_map();
    map_set(map, (vtype)port, (vtype)s);
  }
  s->ref++;
  SERV(o) = s;
  *(m_uint*)RETURN = s->port = port;
}

static MFUN(osc_add_method)
{
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
  m = (struct Method*)calloc(1, sizeof(struct Method));
  m->path = path;
  m->type = type;
  m->client = new_vector();
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

static CTOR(lo_ctor){ ARGS(o) = new_vector(); }
static CTOR(loin_ctor){ METH(o) = new_vector(); CURR(o) = new_vector();}
static CTOR(lo_dtor) { free_vector(ARGS(o)); }
static CTOR(loout_dtor){ if(ADDR(o))lo_address_free(ADDR(o));}
static CTOR(loin_dtor){ free_vector(METH(o)); free_vector(CURR(o));}

static MFUN(oscin_stop)
{
  struct Server* s = SERV(o);
  *(m_uint*)RETURN = lo_server_thread_stop(s->thread);
}

static MFUN(oscin_start)
{
  struct Server* s = SERV(o);
  if(lo_server_thread_start(s->thread) < 0)
  {
    release_server(s);
    *(m_uint*)RETURN = -1;
    return;
  }
  *(m_uint*)RETURN = 1;
}

static MFUN(osc_recv)
{
  Vector c_arg = ARGS(o);
  CURR(o) = (Vector)vector_at(c_arg, 0);
  *(m_uint*)RETURN = CURR(o) ? 1 : 0;
  vector_rem(ARGS(o), 0);
}
static MFUN(oscin_rem)
{
  m_uint i;
  struct Method* m = NULL;
  POP_REG(shred, SZ_INT*2);
  m_str path = STRING(*(M_Object*)MEM(0));
  m_str type = STRING(*(M_Object*)MEM(SZ_INT));
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
        vector_rem(m->client, (vtype)i);
      if((i = vector_find(METH(o), (vtype)m)) >= 0)
        vector_rem(METH(o), (vtype)i);
      release_method(SERV(o), m);
      *(m_uint*)RETURN = 1;
      return;
    }
  }
}

static MFUN(oscin_get_i)
{
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  vector_rem(c_arg, 0);
  *(m_uint*)RETURN = arg->data.i;
  release_Arg(arg);
}

static MFUN(oscin_get_f)
{
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  vector_rem(c_arg, 0);
  *(m_float*)RETURN = arg->data.f;
  release_Arg(arg);
}

static MFUN(oscin_get_s)
{
  Vector c_arg = CURR(o);
  struct Arg* arg = (struct Arg*)vector_at(c_arg, 0);
  vector_rem(c_arg, 0);
  *(m_uint*)RETURN = (m_uint)new_String(shred, arg->data.s);
  release_Arg(arg);
}

IMPORT {
  Type t_lo, t_loin, t_loout;
  CHECK_OB((t_lo    = gwi_mk_type(gwi, "Lo",     SZ_INT, t_event)))
  CHECK_OB((t_loin  = gwi_mk_type(gwi, "OscIn",  SZ_INT, t_lo)))
  CHECK_OB((t_loout = gwi_mk_type(gwi, "OscOut", SZ_INT, t_lo)))
  SET_FLAG(t_lo, ae_flag_abstract);

  CHECK_BB(gwi_class_ini(gwi, t_lo, lo_ctor, NULL))
	gwi_item_ini(gwi,"int",  "@args");
  o_lo_args = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_lo_args)
  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_class_ini(gwi, t_loout, NULL, loout_dtor))
	gwi_item_ini(gwi,"int",  "@addr");
  o_lo_addr = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_lo_addr)
  gwi_func_ini(gwi, "int", "addr", osc_addr);
    gwi_func_arg(gwi, "string", "host");
    gwi_func_arg(gwi, "string", "port");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "send", osc_send);
    gwi_func_arg(gwi, "string", "path");
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_class_ini(gwi, t_loin, loin_ctor, NULL))
	gwi_item_ini(gwi,"int",  "@serv");
  o_lo_serv = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_lo_serv)
	gwi_item_ini(gwi,"int",  "@meth");
  o_lo_meth = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_lo_meth)
	gwi_item_ini(gwi,"int",  "@curr");
  o_lo_curr = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_lo_curr)
  gwi_func_ini(gwi, "int", "add", osc_add_method);
    gwi_func_arg(gwi, "string", "path");
    gwi_func_arg(gwi, "string", "type");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "port", osc_get_port);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "port", osc_port);
    gwi_func_arg(gwi, "int", "port");
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "start", oscin_start);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "stop", oscin_stop);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "recv", osc_recv);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "int", "get_i", oscin_get_i);
  CHECK_BB(gwi_func_end(gwi, 0))
  gwi_func_ini(gwi, "float", "get_f", oscin_get_f);
  CHECK_BB(gwi_func_end(gwi, 0))  
  gwi_func_ini(gwi, "string", "get_s", oscin_get_s);
  CHECK_BB(gwi_func_end(gwi, 0))


  CHECK_BB(gwi_class_end(gwi))

  CHECK_BB(gwi_oper_ini(gwi, "int", "OscOut", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, oscsend_add_int))
  CHECK_BB(gwi_oper_ini(gwi,"float",  "OscOut", "float"))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, oscsend_add_float))
  CHECK_BB(gwi_oper_ini(gwi, "string", "OscOut", "string"))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, oscsend_add_string))

  return 1;
}
