// CDlgParam.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgParam.h"
#include "afxdialogex.h"
#include "MainFrm.h"


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
