#pragma once


// CDlgXray 对话框
#include "EfgIO.h"
class CDlgXray : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgXray)

public:
	CDlgXray(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgXray();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_XRAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
  CBrush m_brush;


public:
  EFG_ConfigParam *m_param;
  virtual BOOL OnInitDialog();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnBnClickedBtnXrayArc();
  afx_msg void OnEnChangeEdtThreshold();
  afx_msg void OnEnChangeEdtConfirmnum();
  afx_msg void OnEnChangeEdtIgnore();
  afx_msg void OnEnChangeEdtFactorh();
  afx_msg void OnEnChangeEdtFactorw();
//  virtual BOOL PreTranslateMessage(MSG* pMsg);
//  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//  virtual BOOL PreTranslateMessage(MSG* pMsg);
  afx_msg void OnBnClickedBtnTestfit();
};
