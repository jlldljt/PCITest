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
#include "DlgTestCalcDegree1.h"

#define CHANNEL_MAX 16

//#define ENABLE_CNT_NUM 4
CMainFrame* GetMainFrame() {
  static CFrameWndEx *pMain;
  if (!pMain)
    pMain = (CFrameWndEx *)AfxGetMainWnd();
  return ((CMainFrame*)pMain);
};


CString g_ini_path;
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
  ON_COMMAND(ID_BUTTON_RUNPAGE, &CMainFrame::OnButtonRunpage)
  ON_COMMAND(ID_BUTTON_DEBUGPAGE, &CMainFrame::OnButtonDebugpage)
  ON_COMMAND(ID_BUTTON_CONFIGCHANNEL, &CMainFrame::OnButtonConfigchannel)
  ON_COMMAND(ID_FILE_SAVE, &CMainFrame::OnFileSave)
  ON_COMMAND(ID_CHK_AUTO_RUN, &CMainFrame::OnChkAutoRun)
  ON_UPDATE_COMMAND_UI(ID_CHK_AUTO_RUN, &CMainFrame::OnUpdateChkAutoRun)
  ON_COMMAND(ID_BUTTON_HALFPAGE, &CMainFrame::OnButtonHalfpage)
  ON_COMMAND(ID_BTN_CALCDEGREE1, &CMainFrame::OnBtnTestCalcDegree1)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
  // TODO: 在此添加成员初始化代码
  theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
  m_splitFrame = NULL;
  m_viewBoard = NULL;
  m_userFrame = NULL;
  m_defaultView = NULL;

  //CPCICtrl::AllDriverInit();

  m_timeIOCtrl = NULL;// CPCICtrl::Create(PCI1780U); //new CTimeIOCtrl;
  //路径
  CString path;
  GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
  path.ReleaseBuffer();
  int pos = path.ReverseFind('\\');
  m_exe_path = path.Left(pos);

  //ini文件初始化或读取
  //目录初始化
  CString ini_path = m_exe_path + _T("\\INI\\EFG.ini");
  ///////////////
  CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
  if (!findini.FindFile(m_exe_path + _T("\\INI\\*.*")))
  {
    if (!CreateDirectory(m_exe_path + _T("\\INI\\"), NULL))
    {
      AfxMessageBox(_T("INI目录创建失败！"));
      exit(0);
    }
  }
  BOOL ifFind = findini.FindFile(ini_path);
  if (!ifFind)  //ini创建，仅此一次
  {
    AfxMessageBox(_T("第一次运行，请进入配置界面配置并保存"));
    ////motor
    //WritePrivateProfileString(_T("电机X"), _T("最大频率"), _T("500"), ini_path);
    //WritePrivateProfileString(_T("电机X"), _T("最小频率"), _T("152"), ini_path);
    //WritePrivateProfileString(_T("电机X"), _T("S弯曲度"), _T("8"), ini_path);

    //WritePrivateProfileString(_T("电机Y"), _T("最大频率"), _T("500"), ini_path);
    //WritePrivateProfileString(_T("电机Y"), _T("最小频率"), _T("152"), ini_path);
    //WritePrivateProfileString(_T("电机Y"), _T("S弯曲度"), _T("8"), ini_path);

    //WritePrivateProfileString(_T("电机U"), _T("最大频率"), _T("500"), ini_path);
    //WritePrivateProfileString(_T("电机U"), _T("最小频率"), _T("152"), ini_path);
    //WritePrivateProfileString(_T("电机U"), _T("S弯曲度"), _T("8"), ini_path);

    //WritePrivateProfileString(_T("激光"), _T("OUT3"), _T("80"), ini_path);
    //WritePrivateProfileString(_T("激光"), _T("OUT6"), _T("120"), ini_path);
  }
  /*wchar_t ret[100] = { 0 };
  GetPrivateProfileString(_T("电机X"), _T("最大频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.x.max_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机X"), _T("最小频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.x.min_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机X"), _T("S弯曲度"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.x.flexible = _wtof(ret);
  GetPrivateProfileString(_T("电机Y"), _T("最大频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.y.max_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机Y"), _T("最小频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.y.min_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机Y"), _T("S弯曲度"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.y.flexible = _wtof(ret);
  GetPrivateProfileString(_T("电机U"), _T("最大频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.u.max_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机U"), _T("最小频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.u.min_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机U"), _T("S弯曲度"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_efg_param.motor.u.flexible = _wtof(ret);

  m_efgio.m_efg_param.laser.out3 = GetPrivateProfileInt(_T("激光"), _T("OUT3"), 0, ini_path);
  m_efgio.m_efg_param.laser.out6 = GetPrivateProfileInt(_T("激光"), _T("OUT6"), 0, ini_path);
*/
  g_ini_path = ini_path;


  EfgParamLoad();
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

  //
  m_wndRibbonBar.ShowContextCategories(ID_CONTEXT_DEBUG, TRUE);
#if (_MFC_VER >= 0x0A00)
    m_wndRibbonBar.ShowCategory(1, TRUE);
    m_wndRibbonBar.ShowCategory(2, TRUE);
    m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(2), FALSE);

