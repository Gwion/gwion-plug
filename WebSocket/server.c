#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "plug.h"
#include "driver.h"

#define PORT 1111
#define BUFLEN 512
#define NCHAN 2

static void send_data(struct lws *wsi) {
  VM *vm = lws_context_user(lws_get_context(wsi));
  unsigned char _c[NCHAN * BUFLEN * sizeof(float) + LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING];
  void *c = &_c[LWS_SEND_BUFFER_PRE_PADDING];
  float*data = (void*)&_c[ LWS_SEND_BUFFER_PRE_PADDING];
  for(m_uint i = 0; i < BUFLEN; ++i) {
    vm->bbq->run(vm);
    next_bbq_pos(vm);
    for(int chan = 0; chan < NCHAN; ++chan)
      data[i + BUFLEN*chan] = vm->bbq->out[chan];
  }
  lws_write( wsi, c, sizeof(float) * BUFLEN * NCHAN, LWS_WRITE_BINARY );
}

static int callback_example(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len){
  (void)user;
  (void)in;
  (void)len;
	switch(reason) {
		case LWS_CALLBACK_RECEIVE:
			lws_callback_on_writable_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));
			break;
		case LWS_CALLBACK_SERVER_WRITEABLE:
      send_data(wsi);
			break;
		default:
			break;
	}
	return 0;
}

enum protocols
{
	PROTOCOL_HTTP = 0,
	PROTOCOL_EXAMPLE,
	PROTOCOL_COUNT
};

static struct lws_protocols protocols[] = {
	/* The first protocol must always be the HTTP handler */
	{ .name="http-only", .callback=callback_example, },
	{ .name=NULL } /* terminator */
};

static DRVINI(ws_ini) {
  struct lws_context_creation_info info =
    { .port=PORT, .protocols=protocols, .user=vm };
	struct lws_context *context = lws_create_context( &info );
  CHECK_OB((vm->bbq->driver->data = context));
  return GW_OK;
}

static DRVRUN(ws_run) {
  struct lws_context *context = vm->bbq->driver->data;
	while(vm->bbq->is_running)
		lws_service(context, 0);
}

static DRVDEL(ws_del) {
  struct lws_context *context = vm->bbq->driver->data;
	lws_context_destroy(context);
}

GWDRIVER(ws) {
  d->ini = ws_ini;
  d->run = ws_run;
  d->del = ws_del;
}

#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"

struct WsMsg {
  size_t capa;
  size_t size;
  m_bit  data[];
};

struct Ws {
  struct WsMsg *msg;
  struct Vector_ v;
	struct lws_context *ctx;
  MUTEX_TYPE mutex;
  MemPool mp;
  THREAD_TYPE thread;
  volatile bool run;
};
/*
static INSTR(ws_add_int) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  struct Ws *ws = &*(struct Ws*)o->data;
  struct WsMsg *msg = ws->msg;
  const m_int i = *(m_int*)REG(0);
  const size_t next_size = msg->size + SZ_INT;
  if(msg->size > msg->capa) {
    const size_t next_capa = msg->capa*2;
    msg = mp_realloc(shred->info->mp, msg->data, msg->size, next_capa);
  }
  *(m_int*)(msg->data + msg->size) = i;
  msg->size = next_size;
}

static INSTR(ws_add_float) {
  POP_REG(shred, SZ_FLOAT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  struct Ws *ws = &*(struct Ws*)o->data;
  struct WsMsg *msg = ws->msg;
  const m_float f = *(m_float*)REG(0);
  const size_t next_size = msg->size + SZ_FLOAT;
  if(msg->size > msg->capa) {
    const size_t next_capa = msg->capa*2;
    msg = mp_realloc(shred->info->mp, msg->data, msg->size, next_capa);
  }
  *(m_float*)(msg->data + msg->size) = f;
  msg->size = next_size;
}
*/
static INSTR(ws_add_string) {
  POP_REG(shred, SZ_INT);
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  struct Ws *ws = &*(struct Ws*)o->data;
  struct WsMsg *msg = ws->msg;
  const m_str s = STRING(*(M_Object*)REG(0));
  const size_t sz = strlen(s);
  const size_t next_size = msg->size + sz;
  if(msg->size > msg->capa) {
    const size_t next_capa = msg->capa*2;
    msg = mp_realloc(shred->info->mp, msg->data, msg->size, next_capa);
  }
  memcpy(msg->data, s, sz);
  msg->size = next_size;
}

