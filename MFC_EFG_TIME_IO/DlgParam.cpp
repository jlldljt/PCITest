// CDlgParam.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgParam.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "EfgAlg.h"


// CDlgParam 对话框

IMPLEMENT_DYNAMIC(CDlgParam, CDialogEx)

CDlgParam::CDlgParam(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_PARAM, pParent)
{
  m_param = &GetMainFrame()->m_efgio.m_configParam;
}

CDlgParam::~CDlgParam()
{
}

void CDlgParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgParam, CDialogEx)
  ON_WM_CTLCOLOR()
  ON_EN_CHANGE(IDC_EDT_FACTOR_A, &CDlgParam::OnEnChangeEdtFactorA)
  ON_EN_CHANGE(IDC_EDT_FACTOR_L, &CDlgParam::OnEnChangeEdtFactorL)
  ON_EN_CHANGE(IDC_EDT_LASER_OFFSET, &CDlgParam::OnEnChangeEdtLaserOffset)
  ON_BN_CLICKED(IDC_BTN_CALC_DEG0, &CDlgParam::OnBnClickedBtnCalcDeg0)
  ON_BN_CLICKED(IDC_BTN_CALC_DEG1, &CDlgParam::OnBnClickedBtnCalcDeg1)
  ON_EN_CHANGE(IDC_EDT_D1, &CDlgParam::OnEnChangeEdtD1)
  ON_EN_CHANGE(IDC_EDT_THETA_OFFSET, &CDlgParam::OnEnChangeEdtThetaOffset)
  ON_EN_CHANGE(IDC_EDT_PHI_OFFSET, &CDlgParam::OnEnChangeEdtPhiOffset)
  ON_EN_CHANGE(IDC_EDT_CNT, &CDlgParam::OnEnChangeEdtCnt)
  ON_BN_CLICKED(IDC_BTN_CALC_EQU, &CDlgParam::OnBnClickedBtnCalcEqu)
END_MESSAGE_MAP()


// CDlgParam 消息处理程序


BOOL CDlgParam::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  CBitmap bmp, sbmp;
  HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, _T("BK5.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
  if (bitmap)
    bmp.Attach(bitmap);
  else
    bmp.LoadBitmap(IDB_BK3);   //IDB_BITMAP1是图片资源ID

  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);
  bmp.DeleteObject();
  //
  CString str;
  str.Format(_T("%.3f"), m_param->laser.factor_a);
  SetDlgItemText(IDC_EDT_FACTOR_A, str);
  str.Format(_T("%.3f"), m_param->laser.factor_l);
  SetDlgItemText(IDC_EDT_FACTOR_L, str);
  str.Format(_T("%.3f"), m_param->laser.offset);
  SetDlgItemText(IDC_EDT_LASER_OFFSET, str);
  str.Format(_T("%.3f"), m_param->laser.theta_offset);
  SetDlgItemText(IDC_EDT_THETA_OFFSET, str);
  str.Format(_T("%.3f"), m_param->laser.phi_offset);
  SetDlgItemText(IDC_EDT_PHI_OFFSET, str);
  
  str.Format(_T("%d"), m_param->user_config.measure.cnt);
  SetDlgItemText(IDC_EDT_CNT, str);

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CDlgParam::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  在此更改 DC 的任何特性
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
  // TODO:  如果默认的不是所需画笔，则返回另一个画笔
  return hbr;
}


void CDlgParam::OnEnChangeEdtFactorA()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_EDT_FACTOR_A, str);
  m_param->laser.factor_a = _wtof(str);
}


void CDlgParam::OnEnChangeEdtFactorL()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_EDT_FACTOR_L, str);
  m_param->laser.factor_l = _wtof(str);
}


void CDlgParam::OnEnChangeEdtLaserOffset()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_EDT_LASER_OFFSET, str);
  m_param->laser.offset = _wtof(str);
}


void CDlgParam::OnBnClickedBtnCalcDeg0()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_EDT_D1, str);
  double D1 = _wtof(str);
  GetDlgItemText(IDC_EDT_D2, str);
  double D2 = _wtof(str);
  GetDlgItemText(IDC_EDT_DM, str);
  double DM = _wtof(str);
  
  double theta0;
  double phi0;
  double u_g;

  EfgAlg alg;
  alg.CalcDegree0(D1,D2,DM,theta0,phi0,u_g);

  str.Format(_T("%06d"), DEG_TO_USER(theta0));
  SetDlgItemText(IDC_EDT_THETA0, str);
  str.Format(_T("%06d"), DEG_TO_USER(phi0));
  SetDlgItemText(IDC_EDT_PHI0, str);

  
}


