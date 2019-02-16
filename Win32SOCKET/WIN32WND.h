#pragma once
class CWIN32WND
{
public:
	CWIN32WND();
	~CWIN32WND();
};

#include <windows.h>

#define MAIN_CLASSNAME _T("KeyMessages")

ATOM WINAPI RegistMainWinClass(HINSTANCE hIns);
LRESULT CALLBACK WndMainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// �����ͼ��Ϣ
void WINAPI OnRedrawWindow(HDC hDC);

// �������ָ���ƶ���Ϣ
void WINAPI OnMouseMove(HWND hWnd, int x, int y, int nMark);

// ����������������Ϣ
void WINAPI OnMouseLButtonDown(HWND hWnd, int x, int y, int nMark);

// ����������̧����Ϣ
void WINAPI OnMouseLButtonUp(HWND hWnd, int x, int y, int nMark);

// ����������˫����Ϣ
void WINAPI OnMouseLButtonDoubleClick(HWND hWnd, int x, int y, int nMark);