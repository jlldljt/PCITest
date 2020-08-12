
// BICALDebug.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Debug1.h"
#include "Debug2.h"

// CBICALDebugApp:
// 有关此类的实现，请参阅 BICALDebug.cpp
//

class CBICALDebugApp : public CWinApp
{
public:
	CBICALDebugApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CBICALDebugApp theApp;
extern CString g_csPath;
extern void Test(int num);
extern UINT ComMsg(LPVOID pParam);
extern CWinThread* gTrdCom;//com线程
extern RefreshTmp gstuRefresh;

extern CWinThread* gTrdRun;//
extern UINT RunThread(LPVOID pParam);//运行线程函数
extern unsigned int TestGrid(int cmd,int addr,unsigned int num);
extern bool update,runbool;
extern char g_chCmd[5000]; 