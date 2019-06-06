#ifndef __KSJ_API_F_H__
#define __KSJ_API_F_H__

#pragma message("Include KSJApiF.h") 

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

// ���֡�����
KSJ_API  int __stdcall KSJ_EmptyFrameBuffer ( int nChannel );

// ����ģʽ����
enum KSJ_TRIGGERMODE { KSJ_TRIGGER_INTERNAL, KSJ_TRIGGER_EXTERNAL, KSJ_TRIGGER_SOFTWARE, KSJ_TRIGGER_FIXFRAMERATE };
KSJ_API  int __stdcall KSJ_TriggerModeSet ( int nChannel, KSJ_TRIGGERMODE TriggerMode   );
KSJ_API  int __stdcall KSJ_TriggerModeGet ( int nChannel, KSJ_TRIGGERMODE *pTriggerMode );

// ͨ������FPGA�Ĵ�����������ù̶�֡�ʣ����ô˺���֮ǰ���������������ΪKSJ_TRIGGER_FIXFRAMERATEģʽ��ͨ����ȡKSJ_SUPPORT_FIXED_FRAMERATE��ѯ�˹��ܡ�
KSJ_API  int __stdcall KSJ_SetFixedFrameRateEx( int nChannel, float fFrameRate );
KSJ_API  int __stdcall KSJ_GetFixedFrameRateEx( int nChannel, float *pfFrameRate );

// ���ù̶�֡��(ͨ������sensor�Ĵ���,ʹsensor����̶�֡��)��ֻ��MUC36C/M(MGYY)��MU3E130C/M(CGYY)֧��
// V4.0.86.12�汾��ʼ��bEnable������Ч��ͨ���û�����KSJ_TRIGGER_FIXFRAMERATEģʽ�������Ƿ�ʹ�ܡ�
KSJ_API  int __stdcall KSJ_SetFixedFrameRate( int nChannel, bool bEnable,   float fFrameRate );
KSJ_API  int __stdcall KSJ_GetFixedFrameRate( int nChannel, bool *pbEnable, float *pfFrameRate );


// ����У������ֵ�͸���ֵ���ã�Ӳ����֧�ִ����ã����������ʱ��Ч��
KSJ_API  int __stdcall KSJ_BadPixelCorrectionSetThreshold( int nChannel, unsigned char btLow,   unsigned char btHigh   );
KSJ_API  int __stdcall KSJ_BadPixelCorrectionGetThreshold( int nChannel, unsigned char *pbtLow, unsigned char *pbtHigh );

enum KSJ_THRESHOLD { KSJ_THRESHOLD_LOW, KSJ_THRESHOLD_HIGH };
// ����У�����ü���ֵ���ã�����ֵ����˸���Ļ���
KSJ_API  int __stdcall KSJ_BadPixelCorrectionSet( int nChannel, bool bEnable,   KSJ_THRESHOLD Threshold );
KSJ_API  int __stdcall KSJ_BadPixelCorrectionGet( int nChannel, bool *pbEnable, KSJ_THRESHOLD *pThreshold);

// ������ģʽ���ã�ʹ�ý�ȡ8λ��ʽ��
enum KSJ_SENSITIVITYMODE { KSJ_LOW = 0, KSJ_MID, KSJ_HIGH, KSJ_HIGHER, KSJ_HIGHEST };
KSJ_API  int __stdcall KSJ_SensitivitySetMode( int nChannel, KSJ_SENSITIVITYMODE SensitivityMode   );
KSJ_API  int __stdcall KSJ_SensitivityGetMode( int nChannel, KSJ_SENSITIVITYMODE *pSensitivityMode );

// ������Ϊ16���ش���ģʽʱ���ɼ����ݱ�����ʱ��һ����
// ������Ϊ16���ش���ģʽʱ��Ԥ��ͼ����ͨ��KSJAPI��16���ص�����ת��Ϊ8���ص�������ʾ�ġ�ת����ʽ��������ģʽ�����ü��Ƿ������LUT��������
// ���������LUT����LUT��ת����������������ģʽ��ת����

