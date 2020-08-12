
// BICALDebug.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "BICALDebug.h"
#include "BICALDebugDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CWinThread* gTrdRun;//
CWinThread* gTrdCom;//com线程
UINT RunThread(LPVOID pParam);//运行线程函数
bool update = 0,runbool = 1;
UINT RunThread(LPVOID pParam)
{
	UINT cmd,addr,num,grid=0,maxgrid,cnt=0,cntnum=3;
	//刷新
	num = TestGrid(3,0,1);//读寄存器
	maxgrid = num;
	g_dlgDebug1->SetDlgItemInt(IDC_XGRID,num,0);
	num = TestGrid(3,1,1);//读寄存器
	maxgrid *=num;
	g_dlgDebug1->SetDlgItemInt(IDC_YGRID,num,0);
	num = TestGrid(3,2,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_X1,num,0);
	num = TestGrid(3,3,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_Y1,num,0);
	num = TestGrid(3,4,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_XEND,num,0);
	num = TestGrid(3,5,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_YEND,num,0);
	num = TestGrid(3,6,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_XMIN,num,0);
	num = TestGrid(3,7,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_YMIN,num,0);
	num = TestGrid(3,8,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_Xlimit,num,0);
	num = TestGrid(3,9,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_Ylimit,num,0);
	num = TestGrid(3,10,1);//读寄存器
	grid = num;
	g_dlgDebug1->SetDlgItemInt(IDC_STEP,num,0);

	num = TestGrid(3,11,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
	num = TestGrid(3,12,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
	num = TestGrid(3,13,1);//读寄存器
	g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);
	num = TestGrid(1,0,16);//读输入
	g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);

	while (runbool)
	{

		if (update)
		{
	//读
			update = 0;
			num = g_dlgDebug1->GetDlgItemInt(IDC_XGRID,0,0);
			TestGrid(2,0,num);//写寄存器
			//g_dlgDebug1->GetDlgItem(IDC_XGRID)->EnableWindow(FALSE);
			num = g_dlgDebug1->GetDlgItemInt(IDC_YGRID,0,0);
			TestGrid(2,1,num);		

			num = g_dlgDebug1->GetDlgItemInt(IDC_X1,0,0);
			TestGrid(2,2,num);
			num = g_dlgDebug1->GetDlgItemInt(IDC_Y1,0,0);
			TestGrid(2,3,num);
			num = g_dlgDebug1->GetDlgItemInt(IDC_XEND,0,0);
			TestGrid(2,4,num);
			num = g_dlgDebug1->GetDlgItemInt(IDC_YEND,0,0);
			TestGrid(2,5,num);
			num = g_dlgDebug1->GetDlgItemInt(IDC_XMIN,0,0);
			TestGrid(2,6,num);
			num = g_dlgDebug1->GetDlgItemInt(IDC_YMIN,0,0);
			TestGrid(2,7,num);
			num = g_dlgDebug1->GetDlgItemInt(IDC_Xlimit,0,0);
			TestGrid(2,8,num);
			num = g_dlgDebug1->GetDlgItemInt(IDC_Ylimit,0,0);
			TestGrid(2,9,num);
			num = g_dlgDebug1->GetDlgItemInt(IDC_STEP,0,0);
			TestGrid(2,10,num);
			
		
		//刷新
			num = TestGrid(3,0,1);//读寄存器
			maxgrid = num;
			g_dlgDebug1->SetDlgItemInt(IDC_XGRID,num,0);
			num = TestGrid(3,1,1);//读寄存器
			maxgrid *=num;
			g_dlgDebug1->SetDlgItemInt(IDC_YGRID,num,0);
			num = TestGrid(3,2,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_X1,num,0);
			num = TestGrid(3,3,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_Y1,num,0);
			num = TestGrid(3,4,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_XEND,num,0);
			num = TestGrid(3,5,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_YEND,num,0);
			num = TestGrid(3,6,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_XMIN,num,0);
			num = TestGrid(3,7,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_YMIN,num,0);
			num = TestGrid(3,8,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_Xlimit,num,0);
			num = TestGrid(3,9,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_Ylimit,num,0);
			num = TestGrid(3,10,1);//读寄存器
			grid = num;
			g_dlgDebug1->SetDlgItemInt(IDC_STEP,num,0);
			num = TestGrid(3,11,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
			num = TestGrid(3,12,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
			num = TestGrid(3,13,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);
			num = TestGrid(1,0,16);//读输入
			g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);
	
		}
		TestGrid(2,14,1000);
		TestGrid(2,15,1000);
		//xyz上料
		TestGrid(0,2,0xff00);
		TestGrid(0,2,0x0000);
		do 
		{
			num = TestGrid(1,0,16);//读输入
			g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);
			Sleep(1);
		} while (0x03 != ((num>>12)&0x03));
		num = TestGrid(3,11,1);//读寄存器
		g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
		num = TestGrid(3,12,1);//读寄存器
		g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
		num = TestGrid(3,13,1);//读寄存器
		g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);
		Sleep(100);
		//xyz回零
		TestGrid(0,1,0xff00);
		TestGrid(0,1,0x0000);
		do 
		{
			num = TestGrid(1,0,16);//读输入
			g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);
			Sleep(1);
		} while (0x03 != ((num>>12)&0x03));
		num = TestGrid(3,11,1);//读寄存器
		g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
		num = TestGrid(3,12,1);//读寄存器
		g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
		num = TestGrid(3,13,1);//读寄存器
		g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);
		//设置走宫格
		
		cnt == cntnum?cnt=0,grid++:cnt++;
		if (grid<1 || grid >maxgrid)
		{
			grid = 1;
		}
		g_dlgDebug1->SetDlgItemInt(IDC_STEP,grid,0);
		
		TestGrid(2,10,grid);
		
		
		//xy取料
		TestGrid(0,0,0xff00);
		TestGrid(0,0,0x0000);
		do 
		{
			num = TestGrid(1,0,16);//读输入
			g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);
			Sleep(1);
		} while (0x03 != ((num>>12)&0x03));
		//判断是否走对
		do 
		{
			num = TestGrid(3,11,1);//读寄存器
			g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
			Sleep(1);
		} while (num != grid);
		
		
		num = TestGrid(3,12,1);//读寄存器
		g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
		num = TestGrid(3,13,1);//读寄存器
		g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);


	}
	return 0;
}
UINT ComMsg(LPVOID pParam)
{
	
		char buf[20];
		bool crc;
		while(gclsCom.m_struct_com.s_hd!=INVALID_HANDLE_VALUE)
		{
			UINT i = gclsCom.RecvCharFromCom(buf,20,&crc);
			if (crc)
			{
				;
			}
			gstuRefresh.bComUpdate=1;
			Sleep(10);
		}
		
	
	return 1;
}
//全局
CString g_csPath =_T(".\\SYSEFG.ini");
char g_chCmd[5000]; 
RefreshTmp gstuRefresh;
char g_chAllCmd[23][7]={
	//x0
0x01,0x06,0x00,0x04,0x0f,0xff,0x00,
0x01,0x05,0x00,0x1A,0xFF,0x00,0x00,
0x01,0x05,0x00,0x1A,0x00,0x00,0x00,
0x01,0x05,0x00,0x20,0xFF,0x00,0x00,
0x01,0x05,0x00,0x20,0x00,0x00,0x00,
//y5
0x01,0x06,0x00,0x05,0x1f,0xff,0x00,
0x01,0x05,0x00,0x1B,0xFF,0x00,0x00,
0x01,0x05,0x00,0x1B,0x00,0x00,0x00,
0x01,0x05,0x00,0x21,0xFF,0x00,0x00,
0x01,0x05,0x00,0x21,0x00,0x00,0x00,
//归零10
0x01,0x05,0x00,0x25,0xFF,0x00,0x00,
0x01,0x05,0x00,0x25,0x00,0x00,0x00,
//气缸12
0x01,0x05,0x00,0x24,0xFF,0x00,0x00,
0x01,0x05,0x00,0x24,0x00,0x00,0x00,
//吸嘴14
0x01,0x05,0x00,0x23,0xFF,0x00,0x00,
0x01,0x05,0x00,0x23,0x00,0x00,0x00,
//查询16
0x01,0x02,0x00,0x10,0x00,0x30,0x00,
//限位xy17
0x01,0x06,0x00,0x3d,0xff,0xff,0x00,
0x01,0x06,0x00,0x3e,0xff,0xff,0x00,
//xy联动19
0x01,0x06,0x00,0x18,0x0f,0xff,0x00,
0x01,0x06,0x00,0x19,0x1f,0xff,0x00,
0x01,0x05,0x00,0x27,0xFF,0x00,0x00,
0x01,0x05,0x00,0x27,0x00,0x00,0x00,
}
;
char g_cmd[][2]={
	0x01,0x05,//写线圈    0
	0x01,0x02,//读输入
	0x01,0x06,//写寄存器
	0x01,0x03,//读寄存器   3
};
char g_addr_05[][2]={
	
	0x00,0x26,//xy移动   0
	0x00,0x25,//xyz回零  1
	0x00,0x28,//xyz上料  2
};
char g_addr_02[][2]={
	0x00,0x10,//xy查询   0
};
char g_addr_06[][2]={
	0x00,0x0d,//x grids  0
	0x00,0x0e,//y grids 
	0x00,0x0a,//x 1点
	0x00,0x0b,//y 1点
	0x00,0x3a,//x 末点   
	0x00,0x3b,//y 末点   5
	0x00,0x42,//x 负限
	0x00,0x43,//y 负限
	0x00,0x3d,//x 限
	0x00,0x3e,//y 限
	0x00,0x17,//取料宫格   10
	0x00,0x39,//当前所处宫格 11
	0x00,0x07,//当前x位置
	0x00,0x08,//当前y位置
	0x00,0x1b,//x上料位置
	0x00,0x1c,//y上料位置
}
;

