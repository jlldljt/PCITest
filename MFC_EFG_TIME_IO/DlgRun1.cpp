// DlgRun1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgRun1.h"
#include "MainFrm.h"
#include "EfgAlg.h"

CWinThread * gTrdMainRun = NULL;
CWinThread * gTrdXRun = NULL;
CWinThread * gTrdYRun = NULL;
CWinThread * gTrdTurntableRun = NULL;
CWinThread * gTrdStaticMeasure = NULL;

UINT Thread_YRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    while (PAUSE == pdlg->m_run.state.mainrun)
    {
      Sleep(1);
    }

    switch (pdlg->m_run.state.y) {
    case START://����
      if (PAUSE == pdlg->m_run.state.turntable)//���yֹͣ������y
      {
        pdlg->m_io->MotoZero(MOTOR_Y);//����
        pdlg->m_io->WriteDo(Y_NOZZLE, IO_ON);//������
        Sleep(pdlg->m_param->user_config.time.y_on);//��ʱ
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//�ƶ����ȴ�λ��
        pdlg->m_run.state.turntable = END;
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_result->measure.cur_pos_step);//�ƶ�����λ

        pdlg->m_run.state.y = END;
        pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//�ر�����
        Sleep(pdlg->m_param->user_config.time.y_off);//��ʱ
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//�ƶ����ȴ�λ��
      }
      break;
    case STOP:
      pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//�ƶ����ȴ�λ��
      pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//�ر�����
      pdlg->m_run.state.y = END;
      return 0;
    default:
      Sleep(1);
      break;
      //gTrdYRun->SuspendThread();
    }
  }

  return 0;
}

UINT Thread_TurntableRun(LPVOID pParam)
{
  if (0 == SetThreadAffinityMask(GetCurrentThread(), 1))
    SetThreadAffinityMask(GetCurrentThread(), 1);

  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    switch (pdlg->m_run.state.turntable) {
    case START://����
      if (GetMainFrame()->StartMeasure(pdlg->m_param->laser.out3, pdlg->m_param->laser.out6))
        pdlg->m_run.state.turntable = RUNNING;
      break;
    case RUNNING:
      if (GetMainFrame()->CheckMeasure())
        pdlg->m_run.state.turntable = WAIT;
      break;
    case WAIT:
      if (END == pdlg->m_run.state.y)//���yֹͣ������y
      {
        //GetMainFrame()->m_diIntCounterSnap.CalcResult();//������ɣ��ȴ�y��������ϣ����м���
        pdlg->CalcResult();
        //pdlg->m_io->GetCurOffPos();//�õ���ǰ������λ��
        //pdlg->ShowResult(1);

        pdlg->m_run.state.turntable = PAUSE;
        pdlg->m_run.state.y = START;
        //gTrdYRun->ResumeThread();
      }
      break;
    case PAUSE:
      break;
    case STOP:
      pdlg->m_run.state.turntable = END;
      return 0;
    default:
      Sleep(1);
      break;
      //gTrdTurntableRun->SuspendThread();
    }
  }

  return 0;
}

UINT Thread_XRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    while (PAUSE == pdlg->m_run.state.mainrun)
    {
      Sleep(1);
    }

    switch (pdlg->m_run.state.x)
    {
    case START://����
      if (pdlg->m_io->ReadDi(READY)) {//�������
        pdlg->m_io->MotoZero(MOTOR_X);//����
        pdlg->m_io->WriteDo(X_NOZZLE, IO_ON);//������
        Sleep(pdlg->m_param->user_config.time.x_on);//��ʱ
        pdlg->m_io->MotoRun(MOTOR_X, pdlg->m_param->user_config.pos.x_wait);//�ƶ����ȴ�λ��
        pdlg->m_run.state.x = WAIT;
      }
      break;
    case WAIT://�ȴ�����
      if (END == pdlg->m_run.state.turntable)
      {
        pdlg->m_io->MotoRun(MOTOR_X, pdlg->m_param->user_config.pos.x_off);//�ƶ�������֧��
        pdlg->m_io->WriteDo(X_NOZZLE, IO_OFF);//�ر�����
        Sleep(pdlg->m_param->user_config.time.x_off);//��ʱ
        pdlg->m_io->MotoRunNoWait(MOTOR_X, pdlg->m_param->user_config.pos.x_wait);//�ƶ����ȴ�λ��

        pdlg->m_run.state.x = START;
        pdlg->m_run.state.turntable = START;

        //gTrdTurntableRun->ResumeThread();
      }
      else
        Sleep(1);
      break;
    case STOP:
      pdlg->m_io->MotoZero(MOTOR_X);//����
      pdlg->m_io->WriteDo(X_NOZZLE, IO_OFF);//�ر�����
      pdlg->m_run.state.x = END;
      return 0;
    default:
      return 0;
    }
  }

  return 0;
}

