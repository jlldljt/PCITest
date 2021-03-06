// DlgDI.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgDI.h"
#include "MainFrm.h"


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

void CDlgDI::InitDlg(int index)
{
  m_index = index;
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"StaticDi");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(0, STATIC_DI);
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"InterruptDi");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(1, INTERRUPT_DI);

  GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(FALSE);
  GetDlgItem(IDC_EDIT_PARAM1)->EnableWindow(FALSE);
  GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}

void CDlgDI::SetDlg(TimeIOType type)
{
  switch (type) {
  case STATIC_DI:
    SetDlgItemText(IDC_STATIC_PARAM0, L"电平");
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
    SetTimer(0, 200, NULL);
    break;
  case INTERRUPT_DI:
    SetDlgItemText(IDC_STATIC_PARAM0, L"中断cnt");
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_PARAM1)->EnableWindow(TRUE);
    break;
  default:
    SetDlgItemText(IDC_STATIC_PARAM0, L"param0");
    SetDlgItemText(IDC_STATIC_PARAM1, L"param1");
    GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_PARAM1)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
    SetDlgItemText(IDC_EDIT_PARAM0, L"");
    SetDlgItemText(IDC_EDIT_PARAM1, L"");
    KillTimer(0);
    break;
  }

}

#include "DlgT0.h"
#include "DlgT1.h"
//void CDlgDI::UserFunc(void *param)
//{
//  GetMainFrame()->DIIntCB();
//}
void CDlgDI::SaveParam()
{
  CString str;
  str.Format(L"DI%d", m_index);
  CString path = L"config/" + m_devName + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == m_config.Create(ppath, pstr))
  {
    ConfigParam param;

    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
	int data = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetItemData(index);
    CString param0, param1;
    GetDlgItemText(IDC_EDIT_PARAM0, param0);
    GetDlgItemText(IDC_EDIT_PARAM1, param1);
    double fparam0 = _wtof(param0);
    double fparam1 = _wtof(param1);
    bool start;
    CString str;
    GetDlgItemText(IDC_BUTTON_START, str);
    if ("开始" == str)
      start = false;
    else
      start = true;
    param.device = m_device;
    param.comboData = data;
    param.param0 = fparam0;
    param.param1 = fparam1;
    param.start = start;
    m_config.SaveParam(param);
  }
}
void CDlgDI::LoadParam()
{
  CString str;
  str.Format(L"DI%d", m_index);
  CString path = L"config/" + m_devName + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == m_config.Create(ppath, pstr))
  {
    ConfigParam* param = m_config.ReadParam();
    if (!param)
      return;

	int count = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCount();
	int i;
	for (i = 0; i< count;i++){
	  int data = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetItemData(i);
	  if(data == param->comboData)
		break;
	}
	if(i >= count)
		return;

    ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetCurSel(i);
    CString param0, param1;
    param0.Format(L"%lf", param->param0);
    param1.Format(L"%lf", param->param1);
    SetDlgItemText(IDC_EDIT_PARAM0, param0);
    SetDlgItemText(IDC_EDIT_PARAM1, param1);
    if (param->start && param->device == m_device) //启动状态
    {
      CString str;
      GetDlgItemText(IDC_BUTTON_CREATE, str);
      if ("创建" == str) {
        OnBnClickedButtonCreate();
        OnBnClickedButtonStart();
      }
    }
    delete param;
  }
}
void CDlgDI::Stop(void)
{
  CString str;
  GetDlgItemText(IDC_BUTTON_START, str);
  if ("开始" != str) {
    if (GetMainFrame()->m_timeIOCtrl->StopDi(m_index)) {
      str = "开始";
      SetDlgItemText(IDC_BUTTON_START, str);
      m_brushBack.DeleteObject();
      m_color = RGB(240, 240, 240);
      m_brushBack.CreateSolidBrush(m_color);

      GetDlgItem(IDC_BUTTON_CREATE)->EnableWindow(TRUE);
    }

  }
  GetDlgItemText(IDC_BUTTON_CREATE, str);
  if ("创建" == str) {
    return;
  }
  GetMainFrame()->m_timeIOCtrl->DeleteDi(m_index);
  SetDlg((TimeIOType)-1);
  str = "创建";
  SetDlgItemText(IDC_BUTTON_CREATE, str);


  Invalidate();
}
void CDlgDI::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgDI, CFormView)
  ON_BN_CLICKED(IDC_BUTTON_CREATE, &CDlgDI::OnBnClickedButtonCreate)
  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_BUTTON_START, &CDlgDI::OnBnClickedButtonStart)
  ON_WM_CTLCOLOR()
  ON_WM_TIMER()
  ON_WM_CREATE()
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