unsigned int TestGrid(int cmd,int addr,unsigned int num)
{
	g_chCmd[0]=g_cmd[cmd][0];
	g_chCmd[1]=g_cmd[cmd][1];
	switch(cmd)
	{
	case 0:
		g_chCmd[2]=g_addr_05[addr][0];
		g_chCmd[3]=g_addr_05[addr][1];
		break;
	case 1:
		g_chCmd[2]=g_addr_02[addr][0];
		g_chCmd[3]=g_addr_02[addr][1];
		break;
	case 2:
		g_chCmd[2]=g_addr_06[addr][0];
		g_chCmd[3]=g_addr_06[addr][1];
		break;
	case 3:
		g_chCmd[2]=g_addr_06[addr][0];
		g_chCmd[3]=g_addr_06[addr][1];
		break;
	default:
		g_chCmd[2]=0xff;
		g_chCmd[3]=0xff;
		break;
	}
	
	g_chCmd[4]=unsigned char(num>>8);				//高位
	g_chCmd[5]=unsigned char(num);
	if(gclsCom.m_struct_com.s_hd!=INVALID_HANDLE_VALUE)
	{
		gclsCom.ClearCom();
		gclsCom.SendCharToComNOID(g_chCmd,6);
		//Sleep(100);
		char buf[20];
		bool crc;
		UINT len;
		do{
			len = gclsCom.RecvCharFromCom(buf,20,&crc);
		}while(len<=0);
		num = 0;
		if (crc)
		{
			//AfxMessageBox(_T("校验通过"));
			switch(buf[1])
			{
			case 5:
				break;
			case 2:
				num = ((unsigned char)(buf[4])<<8)+(unsigned char)buf[3];
				break;
			case 6:
				break;
			case 3:
				num = ((unsigned char)(buf[3])<<8)+(unsigned char)buf[4];
				break;
			default:
				break;
			}
		}
		gstuRefresh.bComUpdate=1;
		return num;
	}
}
// CBICALDebugApp

