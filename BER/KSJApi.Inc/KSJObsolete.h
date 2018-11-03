#ifndef __KSJ_API_OBSOLETE_H__
#define __KSJ_API_OBSOLETE_H__

#pragma message("Include KSJApiObsolete.h") 


#ifdef KSJAPI_EXPORTS
    #define KSJ_API __declspec(dllexport)
#elif defined KSJAPI_STATIC
	#define KSJ_API
#else
    #define KSJ_API __declspec(dllimport)
#endif

// 为了保持对老客户调用的兼容性，这个文件中的函数是以前版本定义的，但在新版本中仍然保留，但我们已经不推荐使用的函数。
// 其中有一些函数可以由其他函数替代，也有些函数已经不再支持。
#ifdef __cplusplus
extern "C"{
#endif

enum KSJ_TRIGGERSTATUS { KSJ_NO, KSJ_HAVE };
// 获取帧存中是否有图像数据（已不再支持）
KSJ_API  int __stdcall KSJ_TriggerStatusGet ( int nChannel, KSJ_TRIGGERSTATUS *pTriggerStatus  );

#if 0

KSJ_API  int __stdcall KSJ_IsSupportLUT( int nChannel );                      // Use KSJ_QueryFunction
KSJ_API  int __stdcall KSJ_EnableLUT( int nChannel, bool bEnable );           // Use KSJ_LutSetEnable
KSJ_API  int __stdcall KSJ_IsEnableLUT( int nChannel, bool *pbEnable );       // Use KSJ_LutGetEnable


// 这个函数会使相机将查找表写入到24LC1024的LUT_BASE_ADDRESS为基址的ulOffsetAddress为偏移的地址。
KSJ_API  int __stdcall KSJ_ReadLUT ( int nChannel, unsigned short usOffsetAddress, int nBytes, unsigned char *pData   );
// nBytes必须不大于64字节，usOffsetAddress 从0开始。
KSJ_API  int __stdcall KSJ_WriteLUT( int nChannel, unsigned short usOffsetAddress, unsigned char *pData, int nBytes  );

// 加载影射表
// 这个表要通过凯视佳专用的影射表生成工具生成, 此函数会根据查找表的不同类型进行相应的加载
// 对于12bit->8bit或者10bit->8bit映射表, 库函数会在设置16bit数据传输模式时,通过影射之后进行显示，此功能由PC端计算完成。
KSJ_API  int __stdcall KSJ_LoadMapTableFile( int nChannel, bool bLoad, const TCHAR *pszTableName );    // Use KSJ_LutFileDownload

#endif
#ifdef __cplusplus
}
#endif

#endif