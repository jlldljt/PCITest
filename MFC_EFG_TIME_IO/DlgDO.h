#pragma once



// CDlgDO ������ͼ

class CDlgDO : public CFormView
{
	DECLARE_DYNCREATE(CDlgDO)

protected:
	CDlgDO();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDlgDO();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DO };
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
  CString m_devName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


