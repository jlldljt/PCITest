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
	struct canvas//���в����ڴ˻����ϻ���
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