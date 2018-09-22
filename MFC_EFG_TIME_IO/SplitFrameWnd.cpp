// SplitFrameWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "SplitFrameWnd.h"
#include "DlgDI.h"
#include "DlgDO.h"
#include "DlgT0.h"
#include "DlgT1.h"

// CSplitFrameWnd

IMPLEMENT_DYNCREATE(CSplitFrameWnd, CFrameWnd)

CSplitFrameWnd::CSplitFrameWnd()
{

}

CSplitFrameWnd::~CSplitFrameWnd()
{
  //for (int i = 0; i < 8; i++) {
  //  m_splitWndEx.DeleteView(0, i);
  //  m_splitWndEx.DeleteView(1, i);
  //  m_splitWndEx.DeleteView(2, i);
  //  m_splitWndEx.DeleteView(3, i);
  //}
}


BEGIN_MESSAGE_MAP(CSplitFrameWnd, CFrameWnd)
  ON_WM_DESTROY()
  ON_WM_CLOSE()
//  ON_WM_HSCROLL()
//  ON_WM_MOUSEHWHEEL()
//  ON_WM_VSCROLL()
ON_WM_HSCROLL()
//ON_WM_MOUSEWHEEL()
ON_WM_VSCROLL()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CSplitFrameWnd 消息处理程序

#define CLIENT_CHANNEL 16
BOOL CSplitFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
  // TODO: 在此添加专用代码和/或调用基类

  //return CFrameWnd::OnCreateClient(lpcs, pContext);

  CRect rect;
  GetClientRect(&rect);
  int nwidth(rect.right);
  int nheight(rect.bottom);   //获取客户区窗口大小  

  //SIZE nsize = CSize((nwidth - 50) / 8, (nheight - 50) / 4);
  SIZE nsize = CSize(190, 160);

  CRect tabRect;
  tabRect = CRect(0, 0, nsize.cx*CLIENT_CHANNEL, nsize.cy * 4); //CRect(0, 0, 243, 268);
                                                                //tabRect = CRect(0, 0, 1000, 300); 
  //SetWindowPos(NULL, rect.left - 190, rect.top - 190, rect.Width(), rect.Height(), SWP_SHOWWINDOW);

  //GetClientRect(&tabRect);
  //SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);

//   ::MoveWindow(this->m_hWnd, 0, 0, 2000, 1000, TRUE);

  if (!m_splitWndEx.CreateStatic(this, 4, CLIENT_CHANNEL))   //窗口分割  
  {
    MessageBox(_T("分割窗口错误"), _T("Error"), MB_OK | MB_ICONERROR);
    return false;
  }
  pContext->m_pCurrentDoc = NULL;
  pContext->m_pCurrentFrame = this;

  //关联相关的View类  
  for (int i = 0; i < CLIENT_CHANNEL; i++) {
    m_splitWndEx.CreateView(0, i, RUNTIME_CLASS(CDlgDI), nsize, pContext);
    m_splitWndEx.CreateView(1, i, RUNTIME_CLASS(CDlgDO), nsize, pContext);
    m_splitWndEx.CreateView(2, i, RUNTIME_CLASS(CDlgT0), nsize, pContext);
    m_splitWndEx.CreateView(3, i, RUNTIME_CLASS(CDlgT1), nsize, pContext);
    CString str;
    str.Format(L"%d", i);
    ((CDlgDI*)m_splitWndEx.GetPane(0, i))->SetDlgItemText(IDC_STATIC, L"DI" + str);
    ((CDlgDO*)m_splitWndEx.GetPane(1, i))->SetDlgItemText(IDC_STATIC, L"DO" + str);
    ((CDlgT0*)m_splitWndEx.GetPane(2, i))->SetDlgItemText(IDC_STATIC, L"T0" + str);
    ((CDlgT1*)m_splitWndEx.GetPane(3, i))->SetDlgItemText(IDC_STATIC, L"T1" + str);


    ((CDlgDI*)m_splitWndEx.GetPane(0, i))->InitDlg(i);
    ((CDlgDO*)m_splitWndEx.GetPane(1, i))->InitDlg(i);
    ((CDlgT0*)m_splitWndEx.GetPane(2, i))->InitDlg(i);
    ((CDlgT1*)m_splitWndEx.GetPane(3, i))->InitDlg(i);
  }

  m_splitWndEx.SetActivePane(0, 0);


  //
  //CRect tabRect;
  //tabRect = CRect(0, 0, nsize.cx*CLIENT_CHANNEL, nsize.cy*4); //CRect(0, 0, 243, 268);
  //                                 //tabRect = CRect(0, 0, 1000, 300); 
  ////GetClientRect(&tabRect);
  
  ScrollInit();
  SetupScrollbars();
  ChangeSize(tabRect);

  return true;
}


