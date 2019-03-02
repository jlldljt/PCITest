#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "TrendChart.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")

#include "StaticDraw.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

const TCHAR* AppName = TEXT("ClassTrend");


CArray<double, double> g_arrCpk;

int APIENTRY win32wnd(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc;
	HWND hwnd;

	//这里是在构建窗口类结构
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;//窗口回调函数指针
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance =  hInstance;//实例句柄
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);//默认图标
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//默认指针
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);//默认背景颜色
	wc.lpszMenuName = NULL;
	wc.lpszClassName = AppName;//窗口类名

	//注册窗口类
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("重复打开!"), TEXT("错误"), MB_ICONERROR);
		return 0;
	}

	//创建窗口
	int style = WS_OVERLAPPEDWINDOW;
	hwnd = CreateWindowEx(NULL, AppName, TEXT("CPK推移图"), style, 0, 0, 512, 384, 0, 0, hInstance, 0);
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("创建窗口失败!"), TEXT("错误"), MB_ICONERROR);
		return 0;
	}
	//无边框窗口
	SetWindowLong(hwnd, GWL_STYLE, WS_SYSMENU|WS_CAPTION | WS_OVERLAPPED /*| WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/);

	//显示、更新窗口
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{ 
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(wc.lpszClassName,wc.hInstance);

	return msg.wParam;
	//return 0;
}

CWinThread* g_create;

UINT CreateThread(LPVOID pParam)
{
	win32wnd(AfxGetInstanceHandle(), SW_SHOW);
	return 0;
}
CStaticDraw *pChart;
//这是窗口回调函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{		
	int xPos, yPos;
	switch (uMsg)
	{
	case WM_PAINT:
		//pChart->UpdateManual();
		break;
	case WM_DRAWITEM:
		//(DRAWITEMSTRUCT*)lParam;
		//SetBkColor(((DRAWITEMSTRUCT*)lParam)->hDC, RGB(0, 0, 255));
		pChart->DrawItem((DRAWITEMSTRUCT*)lParam);
		break;
	case WM_CREATE:
		//CreateWindow(_TEXT("Static"), _TEXT("static"), WS_VISIBLE | WS_CHILD | SS_RIGHT, 0, 0, 20, 20, hwnd, (HMENU)3, ((CREATESTRUCT*)lParam)->hInstance, NULL);
		//CreateWindow(_TEXT("Button"), _TEXT("按钮一"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		//  20, 20, 20, 20, hwnd, (HMENU)2, ((CREATESTRUCT*)lParam)->hInstance, NULL);
		//{
		//  HWND wnd = CreateWindow(_TEXT("Static"), _TEXT("按钮二"), WS_CHILD | BS_OWNERDRAW,
		//    40, 40, 20, 20, hwnd, (HMENU)4, ((CREATESTRUCT*)lParam)->hInstance, NULL);
		//  ShowWindow(wnd, TRUE);
		//}
		pChart = new CStaticDraw(CWnd::FromHandle(hwnd));
		{
			CREATESTRUCT*param = (CREATESTRUCT*)lParam;

			CRect rc;
			GetClientRect(hwnd,&rc);

			pChart->OnCreate((CREATESTRUCT*)lParam, rc/*CRect(10, 10, param->cx-20, param->cy-20)*/);// CreateWindowEx(_TEXT("Static"), _TEXT(""), WS_CHILD | BS_OWNERDRAW, 50, 50, 500, 500, hwnd, (HMENU)1, ((CREATESTRUCT*)(lParam))->hInstance, NULL);

			int count = g_arrCpk.GetCount();
			pChart->SetRange(10,3);
			for(int i = 0 ; i < count; i ++)
			{
				POINTF pt={i,g_arrCpk[i]};
				pChart->AddPoint(RGB(0, 255, i*20), 10,pt );
			}
			pChart->DrawPoint();
		}
		//pChart->Create(_T("chart chart"), WS_CHILD | WS_VISIBLE, CRect(1, 1, 400, 300), CWnd::FromHandle(hwnd), 0x1001);
		pChart->SetBG(RGB(255,255,255), RGB(100, 100, 200), 0, 3, 1);
		pChart->UpdateManual();
		//pChart->SetScrollPixel(100);
		//pChart->SetUpdateTime(100);
		break;
	case WM_CLOSE://询问窗口是否关闭
		break;
	case WM_DESTROY:
		delete pChart;

		PostQuitMessage(0);//退出消息循环，结束应用程序
		return 0;
		break;
	case WM_LBUTTONDOWN:
		//让无边框窗口能够拖动(在窗口客户区拖动)，下面两个任选其一
		//PostMessage(hwnd, WM_SYSCOMMAND, 61458, 0);
		//PostMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;
	case WM_MOUSEMOVE://鼠标移动
		xPos = GET_X_LPARAM(lParam);//鼠标位置X坐标
		yPos = GET_Y_LPARAM(lParam);//鼠标位置Y坐标
		//不要用LOWORD和HIWORD获取坐标，因为坐标有可能是负的
		break;
	case WM_TIMER:
		pChart->OnTimer((UINT_PTR)wParam);
		break;  
	case WM_SIZE:
		{
			int nX = LOWORD(lParam); // width of client area
			int nY = HIWORD(lParam); // height of client area
			//MoveWindow(pChart->m_pWnd->m_hWnd,0,0,nX,nY,TRUE);
			//pChart->SetRange(nX,nY);
			//pChart->DrawPoint();
			//pChart->UpdateManual();
		}
		break;	
	case WM_MOUSEWHEEL:
		{
			 //short fwKeys = LOWORD(wParam); // 
			 //short zDelta = HIWORD(wParam); // 
			 //xPos = LOWORD(lParam); // width of client area
			 //yPos = HIWORD(lParam); // height of client area
			 pChart->OnMouseWheel(LOWORD(wParam), HIWORD(wParam), CPoint(LOWORD(lParam), HIWORD(lParam)));

		}
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);//其他消息交给系统处理
}

CTrendChart::CTrendChart(CArray<double, double> &arrCpk)
{
	g_arrCpk.RemoveAll();
	g_arrCpk.Copy(arrCpk);
}


CTrendChart::~CTrendChart()
{
}

void CTrendChart::Show()
{

	g_create = AfxBeginThread(CreateThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	g_create->m_bAutoDelete = TRUE;
	g_create->ResumeThread();
}
