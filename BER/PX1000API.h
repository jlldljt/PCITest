// PX1000API.h : Defines the interface for the DLL.
// *****************************************************************************
//  PX1000API   VERSION:  2.0.0   DATE: 2010-07-02
//  ----------------------------------------------------------------------------
//		FileName: 	PX1000API
//		Created:	2010-07-02   14:55
//		Author:		孟娟
//		Purpose:	RGB采集卡类接口的声明	
//		Version:	
//					【V2.0.0 2010-07-02 孟娟】
//						Initial
//		Remark:	
//				使用时只需包含PX1000API.h头文件即可。
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
/* v2.0.1changed :  修改版本参数*/
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
	unsigned long  PLLDivider;	 /*点阵调节*/
	unsigned long  BuffersNumber; /*缓冲区数量 0-3个；1-2个*/
	unsigned char  ProductID[8];   //产品id
	VERSIONINF  FpgaVersion;   //固件版本号
	VERSIONINF  DriverVersion;  //驱动版本号
	VERSIONINF  SdkVersion;	//SDK版本号
	unsigned long  FramespSec;	/*每秒帧数*/
	unsigned long   graytest;   //			设置为1进行黑白图像测试
						//			设置为2进行黑白像素间隔
						//			设置为4进行黑白竖线间隔
						//			设置为0进行VGA/DVI图像测试

	unsigned long    Detectflag;		//自动检测分辨率开关
	unsigned long  src[7]; /*保留*/

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

//函数名：RGBCaptureInitialise
//功能描述：初始化图象采集卡
//输入参数：lpNumberOfDevices(存放采集卡数量的指针)   
//输出参数：成功:RGBERROR_NO_ERROR
//          失败:RGBERROR_HARDWARE_NOT_FOUND  // 没有找到VGA视频采集卡    

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureOpen (
   unsigned long deviceIndex,
   PHRGBCAPTURE  pHRGBCapture );

//函数名：RGBCaptureOpen
//功能描述：初始化指定的VGA视频采集卡，返回设备句柄
//输入参数：	deviceIndex		输入需要初始化的设备号，从0开始
//输出参数：	pHRGBCapture	返回设备句柄
//返回值：	成功:RGBERROR_NO_ERROR
//          失败:   RGBERROR_INVALID_POINTER  //没有存放打开设备句柄的指针
// 					RGBERROR_HARDWARE_NOT_FOUND  //没有找到VGA视频采集卡
// 					RGBERROR_INVALID_INDEX  //不存在的设备号
// 					RGBERROR_DEVICE_IN_USE  //设备已被占用
// 					RGBERROR_UNABLE_TO_LOAD_DRIVER  //分配内存资源失败
// 					RGBERROR_INVALID_DEVICE  //打开设备失败
/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureClose (
   HRGBCAPTURE hRGBCapture );

//函数名：	RGBCaptureClose
//功能描述：	关闭指定的VGA视频采集卡
//输入参数：	hRGBCapture 	设备句柄
//输出参数：	无
//返回值：	成功:RGBERROR_NO_ERROR
//          失败:	RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureEnable (
   HRGBCAPTURE    hRGBCapture,
   unsigned long bEnable );

//函数名：	RGBCaptureEnable
//功能描述：启动VGA视频采集
//输入参数：hRGBCapture	设备句柄
//			bEnable		是否启动
//输出参数：	无
//返回值：	成功:RGBERROR_NO_ERROR
//          失败:	RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用
// 					RGBERROR_CAPTURE_OUTSTANDING  //设备未被打开
// 					RGBERROR_THREAD_FAILURE  //创建信号灯失败

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureGetParameters (
   HRGBCAPTURE       hRGBCapture,
   PRGBCAPTUREPARMS  pRGBCaptureParms,
   unsigned long     ulFlag );

//函数名：	RGBCaptureGetParameters
//功能描述：获取VGA视频采集卡配置参数
//输入参数：hRGBCapture	设备句柄
//			ulFlag		参数类型标识
//输出参数：pRGBCaptureParms		返回配置参数
//返回值：	成功:RGBERROR_NO_ERROR
//          失败:   RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用
// 					RGBERROR_INVALID_FLAGS  //不支持的 ulFlag
// 					RGBERROR_INVALID_POINTER  //没有存放打开设备句柄的指针
// 					RGBERROR_INVALID_SIZE  //错误的配置参数大小（pRGBCaptureParms->Size）

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureSetParameters (
   HRGBCAPTURE       hRGBCapture,
   PRGBCAPTUREPARMS  pRGBCaptureParms,
   unsigned long     ulFlag );

