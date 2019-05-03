// Run.cpp : 实现文件
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
extern int head[10][8], height_num, width_num;//图片标志数据
extern RGBCAPTUREBUFFER pCaptureBuffer;
extern RGBBITMAP       RGBBitmap1;     //第一个采集事件位图信息
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
int errorStep = 0;//标记，确保移动方式正确，当y轴走超过2次正转时，报错
bool yOver;//标记，确保开始后，y轴先初始运动完成，x轴再运动
CTime culTime1, culTime2, pauseT1, pauseT2;//提取时间用
int i_bs = 0;
CFont font;

int nShkStat = 0;

int checkstate = 0;

BOOL TXTReadRowStr(LPCTSTR pathTXT, CString& str, UINT row);

BOOL Split(char* source, char*& dest, char division, UINT iSub);

int RCGVGA(void)//读屏幕
{
	//CCSVFile* csv = (CCSVFile*)pParam;
	int rechecknum = 0;//重测次数
	//while (1)
	//{

	//	while (1)//等待EFG检测完
	//	{
	//		if (g_status._CHK2 && 1 == g_status.SORT_END)
	//			break;
	//		if (1 == g_status.SCR_STOP)
	//		{
	//			g_status.SOT_STOP = 1;//控制分档线程停止
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

				pDest.pdata = new TARGB32[width * height];//b,g,r,a  4个8位组成一个像素
				pDest.height = height;
				pDest.width = width;
				pDest.byte_width = width << 2;

				g_height = height;
				g_width = width;
				g_lpGray = new BYTE[g_height * g_width];

				int iW = width, iH = height;
				//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
				//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

				DECODE_RGB_TO_GRAY((const TUInt8*)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

				//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
				DECODE_THRESHOLD(pDest, pDest, g_lpGray, m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

				//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
				delete[] pDest.pdata;
				pDest.pdata = NULL;
				LoadMark(g_path + _T("\\check\\qz_42.txt"));

				FindOrigin2(g_lpGray);
				if (height_num > 100 || width_num > 200 || (height_num < 0 && width_num < 0))
				{
					if (rechecknum > 20)
					{
						AlertCtrl(1);
						//AfxMessageBox(_T("分档值没读到，继续吗？"));
						int status = AfxMessageBox(_T("分档值没读到，继续吗？"), MB_OKCANCEL);
						if (status == 2)//取消
						{
							delete[]g_lpGray;
							g_lpGray = NULL;
							vga_critical.Unlock();
							for (int i = 0; i < 30; i++)
								degree[i] = 0;//模拟end
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
					//g_status._CHK1=1;//供分档线程
					break;
				}
			}
			g_num = new int[10 * 10 * 8];
			GetNumber();

			ScanDegreeDynamic2(g_lpGray, g_path + _T("\\check\\positionDynamic.txt"));//获得degree[]
			g_sort.R1Num = DOUBLE(degree[0] * 100 + degree[1] * 10 + degree[2] + degree[3] * 0.1 + degree[4] * 0.01 + degree[5] * 0.001);

			ScanDegree2(g_lpGray, g_path + _T("\\check\\position.txt"));//获得degree[]
			delete[]g_num;
			delete[]g_lpGray;
			g_lpGray = NULL;
			g_num = NULL;
			vga_critical.Unlock();
		}
		else
		{
			srand((int)time(0)); //模拟begin
			int deg[30] = { 3,4,0,rand() % 9,rand() % 5,rand() % 9,
				2,2 + rand() % 3,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
				rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
				rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
				rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9 };
			for (int i = 0; i < 30; i++)
				degree[i] = deg[i];//模拟end
		}
		CalcEquAngle(); // DATE:180421
		g_sort.sortsn = SortChipR1(&g_sort, degree);//得到档位值
		if (g_sort.sortsn <= 0)
			g_sort.sortsn = g_sort.itemnum + 6;
		//g_status._SCREND = 1;//置位标记，供分档线程
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
	//	//==============//保存角度到文件
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
	//	//流程卡记录：by mmy 171115
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
	//	//多片提醒
	//	/*if(g_sort.sortnum[g_sort.sortsn-1]>60)
	//	{
	//	//g_sort.alertnum[g_sort.sortsn-1]++;
	//	CString tmp;
	//	tmp.Format(_T("%d档片太多了"),g_sort.sortsn);
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
	while (1)//等待EFG检测完并置位标记
	{
		if ((AC6641_DI(g_hDevice, 6) & 0x02) )//当高电平且chk1、2都是0的时候，检测开始
		{
			Sleep(50);
			if ((AC6641_DI(g_hDevice, 6) & 0x02) )
			{
				Sleep(50);
				if ((AC6641_DI(g_hDevice, 6) & 0x02) )
				{
					while (1)
					{
						if (!(AC6641_DI(g_hDevice, 6) & 0x02))//当低时检测结束
						{
							Sleep(40);
							if (!(AC6641_DI(g_hDevice, 6) & 0x02))//当低时检测结束
							{
								Sleep(40);
								if (!(AC6641_DI(g_hDevice, 6) & 0x02))//三次防抖，防止在检测时出现干扰导致度屏幕先读
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
//线程函数
UINT TakeThread(LPVOID pParam)//取片X轴
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
		ReturnZero(1);//归零
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) & 0xfd);//停止测试低电平
		ac6641_critical.Unlock();
		while (1)//等待震料器有片信号
		{
			if (1 == g_status.TAK_STOP)
			{
				g_status.CHK_STOP = 1;//控制检测线程停止
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
				AfxMessageBox(_T("震料器没下去"));
				AlertCtrl(0);*/
				::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_DLGALM, 0, NULL);//发送显示消息给主窗口

				nShkStat = 0;
			}

			time2 = CTime::GetCurrentTime();

			if (time2.GetSecond() - time1.GetSecond() > 30 && time2.GetSecond() - time1.GetSecond() < 60
				||
				time2.GetSecond() - time1.GetSecond() > -30 && time2.GetSecond() - time1.GetSecond() < 0)
			{
				//int status=0;			
				//AlertCtrl(1);
				////status=AfxMessageBox(_T("超时"));
				//MyMsgBoxReadIo("超时");
				//AlertCtrl(0);
				MyMsgBoxReadIo("超时");
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
		TubeCtrl(1, 1);//吸片
		Sleep(g_tim.onX);
		MotorCtrl(1, 1, g_status.Xwait);//移动到等待上片位置

		if (!g_status._SORTTAK)
		{

			while (1)//等待分档臂取走
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
			if (!(AC6641_DI(g_hDevice, 6) & 0x02))//如果EFG低电平，情况正常
				break;
			else
			{
				ac6641_critical.Lock();
				AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) & 0xfd);//停止测试低电平
				ac6641_critical.Unlock();
			}
			Sleep(5);
		}

		if (nNoBlowN > 100)//三孔支架倒吹
		{

			nNoBlowN = 0;
			ac6641_critical.Lock();
			TBLOW;
			ac6641_critical.Unlock();
			BlowBlock(2000);//吹片
			Sleep(3000);
			ac6641_critical.Lock();
			TNOBLOW;
			ac6641_critical.Unlock();

		}


		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) | 0x02);//开始测试高电平
		ac6641_critical.Unlock();
		Sleep(30);//确保分档臂移走
		//StartFT();//因为开始检测之后有500延迟，已经开始测但手臂没离开，不采用。

		BlowBlock(g_tim.blow);//吹片
		MotorCtrl(1, 1, g_status.Xcheck - g_status.Xwait);//移动到上片位置

		Sleep(50);
		TubeCtrl(1, 0);//关气
		nNoBlowN++;
		Sleep(g_tim.offX);

		g_status._DEBUGEFG = 1;
		//ReturnZero(2);//归零
		time1 = CTime::GetCurrentTime();
	}
	return 0;
}
UINT SortThread(LPVOID pParam)//分档Y轴
{
	ReturnZero(1);//归零
	ReturnZero(2);//归零
	TubeCtrl(2, 1);//吸气
	Sleep(g_tim.onY);
	MotorCtrl(2, 1, g_status.Ywait);//移动到等待位置
	yOver = 1;
	Sleep(100);
	TubeCtrl(2, 0);//关气
	Sleep(g_tim.offY);
	/*CTime time1,time2;
	time1=CTime::GetCurrentTime();*/
	while (1)
	{
		while (1)//等待EFG检测完
		{
			if (g_status._CHK1)
				break;
			if (1 == g_status.SOT_STOP)
			{
				g_status.DSP_STOP = 1;//控制显示线程停止
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
			status=AfxMessageBox(_T("超时"));

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
		vga_critical.Lock();//取之前先判断M植，此与读屏幕有冲突，故lock
		if(!ScanDegreeEFGM(g_lpGray,_T("check\\positionM.txt")))//检测M值
		{
		AlertCtrl(1);
		AfxMessageBox(_T("M值读不到,继续检测？"));
		AlertCtrl(0);
		}
		vga_critical.Unlock();
		*/
		ReturnZero(2);//归零
		Sleep(100);
		TubeCtrl(2, 1);//吸气
		Sleep(g_tim.onY);
		//MotorCtrl(2,1,g_status.Ywait/4,1);//转动到等待
		//MotorCtrl(2,1,g_status.Ywait+i_bs);//转动到等待

		g_status.SORT_END = 1;//供度屏幕线程，可以读屏了
		while (1)//等待屏幕读取完
		{
			if (g_status._SCREND)
				break;
			if (1 == g_status.SOT_STOP)
			{
				g_status.DSP_STOP = 1;//控制显示线程停止
				g_status.SOT_STOP = 0;
				return 0;
			}
			Sleep(1);
		}
		g_status._SORTTAK = 1;//供取片线程//0610与下一句交换


		MotorCtrl(2, 1, g_status.Ywait);//转动到等待


		Sleep(500);

		//time1=CTime::GetCurrentTime();
		g_status._SCREND = 0;
		//g_status._SHOW=1;
		//updatesort=1;
		SortBlock(g_sort.sortsn);//分档	
	}
	return 0;
}

UINT SortThreadWithZ(LPVOID pParam)//分档Y轴
{
	ReturnZero(1);//归零
	ReturnZero(2);//归零
	TubeCtrl(2, 1);//吸气
	Sleep(g_tim.onY);
	MotorCtrl(2, 1, g_status.Ywait);//移动到等待位置
	yOver = 1;
	Sleep(100);
	TubeCtrl(2, 0);//关气
	Sleep(g_tim.offY);
	/*CTime time1,time2;
	time1=CTime::GetCurrentTime();*/
	while (1)
	{
		while (1)//等待EFG检测完
		{
			if (g_status._CHK1)
				break;
			if (1 == g_status.SOT_STOP)
			{
				//g_status.DSP_STOP=1;//控制显示线程停止
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
			status=AfxMessageBox(_T("超时"));

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
		vga_critical.Lock();//取之前先判断M植，此与读屏幕有冲突，故lock
		if(!ScanDegreeEFGM(g_lpGray,_T("check\\positionM.txt")))//检测M值
		{
		AlertCtrl(1);
		AfxMessageBox(_T("M值读不到,继续检测？"));
		AlertCtrl(0);
		}
		vga_critical.Unlock();
		*/
		ReturnZero(2);//归零
		Sleep(100);
		TubeCtrl(2, 1);//吸气
		Sleep(g_tim.onY);
		//MotorCtrl(2,1,g_status.Ywait/4,1);//转动到等待
		//MotorCtrl(2,1,g_status.Ywait+i_bs);//转动到等待
		g_status._SORTTAK = 1;//供取片线程//0610与下一句交换
		g_status.SORT_END = 1;//供度屏幕线程，可以读屏了
		MotorCtrl(2, 1, g_status.Ypass);//转动到放片点
		Sleep(200);

		while (1)//等待屏幕读取完
		{
			if (g_status._SCREND && 0 == g_status.CyStat)
				break;
			if (1 == g_status.SOT_STOP && 0 == g_status.CyStat)
			{
				////g_status.DSP_STOP=1;//控制显示线程停止
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
		TubeCtrl(2, 0);//关气
		Sleep(g_tim.offY);
		g_status.CyBlock = 1;
		MotorCtrl(2, 0, g_status.Ypass - g_status.Ywait);//转动到等待
	}
	return 0;
}

UINT Sort2Thread(LPVOID pParam)//分档z轴
{
	g_status.CyStat = 0;
	g_status.CyBlock = 1;
	ReturnZero(3);//归零
	Sleep(100);
	g_sort.sortsn = g_sort.itemnum + 6;
	while (!g_status.SOT2_STOP)
	{
		Sleep(1);
		if (1 == g_status.CyStat)
		{
			TubeCtrl(3, 1);//吸气
			Sleep(g_tim.onZ);
			g_status.CyBlock = 0;
			SortBlockWithZ(g_sort.sortsn);//分档
		}

	}
	g_status.DSP_STOP = 1;//控制显示线程停止
	g_status.SOT2_STOP = 0;
	return 0;
}
UINT CylinderThread(LPVOID pParam)
{
	while (!g_status.CY_STOP)
	{
		Sleep(1);
		switch (g_status.CyBlock)//如果气缸上有片
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
UINT CheckThread(LPVOID lParam)//检测EFG
{
	while (1)
	{
		checkstate = 0;
		while (1)//等待EFG检测完并置位标记
		{
			if ((AC6641_DI(g_hDevice, 6) & 0x02) && !g_status._CHK1 && !g_status._CHK2&& g_status._DEBUGEFG == 1)//当高电平且chk1、2都是0的时候，检测开始
			{
				Sleep(50);
				if ((AC6641_DI(g_hDevice, 6) & 0x02) && !g_status._CHK1 && !g_status._CHK2)
				{
					Sleep(50);
					if ((AC6641_DI(g_hDevice, 6) & 0x02) && !g_status._CHK1 && !g_status._CHK2)
					{
						checkstate = 1;//标记进入检测状态
						while (1)
						{
							if (!(AC6641_DI(g_hDevice, 6) & 0x02))//当低时检测结束
							{
								Sleep(40);
								if (!(AC6641_DI(g_hDevice, 6) & 0x02))//当低时检测结束
								{
									Sleep(40);
									if (!(AC6641_DI(g_hDevice, 6) & 0x02))//三次防抖，防止在检测时出现干扰导致度屏幕先读
										break;
								}
							}
							Sleep(1);
						}
						break;
					}
				}

			}

			/*if(g_status._DEBUGEFG && !g_status._CHK1 && !g_status._CHK2)//调试
			{
			g_status._DEBUGEFG=0;
			Sleep(2000);
			break;
			}*/
			//Sleep(100);
			Sleep(1);
			if (1 == g_status.CHK_STOP)
			{
				g_status.SCR_STOP = 1;//控制度屏幕停止
				g_status.CHK_STOP = 0;
				return 0;
			}

		}
		g_status._DEBUGEFG = 0;
		//Sleep(10);
		g_status._CHK1 = 1;//供分档线程
		g_status._CHK2 = 1;//供读屏幕线程
	}
	return 0;
}
UINT ScreenThread(LPVOID pParam)//读屏幕
{
	CCSVFile *csv = (CCSVFile*)pParam;
	int rechecknum = 0;//重测次数
	while (1)
	{

		while (1)//等待EFG检测完
		{
			if (g_status._CHK2 && 1 == g_status.SORT_END)
				break;
			if (1 == g_status.SCR_STOP)
			{
        // CPK 190503 复制自ber
        if (g_pCpk)
        {
          CProcessCard processCard;

          g_pCpk->CalcCpk(processCard);

          g_pCpk->AddCpkToPlannedCsv(processCard);
        }

				g_status.SOT_STOP = 1;//控制分档线程停止
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

		//		pDest.pdata = new TARGB32[width*height];//b,g,r,a  4个8位组成一个像素
		//		pDest.height = height;
		//		pDest.width = width;
		//		pDest.byte_width = width << 2;

		//		g_height = height;
		//		g_width = width;
		//		g_lpGray = new BYTE[g_height*g_width];

		//		int iW = width, iH = height;
		//		//DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
		//		//SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

		//		DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

		//		//SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
		//		DECODE_THRESHOLD(pDest, pDest, g_lpGray, m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

		//		//SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
		//		delete[] pDest.pdata;
		//		pDest.pdata = NULL;
		//		LoadMark(g_path + _T("\\check\\qz_42.txt"));

		//		FindOrigin2(g_lpGray);
		//		if (height_num > 100 || width_num > 200 || (height_num < 0 && width_num < 0))
		//		{
		//			if (rechecknum > 20)
		//			{
		//				AlertCtrl(1);
		//				AfxMessageBox(_T("分档值没读到，继续吗？"));
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
		//			//g_status._CHK1=1;//供分档线程
		//			break;
		//		}
		//	}
		//	g_num = new int[10 * 10 * 8];
		//	GetNumber();

		//	ScanDegreeDynamic2(g_lpGray, g_path + _T("\\check\\positionDynamic.txt"));//获得degree[]
		//	g_sort.R1Num = DOUBLE(degree[0] * 100 + degree[1] * 10 + degree[2] + degree[3] * 0.1 + degree[4] * 0.01 + degree[5] * 0.001);

		//	ScanDegree2(g_lpGray, g_path + _T("\\check\\position.txt"));//获得degree[]
		//	delete[]g_num;
		//	delete[]g_lpGray;
		//	g_lpGray = NULL;
		//	g_num = NULL;
		//	vga_critical.Unlock();
		//}
		//else
		//{
		//	srand((int)time(0)); //模拟begin
		//	int deg[30] = { 3,4,0,rand() % 9,rand() % 5,rand() % 9,
		//		2,2 + rand() % 3,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
		//		rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
		//		rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9,
		//		rand() % 5,rand() % 9,rand() % 5,rand() % 9,rand() % 5,rand() % 9 };
		//	for (int i = 0; i < 30; i++)
		//		degree[i] = deg[i];//模拟end
		//}
  //  CalcEquAngle(); // DATE:180421
		//g_sort.sortsn = SortChipR1(&g_sort, degree);//得到档位值
		//if (g_sort.sortsn <= 0)
		//	g_sort.sortsn = g_sort.itemnum + 6;
		RCGVGA();
		g_status._SCREND = 1;//置位标记，供分档线程
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
		//==============//保存角度到文件
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
		//流程卡记录：by mmy 171115
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
		//多片提醒
		/*if(g_sort.sortnum[g_sort.sortsn-1]>60)
		{
		//g_sort.alertnum[g_sort.sortsn-1]++;
		CString tmp;
		tmp.Format(_T("%d档片太多了"),g_sort.sortsn);
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

      // cpk实时检查
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
          csCpk += _T("超限");
          ((CATtoSCDlg*)(AfxGetApp()->m_pMainWnd))->m_Run.OnBnClickedRunPause();
          //g_dlgRun->OnBnClickedBtnPause();
        }
        ((CATtoSCDlg*)(AfxGetApp()->m_pMainWnd))->m_Run.SetDlgItemText(IDC_RUN_SHOW, csCpk);
      }
    }

	}
	return 0;
}
UINT DisplayThread(LPVOID pParam)//显示
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
			::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_DISPLAY, 0, NULL);//发送显示消息给主窗口
			nCnt = 0;
		}
		if (ShakeDown)//用于判断是否震料器有下去
			nShkStat = 0;
		Sleep(1);
		nCnt++;
	}
	return 0;
}
//自定义函数
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
void MotorCtrl(int n, bool zf, int steps, bool send)//控制n号电机正反（zf）转steps步
{
	int tmp = steps;
	int ioData = tmp % 256;
	if (n == 1)//x轴
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 0, ioData);
		ioData = tmp / 256,
			AC6641_DO(g_hDevice, 1, ioData);
		ioData = AC6641_DI(g_hDevice, 5) & 0xfb;//52低电平，全电流
		AC6641_DO(g_hDevice, 5, ioData);
		ioData = AC6641_DI(g_hDevice, 4) | 0x02;//将p41置高，为下降沿做准备。
		tmp = zf;//获得正反转
		0 == tmp ? (ioData = ioData & 0xfe) : (ioData = ioData | 0x01);
		AC6641_DO(g_hDevice, 4, ioData);//正反转
		Sleep(1);
		ioData = ioData & 0xfd;
		AC6641_DO(g_hDevice, 4, ioData);//下降沿
		Sleep(10);
		ioData = ioData | 0x02;
		AC6641_DO(g_hDevice, 4, ioData);//回高

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x01);//当高电平时
		//	Sleep(500);
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x01))
				break;
			Sleep(1);
		}
		ac6641_critical.Lock();
		ioData = AC6641_DI(g_hDevice, 5) | 0x04;//52高电平，半电流
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (n == 2)//y轴
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 2, ioData);
		ioData = tmp / 256,
			AC6641_DO(g_hDevice, 3, ioData);
		ioData = AC6641_DI(g_hDevice, 5) & 0xf7;//53低电平，全电流
		AC6641_DO(g_hDevice, 5, ioData);
		ioData = AC6641_DI(g_hDevice, 4) | 0x20;//将p45置高，为下降沿做准备。
		tmp = zf;//获得正反转
		0 == tmp ? (ioData = ioData & 0xef) : (ioData = ioData | 0x10);
		if (0 == tmp)
			errorStep = 0;
		else
		{
			errorStep++;
			if (errorStep > 2)
				AfxMessageBox(_T("error"));
		}
		AC6641_DO(g_hDevice, 4, ioData);//正反转
		Sleep(1);
		ioData = ioData & 0xdf;
		AC6641_DO(g_hDevice, 4, ioData);//下降沿
		Sleep(10);
		ioData = ioData | 0x20;
		AC6641_DO(g_hDevice, 4, ioData);//回高

		ac6641_critical.Unlock();
		if (send)
			g_status._SORTTAK = 1;//供取片线程
		//while(AC6641_DI(g_hDevice,6) & 0x10);//当高电平时
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x10))
				break;
			Sleep(1);
		}
		ac6641_critical.Lock();
		ioData = AC6641_DI(g_hDevice, 5) | 0x08;//53高电平，半电流
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (n == 3)//z轴
	{

		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 8, ioData);
		ioData = tmp / 256,
			AC6641_DO(g_hDevice, 9, ioData);
		ioData = AC6641_DI(g_hDevice, 10) | 0x02;//将p41置高，为下降沿做准备。
		tmp = zf;//获得正反转
		0 == tmp ? (ioData = ioData & 0xfe) : (ioData = ioData | 0x01);
		AC6641_DO(g_hDevice, 10, ioData);//正反转
		Sleep(10);
		ioData = ioData & 0xfd;
		AC6641_DO(g_hDevice, 10, ioData);//下降沿
		Sleep(10);
		ioData = ioData | 0x02;
		AC6641_DO(g_hDevice, 10, ioData);//回高

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x01);//当高电平时
		//	Sleep(500);
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 11) & 0x01))
				break;
			Sleep(1);
		}
	}
}
void MotorCtrl(int n, bool zf, int steps)//控制n号电机正反（zf）转steps步
{
	int tmp = steps;
	int ioData = tmp % 256;
	if (n == 1)//x轴
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 0, ioData);
		ioData = tmp / 256;
		AC6641_DO(g_hDevice, 1, ioData);
		ioData = AC6641_DI(g_hDevice, 5) & 0xfb;//52低电平，全电流
		AC6641_DO(g_hDevice, 5, ioData);
		ioData = AC6641_DI(g_hDevice, 4) | 0x02;//将p41置高，为下降沿做准备。
		tmp = zf;//获得正反转
		0 == tmp ? (ioData = ioData & 0xfe) : (ioData = ioData | 0x01);
		AC6641_DO(g_hDevice, 4, ioData);//正反转
		Sleep(1);
		ioData = ioData & 0xfd;
		AC6641_DO(g_hDevice, 4, ioData);//下降沿
		Sleep(5);//_0610
		ioData = ioData | 0x02;
		AC6641_DO(g_hDevice, 4, ioData);//回高

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x01);//当高电平时
		//	Sleep(500);
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x01))
				break;
			Sleep(1);
		}
		ac6641_critical.Lock();
		ioData = AC6641_DI(g_hDevice, 5) | 0x04;//52高电平，半电流
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (n == 2)//y轴
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 2, ioData);
		ioData = tmp / 256;
		AC6641_DO(g_hDevice, 3, ioData);
		ioData = AC6641_DI(g_hDevice, 5) & 0xf7;//53低电平，全电流
		AC6641_DO(g_hDevice, 5, ioData);
		ioData = AC6641_DI(g_hDevice, 4) | 0x20;//将p45置高，为下降沿做准备。
		tmp = zf;//获得正反转
		0 == tmp ? (ioData = ioData & 0xef) : (ioData = ioData | 0x10);
		if (0 == tmp)
			errorStep = 0;
		else
		{
			errorStep++;
			if (errorStep > 2)
				AfxMessageBox(_T("error"));
		}
		AC6641_DO(g_hDevice, 4, ioData);//正反转
		Sleep(1);
		ioData = ioData & 0xdf;
		AC6641_DO(g_hDevice, 4, ioData);//下降沿
		Sleep(5);//_0610
		ioData = ioData | 0x20;
		AC6641_DO(g_hDevice, 4, ioData);//回高

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x10);//当高电平时
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 6) & 0x10))
				break;
			Sleep(1);
		}
		ac6641_critical.Lock();
		ioData = AC6641_DI(g_hDevice, 5) | 0x08;//53高电平，半电流
		AC6641_DO(g_hDevice, 5, ioData);
		ac6641_critical.Unlock();
	}
	else if (n == 3)//z轴
	{
		ac6641_critical.Lock();
		AC6641_DO(g_hDevice, 8, ioData);
		ioData = tmp / 256;
		AC6641_DO(g_hDevice, 9, ioData);
		ioData = AC6641_DI(g_hDevice, 10) | 0x02;//将p41置高，为下降沿做准备。
		tmp = zf;//获得正反转
		0 == tmp ? (ioData = ioData & 0xfe) : (ioData = ioData | 0x01);
		AC6641_DO(g_hDevice, 10, ioData);//正反转
		Sleep(1);
		ioData = ioData & 0xfd;
		AC6641_DO(g_hDevice, 10, ioData);//下降沿
		Sleep(10);
		ioData = ioData | 0x02;
		AC6641_DO(g_hDevice, 10, ioData);//回高

		ac6641_critical.Unlock();
		//while(AC6641_DI(g_hDevice,6) & 0x01);//当高电平时
		//	Sleep(500);
		for (int i = 0; i < 5000; i++)
		{
			if (!(AC6641_DI(g_hDevice, 11) & 0x01))
				break;
			Sleep(1);
		}
	}
}
void CylinderCtrl(bool stat)//水平气缸
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

void TubeCtrl(int n, bool stat)//控制n号机械臂的气嘴开关（stat）
{
	ac6641_critical.Lock();
	int ioData = 0;
	if (n == 1)//x轴
	{
		ioData = AC6641_DI(g_hDevice, 4);
		if (1 == stat)
			ioData = ioData | 0x08;
		else if (0 == stat)
			ioData = ioData & 0xf7;
		AC6641_DO(g_hDevice, 4, ioData);
	}
	else if (n == 2)//y轴
	{
		ioData = AC6641_DI(g_hDevice, 4);
		if (1 == stat)
			ioData = ioData | 0x80;
		else if (0 == stat)
			ioData = ioData & 0x7f;
		AC6641_DO(g_hDevice, 4, ioData);
	}
	else if (n == 3)//z轴
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
void SortBlock(int n)//分档
{
	n--;//因为sortsn是从1到30，与外部显示相同，但数组编号是从0到29，所以要--。警惕-1
	MotorCtrl(2, 1, g_status.Ysort[n] - g_status.Ywait);//转动到档位
	Sleep(200);
	TubeCtrl(2, 0);//关气
	Sleep(g_tim.offY);
	g_status._N++;
	MotorCtrl(2, 0, g_status.Ysort[n] - g_status.Ywait / 4);//移动到等待位置
}
void SortBlockWithZ(int n)//分档
{
	n--;//因为sortsn是从1到30，与外部显示相同，但数组编号是从0到29，所以要--。警惕-1
	if (n<0 || n>g_sort.itemnum + 6 - 1)
	{
		n = g_sort.itemnum + 6 - 1;
	}
	MotorCtrl(3, 1, g_status.Zsort[n]);//-g_status.Ywait);//转动到档位
	Sleep(50);
	TubeCtrl(3, 0);//关气
	Sleep(g_tim.offZ);
	g_status._N++;
	ReturnZero(3);
	Sleep(100);
	//MotorCtrl(3,0,g_status.Zsort[n]-g_status.Ywait/4);//移动到等待位置
}
void VibrateBlock()//震料器
{
}

int ShakePlatformUp()//震料器
{
	return g_shakeE ? !(AC6641_DI(g_hDevice, 6) & 0x04) : (AC6641_DI(g_hDevice, 6) & 0x04);
}


int ShakePlatformDown()//震料器
{
	return g_shakeE ? (AC6641_DI(g_hDevice, 6) & 0x04) : !(AC6641_DI(g_hDevice, 6) & 0x04);
}


void ReturnZero(int n)//控制1、2号机械臂回零位
{
	if (n == 1)//x轴
		MotorCtrl(n, 0, 10000);//取片等待
	else if (n == 2)//y轴
		MotorCtrl(n, 0, 10000);//取片等待
	else if (n == 3)//z轴
		MotorCtrl(n, 0, 10000);//取片等待
}
void BlowBlock(UINT time)//吹气
{
	ac6641_critical.Lock();
	int ioData = AC6641_DI(g_hDevice, 5) | 0x01;

	AC6641_DO(g_hDevice, 5, ioData);//吹气
	Sleep(time);
	ioData = AC6641_DI(g_hDevice, 5) & 0xfe;
	AC6641_DO(g_hDevice, 5, ioData);//关闭吹气
	ac6641_critical.Unlock();
}
void StartFT()//efg开始检测信号
{
	ac6641_critical.Lock();
	int ioData = AC6641_DI(g_hDevice, 5) | 0x02;

	AC6641_DO(g_hDevice, 5, ioData);
	Sleep(500);
	ioData = AC6641_DI(g_hDevice, 5) & 0xfd;
	AC6641_DO(g_hDevice, 5, ioData);
	ac6641_critical.Unlock();
}

void StartFTLong(bool se)//efg开始检测信号
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
//分档计算程序，使用到全局变量
//返回档位值，供机械手分档
int SortChip(sortchip* s_sort, int *degree)
{
	bool elecyes = 1;//限定轴通过标志
	int sortSN = 0;//返回值，分档值

	//s_sort->nocheckout=0;
	int angle[5] = { (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]),//光轴
		(degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]),//电轴
		(degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]),//等效角
		(degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]),//原始光轴
		(degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29]) };//原始电轴；保存测定出的角度，转换成秒
	if (degree[0] == -1 || angle[3] == 0)//角度没测出
	{
		sortSN = 30;
		s_sort->sortnum[29]++;
		s_sort->nocheckout++;
		return sortSN;
	}
	if (0 == angle[0] && angle[3] != 0)//激光检测不出
	{
		sortSN = 29;
		s_sort->sortnum[28]++;
		s_sort->checkout0++;
		return sortSN;
	}
	//s_sort->checkout0=0;
	if (s_sort->sortsel2 != 3)//如果电轴限定
	{
		switch ((angle[s_sort->sortsel2] <= s_sort->elechigh ? -1 : 0) + (angle[s_sort->sortsel2] >= s_sort->eleclow ? 1 : 0))//计算限定轴并分档
		{
		case 0:break;
		case 1:elecyes = 0; s_sort->sortnum[25]++; sortSN = 26; break;
		case -1:elecyes = 0; s_sort->sortnum[24]++; sortSN = 25; break;
		}
	}
	if (1 == elecyes)//如果限定轴通过
	{
		if (angle[s_sort->sortsel1] >= (s_sort->centerangle - s_sort->sortvalue / 2))//如果检测轴大于预设值
		{
			switch ((angle[s_sort->sortsel1] - s_sort->centerangle + s_sort->sortvalue / 2) / s_sort->sortvalue + 1)//计算检测轴并分档（检测轴大于预设值
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
			switch ((s_sort->centerangle - s_sort->sortvalue / 2 - angle[s_sort->sortsel1] - 1) / s_sort->sortvalue + 1)//计算检测轴并分档（检测轴小于预设值
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
  int sort_num = g_sort.itemnum;//档位数量

  //int *sort_sec = m_resultParam.degree.sort_sec;//各档位中间值
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

//分档计算程序，使用到全局变量
//返回档位值，供机械手分档
//itemnum相关
int SortChipR1(sortchip* s_sort, int *degree)//_0614
{
	bool elecyes = 1;//限定轴通过标志
	int sortSN = 0;//返回值，分档值
	int sel1SN = 5, sel2SN = 5;

	switch (s_sort->sortsel2)
	{
	case 0://光轴限定
		sel2SN = 3;
		break;
	case 1://电轴限定
		sel2SN = 1;
		break;
	case 2://等效角
		sel2SN = 3;
		switch (s_sort->sortsel1)
		{
		case 0://光轴限定
			sel2SN = 1;
			break;
		case 1://电轴限定
			sel2SN = 3;
			break;
		}
		break;
	default:
		break;
	}

	switch (s_sort->sortsel1)
	{
	case 0://光轴限定
		sel1SN = 3;
		break;
	case 1://电轴限定
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
	int angle[5] = { (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]),//光轴
		(degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]),//电轴
		(degree[12] * 10 + degree[13]) * 3600 + (degree[14] * 10 + degree[15]) * 60 + (degree[16] * 10 + degree[17]),//等效角
		(degree[18] * 10 + degree[19]) * 3600 + (degree[20] * 10 + degree[21]) * 60 + (degree[22] * 10 + degree[23]),//原始光轴
		(degree[24] * 10 + degree[25]) * 3600 + (degree[26] * 10 + degree[27]) * 60 + (degree[28] * 10 + degree[29]) };//原始电轴；保存测定出的角度，转换成秒
	if (degree[0] == -1 || angle[3] == 0)//角度没测出
	{
		sortSN = g_sort.itemnum + 6;
		s_sort->sortnum[sortSN - 1]++;
		s_sort->nocheckout++;
		return sortSN;
	}
	if (0 == angle[0] && angle[3] != 0)//激光检测不出
	{
		sortSN = g_sort.itemnum + 5;
		s_sort->sortnum[sortSN - 1]++;
		s_sort->checkout0++;
		return sortSN;
	}
	angle[0] = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]) + g_sort.R1t[g_sort.R1Num];//光轴//_0614
	angle[1] = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]) + g_sort.R1e[g_sort.R1Num];//电轴
	s_sort->nocheckout = 0;
	s_sort->checkout0 = 0;
	if (s_sort->sortsel2 != 3)//如果轴限定
	{
		switch ((angle[s_sort->sortsel2] <= s_sort->elechigh ? -1 : 0) + (angle[s_sort->sortsel2] >= s_sort->eleclow ? 1 : 0))//计算限定轴并分档
		{
		case 0:break;
		case 1:elecyes = 0; sortSN = g_sort.itemnum + sel2SN + 1; s_sort->sortnum[sortSN - 1]++; break;
		case -1:elecyes = 0; sortSN = g_sort.itemnum + sel2SN; s_sort->sortnum[sortSN - 1]++; break;
		}
	}
	if (1 == elecyes)//如果限定轴通过
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
		//	if (angle[s_sort->sortsel1] >= (s_sort->centerangle - s_sort->sortvalue / 2))//如果检测轴大于预设值
		//	{
		//		switch ((angle[s_sort->sortsel1] - s_sort->centerangle + s_sort->sortvalue / 2) / s_sort->sortvalue + 1)//计算检测轴并分档（检测轴大于预设值
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
		//		switch ((s_sort->centerangle - s_sort->sortvalue / 2 - angle[s_sort->sortsel1] - 1) / s_sort->sortvalue + 1)//计算检测轴并分档（检测轴小于预设值
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
		//	if (angle[s_sort->sortsel1] >= (s_sort->centerangle - s_sort->sortvalue / 2))//如果检测轴大于预设值
		//	{
		//		switch ((angle[s_sort->sortsel1] - s_sort->centerangle + s_sort->sortvalue / 2) / s_sort->sortvalue + 1)//计算检测轴并分档（检测轴大于预设值
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
		//		switch ((s_sort->centerangle - s_sort->sortvalue / 2 - angle[s_sort->sortsel1] - 1) / s_sort->sortvalue + 1)//计算检测轴并分档（检测轴小于预设值
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
//计算光轴平均值及标准方程
void SortAvgStd(unsigned int sum, int *angle, double *avg, double *std, long double *std2)
{
	double n = sum;
	for (int i = 0; i < 5; i++)
	{
		double xi = angle[i];
		//double avg;
		if (1 == n)
		{
			avg[i] = xi;//平均值
			std2[i] = xi*xi;//标准偏差中间值
		}
		if (n > 1)
		{
			avg[i] = (avg[i] + xi / (n - 1)) / (n / (n - 1));//求平均值
			std2[i] = (std2[i] + xi*xi / (n - 1)) / (n / (n - 1));//求标准差中间值
			std[i] = sqrt(fabs(std2[i] - avg[i] * avg[i])*n / (n - 1));//求标准差
		}
	}
}
// CRun 对话框

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


// CRun 消息处理程序

void CRun::RunListReset()
{
  CString strValue, strTemps;
  int itemnum = g_sort.itemnum;
  //清空控件及全局变量
  m_runlist.DeleteAllItems();

  for (int i = 0; i < itemnum; i++)
  {
    strValue = "";
    strTemps.Format(_T("%d"), i + 1);//_0611_原i+1
    m_runlist.InsertItem(i, strTemps);
    strTemps.Format(_T("%02d°"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) / 3600);//度
    strValue += strTemps;
    strTemps.Format(_T("%02d\'"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 3600 / 60);//分
    strValue += strTemps;
    strTemps.Format(_T("%02d\""), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 60);//秒
    strValue += strTemps;
    m_runlist.SetItemText(i, 1, strValue);
    m_runlist.SetItemText(i, 2, _T("0"));
  }

  strTemps.Format(_T("%d"), itemnum + 1); m_runlist.InsertItem(itemnum, strTemps); m_runlist.SetItemText(itemnum, 1, _T("电轴-")); m_runlist.SetItemText(itemnum, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 2); m_runlist.InsertItem(itemnum + 1, strTemps); m_runlist.SetItemText(itemnum + 1, 1, _T("电轴+")); m_runlist.SetItemText(itemnum + 1, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 3); m_runlist.InsertItem(itemnum + 2, strTemps); m_runlist.SetItemText(itemnum + 2, 1, _T("光轴-")); m_runlist.SetItemText(itemnum + 2, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 4); m_runlist.InsertItem(itemnum + 3, strTemps); m_runlist.SetItemText(itemnum + 3, 1, _T("光轴+")); m_runlist.SetItemText(itemnum + 3, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 5); m_runlist.InsertItem(itemnum + 4, strTemps); m_runlist.SetItemText(itemnum + 4, 1, _T("激光检测不出")); m_runlist.SetItemText(itemnum + 4, 2, _T("0"));
  strTemps.Format(_T("%d"), itemnum + 6); m_runlist.InsertItem(itemnum + 5, strTemps); m_runlist.SetItemText(itemnum + 5, 1, _T("角度检测不出")); m_runlist.SetItemText(itemnum + 5, 2, _T("0"));
  m_runlist.InsertItem(itemnum + 6, _T("合计")); m_runlist.SetItemText(itemnum + 6, 1, _T("")); m_runlist.SetItemText(itemnum + 6, 2, _T("0"));

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
	// TODO:  在此添加额外的初始化
	CString strValue, strTemps;
	//变量初始化
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
	g_status._DEBUGEFG = 0;//调试
	g_sort.sortsn = 0;
	g_sort.sortsum = 0;//1~24档总片数
	for (int i = 0; i < 30; i++)//每档已分档片数置零
	{
		g_sort.sortnum[i] = 0;
		g_sort.alertnum[i] = 1;
	}
	for (int i = 0; i < 5; i++)//5轴的平均方差初始化
	{
		g_sort.sortavg[i] = 0;
		g_sort.sortstd[i] = 0;
		g_sort.std2[i] = 0;
	}
	//ini文件初始化或读取
	CString strFilePath, txtFilePath;
	int iniint = 0;
	GetModuleFileName(NULL, strFilePath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strFilePath.ReleaseBuffer();
	int pos = strFilePath.ReverseFind('\\');
	txtFilePath = strFilePath.Left(pos) + _T("\\R1Cor.txt");
	strFilePath = strFilePath.Left(pos) + _T("\\SortSet.ini");

	///////////////
	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	BOOL ifFind = findini.FindFile(strFilePath);
	if (!ifFind)
	{

		AfxMessageBox(_T("SortSet.ini 不存在"));
		exit(0);
		/*
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

	//写入ini文件中相应字段
	/*GetPrivateProfileString(_T("分档设定"), _T("流程卡"), 0, strValue.GetBuffer(MAX_PATH), MAX_PATH, strFilePath);
	strValue.ReleaseBuffer();
	SetDlgItemText(IDC_EDIT_CARD, strValue);*/
	iniint = GetPrivateProfileInt(_T("分档设定"), _T("中心角度：度"), 0, strFilePath) * 10000 +
		GetPrivateProfileInt(_T("分档设定"), _T("中心角度：分"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("分档设定"), _T("中心角度：秒"), 0, strFilePath);
	SetDlgItemInt(IDC_DEG, iniint);

	iniint = GetPrivateProfileInt(_T("分档设定"), _T("分档值：分"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("分档设定"), _T("分档值：秒"), 0, strFilePath);
	SetDlgItemInt(IDC_SORT, iniint);

	iniint = GetPrivateProfileInt(_T("分档设定"), _T("切角值：秒"), 0, strFilePath);
	SetDlgItemInt(IDC_EDT_CUTDEG, iniint);

	iniint = GetPrivateProfileInt(_T("分档设定"), _T("开始值：度"), 0, strFilePath) * 10000 +
		GetPrivateProfileInt(_T("分档设定"), _T("开始值：分"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("分档设定"), _T("开始值：秒"), 0, strFilePath);
	SetDlgItemInt(IDC_DEG_MIN, iniint);
	iniint = GetPrivateProfileInt(_T("分档设定"), _T("结束值：度"), 0, strFilePath) * 10000 +
		GetPrivateProfileInt(_T("分档设定"), _T("结束值：分"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("分档设定"), _T("结束值：秒"), 0, strFilePath);
	SetDlgItemInt(IDC_DEG_MAX, iniint);

	iniint = GetPrivateProfileInt(_T("等效角设定"), _T("T0：度"), 0, strFilePath) * 10000 +
		GetPrivateProfileInt(_T("等效角设定"), _T("T0：分"), 0, strFilePath) * 100 +
		GetPrivateProfileInt(_T("等效角设定"), _T("T0：秒"), 0, strFilePath);
	SetDlgItemInt(IDC_PARAM_T0, iniint);

	iniint = GetPrivateProfileInt(_T("等效角设定"), _T("K"), 0, strFilePath);
	SetDlgItemInt(IDC_PARAM_K, iniint);

	//预设值保存到全局变量
	g_sort.centerangle = GetPrivateProfileInt(_T("分档设定"), _T("中心角度：度"), 0, strFilePath) * 3600 +
		GetPrivateProfileInt(_T("分档设定"), _T("中心角度：分"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("分档设定"), _T("中心角度：秒"), 0, strFilePath);
	g_sort.sortvalue = GetPrivateProfileInt(_T("分档设定"), _T("分档值：分"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("分档设定"), _T("分档值：秒"), 0, strFilePath);
	g_sort.cutvalue = GetPrivateProfileInt(_T("分档设定"), _T("切角值：秒"), 0, strFilePath);
	
	g_sort.eleclow = GetPrivateProfileInt(_T("分档设定"), _T("开始值：度"), 0, strFilePath) * 3600 +
		GetPrivateProfileInt(_T("分档设定"), _T("开始值：分"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("分档设定"), _T("开始值：秒"), 0, strFilePath);
	g_sort.elechigh = GetPrivateProfileInt(_T("分档设定"), _T("结束值：度"), 0, strFilePath) * 3600 +
		GetPrivateProfileInt(_T("分档设定"), _T("结束值：分"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("分档设定"), _T("结束值：秒"), 0, strFilePath);
	g_sort.et0 = GetPrivateProfileInt(_T("等效角设定"), _T("T0：度"), 0, strFilePath) * 3600 +
		GetPrivateProfileInt(_T("等效角设定"), _T("T0：分"), 0, strFilePath) * 60 +
		GetPrivateProfileInt(_T("等效角设定"), _T("T0：秒"), 0, strFilePath);
  g_sort.ek = GetPrivateProfileInt(_T("等效角设定"), _T("K"), 0, strFilePath);
	
	//初始化组合框
	m_sortsel1.AddString(_T("光轴"));
	m_sortsel1.AddString(_T("电轴"));
	m_sortsel1.AddString(_T("等效角度"));
	m_sortsel1.AddString(_T("原始光轴"));
	m_sortsel1.AddString(_T("原始电轴"));
	m_sortsel2.AddString(_T("光轴"));
	m_sortsel2.AddString(_T("电轴"));
	m_sortsel2.AddString(_T("等效角度"));
	//m_sortsel2.AddString(_T("原始光轴"));
	//m_sortsel2.AddString(_T("原始电轴"));
	m_sortsel2.AddString(_T("无"));

	g_sort.sortsel1 = GetPrivateProfileInt(_T("分档设定"), _T("检测角：编号"), 0, strFilePath);
	g_sort.sortsel2 = GetPrivateProfileInt(_T("分档设定"), _T("限定角：编号"), 0, strFilePath);
	m_sortsel1.SetCurSel(g_sort.sortsel1);
	m_sortsel2.SetCurSel(g_sort.sortsel2);
	//初始化Run页listcontrol控件
	CRect listRect;

	m_runlist.GetClientRect(&listRect);
	m_runlist.SetExtendedStyle(m_runlist.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_runlist.InsertColumn(0, _T("档位"), LVCFMT_CENTER, listRect.Width() / 8 * 2, 0);
	m_runlist.InsertColumn(1, _T("角度"), LVCFMT_CENTER, listRect.Width() / 8 * 4, 1);
	m_runlist.InsertColumn(2, _T("片数"), LVCFMT_CENTER, listRect.Width() / 8 * 2, 2);
	//itemnum相关
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
		strTemps.Format(_T("%d"), i + 1);//_0611_原i+1
		m_runlist.InsertItem(i, strTemps);
		strTemps.Format(_T("%02d°"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) / 3600);//度
		strValue += strTemps;
		strTemps.Format(_T("%02d\'"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 3600 / 60);//分
		strValue += strTemps;
		strTemps.Format(_T("%02d\""), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 60);//秒
		strValue += strTemps;
		m_runlist.SetItemText(i, 1, strValue);
		m_runlist.SetItemText(i, 2, _T("0"));
	}

	strTemps.Format(_T("%d"), itemnum + 1); m_runlist.InsertItem(itemnum, strTemps); m_runlist.SetItemText(itemnum, 1, _T("电轴-")); m_runlist.SetItemText(itemnum, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 2); m_runlist.InsertItem(itemnum + 1, strTemps); m_runlist.SetItemText(itemnum + 1, 1, _T("电轴+")); m_runlist.SetItemText(itemnum + 1, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 3); m_runlist.InsertItem(itemnum + 2, strTemps); m_runlist.SetItemText(itemnum + 2, 1, _T("光轴-")); m_runlist.SetItemText(itemnum + 2, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 4); m_runlist.InsertItem(itemnum + 3, strTemps); m_runlist.SetItemText(itemnum + 3, 1, _T("光轴+")); m_runlist.SetItemText(itemnum + 3, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 5); m_runlist.InsertItem(itemnum + 4, strTemps); m_runlist.SetItemText(itemnum + 4, 1, _T("激光检测不出")); m_runlist.SetItemText(itemnum + 4, 2, _T("0"));
	strTemps.Format(_T("%d"), itemnum + 6); m_runlist.InsertItem(itemnum + 5, strTemps); m_runlist.SetItemText(itemnum + 5, 1, _T("角度检测不出")); m_runlist.SetItemText(itemnum + 5, 2, _T("0"));
	m_runlist.InsertItem(itemnum + 6, _T("合计")); m_runlist.SetItemText(itemnum + 6, 1, _T("")); m_runlist.SetItemText(itemnum + 6, 2, _T("0"));
  */

	font.CreatePointFont(800, _T("宋体"), NULL);
	GetDlgItem(IDC_STATIC_SHOW)->SetFont(&font);
	char l_ctmp[MAX_PATH];
	CString csR1;
	for (int m = 0; m < 360; m++)
	{
		if (TXTReadRowStr(txtFilePath, csR1, m))
		{

			WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, csR1, -1, l_ctmp, sizeof(l_ctmp), NULL, NULL);
			//l_ctmp[strlen(l_ctmp)-2]=0;//在文件中读出有/R/N，如果不处理掉/R/N，会导致后面的读取路径会找不到或出错
			char *cTmp;
			cTmp = new char[strlen(l_ctmp)];
			Split(l_ctmp, cTmp, '\t', 1);//r1t
			g_sort.R1t[m] = atoi(cTmp);
			Split(l_ctmp, cTmp, '\t', 2);//r1e
			g_sort.R1e[m] = atoi(cTmp);
			delete[] cTmp; cTmp = NULL;
		}
		else
			AfxMessageBox(_T("R1读取失败，重新打开程序"));
	}
	g_tim.onX = GetPrivateProfileInt(_T("等待时间"), _T("X轴取料"), 0, strFilePath);
	g_tim.offX = GetPrivateProfileInt(_T("等待时间"), _T("X轴上料"), 0, strFilePath);
	g_tim.offZ = GetPrivateProfileInt(_T("等待时间"), _T("Z轴放片"), 0, strFilePath);
	g_tim.onZ = GetPrivateProfileInt(_T("等待时间"), _T("Z轴取料"), 0, strFilePath);

	g_tim.onY = GetPrivateProfileInt(_T("等待时间"), _T("Y轴取料"), 0, strFilePath);
	g_tim.offY = GetPrivateProfileInt(_T("等待时间"), _T("Y轴上料"), 0, strFilePath);
	g_tim.blow = GetPrivateProfileInt(_T("等待时间"), _T("吹气"), 0, strFilePath);
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
	EnableToolTips(TRUE);   //true为打开提示功能，false 为关闭提示功能
	m_toolTip.Create(this, TTS_BALLOON);//创建
	m_toolTip.SetTipTextColor(RGB(0, 0, 255));//设定文字颜色
	m_toolTip.SetTipBkColor(RGB(255, 0, 255));
	m_toolTip.SetDelayTime(0);//设定文字停留时间
	m_toolTip.AddTool(GetDlgItem(IDC_EDIT_TIME_XF), _T("输完不要点设置"));
	m_toolTip.AddTool(GetDlgItem(IDC_EDIT_TIME_ZF), _T("输完不要点设置"));

	TRY
	{
		m_csvAllCard = new CCSVFile(g_path + _T("\\data\\流程卡.csv"), CCSVFile::modeWrite);
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
	// 异常: OCX 属性页应返回 FALSE
}


void CRun::OnBnClickedRunStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strValue = _T("");
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");

	int status = 0;
	/*	if(g_hDevice==INVALID_HANDLE_VALUE)
	{
	AfxMessageBox(_T("ac6641 打开失败"));
	return;
	}*/
	//流程卡记录：by mmy 171115
	//CString card;
	CString planned, process;

	GetDlgItemText(IDC_EDIT_PLANNED, planned);

	if ("" == planned)
	{
		AfxMessageBox(_T("计划号？"));

		return;
	}
	GetDlgItemText(IDC_EDIT_CARD, process);

	if ("" == process)
	{
		AfxMessageBox(_T("流程卡号？"));

		return;
	}
	// 对标准
	if (g_sort.needCheck)
	{
		CDlgCheckStd dlg;
		int ret = dlg.DoModal();
		if (ret != 101)
		{
			if (1 == AfxMessageBox(_T("对标准未通过"), MB_OKCANCEL))
				return;
		}
		g_sort.needCheck = 0;
		CString str;
		GetDlgItemText(IDC_DEG, str);
		WritePrivateProfileString(_T("对标设定"), _T("上一次对标中心角度"), str, strFilePath);

	}
  //流程卡记录：190503复制自BER
  TRY
  {
    if (!m_csvAllCard)
      m_csvAllCard = new CCSVFile(g_path + _T("\\data\\流程卡.csv"), CCSVFile::modeWrite);

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
	//	//AfxMessageBox(_T("流程卡号？"));

	//	status = AfxMessageBox(_T("是否抽检？"), MB_OKCANCEL);
	//	if (status == 2)//取消
	//	{
	//		return;
	//	}
	//	else {}
	//	//AfxMessageBox(_T("请输入流程卡号"));
	//	//return;
	//}
	//else
	{
    // 创建cpk计划号、流程卡, 
    // 190503 复制自BER
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

	//CCSVFile csvCard(g_path + _T("\\data\\流程卡.csv"), CCSVFile::modeWrite);
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
		arr.Add(_T("编号"));
		arr.Add(_T("光轴角度"));
		arr.Add(_T("电轴角度"));
		arr.Add(_T("等效角度"));
		arr.Add(_T("原始光轴"));
		arr.Add(_T("原始电轴"));
		arr.Add(_T("档位"));
		csv.WriteData(arr);
		}

		arr.RemoveAll();
		*/
		//分割
		CString str;
		//
		arr.Add(_T("开始"));
		arr.Add(time.Format("%Y/%m/%d"));
		arr.Add(time.Format("%H:%M:%S"));
		m_csvCard->WriteData(arr);
		arr.RemoveAll();
		//
		arr.Add(_T("分档预设"));
		GetDlgItemText(IDC_SORT_SEL1, str);
		if ("" == str)
		{
			AfxMessageBox(_T("请正确设置"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_DEG, str);
		if ("" == str)
		{
			AfxMessageBox(_T("请正确设置"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_SORT, str);
		if ("" == str)
		{
			AfxMessageBox(_T("请正确设置"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_SORT_SEL2, str);
		if ("" == str)
		{
			AfxMessageBox(_T("请正确设置"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_DEG_MIN, str);
		if ("" == str)
		{
			AfxMessageBox(_T("请正确设置"));
			return;
		}
		arr.Add(str);
		GetDlgItemText(IDC_DEG_MAX, str);
		if ("" == str)
		{
			AfxMessageBox(_T("请正确设置"));
			return;
		}
		arr.Add(str);
    GetDlgItemText(IDC_PARAM_T0, str);
    if ("" == str)
    {
      AfxMessageBox(_T("请正确设置"));
      return;
    }
    arr.Add(str);
    GetDlgItemText(IDC_PARAM_K, str);
    if ("" == str)
    {
      AfxMessageBox(_T("请正确设置"));
      return;
    }
    arr.Add(str);
		/* if (g_sort.islongsqu)
		{
		arr.Add(_T("长方片"));
		}
		else
		{
		arr.Add(_T("方片"));
		}*/
		/*str.Format(_T("%d"), g_sort.islongsqu);
		arr.Add(_T("长方片"));
		arr.Add(str);*/
		m_csvCard->WriteData(arr);
		arr.RemoveAll();
		//
		arr.Add(_T("编号"));
		arr.Add(_T("光轴角度"));
		arr.Add(_T("电轴角度"));
		arr.Add(_T("等效角度"));
		arr.Add(_T("原始光轴"));
		arr.Add(_T("原始电轴"));
		arr.Add(_T("档位"));
		m_csvCard->WriteData(arr);
		arr.RemoveAll();

	}

	GetDlgItem(IDC_RUN_START)->EnableWindow(0);
	AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) & 0xfd);//停止测试低电平
	Sleep(3000);
	status = 0;
	if (g_status._N > 0)
		status = AfxMessageBox(_T("是否继承上一次状态量"), MB_OKCANCEL);
	if (status == 2)//按确定进入
	{
		CTime time;
		time = CTime::GetCurrentTime();
		pathSaveDegree = g_path + (_T("\\check\\SortDegree")) + time.Format("%d-%H-%M") + (_T(".txt"));
		//变量初始化
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
		g_status._DEBUGEFG = 0;//调试
		g_sort.sortsn = 0;
		g_sort.sortsum = 0;//1~24档总片数

		for (int i = 0; i < 30; i++)//每档已分档片数置零
		{
			g_sort.sortnum[i] = 0;
			g_sort.alertnum[i] = 1;
		}
		for (int i = 0; i < 5; i++)//5轴的平均方差初始化
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
	GetDlgItem(IDC_RUN_SHOW)->SetWindowText(_T("系统运行中"));

	//font.CreatePointFont(800,_T("宋体"),NULL);
	//GetDlgItem(IDC_STATIC_SHOW)->SetFont(&font);
	//===================//保存角度到文件
	/*CStdioFile file;
	CFont font;
	font.CreatePointFont(200,_T("宋体"),NULL);
	GetDlgItem(IDC_Static控件名)->SetFont(&font);

	file.Open(pathSaveDegree,CFile::modeCreate|CFile::modeWrite);*/
	CString _degree;
	_degree = _T("编号\t光轴角度\t电轴角度\t等效角度\t原始光轴\t原始电轴\t档位\r\n");
	//_degree.Format(_T("No.\tΘ\tΘ\tΘE\tΘ0\tΘ0\r\n"));
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
		AC6641_SetIoMode(g_hDevice, 0x3F, 0x7);//设模式，对应7-0和11-8组,前六组=1为输出，后六组=0为输入
		AC6641_DO(g_hDevice, 0, 0);
		AC6641_DO(g_hDevice, 1, 0);
		AC6641_DO(g_hDevice, 2, 0);
		AC6641_DO(g_hDevice, 3, 0);
		AC6641_DO(g_hDevice, 4, 0x77);//如0会产生下降沿，导致X/Y同时回零，抖动
		AC6641_DO(g_hDevice, 5, 0);
		AC6641_DO(g_hDevice, 8, 0);
		AC6641_DO(g_hDevice, 9, 0);
		AC6641_DO(g_hDevice, 10, 0x03);
	}
	else
	{
		AC6641_SetIoMode(g_hDevice, 0x3f, 0x0);//设模式，对应7-0和11-8组,前六组=1为输出，后六组=0为输入
		AC6641_DO(g_hDevice, 0, 0);
		AC6641_DO(g_hDevice, 1, 0);
		AC6641_DO(g_hDevice, 2, 0);
		AC6641_DO(g_hDevice, 3, 0);
		AC6641_DO(g_hDevice, 4, 0x77);//如0会产生下降沿，导致X/Y同时回零，抖动
		AC6641_DO(g_hDevice, 5, 0);
	}
	//线程标志清0
	g_status._CHK1 = 0;
	g_status._CHK2 = 0;
	g_status._N = 0;
	g_status._SCREND = 0;
	g_status._SORTTAK = 1;//第一次要直接上片
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
	g_status._RUN = 1;//标记进入运行状态
	GetDlgItem(IDC_RUN_STOP)->EnableWindow(1);
	GetDlgItem(IDC_RUN_PAUSE)->EnableWindow(1);
	//设置区禁止修改
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
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_RUN_SHOW)->SetWindowText(_T("系统停止中"));
	//===============//
	/*CStdioFile file;
	file.Open(pathSaveDegree,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);*/
	CString _sort, _avg, _std;
	for (int i = 0; i < 5; i++)
	{
		CString tmp;
		tmp.Format(_T("%02d°%02d'%02d\""), int((g_sort.sortavg[i]) + 0.5) / 3600, int((g_sort.sortavg[i]) + 0.5) % 3600 / 60, int((g_sort.sortavg[i]) + 0.5) % 60);
		_avg += tmp + _T("\t");
		tmp.Format(_T("%02d°%02d'%02d\""), int((g_sort.sortstd[i]) + 0.5) / 3600, int((g_sort.sortstd[i]) + 0.5) % 3600 / 60, int((g_sort.sortstd[i]) + 0.5) % 60);
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
	while (1)//当全部线程退出之后退出线程
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
	AC6641_DO(g_hDevice, 5, AC6641_DI(g_hDevice, 5) | 0x02);//开始测试高电平
	//流程卡记录
	//CString card;
	//GetDlgItemText(IDC_EDIT_CARD, card);
	if (m_csvCard) {
		//CCSVFile csv(g_path + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);
		CStringArray arr;
		CString str;
		CTime time;
		time = CTime::GetCurrentTime();
		arr.Add(_T("结束"));
		arr.Add(time.Format("%Y/%m/%d"));
		arr.Add(time.Format("%H:%M:%S"));
		m_csvCard->WriteData(arr);
		arr.RemoveAll();

		delete m_csvCard;
		m_csvCard = NULL;
	}
	SetDlgItemText(IDC_EDIT_CARD, _T(""));
	GetDlgItem(IDC_RUN_START)->EnableWindow(1);
	g_status._RUN = 0;//标记进入停止状态
	GetDlgItem(IDC_RUN_STOP)->EnableWindow(0);
	GetDlgItem(IDC_RUN_PAUSE)->EnableWindow(0);
	//设置区禁止修改
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
	// TODO: 在此添加控件通知处理程序代码
	CString strValue = _T("");
	CString strFilePath;
	/*GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	strFilePath.ReleaseBuffer();
	int pos = strFilePath.ReverseFind('\\'); */
	strFilePath = g_path + _T("\\SortSet.ini");

	//判断预设值度分秒是否在0~60之间
	if (((GetDlgItemInt(IDC_DEG, 0, 1) / 10000 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG, 0, 1) / 10000 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_DEG, 0, 1) % 10000 / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG, 0, 1) % 10000 / 100 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_DEG, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_DEG, 0, 1) % 100 >= 0 ? 0 : 1)) != 0)
	{
		AfxMessageBox(_T("坐标有误"));
		return;
	}
	if (((GetDlgItemInt(IDC_SORT, 0, 1) / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_SORT, 0, 1) / 100 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_SORT, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_SORT, 0, 1) % 100 >= 0 ? 0 : 1)) != 0)
	{
		AfxMessageBox(_T("分档值有误"));
		return;
	}
	if (g_sort.sortsel1 == g_sort.sortsel2)
	{
		AfxMessageBox(_T("设定轴冲突"));
		return;
	}
	if (3 != g_sort.sortsel2)//如果有轴限定，追加判断
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
			AfxMessageBox(_T("限定值有误"));
			return;
		}
	}
	//等效角参数检验
	if (((GetDlgItemInt(IDC_PARAM_T0, 0, 1) / 10000 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_PARAM_T0, 0, 1) / 10000 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 10000 / 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 10000 / 100 >= 0 ? 0 : 1) +
		(GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 100 <= 60 ? 0 : 1) + (GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 100 >= 0 ? 0 : 1)) != 0)
	{
		AfxMessageBox(_T("等效角参数phi0有误"));
		return;
	}
	//查看分档角度是否大于切角角度
	int tmpCut = USER_TO_SEC(GetDlgItemInt(IDC_EDT_CUTDEG, 0, 1));
	int tmpSort = USER_TO_SEC(GetDlgItemInt(IDC_SORT, 0, 1));
	if (tmpSort > tmpCut || tmpCut % tmpSort)
	{
		AfxMessageBox(_T("分档角度与切角角度关系不对，分档角小于等于切角，且切角能被分档角整除"));
		return;
	}
	//查看中心是否改变
	int edt_value = USER_TO_SEC(GetDlgItemInt(IDC_DEG, 0, 1));
	int ini_value = USER_TO_SEC(GetPrivateProfileInt(_T("对标设定"), _T("上一次对标中心角度"), 0, strFilePath));

	if (edt_value != ini_value)
	{
		g_sort.needCheck = 1;
		AfxMessageBox(_T("与上次中心角度不一致，需要对标"));
	}
	else
	{
		g_sort.needCheck = 0;
		AfxMessageBox(_T("与上次中心角度一致，不用对标"));
	}

	int usl = edt_value - tmpSort / 2 + tmpCut;
	int lsl = edt_value - tmpSort / 2 - tmpCut;

	CString tmpStr;
	tmpStr.Format(_T("cpk usl : %d lsl : %d"), SEC_TO_USER(usl), SEC_TO_USER(lsl));
	AfxMessageBox(tmpStr);
	//将预设值保存到全局变量
	g_sort.centerangle = GetDlgItemInt(IDC_DEG, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_DEG, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_DEG, 0, 1) % 100;
	g_sort.sortvalue = GetDlgItemInt(IDC_SORT, 0, 1) / 100 * 60 + GetDlgItemInt(IDC_SORT, 0, 1) % 100;
	if (3 != g_sort.sortsel2)//如果电轴限定，追加判断
	{
		g_sort.eleclow = GetDlgItemInt(IDC_DEG_MIN, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 100;
		g_sort.elechigh = GetDlgItemInt(IDC_DEG_MAX, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 100;
	}
	g_sort.et0 = GetDlgItemInt(IDC_PARAM_T0, 0, 1) / 10000 * 3600 + GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 10000 / 100 * 60 + GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 100;
	g_sort.ek = GetDlgItemInt(IDC_PARAM_K, 0, 1);
	
	//保存到ini
	//CString strValue = _T("");
	//CString strFilePath;
	///*GetModuleFileName(NULL,strFilePath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	//strFilePath.ReleaseBuffer();
	//int pos = strFilePath.ReverseFind('\\'); */
	//strFilePath = g_path + _T("\\SortSet.ini");

	CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
	BOOL ifFind = findini.FindFile(strFilePath);
	if (!ifFind)
	{

		AfxMessageBox(_T("SortSet.ini 不存在"));
		exit(0);
		/*WritePrivateProfileString(_T("分档设定"), _T("流程卡"), _T("0"), strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("检测角：编号"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：度"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：分"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("中心角度：秒"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("分档值：分"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("分档值：秒"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("限定角：编号"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("开始值：度"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("开始值：分"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("开始值：秒"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：度"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：分"),_T("0"),strFilePath);
		WritePrivateProfileString(_T("分档设定"),_T("结束值：秒"),_T("0"),strFilePath);*/
	}

	GetDlgItemText(IDC_EDIT_PLANNED, strValue);
	WritePrivateProfileString(_T("分档设定"), _T("计划号"), strValue, strFilePath);


	GetDlgItemText(IDC_EDIT_CARD, strValue);
	WritePrivateProfileString(_T("分档设定"), _T("流程卡"), strValue, strFilePath);
	strValue.Format(_T("%d"), g_sort.sortsel1);
	WritePrivateProfileString(_T("分档设定"), _T("检测角：编号"), strValue, strFilePath);
	//
	strValue.Format(_T("%d"), g_sort.sortsel2);
	WritePrivateProfileString(_T("分档设定"), _T("限定角：编号"), strValue, strFilePath);
	if (3 != g_sort.sortsel2)//如果电轴限定，追加判断
	{
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MIN, 0, 1) / 10000);
		WritePrivateProfileString(_T("分档设定"), _T("开始值：度"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 10000 / 100);
		WritePrivateProfileString(_T("分档设定"), _T("开始值：分"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MIN, 0, 1) % 100);
		WritePrivateProfileString(_T("分档设定"), _T("开始值：秒"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MAX, 0, 1) / 10000);
		WritePrivateProfileString(_T("分档设定"), _T("结束值：度"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 10000 / 100);
		WritePrivateProfileString(_T("分档设定"), _T("结束值：分"), strValue, strFilePath);
		strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG_MAX, 0, 1) % 100);
		WritePrivateProfileString(_T("分档设定"), _T("结束值：秒"), strValue, strFilePath);
	}

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG, 0, 1) / 10000);
	WritePrivateProfileString(_T("分档设定"), _T("中心角度：度"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG, 0, 1) % 10000 / 100);
	WritePrivateProfileString(_T("分档设定"), _T("中心角度：分"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_DEG, 0, 1) % 100);
	WritePrivateProfileString(_T("分档设定"), _T("中心角度：秒"), strValue, strFilePath);

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_SORT, 0, 1) / 100);
	WritePrivateProfileString(_T("分档设定"), _T("分档值：分"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_SORT, 0, 1) % 100);
	WritePrivateProfileString(_T("分档设定"), _T("分档值：秒"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_EDT_CUTDEG, 0, 1) % 100);
	WritePrivateProfileString(_T("分档设定"), _T("切角值：秒"), strValue, strFilePath);

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_PARAM_T0, 0, 1) / 10000);
	WritePrivateProfileString(_T("等效角设定"), _T("T0：度"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 10000 / 100);
	WritePrivateProfileString(_T("等效角设定"), _T("T0：分"), strValue, strFilePath);
	strValue.Format(_T("%d"), GetDlgItemInt(IDC_PARAM_T0, 0, 1) % 100);
	WritePrivateProfileString(_T("等效角设定"), _T("T0：秒"), strValue, strFilePath);

	strValue.Format(_T("%d"), GetDlgItemInt(IDC_PARAM_K, 0, 1));
	WritePrivateProfileString(_T("等效角设定"), _T("K"), strValue, strFilePath);

	//itemnum相关
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

	//清空控件及全局变量
	/*m_runlist.DeleteAllItems();
	CString sortValue, strTemps;
	for (int i = 0; i < itemnum; i++)
	{
		sortValue.Format(_T("%d"), i + 1);//_0611_原i+1
		m_runlist.InsertItem(i, sortValue);
		sortValue = "";

		strTemps.Format(_T("%02d°"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) / 3600);
		sortValue += strTemps;
		strTemps.Format(_T("%02d\'"), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 3600 / 60);
		sortValue += strTemps;
		strTemps.Format(_T("%02d\""), (g_sort.centerangle + g_sort.sortvalue*(i - itemnum / 2)) % 60);
		sortValue += strTemps;
		m_runlist.SetItemText(i, 1, sortValue);
		m_runlist.SetItemText(i, 2, _T("0"));
		g_sort.sortnum[i] = 0;
	}
	strTemps.Format(_T("%d"), itemnum + 1); m_runlist.InsertItem(itemnum, strTemps); m_runlist.SetItemText(itemnum, 1, _T("电轴-")); m_runlist.SetItemText(itemnum, 2, _T("0")); g_sort.sortnum[24] = 0;
	strTemps.Format(_T("%d"), itemnum + 2); m_runlist.InsertItem(itemnum + 1, strTemps); m_runlist.SetItemText(itemnum + 1, 1, _T("电轴+")); m_runlist.SetItemText(itemnum + 1, 2, _T("0")); g_sort.sortnum[25] = 0;
	strTemps.Format(_T("%d"), itemnum + 3); m_runlist.InsertItem(itemnum + 2, strTemps); m_runlist.SetItemText(itemnum + 2, 1, _T("光轴-")); m_runlist.SetItemText(itemnum + 2, 2, _T("0")); g_sort.sortnum[26] = 0;
	strTemps.Format(_T("%d"), itemnum + 4); m_runlist.InsertItem(itemnum + 3, strTemps); m_runlist.SetItemText(itemnum + 3, 1, _T("光轴+")); m_runlist.SetItemText(itemnum + 3, 2, _T("0")); g_sort.sortnum[27] = 0;
	strTemps.Format(_T("%d"), itemnum + 5); m_runlist.InsertItem(itemnum + 4, strTemps); m_runlist.SetItemText(itemnum + 4, 1, _T("激光检测不出")); m_runlist.SetItemText(itemnum + 4, 2, _T("0")); g_sort.sortnum[28] = 0;
	strTemps.Format(_T("%d"), itemnum + 6); m_runlist.InsertItem(itemnum + 5, strTemps); m_runlist.SetItemText(itemnum + 5, 1, _T("角度检测不出")); m_runlist.SetItemText(itemnum + 5, 2, _T("0")); g_sort.sortnum[29] = 0;
	m_runlist.InsertItem(itemnum + 6, _T("合计")); m_runlist.SetItemText(itemnum + 6, 1, _T("")); m_runlist.SetItemText(itemnum + 6, 2, _T("0")); g_sort.sortnum[30] = 0;

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
	// TODO: 在此添加控件通知处理程序代码
	g_sort.sortsel1 = m_sortsel1.GetCurSel();

}


void CRun::OnCbnSelchangeSortSel2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sort.sortsel2 = m_sortsel2.GetCurSel();
}


void CRun::OnBnClickedRunPause()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_status.SYS_PAUSE)//暂停
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
		GetDlgItem(IDC_RUN_PAUSE)->SetWindowText(_T("暂停"));
		GetDlgItem(IDC_RUN_STOP)->EnableWindow(1);
	}
	else
	{
		g_status.SYS_PAUSE = 1;
		//pauseT1=CTime::GetCurrentTime();
		//g_time.pause_start=pauseT1.GetHour()*3600+pauseT1.GetMinute()*60+pauseT1.GetSecond();
		GetDlgItem(IDC_RUN_PAUSE)->SetWindowText(_T("继续"));
		GetDlgItem(IDC_RUN_STOP)->EnableWindow(0);
	}
}


void CRun::OnBnClickedStartFt()
{
	// TODO: 在此添加控件通知处理程序代码
	//StartFT();
	StartFTLong(1);
	//MyMsgBoxReadIo("it'syou");
	/*MyMsgBoxReadIo2("it'syou");
	MyMsgBoxReadIo2("it's he");
	MyMsgBoxReadIo("heello");*/
}


//void CRun::OnEnChangeEditbs()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//	UpdateData(1);
//	i_bs=i_edit_bs;
//}


//查找一行，并分析行头是否是要找的序号
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

	BOOL rowend = 0;//一行读完标记
	UINT l_i = 0;		//在buf中取到第几个字符
	int itmp = 0;		//用于保存一行字符串

	while (l_i < l_dword_len)
	{
		while (!rowend && l_i < l_dword_len)//如果一行读取没完毕，读完一行存起来并交给后面分析
		{
			if (l_char_buf[l_i] == '\n')
				if (l_char_buf[l_i - 1] == '\r')
				{
					rowend = 1;

				}
				l_char_tmp[itmp++] = l_char_buf[l_i++];
		}
		if (atoi(l_char_tmp) == row)//如果序号是当前要找的
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

//按字符分割符截取需要的字符串
//iSub从0开始，0是第一个字符串，1第二个
//返回0，没找到
//返回1，成功
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
	// TODO: 在此添加控件通知处理程序代码

	DWORD width = RGBBitmap1.bmiHeader.biWidth;
	DWORD height = -RGBBitmap1.bmiHeader.biHeight;

	pDest.pdata = new TARGB32[width*height];//b,g,r,a  4个8位组成一个像素
	pDest.height = height;
	pDest.width = width;
	pDest.byte_width = width << 2;

	g_height = height;
	g_width = width;
	g_lpGray = new BYTE[g_height*g_width];

	int iW = width, iH = height;
	DECODE_RGB_TO_BGRA((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);
	SaveImageToFile(_T("screen_photo\\yuanshi.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存采集图像

	DECODE_RGB_TO_GRAY((const TUInt8 *)pCaptureBuffer.LpVoidBuffer, pDest);//转灰度:输入lpVoidBuffer，输出pDest

	SaveImageToFile(_T("screen_photo\\huidu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存灰度图像
	DECODE_THRESHOLD(pDest, pDest, g_lpGray, m_VGAthreshold);//转二值化，pDest里存的是0-255的二值化图片，lpGray存的是0-1的二值化数据

	SaveImageToFile(_T("screen_photo\\erzhihu.bmp"), iW, iH, (BYTE *)pDest.pdata);//保存二值化图像
	delete[] pDest.pdata;
	pDest.pdata = NULL;
	LoadMark(g_path + _T("\\check\\qz_42.txt"));

	FindOrigin(g_lpGray);
	if (height_num > 100 || width_num > 200 || (height_num == 0 && width_num == 0))
	{

		AfxMessageBox(_T("没有找到图像标记,请重新设定阈值"));
		delete[]g_lpGray;
		g_lpGray = NULL;
		//GetDlgItem(IDC_COMPARE)->EnableWindow(0);
		return;
		//delete []g_lpGray;
		//g_lpGray=NULL;
	}

	g_num = new int[10 * 10 * 8];
	GetNumber();

	ScanDegreeDynamic(g_lpGray, g_path + _T("\\check\\positionDynamic.txt"));//获得degree[]
	g_sort.R1Num = DOUBLE(degree[0] * 100 + degree[1] * 10 + degree[2] + degree[3] * 0.1 + degree[4] * 0.01 + degree[5] * 0.001);

	ScanDegree(g_lpGray, g_path + _T("\\check\\positionstand.txt"));//获得degree[]
	delete[]g_num;
	delete[]g_lpGray;
	g_lpGray = NULL;
	g_num = NULL;
	vga_critical.Unlock();


	g_sort.sortsn = SortChipR1(&g_sort, degree);//得到档位值
	if (g_sort.sortsn <= 0)
		g_sort.sortsn = g_sort.itemnum + 6;
	g_status._SCREND = 1;//置位标记，供分档线程


	CString _degree, _sort, _avg, _std, _r1;
	_degree.Format(_T("光轴角度：%d\r\n电轴角度：%d\r\n等效角度：%d\r\n原始光轴：%d\r\n原始电轴：%d"), degree[0] * 100000 + degree[1] * 10000 + degree[2] * 1000 + degree[3] * 100 + degree[4] * 10 + degree[5],
		degree[6] * 100000 + degree[7] * 10000 + degree[8] * 1000 + degree[9] * 100 + degree[10] * 10 + degree[11],
		degree[12] * 100000 + degree[13] * 10000 + degree[14] * 1000 + degree[15] * 100 + degree[16] * 10 + degree[17],
		degree[18] * 100000 + degree[19] * 10000 + degree[20] * 1000 + degree[21] * 100 + degree[22] * 10 + degree[23],
		degree[24] * 100000 + degree[25] * 10000 + degree[26] * 1000 + degree[27] * 100 + degree[28] * 10 + degree[29]);
	GetDlgItem(IDC_SHOW_AVG)->SetWindowText(_degree);
	//_0614
	_r1.Format(_T("R1:%d\r\nR1光轴：%d\r\nR1电轴：%d"), g_sort.R1Num, g_sort.R1t[g_sort.R1Num], g_sort.R1e[g_sort.R1Num]);
	GetDlgItem(IDC_SHOW_R1)->SetWindowText(_r1);
	int tmpDeg[2];
	tmpDeg[0] = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5]) + g_sort.R1t[g_sort.R1Num];
	tmpDeg[1] = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11]) + g_sort.R1e[g_sort.R1Num];
	_degree.Format(_T("修正光轴：%d\r\n修正电轴：%d"),
		tmpDeg[0] / 3600 * 10000 + tmpDeg[0] % 3600 / 60 * 100 + tmpDeg[0] % 60, tmpDeg[1] / 3600 * 10000 + tmpDeg[1] % 3600 / 60 * 100 + tmpDeg[1] % 60);
	GetDlgItem(IDC_SHOW_DEGCOR)->SetWindowText(_degree);
}


void CRun::OnEnChangeEditTimeX()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.onX = GetDlgItemInt(IDC_EDIT_TIME_X, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.onX);
	WritePrivateProfileString(_T("等待时间"), _T("X轴取料"), strValue, strFilePath);
}


void CRun::OnBnClickedChkLongSqu()
{
	// TODO: 在此添加控件通知处理程序代码
	g_sort.islongsqu = ((CButton*)GetDlgItem(IDC_CHK_LONG_SQU))->GetCheck();
	CString csTmp = _T("");
	csTmp.Format(_T("%d"), g_sort.islongsqu);
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	WritePrivateProfileString(_T("分档设定"), _T("长方片"), csTmp, strFilePath);
	g_sort.chgsqu = 1;

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
}


void CRun::OnBnClickedEndFt()
{
	// TODO: 在此添加控件通知处理程序代码
	StartFTLong(0);
	//AlertCtrl(false);
}


void CRun::OnEnChangeEditTimeZf()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.offZ = GetDlgItemInt(IDC_EDIT_TIME_ZF, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.offZ);
	WritePrivateProfileString(_T("等待时间"), _T("Z轴放片"), strValue, strFilePath);
}


BOOL CRun::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
		{
			//判断是否按下键盘Enter键
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CRun::OnEnChangeEditTimeXf()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.offX = GetDlgItemInt(IDC_EDIT_TIME_XF, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.offX);
	WritePrivateProfileString(_T("等待时间"), _T("X轴上料"), strValue, strFilePath);
}


void CRun::OnEnChangeEditTimeY()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.onY = GetDlgItemInt(IDC_EDIT_TIME_Y, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.onY);
	WritePrivateProfileString(_T("等待时间"), _T("Y轴取料"), strValue, strFilePath);
}


void CRun::OnEnChangeEditTimeYf()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.offY = GetDlgItemInt(IDC_EDIT_TIME_YF, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.offY);
	WritePrivateProfileString(_T("等待时间"), _T("Y轴上料"), strValue, strFilePath);
}


void CRun::OnEnChangeEditTimeBlow()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.blow = GetDlgItemInt(IDC_EDIT_TIME_BLOW, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.blow);
	WritePrivateProfileString(_T("等待时间"), _T("吹气"), strValue, strFilePath);
}

void CRun::OnEnChangeEditTimeX3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

}


void CRun::OnEnChangeEditTimeZ()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");
	g_tim.onZ = GetDlgItemInt(IDC_EDIT_TIME_Z, 0, 0);
	CString strValue;
	strValue.Format(_T("%d"), g_tim.onZ);
	WritePrivateProfileString(_T("等待时间"), _T("Z轴取料"), strValue, strFilePath);
}

void CRun::OnBnClickedBtnPrint()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
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

				pageHead.Format(_T("测量结果"));

				printed.TextOut(xStep * 5, yStep * 1, pageHead); //打印页眉

				CString title;//设置标题栏

				title.Format(_T("序号     角度     数量"));

				printed.TextOut(xl, yStep * 2, title); //打印标题

				printed.TextOut(xr, yStep * 2, title); //打印标题

				CString stt;

				stt.Format(_T("________________________________"));

				printed.TextOut(xl, yStep * 2 + sizeText.cy, stt); //打印stt

				printed.TextOut(xr, yStep * 2 + sizeText.cy, stt); //打印stt

				for (i = 1; (i < 40 / 2) && (row < count / 2); i++) //打印list
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
				pageBottom.Format(_T("共%d页   第%d页"), page, j);

				printed.TextOut(xStep * 5, yMax - yStep, pageBottom);// 打印页脚

				printed.EndPage();//此页结束
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
	// TODO: 在此添加控件通知处理程序代码
	CString strValue = _T("");
	CString strFilePath;
	strFilePath = g_path + _T("\\SortSet.ini");

	CDlgCheckStd dlg;
	int ret = dlg.DoModal();
	if (ret != 101)
	{
		AfxMessageBox(_T("对标准未通过"));
		return;
	}
	g_sort.needCheck = 0;
	CString str;
	GetDlgItemText(IDC_DEG, str);
	WritePrivateProfileString(_T("对标设定"), _T("上一次对标中心角度"), str, strFilePath);

	return;
}


void CRun::OnBnClickedBtnCpk()
{
  // TODO: 在此添加控件通知处理程序代码
  CDlgCpk dlg(g_pCpk);
  dlg.DoModal();

  return;
}
