#include <stdlib.h>
#include <stdio.h>
#include <nats/nats.h>


// -----Variables-----
natsConnection *conn = NULL;
natsOptions *opts = NULL; 
natsSubscription *sub = NULL;
char *buffmsg=NULL;

static void
onMsg(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure){
  sprintf(buffmsg, "%.*s", natsMsg_GetDataLength(msg), natsMsg_GetData(msg));
  printf("rqueue Dest/ Received msg: %s - %.*s\n -", natsMsg_GetSubject(msg), 
	                              natsMsg_GetDataLength(msg),
                                      natsMsg_GetData(msg));
  sleep(2);
  natsMsg_Destroy(msg);
}

char* rcvmsgq1(char *topic){
  buffmsg=NULL;
  natsStatus stat = NATS_OK;
  
  stat = natsOptions_Create(&opts);
  if (stat == NATS_OK) stat = natsOptions_SetURL(opts, "nats://localhost:4222");
  if (stat == NATS_OK) stat = natsConnection_Connect(&conn, opts);
  if (stat == NATS_OK) 
    {   stat = natsConnection_Subscribe(&sub, conn, topic, onMsg, NULL); }
  if (stat == NATS_OK) stat = natsSubscription_SetPendingLimits(sub, -1, -1);

  printf("rqueue status: %s\n", natsStatus_GetText(stat));
  natsSubscription_Destroy(sub);
  natsConnection_Destroy(conn);
  natsOptions_Destroy(opts);
  nats_Close();
  return buffmsg;
}