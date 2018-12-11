#include "spi_dmx.h"
#include "delay.h"

void SPI_DmxInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  //根据设定参数初始化GPIOA.5
	// GPIO_SetBits(GPIOB, GPIO_Pin_5);				  // 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  //根据设定参数初始化GPIOA.5
	GPIO_SetBits(GPIOB, GPIO_Pin_6);				  //输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  //根据设定参数初始化GPIOA.5
	GPIO_SetBits(GPIOB, GPIO_Pin_5);				  //输出高
}

extern void Delay(u32 count);

extern u8 enSend;
void send_spi(uchar ch1, uchar ch2)
{
	uchar i;
	//	asm("cli");
	if (enSend == 0)
		return;
	if (MCLK__0)
		return; //while(MCLK__0);

	INTX_DISABLE();
	MCLK_OUT;
	SCLK_IN;
	SDA_OUT;
	SCLK_1;
	MCLK_0; //申请通讯
	while (SCLK__0)
		;
	for (i = 0; i < 8; i++)
	{
		while (SCLK__1)
			; //检测从机响应

		if (ch1 & 1)
		{
			SDA_1; //放一个bit到SDA
		}
		else
		{
			SDA_0;
		}
		ch1 >>= 1;

		while (SCLK__0)
			;
	}
	for (i = 0; i < 8; i++)
	{

		while (SCLK__1)
			; //检测从机响应
		if (ch2 & 1)
		{
			SDA_1; //放一个bit到SDA
		}
		else
		{
			SDA_0;
		}
		ch2 >>= 1;
		while (SCLK__0)
			;
	}
	/*	for (i = 0; i < 8; i++)
	{
		while (SCLK__1)
			; //检测从机响应

		if (ch3 & 1)
		{
			SDA_1; //放一个bit到SDA
		}
		else
		{
			SDA_0;
		}
		ch3 >>= 1;
		while (SCLK__0)
			;
	}*/
	MCLK_1;
	SCLK_1;
	MCLK_IN;
	SDA_1;
	SDA_IN;
	enSend = 0;
	INTX_ENABLE();
}

//    for(i=4;i!=0;i--)
//    {
//        while(SCLK__1); //检测从机响应
//    	if(ch&1)
//    	{
//        	SDA_1;   //放一个bit到SDA
//        }
//        else
//        {
//        	SDA_0;
//        }
//        MCLK_1;
//        ch=ch>>1; //准备下一个bit
//
//        while(SCLK__0); //检测从机响应
//        if(ch&1)
//        {
//        	SDA_1;
//        }
//        else
//        {
//        	SDA_0;
//        }
//        MCLK_0;
//        ch=ch>>1; //准备下一个bit
//    }
//    while(SCLK__1); //检测从机收完
//    MCLK_1;
//    while(SCLK__0);//等待从机准备好
//    SDA_1;
//}

//接收一个数据
//调用之前最好检测MCLK是否为0（对方是否申请数据通讯）再进入,否则里面是死等此信号!
uchar get_spi(void)
{
	uchar i;
	uchar Data_buf = 0;
	// INTX_DISABLE();
	SCLK_OUT;
	SCLK_1;
	MCLK_IN;
	SDA_IN;
	Delay(10);
	while (MCLK__1)
	{
		//asm ("wdr");
	}
	Delay(10);
	for (i = 0; i < 8; i++)
	{
		SCLK_0;					  //接受通讯请求
		Data_buf = Data_buf >> 1; //准备收下一个bit
		Delay(10);
		if (SDA__1)
		{
			Data_buf |= 0x80;
		}
		Delay(10);
		SCLK_1;
		Delay(10);
	}
	MCLK_IN;
	// INTX_ENABLE();
	return (Data_buf);
}
