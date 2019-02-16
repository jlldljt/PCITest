
// BER.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "BER.h"
#include "BERDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//全局
RunInfo gstuRun;
RefreshTmp gstuRefresh;
threadstatus gstuTrdStat;//线程全局结构变量
sortchip gstuSort;
CFont g_font,g_font2,g_font3;
CTXT gclsTxt;

//识别
//CIMGRecognition gclsImgRcg;
RecognitionInfo  gstuRcgInfo;
PathInfo gstuPathInf;
int SortChip(sortchip* s_sort,int *degree);
void SortAvgStd(unsigned int sum,int *angle,double *avg,double *std,long double *std2);
//相机相关
//CPicture gclsPic;//pictrue类对象
//SETTINGS_CAMERA gstuCamParam;//相机参数
//Network_Config_Data gstuCamNetCfg;//相机网络参数 
//LONG glCamID;//相机ID，初始化未-1
//VehiclePassRec gstuCamResult;//相机结果结构
//ksj相机
int gnChannel = -1;
CKSJ gclsCamera;
//线程
CWinThread* gTrdRun;//运行线程
CWinThread* gTrdCam;//相机线程
CWinThread* gTrdScr;//屏幕线程
CWinThread* gTrdClb;//校准线程
CWinThread* gTrdCom;
UINT RunThread(LPVOID pParam);//运行线程函数
UINT CameraThread(LPVOID pParam);//图像识别线程函数
UINT ScreenThread(LPVOID pParam);//运行线程函数
UINT CalibrationThread(LPVOID pParam);//校准线程
UINT ComMsg(LPVOID pParam);
void calparameter(double (*xy)[4],double *factor);
// CBERApp

BEGIN_MESSAGE_MAP(CBERApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

libvlc_media_player_t * vlc_player = NULL, *vlc_player_run = NULL;
BOOL gb_PlayOrNot[2]={0};
//vlc播放视频的回调函数
void CALLBACK RealDataCallBack0(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,void *pUser)
{
	;
}

// CBERApp 构造

CBERApp::CBERApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CBERApp 对象

CBERApp theApp;


// CBERApp 初始化

BOOL CBERApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("海盛BER"));

	CBERDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

