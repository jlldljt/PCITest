// DlgMsg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZPJ.h"
#include "DlgMsg.h"
#include "afxdialogex.h"
//CDlgMsgReadIo *g_dlgMsg;
int open = 0;
// CDlgMsg �Ի���

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


// CDlgMsg ��Ϣ�������


void CDlgMsg::OnBnClickedBtnOne()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_STAT_MSG,strShowText);
	SetDlgItemText(IDC_BTN_ONE,strCho1Txt);
	SetDlgItemText(IDC_BTN_TWO,strCho2Txt);
	//SetTimer(READIO, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// CDlgMsg �Ի���

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

//�����ͣ�SHAKEUP,SHAKEDOWN
void CDlgMsgReadIo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_STAT_MSG,strShowText);
	SetDlgItemText(IDC_BTN_ONE,strCho1Txt);
	GetDlgItem(IDC_BTN_TWO)->ShowWindow(FALSE);
	SetTimer(READIO, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CDlgMsgReadIo::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	KillTimer(READIO);
	return CDlgMsg::DestroyWindow();
}

//����1����һ��ѡ�񣬷���2���ڶ���ѡ�񣬷���������δ֪
int MyMsgBox(char* text , char* cho1txt, char* cho2txt)
{
	CDlgMsg dlgMsg;
	dlgMsg.strShowText = text;
	if (cho1txt == "")
	{
		cho1txt = "ȷ��";
	}

	if (cho2txt == "")
	{
		cho2txt = "ȡ��";
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
	dlgMsg.strCho1Txt = "ȷ��";
	dlgMsg.strCho2Txt = "ȡ��";
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

//�Զ�����չmsgbox����1����һ��ѡ�񣬷���2���ڶ���ѡ�񣬷���������δ֪
int MyMsgBoxReadIo(char* text)
{
	CDlgMsgReadIo dlgMsg;
	dlgMsg.strShowText = text;
	dlgMsg.m_nSel = 1;
	g_dlgDevice->AlarmCtrl(1);
	dlgMsg.strCho1Txt = "ȷ��";

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
	g_dlgMsg->strCho1Txt = _T("ȷ��");


	g_dlgMsg->Create(IDD_MSG,g_dlgMsg);    //IDD_DLGΪ�Ѿ�����ĶԻ�����Դ��ID��

	g_dlgMsg->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE |SWP_NOSIZE);
	g_dlgMsg->CenterWindow();
	g_dlgMsg->ShowWindow(SW_SHOW);

	return 1;
}

//void CDlgMsgReadIo::OnDestroy()
//{
//	CDlgMsg::OnDestroy();
//	
//	// TODO: �ڴ˴������Ϣ����������
//	if (2 == m_nSel)
//	{
//		AlertCtrl(0);
//		//delete this;
//		open = 0;
//	}
//}


void CDlgMsgReadIo::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (2 == m_nSel)
	{
		DestroyWindow();
	}
	else
		CDlgMsg::OnCancel();

}


void CDlgMsgReadIo::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (2 == m_nSel)
	{
		g_dlgDevice->AlarmCtrl(0);
		delete this;
		open = 0;
	}
	CDlgMsg::PostNcDestroy();
}
