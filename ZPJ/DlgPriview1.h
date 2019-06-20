#pragma once


// CDlgPriview1 对话框

class CDlgPriview1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPriview1)

public:
	CDlgPriview1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPriview1();

  int m_n;
// 对话框数据
	enum { IDD = IDD_DLG_PRIVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
  afx_msg
    void Process();
  void OnLButtonDown(UINT nFlags, CPoint point);
  virtual BOOL OnInitDialog();
};
