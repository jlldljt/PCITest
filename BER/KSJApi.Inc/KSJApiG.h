#ifndef __KSJ_API_G_H__
#define __KSJ_API_G_H__

#pragma message("Include KSJApiG.h") 

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

// 硬件自动曝光
KSJ_API  int __stdcall KSJ_AECSetEnable ( int nChannel, bool bEnable  );
KSJ_API  int __stdcall KSJ_AECGetEnable ( int nChannel, bool *bEnable  );

KSJ_API  int __stdcall KSJ_AECSetRange ( int nChannel, unsigned short usMin,  unsigned short usMax  );
KSJ_API  int __stdcall KSJ_AECGetRange ( int nChannel, unsigned short *usMin, unsigned short *usMax );

// 得到自动曝光调节的当前曝光值
KSJ_API  int __stdcall KSJ_AECGetCur ( int nChannel, unsigned short *usCurGain  );

// 硬件自动增益
KSJ_API  int __stdcall KSJ_AGCSetEnable ( int nChannel, bool bEnable  );
KSJ_API  int __stdcall KSJ_AGCGetEnable ( int nChannel, bool *bEnable  );

KSJ_API  int __stdcall KSJ_AGCSetRange ( int nChannel, unsigned short usMin,  unsigned short usMax  );
KSJ_API  int __stdcall KSJ_AGCGetRange ( int nChannel, unsigned short *usMin, unsigned short *usMax );

// 得到自动增益调节的当前增益值
KSJ_API  int __stdcall KSJ_AGCGetCur ( int nChannel, unsigned short *usCurGain  );




#ifdef __cplusplus
}
#endif

#endif