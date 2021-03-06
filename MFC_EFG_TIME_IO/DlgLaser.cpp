// DlgLaser.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgLaser.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CDlgLaser 对话框

IMPLEMENT_DYNAMIC(CDlgLaser, CDialogEx)

CDlgLaser::CDlgLaser(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LASER, pParent)
{
  m_param = &GetMainFrame()->m_efgio.m_configParam;
}

CDlgLaser::~CDlgLaser()
{
}

void CDlgLaser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLaser, CDialogEx)
  ON_WM_CTLCOLOR()
  ON_BN_CLICKED(IDC_BTN_SHOW_ARC, &CDlgLaser::OnBnClickedBtnShowArc)
  ON_EN_CHANGE(IDC_EDT_OUT3, &CDlgLaser::OnEnChangeEdtOut3)
  ON_EN_CHANGE(IDC_EDT_OUT6, &CDlgLaser::OnEnChangeEdtOut6)
END_MESSAGE_MAP()


// CDlgLaser 消息处理程序


BOOL CDlgLaser::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  CBitmap bmp, sbmp;
 // bmp.LoadBitmap(IDB_BK3);   //IDB_BITMAP1是图片资源ID
  HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, _T("BK3.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
  if(bitmap)
    bmp.Attach(bitmap);
  else
    bmp.LoadBitmap(IDB_BK3);   //IDB_BITMAP1是图片资源ID

  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);
  bmp.DeleteObject();

  //
  SetDlgItemInt(IDC_EDT_OUT3, (UINT)(m_param->laser.out3));
  SetDlgItemInt(IDC_EDT_OUT6, (UINT)(m_param->laser.out6));

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CDlgLaser::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDlgLaser::OnBnClickedBtnShowArc()
{
  // TODO: 在此添加控件通知处理程序代码
  //GetMainFrame()->m_diIntCounterSnap.BindCard(0, NULL, GetMainFrame()->m_viewBoard);
  ////// GetMainFrame()->Switch(VIEW_BOARD);
  //GetMainFrame()->m_diIntCounterSnap.TestS();
  //GetMainFrame()->m_viewBoard->DrawToDC(GetDlgItem(IDC_PREVIEW)->GetDC());
  //return;
  //GetMainFrame()->StartMeasure(m_param->laser.out3, m_param->laser.out6);
	if(-1 == GetMainFrame()->m_efgio.StartMeasure(1))//;//启动测量
	{
		AfxMessageBox(_T("启动测量失败"));
		return;//启动失败
	}

	GetMainFrame()->StartMeasure(m_param->laser.out3, m_param->laser.out6);

      while (!GetMainFrame()->CheckMeasure());
      
  GetMainFrame()->m_diIntCounterSnap.LaserFit(1);
  GetMainFrame()->m_viewBoard->DrawToDC(GetDlgItem(IDC_PREVIEW)->GetDC());
}


void CDlgLaser::OnEnChangeEdtOut3()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->laser.out3 = GetDlgItemInt(IDC_EDT_OUT3, 0);
}


void CDlgLaser::OnEnChangeEdtOut6()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->laser.out6 = GetDlgItemInt(IDC_EDT_OUT6, 0);
}
