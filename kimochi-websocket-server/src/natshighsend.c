/*
    C File natshighsend.c
    Written by: Asharudin 'aviezab' Achzab
    First written date: 2019 05 25
    This file is granted without any warranty
*/
#include <nats/nats.h>
#include <stdlib.h>


// ----- Variables -----
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


// Function untuk kirim pesan
int
sendmsgq1(char *pesan, char *topic){
    runnit();
    natsStatus stat = NATS_OK;

    printf("Send: %s\n", pesan);
    stat = natsConnection_PublishString(nconns, topic, (const char *) pesan);

    printf("status: %s\n", natsStatus_GetText(stat));
    natsConnection_Destroy(nconns);
    nats_Close();
    return 0;
}