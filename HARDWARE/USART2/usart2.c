#include "delay.h"
#include "usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "stm32f10x_dma.h"
//////////////////////////////////////////////////////////////////////////////////	 							  
////////////////////////////////////////////////////////////////////////////////// 	   

//´®¿Ú·¢ËÍ»º´æÇø 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//·¢ËÍ»º³å,×î´óUSART2_MAX_SEND_LEN×Ö½Ú
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//·¢ËÍ»º³å,×î´óUSART2_MAX_SEND_LEN×Ö½Ú
#ifdef USART2_RX_EN   								//Èç¹ûÊ¹ÄÜÁË½ÓÊÕ   	  
//´®¿Ú½ÓÊÕ»º´æÇø 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//½ÓÊÕ»º³å,×î´óUSART2_MAX_RECV_LEN¸ö×Ö½Ú.
//Í¨¹ıÅĞ¶Ï½ÓÊÕÁ¬Ğø2¸ö×Ö·ûÖ®¼äµÄÊ±¼ä²î²»´óÓÚ10msÀ´¾ö¶¨ÊÇ²»ÊÇÒ»´ÎÁ¬ĞøµÄÊı¾İ.
//Èç¹û2¸ö×Ö·û½ÓÊÕ¼ä¸ô³¬¹ı10ms,ÔòÈÏÎª²»ÊÇ1´ÎÁ¬ĞøÊı¾İ.Ò²¾ÍÊÇ³¬¹ı10msÃ»ÓĞ½ÓÊÕµ½
//ÈÎºÎÊı¾İ,Ôò±íÊ¾´Ë´Î½ÓÊÕÍê±Ï.
//½ÓÊÕµ½µÄÊı¾İ×´Ì¬
//[15]:0,Ã»ÓĞ½ÓÊÕµ½Êı¾İ;1,½ÓÊÕµ½ÁËÒ»ÅúÊı¾İ.
//[14:0]:½ÓÊÕµ½µÄÊı¾İ³¤¶È
u16 USART2_RX_STA=0;   	 
void USART2_IRQHandler(void)
{
	u8 res;	    
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//½ÓÊÕµ½Êı¾İ
	{	 
		res =USART_ReceiveData(USART2);				 
		if(USART2_RX_STA<USART2_MAX_RECV_LEN)		//»¹¿ÉÒÔ½ÓÊÕÊı¾İ
		{
		  TIM_SetCounter(TIM4,0);//¼ÆÊıÆ÷Çå¿Õ      
			if(USART2_RX_STA==0)TIM4_Set(1);	 	//Ê¹ÄÜ¶¨Ê±Æ÷4µÄÖĞ¶Ï 
			USART2_RX_BUF[USART2_RX_STA++]=res;		//¼ÇÂ¼½ÓÊÕµ½µÄÖµ	 
		}
		else 
		{
			USART2_RX_STA|=1<<15;					//Ç¿ÖÆ±ê¼Ç½ÓÊÕÍê³É
		} 
	}  											 
}   
//³õÊ¼»¯IO ´®¿Ú2
//pclk1:PCLK1Ê±ÖÓÆµÂÊ(Mhz)
//bound:²¨ÌØÂÊ	  
void USART2_Init(u32 bound)
{ 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOAÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

 	USART_DeInit(USART2);  //¸´Î»´®¿Ú2
		 //USART2_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
  GPIO_Init(GPIOA, &GPIO_InitStructure); //³õÊ¼»¯PA2
   
    //USART2_RX	  PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //³õÊ¼»¯PA3
	
	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎŞÆæÅ¼Ğ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
  
	USART_Init(USART2, &USART_InitStructure); //³õÊ¼»¯´®¿Ú	2
  
	//²¨ÌØÂÊÉèÖÃ
 //	USART2->BRR=(pclk1*1000000)/(bound);// ²¨ÌØÂÊÉèÖÃ	 
	//USART2->CR1|=0X200C;  	//1Î»Í£Ö¹,ÎŞĞ£ÑéÎ».
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  	//Ê¹ÄÜ´®¿Ú2µÄDMA·¢ËÍ
	UART_DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF);//DMA1Í¨µÀ7,ÍâÉèÎª´®¿Ú2,´æ´¢Æ÷ÎªUSART2_TX_BUF 
	USART_Cmd(USART2, ENABLE);                    //Ê¹ÄÜ´®¿Ú 
	
