/******************************************************************************
 LIG HEAD FILE
 ******************************************************************************
  File Name     : lig_inner_sig_system_param_change.h
  Version       : Initial Draft
  Author        : zhangbin
  Created       : 2018/10/12
  Last Modified :
  Description   : 定义有关LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE信号的各个参数值
                  及判断,涉及的相关的lig_matrix.so的接口有:
                  lig_matrix_set_inner_signal_flag 
  Function List :

  History       :
  1.Date        : 2018/10/12
    Author      : zhangbin
    Modification: Created file
  2.Date	: 2018/12/18
    Author	: zouyuan
    Modification: add upload define
**************************************************************************/
#ifndef __LIG_INNER_SIG_SYSTEM_PARAM_CHANGE_H__
#define __LIG_INNER_SIG_SYSTEM_PARAM_CHANGE_H__
#ifdef __cplusplus
extern "C" {
#endif

/*=============include================================*/
#include "lig_types.h"

/*=============EXTERN ENUM============================*/

//添加 内部信号,系统参数修改的相关宏定义
//1. eth0 mac/ip/mask/gateway/tcp port/udp port 修改
//2. eth1 mac/ip/mask/gateway/tcp port/udp port 修改
//3. model name修改
//4. name 修改
//5. dhcp修改
//6. sn修改
//7. BAUD修改
//8. 切换宏的修改
//9. 默认切换端口设置
//10. 升级过程信息更新

//param_32bit
//31~16bit: bit31~24 = macro_id或设置参数值, bit23~16 = param_flag
//15~0bit: pid
typedef enum
{
	em_lig_sys_param_lock_fp=0,
	/*eth0*/
	em_lig_sys_param_eth0,             //eth0所有参数
	em_lig_sys_param_eth0mac,
	em_lig_sys_param_eth0ip,
	em_lig_sys_param_eth0mask,
	em_lig_sys_param_eth0gateway,
	em_lig_sys_param_eth0tcpport,
	em_lig_sys_param_eth0udpport,
	em_lig_sys_param_dhcp,
	/*eth1*/
	em_lig_sys_param_eth1,             //eth1所有参数
	em_lig_sys_param_eth1mac,
	em_lig_sys_param_eth1ip,
	em_lig_sys_param_eth1mask,
	em_lig_sys_param_eth1gateway,
	em_lig_sys_param_eth1tcpport,       //预留
	em_lig_sys_param_eth1udpport,       //预留
	em_lig_sys_param_eth1dhcp,          //预留
	/*matrix infor*/
	em_lig_sys_param_model_name,
	em_lig_sys_param_name,
	em_lig_sys_param_sn,
	em_lig_sys_param_baud,
	/*macro*/
	em_lig_sys_param_sw_all_macro,      //所有宏
	em_lig_sys_param_sw_audio_macro,
	em_lig_sys_param_sw_video_macro,
	em_lig_sys_param_sw_default_in,     //切换默认输入端口
	/*upload
	bit31~24 = 0 - over;
	           1 - start;
	           2 - updata process message (/tmp/lig_upload.proc.infor); 
	           3 - error exit; 
	           4 - over and need reboot;
	           5 - reboot immediately;
        */
	em_lig_sys_param_upload_process,    
}EM_LIG_SYS_PARAM;


/*=============EXTERN DEF=============================*/
/*******************************
-Define Name   : lig_sys_get_inner_param
-Description   : 获取发送内部信号LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE时,信号的参数
-Input	Param  : param_enum from in EM_LIG_SYS_PARAM
-Author        : zhangbin.eos@foxmail.com 2018/10/12
*********************************/
#define lig_sys_get_inner_param(param_enum)	\
	((param_enum&0x0000ffff)<<16|(getpid()&0xffff))


/*******************************
-Define Name   : lig_sys_get_param_from_inner
-Description   : 获取内部信号LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE参数中的系统参数
-Input	Param  : inner_param from in (struct  sig_flag).src_parameter
-Author        : zhangbin.eos@foxmail.com 2018/10/12
*********************************/
#define lig_sys_get_param_from_inner(inner_param)	\
	((inner_param&0xffff0000)>>16)

/*******************************
-Define Name   : lig_sys_get_param_flag_from_inner
-Description   : 获取内部信号LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE参数中的系统参数标记
-Input	Param  : inner_param from in (struct  sig_flag).src_parameter
-Author        : zouyuan@liguodz.com 2018/10/30
*********************************/
#define lig_sys_get_param_flag_from_inner(inner_param)	\
	((inner_param&0x00ff0000)>>16)

/*******************************
-Define Name   : lig_sys_get_param_from_inner
-Description   : 获取内部信号LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE参数中的系统参数值
-Input	Param  : inner_param from in (struct  sig_flag).src_parameter
-Author        : zouyuan@liguodz.com 2018/10/30
*********************************/
#define lig_sys_get_param_value_from_inner(inner_param)	\
	((inner_param&0xff000000)>>24)

/*******************************
-Define Name   : lig_sys_get_pid_from_inner
-Description   : 获取内部信号LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE参数中的pid
-Input	Param  : inner_param from in (struct  sig_flag).src_parameter
-Author        : zhangbin.eos@foxmail.com 2018/10/12
*********************************/
#define lig_sys_get_pid_from_inner(inner_param)	\
	(inner_param&0x0000ffff)

/*
 使用流程
 1. 发送信号
 	LIG_MATRIX_OBJ_SIGNAL_FLAG sig_info;
 	sig_info.sig_bit_map=LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE;
 	sig_info.src_parameter = lig_sys_get_inner_param(em_lig_sys_param_eth0ip); 
 	sig_info.thread_id = 0;
 	lig_matrix_set_inner_signal_flag(handle,&sig_info);

 2. 接受信号
 	
 	LIG_MATRIX_OBJ_SIGNAL_FLAG sig_info;
 	lig_matrix_get_inner_signal_information(handle,&sig_info);
 	if(sig_info.sig_bit_map==LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE)
 	{
 		//发送进程的PID
 		pid=lig_sys_get_pid_from_inner(sig_info.src_parameter);
 		//系统参数枚举值
 		em_param = lig_sys_get_param_from_inner(sig_info.src_parameter);
 	}
*/


#ifdef __cplusplus
}
#endif

#endif //__LIG_INNER_SIG_SYSTEM_PARAM_CHANGE_H__

