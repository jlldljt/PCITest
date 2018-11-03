// Screen.cpp : 实现文件
//

#include "stdafx.h"
#include "BER.h"
#include "Screen.h"
#include "afxdialogex.h"

//========================================================屏幕识别需要东西START==========================================================================================//
static int PixelFormatToBytesPerPixel[] =
{
	0  /* RGBPIXELFORMAT_UNKNOWN */,
	4  /* RGBPIXELFORMAT_555 */,
	4  /* RGBPIXELFORMAT_565 */,
	8  /* RGBPIXELFORMAT_888 */,
};

RGBCAPTUREBUFFER pCaptureBuffer;//读屏幕中用到
RGBBITMAP       RGBBitmap1;     //第一个采集事件位图信息
TPicRegion pDest;				//读屏幕中用到
int m_VGAthreshold;			//读屏幕中用到

void CALLBACK callbackNotify(PVOID NotifyArg,   UINT   message,   WPARAM   wParam,   LPARAM   lParam)
{
	CScreen* psamp = (CScreen*)NotifyArg;
	switch(message)
	{
	case RGBWM_FRAMECAPTURED:
		{
			psamp->OnMyMessage_Sta1(wParam, lParam);//缓存里已经采集到一帧图像
			break;
		}
	case RGBWM_MODECHANGED:
		{
			psamp->OnMyMessage_modechange(wParam, lParam);//检测改变采集模式
			break;
		}
	case RGBWM_NOSIGNAL:
		{
			psamp->OnMyMessage_nosignal(wParam, lParam);//没信号
			break;
		}
	default:
		{
			break;
		}
	}
	//	::PostMessage(psamp->m_hWnd, message, wParam, lParam);	
}
//========================================================屏幕识别需要东西END==========================================================================================//



// CScreen 对话框

IMPLEMENT_DYNAMIC(CScreen, CDialogEx)

CScreen::CScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreen::IDD, pParent)
	, m_print_xx(0)
	, m_print_yy(0)
{
	g_dlgScreen=this;
	g_num=NULL;//保存10个数的0,1分布
	g_lpGray=NULL;//保存二值化后的图像数据
	g_num=new int[10*10*8];
	// TODO:  初始化
	signalflag = FALSE;
	//pData = new LPVOID[1600*1200*4];
	datamode = 2;
	bufferSize = 0L;         //第1个采集卡对应的缓存大小
	m_VGAthreshold=110;

	LoadMark(_T(".\\check\\qz_42.txt"));
	GetNumber();
	/*EnableToolTips(TRUE);   //true为打开提示功能，false 为关闭提示功能
	m_tooltip.Create(this);//创建
	m_tooltip.SetTipTextColor(RGB(0,0,255));//设定文字颜色
	m_tooltip.SetDelayTime(150);//设定文字停留时间
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_LOAD),"载入图片后，进行实时检测。");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_COMPARE),"一载入图片，点击后开始实时检测。");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_STOP_VGA),"停止采集，需要重新初始化vga后才能再次采集。");
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_TEST),"测试编辑框内的图片文件，可更改测试图片重新点击测试。");
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_BMP_PATH),"测试图片路径");
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_VGA_THRASHOLD),"检测图片时使用的阈值");*/

	
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
//========================================================屏幕识别需要东西START==========================================================================================//




//保存当前显示屏上所有的图形信息 add wygwl 2010-7-16
BOOL CScreen::SaveImageToFile(CString sFile, int iW, int iH, BYTE * pInfo) 
{
	
#define WIDTHBYTES(bits) ((((bits) + 31) >> 5) << 2)
	int nBitCount = 32;
	int nLineBytes = WIDTHBYTES(iW * nBitCount);
	int dwDibSize = sizeof(BITMAPINFOHEADER) + nLineBytes*iH;//图像头和图像数据

	CString sPathName;
	unsigned char * pDib= NULL;
	{
		sPathName  = sFile;
		pDib = new unsigned char[dwDibSize];
		if (pDib == NULL) 
			return 0;
		memset(pDib, 0, dwDibSize);//给pDib指针的数据清零dwDibSize字节
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
		for ( int i = 0; i < iH; i++) // 上下反向
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
			file.Write(pDib, dwDibSize);//把pDib写入到文件中，长度为dwDibSize
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
// bitmap 位图CBitmap对象指针
// lpFileName 为位图文件名  
bool SaveBitmapToFile(CBitmap* bitmap, LPCWSTR lpFileName)
{            
	HBITMAP hBitmap;	// 为刚才的屏幕位图句柄
	HDC hDC; //设备描述表  
	int iBits; //当前显示分辨率下每个像素所占字节数  
	WORD wBitCount; //位图中每个像素所占字节数	
	DWORD dwPaletteSize = 0, //定义调色板大小
		dwBmBitsSize,  //位图中像素字节大小   
		dwDIBSize,	 //位图文件大小
		dwWritten;  //写入文件字节数
	BITMAP Bitmap; //位图属性结构  
	BITMAPFILEHEADER bmfHdr; //位图文件头结构  
	BITMAPINFOHEADER bi; //位图信息头结构    
	LPBITMAPINFOHEADER lpbi; //指向位图信息头结构  
	HANDLE fh,	 //定义文件
		hDib,	 //分配内存句柄
		hPal,	//调色板句柄
		hOldPal = NULL;
	
	//计算位图文件每个像素所占字节数  
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
	
	//计算调色板大小  
	if (wBitCount <= 8)  
		dwPaletteSize =	(1 << wBitCount) * sizeof (RGBQUAD);
	
	//设置位图信息头结构  
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
	
	//为位图内容分配内存  
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof (BITMAPINFOHEADER));  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi =	bi;
	
	// 处理调色板 	   
	hPal = GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
		hDC = ::GetDC(NULL);	
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);	
		RealizePalette(hDC);
	}  
	
	//	 获取该调色板下新的像素值  
	GetDIBits(hDC, hBitmap,	0, (UINT) Bitmap.bmHeight,	
		(LPSTR)lpbi + sizeof (BITMAPINFOHEADER)	+ dwPaletteSize,
		(LPBITMAPINFO)lpbi,	DIB_RGB_COLORS);  
	
	//恢复调色板		
	if (hOldPal)	
	{  
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);  
		RealizePalette(hDC);  
		::ReleaseDC(NULL, hDC);
	}  
	
	//创建位图文件			
	fh = CreateFile(lpFileName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL |	FILE_FLAG_SEQUENTIAL_SCAN, NULL);  
	
	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;  
	
	//	 设置位图文件头  
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
	
	//	 写入位图文件头  
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten,	NULL);	
	
	//	 写入位图文件其余内容  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);
	
	//清除		  
	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  

	return TRUE;	
}

