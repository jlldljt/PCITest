// StaticDraw.cpp : 实现文件
//

#include "stdafx.h"
#include "StaticDraw.h"


// CStaticDraw

//IMPLEMENT_DYNAMIC(CStaticDraw, CStatic)

bool CStaticDraw::AddLine(COLORREF crColor, int nWidth)
{
	if (RGB(0, 0, 0) == crColor || nWidth <= 0)
		return false;

	stLine line;
	line.clr = crColor;
	line.val = nullptr;
	line.preVal = 0;
	line.w = nWidth;
	lsLine.AddTail(line);

	return true;
}

bool CStaticDraw::AddLineEx(COLORREF crColor, int nWidth, int * pnVal)
{
	if (nWidth <= 0)
		return false;

	if (RGB(0, 0, 0) == crColor)
		crColor = RGB(1, 1, 1);

	stLine line;
	line.clr = crColor;

	if (nullptr == pnVal)
	{
		line.val = nullptr;
	}
	else
	{
		line.val = pnVal;
	}

	line.preVal = 0;
	line.w = nWidth;
	lsLine.AddTail(line);

	return true;
}


bool CStaticDraw::AddPoint(COLORREF crColor, int nWidth, POINTF pt)
{
	stPoint tmpPt;

	tmpPt.clr = crColor;

	tmpPt.pt = pt;

	tmpPt.w = nWidth;

	arrPoint.Add(tmpPt);

	return true;
}

int CStaticDraw::GetLineNum(void)
{
	int num = lsLine.GetCount();
	return num;
}

bool CStaticDraw::SetBG(COLORREF crBg, COLORREF crLine, int nGridX, int nGridY, bool show)
{
	bgClr = crBg;
	bgLineClr = crLine;
	showBgLine = show;
	bgXLineNum = nGridX;
	bgYLineNum = nGridY;

	CString str;

	////////////背景网格绘制////////////////
	CDC *pDC = thisDC;
	CRect rc = thisRect;

	HBRUSH hbr = CreateSolidBrush(bgClr);
	bgDC.FillRect(rc, CBrush::FromHandle(hbr));

	DeleteObject(hbr);

	CPen newPen, *oldPen;
	newPen.CreatePen(PS_SOLID, 1, bgLineClr);
	oldPen = bgDC.SelectObject(&newPen);

	bgDC.SetBkMode(TRANSPARENT);

	if (showBgLine) {
		if (bgXLineNum)
		{

			for (int i = 1; i < bgXLineNum; i++)
			{
				int grid = rc.Width() * i / bgXLineNum;
				bgDC.MoveTo(grid, 0);
				bgDC.LineTo(grid, rc.Height());

				str.Format(_T("%d"), rangeX*i/bgXLineNum);
				bgDC.TextOut(grid, rc.Height() - 50, str);
			}
		}

		if (bgYLineNum)
		{

			for (int i = 1; i < bgYLineNum; i++)
			{
				int grid = rc.Height() * i / bgYLineNum;
				bgDC.MoveTo(0, grid);
				bgDC.LineTo(rc.Width(), grid);

				str.Format(_T("%d"), rangeY*(bgYLineNum-i)/bgYLineNum);
				bgDC.TextOut(0, grid, str);
			}
		}
	}
	bgDC.SelectObject(oldPen);
	newPen.DeleteObject();

	//CString str;
	str.Format(_T("%d"), rangeY);
	//bgDC.SetBkMode(TRANSPARENT);
	bgDC.TextOut(0, 0, str);

	return true;
}

//nLineNo from 0
bool CStaticDraw::SetLineVal(int nLineNo, int* pnVal)
{
	stLine line;
	POSITION pos;
	pos = lsLine.FindIndex(nLineNo);
	if (NULL == pos)
		return false;

	line = lsLine.GetAt(pos);
	line.val = pnVal;
	lsLine.SetAt(pos, line);

	return true;
}

bool CStaticDraw::SetScrollPixel(int nPixel)
{
	if (nPixel < 0)
		return false;

	scrollPixel = nPixel;
	return true;
}

bool CStaticDraw::SetUpdateTime(int nTime)
{
	updateTime = nTime;
	if (updateTime)
	{
		//这里必须使用父窗口类来设置，控件窗口设置后无效
		if (m_pWndParent->SetTimer(DRAW_LINE, updateTime, NULL))
			return true;
	}
	else
	{
		if (m_pWndParent->KillTimer(DRAW_LINE))
			return true;
	}

	return false;
}

