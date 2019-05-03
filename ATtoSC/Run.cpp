// Run.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ATtoSC.h"
#include "Run.h"
#include "afxdialogex.h"
#include "ATtoSCDlg.h"
//#include <math.h>
extern threadstatus g_status;
extern sortchip g_sort;
//extern runtime g_time;
extern int degree[30];
extern bool updatesort;
extern HANDLE g_hDevice;
int g_sortgroup[30] = { 1000 };
//CString pathSaveDegree=(_T(".\\check\\SortDegree.txt"));//_0610
extern int *g_num;
extern int g_height;
extern int g_width;
extern BYTE *g_lpGray;
extern int head[10][8], height_num, width_num;//ͼƬ��־����
extern RGBCAPTUREBUFFER pCaptureBuffer;
extern RGBBITMAP       RGBBitmap1;     //��һ���ɼ��¼�λͼ��Ϣ
extern TPicRegion pDest;
extern double m_VGAthreshold;
extern void LoadMark(CString file_temp);
extern void FindOrigin(BYTE* lpGray);
extern void FindOriginEX(BYTE* lpGray);
extern void GetNumber();
extern void ScanDegree(BYTE * lpGray, CString file_temp);
extern void ScanDegreeEX(BYTE * lpGray, CString file_temp);
extern bool ScanDegreeEFGM(BYTE * lpGray, CString file_temp);
extern bool ScanDegreeEFGMEX(BYTE * lpGray, CString file_temp);
extern BOOL SaveImageToFile(CString sFile, int iW, int iH, BYTE * pInfo);
CCriticalSection ac6641_critical;
CCriticalSection vga_critical;
int errorStep = 0;//��ǣ�ȷ���ƶ���ʽ��ȷ����y���߳���2����תʱ������
bool yOver;//��ǣ�ȷ����ʼ��y���ȳ�ʼ�˶���ɣ�x�����˶�
CTime culTime1, culTime2, pauseT1, pauseT2;//��ȡʱ����
int i_bs = 0;
CFont font;

int nShkStat = 0;

int checkstate = 0;

BOOL TXTReadRowStr(LPCTSTR pathTXT, CString& str, UINT row);

BOOL Split(char* source, char*& dest, char division, UINT iSub);

int RCGVGA(void)//����Ļ
{
	//CCSVFile* csv = (CCSVFile*)pParam;
	int rechecknum = 0;//�ز����
	//while (1)
	//{

	//	while (1)//�ȴ�EFG�����
	//	{
	//		if (g_status._CHK2 && 1 == g_status.SORT_END)
	//			break;
	//		if (1 == g_status.SCR_STOP)
	//		{
	//			g_status.SOT_STOP = 1;//���Ʒֵ��߳�ֹͣ
	//			g_status.SCR_STOP = 0;
	//			return 0;
	//		}
	//		Sleep(1);
	//	}
	//	g_status._CHK2 = 0;
	//	g_status.SORT_END = 0;
		//
		Sleep(200);
		if (!g_status._DEBUGSCR)
		{
			vga_critical.Lock();
			while (1)
			{

				DWORD width = RGBBitmap1.bmiHeader.biWidth;
				DWORD height = -RGBBitmap1.bmiHeader.biHeight;

				pDest.pdata = new TARGB32[width * height];//b,g,r,a  4��8λ���һ������
				pDest.height = height;
				pDest.width = width;
				pDest.byte_width = width << 2;

				g_height = height;
				g_width = width;
				g_lpGray = new BYTE[g_height * g_width];

				int iW = width, iH = height;
				//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
				//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

				DECODE_RGB_TO_GRAY((const TUInt8*)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

				//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
				DECODE_THRESHOLD(pDest, pDest, g_lpGray, m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

				//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
				delete[] pDest.pdata;
				pDest.pdata = NULL;
				LoadMark(g_path + _T("\\check\\qz_42.txt"));

				FindOrigin2(g_lpGray);
				if (height_num > 100 || width_num > 200 || (height_num < 0 && width_num < 0))
				{
					if (rechecknum > 20)
					{
						AlertCtrl(1);
						//AfxMessageBox(_T("�ֵ�ֵû������������"));
						int status = AfxMessageBox(_T("�ֵ�ֵû������������"), MB_OKCANCEL);
						if (status == 2)//ȡ��
						{
							delete[]g_lpGray;
							g_lpGray = NULL;
							vga_critical.Unlock();
							for (int i = 0; i < 30; i++)
								degree[i] = 0;//ģ��end
							g_sort.sortsn = g_sort.itemnum + 6;
							return 0;
						}

						rechecknum = 0;
						AlertCtrl(0);
					}
					rechecknum++;
					//delete []g_lpGray;
					//g_lpGray=NULL;
				}
				else
				{
					rechecknum = 0;
					//g_status._CHK1=1;//���ֵ��߳�
					break;
				}
			}
			g_num = new int[10 * 10 * 8];
			GetNumber();

			ScanDegreeDynamic2(g_lpGray, g_path + _T("\\check\\positionDynamic.txt"));//���degree[]
			g_sort.R1Num = DOUBLE(degree[0] * 100 + degree[1] * 10 + degree[2] + degree[3] * 0.1 + degree[4] * 0.01 + degree[5] * 0.001);

			ScanDegree2(g_lpGray, g_path + _T("\\check\\position.txt"));//���degree[]
			delete[]g_num;
			delete[]g_lpGray;
			g_lpGray = NULL;
			g_num = NULL;
			vga_critical.Unlock();
		}
		else
		{
			srand((int)time(0)); //ģ��begin
			int deg[30] = { 3,4,0,rand() % 9,rand() % 5,rand() % 9,
				2,2 + rand() % 3,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
				rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
				rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
				rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9 };
			for (int i = 0; i < 30; i++)
				degree[i] = deg[i];//ģ��end
		}
		CalcEquAngle(); // DATE:180421
		g_sort.sortsn = SortChipR1(&g_sort, degree);//�õ���λֵ
		if (g_sort.sortsn <= 0)
			g_sort.sortsn = g_sort.itemnum + 6;
		//g_status._SCREND = 1;//��λ��ǣ����ֵ��߳�
		///*culTime2=CTime::GetCurrentTime();
		//if(culTime2.GetDay()!=culTime1.GetDay())
		//g_time.end=24;
		//else
		//g_time.end=0;

		//g_time.end=(g_time.end+culTime2.GetHour())*3600+culTime2.GetMinute()*60+culTime2.GetSecond();
		//g_time.sum=g_time.end-g_time.start-g_time.pause_sum;
		//g_time.avg=double(g_time.sum)/double(g_sort.sortsum+g_sort.sortnum[24]+g_sort.sortnum[25]+g_sort.sortnum[26]+g_sort.sortnum[27]+g_sort.sortnum[28]+g_sort.sortnum[29]);
		//*/updatesort = 1;
		//g_status._SHOW = 1;
	//	//==============//����Ƕȵ��ļ�
	//	//CStdioFile file;
	//	//file.Open(pathSaveDegree,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	//	CString _degree;
	//	_degree.Format(_T("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n"), g_status._N, degree[0] * 100000 + degree[1] * 10000 + degree[2] * 1000 + degree[3] * 100 + degree[4] * 10 + degree[5],
	//		degree[6] * 100000 + degree[7] * 10000 + degree[8] * 1000 + degree[9] * 100 + degree[10] * 10 + degree[11],
	//		degree[12] * 100000 + degree[13] * 10000 + degree[14] * 1000 + degree[15] * 100 + degree[16] * 10 + degree[17],
	//		degree[18] * 100000 + degree[19] * 10000 + degree[20] * 1000 + degree[21] * 100 + degree[22] * 10 + degree[23],
	//		degree[24] * 100000 + degree[25] * 10000 + degree[26] * 1000 + degree[27] * 100 + degree[28] * 10 + degree[29],
	//		g_sort.sortsn);
	//	//file.Seek(0,CFile::end);
	//	//file.WriteString( _degree );
	//	//file.Close();
	//	//
	//	CFile myFile;
	//	myFile.Open(pathSaveDegree, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	//	char szANSIString[MAX_PATH];
	//	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, _degree, -1, szANSIString, sizeof(szANSIString), NULL, NULL);
	//	myFile.Seek(0, CFile::end);
	//	//myFile.Write(szANSIString,_sort.GetLength()*2-98);
	//	myFile.Write(szANSIString, CStringA(_degree).GetLength());
	//	myFile.Close();
	//	//���̿���¼��by mmy 171115
	//	if (csv)
	//	{
	//		// CCSVFile csv(g_path + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);
	//		CStringArray arr;
	//		CString str;
	//		str.Format(_T("%d"), g_status._N);
	//		arr.Add(str);
	//		str.Format(_T("%d"), degree[0] * 100000 + degree[1] * 10000 + degree[2] * 1000 + degree[3] * 100 + degree[4] * 10 + degree[5]);
	//		arr.Add(str);
	//		str.Format(_T("%d"), degree[6] * 100000 + degree[7] * 10000 + degree[8] * 1000 + degree[9] * 100 + degree[10] * 10 + degree[11]);
	//		arr.Add(str);
	//		str.Format(_T("%d"), degree[12] * 100000 + degree[13] * 10000 + degree[14] * 1000 + degree[15] * 100 + degree[16] * 10 + degree[17]);
	//		arr.Add(str);
	//		str.Format(_T("%d"), degree[18] * 100000 + degree[19] * 10000 + degree[20] * 1000 + degree[21] * 100 + degree[22] * 10 + degree[23]);
	//		arr.Add(str);
	//		str.Format(_T("%d"), degree[24] * 100000 + degree[25] * 10000 + degree[26] * 1000 + degree[27] * 100 + degree[28] * 10 + degree[29]);
	//		arr.Add(str);
	//		str.Format(_T("%d"), g_sort.sortsn);
	//		arr.Add(str);

	//		csv->WriteData(arr);

	//		arr.RemoveAll();
	//	}
	//	//��Ƭ����
	//	/*if(g_sort.sortnum[g_sort.sortsn-1]>60)
	//	{
	//	//g_sort.alertnum[g_sort.sortsn-1]++;
	//	CString tmp;
	//	tmp.Format(_T("%d��Ƭ̫����"),g_sort.sortsn);
	//	int status=0;
	//	AlertCtrl(1);
	//	status=AfxMessageBox(tmp);
	//	g_sort.sortnum[g_sort.sortsn-1]=0;
	//	AlertCtrl(0);
	//	}*/
	//}
	//return 0;
		return 1;
}
void WaitVGAEnd(void)
{
	while (1)//�ȴ�EFG����겢��λ���
	{
		if ((AC6641_DI(g_hDevice, 6) & 0x02) )//���ߵ�ƽ��chk1��2����0��ʱ�򣬼�⿪ʼ
		{
			Sleep(50);
			if ((AC6641_DI(g_hDevice, 6) & 0x02) )
			{
				Sleep(50);
				if ((AC6641_DI(g_hDevice, 6) & 0x02) )
				{
					while (1)
					{
						if (!(AC6641_DI(g_hDevice, 6) & 0x02))//����ʱ������
						{
							Sleep(40);
							if (!(AC6641_DI(g_hDevice, 6) & 0x02))//����ʱ������
							{
								Sleep(40);
								if (!(AC6641_DI(g_hDevice, 6) & 0x02))//���η�������ֹ�ڼ��ʱ���ָ��ŵ��¶���Ļ�ȶ�
									break;
							}
						}
						Sleep(1);
					}
					break;
				}
			}

		}

	}
}
//�̺߳���
UINT TakeThread(LPVOID pParam)//ȡƬX��
{
	CTime time1, time2;
	time1 = CTime::GetCurrentTime();
	//Sleep(1000);
	nShkStat = 0;
	int nNoBlowN = 0;
	while (1)
	{
		while (!yOver)
			Sleep(1);
		ReturnZero(1);//����
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) & 0xfd);//ֹͣ���Ե͵�ƽ
		ac6641_critical.Unlock();
		while (1)//�ȴ���������Ƭ�ź�
		{
			if (1 == g_status.TAK_STOP)
			{
				g_status.CHK_STOP = 1;//���Ƽ���߳�ֹͣ
				g_status.TAK_STOP = 0;
				return 0;
			}
			if (g_status._DEBUGSCR)
				break;
			while (1)
			{
				if (!g_status.SYS_PAUSE)
					break;
				else
					time1 = CTime::GetCurrentTime();
				Sleep(1);
			}

			if (3 > nShkStat)
			{
				if (ShakeUp)
					if (ShakeUp)
					{
						nShkStat++;
						break;
					}
			}
			else
			{
				/*AlertCtrl(1);
				AfxMessageBox(_T("������û��ȥ"));
				AlertCtrl(0);*/
				::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_DLGALM, 0, NULL);//������ʾ��Ϣ��������

				nShkStat = 0;
			}

			time2 = CTime::GetCurrentTime();

			if (time2.GetSecond() - time1.GetSecond() > 30 && time2.GetSecond() - time1.GetSecond() < 60
				||
				time2.GetSecond() - time1.GetSecond() > -30 && time2.GetSecond() - time1.GetSecond() < 0)
			{
				//int status=0;			
				//AlertCtrl(1);
				////status=AfxMessageBox(_T("��ʱ"));
				//MyMsgBoxReadIo("��ʱ");
				//AlertCtrl(0);
				MyMsgBoxReadIo("��ʱ");
				time1 = CTime::GetCurrentTime();

			}

			Sleep(1);
		}
		//
		/*while(1)
		{
		if(!g_status.SYS_PAUSE)
		break;
		Sleep(1);
		}*/
		Sleep(300);
		TubeCtrl(1, 1);//��Ƭ
		Sleep(g_tim.onX);
		MotorCtrl(1, 1, g_status.Xwait);//�ƶ����ȴ���Ƭλ��

		if (!g_status._SORTTAK)
		{

			while (1)//�ȴ��ֵ���ȡ��
			{
				if (g_status._SORTTAK)
				{
					//Sleep(10);//_0610
					break;
				}
				/*if(1==g_status.TAK_STOP)
				{
				g_status.TAK_STOP=0;
				return 0;
				}*/
				while (1)
				{
					if (!g_status.SYS_PAUSE)
						break;
					else
						time1 = CTime::GetCurrentTime();
					Sleep(1);
				}

				time2 = CTime::GetCurrentTime();

				if (time2.GetSecond() - time1.GetSecond() > 30 && time2.GetSecond() - time1.GetSecond() < 60
					||
					time2.GetSecond() - time1.GetSecond() > -30 && time2.GetSecond() - time1.GetSecond() < 0)
				{

					AlertCtrl(1);
					Sleep(2000);
					AlertCtrl(0);
					time1 = CTime::GetCurrentTime();

				}

				Sleep(1);
			}
		}
		else//_0610
		{
			Sleep(1);
		}
		g_status._SORTTAK = 0;
		//Sleep(20);
		while (1)//0816
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x02))//���EFG�͵�ƽ���������
				break;
			else
			{
				ac6641_critical.Lock();
				AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) & 0xfd);//ֹͣ���Ե͵�ƽ
				ac6641_critical.Unlock();
			}
			Sleep(5);
		}

		if (nNoBlowN > 100)//����֧�ܵ���
		{

			nNoBlowN = 0;
			ac6641_critical.Lock();
			TBLOW;
			ac6641_critical.Unlock();
			BlowBlock(2000);//��Ƭ
			Sleep(3000);
			ac6641_critical.Lock();
			TNOBLOW;
			ac6641_critical.Unlock();

		}


		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) | 0x02);//��ʼ���Ըߵ�ƽ
		ac6641_critical.Unlock();
		Sleep(30);//ȷ���ֵ�������
		//StartFT();//��Ϊ��ʼ���֮����500�ӳ٣��Ѿ���ʼ�⵫�ֱ�û�뿪�������á�

		BlowBlock(g_tim.blow);//��Ƭ
		MotorCtrl(1, 1, g_status.Xcheck - g_status.Xwait);//�ƶ�����Ƭλ��

		Sleep(50);
		TubeCtrl(1, 0);//����
		nNoBlowN++;
		Sleep(g_tim.offX);

		g_status._DEBUGEFG = 1;
		//ReturnZero(2);//����
		time1 = CTime::GetCurrentTime();
	}
	return 0;
}
UINT SortThread(LPVOID pParam)//�ֵ�Y��
{
	ReturnZero(1);//����
	ReturnZero(2);//����
	TubeCtrl(2, 1);//����
	Sleep(g_tim.onY);
	MotorCtrl(2, 1, g_status.Ywait);//�ƶ����ȴ�λ��
	yOver = 1;
	Sleep(100);
	TubeCtrl(2, 0);//����
	Sleep(g_tim.offY);
	/*CTime time1,time2;
	time1=CTime::GetCurrentTime();*/
	while (1)
	{
		while (1)//�ȴ�EFG�����
		{
			if (g_status._CHK1)
				break;
			if (1 == g_status.SOT_STOP)
			{
				g_status.DSP_STOP = 1;//������ʾ�߳�ֹͣ
				g_status.SOT_STOP = 0;
				return 0;
			}
			while (1)
			{
				if (!g_status.SYS_PAUSE)
					break;
				/*else
				time1=CTime::GetCurrentTime();*/
				Sleep(1);
			}
			/*time2=CTime::GetCurrentTime();
			if(time2.GetSecond()-time1.GetSecond()>30&&time2.GetSecond()-time1.GetSecond()<60
			||
			time2.GetSecond()-time1.GetSecond()>-30&&time2.GetSecond()-time1.GetSecond()<0)
			{
			int status=0;
			AlertCtrl(1);
			status=AfxMessageBox(_T("��ʱ"));

			AlertCtrl(0);
			time1=CTime::GetCurrentTime();

			}*/
			Sleep(1);
		}
		g_status._CHK1 = 0;
		while (1)
		{
			if (!g_status.SYS_PAUSE)
				break;
			Sleep(1);
			//else
			//time1=CTime::GetCurrentTime();
		}

		/*
		vga_critical.Lock();//ȡ֮ǰ���ж�Mֲ���������Ļ�г�ͻ����lock
		if(!ScanDegreeEFGM(g_lpGray,_T("check\\positionM.txt")))//���Mֵ
		{
		AlertCtrl(1);
		AfxMessageBox(_T("Mֵ������,������⣿"));
		AlertCtrl(0);
		}
		vga_critical.Unlock();
		*/
		ReturnZero(2);//����
		Sleep(100);
		TubeCtrl(2, 1);//����
		Sleep(g_tim.onY);
		//MotorCtrl(2,1,g_status.Ywait/4,1);//ת�����ȴ�
		//MotorCtrl(2,1,g_status.Ywait+i_bs);//ת�����ȴ�

		g_status.SORT_END = 1;//������Ļ�̣߳����Զ�����
		while (1)//�ȴ���Ļ��ȡ��
		{
			if (g_status._SCREND)
				break;
			if (1 == g_status.SOT_STOP)
			{
				g_status.DSP_STOP = 1;//������ʾ�߳�ֹͣ
				g_status.SOT_STOP = 0;
				return 0;
			}
			Sleep(1);
		}
		g_status._SORTTAK = 1;//��ȡƬ�߳�//0610����һ�佻��


		MotorCtrl(2, 1, g_status.Ywait);//ת�����ȴ�


		Sleep(500);

		//time1=CTime::GetCurrentTime();
		g_status._SCREND = 0;
		//g_status._SHOW=1;
		//updatesort=1;
		SortBlock(g_sort.sortsn);//�ֵ�	
	}
	return 0;
}

