#pragma once


// CViewboard ��ͼ

class CViewboard : public CView
{
	DECLARE_DYNCREATE(CViewboard)

public://protected:
	CViewboard();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CViewboard();

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
};


