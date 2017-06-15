#ifndef __BEEP_H__
#define __BEEP_H__
	 
#include "stm32f10x.h"

//LED¶Ë¿Ú¶¨Òå
#define BEEP_RCC     RCC_APB2Periph_GPIOB                            
#define BEEP_PORT	  GPIOB    
#define BEEP        GPIO_Pin_8

void BEEP_GPIO_Config(void);
void BEEP_ON(void);
void BEEP_OFF(void);		 	
void  BEEP_MODE(u8 mode);
#endif