UINT SortThreadWithZ(LPVOID pParam)//�ֵ�Y��
{
	ReturnZero(1);//����
	ReturnZero(2);//����
	TubeCtrl(2, 1);//����
	Sleep(g_tim.onY);
	MotorCtrl(2, 1, g_status.Ywait);//�ƶ����ȴ�λ��
	yOver = 1;
	Sleep(100);
	TubeCtrl(2, 0);//����
	Sleep(g_tim.offY);
	/*CTime time1,time2;
	time1=CTime::GetCurrentTime();*/
	while (1)
	{
		while (1)//�ȴ�EFG�����
		{
			if (g_status._CHK1)
				break;
			if (1 == g_status.SOT_STOP)
			{
				//g_status.DSP_STOP=1;//������ʾ�߳�ֹͣ
				g_status.CY_STOP = 1;
				g_status.SOT_STOP = 0;
				return 0;
			}
			while (1)
			{
				if (!g_status.SYS_PAUSE)
					break;
				/*else
				time1=CTime::GetCurrentTime();*/
				Sleep(1);
			}
			/*time2=CTime::GetCurrentTime();
			if(time2.GetSecond()-time1.GetSecond()>30&&time2.GetSecond()-time1.GetSecond()<60
			||
			time2.GetSecond()-time1.GetSecond()>-30&&time2.GetSecond()-time1.GetSecond()<0)
			{
			int status=0;
			AlertCtrl(1);
			status=AfxMessageBox(_T("��ʱ"));

			AlertCtrl(0);
			time1=CTime::GetCurrentTime();

			}*/
			Sleep(1);
		}
		g_status._CHK1 = 0;
		while (1)
		{
			if (!g_status.SYS_PAUSE)
				break;
			Sleep(1);
			//else
			//time1=CTime::GetCurrentTime();
		}

		/*
		vga_critical.Lock();//ȡ֮ǰ���ж�Mֲ���������Ļ�г�ͻ����lock
		if(!ScanDegreeEFGM(g_lpGray,_T("check\\positionM.txt")))//���Mֵ
		{
		AlertCtrl(1);
		AfxMessageBox(_T("Mֵ������,������⣿"));
		AlertCtrl(0);
		}
		vga_critical.Unlock();
		*/
		ReturnZero(2);//����
		Sleep(100);
		TubeCtrl(2, 1);//����
		Sleep(g_tim.onY);
		//MotorCtrl(2,1,g_status.Ywait/4,1);//ת�����ȴ�
		//MotorCtrl(2,1,g_status.Ywait+i_bs);//ת�����ȴ�
		g_status._SORTTAK = 1;//��ȡƬ�߳�//0610����һ�佻��
		g_status.SORT_END = 1;//������Ļ�̣߳����Զ�����
		MotorCtrl(2, 1, g_status.Ypass);//ת������Ƭ��
		Sleep(200);

		while (1)//�ȴ���Ļ��ȡ��
		{
			if (g_status._SCREND && 0 == g_status.CyStat)
				break;
			if (1 == g_status.SOT_STOP && 0 == g_status.CyStat)
			{
				////g_status.DSP_STOP=1;//������ʾ�߳�ֹͣ
				//g_status.CY_STOP=1;
				//g_status.SOT_STOP=0;
				//return 0;
				break;
			}
			Sleep(1);
		}
		//time1=CTime::GetCurrentTime();
		g_status._SCREND = 0;
		//g_status._SHOW=1;
		//updatesort=1;
		TubeCtrl(2, 0);//����
		Sleep(g_tim.offY);
		g_status.CyBlock = 1;
		MotorCtrl(2, 0, g_status.Ypass - g_status.Ywait);//ת�����ȴ�
	}
	return 0;
}

