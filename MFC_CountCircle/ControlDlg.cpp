// ControlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_CountCircle.h"
#include "ControlDlg.h"


// CControlDlg

IMPLEMENT_DYNCREATE(CControlDlg, CFormView)

CControlDlg::CControlDlg()
	: CFormView(IDD_CONTROLDLG)
{

}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlDlg, CFormView)
END_MESSAGE_MAP()


// CControlDlg ���

#ifdef _DEBUG
void CControlDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlDlg ��Ϣ�������
