// DlgPassword.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BER.h"
#include "DlgPassword.h"
#include "afxdialogex.h"


// CDlgPassword �Ի���

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


// CDlgPassword ��Ϣ�������


void CDlgPassword::OnBnClickedOk()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
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
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CDialogEx::OnCancel();
}