BEGIN_MESSAGE_MAP(CBICALDebugApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CBICALDebugApp 构造

CBICALDebugApp::CBICALDebugApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CBICALDebugApp 对象

CBICALDebugApp theApp;


// CBICALDebugApp 初始化

BOOL CBICALDebugApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CBICALDebugDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

//吹气控制
void Test(int num)
{
	//if (num <0 || num >=3)
	//	return;
	g_chCmd[0]=g_chAllCmd[num][0];
	g_chCmd[1]=g_chAllCmd[num][1];
	g_chCmd[2]=g_chAllCmd[num][2];
	g_chCmd[3]=g_chAllCmd[num][3];
	g_chCmd[4]=g_chAllCmd[num][4];				//步长高位
	g_chCmd[5]=g_chAllCmd[num][5];
	g_chCmd[6]=g_chAllCmd[num][6];				//步长低位

	gclsCom.ClearCom();
	gclsCom.SendCharToComNOID(g_chCmd,6);
	Sleep(100);
	char buf[20];
	bool crc;
	if(gclsCom.m_struct_com.s_hd!=INVALID_HANDLE_VALUE)
	{
		UINT i = gclsCom.RecvCharFromCom(buf,20,&crc);
		if (crc)
		{
			AfxMessageBox(_T("%d校验通过"),i);
		}
		gstuRefresh.bComUpdate=1;
	}
}
