#pragma once



// CDlgT1 ������ͼ

class CDlgT1 : public CFormView
{
	DECLARE_DYNCREATE(CDlgT1)

protected:
	CDlgT1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDlgT1();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_T1 };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
  int index;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


