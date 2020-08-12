
// BICALDebugDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CBICALDebugDlg �Ի���
class CBICALDebugDlg : public CDialogEx
{
// ����
public:
	CBICALDebugDlg(CWnd* pParent = NULL);	// ��׼���캯��
	//=========�Զ���===========================	
	BOOL SYSInit();
	BOOL SYSQuit();
	void SurfaceRefresh();
// �Ի�������
	enum { IDD = IDD_BICALDEBUG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