/******************************************************************************/
/*
*处理SDK发出的消息RGBWM_FRAMECAPTURED
*消息RGBWM_FRAMECAPTURED:表明当前采集卡缓存准备就绪，其中有一帧图象数据等待被处理
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

	//if ( g_bSaveImage ) // 保存图像到磁盘
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
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，检测失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
				break;
			}

			case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，没有找到设备索引\n"));//没有找到参数指针
				break;
			}

			case RGBERROR_BUFFER_TOO_SMALL:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，缓存太小\n"));//没有找到参数指针
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
*处理像素发生改变时SDK发出的消息RGBWM_MODECHANGED
*重新设置采集参数
*/
void CScreen::OnMyMessage_modechange(WPARAM wParam, LPARAM lParam)
{
	unsigned long *pbufferSize = 0L;	
	RGBCAPTUREPARMS CaptureParms;
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1))  //设置采集参数
	{
		AfxMessageBox(_T("SetCapture(),设置采集参数失败"));
		return;
	}
}
/*****************************************************************************/
/*
*设置采集参数
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
    /*检测当前输入的VGA视频信号分辨率*/
	error = RGBCaptureDetectVideoMode(*phRGBCapture, 0, 0, &pCaptureParms->VideoTimings, FALSE);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureDetectVideoMode(), 检测当前输入的VGA视频信号分辨率，检测失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
				break;
			}

			case RGBERROR_INVALID_POINTER:
			{
				AfxMessageBox(_T("RGBCaptureDetectVideoMode(), 检测当前输入的VGA视频信号分辨率，没有找到参数指针\n"));//没有找到参数指针
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
	/*设置窗口标题*/
	//char szWndTitle[120];
	//wsprintf(szWndTitle, "VGA采集卡： ");
    //::SetWindowText(::GetParent(this->m_hWnd), szWndTitle);

	//////////////////////////////////////////////////////////////////////////
    /* 设置位图参数为输出采集数据做准备 */

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
	//	AfxMessageBox(_T("CRGBSAMPDlg::SetCapture DDRelease不成功!!! \n"));
	//	//return FALSE;
	//}
	//if (DD_OK !=ddrawRGB.DDInit(hWnd,pCaptureParms->VideoTimings.HorAddrTime,pCaptureParms->VideoTimings.VerAddrTime))
	//{
	//	AfxMessageBox(_T("CRGBSAMPDlg::SetCapture DDInit画图初始化不成功!!! \n"));
	//	//return FALSE;
	//}	
	//SetEvent(m_hDDrawEvent);

	//////////////////////////////////////////////////////////////////////////
	/*此处检测缓存是否就绪，成功则继续采集事件*/
	error = RGBCaptureFrameBufferReady (hRGBCapture,
                                       pCaptureBuffer.Index,
                                       *pBufferSize);
	if (error != RGBERROR_NO_ERROR)
	{
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，检测失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
				break;
			}

			case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，没有找到设备索引\n"));//没有找到参数指针
				break;
			}

			case RGBERROR_BUFFER_TOO_SMALL:
			{
				AfxMessageBox(_T("RGBCaptureFrameBufferReady(),  检测当前VGA采集卡缓存是否就绪，缓存太小\n"));//没有找到参数指针
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
			AfxMessageBox(_T("FreeBuffers(),释放缓存失败！"));
			return FALSE;
		}
		return FALSE;
	}
	return TRUE;
}
/*****************************************************************************/
/*
*处理当SDK采集不到正确的数据时发出的消息RGBWM_NOSIGNAL
*/
void CScreen::OnMyMessage_nosignal(WPARAM wParam, LPARAM lParam)
{
	CString strInfo;
	switch(wParam)
	{
		case NOSIGNAL_NO_RESOLUTION:
		{
			strInfo = _T("无 VGA 信 号(错误的分辨率 !!!)");
			break;
		}
		case NOSIGNAL_DDRCHECK_ERROR:
		{
			strInfo = _T("无 VGA 信 号(DDR 自检出错 !!!)");
			break;
		}
		case NOSIGNAL_H_OVERFLOW:
		{
			strInfo = _T("无 VGA 信 号(水平偏移量溢出 !!!)");
			break;
		}
		case NOSIGNAL_V_OVERFLOW:
		{
			strInfo = _T("无 VGA 信 号(垂直偏移量溢出 !!!)");
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
*释放缓存区
*/
BOOL CScreen::FreeBuffers(HRGBCAPTURE hRGBCapture, HWND hWnd, RGBCAPTUREBUFFER *pCaptureBuffer)
{
	RGBERROR error;
	HDC hDC = ::GetDC(hWnd);

	if (bUsingAccBuffer1 && (hRGBCapture == hRGBCapture))
   {
	  bUsingAccBuffer1 = false;  //修改标志缓存是否使用的参数为false
   }


   ::ReleaseDC(hWnd, hDC);
   
	if(pCaptureBuffer->LpVoidBuffer)
	{
		//////////////////////////////////////////////////////////////////////////
		/*删除当前的VGA采集缓存区在使用队列中的索引*/
			error = RGBCaptureReleaseBuffer(hRGBCapture, pCaptureBuffer->Index);
			if (RGBERROR_NO_ERROR != error)
			{
				switch(error)
			{
				case RGBERROR_INVALID_HRGBCAPTURE:
				{
					AfxMessageBox(_T("RGBCaptureReleaseBuffer(),   删除当前的VGA采集缓存区在使用队列中的索引，注册失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
					break;
				}
				case RGBERROR_INVALID_POINTER:
				{
					AfxMessageBox(_T("RGBCaptureReleaseBuffer(),    删除当前的VGA采集缓存区在使用队列中的索引，没有找到指向当前缓存存储区的指针\n"));//没有找到指向当前缓存存储区的指针
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
		/*释放缓存区*/
			GlobalFree(pCaptureBuffer->LpVoidBuffer); 
			pCaptureBuffer->LpVoidBuffer = NULL;
	}
	
	//delete pCaptureBuffer;
	return TRUE;
}
/*****************************************************************************/ 
/*
 *数出像素位数
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
*检测当前像素模式
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
 *按照指定大小分配缓存
 */
BOOL CScreen::AllocateBuffers(HRGBCAPTURE hRGBCapture, 
                                      RGBCAPTUREBUFFER *pCaptureBuffer, 
									  HWND hWnd, 
									  UINT BufferSize)
{
	//////////////////////////////////////////////////////////////////////////
   /*分配缓存*/
   pCaptureBuffer->LpVoidBuffer = GlobalAlloc(GMEM_FIXED, BufferSize);
   if (pCaptureBuffer->LpVoidBuffer == NULL)
   {
	   return FALSE;
   }
	AfxMessageBox(_T("%d"),GlobalSize(pCaptureBuffer->LpVoidBuffer));
   //////////////////////////////////////////////////////////////////////////
   /*注册当前的VGA采集事件*/
   RGBERROR error;
   error = RGBCaptureUseMemoryBuffer(hRGBCapture, pCaptureBuffer->LpVoidBuffer, BufferSize, &pCaptureBuffer->Index);
   if (RGBERROR_NO_ERROR != error)
   {
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  注册当前的VGA采集事件，注册失败（打开的设备队列中没有找到该硬件设备句柄）\n"));//检测失败（打开的设备队列中没有找到该硬件设备句柄）
				break;
			}

			case RGBERROR_INVALID_POINTER:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  注册当前的VGA采集事件，没有找到指向当前缓存存储区的指针\n"));//没有找到指向当前缓存存储区的指针
				break;
			}

			case RGBERROR_INVALID_INDEX:
			{
				AfxMessageBox(_T("RGBCaptureUseMemoryBuffer(),  注册当前的VGA采集事件，没有找到对应当前缓存存储区的索引\n"));//没有找到对应当前缓存存储区的索引
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
	  bUsingAccBuffer1 = true;  //修改标志缓存使用的参数为TRUE
   }
   
   return TRUE;
}
//========================================================屏幕识别需要东西END============================================================================================//

void CScreen::LoadMark(CString file_temp)
{
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	//file_temp="check\\qz_42.txt";   //保存文件相机参数文件；
	CString fileName(file_temp);//CString a (_T ("aaa"));

	if(!File2.Open(fileName,CFile::modeRead))
	{
		AfxMessageBox(_T("数字匹配文件未找到"));
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
				if(tmep_num>75)//判断图片标记
				{
					first=true;
					height_num=i;
					width_num=j;
					strTmp.Format(_T("输出行，列值：%d，%d "),i,j);
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
				if(0==w_num&&b_num<3)//判断图片标记
				{
					first=true;
					height_num=i;
					width_num=j;
					strTmp.Format(_T("输出行，列值：%d，%d"),i,j);
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
//从图片中扫描角度
void CScreen::GetNumber()
{

	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2,file_temp;// 定义一个CString，作为一个缓冲区
	for(int p=0;p<10;p++)
	{
		file_temp.Format(_T(".\\check\\num_%d.txt"),p); 
		if(!File2.Open(file_temp,CFile::modeRead))
		{
			AfxMessageBox(_T("没有找到数字匹配文件"));
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
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	int nocheckout=0;
	char tmep_num=0;
	for(int m=0;m<30;m++,t++)//总共30个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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
	if(nocheckout>5 ) //未找到匹配数字
	{
		degree[0]=-1;
		//break;
	}
}
void CScreen::ScanDegreeEX(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	int nocheckout=0;
	char tmep_num=0;
	for(int m=0;m<30;m++,t++)//总共30个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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
	if(nocheckout>5 ) //未找到匹配数字
	{
		degree[0]=-1;
		//break;
	}
}
void CScreen::ScanDegreeDynamic(BYTE * lpGray,CString file_temp)
{
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;

	int t=0;
	char tmep_num=0;
	for(int m=0;m<6;m++,t++)//总共6个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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

		pDest.pdata = new TARGB32[height*width];//b,g,r,a  4个8位组成一个像素
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("PIC\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

		//SaveImageToFile(_T("PIC\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
		DECODE_THRESHOLD(pDest,pDest,lpGray,m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

		//SaveImageToFile(_T("PIC\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(_T("check/qz_42.txt"));

		FindOrigin(lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 

			//AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
		}
		else
			break;
	}
	//g_num=new int[10*10*8];
	//GetNumber();

	//////////////////////////////////////////////////
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	int M[5]={0};
	int t=0;
	char tmep_num=0;
	for(int m=0;m<5;m++,t++)//总共5个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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
	if(0!=M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4])//判断M值
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

		pDest.pdata = new TARGB32[height*width];//b,g,r,a  4个8位组成一个像素
		pDest.height = height;
		pDest.width  = width;
		pDest.byte_width = width << 2;

		g_height=height;
		g_width=width;
		lpGray=new BYTE[g_height*g_width];

		int iW=width, iH=height;
		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//SaveImageToFile(_T("PIC\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

		//SaveImageToFile(_T("PIC\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
		DECODE_THRESHOLD(pDest,pDest,lpGray,m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

		//SaveImageToFile(_T("PIC\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
		delete [] pDest.pdata;
		pDest.pdata=NULL;
		LoadMark(_T("check/qz_42.txt"));

		FindOrigin(lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 

			//AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
		}
		else
			break;
	}
	g_num=new int[10*10*8];
	GetNumber();

	//////////////////////////////////////////////////
	CStdioFile File2;// 定义一个CStdioFile类变量File
	CString FileData2;//,file_temp;// 定义一个CString，作为一个缓冲区
	CString fileName_position(file_temp);//
	File2.Open(fileName_position,CFile::modeRead);
	int x,y;
	//CString str;
	int M[5]={0};
	int t=0;
	char tmep_num=0;
	for(int m=0;m<5;m++,t++)//总共5个数
	{
		File2.ReadString(FileData2);
		x=_wtoi(FileData2);
		File2.ReadString(FileData2);
		y=_wtoi(FileData2);
		tmep_num=0;
		int i=y;//10行8列
		{
			int j=x;
			{
				for(int p=0;p<10;p++)//10个数
				{
					for(int k=0;k<10;k++)//10行
						for(int l=0;l<8;l++)//8列
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
	if(14742==M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4] || 14993==M[0]*10000+M[1]*1000+M[2]*100+M[3]*10+M[4])//判断M值
		return 1;
	else
		return 0;
}

// CScreen 消息处理程序


void CScreen::OnBnClickedBtnVgasnap()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_VGASCR)->EnableWindow(1);
	GetDlgItem(IDC_BTN_VGASNAP)->EnableWindow(0);
	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	pDest.pdata = new TARGB32[width*height];//b,g,r,a  4个8位组成一个像素
	pDest.height = height;
	pDest.width  = width;
	pDest.byte_width = width << 2;

	g_height=height;
	g_width=width;
	g_lpGray=new BYTE[g_height*g_width];

	int iW=width, iH=height;
	DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	SaveImageToFile(gstuPathInf.csPathExe + _T("\\PIC\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

	DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

	SaveImageToFile(gstuPathInf.csPathExe + _T("\\PIC\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
	DECODE_THRESHOLD(pDest,pDest,g_lpGray,m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据
	DECODE_THRESHOLD2(pDest,pDest,m_VGAthreshold);//像素对换
	SaveImageToFile(gstuPathInf.csPathExe + _T("\\PIC\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像


	delete [] pDest.pdata;
	pDest.pdata=NULL;
	///////////////////////

	CString m_bmp_file=gstuPathInf.csPathExe +_T("\\PIC\\yuanshi.bmp");
	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	BOOL ifFind = findini.FindFile(m_bmp_file);  
	if( !ifFind )  
		return;
	CBitmap m_bmp;//创建类成员
	BITMAP bm;//存放位图信息的结构
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
	m_bmp.GetBitmap(&bm);
	CWnd * pWnd = GetDlgItem(IDC_SCREEN_VGA);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	GetDlgItem(IDC_SCREEN_VGA)->GetClientRect(&rect);
	CDC memDC;        //定义一个设备
	CClientDC dc1(this);      //获取客户
	memDC.CreateCompatibleDC( &dc1 );
	memDC.SelectObject( m_bmp );  //为设备选择对象
	pDC->SetStretchBltMode (HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 1000, 500, SRCCOPY);
	memDC.DeleteDC();	
	m_bmp.DeleteObject();
	DeleteObject(hBitmap1);//记得删除	
	pWnd->ReleaseDC(pDC);

	////////////////////////////////////////////////////////
	GetDlgItem(IDC_BTN_VGASCR)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGASNAP)->EnableWindow(1);
	FindOrigin(g_lpGray);
	if(height_num>100 || width_num>100|| (height_num==0&&width_num==0))
	{ 
		AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
		delete []g_lpGray;
		g_lpGray=NULL;
//		GetDlgItem(IDC_COMPARE)->EnableWindow(0);
		return;
	}
	//g_num=new int[10*10*8];
	//GetNumber();
	ScanDegree(g_lpGray,gstuPathInf.csPathExe + _T("\\check\\position.txt"));//获得degree[]
	CString _degree;
	_degree.Format(_T("光轴角度：%d\r\n电轴角度：%d\r\n等效角度：%d\r\n原始光轴：%d\r\n原始电轴：%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
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
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_VGASTOP)->EnableWindow(1);
	GetDlgItem(IDC_BTN_VGAINIT)->EnableWindow(0);
	// TODO: VGA初始化
	/////////////
	//GetDlgItem(IDC_VGA_INIT)->EnableWindow(0);
	/////////////
	RGBERROR error;
	m_iIndex=0;
	hRGBCapture = 0;
	BufferSize=1024*768*3;
	RGBCAPTUREPARMS CaptureParms;  //存放当前采集卡的参数
	error = RGBCaptureInitialise(&m_ulNumofcard);//初始化
	if (RGBERROR_NO_ERROR != error)
	{
		//初始化失败，返回值仅为：RGBERROR_HARDWARE_NOT_FOUND
		if (RGBERROR_HARDWARE_NOT_FOUND == error)
		{
			//没有找到硬件设备,打印日志
			AfxMessageBox(_T("RGBCaptureInitialise() 初始化，没有找到硬件设备\n"));
			//break;
			//return(FALSE);
		}
		return;
	}
	//打开采集卡
	error = RGBCaptureOpen(m_iIndex,&hRGBCapture);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
		case RGBERROR_HARDWARE_NOT_FOUND :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到硬件设备\n"));//没有找到设备
				break;
			}
		case RGBERROR_INVALID_POINTER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到指向该硬件设备的指针\n"));//没有找到设备
				break;
			}
		case RGBERROR_INVALID_INDEX :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到硬件设备的索引\n"));//没有找到设备
				break;
			}
		case RGBERROR_DEVICE_IN_USE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，硬件设备正在被使用\n"));//硬件设备正在被使用
				break;
			}
		case RGBERROR_UNABLE_TO_LOAD_DRIVER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，不能加载驱动\n"));//软硬件不批配
				break;
			}
		case RGBERROR_INVALID_DEVICE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，硬件不存在\n"));//软硬件不批配
				break;
			}
		default:
			{
				break;
			}
		}
		return;
	}
	pCaptureParms.Input=0;//VGA输入
	pCaptureParms.datamode=2;//0,uyvy,2,RGB格式
	pCaptureParms.BuffersNumber=0;// 高帧率
	pCaptureParms.graytest=0;
	pCaptureParms.FramespSec=50;// 50帧
	pCaptureParms.Size=sizeof(pCaptureParms);// 结构长度
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
		| RGBCAPTURE_PARM_VDIF_DESCRIPTION;    //设置的参数有：当前像素模式，主窗口句柄，通知消息模式
	//pixelFormat = DetectPixelFormat(m_hWnd,(DWORD *)&pRGBBitmap->colorMark);
	//pCaptureParms.PixelFormat = pixelFormat;
	//pCaptureParms.PixelFormat=RGBPIXELFORMAT_888;
	pCaptureParms.NotifyArg=this;
	pCaptureParms.NotifyFn=callbackNotify;
	pCaptureParms.NotifyFlags = RGBNOTIFY_NO_SIGNAL | RGBNOTIFY_MODE_CHANGE ;
	//pCaptureParms.HWnd=m_hWnd;
	//配置参数
	error = RGBCaptureSetParameters(hRGBCapture, &pCaptureParms,RGBCAPTURE_FLAG_CURRENT|RGBCAPTURE_FLAG_REAL_WORLD);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，没有找到硬件设备\n"));//没有找到硬件设备
				break;
			}

		case RGBERROR_INVALID_POINTER:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，没有找到参数指针\n"));//没有找到参数指针
				break;
			}

		case RGBERROR_INVALID_SIZE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，参数指针大小错误\n"));//参数指针大小错误
				break;
			}

		case RGBERROR_INVALID_FLAGS:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的标志位操作\n"));//不支持的标志位操作
				break;
			}

		case RGBERROR_INVALID_FORMAT:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的输入信号格式\n"));//不支持的输入信号格式
				break;
			}

		case RGBERROR_INVALID_BLACKLEVEL:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的黑色度\n"));//不支持的黑色度
				break;
			}

		case RGBERROR_INVALID_PIXEL_FORMAT:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的像素模式\n"));//不支持的像素模式
				break;
			}

		case RGBERROR_INVALID_HWND:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不是当前窗口句柄\n"));//不是当前窗口句柄
				break;
			}

		case RGBERROR_INVALID_SYNCEDGE:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的同步沿\n"));//不支持的同步沿
				break;
			}

		case RGBERROR_HSCALED_OUT_OF_RANGE:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的缩放比例\n"));//不支持的缩放比例
				break;
			}

		default:
			{
				break;
			}
		}
		return;
	}
	/*开始采集事件*/
	error = RGBCaptureEnable(hRGBCapture, TRUE);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，打开队列中没有找到该硬件设备句柄\n"));//打开队列中没有找到该硬件设备句柄
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，硬件设备没有打开\n"));//硬件设备没有打开
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，启动线程失败\n"));//启动线程失败
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
	/*设置采集参数*/
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1)) 
	{
		AfxMessageBox(_T("SetCapture(),设置采集参数失败"));
		return;
	}
	//初始化完毕
}


