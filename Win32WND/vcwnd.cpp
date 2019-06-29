#include "stdafx.h"
#include "vcwnd.h"


vcwnd::vcwnd()
{
}


vcwnd::~vcwnd()
{
}

void GetPathFromBrowse(TCHAR *path)

{
 // setlocale(LC_ALL, "chs");
  //OPENFILENAME openFile;
  //memset(&openFile, 0, sizeof(openFile));
  //openFile.lStructSize = sizeof(OPENFILENAME);
  //openFile.nMaxFile = MAX_PATH;
  //openFile.lpstrFilter = L"文件夹\0..\0\0";
  //openFile.lpstrFile = path;
  //openFile.nFilterIndex = 1;
  //openFile.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST /*| OFN_ENABLEHOOK*/ | OFN_HIDEREADONLY;
  ////openFile.hInstance = (HMODULE)GetCurrentProcess();
  ////openFile.lpfnHook = MyFolderProc;
  //BOOL b = GetOpenFileName(&openFile);

  //OPENFILENAME ofn = { 0 };
  //ofn.lStructSize = sizeof(ofn);
  //ofn.hwndOwner = wnd;
  //ofn.lpstrFilter = _T("Exe文件(*.exe)\0*.exe\0所有文件(*.*)\0*.*\0");//要选择的文件后缀???
  //ofn.lpstrInitialDir = _T("D:\\ProgramFiles");//默认的文件路径???
  //ofn.lpstrFile = path;//存放文件的缓冲区???
  //ofn.nMaxFile = 1/*sizeof(szBuffer) / sizeof(*szBuffer)*/;
  //ofn.nFilterIndex = 0;
  //ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT??
  //BOOL bSel = GetOpenFileName(&ofn);

  OPENFILENAME     ofn;

  memset(&ofn, 0, sizeof(ofn));

  ofn.lStructSize         =     sizeof(ofn);

  ofn.lpstrFile         = path;

  ofn.nMaxFile         =     MAX_PATH;

  ofn.lpstrFilter = L"BMP文件(*.bmp)\*.bmp";//L"*.bmp";

  ofn.lpstrInitialDir     =     _T("c:\\");

  ofn.Flags         = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER

                |     OFN_ENABLESIZING;

  //GetOpenFileName(&ofn);
  GetSaveFileName(&ofn);

}
//用VC进行屏幕截取编程
//潘小龙 顾时明
//屏幕截取是令人比较感兴趣的事情.虽然现在有不少应用程序如HYPERSNAP等可以用来截取你所喜欢的屏幕画面, 但是如果能把这个功能加到自己的程序中, 就更能利用它强大的作用.
//下面用VC来逐步介绍在Windows95下的实现过程.首先我们要确定屏幕截取的区域, 用LPRECT结构来定义.可以截取一个窗口, 或整个屏幕.以下代码把选定的屏幕区域拷贝到位图中.
void CopyBitmapToHdc(HDC hdc , RECT rect, HBITMAP bm)
{
  HDC hSrcDC;
  hSrcDC = CreateCompatibleDC(hdc);
  SelectObject(hSrcDC, bm);
  //SetStretchBltMode(hdc, COLORONCOLOR);

  ::StretchBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hSrcDC, 0, 0, rect.right- rect.left, rect.bottom- rect.top, SRCCOPY);
}
HBITMAP CopyScreenToBitmap(LPRECT lpRect)
//lpRect 代表选定区域
{
	HDC       hScrDC, hMemDC;
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;
	// 位图句柄
	int       nX, nY, nX2, nY2;
	// 选定区域坐标
	int       nWidth, nHeight;
	// 位图宽度和高度
	int       xScrn, yScrn;
	// 屏幕分辨率

	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;
	//为屏幕创建设备描述表
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect ->left;
	nY = lpRect ->top;
	nX2 = lpRect ->right;
	nY2 = lpRect ->bottom;
	// 获得屏幕分辨率
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > xScrn)
		nX2 = xScrn;
	if (nY2 > yScrn)
		nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	BitBlt(hMemDC, 0, 0, nWidth, nHeight,
		hScrDC, nX, nY, SRCCOPY);
	//得到屏幕位图的句柄
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	

