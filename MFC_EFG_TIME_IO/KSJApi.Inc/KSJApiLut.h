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

// �ο������Ӽѹ�ҵ���API����˵���飨���ұ�ƪ����

// ��ȡ�ļ��е�LUT���ݣ���LUT�������ص�����У����ݹ̼��汾��ͬ�������п��ܻᶪʧ��
KSJ_API  int __stdcall KSJ_LutFileDownload( int nChannel, const TCHAR *pszKtbFile );
// ��LUT�������ص�����У����ݹ̼��汾��ͬ�������п��ܻᶪʧ��
KSJ_API  int __stdcall KSJ_LutDataDownload( int nChannel, unsigned char *pData, int nBytes   );
// ʹ��LUTת������
KSJ_API  int __stdcall KSJ_LutSetEnable( int nChannel, bool bEnable   );
// ��ȡLUTת�������Ƿ�ʹ��
KSJ_API  int __stdcall KSJ_LutGetEnable( int nChannel, bool *pbEnable );

// ��ȡktb�е�LUT���ݣ������ݹ̻�������ڲ�
KSJ_API  int __stdcall KSJ_LutFileProgram( int nChannel, TCHAR *pszFile );
// ��LUT���ݹ̻�������ڲ�
KSJ_API  int __stdcall KSJ_LutDataProgram( int nChannel, unsigned char *pData, int nBytes );
// �ӹ̻���EEPROM����ж���LUT����
KSJ_API  int __stdcall KSJ_LutDataReadout( int nChannel, unsigned char *pData, int *pnBytes );


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////Gamma Lut
KSJ_API  int __stdcall KSJ_GammaLutFileDownload( int nChannel, const TCHAR *pszKtbFile );

KSJ_API  int __stdcall KSJ_GammaLutDataDownload( int nChannel, unsigned short *pData, int nWords );
// ʹ��Gamma LUTת������
KSJ_API  int __stdcall KSJ_GammaLutSetEnable( int nChannel, bool bEnable   );
// ��ȡGamma LUTת�������Ƿ�ʹ��
KSJ_API  int __stdcall KSJ_GammaLutGetEnable( int nChannel, bool *pbEnable );

KSJ_API  int __stdcall KSJ_GammaLutFileProgram( int nChannel, TCHAR *pszFile );

KSJ_API  int __stdcall KSJ_GammaLutDataProgram( int nChannel, unsigned short *pData, int nWords );

// �ӹ̻���EEPROM����ж���LUT����
KSJ_API  int __stdcall KSJ_GammaLutDataReadout( int nChannel, unsigned short *pData, int *pnWords );



#ifdef __cplusplus
}
#endif

#endif