UINT Thread_StaticMeasure(LPVOID pParam)
{
  if (0 == SetThreadAffinityMask(GetCurrentThread(), 1))
    SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    switch (pdlg->m_run.state.staticmeasure)
    {
    case START:
      if (END == pdlg->m_run.state.turntable)
      {
        pdlg->m_run.state.turntable = START;
        //gTrdTurntableRun->ResumeThread();
      }

      else if (START == pdlg->m_run.state.y)//���yֹͣ������y
      {
        pdlg->m_run.state.turntable = END;
        Sleep(pdlg->m_param->user_config.time.y_off);//��ʱ
        pdlg->m_run.state.y = END;
      }
      else
        Sleep(1);
      break;
    case STOP:
      pdlg->m_run.state.staticmeasure = END;
      return 0;
    default:
      return 0;

    }

  }
  return 0;
}

UINT Thread_MainRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 1);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    switch (pdlg->m_run.state.mainrun) {
    case START://����
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("��������"));
      while (END != pdlg->m_run.state.x&&END != pdlg->m_run.state.y&&END != pdlg->m_run.state.turntable);
      pdlg->m_io->InitEfgIO();
      pdlg->m_run.state.x = START;
      pdlg->m_run.state.y = END;
      pdlg->m_run.state.turntable = END;

      gTrdXRun = AfxBeginThread(Thread_XRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      gTrdXRun->m_bAutoDelete = TRUE;
      gTrdXRun->ResumeThread();

      gTrdYRun = AfxBeginThread(Thread_YRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      gTrdYRun->m_bAutoDelete = TRUE;
      gTrdYRun->ResumeThread();

      gTrdTurntableRun = AfxBeginThread(Thread_TurntableRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      gTrdTurntableRun->m_bAutoDelete = TRUE;
      gTrdTurntableRun->ResumeThread();

      pdlg->m_run.state.mainrun = RUNNING;
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("��������"));
      break;
    case PAUSE:
      break;
    case STOP:
      pdlg->m_run.state.x = STOP;
      pdlg->m_run.state.y = STOP;
      pdlg->m_run.state.turntable = STOP;
      //gTrdXRun->ResumeThread();
      //gTrdYRun->ResumeThread();
      //gTrdTurntableRun->ResumeThread();
      //gTrdStaticMeasure->ResumeThread();
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T(""));
      return 0;
    case RUNNING:
      gTrdMainRun->SuspendThread();
      break;
    case STATIC_MEASURE_START:
      //pdlg->m_io->InitEfgIO();
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("������̬����"));
      while (END != pdlg->m_run.state.staticmeasure&&END != pdlg->m_run.state.turntable);
 
	  memset(&pdlg->m_io->m_resultParam,0,sizeof(pdlg->m_io->m_resultParam));
	  pdlg->m_io->m_resultParam.measure.min_pluse_num = 100000;
      pdlg->m_run.state.staticmeasure = START;
      pdlg->m_run.state.y = END;
      pdlg->m_run.state.turntable = END;

      gTrdStaticMeasure = AfxBeginThread(Thread_StaticMeasure, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      gTrdStaticMeasure->m_bAutoDelete = TRUE;
      gTrdStaticMeasure->ResumeThread();

      gTrdTurntableRun = AfxBeginThread(Thread_TurntableRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      gTrdTurntableRun->m_bAutoDelete = TRUE;
      gTrdTurntableRun->ResumeThread();


      pdlg->m_run.state.mainrun = RUNNING;
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T("���ھ�̬����"));
      break;
    case STATIC_MEASURE_STOP:
      //return 0;
      pdlg->m_run.state.staticmeasure = STOP;
      pdlg->m_run.state.turntable = STOP;
      //gTrdTurntableRun->ResumeThread();
      pdlg->SetDlgItemText(IDC_STATIC_MESSAGE, _T(""));
      return 0;
    default:
      return 0;
    }
  }

  return 0;
}

// CDlgRun1

IMPLEMENT_DYNCREATE(CDlgRun1, CFormView)

CDlgRun1::CDlgRun1()
  : CFormView(CDlgRun1::IDD)
{
  memset(&m_run, 0, sizeof(m_run));
  m_run.state.x = END;
  m_run.state.y = END;
  m_run.state.turntable = END;
  m_run.state.staticmeasure = END;
}

