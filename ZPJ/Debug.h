#pragma once
#include "afxcmn.h"


// CDebug �Ի���

class CDebug : public CDialogEx
{
	DECLARE_DYNAMIC(CDebug)

public:
	CDebug(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDebug();

// �Ի�������
	enum { IDD = IDD_DEBUG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabDebug;
	afx_msg void OnTcnSelchangeTabDebug(NMHDR *pNMHDR, LRESULT *pResult);
};

extern CDebug *g_dlgDebug;