// DlgDI.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgDI.h"


// CDlgDI

IMPLEMENT_DYNCREATE(CDlgDI, CFormView)

CDlgDI::CDlgDI()
	: CFormView(IDD_DIALOG_DI)
{
  m_device = -1;
}

CDlgDI::~CDlgDI()
{
}

void CDlgDI::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgDI, CFormView)
END_MESSAGE_MAP()


// CDlgDI 诊断

#ifdef _DEBUG
void CDlgDI::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgDI::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgDI 消息处理程序
