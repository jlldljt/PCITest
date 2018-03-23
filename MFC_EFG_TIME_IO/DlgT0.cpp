// DlgT0.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgT0.h"
#include "MainFrm.h"

// CDlgT0

IMPLEMENT_DYNCREATE(CDlgT0, CFormView)

CDlgT0::CDlgT0()
	: CFormView(IDD_DIALOG_T0)
{

}

CDlgT0::~CDlgT0()
{
}

void CDlgT0::InitDlg(void)
{
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"TimerPulse");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(0, TimerPulse);
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"OneShot");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(0, OneShot);
}

void CDlgT0::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
}

BEGIN_MESSAGE_MAP(CDlgT0, CFormView)
  ON_BN_CLICKED(IDC_BUTTON_CREATE, &CDlgT0::OnBnClickedButtonCreate)
  ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgT0 诊断

#ifdef _DEBUG
void CDlgT0::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgT0::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgT0 消息处理程序


void CDlgT0::OnBnClickedButtonCreate()
{
  // TODO: 在此添加控件通知处理程序代码
  int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
  if (index == CB_ERR)
    return;

  int data = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetItemData(index);

  CFrameWndEx *pMain = (CFrameWndEx *)AfxGetMainWnd();
  //((CMainFrame*)pMain)->m_timeIOCtrl
}


int CDlgT0::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFormView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码

  return 0;
}
