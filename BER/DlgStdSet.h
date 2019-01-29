#pragma once
#include "StandardLib.h"

// CDlgStdSet 对话框

class CDlgStdSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStdSet)

public:
	CDlgStdSet(CStandardLib * pLib = NULL, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStdSet();

// 对话框数据
	enum { IDD = IDD_DLG_STD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
  CString m_prevEditStr;

	CStandardLib * m_pLib;
public:
  CGridCtrl m_gridStdSet;
  void InitGrid_StdSet(void);
  void UpdateGrid_StdSet(void);
  void OnBeginLabelEdit_StdSet(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit_StdSet(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged_StdSet(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll_StdSet(NMHDR * pNMHDR, LRESULT * pResult);

  afx_msg void OnBnClickedBtnSave();
  afx_msg void OnBnClickedBtnReload();
  virtual BOOL OnInitDialog();
};
