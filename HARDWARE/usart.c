#include "usart.h"
#include "delay.h"


void USRAT1_IO_Init(void)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);          //初始化GPIOA.9

    //USART1_RX	  GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void USRAT2_IO_Init(unsigned char Set)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //USART2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2 -- TXD  PA3 -- RXD
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    if (0 == Set)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

void USRAT3_IO_Init(unsigned char Set)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //USART3_TX	  GPIOB.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    if (0 == Set)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIOB.10
}

void USRAT_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
}

void USART_BAUD_Init(USART_TypeDef *USARTx, u32 baud, u8 WordLength)
{
    USART_InitTypeDef USART_InitStructure;

    USART_DeInit(USARTx); //复位串口
    USART_Cmd(USARTx, DISABLE);

    USART_InitStructure.USART_BaudRate = baud; //串口波特率
    if (WordLength == 8)
        USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    else
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式

    USART_Init(USARTx, &USART_InitStructure);

    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    USART_Cmd(USARTx, ENABLE);
    USART_ClearFlag(USARTx, USART_FLAG_TC); //清空发送完成标志，防止丢失第一字节数据
}

unsigned int DMA1_MEM_LEN;
void MYDMA_Config(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);
    DMA1_MEM_LEN = cndtr;
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = cndtr;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA_CHx, &DMA_InitStructure);
}
// unsigned int SendDmxData[514];
void DMX_TXD_Init(void)
{
    USRAT3_IO_Init(1);
    USART_BAUD_Init(USART3, 250000, 9);
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
 /*   MYDMA_Config(DMA1_Channel2, (u32)&USART3->DR, (u32)gbyDmxData, 513);
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    DMA_SetCurrDataCounter(DMA1_Channel2, DMA1_MEM_LEN);
    DMA_Cmd(DMA1_Channel2, ENABLE);
    // DMA_Cmd(DMA1_Channel2, ENABLE);*/
}

void DMX_RXD_Init(void)
{
    USRAT2_IO_Init(1);
    USRAT1_IO_Init();
    USART_BAUD_Init(USART2, 250000, 9);
    USART_BAUD_Init(USART1, 256000, 8);
    USRAT_NVIC_Init();
}

void DMX_Send(void)
{
    u16 i;
 /*   if (DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET)
    {
        DMA_ClearFlag(DMA1_FLAG_TC2);
      //  SendDmxData[0] = 0;
        for (i = 513; i > 0; i--)
        {
            gbyDmxData[i] = gbyDmxData[i - 1] | 0x0100;
        }
        gbyDmxData[0] = 0;*/
        GPIO_SetBits(GPIOA, GPIO_Pin_6);    //485发送状态
        USRAT3_IO_Init(0);                  //切换为普通IO
        GPIO_ResetBits(GPIOB, GPIO_Pin_10); //TXD拉低
        delay_us(200);
        GPIO_SetBits(GPIOB, GPIO_Pin_10); //TXD置1
        USRAT3_IO_Init(1);
				delay_us(1);
        USART3->DR = 0x0100;
        while (!(USART3->SR & (1 << 6)))
            ;
        delay_us(5);
        for (i = 0; i < 512; i++)
        {
            USART3->DR = (uint16_t)0x0100 | gbyDmxData[i];
            while (!(USART3->SR & (1 << 6)))
                ;
            delay_us(5);
        }
        GPIO_ResetBits(GPIOA, GPIO_Pin_6);    //485发送状态*/
      /*  DMA_Cmd(DMA1_Channel2, DISABLE);
        DMA_SetCurrDataCounter(DMA1_Channel2, DMA1_MEM_LEN);
        DMA_Cmd(DMA1_Channel2, ENABLE);*/
        // }
}