CDlgRun1::~CDlgRun1()
{
  m_laserdc->SelectObject(m_laseroldbm);
  m_laserbm->DeleteObject();
  m_laserdc->DeleteDC();

  m_xraydc->SelectObject(m_xrayoldbm);
  m_xraybm->DeleteObject();
  m_xraydc->DeleteDC();

}

void CDlgRun1::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID_SORT, m_gridSort);
  DDX_Control(pDX, IDC_BTN_CTRL, m_btn_ctrl);
  DDX_Control(pDX, IDC_TAB_PREVIEW, m_tab_preview);
  DDX_Control(pDX, IDC_STATIC_RESULT, m_static_result);
  DDX_Control(pDX, IDC_STATIC_SORT, m_static_sort);
  DDX_Control(pDX, IDC_STATIC_MESSAGE, m_static_message);
}

BEGIN_MESSAGE_MAP(CDlgRun1, CFormView)
  ON_WM_CTLCOLOR()
  //  ON_BN_CLICKED(IDC_SORT_SET, &CDlgRun1::OnBnClickedSortSet)
  ON_BN_CLICKED(IDC_EDT_RUN_SET, &CDlgRun1::OnBnClickedEdtRunSet)
  ON_CBN_SELCHANGE(IDC_CMB_PRIMARY, &CDlgRun1::OnCbnSelchangeCmbPrimary)
  ON_EN_CHANGE(IDC_EDT_CARD, &CDlgRun1::OnEnChangeEdtCard)
  ON_CBN_SELCHANGE(IDC_CMB_SECONDARY, &CDlgRun1::OnCbnSelchangeCmbSecondary)
  ON_EN_CHANGE(IDC_EDT_CENTER_DEGREE, &CDlgRun1::OnEnChangeEdtCenterDegree)
  ON_EN_CHANGE(IDC_EDT_STEP_DEGREE, &CDlgRun1::OnEnChangeEdtStepDegree)
  ON_EN_CHANGE(IDC_EDT_MIN_DEGREE, &CDlgRun1::OnEnChangeEdtMinDegree)
  ON_EN_CHANGE(IDC_EDT_MAX_DEGREE, &CDlgRun1::OnEnChangeEdtMaxDegree)
  ON_EN_CHANGE(IDC_EDT_EQU_PHI, &CDlgRun1::OnEnChangeEdtEquPhi)
  ON_EN_CHANGE(IDC_EDT_EQU_FACTOR, &CDlgRun1::OnEnChangeEdtEquFactor)
  ON_BN_CLICKED(IDC_BTN_CTRL, &CDlgRun1::OnBnClickedBtnCtrl)
  ON_BN_CLICKED(IDC_CHK_PAUSE, &CDlgRun1::OnBnClickedChkPause)
  ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PREVIEW, &CDlgRun1::OnTcnSelchangeTabPreview)
  ON_BN_CLICKED(IDC_CHK_STATICMEASURE, &CDlgRun1::OnBnClickedChkStaticmeasure)
END_MESSAGE_MAP()


// CDlgRun1 ���

#ifdef _DEBUG
void CDlgRun1::AssertValid() const
{
  CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgRun1::Dump(CDumpContext& dc) const
{
  CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgRun1 ��Ϣ�������


HBRUSH CDlgRun1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  �ڴ˸��� DC ���κ�����
  if (pWnd == this) {
    return m_brush;
  }
  else if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN /*|| nCtlColor == CTLCOLOR_MAX*/)
  {
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));
    if (pWnd->GetDlgCtrlID() == IDC_STATIC || pWnd->GetDlgCtrlID() == IDC_STATIC_SORT || pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT|| pWnd->GetDlgCtrlID() == IDC_STATIC_MESSAGE)
      return (HBRUSH)::GetStockObject(NULL_BRUSH);
  }
  // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

  return hbr;
}


