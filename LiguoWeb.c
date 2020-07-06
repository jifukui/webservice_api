#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <lig_matrix_dll_for_app.h>
#include <lig_inner_sig_system_param_change.h>
#include <sys/time.h>
#include "LiguoWeb.h"
#define DEBUG 0
#define PORTNUM 35
#define STRLEN 	20
#define GROUPPORT 256
#define READFILENUM 1024*12
typedef char int8;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned short int uint16;
typedef short int int16;
extern void *lighandle;
extern Auth_liguo liguoauth;
uint8 ligPorts=PORTNUM;
static uint8 JI_JsonCreatFailed[]="Creat JSON Failed";
key_t semid;
static uint8 ligportinfo[2][STRLEN]={"In Port Info","Out Port Info"};
static uint8 ligsignalinfo[em_matrix_input_signal_dvi+1][STRLEN]={"No Signal","Unknown","HDMI","DVI"};
static uint8 ligHDCPstatus[em_matrix_HDCP_version_2_0+1][STRLEN]={"HDCP OFF","HDCP 1.4","HDCP 2.2"};
static uint8 ligTMDSstatus[em_matrix_signal_TMDS_rates_8K+1][STRLEN]={"Less 4K30","4K30","4K60","8K"};
static uint8 ligCSstatus[em_matrix_signal_cs_YUV420+1][STRLEN]={"RGB444","YUV444","YUV422","YUV420"};
static uint8 ligDCstatus[em_matrix_signal_cd_48bit+1][STRLEN]={"24bits","30bits","36bits","48bits"};
static uint8 ligDisplay[em_matrix_output_display_dvi+1][STRLEN]={"No Display","Unknown","HDMI","DVI"};
static uint8 ligSinkAbility[em_matrix_ssignal_support_8K+1][STRLEN]={"Support 2K","Support 4K30","Support 4K60","Support 8K"};
static uint8 ligSinkCS[em_matrix_cs_support_YUV420+1][STRLEN]={"Support RGB444","Support YUV444","Support YUV422","Support YUV 420"};
static uint8 ligSinkDC[em_matrix_cd_support_48bit+1][STRLEN]={"Support 24bits","Support 30bits","Support 36bits","Support 48bits"};
static uint8 CommandHandle(const char *sstr,json_t *json,json_t *ech,json_t *res,char *estr);
static uint8 GetDeviceModuleName(char *data,char *estr);
static uint8 GetDeviceVideoInfo(char *data,char *estr);
static uint8 GetDeviceAudioInfo(char *data,char *estr);
static uint8 GetDeviceSwitchInfo(char *data,char *estr);
static uint8 GetDeviceDLLVersion(char *data,char *estr);
static uint8 GetPortEDID(json_t* json,char *data,char *estr);
static uint8 GetDeviceCardlist(char *data,char *estr);
static uint8 GetDeviceCardinfo(json_t *index_str ,char *data,char *estr);
static uint8 SetDeviceCardinfo(json_t *Card_json ,char *data,char *estr);
static uint8 GetPortInfo(json_t * json,char *data,char* estr);
static uint8 SetPortInfo(json_t * json,char *data,char* estr);
static uint8 SetDeviceNetwork(json_t * json,char *data,char* estr);
static uint8 SetDeviceRouting(json_t *json,char *estr);
static uint8 SetPortEDID(json_t* json,char *data,char *estr);
static uint8 DeviceVideoRouting(uint32 in,uint32 out);
static uint8 DeviceAudioRouting(uint32 in,uint32 out);
static uint8 GetShapshotPort(json_t *json, char *data, char *estr);
static uint8 _GetVideoInputPortShapshot(uint32 input_port_id, char *data, char *estr);
static uint8 _GetVideoOutputPortShapshot(uint32 output_port_id, char *data, char *estr);
static uint8 _GetAudioPortShapshot(uint32 port_id, uint32 dir, char *data, char *estr);
static uint8 UploadFile(json_t *fileinfo,char *data,char* estr);
static uint8 GetDeviceInfo(char *data,char *estr);
static uint8 SetDeviceReset(char *data,char *estr);
static uint8 SetDeviceFactory(char *data,char *estr);
static uint8 SetCardFactory(json_t *json,char*data,char *estr);
static uint8 SetSecurityStat(json_t *json,char *data,char *estr);
static uint8 GetSecurityStat(char *data,char *estr);
static uint8 SetUserPassword(json_t *json,char *data,char *estr);
static uint8 SetDHCPState(json_t *json,char *data,char *estr);
static uint8 SetDNSName(json_t *json,char *data,char *estr);
static uint8 UPgreadJsonFile(json_t *json,char *data,char *estr);
static uint8 GetStaticNetWork(char *data,char *estr);
static uint8 GetSerialPaud(json_t *json,char *data,char *estr);

/**环境监控*/
static uint8 GetVoltageStatus(char *data,char *estr);
static uint8 GetTemperatureStatus(char *data,char *estr);
static uint8 GetFanStatus(char *data,char *estr);
static uint8 GetAlertStatus(char *data,char *estr);
static uint8 Get48VStatus(char *data,char *estr);
static uint8 Set48VStatus(json_t *json,char *data,char *estr);
/**程序升级*/
static uint8 UntarUpgradeFile(json_t *json,char *data,char *estr);
static uint8 SetFirmwareUpgrade(json_t * json,char *data,char *estr);
static uint8 SetMoreFirmwareUpgrade(json_t * json,char *data,char *estr);
static uint8 GetUpgradeStatus(char * data,char *estr);
/**组操作*/
static uint8 GetSupportGroupPort(json_t *json,char *data,char *estr);
static uint8 SetGroupPragram(json_t *json,char *data,char *estr);
static uint8 GetGroupPragram(json_t *json,char *data,char *estr);
/**工具函数*/
static int Mysystem(const char* cmdstring);
static uint8 JsonGetString(json_t *json,char *data);
static uint8 JsonGetInteger(json_t *json,uint32 *data);
static uint8 JsonGetUint8(json_t *json,uint8 *data);
static void Uint8toString(int8 *str,uint8 *data,uint32 length);
static void StringtoUint8(uint8 *dis,int8 *str);
static uint8 JsonFromFile(uint8 *file,uint8 *data);
static int8  GetUserPassword(uint8 *user,uint8 *psw);
static void JsonInfoSetting(uint8 *flag,uint8 *info,json_t *json);
uint8 CheckPassword(uint8 *password);
/**内部中断*/
static uint8 SendSysIRQ(EM_LIG_SYS_PARAM sysparam);
static uint8 SendCardIRQ(EM_MATRIX_CARD_PARAM_IRQ_TYPE sysparam);
/****/
int binary_semaphore_allocation(key_t key,int sem_flags)
{
    return semget(key,1,sem_flags);
}
int binary_semaphore_deallocate(int semid)
{
    union semun ignored_argument;
    return semctl(semid,1,IPC_RMID,ignored_argument);
}
int binary_semaphore_initialize(int semid)
{
    union semun argument;
    unsigned short values[1];
    values[0]=1;
    argument.array=values;
    return semctl(semid,0,SETALL,argument);
}
int binary_semaphore_wait(int semid)
{
    struct sembuf operations[1];
    operations[0].sem_num=0;
    operations[0].sem_op=-1;
    operations[0].sem_flg=SEM_UNDO;
    //printf("waitting is %d\n",(int)getpid());
    return semop(semid,operations,1);
}
int binary_semaphore_post(int semid)
{
    struct sembuf operations[1];
    operations[0].sem_num=0;
    operations[0].sem_op=1;
    operations[0].sem_flg=SEM_UNDO;
    //printf("process is %d\n",(int)getpid());
    return semop(semid,operations,1);
}
/***/
void Uint8toString(int8 *str,uint8 *data,uint32 length)
{
#if DEBUG
	printf("have call Uint8toString the length is %d\n",length);
	printf("The address of str is %p",str);
#endif
	uint32 i,j=0;
	for(i=0;i<length;i++,j+=2)
	{
		sprintf(str+j,"%02X",*(data+i));
	}
#if DEBUG
	printf("the data is  %s\n",str);
#endif
}
void StringtoUint8(uint8 *dis,int8 *str)
{
	unsigned char data[3];
    data[2]='\0';
	uint32 test;
    while(*(str))
    {
        data[0]=*(str++);
        data[1]=*(str++);
        //sscanf(data,"%02x",dis++);
		sscanf(data,"%02X",&test);
		//printf("%s\n",data);
		//printf("%d\n",test);
		//printf("%d\n",sscanf(data,"%02X",&test));
		*dis=test%256;
		dis++;
    }
}
void JsonInfoSetting(uint8 *flag,uint8 *info,json_t *json)
{
	uint8 *str=NULL;
	str=json_dumps(json,JSON_PRESERVE_ORDER);
	//printf("The str is %s\n",str);
	strcpy(info,str);
	free(str);
	if(str)
	{
		str=NULL;
	}
	*flag=1;
}
uint8 LiguoWeb_GET_Method(const char *sstr,json_t *json,json_t *ech,json_t *res,char *estr)
{
	char* str=strchr(sstr,'=');
	uint8 flag=0;
	if(str)
	{
		flag=CommandHandle((str+1),json,ech,res,estr);
	}
	else
	{
		strcpy(estr,"Error of format");
	}
	if(str)
	{
		str=NULL;
	}
	return flag;
}
uint8 LiguoWeb_POST_Method(const unsigned char *sstr,json_t *json,json_t *ech,json_t * res,char *estr)
{
	return CommandHandle(sstr,json,ech,res,estr);
}
uint8 CommandHandle(const char *sstr,json_t *json,json_t *ech,json_t *res,char *estr)
{
	binary_semaphore_wait(semid);
	json_error_t error;
    json_t *jsonget;
    jsonget=json_loads(sstr,0,&error);
    uint8 flag=0;
	int8 data[20000];
    if(jsonget)
    {
        json_t *cmd;
        cmd=json_object_get(jsonget,"cmd");
		char str[30];
        if(JsonGetString(cmd,str))
        {	
			json_error_t error;
			json_object_set_new(json,"cmd",json_string(str));
            if(!strcmp(str,"matrix_status"))
			{
				flag=GetDeviceModuleName(data,estr);	
			}
			else if(!strcmp(str,"video_info"))
			{
				flag=GetDeviceVideoInfo(data,estr);
			}
			else if(!strcmp(str,"require_switch"))
			{
				flag=GetDeviceSwitchInfo(data,estr);
			}
			else if(!strcmp(str,"version_dll"))
			{
				flag=GetDeviceDLLVersion(data,estr);
			}
			else if(!strcmp(str,"audio_info"))
			{
				flag=GetDeviceAudioInfo(data,estr);
			}
			else if(!strcmp(str,"GetEDID"))
			{
				json_t *getedid;
				getedid=json_object_get(jsonget,"edid");
				if(getedid)
				{
					flag=GetPortEDID(getedid,data,estr);
					json_decref(getedid);
				}
				else
				{
					strcpy(estr,"Get EDID not the key edid");
				}
				json_decref(getedid);
			}
			else if(!strcmp(str,"Card_list"))
			{
				flag=GetDeviceCardlist(data,estr);
			}
			else if(!strcmp(str,"Card_info"))
			{
				json_t *index_str;
				index_str=json_object_get(jsonget,"index");
				flag=GetDeviceCardinfo(index_str,data,estr);
			}
			else if(!strcmp(str,"Card_info_set"))
			{
				json_t *Card_set_str;
				Card_set_str=json_object_get(jsonget,"Ethernet");
				flag=SetDeviceCardinfo(Card_set_str,data,estr);
			}
			else if(!strcmp(str,"routing"))
			{
				json_t *rout;
				rout=json_object_get(jsonget,"routing");										
				if(rout)
				{
					if(json_typeof(rout)==JSON_ARRAY)
					{
						flag=SetDeviceRouting(rout,estr);
					}
					else
					{
						strcpy(estr,"Error of rout type");
					}
					json_decref(rout);
				}
				else
				{
					strcpy(estr,"Error no rout");
				}
			}
			else if(!strcmp(str,"SetEDID"))
			{
				json_t *setedid;
				setedid=json_object_get(jsonget,"edid");
				if(setedid)
				{
					flag=SetPortEDID(setedid,data,estr);
					json_decref(setedid);
				}
				else
				{
					strcpy(estr,"Get EDID not the key edid");
				}
			}
			else if(!strcmp(str,"Upload"))
			{
				json_t *fileinfo;
				fileinfo=json_object_get(jsonget,"file");
				if(fileinfo)
				{
					flag=UploadFile(fileinfo,data,estr);
					json_decref(fileinfo);
				}
				else
				{
					strcpy(estr,"File no The key of file");
				}	
			}	
			else if(!strcmp(str,"GetPortInfo"))
			{
				json_t *portinfo;
				portinfo=json_object_get(jsonget,"PortInfo");
				if(portinfo)
				{
					flag=GetPortInfo(portinfo,data,estr);
				}
				else
				{
					strcpy(estr,"not the PortInfo Key");
				}	
			}
			else if(!strcmp(str,"SetPortInfo"))
			{
				json_t *portinfo;
				portinfo=json_object_get(jsonget,"PortInfo");
				if(portinfo)
				{
					flag=SetPortInfo(portinfo,data,estr);
				}
				else
				{
					strcpy(estr,"not the PortInfo Key");
				}
			}
			else if(!strcmp(str,"SetDeviceNetwork"))
			{
				json_t *SetDevice;
				SetDevice=json_object_get(jsonget,"Network");
				if(SetDevice)
				{
					flag=SetDeviceNetwork(SetDevice,data,estr);
				}
				else
				{
					strcpy(estr,"not the PortInfo Key");
				}
			}
			else if(!strcmp(str,"SnapshotPort"))
			{
				json_t *portinfo;
				portinfo=json_object_get(jsonget,"PortInfo");
				if(portinfo)
				{
					flag=GetShapshotPort(portinfo,data,estr);
				}
				else
				{
					strcpy(estr,"not the PortInfo Key");
				}
			}
			else if(!strcmp(str,"GetDeviceInfo"))
			{
				flag=GetDeviceInfo(data,estr);
			}
			else if(!strcmp(str,"SetDeviceReset"))
			{
				flag=SetDeviceReset(data,estr);
			}
			else if(!strcmp(str,"SetDeviceFactory"))
			{
				flag=SetDeviceFactory(data,estr);
			}
			else if(!strcmp(str,"SetCardFactory"))
			{
				json_t *cardinfo;
				cardinfo=json_object_get(jsonget,"CardInfo");
				if(cardinfo)
				{
					flag=SetCardFactory(cardinfo,data,estr);
				}
				else
				{
					strcpy(estr,"not the CardInfo Key");
				}
			}
			else if(!strcmp(str,"GetVoltageStatus"))
			{
				flag=GetVoltageStatus(data,estr);
			}
			else if(!strcmp(str,"GetTemperatureStatus"))
			{
				flag=GetTemperatureStatus(data,estr);
			}
			else if(!strcmp(str,"GetFanStatus"))
			{
				flag=GetFanStatus(data,estr);
			}
			else if(!strcmp(str,"GetAlertStatus"))
			{
				flag=GetAlertStatus(data,estr);
			}
			else if(!strcmp(str,"UntarFile"))
			{
				json_t *filename;
				filename=json_object_get(jsonget,"FileName");
				if(filename)
				{
					flag=UntarUpgradeFile(filename,data,str);
				}
				else
				{
					strcpy(estr,"not the FileName Key");
				}
			}
			else if(!strcmp(str,"SetUpgrade"))
			{
				json_t *filename;
				filename=json_object_get(jsonget,"file");
				if(filename)
				{
					flag=SetFirmwareUpgrade(filename,data,estr);
				}
				else
				{
					strcpy(estr,"not the file Key");
				}
			}
			else if(!strcmp(str,"SetMoreUpgrade"))
			{
				json_t *filename;
				filename=json_object_get(jsonget,"UpgradeInfo");
				if(filename)
				{
					flag=SetMoreFirmwareUpgrade(filename,data,estr);
				}
				else
				{
					strcpy(estr,"not the file Key");
				}
			}
			else if(!strcmp(str,"GetUpgradeStatus"))
			{
				flag=GetUpgradeStatus(data,estr);
			}
			else if(!strcmp(str,"GetSupportGroupPort"))
			{
				json_t *porttype;
				porttype=json_object_get(jsonget,"porttype");
				if(porttype)
				{
					flag=GetSupportGroupPort(porttype,data,estr);
				}
				else
				{
					strcpy(estr,"not the port type Key");
				}
			}
			else if(!strcmp(str,"GetGroupInfo"))
			{
				json_t *group;
				group=json_object_get(jsonget,"group");
				if(group)
				{
					flag=GetGroupPragram(group,data,estr);
				}
				else
				{
					strcpy(estr,"not the group Key");
				}	
			}
			else if(!strcmp(str,"SetGroupInfo"))
			{
				json_t *group;
				group=json_object_get(jsonget,"group");
				if(group)
				{
					flag=SetGroupPragram(group,data,estr);
				}
				else
				{
					strcpy(estr,"not the group Key");
				}	
			}
			else if(!strcmp(str,"SetSecurityStat"))
			{
				json_t *SecurityStat;
				SecurityStat=json_object_get(jsonget,"Security");
				if(SecurityStat)
				{
					flag=SetSecurityStat(SecurityStat,data,estr);
				}
				else
				{
					strcpy(estr,"not the Security Key");
				}	
			}
			else if(!strcmp(str,"GetSecurityStat"))
			{
				flag=GetSecurityStat(data,estr);
			}
			else if(!strcmp(str,"SetUserPassword"))
			{
				json_t *password;
				password=json_object_get(jsonget,"UserPassword");
				if(password)
				{
					flag=SetUserPassword(password,data,estr);
				}
				else
				{
					strcpy(estr,"not the UserPassword Key");
				}
			}
			else if(!strcmp(str,"setdnsname"))
			{
				json_t *name;
				name=json_object_get(jsonget,"name");
				if(name)
				{
					flag=SetDNSName(name,data,estr);
				}
				else
				{
					strcpy(estr,"not the name Key");
				}
			}
			else if(!strcmp(str,"setDHCPstate"))
			{
				json_t *dhcp;
				dhcp=json_object_get(jsonget,"dhcp");
				if(dhcp)
				{
					flag=SetDHCPState(dhcp,data,estr);
				}
				else
				{
					strcpy(estr,"not the dhcp key");
				}	
			}
			else if(!strcmp(str,"upgradejsonfile"))
			{
				json_t *filename;
				filename=json_object_get(jsonget,"filename");
				if(filename)
				{
					flag=UPgreadJsonFile(filename,data,estr);
				}
				else
				{
					strcpy(estr,"not the filename key");
				}	
			}
			else if(!strcmp(str,"Get48VStatus"))
			{
				flag=Get48VStatus(data,estr);
			}
			else if(!strcmp(str,"Set48VStatus"))
			{
				json_t *status;
				status=json_object_get(jsonget,"status");
				if(status)
				{
					flag=Set48VStatus(status,data,estr);
				}
				else
				{
					strcpy(estr,"Get status failed");
				}
			}
			else if(!strcmp(str,"getstaticnetwork"))
			{
				flag=GetStaticNetWork(data,estr);
			}
			else if(!strcmp(str,"GetSerialPaud"))
			{
				json_t *paud;
				paud=json_object_get(jsonget,"slot_id");
				if(paud)
				{
					flag=GetSerialPaud(paud,data,estr);
				}
				else
				{
					strcpy(estr,"Get Slot ID Failed");
				}
			}
#if DEBUG
			else if(!strcmp(str,"timeout"))
			{
				int n=0;
				while(1)
				{
					sleep(1);
					n++;	
				}
				flag=1;
			}
#endif
			else
			{
				strcpy(estr,"not this command");
			}
			if(flag)
			{
				res=json_loads(data,0,&error);
				json_object_set_new(ech,"result",res);
				json_object_set_new(json,"echo",ech);
			}
        }
        else
        {
            strcpy(estr,"No the key of cmd");
        }
        json_decref(cmd);
    }
    else
    {
        strcpy(estr,"The Format is error");
#if DEBUG
		json_object_set_new(json,"data",json_string(sstr));
		json_object_set_new(json,"errinfo",json_string(error.text));
		json_object_set_new(json,"err",json_string(error.source));
#endif
    }
	//printf("end of this %d\n",getpid());
	binary_semaphore_post(semid);
    return flag;

}

