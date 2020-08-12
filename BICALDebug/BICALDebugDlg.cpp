
// BICALDebugDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BICALDebug.h"
#include "BICALDebugDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBICALDebugDlg 对话框




CBICALDebugDlg::CBICALDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBICALDebugDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBICALDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_tabMain);
}

BEGIN_MESSAGE_MAP(CBICALDebugDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CBICALDebugDlg::OnTcnSelchangeMainTab)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBICALDebugDlg 消息处理程序

BOOL CBICALDebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SYSInit();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBICALDebugDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBICALDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBICALDebugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBICALDebugDlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CRect tabRect; 
	m_tabMain.GetClientRect(&tabRect);   
	tabRect.left += 1;   
	tabRect.right -= 1;   
	tabRect.top += 22;   
	tabRect.bottom -= 1;   
	switch (m_tabMain.GetCurSel())   
	{   
	case 0:   
		g_dlgDebug1->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   
		g_dlgDebug2->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
		break;
	case 1:   
		g_dlgDebug1->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
		g_dlgDebug2->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   
		break;   
	default:   
		break;   
	}
	*pResult = 0;
}
BOOL CBICALDebugDlg::SYSInit()
{
	CRect initRect;   // 标签控件客户区的位置和大小  
	CString csInit;
	
	//tab控件初始化
	m_tabMain.InsertItem(0, _T("调试1")); 
	m_tabMain.InsertItem(1, _T("调试2"));  
	g_dlgDebug1->Create(IDD_DIALOG_DEBUG1, &m_tabMain);   
	g_dlgDebug2->Create(IDD_DIALOG_DEBUG2, &m_tabMain);
	m_tabMain.GetClientRect(&initRect);//tab控件大小
	initRect.left += 1;                  
	initRect.right -= 1;   
	initRect.top += 22;   
	initRect.bottom -= 1;   
	g_dlgDebug1->SetWindowPos(NULL, initRect.left, initRect.top, initRect.Width(), initRect.Height(),SWP_SHOWWINDOW);   
	g_dlgDebug2->SetWindowPos(NULL, initRect.left, initRect.top, initRect.Width(), initRect.Height(),SWP_HIDEWINDOW); 
	m_tabMain.SetCurSel(0);
	
	//目录初始化
	CFileFind findini;
	BOOL ifFind = findini.FindFile(g_csPath);  
	if( !ifFind )  //ini创建，仅此一次
	{
		//com口号
		WritePrivateProfileString(_T("COM"),_T("口号"),_T("0"),g_csPath);  
	}
	
	//读取ini文件
	int l_int=GetPrivateProfileInt(_T("COM"),_T("口号"),0,g_csPath);
	gclsCom.OpenCom(l_int);
	if(gclsCom.stuInf.bComOpen)
	{
		g_dlgDebug1->SetDlgItemInt(IDC_EDIT_COM,l_int);
		gTrdCom=AfxBeginThread(ComMsg, NULL , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
		gTrdCom->ResumeThread();
	}
	//
	SetTimer(SHOW_WINDOWS, 100, NULL);
	return 1;
}

BOOL CBICALDebugDlg::SYSQuit()
{

	//定时器
	KillTimer(1);
	gclsCom.CloseCom();

	//全局指针
	g_dlgDebug1 = NULL;
	g_dlgDebug2 = NULL;
	return 1;
}

void CBICALDebugDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case SHOW_WINDOWS:
		{
			SurfaceRefresh();
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CBICALDebugDlg::SurfaceRefresh()
{
	//刷新com接收内容
	if (1==gstuRefresh.bComUpdate)
	{
		gstuRefresh.bComUpdate=0;
		g_dlgDebug1->GetDlgItem(IDC_EDIT_RS232_RECEIVE)->SetWindowText(gclsCom.stuInf.csRcv);
		g_dlgDebug1->m_edtRs232Rcv.LineScroll(g_dlgDebug1->m_edtRs232Rcv.GetLineCount(),0);
	}	
}


BOOL CBICALDebugDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	SYSQuit();
	return CDialogEx::DestroyWindow();
}
