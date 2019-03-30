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

  m_strdc->SelectObject(m_strOldbm);;
  m_strbm->DeleteObject();
  m_strdc->DeleteDC();

  m_alldc->SelectObject(m_allOldbm);;
  m_allbm->DeleteObject();
  m_alldc->DeleteDC();
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

  CRect rect( 0, 0, WND_WIDTH, WND_HIGHT );
  //GetClientRect(&rect);
  //pDC->BitBlt(rect.left, rect.top + rect.Height(), rect.Width(), -1*rect.Height(), m_dc, 0, 0, SRCCOPY);
  
  DrawToDC(pDC, rect);
  //pDC->StretchBlt(rect.left, rect.top + rect.Height(), rect.Width(), -1 * rect.Height(), m_dc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
  //DrawOutStr(pDC);
  //
  //CFont font, *tmp;
  //font.CreateFont(15,                                          //   nHeight     
  //  10,                                                   //   nWidth     
  //  0,                                                   //   nEscapement   
  //  0,                                                   //   nOrientation     
  //  FW_NORMAL,                                   //   nWeight     
  //  FALSE,                                           //   bItalic     
  //  FALSE,                                           //   bUnderline     
  //  0,                                                   //   cStrikeOut     
  //  ANSI_CHARSET,                             //   nCharSet     
  //  OUT_DEFAULT_PRECIS,                 //   nOutPrecision     
  //  CLIP_DEFAULT_PRECIS,               //   nClipPrecision     
  //  DEFAULT_QUALITY,                       //   nQuality     
  //  DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily       
  //  _T("宋体"));

  //tmp = pDC->SelectObject(&font);
  //pDC->SetBkMode(TRANSPARENT);
  //pDC->TextOut(0, 0, L"每格100");
  //pDC->TextOut(0, 50, m_outStr);
  //
  //pDC->SelectObject(tmp);
  //DeleteObject(font);

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
  CRect rect(0, 0, WND_WIDTH, WND_HIGHT);
  //GetClientRect(&rect);
  double sin_num = GetMainFrame()->m_diIntCounterSnap.m_counter.index[0]-1;
  double time_x = (WND_WIDTH) / sin_num;
  pDC->FillSolidRect(&rect, RGB(255, 255, 255));
  int x, y;
  x = 0, y = 100; pDC->MoveTo(x, y); pDC->LineTo(x+10, y); SetOutStr(_T("100"), x+10, y);
  x = 0, y = 200; pDC->MoveTo(x, y); pDC->LineTo(x+10, y); SetOutStr(_T("200"), x+10, y);
  x = 0, y = 300; pDC->MoveTo(x, y); pDC->LineTo(x+10, y); SetOutStr(_T("300"),x+ 10, y);
  //x = 90<<2, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("100"), x, y + 50);
  x = sin_num/4*time_x, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("90°"), x-30, y + 50);
  x = sin_num/2*time_x, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("180°"), x-30, y + 50);
  x = sin_num*3/4*time_x, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("270°"), x-30, y + 50);
  x = sin_num*time_x, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("360°"), x-30, y + 50);

  /*pDC->TextOut(0, 0, L"-");
  pDC->TextOut(0, 100, L"-");
  pDC->TextOut(0, 200, L"-");
  pDC->TextOut(0, 300, L"-");*/
  //pDC->TextOut(100 << 2, 0, L"|");
  //pDC->TextOut(200 << 2, 0, L"|");
  //pDC->TextOut(300 << 2, 0, L"|");
  CPen pen, *ppen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	ppen = (CPen*)pDC->SelectObject(&pen);
	pen.DeleteObject();

  for (int i = 0; i < sin_num; i++)//GetMainFrame()->m_diIntCounterSnap.m_counter.index[0]-1
  {
    pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(i * time_x, GetMainFrame()->m_diIntCounterSnap.m_counter.tmp_counter[0][i]);
    pDC->LineTo(i * time_x, GetMainFrame()->m_diIntCounterSnap.m_counter.tmp_counter[1][i]);
    pen.DeleteObject();
    pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo((i * time_x) + time_x/2, GetMainFrame()->m_diIntCounterSnap.m_counter.tmp_counter[2][i]);
    pDC->LineTo((i * time_x) + time_x/2, GetMainFrame()->m_diIntCounterSnap.m_counter.tmp_counter[3][i]);
    pen.DeleteObject();
  }

  pDC->SelectObject(ppen);

  //Invalidate();
}

