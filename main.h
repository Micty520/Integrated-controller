#include "stm32f10x.h"

#define uchar u8
#define uint u16
#define ulong u32

//全局变量定义

uchar tUsbFlag, tSoundScensFlag;
uchar gbySpeed[16];
uchar gbyLight;
uchar MazhiType, QbType;
uchar gbyScens, gbyChannel, pTemp;
uchar fFlick, fTimer, gbytimernum, timesup;
uchar byData, gbJidianqi;
uchar gbyCheck, gbyFan;
uchar dmxhead, Dmx512sign, I2CsignOK;
uchar gbyCopyMode, gbTestMode, enCopy;
uchar gbTimer, gbShkong, gbShkongkk, gbStartover;
ulong timercount, timercount1, overnum, overnum1, testovernum;
uchar ReData[5];
uchar disnum1, disnum2, disnum3, disnum4;
uchar tnum1, tnum2, tnum3, tnum4;
uint keynum, shkongcount, shkongdu, enflashnum, fankuitime;
uchar gbyHead, gbyMaLength;
uchar gbyMenuValue, gbSoundState;
uchar kgbyRunStep = 51, gbyRunStep = 1;
uchar spi_da[5];
uchar tHongwaima[25];
uchar enSendKg, enSend, enSendJdq, enShkong, enFlash, enSendHongwai;
uchar Databuf1, Databuf2, Databuf3, Databuf4;
uchar temp1, gbyFlash, flashnum;
uint gbyDmxSerial, kongtainum, kongtaisp;
uchar DeviceID, gwStep0, gwStep1;
uchar bLinkMode;
uchar gbDmx, gbLearn, gbDmxState, gbStartSend;
uchar gbyDmxData[513];
uchar oldDmxData[12];
uchar gbyNumOK;
uchar cMemStep[16];
uchar gbMaxStepFlg[16];
uchar defScens;
uchar imgScens[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uchar gbyKongTiao;
uchar MazhiType;
uchar QbType;
uchar cMemStep1, gbyScenscc;
uchar gbyRemote[1024];
uint indexj;
uchar RestartFlg, RestartFlg0, RestartFlg1;
uchar LampType, LampTypeNum;
uchar enMove, enDipin;
uint SetRemoteFlg;
uchar KongtianType;
uchar LenQBma, LenDGma;
uchar RateQB, RateDG;
uchar TestRateQB, TestRateDG;
uchar gbyRedRfType;
uchar enWrite, enCount;
uint WriteCount, MusicTimes;
uchar EnReadDoub;

// 文件相关定义
uchar gbyErrType;
uchar gbyCardState;
uchar gbySelectedCard;
ulong SegStartAddr[24];
uchar SesStepSum;
uchar SesStepTime[512];
uchar SesStepData[512];
uchar SesStepNum;
uint TimesCnt;
uint SesChannelSum;
uint SesChannelValue[512]; // 最多410
uchar MaxDoubSum;
uchar steps, Maxsteps;
uchar tID[3];
uchar tbuf[1024];

uchar gbMusic,gbMin,gbMout;
uchar LearnInTimes,LearnOutTimes;

unsigned char gbStartScens;
unsigned char gbyPengcnt;
unsigned char tempscens;

uchar Array[512];

//unsigned long const SegStartAddr0[24] = {1, 16, 10, 5, 10, 5, 10, 2, 2, 10, 2, 10, 4, 10, 10, 10, 10};
unsigned long const SegStartAddr0[24] = {0x0001, 0x007a, 0x00cb, 0x00ea, 0x013d, 0x015B, 0x01ab, 0x01b6, 0x01c1, 0x0212, 0x021d, 0x026e, 0x0283, 0x02d4, 0x0325, 0x0376, 0x03c7, 0x03c8, 0x03c9, 0x03cd, 0x03ca, 0x03cb, 0x03cc, 0x03e0};
//121，81， 31， 83， 30， 80， 11， 22， 81， 11， 81, 21, 81, 81, 81, 81, 1, 1, 1
const unsigned char Array0[40] = {0x27, 0x1C, 12, 12, 12, 0x1C, 12, 2, 3, 0x1C, 2, 12, 3, 12, 12, 12, 12, 0x31, 1, 0, 120, 80, 1, 80, 30, 80, 1, 2, 80, 1, 80, 4, 80, 80, 80, 1, 1, 0, 0, 0};
//       		摇头灯参数启用+默认场景			     系统码，墙板码，空调使能
unsigned char const LightClass[18] = {0, 1, 2, 3, 5, 10, 13, 18, 22, 28, 36, 46, 61, 80, 105, 140, 190, 255};
unsigned char const ConlinkData[9] = {0xf5, 0xaa, 0x48, 0x54, 0x4B, 0x6B, 0x4A, 0x43, 0x4F};
unsigned char const StepsIndex[4] = {0xfd, 0xfe, 0x00, 0x01}; //     参数， 备份， 红外， 摇头， 墙板， 系统， 配置,  叠加
unsigned char const ConlinkFeed[9] = {0xf5, 0xaa, 0x42, 0x5A, 0x2d, 0x32, 0x33, 0x38, 0x20}; //控制器型号（BZ-208 ）
uchar const MaxSteps[16] = {120, 80, 30, 80, 30, 80, 10, 10, 80, 10, 80, 20, 80, 80, 80, 80};
//121，81， 31， 83， 30， 80， 11， 22， 81， 11， 81

unsigned long const SegStartAddr1[24] = {
    0x01,
    0xF3,
    0x195,
    0x1D3,
    0x279, 
    0x2B5,
    0x355,
    0x36B,
    0x381,
    0x423,
    0x439,
    0x4DB,
    0x505,
    0x5A8,
    0x649,
    0x6EB};

#define RedRfType gbyRemote[487]
/*
// 存储器地址规划
// 0x00000-0x78E0F  灯光数据
#define LIGHT_CONFIG_ADDR 0x90000 //灯控参数
#define BAK_CONFIG_ADDR 0x90200   //灯控备份参数
#define IR_DATA_ADDR 0x90400      //红外数据
#define PC_QBMA_ADDR 0x90600      //其他墙板码值
#define PC_DGMA_ADDR 0x90800      //其他点歌码值
#define LIGHT_INF_ADDR 0x90A00    //配置灯具信息
#define MOVE_DATA_ADDR 0x90C00    //...0x7ba00

#define DOUB_SCENS_HEAD 0x7C000 //叠加场景文件头起始地址
#define DOUB_SCENS_MARK 0x7C200 //叠加每个场景标志地址 c200 c400 c600 c800 ca00 cc00 ce00 d000 d200 d400 d600 d800 da00 dc00 de00 e000
#define DOUB_SCENS_TIME 0x7E200 //叠加每个场景每步时间 e200 e400 e600 e800 ea00 ec00 ee00 f000 f200 f400 f600 f800 fa00 fc00 fe00 0000

#define DOUB_SCENS_DATA 0x100000 //叠加场景数据开始地址
*/

#define LIGHT_CONFIG_ADDR 0x79000 //灯控参数
#define BAK_CONFIG_ADDR 0x79200   //灯控备份参数
#define IR_DATA_ADDR 0x79400      //红外数据
#define PC_QBMA_ADDR 0x79600      //其他墙板码值
#define PC_DGMA_ADDR 0x79800      //其他点歌码值
#define LIGHT_INF_ADDR 0x79A00    //配置灯具信息
#define MOVE_DATA_ADDR 0x79C00    //...0x7ba00

#define DOUB_SCENS_HEAD 0x7C000 //叠加场景文件头起始地址
#define DOUB_SCENS_MARK 0x7C200 //叠加每个场景标志地址 c200 c400 c600 c800 ca00 cc00 ce00 d000 d200 d400 d600 d800 da00 dc00 de00 e000
#define DOUB_SCENS_TIME 0x7E200 //叠加每个场景每步时间 e200 e400 e600 e800 ea00 ec00 ee00 f000 f200 f400 f600 f800 fa00 fc00 fe00 0000
#define DOUB_SCENS_DATA 0x80200 //叠加场景数据开始地址


#define SES_FILE_HEAD 0XFE
#define SES_DOUB_ENID 0XFE
#define SES_STEP_TIME 0XFD

#define DOMC_OFF PAout(4) = 0
#define DOMC_ON PAout(4) = 1
#define EnableTX PAout(6) = 1
#define EnableRX PAout(6) = 0
#define EnableTX1 PAout(5) = 1
#define EnableRX1 PAout(5) = 0

#define H_AD__0		!PAin(15) 
#define H_AD__1		PAin(15)

#define LEARN_IN__0 !PBin(1)
#define LEARN_IN__1 PBin(1)