#else
  m_wndRibbonBar.ShowCategory(0, FALSE);
  m_wndRibbonBar.ShowCategory(1, TRUE);
  m_wndRibbonBar.SetActiveCategory(m_wndRibbonBar.GetCategory(1), FALSE);

#endif

  //m_wndRibbonBar.ShowCategory(4, TRUE);
  //m_wndRibbonBar.ShowCategory(5, TRUE);


  m_wndRibbonBar.ToggleMimimizeState();


  CList<UINT, UINT> lstQATCmds;

  lstQATCmds.AddTail(ID_BUTTON_RUNPAGE);
  lstQATCmds.AddTail(ID_BUTTON_HALFPAGE);
  lstQATCmds.AddTail(ID_BUTTON_DEBUGPAGE);
  lstQATCmds.AddTail(ID_FILE_SAVE);

  // CMFCRibbonBar m_wndRibbonBar
  m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

  m_wndRibbonBar.ForceRecalcLayout();


  //

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

  //int count = CPCICtrl::m_num;// m_timeIOCtrl->getDevices();
  //DevInf dev;
  ////combo init // combo必须禁止自动排序，或者 会对应不起来
  //CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_TIMEIO));
  //for (int i = 0; i < count; i++) {
  //  int ret = CPCICtrl::getDevice(i, dev);
  //  int nIndex = pComboBox->AddItem(dev.description);
  //  ////pComboBox->SetEditText(m_capControl.m_capName);
  //  if (!m_multiCardCtrl.m_card[i]) {  //CPCICtrl::Delete(m_timeIOCtrl);//出错操作
  //    m_multiCardCtrl.m_card[i] = CPCICtrl::Create(dev.type);
  //  }
  //}
  m_efgio.InitPCI();
  int count = m_efgio.GetPCINum();
  CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_TIMEIO));

  for (int i = 0; i < count; i++) {
    int nIndex = pComboBox->AddItem(m_efgio.GetPCIDesc(i));
  }

  //CRect rect;
  //GetClientRect(&rect);
  ////SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~(WS_SIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX));
  //SetWindowPos(NULL, rect.left - 190, rect.top - 190, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
  if (m_efgio.m_configParam.auto_run)
  {
    OnButtonParamRun();
  }



  return 0;
}

