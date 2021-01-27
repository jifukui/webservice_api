#include "Session.h"

#define PATHNAME "."
#define PROJ_ID 0x6666
static void DisplayOpensslVersion();
static key_t key = 0;
static const int sharesize = 4096; 
struct Session_Version Version={1,0,0};
static struct ConnectInfo *conn;
static shmid = 0;
///////////////////////////////////////////////////////////////////////////////////////////
static int CommonShareMemory(int size,int flags);
void DisplayKeyInfo(){
    printf("the key is %u\r\n",key);
}
int ShareMemoryInit(){
    key = ftok(PATHNAME,PROJ_ID);
    DisplayKeyInfo();
    return CreatShareMemory();
}
int CreatShareMemory(){
    CommonShareMemory(sharesize,IPC_CREAT|IPC_EXCL|0666);
}
int GetShareMemory(){
    conn = shmat(shmid,NULL,0);
}
int SetShareMemory(){
    CommonShareMemory(sharesize,IPC_CREAT);
}
CommonShareMemory(int size,int flags){
    if(key<0)
    {
        perror("ftok error\n");
        return -1;
    }
    if((shmid=shmget(key,size,flags))<0)
    {
        perror("shmget error\n");
        return -2;
    }
    return shmid;
}

void DisplayVersion(){
    printf("The Session Version is %d.%d.%d\r\n",Version.Major,Version.Modify,Version.Id);
    DisplayOpensslVersion();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void DisplayOpensslVersion(){
    printf("the openssl version is %s\r\n",OpenSSL_version(OPENSSL_VERSION));
}
void Display()
{
    int i = 0 ;
    struct ConnectInfo *con;
    for(i;i < 10;i++ ){
        con =&conn[i];
        if(con->stat){
            printf("the %d is used and ip is %u token is %u\r\n",i,con->ipaddr,con->token);
        }
    }
}
void Add(struct ConnectInfo *con1){
    int i = 0 ;
    struct ConnectInfo *con;
    for(i;i < 10;i++ ){
        con =&conn[i];
        if(!con->stat){
            con->stat = 1;
            con->ipaddr = con1->ipaddr;
            con->token = con1->token;
            break;
        }
    }
    Display();
}
void Del(struct ConnectInfo *con){
    int i = 0 ;
    struct ConnectInfo *con;
    for(i;i < 10;i++ ){
        con =&conn[i];
        if(con->stat){
            con->stat = 0;
            con->ipaddr = 0;
            con->token = 0;
            printf("Del connected success\r\n");
            break;
        }
    }
    Display();
}