#pragma once
#include "afxwin.h"



// CDlgT0 ������ͼ

class CDlgT0 : public CFormView
{
	DECLARE_DYNCREATE(CDlgT0)

protected:
	CDlgT0();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDlgT0();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_T0 };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
  int index;
  void InitDlg(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButtonCreate();
  CComboBox m_comboType;
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