uint8 GetDeviceModuleName(char *data,char *estr)
{
	uint8 flag=0;
	uint8 filepath[50]="/tmp/matrix_cfg.segment";
	json_t *json;
	json_t *info;
	json=json_object();
	uint8 jsonfile[READFILENUM];
	json_error_t error;
	json_t *value;
	if(JsonFromFile(filepath,jsonfile))
	{
		info=json_loads(jsonfile,0,&error);
		if(json)
		{
			char *str;
			value=json_object_get(info,"matrix");
			if(value&&(JSON_OBJECT==json_typeof(value)))
			{
				value=json_object_get(value,"model");
				if(value&&(JSON_STRING==json_typeof(value)))
				{
					json_object_set_new(json,"name",value);
					str=json_dumps(json,JSON_PRESERVE_ORDER);
					strcpy(data,str);
					free(str);
					if(str)
					{
						str=NULL;
					}
					flag=1;
					json_decref(json);
				}
				else
				{
					strcpy(estr,"Get model value failed");
				}
			}
			else
			{
				strcpy(estr,"Get matrix value failed");
			}
		}
		else
		{
			strcpy(estr,"Error of command json init");
		}
	}
	else
	{
		strcpy(estr,"Get file fail");
	}
	return flag;
}

uint8 GetDeviceVideoInfo(char *data,char *estr)
{
#if DEBUG
	struct timeval start,end;
	unsigned long time;
	json_t *test;
	test=json_array();
	//gettimeofday(&start,NULL);
#endif
	json_t *json,*port,*arr,*cpy;
	uint8 flag=0;
	json=json_object();
	port=json_object();
	arr=json_array();
	cpy=json_object();
	if(json==(json_t*)0||port==(json_t *)0||arr==(json_t*)0||cpy==(json_t*)0)
	{
		strcpy(estr,"Get video info json init error");
	}
	else
	{
#if DEBUG
		json_object_set_new(json,"i",json_integer(0));
		json_object_set_new(json,"routting",json_integer(0));
		json_object_set_new(json,"time",test);
#endif
		json_object_set_new(json,"Port",arr);
		json_object_set_new(port,"index",json_integer(1));
		json_object_set_new(port,"Dir",json_string("In"));
		json_object_set_new(port,"status",json_string("On"));
		json_object_set_new(port,"switch",json_null());
		flag=1;
		int portinfo[ligPorts];
		int i,in,index=0,iindex=0;
		//memset(portinfo,0,(size_t)ligPorts);
		for(i=0;i<ligPorts;i++)
		{
			portinfo[i]=0;
		}
		//memset(switchinfo,0,ligPorts);
		
		for(i=1;i<=(ligPorts/2);i++)
		{
#if DEBUG
			gettimeofday(&start,NULL);
#endif
			in=lig_matrix_get_slot_online_state(lighandle,i);
#if DEBUG
			gettimeofday(&end,NULL);
			time=1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
			json_array_append(test,json_integer(time));
#endif
			if(in)
			{
				//printf("The solt %d have on line \n",i);
				portinfo[index++]=2*i-1;
				portinfo[index++]=2*i;
			}
		}
		LIG_MATRIX_OBJ_INPUT_SIGNAL in_info;
		LIG_MATRIX_OBJ_OUTPUT_DISPLAY out_info;
		i=0;
#ifdef DEBUG
            json_object_set(port,"i",json_integer(i));
#endif
		while(portinfo[i]>0)
		{
#if DEBUG
			json_object_set(port,"i",json_integer(i));
#endif		
			json_object_set(port,"index",json_integer(portinfo[i]));
#if DEBUG
            gettimeofday(&start,NULL);
#endif
			in=lig_matrix_get_active_video_pipe_io(lighandle,portinfo[i]);
			printf("The online port is %d and i is %d and dir is %d\n",portinfo[i],i,in);
#if DEBUG
            gettimeofday(&end,NULL);
            time=1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
            json_array_append(test,json_integer(time));
#endif
			if(0==in)
			{
				json_object_set(port,"Dir",json_string("In"));
				//input
				in=lig_matrix_get_port_input_signal_information(lighandle,portinfo[i],&in_info);
				if(in)
				{
					//error
					i++;
					continue;
				}
				else
				{
					//修改此处认为所有大于0的值为有输入源
					if(in_info.signal_status>0)
					{
						json_object_set(port,"status",json_string("On"));
					}
					else
					{
						json_object_set(port,"status",json_string("Off"));
					}
				}
				json_object_set(port,"switch",json_null());
				cpy=json_deep_copy(port);
				json_array_append(arr,cpy);
			}				
			else if(1==in)
			{
#if DEBUG
				//json_object_set(port,"routting",json_integer(switchinfo[iindex]));
#endif
				//iindex++;
				json_object_set(port,"Dir",json_string("Out"));
				in=lig_matrix_get_port_output_display_information(lighandle,portinfo[i],&out_info);
				if(in)
				{
					//error
					i++;
					continue;
				}
				else
				{
					//修改此处认为所有大于0的值为有效的负载
					if(out_info.display_status>0)
					{
						json_object_set(port,"status",json_string("On"));	
					}
					else
					{
						json_object_set(port,"status",json_string("Off"));
					}
				}
				in=lig_matrix_get_video_switch_state(lighandle,portinfo[i]);
				if(in>0)
				{
					json_object_set(port,"switch",json_integer(in));
				}
				else
				{
					json_object_set(port,"switch",json_integer(0));
				}
				cpy=json_deep_copy(port);
				json_array_append(arr,cpy);
			}
			else
			{
				//now no support
				printf("have another port online %d\n",in);
			}
			i++;
		}
		char *str;
		str=json_dumps(json,JSON_PRESERVE_ORDER);
		strcpy(data,str);
		free(str);
		if(str!=NULL)
		{
			str=NULL;
		}
		//json_decref(json);
		//json_decref(port);
		//json_decref(arr);
		//json_decref(cpy);
	}
	return flag;
}
uint8 GetDeviceAudioInfo(char *data,char *estr)
{
	json_t *json,*port,*arr,*cpy;
	uint8 flag=0;
	json=json_object();
	port=json_object();
	arr=json_array();
	cpy=json_object();
	if(json==(json_t*)0||port==(json_t *)0||arr==(json_t*)0||cpy==(json_t*)0)
	{
		strcpy(estr,"Get video info json init error");
	}
	else
	{
		json_object_set_new(json,"Port",arr);
		json_object_set_new(port,"index",json_integer(1));
		json_object_set_new(port,"Dir",json_string("In"));
		json_object_set_new(port,"status",json_string("On"));
		json_object_set_new(port,"switch",json_null());
		flag=1;
		int portinfo[ligPorts];
		memset(portinfo,0,ligPorts);
		int i,in,index=0;
		for(i=1;i<=(ligPorts/2);i++)
		{
			in=lig_matrix_get_slot_online_state(lighandle,i);
			if(in)
			{
				portinfo[index++]=2*i-1;
				portinfo[index++]=2*i;
			}
		}
		i=0;
		while(portinfo[i]>0)
		{
			json_object_set(port,"index",json_integer(portinfo[i]));
			in=lig_matrix_get_active_audio_pipe_io(lighandle,portinfo[i]);
			if(0==in)
			{
				//input
				json_object_set(port,"Dir",json_string("In"));
				json_object_set(port,"status",json_string("On"));
				json_object_set(port,"switch",json_null());
				cpy=json_deep_copy(port);
				json_array_append(arr,cpy);
			}				
			else if(1==in)
			{
				//output
				json_object_set(port,"Dir",json_string("Out"));
				json_object_set(port,"status",json_string("On"));
				in=lig_matrix_get_audio_switch_state(lighandle,portinfo[i]);
				json_object_set(port,"switch",json_integer(in));
				cpy=json_deep_copy(port);
				json_array_append(arr,cpy);
			}
			else
			{
					//now no support
			}
			i++;
		}
		char *str;
		str=json_dumps(json,JSON_PRESERVE_ORDER);
		strcpy(data,str);
		free(str);
		if(str!=NULL)
		{
			str=NULL;
		}
		json_decref(json);
		json_decref(port);
		json_decref(arr);
		json_decref(cpy);
	}
	
	return flag;
}
uint8 GetDeviceSwitchInfo(char *data,char *estr)
{
	uint8 i=0;
	int in;
	json_t *json,*port,*cpy,*arr;
	uint8 flag=0;
    json=json_object();
    port=json_object();
    arr=json_array();
    cpy=json_object();
    if(json==(json_t*)0||port==(json_t *)0||arr==(json_t*)0||cpy==(json_t*)0)
    {
        strcpy(estr,"Get switch info json init error");
    }
	else
	{
		flag=1;
		json_object_set_new(json,"routing",arr);
		json_object_set_new(port,"in",json_integer(0));
		json_object_set_new(port,"out",json_integer(0));
		json_object_set_new(port,"type",json_string("v"));
		//while(switchinfo[i])
		for(i=1;i<=PORTNUM;i++)
		{
			in=lig_matrix_get_video_switch_state(lighandle,i);
        	if(in>=0)
        	{
				json_object_set(port,"out",json_integer(i));
        		json_object_set(port,"in",json_integer(in));
        	}
        	else
        	{
			
				continue;
       			//json_object_set(port,"in",json_integer(0));
       		}
            cpy=json_deep_copy(port);
            json_array_append(arr,cpy);
		}
		char *str;
        str=json_dumps(json,JSON_PRESERVE_ORDER);
        strcpy(data,str);
        free(str);
        if(str!=NULL)
        {
            str=NULL;
        }
        json_decref(json);
        json_decref(port);
        json_decref(arr);
        json_decref(cpy);
	}	
	return flag;
}
uint8 GetDeviceDLLVersion(char *data,char *estr)
{
	
	uint8 flag=0;
	json_t *json;
	json=json_object();
	unsigned int version;
#if DEBUG
	struct timeval start,end;
#endif 
	if(json)
	{
		flag=1;
#if DEBUG
		gettimeofday(&start,NULL);
#endif
		
		version=lig_matrix_get_version(lighandle,0);
#if DEBUG	
		gettimeofday(&end,NULL);
		unsigned long time;
		time=1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
		json_object_set_new(json,"time",json_integer(time));
		
#endif
		json_object_set_new(json,"version",json_integer(version));
		json_object_set_new(json,"handle",json_integer((uint32)lighandle));
		char *str;
		str=json_dumps(json,JSON_PRESERVE_ORDER);
        strcpy(data,str);
        free(str);
        if(str!=NULL)
        {
            str=NULL;
        }
        json_decref(json);

	}
	else
	{
		strcpy(estr,"Get DLL Version Error");
	}

	return flag;
}
uint8 GetPortEDID(json_t* json,char *data,char *estr)
{
	uint8 flag=0;
	json_t *gedid;
	json_t *val;
	gedid=json_object();
	int8 sedid[513];
	memset(sedid,0,sizeof(sedid));
	if((json_t*)0!=json&&JSON_OBJECT==json_typeof(json)&&(json_t*)0!=gedid)
	{
		uint32 id,type,status;
		val=json_object_get(json,"index");
		if(JsonGetInteger(val,&id))
		{
			val=json_object_get(json,"type");
			if(JsonGetInteger(val,&type))
			{	
				uint8 value[256];
				status=lig_matrix_get_port_edid(lighandle,id,type,value,sizeof(value));
#if DEBUG
				printf("The id:%d type:%d  Get EDID status is %d\n",id,type,status);
#endif
				if(!status)
				{
					flag=1;
					Uint8toString(sedid,value,256);
					json_object_set_new(gedid,"EDID",json_string(sedid));
				}
				else
				{
					strcpy(estr,"Get EDID Error");
				}
			}
			else
			{
				strcpy(estr,"Get EDID type Error");
			}
		}
		else
		{
			strcpy(estr,"Get EDID index Error");
		}
		char *str;
		str=json_dumps(gedid,JSON_PRESERVE_ORDER);
#if DEBUG
		printf("The edid is %s\n",str);
#endif
        strcpy(data,str);
        free(str);
        if(str!=NULL)
        {
            str=NULL;
        }
        json_decref(gedid);
		json_decref(val);
	}
	else
	{
		strcpy(estr,"Get EDID json init error");
	}
	return flag;
}
uint8 SetPortEDID(json_t* json,char *data,char *estr)
{
	json_t *val,*edata,*val1;
	uint8 flag=0;
	if((json_t*)0!=json&&JSON_OBJECT==json_typeof(json))
	{
		uint32 type,in,origintype;
		uint8 edidstr[513];
		uint8 ediddata[256];
		val=json_object_get(json,"type");
		val1=json_object_get(json,"origintype");
		if(JsonGetInteger(val,&type)&&JsonGetInteger(val1,&origintype))
		{
			if(0==type)
			{
				val=json_object_get(json,"data");
				if((512>=json_string_length(val))&&(json_string_length(val)%128==0)&&JsonGetString(val,edidstr))//(512==json_string_length(val)&&
				{
					memset(ediddata,0xFF,sizeof(ediddata));
					StringtoUint8(ediddata,edidstr);
					val=json_object_get(json,"in");
					if(JSON_ARRAY==json_typeof(val))
					{
						uint8 num,i;
						uint32 in;
						uint32 status;
						num=json_array_size(val);
						for(i=0;i<num;i++)
						{
							edata=json_array_get(val,i);
							if(edata&&JsonGetInteger(edata,&in))
							{
#if DEBUG
								printf("The in is %d type is %d\n",in,type);
#endif
								printf("The origin type is %d\n",origintype);
								int8 cmdbuf[64];
								if(3!=origintype)
								{
									json_t *val2;
									val2=json_object_get(json,"out");
									uint32 out;
									if(JsonGetInteger(val2,&out))
									{
										printf("The out is %d\n",out);
										sprintf(cmdbuf, "lig_echo_cpedid %d %d %d 0", origintype,out,in);
									}
									else
									{
										strcpy(estr,"Get out port error");
										return flag;
									}
									
								}
								else
								{
									sprintf(cmdbuf, "lig_echo_ldedid 256 %d 0", in);
								}
								Mysystem(cmdbuf);
								status=lig_matrix_set_port_edid(lighandle,in,(EM_MATRIX_EDID_TYPE)type,ediddata,sizeof(ediddata));
								if(0!=status)
								{
									sprintf(cmdbuf, "lig_echo_clean_edid");
									Mysystem(cmdbuf);
									strcpy(estr,"COPY EDID failed ");
								}
								else
								{
									flag=1;
								}	
							}
							else
							{
								strcpy(estr,"The in type error");
							}
						}
					}
				}
				else
				{
					strcpy(estr,"error of edid data");
				}
			}
			else
			{
				strcpy(estr,"Error of type value");
			}
		}
		else
		{
			strcpy(estr,"Get  type error");
		}
	}
	else
	{
		strcpy(estr,"edid is not object");
	}
	json_decref(val);
	json_decref(edata);
	return flag;
}
uint8 UploadFile(json_t *fileinfo,char *data,char* estr)
{
	json_t *file;
	json_t *val;
	int8 filename[32];
	uint32 filesize;
	uint8 flag;
	//printf("start upload file\n");
	if(fileinfo&&(JSON_OBJECT==json_typeof(fileinfo)))
	{
		file=json_object_get(fileinfo,"name");
		if(JsonGetString(file,filename))
		{
#if DEBUG
			printf("the file name is %s\n",filename);
#endif
			file=json_object_get(fileinfo,"size");
			if(JsonGetInteger(file,&filesize))
			{
				file=json_object_get(fileinfo,"data");
				uint32 i=0;
				uint8 filedata[1024*1024*5];
#if DEBUG
				printf("The data size id %d\n",filesize);
				printf("The json type is %d\n",json_typeof(file));
				printf("The array size is %d\n",json_array_size(file));
#endif				
				for(i=0;i<filesize;i++)
				{
					val=json_array_get(file,i);
					JsonGetUint8(val,&filedata[i]);
//					printf("The %d value is %d\n",i,filedata[i]);
				}
				FILE *filed=NULL;
				filed=fopen(filename,"wb+");
				if(filed)
				{
					
					if( filesize==fwrite(filedata,filesize,1,filed))
					{
						flag=1;
					}
					else
					{
						strcpy(estr,"copy data error");
					}
				}
				fclose(filed);
				flag=1;
			}
		}
		json_decref(file);
		json_decref(val);
	}
	else
	{
		if(fileinfo)
		{
#if DEBUG
			printf("have this json the type is %d \n",json_typeof(fileinfo));
#endif
		}
		else
		{
#if DEBUG
			printf("not this json\n");
#endif
		}
		strcpy(estr,"error of file type");
	}
	return flag;
}
uint8 GetPortInfo(json_t * json,char *data,char* estr)
{
	uint8 flag=0;
	json_t *value;
	json_t *portinfo;
	json_t *portsetting;
	json_t *infoall;
	json_t *arr;
	json_t *arr1;
	json_t *cpy;
	json_t *abiltyinfo;
	json_t *info;
	portinfo=json_object();
	portsetting=json_object();
	infoall=json_object();
	arr=json_array();
	arr1=json_array();
	abiltyinfo=json_object();
	uint32 portindex;
	uint32 cardnum;
	int32 status;
	uint8 cardname[33];
	uint8 name[65];
	uint32 dir;
	uint8 list[16];
	uint16 num;
	uint32 i,j;
	uint16 datalen;
	uint32 datalist[16];
	json_t *stamp;
	time_t timestamp;
	LIG_MATRIX_OBJ_PARA_ATTRIBUTE ainfo;
	stamp=json_object_get(json,"timestamp");
	if(JsonGetInteger(stamp,&timestamp))
	{
		time_t time2;
		struct stat jifile;
		stat("/nandflash/webserver/thttpd/www/configuration.json",&jifile);
		time2=jifile.st_mtime;
		//printf("The timestat is %d\n",timestamp);
		//printf("The file time is %d\n",time2);
		if(time2<=timestamp)
		{
			json_object_set_new(infoall,"Waitting",json_true());
			JsonInfoSetting(&flag,data,infoall);
			return 1;
		}
	}
	else
	{
		strcpy(estr,"Get timestamp Failed");
		return 0;
	}
	
	if(json&&(JSON_OBJECT==json_typeof(json))&&portinfo&&portsetting&&infoall&&arr)
	{
		value=json_object_get(json,"index");
		if(JsonGetInteger(value,&portindex))
		{
			flag=1;
			json_object_set_new(portinfo,"Port Index",json_integer(portindex));
			cardnum=(portindex+1)/2;
#if DEBUG
			printf("The card num is %d\n",cardnum);
#endif
			status=lig_matrix_get_card_name(lighandle,cardnum,cardname,32);
			if(status>=0)
			{
				json_object_set_new(portinfo,"Port Type",json_string(cardname));
				dir=lig_matrix_get_active_video_pipe_io(lighandle,portindex);
				if(em_matrix_in==dir)
				{
					LIG_MATRIX_OBJ_INPUT_SIGNAL in_info;
					json_object_set_new(portinfo,"Direction",json_string("In"));
#if DEBUG
					status=lig_matrix_get_port_input_signal_information(lighandle,portindex,&in_info);
					if(!status)
					{
						//printf("in_info.signal_status is %d\n",in_info.signal_status);
						if(in_info.signal_status>=em_matrix_input_signal_none&&in_info.signal_status<=em_matrix_input_signal_dvi)
						{
							json_object_set_new(portinfo,"Signal",json_string(ligsignalinfo[in_info.signal_status]));
						}
						else
						{
#if DEBUG
							printf("error of siganl %d\n",in_info.signal_status);
#endif
						}
						//printf("in_info.hdcp_version is %d\n",in_info.hdcp_version);
						if(in_info.hdcp_version>=em_matrix_HDCP_version_none&&in_info.hdcp_version<=em_matrix_HDCP_version_2_0)
						{
							json_object_set_new(portinfo,"HDCP Status",json_string(ligHDCPstatus[in_info.hdcp_version]));
						}
						else
						{
#if DEBUG
							printf("error of HDCP %d\n",in_info.hdcp_version);
#endif
						}
						//printf("in_info.tmds_rates is %d \n",in_info.tmds_rates);
						if(in_info.tmds_rates>=em_matrix_signal_TMDS_rates_2K&&in_info.tmds_rates<=em_matrix_signal_TMDS_rates_8K)
						{
							json_object_set_new(portinfo,"TMDS",json_string(ligTMDSstatus[in_info.tmds_rates]));
						}
						else
						{
#if DEBUG
							printf("TMDS error %d\n",in_info.tmds_rates);
#endif 
						}
						//printf("in_info.cs_status is %d \n",in_info.cs_status);
						if(in_info.cs_status>=em_matrix_signal_cs_RGB444&&in_info.cs_status<=em_matrix_signal_cs_YUV420)
						{
							json_object_set_new(portinfo,"CS Status",json_string(ligCSstatus[in_info.cs_status]));
						}
						else
						{
#if DEBUG
							printf("CS status error %d\n",in_info.cs_status);
#endif
						}
						//printf("in_info.cd_status is %d\n",in_info.cd_status);
						if(in_info.cd_status>=em_matrix_signal_cd_24bit&&in_info.cd_status<=em_matrix_signal_cd_48bit)
						{
							json_object_set_new(portinfo,"DC Status",json_string(ligDCstatus[in_info.cd_status]));
						}
						else
						{
#if DEBUG
							printf("Deep Color error %d\n",in_info.cd_status);
#endif
						}
					}
					else
					{
#if DEBUG
						printf("Get in Port Info error %d\n",status);
#endif
					}
#endif
				}
				else if(em_matrix_out==dir)
				{
					LIG_MATRIX_OBJ_OUTPUT_DISPLAY out_info;
					json_object_set_new(portinfo,"Direction",json_string("Out"));
#if DEBUG
					status=lig_matrix_get_port_output_display_information(lighandle,portindex,&out_info);
					if(!status)
					{
						//printf("out_info.display_status is %d \n",out_info.display_status);
						if(out_info.display_status>=em_matrix_output_display_none&&out_info.display_status<=em_matrix_output_display_dvi)
						{
							json_object_set_new(portinfo,"Sink",json_string(ligDisplay[out_info.display_status]));
						}
						else
						{
#if DEBUG
							printf("error of Display %d\n",out_info.display_status);
#endif
						}
						//printf("out_info.hdcp_version is %d\n",out_info.hdcp_version);
						if(out_info.hdcp_version>=em_matrix_HDCP_version_none&&out_info.hdcp_version<=em_matrix_HDCP_version_2_0)
						{
							json_object_set_new(portinfo,"HDCP Status",json_string(ligHDCPstatus[out_info.hdcp_version]));
						}
						else
						{
#if DEBUG
							printf("error of HDCP %d\n",out_info.hdcp_version);
#endif
						}
						//printf("out_info.ssignal_ability is %d\n",out_info.ssignal_ability);
						if(out_info.ssignal_ability>=em_matrix_ssignal_support_2K&&out_info.ssignal_ability<=em_matrix_ssignal_support_8K)
						{
							json_object_set_new(portinfo,"Sink Ability",json_string(ligSinkAbility[out_info.ssignal_ability]));
						}
						else
						{
#if DEBUG
							printf("Sink Ability error %d\n",out_info.ssignal_ability);
#endif
						}
						//printf("out_info.cs_ability is %d \n",out_info.cs_ability);
						if(out_info.cs_ability>=em_matrix_cs_support_RGB444&&out_info.cs_ability<=em_matrix_cs_support_YUV420)
						{
							json_object_set_new(portinfo,"CS Ability",json_string(ligSinkCS[out_info.cs_ability]));
						}
						else
						{
#if DEBUG
							printf("CS status error %d\n",out_info.cs_ability);
#endif
						}
						//printf("out_info.cd_ability is %d \n");
						if(out_info.cd_ability>=em_matrix_cd_support_24bit&&out_info.cd_ability<=em_matrix_cd_support_48bit)
						{
							json_object_set_new(portinfo,"DP Ability",json_string(ligSinkDC[out_info.cd_ability]));
						}
						else
						{
#if DEBUG
							printf("Deep Color error %d\n",out_info.cd_ability);
#endif
						}
					}
					else
					{
#if DEBUG
						printf("Get Out Port Info error %d\n",status);
#endif
					}
#endif
				}
				else
				{
#if DEBUG
					printf("Error the dir %d\n",dir);
#endif
				}
			}
			else
			{
#if DEBUG
				printf("The status is %d\n",status);
#endif
				strcpy(estr,"Get card name error ");
			}
			json_object_set_new(infoall,"Info",portinfo);
			/*
			status=lig_matrix_get_port_parameter_list(lighandle,portindex,(EM_MATRIX_PARA_LIST *)list,sizeof(list),&num);
#if DEBUG
			printf("The parameter num is %d\n",num);
#endif
			json_object_set_new(abiltyinfo,"Name",json_string(""));
			json_object_set_new(abiltyinfo,"Value",json_integer(1));
			for(i=0;i<num;i++)
			{
				status=lig_matrix_get_port_parameter_attribute(lighandle,portindex,(EM_MATRIX_PARA_LIST)list[i],&ainfo);
				if(!status)
				{
#if DEBUG
					printf("The name is %s\n",ainfo.para_name);
#endif
					strcpy(name,ainfo.para_name);
					status=lig_matrix_get_port_parameter_value(lighandle,portindex,(EM_MATRIX_PARA_LIST)list[i],datalist,16,&datalen);
					if(!status)
					{
#if DEBUG
						printf("The name is %s\n",name);
#endif
						json_object_set(abiltyinfo,"Name",json_string(name));
						json_array_clear(arr);
						for(j=0;j<datalen;j++)
						{
#if DEBUG
							printf("The %d is %d\n",j,datalist[j]);
#endif
							json_array_append(arr,json_integer(datalist[j]));
						}
						json_object_set(abiltyinfo,"Value",arr);
						cpy=json_deep_copy(abiltyinfo);
						json_array_append(arr1,cpy);
					}
					else
					{
#if DEBUG
						printf("The status value is %d\n",status);
#endif
					}
				}
				else
				{
#if DEBUG
					printf("error for get %d",list[i]);
#endif
				}	
			}*/
			uint8 filepath[50];
			uint8 jsonread=0;
			sprintf(filepath,"/tmp/port_%d_cfg.segment",portindex);
			//printf("The path is %s\n",filepath);
			uint8 jsonfile[READFILENUM];
			uint8 loopnum=1;
			json_error_t error;
			do{
				if(JsonFromFile(filepath,jsonfile))
				{
					info=json_loads(jsonfile,0,&error);
					if(info)
					{
						jsonread=1;
						json_object_set_new(abiltyinfo,"Name",json_string(""));
						json_object_set_new(abiltyinfo,"Value",json_integer(0));
						json_object_set_new(abiltyinfo,"sid",json_integer(0));
						json_object_del(info,"datasize");
						json_object_del(info,"index");
						json_object_del(info,"dir");
						json_object_del(info,"type");
						json_object_del(info,"typeid");
						json_object_del(info,"EDID");
						void *iter;
						json_t *temp;
						const char *key;
						json_t *value1;
						json_t *value2;
						iter=json_object_iter(info);
						while(iter)
						{
							key = json_object_iter_key(iter);
							value1 = json_object_iter_value(iter);
							json_object_set(abiltyinfo,"Name",json_string(key));
							temp=json_object_get(value1,"value");
							json_object_set(abiltyinfo,"Value",temp);
							value2=json_object_get(value1,"sid");
							json_object_set(abiltyinfo,"sid",value2);
							//printf("key is %s\n",key);
							cpy=json_deep_copy(abiltyinfo);
							json_array_append(arr1,cpy);
							iter = json_object_iter_next(info, iter);
						}
						break;
					}		
				}
				if(!jsonread)
				{
					sleep(3);
				}
				
			}while (loopnum--);		
			json_object_set_new(infoall,"Setting",arr1);
			char *str;
			str=json_dumps(infoall,JSON_PRESERVE_ORDER);
			strcpy(data,str);
			free(str);
			if(str!=NULL)
			{
				str=NULL;
			}

		}
		else
		{
			strcpy(estr,"Get Index Error");
		}
	}
	else
	{
		strcpy(estr,"Error json");
	}
	return flag;
}

