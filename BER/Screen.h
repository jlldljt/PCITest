#pragma once

//========================================================��Ļʶ����Ҫ����START==========================================================================================//
#include "PX1000API.h"
#include "rgbdefs.h"
#include "ColorSpace.h"
typedef struct
{
	BITMAPINFOHEADER bmiHeader;	
	DWORD       colorMark[3];	//���ڵ�ǰ֧�ֵ���32λ����ģʽ��������Ҫ3��������DWORD���ֱ��ź죬�̣���������ο�MSDN
}  RGBBITMAP, *PRGBBITMAP;
//========================================================��Ļʶ����Ҫ����END==========================================================================================//
// CScreen �Ի���

class CScreen : public CDialogEx
{
	DECLARE_DYNAMIC(CScreen)

public:
	CScreen(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScreen();
	void DrawBmp(CDC*pDC,double xx,double yy);

//========================================================��Ļʶ����Ҫ����START==========================================================================================//
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
//========================================================��Ļʶ����Ҫ����END==========================================================================================//
	int degree[30];//����ʶ���������
	int *g_num;//����10������0,1�ֲ�
	int g_height;//ͼ��Ŀ��  ������ռ�ֽ�  
	int	g_width;
	int g_widthBytes;
	BYTE *g_lpGray;//�����ֵ�����ͼ������
	int head[10][8],height_num,width_num;//ͼƬ��־����
	void LoadMark(CString file_temp);
	void FindOrigin(BYTE* lpGray);
	void FindOriginEX(BYTE* lpGray);
	void GetNumber();
	void ScanDegree(BYTE * lpGray,CString file_temp);
	void ScanDegreeEX(BYTE * lpGray,CString file_temp);
	void ScanDegreeDynamic(BYTE * lpGray,CString file_temp);
	bool ScanDegreeEFGM(BYTE * lpGray,CString file_temp);
	bool ScanDegreeEFGMEX(BYTE * lpGray,CString file_temp);
	BOOL SaveImageToFile(CString sFile, int iW, int iH, BYTE * pInfo) ;
	bool RCGVGA();
// �Ի�������
	enum { IDD = IDD_DIALOG_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnVgasnap();
	afx_msg void OnBnClickedBtnVgainit();
	afx_msg void OnBnClickedBtnVgascr();
	afx_msg void OnBnClickedBtnVgastop();
	afx_msg void OnBnClickedBtnVgaprt();
	afx_msg void OnBnClickedBtnLocal();
	virtual BOOL OnInitDialog();
	double m_print_xx;
	double m_print_yy;
};
extern CScreen *g_dlgScreen;