void CDlgParam::OnBnClickedBtnCalcDeg1()
{
	// TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_EDT_D1, str);
  double D1 = _wtof(str);
  GetDlgItemText(IDC_EDT_D2, str);
  double D2 = _wtof(str);
  GetDlgItemText(IDC_EDT_DM, str);
  double DM = _wtof(str);
  GetDlgItemText(IDC_EDT_R1, str);
  double R1 = _wtof(str);

  
  GetDlgItemText(IDC_EDT_A, str);
  double A = _wtof(str);

  
  GetDlgItemText(IDC_EDT_T, str);
  double T = _wtof(str);
  
  double theta0 = USER_TO_DEG(GetDlgItemInt(IDC_EDT_THETA0));
  double phi0 = USER_TO_DEG(GetDlgItemInt(IDC_EDT_PHI0));

  
  
  double theta1;
  double phi1;
  CEfgIO *m_io = &GetMainFrame()->m_efgio;

  EfgAlg alg;

    double AMP = A/*atan(A * m_io->m_configParam.laser.factor_a / m_io->m_configParam.laser.factor_l) / DPI*/;
  double PHASE = T /*/ DPI*/;


  alg.CalcDegree1(
    AMP,
    PHASE,
    R1,
    m_io->m_configParam.laser.offset,
    theta0,
    phi0,
    theta1,
    phi1,
	DM
  );
  theta1 += m_io->m_configParam.laser.theta_offset/3600;
  phi1 += m_io->m_configParam.laser.phi_offset/3600;

  str.Format(_T("%.3f"), AMP);
  SetDlgItemText(IDC_EDT_TEST_AMP, str);
  str.Format(_T("%.3f"), PHASE);
  SetDlgItemText(IDC_EDT_TEST_PHASE, str);


  str.Format(_T("%06d"), DEG_TO_USER(theta1));
  SetDlgItemText(IDC_EDT_TEST_THETA1, str);
  str.Format(_T("%06d"), DEG_TO_USER(phi1));
  SetDlgItemText(IDC_EDT_TEST_PHI1, str);
}


void CDlgParam::OnEnChangeEdtD1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParam::OnEnChangeEdtThetaOffset()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
  GetDlgItemText(IDC_EDT_THETA_OFFSET, str);
  m_param->laser.theta_offset = _wtof(str);
}


void CDlgParam::OnEnChangeEdtPhiOffset()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
  GetDlgItemText(IDC_EDT_PHI_OFFSET, str);
  m_param->laser.phi_offset = _wtof(str);
}


void CDlgParam::OnEnChangeEdtCnt()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
  GetDlgItemText(IDC_EDT_CNT, str);
  m_param->user_config.measure.cnt = _wtof(str);
}


void CDlgParam::OnBnClickedBtnCalcEqu()
{
	// TODO: 在此添加控件通知处理程序代码

	  double theta1 = USER_TO_DEG(GetDlgItemInt(IDC_EDT_TEST_THETA1,NULL,TRUE));
  double phi1 = USER_TO_DEG(GetDlgItemInt(IDC_EDT_TEST_PHI1,NULL,TRUE));
  
  double equ;
		//计算等效角
  EfgAlg alg;
	alg.CalcEquAngle(
		theta1,
		phi1,
		USER_TO_DEG((&GetMainFrame()->m_efgio)->m_configParam.user_config.measure.equivalent_angle.phi),
		(&GetMainFrame()->m_efgio)->m_configParam.user_config.measure.equivalent_angle.factor/1000.0,
		equ
		);
 // int nequ;
	//alg.CalcEquSec(
	//	DEG_TO_USER(theta0),
	//	DEG_TO_USER(phi0),
	//	(&GetMainFrame()->m_efgio)->m_configParam.user_config.measure.equivalent_angle.phi,
	//	(&GetMainFrame()->m_efgio)->m_configParam.user_config.measure.equivalent_angle.factor,
	//	nequ
	//	);
	
	CString str;
  str.Format(_T("%06d"), DEG_TO_USER(equ));
  SetDlgItemText(IDC_EDT_EQU, str);
}
