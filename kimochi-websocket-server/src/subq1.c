/*
    WORKER FILE! SHOULD COMPILED SEPARATELY
    C File subq1.c
    Written by: Asharudin 'aviezab' Achzab
    First written date: 2019 05 25
    This file is granted without any warranty
    gcc subq1.c natshighsend.c  -L/usr/local/lib -L. -Wl,--as-needed -ldl -lnats -lpthread -lprotobuf -ljson-c -lhiredis -o sub
    gcc subq1.c natshighsend.c redishigh.c  -L/usr/local/lib -L. -I/usr/local/include -I/usr/local/include/hiredis  -Wl,--as-needed -ldl -lnats -lpthread -lprotobuf -ljson-c -lhiredis -o worker1
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <json-c/json.h>
#include <nats/nats.h>
#include <string.h>
#include <setjmp.h>

#include "natshighsend.h"
#include "redishigh.h"

int sendmsgq1(char *pesan, char *topic);
int put_redis(char *key, char *word);

static void
onMsg(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure){
    struct  json_object *parsed_json;
    struct  json_object *jdest;
    char *buffmsg=NULL;
    char *dest_name;
    int err=0;
    buffmsg = (char *) malloc(250000); // agar tidak segment fault
    dest_name = (char *) malloc(1000);
    // isi buffmsg dengan data string dari natsMsg. gunakan .*%s untuk data yg memiliki length
    //sprintf(buffmsg, "%.*s", natsMsg_GetDataLength(msg), natsMsg_GetData(msg)); //sama saja dengan strncpy
    strncpy(buffmsg, natsMsg_GetData(msg), natsMsg_GetDataLength(msg));
    parsed_json = json_tokener_parse(buffmsg);

    // isi jdest dengan data dari key 'dest', lalu pindahkan ke char biasa supaya tidak remain struct
    err=json_object_object_get_ex(parsed_json, "dest", &jdest);
    // Kalau error dihandle oleh exception
    if (err==0) {goto exc;}
    sprintf(dest_name, "%s", json_object_get_string(jdest));
    // print semua data
    printf("Dest: %s; From Topic: %s Msg: \n %s\n", dest_name, natsMsg_GetSubject(msg), buffmsg);

    put_redis(dest_name, buffmsg);
    free(buffmsg);
    free(dest_name);
    usleep(100);
    natsMsg_Destroy(msg);
    return;
  exc:
    printf("Exception: Data received doesn't have key 'dest'\n");
}

int
main(int argc, char **argv){  
  natsConnection *conn = NULL;
  natsOptions *opts = NULL; 
  natsSubscription *sub = NULL;
  natsStatus stat = NATS_OK;

  const char * subj = "topic";
  
  stat = natsOptions_Create(&opts);
  if (stat == NATS_OK) stat = natsOptions_SetURL(opts, "nats://localhost:4222");
  if (stat == NATS_OK) stat = natsConnection_Connect(&conn, opts);
  if (stat == NATS_OK) stat = natsConnection_Subscribe(&sub, conn, subj, onMsg, NULL);
  if (stat == NATS_OK) stat = natsSubscription_SetPendingLimits(sub, -1, -1);
  printf("Worker Queue Connection status: %s\n", natsStatus_GetText(stat));

  usleep(100);
  while (1);

  natsSubscription_Destroy(sub);
  natsConnection_Destroy(conn);
  natsOptions_Destroy(opts);
  nats_Close();
  return 0;
}