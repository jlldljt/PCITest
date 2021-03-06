// CUserSplitterWnd.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "CUserSplitterWnd.h"

#include "DlgDebug1.h"

#include "DlgRun1.h"
// CUserSplitterWnd

IMPLEMENT_DYNCREATE(CUserSplitterWnd, CFrameWnd)

CUserSplitterWnd::CUserSplitterWnd()
{
  m_splitUser.SetCx(10);
}


CUserSplitterWnd::~CUserSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CUserSplitterWnd, CFrameWnd)
END_MESSAGE_MAP()


// CUserSplitterWnd 消息处理程序


BOOL CUserSplitterWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
  // TODO: 在此添加专用代码和/或调用基类
  //return CFrameWnd::OnCreateClient(lpcs, pContext);
  CRect rect;
  GetClientRect(&rect);
  int nwidth(rect.right);
  int nheight(rect.bottom);   //获取客户区窗口大小  

  SIZE nsize = CSize((nwidth - 50) / 2, (nheight - 50) / 1);

  if (!m_splitUser.CreateStatic(this, 1, 2))   //窗口分割  
  {
    MessageBox(_T("分割窗口错误"), _T("Error"), MB_OK | MB_ICONERROR);
    return false;
  }
  pContext->m_pCurrentDoc = NULL;
  pContext->m_pCurrentFrame = this;

  //关联相关的View类  
  m_splitUser.CreateView(0, 0, RUNTIME_CLASS(CDlgRun1), nsize, pContext);
  m_splitUser.CreateView(0, 1, RUNTIME_CLASS(CDlgDebug1), nsize, pContext);
  

  m_splitUser.SetActivePane(0, 0);

  return true;
  //return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CUserSplitterWnd::SetSize(int row, int col, int col_size, int row_size)
{
  int size, min;
  m_splitUser.GetColumnInfo(col, size, min);
  m_splitUser.SetColumnInfo(col, col_size, min);
  m_splitUser.GetRowInfo(row, size, min);
  m_splitUser.SetRowInfo(row, row_size, min);
  m_splitUser.RecalcLayout();
}
