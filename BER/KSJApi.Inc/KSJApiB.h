#ifndef __KSJ_API_B_H__
#define __KSJ_API_B_H__

#pragma message("Include KSJApiB.h") 

#ifdef KSJAPI_EXPORTS
    #define KSJ_API __declspec(dllexport)
#elif defined KSJAPI_STATIC
	#define KSJ_API
#else
    #define KSJ_API __declspec(dllimport)
#endif

/*=================================================================
	以下函数为KSJApi.dll导出函数。
	在你的应用程序中，你应该包含KSJAPI.H并链接KSJApi.LIB。
	然后你就可以调用下面的导出函数。
  -----------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"{
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifdef  UNICODE                     // r_winnt

#ifndef _TCHAR_DEFINED
typedef unsigned short TCHAR, *PTCHAR;
#endif /* !_TCHAR_DEFINED */

#else   /* UNICODE */               // r_winnt
#ifndef _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

#endif /* UNICODE */  

// 获取KSJAPI函数库版本号
KSJ_API  int __stdcall KSJ_GetVersion( int *pnMaj1, int *pnMaj2, int *pnMin1, int *pnMin2 );

// 获得错误的详细信息，可以在任何时候调用
KSJ_API  int __stdcall KSJ_GetErrorInfo( int nErrorNo, TCHAR pszErrorInfo[256] );

// 指定日志保存的文件夹
KSJ_API  int __stdcall KSJ_LogSet( bool bEnable,  const TCHAR *pszFolder );

KSJ_API  int __stdcall KSJ_LogGet( bool *bEnable, TCHAR *pszFolder );

// 初始化KSJApi动态库, 此函数内部会获取连入相机的数目，但不对设备进行打开操作。
KSJ_API  int __stdcall KSJ_Init( void );

// KSJ_API  int __stdcall KSJ_InitEx( int nMaj, int nMin );

// 反初始化KSJApi动态库，必须与KSJ_Init对应
KSJ_API  int __stdcall KSJ_UnInit( void );

// 得到设备数目，此数目是通过调用KSJ_Init函数时得到的
KSJ_API  int __stdcall KSJ_DeviceGetCount( void );

// 凯视佳工业相机设备类型定义：M-Mircon Sensor, R-Rolling Shutter, N-No External Trigger, N-No Frame Buffer
enum KSJ_DEVICETYPE 
{ 
	KSJ_UC130C_MRNN = 0,  // Guass2
	KSJ_UC130M_MRNN,      // Guass2
	KSJ_RESERVED0,
	KSJ_UC320C_MRNN,      // Guass2
	KSJ_UC130C_MRYN,      
	KSJ_UC130M_MRYN, 
	KSJ_RESERVED1, 
	KSJ_UC320C_MRYN, 
	KSJ_UC500C_MRNN, 
	KSJ_UC500M_MRNN, 
	KSJ_UC500C_MRYN, 
	KSJ_UC500M_MRYN, 
	KSJ_UC320C_OCR, 
	KSJ_UC900C_MRNN, 
	KSJ_UC1000C_MRNN, 
	KSJ_UC900C_MRYN, 
	KSJ_UC1000C_MRYN, 

	KSJ_UC130C_MRYY,    // Elanus2
	KSJ_UC130M_MRYY,    // Elanus2 

	KSJ_UD140C_SGNN, 
	KSJ_UD140M_SGNN, 
	KSJ_UC36C_MGNN, 
	KSJ_UC36M_MGNN,
	KSJ_UC36C_MGYN,
	KSJ_UC36M_MGYN,

	KSJ_UC900C_MRYY,    // Elanus2
	KSJ_UC1000C_MRYY,   // Elanus2
	KSJ_UC1400C_MRYY,   // Elanus2
	KSJ_UC36C_MGYY,     // Elanus2
	KSJ_UC36M_MGYY,     // Elanus2
	KSJ_UC320C_MRYY,    // Elanus2
	KSJ_UC500C_MRYY,    // Elanus2
	KSJ_UC500M_MRYY,    // Elanus2

    KSJ_MUC130C_MRYN, 
	KSJ_MUC130M_MRYN, 
	KSJ_MUC320C_MRYN,

	KSJ_MUC36C_MGYYO,    // Jelly2
	KSJ_MUC36M_MGYYO,    // Jelly2 

