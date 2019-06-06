#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "CSVFile.h"

// CRun 对话框

class CRun : public CDialogEx
{
	DECLARE_DYNAMIC(CRun)

public:
	CRun(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRun();
	CDC *mDCMem;
	CBitmap *mBMPMem,*mBMPOld;
	BOOL RunInit();
	BOOL CamView();
// 对话框数据
	enum { IDD = IDD_RUN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_listSort;
	CComboBox m_cmb_sort1;
	CComboBox m_cmb_sort2;
  CCSVFile *m_csvCard;
  CCSVFile *m_csvAllCard;
	afx_msg void OnBnClickedBtnSortset();
	afx_msg void OnCbnSelchangeComboSort1();
	afx_msg void OnCbnSelchangeComboSort2();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRadSqu();
	afx_msg void OnBnClickedRadCir();
	afx_msg void OnBnClickedBtnStandard();
	afx_msg void OnStnClickedPicShow();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedChkPn();
	afx_msg void OnBnClickedBtnStaft();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnTestsplit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnPmove();
	afx_msg void OnBnClickedBtnEndft();
  afx_msg void OnBnClickedChkShake();
  afx_msg void OnBnClickedBtnStdcheck();
  afx_msg void OnBnClickedBtnCpk();
  afx_msg void OnBnClickedBtnPrint();
};


extern CRun *g_dlgRun;