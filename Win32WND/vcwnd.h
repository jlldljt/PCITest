#pragma once
#include<Windows.h>
typedef HWND t_wnd;
class vcwnd
{
public:
	vcwnd();
	~vcwnd();
public:
	struct elm
	{
		int id;
		void* super;
		void* child;
	};
//bsp
private:
	struct canvas//所有操作在此画布上绘制
	{

	};
public:
	elm* createmainwnd(void)
	{
		
	}
	elm* createchildwnd(elm* super);

private:
	void drawpic(int pictype, elm element);

};

void CopyBitmapToHdc(HDC hdc, RECT rect, HBITMAP bm);
HBITMAP CopyScreenToBitmap(LPRECT lpRect);
void testbmp(RECT rect);
void savebmp(RECT rect, LPTSTR lpFileName);
//////////////////////

typedef int(*p_msfn)(WPARAM wParam, POINT   pt);
typedef int(*p_kbfn)(DWORD vkCode, BOOL ctrl, BOOL alt);

LRESULT CALLBACK LowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam);

void StartHook(HINSTANCE hInst, p_msfn msfn, p_kbfn kbfn);
void EndHook();
void GetPathFromBrowse(TCHAR *path);