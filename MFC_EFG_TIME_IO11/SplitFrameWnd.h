#pragma once


#include "MySplitterWnd.h"
// CSplitFrameWnd ���

class CSplitFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CSplitFrameWnd)
public://protected:
	CSplitFrameWnd();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSplitFrameWnd();

  // ����
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


