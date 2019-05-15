//空格 start ； esc stop 退出； tab 保存； enter 截图到clip


#include "stdafx.h"
#include "WIN32WND.h"
// 准备做截图
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
  Nothing = 0,	// 鼠标无状态
  BtnDown = 1,	// 鼠标键被按下
  BtnUp = 2,	// 鼠标键被释放
  BtnDBL = 3	// 鼠标键被双击
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

    //鼠标左击
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
        MessageBox(NULL, _T("save ok"), _T("提示"), MB_OK | MB_ICONINFORMATION);
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

	// 加上 CS_DBLCLKS 样式，窗体可以接收鼠标双击消息
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
	case WM_MOUSEMOVE: // 处理鼠标移动消息
					   // 可以看到，lParam的低4位(一个short int)为鼠标指针的x坐标，高4位为y坐标
					   // wParam为鼠标事件发生时的组合键状态
		OnMouseMove(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;
	case WM_LBUTTONDOWN: // 处理鼠标左键按下消息
		OnMouseLButtonDown(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;
	case WM_LBUTTONUP: // 处理鼠标左键抬起消息
		OnMouseLButtonUp(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;
	case WM_LBUTTONDBLCLK: // 处理鼠标左键双击消息
		OnMouseLButtonDoubleClick(hWnd, LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, _T("是否要退出应用程序？"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDYES)
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
  case WM_KEYDOWN:
      //if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
    {
      //判断是否按下键盘Enter键
      switch (wParam)
      {
      case VK_RETURN:
        //MessageBox(NULL, _T("enter"), _T("错误"), MB_OK | MB_ICONERROR);
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

// 标识鼠标状态的枚举量


// 保存鼠标位置的结构体, 有x, y两个分量域
static POINT g_pntMouse = { 0, 0 };

// 保存指针移动时, 组合键的标识, 
// 组合键有
//
// MK_CONTROL(Ctrl键) 
// MK_SHIFT(Shift键),
// MK_LBUTTON(鼠标左键),
// MB_RBUTTON(鼠标右键)
static int g_nMark = 0;

// 保存在操作鼠标按键时, 鼠标指针的位置
static POINT g_pntL = { 0, 0 };
static POINT g_pntLUp = { 0, 0 };


// 保存操作鼠标按键时, 组合键的标识
// 组合键有
//
// MK_CONTROL(Ctrl键)
// MK_SHIFT(Shift键)
static int g_nMarkL = 0;

// 保存鼠标操作状态
static BUTON_STATUS g_bsL = Nothing;

/**
* 判断字符串是否为空。
* 参数：lpszString指向要判断的字符串指针
* 返回：TRUE, 参数所指向的字符串为空；FALSE, 参数所指向的字符串不为空
*
*	__inline的作用, 在很多系统中, __inline可以作为声明“内联”函数使用
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
* 绘制窗体函数
* 参数：hDC, 绘图设备上下文句柄
*/
void WINAPI OnRedrawWindow(HDC hDC)
{
#if 1
	int nTop = TOP_SIDE;
	int nLen = 0;
	TCHAR szBuffer[BUFSIZ] = _T("");
	HFONT hOldFont = NULL;
	COLORREF cOldFont = 0;

	// 设置字体为系统默认字体
	hOldFont = (HFONT)SelectObject(hDC, GetStockObject(DEFAULT_GUI_FONT));

	// 设置字体颜色
	cOldFont = SetTextColor(hDC, RGB(220, 10, 10));

	// 输出鼠标指针移动位置
  if(g_bStart)
	nLen = _stprintf_s(szBuffer, BUFSIZ, _T("start"));
  else
	nLen = _stprintf_s(szBuffer, BUFSIZ, _T("stop "));
//nLen = _stprintf_s(szBuffer, BUFSIZ, _T("鼠标 X 坐标 %d；Y 坐标 %d。"), g_pntMouse.x, g_pntMouse.y);

	// 输出文字
	TextOut(
		hDC,	// 要输出文字的绘图设备上下文句柄
		LEFT_SIDE,	// 文字输出位置的x坐标
		nTop,		// 文字输出位置的y坐标 
		szBuffer,		// 指向要输出文字的指针
		nLen			// 要输出文字的长度
		);

	// 输出鼠标指针移动时, 组合键状态
	if (g_nMark != 0)
	{
		// 输出坐标下移 20 像素
		nTop += 20;
		_tcscpy_s(szBuffer, BUFSIZ, _T(""));

		// 注意, 得到的组合键状态是多个整数值'或'算的结果, 必须使用'与'运算来判断
		if (g_nMark & MK_CONTROL)      // Ctrl键是否按下
			_tcscat_s(szBuffer, BUFSIZ, _T("CTRL 键被按下"));

		if (g_nMark & MK_LBUTTON) 	// 鼠标左键是否按下
		{
			if (!IsStringEmpty(szBuffer))
				_tcscat_s(szBuffer, BUFSIZ, _T("；"));
			_tcscat_s(szBuffer, BUFSIZ, _T("鼠标 左 键被按下"));
		}

		if (g_nMark & MK_RBUTTON) 	// 鼠标右键是否按下
		{
			if (!IsStringEmpty(szBuffer))
				_tcscat_s(szBuffer, BUFSIZ, _T("；"));
			_tcscat_s(szBuffer, BUFSIZ, _T("鼠标 右 键被按下"));
		}

		if (g_nMark & MK_SHIFT)		// Shift键是否按下
		{
			if (!IsStringEmpty(szBuffer))
				_tcscat_s(szBuffer, BUFSIZ, _T("；"));
			_tcscat_s(szBuffer, BUFSIZ, _T("SHIFT 键被按下"));
		}
		_tcscat_s(szBuffer, BUFSIZ, _T("。"));

		// 设置字体颜色后输出字体
		SetTextColor(hDC, RGB(0, 200, 0));
		TextOut(hDC, LEFT_SIDE, nTop, szBuffer, _tcslen(szBuffer));
	}

	// g_bsL的值不为Nothing, 说明鼠标按键发生了动作, 进一步处理
	if (g_bsL > Nothing)
	{

		// 根据鼠标按键状态生成字符串, 注意三元运算符嵌套用法
		_stprintf_s(szBuffer, BUFSIZ,
			g_bsL == BtnDBL ? _T("LDBL (%d, %d)") :
			(g_bsL == BtnDown ? _T("LD (%d, %d)") : _T("LU (%d, %d)")),
			g_pntL.x, g_pntL.y
			);

		// 根据鼠标组合键状态生成字符串
		if (g_nMarkL & MK_CONTROL)
			_tcscat_s(szBuffer, BUFSIZ, _T(" CTRL"));

		if (g_nMarkL & MK_SHIFT)
			_tcscat_s(szBuffer, BUFSIZ, _T(" SHIFT"));

		// 设置字体颜色, 再次使用了嵌套的三元运算符
		SetTextColor(hDC, g_bsL == BtnDBL ? RGB(0, 200, 200) : (g_bsL == BtnDown ? RGB(0, 0, 200) : RGB(200, 200, 0)));

		// 输出字体
		TextOut(hDC, /*g_pntL.x*/LEFT_SIDE + 20, /*g_pntL.y*/nTop+40, szBuffer, _tcslen(szBuffer));

		// 重置状态
		//g_bsL = Nothing;


	}

  Rectangle(hDC, g_pntL.x, g_pntL.y, g_pntLUp.x, g_pntLUp.y);
    


	SelectObject(hDC, hOldFont);
	SetTextColor(hDC, cOldFont);
#endif

  /////////////////////////////////////////
 // HDC hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
  //SelectObject(hDC,&pen);
  int oldrop2 = SetROP2(g_hDskDC, R2_NOTXORPEN);//实现橡皮筋类的关键函数,同一个地方画两次相当于什么都没画
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
* 处理鼠标指针移动消息
* 参数：hWnd, 窗口句柄
*	     x, 鼠标指针位置x坐标
*	     y, 鼠标指针位置y坐标
*	     nMark, 组合键状态
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
* 处理鼠标左键按下消息
* 参数：hWnd, 窗口句柄
*	     x, 鼠标指针位置x坐标
*	     y, 鼠标指针位置y坐标
*	     nMark, 组合键状态
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
* 处理鼠标左键释放消息
* 参数：hWnd, 窗口句柄
*	     x, 鼠标指针位置x坐标
*	     y, 鼠标指针位置y坐标
*	     nMark, 组合键状态
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
* 处理鼠标左键双击消息
* 参数：hWnd, 窗口句柄
*	     x, 鼠标指针位置x坐标
*	     y, 鼠标指针位置y坐标
*	     nMark, 组合键状态
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
			_T("出现错误，无法注册窗口，错误代码：%d。"), GetLastError());
		MessageBox(NULL, szError, _T("错误"), MB_OK | MB_ICONERROR);
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
			_stprintf_s(szError, BUFSIZ, _T("出现错误，无法创建窗口，错误代码：%d。"), GetLastError());
			MessageBox(NULL, szError, _T("错误"), MB_OK | MB_ICONERROR);
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
