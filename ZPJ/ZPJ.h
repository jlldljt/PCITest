
// ZPJ.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Run.h"
#include "Debug.h"
#include "Camera.h"
#include "Screen.h"
#include "Device.h"
#include "DlgMsg.h"
#include "CSVFile.h"
#include "DlgCheckStd.h"
#include "DlgCpk.h"
// CBERApp:
// 有关此类的实现，请参阅 BER.cpp
//


#define USER_TO_SEC(deg) ((deg)/10000*3600+(deg)%10000/100*60+(deg)%100)
#define DEG_TO_USER(deg) (SEC_TO_USER(DEG_TO_SEC(deg)))
#define SEC_TO_USER(sec) (((int)sec)/3600*10000+((int)sec)%3600/60*100+((int)sec)%60)

#define USER_TO_DEG(deg) ((deg)/10000+(deg)%10000/100/60.0+(deg)%100/3600.0)
#define DEG_TO_SEC(deg) ((int)((deg)*3600))
#define SEC_TO_DEG(sec) ((sec)/3600.0)
//秒的度分秒
#define S_DEG(sec) ((sec)/3600)
#define S_MIN(sec) ((sec)%3600/60)
#define S_SEC(sec) ((sec)%60)
//度的度分秒
#define D_DEG(deg) S_DEG((int)((deg)*3600))
#define D_MIN(deg) S_MIN((int)((deg)*3600))
#define D_SEC(deg) S_SEC((int)((deg)*3600))
//用户输入的度分秒
#define U_DEG(user) ((user)/10000)
#define U_MIN(user) ((user)%10000/100)
#define U_SEC(user) ((user)%100)


#define VAR_X g_npc_inf.var_x1
#define VAR_Y g_npc_inf.var_x2
#define VAR_STATUS g_npc_inf.var_x3
#define VAR_DEG g_npc_inf.var_y1
#define VAR_PN g_npc_inf.var_y2
#define VAR_BELT g_npc_inf.var7
#define VAR_DEFPN g_npc_inf.var8
#define VAR_CUT g_npc_inf.var9
#define VAR_SIZE g_npc_inf.var10

#define VAR_TURNTABLE g_npc_inf.var_y3//规定PN的筛选值

#define NPC_ID g_npc_inf.var_sortup
#define Z_P g_npc_inf.var_sortin  //z轴一圈的脉冲数
#define Z_P_PER_DEG (Z_P/360.0)   //z轴一°的脉冲数
#define Z_S g_npc_inf.var_sortout //z轴偏移脉冲数

#define VAR_NUM g_npc_inf.var_sortdown

//#define PN_DEF g_npc_inf.var_y3//规定PN的筛选值

class CBERApp : public CWinApp
{
public:
	CBERApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CBERApp theApp;
//
extern RunInfo gstuRun;
extern RefreshTmp gstuRefresh;
extern threadstatus gstuTrdStat;//线程全局结构变量
extern sortchip gstuSort;
extern CFont g_font,g_font2,g_font3;
extern CTXT gclsTxt;
//
extern CIMGRecognition gclsImgRcg;
extern RecognitionInfo  gstuRcgInfo;
extern NpcInf g_npc_inf;
extern PathInfo gstuPathInf;
extern CCpkLib *g_pCpk;
extern INT32 *g_vars;
//
//extern CPicture gclsPic;//pictrue类对象
//extern SETTINGS_CAMERA gstuCamParam;//相机参数
//extern Network_Config_Data gstuCamNetCfg;
//extern LONG glCamID;//相机ID，初始化未-1
//extern VehiclePassRec gstuCamResult;//相机结果结构
//
extern int gnChannel;
extern CKSJ gclsCamera;
//
extern CWinThread* gTrdRun;//运行线程
extern CWinThread* gTrdCam;//相机线程
extern CWinThread* gTrdScr;//屏幕线程
extern CWinThread* gTrdClb;//校准线程
extern CWinThread* gTrdCom;//com线程
extern CWinThread* gTrdAuto;

extern UINT RunThread(LPVOID pParam);//运行线程函数
extern UINT CameraThread(LPVOID pParam);//图像识别线程函数
extern UINT ScreenThread(LPVOID pParam);//运行线程函数
extern UINT CalibrationThread(LPVOID pParam);
extern UINT CameraThreadTest(LPVOID pParam);
extern UINT ComMsg(LPVOID pParam);
UINT Thread_Auto(LPVOID pParam);

void calparameter(double (*xy)[4],double *factor);
extern double TimeRecord(unsigned int step, bool start);
extern int CalcEquAngle(int *degree);
extern void CalcAvgStd(const int sum, const double angle, double &avg, double &std, double &std2);

void TranNpcParam(NpcParm * parm);

//vlc播放视频的回调函数
void CALLBACK RealDataCallBack0(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,void *pUser);
extern libvlc_media_player_t * vlc_player,*vlc_player_run;
extern BOOL gb_PlayOrNot[2];