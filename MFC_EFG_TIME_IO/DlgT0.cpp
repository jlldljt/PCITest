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
  m_device = -1;
}

CDlgT0::~CDlgT0()
{
  this;
}

void CDlgT0::InitDlg(int index)
{
  m_index = index;

  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"TimerPulse");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(0, TIME_PULSE);
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"OneShot");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(1, ONE_SHOT);
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"PwModulator");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(2, PW_MODULATOR);
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"EventCounter");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(3, EVENT_COUNTER);

  GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(FALSE);
  GetDlgItem(IDC_EDIT_PARAM1)->EnableWindow(FALSE);
  GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

}

void CDlgT0::SetDlg(TimeIOType type)
{
  switch (type) {
  case TIME_PULSE:
    SetDlgItemText(IDC_STATIC_PARAM0, L"频率Hz");
    GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
    break;
  case PW_MODULATOR:
    SetDlgItemText(IDC_STATIC_PARAM0, L"Hi宽S   ");
    SetDlgItemText(IDC_STATIC_PARAM1, L"Lo宽S   ");
    GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_PARAM1)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
    break;
  case EVENT_COUNTER:
    SetDlgItemText(IDC_STATIC_PARAM0, L"数值    ");//多个空格以填满static
    GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
    SetTimer(0, 40, NULL);
    break;

  case ONE_SHOT:
    SetDlgItemText(IDC_STATIC_PARAM0, L"延时cnt");
    SetDlgItemText(IDC_STATIC_PARAM1, L"极性1/0");
    GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_PARAM1)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
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

