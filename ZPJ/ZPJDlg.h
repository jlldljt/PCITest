
// ZPJDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
//#include "Run.h"
//#include "Debug.h"

// CBERDlg 对话框
class CBERDlg : public CDialogEx
{
// 构造
public:
	CBERDlg(CWnd* pParent = NULL);	// 标准构造函数
//=========自定义===========================	
	//有dlg类操作的
	BOOL SYSInit();
	BOOL DBGInit();
	BOOL SYSQuit();
	void SurfaceRefresh();
	void IIICStartConnect();
	void BtnCtl(bool ctl);
//==========================
// 对话框数据
	enum { IDD = IDD_BER_DIALOG };

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
//	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
