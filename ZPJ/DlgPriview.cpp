// DlgPriview.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZPJ.h"
#include "DlgPriview.h"
#include "Camera.h"
#include "afxdialogex.h"


// CDlgPriview �Ի���

IMPLEMENT_DYNAMIC(CDlgPriview, CDialogEx)

CDlgPriview::CDlgPriview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPriview::IDD, pParent)
{

}

CDlgPriview::~CDlgPriview()
{
}

void CDlgPriview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriview, CDialogEx)
  ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgPriview ��Ϣ�������


void CDlgPriview::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
  if (0 == gclsImgRcg.g_stu_square.nN)
  {
    return;
  }
  double xt, yt;
  POINT pt;//�����
  GetCursorPos(&pt);//ȡ�õ�ǰ����
  CRect lRect;
  GetWindowRect(&lRect);
  
  double temp_x = pt.x - lRect.left;
  double temp_y = pt.y - lRect.top;
  xt =/*(int)*/(temp_x * gclsImgRcg.g_stu_square.nBMPW / (lRect.Width()));//����������Ӧ��ͼ���ϵ�x��
  yt =/*(int)*/(temp_y * gclsImgRcg.g_stu_square.nBMPH / (lRect.Height()));//����������Ӧ��ͼ���ϵ�y��
  //if(!m_RadioCircle && m_RadioSquare)
  {
    //if(avg1.empty()){AfxMessageBox(_T("û�ж�λ���꣬�붨λ"));return;}

    int temp = 0;
    int i;

    for (i = 0; i < gclsImgRcg.g_stu_square.nN; i++)
    {

      if (sqrt(pow(double(gclsImgRcg.g_stu_square.pnZPX[i] - xt), 2) + pow(double(gclsImgRcg.g_stu_square.pnZPY[i] - yt), 2)) < ((gclsImgRcg.g_stu_square.pnWth[i]) >> 1)) //���λ����Ƭ�Ĵ����������С��s_sort.allowable_devision����ô����Ҫ���Ե��㲻׼ʱ���õ���
      {
        //DrawPicToHDC(line_img, IDC_PREVIEW);//��ͼƬ�ؼ�����ʾ
        CWnd* pWnd = this;
        CDC* pDC = pWnd->GetDC();
        CRect rect,rect_camera;
        GetClientRect(&rect);
        g_dlgCamera->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
        CPen PenRed;
        PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//����һ֧��ɫ�Ļ���.
        pDC->SelectObject(PenRed);//ѡ�л���.
        pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), g_dlgCamera->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
        if (gstuRcgInfo.bIsCir)
        {
          CString csTmp;
          csTmp.Format(_T("�뾶%d"), gclsImgRcg.g_stu_square.pnWth[i]);
          GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          pDC->Ellipse
          (gclsImgRcg.g_stu_square.pnZPX[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW - gclsImgRcg.g_stu_square.pnWth[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
            , gclsImgRcg.g_stu_square.pnZPY[i] * rect.Height() / gclsImgRcg.g_stu_square.nBMPH - gclsImgRcg.g_stu_square.pnWth[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
            , gclsImgRcg.g_stu_square.pnZPX[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW + gclsImgRcg.g_stu_square.pnWth[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
            , gclsImgRcg.g_stu_square.pnZPY[i] * rect.Height() / gclsImgRcg.g_stu_square.nBMPH + gclsImgRcg.g_stu_square.pnWth[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
          );
          //���浽��������
          gclsImgRcg.stuRef.Rad = gclsImgRcg.g_stu_square.pnWth[i];
        }
        else
        {
          pt.x = gclsImgRcg.g_stu_square.pnPX[i][3] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW;
          pt.y = gclsImgRcg.g_stu_square.pnPY[i][3] * rect.Height() / gclsImgRcg.g_stu_square.nBMPH;
          pDC->MoveTo(pt);
          //
          POINT pPoint[5];
          for (int j = 0; j < 4; j++)
          {
            pPoint[j].x = gclsImgRcg.g_stu_square.pnPX[i][j] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW;
            pPoint[j].y = gclsImgRcg.g_stu_square.pnPY[i][j] * rect.Height() / gclsImgRcg.g_stu_square.nBMPH;
          }
          pPoint[4].x = gclsImgRcg.g_stu_square.pnPX[i][0] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW;
          pPoint[4].y = gclsImgRcg.g_stu_square.pnPY[i][0] * rect.Height() / gclsImgRcg.g_stu_square.nBMPH;

          pDC->Polygon(pPoint, 4);
          //test
          double angle_test = gclsImgRcg.CalculateVectorAngle(pPoint[1].x - pPoint[0].x,
            pPoint[1].y - pPoint[0].y, 1, 0);
          /*int nLenNo = gclsImgRcg.g_stu_square.lenNo1PN[i];
          int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
          */
          int nLenNo = 0;
          int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
          pDC->Ellipse(pPoint[nLenNo].x - 2, pPoint[nLenNo].y - 2, pPoint[nLenNo].x + 2, pPoint[nLenNo].y + 2);

          pDC->Ellipse(pPoint[nLenNoNext].x - 4, pPoint[nLenNoNext].y - 4, pPoint[nLenNoNext].x + 4, pPoint[nLenNoNext].y + 4);

          //
          gclsImgRcg.stuRef.Len = gclsImgRcg.g_stu_square.pnLen[i];
          gclsImgRcg.stuRef.Wth = gclsImgRcg.g_stu_square.pnWth[i];
          gstuRcgInfo.nPN = gclsImgRcg.g_stu_square.bPN[i];


          CString csTmp;
          csTmp.Format(_T("��%d��%dX�Ƕ�%.1f"), gclsImgRcg.g_stu_square.pnLen[i], gclsImgRcg.g_stu_square.pnWth[i], (angle_test));

          //csTmp.Format(_T("��%d��%dX����%d�Ƕ�%.1f"),gclsImgRcg.g_stu_square.pnLen[i],gclsImgRcg.g_stu_square.pnWth[i],gclsImgRcg.g_stu_square.bPN[i], gclsImgRcg.g_stu_square.angreePN[i]);
          g_dlgCamera->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          //AfxMessageBox(csTmp);
        }
        pWnd->ReleaseDC(pDC);
        gclsImgRcg.stuRef.Dev = gstuRcgInfo.nAllowDefect;
        gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][0] = gclsImgRcg.g_stu_square.pnZPX[i]; gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][1] = gclsImgRcg.g_stu_square.pnZPY[i];//xy�������һ���Ķ�������ͬλ�ã���ͬxy���Ƿ������һ���������ԣ�0,1��ͼ��x��y��2�ǹ̶�1��3�ǵ������
        gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][0] = gclsImgRcg.g_stu_square.pnZPX[i]; gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][1] = gclsImgRcg.g_stu_square.pnZPY[i];
        int X = gclsImgRcg.g_stu_square.pnZPX[i] + 0.5;
        int Y = gclsImgRcg.g_stu_square.pnZPY[i] + 0.5;
        /*
                int X = gstuRcgInfo.g_factor[0][0] * (double)gclsImgRcg.g_stu_square.pnZPX[i] + gstuRcgInfo.g_factor[0][1] * (double)gclsImgRcg.g_stu_square.pnZPY[i] + gstuRcgInfo.g_factor[0][2];
                int Y = gstuRcgInfo.g_factor[1][0] * (double)gclsImgRcg.g_stu_square.pnZPX[i] + gstuRcgInfo.g_factor[1][1] * (double)gclsImgRcg.g_stu_square.pnZPY[i] + gstuRcgInfo.g_factor[1][2];*/
        CString csTmp;
        csTmp.Format(_T("X:%d	Y:%dX"), X, Y);
        g_dlgCamera->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
        gstuRcgInfo.bClbPos = 1;
        break;
      }

    }

      if(i == gclsImgRcg.g_stu_square.nN) {
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
        g_dlgCamera->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);

        int X = xt + 0.5;
        int Y = yt + 0.5;

        csTmp.Format(_T("X:%d	Y:%dX"), X, Y);
        g_dlgCamera->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
      }
    return;
  }

  CDialogEx::OnLButtonDown(nFlags, point);
}
