// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MFC_EFG_TIME_IOView.cpp : CMFC_EFG_TIME_IOView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_EFG_TIME_IOView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFC_EFG_TIME_IOView ����/����

CMFC_EFG_TIME_IOView::CMFC_EFG_TIME_IOView()
{
	// TODO: �ڴ˴���ӹ������

}

CMFC_EFG_TIME_IOView::~CMFC_EFG_TIME_IOView()
{
}

BOOL CMFC_EFG_TIME_IOView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFC_EFG_TIME_IOView ����

void CMFC_EFG_TIME_IOView::OnDraw(CDC* /*pDC*/)
{
	//CMFC_EFG_TIME_IODoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;
  CDocument* pDoc = GetDocument();
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMFC_EFG_TIME_IOView ��ӡ


void CMFC_EFG_TIME_IOView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_EFG_TIME_IOView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFC_EFG_TIME_IOView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFC_EFG_TIME_IOView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CMFC_EFG_TIME_IOView ���

#ifdef _DEBUG
void CMFC_EFG_TIME_IOView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_EFG_TIME_IOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//CMFC_EFG_TIME_IODoc* CMFC_EFG_TIME_IOView::GetDocument() const // �ǵ��԰汾��������
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_EFG_TIME_IODoc)));
//	return (CMFC_EFG_TIME_IODoc*)m_pDocument;
//}
#endif //_DEBUG


// CMFC_EFG_TIME_IOView ��Ϣ�������
