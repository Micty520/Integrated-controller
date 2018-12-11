#include "main.h"
#include "spi.h"
#include "flash.h"
#include "spi_dmx.h"
#include "delay.h"
#include "stmflash.h"
#include "usart.h"
#include "usbh_usr.h"
#include "malloc.h"
#include "exfuns.h"


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

void Delay(u32 count)
{
    u32 i = 0;
    for (; i < count; i++)
        ;
}

int main(void)
{
    uchar by;
    uint i, j;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); //io时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //选择使用优先级组第三组
    delay_init();
    EN485_IoInit();
    HLAD_Init();
    SPI_DmxInit();
    SPI_Flash_Init();
    DMX_TXD_Init();
    DMX_RXD_Init();
    Time_Init();
    IWDG_Init(4, 1025);
	ReadFileHead();
    EXT_WdgInit();
    mem_init();    //初始化内存池
    exfuns_init(); //为fatfs相关变量申请内存
 //   tUsbFlag = 0x11;
    USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_cb); //初始化USB主机

   
 
        
	
}


