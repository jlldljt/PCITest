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


void testbmp();

//////////////////////

typedef int(*p_fn)(WPARAM wParam, POINT   pt);

LRESULT CALLBACK LowLevelMouseProc(INT nCode, WPARAM wParam, LPARAM lParam);

void StartHook(HINSTANCE hInst, p_fn fn);
void EndHook();