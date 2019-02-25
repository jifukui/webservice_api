/******************************************************************************
 LIG
 ******************************************************************************
  File Name     : lig_pid_file.h        pid_file=>pidf

  Version       : Initial Draft
  Author        : zhangbin
  Created       : Friday, February 17, 2017
  Last Modified :
  Description   :
  Function List :

  History       :
  1.Date        : Friday, February 17, 2017
    Author      : zhangbin
    Modification: Created file

******************************************************************************/
#ifndef __LIG_PID_FILE_H__
#define __LIG_PID_FILE_H__

#ifdef __cplusplus    //
extern "C" {          //
#endif
//============include=================================

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>


//=============EXTERN define==========================
#define LIG_PID_FILE_PATH   "/var/run/"



//=============EXTERN FUNC============================

/*******************************
-Function Name : lig_pidf_open
-Description   : 打开pid文件,文件路径在LIG_PID_FILE_PATH,文件名为[programname].pid
		 如果依赖文件被锁定,函数会阻塞
		 注: [self].pid文件中记录了创建者的pid值,二进制

-Input  Param  : dependproc:依赖的程序名字,NULL为无依赖,不等待

-Return Value  : 返回自身pid文件的fd,-1=错误,详细错误在日志中
*********************************/
extern int lig_pidf_open(const char *dependproc);


/*******************************
-Function Name : lig_pidf_close
-Description   : 关闭自身的pid文件,并解除锁定

-Input  Param  : fd:自身的pid文件fd

-Return Value  :
*********************************/
extern void lig_pidf_close(int fd);


#ifdef __cplusplus
}
#endif

#endif//endfile __LIG_PID_FILE_H__