UINT Sort2Thread(LPVOID pParam)//�ֵ�z��
{
	g_status.CyStat = 0;
	g_status.CyBlock = 1;
	ReturnZero(3);//����
	Sleep(100);
	g_sort.sortsn = g_sort.itemnum + 6;
	while (!g_status.SOT2_STOP)
	{
		Sleep(1);
		if (1 == g_status.CyStat)
		{
			TubeCtrl(3, 1);//����
			Sleep(g_tim.onZ);
			g_status.CyBlock = 0;
			SortBlockWithZ(g_sort.sortsn);//�ֵ�
		}

	}
	g_status.DSP_STOP = 1;//������ʾ�߳�ֹͣ
	g_status.SOT2_STOP = 0;
	return 0;
}
UINT CylinderThread(LPVOID pParam)
{
	while (!g_status.CY_STOP)
	{
		Sleep(1);
		switch (g_status.CyBlock)//�����������Ƭ
		{
		case 0:
			CylinderCtrl(false);
			break;
		case 1:
			CylinderCtrl(true);
			break;
		default:
			break;
		}

	}
	g_status.SOT2_STOP = 1;
	g_status.CY_STOP = 0;
	return 0;
}
UINT CheckThread(LPVOID lParam)//���EFG
{
	while (1)
	{
		checkstate = 0;
		while (1)//�ȴ�EFG����겢��λ���
		{
			if ((AC6641_DI(g_hDevice, 6) & 0x02) && !g_status._CHK1 && !g_status._CHK2&& g_status._DEBUGEFG == 1)//���ߵ�ƽ��chk1��2����0��ʱ�򣬼�⿪ʼ
			{
				Sleep(50);
				if ((AC6641_DI(g_hDevice, 6) & 0x02) && !g_status._CHK1 && !g_status._CHK2)
				{
					Sleep(50);
					if ((AC6641_DI(g_hDevice, 6) & 0x02) && !g_status._CHK1 && !g_status._CHK2)
					{
						checkstate = 1;//��ǽ�����״̬
						while (1)
						{
							if (!(AC6641_DI(g_hDevice, 6) & 0x02))//����ʱ������
							{
								Sleep(40);
								if (!(AC6641_DI(g_hDevice, 6) & 0x02))//����ʱ������
								{
									Sleep(40);
									if (!(AC6641_DI(g_hDevice, 6) & 0x02))//���η�������ֹ�ڼ��ʱ���ָ��ŵ��¶���Ļ�ȶ�
										break;
								}
							}
							Sleep(1);
						}
						break;
					}
				}

			}

			/*if(g_status._DEBUGEFG && !g_status._CHK1 && !g_status._CHK2)//����
			{
			g_status._DEBUGEFG=0;
			Sleep(2000);
			break;
			}*/
			//Sleep(100);
			Sleep(1);
			if (1 == g_status.CHK_STOP)
			{
				g_status.SCR_STOP = 1;//���ƶ���Ļֹͣ
				g_status.CHK_STOP = 0;
				return 0;
			}

		}
		g_status._DEBUGEFG = 0;
		//Sleep(10);
		g_status._CHK1 = 1;//���ֵ��߳�
		g_status._CHK2 = 1;//������Ļ�߳�
	}
	return 0;
}
UINT ScreenThread(LPVOID pParam)//����Ļ
{
	CCSVFile *csv = (CCSVFile*)pParam;
	int rechecknum = 0;//�ز����
	while (1)
	{

		while (1)//�ȴ�EFG�����
		{
			if (g_status._CHK2 && 1 == g_status.SORT_END)
				break;
			if (1 == g_status.SCR_STOP)
			{
        // CPK 190503 ������ber
        if (g_pCpk)
        {
          CProcessCard processCard;

          g_pCpk->CalcCpk(processCard);

          g_pCpk->AddCpkToPlannedCsv(processCard);
        }

				g_status.SOT_STOP = 1;//���Ʒֵ��߳�ֹͣ
				g_status.SCR_STOP = 0;
				return 0;
			}
			Sleep(1);
		}
		g_status._CHK2 = 0;
		g_status.SORT_END = 0;
		//
		//Sleep(200);
		//if (!g_status._DEBUGSCR)
		//{
		//	vga_critical.Lock();
		//	while (1)
		//	{

		//		DWORD width = RGBBitmap1.bmiHeader.biWidth;
		//		DWORD height = -RGBBitmap1.bmiHeader.biHeight;

		//		pDest.pdata = new TARGB32[width*height];//b,g,r,a  4��8λ���һ������
		//		pDest.height = height;
		//		pDest.width = width;
		//		pDest.byte_width = width << 2;

		//		g_height = height;
		//		g_width = width;
		//		g_lpGray = new BYTE[g_height*g_width];

		//		int iW = width, iH = height;
		//		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//		//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

		//		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

		//		//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
		//		DECODE_THRESHOLD(pDest, pDest, g_lpGray, m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

		//		//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
		//		delete[] pDest.pdata;
		//		pDest.pdata = NULL;
		//		LoadMark(g_path + _T("\\check\\qz_42.txt"));

		//		FindOrigin2(g_lpGray);
		//		if (height_num > 100 || width_num > 200 || (height_num < 0 && width_num < 0))
		//		{
		//			if (rechecknum > 20)
		//			{
		//				AlertCtrl(1);
		//				AfxMessageBox(_T("�ֵ�ֵû������������"));
		//				rechecknum = 0;
		//				AlertCtrl(0);
		//			}
		//			rechecknum++;
		//			//delete []g_lpGray;
		//			//g_lpGray=NULL;
		//		}
		//		else
		//		{
		//			rechecknum = 0;
		//			//g_status._CHK1=1;//���ֵ��߳�
		//			break;
		//		}
		//	}
		//	g_num = new int[10 * 10 * 8];
		//	GetNumber();

		//	ScanDegreeDynamic2(g_lpGray, g_path + _T("\\check\\positionDynamic.txt"));//���degree[]
		//	g_sort.R1Num = DOUBLE(degree[0] * 100 + degree[1] * 10 + degree[2] + degree[3] * 0.1 + degree[4] * 0.01 + degree[5] * 0.001);

		//	ScanDegree2(g_lpGray, g_path + _T("\\check\\position.txt"));//���degree[]
		//	delete[]g_num;
		//	delete[]g_lpGray;
		//	g_lpGray = NULL;
		//	g_num = NULL;
		//	vga_critical.Unlock();
		//}
		//else
		//{
		//	srand((int)time(0)); //ģ��begin
		//	int deg[30] = { 3,4,0,rand() % 9,rand() % 5,rand() % 9,
		//		2,2 + rand() % 3,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
		//		rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
		//		rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
		//		rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9 };
		//	for (int i = 0; i < 30; i++)
		//		degree[i] = deg[i];//ģ��end
		//}
  //  CalcEquAngle(); // DATE:180421
		//g_sort.sortsn = SortChipR1(&g_sort, degree);//�õ���λֵ
		//if (g_sort.sortsn <= 0)
		//	g_sort.sortsn = g_sort.itemnum + 6;
		RCGVGA();
		g_status._SCREND = 1;//��λ��ǣ����ֵ��߳�
		/*culTime2=CTime::GetCurrentTime();
		if(culTime2.GetDay()!=culTime1.GetDay())
		g_time.end=24;
		else
		g_time.end=0;

		g_time.end=(g_time.end+culTime2.GetHour())*3600+culTime2.GetMinute()*60+culTime2.GetSecond();
		g_time.sum=g_time.end-g_time.start-g_time.pause_sum;
		g_time.avg=double(g_time.sum)/double(g_sort.sortsum+g_sort.sortnum[24]+g_sort.sortnum[25]+g_sort.sortnum[26]+g_sort.sortnum[27]+g_sort.sortnum[28]+g_sort.sortnum[29]);
		*/updatesort = 1;
		g_status._SHOW = 1;
		//==============//����Ƕȵ��ļ�
		//CStdioFile file;
		//file.Open(pathSaveDegree,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
		CString _degree;
		_degree.Format(_T("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n"), g_status._N, degree[0] * 100000 + degree[1] * 10000 + degree[2] * 1000 + degree[3] * 100 + degree[4] * 10 + degree[5],
			degree[6] * 100000 + degree[7] * 10000 + degree[8] * 1000 + degree[9] * 100 + degree[10] * 10 + degree[11],
			degree[12] * 100000 + degree[13] * 10000 + degree[14] * 1000 + degree[15] * 100 + degree[16] * 10 + degree[17],
			degree[18] * 100000 + degree[19] * 10000 + degree[20] * 1000 + degree[21] * 100 + degree[22] * 10 + degree[23],
			degree[24] * 100000 + degree[25] * 10000 + degree[26] * 1000 + degree[27] * 100 + degree[28] * 10 + degree[29],
			g_sort.sortsn);
		//file.Seek(0,CFile::end);
		//file.WriteString( _degree );
		//file.Close();
		//
		CFile myFile;
		myFile.Open(pathSaveDegree, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
		char szANSIString[MAX_PATH];
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, _degree, -1, szANSIString, sizeof(szANSIString), NULL, NULL);
		myFile.Seek(0, CFile::end);
		//myFile.Write(szANSIString,_sort.GetLength()*2-98);
		myFile.Write(szANSIString, CStringA(_degree).GetLength());
		myFile.Close();
		//���̿���¼��by mmy 171115
		if (csv)
		{
			// CCSVFile csv(g_path + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);
			CStringArray arr;
			CString str;
			str.Format(_T("%d"), g_status._N);
			arr.Add(str);
			str.Format(_T("%d"), degree[0] * 100000 + degree[1] * 10000 + degree[2] * 1000 + degree[3] * 100 + degree[4] * 10 + degree[5]);
			arr.Add(str);
			str.Format(_T("%d"), degree[6] * 100000 + degree[7] * 10000 + degree[8] * 1000 + degree[9] * 100 + degree[10] * 10 + degree[11]);
			arr.Add(str);
			str.Format(_T("%d"), degree[12] * 100000 + degree[13] * 10000 + degree[14] * 1000 + degree[15] * 100 + degree[16] * 10 + degree[17]);
			arr.Add(str);
			str.Format(_T("%d"), degree[18] * 100000 + degree[19] * 10000 + degree[20] * 1000 + degree[21] * 100 + degree[22] * 10 + degree[23]);
			arr.Add(str);
			str.Format(_T("%d"), degree[24] * 100000 + degree[25] * 10000 + degree[26] * 1000 + degree[27] * 100 + degree[28] * 10 + degree[29]);
			arr.Add(str);
			str.Format(_T("%d"), g_sort.sortsn);
			arr.Add(str);

			csv->WriteData(arr);

			arr.RemoveAll();
		}
		//��Ƭ����
		/*if(g_sort.sortnum[g_sort.sortsn-1]>60)
		{
		//g_sort.alertnum[g_sort.sortsn-1]++;
		CString tmp;
		tmp.Format(_T("%d��Ƭ̫����"),g_sort.sortsn);
		int status=0;
		AlertCtrl(1);
		status=AfxMessageBox(tmp);
		g_sort.sortnum[g_sort.sortsn-1]=0;
		AlertCtrl(0);
		}*/
    if (g_pCpk)
    {
      //int* degree = degree;
      CProcessData processData;
      processData.equal = (degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]);
      processData.laser = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]);
      processData.laser0 = (degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]);
      processData.no = g_status._N;
      processData.phi = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]);
      processData.phi0 = (degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29]);
      processData.sort = g_sort.sortsn;

      g_pCpk->AddToProcessCardCsv(processData);

      // cpkʵʱ���
      CProcessCard processCard;

      if (processData.no > g_pCpk->m_cpkSet.m_start_num)
      {
        g_pCpk->CalcCpk(processCard);

        CString csCpk = _T(""), tmp;
        tmp.Format(_T("cp %.3f "), processCard.cp);
        csCpk += tmp;
        tmp.Format(_T("ca %.3f "), processCard.ca);
        csCpk += tmp;
        tmp.Format(_T("cpk %.3f "), processCard.cpk);
        csCpk += tmp;
        tmp.Format(_T("num %d "), processCard.num);
        csCpk += tmp;
        if (processCard.cp < g_pCpk->m_cpkSet.m_min_cp || processCard.ca > g_pCpk->m_cpkSet.m_max_ca || processCard.cpk < g_pCpk->m_cpkSet.m_min_cpk)
        {
          csCpk += _T("����");
          ((CATtoSCDlg*)(AfxGetApp()->m_pMainWnd))->m_Run.OnBnClickedRunPause();
          //g_dlgRun->OnBnClickedBtnPause();
        }
        ((CATtoSCDlg*)(AfxGetApp()->m_pMainWnd))->m_Run.SetDlgItemText(IDC_RUN_SHOW, csCpk);
      }
    }

	}
	return 0;
}
UINT DisplayThread(LPVOID pParam)//��ʾ
{
	int nCnt = 0;
	while (1)
	{
		if (1 == g_status.DSP_STOP)
		{
			g_status.DSP_STOP = 0;
			return 0;
		}
		if (nCnt > 50)
		{
			::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_DISPLAY, 0, NULL);//������ʾ��Ϣ��������
			nCnt = 0;
		}
		if (ShakeDown)//�����ж��Ƿ�����������ȥ
			nShkStat = 0;
		Sleep(1);
		nCnt++;
	}
	return 0;
}
//�Զ��庯��
int nAlert = 0;
void AlertCtrl(bool stat)
{

	__int32 ioData;
	if (stat)
	{
		if (nAlert++ > 0)
		{
			return;
		}
		ioData = AC6641_DI(g_hDevice, 5) | 0x10;// 
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (nAlert > 0)
	{
		if (--nAlert > 0)
		{
			return;
		}
		ioData = AC6641_DI(g_hDevice, 5) & 0xef;// 
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
}
void MotorCtrl(int n, bool zf, int steps, bool send)//����n�ŵ��������zf��תsteps��
{
	int tmp = steps;
	int ioData = tmp % 256;
	if (n == 1)//x��
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 0, ioData);
		ioData = tmp / 256,
			AC6641_DO(g_hDevice, 1, ioData);
		ioData = AC6641_DI(g_hDevice, 5) & 0xfb;//52�͵�ƽ��ȫ����
		AC6641_DO(g_hDevice, 5, ioData);
		ioData = AC6641_DI(g_hDevice, 4) | 0x02;//��p41�øߣ�Ϊ�½�����׼����
		tmp = zf;//�������ת
		0 == tmp ? (ioData = ioData & 0xfe) : (ioData = ioData | 0x01);
		AC6641_DO(g_hDevice, 4, ioData);//����ת
		Sleep(1);
		ioData = ioData & 0xfd;
		AC6641_DO(g_hDevice, 4, ioData);//�½���
		Sleep(10);
		ioData = ioData | 0x02;
		AC6641_DO(g_hDevice, 4, ioData);//�ظ�

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x01);//���ߵ�ƽʱ
		//	Sleep(500);
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x01))
				break;
			Sleep(1);
		}
		ac6641_critical.Lock();
		ioData = AC6641_DI(g_hDevice, 5) | 0x04;//52�ߵ�ƽ�������
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (n == 2)//y��
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 2, ioData);
		ioData = tmp / 256,
			AC6641_DO(g_hDevice, 3, ioData);
		ioData = AC6641_DI(g_hDevice, 5) & 0xf7;//53�͵�ƽ��ȫ����
		AC6641_DO(g_hDevice, 5, ioData);
		ioData = AC6641_DI(g_hDevice, 4) | 0x20;//��p45�øߣ�Ϊ�½�����׼����
		tmp = zf;//�������ת
		0 == tmp ? (ioData = ioData & 0xef) : (ioData = ioData | 0x10);
		if (0 == tmp)
			errorStep = 0;
		else
		{
			errorStep++;
			if (errorStep > 2)
				AfxMessageBox(_T("error"));
		}
		AC6641_DO(g_hDevice, 4, ioData);//����ת
		Sleep(1);
		ioData = ioData & 0xdf;
		AC6641_DO(g_hDevice, 4, ioData);//�½���
		Sleep(10);
		ioData = ioData | 0x20;
		AC6641_DO(g_hDevice, 4, ioData);//�ظ�

		ac6641_critical.Unlock();
		if (send)
			g_status._SORTTAK = 1;//��ȡƬ�߳�
		//while(AC6641_DI(g_hDevice,6) & 0x10);//���ߵ�ƽʱ
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x10))
				break;
			Sleep(1);
		}
		ac6641_critical.Lock();
		ioData = AC6641_DI(g_hDevice, 5) | 0x08;//53�ߵ�ƽ�������
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (n == 3)//z��
	{

		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 8, ioData);
		ioData = tmp / 256,
			AC6641_DO(g_hDevice, 9, ioData);
		ioData = AC6641_DI(g_hDevice, 10) | 0x02;//��p41�øߣ�Ϊ�½�����׼����
		tmp = zf;//�������ת
		0 == tmp ? (ioData = ioData & 0xfe) : (ioData = ioData | 0x01);
		AC6641_DO(g_hDevice, 10, ioData);//����ת
		Sleep(10);
		ioData = ioData & 0xfd;
		AC6641_DO(g_hDevice, 10, ioData);//�½���
		Sleep(10);
		ioData = ioData | 0x02;
		AC6641_DO(g_hDevice, 10, ioData);//�ظ�

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x01);//���ߵ�ƽʱ
		//	Sleep(500);
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 11) & 0x01))
				break;
			Sleep(1);
		}
	}
}
void MotorCtrl(int n, bool zf, int steps)//����n�ŵ��������zf��תsteps��
{
	int tmp = steps;
	int ioData = tmp % 256;
	if (n == 1)//x��
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 0, ioData);
		ioData = tmp / 256;
		AC6641_DO(g_hDevice, 1, ioData);
		ioData = AC6641_DI(g_hDevice, 5) & 0xfb;//52�͵�ƽ��ȫ����
		AC6641_DO(g_hDevice, 5, ioData);
		ioData = AC6641_DI(g_hDevice, 4) | 0x02;//��p41�øߣ�Ϊ�½�����׼����
		tmp = zf;//�������ת
		0 == tmp ? (ioData = ioData & 0xfe) : (ioData = ioData | 0x01);
		AC6641_DO(g_hDevice, 4, ioData);//����ת
		Sleep(1);
		ioData = ioData & 0xfd;
		AC6641_DO(g_hDevice, 4, ioData);//�½���
		Sleep(5);//_0610
		ioData = ioData | 0x02;
		AC6641_DO(g_hDevice, 4, ioData);//�ظ�

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x01);//���ߵ�ƽʱ
		//	Sleep(500);
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x01))
				break;
			Sleep(1);
		}
		ac6641_critical.Lock();
		ioData = AC6641_DI(g_hDevice, 5) | 0x04;//52�ߵ�ƽ�������
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (n == 2)//y��
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 2, ioData);
		ioData = tmp / 256;
		AC6641_DO(g_hDevice, 3, ioData);
		ioData = AC6641_DI(g_hDevice, 5) & 0xf7;//53�͵�ƽ��ȫ����
		AC6641_DO(g_hDevice, 5, ioData);
		ioData = AC6641_DI(g_hDevice, 4) | 0x20;//��p45�øߣ�Ϊ�½�����׼����
		tmp = zf;//�������ת
		0 == tmp ? (ioData = ioData & 0xef) : (ioData = ioData | 0x10);
		if (0 == tmp)
			errorStep = 0;
		else
		{
			errorStep++;
			if (errorStep > 2)
				AfxMessageBox(_T("error"));
		}
		AC6641_DO(g_hDevice, 4, ioData);//����ת
		Sleep(1);
		ioData = ioData & 0xdf;
		AC6641_DO(g_hDevice, 4, ioData);//�½���
		Sleep(5);//_0610
		ioData = ioData | 0x20;
		AC6641_DO(g_hDevice, 4, ioData);//�ظ�

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x10);//���ߵ�ƽʱ
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x10))
				break;
			Sleep(1);
		}
		ac6641_critical.Lock();
		ioData = AC6641_DI(g_hDevice, 5) | 0x08;//53�ߵ�ƽ�������
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (n == 3)//z��
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 8, ioData);
		ioData = tmp / 256;
		AC6641_DO(g_hDevice, 9, ioData);
		ioData = AC6641_DI(g_hDevice, 10) | 0x02;//��p41�øߣ�Ϊ�½�����׼����
		tmp = zf;//�������ת
		0 == tmp ? (ioData = ioData & 0xfe) : (ioData = ioData | 0x01);
		AC6641_DO(g_hDevice, 10, ioData);//����ת
		Sleep(1);
		ioData = ioData & 0xfd;
		AC6641_DO(g_hDevice, 10, ioData);//�½���
		Sleep(10);
		ioData = ioData | 0x02;
		AC6641_DO(g_hDevice, 10, ioData);//�ظ�

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x01);//���ߵ�ƽʱ
		//	Sleep(500);
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 11) & 0x01))
				break;
			Sleep(1);
		}
	}
}
void CylinderCtrl(bool stat)//ˮƽ����
{
	ac6641_critical.Lock();
	int ioData = AC6641_DI(g_hDevice, 10);

	if (1 == stat)
		ioData = ioData | 0x04;
	else if (0 == stat)
		ioData = ioData & 0xfb;
	AC6641_DO(g_hDevice, 10, ioData);
	ac6641_critical.Unlock();
	g_status.CyStat = -1;
	while (1)//for(int i=0;i<1000;i++)
	{
		if (0 == stat)
		{
			if ((AC6641_DI(g_hDevice, 11) & 0x02))
				break;
		}
		else if (1 == stat)
		{
			if ((AC6641_DI(g_hDevice, 11) & 0x04))
				break;
		}
		Sleep(1);
	}
	Sleep(200);
	g_status.CyStat = stat;
}

void CylinderCtrlDbg(bool stat)
{
	ac6641_critical.Lock();
	int ioData = AC6641_DI(g_hDevice, 10);

	if (1 == stat)
		ioData = ioData | 0x04;
	else if (0 == stat)
		ioData = ioData & 0xfb;
	AC6641_DO(g_hDevice, 10, ioData);
	ac6641_critical.Unlock();
	g_status.CyStat = -1;
	for (int i = 0; i < 1000; i++)
	{
		if (0 == stat)
		{
			if ((AC6641_DI(g_hDevice, 11) & 0x02))
				break;
		}
		else if (1 == stat)
		{
			if ((AC6641_DI(g_hDevice, 11) & 0x04))
				break;
		}
		Sleep(1);
	}
	Sleep(200);
	g_status.CyStat = stat;
}

void TubeCtrl(int n, bool stat)//����n�Ż�е�۵����쿪�أ�stat��
{
	ac6641_critical.Lock();
	int ioData = 0;
	if (n == 1)//x��
	{
		ioData = AC6641_DI(g_hDevice, 4);
		if (1 == stat)
			ioData = ioData | 0x08;
		else if (0 == stat)
			ioData = ioData & 0xf7;
		AC6641_DO(g_hDevice, 4, ioData);
	}
	else if (n == 2)//y��
	{
		ioData = AC6641_DI(g_hDevice, 4);
		if (1 == stat)
			ioData = ioData | 0x80;
		else if (0 == stat)
			ioData = ioData & 0x7f;
		AC6641_DO(g_hDevice, 4, ioData);
	}
	else if (n == 3)//z��
	{
		ioData = AC6641_DI(g_hDevice, 10);
		if (1 == stat)
			ioData = ioData | 0x08;
		else if (0 == stat)
			ioData = ioData & 0xf7;
		AC6641_DO(g_hDevice, 10, ioData);
	}

	ac6641_critical.Unlock();
}
void SortBlock(int n)//�ֵ�
{
	n--;//��Ϊsortsn�Ǵ�1��30�����ⲿ��ʾ��ͬ�����������Ǵ�0��29������Ҫ--������-1
	MotorCtrl(2, 1, g_status.Ysort[n] - g_status.Ywait);//ת������λ
	Sleep(200);
	TubeCtrl(2, 0);//����
	Sleep(g_tim.offY);
	g_status._N++;
	MotorCtrl(2, 0, g_status.Ysort[n] - g_status.Ywait / 4);//�ƶ����ȴ�λ��
}
void SortBlockWithZ(int n)//�ֵ�
{
	n--;//��Ϊsortsn�Ǵ�1��30�����ⲿ��ʾ��ͬ�����������Ǵ�0��29������Ҫ--������-1
	if (n<0 || n>g_sort.itemnum + 6 - 1)
	{
		n = g_sort.itemnum + 6 - 1;
	}
	MotorCtrl(3, 1, g_status.Zsort[n]);//-g_status.Ywait);//ת������λ
	Sleep(50);
	TubeCtrl(3, 0);//����
	Sleep(g_tim.offZ);
	g_status._N++;
	ReturnZero(3);
	Sleep(100);
	//MotorCtrl(3,0,g_status.Zsort[n]-g_status.Ywait/4);//�ƶ����ȴ�λ��
}
void VibrateBlock()//������
{
}

int ShakePlatformUp()//������
{
	return g_shakeE ? !(AC6641_DI(g_hDevice, 6) & 0x04) : (AC6641_DI(g_hDevice, 6) & 0x04);
}


int ShakePlatformDown()//������
{
	return g_shakeE ? (AC6641_DI(g_hDevice, 6) & 0x04) : !(AC6641_DI(g_hDevice, 6) & 0x04);
}


void ReturnZero(int n)//����1��2�Ż�е�ۻ���λ
{
	if (n == 1)//x��
		MotorCtrl(n, 0, 10000);//ȡƬ�ȴ�
	else if (n == 2)//y��
		MotorCtrl(n, 0, 10000);//ȡƬ�ȴ�
	else if (n == 3)//z��
		MotorCtrl(n, 0, 10000);//ȡƬ�ȴ�
}
void BlowBlock(UINT time)//����
{
	ac6641_critical.Lock();
	int ioData = AC6641_DI(g_hDevice, 5) | 0x01;

	AC6641_DO(g_hDevice, 5, ioData);//����
	Sleep(time);
	ioData = AC6641_DI(g_hDevice, 5) & 0xfe;
	AC6641_DO(g_hDevice, 5, ioData);//�رմ���
	ac6641_critical.Unlock();
}
void StartFT()//efg��ʼ����ź�
{
	ac6641_critical.Lock();
	int ioData = AC6641_DI(g_hDevice, 5) | 0x02;

	AC6641_DO(g_hDevice, 5, ioData);
	Sleep(500);
	ioData = AC6641_DI(g_hDevice, 5) & 0xfd;
	AC6641_DO(g_hDevice, 5, ioData);
	ac6641_critical.Unlock();
}

void StartFTLong(bool se)//efg��ʼ����ź�
{
	ac6641_critical.Lock();

	int ioData;
	if (se)
	{
		ioData = AC6641_DI(g_hDevice, 5) | 0x02;
		AC6641_DO(g_hDevice, 5, ioData);
	}
	else
	{

		ioData = AC6641_DI(g_hDevice, 5) & 0xfd;
		AC6641_DO(g_hDevice, 5, ioData);
	}

	ac6641_critical.Unlock();
}
//�ֵ��������ʹ�õ�ȫ�ֱ���
//���ص�λֵ������е�ֵַ�
int SortChip(sortchip* s_sort, int *degree)
{
	bool elecyes = 1;//�޶���ͨ����־
	int sortSN = 0;//����ֵ���ֵ�ֵ

	//s_sort->nocheckout=0;
	int angle[5] = { (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]),//����
		(degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]),//����
		(degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]),//��Ч��
		(degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]),//ԭʼ����
		(degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29]) };//ԭʼ���᣻����ⶨ���ĽǶȣ�ת������
	if (degree[0] == -1 || angle[3] == 0)//�Ƕ�û���
	{
		sortSN = 30;
		s_sort->sortnum[29]++;
		s_sort->nocheckout++;
		return sortSN;
	}
	if (0 == angle[0] && angle[3] != 0)//�����ⲻ��
	{
		sortSN = 29;
		s_sort->sortnum[28]++;
		s_sort->checkout0++;
		return sortSN;
	}
	//s_sort->checkout0=0;
	if (s_sort->sortsel2 != 3)//��������޶�
	{
		switch ((angle[s_sort->sortsel2] <= s_sort->elechigh ? -1 : 0) + (angle[s_sort->sortsel2] >= s_sort->eleclow ? 1 : 0))//�����޶��Ტ�ֵ�
		{
		case 0:break;
		case 1:elecyes = 0; s_sort->sortnum[25]++; sortSN = 26; break;
		case -1:elecyes = 0; s_sort->sortnum[24]++; sortSN = 25; break;
		}
	}
	if (1 == elecyes)//����޶���ͨ��
	{
		if (angle[s_sort->sortsel1] >= (s_sort->centerangle - s_sort->sortvalue / 2))//�����������Ԥ��ֵ
		{
			switch ((angle[s_sort->sortsel1] - s_sort->centerangle + s_sort->sortvalue / 2) / s_sort->sortvalue + 1)//�������Ტ�ֵ�����������Ԥ��ֵ
			{
			case 1:s_sort->sortnum[12]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 13; break;
			case 2:s_sort->sortnum[13]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 14; break;
			case 3:s_sort->sortnum[14]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 15; break;
			case 4:s_sort->sortnum[15]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 16; break;
			case 5:s_sort->sortnum[16]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 17; break;
			case 6:s_sort->sortnum[17]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 18; break;
			case 7:s_sort->sortnum[18]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 19; break;
			case 8:s_sort->sortnum[19]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 20; break;
			case 9:s_sort->sortnum[20]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 21; break;
			case 10:s_sort->sortnum[21]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 22; break;
			case 11:s_sort->sortnum[22]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 23; break;
			case 12:s_sort->sortnum[23]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 24; break;
			default:s_sort->sortnum[27]++; sortSN = 28; break;
			}
		}
		else
		{
			switch ((s_sort->centerangle - s_sort->sortvalue / 2 - angle[s_sort->sortsel1] - 1) / s_sort->sortvalue + 1)//�������Ტ�ֵ��������С��Ԥ��ֵ
			{
			case 1:s_sort->sortnum[11]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 12; break;
			case 2:s_sort->sortnum[10]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 11; break;
			case 3:s_sort->sortnum[9]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 10; break;
			case 4:s_sort->sortnum[8]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 9; break;
			case 5:s_sort->sortnum[7]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 8; break;
			case 6:s_sort->sortnum[6]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 7; break;
			case 7:s_sort->sortnum[5]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 6; break;
			case 8:s_sort->sortnum[4]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 5; break;
			case 9:s_sort->sortnum[3]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 4; break;
			case 10:s_sort->sortnum[2]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 3; break;
			case 11:s_sort->sortnum[1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 2; break;
			case 12:s_sort->sortnum[0]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); sortSN = 1; break;
			default:s_sort->sortnum[26]++; sortSN = 27; break;
			}
		}

	}

	return sortSN;
}

#define SORT_SEC(i) g_sort.centerangle + g_sort.sortvalue*((i) - sort_num / 2)
void JudegSortWhich(double sec, int &which)
{
  int sort_num = g_sort.itemnum;//��λ����

  //int *sort_sec = m_resultParam.degree.sort_sec;//����λ�м�ֵ
  int step_sec = g_sort.sortvalue;

  double d_sec = double(step_sec) / 2;
  if (sec < SORT_SEC(0) - d_sec)
  {
    which = -1;
    return;
  }

  int i;
  for (i = 0; i < sort_num; i++)
  {
    if (sec < SORT_SEC(i) + d_sec)
      break;
  }

  which = i;

}

//�ֵ��������ʹ�õ�ȫ�ֱ���
//���ص�λֵ������е�ֵַ�
//itemnum���
int SortChipR1(sortchip* s_sort, int *degree)//_0614
{
	bool elecyes = 1;//�޶���ͨ����־
	int sortSN = 0;//����ֵ���ֵ�ֵ
	int sel1SN = 5, sel2SN = 5;

	switch (s_sort->sortsel2)
	{
	case 0://�����޶�
		sel2SN = 3;
		break;
	case 1://�����޶�
		sel2SN = 1;
		break;
	case 2://��Ч��
		sel2SN = 3;
		switch (s_sort->sortsel1)
		{
		case 0://�����޶�
			sel2SN = 1;
			break;
		case 1://�����޶�
			sel2SN = 3;
			break;
		}
		break;
	default:
		break;
	}

	switch (s_sort->sortsel1)
	{
	case 0://�����޶�
		sel1SN = 3;
		break;
	case 1://�����޶�
		sel1SN = 1;
		break;
	case 2:
		switch (sel2SN)
		{
		case 1:
			sel1SN = 3;
			break;
		case 3:
			sel1SN = 1;
			break;
		default:
			sel1SN = 3;
			break;
		}

		break;
	default:
		break;
	}
	//s_sort->nocheckout=0;
	int angle[5] = { (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]),//����
		(degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]),//����
		(degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]),//��Ч��
		(degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]),//ԭʼ����
		(degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29]) };//ԭʼ���᣻����ⶨ���ĽǶȣ�ת������
	if (degree[0] == -1 || angle[3] == 0)//�Ƕ�û���
	{
		sortSN = g_sort.itemnum + 6;
		s_sort->sortnum[sortSN - 1]++;
		s_sort->nocheckout++;
		return sortSN;
	}
	if (0 == angle[0] && angle[3] != 0)//�����ⲻ��
	{
		sortSN = g_sort.itemnum + 5;
		s_sort->sortnum[sortSN - 1]++;
		s_sort->checkout0++;
		return sortSN;
	}
	angle[0] = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]) + g_sort.R1t[g_sort.R1Num];//����//_0614
	angle[1] = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]) + g_sort.R1e[g_sort.R1Num];//����
	s_sort->nocheckout = 0;
	s_sort->checkout0 = 0;
	if (s_sort->sortsel2 != 3)//������޶�
	{
		switch ((angle[s_sort->sortsel2] <= s_sort->elechigh ? -1 : 0) + (angle[s_sort->sortsel2] >= s_sort->eleclow ? 1 : 0))//�����޶��Ტ�ֵ�
		{
		case 0:break;
		case 1:elecyes = 0; sortSN = g_sort.itemnum + sel2SN + 1; s_sort->sortnum[sortSN - 1]++; break;
		case -1:elecyes = 0; sortSN = g_sort.itemnum + sel2SN; s_sort->sortnum[sortSN - 1]++; break;
		}
	}
	if (1 == elecyes)//����޶���ͨ��
	{


    JudegSortWhich(angle[s_sort->sortsel1], sortSN);
    if (sortSN == -1)
    {
      sortSN = g_sort.itemnum + sel1SN; s_sort->sortnum[sortSN - 1]++;
    }
    else if (sortSN == g_sort.itemnum)
    {
      sortSN = g_sort.itemnum + sel1SN + 1; s_sort->sortnum[sortSN - 1]++;
    }
    else
    {
      sortSN++;
      s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2);
    }


		//if (g_sort.islongsqu)
		//{
		//	if (angle[s_sort->sortsel1] >= (s_sort->centerangle - s_sort->sortvalue / 2))//�����������Ԥ��ֵ
		//	{
		//		switch ((angle[s_sort->sortsel1] - s_sort->centerangle + s_sort->sortvalue / 2) / s_sort->sortvalue + 1)//�������Ტ�ֵ�����������Ԥ��ֵ
		//		{
		//		case 1:sortSN = 10; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 2:sortSN = 11; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 3:sortSN = 12; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 4:sortSN = 13; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 5:sortSN = 14; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 6:sortSN = 15; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 7:sortSN = 16; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 8:sortSN = 17; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 9:sortSN = 18; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		default:sortSN = g_sort.itemnum + sel1SN + 1; s_sort->sortnum[sortSN - 1]++; break;
		//		}
		//	}
		//	else
		//	{
		//		switch ((s_sort->centerangle - s_sort->sortvalue / 2 - angle[s_sort->sortsel1] - 1) / s_sort->sortvalue + 1)//�������Ტ�ֵ��������С��Ԥ��ֵ
		//		{
		//		case 1:sortSN = 9; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 2:sortSN = 8; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 3:sortSN = 7; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 4:sortSN = 6; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 5:sortSN = 5; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 6:sortSN = 4; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 7:sortSN = 3; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 8:sortSN = 2; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 9:sortSN = 1; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		default:sortSN = g_sort.itemnum + sel1SN; s_sort->sortnum[sortSN - 1]++; break;
		//		}
		//	}
		//}
		//else
		//{
		//	if (angle[s_sort->sortsel1] >= (s_sort->centerangle - s_sort->sortvalue / 2))//�����������Ԥ��ֵ
		//	{
		//		switch ((angle[s_sort->sortsel1] - s_sort->centerangle + s_sort->sortvalue / 2) / s_sort->sortvalue + 1)//�������Ტ�ֵ�����������Ԥ��ֵ
		//		{
		//		case 1:sortSN = 13; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 2:sortSN = 14; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 3:sortSN = 15; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 4:sortSN = 16; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 5:sortSN = 17; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 6:sortSN = 18; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 7:sortSN = 19; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 8:sortSN = 20; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 9:sortSN = 21; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 10:sortSN = 22; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 11:sortSN = 23; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 12:sortSN = 24; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		default:sortSN = g_sort.itemnum + sel1SN + 1; s_sort->sortnum[sortSN - 1]++; break;
		//		}
		//	}
		//	else
		//	{
		//		switch ((s_sort->centerangle - s_sort->sortvalue / 2 - angle[s_sort->sortsel1] - 1) / s_sort->sortvalue + 1)//�������Ტ�ֵ��������С��Ԥ��ֵ
		//		{
		//		case 1:sortSN = 12; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 2:sortSN = 11; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 3:sortSN = 10; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 4:sortSN = 9; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 5:sortSN = 8; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 6:sortSN = 7; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 7:sortSN = 6; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 8:sortSN = 5; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 9:sortSN = 4; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 10:sortSN = 3; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 11:sortSN = 2; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		case 12:sortSN = 1; s_sort->sortnum[sortSN - 1]++; s_sort->sortsum++; SortAvgStd(s_sort->sortsum, angle, s_sort->sortavg, s_sort->sortstd, s_sort->std2); break;
		//		default:sortSN = g_sort.itemnum + sel1SN; s_sort->sortnum[sortSN - 1]++; break;
		//		}
		//	}
		//}


	}

	return sortSN;
}
//�������ƽ��ֵ����׼����
void SortAvgStd(unsigned int sum, int *angle, double *avg, double *std, long double *std2)
{
	double n = sum;
	for (int i = 0; i < 5; i++)
	{
		double xi = angle[i];
		//double avg;
		if (1 == n)
		{
			avg[i] = xi;//ƽ��ֵ
			std2[i] = xi*xi;//��׼ƫ���м�ֵ
		}
		if (n > 1)
		{
			avg[i] = (avg[i] + xi / (n - 1)) / (n / (n - 1));//��ƽ��ֵ
			std2[i] = (std2[i] + xi*xi / (n - 1)) / (n / (n - 1));//���׼���м�ֵ
			std[i] = sqrt(fabs(std2[i] - avg[i] * avg[i])*n / (n - 1));//���׼��
		}
	}
}
// CRun �Ի���

IMPLEMENT_DYNAMIC(CRun, CDialogEx)

	CRun::CRun(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRun::IDD, pParent)
	//, i_edit_bs(0)
{
	m_csvCard = NULL;
	m_csvAllCard = NULL;
}

CRun::~CRun()
{
	font.DeleteObject();
}

void CRun::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RUN_LIST, m_runlist);
	//  DDX_Control(pDX, IDC_SORT_SEL1, m_SortSel1);
	DDX_Control(pDX, IDC_SORT_SEL1, m_sortsel1);
	DDX_Control(pDX, IDC_SORT_SEL2, m_sortsel2);
	//  DDX_Text(pDX, IDC_EDIT_bs, i_edit_bs);
}


