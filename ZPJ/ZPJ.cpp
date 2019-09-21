
// BER.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ZPJ.h"
#include "ZPJDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//ȫ��
RunInfo gstuRun;
RefreshTmp gstuRefresh;
threadstatus gstuTrdStat;//�߳�ȫ�ֽṹ����
sortchip gstuSort;
CFont g_font,g_font2,g_font3;
CTXT gclsTxt;
CCpkLib *g_pCpk;
INT32 *g_vars = NULL;

//ʶ��
//CIMGRecognition gclsImgRcg;
RecognitionInfo  gstuRcgInfo;
NpcInf g_npc_inf;
PathInfo gstuPathInf;
int SortChip(sortchip* s_sort,int *degree);
void SortAvgStd(unsigned int sum,int *angle,double *avg,double *std,long double *std2);
//������
//CPicture gclsPic;//pictrue�����
//SETTINGS_CAMERA gstuCamParam;//�������
//Network_Config_Data gstuCamNetCfg;//���������� 
//LONG glCamID;//���ID����ʼ��δ-1
//VehiclePassRec gstuCamResult;//�������ṹ
//ksj���
int gnChannel = -1;
CKSJ gclsCamera;
//�߳�
CWinThread* gTrdRun;//�����߳�
CWinThread* gTrdCam;//����߳�
CWinThread* gTrdScr;//��Ļ�߳�
CWinThread* gTrdClb;//У׼�߳�
CWinThread* gTrdCom;
CWinThread* gTrdAuto = NULL;

UINT RunThread(LPVOID pParam);//�����̺߳���
UINT CameraThread(LPVOID pParam);//ͼ��ʶ���̺߳���
UINT ScreenThread(LPVOID pParam);//�����̺߳���
UINT CalibrationThread(LPVOID pParam);//У׼�߳�
UINT ComMsg(LPVOID pParam);
void calparameter(double (*xy)[4],double *factor);
// CBERApp

