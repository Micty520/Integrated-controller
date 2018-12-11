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
    for(j = 0; j < 255; j++)
    {
        for (i = 0; i < 10000; i++)
		{
            ExtWDG_Feed();
            USBH_Process(&USB_OTG_Core, &USB_Host);
			
		}
    }

    for (j = 0; j < 255; j++)
    {

        delay_ms(10);
        // asm("wdr");
        IWDG_Feed();
        ExtWDG_Feed();
        if (MCLK__0)
        {
            spi_da[0] = get_spi();
            spi_da[1] = get_spi();
            spi_da[2] = get_spi();
            if (spi_da[0] == 0xfa)
            {
                if (spi_da[1] == 0xf5)
                {
                    if (spi_da[2] == 0x99)
                    {
                        gbSoundState = 1;
                        EXTI_ENABLE();
                        RestartFlg = 1;

                        goto bootend;
                    }
                    else if (spi_da[2] == 0x98)
                    {
                        RestartFlg = 2;
                        goto bootend;
                    }
                }
                else
                {
                    ; //goto progstart;
                }
            }
            else
            {
                ; //goto progstart;
            }
        }
    }
bootend:
    // asm("wdr");

    IWDG_Feed();
    ExtWDG_Feed();
    STMFLASH_Read(0x1FFFF804, (u16 *)&SetRemoteFlg, 1);
    if ((SetRemoteFlg & 0x00FF) == 0X00ff) // 做初始数据
    {
        for (j = 0; j < 40; j++)
        {
            Array[j] = Array0[j];
        }
        for (j = 0; j < 16; j++)
        {
            cMemStep[j] = Array[j + 20];
        }
        enWrite = 0x55;
        WriteBootByte();

        // SetRemoteFlg = 0x5555;
        FLASH_Unlock();
        FLASH_EraseOptionBytes();
        FLASH_ProgramOptionByteData(0x1FFFF804, 0x55);
        FLASH_Lock();
        // STMFLASH_Write(EEPROM_Addr, (u16 *)&SetRemoteFlg, 1);
        enWrite = 0;
        goto movestart;
    }

    // asm("CLI");
    SPI_Flash_Read(LIGHT_CONFIG_ADDR << 3, Array); //读取到ARRAY
movestart:
    //读每个场景步数值
    for (j = 0; j < 16; j++)
    {
        cMemStep[j] = Array[j + 20];
    }
    enMove = Array[0] & 0xf0;
    enMove >>= 4;
    if ( ((Array[18] & 0x0f) == 0x0A) || ((Array[36] & 0x0f) == 0x0A) ) // 墙板类型==其他
    {
        RestartFlg0 = 3;
    }
    if ((Array[17] & 0x0f) == 0x0A) // 点歌码类型==其他
    {
        RestartFlg1 = 3;
    }
    tSoundScensFlag = Array[37] & 0x01; //节奏跳转标志
    if (RestartFlg == 0)
    {
        RestartFlg = 0xff;
        /*   STMFLASH_Read(EEPROM_Addr + 18 * 2, (u16 *)&gbSoundState, 1);
        if (gbSoundState == 0)
        {
            // EIMSK = 0;
        }
        else
        {
            STMFLASH_Read(EEPROM_Addr + 19 * 2, (u16 *)&gbyScenscc, 1);
            STMFLASH_Read(EEPROM_Addr + 20 * 2, (u16 *)&cMemStep1, 1);
            ProcessSesData(gbyScenscc);
            // EIMSK = 4;
        }*/
        goto progstart; //if(gbSoundState==1)
    }
    else
    {
        gbyScenscc = Array[0] & 0x0f;
        ProcessSesData(gbyScenscc);
        if (Array[(gbyScenscc + 1) & 0x0f] == 0x0c) // 默认场景是声控模式
        {
            cMemStep1 = Array[gbyScenscc + 20];
            gbSoundState = 1;
            // SPI_Flash_Write(&gbyScenscc, 1, 1);
            // SPI_Flash_Write(&cMemStep1, 3, 1);
        }
    }
    enSend = 1;
    send_spi(0xde, 0xae);
    delay_ms(1);
    for (j = 0; j < 20; j++)
    {
        enSend = 1;
        send_spi(Array[j * 2], Array[j * 2 + 1]);
        delay_ms(1);
    }

    delay_ms(20);
    SPI_Flash_Read(IR_DATA_ADDR << 3, gbyRemote);
    for (j = 0; j < 22; j++)
    {
        // D3,0X,...22个字节 ———— 红外码发送
        enSend = 1;
        send_spi(0xd3, j);
        for (i = 0; i < 11; i++)
        {
            enSend = 1;
            send_spi(gbyRemote[i * 2 + j * 22], gbyRemote[i * 2 + j * 22 + 1]);
            delay_us(1);
        }
        delay_ms(5);
        RestartFlg = 3;
        // asm("wdr");
        IWDG_Feed();
        ExtWDG_Feed();
    }
progstart:
    temp1 = 0;
    dmxhead = 0xff;
    delay_ms(10);
    gbDmx = 0;
    gbDmxState = 0;
    gbStartSend = 0;
    kgbyRunStep = 40;
    gbyLight = 17;
    enFlash = 0;
    I2CsignOK = 0;
    enCopy = 0;
    fankuitime = 0;
    delay_ms(100);
    /*****预留******/
    /***初始化串口***/
    DOMC_OFF;
    gbyRunStep = 0;
    LampType = 1;
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
   
 
        
	
}


