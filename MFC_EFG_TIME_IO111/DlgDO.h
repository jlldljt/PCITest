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
  void LoadParam();
  void Stop(void);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnBnClickedButtonCreate();
  afx_msg void OnBnClickedButtonStart();
};


