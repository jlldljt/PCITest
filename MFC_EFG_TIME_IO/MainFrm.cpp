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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgDI.h"
#include "DlgDO.h"
#include "DlgT0.h"
#include "DlgT1.h"
#include "MainFrm.h"

//#define ENABLE_CNT_NUM 4
CMainFrame* GetMainFrame() {
  static CFrameWndEx *pMain;
  if(!pMain)
  pMain = (CFrameWndEx *)AfxGetMainWnd();
  return ((CMainFrame*)pMain);
};
//struct tagCounter {
//  bool start;
//  bool flag;
//  int index;
//  double counter[4][312];
//};
//
//tagCounter g_counter;
//
//void DIIntCB(void)
//{
//  if (g_counter.start) {
//    memset(&g_counter, 0, sizeof(g_counter));
//
//    for (int i = 0; i < 4; i++) {
//      GetMainFrame()->m_timeIOCtrl.StopT0(i);
//      GetMainFrame()->m_timeIOCtrl.StartT0(i, ((CDlgT0*)(GetMainFrame()->m_splitFrame->m_splitWndEx.GetPane(2, i)))->m_device, 0, 0);//out6
//    }
//    g_counter.flag = 1;
//  }
//
//  if (!g_counter.flag)
//    return;
//
//  double fparam;
//  for (int i = 0; i < 4; i++) {
//    GetMainFrame()->m_timeIOCtrl.ReadT0(i, g_counter.counter[i][g_counter.index], fparam);
//  }
//
//  if (g_counter.index++ >= 311)
//  {
//    g_counter.flag = 0;
//  }
//}
//
//
//void StartCounter(double delay1/*out3*/, double delay2/*out6*/)
//{
//    GetMainFrame()->m_timeIOCtrl.StopT0(4);//out6
//    GetMainFrame()->m_timeIOCtrl.StopT0(5);//out3
//
//    GetMainFrame()->m_timeIOCtrl.StartT0(4, ((CDlgT0*)(GetMainFrame()->m_splitFrame->m_splitWndEx.GetPane(2, 4)))->m_device, delay2, 0);//out6
//    GetMainFrame()->m_timeIOCtrl.StartT0(5, ((CDlgT0*)(GetMainFrame()->m_splitFrame->m_splitWndEx.GetPane(2, 5)))->m_device, delay1, 0);//out3
//    g_counter.start = 1;
//}