	KSJ_MUC130C_MRYY, 
	KSJ_MUC130M_MRYY, 
	KSJ_MUC320C_MRYY, 
	KSJ_MUC500C_MRYY, 
	KSJ_MUC500M_MRYY,
    KSJ_MUC900C_MRYY, 
	KSJ_MUC1000C_MRYY, 
	KSJ_MUC1400C_MRYY,

	KSJ_UD205C_SGYY,     // Elanus2
	KSJ_UD205M_SGYY,     // Elanus2
	KSJ_UD274C_SGYY,     // Elanus2
	KSJ_UD274M_SGYY,     // Elanus2
	KSJ_UD285C_SGYY,     // Elanus2
	KSJ_UD285M_SGYY,     // Elanus2

	KSJ_MU3C500C_MRYYO,  // Jelly3 
	KSJ_MU3C500M_MRYYO,  // Jelly3
	KSJ_MU3C1000C_MRYYO, // Jelly3
	KSJ_MU3C1400C_MRYYO, // Jelly3
	KSJ_MU3V130C_CGYYO,  // Jelly3
	KSJ_MU3V130M_CGYYO,  // Jelly3
	KSJ_MU3E130C_EGYYO,  // Jelly3
	KSJ_MU3E130M_EGYYO,  // Jelly3

	KSJ_MUC36C_MGYFO,    // Jelly1
	KSJ_MUC36M_MGYFO,    // Jelly1

	KSJ_MU3C120C_MGYYO,  // Jelly3
	KSJ_MU3C120M_MGYYO,  // Jelly3
	KSJ_MU3E200C_EGYYO,  // Jelly3
	KSJ_MU3E200M_EGYYO,  // Jelly3

	KSJ_MUC130C_MRYNO,   // Jelly1
	KSJ_MUC130M_MRYNO,   // Jelly1
	KSJ_MUC320C_MRYNO,   // Jelly1

	KSJ_U3C130C_MRYNO,   // Guass3
	KSJ_U3C130M_MRYNO,   // Guass3
	KSJ_U3C320C_MRYNO,   // Guass3
	KSJ_U3C500C_MRYNO,   // Guass3
	KSJ_U3C500M_MRYNO,   // Guass3

	KSJ_MU3C1401C_MRYYO,
	KSJ_MU3C1001C_MRYYO,
	KSJ_MUC131M_MRYN,    // WD
	KSJ_MU3C501C_MRYYO,
	KSJ_MU3C501M_MRYYO,
	KSJ_MU3C121C_MGYYO,
	KSJ_MU3C121M_MGYYO,
	KSJ_MU3E131C_EGYYO,
	KSJ_MU3E131M_EGYYO,
	KSJ_MU3E201C_EGYYO,
	KSJ_MU3E201M_EGYYO,

	KSJ_MISSING_DEVICE,	// 丢失程序的设备

};

// 关闭指定索引的设备，注意所有第一个参数为nChannle的函数都会自动打开指定的相机，如果不关闭指定相机，那么在多进程下是不能操作这台相机的。
KSJ_API  int __stdcall KSJ_DeviceClose( int nChannel );

enum KSJ_DEVICESTATUS { KSJ_DS_CLOSE, KSJ_DS_OPEN };
KSJ_API  int __stdcall KSJ_DeviceGetStatus( int nChannel, KSJ_DEVICESTATUS *pDs );

// 得到设备信息（型号，序号，固件版本号）
KSJ_API  int __stdcall KSJ_DeviceGetInformation( IN int nChannel, OUT KSJ_DEVICETYPE *pDeviceType, OUT int *pnIndex, OUT unsigned short *pwFirmwareVersion );

KSJ_API  int __stdcall KSJ_DeviceGetInformationEx( IN int nChannel, OUT KSJ_DEVICETYPE *pDeviceType, OUT int *pnIndex, OUT unsigned short *pwFirmwareVersion, OUT unsigned short *pwFpgaVersion );

// 抽点模式定义（分为列抽点和行抽点）：
enum KSJ_ADDRESSMODE { KSJ_SKIPNONE = 0, KSJ_SKIP2, KSJ_SKIP3, KSJ_SKIP4, KSJ_SKIP8 };

// 获取采集设备所支持的默认视场（最大的市场设置范围）
KSJ_API  int __stdcall KSJ_CaptureGetDefaultFieldOfView( int nChannel, int *pnColStart, int *pnRowStart, int *pnColumnSize, int *pnRowSize, KSJ_ADDRESSMODE *pColumnAddressMode, KSJ_ADDRESSMODE *pRowAddressMode );

