#pragma once


// CDlgParam 对话框
#include "EfgIO.h"

class CDlgParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParam)

public:
	CDlgParam(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgParam();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


private:
  CBrush m_brush;
  EFG_ConfigParam *m_param;


public:
  virtual BOOL OnInitDialog();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnEnChangeEdtFactorA();
  afx_msg void OnEnChangeEdtFactorL();
  afx_msg void OnEnChangeEdtLaserOffset();
  afx_msg void OnBnClickedBtnCalcDeg0();
  afx_msg void OnBnClickedBtnCalcDeg1();
  afx_msg void OnEnChangeEdtD1();
  afx_msg void OnEnChangeEdtThetaOffset();
  afx_msg void OnEnChangeEdtPhiOffset();
  afx_msg void OnEnChangeEdtCnt();
  afx_msg void OnBnClickedBtnCalcEqu();
};
