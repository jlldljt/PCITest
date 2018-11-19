#pragma once


#include "DlgDebugDevice.h"
#include "DlgLaser.h"
#include "DlgXray.h"
#include "DlgParam.h"

// CDlgDebug1 窗体视图

class CDlgDebug1 : public CFormView
{
	DECLARE_DYNCREATE(CDlgDebug1)

protected:
	CDlgDebug1();           // 动态创建所使用的受保护的构造函数
	virtual ~CDlgDebug1();

public:
	enum { IDD = IDD_DLG_DEBUG1 };
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
public:
  CDlgDebugDevice * m_dlg_debug_device;
  CDlgLaser * m_dlg_laser;
  CDlgXray *m_dlg_xray;
  CDlgParam *m_dlg_param;
  CTabCtrl m_tab_debug;
  afx_msg void OnTcnSelchangeTabDebug(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  virtual void OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
  virtual void OnInitialUpdate();
  virtual void PreSubclassWindow();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


