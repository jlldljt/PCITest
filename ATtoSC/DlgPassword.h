#pragma once
#include "StandardLib.h"

// CDlgPassword �Ի���

class CDlgPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPassword)

public:
	CDlgPassword(CStandardLib * pLib = NULL, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPassword();

// �Ի�������
	enum { IDD = IDD_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
  CStandardLib * m_pLib;

  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
};
