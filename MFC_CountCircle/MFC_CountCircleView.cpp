// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MFC_CountCircleView.cpp : CMFC_CountCircleView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFC_CountCircle.h"
#endif

#include "MFC_CountCircleDoc.h"
#include "MFC_CountCircleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_CountCircleView

IMPLEMENT_DYNCREATE(CMFC_CountCircleView, CView)

BEGIN_MESSAGE_MAP(CMFC_CountCircleView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_CountCircleView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC_CountCircleView 构造/析构

CMFC_CountCircleView::CMFC_CountCircleView()
{
	// TODO: 在此处添加构造代码

}

CMFC_CountCircleView::~CMFC_CountCircleView()
{
}

BOOL CMFC_CountCircleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFC_CountCircleView 绘制

void CMFC_CountCircleView::OnDraw(CDC* /*pDC*/)
{
	CMFC_CountCircleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFC_CountCircleView 打印


void CMFC_CountCircleView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_CountCircleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFC_CountCircleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFC_CountCircleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFC_CountCircleView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC_CountCircleView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC_CountCircleView 诊断

#ifdef _DEBUG
void CMFC_CountCircleView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_CountCircleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_CountCircleDoc* CMFC_CountCircleView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_CountCircleDoc)));
	return (CMFC_CountCircleDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_CountCircleView 消息处理程序
