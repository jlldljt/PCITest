#pragma once
#include "afxwin.h"


// CCamera �Ի���

class CCamera : public CDialogEx
{
	DECLARE_DYNAMIC(CCamera)

public:
	CCamera(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCamera();
	CDC *mDCMem;
	CBitmap *mBMPMem,*mBMPOld;
	void IIICStartConnect();
// �Ի�������
	enum { IDD = IDD_DIALOG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSnap();
	CComboBox m_cmbAWB;
	CComboBox m_cmbBl;
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnCamset();
	virtual BOOL OnInitDialog();
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
};
extern CCamera *g_dlgCamera;