// 设置采集视场大小
KSJ_API  int __stdcall KSJ_CaptureSetFieldOfView( int nChannel, int nColStart, int nRowStart, int nColSize, int nRowSize, KSJ_ADDRESSMODE ColAddressMode, KSJ_ADDRESSMODE RowAddressMode );

// 获取采集视场的实际大小
KSJ_API  int __stdcall KSJ_CaptureGetFieldOfView( int nChannel, int *pnColStart, int *pnRowStart, int *pnColumnSize, int *pnRowSize, KSJ_ADDRESSMODE *pColumnAddressMode, KSJ_ADDRESSMODE *pRowAddressMode );

// 设置采集视场和拼接视场个数
KSJ_API  int __stdcall KSJ_CaptureSetFieldOfViewEx( int nChannel, int nColStart, int nRowStart, int nColSize, int nRowSize, KSJ_ADDRESSMODE ColAddressMode, KSJ_ADDRESSMODE RowAddressMode, unsigned short wMultiFrameNum );

KSJ_API  int __stdcall KSJ_CaptureGetFieldOfViewEx( int nChannel, int *pnColStart, int *pnRowStart, int *pnColumnSize, int *pnRowSize, KSJ_ADDRESSMODE *pColumnAddressMode, KSJ_ADDRESSMODE *pRowAddressMode, unsigned short *pwMultiFrameNum );

// 设置读取的超时时间（毫秒）
KSJ_API  int __stdcall KSJ_CaptureSetTimeOut( int nChannel, unsigned long dwTimeOut );

KSJ_API  int __stdcall KSJ_CaptureGetTimeOut( int nChannel, unsigned long *pdwTimeOut );

// 设置读取出错时是否进行重新恢复操作
KSJ_API  int __stdcall KSJ_CaptureSetRecover( int nChannel, bool bRecover );

KSJ_API  int __stdcall KSJ_CaptureGetRecover( int nChannel, bool *pbRecover );

// 设置读取的超时时间（毫秒）和读取出错时是否进行重新恢复操作
KSJ_API  int __stdcall KSJ_CaptureSetTimeOutEx( int nChannel, unsigned long dwTimeOut, bool bRecover );

KSJ_API  int __stdcall KSJ_CaptureGetTimeOutEx( int nChannel, unsigned long *pdwTimeOut, bool *pbRecover );

// 得到采集图像的宽度和高度（单位：像素）
KSJ_API  int __stdcall KSJ_CaptureGetSize( int nChannel, int *pnWidth, int *pnHeight );
// 得到采集图像的宽度和高度,比特位数（单位：像素）
KSJ_API  int __stdcall KSJ_CaptureGetSizeEx( int nChannel, int *pnWidth, int *pnHeight, int *pnBitCount );
// 由于加入了16bit的读取数据模式,以前用户通过判断返回的Width, Height, BitCount来判断图像字节数据量将带来不方便,特增加了此函数
// 这样在调用KSJ_CaptureXXXXXXX函数前分配的内存可以根据nBitsPerSample的值来分配, nBitsPerSample=8 or 16
KSJ_API  int __stdcall KSJ_CaptureGetSizeExEx( int nChannel, int *pnWidth, int *pnHeight, int *pnBitCount, int *nBitsPerSample );

// 采集一帧原始数据图像数据
// 如果设置了16位的数据读取模式,那么采集的数据也是16位的
KSJ_API  int __stdcall KSJ_CaptureRawData     ( int nChannel, unsigned char *pRawData );
// 采集一帧8灰度图像或24或32位彩色图像数据（与KSJ_BayerSetMode和KSJ_PreviewSetMode相关）
// 如果设置了16位的数据读取模式,那么采集的数据也是16位的
KSJ_API  int __stdcall KSJ_CaptureRgbData     ( int nChannel, unsigned char *pRgbData );

KSJ_API  int __stdcall KSJ_CaptureRgbDataToClipboard( int nChannel );

// 如果设置了16位的数据读取模式,那么采集的数据也是16位的
KSJ_API  int __stdcall KSJ_CaptureRgbDataEx   ( int nChannel, unsigned char *pRgbData, int *pnWidth, int *pnHeight, int *pnBitCount );

