/*
    Main C File Kimochi Server
    Written by: Asharudin 'aviezab' Achzab, Anas 'Nicabreon'
    First written date: 2019 05 25
    This file is granted without any warranty
*/
#include <kore/kore.h>
#include <kore/http.h>

#include "natshighsend.h"
#include "natshighrcv.h"
#include "redishigh.h"

// Functions Declaration
void		websocket_connect(struct connection *);
void		websocket_connect_ask(struct connection *);
void		websocket_disconnect(struct connection *);
void		websocket_disconnect_ask(struct connection *);
int			page_ws_connect(struct http_request *);
int			page_ws_connect_ask(struct http_request *);
void		websocket_message(struct connection *, u_int8_t, void *, size_t);
// void websocket_ask_msg(struct connection *c, u_int8_t op, void *data, size_t len);
void		websocket_message_ask(struct connection *, u_int8_t , void *, size_t);

int sendmsgq1(char *pesan, char *topic);
//char *rcvmsgq1(char *topic);
char *get_redis(char *key);
/* Dipanggil kapan pun saat ada koneksi websocket terhubung */

/* Connect End Point start */
void
websocket_connect(struct connection *c){
	kore_log(LOG_NOTICE, "%p: connected", c);
}

void
websocket_disconnect(struct connection *c){
	kore_log(LOG_NOTICE, "%p: disconnecting", c);
}

void
websocket_message(struct connection *c, u_int8_t op, void *data, size_t len){
	int status_send=0;
	kore_log(LOG_NOTICE, "data   : %s ", data);
	kore_log(LOG_NOTICE, "length : %zu ", len);
	kore_log(LOG_NOTICE, "OP     : %d ", op);
	char *pesan = NULL;
	pesan = (char *) malloc(250000);
	sprintf(pesan, data);
	status_send=sendmsgq1(pesan, "topic");
	if(status_send!=0)
		{kore_log(LOG_NOTICE, "Error publishing msg to Queue server");}
	free(pesan);
}

int
page_ws_connect(struct http_request *req){
	/* Lakukan websocket handshake dan berikan callback */
	kore_websocket_handshake(req, "websocket_connect",
	    "websocket_message", "websocket_disconnect");

	return (KORE_RESULT_OK);
}
/* Connect End Point end */

/* Ask End Point start*/
void
websocket_connect_ask(struct connection *c){
	kore_log(LOG_NOTICE, "%p: connected ask", c);
}

void
websocket_disconnect_ask(struct connection *c){
	kore_log(LOG_NOTICE, "%p: disconnecting ask", c);
}

void
websocket_message_ask(struct connection *c, u_int8_t op, void *data, size_t len){
	kore_log(LOG_NOTICE, "data ask   : %s ", data);
	kore_log(LOG_NOTICE, "length ask : %zu ", len);
	kore_log(LOG_NOTICE, "OP ask     : %d ", op);
	char *topic_to_ask = NULL;
	char *pesan_return = NULL;
	topic_to_ask = (char *) malloc(1000);
	pesan_return = (char *) malloc(250000);
	if (data){
		kore_log(LOG_NOTICE, "from /ask got %s", data);
		sprintf(topic_to_ask, data);
		kore_log(LOG_NOTICE, "topic_to_ask now %s\n", topic_to_ask);
		pesan_return=get_redis(topic_to_ask);
		if(!pesan_return) {kore_log(LOG_NOTICE, "Error Getting msg from Redis Server");}
		kore_log(LOG_NOTICE, "Got msg from sender - %s", pesan_return);
		free(topic_to_ask);
		free(pesan_return);
	}
}

int
page_ws_connect_ask(struct http_request *req){
	/* Lakukan websocket handshake dan berikan callback */
	kore_websocket_handshake(req, "websocket_connect_ask",
	    "websocket_message_ask", "websocket_disconnect_ask");

	return (KORE_RESULT_OK);
}
/* Ask End Point end*/