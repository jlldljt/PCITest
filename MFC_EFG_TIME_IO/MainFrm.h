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
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
  CPCIBase *m_timeIOCtrl;//�ָ�ڵ�����
  //CSplitterWnd m_splitwnd;
  CParamConfig m_config;
  CString m_exe_path;//��Ŀ¼·�������Ҫʹ����������� + _T("\\xx\\xx.ini");��·��

  CMFC_EFG_TIME_IOView *m_defaultView;
  CSplitFrameWnd *m_splitFrame;
  CBoardView *m_viewBoard;
  CUserSplitterWnd *m_userFrame;

  //����������һ֡������غ���
  //struct tagCounter m_counter;
  //void StartCounter(double delay1/*out3*/, double delay2/*out6*/);
  //void DIIntCB(void);
  CDiIntCounterSnap m_diIntCounterSnap;
  //���������������ò����У��Ӵ��ڲ���ʾ
  CMultiCardCtrl m_multiCardCtrl;
  CEfgIO m_efgio;
  //���浱ǰ��ѡ�豸�ţ���combo sel ����һ������
  int m_deviceNumber;
  //POINT m_laserSpikes[10];
// ����
public:

  void Switch(VIEW_ID id);
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


public:
  // ��������������غ����ͱ���
  int  m_nHorzInc, m_nVertInc,
    m_nVscrollMax, m_nHscrollMax,//������λ��
    m_nVscrollPos, m_nHscrollPos;//��ǰ����λ��

  CRect m_ClientRect;
  CRect m_TabRect;
  CScrollBar m_Scroll;

  void ChangeSize(CRect rect);

  void ScrollInit(void);
  void SetupScrollbars();
  void ResetScrollbars();

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