//0 m_splitFrame
//1 m_viewBoard
void CMainFrame::Switch(VIEW_ID id)
{
  CRect rect;	// 存储当前窗口
  int cx = GetSystemMetrics(SM_CXSCREEN);
  int cy = GetSystemMetrics(SM_CYSCREEN);

  switch (id)
  {
  case SPLIT_FRAME:
    if (AFX_IDW_PANE_FIRST == m_splitFrame->GetDlgCtrlID())
      return;
    m_splitFrame->ShowWindow(SW_SHOW);
    m_splitFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
    m_viewBoard->ShowWindow(SW_HIDE);
    m_viewBoard->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 1);
    m_userFrame->ShowWindow(SW_HIDE);
    m_userFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 2);
    MoveWindow(0, 0, 190 * 16 + 24, cy/* 170 * 4 + 67*/, TRUE);  	// 改变窗

    break;
  case VIEW_BOARD:
    if (AFX_IDW_PANE_FIRST == m_viewBoard->GetDlgCtrlID())
      return;
    m_viewBoard->ShowWindow(SW_SHOW);
    m_viewBoard->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
    m_splitFrame->ShowWindow(SW_HIDE);
    m_splitFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 1);
    m_userFrame->ShowWindow(SW_HIDE);
    m_userFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 2);

    //GetWindowRect(rect);	// 得到当前窗体的位置及大小

    MoveWindow(0, 0, cx, cy, TRUE);  	// 改变窗


    break;
  case USER_FRAME:
    if (AFX_IDW_PANE_FIRST == m_userFrame->GetDlgCtrlID())
      return;
    m_userFrame->ShowWindow(SW_SHOW);
    m_userFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
    m_splitFrame->ShowWindow(SW_HIDE);
    m_splitFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 1);
    m_viewBoard->ShowWindow(SW_HIDE);
    m_viewBoard->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 2);
    MoveWindow(0, 0, cx, cy, TRUE);  	// 改变窗
    break;
  default:
    break;
  }

  RecalcLayout();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!CFrameWndEx::PreCreateWindow(cs))
    return FALSE;
  // TODO: 在此处通过修改
  //  CREATESTRUCT cs 来修改窗口类或样式
  int cx = GetSystemMetrics(SM_CXSCREEN);
  int cy = GetSystemMetrics(SM_CYSCREEN);
  cs.x = 0;
  cs.y = 0;
  cs.cx = cx;//高度设为300
  cs.cy = cy;//宽度设为200
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

  //DevInf dev;
  int sel = pComboBox->GetCurSel();
  if (sel < 0 || sel >= MAX_CARD_NUM)
    return;
  //int ret = CPCICtrl::getDevice(sel, dev);
  int deviceNumber = m_efgio.GetPCIDeviceNumber(sel);
  CString description = m_efgio.GetPCIDesc(sel);

  //if (!m_multiCardCtrl.m_card[sel]) {  //CPCICtrl::Delete(m_timeIOCtrl);//出错操作
  //  m_multiCardCtrl.m_card[sel] = CPCICtrl::Create(dev.type);
  //}


  for (int i = 0; i < CHANNEL_MAX; i++) {
    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->Stop();
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->Stop();
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->Stop();
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->Stop();
  }
  //m_timeIOCtrl = m_multiCardCtrl.m_card[sel];
  m_timeIOCtrl = m_efgio.GetPCI(sel);

  for (int i = 0; i < CHANNEL_MAX; i++) {
    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->m_device = deviceNumber;
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->m_device = deviceNumber;
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->m_device = deviceNumber;
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->m_device = deviceNumber;

    ((CDlgDI*)m_splitFrame->m_splitWndEx.GetPane(0, i))->m_devName = description;
    ((CDlgDO*)m_splitFrame->m_splitWndEx.GetPane(1, i))->m_devName = description;
    ((CDlgT0*)m_splitFrame->m_splitWndEx.GetPane(2, i))->m_devName = description;
    ((CDlgT1*)m_splitFrame->m_splitWndEx.GetPane(3, i))->m_devName = description;

  }
  m_deviceNumber = deviceNumber;
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

  m_splitFrame->ShowWindow(SW_HIDE);
  //
  //m_splitFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST);//单文档设置当前活动view的默认id给前台view

  //pContext->m_pNewViewClass = (CRuntimeClass*)m_splitFrame;   //设置默认视图类

  m_viewBoard = new CBoardView;
  m_viewBoard->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW &~WS_BORDER, rect/*CFrameWndEx::rectDefault*/, this, NULL/*如果是CFrameView需要对应的dialog IDD*/, pContext);
  m_viewBoard->ShowWindow(SW_HIDE);
  //add by mmy 201809
 // m_dlgDebug1 = new CDlgDebug1;
 // m_dlgDebug1->CreateEx(0, NULL, NULL, AFX_WS_DEFAULT_VIEW &~WS_BORDER, rect, this,0);

  m_userFrame = new CUserSplitterWnd;
  m_userFrame->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW &~WS_BORDER, rect, this, NULL, 0, pContext);
  m_userFrame->ShowWindow(SW_SHOW);


  m_userFrame->SetDlgCtrlID(AFX_IDW_PANE_FIRST);//单文档设置当前活动view的默认id给前台view

  pContext->m_pNewViewClass = (CRuntimeClass*)m_userFrame;   //设置默认视图类

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
	if (m_diIntCounterSnap.CheckStart() > 0) {
    //AfxMessageBox(L"正在采集");
    return;
  }

  int index = m_efgio.GetCardIndex();

  if (index < 0)
    return;


  m_diIntCounterSnap.BindCard(m_efgio.GetPCIDeviceNumber(index), m_efgio.GetPCI(index), m_viewBoard, &m_efgio);
  m_diIntCounterSnap.StartDiIntLaserCircle(0);

  m_diIntCounterSnap.StartMeasure();

  return;
  //m_diIntCounterSnap.BindCard(m_deviceNumber, m_timeIOCtrl, m_viewBoard);
  //m_diIntCounterSnap.StartDiIntLaserCircle(0);
  //m_diIntCounterSnap.StartCaptureCircle(1);// TODO：

  //Switch(VIEW_BOARD);
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

  for (int i = 0; i < CHANNEL_MAX; i++) {
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

  for (int i = 0; i < CHANNEL_MAX; i++) {
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

  //int sel = pComboBox->GetCount();
  //if (sel <= 0)
  //  return;

  //for (int i = 0; i < sel; i++) {
  //  //m_multiCardCtrl.Load(i);
  //  m_efgio.RunPCI(i);
  //}

  /////////////////////out3 out6///////////////////////
  //DevInf dev;
  ////combo init // combo必须禁止自动排序，或者 会对应不起来
  //int count = CPCICtrl::m_num;
  //if (!count)
  //  return;

  //int index = -1;
  //int deviceNumber = -1;
  //for (int i = 0; i < count; i++) {
  //  int ret = CPCICtrl::getDevice(i, dev);
  //  if (TMC12A == dev.type) {
  //    index = i;
  //    deviceNumber = dev.deviceNumber;
  //    break;
  //  }
  //}

  //if (-1 == index) {
  //  AfxMessageBox(L"没有tmc12a");
  //  return;
  //}

  if (FALSE == m_efgio.CardOn()) {
    AfxMessageBox(L"板卡启动失败");
    return;
  }

  CString val_out3;
  val_out3.Format(L"%d", m_efgio.GetOut3());
  CString val_out6;
  val_out6.Format(L"%d", m_efgio.GetOut6());
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

  //for (int i = 0; i < sel; i++) {
  //  m_multiCardCtrl.Stop(i);
  //}
  if (FALSE == m_efgio.CardOff()) {
    AfxMessageBox(L"tmc12a关闭失败");
    return;
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
  if (m_diIntCounterSnap.CheckStart() > 0) {
    AfxMessageBox(L"正在采集");
    return;
  }

#ifndef __DEBUG__
  //DevInf dev;
  ////combo init // combo必须禁止自动排序，或者 会对应不起来
  //int count = CPCICtrl::m_num; 
  //if (!count)
  //  return;

  //int index = -1;
  //int deviceNumber = -1;
  //for (int i = 0; i < count; i++) {
  //  int ret = CPCICtrl::getDevice(i, dev);
  //  if (TMC12A == dev.type) {
  //    index = i;
  //    deviceNumber = dev.deviceNumber;
   // break;
  //  }
  //}

  //if (-1 == index) {
  //  AfxMessageBox(L"没有tmc12a");
  //  return;
  //}

  int index = m_efgio.GetCardIndex();

  if (index < 0)
    return;

  m_diIntCounterSnap.BindCard(m_efgio.GetPCIDeviceNumber(index), m_efgio.GetPCI(index), m_viewBoard, &m_efgio);
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
  while (!GetMainFrame()->CheckMeasure());
  AfxMessageBox(L"over");
#endif
}


void CMainFrame::OnButtonHome()
{
  // TODO: 在此添加命令处理程序代码
  Switch(USER_FRAME);
}


void CMainFrame::OnButtonLaserFit()
{
  // TODO: 在此添加命令处理程序代码
  if (0 != m_diIntCounterSnap.LaserFit(1))
    AfxMessageBox(L"error");
  else
    Switch(VIEW_BOARD);
}


void CMainFrame::OnButtonXrayFit()
{
  // TODO: 在此添加命令处理程序代码
  if (0 != m_diIntCounterSnap.XrayFit(1))
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
	#ifndef USE_EFGV1
#ifndef __DEBUG__
  //DevInf dev;
  ////combo init // combo必须禁止自动排序，或者 会对应不起来
  //int count = CPCICtrl::m_num; 
  //if (!count)
  //  return;

  //int index = -1;
  //int deviceNumber = -1;
  //for (int i = 0; i < count; i++) {
  //  int ret = CPCICtrl::getDevice(i, dev);
  //  if (TMC12A == dev.type) {
  //    index = i;
  //    deviceNumber = dev.deviceNumber;
   // break;
  //  }
  //}

  //if (-1 == index) {
  //  AfxMessageBox(L"没有tmc12a");
  //  return;
  //}
  int index = m_efgio.GetCardIndex();

  if (index < 0)
    return;

  m_diIntCounterSnap.BindCard(m_efgio.GetPCIDeviceNumber(index), m_efgio.GetPCI(index), m_viewBoard, &m_efgio);

  //TODO:电机

  CMFCRibbonEdit *p_edit_ustep = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_EDIT_USTEP));
  CString val_ustep = p_edit_ustep->GetEditText();


  m_diIntCounterSnap.StartURunTrd(_wtof(val_ustep), 1, 2);
#endif
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
  m_diIntCounterSnap.TestS();

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


void CMainFrame::OnButtonRunpage()
{
  // TODO: 在此添加命令处理程序代码

  Switch(USER_FRAME);

  CRect   rect;
  GetClientRect(&rect);

  int cx = GetSystemMetrics(SM_CXSCREEN);
  int cy = GetSystemMetrics(SM_CYSCREEN);
  m_userFrame->SetSize(0, 0, rect.Width() - 1, rect.Height());

  m_userFrame->SetSize(0, 1, 1, rect.Height());
  m_wndRibbonBar.ShowContextCategories(ID_CONTEXT_DEBUG, FALSE);
  m_wndRibbonBar.ForceRecalcLayout();
}


void CMainFrame::OnButtonDebugpage()
{
  // TODO: 在此添加命令处理程序代码
  Switch(USER_FRAME);

  CRect   rect;
  GetClientRect(&rect);

  int cx = GetSystemMetrics(SM_CXSCREEN);
  int cy = GetSystemMetrics(SM_CYSCREEN);
  m_userFrame->SetSize(0, 0, 1, rect.Height());
  m_userFrame->SetSize(0, 1, rect.Width() - 1, rect.Height());
  m_wndRibbonBar.ShowContextCategories(ID_CONTEXT_DEBUG, TRUE);
  m_wndRibbonBar.ForceRecalcLayout();
}


void CMainFrame::OnButtonConfigchannel()
{
  // TODO: 在此添加命令处理程序代码
  Switch(SPLIT_FRAME);
}


void CMainFrame::OnFileSave()
{
  // TODO: 在此添加命令处理程序代码
  EfgParamSave();


  AfxMessageBox(_T("保存成功！"));
  ////link CMainFrame()
  //CString ini_path = g_ini_path;
  /////////////////
  //CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
  //if (!findini.FindFile(m_exe_path + _T("\\INI\\*.*")))
  //{
  //  if (!CreateDirectory(m_exe_path + _T("\\INI\\"), NULL))
  //  {
  //    AfxMessageBox(_T("INI目录创建失败！"));
  //    return;
  //  }
  //}

  //CString str;
  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.x.max_freq);
  //WritePrivateProfileString(_T("电机X"), _T("最大频率"), str, ini_path);
  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.x.min_freq);
  //WritePrivateProfileString(_T("电机X"), _T("最小频率"), str, ini_path);
  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.x.flexible);
  //WritePrivateProfileString(_T("电机X"), _T("S弯曲度"), str, ini_path);

  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.y.max_freq);
  //WritePrivateProfileString(_T("电机Y"), _T("最大频率"), str, ini_path);
  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.y.min_freq);
  //WritePrivateProfileString(_T("电机Y"), _T("最小频率"), str, ini_path);
  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.y.flexible);
  //WritePrivateProfileString(_T("电机Y"), _T("S弯曲度"), str, ini_path);

  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.u.max_freq);
  //WritePrivateProfileString(_T("电机U"), _T("最大频率"), str, ini_path);
  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.u.min_freq);
  //WritePrivateProfileString(_T("电机U"), _T("最小频率"), str, ini_path);
  //str.Format(_T("%.2f"), m_efgio.m_efg_param.motor.u.flexible);
  //WritePrivateProfileString(_T("电机U"), _T("S弯曲度"), str, ini_path);

  //str.Format(_T("%.2f"), m_efgio.m_efg_param.laser.out3);
  //WritePrivateProfileString(_T("激光"), _T("OUT3"), str, ini_path);
  //str.Format(_T("%.2f"), m_efgio.m_efg_param.laser.out6);
  //WritePrivateProfileString(_T("激光"), _T("OUT6"), str, ini_path);

  //AfxMessageBox(_T("保存成功！"));
}

