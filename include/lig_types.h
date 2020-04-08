/*
 * lig_types.h
 *
 *  Created on: 2017-07-18
 *      Author: zouyuan
 */

#ifndef LIG_TYPES_H_
#define LIG_TYPES_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <syslog.h>

#define STATIC      static
#define INLINE      __inline
#define CONSTANT    const
#define EXTERNAL    extern
#define VOLATILE    volatile

/*=======================================
 * Data types
 *======================================*/
#ifndef BYTE
	#define BYTE			uint8_t
#endif

#ifndef WORD
	#define WORD			uint16_t
#endif

#ifndef DWORD
	#define DWORD			uint32_t
#endif

#ifndef BOOL
	#define BOOL         uint8_t
#endif

#ifndef NULL
	#define NULL         0 //    ((void *)0)
#endif

/* Emulate C99/C++ bool type    */
#ifdef __cplusplus
	typedef bool bool_t;
#else
	typedef enum
	{
		false   = 0,
		true    = !(false)
	} bool_t;
#endif // __cplusplus

#ifndef ON
	#define ON  1
#endif

#ifndef OFF
	#define OFF 0
#endif

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#define ABS_FUN(A, B) ((A>B)?(A-B):(B-A))

/*=======================================
 * ERROR CODE
 *======================================*/
typedef enum LIG_ERROR
{
	em_success = 0,
	em_err_unknow = -1,
	em_err_inparam = -2,					//input parameter for function is error
	em_err_obj_invalid = -3,
	em_err_open = -4,
	em_err_write = -5,
	em_err_read = -6,
	em_err_inprtl_format = -7,
	em_err_inprtl_timeout = -8,
	em_err_inprtl_dateflow = -9,
	em_err_inprtl_com_nosup= -10,
	em_err_inprtl_com_senddata= -14, //发送数据错，命令返回F2
	em_err_inprtl_com_bufout= -15, //子板缓冲溢出，命令返回F4
	em_err_inprtl_com_crc= -16, //冗余错误，命令返回F5
	em_err_inprtl_com_length= -17, //命令长度错误，命令返回F7
	em_err_inprtl_com_sendtimeout= -18, //命令发送超时，命令返回F8
	em_err_inprtl_com_unboot = -19, //不能进入BOOT模式
}LIG_ENUM_ERROR_CODE;

extern char *program_invocation_name;
extern char *program_invocation_short_name;
extern char *__progname;

#define LIG_MTRX_LOG_EMERG      0       /* system is unusable */
#define	LIG_MTRX_LOG_ALERT      1       /* action must be taken immediately */
#define	LIG_MTRX_LOG_CRIT       2       /* critical conditions */
#define	LIG_MTRX_LOG_ERR        3       /* error conditions */
#define	LIG_MTRX_LOG_WARNING    4       /* warning conditions */
#define	LIG_MTRX_LOG_NOTICE     5       /* normal but significant condition */
#define	LIG_MTRX_LOG_INFO       6       /* informational */
#define	LIG_MTRX_LOG_DEBUG      7       /* debug-level messages */


#define LIG_DEBUG_OUT_NULL              0x00
#define LIG_DEBUG_OUT_SCREEN            0x01
#define LIG_DEBUG_OUT_LOG               0x02

#define LIG_DEBUG                       0x02

/*
调用了liblig_matrix.so的程序可以不使用open/close,已经集成在内部
*/
#define lig_syslog_open()       openlog(program_invocation_short_name,LOG_NDELAY|LOG_PID,LOG_USER)
#define lig_syslog_close()      closelog()

#if LIG_DEBUG == 1
		#define pr_debug(fmt,args...)      fprintf(stderr,"[Func:%s]"fmt,__FUNCTION__,##args)//fprintf(stderr,"[Func:%s]"fmt,__FUNCTION__,##args)
#elif LIG_DEBUG == 2
		#define pr_debug(fmt,args...)      syslog(LOG_DEBUG|LOG_USER,"func=[%s]"fmt,__FUNCTION__,##args)
#else
		#define pr_debug(fmt,args...)      /* do nothing */
#endif

#define pr_log(lvl,fmt,args...)      syslog(lvl|LOG_USER,"func=[%s]"fmt,__FUNCTION__,##args)

/*
        for system error
*/

extern int errno;


/*
        self def func

*/

#include <asm/unistd.h>
#define lig_types_gettid()        ((uint32_t)(syscall(__NR_gettid)))


#ifdef __cplusplus
	}
#endif

#endif /* LIG_TYPES_H_ */
