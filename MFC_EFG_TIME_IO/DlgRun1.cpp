// DlgRun1.cpp : 实现文件
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

UINT Thread_YRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 0);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    while (MAIN_PAUSE == pdlg->m_run.state.mainrun)
    {
      Sleep(1);
    }

    switch (pdlg->m_run.state.y) {
    case Y_START://拿料
      if (TURNTABLE_END == pdlg->m_run.state.turntable)//如果y停止，启动y
      {
        pdlg->m_io->MotoZero(MOTOR_Y);//回零
        pdlg->m_io->WriteDo(Y_NOZZLE, IO_ON);//打开吸嘴
        Sleep(pdlg->m_param->user_config.time.y_on);//延时
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_wait);//移动到等待位置
        pdlg->m_run.state.turntable = TURNTABLE_NULL;
        pdlg->m_io->MotoRun(MOTOR_Y, pdlg->m_param->user_config.pos.y_off[pdlg->m_param ->user_config.type][pdlg->m_result->measure.cur_pos]);//移动到档位
        pdlg->m_io->WriteDo(Y_NOZZLE, IO_OFF);//打开吸嘴
        Sleep(pdlg->m_param->user_config.time.y_off);//延时
        pdlg->m_run.state.y = Y_END;
      }
      break;
    case Y_STOP:
      break;
    default:
      gTrdYRun->SuspendThread();
    }
  }

  return 0;
}

UINT Thread_TurntableRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 0);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    switch (pdlg->m_run.state.turntable) {
    case TURNTABLE_MEASURE://测量
      if (GetMainFrame()->StartMeasure(pdlg->m_param->laser.out3, pdlg->m_param->laser.out6))
        pdlg->m_run.state.turntable = TURNTABLE_MEASURING;
      break;
    case TURNTABLE_MEASURING:
      if (GetMainFrame()->CheckMeasure())
        pdlg->m_run.state.turntable = TURNTABLE_END;
      break;
    case TURNTABLE_END:
      if (Y_END == pdlg->m_run.state.y)//如果y停止，启动y
      {
        pdlg->m_run.state.y = Y_START;
        gTrdYRun->ResumeThread();
      }
      break;
    default:
      gTrdTurntableRun->SuspendThread();
    }
  }

  return 0;
}

UINT Thread_XRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 0);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    while (MAIN_PAUSE == pdlg->m_run.state.mainrun)
    {
      Sleep(1);
    }

    switch (pdlg->m_run.state.x) 
    {
    case X_START://等料
      if (pdlg->m_io->ReadDi(READY)) {//如果有料
        pdlg->m_io->MotoZero(MOTOR_X);//回零
        pdlg->m_io->WriteDo(X_NOZZLE, IO_ON);//打开吸嘴
        Sleep(pdlg->m_param->user_config.time.x_on);//延时
        pdlg->m_io->MotoRun(MOTOR_X, pdlg->m_param->user_config.pos.x_wait);//移动到等待位置
        pdlg->m_run.state.x = X_WAIT;
      }
      break;
    case X_WAIT://等待上料
      if (TURNTABLE_NULL == pdlg->m_run.state.turntable)
      {
        pdlg->m_io->MotoRun(MOTOR_X, pdlg->m_param->user_config.pos.x_off);//移动到三孔支架
        pdlg->m_io->WriteDo(X_NOZZLE, IO_OFF);//关闭吸嘴
        Sleep(pdlg->m_param->user_config.time.x_off);//延时
        pdlg->m_io->MotoRunNoWait(MOTOR_X, pdlg->m_param->user_config.pos.x_wait);//移动到等待位置
        
        pdlg->m_run.state.x = X_START;
        pdlg->m_run.state.turntable = TURNTABLE_MEASURE;

        //gTrdTurntableRun = AfxBeginThread(Thread_TurntableRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
        //gTrdTurntableRun->m_bAutoDelete = TRUE;
        gTrdTurntableRun->ResumeThread();
      }
      break;
    case X_STOP:
      pdlg->m_io->MotoZero(MOTOR_X);//回零
      pdlg->m_io->WriteDo(X_NOZZLE, IO_OFF);//关闭吸嘴
      pdlg->m_run.state.x = X_END;
      break;
    default:
      return 0;
    }
  }

  return 0;
}

