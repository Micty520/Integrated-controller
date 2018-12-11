#ifndef __SPI_DMX_H
#define __SPI_DMX_H
#include "sys.h"

#define MCLK_OUT                			   \
    {                                          \
        GPIOB->CRL &= 0X0FFFFFFF;              \
        GPIOB->CRL |= ((unsigned int)3 << 28); \
    }
#define MCLK_IN                 			   \
    {                                          \
        GPIOB->CRL &= 0X0FFFFFFF;              \
        GPIOB->CRL |= ((unsigned int)8 << 28); \
    }
#define MCLK__1 PBin(7) //输入
#define MCLK__0 !PBin(7)
#define MCLK_1 PBout(7) = 1 //输出
#define MCLK_0 PBout(7) = 0

#define SDA_OUT                   \
    {                             \
        GPIOB->CRL &= 0XF0FFFFFF; \
        GPIOB->CRL |= 3 << 24;    \
    }
#define SDA_IN                    \
    {                             \
        GPIOB->CRL &= 0XF0FFFFFF; \
        GPIOB->CRL |= 8 << 24;    \
    }
#define SDA__1 PBin(6)     //输入
#define SDA_1 PBout(6) = 1 //输出
#define SDA_0 PBout(6) = 0 //输出

#define SCLK_OUT                  \
	{                             \
        GPIOB->CRL &= 0XFF0FFFFF; \
        GPIOB->CRL |= 3 << 20;    \
    }
    
#define SCLK_IN                   \
	{                             \
        GPIOB->CRL &= 0XFF0FFFFF; \
        GPIOB->CRL |= 8 << 20;    \
    }
    
#define SCLK__1 PBin(5) //输入
#define SCLK__0 !PBin(5)
#define SCLK_1 PBout(5) = 1 //输出
#define SCLK_0 PBout(5) = 0

#define uchar u8
#define uint u16

void SPI_DmxInit(void);
void send_spi(uchar ch1, uchar ch2);
uchar get_spi(void);

#endif
