#pragma once


// CDlgPriview 对话框

class CDlgPriview : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPriview)

public:
	CDlgPriview(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPriview();

// 对话框数据
	enum { IDD = IDD_DLG_PRIVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  virtual BOOL OnInitDialog();
//  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