// 相机参数
enum KSJ_PARAM { 
	KSJ_EXPOSURE = 0,       // 曝光时间（单位：毫秒）
	KSJ_RED, 
	KSJ_GREEN, 
	KSJ_BLUE, 
	KSJ_GAMMA, 
	KSJ_PREVIEW_COLUMNSTART, 
	KSJ_PREVIEW_ROWSTART, 
	KSJ_CAPTURE_COLUMNSTART, 
	KSJ_CAPTURE_ROWSTART, 
	KSJ_HORIZONTALBLANK, 
	KSJ_VERTICALBLANK, 
	KSJ_FLIP, 
	KSJ_BIN, 
	KSJ_MIRROR, 
	KSJ_CONTRAST, 
	KSJ_BRIGHTNESS, 
	KSJ_VGAGAIN, 
	KSJ_CLAMPLEVEL, 
	KSJ_CDSGAIN,
	KSJ_RED_SHIFT,
	KSJ_GREEN_SHIFT,
	KSJ_BLUE_SHIFT,
    KSJ_COMPANDING,
	KSJ_EXPOSURE_LINES,    // 23
	KSJ_SATURATION,
	KSJ_TRIGGERDELAY,
	KSJ_STROBEDELAY,
	KSJ_TRIGGER_MODE,
	KSJ_TRIGGER_METHOD, 
	KSJ_BLACKLEVEL,
	KSJ_BLACKLEVEL_THRESHOLD_AUTO,
	KSJ_BLACKLEVEL_THRESHOLD_LO,
	KSJ_BLACKLEVEL_THRESHOLD_HI
};

// 获得参数的设置范围
KSJ_API  int __stdcall KSJ_GetParamRange( IN int nChannel, IN KSJ_PARAM Param, OUT int *pnMinValue, OUT int *pnMaxValue );
// 设置参数
KSJ_API  int __stdcall KSJ_SetParam     ( IN int nChannel, IN KSJ_PARAM Param, IN  int nValue );
// 获取参数当前值
KSJ_API  int __stdcall KSJ_GetParam     ( IN int nChannel, IN KSJ_PARAM Param, OUT int *pnValue );

// 设置Gamma值，范围 -9 - +9
KSJ_API  int __stdcall KSJ_GammaSetValue( int nChannel, int nValue );
// 打开或关闭Gamma校正，即影响预览又影响采集
KSJ_API  int __stdcall KSJ_GammaOn( int nChannel, bool bOn );

// 枚举类型贝耶尔模式
enum KSJ_BAYERMODE { KSJ_BGGR_BGR24 = 0, KSJ_GRBG_BGR24, KSJ_RGGB_BGR24, KSJ_GBRG_BGR24, KSJ_BGGR_BGR24_FLIP, KSJ_GRBG_BGR24_FLIP,  KSJ_RGGB_BGR24_FLIP, KSJ_GBRG_BGR24_FLIP, 
                     KSJ_BGGR_BGR32,     KSJ_GRBG_BGR32, KSJ_RGGB_BGR32, KSJ_GBRG_BGR32, KSJ_BGGR_BGR32_FLIP, KSJ_GRBG_BGR32_FLIP,  KSJ_RGGB_BGR32_FLIP, KSJ_GBRG_BGR32_FLIP,
                     KSJ_BGGR_GRAY8,     KSJ_GRBG_GRAY8, KSJ_RGGB_GRAY8, KSJ_GBRG_GRAY8, KSJ_BGGR_GRAY8_FLIP, KSJ_GRBG_GRAY8_FLIP,  KSJ_RGGB_GRAY8_FLIP, KSJ_GBRG_GRAY8_FLIP };

// 得到当前相机的默认Bayer模式
KSJ_API   int __stdcall KSJ_BayerGetDefaultMode( int nChannel, KSJ_BAYERMODE *pBayerMode );
// 设置Bayer模式，同时影响采集和预览，这个函数除特殊需要调用，否则用户不需调用此功能。
KSJ_API   int __stdcall KSJ_BayerSetMode( int nChannel, KSJ_BAYERMODE BayerMode );
// 获得当前Bayer模式
KSJ_API   int __stdcall KSJ_BayerGetMode( int nChannel, KSJ_BAYERMODE *pBayerMode );


enum KSJ_FILTERMODE { KSJ_NEARESTNEIGHBOR, KSJ_BILINEAR, KSJ_SMOOTHHUE, KSJ_EDGESENSING, KSJ_LAPLACIAN, KSJ_FASTBILINEAR };

