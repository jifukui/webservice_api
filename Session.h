#ifndef __SESSION_H_
#define __SESSION_H_
#include <openssl/ssl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "timers.h"
struct Session_Management{
    
};
struct Session_Version{
    unsigned int Major;
    unsigned int Modify;
    unsigned int Id;
};
struct UserInfo{

};
struct ConnectInfo{
    unsigned int stat ;
    unsigned int ipaddr;
    unsigned int token ;
    Timer * timer;
};
void DisplayVersion();
int CreatShareMemory();
int GetShareMemory();
int ShareMemoryInit();
void Display();
void Add(struct ConnectInfo *con);
void Del(struct ConnectInfo *con);
#endif