uint8 SetPortInfo(json_t * json,char *data,char* estr)
{
	uint8 flag=0;
	json_t *info;
	json_t *value;
	json_t *stamp=json_object();
	uint32 portindex;
	uint32 sid;
	uint32 val;
	int32 status;
	uint32 errorid[128];
	uint8 error=0;
	uint8 oldflag=0;
	uint32 i;
	time_t time1=time(NULL);
	//time_t time2;
	//struct stat jifile;
	//stat("/nandflash/webserver/thttpd/www/configuration.json",&jifile);
	//time2=jifile.st_mtime;
	//printf("the time 1 is %d\n",time1);
	//printf("the time 2 is %d\n",time2);
	if(stamp&&json&&(JSON_ARRAY)==json_typeof(json))
	{	
#if DEBUG
		printf("The json size is %d\n",json_array_size(json));
#endif
		for(i=0;i<json_array_size(json);i++)
		{
			info=json_array_get(json,i);
			value=json_object_get(info,"index");
			if(JsonGetInteger(value,&portindex))
			{
				value=json_object_get(info,"sid");
				if(JsonGetInteger(value,&sid))
				{
					value=json_object_get(info,"value");
					if(JsonGetInteger(value,&val))
					{
#if DEBUG
						printf("The index is %d sid is %d value is %d\n",portindex,sid,val);
#endif
						status=lig_matrix_set_port_parameter_value(lighandle,portindex,(EM_MATRIX_PARA_LIST)sid,val);
						if(!status)
						{
							//errorid[error++]=sid;
							flag=1;
						}
						else
						{
							errorid[error++]=sid;
							printf("Set error is %d\n",status);
#if DEBUG
							printf("Set error is %d\n",status);
#endif
						}
					}
					else
					{
#if DEBUG
						printf("Get value error");
#endif
					}
				}
				else
				{
#if DEBUG
					printf("Get sid error");
#endif 
				}
			}
			else
			{
#if DEBUG
				printf("Get index error");
#endif 
			}
		}
		//jifukuistamp
		oldflag=flag;
		json_object_set(stamp,"timestamp",json_integer(time1));
		JsonInfoSetting(&flag,data,stamp);
		flag=oldflag;
	}
	else
	{
#if DEBUG
		printf("The json typs is %d\n",json_typeof(json));
#endif
		strcpy(estr,"Error josn type");
	}
	for(i=0;i<error;i++)
	{
		sprintf(estr+strlen(estr),"%d,",errorid[i]);
	}
	if(error!=0)
	{
		estr[strlen(estr)-1]=NULL;
		printf("The error data is %s\r\n",estr);
	}
	//printf("the data is %s\n",data);
	return flag;
}

