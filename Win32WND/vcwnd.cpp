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
  //openFile.lpstrFilter = L"�ļ���\0..\0\0";
  //openFile.lpstrFile = path;
  //openFile.nFilterIndex = 1;
  //openFile.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST /*| OFN_ENABLEHOOK*/ | OFN_HIDEREADONLY;
  ////openFile.hInstance = (HMODULE)GetCurrentProcess();
  ////openFile.lpfnHook = MyFolderProc;
  //BOOL b = GetOpenFileName(&openFile);

  //OPENFILENAME ofn = { 0 };
  //ofn.lStructSize = sizeof(ofn);
  //ofn.hwndOwner = wnd;
  //ofn.lpstrFilter = _T("Exe�ļ�(*.exe)\0*.exe\0�����ļ�(*.*)\0*.*\0");//Ҫѡ����ļ���׺???
  //ofn.lpstrInitialDir = _T("D:\\ProgramFiles");//Ĭ�ϵ��ļ�·��???
  //ofn.lpstrFile = path;//����ļ��Ļ�����???
  //ofn.nMaxFile = 1/*sizeof(szBuffer) / sizeof(*szBuffer)*/;
  //ofn.nFilterIndex = 0;
  //ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT??
  //BOOL bSel = GetOpenFileName(&ofn);

  OPENFILENAME     ofn;

  memset(&ofn, 0, sizeof(ofn));

  ofn.lStructSize         =     sizeof(ofn);

  ofn.lpstrFile         = path;

  ofn.nMaxFile         =     MAX_PATH;

  ofn.lpstrFilter = L"BMP�ļ�(*.bmp)\*.bmp";//L"*.bmp";

  ofn.lpstrInitialDir     =     _T("c:\\");

  ofn.Flags         = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER

                |     OFN_ENABLESIZING;

  //GetOpenFileName(&ofn);
  GetSaveFileName(&ofn);

}
//��VC������Ļ��ȡ���
//��С�� ��ʱ��
//��Ļ��ȡ�����˱Ƚϸ���Ȥ������.��Ȼ�����в���Ӧ�ó�����HYPERSNAP�ȿ���������ȡ����ϲ������Ļ����, ��������ܰ�������ܼӵ��Լ��ĳ�����, �͸���������ǿ�������.
//������VC���𲽽�����Windows95�µ�ʵ�ֹ���.��������Ҫȷ����Ļ��ȡ������, ��LPRECT�ṹ������.���Խ�ȡһ������, ��������Ļ.���´����ѡ������Ļ���򿽱���λͼ��.
void CopyBitmapToHdc(HDC hdc , RECT rect, HBITMAP bm)
{
  HDC hSrcDC;
  hSrcDC = CreateCompatibleDC(hdc);
  SelectObject(hSrcDC, bm);
  //SetStretchBltMode(hdc, COLORONCOLOR);

  ::StretchBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hSrcDC, 0, 0, rect.right- rect.left, rect.bottom- rect.top, SRCCOPY);
}
HBITMAP CopyScreenToBitmap(LPRECT lpRect)
//lpRect ����ѡ������
{
	HDC       hScrDC, hMemDC;
	// ��Ļ���ڴ��豸������
	HBITMAP    hBitmap, hOldBitmap;
	// λͼ���
	int       nX, nY, nX2, nY2;
	// ѡ����������
	int       nWidth, nHeight;
	// λͼ��Ⱥ͸߶�
	int       xScrn, yScrn;
	// ��Ļ�ֱ���

	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
		return NULL;
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect ->left;
	nY = lpRect ->top;
	nX2 = lpRect ->right;
	nY2 = lpRect ->bottom;
	// �����Ļ�ֱ���
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//ȷ��ѡ�������ǿɼ���
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
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	BitBlt(hMemDC, 0, 0, nWidth, nHeight,
		hScrDC, nX, nY, SRCCOPY);
	//�õ���Ļλͼ�ľ��
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	

// ����λͼ���
	return hBitmap;
}


void putthebmpinclip(HWND hWnd, HANDLE hBitmap)
{
	//�õ���Ļλͼ����Ժ�, ����
	//���԰���Ļ����ճ������������.
	if (OpenClipboard(GetDesktopWindow()/*hWnd*/))
		//hWndΪ���򴰿ھ��
	{
		//��ռ�����
		EmptyClipboard();
		//����Ļ����ճ������������,
		/*hBitmap Ϊ�ղŵ���Ļλͼ���*/
		SetClipboardData(CF_BITMAP, hBitmap);
		//�رռ�����
		CloseClipboard();
	}
}

HBITMAP takethebmpfromclip(HWND hWnd)
{
	//�õ���Ļλͼ����Ժ�, ����
  HBITMAP hBitmap = NULL;
  if (IsClipboardFormatAvailable(CF_BITMAP))
  {
    //��1����ʼ�Ӽ�����ȡ����    
    OpenClipboard(GetDesktopWindow());
    hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
    CloseClipboard();
  }
  return hBitmap;
}
//����Ҳ���԰���Ļ������λͼ��ʽ�浽�����ļ���.

int SaveBitmapToFile(HBITMAP hBitmap,
	LPTSTR lpFileName) //hBitmap Ϊ�ղŵ���Ļλͼ���
{    	 //lpFileName Ϊλͼ�ļ���
	HDC            hDC;
	//�豸������
	int            iBits;
	//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD            wBitCount;
	//λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С ��
	/*λͼ�ļ���С �� д���ļ��ֽ���*/
		DWORD           dwPaletteSize = 0,
		dwBmBitsSize,
		dwDIBSize, dwWritten;
	BITMAP          Bitmap;
	//λͼ���Խṹ
	BITMAPFILEHEADER   bmfHdr;
	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER   bi;
	//λͼ��Ϣͷ�ṹ 
	LPBITMAPINFOHEADER lpbi;
	//ָ��λͼ��Ϣͷ�ṹ
	HANDLE          fh, hDib, hPal, hOldPal = NULL;
	//�����ļ��������ڴ�������ɫ����

	//����λͼ�ļ�ÿ��������ռ�ֽ���
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
	//�����ɫ���С
	if (wBitCount <= 8)
		dwPaletteSize = (1 <<  wBitCount) *
		sizeof(RGBQUAD);

	//����λͼ��Ϣͷ�ṹ
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
	//Ϊλͼ���ݷ����ڴ�
	hDib = GlobalAlloc(GHND, dwBmBitsSize +
		dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// �����ɫ��   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}
	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize,
		(LPBITMAPINFO)
		lpbi, DIB_RGB_COLORS);
	//�ָ���ɫ��   
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}
	//����λͼ�ļ�    
	fh = CreateFile(lpFileName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;
	// ����λͼ�ļ�ͷ
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
	// д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof
		(BITMAPFILEHEADER), &dwWritten, NULL);
	// д��λͼ�ļ���������
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);
	//���   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
}

void SaveBitmapToFile2(HBITMAP hBitmap, LPTSTR lpFileName)
{
  HDC hdc;			//�豸������
  int ibits;
  WORD wbitcount;     //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���

  //λͼ��ÿ��������ռ�ֽ����������ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С ��д���ļ��ֽ���
  DWORD dwpalettesize = 0, dwbmbitssize, dwdibsize, dwwritten;

  BITMAP bitmap;				//λͼ���Խṹ
  BITMAPFILEHEADER bmfhdr;	//λͼ�ļ�ͷ�ṹ
  BITMAPINFOHEADER bi;		//λͼ��Ϣͷ�ṹ
  LPBITMAPINFOHEADER lpbi;	//ָ��λͼ��Ϣͷ�ṹ

  //�����ļ��������ڴ�������ɫ����
  HANDLE fh, hdib, hpal, holdpal = NULL;

  //����λͼ�ļ�ÿ��������ռ�ֽ���
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

  //�����ɫ���С
  if (wbitcount <= 8)
    dwpalettesize = (1 << wbitcount) * sizeof(RGBQUAD);

  //����λͼ��Ϣͷ�ṹ
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
  //Ϊλͼ���ݷ����ڴ�
  hdib = GlobalAlloc(GHND, dwbmbitssize + dwpalettesize + sizeof(BITMAPINFOHEADER));
  lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
  *lpbi = bi;

  // �����ɫ�� 
  hpal = GetStockObject(DEFAULT_PALETTE);
  if (hpal)
  {
    hdc = ::GetDC(NULL);
    holdpal = SelectPalette(hdc, (HPALETTE)hpal, false);
    RealizePalette(hdc);
  }

  // ��ȡ�õ�ɫ�����µ�����ֵ
  GetDIBits(hdc, hBitmap, 0, (UINT)bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwpalettesize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

  //�ָ���ɫ�� 
  if (holdpal)
  {
    SelectPalette(hdc, (HPALETTE)holdpal, true);
    RealizePalette(hdc);
    ::ReleaseDC(NULL, hdc);
  }

  //����λͼ�ļ� 
  fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  if (fh == INVALID_HANDLE_VALUE)
    return;

  // ����λͼ�ļ�ͷ
  bmfhdr.bfType = 0x4d42; // "bm"
  dwdibsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
    dwpalettesize + dwbmbitssize;
  bmfhdr.bfSize = dwdibsize;
  bmfhdr.bfReserved1 = 0;
  bmfhdr.bfReserved2 = 0;
  bmfhdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
    (DWORD)sizeof(BITMAPINFOHEADER) + dwpalettesize;

  //д��λͼ�ļ�ͷ
  WriteFile(fh, (LPSTR)&bmfhdr, sizeof(BITMAPFILEHEADER), &dwwritten, NULL);

  //д��λͼ�ļ���������
  WriteFile(fh, (LPSTR)lpbi, dwdibsize, &dwwritten, NULL);
  //��� 
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


///////////////////////////��깳��
//�ͼ����Ӳ���Ҫdll����ֻ�ܻ�õ�ǰ���г����ڵ��������ͼ��̵���Ϣ
//������Ҫ����ȫ���̵߳����ͼ�����Ϣ������Ҫ�����Ӻ�����װ�ڶ�̬���ӿ��ڣ�������
//
// ȫ�ֱ�����ȫ�ֺ�������
//
p_msfn g_msfn;
p_kbfn g_kbfn;
HHOOK hhookMs = NULL;
HHOOK hhookKb = NULL;
void UninstallKbHook();
void InstallKbHook(HINSTANCE hInst);
//
// ��װ���Hook
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
// ж������Hook
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
    //����ƶ�
    if (wParam == WM_MOUSEMOVE)
    {
      //sprintf(strMsg, "WM_MOUSEMOVE: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);
      //OutputDebugString(strMsg);
    }

    //������
    if (wParam == WM_LBUTTONDOWN)
    {
      //sprintf(strMsg, "WM_LBUTTONDOWN: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);
      //OutputDebugString(strMsg);
    }

    // 			//�����¼�
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