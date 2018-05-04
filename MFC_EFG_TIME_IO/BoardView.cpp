// Viewboard.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "BoardView.h"

#include "MainFrm.h"

// CViewboard

IMPLEMENT_DYNCREATE(CBoardView, CView)

CBoardView::CBoardView()
{

}

CBoardView::~CBoardView()
{
  m_dc->SelectObject(m_oldbm);
  m_bm->DeleteObject();
  m_dc->DeleteDC();
}

BEGIN_MESSAGE_MAP(CBoardView, CView)
  ON_WM_CREATE()
END_MESSAGE_MAP()


// CViewboard 绘图

void CBoardView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
  /*pDC->TextOut(0, 0, L"0");
  pDC->TextOut(0, 300, L"300");
  pDC->TextOut(0, 600, L"600");
  pDC->TextOut(0, 900, L"900");
  pDC->TextOut(30 << 2, 0, L"30");
  pDC->TextOut(60 << 2, 0, L"60");
  pDC->TextOut(90 << 2, 0, L"90");
  pDC->TextOut(120 << 2, 0, L"120");
  pDC->TextOut(150 << 2, 0, L"150");
  pDC->TextOut(180 << 2, 0, L"180");
  CPen pen, *ppen;
  pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  ppen = (CPen*)pDC->SelectObject(&pen);
  pen.DeleteObject();

  for (int i = 0; i < COUNTER_NUM; i++)
  {
    pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(i << 2, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[0][i]);
    pDC->LineTo(i << 2, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[1][i]);
    pen.DeleteObject();
    pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo((i << 2)+2, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[2][i]);
    pDC->LineTo((i << 2) + 2, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[3][i]);
    pen.DeleteObject();
  }
  
  pDC->SelectObject(ppen);*/

  CRect rect;
  GetClientRect(&rect);
  pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), m_dc, 0, 0, SRCCOPY);


}


// CViewboard 诊断

#ifdef _DEBUG
void CBoardView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBoardView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewboard 消息处理程序

void CBoardView::DrawSin()
{
  CDC* pDC = m_dc;
  CRect rect;
  GetClientRect(&rect);

  pDC->FillSolidRect(&rect, RGB(255, 255, 255));
  pDC->TextOut(0, 0, L"0");
  pDC->TextOut(0, 300, L"300");
  pDC->TextOut(0, 600, L"600");
  pDC->TextOut(0, 900, L"900");
  pDC->TextOut(30 << 2, 0, L"30");
  pDC->TextOut(60 << 2, 0, L"60");
  pDC->TextOut(90 << 2, 0, L"90");
  pDC->TextOut(120 << 2, 0, L"120");
  pDC->TextOut(150 << 2, 0, L"150");
  pDC->TextOut(180 << 2, 0, L"180");
  CPen pen, *ppen;
  pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  ppen = (CPen*)pDC->SelectObject(&pen);
  pen.DeleteObject();

  for (int i = 0; i < SIN_NUM; i++)
  {
    pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(i << 2, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[0][i]);
    pDC->LineTo(i << 2, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[1][i]);
    pen.DeleteObject();
    pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo((i << 2) + 2, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[2][i]);
    pDC->LineTo((i << 2) + 2, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[3][i]);
    pen.DeleteObject();
  }

  pDC->SelectObject(ppen);

  Invalidate();
}

void CBoardView::DrawCircle()
{
  CDC* pDC = m_dc;
  CRect rect;
  GetClientRect(&rect);

  pDC->FillSolidRect(&rect, RGB(255, 255, 255));
  pDC->TextOut(0, 0, L"0");
  pDC->TextOut(0, 200, L"200");
  pDC->TextOut(0, 400, L"400");
  pDC->TextOut(0, 600, L"600");
  pDC->TextOut(200 , 0, L"200");
  pDC->TextOut(400 , 0, L"400");
  pDC->TextOut(600 , 0, L"600");
  CPen pen, *ppen;
  pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  ppen = (CPen*)pDC->SelectObject(&pen);
  pen.DeleteObject();

  for (int i = 0; i < CIRCLE_NUM; i++)
  {
    pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(GetMainFrame()->m_diIntCounterSnap.m_counter.counter[0][i],i);
    pDC->LineTo(GetMainFrame()->m_diIntCounterSnap.m_counter.counter[1][i],i);
    pen.DeleteObject();
  }

  pDC->SelectObject(ppen);

  Invalidate();
}

int CBoardView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  在此添加您专用的创建代码
  CWnd * pWnd = FromHandle(this->m_hWnd);
  CDC* pDC = pWnd->GetDC();
  CRect rect;
  GetClientRect(&rect);
  m_dc = new CDC;
  m_bm = new CBitmap;
  m_dc->CreateCompatibleDC(pDC);
  m_bm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  m_oldbm = m_dc->SelectObject(m_bm);//无这句和上句，对mdcmen操作无效
  m_dc->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(m_dc->m_hDC, 0, 0, NULL);
  pWnd->ReleaseDC(pDC);
  return 0;
}