UINT RunThread(LPVOID pParam)
{
	SetThreadAffinityMask(GetCurrentThread(), gstuTrdStat.unProcNum);
	LARGE_INTEGER l_lgint_start, l_lgint_end;            
	LARGE_INTEGER l_lgint_freq;
	QueryPerformanceFrequency(&l_lgint_freq);  
	int nN=0;
	int nCanShk = 0;
	int bShk=0;//抖动标记
	int bMov = 0;//移动次数标记
	int ln_count=0;//重发计数
	while(1)
	{
		
#ifndef DEBUG_COM
		do{
			if (gstuRun.bTrdExit)
			{
				gstuRun.unTrdExitNum++;//线程退出数++
				return 0;
			}
			Sleep(1);
		}while(! (gstuRun.chStmCmd & (1<<1)));
		TimeRecord(1,0);//无片坐标接收完成，计时结束
#else
		Sleep(3000);
#endif
		
		QueryPerformanceCounter(&l_lgint_start);
		gstuRun.bOverRcg=0;
		gclsImgRcg.g_stu_square.nN=0;
		nN=0;
		nCanShk = 0;
		
		TimeRecord(4,1);//启动拍照2，计时开始

		DWORD dwCode = 0;
		if (NULL == gTrdCam)
		{

		}
		else
		{
			do 
			{
				Sleep(1);
				GetExitCodeThread(gTrdCam->m_hThread , &dwCode);

			} while (dwCode == STILL_ACTIVE);

			gTrdCam->Delete();
			gTrdCam = NULL;
		}
		


		gTrdCam=AfxBeginThread(CameraThread , NULL , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
		if (NULL == gTrdCam)
		{
			AfxMessageBox(_T("启动拍照线程失败"));
			return 0;
		}
		gTrdCam->m_bAutoDelete = FALSE;
		gTrdCam->ResumeThread();

		TimeRecord(4,0);//线程启动，计时结束
		
		while(!gstuRun.bOverRcg)
		{
			if (gstuRun.bTrdExit)
			{
				gstuRun.unTrdExitNum++;//线程退出数++
				return 0;
			}
			Sleep(1);

			while(nN<gclsImgRcg.g_stu_square.nN && nN < 6)//提取坐标
			{
				if (gstuRun.bTrdExit)
				{
					gstuRun.unTrdExitNum++;//线程退出数++
					return 0;
				}
#ifndef DEBUG_COM
				if ((gstuRun.chStmCmd & (1<<1)) && !gstuRun.bTrdPause)//如果无片
				{
					if (gstuRcgInfo.bChkPN && !gstuRcgInfo.bIsCir)//如果判断正反面且是方形
					{
						if (gstuRcgInfo.nPN==gclsImgRcg.g_stu_square.bPN[nN])//如果正反一样
						{
							int X=gstuRcgInfo.g_factor[0][0]*(double)gclsImgRcg.g_stu_square.pnZPX[nN]+gstuRcgInfo.g_factor[0][1]*(double)gclsImgRcg.g_stu_square.pnZPY[nN]+gstuRcgInfo.g_factor[0][2];
							int Y=gstuRcgInfo.g_factor[1][0]*(double)gclsImgRcg.g_stu_square.pnZPX[nN]+gstuRcgInfo.g_factor[1][1]*(double)gclsImgRcg.g_stu_square.pnZPY[nN]+gstuRcgInfo.g_factor[1][2];
							if(X>20 && Y>20)
							{
								g_dlgDevice->XYSend(X,Y);
								CString csTmp;
								csTmp.Format(_T("X:%d	Y:%d"),X,Y);
								g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(csTmp);
								gstuRun.chStmCmd &= ~(1<<1);
								nCanShk = 1;
								TimeRecord(6,1);
								TimeRecord(5,0);
							}
						}
					}
					else//如果不需要正反面或者圆形
					{
						int X=gstuRcgInfo.g_factor[0][0]*(double)gclsImgRcg.g_stu_square.pnZPX[nN]+gstuRcgInfo.g_factor[0][1]*(double)gclsImgRcg.g_stu_square.pnZPY[nN]+gstuRcgInfo.g_factor[0][2];
						int Y=gstuRcgInfo.g_factor[1][0]*(double)gclsImgRcg.g_stu_square.pnZPX[nN]+gstuRcgInfo.g_factor[1][1]*(double)gclsImgRcg.g_stu_square.pnZPY[nN]+gstuRcgInfo.g_factor[1][2];
						if(X>20 && Y>20)
						{
							g_dlgDevice->XYSend(X,Y);
							CString csTmp;
							csTmp.Format(_T("X:%d	Y:%d"),X,Y);
							g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(csTmp);
							gstuRun.chStmCmd &= ~(1<<1);
							nCanShk = 1;
							TimeRecord(6,1);
							TimeRecord(5,0);
						}
					}
					nN++;
				}
#else
				if (!gstuRun.bTrdPause)//如果无片
				{
					if (gstuRcgInfo.bChkPN && !gstuRcgInfo.bIsCir)
					{
						if (gstuRcgInfo.nPN==gclsImgRcg.g_stu_square.bPN[nN])
						{
							Sleep(1000);
							int X=gstuRcgInfo.g_factor[0][0]*(double)gclsImgRcg.g_stu_square.pnZPX[nN]+gstuRcgInfo.g_factor[0][1]*(double)gclsImgRcg.g_stu_square.pnZPY[nN]+gstuRcgInfo.g_factor[0][2];
							int Y=gstuRcgInfo.g_factor[1][0]*(double)gclsImgRcg.g_stu_square.pnZPX[nN]+gstuRcgInfo.g_factor[1][1]*(double)gclsImgRcg.g_stu_square.pnZPY[nN]+gstuRcgInfo.g_factor[1][2];
							g_dlgDevice->XYSend(X,Y);
							CString csTmp;
							csTmp.Format(_T("X:%d	Y:%d"),X,Y);
							g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(csTmp);
							
						}
					}
					else
					{
						Sleep(1000);
						int X=gstuRcgInfo.g_factor[0][0]*(double)gclsImgRcg.g_stu_square.pnZPX[nN]+gstuRcgInfo.g_factor[0][1]*(double)gclsImgRcg.g_stu_square.pnZPY[nN]+gstuRcgInfo.g_factor[0][2];
						int Y=gstuRcgInfo.g_factor[1][0]*(double)gclsImgRcg.g_stu_square.pnZPX[nN]+gstuRcgInfo.g_factor[1][1]*(double)gclsImgRcg.g_stu_square.pnZPY[nN]+gstuRcgInfo.g_factor[1][2];
						g_dlgDevice->XYSend(X,Y);
						CString csTmp;
						csTmp.Format(_T("X:%d	Y:%d"),X,Y);
						g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(csTmp);
						
					}
					nN++;
				}
#endif
				Sleep(1);
			}
			
		}

#ifndef DEBUG_COM	
		do{
			if (gstuRun.bTrdExit)
			{
				gstuRun.unTrdExitNum++;//线程退出数++
				return 0;
			}
			Sleep(1);
		}while(!(gstuRun.chStmCmd & (1<<1)) || gstuRun.bTrdPause);
#endif
		if(nN>0)
			bMov = 0;
		QueryPerformanceCounter(&l_lgint_end);
		if(gclsImgRcg.g_stu_square.nN > 10)
		{}
		else 
		{if (!gstuRun.closeShake && gclsImgRcg.g_stu_square.bIsLap && bShk<6 && nCanShk == 1)//nN>0)//如果片重叠
		{
			g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(_T("抖动"));
			g_dlgDevice->AxisMove(0x05,0,1);//抖动
			bShk++;
		}
		else //if(0 == nN)
		{
			g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(_T("转动"));
			g_dlgDevice->AxisMove(0x05,100,0);//转动100
			bShk = 0;
			bMov ++;
		}
#ifndef DEBUG_COM
		ln_count = 0;
		while(! (gstuRun.chStmCmd & (1<<4)))//等待被料盘运行完成
		{
			if (gstuRun.bTrdExit)
			{
				gstuRun.unTrdExitNum++;//线程退出数++
				return 0;
			}
			if(++ln_count>3000)
				break;//转动100
			Sleep(1);
		}
#else
		Sleep(2000);
#endif
		
		if(0 == bShk)
			Sleep(600);
		else
			Sleep(500);
		}
		g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(_T("盘停止"));
		gstuRun.chStmCmd &= ~(1<<4);
		double d_run_t=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/l_lgint_freq.QuadPart;
		if(bMov>20)
		{
			g_dlgDevice->AlarmCtrl(1);
			AfxMessageBox(_T("相机检测不出片"));
			g_dlgDevice->AlarmCtrl(0);
			bMov = 0;
		}

	}
	
	return 0;
}

UINT CameraThread(LPVOID pParam)
{	
/*#ifndef DEBUG_MODE
	if (glCamID>=0)
	{
		NET_SNAPCFG struSnapCfg;
		gstuRun.bOverCam=0;

		NET_ManualSnap(glCamID, &struSnapCfg, &gstuCamResult);
		while(!gstuRun.bOverCam)//等待采集图像结束
			Sleep(1);
#endif
		CString m_bmp_file=gstuPathInf.csPathExe + _T("\\PIC\\原图0.bmp");

		CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
		BOOL ifFind;
		do{
			ifFind = findini.FindFile(m_bmp_file); 
			Sleep(1);
		}while( !ifFind );

		//gstuRcgInfo.nPToL=2;//直线点距
		//gstuRcgInfo.nDefectPToL=10;//缺陷点距
		//gstuRcgInfo.nThreshold=30;//阀值
		//char * chPath = (LPSTR)(LPCTSTR)m_bmp_file;
		char chPath[MAX_PATH];
		//WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,readPath,CStringA(m_bmp_file).GetLength(),NULL,NULL); 
		WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,chPath,sizeof(chPath),NULL,NULL); 
		gclsImgRcg.RCGBMPSPLIT(NULL,NULL,chPath,gstuRcgInfo.nPToL,gstuRcgInfo.nDefectPToL,gstuRcgInfo.nThreshold,0,gstuRcgInfo.bIsCir,gstuRcgInfo.bThrdAuto,gstuRcgInfo.bDelNoise);

		if(0==gclsImgRcg.g_stu_square.nN && gclsImgRcg.g_stu_square.bIsLap)
		gclsImgRcg.RCGBMP(NULL,NULL,chPath,gstuRcgInfo.nPToL,gstuRcgInfo.nDefectPToL,gstuRcgInfo.nThreshold,0,gstuRcgInfo.bIsCir,gstuRcgInfo.bThrdAuto,gstuRcgInfo.bDelNoise);

#ifndef DEBUG_MODE
	}
	else
		g_dlgRun->SetDlgItemText(IDC_STAT_RUN,_T("相机未连接"));
#endif*/
	//int takeCnt = 0;

	//TimeRecord(2,1);//启动拍照识别，计时开始
	//while(!gclsIiic.TakeBmpFromRtsp())
	//{
	//	Sleep(500);
	//	
	//	if (takeCnt++>10)
	//	{
	//		g_dlgDevice->AlarmCtrl(1);
	//		AfxMessageBox(_T("取不到相机图像"));
	//		g_dlgDevice->AlarmCtrl(0);
	//		takeCnt =0;
	//		gstuRun.bOverRcg=1;
	//		return 0;
	//	}
	//}
	//TimeRecord(2,0);//拍照结束，计时结束
	//TimeRecord(3,1);//识别开始，计时开始
	///*char l_AnsiStr[MAX_PATH];  
	//WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,gstuPathInf.csPathExe+_T("\\PIC\\原图0.bmp"),-1,l_AnsiStr,sizeof(l_AnsiStr),NULL,NULL); 

	//gclsIiic.Save(l_AnsiStr);*/
	//gclsImgRcg.RCGBMPDATASPLIT(NULL,NULL,pFrameRGB->data[0],gstuRcgInfo.nPToL,gstuRcgInfo.nDefectPToL,gstuRcgInfo.nThreshold,0,gstuRcgInfo.bIsCir,gstuRcgInfo.bThrdAuto,gstuRcgInfo.bDelNoise);
	//TimeRecord(3,0);//识别结束，计时结束
	//gstuRun.bOverRcg=1;

	/////////////////////////////////ksj
	char l_AnsiStr[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, gstuPathInf.csPathExe + _T("\\PIC\\原图0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);

	int nWidth, nHeight;
	int nBitCount;
	int nBitsPerSample;
	int takeCnt = 0;

	TimeRecord(2, 1);//启动拍照识别，计时开始
	while (nullptr == gclsCamera.SnapEx(gnChannel, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, NULL))
	{
		Sleep(500);

		if (takeCnt++>10)
		{
			g_dlgDevice->AlarmCtrl(1);
			AfxMessageBox(_T("取不到相机图像"));
			g_dlgDevice->AlarmCtrl(0);
			takeCnt = 0;
			gstuRun.bOverRcg = 1;
			return 0;
		}
	}
	
	TimeRecord(2, 0);//拍照结束，计时结束
	TimeRecord(3, 1);//识别开始，计时开始
	//
	gclsImgRcg.RCGBMPSPLIT(NULL, NULL, l_AnsiStr, gstuRcgInfo.nPToL, gstuRcgInfo.nDefectPToL, gstuRcgInfo.nThreshold, 0, gstuRcgInfo.bIsCir, gstuRcgInfo.bThrdAuto, gstuRcgInfo.bDelNoise);
	//
	TimeRecord(3, 0);//识别结束，计时结束
	gstuRun.bOverRcg = 1;
	return 0;
}

//
//UINT CameraThreadTest(LPVOID pParam)
//{	
//	CString tmp,csTmpPath;
//	SetThreadAffinityMask(GetCurrentThread(), gstuTrdStat.unProcNum);
//	UINT *num = (UINT*)pParam;
//	LARGE_INTEGER l_lgint_start, l_lgint_end;            
//	LARGE_INTEGER l_lgint_freq;
//	QueryPerformanceFrequency(&l_lgint_freq);  
//
//	char l_char_tmp[256];			
//	gclsTxt.GetHMS(gstuRun.chTmHMS);
//
//	sprintf(l_char_tmp,"\\Data\\test_%s.txt",gstuRun. chTmHMS);
//	csTmpPath = gstuPathInf.csPathExe+(CString)l_char_tmp;
//	CFile file(csTmpPath,CFile::modeCreate | CFile::modeWrite|CFile::modeNoTruncate| CFile::shareDenyRead);
//	file.Close();
//	gclsTxt.TXTAddStr(csTmpPath,_T("单位：秒\r\n"));
//
//	if (glCamID>=0)
//	{
//
//		NET_SNAPCFG struSnapCfg;
//		
//		for ( int i=0; i<20 ; i++)
//		{
//			QueryPerformanceCounter(&l_lgint_start);
//			gstuRun.bOverCam=0;
//			NET_ManualSnap(glCamID, &struSnapCfg, &gstuCamResult);
//			while(!gstuRun.bOverCam)//等待采集图像结束
//				Sleep(1);
//			QueryPerformanceCounter(&l_lgint_end);
//			gstuRun.dRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/l_lgint_freq.QuadPart;	
//			tmp.Format(_T("%.6f\r\n"),gstuRun.dRT);
//			gclsTxt.TXTAddStr(csTmpPath,tmp);
//		}
//		
//	}
//	else
//		g_dlgRun->SetDlgItemText(IDC_STAT_RUN,_T("相机未连接"));
//	gstuRun.bOverRcg=1;
//	return 0;
//}


UINT ScreenThread(LPVOID pParam)
{
  CCSVFile *csv = (CCSVFile*)pParam;
	if(gstuTrdStat.unProcNum>2)
		SetThreadAffinityMask(GetCurrentThread(),2);
	else
		SetThreadAffinityMask(GetCurrentThread(), 1);
	LARGE_INTEGER l_lgint_start, l_lgint_end;
	LARGE_INTEGER l_lgint_freq;
	QueryPerformanceFrequency(&l_lgint_freq);  
	QueryPerformanceCounter(&l_lgint_start);
	while(1)
	{
		Sleep(1);
#ifndef DEBUG_COM
		do{
			if (gstuRun.bTrdExit && 2==gstuRun.unTrdExitNum)//当退出时等待前面的线程都退出
			{
				gstuRun.bTrdExit=0;
				return 0;
			}
			if(!gstuRun.bTrdPause)
			{
				QueryPerformanceCounter(&l_lgint_end);
				double tmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
				if(tmpRT>10)
				{
					//
					gclsCom.CloseCom();
					CString l_str;
					int l_int=g_dlgDevice->GetDlgItemInt(IDC_EDT_COM,0,0);
					l_str.Format(_T("%d"),l_int);
					gclsCom.OpenCom(l_int);
					if (!gclsCom.stuInf.bComOpen)
					{
						AfxMessageBox(_T("打开COM失败"));
					}
					//
					//g_dlgDevice->AlarmCtrl(1);
					MyMsgBoxReadIo("超时");//AfxMessageBox(_T("超时"));
					//g_dlgDevice->AlarmCtrl(0);
					QueryPerformanceCounter(&l_lgint_start);
				}

			}
			else
			{
				QueryPerformanceCounter(&l_lgint_start);
			}
			Sleep(1);
		}while(! (gstuRun.chStmCmd & (1<<0)));//||(gstuRun.chStmCmd & (1<<6)) );//|| gstuRun.bTrdPause);//20150327

// 		do{
// 			if (gstuRun.bTrdExit && 2==gstuRun.unTrdExitNum)
// 			{
// 				gstuRun.bTrdExit=0;
// 				return 0;
// 			}
// 			Sleep(1);
// 		}while(gstuRun.bTrdPause);
#else
		Sleep(3000);
		do{
			if (gstuRun.bTrdExit && 2==gstuRun.unTrdExitNum)
			{
				gstuRun.bTrdExit=0;
				return 0;
			}
			Sleep(1);
		}while(gstuRun.bTrdPause);
		
#endif
		gstuRun.chStmCmd &= ~(1<<0);
#ifndef	DEBUG_SCREEN	
		if (1 == gstuRun.bExit)
		{
			gstuSort.sortsn=29;

			g_dlgDevice->SortSend(char(gstuSort.sortsn));
			CString _degree;
			_degree.Format(_T("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n"),gstuTrdStat._N,g_dlgScreen->degree[0]*100000+g_dlgScreen->degree[1]*10000+g_dlgScreen->degree[2]*1000+g_dlgScreen->degree[3]*100+g_dlgScreen->degree[4]*10+g_dlgScreen->degree[5],
				g_dlgScreen->degree[6]*100000+g_dlgScreen->degree[7]*10000+g_dlgScreen->degree[8]*1000+g_dlgScreen->degree[9]*100+g_dlgScreen->degree[10]*10+g_dlgScreen->degree[11],
				g_dlgScreen->degree[12]*100000+g_dlgScreen->degree[13]*10000+g_dlgScreen->degree[14]*1000+g_dlgScreen->degree[15]*100+g_dlgScreen->degree[16]*10+g_dlgScreen->degree[17],
				g_dlgScreen->degree[18]*100000+g_dlgScreen->degree[19]*10000+g_dlgScreen->degree[20]*1000+g_dlgScreen->degree[21]*100+g_dlgScreen->degree[22]*10+g_dlgScreen->degree[23],
				g_dlgScreen->degree[24]*100000+g_dlgScreen->degree[25]*10000+g_dlgScreen->degree[26]*1000+g_dlgScreen->degree[27]*100+g_dlgScreen->degree[28]*10+g_dlgScreen->degree[29],
				gstuSort.sortsn);

			gclsTxt.TXTAddStr(gstuPathInf.csPathTxt,_degree);

			gstuRun.unSort[2]=gstuRun.unSort[1];
			gstuRun.unSort[1]=gstuRun.unSort[0];
			gstuRun.unSort[0]=gstuSort.sortsn+1;
			QueryPerformanceCounter(&l_lgint_end);
			gstuRun.dRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/l_lgint_freq.QuadPart;	
			l_lgint_start.QuadPart=l_lgint_end.QuadPart;
			gstuTrdStat._N++;
		}
		else if(g_dlgScreen->RCGVGA())
		{
      CalcEquAngle(g_dlgScreen->degree);//DATE 180421
			gstuSort.sortsn=SortChip(&gstuSort,g_dlgScreen->degree);//得到档位值
			if(gstuSort.sortsn<0)
				gstuSort.sortsn=29;

			g_dlgDevice->SortSend(char(gstuSort.sortsn));
			CString _degree;
			_degree.Format(_T("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n"),gstuTrdStat._N,g_dlgScreen->degree[0]*100000+g_dlgScreen->degree[1]*10000+g_dlgScreen->degree[2]*1000+g_dlgScreen->degree[3]*100+g_dlgScreen->degree[4]*10+g_dlgScreen->degree[5],
				g_dlgScreen->degree[6]*100000+g_dlgScreen->degree[7]*10000+g_dlgScreen->degree[8]*1000+g_dlgScreen->degree[9]*100+g_dlgScreen->degree[10]*10+g_dlgScreen->degree[11],
				g_dlgScreen->degree[12]*100000+g_dlgScreen->degree[13]*10000+g_dlgScreen->degree[14]*1000+g_dlgScreen->degree[15]*100+g_dlgScreen->degree[16]*10+g_dlgScreen->degree[17],
				g_dlgScreen->degree[18]*100000+g_dlgScreen->degree[19]*10000+g_dlgScreen->degree[20]*1000+g_dlgScreen->degree[21]*100+g_dlgScreen->degree[22]*10+g_dlgScreen->degree[23],
				g_dlgScreen->degree[24]*100000+g_dlgScreen->degree[25]*10000+g_dlgScreen->degree[26]*1000+g_dlgScreen->degree[27]*100+g_dlgScreen->degree[28]*10+g_dlgScreen->degree[29],
				gstuSort.sortsn);
				
			gclsTxt.TXTAddStr(gstuPathInf.csPathTxt,_degree);

      //流程卡记录：by mmy 171115
      if (csv)
      {
       // CCSVFile csv(gstuPathInf.csPathExe + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);
        CStringArray arr;
        CString str;
        str.Format(_T("%d"), gstuTrdStat._N);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[0] * 100000 + g_dlgScreen->degree[1] * 10000 + g_dlgScreen->degree[2] * 1000 + g_dlgScreen->degree[3] * 100 + g_dlgScreen->degree[4] * 10 + g_dlgScreen->degree[5]);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[6] * 100000 + g_dlgScreen->degree[7] * 10000 + g_dlgScreen->degree[8] * 1000 + g_dlgScreen->degree[9] * 100 + g_dlgScreen->degree[10] * 10 + g_dlgScreen->degree[11]);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[12] * 100000 + g_dlgScreen->degree[13] * 10000 + g_dlgScreen->degree[14] * 1000 + g_dlgScreen->degree[15] * 100 + g_dlgScreen->degree[16] * 10 + g_dlgScreen->degree[17]);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[18] * 100000 + g_dlgScreen->degree[19] * 10000 + g_dlgScreen->degree[20] * 1000 + g_dlgScreen->degree[21] * 100 + g_dlgScreen->degree[22] * 10 + g_dlgScreen->degree[23]);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[24] * 100000 + g_dlgScreen->degree[25] * 10000 + g_dlgScreen->degree[26] * 1000 + g_dlgScreen->degree[27] * 100 + g_dlgScreen->degree[28] * 10 + g_dlgScreen->degree[29]);
        arr.Add(str);
        str.Format(_T("%d"), gstuSort.sortsn);
        arr.Add(str);

        csv->WriteData(arr);

        arr.RemoveAll();
      }

					
			gstuRun.unSort[2]=gstuRun.unSort[1];
			gstuRun.unSort[1]=gstuRun.unSort[0];
			gstuRun.unSort[0]=gstuSort.sortsn+1;
			QueryPerformanceCounter(&l_lgint_end);
			gstuRun.dRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/l_lgint_freq.QuadPart;	
			l_lgint_start.QuadPart=l_lgint_end.QuadPart;
			gstuTrdStat._N++;
		}
		else
			g_dlgRun->SetDlgItemText(IDC_TIME_RUN,_T("屏幕识别失败"));
