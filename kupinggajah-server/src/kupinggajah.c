#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "kupinggajah.h"

void str_append(char subject[], const char insert[], int pos) {
    char *buf = NULL;
    buf = calloc(sizeof(char), strlen(subject) + pos + 1);
    //char buf[100] = {};
    // salin pos characters paling pertama
    strncpy(buf, subject, pos);
    int len = strlen(buf);
    // salin insert[] pada bagian akhir
    strcpy(buf+len, insert);
    // tambahkan ukuran insert
    len += strlen(insert);
    // salin sisanya
    strcpy(buf+len, subject+pos);
    // kembalikan ke subject
    strcpy(subject, buf);
    // dealokasi buf
    free(buf);
}