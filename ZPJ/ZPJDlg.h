
// ZPJDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
//#include "Run.h"
//#include "Debug.h"

// CBERDlg �Ի���
class CBERDlg : public CDialogEx
{
// ����
public:
	CBERDlg(CWnd* pParent = NULL);	// ��׼���캯��
//=========�Զ���===========================	
	//��dlg�������
	BOOL SYSInit();
	BOOL DBGInit();
	BOOL SYSQuit();
	void SurfaceRefresh();
	void IIICStartConnect();
	void BtnCtl(bool ctl);
//==========================
// �Ի�������
	enum { IDD = IDD_BER_DIALOG };

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
//	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
