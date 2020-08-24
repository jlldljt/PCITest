// Camera.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgCamera.h"
#include "afxdialogex.h"
#include "MainFrm.h"

#define TIMER_VIDEO  1001
//UINT Thread_Auto(LPVOID pParam)
//{
//  SetThreadAffinityMask(GetCurrentThread(), 1);
//  CCamera* pdlg = (CCamera*)pParam;
//  char l_AnsiStr[MAX_PATH];
//  int nWidth, nHeight;
//  int nBitCount;
//  int nBitsPerSample;
//  int takeCnt = 0;
//
//  pdlg->GetDlgItem(IDC_BTN_AUTO)->EnableWindow(TRUE);
//
//  while (gstuRun.unStatRun == RUN_STAT_RUN)
//  {
//    //=====================================================识别开始===============================================================
//    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, gstuPathInf.csPathExe + _T("\\PIC\\原图0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);
//    TimeRecord(2, 1);//启动拍照，计时开始
//
//    if (nullptr == gclsCamera.SnapEx(gnChannel, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, NULL))
//    {
//      Sleep(500);
//
//      if (takeCnt++ > 10)//取不到图像，退出
//      {
//        AfxMessageBox(_T("取不到相机图像，请检查相机"));
//        takeCnt = 0;
//        gstuRun.unStatRun == RUN_STAT_STOP;
//        pdlg->SetDlgItemText(IDC_BTN_AUTO, _T("自动"));
//        break;
//      }
//      continue;
//    }
//
//    TimeRecord(2, 0);//拍照结束，计时结束
//    TimeRecord(3, 1);//识别开始，计时开始
//    //
//    //m_rcg.RCGBMPSPLIT(NULL, NULL, l_AnsiStr, m_param->camera.nPToL, m_param->camera.nDefectPToL, m_param->camera.nThreshold, 0, m_param->camera.bIsCir, m_param->camera.bThrdAuto, m_param->camera.bDelNoise);
//
//
//    m_rcg.RCGBMP(NULL, NULL, l_AnsiStr, m_param->camera.nPToL, m_param->camera.nDefectPToL, m_param->camera.nThreshold, 0, m_param->camera.bIsCir, m_param->camera.bThrdAuto, m_param->camera.bDelNoise);
//    //
//
//    //
//    TimeRecord(3, 0);//识别结束，计时结束
//    //=====================================================传输开始===============================================================
//
//    for (int i = 0; i < m_rcg.g_stu_square.nN; i++)
//    {
//      /*if (PN_DEF != m_rcg.g_stu_square.bPN[i])
//        continue;*/
//      if (
//        m_rcg.g_stu_square.pnZPX[i] < m_npc_inf.left ||
//        m_rcg.g_stu_square.pnZPX[i] > m_npc_inf.right ||
//        m_rcg.g_stu_square.pnZPY[i] < m_npc_inf.top ||
//        m_rcg.g_stu_square.pnZPY[i] > m_npc_inf.bottom
//        )
//        continue;
//      //长判断
//      if (m_rcg.g_stu_square.pnLen[i] < (m_rcg.stuRef.Len - m_rcg.stuRef.Dev) || m_rcg.g_stu_square.pnLen[i] > (m_rcg.stuRef.Len + m_rcg.stuRef.Dev))
//      {
//        continue;
//      }
//      //宽判断
//      if (m_rcg.g_stu_square.pnWth[i] < (m_rcg.stuRef.Wth - m_rcg.stuRef.Dev) || m_rcg.g_stu_square.pnWth[i] > (m_rcg.stuRef.Wth + m_rcg.stuRef.Dev))
//      {
//        continue;
//      }
//
//      NpcParm par;
//
//
//      //
//      POINT pPoint[5];
//      for (int j = 0; j < 4; j++)
//      {
//        pPoint[j].x = m_rcg.g_stu_square.pnPX[i][j];
//        pPoint[j].y = m_rcg.g_stu_square.pnPY[i][j];
//      }
//      pPoint[4].x = m_rcg.g_stu_square.pnPX[i][0];
//      pPoint[4].y = m_rcg.g_stu_square.pnPY[i][0];
//
//      //test
//      //double angle_test = m_rcg.CalculateVectorAngle(pPoint[1].x - pPoint[0].x,
//      //  pPoint[1].y - pPoint[0].y, 1, 0);
//      /*int nLenNo = m_rcg.g_stu_square.lenNo1PN[i];
//      int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);
//      */
//
//      par.deg0 = m_rcg.g_stu_square.angreePN[i];// angle_test;
//      par.x0 = m_rcg.g_stu_square.pnZPX[i];
//      par.y0 = m_rcg.g_stu_square.pnZPY[i];
//      par.pn0 = m_rcg.g_stu_square.bPN[i];//m_param->camera.nPN;m_rcg.g_stu_square.bPN[i]
//
//      CString csTmp;
//      csTmp.Format(_T("长%d 宽%d 方向 %d 角度%.1f X:%d Y:%d"), m_rcg.g_stu_square.pnLen[i], m_rcg.g_stu_square.pnWth[i], (int)(par.pn0), par.deg0, (int)(par.x0), (int)(par.y0));
//      pdlg->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
//      TranNpcParam(&par);
//
//      csTmp.Format(_T("No%d 方向 %d 角度%d X:%d Y:%d"), i, par.pn, par.deg, par.x, par.y);
//      pdlg->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
//
//      //发送参数
//      while (0 != g_dlgDevice->ParamMove(par.x, par.y, par.deg, par.pn))
//      {
//        Sleep(1000);
//      }
//
//    }//for (int i = 0; i < m_rcg.g_stu_square.nN; i++)
//
//  }//while (gstuRun.unStatRun == RUN_STAT_RUN)
//  pdlg->GetDlgItem(IDC_BTN_AUTO)->EnableWindow(TRUE);
//
//  return 0;
//}
//坐标系转换系数计算
void calparameter(double(*xy)[4], double* factor)
//void calparameter(double **xy,double *factor)
{
  int k, i, j, f;//循环参数
  for (k = 0; k < 3; k++)//列主元高斯消元               
  {
    //double ark=findmax(k,xy);
    double max = 0;
    for (int i = k; i < 3; i++)
      if (fabs(xy[i][k]) > max)        //对数组中的元素进行取绝对值比较大小的运算
      {
        max = fabs(xy[i][k]);
        f = i;
      }

    if (max == 0)
    {
      AfxMessageBox(_T("此方程组无解!"));
      return;
    }
    else if (f != k)
    {
      //change(f,k,xy);
      double temp[4];
      for (int i = 0; i < 4; i++)
        temp[i] = xy[f][i];
      for (int i = 0; i < 4; i++)
        xy[f][i] = xy[k][i];
      for (int i = 0; i < 4; i++)
        xy[k][i] = temp[i];
    }
    for (int i = k + 1; i < 3; i++)
      for (int j = 3; j >= 0; j--)
        xy[i][j] = xy[i][j] - xy[k][j] * xy[i][k] / xy[k][k];
  }
  factor[2] = xy[2][3] / xy[2][2];      //先求出最后一个未知数的解
  for (k = 1; k >= 0; k--)//回调
  {
    double p1 = 0;
    for (j = k + 1; j < 3; j++)
    {
      p1 = p1 + xy[k][j] * factor[j];
    }
    factor[k] = (xy[k][3] - p1) / xy[k][k];
  }
}

/*****vlc ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++test********/


/*******end***++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++***/
// CCamera 对话框

IMPLEMENT_DYNAMIC(CCamera, CDialogEx)