#ifdef USART2_RX_EN		  	//Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
	//Ê¹ÄÜ½ÓÊÕÖĞ¶Ï
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//¿ªÆôÖĞ¶Ï   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷
	TIM4_Init(99,7199);		//10msÖĞ¶Ï
	USART2_RX_STA=0;		//ÇåÁã
	TIM4_Set(0);			//¹Ø±Õ¶¨Ê±Æ÷4
#endif	 									  	
}
//´®¿Ú2,printf º¯Êı
//È·±£Ò»´Î·¢ËÍÊı¾İ²»³¬¹ıUSART2_MAX_SEND_LEN×Ö½Ú
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA1_Channel7->CNDTR!=0);	//µÈ´ıÍ¨µÀ7´«ÊäÍê³É   
	UART_DMA_Enable(DMA1_Channel7,strlen((const char*)USART2_TX_BUF)); 	//Í¨¹ıdma·¢ËÍ³öÈ¥
}
//¶¨Ê±Æ÷4ÖĞ¶Ï·şÎñ³ÌĞò		    
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//ÊÇ¸üĞÂÖĞ¶Ï
	{	 			   
		USART2_RX_STA|=1<<15;	//±ê¼Ç½ÓÊÕÍê³É
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //Çå³ıTIMx¸üĞÂÖĞ¶Ï±êÖ¾    
		TIM4_Set(0);			//¹Ø±ÕTIM4  
	}   
}
//ÉèÖÃTIM4µÄ¿ª¹Ø
//sta:0£¬¹Ø±Õ;1,¿ªÆô;
void TIM4_Set(u8 sta)
{
	if(sta)
	{       
		TIM_SetCounter(TIM4,0);//¼ÆÊıÆ÷Çå¿Õ
		TIM_Cmd(TIM4, ENABLE);  //Ê¹ÄÜTIMx	
	}else TIM_Cmd(TIM4, DISABLE);//¹Ø±Õ¶¨Ê±Æ÷4
}
//Í¨ÓÃ¶¨Ê±Æ÷ÖĞ¶Ï³õÊ¼»¯
//ÕâÀïÊ¼ÖÕÑ¡ÔñÎªAPB1µÄ2±¶£¬¶øAPB1Îª36M
//arr£º×Ô¶¯ÖØ×°Öµ¡£
//psc£ºÊ±ÖÓÔ¤·ÖÆµÊı		 
void TIM4_Init(u16 arr,u16 psc)
{	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //Ê±ÖÓÊ¹ÄÜ//TIM4Ê±ÖÓÊ¹ÄÜ    
	
	//¶¨Ê±Æ÷TIM3³õÊ¼»¯
	TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üĞÂÊÂ¼ş×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ıÊıµÄÔ¤·ÖÆµÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊıµ¥Î»
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //Ê¹ÄÜÖ¸¶¨µÄTIM4ÖĞ¶Ï,ÔÊĞí¸üĞÂÖĞ¶Ï
	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷	
}
#endif		 
///////////////////////////////////////USART2 DMA·¢ËÍÅäÖÃ²¿·Ö//////////////////////////////////	   		    
//DMA1µÄ¸÷Í¨µÀÅäÖÃ
//ÕâÀïµÄ´«ÊäĞÎÊ½ÊÇ¹Ì¶¨µÄ,ÕâµãÒª¸ù¾İ²»Í¬µÄÇé¿öÀ´ĞŞ¸Ä
//´Ó´æ´¢Æ÷->ÍâÉèÄ£Ê½/8Î»Êı¾İ¿í¶È/´æ´¢Æ÷ÔöÁ¿Ä£Ê½
//DMA_CHx:DMAÍ¨µÀCHx
//cpar:ÍâÉèµØÖ·
//cmar:´æ´¢Æ÷µØÖ·    
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
{
	DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//Ê¹ÄÜDMA´«Êä
    DMA_DeInit(DMA_CHx);   //½«DMAµÄÍ¨µÀ1¼Ä´æÆ÷ÖØÉèÎªÈ±Ê¡Öµ
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMAÍâÉèADC»ùµØÖ·
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMAÄÚ´æ»ùµØÖ·
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //Êı¾İ´«Êä·½Ïò£¬´ÓÄÚ´æ¶ÁÈ¡·¢ËÍµ½ÍâÉè
	DMA_InitStructure.DMA_BufferSize = 0;  //DMAÍ¨µÀµÄDMA»º´æµÄ´óĞ¡
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //ÍâÉèµØÖ·¼Ä´æÆ÷²»±ä
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //ÄÚ´æµØÖ·¼Ä´æÆ÷µİÔö
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //Êı¾İ¿í¶ÈÎª8Î»
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //Êı¾İ¿í¶ÈÎª8Î»
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //¹¤×÷ÔÚÕı³£»º´æÄ£Ê½
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAÍ¨µÀ xÓµÓĞÖĞÓÅÏÈ¼¶ 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAÍ¨µÀxÃ»ÓĞÉèÖÃÎªÄÚ´æµ½ÄÚ´æ´«Êä
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //¸ù¾İDMA_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯DMAµÄÍ¨µÀUSART1_Tx_DMA_ChannelËù±êÊ¶µÄ¼Ä´æÆ÷	
} 
//¿ªÆôÒ»´ÎDMA´«Êä
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
{
	DMA_Cmd(DMA_CHx, DISABLE );  //¹Ø±Õ Ö¸Ê¾µÄÍ¨µÀ        
	DMA_SetCurrDataCounter(DMA_CHx,len);//DMAÍ¨µÀµÄDMA»º´æµÄ´óĞ¡	
	DMA_Cmd(DMA_CHx, ENABLE);           //¿ªÆôDMA´«Êä
}

#ifdef USART3_RX_EN   								//Èç¹ûÊ¹ÄÜÁË½ÓÊÕ 

u16 USART3_RX_STA=0;
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//½ÓÊÕ»º³å,×î´óUSART3_MAX_RECV_LEN¸ö×Ö½Ú.
//ÕâÀïÖ»¼ìË÷RMC
void USART3_IRQHandler(void)
{	    
  u8 res;
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//½ÓÊÕµ½Êı¾İ
	{	 
		res =USART_ReceiveData(USART3);	
        if((USART3_RX_STA&0x8000)==0)
        {
            if(USART3_RX_STA<USART3_MAX_RECV_LEN)		//»¹¿ÉÒÔ½ÓÊÕÊı¾İ
            {
                USART3_RX_BUF[USART3_RX_STA]=res;		//¼ÇÂ¼½ÓÊÕµ½µÄÖµ
                if(USART3_RX_BUF[0]=='R')//¼ìË÷Êı¾İÍ·
                {
                    if(USART3_RX_STA>=2)
                    {
                        if((USART3_RX_BUF[1]=='M')&&(USART3_RX_BUF[2]=='C'))
                        {
                            if(USART3_RX_BUF[USART3_RX_STA]=='*')//½ÓÊÕÍê³É
                            {
                                USART3_RX_STA|=1<<15;
                                USART3_RX_STA++;
                                USART3_RX_BUF[USART3_RX_STA]=0;//Ìí¼Ó×Ö·û´®½áÊø·û
                            }
                            //USART3_RX_STA++;
                        }else
                        {
                            USART3_RX_STA=0;
                            return;
                        }
                    }
                    USART3_RX_STA++;
                }
            }
            else 
            {
                USART3_RX_STA|=1<<15;					//Ç¿ÖÆ±ê¼Ç½ÓÊÕÍê³É
            }
        }        
	}  											 
}  
#endif
//³õÊ¼»¯IO ´®¿Ú3
//pclk1:PCLK1Ê±ÖÓÆµÂÊ(Mhz)
//bound:²¨ÌØÂÊ	  
void USART3_Init(u32 bound)
{ 	
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOBÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

 	USART_DeInit(USART3);  //¸´Î»´®¿3
	//USART2_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
  GPIO_Init(GPIOB, &GPIO_InitStructure); //³õÊ¼»¯PB10
   
  //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //³õÊ¼»¯PB11
	
	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎŞÆæÅ¼Ğ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
  
	USART_Init(USART3, &USART_InitStructure); //³õÊ¼»¯´®¿Ú	3
  
#ifdef USART3_RX_EN		  	//Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
	//Ê¹ÄÜ½ÓÊÕÖĞ¶Ï
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//¿ªÆôÖĞ¶Ï   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷	
    USART3_RX_STA=0;		//ÇåÁã
#endif	
    USART_Cmd(USART3, ENABLE);                    //Ê¹ÄÜ´®¿Ú3
}

 /*·¢ËÍÒ»¸ö×Ö½ÚÊı¾İ*/
