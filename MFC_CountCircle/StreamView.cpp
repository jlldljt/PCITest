// StreamView.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_CountCircle.h"
#include "StreamView.h"
#include "MainFrm.h"

// CStreamView

IMPLEMENT_DYNCREATE(CStreamView, CView)

CStreamView::CStreamView()
{

}

CStreamView::~CStreamView()
{
}

BEGIN_MESSAGE_MAP(CStreamView, CView)
  ON_WM_SIZE()
END_MESSAGE_MAP()


// CStreamView 绘图

void CStreamView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
  ((CMainFrame*)AfxGetMainWnd())->m_capControl.RePaint();
  
}


// CStreamView 诊断

#ifdef _DEBUG
void CStreamView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CStreamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStreamView 消息处理程序


void CStreamView::OnSize(UINT nType, int cx, int cy)
{
  CView::OnSize(nType, cx, cy);

  // TODO: 在此处添加消息处理程序代码
  CMainFrame* pwnd= NULL;
  RECT rc = { 0, 0, cx, cy };
  pwnd = (CMainFrame*)AfxGetMainWnd();
  if(pwnd)
    pwnd->m_capControl.SetVideoWindowPosition(rc);
}
