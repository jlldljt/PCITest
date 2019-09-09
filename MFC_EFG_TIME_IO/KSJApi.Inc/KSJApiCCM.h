#ifndef __KSJ_API_COLORCORRECTIONMATRIC_H__
#define __KSJ_API_COLORCORRECTIONMATRIC_H__

#pragma message("Include KSJApiCCM.h") 

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 参考“凯视佳工业相机API函数说明书（颜色校正篇）”
// 只有当KSJ_SCCM_MANUAL或KSJ_HCCM_MANUAL有效时，设置预设组值的索引
// Rc = fMatrix[0][0] * Rw + fMatrix[0][1] * Gw + fMatrix[0][2] * Bw
KSJ_API  int __stdcall KSJ_ColorCorrectionMatrixSet( int nChannel, float fMatrix[3][3] );
KSJ_API  int __stdcall KSJ_ColorCorrectionMatrixGet( int nChannel, float fMatrix[3][3] );

// 只有当KSJ_SCCM_PRESETTINGS或KSJ_HCCM_PRESETTINGS有效时，设置预设组值的索引
KSJ_API  int __stdcall KSJ_ColorCorrectionPresettingSet( int nChannel, KSJ_COLOR_TEMPRATURE CcmPresetting   );
KSJ_API  int __stdcall KSJ_ColorCorrectionPresettingGet( int nChannel, KSJ_COLOR_TEMPRATURE *pCcmPresetting );


KSJ_API  int __stdcall KSJ_ColorCorrectionSet( int nChannel, KSJ_CCM_MODE CcmMode   );               
KSJ_API  int __stdcall KSJ_ColorCorrectionGet( int nChannel, KSJ_CCM_MODE *pCcmMode ); 



#ifdef __cplusplus
}
#endif

#endif