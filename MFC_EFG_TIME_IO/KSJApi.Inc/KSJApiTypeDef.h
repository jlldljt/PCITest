#ifndef __KSJ_API_TYPEDEFINE_H__
#define __KSJ_API_TYPEDEFINE_H__

#pragma message("Include KSJApiTypeDef.h") 

typedef void (__stdcall *KSJ_WB_CALLBACK )( float fMatrix[3],  void *lpContext );

typedef void (__stdcall *KSJ_WB_CALLBACK_EX )( float fMatrixR,  float fMatrixG,  float fMatrixB, void *lpContext );


enum KSJ_COLOR_TEMPRATURE { KSJ_CT_5000K, KSJ_CT_6500K, KSJ_CT2800K };

enum KSJ_WB_MODE { KSJ_WB_DISABLE, KSJ_SWB_AUTO_ONCE, KSJ_SWB_AUTO_CONITNUOUS, KSJ_SWB_PRESETTINGS, KSJ_SWB_MANUAL, KSJ_HWB_AUTO_ONCE, KSJ_HWB_AUTO_CONITNUOUS, KSJ_HWB_PRESETTINGS, KSJ_HWB_MANUAL };

enum KSJ_CCM_MODE { KSJ_CCM_DISABLE, KSJ_SCCM_PRESETTINGS, KSJ_SCCM_MANUAL, KSJ_HCCM_PRESETTINGS, KSJ_HCCM_MANUAL };

#endif