CCamera::CCamera(CWnd* pParent /*=NULL*/)
  : CDialogEx(CCamera::IDD, pParent)
{
  m_param = &GetMainFrame()->m_efgio.m_configParam;
  m_io = &GetMainFrame()->m_efgio;
  m_ch = -1;
  gb_PlayOrNot[0] = 0;
  gb_PlayOrNot[1] = 0;
}


CCamera::~CCamera()
{
  mDCMem->SelectObject(mBMPOld);
  mBMPMem->DeleteObject();
  mDCMem->DeleteDC();
}

void CCamera::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CMB_AWB, m_cmbAWB);
  DDX_Control(pDX, IDC_CMB_BACKLIGHT, m_cmbBl);
}


BEGIN_MESSAGE_MAP(CCamera, CDialogEx)
  ON_BN_CLICKED(IDC_BTN_SNAP, &CCamera::OnBnClickedBtnSnap)
  ON_BN_CLICKED(IDC_BTN_TEST, &CCamera::OnBnClickedBtnTest)
  ON_BN_CLICKED(IDC_BTN_CAMSET, &CCamera::OnBnClickedBtnCamset)
  ON_STN_CLICKED(IDC_PREVIEW, &CCamera::OnStnClickedPreview)
  ON_BN_CLICKED(IDC_BTN_CALIBRATION, &CCamera::OnBnClickedBtnCalibration)
  ON_BN_CLICKED(IDC_BTN_VIDEO, &CCamera::OnBnClickedBtnVideo)
  ON_WM_PAINT()
  ON_EN_CHANGE(IDC_EDT_OUTPALLOWL, &CCamera::OnEnChangeEdtOutpallowl)
  ON_EN_CHANGE(IDC_EDT_DEFECTALLOWL, &CCamera::OnEnChangeEdtDefectallowl)
  ON_EN_CHANGE(IDC_EDT_THRESHOLD2, &CCamera::OnEnChangeEdtThreshold)
  ON_EN_CHANGE(IDC_EDT_FTOUTPOINT, &CCamera::OnEnChangeEdtFtoutpoint)
  ON_BN_CLICKED(IDC_BTN_GETSET, &CCamera::OnBnClickedBtnGetset)
  ON_BN_CLICKED(IDC_CHK_AUTOTHRD, &CCamera::OnBnClickedChkAutothrd)
  ON_BN_CLICKED(IDC_CHK_DELNOISE, &CCamera::OnBnClickedChkDelnoise)
  //	ON_BN_CLICKED(IDC_CHECK3, &CCamera::OnBnClickedCheck3)
  ON_BN_CLICKED(IDC_CHK_DEBUG, &CCamera::OnBnClickedChkDebug)
  ON_BN_CLICKED(IDC_BTN_SPLIT, &CCamera::OnBnClickedBtnSplit)
  ON_BN_CLICKED(IDC_BTN_RECONNECT, &CCamera::OnBnClickedBtnReconnect)
  ON_BN_CLICKED(IDC_BTN_PARMSEND, &CCamera::OnBnClickedBtnParmsend)
  ON_BN_CLICKED(IDC_BTN_AUTO, &CCamera::OnBnClickedBtnAuto)
  ON_EN_CHANGE(IDC_EDT_SQU_MINDEG, &CCamera::OnEnChangeEdtSquMindeg)
  ON_EN_CHANGE(IDC_EDT_SQU_MAXDEG, &CCamera::OnEnChangeEdtSquMaxdeg)
  ON_EN_CHANGE(IDC_EDT_PN_MINDEG, &CCamera::OnEnChangeEdtPnMindeg)
  ON_EN_CHANGE(IDC_EDT_PN_MAXDEG, &CCamera::OnEnChangeEdtPnMaxdeg)
  ON_EN_CHANGE(IDC_EDT_THRESHOLD2, &CCamera::OnEnChangeEdtThreshold2)
  ON_EN_CHANGE(IDC_EDT_CLBPOS1X, &CCamera::OnEnChangeEdtClbpos1x)
  ON_EN_CHANGE(IDC_EDT_CLBPOS1Y, &CCamera::OnEnChangeEdtClbpos1y)
  ON_BN_CLICKED(IDC_BTN_CLBPOS2TEST, &CCamera::OnBnClickedBtnClbpos2test)
  ON_EN_CHANGE(IDC_EDT_CLBPOS2X, &CCamera::OnEnChangeEdtClbpos2x)
  ON_EN_CHANGE(IDC_EDT_CLBPOS3X, &CCamera::OnEnChangeEdtClbpos3x)
  ON_EN_CHANGE(IDC_EDT_CLBPOS2Y, &CCamera::OnEnChangeEdtClbpos2y)
  ON_EN_CHANGE(IDC_EDT_CLBPOS3Y, &CCamera::OnEnChangeEdtClbpos3y)
  ON_EN_CHANGE(IDC_EDT_TRANSFERX, &CCamera::OnEnChangeEdtTransferx)
  ON_EN_CHANGE(IDC_EDT_TRANSFERY, &CCamera::OnEnChangeEdtTransfery)
  ON_BN_CLICKED(IDC_BTN_CLBPOS1TEST, &CCamera::OnBnClickedBtnClbpos1test)
  ON_BN_CLICKED(IDC_BTN_TRANSFERTEST, &CCamera::OnBnClickedBtnTransfertest)
  ON_BN_CLICKED(IDC_BTN_CLBPOS3TEST, &CCamera::OnBnClickedBtnClbpos3test)
  ON_WM_TIMER()
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_BTN_TESTZERO, &CCamera::OnBnClickedBtnTestzero)
END_MESSAGE_MAP()


// CCamera 消息处理程序