void CDlgRun1::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  m_param = &GetMainFrame()->m_efgio.m_configParam;
  m_io = &GetMainFrame()->m_efgio;
  m_result = &GetMainFrame()->m_efgio.m_resultParam;

  // TODO: �ڴ����ר�ô����/����û���
  //combobox�������tab�ؼ�֮��
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("��"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("����"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("����"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("��Ч��"));
  //combobox�������tab�ؼ�֮��
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("��"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("����"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("����"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("��Ч��"));

  m_btn_ctrl.LoadBitmaps(IDB_START);
  m_btn_ctrl.SizeToContent();

  //////////////tab
                   //tab�ؼ���ʼ��
  CRect rect;
  m_tab_preview.InsertItem(0, _T("���ͼ"));
  m_tab_preview.InsertItem(1, _T("ҡ������"));
  m_tab_preview.SetCurSel(0);

  m_tab_preview.GetClientRect(&rect);
  m_preview_rect = rect;

  rect.left += 1;
  rect.right -= 1;
  rect.top += 22;
  rect.bottom -= 1;

  m_preview_rect.right -= 2;
  m_preview_rect.bottom -= 23;

  CDC *pDC = m_tab_preview.GetDC();
  m_laserdc = new CDC;
  m_laserbm = new CBitmap;
  m_laserdc->CreateCompatibleDC(pDC);
  m_laserbm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  m_laseroldbm = m_laserdc->SelectObject(m_laserbm);
  m_laserdc->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(m_laserdc->m_hDC, 0, 0, NULL);


  m_xraydc = new CDC;
  m_xraybm = new CBitmap;
  m_xraydc->CreateCompatibleDC(pDC);
  m_xraybm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
  m_xrayoldbm = m_xraydc->SelectObject(m_xraybm);
  m_xraydc->SetStretchBltMode(HALFTONE);
  SetBrushOrgEx(m_xraydc->m_hDC, 0, 0, NULL);

  ReleaseDC(pDC);

  // ������ͼ
  CBitmap bmp, sbmp;
  HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, _T("BK1.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
  if (bitmap)
    bmp.Attach(bitmap);
  else
    bmp.LoadBitmap(IDB_BK1);   //IDB_BITMAP1��ͼƬ��ԴID
//  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);

  bmp.DeleteObject();
  //��λ������
  m_font.CreatePointFont(800, _T("����"), NULL);
  GetDlgItem(IDC_STATIC_SORT)->SetFont(&m_font);

  //��ʼ���ı���
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->SetCurSel(m_param->user_config.measure.primary.type);
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->SetCurSel(m_param->user_config.measure.secondary.type);
  SetDlgItemText(IDC_EDT_CARD, (m_param->user_config.measure.card));
  SetDlgItemInt(IDC_EDT_CENTER_DEGREE, (UINT)(m_param->user_config.measure.primary.center_degree));
  SetDlgItemInt(IDC_EDT_STEP_DEGREE, (UINT)(m_param->user_config.measure.primary.step_degree));
  SetDlgItemInt(IDC_EDT_MIN_DEGREE, (UINT)(m_param->user_config.measure.secondary.min_degree));
  SetDlgItemInt(IDC_EDT_MAX_DEGREE, (UINT)(m_param->user_config.measure.secondary.max_degree));
  SetDlgItemInt(IDC_EDT_EQU_PHI, (UINT)(m_param->user_config.measure.equivalent_angle.phi));
  SetDlgItemInt(IDC_EDT_EQU_FACTOR, (UINT)(m_param->user_config.measure.equivalent_angle.factor));

  InitGrid();
  UpdateGridWithRecalc();
  //test
  
  m_file.Open(_T("test.txt"),CFile::modeCreate | CFile::modeWrite | CFile::shareDenyRead);
  m_file.Seek(0,CFile::end);
}


//void CDlgRun1::OnBnClickedSortSet()
//{
//  // TODO: �ڴ���ӿؼ�֪ͨ����������
//}


void CDlgRun1::OnBnClickedEdtRunSet()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������

  GetMainFrame()->EfgParamSave();

  UpdateGridWithRecalc();
}


void CDlgRun1::OnCbnSelchangeCmbPrimary()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  m_param->user_config.measure.primary.type = ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->GetCurSel();
}


void CDlgRun1::OnEnChangeEdtCard()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  GetDlgItemText(IDC_EDT_CARD, m_param->user_config.measure.card, MAX_CARD_LEN);
}


void CDlgRun1::OnCbnSelchangeCmbSecondary()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  m_param->user_config.measure.secondary.type = ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->GetCurSel();
}


void CDlgRun1::OnEnChangeEdtCenterDegree()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  m_param->user_config.measure.primary.center_degree = GetDlgItemInt(IDC_EDT_CENTER_DEGREE, 0);
}


