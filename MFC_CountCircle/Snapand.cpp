// Snapand.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_CountCircle.h"
#include "Snapand.h"


// CSnapand

IMPLEMENT_DYNCREATE(CSnapand, CView)

CSnapand::CSnapand()
{

}

CSnapand::~CSnapand()
{
}

BEGIN_MESSAGE_MAP(CSnapand, CView)
END_MESSAGE_MAP()


// CSnapand ��ͼ

void CSnapand::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  �ڴ���ӻ��ƴ���
}


// CSnapand ���

#ifdef _DEBUG
void CSnapand::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSnapand::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSnapand ��Ϣ�������
