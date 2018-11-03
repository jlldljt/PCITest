
// ATtoSC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#pragma comment(lib,"AC6641.lib")
#include"AC6641.h"

#define ShakeUp ShakePlatformUp()//!(AC6641_DI(g_hDevice,6) & 0x04)//3.4.5!
#define ShakeDown ShakePlatformDown()//(AC6641_DI(g_hDevice,6) & 0x04)//6!
#define TBLOW AC6641_DO(g_hDevice,5,AC6641_DI(g_hDevice,5) | 0x20)  //����֧�ܵ���
#define TNOBLOW AC6641_DO(g_hDevice,5,AC6641_DI(g_hDevice,5) & 0xdf) 

//csv
#include "CSVFile.h"
typedef struct
{
	BITMAPINFOHEADER bmiHeader;	
	DWORD       colorMark[3];	//���ڵ�ǰ֧�ֵ���32λ����ģʽ��������Ҫ3��������DWORD���ֱ��ź죬�̣���������ο�MSDN
}  RGBBITMAP, *PRGBBITMAP;
#include "DlgMsg.h"
#include <math.h>
struct threadstatus//�߳�״̬ȫ�ֽṹ
{
	int _N,_RUN;//run�������״̬��n����Ѽ������
	bool _SHOW;//show���ִ�н���ˢ�²���
	bool _CHK1,_CHK2,_SCREND,_SORTTAK;//chk12��EFG����ʱ������־������Ļ�ͷֵ��߳�,SCREND��ʾ��Ļ��ȡ���,sorttak�ֵ���ȡ��
	//double _X,_Y,_TOTAL,_AVG,_TN,lasttime,maxtime,mintime;//ģ���е������
	bool TAK_STOP,SOT_STOP,SOT2_STOP,SCR_STOP,DSP_STOP,CHK_STOP,CY_STOP,SYS_PAUSE;//���߳�ֹͣ���
	unsigned int Xwait,Xcheck,Ywait,Ysort[30],Ypass,Zsort[30];
	bool _DEBUGEFG;//ģ��EFG��ʼ����ź�
	int SORT_END;//�ֵ���ȡƬ���
	//clock_t _START,_TMP,_END;
	bool _DEBUGSCR;//����Ļģ��
	int CyStat,CyBlock;
  int _MOTOR_NUM;//�����
};
struct sortchip//�ֵ�ȫ�ֽṹ
{
	bool islongsqu , chgsqu;//ָ���Ƿ񳤷�Ƭ
	int itemnum;//�涨���ٵ�λ
	CString itemstr;
	int centerangle,sortvalue,eleclow,elechigh;//Ԥ��ֵ
	int et0,ek;//�����Ч�ǵĲ��� 180414
	unsigned int sortnum[30],sortsum,sortsn,alertnum[30];//sortsum1~24��Ƭ����sortsn�ֵ�ֵ,sortsum�Ѳ��1~24����Ƭ��
	double sortavg[5],sortstd[5];//��ȡ1~24����ƽ��ֵ����׼ƫ��
	int sortsel1,sortsel2;//��ǲ����ἰ�޶���
	long double std2[5];//�����׼ƫ���м�ֵ
	int checkout0,nocheckout;//���0�Ĵ��������������ܲ��
	int R1t[360],R1e[360],R1Num;//R1������������������
	//CString R1tmp;
};
struct stuTime//�ֵ�ȫ�ֽṹ
{
	int onX,onY,offX,offY,onZ,offZ,blow;//Ԥ��ֵ
	//CString R1tmp;
};
/*struct runtime
{
	unsigned long start,end,sum,pause_start,pause_end,pause_sum;
	double avg;
};*/
// CATtoSCApp:
// �йش����ʵ�֣������ ATtoSC.cpp
//

class CATtoSCApp : public CWinApp
{
public:
	CATtoSCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CATtoSCApp theApp;
extern CString g_path;
extern CString pathSaveR1;
extern CString pathSaveDegree;
extern stuTime g_tim;
extern HANDLE g_hDevice;//AC6641���
extern int g_shakeE;



extern void AlertCtrl(bool stat);//���ƾ���


int ShakePlatformUp();

int ShakePlatformDown();
