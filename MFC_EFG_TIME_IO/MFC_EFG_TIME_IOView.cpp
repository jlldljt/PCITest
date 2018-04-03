// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MFC_EFG_TIME_IOView.cpp : CMFC_EFG_TIME_IOView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFC_EFG_TIME_IO.h"
#endif

#include "MFC_EFG_TIME_IODoc.h"
#include "MFC_EFG_TIME_IOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_EFG_TIME_IOView

IMPLEMENT_DYNCREATE(CMFC_EFG_TIME_IOView, CView)

BEGIN_MESSAGE_MAP(CMFC_EFG_TIME_IOView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_EFG_TIME_IOView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC_EFG_TIME_IOView 构造/析构

CMFC_EFG_TIME_IOView::CMFC_EFG_TIME_IOView()
{
	// TODO: 在此处添加构造代码

}

CMFC_EFG_TIME_IOView::~CMFC_EFG_TIME_IOView()
{
}

BOOL CMFC_EFG_TIME_IOView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFC_EFG_TIME_IOView 绘制

void CMFC_EFG_TIME_IOView::OnDraw(CDC* /*pDC*/)
{
	//CMFC_EFG_TIME_IODoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;
  CDocument* pDoc = GetDocument();
	// TODO: 在此处为本机数据添加绘制代码
}


// CMFC_EFG_TIME_IOView 打印


void CMFC_EFG_TIME_IOView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_EFG_TIME_IOView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFC_EFG_TIME_IOView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFC_EFG_TIME_IOView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFC_EFG_TIME_IOView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC_EFG_TIME_IOView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC_EFG_TIME_IOView 诊断

#ifdef _DEBUG
void CMFC_EFG_TIME_IOView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_EFG_TIME_IOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//CMFC_EFG_TIME_IODoc* CMFC_EFG_TIME_IOView::GetDocument() const // 非调试版本是内联的
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_EFG_TIME_IODoc)));
//	return (CMFC_EFG_TIME_IODoc*)m_pDocument;
//}
#endif //_DEBUG


// CMFC_EFG_TIME_IOView 消息处理程序
