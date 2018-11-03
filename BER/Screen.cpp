// Screen.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BER.h"
#include "Screen.h"
#include "afxdialogex.h"

//========================================================��Ļʶ����Ҫ����START==========================================================================================//
static int PixelFormatToBytesPerPixel[] =
{
	0  /* RGBPIXELFORMAT_UNKNOWN */,
	4  /* RGBPIXELFORMAT_555 */,
	4  /* RGBPIXELFORMAT_565 */,
	8  /* RGBPIXELFORMAT_888 */,
};

RGBCAPTUREBUFFER pCaptureBuffer;//����Ļ���õ�
RGBBITMAP       RGBBitmap1;     //��һ���ɼ��¼�λͼ��Ϣ
TPicRegion pDest;				//����Ļ���õ�
int m_VGAthreshold;			//����Ļ���õ�

void CALLBACK callbackNotify(PVOID NotifyArg,   UINT   message,   WPARAM   wParam,   LPARAM   lParam)
{
	CScreen* psamp = (CScreen*)NotifyArg;
	switch(message)
	{
	case RGBWM_FRAMECAPTURED:
		{
			psamp->OnMyMessage_Sta1(wParam, lParam);//�������Ѿ��ɼ���һ֡ͼ��
			break;
		}
	case RGBWM_MODECHANGED:
		{
			psamp->OnMyMessage_modechange(wParam, lParam);//���ı�ɼ�ģʽ
			break;
		}
	case RGBWM_NOSIGNAL:
		{
			psamp->OnMyMessage_nosignal(wParam, lParam);//û�ź�
			break;
		}
	default:
		{
			break;
		}
	}
	//	::PostMessage(psamp->m_hWnd, message, wParam, lParam);	
}
//========================================================��Ļʶ����Ҫ����END==========================================================================================//



// CScreen �Ի���

IMPLEMENT_DYNAMIC(CScreen, CDialogEx)

CScreen::CScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreen::IDD, pParent)
	, m_print_xx(0)
	, m_print_yy(0)
{
	g_dlgScreen=this;
	g_num=NULL;//����10������0,1�ֲ�
	g_lpGray=NULL;//�����ֵ�����ͼ������
	g_num=new int[10*10*8];
	// TODO:  ��ʼ��
	signalflag = FALSE;
	//pData = new LPVOID[1600*1200*4];
	datamode = 2;
	bufferSize = 0L;         //��1���ɼ�����Ӧ�Ļ����С
	m_VGAthreshold=110;

	LoadMark(_T(".\\check\\qz_42.txt"));
	GetNumber();
	/*EnableToolTips(TRUE);   //trueΪ����ʾ���ܣ�false Ϊ�ر���ʾ����
	m_tooltip.Create(this);//����
	m_tooltip.SetTipTextColor(RGB(0,0,255));//�趨������ɫ
	m_tooltip.SetDelayTime(150);//�趨����ͣ��ʱ��
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_LOAD),"����ͼƬ�󣬽���ʵʱ��⡣");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_COMPARE),"һ����ͼƬ�������ʼʵʱ��⡣");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_STOP_VGA),"ֹͣ�ɼ�����Ҫ���³�ʼ��vga������ٴβɼ���");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_TEST),"���Ա༭���ڵ�ͼƬ�ļ����ɸ��Ĳ���ͼƬ���µ�����ԡ�");
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_BMP_PATH),"����ͼƬ·��");
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_VGA_THRASHOLD),"���ͼƬʱʹ�õ���ֵ");*/

	
}

CScreen dlgScreen;
CScreen *g_dlgScreen=NULL;

CScreen::~CScreen()
{
	delete []g_num;
	g_num=NULL;
}

void CScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_XX, m_print_xx);
	DDX_Text(pDX, IDC_EDT_YY, m_print_yy);
}


BEGIN_MESSAGE_MAP(CScreen, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_VGASNAP, &CScreen::OnBnClickedBtnVgasnap)
	ON_BN_CLICKED(IDC_BTN_VGAINIT, &CScreen::OnBnClickedBtnVgainit)
	ON_BN_CLICKED(IDC_BTN_VGASCR, &CScreen::OnBnClickedBtnVgascr)
	ON_BN_CLICKED(IDC_BTN_VGASTOP, &CScreen::OnBnClickedBtnVgastop)
	ON_BN_CLICKED(IDC_BTN_VGAPRT, &CScreen::OnBnClickedBtnVgaprt)
	ON_BN_CLICKED(IDC_BTN_LOCAL, &CScreen::OnBnClickedBtnLocal)
END_MESSAGE_MAP()
//========================================================��Ļʶ����Ҫ����START==========================================================================================//




//���浱ǰ��ʾ�������е�ͼ����Ϣ add wygwl 2010-7-16
BOOL CScreen::SaveImageToFile(CString sFile, int iW, int iH, BYTE * pInfo) 
{
	
#define WIDTHBYTES(bits) ((((bits) + 31) >> 5) << 2)
	int nBitCount = 32;
	int nLineBytes = WIDTHBYTES(iW * nBitCount);
	int dwDibSize = sizeof(BITMAPINFOHEADER) + nLineBytes*iH;//ͼ��ͷ��ͼ������

	CString sPathName;
	unsigned char * pDib= NULL;
	{
		sPathName  = sFile;
		pDib = new unsigned char[dwDibSize];
		if (pDib == NULL) 
			return 0;
		memset(pDib, 0, dwDibSize);//��pDibָ�����������dwDibSize�ֽ�
		unsigned char *pDibBits = pDib + sizeof(BITMAPINFOHEADER) ;
		BITMAPINFOHEADER*	pBIH = (BITMAPINFOHEADER*)pDib;
		pBIH->biSize		= sizeof(BITMAPINFOHEADER);
		pBIH->biWidth		= iW;
		pBIH->biHeight		= iH;
		pBIH->biPlanes		= 1;
		pBIH->biBitCount	= nBitCount;
		pBIH->biCompression	= BI_RGB;
		pBIH->biSizeImage	= nLineBytes*iH;
		pBIH->biClrUsed		= 0;
		
//		memcpy( pDibBits, , nLineBytes*iH);
		pInfo += nLineBytes*(iH-1);
		for ( int i = 0; i < iH; i++) // ���·���
		{
			memcpy( pDibBits + i*nLineBytes, pInfo-i*nLineBytes, nLineBytes);
		}
	}
	
	//save bmp
	{
		CFile file;
		CFileException e;
		if (file.Open(sPathName, CFile::modeCreate | CFile::modeWrite, &e))
		{
			BITMAPFILEHEADER bf;
			memset(&bf, 0, sizeof(bf));
			bf.bfType = 'MB';
			bf.bfSize = sizeof(bf) + dwDibSize;
			bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ;
			
			file.Write(&bf, sizeof(bf));
			file.Write(pDib, dwDibSize);//��pDibд�뵽�ļ��У�����ΪdwDibSize
			file.Close();
		}
		else
		{
			return FALSE;
		}
		delete [] pDib;
		pDib = NULL;
	}
	return TRUE;
}
// http://blog.sina.com.cn/s/blog_6ee382440100moeu.html
// http://apps.hi.baidu.com/share/detail/32878068
// bitmap λͼCBitmap����ָ��
// lpFileName Ϊλͼ�ļ���  
bool SaveBitmapToFile(CBitmap* bitmap, LPCWSTR lpFileName)
{            
	HBITMAP hBitmap;	// Ϊ�ղŵ���Ļλͼ���
	HDC hDC; //�豸������  
	int iBits; //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���  
	WORD wBitCount; //λͼ��ÿ��������ռ�ֽ���	
	DWORD dwPaletteSize = 0, //�����ɫ���С
		dwBmBitsSize,  //λͼ�������ֽڴ�С   
		dwDIBSize,	 //λͼ�ļ���С
		dwWritten;  //д���ļ��ֽ���
	BITMAP Bitmap; //λͼ���Խṹ  
	BITMAPFILEHEADER bmfHdr; //λͼ�ļ�ͷ�ṹ  
	BITMAPINFOHEADER bi; //λͼ��Ϣͷ�ṹ    
	LPBITMAPINFOHEADER lpbi; //ָ��λͼ��Ϣͷ�ṹ  
	HANDLE fh,	 //�����ļ�
		hDib,	 //�����ڴ���
		hPal,	//��ɫ����
		hOldPal = NULL;
	
	//����λͼ�ļ�ÿ��������ռ�ֽ���  
	hBitmap = (HBITMAP)*bitmap;  
	hDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL);  
	iBits =	GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);  
	DeleteDC(hDC);
	
	if (iBits <= 1)  
		wBitCount =	1;	
	else if	(iBits <= 4)  
		wBitCount =	4;	
	else if (iBits <= 8)  
		wBitCount =	8;	
	else if	(iBits <= 24)  
		wBitCount =	24;  
	else if (iBits <= 32)
		wBitCount = 32;
	
	//�����ɫ���С  
	if (wBitCount <= 8)  
		dwPaletteSize =	(1 << wBitCount) * sizeof (RGBQUAD);
	
	//����λͼ��Ϣͷ�ṹ  
	GetObject(hBitmap, sizeof (BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof (BITMAPINFOHEADER);  
	bi.biWidth = Bitmap.bmWidth;  
	bi.biHeight = Bitmap.bmHeight;  
	bi.biPlanes = 1;	
	bi.biBitCount =	wBitCount;	
	bi.biCompression = BI_RGB;  
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;  
	bi.biYPelsPerMeter = 0;  
	bi.biClrUsed = 0;  
	bi.biClrImportant = 0;  
	
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31) / 32) * 4 * Bitmap.bmHeight;  
	
	//Ϊλͼ���ݷ����ڴ�  
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof (BITMAPINFOHEADER));  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi =	bi;
	
	// �����ɫ�� 	   
	hPal = GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
		hDC = ::GetDC(NULL);	
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);	
		RealizePalette(hDC);
	}  
	
	//	 ��ȡ�õ�ɫ�����µ�����ֵ  
	GetDIBits(hDC, hBitmap,	0, (UINT) Bitmap.bmHeight,	
		(LPSTR)lpbi + sizeof (BITMAPINFOHEADER)	+ dwPaletteSize,
		(LPBITMAPINFO)lpbi,	DIB_RGB_COLORS);  
	
	//�ָ���ɫ��		
	if (hOldPal)	
	{  
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);  
		RealizePalette(hDC);  
		::ReleaseDC(NULL, hDC);
	}  
	
	//����λͼ�ļ�			
	fh = CreateFile(lpFileName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL |	FILE_FLAG_SEQUENTIAL_SCAN, NULL);  
	
	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;  
	
	//	 ����λͼ�ļ�ͷ  
	bmfHdr.bfType =	0x4D42; 	//	 "BM"  
	dwDIBSize = sizeof (BITMAPFILEHEADER)	  
		+ sizeof (BITMAPINFOHEADER)  
		+ dwPaletteSize	+ dwBmBitsSize;	   
	bmfHdr.bfSize =	dwDIBSize;	
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;  
	bmfHdr.bfOffBits = (DWORD)sizeof (BITMAPFILEHEADER)	  
		+ (DWORD)sizeof (BITMAPINFOHEADER)  
		+ dwPaletteSize;	
	
	//	 д��λͼ�ļ�ͷ  
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten,	NULL);	
	
	//	 д��λͼ�ļ���������  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);
	
	//���		  
	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  

	return TRUE;	
}

