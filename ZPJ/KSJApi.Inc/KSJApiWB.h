#ifndef __KSJ_API_WHITEBALANCE_H__
#define __KSJ_API_WHITEBALANCE_H__

#pragma message("Include KSJApiWB.h") 

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

#include "KSJApiTypeDef.h"

// 参考“凯视佳工业相机API函数说明书（白平衡篇）”

// White Balance
// Rw = fMatrix[0] * R, Gw = fMatrix[1] * G, Bw = fMatrix[1] * B
// 只有当KSJ_AWB_MANUAL模式时有效
KSJ_API  int __stdcall KSJ_WhiteBalanceMatrixSet( int nChannel, float fMatrix[3] );
KSJ_API  int __stdcall KSJ_WhiteBalanceMatrixGet( int nChannel, float fMatrix[3] );           // Use This Function, You can also Get Auto Matrix.

// 只有当KSJ_AWB_AUTO_ONCE，KSJ_AWB_AUTO_CONITNUOUS时有效
// Call Callback Function When Auto Done. Finall Matrix will Return.
KSJ_API  int __stdcall KSJ_WhiteBalanceAutoSetCallBack  ( int nChannel, KSJ_WB_CALLBACK    pfCallBack,   void *lpContext );      // The nPhi decide auto calculate Matrix.
KSJ_API  int __stdcall KSJ_WhiteBalanceAutoSetCallBackEx( int nChannel, KSJ_WB_CALLBACK_EX pfCallBackEx, void *lpContext ); 

KSJ_API  int __stdcall KSJ_WhiteBalanceAutoSet( int nChannel, int nPhi );                     // The nPhi decide auto calculate Matrix.
KSJ_API  int __stdcall KSJ_WhiteBalanceAutoGet( int nChannel, int *pnPhi );                   // This function will auto calculate Matrix.

// 只有当KSJ_AWB_PRESETTINGS时有效，设置预设组值的索引
KSJ_API  int __stdcall KSJ_WhiteBalancePresettingSet( int nChannel, KSJ_COLOR_TEMPRATURE WbPresetting );
KSJ_API  int __stdcall KSJ_WhiteBalancePresettingGet( int nChannel, KSJ_COLOR_TEMPRATURE *pWbPresetting );

// 设置白平衡模式
KSJ_API  int __stdcall KSJ_WhiteBalanceSet( int nChannel, KSJ_WB_MODE WbMode   );               
KSJ_API  int __stdcall KSJ_WhiteBalanceGet( int nChannel, KSJ_WB_MODE *pWbMode );               



#ifdef __cplusplus
}
#endif

#endif