int Mysystem(const char* cmdstring)
{
	printf("The data is %s\n",cmdstring);
	return system(cmdstring);;
}

uint8 SetDeviceNetwork(json_t * json,char *data,char* estr)
{
	uint8 flag=0;
	json_t *info;
	json_t *v_json;
	uint8 name[5];
	uint8 parameter[10];
	uint8 value[64];
	uint8 changeflag=0;
	char buf_ifconfig[128]={0};
	uint8 oldflag;
#if DEBUG
	struct timeval start,end;
	unsigned long time;
#endif
	
	if(json&&(JSON_ARRAY)==json_typeof(json))
	{
		uint32 i;
		for(i=0;i<json_array_size(json);i++)
		{
			info=json_array_get(json,i);
			v_json=json_object_get(info,"name");
			if(JsonGetString(v_json,name))
			{
				v_json=json_object_get(info,"parameter");
				if(JsonGetString(v_json,parameter))
				{
					v_json=json_object_get(info,"value");
					if(JsonGetString(v_json,value))
					{
						memset(buf_ifconfig,0,128);
						sprintf(buf_ifconfig,"lig_ifcfg %s %s %s",name,parameter,value);
#if DEBUG
						printf("%s!!\n",buf_ifconfig);
#endif 
#if DEBUG
						gettimeofday(&start,NULL);
#endif
						if(Mysystem((const char *)buf_ifconfig)==0)
						{
#if DEBUG
							gettimeofday(&end,NULL);
							time=1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec;
							printf("time is %ld\n",time);
#endif
							if(!strcmp(name,"eth0"))
							{
								if(!strcmp(parameter,"IP"))
								{
									//flag=SendSysIRQ(em_lig_sys_param_eth0ip);
									changeflag|=(1<<0);
								}
								else if(!strcmp(parameter,"MASK"))
								{
									//flag=SendSysIRQ(em_lig_sys_param_eth0mask);
									changeflag|=(1<<1);
								}
								else if(!strcmp(parameter,"GATEWAY"))
								{
									//flag=SendSysIRQ(em_lig_sys_param_eth0gateway);
									changeflag|=(1<<2);
								}
								else if(!strcmp(parameter,"TCP_PORT"))
								{
									//system("kill -2 `ps | awk '{if($5~/lig_tcp/)printf $1}'`");
									oldflag=SendSysIRQ(em_lig_sys_param_eth0tcpport);
									if(oldflag)
									{
										flag|=(1<<0);
									}
									else
									{
										strcat(estr,parameter);
										strcat(estr," IRQ error ");
									}
									
								}
								else if(!strcmp(parameter,"UDP_PORT"))
								{
									//system("kill -2 `ps | awk '{if($5~/lig_udp/)printf $1}'`");
									oldflag=SendSysIRQ(em_lig_sys_param_eth0udpport);
									if(oldflag)
									{
										flag|=(1<<1);
									}
									else
									{
										strcat(estr,parameter);
										strcat(estr," IRQ error ");
									}
								}
								else
								{
									strcpy(estr,"ETH0 Not this");
								}
							}
							else if(!strcmp(name,"eth1"))
							{
								if(!strcmp(parameter,"IP"))
								{
									flag=SendSysIRQ(em_lig_sys_param_eth1ip);
								}
								else if(!strcmp(parameter,"MASK"))
								{
									flag=SendSysIRQ(em_lig_sys_param_eth1mask);
								}
								else if(!strcmp(parameter,"GATEWAY"))
								{
									flag=SendSysIRQ(em_lig_sys_param_eth1gateway);
								}
								else
								{
									strcpy(estr,"ETH1 Not this");
								}
							}
							
						}
						else
						{
#if DEBUG
							printf("set ERR!!!\n");
#endif 
							strcat(estr,parameter);
							strcat(estr," set error ");
						}
					}
					else
					{
#if DEBUG
						printf("NO value!!!\n");
#endif 
					}
				}
				else
				{
#if DEBUG
					printf("NO parameter!!!\n");
#endif 
				}
			}
			else
			{
#if DEBUG
				printf("NO Name!!!\n");
#endif 
			}
		}
		printf("The change flag is %d\n",changeflag);
		if(changeflag)
		{
			oldflag=SendSysIRQ(em_lig_sys_param_eth0);
			if(oldflag)
			{
				flag|=(1<<3);
			}
			else
			{
				strcat(estr," ETH0 IRQ error ");
			}
		}
	}
	else
	{
#if DEBUG
		printf("The json typs is %d\n",json_typeof(json));
#endif
		strcpy(estr,"Error josn type");
	}
	return flag;
}


uint8 _GetVideoInputPortShapshot(uint32 input_port_id, char *data, char *estr)
{
    uint8 flag=0;
    EM_MATRIX_HDCP_VERSION hdcp_version;
    EM_MATRIX_INPUT_SIGNAL_STATUS signal_status;
    uint32 cardnum;
    int32 status;
    uint8 cardname[33];
    LIG_MATRIX_OBJ_INPUT_SIGNAL in_info;

    json_t *port_info;
	json_t *info;
	json_t *value;
	json_t *arr;
	json_t *cpy;
	info=json_object();
	value=json_object();
	arr=json_array();
    port_info=json_object();
	uint16 datalen;
	uint32 datalist[16];
    if((json_t*)0==port_info||(json_t*)0==info||(json_t*)0==value||(json_t*)0==arr) {
        strcpy(estr,"Generate PortInfo error");
        return flag;
    }

    hdcp_version = lig_matrix_get_port_input_signal_hdcp_version(lighandle, input_port_id);//1:1.4 2:2.0
    signal_status = lig_matrix_get_port_input_signal_status(lighandle, input_port_id); //2:HDMI 3:DVI

    //Port Type
    cardnum = (input_port_id+1)/2;
    status = lig_matrix_get_card_name(lighandle,cardnum,cardname,32);
    if(status >= 0) {
        json_object_set_new(port_info,"Port Type",json_string(cardname));
    }

    //HDMI/DVI
    if (signal_status == em_matrix_input_signal_hdmi)
        json_object_set_new(port_info,"Signal Type",json_string("HDMI"));
    else if (signal_status == em_matrix_input_signal_dvi)
        json_object_set_new(port_info,"Signal Type",json_string("DVI"));
    else if (signal_status == em_matrix_input_signal_none)
        json_object_set_new(port_info,"Signal Type",json_string("NONE"));
    else if (signal_status == em_matrix_input_signal_unknown)
        json_object_set_new(port_info,"Signal Type",json_string("unknown"));

    status = lig_matrix_get_port_input_signal_information(lighandle,input_port_id,&in_info);
    if (status == 0) {
        if(in_info.tmds_rates >= 2) {
            json_object_set(port_info,"6G",json_string("Yes"));
        } else if(in_info.tmds_rates >= 0){
            json_object_set(port_info,"6G",json_string("No"));
        }
    }

    //HDCP Status
    if (hdcp_version == em_matrix_HDCP_version_1_4)
        json_object_set_new(port_info,"HDCP Status",json_string("1.4"));
    else if (hdcp_version == em_matrix_HDCP_version_2_0)
        json_object_set_new(port_info,"HDCP Status",json_string("2.2"));
    else if (hdcp_version == em_matrix_HDCP_version_none)
        json_object_set_new(port_info,"HDCP Status",json_string("NONE"));



	status=lig_matrix_get_port_parameter_value(lighandle,input_port_id, 110, datalist,16, &datalen);
    if(!status)
    {
		json_object_set(value,"Name",json_string("InHDCPVersion"));
		json_object_set(value,"Value",json_integer(datalist[0]));
		cpy=json_deep_copy(value);
		json_array_append(arr,cpy);
    }	
    flag = 1;
    char *str;
	json_object_set_new(info,"Info",port_info);
	json_object_set_new(info,"Setting",arr);
    str=json_dumps(info,JSON_PRESERVE_ORDER);
    strcpy(data,str);
    free(str);
    if(str!=NULL) {
        str=NULL;
    }
    json_decref(info);
    return flag;
}