#else
		srand((int)time(0)); //模拟begin
		int deg[30]={3,4,0,rand()%9,rand()%5,rand()%9,
					2,2+rand()%3,rand()%5,rand()%9,rand()%5,rand()%9,
					rand()%5,rand()%9,rand()%5,rand()%9,rand()%5,rand()%9,
					rand()%5,rand()%9,rand()%5,rand()%9,rand()%5,rand()%9,
					rand()%5,rand()%9,rand()%5,rand()%9,rand()%5,rand()%9};
		for(int i=0;i<30;i++)
			g_dlgScreen->degree[i]=deg[i];//模拟end
		gstuSort.sortsn=SortChip(&gstuSort,g_dlgScreen->degree);//得到档位值
		//gstuSort.sortsn ++ ;
		unsigned int unTmpSN = 28;
		/*if(gstuSort.sortsn<0 ||gstuSort.sortsn >29)
			gstuSort.sortsn=29;
		else if(gstuSort.sortsn<15 &&gstuSort.sortsn >1)
			unTmpSN=1;
		else if(gstuSort.sortsn<18 &&gstuSort.sortsn >14)
			unTmpSN=19;
		else if(gstuSort.sortsn<27 &&gstuSort.sortsn >19)
			unTmpSN=29;
		else
			unTmpSN = gstuSort.sortsn;*/
		g_dlgDevice->SortSend(char(unTmpSN));
		CString _degree;
		_degree.Format(_T("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n"),gstuTrdStat._N,g_dlgScreen->degree[0]*100000+g_dlgScreen->degree[1]*10000+g_dlgScreen->degree[2]*1000+g_dlgScreen->degree[3]*100+g_dlgScreen->degree[4]*10+g_dlgScreen->degree[5],
			g_dlgScreen->degree[6]*100000+g_dlgScreen->degree[7]*10000+g_dlgScreen->degree[8]*1000+g_dlgScreen->degree[9]*100+g_dlgScreen->degree[10]*10+g_dlgScreen->degree[11],
			g_dlgScreen->degree[12]*100000+g_dlgScreen->degree[13]*10000+g_dlgScreen->degree[14]*1000+g_dlgScreen->degree[15]*100+g_dlgScreen->degree[16]*10+g_dlgScreen->degree[17],
			g_dlgScreen->degree[18]*100000+g_dlgScreen->degree[19]*10000+g_dlgScreen->degree[20]*1000+g_dlgScreen->degree[21]*100+g_dlgScreen->degree[22]*10+g_dlgScreen->degree[23],
			g_dlgScreen->degree[24]*100000+g_dlgScreen->degree[25]*10000+g_dlgScreen->degree[26]*1000+g_dlgScreen->degree[27]*100+g_dlgScreen->degree[28]*10+g_dlgScreen->degree[29],
			gstuSort.sortsn);

		gclsTxt.TXTAddStr(gstuPathInf.csPathTxt,_degree);

		gstuRun.unSort[2]=gstuRun.unSort[1];
		gstuRun.unSort[1]=gstuRun.unSort[0];
		gstuRun.unSort[0]=gstuSort.sortsn;
		QueryPerformanceCounter(&l_lgint_end);
		gstuRun.dRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
		l_lgint_start.QuadPart=l_lgint_end.QuadPart;
		gstuTrdStat._N++;
