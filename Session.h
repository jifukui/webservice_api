#ifndef __SESSION_H_
#define __SESSION_H_
#include "User.h"
#define SESSION_NUM 20
struct Session_Version{
    unsigned int Major;
    unsigned int Modify;
    unsigned int Id;
};
struct UserInfo{
    char username[15];
    char password[PASSWORDLEN];
    unsigned char power;
};

struct ConnectInfo{
    unsigned int ipaddr;
    unsigned int token ;
};
typedef struct 
{
    char username[15];
    char password[PASSWORDLEN];
    unsigned char level;
    unsigned char times;
    unsigned char power;
    Timer * timer;
}authenticate;              
typedef struct{
    int security;
    authenticate Auth[AUTH_NUM];
}Auth_liguo;
struct SessionInfo{
    unsigned int stat ;
    unsigned int index ;
    struct ConnectInfo connect;
    struct UserInfo user;
    Timer * timer;
};
struct Session_Management{
    unsigned int min;
    unsigned int max;
    unsigned int num;
    struct SessionInfo sesssion[SESSION_NUM];
};
void DisplayVersion();
int CreatShareMemory();
int GetShareMemory();
int ShareMemoryInit();
void ShareMemoryDest();
void Display();
int Add(struct ConnectInfo conn);
int Del(int index);
#endif