void CDlgDI::OnBnClickedButtonCreate()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_BUTTON_CREATE, str);
  if ("创建" == str) {

    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
    if (index == CB_ERR)
      return;

    int data = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetItemData(index);

    //CFrameWndEx *pMain = (CFrameWndEx *)AfxGetMainWnd();
    //if (((CMainFrame*)pMain)
    if (GetMainFrame()->m_timeIOCtrl->CreateDi(m_index, (TimeIOType)data, m_device)) {
      // 刷新按钮
      SetDlg((TimeIOType)data);
      str = "删除";
      SetDlgItemText(IDC_BUTTON_CREATE, str);
    }
    else {
    }
  }// if ("创建" == str) end
  else {
    //CFrameWndEx *pMain = (CFrameWndEx *)AfxGetMainWnd();
    //((CMainFrame*)pMain)
    GetMainFrame()->m_timeIOCtrl->DeleteDi(m_index);

    //((CComboBox*)GetDlgItem(IDC_COMBO_CHANNEL))->ResetContent();
    SetDlg((TimeIOType)-1);

    str = "创建";
    SetDlgItemText(IDC_BUTTON_CREATE, str);
  }
}


void CDlgDI::OnBnClickedButtonStart()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_BUTTON_START, str);
  if ("开始" == str) {
    CString param0, param1;
    GetDlgItemText(IDC_EDIT_PARAM0, param0);
    GetDlgItemText(IDC_EDIT_PARAM1, param1);
    double fparam0 = _wtof(param0);
    double fparam1 = _wtof(param1);
    if (GetMainFrame()->m_timeIOCtrl->StartDi(m_index, m_device, fparam0, fparam1/*, UserFunc*/)) {
      str = "结束";
      SetDlgItemText(IDC_BUTTON_START, str);
      m_brushBack.DeleteObject();
      m_color = RGB(0, 200, 50);
      m_brushBack.CreateSolidBrush(m_color);

      GetDlgItem(IDC_BUTTON_CREATE)->EnableWindow(FALSE);
    }
  }
  else {
    if (GetMainFrame()->m_timeIOCtrl->StopDi(m_index)) {
      str = "开始";
      SetDlgItemText(IDC_BUTTON_START, str);
      m_brushBack.DeleteObject();
      m_color = RGB(240, 240, 240);
      m_brushBack.CreateSolidBrush(m_color);

      GetDlgItem(IDC_BUTTON_CREATE)->EnableWindow(TRUE);
    }
  }
  //SaveParam();
  Invalidate();
}


HBRUSH CDlgDI::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  在此更改 DC 的任何特性
  if (nCtlColor == CTLCOLOR_DLG)
    return   m_brushBack;
  else if (nCtlColor == CTLCOLOR_STATIC)
    pDC->SetBkColor(m_color);
  // TODO:  如果默认的不是所需画笔，则返回另一个画笔
  return hbr;
}


void CDlgDI::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  double fparam0, fparam1;
  if (GetMainFrame()->m_timeIOCtrl->ReadDi(m_index, fparam0, fparam1)) {
    CString sparam0, sparam1;
    sparam0.Format(L"%lf", fparam0);
    sparam1.Format(L"%lf", fparam1);
    SetDlgItemText(IDC_EDIT_PARAM0, sparam0);
    SetDlgItemText(IDC_EDIT_PARAM1, sparam1);
  }
  CFormView::OnTimer(nIDEvent);
}


int CDlgDI::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFormView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码

  m_color = RGB(240, 240, 240);
  m_brushBack.CreateSolidBrush(m_color);
  return 0;
}