void CSplitFrameWnd::OnDestroy()
{
  CFrameWnd::OnDestroy();

  // TODO: 在此处添加消息处理程序代码

}


void CSplitFrameWnd::AssertValid() const
{
  CFrameWnd::AssertValid();

  // TODO: 在此添加专用代码和/或调用基类
}


void CSplitFrameWnd::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);

  // TODO: 在此添加专用代码和/或调用基类
}


void CSplitFrameWnd::OnClose()
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  CFrameWnd::OnClose();
}

//scroll
void CSplitFrameWnd::ChangeSize(CRect rect)
{
  m_ClientRect = rect;
  ResetScrollbars();
  SetupScrollbars();
}

void CSplitFrameWnd::ScrollInit(void)
{
  CRect   rectWindow;
  GetWindowRect(&rectWindow);
  m_nHscrollPos = 0;
  m_nVscrollPos = 0;
  GetParent()->GetClientRect(&m_TabRect);
  if (m_TabRect.right > rectWindow.bottom)
    m_TabRect.right = rectWindow.bottom;
  GetClientRect(&m_ClientRect);
  /*m_ClientRect.right = m_ClientRect.left + m_strPageInfo.rectAbs.Width();
  m_ClientRect.bottom = m_ClientRect.top + m_strPageInfo.rectAbs.Height();*/
  CRect initRect = m_TabRect;
  //initRect.left = initRect.right - GetSystemMetrics(SM_CXHSCROLL);
  //initRect.top = initRect.bottom - GetSystemMetrics(SM_CYVSCROLL);
  DWORD dwStyle = WS_CHILD |
    SBS_SIZEBOX |
    SBS_SIZEBOXBOTTOMRIGHTALIGN |
    SBS_SIZEGRIP |
    WS_VISIBLE;

  m_Scroll.Create(dwStyle, initRect, this, IDC_SCROLL/*AFX_IDW_SIZE_BOX*/);
}

void CSplitFrameWnd::SetupScrollbars()
{
  CRect tempRect = m_TabRect;


  //tempRect.left += LEFT_SPACE;
  //tempRect.right -= RIGHT_SPACE;
  //tempRect.top += TOP_SPACE;
  //tempRect.bottom -= BOTTOM_SPACE;

  // Calculate how many scrolling increments for the client area
  m_nHorzInc = (m_ClientRect.Width() - tempRect.Width()) / HORZ_PTS;
  m_nVertInc = (m_ClientRect.Height() - tempRect.Height()) / VERT_PTS;

  // Set the vertical and horizontal scrolling info
  m_nHscrollMax = max(0, m_nHorzInc);
  m_nHscrollPos = min(m_nHscrollPos, m_nHscrollMax);

  if (m_nHscrollMax)
  {
    m_nHscrollMax = (m_ClientRect.Width() - tempRect.Width()) / HORZ_PTS;// + GetSystemMetrics(SM_CXHSCROLL)
  }
  SetScrollRange(SB_HORZ, 0, m_nHscrollMax, FALSE);
  SetScrollPos(SB_HORZ, m_nHscrollPos, TRUE);

  m_nVscrollMax = max(0, m_nVertInc);
  m_nVscrollPos = min(m_nVscrollPos, m_nVscrollMax);

  if (m_nVscrollMax)
  {
    m_nVscrollMax = (m_ClientRect.Height() - tempRect.Height()) / VERT_PTS;// + GetSystemMetrics(SM_CXHSCROLL)
  }
  SetScrollRange(SB_VERT, 0, m_nVscrollMax, FALSE);
  SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);

}

