// Camera.cpp : ʵ���ļ�
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
//    //=====================================================ʶ��ʼ===============================================================
//    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, gstuPathInf.csPathExe + _T("\\PIC\\ԭͼ0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);
//    TimeRecord(2, 1);//�������գ���ʱ��ʼ
//
//    if (nullptr == gclsCamera.SnapEx(gnChannel, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, NULL))
//    {
//      Sleep(500);
//
//      if (takeCnt++ > 10)//ȡ����ͼ���˳�
//      {
//        AfxMessageBox(_T("ȡ�������ͼ���������"));
//        takeCnt = 0;
//        gstuRun.unStatRun == RUN_STAT_STOP;
//        pdlg->SetDlgItemText(IDC_BTN_AUTO, _T("�Զ�"));
//        break;
//      }
//      continue;
//    }
//
//    TimeRecord(2, 0);//���ս�������ʱ����
//    TimeRecord(3, 1);//ʶ��ʼ����ʱ��ʼ
//    //
//    //m_rcg.RCGBMPSPLIT(NULL, NULL, l_AnsiStr, m_param->camera.nPToL, m_param->camera.nDefectPToL, m_param->camera.nThreshold, 0, m_param->camera.bIsCir, m_param->camera.bThrdAuto, m_param->camera.bDelNoise);
//
//
//    m_rcg.RCGBMP(NULL, NULL, l_AnsiStr, m_param->camera.nPToL, m_param->camera.nDefectPToL, m_param->camera.nThreshold, 0, m_param->camera.bIsCir, m_param->camera.bThrdAuto, m_param->camera.bDelNoise);
//    //
//
//    //
//    TimeRecord(3, 0);//ʶ���������ʱ����
//    //=====================================================���俪ʼ===============================================================
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
//      //���ж�
//      if (m_rcg.g_stu_square.pnLen[i] < (m_rcg.stuRef.Len - m_rcg.stuRef.Dev) || m_rcg.g_stu_square.pnLen[i] > (m_rcg.stuRef.Len + m_rcg.stuRef.Dev))
//      {
//        continue;
//      }
//      //���ж�
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
//      csTmp.Format(_T("��%d ��%d ���� %d �Ƕ�%.1f X:%d Y:%d"), m_rcg.g_stu_square.pnLen[i], m_rcg.g_stu_square.pnWth[i], (int)(par.pn0), par.deg0, (int)(par.x0), (int)(par.y0));
//      pdlg->GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
//      TranNpcParam(&par);
//
//      csTmp.Format(_T("No%d ���� %d �Ƕ�%d X:%d Y:%d"), i, par.pn, par.deg, par.x, par.y);
//      pdlg->GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
//
//      //���Ͳ���
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
//����ϵת��ϵ������
void calparameter(double(*xy)[4], double* factor)
//void calparameter(double **xy,double *factor)
{
  int k, i, j, f;//ѭ������
  for (k = 0; k < 3; k++)//����Ԫ��˹��Ԫ               
  {
    //double ark=findmax(k,xy);
    double max = 0;
    for (int i = k; i < 3; i++)
      if (fabs(xy[i][k]) > max)        //�������е�Ԫ�ؽ���ȡ����ֵ�Ƚϴ�С������
      {
        max = fabs(xy[i][k]);
        f = i;
      }

    if (max == 0)
    {
      AfxMessageBox(_T("�˷������޽�!"));
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
  factor[2] = xy[2][3] / xy[2][2];      //��������һ��δ֪���Ľ�
  for (k = 1; k >= 0; k--)//�ص�
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
// CCamera �Ի���

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


// CCamera ��Ϣ�������


void CCamera::OnBnClickedBtnSnap()
{

  ///////////////////////////////////ksj///////////////////////////
  LARGE_INTEGER l_lgint_start, l_lgint_end;
  LARGE_INTEGER l_lgint_freq;
  QueryPerformanceFrequency(&l_lgint_freq);
  QueryPerformanceCounter(&l_lgint_start);
  char l_AnsiStr[MAX_PATH];
  WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, GetMainFrame()->m_exe_path + _T("\\PIC\\ԭͼ0.bmp"), -1, l_AnsiStr, sizeof(l_AnsiStr), NULL, NULL);

  int nWidth, nHeight;
  int nBitCount;
  int nBitsPerSample;
  //unsigned char* pData = m_ksj.SnapEx(m_ch, &nWidth, &nHeight, &nBitCount, &nBitsPerSample, l_AnsiStr, GetDlgItem(IDC_PREVIEW));
  m_cam.snap();
  m_cam.captureBmp(l_AnsiStr);
  QueryPerformanceCounter(&l_lgint_end);
  double dTmpRT = double(l_lgint_end.QuadPart - l_lgint_start.QuadPart) / double(l_lgint_freq.QuadPart);
  CString csTmp;
  csTmp.Format(_T("%.2f��"), dTmpRT);
  SetDlgItemText(IDC_SELECT_RESULT, csTmp);

  //	bOverRcg=1;
}


void CCamera::OnBnClickedBtnTest()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������

  CString m_bmp_file = GetMainFrame()->m_exe_path + _T("\\PIC\\ԭͼ0.bmp");

  CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
  BOOL ifFind = findini.FindFile(m_bmp_file);

  if (!ifFind)
    return;

  CBitmap m_bmp;//�������Ա
  BITMAP bm;//���λͼ��Ϣ�Ľṹ
  HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL, m_bmp_file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//����bitmapָ��
  m_bmp.Attach(hBitmap1);//���������cbitmap����
  m_bmp.GetBitmap(&bm);
  CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
  CDC* pDC = pWnd->GetDC();
  CRect rect;
  GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
  CDC memDC;        //����һ���豸
  CClientDC dc1(this);      //��ȡ�ͻ�
  memDC.CreateCompatibleDC(&dc1);
  memDC.SelectObject(m_bmp);  //Ϊ�豸ѡ�����

  mDCMem->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

  pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
  memDC.DeleteDC();
  m_bmp.DeleteObject();
  DeleteObject(hBitmap1);//�ǵ�ɾ��	

  CPen PenRed;
  CGdiObject* o = NULL;
  PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//����һ֧��ɫ�Ļ���.

  //pWnd = GetDlgItem(IDC_image1); //IDC_STATIC��picture control��ID.����ǵõ�picture�ľ��.
  //pDC = pWnd->GetDC();//Ȼ��õ��豸����.
  //RECT rect;//����һ��rect
  //pWnd->GetClientRect(&rect);//����picture�ؼ���������rect����.
  mDCMem->SelectObject(PenRed);//ѡ�л���.

  /*m_param->camera.nPToL=2;//ֱ�ߵ��
  m_param->camera.nDefectPToL=10;//ȱ�ݵ��
  m_param->camera.nThreshold=30;//��ֵ*/
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
  csTmp.Format(_T("%.2f��"), dTmpRT);
  SetDlgItemText(IDC_SELECT_RESULT, csTmp);
  //
  pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);

  pDC->SelectObject(PenRed);//ѡ�л���.
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
    l_cstr_tmp.Format(_T("%d��%d"),g_class_img_recognition.g_stu_square.crests,g_class_img_recognition.g_stu_square.sags);
    GetDlgItem(IDC_EDIT_BROKEN_RAW)->SetWindowText(l_cstr_tmp);
    l_cstr_tmp.Format(_T("%.2lf��%.2lf��%.2lf��%.2lf"),g_class_img_recognition.g_stu_square.d_len[0],g_class_img_recognition.g_stu_square.d_len[1],g_class_img_recognition.g_stu_square.d_len[2],g_class_img_recognition.g_stu_square.d_len[3]);
    GetDlgItem(IDC_EDIT_SIZE_RAW)->SetWindowText(l_cstr_tmp);*/
}