#endif

	}
	
	
	return 0;
}

UINT CalibrationThread(LPVOID pParam)
{
	double x;
	double y;
	for (gstuRcgInfo.Nxy=0;gstuRcgInfo.Nxy<3;gstuRcgInfo.Nxy++)
	{
		x = gstuRcgInfo.nClbPosX[gstuRcgInfo.Nxy];
		y = gstuRcgInfo.nClbPosY[gstuRcgInfo.Nxy];
		g_dlgDevice->AxisMove(0x06,0,1);//中转盘归0
		Sleep(1000);
		g_dlgDevice->AxisMove(0x02,0,1);//xy归零
		Sleep(1000);
		if(gstuRcgInfo.Nxy==0)
			g_dlgDevice->XYMove(g_dlgDevice->GetDlgItemInt(IDC_EDT_XY1X,0,1),g_dlgDevice->GetDlgItemInt(IDC_EDT_XY1Y,0,1));
		else
			g_dlgDevice->XYMove(gstuRcgInfo.nClbPosX[gstuRcgInfo.Nxy-1],gstuRcgInfo.nClbPosY[gstuRcgInfo.Nxy-1]);
		Sleep(2000);
		g_dlgDevice->MHCtrl(0x01,1);//下
		Sleep(1000);
		g_dlgDevice->SNCtrl(0x01,1);//吸
		Sleep(1000);
		g_dlgDevice->MHCtrl(0x01,0);//上
		Sleep(1000);
		//Sleep(1000);
		g_dlgDevice->AxisMove(0x02,0,1);//xy归零
		Sleep(2000);
		g_dlgDevice->XYMove(x,y);//移动到校准点
		Sleep(1000);
		g_dlgDevice->MHCtrl(0x01,1);
		Sleep(1000);
		g_dlgDevice->SNCtrl(0x01,0);
		
		AfxMessageBox(_T("运行完成按确定"));
		g_dlgDevice->MHCtrl(0x01,0);
		Sleep(1000);
		
		g_dlgDevice->AxisMove(0x02,0,1);
		/////////////////////////////////////////////////////////////////
		gstuRcgInfo.bClbPos=0;
		g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("1、图像采集；2、识别测试"));
		while (!gstuRcgInfo.bClbPos)
			Sleep(1);
		g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("1、在屏幕上正确选择校准片"));
		do 
		{
			gstuRcgInfo.bClbPos = 0;
			while (!gstuRcgInfo.bClbPos)
			{
				Sleep(1);
			}
			int status=AfxMessageBox(_T("是否正确选择？"),MB_OKCANCEL);
			if(status==1)//按确定退出
				break;
			else
			{
				gstuRcgInfo.bClbPos=0;
				g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("1、图像采集；2、识别测试"));
				while (!gstuRcgInfo.bClbPos)
					Sleep(1);
				g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("1、在屏幕上正确选择校准片"));
			}
		}while(1);
		//////////////////////////////////////////////////////////////////
		gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][2]=1;
		gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][2]=1;
		gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][3]=x;
		gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][3]=y;
	}


	calparameter(gstuRcgInfo.Xxy,gstuRcgInfo.g_factor[0]);
	calparameter(gstuRcgInfo.Yxy,gstuRcgInfo.g_factor[1]);
	//保存
	CString strValue;
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[0][0]);
	WritePrivateProfileString(_T("校准系数"),_T("00"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[0][1]);
	WritePrivateProfileString(_T("校准系数"),_T("01"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[0][2]);
	WritePrivateProfileString(_T("校准系数"),_T("02"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[1][0]);
	WritePrivateProfileString(_T("校准系数"),_T("10"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[1][1]);
	WritePrivateProfileString(_T("校准系数"),_T("11"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[1][2]);
	WritePrivateProfileString(_T("校准系数"),_T("12"),strValue,gstuPathInf.csPathIni);
	g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("校准完成"));
	return 1;
}

UINT ComMsg(LPVOID pParam)
{
	gstuRun.bPosSet==0;
	while(gclsCom.m_struct_com.s_hd!=INVALID_HANDLE_VALUE)
	{//在ontimer中会与按钮冲突
			if(!gclsCom.RecvCharFromCom())
			{
				gstuRefresh.bComUpdate=1;
				//if(gstuRun.unStatRun != 0)//如果不是停止态，解析
				{
					UINT unCmdTmp = gclsCom.RS232Parse();
					if (2 == unCmdTmp)
					{
						TimeRecord(1,1);//无片坐标到，开始计时
						TimeRecord(5,1);
						TimeRecord(6,0);//发出坐标到接收到下一个无片命令
					}
					if(unCmdTmp!=0)
						gstuRun.chStmCmd = gstuRun.chStmCmd | (1<<(unCmdTmp-1));
				}
				
				/*switch(gclsCom.RS232Parse())
				{
				case 1://EFG检测完成
					chStmCmd[1] = chStmCmd[1] | (1<<0);
					break;
				case 2://无片坐标
					unCmd=2;
					break;
				case 3://初始化完成
					unCmd=3;
					break;
				case 4://作业停止
					unCmd=4;
					break;
				case 5://备料盘运行完成
					unCmd=5;
					break;
				case 6://位置设定完毕
					unCmd=6;
					break;
				default:
					break;
				case 2:
						//g_struct_info_run.s_bool_stminit=1;			//标记下位机反馈初始化命令，告诉run线程做相应处理

						break;
					case 0:
						//g_struct_info_com.s_bool_command_success=1;
						//g_struct_info_run.ui_count_resend=0;//放在resendcom中做，会因为没时间判断到success=1而导致不执行
						break;
					default:
						break;
				}*/
			}
	#ifndef DEBUG_COM
		if((gstuRun.chStmCmd & (1<<3)))
		{
			gstuRun.chStmCmd &= ~(1<<3);
			//g_dlgDevice->SetDlgItemTextW(IDC_MSG,_T("XY轴位置设置完成"));
			gstuRun.bPosSet=1;
		}

		if((gstuRun.chStmCmd & (1<<5)))
		{
			gstuRun.chStmCmd &= ~(1<<5);
			//g_dlgDevice->SetDlgItemTextW(IDC_MSG,_T("换片轴位置设置完成"));
			gstuRun.bPosSet=2;
		}

		if((gstuRun.chStmCmd & (1<<6)) &&!gstuRun.bTrdPause)
		{
			
			if(gstuRun.unSort[0]>0)
			g_dlgDevice->SortSend(char(gstuRun.unSort[0]-1));
			//gstuRun.chStmCmd = gstuRun.chStmCmd | (1<<(1));
			gstuRun.chStmCmd &= ~(1<<6);
		}
	#endif
		Sleep(10);
	}
	return 1;
}

//坐标系转换系数计算
void calparameter(double (*xy)[4],double *factor)
	//void calparameter(double **xy,double *factor)
{
	int k,i,j,f;//循环参数
	for(k=0;k<3;k++)//列主元高斯消元               
	{
		//double ark=findmax(k,xy);
		double max=0;
		for(int i=k;i<3;i++)
			if(fabs(xy[i][k])>max)        //对数组中的元素进行取绝对值比较大小的运算
			{
				max=fabs(xy[i][k]);
				f=i;
			}

			if(max==0)
			{
				AfxMessageBox(_T("此方程组无解!"));
				return;
			}
			else if(f!=k)
			{
				//change(f,k,xy);
				double temp[4];
				for(int i=0;i<4;i++)
					temp[i]=xy[f][i];
				for(int i=0;i<4;i++)
					xy[f][i]=xy[k][i];
				for(int i=0;i<4;i++)
					xy[k][i]=temp[i];
			}
			for(int i=k+1;i<3;i++)
				for(int j=3;j>=0;j--)
					xy[i][j]=xy[i][j]-xy[k][j]*xy[i][k]/xy[k][k];         
	}
	factor[2]=xy[2][3]/xy[2][2];      //先求出最后一个未知数的解
	for( k=1;k>=0;k--)//回调
	{
		double p1=0;
		for(j=k+1;j<3;j++)
		{
			p1=p1+xy[k][j]*factor[j];
		}
		factor[k]=(xy[k][3]-p1)/xy[k][k];       
	}
}

//ret 0 success
//ret !0 failure
int CalcEquAngle(int *degree)
{
  for (int i = 0; i < 30; i++) {
    if (degree[i] > 60 || degree[i] < 0)
      return -1;
  }

  int angle[5] = {
    (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]),//光轴
    (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]),//电轴
    (degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]),//等效角
    (degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]),//原始光轴
    (degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29])
  };//原始电轴；保存测定出的角度，转换成秒

  int t0 = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5])/* + g_sort.R1t[g_sort.R1Num]*/;//光轴//_0614
  int e0 = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11])/* + g_sort.R1e[g_sort.R1Num]*/;//电轴
  int equal_angle;

  // 等效角 = 原始光轴 + （原始电轴 - 等效角电轴参数）/等效角因子
  if (gstuSort.factor) {
    equal_angle = t0 + double(e0 - gstuSort.phi0) * 1000 / gstuSort.factor + 0.5;
  }
  else {
    equal_angle = 0;
  }

  degree[12] = equal_angle / 3600 / 10;
  degree[13] = equal_angle / 3600 % 10;
  degree[14] = equal_angle % 3600 / 60 / 10;
  degree[15] = equal_angle % 3600 / 60 % 10;
  degree[16] = equal_angle % 60 / 10;
  degree[17] = equal_angle % 60 % 10;

  return 0;
}
//分档计算程序，使用到全局变量
//返回档位值，供机械手分档 0-29
int SortChip(sortchip* s_sort,int *degree)
{
	bool elecyes=1;//限定轴通过标志
	int sortSN=-1;//返回值，分档值
	
	int angle[5]={(degree[0]*10+degree[1])*3600+(degree[2]*10+degree[3])*60+(degree[4]*10+degree[5]),//光轴
		(degree[6]*10+degree[7])*3600+(degree[8]*10+degree[9])*60+(degree[10]*10+degree[11]),//电轴
		(degree[12]*10+degree[13])*3600+(degree[14]*10+degree[15])*60+(degree[16]*10+degree[17]),//等效角
		(degree[18]*10+degree[19])*3600+(degree[20]*10+degree[21])*60+(degree[22]*10+degree[23]),//原始光轴
		(degree[24]*10+degree[25])*3600+(degree[26]*10+degree[27])*60+(degree[28]*10+degree[29])};//原始电轴；保存测定出的角度，转换成秒
	if(degree[0]==-1 || angle[3]==0)//角度没测出
	{
		sortSN=29;
		s_sort->sortnum[29]++;
		s_sort->nocheckout++;
		return sortSN;
	}
	if(s_sort->nocheckout > 0)
		s_sort->nocheckout--;
	if (0==angle[0] && angle[3]!=0)//激光检测不出
	{
		sortSN=28;
		s_sort->sortnum[28]++;
		s_sort->checkout0++;
		return sortSN;
	}
	if (s_sort->checkout0 > 0)
		s_sort->checkout0--;
	if(s_sort->sortsel2!=3)//如果电轴限定
	{
		switch((angle[s_sort->sortsel2]<=s_sort->elechigh?-1:0)+(angle[s_sort->sortsel2]>=s_sort->eleclow?1:0))//计算限定轴并分档
		{
		case 0:break;
		case 1:elecyes=0;s_sort->sortnum[25]++;sortSN=25;break;
		case -1:elecyes=0;s_sort->sortnum[24]++;sortSN=24;break;
		}
	}
	if(1==elecyes)//如果限定轴通过
	{
		if(angle[s_sort->sortsel1]>=(s_sort->centerangle-s_sort->sortvalue/2))//如果检测轴大于预设值
		{
			switch((angle[s_sort->sortsel1]-s_sort->centerangle+s_sort->sortvalue/2)/s_sort->sortvalue+1)//计算检测轴并分档（检测轴大于预设值
			{
			case 1:s_sort->sortnum[12]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=12;break;
			case 2:s_sort->sortnum[13]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=13;break;
			case 3:s_sort->sortnum[14]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=14;break;
			case 4:s_sort->sortnum[15]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=15;break;
			case 5:s_sort->sortnum[16]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=16;break;
			case 6:s_sort->sortnum[17]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=17;break;
			case 7:s_sort->sortnum[18]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=18;break;
			case 8:s_sort->sortnum[19]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=19;break;
			case 9:s_sort->sortnum[20]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=20;break;
			case 10:s_sort->sortnum[21]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=21;break;
			case 11:s_sort->sortnum[22]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=22;break;
			case 12:s_sort->sortnum[23]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=23;break;
			default:s_sort->sortnum[27]++;sortSN=27;break;
			}
		}
		else
		{
			switch((s_sort->centerangle-s_sort->sortvalue/2-angle[s_sort->sortsel1]-1)/s_sort->sortvalue+1)//计算检测轴并分档（检测轴小于预设值
			{
			case 1:s_sort->sortnum[11]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=11;break;
			case 2:s_sort->sortnum[10]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=10;break;
			case 3:s_sort->sortnum[9]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=9;break;
			case 4:s_sort->sortnum[8]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=8;break;
			case 5:s_sort->sortnum[7]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=7;break;
			case 6:s_sort->sortnum[6]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=6;break;
			case 7:s_sort->sortnum[5]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=5;break;
			case 8:s_sort->sortnum[4]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=4;break;
			case 9:s_sort->sortnum[3]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=3;break;
			case 10:s_sort->sortnum[2]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=2;break;
			case 11:s_sort->sortnum[1]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=1;break;
			case 12:s_sort->sortnum[0]++;s_sort->sortsum++;SortAvgStd(s_sort->sortsum,angle,s_sort->sortavg,s_sort->sortstd,s_sort->std2);sortSN=0;break;
			default:s_sort->sortnum[26]++;sortSN=26;break;
			}
		}
	}

	return sortSN;
}
void CalcAvgStd(const int sum, const double angle, double &avg, double &std, double &std2)
{
  double n = sum;

  double xi = angle;
  //double avg;
  if (1 == n)
  {
    avg = xi;//平均值
    std2 = xi * xi;//标准偏差中间值
  }
  if (n > 1)
  {
    avg = (avg + xi / (n - 1)) / (n / (n - 1));//求平均值
    std2 = (std2 + xi * xi / (n - 1)) / (n / (n - 1));//求标准差中间值
    std = sqrt(abs(std2 - avg * avg)*n / (n - 1));//求标准差
  }
}
//计算光轴平均值及标准方程
void SortAvgStd(unsigned int sum,int *angle,double *avg,double *std,long double *std2)
{
	double n=sum;
	for(int i=0;i<5;i++)
	{
		double xi=angle[i];
		//double avg;
		if(1==n)
		{
			avg[i]=xi;//平均值
			std2[i]=xi*xi;//标准偏差中间值
		}
		if(n>1)
		{
			avg[i]=(avg[i]+xi/(n-1))/(n/(n-1));//求平均值
			std2[i]=(std2[i]+xi*xi/(n-1))/(n/(n-1));//求标准差中间值
			std[i]=sqrt(abs(std2[i]-avg[i]*avg[i])*n/(n-1));//求标准差
		}
	}
}


