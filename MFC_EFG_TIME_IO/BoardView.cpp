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
  //pDC->BitBlt(rect.left, rect.top + rect.Height(), rect.Width(), -1*rect.Height(), m_dc, 0, 0, SRCCOPY);
  pDC->StretchBlt(rect.left, rect.top + rect.Height(), rect.Width(), -1 * rect.Height(), m_dc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
  
  
  CFont font, *tmp;
  font.CreateFont(15,                                          //   nHeight     
    10,                                                   //   nWidth     
    0,                                                   //   nEscapement   
    0,                                                   //   nOrientation     
    FW_NORMAL,                                   //   nWeight     
    FALSE,                                           //   bItalic     
    FALSE,                                           //   bUnderline     
    0,                                                   //   cStrikeOut     
    ANSI_CHARSET,                             //   nCharSet     
    OUT_DEFAULT_PRECIS,                 //   nOutPrecision     
    CLIP_DEFAULT_PRECIS,               //   nClipPrecision     
    DEFAULT_QUALITY,                       //   nQuality     
    DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily       
    _T("宋体"));

  tmp = pDC->SelectObject(&font);
  pDC->SetBkMode(TRANSPARENT);
  pDC->TextOut(0, 0, L"每格100");
  pDC->TextOut(0, 50, m_outStr);
  
  pDC->SelectObject(tmp);
  DeleteObject(font);

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

void CBoardView::DrawLaserSin()
{
  CDC* pDC = m_dc;
  CRect rect;
  GetClientRect(&rect);

  pDC->FillSolidRect(&rect, RGB(255, 255, 255));
  pDC->TextOut(0, 0, L"-");
  pDC->TextOut(0, 100, L"-");
  pDC->TextOut(0, 200, L"-");
  pDC->TextOut(0, 300, L"-");
  pDC->TextOut(100 << 2, 0, L"|");
  pDC->TextOut(200 << 2, 0, L"|");
  pDC->TextOut(300 << 2, 0, L"|");
  CPen pen, *ppen;
  pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  ppen = (CPen*)pDC->SelectObject(&pen);
  pen.DeleteObject();

  for (int i = 0; i < LASER_SIN_NUM; i++)
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

  //Invalidate();
}

void CBoardView::DrawLaserCircle()
{
  CDC* pDC = m_dc;
  CRect rect;
  GetClientRect(&rect);

  pDC->FillSolidRect(&rect, RGB(255, 255, 255));
  pDC->TextOut(0, 0, L"-");
  pDC->TextOut(0, 100, L"-");
  pDC->TextOut(0, 200, L"-");
  pDC->TextOut(0, 300, L"-");
  pDC->TextOut(100 , 0, L"|");
  pDC->TextOut(200 , 0, L"|");
  pDC->TextOut(300 , 0, L"|");
  CPen pen, *ppen;
  pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  ppen = (CPen*)pDC->SelectObject(&pen);
  pen.DeleteObject();

  for (int i = 0; i < LASER_CIRCLE_NUM; i++)
  {
    pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(GetMainFrame()->m_diIntCounterSnap.m_counter.counter[0][i],i);
    pDC->LineTo(GetMainFrame()->m_diIntCounterSnap.m_counter.counter[1][i],i);
    pen.DeleteObject();
  }

  pDC->SelectObject(ppen);

  //Invalidate();
}

void CBoardView::DrawXRayOneShot()
{
  CDC* pDC = m_dc;
  CRect rect;
  GetClientRect(&rect);

  pDC->FillSolidRect(&rect, RGB(255, 255, 255));
  pDC->TextOut(0, 0, L"");
  pDC->TextOut(0, 100, L"¯");
  pDC->TextOut(0, 200, L"¯");
  pDC->TextOut(0, 300, L"¯");
  pDC->TextOut(0, 400, L"¯");
  pDC->TextOut(0, 500, L"¯");
  pDC->TextOut(0, 600, L"¯");
  pDC->TextOut(100, 0, L"|");
  pDC->TextOut(200, 0, L"|");
  pDC->TextOut(300, 0, L"|");
  pDC->TextOut(400, 0, L"|");
  pDC->TextOut(500, 0, L"|");
  pDC->TextOut(600, 0, L"|");
  CPen pen, *ppen;
  pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  ppen = (CPen*)pDC->SelectObject(&pen);
  pen.DeleteObject();

  for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
  {
    //TODO：调试
//#ifdef __DEBUG__
//    pDC->SetPixel(i, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[0][i], RGB(0, 255, 0));
//    pDC->SetPixel(i, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[1][i], RGB(255, 0, 0));
//    pDC->SetPixel(i, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[2][i], RGB(0, 0, 255));
//
//    continue;
//#endif
    pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(i, 0);
    pDC->LineTo(i, GetMainFrame()->m_diIntCounterSnap.m_counter.fit[0][i]);
    pen.DeleteObject();
   /* pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(i, 300);
    pDC->LineTo(i, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[1][i] + 300);
    pen.DeleteObject();*/
  }

  pDC->SelectObject(ppen);

  //Invalidate();
}

void CBoardView::DrawPoint(POINT point, COLORREF color)
{
  CDC* pDC = m_dc;
  pDC->SetPixel(point.x,point.y, color);
  //Invalidate();
}
void CBoardView::DrawCircle(POINT point, LONG r)
{
  CDC* pDC = m_dc;
  CBrush brush;
  CBrush *oldbrush;
  // brush.CreateSolidBrush(RGB(0xD1, 0xFB, 0xED));
  // oldbrush = pDC->SelectObject(&brush);
  oldbrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
  RECT rect = { point.x - r,point.y -r, point.x + r,point.y + r };
  pDC->Ellipse(&rect);
  pDC->SelectObject(oldbrush);
  //Invalidate();
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
void CBoardView::Erase(void)
{
  CDC* pDC = m_dc;
  CRect rect;
  GetClientRect(&rect);
  pDC->FillSolidRect(&rect, RGB(0, 0, 0));
  m_outStr = "";
  Invalidate();
}

void CBoardView::SetOutStr(CString str)
{
  m_outStr = str;
}
