#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include "TrendChart.h"
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

const TCHAR* AppName = TEXT("ClassTrend");

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
    MessageBox(NULL, TEXT("注册窗口类失败!"), TEXT("错误"), MB_ICONERROR);
    return 0;
  }

  //创建窗口
  int style = WS_OVERLAPPEDWINDOW;
  hwnd = CreateWindowEx(NULL, AppName, TEXT("窗口标题"), style, 50, 50, 500, 500, 0, 0, hInstance, 0);
  if (hwnd == NULL)
  {
    MessageBox(NULL, TEXT("创建窗口失败!"), TEXT("错误"), MB_ICONERROR);
    return 0;
  }
  //无边框窗口
  SetWindowLong(hwnd, GWL_STYLE, WS_SYSMENU|WS_CAPTION | WS_OVERLAPPED | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

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
  return msg.wParam;
  return 0;
}

CWinThread* g_create;

UINT CreateThread(LPVOID pParam)
{
  win32wnd(AfxGetInstanceHandle(), SW_SHOW);
  return 0;
}

//这是窗口回调函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int xPos, yPos;
  switch (uMsg)
  {
  case WM_CLOSE://询问窗口是否关闭
    if (MessageBox(hwnd, TEXT("要关闭窗口吗？"), TEXT("请确认"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
    {
      return 0;//表示处理了此消息，系统不再处理
    }
    break;
  case WM_DESTROY:
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
  default:
    break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);//其他消息交给系统处理
}

CTrendChart::CTrendChart()
{
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
