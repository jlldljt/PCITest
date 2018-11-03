#pragma once
#include "PX1000API.h"
#include "afxwin.h"
#include "rgbdefs.h"
#include "afxwin.h"
#include "ColorSpace.h"
#include "afxcmn.h"

UINT DebugDisplay(LPVOID pParam);//线程函数
UINT TestStart(LPVOID pParam);//显示
UINT DynamicDetect(LPVOID pParam);//动态检测
void LoadMark(CString file_temp);
void FindOrigin(BYTE* lpGray);
void FindOrigin2(BYTE* lpGray);
void FindOriginEX(BYTE* lpGray);
void GetNumber();
void ScanDegree(BYTE * lpGray,CString file_temp);
void ScanDegree2(BYTE * lpGray, CString file_temp);
void ScanDegreeEX(BYTE * lpGray,CString file_temp);
void ScanDegreeDynamic(BYTE * lpGray,CString file_temp);
void ScanDegreeDynamic2(BYTE * lpGray, CString file_temp);
bool ScanDegreeEFGM(BYTE * lpGray,CString file_temp);
bool ScanDegreeEFGMEX(BYTE * lpGray,CString file_temp);

int CalcEquAngle();

BOOL SaveImageToFile(CString sFile, int iW, int iH, BYTE * pInfo) ;

// CDebug 对话框

