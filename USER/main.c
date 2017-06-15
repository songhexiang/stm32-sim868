#include "sim900a.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "sys.h"
#include "bmp.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_dma.h"
#include "string.h"
#include "gpio.h"

//GPS_EN需要接到PB.13或者接到EXT脚
char ip[]="182.92.123.13";		//服务器IP或域名
char port[]="6666";						//端口
char id[]="15291453406：";		//设备ID，用手机号,长度12


int main(void)
 {
	u8 res=1;
	char message[100];
	char zigbee[100];
	delay_init();	    	 			//延时函数初始化	  
	NVIC_Configuration(); 	 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
  GPIO_Config();          	//配置输出IO
  uart_init(115200);      	//初始化串口1
	USART2_Init(115200);			//初始化串口2 
  USART3_Init(115200);    	//初始化串口3 
	USART4_Init(115200);
	TIM2_NVIC_Configuration(); /* TIM2 定时配置 */
	TIM2_Configuration(); 		
	//EXTIX_Init();//外部中断
  delay_ms(10);							//等待系统稳定
  SIM868_EN(ON);						//打开SIM868电源
  GPS_EN(ON);								//使能SIM868 GPS 
  delay_ms(100);						//等待系统稳定
  printf("\r\n****程序版本V1.0*****\r\n");
  printf("\r\n******系统开机*******\r\n");
	while(res)
	{
		res=sim900a_word_test();	
		switch(res)
		{
			case 1:	//无通信失败
								 
				break;
			case 2:	//无SIM卡

				break;
			case 3:	//等待注册到网络

				break;
				
			default:
				break;				
		}	
    
	} //系统检测完毕	
    //连接GPRS服务器
  printf("\r\n******SIM868设备正常*******\r\n");
	res=1;
	while(res)
	{		
		res=SIM900A_CONNECT_SERVER((u8*)ip,(u8*)port);
	}
	delay_ms(1000);
  printf("\r\n******GPRS连接成功*******\r\n");
	while(1)
	{

        if(USART3_RX_STA&0x8000)
        {
            strcpy(&message[0],(const char*)id);
            printf("\r\n中心节点GPS数据:%s\r\n",USART3_RX_BUF);
            strcat(&message[0],(const char*)USART3_RX_BUF);
            USART3_RX_STA=0;
        }
				if(USART4_RX_STA&0x8000)
        {
            strcpy(&zigbee[0],(const char*)id);
            printf("\r\nZigbee模块数据:%s\r\n",USART4_RX_BUF);
            strcat(&zigbee[0],(const char*)USART4_RX_BUF);
            USART4_RX_STA=0;
        }
				if((Flag_Send_data&0x8080))
				{
					Flag_Send_data=0;
					START_TIME;
					res=SIM900A_GPRS_SEND_DATA((u8 *)message);
					//SIM900A_GPRS_SEND_DATA("\r\n");
					SIM900A_GPRS_SEND_DATA((u8 *)zigbee);
					USART4_RX_STA=0;
					switch(res)
					{
						case 0:
							//START_TIME;
						default:
							while(res)
							{		
								res=SIM900A_CONNECT_SERVER((u8*)ip,(u8*)port);
							}					
					}
				}
		}	
}