void CScreen::OnBnClickedBtnVgascr()
{
	// TODO: 在此添加控件通知处理程序代码
	//LoadMark(gstuPathInf.csPathExe + _T("\\check\\qz_42.txt"));
	/*
	找到图片的标记起点
	*/
	GetDlgItem(IDC_BTN_VGASCR)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGASNAP)->EnableWindow(1);
	FindOrigin(g_lpGray);
	if(height_num>100 || width_num>100|| (height_num==0&&width_num==0))
	{ 
		AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
		delete []g_lpGray;
		g_lpGray=NULL;
//		GetDlgItem(IDC_COMPARE)->EnableWindow(0);
		return;
	}
	//g_num=new int[10*10*8];
	//GetNumber();
	ScanDegree(g_lpGray,gstuPathInf.csPathExe + _T("\\check\\position.txt"));//获得degree[]
	CString _degree;
	_degree.Format(_T("光轴角度：%d\r\n电轴角度：%d\r\n等效角度：%d\r\n原始光轴：%d\r\n原始电轴：%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
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
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_VGASTOP)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGAINIT)->EnableWindow(1);
	//stopVGA(m_hWnd, &hRGBCapture1, pCaptureBuffer1);
	RGBERROR error;	
	error = RGBCaptureEnable(hRGBCapture, FALSE);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
		case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，打开队列中没有找到该硬件设备句柄\n"));//打开队列中没有找到该硬件设备句柄
				break;
			}

		case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，硬件设备没有打开\n"));//硬件设备没有打开
				break;
			}

		case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，启动线程失败\n"));//启动线程失败
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
	/*释放缓存*/
	if (NULL != &pCaptureBuffer)
	{
		if (!FreeBuffers(hRGBCapture, m_hWnd, &pCaptureBuffer))
		{
			AfxMessageBox(_T("FreeBuffers(),释放缓存失败！"));
			return;
		}
	}
	/*关闭采集卡*/
	RGBCaptureClose(hRGBCapture);
	hRGBCapture = 0;
}


