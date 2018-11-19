#pragma once


#define WND_WIDTH 800
#define WND_HIGHT 600
#define XRAY_Y_TIMES 2 //x光图 y方向放大系数
// CViewboard 视图
typedef struct tagOutStr {
  CString str;
  POINT p;
} OutStr;


class CBoardView : public CView
{
	DECLARE_DYNCREATE(CBoardView)

public://protected:
	CBoardView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBoardView();
  CDC *m_dc;
  CBitmap *m_bm, *m_oldbm;
  CDC *m_strdc;
  CBitmap *m_strbm, *m_strOldbm;
  CString m_outStr;
  CDC *m_alldc;
  CBitmap *m_allbm, *m_allOldbm;
  CArray<OutStr, OutStr&>  m_outStrs;
public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	DECLARE_MESSAGE_MAP()


public:
  void DrawLaserSin();
  void DrawLaserCircle();
  void DrawXRayOneShot();
  void DrawPoint(POINT point, COLORREF color = RGB(0, 0, 255));
  void DrawStr(POINT point, const CString &str);
  void DrawCircle(POINT point, LONG r);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  void Erase(void);
  void SetOutStr(CString str);
  void SetOutStr(CString str, int x, int y);
  void SetOutStr(CString str, POINT p);
  //void ClearOutStr(void);
  void DrawToDC(CDC* pDC);
  void DrawToDC(CDC * pDC, CRect rect);
  //void DrawOutStr(CDC * pDC);
};


