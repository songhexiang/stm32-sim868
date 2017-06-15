/**********************************************************************************
 * 文件名  ：BEEP.c
 * 描述    ：BEEP 应用函数库         
 * 硬件连接：       
 *             PB7 -> BEEP     
 *               
 * 库版本  ：ST_v3.5
**********************************************************************************/
#include "BEEP.h"	   
#include "delay.h"	   
/*******************************************************************************
* 函数名  : GPIO_Config
* 描述    : BEEP
* 输入    : 无
* 输出    : 无
* 返回    : 无 
*******************************************************************************/
void BEEP_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(BEEP_RCC, ENABLE);			//使能GPIOB的外设时钟	
	
	GPIO_InitStructure.GPIO_Pin = BEEP;	//选择要初始化的GPIOB引脚(PB5,PB6,PB7,PB8,PB9)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置引脚工作模式为通用推挽输出 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);			//调用库函数中的GPIO初始化函数，初始化GPIOB中的PB5,PB6,PB7,PB8,PB9引脚			 
	BEEP_OFF();
}

//点亮BEEP.PB7
void BEEP_ON(void) 
{
	GPIO_SetBits(BEEP_PORT,BEEP);
}
//关闭BEEP.PB7
void BEEP_OFF(void)
{
	GPIO_ResetBits(BEEP_PORT,BEEP);
}
void  BEEP_MODE(u8 mode)
{
	switch(mode)	
	{
		case 0:	//初始化成功
			BEEP_ON();
		  delay_ms(1000);
			BEEP_OFF();
		  break;
		case 1:	//自检测不过
			BEEP_ON();
		  delay_ms(200);
			BEEP_OFF();
		  break;
		case 2:	//来短信			
			BEEP_ON();
		  delay_ms(500);
			BEEP_OFF();
		  break;
		case 3:	//短信发送失败
			BEEP_ON();
		  delay_ms(200);
			BEEP_OFF();
			delay_ms(200);
		default:
			break;
	}
}
