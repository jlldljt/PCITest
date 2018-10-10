// DlgRun1.cpp : 实现文件
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


// CDlgRun1 诊断

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


// CDlgRun1 消息处理程序


HBRUSH CDlgRun1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  在此更改 DC 的任何特性
  if (pWnd == this) {
    return m_brush;
  }
  else if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN /*|| nCtlColor == CTLCOLOR_MAX*/)
  {
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 255));
    return (HBRUSH)::GetStockObject(NULL_BRUSH);
  }
  // TODO:  如果默认的不是所需画笔，则返回另一个画笔
  return hbr;
}


void CDlgRun1::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  // TODO: 在此添加专用代码和/或调用基类
  CBitmap bmp, sbmp;
  bmp.LoadBitmap(IDB_BK1);   //IDB_BITMAP1是图片资源ID
  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);
  bmp.DeleteObject();
}