BEGIN_MESSAGE_MAP(CRun, CDialogEx)
	ON_BN_CLICKED(IDC_RUN_START, &CRun::OnBnClickedRunStart)
	ON_BN_CLICKED(IDC_RUN_STOP, &CRun::OnBnClickedRunStop)
	ON_BN_CLICKED(IDC_SORT_SET, &CRun::OnBnClickedSortSet)
	ON_CBN_SELCHANGE(IDC_SORT_SEL1, &CRun::OnCbnSelchangeSortSel1)
	ON_CBN_SELCHANGE(IDC_SORT_SEL2, &CRun::OnCbnSelchangeSortSel2)
	ON_BN_CLICKED(IDC_RUN_PAUSE, &CRun::OnBnClickedRunPause)
	ON_BN_CLICKED(IDC_START_FT, &CRun::OnBnClickedStartFt)
	//	ON_EN_CHANGE(IDC_EDIT_bs, &CRun::OnEnChangeEditbs)
	ON_BN_CLICKED(IDC_BTN_STANDAND, &CRun::OnBnClickedBtnStandand)
	ON_EN_CHANGE(IDC_EDIT_TIME_X, &CRun::OnEnChangeEditTimeX)
	ON_BN_CLICKED(IDC_CHK_LONG_SQU, &CRun::OnBnClickedChkLongSqu)
	ON_BN_CLICKED(IDC_END_FT, &CRun::OnBnClickedEndFt)
	ON_EN_CHANGE(IDC_EDIT_TIME_ZF, &CRun::OnEnChangeEditTimeZf)
	ON_EN_CHANGE(IDC_EDIT_TIME_XF, &CRun::OnEnChangeEditTimeXf)
	ON_EN_CHANGE(IDC_EDIT_TIME_Y, &CRun::OnEnChangeEditTimeY)
	ON_EN_CHANGE(IDC_EDIT_TIME_YF, &CRun::OnEnChangeEditTimeYf)
	ON_EN_CHANGE(IDC_EDIT_TIME_BLOW, &CRun::OnEnChangeEditTimeBlow)
	ON_EN_CHANGE(IDC_EDIT_TIME_Z, &CRun::OnEnChangeEditTimeZ)
	ON_BN_CLICKED(IDC_BTN_PRINT, &CRun::OnBnClickedBtnPrint)
	ON_BN_CLICKED(IDC_BTN_STDCHECK, &CRun::OnBnClickedBtnStdcheck)
  ON_BN_CLICKED(IDC_BTN_CPK, &CRun::OnBnClickedBtnCpk)
END_MESSAGE_MAP()


// CRun ��Ϣ�������

