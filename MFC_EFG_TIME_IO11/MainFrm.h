// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.h : CMainFrame ��Ľӿ�
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
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
  CPCIBase *m_timeIOCtrl;//�ָ�ڵ�����
  //CSplitterWnd m_splitwnd;
  CParamConfig m_config;

  CMFC_EFG_TIME_IOView *m_defaultView;
  CSplitFrameWnd *m_splitFrame;
  CBoardView *m_viewBoard;

  //����������һ֡������غ���
  //struct tagCounter m_counter;
  //void StartCounter(double delay1/*out3*/, double delay2/*out6*/);
  //void DIIntCB(void);
  CDiIntCounterSnap m_diIntCounterSnap;
  //���������������ò����У��Ӵ��ڲ���ʾ
  CMultiCardCtrl m_multiCardCtrl;
  //���浱ǰ��ѡ�豸�ţ���combo sel ����һ������
  int m_deviceNumber;

  //POINT m_laserSpikes[10];
// ����
public:
  enum VIEW_ID {
    SPLIT_FRAME = 0,
    VIEW_BOARD,
  };
  void Switch(VIEW_ID id);
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
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