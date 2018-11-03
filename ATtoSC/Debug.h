#pragma once
#include "PX1000API.h"
#include "afxwin.h"
#include "rgbdefs.h"
#include "afxwin.h"
#include "ColorSpace.h"
#include "afxcmn.h"

UINT DebugDisplay(LPVOID pParam);//�̺߳���
UINT TestStart(LPVOID pParam);//��ʾ
UINT DynamicDetect(LPVOID pParam);//��̬���
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

// CDebug �Ի���

class CDebug : public CDialogEx
{
	DECLARE_DYNAMIC(CDebug)

public:
	CDebug(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDebug();
	void DrawBmp(CDC*pDC);
// �Ի�������
	enum { IDD = IDD_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
			Purpose:		����ָ����С���仺��
			Parameter:		
							hWnd ��ǰ��Ĳɼ��ؼ��Ĵ��ھ��
							hRGBCapture ��ǰ��Ĳɼ������
							pCaptureBufferָ��ǰ��¼��ɼ����軺���ָ��
							BufferSize ��������Ļ����С
			Return:		
			                �ɼ�������ָ��pCaptureBuffer
							�ɹ����أ�TRUE                         RGBERROR_NO_ERROR
							ʧ�ܷ��أ�FALSE                        RGBERROR_INVALID_HRGBCAPTURE
							                                       RGBERROR_INVALID_POINTER
									                               RGBERROR_INVALID_INDEX
			Remark:			N/A
		*****************************************************************************/
		BOOL AllocateBuffers( HRGBCAPTURE hRGBCapture
                              , RGBCAPTUREBUFFER *pCaptureBuffer
							  , HWND hWnd
							  , UINT BufferSize);     //���仺��
	/*****************************************************************************
			FunctionName:	FreeBuffers
			Purpose:		�ͷŻ���
			Parameter:		
							hWnd ��ǰ��Ĳɼ��ؼ��Ĵ��ھ��
							hRGBCapture ��ǰ��Ĳɼ������
							pCaptureBufferָ��ǰ��¼��ɼ�����Ļ����ָ��
			Return:			
							�ɹ����أ�TRUE                     RGBERROR_NO_ERROR
							ʧ�ܷ��أ�FALSE                    RGBERROR_INVALID_HRGBCAPTURE
							                                   RGBERROR_INVALID_INDEX
			Remark:			N/A
		*****************************************************************************/
		BOOL FreeBuffers( HRGBCAPTURE hRGBCapture
						  , HWND hWnd 
						  , RGBCAPTUREBUFFER *pCaptureBuffer);  //�ͷŻ�����
		/*****************************************************************************
			FunctionName:	CountBits
			Purpose:		��������λ��
			Parameter:		
							ulValue ��ŵ�ǰ���λͼ���������ݻ���
							start  ��ʼ������ֵ��Ϊ0
							end  ���������ݵ�������
			Return:			��������λ��

			Remark:			N/A
		*****************************************************************************/
	    int CountBits ( unsigned long  ulValue
					    , int            start
						, int            end );    //��������λ��
		/*****************************************************************************
			FunctionName:	DetectPixelFormat
			Purpose:		��⵱ǰ����ģʽ
			Parameter:		
							hWnd ��ǰ��Ĳɼ��ؼ��Ĵ��ھ��
							pColourMaskָ��һ�����������ֵ����飬ÿ���ֱַ�װ���ŵ�ǰͼ��λ�ĺ죬�̣���ɫ������  

			Return:		    
			                ���ص�ǰ����ģʽ:
							RGBPIXELFORMAT_555
                            RGBPIXELFORMAT_565
			                RGBPIXELFORMAT_888
                            RGBPIXELFORMAT_UNKNOWN

			Remark:			N/A
		*****************************************************************************/
		unsigned short DetectPixelFormat ( HWND hWnd 
										   , DWORD *pColourMask );//��⵱ǰ����ģʽ
	/*****************************************************************************
			FunctionName:	SetCapture
			Purpose:		���òɼ�����
			Parameter:		
			                hWnd ��ǰ��Ĳɼ��ؼ��Ĵ��ھ��
			                pCaptureParmsָ��ǰ�ɼ��¼��Ĳ�����ָ��
							phRGBCapture ָ��ǰ��Ĳɼ��������ָ��
							pCaptureBufferָ��ǰ��¼��ɼ����軺���ָ��
							pBufferSize ָ�򻺴��С��ָ��
							pRGBBitmap λͼ��Ϣָ��
			Return:		    
							�ɹ����أ�TRUE                            RGBERROR_NO_ERROR
							ʧ�ܷ��أ�FALSE                           RGBERROR_INVALID_HRGBCAPTURE
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
						 , RGBBITMAP        *pRGBBitmap);//���òɼ�����
	void OnMyMessage_Sta1(WPARAM wParam, LPARAM lParam);
	void OnMyMessage_modechange(WPARAM wParam, LPARAM lParam);
	void OnMyMessage_nosignal(WPARAM wParam, LPARAM lParam);
	
	unsigned long m_ulNumofcard; //��⵽�İ忨����
	int m_iIndex;
	HRGBCAPTURE hRGBCapture; //��1���ɼ�����Ӧ�ľ��
	RGBCAPTUREPARMS pCaptureParms;
	//RGBCAPTUREBUFFER pCaptureBuffer;
	UINT BufferSize;
	HANDLE  m_hDDrawEvent;
	int emp_flag;
	unsigned short   pixelFormat;             //����ģʽ
	/*
	*�źű�־
	*/
	bool signalflag;
	/*
	*�ɼ��¼�λͼ��Ϣ
	*/
	//RGBBITMAP       RGBBitmap1;     //��һ���ɼ��¼�λͼ��Ϣ
	/*
	*���ݴ����ʽ��0��yuv��Ĭ�ϣ���1��rgb��
	*/
	DWORD datamode;
	/*
	*�ɼ�����Ӧ�Ļ����С
	*/
	unsigned long    bufferSize;         //��1���ɼ�����Ӧ�Ļ����С
	/*
	*��־��ǰ�����Ƿ��ѱ�ʹ��
	*/
	bool      bUsingAccBuffer1;  //��־��ǰ�����Ƿ��ѱ�ʹ�ã�true�ǣ�false��
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
