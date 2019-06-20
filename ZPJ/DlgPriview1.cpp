// DlgPriview1.cpp : 实现文件
//

#include "stdafx.h"
#include "ZPJ.h"
#include "DlgPriview1.h"
#include "afxdialogex.h"


// CDlgPriview1 对话框

IMPLEMENT_DYNAMIC(CDlgPriview1, CDialogEx)

CDlgPriview1::CDlgPriview1(CWnd* pParent /*=NULL*/)
  : CDialogEx(CDlgPriview1::IDD, pParent)
{
  m_n = 0;
}

CDlgPriview1::~CDlgPriview1()
{
}

void CDlgPriview1::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriview1, CDialogEx)
  ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgPriview1 消息处理程序


void CDlgPriview1::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  double xt, yt;
  POINT pt;//定义点
  GetCursorPos(&pt);//取得当前坐标
  CRect lRect;
  GetWindowRect(&lRect);

  int y_caption = GetSystemMetrics(SM_CYCAPTION);
  int x_frame = GetSystemMetrics(SM_CXDLGFRAME);
  int y_frame = GetSystemMetrics(SM_CYDLGFRAME);
  lRect.left += x_frame;
  lRect.right -= x_frame;
  lRect.top += y_frame + y_caption;
  lRect.bottom -= y_frame;
  double temp_x = pt.x - lRect.left;
  double temp_y = pt.y - lRect.top;
  xt =/*(int)*/(temp_x * gclsImgRcg.g_stu_square.nBMPW / (lRect.Width()));//点击的坐标对应在图像上的x点
  yt =/*(int)*/(temp_y * gclsImgRcg.g_stu_square.nBMPH / (lRect.Height()));//点击的坐标对应在图像上的y点




  CWnd* pWnd = this;
  CDC* pDC = pWnd->GetDC();
  CRect rect, rect_camera;
  GetClientRect(&rect);
  g_dlgCamera->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
  CPen PenRed;
  PenRed.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//创建一支的画笔.
  pDC->SelectObject(PenRed);//选中画笔.
  //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), g_dlgCamera->mDCMem, 0, 0, SRCCOPY);
  pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), g_dlgCamera->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
  POINT pt_dc;//定义点
  pt_dc.x = temp_x - 10 + 0.5;
  pt_dc.y = temp_y + 0.5;
  pDC->MoveTo(pt_dc);
  pt_dc.x = temp_x + 10 + 0.5;
  pDC->LineTo(pt_dc);
  //
  pt_dc.x = temp_x + 0.5;
  pt_dc.y = temp_y - 10 + 0.5;
  pDC->MoveTo(pt_dc);
  pt_dc.y = temp_y + 10 + 0.5;
  pDC->LineTo(pt_dc);

  pWnd->ReleaseDC(pDC);
  /////////////////////
  CString csTmp;

  int X = xt + 0.5;
  int Y = yt + 0.5;

  csTmp.Format(_T("X:%d	Y:%dX"), X, Y);



  //////
  CString str;

  if (m_n == 0) {
    str.Format(_T("是否把图像坐标X:%d	Y:%d 作为左上角"), X, Y);
    g_npc_inf.left = X;
    g_npc_inf.top = Y;
  }
  else if (m_n == 1) {
    str.Format(_T("是否把图像坐标X:%d	Y:%d 作为右下角"), X, Y);
    g_npc_inf.right = X;
    g_npc_inf.bottom = Y;
  }

  int status = AfxMessageBox(str, MB_OKCANCEL);
  if (status == 1)//按确定退出
  {


    m_n++;

    if (m_n == 2)
    {
      //保存
      CString strValue;
      strValue.Format(_T("%d"), g_npc_inf.left);
      WritePrivateProfileString(_T("范围设置"), _T("left"), strValue, gstuPathInf.csPathIni);
      strValue.Format(_T("%d"), g_npc_inf.top);
      WritePrivateProfileString(_T("范围设置"), _T("top"), strValue, gstuPathInf.csPathIni);
      strValue.Format(_T("%d"), g_npc_inf.right);
      WritePrivateProfileString(_T("范围设置"), _T("right"), strValue, gstuPathInf.csPathIni);
      strValue.Format(_T("%d"), g_npc_inf.bottom);
      WritePrivateProfileString(_T("范围设置"), _T("bottom"), strValue, gstuPathInf.csPathIni);

      AfxMessageBox(_T("参数更新完成"), MB_OK);
      m_n = 0;
    }
  }
  else
  {
    //gstuRcgInfo.Nxy=0;
  }

  CDialogEx::OnLButtonDown(nFlags, point);
}
