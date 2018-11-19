#pragma once
#include "EfgIO.h"
#include "staticex.h"

enum ENUM_STATE
{
  START,
  RUNNING,
  PAUSE,
  STOP,
  STATIC_MEASURE_START,
  STATIC_MEASURE_STOP,
  WAIT,
  END,
};

typedef struct {
  struct {
    ENUM_STATE mainrun;//mainrun线程的状态 1
    ENUM_STATE x;
    ENUM_STATE y;
    ENUM_STATE turntable;//转盘状态
    ENUM_STATE staticmeasure;//静态测量
  }state;
  int measure; // 0无测量
}Run_Param;

// CDlgRun1 窗体视图

class CDlgRun1 : public CFormView
{
	DECLARE_DYNCREATE(CDlgRun1)

protected:
	CDlgRun1();           // 动态创建所使用的受保护的构造函数
	virtual ~CDlgRun1();

public:
	enum { IDD = IDD_DLG_RUN1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
  CBrush m_brush;
  CFont m_font;

public:
  CDC * m_laserdc;
  CBitmap *m_laserbm, *m_laseroldbm;
  CDC *m_xraydc;
  CBitmap *m_xraybm, *m_xrayoldbm;
  CRect m_preview_rect;

  Run_Param m_run;//运行参数
  EFG_ConfigParam *m_param;//配置参数
  CEfgIO *m_io;//io控制
  EFG_ResultParam *m_result;//运行结果
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  virtual void OnInitialUpdate();
//  afx_msg void OnBnClickedSortSet();
  afx_msg void OnBnClickedEdtRunSet();
  afx_msg void OnCbnSelchangeCmbPrimary();
  afx_msg void OnEnChangeEdtCard();
  afx_msg void OnCbnSelchangeCmbSecondary();
  afx_msg void OnEnChangeEdtCenterDegree();
  afx_msg void OnEnChangeEdtStepDegree();
  afx_msg void OnEnChangeEdtMinDegree();
  afx_msg void OnEnChangeEdtMaxDegree();
  afx_msg void OnEnChangeEdtEquPhi();
  afx_msg void OnEnChangeEdtEquFactor();
  CGridCtrl m_gridSort;
  void InitGrid(void);
  void UpdateGrid();
  void UpdateGridWithRecalc();
  afx_msg void OnBnClickedBtnCtrl();
  afx_msg void OnBnClickedChkPause();
  CBitmapButton m_btn_ctrl;
  CTabCtrl m_tab_preview;
  void ShowResult(BOOL sw);
  afx_msg void OnTcnSelchangeTabPreview(NMHDR *pNMHDR, LRESULT *pResult);
  int CalcResult();
  CStaticEx m_static_result;
  afx_msg void OnBnClickedChkStaticmeasure();
  CStaticEx m_static_sort;
  CStaticEx m_static_message;
};


