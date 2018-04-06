#pragma once


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
};


