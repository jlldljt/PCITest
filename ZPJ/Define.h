#ifndef _DEFINE_H_
#define _DEFINE_H_

#define MAXFILENAMELEN		256
//#define DEBUG_MODE///
//#define DEBUG_SCREEN
//#define DEBUG_COM
/*
#pragma comment(lib,"ws2_32.lib")

using namespace std;

#define  PORT 8017
#define  IP_ADDRESS "192.168.1.118"*/

enum{
	READ_COM = 1,
	SHOW_WINDOWS,
	RESEND_COM,
};
enum fy{
	d1 = 262,
	d1_ = 277,
	d2 = 294,
	d2_ = 311,
	d3 = 330,
	d4 = 349,
	d5 = 392,
	d5_= 415,
	d6 = 440,
	d6_= 466,
	d7 = 494,
	z1 = 523,
	z1_ = 554,
	z2 = 578,
	z2_ = 622,
	z3 = 659,
	z4 = 698,
	z4_ = 740,
	z5 = 784,
	z5_ = 831,
	z6 = 880,
	z6_ = 932,
	z7 = 988,
	g1 = 1046,
	g1_ = 1109,
	g2 = 1175,
	g2_ = 1245,
	g3 = 1318,
	g4 = 1397,
	g4_ = 1480,
	g5 = 1568,
	g5_ = 1661,
	g6 = 1760,
	g6_ = 1865,
	g7 = 1976,
	yaya = 0
};

typedef struct yf
{
	enum fy s;
	int t;
}yf;
//�������ݶ���
typedef struct RunInfo
{
	BOOL bTrdExit;								// 	�����˳�������0��1����׼���˳���ָʾ�����߳������˳�
	UINT unTrdExitNum;							//	���߳��˳�ʱ��ָʾ�˳����߳���
	BOOL bTrdPause;								// 	�߳��˳�������0��1����׼��ֹͣ��ָʾ�ⲿ��ť����߳������˳�
	UINT unStatRun;								// 	����״̬��run_state����0��0(ֹͣ״̬)��1(��ͣ״̬)��2(����״̬)
	BOOL bOverRcg;								// 	ʶ����ɣ�Image_recognition_over����1��1��ʾ���ʶ����ɿ���ȡƬ
	BOOL bOverCam;								//	�������ɼ�ͼ�����
	//BOOL s_bool_start_imgrecognition;			// 	ʶ��ʼ��Image_recognition_start����0��1ָʾ��Ƭ׼���ã�ʶ��������Կ�ʼ
	char chStmCmd;								//	Stm���ڷ���
	char chStmCmd0;                             //  stm����0����δ����ʱ��������ź�
	char chCmd[7];								//	��������
	char chTmYMD[20];							//	�����������ļ��еĽ���
	char chTmHMS[10];							//	����ʱ����ͼƬ��ʼʱ��ı���
	UINT unCntResend;							//	�ط������������ж��Ƿ�ʱ
	double dRT;									//	����ʱ��
	bool bExit;									//	�Ƿ���Ҫ�˳�
	UINT unCntStart;							//	��¼��ʼ������
	unsigned int unSort[3];						//	��¼��������ηֵ�ֵ
	BOOL bIsSelFt;								//	��¼�Ƿ���ѡ������Ƭ
	bool bRunStatChg;							//	��¼�Ƿ������б仯�¼�	
	int bPosSet;								//	PosSet��ǣ�1stm������XYλ��2�����û�Ƭλ��
	int nEfgStaSel;								//	EFG��ʼ���λ��ѡ����
	int runCnt;
	DWORD timRun[10][2];				    //  ����ʱ���¼
	bool closeShake;
}RunInfo;

//ˢ�»���ṹ
typedef struct RefreshTmp
{
	CString s_cstr_recv;						//	���������ʾ�ַ���
	UINT unStatRun;								//	����״̬
	UINT unN;									//	ͼ��ɼ�����
	double dRT;									//	����ʱ��
	//UINT s_uint_t;							//  ������Ҫ�����������͵�����
	char chID;									//	���������id��ÿ����ͬ��һ�����id++
	bool bComUpdate;							//	����Ƿ���Ҫ����Com����
	int alarm;
}RefreshTmp;

