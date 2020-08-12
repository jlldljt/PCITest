
// BICALDebugDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BICALDebug.h"
#include "BICALDebugDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CBICALDebugDlg �Ի���




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


// CBICALDebugDlg ��Ϣ�������

BOOL CBICALDebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SYSInit();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBICALDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBICALDebugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBICALDebugDlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	CRect initRect;   // ��ǩ�ؼ��ͻ�����λ�úʹ�С  
	CString csInit;
	
	//tab�ؼ���ʼ��
	m_tabMain.InsertItem(0, _T("����1")); 
	m_tabMain.InsertItem(1, _T("����2"));  
	g_dlgDebug1->Create(IDD_DIALOG_DEBUG1, &m_tabMain);   
	g_dlgDebug2->Create(IDD_DIALOG_DEBUG2, &m_tabMain);
	m_tabMain.GetClientRect(&initRect);//tab�ؼ���С
	initRect.left += 1;                  
	initRect.right -= 1;   
	initRect.top += 22;   
	initRect.bottom -= 1;   
	g_dlgDebug1->SetWindowPos(NULL, initRect.left, initRect.top, initRect.Width(), initRect.Height(),SWP_SHOWWINDOW);   
	g_dlgDebug2->SetWindowPos(NULL, initRect.left, initRect.top, initRect.Width(), initRect.Height(),SWP_HIDEWINDOW); 
	m_tabMain.SetCurSel(0);
	
	//Ŀ¼��ʼ��
	CFileFind findini;
	BOOL ifFind = findini.FindFile(g_csPath);  
	if( !ifFind )  //ini����������һ��
	{
		//com�ں�
		WritePrivateProfileString(_T("COM"),_T("�ں�"),_T("0"),g_csPath);  
	}
	
	//��ȡini�ļ�
	int l_int=GetPrivateProfileInt(_T("COM"),_T("�ں�"),0,g_csPath);
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

	//��ʱ��
	KillTimer(1);
	gclsCom.CloseCom();

	//ȫ��ָ��
	g_dlgDebug1 = NULL;
	g_dlgDebug2 = NULL;
	return 1;
}

void CBICALDebugDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	//ˢ��com��������
	if (1==gstuRefresh.bComUpdate)
	{
		gstuRefresh.bComUpdate=0;
		g_dlgDebug1->GetDlgItem(IDC_EDIT_RS232_RECEIVE)->SetWindowText(gclsCom.stuInf.csRcv);
		g_dlgDebug1->m_edtRs232Rcv.LineScroll(g_dlgDebug1->m_edtRs232Rcv.GetLineCount(),0);
	}	
}


BOOL CBICALDebugDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	SYSQuit();
	return CDialogEx::DestroyWindow();
}