void CDlgRun1::OnEnChangeEdtStepDegree()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  m_param->user_config.measure.primary.step_degree = GetDlgItemInt(IDC_EDT_STEP_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtMinDegree()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  m_param->user_config.measure.secondary.min_degree = GetDlgItemInt(IDC_EDT_MIN_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtMaxDegree()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  m_param->user_config.measure.secondary.max_degree = GetDlgItemInt(IDC_EDT_MAX_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtEquPhi()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  m_param->user_config.measure.equivalent_angle.phi = GetDlgItemInt(IDC_EDT_EQU_PHI, 0);

}


void CDlgRun1::OnEnChangeEdtEquFactor()
{
  // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
  // ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
  // ���������� CRichEditCtrl().SetEventMask()��
  // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

  // TODO:  �ڴ���ӿؼ�֪ͨ����������
  m_param->user_config.measure.equivalent_angle.factor = GetDlgItemInt(IDC_EDT_EQU_FACTOR, 0);

}

void CDlgRun1::InitGrid(void)
{
  m_gridSort.SetEditable(false);
  m_gridSort.SetTextBkColor(RGB(204, 232, 207));//��ɫ����
  m_gridSort.SetRowCount(2); //��ʼΪ2��

  for (int i = 1; i < 2; i++)
  {
    m_gridSort.SetRowHeight(i, 25);
  }
  m_gridSort.SetColumnCount(3); //��ʼ��Ϊ3��
  m_gridSort.SetFixedRowCount(1); //��ͷΪ1��
  m_gridSort.SetFixedColumnCount(0); //��ͷΪ1��
  m_gridSort.SetRowHeight(0, 30); //���ø��и�         
                                     //m_gridSort.SetColumnWidth(0, 320); //����0�п� 
  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("��λ"));
  Item.row = 0;
  Item.col = 0;
  m_gridSort.SetItem(&Item);
  Item.strText.Format(_T("�Ƕ�"));
  Item.row = 0;
  Item.col = 1;
  m_gridSort.SetItem(&Item);
  Item.strText.Format(_T("Ƭ��"));
  Item.row = 0;
  Item.col = 2;
  m_gridSort.SetItem(&Item);

  CRect cRect;
  GetDlgItem(IDC_GRID_SORT)->GetClientRect(&cRect);
  m_gridSort.SetColumnWidth(0, cRect.Width() / 6 * 1); //�����п� 
  m_gridSort.SetColumnWidth(1, cRect.Width() / 6 * 3); //
  m_gridSort.SetColumnWidth(2, cRect.Width() / 6 * 2); //
  m_gridSort.ExpandLastColumn();
  m_gridSort.SetColumnResize(FALSE);
  m_gridSort.SetRowResize(FALSE);
  //m_gridSort.SetSingleRowSelection(TRUE);
  m_gridSort.EnableSelection(FALSE);
  //SetAutoSizeStyle
  //m_gridSort.EnableScrollBar();
  // m_gridSort.EnableScrollBarCtrl();
  m_gridSort.ShowBar(SB_HORZ, FALSE);
  //m_gridSort.SetRedraw(TRUE);
}

void CDlgRun1::UpdateGrid()
{
  //m_gridSort.SetRedraw(FALSE);
  int min = 1;
  int max = m_gridSort.GetRowCount();
  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
  int sort_type = m_param->user_config.type;
  ASSERT(sort_type < MAX_TYPE_NUM);
  int sort_num = m_param->user_config.type_sort_num[sort_type];


  for (int i = max; i >= min; i--)
  {
    m_gridSort.DeleteRow(i);
  }
  int line = 1;
  //��ӷֵ���λ
  CString str;
  for (int i = 0; i < sort_num; i++, line++)
  {
    str.Format(_T("%d"), line);
    m_gridSort.InsertRow(str, -1);
    str.Format(_T("%02d��%02d��%02d��"), S_DEG(m_result->degree.sort_sec[i])
      , S_MIN(m_result->degree.sort_sec[i])
      , S_SEC(m_result->degree.sort_sec[i]));
    m_gridSort.SetItemText(line, 1, str);
    str.Format(_T("%d"), m_result->num.y_off[i]);
    m_gridSort.SetItemText(line, 2, str);
  }
  //��Ӳ��ϸ�λ
  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1
    , m_io->GetMeasureType(m_param->user_config.measure.primary.type) + _T("-"));
  str.Format(_T("%d"), m_result->num.primary_n);
  m_gridSort.SetItemText(line++, 2, str);


  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1
    , m_io->GetMeasureType(m_param->user_config.measure.primary.type) + _T("+"));
  str.Format(_T("%d"), m_result->num.primary_p);
  m_gridSort.SetItemText(line++, 2, str);

  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1
    , m_io->GetMeasureType(m_param->user_config.measure.secondary.type) + _T("-"));
  str.Format(_T("%d"), m_result->num.secondary_n);
  m_gridSort.SetItemText(line++, 2, str);

  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1
    , m_io->GetMeasureType(m_param->user_config.measure.secondary.type) + _T("+"));
  str.Format(_T("%d"), m_result->num.secondary_p);
  m_gridSort.SetItemText(line++, 2, str);

  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1, _T("δ���"));
  str.Format(_T("%d"), m_result->num.not_detected);
  m_gridSort.SetItemText(line++, 2, str);

  m_gridSort.ExpandLastColumn();
  m_gridSort.Refresh();
  //m_gridSort.SetRedraw(TRUE);
  //m_gridSort.Invalidate();
}
//���¼����������ֵ
void CDlgRun1::UpdateGridWithRecalc()
{
  EfgAlg alg;
  alg.GetAllSortDegree(m_result->degree.sort_sec
    , USER_TO_SEC(m_param->user_config.measure.primary.center_degree)
    , USER_TO_SEC(m_param->user_config.measure.primary.step_degree)
    , m_param->user_config.type_sort_num[m_param->user_config.type]);

  UpdateGrid();
}


