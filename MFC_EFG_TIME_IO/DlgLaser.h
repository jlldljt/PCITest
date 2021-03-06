#pragma once


// CDlgLaser 对话框
#include "EfgIO.h"
class CDlgLaser : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLaser)

public:
	CDlgLaser(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgLaser();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LASER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
  CBrush m_brush;
  EFG_ConfigParam *m_param;


public:
  virtual BOOL OnInitDialog();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnBnClickedBtnShowArc();
  afx_msg void OnEnChangeEdtOut3();
  afx_msg void OnEnChangeEdtOut6();
};
