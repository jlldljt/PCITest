// MySplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "MySplitterWnd.h"
#include <math.h>

// CMySplitterWnd

IMPLEMENT_DYNAMIC(CMySplitterWnd, CSplitterWndEx)

CMySplitterWnd::CMySplitterWnd()
{
  m_cxSplitter = 4;    //must >=4 ,拖动时拖动条的宽度
  m_cySplitter = 4;
  m_cxBorderShare = 0; //按下鼠标时拖动条的偏移量
  m_cyBorderShare = 0;
  m_cxSplitterGap = 1;  //splitter拖动条的宽度
  m_cySplitterGap = 1;
}


CMySplitterWnd::~CMySplitterWnd()
{
  this;
}


BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWndEx)
  ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMySplitterWnd 消息处理程序




void CMySplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  //CSplitterWndEx::OnLButtonDown(nFlags, point);
}


void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  //CSplitterWndEx::OnMouseMove(nFlags, point);
}

void CMySplitterWnd::SetCx(int cx)
{
  m_cxSplitterGap = cx;
}




void CMySplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
  // TODO: 在此添加专用代码和/或调用基类
  //if (pDC) {
  //  CBrush brush;
  //  brush.CreateSolidBrush(RGB(255, 255, 255));
  //  pDC->FillRect(rect, &brush);
  //  brush.DeleteObject();
  //}
  CSplitterWndEx::OnDrawSplitter(pDC, nType, rect);
}
