#ifndef __USART_H
#define __USART_H
#include "sys.h"

extern uint8_t gbyDmxData[513];

void USRAT2_IO_Init(unsigned char Set);
void USRAT3_IO_Init(unsigned char Set);
void USRAT_NVIC_Init(void);
void USART_BAUD_Init(USART_TypeDef *USARTx, u32 baud, u8 WordLength);
void MYDMA_Config(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u16 cndtr);
void DMX_TXD_Init(void);
void DMX_RXD_Init(void);
void DMX_Send(void);
void SendCheck1(unsigned data);
void SendI2CData(unsigned char Scens, unsigned char RunStep, unsigned char *DmxData);
void SendDoubData(unsigned char gbyScens, unsigned char gwStep0, unsigned char gwStep1, unsigned char *gbyRemote);
void SendConCheck1(unsigned char by);
void SendHongwaiEnd(void);
void SendOneHongwaima(unsigned char *hongwaima, unsigned char check);
void SendConLinkFeed(unsigned char const *conlinkfeed);
void SendReData(unsigned char *redata, unsigned char len);
void SendStepData(unsigned char *step);

#endif

