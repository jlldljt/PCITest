// DlgPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "BER.h"
#include "DlgPassword.h"
#include "afxdialogex.h"


// CDlgPassword 对话框

IMPLEMENT_DYNAMIC(CDlgPassword, CDialogEx)

CDlgPassword::CDlgPassword( CStandardLib * pLib,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPassword::IDD, pParent)
{
  ASSERT(pLib);
  
  m_pLib = pLib;
}

CDlgPassword::~CDlgPassword()
{
}

void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialogEx)
  ON_BN_CLICKED(IDOK, &CDlgPassword::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &CDlgPassword::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgPassword 消息处理程序


void CDlgPassword::OnBnClickedOk()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_EDT_PASSWORD, str);

  if (TRUE == m_pLib->CheckPassword(str))
  {
    CDialogEx::OnOK();
  }
  else
  {
    CDialogEx::OnCancel();
  }

}


void CDlgPassword::OnBnClickedCancel()
{
  // TODO: 在此添加控件通知处理程序代码
  CDialogEx::OnCancel();
}
