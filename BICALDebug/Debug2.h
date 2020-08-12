#pragma once


// CDebug2 对话框

class CDebug2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDebug2)

public:
	CDebug2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDebug2();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEBUG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

extern CDebug2 *g_dlgDebug2;