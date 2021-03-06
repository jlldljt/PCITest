// DlgTestCalcDegree1.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgTestCalcDegree1.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "EfgAlg.h"


// CDlgTestCalcDegree1 对话框

IMPLEMENT_DYNAMIC(CDlgTestCalcDegree1, CDialogEx)

CDlgTestCalcDegree1::CDlgTestCalcDegree1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_TEST_CALC_DEGREE1, pParent)
{
  m_param = &GetMainFrame()->m_efgio.m_configParam;
}

CDlgTestCalcDegree1::~CDlgTestCalcDegree1()
{
}

void CDlgTestCalcDegree1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTestCalcDegree1, CDialogEx)
  ON_BN_CLICKED(IDC_BTN_CALC_DEGREE1, &CDlgTestCalcDegree1::OnBnClickedBtnCalcDegree1)
END_MESSAGE_MAP()


// CDlgTestCalcDegree1 消息处理程序


void CDlgTestCalcDegree1::OnBnClickedBtnCalcDegree1()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  double amp  = USER_TO_DEG(GetDlgItemInt(IDC_EDT_AMP));
  GetDlgItemText(IDC_EDT_PHASE, str);
  double phase = _wtof(str);
  GetDlgItemText(IDC_EDT_R1, str);
  double r1 = _wtof(str);
  GetDlgItemText(IDC_EDT_DM, str);
  double dm = _wtof(str);
  
  double theta0 = USER_TO_DEG(GetDlgItemInt(IDC_EDT_THETA0));
  double phi0 = USER_TO_DEG(GetDlgItemInt(IDC_EDT_PHI0));


  double theta1;
  double phi1;

  EfgAlg alg;

  alg.CalcDegree1(amp, phase, r1, m_param->laser.offset, theta0, phi0, theta1, phi1, dm);
  
  theta1 += m_param->laser.theta_offset/3600;
  phi1 += m_param->laser.phi_offset/3600;

  str.Format(_T("%06d"), DEG_TO_USER(theta1));
  SetDlgItemText(IDC_EDT_THETA1, str);
  str.Format(_T("%06d"), DEG_TO_USER(phi1));
  SetDlgItemText(IDC_EDT_PHI1, str);
}

