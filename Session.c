#include "Session.h"

#define PATHNAME "."
#define PATHNAME1 "../"
#define PROJ_ID 0x6666
#define PROJ_ID1 0x6667
static void DisplayOpensslVersion();
static key_t key = 0;
static key_t key1 = 0;
static const int sharesize = 4096*2; 
struct Session_Version Version={1,0,0};
static struct Session_Management *sessionmanagement;
key_t session_shmid = 0;
key_t session_bshmid = 0 ;
///////////////////////////////////////////////////////////////////////////////////////////
static int CommonShareMemory(int size,int flags);
static int semaphore_allocation(key_t key,int sem_flags)
{
    return semget(key,1,sem_flags);
}
static int semaphore_deallocate()
{
    union semun ignored_argument;
    return semctl(session_bshmid,1,IPC_RMID,ignored_argument);
}
static int semaphore_initialize()
{
    union semun argument;
    unsigned short values[1];
    values[0]=1;
    argument.array=values;
    return semctl(session_bshmid,0,SETALL,argument);
}
static int semaphore_wait()
{
    struct sembuf operations[1];
    operations[0].sem_num=0;
    operations[0].sem_op=-1;
    operations[0].sem_flg=SEM_UNDO;
    printf("session waitting is %d\n",(int)getpid());
    return semop(session_bshmid,operations,1);
}
static int semaphore_post()
{
    struct sembuf operations[1];
    operations[0].sem_num=0;
    operations[0].sem_op=1;
    operations[0].sem_flg=SEM_UNDO;
    printf("session process is %d\n",(int)getpid());
    return semop(session_bshmid,operations,1);
}
void DisplayKeyInfo(){
    printf("the key is %u\r\n",key);
    printf("the key1 is %u\r\n",key1);
    printf("the session_shmid is %u\r\n",session_shmid);
    printf("the session_bshmid is %u\r\n",session_bshmid);
}
int ShareMemoryInit(){
    key = ftok(PATHNAME,PROJ_ID);
    key1 = ftok(PATHNAME1,PROJ_ID1);
    session_shmid =CreatShareMemory();
    session_bshmid = semaphore_allocation(key1,IPC_CREAT |0666);
    semaphore_initialize(key1);
    DisplayKeyInfo();
    //return SetShareMemory();
    return session_shmid;
}
void ShareMemoryDest(){
    if(session_bshmid!=-1)
	{
		semaphore_deallocate(session_bshmid);
	}
	if(session_shmid!=-1)
	{
		DestoryShm(session_bshmid);
	}
}
int CreatShareMemory(){
    CommonShareMemory(sharesize,IPC_CREAT|0666);
}
int GetShareMemory(){
    sessionmanagement = shmat(session_shmid,NULL,0);
}
int SetShareMemory(){
    CommonShareMemory(sharesize,IPC_CREAT);
}
int CommonShareMemory(int size,int flags){
    if(key<=0)
    {
        perror("ftok error\n");
        return -1;
    }
    if((session_shmid=shmget(key,size,flags))<0)
    {
        perror("shmget error\n");
        return -2;
    }
    return session_shmid;
}
int DestoryShm(int shmid)
{
    if(shmctl(shmid,IPC_RMID,NULL)<0)
    {
        perror("shmctl error\n");
        return -1;
    }
    return 0;
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
    printf("process is %d\n",(int)getpid());
    printf("the address is %u\r\n",sessionmanagement);
    printf("the min is %d max is %d num is %d\r\n",sessionmanagement->min,sessionmanagement->max,sessionmanagement->num);
    for(i;i < SESSION_NUM;i++ ){
        con = &sessionmanagement->sesssion[i] ;
        if(con->stat){
            printf("%d is used and stat is %d\r\n",i,con->stat);
            printf("the id is %u the token is %u\r\n",con->connect.ipaddr,con->connect.token);
            if(con->stat>1){
                printf("the user name is %s\r\n",con->user.username);
                printf("the user power is %d\r\n",con->user.power);
            }
        }
    }
}
int Add(struct ConnectInfo conn){
    printf("add\r\n");
    semaphore_wait();
    int i = sessionmanagement->min ;
    struct SessionInfo *con;
    if(sessionmanagement->num>=SESSION_NUM){
        return 0;
    }
    for(i;i < SESSION_NUM;i++ ){
        con = &sessionmanagement->sesssion[i] ;
        if(!con->stat){
            sessionmanagement->num++;
            con->stat = 1;
            con->connect.ipaddr = conn.ipaddr;
            con->connect.token = conn.token;
            sessionmanagement->sesssion->index = i;
            sessionmanagement->min = i;
            if(i>sessionmanagement->max){
                sessionmanagement->max = i;
            }
            break;
        }
    }
    semaphore_post();
    Display();
    if(i<=SESSION_NUM){
        return i;
    }else{
        return 0;
    }
}
int SetLogStat(unsigned int index,char *str){
    printf("SetLogStat\r\n");
    semaphore_wait();
    struct SessionInfo *con;
    if(!str){
        printf("SetLogStat error Info\r\n");
        return 0;
    }
    if(sessionmanagement->num<=0){
        return 0;
    }
    if(index<0 && index>sessionmanagement->max){
        return 0;
    }
    con = &sessionmanagement->sesssion[index] ;
    if(con->stat){
        con->stat = 2;
        strncpy(con->user.username,str,PASSWORDLEN-1);
        con->timer = NULL;
        if(index<sessionmanagement->min){
            sessionmanagement->min = index;
        }
        printf("SetLogStat connected success\r\n");
        
    }else{
        printf("SetLogStat connected failed\r\n");
    }
    semaphore_post();
    Display();
    return index;
}
int Del(unsigned int index){
    printf("Del\r\n");
    semaphore_wait();
    struct SessionInfo *con;
    if(index<0){
        index = sessionmanagement->min;
    }
    if(sessionmanagement->num<=0){
        return 0;
    }
    if(index<0&&index>sessionmanagement->max){
        return 0;
    }
    con = &sessionmanagement->sesssion[index] ;
    if(con->stat){
        sessionmanagement->num--;
        con->stat = 0;
        con->user.username[0]=0;
        con->timer = NULL;
        if(index<sessionmanagement->min){
            sessionmanagement->min = index;
        }
        printf("Del connected success\r\n");
        
    }else{
        printf("Del connected failed\r\n");
    }
    semaphore_post();
    Display();
    return index;
}