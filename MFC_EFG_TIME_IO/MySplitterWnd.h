#pragma once


// CMySplitterWnd

class CMySplitterWnd : public CSplitterWndEx
{
	DECLARE_DYNAMIC(CMySplitterWnd)

public:
	CMySplitterWnd();
	virtual ~CMySplitterWnd();

protected:
	DECLARE_MESSAGE_MAP()
  CString m_clsName;  //´°¿ÚÀàÃû
public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);

  void SetCx(int cx);
  virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


