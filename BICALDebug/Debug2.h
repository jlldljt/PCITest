#pragma once


// CDebug2 �Ի���

class CDebug2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDebug2)

public:
	CDebug2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDebug2();

// �Ի�������
	enum { IDD = IDD_DIALOG_DEBUG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

extern CDebug2 *g_dlgDebug2;