uint8 _GetVideoOutputPortShapshot(uint32 output_port_id, char *data, char *estr)
{
    uint8 flag=0;
    EM_MATRIX_HDCP_VERSION hdcp_version;
    EM_MATRIX_OUTPUT_DISPLAY_STATUS signal_status;

    uint32 cardnum;
    int32 status;
    uint8 cardname[33];
    LIG_MATRIX_OBJ_OUTPUT_DISPLAY out_info;
    uint16 datalen;
    uint32 datalist[16];
	json_t *arr;
	json_t *info;
	json_t *value;
    json_t *port_info;
	json_t *cpy;
	arr=json_array();
	info=json_object();
	value=json_object();
    port_info=json_object();
    if((json_t*)0==port_info||(json_t*)0==arr||(json_t*)0==info||(json_t*)0==value) {
        strcpy(estr,"Generate PortInfo error");
        return flag;
    }

    hdcp_version = lig_matrix_get_port_output_display_hdcp_version(lighandle, output_port_id);//1:1.4 2:2.0
    signal_status = lig_matrix_get_port_output_display_status(lighandle, output_port_id); //2:HDMI 3:DVI

    //Port Type
    cardnum = (output_port_id+1)/2;
    status = lig_matrix_get_card_name(lighandle,cardnum,cardname,32);
    if(status >= 0) {
        json_object_set_new(port_info,"Port Type",json_string(cardname));
    }

    //HDMI/DVI
    if (signal_status == em_matrix_output_display_hdmi)
        json_object_set_new(port_info,"Sink Type",json_string("HDMI"));
    else if (signal_status == em_matrix_output_display_dvi)
        json_object_set_new(port_info,"Sink Type",json_string("DVI"));
    else if (signal_status == em_matrix_output_display_none)
        json_object_set_new(port_info,"Sink Type",json_string("NONE"));
    else if (signal_status == em_matrix_output_display_unknown)
        json_object_set_new(port_info,"Sink Type",json_string("unknown"));

    //3G/6G
    status = lig_matrix_get_port_output_display_information(lighandle, output_port_id, &out_info);
    if (status == 0) {
        if(out_info.ssignal_ability >= 2) {
            json_object_set(port_info,"6G",json_string("Yes"));
        } else if(out_info.ssignal_ability >= 0){
            json_object_set(port_info,"6G",json_string("No"));
        }
    }

    //HDCP Status
    if (hdcp_version == em_matrix_HDCP_version_1_4)
        json_object_set_new(port_info,"HDCP Status",json_string("1.4"));
    else if (hdcp_version == em_matrix_HDCP_version_2_0)
        json_object_set_new(port_info,"HDCP Status",json_string("2.2"));
    else if (hdcp_version == em_matrix_HDCP_version_none)
        json_object_set_new(port_info,"HDCP Status",json_string("NONE"));

	json_object_set_new(value,"Name",json_string(""));
	json_object_set_new(value,"Value",json_integer(0));
    //Switch Speed
    status=lig_matrix_get_port_parameter_value(lighandle,output_port_id, 75, datalist,16, &datalen);
    if(!status)
    {
        //static uint8 ligswitchspeed[em_para_sw_seamless+1][STRLEN]={"Normal", "fast", "Ext-fast", "Seamless"};
        //json_object_set(port_info,"SwitchSpeed",json_string(ligswitchspeed[(EM_MATRIX_PARA_SWITCH_SPEED)datalist[0]]));
		json_object_set(value,"Name",json_string("SwitchSpeed_2"));
		json_object_set(value,"Value",json_integer((EM_MATRIX_PARA_SWITCH_SPEED)datalist[0]));
		cpy=json_deep_copy(value);
		json_array_append(arr,cpy);
    }
	json_object_set_new(info,"Info",port_info);
	json_object_set_new(info,"Setting",arr);
    flag = 1;
    char *str;
    str=json_dumps(info,JSON_PRESERVE_ORDER);
    strcpy(data,str);
    free(str);
    if(str!=NULL) {
        str=NULL;
    }
    json_decref(info);
    return flag;
}

uint8 _GetAudioPortShapshot(uint32 port_id, uint32 dir, char *data, char *estr)
{
    uint8 flag=0;
    int32 status;

    uint16 datalen;
    uint32 datalist[16];

    json_t *port_info, *analog_audio;

    port_info = json_object();
    analog_audio = json_object();
	json_t *arr;
	json_t *arr1;
	json_t *info;
	json_t *value;
	json_t *cpy;
	json_t *setting;
	arr=json_array();
	arr1=json_array();
	info=json_object();
	value=json_object();
    port_info=json_object();
	setting=json_object();
    if((json_t*)0==port_info||(json_t*)0==analog_audio||(json_t*)0==arr||(json_t*)0==arr1||(json_t*)0==info||(json_t*)0==value||(json_t*)0==setting) {
        strcpy(estr,"Generate PortInfo error");
        return flag;
    }
	json_object_set_new(value,"Name",json_string(""));
	json_object_set_new(value,"Value",json_integer(0));
    //吉富逵实现
    // //Audio Source
    if(0==dir)
	{
		status=lig_matrix_get_port_parameter_value(lighandle,port_id, 101, datalist,16, &datalen);
     	//printf("Audio Source : %d\n", status);
     	if(!status)
     	{
        	json_object_set(setting,"Name",json_string("MatrixAudioSource"));
			json_object_set(setting,"Value",json_integer(datalist[0]));
     	}
	}
	else if(1==dir)
	{
		/*3.5方向*/
		uint32 dirflag;
		status=lig_matrix_get_port_parameter_value(lighandle,port_id, 104, datalist,16, &datalen);
		if(!status)
		{
			dirflag=datalist[0];
			json_object_set(value,"Name",json_string("ExtAudioADir"));
			json_object_set(value,"Value",json_integer(datalist[0]));
			cpy=json_deep_copy(value);
			json_array_append(arr,cpy);
		}
		/*3.5源*/
		if(0==dirflag)
		{

		}
		else
		{
			status=lig_matrix_get_port_parameter_value(lighandle,port_id, 105, datalist,16, &datalen);
			if(!status)
			{
				json_object_set(value,"Name",json_string("ExtAudioASource"));
				json_object_set(value,"Value",json_integer(datalist[0]));
				cpy=json_deep_copy(value);
				json_array_append(arr,cpy);
			}
		}
		
		/*HDMI源*/
		status=lig_matrix_get_port_parameter_value(lighandle,port_id, 106, datalist,16, &datalen);
		if(!status)
		{
			dirflag=datalist[0];
			json_object_set(value,"Name",json_string("HDMIOutAudioSelect"));
			json_object_set(value,"Value",json_integer(datalist[0]));
			cpy=json_deep_copy(value);
			json_array_append(arr1,cpy);
		}
	}
    // //Audio Source
    

    //ARC
    status=lig_matrix_get_port_parameter_value(lighandle,port_id, 102, datalist,16, &datalen);
    //printf("ARC status is : %d\n", status);
    if(!status)
    {
        //static uint8 ligaudiomute[em_para_arc_open+1][STRLEN]={"On", "Off"};
        //json_object_set(port_info,"ARC",json_string(ligaudiomute[(EM_MATRIX_PARA_ARC)datalist[0]]));
		//json_object_set(port_info,"ARC",json_integer((EM_MATRIX_PARA_ARC)datalist[0]));
		json_object_set(value,"Name",json_string("ARCEnable"));
		json_object_set(value,"Value",json_integer((EM_MATRIX_PARA_ARC)datalist[0]));
		cpy=json_deep_copy(value);
		json_array_append(arr1,cpy);
    }

    //Analog Audio: MUTE
    status=lig_matrix_get_port_parameter_value(lighandle,port_id, 91, datalist, 16,&datalen);
    if(!status)
    {
        //static uint8 ligaudiomute[em_para_aud_unmute+1][STRLEN]={"MUTE", "UNMUTE"};
        //json_object_set(analog_audio,"MUTE",json_string(ligaudiomute[(EM_MATRIX_PARA_AUD_MUTE)datalist[0]]));
		//json_object_set(analog_audio,"MUTE",json_integer((EM_MATRIX_PARA_AUD_MUTE)datalist[0]));
		json_object_set(value,"Name",json_string("AudioMute"));
		json_object_set(value,"Value",json_integer((EM_MATRIX_PARA_AUD_MUTE)datalist[0]));
		cpy=json_deep_copy(value);
		json_array_append(arr,cpy);
    }

    //Analog Audio: Volume
    status=lig_matrix_get_port_parameter_value(lighandle,port_id, 81, datalist, 16,&datalen);
    if(!status)
    {
        //json_object_set(analog_audio,"Volume",json_integer(datalist[0]));
		json_object_set(value,"Name",json_string("Volume"));
		json_object_set(value,"Value",json_integer(datalist[0]));
		cpy=json_deep_copy(value);
		json_array_append(arr,cpy);
    }

    //Analog Audio: Balance
    status=lig_matrix_get_port_parameter_value(lighandle,port_id, 84, datalist, 16,&datalen);
    if(!status)
    {
        //json_object_set(analog_audio,"Balance",json_integer(datalist[0]));
		json_object_set(value,"Name",json_string("AudioBalance"));
		json_object_set(value,"Value",json_integer(datalist[0]));
		cpy=json_deep_copy(value);
		json_array_append(arr,cpy);
    }

    //Analog Audio: Treble
    status=lig_matrix_get_port_parameter_value(lighandle,port_id, 88, datalist,16, &datalen);
    if(!status)
    {
        //json_object_set(analog_audio,"TREBLE",json_integer(datalist[0]));
		json_object_set(value,"Name",json_string("AudioTreble"));
		json_object_set(value,"Value",json_integer(datalist[0]));
		cpy=json_deep_copy(value);
		json_array_append(arr,cpy);
    }
	json_object_set_new(setting,"Analog Audio",arr);
	json_object_set_new(setting,"Digital Audio",arr1);
	json_object_set_new(info,"Info",json_object());
	json_object_set_new(info,"Setting",setting);
    //json_object_set(port_info,"Analog Audio",analog_audio);

    flag = 1;
    char *str;
    str=json_dumps(info,JSON_PRESERVE_ORDER);
    strcpy(data,str);
    free(str);
    if(str!=NULL) {
        str=NULL;
    }

    //json_decref(analog_audio);
    json_decref(info);
    return flag;
}


uint8 GetShapshotPort(json_t *json, char *data, char *estr)
{
    uint8 flag=0;
    json_t *val;

    uint32 index;
    char dir[20], type[20];

    //Guard json
    if ((json_t *)0 == json || JSON_OBJECT != json_typeof(json)) {
        strcpy(estr,"Get SnapshotPort json init error");
        return flag; 
    }
    
    //Guard index
    val=json_object_get(json,"index");
    if (!JsonGetInteger(val,&index)) {
        strcpy(estr,"Get SnapshotPort index Error");
        json_decref(val);
        return flag;
    }
  
    //Guard dir
    val=json_object_get(json,"Dir");
    if (!JsonGetString(val,dir)) {
        strcpy(estr,"Get SnapshotPort Dir Error");
        json_decref(val);
        return flag;
    }

    //Guard type
    val=json_object_get(json,"type");
    if (!JsonGetString(val,type)) {
        strcpy(estr,"Get SnapshotPort type Error");
        json_decref(val);
        return flag;
    }
    //Video
    if (!strcmp(type,"Video")) {
        if (!strcmp(dir,"In")) {
            flag = _GetVideoInputPortShapshot(index, data, estr);
        } else if (!strcmp(dir, "Out")) {
            flag = _GetVideoOutputPortShapshot(index, data, estr);
        }
    }

    //Audio
    if (!strcmp(type,"Audio")) {
        if (!strcmp(dir,"In")) {
            flag = _GetAudioPortShapshot(index, 0, data, estr);
        } else if (!strcmp(dir, "Out")) {
            flag = _GetAudioPortShapshot(index, 1, data, estr);
        }
    }

    json_decref(val);
    return flag;
}

