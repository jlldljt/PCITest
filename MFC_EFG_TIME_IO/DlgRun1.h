#pragma once



// CDlgRun1 窗体视图

class CDlgRun1 : public CFormView
{
	DECLARE_DYNCREATE(CDlgRun1)

protected:
	CDlgRun1();           // 动态创建所使用的受保护的构造函数
	virtual ~CDlgRun1();

public:
	enum { IDD = IDD_DLG_RUN1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
  CBrush m_brush;
public:
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  virtual void OnInitialUpdate();
};


