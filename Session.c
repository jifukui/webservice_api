#include "Session.h"

#define PATHNAME "."
#define PROJ_ID 0x6666
static void DisplayOpensslVersion();
static key_t key = 0;
static const int sharesize = 4096*2; 
struct Session_Version Version={1,0,0};
static struct Session_Management sessionmanagement;
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
    sessionmanagement = shmat(shmid,NULL,0);
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
    struct SessionInfo *con;
    for(i;i < SESSION_NUM;i++ ){
        con = &sessionmanagement.sesssion[i] ;
        if(con->stat){
            printf("%d is used and stat is %d\r\n",i,con->stat);
            printf("the id is %u the token is %u\r\n",con->connect.ipaddr,con->connect.token);
            if(con->stat>1){
                printf("the user name is %s\r\n",con->user.username);
            }
        }
    }
}
int Add(struct ConnectInfo conn){
    int i = sessionmanagement.min ;
    struct SessionInfo *con;
    if(num>=SESSION_NUM){
        return 0;
    }
    for(i;i < SESSION_NUM;i++ ){
        con =&sessionmanagement.sesssion[i] ;
        if(!con->stat){
            sessionmanagement.num++;
            con->stat = 1;
            con->connect.ipaddr = conn.ipaddr;
            con->connect.token = conn.token;
            sessionmanagement.num++;
            sessionmanagement.sesssion->index = i;
            sessionmanagement.min = i;
            if(i>sessionmanagement.max){
                sessionmanagement.max = i;
            }
            break;
        }
    }
    Display();
    if(i<=SESSION_NUM){
        return i;
    }else{
        return 0;
    }
}
int SetLogStat(unsigned int index,char *str){
    struct SessionInfo *con;
    if(!str){
        printf("SetLogStat error Info\r\n");
        return 0;
    }
    if(sessionmanagement.num<=0){
        return 0;
    }
    if(index<0 && index>sessionmanagement.max){
        return 0;
    }
    con =&sessionmanagement.sesssion[i] ;
    if(con->stat){
        con->stat = 2;
        strncpy(con->user.username,str,14);
        con->timer = NULL;
        if(i<sessionmanagement.min){
            sessionmanagement.min = i;
        }
        printf("Del connected success\r\n");
        
    }else{
        printf("Del connected failed\r\n");
    }
    Display();
    if(i<=SESSION_NUM){
        return i;
    }else{
        return 0;
    }
}
int Del(unsigned int index){
    struct SessionInfo *con;
    if(sessionmanagement.num<=0){
        return 0;
    }
    if(index<0&&index>sessionmanagement.max){
        return 0;
    }
    con =&sessionmanagement.sesssion[index] ;
    if(con->stat){
        sessionmanagement.num--;
        con->stat = 0;
        con->user.username[0]=0;
        con->timer = NULL;
        if(i<sessionmanagement.min){
            sessionmanagement.min = i;
        }
        printf("Del connected success\r\n");
        
    }else{
        printf("Del connected failed\r\n");
    }
    Display();
    if(i<=SESSION_NUM){
        return i;
    }else{
        return 0;
    }
}