//txt������ؽṹ
typedef struct PathInfo
{
	CString csPathIni,csPathTxt;	//	txtĿ¼
	CString csPathExe;
	CString csPathLog;
}PathInfo;
//������ݶ���
// typedef struct IIICInfo
// {
// 	LONG glCamID;
// 	VehiclePassRec gstuCamResult;
// }IIICInfo;
typedef struct RecognitionInfo
{
	bool bChkPN;//�Ƿ���������
	int nPN;//��������ƥ��ķ���
	int nPToL;//ֱ�ߵ��
	int nDefectPToL;//ȱ�ݵ��
	//����������ֱ�߲�ȷ����ֱ����ĵ�
	int nThreshold;//��ֵ
	bool bThrdAuto;//�Զ���ֵ��ʶ
	bool bDelNoise;//ȥ���ʶ
	bool bDebug;//ָʾ�Ƿ���DEBUG//�ٶȺ���
  bool bOnce;// ֻȡһƬ
	int nAllowDefect;//���������������ж�ʶ����ĳ����Ƿ����Ҫ��
	int nLength,nWide;//������������
	bool bIsCir;
	double g_factor[2][3];//������Ļ����ת������е�������ϵ����ת����ʽΪX=g_factor[0][0]*x+g_factor[0][1]*y+g_factor[0][2];Y=g_factor[1][0]*x+g_factor[1][1]*y+g_factor[1][2];
	double Xxy[3][4],Yxy[3][4];//��������У׼
	int Nxy;//��������У׼
	int nClbPosX[3],nClbPosY[3];//���ڱ�������У׼�Ĺ̶�λ��
	bool bClbPos;
	int nSNCHGX,nSNCHGY;//xy�ỻ��ͷλ��
}RecognitionInfo;

struct threadstatus//�߳�״̬ȫ�ֽṹ
{
	int _N,_RUN;//run�������״̬��n����Ѽ������
	bool _SHOW;//show���ִ�н���ˢ�²���
	bool _CHK1,_CHK2,_SCREND,_SORTTAK;//chk12��EFG����ʱ������־������Ļ�ͷֵ��߳�,SCREND��ʾ��Ļ��ȡ���,sorttak�ֵ���ȡ��
	//double _X,_Y,_TOTAL,_AVG,_TN,lasttime,maxtime,mintime;//ģ���е������
	bool TAK_STOP,SOT_STOP,SCR_STOP,DSP_STOP,CHK_STOP,SYS_PAUSE;//���߳�ֹͣ���
	unsigned int Xwait,Xcheck,Ywait,Ysort[30];
	bool _DEBUGEFG;//ģ��EFG��ʼ����ź�
	UINT unProcNum;//��¼CPU��
	//clock_t _START,_TMP,_END;
};

struct sortchip//�ֵ�ȫ�ֽṹ
{
	int centerangle,sortvalue,eleclow,elechigh,cutvalue;//Ԥ��ֵ
	int phi0, factor;//�����Ч�ǵĲ��� DATE 180414
	unsigned int sortnum[30],sortsum,sortsn,alertnum[30];//sortsum1~24��Ƭ����sortsn�ֵ�ֵ,sortsum�Ѳ��1~24����Ƭ��
	double sortavg[5],sortstd[5];//��ȡ1~24����ƽ��ֵ����׼ƫ��
	int sortsel1,sortsel2;//��ǲ����ἰ�޶���
	long double std2[5];//�����׼ƫ���м�ֵ
	int checkout0,nocheckout;//���0�Ĵ��������������ܲ��
	bool needCheck;
};

//ע������ṹ������Ʋ������ģ�ֻ��ӭ��ԭ�ȿؼ������ƣ������ô���Ҫ��device.hͷ��#define
typedef struct tagNpcInf
{
  int var_x1, var_x2, var_x3;
  int var_y1, var_y2, var_y3;
  int var7, var8, var9, var10;
  int var_sortup, var_sortin, var_sortout, var_sortdown;

  int offset_x1, offset_y1;//xy���ƫ�Ʋ���<��Ȧ
  int offset_x2, offset_y2;//xy���ƫ�Ʋ���>��Ȧ
  int offset_p, offset_n;//
  int left, top, right, bottom;//��ѡ��Χ
}NpcInf;

typedef struct tagNpcParm
{
  double x0, y0, deg0, pn0;//��������Ķ�Ӧͼ���ϵ�λ��
  int x, y, deg, pn;//��Ӧ��ʵ�ʵ������
}NpcParm;

enum RUN_STAT {
  RUN_STAT_STOP = 0,
  RUN_STAT_PAUSE,
  RUN_STAT_RUN
};

#endif

