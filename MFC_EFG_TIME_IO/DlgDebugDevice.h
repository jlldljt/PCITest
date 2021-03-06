#pragma once

#include "EfgIO.h"
#include "..\gridctrl_demo\gridctrl_src\gridctrl.h"

// CDlgDebugDevice 对话框

class CDlgDebugDevice : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDebugDevice)

public:
	CDlgDebugDevice(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgDebugDevice();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DEVICE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
private:
  CBrush m_brush;
  EFG_ConfigParam *m_param;
  CEfgIO *m_io;

  //方法
  void UpdateOtherFrameAboutSort(void);//当修改档位相关内容后同步刷新其他窗口
public:
//  afx_msg void OnBnClickedBtnMeasure();
  afx_msg void OnBnClickedXTip();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedXrayGate();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnBnClickedBtnRealtime();
  afx_msg void OnCbnSelchangeMotorSel();
  afx_msg void OnBnClickedMotorEn();
  afx_msg void OnEnChangeMotorMax();
  afx_msg void OnEnChangeMotorMin();
  afx_msg void OnEnChangeMotorFlexible();
//  afx_msg void OnEnChangeEditOut3();
//  afx_msg void OnEnChangeEditOut6();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  void InitGrid(void);
  void UpdateGrid();
  void OnBeginLabelEdit(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll(NMHDR * pNMHDR, LRESULT * pResult);
  CGridCtrl m_gridStepSet;
  CString m_prevEditStr;
  afx_msg void OnCbnSelchangeCmbType();
  afx_msg void OnEnChangeEdtSortNum();
//  afx_msg void OnEnChangeEditTimeBlow();
  afx_msg void OnEnChangeEdtTimeBlow();
  afx_msg void OnEnChangeEdtTimeXon();
  afx_msg void OnEnChangeEdtTimeXoff();
  afx_msg void OnEnChangeEdtTimeYon();
  afx_msg void OnEnChangeEdtTimeYoff();
  afx_msg void OnEnChangeEdtTimeClean();
  afx_msg void OnBnClickedMotorRun();
  afx_msg void OnEnChangeMotorDst();
  afx_msg void OnBnClickedMotorZero();
  afx_msg void OnBnClickedYTip();
  afx_msg void OnBnClickedTipBlow();
  afx_msg void OnBnClickedTipThree();
  afx_msg void OnBnClickedAlert();
//  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnBnClickedStepTest();
  afx_msg void OnBnClickedEasySet();
  CGridCtrl m_gridUSet;
  void OnEndLabelEditUSet(NMHDR *pNMHDR, LRESULT *pResult);
  void InitGridUSet(void);
  afx_msg void OnBnClickedBtnUsetTestMove();
  afx_msg void OnBnClickedAirFlag1();
  afx_msg void OnBnClickedAirFlag2();
  afx_msg void OnBnClickedAbTip();
  afx_msg void OnBnClickedAbCy();
  afx_msg void OnBnClickedMotorShake();
};
