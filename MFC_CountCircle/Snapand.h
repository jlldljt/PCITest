#pragma once


// CSnapand ��ͼ

class CSnapand : public CView
{
	DECLARE_DYNCREATE(CSnapand)

protected:
	CSnapand();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSnapand();

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