void CRun::RunListReset()
{
  CString strValue, strTemps;
  int itemnum = g_sort.itemnum;
  //��տؼ���ȫ�ֱ���
  m_runlist.DeleteAllItems();

  for (int i = 0; i < itemnum; i++)
  {
    strValue = "";
    strTemps.Format(_T("%d"), i + 1);//_0611_ԭi+1
    m_runlist.InsertItem(i, strTemps);
    strTemps.Format(_T("%02d��"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) / 3600);//��
    strValue += strTemps;
    strTemps.Format(_T("%02d\'"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 3600 / 60);//��
    strValue += strTemps;
    strTemps.Format(_T("%02d\""), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 60);//��
    strValue += strTemps;
    m_runlist.SetItemText(i, 1, strValue);
    m_runlist.SetItemText(i, 2, _T("0"));
  }

  strTemps.Format(_T("%d"), itemnum + 1); m_runlist.InsertItem(itemnum, strTemps); m_runlist.SetItemText(itemnum, 1, _T("����-")); m_runlist.SetItemText(itemnum, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 2); m_runlist.InsertItem(itemnum + 1, strTemps); m_runlist.SetItemText(itemnum + 1, 1, _T("����+")); m_runlist.SetItemText(itemnum + 1, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 3); m_runlist.InsertItem(itemnum + 2, strTemps); m_runlist.SetItemText(itemnum + 2, 1, _T("����-")); m_runlist.SetItemText(itemnum + 2, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 4); m_runlist.InsertItem(itemnum + 3, strTemps); m_runlist.SetItemText(itemnum + 3, 1, _T("����+")); m_runlist.SetItemText(itemnum + 3, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 5); m_runlist.InsertItem(itemnum + 4, strTemps); m_runlist.SetItemText(itemnum + 4, 1, _T("�����ⲻ��")); m_runlist.SetItemText(itemnum + 4, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 6); m_runlist.InsertItem(itemnum + 5, strTemps); m_runlist.SetItemText(itemnum + 5, 1, _T("�Ƕȼ�ⲻ��")); m_runlist.SetItemText(itemnum + 5, 2, _T("0"));
  m_runlist.InsertItem(itemnum + 6, _T("�ϼ�")); m_runlist.SetItemText(itemnum + 6, 1, _T("")); m_runlist.SetItemText(itemnum + 6, 2, _T("0"));

  memset(g_sort.sortnum, 0, sizeof(g_sort.sortnum));
  for (int i = 0; i < 5; i++)
  {
    g_sort.sortavg[i] = 0;
    g_sort.sortstd[i] = 0;
  }
  g_sort.sortsn = 0;
  g_sort.sortsum = 0;
}

BOOL CRun::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CButton*)GetDlgItem(IDC_CHK_LONG_SQU))->SetCheck(g_sort.islongsqu);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strValue, strTemps;
	//������ʼ��
	g_status._N = 0;
	g_status._RUN = 0;
	g_status._SHOW = 0;
	g_status.CHK_STOP = 0;
	g_status.DSP_STOP = 0;
	g_status.SCR_STOP = 0;
	g_status.SOT_STOP = 0;
	g_status.SYS_PAUSE = 0;
	g_status.TAK_STOP = 0;
	g_status.SOT2_STOP = 0;
	g_status.CY_STOP = 0;
	g_status._DEBUGEFG = 0;//����
	g_sort.sortsn = 0;
	g_sort.sortsum = 0;//1~24����Ƭ��
	for (int i = 0; i < 30; i++)//ÿ���ѷֵ�Ƭ������
	{
		g_sort.sortnum[i] = 0;
		g_sort.alertnum[i] = 1;
	}
	for (int i = 0; i < 5; i++)//5���ƽ�������ʼ��
	{
		g_sort.sortavg[i] = 0;
		g_sort.sortstd[i] = 0;
		g_sort.std2[i] = 0;
	}
	//ini�ļ���ʼ�����ȡ
	CString strFilePath, txtFilePath;
	int iniint = 0;
	GetModuleFileName(NULL, strFilePath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strFilePath.ReleaseBuffer();
	int pos = strFilePath.ReverseFind('\\');
	txtFilePath = strFilePath.Left(pos) + _T("\\R1Cor.txt");
	strFilePath = strFilePath.Left(pos) + _T("\\SortSet.ini");

	///////////////
	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(strFilePath);
	if (!ifFind)
	{

		AfxMessageBox(_T("SortSet.ini ������"));
		exit(0);
		/*
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

	//д��ini�ļ�����Ӧ�ֶ�
	/*GetPrivateProfileString(_T("�ֵ��趨"), _T("���̿�"), 0, strValue.GetBuffer(MAX_PATH), MAX_PATH, strFilePath);
	strValue.ReleaseBuffer();
	SetDlgItemText(IDC_EDIT_CARD, strValue);*/
	iniint = GetPrivateProfileInt(_T("�ֵ��趨"), _T("���ĽǶȣ���"), 0, strFilePath) * 10000 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("���ĽǶȣ���"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("���ĽǶȣ���"), 0, strFilePath);
	SetDlgItemInt(IDC_DEG, iniint);

	iniint = GetPrivateProfileInt(_T("�ֵ��趨"), _T("�ֵ�ֵ����"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("�ֵ�ֵ����"), 0, strFilePath);
	SetDlgItemInt(IDC_SORT, iniint);

	iniint = GetPrivateProfileInt(_T("�ֵ��趨"), _T("�н�ֵ����"), 0, strFilePath);
	SetDlgItemInt(IDC_EDT_CUTDEG, iniint);

	iniint = GetPrivateProfileInt(_T("�ֵ��趨"), _T("��ʼֵ����"), 0, strFilePath) * 10000 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("��ʼֵ����"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("��ʼֵ����"), 0, strFilePath);
	SetDlgItemInt(IDC_DEG_MIN, iniint);
	iniint = GetPrivateProfileInt(_T("�ֵ��趨"), _T("����ֵ����"), 0, strFilePath) * 10000 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("����ֵ����"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("����ֵ����"), 0, strFilePath);
	SetDlgItemInt(IDC_DEG_MAX, iniint);

	iniint = GetPrivateProfileInt(_T("��Ч���趨"), _T("T0����"), 0, strFilePath) * 10000 +
		GetPrivateProfileInt(_T("��Ч���趨"), _T("T0����"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("��Ч���趨"), _T("T0����"), 0, strFilePath);
	SetDlgItemInt(IDC_PARAM_T0, iniint);

	iniint = GetPrivateProfileInt(_T("��Ч���趨"), _T("K"), 0, strFilePath);
	SetDlgItemInt(IDC_PARAM_K, iniint);

	//Ԥ��ֵ���浽ȫ�ֱ���
	g_sort.centerangle = GetPrivateProfileInt(_T("�ֵ��趨"), _T("���ĽǶȣ���"), 0, strFilePath) * 3600 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("���ĽǶȣ���"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("���ĽǶȣ���"), 0, strFilePath);
	g_sort.sortvalue = GetPrivateProfileInt(_T("�ֵ��趨"), _T("�ֵ�ֵ����"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("�ֵ�ֵ����"), 0, strFilePath);
	g_sort.cutvalue = GetPrivateProfileInt(_T("�ֵ��趨"), _T("�н�ֵ����"), 0, strFilePath);
	
	g_sort.eleclow = GetPrivateProfileInt(_T("�ֵ��趨"), _T("��ʼֵ����"), 0, strFilePath) * 3600 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("��ʼֵ����"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("��ʼֵ����"), 0, strFilePath);
	g_sort.elechigh = GetPrivateProfileInt(_T("�ֵ��趨"), _T("����ֵ����"), 0, strFilePath) * 3600 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("����ֵ����"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("�ֵ��趨"), _T("����ֵ����"), 0, strFilePath);
	g_sort.et0 = GetPrivateProfileInt(_T("��Ч���趨"), _T("T0����"), 0, strFilePath) * 3600 +
		GetPrivateProfileInt(_T("��Ч���趨"), _T("T0����"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("��Ч���趨"), _T("T0����"), 0, strFilePath);
  g_sort.ek = GetPrivateProfileInt(_T("��Ч���趨"), _T("K"), 0, strFilePath);
	
	//��ʼ����Ͽ�
	m_sortsel1.AddString(_T("����"));
	m_sortsel1.AddString(_T("����"));
	m_sortsel1.AddString(_T("��Ч�Ƕ�"));
	m_sortsel1.AddString(_T("ԭʼ����"));
	m_sortsel1.AddString(_T("ԭʼ����"));
	m_sortsel2.AddString(_T("����"));
	m_sortsel2.AddString(_T("����"));
	m_sortsel2.AddString(_T("��Ч�Ƕ�"));
	//m_sortsel2.AddString(_T("ԭʼ����"));
	//m_sortsel2.AddString(_T("ԭʼ����"));
	m_sortsel2.AddString(_T("��"));

	g_sort.sortsel1 = GetPrivateProfileInt(_T("�ֵ��趨"), _T("���ǣ����"), 0, strFilePath);
	g_sort.sortsel2 = GetPrivateProfileInt(_T("�ֵ��趨"), _T("�޶��ǣ����"), 0, strFilePath);
	m_sortsel1.SetCurSel(g_sort.sortsel1);
	m_sortsel2.SetCurSel(g_sort.sortsel2);
	//��ʼ��Runҳlistcontrol�ؼ�
	CRect listRect;

	m_runlist.GetClientRect(&listRect);
	m_runlist.SetExtendedStyle(m_runlist.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_runlist.InsertColumn(0, _T("��λ"), LVCFMT_CENTER, listRect.Width() / 8 * 2, 0);
	m_runlist.InsertColumn(1, _T("�Ƕ�"), LVCFMT_CENTER, listRect.Width() / 8 * 4, 1);
	m_runlist.InsertColumn(2, _T("Ƭ��"), LVCFMT_CENTER, listRect.Width() / 8 * 2, 2);
	//itemnum���
	int itemnum = g_sort.itemnum;
  RunListReset();
	/*if (g_sort.islongsqu)
	{
	itemnum = 12;
	}
	else
	{
	itemnum = 24;
	}*/
	/*for (int i = 0; i < itemnum; i++)
	{
		strValue = "";
		strTemps.Format(_T("%d"), i + 1);//_0611_ԭi+1
		m_runlist.InsertItem(i, strTemps);
		strTemps.Format(_T("%02d��"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) / 3600);//��
		strValue += strTemps;
		strTemps.Format(_T("%02d\'"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 3600 / 60);//��
		strValue += strTemps;
		strTemps.Format(_T("%02d\""), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 60);//��
		strValue += strTemps;
		m_runlist.SetItemText(i, 1, strValue);
		m_runlist.SetItemText(i, 2, _T("0"));
	}

	strTemps.Format(_T("%d"), itemnum + 1); m_runlist.InsertItem(itemnum, strTemps); m_runlist.SetItemText(itemnum, 1, _T("����-")); m_runlist.SetItemText(itemnum, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 2); m_runlist.InsertItem(itemnum + 1, strTemps); m_runlist.SetItemText(itemnum + 1, 1, _T("����+")); m_runlist.SetItemText(itemnum + 1, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 3); m_runlist.InsertItem(itemnum + 2, strTemps); m_runlist.SetItemText(itemnum + 2, 1, _T("����-")); m_runlist.SetItemText(itemnum + 2, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 4); m_runlist.InsertItem(itemnum + 3, strTemps); m_runlist.SetItemText(itemnum + 3, 1, _T("����+")); m_runlist.SetItemText(itemnum + 3, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 5); m_runlist.InsertItem(itemnum + 4, strTemps); m_runlist.SetItemText(itemnum + 4, 1, _T("�����ⲻ��")); m_runlist.SetItemText(itemnum + 4, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 6); m_runlist.InsertItem(itemnum + 5, strTemps); m_runlist.SetItemText(itemnum + 5, 1, _T("�Ƕȼ�ⲻ��")); m_runlist.SetItemText(itemnum + 5, 2, _T("0"));
	m_runlist.InsertItem(itemnum + 6, _T("�ϼ�")); m_runlist.SetItemText(itemnum + 6, 1, _T("")); m_runlist.SetItemText(itemnum + 6, 2, _T("0"));
  */

	font.CreatePointFont(800, _T("����"), NULL);
	GetDlgItem(IDC_STATIC_SHOW)->SetFont(&font);
	char l_ctmp[MAX_PATH];
	CString csR1;
	for (int m = 0; m < 360; m++)
	{
		if (TXTReadRowStr(txtFilePath, csR1, m))
		{

			WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, csR1, -1, l_ctmp, sizeof(l_ctmp), NULL, NULL);
			//l_ctmp[strlen(l_ctmp)-2]=0;//���ļ��ж�����/R/N������������/R/N���ᵼ�º���Ķ�ȡ·�����Ҳ��������
			char *cTmp;
			cTmp = new char[strlen(l_ctmp)];
			Split(l_ctmp, cTmp, '\t', 1);//r1t
			g_sort.R1t[m] = atoi(cTmp);
			Split(l_ctmp, cTmp, '\t', 2);//r1e
			g_sort.R1e[m] = atoi(cTmp);
			delete[] cTmp; cTmp = NULL;
		}
		else
			AfxMessageBox(_T("R1��ȡʧ�ܣ����´򿪳���"));
	}
	g_tim.onX = GetPrivateProfileInt(_T("�ȴ�ʱ��"), _T("X��ȡ��"), 0, strFilePath);
	g_tim.offX = GetPrivateProfileInt(_T("�ȴ�ʱ��"), _T("X������"), 0, strFilePath);
	g_tim.offZ = GetPrivateProfileInt(_T("�ȴ�ʱ��"), _T("Z���Ƭ"), 0, strFilePath);
	g_tim.onZ = GetPrivateProfileInt(_T("�ȴ�ʱ��"), _T("Z��ȡ��"), 0, strFilePath);

	g_tim.onY = GetPrivateProfileInt(_T("�ȴ�ʱ��"), _T("Y��ȡ��"), 0, strFilePath);
	g_tim.offY = GetPrivateProfileInt(_T("�ȴ�ʱ��"), _T("Y������"), 0, strFilePath);
	g_tim.blow = GetPrivateProfileInt(_T("�ȴ�ʱ��"), _T("����"), 0, strFilePath);
	SetDlgItemInt(IDC_EDIT_TIME_X, g_tim.onX, 0);
	SetDlgItemInt(IDC_EDIT_TIME_XF, g_tim.offX, 0);
	SetDlgItemInt(IDC_EDIT_TIME_Y, g_tim.onY, 0);
	SetDlgItemInt(IDC_EDIT_TIME_YF, g_tim.offY, 0);
	SetDlgItemInt(IDC_EDIT_TIME_ZF, g_tim.offZ, 0);
	SetDlgItemInt(IDC_EDIT_TIME_Z, g_tim.onZ, 0);

	if(3 != g_status._MOTOR_NUM)
	{
		GetDlgItem(IDC_EDIT_TIME_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIME_ZF)->EnableWindow(FALSE);
	}


	SetDlgItemInt(IDC_EDIT_TIME_BLOW, g_tim.blow, 0);
	EnableToolTips(TRUE);   //trueΪ����ʾ���ܣ�false Ϊ�ر���ʾ����
	m_toolTip.Create(this, TTS_BALLOON);//����
	m_toolTip.SetTipTextColor(RGB(0, 0, 255));//�趨������ɫ
	m_toolTip.SetTipBkColor(RGB(255, 0, 255));
	m_toolTip.SetDelayTime(0);//�趨����ͣ��ʱ��
	m_toolTip.AddTool(GetDlgItem(IDC_EDIT_TIME_XF), _T("���겻Ҫ������"));
	m_toolTip.AddTool(GetDlgItem(IDC_EDIT_TIME_ZF), _T("���겻Ҫ������"));

	TRY
	{
		m_csvAllCard = new CCSVFile(g_path + _T("\\data\\���̿�.csv"), CCSVFile::modeWrite);
	}
	CATCH(CFileException, e)
	{
		CString str;
		//if (e->m_cause == CFileException::accessDenied)
		e->GetErrorMessage(str.GetBufferSetLength(256), 256);
		str.ReleaseBufferSetLength(256);
		// AfxMessageBox(_T("ERROR in open csv\n"));
		AfxMessageBox(str);
	}
	END_CATCH
	
  return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CRun::OnBnClickedRunStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strValue = _T("");
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");

	int status = 0;
	/*	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
	AfxMessageBox(_T("ac6641 ��ʧ��"));
	return;
	}*/
	//���̿���¼��by mmy 171115
	//CString card;
	CString planned, process;

	GetDlgItemText(IDC_EDIT_PLANNED, planned);

	if ("" == planned)
	{
		AfxMessageBox(_T("�ƻ��ţ�"));

		return;
	}
	GetDlgItemText(IDC_EDIT_CARD, process);

	if ("" == process)
	{
		AfxMessageBox(_T("���̿��ţ�"));

		return;
	}
	// �Ա�׼
	if (g_sort.needCheck)
	{
		CDlgCheckStd dlg;
		int ret = dlg.DoModal();
		if (ret != 101)
		{
			if (1 == AfxMessageBox(_T("�Ա�׼δͨ��"), MB_OKCANCEL))
				return;
		}
		g_sort.needCheck = 0;
		CString str;
		GetDlgItemText(IDC_DEG, str);
		WritePrivateProfileString(_T("�Ա��趨"), _T("��һ�ζԱ����ĽǶ�"), str, strFilePath);

	}
  //���̿���¼��190503������BER
  TRY
  {
    if (!m_csvAllCard)
      m_csvAllCard = new CCSVFile(g_path + _T("\\data\\���̿�.csv"), CCSVFile::modeWrite);

  }
    CATCH(CFileException, e)
  {
    CString str;
    e->GetErrorMessage(str.GetBufferSetLength(256), 256);
    str.ReleaseBufferSetLength(256);
    AfxMessageBox(str);
    return;
  }
  END_CATCH
	
	//if ("" == process)
	//{
	//	//AfxMessageBox(_T("���̿��ţ�"));

	//	status = AfxMessageBox(_T("�Ƿ��죿"), MB_OKCANCEL);
	//	if (status == 2)//ȡ��
	//	{
	//		return;
	//	}
	//	else {}
	//	//AfxMessageBox(_T("���������̿���"));
	//	//return;
	//}
	//else
	{
    // ����cpk�ƻ��š����̿�, 
    // 190503 ������BER
    if (!g_pCpk->OpenProcessCard(planned, process)) {}
    GetDlgItemText(IDC_EDIT_CARD, process);
    CProcessCard processCard;
    processCard.no = process;
    CString axis;
    GetDlgItemText(IDC_COMBO_SORT1, axis);
    processCard.axis = axis;
    processCard.center = g_sort.centerangle;

    int tmpCut = USER_TO_SEC(GetDlgItemInt(IDC_EDT_CUTDEG, 0, 1));
    int tmpSort = USER_TO_SEC(GetDlgItemInt(IDC_EDT_SORTDEG, 0, 1));

    processCard.usl = processCard.center - tmpSort / 2 + tmpCut;
    processCard.lsl = processCard.center - tmpSort / 2 - tmpCut;

    CString tmpStr;
    tmpStr.Format(_T("usl : %d lsl : %d"), SEC_TO_USER(processCard.usl), SEC_TO_USER(processCard.lsl));
    AfxMessageBox(tmpStr);


    g_pCpk->AddToPlannedCsv(processCard);

		TRY
		{
			m_csvCard = new CCSVFile(g_path + _T("\\data\\") + process + _T(".csv"), CCSVFile::modeWrite);
		}
		CATCH(CFileException, e)
		{
			CString str;
			//if (e->m_cause == CFileException::accessDenied)
			e->GetErrorMessage(str.GetBufferSetLength(256), 256);
			str.ReleaseBufferSetLength(256);
			// AfxMessageBox(_T("ERROR in open csv\n"));
			AfxMessageBox(str);
			return;
		}
		END_CATCH
	}

	//CCSVFile csvCard(g_path + _T("\\data\\���̿�.csv"), CCSVFile::modeWrite);
	CStringArray arr;
	CTime time;
	time = CTime::GetCurrentTime();

	arr.Add(time.Format("%Y/%m/%d"));
	arr.Add(time.Format("%H:%M:%S"));
	arr.Add(process);
	m_csvAllCard->WriteData(arr);
	arr.RemoveAll();

	if (m_csvCard)
	{
		//CCSVFile csv(g_path + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);

		/*if (0 == csv.GetLength())
		{
		arr.Add(_T("���"));
		arr.Add(_T("����Ƕ�"));
		arr.Add(_T("����Ƕ�"));
		arr.Add(_T("��Ч�Ƕ�"));
		arr.Add(_T("ԭʼ����"));
		arr.Add(_T("ԭʼ����"));
		arr.Add(_T("��λ"));
		csv.WriteData(arr);
		}

		arr.RemoveAll();
		*/
		//�ָ�
		CString str;
		//
		arr.Add(_T("��ʼ"));
		arr.Add(time.Format("%Y/%m/%d"));
		arr.Add(time.Format("%H:%M:%S"));
		m_csvCard->WriteData(arr);
		arr.RemoveAll();
		//
		arr.Add(_T("�ֵ�Ԥ��"));
		GetDlgItemText(IDC_SORT_SEL1, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_DEG, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_SORT, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_SORT_SEL2, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_DEG_MIN, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_DEG_MAX, str);
		if ("" == str)
		{
			AfxMessageBox(_T("����ȷ����"));
			return;
		}
		arr.Add(str);
    GetDlgItemText(IDC_PARAM_T0, str);
    if ("" == str)
    {
      AfxMessageBox(_T("����ȷ����"));
      return;
    }
    arr.Add(str);
    GetDlgItemText(IDC_PARAM_K, str);
    if ("" == str)
    {
      AfxMessageBox(_T("����ȷ����"));
      return;
    }
    arr.Add(str);
		/* if (g_sort.islongsqu)
		{
		arr.Add(_T("����Ƭ"));
		}
		else
		{
		arr.Add(_T("��Ƭ"));
		}*/
		/*str.Format(_T("%d"), g_sort.islongsqu);
		arr.Add(_T("����Ƭ"));
		arr.Add(str);*/
		m_csvCard->WriteData(arr);
		arr.RemoveAll();
		//
		arr.Add(_T("���"));
		arr.Add(_T("����Ƕ�"));
		arr.Add(_T("����Ƕ�"));
		arr.Add(_T("��Ч�Ƕ�"));
		arr.Add(_T("ԭʼ����"));
		arr.Add(_T("ԭʼ����"));
		arr.Add(_T("��λ"));
		m_csvCard->WriteData(arr);
		arr.RemoveAll();

	}

	GetDlgItem(IDC_RUN_START)->EnableWindow(0);
	AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) & 0xfd);//ֹͣ���Ե͵�ƽ
	Sleep(3000);
	status = 0;
	if (g_status._N > 0)
		status = AfxMessageBox(_T("�Ƿ�̳���һ��״̬��"), MB_OKCANCEL);
	if (status == 2)//��ȷ������
	{
		CTime time;
		time = CTime::GetCurrentTime();
		pathSaveDegree = g_path + (_T("\\check\\SortDegree")) + time.Format("%d-%H-%M") + (_T(".txt"));
		//������ʼ��
		g_status._N = 0;
		g_status._RUN = 0;
		g_status._SHOW = 0;
		g_status.CHK_STOP = 0;
		g_status.DSP_STOP = 0;
		g_status.SCR_STOP = 0;
		g_status.SOT_STOP = 0;
		g_status.CY_STOP = 0;
		g_status.SOT2_STOP = 0;
		g_status.SYS_PAUSE = 0;
		g_status.TAK_STOP = 0;
		g_status._DEBUGEFG = 0;//����
		g_sort.sortsn = 0;
		g_sort.sortsum = 0;//1~24����Ƭ��

		for (int i = 0; i < 30; i++)//ÿ���ѷֵ�Ƭ������
		{
			g_sort.sortnum[i] = 0;
			g_sort.alertnum[i] = 1;
		}
		for (int i = 0; i < 5; i++)//5���ƽ�������ʼ��
		{
			g_sort.sortavg[i] = 0;
			g_sort.sortstd[i] = 0;
			g_sort.std2[i] = 0;
		}
		//
		for (int i = 0; i < 31; i++)
		{
			m_runlist.SetItemText(i, 2, _T("0"));
		}
	}
	GetDlgItem(IDC_RUN_SHOW)->SetWindowText(_T("ϵͳ������"));

	//font.CreatePointFont(800,_T("����"),NULL);
	//GetDlgItem(IDC_STATIC_SHOW)->SetFont(&font);
	//===================//����Ƕȵ��ļ�
	/*CStdioFile file;
	CFont font;
	font.CreatePointFont(200,_T("����"),NULL);
	GetDlgItem(IDC_Static�ؼ���)->SetFont(&font);

	file.Open(pathSaveDegree,CFile::modeCreate|CFile::modeWrite);*/
	CString _degree;
	_degree = _T("���\t����Ƕ�\t����Ƕ�\t��Ч�Ƕ�\tԭʼ����\tԭʼ����\t��λ\r\n");
	//_degree.Format(_T("No.\t��\t��\t��E\t��0\t��0\r\n"));
	CFile myFile;
	myFile.Open(pathSaveDegree, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	char szANSIString[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, _degree, -1, szANSIString, sizeof(szANSIString), NULL, NULL);
	//myFile.Write(szANSIString,_degree.GetLength()*2-7);
	myFile.Write(szANSIString, CStringA(_degree).GetLength());

	myFile.Close();


	/*file.Seek(0,CFile::end);
	file.WriteString( _degree );
	file.Close();*/
	//========================================//
	if (3 == g_status._MOTOR_NUM)
	{
		AC6641_SetIoMode(g_hDevice, 0x3F, 0x7);//��ģʽ����Ӧ7-0��11-8��,ǰ����=1Ϊ�����������=0Ϊ����
		AC6641_DO(g_hDevice, 0, 0);
		AC6641_DO(g_hDevice, 1, 0);
		AC6641_DO(g_hDevice, 2, 0);
		AC6641_DO(g_hDevice, 3, 0);
		AC6641_DO(g_hDevice, 4, 0x77);//��0������½��أ�����X/Yͬʱ���㣬����
		AC6641_DO(g_hDevice, 5, 0);
		AC6641_DO(g_hDevice, 8, 0);
		AC6641_DO(g_hDevice, 9, 0);
		AC6641_DO(g_hDevice, 10, 0x03);
	}
	else
	{
		AC6641_SetIoMode(g_hDevice, 0x3f, 0x0);//��ģʽ����Ӧ7-0��11-8��,ǰ����=1Ϊ�����������=0Ϊ����
		AC6641_DO(g_hDevice, 0, 0);
		AC6641_DO(g_hDevice, 1, 0);
		AC6641_DO(g_hDevice, 2, 0);
		AC6641_DO(g_hDevice, 3, 0);
		AC6641_DO(g_hDevice, 4, 0x77);//��0������½��أ�����X/Yͬʱ���㣬����
		AC6641_DO(g_hDevice, 5, 0);
	}
	//�̱߳�־��0
	g_status._CHK1 = 0;
	g_status._CHK2 = 0;
	g_status._N = 0;
	g_status._SCREND = 0;
	g_status._SORTTAK = 1;//��һ��Ҫֱ����Ƭ
	g_status.SORT_END = 0;
	g_sort.checkout0 = 0;
	g_sort.nocheckout = 0;
	yOver = 0;
	errorStep = 0;
	p_take = AfxBeginThread(TakeThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (3 == g_status._MOTOR_NUM)
		p_sort = AfxBeginThread(SortThreadWithZ, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	else
		p_sort = AfxBeginThread(SortThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	p_check = AfxBeginThread(CheckThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	p_screen = AfxBeginThread(ScreenThread, m_csvCard, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	p_display = AfxBeginThread(DisplayThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (3 == g_status._MOTOR_NUM)
	{
		p_sort2 = AfxBeginThread(Sort2Thread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		p_cylinder = AfxBeginThread(CylinderThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

		p_sort2->ResumeThread();
		p_cylinder->ResumeThread();
	}
	p_take->ResumeThread();
	p_sort->ResumeThread();
	p_check->ResumeThread();
	p_screen->ResumeThread();
	p_display->ResumeThread();
	culTime1 = CTime::GetCurrentTime();
	/*g_time.start=culTime1.GetHour()*3600+culTime1.GetMinute()*60+culTime1.GetSecond();
	g_time.pause_sum=0;
	g_time.sum=0;*/
	g_status._RUN = 1;//��ǽ�������״̬
	GetDlgItem(IDC_RUN_STOP)->EnableWindow(1);
	GetDlgItem(IDC_RUN_PAUSE)->EnableWindow(1);
	//��������ֹ�޸�
	GetDlgItem(IDC_EDIT_CARD)->EnableWindow(0);
	GetDlgItem(IDC_SORT_SEL1)->EnableWindow(0);
	GetDlgItem(IDC_DEG)->EnableWindow(0);
	GetDlgItem(IDC_SORT)->EnableWindow(0);
	GetDlgItem(IDC_SORT_SEL2)->EnableWindow(0);
	GetDlgItem(IDC_DEG_MIN)->EnableWindow(0);
	GetDlgItem(IDC_DEG_MAX)->EnableWindow(0);
	GetDlgItem(IDC_CHK_LONG_SQU)->EnableWindow(0);
	GetDlgItem(IDC_SORT_SET)->EnableWindow(0);
  GetDlgItem(IDC_PARAM_T0)->EnableWindow(0);
  GetDlgItem(IDC_PARAM_K)->EnableWindow(0);
}


void CRun::OnBnClickedRunStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_RUN_SHOW)->SetWindowText(_T("ϵͳֹͣ��"));
	//===============//
	/*CStdioFile file;
	file.Open(pathSaveDegree,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);*/
	CString _sort, _avg, _std;
	for (int i = 0; i < 5; i++)
	{
		CString tmp;
		tmp.Format(_T("%02d��%02d'%02d\""), int((g_sort.sortavg[i]) + 0.5) / 3600, int((g_sort.sortavg[i]) + 0.5) % 3600 / 60, int((g_sort.sortavg[i]) + 0.5) % 60);
		_avg += tmp + _T("\t");
		tmp.Format(_T("%02d��%02d'%02d\""), int((g_sort.sortstd[i]) + 0.5) / 3600, int((g_sort.sortstd[i]) + 0.5) % 3600 / 60, int((g_sort.sortstd[i]) + 0.5) % 60);
		_std += tmp + _T("\t");
	}
	_sort = _T("avg\t") + _avg + _T("\r\nstd\t") + _std;
	/*file.Seek(0,CFile::end);
	file.WriteString( _sort );
	file.Close();*/
	CFile myFile;
	myFile.Open(pathSaveDegree, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate);
	char szANSIString[MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, _sort, -1, szANSIString, sizeof(szANSIString), NULL, NULL);
	myFile.Seek(0, CFile::end);
	//myFile.Write(szANSIString,_sort.GetLength()*2-98);
	myFile.Write(szANSIString, CStringA(_sort).GetLength());
	myFile.Close();
	//=================//
	//g_status.CHK_STOP=1;
	//g_status.DSP_STOP=1;
	//g_status.SCR_STOP=1;
	//g_status.SOT_STOP=1;
	g_status.TAK_STOP = 1;
	while (1)//��ȫ���߳��˳�֮���˳��߳�
	{
		if ((!g_status.CHK_STOP) && (!g_status.DSP_STOP) && (!g_status.SCR_STOP) && (!g_status.SOT_STOP) && (!g_status.TAK_STOP) && (!g_status.SOT2_STOP) && (!g_status.CY_STOP))
		{
			Sleep(5);
			if ((!g_status.CHK_STOP) && (!g_status.DSP_STOP) && (!g_status.SCR_STOP) && (!g_status.SOT_STOP) && (!g_status.TAK_STOP) && (!g_status.SOT2_STOP) && (!g_status.CY_STOP))
			{
				break;
			}
		}
		Sleep(1);
	}
	//g_status._N=0;
	AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) | 0x02);//��ʼ���Ըߵ�ƽ
	//���̿���¼
	//CString card;
	//GetDlgItemText(IDC_EDIT_CARD, card);
	if (m_csvCard) {
		//CCSVFile csv(g_path + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);
		CStringArray arr;
		CString str;
		CTime time;
		time = CTime::GetCurrentTime();
		arr.Add(_T("����"));
		arr.Add(time.Format("%Y/%m/%d"));
		arr.Add(time.Format("%H:%M:%S"));
		m_csvCard->WriteData(arr);
		arr.RemoveAll();

		delete m_csvCard;
		m_csvCard = NULL;
	}
	SetDlgItemText(IDC_EDIT_CARD, _T(""));
	GetDlgItem(IDC_RUN_START)->EnableWindow(1);
	g_status._RUN = 0;//��ǽ���ֹͣ״̬
	GetDlgItem(IDC_RUN_STOP)->EnableWindow(0);
	GetDlgItem(IDC_RUN_PAUSE)->EnableWindow(0);
	//��������ֹ�޸�
	GetDlgItem(IDC_EDIT_CARD)->EnableWindow(1);
	GetDlgItem(IDC_SORT_SEL1)->EnableWindow(1);
	GetDlgItem(IDC_DEG)->EnableWindow(1);
	GetDlgItem(IDC_SORT)->EnableWindow(1);
	GetDlgItem(IDC_SORT_SEL2)->EnableWindow(1);
	GetDlgItem(IDC_DEG_MIN)->EnableWindow(1);
	GetDlgItem(IDC_DEG_MAX)->EnableWindow(1);
	GetDlgItem(IDC_CHK_LONG_SQU)->EnableWindow(1);
	GetDlgItem(IDC_SORT_SET)->EnableWindow(1);
  GetDlgItem(IDC_PARAM_T0)->EnableWindow(1);
  GetDlgItem(IDC_PARAM_K)->EnableWindow(1);
}



void CRun::OnBnClickedSortSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strValue = _T("");
	CString strFilePath;
	/*GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	strFilePath.ReleaseBuffer();
	int pos = strFilePath.ReverseFind('\\'); */
	strFilePath = g_path + _T("\\SortSet.ini");

	//�ж�Ԥ��ֵ�ȷ����Ƿ���0~60֮��
	if (((GetDlgItemInt(IDC_DEG, 0, 1) / 10000 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG, 0, 1) / 10000 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_DEG, 0, 1) % 10000 / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG, 0, 1) % 10000 / 100 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_DEG, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG, 0, 1) % 100 >= 0 ? 0 : 1)) != 0)
	{
		AfxMessageBox(_T("��������"));
		return;
	}
	if (((GetDlgItemInt(IDC_SORT, 0, 1) / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_SORT, 0, 1) / 100 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_SORT, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_SORT, 0, 1) % 100 >= 0 ? 0 : 1)) != 0)
	{
		AfxMessageBox(_T("�ֵ�ֵ����"));
		return;
	}
	if (g_sort.sortsel1 == g_sort.sortsel2)
	{
		AfxMessageBox(_T("�趨���ͻ"));
		return;
	}
	if (3 != g_sort.sortsel2)//��������޶���׷���ж�
	{
		if (
			((GetDlgItemInt(IDC_DEG_MIN, 0, 1) / 10000 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG_MIN, 0, 1) / 10000 >= 0 ? 0 : 1) +
			(GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 10000 / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 10000 / 100 >= 0 ? 0 : 1) +
			(GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 100 >= 0 ? 0 : 1) +
			(GetDlgItemInt(IDC_DEG_MAX, 0, 1) / 10000 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG_MAX, 0, 1) / 10000 >= 0 ? 0 : 1) +
			(GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 10000 / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 10000 / 100 >= 0 ? 0 : 1) +
			(GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 100 >= 0 ? 0 : 1))
			!= 0
			)
		{
			AfxMessageBox(_T("�޶�ֵ����"));
			return;
		}
	}
	//��Ч�ǲ�������
	if (((GetDlgItemInt(IDC_PARAM_T0, 0, 1) / 10000 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_PARAM_T0, 0, 1) / 10000 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 10000 / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 10000 / 100 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 100 >= 0 ? 0 : 1)) != 0)
	{
		AfxMessageBox(_T("��Ч�ǲ���phi0����"));
		return;
	}
	//�鿴�ֵ��Ƕ��Ƿ�����нǽǶ�
	int tmpCut = USER_TO_SEC(GetDlgItemInt(IDC_EDT_CUTDEG, 0, 1));
	int tmpSort = USER_TO_SEC(GetDlgItemInt(IDC_SORT, 0, 1));
	if (tmpSort > tmpCut || tmpCut % tmpSort)
	{
		AfxMessageBox(_T("�ֵ��Ƕ����нǽǶȹ�ϵ���ԣ��ֵ���С�ڵ����нǣ����н��ܱ��ֵ�������"));
		return;
	}
	//�鿴�����Ƿ�ı�
	int edt_value = USER_TO_SEC(GetDlgItemInt(IDC_DEG, 0, 1));
	int ini_value = USER_TO_SEC(GetPrivateProfileInt(_T("�Ա��趨"), _T("��һ�ζԱ����ĽǶ�"), 0, strFilePath));

	if (edt_value != ini_value)
	{
		g_sort.needCheck = 1;
		AfxMessageBox(_T("���ϴ����ĽǶȲ�һ�£���Ҫ�Ա�"));
	}
	else
	{
		g_sort.needCheck = 0;
		AfxMessageBox(_T("���ϴ����ĽǶ�һ�£����öԱ�"));
	}

	int usl = edt_value - tmpSort / 2 + tmpCut;
	int lsl = edt_value - tmpSort / 2 - tmpCut;

	CString tmpStr;
	tmpStr.Format(_T("cpk usl : %d lsl : %d"), SEC_TO_USER(usl), SEC_TO_USER(lsl));
	AfxMessageBox(tmpStr);
	//��Ԥ��ֵ���浽ȫ�ֱ���
	g_sort.centerangle = GetDlgItemInt(IDC_DEG, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_DEG, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_DEG, 0, 1) % 100;
	g_sort.sortvalue = GetDlgItemInt(IDC_SORT, 0, 1) / 100 * 60 + GetDlgItemInt(IDC_SORT, 0, 1) % 100;
	if (3 != g_sort.sortsel2)//��������޶���׷���ж�
	{
		g_sort.eleclow = GetDlgItemInt(IDC_DEG_MIN, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 100;
		g_sort.elechigh = GetDlgItemInt(IDC_DEG_MAX, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 100;
	}
	g_sort.et0 = GetDlgItemInt(IDC_PARAM_T0, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 100;
	g_sort.ek = GetDlgItemInt(IDC_PARAM_K, 0, 1);
	
	//���浽ini
	//CString strValue = _T("");
	//CString strFilePath;
	///*GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	//strFilePath.ReleaseBuffer();
	//int pos = strFilePath.ReverseFind('\\'); */
	//strFilePath = g_path + _T("\\SortSet.ini");

	CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
	BOOL ifFind = findini.FindFile(strFilePath);
	if (!ifFind)
	{

		AfxMessageBox(_T("SortSet.ini ������"));
		exit(0);
		/*WritePrivateProfileString(_T("�ֵ��趨"), _T("���̿�"), _T("0"), strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("���ǣ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("���ĽǶȣ���"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("�ֵ�ֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("�ֵ�ֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("�޶��ǣ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("��ʼֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("�ֵ��趨"),_T("����ֵ����"),_T("0"),strFilePath);*/
	}

	GetDlgItemText(IDC_EDIT_PLANNED, strValue);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("�ƻ���"), strValue, strFilePath);


	GetDlgItemText(IDC_EDIT_CARD, strValue);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("���̿�"), strValue, strFilePath);
	strValue.Format(_T("%d"), g_sort.sortsel1);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("���ǣ����"), strValue, strFilePath);
	//
	strValue.Format(_T("%d"), g_sort.sortsel2);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("�޶��ǣ����"), strValue, strFilePath);
	if (3 != g_sort.sortsel2)//��������޶���׷���ж�
	{
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MIN, 0, 1) / 10000);
		WritePrivateProfileString(_T("�ֵ��趨"), _T("��ʼֵ����"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 10000 / 100);
		WritePrivateProfileString(_T("�ֵ��趨"), _T("��ʼֵ����"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 100);
		WritePrivateProfileString(_T("�ֵ��趨"), _T("��ʼֵ����"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MAX, 0, 1) / 10000);
		WritePrivateProfileString(_T("�ֵ��趨"), _T("����ֵ����"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 10000 / 100);
		WritePrivateProfileString(_T("�ֵ��趨"), _T("����ֵ����"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 100);
		WritePrivateProfileString(_T("�ֵ��趨"), _T("����ֵ����"), strValue, strFilePath);
	}

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG, 0, 1) / 10000);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("���ĽǶȣ���"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG, 0, 1) % 10000 / 100);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("���ĽǶȣ���"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG, 0, 1) % 100);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("���ĽǶȣ���"), strValue, strFilePath);

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_SORT, 0, 1) / 100);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("�ֵ�ֵ����"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_SORT, 0, 1) % 100);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("�ֵ�ֵ����"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_EDT_CUTDEG, 0, 1) % 100);
	WritePrivateProfileString(_T("�ֵ��趨"), _T("�н�ֵ����"), strValue, strFilePath);

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_PARAM_T0, 0, 1) / 10000);
	WritePrivateProfileString(_T("��Ч���趨"), _T("T0����"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 10000 / 100);
	WritePrivateProfileString(_T("��Ч���趨"), _T("T0����"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 100);
	WritePrivateProfileString(_T("��Ч���趨"), _T("T0����"), strValue, strFilePath);

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_PARAM_K, 0, 1));
	WritePrivateProfileString(_T("��Ч���趨"), _T("K"), strValue, strFilePath);

	//itemnum���
	int itemnum = g_sort.itemnum;
  RunListReset();
	/*if (g_sort.islongsqu)
	{
	itemnum = 12;
	}
	else
	{
	itemnum = 24;
	}*/

	//��տؼ���ȫ�ֱ���
	/*m_runlist.DeleteAllItems();
	CString sortValue, strTemps;
	for (int i = 0; i < itemnum; i++)
	{
		sortValue.Format(_T("%d"), i + 1);//_0611_ԭi+1
		m_runlist.InsertItem(i, sortValue);
		sortValue = "";

		strTemps.Format(_T("%02d��"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) / 3600);
		sortValue += strTemps;
		strTemps.Format(_T("%02d\'"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 3600 / 60);
		sortValue += strTemps;
		strTemps.Format(_T("%02d\""), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 60);
		sortValue += strTemps;
		m_runlist.SetItemText(i, 1, sortValue);
		m_runlist.SetItemText(i, 2, _T("0"));
		g_sort.sortnum[i] = 0;
	}
	strTemps.Format(_T("%d"), itemnum + 1); m_runlist.InsertItem(itemnum, strTemps); m_runlist.SetItemText(itemnum, 1, _T("����-")); m_runlist.SetItemText(itemnum, 2, _T("0")); g_sort.sortnum[24] = 0;
	strTemps.Format(_T("%d"), itemnum + 2); m_runlist.InsertItem(itemnum + 1, strTemps); m_runlist.SetItemText(itemnum + 1, 1, _T("����+")); m_runlist.SetItemText(itemnum + 1, 2, _T("0")); g_sort.sortnum[25] = 0;
	strTemps.Format(_T("%d"), itemnum + 3); m_runlist.InsertItem(itemnum + 2, strTemps); m_runlist.SetItemText(itemnum + 2, 1, _T("����-")); m_runlist.SetItemText(itemnum + 2, 2, _T("0")); g_sort.sortnum[26] = 0;
	strTemps.Format(_T("%d"), itemnum + 4); m_runlist.InsertItem(itemnum + 3, strTemps); m_runlist.SetItemText(itemnum + 3, 1, _T("����+")); m_runlist.SetItemText(itemnum + 3, 2, _T("0")); g_sort.sortnum[27] = 0;
	strTemps.Format(_T("%d"), itemnum + 5); m_runlist.InsertItem(itemnum + 4, strTemps); m_runlist.SetItemText(itemnum + 4, 1, _T("�����ⲻ��")); m_runlist.SetItemText(itemnum + 4, 2, _T("0")); g_sort.sortnum[28] = 0;
	strTemps.Format(_T("%d"), itemnum + 6); m_runlist.InsertItem(itemnum + 5, strTemps); m_runlist.SetItemText(itemnum + 5, 1, _T("�Ƕȼ�ⲻ��")); m_runlist.SetItemText(itemnum + 5, 2, _T("0")); g_sort.sortnum[29] = 0;
	m_runlist.InsertItem(itemnum + 6, _T("�ϼ�")); m_runlist.SetItemText(itemnum + 6, 1, _T("")); m_runlist.SetItemText(itemnum + 6, 2, _T("0")); g_sort.sortnum[30] = 0;

	for (int i = 0; i < 5; i++)
	{
		g_sort.sortavg[i] = 0;
		g_sort.sortstd[i] = 0;
	}
	g_sort.sortsn = 0;
	g_sort.sortsum = 0;*/
}