void CCamera::OnBnClickedBtnSnap()
{

  ///////////////////////////////////ksj///////////////////////////
  LARGE_INTEGER l_lgint_start, l_lgint_end;
  LARGE_INTEGER l_lgint_freq;
  QueryPerformanceFrequency(&l_lgint_freq);
  QueryPerformanceCounter(&l_lgint_start);
  char l_AnsiStr[MAX_PATH];
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, GetMainFrame()->m_exe_path + _T("\\PIC\\原图0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);

  int nWidth, nHeight;
  int nBitCount;
  int nBitsPerSample;
  //unsigned char* pData = m_ksj.SnapEx(m_ch, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, GetDlgItem(IDC_PREVIEW));
  m_cam.snap();
  m_cam.captureBmp(l_AnsiStr);
  QueryPerformanceCounter(&l_lgint_end);
  double dTmpRT = double(l_lgint_end.QuadPart - l_lgint_start.QuadPart) / double(l_lgint_freq.QuadPart);
  CString csTmp;
  csTmp.Format(_T("%.2f秒"), dTmpRT);
  SetDlgItemText(IDC_SELECT_RESULT, csTmp);

  //	bOverRcg=1;
}


void CCamera::OnBnClickedBtnTest()
{
  // TODO: 在此添加控件通知处理程序代码

  CString m_bmp_file = GetMainFrame()->m_exe_path + _T("\\PIC\\原图0.bmp");

  CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
  BOOL ifFind = findini.FindFile(m_bmp_file);

  if (!ifFind)
    return;

  CBitmap m_bmp;//创建类成员
  BITMAP bm;//存放位图信息的结构
  HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL, m_bmp_file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//创建bitmap指针
  m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
  m_bmp.GetBitmap(&bm);
  CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
  CDC* pDC = pWnd->GetDC();
  CRect rect;
  GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
  CDC memDC;        //定义一个设备
  CClientDC dc1(this);      //获取客户
  memDC.CreateCompatibleDC(&dc1);
  memDC.SelectObject(m_bmp);  //为设备选择对象

  mDCMem->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

  pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
  memDC.DeleteDC();
  m_bmp.DeleteObject();
  DeleteObject(hBitmap1);//记得删除	

  CPen PenRed;
  CGdiObject* o = NULL;
  PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//创建一支红色的画笔.

  //pWnd = GetDlgItem(IDC_image1); //IDC_STATIC是picture control的ID.这句是得到picture的句柄.
  //pDC = pWnd->GetDC();//然后得到设备环境.
  //RECT rect;//声明一个rect
  //pWnd->GetClientRect(&rect);//并把picture控件的坐标用rect接收.
  mDCMem->SelectObject(PenRed);//选中画笔.

  /*m_param->camera.nPToL=2;//直线点距
  m_param->camera.nDefectPToL=10;//缺陷点距
  m_param->camera.nThreshold=30;//阀值*/
  //char * chPath = (LPSTR)(LPCTSTR)m_bmp_file;
  char chPath[MAX_PATH];
  //WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,readPath,CStringA(m_bmp_file).GetLength(),NULL,NULL); 
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_bmp_file, -1, chPath, sizeof(chPath), NULL, NULL);
  //
  LARGE_INTEGER l_lgint_start, l_lgint_end;
  LARGE_INTEGER l_lgint_freq;
  QueryPerformanceFrequency(&l_lgint_freq);
  QueryPerformanceCounter(&l_lgint_start);
  //
  m_rcg.RCGBMP(mDCMem, rect, chPath, m_param->camera.nPToL, m_param->camera.nDefectPToL, m_param->camera.nThreshold, m_param->camera.bDebug, m_param->camera.bIsCir, m_param->camera.bThrdAuto, m_param->camera.bDelNoise);
  //
  QueryPerformanceCounter(&l_lgint_end);
  double dTmpRT = double(l_lgint_end.QuadPart - l_lgint_start.QuadPart) / double(l_lgint_freq.QuadPart);
  CString csTmp;
  csTmp.Format(_T("%.2f秒"), dTmpRT);
  SetDlgItemText(IDC_SELECT_RESULT, csTmp);
  //
  pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);

  pDC->SelectObject(PenRed);//选中画笔.
  CBrush br;
  br.CreateStockObject(NULL_BRUSH);
  pDC->SelectObject(&br);
  pDC->Rectangle(m_npc_inf.left * rect.Width() / m_rcg.g_stu_square.nBMPW
    , m_npc_inf.top * rect.Height() / m_rcg.g_stu_square.nBMPH
    , m_npc_inf.right * rect.Width() / m_rcg.g_stu_square.nBMPW
    , m_npc_inf.bottom * rect.Height() / m_rcg.g_stu_square.nBMPH);


  pWnd->ReleaseDC(pDC);
  m_param->camera.bClbPos = 1;
  /*	g_tmp_path=_T(".\\PIC\\temp1.bmp");
    CString l_cstr_tmp;
    l_cstr_tmp.Format(_T("%d、%d"),g_class_img_recognition.g_stu_square.crests,g_class_img_recognition.g_stu_square.sags);
    GetDlgItem(IDC_EDIT_BROKEN_RAW)->SetWindowText(l_cstr_tmp);
    l_cstr_tmp.Format(_T("%.2lf、%.2lf、%.2lf、%.2lf"),g_class_img_recognition.g_stu_square.d_len[0],g_class_img_recognition.g_stu_square.d_len[1],g_class_img_recognition.g_stu_square.d_len[2],g_class_img_recognition.g_stu_square.d_len[3]);
    GetDlgItem(IDC_EDIT_SIZE_RAW)->SetWindowText(l_cstr_tmp);*/
}


void CCamera::OnBnClickedBtnCamset()
{
  // TODO: 在此添加控件通知处理程序代码


  m_ksj.SetParam(m_ch, KSJ_EXPOSURE, GetDlgItemInt(IDC_EDT_EV));//
  m_ksj.SetParam(m_ch, KSJ_RED, GetDlgItemInt(IDC_EDT_GAIN));//
  m_ksj.SetParam(m_ch, KSJ_BRIGHTNESS, GetDlgItemInt(IDC_EDT_BRIGHT));//
  m_ksj.SetParam(m_ch, KSJ_CONTRAST, GetDlgItemInt(IDC_EDT_CONTRAST));//
}


