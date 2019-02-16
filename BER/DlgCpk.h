#pragma once


// CDlgCpk 对话框
#include "CpkLib.h"

class CDlgCpk : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCpk)

public:
	CDlgCpk(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCpk();

// 对话框数据
	enum { IDD = IDD_DLG_CPK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
  CCpkLib * m_pCpk;
  CString m_prevEditStr;
  // 计划号列表
  CGridCtrl m_gridPlannedNo;
  void InitGrid_PlannedNo(void);
  void UpdateGrid_PlannedNo(void);
  void OnBeginLabelEdit_PlannedNo(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit_PlannedNo(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged_PlannedNo(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll_PlannedNo(NMHDR * pNMHDR, LRESULT * pResult);
  // 流程卡列表
  CGridCtrl m_gridProcessCardNo;
  void InitGrid_ProcessCardNo(void);
  void UpdateGrid_ProcessCardNo(void);
  void OnBeginLabelEdit_ProcessCardNo(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit_ProcessCardNo(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged_ProcessCardNo(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll_ProcessCardNo(NMHDR * pNMHDR, LRESULT * pResult);

  afx_msg void OnBnClickedBtnSet();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedBtnTest();
};
