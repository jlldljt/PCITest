// DlgPriview.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgPriview.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CDlgPriview �Ի���

IMPLEMENT_DYNAMIC(CDlgPriview, CDialogEx)

CDlgPriview::CDlgPriview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPriview::IDD, pParent)
{
  m_p = (CCamera*)pParent;
  m_p->m_param->camera.Nxy = 0;
}

CDlgPriview::~CDlgPriview()
{
  m_p->m_param->camera.Nxy = 0;
}

void CDlgPriview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriview, CDialogEx)
  ON_WM_LBUTTONDOWN()
//  ON_WM_SETCURSOR()
ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgPriview ��Ϣ�������


void CDlgPriview::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
  if (0 >= m_p->m_rcg.g_stu_square.nBMPW || 0 >= m_p->m_rcg.g_stu_square.nBMPH) {
    AfxMessageBox(_T("�ȵ��ʶ�����"));
    return;
  }

  if (0 == m_p->m_rcg.g_stu_square.nN)
  {
    //return;
  }
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
  double temp_y = pt.y -lRect.top;
  xt =/*(int)*/(temp_x * m_p->m_rcg.g_stu_square.nBMPW / (lRect.Width()));//����������Ӧ��ͼ���ϵ�x��
  yt =/*(int)*/(temp_y * m_p->m_rcg.g_stu_square.nBMPH / (lRect.Height()));//����������Ӧ��ͼ���ϵ�y��
  //if(!m_RadioCircle && m_RadioSquare)
  {
    //if(avg1.empty()){AfxMessageBox(_T("û�ж�λ���꣬�붨λ"));return;}

    int temp = 0;
    int i;

    for (i = 0; i < m_p->m_rcg.g_stu_square.nN; i++)
    {

      if (sqrt(pow(double(m_p->m_rcg.g_stu_square.pnZPX[i] - xt), 2) + pow(double(m_p->m_rcg.g_stu_square.pnZPY[i] - yt), 2)) < ((m_p->m_rcg.g_stu_square.pnWth[i]) >> 1)) //���λ����Ƭ�Ĵ����������С��s_sort.allowable_devision����ô����Ҫ���Ե��㲻׼ʱ���õ���
      {
        //DrawPicToHDC(line_img, IDC_PREVIEW);//��ͼƬ�ؼ�����ʾ
        CWnd* pWnd = this;
        CDC* pDC = pWnd->GetDC();
        CRect rect,rect_camera;
        GetClientRect(&rect);
        m_p->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
        CPen PenRed;
        PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//����һ֧��ɫ�Ļ���.
        pDC->SelectObject(PenRed);//ѡ�л���.
        pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), m_p->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
        if (m_p->m_param->camera.bIsCir)
        {
          CString csTmp;
          csTmp.Format(_T("�뾶%d"), m_p->m_rcg.g_stu_square.pnWth[i]);
          GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          pDC->Ellipse
          (m_p->m_rcg.g_stu_square.pnZPX[i] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW - m_p->m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW
            , m_p->m_rcg.g_stu_square.pnZPY[i] * rect.Height() / m_p->m_rcg.g_stu_square.nBMPH - m_p->m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW
            , m_p->m_rcg.g_stu_square.pnZPX[i] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW + m_p->m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW
            , m_p->m_rcg.g_stu_square.pnZPY[i] * rect.Height() / m_p->m_rcg.g_stu_square.nBMPH + m_p->m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW
          );
          //���浽��������
          m_p->m_rcg.stuRef.Rad = m_p->m_rcg.g_stu_square.pnWth[i];
        }
        else
        {
          pt.x = m_p->m_rcg.g_stu_square.pnPX[i][3] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW;
          pt.y = m_p->m_rcg.g_stu_square.pnPY[i][3] * rect.Height() / m_p->m_rcg.g_stu_square.nBMPH;
          pDC->MoveTo(pt);
          //
          POINT pPoint[5];
          for (int j = 0; j < 4; j++)
          {
            pPoint[j].x = m_p->m_rcg.g_stu_square.pnPX[i][j] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW;
            pPoint[j].y = m_p->m_rcg.g_stu_square.pnPY[i][j] * rect.Height() / m_p->m_rcg.g_stu_square.nBMPH;
          }
          pPoint[4].x = m_p->m_rcg.g_stu_square.pnPX[i][0] * rect.Width() / m_p->m_rcg.g_stu_square.nBMPW;
          pPoint[4].y = m_p->m_rcg.g_stu_square.pnPY[i][0] * rect.Height() / m_p->m_rcg.g_stu_square.nBMPH;

          pDC->Polygon(pPoint, 4);
          //test
          double angle_test = m_p->m_rcg.CalculateVectorAngle(pPoint[1].x - pPoint[0].x,
            pPoint[1].y - pPoint[0].y, 1, 0);
          /*int nLenNo = m_p->m_rcg.g_stu_square.lenNo1PN[i];
          int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
          */
          int nLenNo = 0;
          int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
          pDC->Ellipse(pPoint[nLenNo].x - 2, pPoint[nLenNo].y - 2, pPoint[nLenNo].x + 2, pPoint[nLenNo].y + 2);

          pDC->Ellipse(pPoint[nLenNoNext].x - 4, pPoint[nLenNoNext].y - 4, pPoint[nLenNoNext].x + 4, pPoint[nLenNoNext].y + 4);

          //
          m_p->m_rcg.stuRef.Len = m_p->m_rcg.g_stu_square.pnLen[i];
          m_p->m_rcg.stuRef.Wth = m_p->m_rcg.g_stu_square.pnWth[i];
          m_p->m_param->camera.nPN = m_p->m_rcg.g_stu_square.bPN[i];


          CString csTmp;
          csTmp.Format(_T("��%d��%dX�Ƕ�%.1f"), m_p->m_rcg.g_stu_square.pnLen[i], m_p->m_rcg.g_stu_square.pnWth[i], (angle_test));

          //csTmp.Format(_T("��%d��%dX����%d�Ƕ�%.1f"),m_p->m_rcg.g_stu_square.pnLen[i],m_p->m_rcg.g_stu_square.pnWth[i],m_p->m_rcg.g_stu_square.bPN[i], m_p->m_rcg.g_stu_square.angreePN[i]);
          m_p->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          //AfxMessageBox(csTmp);
        }
        pWnd->ReleaseDC(pDC);
        m_p->m_rcg.stuRef.Dev = m_p->m_param->camera.nAllowDefect;
        m_p->m_param->camera.Xxy[m_p->m_param->camera.Nxy][0] = m_p->m_rcg.g_stu_square.pnZPX[i]; m_p->m_param->camera.Xxy[m_p->m_param->camera.Nxy][1] = m_p->m_rcg.g_stu_square.pnZPY[i];//xy�������һ���Ķ�������ͬλ�ã���ͬxy���Ƿ������һ���������ԣ�0,1��ͼ��x��y��2�ǹ̶�1��3�ǵ������
        m_p->m_param->camera.Yxy[m_p->m_param->camera.Nxy][0] = m_p->m_rcg.g_stu_square.pnZPX[i]; m_p->m_param->camera.Yxy[m_p->m_param->camera.Nxy][1] = m_p->m_rcg.g_stu_square.pnZPY[i];
        int X = m_p->m_rcg.g_stu_square.pnZPX[i] + 0.5;
        int Y = m_p->m_rcg.g_stu_square.pnZPY[i] + 0.5;
        /*
                int X = m_p->m_param->camera.g_factor[0][0] * (double)m_p->m_rcg.g_stu_square.pnZPX[i] + m_p->m_param->camera.g_factor[0][1] * (double)m_p->m_rcg.g_stu_square.pnZPY[i] + m_p->m_param->camera.g_factor[0][2];
                int Y = m_p->m_param->camera.g_factor[1][0] * (double)m_p->m_rcg.g_stu_square.pnZPX[i] + m_p->m_param->camera.g_factor[1][1] * (double)m_p->m_rcg.g_stu_square.pnZPY[i] + m_p->m_param->camera.g_factor[1][2];*/
        CString csTmp;
        csTmp.Format(_T("X:%d	Y:%dX"), X, Y);
        m_p->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
        m_p->m_param->camera.bClbPos = 1;
        break;
      }

    }

      if(i == m_p->m_rcg.g_stu_square.nN) {
        CWnd* pWnd = this;
        CDC* pDC = pWnd->GetDC();
        CRect rect, rect_camera;
        GetClientRect(&rect);
        m_p->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
        CPen PenRed;
        PenRed.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//����һ֧�Ļ���.
        pDC->SelectObject(PenRed);//ѡ�л���.
        //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), m_p->mDCMem, 0, 0, SRCCOPY);
        pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), m_p->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
        POINT point;//�����
        point.x = temp_x - 10 + 0.5;
        point.y = temp_y + 0.5;
        pDC->MoveTo(point);
        point.x = temp_x + 10 + 0.5;
        pDC->LineTo(point);
        //
        point.x = temp_x + 0.5;
        point.y = temp_y - 10 + 0.5;
        pDC->MoveTo(point);
        point.y = temp_y + 10 + 0.5;
        pDC->LineTo(point);

        pWnd->ReleaseDC(pDC);
        /////////////////////
        CString csTmp;
        csTmp.Format(_T("�� �� �Ƕ�"));
        m_p->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);

        m_p->m_param->camera.Xxy[m_p->m_param->camera.Nxy][0] = xt; m_p->m_param->camera.Xxy[m_p->m_param->camera.Nxy][1] = yt;//xy�������һ���Ķ�������ͬλ�ã���ͬxy���Ƿ������һ���������ԣ�0,1��ͼ��x��y��2�ǹ̶�1��3�ǵ������
        m_p->m_param->camera.Yxy[m_p->m_param->camera.Nxy][0] = xt; m_p->m_param->camera.Yxy[m_p->m_param->camera.Nxy][1] = yt;

        int X = xt + 0.5;
        int Y = yt + 0.5;

        csTmp.Format(_T("X:%d	Y:%dX"), X, Y);
        m_p->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);



        //////
        CString str;

        int x = m_p->m_param->camera.nClbPosX[m_p->m_param->camera.Nxy];
        int y = m_p->m_param->camera.nClbPosY[m_p->m_param->camera.Nxy];

        str.Format(_T("�Ƿ��ͼ������X:%d	Y:%dX �� ��%d��Ԥ���X:%d	Y:%dX������"), X, Y, m_p->m_param->camera.Nxy+1, x, y);
          
        int status = AfxMessageBox(str, MB_OKCANCEL);
        if (status == 1)//��ȷ���˳�
        {
          m_p->m_param->camera.Xxy[m_p->m_param->camera.Nxy][2] = 1;
          m_p->m_param->camera.Yxy[m_p->m_param->camera.Nxy][2] = 1;
          m_p->m_param->camera.Xxy[m_p->m_param->camera.Nxy][3] = x;
          m_p->m_param->camera.Yxy[m_p->m_param->camera.Nxy][3] = y;

          m_p->m_param->camera.Nxy++;

          if (m_p->m_param->camera.Nxy == 3)
          {

            calparameter(m_p->m_param->camera.Xxy, m_p->m_param->camera.g_factor[0]);
            calparameter(m_p->m_param->camera.Yxy, m_p->m_param->camera.g_factor[1]);
            //����
            CString strValue;
            strValue.Format(_T("%.18lf"), m_p->m_param->camera.g_factor[0][0]);
            WritePrivateProfileString(_T("У׼ϵ��"), _T("00"), strValue, g_ini_path);
            strValue.Format(_T("%.18lf"), m_p->m_param->camera.g_factor[0][1]);
            WritePrivateProfileString(_T("У׼ϵ��"), _T("01"), strValue, g_ini_path);
            strValue.Format(_T("%.18lf"), m_p->m_param->camera.g_factor[0][2]);
            WritePrivateProfileString(_T("У׼ϵ��"), _T("02"), strValue, g_ini_path);
            strValue.Format(_T("%.18lf"), m_p->m_param->camera.g_factor[1][0]);
            WritePrivateProfileString(_T("У׼ϵ��"), _T("10"), strValue, g_ini_path);
            strValue.Format(_T("%.18lf"), m_p->m_param->camera.g_factor[1][1]);
            WritePrivateProfileString(_T("У׼ϵ��"), _T("11"), strValue, g_ini_path);
            strValue.Format(_T("%.18lf"), m_p->m_param->camera.g_factor[1][2]);
            WritePrivateProfileString(_T("У׼ϵ��"), _T("12"), strValue, g_ini_path);
            m_p->SetDlgItemText(IDC_SELECT_RESULT, _T("У׼���"));
            AfxMessageBox(_T("�����������"), MB_OK);
            m_p->m_param->camera.Nxy = 0;
          }
        }
        else
        {
          //m_p->m_param->camera.Nxy=0;
        }
        /////////
      }
    return;
  }

  CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CDlgPriview::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  �ڴ���Ӷ���ĳ�ʼ��

  return TRUE;  // return TRUE unless you set the focus to a control
                // �쳣: OCX ����ҳӦ���� FALSE
}


//BOOL CDlgPriview::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//  return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
//}


void CDlgPriview::OnPaint()
{
  CPaintDC dc(this); // device context for painting
                     // TODO: �ڴ˴������Ϣ����������
                     // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
  CRect wnd_rc, rect_camera;
  GetClientRect(&wnd_rc);
  m_p->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
  dc.TransparentBlt/*BitBlt*/(wnd_rc.left, wnd_rc.top, wnd_rc.Width(), wnd_rc.Height(), m_p->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);

}
