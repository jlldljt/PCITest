#pragma once


// CStreamView ��ͼ

class CStreamView : public CView
{
	DECLARE_DYNCREATE(CStreamView)

protected:
	CStreamView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CStreamView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
};