/******************************************************************************/
/*
*����SDK��������ϢRGBWM_FRAMECAPTURED
*��ϢRGBWM_FRAMECAPTURED:������ǰ�ɼ�������׼��������������һ֡ͼ�����ݵȴ�������
*/
void CScreen::OnMyMessage_Sta1(WPARAM wParam, LPARAM lParam)
{
	
	if ((NULL == hRGBCapture) || (0 == hRGBCapture))
	{
		return;
	}
// 	if (NULL == m_hCapEvent)
// 	{
// 		return;
// 	}

	RGBERROR error;
	if (!signalflag)
	{
		signalflag = TRUE;
		SetWindowText(_T(""));
	}
	pCaptureBuffer.bufferflags = TRUE;

//////////////////////////////////////////////////////////////////////////

	//if ( g_bSaveImage ) // ����ͼ�񵽴���
	//{
	//	g_bSaveImage = 0;
	//	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	//	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	//	//---memset(pData,0,1600*1200*4);
	//	TPicRegion pDest;
	//	pDest.pdata = (TARGB32*)pData;
	//	pDest.height = height;
	//	pDest.width  = width;
	//	pDest.byte_width = width << 2;
	//	if (1 < datamode)
	//	{
	//		DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	//	}
	//	else if (2 > datamode)
	//	{
	//		DECODE_UYVY_TO_RGB32((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	//	}

	//	int iW=width, iH=height;
	//	SaveImageToFile(g_strFile, iW, iH, (BYTE *)pData);
	//}
		

//////////////////////////////////////////////////////////////////////////


	//SetEvent(m_hPThreadEvents[0]);
	//Sleep(1);
	//emp_flag = (1==emp_flag) ? 0 : 1;	
	//while (pCaptureBuffer.bufferflags)
	//{
	//	//AfxMessageBox(_T("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BUFFER IS NOT EMPTY!!! PAINT NOT FINISHED!!!!"));
	//	Sleep(1);
	//}

	error = RGBCaptureFrameBufferReady (hRGBCapture,pCaptureBuffer.Index,bufferSize);
    if (error != RGBERROR_NO_ERROR)
	{
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ���������ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
				break;
			}

			case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ������û���ҵ��豸����\n"));//û���ҵ�����ָ��
				break;
			}

			case RGBERROR_BUFFER_TOO_SMALL:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ����������̫С\n"));//û���ҵ�����ָ��
				break;
			}

			default:
			{
				break;
			}
		}
		return;
