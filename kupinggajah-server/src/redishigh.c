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

// Deklarasi Prototipe
int redis_init(void);
int ping_redis(void);
int redis_check(char *);
int put_redis(char *, char *);
char *get_redis(char *);

// Fungsi untuk inisiasi MQ2
int
redis_init(void)
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
    }
    return 0;
}

// Fungsi untuk nyolek MQ2
int
ping_redis(void)
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

// Fungsi untuk cek key di MQ2 ada atau tidak
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

// Fungsi untuk naruh tumpukan pesan di MQ2
int
put_redis(char *key, char *word)
{
    if (redis_init()!=0)
    {
        return -1;
    }
    // Sesuaikan besar memory dengan pesan yang ingin disimpan.
    reply = redisCommand(red, "LPUSH %s \"%s\"", key, word);
    printf("Redis PUSH status: %lld\n", reply->integer);
    freeReplyObject(reply);
    redisFree(red);
    return 0;
}

// Fungsi untuk ambil tumpukan pesan di MQ2
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
    freeReplyObject(reply);
    free(buffr);
    redisFree(red);
    return balasan;
}