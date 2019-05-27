/*
    C File redishigh.c
    Written by: Asharudin 'aviezab' Achzab
    First written date: 2019 05 26
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
put_redis(char *key, char *word)
{
    redis_init();
    char *buffr=NULL;
    buffr=(char *) malloc(200);
    sprintf(buffr, "LPUSH %s %s", key, word);
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
    buffr=(char *) malloc(200);
    sprintf(buffr, "RPOP %s", key);
    reply = redisCommand(red, buffr);
    printf("Redis POP %s: %s\n", buffr, reply->str);
    freeReplyObject(reply);
    free(buffr);
    redisFree(red);
}