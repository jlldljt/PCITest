// DlgT1.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgT1.h"


// CDlgT1

IMPLEMENT_DYNCREATE(CDlgT1, CFormView)

CDlgT1::CDlgT1()
	: CFormView(IDD_DIALOG_T1)
{

}

CDlgT1::~CDlgT1()
{
}

void CDlgT1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgT1, CFormView)
END_MESSAGE_MAP()


// CDlgT1 诊断

#ifdef _DEBUG
void CDlgT1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgT1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgT1 消息处理程序