//         char text[80];
//         wsprintf(text, "RGBCaptureFrameBufferReady returned: %s", RGBErrorToString(error));
//         ::MessageBox(m_hWnd, text, NULL, MB_OK | MB_ICONERROR);
		//AfxMessageBox("RGBCaptureFrameBufferReady returned: %s", RGBErrorToString(error) );
	}  
}
/*****************************************************************************/
/*
*�������ط����ı�ʱSDK��������ϢRGBWM_MODECHANGED
*�������òɼ�����
*/
void CScreen::OnMyMessage_modechange(WPARAM wParam, LPARAM lParam)
{
	unsigned long *pbufferSize = 0L;	
	RGBCAPTUREPARMS CaptureParms;
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1))  //���òɼ�����
	{
		AfxMessageBox(_T("SetCapture(),���òɼ�����ʧ��"));
		return;
	}
}
/*****************************************************************************/
/*
*���òɼ�����
*/
BOOL CScreen::SetCapture(HWND hWnd
							 , RGBCAPTUREPARMS  *pCaptureParms
							 , HRGBCAPTURE      *phRGBCapture
							 , RGBCAPTUREBUFFER pCaptureBuffer
							 , unsigned long    *pBufferSize
							 , RGBBITMAP       *pRGBBitmap)
{
	RGBERROR error;
	memset(pCaptureParms, 0, sizeof (*pCaptureParms));

	//////////////////////////////////////////////////////////////////////////
    /*��⵱ǰ�����VGA��Ƶ�źŷֱ���*/
	error = RGBCaptureDetectVideoMode(*phRGBCapture, 0, 0, &pCaptureParms->VideoTimings, FALSE);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureDetectVideoMode(), ��⵱ǰ�����VGA��Ƶ�źŷֱ��ʣ����ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
				break;
			}

			case RGBERROR_INVALID_POINTER:
			{
				AfxMessageBox(_T("RGBCaptureDetectVideoMode(), ��⵱ǰ�����VGA��Ƶ�źŷֱ��ʣ�û���ҵ�����ָ��\n"));//û���ҵ�����ָ��
				break;
			}

			default:
			{
				break;
			}
		}
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	/*���ô��ڱ���*/
	//char szWndTitle[120];
	//wsprintf(szWndTitle, "VGA�ɼ����� ");
    //::SetWindowText(::GetParent(this->m_hWnd), szWndTitle);

	//////////////////////////////////////////////////////////////////////////
    /* ����λͼ����Ϊ����ɼ�������׼�� */

    pRGBBitmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pRGBBitmap->bmiHeader.biPlanes = 1;
    pRGBBitmap->bmiHeader.biCompression = BI_BITFIELDS;
    pRGBBitmap->bmiHeader.biSizeImage = 0;
    pRGBBitmap->bmiHeader.biXPelsPerMeter = 3000;
    pRGBBitmap->bmiHeader.biYPelsPerMeter = 3000;
    pRGBBitmap->bmiHeader.biClrUsed = 0;
    pRGBBitmap->bmiHeader.biClrImportant = 0;
    pRGBBitmap->bmiHeader.biWidth = pCaptureParms->VideoTimings.HorAddrTime;
    pRGBBitmap->bmiHeader.biHeight = - (int)pCaptureParms->VideoTimings.VerAddrTime;
	
	//WaitForSingleObject(m_hDDrawEvent,INFINITE);
	//ResetEvent(m_hDDrawEvent);
	//if (DD_OK !=ddrawRGB.DDRelease())
	//{
	//	AfxMessageBox(_T("CRGBSAMPDlg::SetCapture DDRelease���ɹ�!!! \n"));
	//	//return FALSE;
	//}
	//if (DD_OK !=ddrawRGB.DDInit(hWnd,pCaptureParms->VideoTimings.HorAddrTime,pCaptureParms->VideoTimings.VerAddrTime))
	//{
	//	AfxMessageBox(_T("CRGBSAMPDlg::SetCapture DDInit��ͼ��ʼ�����ɹ�!!! \n"));
	//	//return FALSE;
	//}	
	//SetEvent(m_hDDrawEvent);

	//////////////////////////////////////////////////////////////////////////
	/*�˴���⻺���Ƿ�������ɹ�������ɼ��¼�*/
	error = RGBCaptureFrameBufferReady (hRGBCapture,
                                       pCaptureBuffer.Index,
                                       *pBufferSize);
	if (error != RGBERROR_NO_ERROR)
	{
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ���������ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
				break;
			}

			case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ������û���ҵ��豸����\n"));//û���ҵ�����ָ��
				break;
			}

			case RGBERROR_BUFFER_TOO_SMALL:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  ��⵱ǰVGA�ɼ��������Ƿ����������̫С\n"));//û���ҵ�����ָ��
				break;
			}

			default:
			{
				break;
			}
		}

// 		char text[80];
// 		wsprintf(text, "RGBCaptureFrameBufferReady returned: %s", RGBErrorToString(error));
// 		::MessageBox(NULL, text, NULL, MB_OK | MB_ICONERROR);
		//AfxMessageBox("RGBCaptureFrameBufferReady returned: %s", RGBErrorToString(error) );

		RGBCaptureClose(*phRGBCapture);
		*phRGBCapture = 0;

		if (!FreeBuffers(*phRGBCapture, hWnd, &pCaptureBuffer))
		{
			AfxMessageBox(_T("FreeBuffers(),�ͷŻ���ʧ�ܣ�"));
			return FALSE;
		}
		return FALSE;
	}
	return TRUE;
}
/*****************************************************************************/
/*
*����SDK�ɼ�������ȷ������ʱ��������ϢRGBWM_NOSIGNAL
*/
void CScreen::OnMyMessage_nosignal(WPARAM wParam, LPARAM lParam)
{
	CString strInfo;
	switch(wParam)
	{
		case NOSIGNAL_NO_RESOLUTION:
		{
			strInfo = _T("�� VGA �� ��(����ķֱ��� !!!)");
			break;
		}
		case NOSIGNAL_DDRCHECK_ERROR:
		{
			strInfo = _T("�� VGA �� ��(DDR �Լ���� !!!)");
			break;
		}
		case NOSIGNAL_H_OVERFLOW:
		{
			strInfo = _T("�� VGA �� ��(ˮƽƫ������� !!!)");
			break;
		}
		case NOSIGNAL_V_OVERFLOW:
		{
			strInfo = _T("�� VGA �� ��(��ֱƫ������� !!!)");
			break;
		}
		default:
		{
			break;
		}
	}
	signalflag = FALSE;
		
	//this->RedrawWindow();
	//this->m_static.ShowWindow(SW_SHOW);
	//this->m_static.SetWindowText(strInfo);
	//this->SetWindowText(strInfo);
}
/******************************************************************************/ 
/*
*�ͷŻ�����
*/
BOOL CScreen::FreeBuffers(HRGBCAPTURE hRGBCapture, HWND hWnd, RGBCAPTUREBUFFER *pCaptureBuffer)
{
	RGBERROR error;
	HDC hDC = ::GetDC(hWnd);

	if (bUsingAccBuffer1 && (hRGBCapture == hRGBCapture))
   {
	  bUsingAccBuffer1 = false;  //�޸ı�־�����Ƿ�ʹ�õĲ���Ϊfalse
   }


   ::ReleaseDC(hWnd, hDC);
   
	if(pCaptureBuffer->LpVoidBuffer)
	{
		//////////////////////////////////////////////////////////////////////////
		/*ɾ����ǰ��VGA�ɼ���������ʹ�ö����е�����*/
			error = RGBCaptureReleaseBuffer(hRGBCapture, pCaptureBuffer->Index);
			if (RGBERROR_NO_ERROR != error)
			{
				switch(error)
			{
				case RGBERROR_INVALID_HRGBCAPTURE:
				{
					AfxMessageBox(_T("RGBCaptureReleaseBuffer(),   ɾ����ǰ��VGA�ɼ���������ʹ�ö����е�������ע��ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
					break;
				}
				case RGBERROR_INVALID_POINTER:
				{
					AfxMessageBox(_T("RGBCaptureReleaseBuffer(),    ɾ����ǰ��VGA�ɼ���������ʹ�ö����е�������û���ҵ�ָ��ǰ����洢����ָ��\n"));//û���ҵ�ָ��ǰ����洢����ָ��
					break;
				}
				default:
				{
					break;
				}
			}
			return FALSE;	
			}
		//////////////////////////////////////////////////////////////////////////
		/*�ͷŻ�����*/
			GlobalFree(pCaptureBuffer->LpVoidBuffer); 
			pCaptureBuffer->LpVoidBuffer = NULL;
	}
	
	//delete pCaptureBuffer;
	return TRUE;
}
/*****************************************************************************/ 
/*
 *��������λ��
 */
int CScreen::CountBits(unsigned long  ulValue, int start, int end)
{
    int count = 0, i;
    unsigned long ulMask;

    for (i = start, ulMask = 1 << start; i <= end; i++, ulMask <<= 1)
	{
		if (ulMask & ulValue)
		{
			count++;
		}
	}
	return count;
}
/*****************************************************************************/ 
/*
*��⵱ǰ����ģʽ
*/
unsigned short CScreen::DetectPixelFormat( HWND hWnd, DWORD *pColourMask )
/*
 * Summary: Detects the pixel format of the graphics card frame buffer.
 *
 * Purpose: 
 *          pColourMask points to an array of three DWORDs. Each DWORD is
 *             loaded with a bit mask representing the bits for red, green
 *             and blue respectively.
 *
 * Return:  One of the following values:
 *             RGBPIXELFORMAT_555,
 *             RGBPIXELFORMAT_565,
 *             RGBPIXELFORMAT_888,
 *             RGBPIXELFORMAT_UNKNOWN.
 */
{
    HDC            hDC, hDCCompatible;
    HBITMAP        hBitmap, hBitmapOld;
    DWORD          dwPixel, dwMask;
    int            nBytes, end, red, i, green, blue;
    unsigned short format;

    hDC = ::GetDC(hWnd);
    hDCCompatible = CreateCompatibleDC(hDC);

    hBitmap = CreateCompatibleBitmap(hDC, 1, 1);
    hBitmapOld = (HBITMAP)SelectObject(hDCCompatible, hBitmap);

    SetPixel(hDCCompatible, 0, 0, RGB (255, 0, 0));
    nBytes = GetBitmapBits(hBitmap, sizeof (dwPixel), &dwPixel);
    end = (nBytes << 3) - 1;
    red = CountBits(dwPixel, 0, end);

    dwMask = 0;
    for (i = 0; i < nBytes; i++)
	{
		dwMask |= (0xff << ( i << 3 ));
	}
    pColourMask[0] = dwPixel & dwMask;

    SetPixel(hDCCompatible, 0, 0, RGB (0, 255, 0));
    GetBitmapBits(hBitmap, sizeof(dwPixel ), &dwPixel);
    green = CountBits(dwPixel, 0, end);
    pColourMask[1] = dwPixel & dwMask;

    SetPixel(hDCCompatible, 0, 0, RGB (0, 0, 255));
    GetBitmapBits(hBitmap, sizeof (dwPixel), &dwPixel);
    blue = CountBits(dwPixel, 0, end);
    pColourMask[2] = dwPixel & dwMask;

    SelectObject(hDCCompatible, hBitmapOld);
    DeleteDC(hDCCompatible);
    DeleteObject(hBitmap);

    ::ReleaseDC(hWnd, hDC);

    if ((green == 5) && (red == 5) && (blue == 5))
	{
        format = RGBPIXELFORMAT_555;
	}
    else if ((green == 6) && (red == 5) && (blue == 5))
	{
        format = RGBPIXELFORMAT_565;
	}
    else if ((green == 8) && (red == 8) && (blue == 8))
	{
        format = RGBPIXELFORMAT_888;
	}
    else
	{
        format = RGBPIXELFORMAT_UNKNOWN;
	}
    return format;
}

/*****************************************************************************/
/*
 *����ָ����С���仺��
 */
BOOL CScreen::AllocateBuffers(HRGBCAPTURE hRGBCapture, 
                                      RGBCAPTUREBUFFER *pCaptureBuffer, 
									  HWND hWnd, 
									  UINT BufferSize)
{
	//////////////////////////////////////////////////////////////////////////
   /*���仺��*/
   pCaptureBuffer->LpVoidBuffer = GlobalAlloc(GMEM_FIXED, BufferSize);
   if (pCaptureBuffer->LpVoidBuffer == NULL)
   {
	   return FALSE;
   }
	AfxMessageBox(_T("%d"),GlobalSize(pCaptureBuffer->LpVoidBuffer));
   //////////////////////////////////////////////////////////////////////////
   /*ע�ᵱǰ��VGA�ɼ��¼�*/
   RGBERROR error;
   error = RGBCaptureUseMemoryBuffer(hRGBCapture, pCaptureBuffer->LpVoidBuffer, BufferSize, &pCaptureBuffer->Index);
   if (RGBERROR_NO_ERROR != error)
   {
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  ע�ᵱǰ��VGA�ɼ��¼���ע��ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����\n"));//���ʧ�ܣ��򿪵��豸������û���ҵ���Ӳ���豸�����
				break;
			}

			case RGBERROR_INVALID_POINTER:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  ע�ᵱǰ��VGA�ɼ��¼���û���ҵ�ָ��ǰ����洢����ָ��\n"));//û���ҵ�ָ��ǰ����洢����ָ��
				break;
			}

			case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  ע�ᵱǰ��VGA�ɼ��¼���û���ҵ���Ӧ��ǰ����洢��������\n"));//û���ҵ���Ӧ��ǰ����洢��������
				break;
			}

			default:
			{
				break; 
			}
		}
		return FALSE;
   }
   if ((bUsingAccBuffer1 == FALSE) && (hRGBCapture == hRGBCapture))
   {
	  bUsingAccBuffer1 = true;  //�޸ı�־����ʹ�õĲ���ΪTRUE
   }
   
   return TRUE;
}
//========================================================��Ļʶ����Ҫ����END============================================================================================//

