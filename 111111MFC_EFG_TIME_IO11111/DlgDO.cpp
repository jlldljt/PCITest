// DlgDO.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgDO.h"


// CDlgDO

IMPLEMENT_DYNCREATE(CDlgDO, CFormView)

CDlgDO::CDlgDO()
	: CFormView(IDD_DIALOG_DO)
{
  m_device = -1;
}

CDlgDO::~CDlgDO()
{
}

void CDlgDO::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgDO, CFormView)
END_MESSAGE_MAP()


// CDlgDO 诊断

#ifdef _DEBUG
void CDlgDO::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgDO::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgDO 消息处理程序
