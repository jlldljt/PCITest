#ifndef __KSJ_API_H_H__
#define __KSJ_API_H_H__

#pragma message("Include KSJApiH.h") 


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

// �õ�sensor��������ı���λ��������ʶ��16λ�������ݵ���Чλ�������ұ�Ĵ�С��
KSJ_API  int __stdcall KSJ_GetADCResolution( int nChannel, int *pnADCResolution );

enum KSJ_FUNCTION 
{
	KSJ_PROPERTY_MONO_DEVICE,           // �Ƿ�Ϊ�ڰ������     0 - �ǣ�     1 - ����
	KSJ_PROPERTY_ADC_RESOLUTION,        // ���ADC��ת������λ����8��10��12����KSJ_GetADCResolution������ͬ
	KSJ_SUPPORT_RESET_DEVICE,           // �Ƿ�֧�ָ�λ�豸��   0 - ��֧�֣� 1 - ֧��
	KSJ_SUPPORT_EMPTY_FRAMEBUFFER,      // �Ƿ�֧�����֡�档0 - ��֧�֣� 1 - ֧��
	KSJ_SUPPORT_LUT,                    // �Ƿ�֧��Ӳ�����ұ����ز�ʹ�ܣ��� 0 - ��֧�֣� 1 - ֧��
	KSJ_SUPPORT_16BITS,			        // �Ƿ�֧��˫�ֽڴ��䣬һ��������2���ֽڣ����ݶ�ȡ�ӱ���0 - ��֧�֣�1 - ֧��
	KSJ_SUPPORT_MULTI_FRAMES,	        // �Ƿ�֧��һ�ζ�ȡ��֡���ݽ��д�ֱƴͼ��0 - ��֧�֣�1 - ֧��
	KSJ_SUPPORT_FIXED_FRAMERATE,	    // �Ƿ�֧��ͨ������sensor�̶�֡�ʡ�0 - ��֧�֣�1 - ֧��
	KSJ_SUPPORT_TRIGGER_DELAY,		    // �Ƿ�֧�ֶ��ⴥ���źŵ���ʱ��0 - ��֧�֣�1 - ֧��
	KSJ_SUPPORT_FAST_UPDATE,            // �Ƿ�֧�ֿ�������
	KSJ_SUPPORT_WORKINGMODE_PROGRAM,    // �Ƿ�֧�ֹ̻�����ģʽ
	KSJ_SUPPORT_SERIALSDESC_PROGRAM,    // �Ƿ�֧�ֹ̻��豸���������
	KSJ_SUPPORT_COLORMODE_PROGRAM,      // �Ƿ�֧�ֹ̻�ɫ��ģʽ
	KSJ_SUPPORT_LUT_PROGRAM,            // �Ƿ�֧�ֹ̻�LUT���ұ�����
	KSJ_SUPPORT_FIXED_FRAMERATE_EX,     // �Ƿ�֧������FPGA�̶�����Ƶ��
	KSJ_SUPPORT_GAMMA_LUT_DOWNLOAD,     // �Ƿ�֧��Gamma LUT���ص�FPGA�����֧�ִ˹��ܣ��Ϳ���ʹ�ܲ���
	KSJ_SUPPORT_GAMMA_LUT_PROGRAM,      // �Ƿ�֧�ֹ̻�Gamma LUT�������
	KSJ_SUPPORT_WORKINGMODEEX_PROGRAM,  // �Ƿ�֧�ֹ̻���������ģʽ
	KSJ_SUPPORT_COLORMATRIX_PROGRAM     // �Ƿ�֧�ֽ�WB,CCMУ������̻�

};

KSJ_API  int __stdcall KSJ_QueryFunction( int nChannel, KSJ_FUNCTION Function, int *pInfo ); 

#ifdef __cplusplus
}
#endif

#endif