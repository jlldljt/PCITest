// Viewboard.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "Viewboard.h"


// CViewboard

IMPLEMENT_DYNCREATE(CViewboard, CView)

CViewboard::CViewboard()
{

}

CViewboard::~CViewboard()
{
  this;
}

BEGIN_MESSAGE_MAP(CViewboard, CView)
END_MESSAGE_MAP()


// CViewboard ��ͼ

void CViewboard::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  �ڴ���ӻ��ƴ���
 // pDC->TextOut(50, 50, L"�úúúú�");
}


// CViewboard ���

#ifdef _DEBUG
void CViewboard::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewboard::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewboard ��Ϣ�������
