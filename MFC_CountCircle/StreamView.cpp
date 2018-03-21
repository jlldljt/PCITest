// StreamView.cpp : ʵ���ļ�
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


// CStreamView ��ͼ

void CStreamView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  �ڴ���ӻ��ƴ���
  ((CMainFrame*)AfxGetMainWnd())->m_capControl.RePaint();
  
}


// CStreamView ���

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


// CStreamView ��Ϣ�������


void CStreamView::OnSize(UINT nType, int cx, int cy)
{
  CView::OnSize(nType, cx, cy);

  // TODO: �ڴ˴������Ϣ����������
  CMainFrame* pwnd= NULL;
  RECT rc = { 0, 0, cx, cy };
  pwnd = (CMainFrame*)AfxGetMainWnd();
  if(pwnd)
    pwnd->m_capControl.SetVideoWindowPosition(rc);
}