// �������Ϊ16bit�Ĳɼ�ģʽ,����ʵ��AD��ת������,��Чλ�����Ը���KSJ_GetADCResolution������ȡ
KSJ_API  int __stdcall KSJ_SetData16Bits ( int nChannel, bool bEnable  );
KSJ_API  int __stdcall KSJ_GetData16Bits ( int nChannel, bool *bEnable );

// �ⴥ��ģʽʱ���ⴥ���źŵĴ�����ʽ����
enum KSJ_TRIGGERMETHOD { KSJ_TRIGGER_FALLINGEDGE, KSJ_TRIGGER_RISINGEDGE, KSJ_TRIGGER_HIGHLEVEL, KSJ_TRIGGER_LOWLEVEL };
KSJ_API  int __stdcall KSJ_TriggerMethodSet ( int nChannel, KSJ_TRIGGERMETHOD  TriggerMethod  );
KSJ_API  int __stdcall KSJ_TriggerMethodGet ( int nChannel, KSJ_TRIGGERMETHOD *pTriggerMethod  );

// �ⴥ��ģʽʱ�����ô����ź���Ч���ɼ�����ʱʱ��
KSJ_API  int __stdcall KSJ_TriggerDelaySet( int nChannel, unsigned short wDelay   );
KSJ_API  int __stdcall KSJ_TriggerDelayGet( int nChannel, unsigned short *pwDelay );
KSJ_API  int __stdcall KSJ_TriggerDelayGetRange( int nChannel, unsigned short *wMin, unsigned short *wMax );

// �����֡����ٲɼ�ԭʼͼ��
KSJ_API  int __stdcall KSJ_CaptureRawDataAfterEmptyFrameBuffer     ( int nChannel, unsigned char *pRawData );
// �����֡����ٲɼ���ɫͼ��
KSJ_API  int __stdcall KSJ_CaptureRgbDataAfterEmptyFrameBuffer     ( int nChannel, unsigned char *pRgbData );
// �����֡����ٲɼ���ɫͼ��
KSJ_API  int __stdcall KSJ_CaptureRgbDataExAfterEmptyFrameBuffer   ( int nChannel, unsigned char *pRgbData, int *pnWidth, int *pnHeight, int *pnBitCount );

// ������ź������ʵ������������ʱ���ã��ݲ�֧�֣�
KSJ_API  int __stdcall KSJ_StrobeDelaySet( int nChannel, unsigned short wDelay   );
KSJ_API  int __stdcall KSJ_StrobeDelayGet( int nChannel, unsigned short *pwDelay );
KSJ_API  int __stdcall KSJ_StrobeDelayGetRange( int nChannel, unsigned short *wMin, unsigned short *wMax );

// ��������ƴ����źŵ�ʹ��,��Ч��ƽ,����ģʽ
KSJ_API  int __stdcall KSJ_FlashControlSet ( int nChannel, bool bEnable,   bool bInvert,   int nMode   );
KSJ_API  int __stdcall KSJ_FlashControlGet ( int nChannel, bool *pbEnable, bool *pbInvert, int *pnMode );

enum KSJ_FLASHMODE { KSJ_FM_TROW, KSJ_FM_SIMULTANEOUSEXPOSURE, KSJ_FM_SHUTTERWIDTH };
KSJ_API  int __stdcall KSJ_FlashSetMode ( int nChannel, KSJ_FLASHMODE FlashMode   );
KSJ_API  int __stdcall KSJ_FlashGetMode ( int nChannel, KSJ_FLASHMODE *pFlashMode );

// ����ͼ�񴫸����Ĺ�����ʽ��V4.0.86.12֧��500W�����
enum KSJ_SENSORMODE { KSJ_SM_TRIGGER_MODE, KSJ_SM_FREERUN_MODE, KSJ_SM_GRR_MODE };
KSJ_API  int __stdcall KSJ_SensorModeSet ( int nChannel, KSJ_SENSORMODE SensorMode  );
KSJ_API  int __stdcall KSJ_SensorModeGet ( int nChannel, KSJ_SENSORMODE *SensorMode  );


#ifdef __cplusplus
}
#endif

#endif