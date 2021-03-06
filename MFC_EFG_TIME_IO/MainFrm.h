// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.h : CMainFrame 类的接口
//

#pragma once



#include "MFC_EFG_TIME_IOView.h"
#include "SplitFrameWnd.h"
#include "BoardView.h"
#include "DiIntCounterSnap.h"
//#include "MultiCardCtrl.h"
#include "CUserSplitterWnd.h"
#include "EfgIO.h"

extern CString g_ini_path;

//#define COUNTER_NUM 180
//struct tagCounter {
//  bool start;
//  bool flag;
//  int index;
//  double counter[4][COUNTER_NUM];
//};
  enum VIEW_ID {
    SPLIT_FRAME = 0,
    VIEW_BOARD,
    USER_FRAME,
  };


class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
  CPCIBase *m_timeIOCtrl;//分割窗口调试用
  //CSplitterWnd m_splitwnd;
  CParamConfig m_config;
  CString m_exe_path;//是目录路径，如果要使用需加上形如 + _T("\\xx\\xx.ini");的路径

  CMFC_EFG_TIME_IOView *m_defaultView;
  CSplitFrameWnd *m_splitFrame;
  CBoardView *m_viewBoard;
  CUserSplitterWnd *m_userFrame;

  //计数器计数一帧数据相关函数
  //struct tagCounter m_counter;
  //void StartCounter(double delay1/*out3*/, double delay2/*out6*/);
  //void DIIntCB(void);
  CDiIntCounterSnap m_diIntCounterSnap;
  //用来加载所有配置并运行，子窗口不显示
  CMultiCardCtrl m_multiCardCtrl;
  CEfgIO m_efgio;
  //保存当前所选设备号，与combo sel 不是一个概念
  int m_deviceNumber;
  //POINT m_laserSpikes[10];
// 操作
public:

  void Switch(VIEW_ID id);
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


public:
  // 产生滚动条的相关函数和变量
  int  m_nHorzInc, m_nVertInc,
    m_nVscrollMax, m_nHscrollMax,//最大滚动位置
    m_nVscrollPos, m_nHscrollPos;//当前滚动位置

  CRect m_ClientRect;
  CRect m_TabRect;
  CScrollBar m_Scroll;

  void ChangeSize(CRect rect);

  void ScrollInit(void);
  void SetupScrollbars();
  void ResetScrollbars();

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnComboTimeio();
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  afx_msg void OnButtonLaserSin();
  afx_msg void OnUpdateEditOut3(CCmdUI *pCmdUI);
  afx_msg void OnUpdateEditOut6(CCmdUI *pCmdUI);
  afx_msg void OnDestroy();
  virtual BOOL DestroyWindow();
  afx_msg void OnClose();
  afx_msg void OnButtonSnap();
  afx_msg void OnButtonParamLoad();
  afx_msg void OnButtonParamSave();
  afx_msg void OnButtonParamRun();
  afx_msg void OnButtonParamStop();
  afx_msg void OnButtonXrayOneshot();
  afx_msg void OnButtonMeasure();
  afx_msg void OnButtonHome();
  afx_msg void OnButtonLaserFit();
  afx_msg void OnButtonXrayFit();
  afx_msg void OnButtonUrun();
  afx_msg void OnEditUstep();
  afx_msg void OnUpdateEditUstep(CCmdUI *pCmdUI);
  afx_msg void OnUpdateEditXstep(CCmdUI *pCmdUI);
  afx_msg void OnButtonXrun();
//  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
  afx_msg void OnButtonRunpage();
  afx_msg void OnButtonDebugpage();
  afx_msg void OnButtonConfigchannel();
  afx_msg void OnFileSave();
  void EfgParamLoad();
  void EfgParamSave();
  BOOL StartMeasure(int out3, int out6);
  BOOL CheckMeasure();
  afx_msg void OnChkAutoRun();
  afx_msg void OnUpdateChkAutoRun(CCmdUI *pCmdUI);
  afx_msg void OnButtonHalfpage();
  afx_msg void OnBtnTestCalcDegree1();
//  virtual BOOL PreTranslateMessage(MSG* pMsg);
};


CMainFrame* GetMainFrame();
void DIIntCB(void);