void CScreen::LoadMark(CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	//file_temp="check\\qz_42.txt";   //�����ļ���������ļ���
	CString fileName(file_temp);//CString a (_T ("aaa"));

	if(!File2.Open(fileName,CFile::modeRead))
	{
		AfxMessageBox(_T("����ƥ���ļ�δ�ҵ�"));
		return;
	}

	for(int j=0;j<10;j++)
	{
		File2.ReadString(FileData2);
		FileData2.MakeReverse();
		int tmp=_wtoi(FileData2);
		for(int k=0;k<8;k++)
		{
			head[j][k]=tmp%10;
			tmp/=10;
		}
	}
	File2.Close();
}
void CScreen::FindOrigin(BYTE* lpGray)
{
	bool first=false;
	char tmep_num=0;
	height_num=0;
	width_num=0;
	CString strTmp;
	for(int i=0;i<g_height-11;i++)
	{	
		for(int j=0;j<g_width-9;j++)
		{
			for(int x=0;x<10;x++)
				for(int y=0;y<8;y++)
				{
					if(lpGray[(i+x)*g_width+(j+y)]==head[x][y]) tmep_num++; 
				}
				if(tmep_num>75)//�ж�ͼƬ���
				{
					first=true;
					height_num=i;
					width_num=j;
					strTmp.Format(_T("����У���ֵ��%d��%d "),i,j);
					//AfxMessageBox(strTmp);
					break;
				}
				else tmep_num=0;

		}
		if(first) break;
	}

}
void CScreen::FindOriginEX(BYTE* lpGray)
{
	bool first=false;
	char w_num=0,b_num=0;
	height_num=0;
	width_num=0;
	CString strTmp;
	for(int i=0;i<g_height-11;i++)
	{	
		for(int j=0;j<g_width-9;j++)
		{
			for(int x=0;x<10;x++)
				for(int y=0;y<8;y++)
				{
					if(0==lpGray[(i+x)*g_width+(j+y)]&&1==head[x][y]) w_num++; 
					if(1==lpGray[(i+x)*g_width+(j+y)]&&0==head[x][y]) 
						if(y<7)
							if(1==head[x][y+1])
								;
							else
								b_num++; 
						else
							b_num++;
				}
				if(0==w_num&&b_num<3)//�ж�ͼƬ���
				{
					first=true;
					height_num=i;
					width_num=j;
					strTmp.Format(_T("����У���ֵ��%d��%d"),i,j);
					//AfxMessageBox(strTmp);
					break;
				}
				else 
				{
					w_num=0;
					b_num=0;
				}

		}
		if(first) break;
	}

}
//��ͼƬ��ɨ��Ƕ�
void CScreen::GetNumber()
{

	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2,file_temp;// ����һ��CString����Ϊһ��������
	for(int p=0;p<10;p++)
	{
		file_temp.Format(_T(".\\check\\num_%d.txt"),p); 
		if(!File2.Open(file_temp,CFile::modeRead))
		{
			AfxMessageBox(_T("û���ҵ�����ƥ���ļ�"));
			return;
		}
		for(int j=0;j<10;j++)
		{
			File2.ReadString(FileData2);
			FileData2.MakeReverse();
			int tmp=_wtoi(FileData2);
			for(int k=0;k<8;k++)
			{
				g_num[p*80+j*8+k]=tmp%10;
				tmp/=10;
			}
		}
		File2.Close();
	}
}