// 返回位图句柄
	return hBitmap;
}


void putthebmpinclip(HWND hWnd, HANDLE hBitmap)
{
	//得到屏幕位图句柄以后, 我们
	//可以把屏幕内容粘贴到剪贴板上.
	if (OpenClipboard(GetDesktopWindow()/*hWnd*/))
		//hWnd为程序窗口句柄
	{
		//清空剪贴板
		EmptyClipboard();
		//把屏幕内容粘贴到剪贴板上,
		/*hBitmap 为刚才的屏幕位图句柄*/
		SetClipboardData(CF_BITMAP, hBitmap);
		//关闭剪贴板
		CloseClipboard();
	}
}

HBITMAP takethebmpfromclip(HWND hWnd)
{
	//得到屏幕位图句柄以后, 我们
  HBITMAP hBitmap = NULL;
  if (IsClipboardFormatAvailable(CF_BITMAP))
  {
    //（1）开始从剪贴板取数据    
    OpenClipboard(GetDesktopWindow());
    hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
    CloseClipboard();
  }
  return hBitmap;
}
//我们也可以把屏幕内容以位图格式存到磁盘文件上.

int SaveBitmapToFile(HBITMAP hBitmap,
	LPTSTR lpFileName) //hBitmap 为刚才的屏幕位图句柄
{    	 //lpFileName 为位图文件名
	HDC            hDC;
	//设备描述表
	int            iBits;
	//当前显示分辨率下每个像素所占字节数
	WORD            wBitCount;
	//位图中每个像素所占字节数
	//定义调色板大小， 位图中像素字节大小 ，
	/*位图文件大小 ， 写入文件字节数*/
		DWORD           dwPaletteSize = 0,
		dwBmBitsSize,
		dwDIBSize, dwWritten;
	BITMAP          Bitmap;
	//位图属性结构
	BITMAPFILEHEADER   bmfHdr;
	//位图文件头结构
	BITMAPINFOHEADER   bi;
	//位图信息头结构 
	LPBITMAPINFOHEADER lpbi;
	//指向位图信息头结构
	HANDLE          fh, hDib, hPal, hOldPal = NULL;
	//定义文件，分配内存句柄，调色板句柄

	//计算位图文件每个像素所占字节数
	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) *
		GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
  else
    wBitCount = 32;
	//计算调色板大小
	if (wBitCount <= 8)
		dwPaletteSize = (1 <<  wBitCount) *
		sizeof(RGBQUAD);

	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmBitsSize = ((Bitmap.bmWidth *
		wBitCount + 31) / 32) * 4
		* Bitmap.bmHeight;
	//为位图内容分配内存
	hDib = GlobalAlloc(GHND, dwBmBitsSize +
		dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize,
		(LPBITMAPINFO)
		lpbi, DIB_RGB_COLORS);
	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}
	//创建位图文件    
	fh = CreateFile(lpFileName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;
	// 设置位图文件头
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof
		(BITMAPFILEHEADER)
		+ (DWORD)sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize;
	// 写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof
		(BITMAPFILEHEADER), &dwWritten, NULL);
	// 写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
}