//void CMainFrame::DIIntCB(void)
//{
//  if (m_counter.start) {
//    memset(&m_counter, 0, sizeof(m_counter));
//
//    for (int i = 0; i < ENABLE_CNT_NUM; i++) {
//      m_timeIOCtrl.StopT0(i);
//      m_timeIOCtrl.StartT0(i, m_deviceNumber, 0, 0);//out6
//    }
//    m_counter.flag = 1;
//  }
//
//  if (!m_counter.flag)
//    return;
//
//  double fparam;
//  for (int i = 0; i < ENABLE_CNT_NUM; i++) {
//    m_timeIOCtrl.ReadT0(i, m_counter.counter[i][m_counter.index], fparam);
//  }
//
//  if (m_counter.index++ >= COUNTER_NUM-1)
//  {
//    m_counter.flag = 0;
//
//	for (int i = COUNTER_NUM- 1; i > 0; i--) {
//		for (int j = 0; j < ENABLE_CNT_NUM; j++) {
//	  m_counter.counter[j][i]-=m_counter.counter[j][i-1];
//		}
//	}
//
//    m_viewBoard->Invalidate();
//  }
//}
//
//
//void CMainFrame::StartCounter(double delay1/*out3*/, double delay2/*out6*/)
//{
//  m_timeIOCtrl.StopT0(4);//out6
//  m_timeIOCtrl.StopT0(5);//out3
//
//  m_timeIOCtrl.StartT0(4, m_deviceNumber, delay2, 0);//out6
//  m_timeIOCtrl.StartT0(5, m_deviceNumber, delay1, 0);//out3
//  m_counter.start = 1;
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
  ON_COMMAND(ID_COMBO_TIMEIO, &CMainFrame::OnComboTimeio)
  ON_COMMAND(ID_BUTTON_LASER_SIN, &CMainFrame::OnButtonLaserSin)
  ON_UPDATE_COMMAND_UI(ID_EDIT_OUT3, &CMainFrame::OnUpdateEditOut3)
  ON_UPDATE_COMMAND_UI(ID_EDIT_OUT6, &CMainFrame::OnUpdateEditOut6)
  ON_WM_DESTROY()
  ON_WM_CLOSE()
  ON_COMMAND(ID_BUTTON_SNAP, &CMainFrame::OnButtonSnap)
  ON_COMMAND(ID_BUTTON_PARAM_LOAD, &CMainFrame::OnButtonParamLoad)
  ON_COMMAND(ID_BUTTON_PARAM_SAVE, &CMainFrame::OnButtonParamSave)
  ON_COMMAND(ID_BUTTON_PARAM_RUN, &CMainFrame::OnButtonParamRun)
  ON_COMMAND(ID_BUTTON_PARAM_STOP, &CMainFrame::OnButtonParamStop)
  ON_COMMAND(ID_BUTTON_XRAY_ONESHOT, &CMainFrame::OnButtonXrayOneshot)
  ON_COMMAND(ID_BUTTON_MEASURE, &CMainFrame::OnButtonMeasure)
  ON_COMMAND(ID_BUTTON_HOME, &CMainFrame::OnButtonHome)
  ON_COMMAND(ID_BUTTON_LASER_FIT, &CMainFrame::OnButtonLaserFit)
  ON_COMMAND(ID_BUTTON_XRUN, &CMainFrame::OnButtonXrun)
  ON_COMMAND(ID_BUTTON_XRAY_FIT, &CMainFrame::OnButtonXrayFit)
  ON_COMMAND(ID_BUTTON_URUN, &CMainFrame::OnButtonUrun)
  ON_COMMAND(ID_EDIT_USTEP, &CMainFrame::OnEditUstep)
  ON_UPDATE_COMMAND_UI(ID_EDIT_USTEP, &CMainFrame::OnUpdateEditUstep)
  ON_UPDATE_COMMAND_UI(ID_EDIT_XSTEP, &CMainFrame::OnUpdateEditXstep)
//  ON_WM_HSCROLL()
//  ON_WM_MOUSEWHEEL()
//  ON_WM_VSCROLL()
ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
  m_splitFrame = NULL;
  m_viewBoard = NULL;
  m_defaultView = NULL;

  CPCICtrl::AllDriverInit();

  m_timeIOCtrl = NULL;// CPCICtrl::Create(PCI1780U); //new CTimeIOCtrl;
}

CMainFrame::~CMainFrame()
{
  

  //CPCICtrl::Delete(m_timeIOCtrl);
  
  CPCICtrl::AllDriverClose();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
  //设置标题栏的图标  
  HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_EFG);
  SetIcon(m_hIcon, TRUE);
  SetIcon(m_hIcon, FALSE);

  int count = CPCICtrl::m_num;// m_timeIOCtrl->getDevices();
  DevInf dev;
  //combo init // combo必须禁止自动排序，或者 会对应不起来
  CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_TIMEIO));
  for (int i = 0; i < count; i++) {
    int ret = CPCICtrl::getDevice(i, dev);
    int nIndex = pComboBox->AddItem(dev.description);
    //pComboBox->SetEditText(m_capControl.m_capName);
    if (!m_multiCardCtrl.m_card[i]) {  //CPCICtrl::Delete(m_timeIOCtrl);//出错操作
      m_multiCardCtrl.m_card[i] = CPCICtrl::Create(dev.type);
    }
  }

  //SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX));
 
	return 0;
}

