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
END_MESSAGE_MAP()


// CSplitFrameWnd 消息处理程序


BOOL CSplitFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
  // TODO: 在此添加专用代码和/或调用基类

  //return CFrameWnd::OnCreateClient(lpcs, pContext);

  CRect rect;
  GetClientRect(&rect);
  int nwidth(rect.right);
  int nheight(rect.bottom);   //获取客户区窗口大小  

  if (!m_splitWndEx.CreateStatic(this, 4, 8))   //窗口分割  
  {
    MessageBox(_T("分割窗口错误"), _T("Error"), MB_OK | MB_ICONERROR);
    return false;
  }
  pContext->m_pCurrentDoc = NULL;
  pContext->m_pCurrentFrame = this;
  
  //关联相关的View类  
  for (int i = 0; i < 8; i++) {
    m_splitWndEx.CreateView(0, i, RUNTIME_CLASS(CDlgDI), CSize((nwidth - 50) / 8, (nheight - 50) / 4), pContext);
    m_splitWndEx.CreateView(1, i, RUNTIME_CLASS(CDlgDO), CSize((nwidth - 50) / 8, (nheight - 50) / 4), pContext);
    m_splitWndEx.CreateView(2, i, RUNTIME_CLASS(CDlgT0), CSize((nwidth - 50) / 8, (nheight - 50) / 4), pContext);
    m_splitWndEx.CreateView(3, i, RUNTIME_CLASS(CDlgT1), CSize((nwidth - 50) / 8, (nheight - 50) / 4), pContext);
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