void SaveBitmapToFile2(HBITMAP hBitmap, LPTSTR lpFileName)
{
  HDC hdc;			//设备描述表
  int ibits;
  WORD wbitcount;     //当前显示分辨率下每个像素所占字节数

  //位图中每个像素所占字节数，定义调色板大小，位图中像素字节大小，位图文件大小 ，写入文件字节数
  DWORD dwpalettesize = 0, dwbmbitssize, dwdibsize, dwwritten;

  BITMAP bitmap;				//位图属性结构
  BITMAPFILEHEADER bmfhdr;	//位图文件头结构
  BITMAPINFOHEADER bi;		//位图信息头结构
  LPBITMAPINFOHEADER lpbi;	//指向位图信息头结构

  //定义文件，分配内存句柄，调色板句柄
  HANDLE fh, hdib, hpal, holdpal = NULL;

  //计算位图文件每个像素所占字节数
  hdc = CreateDC(L"display", NULL, NULL, NULL);
  ibits = GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES);
  DeleteDC(hdc);

  if (ibits <= 1)
    wbitcount = 1;
  else if (ibits <= 4)
    wbitcount = 4;
  else if (ibits <= 8)
    wbitcount = 8;
  else if (ibits <= 16)
    wbitcount = 16;
  else if (ibits <= 24)
    wbitcount = 24;
  else
    wbitcount = 32;

  //计算调色板大小
  if (wbitcount <= 8)
    dwpalettesize = (1 << wbitcount) * sizeof(RGBQUAD);

  //设置位图信息头结构
  GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bitmap);
  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biWidth = bitmap.bmWidth;
  bi.biHeight = bitmap.bmHeight;
  bi.biPlanes = 1;
  bi.biBitCount = wbitcount;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed = 0;
  bi.biClrImportant = 0;

  dwbmbitssize = ((bitmap.bmWidth * wbitcount + 31) / 32) * 4 * bitmap.bmHeight;
  //为位图内容分配内存
  hdib = GlobalAlloc(GHND, dwbmbitssize + dwpalettesize + sizeof(BITMAPINFOHEADER));
  lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
  *lpbi = bi;

  // 处理调色板 
  hpal = GetStockObject(DEFAULT_PALETTE);
  if (hpal)
  {
    hdc = ::GetDC(NULL);
    holdpal = SelectPalette(hdc, (HPALETTE)hpal, false);
    RealizePalette(hdc);
  }

  // 获取该调色板下新的像素值
  GetDIBits(hdc, hBitmap, 0, (UINT)bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwpalettesize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

  //恢复调色板 
  if (holdpal)
  {
    SelectPalette(hdc, (HPALETTE)holdpal, true);
    RealizePalette(hdc);
    ::ReleaseDC(NULL, hdc);
  }

  //创建位图文件 
  fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  if (fh == INVALID_HANDLE_VALUE)
    return;

  // 设置位图文件头
  bmfhdr.bfType = 0x4d42; // "bm"
  dwdibsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
    dwpalettesize + dwbmbitssize;
  bmfhdr.bfSize = dwdibsize;
  bmfhdr.bfReserved1 = 0;
  bmfhdr.bfReserved2 = 0;
  bmfhdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
    (DWORD)sizeof(BITMAPINFOHEADER) + dwpalettesize;

  //写入位图文件头
  WriteFile(fh, (LPSTR)&bmfhdr, sizeof(BITMAPFILEHEADER), &dwwritten, NULL);

  //写入位图文件其余内容
  WriteFile(fh, (LPSTR)lpbi, dwdibsize, &dwwritten, NULL);
  //清除 
  GlobalUnlock(hdib);
  GlobalFree(hdib);
  CloseHandle(fh);
}

void testbmp(RECT rect)
{
	//RECT rect = { 10, 10, 100, 100};
	HBITMAP hbitmap = CopyScreenToBitmap(&rect);
	putthebmpinclip(NULL, hbitmap);
}

void savebmp(RECT rect, LPTSTR lpFileName)
{
  //RECT rect = { 10, 10, 100, 100};
  HBITMAP hbitmap = takethebmpfromclip(NULL);
  if(hbitmap)
  SaveBitmapToFile2(hbitmap, lpFileName);
}


