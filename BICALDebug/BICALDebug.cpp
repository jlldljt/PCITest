
// BICALDebug.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "BICALDebug.h"
#include "BICALDebugDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CWinThread* gTrdRun;//
CWinThread* gTrdCom;//com�߳�
UINT RunThread(LPVOID pParam);//�����̺߳���
bool update = 0,runbool = 1;
UINT RunThread(LPVOID pParam)
{
	UINT cmd,addr,num,grid=0,maxgrid,cnt=0,cntnum=3;
	//ˢ��
	num = TestGrid(3,0,1);//���Ĵ���
	maxgrid = num;
	g_dlgDebug1->SetDlgItemInt(IDC_XGRID,num,0);
	num = TestGrid(3,1,1);//���Ĵ���
	maxgrid *=num;
	g_dlgDebug1->SetDlgItemInt(IDC_YGRID,num,0);
	num = TestGrid(3,2,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_X1,num,0);
	num = TestGrid(3,3,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_Y1,num,0);
	num = TestGrid(3,4,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_XEND,num,0);
	num = TestGrid(3,5,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_YEND,num,0);
	num = TestGrid(3,6,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_XMIN,num,0);
	num = TestGrid(3,7,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_YMIN,num,0);
	num = TestGrid(3,8,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_Xlimit,num,0);
	num = TestGrid(3,9,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_Ylimit,num,0);
	num = TestGrid(3,10,1);//���Ĵ���
	grid = num;
	g_dlgDebug1->SetDlgItemInt(IDC_STEP,num,0);

	num = TestGrid(3,11,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
	num = TestGrid(3,12,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
	num = TestGrid(3,13,1);//���Ĵ���
	g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);
	num = TestGrid(1,0,16);//������
	g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);

	while (runbool)
	{

		if (update)
		{
	//��
			update = 0;
			num = g_dlgDebug1->GetDlgItemInt(IDC_XGRID,0,0);
			TestGrid(2,0,num);//д�Ĵ���
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
			
		
		//ˢ��
			num = TestGrid(3,0,1);//���Ĵ���
			maxgrid = num;
			g_dlgDebug1->SetDlgItemInt(IDC_XGRID,num,0);
			num = TestGrid(3,1,1);//���Ĵ���
			maxgrid *=num;
			g_dlgDebug1->SetDlgItemInt(IDC_YGRID,num,0);
			num = TestGrid(3,2,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_X1,num,0);
			num = TestGrid(3,3,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_Y1,num,0);
			num = TestGrid(3,4,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_XEND,num,0);
			num = TestGrid(3,5,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_YEND,num,0);
			num = TestGrid(3,6,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_XMIN,num,0);
			num = TestGrid(3,7,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_YMIN,num,0);
			num = TestGrid(3,8,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_Xlimit,num,0);
			num = TestGrid(3,9,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_Ylimit,num,0);
			num = TestGrid(3,10,1);//���Ĵ���
			grid = num;
			g_dlgDebug1->SetDlgItemInt(IDC_STEP,num,0);
			num = TestGrid(3,11,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
			num = TestGrid(3,12,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
			num = TestGrid(3,13,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);
			num = TestGrid(1,0,16);//������
			g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);
	
		}
		TestGrid(2,14,1000);
		TestGrid(2,15,1000);
		//xyz����
		TestGrid(0,2,0xff00);
		TestGrid(0,2,0x0000);
		do 
		{
			num = TestGrid(1,0,16);//������
			g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);
			Sleep(1);
		} while (0x03 != ((num>>12)&0x03));
		num = TestGrid(3,11,1);//���Ĵ���
		g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
		num = TestGrid(3,12,1);//���Ĵ���
		g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
		num = TestGrid(3,13,1);//���Ĵ���
		g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);
		Sleep(100);
		//xyz����
		TestGrid(0,1,0xff00);
		TestGrid(0,1,0x0000);
		do 
		{
			num = TestGrid(1,0,16);//������
			g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);
			Sleep(1);
		} while (0x03 != ((num>>12)&0x03));
		num = TestGrid(3,11,1);//���Ĵ���
		g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
		num = TestGrid(3,12,1);//���Ĵ���
		g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
		num = TestGrid(3,13,1);//���Ĵ���
		g_dlgDebug1->SetDlgItemInt(IDC_YPOS,num,0);
		//�����߹���
		
		cnt == cntnum?cnt=0,grid++:cnt++;
		if (grid<1 || grid >maxgrid)
		{
			grid = 1;
		}
		g_dlgDebug1->SetDlgItemInt(IDC_STEP,grid,0);
		
		TestGrid(2,10,grid);
		
		
		//xyȡ��
		TestGrid(0,0,0xff00);
		TestGrid(0,0,0x0000);
		do 
		{
			num = TestGrid(1,0,16);//������
			g_dlgDebug1->SetDlgItemInt(IDC_XYSTOP,(num>>12)&0x03,0);
			Sleep(1);
		} while (0x03 != ((num>>12)&0x03));
		//�ж��Ƿ��߶�
		do 
		{
			num = TestGrid(3,11,1);//���Ĵ���
			g_dlgDebug1->SetDlgItemInt(IDC_CURRENT,num,0);
			Sleep(1);
		} while (num != grid);
		
		
		num = TestGrid(3,12,1);//���Ĵ���
		g_dlgDebug1->SetDlgItemInt(IDC_XPOS,num,0);
		num = TestGrid(3,13,1);//���Ĵ���
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
//ȫ��
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
//����10
0x01,0x05,0x00,0x25,0xFF,0x00,0x00,
0x01,0x05,0x00,0x25,0x00,0x00,0x00,
//����12
0x01,0x05,0x00,0x24,0xFF,0x00,0x00,
0x01,0x05,0x00,0x24,0x00,0x00,0x00,
//����14
0x01,0x05,0x00,0x23,0xFF,0x00,0x00,
0x01,0x05,0x00,0x23,0x00,0x00,0x00,
//��ѯ16
0x01,0x02,0x00,0x10,0x00,0x30,0x00,
//��λxy17
0x01,0x06,0x00,0x3d,0xff,0xff,0x00,
0x01,0x06,0x00,0x3e,0xff,0xff,0x00,
//xy����19
0x01,0x06,0x00,0x18,0x0f,0xff,0x00,
0x01,0x06,0x00,0x19,0x1f,0xff,0x00,
0x01,0x05,0x00,0x27,0xFF,0x00,0x00,
0x01,0x05,0x00,0x27,0x00,0x00,0x00,
}
;
char g_cmd[][2]={
	0x01,0x05,//д��Ȧ    0
	0x01,0x02,//������
	0x01,0x06,//д�Ĵ���
	0x01,0x03,//���Ĵ���   3
};
char g_addr_05[][2]={
	
	0x00,0x26,//xy�ƶ�   0
	0x00,0x25,//xyz����  1
	0x00,0x28,//xyz����  2
};
char g_addr_02[][2]={
	0x00,0x10,//xy��ѯ   0
};
char g_addr_06[][2]={
	0x00,0x0d,//x grids  0
	0x00,0x0e,//y grids 
	0x00,0x0a,//x 1��
	0x00,0x0b,//y 1��
	0x00,0x3a,//x ĩ��   
	0x00,0x3b,//y ĩ��   5
	0x00,0x42,//x ����
	0x00,0x43,//y ����
	0x00,0x3d,//x ��
	0x00,0x3e,//y ��
	0x00,0x17,//ȡ�Ϲ���   10
	0x00,0x39,//��ǰ�������� 11
	0x00,0x07,//��ǰxλ��
	0x00,0x08,//��ǰyλ��
	0x00,0x1b,//x����λ��
	0x00,0x1c,//y����λ��
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
	
	g_chCmd[4]=unsigned char(num>>8);				//��λ
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
			//AfxMessageBox(_T("У��ͨ��"));
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

// CBICALDebugApp ����

CBICALDebugApp::CBICALDebugApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CBICALDebugApp ����

CBICALDebugApp theApp;


// CBICALDebugApp ��ʼ��

BOOL CBICALDebugApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CBICALDebugDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

//��������
void Test(int num)
{
	//if (num <0 || num >=3)
	//	return;
	g_chCmd[0]=g_chAllCmd[num][0];
	g_chCmd[1]=g_chAllCmd[num][1];
	g_chCmd[2]=g_chAllCmd[num][2];
	g_chCmd[3]=g_chAllCmd[num][3];
	g_chCmd[4]=g_chAllCmd[num][4];				//������λ
	g_chCmd[5]=g_chAllCmd[num][5];
	g_chCmd[6]=g_chAllCmd[num][6];				//������λ

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
			AfxMessageBox(_T("%dУ��ͨ��"),i);
		}
		gstuRefresh.bComUpdate=1;
	}
}