bool CStaticDraw::UpdateManual(void)
{
	CDC *pDC = thisDC;
	CRect rc = thisRect;
	//memDC.BitBlt(0, 0, rc.Width(), rc.Height(), &bgDC, 0, 0, SRCCOPY);
	memDC.StretchBlt(0, 0, rc.Width(), rc.Height(), &bgDC, 0, rc.Height(), rc.Width(), -rc.Height(), SRCCOPY);
	memDC.StretchBlt(0, 0, rc.Width(), rc.Height(), &pointDC, m_rcPoint.Width()*m_showPercent/100, rc.Height(), rc.Width(), -rc.Height(), SRCCOPY);
	//memDC.TransparentBlt(0, 0, rc.Width(), rc.Height(), &pointDC, 0, 0, rc.Width(), rc.Height(), RGB(0, 0, 0));
	
	//memDC.TransparentBlt(0, 0, rc.Width(), rc.Height(), &pointDC, 0, rc.Height(), rc.Width(), -rc.Height(), RGB(0, 0, 0));
	
	memDC.TransparentBlt(0, 0, rc.Width(), rc.Height(), &lineDC, 0, 0, rc.Width(), rc.Height(), RGB(0, 0, 0));
	m_pWnd->InvalidateRect(rc, 0);
	return false;
}

bool CStaticDraw::SetRange(int nX, int nY)
{
	if (nX <= 0 || nY <= 0)
	{
		return false;
	}

	CDC  *pDC = thisDC;
	CRect rc = thisRect;
	CDC tmpDC;
	CBitmap tmpBM;
	tmpDC.CreateCompatibleDC(pDC);
	tmpBM.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
	tmpDC.SelectObject(&tmpBM);

	if (rangeY)
	{
		tmpDC.SetStretchBltMode(WHITEONBLACK);
		tmpDC.StretchBlt(0, 0, rc.Width(), rc.Height() * rangeY / nY, &lineDC, 0, 0, rc.Width(), rc.Height(), SRCCOPY);
	}
	else
		tmpDC.BitBlt(0, 0, rc.Width(), rc.Height(), &lineDC, 0, 0, SRCCOPY);

	lineDC.BitBlt(0, 0, rc.Width(), rc.Height(), &tmpDC, 0, 0, SRCCOPY);
	tmpBM.DeleteObject();
	tmpDC.DeleteDC();
	

	rangeX = nX;
	rangeY = nY;

	return true;
}

CStaticDraw::CStaticDraw(CWnd * pWnd)
{
	m_pWndParent = pWnd;
	m_showPercent = 0;
}

CStaticDraw::~CStaticDraw()
{
	this->DestroyWindow();
}

//
//BEGIN_MESSAGE_MAP(CStaticDraw, CStatic)
//	ON_WM_TIMER()
//	ON_WM_CTLCOLOR()
//	ON_WM_CREATE()
//END_MESSAGE_MAP()



// CStaticDraw 消息处理程序




void CStaticDraw::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{

	// TODO:  添加您的代码以绘制指定项
	CDC  *pDC = thisDC;
	CRect rc = thisRect;

	pDC->StretchBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, rc.Height(), rc.Width(), -rc.Height(), SRCCOPY);
}


//void CStaticDraw::PreSubclassWindow()
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	DWORD dwStyle = GetStyle();
//	SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_OWNERDRAW | WS_BORDER);
//	CStatic::PreSubclassWindow();
//}


void CStaticDraw::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case DRAW_LINE:
		{
			CDC  *pDC = thisDC;
			CRect rc = thisRect;
			stLine line;
			POSITION pos;
			int nowX = scrollX;
			int nextX = scrollX + scrollPixel;

			int grid = scrollPixel;

			if (bgXLineNum)
			{
				grid = rc.Width() / bgXLineNum;
			}

			if (grid < scrollPixel)
				grid = scrollPixel;

			if (nextX >= rc.Width())
			{
				CDC tmpDC;
				CBitmap tmpBM;
				tmpDC.CreateCompatibleDC(pDC);
				tmpBM.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
				tmpDC.SelectObject(&tmpBM);

				tmpDC.BitBlt(0, 0, rc.Width(), rc.Height(), &lineDC, grid/*scrollPixel*/, 0, SRCCOPY);

				nowX -= grid/*scrollPixel*/;
				nextX -= grid/*scrollPixel*/;

				lineDC.BitBlt(0, 0, rc.Width(), rc.Height(), &tmpDC, 0, 0, SRCCOPY);
				tmpBM.DeleteObject();
				tmpDC.DeleteDC();
			}

			scrollX = nextX;
			pos = lsLine.GetHeadPosition();
			while (NULL != pos)
			{
				POSITION tmpPos = pos;
				line = lsLine.GetNext(pos);
				int nowY = line.preVal * rc.Height() / rangeY;
				int nextY = nowY;

				if (nullptr != line.val)
				{
					nextY = *line.val * rc.Height() / rangeY;
					line.preVal = *line.val;
				}

				lsLine.SetAt(tmpPos, line);


				CPen newPen, *oldPen;

				newPen.CreatePen(PS_SOLID, line.w, line.clr);
				oldPen = lineDC.SelectObject(&newPen);

				lineDC.MoveTo(nowX, nowY);
				lineDC.LineTo(nextX, nextY);
				lineDC.SelectObject(oldPen);
				newPen.DeleteObject();
			}
			UpdateManual();
		}
		break;
	default:
		break;
	}
	//OnTimer(nIDEvent);
}

