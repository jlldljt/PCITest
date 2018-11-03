// PX1000API.h : Defines the interface for the DLL.
// *****************************************************************************
//  PX1000API   VERSION:  2.0.0   DATE: 2010-07-02
//  ----------------------------------------------------------------------------
//		FileName: 	PX1000API
//		Created:	2010-07-02   14:55
//		Author:		�Ͼ�
//		Purpose:	RGB�ɼ�����ӿڵ�����	
//		Version:	
//					��V2.0.0 2010-07-02 �Ͼ꡿
//						Initial
//		Remark:	
//				ʹ��ʱֻ�����PX1000API.hͷ�ļ����ɡ�
//  ----------------------------------------------------------------------------
//  Copyright (C) 2010 
//	Nanjing Talent Electronics and Technology Co., LTD.
//  All Rights Reserved
// *****************************************************************************


#ifndef _RGBPROSDK_API_H_
#define _RGBPROSDK_API_H_
 
#ifdef RGBPROSDK_EXPORTS
	#define RGBPROSDKAPI __declspec(dllexport)
#else
	#define RGBPROSDKAPI __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif


#ifndef RGBCAPI
#define RGBCAPI __stdcall
#endif

#include "RGBDEFS.H"

typedef  void (CALLBACK RGBNOTIFYFN)(PVOID NotifyArg, 
                                    UINT   uMsg, 
                                    WPARAM wParam,
                                    LPARAM lParam);

typedef unsigned long   HRGBCAPTURE, *PHRGBCAPTURE;

typedef struct SRGBBuffer
{
   LPVOID         LpVoidBuffer;  /* Pointer application uses to access data. */
   unsigned long  Index;         /* Used to identify buffer to RGB device. */
   BOOL bufferflags;

   SRGBBuffer()
	   : bufferflags(FALSE)
   {

	   Index = 0;

   }
}  RGBCAPTUREBUFFER, *PRGBCAPTUREBUFFER;


typedef struct tagVDIF
{
   unsigned long  HorFrequency;     /* Line rate in Hz. */
   unsigned long  VerFrequency;     /* Refresh rate in Hz*1000. */
   unsigned long  PixelClock;       /* Dot clock in Hz. */

   unsigned short Flags;            /* Bitwise OR of VDIF_FLAG_.*. */

   /* The following values are in pixels. */
   unsigned long  HorAddrTime;      /* Amount of active video (resolution). */
   unsigned long  HorRightBorder;
   unsigned long  HorFrontPorch;
   unsigned long  HorSyncTime;
   unsigned long  HorBackPorch;
   unsigned long  HorLeftBorder;

   /* The following values are in lines. */
   unsigned long  VerAddrTime;      /* Amount of active video (resolution). */
   unsigned long  VerBottomBorder;
   unsigned long  VerFrontPorch;
   unsigned long  VerSyncTime;
   unsigned long  VerBackPorch;
   unsigned long  VerTopBorder;

   /* TODO: Could make this [1] or a pointer. */
   char          Description[128];
}  VDIF, *PVDIF;
/************************************************************************/
/* v2.0.1changed :  �޸İ汾����*/
typedef struct tagVERSIONINF
{
	unsigned char major;
	unsigned char sub1;
	unsigned char sub2;
	tagVERSIONINF()
		: major(0)
		, sub1(0)
		, sub2(0)
	{

	}
} VERSIONINF;


