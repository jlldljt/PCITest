#pragma once

#include "CpkLib.h"
// CDlgCpkSet 对话框

class CDlgCpkSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCpkSet)

public:
	CDlgCpkSet(CCpkLib * pLib = NULL, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCpkSet();

// 对话框数据
	enum { IDD = IDD_DLG_CPK_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
  CCpkLib *m_cpk;
  CString m_prevEditStr;

  CGridCtrl m_gridCpkSet;
  void InitGrid_CpkSet(void);
  void UpdateGrid_CpkSet(void);
  void OnBeginLabelEdit_CpkSet(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit_CpkSet(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged_CpkSet(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll_CpkSet(NMHDR * pNMHDR, LRESULT * pResult);

  afx_msg void OnBnClickedBtnSave();
  afx_msg void OnBnClickedBtnReload();
  virtual BOOL OnInitDialog();
};
