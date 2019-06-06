#pragma once
#include "StandardLib.h"

// CDlgPassword 对话框

class CDlgPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPassword)

public:
	CDlgPassword(CStandardLib * pLib = NULL, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPassword();

// 对话框数据
	enum { IDD = IDD_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
  CStandardLib * m_pLib;

  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
};
