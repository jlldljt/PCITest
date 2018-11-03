
// ATtoSCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Debug.h"
#include "Run.h"

#define WM_DISPLAY WM_USER+1
#define WM_DLGALM WM_USER+2
// CATtoSCDlg �Ի���
class CATtoSCDlg : public CDialogEx
{
// ����
public:
	CATtoSCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ATTOSC_DIALOG };

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
	LRESULT OnDisplay(WPARAM wPrarm,LPARAM lParam);//��Ϣ����
	LRESULT OnCreateDlg(WPARAM wPrarm,LPARAM lParam);//��Ϣ��������
public:
	CTabCtrl m_Tab;
	CDebug m_Debug;
	CRun m_Run;
	afx_msg void OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
