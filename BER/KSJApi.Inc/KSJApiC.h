#ifndef __KSJ_API_C_H__
#define __KSJ_API_C_H__

#pragma message("Include KSJApiC.h") 

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

// 设置预览视场
KSJ_API  int __stdcall KSJ_PreviewSetFieldOfView( int nChannel, int nColStart, int nRowStart, int nColSize, int nRowSize, KSJ_ADDRESSMODE ColAddressMode, KSJ_ADDRESSMODE RowAddressMode );

// 获取预览视场
KSJ_API  int __stdcall KSJ_PreviewGetFieldOfView( int nChannel, int *pnColStart, int *pnRowStart, int *pnColumnSize, int *pnRowSize, KSJ_ADDRESSMODE *pColumnAddressMode, KSJ_ADDRESSMODE *pRowAddressMode );

// Get Default Field Of View for Preview
KSJ_API  int __stdcall KSJ_PreviewGetDefaultFieldOfView( int nChannel, int *pnColStart, int *pnRowStart, int *pnColumnSize, int *pnRowSize, KSJ_ADDRESSMODE *pColumnAddressMode, KSJ_ADDRESSMODE *pRowAddressMode );

KSJ_API  int __stdcall KSJ_PreviewSetFieldOfViewEx( int nChannel, int nColStart, int nRowStart, int nColSize, int nRowSize, KSJ_ADDRESSMODE ColAddressMode, KSJ_ADDRESSMODE RowAddressMode, unsigned short wMultiFrameNum );

KSJ_API  int __stdcall KSJ_PreviewGetFieldOfViewEx( int nChannel, int *pnColStart, int *pnRowStart, int *pnColumnSize, int *pnRowSize, KSJ_ADDRESSMODE *pColumnAddressMode, KSJ_ADDRESSMODE *pRowAddressMode, unsigned short *pwMultiFrameNum );

// 获取在用户设置的视场模式下，预览图像的宽度和高度（像素）
KSJ_API  int __stdcall KSJ_PreviewGetSize       ( int nChannel, int *pnWidth, int *pnHeight );

KSJ_API  int __stdcall KSJ_PreviewGetSizeEx     ( int nChannel, int *pnWidth, int *pnHeight, int *pnBitCount );

// 设置预览图像的显示位置
KSJ_API  int __stdcall KSJ_PreviewSetPos        ( int nChannel, HWND hWnd, int x, int y, int nWidth, int nHeight );
// 获取预览图像的显示位置
KSJ_API  int __stdcall KSJ_PreviewGetPos        ( int nChannel, int *px, int *py, int *pnWidth, int *pnHeight );
// 设置预览模式
enum KSJ_PREVIEWMODE { PM_RAWDATA, PM_RGBDATA };
KSJ_API  int __stdcall KSJ_PreviewSetMode       ( int nChannel, KSJ_PREVIEWMODE PreviewMode  );
// 获取预览模式
KSJ_API  int __stdcall KSJ_PreviewGetMode       ( int nChannel, KSJ_PREVIEWMODE *pPreviewMode  );

KSJ_API  int __stdcall KSJ_PreviewGetDefaultMode( int nChannel, KSJ_PREVIEWMODE *pPreviewMode  );

// 启动或停止预览
KSJ_API  int __stdcall KSJ_PreviewStart         ( int nChannel, bool bStart );
// 暂停或继续预览
KSJ_API  int __stdcall KSJ_PreviewPause         ( int nChannel, bool bPause );

enum    KSJ_PREVIEWSTATUS { PS_STOP, PS_START, PS_PAUSE };
// 得到当前视频流的状态，停止，启动，暂停
KSJ_API  int __stdcall KSJ_PreviewGetStatus     ( int nChannel, KSJ_PREVIEWSTATUS *pPreviewStatus );
 
// 获取暂停时的图像数据，用户必须调用了KSJ_PreviewPause( , true); 成功后调用。根据当前设置的预览模式，取得相应的数据。
KSJ_API  int __stdcall KSJ_PreviewGetPausedData ( int nChannel, unsigned char *pData );

KSJ_API  int __stdcall KSJ_PreviewGetPausedDataEx ( int nChannel, unsigned char *pData, int *pnWidth, int *pnHeight, int *pnBitCount );

// 获取预览帧速
KSJ_API  int __stdcall KSJ_PreviewGetFrameRate  ( int nChannel, float *fFrameRate );

// 设置预览回调
typedef VOID (WINAPI *KSJ_PREVIEWCALLBACK )( unsigned char *pImageData, int nWidth, int nHeight, int nBitCount,  void *lpContext );
KSJ_API  int __stdcall KSJ_PreviewSetCallback   ( int nChannel, KSJ_PREVIEWCALLBACK pfPreviewCallback, void *lpContext );

typedef VOID (WINAPI *KSJ_PREVIEWCALLBACKEX )( HDC  hDC, unsigned char *pImageData, int nWidth, int nHeight, int nBitCount,  void *lpContext );
KSJ_API  int __stdcall KSJ_PreviewSetCallbackEx   ( int nChannel, KSJ_PREVIEWCALLBACKEX pfPreviewCallbackEx, void *lpContext );


#ifdef __cplusplus
}
#endif

#endif