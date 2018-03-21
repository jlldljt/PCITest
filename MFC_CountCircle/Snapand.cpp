// Snapand.cpp : 实现文件
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


// CSnapand 绘图

void CSnapand::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// CSnapand 诊断

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


// CSnapand 消息处理程序
