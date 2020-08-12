// Debug2.cpp : 实现文件
//

#include "stdafx.h"
#include "BICALDebug.h"
#include "Debug2.h"
#include "afxdialogex.h"


// CDebug2 对话框

IMPLEMENT_DYNAMIC(CDebug2, CDialogEx)

CDebug2::CDebug2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebug2::IDD, pParent)
{
	g_dlgDebug2=this;
}
CDebug2 dlgDebug2;
CDebug2 *g_dlgDebug2=NULL;

CDebug2::~CDebug2()
{
}

void CDebug2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDebug2, CDialogEx)
END_MESSAGE_MAP()


// CDebug2 消息处理程序
