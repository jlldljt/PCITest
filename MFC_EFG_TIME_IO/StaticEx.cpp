// StaticEx.cpp: 实现文件
//

#include "stdafx.h"
#include "StaticEx.h"
#include "uxtheme.h"
#pragma comment(lib,"UxTheme.lib")

// CStaticEx

IMPLEMENT_DYNAMIC(CStaticEx, CStatic)

CStaticEx::CStaticEx()
{

}

CStaticEx::~CStaticEx()
{
}


BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CStaticEx 消息处理程序




HBRUSH CStaticEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  在此更改 DC 的任何特性

  // TODO:  如果默认的不是所需画笔，则返回另一个画笔
  return hbr;
}


LRESULT CStaticEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  // TODO: 在此添加专用代码和/或调用基类
  if (message == WM_SETTEXT) {
    CRect rect;
    CDC* pDC = GetDC();
    GetClientRect(rect);
    DrawThemeParentBackground(m_hWnd, pDC->GetSafeHdc(), &rect);
    ReleaseDC(pDC);
  }
  return CStatic::DefWindowProc(message, wParam, lParam);
}