//void CDlgT0::SaveParamWithDev()
//{
//  CString str;
//  str.Format(L"T0%d.xml", m_index);
//  CString path = L"config/" + str;
//  char  ppath[50], pstr[50];
//  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
//  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
//  if (0 == m_config.Create(ppath, pstr))
//  {
//    ConfigParam param;
//
//    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
//    CString param0, param1;
//    GetDlgItemText(IDC_EDIT_PARAM0, param0);
//    GetDlgItemText(IDC_EDIT_PARAM1, param1);
//    double fparam0 = _wtof(param0);
//    double fparam1 = _wtof(param1);
//    bool start;
//    CString str;
//    GetDlgItemText(IDC_BUTTON_START, str);
//    if ("开始" == str)
//      start = false;
//    else
//      start = true;
//    param.device = m_device;
//    param.comboData = index;
//    param.param0 = fparam0;
//    param.param1 = fparam1;
//    param.start = start;
//    m_config.SaveParam(param);
//  }
//
//}
void CDlgT0::SaveParam()
{
  CString str;
  str.Format(L"T0%d", m_index);
  CString path = L"config/" + m_devName+ L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == m_config.Create(ppath, pstr))
  {
    ConfigParam param;

    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
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
    param.comboData = index;
    param.param0 = fparam0;
    param.param1 = fparam1;
    param.start = start;
    m_config.SaveParam(param);
  }

}
//void CDlgT0::LoadParam()
//{
//  CString str;
//  str.Format(L"T0%d.xml", m_index);
//  CString path = L"config/" + str;
//  char  ppath[50], pstr[50];
//  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
//  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
//  if (0 == m_config.Create(ppath, pstr))
//  {
//    ConfigParam* param = m_config.ReadParam();
//    if (!param)
//      return;
//    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetCurSel(param->comboData);
//    CString param0, param1;
//    param0.Format(L"%lf", param->param0);
//    param1.Format(L"%lf", param->param1);
//    SetDlgItemText(IDC_EDIT_PARAM0, param0);
//    SetDlgItemText(IDC_EDIT_PARAM1, param1);
//    if (param->start && param->device == m_device) //启动状态
//    {
//      CString str;
//      GetDlgItemText(IDC_BUTTON_CREATE, str);
//      if ("创建" == str) {
//        OnBnClickedButtonCreate();
//        OnBnClickedButtonStart();
//      }
//    }
//    delete param;
//  }
//}
void CDlgT0::LoadParam()
{
  CString str;
  str.Format(L"T0%d", m_index);
  CString path = L"config/" + m_devName + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == m_config.Create(ppath, pstr))
  {
    ConfigParam* param = m_config.ReadParam();
    if (!param)
      return;
    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetCurSel(param->comboData);
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
void CDlgT0::Stop(void)
{
  CString str;
  GetDlgItemText(IDC_BUTTON_START, str);
  //if ("开始" == str) {
  //  return;
  //}
  if ("开始" != str) {

    if (GetMainFrame()->m_timeIOCtrl.StopT0(m_index)) {
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
    GetMainFrame()->m_timeIOCtrl.DeleteT0(m_index);
    SetDlg((TimeIOType)-1);
    str = "创建";
    SetDlgItemText(IDC_BUTTON_CREATE, str);

    Invalidate();
}

void CDlgT0::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
}

BEGIN_MESSAGE_MAP(CDlgT0, CFormView)
  ON_BN_CLICKED(IDC_BUTTON_CREATE, &CDlgT0::OnBnClickedButtonCreate)
  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_BUTTON_START, &CDlgT0::OnBnClickedButtonStart)
  ON_WM_CTLCOLOR()
  ON_WM_TIMER()
  ON_WM_DESTROY()
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
  CString str;
  GetDlgItemText(IDC_BUTTON_CREATE, str);
  if ("创建" == str) {

    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
    if (index == CB_ERR)
      return;

    int data = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetItemData(index);
    
    //CFrameWndEx *pMain = (CFrameWndEx *)AfxGetMainWnd();
    //if (((CMainFrame*)pMain)
    if (GetMainFrame()->m_timeIOCtrl.CreateT0(m_index, (TimeIOType)data, m_device)) {
      //// 初始化combo channel
      //;
      //int count = ((CMainFrame*)pMain)->m_timeIOCtrl.GetT0ChannelCount(m_index);

      //for (int i = 0; i < count; i++) {
      //  int channel = ((CMainFrame*)pMain)->m_timeIOCtrl.GetT0Channel(m_index, i);
      //  CString str;
      //  str.Format(L"%d", channel);
      //  ((CComboBox*)GetDlgItem(IDC_COMBO_CHANNEL))->AddString(str);
      //}

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
    GetMainFrame()->m_timeIOCtrl.DeleteT0(m_index);

    //((CComboBox*)GetDlgItem(IDC_COMBO_CHANNEL))->ResetContent();
    SetDlg((TimeIOType)-1);

    str = "创建";
    SetDlgItemText(IDC_BUTTON_CREATE, str);
  }
}


int CDlgT0::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFormView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码
  m_color = RGB(240, 240, 240);
  m_brushBack.CreateSolidBrush(m_color);
  return 0;
}


void CDlgT0::OnBnClickedButtonStart()
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
    if (GetMainFrame()->m_timeIOCtrl.StartT0(m_index, m_device, fparam0, fparam1)) {
      str = "结束";
      SetDlgItemText(IDC_BUTTON_START, str);
      m_brushBack.DeleteObject();
      m_color = RGB(0, 200, 50);
      m_brushBack.CreateSolidBrush(m_color);

      GetDlgItem(IDC_BUTTON_CREATE)->EnableWindow(FALSE);

    }
  }
  else {
    if (GetMainFrame()->m_timeIOCtrl.StopT0(m_index)) {
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


HBRUSH CDlgT0::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDlgT0::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  double fparam0, fparam1;
  if (GetMainFrame()->m_timeIOCtrl.ReadT0(m_index, fparam0, fparam1)) {
    CString sparam0, sparam1;
    sparam0.Format(L"%lf", fparam0);
    sparam1.Format(L"%lf", fparam1);
    SetDlgItemText(IDC_EDIT_PARAM0, sparam0);
    SetDlgItemText(IDC_EDIT_PARAM1, sparam1);
  }
  CFormView::OnTimer(nIDEvent);
}


void CDlgT0::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: 在此处添加消息处理程序代码
      m_brushBack.DeleteObject();

}