void CRun::OnCbnSelchangeSortSel1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_sort.sortsel1 = m_sortsel1.GetCurSel();

}


void CRun::OnCbnSelchangeSortSel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_sort.sortsel2 = m_sortsel2.GetCurSel();
}


void CRun::OnBnClickedRunPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (g_status.SYS_PAUSE)//��ͣ
	{
		g_status.SYS_PAUSE = 0;
		/*pauseT2=CTime::GetCurrentTime();
		if(pauseT2.GetDay()!=pauseT1.GetDay())
		g_time.pause_end=24;
		else
		g_time.pause_end=0;
		*/
		//g_time.pause_end=(g_time.pause_end+pauseT2.GetHour())*3600+pauseT2.GetMinute()*60+pauseT2.GetSecond();
		//g_time.pause_sum+=g_time.pause_end-g_time.pause_start;
		GetDlgItem(IDC_RUN_PAUSE)->SetWindowText(_T("��ͣ"));
		GetDlgItem(IDC_RUN_STOP)->EnableWindow(1);
	}
	else
	{
		g_status.SYS_PAUSE = 1;
		//pauseT1=CTime::GetCurrentTime();
		//g_time.pause_start=pauseT1.GetHour()*3600+pauseT1.GetMinute()*60+pauseT1.GetSecond();
		GetDlgItem(IDC_RUN_PAUSE)->SetWindowText(_T("����"));
		GetDlgItem(IDC_RUN_STOP)->EnableWindow(0);
	}
}


