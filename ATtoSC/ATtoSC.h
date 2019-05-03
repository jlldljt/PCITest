
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
#include "DlgCheckStd.h"
#include "DlgCpk.h"


#define USER_TO_SEC(deg) ((deg)/10000*3600+(deg)%10000/100*60+(deg)%100)
#define DEG_TO_USER(deg) (SEC_TO_USER(DEG_TO_SEC(deg)))
#define SEC_TO_USER(sec) (((int)sec)/3600*10000+((int)sec)%3600/60*100+((int)sec)%60)

#define USER_TO_DEG(deg) ((deg)/10000+(deg)%10000/100/60.0+(deg)%100/3600.0)
#define DEG_TO_SEC(deg) ((int)((deg)*3600))
#define SEC_TO_DEG(sec) ((sec)/3600.0)
//��Ķȷ���
#define S_DEG(sec) ((sec)/3600)
#define S_MIN(sec) ((sec)%3600/60)
#define S_SEC(sec) ((sec)%60)
//�ȵĶȷ���
#define D_DEG(deg) S_DEG((int)((deg)*3600))
#define D_MIN(deg) S_MIN((int)((deg)*3600))
#define D_SEC(deg) S_SEC((int)((deg)*3600))
//�û�����Ķȷ���
#define U_DEG(user) ((user)/10000)
#define U_MIN(user) ((user)%10000/100)
#define U_SEC(user) ((user)%100)

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
	int centerangle,sortvalue,eleclow,elechigh, cutvalue;//Ԥ��ֵ
	int et0,ek;//�����Ч�ǵĲ��� 180414
	unsigned int sortnum[30],sortsum,sortsn,alertnum[30];//sortsum1~24��Ƭ����sortsn�ֵ�ֵ,sortsum�Ѳ��1~24����Ƭ��
	double sortavg[5],sortstd[5];//��ȡ1~24����ƽ��ֵ����׼ƫ��
	int sortsel1,sortsel2;//��ǲ����ἰ�޶���
	long double std2[5];//�����׼ƫ���м�ֵ
	int checkout0,nocheckout;//���0�Ĵ��������������ܲ��
	int R1t[360],R1e[360],R1Num;//R1������������������
	//CString R1tmp;
	bool needCheck;
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
extern CTXT gclsTxt;
extern CCpkLib* g_pCpk;

extern void AlertCtrl(bool stat);//���ƾ���


int ShakePlatformUp();

int ShakePlatformDown();

void CalcAvgStd(const int sum, const double angle, double& avg, double& std, double& std2);