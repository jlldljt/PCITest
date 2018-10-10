// DlgRun1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgRun1.h"


// CDlgRun1

IMPLEMENT_DYNCREATE(CDlgRun1, CFormView)

CDlgRun1::CDlgRun1()
	: CFormView(CDlgRun1::IDD)
{

}

CDlgRun1::~CDlgRun1()
{
}

void CDlgRun1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgRun1, CFormView)
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgRun1 ���

#ifdef _DEBUG
void CDlgRun1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgRun1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgRun1 ��Ϣ�������


HBRUSH CDlgRun1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  �ڴ˸��� DC ���κ�����
  if (pWnd == this) {
    return m_brush;
  }
  else if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN /*|| nCtlColor == CTLCOLOR_MAX*/)
  {
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 255));
    return (HBRUSH)::GetStockObject(NULL_BRUSH);
  }
  // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
  return hbr;
}


void CDlgRun1::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  // TODO: �ڴ����ר�ô����/����û���
  CBitmap bmp, sbmp;
  bmp.LoadBitmap(IDB_BK1);   //IDB_BITMAP1��ͼƬ��ԴID
  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);
  bmp.DeleteObject();
}
