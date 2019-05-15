//�ո� start �� esc stop �˳��� tab ���棻 enter ��ͼ��clip


#include "stdafx.h"
#include "WIN32WND.h"
// ׼������ͼ
HWND g_hWnd = NULL;
HBITMAP g_hBm = NULL;
HDC g_hDskDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
RECT Rop2Rc = { 0 };
BOOL g_bStart = FALSE;
CWIN32WND::CWIN32WND()
{
}


CWIN32WND::~CWIN32WND()
{
}
#include "vcwnd.h"
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
typedef enum
{
  Nothing = 0,	// �����״̬
  BtnDown = 1,	// ����������
  BtnUp = 2,	// �������ͷ�
  BtnDBL = 3	// ������˫��
} BUTON_STATUS;
static BUTON_STATUS g_dpBsL = Nothing;
static POINT g_dpPntLDown = { 0, 0 };
static POINT g_dpPntLUp = { 0, 0 };

void ReFreshDesktopRect(RECT rect, BOOL bErase)
{
  HWND hWndDesk = GetDesktopWindow();
  HDC hDC = ::GetWindowDC(hWndDesk);
  FillRect(hDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
  if (bErase)
  {
    RedrawWindow(hWndDesk, &rect, 0, RDW_INVALIDATE | RDW_ERASE);// 1 4
  }
  else
  {
    RedrawWindow(hWndDesk, &rect, 0, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);// 1 4 80 =0x85);
  }
}

int MouseMsg(WPARAM wParam, POINT   pt)
{
  if (g_bStart)
  {
    if (wParam == WM_MOUSEMOVE)
    {
      //sprintf(strMsg, "WM_MOUSEMOVE: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);
      //OutputDebugString(strMsg);
      //int DrawMode = dc.GetROP2();

      if (g_dpBsL == BtnDown)
      {
        g_dpPntLUp.x = pt.x;
        g_dpPntLUp.y = pt.y;
        //ClientToScreen(hWnd, &g_dpPntLUp);
        RedrawWindow(g_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW /*| RDW_ERASE*/);
        //return TRUE;
       // RECT rect = { g_dpPntLDown.x ,g_dpPntLDown.y, g_dpPntLUp.x, g_dpPntLUp.y };
      }



    }

    //������
    if (wParam == WM_LBUTTONDOWN)
    {
      //sprintf(strMsg, "WM_LBUTTONDOWN: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);
      //OutputDebugString(strMsg);


      g_dpPntLDown.x = pt.x;
      g_dpPntLDown.y = pt.y;
      g_dpPntLUp.x = pt.x;
      g_dpPntLUp.y = pt.y;
      RedrawWindow(g_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
      g_dpBsL = BtnDown;
      return TRUE;
    }

    if (wParam == WM_LBUTTONUP)
    {
      ///ReFreshDesktopRect({ g_dpPntLDown.x ,g_dpPntLDown.y, g_dpPntLUp.x, g_dpPntLUp.y }, 1);
      //sprintf(strMsg, "WM_LBUTTONDOWN: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);
      //OutputDebugString(strMsg);
      g_dpPntLUp.x = pt.x;
      g_dpPntLUp.y = pt.y;
      ///ReFreshDesktopRect({ g_dpPntLDown.x ,g_dpPntLDown.y, g_dpPntLUp.x, g_dpPntLUp.y }, 1);
      RedrawWindow(g_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
      g_dpBsL = BtnUp;
      return TRUE;
    }
  }
  return FALSE;
}
TCHAR g_path[MAX_PATH] = { '\0' };
int KeyBoardMsg(DWORD vkCode, BOOL ctrl, BOOL alt)
{
  switch (vkCode)
  {
  case VK_ESCAPE:
    if (!g_bStart)
    {
      exit(0);
    }
    else
    {
      g_bStart = FALSE;
      g_dpPntLDown.x = 0;
      g_dpPntLDown.y = 0;
      g_dpPntLUp.x = 0;
      g_dpPntLUp.y = 0;
      RedrawWindow(g_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW /*| RDW_ERASE*/);
    }
    return TRUE;
  case VK_TAB:
    if (!g_bStart)
    {
      GetPathFromBrowse(g_path);
      if (g_path!=L"") {
        wcscat_s(g_path, L".bmp");
		RECT rc = { g_dpPntLDown.x ,g_dpPntLDown.y, g_dpPntLUp.x, g_dpPntLUp.y };
        savebmp(rc, g_path);
        MessageBox(NULL, _T("save ok"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
      }
    }
    return TRUE;
  case VK_SPACE:
    g_bStart = TRUE;
    RedrawWindow(g_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW /*| RDW_ERASE*/);
    return TRUE;
  case VK_RETURN:
	  if (g_bStart)
	  {
      
		  RECT rc = { min(g_dpPntLDown.x,g_dpPntLUp.x)+1 ,
        min(g_dpPntLDown.y, g_dpPntLUp.y)+1, 
        max(g_dpPntLDown.x,g_dpPntLUp.x)-1, 
        max(g_dpPntLDown.y, g_dpPntLUp.y)-1 };
 
		  testbmp(rc);
	  }
    return TRUE;
  default:
    break;
  } 
  return FALSE;
}

ATOM WINAPI RegistMainWinClass(HINSTANCE hIns)
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(wcex);

	// ���� CS_DBLCLKS ��ʽ��������Խ������˫����Ϣ
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndMainProc;
	wcex.hInstance = hIns;
	wcex.hIcon = LoadIcon(hIns, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName = MAIN_CLASSNAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	return RegisterClassEx(&wcex);
}


LRESULT CALLBACK WndMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC = NULL;
	PAINTSTRUCT ps;

	LRESULT lReturn = 0L;
	switch (message) {
	case WM_CREATE:
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
    
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		OnRedrawWindow(hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE: // ��������ƶ���Ϣ
					   // ���Կ�����lParam�ĵ�4λ(һ��short int)Ϊ���ָ���x���꣬��4λΪy����
					   // wParamΪ����¼�����ʱ����ϼ�״̬
		OnMouseMove(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;
	case WM_LBUTTONDOWN: // ����������������Ϣ
		OnMouseLButtonDown(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;
	case WM_LBUTTONUP: // ����������̧����Ϣ
		OnMouseLButtonUp(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;
	case WM_LBUTTONDBLCLK: // ����������˫����Ϣ
		OnMouseLButtonDoubleClick(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, _T("�Ƿ�Ҫ�˳�Ӧ�ó���"), _T("��ʾ"), MB_YESNO | MB_ICONQUESTION) == IDYES)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
  case WM_KEYDOWN:
      //if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
    {
      //�ж��Ƿ��¼���Enter��
      switch (wParam)
      {
      case VK_RETURN:
        //MessageBox(NULL, _T("enter"), _T("����"), MB_OK | MB_ICONERROR);
        break;
      case VK_SPACE:
        //testbmp({ g_dpPntLDown.x ,g_dpPntLDown.y, g_dpPntLUp.x, g_dpPntLUp.y });

        break;
      case VK_ESCAPE:
        //exit(0);
        break;
        /*case 'X':
          if(GetKeyState(VK_CONTROL)<0 )
            g_dlgRun->OnBnClickedBtnStandard();*/
      default:
        break;
      }
    }
    break;
	default:
		lReturn = DefWindowProc(hWnd, message, wParam, lParam);
	}
	return lReturn;
}


#define LEFT_SIDE 20
#define TOP_SIDE 10

// ��ʶ���״̬��ö����


// �������λ�õĽṹ��, ��x, y����������
static POINT g_pntMouse = { 0, 0 };

// ����ָ���ƶ�ʱ, ��ϼ��ı�ʶ, 
// ��ϼ���
//
// MK_CONTROL(Ctrl��) 
// MK_SHIFT(Shift��),
// MK_LBUTTON(������),
// MB_RBUTTON(����Ҽ�)
static int g_nMark = 0;

// �����ڲ�����갴��ʱ, ���ָ���λ��
static POINT g_pntL = { 0, 0 };
static POINT g_pntLUp = { 0, 0 };


// ���������갴��ʱ, ��ϼ��ı�ʶ
// ��ϼ���
//
// MK_CONTROL(Ctrl��)
// MK_SHIFT(Shift��)
static int g_nMarkL = 0;

// ����������״̬
static BUTON_STATUS g_bsL = Nothing;

/**
* �ж��ַ����Ƿ�Ϊ�ա�
* ������lpszStringָ��Ҫ�жϵ��ַ���ָ��
* ���أ�TRUE, ������ָ����ַ���Ϊ�գ�FALSE, ������ָ����ַ�����Ϊ��
*
*	__inline������, �ںܶ�ϵͳ��, __inline������Ϊ����������������ʹ��
*/
__inline BOOL IsStringEmpty(CONST LPCTSTR lpszString)
{
	return lpszString[0] == _T('\0');
}
void WINAPI OnRedrawDesk(HDC hDC)
{
  HDC hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);


}
/**
* ���ƴ��庯��
* ������hDC, ��ͼ�豸�����ľ��
*/
void WINAPI OnRedrawWindow(HDC hDC)
{
#if 1
	int nTop = TOP_SIDE;
	int nLen = 0;
	TCHAR szBuffer[BUFSIZ] = _T("");
	HFONT hOldFont = NULL;
	COLORREF cOldFont = 0;

	// ��������ΪϵͳĬ������
	hOldFont = (HFONT)SelectObject(hDC, GetStockObject(DEFAULT_GUI_FONT));

	// ����������ɫ
	cOldFont = SetTextColor(hDC, RGB(220, 10, 10));

	// ������ָ���ƶ�λ��
  if(g_bStart)
	nLen = _stprintf_s(szBuffer, BUFSIZ, _T("start"));
  else
	nLen = _stprintf_s(szBuffer, BUFSIZ, _T("stop "));
//nLen = _stprintf_s(szBuffer, BUFSIZ, _T("��� X ���� %d��Y ���� %d��"), g_pntMouse.x, g_pntMouse.y);

	// �������
	TextOut(
		hDC,	// Ҫ������ֵĻ�ͼ�豸�����ľ��
		LEFT_SIDE,	// �������λ�õ�x����
		nTop,		// �������λ�õ�y���� 
		szBuffer,		// ָ��Ҫ������ֵ�ָ��
		nLen			// Ҫ������ֵĳ���
		);

	// ������ָ���ƶ�ʱ, ��ϼ�״̬
	if (g_nMark != 0)
	{
		// ����������� 20 ����
		nTop += 20;
		_tcscpy_s(szBuffer, BUFSIZ, _T(""));

		// ע��, �õ�����ϼ�״̬�Ƕ������ֵ'��'��Ľ��, ����ʹ��'��'�������ж�
		if (g_nMark & MK_CONTROL)      // Ctrl���Ƿ���
			_tcscat_s(szBuffer, BUFSIZ, _T("CTRL ��������"));

		if (g_nMark & MK_LBUTTON) 	// �������Ƿ���
		{
			if (!IsStringEmpty(szBuffer))
				_tcscat_s(szBuffer, BUFSIZ, _T("��"));
			_tcscat_s(szBuffer, BUFSIZ, _T("��� �� ��������"));
		}

		if (g_nMark & MK_RBUTTON) 	// ����Ҽ��Ƿ���
		{
			if (!IsStringEmpty(szBuffer))
				_tcscat_s(szBuffer, BUFSIZ, _T("��"));
			_tcscat_s(szBuffer, BUFSIZ, _T("��� �� ��������"));
		}

		if (g_nMark & MK_SHIFT)		// Shift���Ƿ���
		{
			if (!IsStringEmpty(szBuffer))
				_tcscat_s(szBuffer, BUFSIZ, _T("��"));
			_tcscat_s(szBuffer, BUFSIZ, _T("SHIFT ��������"));
		}
		_tcscat_s(szBuffer, BUFSIZ, _T("��"));

		// ����������ɫ���������
		SetTextColor(hDC, RGB(0, 200, 0));
		TextOut(hDC, LEFT_SIDE, nTop, szBuffer, _tcslen(szBuffer));
	}

	// g_bsL��ֵ��ΪNothing, ˵����갴�������˶���, ��һ������
	if (g_bsL > Nothing)
	{

		// ������갴��״̬�����ַ���, ע����Ԫ�����Ƕ���÷�
		_stprintf_s(szBuffer, BUFSIZ,
			g_bsL == BtnDBL ? _T("LDBL (%d, %d)") :
			(g_bsL == BtnDown ? _T("LD (%d, %d)") : _T("LU (%d, %d)")),
			g_pntL.x, g_pntL.y
			);

		// ���������ϼ�״̬�����ַ���
		if (g_nMarkL & MK_CONTROL)
			_tcscat_s(szBuffer, BUFSIZ, _T(" CTRL"));

		if (g_nMarkL & MK_SHIFT)
			_tcscat_s(szBuffer, BUFSIZ, _T(" SHIFT"));

		// ����������ɫ, �ٴ�ʹ����Ƕ�׵���Ԫ�����
		SetTextColor(hDC, g_bsL == BtnDBL ? RGB(0, 200, 200) : (g_bsL == BtnDown ? RGB(0, 0, 200) : RGB(200, 200, 0)));

		// �������
		TextOut(hDC, /*g_pntL.x*/LEFT_SIDE + 20, /*g_pntL.y*/nTop+40, szBuffer, _tcslen(szBuffer));

		// ����״̬
		//g_bsL = Nothing;


	}

  Rectangle(hDC, g_pntL.x, g_pntL.y, g_pntLUp.x, g_pntLUp.y);
    


	SelectObject(hDC, hOldFont);
	SetTextColor(hDC, cOldFont);
#endif

  /////////////////////////////////////////
 // HDC hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
  //SelectObject(hDC,&pen);
  int oldrop2 = SetROP2(g_hDskDC, R2_NOTXORPEN);//ʵ����Ƥ����Ĺؼ�����,ͬһ���ط��������൱��ʲô��û��
  Rectangle(g_hDskDC, Rop2Rc.left, Rop2Rc.top, Rop2Rc.right, Rop2Rc.bottom);

  //HBRUSH oldbr = (HBRUSH)SelectObject(g_hDskDC,HBRUSH(GetStockObject(HOLLOW_BRUSH)));
  Rectangle(g_hDskDC, g_dpPntLDown.x, g_dpPntLDown.y, g_dpPntLUp.x, g_dpPntLUp.y);
//  Rop2Rc = { g_dpPntLDown.x, g_dpPntLDown.y, g_dpPntLUp.x, g_dpPntLUp.y };
  Rop2Rc.left = g_dpPntLDown.x;
  Rop2Rc.top = g_dpPntLDown.y;
  Rop2Rc.right = g_dpPntLUp.x;
  Rop2Rc.bottom = g_dpPntLUp.y;
  //SelectObject(g_hDskDC, oldbr);

  SetROP2(g_hDskDC, oldrop2);

}

/**
* �������ָ���ƶ���Ϣ
* ������hWnd, ���ھ��
*	     x, ���ָ��λ��x����
*	     y, ���ָ��λ��y����
*	     nMark, ��ϼ�״̬
*/
void WINAPI OnMouseMove(HWND hWnd, int x, int y, int nMark)
{
	g_pntMouse.x = x;
	g_pntMouse.y = y;
  if (g_bsL == BtnDown)
  {
    g_pntLUp.x = x;
    g_pntLUp.y = y;

    //g_dpPntLUp.x = x;
    //g_dpPntLUp.y = y;
    //ClientToScreen(hWnd, &g_dpPntLUp);
  }
	g_nMark = nMark;
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

/**
* ����������������Ϣ
* ������hWnd, ���ھ��
*	     x, ���ָ��λ��x����
*	     y, ���ָ��λ��y����
*	     nMark, ��ϼ�״̬
*/
void WINAPI OnMouseLButtonDown(HWND hWnd, int x, int y, int nMark)
{
	g_pntL.x = x;
	g_pntL.y = y;
  g_pntLUp.x = x;
  g_pntLUp.y = y;

  //g_dpPntLDown.x = x;
  //g_dpPntLDown.y = y;
  //g_dpPntLUp.x = x;
  //g_dpPntLUp.y = y;
  //ClientToScreen(hWnd,&g_dpPntLDown);
  //ClientToScreen(hWnd,&g_dpPntLUp);

	g_nMarkL = nMark;
	g_bsL = BtnDown;
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

/**
* �����������ͷ���Ϣ
* ������hWnd, ���ھ��
*	     x, ���ָ��λ��x����
*	     y, ���ָ��λ��y����
*	     nMark, ��ϼ�״̬
*/
void WINAPI OnMouseLButtonUp(HWND hWnd, int x, int y, int nMark)
{
  g_pntLUp.x = x;
  g_pntLUp.y = y;
  //g_dpPntLUp.x = x;
  //g_dpPntLUp.y = y;
  //ClientToScreen(hWnd, &g_dpPntLUp);

	g_nMarkL = nMark;
	g_bsL = BtnUp;
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

/**
* ����������˫����Ϣ
* ������hWnd, ���ھ��
*	     x, ���ָ��λ��x����
*	     y, ���ָ��λ��y����
*	     nMark, ��ϼ�״̬
*/
void WINAPI OnMouseLButtonDoubleClick(HWND hWnd, int x, int y, int nMark)
{
	g_pntL.x = x;
	g_pntL.y = y;
	g_nMarkL = nMark;
	g_bsL = BtnDBL;
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	TCHAR szError[BUFSIZ] = _T("");
	HWND hWnd = NULL;
	MSG msg;

	ZeroMemory(&msg, sizeof(msg));

	if (RegistMainWinClass(hInstance) == 0)
	{
		_stprintf_s(szError, BUFSIZ,
			_T("���ִ����޷�ע�ᴰ�ڣ�������룺%d��"), GetLastError());
		MessageBox(NULL, szError, _T("����"), MB_OK | MB_ICONERROR);
	}
	else
	{
    StartHook(hInstance, MouseMsg, KeyBoardMsg);
    /*CreateWindowExW(
      _In_ DWORD dwExStyle,
      _In_opt_ LPCWSTR lpClassName,
      _In_opt_ LPCWSTR lpWindowName,
      _In_ DWORD dwStyle,
      _In_ int X,
      _In_ int Y,
      _In_ int nWidth,
      _In_ int nHeight,
      _In_opt_ HWND hWndParent,
      _In_opt_ HMENU hMenu,
      _In_opt_ HINSTANCE hInstance,
      _In_opt_ LPVOID lpParam);*/
#define WS_MYSTYLE (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME)

		hWnd = CreateWindowEx(0, MAIN_CLASSNAME,
			_T("Hello World"), WS_MYSTYLE/*WS_OVERLAPPEDWINDOW*/, /*CW_USEDEFAULT,
			0, CW_USEDEFAULT, 0,*/0,0,50,100, NULL, NULL, hInstance, NULL
			);

		if (hWnd == NULL)
		{
			_stprintf_s(szError, BUFSIZ, _T("���ִ����޷��������ڣ�������룺%d��"), GetLastError());
			MessageBox(NULL, szError, _T("����"), MB_OK | MB_ICONERROR);
		}
		else
		{
      g_hWnd = hWnd;
			ShowWindow(hWnd, nCmdShow);
			UpdateWindow(hWnd);

			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
  EndHook();
	return msg.wParam;
}
