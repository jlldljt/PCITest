// DlgPriview1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZPJ.h"
#include "DlgPriview1.h"
#include "afxdialogex.h"


// CDlgPriview1 �Ի���

IMPLEMENT_DYNAMIC(CDlgPriview1, CDialogEx)

CDlgPriview1::CDlgPriview1(CWnd* pParent /*=NULL*/)
  : CDialogEx(CDlgPriview1::IDD, pParent)
{
  m_n = 0;
}

CDlgPriview1::~CDlgPriview1()
{
}

void CDlgPriview1::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriview1, CDialogEx)
  ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgPriview1 ��Ϣ�������


void CDlgPriview1::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

  double xt, yt;
  POINT pt;//�����
  GetCursorPos(&pt);//ȡ�õ�ǰ����
  CRect lRect;
  GetWindowRect(&lRect);

  int y_caption = GetSystemMetrics(SM_CYCAPTION);
  int x_frame = GetSystemMetrics(SM_CXDLGFRAME);
  int y_frame = GetSystemMetrics(SM_CYDLGFRAME);
  lRect.left += x_frame;
  lRect.right -= x_frame;
  lRect.top += y_frame + y_caption;
  lRect.bottom -= y_frame;
  double temp_x = pt.x - lRect.left;
  double temp_y = pt.y - lRect.top;
  xt =/*(int)*/(temp_x * gclsImgRcg.g_stu_square.nBMPW / (lRect.Width()));//����������Ӧ��ͼ���ϵ�x��
  yt =/*(int)*/(temp_y * gclsImgRcg.g_stu_square.nBMPH / (lRect.Height()));//����������Ӧ��ͼ���ϵ�y��




  CWnd* pWnd = this;
  CDC* pDC = pWnd->GetDC();
  CRect rect, rect_camera;
  GetClientRect(&rect);
  g_dlgCamera->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
  CPen PenRed;
  PenRed.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//����һ֧�Ļ���.
  pDC->SelectObject(PenRed);//ѡ�л���.
  //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), g_dlgCamera->mDCMem, 0, 0, SRCCOPY);
  pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), g_dlgCamera->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
  POINT pt_dc;//�����
  pt_dc.x = temp_x - 10 + 0.5;
  pt_dc.y = temp_y + 0.5;
  pDC->MoveTo(pt_dc);
  pt_dc.x = temp_x + 10 + 0.5;
  pDC->LineTo(pt_dc);
  //
  pt_dc.x = temp_x + 0.5;
  pt_dc.y = temp_y - 10 + 0.5;
  pDC->MoveTo(pt_dc);
  pt_dc.y = temp_y + 10 + 0.5;
  pDC->LineTo(pt_dc);

  pWnd->ReleaseDC(pDC);
  /////////////////////
  CString csTmp;

  int X = xt + 0.5;
  int Y = yt + 0.5;

  csTmp.Format(_T("X:%d	Y:%dX"), X, Y);



  //////
  CString str;

  if (m_n == 0) {
    str.Format(_T("�Ƿ��ͼ������X:%d	Y:%d ��Ϊ���Ͻ�"), X, Y);
    g_npc_inf.left = X;
    g_npc_inf.top = Y;
  }
  else if (m_n == 1) {
    str.Format(_T("�Ƿ��ͼ������X:%d	Y:%d ��Ϊ���½�"), X, Y);
    g_npc_inf.right = X;
    g_npc_inf.bottom = Y;
  }

  int status = AfxMessageBox(str, MB_OKCANCEL);
  if (status == 1)//��ȷ���˳�
  {


    m_n++;

    if (m_n == 2)
    {
      //����
      CString strValue;
      strValue.Format(_T("%d"), g_npc_inf.left);
      WritePrivateProfileString(_T("��Χ����"), _T("left"), strValue, gstuPathInf.csPathIni);
      strValue.Format(_T("%d"), g_npc_inf.top);
      WritePrivateProfileString(_T("��Χ����"), _T("top"), strValue, gstuPathInf.csPathIni);
      strValue.Format(_T("%d"), g_npc_inf.right);
      WritePrivateProfileString(_T("��Χ����"), _T("right"), strValue, gstuPathInf.csPathIni);
      strValue.Format(_T("%d"), g_npc_inf.bottom);
      WritePrivateProfileString(_T("��Χ����"), _T("bottom"), strValue, gstuPathInf.csPathIni);

      AfxMessageBox(_T("�����������"), MB_OK);
      m_n = 0;
    }
  }
  else
  {
    //gstuRcgInfo.Nxy=0;
  }

  CDialogEx::OnLButtonDown(nFlags, point);
}
