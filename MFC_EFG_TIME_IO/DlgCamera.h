#pragma once
#include "EfgIO.h"
#include "KSJ.h"
#include "IMGRecognition.h"
#include "CvCam.h"

using namespace KSJ_NAME_SPACE;

typedef struct tagNpcParm
{
	double x0, y0, deg0, pn0;//��������Ķ�Ӧͼ���ϵ�λ��
	int x, y, deg, pn;//��Ӧ��ʵ�ʵ������
}NpcParm;

//ע������ṹ������Ʋ������ģ�ֻ��ӭ��ԭ�ȿؼ������ƣ������ô���Ҫ��device.hͷ��#define
typedef struct tagNpcInf
{
	int var_x1, var_x2, var_x3;
	int var_y1, var_y2, var_y3;
	int var_sortup, steps360, offset360, var_sortdown;

	int left, top, right, bottom;
}NpcInf;

#define Z_P m_npc_inf.steps360  //z��һȦ��������
#define Z_P_PER_DEG (Z_P/360.0)   //z��һ���������
#define Z_S m_npc_inf.offset360 //z��ƫ��������

// CCamera �Ի���

class CCamera : public CDialogEx
{
	DECLARE_DYNAMIC(CCamera)

public:
	CCamera(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCamera();
	CDC *mDCMem;
	CBitmap *mBMPMem,*mBMPOld;
	EFG_ConfigParam* m_param;
	void IIICStartConnect();
	// �Ի�������
	enum { IDD = IDD_DLG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSnap();
	CComboBox m_cmbAWB;
	CComboBox m_cmbBl;
	//int m_clked_pos_x;//�����ת���������x
	//int m_clked_pos_y;//�����ת���������y
	//int m_clked_degree;//�����Ƭ�ĽǶ�
	NpcParm m_par;
	NpcInf m_npc_inf;
	// BOOL bOverRcg;
	int m_ch;
	CKSJ m_ksj;
	BOOL gb_PlayOrNot[2];
	CIMGRecognition m_rcg;
	CEfgIO* m_io;
	CCvCam m_cam;

	void VideoOnOff(BOOL on);
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnCamset();
	virtual BOOL OnInitDialog();
	void TranNpcParam(NpcParm* parm);
	afx_msg void OnStnClickedPreview();
	afx_msg void OnBnClickedBtnCalibration();
	afx_msg void OnBnClickedBtnVideo();
	afx_msg void OnPaint();
	afx_msg void OnEnChangeEdtOutpallowl();
	afx_msg void OnEnChangeEdtDefectallowl();
	afx_msg void OnEnChangeEdtThreshold();
	afx_msg void OnEnChangeEdtFtoutpoint();
	afx_msg void OnBnClickedBtnGetset();
	afx_msg void OnBnClickedChkAutothrd();
	afx_msg void OnBnClickedChkDelnoise();
	//	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedChkDebug();
	afx_msg void OnBnClickedBtnSplit();
	afx_msg void OnBnClickedBtnReconnect();
	afx_msg void OnBnClickedBtnParmsend();
	afx_msg void OnBnClickedBtnAuto();
	afx_msg void OnEnChangeEdtSquMindeg();
	afx_msg void OnEnChangeEdtSquMaxdeg();
	afx_msg void OnEnChangeEdtPnMindeg();
	afx_msg void OnEnChangeEdtPnMaxdeg();
	afx_msg void OnEnChangeEdtThreshold2();
	afx_msg void OnEnChangeEdtClbpos1x();
	afx_msg void OnEnChangeEdtClbpos1y();
	afx_msg void OnBnClickedBtnClbpos2test();
	afx_msg void OnEnChangeEdtClbpos2x();
	afx_msg void OnEnChangeEdtClbpos3x();
	afx_msg void OnEnChangeEdtClbpos2y();
	afx_msg void OnEnChangeEdtClbpos3y();
	afx_msg void OnEnChangeEdtTransferx();
	afx_msg void OnEnChangeEdtTransfery();
	afx_msg void OnBnClickedBtnClbpos1test();
	afx_msg void OnBnClickedBtnTransfertest();
	afx_msg void OnBnClickedBtnClbpos3test();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnTestzero();
};

void calparameter(double(*xy)[4], double* factor);//��������ת������