
// BER.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
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
// CBERApp:
// �йش����ʵ�֣������ BER.cpp
//


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
extern PathInfo gstuPathInf;
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
extern UINT RunThread(LPVOID pParam);//�����̺߳���
extern UINT CameraThread(LPVOID pParam);//ͼ��ʶ���̺߳���
extern UINT ScreenThread(LPVOID pParam);//�����̺߳���
extern UINT CalibrationThread(LPVOID pParam);
extern UINT CameraThreadTest(LPVOID pParam);
extern UINT ComMsg(LPVOID pParam);
void calparameter(double (*xy)[4],double *factor);
extern double TimeRecord(unsigned int step, bool start);
extern int CalcEquAngle(int *degree);

//vlc������Ƶ�Ļص�����
void CALLBACK RealDataCallBack0(LONG lRealHandle,DWORD dwDataType,BYTE *pBuffer,DWORD dwBufSize,void *pUser);
extern libvlc_media_player_t * vlc_player,*vlc_player_run;
extern BOOL gb_PlayOrNot[2];