class CDebug : public CDialogEx
{
	DECLARE_DYNAMIC(CDebug)

public:
	CDebug(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDebug();
	void DrawBmp(CDC*pDC);
// 对话框数据
	enum { IDD = IDD_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CWinThread* p_show;
	CWinThread* p_test;
	CWinThread* p_dynamic;
	DECLARE_MESSAGE_MAP()
public:
		
	afx_msg void OnBnClickedMotor1Run();
	CComboBox m_motorturn;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAcRead();
	afx_msg void OnBnClickedAcWrite();
	afx_msg void OnBnClickedScrread();
	afx_msg void OnBnClickedVgaInit();
	/*****************************************************************************
			FunctionName:	AllocateBuffers
			Purpose:		按照指定大小分配缓存
			Parameter:		
							hWnd 当前活动的采集控件的窗口句柄
							hRGBCapture 当前活动的采集卡句柄
							pCaptureBuffer指向当前活动事件采集所需缓存的指针
							BufferSize 所需申请的缓存大小
			Return:		
			                采集缓存区指针pCaptureBuffer
							成功返回：TRUE                         RGBERROR_NO_ERROR
							失败返回：FALSE                        RGBERROR_INVALID_HRGBCAPTURE
							                                       RGBERROR_INVALID_POINTER
									                               RGBERROR_INVALID_INDEX
			Remark:			N/A
		*****************************************************************************/
		BOOL AllocateBuffers( HRGBCAPTURE hRGBCapture
                              , RGBCAPTUREBUFFER *pCaptureBuffer
							  , HWND hWnd
							  , UINT BufferSize);     //分配缓存
	/*****************************************************************************
			FunctionName:	FreeBuffers
			Purpose:		释放缓存
			Parameter:		
							hWnd 当前活动的采集控件的窗口句柄
							hRGBCapture 当前活动的采集卡句柄
							pCaptureBuffer指向当前活动事件采集所需的缓存的指针
			Return:			
							成功返回：TRUE                     RGBERROR_NO_ERROR
							失败返回：FALSE                    RGBERROR_INVALID_HRGBCAPTURE
							                                   RGBERROR_INVALID_INDEX
			Remark:			N/A
		*****************************************************************************/
		BOOL FreeBuffers( HRGBCAPTURE hRGBCapture
						  , HWND hWnd 
						  , RGBCAPTUREBUFFER *pCaptureBuffer);  //释放缓冲区
		/*****************************************************************************
			FunctionName:	CountBits
			Purpose:		数出像素位数
			Parameter:		
							ulValue 存放当前相关位图二进制数据缓存
							start  开始计数的值，为0
							end  缓存中数据的总数量
			Return:			返回像素位数

			Remark:			N/A
		*****************************************************************************/
	    int CountBits ( unsigned long  ulValue
					    , int            start
						, int            end );    //数出像素位数
		/*****************************************************************************
			FunctionName:	DetectPixelFormat
			Purpose:		检测当前像素模式
			Parameter:		
							hWnd 当前活动的采集控件的窗口句柄
							pColourMask指向一个包含三个字的数组，每个字分辨装载着当前图象位的红，绿，蓝色素数据  

			Return:		    
			                返回当前象素模式:
							RGBPIXELFORMAT_555
                            RGBPIXELFORMAT_565
			                RGBPIXELFORMAT_888
                            RGBPIXELFORMAT_UNKNOWN

			Remark:			N/A
		*****************************************************************************/
		unsigned short DetectPixelFormat ( HWND hWnd 
										   , DWORD *pColourMask );//检测当前像素模式
	/*****************************************************************************
			FunctionName:	SetCapture
			Purpose:		设置采集参数
			Parameter:		
			                hWnd 当前活动的采集控件的窗口句柄
			                pCaptureParms指向当前采集事件的参数的指针
							phRGBCapture 指向当前活动的采集卡句柄的指针
							pCaptureBuffer指向当前活动事件采集所需缓存的指针
							pBufferSize 指向缓存大小的指针
							pRGBBitmap 位图信息指针
			Return:		    
							成功返回：TRUE                            RGBERROR_NO_ERROR
							失败返回：FALSE                           RGBERROR_INVALID_HRGBCAPTURE
																	  RGBERROR_INVALID_INDEX
																	  RGBERROR_BUFFER_TOO_SMALL
																	  RGBERROR_INVALID_POINTER
			Remark:			N/A
		*****************************************************************************/
		BOOL SetCapture( HWND hWnd
						 , RGBCAPTUREPARMS  *pCaptureParms
						 , HRGBCAPTURE      *phRGBCapture
						 , RGBCAPTUREBUFFER pCaptureBuffer
						 , unsigned long    *pBufferSize
						 , RGBBITMAP        *pRGBBitmap);//设置采集参数
	void OnMyMessage_Sta1(WPARAM wParam, LPARAM lParam);
	void OnMyMessage_modechange(WPARAM wParam, LPARAM lParam);
	void OnMyMessage_nosignal(WPARAM wParam, LPARAM lParam);
	
	unsigned long m_ulNumofcard; //检测到的板卡数量
	int m_iIndex;
	HRGBCAPTURE hRGBCapture; //第1个采集卡对应的句柄
	RGBCAPTUREPARMS pCaptureParms;
	//RGBCAPTUREBUFFER pCaptureBuffer;
	UINT BufferSize;
	HANDLE  m_hDDrawEvent;
	int emp_flag;
	unsigned short   pixelFormat;             //像素模式
	/*
	*信号标志
	*/
	bool signalflag;
	/*
	*采集事件位图信息
	*/
	//RGBBITMAP       RGBBitmap1;     //第一个采集事件位图信息
	/*
	*数据传输格式（0：yuv（默认）；1：rgb）
	*/
	DWORD datamode;
	/*
	*采集卡对应的缓存大小
	*/
	unsigned long    bufferSize;         //第1个采集卡对应的缓存大小
	/*
	*标志当前缓存是否已被使用
	*/
	bool      bUsingAccBuffer1;  //标志当前缓存是否已被使用，true是；false否
	RGBBITMAP        *pRGBBitmap;
	//TPicRegion pDest;
	//void CALLBACK callbackNotify(PVOID NotifyArg,   UINT   message,   WPARAM   wParam,   LPARAM   lParam);


	afx_msg void OnBnClickedLoadPic();
	afx_msg void OnBnClickedCompare();
	afx_msg void OnBnClickedStopVga();
	CListCtrl m_debuglist;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSortMove();
	afx_msg void OnBnClickedSortTest();
	afx_msg void OnBnClickedSortSet();
	afx_msg void OnBnClickedMotor2Run();
	afx_msg void OnBnClickedTipX();
	BOOL m_tip_x;
//	CButton m_tip_y;
	BOOL m_tip_y;
	BOOL m_tip_blow;
	afx_msg void OnBnClickedTipY();
	afx_msg void OnBnClickedTipBlow();
	afx_msg void OnBnClickedYZero();
	afx_msg void OnBnClickedXZero();
//	BOOL m_shock;
	afx_msg void OnBnClickedButton1();
	CStatic m_preview;
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedDynamicDetect();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnPaint();
	BOOL m_alert;
	afx_msg void OnBnClickedAlert();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedChkDebug();
	afx_msg void OnBnClickedChkTblow();
	afx_msg void OnBnClickedEasySet();
	afx_msg void OnBnClickedMotor3Run();
	afx_msg void OnBnClickedZZero();
	afx_msg void OnBnClickedTipZ();
	afx_msg void OnBnClickedChkCy();
	afx_msg void OnBnClickedChkEfg();
	afx_msg void OnBnClickedCheck4();
};
