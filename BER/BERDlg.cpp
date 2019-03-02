
// BERDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BER.h"
#include "BERDlg.h"
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

////相机采集图像回调函数
//void CALLBACK MessageCallback0(LONG lCommand,Alarmmes *pAlarmer,LPVehiclePassRec pAlarmInfo,DWORD dwBufLen,void* pUser)
//{	
//	//SetThreadAffinityMask(GetCurrentThread(), 1);
//	//CString csPassTime,csCaptureNum,csDriveChan,csPlateColor,csPlateType,csLicense,csIndex,csVehicleType,csVehicleColor,csSpeed,csIllegalType,csPicPath;
//	int i = 0;
//	char szFileName[3][256] = {0};//存放图像路径
//	//    char szPlateFileName[256] = {0};
//	char szSubPicFilePath[256] = {0};
//	//	char szSubPlateFilePath[256] = {0};
//	char szPicDataTime[256] = {0};
//	//	char szPlateDataTime[256] = {0};
//	CRect struRect; 
//	VehiclePassRec *l_struct_result_iiic;								//返回采集结果，包括图像缓存
//	l_struct_result_iiic = new VehiclePassRec;							//申请指定结构类型内存空间
//	memset(l_struct_result_iiic, 0, sizeof(VehiclePassRec));			//内存空间清0
//	memcpy(l_struct_result_iiic, pAlarmInfo, sizeof(VehiclePassRec));	//保存形参量
//	//时间
//	LARGE_INTEGER l_lgint_start, l_lgint_end;            
//	LARGE_INTEGER l_lgint_freq;
////	gclsTxt.GetHMS(g_struct_info_run.s_char_time_hms);//读取开始时间时：分：秒
//	QueryPerformanceFrequency(&l_lgint_freq);  
//	QueryPerformanceCounter(&l_lgint_start);
//
//	//每次会从相机中取3张图像保存在VehiclePassRec结构的picture【3】中，暂时只有第一张有效。这里是将之保存到本地图片
//	CString csTmpPath;
//	for (i = 0; i <1; i++)//图像识别时先存成bmp，下面pic文件夹分类中存成jpg
//	{
//
//		sprintf(szFileName[i], ("\\PIC\\原图%d.jpg"),i);
//		DWORD l_dword_return = 0;	
//		csTmpPath = gstuPathInf.csPathExe + (CString)szFileName[i];
//		HANDLE l_handle = CreateFile(csTmpPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//		if (l_handle == INVALID_HANDLE_VALUE)
//		{
//			//TRACE("create file failed! Error code:%d", WSAGetLastError());
//			break;
//		}	
//
//		WriteFile(l_handle, l_struct_result_iiic->picture[i].picBuf, l_struct_result_iiic->picture[i].picLen, &l_dword_return, NULL);  
//		//FlushFileBuffers(l_handle);
//		CloseHandle(l_handle);
//	}
//	//sprintf(g_struct_info_run.s_char_newest_pic_path,szFileName[i]);
//	i=0;//只有0图像有用
//	//图像处理
//	csTmpPath = gstuPathInf.csPathExe + (CString)szFileName[i];
//	//char * readPath = (LPSTR)(LPCTSTR)csTmpPath;//
//	char readPath[MAX_PATH];
//	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,csTmpPath,-1,readPath,sizeof(readPath),NULL,NULL); 
//	//readPath=csTmpPath.GetBuffer(csTmpPath.GetLength());//新获取其内部字符缓冲区的指针
//	//csTmpPath.ReleaseBuffer();
//
//	//加载pic到CPicture全局类对象，（并在控件中显示（该功能可能需在重绘中实现）//保存原始图片，之后图像处理保存处理后图片
//	HBITMAP hBitmap1 = gclsPic.LoadPicture(readPath);//(HBITMAP)LoadImage(NULL,bmp_path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
//
//	if(!hBitmap1)
//	{
//		AfxMessageBox(_T("Fail to reload picture"));
//		return;
//	}
//	CBitmap m_bmp;//创建类成员
//	m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
//	gclsPic.SaveBitmapToFile(&m_bmp, gstuPathInf.csPathExe+_T("\\PIC\\原图0.bmp"));
//	m_bmp.DeleteObject();
//	DeleteObject(hBitmap1);//记得删除	
//
//	QueryPerformanceCounter(&l_lgint_end);
//	
//	//记录结束
//	delete [] l_struct_result_iiic;
//	l_struct_result_iiic = NULL;
//	gstuRun.bOverCam=1;
//	//
//}

// CBERDlg 对话框




CBERDlg::CBERDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBERDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_tabMain);
}

BEGIN_MESSAGE_MAP(CBERDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CBERDlg::OnTcnSelchangeMainTab)
	//	ON_BN_CLICKED(IDOK, &CBERDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CBERDlg 消息处理程序