void CDlgRun1::OnBnClickedBtnCtrl()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CString str;
  GetDlgItemText(IDC_BTN_CTRL, str);
  if ("��ʼ" == str) {
    //SetTimer(0, 100, NULL);
    m_run.state.mainrun = START;

    gTrdMainRun = AfxBeginThread(Thread_MainRun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    gTrdMainRun->m_bAutoDelete = TRUE;
    gTrdMainRun->ResumeThread();


    m_btn_ctrl.LoadBitmaps(IDB_STOP);
    SetDlgItemText(IDC_BTN_CTRL, _T("ֹͣ"));
  }
  else {
    //KillTimer(0);
    m_run.state.mainrun = STOP;
    gTrdMainRun->ResumeThread();

    m_btn_ctrl.LoadBitmaps(IDB_START);
    SetDlgItemText(IDC_BTN_CTRL, _T("��ʼ"));
  }

}


void CDlgRun1::OnBnClickedChkPause()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_PAUSE);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
  }
  else
  {

  }
}

//sw 1ˢ�£�sw����
void CDlgRun1::ShowResult(BOOL sw)
{
  CString str_deg = _T("");
  int str_pos = 0;
  if (sw)
  {
    //str_deg.Format(_T("��  ��\t�� ǰ ֵ\tƽ �� ֵ\tɢ    ��\t����������%d\r\n����\t%07.3lf\t%07.3lf\t%07.3lf\r\n����\t%07.3lf\t%3.3lf\t%3.3lf\r\n��Ч��\t%3.3f\t%3.3f\t%3.3f\r\n����0\t%3.3f\t%3.3f\t%3.3f\r\n����0\t%3.3f\t%3.3f\t%3.3f\r\n"),

    str_deg.Format(_T("��  ��\t��ǰֵ\tƽ��ֵ\tɢ  ��\t����������%d\r\n����\t%06d\t%06d\t%06.3f\r\n����\t%06d\t%06d\t%06.3f\r\n��Ч��\t%06d\t%06d\t%06.3f\r\n����0\t%06d\t%06d\t%06.3f\r\n����0\t%06d\t%06d\t%06.3f\r\n"),
      m_io->m_resultParam.measure.num,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_theta1), DEG_TO_USER(m_io->m_resultParam.measure.avg_theta1), (m_io->m_resultParam.measure.std_theta1)*3600,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_phi1), DEG_TO_USER(m_io->m_resultParam.measure.avg_phi1), (m_io->m_resultParam.measure.std_phi1)*3600,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_equ), DEG_TO_USER(m_io->m_resultParam.measure.avg_equ), (m_io->m_resultParam.measure.std_equ)*3600,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_theta0), DEG_TO_USER(m_io->m_resultParam.measure.avg_theta0), (m_io->m_resultParam.measure.std_theta0)*3600,
      DEG_TO_USER(m_io->m_resultParam.measure.cur_phi0), DEG_TO_USER(m_io->m_resultParam.measure.avg_phi0), (m_io->m_resultParam.measure.std_phi0)*3600
    );

    str_pos = m_io->m_resultParam.measure.cur_pos;
  }
  else
  {
    m_io->ClearMeasureResult();
  }
  GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(str_deg);
  /////////////////////////////////////
   str_deg.Format(_T("��  ��\t��ǰֵ\tƽ��ֵ\tɢ  ��\r\nD1\t%06d\t%06d\t%06d\r\nD2\t%06d\t%06d\t%06d\r\nDM\t%06d\t%06d\t%06d\r\nR1\t%06d\t%06d\t%06d\r\n������\t%.0f\t%.1f\t%.1f\r\nug\t%06d\t%06d\t%06d\r\nA\t%.3f\t%.3f\t%.3f\r\nw\t%.3f\t%.3f\t%.3f\r\nt\t%.3f\t%.3f\t%.3f\r\nk\t%.3f\t%.3f\t%.3f\r\n"),
      DEG_TO_USER(m_io->m_resultParam.measure.D1), DEG_TO_USER(m_io->m_resultParam.measure.avg_D1), DEG_TO_USER(m_io->m_resultParam.measure.std_D1),
      DEG_TO_USER(m_io->m_resultParam.measure.D2), DEG_TO_USER(m_io->m_resultParam.measure.avg_D2), DEG_TO_USER(m_io->m_resultParam.measure.std_D2),
      DEG_TO_USER(m_io->m_resultParam.measure.DM), DEG_TO_USER(m_io->m_resultParam.measure.avg_DM), DEG_TO_USER(m_io->m_resultParam.measure.std_DM),
      DEG_TO_USER(m_io->m_resultParam.measure.R1), DEG_TO_USER(m_io->m_resultParam.measure.avg_R1), DEG_TO_USER(m_io->m_resultParam.measure.std_R1),
      (m_io->m_resultParam.measure.pluse_num), (m_io->m_resultParam.measure.avg_pluse_num), (m_io->m_resultParam.measure.std_pluse_num),
      DEG_TO_USER(m_io->m_resultParam.measure.u_g), DEG_TO_USER(m_io->m_resultParam.measure.avg_u_g), DEG_TO_USER(m_io->m_resultParam.measure.std_u_g),
      (m_io->m_resultParam.measure.A), (m_io->m_resultParam.measure.avg_A), (m_io->m_resultParam.measure.std_A),
      (m_io->m_resultParam.measure.w), (m_io->m_resultParam.measure.avg_w), (m_io->m_resultParam.measure.std_w),
      (m_io->m_resultParam.measure.t), (m_io->m_resultParam.measure.avg_t), (m_io->m_resultParam.measure.std_t),
      (m_io->m_resultParam.measure.k), (m_io->m_resultParam.measure.avg_k), (m_io->m_resultParam.measure.std_k)
    );
  SetDlgItemText(IDC_SHOW_DEGREE, str_deg);


   str_deg.Format(_T("%06.3f\t%06.3f\t%06.3f\t%06.3f\r\n"),
      (m_io->m_resultParam.measure.D1), 
      (m_io->m_resultParam.measure.D2), 
      (m_io->m_resultParam.measure.DM), 
      (m_io->m_resultParam.measure.R1));
  m_file.Write(CStringA(str_deg).GetBuffer(),CStringA(str_deg).GetLength());
  CStringA(str_deg).ReleaseBuffer();

  //SetDlgItemText(IDC_STATIC_RESULT, str_deg);
   ///////////////

  OnTcnSelchangeTabPreview(NULL, NULL);
  //UpdateGrid();

  SetDlgItemInt(IDC_STATIC_SORT, str_pos + 1);
}