///////////////////////////鼠标钩子
//低级钩子不需要dll，但只能获得当前运行程序内的所有鼠标和键盘的消息
//但是若要捕获全部线程的鼠标和键盘消息，就需要将钩子函数安装在动态链接库内，不考虑
//
// 全局变量和全局函数定义
//
p_msfn g_msfn;
p_kbfn g_kbfn;
HHOOK hhookMs = NULL;
HHOOK hhookKb = NULL;
void UninstallKbHook();
void InstallKbHook(HINSTANCE hInst);
//
// 安装鼠标Hook
//
void StartHook(HINSTANCE hInst, p_msfn msfn, p_kbfn kbfn)
{
  if (!msfn || !kbfn)
    return;

  g_msfn = msfn;
  g_kbfn = kbfn;

  InstallKbHook(hInst);
}

//
// 卸掉键盘Hook
//
void EndHook()
{
  UninstallKbHook();

}

LRESULT CALLBACK LowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
  MSLLHOOKSTRUCT *pkbhs = (MSLLHOOKSTRUCT *)lParam;
  char strMsg[100] = { 0 };

  switch (nCode)
  {
  case HC_ACTION:
  {
    if (g_msfn)
    {
      if (g_msfn(wParam, pkbhs->pt))
        return TRUE;
    }    
    //鼠标移动
    if (wParam == WM_MOUSEMOVE)
    {
      //sprintf(strMsg, "WM_MOUSEMOVE: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);
      //OutputDebugString(strMsg);
    }

    //鼠标左击
    if (wParam == WM_LBUTTONDOWN)
    {
      //sprintf(strMsg, "WM_LBUTTONDOWN: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);
      //OutputDebugString(strMsg);
    }

    // 			//滚轮事件
    // 			if (wParam == WM_MOUSEWHEEL)
    // 			{
    // 				sprintf(strMsg, "WM_MOUSEWHEEL: %d\n", HIWORD(pkbhs->mouseData));
    // 				OutputDebugString(strMsg);
    // 			}
  }
  default:
    break;
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
  // By returning a non-zero value from the hook procedure, the
  // message does not get passed to the target window
  KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *)lParam;
  BOOL bControlKeyDown = 0;

  HWND hWnd = GetForegroundWindow();
  WCHAR szClassName[MAX_PATH] = { 0 };
  ::GetClassName(hWnd, szClassName, MAX_PATH);

  switch (nCode)
  {
  case HC_ACTION:
  {

    // Check to see if the CTRL key is pressed
    bControlKeyDown = GetAsyncKeyState(VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);

    if (g_kbfn && WM_KEYDOWN== wParam)
    {
      if (g_kbfn(pkbhs->vkCode, bControlKeyDown, pkbhs->flags & LLKHF_ALTDOWN))
        return TRUE;
    }
    ////Disable CTRL+ESC
    //if (pkbhs->vkCode == VK_ESCAPE && bControlKeyDown)
    //  return 1;

    ////Disable ALT+TAB
    //if (pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN)
    //  return 1;

    ////Disable ALT+ESC
    //if (pkbhs->vkCode == VK_ESCAPE && pkbhs->flags & LLKHF_ALTDOWN)
    //  return 1;

    break;
  }

  default:
    break;
  }
  //MessageBoxA(NULL, "aaa", NULL, MB_OK);
  return CallNextHookEx(NULL, nCode, wParam, lParam);



}

void InstallKbHook(HINSTANCE hInst)
{

  if (hhookMs || hhookKb)
    UninstallKbHook();

  hhookMs = SetWindowsHookEx(WH_MOUSE_LL,
    (HOOKPROC)LowLevelMouseProc, hInst/*AfxGetApp()->m_hInstance*/, NULL);

  hhookKb = SetWindowsHookEx(WH_KEYBOARD_LL,
    (HOOKPROC)LowLevelKeyboardProc, hInst/*AfxGetApp()->m_hInstance*/, NULL);

  return;

}

void UninstallKbHook()
{
  if (hhookMs) {
    BOOL fOk = UnhookWindowsHookEx(hhookMs);
    hhookMs = NULL;
  }

  if (hhookKb) {
    BOOL fOk = UnhookWindowsHookEx(hhookKb);
    hhookKb = NULL;
  }
  return;
}