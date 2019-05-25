
#include <kore/kore.h>
#include <kore/http.h>

#include <nats/nats.h>
#include "assets.h"

int		page(struct http_request *);
int		page_ws_connect(struct http_request *);

void		websocket_connect(struct connection *); 
void		websocket_disconnect(struct connection *);
void		websocket_message(struct connection *,
		    u_int8_t, void *, size_t);
// void		websocket_connection_list(struct connection_list *);
natsConnection      *nconns;
natsSubscription    *sub;
natsMsg             *msg;

void
runnit(){
    nconns  = NULL;
    sub = NULL;
    msg = NULL;
    natsConnection_ConnectTo(&nconns, "nats://localhost:4222");
}

int sendmsgq1(char *pesan, char *topic){
runnit();
int count=0;
natsStatus stat = NATS_OK;
pesan = (char *) malloc(250000);

for(count=0; count<10; count++){
	      sprintf(pesan, "hello %d", count);
	      printf("Send: %s\n", pesan);
	      stat = natsConnection_PublishString(nconns, topic, (const char *) pesan);
	      // Error handling
          if (stat != NATS_OK) {
            free(pesan);
            break;
          }
	   }

    printf("status: %s\n", natsStatus_GetText(stat));
    natsConnection_Destroy(nconns);
    nats_Close();
    return 0;
}
/* Called whenever we get a new websocket connection. */
void
websocket_connect(struct connection *c)
{
	kore_log(LOG_NOTICE, "%p: connected", c);
	// kore_log(LOG_NOTICE, "Connection List : %p", cl)
}

/* void
websocket_connection_list(struct connection_list *cl)
{
	kore_log(LOG_NOTICE, "connection list : %p", cl);
 } */

void
websocket_message(struct connection *c, u_int8_t op, void *data, size_t len)
{
	// printf("%p", data);
	kore_log(LOG_NOTICE, "data : %s ", data);
	kore_log(LOG_NOTICE, "length : %zu ", len);
	kore_log(LOG_NOTICE, "OP : %d ", op);
	kore_websocket_broadcast(c, op, data, len, WEBSOCKET_BROADCAST_GLOBAL);
	char *pesan = NULL;
	pesan = (char *) malloc(250000);
	sprintf(pesan, "Coba pesan 1");
	sendmsgq1(pesan, "topic");
	free(pesan);
}

void
websocket_disconnect(struct connection *c)
{
	kore_log(LOG_NOTICE, "%p: disconnecting", c);
}

int
page(struct http_request *req)
{
	http_response_header(req, "content-type", "text/html");
	http_response(req, 200, asset_frontend_html, asset_len_frontend_html);

	return (KORE_RESULT_OK);
}

int
page_ws_connect(struct http_request *req)
{
	/* Perform the websocket handshake, passing our callbacks. */
	kore_websocket_handshake(req, "websocket_connect",
	    "websocket_message", "websocket_disconnect");

	return (KORE_RESULT_OK);
}