uint8 GetDeviceCardlist(char *data,char *estr)
{
	json_t *json,*solt,*arr,*cpy;
	char namebuf[65];
	uint32 result;
	uint8 flag=0;
	json=json_object();
	solt=json_object();
	arr=json_array();
	cpy=json_object();
	LIG_MATRIX_OBJ_CARD Card_Info;
	if(json==(json_t*)0||solt==(json_t *)0||arr==(json_t*)0||cpy==(json_t*)0)
	{
		strcpy(estr,"Get Card list json init error");
	}
	else
	{

		json_object_set_new(json,"solt",arr);
		json_object_set_new(solt,"index",json_integer(1));
		json_object_set_new(solt,"status",json_string("online"));
		json_object_set_new(solt,"name",json_null());//json_string("On"));
		json_object_set_new(solt,"Direction",json_string("In"));
		json_object_set_new(solt,"Model_ID",json_integer(0));
		flag=1;
		int portinfo[ligPorts];
		memset(portinfo,0,ligPorts);
		int i,in,n;
		for(i=1;i<=(ligPorts/2);i++)
		{

			in=lig_matrix_get_slot_online_state(lighandle,i);

			if(in)
			{	
				memset(namebuf,0x00,64);
				n=lig_matrix_get_card_name(lighandle,(uint32)i,namebuf,64);
			}
			else
			{
				n=-1;
			}
			if(n<0)
			{
				json_object_set(solt,"index",json_integer(i));
				json_object_set(solt,"status",json_string("offline"));
				json_object_set(solt,"name",json_string("UnKnown"));
				json_object_set(solt,"Direction",json_null());
				json_object_set(solt,"Model_ID",json_null());
			}
			else
			{
				json_object_set(solt,"index",json_integer(i));
				json_object_set(solt,"status",json_string("online"));
				json_object_set(solt,"name",json_string((const char *)namebuf));
				result=lig_matrix_get_card_information(lighandle,i,&Card_Info);
				if(result<0)
				{
					//strcpy(estr,"Get Card info True error");
					printf("Get card %d infor error",i);
					json_object_set(solt,"Direction",json_null());
					json_object_set(solt,"Model_ID",json_null());
				}
				else
				{
					json_object_set(solt,"Model_ID",json_integer((int)Card_Info.type_id));
					if(Card_Info.io_dir==em_matrix_in)
					{
						json_object_set(solt,"Direction",json_string("In"));
					}
					else if(Card_Info.io_dir==em_matrix_out)
					{
						json_object_set(solt,"Direction",json_string("Out"));
					}
					else
					{
						json_object_set(solt,"Direction",json_string("Unknown"));
					}
				}	
			}
			cpy=json_deep_copy(solt);
			json_array_append(arr,cpy);
		}

		char *str;
		str=json_dumps(json,JSON_PRESERVE_ORDER);
		strcpy(data,str);
		free(str);
		if(str!=NULL)
		{
			str=NULL;
		}
		json_decref(json);
		json_decref(solt);
		json_decref(arr);
		json_decref(cpy);
	}
	
	return flag;
}
uint8 GetDeviceCardinfo(json_t *index_str ,char *data,char *estr)
{
	json_t *json,*Ethernet,*arr,*cpy;
	char namebuf[65];
	uint8 Ethbuf[32]={0};
	uint8 flag=0;
	int card_index=0,result;
	LIG_MATRIX_OBJ_CARD Card_Info;
	
	json=json_object();
	Ethernet=json_object();
	arr=json_array();
	cpy=json_object();
	if(json==(json_t*)0||Ethernet==(json_t *)0||arr==(json_t*)0||cpy==(json_t*)0)
	{
		strcpy(estr,"Get Card info json init error");
	}
	else
	{
		if(JsonGetInteger(index_str,&card_index))
		{
			//JsonGetInteger(index_str,card_index);
			//card_index=atoi(index_str);
			result=lig_matrix_get_card_information(lighandle,(uint32)card_index,&Card_Info);
			if(result<0)
			{
				strcpy(estr,"Get Card info True error");
			}
			else
			{
				json_object_set_new(json,"Index",json_integer(card_index));
				if(Card_Info.io_dir==em_matrix_in)
				{
					json_object_set_new(json,"Type",json_string("Input"));
				}
				else if(Card_Info.io_dir==em_matrix_out)
				{
					json_object_set_new(json,"Type",json_string("Output"));
				}
				else
				{
					json_object_set_new(json,"Type",json_string("Unknown"));
				}
				result=lig_matrix_get_card_name(lighandle,(uint32)card_index,namebuf,64);
				json_object_set(json,"Model",json_string((const char *)namebuf));
				json_object_set(json,"Model_ID",json_integer((int)Card_Info.type_id));
				if(Card_Info.io_dir==em_matrix_in)
				{
					json_object_set_new(json,"Direction",json_string("In"));
				}
				else if(Card_Info.io_dir==em_matrix_out)
				{
					json_object_set_new(json,"Direction",json_string("Out"));
				}
				else
				{
					json_object_set_new(json,"Direction",json_string("Unknown"));
				}
				memset(namebuf,0,64);
				sprintf(namebuf,"%d.%d.%d",Card_Info.ver_major,Card_Info.ver_minor,Card_Info.ver_build);
				
				json_object_set_new(json,"Version",json_string((const char *)namebuf));
				/*
				if(Card_Info.network_support==em_matrix_card_network_enable)
				{
					memset(Ethbuf,0,32);
					memset(namebuf,0,64);
					json_object_set_new(json,"Ethernet",arr);
					result=lig_matrix_get_card_mac(lighandle,(uint32)card_index,Ethbuf);
					if(result<0)
						json_object_set(Ethernet,"MAC",json_string("Unknown"));
					else
					{
						sprintf(namebuf,"%02x-%02x-%02x-%02x-%02x-%02x",Ethbuf[0],Ethbuf[1],Ethbuf[2],Ethbuf[3],Ethbuf[4],Ethbuf[5]);
						//printf("MAC is %s\n",namebuf);
						json_object_set(Ethernet,"MAC",json_string((const char *)namebuf));
					}
					memset(Ethbuf,0,32);
					memset(namebuf,0,64);
					result=lig_matrix_get_card_ip(lighandle,(uint32)card_index,Ethbuf);
					if(result<0)
						json_object_set(Ethernet,"IP",json_string("Unknown"));
					else
					{
						//printf("IP is %02x.%02x.%02x.%02x.%02x.%02x\n",Ethbuf[0],Ethbuf[1],Ethbuf[2],Ethbuf[3],Ethbuf[4],Ethbuf[5]);
						sprintf(namebuf,"%d.%d.%d.%d",Ethbuf[0],Ethbuf[1],Ethbuf[2],Ethbuf[3]);
						//printf("IP is %s\n",namebuf);
						json_object_set(Ethernet,"IP",json_string((const char *)namebuf));
					}
					memset(Ethbuf,0,32);
					memset(namebuf,0,64);
					result=lig_matrix_get_card_mask(lighandle,(uint32)card_index,Ethbuf);
					if(result<0)
						json_object_set(Ethernet,"MASK",json_string("Unknown"));
					else
					{
						sprintf(namebuf,"%d.%d.%d.%d",Ethbuf[0],Ethbuf[1],Ethbuf[2],Ethbuf[3]);
						//printf("MASK is %s\n",namebuf);
						json_object_set(Ethernet,"MASK",json_string((const char *)namebuf));
					}
					memset(Ethbuf,0,32);
					memset(namebuf,0,64);
					result=lig_matrix_get_card_gate(lighandle,(uint32)card_index,Ethbuf);
					if(result<0)
						json_object_set(Ethernet,"GateWay",json_string("Unknown"));
					else
					{
						sprintf(namebuf,"%d.%d.%d.%d",Ethbuf[0],Ethbuf[1],Ethbuf[2],Ethbuf[3]);
						//printf("GateWay is %s\n",namebuf);
						json_object_set(Ethernet,"GateWay",json_string((const char *)namebuf));
					}
					
					memset(Ethbuf,0,32);
					memset(namebuf,0,64);
					result=lig_matrix_get_card_net_port(lighandle,(uint32)card_index,em_matrix_net_tcp);
					if(result<0)
						json_object_set(Ethernet,"TCP",json_string("Unknown"));
					else
						json_object_set(Ethernet,"TCP",json_integer(result));
					memset(Ethbuf,0,32);
					memset(namebuf,0,64);
					result=lig_matrix_get_card_net_port(lighandle,(uint32)card_index,em_matrix_net_udp);
					if(result<0)
						json_object_set(Ethernet,"UDP",json_string("Unknown"));
					else
						json_object_set(Ethernet,"UDP",json_integer(result));
					cpy=json_deep_copy(Ethernet);
					json_array_append(arr,cpy);
				}else
				{
					json_object_set_new(json,"Ethernet",json_null());
				}*/
				flag=1;
			}
		}
		else
		{
			strcpy(estr,"Get Card info Index error");
		}		
		char *str;
		str=json_dumps(json,JSON_PRESERVE_ORDER);
		strcpy(data,str);
		free(str);
		if(str!=NULL)
		{
			str=NULL;
		}
		json_decref(json);
		json_decref(Ethernet);
		json_decref(arr);
		json_decref(cpy);
	}
	
	return flag;
}

uint8 SetDeviceCardinfo(json_t *Card_json ,char *data,char *estr)
{
	json_t *jsonindex,*IP_json,*Mask_json,*GATE_json,*TCP_json,*UDP_json;
	uint8 Netbuf[64];
	uint8 NetSetbuf[16];
	uint32 NetGetBuf[16],PortVal;
	uint16 NetPortVal;
	uint8 flag=0;
	int card_index=0,result;
	if((json_t*)0!=Card_json&&JSON_OBJECT==json_typeof(Card_json))
	{
		jsonindex=json_object_get(Card_json,"index");
		if(JsonGetInteger(jsonindex,&card_index))
		{
			IP_json=json_object_get(Card_json,"IP");
			memset(Netbuf,0,64);
			memset(NetSetbuf,0,16);
			memset(NetGetBuf,0,16);
			if(JsonGetString(IP_json,Netbuf))
			{
#if DEBUG
				printf("GET IP IS %s\n",Netbuf);
#endif
				sscanf((char *)Netbuf,"%d.%d.%d.%d\n",&NetGetBuf[0],&NetGetBuf[1],&NetGetBuf[2],&NetGetBuf[3]);
				NetSetbuf[0]=NetGetBuf[0];
				NetSetbuf[1]=NetGetBuf[1];
				NetSetbuf[2]=NetGetBuf[2];
				NetSetbuf[3]=NetGetBuf[3];
				flag =1;
				result=lig_matrix_set_card_ip(lighandle,(uint32)card_index,NetSetbuf);
				if(result<0)
				{
					strcpy(estr,"Set Card info IP error");
					flag =0;
				}
				else
					flag =1;
			}
			
			Mask_json=json_object_get(Card_json,"MASK");
			memset(Netbuf,0,64);
			memset(NetSetbuf,0,16);
			memset(NetGetBuf,0,16);
			if(JsonGetString(Mask_json,Netbuf))
			{
#if DEBUG
				printf("GET MASK IS %s\n",Netbuf);
#endif
				sscanf((char *)Netbuf,"%d.%d.%d.%d\n",&NetGetBuf[0],&NetGetBuf[1],&NetGetBuf[2],&NetGetBuf[3]);
				NetSetbuf[0]=NetGetBuf[0];
				NetSetbuf[1]=NetGetBuf[1];
				NetSetbuf[2]=NetGetBuf[2];
				NetSetbuf[3]=NetGetBuf[3];
				flag =1;
				result=lig_matrix_set_card_mask(lighandle,(uint32)card_index,NetSetbuf);
				if(result<0)
				{
					strcpy(estr,"Set Card info MASK error");
					flag =0;
				}
				else
					flag =1;
			}
						
			GATE_json=json_object_get(Card_json,"Gate");
			memset(Netbuf,0,64);
			memset(NetSetbuf,0,16);
			memset(NetGetBuf,0,16);
			if(JsonGetString(GATE_json,Netbuf))
			{
#if DEBUG
				printf("GET Gate IS %s\n",Netbuf);
#endif
				sscanf((char *)Netbuf,"%d.%d.%d.%d\n",&NetGetBuf[0],&NetGetBuf[1],&NetGetBuf[2],&NetGetBuf[3]);
				NetSetbuf[0]=NetGetBuf[0];
				NetSetbuf[1]=NetGetBuf[1];
				NetSetbuf[2]=NetGetBuf[2];
				NetSetbuf[3]=NetGetBuf[3];
				flag =1;
				result=lig_matrix_set_card_gate(lighandle,(uint32)card_index,NetSetbuf);
				if(result<0)
				{
					strcpy(estr,"Set Card info Gate error");
					flag =0;
				}
				else
					flag =1;
			}
			
			TCP_json=json_object_get(Card_json,"TCP");
			NetPortVal=0;
			if(JsonGetInteger(TCP_json,&PortVal))
			{
#if DEBUG
				printf("GET TCP IS %d\n",PortVal);
#endif
				NetPortVal=PortVal;
				result=lig_matrix_set_card_net_port(lighandle,(uint32)card_index,em_matrix_net_tcp,NetPortVal);
				if(result<0)
				{
					strcpy(estr,"Set Card info TCP error");
					flag =0;
				}
				else
					flag =1;
			}
			else
			{
				strcpy(estr,"Error TCP");
			}
			UDP_json=json_object_get(Card_json,"UDP");
			NetPortVal=0;
			if(JsonGetInteger(UDP_json,&PortVal))
			{
#if DEBUG
				printf("GET UDP IS %d\n",PortVal);
#endif
				NetPortVal=PortVal;
				result=lig_matrix_set_card_net_port(lighandle,(uint32)card_index,em_matrix_net_udp,NetPortVal);
				if(result<0)
				{
					strcpy(estr,"Set Card info UDP error");
					flag =0;
				}
				else
					flag =1;
			}
			
		}else
		{
			strcpy(estr,"Set Card info index error");
		}
	}
	else
	{
		strcpy(estr,"Json init error");
	}
	json_decref(jsonindex);
	json_decref(IP_json);
	json_decref(Mask_json);
	json_decref(GATE_json);
	json_decref(TCP_json);
	json_decref(UDP_json);
	return flag;
}


/*只是截取数据并未对数据的正确性进行*/
uint8 SetDeviceRouting(json_t *json,char *estr)
{
	uint8 flag=0;
	uint8 ff91=1;
	json_t *data;
	json_t * val;
	uint8 num;
	num=json_array_size(json);
	uint8 i;
	uint32 in,out;
	int32 value;
	char str[5];
	//value=lig_matrix_set_db_mode(lighandle,1);
#if DEBUG
	printf("The open database value is %d\n",value);
#endif
	for(i=0;i<num;i++)
	{
		data=json_array_get(json,i);
		if(data&&json_typeof(data)==JSON_OBJECT)
		{
			val=json_object_get(data,"in");
			if(JsonGetInteger(val,&in))
			{
				val=json_object_get(data,"out");
				if(JsonGetInteger(val,&out))
				{
					val=json_object_get(data,"type");
					if(JsonGetString(val,str))
					{
						/*视频切换*/
						if(!strcmp(str,"v"))
						{	
							/*视频预切换错�?*/
							value=ex_lig_matrix_stash_video_switch_data(lighandle,in,out);
							//if(!DeviceVideoRouting(in,out))
#if DEBUG
							printf("The pre Switch status is %d",value);
#endif
							if(value)
							{
								ff91=0;
								strcpy(estr,"Routing Video Error");
								break;
							}
						}
						/*音频切换*/
						else if(!strcmp(str,"a"))
						{
							if(!DeviceAudioRouting(in,out))
							{
								ff91=0;
								strcpy(estr,"Routing Audio Error");
								break;
							}
						}
						/*音视频切�?*/
						else if(!strcmp(str,"av"))
						{
							if(!DeviceVideoRouting(in,out))
							{
								ff91=0;
								strcpy(estr,"Routing Video Error");
								break;
							}
							if(!DeviceAudioRouting(in,out))
							{
								ff91=0;
								strcpy(estr,"Routing Audio Error");
								break;
							}
						}
						else
						{
							ff91=0;
							sprintf(estr,"Error of type type %s",str);
							break;
						}
						
					}
					else
					{
						ff91=0;
						strcpy(estr,"Error type is not string");
						break;
					}
				}
				else
				{
					ff91=0;
					strcpy(estr,"Error of out type");
					break;
				}
			}
			else
			{
				ff91=0;
				strcpy(estr,"Error of in type");
				break;
			}
		}
		else
		{
			ff91=0;
			strcpy(estr,"The array is not object");
			break;
		}
	}
	if(ff91)
	{
		value=ex_lig_matrix_set_video_switch_done(lighandle);
		//value=lig_matrix_set_video_switch_done(lighandle);
#if DEBUG
		printf("The video switch  value is %d\n",value);
#endif
		if(value>=0)
		{
#if DEBUG
			printf("The audio switch  value is %d\n",value);
#endif
			value=lig_matrix_set_audio_switch_done(lighandle);
			if(value>=0)
			{
				flag=1;
			}
			else
			{
				strcpy(estr,"Audio done error");
			}
		}
		else
		{
			strcpy(estr,"Video done error");
		}
		
	}
	//value=lig_matrix_set_db_mode(lighandle,0);
#if DEBUG
	printf("The ff91 status is %d\n",ff91);
	printf("The close database value is %d\n",value);
#endif
	return flag;
}
uint8 GetDeviceInfo(char *data,char *estr)
{
	uint8 flag=0;
	uint8 filepath[50]="/tmp/matrix_cfg.segment";
	json_t *info;
	json_t *deviceInfo;
	json_t *baseinfo;
	json_t *device;
	//json_t *arr;
	uint8 temp[50]="test";
	//json_t *cpy;
	json_t *json;
	json_t *value;
	deviceInfo=json_object();
	baseinfo=json_object();
	device=json_object();
	//ethinfo2=json_object();
	//ETH=json_object();
	uint8 jsonfile[READFILENUM];
	json_error_t error;
	if(JsonFromFile(filepath,jsonfile))
	{
		info=json_loads(jsonfile,0,&error);
		if(deviceInfo&&baseinfo&&info&&device)
		{
			json=json_object_get(info,"matrix");
			if(json&&(JSON_OBJECT==json_typeof(json)))
			{
				value=json_object_get(json,"model");
				if(JsonGetString(value,temp))
				{
					json_object_set_new(baseinfo,"Model",json_string(temp));
					value=json_object_get(json,"name");
					if(JsonGetString(value,temp))
					{
						json_object_set_new(baseinfo,"Name",json_string(temp));
						value=json_object_get(json,"sn");
						if(JsonGetString(value,temp))
						{
							json_object_set_new(baseinfo,"SN",json_string(temp));
							value=json_object_get(json,"version");
							if(JsonGetString(value,temp))
							{
								flag=1;
								json_object_set_new(baseinfo,"FirewareVersion",json_string(temp));
							}
							else
							{
#if DEBUG
								printf("Error get sn");
#endif 
							}
						}
						else
						{
#if DEBUG
							printf("Error get sn");
#endif 						
						}
					}
					else
					{
#if DEBUG
						printf("Error get name");
#endif 						
					}
				}
				else
				{
#if DEBUG
					printf("Error get model");
#endif 
				}
			}
			else
			{
				//
				if(json)
				{
					printf("good for get \n");
					printf("The type of json is %d\n",json_typeof(json));
				}
				else
				{
					printf("Error for get \n");
				}
				strcpy(estr,"Error of file for matrix");
			}
			json=json_object_get(info,"communication");
			json=json_object_get(json,"Ethernet");
			json_object_set_new(deviceInfo,"BaseInfo",baseinfo);
			json_object_set_new(deviceInfo,"Ethernet",json);
			json_object_set_new(device,"DeviceInfo",deviceInfo);
			char *str;
			str=json_dumps(device,JSON_PRESERVE_ORDER);
			strcpy(data,str);
			free(str);
			if(str!=NULL)
			{
				str=NULL;
			}
			json_decref(device);
		
		}
		else
		{
			if(info)
			{
				printf("good for info \n");
			}
			else
			{
				printf("error for info\n");
			}
			strcpy(estr,"Json init error");
		}
	}
	else
	{
		strcpy(estr,"Get file failed");
	}
	return flag;
}
uint8 SetDeviceReset(char *data,char *estr)
{
	uint8 flag=0;
	uint32 status;
	status=lig_matrix_set_inner_signal_reboot_opt(lighandle, em_irq_reboot_reset);
	if(!status)
	{
		flag=1;
	}
	else
	{
		strcpy(estr,"Reset Device error");
	}
	return flag;
}
uint8 SetDeviceFactory(char *data,char *estr)
{
	uint8 flag=0;
	uint32 status;
	status=lig_matrix_set_inner_signal_reboot_opt(lighandle, em_irq_reboot_factory);
	if(!status)
	{
		flag=1;
	}
	else
	{
		strcpy(estr,"factory Device error");
	}
	return flag;
}
uint8 SetCardFactory(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	uint32 status;
	json_t *value;
	json_t *val;
	uint32 index;
	uint32 type;
	value=json_object_get(json,"index");
	if(value)
	{
		if(JsonGetInteger(value,&index))
		{
			val=json_object_get(json,"type");
			if(JsonGetInteger(val,&type))
			{
				status=lig_matrix_card_factory(lighandle,index,(EM_MATRIX_IO_TYPE)type,em_matrix_kramer_mode);
				if(!status)
				{
					//SendCardIRQ(em_matrix_card_param_irq_reset);
					flag=1;
				}
				else
				{
					printf("The status is %d\n",status);
					strcpy(estr,"Card Factory Failed");
				}
			}
			else
			{
				strcpy(estr,"Error of type");
			}
		}
		else
		{
			strcpy(estr,"Error of Index");
		}
	}
	else
	{
		strcpy(estr,"not the key index");
	}
	return  flag;
}