void SendCheck1(unsigned data)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_5);   //dmx接收口拉高
    USRAT2_IO_Init(0);                 //切换为普通IO
    GPIO_ResetBits(GPIOA, GPIO_Pin_2); //TXD拉低
    delay_us(200);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    delay_us(1);
    USRAT2_IO_Init(1); //250Kbps 9位数据 1位停止
    USART2->DR = 0x01fc;
    while (!(USART2->SR & (1 << 6)))
        ;
    delay_us(5);
    USART2->DR = data;
    while (!(USART2->SR & (1 << 6)))
        ;
    GPIO_ResetBits(GPIOA, GPIO_Pin_5); //dmx接收口拉低
}

void SendI2CData(unsigned char Scens, unsigned char RunStep, unsigned char *DmxData)
{
    unsigned char check;
    unsigned int i;
    GPIO_SetBits(GPIOA, GPIO_Pin_5);   //dmx接收口拉高
    USRAT2_IO_Init(0);                 //切换为普通IO
    GPIO_ResetBits(GPIOA, GPIO_Pin_2); //TXD拉低
    delay_us(200);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    delay_us(1);
    USRAT2_IO_Init(1); //250Kbps 9位数据 1位停止
    USART2->DR = 0x01fa;
    while (!(USART2->SR & (1 << 6)))
        ;
    check = 0;
    USART2->DR = Scens;
    if (Scens == 16)
    {
        DmxData[17] &= 0x0f;
    }
    while (!(USART2->SR & (1 << 6)))
        ;
    check += Scens;
    USART2->DR = RunStep;
    while (!(USART2->SR & (1 << 6)))
        ;
    check += RunStep;
    for (i = 0; i < 512; i++)
    {
        USART2->DR = DmxData[i];
        while (!(USART2->SR & (1 << 6)))
            ; // 等待发送缓冲空
        check += DmxData[i];
    }
    if (Scens > 15)
        check++;
    USART2->DR = check;
    while (!(USART2->SR & (1 << 6)))
        ; // 等待发送缓冲空，UDRE变为1
    delay_us(60);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5); //TXD拉低
}

void SendDoubData(unsigned char gbyScens, unsigned char gwStep0, unsigned char gwStep1, unsigned char *gbyRemote)
{
    unsigned char gbyCheck;
    unsigned int i;
    GPIO_SetBits(GPIOA, GPIO_Pin_5);   //dmx接收口拉高
    USRAT2_IO_Init(0);                 //切换为普通IO
    GPIO_ResetBits(GPIOA, GPIO_Pin_2); //TXD拉低
    delay_us(200);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    delay_us(1);
    USRAT2_IO_Init(1); //250Kbps 9位数据 1位停止
    USART2->DR = 0x01f9;
    while (!(USART2->SR & (1 << 6)))
        ;
    gbyCheck = 0;
    USART2->DR = gbyScens;
    while (!(USART2->SR & (1 << 6)))
       ; // 等待发送缓冲空，UDRE变为1
    gbyCheck += gbyScens;
    USART2->DR = gwStep0;
    while (!(USART2->SR & (1 << 6)))
       ; // 等待发送缓冲空，UDRE变为1
    gbyCheck += gwStep0;
    if (gwStep1 == 0)
    {
        USART2->DR = 0xf1;
        while (!(USART2->SR & (1 << 6)))
            ; // 等待发送缓冲空，UDRE变为1
        gbyCheck += 0xf1;
        for (i = 0; i < 1024; i++)
        {
            USART2->DR = gbyRemote[i];
            while (!(USART2->SR & (1 << 6)))
                ; // 等待发送缓冲空，UDRE变为1
            gbyCheck += gbyRemote[i];
        }
    }
    
    else
    {
        USART2->DR = gwStep1;
        while (!(USART2->SR & (1 << 6)))
            ; // 等待发送缓冲空，UDRE变为1
        gbyCheck += gwStep1;

        for (i = 0; i < 512; i++)
        {
            USART2->DR = gbyRemote[i];
            while (!(USART2->SR & (1 << 6)))
                ; // 等待发送缓冲空，UDRE变为1
            gbyCheck += gbyRemote[i];
        }
    }

    USART2->DR = gbyCheck;
    while (!(USART2->SR & (1 << 6)))
        ; // 等待发送缓冲空，UDRE变为1
    delay_us(60);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5); //TXD拉低
}

