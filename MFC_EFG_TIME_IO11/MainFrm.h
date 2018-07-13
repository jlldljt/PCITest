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
#include "MultiCardCtrl.h"

//#define COUNTER_NUM 180
//struct tagCounter {
//  bool start;
//  bool flag;
//  int index;
//  double counter[4][COUNTER_NUM];
//};

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

  CMFC_EFG_TIME_IOView *m_defaultView;
  CSplitFrameWnd *m_splitFrame;
  CBoardView *m_viewBoard;

  //计数器计数一帧数据相关函数
  //struct tagCounter m_counter;
  //void StartCounter(double delay1/*out3*/, double delay2/*out6*/);
  //void DIIntCB(void);
  CDiIntCounterSnap m_diIntCounterSnap;
  //用来加载所有配置并运行，子窗口不显示
  CMultiCardCtrl m_multiCardCtrl;
  //保存当前所选设备号，与combo sel 不是一个概念
  int m_deviceNumber;

  //POINT m_laserSpikes[10];
// 操作
public:
  enum VIEW_ID {
    SPLIT_FRAME = 0,
    VIEW_BOARD,
  };
  void Switch(VIEW_ID id);
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

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
};


CMainFrame* GetMainFrame();
void DIIntCB(void);