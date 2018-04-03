// Viewboard.cpp : 实现文件
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


// CViewboard 绘图

void CViewboard::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
 // pDC->TextOut(50, 50, L"好好好好好");
}


// CViewboard 诊断

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


// CViewboard 消息处理程序
