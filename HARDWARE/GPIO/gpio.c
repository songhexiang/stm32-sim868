
#include "gpio.h"

/***************  配置输出控制用到的I/O口 *******************/
void GPIO_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PC端口
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出，自己上拉       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PC端口
  //关闭所有输出
  GPIO_SetBits(GPIOB, GPIO_Pin_14);	 // 关闭SIM868电源
  GPIO_ResetBits(GPIOB, GPIO_Pin_13);// 关闭SIM868 GPS使能
}



