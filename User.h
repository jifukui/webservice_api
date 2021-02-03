#ifndef __USER_H_
#define __USER_H_
#include <openssl/ssl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "timers.h"
#include "config.h"
#include <jansson.h>
#define USER_NUM 20
#define NameLen 20
#define PassLen 64
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};
struct User{
    char name[NameLen];
    char pwd[PassLen];
    unsigned int forbidstat;
    int error;
    int forbidTimes;
    unsigned int time;
    unsigned int power;
};
void UserInfoInit();
#endif