/*for_core*//*for_app*/
/******************************************************************************
 LIG HEAD FILE
 ******************************************************************************
  File Name     : lig_matrix_dll.h
  Version       : Initial Draft
  Author        : zhangbin
  Created       : Monday, August 14, 2017
  Last Modified :
  Description   : for core
  Function List :

  History       :
  1.Date        : Monday, August 14, 2017
    Author      : zhangbin
    Modification: Created file
  2.Date        : Thursday, January 18, 2018
    Author      : wyf
    Modification: rewrite file
  3.Date        : Friday, October 19, 2018 - V1.0.5
    Author      : zouyuan
    Modification: according to new Liguo protocol rewrite module card init and input&output card checking process

******************************************************************************/
#ifndef __LIG_MATRIX_DLL_H__
#define __LIG_MATRIX_DLL_H__
#ifdef __cplusplus    
extern "C" {          
#endif
/*============include=================================*/

#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/file.h>


//#define LIG_MATRIX_DLL_DEBUG_EN

#ifdef LIG_MATRIX_DLL_DEBUG_EN
	#define pr_mx_dll_debug(fmt,args...)        pr_debug(fmt, ##args)
#else
	#define pr_mx_dll_debug(fmt,args...)               /* do nothing */
#endif

#ifdef bool
	#undef bool
#endif

#define bool int



/*=============EXTERN define==========================*/
#define LIG_MATRIX_VER_MAIN       1       <<24
#define LIG_MATRIX_VER_MINOR      0       <<16
#define LIG_MATRIX_VER_BUILD      5

#define LIG_MATRIX_DLL_VER        (LIG_MATRIX_VER_MAIN|LIG_MATRIX_VER_MINOR|LIG_MATRIX_VER_BUILD)


#define LIG_MATRIX_NO_ERR                 0

#define LIG_MATRIX_ERR_SYS                 (0-1)   //Matrix error
#define LIG_MATRIX_ERR_PARAM               (0-2)   //param err
#define LIG_MATRIX_ERR_DEV                 (0-3)
#define LIG_MATRIX_ERR_TIMEOUT             (0-4)   //timeout or device busy
#define LIG_MATRIX_ERR_DLL                 (0-5)   //dll interface err
#define LIG_MATRIX_ERR_TOO_LEN             (0-6)   //data too len,or buff not enough
#define LIG_MATRIX_ERR_OTHER               (0-7)
#define LIG_MATRIX_ERR_DB                  (0-8)
#define LIG_MATRIX_ERR_CMM                 (0-9)   //err communicate
#define LIG_MATRIX_ERR_NOT_SUP             (0-10)   //cmd dont support，发送数据错，命令返回F1
#define LIG_MATRIX_ERR_BUSY                (0-11)   //dll or dev  busy
#define LIG_MATRIX_ERR_DB_NOFOUND          (0-12)   //DB ,no value
#define LIG_MATRIX_ERR_EDID                (0-13)
#define LIG_MATRIX_ERR_CMM_SEND_DATA       (0-14)   //发送数据错，命令返回F2
#define LIG_MATRIX_ERR_CMM_CARD_BUFFER_OUT (0-15)   //子板缓冲溢出，命令返回F4
#define LIG_MATRIX_ERR_CMM_CRC             (0-16)   //冗余错误，命令返回F5
#define LIG_MATRIX_ERR_CMM_LENGTH          (0-17)   //命令长度错误，命令返回F7
#define LIG_MATRIX_ERR_CMM_SEND_TIME_OUT   (0-18)   //命令发送超时，命令返回F8
#define LIG_MATRIX_ERR_CMM_UNBOOT          (0-19)   //不能进入BOOT模式
#define LIG_MATRIX_ERR_IAP_FAILD_FILE      (0-20)   //IAP过程失败，升级文件异常
#define LIG_MATRIX_ERR_IAP_FAILD_TRANS     (0-21)   //IAP过程失败，数据包传输异常
#define LIG_MATRIX_ERR_IAP_FAILD_RESTART   (0-22)   //IAP过程失败，无法重启
#define LIG_MATRIX_ERR_REG_WRITE           (0-23)   //寄存器写错误
#define LIG_MATRIX_ERR_REG_READ            (0-24)   //寄存器读错误
#define LIG_MATRIX_ERR_IAP_FAILD_SUB_SHACK (0-25)   //子芯片升级握手失败

//inner sigal 事件
#define LIG_MATRIX_MEVENT_SLOT_CHANGE              (0x1<<0)  //插槽ACK状态改变
#define LIG_MATRIX_MEVENT_SIGNAL_CHANGE            (0x1<<1)  //输入端口信号状态改变
#define LIG_MATRIX_MEVENT_SINK_CHANGE              (0x1<<2)  //输出端口负载状态改变
#define LIG_MATRIX_MEVENT_CARD_INTERRUPT           (0x1<<3)  //子板硬件中断
#define LIG_MATRIX_MEVENT_PIPE_PARAM_CHANGE        (0x1<<4)  //端口子参数改变
#define LIG_MATRIX_MEVENT_CONNECT_VIDEO_CHANGE     (0x1<<5)  //视频切换
#define LIG_MATRIX_MEVENT_CONNECT_AUDIO_CHANGE     (0x1<<6)  //音频切换
#define LIG_MATRIX_MEVENT_CARD_STATUS_CHANGE       (0x1<<7)  //子板状态改变
#define LIG_MATRIX_MEVENT_ALARM                    (0x1<<8)  //警告
#define LIG_MATRIX_MEVENT_PORT_VIDEO_STATUS_CHANGE (0x1<<9)  //视频端口方向改变
#define LIG_MATRIX_MEVENT_PORT_AUDIO_STATUS_CHANGE (0x1<<10) //音频端口方向改变
#define LIG_MATRIX_MEVENT_UPDATE_STATUS_CHANGE     (0x1<<11) //升级状态改变
#define LIG_MATRIX_MEVENT_REBOOT_OPERATION         (0x1<<12) //有程序发起了系统重新启动的操作
#define LIG_MATRIX_MEVENT_SYS_PARAM_CHANGE         (0x1<<13) //Linux系统内部，或用户自定义参数改变中断
#define LIG_MATRIX_MEVENT_CARD_PARAM_CHANGE        (0x1<<14) //端口子参数改变


#define LIG_MATRIX_LOG_EMERG            0 /* system is unusable */
#define LIG_MATRIX_LOG_ALERT            1 /* action must be taken immediately */
#define LIG_MATRIX_LOG_CRIT             2 /* critical conditions */
#define LIG_MATRIX_LOG_ERR              3 /* error conditions */
#define LIG_MATRIX_LOG_WARNING          4 /* warning conditions */
#define LIG_MATRIX_LOG_NOTICE           5 /* normal but significant condition */
#define LIG_MATRIX_LOG_INFO             6 /* informational */
#define LIG_MATRIX_LOG_DEBUG            7 /* debug-level messages */

#define LIG_MATRIX_SIG_1                SIGRTMIN


/*=============EXTERN ENUM============================*/
typedef enum
{
 em_matrix_ver_lib	,  //查询动态库版本
 em_matrix_ver_back_hd,	//背板硬件程序版本
 em_matrix_ver_ctrl_hd,	//控制板硬件程序版本
}EM_MATRIX_VER_TYPE;


typedef enum
{
 em_matrix_build_lib,              //查询动态库
 em_matrix_build_core,             //查询core
}EM_MATRIX_BUILD_TYPE;

typedef enum
{
 em_matrix_port_v=0,
 em_matrix_port_a,
 em_matrix_port_av,
}EM_MATRIX_PORT_TYPE;

typedef enum
{
 em_matrix_unknown=-1,
 em_matrix_in=0,
 em_matrix_out=1,
 em_matrix_mix=2,
 em_matrix_ioundefine=3,
}EM_MATRIX_IO_TYPE;

typedef enum
{
 em_matrix_powerunknow=-1,
 em_matrix_poweroff=0,
 em_matrix_poweron=1,
 em_matrix_powernochange,
}EM_MATRIX_POWER_STATE;

typedef enum
{
 em_matrix_card_gen_unknown = -1,
 em_matrix_card_gen_1,     //3G
 em_matrix_card_gen_2,     //4K60
 em_matrix_card_gen_3,     //8K
}EM_MATRIX_CARD_GEN;

typedef enum
{
 em_matrix_card_audiomatrix_unknown = -1,
 em_matrix_card_audiomatrix_disable = 0,
 em_matrix_card_audiomatrix_enable = 1,
}EM_MATRIX_CARD_AUDIOMATRIX;

typedef enum
{
 em_matrix_card_unlink = -1,
 em_matrix_card_slink = 0,
 em_matrix_card_dlink = 1,
}EM_MATRIX_CARD_LINKMODE;

typedef enum
{
 em_matrix_card_iap_unknown = -1,
 em_matrix_card_iap_disable = 0,
 em_matrix_card_iap_enable = 1,
}EM_MATRIX_CARD_IAP;

typedef enum
{
 em_matrix_card_network_unknown = -1,
 em_matrix_card_network_disable = 0,
 em_matrix_card_network_enable = 1,
}EM_MATRIX_CARD_NETWORK;

typedef enum
{
 em_matrix_card_irq_unknown = -1,
 em_matrix_card_irq_disable = 0,
 em_matrix_card_irq_enable = 1,
}EM_MATRIX_CARD_IRQ;

typedef enum
{
 em_matrix_card_uartset_unknown = -1,
 em_matrix_card_uartset_disable = 0,
 em_matrix_card_uartset_enable = 1,
}EM_MATRIX_CARD_UARTSETTING;

typedef enum
{
 em_matrix_card_sync_unknown = -1,
 em_matrix_card_sync_disable = 0,
 em_matrix_card_sync_enable = 1,
}EM_MATRIX_CARD_SYNC;

typedef enum 
{
 em_matrix_sig_src_subboard,
 em_matrix_sig_src_core,
 em_matrix_sig_app,
}EM_MATRIX_SIGNAL_SRC;

typedef enum
{
 em_matrix_card_dllbusy = -2, //异常，DLL忙
 em_matrix_card_unknown=-1,   //未知，检测异常
 em_matrix_card_offline=0,    //无卡
 em_matrix_card_online=1,     //有卡， 用于硬件检测子板在线使用
 em_matrix_card_init=1,       //有卡，板卡处于初始化状态
 em_matrix_card_normal=2,     //有卡，板卡处于正常工作状态
 em_matrix_card_power_down=3, //有卡，板卡被强制下电状态
 em_matrix_card_uart_error=4, //有卡，板卡通讯不正常
 em_matrix_card_iap=5,        //有卡，卡正在升级过程
}EM_MATRIX_CARD_STATE;

typedef enum
{
 em_matrix_liguo_mode=0,
 em_matrix_kramer_mode=1,
}EM_MATRIX_FACTORY_MODE;

typedef enum
{
 em_matrix_ip_v4=4,
 em_matrix_ip_v6=6,
}EM_MATRIX_IP_TYPE;

typedef enum
{
 em_matrix_net_tcp=0,
 em_matrix_net_udp=1,
}EM_MATRIX_NET_PORT_TYPE;

typedef enum
{
 em_matrix_net_port_close=0,
 em_matrix_net_port_open=1,
 em_matrix_net_port_reset=2,
}EM_MATRIX_NET_PORT_OPERATION;

typedef enum
{
	em_matrix_btype_NULL = -1,             //无效ID
	em_matrix_btype_DVI_NoHDCP = 0,        //0
	em_matrix_btype_DVI_HDCP,              //1
	em_matrix_btype_HDMI_NoHDCP,           //2
	em_matrix_btype_HDMI_HDCP,             //3
	em_matrix_btype_DUAL_DVI_NoHDCP,       //4
	em_matrix_btype_DUAL_DVI_HDCP,         //5
	em_matrix_btype_SACLAR,                //6
	em_matrix_btype_DP,                    //7
	em_matrix_btype_SDI,                   //8
	em_matrix_btype_FIBER_NoHDCP,          //9
	em_matrix_btype_FIBER_HDCP,            //10
	em_matrix_btype_CAT5E_NoHDCP,          //11
	em_matrix_btype_CAT5E_HDCP,            //12
	em_matrix_btype_ONLY_CVBS,             //13
	em_matrix_btype_ONLY_YC,               //14
	em_matrix_btype_ONLY_YUV,              //15
	em_matrix_btype_VGA,                   //16
	em_matrix_btype_VGA_SAUDIO,            //17
	em_matrix_btype_VGA_DAUDIO,            //18
	em_matrix_btype_ONLY_VIDEO,            //19
	em_matrix_btype_VIDEO_SAUDIO,          //20
	em_matrix_btype_VIDEO_DAUDIO,          //21
	em_matrix_btype_AUDIO_ANALOG,          //22
	em_matrix_btype_ONLY_DAUDIO,           //23
	em_matrix_btype_HDMI_HDCP_AngAUDIO,    //24
	em_matrix_btype_HDMI_HDCP_DigAUDIO,    //25
	em_matrix_btype_DUAL_FIBER_NoHDCP,     //26
	em_matrix_btype_DUAL_FIBER_HDCP,       //27
	em_matrix_btype_DP_AngAUDIO,           //28
	em_matrix_btype_DP_DigAUDIO,           //29
	em_matrix_btype_HDBASET_HDCP,          //30
	em_matrix_btype_FIBER_OD,              //31
	em_matrix_btype_SDI_AngAUDIO,          //32
	em_matrix_btype_SDI_DigAUDIO,          //33
	em_matrix_btype_HDBT_LITE,             //34
	em_matrix_btype_DGKAT_AngAUDIO,
	em_matrix_btype_DGKAT_DigAUDIO,
	em_matrix_btyte_RESERVE_37,
	em_matrix_btype_RESERVE_38,
	em_matrix_btype_RESERVE_39,
	em_matrix_btype_RESERVE_40,
	em_matrix_btype_HUHD,
	em_matrix_btype_HUHD_Ang,
	em_matrix_btype_DGKAT,
	em_matrix_btype_DGKATA,
	em_matrix_btype_HDBT,
	em_matrix_btype_HDBT_Ang,
	em_matrix_btyte_TEST,
	em_matrix_btype_LARM,
	em_matrix_btype_TrueUHD,
	em_matrix_btype_TrueUHDA,
	em_matrix_btyte_F670_OMRON,
	em_matrix_btype_F670A_OMRON,
	em_matrix_btype_UNKNOW
}EM_MATRIX_BOARD_TYPE;

typedef enum
{
em_matrix_edid_unknow = -1,    //未知的 留用
em_matrix_edid_in=0,           //输入口EDID
em_matrix_edid_load=1,         //输出负载的EDID
em_matrix_edid_default=2,      //默认EDID,子板EDID
}EM_MATRIX_EDID_TYPE;

typedef enum
{
em_matrix_input_signal_unknown = -1,   //未知或异常
em_matrix_input_signal_none=0,         //无输入信号
em_matrix_input_signal_active=1,       //有输入信号，不知道信号类型
em_matrix_input_signal_hdmi=2,         //有输入信号，且为HDMI信号
em_matrix_input_signal_dvi =3,         //有输入信号，且为DVI信号
}EM_MATRIX_INPUT_SIGNAL_STATUS;

typedef enum
{
em_matrix_output_display_unknown = -1,   //未知或异常
em_matrix_output_display_none=0,         //无输出负载
em_matrix_output_display_plugin=1,       //有输出负载接入,不知道信号类型
em_matrix_output_display_hdmi=2,         //有输出负载接入，且为HDMI显示器
em_matrix_output_display_dvi=3,          //有输出负载接入，且为DVI显示器
}EM_MATRIX_OUTPUT_DISPLAY_STATUS;

typedef enum
{
em_matrix_HDCP_version_unknown = -1,    //未知或异常
em_matrix_HDCP_version_none=0,         //不支持HDCP
em_matrix_HDCP_version_1_4=1,          //支持HDCP，且HDCP版本为1.4
em_matrix_HDCP_version_2_0=2,          //支持HDCP， 且HDCP版本为2.0
}EM_MATRIX_HDCP_VERSION;

typedef enum
{
em_matrix_signal_TMDS_rates_unknown = -1,     //未知或异常
em_matrix_signal_TMDS_rates_2K=0,             //TMDS <340 Mcsc
em_matrix_signal_TMDS_rates_4K30=1,           //340 < TMDS <600 Mcsc
em_matrix_signal_TMDS_rates_4K60=2,           //TMDS = 600 Mcsc
em_matrix_signal_TMDS_rates_8K=3,             //TMDS > 600 Mcsc
}EM_MATRIX_TMDS_RATES;

typedef enum
{
em_matrix_signal_cs_unknown = -1,     //未知或异常
em_matrix_signal_cs_RGB444=0,
em_matrix_signal_cs_YUV444=1,
em_matrix_signal_cs_YUV422=2,
em_matrix_signal_cs_YUV420=3,
}EM_MATRIX_COLORSPACE_STATUS;

typedef enum
{
em_matrix_signal_cd_unknown = -1,     //未知或异常
em_matrix_signal_cd_24bit=0,
em_matrix_signal_cd_30bit=1,
em_matrix_signal_cd_36bit=2,
em_matrix_signal_cd_48bit=3,
}EM_MATRIX_COLORDEPTH_STATUS;

//输出负载能力：支持信号能力，或操作
typedef enum
{
em_matrix_ssignal_support_unknown = -1,     //未知或异常
em_matrix_ssignal_support_2K=0x00,          //一定支持
em_matrix_ssignal_support_4K30=0x01,        //支持4K30
em_matrix_ssignal_support_4K60=0x02,        //支持4K60
em_matrix_ssignal_support_8K=0x04,          //支持8K
}EM_MATRIX_SINK_SIGNAL_ABILITY;

//输出负载能力：支持的colorspace能力，或操作, 所有显示器都支持RGB444
typedef enum
{
em_matrix_cs_support_unknown = -1,     //未知或异常
em_matrix_cs_support_RGB444=0x00,      //一定支持
em_matrix_cs_support_YUV422=0x01,
em_matrix_cs_support_YUV444=0x02,
em_matrix_cs_support_YUV420=0x04,
}EM_MATRIX_COLORSPACE_ABILITY;

//输出负载能力：支持的colordepth能力，或操作, 所有显示器都支持24bit
typedef enum
{
em_matrix_cd_support_unknown = -1,     //未知或异常
em_matrix_cd_support_24bit=0x00,       //一定支持
em_matrix_cd_support_30bit=0x01,
em_matrix_cd_support_36bit=0x02,
em_matrix_cd_support_48bit=0x04,
}EM_MATRIX_COLORDEPTH_ABILITY;

//子板主芯片类型
typedef enum
{
 em_matrix_card_chip_unknown = -1,
 em_matrix_card_chip_P89V51RD2BN = 0,
 em_matrix_card_chip_STC11F60XE = 1,
 em_matrix_card_chip_STC89LE516RD_PLUS = 2,
 em_matrix_card_chip_STC90C516RD_PLUS = 3,
 em_matrix_card_chip_LPC1768 = 4,
 em_matrix_card_chip_LPC1778 = 5,
}EM_MATRIX_CARD_CHIP_TYPE;

typedef enum
{
em_resolution_detect_adjustment=1,
em_phase_mode,
em_gain,
em_equalization,
em_emphasis,
em_pc_dvd,
em_video_compensation,
em_hdcp_out_set,
em_hdmi_out_set,
em_colorspace_out_set,
em_deepcolor_out_set,
em_pattern_select,
em_out_mode,
em_brightness,
em_contrast,
em_hue,
em_color_saturation,
em_sharpness,
em_color_temperature,
em_motion_contrast,
em_gamma,
em_video_filter_type_set,
em_y_delay,
em_phase_adjustment,
em_gain_adjustment,
em_equalization_adjustment,
em_emphasis_adjustment,
em_compensation_adjustment,
em_fan_status,
em_card_port_reset,
em_scale_cut_mode,
em_out_windows_adjust,
em_video_freeze,
em_tv_format,
em_video_format,
em_picture_aspect_ratio_mode1,
em_picture_aspect_ratio_mode2,
em_pbp_adjustment,
em_out_resolution,
em_input_hor_position_adjustment,
em_input_ver_position,
em_input_hor_size_adjustment,
em_input_ver_size_adjustment,
em_output_hor_position_adjustment,
em_output_ver_position_adjustment,
em_output_hor_size_adjustment,
em_output_ver_size_adjustment,
em_zoom_b_hor_position_adjustment,
em_zoom_b_ver_position_adjustment,
em_zoom_b_hor_size_adjustment,
em_zoom_b_ver_size_adjustment,
em_zoom_w_hor_position,
em_zoom_w_ver_position,
em_zoom_w_hor_size_adjustment,
em_zoom_w_ver_size_adjustment,
em_resolution,
em_hor_total_pixels_set,
em_ver_total_pixels_set,
em_get_max_input_pixel_clock,
em_horizontal_start,
em_horizontal_active,
em_vertical_start,
em_vertical_active,
em_dp_channels,
em_dpcp,
em_restore_setup,
em_recall_setup,
em_switch_speed,
em_volume,
em_background_volume,
em_mic_volume,
em_audio_balance,
em_background_balance,
em_mic_balance,
em_audio_bass,
em_audio_treble,
em_audio_sample_frequency,
em_audio_sample_size,
em_audio_mute,
em_audio_midrange,
em_audio_loudness,
em_audio_mix_mode,
em_audio_stereo_mode,
em_audio_talk_over,
em_audio_aux_delay,
em_audio_select,
em_audio_channel,
em_signal_information,
em_inport_information,
em_monitor_information,
em_outport_information,
em_temperature,
em_voltage,
em_edid_status,
em_bus_status,
em_gen2_arc_enable_disable,
em_gen2_ana_3_5_dir,
em_gen2_input_card_analog_out_src,
em_gen2_output_card_analog_out_src,
em_gen2_input_card_audio_matrix_in_src,
em_gen2_output_card_audio_matrix_in_src,
em_gen2_input_port_hdcp_ver_max_support,
em_gen2_output_signal_hdcp_encrypt_ver,
em_gen1_nomal_hdcp_set,
em_gen1_hdbt_in_set_cable_long_short_line_mode,
em_gen1_hdbt_out_set_cable_long_short_line_mode,
em_gen1_no_arc_set_ana_port_dir,
em_gen2_sup_arc_set_ana_port_dir,
}EM_MATRIX_PARA_LIST;

typedef enum
{
	em_para_func_unknown = 0,   // 未知类型。
	em_para_func_numeric=1,    //数值型
	em_para_func_bit_option=2, //bit 型
	em_para_func_str=3,        //字符串型
}EM_MATRIX_PARA_FUC_TYPE;

typedef enum
{
	em_para_ability_unknow = 0,            // 未知类型。
	em_para_ability_sup_none=1,            //既不支持查询也不支持设置
	em_para_ability_sup_query_only=2,      //只支持查询
	em_para_ability_sup_set_only=3,        //只支持设置
	em_para_ability_sup_query_and_set=4,   //支持查询和设置
}EM_MATRIX_PARA_ABILITY;

typedef enum
{
	em_no_relevance = 0,       // 不关联。
	em_relevance=1,            //关联
}EM_MATRIX_PARA_RELEVANCE;

typedef enum
{
	em_para_port_reset = 0,
	em_para_port_factory =1,
}EM_MATRIX_PARA_RESET; //参数36

typedef enum
{
	em_para_sw_nomal=0,
	em_para_sw_fast=1,
	em_para_sw_ex_fast=2,
	em_para_sw_seamless=3,

}EM_MATRIX_PARA_SWITCH_SPEED;//参数75

typedef enum
{
	em_para_aud_mute = 0,
	em_para_aud_unmute = 1,
}EM_MATRIX_PARA_AUD_MUTE; //参数91

typedef enum
{
	em_para_aud_mix_mode_close = 0,
	em_para_aud_mix_mode_open = 1,
}EM_MATRIX_PARA_MIX_MODE; //参数94

typedef enum
{
	em_para_aud_select_auto =0 ,
	em_para_aud_select_hdmi= 1,
	em_para_aud_select_ex_digital =2,
	em_para_aud_select_ex_analog=3,
	em_para_aud_select_ex_aud_matrix=4,
}EM_MATRIX_PARA_AUD_SEL;//参数98

typedef enum
{
	em_para_arc_close = 0,
	em_para_arc_open = 1,
}EM_MATRIX_PARA_ARC; //参数159 for 6G

typedef enum
{
	em_para_analog_aud_dir_in = 0,
	em_para_analog_aud_dir_out = 1,
}EM_MATRIX_PARA_ANALOG_DIR; //参数160 for 6G

typedef enum
{
	em_para_cardin_ana_src_hdmi =1,
	em_para_cardin_ana_src_audmatrix=2,
	em_para_cardout_ana_src_audmatrix=1,
	em_para_cardout_ana_src_arc =2,
}EM_MATRIX_PARA_ANALOG_OUT_SRC; //参数161 for 6G

typedef enum
{
	em_para_card_audmatrix_src_analog =1,
	em_para_cardin_audmatrix_src_hdmi=2,
	em_para_cardout_audmatrix_src_arc =2,
}EM_MATRIX_PARA_AUD_MATRIX_SRC; //参数162 for 6G

typedef enum
{
	em_para_hdcp_1_x =1,
	em_para_hdcp_2_x=2,
	em_para_cardout_hdcp_follow =3,
	em_para_cardin_hdcp_disable =3,
}EM_MATRIX_PARA_HDCP_VERSION; //参数163 for 6G

typedef enum
{
	em_irq_reboot_none=0,
	em_irq_reboot_reset=1,
	em_irq_reboot_factory=2,
	em_irq_reboot_update=3,
}EM_MATRIX_IRQ_REBOOT_OPT;

typedef enum
{
	em_reg_type_unknown=-1,
	em_reg_type_fpga=0,
	em_reg_type_swchip=1,
}EM_MATRIX_REGISTER_TYPE;

/*=============EXTERN STRUCT =========================*/
//中断相关
typedef  uint32_t matrix_sig_t;
typedef struct  sig_flag
{
 EM_MATRIX_SIGNAL_SRC    src;
 uint32_t                thread_id;//thread_ID
 uint32_t                src_parameter;
 matrix_sig_t            sig_bit_map;
}LIG_MATRIX_OBJ_SIGNAL_FLAG;

//中断回调函数定义， 参数int 为系统信号的编号,没有实际的作用,仅用于保持格式上的一致
typedef void (* LIG_MATRIX_SIGNAL_HANDLE)(int);

//子板基本信息结构体
typedef struct obj_card
{
 uint32_t                   slot_id;                   //插槽ID
 EM_MATRIX_CARD_STATE       card_status;               //当前子板工作状态
 int16_t                    type_id;                   //子板类型ID，参考内部协议
 int16_t                    ver_major;                 //子板固件程序主版本号
 int16_t                    ver_minor;                 //子板固件程序次版本号
 int16_t                    ver_build;                 //子板固件程序build号
 EM_MATRIX_IO_TYPE          io_dir;                    //子板视频端口方向
 EM_MATRIX_CARD_LINKMODE    link_mode;                 //子板link模式，单联或双联
 EM_MATRIX_CARD_GEN         gens;                      //第几代子板
 EM_MATRIX_CARD_IAP         iap_support;               //子板是否支持内部在线升级程序的功能
 EM_MATRIX_CARD_AUDIOMATRIX audiomatrix_support;       //子板是否支持音频矩阵
 EM_MATRIX_CARD_NETWORK     network_support;           //子板是否支持网络参数配置
 EM_MATRIX_CARD_IRQ         inner_irq_support;         //子板是否支持内部中断处理
 EM_MATRIX_CARD_UARTSETTING uart_set_support;          //子板是否支持串口设置
 EM_MATRIX_CARD_SYNC        sync_support;              //子板是否支持内同步
 int32_t                    subfun_number;             //子板支持的端口参数个数
 EM_MATRIX_CARD_CHIP_TYPE   main_cpu_type;             //主CPU芯片类型
}LIG_MATRIX_OBJ_CARD;

//端口属性结构体
typedef struct obj_port
{
	uint32_t              port_id;
	EM_MATRIX_IO_TYPE     io_dir;
	EM_MATRIX_POWER_STATE power_setting;
}LIG_MATRIX_OBJ_PORT;

//输入信号信息结构体
typedef struct obj_input_signal
{
 uint32_t                      port_id;              //端口物理ID
 EM_MATRIX_INPUT_SIGNAL_STATUS signal_status;        //信号状态
 EM_MATRIX_HDCP_VERSION        hdcp_version;         //信号hdcp版本
 EM_MATRIX_TMDS_RATES          tmds_rates;           //TMDS信息
 EM_MATRIX_COLORSPACE_STATUS   cs_status;            //输入信号的colorspace状态
 EM_MATRIX_COLORDEPTH_STATUS   cd_status;            //输入信号的colordepth状态
 uint32_t                      scdc_value;           //信号scdc值，如果是3G信号，默认为0x2000
}LIG_MATRIX_OBJ_INPUT_SIGNAL;

//输出负载信息结构体
typedef struct obj_output_display
{
 uint32_t                        port_id;          //端口物理ID
 EM_MATRIX_OUTPUT_DISPLAY_STATUS display_status;   //负载状态
 EM_MATRIX_HDCP_VERSION          hdcp_version;     //负载支持hdcp版本
 EM_MATRIX_SINK_SIGNAL_ABILITY   ssignal_ability;  //负载信号能力
 EM_MATRIX_COLORSPACE_ABILITY    cs_ability;       //负载colorspace能力
 EM_MATRIX_COLORDEPTH_ABILITY    cd_ability;       //负载colordepth能力
}LIG_MATRIX_OBJ_OUTPUT_DISPLAY;

typedef struct para_attribute
{
EM_MATRIX_PARA_LIST     para; //参数的ENUM值
char                    para_name[64];//参数的名称
EM_MATRIX_PARA_ABILITY  para_ability;//参数的支持能力
                                               // 0保留
                                               // 1：不支持查询也不支持设置
							          		   //2：仅支持查询
											   //3：仅支持设置
											   //4：支持查询和设置
EM_MATRIX_PARA_FUC_TYPE  para_func_type;//参数的功能类型
                                                 //1数值型
                                     //2 BIT型
                                                 //3字符串型
EM_MATRIX_PARA_RELEVANCE para_relevance;//参数关联性 0 不关联 1关联
uint8_t                  para_occupied_byte;//参数占用的BYTE
uint16_t                 para_max;//参数的最大值 0-0x3FFF
uint16_t                 para_min;//参数的最小值 0-0x3FFF
uint16_t                 para_adjust_step_len;//参数的调节步长0-0x3FFF
uint8_t                  para_adjust_start_val;//参数起始调节值
uint16_t                 para_factory_default;//参数出厂缺省值0-0x3FFF

}LIG_MATRIX_OBJ_PARA_ATTRIBUTE;

/*=============EXTERN variable========================*/

/*=============BACK_BOARD START=======================*/

        /*******************************
        -函数名:             lig_matrix_open
        -函数功能描述:            打开设备
        -调用清单:      
        -何处调用：     
        -输入参数说明             dll_version =  LIG_MATRIX_DLL_VER
        -输出参数说明
        -返回值：设备句柄
        *********************************/
        extern void * lig_matrix_open(int dll_version);
        
        /*******************************
        -函数名:                lig_matrix_close
        -函数功能描述:            关闭设备
        -调用清单:      
        -何处调用：     
        -输入参数说明           handle 设备句柄
        -输出参数说明         
        -返回值              -1=失败;0=成功
        *********************************/
        extern int32_t lig_matrix_close(void*handle);

        
        /*******************************
        -函数名:           lig_matrix_get_version
        -函数功能描述:            获取设备版本号    
        -调用清单:  
        -何处调用：  
        -输入参数说明:        handle 设备句柄
                        ver_type:
                        em_matrix_ver_lib         查询动态库版本
                        em_matrix_ver_back_hd     背板硬件程序版本
                        em_matrix_ver_ctrl_hd     控制板硬件程序版本
        -返回值：       
                        bit31~24        主版本号8
                        bit23~16        次版本号8
                        bit15~0         builder号16
        *********************************/
        extern uint32_t lig_matrix_get_version(void*handle,EM_MATRIX_VER_TYPE ver_type);

        /*******************************
        -函数名:             lig_matrix_get_device_type
        -函数功能描述:          获取设备类型
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
        -返回值：             >0=设备可用端口,即类型,34,68等
        *********************************/
        extern int32_t lig_matrix_get_device_type(void * handle);

        /*******************************
        -Function Name : lig_matrix_set_back_uart
        -Description   : 切换背板串口,使用前记得加锁(lig_matrix_lock      )
        -Input  Param  : handle 设备句柄
                         slot_id =0 ARM ,1-17==slot 1-17
        -Return Value  : 0=success;(ERROR)=参考错误定义 
        *********************************/
        extern int32_t lig_matrix_set_back_uart(void * handle,uint32_t slot_id);

        /*******************************
		-Function Name : lig_matrix_set_devlock
		-Description   : 对设备上锁,同时会对内部串口和并口上锁
				 上锁可以设置为阻塞和非阻塞的
		-Input	Param  : handle 库句柄
				 lockflag 锁标记,0=unlock,1=lock,其他值默认lock
				 blockflag 阻塞标记,0=非阻塞,1=阻塞,其他值默认阻塞
		-Return Value  : 0=成功,非零参考错误宏定义
		*********************************/
		extern int lig_matrix_set_devlock(void* handle, int lockflag,int blockflag);

		/*******************************
		-Function Name : lig_matrix_get_devlock
		-Description   : 查询锁的状态
		-Input	Param  : handle 库句柄
		-Return Value  : lockflag 锁标记,0=unlock,1=lock,非零参考错误宏定义
		*********************************/
		extern int lig_matrix_get_devlock(void* handle);

		 /*******************************
		-Function Name : lig_matrix_set_db_mode
		-Description   : 设置数据库的模式，打开和关闭事物模式
		-Input	Param  : handle 库句柄
                        enable_transaction_mode 是否开启数据库的事物模式， = 0关闭，=1开启
		-Return Value  : 0=成功,非零参考错误宏定义
		*********************************/
		extern int lig_matrix_set_db_mode(void* handle, int enable_transaction_mode);

/*for_core_end*//*for_app_end*/


        
/*for_core*/
        /*******************************
	   -Function Name : lig_matrix_lock
	   -Description   : 加锁,可设置阻塞和非阻塞
	   -Call          :
	   -Called By     :
	   -Input  Param  :
	   -Output Param  :
	   -Return Value  :
	   *********************************/
	   //使用建议锁 flock
	   //lock_type = 0/ LOCK_NB
	   extern  int lig_matrix_lock(void*handle,int lock_type);


	   /*******************************
	   -Function Name : lig_matrix_unlock
	   -Description   : 解锁/释放锁(关闭文件自动释放)
	   -Call          :
	   -Called By     :
	   -Input  Param  :
	   -Output Param  :
	   -Return Value  :
	   *********************************/
	   extern  int lig_matrix_unlock(void*handle);

        /*******************************
        -Function Name : lig_matrix_set_slot_power
        -Description   : 设置槽电源状态,为core提供
        -Call          : 
        -Called By     : 
        -Input  Param  : slot_id [0,17],0=all,
                         power_state 参考 EM_MATRIX_POWER_STATE
        -Output Param  : 
        -Return Value  : 0=success;(ERROR)=参考错误定义 
        *********************************/
        extern int32_t lig_matrix_set_slot_power(void*handle,uint32_t slot_id, EM_MATRIX_POWER_STATE power_state);


        /*******************************
        -Function Name : lig_matrix_get_slot_force_power_state
        -Description   : server获取插槽的强制上电或者下电状态
        -Input  Param  : handle 设备句柄
                         slot_id [1,17]:槽编号
        -Return Value  : power_state   : em_matrix_powerunknow == auto
                                         em_matrix_poweroff 强制关闭,
                                         em_matrix_poweron  强制打开,
        *********************************/
        extern EM_MATRIX_POWER_STATE  lig_matrix_get_slot_force_power_state(void*handle,uint32_t slot_id);

        /*******************************
		-函数名:         lig_matrix_set_card_information
		-函数功能描述:  设置指定插槽中子板的信息(更新数据库)
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1,17] 卡槽编号
						 card_obj 子板信息；
		-返回值：       0=success;(ERROR)=参考错误定义
		*********************************/
		  extern int32_t lig_matrix_set_card_information(void* handle, uint32_t slot_id, LIG_MATRIX_OBJ_CARD *card_obj);

	   /*******************************
	   -Function Name : lig_matrix_get_card_information_fromHW
	   -Description   : 通过内部通讯0x00命令，获取插槽上卡的基本信息
	   -Call          :
	   -Called By     :
	   -Input  Param  :
	   -Output Param  :
	   -Return Value  : 0=success;(ERROR)=参考错误定义
	   *********************************/
	    extern  int32_t lig_matrix_get_card_information_fromHW(void*handle, uint32_t slot_id, LIG_MATRIX_OBJ_CARD *card_obj);

	    /*******************************
	    -Function Name : lig_matrix_set_cardObj_default
	    -Description   : 设置子板对象为默认值,但是slot_id和card_status不设置；
	    -Input  Param  : card_obj 待设置子板对象指针；
	    -Return Value  : NULL.
	    *********************************/
	    extern void lig_matrix_set_cardObj_default(LIG_MATRIX_OBJ_CARD *card_obj);
	    extern void lig_matrix_copy_cardObj(LIG_MATRIX_OBJ_CARD *card_obj_sor, LIG_MATRIX_OBJ_CARD *card_obj_des);
	    /*******************************
		-函数名:         lig_matrix_set_input_signal_information
		-函数功能描述:  设置指定端口输入信号信息(更新数据库)
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 input_port_id=[1,34] 端口编号,输入端口编号
						 insignal_obj 输入信号信息；
		-返回值：       0=success;(ERROR)=参考错误定义
		*********************************/
		  extern int32_t lig_matrix_set_port_input_signal_information(void* handle, uint32_t input_port_id,
                                                                    LIG_MATRIX_OBJ_INPUT_SIGNAL *insignal_obj);

	   /*******************************
	   -Function Name : lig_matrix_get_card_input_signal_information_fromHW
	   -Description   : 通过内部通讯0x12命令，获取子板输入信号值,
	   -Call          :
	   -Called By     :
	   -Input  Param  : insignal_obj_list  - 查询到的信号对象列表，输出返回参数;
	                    obj_number - 待获取对象个数，即子板上端口个数；
	   -Output Param  :
	   -Return Value  : 0=success;(ERROR)=参考错误定义
	   *********************************/
		 extern  int32_t lig_matrix_get_card_input_signal_information_fromHW(void*handle, uint32_t slot_id,
				                                                LIG_MATRIX_OBJ_INPUT_SIGNAL *insignal_obj_list, uint16_t obj_number);
		 /*******************************
		 -Function Name : lig_matrix_set_insignalObj_default
		 -Description   : 设置输入信号对象为默认值；
		 -Input  Param  : insignal_obj 待设置输入信号对象指针；
		 -Return Value  : NULL.
		 *********************************/
		 extern void lig_matrix_set_insignalObj_default(LIG_MATRIX_OBJ_INPUT_SIGNAL *insignal_obj);
		 extern void lig_matrix_copy_insignalObj(LIG_MATRIX_OBJ_INPUT_SIGNAL *insignal_obj_sor,
				                                   LIG_MATRIX_OBJ_INPUT_SIGNAL *insignal_obj_des);

		 /*******************************
		-函数名:         lig_matrix_broadcast_input_signal_change
		-函数功能描述:  把输入信号改变广播给对应的输出端口，0x0D命令;
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 input_port_id=[1,34] 端口编号,输入端口编号
		-返回值：       NULL
		*********************************/
		  extern void lig_matrix_broadcast_input_signal_change(void* handle, uint32_t input_port_id);

	  /*******************************
		-函数名:         lig_matrix_resend_switch_command
		-函数功能描述:  对输出端口，重新发送一次切换相关命令， 0x0D命令；
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 output_port_id=[1,34] 端口编号,输出端口编号
		-返回值：       NULL
		*********************************/
		  extern void lig_matrix_resend_switch_command(void* handle, uint32_t output_port_id);

	  /*******************************
		-函数名:         lig_matrix_get_resend_switch_command_count
		-函数功能描述:  获取输出端口，重新发送切换相关命令的次数；
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 output_port_id=[1,34] 端口编号,输出端口编号
		-返回值：       >=0 成功； <0 异常；
		*********************************/
		  extern int32_t lig_matrix_get_resend_switch_command_count(void* handle, uint32_t output_port_id);

	  /*******************************
		-函数名:         lig_matrix_set_output_display_information
		-函数功能描述:  设置指定端口输出负载信息(更新数据库)
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 output_port_id=[1,34] 端口编号,输出端口编号
						 outdisplay_obj 输出负载信息；
		-返回值：       0=success;(ERROR)=参考错误定义
		*********************************/
		  extern int32_t lig_matrix_set_port_output_display_information(void* handle, uint32_t output_port_id,
																	 LIG_MATRIX_OBJ_OUTPUT_DISPLAY *outdisplay_obj);

	   /*******************************
	   -Function Name : lig_matrix_get_card_output_display_information_fromHW
	   -Description   : 通过内部通讯命令0x02，获取子板输出负载值,
	   -Call          :
	   -Called By     :
	   -Input  Param  : outdisplay_obj_list  - 查询到的负载对象列表，输出返回参数;
	                    obj_number - 待获取对象个数，即子板上端口个数；
	   -Output Param  :
	   -Return Value  : >0 success, 获取到的参数个数;(ERROR)=参考错误定义
	   *********************************/
		 extern  int32_t lig_matrix_get_card_output_display_information_fromHW(void*handle, uint32_t slot_id,
				                         LIG_MATRIX_OBJ_OUTPUT_DISPLAY *outdisplay_obj_list, uint16_t obj_number);

		 /*******************************
		 -Function Name : lig_matrix_set_outdisplayObj_default
		 -Description   : 设置输出负载对象为默认值；
		 -Input  Param  : outdisplay_obj 待设置输出负载对象指针；
		 -Return Value  : NULL.
		 *********************************/
		 extern void lig_matrix_set_outdisplayObj_default(LIG_MATRIX_OBJ_OUTPUT_DISPLAY *outdisplay_obj);
		 extern void lig_matrix_copy_outdisplayObj(LIG_MATRIX_OBJ_OUTPUT_DISPLAY *outdisplay_obj_sor,
				                                     LIG_MATRIX_OBJ_OUTPUT_DISPLAY *outdisplay_obj_des);

/*for_core_end*/


/*for_app*/



        /*******************************
        -Function Name : lig_matrix_set_slot_force_power_state
        -Description   : app设置插槽强制上电或者下电状态
        -Input  Param  : handle 设备句柄
                         slot_id [0,17]:槽编号,0=all
                         power_state   : em_matrix_powerunknow == auto
                                         em_matrix_poweroff 强制关闭,
                                         em_matrix_poweron  强制打开,
                                         
        -Return Value  : 0=success;(ERROR)=参考错误定义 
        *********************************/
        extern int32_t  lig_matrix_set_slot_force_power_state(void*handle,uint32_t slot_id,EM_MATRIX_POWER_STATE power_state);


        /*******************************
        -Function Name : lig_matrix_get_slot_power
        -Description   : app获取插槽当前的电源状态
        -Input  Param  : handle 设备句柄
                         slot_id [1,17]:槽编号
        -Return Value  : power_state :   em_matrix_powerunknow 没有查询到
                                         em_matrix_poweroff 关闭,
                                         em_matrix_poweron  打开,
        *********************************/
        extern EM_MATRIX_POWER_STATE  lig_matrix_get_slot_power(void*handle,uint32_t slot_id);

        /*******************************
		-函数名:         lig_matrix_get_card_information
		-函数功能描述:  获取指定插槽中子板的信息
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1,17] 卡槽编号
						 card_obj 子板信息结构体缓存，查询到的信息放到这个变量中；
		-返回值：       0=success;(ERROR)=参考错误定义
		*********************************/
		  extern int32_t lig_matrix_get_card_information(void* handle, uint32_t slot_id, LIG_MATRIX_OBJ_CARD *card_obj);

		  extern EM_MATRIX_CARD_STATE lig_matrix_get_card_infor_workstatus(void* handle, uint32_t slot_id); //当前子板工作状态
		  extern int32_t lig_matrix_get_card_infor_typeid(void* handle, uint32_t slot_id); //子板类型ID，
		  extern int32_t lig_matrix_get_card_infor_version(void* handle, uint32_t slot_id); //子板固件程序主版本号|次版本号,16bit[bit15~8 Major | bit7~0 Minor]
		  extern int32_t lig_matrix_get_card_infor_ver_build(void* handle, uint32_t slot_id); //子板固件程序build号
		  extern EM_MATRIX_IO_TYPE lig_matrix_get_card_infor_direction(void* handle, uint32_t slot_id); //子板视频端口方向
		  extern EM_MATRIX_CARD_LINKMODE lig_matrix_get_card_infor_linkmode(void* handle, uint32_t slot_id); //子板link模式，单联或双联
        extern EM_MATRIX_CARD_GEN lig_matrix_get_card_infor_generation(void* handle, uint32_t slot_id); //第几代子板
        extern EM_MATRIX_CARD_IAP lig_matrix_get_card_infor_iap(void* handle, uint32_t slot_id);  //子板是否支持内部在线升级程序的功能
        extern EM_MATRIX_CARD_AUDIOMATRIX lig_matrix_get_card_infor_audiomatrix(void* handle, uint32_t slot_id); //子板是否支持音频矩阵
        extern EM_MATRIX_CARD_NETWORK lig_matrix_get_card_infor_network(void* handle, uint32_t slot_id);  //子板是否支持网络参数配置
        extern EM_MATRIX_CARD_IRQ lig_matrix_get_card_infor_irq(void* handle, uint32_t slot_id); //子板是否支持内部中断处理
        extern EM_MATRIX_CARD_UARTSETTING lig_matrix_get_card_infor_uartsetting(void* handle, uint32_t slot_id);  //子板是否支持串口设置
        extern EM_MATRIX_CARD_SYNC lig_matrix_get_card_infor_sync(void* handle, uint32_t slot_id);  //子板是否支持内同步
        extern int32_t lig_matrix_get_card_infor_subfun(void* handle, uint32_t slot_id);  //子板支持的参数个数
        extern EM_MATRIX_CARD_CHIP_TYPE lig_matrix_get_card_infor_chiptype(void* handle, uint32_t slot_id); //子板主芯片类型

          //获取芯片名称
        extern int32_t lig_matrix_get_card_chipname(EM_MATRIX_CARD_CHIP_TYPE chip_type, char* str_buffer, int buflen);
		/*******************************
		-函数名:         lig_matrix_get_card_name
		-函数功能描述:  获取指定插槽子板的名称，子板的type_id对应的名称
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1,17] 卡槽编号
						 str_buffer 输出参数，子板名称缓存，查询到的名称放到这个变量中；
						 buflen 子板名称缓存str_buffer的长度，不能小于32；
		-返回值：       >=0 success;(ERROR)=参考错误定义
		*********************************/
		  extern int32_t lig_matrix_get_card_name(void* handle, uint32_t slot_id, char* str_buffer, int buflen);
		  extern int32_t lig_matrix_get_cardtype_name(EM_MATRIX_BOARD_TYPE board_type, char* str_buffer, int buflen);
	   /*******************************
		-函数名:         lig_matrix_get_port_input_signal_information
		-函数功能描述:  获取指定输入端口的输入信号的信息
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 input_port_id=[1,34] 端口编号
						 insignal_obj 输入信号信息结构体缓存，查询到的信息放到这个变量中；
		-返回值：       0=success;(ERROR)=参考错误定义
		*********************************/
		  extern int32_t lig_matrix_get_port_input_signal_information(void* handle, uint32_t input_port_id,
                                                                    LIG_MATRIX_OBJ_INPUT_SIGNAL *insignal_obj);

        extern EM_MATRIX_INPUT_SIGNAL_STATUS lig_matrix_get_port_input_signal_status(void* handle, uint32_t input_port_id);
        extern EM_MATRIX_HDCP_VERSION lig_matrix_get_port_input_signal_hdcp_version(void* handle, uint32_t input_port_id);
        extern EM_MATRIX_TMDS_RATES lig_matrix_get_port_input_signal_tmds_status(void* handle, uint32_t input_port_id);
        extern EM_MATRIX_COLORSPACE_STATUS lig_matrix_get_port_input_signal_colorspace_status(void* handle, uint32_t input_port_id);
        extern EM_MATRIX_COLORDEPTH_STATUS lig_matrix_get_port_input_signal_colordepth_status(void* handle, uint32_t input_port_id);
        extern uint32_t lig_matrix_get_port_input_signal_scdc(void* handle, uint32_t input_port_id);

	    /*******************************
		-函数名:         lig_matrix_get_port_output_display_information
		-函数功能描述:  获取指定输出端口的输出负载的信息
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 output_port_id=[1,34] 端口编号
						 outdisplay_obj 输出负载信息结构体缓存，查询到的信息放到这个变量中；
		-返回值：       0=success;(ERROR)=参考错误定义
		*********************************/
		  extern int32_t lig_matrix_get_port_output_display_information(void* handle, uint32_t output_port_id,
																	  LIG_MATRIX_OBJ_OUTPUT_DISPLAY *outdisplay_obj);

		  extern EM_MATRIX_OUTPUT_DISPLAY_STATUS lig_matrix_get_port_output_display_status(void* handle, uint32_t output_port_id);
		  extern EM_MATRIX_HDCP_VERSION lig_matrix_get_port_output_display_hdcp_version(void* handle, uint32_t output_port_id);
		  //以下参数为能力支持，参数值有可能是或的关系
		  extern EM_MATRIX_SINK_SIGNAL_ABILITY lig_matrix_get_port_output_display_signal_ability(void* handle, uint32_t output_port_id);
		  extern EM_MATRIX_COLORSPACE_ABILITY lig_matrix_get_port_output_display_colorspace_ability(void* handle, uint32_t output_port_id);
		  extern EM_MATRIX_COLORDEPTH_ABILITY lig_matrix_get_port_output_display_colordepth_ability(void* handle, uint32_t output_port_id);
/*for_app_end*/


/*for_core*//*for_app*/


        /*******************************
        -函数名:             lig_matrix_get_slot_online_state
        -函数功能描述:          获取指定插槽是否有子板
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         slot_id=[1,17] 卡槽编号
        -返回值：                参考 EM_MATRIX_CARD_STATE
        *********************************/
        extern EM_MATRIX_CARD_STATE lig_matrix_get_slot_online_state(void *handle,uint32_t slot_id);
        /*******************************
        -函数名:             lig_matrix_get_port_parameter_list
        -函数功能描述:          获取指定端口的参数列表
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         port_id=[1，34] 端口编号
                         listbuf 参数列表缓冲 参数参考EM_MATRIX_PARA_LIST
                         buflen参数列表缓冲区长度
                         para_list_len 得到的参数数据个数

        -返回值：                0=success 其他 参考错误定义
        *********************************/
        extern int32_t lig_matrix_get_port_parameter_list(void *handle,uint32_t port_id
        		,EM_MATRIX_PARA_LIST *listbuf,uint8_t buflen,uint16_t *para_list_len);

        /*******************************
        -函数名:             lig_matrix_get_port_parameter_attribute
        -函数功能描述:          获取指定端口的指定参数的参数属性。
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         port_id=[1，34] 端口编号
                         para 参考 EM_MATRIX_PARA_LIST
                         para_attr 参数属性参考 LIG_MATRIX_OBJ_PARA_ATTRIBUTE

        -返回值：                0=success 其他 参考错误定义
        *********************************/
        extern int32_t lig_matrix_get_port_parameter_attribute(void *handle,
        		uint32_t port_id ,EM_MATRIX_PARA_LIST para,LIG_MATRIX_OBJ_PARA_ATTRIBUTE *para_attr);

        /*******************************
        -函数名:             lig_matrix_set_port_parameter_value
        -函数功能描述:          设置指定端口的指定参数的参数值。
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         port_id=[1，34] 端口编号
                         para 参考 EM_MATRIX_PARA_LIST
                         para_vol :需要设置端口参数的参数值（对于选项型参数，此处设置的参数参考对应参数的ENUM值）

        -返回值：                0=success 其他 参考错误定义
        *********************************/
        extern int32_t lig_matrix_set_port_parameter_value(void *handle,uint32_t port_id ,EM_MATRIX_PARA_LIST para,uint32_t para_vol);

        /*******************************
        -函数名:             lig_matrix_get_port_parameter_value
        -函数功能描述:          获取指定端口的指定参数的参数属性。
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         port_id=[1，34] 端口编号
                         para 参考 EM_MATRIX_PARA_LIST
                         getvolue 要获取的参数值缓冲（对于选项型参数，此处回送的参数可以参考对应参数的ENUM值）
                         getvolue_len：获取到参数的缓冲长度
        -返回值：                0=success 其他 参考错误定义
        *********************************/
        extern int32_t lig_matrix_get_port_parameter_value(void *handle,uint32_t port_id ,EM_MATRIX_PARA_LIST para,uint32_t *getvolue,uint16_t *getvolue_len);
        /*******************************
        -函数名:             lig_matrix_get_port_delay
        -函数功能描述:          获取指定端口输出延时。
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         port_id=[1，34] 端口编号
                         para : 输出延时step（0-15）每一个step是500ms
        -返回值：               大于等于0 是delay值，小于0 参考错误定义
        *********************************/
        extern int32_t lig_matrix_get_port_delay(void *handle,uint32_t port_id );
        /*******************************
        -函数名:             lig_matrix_set_port_delay
        -函数功能描述:         设置指定端口输出延时。
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         port_id=[1，34] 端口编号
                         para : 输出延时step（0-15）每一个step是500ms
        -返回值：                0=success 其他 参考错误定义
        *********************************/
        extern int32_t lig_matrix_set_port_delay(void *handle,uint32_t port_id ,uint8_t delay_step);
/*for_core_end*//*for_app_end*/



/*for_core*/


/*******************************
-Function Name : lig_matrix_get_hard_inner_signal
-Description   : 检测背板硬件的中断管脚,返回中断寄存器中的bit值
-Called By     : server
-Input  Param  : handle:设备句柄
-Return Value  : 返回检测到的bitmap,这个函数不返回错误
*********************************/
extern uint32_t lig_matrix_get_hard_inner_signal(void*handle);


/*******************************
-Function Name : lig_matrix_set_inner_signal_toAPP
-Description   : server检测到事件后,使用此函数发送内部状态信号到注册了此事件的app上
-Called By     : server
-Input  Param  : handle :设备句柄
                 sig_info :事件结构体指针,这里的信息会被APP所获取
-Return Value  : 0=success;(ERROR)=参考错误定义 
*********************************/
extern int32_t lig_matrix_set_inner_signal_toAPP(void*handle,LIG_MATRIX_OBJ_SIGNAL_FLAG * sig_info);

/*******************************
-Function Name : lig_matrix_get_inner_signal_fromAPP
-Description   : 检测app设置的信号标记
-Called By     : server
-Input  Param  : handle :设备句柄
                 sig_info :信号标记的事件的详细信息
-Return Value  : 0=没有标记
                 1=有标记
                 (ERROR)=参考错误定义 
*********************************/
extern int32_t lig_matrix_get_inner_signal_fromAPP(void*handle,LIG_MATRIX_OBJ_SIGNAL_FLAG * sig_info);


/*for_core_end*/

/*for_app*/
/*******************************
-Function Name : lig_matrix_register_inner_signal
-Description   : 用于app注册信号,注册事件,注册信号处理函数
-Called By     : app
-Input  Param  : handle :设备句柄
                 maskbit :事件掩码
                 func :信号处理函数
-Return Value  : 0=success;(ERROR)=参考错误定义
*********************************/
extern int32_t lig_matrix_register_inner_signal(void*handle,matrix_sig_t maskbit,LIG_MATRIX_SIGNAL_HANDLE  func);


/*******************************
-Function Name : lig_matrix_unregister_inner_signal
-Description   : 用于app注销信号,注销事件,目前调用后注销所有的事件
-Called By     : app
-Input  Param  : handle :设备句柄
                 
-Return Value  : 0=success;(ERROR)=参考错误定义 
*********************************/
extern int32_t lig_matrix_unregister_inner_signal(void*handle,matrix_sig_t maskbit);


/*******************************
-Function Name : lig_matrix_set_inner_signal_flag
-Description   : app设置信号标记,用于通知server app触发了信号
-Called By     : app
-Input  Param  : handle :设备句柄
                 sig_info :信号标记的事件的详细信息
-Return Value  : 0=success;(ERROR)=参考错误定义 
                 当有其他app的信息在等待处理时,此函数将返回BUSY
*********************************/
extern int32_t lig_matrix_set_inner_signal_flag(void*handle,LIG_MATRIX_OBJ_SIGNAL_FLAG * sig_info);


/*******************************
-Function Name : lig_matrix_get_inner_signal_information
-Description   : app 在信号处理函数中使用,用于检测详细的信号信息,表明信号包含的事件来源和事件掩码
-Called By     : app
-Input  Param  : handle :设备句柄
                 sig_info :信号包含的事件来源和事件掩码
-Return Value  : 0=success;(ERROR)=参考错误定义 
*********************************/
extern int32_t lig_matrix_get_inner_signal_information(void*handle,LIG_MATRIX_OBJ_SIGNAL_FLAG * sig_info);

/*******************************
-Function Name : lig_matrix_set_reboot_opt
-Description   : APP发出重启相关的中断给其它APP
-Called By     : app
-Input  Param  : handle :设备句柄
                 sig_info :信号包含的事件来源和事件掩码
-Return Value  : 0=success;(ERROR)=参考错误定义
*********************************/
extern int32_t lig_matrix_set_inner_signal_reboot_opt(void*handle, EM_MATRIX_IRQ_REBOOT_OPT em_reboot_opt);
/*******************************
-Function Name : lig_matrix_set_inner_signal_lock_fp_opt
-Description   : APP发出锁定前面板相关的中断给其它APP
-Called By     : app
-Input  Param  : handle :设备句柄
-Return Value  : 0=success;(ERROR)=参考错误定义
*********************************/
extern int32_t lig_matrix_set_inner_signal_lock_fp_opt(void*handle);

/*******************************
-Function Name : lig_matrix_set_inner_signal_fan_alarm_opt
-Description   : APP发出风扇警报
-Called By     : app
-Input  Param  : handle :设备句柄
					  fan_id ：1-10 表明是那个风扇
-Return Value  : 0=success;(ERROR)=参考错误定义
*********************************/
extern int32_t lig_matrix_set_inner_signal_fan_alarm_opt(void*handle,uint32_t fan_id);

/*******************************
-Function Name : lig_matrix_set_inner_signal_temp_alarm_opt
-Description   : APP发出温度警报
-Called By     : app
-Input  Param  : handle :设备句柄
					  slot_id ：1-17 对应的slot，204 EM 205 M21167
-Return Value  : 0=success;(ERROR)=参考错误定义
*********************************/
extern int32_t lig_matrix_set_inner_signal_temp_alarm_opt(void*handle,uint32_t slot_id);

/*******************************
-Function Name : lig_matrix_set_inner_signal_power_alarm_opt
-Description   : APP发出电源警报
-Called By     : app
-Input  Param  : handle :设备句柄
					  power_id ：1 power1  2 power2
-Return Value  : 0=success;(ERROR)=参考错误定义
*********************************/
extern int32_t lig_matrix_set_inner_signal_power_alarm_opt(void*handle,uint32_t power_id);
/*for_app_end*/



/*for_core*//*for_app*/


        /*---------------------------------------------------------------*/

        /*******Video***************/
        //              设置输入视频流；        
        //              设置输出视频流；
        /*******************************
        -函数名:             lig_matrix_get_video_pipe_io
        -函数功能描述:            获取视频管道的方向
        -调用清单:        
        -何处调用：       
        -输入参数说明:        handle 设备句柄
                       pipnum=[1,MAX]
                          
        -返回值：            参考enum定义
        *********************************/
        extern EM_MATRIX_IO_TYPE lig_matrix_get_video_pipe_io(void*handle,uint32_t pip_id);
        
        /*******************************
		-函数名:             lig_matrix_get_active_video_pipe_io
		-函数功能描述:            获取视频管道的方向，需要判断端口是否有效（双联板的第二个端口）
		-调用清单:
		-何处调用：
		-输入参数说明:        handle 设备句柄
					   pipnum=[1,MAX]

		-返回值：            参考enum定义
		*********************************/
		extern EM_MATRIX_IO_TYPE lig_matrix_get_active_video_pipe_io(void*handle,uint32_t pip_id);

        /*******************************
        -函数名:             lig_matrix_set_video_pipe_io
        -函数功能描述:          设置每个视频物理端口方向
        -调用清单:        
        -何处调用：       
        -输入参数说明:       handle 设备句柄
                      pip_id=[1,MAX]
                      io_type 参考EM_MATRIX_POWER_STATE
                      powerstate  参考EM_MATRIX_POWER_STATE
        -返回值：             0=success;(ERROR)=参考错误定义 
        *********************************/
        extern int32_t lig_matrix_set_video_pipe_io(void*handle,uint32_t pip_id,EM_MATRIX_IO_TYPE io_type,
        														EM_MATRIX_POWER_STATE init_powerstate);

        extern int32_t lig_matrix_set_video_mutil_pipe_io(void*handle, LIG_MATRIX_OBJ_PORT* port_obj_list, uint16_t obj_num);
        /*******************************
        -函数名:             lig_matrix_set_video_pipe_power
        -函数功能描述:          设置视频端口的power down
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         pip_id=[1,MAX]
                         io_type 参考EM_MATRIX_IO_TYPE
                         powerstate 参考 EM_MATRIX_POWER_STATE
        -返回值：                参考错误定义
        *********************************/
        extern int32_t lig_matrix_set_video_pipe_power(void*handle,uint32_t pip_id,EM_MATRIX_IO_TYPE io_type,
        		EM_MATRIX_POWER_STATE powerstate);

        /*******************************
        -函数名:             lig_matrix_get_video_pipe_power
        -函数功能描述:          设置视频端口的power down
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         pip_id=[1,MAX]
                         io_type 参考EM_MATRIX_IO_TYPE
        -返回值：                参考 EM_MATRIX_POWER_STATE
        *********************************/
        extern EM_MATRIX_POWER_STATE lig_matrix_get_video_pipe_power(void*handle,uint32_t pip_id,
        		EM_MATRIX_IO_TYPE io_type);


        /*******************************
        -Function Name : lig_matrix_set_video_switch
        -Description   : 设置视频切换状态,暂存切换状态,准备切换的条件
        -Called By     : APP
        -Input  Param  : handle         设备句柄
                         video_in       要设置的输入端口物理id[0,34],
                                        输入为0表示关闭对应的输出
                         video_out      要设置的输出端口物理id[1,34]
        -Return Value  : 0 : 成功
                         小于0:参考错误定义
        *********************************/
        extern int32_t lig_matrix_set_video_switch (void*handle,uint32_t video_in, 
                                                               uint32_t video_out);


        /*******************************
        -Function Name : lig_matrix_stash_video_switch_data
        -Description   : 暂存切换数据
        -Call          : 
        -Called By     : 
        -Input  Param  : 
        -Output Param  : 
        -Return Value  : 0=: 成功;
                         LIG_MATRIX_ERR_TOO_LEN暂存队列满,请调用lig_matrix_set_video_switch_done
        *********************************/
        extern int32_t lig_matrix_stash_video_switch_data(void*handle,uint32_t video_in, 
                                                        uint32_t video_out);
                                                        
        /*******************************
			-Function Name : ex_lig_matrix_stash_video_switch_data
			-Description   : 暂存切换数据,删除重复切换的数据
			-Call          :
			-Called By     :
			-Input  Param  :
			-Output Param  :
			-Return Value  : 0=: 成功;
							 LIG_MATRIX_ERR_TOO_LEN暂存队列满,请调用ex_lig_matrix_set_video_switch_done
		*********************************/
			extern int32_t ex_lig_matrix_stash_video_switch_data(void*handle,uint32_t video_in,
														                    uint32_t video_out);

        /*******************************
        -Function Name : lig_matrix_set_video_switch_done
        -Description   : 执行lig_matrix_set_video_switch的暂存,更新切换状态数据库,
                         设置物理寄存实现切换
        -Called By     : APP
        -Input  Param  : handle         设备句柄
        -Return Value  : >=0 : 成功次数
                         小于0:参考错误定义
        *********************************/
        extern int32_t lig_matrix_set_video_switch_done (void*handle);

        /*******************************
		   -Function Name : ex_lig_matrix_set_video_switch_done
		   -Description   : 执行ex_lig_matrix_stash_video_switch_data的暂存,
		                          分析切换数据，使用广播方式发送命令;
		                          更新切换状态数据库,设置物理寄存实现切换;
		   -Called By     : APP
		   -Input  Param  : handle         设备句柄
		   -Return Value  : >=0 : 成功次数
							小于0:参考错误定义
	   *********************************/
        extern int32_t ex_lig_matrix_set_video_switch_done (void*handle);


        /*******************************
        -Function Name : lig_matrix_get_video_switch_state
        -Description   : 获取视频切换状态,根据输出端口返回切换对应的输入
        -Called By     : APP
        -Input  Param  : handle         设备句柄
                         video_out      要查询的输出端口[1,34]
        -Return Value  : 0-34 :video_in,其中0表示关闭
                         小于0:参考错误定义
        *********************************/
        extern int32_t lig_matrix_get_video_switch_state(void*handle,  uint32_t video_out);

        
        /*******************************
        -函数名:             lig_matrix_set_audio_pipe_io
        -函数功能描述:          设置音频端口的方向
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         pip_id=[1,MAX]
                         io_type 参考EM_MATRIX_IO_TYPE
        -返回值：                参考错误定义
        *********************************/
        extern int32_t lig_matrix_set_audio_pipe_io (void* handle,uint32_t pip_id,EM_MATRIX_IO_TYPE io_type);
        extern int32_t lig_matrix_set_audio_mutil_pipe_io(void*handle, LIG_MATRIX_OBJ_PORT* port_obj_list, uint16_t obj_num);
        /*******************************
        -函数名:             lig_matrix_get_audio_pipe_io
        -函数功能描述:          查询音频端口的方向
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         pip_id=[1,MAX]
        -返回值：                参考EM_MATRIX_IO_TYPE
        *********************************/
        extern EM_MATRIX_IO_TYPE lig_matrix_get_audio_pipe_io(void*handle,uint32_t pip_id);

        /*******************************
        -函数名:             lig_matrix_sw_audio
        -函数功能描述:          	切换音频
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         audio_in=[1,MAX]
                         audio_out=[1,max]

        -返回值：                参考错误定义
        *********************************/
        extern int32_t lig_matrix_switch_audio (void* handle, uint32_t audio_in,uint32_t audio_out);
        /*******************************
        -函数名:             lig_matrix_close_audio_out
        -函数功能描述:          	切换音频
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         audio_out=[1,max]

        -返回值：                参考错误定义
        *********************************/
        extern int32_t lig_matrix_close_audio_out (void* handle, uint32_t audio_out);
        extern int32_t lig_matrix_set_audio_pipe_power(void* handle, uint32_t pip_id, EM_MATRIX_POWER_STATE powerstate);

        /*******************************
		-Function Name : lig_matrix_set_audio_switch
		-Description   : 设置音频切换状态,暂存切换状态,准备切换的条件
		-Called By     : APP
		-Input  Param  : handle         设备句柄
						     audio_in       要设置的输入端口物理id[0,34],输入为0表示关闭对应的输出
						     audio_out      要设置的输出端口物理id[1,34]
		-Return Value  : 0 : 成功； 小于0:参考错误定义
		*********************************/
		  extern int32_t lig_matrix_set_audio_switch(void*handle,uint32_t audio_in,
															                  uint32_t audio_out);

		/*******************************
		-Function Name : lig_matrix_stash_audio_switch_data
		-Description   : 暂存切换数据，没有发送命令等操作，只把待切换存储到缓冲中
		-Call          :
		-Called By     :
		-Input  Param  :
		-Output Param  :
		-Return Value  : 0=: 成功;
						 LIG_MATRIX_ERR_TOO_LEN暂存队列满,请调用lig_matrix_set_audio_switch_done
		*********************************/
		  extern int32_t lig_matrix_stash_audio_switch_data(void*handle,uint32_t audio_in,
														                            uint32_t audio_out);

		/*******************************
		-Function Name : lig_matrix_set_audio_switch_done
		-Description   : 执行实际的切换操作，更新切换状态数据库,设置物理寄存实现切换，并发送音频切换中断信号；
		-Called By     : APP
		-Input  Param  : handle     设备句柄
		-Return Value  : >=0 : 成功次数， 小于0:参考错误定义
		*********************************/
		   extern int32_t lig_matrix_set_audio_switch_done(void*handle);

        /*******************************
        -函数名:             lig_matrix_get_audio_sw_state
        -函数功能描述:          	切换音频
        -调用清单:
        -何处调用：
        -输入参数说明:          handle 设备句柄
                         audio_out=[1,max]

        -返回值：              正确  ： 回送当前切换的输入口 错误 ： 参考错误定义
        *********************************/
        extern int32_t lig_matrix_get_audio_switch_state (void* handle,uint32_t audio_out);



/*for_core_end*//*for_app_end*/


/*for_app*/


		/*******************************
		-函数名:             lig_matrix_card_factory
		-函数功能描述:          设置子板的出厂复位
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 io_type 参考EM_MATRIX_IO_TYPE
						 fac_mode 参考EM_MATRIX_FACTORY_MODE
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_card_factory(void *handle ,uint32_t slot_id,EM_MATRIX_IO_TYPE io_type,
													EM_MATRIX_FACTORY_MODE fac_mode);
		/*******************************
		-函数名:             lig_matrix_set_card_mac
		-函数功能描述:          设置子板的网络口的MAC
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 macbuf 要设置的MAC 地址 如[4C,08,3C,00,01,01]
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_set_card_mac(void *handle ,uint32_t slot_id,uint8_t *macbuf);
		/*******************************
		-函数名:             lig_matrix_get_card_mac
		-函数功能描述:          得到子板的网络口的MAC
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 macbuf 得到的MAC存储缓冲 最小为6
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_get_card_mac(void *handle ,uint32_t slot_id,uint8_t *macbuf);
		/*******************************
		-函数名:             lig_matrix_set_card_ip_type
		-函数功能描述:          设置子办的IP TYPE
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 ip_type 参考 EM_MATRIX_IP_TYPE
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_set_card_ip_type(void *handle ,uint32_t slot_id,EM_MATRIX_IP_TYPE ip_type);

		/*******************************
		-函数名:             lig_matrix_get_card_ip_type
		-函数功能描述:          设置子办的IP TYPE
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 ip_type 参考 EM_MATRIX_IP_TYPE
		-返回值：       返回子板的IP 类型 ， 小于0 参考错误定义
		*********************************/
        extern int32_t lig_matrix_get_card_ip_type(void *handle ,uint32_t slot_id);
		/*******************************
		-函数名:             lig_matrix_set_card_ip
		-函数功能描述:          设置子板的网络口的IP
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 ipbuf 要设置的IP 地址 如[192 168 20 144]
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_set_card_ip(void *handle ,uint32_t slot_id,uint8_t *ipbuf);
		/*******************************
		-函数名:             lig_matrix_get_card_ip
		-函数功能描述:          得到子板的网络口的IP
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 ipbuf 得到子板的IP缓存 最小长度为4
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_get_card_ip(void *handle ,uint32_t slot_id,uint8_t *ipbuf);
		/*******************************
		-函数名:             lig_matrix_set_card_mask
		-函数功能描述:          设置子板的网络口的mask
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 maskbuf 要设置的mask
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_set_card_mask(void *handle ,uint32_t slot_id,uint8_t *maskbuf);
		/*******************************
		-函数名:             lig_matrix_set_card_mask
		-函数功能描述:          得到子板的网络口的mask
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 maskbuf 得到子板的mask缓存 最小长度为4
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_get_card_mask(void *handle ,uint32_t slot_id,uint8_t *maskbuf);
		/*******************************
		-函数名:             lig_matrix_set_card_gate
		-函数功能描述:          设置子板的网络口的gate
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 gatebuf 要设置的mask
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_set_card_gate(void *handle ,uint32_t slot_id,uint8_t *gatebuf);
		/*******************************
		-函数名:             lig_matrix_get_card_gate
		-函数功能描述:          得到子板的网络口的gate
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 gatebuf 得到子板的mask缓存 最小长度为4
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_get_card_gate(void *handle ,uint32_t slot_id,uint8_t *gatebuf);
		/*******************************
		-函数名:             lig_matrix_set_card_net_port
		-函数功能描述:          得到子板的网络口的gate
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 port_type 参考EM_MATRIX_NET_PORT_TYPE
						 port_val 1-65535
		-返回值：                参考错误定义
		*********************************/
        extern int32_t lig_matrix_set_card_net_port(void *handle ,uint32_t slot_id,EM_MATRIX_NET_PORT_TYPE port_type
														,uint16_t port_val);
		/*******************************
		-函数名:             lig_matrix_get_card_net_port
		-函数功能描述:          得到子板的网络口的gate
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 port_type 参考EM_MATRIX_NET_PORT_TYPE
		-返回值：               >0 TCP UDP 端口号  其他 参考错误定义
		*********************************/
        extern int32_t lig_matrix_get_card_net_port(void *handle ,uint32_t slot_id,EM_MATRIX_NET_PORT_TYPE port_type);
		/*******************************
		-函数名:             lig_matrix_card_net_operate
		-函数功能描述:          操作子板网络口 开关 复位
		-调用清单:
		-何处调用：
		-输入参数说明:          handle 设备句柄
						 slot_id=[1,17] 18 ：linux 板小ARM
						 opt 参考EM_MATRIX_NET_PORT_OPERATION
		-返回值：            参考错误定义
		*********************************/
        extern int32_t lig_matrix_card_net_operate(void *handle ,uint32_t slot_id,EM_MATRIX_NET_PORT_OPERATION opt);


/*for_app_end*/


/*for_core*//*for_app*/


		/*******************************
		-函数名:             lig_matrix_get_port_edid
		-函数功能描述:       得到端口的EDID数据，并放到EDID buffer中
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 pip_id=[1，34]
						 edid_type 参考EM_MATRIX_EDID_TYPE
						 edid_buf  得到的edid存储缓存
						 buf_len   edid缓存的长度，256以上
		-返回值：            参考错误定义
		*********************************/
        extern int32_t lig_matrix_get_port_edid(void*handle, uint32_t pip_id,EM_MATRIX_EDID_TYPE edid_type,
        		uint8_t *edid_buf,uint16_t buf_len);
		/*******************************
		-函数名:             lig_matrix_set_port_edid
		-函数功能描述:       设置端口的EDID
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 pip_id=[1，34]
						 edid_type 参考EM_MATRIX_EDID_TYPE（暂时只支持设置输入口的EDID）
						 edid_buf  设置的edid存储缓存
						 buf_len   edid缓存的长度，128或256
		-返回值：            参考错误定义
		*********************************/
        extern int32_t lig_matrix_set_port_edid(void*handle, uint32_t pip_id,EM_MATRIX_EDID_TYPE edid_type,
        		uint8_t *edid_buf,uint16_t buf_len);

        /*******************************
		-函数名:             lig_matrix_bypass_slot_ligCommand
		-函数功能描述:       使用直通命令，标准liguoA格式命令
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1，24]，1~17 - 标准子板插槽； 18 - Ethnet板； 19 - Linux_ARM； 20 - EMonitor；
						 sendvalue - 输入，发送的命令数据；
                   sendlen - sendvalue中发送的命令数据长度；
                   recvbuffer - 输出，接收的命令数据缓存区；按liguo_A格式接收完成
                   recvbuflen - recvbuffer缓存长度;
                   cmd_time_out - 命令超时时间; 单位:毫秒，ms;
		-返回值：        >=0表示成功，并返回接收到的数据长度； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_bypass_slot_ligCommand(void*handle, uint32_t slot_id,
				uint8_t *sendvalue, uint16_t sendlen, uint8_t *recvbuffer, uint16_t recvbuflen, uint16_t cmd_time_out);

		/*******************************
		-函数名:             lig_matrix_bypass_slot_Command
		-函数功能描述:       使用直通命令，无任何格式,发送数据后，在规定时间内接收数据，接收到多少就返回多少
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1，24]，1~17 - 标准子板插槽； 18 - Ethnet板； 19 - Linux_ARM； 20 - EMonitor；
						 sendvalue - 输入，发送的命令数据；
				       sendlen - sendvalue中发送的命令数据长度；
				       recvbuffer - 输出，接收的命令数据缓存区；
				       recvbuflen - recvbuffer缓存长度;
				       recv_cmd_time - 命令接收时间; 单位:毫秒，ms;
		-返回值：       >=0表示成功，并返回接收到的数据长度； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_bypass_slot_Command(void*handle, uint32_t slot_id,
				uint8_t *sendvalue, uint16_t sendlen, uint8_t *recvbuffer, uint16_t recvbuflen, uint16_t recv_cmd_time);

		/*******************************
		-函数名:             lig_matrix_get_iap_process_PCT
		-函数功能描述:       获取当前IAP升级过程百分比
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1，18], 18是LinuxARM
		-返回值：       >=0表示当前百分比； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_get_iap_process_PCT(void*handle, uint32_t slot_id);

		/*******************************
		-函数名:             lig_matrix_get_iap_process_slotID
		-函数功能描述:       获取当前IAP升级过程百分比
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
		-返回值：       >0表示当前升级的插槽,==0表示没有升级子板； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_get_iap_process_slotID(void*handle);

		/*******************************
		-函数名:             lig_matrix_iap_card_mainCPU
		-函数功能描述:       升级子板主CPU,带子板校验
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1，24]，1~17 - 标准子板插槽； 18 - Ethnet板； 19 - Linux_ARM； 20 - EMonitor；
						 type_id = 子板类型ID;
                   card_vid_io_dir = 子板视频端口方向;
						 filepath - 文件路径；
		-返回值：       =0表示成功； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_iap_card_mainCPU(void*handle, uint32_t slot_id, char *filepath,
				                                     int16_t card_type_id, EM_MATRIX_IO_TYPE card_vid_io_dir);

		/*******************************
		-函数名:             lig_matrix_iap_card_force_mainCPU
		-函数功能描述:       强制升级子板主CPU,不经过任何子板状态校验
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1，24]，1~17 - 标准子板插槽； 18 - Ethnet板； 19 - Linux_ARM； 20 - EMonitor；
						 type_id = 子板类型ID;
				       card_vid_io_dir = 子板视频端口方向;
						 filepath - 文件路径；
		-返回值：       =0表示成功； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_iap_card_force_mainCPU(void*handle, uint32_t slot_id, char *filepath);

		/*******************************
		-函数名:             lig_matrix_iap_card_SubChip
		-函数功能描述:       升级子板的子芯片程序
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 slot_id=[1，24]，1~17 - 标准子板插槽； 18 - Ethnet板； 19 - Linux_ARM； 20 - EMonitor；
						 filepath - 文件路径；
		-返回值：       =0表示成功； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_iap_card_SubChip(void*handle, uint32_t slot_id, char *filepath,
				int16_t card_type_id, EM_MATRIX_IO_TYPE card_vid_io_dir);

		/*******************************
		-函数名:             lig_matrix_write_register
		-函数功能描述:       写寄存器
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 reg_type - 寄存器类型，FPGA或切换芯片；
					    reg_page_addr - 寄存器页地址；
					    reg_offset_addr - 寄存器偏移地址;
					        寄存器实际地址 = reg_page_addr + reg_offset_addr；
					   reg_value - 写寄存器值;
		-返回值：      =0表示成功； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_write_register(void*handle, EM_MATRIX_REGISTER_TYPE reg_type,
				                       uint16_t reg_page_addr, uint16_t reg_offset_addr, uint8_t reg_value);
		/*******************************
		-函数名:             lig_matrix_read_register
		-函数功能描述:       读寄存器
		-调用清单:
		-何处调用：
		-输入参数说明:  handle 设备句柄
						 reg_type - 寄存器类型，FPGA或切换芯片；
						reg_page_addr - 寄存器页地址；
						reg_offset_addr - 寄存器偏移地址;
							寄存器实际地址 = reg_page_addr + reg_offset_addr；
		-返回值：      >=0表示成功,获取到的寄存器值； <0, 参考错误定义
		*********************************/
		extern int32_t lig_matrix_read_register(void*handle, EM_MATRIX_REGISTER_TYPE reg_type,
									   uint16_t reg_page_addr, uint16_t reg_offset_addr);
/*for_all_end*/

	#include<syslog.h>

	#define lig_matrix_log_maco(priority,msg,args...)  syslog(priority|LOG_USER,"func=[%s]"msg,__FUNCTION__,##args)
/*=============sub BOARD ctrl END========================*/


#ifdef __cplusplus
}
#endif

#endif  /*endfile*/



/*for_core_end*/ /*for_app_end*/ 