void CScreen::OnBnClickedBtnVgaprt()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CPrintDialog dlg(FALSE,PD_PAGENUMS);     //定义打印机
	//dlg.GetDefaults();
	
	CDC dc;
	if (dlg.DoModal() == IDCANCEL)             //获取用户对打印机的设置
		return;
	HDC hdcPrinter = dlg.GetPrinterDC();       //获得打印机的句柄	

	if (hdcPrinter == NULL)
	{
		MessageBox(_T("Buy a printer!"));
	}
	else
	{
		//GetDlgItem(IDC_PRINT)->EnableWindow(0);
		CDC dcPrinter;                            //定义一个设备
		dcPrinter.Attach(hdcPrinter);             //把打印机附于这个设备
		dcPrinter.m_bPrinting = TRUE;
		DOCINFO docinfo;                          //打印机属性
		memset(&docinfo, 0, sizeof(docinfo));
		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDocName = _T("SC-EFG");
		CPrintInfo info;
		info.m_rectDraw.SetRect(0,0, 
			dcPrinter.GetDeviceCaps(HORZRES), 
			dcPrinter.GetDeviceCaps(VERTRES));//设置范围
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
				SetTextColor(dcPrinter,RGB(0, 255, 0));		  //设置打印机的字体颜色，其实还可以设置字体的			
				DrawBmp(&dcPrinter,m_print_xx,m_print_yy);           //调用打印图片函数，标准的做法应该是还传一个要打印的图片的
				CString str; //获取系统时间
				CTime tm;
				tm=CTime::GetCurrentTime();
				str=tm.Format("%Y-%m-%d %X");
				dcPrinter.TextOut(10, 10, str, str.GetLength());
				dcPrinter.EndPage();            //结束这页
				dcPrinter.EndDoc();             //结束      
			}
		}
		dcPrinter.Detach();             //释放对象
		dcPrinter.DeleteDC();           //释放设备
	}
	//DeleteObject(hdcPrinter);
}

