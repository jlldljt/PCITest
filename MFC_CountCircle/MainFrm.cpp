// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MFC_CountCircle.h"

#include "MainFrm.h"



#include "MFC_CountCircleView.h"
#include "StreamView.h"
#include "DlgView.h"

#define SHOW_TIMER  1000

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
  ON_COMMAND(ID_BTN_START, &CMainFrame::OnBtnStart)
  ON_COMMAND(ID_BTN_STOP, &CMainFrame::OnBtnStop)
  ON_COMMAND(ID_BTN_SNAP, &CMainFrame::OnBtnSnap)
  ON_COMMAND(ID_COMBO_CAPNAME, &CMainFrame::OnComboCapname)
  ON_WM_TIMER()
END_MESSAGE_MAP()


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_bMeasureApertureThread_State = 0;
	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

  CStreamView* pview = (CStreamView*)m_splitwnd.GetPane(0, 0);
  HWND hwnd = pview->GetSafeHwnd();
  if (m_capControl.init(hwnd))
  {
    CMFCRibbonComboBox *pComboBox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, m_wndRibbonBar.FindByID(ID_COMBO_CAPNAME));
    int nIndex = pComboBox->AddItem(m_capControl.m_capName);
    pComboBox->SetEditText(m_capControl.m_capName);
  }
  m_cam.init(hwnd);


  CDlgView* pdlg = (CDlgView*)m_splitwnd.GetPane(0, 1);
  pdlg->GetDlgItem(IDC_STA_NUMBER_OF_APERTURE)->SetFont(&pdlg->m_fontResult);

  SetTimer(SHOW_TIMER, 33, NULL);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������

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
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // ǿ�ƹرա���ӡԤ����ģʽ
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}


void CMainFrame::OnBtnStart()
{
  // TODO: �ڴ���������������
  CStreamView* pview = (CStreamView*)m_splitwnd.GetPane(0, 0);
  m_capControl.Startstreaming();
  pview->Invalidate();
  m_pview = (CDlgView*)m_splitwnd.GetPane(0, 1);
  MeasureApertureThreadBegin();
}


void CMainFrame::OnBtnStop()
{
  // TODO: �ڴ���������������
  CStreamView* pview = (CStreamView*)m_splitwnd.GetPane(0, 0);
  m_capControl.Stopstreaming();
  pview->Invalidate();
  MeasureApertureThreadSuspend();
}

//void CMainFrame::OnBtnSnap()
//{
//  // TODO: �ڴ���������������
//  m_cam.captureBmp("img.bmp");
//  
//  return;
//
//  CDlgView* pview = (CDlgView*)m_splitwnd.GetPane(0, 1);
//  //m_capControl.CaptureSingle(L"cap.bmp");
//  m_capControl.CaptureSingleBuffer();
//
//  CTime tm; 
//	tm=CTime::GetCurrentTime();
//	CStringA csSavePathName;
//	static int Number = 0;
//	csSavePathName.Format("../TestPicture/%d%02d%02d%02d%02d%02d_%d.jpg",tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond(),Number++);
//  m_capControl.SaveImageData((CString)csSavePathName);
// // ShowLocalPicture(L"cap.bmp", pview->GetDlgItem(IDC_PICTURE_SHOW));  
// // m_capControl.SaveImageData(csSavePathName);
//
//  //m_image.Load("./cap3.jpg");
//  //m_pview = pview;
//  //pview->OnBnClickedUpdataBtn();
// /// MeasureAperture(1);
// 
//}

void CMainFrame::OnBtnSnap()
{
  // TODO: �ڴ���������������
  //m_cam.captureBmp("img.bmp");

  //return;

  CDlgView* pview = (CDlgView*)m_splitwnd.GetPane(0, 1);
  //m_capControl.CaptureSingle(L"cap.bmp");
 // m_capControl.CaptureSingleBuffer();

  CTime tm;
  tm = CTime::GetCurrentTime();
  CStringA csSavePathName;
  static int Number = 0;
  csSavePathName.Format("../TestPicture/%d%02d%02d%02d%02d%02d_%d.jpg", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond(), Number++);
  m_capControl.SaveImageData((CString)csSavePathName);
   ShowLocalPicture(L"cap.bmp", pview->GetDlgItem(IDC_PICTURE_SHOW));  
   m_capControl.SaveImageData(csSavePathName);

   m_image.Load("./cap3.jpg");
   m_pview = pview;
   pview->OnBnClickedUpdataBtn();
   MeasureAperture(1);

}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
  // TODO: �ڴ����ר�ô����/����û���
//  m_splitter.SetColumnInfo(0, width, 0);
 // m_splitter.RecalcLayout();


  CRect rect;
  GetClientRect(&rect);
  int nwidth(rect.right);
  int nheight(rect.bottom);   //��ȡ�ͻ������ڴ�С  

  if (!m_splitwnd.CreateStatic(this, 1, 2))   //���ڷָ�  
  {
    MessageBox(_T("�ָ�ڴ���"), _T("Error"), MB_OK | MB_ICONERROR);
    return false;
  }

  //������ص�View��  
  m_splitwnd.CreateView(0, 0, RUNTIME_CLASS(CStreamView), CSize(nwidth / 16 * 11, nheight), pContext);
  m_splitwnd.CreateView(0, 1, RUNTIME_CLASS(CDlgView), CSize(nwidth / 16 * 5, nheight), pContext);
  

  m_splitwnd.SetActivePane(0, 0);
  return true;
  //return CFrameWnd::OnCreateClient(lpcs, pContext);  
}


void CMainFrame::OnComboCapname()
{
  // TODO: �ڴ���������������
}
UINT CMainFrame:: MeasureApertureThread(LPVOID lpParam)  //������Ȧ�߳�
{
	CMainFrame *Frame = (CMainFrame*)lpParam;

	for (;;)
	{	
		Frame->m_capControl.CaptureSingleBuffer();
		Frame->m_capControl.SaveImageData(_T("cap.jpg"));
		m_image.Load("./cap.jpg");
		Frame->MeasureAperture(0);
	}
}
void CMainFrame::MeasureApertureThreadBegin()  //������Ȧ�߳�����
{
	if (m_bMeasureApertureThread_State==FALSE)
	{
		pMeasureApertureThread = AfxBeginThread(MeasureApertureThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		pMeasureApertureThread->m_bAutoDelete = TRUE;
		pMeasureApertureThread->ResumeThread();
		m_bMeasureApertureThread_State = TRUE;
	}
	
}
void CMainFrame::MeasureApertureThreadSuspend()  //������Ȧ�̹߳���
{
	if (m_bMeasureApertureThread_State==TRUE)
	{
		int dwReturn = pMeasureApertureThread->SuspendThread();
		while (dwReturn < 0)
		{
			dwReturn = pMeasureApertureThread->SuspendThread();
		}
		m_bMeasureApertureThread_State = FALSE;
	}
}
void CMainFrame::MeasureApertureThreadResume()  //������Ȧ�ָ̻߳�
{
	DWORD ret;
	ret = pMeasureApertureThread->ResumeThread();
	while (ret>1)
	{
		ret = pMeasureApertureThread->ResumeThread();
	}
	m_bMeasureApertureThread_State = TRUE;
}
void CMainFrame::MeasureAperture(int mode)
{
	if (mode)
	{
		CTime tm; 
		tm=CTime::GetCurrentTime();
		CStringA csSavePathName;
		static int Number = 0;
		csSavePathName.Format("../TestPicture/%d%02d%02d%02d%02d%02d_%d.jpg",tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond(),Number++);
		cvSaveImage(csSavePathName.GetBuffer(),m_image.GetImage());
		csSavePathName.ReleaseBuffer();
	}
	m_pview->MeasureAperture();
	cvSaveImage("./result.bmp", m_image.GetImage());
	m_pview->m_bmp = _T("./result.bmp");
	ShowLocalPicture(L"result.bmp", m_pview->GetDlgItem(IDC_PICTURE_SHOW));
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
  m_cam.snap();
  CFrameWndEx::OnTimer(nIDEvent);
}
