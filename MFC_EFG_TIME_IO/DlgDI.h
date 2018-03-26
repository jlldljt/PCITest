#pragma once



// CDlgDI 窗体视图

class CDlgDI : public CFormView
{
	DECLARE_DYNCREATE(CDlgDI)

protected:
	CDlgDI();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