double TimeRecord(unsigned int step, bool start)
{
	double ldv_tim = 0;
	if (start)
	{
		gstuRun.timRun[step][0] = GetTickCount();
		
	} 
	else
	{
		gstuRun.timRun[step][1] = GetTickCount();

		if (gstuRun.timRun[step][1]>gstuRun.timRun[step][0])
		{
			ldv_tim = (gstuRun.timRun[step][1]-gstuRun.timRun[step][0])*1.0/1000;		
		} 

		CString csRec;

		switch(step)
		{
		case 0:
			csRec.Format(_T("：%.3f\r\n"),ldv_tim);
			break;
		case 1:
			csRec.Format(_T("无片坐标接收至启动相机前：%.3f\r\n"),ldv_tim);
			break;
		case 2:
			csRec.Format(_T("相机抓拍：%.3f\r\n"),ldv_tim);
			break;
		case 3:
			csRec.Format(_T("识别过程：%.3f\r\n"),ldv_tim);
			break;
		case 4:
			csRec.Format(_T("启动相机线程：%.3f\r\n"),ldv_tim);
			break;
		case 5:
			csRec.Format(_T("无片坐标接收至发出坐标：%.3f\r\n"),ldv_tim);
			break;
		case 6:
			csRec.Format(_T("发出坐标到接收到下一个无片命令：%.3f\r\n"),ldv_tim);
			break;
		case 7:
			csRec.Format(_T("：%.3f\r\n"),ldv_tim);
			break;
		case 8:
			csRec.Format(_T("：%.3f\r\n"),ldv_tim);
			break;
		case 9:
			csRec.Format(_T("：%.3f\r\n"),ldv_tim);
			break;
		default:
			break;
		}
				
		gclsTxt.TXTAddStr(gstuPathInf.csPathLog,csRec);
	}
	return ldv_tim;
	
}