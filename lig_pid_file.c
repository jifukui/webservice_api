/******************************************************************************
 LIG
 ******************************************************************************
  File Name     : lig_pid_file.c
  Version       : Initial Draft
  Author        : zhangbin
  Created       : Friday, February 17, 2017
  Last Modified :
  Description   : 创建进程文件
  Function List :

  History       :
  1.Date        :
    Author      : zhangbin
    Modification: Created file

*****************************************************************************/
//============include=========================

#include "lig_pid_file.h"

#include <stdio.h>

//int
#include <stdint.h>

//string strlen
#include <string.h>

//exit
#include <stdlib.h>

#include "lig_types.h"




//============STATIC define=================================
#if LIG_PID_FILE_DEBUG_EN
//pidf<==pid_file
#define pr_pidf_debug(fmt,args...)           pr_debug(fmt, ##args)
#else
#define pr_pidf_debug(fmt,args...)               /* do nothing */
#endif



//=============STATIC ENUM===================================


//=============STATIC STRUCT ================================

//=============STATIC variable========================


//=============EXTERN variable========================

//=============STATIC FUNC DEFINE=====================


int lig_pidf_lock(int fd, int nbflag);


int lig_pidf_unlock(int fd, int nbflag);


int lig_pidf_get_lock(int fd, pid_t *pid);



/*******************************
-Function Name : lig_pidf_wait_depend
-Description   : 等待依赖文件解锁/可用
-Call          :
-Called By     :
-Input  Param  :
-Output Param  :
-Return Value  :
*********************************/
int lig_pidf_wait_depend(const char *dependproc);

//=============EXTERN FUNC============================

int lig_pidf_open(const char *dependproc)
{
        char path[64];
        int fd = 0;
        int pid = 0;
        snprintf(path, sizeof(path), "%s%s.pid", LIG_PID_FILE_PATH,
                program_invocation_short_name);
        fd = open(path, O_RDWR | O_CREAT );

        if (fd == -1)
        {
                pr_log(LOG_ERR, "open %s,err:%s", path, strerror(errno));
                return -1;
        }

        /*文件加锁*/
        lig_pidf_lock(fd, 0);

        if(dependproc != NULL)
        {
                lig_pidf_wait_depend(dependproc);
        }

        /*清空文件*/
        if(ftruncate(fd, 0) != 0)
        {
                pr_log(LOG_ERR, "ftruncate err:%s", strerror(errno));
                return -1;
        }

        pid = (int)getpid();

        if( write(fd, (char * )(&pid), sizeof(pid)) != (int)sizeof(pid) )
        {
                pr_log(LOG_ERR, "ftruncate err:%s", strerror(errno));
                return -1;
        }

        return fd;
}

void lig_pidf_close(int fd)
{
        //char path[64];
        lig_pidf_unlock(fd, 0);
        close(fd);
}



//=============STATIC FUNC============================
int lig_pidf_lock(int fd, int nbflag)
{
        struct flock flockstr;

        flockstr.l_type		= F_WRLCK;
        flockstr.l_whence	= SEEK_SET;
        flockstr.l_start	= 0;
        flockstr.l_len		= 0;

        if(fcntl(fd, F_SETLKW, &flockstr) == 0)
        {
                return 0;
        }

        pr_log(LOG_ERR, "lock err:%s", strerror(errno));
        return -1;
}

int lig_pidf_unlock(int fd, int nbflag)
{
        struct flock flockstr;
        flockstr.l_type		= F_UNLCK;
        flockstr.l_whence	= SEEK_SET;
        flockstr.l_start	= 0;
        flockstr.l_len		= 0;

        if(fcntl(fd, F_SETLKW, &flockstr) == 0)
        {
                return 0;
        }

        pr_log(LOG_ERR, "lock err:%s", strerror(errno));
        return -1;
}

int lig_pidf_get_lock(int fd, pid_t *pid)
{
        struct flock flockstr;
        flockstr.l_type		= F_WRLCK;
        flockstr.l_whence	= SEEK_SET;
        flockstr.l_start	= 0;
        flockstr.l_len		= 0;

        if(fcntl(fd, F_GETLK, &flockstr) == 0)
        {
                if(pid != NULL)
                {
                        *pid = flockstr.l_pid;
                }

                return flockstr.l_type == F_UNLCK ? 0 : 1;
        }

        pr_log(LOG_ERR, "fcntl F_GETLK:%s", strerror(errno));
        return -1;
}

/*******************************
-Function Name : lig_pidf_wait_depend
-Description   : 等待依赖文件解锁/可用
-Call          :
-Called By     :
-Input  Param  :
-Output Param  :
-Return Value  :
*********************************/
int lig_pidf_wait_depend(const char *dependproc)
{
        int fd = 0;
        pid_t pid = 0;
        int res;
        char path[64];

        if(dependproc == NULL)
        {
                return -1;
        }

        snprintf(path, sizeof(path), "%s%s.pid", LIG_PID_FILE_PATH, dependproc);
        fd = open(path, O_RDWR);

        if (fd == -1)
        {
                pr_log(LOG_ERR, "open depend %s,err:%s", path, strerror(errno));
                return -1;
        }
        res = lig_pidf_get_lock(fd, &pid);
        
	pr_log(LOG_NOTICE, "get lock stat %d,pid=%d",res,pid);
	
        lig_pidf_lock(fd, 0);

        lig_pidf_unlock(fd, 0);

        return 0;

}


