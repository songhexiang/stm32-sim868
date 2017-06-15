#ifndef __USART2_H
#define __USART2_H	 
#include "sys.h"  
	   

#define USART2_MAX_RECV_LEN		1024				//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		1024				//最大发送缓存字节数
#define USART2_RX_EN 			1					//0,不接收;1,接收.

#define USART3_MAX_RECV_LEN		100				//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		100				//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

#define USART4_MAX_RECV_LEN		100				//最大接收缓存字节数
#define USART4_MAX_SEND_LEN		100				//最大发送缓存字节数
#define USART4_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern u16 USART2_RX_STA;   						//接收数据状态

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern u16 USART3_RX_STA;   						//接收数据状态

extern u8  USART4_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART4_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern u16 USART4_RX_STA;   						//接收数据状态

void USART2_Init(u32 bound);				//串口2初始化 
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len);
void u2_printf(char* fmt, ...);
void USART3_Init(u32 bound);
void UART3SendByte(unsigned char SendData);
void USART4_Init(u32 bound);
void UART4SendByte(unsigned char SendData);
#endif