uint8 SetSecurityStat(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	json_t *value;
	uint8 name[USERNAMELEN];
	uint8 password[PASSWORDLEN];
	uint8 pws[PASSWORDLEN];
	int32 Security;
	value=json_object_get(json,"username");
	if(value)
	{
		if(JsonGetString(value,name))
		{
			value=json_object_get(json,"password");
			if(value)
			{
				if(JsonGetString(value,password))
				{
					if(GetUserPassword(name,pws)>=0)
					{
						if(!strcmp(password,pws))
						{
							value=json_object_get(json,"Security");
							if(value)
							{
								if(JsonGetInteger(value,&Security))
								{
									//printf("The Security is %d\n",Security);
									//printf("The liguoauth.security is %d\n",liguoauth.security);
									if(Security!=liguoauth.security)
									{
										printf("no equal\n");
										liguoauth.security=Security;
										writesecurityfile();
										SendSysIRQ(em_lig_sys_param_web_security);
									}
									printf("The liguoauth.security is %d\n",liguoauth.security);
									flag=1;
								}
								else
								{
									strcpy(estr,"Security type error");
								}
								
							}
							else
							{
								strcpy(estr,"Get Security error");
							}
						}
						else
						{
							strcpy(estr,"Password error");
						}
					}
					else
					{
						strcpy(estr,"no this User");
					}
				}
				else
				{
					strcpy(estr,"password type error");
				}
			}
			else
			{
				strcpy(estr,"Get password error");
			}
		}
		else
		{
			strcpy(estr,"Username type error");
		}
	}
	else
	{
		strcpy(estr,"Get Username error");
	}
	return flag;
}
uint8 GetSecurityStat(char *data,char *estr)
{
	uint8 flag=0;
	json_t *json;
	json=json_object();
	if(json)
	{
		json_object_set(json,"securityStat",json_integer(liguoauth.security));
		JsonInfoSetting(&flag,data,json);
	}
	else
	{
		strcpy(estr,"Init Json Error");
	}
	return flag;
}
uint8 SetUserPassword(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	json_t *value;
	uint8 name[USERNAMELEN];
	uint8 password[PASSWORDLEN];
	uint8 pws[PASSWORDLEN];
	uint8 newpassword[PASSWORDLEN];
	value=json_object_get(json,"username");
	uint8 index=0;
	uint8 status=0;
	if(value)
	{
		if(JsonGetString(value,name))
		{
			value=json_object_get(json,"password");
			if(value)
			{
				if(JsonGetString(value,password))
				{
					index=GetUserPassword(name,pws);
					if(index>=0)
					{
						if(!strcmp(password,pws))
						{
							value=json_object_get(json,"newpassword");
							if(value)
							{
								if(!status)
								{
									if(JsonGetString(value,newpassword))
									{
										status=CheckPassword(newpassword);
										if(!status)
										{
											if(strcmp(password,newpassword))
											{
												strcpy(liguoauth.Auth[index].password,newpassword);
												writesecurityfile();
											}
											flag=1;
										}
										else
										{
											if(status==1)
											{
												strcpy(estr,"length error");
											}
											else if(status==2)
											{
												strcpy(estr,"have Illegal character");
											}
										}	
									}
									else
									{
										strcpy(estr,"Security type error");
									}
								}
							}
							else
							{
								strcpy(estr,"Get Security error");
							}
						}
						else
						{
							strcpy(estr,"Password error");
						}
					}
					else
					{
						strcpy(estr,"no this User");
					}
				}
				else
				{
					strcpy(estr,"password type error");
				}
			}
			else
			{
				strcpy(estr,"Get password error");
			}
		}
		else
		{
			strcpy(estr,"Username type error");
		}
	}
	else
	{
		strcpy(estr,"Get Username error");
	}
	return flag;
}


uint8 GetVoltageStatus(char *data,char *estr)
{
	uint8 flag=0;
	uint8 filepath[50]="/tmp/voltage_cfg.segment";
	json_t *info;
	uint8 jsonfile[READFILENUM];
	json_error_t error;
	if(JsonFromFile(filepath,jsonfile))
	{
		info=json_loads(jsonfile,0,&error);
		if(info)
		{
			JsonInfoSetting(&flag,data,info);
			json_decref(info);
		}
		else
		{
			strcpy(estr,"Get voltage data error");
		}
	}
	else
	{
		strcpy(estr,"Get file error,Please try again later.");
	}
	return flag;
}

uint8 GetTemperatureStatus(char *data,char *estr)
{
	uint8 flag=0;
	uint8 filepath[50]="/tmp/temp_cfg.segment";
	json_t *info;
	uint8 jsonfile[READFILENUM];
	json_error_t error;
	if(JsonFromFile(filepath,jsonfile))
	{
		info=json_loads(jsonfile,0,&error);
		if(info)
		{
			JsonInfoSetting(&flag,data,info);
			json_decref(info);
		}
		else
		{
			strcpy(estr,"Get temp data error");
		}
	}
	else
	{
		strcpy(estr,"Get file error,Please try again later.");
	}
	return flag;
}
uint8 GetFanStatus(char *data,char *estr)
{
	uint8 flag=0;
	uint8 filepath[50]="/tmp/fan_cfg.segment";
	json_t *info;
	uint8 jsonfile[READFILENUM];
	json_error_t error;
	if(JsonFromFile(filepath,jsonfile))
	{
		info=json_loads(jsonfile,0,&error);
		if(info)
		{
			JsonInfoSetting(&flag,data,info);
			json_decref(info);
			
		}
		else
		{
			strcpy(estr,"Get fan data error");
		}
	}
	else
	{
		strcpy(estr,"Get file error,Please try again later.");
	}
	return flag;
}

uint8 GetAlertStatus(char *data,char *estr)
{
	uint8 flag=0;
	uint8 filepath[50]="/tmp/alert_cfg.segment";
	json_t *info;
	uint8 jsonfile[READFILENUM];
	json_error_t error;
	if(JsonFromFile(filepath,jsonfile))
	{
		info=json_loads(jsonfile,0,&error);
		if(info)
		{
			JsonInfoSetting(&flag,data,info);
		}
		else
		{
			strcpy(estr,"Get alert data error");
		}
	}
	else
	{
		strcpy(estr,"Get file error,Please try again later.");
	}
	return flag;
}

uint8 UntarUpgradeFile(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	uint8 filename[1024];
	uint8 untarfilename[1024];
	json_t *untar;

	if(JsonGetString(json,filename))
	{
		FILE * fstream;
		printf("file name is %s\n",filename);
		//system("rm /te");
		sprintf(untarfilename,"unzip -o \"/tmp/%s\" -d /tmp  > /dev/null && ls -t /tmp/ | grep -i \".xml\"",filename);
		if(NULL==(fstream=popen(untarfilename,"r"))||NULL==fgets(untarfilename,sizeof(untarfilename), fstream))    
		{    
			strcpy(estr,"untar file failed");
			return flag;	
		}
		pclose(fstream);
		untarfilename[strlen(untarfilename)-1]=NULL;
		//struct stat file;
		sprintf(filename,"/tmp/%s",untarfilename);
		sprintf(data,"{\"FileName\":\"%s\"}",untarfilename);
		//printf("file name is %s\n",filename);
		//stat(filename,&file);
		sprintf(untarfilename,"mv %s /nandflash/webserver/thttpd/www/",filename);
		printf("The value is %s\n",untarfilename);
		system(untarfilename);
		flag=1;
		//printf("jifukui file size is %d\n",file.st_size);
		//sprintf(data,"{\"FileName\":\"%s\",\"FileSize\":\"%d\"}",untarfilename,file.st_size);

		//printf("The data is %s\n",data);
	}
	else
	{
		strcpy(estr,"Get File Name have a error");
	}
	return flag;
}

uint8 SetFirmwareUpgrade(json_t * json,char *data,char *estr)
{
	uint8 flag=0;
	uint8 filepath[1024]="/nandflash/config/lig_upload.upfile.infor";
	uint8 codepath[1024];
	uint8 upgradepath[256];
	uint8 filename[64];
	uint8 * index=NULL;
	FILE *fp=NULL;
	fp=fopen(filepath,"w+");
	JsonGetString(json,filename);
	printf("The fp is %d\n",fp);
	printf("The json is %s\n",filename);
	if(fp&&json&&JsonGetString(json,filename))
	{
		/*fclose(fp);
		index=strstr(filename,".kptw");
		if(!index)
		{
			sprintf(codepath,"/nandflash/bin/lig_upload  /nandflash/webserver/thttpd/www/cgi-bin/\"%s\" %d",filename,1);
		}
		else
		{
			FILE * fstream;
			sprintf(upgradepath,"unzip -o /nandflash/webserver/thttpd/www/cgi-bin/%s -d /tmp  > /dev/null && ls /tmp/ | grep \"kptw\"",filename);
			if(NULL==(fstream=popen(upgradepath,"r"))||NULL==fgets(upgradepath,sizeof(upgradepath), fstream))    
			{    
				strcpy(estr,"file or error");
				//printf("have error\n");
				return 0;	
			}
			pclose(fstream);
			upgradepath[strlen(upgradepath)-1]=NULL;
			sprintf(codepath,"/nandflash/bin/lig_upload  /tmp/\"%s\" %d",upgradepath,1);
			//printf("codepath is %s\n",codepath);
		}
		index=NULL;
		//printf("The filename is %s\n",filename);
		*/
		sprintf(codepath,"/nandflash/bin/lig_upload  /tmp/\"%s\" %d",filename,1);
#if DEBUG
		printf("The code is %s\n",codepath);
#endif
		pid_t status=0;
		status=system(codepath);
		printf("end of upgrade file\n");
		//printf("The one status is %s\n",status);
#if DEBUG
		printf("The status is %s\n",status);
#endif
		if(0==status)
		{
			flag=1;
		}
		else 
		{
			strcpy(estr,"upgrade error");	
		}		
	}
	else
	{
		strcpy(estr,"file or error");
	}
	return flag;
}