void CRun::OnBnClickedStartFt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//StartFT();
	StartFTLong(1);
	//MyMsgBoxReadIo("it'syou");
	/*MyMsgBoxReadIo2("it'syou");
	MyMsgBoxReadIo2("it's he");
	MyMsgBoxReadIo("heello");*/
}


//void CRun::OnEnChangeEditbs()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	UpdateData(1);
//	i_bs=i_edit_bs;
//}


//����һ�У���������ͷ�Ƿ���Ҫ�ҵ����
BOOL TXTReadRowStr(LPCTSTR pathTXT, CString& str, UINT row)
{
	CFile l_file_;
	if (!l_file_.Open(pathTXT, CFile::modeRead | CFile::shareDenyWrite))
		return 0;
	char* l_char_buf = NULL;
	char l_char_tmp[256] = { 0 };
	DWORD l_dword_len = (DWORD)l_file_.GetLength();
	l_char_buf = new char[l_dword_len + 1];
	memset(l_char_buf, 0, l_dword_len + 1);
	l_file_.Read(l_char_buf, l_dword_len);
	l_file_.Close();

	BOOL rowend = 0;//һ�ж�����
	UINT l_i = 0;		//��buf��ȡ���ڼ����ַ�
	int itmp = 0;		//���ڱ���һ���ַ���

	while (l_i < l_dword_len)
	{
		while (!rowend && l_i < l_dword_len)//���һ�ж�ȡû��ϣ�����һ�д������������������
		{
			if (l_char_buf[l_i] == '\n')
				if (l_char_buf[l_i - 1] == '\r')
				{
					rowend = 1;

				}
				l_char_tmp[itmp++] = l_char_buf[l_i++];
		}
		if (atoi(l_char_tmp) == row)//�������ǵ�ǰҪ�ҵ�
		{
			str = l_char_tmp;
			delete[] l_char_buf;
			l_char_buf = NULL;
			return 1;
		}
		else if (atoi(l_char_tmp) < (int)row)
		{
			memset(l_char_tmp, 0, 256);
			itmp = 0;
			rowend = 0;
		}
		else if (atoi(l_char_tmp) > (int)row)
		{
			delete[] l_char_buf;
			l_char_buf = NULL;
			return 0;
		}

	}
	delete[] l_char_buf;
	l_char_buf = NULL;
	return 0;
}

//���ַ��ָ����ȡ��Ҫ���ַ���
//iSub��0��ʼ��0�ǵ�һ���ַ�����1�ڶ���
//����0��û�ҵ�
//����1���ɹ�
BOOL Split(char* source, char*& dest, char division, UINT iSub)
{
	//dest="";
	char* l_tmpchar;
	// 	int l_ipos = 0;
	// 	int l_pre_ipos = 0;
	int l_ifind = 0;
	int l_ilen = strlen(source);
	l_tmpchar = new char[l_ilen];
	memset(l_tmpchar, 0, l_ilen);
	//CString l_cstr_tmp;
	int j = 0;

	for (int i = 0; i < l_ilen; i++)
	{
		if (source[i] == division)
		{
			l_ifind++;
			continue;
		}

		if (l_ifind == iSub)
		{
			l_tmpchar[j] = source[i];
			j++;
		}

	}

	l_ilen = strlen(l_tmpchar);

	if (0 == l_ilen)
	{
		return 0;
	}

	memset(dest, 0, l_ilen + 1);

	for (int i = 0; i < l_ilen; i++)
	{
		dest[i] = l_tmpchar[i];
	}

	delete[] l_tmpchar;
	l_tmpchar = NULL;
	return 1;

}

void CRun::OnBnClickedBtnStandand()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	pDest.pdata = new TARGB32[width*height];//b,g,r,a  4��8λ���һ������
	pDest.height = height;
	pDest.width = width;
	pDest.byte_width = width << 2;

	g_height = height;
	g_width = width;
	g_lpGray = new BYTE[g_height*g_width];

	int iW = width, iH = height;
	DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//����ɼ�ͼ��

	DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//ת�Ҷ�:����lpVoidBuffer�����pDest

	SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//����Ҷ�ͼ��
	DECODE_THRESHOLD(pDest, pDest, g_lpGray, m_VGAthreshold);//ת��ֵ����pDest������0-255�Ķ�ֵ��ͼƬ��lpGray�����0-1�Ķ�ֵ������

	SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//�����ֵ��ͼ��
	delete[] pDest.pdata;
	pDest.pdata = NULL;
	LoadMark(g_path + _T("\\check\\qz_42.txt"));

	FindOrigin(g_lpGray);
	if (height_num > 100 || width_num > 200 || (height_num == 0 && width_num == 0))
	{

		AfxMessageBox(_T("û���ҵ�ͼ����,�������趨��ֵ"));
		delete[]g_lpGray;
		g_lpGray = NULL;
		//GetDlgItem(IDC_COMPARE)->EnableWindow(0);
		return;
		//delete []g_lpGray;
		//g_lpGray=NULL;
	}

	g_num = new int[10 * 10 * 8];
	GetNumber();

	ScanDegreeDynamic(g_lpGray, g_path + _T("\\check\\positionDynamic.txt"));//���degree[]
	g_sort.R1Num = DOUBLE(degree[0] * 100 + degree[1] * 10 + degree[2] + degree[3] * 0.1 + degree[4] * 0.01 + degree[5] * 0.001);

	ScanDegree(g_lpGray, g_path + _T("\\check\\positionstand.txt"));//���degree[]
	delete[]g_num;
	delete[]g_lpGray;
	g_lpGray = NULL;
	g_num = NULL;
	vga_critical.Unlock();


	g_sort.sortsn = SortChipR1(&g_sort, degree);//�õ���λֵ
	if (g_sort.sortsn <= 0)
		g_sort.sortsn = g_sort.itemnum + 6;
	g_status._SCREND = 1;//��λ��ǣ����ֵ��߳�


	CString _degree, _sort, _avg, _std, _r1;
	_degree.Format(_T("����Ƕȣ�%d\r\n����Ƕȣ�%d\r\n��Ч�Ƕȣ�%d\r\nԭʼ���᣺%d\r\nԭʼ���᣺%d"), degree[0] * 100000 + degree[1] * 10000 + degree[2] * 1000 + degree[3] * 100 + degree[4] * 10 + degree[5],
		degree[6] * 100000 + degree[7] * 10000 + degree[8] * 1000 + degree[9] * 100 + degree[10] * 10 + degree[11],
		degree[12] * 100000 + degree[13] * 10000 + degree[14] * 1000 + degree[15] * 100 + degree[16] * 10 + degree[17],
		degree[18] * 100000 + degree[19] * 10000 + degree[20] * 1000 + degree[21] * 100 + degree[22] * 10 + degree[23],
		degree[24] * 100000 + degree[25] * 10000 + degree[26] * 1000 + degree[27] * 100 + degree[28] * 10 + degree[29]);
	GetDlgItem(IDC_SHOW_AVG)->SetWindowText(_degree);
	//_0614
	_r1.Format(_T("R1:%d\r\nR1���᣺%d\r\nR1���᣺%d"), g_sort.R1Num, g_sort.R1t[g_sort.R1Num], g_sort.R1e[g_sort.R1Num]);
	GetDlgItem(IDC_SHOW_R1)->SetWindowText(_r1);
	int tmpDeg[2];
	tmpDeg[0] = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]) + g_sort.R1t[g_sort.R1Num];
	tmpDeg[1] = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]) + g_sort.R1e[g_sort.R1Num];
	_degree.Format(_T("�������᣺%d\r\n�������᣺%d"),
		tmpDeg[0] / 3600 * 10000 + tmpDeg[0] % 3600 / 60 * 100 + tmpDeg[0] % 60, tmpDeg[1] / 3600 * 10000 + tmpDeg[1] % 3600 / 60 * 100 + tmpDeg[1] % 60);
	GetDlgItem(IDC_SHOW_DEGCOR)->SetWindowText(_degree);
}


void CRun::OnEnChangeEditTimeX()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.onX = GetDlgItemInt(IDC_EDIT_TIME_X, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.onX);
	WritePrivateProfileString(_T("�ȴ�ʱ��"), _T("X��ȡ��"), strValue, strFilePath);
}


void CRun::OnBnClickedChkLongSqu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_sort.islongsqu = ((CButton*)GetDlgItem(IDC_CHK_LONG_SQU))->GetCheck();
	CString csTmp = _T("");
	csTmp.Format(_T("%d"), g_sort.islongsqu);
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	WritePrivateProfileString(_T("�ֵ��趨"), _T("����Ƭ"), csTmp, strFilePath);
	g_sort.chgsqu = 1;

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
}


void CRun::OnBnClickedEndFt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StartFTLong(0);
	//AlertCtrl(false);
}


void CRun::OnEnChangeEditTimeZf()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.offZ = GetDlgItemInt(IDC_EDIT_TIME_ZF, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.offZ);
	WritePrivateProfileString(_T("�ȴ�ʱ��"), _T("Z���Ƭ"), strValue, strFilePath);
}


BOOL CRun::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
		{
			//�ж��Ƿ��¼���Enter��
			switch (pMsg->wParam)
			{
				/*case VK_RETURN:
				OnBnClickedBtnFit();
				return TRUE;*/
			case VK_SPACE:
				StartFTLong(1);
				Sleep(3000);
				StartFTLong(0);
				break;
			default:
				break;
			}
			//return TRUE;
			//Sleep(100);
		}
	}
	else
		m_toolTip.RelayEvent(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}





void CRun::OnEnChangeEditTimeX2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CRun::OnEnChangeEditTimeXf()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.offX = GetDlgItemInt(IDC_EDIT_TIME_XF, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.offX);
	WritePrivateProfileString(_T("�ȴ�ʱ��"), _T("X������"), strValue, strFilePath);
}


void CRun::OnEnChangeEditTimeY()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.onY = GetDlgItemInt(IDC_EDIT_TIME_Y, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.onY);
	WritePrivateProfileString(_T("�ȴ�ʱ��"), _T("Y��ȡ��"), strValue, strFilePath);
}


void CRun::OnEnChangeEditTimeYf()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.offY = GetDlgItemInt(IDC_EDIT_TIME_YF, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.offY);
	WritePrivateProfileString(_T("�ȴ�ʱ��"), _T("Y������"), strValue, strFilePath);
}


void CRun::OnEnChangeEditTimeBlow()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.blow = GetDlgItemInt(IDC_EDIT_TIME_BLOW, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.blow);
	WritePrivateProfileString(_T("�ȴ�ʱ��"), _T("����"), strValue, strFilePath);
}

void CRun::OnEnChangeEditTimeX3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


void CRun::OnEnChangeEditTimeZ()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.onZ = GetDlgItemInt(IDC_EDIT_TIME_Z, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.onZ);
	WritePrivateProfileString(_T("�ȴ�ʱ��"), _T("Z��ȡ��"), strValue, strFilePath);
}

void CRun::OnBnClickedBtnPrint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		// TODO: �ڴ���ӿؼ�֪ͨ����������
	 // CTrendChart trend;

  //trend.Show();

  //return;

	int count = this->m_runlist.GetItemCount(), page, row = 0;

	int i, j;

	page = count / 40 + 1;

	CPrintDialog print(false);

	if (print.DoModal() == IDOK)
	{
		CDC printed;

		printed.Attach(print.GetPrinterDC());

		DOCINFO pdoc;

		memset(&pdoc, 0, sizeof(pdoc));

		pdoc.cbSize = sizeof(pdoc);

		pdoc.lpszDocName = L"pdoc";

		pdoc.lpszDatatype = NULL;

		pdoc.fwType = NULL;

		pdoc.lpszOutput = NULL;

		if (printed.StartDoc(&pdoc) >= 0)
		{
			LOGFONT logfont;

			memset(&logfont, 0, sizeof(LOGFONT));

			logfont.lfHeight = 75;

			CFont font;

			CFont* oldfont = NULL;

			if (font.CreateFontIndirect(&logfont))

				oldfont = (CFont*)printed.SelectObject(&font);

			CRect rectPrint(0, 0,

				printed.GetDeviceCaps(HORZRES),

				printed.GetDeviceCaps(VERTRES));

			printed.DPtoLP(&rectPrint);

			CSize sizeText(75, 75);

			sizeText = printed.GetTextExtent(_T("00"), 2);

			int xMax = rectPrint.Width();

			int xStep = rectPrint.Width() / 12;

			int yMax = rectPrint.Height() / 2;

			int yStep = rectPrint.Height() / 2 / ((count + 1) / 2 + 5);

			int xl = xStep;

			int xr = rectPrint.Width() / 2 + xStep;

			for (j = 1; j <= page; j++)
			{
				printed.StartPage();

				CString pageHead, pageBottom;

				pageHead.Format(_T("�������"));

				printed.TextOut(xStep * 5, yStep * 1, pageHead); //��ӡҳü

				CString title;//���ñ�����

				title.Format(_T("���     �Ƕ�     ����"));

				printed.TextOut(xl, yStep * 2, title); //��ӡ����

				printed.TextOut(xr, yStep * 2, title); //��ӡ����

				CString stt;

				stt.Format(_T("________________________________"));

				printed.TextOut(xl, yStep * 2 + sizeText.cy, stt); //��ӡstt

				printed.TextOut(xr, yStep * 2 + sizeText.cy, stt); //��ӡstt

				for (i = 1; (i < 40 / 2) && (row < count / 2); i++) //��ӡlist
				{
					CString record(_T(""));

					record += this->m_runlist.GetItemText(row, 0) + L"     ";

					record += this->m_runlist.GetItemText(row, 1) + L"     ";

					record += this->m_runlist.GetItemText(row, 2);

					printed.TextOut(xl, yStep * (2 + i), record);

					printed.TextOut(xl, yStep * (2 + i) + sizeText.cy, stt);

					row++;
				}
				for (i = 1; (i < 40 / 2) && (row < count); i++)
				{
					CString record(_T(""));

					record += this->m_runlist.GetItemText(row, 0) + L"     ";

					record += this->m_runlist.GetItemText(row, 1) + L"     ";

					record += this->m_runlist.GetItemText(row, 2);

					printed.TextOut(xr, yStep * (2 + i), record);

					printed.TextOut(xr, yStep * (2 + i) + sizeText.cy, stt);

					row++;
				}
				pageBottom.Format(_T("��%dҳ   ��%dҳ"), page, j);

				printed.TextOut(xStep * 5, yMax - yStep, pageBottom);// ��ӡҳ��

				printed.EndPage();//��ҳ����
			}

			if (oldfont != NULL)

				printed.SelectObject(oldfont);

			font.DeleteObject();

			printed.EndDoc();
		}
		printed.Detach();

		printed.DeleteDC();
	}
}


void CRun::OnBnClickedBtnStdcheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strValue = _T("");
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");

	CDlgCheckStd dlg;
	int ret = dlg.DoModal();
	if (ret != 101)
	{
		AfxMessageBox(_T("�Ա�׼δͨ��"));
		return;
	}
	g_sort.needCheck = 0;
	CString str;
	GetDlgItemText(IDC_DEG, str);
	WritePrivateProfileString(_T("�Ա��趨"), _T("��һ�ζԱ����ĽǶ�"), str, strFilePath);

	return;
}


void CRun::OnBnClickedBtnCpk()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CDlgCpk dlg(g_pCpk);
  dlg.DoModal();

  return;
}
