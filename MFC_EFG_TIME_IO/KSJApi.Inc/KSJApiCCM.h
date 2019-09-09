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
// �ο������Ӽѹ�ҵ���API����˵���飨��ɫУ��ƪ����
// ֻ�е�KSJ_SCCM_MANUAL��KSJ_HCCM_MANUAL��Чʱ������Ԥ����ֵ������
// Rc = fMatrix[0][0] * Rw + fMatrix[0][1] * Gw + fMatrix[0][2] * Bw
KSJ_API  int __stdcall KSJ_ColorCorrectionMatrixSet( int nChannel, float fMatrix[3][3] );
KSJ_API  int __stdcall KSJ_ColorCorrectionMatrixGet( int nChannel, float fMatrix[3][3] );

// ֻ�е�KSJ_SCCM_PRESETTINGS��KSJ_HCCM_PRESETTINGS��Чʱ������Ԥ����ֵ������
KSJ_API  int __stdcall KSJ_ColorCorrectionPresettingSet( int nChannel, KSJ_COLOR_TEMPRATURE CcmPresetting   );
KSJ_API  int __stdcall KSJ_ColorCorrectionPresettingGet( int nChannel, KSJ_COLOR_TEMPRATURE *pCcmPresetting );


KSJ_API  int __stdcall KSJ_ColorCorrectionSet( int nChannel, KSJ_CCM_MODE CcmMode   );               
KSJ_API  int __stdcall KSJ_ColorCorrectionGet( int nChannel, KSJ_CCM_MODE *pCcmMode ); 



#ifdef __cplusplus
}
#endif

#endif