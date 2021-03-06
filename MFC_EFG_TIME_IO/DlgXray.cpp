// DlgXray.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgXray.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgXray 对话框

IMPLEMENT_DYNAMIC(CDlgXray, CDialogEx)

CDlgXray::CDlgXray(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_XRAY, pParent)
{
  m_param = &GetMainFrame()->m_efgio.m_configParam;
}

CDlgXray::~CDlgXray()
{
}

void CDlgXray::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgXray, CDialogEx)
  ON_WM_CTLCOLOR()
  ON_BN_CLICKED(IDC_BTN_XRAY_ARC, &CDlgXray::OnBnClickedBtnXrayArc)
  ON_EN_CHANGE(IDC_EDT_THRESHOLD, &CDlgXray::OnEnChangeEdtThreshold)
  ON_EN_CHANGE(IDC_EDT_CONFIRMNUM, &CDlgXray::OnEnChangeEdtConfirmnum)
  ON_EN_CHANGE(IDC_EDT_IGNORE, &CDlgXray::OnEnChangeEdtIgnore)
  ON_EN_CHANGE(IDC_EDT_FACTORH, &CDlgXray::OnEnChangeEdtFactorh)
  ON_EN_CHANGE(IDC_EDT_FACTORW, &CDlgXray::OnEnChangeEdtFactorw)
//  ON_WM_KEYDOWN()
ON_BN_CLICKED(IDC_BTN_TESTFIT, &CDlgXray::OnBnClickedBtnTestfit)
END_MESSAGE_MAP()


// CDlgXray 消息处理程序


