
// ATtoSC.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ATtoSC.h"
#include "ATtoSCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//ȫ�ֱ���
CString g_path;
CString pathSaveR1;//=(_T(".\\check\\R1Number.txt"));
CString pathSaveDegree;//=(_T(".\\check\\SortDegree.txt"));
threadstatus g_status;//�߳�ȫ�ֽṹ����
sortchip g_sort;//�ֵ�ȫ�ֽṹ����
stuTime g_tim;
//runtime g_time;//ʱ��ȫ�ֱ���
HANDLE g_hDevice;//AC6641���
int degree[30];//����ʶ���������
bool updatesort;//�ֵ�����ˢ�±�־
bool debugupdate;//debugҳ��ˢ�±�־
int *g_num=NULL;//����10������0,1�ֲ�
int g_height;//ͼ��Ŀ��  ������ռ�ֽ�  
int	g_width;
int g_widthBytes;
BYTE *g_lpGray=NULL;//�����ֵ�����ͼ������
int head[10][8],height_num,width_num;//ͼƬ��־����
int g_shakeE;

RGBCAPTUREBUFFER pCaptureBuffer;//����Ļ���õ�
RGBBITMAP       RGBBitmap1;     //��һ���ɼ��¼�λͼ��Ϣ
TPicRegion pDest;				//����Ļ���õ�
double m_VGAthreshold;			//����Ļ���õ�
//double m_VGAthreshold;
// CATtoSCApp

BEGIN_MESSAGE_MAP(CATtoSCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CATtoSCApp ����

CATtoSCApp::CATtoSCApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CATtoSCApp ����

CATtoSCApp theApp;


// CATtoSCApp ��ʼ��

BOOL CATtoSCApp::InitInstance()
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

	CATtoSCDlg dlg;
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