UINT Thread_MainRun(LPVOID pParam)
{
  SetThreadAffinityMask(GetCurrentThread(), 0);
  CDlgRun1* pdlg = (CDlgRun1*)pParam;

  while (1)
  {
    switch (pdlg->m_run.state.mainrun) {
    case MAIN_START://启动
      pdlg->m_io->InitEfgIO();
      pdlg->m_run.state.x = X_START;
      pdlg->m_run.state.y = Y_START;
      pdlg->m_run.state.turntable = TURNTABLE_NULL;

      gTrdXRun = AfxBeginThread(Thread_XRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      gTrdXRun->m_bAutoDelete = TRUE;
      gTrdXRun->ResumeThread();
      gTrdYRun = AfxBeginThread(Thread_YRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      gTrdYRun->m_bAutoDelete = TRUE;
      gTrdYRun->ResumeThread();

      gTrdTurntableRun = AfxBeginThread(Thread_TurntableRun, pParam, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      gTrdTurntableRun->m_bAutoDelete = TRUE;
      gTrdTurntableRun->ResumeThread();
      break;
    case MAIN_PAUSE:
      break;
    case MAIN_STOP:
      break;
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
}

CDlgRun1::~CDlgRun1()
{
}

void CDlgRun1::DoDataExchange(CDataExchange* pDX)
{
  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID_SORT, m_gridSort);
  DDX_Control(pDX, IDC_BTN_CTRL, m_btn_ctrl);
  DDX_Control(pDX, IDC_TAB_PREVIEW, m_tab_preview);
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
END_MESSAGE_MAP()


// CDlgRun1 诊断

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


// CDlgRun1 消息处理程序


HBRUSH CDlgRun1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  在此更改 DC 的任何特性
  if (pWnd == this) {
    return m_brush;
  }
  else if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN /*|| nCtlColor == CTLCOLOR_MAX*/)
  {
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 255));
    return (HBRUSH)::GetStockObject(NULL_BRUSH);
  }
  // TODO:  如果默认的不是所需画笔，则返回另一个画笔
 
  return hbr;
}


void CDlgRun1::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  m_param = &GetMainFrame()->m_efgio.m_configParam;
  m_io = &GetMainFrame()->m_efgio;
  m_result = &GetMainFrame()->m_efgio.m_resultParam;

  // TODO: 在此添加专用代码和/或调用基类
  //combobox必须放在tab控件之后
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("无"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("光轴"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("电轴"));
  ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->AddString(_T("等效角"));
  //combobox必须放在tab控件之后
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("无"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("光轴"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("电轴"));
  ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->AddString(_T("等效角"));

  m_btn_ctrl.LoadBitmaps(IDB_START);
  m_btn_ctrl.SizeToContent();


  CBitmap bmp, sbmp;
  HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, _T("BK1.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
  if (bitmap)
    bmp.Attach(bitmap);
  else
    bmp.LoadBitmap(IDB_BK1);   //IDB_BITMAP1是图片资源ID
  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);
  
  bmp.DeleteObject();
  //档位大字体
  m_font.CreatePointFont(800, _T("宋体"), NULL);
  GetDlgItem(IDC_STATIC_SORT)->SetFont(&m_font);

  //初始化文本框
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

}


//void CDlgRun1::OnBnClickedSortSet()
//{
//  // TODO: 在此添加控件通知处理程序代码
//}


void CDlgRun1::OnBnClickedEdtRunSet()
{
  // TODO: 在此添加控件通知处理程序代码
  
  GetMainFrame()->EfgParamSave();
  
  UpdateGridWithRecalc();
}


void CDlgRun1::OnCbnSelchangeCmbPrimary()
{
  // TODO: 在此添加控件通知处理程序代码
  m_param->user_config.measure.primary.type = ((CComboBox*)GetDlgItem(IDC_CMB_PRIMARY))->GetCurSel();
}


void CDlgRun1::OnEnChangeEdtCard()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  GetDlgItemText(IDC_EDT_CARD,m_param->user_config.measure.card, MAX_CARD_LEN);
}


void CDlgRun1::OnCbnSelchangeCmbSecondary()
{
  // TODO: 在此添加控件通知处理程序代码
  m_param->user_config.measure.secondary.type = ((CComboBox*)GetDlgItem(IDC_CMB_SECONDARY))->GetCurSel(); 
}


void CDlgRun1::OnEnChangeEdtCenterDegree()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.primary.center_degree = GetDlgItemInt(IDC_EDT_CENTER_DEGREE, 0);
}


void CDlgRun1::OnEnChangeEdtStepDegree()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.primary.step_degree = GetDlgItemInt(IDC_EDT_STEP_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtMinDegree()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.secondary.min_degree = GetDlgItemInt(IDC_EDT_MIN_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtMaxDegree()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.secondary.max_degree = GetDlgItemInt(IDC_EDT_MAX_DEGREE, 0);

}


void CDlgRun1::OnEnChangeEdtEquPhi()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.equivalent_angle.phi = GetDlgItemInt(IDC_EDT_EQU_PHI, 0);

}


