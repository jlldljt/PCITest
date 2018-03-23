#pragma once



// CDlgT1 窗体视图

class CDlgT1 : public CFormView
{
	DECLARE_DYNCREATE(CDlgT1)

protected:
	CDlgT1();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


