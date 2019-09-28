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
//运行数据定义
typedef struct RunInfo
{
	BOOL bTrdExit;								// 	程序退出（）：0；1程序准备退出，指示各个线程任务退出
	UINT unTrdExitNum;							//	当线程退出时，指示退出的线程数
	BOOL bTrdPause;								// 	线程退出（）：0；1程序准备停止，指示外部按钮检测线程任务退出
	UINT unStatRun;								// 	程行状态（run_state）：0；0(停止状态)、1(暂停状态)、2(运行状态)
	BOOL bOverRcg;								// 	识别完成（Image_recognition_over）：1；1表示相机识别完成可以取片
	BOOL bOverCam;								//	相机照相采集图像完成
	//BOOL s_bool_start_imgrecognition;			// 	识别开始（Image_recognition_start）：0；1指示晶片准备好，识别任务可以开始
	char chStmCmd;								//	Stm串口反馈
	char chStmCmd0;                             //  stm反馈0，在未启动时测量完成信号
	char chCmd[7];								//	串口命令
	char chTmYMD[20];							//	用于年月日文件夹的建立
	char chTmHMS[10];							//	用于时分秒图片开始时间的保存
	UINT unCntResend;							//	重发次数，用于判断是否超时
	double dRT;									//	运行时间
	bool bExit;									//	是否需要退出
	UINT unCntStart;							//	记录开始检测次数
	unsigned int unSort[3];						//	记录最近的三次分档值
	BOOL bIsSelFt;								//	记录是否已选择特征片
	bool bRunStatChg;							//	记录是否有运行变化事件	
	int bPosSet;								//	PosSet标记，1stm已设置XY位置2已设置换片位置
	int nEfgStaSel;								//	EFG开始检测位置选择标记
	int runCnt;
	DWORD timRun[10][2];				    //  运行时间记录
	bool closeShake;
}RunInfo;

//刷新缓存结构
typedef struct RefreshTmp
{
	CString s_cstr_recv;						//	保存接收显示字符串
	UINT unStatRun;								//	程序状态
	UINT unN;									//	图像采集总数
	double dRT;									//	运行时间
	//UINT s_uint_t;							//  现在需要或正在做发送的命令
	char chID;									//	串口命令发送id，每发不同的一条命令，id++
	bool bComUpdate;							//	标记是否需要更新Com内容
	int alarm;
}RefreshTmp;

//txt操作相关结构
typedef struct PathInfo
{
	CString csPathIni,csPathTxt;	//	txt目录
	CString csPathExe;
	CString csPathLog;
}PathInfo;
//相机数据定义
// typedef struct IIICInfo
// {
// 	LONG glCamID;
// 	VehiclePassRec gstuCamResult;
// }IIICInfo;
typedef struct RecognitionInfo
{
	bool bChkPN;//是否区分正反
	int nPN;//保存用来匹配的方向
	int nPToL;//直线点距
	int nDefectPToL;//缺陷点距
	//以上用于找直线并确定在直线外的点
	int nThreshold;//阀值
	bool bThrdAuto;//自动阈值标识
	bool bDelNoise;//去噪标识
	bool bDebug;//指示是否开启DEBUG//速度很慢
  bool bOnce;// 只取一片
	int nAllowDefect;//特征允许误差，用于判断识别出的长宽是否符合要求
	int nLength,nWide;//保存特征长宽
	bool bIsCir;
	double g_factor[2][3];//保存屏幕坐标转换到机械手坐标的系数。转换公式为X=g_factor[0][0]*x+g_factor[0][1]*y+g_factor[0][2];Y=g_factor[1][0]*x+g_factor[1][1]*y+g_factor[1][2];
	double Xxy[3][4],Yxy[3][4];//用于坐标校准
	int Nxy;//用于坐标校准
	int nClbPosX[3],nClbPosY[3];//用于保存坐标校准的固定位置
	bool bClbPos;
	int nSNCHGX,nSNCHGY;//xy轴换吸头位置
}RecognitionInfo;

struct threadstatus//线程状态全局结构
{
	int _N,_RUN;//run标记运行状态，n标记已检测数量
	bool _SHOW;//show标记执行界面刷新操作
	bool _CHK1,_CHK2,_SCREND,_SORTTAK;//chk12当EFG测完时，发标志给读屏幕和分档线程,SCREND表示屏幕读取完毕,sorttak分档臂取走
	//double _X,_Y,_TOTAL,_AVG,_TN,lasttime,maxtime,mintime;//模拟机械手坐标
	bool TAK_STOP,SOT_STOP,SCR_STOP,DSP_STOP,CHK_STOP,SYS_PAUSE;//各线程停止标记
	unsigned int Xwait,Xcheck,Ywait,Ysort[30];
	bool _DEBUGEFG;//模拟EFG开始检测信号
	UINT unProcNum;//记录CPU数
	//clock_t _START,_TMP,_END;
};

struct sortchip//分档全局结构
{
	int centerangle,sortvalue,eleclow,elechigh,cutvalue;//预设值
	int phi0, factor;//计算等效角的参数 DATE 180414
	unsigned int sortnum[30],sortsum,sortsn,alertnum[30];//sortsum1~24档片数，sortsn分档值,sortsum已测出1~24档总片数
	double sortavg[5],sortstd[5];//存取1~24档的平均值及标准偏差
	int sortsel1,sortsel2;//标记测量轴及限定轴
	long double std2[5];//计算标准偏差中间值
	int checkout0,nocheckout;//测出0的次数（连续，不能测出
	bool needCheck;
};

//注意这个结构体的名称不正常的，只是迎合原先控件的名称，至于用处，要看device.h头的#define
typedef struct tagNpcInf
{
  int var_x1, var_x2, var_x3;
  int var_y1, var_y2, var_y3;
  int var7, var8, var9, var10;
  int var_sortup, var_sortin, var_sortout, var_sortdown;

  int offset_x1, offset_y1;//xy电机偏移补偿<半圈
  int offset_x2, offset_y2;//xy电机偏移补偿>半圈
  int offset_p, offset_n;//
  int left, top, right, bottom;//可选范围
}NpcInf;

typedef struct tagNpcParm
{
  double x0, y0, deg0, pn0;//程序出来的对应图像上的位置
  int x, y, deg, pn;//对应到实际电机步数
}NpcParm;

enum RUN_STAT {
  RUN_STAT_STOP = 0,
  RUN_STAT_PAUSE,
  RUN_STAT_RUN
};

#endif

