#include <stdio.h>
#include <string.h>
#include <jansson.h>
char uploadfilename[64];
int contentlen;
int codelength;
void getstartcode();
int getfilename();
void getfiletype();
int getfilecontent();
void getendcode();
int getdata(char *data);
int main()
{
	int flag=0;
	char *val=NULL;
	json_t *obj;
	obj=json_object();
	val=getenv("CONTENT_LENGTH");
	printf("Content-type: application/json; charset=utf-8\r\n");
	printf("Cache-Control: no-cache\r\n");
	printf("Accept-Rangs:bytes\r\n");
	printf("Connection:close\r\n");
	printf("\r\n");
	if(val!=NULL&&obj)
	{
		contentlen=atoi(val);
//		printf("get length\n");
		json_object_set_new(obj,"cmd",json_string("UploadFile"));
		json_object_set_new(obj,"status",json_string("ERROR"));
		json_object_set_new(obj,"error",json_null());
		getstartcode();
//		printf("start\n");
		if(!getfilename())
		{
			getfiletype();
			int code=getfilecontent();
	        if(!code)
        	{
				getendcode();
				flag=1;
            }
            else
            {
				json_object_set(obj,"error",json_string("write file error"));
				json_object_set_new(obj,"code",json_integer(code));
            }
                	
		}
		else
		{
			json_object_set(obj,"error",json_string("get file name error"));
		}
	}
	else
	{
		printf("{\"cmd\":\"UploadFile\",\"status\":\"ERROR\",\"error\":\"init error\"}");		
	}

	if(flag)
	{
		json_object_set(obj,"status",json_string("SUCCESS"));
	}
	char *str=NULL;
	str=json_dumps(obj,JSON_PRESERVE_ORDER);
	printf(str);
	free(str);
	if(str)
	{
		str=NULL;
	}
	printf("\r\n");
	return 0;
}
void getstartcode()
{
	char val[100];	
	codelength=getdata(val);
	codelength+=2;
}
int getfilename()
{
	int len=0;
	char *name=NULL;
	char filename[256];
	len=getdata(filename);
	filename[len-3]=0;
	name=strstr(filename,"filename");
	if(name)
	{
		strcpy(uploadfilename,name+10);
		return 0;
	}
	else
	{
		return 1;
	}
}
void getfiletype()
{
	char filetype[256];
	getdata(filetype);
	getchar();
	getchar();
	contentlen-=2;
}
int getfilecontent()
{
	static int filesize=1024;
	int i;
	int nums;
  	ssize_t num;
	int length=0;
	int readlength=0;
	char data[filesize];
	char path[256];
	length=contentlen-codelength-2;
	nums=length/filesize;
	FILE *fp=NULL;
	int n;
	sprintf(path,"/tmp/www/%s",uploadfilename);
	fp=fopen(path,"wb+");
	if(fp)
	{
		//printf("length is %d",length);

		for(i=0;i<nums;i++)
		{
			for(n=0;n<filesize;)
			{
				data[n]=getchar();
				//if(read(stdin,&data,1))
				{
					n++;
				}
				//else
				{
				//	usleep(10);
				}
			}

			//read(stdin,&data,filesize);
			if(filesize==fwrite(data,1,filesize,fp))
		//	if(1)
			{
				readlength+=filesize;
		//		printf("index is %d filesize is %d\n",i,filesize);
			}
			else
			{
				return 1;
			}
	//		usleep(100);
		}
		if(readlength<length)
		{
		//	printf("have anoher");
			i=length-readlength;
			//read(stdin,&data,i);
			for(n=0;n<i;)
			{
				data[n]=getchar();
			//	if(read(stdin,&data,1))
				{
					n++;
				}
			//	else
				{
			//		usleep(10);
				}
			}
			if(i==fwrite(data,1,i,fp))
			//if(1)
			{		
				readlength+=i;
			}
			else
			{
				return 1;
			}
		}
		getchar();
		getchar();
		if(readlength==length)
		{
			return 0;
		}
	}
//	printf("HAVe sin e ");
	return 1;
}
void getendcode()
{
	char data[100];
	getdata(data);
}
int getdata(char *data)
{
	int flag=0;
	int n=0;
	char ch;
	while(contentlen>0)
    {
        ch=getchar();
    	data[n]=ch;
        n++;
		contentlen--;
        if(flag)
        {
            flag=0;
            if(ch==10)
            {
                data[n]=0;
                return n;
            }
        }
        else
        {
            if(ch==13)
            {
                flag=1;		
            }
        }
    }
	return 0;
}
