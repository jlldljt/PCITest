// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MFC_CountCircleView.cpp : CMFC_CountCircleView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_CountCircleView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC_CountCircleView ����/����

CMFC_CountCircleView::CMFC_CountCircleView()
{
	// TODO: �ڴ˴���ӹ������

}

CMFC_CountCircleView::~CMFC_CountCircleView()
{
}

BOOL CMFC_CountCircleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFC_CountCircleView ����

void CMFC_CountCircleView::OnDraw(CDC* /*pDC*/)
{
	CMFC_CountCircleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMFC_CountCircleView ��ӡ


void CMFC_CountCircleView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_CountCircleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFC_CountCircleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFC_CountCircleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CMFC_CountCircleView ���

#ifdef _DEBUG
void CMFC_CountCircleView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_CountCircleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_CountCircleDoc* CMFC_CountCircleView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_CountCircleDoc)));
	return (CMFC_CountCircleDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_CountCircleView ��Ϣ�������
