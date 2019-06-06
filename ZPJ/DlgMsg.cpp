// DlgMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZPJ.h"
#include "DlgMsg.h"
#include "afxdialogex.h"
//CDlgMsgReadIo *g_dlgMsg;
int open = 0;
// CDlgMsg 对话框

IMPLEMENT_DYNAMIC(CDlgMsg, CDialogEx)

	CDlgMsg::CDlgMsg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMsg::IDD, pParent)
	, strShowText(_T(""))
{

}

CDlgMsg::~CDlgMsg()
{

}

void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMsg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ONE, &CDlgMsg::OnBnClickedBtnOne)
	ON_BN_CLICKED(IDC_BTN_TWO, &CDlgMsg::OnBnClickedBtnTwo)
	//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgMsg 消息处理程序


void CDlgMsg::OnBnClickedBtnOne()
{
	// TODO: 在此添加控件通知处理程序代码

	//EndDialog (101);
	if (1 == m_nSel)
	{
		
			EndDialog (101);
	}
	else if (2 == m_nSel)
	{
		
			g_dlgDevice->AlarmCtrl(0);
			//ShowWindow(0);
			OnCancel();		

	}

}


void CDlgMsg::OnBnClickedBtnTwo()
{
	// TODO: 在此添加控件通知处理程序代码
	//EndDialog (102);
	if (1 == m_nSel)
	{

		EndDialog (102);
	}
	else if (2 == m_nSel)
	{

		g_dlgDevice->AlarmCtrl(0);
		//ShowWindow(0);
		OnCancel();		

	}

}


BOOL CDlgMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_STAT_MSG,strShowText);
	SetDlgItemText(IDC_BTN_ONE,strCho1Txt);
	SetDlgItemText(IDC_BTN_TWO,strCho2Txt);
	//SetTimer(READIO, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CDlgMsg 对话框

IMPLEMENT_DYNAMIC(CDlgMsgReadIo, CDlgMsg)

	CDlgMsgReadIo::CDlgMsgReadIo(CWnd* pParent /*=NULL*/)
	: CDlgMsg(pParent)
{

}

CDlgMsgReadIo::~CDlgMsgReadIo()
{
}

void CDlgMsgReadIo::DoDataExchange(CDataExchange* pDX)
{
	CDlgMsg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgMsgReadIo, CDlgMsg)
	ON_WM_TIMER()
	//	ON_WM_DESTROY()
END_MESSAGE_MAP()

//定制型，SHAKEUP,SHAKEDOWN
void CDlgMsgReadIo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case READIO:
		{
			if (1 == m_nSel)
			{
				if(gstuRun.chStmCmd & (1<<0))
					EndDialog (101);
			}
			else if (2 == m_nSel)
			{
				if(gstuRun.chStmCmd & (1<<0))
				{
					g_dlgDevice->AlarmCtrl(0);
					//ShowWindow(0);
					OnCancel();
					return;
				}

			}
		}
		break;
	default:
		break;
	}
	CDlgMsg::OnTimer(nIDEvent);
}


BOOL CDlgMsgReadIo::OnInitDialog()
{
	CDlgMsg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_STAT_MSG,strShowText);
	SetDlgItemText(IDC_BTN_ONE,strCho1Txt);
	GetDlgItem(IDC_BTN_TWO)->ShowWindow(FALSE);
	SetTimer(READIO, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgMsgReadIo::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	KillTimer(READIO);
	return CDlgMsg::DestroyWindow();
}

//返回1，第一个选择，返回2，第二个选择，返回其他，未知
int MyMsgBox(char* text , char* cho1txt, char* cho2txt)
{
	CDlgMsg dlgMsg;
	dlgMsg.strShowText = text;
	if (cho1txt == "")
	{
		cho1txt = "确定";
	}

	if (cho2txt == "")
	{
		cho2txt = "取消";
	}
	dlgMsg.strCho1Txt = cho1txt;
	dlgMsg.strCho2Txt = cho2txt;
	int ret = dlgMsg.DoModal();
	switch(ret)
	{
	case 101:
		ret = 1;
	case 102:
		ret = 2;
	default:
		ret = 0;
	}
	return ret;
}
int MyMsgBox(char* text)
{
	CDlgMsg dlgMsg;

	dlgMsg.strShowText = text;
	dlgMsg.strCho1Txt = "确定";
	dlgMsg.strCho2Txt = "取消";
	int ret = dlgMsg.DoModal();
	switch(ret)
	{
	case 101:
		ret = 1;
	case 102:
		ret = 2;
	default:
		ret = 0;
	}

	return ret;
}

//自定义拓展msgbox返回1，第一个选择，返回2，第二个选择，返回其他，未知
int MyMsgBoxReadIo(char* text)
{
	CDlgMsgReadIo dlgMsg;
	dlgMsg.strShowText = text;
	dlgMsg.m_nSel = 1;
	g_dlgDevice->AlarmCtrl(1);
	dlgMsg.strCho1Txt = "确定";

	int ret = dlgMsg.DoModal();
	switch(ret)
	{
	case 101:
		ret = 1;
	case 102:
		ret = 2;
	default:
		ret = 0;
	}
	g_dlgDevice->AlarmCtrl(0);
	return ret;
}

int MyMsgBoxReadIo2(char* text)
{
	/*if(open)
		return 0;
	else
		open = 1;*/
	CDlgMsgReadIo *g_dlgMsg = new CDlgMsgReadIo;
	g_dlgMsg->strShowText = text;
	g_dlgMsg->m_nSel = 2;
	g_dlgDevice->AlarmCtrl(1);
	g_dlgMsg->strCho1Txt = _T("确定");


	g_dlgMsg->Create(IDD_MSG,g_dlgMsg);    //IDD_DLG为已经定义的对话框资源的ID号

	g_dlgMsg->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	g_dlgMsg->CenterWindow();
	g_dlgMsg->ShowWindow(SW_SHOW);

	return 1;
}

//void CDlgMsgReadIo::OnDestroy()
//{
//	CDlgMsg::OnDestroy();
//	
//	// TODO: 在此处添加消息处理程序代码
//	if (2 == m_nSel)
//	{
//		AlertCtrl(0);
//		//delete this;
//		open = 0;
//	}
//}


void CDlgMsgReadIo::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (2 == m_nSel)
	{
		DestroyWindow();
	}
	else
		CDlgMsg::OnCancel();

}


void CDlgMsgReadIo::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (2 == m_nSel)
	{
		g_dlgDevice->AlarmCtrl(0);
		delete this;
		open = 0;
	}
	CDlgMsg::PostNcDestroy();
}
