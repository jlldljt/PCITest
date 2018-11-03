
// ATtoSC.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ATtoSC.h"
#include "ATtoSCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//全局变量
CString g_path;
CString pathSaveR1;//=(_T(".\\check\\R1Number.txt"));
CString pathSaveDegree;//=(_T(".\\check\\SortDegree.txt"));
threadstatus g_status;//线程全局结构变量
sortchip g_sort;//分档全局结构变量
stuTime g_tim;
//runtime g_time;//时间全局变量
HANDLE g_hDevice;//AC6641句柄
int degree[30];//保存识别出的数据
bool updatesort;//分档数据刷新标志
bool debugupdate;//debug页面刷新标志
int *g_num=NULL;//保存10个数的0,1分布
int g_height;//图像的宽高  ，宽所占字节  
int	g_width;
int g_widthBytes;
BYTE *g_lpGray=NULL;//保存二值化后的图像数据
int head[10][8],height_num,width_num;//图片标志数据
int g_shakeE;

RGBCAPTUREBUFFER pCaptureBuffer;//读屏幕中用到
RGBBITMAP       RGBBitmap1;     //第一个采集事件位图信息
TPicRegion pDest;				//读屏幕中用到
double m_VGAthreshold;			//读屏幕中用到
//double m_VGAthreshold;
// CATtoSCApp

BEGIN_MESSAGE_MAP(CATtoSCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CATtoSCApp 构造

CATtoSCApp::CATtoSCApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CATtoSCApp 对象

CATtoSCApp theApp;


// CATtoSCApp 初始化

BOOL CATtoSCApp::InitInstance()
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

	CATtoSCDlg dlg;
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