// 设置Bayer Filter模式，同时影响采集和预览，这个函数除特殊需要调用，否则用户不需调用此功能。
KSJ_API   int __stdcall KSJ_FilterSetMode( int nChannel, KSJ_FILTERMODE FilterMode );
// 获得当前Bayer模式
KSJ_API   int __stdcall KSJ_FilterGetMode( int nChannel, KSJ_FILTERMODE *pFilterMode );

// 设置自动曝光的区域，这个区域是相对预览图像的左上角像素位置
KSJ_API  int __stdcall KSJ_AESetRegion( int nChannel, int nX, int nY,  int nW, int nH );
// 自动曝光回调函数
typedef void (__stdcall *KSJ_AECALLBACK )( bool bSuccess, int nResult,  void *lpContext );
// 定义自动曝光操作结束后的回调函数，当自动曝光结束时，KSJ开发包会自动调用AECallback用户指定的回调函数
KSJ_API  int __stdcall KSJ_AESetCallback( int nChannel, KSJ_AECALLBACK pfAECallback, void *lpContext );
// 开始或终止自动曝光调节
KSJ_API  int __stdcall KSJ_AEStart( int nChannel, bool bStart, int nMaxCount, int nTarget );

// 设置自动白平衡区域
KSJ_API  int __stdcall KSJ_AWBSetRegion( int nChannel, int nX, int nY,  int nW, int nH );
// 自动白平衡回调函数
typedef void (__stdcall *KSJ_AWBCALLBACK )( bool bSuccess, int nValueR, int nValueG, int nValueB,  void *lpContext );
// 定义自动白平衡（操作硬件）操作结束后的回调函数，当自动曝光结束时，KSJ开发包会自动调用AECallback用户指定的回调函数
KSJ_API  int __stdcall KSJ_AWBSetCallback( int nChannel, KSJ_AWBCALLBACK fpAWBCallback, void *lpContext );
// 开始或终止自动白平衡调节
KSJ_API  int __stdcall KSJ_AWBStart( int nChannel, bool bStart, int nMaxCount, int nRTarget, int nGTarget, int nBTarget );

// 设置聚焦评估区域
KSJ_API  int __stdcall KSJ_AFSetRegion( int nChannel, int nX, int nY,  int nW, int nH );
// 聚焦评估回调函数, nValue为返回的评估值
typedef void (__stdcall *KSJ_AFCALLBACK )( int nValue,  void *lpContext );
// 设置聚焦评估回调
KSJ_API  int __stdcall KSJ_AFSetCallback( int nChannel, KSJ_AFCALLBACK fpAFCallback, void *lpContext );
// 开始或终止聚焦评估
KSJ_API  int __stdcall KSJ_AFStart( int nChannel, bool bStart );

// 设置对比度，设置值范围（-128~128）
KSJ_API  int __stdcall KSJ_ProcessContrastSet( int nChannel, int nContrast );

KSJ_API  int __stdcall KSJ_ProcessContrastGet( int nChannel, int *pnContrast );

KSJ_API  int __stdcall KSJ_ProcessContrastGetRange( int nChannel, int *pnMinContrast, int *pnMaxContrast );

// 设置亮度，设置值范围（-128~128）
KSJ_API  int __stdcall KSJ_ProcessBrightnessSet( int nChannel, int nBrightness );

KSJ_API  int __stdcall KSJ_ProcessBrightnessGet( int nChannel, int *pnBrightness );

KSJ_API  int __stdcall KSJ_ProcessBrightnessGetRange( int nChannel, int *pnMinBrightness, int *pnMaxBrightness );

KSJ_API  int __stdcall KSJ_ProcessSaturationSet( int nChannel, int nSaturation );

KSJ_API  int __stdcall KSJ_ProcessSaturationGet( int nChannel, int *pnStaturation );

KSJ_API  int __stdcall KSJ_ProcessSaturationGetRange( int nChannel, int *pnMinSaturation, int *pnMaxSaturation );


KSJ_API  int __stdcall KSJ_ExposureTimeSet( int nChannel, float fExpTimeMs );
KSJ_API  int __stdcall KSJ_ExposureTimeGet( int nChannel, float *pfExpTimeMs );

// 调用此函数时必须开启预览，视频文件根据预览视场
KSJ_API  int __stdcall KSJ_RecordStart  ( int nChannel, bool bStart, float fRecordRate, const TCHAR *pFileName );