void CScreen::DrawBmp(CDC*pDC,double xx,double yy)
{

	SIZE s1;
	s1.cx = pDC->GetDeviceCaps(HORZRES) ;
	s1.cy = pDC->GetDeviceCaps(VERTRES) ;
	pDC->SetMapMode(MM_ANISOTROPIC); //转化坐标映射模式
	int xLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSX);  //得到设备每逻辑英寸的像素数量
	int yLogPixPerInch = pDC->GetDeviceCaps(LOGPIXELSY);
	float fWidth = (float)xLogPixPerInch / 96 ; //得到电脑屏幕映射到视窗大小比率
	float fHeight = (float)yLogPixPerInch / 96; //一般得到的fWidth = fHeight 
	CDC memDC;        //定义一个设备
	CClientDC dc(this);      //获取客户
	memDC.CreateCompatibleDC( &dc );
	CBitmap m_bmp;//创建类成员
	BITMAP bm;//存放位图信息的结构
	//CString bmp_path =_T(".\\PIC\\screen.bmp");
	//HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	CString bmp_path = gstuPathInf.csPathExe +_T("\\PIC\\erzhihu.bmp"); 
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	//m_bmp.LoadBitmap(bmp_path);
	if(hBitmap1!=NULL)
	{
		m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
		m_bmp.GetBitmap(&bm);//
		CSize size = CSize(bm.bmWidth, bm.bmHeight);
		pDC->SetWindowExt(size); //设置视窗大小
		long xExt = (long)(fWidth * size.cx); //得到视窗大小
		long yExt = (long)(fHeight * size.cy) ;
		if(xExt>s1.cx)
		{

			yExt=yExt*s1.cx/xExt;
			xExt=s1.cx;
		}
		//pDC->SetViewportExt((int)xExt, (int)yExt); //设置视窗大小(由电脑屏幕映射到视窗大小)
		pDC->SetViewportExt((int)size.cx, (int)size.cy); //设置视窗大小(由电脑屏幕映射到视窗大小)
		memDC.SelectObject( m_bmp );  //为设备选择对象
		//pDC->BitBlt(40, 40,(int)xExt, (int)yExt, &memDC, 0, 0, SRCCOPY);             //把源以copy的形式装载到pDC设备中
		pDC->StretchBlt(10, 40,(int)(xExt*xx), (int)(yExt*yy), &memDC, 0, 0,bm.bmWidth, bm.bmHeight, SRCCOPY);
		m_bmp.DeleteObject();
	}
	DeleteObject(hBitmap1);//记得删除	
	memDC.DeleteDC();	
}



