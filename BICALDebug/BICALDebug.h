
// BICALDebug.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Debug1.h"
#include "Debug2.h"

// CBICALDebugApp:
// �йش����ʵ�֣������ BICALDebug.cpp
//

class CBICALDebugApp : public CWinApp
{
public:
	CBICALDebugApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBICALDebugApp theApp;
extern CString g_csPath;
extern void Test(int num);
extern UINT ComMsg(LPVOID pParam);
extern CWinThread* gTrdCom;//com�߳�
extern RefreshTmp gstuRefresh;

extern CWinThread* gTrdRun;//
extern UINT RunThread(LPVOID pParam);//�����̺߳���
extern unsigned int TestGrid(int cmd,int addr,unsigned int num);
extern bool update,runbool;
extern char g_chCmd[5000]; 