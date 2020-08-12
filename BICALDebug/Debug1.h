#pragma once
#include "afxwin.h"


// CDebug1 对话框

class CDebug1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDebug1)

public:
	CDebug1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDebug1();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEBUG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditCom();
	afx_msg void OnBnClickedButtonTest();
	CEdit m_edtRs232Rcv;
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeXgrid();
	afx_msg void OnEnChangeYgrid();
	afx_msg void OnEnChangeX1();
	afx_msg void OnEnChangeY1();
	afx_msg void OnEnChangeXend();
	afx_msg void OnEnChangeYend();
	afx_msg void OnEnChangeXlimit();
	afx_msg void OnEnChangeYlimit();
	afx_msg void OnEnChangeXmin();
	afx_msg void OnEnChangeYmin();
	afx_msg void OnEnChangeStep();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedButtonSendtext();
};

extern CDebug1 *g_dlgDebug1;