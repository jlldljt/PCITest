#pragma once



// CDlgDI ������ͼ

class CDlgDI : public CFormView
{
	DECLARE_DYNCREATE(CDlgDI)

protected:
	CDlgDI();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDlgDI();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DI };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
  int m_index;
  int m_device;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


