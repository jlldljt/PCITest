// DlgPriview1.cpp : 实现文件
//
/*
要在静态库中使用crecttracker

#define _AFX_NO_SPLITTER_RESOURCES
#define _AFX_NO_OLE_RESOURCES
#define _AFX_NO_TRACKER_RESOURCES
#define _AFX_NO_PROPERTY_RESOURCES

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
LANGUAGE 4, 2
#include "res\BER.rc2"  // 非 Microsoft Visual C++ 编辑的资源
#include "l.CHS\afxres.rc"      // 标准组件
#endif


*/

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgPriview1.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CDlgPriview1 对话框

IMPLEMENT_DYNAMIC(CDlgPriview1, CDialogEx)

CDlgPriview1::CDlgPriview1(CWnd* pParent /*=NULL*/)
  : CDialogEx(CDlgPriview1::IDD, pParent)
{
  m_n = 0;

  m_p = (CCamera*)pParent;
  bDraw = FALSE;

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
  ON_WM_PAINT()
  ON_WM_SETCURSOR()
  ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgPriview1 消息处理程序
void CDlgPriview1::Process()
{
}

void CDlgPriview1::OnLButtonDown(UINT nFlags, CPoint point)
{
  int nRetCode = m_RectTracker.HitTest(point);

  if (nRetCode < 0)            //不在矩形区域
  {
    //给temp变量初始化
    CRectTracker  temp;
    temp.m_rect.SetRect(0, 0, 0, 0);
    temp.m_nStyle = CRectTracker::hatchedBorder | CRectTracker::resizeInside;

    //TrackRubberBand函数直到鼠标左键提起的时候才会返回，并把左键提起的位置也记录了下来
    //该函数过程中，不会产生鼠标移动消息
    temp.TrackRubberBand(this, point, TRUE);
    //使得矩形为正确的坐标，不会出现负的坐标
    temp.m_rect.NormalizeRect();
    //此时把bDraw设为false
    bDraw = FALSE;
    CClientDC dc(this);
    //把新的矩形区域绘制出来
    temp.Draw(&dc);
    m_RectTracker.m_rect = temp.m_rect;
    Invalidate(TRUE);
  }
  else                        //在矩形区域内
  {
    //Track和TrackRubberBand类似，直到鼠标左键提起的时候才会返回，并把左键提起的位置也记录了下来
    //该函数过程中，不会产生鼠标移动消息
    m_RectTracker.Track(this, point, TRUE);
    //设置为true,则需要在矩形内绘制椭圆
    bDraw = TRUE;

    //窗口无效，调用Onpaint函数
    Invalidate(TRUE);
  }

  CDialogEx::OnLButtonDown(nFlags, point);
}
//void CDlgPriview1::OnLButtonDown(UINT nFlags, CPoint point)
//{
//  // TODO: 在此添加消息处理程序代码和/或调用默认值
//  if (0 == m_p->m_rcg.g_stu_square.nBMPW || 0 == m_p->m_rcg.g_stu_square.nBMPH) {
//    AfxMessageBox(_T("先点击识别测试"));
//    return;
//  }
//  double xt, yt;
//  POINT pt;//定义点
//  GetCursorPos(&pt);//取得当前坐标
//  CRect lRect;
//  GetWindowRect(&lRect);
//
//  int y_caption = GetSystemMetrics(SM_CYCAPTION);
//  int x_frame = GetSystemMetrics(SM_CXDLGFRAME);
//  int y_frame = GetSystemMetrics(SM_CYDLGFRAME);
//  lRect.left += x_frame;
//  lRect.right -= x_frame;
//  lRect.top += y_frame + y_caption;
//  lRect.bottom -= y_frame;
//  double temp_x = pt.x - lRect.left;
//  double temp_y = pt.y - lRect.top;
//  xt =/*(int)*/(temp_x * m_p->m_rcg.g_stu_square.nBMPW / (lRect.Width()));//点击的坐标对应在图像上的x点
//  yt =/*(int)*/(temp_y * m_p->m_rcg.g_stu_square.nBMPH / (lRect.Height()));//点击的坐标对应在图像上的y点
//
//
//
//
//  CWnd* pWnd = this;
//  CDC* pDC = pWnd->GetDC();
//  CRect rect, rect_camera;
//  GetClientRect(&rect);
//  m_p->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
//  CPen PenRed;
//  PenRed.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//创建一支的画笔.
//  pDC->SelectObject(PenRed);//选中画笔.
//  CBrush br;
//  br.CreateStockObject(NULL_BRUSH);
//  pDC->SelectObject(&br);
//
//  //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), m_p->mDCMem, 0, 0, SRCCOPY);
//  pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), m_p->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
//  POINT pt_dc;//定义点
//  pt_dc.x = temp_x - 10 + 0.5;
//  pt_dc.y = temp_y + 0.5;
//  pDC->MoveTo(pt_dc);
//  pt_dc.x = temp_x + 10 + 0.5;
//  pDC->LineTo(pt_dc);
//  //
//  pt_dc.x = temp_x + 0.5;
//  pt_dc.y = temp_y - 10 + 0.5;
//  pDC->MoveTo(pt_dc);
//  pt_dc.y = temp_y + 10 + 0.5;
//  pDC->LineTo(pt_dc);
//  /////////////////////
//  CString csTmp;
//
//  int X = xt + 0.5;
//  int Y = yt + 0.5;
//
//  csTmp.Format(_T("X:%d	Y:%dX"), X, Y);
//
//
//
//  //////
//  CString str;
//
//  if (m_n == 0) {
//    str.Format(_T("是否把图像坐标X:%d	Y:%d 作为左上角"), X, Y);
//   // m_p->m_npc_inf.left = X;
//   // m_p->m_npc_inf.top = Y;
//  }
//  else if (m_n == 1) {
//    str.Format(_T("是否把图像坐标X:%d	Y:%d 作为右下角"), X, Y);
//   // m_p->m_npc_inf.right = X;
//   // m_p->m_npc_inf.bottom = Y;
//  }
//
//  pDC->Rectangle(m_p->m_npc_inf.left * lRect.Width()/m_p->m_rcg.g_stu_square.nBMPW  
//    , m_p->m_npc_inf.top *  lRect.Height()/m_p->m_rcg.g_stu_square.nBMPH 
//    , m_p->m_npc_inf.right *  lRect.Width()/m_p->m_rcg.g_stu_square.nBMPW 
//    , m_p->m_npc_inf.bottom *lRect.Height()  / m_p->m_rcg.g_stu_square.nBMPH);
//  pWnd->ReleaseDC(pDC);
//
//  int status = AfxMessageBox(str, MB_OKCANCEL);
//  if (status == 1)//按确定退出
//  {
//
//    if (m_n == 0) {
//      m_p->m_npc_inf.left = X;
//      m_p->m_npc_inf.top = Y;
//    }
//    else if (m_n == 1) {
//      m_p->m_npc_inf.right = X;
//      m_p->m_npc_inf.bottom = Y;
//    }
//
//    m_n++;
//
//    if (m_n == 2)
//    {
//      //保存
//      CString strValue;
//      strValue.Format(_T("%d"), m_p->m_npc_inf.left);
//      WritePrivateProfileString(_T("范围设置"), _T("left"), strValue, g_ini_path);
//      strValue.Format(_T("%d"), m_p->m_npc_inf.top);
//      WritePrivateProfileString(_T("范围设置"), _T("top"), strValue, g_ini_path);
//      strValue.Format(_T("%d"), m_p->m_npc_inf.right);
//      WritePrivateProfileString(_T("范围设置"), _T("right"), strValue, g_ini_path);
//      strValue.Format(_T("%d"), m_p->m_npc_inf.bottom);
//      WritePrivateProfileString(_T("范围设置"), _T("bottom"), strValue, g_ini_path);
//
//      AfxMessageBox(_T("参数更新完成"), MB_OK);
//      m_n = 0;
//    }
//  }
//  else
//  {
//    //gstuRcgInfo.Nxy=0;
//  }
//
//  CDialogEx::OnLButtonDown(nFlags, point);
//}


BOOL CDlgPriview1::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  if (0 == m_p->m_rcg.g_stu_square.nBMPW || 0 == m_p->m_rcg.g_stu_square.nBMPH) {
    AfxMessageBox(_T("先点击识别测试"));
    //DestroyWindow();
    return FALSE;
  }

  CRect rect;

  GetClientRect(&rect);
  
  m_RectTracker.m_rect.SetRect(m_p->m_npc_inf.left* rect.Width() / m_p->m_rcg.g_stu_square.nBMPW
    , m_p->m_npc_inf.top* rect.Height() / m_p->m_rcg.g_stu_square.nBMPH
    , m_p->m_npc_inf.right* rect.Width() / m_p->m_rcg.g_stu_square.nBMPW
    , m_p->m_npc_inf.bottom* rect.Height() / m_p->m_rcg.g_stu_square.nBMPH);

    m_RectTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}