BOOL CDlgXray::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  CBitmap bmp, sbmp;
  HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, _T("BK4.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
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

  SetDlgItemInt(IDC_EDT_THRESHOLD, (UINT)(m_param->xray.threshold));
  SetDlgItemInt(IDC_EDT_CONFIRMNUM, (UINT)(m_param->xray.confirmNum));
  SetDlgItemInt(IDC_EDT_IGNORE, (UINT)(m_param->xray.ignore));

  CString str;
  str.Format(_T("%.3f"), m_param->xray.factor_h);
  SetDlgItemText(IDC_EDT_FACTORH, str);
  str.Format(_T("%.3f"), m_param->xray.factor_w);
  SetDlgItemText(IDC_EDT_FACTORW, str);

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CDlgXray::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

CWinThread * gTrdPictureOfXRay = NULL;

UINT Thread_PictureOfXRay(LPVOID pParam)
{
	SetThreadAffinityMask(GetCurrentThread(), 1);
	CDlgXray* pdlg = (CDlgXray*)pParam;

	GetMainFrame()->m_viewBoard->Erase();
	GetMainFrame()->m_viewBoard->DrawToDC(pdlg->GetDlgItem(IDC_PREVIEW)->GetDC());

	GetMainFrame()->m_efgio.WriteDo(XRAY_GATE, IO_ON);//打开光门

	while(-1 == GetMainFrame()->m_efgio.StartMeasure(1))//;//启动测量
	{
		//AfxMessageBox(_T("启动测量失败"));
		//((CButton*)pdlg->GetDlgItem(IDC_BTN_XRAY_ARC))->EnableWindow(TRUE);
		//return 1;//启动失败
		;
	}

	GetMainFrame()->StartMeasure(pdlg->m_param->laser.out3, pdlg->m_param->laser.out6);

	while (!GetMainFrame()->CheckMeasure());
    
	GetMainFrame()->m_efgio.WriteDo(XRAY_GATE, IO_OFF);//关闭光门

	GetMainFrame()->m_diIntCounterSnap.XrayFit(1);
	GetMainFrame()->m_viewBoard->DrawToDC(pdlg->GetDlgItem(IDC_PREVIEW)->GetDC());
	((CButton*)pdlg->GetDlgItem(IDC_BTN_XRAY_ARC))->EnableWindow(TRUE);
	//((CButton*)pdlg->GetDlgItem(IDC_BTN_XRAY_ARC))->GetParent()->SetFocus();
	return 0;
}
UINT Thread_LocalPictureOfXRay(LPVOID pParam)
{
	SetThreadAffinityMask(GetCurrentThread(), 1);
	CDlgXray* pdlg = (CDlgXray*)pParam;
	//GetMainFrame()->m_efgio.WriteDo(XRAY_GATE, IO_ON);//打开光门

	//GetMainFrame()->StartMeasure(pdlg->m_param->laser.out3, pdlg->m_param->laser.out6);

	//while (!GetMainFrame()->CheckMeasure());
  GetMainFrame()->m_diIntCounterSnap.BindCard(0, NULL, GetMainFrame()->m_viewBoard);
	//GetMainFrame()->m_efgio.WriteDo(XRAY_GATE, IO_OFF);//关闭光门

	GetMainFrame()->m_diIntCounterSnap.XrayFitTest(1, pdlg->GetDlgItemInt(IDC_EDT_THRESHOLD, 0), pdlg->GetDlgItemInt(IDC_EDT_CONFIRMNUM, 0), pdlg->GetDlgItemInt(IDC_EDT_IGNORE, 0));
	GetMainFrame()->m_viewBoard->DrawToDC(pdlg->GetDlgItem(IDC_PREVIEW)->GetDC());
	((CButton*)pdlg->GetDlgItem(IDC_BTN_TESTFIT))->EnableWindow(TRUE);
	return 0;
}

//UINT Thread_PN(LPVOID pParam)
//{
//  CDlgXray* pdlg = (CDlgXray*)pParam;
//  Thread_PictureOfXRay(pParam);
//
//
//}

void CDlgXray::OnBnClickedBtnXrayArc()
{
  // TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_XRAY_ARC);

	if(!pBtn->IsWindowEnabled())
		return;

    pBtn->EnableWindow(FALSE);
  //GetMainFrame()->StartMeasure(m_param->laser.out3, m_param->laser.out6);
	gTrdPictureOfXRay = AfxBeginThread(Thread_PictureOfXRay, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    gTrdPictureOfXRay->m_bAutoDelete = TRUE;
    gTrdPictureOfXRay->ResumeThread();



	GetParent()->SetFocus();//为了响应空格
	//SetActiveWindow()
}


void CDlgXray::OnEnChangeEdtThreshold()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->xray.threshold = GetDlgItemInt(IDC_EDT_THRESHOLD, 0);
}


void CDlgXray::OnEnChangeEdtConfirmnum()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->xray.confirmNum = GetDlgItemInt(IDC_EDT_CONFIRMNUM, 0);
}


void CDlgXray::OnEnChangeEdtIgnore()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->xray.ignore = GetDlgItemInt(IDC_EDT_IGNORE, 0);
}


void CDlgXray::OnEnChangeEdtFactorh()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_EDT_FACTORH, str);
  m_param->xray.factor_h = _wtof(str);
}


void CDlgXray::OnEnChangeEdtFactorw()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_EDT_FACTORW, str);
  m_param->xray.factor_w = _wtof(str);
}


//BOOL CDlgXray::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
//		{
//			//判断是否按下键盘Enter键
//			switch (pMsg->wParam)
//			{
//				/*case VK_RETURN:
//				OnBnClickedBtnFit();
//				return TRUE;*/
//			case VK_SPACE:
//				OnBnClickedBtnXrayArc();
//				break;
//			default:
//				break;
//			}
//		}
//	}
//
//	return CDialogEx::PreTranslateMessage(pMsg);
//}


//void CDlgXray::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}



void CDlgXray::OnBnClickedBtnTestfit()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_TESTFIT);

  if (!pBtn->IsWindowEnabled())
    return;

  pBtn->EnableWindow(FALSE);
  //GetMainFrame()->StartMeasure(m_param->laser.out3, m_param->laser.out6);
  gTrdPictureOfXRay = AfxBeginThread(Thread_LocalPictureOfXRay, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
  gTrdPictureOfXRay->m_bAutoDelete = TRUE;
  gTrdPictureOfXRay->ResumeThread();

}
