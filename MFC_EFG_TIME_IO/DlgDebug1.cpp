// DlgDebug1.cpp : ʵ���ļ�
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
  m_dlg_xray = NULL;
  m_dlg_param = NULL;
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


// CDlgDebug1 ���

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


// CDlgDebug1 ��Ϣ��������


void CDlgDebug1::OnTcnSelchangeTabDebug(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: �ڴ����ӿؼ�֪ͨ�����������
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
    m_dlg_xray->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_param->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

    break;
  case 1:
    m_dlg_debug_device->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_laser->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
    m_dlg_xray->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_param->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

    break;
  case 2:
    m_dlg_debug_device->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_laser->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_xray->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
    m_dlg_param->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);

    break;
  case 3:
    m_dlg_debug_device->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_laser->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_xray->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_param->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);

    break;
  default:
    m_dlg_debug_device->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_laser->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_xray->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    m_dlg_param->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
    break;
  }
  *pResult = 0;
}


int CDlgDebug1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFormView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  �ڴ�������ר�õĴ�������
  return 0;


}


void CDlgDebug1::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
  // TODO: �ڴ�����ר�ô����/����û���

  CFormView::OnActivateFrame(nState, pDeactivateFrame);
}


void CDlgDebug1::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  // TODO: �ڴ�����ר�ô����/����û���
  CBitmap bmp, sbmp;
  HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, _T("BK1.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
  if (bitmap)
    bmp.Attach(bitmap);
  else
    bmp.LoadBitmap(IDB_BK1);   //IDB_BITMAP1��ͼƬ��ԴID
  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);
  bmp.DeleteObject();

  //////////////tab
  CRect dbgRect;   // ��ǩ�ؼ��ͻ�����λ�úʹ�С  
  //tab�ؼ���ʼ��
  m_tab_debug.InsertItem(0, _T("��������"));
  m_tab_debug.InsertItem(1, _T("�������"));
  m_tab_debug.InsertItem(2, _T("X�����"));
  m_tab_debug.InsertItem(3, _T("��������"));
  m_dlg_debug_device = new CDlgDebugDevice;
  m_dlg_debug_device->Create(IDD_DIALOG_DEVICE, &m_tab_debug);
  m_dlg_laser = new CDlgLaser;
  m_dlg_laser->Create(IDD_DLG_LASER, &m_tab_debug);
  m_dlg_xray = new CDlgXray;
  m_dlg_xray->Create(IDD_DLG_XRAY, &m_tab_debug);
  m_dlg_param = new CDlgParam;
  m_dlg_param->Create(IDD_DLG_PARAM, &m_tab_debug);
  m_tab_debug.GetClientRect(&dbgRect);//tab�ؼ���С
  dbgRect.left += 1;
  dbgRect.right -= 1;
  dbgRect.top += 22;
  dbgRect.bottom -= 1;
  m_dlg_debug_device->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(), SWP_SHOWWINDOW);
  m_dlg_laser->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(), SWP_HIDEWINDOW);
  m_dlg_xray->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(), SWP_HIDEWINDOW);
  m_dlg_param->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(), SWP_HIDEWINDOW);
  m_tab_debug.SetCurSel(0);
}


void CDlgDebug1::PreSubclassWindow()
{
  // TODO: �ڴ�����ר�ô����/����û���

  CFormView::PreSubclassWindow();
}


HBRUSH CDlgDebug1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  �ڴ˸��� DC ���κ�����
  if (pWnd == this) {
    return m_brush;
  }
  else if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN  /*|| nCtlColor == CTLCOLOR_MAX*/)
  {
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));
    if (pWnd->GetDlgCtrlID() == IDC_STATIC)
    return (HBRUSH)::GetStockObject(NULL_BRUSH);
  }
  // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
  return hbr;
}