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
		MessageBox(NULL, TEXT("�ظ���!"), TEXT("����"), MB_ICONERROR);
		return 0;
	}

	//��������
	int style = WS_OVERLAPPEDWINDOW;
	hwnd = CreateWindowEx(NULL, AppName, TEXT("CPK����ͼ"), style, 0, 0, 512, 384, 0, 0, hInstance, 0);
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("��������ʧ��!"), TEXT("����"), MB_ICONERROR);
		return 0;
	}
	//�ޱ߿򴰿�
	SetWindowLong(hwnd, GWL_STYLE, WS_SYSMENU|WS_CAPTION | WS_OVERLAPPED /*| WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/);

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
//���Ǵ��ڻص�����
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
		//CreateWindow(_TEXT("Button"), _TEXT("��ťһ"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		//  20, 20, 20, 20, hwnd, (HMENU)2, ((CREATESTRUCT*)lParam)->hInstance, NULL);
		//{
		//  HWND wnd = CreateWindow(_TEXT("Static"), _TEXT("��ť��"), WS_CHILD | BS_OWNERDRAW,
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
	case WM_CLOSE://ѯ�ʴ����Ƿ�ر�
		break;
	case WM_DESTROY:
		delete pChart;

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
	return DefWindowProc(hwnd, uMsg, wParam, lParam);//������Ϣ����ϵͳ����
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
