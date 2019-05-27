/*
    C File redishigh.c
    Written by: Asharudin 'aviezab' Achzab
    First written date: 2019 05 27
    This file is granted without any warranty
    -lhiredis
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis.h>

// -----VARIABLES-----
redisContext *red;
redisReply *reply;
const char *hostname = "127.0.0.1";
int port = 6379;
struct timeval timeout = { 1, 500000 }; // 1.5 seconds

void
redis_init()
{
    red = redisConnectWithTimeout(hostname, port, timeout);
    if (red == NULL || red->err) {
        if (red) {
            printf("Connection error: %s\n", red->errstr);
            redisFree(red);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }
}

int
ping_redis()
{
    redis_init();
    reply = redisCommand(red,"PING");
    printf("Redis PING: %s\n", reply->str);
    freeReplyObject(reply);
    redisFree(red);
    return 0;
}

int
redis_check(char *key)
{
    redis_init();
    char *buffr=NULL;
    int errb=1000;
    buffr=(char *) malloc(25000);
    sprintf(buffr, "EXISTS %s", key);
    reply = redisCommand(red, buffr);
    errb=reply->integer;
    //printf("Err number: %d\n", errb);
    freeReplyObject(reply);
    free(buffr);
    redisFree(red);
    return errb;
}

int
put_redis(char *key, char *word)
{
    redis_init();
    char *buffr=NULL;
    buffr=(char *) malloc(250000);
    sprintf(buffr, "LPUSH %s \"%s\"", key, word);
    // printf("Push buffr %s\n", buffr);
    reply = redisCommand(red,buffr);
    printf("Redis PUSH status: %lld\n", reply->integer);
    freeReplyObject(reply);
    redisFree(red);
    free(buffr);
    return 0;
}

char
*get_redis(char *key)
{
    redis_init();
    char *buffr=NULL;
    char *balasan=NULL;
    int errb=1000;
    buffr=(char *) malloc(25000);
    balasan=(char *) malloc(25000);
    sprintf(buffr, "RPOP %s", key);
    reply = redisCommand(red, buffr);
    sprintf(balasan, "%s", reply->str);
    //errb=reply->integer;
    //printf("Err number: %d\n", errb);
    // printf("Redis POP %s: %s\n", buffr, balasan);
    // printf("balasan: %s\n", balasan);
    freeReplyObject(reply);
    free(buffr);
    redisFree(red);
    return balasan;
}