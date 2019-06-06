#ifndef __KSJ_API_E_H__
#define __KSJ_API_E_H__

#pragma message("Include KSJApiE.h") 

#ifdef KSJAPI_EXPORTS
    #define KSJ_API __declspec(dllexport)
#elif defined KSJAPI_STATIC
	#define KSJ_API
#else
    #define KSJ_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"{
#endif

// 如下函数只适用于KSJ_UCxxxx_MRYN/MGYN型号相机，其中Y代表含义表示支持外触发和IO功能

// 得到外触发采集按键的状态，KSJ_UP表示按键没有按下，KSJ_DOWN表示按键按下
enum KSJ_SNAPBUTTONSTATUS { KSJ_UP, KSJ_DOWN };
KSJ_API  int __stdcall KSJ_SnapButtonStatus( int nChannel, KSJ_SNAPBUTTONSTATUS *pSnapButtonStatus );

// 设置GPIO引脚的输入输出状态，btDirection位寻址，BIT0对应GPIO0, BIT1对应GPIO1, BIT2对应GPIO2, BIT3对应GPIO3(=1表示此GPIO输出，=0表示此GPIO输入)
KSJ_API  int __stdcall KSJ_GpioSetDirection( int nChannel, unsigned char btDirection );
// 获取GPIO引脚的输入输出状态，pbtDirection位寻址，BIT0对应GPIO0, BIT1对应GPIO1, BIT2对应GPIO2, BIT3对应GPIO3(=1表示此GPIO输出，=0表示此GPIO输入)
KSJ_API  int __stdcall KSJ_GpioGetDirection( int nChannel, unsigned char *pbtDirection );

// 控制GPIO引脚输出，只有btDirection相应位设置为1的引脚才能通过此函数控制引脚输出。btGpioStatus位寻址，BIT0对应GPIO0, BIT1对应GPIO1, BIT2对应GPIO2, BIT3对应GPIO3(=1表示此GPIO输出TTL高电平，=0表示此GPIO输出TTL低电平)
KSJ_API  int __stdcall KSJ_GpioSetStatus   ( int nChannel, unsigned char btGpioStatus );
// 读取GPIO引脚状态，*pbtGpioStatus位寻址，BIT0对应GPIO0, BIT1对应GPIO1, BIT2对应GPIO2, BIT3对应GPIO3(=1表示此GPIO输出TTL高电平，=0表示此GPIO输出TTL低电平)
KSJ_API  int __stdcall KSJ_GpioGetStatus   ( int nChannel, unsigned char *pbtGpioStatus );




#ifdef __cplusplus
}
#endif

#endif