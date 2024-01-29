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
#define BUFLEN 1024
#define NCHAN 2

ANN static void send_data(struct lws *wsi) {
  VM *vm = lws_context_user(lws_get_context(wsi));
  static unsigned char _c[NCHAN * BUFLEN * sizeof(float) + LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING];
  void *c = &_c[LWS_SEND_BUFFER_PRE_PADDING];
  float*data = (float*)&_c[ LWS_SEND_BUFFER_PRE_PADDING];
  for(m_uint i = 0; i < BUFLEN; i++) {
    vm->bbq->run(vm);
    next_bbq_pos(vm);
    for(int chan = 0; chan < NCHAN; chan++)
      data[i + chan*BUFLEN] = vm->bbq->out[chan];
  }
  lws_write(wsi, c, sizeof(float) * BUFLEN * NCHAN, LWS_WRITE_BINARY );
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

GWDRIVER(WebSocket) {
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
  struct Vector_ recv;
  gwtlock_t mutex;
  MemPool mp;
  THREAD_TYPE thread;
  M_Object self;
  int port;
  volatile bool run;
} Ws;

static MFUN(ws_send_string) {
  Ws *const ws = &*(Ws*)(o->data + SZ_INT);
  const m_str s = STRING(*(M_Object*)MEM(SZ_INT));
  const size_t len = strlen(s) + 1;
  const size_t sz = sizeof(struct WsMsg) + len;
  struct WsMsg *msg = mp_malloc2(shred->info->mp, sz);
  msg->size = len;
  msg->capa = sz;
  memcpy(msg->data, s, len);
  gwt_lock(&ws->mutex);
  vector_add(&ws->v, (m_uint)msg);
  gwt_unlock(&ws->mutex);
}

ANN static void wscb_read(struct lws *wsi, char *in, size_t len) {
  M_Object o = lws_context_user(lws_get_context(wsi)); 
  Ws *const ws = &*(Ws*)(o->data + SZ_INT);
  m_str str = mp_malloc2(ws->mp, len + 1);
  strncat(str, (const char*)in, len);
  str[len] = '\0';
  gwt_lock(&ws->mutex);
  vector_add(&ws->recv, (m_uint)str);
  gwt_unlock(&ws->mutex);
  broadcast(o);
}

ANN static void wscb_write(struct lws *wsi) {
  M_Object o = lws_context_user(lws_get_context(wsi)); 
  Ws *const ws = &*(Ws*)(o->data + SZ_INT);
  gwt_lock(&ws->mutex);
  for(m_uint i = 0; i < vector_size(&ws->v); i++) {
    struct WsMsg *msg = (struct WsMsg*)vector_at(&ws->v, i);
    lws_write(wsi, msg->data, msg->size, LWS_WRITE_BINARY);
    mp_free2(ws->mp, msg->capa, msg);
  }
  vector_clear(&ws->v);
  gwt_unlock(&ws->mutex);
}

static int ws_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user NUSED, void *in, size_t len){
	switch(reason) {
		case LWS_CALLBACK_RECEIVE:
      wscb_read(wsi, (char *)in, len);
			lws_callback_on_writable_all_protocol(lws_get_context(wsi), lws_get_protocol(wsi));
			break;
		case LWS_CALLBACK_SERVER_WRITEABLE:
      wscb_write(wsi);
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
  M_Object o = (M_Object)data;
  Ws *const ws = &*(Ws*)(o->data + SZ_INT);
  struct lws_context_creation_info info =
    { .port=ws->port, .protocols=ws_protocols, .user=o };
  struct lws_context *ctx = lws_create_context( &info );
  while(ws->run) lws_service(ctx, 0);
  lws_context_destroy(ctx);
  return NULL;
}

static MFUN(ws_new) {
  *(M_Object*)RETURN = o;
  Ws *const ws = &*(Ws*)(o->data + SZ_INT);
  vector_init(&ws->v);
  vector_init(&ws->recv);
  ws->self = o;
  ws->port = *(m_int*)MEM(SZ_INT);
  gwt_lock_ini(&ws->mutex);
  ws->mp = shred->info->mp;
  ws->run = true;
  THREAD_CREATE(ws->thread, ws_process, o);
}

static DTOR(ws_dtor) {
  Ws *const ws = &*(Ws*)(o->data + SZ_INT);
  ws->run = false;
	//lws_cancel_service(ws->ctx);
  THREAD_JOIN(ws->thread);
  for(m_uint i = 0; i < vector_size(&ws->v); i++) {
    struct WsMsg *msg = (struct WsMsg*)vector_at(&ws->v, i);
    mp_free2(ws->mp, msg->capa, msg);
  }
  vector_release(&ws->v);
  for(m_uint i = 0; i < vector_size(&ws->recv); i++) {
    m_str str = (m_str)vector_at(&ws->recv, i);
    free_mstr(ws->mp, str);
  }
  vector_release(&ws->recv);
  gwt_lock_end(&ws->mutex);
	//lws_context_destroy(ws->ctx);
}

static MFUN(ws_recv) {
  Ws *const ws = &*(Ws*)(o->data + SZ_INT);
  *(m_uint*)RETURN = !!vector_size(&ws->recv);
}

static MFUN(ws_read) {
  Ws *const ws = &*(Ws*)(o->data + SZ_INT);
  if(vector_size(&ws->recv)) {
    const m_str str = (m_str)vector_front(&ws->recv);
    M_Object ret = new_object(shred->info->mp, shred->info->vm->gwion->type[et_string]);
    STRING(ret) = str;
    *(M_Object*)RETURN = ret;
    vector_rem(&ws->recv, 0);
  } else xfun_handle(shred, "WebSocketNoMessage");
}

static SFUN(ws_new_client) {
  m_str inputURL = STRING(*(M_Object*)MEM(0));
  struct lws_client_connect_info clientConnectInfo;
  const char *urlProtocol, *urlTempPath;
  char urlPath[300]; // The final path string
  if (lws_parse_uri(inputURL, &urlProtocol, &clientConnectInfo.address, &clientConnectInfo.port, &urlTempPath))
	{
		xfun_handle(shred, "WebSocketInvalidURL");
    return;
	}
  urlPath[0] = '/';
	strncpy(urlPath + 1, urlTempPath, sizeof(urlPath) - 2);
	urlPath[sizeof(urlPath) - 1] = '\0';
  clientConnectInfo.path = urlPath;
struct lws_context_creation_info ctxCreationInfo; 
  
	// Set up the context creation info
	//ctxCreationInfo.port = CONTEXT_PORT_NO_LISTEN; // We don't want this client to listen
	ctxCreationInfo.protocols = protocols; // Use our protocol list
	ctxCreationInfo.gid = -1; // Set the gid and uid to -1, isn't used much
	ctxCreationInfo.uid = -1;
	ctxCreationInfo.extensions = extensions; // Use our extensions list

	// Create the context with the info
	ctx = lws_create_context(&ctxCreationInfo);
	if (ctx == NULL)
	{
		printf("Error creating context\n");
		return 1;
	}

	// Set up the client creation info
	clientConnectInfo.context = ctx; // Use our created context
	clientConnectInfo.ssl_connection = 0; // Don't use SSL for this test
	clientConnectInfo.host = clientConnectInfo.address; // Set the connections host to the address
	clientConnectInfo.origin = clientConnectInfo.address; // Set the conntections origin to the address
	clientConnectInfo.ietf_version_or_minus_one = -1; // IETF version is -1 (the latest one)
	clientConnectInfo.protocol = protocols[PROTOCOL_TEST].name; // We use our test protocol
	clientConnectInfo.pwsi = &wsiTest; // The created client should be fed inside the wsi_test variable

	printf("Connecting to %s://%s:%d%s \n\n", urlProtocol, clientConnectInfo.address, clientConnectInfo.port, urlPath);

	// Connect with the client info
	lws_client_connect_via_info(&clientConnectInfo);
	if (wsiTest == NULL)
	{
		printf("Error creating the client\n");
		return 1;
	}
}

GWION_IMPORT(WebSocket) {

  lws_set_log_level(LLL_ERR, NULL);

  gwidoc(gwi, "A class To Handle Websockets");
  DECL_OB(const Type, t_ws, = gwi_class_ini(gwi, "WebSocket", "Event"));
  SET_FLAG(t_ws, abstract);
  gwi_class_xtor(gwi, NULL, ws_dtor);
  t_ws->nspc->offset += sizeof(Ws);

  gwidoc(gwi, "Websocket constructor");
  GWI_BB(gwi_func_ini(gwi, "auto", "new"));
  GWI_BB(gwi_func_arg(gwi, "int", "port"));
  GWI_BB(gwi_func_end(gwi, ws_new, ae_flag_none));

  gwidoc(gwi, "client constructor");
  GWI_BB(gwi_func_ini(gwi, "WebSocket", "client"));
  GWI_BB(gwi_func_arg(gwi, "string", "uri"));
  GWI_BB(gwi_func_end(gwi, ws_new_client, ae_flag_static));

  gwidoc(gwi, "send function");
  GWI_BB(gwi_func_ini(gwi, "void", "send"));
  GWI_BB(gwi_func_arg(gwi, "string", "data"));
  GWI_BB(gwi_func_end(gwi, ws_send_string, ae_flag_none));

  gwidoc(gwi, "returns true if there are messages to read");
  GWI_BB(gwi_func_ini(gwi, "string", "recv"));
  GWI_BB(gwi_func_end(gwi, ws_recv, ae_flag_none));

  gwidoc(gwi, "recieve function");
  GWI_BB(gwi_func_ini(gwi, "string", "read"));
  GWI_BB(gwi_func_end(gwi, ws_read, ae_flag_none));
  GWI_BB(gwi_class_end(gwi));

  return GW_OK;
}
