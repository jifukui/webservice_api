#include "Session.h"

#define PATHNAME "."
#define PATHNAME1 "../User.c"
#define PROJ_ID 0x6666
#define PROJ_ID1 0x6667

static key_t key = 0;
static key_t key1 = 0;
static const int sharesize = 4096*2; 
static struct Session_Management *sessionmanagement;
static key_t session_shmid = 0;
static key_t session_bshmid = 0 ;
static void DisplayKeyInfo();
void InitUserInfo();
static void writesecurityfile();
static int CreatShareMemory();
struct AuthFile{
    unsigned int security;
    struct User user[USER_NUM];
    unsigned int num;
};
struct AuthFile JwtAuth;
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
    printf("user process is %d\n",(int)getpid());
    return semop(session_bshmid,operations,1);
}
static void DisplayKeyInfo(){
    printf("the user key is %u\r\n",key);
    printf("the user key1 is %u\r\n",key1);
    printf("the user_shmid is %u\r\n",session_shmid);
    printf("the user_bshmid is %u\r\n",session_bshmid);
}
static int CreatShareMemory(){
    CommonShareMemory(sharesize,IPC_CREAT|0666);
    DisplayKeyInfo();
}
static int GetShareMemory(){
    sessionmanagement = shmat(session_shmid,NULL,0);
}
static int SetShareMemory(){
    CommonShareMemory(sharesize,IPC_CREAT);
}
static int CommonShareMemory(int size,int flags){
    if(key<=0)
    {
        perror("user ftok error\n");
        return -1;
    }
    if((session_shmid=shmget(key,size,flags))<0)
    {
        perror("user shmget error\n");
        return -2;
    }
    return session_shmid;
}
static int DestoryShm(int shmid)
{
    if(shmctl(shmid,IPC_RMID,NULL)<0)
    {
        perror("user shmctl error\n");
        return -1;
    }
    return 0;
}
void UserInfoInit(){
    JwtAuth.security=2;
    int security;
	json_t *authfile,*authdata;
	json_error_t error;
	authfile=json_load_file("/nandflash/webserver/thttpd/bin/security.json",0,&error);
	if(authfile)
	{
		
		authdata=json_object_get(authfile,"security");
		security=(unsigned int )json_integer_value(authdata);
		printf("liguoauth.security is %d\n",security);
        if(security<2){

        }else if(security ==2){

        }
		authdata=json_object_get(authfile,"User");
		int i=0;
		char *str=NULL;
        json_t *authdata1;
	    json_t *authdata2;
		for(i;i<AUTH_NUM&&i<json_array_size(authdata);i++)
		{
			
		}
	}
	else
	{
		
	}
}
void InitUserInfo(){
    json_t * obj,*user,*arr;
    obj = json_object();
    arr=json_array();
    user = json_object();
    if(obj&&user){
        json_object_set_new(obj,"security",json_integer(2));
        json_object_set_new(user,"username",json_string("Admin"));
        json_object_set_new(user,"password",json_string("Admin"));
        json_object_set_new(user,"forbidstat",json_integer(0));
        json_object_set_new(user,"error",json_integer(3));
        json_object_set_new(user,"forbidTimes",json_integer(3));
        json_object_set_new(user,"time",json_integer(0));
        json_object_set_new(user,"power",json_integer(0));
        json_array_append(arr,user);
        json_object_set_new(obj,"User",arr);
        char *str=NULL;
		str=json_dumps(obj,JSON_PRESERVE_ORDER);
		printf("The str is %s\n",str);
		FILE *authfile=NULL;
		authfile=fopen("/nandflash/webserver/thttpd/bin/security1.json","w+");
		fwrite(str,1,strlen(str),authfile);
		fclose(authfile);
		free(str);
		if(str)
		{
			str=NULL;
		}

    }else{
        printf("Init user info error \r\n");
    }
}
void writesecurityfile()
{/*
	json_t *file;
	json_t *userarray;
	json_t *json;
	json_t *data;
	json_t *cpy;
	json_t *value;
	json=json_object();
	userarray=json_array();
	file=json_object();
	if(file&&userarray&&json)
	{
		int ji=0;
		data=json_integer(liguoauth.security);
		printf("The liguoauth.security is %d\n",liguoauth.security);
		json_object_set_new(file,"security",data);
		int i=0;
		for(i;i<AUTH_NUM;i++)
		{
			if(liguoauth.Auth[i].username[0]==0)
			{
				break;
			}
			else
			{
				printf("username is %s\n",liguoauth.Auth[i].username);
				printf("password is %s\n",liguoauth.Auth[i].password);
				data=json_string(liguoauth.Auth[i].username);
				json_object_set(json,"username",data);
				value=json_string(liguoauth.Auth[i].password);
				json_object_set(json,"password",value);
				cpy=json_deep_copy(json);
				json_array_append(userarray,cpy);
			}
		}
		json_object_set_new(file,"User",userarray);
		char *str=NULL;
		str=json_dumps(file,JSON_PRESERVE_ORDER);
		printf("The str is %s\n",str);
		FILE *authfile=NULL;
		authfile=fopen("/nandflash/webserver/thttpd/bin/security.json","w+");
		fwrite(str,1,strlen(str),authfile);
		fclose(authfile);
		free(str);
		if(str)
		{
			str=NULL;
		}
		json_decref(file);
		json_decref(userarray);
		json_decref(json);
		json_decref(data);
		json_decref(cpy);
		json_decref(value);
	}
	else
	{
		printf("write file error\n");
	}	*/
}