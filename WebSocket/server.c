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
#include "gwi.h"

struct WsMsg {
  size_t capa;
  size_t size;
  m_bit  data[];
};

typedef struct Ws_ {
  struct Vector_ v;
	struct lws_context *ctx;
  MUTEX_TYPE mutex;
  MemPool mp;
  THREAD_TYPE thread;
  volatile bool run;
} Ws;

static MFUN(ws_send_string) {
  Ws *const ws = &*(Ws*)o->data;
  const m_str s = STRING(*(M_Object*)MEM(SZ_INT));
  const size_t len = strlen(s) + 1;
  const size_t sz = sizeof(struct WsMsg) + len;
  struct WsMsg *msg = mp_malloc2(shred->info->mp, sz);
  msg->size = len;
  msg->capa = sz;
  memcpy(msg->data, s, len);
  MUTEX_LOCK(ws->mutex);
  vector_add(&ws->v, (m_uint)msg);
  MUTEX_UNLOCK(ws->mutex);
}

static int ws_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user NUSED, void *in, size_t len){
  (void)in;
  (void)len;
  Ws *const ws = lws_context_user(lws_get_context(wsi));
	switch(reason) {
		case LWS_CALLBACK_RECEIVE:
			lws_callback_on_writable_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));
			break;
		case LWS_CALLBACK_SERVER_WRITEABLE:
      MUTEX_LOCK(ws->mutex);
      for(m_uint i = 0; i < vector_size(&ws->v); i++) {
        struct WsMsg *msg = (struct WsMsg*)vector_at(&ws->v, i);
        lws_write(wsi, msg->data, msg->size, LWS_WRITE_BINARY);
        mp_free2(ws->mp, msg->capa, msg);
      }
      vector_clear(&ws->v);
      MUTEX_UNLOCK(ws->mutex);
			lws_callback_on_writable_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));
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
  Ws *const ws = (Ws*)data;
  while(ws->run)
    lws_service(ws->ctx, 0);
  return NULL;
}

static MFUN(ws_new) {
  *(M_Object*)RETURN = o;
  Ws *const ws = &*(Ws*)o->data;
  const m_int port = *(m_int*)MEM(SZ_INT);
  vector_init(&ws->v);
  MUTEX_SETUP(ws->mutex);
  struct lws_context_creation_info info =
    { .port=port, .protocols=ws_protocols, .user=ws };
	ws->ctx = lws_create_context( &info );
  ws->mp = shred->info->mp;
  ws->run = true;
  THREAD_CREATE(ws->thread, ws_process, ws);
}

static DTOR(ws_dtor) {
  Ws *const ws = &*(Ws*)o->data;
  ws->run = false;
	lws_cancel_service(ws->ctx);
  THREAD_JOIN(ws->thread);
  for(m_uint i = 0; i < vector_size(&ws->v); i++) {
    struct WsMsg *msg = (struct WsMsg*)vector_at(&ws->v, i);
    mp_free2(ws->mp, msg->capa, msg);
  }
  vector_release(&ws->v);
  MUTEX_CLEANUP(ws->mutex);
	lws_context_destroy(ws->ctx);
}

GWION_IMPORT(ws) {

  lws_set_log_level(LLL_ERR, NULL);

  gwidoc(gwi, "A class To Handle Websockets");
  DECL_OB(const Type, t_ws, = gwi_class_ini(gwi, "WebSocket", "Object"));
  SET_FLAG(t_ws, abstract);
  gwi_class_xtor(gwi, NULL, ws_dtor);
  t_ws->nspc->offset += sizeof(Ws);

  gwidoc(gwi, "Websocket construtor");
  GWI_BB(gwi_func_ini(gwi, "auto", "new"));
  GWI_BB(gwi_func_arg(gwi, "int", "port"));
  GWI_BB(gwi_func_end(gwi, ws_new, ae_flag_none));

  gwidoc(gwi, "send function");
  GWI_BB(gwi_func_ini(gwi, "void", "send"));
  GWI_BB(gwi_func_arg(gwi, "string", "data"));
  GWI_BB(gwi_func_end(gwi, ws_send_string, ae_flag_none));
  GWI_BB(gwi_class_end(gwi));

  return GW_OK;
}
