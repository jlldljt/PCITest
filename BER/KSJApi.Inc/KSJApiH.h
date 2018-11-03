#ifndef __KSJ_API_H_H__
#define __KSJ_API_H_H__

#pragma message("Include KSJApiH.h") 


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

// 得到sensor可以输出的比特位数，用于识别16位传输数据的有效位数及查找表的大小。
KSJ_API  int __stdcall KSJ_GetADCResolution( int nChannel, int *pnADCResolution );

enum KSJ_FUNCTION 
{
	KSJ_PROPERTY_MONO_DEVICE,           // 是否为黑白相机，     0 - 是；     1 - 不是
	KSJ_PROPERTY_ADC_RESOLUTION,        // 相机ADC的转换比特位数，8，10，12，与KSJ_GetADCResolution功能相同
	KSJ_SUPPORT_RESET_DEVICE,           // 是否支持复位设备，   0 - 不支持； 1 - 支持
	KSJ_SUPPORT_EMPTY_FRAMEBUFFER,      // 是否支持清空帧存。0 - 不支持； 1 - 支持
	KSJ_SUPPORT_LUT,                    // 是否支持硬件查找表（下载并使能）， 0 - 不支持； 1 - 支持
	KSJ_SUPPORT_16BITS,			        // 是否支持双字节传输，一个像素用2个字节，数据读取加倍。0 - 不支持，1 - 支持
	KSJ_SUPPORT_MULTI_FRAMES,	        // 是否支持一次读取多帧数据进行垂直拼图。0 - 不支持，1 - 支持
	KSJ_SUPPORT_FIXED_FRAMERATE,	    // 是否支持通过设置sensor固定帧率。0 - 不支持，1 - 支持
	KSJ_SUPPORT_TRIGGER_DELAY,		    // 是否支持对外触发信号的延时。0 - 不支持，1 - 支持
	KSJ_SUPPORT_FAST_UPDATE,            // 是否支持快速升级
	KSJ_SUPPORT_WORKINGMODE_PROGRAM,    // 是否支持固化工作模式
	KSJ_SUPPORT_SERIALSDESC_PROGRAM,    // 是否支持固化设备序号描述符
	KSJ_SUPPORT_COLORMODE_PROGRAM,      // 是否支持固化色彩模式
	KSJ_SUPPORT_LUT_PROGRAM,            // 是否支持固化LUT查找表数据
	KSJ_SUPPORT_FIXED_FRAMERATE_EX,     // 是否支持设置FPGA固定触发频率
	KSJ_SUPPORT_GAMMA_LUT_DOWNLOAD,     // 是否支持Gamma LUT下载到FPGA，如果支持此功能，就可以使能操作
	KSJ_SUPPORT_GAMMA_LUT_PROGRAM,      // 是否支持固化Gamma LUT到相机中
	KSJ_SUPPORT_WORKINGMODEEX_PROGRAM,  // 是否支持固化超级工作模式
	KSJ_SUPPORT_COLORMATRIX_PROGRAM     // 是否支持将WB,CCM校正矩阵固化

};

KSJ_API  int __stdcall KSJ_QueryFunction( int nChannel, KSJ_FUNCTION Function, int *pInfo ); 

#ifdef __cplusplus
}
#endif

#endif