#pragma once
#include "EfgIO.h"


// CDlgTestCalcDegree1 对话框

class CDlgTestCalcDegree1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTestCalcDegree1)

public:
	CDlgTestCalcDegree1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgTestCalcDegree1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TEST_CALC_DEGREE1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedBtnCalcDegree1();

  EFG_ConfigParam *m_param;//配置参数
};