void CDlgPriview1::OnPaint()
{
  CPaintDC dc(this); // device context for painting
                     // TODO: 在此处添加消息处理程序代码
                     // 不为绘图消息调用 CDialogEx::OnPaint()
  CBrush brush(RGB(0, 0, 255));
  CBrush* oldBrush = dc.SelectObject(&brush);
  CRect rect,wnd_rc, rect_camera;

  GetClientRect(&wnd_rc);
  m_p->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
  dc.TransparentBlt/*BitBlt*/(wnd_rc.left, wnd_rc.top, wnd_rc.Width(), wnd_rc.Height(), m_p->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
  // 

  m_RectTracker.GetTrueRect(&rect); //得到CRectTracker中的m_rect的大小，将其传递给rect；

  if (bDraw)
  {
    //dc.Ellipse(&rect);
  }

  m_RectTracker.Draw(&dc);            //这句画才真正的将这个四边形画出来；

  dc.SelectObject(oldBrush);

  //

}


BOOL CDlgPriview1::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  // 改变鼠标形状
  if ((pWnd == this) && (m_RectTracker.SetCursor(this, nHitTest)))
  {
    return TRUE;
  }

  return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CDlgPriview1::OnDestroy()
{
  CDialogEx::OnDestroy();

  // TODO: 在此处添加消息处理程序代码
    // TODO:  在此添加额外的初始化
  if (0 == m_p->m_rcg.g_stu_square.nBMPW || 0 == m_p->m_rcg.g_stu_square.nBMPH) {
    return;
  }

  CRect rect;
  CRect wnd_rc;

  m_RectTracker.GetTrueRect(&rect); //得到CRectTracker中的m_rect的大小，将其传递给rect；
  GetClientRect(&wnd_rc);

  m_p->m_npc_inf.left = (rect.left * m_p->m_rcg.g_stu_square.nBMPW / (wnd_rc.Width()));
  m_p->m_npc_inf.top=(rect.top * m_p->m_rcg.g_stu_square.nBMPH / (wnd_rc.Height()));
  m_p->m_npc_inf.right=(rect.right * m_p->m_rcg.g_stu_square.nBMPW / (wnd_rc.Width()));
  m_p->m_npc_inf.bottom=(rect.bottom * m_p->m_rcg.g_stu_square.nBMPH / (wnd_rc.Height()));


      //保存
      CString strValue;
      strValue.Format(_T("%d"), m_p->m_npc_inf.left);
      WritePrivateProfileString(_T("范围设置"), _T("left"), strValue, g_ini_path);
      strValue.Format(_T("%d"), m_p->m_npc_inf.top);
      WritePrivateProfileString(_T("范围设置"), _T("top"), strValue, g_ini_path);
      strValue.Format(_T("%d"), m_p->m_npc_inf.right);
      WritePrivateProfileString(_T("范围设置"), _T("right"), strValue, g_ini_path);
      strValue.Format(_T("%d"), m_p->m_npc_inf.bottom);
      WritePrivateProfileString(_T("范围设置"), _T("bottom"), strValue, g_ini_path);


}
