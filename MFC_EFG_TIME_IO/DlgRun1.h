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
    ENUM_STATE mainrun;//mainrun�̵߳�״̬ 1
    ENUM_STATE x;
    ENUM_STATE y;
    ENUM_STATE turntable;//ת��״̬
    ENUM_STATE staticmeasure;//��̬����
  }state;
  int measure; // 0�޲���
}Run_Param;

// CDlgRun1 ������ͼ

class CDlgRun1 : public CFormView
{
	DECLARE_DYNCREATE(CDlgRun1)

protected:
	CDlgRun1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

  Run_Param m_run;//���в���
  EFG_ConfigParam *m_param;//���ò���
  CEfgIO *m_io;//io����
  EFG_ResultParam *m_result;//���н��
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