void CCamera::OnBnClickedBtnCamset()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������


  m_ksj.SetParam(m_ch, KSJ_EXPOSURE, GetDlgItemInt(IDC_EDT_EV));//
  m_ksj.SetParam(m_ch, KSJ_RED, GetDlgItemInt(IDC_EDT_GAIN));//
  m_ksj.SetParam(m_ch, KSJ_BRIGHTNESS, GetDlgItemInt(IDC_EDT_BRIGHT));//
  m_ksj.SetParam(m_ch, KSJ_CONTRAST, GetDlgItemInt(IDC_EDT_CONTRAST));//
}


BOOL CCamera::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  �ڴ���Ӷ���ĳ�ʼ��
  //CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
  //CDC* pDC = pWnd->GetDC();
  //mDCMem= new CDC;
  //CClientDC dc1(GetDlgItem(IDC_PREVIEW)->GetDC());      //��ȡ�ͻ�
  CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
  CDC* pDC = pWnd->GetDC();
  CRect rect;
  GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
  mDCMem = new CDC;
  mBMPMem = new CBitmap;
  mDCMem->CreateCompatibleDC(pDC);
  mBMPMem->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  mBMPOld = mDCMem->SelectObject(mBMPMem);//�������Ͼ䣬��mdcmen������Ч
  mDCMem->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(mDCMem->m_hDC, 0, 0, NULL);
  pWnd->ReleaseDC(pDC);
  //mDCMem->CreateCompatibleDC(GetDlgItem(IDC_PREVIEW)->GetDC());

  //=====================CAMERA===========================
  //combobox�������tab�ؼ�֮��
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

  CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
  BOOL ifFind = findini.FindFile(g_ini_path);
  if (ifFind)
  {//����ini
    //����ini�ļ�����Ӧ�ֶ�
    iniint = GetPrivateProfileInt(_T("�������"), _T("����ع�ֵ"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_EV, iniint);
    iniint = GetPrivateProfileInt(_T("�������"), _T("�������"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_GAIN, iniint);
    iniint = GetPrivateProfileInt(_T("�������"), _T("����"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_BRIGHT, iniint);
    iniint = GetPrivateProfileInt(_T("�������"), _T("�Աȶ�"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CONTRAST, iniint);
    iniint = GetPrivateProfileInt(_T("�������"), _T("���Ͷ�"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_SATURATION, iniint);
    iniint = GetPrivateProfileInt(_T("�������"), _T("���"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_SHARPNESS, iniint);
    iniint = GetPrivateProfileInt(_T("�������"), _T("��ƽ�����"), 0, g_ini_path);
    m_cmbAWB.SetCurSel(iniint);
    iniint = GetPrivateProfileInt(_T("�������"), _T("����ֵ"), 0, g_ini_path);
    m_cmbBl.SetCurSel(iniint);
    //���ʶ��
    iniint = GetPrivateProfileInt(_T("ʶ�����"), _T("ֱ�ߵ��"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_OUTPALLOWL, iniint); m_param->camera.nPToL = iniint;
    iniint = GetPrivateProfileInt(_T("ʶ�����"), _T("ȱ�ݵ��"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_DEFECTALLOWL, iniint); m_param->camera.nDefectPToL = iniint;
    iniint = GetPrivateProfileInt(_T("ʶ�����"), _T("��ֵ"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_THRESHOLD2, iniint); m_param->camera.nThreshold = iniint;
    iniint = GetPrivateProfileInt(_T("ʶ�����"), _T("�����������"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_FTOUTPOINT, iniint); m_param->camera.nAllowDefect = iniint;

    //ʶ��

    iniint = GetPrivateProfileInt(_T("��Ƭ����"), _T("��С��"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_SQU_MINDEG, iniint); m_rcg.stuRef.SAg = iniint;
    iniint = GetPrivateProfileInt(_T("��Ƭ����"), _T("����"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_SQU_MAXDEG, iniint); m_rcg.stuRef.BAg = iniint;
    iniint = GetPrivateProfileInt(_T("�������"), _T("��С��"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_PN_MINDEG, iniint); m_rcg.stuRef.PN_sag = iniint;
    iniint = GetPrivateProfileInt(_T("�������"), _T("����"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_PN_MAXDEG, iniint); m_rcg.stuRef.PN_bag = iniint;

    //
    iniint = GetPrivateProfileInt(_T("ȡƬУ׼��"), _T("1X"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS1X, iniint);
    m_param->camera.nClbPosX[0] = iniint;
    iniint = GetPrivateProfileInt(_T("ȡƬУ׼��"), _T("1Y"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS1Y, iniint);
    m_param->camera.nClbPosY[0] = iniint;
    iniint = GetPrivateProfileInt(_T("ȡƬУ׼��"), _T("2X"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS2X, iniint);
    m_param->camera.nClbPosX[1] = iniint;
    iniint = GetPrivateProfileInt(_T("ȡƬУ׼��"), _T("2Y"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS2Y, iniint);
    m_param->camera.nClbPosY[1] = iniint;
    iniint = GetPrivateProfileInt(_T("ȡƬУ׼��"), _T("3X"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS3X, iniint);
    m_param->camera.nClbPosX[2] = iniint;
    iniint = GetPrivateProfileInt(_T("ȡƬУ׼��"), _T("3Y"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_CLBPOS3Y, iniint);
    m_param->camera.nClbPosY[2] = iniint;
    // ��ת��
    iniint = GetPrivateProfileInt(_T("��ת��"), _T("X"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_TRANSFERX, iniint);
    m_param->camera.transferx = iniint;
    iniint = GetPrivateProfileInt(_T("��ת��"), _T("Y"), 0, g_ini_path);
    SetDlgItemInt(IDC_EDT_TRANSFERY, iniint);
    m_param->camera.transfery = iniint;
    //��ȡini
    TCHAR strbuff[256];
    CString strValue;
    GetPrivateProfileString(_T("У׼ϵ��"), _T("00"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[0][0] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("У׼ϵ��"), _T("01"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[0][1] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("У׼ϵ��"), _T("02"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[0][2] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("У׼ϵ��"), _T("10"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[1][0] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("У׼ϵ��"), _T("11"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[1][1] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
    GetPrivateProfileString(_T("У׼ϵ��"), _T("12"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_param->camera.g_factor[1][2] = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();


    GetPrivateProfileString(_T("��Χ����"), _T("left"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_npc_inf.left = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();

    GetPrivateProfileString(_T("��Χ����"), _T("top"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_npc_inf.top = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();

    GetPrivateProfileString(_T("��Χ����"), _T("right"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_npc_inf.right = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();

    GetPrivateProfileString(_T("��Χ����"), _T("bottom"), NULL, strbuff, 80, g_ini_path);
    strValue = strbuff;
    m_npc_inf.bottom = _wtof(strValue.GetBuffer(strValue.GetLength()));
    strValue.ReleaseBuffer();
  }
  else
    AfxMessageBox(_T("iniû�д���"));

  m_param->camera.bThrdAuto = 0;//�Զ���ֵ��ʶ0
  m_param->camera.bDelNoise = 0;
  m_param->camera.bDebug = 0;

  //m_ch = m_ksj.GetKsj(KSJ_UD205M_SGYY);
  //if (0 > m_ch)
  //  AfxMessageBox(_T("KSJ_UD205M_SGYY �����ʧ��	,�����Ƿ����Ӹ����"));
  //if(-1==m_cam.init(GetDlgItem(IDC_PREVIEW)->m_hWnd))
//	  AfxMessageBox(_T("usb�������ʧ��"));
  
  return TRUE;  // return TRUE unless you set the focus to a control
  // �쳣: OCX ����ҳӦ���� FALSE
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
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  //NpcParm par = { 0 };

  if (0 == m_rcg.g_stu_square.nN)
  {
    //return;
  }
  double xt, yt;
  POINT pt;//�����
  GetCursorPos(&pt);//ȡ�õ�ǰ����
  CRect lRect;
  GetDlgItem(IDC_PREVIEW)->GetWindowRect(&lRect);
  double temp_x = pt.x - lRect.left;
  double temp_y = pt.y - lRect.top;
  xt =/*(int)*/(temp_x * m_rcg.g_stu_square.nBMPW / (lRect.Width()));//����������Ӧ��ͼ���ϵ�x��
  yt =/*(int)*/(temp_y * m_rcg.g_stu_square.nBMPH / (lRect.Height()));//����������Ӧ��ͼ���ϵ�y��
  //if(!m_RadioCircle && m_RadioSquare)
  {
    //if(avg1.empty()){AfxMessageBox(_T("û�ж�λ���꣬�붨λ"));return;}

    int temp = 0;
    int i;

    for (i = 0; i < m_rcg.g_stu_square.nN; i++)
    {

      if (sqrt(pow(double(m_rcg.g_stu_square.pnZPX[i] - xt), 2) + pow(double(m_rcg.g_stu_square.pnZPY[i] - yt), 2)) < ((m_rcg.g_stu_square.pnWth[i]) >> 1)) //���λ����Ƭ�Ĵ����������С��s_sort.allowable_devision����ô����Ҫ���Ե��㲻׼ʱ���õ���
      {
        //DrawPicToHDC(line_img, IDC_PREVIEW);//��ͼƬ�ؼ�����ʾ
        CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
        CDC* pDC = pWnd->GetDC();
        CRect rect;
        GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
        CPen PenRed, PenGreen;
        PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//����һ֧��ɫ�Ļ���.
        PenGreen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//����һ֧��ɫ�Ļ���.
        pDC->SelectObject(PenRed);//ѡ�л���.
        pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
        if (m_param->camera.bIsCir)
        {
          CString csTmp;
          csTmp.Format(_T("�뾶%d"), m_rcg.g_stu_square.pnWth[i]);
          GetDlgItem(IDC_SELECT_RESULT)->SetWindowText(csTmp);
          pDC->Ellipse
          (m_rcg.g_stu_square.pnZPX[i] * rect.Width() / m_rcg.g_stu_square.nBMPW - m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_rcg.g_stu_square.nBMPW
            , m_rcg.g_stu_square.pnZPY[i] * rect.Height() / m_rcg.g_stu_square.nBMPH - m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_rcg.g_stu_square.nBMPW
            , m_rcg.g_stu_square.pnZPX[i] * rect.Width() / m_rcg.g_stu_square.nBMPW + m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_rcg.g_stu_square.nBMPW
            , m_rcg.g_stu_square.pnZPY[i] * rect.Height() / m_rcg.g_stu_square.nBMPH + m_rcg.g_stu_square.pnWth[i] * rect.Width() / m_rcg.g_stu_square.nBMPW
          );
          //���浽��������
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
          ////���浽��������
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
          //csTmp.Format(_T("��%d ��%d ���� %d �Ƕ�%.1f X:%d Y:%d"), m_rcg.g_stu_square.pnLen[i],m_rcg.g_stu_square.pnWth[i], (int)(m_par.pn0), (angle_test), X, Y);

          csTmp.Format(_T("��%d ��%d ���� %d �Ƕ�%.1f X:%d Y:%d"), m_rcg.g_stu_square.pnLen[i], m_rcg.g_stu_square.pnWth[i], (int)(m_par.pn0), (m_par.deg0), (int)(m_par.x0), (int)(m_par.y0));
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
        m_param->camera.Xxy[m_param->camera.Nxy][0] = m_rcg.g_stu_square.pnZPX[i]; m_param->camera.Xxy[m_param->camera.Nxy][1] = m_rcg.g_stu_square.pnZPY[i];//xy�������һ���Ķ�������ͬλ�ã���ͬxy���Ƿ������һ���������ԣ�0,1��ͼ��x��y��2�ǹ̶�1��3�ǵ������
        m_param->camera.Yxy[m_param->camera.Nxy][0] = m_rcg.g_stu_square.pnZPX[i]; m_param->camera.Yxy[m_param->camera.Nxy][1] = m_rcg.g_stu_square.pnZPY[i];


        //int X = m_param->camera.g_factor[0][0] * (double)m_rcg.g_stu_square.pnZPX[i] + m_param->camera.g_factor[0][1] * (double)m_rcg.g_stu_square.pnZPY[i] + m_param->camera.g_factor[0][2]+0.5;
        //int Y = m_param->camera.g_factor[1][0] * (double)m_rcg.g_stu_square.pnZPX[i] + m_param->camera.g_factor[1][1] * (double)m_rcg.g_stu_square.pnZPY[i] + m_param->camera.g_factor[1][2]+0.5;
        TranNpcParam(&m_par);

        //m_clked_pos_x = par.x;
        //m_clked_pos_y = par.y;

        CString csTmp;
        csTmp.Format(_T("No%d ���� %d �Ƕ�%d X:%d Y:%d"), i, m_par.pn, m_par.deg, m_par.x, m_par.y);
        GetDlgItem(IDC_SELECT_XY)->SetWindowText(csTmp);
        m_param->camera.bClbPos = 1;
        break;
      }

    }
    //û�е㵽Ƭ��
    if (i == m_rcg.g_stu_square.nN) {
      CWnd* pWnd = GetDlgItem(IDC_PREVIEW);
      CDC* pDC = pWnd->GetDC();
      CRect rect;
      GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
      CPen PenRed, PenGreen;
      PenRed.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//����һ֧��ɫ�Ļ���.
      PenGreen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//����һ֧��ɫ�Ļ���.
      pDC->SelectObject(PenRed);//ѡ�л���.
      pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
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

      m_param->camera.Xxy[m_param->camera.Nxy][0] = xt; m_param->camera.Xxy[m_param->camera.Nxy][1] = yt;//xy�������һ���Ķ�������ͬλ�ã���ͬxy���Ƿ������һ���������ԣ�0,1��ͼ��x��y��2�ǹ̶�1��3�ǵ������
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
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CDlgPriview dlg(this);
  dlg.DoModal();
}




void CCamera::OnBnClickedBtnVideo()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������

  if (!gb_PlayOrNot[1])
  {
    gb_PlayOrNot[1] = 1;
    //m_ksj.Preview(m_ch, GetDlgItem(IDC_PREVIEW));
    SetTimer(TIMER_VIDEO, 200, NULL);
    SetDlgItemText(IDC_BTN_VIDEO, _T("��ƵԤ����"));
  }
  else
  {
    gb_PlayOrNot[1] = 0;

  //  m_ksj.StopPreview(m_ch);  //ֹͣԤ��
    KillTimer(TIMER_VIDEO);
    SetDlgItemText(IDC_BTN_VIDEO, _T("��ƵԤ��"));

  }


}


void CCamera::OnPaint()
{
  CRect rect, rcUpdate;
  GetUpdateRect(&rcUpdate);

  CPaintDC dc(this); // device context for painting
  // TODO: �ڴ˴������Ϣ����������
  // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
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
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ʶ�����"), _T("ֱ�ߵ��"), strValue, g_ini_path);
  m_param->camera.nPToL = nValue;
}


void CCamera::OnEnChangeEdtDefectallowl()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ʶ�����"), _T("ȱ�ݵ��"), strValue, g_ini_path);
  m_param->camera.nDefectPToL = nValue;
}


void CCamera::OnEnChangeEdtThreshold()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ʶ�����"), _T("��ֵ"), strValue, g_ini_path);
  m_param->camera.nThreshold = nValue;
}


void CCamera::OnEnChangeEdtFtoutpoint()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ʶ�����"), _T("�����������"), strValue, g_ini_path);
  m_param->camera.nAllowDefect = nValue;
  m_rcg.stuRef.Dev = m_param->camera.nAllowDefect;
}


void CCamera::OnBnClickedBtnGetset()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
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
  // TODO: �ڴ���ӿؼ�֪ͨ����������
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
  // TODO: �ڴ���ӿؼ�֪ͨ����������
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}


void CCamera::OnBnClickedChkDebug()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
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
  // TODO: �ڴ���ӿؼ�֪ͨ����������

  CDlgPriview1 dlg(this);
  dlg.DoModal();


  //
  //CString m_bmp_file=GetMainFrame()->m_exe_path + _T("\\PIC\\ԭͼ0.bmp");
  //
  //CFileFind findini;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����   
  //BOOL ifFind = findini.FindFile(m_bmp_file);  
  //if( !ifFind )  
  //	return;
  //CBitmap m_bmp;//�������Ա
  //BITMAP bm;//���λͼ��Ϣ�Ľṹ
  //HBITMAP hBitmap1 = (HBITMAP)LoadImage(NULL,m_bmp_file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//����bitmapָ��
  //m_bmp.Attach(hBitmap1);//���������cbitmap����
  //m_bmp.GetBitmap(&bm);
  //CWnd * pWnd = GetDlgItem(IDC_PREVIEW);
  //CDC* pDC = pWnd->GetDC();
  //CRect rect;
  //GetDlgItem(IDC_PREVIEW)->GetClientRect(&rect);
  //CDC memDC;        //����һ���豸
  //CClientDC dc1(this);      //��ȡ�ͻ�
  //memDC.CreateCompatibleDC( &dc1 );
  //memDC.SelectObject( m_bmp );  //Ϊ�豸ѡ�����

  //mDCMem->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

  //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
  //memDC.DeleteDC();
  //m_bmp.DeleteObject();	
  //DeleteObject(hBitmap1);//�ǵ�ɾ��	
  //
  //CPen PenRed;
  //CGdiObject *o = NULL;
  //PenRed.CreatePen(PS_SOLID,1,RGB(255,0,0));//����һ֧��ɫ�Ļ���.
  ////pWnd = GetDlgItem(IDC_image1); //IDC_STATIC��picture control��ID.����ǵõ�picture�ľ��.
  ////pDC = pWnd->GetDC();//Ȼ��õ��豸����.
  ////RECT rect;//����һ��rect
  ////pWnd->GetClientRect(&rect);//����picture�ؼ���������rect����.
  //mDCMem->SelectObject(PenRed);//ѡ�л���.

  ///*m_param->camera.nPToL=2;//ֱ�ߵ��
  //m_param->camera.nDefectPToL=10;//ȱ�ݵ��
  //m_param->camera.nThreshold=30;//��ֵ*/
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
  //csTmp.Format(_T("%.2f��"),dTmpRT);
  //SetDlgItemText(IDC_SELECT_RESULT,csTmp);
  //	//
  //pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), mDCMem, 0, 0, SRCCOPY);
  //pWnd->ReleaseDC(pDC);
  //m_param->camera.bClbPos=1;
}


void CCamera::OnBnClickedBtnReconnect()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  //m_ch = m_ksj.GetKsj(KSJ_UD205M_SGYY);
  //if (0 > m_ch)
  //  AfxMessageBox(_T("KSJ_UD205M_SGYY �����ʧ��	,�����Ƿ����Ӹ����"));
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
//		MessageBox(_T("�����Ѿ�����!"));
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
//	//	NET_SetLogToFile(1,);//SDK��־����δʵ��
//
//
//	NET_SetConnectTime(3000,3);
//	NET_SetReconnect(3000,TRUE);
//	glCamID = NET_Login(sAddressIp, (WORD)dwPort, sUserName, sPassword, NULL);
//
//
//
//	if (glCamID < 0)
//		AfxMessageBox(_T("����IIIC���ʧ�ܣ�"));
//	else if(gclsIiic.OpenRtsp()!=1)
//		AfxMessageBox(_T("RTSP FAIL"));
//	/*int connectCNT= 0;
//	while(glCamID < 0 )
//	{
//		glCamID = NET_Login(sAddressIp, (WORD)dwPort, sUserName, sPassword, NULL);
//		connectCNT ++;
//		if (connectCNT >10000)
//		{
//			AfxMessageBox(_T("����IIIC���ʧ�ܣ�"));
//			OnClose();
//			break;
//		}
//	}*/
//
//
//}


void CCamera::OnBnClickedBtnParmsend()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������

  ////m_clked_pos_x = X;
  ////m_clked_pos_y = Y;m_par
  //if (0 == g_dlgDevice->ParamMove(m_par.x, m_par.y, m_par.deg, m_par.pn))
  //{
  //  AfxMessageBox(_T("���ͳɹ�"));
  //}
  //else
  //{
  //  AfxMessageBox(_T("status is not ready"));
  //}

}


void CCamera::OnBnClickedBtnAuto()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������

  //CString str;
  //GetDlgItemText(IDC_BTN_AUTO, str);
  //((CButton*)GetDlgItem(IDC_BTN_AUTO))->EnableWindow(FALSE);
  //if ("�Զ�" == str)
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
  //  SetDlgItemText(IDC_BTN_AUTO, _T("�Զ���"));
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
  //    m_ksj.StopPreview(m_ch);  //ֹͣԤ��
  //  }
  //  gstuRun.unStatRun = RUN_STAT_STOP;
  //  SetDlgItemText(IDC_BTN_AUTO, _T("�Զ�"));

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
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("��Ƭ����"), _T("��С��"), strValue, g_ini_path);
  m_rcg.stuRef.SAg = nValue;
}


void CCamera::OnEnChangeEdtSquMaxdeg()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("��Ƭ����"), _T("����"), strValue, g_ini_path);
  m_rcg.stuRef.BAg = nValue;
}


void CCamera::OnEnChangeEdtPnMindeg()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("�������"), _T("��С��"), strValue, g_ini_path);
  m_rcg.stuRef.PN_sag = nValue;
}


void CCamera::OnEnChangeEdtPnMaxdeg()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("�������"), _T("����"), strValue, g_ini_path);
  m_rcg.stuRef.PN_bag = nValue;
}


void CCamera::OnEnChangeEdtThreshold2()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ʶ�����"), _T("��ֵ"), strValue, g_ini_path);
  m_param->camera.nThreshold = nValue;
}


void CCamera::OnBnClickedBtnClbpos2test()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  int x = GetDlgItemInt(IDC_EDT_CLBPOS2X);
  int y = GetDlgItemInt(IDC_EDT_CLBPOS2Y);
  m_io->MotoRunNoWait(MOTOR_A, x);
  m_io->MotoRunNoWait(MOTOR_B, y);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}


void CCamera::OnEnChangeEdtClbpos1x()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ȡƬУ׼��"), _T("1X"), strValue, g_ini_path);
  m_param->camera.nClbPosX[0] = nValue;

}



void CCamera::OnEnChangeEdtClbpos2x()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ȡƬУ׼��"), _T("2X"), strValue, g_ini_path);
  m_param->camera.nClbPosX[1] = nValue;
}


void CCamera::OnEnChangeEdtClbpos3x()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ȡƬУ׼��"), _T("3X"), strValue, g_ini_path);
  m_param->camera.nClbPosX[2] = nValue;
}


void CCamera::OnEnChangeEdtClbpos1y()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ȡƬУ׼��"), _T("1Y"), strValue, g_ini_path);
  m_param->camera.nClbPosY[0] = nValue;
}

void CCamera::OnEnChangeEdtClbpos2y()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ȡƬУ׼��"), _T("2Y"), strValue, g_ini_path);
  m_param->camera.nClbPosY[1] = nValue;
}


void CCamera::OnEnChangeEdtClbpos3y()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("ȡƬУ׼��"), _T("3Y"), strValue, g_ini_path);
  m_param->camera.nClbPosY[2] = nValue;
}


void CCamera::OnEnChangeEdtTransferx()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("��ת��"), _T("X"), strValue, g_ini_path);
  m_param->camera.transferx = nValue;
}


void CCamera::OnEnChangeEdtTransfery()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
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
    AfxMessageBox(_T("�������ļ�"));
    return;
  }

  strValue.Format(_T("%d"), nValue);
  WritePrivateProfileString(_T("��ת��"), _T("Y"), strValue, g_ini_path);
  m_param->camera.transfery = nValue;
}


void CCamera::OnBnClickedBtnClbpos1test()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  int x = GetDlgItemInt(IDC_EDT_CLBPOS1X);
  int y = GetDlgItemInt(IDC_EDT_CLBPOS1Y);
  m_io->MotoRunNoWait(MOTOR_A, x);
  m_io->MotoRunNoWait(MOTOR_B, y);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}



void CCamera::OnBnClickedBtnClbpos3test()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  int x = GetDlgItemInt(IDC_EDT_CLBPOS3X);
  int y = GetDlgItemInt(IDC_EDT_CLBPOS3Y);
  m_io->MotoRunNoWait(MOTOR_A, x);
  m_io->MotoRunNoWait(MOTOR_B, y);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}


void CCamera::OnBnClickedBtnTransfertest()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  int x = GetDlgItemInt(IDC_EDT_TRANSFERX);
  int y = GetDlgItemInt(IDC_EDT_TRANSFERY);
  m_io->MotoRunNoWait(MOTOR_A, x);
  m_io->MotoRunNoWait(MOTOR_B, y);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}


void CCamera::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
  if(-1 == m_cam.snap())
	  m_cam.init(GetDlgItem(IDC_PREVIEW)->m_hWnd);
  CDialogEx::OnTimer(nIDEvent);
}


void CCamera::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_cam.unInit();// ��ִ�У�����������˳�����
}
void CCamera::VideoOnOff(BOOL on)
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
  m_io->MotoZeroNoWait(MOTOR_A);
  m_io->MotoZeroNoWait(MOTOR_B);
  while (!m_io->CheckMotoEnd(MOTOR_A)) { Sleep(100); };
  while (!m_io->CheckMotoEnd(MOTOR_B)) { Sleep(100); };
}