void CDlgRun1::OnTcnSelchangeTabPreview(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CRect tabRect;
  m_tab_preview.GetClientRect(&tabRect);
  tabRect.left += 1;
  tabRect.right -= 1;
  tabRect.top += 22;
  tabRect.bottom -= 1;
  CDC *pDC = m_tab_preview.GetDC();
  switch (m_tab_preview.GetCurSel())
  {
  case 0:
    pDC->BitBlt(tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), m_xraydc, 0, 0, SRCCOPY);
    break;
  case 1:
    pDC->BitBlt(tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), m_laserdc, 0, 0, SRCCOPY);

    break;
  }
  ReleaseDC(pDC);

  if (pResult)
    *pResult = 0;
}
//�õ��û����ս����ˢ�½���
int CDlgRun1::CalcResult()
{
  if (-1 == GetMainFrame()->m_diIntCounterSnap.LaserFit()) {
    return -1;
  }

  if (GetMainFrame()->m_viewBoard)
    GetMainFrame()->m_viewBoard->DrawToDC(m_laserdc, m_preview_rect);

  if (-1 == GetMainFrame()->m_diIntCounterSnap.XrayFit()) {
    return -1;
  }

  if (GetMainFrame()->m_viewBoard)
    GetMainFrame()->m_viewBoard->DrawToDC(m_xraydc, m_preview_rect);

  EfgAlg alg;

  struct tagSinParam sin_param = {
    m_io->m_resultParam.measure.A,
    m_io->m_resultParam.measure.w,
    m_io->m_resultParam.measure.t,
    m_io->m_resultParam.measure.k
  };
  //����������
  alg.CalcDegree1(
    atan(m_io->m_resultParam.measure.A * m_io->m_configParam.laser.factor_a / m_io->m_configParam.laser.factor_l) / DPI,
    m_io->m_resultParam.measure.t / DPI,
    m_io->m_resultParam.measure.R1,
    m_io->m_configParam.laser.offset,
    m_io->m_resultParam.measure.cur_theta0,
    m_io->m_resultParam.measure.cur_phi0,
    m_io->m_resultParam.measure.cur_theta1,
    m_io->m_resultParam.measure.cur_phi1
  );
  //�����Ч��
  alg.CalcEquAngle(
    m_io->m_resultParam.measure.cur_theta0,
    m_io->m_resultParam.measure.cur_phi0,
    USER_TO_DEG(m_io->m_configParam.user_config.measure.equivalent_angle.phi),
    m_io->m_configParam.user_config.measure.equivalent_angle.factor / 1000.0,
    m_io->m_resultParam.measure.cur_equ
  );

  // �ۼƲ��ֵ
  m_io->m_resultParam.measure.num++;

  // �ۼ�ƽ��ֵ��ɢ��
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_theta0,
    m_io->m_resultParam.measure.avg_theta0,
    m_io->m_resultParam.measure.std_theta0,
    m_io->m_resultParam.measure.std2_theta0
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_phi0,
    m_io->m_resultParam.measure.avg_phi0,
    m_io->m_resultParam.measure.std_phi0,
    m_io->m_resultParam.measure.std2_phi0
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_theta1,
    m_io->m_resultParam.measure.avg_theta1,
    m_io->m_resultParam.measure.std_theta1,
    m_io->m_resultParam.measure.std2_theta1
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_phi1,
    m_io->m_resultParam.measure.avg_phi1,
    m_io->m_resultParam.measure.std_phi1,
    m_io->m_resultParam.measure.std2_phi1
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.cur_equ,
    m_io->m_resultParam.measure.avg_equ,
    m_io->m_resultParam.measure.std_equ,
    m_io->m_resultParam.measure.std2_equ
  );
  //test
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.D1,
    m_io->m_resultParam.measure.avg_D1,
    m_io->m_resultParam.measure.std_D1,
    m_io->m_resultParam.measure.std2_D1
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.D2,
    m_io->m_resultParam.measure.avg_D2,
    m_io->m_resultParam.measure.std_D2,
    m_io->m_resultParam.measure.std2_D2
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.DM,
    m_io->m_resultParam.measure.avg_DM,
    m_io->m_resultParam.measure.std_DM,
    m_io->m_resultParam.measure.std2_DM
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.R1,
    m_io->m_resultParam.measure.avg_R1,
    m_io->m_resultParam.measure.std_R1,
    m_io->m_resultParam.measure.std2_R1
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.pluse_num,
    m_io->m_resultParam.measure.avg_pluse_num,
    m_io->m_resultParam.measure.std_pluse_num,
    m_io->m_resultParam.measure.std2_pluse_num
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.u_g,
    m_io->m_resultParam.measure.avg_u_g,
    m_io->m_resultParam.measure.std_u_g,
    m_io->m_resultParam.measure.std2_u_g
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.A,
    m_io->m_resultParam.measure.avg_A,
    m_io->m_resultParam.measure.std_A,
    m_io->m_resultParam.measure.std2_A
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.w,
    m_io->m_resultParam.measure.avg_w,
    m_io->m_resultParam.measure.std_w,
    m_io->m_resultParam.measure.std2_w
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.t,
    m_io->m_resultParam.measure.avg_t,
    m_io->m_resultParam.measure.std_t,
    m_io->m_resultParam.measure.std2_t
  );
  alg.SortAvgStd(
    m_io->m_resultParam.measure.num,
    m_io->m_resultParam.measure.k,
    m_io->m_resultParam.measure.avg_k,
    m_io->m_resultParam.measure.std_k,
    m_io->m_resultParam.measure.std2_k
  );
  //���㵵λ���ڱ�ĵط���������efgio

  int pos;
  int pos_num;
  pos = m_io->GetCurOffPos(pos_num);//�õ���ǰ������λ��
  ShowResult(1);//ˢ�½���

  CString str;
  str.Format(_T("%d"), pos_num);
  m_gridSort.SetItemText(pos + 1, 2, str);

  m_gridSort.Refresh();
  return 0;
}

void CDlgRun1::OnBnClickedChkStaticmeasure()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_STATICMEASURE);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_run.state.mainrun = STATIC_MEASURE_START;
    gTrdMainRun = AfxBeginThread(Thread_MainRun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    gTrdMainRun->m_bAutoDelete = TRUE;
    gTrdMainRun->ResumeThread();

  }
  else
  {
    m_run.state.mainrun = STATIC_MEASURE_STOP;
    gTrdMainRun->ResumeThread();
  }
}
