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
  CBrush m_brushBack;
  COLORREF m_color;
  CParamConfig m_config;
public:
  int m_index;
  int m_device;
  void InitDlg(int index);
  void SetDlg(TimeIOType type);
  static void UserFunc(void *param);
  void SaveParam();
  void LoadParam();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButtonCreate();
  afx_msg void OnBnClickedButtonStart();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


