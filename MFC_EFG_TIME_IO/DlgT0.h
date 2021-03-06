#pragma once
#include "afxwin.h"



// CDlgT0 窗体视图

class CDlgT0 : public CFormView
{
	DECLARE_DYNCREATE(CDlgT0)

protected:
	CDlgT0();           // 动态创建所使用的受保护的构造函数
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
  CBrush m_brushBack;
  COLORREF m_color;
  CParamConfig m_config;
  CString m_devName;
public:
  int m_index;
  int m_device;
  void InitDlg(int index);
  void SetDlg(TimeIOType type);
  void SaveParam();
  void SaveParamWithDev();
  void LoadParam();
  void Stop(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButtonCreate();
  CComboBox m_comboType;
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnBnClickedButtonStart();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnDestroy();
};