BOOL CCamera::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  //CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
  //CDC* pDC = pWnd->GetDC();
  //mDCMem= new CDC;
  //CClientDC dc1(GetDlgItem(IDC_PREVIEW)->GetDC());      //获取客户
  CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
  CDC* pDC = pWnd->GetDC();
  CRect rect;
  GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
  mDCMem = new CDC;
  mBMPMem = new CBitmap;
  mDCMem->CreateCompatibleDC(pDC);
  mBMPMem->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  mBMPOld = mDCMem->SelectObject(mBMPMem);//无这句和上句，对mdcmen操作无效
  mDCMem->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(mDCMem->m_hDC, 0, 0, NULL);
  pWnd->ReleaseDC(pDC);
  //mDCMem->CreateCompatibleDC(GetDlgItem(IDC_PREVIEW)->GetDC());

  //=====================CAMERA===========================
  //combobox必须放在tab控件之后
  m_cmbAWB.AddString(_T("Auto"));
  m_cmbAWB.AddString(_T("Day_D55"));
  m_cmbAWB.AddString(_T("Day_D65"));
  m_cmbAWB.AddString(_T("Florescent"));
  m_cmbAWB.AddString(_T("Incandscent"));
  m_cmbAWB.SetCurSel(0);
  m_cmbBl.AddString(_T("Min"));
  m_cmbBl.AddString(_T("Mid"));
  m_cmbBl.AddString(_T("Max"));
  m_cmbBl.SetCurSel(0);
  //ini

  int iniint = 0;

  CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
  BOOL ifFind = findini.FindFile(g_ini_path);
  if (ifFind)
  {//创建ini
    //读入ini文件中相应字段
    iniint = GetPrivateProfileInt(_T("相机参数"), _T("最大曝光值"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_EV, iniint);
    iniint = GetPrivateProfileInt(_T("相机参数"), _T("最大增益"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_GAIN, iniint);
    iniint = GetPrivateProfileInt(_T("相机参数"), _T("亮度"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_BRIGHT, iniint);
    iniint = GetPrivateProfileInt(_T("相机参数"), _T("对比度"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CONTRAST, iniint);
    iniint = GetPrivateProfileInt(_T("相机参数"), _T("饱和度"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_SATURATION, iniint);
    iniint = GetPrivateProfileInt(_T("相机参数"), _T("锐度"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_SHARPNESS, iniint);
    iniint = GetPrivateProfileInt(_T("相机参数"), _T("白平衡控制"), 0, g_ini_path);
    m_cmbAWB.SetCurSel(iniint);
    iniint = GetPrivateProfileInt(_T("相机参数"), _T("背光值"), 0, g_ini_path);
    m_cmbBl.SetCurSel(iniint);
    //相机识别
    iniint = GetPrivateProfileInt(_T("识别参数"), _T("直线点距"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_OUTPALLOWL, iniint); m_param->camera.nPToL = iniint;
    iniint = GetPrivateProfileInt(_T("识别参数"), _T("缺陷点距"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_DEFECTALLOWL, iniint); m_param->camera.nDefectPToL = iniint;
    iniint = GetPrivateProfileInt(_T("识别参数"), _T("阈值"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_THRESHOLD2, iniint); m_param->camera.nThreshold = iniint;
    iniint = GetPrivateProfileInt(_T("识别参数"), _T("特征允许误差"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_FTOUTPOINT, iniint); m_param->camera.nAllowDefect = iniint;

    //识别

    iniint = GetPrivateProfileInt(_T("方片参数"), _T("最小角"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_SQU_MINDEG, iniint); m_rcg.stuRef.SAg = iniint;
    iniint = GetPrivateProfileInt(_T("方片参数"), _T("最大角"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_SQU_MAXDEG, iniint); m_rcg.stuRef.BAg = iniint;
    iniint = GetPrivateProfileInt(_T("方向参数"), _T("最小角"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_PN_MINDEG, iniint); m_rcg.stuRef.PN_sag = iniint;
    iniint = GetPrivateProfileInt(_T("方向参数"), _T("最大角"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_PN_MAXDEG, iniint); m_rcg.stuRef.PN_bag = iniint;

    //
    iniint = GetPrivateProfileInt(_T("取片校准点"), _T("1X"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS1X, iniint);
    m_param->camera.nClbPosX[0] = iniint;
    iniint = GetPrivateProfileInt(_T("取片校准点"), _T("1Y"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS1Y, iniint);
    m_param->camera.nClbPosY[0] = iniint;
    iniint = GetPrivateProfileInt(_T("取片校准点"), _T("2X"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS2X, iniint);
    m_param->camera.nClbPosX[1] = iniint;
    iniint = GetPrivateProfileInt(_T("取片校准点"), _T("2Y"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS2Y, iniint);
    m_param->camera.nClbPosY[1] = iniint;
    iniint = GetPrivateProfileInt(_T("取片校准点"), _T("3X"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS3X, iniint);
    m_param->camera.nClbPosX[2] = iniint;
    iniint = GetPrivateProfileInt(_T("取片校准点"), _T("3Y"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS3Y, iniint);
    m_param->camera.nClbPosY[2] = iniint;
    // 中转点
    iniint = GetPrivateProfileInt(_T("中转点"), _T("X"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_TRANSFERX, iniint);
    m_param->camera.transferx = iniint;
    iniint = GetPrivateProfileInt(_T("中转点"), _T("Y"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_TRANSFERY, iniint);
    m_param->camera.transfery = iniint;
    //读取ini
    TCHAR strbuff[256];
    CString strValue;
    GetPrivateProfileString(_T("校准系数"), _T("00"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[0][0] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("校准系数"), _T("01"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[0][1] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("校准系数"), _T("02"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[0][2] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("校准系数"), _T("10"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[1][0] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("校准系数"), _T("11"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[1][1] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("校准系数"), _T("12"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[1][2] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();


    GetPrivateProfileString(_T("范围设置"), _T("left"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_npc_inf.left = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();

    GetPrivateProfileString(_T("范围设置"), _T("top"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_npc_inf.top = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();

    GetPrivateProfileString(_T("范围设置"), _T("right"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_npc_inf.right = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();

    GetPrivateProfileString(_T("范围设置"), _T("bottom"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_npc_inf.bottom = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
  }
  else
    AfxMessageBox(_T("ini没有创建"));

  m_param->camera.bThrdAuto = 0;//自动阈值标识0
  m_param->camera.bDelNoise = 0;
  m_param->camera.bDebug = 0;

  //m_ch = m_ksj.GetKsj(KSJ_UD205M_SGYY);
  //if (0 > m_ch)
  //  AfxMessageBox(_T("KSJ_UD205M_SGYY 相机打开失败	,请检查是否连接该相机"));
  //if(-1==m_cam.init(GetDlgItem(IDC_PREVIEW)->m_hWnd))
//	  AfxMessageBox(_T("usb相机启动失败"));
  
  return TRUE;  // return TRUE unless you set the focus to a control
  // 异常: OCX 属性页应返回 FALSE
}


void CCamera::TranNpcParam(NpcParm* parm)
{

  parm->x = m_param->camera.g_factor[0][0] * parm->x0 + m_param->camera.g_factor[0][1] * parm->y0 + m_param->camera.g_factor[0][2] + 0.5;
  parm->y = m_param->camera.g_factor[1][0] * parm->x0 + m_param->camera.g_factor[1][1] * parm->y0 + m_param->camera.g_factor[1][2] + 0.5;
  parm->deg = parm->deg0 * Z_P_PER_DEG + Z_S + 0.5;
  parm->pn = parm->pn0 + 0.5;

}

void CCamera::OnStnClickedPreview()
{
  // TODO: 在此添加控件通知处理程序代码
  //NpcParm par = { 0 };

  if (0 == m_rcg.g_stu_square.nN)
  {
    //return;
  }
  double xt, yt;
  POINT pt;//定义点
  GetCursorPos(&pt);//取得当前坐标
  CRect lRect;
  GetDlgItem(IDC_PREVIEW)->GetWindowRect(&lRect);
  double temp_x = pt.x - lRect.left;
  double temp_y = pt.y - lRect.top;
  xt =/*(int)*/(temp_x * m_rcg.g_stu_square.nBMPW / (lRect.Width()));//点击的坐标对应在图像上的x点
  yt =/*(int)*/(temp_y * m_rcg.g_stu_square.nBMPH / (lRect.Height()));//点击的坐标对应在图像上的y点
  //if(!m_RadioCircle && m_RadioSquare)
  {
    //if(avg1.empty()){AfxMessageBox(_T("没有定位坐标，请定位"));return;}

    int temp = 0;
    int i;

    for (i = 0; i < m_rcg.g_stu_square.nN; i++)
    {

      if (sqrt(pow(double(m_rcg.g_stu_square.pnZPX[i] - xt), 2) + pow(double(m_rcg.g_stu_square.pnZPY[i] - yt), 2)) < ((m_rcg.g_stu_square.pnWth[i]) >> 1)) //点击位置离片的粗略中心相距小于s_sort.allowable_devision，这么做主要可以当点不准时设置调节
      {
        //DrawPicToHDC(line_img, IDC_PREVIEW);//在图片控件上显示
        CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
        CDC* pDC = pWnd->GetDC();
        CRect rect;
        GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
        CPen PenRed, PenGreen;
        PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//创建一支红色的画笔.
        PenGreen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//创建一支红色的画笔.
        pDC->SelectObject(PenRed);//选中画笔.
        pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
        if (m_param->camera.bIsCir)
        {
          CString csTmp;
          csTmp.Format(_T("半径%d"), m_rcg.g_stu_square.pnWth[i]);
          GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          pDC->Ellipse
          (m_rcg.g_stu_square.pnZPX[i] * rect.Width() / m_rcg.g_stu_square.nBMPW - m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_rcg.g_stu_square.nBMPW
            , m_rcg.g_stu_square.pnZPY[i] * rect.Height() / m_rcg.g_stu_square.nBMPH - m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_rcg.g_stu_square.nBMPW
            , m_rcg.g_stu_square.pnZPX[i] * rect.Width() / m_rcg.g_stu_square.nBMPW + m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_rcg.g_stu_square.nBMPW
            , m_rcg.g_stu_square.pnZPY[i] * rect.Height() / m_rcg.g_stu_square.nBMPH + m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_rcg.g_stu_square.nBMPW
          );
          //保存到特征变量
          m_rcg.stuRef.Rad = m_rcg.g_stu_square.pnWth[i];
        }
        else
        {
          pt.x = m_rcg.g_stu_square.pnPX[i][3] * rect.Width() / m_rcg.g_stu_square.nBMPW;
          pt.y = m_rcg.g_stu_square.pnPY[i][3] * rect.Height() / m_rcg.g_stu_square.nBMPH;
          pDC->MoveTo(pt);
          //
          POINT pPoint[5];
          for (int j = 0; j < 4; j++)
          {
            pPoint[j].x = m_rcg.g_stu_square.pnPX[i][j] * rect.Width() / m_rcg.g_stu_square.nBMPW;
            pPoint[j].y = m_rcg.g_stu_square.pnPY[i][j] * rect.Height() / m_rcg.g_stu_square.nBMPH;
          }
          pPoint[4].x = m_rcg.g_stu_square.pnPX[i][0] * rect.Width() / m_rcg.g_stu_square.nBMPW;
          pPoint[4].y = m_rcg.g_stu_square.pnPY[i][0] * rect.Height() / m_rcg.g_stu_square.nBMPH;

          pDC->Polygon(pPoint, 4);
          //test
          //double angle_test = m_rcg.CalculateVectorAngle(pPoint[1].x-pPoint[0].x,
          //  pPoint[1].y-pPoint[0].y, 1, 0);
		  if(0!=m_rcg.g_stu_square.bPN[i])
          
			  {int nLenNo = m_rcg.g_stu_square.lenNo1PN[i];
          int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);

          /*int nLenNo = 0;
          int nLenNoNext = (3 == nLenNo) ? 0 : (nLenNo + 1);*/

          pDC->Ellipse(pPoint[nLenNo].x - 2, pPoint[nLenNo].y - 2, pPoint[nLenNo].x + 2, pPoint[nLenNo].y + 2);
          pDC->Ellipse(pPoint[nLenNoNext].x - 4, pPoint[nLenNoNext].y - 4, pPoint[nLenNoNext].x + 4, pPoint[nLenNoNext].y + 4);
		  }
          ////保存到特征变量
          m_rcg.stuRef.Len = m_rcg.g_stu_square.pnLen[i];
          m_rcg.stuRef.Wth = m_rcg.g_stu_square.pnWth[i];
          m_param->camera.nPN = m_rcg.g_stu_square.bPN[i];

          m_par.deg0 = m_rcg.g_stu_square.angreePN[i];// angle_test;
          int X = m_rcg.g_stu_square.pnZPX[i];
          int Y = m_rcg.g_stu_square.pnZPY[i];
          m_par.x0 = X;
          m_par.y0 = Y;
          m_par.pn0 = m_rcg.g_stu_square.bPN[i];//m_param->camera.nPN;
          CString csTmp;
          //csTmp.Format(_T("长%d 宽%d 方向 %d 角度%.1f X:%d Y:%d"), m_rcg.g_stu_square.pnLen[i],m_rcg.g_stu_square.pnWth[i], (int)(m_par.pn0), (angle_test), X, Y);

          csTmp.Format(_T("长%d 宽%d 方向 %d 角度%.1f X:%d Y:%d"), m_rcg.g_stu_square.pnLen[i], m_rcg.g_stu_square.pnWth[i], (int)(m_par.pn0), (m_par.deg0), (int)(m_par.x0), (int)(m_par.y0));
          GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          //AfxMessageBox(csTmp);
        }
        if (
          m_rcg.g_stu_square.pnZPX[i] < m_npc_inf.left ||
          m_rcg.g_stu_square.pnZPX[i] > m_npc_inf.right ||
          m_rcg.g_stu_square.pnZPY[i] < m_npc_inf.top ||
          m_rcg.g_stu_square.pnZPY[i] > m_npc_inf.bottom
          )
          pDC->SelectObject(PenRed);
        else
          pDC->SelectObject(PenGreen);

        CBrush br;
        br.CreateStockObject(NULL_BRUSH);
        pDC->SelectObject(&br);
        pDC->Rectangle(m_npc_inf.left * rect.Width() / m_rcg.g_stu_square.nBMPW
          , m_npc_inf.top * rect.Height() / m_rcg.g_stu_square.nBMPH
          , m_npc_inf.right * rect.Width() / m_rcg.g_stu_square.nBMPW
          , m_npc_inf.bottom * rect.Height() / m_rcg.g_stu_square.nBMPH);

        pWnd->ReleaseDC(pDC);
        m_rcg.stuRef.Dev = m_param->camera.nAllowDefect;
        m_param->camera.Xxy[m_param->camera.Nxy][0] = m_rcg.g_stu_square.pnZPX[i]; m_param->camera.Xxy[m_param->camera.Nxy][1] = m_rcg.g_stu_square.pnZPY[i];//xy保存的是一样的东西，相同位置，相同xy，是否可以用一个？不可以，0,1是图像x，y；2是固定1；3是电机步数
        m_param->camera.Yxy[m_param->camera.Nxy][0] = m_rcg.g_stu_square.pnZPX[i]; m_param->camera.Yxy[m_param->camera.Nxy][1] = m_rcg.g_stu_square.pnZPY[i];


        //int X = m_param->camera.g_factor[0][0] * (double)m_rcg.g_stu_square.pnZPX[i] + m_param->camera.g_factor[0][1] * (double)m_rcg.g_stu_square.pnZPY[i] + m_param->camera.g_factor[0][2]+0.5;
        //int Y = m_param->camera.g_factor[1][0] * (double)m_rcg.g_stu_square.pnZPX[i] + m_param->camera.g_factor[1][1] * (double)m_rcg.g_stu_square.pnZPY[i] + m_param->camera.g_factor[1][2]+0.5;
        TranNpcParam(&m_par);

        //m_clked_pos_x = par.x;
        //m_clked_pos_y = par.y;

        CString csTmp;
        csTmp.Format(_T("No%d 方向 %d 角度%d X:%d Y:%d"), i, m_par.pn, m_par.deg, m_par.x, m_par.y);
        GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
        m_param->camera.bClbPos = 1;
        break;
      }

    }
    //没有点到片上
    if (i == m_rcg.g_stu_square.nN) {
      CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
      CDC* pDC = pWnd->GetDC();
      CRect rect;
      GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
      CPen PenRed, PenGreen;
      PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//创建一支红色的画笔.
      PenGreen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//创建一支红色的画笔.
      pDC->SelectObject(PenRed);//选中画笔.
      pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
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

      if (
        xt + 0.5 < m_npc_inf.left ||
        xt + 0.5 > m_npc_inf.right ||
        yt + 0.5 < m_npc_inf.top ||
        yt + 0.5 > m_npc_inf.bottom
        )
        pDC->SelectObject(PenRed);
      else
        pDC->SelectObject(PenGreen);

      CBrush br;
      br.CreateStockObject(NULL_BRUSH);
      pDC->SelectObject(&br);
      pDC->Rectangle(m_npc_inf.left * rect.Width() / m_rcg.g_stu_square.nBMPW
        , m_npc_inf.top * rect.Height() / m_rcg.g_stu_square.nBMPH
        , m_npc_inf.right * rect.Width() / m_rcg.g_stu_square.nBMPW
        , m_npc_inf.bottom * rect.Height() / m_rcg.g_stu_square.nBMPH);
      pWnd->ReleaseDC(pDC);
      /////////////////////
      int X = xt + 0.5;
      int Y = yt + 0.5;
      m_par.x0 = xt;
      m_par.y0 = yt;

      CString csTmp;

      csTmp.Format(_T("X:%d Y:%d"), X, Y);

      GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);

      m_param->camera.Xxy[m_param->camera.Nxy][0] = xt; m_param->camera.Xxy[m_param->camera.Nxy][1] = yt;//xy保存的是一样的东西，相同位置，相同xy，是否可以用一个？不可以，0,1是图像x，y；2是固定1；3是电机步数
      m_param->camera.Yxy[m_param->camera.Nxy][0] = xt; m_param->camera.Yxy[m_param->camera.Nxy][1] = yt;

      TranNpcParam(&m_par);
      //int X = m_param->camera.g_factor[0][0] * xt + m_param->camera.g_factor[0][1] * yt + m_param->camera.g_factor[0][2] + 0.5;
      //int Y = m_param->camera.g_factor[1][0] * xt + m_param->camera.g_factor[1][1] * yt + m_param->camera.g_factor[1][2] + 0.5;
      //m_clked_pos_x = par.x;
      //m_clked_pos_y = par.y;

      csTmp.Format(_T("X:%d Y:%d"), m_par.x, m_par.y);
      GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
    }
    return;
  }
}

#include "DlgPriview.h"
void CCamera::OnBnClickedBtnCalibration()
{
  // TODO: 在此添加控件通知处理程序代码
  CDlgPriview dlg(this);
  dlg.DoModal();
}




void CCamera::OnBnClickedBtnVideo()
{
  // TODO: 在此添加控件通知处理程序代码

  if (!gb_PlayOrNot[1])
  {
    gb_PlayOrNot[1] = 1;
    //m_ksj.Preview(m_ch, GetDlgItem(IDC_PREVIEW));
    SetTimer(TIMER_VIDEO, 200, NULL);
    SetDlgItemText(IDC_BTN_VIDEO, _T("视频预览关"));
  }
  else
  {
    gb_PlayOrNot[1] = 0;

  //  m_ksj.StopPreview(m_ch);  //停止预览
    KillTimer(TIMER_VIDEO);
    SetDlgItemText(IDC_BTN_VIDEO, _T("视频预览"));

  }


}


void CCamera::OnPaint()
{
  CRect rect, rcUpdate;
  GetUpdateRect(&rcUpdate);

  CPaintDC dc(this); // device context for painting
  // TODO: 在此处添加消息处理程序代码
  // 不为绘图消息调用 CDialogEx::OnPaint()
  CRect rcWindow;
  GetDlgItem(IDC_PREVIEW)->GetWindowRect(rcWindow);
  ScreenToClient(rcWindow);

  if (rcWindow.left<rcUpdate.right && rcWindow.right>rcUpdate.left &&
    rcWindow.top<rcUpdate.bottom && rcWindow.bottom>rcUpdate.top)
  {
    GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
    GetDlgItem(IDC_PREVIEW)->RedrawWindow(&rect, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);//RDW_VALIDATE|RDW_UPDATENOW
  }
}


void CCamera::OnEnChangeEdtOutpallowl()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_OUTPALLOWL, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("识别参数"), _T("直线点距"), strValue, g_ini_path);
  m_param->camera.nPToL = nValue;
}


void CCamera::OnEnChangeEdtDefectallowl()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_DEFECTALLOWL, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("识别参数"), _T("缺陷点距"), strValue, g_ini_path);
  m_param->camera.nDefectPToL = nValue;
}


void CCamera::OnEnChangeEdtThreshold()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_THRESHOLD2, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("识别参数"), _T("阈值"), strValue, g_ini_path);
  m_param->camera.nThreshold = nValue;
}


void CCamera::OnEnChangeEdtFtoutpoint()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_FTOUTPOINT, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("识别参数"), _T("特征允许误差"), strValue, g_ini_path);
  m_param->camera.nAllowDefect = nValue;
  m_rcg.stuRef.Dev = m_param->camera.nAllowDefect;
}


void CCamera::OnBnClickedBtnGetset()
{
  // TODO: 在此添加控件通知处理程序代码
  //char szLan[128] = {0};
  //char szTemp[256] = {0};  
  //char szErrorMsg[256] = {0};
  //DWORD dwReturn;
  //if(!NET_GetConfig(glCamID,IPC_GET_SETTINGS_CAMERA_PARAM,&gstuCamParam, sizeof(gstuCamParam),&dwReturn))
  //{
  //	NET_GetErrorMsg(NET_GetLastError(), szErrorMsg);
  //	sprintf(szTemp, "%s %s", szLan, szErrorMsg);
  //	AfxMessageBox((CString)szTemp);
  //	return;
  //}
  ////
  //SetDlgItemInt(IDC_EDT_EV,gstuCamParam.maxexposuretime);
  //SetDlgItemInt(IDC_EDT_GAIN,gstuCamParam.maxgain);
  //SetDlgItemInt(IDC_EDT_BRIGHT,gstuCamParam.Brightness);
  //SetDlgItemInt(IDC_EDT_CONTRAST,gstuCamParam.Contrast);
  //SetDlgItemInt(IDC_EDT_SATURATION,gstuCamParam.Saturation);
  //SetDlgItemInt(IDC_EDT_SHARPNESS,gstuCamParam.Sharpness);
  //m_cmbAWB.SetCurSel(gstuCamParam.awb);
  //m_cmbBl.SetCurSel(gstuCamParam.BackLight);
  int    val;
  CString s1;

  m_ksj.GetParam(m_ch, KSJ_EXPOSURE, &val);
  s1.Format(_T("%d"), val);
  ((CEdit*)GetDlgItem(IDC_EDT_EV))->SetWindowText(s1);


  m_ksj.GetParam(m_ch, KSJ_RED, &val);
  s1.Format(_T("%d"), val);
  ((CEdit*)GetDlgItem(IDC_EDT_GAIN))->SetWindowText(s1);


  m_ksj.GetParam(m_ch, KSJ_BRIGHTNESS, &val);
  s1.Format(_T("%d"), val);
  ((CEdit*)GetDlgItem(IDC_EDT_BRIGHT))->SetWindowText(s1);


  m_ksj.GetParam(m_ch, KSJ_CONTRAST, &val);
  s1.Format(_T("%d"), val);
  ((CEdit*)GetDlgItem(IDC_EDT_CONTRAST))->SetWindowText(s1);

  return;
}


void CCamera::OnBnClickedChkAutothrd()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_AUTOTHRD);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_param->camera.bThrdAuto = 1;
  }
  else
  {
    m_param->camera.bThrdAuto = 0;
  }
}


void CCamera::OnBnClickedChkDelnoise()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_DELNOISE);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_param->camera.bDelNoise = 1;
  }
  else
  {
    m_param->camera.bDelNoise = 0;
  }
}


//void CCamera::OnBnClickedCheck3()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


void CCamera::OnBnClickedChkDebug()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_DEBUG);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_param->camera.bDebug = 1;
  }
  else
  {
    m_param->camera.bDebug = 0;
  }
}

#include "DlgPriview1.h"
void CCamera::OnBnClickedBtnSplit()
{
  // TODO: 在此添加控件通知处理程序代码

  CDlgPriview1 dlg(this);
  dlg.DoModal();


  //
  //CString m_bmp_file=GetMainFrame()->m_exe_path + _T("\\PIC\\原图0.bmp");
  //
  //CFileFind findini;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用   
  //BOOL ifFind = findini.FindFile(m_bmp_file);  
  //if( !ifFind )  
  //	return;
  //CBitmap m_bmp;//创建类成员
  //BITMAP bm;//存放位图信息的结构
  //HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//创建bitmap指针
  //m_bmp.Attach(hBitmap1);//关联句柄和cbitmap关联
  //m_bmp.GetBitmap(&bm);
  //CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
  //CDC* pDC = pWnd->GetDC();
  //CRect rect;
  //GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
  //CDC memDC;        //定义一个设备
  //CClientDC dc1(this);      //获取客户
  //memDC.CreateCompatibleDC( &dc1 );
  //memDC.SelectObject( m_bmp );  //为设备选择对象

  //mDCMem->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

  //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
  //memDC.DeleteDC();
  //m_bmp.DeleteObject();	
  //DeleteObject(hBitmap1);//记得删除	
  //
  //CPen PenRed;
  //CGdiObject *o = NULL;
  //PenRed.CreatePen(PS_SOLID,1,RGB(255,0,0));//创建一支红色的画笔.
  ////pWnd = GetDlgItem(IDC_image1); //IDC_STATIC是picture control的ID.这句是得到picture的句柄.
  ////pDC = pWnd->GetDC();//然后得到设备环境.
  ////RECT rect;//声明一个rect
  ////pWnd->GetClientRect(&rect);//并把picture控件的坐标用rect接收.
  //mDCMem->SelectObject(PenRed);//选中画笔.

  ///*m_param->camera.nPToL=2;//直线点距
  //m_param->camera.nDefectPToL=10;//缺陷点距
  //m_param->camera.nThreshold=30;//阀值*/
  ////char * chPath = (LPSTR)(LPCTSTR)m_bmp_file;
  //char chPath[MAX_PATH];
  ////WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,readPath,CStringA(m_bmp_file).GetLength(),NULL,NULL); 
  //WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_bmp_file,-1,chPath,sizeof(chPath),NULL,NULL); 
  ////
  //LARGE_INTEGER l_lgint_start, l_lgint_end;
  //LARGE_INTEGER l_lgint_freq;
  //QueryPerformanceFrequency(&l_lgint_freq);  
  //QueryPerformanceCounter(&l_lgint_start);
  ////
  //m_rcg.RCGBMPSPLIT(mDCMem,rect,chPath,m_param->camera.nPToL,m_param->camera.nDefectPToL,m_param->camera.nThreshold,m_param->camera.bDebug,m_param->camera.bIsCir,m_param->camera.bThrdAuto,m_param->camera.bDelNoise);
  ////
  //QueryPerformanceCounter(&l_lgint_end);
  //double dTmpRT=double(l_lgint_end.QuadPart-l_lgint_start.QuadPart)/double(l_lgint_freq.QuadPart);	
  //CString csTmp;
  //csTmp.Format(_T("%.2f秒"),dTmpRT);
  //SetDlgItemText(IDC_SELECT_RESULT,csTmp);
  //	//
  //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
  //pWnd->ReleaseDC(pDC);
  //m_param->camera.bClbPos=1;
}


void CCamera::OnBnClickedBtnReconnect()
{
  // TODO: 在此添加控件通知处理程序代码
  //m_ch = m_ksj.GetKsj(KSJ_UD205M_SGYY);
  //if (0 > m_ch)
  //  AfxMessageBox(_T("KSJ_UD205M_SGYY 相机打开失败	,请检查是否连接该相机"));
  m_cam.init(GetDlgItem(IDC_PREVIEW)->m_hWnd);

}
//
//void CCamera::IIICStartConnect()
//{
//	//
//	char sUserName[32];
//	char sPassword[16];
//	char sAddressIp[16];
//	DWORD dwPort;
//	
//	//
//
//	char szLan[128] = {0};
//	char szTemp[256] = {0};
//	CString csTemp;
//
//	if (glCamID >= 0)
//	{
//		MessageBox(_T("连接已经启动!"));
//		return;
//	}
//	//test
//	strncpy(sUserName, "admin", 32);
//	strncpy(sPassword, "9999", 16);
//	strncpy(sAddressIp, "192.168.1.102", 16);
//	dwPort = 8000;
//	//end
//	DWORD dwReturn;
//	NET_GetConfig(1,IPC_GET_NETWORK_PARAM,&gstuCamNetCfg, sizeof(gstuCamNetCfg),&dwReturn);
//	//sprintf(sUserName,"%s.%s.%s.%s",gstuCamNetCfg.ip.S_un.S_addr[1]);
//
//	NET_SDK_DEVICEINFO m_struDeviceInfo;
//	memset(&m_struDeviceInfo, 0, sizeof(m_struDeviceInfo));
//
//	//NET_SetMessageCallBack(MessageCallback0,NULL);
//
//	//	NET_DVR_SetLogToFile(3, NULL, FALSE);
//	//	NET_SetLogToFile(1,);//SDK日志功能未实现
//
//
//	NET_SetConnectTime(3000,3);
//	NET_SetReconnect(3000,TRUE);
//	glCamID = NET_Login(sAddressIp, (WORD)dwPort, sUserName, sPassword, NULL);
//
//
//
//	if (glCamID < 0)
//		AfxMessageBox(_T("连接IIIC相机失败！"));
//	else if(gclsIiic.OpenRtsp()!=1)
//		AfxMessageBox(_T("RTSP FAIL"));
//	/*int connectCNT= 0;
//	while(glCamID < 0 )
//	{
//		glCamID = NET_Login(sAddressIp, (WORD)dwPort, sUserName, sPassword, NULL);
//		connectCNT ++;
//		if (connectCNT >10000)
//		{
//			AfxMessageBox(_T("连接IIIC相机失败！"));
//			OnClose();
//			break;
//		}
//	}*/
//
//
//}


void CCamera::OnBnClickedBtnParmsend()
{
  // TODO: 在此添加控件通知处理程序代码

  ////m_clked_pos_x = X;
  ////m_clked_pos_y = Y;m_par
  //if (0 == g_dlgDevice->ParamMove(m_par.x, m_par.y, m_par.deg, m_par.pn))
  //{
  //  AfxMessageBox(_T("发送成功"));
  //}
  //else
  //{
  //  AfxMessageBox(_T("status is not ready"));
  //}

}


void CCamera::OnBnClickedBtnAuto()
{
  // TODO: 在此添加控件通知处理程序代码

  //CString str;
  //GetDlgItemText(IDC_BTN_AUTO, str);
  //((CButton*)GetDlgItem(IDC_BTN_AUTO))->EnableWindow(FALSE);
  //if ("自动" == str)
  //{
  //  if (!gb_PlayOrNot[1])
  //  {
  //    gb_PlayOrNot[1] = 1;
  //    m_ksj.Preview(m_ch, GetDlgItem(IDC_PREVIEW));
  //  }

  //  gstuRun.unStatRun = RUN_STAT_RUN;
  //  gTrdAuto = AfxBeginThread(Thread_Auto, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
  //  gTrdAuto->m_bAutoDelete = TRUE;
  //  gTrdAuto->ResumeThread();
  //  SetDlgItemText(IDC_BTN_AUTO, _T("自动关"));
  //  //

  //  GetDlgItem(IDC_BTN_VIDEO)->EnableWindow(FALSE);
  //  GetDlgItem(IDC_BTN_SNAP)->EnableWindow(FALSE);
  //  GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
  //  GetDlgItem(IDC_BTN_PARMSEND)->EnableWindow(FALSE);
  //  GetDlgItem(IDC_BTN_CALIBRATION)->EnableWindow(FALSE);
  //  GetDlgItem(IDC_BTN_RECONNECT)->EnableWindow(FALSE);
  //  GetDlgItem(IDC_BTN_SPLIT)->EnableWindow(FALSE);
  //  //
  //}
  //else
  //{
  //  if (gb_PlayOrNot[1])
  //  {
  //    gb_PlayOrNot[1] = 0;
  //    m_ksj.StopPreview(m_ch);  //停止预览
  //  }
  //  gstuRun.unStatRun = RUN_STAT_STOP;
  //  SetDlgItemText(IDC_BTN_AUTO, _T("自动"));

  //  //

  //  GetDlgItem(IDC_BTN_VIDEO)->EnableWindow(TRUE);
  //  GetDlgItem(IDC_BTN_SNAP)->EnableWindow(TRUE);
  //  GetDlgItem(IDC_BTN_TEST)->EnableWindow(TRUE);
  //  GetDlgItem(IDC_BTN_PARMSEND)->EnableWindow(TRUE);
  //  GetDlgItem(IDC_BTN_CALIBRATION)->EnableWindow(TRUE);
  //  GetDlgItem(IDC_BTN_RECONNECT)->EnableWindow(TRUE);
  //  GetDlgItem(IDC_BTN_SPLIT)->EnableWindow(TRUE);

  //  //

  //}
}


void CCamera::OnEnChangeEdtSquMindeg()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_SQU_MINDEG, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("方片参数"), _T("最小角"), strValue, g_ini_path);
  m_rcg.stuRef.SAg = nValue;
}


void CCamera::OnEnChangeEdtSquMaxdeg()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_SQU_MAXDEG, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("方片参数"), _T("最大角"), strValue, g_ini_path);
  m_rcg.stuRef.BAg = nValue;
}


void CCamera::OnEnChangeEdtPnMindeg()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_PN_MINDEG, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("方向参数"), _T("最小角"), strValue, g_ini_path);
  m_rcg.stuRef.PN_sag = nValue;
}


void CCamera::OnEnChangeEdtPnMaxdeg()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_PN_MAXDEG, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("方向参数"), _T("最大角"), strValue, g_ini_path);
  m_rcg.stuRef.PN_bag = nValue;
}


void CCamera::OnEnChangeEdtThreshold2()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_THRESHOLD2, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("识别参数"), _T("阈值"), strValue, g_ini_path);
  m_param->camera.nThreshold = nValue;
}


void CCamera::OnBnClickedBtnClbpos2test()
{
  // TODO: 在此添加控件通知处理程序代码
  int x = GetDlgItemInt(IDC_EDT_CLBPOS2X);
  int y = GetDlgItemInt(IDC_EDT_CLBPOS2Y);
  m_io->MotoRunNoWait(MOTOR_A, x);
  m_io->MotoRunNoWait(MOTOR_B, y);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}


void CCamera::OnEnChangeEdtClbpos1x()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_CLBPOS1X, &bFlag, 1);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  //  SetDlgItemInt(IDC_EDT_CLBPOS1X, nValue, 1);
  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("取片校准点"), _T("1X"), strValue, g_ini_path);
  m_param->camera.nClbPosX[0] = nValue;

}



void CCamera::OnEnChangeEdtClbpos2x()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_CLBPOS2X, &bFlag, 1);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  //SetDlgItemInt(IDC_EDT_CLBPOS2X, nValue, 1);
  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("取片校准点"), _T("2X"), strValue, g_ini_path);
  m_param->camera.nClbPosX[1] = nValue;
}


void CCamera::OnEnChangeEdtClbpos3x()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_CLBPOS3X, &bFlag, 1);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  //SetDlgItemInt(IDC_EDT_CLBPOS3X, nValue, 1);
  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("取片校准点"), _T("3X"), strValue, g_ini_path);
  m_param->camera.nClbPosX[2] = nValue;
}