typedef struct tagRGBCAPTUREPARMS
{
	unsigned long  Size;          /* Size in bytes of this structure. */
	unsigned long  Flags;         /* Indicates which fields are relevant. */
	unsigned long  Input;         /* Which input connector to capture from. */
	unsigned long  Format;        /* Format of RGB signal to be captured. */
	VDIF          VideoTimings;  /* The characteristics of the signal. */
	unsigned long  Phase;         /* Aligns samples with pixels in signal. */
	unsigned long  Brightness;    /* Modifies DC offset. */
	unsigned long  Brightness_R;
	unsigned long  Brightness_G;
	unsigned long  Brightness_B;
	unsigned long  Contrast;      /* Input voltage/gain. */
	unsigned long  Contrast_R;
	unsigned long  Contrast_G;
	unsigned long  Contrast_B;
	unsigned long  BlackLevel;    /* When to sample black level (number of
								  * pixels from the reference point). */
	unsigned long  SampleRate;    /* Frames to drop between samples. */
	unsigned long  PixelFormat;   /* Pixel format for frame buffer reads. */
	unsigned long  SyncEdge;      /* The HSYNC edge to use as reference. */
	signed long    HorScale;      /* . */
	signed long    HorOffset;     /* . */
	signed long    VerOffset;     /* . */
	HWND           HWnd;          /* Window to receive messages from ISR. */
	unsigned long  HScaled;       /* horizontal size after scaling */
	unsigned long  VScaled;       /* vertical size after scaling */
	RGBNOTIFYFN   *NotifyFn;      /* Pointer to the notification function */
	PVOID          NotifyArg;     /* Pointer to a user supplied argument for the
								  * notification function. */
	unsigned long  NotifyFlags;   /* Indicates the required notification 
								  * messages. */
	unsigned long  datamode;      /*rgb or yuv*/
	unsigned long  PLLDivider;	 /*�������*/
	unsigned long  BuffersNumber; /*���������� 0-3����1-2��*/
	unsigned char  ProductID[8];   //��Ʒid
	VERSIONINF  FpgaVersion;   //�̼��汾��
	VERSIONINF  DriverVersion;  //�����汾��
	VERSIONINF  SdkVersion;	//SDK�汾��
	unsigned long  FramespSec;	/*ÿ��֡��*/
	unsigned long   graytest;   //			����Ϊ1���кڰ�ͼ�����
						//			����Ϊ2���кڰ����ؼ��
						//			����Ϊ4���кڰ����߼��
						//			����Ϊ0����VGA/DVIͼ�����

	unsigned long    Detectflag;		//�Զ����ֱ��ʿ���
	unsigned long  src[7]; /*����*/

   tagRGBCAPTUREPARMS()
   {
	   Detectflag = 1;
	   for (int i=0;i<8;i++)
	   {
		   ProductID[i] = 0;
	   }
   }

} RGBCAPTUREPARMS, *PRGBCAPTUREPARMS;
/************************************************************************/


/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureInitialise (
   unsigned long *pNumberOfDevices );

//��������RGBCaptureInitialise
//������������ʼ��ͼ��ɼ���
//���������lpNumberOfDevices(��Ųɼ���������ָ��)   
//����������ɹ�:RGBERROR_NO_ERROR
//          ʧ��:RGBERROR_HARDWARE_NOT_FOUND  // û���ҵ�VGA��Ƶ�ɼ���    

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureOpen (
   unsigned long deviceIndex,
   PHRGBCAPTURE  pHRGBCapture );

//��������RGBCaptureOpen
//������������ʼ��ָ����VGA��Ƶ�ɼ����������豸���
//���������	deviceIndex		������Ҫ��ʼ�����豸�ţ���0��ʼ
//���������	pHRGBCapture	�����豸���
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
//          ʧ��:   RGBERROR_INVALID_POINTER  //û�д�Ŵ��豸�����ָ��
// 					RGBERROR_HARDWARE_NOT_FOUND  //û���ҵ�VGA��Ƶ�ɼ���
// 					RGBERROR_INVALID_INDEX  //�����ڵ��豸��
// 					RGBERROR_DEVICE_IN_USE  //�豸�ѱ�ռ��
// 					RGBERROR_UNABLE_TO_LOAD_DRIVER  //�����ڴ���Դʧ��
// 					RGBERROR_INVALID_DEVICE  //���豸ʧ��
/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureClose (
   HRGBCAPTURE hRGBCapture );

//��������	RGBCaptureClose
//����������	�ر�ָ����VGA��Ƶ�ɼ���
//���������	hRGBCapture 	�豸���
//���������	��
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
//          ʧ��:	RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureEnable (
   HRGBCAPTURE    hRGBCapture,
   unsigned long bEnable );