BEGIN_MESSAGE_MAP(CBERApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

libvlc_media_player_t * vlc_player = NULL, *vlc_player_run = NULL;
BOOL gb_PlayOrNot[2]={0};
//vlc������Ƶ�Ļص�����
void CALLBACK RealDataCallBack0(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,void *pUser)
{
	;
}

// CBERApp ����

CBERApp::CBERApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CBERApp ����

CBERApp theApp;


// CBERApp ��ʼ��

BOOL CBERApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("��ʢZPJ"));

	CBERDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
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
	int bShk=0;//�������
	int bMov = 0;//�ƶ��������
	int ln_count=0;//�ط�����
	while(1)
	{

#ifndef DEBUG_COM
		do{
			if (gstuRun.bTrdExit)
			{
				gstuRun.unTrdExitNum++;//�߳��˳���++
				return 0;
			}
			Sleep(1);
		}while(! (gstuRun.chStmCmd & (1<<1)));
		TimeRecord(1,0);//��Ƭ���������ɣ���ʱ����
#else
		Sleep(3000);
#endif

		QueryPerformanceCounter(&l_lgint_start);
		gstuRun.bOverRcg=0;
		gclsImgRcg.g_stu_square.nN=0;
		nN=0;
		nCanShk = 0;

		TimeRecord(4,1);//��������2����ʱ��ʼ

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
			AfxMessageBox(_T("���������߳�ʧ��"));
			return 0;
		}
		gTrdCam->m_bAutoDelete = FALSE;
		gTrdCam->ResumeThread();

		TimeRecord(4,0);//�߳���������ʱ����

		while(!gstuRun.bOverRcg)
		{
			if (gstuRun.bTrdExit)
			{
				gstuRun.unTrdExitNum++;//�߳��˳���++
				return 0;
			}
			Sleep(1);

			while(nN<gclsImgRcg.g_stu_square.nN && nN < 6)//��ȡ����
			{
				if (gstuRun.bTrdExit)
				{
					gstuRun.unTrdExitNum++;//�߳��˳���++
					return 0;
				}
#ifndef DEBUG_COM
				if ((gstuRun.chStmCmd & (1<<1)) && !gstuRun.bTrdPause)//�����Ƭ
				{
					if (gstuRcgInfo.bChkPN && !gstuRcgInfo.bIsCir)//����ж����������Ƿ���
					{
						if (gstuRcgInfo.nPN==gclsImgRcg.g_stu_square.bPN[nN])//�������һ��
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
					else//�������Ҫ���������Բ��
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
				if (!gstuRun.bTrdPause)//�����Ƭ
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
				gstuRun.unTrdExitNum++;//�߳��˳���++
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
		{if (!gstuRun.closeShake && gclsImgRcg.g_stu_square.bIsLap && bShk<6 && nCanShk == 1)//nN>0)//���Ƭ�ص�
		{
			g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(_T("����"));
			g_dlgDevice->AxisMove(0x05,0,1);//����
			bShk++;
		}
		else //if(0 == nN)
		{
			g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(_T("ת��"));
			g_dlgDevice->AxisMove(0x05,100,0);//ת��100
			bShk = 0;
			bMov ++;
		}
#ifndef DEBUG_COM
		ln_count = 0;
		while(! (gstuRun.chStmCmd & (1<<4)))//�ȴ��������������
		{
			if (gstuRun.bTrdExit)
			{
				gstuRun.unTrdExitNum++;//�߳��˳���++
				return 0;
			}
			if(++ln_count>3000)
				break;//ת��100
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
		g_dlgRun->GetDlgItem(IDC_STAT_RUN)->SetWindowText(_T("��ֹͣ"));
		gstuRun.chStmCmd &= ~(1<<4);
		double d_run_t=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/l_lgint_freq.QuadPart;
		if(bMov>20)
		{
			g_dlgDevice->AlarmCtrl(1);
			AfxMessageBox(_T("�����ⲻ��Ƭ"));
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
	while(!gstuRun.bOverCam)//�ȴ��ɼ�ͼ�����
	Sleep(1);
	#endif
	CString m_bmp_file=gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp");

	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind;
	do{
	ifFind = findini.FindFile(m_bmp_file); 
	Sleep(1);
	}while( !ifFind );

	//gstuRcgInfo.nPToL=2;//ֱ�ߵ��
	//gstuRcgInfo.nDefectPToL=10;//ȱ�ݵ��
	//gstuRcgInfo.nThreshold=30;//��ֵ
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
	g_dlgRun->SetDlgItemText(IDC_STAT_RUN,_T("���δ����"));
	#endif*/
	//int takeCnt = 0;

	//TimeRecord(2,1);//��������ʶ�𣬼�ʱ��ʼ
	//while(!gclsIiic.TakeBmpFromRtsp())
	//{
	//	Sleep(500);
	//	
	//	if (takeCnt++>10)
	//	{
	//		g_dlgDevice->AlarmCtrl(1);
	//		AfxMessageBox(_T("ȡ�������ͼ��"));
	//		g_dlgDevice->AlarmCtrl(0);
	//		takeCnt =0;
	//		gstuRun.bOverRcg=1;
	//		return 0;
	//	}
	//}
	//TimeRecord(2,0);//���ս�������ʱ����
	//TimeRecord(3,1);//ʶ��ʼ����ʱ��ʼ
	///*char l_AnsiStr[MAX_PATH];  
	//WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,gstuPathInf.csPathExe+_T("\\PIC\\ԭͼ0.bmp"),-1,l_AnsiStr,sizeof(l_AnsiStr),NULL,NULL); 

	//gclsIiic.Save(l_AnsiStr);*/
	//gclsImgRcg.RCGBMPDATASPLIT(NULL,NULL,pFrameRGB->data[0],gstuRcgInfo.nPToL,gstuRcgInfo.nDefectPToL,gstuRcgInfo.nThreshold,0,gstuRcgInfo.bIsCir,gstuRcgInfo.bThrdAuto,gstuRcgInfo.bDelNoise);
	//TimeRecord(3,0);//ʶ���������ʱ����
	//gstuRun.bOverRcg=1;

	/////////////////////////////////ksj
	char l_AnsiStr[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);

	int nWidth, nHeight;
	int nBitCount;
	int nBitsPerSample;
	int takeCnt = 0;

	TimeRecord(2, 1);//��������ʶ�𣬼�ʱ��ʼ
	while (nullptr == gclsCamera.SnapEx(gnChannel, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, NULL))
	{
		Sleep(500);

		if (takeCnt++>10)
		{
			g_dlgDevice->AlarmCtrl(1);
			AfxMessageBox(_T("ȡ�������ͼ��"));
			g_dlgDevice->AlarmCtrl(0);
			takeCnt = 0;
			gstuRun.bOverRcg = 1;
			return 0;
		}
	}

	TimeRecord(2, 0);//���ս�������ʱ����
	TimeRecord(3, 1);//ʶ��ʼ����ʱ��ʼ
	//
	gclsImgRcg.RCGBMPSPLIT(NULL, NULL, l_AnsiStr, gstuRcgInfo.nPToL, gstuRcgInfo.nDefectPToL, gstuRcgInfo.nThreshold, 0, gstuRcgInfo.bIsCir, gstuRcgInfo.bThrdAuto, gstuRcgInfo.bDelNoise);
	//
	TimeRecord(3, 0);//ʶ���������ʱ����
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
//	gclsTxt.TXTAddStr(csTmpPath,_T("��λ����\r\n"));
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
//			while(!gstuRun.bOverCam)//�ȴ��ɼ�ͼ�����
//				Sleep(1);
//			QueryPerformanceCounter(&l_lgint_end);
//			gstuRun.dRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/l_lgint_freq.QuadPart;	
//			tmp.Format(_T("%.6f\r\n"),gstuRun.dRT);
//			gclsTxt.TXTAddStr(csTmpPath,tmp);
//		}
//		
//	}
//	else
//		g_dlgRun->SetDlgItemText(IDC_STAT_RUN,_T("���δ����"));
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
			if (gstuRun.bTrdExit && 2==gstuRun.unTrdExitNum)//���˳�ʱ�ȴ�ǰ����̶߳��˳�
			{
				// CPK
				if(g_pCpk)
				{
					CProcessCard processCard;

					g_pCpk->CalcCpk(processCard);

					g_pCpk->AddCpkToPlannedCsv(processCard);
				}

				gstuRun.bTrdExit=0;

				return 0;
			}
			if(!gstuRun.bTrdPause)
			{
				QueryPerformanceCounter(&l_lgint_end);
				double tmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
				if(tmpRT>20)//190522 ����4�γ���10s���ĳ�20s
				{
					//
					gclsCom.CloseCom();
					CString l_str;
					int l_int=g_dlgDevice->GetDlgItemInt(IDC_EDT_COM,0,0);
					l_str.Format(_T("%d"),l_int);
					gclsCom.OpenCom(l_int);
					if (!gclsCom.stuInf.bComOpen)
					{
						AfxMessageBox(_T("��COMʧ��"));
					}
					//
					//g_dlgDevice->AlarmCtrl(1);
					MyMsgBoxReadIo("��ʱ");//AfxMessageBox(_T("��ʱ"));
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
				// CPK
				if(g_pCpk)
				{
					CProcessCard processCard;

					g_pCpk->CalcCpk(processCard);

					g_pCpk->AddCpkToPlannedCsv(processCard);
				}
				//
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
			gstuSort.sortsn=SortChip(&gstuSort,g_dlgScreen->degree);//�õ���λֵ
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

			//���̿���¼��by mmy 171115
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

			if(g_pCpk)
			{
				int *degree = g_dlgScreen->degree;
				CProcessData processData;
				processData.equal = (degree[12]*10+degree[13])*3600+(degree[14]*10+degree[15])*60+(degree[16]*10+degree[17]);
				processData.laser = (degree[0]*10+degree[1])*3600+(degree[2]*10+degree[3])*60+(degree[4]*10+degree[5]);
				processData.laser0 = (degree[18]*10+degree[19])*3600+(degree[20]*10+degree[21])*60+(degree[22]*10+degree[23]);
				processData.no = gstuTrdStat._N;
				processData.phi = (degree[6]*10+degree[7])*3600+(degree[8]*10+degree[9])*60+(degree[10]*10+degree[11]);
				processData.phi0 = (degree[24]*10+degree[25])*3600+(degree[26]*10+degree[27])*60+(degree[28]*10+degree[29]);
				processData.sort = gstuSort.sortsn;

				g_pCpk->AddToProcessCardCsv(processData);

				// cpkʵʱ���
				CProcessCard processCard;

				if(processData.no>g_pCpk->m_cpkSet.m_start_num)
				{
					g_pCpk->CalcCpk(processCard);

					CString csCpk=_T(""),tmp;
					tmp.Format( _T("cp %.3f "), processCard.cp);
					csCpk+=tmp;
					tmp.Format( _T("ca %.3f "), processCard.ca);
					csCpk+=tmp;
					tmp.Format( _T("cpk %.3f "), processCard.cpk);
					csCpk+=tmp;
					tmp.Format( _T("num %d "), processCard.num);
					csCpk+=tmp;
					if(processCard.cp < g_pCpk->m_cpkSet.m_min_cp||processCard.ca > g_pCpk->m_cpkSet.m_max_ca||processCard.cpk < g_pCpk->m_cpkSet.m_min_cpk)
					{
						csCpk += _T("����");
						g_dlgRun->OnBnClickedBtnPause();
					}
					g_dlgRun->SetDlgItemText(IDC_STATIC_CPK,csCpk);
				}
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
			g_dlgRun->SetDlgItemText(IDC_TIME_RUN,_T("��Ļʶ��ʧ��"));
#else
		srand((int)time(0)); //ģ��begin
		int deg[30]={3,4,0,rand()%9,rand()%5,rand()%9,
			2,2+rand()%3,rand()%5,rand()%9,rand()%5,rand()%9,
			rand()%5,rand()%9,rand()%5,rand()%9,rand()%5,rand()%9,
			rand()%5,rand()%9,rand()%5,rand()%9,rand()%5,rand()%9,
			rand()%5,rand()%9,rand()%5,rand()%9,rand()%5,rand()%9};
		for(int i=0;i<30;i++)
			g_dlgScreen->degree[i]=deg[i];//ģ��end
		gstuSort.sortsn=SortChip(&gstuSort,g_dlgScreen->degree);//�õ���λֵ
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
		g_dlgDevice->AxisMove(0x06,0,1);//��ת�̹�0
		Sleep(1000);
		g_dlgDevice->AxisMove(0x02,0,1);//xy����
		Sleep(1000);
		if(gstuRcgInfo.Nxy==0)
			g_dlgDevice->XYMove(g_dlgDevice->GetDlgItemInt(IDC_EDT_XY1X,0,1),g_dlgDevice->GetDlgItemInt(IDC_EDT_XY1Y,0,1));
		else
			g_dlgDevice->XYMove(gstuRcgInfo.nClbPosX[gstuRcgInfo.Nxy-1],gstuRcgInfo.nClbPosY[gstuRcgInfo.Nxy-1]);
		Sleep(2000);
		g_dlgDevice->MHCtrl(0x01,1);//��
		Sleep(1000);
		g_dlgDevice->SNCtrl(0x01,1);//��
		Sleep(1000);
		g_dlgDevice->MHCtrl(0x01,0);//��
		Sleep(1000);
		//Sleep(1000);
		g_dlgDevice->AxisMove(0x02,0,1);//xy����
		Sleep(2000);
		g_dlgDevice->XYMove(x,y);//�ƶ���У׼��
		Sleep(1000);
		g_dlgDevice->MHCtrl(0x01,1);
		Sleep(1000);
		g_dlgDevice->SNCtrl(0x01,0);

		AfxMessageBox(_T("������ɰ�ȷ��"));
		g_dlgDevice->MHCtrl(0x01,0);
		Sleep(1000);

		g_dlgDevice->AxisMove(0x02,0,1);
		/////////////////////////////////////////////////////////////////
		gstuRcgInfo.bClbPos=0;
		g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("1��ͼ��ɼ���2��ʶ�����"));
		while (!gstuRcgInfo.bClbPos)
			Sleep(1);
		g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("1������Ļ����ȷѡ��У׼Ƭ"));
		do 
		{
			gstuRcgInfo.bClbPos = 0;
			while (!gstuRcgInfo.bClbPos)
			{
				Sleep(1);
			}
			int status=AfxMessageBox(_T("�Ƿ���ȷѡ��"),MB_OKCANCEL);
			if(status==1)//��ȷ���˳�
				break;
			else
			{
				gstuRcgInfo.bClbPos=0;
				g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("1��ͼ��ɼ���2��ʶ�����"));
				while (!gstuRcgInfo.bClbPos)
					Sleep(1);
				g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("1������Ļ����ȷѡ��У׼Ƭ"));
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
	//����
	CString strValue;
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[0][0]);
	WritePrivateProfileString(_T("У׼ϵ��"),_T("00"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[0][1]);
	WritePrivateProfileString(_T("У׼ϵ��"),_T("01"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[0][2]);
	WritePrivateProfileString(_T("У׼ϵ��"),_T("02"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[1][0]);
	WritePrivateProfileString(_T("У׼ϵ��"),_T("10"),strValue,gstuPathInf.csPathIni); 
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[1][1]);
	WritePrivateProfileString(_T("У׼ϵ��"),_T("11"),strValue,gstuPathInf.csPathIni);
	strValue.Format(_T("%.18lf"),gstuRcgInfo.g_factor[1][2]);
	WritePrivateProfileString(_T("У׼ϵ��"),_T("12"),strValue,gstuPathInf.csPathIni);
	g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT,_T("У׼���"));
	return 1;
}
#define COM_BUF_LEN 1024
char com_recv_buf[COM_BUF_LEN];
UINT com_recv_len;

UINT ComMsg(LPVOID pParam)
{
   bool crc;
	gstuRun.bPosSet==0;
	while(gclsCom.m_struct_com.hd!=INVALID_HANDLE_VALUE)
	{//��ontimer�л��밴ť��ͻ
    com_recv_len = gclsCom.RecvCharFromCom(com_recv_buf, COM_BUF_LEN, &crc, 5);
    if (com_recv_len > 0)
    {
      if (crc) {
        //��ӡ��str
        CString tmp;
        tmp = "\r\n";
        gclsCom.stuInf.str += tmp;
        for (UINT j = 0; j < com_recv_len; j++)
        {
          tmp.Format(_T("%02X"), (UCHAR)com_recv_buf[j]);
          gclsCom.stuInf.str += tmp;
        }
      }
      else {

        gclsCom.stuInf.str += "\r\n";
        gclsCom.stuInf.str += com_recv_buf;
        gclsCom.stuInf.str += "\0";
      }



      if (gclsCom.stuInf.str.GetLength() > 1000) 
      {
        gclsCom.stuInf.str = gclsCom.stuInf.str.Right(500);
      }
      gstuRefresh.bComUpdate = 1;
    }
			
		Sleep(10);
	}
	return 1;
}
// ZPJר�õ��̣߳����϶�û��
UINT Thread_Auto(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CCamera* pdlg = (CCamera*)pParam;
  char l_AnsiStr[MAX_PATH];
  int nWidth, nHeight;
  int nBitCount;
  int nBitsPerSample;
  int takeCnt = 0; 
  int turnTableCnt = 0;
  int status = -1;

  pdlg->GetDlgItem(IDC_BTN_AUTO)->EnableWindow(TRUE);

  while (gstuRun.unStatRun == RUN_STAT_RUN)
  {
    //=====================================================ʶ��ʼ===============================================================
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);
    TimeRecord(2, 1);//�������գ���ʱ��ʼ

    if (nullptr == gclsCamera.SnapEx(gnChannel, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, NULL))
    {
      Sleep(500);

      if (takeCnt++ > 10)//ȡ����ͼ���˳�
      {
        AfxMessageBox(_T("ȡ�������ͼ���������"));
        takeCnt = 0;
        gstuRun.unStatRun == RUN_STAT_STOP;
        pdlg->SetDlgItemText(IDC_BTN_AUTO, _T("�Զ�"));
        break;
      }
      continue;
    }

    TimeRecord(2, 0);//���ս�������ʱ����
    TimeRecord(3, 1);//ʶ��ʼ����ʱ��ʼ
    //
    //gclsImgRcg.RCGBMPSPLIT(NULL, NULL, l_AnsiStr, gstuRcgInfo.nPToL, gstuRcgInfo.nDefectPToL, gstuRcgInfo.nThreshold, 0, gstuRcgInfo.bIsCir, gstuRcgInfo.bThrdAuto, gstuRcgInfo.bDelNoise);
    

    gclsImgRcg.RCGBMP(NULL, NULL, l_AnsiStr, gstuRcgInfo.nPToL, gstuRcgInfo.nDefectPToL, gstuRcgInfo.nThreshold, 0, gstuRcgInfo.bIsCir, gstuRcgInfo.bThrdAuto, gstuRcgInfo.bDelNoise);
    //
    
    //
    TimeRecord(3, 0);//ʶ���������ʱ����
    //=====================================================���俪ʼ===============================================================

    //if (0 == gclsImgRcg.g_stu_square.nN)
    //{
    //  // ָʾplcתȦȦ
    //  turnTableCnt++;
    //  g_dlgDevice->TurnTableStatSet(turnTableCnt);

    //  CString csTmp;
    //  csTmp.Format(_T("��"));
    //  pdlg->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);

    //  csTmp.Format(_T("�ȴ�plcתȦ"));
    //  pdlg->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);

    //  do {
    //    g_dlgDevice->TurnTableStatGet(status);
    //  } while (status && RUN_STAT_RUN == gstuRun.unStatRun);

    //  continue;
    //}

    //turnTableCnt = 0;
    int i;

    for (i = 0; i < gclsImgRcg.g_stu_square.nN; i++)
    {
      /*if (PN_DEF != gclsImgRcg.g_stu_square.bPN[i])
        continue;*/
      if (0 == gclsImgRcg.g_stu_square.bPN[i])
          continue;

      if (
        gclsImgRcg.g_stu_square.pnZPX[i] < g_npc_inf.left ||
        gclsImgRcg.g_stu_square.pnZPX[i] > g_npc_inf.right ||
        gclsImgRcg.g_stu_square.pnZPY[i] < g_npc_inf.top ||
        gclsImgRcg.g_stu_square.pnZPY[i] > g_npc_inf.bottom
        )
        continue;
      //���ж�
      if (gclsImgRcg.g_stu_square.pnLen[i] < (gclsImgRcg.stuRef.Len - gclsImgRcg.stuRef.Dev) || gclsImgRcg.g_stu_square.pnLen[i] > (gclsImgRcg.stuRef.Len + gclsImgRcg.stuRef.Dev))
      {
        continue;
      }
      //���ж�
      if (gclsImgRcg.g_stu_square.pnWth[i] < (gclsImgRcg.stuRef.Wth - gclsImgRcg.stuRef.Dev) || gclsImgRcg.g_stu_square.pnWth[i] > (gclsImgRcg.stuRef.Wth + gclsImgRcg.stuRef.Dev))
      {
        continue;
      }


      turnTableCnt = 0;

      NpcParm par;
      //
      POINT pPoint[5];
      for (int j = 0; j < 4; j++)
      {
        pPoint[j].x = gclsImgRcg.g_stu_square.pnPX[i][j];
        pPoint[j].y = gclsImgRcg.g_stu_square.pnPY[i][j];
      }
      pPoint[4].x = gclsImgRcg.g_stu_square.pnPX[i][0];
      pPoint[4].y = gclsImgRcg.g_stu_square.pnPY[i][0];

      //test
      //double angle_test = gclsImgRcg.CalculateVectorAngle(pPoint[1].x - pPoint[0].x,
      //  pPoint[1].y - pPoint[0].y, 1, 0);
      /*int nLenNo = gclsImgRcg.g_stu_square.lenNo1PN[i];
      int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
      */

      par.deg0 = gclsImgRcg.g_stu_square.angreePN[i];// angle_test;
      par.x0 = gclsImgRcg.g_stu_square.pnZPX[i];
      par.y0 = gclsImgRcg.g_stu_square.pnZPY[i];
      par.pn0 = gclsImgRcg.g_stu_square.bPN[i];//gstuRcgInfo.nPN;gclsImgRcg.g_stu_square.bPN[i]

      CString csTmp;
      csTmp.Format(_T("��%d ��%d ���� %d �Ƕ�%.1f X:%d Y:%d"), gclsImgRcg.g_stu_square.pnLen[i], gclsImgRcg.g_stu_square.pnWth[i], (int)(par.pn0), par.deg0, (int)(par.x0), (int)(par.y0));
      pdlg->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
      TranNpcParam(&par);

      csTmp.Format(_T("No%d ���� %d �Ƕ�%d X:%d Y:%d"), i, par.pn, par.deg, par.x, par.y);
      pdlg->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);

      //���Ͳ���
      while (0 != g_dlgDevice->ParamMove(par.x, par.y, par.deg, par.pn) && RUN_STAT_RUN == gstuRun.unStatRun)
      {
        Sleep(1000);
      }

      csTmp.Format(_T("�����������"));
      pdlg->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
      // �ȴ������ҵ�������
     
      do {
        status = -1;
        g_dlgDevice->StatusGet(status);
        Sleep(500);
      } while (0 != status && RUN_STAT_RUN == gstuRun.unStatRun);

      csTmp.Format(_T("�ȴ�ȡƬ��ҵ���"));
      pdlg->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);

      break;//ȡ��Ƭ֮������ᶶ����������
    }//for (int i = 0; i < gclsImgRcg.g_stu_square.nN; i++)

    if (i >= gclsImgRcg.g_stu_square.nN)
    {
      // ָʾplcתȦȦ
      turnTableCnt++;
      g_dlgDevice->TurnTableStatSet(1/*turnTableCnt*/);

      CString csTmp;
      csTmp.Format(_T("��"));
      pdlg->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);

      csTmp.Format(_T("�ȴ�plcתȦ"));
      pdlg->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);

      do {
        g_dlgDevice->TurnTableStatGet(status);
      } while (status && RUN_STAT_RUN == gstuRun.unStatRun);

      continue;
    }


  }//while (gstuRun.unStatRun == RUN_STAT_RUN)
  pdlg->GetDlgItem(IDC_BTN_AUTO)->EnableWindow(TRUE);

  return 0;
}

//����ϵת��ϵ������
void calparameter(double (*xy)[4],double *factor)
	//void calparameter(double **xy,double *factor)
{
	int k,i,j,f;//ѭ������
	for(k=0;k<3;k++)//����Ԫ��˹��Ԫ               
	{
		//double ark=findmax(k,xy);
		double max=0;
		for(int i=k;i<3;i++)
			if(fabs(xy[i][k])>max)        //�������е�Ԫ�ؽ���ȡ����ֵ�Ƚϴ�С������
			{
				max=fabs(xy[i][k]);
				f=i;
			}

			if(max==0)
			{
				AfxMessageBox(_T("�˷������޽�!"));
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
	factor[2]=xy[2][3]/xy[2][2];      //��������һ��δ֪���Ľ�
	for( k=1;k>=0;k--)//�ص�
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
		(degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]),//����
		(degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]),//����
		(degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]),//��Ч��
		(degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]),//ԭʼ����
		(degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29])
	};//ԭʼ���᣻����ⶨ���ĽǶȣ�ת������

	int t0 = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5])/* + g_sort.R1t[g_sort.R1Num]*/;//����//_0614
	int e0 = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11])/* + g_sort.R1e[g_sort.R1Num]*/;//����
	int equal_angle;

	// ��Ч�� = ԭʼ���� + ��ԭʼ���� - ��Ч�ǵ��������/��Ч������
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
//�ֵ��������ʹ�õ�ȫ�ֱ���
//���ص�λֵ������е�ֵַ� 0-29
int SortChip(sortchip* s_sort,int *degree)
{
	bool elecyes=1;//�޶���ͨ����־
	int sortSN=-1;//����ֵ���ֵ�ֵ

	int angle[5]={(degree[0]*10+degree[1])*3600+(degree[2]*10+degree[3])*60+(degree[4]*10+degree[5]),//����
		(degree[6]*10+degree[7])*3600+(degree[8]*10+degree[9])*60+(degree[10]*10+degree[11]),//����
		(degree[12]*10+degree[13])*3600+(degree[14]*10+degree[15])*60+(degree[16]*10+degree[17]),//��Ч��
		(degree[18]*10+degree[19])*3600+(degree[20]*10+degree[21])*60+(degree[22]*10+degree[23]),//ԭʼ����
		(degree[24]*10+degree[25])*3600+(degree[26]*10+degree[27])*60+(degree[28]*10+degree[29])};//ԭʼ���᣻����ⶨ���ĽǶȣ�ת������
	if(degree[0]==-1 || angle[3]==0)//�Ƕ�û���
	{
		sortSN=29;
		s_sort->sortnum[29]++;
		s_sort->nocheckout++;
		return sortSN;
	}
	if(s_sort->nocheckout > 0)
		s_sort->nocheckout--;
	if (0==angle[0] && angle[3]!=0)//�����ⲻ��
	{
		sortSN=28;
		s_sort->sortnum[28]++;
		s_sort->checkout0++;
		return sortSN;
	}
	if (s_sort->checkout0 > 0)
		s_sort->checkout0--;
	if(s_sort->sortsel2!=3)//��������޶�
	{
		switch((angle[s_sort->sortsel2]<=s_sort->elechigh?-1:0)+(angle[s_sort->sortsel2]>=s_sort->eleclow?1:0))//�����޶��Ტ�ֵ�
		{
		case 0:break;
		case 1:elecyes=0;s_sort->sortnum[25]++;sortSN=25;break;
		case -1:elecyes=0;s_sort->sortnum[24]++;sortSN=24;break;
		}
	}
	if(1==elecyes)//����޶���ͨ��
	{
		if(angle[s_sort->sortsel1]>=(s_sort->centerangle-s_sort->sortvalue/2))//�����������Ԥ��ֵ
		{
			switch((angle[s_sort->sortsel1]-s_sort->centerangle+s_sort->sortvalue/2)/s_sort->sortvalue+1)//�������Ტ�ֵ�����������Ԥ��ֵ
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
			switch((s_sort->centerangle-s_sort->sortvalue/2-angle[s_sort->sortsel1]-1)/s_sort->sortvalue+1)//�������Ტ�ֵ��������С��Ԥ��ֵ
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
		avg = xi;//ƽ��ֵ
		std2 = xi * xi;//��׼ƫ���м�ֵ
	}
	if (n > 1)
	{
		avg = (avg + xi / (n - 1)) / (n / (n - 1));//��ƽ��ֵ
		std2 = (std2 + xi * xi / (n - 1)) / (n / (n - 1));//���׼���м�ֵ
		std = sqrt(abs(std2 - avg * avg)*n / (n - 1));//���׼��
	}
}
void TranNpcParam(NpcParm* parm)
{

  parm->x = gstuRcgInfo.g_factor[0][0] * parm->x0 + gstuRcgInfo.g_factor[0][1] * parm->y0 + gstuRcgInfo.g_factor[0][2] + 0.5;
  parm->y = gstuRcgInfo.g_factor[1][0] * parm->x0 + gstuRcgInfo.g_factor[1][1] * parm->y0 + gstuRcgInfo.g_factor[1][2] + 0.5;


  parm->deg = parm->deg0 * Z_P_PER_DEG + Z_S+0.5;

  parm->pn = -parm->pn0;

  if (parm->pn > 0)
  {
    parm->deg += g_npc_inf.offset_p;
  }
  else if (parm->pn < 0)
  {
    parm->deg += g_npc_inf.offset_n;
  }

  parm->deg = parm->deg - (parm->deg / Z_P) * Z_P;

  if (parm->deg < Z_P)
  {
    parm->x += g_npc_inf.offset_x1;
    parm->y += g_npc_inf.offset_y1;
  }
  else
  {
    parm->x += g_npc_inf.offset_x2;
    parm->y += g_npc_inf.offset_y2;
  }


}
//�������ƽ��ֵ����׼����
void SortAvgStd(unsigned int sum,int *angle,double *avg,double *std,long double *std2)
{
	double n=sum;
	for(int i=0;i<5;i++)
	{
		double xi=angle[i];
		//double avg;
		if(1==n)
		{
			avg[i]=xi;//ƽ��ֵ
			std2[i]=xi*xi;//��׼ƫ���м�ֵ
		}
		if(n>1)
		{
			avg[i]=(avg[i]+xi/(n-1))/(n/(n-1));//��ƽ��ֵ
			std2[i]=(std2[i]+xi*xi/(n-1))/(n/(n-1));//���׼���м�ֵ
			std[i]=sqrt(abs(std2[i]-avg[i]*avg[i])*n/(n-1));//���׼��
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
			csRec.Format(_T("��%.3f\r\n"),ldv_tim);
			break;
		case 1:
			csRec.Format(_T("��Ƭ����������������ǰ��%.3f\r\n"),ldv_tim);
			break;
		case 2:
			csRec.Format(_T("���ץ�ģ�%.3f\r\n"),ldv_tim);
			break;
		case 3:
			csRec.Format(_T("ʶ����̣�%.3f\r\n"),ldv_tim);
			break;
		case 4:
			csRec.Format(_T("��������̣߳�%.3f\r\n"),ldv_tim);
			break;
		case 5:
			csRec.Format(_T("��Ƭ����������������꣺%.3f\r\n"),ldv_tim);
			break;
		case 6:
			csRec.Format(_T("�������굽���յ���һ����Ƭ���%.3f\r\n"),ldv_tim);
			break;
		case 7:
			csRec.Format(_T("��%.3f\r\n"),ldv_tim);
			break;
		case 8:
			csRec.Format(_T("��%.3f\r\n"),ldv_tim);
			break;
		case 9:
			csRec.Format(_T("��%.3f\r\n"),ldv_tim);
			break;
		default:
			break;
		}

		gclsTxt.TXTAddStr(gstuPathInf.csPathLog,csRec);
	}
	return ldv_tim;

}