#define WS_MSG_CAP (8 + sizeof(struct WsMsg))
static struct WsMsg* new_wsmsg(const MemPool mp) {
  struct WsMsg *msg = mp_calloc2(mp, WS_MSG_CAP);
  msg->capa  = WS_MSG_CAP;
  msg->size = 0;
  return msg;
}

static MFUN(ws_send) {
  struct Ws *ws = &*(struct Ws*)o->data;
  MUTEX_LOCK(ws->mutex);
  vector_add(&ws->v, (m_uint)*(struct WsMsg**)o->data);
  MUTEX_UNLOCK(ws->mutex);
  *(struct WsMsg**)o->data = new_wsmsg(shred->info->mp);
}

static int ws_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len){
  (void)in;
  (void)len;
  const M_Object o = lws_context_user(lws_get_context(wsi));
	switch(reason) {
		case LWS_CALLBACK_RECEIVE:
			lws_callback_on_writable_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));
			break;
		case LWS_CALLBACK_SERVER_WRITEABLE:
{
      struct Ws *ws = &*(struct Ws*)o->data;
      MUTEX_LOCK(ws->mutex);
      for(m_uint i = 0; i < vector_size(&ws->v); i++) {
        struct WsMsg *msg = (struct WsMsg*)vector_at(&ws->v, i);
        lws_write( wsi, msg->data, msg->size, LWS_WRITE_BINARY );
        mp_free2(ws->mp, msg->capa, msg);
      }
      vector_clear(&ws->v);
      MUTEX_UNLOCK(ws->mutex);
			lws_callback_on_writable_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));
}
			break;
		default:
			break;
	}
	return 0;
}

static struct lws_protocols ws_protocols[] = {
	{ .name="http-only", .callback=ws_callback, },
	{ .name=NULL } /* terminator */
};

static void* ws_process(void* data) {
  struct Ws *ws = (struct Ws*)data;
  while(ws->run)
    lws_service(ws->ctx, 0);
  return NULL;
}

static MFUN(ws_new) {
  *(M_Object*)RETURN = o;
  struct Ws *ws = &*(struct Ws*)o->data;
  const m_int port = *(m_int*)MEM(SZ_INT);
  ws->msg = new_wsmsg(shred->info->mp);
  vector_init(&ws->v);
  MUTEX_SETUP(ws->mutex);
  struct lws_context_creation_info info =
    { .port=port, .protocols=ws_protocols, .user=o };
	ws->ctx = lws_create_context( &info );
  ws->mp = shred->info->mp;
  ws->run = true;
  THREAD_CREATE(ws->thread, ws_process, ws);
}

static DTOR(ws_dtor) {
  struct Ws *ws = &*(struct Ws*)o->data;
  struct WsMsg *msg = ws->msg;
	lws_context_destroy(ws->ctx);
  for(m_uint i = 0; i < vector_size(&ws->v); i++) {
    struct WsMsg *msg = *(struct WsMsg**)o->data;
    mp_free2(shred->info->mp, msg->capa, msg);
  }
  mp_free2(shred->info->mp, msg->capa, msg->data);
  vector_release((Vector)(o->data + SZ_INT));
  MUTEX_CLEANUP(ws->mutex);
}

GWION_IMPORT(ws) {

  lws_set_log_level(LLL_ERR, NULL);

  DECL_BB(const Type, t_ws, = gwi_class_ini(gwi, "WebSocket", "Object"));
  SET_FLAG(t_ws, abstract);
  gwi_class_xtor(gwi, NULL, ws_dtor);
  t_ws->nspc->offset += sizeof(struct Ws);
  GWI_BB(gwi_func_ini(gwi, "auto", "new"));
  GWI_BB(gwi_func_arg(gwi, "int", "port"));
  GWI_BB(gwi_func_end(gwi, ws_new, ae_flag_none));
  GWI_BB(gwi_func_ini(gwi, "void", "send"));
  GWI_BB(gwi_func_end(gwi, ws_send, ae_flag_none));
  GWI_BB(gwi_class_end(gwi));
/*
  GWI_BB(gwi_oper_ini(gwi, "WebSocket", "int", "WebSocket"));
  GWI_BB(gwi_oper_end(gwi, "<<", ws_add_int));

  GWI_BB(gwi_oper_ini(gwi, "WebSocket", "float", "WebSocket"));
  GWI_BB(gwi_oper_end(gwi, "<<", ws_add_float));
*/
  GWI_BB(gwi_oper_ini(gwi, "WebSocket", "string", "WebSocket"));
  GWI_BB(gwi_oper_end(gwi, "<<", ws_add_string));

}