void CSplitFrameWnd::ResetScrollbars()
{
  // Reset our window scrolling information
  ScrollWindow(m_nHscrollPos*HORZ_PTS, 0, NULL, NULL);
  ScrollWindow(0, m_nVscrollPos*VERT_PTS, NULL, NULL);
  m_nHscrollPos = 0;
  m_nVscrollPos = 0;
  SetScrollPos(SB_HORZ, m_nHscrollPos, TRUE);
  SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
}


void CSplitFrameWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  int nInc;
  switch (nSBCode)
  {
  case SB_LEFT:        nInc = -m_nHscrollPos;               break;
  case SB_RIGHT:     nInc = m_nHscrollMax - m_nHscrollPos;  break;
  case SB_LINELEFT:     nInc = -1;                           break;
  case SB_LINERIGHT:   nInc = 1;                            break;
  case SB_PAGELEFT:     nInc = min(-1, -m_nHorzInc);         break;
  case SB_PAGERIGHT:   nInc = max(1, m_nHorzInc);           break;
  case SB_THUMBTRACK: nInc = nPos - m_nHscrollPos;         break;
  default:            nInc = 0;
  }

  nInc = max(-m_nHscrollPos, min(nInc, m_nHscrollMax - m_nHscrollPos));
  m_nHscrollPos += nInc;
  int iMove = -HORZ_PTS * nInc;
  ScrollWindow(iMove, 0, NULL, NULL);
  SetScrollPos(SB_HORZ, m_nHscrollPos, TRUE);

  CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


//BOOL CSplitFrameWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//  // 此功能要求 Windows Vista 或更高版本。
//  // _WIN32_WINNT 符号必须 >= 0x0600。
//  // TODO: 在此添加消息处理程序代码和/或调用默认值
//  int nInc = -10 * zDelta / 120;
//  
//    nInc = max(-m_nVscrollPos, min(nInc, m_nVscrollMax - m_nVscrollPos));
//    m_nVscrollPos += nInc;
//    int iMove = -VERT_PTS * nInc;
//  
//    ScrollWindow(0, iMove, NULL, NULL);
//    SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
//  CFrameWnd::OnMouseHWheel(nFlags, zDelta, pt);
//}


void CSplitFrameWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  int nInc;
    switch (nSBCode)
    {
    case SB_TOP:        nInc = -m_nVscrollPos;               break;
    case SB_BOTTOM:     nInc = m_nVscrollMax - m_nVscrollPos;  break;
    case SB_LINEUP:     nInc = -1;                           break;
    case SB_LINEDOWN:   nInc = 1;                            break;
    case SB_PAGEUP:     nInc = min(-1, -m_nVertInc);         break;
    case SB_PAGEDOWN:   nInc = max(1, m_nVertInc);           break;
    case SB_THUMBTRACK: nInc = nPos - m_nVscrollPos;         break;
    default:            nInc = 0;
    }
  
    nInc = max(-m_nVscrollPos, min(nInc, m_nVscrollMax - m_nVscrollPos));
    m_nVscrollPos += nInc;
    int iMove = -VERT_PTS * nInc;
  
    ScrollWindow(0, iMove, NULL, NULL);
    SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
    
    Invalidate(TRUE);

  CFrameWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}



BOOL CSplitFrameWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
    int nInc = -10 * zDelta / 120;
    
      nInc = max(-m_nVscrollPos, min(nInc, m_nVscrollMax - m_nVscrollPos));
      m_nVscrollPos += nInc;
      int iMove = -VERT_PTS * nInc;
    
      ScrollWindow(0, iMove, NULL, NULL);
      SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);

  return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}
