
// DlgT1.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgT1.h"
#include "MainFrm.h"

// CDlgT1

IMPLEMENT_DYNCREATE(CDlgT1, CFormView)

CDlgT1::CDlgT1()
  : CFormView(IDD_DIALOG_T1)
{
  m_device = -1;
}

CDlgT1::~CDlgT1()
{
  this;
}

void CDlgT1::InitDlg(int index)
{
  m_index = index;
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"TimerPulse");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(0, TIME_PULSE);
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->AddString(L"Tmc12Mode");
  ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetItemData(1, TMC12_COUNTER);

  
	GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(FALSE);
  GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}

void CDlgT1::SetDlg(TimeIOType type)
{
  switch (type) {
  case TMC12_COUNTER:
    SetDlgItemText(IDC_STATIC_PARAM0, L"mode 0 - 6");
    SetDlgItemText(IDC_STATIC_PARAM2, L"计数值");
    GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
    break;
  case TIME_PULSE:
    SetDlgItemText(IDC_STATIC_PARAM0, L"频率Hz");
	GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
    break;
  default:
    SetDlgItemText(IDC_STATIC_PARAM0, L"param0");
    SetDlgItemText(IDC_STATIC_PARAM2, L"param1");
	GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(FALSE);
  GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
    SetDlgItemText(IDC_EDIT_PARAM0, L"");
    SetDlgItemText(IDC_EDIT_PARAM1, L"");
    break;
  }


}

//void CDlgT1::SaveParam()
//{
//  CString str;
//  str.Format(L"T1%d.xml", m_index);
//  CString path = L"config/" + str;
//  char  ppath[50], pstr[50];
//  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
//  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
//  if (0 == m_config.Create(ppath, pstr))
//  {
//    ConfigParam param;
//
//    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
//    CString param0;
//    GetDlgItemText(IDC_EDIT_PARAM0, param0);
//    double fparam0 = _wtof(param0);
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
//    param.param1 = 0;
//    param.start = start;
//    m_config.SaveParam(param);
//  }
//
//}
void CDlgT1::SaveParam()
{
  CString str;
  str.Format(L"T1%d", m_index);
  CString path = L"config/" + m_devName + L".xml";
  char  ppath[50], pstr[50];
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)path, -1, ppath, 50, 0, false);
  WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)str, -1, pstr, 50, 0, false);
  if (0 == m_config.Create(ppath, pstr))
  {
    ConfigParam param;

    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
	int data = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetItemData(index);
    CString param0,param1;
    GetDlgItemText(IDC_EDIT_PARAM0, param0);
    GetDlgItemText(IDC_EDIT_PARAM2, param1);
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
void CDlgT1::LoadParam()
{
  CString str;
  str.Format(L"T1%d", m_index);
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
    //int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetCurSel(param->comboData);
    CString param0,param1;
    param0.Format(L"%lf", param->param0);
    param1.Format(L"%lf", param->param1);
    SetDlgItemText(IDC_EDIT_PARAM0, param0);
    SetDlgItemText(IDC_EDIT_PARAM2, param1);
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

void CDlgT1::Stop(void)
{
  CString str;
  GetDlgItemText(IDC_BUTTON_START, str);

  if ("开始" != str) {
    if (GetMainFrame()->m_timeIOCtrl->StopT1(m_index)) {
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
  GetMainFrame()->m_timeIOCtrl->DeleteT1(m_index);
  SetDlg((TimeIOType)-1);
  str = "创建";
  SetDlgItemText(IDC_BUTTON_CREATE, str);


  Invalidate();
}

void CDlgT1::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
}

BEGIN_MESSAGE_MAP(CDlgT1, CFormView)
  ON_BN_CLICKED(IDC_BUTTON_CREATE, &CDlgT1::OnBnClickedButtonCreate)
  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_BUTTON_START, &CDlgT1::OnBnClickedButtonStart)
  ON_WM_CTLCOLOR()
  ON_WM_DESTROY()
  ON_WM_TIMER()
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


void CDlgT1::OnBnClickedButtonCreate()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_BUTTON_CREATE, str);
  if ("创建" == str) {

    int index = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetCurSel();
    if (index == CB_ERR)
      return;

    int data = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->GetItemData(index);

    if (GetMainFrame()->m_timeIOCtrl->CreateT1(m_index, (TimeIOType)data, m_device)) {
      //// 初始化combo channel
      //;
      //int count = ((CMainFrame*)pMain)->m_timeIOCtrl.GetT1ChannelCount(m_index);

      //for (int i = 0; i < count; i++) {
      //  int channel = ((CMainFrame*)pMain)->m_timeIOCtrl.GetT1Channel(m_index, i);
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
    GetMainFrame()->m_timeIOCtrl->DeleteT1(m_index);

    //((CComboBox*)GetDlgItem(IDC_COMBO_CHANNEL))->ResetContent();
    SetDlg((TimeIOType)-1);

    str = "创建";
    SetDlgItemText(IDC_BUTTON_CREATE, str);
  }
}


int CDlgT1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFormView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码
  m_color = RGB(240, 240, 240);
  m_brushBack.CreateSolidBrush(m_color);
  return 0;
}


void CDlgT1::OnBnClickedButtonStart()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_BUTTON_START, str);
  if ("开始" == str) {
	  CString param0, param1;
    GetDlgItemText(IDC_EDIT_PARAM0, param0);
    GetDlgItemText(IDC_EDIT_PARAM2, param1);
    double fparam0 = _wtof(param0);
    double fparam1 = _wtof(param1);

    if (GetMainFrame()->m_timeIOCtrl->StartT1(m_index, m_device, fparam0, fparam1)) {
      str = "结束";
      SetDlgItemText(IDC_BUTTON_START, str);
      m_brushBack.DeleteObject();
      m_color = RGB(0, 200, 50);
      m_brushBack.CreateSolidBrush(m_color);

      GetDlgItem(IDC_BUTTON_CREATE)->EnableWindow(FALSE);

	  if(fparam0 == 4)
		  SetTimer(0, 200, NULL);
    }
  }
  else {
	  
	  KillTimer(0);
	  Sleep(1);

    if (GetMainFrame()->m_timeIOCtrl->StopT1(m_index)) {
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


HBRUSH CDlgT1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDlgT1::OnDestroy()
{
  CFormView::OnDestroy();

  // TODO: 在此处添加消息处理程序代码
  m_brushBack.DeleteObject();
}


void CDlgT1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//double fparam0, fparam1;
 // if (GetMainFrame()->m_timeIOCtrl->ReadT1(m_index, fparam0, fparam1)) {
 //   CString sparam0, sparam1;
 //   sparam0.Format(L"%lf", fparam0);
 //   sparam1.Format(L"%lf", fparam1);
 //   //SetDlgItemText(IDC_EDIT_PARAM0, sparam0);
 //   SetDlgItemText(IDC_EDIT_PARAM2, sparam1);
 // }
	CFormView::OnTimer(nIDEvent);
}
