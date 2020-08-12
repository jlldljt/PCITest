// Debug1.cpp : 实现文件
//

#include "stdafx.h"
#include "BICALDebug.h"
#include "Debug1.h"
#include "afxdialogex.h"


// CDebug1 对话框

IMPLEMENT_DYNAMIC(CDebug1, CDialogEx)

CDebug1::CDebug1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebug1::IDD, pParent)
{
	g_dlgDebug1=this;
}
CDebug1 dlgDebug1;
CDebug1 *g_dlgDebug1=NULL;

CDebug1::~CDebug1()
{
}

void CDebug1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RS232_RECEIVE, m_edtRs232Rcv);
}


BEGIN_MESSAGE_MAP(CDebug1, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_COM, &CDebug1::OnEnChangeEditCom)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDebug1::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDebug1::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON1, &CDebug1::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_XGRID, &CDebug1::OnEnChangeXgrid)
	ON_EN_CHANGE(IDC_YGRID, &CDebug1::OnEnChangeYgrid)
	ON_EN_CHANGE(IDC_X1, &CDebug1::OnEnChangeX1)
	ON_EN_CHANGE(IDC_Y1, &CDebug1::OnEnChangeY1)
	ON_EN_CHANGE(IDC_XEND, &CDebug1::OnEnChangeXend)
	ON_EN_CHANGE(IDC_YEND, &CDebug1::OnEnChangeYend)
	ON_EN_CHANGE(IDC_Xlimit, &CDebug1::OnEnChangeXlimit)
	ON_EN_CHANGE(IDC_Ylimit, &CDebug1::OnEnChangeYlimit)
	ON_EN_CHANGE(IDC_XMIN, &CDebug1::OnEnChangeXmin)
	ON_EN_CHANGE(IDC_YMIN, &CDebug1::OnEnChangeYmin)
	ON_EN_CHANGE(IDC_STEP, &CDebug1::OnEnChangeStep)
	ON_BN_CLICKED(IDC_UPDATE, &CDebug1::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_BUTTON_SENDTEXT, &CDebug1::OnBnClickedButtonSendtext)
END_MESSAGE_MAP()


// CDebug1 消息处理程序


void CDebug1::OnEnChangeEditCom()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	gclsCom.CloseCom();
	CString l_str;
	int l_int=GetDlgItemInt(IDC_EDIT_COM,0,0);
	l_str.Format(_T("%d"),l_int);
	gclsCom.OpenCom(l_int);
	if (gclsCom.stuInf.bComOpen)
	{
		AfxMessageBox(_T("打开COM"));
		WritePrivateProfileString(_T("COM"),_T("口号"),l_str,g_csPath);  
	}
}


void CDebug1::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	int nValue = GetDlgItemInt(IDC_EDIT_TEST,0,0);
	if (0==nValue)
	{
		Test(0);
		Sleep(1000);
		Test(1);
		Sleep(1000);
		Test(2);
		Sleep(1000);
	}
	else if (1==nValue)
	{
		Test(0);
		Sleep(1000);
		Test(3);
		Sleep(1000);
		Test(4);
		Sleep(1000);
	}

	else if (2==nValue)
	{
		Test(5);
		Sleep(1000);
		Test(6);
		Sleep(1000);
		Test(7);
		Sleep(1000);
	}

	else if (3==nValue)
	{
		Test(5);
		Sleep(1000);
		Test(8);
		Sleep(1000);
		Test(9);
		Sleep(1000);
	}

	else if (4==nValue)
	{
		Test(10);
		Sleep(1000);
		Test(11);
	}

	else if (5==nValue)
	{
		Test(16);
	}

	else if (6==nValue)
	{
		Test(12);
	}
	else if (7==nValue)
	{
		Test(13);
	}
	else if (8==nValue)
	{
		Test(14);
	}
	else if (9==nValue)
	{
		Test(15);
	}
	else if (10==nValue)
	{
		Test(17);
	}
	else if (11==nValue)
	{
		Test(18);
	}
	else if (12==nValue)
	{
		Test(19);
		Sleep(1000);
		Test(20);
		Sleep(1000);
		Test(21);
		Sleep(1000);Test(22);
	}
}


void CDebug1::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	gclsCom.stuInf.csRcv="";
	gstuRefresh.bComUpdate=1;
}


void CDebug1::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (runbool)
	{
		runbool = 0;
		Sleep(1000);
	}
	else
	{

		runbool = 1;
		gTrdRun=AfxBeginThread(RunThread , NULL , THREAD_PRIORITY_NORMAL , 0 , CREATE_SUSPENDED);
		if (NULL == gTrdRun)
		{
			AfxMessageBox(_T("启动线程失败"));
			return ;
		}
		gTrdRun->m_bAutoDelete = TRUE;
		gTrdRun->ResumeThread();
	}

}


void CDebug1::OnEnChangeXgrid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//update = 1;
}


void CDebug1::OnEnChangeYgrid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeX1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeY1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeXend()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeYend()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeXlimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeYlimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeXmin()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeYmin()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnEnChangeStep()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	//update = 1;
	// TODO:  在此添加控件通知处理程序代码
}


void CDebug1::OnBnClickedUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	update = 1;
}

//针对CSTRING
//按字符分割符截取需要的字符串
//iSub从0开始，0是第一个字符串，1第二个
//返回0，没找到
//返回1，成功
BOOL Split(LPCTSTR lpSource, char*& dest, char division, UINT iSub)
{
	char source[10000];

	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,lpSource,-1,source,sizeof(source),NULL,NULL); 

	//dest="";
	char* l_tmpchar;
	// 	int l_ipos = 0;
	// 	int l_pre_ipos = 0;
	int l_ifind=0;
	int l_ilen=strlen(source);
	l_tmpchar=new char[l_ilen+1];
	memset(l_tmpchar,0,l_ilen+1);
	//CString l_cstr_tmp;
	int j=0;

	for(int i=0;i<l_ilen;i++ )
	{
		if(source[i]==division) 
		{
			l_ifind++;
			continue;
		}

		if (l_ifind==iSub)
		{
			l_tmpchar[j]=source[i];
			j++;
		}

		else if (l_ifind > iSub)
		{
			break;
		}


	}

	l_ilen=strlen(l_tmpchar);

	if (0 == l_ilen)
	{
		return 0;
	}

	memset(dest,0,l_ilen+1);

	for (int i=0;i<l_ilen;i++)
	{
		dest[i]=l_tmpchar[i];
	}

	delete[] l_tmpchar;
	l_tmpchar=NULL;
	return 1;

}

//按字符分割符截取需要的字符串
//iSub从0开始，0是第一个字符串，1第二个
//返回0，没找到
//返回1，成功
BOOL Split(char* source, char*& dest, char division, UINT iSub)
{
	//dest="";
	char* l_tmpchar;
	// 	int l_ipos = 0;
	// 	int l_pre_ipos = 0;
	int l_ifind=0;
	int l_ilen=strlen(source);
	l_tmpchar=new char[l_ilen];
	memset(l_tmpchar,0,l_ilen);
	//CString l_cstr_tmp;
	int j=0;

	for(int i=0;i<l_ilen;i++ )
	{
		if(source[i]==division) 
		{
			l_ifind++;
			continue;
		}

		if (l_ifind==iSub)
		{
			l_tmpchar[j]=source[i];
			j++;
		}

	}

	l_ilen=strlen(l_tmpchar);

	if (0 == l_ilen)
	{
		return 0;
	}

	memset(dest,0,l_ilen+1);

	for (int i=0;i<l_ilen;i++)
	{
		dest[i]=l_tmpchar[i];
	}

	delete[] l_tmpchar;
	l_tmpchar=NULL;
	return 1;

}
unsigned int ToInt(char abc);
void CDebug1::OnBnClickedButtonSendtext()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strBuffer;
	GetDlgItemText(IDC_EDIT_COMTEXT,strBuffer);

	char* l_char_buf = NULL;
	
	DWORD l_dword_len = (DWORD)strBuffer.GetLength();
	l_char_buf = new char[l_dword_len + 1];
	memset(l_char_buf, 0, l_dword_len + 1);
	int i = 0;
	while(Split(strBuffer,l_char_buf,' ',i))
	{
		g_chCmd[i++] = ToInt(l_char_buf[0])*0x10 + ToInt(l_char_buf[1]);
	}

	gclsCom.ClearCom();
	gclsCom.SendCharToComNOID(g_chCmd,i);
}

unsigned int ToInt(char abc)
{
	if (abc >=0x30 && abc <=0x39)
	{
		return abc - 0x30;
	} 
	else if (abc >=0x41 && abc <=0x46)
	{
		return abc - 0x41+10;
	}
	else if (abc >=0x61 && abc <=0x66)
	{
		return abc - 0x61+10;
	}


}