void CScreen::OnBnClickedBtnLocal()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CBitmap m_bmp;//创建类成员
	
	BITMAP bm;//存放位图信息的结构
	CString bmp_path =gstuPathInf.csPathExe + _T("\\PIC\\screen.bmp");//_T("d://tmep1.bmp")
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	//m_bmp.LoadBitmap(bmp_path);
	if(NULL!=hBitmap1)
	{
		m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
		m_bmp.GetBitmap(&bm);//
		g_height = bm.bmHeight;//图像的宽高    
		g_width = bm.bmWidth;
		g_widthBytes=bm.bmWidthBytes;//图像一行数据所占的字节数
		int size=g_height*g_widthBytes; 
		BYTE *lpBits=new BYTE[size]; 
		m_bmp.GetBitmapBits(size,lpBits);//得到RGB数据
		BYTE *lpGray=new BYTE[g_height*g_width];
		BYTE R, G, B;
		for(int i=0;i<g_height;i++)
			for(int j=0;j<g_width;j++)
			{
			  B=lpBits[i*g_widthBytes+j*4];
			  G=lpBits[i*g_widthBytes+j*4+1];
			  R=lpBits[i*g_widthBytes+j*4+2];
			  lpGray[i*g_width+j]=BYTE(R*0.3+G*0.59+B*0.11);///(B+G+R)/3;//转灰度
		 
			}
		for(int i=0;i<g_height;i++)
			for(int j=0;j<g_width;j++)
			{
				if(lpGray[i*g_width+j]< 128 )//二值化
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
		找到图片的标记起点
		*/
		FindOrigin(lpGray);
		if(height_num>100 || width_num>100|| (height_num==0&&width_num==0))
		{ 
			AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
			delete []g_lpGray;
			g_lpGray=NULL;
	//		GetDlgItem(IDC_COMPARE)->EnableWindow(0);
			return;
		}


		//g_num=new int[10*10*8];
		//GetNumber();
		ScanDegree(lpGray,gstuPathInf.csPathExe + _T("\\check\\position.txt"));//获得degree[]
		CString _degree;
		_degree.Format(_T("光轴角度：%d\r\n电轴角度：%d\r\n等效角度：%d\r\n原始光轴：%d\r\n原始电轴：%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
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
	DeleteObject(hBitmap1);//记得删除	
}

//返回1，成功
bool CScreen::RCGVGA()
{
	int rechecknum=0;//重测次数
	bool bSuc = 0;//返回值
	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	pDest.pdata = new TARGB32[height*width];//b,g,r,a  4个8位组成一个像素
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
		//SaveImageToFile(_T("PIC\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

		//SaveImageToFile(_T("PIC\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
		DECODE_THRESHOLD(pDest,pDest,g_lpGray,m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

		//SaveImageToFile(_T("PIC\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
		
		//LoadMark(gstuPathInf.csPathExe + _T("\\check\\qz_42.txt"));

		FindOrigin(g_lpGray);
		if(height_num>100 || width_num>200|| (height_num==0&&width_num==0))
		{ 
			if(rechecknum>20)
			{
//				AlertCtrl(1);
				//AfxMessageBox(_T("分档值没读到，继续吗？"));
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
			//gstuTrdStat._CHK1=1;//供分档线程
			ScanDegree(g_lpGray,gstuPathInf.csPathExe + _T("\\check\\position.txt"));//获得degree[]
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

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_BTN_VGASTOP)->EnableWindow(1);
	GetDlgItem(IDC_BTN_VGAINIT)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGASCR)->EnableWindow(0);
	GetDlgItem(IDC_BTN_VGASNAP)->EnableWindow(1);
	m_print_xx = 0.8;
	m_print_yy = 1;
	UpdateData(FALSE);
	// TODO: VGA初始化
	RGBERROR error;
	m_iIndex=0;
	hRGBCapture = 0;
	BufferSize=1024*768*3;
	RGBCAPTUREPARMS CaptureParms;  //存放当前采集卡的参数
	error = RGBCaptureInitialise(&m_ulNumofcard);//初始化
	if (RGBERROR_NO_ERROR != error)
	{
		//初始化失败，返回值仅为：RGBERROR_HARDWARE_NOT_FOUND
		if (RGBERROR_HARDWARE_NOT_FOUND == error)
		{
			//没有找到硬件设备,打印日志
			AfxMessageBox(_T("RGBCaptureInitialise() 初始化，没有找到硬件设备\n"));
			//break;
			//return(FALSE);
		}
		return 0;
	}
	//打开采集卡
	error = RGBCaptureOpen(m_iIndex,&hRGBCapture);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
			case RGBERROR_HARDWARE_NOT_FOUND :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到硬件设备\n"));//没有找到设备
				break;
			}
			case RGBERROR_INVALID_POINTER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到指向该硬件设备的指针\n"));//没有找到设备
				break;
			}
			case RGBERROR_INVALID_INDEX :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，没有找到硬件设备的索引\n"));//没有找到设备
				break;
			}
			case RGBERROR_DEVICE_IN_USE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，硬件设备正在被使用\n"));//硬件设备正在被使用
				break;
			}
			case RGBERROR_UNABLE_TO_LOAD_DRIVER :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，不能加载驱动\n"));//软硬件不批配
				break;
			}
			case RGBERROR_INVALID_DEVICE :
			{
				AfxMessageBox(_T("RGBCaptureOpen() 打开设备，硬件不存在\n"));//软硬件不批配
				break;
			}
			default:
			{
				break;
			}
		}
		return 0;
	}
	pCaptureParms.Input=0;//VGA输入
	pCaptureParms.datamode=2;//0,uyvy,2,RGB格式
	pCaptureParms.BuffersNumber=0;// 高帧率
	pCaptureParms.graytest=0;
	pCaptureParms.FramespSec=50;// 50帧
	pCaptureParms.Size=sizeof(pCaptureParms);// 结构长度
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
						   | RGBCAPTURE_PARM_VDIF_DESCRIPTION;    //设置的参数有：当前像素模式，主窗口句柄，通知消息模式
	//pixelFormat = DetectPixelFormat(m_hWnd,(DWORD *)&pRGBBitmap->colorMark);
    //pCaptureParms.PixelFormat = pixelFormat;
	//pCaptureParms.PixelFormat=RGBPIXELFORMAT_888;
	pCaptureParms.NotifyArg=this;
	pCaptureParms.NotifyFn=callbackNotify;
    pCaptureParms.NotifyFlags = RGBNOTIFY_NO_SIGNAL | RGBNOTIFY_MODE_CHANGE ;
	//pCaptureParms.HWnd=m_hWnd;
	//配置参数
	error = RGBCaptureSetParameters(hRGBCapture, &pCaptureParms,RGBCAPTURE_FLAG_CURRENT|RGBCAPTURE_FLAG_REAL_WORLD);
	if (RGBERROR_NO_ERROR != error)
	{
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				 AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，没有找到硬件设备\n"));//没有找到硬件设备
				 break;
			}

			case RGBERROR_INVALID_POINTER:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，没有找到参数指针\n"));//没有找到参数指针
				break;
			}

			case RGBERROR_INVALID_SIZE:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，参数指针大小错误\n"));//参数指针大小错误
				break;
			}

			case RGBERROR_INVALID_FLAGS:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的标志位操作\n"));//不支持的标志位操作
				break;
			}

			case RGBERROR_INVALID_FORMAT:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的输入信号格式\n"));//不支持的输入信号格式
				break;
			}

			case RGBERROR_INVALID_BLACKLEVEL:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的黑色度\n"));//不支持的黑色度
				break;
			}

			case RGBERROR_INVALID_PIXEL_FORMAT:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的像素模式\n"));//不支持的像素模式
				break;
			}

			case RGBERROR_INVALID_HWND:
			{
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不是当前窗口句柄\n"));//不是当前窗口句柄
				break;
			}

			case RGBERROR_INVALID_SYNCEDGE:
			{   
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的同步沿\n"));//不支持的同步沿
				break;
			}

			case RGBERROR_HSCALED_OUT_OF_RANGE:
			{  
				AfxMessageBox(_T("RGBCaptureSetParameters() 设置采集卡参数，不支持的缩放比例\n"));//不支持的缩放比例
				break;
			}

			default:
			{
				break;
			}
		}
		return 0;
	}
	/*开始采集事件*/
	error = RGBCaptureEnable(hRGBCapture, TRUE);
	if (RGBERROR_NO_ERROR != error)     //打开设备失败，返回结果如下：
	{	
		switch (error)
		{
			case RGBERROR_INVALID_HRGBCAPTURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，打开队列中没有找到该硬件设备句柄\n"));//打开队列中没有找到该硬件设备句柄
				break;
			}

			case RGBERROR_CAPTURE_OUTSTANDING:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，硬件设备没有打开\n"));//硬件设备没有打开
				break;
			}

			case RGBERROR_THREAD_FAILURE:
			{
				AfxMessageBox(_T("RGBCaptureEnable() 启动采集卡，启动线程失败\n"));//启动线程失败
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
	/*设置采集参数*/
	if (!SetCapture(m_hWnd, &CaptureParms, &hRGBCapture, pCaptureBuffer, &bufferSize, &RGBBitmap1)) 
	{
		AfxMessageBox(_T("SetCapture(),设置采集参数失败"));
		return 0;
	}//初始化完毕
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
