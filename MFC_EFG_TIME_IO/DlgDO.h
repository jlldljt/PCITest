#pragma once



// CDlgDO 窗体视图

class CDlgDO : public CFormView
{
	DECLARE_DYNCREATE(CDlgDO)

protected:
	CDlgDO();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


