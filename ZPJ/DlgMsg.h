#pragma once


// CDlgMsg 对话框

class CDlgMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMsg)

public:
	CDlgMsg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMsg();

	// 对话框数据
	enum { IDD = IDD_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString strShowText;
	CString strCho1Txt;
	CString strCho2Txt;
	int m_nSel;
	afx_msg void OnBnClickedBtnOne();
	afx_msg void OnBnClickedBtnTwo();
	virtual BOOL OnInitDialog();
	//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//	virtual BOOL DestroyWindow();
};

class CDlgMsgReadIo : public CDlgMsg
{
	DECLARE_DYNAMIC(CDlgMsgReadIo)

public:
	CDlgMsgReadIo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMsgReadIo();

	// 对话框数据
	enum
	{
		READIO = 1

	};

	/*int m_nSel;*/


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	//	afx_msg void OnDestroy();
	virtual void OnCancel();
	virtual void PostNcDestroy();
};

extern int MyMsgBox(char* text , char* cho1txt , char* cho2txt);
extern int MyMsgBox(char* text);
extern int MyMsgBoxReadIo(char* text);
extern int MyMsgBoxReadIo2(char* text);