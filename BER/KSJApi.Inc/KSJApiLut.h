#ifndef __KSJ_API_LOOKUPTABLE_H__
#define __KSJ_API_LOOKUPTABLE_H__

#pragma message("Include KSJApiLut.h") 

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

// 参考“凯视佳工业相机API函数说明书（查找表篇）”

// 提取文件中的LUT数据，将LUT数据下载到相机中（根据固件版本不同，掉电有可能会丢失）
KSJ_API  int __stdcall KSJ_LutFileDownload( int nChannel, const TCHAR *pszKtbFile );
// 将LUT数据下载到相机中（根据固件版本不同，掉电有可能会丢失）
KSJ_API  int __stdcall KSJ_LutDataDownload( int nChannel, unsigned char *pData, int nBytes   );
// 使能LUT转换功能
KSJ_API  int __stdcall KSJ_LutSetEnable( int nChannel, bool bEnable   );
// 获取LUT转换功能是否使能
KSJ_API  int __stdcall KSJ_LutGetEnable( int nChannel, bool *pbEnable );

// 提取ktb中的LUT数据，将数据固化到相机内部
KSJ_API  int __stdcall KSJ_LutFileProgram( int nChannel, TCHAR *pszFile );
// 将LUT数据固化到相机内部
KSJ_API  int __stdcall KSJ_LutDataProgram( int nChannel, unsigned char *pData, int nBytes );
// 从固化到EEPROM相机中读出LUT数据
KSJ_API  int __stdcall KSJ_LutDataReadout( int nChannel, unsigned char *pData, int *pnBytes );


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////Gamma Lut
KSJ_API  int __stdcall KSJ_GammaLutFileDownload( int nChannel, const TCHAR *pszKtbFile );

KSJ_API  int __stdcall KSJ_GammaLutDataDownload( int nChannel, unsigned short *pData, int nWords );
// 使能Gamma LUT转换功能
KSJ_API  int __stdcall KSJ_GammaLutSetEnable( int nChannel, bool bEnable   );
// 获取Gamma LUT转换功能是否使能
KSJ_API  int __stdcall KSJ_GammaLutGetEnable( int nChannel, bool *pbEnable );

KSJ_API  int __stdcall KSJ_GammaLutFileProgram( int nChannel, TCHAR *pszFile );

KSJ_API  int __stdcall KSJ_GammaLutDataProgram( int nChannel, unsigned short *pData, int nWords );

// 从固化到EEPROM相机中读出LUT数据
KSJ_API  int __stdcall KSJ_GammaLutDataReadout( int nChannel, unsigned short *pData, int *pnWords );



#ifdef __cplusplus
}
#endif

#endif