//0 m_splitFrame
//1 m_viewBoard
void CMainFrame::Switch(VIEW_ID id)
{
  switch (id)
  {
  case SPLIT_FRAME:
    if (AFX_IDW_PANE_FIRST == m_splitFrame->GetDlgCtrlID())
      return;
    m_splitFrame->ShowWindow(SW_SHOW);
    m_splitFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
    m_viewBoard->ShowWindow(SW_HIDE);
    m_viewBoard->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 1);
    break;
  case VIEW_BOARD:
    if (AFX_IDW_PANE_FIRST == m_viewBoard->GetDlgCtrlID())
      return;
    m_viewBoard->ShowWindow(SW_SHOW);
    m_viewBoard->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
    m_splitFrame->ShowWindow(SW_HIDE);
    m_splitFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 1);
    break;
  default:
    break;
  }

  RecalcLayout();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
  cs.x = 0;
  cs.y = 0;
  cs.cx = 190*16+24;//高度设为300
  cs.cy = 170*4+67;//宽度设为200
  //cs.style &= ~WS_MAXIMIZEBOX;
  cs.style &= ~WS_MAXIMIZEBOX;
  cs.style &= ~WS_THICKFRAME;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 强制关闭“打印预览”模式
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}


void CMainFrame::OnComboTimeio()
{
  // TODO: 在此添加命令处理程序代码
  //TEST
  //CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_TIMEIO));
  //
  //DevInf dev;
  //int sel = pComboBox->GetCurSel();
  //if(sel < 0)
  //  return;
  //int ret = m_timeIOCtrl.getDevice(sel, dev);
  ///*DeviceCtrl *obj = DeviceCtrl::Create(dev.deviceNumber, dev.description.GetBuffer(), ModeWriteWithReset);
  //dev.description.ReleaseBuffer();
  //int scenarios = DeviceCtrl_getSupportedScenarios(obj);*/
  //for (int i = 0; i < 8; i++) {
  //  ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->Stop();
  //  ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->Stop();
  //  ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->Stop();
  //  
  //  ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->m_device = dev.deviceNumber;
  //  ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->m_device = dev.deviceNumber;
  //  ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->m_device = dev.deviceNumber;
  //  ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->m_device = dev.deviceNumber;

  //  ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->m_devName = dev.description;
  //  ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->m_devName = dev.description;
  //  ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->m_devName = dev.description;
  //  ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->m_devName = dev.description;

  //  ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->LoadParam();
  //  ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->LoadParam();
  //  ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->LoadParam();
  //}
  //m_deviceNumber = dev.deviceNumber;

  
  CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_TIMEIO));
  
  DevInf dev;
  int sel = pComboBox->GetCurSel();
  if(sel < 0 || sel >= MAX_CARD_NUM)
    return;
  int ret = CPCICtrl::getDevice(sel, dev);

  if (!m_multiCardCtrl.m_card[sel]) {  //CPCICtrl::Delete(m_timeIOCtrl);//出错操作
    m_multiCardCtrl.m_card[sel] = CPCICtrl::Create(dev.type);
  }
  for (int i = 0; i < 8; i++) {
	((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->Stop();
	((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->Stop();
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->Stop();
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->Stop();
  }
  m_timeIOCtrl = m_multiCardCtrl.m_card[sel];

  for (int i = 0; i < 8; i++) {
    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->m_device = dev.deviceNumber;
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->m_device = dev.deviceNumber;
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->m_device = dev.deviceNumber;
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->m_device = dev.deviceNumber;

    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->m_devName = dev.description;
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->m_devName = dev.description;
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->m_devName = dev.description;
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->m_devName = dev.description;

  }
  m_deviceNumber = dev.deviceNumber;
  Switch(SPLIT_FRAME);
  return;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
  // TODO: 在此添加专用代码和/或调用基类
  CDocument *doc = pContext->m_pCurrentDoc;
  CRect rect;
  GetClientRect(&rect);
 // rect = CRect(-300, -300, 190*16, 190 * 4);
  int nwidth(rect.right);
  int nheight(rect.bottom);   //获取客户区窗口大小  

  m_splitFrame = new CSplitFrameWnd;

  m_splitFrame->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW &~WS_BORDER, rect/*CFrameWndEx::rectDefault*/, this, NULL, 0, pContext);
 
  m_splitFrame->ShowWindow(SW_SHOW);
  
  m_splitFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST);//单文档设置当前活动view的默认id给前台view

  pContext->m_pNewViewClass = (CRuntimeClass*)m_splitFrame;   //设置默认视图类

  m_viewBoard = new CBoardView;
  m_viewBoard->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW &~WS_BORDER, rect/*CFrameWndEx::rectDefault*/, this, NULL/*如果是CFrameView需要对应的dialog IDD*/, pContext);
  m_viewBoard->ShowWindow(SW_HIDE);

//////////////////

  //m_defaultView = new CMFC_EFG_TIME_IOView();
  //m_defaultView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW &~WS_BORDER, rect, this, NULL, pContext);
  //m_defaultView->ShowWindow(SW_HIDE);

  return true;
  //return CFrameWndEx::OnCreateClient(lpcs, pContext);
}


void CMainFrame::OnButtonLaserSin()
{
  // TODO: 在此添加命令处理程序代码
  PCIType type = m_timeIOCtrl->m_type;

  if (PCI1780U != type) {
    AfxMessageBox(L"选中卡不是1780");
    return;
  }

  //if(AFX_IDW_PANE_FIRST != m_splitFrame->GetDlgCtrlID())
  //  Switch(SPLIT_FRAME);

  CMFCRibbonEdit *p_edit_out3 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_OUT3));
  CMFCRibbonEdit *p_edit_out6 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_OUT6));
  CString val_out3 = p_edit_out3->GetEditText();
  CString val_out6 = p_edit_out6->GetEditText();

  // 开启一次捕捉
  m_diIntCounterSnap.BindCard(m_deviceNumber, m_timeIOCtrl, m_viewBoard);
  m_diIntCounterSnap.StartDiIntLaserSin(0);
#ifdef __DEBUG__
  m_diIntCounterSnap.StartCaptureSin(0, _wtof(val_out3), 0, _wtof(val_out6));// TODO：应该时5和4
#else
  m_diIntCounterSnap.StartCaptureSin(OUT3_COUNTER, _wtof(val_out3), OUT6_COUNTER, _wtof(val_out6));// TODO：应该时5和4
#endif
  Switch(VIEW_BOARD);
}


void CMainFrame::OnUpdateEditOut3(CCmdUI *pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  pCmdUI->Enable(true);
}


void CMainFrame::OnUpdateEditOut6(CCmdUI *pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  pCmdUI->Enable(true);
}


void CMainFrame::OnDestroy()
{
  CFrameWndEx::OnDestroy();

  // TODO: 在此处添加消息处理程序代码

}


BOOL CMainFrame::DestroyWindow()
{
  // TODO: 在此添加专用代码和/或调用基类

  return CFrameWndEx::DestroyWindow();
}


void CMainFrame::OnClose()
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  CDocument* doc = GetActiveDocument();
  SetActiveView(m_viewBoard);
  delete m_splitFrame;
  m_splitFrame = NULL;
  
  CFrameWndEx::OnClose();
}


void CMainFrame::OnButtonSnap()
{
  // TODO: 在此添加命令处理程序代码

  // 开启一次捕捉
  m_diIntCounterSnap.BindCard(m_deviceNumber, m_timeIOCtrl, m_viewBoard);
  m_diIntCounterSnap.StartDiIntLaserCircle(0);
  m_diIntCounterSnap.StartCaptureCircle(1);// TODO：

  Switch(VIEW_BOARD);
}


void CMainFrame::OnButtonParamLoad()
{
  // TODO: 在此添加命令处理程序代码
  CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_TIMEIO));

  DevInf dev;
  int sel = pComboBox->GetCurSel();
  if (sel < 0)
    return;
  int ret = CPCICtrl::getDevice(sel, dev);

  for (int i = 0; i < 8; i++) {
    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->Stop();
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->Stop();
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->Stop();
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->Stop();

    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->m_device = dev.deviceNumber;
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->m_device = dev.deviceNumber;
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->m_device = dev.deviceNumber;
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->m_device = dev.deviceNumber;

    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->m_devName = dev.description;
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->m_devName = dev.description;
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->m_devName = dev.description;
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->m_devName = dev.description;

    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->LoadParam();
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->LoadParam();
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->LoadParam();
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->LoadParam();
  }
  m_deviceNumber = dev.deviceNumber;
 // Switch(SPLIT_FRAME);
  return;
}


void CMainFrame::OnButtonParamSave()
{
  // TODO: 在此添加命令处理程序代码

  for (int i = 0; i < 8; i++) {
    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->SaveParam();
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->SaveParam();
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->SaveParam();
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->SaveParam();
  }

  return;
}


void CMainFrame::OnButtonParamRun()
{
  // TODO: 在此添加命令处理程序代码
  CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_TIMEIO));

  int sel = pComboBox->GetCount();
  if (sel <= 0)
    return;

  for (int i = 0; i < sel; i++) {
    m_multiCardCtrl.Load(i);
  }

  ///////////////////out3 out6///////////////////////
  DevInf dev;
  //combo init // combo必须禁止自动排序，或者 会对应不起来
  int count = CPCICtrl::m_num;
  if (!count)
    return;

  int index = -1;
  int deviceNumber = -1;
  for (int i = 0; i < count; i++) {
    int ret = CPCICtrl::getDevice(i, dev);
    if (TMC12A == dev.type) {
      index = i;
      deviceNumber = dev.deviceNumber;
      break;
    }
  }

  if (-1 == index) {
    AfxMessageBox(L"没有tmc12a");
    return;
  }

  double param0, param1;
  m_multiCardCtrl.LoadParam(L"T0", OUT3_COUNTER, index, param0, param1);
  CString val_out3;
  val_out3.Format(L"%lf", param1);
  m_multiCardCtrl.LoadParam(L"T0", OUT6_COUNTER, index, param0, param1);
  CString val_out6;
  val_out6.Format(L"%lf", param1);
  CMFCRibbonEdit *p_edit_out3 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_OUT3));
  CMFCRibbonEdit *p_edit_out6 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_OUT6));
  
  p_edit_out3->SetEditText(val_out3);
  p_edit_out6->SetEditText(val_out6);

}


void CMainFrame::OnButtonParamStop()
{
  // TODO: 在此添加命令处理程序代码
  CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_TIMEIO));

  int sel = pComboBox->GetCount();
  if (sel <= 0)
    return;

  for (int i = 0; i < sel; i++) {
    m_multiCardCtrl.Stop(i);
  }
}


void CMainFrame::OnButtonXrayOneshot()
{
  // TODO: 在此添加命令处理程序代码
  PCIType type = m_timeIOCtrl->m_type;

  if (PCI1780U != type) {
    AfxMessageBox(L"选中卡不是1780");
    return;
  }
  // 开启一次捕捉
  m_diIntCounterSnap.BindCard(m_deviceNumber, m_timeIOCtrl, m_viewBoard);
  m_diIntCounterSnap.StartDiIntXRayOneShot(0);
  m_diIntCounterSnap.StartCaptureXRayOneShot();

  Switch(VIEW_BOARD);
}


void CMainFrame::OnButtonMeasure()
{
  // TODO: 在此添加命令处理程序代码
	if(m_diIntCounterSnap.CheckStart() > 0) {
		AfxMessageBox(L"正在采集");
		return;
	}

#ifndef __DEBUG__
  DevInf dev;
  //combo init // combo必须禁止自动排序，或者 会对应不起来
  int count = CPCICtrl::m_num; 
  if (!count)
    return;

  int index = -1;
  int deviceNumber = -1;
  for (int i = 0; i < count; i++) {
    int ret = CPCICtrl::getDevice(i, dev);
    if (TMC12A == dev.type) {
      index = i;
      deviceNumber = dev.deviceNumber;
	  break;
    }
  }

  if (-1 == index) {
    AfxMessageBox(L"没有tmc12a");
    return;
  }

  m_diIntCounterSnap.BindCard(deviceNumber, m_multiCardCtrl.m_card[index], m_viewBoard);
  //m_diIntCounterSnap.StartDiIntLaserSin(0);
  //m_diIntCounterSnap.StartDiIntXRayOneShot(0);
  m_diIntCounterSnap.StartXRayAndLaser(0);

  //TODO:打开X光

  CMFCRibbonEdit *p_edit_out3 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_OUT3));
  CMFCRibbonEdit *p_edit_out6 = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_OUT6));
  CString val_out3 = p_edit_out3->GetEditText();
  CString val_out6 = p_edit_out6->GetEditText();
  m_diIntCounterSnap.StartCaptureSin(OUT3_COUNTER, _wtof(val_out3), OUT6_COUNTER, _wtof(val_out6));// TODO：应该时5和4
  m_diIntCounterSnap.StartCaptureXRayOneShot();

  // TODO
  // 这里要加入打开 转盘零位gate输出
  // 加入等待零位gate out 低
  // 加入等待零位gate out 高
  // 加入处理代码
  m_diIntCounterSnap.StartMeasure();