void CMainFrame::EfgParamLoad()
{
  //目录初始化
  CString ini_path = g_ini_path;
  wchar_t ret[100] = { 0 };
  //laser
  m_efgio.m_configParam.laser.out3 = GetPrivateProfileInt(_T("激光"), _T("OUT3"), 0, ini_path);
  m_efgio.m_configParam.laser.out6 = GetPrivateProfileInt(_T("激光"), _T("OUT6"), 0, ini_path);

  GetPrivateProfileString(_T("激光"), _T("幅度因子"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.laser.factor_a = _wtof(ret);
  GetPrivateProfileString(_T("激光"), _T("距离因子"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.laser.factor_l = _wtof(ret);
  GetPrivateProfileString(_T("激光"), _T("角度偏移"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.laser.offset = _wtof(ret);
  GetPrivateProfileString(_T("激光"), _T("光轴偏移"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.laser.theta_offset = _wtof(ret);
  GetPrivateProfileString(_T("激光"), _T("电轴偏移"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.laser.phi_offset = _wtof(ret);

  
  m_efgio.m_configParam.user_config.measure.cnt = GetPrivateProfileInt(_T("测量配置"), _T("测量次数"), 0, ini_path);
  //xray

  m_efgio.m_configParam.xray.threshold = GetPrivateProfileInt(_T("X光"), _T("阈值"), 0, ini_path);
  m_efgio.m_configParam.xray.confirmNum = GetPrivateProfileInt(_T("X光"), _T("最小宽度"), 0, ini_path);
  m_efgio.m_configParam.xray.ignore = GetPrivateProfileInt(_T("X光"), _T("忽略值"), 0, ini_path);
  GetPrivateProfileString(_T("X光"), _T("纵向因子"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.xray.factor_h = _wtof(ret);
  GetPrivateProfileString(_T("X光"), _T("横向因子"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.xray.factor_w = _wtof(ret);
  //u_auto
  m_efgio.m_configParam.u_auto.degree[0] = GetPrivateProfileInt(_T("U自动参数"), _T("角度1"), 0, ini_path);
  m_efgio.m_configParam.u_auto.step[0] = GetPrivateProfileInt(_T("U自动参数"), _T("步数1"), 0, ini_path);
  
  m_efgio.m_configParam.u_auto.degree[1] = GetPrivateProfileInt(_T("U自动参数"), _T("角度2"), 0, ini_path);
  m_efgio.m_configParam.u_auto.step[1] = GetPrivateProfileInt(_T("U自动参数"), _T("步数2"), 0, ini_path);
  

  //motor
  GetPrivateProfileString(_T("电机X"), _T("最大频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.x.max_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机X"), _T("最小频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.x.min_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机X"), _T("S弯曲度"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.x.flexible = _wtof(ret);
  GetPrivateProfileString(_T("电机Y"), _T("最大频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.y.max_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机Y"), _T("最小频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.y.min_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机Y"), _T("S弯曲度"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.y.flexible = _wtof(ret);
  GetPrivateProfileString(_T("电机U"), _T("最大频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.u.max_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机U"), _T("最小频率"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.u.min_freq = _wtof(ret);
  GetPrivateProfileString(_T("电机U"), _T("S弯曲度"), _T(""), ret, sizeof(ret), ini_path);
  m_efgio.m_configParam.motor.u.flexible = _wtof(ret);
  //user_config
  m_efgio.m_configParam.user_config.type = GetPrivateProfileInt(_T("基本配置"), _T("晶片类型"), 0, ini_path);
  for (int i = 0; i < MAX_TYPE_NUM; i++)
  {
    CString tmp;
    tmp.Format(_T("%d类档数"), i);
    m_efgio.m_configParam.user_config.type_sort_num[i] = GetPrivateProfileInt(_T("基本配置"), tmp, 0, ini_path);
  }
  m_efgio.m_configParam.user_config.pos.x_off = GetPrivateProfileInt(_T("基本配置"), _T("x放料位"), 0, ini_path);
  m_efgio.m_configParam.user_config.pos.x_wait = GetPrivateProfileInt(_T("基本配置"), _T("x等待位"), 0, ini_path);
  m_efgio.m_configParam.user_config.pos.y_wait = GetPrivateProfileInt(_T("基本配置"), _T("y等待位"), 0, ini_path);
  for (int i = 0; i < MAX_TYPE_NUM; i++)
  {
    CString tmp_type;
    tmp_type.Format(_T("%d类档位"), i);
    ASSERT(m_efgio.m_configParam.user_config.type_sort_num[i] < MAX_SORT_NUM);
    for (int j = 0; j < m_efgio.m_configParam.user_config.type_sort_num[i]; j++)
    {
      CString tmp_step;
      tmp_step.Format(_T("第%d档位"), j);
      m_efgio.m_configParam.user_config.pos.y_off[i][j] = GetPrivateProfileInt(tmp_type, tmp_step, 0, ini_path);
    }
  }
  m_efgio.m_configParam.user_config.pos.primary_n = GetPrivateProfileInt(_T("基本配置"), _T("主测角低"), 0, ini_path);
  m_efgio.m_configParam.user_config.pos.primary_p = GetPrivateProfileInt(_T("基本配置"), _T("主测角高"), 0, ini_path);
  m_efgio.m_configParam.user_config.pos.secondary_n = GetPrivateProfileInt(_T("基本配置"), _T("次测角低"), 0, ini_path);
  m_efgio.m_configParam.user_config.pos.secondary_p = GetPrivateProfileInt(_T("基本配置"), _T("次测角高"), 0, ini_path);
  m_efgio.m_configParam.user_config.pos.not_detected = GetPrivateProfileInt(_T("基本配置"), _T("未检出"), 0, ini_path);
  //时间
  m_efgio.m_configParam.user_config.time.x_on = GetPrivateProfileInt(_T("基本配置"), _T("x吸料时间"), 0, ini_path);
  m_efgio.m_configParam.user_config.time.x_off = GetPrivateProfileInt(_T("基本配置"), _T("x放料时间"), 0, ini_path);
  m_efgio.m_configParam.user_config.time.y_on = GetPrivateProfileInt(_T("基本配置"), _T("y吸料时间"), 0, ini_path);
  m_efgio.m_configParam.user_config.time.y_off = GetPrivateProfileInt(_T("基本配置"), _T("y放料时间"), 0, ini_path);
  m_efgio.m_configParam.user_config.time.blow = GetPrivateProfileInt(_T("基本配置"), _T("吹气时间"), 0, ini_path);
  m_efgio.m_configParam.user_config.time.clean = GetPrivateProfileInt(_T("基本配置"), _T("清理三孔时间"), 0, ini_path);

  GetPrivateProfileString(_T("测量配置"), _T("流程卡"), _T(""), m_efgio.m_configParam.user_config.measure.card, MAX_CARD_LEN, ini_path);

  //m_efgio.m_configParam.user_config.measure.card= GetPrivateProfileInt(_T("测量配置"), _T("流程卡"), 0, ini_path);
  m_efgio.m_configParam.user_config.measure.primary.type = GetPrivateProfileInt(_T("测量配置"), _T("主测量类型"), 0, ini_path);
  m_efgio.m_configParam.user_config.measure.primary.center_degree = GetPrivateProfileInt(_T("测量配置"), _T("中心角度"), 0, ini_path);
  m_efgio.m_configParam.user_config.measure.primary.step_degree = GetPrivateProfileInt(_T("测量配置"), _T("步进角度"), 0, ini_path);
  m_efgio.m_configParam.user_config.measure.secondary.type = GetPrivateProfileInt(_T("测量配置"), _T("次测量类型"), 0, ini_path);
  m_efgio.m_configParam.user_config.measure.secondary.min_degree = GetPrivateProfileInt(_T("测量配置"), _T("最小角度"), 0, ini_path);
  m_efgio.m_configParam.user_config.measure.secondary.max_degree = GetPrivateProfileInt(_T("测量配置"), _T("最大角度"), 0, ini_path);
  m_efgio.m_configParam.user_config.measure.equivalent_angle.phi = GetPrivateProfileInt(_T("测量配置"), _T("等效角电轴"), 0, ini_path);
  m_efgio.m_configParam.user_config.measure.equivalent_angle.factor = GetPrivateProfileInt(_T("测量配置"), _T("等效角因子"), 0, ini_path);

  //其他配置
  m_efgio.m_configParam.auto_run = GetPrivateProfileInt(_T("其他配置"), _T("自动运行"), 0, ini_path);
}

void CMainFrame::EfgParamSave()
{
  // TODO: 在此添加命令处理程序代码
  //link CMainFrame()
  CString ini_path = g_ini_path;
  ///////////////
  //CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
  //if (!findini.FindFile(m_exe_path + _T("\\INI\\*.*")))
  //{
  //  if (!CreateDirectory(m_exe_path + _T("\\INI\\"), NULL))
  //  {
  //    AfxMessageBox(_T("INI目录创建失败！"));
  //    return;
  //  }
  //}

  CString str;
  //laser
  str.Format(_T("%d"), m_efgio.m_configParam.laser.out3);
  WritePrivateProfileString(_T("激光"), _T("OUT3"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.laser.out6);
  WritePrivateProfileString(_T("激光"), _T("OUT6"), str, ini_path);

  str.Format(_T("%.3f"), m_efgio.m_configParam.laser.factor_a);
  WritePrivateProfileString(_T("激光"), _T("幅度因子"), str, ini_path);
  str.Format(_T("%.3f"), m_efgio.m_configParam.laser.factor_l);
  WritePrivateProfileString(_T("激光"), _T("距离因子"), str, ini_path);
  str.Format(_T("%.3f"), m_efgio.m_configParam.laser.offset);
  WritePrivateProfileString(_T("激光"), _T("角度偏移"), str, ini_path);
  str.Format(_T("%.3f"), m_efgio.m_configParam.laser.theta_offset);
  WritePrivateProfileString(_T("激光"), _T("光轴偏移"), str, ini_path);
  str.Format(_T("%.3f"), m_efgio.m_configParam.laser.phi_offset);
  WritePrivateProfileString(_T("激光"), _T("电轴偏移"), str, ini_path);


  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.cnt);
  WritePrivateProfileString(_T("测量配置"), _T("测量次数"), str, ini_path);
  //xray
  str.Format(_T("%d"), m_efgio.m_configParam.xray.threshold);
  WritePrivateProfileString(_T("X光"), _T("阈值"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.xray.confirmNum);
  WritePrivateProfileString(_T("X光"), _T("最小宽度"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.xray.ignore);
  WritePrivateProfileString(_T("X光"), _T("忽略值"), str, ini_path);
  str.Format(_T("%.3f"), m_efgio.m_configParam.xray.factor_h);
  WritePrivateProfileString(_T("X光"), _T("纵向因子"), str, ini_path);
  str.Format(_T("%.3f"), m_efgio.m_configParam.xray.factor_w);
  WritePrivateProfileString(_T("X光"), _T("横向因子"), str, ini_path);
  //u_auto
  str.Format(_T("%d"), m_efgio.m_configParam.u_auto.degree[0]);
  WritePrivateProfileString(_T("U自动参数"), _T("角度1"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.u_auto.step[0]);
  WritePrivateProfileString(_T("U自动参数"), _T("步数1"), str, ini_path);

  str.Format(_T("%d"), m_efgio.m_configParam.u_auto.degree[1]);
  WritePrivateProfileString(_T("U自动参数"), _T("角度2"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.u_auto.step[1]);
  WritePrivateProfileString(_T("U自动参数"), _T("步数2"), str, ini_path);
  //motor
  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.x.max_freq);
  WritePrivateProfileString(_T("电机X"), _T("最大频率"), str, ini_path);
  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.x.min_freq);
  WritePrivateProfileString(_T("电机X"), _T("最小频率"), str, ini_path);
  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.x.flexible);
  WritePrivateProfileString(_T("电机X"), _T("S弯曲度"), str, ini_path);

  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.y.max_freq);
  WritePrivateProfileString(_T("电机Y"), _T("最大频率"), str, ini_path);
  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.y.min_freq);
  WritePrivateProfileString(_T("电机Y"), _T("最小频率"), str, ini_path);
  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.y.flexible);
  WritePrivateProfileString(_T("电机Y"), _T("S弯曲度"), str, ini_path);

  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.u.max_freq);
  WritePrivateProfileString(_T("电机U"), _T("最大频率"), str, ini_path);
  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.u.min_freq);
  WritePrivateProfileString(_T("电机U"), _T("最小频率"), str, ini_path);
  str.Format(_T("%.2f"), m_efgio.m_configParam.motor.u.flexible);
  WritePrivateProfileString(_T("电机U"), _T("S弯曲度"), str, ini_path);

  //user_config
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.type);
  WritePrivateProfileString(_T("基本配置"), _T("晶片类型"), str, ini_path);
  for (int i = 0; i < MAX_TYPE_NUM; i++)
  {
    CString tmp;
    tmp.Format(_T("%d类档数"), i);
    str.Format(_T("%d"), m_efgio.m_configParam.user_config.type_sort_num[i]);
    WritePrivateProfileString(_T("基本配置"), tmp, str, ini_path);
  }
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.x_off);
  WritePrivateProfileString(_T("基本配置"), _T("x放料位"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.x_wait);
  WritePrivateProfileString(_T("基本配置"), _T("x等待位"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.y_wait);
  WritePrivateProfileString(_T("基本配置"), _T("y等待位"), str, ini_path);
  for (int i = 0; i < MAX_TYPE_NUM; i++)
  {
    CString tmp_type;
    tmp_type.Format(_T("%d类档位"), i);
    ASSERT(m_efgio.m_configParam.user_config.type_sort_num[i] < MAX_SORT_NUM);
    for (int j = 0; j < m_efgio.m_configParam.user_config.type_sort_num[i]; j++)
    {
      CString tmp_step;
      tmp_step.Format(_T("第%d档位"), j);
      str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.y_off[i][j]);
      WritePrivateProfileString(tmp_type, tmp_step, str, ini_path);
    }
  }
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.primary_n);
  WritePrivateProfileString(_T("基本配置"), _T("主测角低"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.primary_p);
  WritePrivateProfileString(_T("基本配置"), _T("主测角高"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.secondary_n);
  WritePrivateProfileString(_T("基本配置"), _T("次测角低"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.secondary_p);
  WritePrivateProfileString(_T("基本配置"), _T("次测角高"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.pos.not_detected);
  WritePrivateProfileString(_T("基本配置"), _T("未检出"), str, ini_path);
  //时间
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.time.x_on);
  WritePrivateProfileString(_T("基本配置"), _T("x吸料时间"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.time.x_off);
  WritePrivateProfileString(_T("基本配置"), _T("x放料时间"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.time.y_on);
  WritePrivateProfileString(_T("基本配置"), _T("y吸料时间"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.time.y_off);
  WritePrivateProfileString(_T("基本配置"), _T("y放料时间"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.time.blow);
  WritePrivateProfileString(_T("基本配置"), _T("吹气时间"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.time.clean);
  WritePrivateProfileString(_T("基本配置"), _T("清理三孔时间"), str, ini_path);

  //str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.card);
  WritePrivateProfileString(_T("测量配置"), _T("流程卡"), m_efgio.m_configParam.user_config.measure.card, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.primary.type);
  WritePrivateProfileString(_T("测量配置"), _T("主测量类型"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.primary.center_degree);
  WritePrivateProfileString(_T("测量配置"), _T("中心角度"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.primary.step_degree);
  WritePrivateProfileString(_T("测量配置"), _T("步进角度"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.secondary.type);
  WritePrivateProfileString(_T("测量配置"), _T("次测量类型"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.secondary.min_degree);
  WritePrivateProfileString(_T("测量配置"), _T("最小角度"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.secondary.max_degree);
  WritePrivateProfileString(_T("测量配置"), _T("最大角度"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.equivalent_angle.phi);
  WritePrivateProfileString(_T("测量配置"), _T("等效角电轴"), str, ini_path);
  str.Format(_T("%d"), m_efgio.m_configParam.user_config.measure.equivalent_angle.factor);
  WritePrivateProfileString(_T("测量配置"), _T("等效角因子"), str, ini_path);

  //其他配置
  str.Format(_T("%d"), m_efgio.m_configParam.auto_run);
  WritePrivateProfileString(_T("其他配置"), _T("自动运行"), str, ini_path);
}
// 修改，更换之后需要修改测量方式
BOOL CMainFrame::StartMeasure(int out3, int out6)
{
  if (m_diIntCounterSnap.CheckStart() > 0) {
    //AfxMessageBox(L"正在采集");
    return FALSE;
  }

  int index = m_efgio.GetCardIndex();

  if (index < 0)
    return FALSE;


  m_diIntCounterSnap.BindCard(m_efgio.GetPCIDeviceNumber(index), m_efgio.GetPCI(index), m_viewBoard, &m_efgio);
  m_diIntCounterSnap.StartXRayAndLaser(0);

#ifdef USE_EFGV1
m_efgio.SetOut(out3, out6);
Sleep(100);
#elif USE_AC6641
  m_diIntCounterSnap.StartCaptureSin(OUT3_COUNTER, out3, OUT6_COUNTER, out6);// TODO：应该时5和4
#endif
  m_diIntCounterSnap.StartCaptureXRayOneShot();

  m_diIntCounterSnap.StartMeasure();

  return TRUE;
}
// TRUE 在测量中，False 没有测量
// 修改
BOOL CMainFrame::CheckMeasure()
{
  if (m_diIntCounterSnap.CheckStart() > 0) {
    return FALSE;
  }
  return TRUE;
}


void CMainFrame::OnChkAutoRun()
{
  // TODO: 在此添加命令处理程序代码
  m_efgio.m_configParam.auto_run = !m_efgio.m_configParam.auto_run;
}


void CMainFrame::OnUpdateChkAutoRun(CCmdUI *pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码

  pCmdUI->SetCheck(/*p_chk->IsVisible()*/m_efgio.m_configParam.auto_run);
}


void CMainFrame::OnButtonHalfpage()
{
  // TODO: 在此添加命令处理程序代码
  Switch(USER_FRAME);

  CRect   rect;
  GetClientRect(&rect);

  int cx = GetSystemMetrics(SM_CXSCREEN);
  int cy = GetSystemMetrics(SM_CYSCREEN);
  m_userFrame->SetSize(0, 0, rect.Width() / 2, rect.Height());
  m_userFrame->SetSize(0, 1, rect.Width() / 2, rect.Height());
}


void CMainFrame::OnBtnTestCalcDegree1()
{
  // TODO: 在此添加命令处理程序代码
  AfxMessageBox(_T("xxxxxxxx"));
  CDlgTestCalcDegree1 dlg;
  dlg.DoModal();


}


//BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
//		{
//			//判断是否按下键盘Enter键
//			switch (pMsg->wParam)
//			{
//				/*case VK_RETURN:
//				OnBnClickedBtnFit();
//				return TRUE;*/
//			case VK_SPACE:
//				//if (2 == m_tab_debug.GetCurSel())
//				//m_dlg_xray->OnBnClickedBtnXrayArc();
//				break;
//			default:
//				break;
//			}
//		}
//	}
//	return CFrameWndEx::PreTranslateMessage(pMsg);
//}