void CStaticDraw::DrawPoint(void)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDC  *pDC = thisDC;
	int count = arrPoint.GetCount();
	CRect rc = CRect(0,0,thisRect.right*count/rangeX, thisRect.bottom);

	m_rcPoint = rc;

	POINT pt;

	//////////////////
	///////////////调整point的长度
	pointBM.DeleteObject();
	pointDC.DeleteDC();

	pointDC.CreateCompatibleDC(pDC);
	pointBM.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
	pointDC.SelectObject(&pointBM);
	//////////////////
	pointDC.FillSolidRect(rc,RGB(255,255,255));

	pt.x = arrPoint[0].pt.x * rc.Width() / count;
	pt.y = rc.Height() - arrPoint[0].pt.y * rc.Height() / rangeY;

	pointDC.MoveTo(pt);
	//pointDC.TextOut(20, 20, _T("A123"));
	pointDC.SetBkMode(TRANSPARENT);

	for (int i = 0; i < count; i++)
	{
		//pt = arrPoint[i].pt;
		CPen newPen, *oldPen;
		newPen.CreatePen(PS_SOLID, 1/*arrPoint[i].w*/, arrPoint[i].clr);
		oldPen = pointDC.SelectObject(&newPen);

		CBrush newBrush(arrPoint[i].clr),* oldBrush;
		oldBrush = pointDC.SelectObject(&newBrush);

		pointDC.SetTextColor(arrPoint[i].clr);

		pt.x = arrPoint[i].pt.x * rc.Width() / count;
		pt.y = rc.Height() - arrPoint[i].pt.y * rc.Height() / rangeY;

		int w = arrPoint[i].w / 2 ;
		// 线
		pointDC.LineTo(pt);
		// 点
		pointDC.Ellipse(pt.x-w, pt.y -w, pt.x +w, pt.y+w);
		// 字
		CString str;
		str.Format(_T("%.3f"), arrPoint[i].pt.y);
		pointDC.TextOut(pt.x +w, pt.y+w, str);
		//pointDC.TextOut(-10, 10, _T("X123"));
		pointDC.SelectObject(oldPen);
		pointDC.SelectObject(oldBrush);
		newPen.DeleteObject();
		newPen.DeleteObject();
		str.Format(_T("%d"), i);
		pointDC.TextOut(pt.x, rc.Height() - 20, str);
	}

	UpdateManual();

}

BOOL CStaticDraw::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pWndParent->KillTimer(DRAW_LINE);

	memBM.DeleteObject();
	memDC.DeleteDC();
	bgBM.DeleteObject();
	bgDC.DeleteDC();
	lineBM.DeleteObject();
	lineDC.DeleteDC();

	pointBM.DeleteObject();
	pointDC.DeleteDC();
	m_pWnd->ReleaseDC(thisDC);
	lsLine.RemoveAll();
	arrPoint.RemoveAll();

	return m_pWnd->DestroyWindow();
}



int CStaticDraw::OnCreate(LPCREATESTRUCT lpCreateStruct,CRect rcCreate)
{

	HWND pWnd = CreateWindow(_TEXT("Static"), _TEXT("Static"), WS_THICKFRAME|WS_CHILD | SS_OWNERDRAW, rcCreate.left, rcCreate.top, rcCreate.Width(), rcCreate.Height(),m_pWndParent->m_hWnd, (HMENU)1, lpCreateStruct->hInstance, NULL);

	if (!pWnd)
		return -1;

	m_pWnd = CWnd::FromHandle(pWnd);

	m_pWnd->ShowWindow(TRUE);
	// TODO:  在此添加您专用的创建代码

	SetScrollPixel(1);

	thisDC = m_pWnd->GetDC();

	m_pWnd->GetClientRect(&thisRect);

	CDC *pDC = thisDC;

	CRect rc = thisRect;

	memDC.CreateCompatibleDC(pDC);
	memBM.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
	memDC.SelectObject(&memBM);

	lineDC.CreateCompatibleDC(pDC);
	lineBM.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
	lineDC.SelectObject(&lineBM);

	pointDC.CreateCompatibleDC(pDC);
	pointBM.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
	pointDC.SelectObject(&pointBM);

	bgDC.CreateCompatibleDC(pDC);
	bgBM.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
	bgDC.SelectObject(&bgBM);

	SetRange(rc.Width(), rc.Height());

	return 0;
}
BOOL CStaticDraw::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int step = - zDelta / 120;

	m_showPercent += step;

	if(m_showPercent < 0 )
		m_showPercent = 0;
	else if(m_showPercent > 100)
		m_showPercent = 100;

	UpdateManual();

	return TRUE;
}