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

  //�������ڹ���������ṹ
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;//���ڻص�����ָ��
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance =  hInstance;//ʵ�����
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);//Ĭ��ͼ��
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);//Ĭ��ָ��
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);//Ĭ�ϱ�����ɫ
  wc.lpszMenuName = NULL;
  wc.lpszClassName = AppName;//��������

  //ע�ᴰ����
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, TEXT("ע�ᴰ����ʧ��!"), TEXT("����"), MB_ICONERROR);
    return 0;
  }

  //��������
  int style = WS_OVERLAPPEDWINDOW;
  hwnd = CreateWindowEx(NULL, AppName, TEXT("���ڱ���"), style, 50, 50, 500, 500, 0, 0, hInstance, 0);
  if (hwnd == NULL)
  {
    MessageBox(NULL, TEXT("��������ʧ��!"), TEXT("����"), MB_ICONERROR);
    return 0;
  }
  //�ޱ߿򴰿�
  SetWindowLong(hwnd, GWL_STYLE, WS_SYSMENU|WS_CAPTION | WS_OVERLAPPED | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

  //��ʾ�����´���
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  //��Ϣѭ��
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

//���Ǵ��ڻص�����
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  int xPos, yPos;
  switch (uMsg)
  {
  case WM_CLOSE://ѯ�ʴ����Ƿ�ر�
    if (MessageBox(hwnd, TEXT("Ҫ�رմ�����"), TEXT("��ȷ��"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
    {
      return 0;//��ʾ�����˴���Ϣ��ϵͳ���ٴ���
    }
    break;
  case WM_DESTROY:
    PostQuitMessage(0);//�˳���Ϣѭ��������Ӧ�ó���
    return 0;
    break;
  case WM_LBUTTONDOWN:
    //���ޱ߿򴰿��ܹ��϶�(�ڴ��ڿͻ����϶�)������������ѡ��һ
    //PostMessage(hwnd, WM_SYSCOMMAND, 61458, 0);
    //PostMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    break;
  case WM_MOUSEMOVE://����ƶ�
    xPos = GET_X_LPARAM(lParam);//���λ��X����
    yPos = GET_Y_LPARAM(lParam);//���λ��Y����
    //��Ҫ��LOWORD��HIWORD��ȡ���꣬��Ϊ�����п����Ǹ���
    break;
  default:
    break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);//������Ϣ����ϵͳ����
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