// 调用此函数时必须开启预览，视频文件根据预览视场
KSJ_API  int __stdcall KSJ_RecordStartEx( int nChannel, bool bStart, float fRecordRate, const TCHAR *pFileName, bool bShowCompossorSelection );

// 调用此函数时可以关闭预览，视频文件根据用户设置的宽度高度，文件的视频数据由KSJ_RecordAddOneFrame添加，注意宽度，高度，比特位数必须相等
KSJ_API  int __stdcall KSJ_RecordStartExEx( int nChannel, bool bStart, float fRecordRate, int nWidth, int nHeight, int nBitCount, const TCHAR *pFileName, bool bShowCompossorSelection );

KSJ_API  int __stdcall KSJ_RecordAddOneFrame( int nChannel, unsigned char *pFrameData, int nFrameNum, int nWidth, int nHeight, int nBitcount );

KSJ_API  int __stdcall KSJ_RecordStatusGet( int nChannel, bool *bStart );


// 将pData(宽度为nWidth, 高度为nHeight, 比特位数为nBitCount)的图像数据缩放为宽度为nStretchWidh, 高度为nStretchHeight的数据并填充到用户分配的pStretchData缓冲区,比特位数不变
KSJ_API  int __stdcall KSJ_HelperStretchBmp( unsigned char *pData, int nWidth, int nHeight, int nBitCount, unsigned char *pStretchData, int nStretchWidth, int nStretchHeight );

// 根据图像的比特深度得到BMP图像的文件头,信息头,及颜色表总的字节数目
KSJ_API  int __stdcall KSJ_HelperGetBmpFileHeaderSize( int nBitCount, int *pnTotalBytes );

// 得到BMP图像的文件头和信息头
KSJ_API  int __stdcall KSJ_HelperGetBmpFileHeader( int nWidth, int nHeight, int nBitCount, void *pHeader );

// 保存为Bmp文件
KSJ_API  int __stdcall KSJ_HelperSaveToBmp( unsigned char *pData, int nWidth, int nHeight, int nBitCount, const TCHAR *pszFileName );
// 保存为Jpg文件
KSJ_API  int __stdcall KSJ_HelperSaveToJpg( unsigned char *pData, int nWidth, int nHeight, int nBitCount, int nQulity, const TCHAR *pszFileName );
// 保存为tiff文件,可以保存16bit采样和8bit采样的数据
// nBitPerSample = 8 或16
// nSamplesPerPixel = 1或3或4(分别对应BitCount=8,24,32),参考KSJ_CaptureGetSizeExEx函数
KSJ_API  int __stdcall KSJ_HelperSaveToTiff( unsigned char *pData, int nWidth, int nHeight, int nBitPerSample, int nSamplesPerPixel, const TCHAR *pszFileName );

///////////////////////////////////////////////Color Correction////////////////////////////////////////////////////////////////
typedef void (__stdcall *KSJ_CCCALLBACK )( bool bSuccess, int nRShift, int nGShift, int nBShift,  void *lpContext );

// 如下函数为软件颜色校正
KSJ_API  int __stdcall KSJ_ColorCorrectSetCallback( int nChannel, KSJ_CCCALLBACK fpCCCallback, void *lpContext );

enum KSJ_COLORCORRECT { KSJ_CC_NONE, KSJ_CC_SHIFTRGB, KSJ_CC_EQRGB, KSJ_CC_EQALL, KSJ_CC_WB };
KSJ_API  int __stdcall KSJ_ColorCorrectSet  ( int nChannel, KSJ_COLORCORRECT ColorCorrect, int nRTarget, int nGTarget, int nBTarget );
KSJ_API  int __stdcall KSJ_ColorCorrectGet  ( int nChannel, KSJ_COLORCORRECT *pColorCorrect );
KSJ_API  int __stdcall KSJ_ColorCorrectGetEx( int nChannel, KSJ_COLORCORRECT *pColorCorrect, int *pnRTarget, int *pnGTarget, int *pnBTarget );

KSJ_API  int __stdcall KSJ_ColorCorrectTableSave( int nChannel, const TCHAR *pszFileName );
KSJ_API  int __stdcall KSJ_ColorCorrectTableLoad( int nChannel, const TCHAR *pszFileName );
KSJ_API  int __stdcall KSJ_IsColorCorrectTableLoad( int nChannel, bool *bLoad );



#ifdef __cplusplus
}
#endif

#endif


