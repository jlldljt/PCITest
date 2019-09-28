
// ZPJ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
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
// �йش����ʵ�֣������ BER.cpp
//


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


#define VAR_X g_npc_inf.var_x1
#define VAR_Y g_npc_inf.var_x2
#define VAR_STATUS g_npc_inf.var_x3
#define VAR_DEG g_npc_inf.var_y1
#define VAR_PN g_npc_inf.var_y2
#define VAR_BELT g_npc_inf.var7
#define VAR_DEFPN g_npc_inf.var8
#define VAR_CUT g_npc_inf.var9
#define VAR_SIZE g_npc_inf.var10

#define VAR_TURNTABLE g_npc_inf.var_y3//�涨PN��ɸѡֵ

#define NPC_ID g_npc_inf.var_sortup
#define Z_P g_npc_inf.var_sortin  //z��һȦ��������
#define Z_P_PER_DEG (Z_P/360.0)   //z��һ���������
#define Z_S g_npc_inf.var_sortout //z��ƫ��������

#define VAR_NUM g_npc_inf.var_sortdown

//#define PN_DEF g_npc_inf.var_y3//�涨PN��ɸѡֵ

class CBERApp : public CWinApp
{
public:
	CBERApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBERApp theApp;
//
extern RunInfo gstuRun;
extern RefreshTmp gstuRefresh;
extern threadstatus gstuTrdStat;//�߳�ȫ�ֽṹ����
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
//extern CPicture gclsPic;//pictrue�����
//extern SETTINGS_CAMERA gstuCamParam;//�������
//extern Network_Config_Data gstuCamNetCfg;
//extern LONG glCamID;//���ID����ʼ��δ-1
//extern VehiclePassRec gstuCamResult;//�������ṹ
//
extern int gnChannel;
extern CKSJ gclsCamera;
//
extern CWinThread* gTrdRun;//�����߳�
extern CWinThread* gTrdCam;//����߳�
extern CWinThread* gTrdScr;//��Ļ�߳�
extern CWinThread* gTrdClb;//У׼�߳�
extern CWinThread* gTrdCom;//com�߳�
extern CWinThread* gTrdAuto;

extern UINT RunThread(LPVOID pParam);//�����̺߳���
extern UINT CameraThread(LPVOID pParam);//ͼ��ʶ���̺߳���
extern UINT ScreenThread(LPVOID pParam);//�����̺߳���
extern UINT CalibrationThread(LPVOID pParam);
extern UINT CameraThreadTest(LPVOID pParam);
extern UINT ComMsg(LPVOID pParam);
UINT Thread_Auto(LPVOID pParam);

void calparameter(double (*xy)[4],double *factor);
extern double TimeRecord(unsigned int step, bool start);
extern int CalcEquAngle(int *degree);
extern void CalcAvgStd(const int sum, const double angle, double &avg, double &std, double &std2);

void TranNpcParam(NpcParm * parm);

//vlc������Ƶ�Ļص�����
void CALLBACK RealDataCallBack0(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,void *pUser);
extern libvlc_media_player_t * vlc_player,*vlc_player_run;
extern BOOL gb_PlayOrNot[2];