void CDlgRun1::OnEnChangeEdtEquFactor()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CFormView::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.measure.equivalent_angle.factor = GetDlgItemInt(IDC_EDT_EQU_FACTOR, 0);

}

void CDlgRun1::InitGrid(void)
{
  m_gridSort.SetEditable(false);
  m_gridSort.SetTextBkColor(RGB(204, 232, 207));//黄色背景
  m_gridSort.SetRowCount(2); //初始为2行
  
  for (int i = 1; i < 2; i++)
  {
    m_gridSort.SetRowHeight(i, 25);
  }
  m_gridSort.SetColumnCount(3); //初始化为3列
  m_gridSort.SetFixedRowCount(1); //表头为1行
  m_gridSort.SetFixedColumnCount(0); //表头为1列
  m_gridSort.SetRowHeight(0, 30); //设置各行高         
                                     //m_gridSort.SetColumnWidth(0, 320); //设置0列宽 
  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("档位"));
  Item.row = 0;
  Item.col = 0;
  m_gridSort.SetItem(&Item);
  Item.strText.Format(_T("角度"));
  Item.row = 0;
  Item.col = 1;
  m_gridSort.SetItem(&Item);
  Item.strText.Format(_T("片数"));
  Item.row = 0;
  Item.col = 2;
  m_gridSort.SetItem(&Item);

  CRect cRect;
  GetDlgItem(IDC_GRID_SORT)->GetClientRect(&cRect);
  m_gridSort.SetColumnWidth(0, cRect.Width() / 5 * 1); //设置列宽 
  m_gridSort.SetColumnWidth(1, cRect.Width() / 5 * 2); //
  m_gridSort.SetColumnWidth(2, cRect.Width() / 5 * 2); //
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
  //添加分档档位
  CString str;
  for (int i = 0; i < sort_num; i++, line++)
  {
    str.Format(_T("%d"), line);
    m_gridSort.InsertRow(str, -1);
    str.Format(_T("%02d°%02d′%02d″"), S_DEG(m_result->degree.sort_sec[i])
      , S_MIN(m_result->degree.sort_sec[i])
      , S_SEC(m_result->degree.sort_sec[i]));
    m_gridSort.SetItemText(line, 1, str);
    str.Format(_T("%d"), m_result->num.y_off[i]);
    m_gridSort.SetItemText(line, 2, str);
  }
  //添加不合格档位
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
    ,m_io->GetMeasureType(m_param->user_config.measure.secondary.type) + _T("-"));
  str.Format(_T("%d"), m_result->num.secondary_n);
  m_gridSort.SetItemText(line++, 2, str);

  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1 
    ,m_io->GetMeasureType(m_param->user_config.measure.secondary.type) + _T("+"));
  str.Format(_T("%d"), m_result->num.secondary_p);
  m_gridSort.SetItemText(line++, 2, str);

  str.Format(_T("%d"), line);
  m_gridSort.InsertRow(str, -1);
  m_gridSort.SetItemText(line, 1, _T("未检出"));
  str.Format(_T("%d"), m_result->num.not_detected);
  m_gridSort.SetItemText(line++, 2, str);

  m_gridSort.ExpandLastColumn();

  //m_gridSort.SetRedraw(TRUE);
  m_gridSort.Invalidate();
}
//重新计算各档中心值
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
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_BTN_CTRL, str);
  if ("开始" == str) {
    //SetTimer(0, 100, NULL);

    gTrdMainRun = AfxBeginThread(Thread_MainRun, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    gTrdMainRun->m_bAutoDelete = TRUE;
    gTrdMainRun->ResumeThread();


    m_btn_ctrl.LoadBitmaps(IDB_STOP);
    SetDlgItemText(IDC_BTN_CTRL, _T("停止"));
  }
  else {
    //KillTimer(0);
    m_btn_ctrl.LoadBitmaps(IDB_START);
    SetDlgItemText(IDC_BTN_CTRL, _T("开始"));
  }

}


void CDlgRun1::OnBnClickedChkPause()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_CHK_PAUSE);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
   // GetDlgItem(IDC_CHK_PAUSE)->GetDC()->SetBkColor(RGB(255, 0, 0));
  }
  else
  {
   // GetDlgItem(IDC_CHK_PAUSE)->GetDC()->SetBkColor(RGB(0, 255, 0));
  }
}