void CScreen::ScanDegree(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	int nocheckout=0;
	char tmep_num=0;
	for(int m=0;m<30;m++,t++)//�ܹ�30����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]==g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(tmep_num>75)
						{
							degree[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9&&(t==0||t==6||t==12||t==18||t==24))
						{
							nocheckout++;
							degree[t]=0;
							break;
						}
						else if(p==9)
						{
							degree[0]=-1;
						}
				}
			}
		}

	}//return degree[30];
	if(nocheckout>5 ) //δ�ҵ�ƥ������
	{
		degree[0]=-1;
		//break;
	}
}
void CScreen::ScanDegreeEX(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	int nocheckout=0;
	char tmep_num=0;
	for(int m=0;m<30;m++,t++)//�ܹ�30����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(0==lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]&&1==g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(0==tmep_num)
						{
							degree[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9&&(t==0||t==6||t==12||t==18||t==24))
						{
							nocheckout++;
							degree[t]=0;
							break;
						}
						else if(p==9)
						{
							degree[0]=-1;
						}
				}
			}
		}

	}//return degree[30];
	if(nocheckout>5 ) //δ�ҵ�ƥ������
	{
		degree[0]=-1;
		//break;
	}
}
void CScreen::ScanDegreeDynamic(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	char tmep_num=0;
	for(int m=0;m<6;m++,t++)//�ܹ�6����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]==g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(tmep_num>75)
						{
							degree[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9)
						{
							degree[t]=0;
							break;
						}
				}
			}
		}
		//if(degree[0]==-1) break;
	}//return degree[30];
}
bool CScreen::ScanDegreeEFGM(BYTE * lpGray,CString file_temp)
{
	while(1)
	{
		DWORD width = RGBBitmap1.bmiHeader.biWidth;
		DWORD height = -RGBBitmap1.bmiHeader.biHeight;

		pDest.pdata = new TARGB32[height*width];//b,g,r,a  4��8λ���һ������
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("PIC\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

		//SaveImageToFile(_T("PIC\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
		DECODE_THRESHOLD(pDest,pDest,lpGray,m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

		//SaveImageToFile(_T("PIC\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(_T("check/qz_42.txt"));

		FindOrigin(lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 

			//AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
		}
		else
			break;
	}
	//g_num=new int[10*10*8];
	//GetNumber();

	//////////////////////////////////////////////////
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	int M[5]={0};
	int t=0;
	char tmep_num=0;
	for(int m=0;m<5;m++,t++)//�ܹ�5����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]!=g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(tmep_num>=75)
						{
							M[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9)
						{
							M[t]=0;
							break;
						}
				}
			}
		}
		//if(degree[0]==-1) break;
	}//return degree[30];
	//delete []g_num;
	delete []lpGray;
	lpGray=NULL;
	//g_num=NULL;
	if(0!=M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4])//�ж�Mֵ
		return 1;
	else
		return 0;
}
bool CScreen::ScanDegreeEFGMEX(BYTE * lpGray,CString file_temp)
{
	while(1)
	{
		DWORD width = RGBBitmap1.bmiHeader.biWidth;
		DWORD height = -RGBBitmap1.bmiHeader.biHeight;

		pDest.pdata = new TARGB32[height*width];//b,g,r,a  4��8λ���һ������
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("PIC\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

		//SaveImageToFile(_T("PIC\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
		DECODE_THRESHOLD(pDest,pDest,lpGray,m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

		//SaveImageToFile(_T("PIC\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(_T("check/qz_42.txt"));

		FindOrigin(lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 

			//AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
		}
		else
			break;
	}
	g_num=new int[10*10*8];
	GetNumber();

	//////////////////////////////////////////////////
	CStdioFile File2;// ����һ��CStdioFile�����File
	CString FileData2;//,file_temp;// ����һ��CString����Ϊһ��������
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	int M[5]={0};
	int t=0;
	char tmep_num=0;
	for(int m=0;m<5;m++,t++)//�ܹ�5����
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10��8��
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10����
				{
					for(int k=0;k<10;k++)//10��
						for(int l=0;l<8;l++)//8��
						{
							if(1==lpGray[(i+k+(height_num-2))*g_width+(j+l+(width_num-46))]&&1==g_num[p*80+k*8+l]) 
								tmep_num++; 
						}
						if(0==tmep_num)
						{
							M[t]=p;
							break;
						}
						else tmep_num=0;
						if(p==9)
						{
							M[t]=0;
							break;
						}
				}
			}
		}
		//if(degree[0]==-1) break;
	}//return degree[30];
	delete []g_num;
	delete []lpGray;
	lpGray=NULL;
	g_num=NULL;
	if(14742==M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4] || 14993==M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4])//�ж�Mֵ
		return 1;
	else
		return 0;
}

// CScreen ��Ϣ�������


void CScreen::OnBnClickedBtnVgasnap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BTN_VGASCR)->EnableWindow(1);
	GetDlgItem(IDC_BTN_VGASNAP)->EnableWindow(0);
	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	pDest.pdata = new TARGB32[width*height];//b,g,r,a  4��8λ���һ������
	pDest.height = height;
	pDest.width  = width;
	pDest.byte_width = width << 2;

	g_height=height;
	g_width=width;
	g_lpGray=new BYTE[g_height*g_width];

	int iW=width, iH=height;
	DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	SaveImageToFile(gstuPathInf.csPathExe + _T("\\PIC\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

	DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

	SaveImageToFile(gstuPathInf.csPathExe + _T("\\PIC\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
	DECODE_THRESHOLD(pDest,pDest,g_lpGray,m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������
	DECODE_THRESHOLD2(pDest,pDest,m_VGAthreshold);//���ضԻ�
	SaveImageToFile(gstuPathInf.csPathExe + _T("\\PIC\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��


	delete [] pDest.pdata;
	pDest.pdata=NULL;
	///////////////////////

	CString m_bmp_file=gstuPathInf.csPathExe +_T("\\PIC\\yuanshi.bmp");
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(m_bmp_file);  
	if( !ifFind )  
		return;
	CBitmap m_bmp;//�������Ա
	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	m_bmp.Attach(hBitmap1);//���������cbitmap����
	m_bmp.GetBitmap(&bm);
	CWnd * pWnd = GetDlgItem(IDC_SCREEN_VGA);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_SCREEN_VGA)->GetClientRect(&rect);
	CDC memDC;        //����һ���豸
	CClientDC dc1(this);      //��ȡ�ͻ�
	memDC.CreateCompatibleDC( &dc1 );
	memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
	pDC->SetStretchBltMode (HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 1000, 500, SRCCOPY);
	memDC.DeleteDC();	
	m_bmp.DeleteObject();
	DeleteObject(hBitmap1);//�ǵ�ɾ��	
	pWnd->ReleaseDC(pDC);

	////////////////////////////////////////////////////////
	GetDlgItem(IDC_BTN_VGASCR)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGASNAP)->EnableWindow(1);
	FindOrigin(g_lpGray);
	if(height_num>100 || width_num>100|| (height_num==0&&width_num==0))
	{ 
		AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
		delete []g_lpGray;
		g_lpGray=NULL;
//		GetDlgItem(IDC_COMPARE)->EnableWindow(0);
		return;
	}
	//g_num=new int[10*10*8];
	//GetNumber();
	ScanDegree(g_lpGray,gstuPathInf.csPathExe + _T("\\check\\position.txt"));//���degree[]
	CString _degree;
	_degree.Format(_T("����Ƕȣ�%d\r\n����Ƕȣ�%d\r\n��Ч�Ƕȣ�%d\r\nԭʼ���᣺%d\r\nԭʼ���᣺%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
		degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
		degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
		degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
		degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);
	GetDlgItem(IDC_EDT_VGARESULT)->SetWindowText(_degree);
	//delete []g_num;
	delete []g_lpGray;
	g_lpGray=NULL;
	//g_num=NULL;
}


void CScreen::OnBnClickedBtnVgainit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BTN_VGASTOP)->EnableWindow(1);
	GetDlgItem(IDC_BTN_VGAINIT)->EnableWindow(0);
	// TODO: VGA��ʼ��
	/////////////
	//GetDlgItem(IDC_VGA_INIT)->EnableWindow(0);
	/////////////
	RGBERROR error;
	m_iIndex=0;
	hRGBCapture = 0;
	BufferSize=1024*768*3;
	RGBCAPTUREPARMS CaptureParms;  //��ŵ�ǰ�ɼ����Ĳ���
	error = RGBCaptureInitialise(&m_ulNumofcard);//��ʼ��
	if (RGBERROR_NO_ERROR != error)
	{
		//��ʼ��ʧ�ܣ�����ֵ��Ϊ��RGBERROR_HARDWARE_NOT_FOUND
		if (RGBERROR_HARDWARE_NOT_FOUND == error)
		{
			//û���ҵ�Ӳ���豸,��ӡ��־
			AfxMessageBox(_T("RGBCaptureInitialise() ��ʼ����û���ҵ�Ӳ���豸\n"));
			//break;
			//return(FALSE);
		}
		return;
	}
	//�򿪲ɼ���
	error = RGBCaptureOpen(m_iIndex,&hRGBCapture);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
		case RGBERROR_HARDWARE_NOT_FOUND :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�Ӳ���豸\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_INVALID_POINTER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�ָ���Ӳ���豸��ָ��\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_INVALID_INDEX :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�Ӳ���豸������\n"));//û���ҵ��豸
				break;
			}
		case RGBERROR_DEVICE_IN_USE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��Ӳ���豸���ڱ�ʹ��\n"));//Ӳ���豸���ڱ�ʹ��
				break;
			}
		case RGBERROR_UNABLE_TO_LOAD_DRIVER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸�����ܼ�������\n"));//��Ӳ��������
				break;
			}
		case RGBERROR_INVALID_DEVICE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��Ӳ��������\n"));//��Ӳ��������
				break;
			}
		default:
			{
				break;
			}
		}
		return;
	}
	pCaptureParms.Input=0;//VGA����
	pCaptureParms.datamode=2;//0,uyvy,2,RGB��ʽ
	pCaptureParms.BuffersNumber=0;// ��֡��
	pCaptureParms.graytest=0;
	pCaptureParms.FramespSec=50;// 50֡
	pCaptureParms.Size=sizeof(pCaptureParms);// �ṹ����
	pCaptureParms.Flags= //RGBCAPTURE_PARM_PIXELFORMAT
		RGBCAPTURE_PARM_NOTIFY_ARG
		| RGBCAPTURE_PARM_NOTIFY_FN
		// | RGBCAPTURE_PARM_HWND
		| RGBCAPTURE_PARM_DATAMODE
		| RGBCAPTURE_PARM_GRAYTEST
		| RGBCAPTURE_PARM_FRAMESPSEC
		| RGBCAPTURE_PARM_INPUT
		| RGBCAPTURE_PARM_BUFFERSNUM
		| RGBCAPTURE_PARM_NOTIFY_FLAGS
		| RGBCAPTURE_PARM_VDIF_DESCRIPTION;    //���õĲ����У���ǰ����ģʽ�������ھ����֪ͨ��Ϣģʽ
	//pixelFormat = DetectPixelFormat(m_hWnd,(DWORD *)&pRGBBitmap->colorMark);
	//pCaptureParms.PixelFormat = pixelFormat;
	//pCaptureParms.PixelFormat=RGBPIXELFORMAT_888;
	pCaptureParms.NotifyArg=this;
	pCaptureParms.NotifyFn=callbackNotify;
	pCaptureParms.NotifyFlags = RGBNOTIFY_NO_SIGNAL | RGBNOTIFY_MODE_CHANGE ;
	//pCaptureParms.HWnd=m_hWnd;
	//���ò���
	error = RGBCaptureSetParameters(hRGBCapture, &pCaptureParms,RGBCAPTURE_FLAG_CURRENT|RGBCAPTURE_FLAG_REAL_WORLD);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ���������û���ҵ�Ӳ���豸\n"));//û���ҵ�Ӳ���豸
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ���������û���ҵ�����ָ��\n"));//û���ҵ�����ָ��
				break;
			}

		case RGBERROR_INVALID_SIZE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�������������ָ���С����\n"));//����ָ���С����
				break;
			}

		case RGBERROR_INVALID_FLAGS:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵı�־λ����\n"));//��֧�ֵı�־λ����
				break;
			}

		case RGBERROR_INVALID_FORMAT:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ������źŸ�ʽ\n"));//��֧�ֵ������źŸ�ʽ
				break;
			}

		case RGBERROR_INVALID_BLACKLEVEL:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵĺ�ɫ��\n"));//��֧�ֵĺ�ɫ��
				break;
			}

		case RGBERROR_INVALID_PIXEL_FORMAT:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ�����ģʽ\n"));//��֧�ֵ�����ģʽ
				break;
			}

		case RGBERROR_INVALID_HWND:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ������������ǵ�ǰ���ھ��\n"));//���ǵ�ǰ���ھ��
				break;
			}

		case RGBERROR_INVALID_SYNCEDGE:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ�ͬ����\n"));//��֧�ֵ�ͬ����
				break;
			}

		case RGBERROR_HSCALED_OUT_OF_RANGE:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ����ű���\n"));//��֧�ֵ����ű���
				break;
			}

		default:
			{
				break;
			}
		}
		return;
	}
	/*��ʼ�ɼ��¼�*/
	error = RGBCaptureEnable(hRGBCapture, TRUE);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ������򿪶�����û���ҵ���Ӳ���豸���\n"));//�򿪶�����û���ҵ���Ӳ���豸���
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ�����Ӳ���豸û�д�\n"));//Ӳ���豸û�д�
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ����������߳�ʧ��\n"));//�����߳�ʧ��
				break;
			}

		default:
			{
				break;
			}
		}
		return;
	}
	bufferSize =1600 * 1200 * PixelFormatToBytesPerPixel[RGBPIXELFORMAT_888];
	if (!AllocateBuffers(hRGBCapture, &pCaptureBuffer, m_hWnd, bufferSize))
	{
		return ;
	} 
	//////////////////////////////////////////////////////////////////////////
	/*���òɼ�����*/
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1)) 
	{
		AfxMessageBox(_T("SetCapture(),���òɼ�����ʧ��"));
		return;
	}
	//��ʼ�����
}


void CScreen::OnBnClickedBtnVgascr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//LoadMark(gstuPathInf.csPathExe + _T("\\check\\qz_42.txt"));
	/*
	�ҵ�ͼƬ�ı�����
	*/
	GetDlgItem(IDC_BTN_VGASCR)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGASNAP)->EnableWindow(1);
	FindOrigin(g_lpGray);
	if(height_num>100 || width_num>100|| (height_num==0&&width_num==0))
	{ 
		AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
		delete []g_lpGray;
		g_lpGray=NULL;
//		GetDlgItem(IDC_COMPARE)->EnableWindow(0);
		return;
	}
	//g_num=new int[10*10*8];
	//GetNumber();
	ScanDegree(g_lpGray,gstuPathInf.csPathExe + _T("\\check\\position.txt"));//���degree[]
	CString _degree;
	_degree.Format(_T("����Ƕȣ�%d\r\n����Ƕȣ�%d\r\n��Ч�Ƕȣ�%d\r\nԭʼ���᣺%d\r\nԭʼ���᣺%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
		degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
		degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
		degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
		degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);
	GetDlgItem(IDC_EDT_VGARESULT)->SetWindowText(_degree);
	//delete []g_num;
	delete []g_lpGray;
	g_lpGray=NULL;
	//g_num=NULL;
}


void CScreen::OnBnClickedBtnVgastop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BTN_VGASTOP)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGAINIT)->EnableWindow(1);
	//stopVGA(m_hWnd, &hRGBCapture1, pCaptureBuffer1);
	RGBERROR error;	
	error = RGBCaptureEnable(hRGBCapture, FALSE);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ������򿪶�����û���ҵ���Ӳ���豸���\n"));//�򿪶�����û���ҵ���Ӳ���豸���
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ�����Ӳ���豸û�д�\n"));//Ӳ���豸û�д�
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ����������߳�ʧ��\n"));//�����߳�ʧ��
				break;
			}

		default:
			{
				break;
			}
		}
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	/*�ͷŻ���*/
	if (NULL != &pCaptureBuffer)
	{
		if (!FreeBuffers(hRGBCapture, m_hWnd, &pCaptureBuffer))
		{
			AfxMessageBox(_T("FreeBuffers(),�ͷŻ���ʧ�ܣ�"));
			return;
		}
	}
	/*�رղɼ���*/
	RGBCaptureClose(hRGBCapture);
	hRGBCapture = 0;
}


