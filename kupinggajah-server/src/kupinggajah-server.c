/*
    Main C File kupinggajah Server
    Written by: Asharudin 'aviezab' Achzab, Anas 'Nicabreon'
    First written date: 2019 05 25
    This file is granted without any warranty
*/

#include <kore/kore.h>
#include <kore/http.h>
#include <string.h>

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
void		websocket_message_ask(struct connection *, u_int8_t , void *, size_t);

int sendmsgq1(char *pesan, char *topic);
int redis_check(char *key);
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
	char *propertiws = NULL;
	propertiws = (char *) malloc(50 + len);
	sprintf(propertiws, "\nData : %s | length ask: %zu | OP ask: %d", data, len, op);
	kore_log(LOG_NOTICE, "%s" ,propertiws);
	char *pesan = NULL;
	pesan = (char *) malloc(len + 1);
	sprintf(pesan, data);
	status_send=sendmsgq1(pesan, "topic");
	if(status_send!=0)
		{kore_log(LOG_NOTICE, "Error publishing msg to Queue server");}
	free(pesan);
	free(propertiws);
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
	char *propertiws = NULL;
	char *topic_to_ask = NULL;
	char *pesan_return = NULL;
	int errorga=0;
	propertiws = (char *) malloc(50 + len);
	sprintf(propertiws, "\nData ask: %s | length ask: %zu | OP ask: %d", data, len, op);
	kore_log(LOG_NOTICE, "%s" ,propertiws);
	topic_to_ask = (char *) malloc(1000);
	if (data){
		strcpy(topic_to_ask, data);
		kore_log(LOG_NOTICE, "from /ask got %s", data);
		kore_log(LOG_NOTICE, "topic_to_ask now %s\n", topic_to_ask);
		errorga=redis_check(topic_to_ask);
		while(errorga==1)
		{
			pesan_return=get_redis(topic_to_ask);
			if(pesan_return==NULL) {kore_log(LOG_NOTICE, "Error Getting msg from Redis Server");}
			kore_log(LOG_NOTICE, "Got msg from sender - %s", pesan_return);
			size_t pjg_pesan_return = strlen(pesan_return);
			kore_websocket_broadcast(c, op, pesan_return, pjg_pesan_return, WEBSOCKET_BROADCAST_GLOBAL);
			errorga=redis_check(topic_to_ask);
			if (pesan_return != NULL) free(pesan_return); pesan_return = NULL;
		}
		
		errorga=redis_check(topic_to_ask);
		if (errorga==-1)
		{
			kore_log(LOG_NOTICE, "Redis connection error!\n");
		}
		if(errorga!=1)
		{	
			kore_log(LOG_NOTICE, "You don't have any msg");
			if (pesan_return != NULL) free(pesan_return); pesan_return = NULL;
		}
	}
	free(topic_to_ask);
	free(propertiws);
}

int
page_ws_connect_ask(struct http_request *req){
	/* Lakukan websocket handshake dan berikan callback */
	kore_websocket_handshake(req, "websocket_connect_ask",
	    "websocket_message_ask", "websocket_disconnect_ask");

	return (KORE_RESULT_OK);
}
/* Ask End Point end*/