void CCamera::OnEnChangeEdtClbpos1y()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_CLBPOS1Y, &bFlag, 1);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  //SetDlgItemInt(IDC_EDT_CLBPOS1Y, nValue, 1);
  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("取片校准点"), _T("1Y"), strValue, g_ini_path);
  m_param->camera.nClbPosY[0] = nValue;
}

void CCamera::OnEnChangeEdtClbpos2y()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_CLBPOS2Y, &bFlag, 1);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  //SetDlgItemInt(IDC_EDT_CLBPOS2Y, nValue, 1);
  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("取片校准点"), _T("2Y"), strValue, g_ini_path);
  m_param->camera.nClbPosY[1] = nValue;
}


void CCamera::OnEnChangeEdtClbpos3y()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_CLBPOS3Y, &bFlag, 1);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  // SetDlgItemInt(IDC_EDT_CLBPOS3Y, nValue, 1);
  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("取片校准点"), _T("3Y"), strValue, g_ini_path);
  m_param->camera.nClbPosY[2] = nValue;
}


void CCamera::OnEnChangeEdtTransferx()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_TRANSFERX, &bFlag, 1);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  //  SetDlgItemInt(IDC_EDT_CLBPOS1X, nValue, 1);
  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("中转点"), _T("X"), strValue, g_ini_path);
  m_param->camera.transferx = nValue;
}