void UART3SendByte(unsigned char SendData)
{	   
		USART_SendData(USART3,SendData);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	    
} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 									 

#ifdef USART4_RX_EN   								//Èç¹ûÊ¹ÄÜÁË½ÓÊÕ

u16 USART4_RX_STA=0;
u8 USART4_RX_BUF[USART4_MAX_RECV_LEN]; 				//½ÓÊÕ»º³å,×î´óUSART3_MAX_RECV_LEN¸ö×Ö½Ú.
//ÕâÀïÖ»¼ìË÷RMC
void UART4_IRQHandler(void)
{	    
  u8 res;
  if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//½ÓÊÕµ½Êı¾İ
	{	 
		res =USART_ReceiveData(UART4);	
        if((USART4_RX_STA&0x8000)==0)
        {
            if(USART4_RX_STA<USART4_MAX_RECV_LEN)		//»¹¿ÉÒÔ½ÓÊÕÊı¾İ
            {
                USART4_RX_BUF[USART4_RX_STA]=res;		//¼ÇÂ¼½ÓÊÕµ½µÄÖµ
                if(USART4_RX_BUF[0]=='Z')//¼ìË÷Êı¾İÍ·
                {
                    if(USART4_RX_STA>=2)
                    {
                        if((USART4_RX_BUF[1]=='I')&&(USART4_RX_BUF[2]=='G'))
                        {
                            if(USART4_RX_BUF[USART4_RX_STA]=='d')//½ÓÊÕÍê³É
                            {
                                USART4_RX_STA|=1<<15;
                                USART4_RX_STA++;
                                USART4_RX_BUF[USART4_RX_STA]=0;//Ìí¼Ó×Ö·û´®½áÊø·û
                            }
                            //USART3_RX_STA++;
                        }else
                        {
                            USART4_RX_STA=0;
                            return;
                        }
                    }
                    USART4_RX_STA++;
                }
            }
            else 
            {
                USART4_RX_STA|=1<<15;					//Ç¿ÖÆ±ê¼Ç½ÓÊÕÍê³É
            }
        }        
	}  											 
}  
#endif

void USART4_Init(u32 bound)
{ 	
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	// GPIOBÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

 	USART_DeInit(UART4);  //¸´Î»´®¿3
	//USART2_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
  GPIO_Init(GPIOC, &GPIO_InitStructure); //³õÊ¼»¯PC2
   
  //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
  GPIO_Init(GPIOC, &GPIO_InitStructure);  //³õÊ¼»¯PC3
	
	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎŞÆæÅ¼Ğ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
  
	USART_Init(UART4, &USART_InitStructure); //³õÊ¼»¯´®¿Ú	3
  
#ifdef USART4_RX_EN		  	//Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
	//Ê¹ÄÜ½ÓÊÕÖĞ¶Ï
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//¿ªÆôÖĞ¶Ï   
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷	
    USART3_RX_STA=0;		//ÇåÁã
#endif
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE);                    //Ê¹ÄÜ´®¿Ú3
}

 /*·¢ËÍÒ»¸ö×Ö½ÚÊı¾İ*/
void UART4SendByte(unsigned char SendData)
{	   
		USART_SendData(UART4,SendData);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);	    
} 



















