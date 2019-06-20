/*
    C File natshighsend.c
    Written by: Asharudin 'aviezab' Achzab
    First written date: 2019 05 25
    This file is granted without any warranty
*/
#include <stdlib.h>
#include <stdio.h>
#include <nats/nats.h>

// ----- Variables -----
natsConnection      *nconns;
natsSubscription    *sub;
natsMsg             *msg;


// Deklarasi Prototipe
int sendmsgq1(char *, char *);

void
runnit(void){
    nconns  = NULL;
    sub = NULL;
    msg = NULL;
    natsConnection_ConnectTo(&nconns, "nats://localhost:4222");
}


// Function untuk kirim pesan ke MQ1
int
sendmsgq1(char *pesan, char *topic){
    runnit();
    natsStatus stat = NATS_OK;

    printf("Queue Send: %s\n", pesan);
    stat = natsConnection_PublishString(nconns, topic, (const char *) pesan);
    if (stat != NATS_OK)
        {return -1;}
    printf("squeue status: %s | on topic: %s\n", natsStatus_GetText(stat), topic);
    natsConnection_Destroy(nconns);
    nats_Close();
    return 0;
}