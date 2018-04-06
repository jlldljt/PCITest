// Viewboard.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "BoardView.h"

#include "MainFrm.h"

// CViewboard

IMPLEMENT_DYNCREATE(CBoardView, CView)

CBoardView::CBoardView()
{

}

CBoardView::~CBoardView()
{
  this;
}

BEGIN_MESSAGE_MAP(CBoardView, CView)
END_MESSAGE_MAP()


// CViewboard 绘图

void CBoardView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
  pDC->TextOut(50, 50, L"好好好好好");

  CPen pen, *ppen;
  pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  ppen = (CPen*)pDC->SelectObject(&pen);
  pen.DeleteObject();

  for (int i = 0; i < COUNTER_NUM; i++)
  {
    pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(i << 2, GetMainFrame()->m_counter.counter[0][i]);
    pDC->LineTo(i << 2, GetMainFrame()->m_counter.counter[1][i]);
    pen.DeleteObject();
    pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo((i << 2)+2, GetMainFrame()->m_counter.counter[2][i]);
    pDC->LineTo((i << 2) + 2, GetMainFrame()->m_counter.counter[3][i]);
    pen.DeleteObject();
  }
  
  pDC->SelectObject(ppen);

}


// CViewboard 诊断

#ifdef _DEBUG
void CBoardView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBoardView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewboard 消息处理程序
