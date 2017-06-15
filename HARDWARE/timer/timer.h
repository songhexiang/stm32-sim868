#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

////////////////////////////////////////////////////////////////////////////////// 	
extern void TIM2_NVIC_Configuration(void); /* TIM2 ∂® ±≈‰÷√ */
extern void TIM2_Configuration(void); 
//void TIM7_Int_Init(u16 arr,u16 psc);
#endif
