
// BER.h : PROJECT_NAME 应用程序的主头文件
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
// CBERApp:
// 有关此类的实现，请参阅 BER.cpp
//


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
extern PathInfo gstuPathInf;
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
extern UINT RunThread(LPVOID pParam);//运行线程函数
extern UINT CameraThread(LPVOID pParam);//图像识别线程函数
extern UINT ScreenThread(LPVOID pParam);//运行线程函数
extern UINT CalibrationThread(LPVOID pParam);
extern UINT CameraThreadTest(LPVOID pParam);
extern UINT ComMsg(LPVOID pParam);
void calparameter(double (*xy)[4],double *factor);
extern double TimeRecord(unsigned int step, bool start);
extern int CalcEquAngle(int *degree);

//vlc播放视频的回调函数
void CALLBACK RealDataCallBack0(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,void *pUser);
extern libvlc_media_player_t * vlc_player,*vlc_player_run;
extern BOOL gb_PlayOrNot[2];