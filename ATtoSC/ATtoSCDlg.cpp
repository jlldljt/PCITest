
// ATtoSCDlg.cpp : ʵ���ļ�
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
extern void AlertCtrl(bool stat);//���ƾ���
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


// CATtoSCDlg �Ի���




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
	ON_MESSAGE(WM_DISPLAY,OnDisplay)//WM_DISPLAY��Ϣӳ��
	ON_MESSAGE(WM_DLGALM,OnCreateDlg)//��Ϣӳ��
	ON_BN_CLICKED(IDCANCEL, &CATtoSCDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
LRESULT CATtoSCDlg::OnDisplay(WPARAM wPrarm,LPARAM lParam)
{
	if(g_status._SHOW==1)//��ʾ��λ
	{
		g_status._SHOW=0;
		CString strXY,strTmp;
		strTmp.Format(_T("%d\t"),g_sort.sortsn);
		//m_Run.GetDlgItemText(IDC_STATIC_SHOW,strXY);
		//CFont font;
		//font.CreatePointFont(800,_T("����"),NULL);
		//m_Run.GetDlgItem(IDC_STATIC_SHOW)->SetFont(&font);
		//strXY+=strTmp;
		strXY=strTmp;
		m_Run.GetDlgItem(IDC_STATIC_SHOW)->SetWindowText(strXY);

	}
	if(updatesort)//��ʾ����Ļֵ
	{
		updatesort=0;
		CString _degree,_sort,_avg,_std,_r1;
		for(int i=0;i<5;i++)
		{
			CString tmp;
			tmp.Format(_T("%02d��%02d'%02d\""),int((g_sort.sortavg[i])+0.5)/3600,int((g_sort.sortavg[i])+0.5)%3600/60,int((g_sort.sortavg[i])+0.5)%60);
			_avg+=tmp+_T("\r\n");
			tmp.Format(_T("%02d��%02d'%02d\""),int((g_sort.sortstd[i])+0.5)/3600,int((g_sort.sortstd[i])+0.5)%3600/60,int((g_sort.sortstd[i])+0.5)%60);
			_std+=tmp+_T("\r\n");
		}
		m_Run.GetDlgItem(IDC_SHOW_AVG)->SetWindowText(_avg);
		m_Run.GetDlgItem(IDC_SHOW_STD)->SetWindowText(_std);
		_degree.Format(_T("����Ƕȣ�%d\r\n����Ƕȣ�%d\r\n��Ч�Ƕȣ�%d\r\nԭʼ���᣺%d\r\nԭʼ���᣺%d"),degree[0]*100000+degree[1]*10000+degree[2]*1000+degree[3]*100+degree[4]*10+degree[5],
			degree[6]*100000+degree[7]*10000+degree[8]*1000+degree[9]*100+degree[10]*10+degree[11],
			degree[12]*100000+degree[13]*10000+degree[14]*1000+degree[15]*100+degree[16]*10+degree[17],
			degree[18]*100000+degree[19]*10000+degree[20]*1000+degree[21]*100+degree[22]*10+degree[23],
			degree[24]*100000+degree[25]*10000+degree[26]*1000+degree[27]*100+degree[28]*10+degree[29]);
		m_Run.GetDlgItem(IDC_SHOW_DEGREE)->SetWindowText(_degree);
		//_0614
		_r1.Format(_T("R1:%d\r\nR1���᣺%d\r\nR1���᣺%d"),g_sort.R1Num,g_sort.R1t[g_sort.R1Num],g_sort.R1e[g_sort.R1Num]);
		m_Run.GetDlgItem(IDC_SHOW_R1)->SetWindowText(_r1);
		int tmpDeg[2];
		tmpDeg[0] = (degree[0]*10+degree[1])*3600+(degree[2]*10+degree[3])*60+(degree[4]*10+degree[5])+g_sort.R1t[g_sort.R1Num];
		tmpDeg[1] = (degree[6]*10+degree[7])*3600+(degree[8]*10+degree[9])*60+(degree[10]*10+degree[11])+g_sort.R1e[g_sort.R1Num];
		_degree.Format(_T("�������᣺%d\r\n�������᣺%d"),
			tmpDeg[0]/3600*10000+tmpDeg[0]%3600/60*100+tmpDeg[0]%60,tmpDeg[1]/3600*10000+tmpDeg[1]%3600/60*100+tmpDeg[1]%60);
		m_Run.GetDlgItem(IDC_SHOW_DEGCOR)->SetWindowText(_degree);
		//_degree.Format(_T("������ʱ�䣺%d��\r\nƽ��ʱ�䣺%.1f��"),g_time.sum/60,g_time.avg);
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
			AfxMessageBox(_T("�Ƕȼ�ⲻ��"));
			AlertCtrl(0);
		}
		if(g_sort.checkout0>9)
		{
			g_sort.checkout0=0;
			//m_Run.OnBnClickedRunPause();
			AlertCtrl(1);
			AfxMessageBox(_T("����Ǽ�ⲻ��"));
			AlertCtrl(0);
		}
	}
	if(debugupdate)//�����µ�ʵʱ��ʾ
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
	MyMsgBoxReadIo2("������û��ȥ");
	return 0;
}
// CATtoSCDlg ��Ϣ�������

BOOL CATtoSCDlg::OnInitDialog()
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
	g_status._DEBUGSCR = 0;
	//ini�ļ���ʼ�����ȡ
	CString strFilePath,txtFilePath;
	int iniint=0;
	GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	strFilePath.ReleaseBuffer(); 
	int pos = strFilePath.ReverseFind('\\'); 
	txtFilePath = strFilePath.Left(pos)+_T("\\R1Cor.txt"); 
	strFilePath = strFilePath.Left(pos)+_T("\\SortSet.ini"); 
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(strFilePath);  
  CString strValue;
	if( !ifFind )  
	{
		AfxMessageBox(_T("SortSet.ini ������"));
		exit(0);
		/*WritePrivateProfileString(_T("�ֵ��趨"),_T("����Ƭ"),_T("0"),strFilePath); 
		
		WritePrivateProfileString(_T("�ֵ��趨"), _T("���̿�"), _T("0"), strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("���ǣ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("�ֵ�ֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("�ֵ�ֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("�޶��ǣ����"),_T("1"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),_T("0"),strFilePath);
		for(int i=0;i<33;i++)
		{
		strValue.Format(_T("%d��"),i+1);
		WritePrivateProfileString(_T("�ֵ�����"),strValue,_T("0"),strFilePath);
		}
		WritePrivateProfileString(_T("�ȴ�ʱ��"),_T("X��ȡ��"),_T("100"),strFilePath);
		WritePrivateProfileString(_T("�ȴ�ʱ��"),_T("X������"),_T("100"),strFilePath);
		WritePrivateProfileString(_T("�ȴ�ʱ��"),_T("Y��ȡ��"),_T("100"),strFilePath);
		WritePrivateProfileString(_T("�ȴ�ʱ��"),_T("Y������"),_T("100"),strFilePath);
		WritePrivateProfileString(_T("�ȴ�ʱ��"),_T("����"),_T("20"),strFilePath);
		for(int i=0;i<21;i++)
		{
		strValue.Format(_T("%d��"),i+1);
		WritePrivateProfileString(_T("����Ƭ�ֵ�����"),strValue,_T("0"),strFilePath);
		}*/
	}
	g_sort.needCheck = 1;
	//д��ini�ļ�����Ӧ�ֶ�
	g_sort.islongsqu=GetPrivateProfileInt(_T("�ֵ��趨"),_T("����Ƭ"),0,strFilePath);    
	//itemnum���
	if (g_sort.islongsqu)
	{
		g_sort.itemnum = GetPrivateProfileInt(_T("�����趨"), _T("����Ƭ��λ��"), 0, strFilePath);
		g_sort.itemstr = _T("����Ƭ�ֵ�����");
	}
	else
	{
		g_sort.itemnum = GetPrivateProfileInt(_T("�����趨"), _T("��λ��"), 0, strFilePath);
		g_sort.itemstr = _T("�ֵ�����");
	}

	g_shakeE = GetPrivateProfileInt(_T("�����趨"), _T("��̨��ƽ�л�"), 0, strFilePath);

	g_status._MOTOR_NUM = GetPrivateProfileInt(_T("�����趨"), _T("�����"), 0, strFilePath);

	if (3 != g_status._MOTOR_NUM)
	{
		g_status._MOTOR_NUM = 2;
	}

	CString bmp_path;
	GetModuleFileName(NULL,bmp_path.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	bmp_path.ReleaseBuffer(); 
	pos = bmp_path.ReverseFind('\\'); 
	g_path = bmp_path.Left(pos); 

	pathSaveR1=g_path + (_T("\\check\\��̬����.txt"));
	CTime time;
	time=CTime::GetCurrentTime();
	pathSaveDegree=g_path + (_T("\\check\\SortDegree"))+time.Format("%d-%H-%M")+(_T(".txt"));



	//tab�ؼ���ʼ��
	CRect tabRect;   // ��ǩ�ؼ��ͻ�����λ�úʹ�С   
	m_Tab.InsertItem(0, _T("����")); 
	m_Tab.InsertItem(1, _T("����"));  
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
	//��ʼ������

  g_pCpk = new CCpkLib(g_path);

	//AC��ʼ��
	g_hDevice=AC6641_OpenDevice(0);
	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("ac6641 ��ʧ��"));
		return FALSE;
	}
	if(3 == g_status._MOTOR_NUM)
		AC6641_SetIoMode(g_hDevice,0x3F,0x7);//��ģʽ����Ӧ7-0��11-8��,ǰ0-5;8-10��=1Ϊ�����6,7,11=0Ϊ����
	else
		AC6641_SetIoMode(g_hDevice,0x3f,0x0);//��ģʽ����Ӧ7-0��11-8��,ǰ����=1Ϊ�����������=0Ϊ����
	int ioData=0x77;
	AC6641_DO(g_hDevice,4,ioData);//01110111
	ioData=0x00;
	AC6641_DO(g_hDevice,0,ioData);//��Ƭ��p0p1��ʼ��
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CATtoSCDlg::OnPaint()
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
HCURSOR CATtoSCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CATtoSCDlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CRect tabRect;    // ��ǩ�ؼ��ͻ�����Rect   
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*if(0!=g_status._RUN || debugupdate || testStart || dynamicDetect)
	{
	int status=0;
	status=AfxMessageBox(_T("�߳�δ�˳�,�˳���"));
	if(status==2)//��ȷ������
	return;
	}
	AC6641_CloseDevice(g_hDevice); //�رղ������
	//
	//stopVGA(m_hWnd, &hRGBCapture1, pCaptureBuffer1);
	m_Debug.OnBnClickedStopVga();*/
	//
	CDialogEx::OnCancel();
}


void CATtoSCDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//return;
	if(0!=g_status._RUN || debugupdate || testStart || dynamicDetect)
	{
		int status=0;
		status=AfxMessageBox(_T("�߳�δ�˳�,�˳���"),MB_OKCANCEL);
		if(status==2)//��ȷ������
			return;
	}
	AC6641_CloseDevice(g_hDevice); //�رղ������
	//
	//stopVGA(m_hWnd, &hRGBCapture1, pCaptureBuffer1);
	m_Debug.OnBnClickedStopVga();
  delete g_pCpk;
	CDialogEx::OnCancel();
	CDialogEx::OnClose();
}


BOOL CATtoSCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//�ж��Ƿ��¼���Enter��
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