#endif
}


void CMainFrame::OnButtonHome()
{
  // TODO: 在此添加命令处理程序代码
  Switch(SPLIT_FRAME);
}


void CMainFrame::OnButtonLaserFit()
{
  // TODO: 在此添加命令处理程序代码
  if (0 != m_diIntCounterSnap.LaserFit())
    AfxMessageBox(L"error");
  else
  Switch(VIEW_BOARD);
}


void CMainFrame::OnButtonXrayFit()
{
  // TODO: 在此添加命令处理程序代码
  if (0 != m_diIntCounterSnap.XrayFit())
    AfxMessageBox(L"error");
  else
  Switch(VIEW_BOARD);
}


void CMainFrame::OnButtonUrun()
{
	// TODO: 在此添加命令处理程序代码
	//if(m_diIntCounterSnap.CheckStart() > 0) {
	//	AfxMessageBox(L"正在采集");
	//	return;
	//}

#ifndef __DEBUG__
  DevInf dev;
  //combo init // combo必须禁止自动排序，或者 会对应不起来
  int count = CPCICtrl::m_num; 
  if (!count)
    return;

  int index = -1;
  int deviceNumber = -1;
  for (int i = 0; i < count; i++) {
    int ret = CPCICtrl::getDevice(i, dev);
    if (TMC12A == dev.type) {
      index = i;
      deviceNumber = dev.deviceNumber;
	  break;
    }
  }

  if (-1 == index) {
    AfxMessageBox(L"没有tmc12a");
    return;
  }

  m_diIntCounterSnap.BindCard(deviceNumber, m_multiCardCtrl.m_card[index], m_viewBoard);

  //TODO:电机

  CMFCRibbonEdit *p_edit_ustep = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_USTEP));
  CString val_ustep = p_edit_ustep->GetEditText();


  m_diIntCounterSnap.StartURunTrd(_wtof(val_ustep), 1, 2);
#endif
}


void CMainFrame::OnEditUstep()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnUpdateEditUstep(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	 pCmdUI->Enable(true);
}


void CMainFrame::OnUpdateEditXstep(CCmdUI *pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  pCmdUI->Enable(true);
}


void CMainFrame::OnButtonXrun()
{
  // TODO: 在此添加命令处理程序代码
  m_diIntCounterSnap.BindCard(0, NULL, m_viewBoard);
  Switch(VIEW_BOARD);
  m_diIntCounterSnap. TestS();
 
}

//   最大最小尺寸的象素点（具体根据实际需要设置） 
#define   MINX   200  
#define   MINY   300  
#define   MAXX   190*16  +24
#define   MAXY   170*4 +67 
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  CRect   rectWindow;
  GetWindowRect(&rectWindow);

  CRect   rectClient;
  GetClientRect(&rectClient);

  //   get   offset   of   toolbars,   scrollbars,   etc.  
  int   nWidthOffset = rectWindow.Width() - rectClient.Width();
  int   nHeightOffset = rectWindow.Height() - rectClient.Height();
  lpMMI->ptMinTrackSize.x = MINX + nWidthOffset;
  lpMMI->ptMinTrackSize.y = MINY + nHeightOffset;
  lpMMI->ptMaxTrackSize.x = MAXX + nWidthOffset;
  lpMMI->ptMaxTrackSize.y = MAXY + nHeightOffset;
  //CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}
