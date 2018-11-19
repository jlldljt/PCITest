#pragma once
///////////////////////////////////////////滚动条//////////////////////////////////////////////

#define HORZ_PTS 1
#define VERT_PTS 1
#define IDC_SCROLL                    25000
#include "MySplitterWnd.h"
// CSplitFrameWnd 框架

class CSplitFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CSplitFrameWnd)
public://protected:
	CSplitFrameWnd();           // 动态创建所使用的受保护的构造函数
	virtual ~CSplitFrameWnd();

  // 特性
public:
  CMySplitterWnd m_splitWndEx;

protected:
	DECLARE_MESSAGE_MAP()
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
  afx_msg void OnDestroy();
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
  afx_msg void OnClose();

public:
  // 产生滚动条的相关函数和变量
  int  m_nHorzInc, m_nVertInc,
    m_nVscrollMax, m_nHscrollMax,//最大滚动位置
    m_nVscrollPos, m_nHscrollPos;//当前滚动位置

  CRect m_ClientRect;
  CRect m_TabRect;
  CScrollBar m_Scroll;

  void ChangeSize(CRect rect);

  void ScrollInit(void);
  void SetupScrollbars();
  void ResetScrollbars();

//  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//  afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
//  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


