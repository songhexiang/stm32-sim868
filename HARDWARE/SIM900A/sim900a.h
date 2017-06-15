#ifndef __SIM900A_H__
#define __SIM900A_H__	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-SIM900A GSM/GPRS模块驱动	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//无
//////////////////////////////////////////////////////////////////////////////////	

#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8		//高低字节交换宏定义
extern u8 SIM900_CSQ[3];
extern u8 Flag_send_message;
void sim900a_test(void);//sim900a主测试程序 
void sim_send_sms(u8*phonenumber,u8*msg);
void sim_at_response(u8 mode);	
u8* sim900a_check_cmd(u8 *str);
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 sim900a_chr2hex(u8 chr);
u8 sim900a_hex2chr(u8 hex);
void sim900a_unigbk_exchange(u8 *src,u8 *dst,u8 mode);
void sim900a_load_keyboard(u16 x,u16 y,u8 **kbtbl);
void sim900a_key_staset(u16 x,u16 y,u8 keyx,u8 sta);
u8 sim900a_get_keynum(u16 x,u16 y);
//u8 sim900a_call_test(void);			//拨号测试
//void sim900a_sms_read_test(void);	//读短信测试
//void sim900a_sms_send_test(void);	//发短信测试 
//void sim900a_sms_ui(u16 x,u16 y);	//短信测试UI界面函数
//u8 sim900a_sms_test(void);			//短信测试
//void sim900a_mtest_ui(u16 x,u16 y);	//SIM900A主测试UI
//u8 sim900a_gsminfo_show(u16 x,u16 y);//显示GSM模块信息
extern u8 sim900a_word_test(void);
extern u8 SIM900A_MESSAGE_MODE(u8 MODE);
extern u8 SIM900A_SEND_MESSAGE(u8 *numerb,u8 *data);
void sim900a_test(void);			//SIM900A主测试函数
u8 Sim900_send_message_show(u8 *content);
u8 SIM900A_CONNECT_SERVER(u8 *IP_ADD,u8 *COM);
extern u8 sim808_open_gps(void);
u8 SIM900A_GPRS_SEND_DATA(u8 *DATA);
u8 SIM808_GPS_HANDLE(u8 * message);
#endif





