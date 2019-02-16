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


// 处理绘图消息
void WINAPI OnRedrawWindow(HDC hDC);

// 处理鼠标指针移动消息
void WINAPI OnMouseMove(HWND hWnd, int x, int y, int nMark);

// 处理鼠标左键按下消息
void WINAPI OnMouseLButtonDown(HWND hWnd, int x, int y, int nMark);

// 处理鼠标左键抬起消息
void WINAPI OnMouseLButtonUp(HWND hWnd, int x, int y, int nMark);

// 处理鼠标左键双击消息
void WINAPI OnMouseLButtonDoubleClick(HWND hWnd, int x, int y, int nMark);