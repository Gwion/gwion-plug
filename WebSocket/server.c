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
    ++vm->bbq->pos;
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
  CHECK_OB((vm->bbq->driver->data = context))
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
