#pragma once


#include "MySplitterWnd.h"
// CUserSplitterWnd 框架

class CUserSplitterWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CUserSplitterWnd)
public://protected:
	CUserSplitterWnd();           // 动态创建所使用的受保护的构造函数
	virtual ~CUserSplitterWnd();

protected:
	DECLARE_MESSAGE_MAP()


public:
  CMySplitterWnd m_splitUser;
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

  void SetSize(int col, int row, int col_size, int row_size);
};


