#ifndef __GPIO_H
#define	__GPIO_H

#include "stm32f10x.h"

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1

/*
#define SIM868_EN(a)	if (a)	\
                        GPIO_SetBits(GPIOB,GPIO_Pin_14);\
                        else		\
                        GPIO_ResetBits(GPIOB,GPIO_Pin_14)
*/
#define SIM868_EN(a)	if (a)	\
                        GPIO_SetBits(GPIOB,GPIO_Pin_14);\
                        else		\
                        GPIO_ResetBits(GPIOB,GPIO_Pin_14)

#define GPS_EN(a)	    if (a)	\
                        GPIO_ResetBits(GPIOB,GPIO_Pin_13);\
                        else		\
                        GPIO_SetBits(GPIOB,GPIO_Pin_13)
                        



void GPIO_Config(void)	;

#endif /* __LED_H */
