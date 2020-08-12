
// BICALDebugDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CBICALDebugDlg 对话框
class CBICALDebugDlg : public CDialogEx
{
// 构造
public:
	CBICALDebugDlg(CWnd* pParent = NULL);	// 标准构造函数
	//=========自定义===========================	
	BOOL SYSInit();
	BOOL SYSQuit();
	void SurfaceRefresh();
// 对话框数据
	enum { IDD = IDD_BICALDEBUG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabMain;
	afx_msg void OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
};