//��������	RGBCaptureEnable
//��������������VGA��Ƶ�ɼ�
//���������hRGBCapture	�豸���
//			bEnable		�Ƿ�����
//���������	��
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
//          ʧ��:	RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��
// 					RGBERROR_CAPTURE_OUTSTANDING  //�豸δ����
// 					RGBERROR_THREAD_FAILURE  //�����źŵ�ʧ��

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureGetParameters (
   HRGBCAPTURE       hRGBCapture,
   PRGBCAPTUREPARMS  pRGBCaptureParms,
   unsigned long     ulFlag );

//��������	RGBCaptureGetParameters
//������������ȡVGA��Ƶ�ɼ������ò���
//���������hRGBCapture	�豸���
//			ulFlag		�������ͱ�ʶ
//���������pRGBCaptureParms		�������ò���
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
//          ʧ��:   RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��
// 					RGBERROR_INVALID_FLAGS  //��֧�ֵ� ulFlag
// 					RGBERROR_INVALID_POINTER  //û�д�Ŵ��豸�����ָ��
// 					RGBERROR_INVALID_SIZE  //��������ò�����С��pRGBCaptureParms->Size��

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureSetParameters (
   HRGBCAPTURE       hRGBCapture,
   PRGBCAPTUREPARMS  pRGBCaptureParms,
   unsigned long     ulFlag );

//��������	RGBCaptureSetParameters
//��������������VGA��Ƶ�ɼ������ò���
//���������hRGBCapture	�豸���
//			pRGBCaptureParms		ָ�������ò���
//			ulFlag		���ò������ͱ�ʶ
//�����������
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
// 			ʧ��:   RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��
// 					RGBERROR_INVALID_POINTER  //û�д�Ŵ��豸�����ָ��
// 					RGBERROR_INVALID_SIZE  //��������ò�����С��pRGBCaptureParms->Size��
// 					RGBERROR_INVALID_FLAGS  //��֧�ֵ� ulFlag
// 					RGBERROR_INVALID_PIXEL_FORMAT  //��֧�ֵ���ʾģʽ pRGBCaptureParms->PixelFormat
// 					RGBERROR_INVALID_HWND  //��֧�ֵ���Ϣ���ܴ��� pRGBCaptureParms->HWnd
// 					RGBERROR_INVALID_FORMAT  //��֧�ֵ������źŸ�ʽ pRGBCaptureParms->Format
// 					RGBERROR_INVALID_INPUT  //��֧�ֻ�����ʧ������˿� pRGBCaptureParms->Input
// 					RGBERROR_INVALID_PHASE  //��֧�ֻ�����ʧ����λ pRGBCaptureParms-> Phase
// 					RGBERROR_INVALID_BRIGHTNESS  //��֧�ֻ�����ʧ������ pRGBCaptureParms->Brightness
// 					RGBERROR_INVALID_CONTRAST  //��֧�ֻ�����ʧ�ܶԱȶ�pRGBCaptureParms->Contrast
// 					RGBERROR_INVALID_BLACKLEVEL  //��֧�ֵĺ�ɫ�� pRGBCaptureParms->BlackLevel
// 					RGBERROR_INVALID_SYNCEDGE  //��֧�ֵ�ͬ���� pRGBCaptureParms->SyncEdge
// 					RGBERROR_VSCALED_OUT_OF_RANGE  //��֧�ֵ�ˮƽ���ű��� pRGBCaptureParms->VScaled
// 					RGBERROR_HSCALED_OUT_OF_RANGE  //��֧�ֵĴ�ֱ���ű��� pRGBCaptureParms->HScaled
// 					RGBERROR_SCALING_NOT_SUPPORTED  //��֧�ֻ�����ˮƽƫ��ʧ��pRGBCaptureParms->HorOffset

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureFrameBufferReady (
   HRGBCAPTURE    hRGBCapture,
   unsigned long  applicationBuffer,
   unsigned long  ulNumberBytes );

