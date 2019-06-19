// DlgPriview.cpp : 实现文件
//

#include "stdafx.h"
#include "ZPJ.h"
#include "DlgPriview.h"
#include "Camera.h"
#include "afxdialogex.h"


// CDlgPriview 对话框

IMPLEMENT_DYNAMIC(CDlgPriview, CDialogEx)

CDlgPriview::CDlgPriview(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPriview::IDD, pParent)
{
  gstuRcgInfo.Nxy = 0;
}

CDlgPriview::~CDlgPriview()
{
  gstuRcgInfo.Nxy = 0;
}

void CDlgPriview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPriview, CDialogEx)
  ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgPriview 消息处理程序


void CDlgPriview::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  if (0 == gclsImgRcg.g_stu_square.nN)
  {
    //return;
  }
  double xt, yt;
  POINT pt;//定义点
  GetCursorPos(&pt);//取得当前坐标
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
  xt =/*(int)*/(temp_x * gclsImgRcg.g_stu_square.nBMPW / (lRect.Width()));//点击的坐标对应在图像上的x点
  yt =/*(int)*/(temp_y * gclsImgRcg.g_stu_square.nBMPH / (lRect.Height()));//点击的坐标对应在图像上的y点
  //if(!m_RadioCircle && m_RadioSquare)
  {
    //if(avg1.empty()){AfxMessageBox(_T("没有定位坐标，请定位"));return;}

    int temp = 0;
    int i;

    for (i = 0; i < gclsImgRcg.g_stu_square.nN; i++)
    {

      if (sqrt(pow(double(gclsImgRcg.g_stu_square.pnZPX[i] - xt), 2) + pow(double(gclsImgRcg.g_stu_square.pnZPY[i] - yt), 2)) < ((gclsImgRcg.g_stu_square.pnWth[i]) >> 1)) //点击位置离片的粗略中心相距小于s_sort.allowable_devision，这么做主要可以当点不准时设置调节
      {
        //DrawPicToHDC(line_img, IDC_PREVIEW);//在图片控件上显示
        CWnd* pWnd = this;
        CDC* pDC = pWnd->GetDC();
        CRect rect,rect_camera;
        GetClientRect(&rect);
        g_dlgCamera->GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect_camera);
        CPen PenRed;
        PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//创建一支红色的画笔.
        pDC->SelectObject(PenRed);//选中画笔.
        pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), g_dlgCamera->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
        if (gstuRcgInfo.bIsCir)
        {
          CString csTmp;
          csTmp.Format(_T("半径%d"), gclsImgRcg.g_stu_square.pnWth[i]);
          GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          pDC->Ellipse
          (gclsImgRcg.g_stu_square.pnZPX[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW - gclsImgRcg.g_stu_square.pnWth[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
            , gclsImgRcg.g_stu_square.pnZPY[i] * rect.Height() / gclsImgRcg.g_stu_square.nBMPH - gclsImgRcg.g_stu_square.pnWth[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
            , gclsImgRcg.g_stu_square.pnZPX[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW + gclsImgRcg.g_stu_square.pnWth[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
            , gclsImgRcg.g_stu_square.pnZPY[i] * rect.Height() / gclsImgRcg.g_stu_square.nBMPH + gclsImgRcg.g_stu_square.pnWth[i] * rect.Width() / gclsImgRcg.g_stu_square.nBMPW
          );
          //保存到特征变量
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
          csTmp.Format(_T("长%d宽%dX角度%.1f"), gclsImgRcg.g_stu_square.pnLen[i], gclsImgRcg.g_stu_square.pnWth[i], (angle_test));

          //csTmp.Format(_T("长%d宽%dX方向%d角度%.1f"),gclsImgRcg.g_stu_square.pnLen[i],gclsImgRcg.g_stu_square.pnWth[i],gclsImgRcg.g_stu_square.bPN[i], gclsImgRcg.g_stu_square.angreePN[i]);
          g_dlgCamera->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          //AfxMessageBox(csTmp);
        }
        pWnd->ReleaseDC(pDC);
        gclsImgRcg.stuRef.Dev = gstuRcgInfo.nAllowDefect;
        gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][0] = gclsImgRcg.g_stu_square.pnZPX[i]; gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][1] = gclsImgRcg.g_stu_square.pnZPY[i];//xy保存的是一样的东西，相同位置，相同xy，是否可以用一个？不可以，0,1是图像x，y；2是固定1；3是电机步数
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
        PenRed.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//创建一支的画笔.
        pDC->SelectObject(PenRed);//选中画笔.
        //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), g_dlgCamera->mDCMem, 0, 0, SRCCOPY);
        pDC->TransparentBlt/*BitBlt*/(rect.left, rect.top, rect.Width(), rect.Height(), g_dlgCamera->mDCMem, 0, 0, rect_camera.Width(), rect_camera.Height(), SRCCOPY);
        POINT point;//定义点
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
        csTmp.Format(_T("长 宽 角度"));
        g_dlgCamera->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);

        gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][0] = xt; gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][1] = yt;//xy保存的是一样的东西，相同位置，相同xy，是否可以用一个？不可以，0,1是图像x，y；2是固定1；3是电机步数
        gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][0] = xt; gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][1] = yt;

        int X = xt + 0.5;
        int Y = yt + 0.5;

        csTmp.Format(_T("X:%d	Y:%dX"), X, Y);
        g_dlgCamera->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);



        //////
        CString str;

        int x = gstuRcgInfo.nClbPosX[gstuRcgInfo.Nxy];
        int y = gstuRcgInfo.nClbPosY[gstuRcgInfo.Nxy];

        str.Format(_T("是否把图像坐标X:%d	Y:%dX 与 第%d个预设点X:%d	Y:%dX绑定起来"), X, Y, gstuRcgInfo.Nxy+1, x, y);
          
        int status = AfxMessageBox(str, MB_OKCANCEL);
        if (status == 1)//按确定退出
        {
          gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][2] = 1;
          gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][2] = 1;
          gstuRcgInfo.Xxy[gstuRcgInfo.Nxy][3] = x;
          gstuRcgInfo.Yxy[gstuRcgInfo.Nxy][3] = y;

          gstuRcgInfo.Nxy++;

          if (gstuRcgInfo.Nxy == 3)
          {

            calparameter(gstuRcgInfo.Xxy, gstuRcgInfo.g_factor[0]);
            calparameter(gstuRcgInfo.Yxy, gstuRcgInfo.g_factor[1]);
            //保存
            CString strValue;
            strValue.Format(_T("%.18lf"), gstuRcgInfo.g_factor[0][0]);
            WritePrivateProfileString(_T("校准系数"), _T("00"), strValue, gstuPathInf.csPathIni);
            strValue.Format(_T("%.18lf"), gstuRcgInfo.g_factor[0][1]);
            WritePrivateProfileString(_T("校准系数"), _T("01"), strValue, gstuPathInf.csPathIni);
            strValue.Format(_T("%.18lf"), gstuRcgInfo.g_factor[0][2]);
            WritePrivateProfileString(_T("校准系数"), _T("02"), strValue, gstuPathInf.csPathIni);
            strValue.Format(_T("%.18lf"), gstuRcgInfo.g_factor[1][0]);
            WritePrivateProfileString(_T("校准系数"), _T("10"), strValue, gstuPathInf.csPathIni);
            strValue.Format(_T("%.18lf"), gstuRcgInfo.g_factor[1][1]);
            WritePrivateProfileString(_T("校准系数"), _T("11"), strValue, gstuPathInf.csPathIni);
            strValue.Format(_T("%.18lf"), gstuRcgInfo.g_factor[1][2]);
            WritePrivateProfileString(_T("校准系数"), _T("12"), strValue, gstuPathInf.csPathIni);
            g_dlgCamera->SetDlgItemText(IDC_SELECT_RESULT, _T("校准完成"));
            AfxMessageBox(_T("参数更新完成"), MB_OK);
            gstuRcgInfo.Nxy = 0;
          }
        }
        else
        {
          //gstuRcgInfo.Nxy=0;
        }
        /////////
      }
    return;
  }

  CDialogEx::OnLButtonDown(nFlags, point);
}