void CCamera::OnEnChangeEdtTransfery()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_TRANSFERY, &bFlag, 1);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }

  //  SetDlgItemInt(IDC_EDT_CLBPOS1X, nValue, 1);
  CString strValue = _T("");
  CFileFind findini;
  BOOL ifFind = findini.FindFile(g_ini_path);

  if (!ifFind)
  {
    AfxMessageBox(_T("无配置文件"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("中转点"), _T("Y"), strValue, g_ini_path);
  m_param->camera.transfery = nValue;
}


void CCamera::OnBnClickedBtnClbpos1test()
{
  // TODO: 在此添加控件通知处理程序代码
  int x = GetDlgItemInt(IDC_EDT_CLBPOS1X);
  int y = GetDlgItemInt(IDC_EDT_CLBPOS1Y);
  m_io->MotoRunNoWait(MOTOR_A, x);
  m_io->MotoRunNoWait(MOTOR_B, y);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}



void CCamera::OnBnClickedBtnClbpos3test()
{
  // TODO: 在此添加控件通知处理程序代码
  int x = GetDlgItemInt(IDC_EDT_CLBPOS3X);
  int y = GetDlgItemInt(IDC_EDT_CLBPOS3Y);
  m_io->MotoRunNoWait(MOTOR_A, x);
  m_io->MotoRunNoWait(MOTOR_B, y);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}


void CCamera::OnBnClickedBtnTransfertest()
{
  // TODO: 在此添加控件通知处理程序代码
  int x = GetDlgItemInt(IDC_EDT_TRANSFERX);
  int y = GetDlgItemInt(IDC_EDT_TRANSFERY);
  m_io->MotoRunNoWait(MOTOR_A, x);
  m_io->MotoRunNoWait(MOTOR_B, y);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}


void CCamera::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  if(-1 == m_cam.snap())
	  m_cam.init(GetDlgItem(IDC_PREVIEW)->m_hWnd);
  CDialogEx::OnTimer(nIDEvent);
}


void CCamera::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_cam.unInit();// 不执行，程序假死，退出不了
}
void CCamera::VideoOnOff(BOOL on)
{
  // TODO: 在此添加控件通知处理程序代码

  if (on)
  {
    SetTimer(TIMER_VIDEO, 200, NULL);
  }
  else
  {
    KillTimer(TIMER_VIDEO);
  }


}

void CCamera::OnBnClickedBtnTestzero()
{
	// TODO: 在此添加控件通知处理程序代码
  m_io->MotoZeroNoWait(MOTOR_A);
  m_io->MotoZeroNoWait(MOTOR_B);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}
