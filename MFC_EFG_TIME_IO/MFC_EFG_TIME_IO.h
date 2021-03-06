// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MFC_EFG_TIME_IO.h : MFC_EFG_TIME_IO 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

//#include "TimeIOCtrl.h"
#include "CPCICtrl.h"
#include "ParamConfig.h"


// CMFC_EFG_TIME_IOApp:
// 有关此类的实现，请参阅 MFC_EFG_TIME_IO.cpp
//
//#include <gdiplus.h>
//using namespace Gdiplus;
//#pragma comment(lib, "gdiplus.lib")

class CMFC_EFG_TIME_IOApp : public CWinAppEx
{
public:
	CMFC_EFG_TIME_IOApp();
  //ULONG_PTR m_gdiplusToken;
  //GdiplusStartupInput m_gdiplusStartupInput;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFC_EFG_TIME_IOApp theApp;

void ScaleBitmap(CBitmap *pBitmap, CBitmap &BitmapNew, int nWidth, int nHeight);