uint8 SetMoreFirmwareUpgrade(json_t * json,char *data,char *estr)
{
	uint8 flag=0;
	uint8 filepath[512]="/nandflash/config/lig_upload.upfile.infor";
	uint8 oldpath[512];
	uint8 newpath[512];
	uint8 codepath[512];
	uint8 upgradepath[256];
	uint8 oldfilename[64];
	uint8 newfilename[64];
	uint32 Cleanflag;
	uint8 * index=NULL;
	FILE *fp=NULL;
	fp=fopen(filepath,"w+");
	uint32 jifile;
	json_t *filedata1;
	json_t *filedata2;
	json_t *filedata3;
	filedata1=json_object_get(json,"oldfile");
	/*
	if(filedata1)
	{
		printf("oldname good\n");
	}
	else
	{
		printf("oldname bad\n");
	}*/
	filedata2=json_object_get(json,"newfile");
	/*
	if(filedata2)
	{
		printf("newname good\n");
	}
	else
	{
		printf("newname bad\n");
	}*/
	filedata3=json_object_get(json,"End");
	/*
	if(filedata3)
	{
		printf("end good\n");
	}
	else
	{
		printf("end bad\n");
	}*/
	if(fp&&json&&JsonGetString(filedata1,oldfilename)&&JsonGetString(filedata2,newfilename)&&JsonGetInteger(filedata3,&Cleanflag))
	{
		fclose(fp);
		sprintf(oldpath,"/tmp/%s",oldfilename);
		sprintf(newpath,"/tmp/%s",newfilename);
		/*printf("The oldpath is %s\n",oldpath);
		printf("The newpath is %s\n",newpath);*/
		jifile=rename(oldpath,newpath);
		if(!jifile)
		{
			if(Cleanflag==1)
			{
				sprintf(codepath,"/nandflash/bin/lig_upload  /tmp/\"%s\" %d",newfilename,1);
			}
			else
			{
				sprintf(codepath,"/nandflash/bin/lig_upload  /tmp/\"%s\" %d",newfilename,0);
			}		
			pid_t status=0;
			status=system(codepath);
			//printf("The status is %s\n",status);
			if(0==status)
			{
				flag=1;
			}
			else 
			{
				strcpy(estr,"upgrade error");	
			}
		}
		else
		{
			strcpy(estr,"rename filename error");
		}
		//printf("The rename code is %d\n",jifile);
	}
	else
	{
		strcpy(estr,"file or error");
	}
	return flag;
}


uint8 GetUpgradeStatus(char * data,char *estr)
{
	uint8 flag=0;
	/*LIG_MATRIX_OBJ_SIGNAL_FLAG * param;
	int32 sys_param=0;
	int32 sys_value=0;
	json_t *json;
	json=json_object();
	lig_matrix_get_inner_signal_information(lighandle,&param);
	sys_param = lig_sys_get_param_flag_from_inner(param->src_parameter);
	sys_value = lig_sys_get_param_value_from_inner(param->src_parameter);
	if(json)
	{
		switch (sys_param)
		{	
			case em_lig_sys_param_upload_process:
			{

			}
			default:
			{

			}
		}
	}
	else
	{
		strcpy(estr,"json init error");
	}*/
	return flag;
}

uint8 GetSupportGroupPort(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	uint32 porttype;
	json_t *result,*info;
	result=json_array();
	if(JsonGetInteger(json,&porttype))
	{
		if(result)
		{
			uint32 dat[GROUPPORT],len;
			int32 status;
			status=lig_matrix_get_support_group_enable_list(lighandle,(EM_MATRIX_PORT_TYPE)porttype,dat,(int32)GROUPPORT);
			if(status>=0)
			{
				len=0;
				while(len<status)
				{
					json_array_append(result,json_integer(dat[len]));
					len++;
				}
				JsonInfoSetting(&flag,data,result);
			}
			else
			{
				sprintf(estr,"Get support group port error is %d",status);
			}
		}
		else
		{
			strcpy(estr,"Great Json failed");
		}
	}
	else
	{
		strcpy(estr,"Get Port type error");
	}
	return flag;
}
uint8 GetGroupPragram(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	json_t *result,*info;
	result=json_array();
	uint32 porttype,groupid;
	if(result)
	{
		info=json_object_get(json,"porttype");
		if(JsonGetInteger(info,&porttype))
		{
			info=json_object_get(json,"groupid");
			if(JsonGetInteger(info,&groupid))
			{
				uint32 dat[256],len;
				int32 status;
				status=lig_matrix_get_switch_group(lighandle,(EM_MATRIX_PORT_TYPE)porttype,groupid,dat,(int32)GROUPPORT);
				if(status>=0)
				{
					len=0;
					while(len<status)
					{
						json_array_append(result,json_integer(dat[len]));
						len++;
					}
					JsonInfoSetting(&flag,data,result);
			}
			else
			{
				sprintf(estr,"Get support group port error is %d",status);
			}
			}
			else
			{
				strcpy(estr,"Get Group id error");
			}
			
		}
		else
		{
			strcpy(estr,"Get Port type error");
		}
	}
	else
	{
		strcpy(estr,"Great Json failed");
	}
	return flag;
}
uint8 SetGroupPragram(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	json_t *result,*info,*jport;
	result=json_array();
	uint32 porttype,groupid;
	if(result)
	{
		info=json_object_get(json,"porttype");
		if(JsonGetInteger(info,&porttype))
		{
			info=json_object_get(json,"groupid");
			if(JsonGetInteger(info,&groupid))
			{
				info=json_object_get(json,"port");
				if(JSON_ARRAY==json_typeof(info))
				{
					uint32 dat[GROUPPORT];
					int32 len,i=0,j=0;
					int32 status;
					len=json_array_size(info);
					for(i;i<len;i++)
					{
						jport=json_array_get(info,i);
						if(JsonGetInteger(jport,&dat[j]))
						{
							j++;
						}
					}
					status=lig_matrix_set_switch_group(lighandle,(EM_MATRIX_PORT_TYPE)porttype,groupid,dat,j);
					if(0==status)
					{
						flag=1;
					}
				}
				else
				{
					strcpy(estr,"port type must be array");
				}
			}
			else
			{
				strcpy(estr,"Get Group id error");
			}
			
		}
		else
		{
			strcpy(estr,"Get Port type error");
		}
	}
	else
	{
		strcpy(estr,"Great Json failed");
	}
	return flag;
}

uint8 JsonGetString(json_t *json,char *data)
{
	uint8 flag=0;
	if(json)
	{
		if(json_typeof(json)==JSON_STRING)
		{
			strcpy(data,json_string_value(json));
			flag=1;
		}
#if DEBUG
		else
		{
			printf("this is not string is %d\n",json_typeof(json));
		}
#endif	
	}
#if DEBUG
	else
	{
		printf("this not json\n");
	}
#endif
	return flag;
}
uint8 JsonGetInteger(json_t *json,uint32 *data)
{
	uint8 flag=0;
	if(json)
	{
		if(json_typeof(json)==JSON_INTEGER)
		{
			*data=json_integer_value(json);
			flag=1;
		}
#if DEBUG
		else
		{
			printf("Not integer %d\n",json_typeof(json));
		}
#endif 
	}
#if DEBUG
	else
	{
		printf("not the json\n");
	}
#endif
	return flag;
}
uint8 JsonGetUint8(json_t *json,uint8 *data)
{
	uint8 flag=0;
	uint32 val;
	if(json)
	{
		if(json_typeof(json)==JSON_INTEGER)
		{
			val=json_integer_value(json);
			*data=val%256;
			flag=1;
		}

	}
	return flag;
}
uint8 DeviceVideoRouting(uint32 in,uint32 out)
{
	uint32 value;
	uint8 flag=1;	
	value=ex_lig_matrix_stash_video_switch_data(lighandle,in,out);
#if DEBUG
	printf("Video Pre switch value is %d in is %d out is %d\n",value,in,out);
#endif 
	if(!value)
	{
		flag=1;
	}
	return flag;
}
uint8 DeviceAudioRouting(uint32 in,uint32 out)
{
	uint32 value;
	uint8 flag=1;
	value=lig_matrix_stash_audio_switch_data(lighandle,in,out);
#if DEBUG
	printf("Audio Pre switch value is %d in is %d out is %d\n",value,in,out);
#endif 
	if(!value)
	{
		flag=1;
	}
	return flag;
}
uint8 JsonFromFile(uint8 *filepath,uint8 *data)
{
	uint8 flag=0;
	FILE *file=NULL;
	file=fopen(filepath,"r");
	if(file)
	{
		uint32 i;
		for(i=0;i<READFILENUM-1;i++)
		{
			*(data+i)=fgetc(file);
			
			if(0==*(data+i))
			{
				break;
			}
		}
		flag=1;
	}
	return flag;
}

int8 GetUserPassword(uint8 *user,uint8 *psw)
{
	psw[0]=0;
	uint8 i=0;
	uint8 flag=-1;
	uint8 str[PASSWORDLEN];
	while (i<AUTH_NUM&&liguoauth.Auth[i].username[0])
	{
		strcpy(str,liguoauth.Auth[i].username);
		if(strcmp(str,user))
		{

		}
		else
		{
			strcpy(psw,liguoauth.Auth[i].password);
			flag=i;
			return flag;
		}			
		i++;
	}
	return flag;
}

uint8 SetDHCPState(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	uint32 state=0;
	uint8 str[128];
	int status=0;
	if(JsonGetInteger(json,&state))
	{
		state%=2;
		sprintf(str,"printf \"#net-DHCP %d\r\" >> /tmp/lig_com_in  /",state);
		status=Mysystem(str);
		if(!status)
		{
			flag=1;
		}
	}
	else
	{
		strcpy(estr,"Error of json type");
	}
	return flag;
}
uint8 SetDNSName(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	uint8 name[128];
	uint8 str[256];
	int status=0;
	printf("Hello this is dns\n");
	if(JsonGetString(json,name))
	{
		//printf("The name is %s\n",name);
		sprintf(str,"printf \"#name %s\r\" > /tmp/lig_com_in",name);
		//printf("%s\n",str);
		status=Mysystem(str);
		if(!status)
		{
			flag=1;
		}
	}
	else
	{
		printf("have error\n");
		strcpy(estr,"Error of json type");
	}
	return flag;
}
uint8 UPgreadJsonFile(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	uint8 filename[128];
	uint8 str[256];
	int status=0;
	if(JsonGetString(json,filename))
	{
		sprintf(str,"lig_matrix_config_json -R /tmp/%s",filename);
		status=Mysystem(str);
		if(!status)
		{
			flag=1;
		}
		else
		{
			strcpy(estr,"File content error");
		}
		
	}
	else
	{
		strcpy(estr,"Error of json type");
	}
	return flag;
}
uint8 GetStaticNetWork(char *data,char *estr)
{
	uint8 flag=0;
	int8 mac[48];
	int8 ip[24];
	int8 mask[24];
	int8 gateway[24];
	FILE * fstream;
	int8 str[512];
	int32 status;
	memset(str,0,sizeof(str));
    memset(mac,0,sizeof(mac));
    memset(ip,0,sizeof(ip));
    memset(mask,0,sizeof(mask));
    memset(gateway,0,sizeof(gateway));
	json_t *ethnet=json_object();
	json_t *info=json_object();
	if(ethnet&&info)
	{
		strcpy(str,"lig_ifcfg eth0 LASTIP 0 |awk '{print $1,$3}'");
		if(NULL==(fstream=popen(str,"r"))||NULL==fread(str,1,sizeof(str),fstream))
		{
			printf("Have error\n");
		}
		pclose(fstream);
		str[strlen(str)-1]=NULL;
		status=sscanf(str,"MAC %s\nIP %s\nMASK %s\nGATEWAY %s\n",mac,ip,mask,gateway);
		if(4==status)
		{
			json_object_set_new(ethnet,"IP",json_string(ip));
			json_object_set_new(ethnet,"MASK",json_string(mask));
			json_object_set_new(ethnet,"GATEWAY",json_string(gateway));
			json_object_set_new(info,"StaticNetwork",ethnet);
			JsonInfoSetting(&flag,data,info);
		}
		else
		{
			strcpy(estr,"Get network failed");
		}
	}
	else
	{
		strcpy(estr,"creat json obejct failed");
	}
	return flag;
}
uint8 GetSerialPaud(json_t *json,char *data,char *estr)
{
	int32 value=0,slot=0;
	uint8 flag=0;
	json_t *paud=json_object();
	json_t *info=json_object();
	if(info&&paud)
	{
		if(JsonGetInteger(json,&slot))
		{
			value=lig_matrix_get_card_uart_maxspeed(lighandle,slot);
			json_object_set(paud,"paud",json_integer(value));
			json_object_set(info,"Uartinfo",paud);
			JsonInfoSetting(&flag,data,info);
		}
		else
		{
			strcpy(estr,"slot id get error");
		}
	}
	else
	{
		strcpy(estr,JI_JsonCreatFailed);
	}
	return flag;

}
uint8 Get48VStatus(char *data,char *estr)
{
	uint8 flag=0;
	int32 status=0;
	json_t *value=json_object();
	if(value)
	{
		status=lig_matrix_get_power_48V(lighandle);
		printf("The status is %d\n",status);
		if(status>=0)
		{
			if(status==0)
			{
				json_object_set(value,"48VStatus",json_string("OFF"));
			}
			else if(status==1)
			{
				json_object_set(value,"48VStatus",json_string("ON"));
			}
			else
			{
				json_object_set(value,"48VStatus",json_string("Other"));
			}
			flag=1;
			char *str;
			str=json_dumps(value,JSON_PRESERVE_ORDER);
			strcpy(data,str);
			free(str);
			if(str!=NULL)
			{
				str=NULL;
			}
		}
		else
		{
			strcpy(estr,"Get 48V infomation failed");
		}
	}
	else
	{
		strcpy(estr,"Creat json file error");
	}
	return flag;
}
uint8 Set48VStatus(json_t *json,char *data,char *estr)
{
	uint8 flag=0;
	uint8 value=0;
	int32 status=0;
	if(json)
	{
		if(JsonGetUint8(json,&value))
		{
			printf("The data is %d\n",value);
			status=lig_matrix_set_power_48V(lighandle,value);
			printf("The status is %d\n",status);
			if(status>=0)
			{
				flag=1;
			}
			else
			{
				strcpy(estr,"Set 48v status error");
			}
		}
		else
		{
			strcpy(estr,"Get json data failed");
		}
	}
	else
	{
		strcpy(estr,"json Object  error");
	}
	return flag;
}
uint8 SendSysIRQ(EM_LIG_SYS_PARAM sysparam)
{
	uint8 flag=0;
	uint32 status;
	LIG_MATRIX_OBJ_SIGNAL_FLAG sig_info;
	sig_info.sig_bit_map=LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE;
	sig_info.src_parameter=lig_sys_get_inner_param(sysparam);
	status=lig_matrix_set_inner_signal_flag(lighandle,& sig_info);
	if(!status)
	{
		//printf("Good for this \n");
		flag=1;
	}
	else
	{
		//printf("Error for this is %d\n",status);
	}
	return flag;
}
uint8 SendCardIRQ(EM_MATRIX_CARD_PARAM_IRQ_TYPE sysparam)
{
	LIG_MATRIX_OBJ_SIGNAL_FLAG sig_info;
	sig_info.sig_bit_map=LIG_MATRIX_MEVENT_CARD_PARAM_CHANGE;
	sig_info.src_parameter=lig_sys_get_inner_param(sysparam);
	lig_matrix_set_inner_signal_flag(lighandle,& sig_info);
	return 1;
}

void writesecurityfile()
{
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
	}	
}

uint8 CheckPassword(uint8 *password)
{
	uint8 flag=0;
	uint8 i=0;
	if(strlen(password)>0&&strlen(password)<=16)
	{
		
		for(i;i<strlen(password);i++)
		{
			if(!isalnum(password[i]))
			{
				break;
			}
		}
		if(i!=strlen(password))
		{
			flag=2;
		}
	}
	else
	{
		flag=1;
	}
	return flag ;
}