void CBoardView::DrawLaserCircle()
{
  CDC* pDC = m_dc;
  CRect rect ( 0, 0, WND_WIDTH, WND_HIGHT );;
  //GetClientRect(&rect);

  pDC->FillSolidRect(&rect, RGB(255, 255, 255));
  int x, y;
  x = 0, y = 100; pDC->MoveTo(x, y); pDC->LineTo(x + 10, y); SetOutStr(_T("100"), x + 10, y);
  x = 0, y = 200; pDC->MoveTo(x, y); pDC->LineTo(x + 10, y); SetOutStr(_T("200"), x + 10, y);
  x = 0, y = 300; pDC->MoveTo(x, y); pDC->LineTo(x + 10, y); SetOutStr(_T("300"), x + 10, y);
  x = 100, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("100"), x, y + 10);
  x = 200, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("200"), x, y + 10);
  x = 300, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("300"), x, y + 10);

  //pDC->TextOut(0, 0, L"-");
  //pDC->TextOut(0, 100, L"-");
  //pDC->TextOut(0, 200, L"-");
  //pDC->TextOut(0, 300, L"-");
  //pDC->TextOut(100 , 0, L"|");
  //pDC->TextOut(200 , 0, L"|");
  //pDC->TextOut(300 , 0, L"|");
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
  CRect rect ( 0, 0, WND_WIDTH, WND_HIGHT );;
  //GetClientRect(&rect);

  pDC->FillSolidRect(&rect, RGB(255, 255, 255));
  int x, y;
  double times = 1.0*(GetMainFrame()->m_diIntCounterSnap.m_counter.index[1]-1)/WND_WIDTH;
  //int deg = (GetMainFrame()->m_diIntCounterSnap.m_counter.index[1]-1)/8;//360°
  x = 0, y = 100*XRAY_Y_TIMES; pDC->MoveTo(x, y); pDC->LineTo(x + 10, y); SetOutStr(_T("100"), x + 10, y);
  x = 0, y = 200*XRAY_Y_TIMES; pDC->MoveTo(x, y); pDC->LineTo(x + 10, y); SetOutStr(_T("200"), x + 10, y);
  //x = 0, y = 300*XRAY_Y_TIMES; pDC->MoveTo(x, y); pDC->LineTo(x + 10, y); SetOutStr(_T("300"), x + 10, y);
  //x = 0, y = 400*XRAY_Y_TIMES; pDC->MoveTo(x, y); pDC->LineTo(x + 10, y); SetOutStr(_T("400"), x + 10, y);
  //x = 0, y = 500*XRAY_Y_TIMES; pDC->MoveTo(x, y); pDC->LineTo(x + 10, y); SetOutStr(_T("500"), x + 10, y);
  x = WND_WIDTH/4, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("90°"), x-30, y + 50);
  x = WND_WIDTH/2, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("180°"), x-30, y + 50);
  x = WND_WIDTH*3/4, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("270°"), x-30, y + 50);
  x = WND_WIDTH, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("360°"), x-30, y + 50);
  //x = 500, y = 0; pDC->MoveTo(x, y); pDC->LineTo(x, y + 10); SetOutStr(_T("500"), x, y + 50);

  //pDC->TextOut(0, 0, L"");
  //pDC->TextOut(0, 100, L"¯");
  //pDC->TextOut(0, 200, L"¯");
  //pDC->TextOut(0, 300, L"¯");
  //pDC->TextOut(0, 400, L"¯");
  //pDC->TextOut(0, 500, L"¯");
  //pDC->TextOut(0, 600, L"¯");
  //pDC->TextOut(100, 0, L"|");
  //pDC->TextOut(200, 0, L"|");
  //pDC->TextOut(300, 0, L"|");
  //pDC->TextOut(400, 0, L"|");
  //pDC->TextOut(500, 0, L"|");
  //pDC->TextOut(600, 0, L"|");
  CPen pen, *ppen;
  pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 250));
  ppen = (CPen*)pDC->SelectObject(&pen);
  pen.DeleteObject();

  for (int i = 0; i < WND_WIDTH/*(GetMainFrame()->m_diIntCounterSnap.m_counter.index[1]-1)/8*/; i++)
  {
    //TODO：调试
//#ifdef __DEBUG__
//    pDC->SetPixel(i, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[0][i], RGB(0, 255, 0));
//    pDC->SetPixel(i, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[1][i], RGB(255, 0, 0));
//    pDC->SetPixel(i, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[2][i], RGB(0, 0, 255));
//
//    continue;
//#endif
    pen.CreatePen(PS_SOLID, 2, RGB(150, 150, 250));
    pDC->SelectObject(&pen);
    pDC->MoveTo(i, 0);

	int lineTo = 0;
	for (int j = 0; j <  times; j++)
	{
		if(lineTo <GetMainFrame()->m_diIntCounterSnap.m_counter.fit[0][int(i*times) + j])
			lineTo =GetMainFrame()->m_diIntCounterSnap.m_counter.fit[0][int(i*times) + j];
	}
	lineTo=lineTo*XRAY_Y_TIMES;
    pDC->LineTo(i, lineTo/*GetMainFrame()->m_diIntCounterSnap.m_counter.fit[0][i]*/);
    
	//pDC->SetPixel(i, lineTo, RGB(0, 0, 250));
	
	pen.DeleteObject();
   /* pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    pDC->SelectObject(&pen);
    pDC->MoveTo(i, 300);
    pDC->LineTo(i, GetMainFrame()->m_diIntCounterSnap.m_counter.counter[1][i] + 300);
    pen.DeleteObject();*/
  }
  for (int i = 0; i < WND_WIDTH/*(GetMainFrame()->m_diIntCounterSnap.m_counter.index[1]-1)/8*/; i++)
  {
	int lineTo = 0;
	for (int j = 0; j <  times; j++)
	{
		if(lineTo <GetMainFrame()->m_diIntCounterSnap.m_counter.fit[0][int(i*times) + j])
			lineTo =GetMainFrame()->m_diIntCounterSnap.m_counter.fit[0][int(i*times) + j];
	}
	lineTo=lineTo*XRAY_Y_TIMES;
	pDC->SetPixel(i, lineTo, RGB(0, 0, 200));
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

void CBoardView::DrawStr(POINT point, const CString &str)
{
  CDC* pDC = m_dc;
  pDC->TextOut(point.x, point.y, str);
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
  CRect rect ( 0, 0, WND_WIDTH, WND_HIGHT );;
  //GetClientRect(&rect);
  m_dc = new CDC;
  m_bm = new CBitmap;
  m_dc->CreateCompatibleDC(pDC);
  m_bm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  m_oldbm = m_dc->SelectObject(m_bm);
  m_dc->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(m_dc->m_hDC, 0, 0, NULL);

  m_strdc = new CDC;
  m_strbm = new CBitmap;
  m_strdc->CreateCompatibleDC(pDC);
  m_strbm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  m_strOldbm = m_strdc->SelectObject(m_strbm);
  m_strdc->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(m_strdc->m_hDC, 0, 0, NULL);

  m_alldc = new CDC;
  m_allbm = new CBitmap;
  m_alldc->CreateCompatibleDC(pDC);
  m_allbm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  m_allOldbm = m_alldc->SelectObject(m_allbm);
  m_alldc->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(m_alldc->m_hDC, 0, 0, NULL);


  pWnd->ReleaseDC(pDC);

  return 0;
}
void CBoardView::Erase(void)
{
  CDC* pDC = m_dc;
  CRect rect ( 0, 0, WND_WIDTH, WND_HIGHT );;
  //GetClientRect(&rect);
  pDC->FillSolidRect(&rect, RGB(0, 0, 0));
  pDC = m_strdc;
  pDC->FillSolidRect(&rect, RGB(0, 0, 0));
  //m_outStr = "";
  SetOutStr(_T(""));
  //ClearOutStr();
  Invalidate();
}

void CBoardView::SetOutStr(CString str)
{
  m_outStr = str;
}

void CBoardView::SetOutStr(CString str, int x, int y)
{
  CDC* pDC = m_strdc;
  CFont font, *tmp;
  font.CreateFont(30,                                          //   nHeight     
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

  pDC->SetTextColor(RGB(255, 0, 0));
  pDC->TextOut(x, WND_HIGHT - y, str);

  pDC->SelectObject(tmp);
  DeleteObject(font);
}

void CBoardView::SetOutStr(CString str, POINT p)
{
  
  SetOutStr(str, p.x, p.y);
  //OutStr out = { str,p };

  //m_outStrs.Add(out);



}

//void CBoardView::ClearOutStr(void)
//{
//  m_outStrs.RemoveAll();
//}

void CBoardView::DrawToDC(CDC * pDC)
{
  CRect rect ( 0, 0, WND_WIDTH, WND_HIGHT );; 
  CRect torect;
  //GetClientRect(&rect);
  pDC->GetWindow()->GetClientRect(&torect);
  pDC->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
  m_alldc->StretchBlt(rect.left, rect.top + rect.Height(), rect.Width(), -1 * rect.Height(), m_dc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
  m_alldc->TransparentBlt(rect.left, rect.top, rect.Width(), rect.Height(), m_strdc, 0, 0, rect.Width(), rect.Height(), RGB(0, 0, 0));
  pDC->StretchBlt(torect.left, torect.top, torect.Width(), torect.Height(), m_alldc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
  //DrawOutStr(pDC);
}

void CBoardView::DrawToDC(CDC * pDC,CRect torect)
{
  CRect rect(0, 0, WND_WIDTH, WND_HIGHT);;
  //CRect torect;
  //GetClientRect(&rect);
  //pDC->GetWindow()->GetClientRect(&torect);



  pDC->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
  m_alldc->StretchBlt(rect.left, rect.top + rect.Height(), rect.Width(), -1 * rect.Height(), m_dc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
  m_alldc->TransparentBlt(rect.left, rect.top, rect.Width(), rect.Height(), m_strdc, 0, 0, rect.Width(), rect.Height(), RGB(0, 0, 0));
  pDC->StretchBlt(torect.left, torect.top, torect.Width(), torect.Height(), m_alldc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

  CBrush* oldbrush = pDC->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
  pDC->Rectangle(torect);
  pDC->SelectObject(oldbrush);

  //DrawOutStr(pDC);
}

//void CBoardView::DrawOutStr(CDC * pDC)
//{
//  CFont font, *tmp;
//  font.CreateFont(15,                                          //   nHeight     
//    10,                                                   //   nWidth     
//    0,                                                   //   nEscapement   
//    0,                                                   //   nOrientation     
//    FW_NORMAL,                                   //   nWeight     
//    FALSE,                                           //   bItalic     
//    FALSE,                                           //   bUnderline     
//    0,                                                   //   cStrikeOut     
//    ANSI_CHARSET,                             //   nCharSet     
//    OUT_DEFAULT_PRECIS,                 //   nOutPrecision     
//    CLIP_DEFAULT_PRECIS,               //   nClipPrecision     
//    DEFAULT_QUALITY,                       //   nQuality     
//    DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily       
//    _T("宋体"));
//
//  tmp = pDC->SelectObject(&font);
//  pDC->SetBkMode(TRANSPARENT);
//  //pDC->TextOut(0, 0, L"每格100");
//  pDC->TextOut(0, 50, m_outStr);
//  for (int i = 0; i < m_outStrs.GetCount(); i++)
//  {
//    OutStr out = m_outStrs[i];
//    pDC->SetTextColor(RGB(255, 255,255));
//    pDC->TextOut(out.p.x, out.p.y, out.str);
//  }
//
//  pDC->SelectObject(tmp);
//  DeleteObject(font);
//}