#pragma once
#include "afxcmn.h"


// CDebug 对话框

class CDebug : public CDialogEx
{
	DECLARE_DYNAMIC(CDebug)

public:
	CDebug(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDebug();

// 对话框数据
	enum { IDD = IDD_DEBUG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabDebug;
	afx_msg void OnTcnSelchangeTabDebug(NMHDR *pNMHDR, LRESULT *pResult);
};

extern CDebug *g_dlgDebug;