void SendConCheck1(unsigned char by)
{
    USART_BAUD_Init(USART2, 115200, 8);
    PAout(5) = 1;
    delay_us(5);
    USART1->DR = by;
    USART2->DR = by;
    while (!(USART2->SR & (1 << 6)))
        ; // 等待发送缓冲空
    delay_us(60);
    USART_BAUD_Init(USART2, 250000, 9);
    PAout(5) = 0;
}

void SendHongwaiEnd(void)
{
    unsigned char i;
    USART_BAUD_Init(USART2, 115200, 8);
    PAout(5) = 1;
    delay_us(5);
    for (i = 0; i < 25; i++)
    {
        USART1->DR = 0xf7;
        USART2->DR = 0xf7;
        while (!(USART2->SR & (1 << 6)))
            ; // 等待发送缓冲空
    }
    delay_us(60);
    USART_BAUD_Init(USART2, 250000, 9);
    PAout(5) = 0;
}

void SendOneHongwaima(unsigned char *hongwaima, unsigned char check)
{

    unsigned char i;
    USART_BAUD_Init(USART2, 115200, 8);
    PAout(5) = 1;
    delay_us(5);
    for (i = 0; i < 24; i++)
    {
        USART1->DR = hongwaima[i];
        USART2->DR = hongwaima[i];
        while (!(USART2->SR & (1 << 6)))
            ; // 等待发送缓冲空
    }
    USART2->DR = check;
    while (!(USART2->SR & (1 << 6)))
        ; // 等待发送缓冲空
    delay_us(60);
    USART_BAUD_Init(USART2, 250000, 9);
    PAout(5) = 0;
}

void SendConLinkFeed(unsigned char const *conlinkfeed)
{
    unsigned char i;
    USART_BAUD_Init(USART2, 115200, 8);
    PAout(5) = 1;
    delay_us(10);
    for (i = 0; i < 9; i++)
    {
        USART1->DR = conlinkfeed[i];
        USART2->DR = conlinkfeed[i];
        while (!(USART2->SR & (1 << 6)))
            ; // 等待发送缓冲空
    }
    delay_us(60);
    USART_BAUD_Init(USART2, 250000, 9);
    PAout(5) = 0;
}

void SendReData(unsigned char *redata, unsigned char len)
{
    unsigned char i;
    USART_BAUD_Init(USART2, 115200, 8);
    PAout(5) = 1;
    delay_us(5);
    for (i = 0; i < len; i++)
    {
        USART2->DR = redata[i];
        while (!(USART2->SR & (1 << 6)))
            ; // 等待发送缓冲空
    }
    delay_us(60);
    USART_BAUD_Init(USART2, 250000, 9);
    PAout(5) = 0;
}

void SendStepData(unsigned char *step)
{
    unsigned char check,i;
    GPIO_SetBits(GPIOA, GPIO_Pin_5);   //dmx接收口拉高
    USRAT2_IO_Init(0);                 //切换为普通IO
    GPIO_ResetBits(GPIOA, GPIO_Pin_2); //TXD拉低
    delay_us(200);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    delay_us(1);
    USRAT2_IO_Init(1); //250Kbps 9位数据 1位停止
    USART2->DR = 0x01fb;
    check = 0;
    while (!(USART2->SR & (1 << 6)))
        ; // 等待发送缓冲空
    for (i = 0; i < 16; i++)
    {
        USART2->DR = step[i];
        while (!(USART2->SR & (1 << 6)))
            ; // 等待发送缓冲空
        check += step[i];
    }
    USART2->DR = check;
    while (!(USART2->SR & (1 << 6)))
        ; // 等待发送缓冲空
    delay_us(60);
    // USART_BAUD_Init(USART2, 250000, 9);
    PAout(5) = 0;
}

