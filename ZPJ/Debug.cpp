// Debug.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZPJ.h"
#include "Debug.h"
#include "afxdialogex.h"


// CDebug �Ի���

IMPLEMENT_DYNAMIC(CDebug, CDialogEx)

CDebug::CDebug(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebug::IDD, pParent)
{
	g_dlgDebug=this;
}

CDebug::~CDebug()
{
}

void CDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_DEBUG, m_tabDebug);
}
CDebug dlgDebug;
CDebug *g_dlgDebug=NULL;

BEGIN_MESSAGE_MAP(CDebug, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DEBUG, &CDebug::OnTcnSelchangeTabDebug)
END_MESSAGE_MAP()


// CDebug ��Ϣ�������


void CDebug::OnTcnSelchangeTabDebug(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRect tabRect; 
    m_tabDebug.GetClientRect(&tabRect);   
    tabRect.left += 1;   
    tabRect.right -= 1;   
    tabRect.top += 22;   
    tabRect.bottom -= 1;   
    switch (m_tabDebug.GetCurSel())   
    {   
    case 0:   
        g_dlgCamera->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);   
        //g_dlgScreen->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		g_dlgDevice->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
        break;
    case 1:   
        g_dlgCamera->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
        //g_dlgScreen->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		g_dlgDevice->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
        break;  
	//case 2:
	//	g_dlgCamera->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);   
 //       //g_dlgScreen->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	//	g_dlgDevice->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
    default:   
        break;   
	}
	*pResult = 0;
}
