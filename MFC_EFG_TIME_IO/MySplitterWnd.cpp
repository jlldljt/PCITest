// MySplitterWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "MySplitterWnd.h"


// CMySplitterWnd

IMPLEMENT_DYNAMIC(CMySplitterWnd, CSplitterWndEx)

CMySplitterWnd::CMySplitterWnd()
{
  m_cxSplitter = 4;    //must >=4 ,�϶�ʱ�϶����Ŀ���
  m_cySplitter = 4;
  m_cxBorderShare = 0; //�������ʱ�϶�����ƫ����
  m_cyBorderShare = 0;
  m_cxSplitterGap = 1;  //splitter�϶����Ŀ���
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



// CMySplitterWnd ��Ϣ��������




void CMySplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ

  //CSplitterWndEx::OnLButtonDown(nFlags, point);
}


void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ

  //CSplitterWndEx::OnMouseMove(nFlags, point);
}