//��������	RGBCaptureFrameBufferReady
//����������֪ͨSDK�ɼ��������Ѿ���
//���������hRGBCapture	�豸���
//			applicationBuffer	�ɼ�����������
//			ulNumberBytes		�ɼ���������С
//�����������
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
//          ʧ��:RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��
// 				 RGBERROR_INVALID_INDEX   //��Ч�Ĳɼ���������
// 				 RGBERROR_BUFFER_TOO_SMALL  //�ɼ�����̫С

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureDetectVideoMode (
   HRGBCAPTURE hRGBCapture,
   int ratioN,
   int RatioD,
   VDIF * pResultVdif,
   BOOL bForceInterlaced);

//��������	RGBCaptureDetectVideoMode
//������������⵱ǰ�����VGA��Ƶ�źŷֱ���
//���������hRGBCapture	�豸���
//			ratioN, RatioD, bForceInterlaced	��Ч����
//���������pResultVdif	���ػ�ȡ������ź�ģʽ
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
//          ʧ��:RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��
//         		 RGBERROR_INVALID_POINTER  //û�д�ŵ�ǰ��Ƶʱ�ӵ�ָ��VideoTimings

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureUseMemoryBuffer (
   HRGBCAPTURE hRGBCaptureBogus, 
   LPVOID pMemory, 
   DWORD dwMemorySize,
   DWORD *pdwIndex);

//��������	RGBCaptureUseMemoryBuffer
//����������ע���û�����Ĳɼ�������
//���������hRGBCaptureBogus	�豸���
//			pMemory			�û�����Ĳɼ�������ָ��
//			dwMemorySize		�û�����Ĳɼ���������С
//���������pdwIndex			���زɼ�����������
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
// 			ʧ��:RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��
// 		         RGBERROR_INVALID_POINTER  //��ŵ�ǰ�ɼ������ָ�����

/* Allows captures to use buffers which originate in the application, or from
*  libraries other than the RGBCapture SDK library.
*/

/************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
 RGBCaptureReleaseBuffer (
   HRGBCAPTURE hRGBCapture, 
   DWORD dwIndex);
//��������	RGBCaptureReleaseBuffer
//����������ע���û�����Ĳɼ�������
//���������hRGBCapture	�豸���
//			dwIndex		�ɼ�����������
//�����������
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
//          ʧ��:RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��
// 		         RGBERROR_INVALID_INDEX  //��Ч�Ĳɼ���������

/************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
 RGBCaptureUpdateFpga (
   HRGBCAPTURE hRGBCapture, 
   const char * updatepath);
//��������	RGBCaptureUpdateFpga
//�������������¹̼��汾
//���������hRGBCapture	�豸���
//			updatepath  �ļ�·��
//�����������
//����ֵ��	�ɹ�:RGBERROR_NO_ERROR
//          ʧ��:RGBERROR_INVALID_HRGBCAPTURE  //�豸δ��ʹ��
// 		         RGBERROR_INVALID_INDEX  //��Ч�Ĳɼ���������



//////////////////////////////////////////////////////////////////////////
//���Խ��
//////////////////////////////////////////////////////////////////////////
RGBPROSDKAPI ULONG RGBCAPI
 RGBCapturePCIRd (
   HRGBCAPTURE hRGBCapture
   , unsigned long ulAddr);

RGBPROSDKAPI VOID RGBCAPI
 GRBCapturePCIWr (
   HRGBCAPTURE hRGBCapture
   , unsigned long ulAddr,unsigned long ulRegValue);

RGBPROSDKAPI ULONG RGBCAPI
 GRBCapturePCILocalRd (
   HRGBCAPTURE hRGBCapture
   , unsigned long ulAddr);

RGBPROSDKAPI VOID RGBCAPI
 GRBCapturePCILocalWr (
   HRGBCAPTURE hRGBCapture
   , unsigned long ulAddr,unsigned long ulRegValue);

RGBPROSDKAPI bool RGBCAPI
 RGBCaptureGetDMADATA (
   HRGBCAPTURE hRGBCapture,				/* �豸���*/
	PUCHAR	pOutputBuf,				/* �������������*/
	ULONG	ulReadNum,				/* ��Ҫ��ȡ���ֽ���*/
	PULONG	pulDMANum,				/* ʵ�ʶ��õ�DMA�ֽ���*/
	PDWORD 	pError);





#ifdef __cplusplus
}
#endif

 #endif//_RGBPROSDK_API_H_