void CScreen::OnBnClickedBtnVgaprt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CPrintDialog dlg(FALSE,PD_PAGENUMS);     //�����ӡ��
	//dlg.GetDefaults();
	
	CDC dc;
	if (dlg.DoModal() == IDCANCEL)             //��ȡ�û��Դ�ӡ��������
		return;
	HDC hdcPrinter = dlg.GetPrinterDC();       //��ô�ӡ���ľ��	

	if (hdcPrinter == NULL)
	{
		MessageBox(_T("Buy a printer!"));
	}
	else
	{
		//GetDlgItem(IDC_PRINT)->EnableWindow(0);
		CDC dcPrinter;                            //����һ���豸
		dcPrinter.Attach(hdcPrinter);             //�Ѵ�ӡ����������豸
		dcPrinter.m_bPrinting = TRUE;
		DOCINFO docinfo;                          //��ӡ������
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("SC-EFG");
		CPrintInfo info;
		info.m_rectDraw.SetRect(0,0, 
			dcPrinter.GetDeviceCaps(HORZRES), 
			dcPrinter.GetDeviceCaps(VERTRES));//���÷�Χ
		if (dcPrinter.StartDoc(&docinfo) < 0)
		{
			MessageBox(_T("Printer wouldn't initalize"));
		}
		else
		{
			// start a page
			if (dcPrinter.StartPage() < 0)
			{
				MessageBox(_T("Could not start page"));
				dcPrinter.AbortDoc();
			}
			else
			{
				SetTextColor(dcPrinter,RGB(0, 255, 0));		  //���ô�ӡ����������ɫ����ʵ���������������			
				DrawBmp(&dcPrinter,m_print_xx,m_print_yy);           //���ô�ӡͼƬ��������׼������Ӧ���ǻ���һ��Ҫ��ӡ��ͼƬ��
				CString str; //��ȡϵͳʱ��
				CTime tm;
				tm=CTime::GetCurrentTime();
				str=tm.Format("%Y-%m-%d %X");
				dcPrinter.TextOut(10, 10, str, str.GetLength());
				dcPrinter.EndPage();            //������ҳ
				dcPrinter.EndDoc();             //����      
			}
		}
		dcPrinter.Detach();             //�ͷŶ���
		dcPrinter.DeleteDC();           //�ͷ��豸
	}
	//DeleteObject(hdcPrinter);
}

void CScreen::DrawBmp(CDC*pDC,double xx,double yy)
{

	SIZE s1;
	s1.cx = pDC->GetDeviceCaps(HORZRES) ;
	s1.cy = pDC->GetDeviceCaps(VERTRES) ;
	pDC->SetMapMode(MM_ANISOTROPIC); //ת������ӳ��ģʽ
	int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX);  //�õ��豸ÿ�߼�Ӣ�����������
	int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY);
	float fWidth = (float)xLogPixPerInch / 96 ; //�õ�������Ļӳ�䵽�Ӵ���С����
	float fHeight = (float)yLogPixPerInch / 96; //һ��õ���fWidth = fHeight 
	CDC memDC;        //����һ���豸
	CClientDC dc(this);      //��ȡ�ͻ�
	memDC.CreateCompatibleDC( &dc );
	CBitmap m_bmp;//�������Ա
	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	//CString bmp_path =_T(".\\PIC\\screen.bmp");
	//HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	CString bmp_path = gstuPathInf.csPathExe +_T("\\PIC\\erzhihu.bmp"); 
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	//m_bmp.LoadBitmap(bmp_path);
	if(hBitmap1!=NULL)
	{
		m_bmp.Attach(hBitmap1);//���������cbitmap����
		m_bmp.GetBitmap(&bm);//
		CSize size = CSize(bm.bmWidth, bm.bmHeight);
		pDC->SetWindowExt(size); //�����Ӵ���С
		long xExt = (long)(fWidth * size.cx); //�õ��Ӵ���С
		long yExt = (long)(fHeight * size.cy) ;
		if(xExt>s1.cx)
		{

			yExt=yExt*s1.cx/xExt;
			xExt=s1.cx;
		}
		//pDC->SetViewportExt((int)xExt, (int)yExt); //�����Ӵ���С(�ɵ�����Ļӳ�䵽�Ӵ���С)
		pDC->SetViewportExt((int)size.cx, (int)size.cy); //�����Ӵ���С(�ɵ�����Ļӳ�䵽�Ӵ���С)
		memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����
		//pDC->BitBlt(40, 40,(int)xExt, (int)yExt, &memDC, 0, 0, SRCCOPY);             //��Դ��copy����ʽװ�ص�pDC�豸��
		pDC->StretchBlt(10, 40,(int)(xExt*xx), (int)(yExt*yy), &memDC, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
		m_bmp.DeleteObject();
	}
	DeleteObject(hBitmap1);//�ǵ�ɾ��	
	memDC.DeleteDC();	
}



void CScreen::OnBnClickedBtnLocal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBitmap m_bmp;//�������Ա
	
	BITMAP bm;//���λͼ��Ϣ�Ľṹ
	CString bmp_path =gstuPathInf.csPathExe + _T("\\PIC\\screen.bmp");//_T("d://tmep1.bmp")
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
	//m_bmp.LoadBitmap(bmp_path);
	if(NULL!=hBitmap1)
	{
		m_bmp.Attach(hBitmap1);//���������cbitmap����
		m_bmp.GetBitmap(&bm);//
		g_height = bm.bmHeight;//ͼ��Ŀ��    
		g_width = bm.bmWidth;
		g_widthBytes=bm.bmWidthBytes;//ͼ��һ��������ռ���ֽ���
		int size=g_height*g_widthBytes; 
		BYTE *lpBits=new BYTE[size]; 
		m_bmp.GetBitmapBits(size,lpBits);//�õ�RGB����
		BYTE *lpGray=new BYTE[g_height*g_width];
		BYTE R, G, B;
		for(int i=0;i<g_height;i++)
			for(int j=0;j<g_width;j++)
			{
			  B=lpBits[i*g_widthBytes+j*4];
			  G=lpBits[i*g_widthBytes+j*4+1];
			  R=lpBits[i*g_widthBytes+j*4+2];
			  lpGray[i*g_width+j]=BYTE(R*0.3+G*0.59+B*0.11);///(B+G+R)/3;//ת�Ҷ�
		 
			}
		for(int i=0;i<g_height;i++)
			for(int j=0;j<g_width;j++)
			{
				if(lpGray[i*g_width+j]< 128 )//��ֵ��
				{
					lpGray[i*g_width+j]=0;
					/*lpGray[i*widthBytes+j*4+1]=1;
					lpGray[i*widthBytes+j*4+2]=1;*/
				}
				else
				{
					lpGray[i*g_width+j]=1;
					/*lpGray[i*widthBytes+j*4+1]=0;
					lpGray[i*widthBytes+j*4+2]=0;*/
				}
			}

			//LoadMark(gstuPathInf.csPathExe + _T("\\check\\qz_42.txt"));
		/*
		�ҵ�ͼƬ�ı�����
		*/
		FindOrigin(lpGray);
		if(height_num>100 || width_num>100|| (height_num==0&&width_num==0))
		{ 
			AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
			delete []g_lpGray;
			g_lpGray=NULL;
	//		GetDlgItem(IDC_COMPARE)->EnableWindow(0);
			return;
		}


		//g_num=new int[10*10*8];
		//GetNumber();
		ScanDegree(lpGray,gstuPathInf.csPathExe + _T("\\check\\position.txt"));//���degree[]
		CString _degree;
		_degree.Format(_T("����Ƕȣ�%d\r\n����Ƕȣ�%d\r\n��Ч�Ƕȣ�%d\r\nԭʼ���᣺%d\r\nԭʼ���᣺%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
			degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
			degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
			degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
			degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);
		GetDlgItem(IDC_EDT_VGARESULT)->SetWindowText(_degree);
		//UpdateData(0);
	
		delete []lpBits;
		delete []lpGray;
		//delete []g_num;
		//g_num=NULL;
		lpBits=NULL;
		lpGray=NULL;
	}
	DeleteObject(hBitmap1);//�ǵ�ɾ��	
}

