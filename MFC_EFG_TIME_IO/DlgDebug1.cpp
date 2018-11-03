// DlgDebug1.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgDebug1.h"
#include "MainFrm.h"


// CDlgDebug1

IMPLEMENT_DYNCREATE(CDlgDebug1, CFormView)

CDlgDebug1::CDlgDebug1()
	: CFormView(CDlgDebug1::IDD)
{
  m_dlg_debug_device = NULL;
  m_dlg_laser = NULL;
}

CDlgDebug1::~CDlgDebug1()
{
}

void CDlgDebug1::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TAB_DEBUG, m_tab_debug);
}

BEGIN_MESSAGE_MAP(CDlgDebug1, CFormView)
  ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DEBUG, &CDlgDebug1::OnTcnSelchangeTabDebug)
  ON_WM_CREATE()
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgDebug1 诊断

#ifdef _DEBUG
void CDlgDebug1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgDebug1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgDebug1 消息处理程序


void CDlgDebug1::OnTcnSelchangeTabDebug(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: 在此添加控件通知处理程序代码
  CRect tabRect;
  m_tab_debug.GetClientRect(&tabRect);
  tabRect.left += 1;
  tabRect.right -= 1;
  tabRect.top += 22;
  tabRect.bottom -= 1;
  switch (m_tab_debug.GetCurSel())
  {
  case 0:
    m_dlg_debug_device->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
    m_dlg_laser->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    
    break;
  case 1:
    m_dlg_debug_device->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_laser->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
    
    break;
  default:
    m_dlg_debug_device->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_laser->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    break;
  }
  *pResult = 0;
}


int CDlgDebug1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFormView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码
  return 0;


}


void CDlgDebug1::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
  // TODO: 在此添加专用代码和/或调用基类

  CFormView::OnActivateFrame(nState, pDeactivateFrame);
}


void CDlgDebug1::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  // TODO: 在此添加专用代码和/或调用基类
  
  //////////////tab
  CRect dbgRect;   // 标签控件客户区的位置和大小  
  //tab控件初始化
  m_tab_debug.InsertItem(0, _T("机构调试"));
  m_tab_debug.InsertItem(1, _T("其他调试"));
  m_dlg_debug_device = new CDlgDebugDevice;
  m_dlg_debug_device->Create(IDD_DIALOG_DEVICE, &m_tab_debug);
  m_dlg_laser = new CDlgLaser;
  m_dlg_laser->Create(IDD_DLG_LASER, &m_tab_debug);
  m_tab_debug.GetClientRect(&dbgRect);//tab控件大小
  dbgRect.left += 1;
  dbgRect.right -= 1;
  dbgRect.top += 22;
  dbgRect.bottom -= 1;
  m_dlg_debug_device->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(), SWP_SHOWWINDOW);
  m_dlg_laser->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(), SWP_HIDEWINDOW);
  m_tab_debug.SetCurSel(0);
}


void CDlgDebug1::PreSubclassWindow()
{
  // TODO: 在此添加专用代码和/或调用基类

  CFormView::PreSubclassWindow();
}


HBRUSH CDlgDebug1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  在此更改 DC 的任何特性

  // TODO:  如果默认的不是所需画笔，则返回另一个画笔
  return hbr;
}
