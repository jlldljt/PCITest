
// ATtoSCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ATtoSC.h"
#include "ATtoSCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern threadstatus g_status;
extern HANDLE g_hDevice;
extern sortchip g_sort;
//extern runtime g_time;
extern int degree[30];
extern bool updatesort;
extern bool debugupdate;
extern bool testStart;
extern bool dynamicDetect;
extern void AlertCtrl(bool stat);//控制警报
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


// CATtoSCDlg 对话框




CATtoSCDlg::CATtoSCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CATtoSCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CATtoSCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CATtoSCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CATtoSCDlg::OnTcnSelchangeMainTab)
	ON_MESSAGE(WM_DISPLAY,OnDisplay)//WM_DISPLAY消息映射
	ON_MESSAGE(WM_DLGALM,OnCreateDlg)//消息映射
	ON_BN_CLICKED(IDCANCEL, &CATtoSCDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
LRESULT CATtoSCDlg::OnDisplay(WPARAM wPrarm,LPARAM lParam)
{
	if(g_status._SHOW==1)//显示大档位
	{
		g_status._SHOW=0;
		CString strXY,strTmp;
		strTmp.Format(_T("%d\t"),g_sort.sortsn);
		//m_Run.GetDlgItemText(IDC_STATIC_SHOW,strXY);
		//CFont font;
		//font.CreatePointFont(800,_T("宋体"),NULL);
		//m_Run.GetDlgItem(IDC_STATIC_SHOW)->SetFont(&font);
		//strXY+=strTmp;
		strXY=strTmp;
		m_Run.GetDlgItem(IDC_STATIC_SHOW)->SetWindowText(strXY);

	}
	if(updatesort)//显示度屏幕值
	{
		updatesort=0;
		CString _degree,_sort,_avg,_std,_r1;
		for(int i=0;i<5;i++)
		{
			CString tmp;
			tmp.Format(_T("%02d°%02d'%02d\""),int((g_sort.sortavg[i])+0.5)/3600,int((g_sort.sortavg[i])+0.5)%3600/60,int((g_sort.sortavg[i])+0.5)%60);
			_avg+=tmp+_T("\r\n");
			tmp.Format(_T("%02d°%02d'%02d\""),int((g_sort.sortstd[i])+0.5)/3600,int((g_sort.sortstd[i])+0.5)%3600/60,int((g_sort.sortstd[i])+0.5)%60);
			_std+=tmp+_T("\r\n");
		}
		m_Run.GetDlgItem(IDC_SHOW_AVG)->SetWindowText(_avg);
		m_Run.GetDlgItem(IDC_SHOW_STD)->SetWindowText(_std);
		_degree.Format(_T("光轴角度：%d\r\n电轴角度：%d\r\n等效角度：%d\r\n原始光轴：%d\r\n原始电轴：%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
			degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
			degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
			degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
			degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);
		m_Run.GetDlgItem(IDC_SHOW_DEGREE)->SetWindowText(_degree);
		//_0614
		_r1.Format(_T("R1:%d\r\nR1光轴：%d\r\nR1电轴：%d"),g_sort.R1Num,g_sort.R1t[g_sort.R1Num],g_sort.R1e[g_sort.R1Num]);
		m_Run.GetDlgItem(IDC_SHOW_R1)->SetWindowText(_r1);
		int tmpDeg[2];
		tmpDeg[0] = (degree[0]*10+degree[1])*3600+(degree[2]*10+degree[3])*60+(degree[4]*10+degree[5])+g_sort.R1t[g_sort.R1Num];
		tmpDeg[1] = (degree[6]*10+degree[7])*3600+(degree[8]*10+degree[9])*60+(degree[10]*10+degree[11])+g_sort.R1e[g_sort.R1Num];
		_degree.Format(_T("修正光轴：%d\r\n修正电轴：%d"),
			tmpDeg[0]/3600*10000+tmpDeg[0]%3600/60*100+tmpDeg[0]%60,tmpDeg[1]/3600*10000+tmpDeg[1]%3600/60*100+tmpDeg[1]%60);
		m_Run.GetDlgItem(IDC_SHOW_DEGCOR)->SetWindowText(_degree);
		//_degree.Format(_T("运行总时间：%d分\r\n平均时间：%.1f秒"),g_time.sum/60,g_time.avg);
		//m_Run.GetDlgItem(IDC_RUN_SHOW)->SetWindowText(_degree);
		if(g_sort.sortsn!=0)
		{
			_sort="";
			_sort.Format(_T("%d"),g_sort.sortnum[g_sort.sortsn-1]);
			m_Run.m_runlist.SetItemText(g_sort.sortsn-1,2,_sort);   
			_sort.Format(_T("%d"),g_sort.sortsum+g_sort.sortnum[g_sort.itemnum]+g_sort.sortnum[g_sort.itemnum+1]+g_sort.sortnum[g_sort.itemnum+2]+g_sort.sortnum[g_sort.itemnum+3]+g_sort.sortnum[g_sort.itemnum+4]+g_sort.sortnum[g_sort.itemnum+5]);
			m_Run.m_runlist.SetItemText(g_sort.itemnum+6,2,_sort);   

			//UpdateData(1);
		}
		if(g_sort.nocheckout>9)
		{
			g_sort.nocheckout=0;
			//m_Run.OnBnClickedRunStop();
			AlertCtrl(1);
			AfxMessageBox(_T("角度检测不出"));
			AlertCtrl(0);
		}
		if(g_sort.checkout0>9)
		{
			g_sort.checkout0=0;
			//m_Run.OnBnClickedRunPause();
			AlertCtrl(1);
			AfxMessageBox(_T("所测角检测不出"));
			AlertCtrl(0);
		}
	}
	if(debugupdate)//调试下的实时显示
	{
		CButton* pBtn1 = (CButton*)m_Debug.GetDlgItem(IDC_SHOCK);
		if(ShakeDown)// 0000 0100
			pBtn1->SetCheck(1);
		else
			pBtn1->SetCheck(0);
		CButton* pBtn2 = (CButton*)m_Debug.GetDlgItem(IDC_MEASURE_END);
		if(AC6641_DI(g_hDevice,6) & 0x02)// 0000 0010
			pBtn2->SetCheck(1);
		else
			pBtn2->SetCheck(0);
		//UpdateData(0);
		if(3 == g_status._MOTOR_NUM)
		{
			CButton* pBtn3 = (CButton*)m_Debug.GetDlgItem(IDC_CY_0);
			if((AC6641_DI(g_hDevice,11) & 0x02))// 0000 0010
				pBtn3->SetCheck(1);
			else
				pBtn3->SetCheck(0);
			CButton* pBtn4 = (CButton*)m_Debug.GetDlgItem(IDC_CY_1);
			if(AC6641_DI(g_hDevice,11) & 0x04)// 0000 0100
				pBtn4->SetCheck(1);
			else
				pBtn4->SetCheck(0);
		}
	}
	return 0;
}
LRESULT CATtoSCDlg::OnCreateDlg(WPARAM wPrarm,LPARAM lParam)
{
	MyMsgBoxReadIo2("震料器没下去");
	return 0;
}
// CATtoSCDlg 消息处理程序

BOOL CATtoSCDlg::OnInitDialog()
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
	g_status._DEBUGSCR = 0;
	//ini文件初始化或读取
	CString strFilePath,txtFilePath;
	int iniint=0;
	GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strFilePath.ReleaseBuffer(); 
	int pos = strFilePath.ReverseFind('\\'); 
	txtFilePath = strFilePath.Left(pos)+_T("\\R1Cor.txt"); 
	strFilePath = strFilePath.Left(pos)+_T("\\SortSet.ini"); 
	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	BOOL ifFind = findini.FindFile(strFilePath);  
  CString strValue;
	if( !ifFind )  
	{
		AfxMessageBox(_T("SortSet.ini 不存在"));
		exit(0);
		/*WritePrivateProfileString(_T("分档设定"),_T("长方片"),_T("0"),strFilePath); 
		
		WritePrivateProfileString(_T("分档设定"), _T("流程卡"), _T("0"), strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("检测角：编号"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：度"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：分"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：秒"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("分档值：分"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("分档值：秒"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("限定角：编号"),_T("1"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("开始值：度"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("开始值：分"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("开始值：秒"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：度"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：分"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：秒"),_T("0"),strFilePath);
		for(int i=0;i<33;i++)
		{
		strValue.Format(_T("%d档"),i+1);
		WritePrivateProfileString(_T("分档步数"),strValue,_T("0"),strFilePath);
		}
		WritePrivateProfileString(_T("等待时间"),_T("X轴取料"),_T("100"),strFilePath);
		WritePrivateProfileString(_T("等待时间"),_T("X轴上料"),_T("100"),strFilePath);
		WritePrivateProfileString(_T("等待时间"),_T("Y轴取料"),_T("100"),strFilePath);
		WritePrivateProfileString(_T("等待时间"),_T("Y轴上料"),_T("100"),strFilePath);
		WritePrivateProfileString(_T("等待时间"),_T("吹气"),_T("20"),strFilePath);
		for(int i=0;i<21;i++)
		{
		strValue.Format(_T("%d档"),i+1);
		WritePrivateProfileString(_T("长方片分档步数"),strValue,_T("0"),strFilePath);
		}*/
	}
	g_sort.needCheck = 1;
	//写入ini文件中相应字段
	g_sort.islongsqu=GetPrivateProfileInt(_T("分档设定"),_T("长方片"),0,strFilePath);    
	//itemnum相关
	if (g_sort.islongsqu)
	{
		g_sort.itemnum = GetPrivateProfileInt(_T("其他设定"), _T("长方片档位数"), 0, strFilePath);
		g_sort.itemstr = _T("长方片分档步数");
	}
	else
	{
		g_sort.itemnum = GetPrivateProfileInt(_T("其他设定"), _T("档位数"), 0, strFilePath);
		g_sort.itemstr = _T("分档步数");
	}

	g_shakeE = GetPrivateProfileInt(_T("其他设定"), _T("震动台电平切换"), 0, strFilePath);

	g_status._MOTOR_NUM = GetPrivateProfileInt(_T("其他设定"), _T("电机数"), 0, strFilePath);

	if (3 != g_status._MOTOR_NUM)
	{
		g_status._MOTOR_NUM = 2;
	}

	CString bmp_path;
	GetModuleFileName(NULL,bmp_path.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	bmp_path.ReleaseBuffer(); 
	pos = bmp_path.ReverseFind('\\'); 
	g_path = bmp_path.Left(pos); 

	pathSaveR1=g_path + (_T("\\check\\动态测量.txt"));
	CTime time;
	time=CTime::GetCurrentTime();
	pathSaveDegree=g_path + (_T("\\check\\SortDegree"))+time.Format("%d-%H-%M")+(_T(".txt"));



	//tab控件初始化
	CRect tabRect;   // 标签控件客户区的位置和大小   
	m_Tab.InsertItem(0, _T("运行")); 
	m_Tab.InsertItem(1, _T("调试"));  
	m_Run.Create(IDD_RUN, &m_Tab);   
	m_Debug.Create(IDD_DEBUG, &m_Tab);
	m_Tab.GetClientRect(&tabRect);
	tabRect.left += 1;                  
	tabRect.right -= 1;   
	tabRect.top += 22;   
	tabRect.bottom -= 1;   
	m_Run.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(),SWP_SHOWWINDOW);   
	m_Debug.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(),SWP_HIDEWINDOW); 
	m_Tab.SetCurSel(0);
	//初始化变量

  g_pCpk = new CCpkLib(g_path);

	//AC初始化
	g_hDevice=AC6641_OpenDevice(0);
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 打开失败"));
		return FALSE;
	}
	if(3 == g_status._MOTOR_NUM)
		AC6641_SetIoMode(g_hDevice,0x3F,0x7);//设模式，对应7-0和11-8组,前0-5;8-10组=1为输出，6,7,11=0为输入
	else
		AC6641_SetIoMode(g_hDevice,0x3f,0x0);//设模式，对应7-0和11-8组,前六组=1为输出，后六组=0为输入
	int ioData=0x77;
	AC6641_DO(g_hDevice,4,ioData);//01110111
	ioData=0x00;
	AC6641_DO(g_hDevice,0,ioData);//单片机p0p1初始化
	AC6641_DO(g_hDevice,2,ioData);
	AC6641_DO(g_hDevice,1,ioData);
	AC6641_DO(g_hDevice,3,ioData);
	AC6641_DO(g_hDevice,5,0x0e);
	if(3 == g_status._MOTOR_NUM)
	{
		AC6641_DO(g_hDevice,8,ioData);
		AC6641_DO(g_hDevice,9,ioData);
		ioData=0x03;
		AC6641_DO(g_hDevice,10,ioData);
	}


	return TRUE;
}

void CATtoSCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CATtoSCDlg::OnPaint()
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
HCURSOR CATtoSCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CATtoSCDlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	CRect tabRect;    // 标签控件客户区的Rect   
	m_Tab.GetClientRect(&tabRect);   
	tabRect.left += 1;   
	tabRect.right -= 1;   
	tabRect.top += 22;   
	tabRect.bottom -= 1;   
	switch (m_Tab.GetCurSel())   
	{   
	case 0:   
		m_Run.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   
		m_Debug.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
		break;
	case 1:   
		m_Run.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
		m_Debug.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   
		break;   
	default:   
		break;   
	}
	*pResult = 0;

}


void CATtoSCDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if(0!=g_status._RUN || debugupdate || testStart || dynamicDetect)
	{
	int status=0;
	status=AfxMessageBox(_T("线程未退出,退出？"));
	if(status==2)//按确定进入
	return;
	}
	AC6641_CloseDevice(g_hDevice); //关闭操作句柄
	//
	//stopVGA(m_hWnd, &hRGBCapture1, pCaptureBuffer1);
	m_Debug.OnBnClickedStopVga();*/
	//
	CDialogEx::OnCancel();
}


void CATtoSCDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//return;
	if(0!=g_status._RUN || debugupdate || testStart || dynamicDetect)
	{
		int status=0;
		status=AfxMessageBox(_T("线程未退出,退出？"),MB_OKCANCEL);
		if(status==2)//按确定进入
			return;
	}
	AC6641_CloseDevice(g_hDevice); //关闭操作句柄
	//
	//stopVGA(m_hWnd, &hRGBCapture1, pCaptureBuffer1);
	m_Debug.OnBnClickedStopVga();
  delete g_pCpk;
	CDialogEx::OnCancel();
	CDialogEx::OnClose();
}


BOOL CATtoSCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//判断是否按下键盘Enter键
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			break;
		case VK_SPACE:
			StartFTLong(1);
			Sleep(3000);
			StartFTLong(0);
			break;
		default:
			break;
		}
		return TRUE;

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