//����1���ɹ�
bool CScreen::RCGVGA()
{
	int rechecknum=0;//�ز����
	bool bSuc = 0;//����ֵ
	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	pDest.pdata = new TARGB32[height*width];//b,g,r,a  4��8λ���һ������
	pDest.height = height;
	pDest.width  = width;
	pDest.byte_width = width << 2;

	g_height=height;
	g_width=width;
	g_lpGray=new BYTE[g_height*g_width];

	int iW=width, iH=height;
	while(1)
	{

		
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("PIC\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

		//SaveImageToFile(_T("PIC\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
		DECODE_THRESHOLD(pDest,pDest,g_lpGray,m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

		//SaveImageToFile(_T("PIC\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
		
		//LoadMark(gstuPathInf.csPathExe + _T("\\check\\qz_42.txt"));

		FindOrigin(g_lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 
			if(rechecknum>20)
			{
//				AlertCtrl(1);
				//AfxMessageBox(_T("�ֵ�ֵû������������"));
				bSuc=0;
				break;
//				AlertCtrl(0);
			}
			rechecknum++;
			//delete []g_lpGray;
			//g_lpGray=NULL;
		}
		else
		{
			bSuc=1;
			//gstuTrdStat._CHK1=1;//���ֵ��߳�
			ScanDegree(g_lpGray,gstuPathInf.csPathExe + _T("\\check\\position.txt"));//���degree[]
			break;
		}
	}
	//g_num=new int[10*10*8];
	//GetNumber();
	delete [] pDest.pdata;
		pDest.pdata=NULL;
	//delete []g_num;
	delete []g_lpGray;
	g_lpGray=NULL;
	//g_num=NULL;
	return bSuc;
}

BOOL CScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_BTN_VGASTOP)->EnableWindow(1);
	GetDlgItem(IDC_BTN_VGAINIT)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGASCR)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGASNAP)->EnableWindow(1);
	m_print_xx = 0.8;
	m_print_yy = 1;
	UpdateData(FALSE);
	// TODO: VGA��ʼ��
	RGBERROR error;
	m_iIndex=0;
	hRGBCapture = 0;
	BufferSize=1024*768*3;
	RGBCAPTUREPARMS CaptureParms;  //��ŵ�ǰ�ɼ����Ĳ���
	error = RGBCaptureInitialise(&m_ulNumofcard);//��ʼ��
	if (RGBERROR_NO_ERROR != error)
	{
		//��ʼ��ʧ�ܣ�����ֵ��Ϊ��RGBERROR_HARDWARE_NOT_FOUND
		if (RGBERROR_HARDWARE_NOT_FOUND == error)
		{
			//û���ҵ�Ӳ���豸,��ӡ��־
			AfxMessageBox(_T("RGBCaptureInitialise() ��ʼ����û���ҵ�Ӳ���豸\n"));
			//break;
			//return(FALSE);
		}
		return 0;
	}
	//�򿪲ɼ���
	error = RGBCaptureOpen(m_iIndex,&hRGBCapture);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
			case RGBERROR_HARDWARE_NOT_FOUND :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�Ӳ���豸\n"));//û���ҵ��豸
				break;
			}
			case RGBERROR_INVALID_POINTER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�ָ���Ӳ���豸��ָ��\n"));//û���ҵ��豸
				break;
			}
			case RGBERROR_INVALID_INDEX :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��û���ҵ�Ӳ���豸������\n"));//û���ҵ��豸
				break;
			}
			case RGBERROR_DEVICE_IN_USE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��Ӳ���豸���ڱ�ʹ��\n"));//Ӳ���豸���ڱ�ʹ��
				break;
			}
			case RGBERROR_UNABLE_TO_LOAD_DRIVER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸�����ܼ�������\n"));//��Ӳ��������
				break;
			}
			case RGBERROR_INVALID_DEVICE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() ���豸��Ӳ��������\n"));//��Ӳ��������
				break;
			}
			default:
			{
				break;
			}
		}
		return 0;
	}
	pCaptureParms.Input=0;//VGA����
	pCaptureParms.datamode=2;//0,uyvy,2,RGB��ʽ
	pCaptureParms.BuffersNumber=0;// ��֡��
	pCaptureParms.graytest=0;
	pCaptureParms.FramespSec=50;// 50֡
	pCaptureParms.Size=sizeof(pCaptureParms);// �ṹ����
	pCaptureParms.Flags= //RGBCAPTURE_PARM_PIXELFORMAT
							RGBCAPTURE_PARM_NOTIFY_ARG
						   | RGBCAPTURE_PARM_NOTIFY_FN
                          // | RGBCAPTURE_PARM_HWND
						   | RGBCAPTURE_PARM_DATAMODE
						   | RGBCAPTURE_PARM_GRAYTEST
						   | RGBCAPTURE_PARM_FRAMESPSEC
						   | RGBCAPTURE_PARM_INPUT
						   | RGBCAPTURE_PARM_BUFFERSNUM
                           | RGBCAPTURE_PARM_NOTIFY_FLAGS
						   | RGBCAPTURE_PARM_VDIF_DESCRIPTION;    //���õĲ����У���ǰ����ģʽ�������ھ����֪ͨ��Ϣģʽ
	//pixelFormat = DetectPixelFormat(m_hWnd,(DWORD *)&pRGBBitmap->colorMark);
    //pCaptureParms.PixelFormat = pixelFormat;
	//pCaptureParms.PixelFormat=RGBPIXELFORMAT_888;
	pCaptureParms.NotifyArg=this;
	pCaptureParms.NotifyFn=callbackNotify;
    pCaptureParms.NotifyFlags = RGBNOTIFY_NO_SIGNAL | RGBNOTIFY_MODE_CHANGE ;
	//pCaptureParms.HWnd=m_hWnd;
	//���ò���
	error = RGBCaptureSetParameters(hRGBCapture, &pCaptureParms,RGBCAPTURE_FLAG_CURRENT|RGBCAPTURE_FLAG_REAL_WORLD);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				 AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ���������û���ҵ�Ӳ���豸\n"));//û���ҵ�Ӳ���豸
				 break;
			}

			case RGBERROR_INVALID_POINTER:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ���������û���ҵ�����ָ��\n"));//û���ҵ�����ָ��
				break;
			}

			case RGBERROR_INVALID_SIZE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�������������ָ���С����\n"));//����ָ���С����
				break;
			}

			case RGBERROR_INVALID_FLAGS:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵı�־λ����\n"));//��֧�ֵı�־λ����
				break;
			}

			case RGBERROR_INVALID_FORMAT:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ������źŸ�ʽ\n"));//��֧�ֵ������źŸ�ʽ
				break;
			}

			case RGBERROR_INVALID_BLACKLEVEL:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵĺ�ɫ��\n"));//��֧�ֵĺ�ɫ��
				break;
			}

			case RGBERROR_INVALID_PIXEL_FORMAT:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ�����ģʽ\n"));//��֧�ֵ�����ģʽ
				break;
			}

			case RGBERROR_INVALID_HWND:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ������������ǵ�ǰ���ھ��\n"));//���ǵ�ǰ���ھ��
				break;
			}

			case RGBERROR_INVALID_SYNCEDGE:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ�ͬ����\n"));//��֧�ֵ�ͬ����
				break;
			}

			case RGBERROR_HSCALED_OUT_OF_RANGE:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() ���òɼ�����������֧�ֵ����ű���\n"));//��֧�ֵ����ű���
				break;
			}

			default:
			{
				break;
			}
		}
		return 0;
	}
	/*��ʼ�ɼ��¼�*/
	error = RGBCaptureEnable(hRGBCapture, TRUE);
	if (RGBERROR_NO_ERROR != error)     //���豸ʧ�ܣ����ؽ�����£�
	{	
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ������򿪶�����û���ҵ���Ӳ���豸���\n"));//�򿪶�����û���ҵ���Ӳ���豸���
				break;
			}

			case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ�����Ӳ���豸û�д�\n"));//Ӳ���豸û�д�
				break;
			}

			case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() �����ɼ����������߳�ʧ��\n"));//�����߳�ʧ��
				break;
			}

			default:
			{
				break;
			}
		}
		return 0;
	}
	bufferSize =1600 * 1200 * PixelFormatToBytesPerPixel[RGBPIXELFORMAT_888];
	if (!AllocateBuffers(hRGBCapture, &pCaptureBuffer, m_hWnd, bufferSize))
	{
		return 0;
	} 
	//////////////////////////////////////////////////////////////////////////
	/*���òɼ�����*/
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1)) 
	{
		AfxMessageBox(_T("SetCapture(),���òɼ�����ʧ��"));
		return 0;
	}//��ʼ�����
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
