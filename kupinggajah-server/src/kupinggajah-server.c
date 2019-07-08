/*
    Main C File kupinggajah Server
    Written by: Asharudin 'aviezab' Achzab, Anas 'Nicabreon'
    First written date: 2019 05 25
    This file is granted without any warranty
*/

/* Definisi MACRO untuk debugging.
 * apabila iya, maka log dan printf ditampilkan.
 */
#ifdef DEBUG
#define DEBUGGING_ 1
#else
#define DEBUGGING_ 0
#endif
#define KGAJAH_MQ2_ERR "{\"kgajah\":\"MQ2_connection_error\"}"
#define KGAJAH_USERMSG_NULL "{\"kgajah\":\"msgnull\"}"

#include <string.h>
#include <unistd.h>
#include <kore/kore.h>
#include <kore/http.h>

#include "kupinggajah.h"
#include "natshighsend.h"
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
void is_debug_print(void *, void *);
void str_append(char subject[], const char insert[], int pos);
/* Dipanggil kapan pun saat ada koneksi websocket terhubung */

void is_debug_print(void *pesan_debug1, void *pesan_debug2){
	if(DEBUGGING_ == 1) {
		kore_log(LOG_NOTICE, "%s : %s", (char *) pesan_debug1, (char *) pesan_debug2 );
	}
}

/* Connect End Point start */
void
websocket_connect(struct connection *c){
	is_debug_print("Connected", c);
}

void
websocket_disconnect(struct connection *c){
	is_debug_print("Disconnecting", c);
}

void
websocket_message(struct connection *c, u_int8_t op, void *data, size_t len){
	int status_send=0;
	char *propertiws = NULL;
	propertiws = (char *) malloc(50 + len);
	sprintf(propertiws, "\nData : %p | length ask: %zu | OP ask: %d", data, len, op);
	is_debug_print("Pr WS", propertiws);
	char *pesan = NULL;
	pesan = (char *) calloc(sizeof(char), len + 1);
	strcpy(pesan, data);
	is_debug_print("pesan", pesan);
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
	is_debug_print("Connected", c);
}

void
websocket_disconnect_ask(struct connection *c){
	is_debug_print("Disconnecting", c);
}

void
websocket_message_ask(struct connection *c, u_int8_t op, void *data, size_t len){
	char *propertiws = NULL;
	char *topic_to_ask = NULL;
	char *pesan_return = NULL;
	int errorga=0;
	propertiws = (char *) malloc(50 + len);
	sprintf(propertiws, "\nData ask: %p | length ask: %zu | OP ask: %d", data, len, op);
	is_debug_print("Pr WS", propertiws);
	topic_to_ask = (char *) malloc(1000);
	if (data){
		strcpy(topic_to_ask, data);
		is_debug_print("From /ask got", data);
		is_debug_print("topic_to_ask is now", topic_to_ask);
		errorga=redis_check(topic_to_ask);
		while(errorga==1)
		{
			pesan_return=get_redis(topic_to_ask);
			size_t del_at;
			del_at=0;
			memmove(&pesan_return[del_at], &pesan_return[del_at + 1], strlen(pesan_return) - del_at);
			del_at=strlen(pesan_return) - 1;
			memmove(&pesan_return[del_at], &pesan_return[del_at + 1], strlen(pesan_return) - del_at);
			if(pesan_return==NULL) {kore_log(LOG_NOTICE, "Error Getting msg from Redis Server");}
			is_debug_print("Got msg from sender", pesan_return);
			size_t pjg_pesan_return = strlen(pesan_return);
			// Kirim semua pesan ke pengguna tujuan setiap 10 mikrodetik
			kore_websocket_broadcast(c, op, pesan_return, pjg_pesan_return, WEBSOCKET_BROADCAST_GLOBAL);
			usleep(10);
			errorga=redis_check(topic_to_ask);
			if (pesan_return != NULL) {free(pesan_return); pesan_return = NULL;}
		}
		
		errorga=redis_check(topic_to_ask);
		if (errorga==-1)
		{
			is_debug_print("Error","Redis Connection");
			kore_websocket_broadcast(c, op, KGAJAH_MQ2_ERR, strlen(KGAJAH_MQ2_ERR), WEBSOCKET_BROADCAST_GLOBAL);
		}
		if(errorga!=1)
		{	
			is_debug_print(data,"doesn't have any msg");
			kore_websocket_broadcast(c, op, KGAJAH_USERMSG_NULL, strlen(KGAJAH_USERMSG_NULL), WEBSOCKET_BROADCAST_GLOBAL);
			if (pesan_return != NULL) {free(pesan_return); pesan_return = NULL;}
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