//函数名：	RGBCaptureSetParameters
//功能描述：设置VGA视频采集卡配置参数
//输入参数：hRGBCapture	设备句柄
//			pRGBCaptureParms		指定的配置参数
//			ulFlag		配置参数类型标识
//输出参数：无
//返回值：	成功:RGBERROR_NO_ERROR
// 			失败:   RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用
// 					RGBERROR_INVALID_POINTER  //没有存放打开设备句柄的指针
// 					RGBERROR_INVALID_SIZE  //错误的配置参数大小（pRGBCaptureParms->Size）
// 					RGBERROR_INVALID_FLAGS  //不支持的 ulFlag
// 					RGBERROR_INVALID_PIXEL_FORMAT  //不支持的显示模式 pRGBCaptureParms->PixelFormat
// 					RGBERROR_INVALID_HWND  //不支持的消息接受窗口 pRGBCaptureParms->HWnd
// 					RGBERROR_INVALID_FORMAT  //不支持的输入信号格式 pRGBCaptureParms->Format
// 					RGBERROR_INVALID_INPUT  //不支持或设置失败输入端口 pRGBCaptureParms->Input
// 					RGBERROR_INVALID_PHASE  //不支持或设置失败相位 pRGBCaptureParms-> Phase
// 					RGBERROR_INVALID_BRIGHTNESS  //不支持或设置失败亮度 pRGBCaptureParms->Brightness
// 					RGBERROR_INVALID_CONTRAST  //不支持或设置失败对比度pRGBCaptureParms->Contrast
// 					RGBERROR_INVALID_BLACKLEVEL  //不支持的黑色度 pRGBCaptureParms->BlackLevel
// 					RGBERROR_INVALID_SYNCEDGE  //不支持的同步沿 pRGBCaptureParms->SyncEdge
// 					RGBERROR_VSCALED_OUT_OF_RANGE  //不支持的水平缩放比例 pRGBCaptureParms->VScaled
// 					RGBERROR_HSCALED_OUT_OF_RANGE  //不支持的垂直缩放比例 pRGBCaptureParms->HScaled
// 					RGBERROR_SCALING_NOT_SUPPORTED  //不支持或设置水平偏移失败pRGBCaptureParms->HorOffset

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureFrameBufferReady (
   HRGBCAPTURE    hRGBCapture,
   unsigned long  applicationBuffer,
   unsigned long  ulNumberBytes );

//函数名：	RGBCaptureFrameBufferReady
//功能描述：通知SDK采集缓冲区已就绪
//输入参数：hRGBCapture	设备句柄
//			applicationBuffer	采集缓冲区索引
//			ulNumberBytes		采集缓冲区大小
//输出参数：无
//返回值：	成功:RGBERROR_NO_ERROR
//          失败:RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用
// 				 RGBERROR_INVALID_INDEX   //无效的采集缓存索引
// 				 RGBERROR_BUFFER_TOO_SMALL  //采集缓存太小

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureDetectVideoMode (
   HRGBCAPTURE hRGBCapture,
   int ratioN,
   int RatioD,
   VDIF * pResultVdif,
   BOOL bForceInterlaced);

//函数名：	RGBCaptureDetectVideoMode
//功能描述：检测当前输入的VGA视频信号分辨率
//输入参数：hRGBCapture	设备句柄
//			ratioN, RatioD, bForceInterlaced	无效参数
//输出参数：pResultVdif	返回获取的输出信号模式
//返回值：	成功:RGBERROR_NO_ERROR
//          失败:RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用
//         		 RGBERROR_INVALID_POINTER  //没有存放当前视频时钟的指针VideoTimings

/******************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
RGBCaptureUseMemoryBuffer (
   HRGBCAPTURE hRGBCaptureBogus, 
   LPVOID pMemory, 
   DWORD dwMemorySize,
   DWORD *pdwIndex);

//函数名：	RGBCaptureUseMemoryBuffer
//功能描述：注册用户分配的采集缓冲区
//输入参数：hRGBCaptureBogus	设备句柄
//			pMemory			用户分配的采集缓冲区指针
//			dwMemorySize		用户分配的采集缓冲区大小
//输出参数：pdwIndex			返回采集缓冲区索引
//返回值：	成功:RGBERROR_NO_ERROR
// 			失败:RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用
// 		         RGBERROR_INVALID_POINTER  //存放当前采集缓存的指针错误

/* Allows captures to use buffers which originate in the application, or from
*  libraries other than the RGBCapture SDK library.
*/

/************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
 RGBCaptureReleaseBuffer (
   HRGBCAPTURE hRGBCapture, 
   DWORD dwIndex);
//函数名：	RGBCaptureReleaseBuffer
//功能描述：注销用户分配的采集缓冲区
//输入参数：hRGBCapture	设备句柄
//			dwIndex		采集缓冲区索引
//输出参数：无
//返回值：	成功:RGBERROR_NO_ERROR
//          失败:RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用
// 		         RGBERROR_INVALID_INDEX  //无效的采集缓存索引

/************************************************************************/

RGBPROSDKAPI RGBERROR RGBCAPI
 RGBCaptureUpdateFpga (
   HRGBCAPTURE hRGBCapture, 
   const char * updatepath);
//函数名：	RGBCaptureUpdateFpga
//功能描述：更新固件版本
//输入参数：hRGBCapture	设备句柄
//			updatepath  文件路径
//输出参数：无
//返回值：	成功:RGBERROR_NO_ERROR
//          失败:RGBERROR_INVALID_HRGBCAPTURE  //设备未被使用
// 		         RGBERROR_INVALID_INDEX  //无效的采集缓存索引



//////////////////////////////////////////////////////////////////////////
//调试借口
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
   HRGBCAPTURE hRGBCapture,				/* 设备句柄*/
	PUCHAR	pOutputBuf,				/* 数据输出缓冲区*/
	ULONG	ulReadNum,				/* 需要读取的字节数*/
	PULONG	pulDMANum,				/* 实际读得的DMA字节数*/
	PDWORD 	pError);





#ifdef __cplusplus
}
#endif

 #endif//_RGBPROSDK_API_H_
