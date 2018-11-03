#ifndef __KSJ_API_F_H__
#define __KSJ_API_F_H__

#pragma message("Include KSJApiF.h") 

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

// 清空帧存操作
KSJ_API  int __stdcall KSJ_EmptyFrameBuffer ( int nChannel );

// 触发模式设置
enum KSJ_TRIGGERMODE { KSJ_TRIGGER_INTERNAL, KSJ_TRIGGER_EXTERNAL, KSJ_TRIGGER_SOFTWARE, KSJ_TRIGGER_FIXFRAMERATE };
KSJ_API  int __stdcall KSJ_TriggerModeSet ( int nChannel, KSJ_TRIGGERMODE TriggerMode   );
KSJ_API  int __stdcall KSJ_TriggerModeGet ( int nChannel, KSJ_TRIGGERMODE *pTriggerMode );

// 通过设置FPGA的触发间隔来设置固定帧率，调用此函数之前，必须先设置相机为KSJ_TRIGGER_FIXFRAMERATE模式，通过获取KSJ_SUPPORT_FIXED_FRAMERATE查询此功能。
KSJ_API  int __stdcall KSJ_SetFixedFrameRateEx( int nChannel, float fFrameRate );
KSJ_API  int __stdcall KSJ_GetFixedFrameRateEx( int nChannel, float *pfFrameRate );

// 设置固定帧速(通过设置sensor寄存器,使sensor输出固定帧率)，只有MUC36C/M(MGYY)和MU3E130C/M(CGYY)支持
// V4.0.86.12版本开始，bEnable参数无效，通过用户设置KSJ_TRIGGER_FIXFRAMERATE模式来决定是否使能。
KSJ_API  int __stdcall KSJ_SetFixedFrameRate( int nChannel, bool bEnable,   float fFrameRate );
KSJ_API  int __stdcall KSJ_GetFixedFrameRate( int nChannel, bool *pbEnable, float *pfFrameRate );


// 坏点校正低阈值和高阈值设置（硬件不支持此设置，由软件处理时有效）
KSJ_API  int __stdcall KSJ_BadPixelCorrectionSetThreshold( int nChannel, unsigned char btLow,   unsigned char btHigh   );
KSJ_API  int __stdcall KSJ_BadPixelCorrectionGetThreshold( int nChannel, unsigned char *pbtLow, unsigned char *pbtHigh );

enum KSJ_THRESHOLD { KSJ_THRESHOLD_LOW, KSJ_THRESHOLD_HIGH };
// 坏点校正设置及阈值设置，低阈值会过滤更多的坏点
KSJ_API  int __stdcall KSJ_BadPixelCorrectionSet( int nChannel, bool bEnable,   KSJ_THRESHOLD Threshold );
KSJ_API  int __stdcall KSJ_BadPixelCorrectionGet( int nChannel, bool *pbEnable, KSJ_THRESHOLD *pThreshold);

// 灵敏度模式设置（使用截取8位方式）
enum KSJ_SENSITIVITYMODE { KSJ_LOW = 0, KSJ_MID, KSJ_HIGH, KSJ_HIGHER, KSJ_HIGHEST };
KSJ_API  int __stdcall KSJ_SensitivitySetMode( int nChannel, KSJ_SENSITIVITYMODE SensitivityMode   );
KSJ_API  int __stdcall KSJ_SensitivityGetMode( int nChannel, KSJ_SENSITIVITYMODE *pSensitivityMode );

// 当设置为16比特传输模式时，采集数据比正常时多一倍。
// 当设置为16比特传输模式时，预览图像是通过KSJAPI将16比特的数据转换为8比特的数据显示的。转换方式由灵敏度模式的设置及是否加载了LUT来决定。
// 如果加载了LUT则以LUT来转换，否则由灵敏度模式来转换。

// 设置相机为16bit的采集模式,根据实际AD的转换精度,有效位数可以根据KSJ_GetADCResolution函数获取
KSJ_API  int __stdcall KSJ_SetData16Bits ( int nChannel, bool bEnable  );
KSJ_API  int __stdcall KSJ_GetData16Bits ( int nChannel, bool *bEnable );

// 外触发模式时，外触发信号的触发方式设置
enum KSJ_TRIGGERMETHOD { KSJ_TRIGGER_FALLINGEDGE, KSJ_TRIGGER_RISINGEDGE, KSJ_TRIGGER_HIGHLEVEL, KSJ_TRIGGER_LOWLEVEL };
KSJ_API  int __stdcall KSJ_TriggerMethodSet ( int nChannel, KSJ_TRIGGERMETHOD  TriggerMethod  );
KSJ_API  int __stdcall KSJ_TriggerMethodGet ( int nChannel, KSJ_TRIGGERMETHOD *pTriggerMethod  );

// 外触发模式时，设置触发信号有效到采集的延时时间
KSJ_API  int __stdcall KSJ_TriggerDelaySet( int nChannel, unsigned short wDelay   );
KSJ_API  int __stdcall KSJ_TriggerDelayGet( int nChannel, unsigned short *pwDelay );
KSJ_API  int __stdcall KSJ_TriggerDelayGetRange( int nChannel, unsigned short *wMin, unsigned short *wMax );

// 先清空帧存后再采集原始图像
KSJ_API  int __stdcall KSJ_CaptureRawDataAfterEmptyFrameBuffer     ( int nChannel, unsigned char *pRawData );
// 先清空帧存后再采集彩色图像
KSJ_API  int __stdcall KSJ_CaptureRgbDataAfterEmptyFrameBuffer     ( int nChannel, unsigned char *pRgbData );
// 先清空帧存后再采集彩色图像
KSJ_API  int __stdcall KSJ_CaptureRgbDataExAfterEmptyFrameBuffer   ( int nChannel, unsigned char *pRgbData, int *pnWidth, int *pnHeight, int *pnBitCount );

// 闪光灯信号输出到实际相机输出的延时设置（暂不支持）
KSJ_API  int __stdcall KSJ_StrobeDelaySet( int nChannel, unsigned short wDelay   );
KSJ_API  int __stdcall KSJ_StrobeDelayGet( int nChannel, unsigned short *pwDelay );
KSJ_API  int __stdcall KSJ_StrobeDelayGetRange( int nChannel, unsigned short *wMin, unsigned short *wMax );

// 控制闪光灯触发信号的使能,有效电平,闪光模式
KSJ_API  int __stdcall KSJ_FlashControlSet ( int nChannel, bool bEnable,   bool bInvert,   int nMode   );
KSJ_API  int __stdcall KSJ_FlashControlGet ( int nChannel, bool *pbEnable, bool *pbInvert, int *pnMode );

enum KSJ_FLASHMODE { KSJ_FM_TROW, KSJ_FM_SIMULTANEOUSEXPOSURE, KSJ_FM_SHUTTERWIDTH };
KSJ_API  int __stdcall KSJ_FlashSetMode ( int nChannel, KSJ_FLASHMODE FlashMode   );
KSJ_API  int __stdcall KSJ_FlashGetMode ( int nChannel, KSJ_FLASHMODE *pFlashMode );

// 设置图像传感器的工作方式（V4.0.86.12支持500W相机）
enum KSJ_SENSORMODE { KSJ_SM_TRIGGER_MODE, KSJ_SM_FREERUN_MODE, KSJ_SM_GRR_MODE };
KSJ_API  int __stdcall KSJ_SensorModeSet ( int nChannel, KSJ_SENSORMODE SensorMode  );
KSJ_API  int __stdcall KSJ_SensorModeGet ( int nChannel, KSJ_SENSORMODE *SensorMode  );


#ifdef __cplusplus
}
#endif

#endif