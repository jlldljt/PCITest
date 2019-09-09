// DlgPriview1.cpp : ʵ���ļ�
//
/*
Ҫ�ھ�̬����ʹ��crecttracker

#define _AFX_NO_SPLITTER_RESOURCES
#define _AFX_NO_OLE_RESOURCES
#define _AFX_NO_TRACKER_RESOURCES
#define _AFX_NO_PROPERTY_RESOURCES

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_CHS)
LANGUAGE 4, 2
#include "res\BER.rc2"  // �� Microsoft Visual C++ �༭����Դ
#include "l.CHS\afxres.rc"      // ��׼���
#endif


*/

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgPriview1.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CDlgPriview1 �Ի���

IMPLEMENT_DYNAMIC(CDlgPriview1, CDialogEx)

CDlgPriview1::CDlgPriview1(CWnd* pParent /*=NULL*/)
  : CDialogEx(CDlgPriview1::IDD, pParent)
{
  m_n = 0;

  m_p = (CCamera*)pParent;
  bDraw = FALSE;

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
  ON_WM_PAINT()
  ON_WM_SETCURSOR()
  ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgPriview1 ��Ϣ�������
void CDlgPriview1::Process()
{
}

void CDlgPriview1::OnLButtonDown(UINT nFlags, CPoint point)
{
  int nRetCode = m_RectTracker.HitTest(point);

  if (nRetCode < 0)            //���ھ�������
  {
    //��temp������ʼ��
    CRectTracker  temp;
    temp.m_rect.SetRect(0, 0, 0, 0);
    temp.m_nStyle = CRectTracker::hatchedBorder | CRectTracker::resizeInside;

    //TrackRubberBand����ֱ�������������ʱ��Ż᷵�أ�������������λ��Ҳ��¼������
    //�ú��������У������������ƶ���Ϣ
    temp.TrackRubberBand(this, point, TRUE);
    //ʹ�þ���Ϊ��ȷ�����꣬������ָ�������
    temp.m_rect.NormalizeRect();
    //��ʱ��bDraw��Ϊfalse
    bDraw = FALSE;
    CClientDC dc(this);
    //���µľ���������Ƴ���
    temp.Draw(&dc);
    m_RectTracker.m_rect = temp.m_rect;
    Invalidate(TRUE);
  }
  else                        //�ھ���������
  {
    //Track��TrackRubberBand���ƣ�ֱ�������������ʱ��Ż᷵�أ�������������λ��Ҳ��¼������
    //�ú��������У������������ƶ���Ϣ
    m_RectTracker.Track(this, point, TRUE);
    //����Ϊtrue,����Ҫ�ھ����ڻ�����Բ
    bDraw = TRUE;

    //������Ч������Onpaint����
    Invalidate(TRUE);
  }

  CDialogEx::OnLButtonDown(nFlags, point);
}
//void CDlgPriview1::OnLButtonDown(UINT nFlags, CPoint point)
//{
//  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//  if (0 == m_p->m_rcg.g_stu_square.nBMPW || 0 == m_p->m_rcg.g_stu_square.nBMPH) {
//    AfxMessageBox(_T("�ȵ��ʶ�����"));
//    return;
//  }
//  double xt, yt;
//  POINT pt;//�����
//  GetCursorPos(&pt);//ȡ�õ�ǰ����
//  CRect lRect;
//  GetWindowRect(&lRect);
//
//  int y_caption = GetSystemMetrics(SM_CYCAPTION);
//  int x_frame = GetSystemMetrics(SM_CXDLGFRAME);
//  int y_frame = GetSystemMetrics(SM_CYDLGFRAME);
//  lRect.left += x_frame;
//  lRect.right -= x_frame;
//  lRect.top += y_frame + y_caption;
//  lRect.bottom -= y_frame;
//  double temp_x = pt.x - lRect.left;
//  double temp_y = pt.y - lRect.top;
//  xt =/*(int)*/(temp_x * m_p->m_rcg.g_stu_square.nBMPW / (lRect.Width()));//����������Ӧ��ͼ���ϵ�x��
//  yt =/*(int)*/(temp_y * m_p->m_rcg.g_stu_square.nBMPH / (lRect.Height()));//����������Ӧ��ͼ���ϵ�y��
//
//
//
//
//  CWnd* pWnd = this;
//  CDC* pDC = pWnd->GetDC();
//  CRect rect, rect_camera;
//  GetClientRect(&rect);
//  m_p->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
//  CPen PenRed;
//  PenRed.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//����һ֧�Ļ���.
//  pDC->SelectObject(PenRed);//ѡ�л���.
//  CBrush br;
//  br.CreateStockObject(NULL_BRUSH);
//  pDC->SelectObject(&br);
//
//  //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), m_p->mDCMem, 0, 0, SRCCOPY);
//  pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), m_p->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
//  POINT pt_dc;//�����
//  pt_dc.x = temp_x - 10 + 0.5;
//  pt_dc.y = temp_y + 0.5;
//  pDC->MoveTo(pt_dc);
//  pt_dc.x = temp_x + 10 + 0.5;
//  pDC->LineTo(pt_dc);
//  //
//  pt_dc.x = temp_x + 0.5;
//  pt_dc.y = temp_y - 10 + 0.5;
//  pDC->MoveTo(pt_dc);
//  pt_dc.y = temp_y + 10 + 0.5;
//  pDC->LineTo(pt_dc);
//  /////////////////////
//  CString csTmp;
//
//  int X = xt + 0.5;
//  int Y = yt + 0.5;
//
//  csTmp.Format(_T("X:%d	Y:%dX"), X, Y);
//
//
//
//  //////
//  CString str;
//
//  if (m_n == 0) {
//    str.Format(_T("�Ƿ��ͼ������X:%d	Y:%d ��Ϊ���Ͻ�"), X, Y);
//   // m_p->m_npc_inf.left = X;
//   // m_p->m_npc_inf.top = Y;
//  }
//  else if (m_n == 1) {
//    str.Format(_T("�Ƿ��ͼ������X:%d	Y:%d ��Ϊ���½�"), X, Y);
//   // m_p->m_npc_inf.right = X;
//   // m_p->m_npc_inf.bottom = Y;
//  }
//
//  pDC->Rectangle(m_p->m_npc_inf.left * lRect.Width()/m_p->m_rcg.g_stu_square.nBMPW  
//    , m_p->m_npc_inf.top *  lRect.Height()/m_p->m_rcg.g_stu_square.nBMPH 
//    , m_p->m_npc_inf.right *  lRect.Width()/m_p->m_rcg.g_stu_square.nBMPW 
//    , m_p->m_npc_inf.bottom *lRect.Height()  / m_p->m_rcg.g_stu_square.nBMPH);
//  pWnd->ReleaseDC(pDC);
//
//  int status = AfxMessageBox(str, MB_OKCANCEL);
//  if (status == 1)//��ȷ���˳�
//  {
//
//    if (m_n == 0) {
//      m_p->m_npc_inf.left = X;
//      m_p->m_npc_inf.top = Y;
//    }
//    else if (m_n == 1) {
//      m_p->m_npc_inf.right = X;
//      m_p->m_npc_inf.bottom = Y;
//    }
//
//    m_n++;
//
//    if (m_n == 2)
//    {
//      //����
//      CString strValue;
//      strValue.Format(_T("%d"), m_p->m_npc_inf.left);
//      WritePrivateProfileString(_T("��Χ����"), _T("left"), strValue, g_ini_path);
//      strValue.Format(_T("%d"), m_p->m_npc_inf.top);
//      WritePrivateProfileString(_T("��Χ����"), _T("top"), strValue, g_ini_path);
//      strValue.Format(_T("%d"), m_p->m_npc_inf.right);
//      WritePrivateProfileString(_T("��Χ����"), _T("right"), strValue, g_ini_path);
//      strValue.Format(_T("%d"), m_p->m_npc_inf.bottom);
//      WritePrivateProfileString(_T("��Χ����"), _T("bottom"), strValue, g_ini_path);
//
//      AfxMessageBox(_T("�����������"), MB_OK);
//      m_n = 0;
//    }
//  }
//  else
//  {
//    //gstuRcgInfo.Nxy=0;
//  }
//
//  CDialogEx::OnLButtonDown(nFlags, point);
//}


BOOL CDlgPriview1::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  �ڴ���Ӷ���ĳ�ʼ��
  if (0 == m_p->m_rcg.g_stu_square.nBMPW || 0 == m_p->m_rcg.g_stu_square.nBMPH) {
    AfxMessageBox(_T("�ȵ��ʶ�����"));
    //DestroyWindow();
    return FALSE;
  }

  CRect rect;

  GetClientRect(&rect);
  
  m_RectTracker.m_rect.SetRect(m_p->m_npc_inf.left* rect.Width() / m_p->m_rcg.g_stu_square.nBMPW
    , m_p->m_npc_inf.top* rect.Height() / m_p->m_rcg.g_stu_square.nBMPH
    , m_p->m_npc_inf.right* rect.Width() / m_p->m_rcg.g_stu_square.nBMPW
    , m_p->m_npc_inf.bottom* rect.Height() / m_p->m_rcg.g_stu_square.nBMPH);

    m_RectTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside;

  return TRUE;  // return TRUE unless you set the focus to a control
                // �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgPriview1::OnPaint()
{
  CPaintDC dc(this); // device context for painting
                     // TODO: �ڴ˴������Ϣ����������
                     // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
  CBrush brush(RGB(0, 0, 255));
  CBrush* oldBrush = dc.SelectObject(&brush);
  CRect rect,wnd_rc, rect_camera;

  GetClientRect(&wnd_rc);
  m_p->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
  dc.TransparentBlt/*BitBlt*/(wnd_rc.left, wnd_rc.top, wnd_rc.Width(), wnd_rc.Height(), m_p->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
  // 

  m_RectTracker.GetTrueRect(&rect); //�õ�CRectTracker�е�m_rect�Ĵ�С�����䴫�ݸ�rect��

  if (bDraw)
  {
    //dc.Ellipse(&rect);
  }

  m_RectTracker.Draw(&dc);            //��仭�������Ľ�����ı��λ�������

  dc.SelectObject(oldBrush);

  //

}


BOOL CDlgPriview1::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

  // �ı������״
  if ((pWnd == this) && (m_RectTracker.SetCursor(this, nHitTest)))
  {
    return TRUE;
  }

  return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CDlgPriview1::OnDestroy()
{
  CDialogEx::OnDestroy();

  // TODO: �ڴ˴������Ϣ����������
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
  if (0 == m_p->m_rcg.g_stu_square.nBMPW || 0 == m_p->m_rcg.g_stu_square.nBMPH) {
    return;
  }

  CRect rect;
  CRect wnd_rc;

  m_RectTracker.GetTrueRect(&rect); //�õ�CRectTracker�е�m_rect�Ĵ�С�����䴫�ݸ�rect��
  GetClientRect(&wnd_rc);

  m_p->m_npc_inf.left = (rect.left * m_p->m_rcg.g_stu_square.nBMPW / (wnd_rc.Width()));
  m_p->m_npc_inf.top=(rect.top * m_p->m_rcg.g_stu_square.nBMPH / (wnd_rc.Height()));
  m_p->m_npc_inf.right=(rect.right * m_p->m_rcg.g_stu_square.nBMPW / (wnd_rc.Width()));
  m_p->m_npc_inf.bottom=(rect.bottom * m_p->m_rcg.g_stu_square.nBMPH / (wnd_rc.Height()));


      //����
      CString strValue;
      strValue.Format(_T("%d"), m_p->m_npc_inf.left);
      WritePrivateProfileString(_T("��Χ����"), _T("left"), strValue, g_ini_path);
      strValue.Format(_T("%d"), m_p->m_npc_inf.top);
      WritePrivateProfileString(_T("��Χ����"), _T("top"), strValue, g_ini_path);
      strValue.Format(_T("%d"), m_p->m_npc_inf.right);
      WritePrivateProfileString(_T("��Χ����"), _T("right"), strValue, g_ini_path);
      strValue.Format(_T("%d"), m_p->m_npc_inf.bottom);
      WritePrivateProfileString(_T("��Χ����"), _T("bottom"), strValue, g_ini_path);


}