BOOL CBERDlg::SYSInit()
{
	CRect initRect;   // 标签控件客户区的位置和大小  
	CString csInit;

	//tab控件初始化
	m_tabMain.InsertItem(0, _T("运行")); 
	m_tabMain.InsertItem(1, _T("调试"));  
	g_dlgRun->Create(IDD_RUN_DIALOG, &m_tabMain);   
	g_dlgDebug->Create(IDD_DEBUG_DIALOG, &m_tabMain);
	m_tabMain.GetClientRect(&initRect);//tab控件大小
	initRect.left += 1;                  
	initRect.right -= 1;   
	initRect.top += 22;   
	initRect.bottom -= 1;   
	g_dlgRun->SetWindowPos(NULL, initRect.left, initRect.top, initRect.Width(), initRect.Height(),SWP_SHOWWINDOW);   
	g_dlgDebug->SetWindowPos(NULL, initRect.left, initRect.top, initRect.Width(), initRect.Height(),SWP_HIDEWINDOW); 
	m_tabMain.SetCurSel(0);
	//组合框初始化
	CString strValue,strTemps;
	//变量初始化
	gstuRun.bIsSelFt = 0;
	gstuRun.bRunStatChg = 0;
	gstuTrdStat._N=0;
	gstuTrdStat._RUN=0;
	gstuTrdStat._SHOW=0;
	gstuTrdStat.CHK_STOP=0;
	gstuTrdStat.DSP_STOP=0;
	gstuTrdStat.SCR_STOP=0;
	gstuTrdStat.SOT_STOP=0;
	gstuTrdStat.SYS_PAUSE=0;
	gstuTrdStat.TAK_STOP=0;
	gstuTrdStat._DEBUGEFG=0;//调试
	gstuSort.sortsn=0;
	gstuSort.sortsum=0;//1~24档总片数
	gstuRcgInfo.bChkPN=0;//区分方向标志初始化
	gstuRun.runCnt = 0;
	gstuRun.closeShake = 0;


	for(int i=0;i<30;i++)//每档已分档片数置零
	{
		gstuSort.sortnum[i]=0;
		gstuSort.alertnum[i]=1;
	}
	for(int i=0;i<5;i++)//5轴的平均方差初始化
	{
		gstuSort.sortavg[i]=0;
		gstuSort.sortstd[i]=0;
		gstuSort.std2[i]=0;
	}
	//ini文件初始化或读取
	int iniint=0;
	CString bmp_path;
	GetModuleFileName(NULL,bmp_path.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	bmp_path.ReleaseBuffer(); 
	int pos = bmp_path.ReverseFind('\\'); 
	gstuPathInf.csPathExe = bmp_path.Left(pos); 
	//目录初始化
	gstuPathInf.csPathIni = gstuPathInf.csPathExe+_T("\\INI\\SYSEFG.ini");
	gstuPathInf.csPathLog = gstuPathInf.csPathExe + _T("\\Record.log");
	///////////////
	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	if (!findini.FindFile(gstuPathInf.csPathExe+_T("\\INI\\*.*"))) 
	{
		if(!CreateDirectory(gstuPathInf.csPathExe+_T("\\INI\\"), NULL))
		{
			AfxMessageBox(_T("INI目录创建失败！"));
		}
	}
	if (!findini.FindFile(gstuPathInf.csPathExe+_T("\\PIC\\*.*"))) 
	{
		if(!CreateDirectory(gstuPathInf.csPathExe+_T("\\PIC\\"), NULL))
		{
			AfxMessageBox(_T("PIC目录创建失败！"));
		}
	}
	if (!findini.FindFile(gstuPathInf.csPathExe+_T("\\Data\\*.*"))) 
	{
		if(!CreateDirectory(gstuPathInf.csPathExe+_T("\\Data\\"), NULL))
		{
			AfxMessageBox(_T("Data目录创建失败！"));
		}
	}
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  
	if( !ifFind )  //ini创建，仅此一次
	{
		//运行参考数据
		WritePrivateProfileString(_T("分档设定"), _T("流程卡"), _T("0"), gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("分档设定"),_T("检测角：编号"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：度"),_T("0"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：分"),_T("0"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：秒"),_T("0"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("分档设定"),_T("分档值：分"),_T("0"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("分档设定"),_T("分档值：秒"),_T("0"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("分档设定"),_T("限定角：编号"),_T("3"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("分档设定"),_T("开始值：度"),_T("0"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("分档设定"),_T("开始值：分"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("分档设定"),_T("开始值：秒"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：度"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：分"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：秒"),_T("0"),gstuPathInf.csPathIni);
		//相机
		WritePrivateProfileString(_T("相机参数"),_T("最大曝光值"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("相机参数"),_T("最大增益"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("相机参数"),_T("亮度"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("相机参数"),_T("对比度"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("相机参数"),_T("饱和度"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("相机参数"),_T("锐度"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("相机参数"),_T("白平衡控制"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("相机参数"),_T("背光值"),_T("0"),gstuPathInf.csPathIni);
		//相机识别
		WritePrivateProfileString(_T("识别参数"),_T("直线点距"),_T("0"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("识别参数"),_T("缺陷点距"),_T("0"),gstuPathInf.csPathIni);   
		WritePrivateProfileString(_T("识别参数"),_T("阈值"),_T("0"),gstuPathInf.csPathIni);  
		WritePrivateProfileString(_T("识别参数"),_T("特征允许误差"),_T("0"),gstuPathInf.csPathIni);
		//校准系数
		WritePrivateProfileString(_T("校准系数"),_T("00"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("校准系数"),_T("01"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("校准系数"),_T("02"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("校准系数"),_T("10"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("校准系数"),_T("11"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("校准系数"),_T("12"),_T("0"),gstuPathInf.csPathIni);
		//位置
		WritePrivateProfileString(_T("取料中转"),_T("1X"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("取料中转"),_T("1Y"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("取料中转"),_T("2X"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("取料中转"),_T("2Y"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("取料中转"),_T("3X"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("取料中转"),_T("3Y"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("换片位置"),_T("上料中转"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("换片位置"),_T("换片上片"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("换片位置"),_T("换片下片"),_T("0"),gstuPathInf.csPathIni);
		WritePrivateProfileString(_T("换片位置"),_T("下料中转"),_T("0"),gstuPathInf.csPathIni);
		for(int i=0;i<30;i++)
		{
			strValue.Format(_T("%d档"),i);
			WritePrivateProfileString(_T("分档步数"),strValue,_T("0"),gstuPathInf.csPathIni);  
		}
		//com口号
		WritePrivateProfileString(_T("COM"),_T("口号"),_T("0"),gstuPathInf.csPathIni);  
	}

	//读取ini文件
	//GetPrivateProfileString(_T("分档设定"), _T("流程卡"), 0, strValue.GetBuffer(MAX_PATH), MAX_PATH, gstuPathInf.csPathIni);
	//strValue.ReleaseBuffer();
	//g_dlgRun->SetDlgItemText(IDC_EDIT_CARD, strValue);

	iniint=GetPrivateProfileInt(_T("分档设定"),_T("中心角度：度"),0,gstuPathInf.csPathIni)*10000+
		GetPrivateProfileInt(_T("分档设定"),_T("中心角度：分"),0,gstuPathInf.csPathIni)*100+
		GetPrivateProfileInt(_T("分档设定"),_T("中心角度：秒"),0,gstuPathInf.csPathIni); 

	g_dlgRun->SetDlgItemInt(IDC_EDT_MIDDEG,iniint);

	iniint=GetPrivateProfileInt(_T("分档设定"),_T("分档值：分"),0,gstuPathInf.csPathIni)*100+
		GetPrivateProfileInt(_T("分档设定"),_T("分档值：秒"),0,gstuPathInf.csPathIni);
	g_dlgRun->SetDlgItemInt(IDC_EDT_SORTDEG,iniint);

	iniint=GetPrivateProfileInt(_T("分档设定"),_T("切角值：秒"),0,gstuPathInf.csPathIni);
	g_dlgRun->SetDlgItemInt(IDC_EDT_CUTDEG,iniint);

	iniint=GetPrivateProfileInt(_T("分档设定"),_T("开始值：度"),0,gstuPathInf.csPathIni)*10000+
		GetPrivateProfileInt(_T("分档设定"),_T("开始值：分"),0,gstuPathInf.csPathIni)*100+
		GetPrivateProfileInt(_T("分档设定"),_T("开始值：秒"),0,gstuPathInf.csPathIni);

	g_dlgRun->SetDlgItemInt(IDC_EDT_MINDEG,iniint); 

	iniint=GetPrivateProfileInt(_T("分档设定"),_T("结束值：度"),0,gstuPathInf.csPathIni)*10000+
		GetPrivateProfileInt(_T("分档设定"),_T("结束值：分"),0,gstuPathInf.csPathIni)*100+
		GetPrivateProfileInt(_T("分档设定"),_T("结束值：秒"),0,gstuPathInf.csPathIni);
	g_dlgRun->SetDlgItemInt(IDC_EDT_MAXDEG,iniint);  

	iniint = GetPrivateProfileInt(_T("等效角设定"), _T("phi0：度"), 0, gstuPathInf.csPathIni) * 10000 +
		GetPrivateProfileInt(_T("等效角设定"), _T("phi0：分"), 0, gstuPathInf.csPathIni) * 100 +
		GetPrivateProfileInt(_T("等效角设定"), _T("phi0：秒"), 0, gstuPathInf.csPathIni);
	g_dlgRun->SetDlgItemInt(IDC_EDT_PHI0, iniint);

	iniint = GetPrivateProfileInt(_T("等效角设定"), _T("factor"), 0, gstuPathInf.csPathIni);
	
	g_dlgRun->SetDlgItemInt(IDC_EDT_FACTOR, iniint);
	//预设值保存到全局变量
	gstuSort.centerangle=GetPrivateProfileInt(_T("分档设定"),_T("中心角度：度"),0,gstuPathInf.csPathIni)*3600+
		GetPrivateProfileInt(_T("分档设定"),_T("中心角度：分"),0,gstuPathInf.csPathIni)*60+
		GetPrivateProfileInt(_T("分档设定"),_T("中心角度：秒"),0,gstuPathInf.csPathIni);     
	gstuSort.sortvalue=GetPrivateProfileInt(_T("分档设定"),_T("分档值：分"),0,gstuPathInf.csPathIni)*60+
		GetPrivateProfileInt(_T("分档设定"),_T("分档值：秒"),0,gstuPathInf.csPathIni);     
	gstuSort.cutvalue=	GetPrivateProfileInt(_T("分档设定"),_T("切角值：秒"),0,gstuPathInf.csPathIni);
	gstuSort.eleclow=GetPrivateProfileInt(_T("分档设定"),_T("开始值：度"),0,gstuPathInf.csPathIni)*3600+
		GetPrivateProfileInt(_T("分档设定"),_T("开始值：分"),0,gstuPathInf.csPathIni)*60+
		GetPrivateProfileInt(_T("分档设定"),_T("开始值：秒"),0,gstuPathInf.csPathIni);
	gstuSort.elechigh=GetPrivateProfileInt(_T("分档设定"),_T("结束值：度"),0,gstuPathInf.csPathIni)*3600+
		GetPrivateProfileInt(_T("分档设定"),_T("结束值：分"),0,gstuPathInf.csPathIni)*60+
		GetPrivateProfileInt(_T("分档设定"),_T("结束值：秒"),0,gstuPathInf.csPathIni);
	gstuSort.phi0 = GetPrivateProfileInt(_T("等效角设定"), _T("phi0：度"), 0, gstuPathInf.csPathIni) * 3600 +
		GetPrivateProfileInt(_T("等效角设定"), _T("phi0：分"), 0, gstuPathInf.csPathIni) * 60 +
		GetPrivateProfileInt(_T("等效角设定"), _T("phi0：秒"), 0, gstuPathInf.csPathIni);
	gstuSort.factor = GetPrivateProfileInt(_T("等效角设定"), _T("factor"), 0, gstuPathInf.csPathIni);
	//初始化radio
	gstuRcgInfo.bIsCir=1;
	((CButton *)g_dlgRun->GetDlgItem(IDC_RAD_CIR))->SetCheck(TRUE);
	//初始化组合框
	g_dlgRun->m_cmb_sort1.AddString(_T("光轴"));
	g_dlgRun->m_cmb_sort1.AddString(_T("电轴"));
	g_dlgRun->m_cmb_sort1.AddString(_T("等效角度"));
	g_dlgRun->m_cmb_sort1.AddString(_T("原始光轴"));
	g_dlgRun->m_cmb_sort1.AddString(_T("原始电轴"));
	g_dlgRun->m_cmb_sort2.AddString(_T("光轴"));
	g_dlgRun->m_cmb_sort2.AddString(_T("电轴"));
	g_dlgRun->m_cmb_sort2.AddString(_T("等效角度"));
	//m_cmb_sort2.AddString(_T("原始光轴"));
	//m_cmb_sort2.AddString(_T("原始电轴"));
	g_dlgRun->m_cmb_sort2.AddString(_T("无"));

	gstuSort.sortsel1=GetPrivateProfileInt(_T("分档设定"),_T("检测角：编号"),0,gstuPathInf.csPathIni);
	gstuSort.sortsel2=GetPrivateProfileInt(_T("分档设定"),_T("限定角：编号"),0,gstuPathInf.csPathIni);
	g_dlgRun->m_cmb_sort1.SetCurSel(gstuSort.sortsel1);
	g_dlgRun->m_cmb_sort2.SetCurSel(gstuSort.sortsel2);
	//初始化Run页listcontrol控件
	CRect listRect;

	g_dlgRun->m_listSort.GetClientRect(&listRect);
	g_dlgRun->m_listSort.SetExtendedStyle(g_dlgRun->m_listSort.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	g_dlgRun->m_listSort.InsertColumn(0, _T("档位"), LVCFMT_CENTER, listRect.Width()/8*2, 0);   
	g_dlgRun->m_listSort.InsertColumn(1, _T("角度"), LVCFMT_CENTER, listRect.Width()/8*4, 1);   
	g_dlgRun->m_listSort.InsertColumn(2, _T("片数"), LVCFMT_CENTER, listRect.Width()/8*2, 2);

	for(int i=0;i<24;i++)
	{
		strValue="";
		strTemps.Format(_T("%d"),i+1);
		g_dlgRun->m_listSort.InsertItem(i, strTemps);   
		strTemps.Format(_T("%02d°"),(gstuSort.centerangle+gstuSort.sortvalue*(i-12))/3600);//度
		strValue+=strTemps;
		strTemps.Format(_T("%02d\'"),(gstuSort.centerangle+gstuSort.sortvalue*(i-12))%3600/60);//分
		strValue+=strTemps;
		strTemps.Format(_T("%02d\""),(gstuSort.centerangle+gstuSort.sortvalue*(i-12))%60);//秒
		strValue+=strTemps;
		g_dlgRun->m_listSort.SetItemText(i, 1, strValue);   
		g_dlgRun->m_listSort.SetItemText(i, 2, _T("0"));   
	}
	g_dlgRun->m_listSort.InsertItem(24, _T("25")); g_dlgRun->m_listSort.SetItemText(24, 1, _T("电轴-"));g_dlgRun->m_listSort.SetItemText(24, 2, _T("0"));   
	g_dlgRun->m_listSort.InsertItem(25, _T("26")); g_dlgRun->m_listSort.SetItemText(25, 1, _T("电轴+"));g_dlgRun->m_listSort.SetItemText(25, 2, _T("0"));  
	g_dlgRun->m_listSort.InsertItem(26, _T("27")); g_dlgRun->m_listSort.SetItemText(26, 1, _T("光轴-"));g_dlgRun->m_listSort.SetItemText(26, 2, _T("0"));  
	g_dlgRun->m_listSort.InsertItem(27, _T("28")); g_dlgRun->m_listSort.SetItemText(27, 1, _T("光轴+"));g_dlgRun->m_listSort.SetItemText(27, 2, _T("0"));  
	g_dlgRun->m_listSort.InsertItem(28, _T("29")); g_dlgRun->m_listSort.SetItemText(28, 1, _T("激光检测不出"));g_dlgRun->m_listSort.SetItemText(28, 2, _T("0"));  
	g_dlgRun->m_listSort.InsertItem(29, _T("30")); g_dlgRun->m_listSort.SetItemText(29, 1, _T("角度检测不出"));g_dlgRun->m_listSort.SetItemText(29, 2, _T("0"));  
	g_dlgRun->m_listSort.InsertItem(30, _T("合计")); g_dlgRun->m_listSort.SetItemText(30, 1, _T("总测数"));g_dlgRun->m_listSort.SetItemText(30, 2, _T("0"));  

	g_font.CreatePointFont(700,_T("黑体"),NULL);
	g_font2.CreatePointFont(200,_T("黑体"),NULL);
	g_font3.CreatePointFont(450,_T("黑体"),NULL);
	g_dlgRun->GetDlgItem(IDC_PIC_SORT)->SetFont(&g_font); 
	g_dlgRun->GetDlgItem(IDC_PIC_SORT2)->SetFont(&g_font3); 
	g_dlgRun->GetDlgItem(IDC_PIC_SORT3)->SetFont(&g_font2); 
	g_dlgRun->GetDlgItem(IDC_BTN_RUN)->SetFont(&g_font2); 
	/*CWnd * pWnd = g_dlgRun->GetDlgItem(IDC_PIC_SORT);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	g_dlgRun->GetDlgItem(IDC_PIC_SORT)->GetClientRect(&rect);
	g_dlgRun->GetDlgItem(IDC_SHOW_VAL)->SetWindowPos(pWnd,rect.left+14,rect.top+14,rect.right/3+14,rect.bottom/3+14,SWP_SHOWWINDOW);// MoveWindow(,1);
	pWnd->ReleaseDC(pDC);
	*/
	//g_dlgDevice->SetDlgItemInt(IDC_EDT_COM,2);//会出错
	//定时器
	SetTimer(SHOW_WINDOWS, 100, NULL);
	//相机相关
	/*NET_Init();
	IIICStartConnect();mmy201608*/
	gnChannel = gclsCamera.GetKsj(KSJ_UC500M_MRYY);
	if (0 > gnChannel)
		AfxMessageBox(_T("KSJ_UC500M_MRYY 相机打开失败	,请检查是否连接该相机"));
	//获取信息
	SYSTEM_INFO siSysInfo;

	// Copy the hardware information to the SYSTEM_INFO structure. 

	GetSystemInfo(&siSysInfo); 
	gstuTrdStat.unProcNum=siSysInfo.dwNumberOfProcessors;


	//刷新数据初始化
	gstuRefresh.unStatRun=255;
	gstuRefresh.dRT=-1;
	gstuRefresh.unN=-1;
	// Display the contents of the SYSTEM_INFO structure. 
	/*CString TEMP=NULL,TEMP2=NULL;
	TEMP2.Format(_T("Hardware information: \n"));  
	TEMP+=TEMP2;
	TEMP2.Format(_T("  OEM ID: %u\n"), siSysInfo.dwOemId);
	TEMP+=TEMP2;
	TEMP2.Format(_T("  Number of processors: %u\n"), 
	siSysInfo.dwNumberOfProcessors); 
	TEMP+=TEMP2;
	TEMP2.Format(_T("  Page size: %u\n"), siSysInfo.dwPageSize); 
	TEMP+=TEMP2;
	TEMP2.Format(_T("  Processor type: %u\n"), siSysInfo.dwProcessorType); 
	TEMP+=TEMP2;
	TEMP2.Format(_T("  Minimum application address: %lx\n"), 
	siSysInfo.lpMinimumApplicationAddress); 
	TEMP+=TEMP2;
	TEMP2.Format(_T("  Maximum application address: %lx\n"), 
	siSysInfo.lpMaximumApplicationAddress); 
	TEMP+=TEMP2;
	TEMP2.Format(_T("  Active processor mask: %u\n"), 
	siSysInfo.dwActiveProcessorMask); 
	TEMP+=TEMP2;
	AfxMessageBox(TEMP);*/
	gTrdCom=AfxBeginThread(ComMsg, NULL , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
	gTrdCom->m_bAutoDelete = TRUE;
	gTrdCom->ResumeThread();

	g_pCpk = new CCpkLib(gstuPathInf.csPathExe);

	return 1;
}
//
//void CBERDlg::IIICStartConnect()
//{
//	//
//	char sUserName[32];
//	char sPassword[16];
//	char sAddressIp[16];
//	DWORD dwPort;
//	glCamID=-1;
//	//
//
//	char szLan[128] = {0};
//	char szTemp[256] = {0};
//	CString csTemp;
//
//	if (glCamID >= 0)
//	{
//		MessageBox(_T("连接已经启动!"));
//		return;
//	}
//	//test
//	strncpy(sUserName, "admin", 32);
//	strncpy(sPassword, "9999", 16);
//	//strncpy(sAddressIp, "192.168.1.107", 16);
//	strncpy(sAddressIp, "192.168.1.102", 16);
//	dwPort = 8000;
//	//end
//	DWORD dwReturn;
//	NET_GetConfig(1,IPC_GET_NETWORK_PARAM,&gstuCamNetCfg, sizeof(gstuCamNetCfg),&dwReturn);
//	//sprintf(sUserName,"%s.%s.%s.%s",gstuCamNetCfg.ip.S_un.S_addr[1]);
//
//	NET_SDK_DEVICEINFO m_struDeviceInfo;
//	memset(&m_struDeviceInfo, 0, sizeof(m_struDeviceInfo));
//
//	NET_SetMessageCallBack(MessageCallback0,NULL);
//
//	//	NET_DVR_SetLogToFile(3, NULL, FALSE);
//	//	NET_SetLogToFile(1,);//SDK日志功能未实现
//
//
//	NET_SetConnectTime(3000,3);
//	NET_SetReconnect(3000,TRUE);
//	glCamID = NET_Login(sAddressIp, (WORD)dwPort, sUserName, sPassword, NULL);
//
//
//
//	if (glCamID < 0)
//		AfxMessageBox(_T("连接IIIC相机失败！"));
//	else if(gclsIiic.OpenRtsp()!=1)
//		AfxMessageBox(_T("RTSP FAIL"));
//
//}


BOOL CBERDlg::DBGInit()
{

	CRect dbgRect;   // 标签控件客户区的位置和大小  
	CString csInit;
	//=====================CAMERA===========================
	////debug_camera

	//tab控件初始化
	g_dlgDebug->m_tabDebug.InsertItem(0, _T("相机调试")); 
	g_dlgDebug->m_tabDebug.InsertItem(1, _T("屏幕调试")); 
	g_dlgDebug->m_tabDebug.InsertItem(2, _T("机构调试"));  
	g_dlgCamera->Create(IDD_DIALOG_CAMERA, &g_dlgDebug->m_tabDebug);   
	g_dlgScreen->Create(IDD_DIALOG_SCREEN, &g_dlgDebug->m_tabDebug);
	g_dlgDevice->Create(IDD_DIALOG_DEVICE, &g_dlgDebug->m_tabDebug);
	g_dlgDebug->m_tabDebug.GetClientRect(&dbgRect);//tab控件大小
	dbgRect.left += 1;                  
	dbgRect.right -= 1;   
	dbgRect.top += 22;   
	dbgRect.bottom -= 1;   
	g_dlgCamera->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(),SWP_SHOWWINDOW);   
	g_dlgScreen->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(),SWP_HIDEWINDOW); 
	g_dlgDevice->SetWindowPos(NULL, dbgRect.left, dbgRect.top, dbgRect.Width(), dbgRect.Height(),SWP_HIDEWINDOW);
	g_dlgDebug->m_tabDebug.SetCurSel(0);
	//combobox必须放在tab控件之后
	g_dlgCamera->m_cmbAWB.AddString(_T("Auto"));
	g_dlgCamera->m_cmbAWB.AddString(_T("Day_D55"));
	g_dlgCamera->m_cmbAWB.AddString(_T("Day_D65"));
	g_dlgCamera->m_cmbAWB.AddString(_T("Florescent"));
	g_dlgCamera->m_cmbAWB.AddString(_T("Incandscent"));
	g_dlgCamera->m_cmbAWB.SetCurSel(0);
	g_dlgCamera->m_cmbBl.AddString(_T("Min"));
	g_dlgCamera->m_cmbBl.AddString(_T("Mid"));
	g_dlgCamera->m_cmbBl.AddString(_T("Max"));
	g_dlgCamera->m_cmbBl.SetCurSel(0);
	//ini

	int iniint=0;

	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	BOOL ifFind = findini.FindFile(gstuPathInf.csPathIni);  
	if( ifFind )  
	{//创建ini
		//读入ini文件中相应字段
		iniint=GetPrivateProfileInt(_T("相机参数"),_T("最大曝光值"),0,gstuPathInf.csPathIni);      
		g_dlgCamera->SetDlgItemInt(IDC_EDT_EV,iniint);
		iniint=GetPrivateProfileInt(_T("相机参数"),_T("最大增益"),0,gstuPathInf.csPathIni);  
		g_dlgCamera->SetDlgItemInt(IDC_EDT_GAIN,iniint);
		iniint=GetPrivateProfileInt(_T("相机参数"),_T("亮度"),0,gstuPathInf.csPathIni);  
		g_dlgCamera->SetDlgItemInt(IDC_EDT_BRIGHT,iniint);
		iniint=GetPrivateProfileInt(_T("相机参数"),_T("对比度"),0,gstuPathInf.csPathIni);  
		g_dlgCamera->SetDlgItemInt(IDC_EDT_CONTRAST,iniint);
		iniint=GetPrivateProfileInt(_T("相机参数"),_T("饱和度"),0,gstuPathInf.csPathIni);  
		g_dlgCamera->SetDlgItemInt(IDC_EDT_SATURATION,iniint);
		iniint=GetPrivateProfileInt(_T("相机参数"),_T("锐度"),0,gstuPathInf.csPathIni);  
		g_dlgCamera->SetDlgItemInt(IDC_EDT_SHARPNESS,iniint);
		iniint=GetPrivateProfileInt(_T("相机参数"),_T("白平衡控制"),0,gstuPathInf.csPathIni);  
		g_dlgCamera->m_cmbAWB.SetCurSel(iniint);
		iniint=GetPrivateProfileInt(_T("相机参数"),_T("背光值"),0,gstuPathInf.csPathIni);
		g_dlgCamera->m_cmbBl.SetCurSel(iniint);
		//相机识别
		iniint=GetPrivateProfileInt(_T("识别参数"),_T("直线点距"),0,gstuPathInf.csPathIni);
		g_dlgCamera->SetDlgItemInt(IDC_EDT_OUTPALLOWL,iniint);gstuRcgInfo.nPToL = iniint;
		iniint=GetPrivateProfileInt(_T("识别参数"),_T("缺陷点距"),0,gstuPathInf.csPathIni);
		g_dlgCamera->SetDlgItemInt(IDC_EDT_DEFECTALLOWL,iniint);gstuRcgInfo.nDefectPToL = iniint;
		iniint=GetPrivateProfileInt(_T("识别参数"),_T("阈值"),0,gstuPathInf.csPathIni);
		g_dlgCamera->SetDlgItemInt(IDC_EDT_THRESHOLD,iniint);gstuRcgInfo.nThreshold = iniint;
		iniint=GetPrivateProfileInt(_T("识别参数"),_T("特征允许误差"),0,gstuPathInf.csPathIni);
		g_dlgCamera->SetDlgItemInt(IDC_EDT_FTOUTPOINT,iniint);gstuRcgInfo.nAllowDefect = iniint;


	}
	else
		AfxMessageBox(_T("SYSEFG.ini没有创建"));

	gstuRcgInfo.bThrdAuto = 0;//自动阈值标识0
	gstuRcgInfo.bDelNoise = 0;
	gstuRcgInfo.bDebug = 0;
	//=======================DEVICE============================
	//com初始化
	int l_int=GetPrivateProfileInt(_T("COM"),_T("口号"),0,gstuPathInf.csPathIni);
	gclsCom.OpenCom(l_int);
	if(gclsCom.stuInf.bComOpen)
	{
		SetTimer(READ_COM, 10, NULL);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_COM,l_int);//将打开com的语句放sysinit会出错，原因是device的tab页面在dbginit中初始化，而在sys中还没有，导致找不到页面
	}
	//ini文件初始化或读取
	CString strValue;
	///////////////
	ifFind = findini.FindFile(gstuPathInf.csPathIni);  
	if( ifFind )  
	{
		for(int i=0;i<30;i++)
		{
			strValue.Format(_T("%d档"),i);
			gstuTrdStat.Ysort[i]=GetPrivateProfileInt(_T("分档步数"),strValue,0,gstuPathInf.csPathIni);  //写入全局变量
		}
	}
	//初始化Debug页listcontrol控件
	CRect listRect;

	g_dlgDevice->m_listDevice.GetClientRect(&listRect);
	g_dlgDevice->m_listDevice.SetExtendedStyle(g_dlgDevice->m_listDevice.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	g_dlgDevice->m_listDevice.InsertColumn(0, _T("档位"), LVCFMT_CENTER, listRect.Width()/8*2, 0);   
	g_dlgDevice->m_listDevice.InsertColumn(1, _T("步数"), LVCFMT_CENTER, listRect.Width()/8*2, 1);   
	g_dlgDevice->m_listDevice.InsertColumn(2, _T("说明"), LVCFMT_CENTER, listRect.Width()/8*4, 2);

	for(int i=0;i<30;i++)
	{
		strValue="";
		strValue.Format(_T("%d"),i+1);
		g_dlgDevice->m_listDevice.InsertItem(i, strValue);   
		strValue.Format(_T("%d"),gstuTrdStat.Ysort[i]);//度
		g_dlgDevice->m_listDevice.SetItemText(i, 1, strValue);   
		strValue.Format(_T("第%02d档步数"),i+1);//度
		g_dlgDevice->m_listDevice.SetItemText(i, 2, strValue);   
	}
	//cmb
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("0:1380"));
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("1:1380"));
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("2:1140"));
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("3:1140"));
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("4:860"));
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("5:760"));
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("6:660"));
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("7:520"));
	g_dlgDevice->m_cmb_efgstasel.AddString(_T("8:520"));
	//g_dlgDevice->m_cmb_efgstasel.AddString(_T("9"));
	g_dlgDevice->m_cmb_efgstasel.SetCurSel(0);
	//初始化窗口
	if( ifFind )  
	{
		iniint=GetPrivateProfileInt(_T("取料中转"),_T("1X"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_XY1X,iniint);
		iniint=GetPrivateProfileInt(_T("取料中转"),_T("1Y"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_XY1Y,iniint);
		iniint=GetPrivateProfileInt(_T("取料中转"),_T("2X"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_XY2X,iniint);
		iniint=GetPrivateProfileInt(_T("取料中转"),_T("2Y"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_XY2Y,iniint);
		iniint=GetPrivateProfileInt(_T("取料中转"),_T("3X"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_XY3X,iniint);
		iniint=GetPrivateProfileInt(_T("取料中转"),_T("3Y"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_XY3Y,iniint);

		iniint=GetPrivateProfileInt(_T("换片位置"),_T("上料中转"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_SORTUP,iniint);
		iniint=GetPrivateProfileInt(_T("换片位置"),_T("换片上片"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_SORTIN,iniint);
		iniint=GetPrivateProfileInt(_T("换片位置"),_T("换片下片"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_SORTOUT,iniint);
		iniint=GetPrivateProfileInt(_T("换片位置"),_T("下料中转"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_SORTDOWN,iniint);

		iniint=GetPrivateProfileInt(_T("取片校准点"),_T("1X"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_CLBPOS1X,iniint);
		gstuRcgInfo.nClbPosX[0] = iniint;
		iniint=GetPrivateProfileInt(_T("取片校准点"),_T("1Y"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_CLBPOS1Y,iniint);
		gstuRcgInfo.nClbPosY[0] = iniint;
		iniint=GetPrivateProfileInt(_T("取片校准点"),_T("2X"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_CLBPOS2X,iniint);
		gstuRcgInfo.nClbPosX[1] = iniint;
		iniint=GetPrivateProfileInt(_T("取片校准点"),_T("2Y"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_CLBPOS2Y,iniint);
		gstuRcgInfo.nClbPosY[1] = iniint;
		iniint=GetPrivateProfileInt(_T("取片校准点"),_T("3X"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_CLBPOS3X,iniint);
		gstuRcgInfo.nClbPosX[2] = iniint;
		iniint=GetPrivateProfileInt(_T("取片校准点"),_T("3Y"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_CLBPOS3Y,iniint);
		gstuRcgInfo.nClbPosY[2] = iniint;

		iniint=GetPrivateProfileInt(_T("换吸头点"),_T("X"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_SNCHGX,iniint);
		gstuRcgInfo.nSNCHGX = iniint;
		iniint=GetPrivateProfileInt(_T("换吸头点"),_T("Y"),0,gstuPathInf.csPathIni);
		g_dlgDevice->SetDlgItemInt(IDC_EDT_SNCHGY,iniint);
		gstuRcgInfo.nSNCHGY = iniint;

		//EFG开始检测位置
		gstuRun.nEfgStaSel=GetPrivateProfileInt(_T("EFG设定"),_T("开始检测位置"),0,gstuPathInf.csPathIni);
		g_dlgDevice->m_cmb_efgstasel.SetCurSel(gstuRun.nEfgStaSel);
	}

	//读取ini
	TCHAR strbuff[256];
	GetPrivateProfileString(_T("校准系数"),_T("00"),NULL,strbuff,80,gstuPathInf.csPathIni);  
	strValue=strbuff;
	gstuRcgInfo.g_factor[0][0] = _wtof(strValue.GetBuffer(strValue.GetLength()));
	strValue.ReleaseBuffer( );    
	GetPrivateProfileString(_T("校准系数"),_T("01"),NULL,strbuff,80,gstuPathInf.csPathIni);
	strValue=strbuff;
	gstuRcgInfo.g_factor[0][1] = _wtof(strValue.GetBuffer(strValue.GetLength()));
	strValue.ReleaseBuffer( ); 
	GetPrivateProfileString(_T("校准系数"),_T("02"),NULL,strbuff,80,gstuPathInf.csPathIni);
	strValue=strbuff;
	gstuRcgInfo.g_factor[0][2] = _wtof(strValue.GetBuffer(strValue.GetLength()));
	strValue.ReleaseBuffer( ); 
	GetPrivateProfileString(_T("校准系数"),_T("10"),NULL,strbuff,80,gstuPathInf.csPathIni); 
	strValue=strbuff;
	gstuRcgInfo.g_factor[1][0] = _wtof(strValue.GetBuffer(strValue.GetLength()));
	strValue.ReleaseBuffer( ); 
	GetPrivateProfileString(_T("校准系数"),_T("11"),NULL,strbuff,80,gstuPathInf.csPathIni);
	strValue=strbuff;
	gstuRcgInfo.g_factor[1][1] = _wtof(strValue.GetBuffer(strValue.GetLength()));
	strValue.ReleaseBuffer( ); 
	GetPrivateProfileString(_T("校准系数"),_T("12"),NULL,strbuff,80,gstuPathInf.csPathIni);
	strValue=strbuff;
	gstuRcgInfo.g_factor[1][2] = _wtof(strValue.GetBuffer(strValue.GetLength()));
	strValue.ReleaseBuffer( ); 
	return 1;
}

BOOL CBERDlg::SYSQuit()
{

	//定时器
	KillTimer(2);
	gclsCom.CloseCom();

	//相机相关
	/*if (glCamID >= 0)
	{
	NET_Logout(glCamID);
	glCamID = -1;
	}
	gclsPic.FreePicture();*/
	//全局指针
	g_dlgRun = NULL;
	g_dlgDebug = NULL;
	g_dlgCamera = NULL;
	g_dlgDevice = NULL;
	g_dlgScreen = NULL;
	g_font.DeleteObject();
	g_font2.DeleteObject();
	g_font3.DeleteObject();
	TerminateThread(gTrdCom,0);

	delete g_pCpk;
	return 1;
}

void CBERDlg::SurfaceRefresh()
{
	//刷新com接收内容
	if (1==gstuRefresh.bComUpdate)
	{
		gstuRefresh.bComUpdate=0;
		g_dlgDevice->GetDlgItem(IDC_EDIT_RS232_RECEIVE)->SetRedraw(FALSE);
		g_dlgDevice->GetDlgItem(IDC_EDIT_RS232_RECEIVE)->SetWindowText(gclsCom.stuInf.csRcv);
		g_dlgDevice->m_edtRs232Rcv.LineScroll(g_dlgDevice->m_edtRs232Rcv.GetLineCount(),0);
		g_dlgDevice->GetDlgItem(IDC_EDIT_RS232_RECEIVE)->SetRedraw(true);
	}
	//调试刷新运行数据
	if (gstuRefresh.unN != gstuTrdStat._N)
	{
		gstuRefresh.unN = gstuTrdStat._N;

		CString _degree,_sort,_avg,_std,tmp;
		for(int i=0;i<5;i++)
		{
			tmp.Format(_T("%02d°%02d'%02d\""),int((gstuSort.sortavg[i])+0.5)/3600,int((gstuSort.sortavg[i])+0.5)%3600/60,int((gstuSort.sortavg[i])+0.5)%60);
			_avg+=tmp+_T("\r\n");
			tmp.Format(_T("%02d°%02d'%02d\""),int((gstuSort.sortstd[i])+0.5)/3600,int((gstuSort.sortstd[i])+0.5)%3600/60,int((gstuSort.sortstd[i])+0.5)%60);
			_std+=tmp+_T("\r\n");
		}
		g_dlgRun->GetDlgItem(IDC_SHOW_AVG)->SetWindowText(_avg);
		g_dlgRun->GetDlgItem(IDC_SHOW_STD)->SetWindowText(_std);
		_degree.Format(_T("光轴角度：%d\r\n电轴角度：%d\r\n等效角度：%d\r\n原始光轴：%d\r\n原始电轴：%d"),g_dlgScreen->degree[0]*100000+g_dlgScreen->degree[1]*10000+g_dlgScreen->degree[2]*1000+g_dlgScreen->degree[3]*100+g_dlgScreen->degree[4]*10+g_dlgScreen->degree[5],
			g_dlgScreen->degree[6]*100000+g_dlgScreen->degree[7]*10000+g_dlgScreen->degree[8]*1000+g_dlgScreen->degree[9]*100+g_dlgScreen->degree[10]*10+g_dlgScreen->degree[11],
			g_dlgScreen->degree[12]*100000+g_dlgScreen->degree[13]*10000+g_dlgScreen->degree[14]*1000+g_dlgScreen->degree[15]*100+g_dlgScreen->degree[16]*10+g_dlgScreen->degree[17],
			g_dlgScreen->degree[18]*100000+g_dlgScreen->degree[19]*10000+g_dlgScreen->degree[20]*1000+g_dlgScreen->degree[21]*100+g_dlgScreen->degree[22]*10+g_dlgScreen->degree[23],
			g_dlgScreen->degree[24]*100000+g_dlgScreen->degree[25]*10000+g_dlgScreen->degree[26]*1000+g_dlgScreen->degree[27]*100+g_dlgScreen->degree[28]*10+g_dlgScreen->degree[29]);
		g_dlgRun->GetDlgItem(IDC_SHOW_DEG)->SetWindowText(_degree);
		//_degree.Format(_T("运行总时间：%d分\r\n平均时间：%.1f秒"),g_time.sum/60,g_time.avg);
		//g_dlgRun->GetDlgItem(IDC_RUN_SHOW)->SetWindowText(_degree);
		if(gstuSort.sortsn!=-1)
		{
			_sort="";
			_sort.Format(_T("%d"),gstuSort.sortnum[gstuSort.sortsn]);
			g_dlgRun->m_listSort.SetItemText(gstuSort.sortsn,2,_sort);   
			_sort.Format(_T("%d"),gstuRefresh.unN);//gstuSort.sortsum);
			g_dlgRun->m_listSort.SetItemText(30,2,_sort);   

			//UpdateData(1);
		}
		if(gstuSort.nocheckout>9)
		{

			//g_dlgRun->OnBnClickedRunStop();
			g_dlgDevice->AlarmCtrl(1);
			if(0 == gstuRefresh.alarm)
				AfxMessageBox(_T("角度检测不出"));
			g_dlgDevice->AlarmCtrl(0);
			gstuSort.nocheckout=0;
		}
		if(gstuSort.checkout0>9)
		{

			//g_dlgRun->OnBnClickedRunPause();
			g_dlgDevice->AlarmCtrl(1);
			if(0 == gstuRefresh.alarm)
				AfxMessageBox(_T("所测角检测不出"));
			g_dlgDevice->AlarmCtrl(0);
			gstuSort.checkout0=0;
		}
		g_dlgRun->SetDlgItemInt(IDC_PIC_SORT,gstuRun.unSort[0],0);
		g_dlgRun->SetDlgItemInt(IDC_PIC_SORT2,gstuRun.unSort[1],0);
		g_dlgRun->SetDlgItemInt(IDC_PIC_SORT3,gstuRun.unSort[2],0);
		tmp.Format(_T("%.2f秒"),gstuRun.dRT);
		g_dlgRun->SetDlgItemText(IDC_TIME_RUN,tmp);
	}
	/*CString csText;
	csText.Format(_T("%d"),18);
	g_dlgRun->GetDlgItem(IDC_PIC_SORT)->SetWindowText(csText);
	g_dlgRun->GetDlgItem(IDC_PIC_SORT2)->SetWindowText(csText);
	g_dlgRun->GetDlgItem(IDC_PIC_SORT3)->SetWindowText(csText);
	g_dlgRun->GetDlgItem(IDC_TIME_RUN)->SetWindowText(_T("220"));*/


	//刷新运行状态
	if(gstuRun.bRunStatChg)
	{
		int status;
		switch (gstuRun.unStatRun)
		{
		case 0://如果停止状态-》运行
#ifndef DEBUG_COM
			if((gstuRun.chStmCmd & (1<<2)))//等待下位机初始化完成
#endif
			{
				gclsCom.stuInf.chWrtID=0x00;
				gclsCom.stuInf.chRcvID=0x00;
				g_dlgRun->SetDlgItemText(IDC_STAT_RUN,_T("下位机初始化完成"));

				gstuRun.chStmCmd &= ~(1<<0);

				g_dlgDevice->EFGCut(true);//_0728
				/*	for(int i = 0; i<50 ; i++)
				{
				g_dlgDevice->EFGCtrl(1);
				Sleep(150);
				}*/
				/*CString card;
				g_dlgRun->GetDlgItemText(IDC_EDIT_CARD, card);*/

				gstuRun.unStatRun=2;
				gTrdRun=AfxBeginThread(RunThread, NULL , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
				gTrdRun->m_bAutoDelete = TRUE;
				gTrdRun->ResumeThread();
				gTrdScr=AfxBeginThread(ScreenThread , g_dlgRun->m_csvCard, THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
				gTrdScr->m_bAutoDelete = TRUE;
				gTrdScr->ResumeThread();
				gstuRun.bRunStatChg = 0;
				g_dlgRun->GetDlgItem(IDC_BTN_RUN)->EnableWindow(1);
				g_dlgRun->GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(1);
			}

			break;
		case 2://如果运行状态-》退出
#ifndef DEBUG_COM
			if((gstuRun.chStmCmd & (1<<2)) && 1==gstuRun.unTrdExitNum)
#endif
			{
				//流程卡记录
				/*CString card;
				g_dlgRun->GetDlgItemText(IDC_EDIT_CARD, card);*/
				if (g_dlgRun->m_csvCard) {
					//CCSVFile csv(gstuPathInf.csPathExe + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);
					CStringArray arr;
					CString str;
					CTime time;
					time = CTime::GetCurrentTime();
					arr.Add(_T("结束"));
					arr.Add(time.Format("%Y/%m/%d"));
					arr.Add(time.Format("%H:%M:%S"));
					g_dlgRun->m_csvCard->WriteData(arr);
					arr.RemoveAll();


					delete g_dlgRun->m_csvCard;
					g_dlgRun->m_csvCard = NULL;
				}
				// 


				g_dlgRun->SetDlgItemText(IDC_EDIT_PLANNED, _T(""));
				g_dlgRun->SetDlgItemText(IDC_EDIT_CARD, _T(""));
				gstuRun.unTrdExitNum++;//线程退出数++，将下位机当做一个线程
				g_dlgRun->SetDlgItemText(IDC_STAT_RUN,_T("下位机停止"));
				gstuRun.bRunStatChg = 0;
				g_dlgRun->GetDlgItem(IDC_BTN_RUN)->EnableWindow(1);
				g_dlgRun->GetDlgItem(IDC_BTN_PAUSE)->EnableWindow(0);
				gclsCom.stuInf.csRcv="";
				gstuRefresh.bComUpdate=1;
			}

			break;
		default:
			break;
		}
	}

	//刷新运行按钮
	if (gstuRefresh.unStatRun!=gstuRun.unStatRun)
	{
		switch(gstuRun.unStatRun)
		{
		case 0:
			g_dlgRun->GetDlgItem(IDC_BTN_RUN)->SetWindowText(_T("已停止"));
			BtnCtl(true);
			break;
		case 1:
			g_dlgRun->GetDlgItem(IDC_BTN_RUN)->SetWindowText(_T("已暂停"));
			break;
		case 2:
			g_dlgRun->GetDlgItem(IDC_BTN_RUN)->SetWindowText(_T("运行中"));
			BtnCtl(false);
			break;
		default:
			break;
		}
		gstuRefresh.unStatRun=gstuRun.unStatRun;
	}

	//处理退出//本来在退出按钮中实现，但是不理想
	if(!gstuRun.bTrdExit && gstuRun.bExit)//当线程都退出时bTrdExit==0,
	{
		gstuRun.bExit=0;
		gstuRun.unStatRun=0;
		gclsCom.stuInf.bIsRun=1;
		gclsImgRcg.g_stu_square.bJudgeFeature=0;
		CString csTmp,csAvg=_T("avg\t"),csStd=_T("std\t");
		csTmp.Format(_T("0-24:%d\r\n总测数:%d\r\n"),gstuSort.sortsum,gstuSort.sortsum+gstuSort.sortnum[24]+gstuSort.sortnum[25]+gstuSort.sortnum[26]+gstuSort.sortnum[27]+gstuSort.sortnum[28]+gstuSort.sortnum[29]);
		gclsTxt.TXTAddStr(gstuPathInf.csPathTxt,csTmp);
		for(int i=0;i<5;i++)
		{
			csTmp.Format(_T("%02d%02d%02d"),int((gstuSort.sortavg[i])+0.5)/3600,int((gstuSort.sortavg[i])+0.5)%3600/60,int((gstuSort.sortavg[i])+0.5)%60);
			csAvg+=csTmp+_T("\t");
			csTmp.Format(_T("%02d%02d%02d"),int((gstuSort.sortstd[i])+0.5)/3600,int((gstuSort.sortstd[i])+0.5)%3600/60,int((gstuSort.sortstd[i])+0.5)%60);
			csStd+=csTmp+_T("\t");
		}
		csAvg+=_T("\r\n");
		csStd+=_T("\r\n");
		gclsTxt.TXTAddStr(gstuPathInf.csPathTxt,csAvg);
		gclsTxt.TXTAddStr(gstuPathInf.csPathTxt,csStd);
		//CDialogEx::OnOK();
		CString _sort;
		for(int i= 0; i<30 ; i++)
		{
			_sort.Format(_T("%d\t%d\r\n"),i,gstuSort.sortnum[i]);
			gclsTxt.TXTAddStr(gstuPathInf.csPathTxt,_sort);
		}
		//g_dlgRun->m_listSort.SetItemText(30,2,_sort);   

		//g_dlgDevice->EFGCut(false);//_0728

	}


	if(gstuRun.bPosSet==1)
	{
		g_dlgDevice->SetDlgItemTextW(IDC_MSG,_T("XY轴位置设置完成"));
	}

	if(gstuRun.bPosSet==2)
	{
		g_dlgDevice->SetDlgItemTextW(IDC_MSG,_T("换片轴位置设置完成"));
	}

	//
	/*CString m_bmp_file=_T(".\\原图0.bmp");
	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	BOOL ifFind = findini.FindFile(m_bmp_file);  
	if( !ifFind )  
	return;
	CBitmap m_bmp;//创建类成员
	BITMAP bm;//存放位图信息的结构
	HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
	m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
	m_bmp.GetBitmap(&bm);
	CWnd * pWnd = g_dlgRun->GetDlgItem(IDC_PIC_SHOW);
	CDC* pDC = pWnd->GetDC();
	CRect rect;
	g_dlgRun->GetDlgItem(IDC_PIC_SHOW)->GetClientRect(&rect);
	CDC memDC;        //定义一个设备
	CClientDC dc1(this);      //获取客户
	memDC.CreateCompatibleDC( &dc1 );
	memDC.SelectObject( m_bmp );  //为设备选择对象
	pDC->SetStretchBltMode (HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 1000, 500, SRCCOPY);
	memDC.DeleteDC();	
	m_bmp.DeleteObject();
	DeleteObject(hBitmap1);//记得删除	
	pWnd->ReleaseDC(pDC);*/
}

//集中控制按钮的enable和disable，防止在运行时乱点
void CBERDlg::BtnCtl(bool ctl)
{
	g_dlgRun->GetDlgItem(IDC_COMBO_SORT1)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_COMBO_SORT2)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_BTN_SORTSET)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDT_CUTDEG)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDT_MIDDEG)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDT_SORTDEG)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDT_MINDEG)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDT_MAXDEG)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDT_PHI0)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDT_FACTOR)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDT_TXTNM)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_RAD_SQU)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_RAD_CIR)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_BTN_STANDARD)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_CHK_PN)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_BTN_STDCHECK)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_BTN_CPK)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDIT_PLANNED)->EnableWindow(ctl);
	g_dlgRun->GetDlgItem(IDC_EDIT_CARD)->EnableWindow(ctl);
}

BOOL CBERDlg::OnInitDialog()
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
	DBGInit();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBERDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBERDlg::OnPaint()
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
HCURSOR CBERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBERDlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if(gstuRun.unStatRun==2)
		m_tabMain.SetCurSel(0);
	CRect tabRect; 
	m_tabMain.GetClientRect(&tabRect);   
	tabRect.left += 1;   
	tabRect.right -= 1;   
	tabRect.top += 22;   
	tabRect.bottom -= 1;   
	switch (m_tabMain.GetCurSel())   
	{   
	case 0:   
		g_dlgRun->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   
		g_dlgDebug->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
		break;
	case 1:   
		g_dlgRun->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
		g_dlgDebug->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   
		break;   
	default:   
		break;   
	}
	*pResult = 0;
}


//void CBERDlg::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	//CDialogEx::OnOK();
//}


void CBERDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case READ_COM:
		{

		}
		break;
	case SHOW_WINDOWS:
		{
			SurfaceRefresh();
		}
		break;
	case RESEND_COM:
		{
			gclsCom.ReSend();
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CBERDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	/*char chTmp[10];			
	gclsTxt.GetHMS(chTmp);
	CString csTmp,csTmp2;
	csTmp2 = chTmp;
	csTmp.Format(_T("\t%d\r\n"),gclsIiic.nOpenCnt);
	csTmp = csTmp2 + csTmp;
	gclsTxt.TXTAddStr(gstuPathInf.csPathExe+"\\RtspON.txt",csTmp);*/
	SYSQuit();
	return CDialogEx::DestroyWindow();
}


void CBERDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(gstuRun.unStatRun!=0)
	{
		int status=AfxMessageBox(_T("运行中是否强退？"),MB_OKCANCEL);
		if(status==2)//按确定退出
		{
			return;
		}
	}
	CDialogEx::OnClose();
}



BOOL CBERDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//判断是否为键盘消息
	if(pMsg->message == WM_KEYDOWN)
	//if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//判断是否按下键盘Enter键
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			break;
    case VK_SPACE:
      g_dlgDevice->EFGCtrl(true);
      break;
		case VK_ESCAPE:
			//OnCancel();
			return TRUE;
		/*case 'X':
			if(GetKeyState(VK_CONTROL)<0 )
				g_dlgRun->OnBnClickedBtnStandard();*/
			default:
				break;
			}
			//return TRUE;
			//Sleep(100);
		}
		return CDialogEx::PreTranslateMessage(pMsg);
}


//void CBERDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if (WM_KEYFIRST <= nChar && nChar <= WM_KEYLAST)
//	{
//		//判断是否按下键盘Enter键
//		switch(nChar)
//		{
//		case VK_RETURN:
//			break;
//		case VK_SPACE:
//			g_dlgRun->OnBnClickedBtnRun();
//			break;
//		case VK_ESCAPE:
//			OnCancel();
//			break;
//		default:
//			break;
//		}
//		//return TRUE;
//		//Sleep(100);
//	}
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}
