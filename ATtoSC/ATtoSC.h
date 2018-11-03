
// ATtoSC.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#pragma comment(lib,"AC6641.lib")
#include"AC6641.h"

#define ShakeUp ShakePlatformUp()//!(AC6641_DI(g_hDevice,6) & 0x04)//3.4.5!
#define ShakeDown ShakePlatformDown()//(AC6641_DI(g_hDevice,6) & 0x04)//6!
#define TBLOW AC6641_DO(g_hDevice,5,AC6641_DI(g_hDevice,5) | 0x20)  //三孔支架倒吹
#define TNOBLOW AC6641_DO(g_hDevice,5,AC6641_DI(g_hDevice,5) & 0xdf) 

//csv
#include "CSVFile.h"
typedef struct
{
	BITMAPINFOHEADER bmiHeader;	
	DWORD       colorMark[3];	//由于当前支持的是32位像素模式，所以需要3个单独的DWORD来分别存放红，绿，蓝，具体参考MSDN
}  RGBBITMAP, *PRGBBITMAP;
#include "DlgMsg.h"
#include <math.h>
struct threadstatus//线程状态全局结构
{
	int _N,_RUN;//run标记运行状态，n标记已检测数量
	bool _SHOW;//show标记执行界面刷新操作
	bool _CHK1,_CHK2,_SCREND,_SORTTAK;//chk12当EFG测完时，发标志给读屏幕和分档线程,SCREND表示屏幕读取完毕,sorttak分档臂取走
	//double _X,_Y,_TOTAL,_AVG,_TN,lasttime,maxtime,mintime;//模拟机械手坐标
	bool TAK_STOP,SOT_STOP,SOT2_STOP,SCR_STOP,DSP_STOP,CHK_STOP,CY_STOP,SYS_PAUSE;//各线程停止标记
	unsigned int Xwait,Xcheck,Ywait,Ysort[30],Ypass,Zsort[30];
	bool _DEBUGEFG;//模拟EFG开始检测信号
	int SORT_END;//分档臂取片完毕
	//clock_t _START,_TMP,_END;
	bool _DEBUGSCR;//读屏幕模拟
	int CyStat,CyBlock;
  int _MOTOR_NUM;//电机数
};
struct sortchip//分档全局结构
{
	bool islongsqu , chgsqu;//指定是否长方片
	int itemnum;//规定多少档位
	CString itemstr;
	int centerangle,sortvalue,eleclow,elechigh;//预设值
	int et0,ek;//计算等效角的参数 180414
	unsigned int sortnum[30],sortsum,sortsn,alertnum[30];//sortsum1~24档片数，sortsn分档值,sortsum已测出1~24档总片数
	double sortavg[5],sortstd[5];//存取1~24档的平均值及标准偏差
	int sortsel1,sortsel2;//标记测量轴及限定轴
	long double std2[5];//计算标准偏差中间值
	int checkout0,nocheckout;//测出0的次数（连续，不能测出
	int R1t[360],R1e[360],R1Num;//R1光轴修正，电轴修正
	//CString R1tmp;
};
struct stuTime//分档全局结构
{
	int onX,onY,offX,offY,onZ,offZ,blow;//预设值
	//CString R1tmp;
};
/*struct runtime
{
	unsigned long start,end,sum,pause_start,pause_end,pause_sum;
	double avg;
};*/
// CATtoSCApp:
// 有关此类的实现，请参阅 ATtoSC.cpp
//

class CATtoSCApp : public CWinApp
{
public:
	CATtoSCApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CATtoSCApp theApp;
extern CString g_path;
extern CString pathSaveR1;
extern CString pathSaveDegree;
extern stuTime g_tim;
extern HANDLE g_hDevice;//AC6641句柄
extern int g_shakeE;



extern void AlertCtrl(bool stat);//控制警报


int ShakePlatformUp();

int ShakePlatformDown();
