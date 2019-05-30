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
char *buffrp=NULL;
redisContext *red;
redisReply *reply;
const char *hostname = "127.0.0.1";
int port = 6379;
struct timeval timeout = { 1, 500000 }; // 1.5 seconds

int
redis_init()
{
    red = redisConnectWithTimeout(hostname, port, timeout);
    if (red == NULL || red->err) {
        if (red) {
            printf("Connection error: %s\n", red->errstr);
            redisFree(red);
            return -1;
        } else {
            printf("Connection error: can't allocate redis context\n");
            return -1;
        }
        //exit(1);
    }
    return 0;
}

int
ping_redis()
{
    if (redis_init()!=0)
        {
            return -1;
        }
    reply = redisCommand(red,"PING");
    printf("Redis PING: %s\n", reply->str);
    freeReplyObject(reply);
    redisFree(red);
    return 0;
}

int
redis_check(char *key)
{
    if (redis_init()!=0)
    {
        return -1;
    }
    char *buffr=NULL;
    int errb=0;
    buffr=(char *) malloc(25000);
    sprintf(buffr, "EXISTS %s", key);
    reply = redisCommand(red, buffr);
    errb=reply->integer;
    // printf("Err number: %d\n", errb);
    freeReplyObject(reply);
    free(buffr);
    redisFree(red);
    return errb;
}

int
put_redis(char *key, char *word)
{
    if (redis_init()!=0)
    {
        return -1;
    }
    unsigned long pjg_word;
    pjg_word= (unsigned long) strlen(word);
    // Sesuaikan besar memory dengan pesan yang ingin disimpan.
    // printf("size pesan: %lu\n", strlen(word));
    // buffrp=(char *) malloc(300000);
    buffrp = (char *) realloc(buffrp, pjg_word * sizeof(char) + 1 );
    //buffrp = (char *) realloc(buffrp, pjg_word);
    sprintf(buffrp, "LPUSH %s \"%s\"", key, word);
    // printf("Push buffr %s\n", buffr);
    reply = redisCommand(red,buffrp);
    printf("Redis PUSH status: %lld\n", reply->integer);
    freeReplyObject(reply);
    redisFree(red);
    free(buffrp);
    return 0;
}

/*
char
*putn_redis(char *key, char *word, int wordlong)
{
    if (redis_init()!=0)
    {
        return "";
    }
    char *buffrnp=NULL;
    buffrnp = (char *) malloc(wordlong * sizeof(char) +1);
    if (buffrnp == NULL) printf("Gagal malloc");
    sprintf(buffrnp, "LPUSH %s \"%s\"", key, word);
    // printf("Push buffr %s\n", buffr);
    reply = redisCommand(red,buffrnp);
    printf("Redis PUSH status: %lld\n", reply->integer);
    freeReplyObject(reply);
    redisFree(red);
    return buffrnp;
}
*/

char
*get_redis(char *key)
{
if (redis_init()!=0)
    {
        return "";
    }
    char *buffr=NULL;
    char *balasan=NULL;
    // topic hanya maksimum 1000 character
    buffr=(char *) malloc(1010);
    sprintf(buffr, "RPOP %s", key);
    reply = redisCommand(red, buffr);
    // membuat besar balasan di memory sesuai besar pesan yang diambil di MQ2
    balasan=(char *) malloc(strlen(reply->str) + 1 );
    sprintf(balasan, "%s", reply->str);
    /*  Printf Debugging
        int errb=1000;
        errb=reply->integer;
        printf("Err number: %d\n", errb);
        printf("Redis POP %s: %s\n", buffr, balasan);
        printf("balasan: %s\n", balasan);
    */
    freeReplyObject(reply);
    free(buffr);
    redisFree(red);
    return balasan;
}