// MySplitterWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "MySplitterWnd.h"
#include <math.h>

// CMySplitterWnd

IMPLEMENT_DYNAMIC(CMySplitterWnd, CSplitterWndEx)

CMySplitterWnd::CMySplitterWnd()
{
  m_cxSplitter = 4;    //must >=4 ,�϶�ʱ�϶����Ŀ��
  m_cySplitter = 4;
  m_cxBorderShare = -4; //�������ʱ�϶�����ƫ����
  m_cyBorderShare = -4;
  m_cxSplitterGap = 1;  //splitter�϶����Ŀ��
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



// CMySplitterWnd ��Ϣ�������




void CMySplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
  ::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
  CWnd::OnLButtonDown(nFlags, point);
  //CSplitterWndEx::OnLButtonDown(nFlags, point);
}


void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
  CWnd::OnMouseMove(nFlags, point);
  //CSplitterWndEx::OnMouseMove(nFlags, point);
}

void CMySplitterWnd::SetCx(int cx)
{
  m_cxSplitterGap = cx;
}




void CMySplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
  // TODO: �ڴ����ר�ô����/����û���
  //if (pDC) {
  //  CBrush brush;
  //  brush.CreateSolidBrush(RGB(255, 255, 255));
  //  pDC->FillRect(rect, &brush);
  //  brush.DeleteObject();
  //}
  CSplitterWndEx::OnDrawSplitter(pDC, nType, rect);
}


BOOL CMySplitterWnd::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: �ڴ����ר�ô����/����û���
  m_clsName = AfxRegisterWndClass(0, ::LoadCursor(NULL,
    IDC_ARROW), ::CreateSolidBrush(RGB(255, 95, 17)));
  cs.lpszClass = (const TCHAR*)m_clsName;
  return CSplitterWndEx::PreCreateWindow(cs);
}
