// DlgDebugDevice.cpp: 实现文件
//

#include "stdafx.h"
#include "MFC_EFG_TIME_IO.h"
#include "DlgDebugDevice.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DlgDebug1.h"//访问父窗口
#include "DlgRun1.h"//不得已要刷新运行窗口grid，不然不用包含
#include "EfgAlg.h"

// CDlgDebugDevice 对话框

IMPLEMENT_DYNAMIC(CDlgDebugDevice, CDialogEx)

CDlgDebugDevice::CDlgDebugDevice(CWnd* pParent /*=nullptr*/)
  : CDialogEx(IDD_DIALOG_DEVICE, pParent)
{
  m_param = &GetMainFrame()->m_efgio.m_configParam;
  m_io = &GetMainFrame()->m_efgio;
}

CDlgDebugDevice::~CDlgDebugDevice()
{
}

void CDlgDebugDevice::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID_STEPSET, m_gridStepSet);
  DDX_Control(pDX, IDC_GRID_USET, m_gridUSet);
}


BEGIN_MESSAGE_MAP(CDlgDebugDevice, CDialogEx)
  ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID_STEPSET, OnBeginLabelEdit)
  ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_STEPSET, OnEndLabelEdit)
  ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_STEPSET, OnSelChanged)
  ON_NOTIFY(GVN_ENDSCROLL, IDC_GRID_STEPSET, OnEndScroll)
  ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_USET, OnEndLabelEditUSet)
  //  ON_BN_CLICKED(IDC_BTN_MEASURE, &CDlgDebugDevice::OnBnClickedBtnMeasure)
  ON_BN_CLICKED(IDC_X_TIP, &CDlgDebugDevice::OnBnClickedXTip)
  ON_BN_CLICKED(IDC_XRAY_GATE, &CDlgDebugDevice::OnBnClickedXrayGate)
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_BTN_REALTIME, &CDlgDebugDevice::OnBnClickedBtnRealtime)
  ON_CBN_SELCHANGE(IDC_MOTOR_SEL, &CDlgDebugDevice::OnCbnSelchangeMotorSel)
  ON_BN_CLICKED(IDC_MOTOR_EN, &CDlgDebugDevice::OnBnClickedMotorEn)
  ON_EN_CHANGE(IDC_MOTOR_MAX, &CDlgDebugDevice::OnEnChangeMotorMax)
  ON_EN_CHANGE(IDC_MOTOR_MIN, &CDlgDebugDevice::OnEnChangeMotorMin)
  ON_EN_CHANGE(IDC_MOTOR_FLEXIBLE, &CDlgDebugDevice::OnEnChangeMotorFlexible)
  //  ON_EN_CHANGE(IDC_EDIT_OUT3, &CDlgDebugDevice::OnEnChangeEditOut3)
  //  ON_EN_CHANGE(IDC_EDIT_OUT6, &CDlgDebugDevice::OnEnChangeEditOut6)
  ON_WM_ERASEBKGND()
  ON_WM_CTLCOLOR()
  ON_CBN_SELCHANGE(IDC_CMB_TYPE, &CDlgDebugDevice::OnCbnSelchangeCmbType)
  ON_EN_CHANGE(IDC_EDT_SORT_NUM, &CDlgDebugDevice::OnEnChangeEdtSortNum)
  //  ON_EN_CHANGE(IDC_EDIT_TIME_BLOW, &CDlgDebugDevice::OnEnChangeEditTimeBlow)
  ON_EN_CHANGE(IDC_EDT_TIME_BLOW, &CDlgDebugDevice::OnEnChangeEdtTimeBlow)
  ON_EN_CHANGE(IDC_EDT_TIME_XON, &CDlgDebugDevice::OnEnChangeEdtTimeXon)
  ON_EN_CHANGE(IDC_EDT_TIME_XOFF, &CDlgDebugDevice::OnEnChangeEdtTimeXoff)
  ON_EN_CHANGE(IDC_EDT_TIME_YON, &CDlgDebugDevice::OnEnChangeEdtTimeYon)
  ON_EN_CHANGE(IDC_EDT_TIME_YOFF, &CDlgDebugDevice::OnEnChangeEdtTimeYoff)
  ON_EN_CHANGE(IDC_EDT_TIME_CLEAN, &CDlgDebugDevice::OnEnChangeEdtTimeClean)
  ON_BN_CLICKED(IDC_MOTOR_RUN, &CDlgDebugDevice::OnBnClickedMotorRun)
  ON_EN_CHANGE(IDC_MOTOR_DST, &CDlgDebugDevice::OnEnChangeMotorDst)
  ON_BN_CLICKED(IDC_MOTOR_ZERO, &CDlgDebugDevice::OnBnClickedMotorZero)
  ON_BN_CLICKED(IDC_Y_TIP, &CDlgDebugDevice::OnBnClickedYTip)
  ON_BN_CLICKED(IDC_TIP_BLOW, &CDlgDebugDevice::OnBnClickedTipBlow)
  ON_BN_CLICKED(IDC_TIP_THREE, &CDlgDebugDevice::OnBnClickedTipThree)
  ON_BN_CLICKED(IDC_ALERT, &CDlgDebugDevice::OnBnClickedAlert)
  //ON_WM_VSCROLL()
  ON_BN_CLICKED(IDC_STEP_TEST, &CDlgDebugDevice::OnBnClickedStepTest)
  ON_BN_CLICKED(IDC_EASY_SET, &CDlgDebugDevice::OnBnClickedEasySet)
  ON_BN_CLICKED(IDC_BTN_USET_TEST_MOVE, &CDlgDebugDevice::OnBnClickedBtnUsetTestMove)
  ON_BN_CLICKED(IDC_AIR_FLAG1, &CDlgDebugDevice::OnBnClickedAirFlag1)
  ON_BN_CLICKED(IDC_AIR_FLAG2, &CDlgDebugDevice::OnBnClickedAirFlag2)
  ON_BN_CLICKED(IDC_AB_TIP, &CDlgDebugDevice::OnBnClickedAbTip)
  ON_BN_CLICKED(IDC_AB_CY, &CDlgDebugDevice::OnBnClickedAbCy)
  ON_BN_CLICKED(IDC_MOTOR_SHAKE, &CDlgDebugDevice::OnBnClickedMotorShake)
END_MESSAGE_MAP()


// CDlgDebugDevice 消息处理程序


//void CDlgDebugDevice::OnBnClickedBtnMeasure()
//{
//  // TODO: 在此添加控件通知处理程序代码
//  GetMainFrame()->m_diIntCounterSnap.BindCard(0, NULL, GetMainFrame()->m_viewBoard);
// // GetMainFrame()->Switch(VIEW_BOARD);
//  GetMainFrame()->m_diIntCounterSnap.TestS();
//  //GetMainFrame()->m_viewBoard->DrawToDC(GetDlgItem(IDC_PREVIEW)->GetDC());
//}


void CDlgDebugDevice::UpdateOtherFrameAboutSort(void)
{
  //同步刷新run，隐藏如此之深，应该要封装O(∩_∩)O
  ((CDlgRun1*)GetMainFrame()->m_userFrame->m_splitUser.GetPane(0, 0))->UpdateGridWithRecalc();

}

void CDlgDebugDevice::OnBnClickedXTip()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_X_TIP);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(X_NOZZLE, IO_ON);
  }
  else
  {
    m_io->WriteDo(X_NOZZLE, IO_OFF);
  }
}

BOOL CDlgDebugDevice::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  //combobox必须放在tab控件之后
  ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->AddString(_T("X"));
  ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->AddString(_T("Y"));
  ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->AddString(_T("U"));
  ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->AddString(_T("A"));
  ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->AddString(_T("B"));
  ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->AddString(_T("C"));
  ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->AddString(_T("D"));
  //((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->SetCurSel(0);

  CBitmap bmp, sbmp;
  HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, _T("BK2.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
  if (bitmap)
    bmp.Attach(bitmap);
  else
    bmp.LoadBitmap(IDB_BK2);   //IDB_BITMAP1是图片资源ID

  CRect rect;
  GetClientRect(&rect);
  ScaleBitmap(&bmp, sbmp, rect.Width(), rect.Height());
  m_brush.CreatePatternBrush(&sbmp);
  bmp.DeleteObject();

  ((CButton*)GetDlgItem(IDC_STEP_TEST))->SetIcon(AfxGetApp()->LoadIcon(IDI_PLAY));



  //初始化Grid
  InitGrid();
  UpdateGrid();
  InitGridUSet();
  //type combo
  for (int i = 0; i < MAX_TYPE_NUM; i++)
  {
    CString tmp;
    tmp.Format(_T("%d类晶片"), i);
    ((CComboBox*)GetDlgItem(IDC_CMB_TYPE))->AddString(tmp);
  }
  ((CComboBox*)GetDlgItem(IDC_CMB_TYPE))->SetCurSel(m_param->user_config.type);
  SetDlgItemInt(IDC_EDT_SORT_NUM, (UINT)(m_param->user_config.type_sort_num[m_param->user_config.type]));
  //初始化时间编辑框
  SetDlgItemInt(IDC_EDT_TIME_BLOW, (UINT)(m_param->user_config.time.blow));
  SetDlgItemInt(IDC_EDT_TIME_XON, (UINT)(m_param->user_config.time.x_on));
  SetDlgItemInt(IDC_EDT_TIME_XOFF, (UINT)(m_param->user_config.time.x_off));
  SetDlgItemInt(IDC_EDT_TIME_YON, (UINT)(m_param->user_config.time.y_on));
  SetDlgItemInt(IDC_EDT_TIME_YOFF, (UINT)(m_param->user_config.time.y_off));
  SetDlgItemInt(IDC_EDT_TIME_CLEAN, (UINT)(m_param->user_config.time.clean));

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}


void CDlgDebugDevice::OnBnClickedXrayGate()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_XRAY_GATE);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(XRAY_GATE, IO_ON);
  }
  else
  {
    m_io->WriteDo(XRAY_GATE, IO_OFF);
  }
}


void CDlgDebugDevice::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  CButton* pChk = (CButton*)GetDlgItem(IDC_SHOCK);
  if (IO_ON == m_io->ReadDi(TURNTABLE_ZERO))
  {
    pChk->SetCheck(1);

  }
  else
  {
    pChk->SetCheck(0);
  }
  /*POINT p;
  CCellRange sel = m_gridStepSet.GetSelectedCellRange();
  int row = sel.GetMinRow();
  m_gridStepSet.GetCellOrigin(row, 1, &p);
  if (p.y >= 0)
  {
    CRect rect_btn, rect_grid;
    GetDlgItem(IDC_STEP_TEST)->GetWindowRect(&rect_btn);
    ScreenToClient(&rect_btn);
    GetDlgItem(IDC_GRID_STEPSET)->GetWindowRect(&rect_grid);
    ScreenToClient(&rect_grid);

    GetDlgItem(IDC_STEP_TEST)->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    GetDlgItem(IDC_STEP_TEST)->MoveWindow(rect_btn.left, rect_grid.top + p.y, rect_btn.Width(), rect_btn.Height());
    GetDlgItem(IDC_STEP_TEST)->ShowWindow(SW_SHOW);
  }
  else
  {
    GetDlgItem(IDC_STEP_TEST)->ShowWindow(SW_HIDE);
  }*/
  CDialogEx::OnTimer(nIDEvent);
}


void CDlgDebugDevice::OnBnClickedBtnRealtime()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  GetDlgItemText(IDC_BTN_REALTIME, str);
  if ("实时" == str) {
    SetTimer(0, 100, NULL);
    SetDlgItemText(IDC_BTN_REALTIME, _T("停止"));
  }
  else {
    KillTimer(0);
    SetDlgItemText(IDC_BTN_REALTIME, _T("实时"));
  }

}


void CDlgDebugDevice::OnCbnSelchangeMotorSel()
{
  // TODO: 在此添加控件通知处理程序代码
  CString str;
  int pos;
  int dst_pos;
  float max_freq;
  float min_freq;
  float flexible;
  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
  //GetDlgItemText(IDC_MOTOR_SEL, str);/这里应该改内存，保存ini单独保存ctrl+s
  int sel = ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->GetCurSel();
  switch (sel) {
  case 0:
    pos = m_param->motor.x.pos;
    dst_pos = m_param->motor.x.dst_pos;
    max_freq = m_param->motor.x.max_freq;
    min_freq = m_param->motor.x.min_freq;
    flexible = m_param->motor.x.flexible;
    break;
  case 1:
    pos = m_param->motor.y.pos;
    dst_pos = m_param->motor.y.dst_pos;
    max_freq = m_param->motor.y.max_freq;
    min_freq = m_param->motor.y.min_freq;
    flexible = m_param->motor.y.flexible;
    break;
  case 2:
    pos = m_param->motor.u.pos;
    dst_pos = m_param->motor.u.dst_pos;
    max_freq = m_param->motor.u.max_freq;
    min_freq = m_param->motor.u.min_freq;
    flexible = m_param->motor.u.flexible;
    break;
  case 3:
    pos = m_param->motor.a.pos;
    dst_pos = m_param->motor.a.dst_pos;
    max_freq = m_param->motor.a.max_freq;
    min_freq = m_param->motor.a.min_freq;
    flexible = m_param->motor.a.flexible;
    break;
  case 4:
    pos = m_param->motor.b.pos;
    dst_pos = m_param->motor.b.dst_pos;
    max_freq = m_param->motor.b.max_freq;
    min_freq = m_param->motor.b.min_freq;
    flexible = m_param->motor.b.flexible;
    break;
  case 5:
    pos = m_param->motor.c.pos;
    dst_pos = m_param->motor.c.dst_pos;
    max_freq = m_param->motor.c.max_freq;
    min_freq = m_param->motor.c.min_freq;
    flexible = m_param->motor.c.flexible;
    break;
  case 6:
    pos = m_param->motor.d.pos;
    dst_pos = m_param->motor.d.dst_pos;
    max_freq = m_param->motor.d.max_freq;
    min_freq = m_param->motor.d.min_freq;
    flexible = m_param->motor.d.flexible;
    break;
  default:
    return;
  }
  //
  str.Format(_T("%d"), pos);
  SetDlgItemText(IDC_MOTOR_SRC, str);
  str.Format(_T("%d"), dst_pos);
  SetDlgItemText(IDC_MOTOR_DST, str);
  str.Format(_T("%.2f"), max_freq);
  SetDlgItemText(IDC_MOTOR_MAX, str);
  str.Format(_T("%.2f"), min_freq);
  SetDlgItemText(IDC_MOTOR_MIN, str);
  str.Format(_T("%.2f"), flexible);
  SetDlgItemText(IDC_MOTOR_FLEXIBLE, str);
}


void CDlgDebugDevice::OnBnClickedMotorEn()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_MOTOR_EN);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    // GetMainFrame()->m_efgio.WriteDo(U_EN, 1);
  }
  else
  {
    //  GetMainFrame()->m_efgio.WriteDo(U_EN, 0);
  }
}


void CDlgDebugDevice::OnEnChangeMotorMax()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  CString str;
  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
  GetDlgItemText(IDC_MOTOR_MAX, str);
  double val = _wtof(str);
  int sel = ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->GetCurSel();
  switch (sel) {
  case 0:
    m_param->motor.x.max_freq = val;
    break;
  case 1:
    m_param->motor.y.max_freq = val;
    break;
  case 2:
    m_param->motor.u.max_freq = val;
    break;
  default:
    return;
  }
}


void CDlgDebugDevice::OnEnChangeMotorMin()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  CString str;
  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
  GetDlgItemText(IDC_MOTOR_MIN, str);
  double val = _wtof(str);
  int sel = ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->GetCurSel();
  switch (sel) {
  case 0:
    m_param->motor.x.min_freq = val;
    break;
  case 1:
    m_param->motor.y.min_freq = val;
    break;
  case 2:
    m_param->motor.u.min_freq = val;
    break;
  default:
    return;
  }
}


void CDlgDebugDevice::OnEnChangeMotorFlexible()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  CString str;
  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
  GetDlgItemText(IDC_MOTOR_FLEXIBLE, str);
  double val = _wtof(str);
  int sel = ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->GetCurSel();
  switch (sel) {
  case 0:
    m_param->motor.x.flexible = val;
    break;
  case 1:
    m_param->motor.y.flexible = val;
    break;
  case 2:
    m_param->motor.u.flexible = val;
    break;
  default:
    return;
  }
}


//void CDlgDebugDevice::OnEnChangeEditOut3()
//{
//  // TODO:  如果该控件是 RICHEDIT 控件，它将不
//  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
//  // 函数并调用 CRichEditCtrl().SetEventMask()，
//  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//  // TODO:  在此添加控件通知处理程序代码
//  CString str;
//  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
//  GetDlgItemText(IDC_EDIT_OUT3, str);
//  double val = _wtof(str);
//  m_param->laser.out3 = val;
//}


//void CDlgDebugDevice::OnEnChangeEditOut6()
//{
//  // TODO:  如果该控件是 RICHEDIT 控件，它将不
//  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
//  // 函数并调用 CRichEditCtrl().SetEventMask()，
//  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//  // TODO:  在此添加控件通知处理程序代码
//  CString str;
//  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
//  GetDlgItemText(IDC_EDIT_OUT6, str);
//  double val = _wtof(str);
//  m_param->laser.out6 = val;
//}


BOOL CDlgDebugDevice::OnEraseBkgnd(CDC* pDC)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  return CDialogEx::OnEraseBkgnd(pDC);

  //HBITMAP hBitmap = ::LoadBitmap(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BK2));
  ////获取位图尺寸
  //BITMAP bitmap;
  //GetObject(hBitmap, sizeof(BITMAP), &bitmap);

  ////获取对话框尺寸
  //CRect rect;
  //GetClientRect(&rect);

  ////创建DC
  //HDC m_hBkDC = ::CreateCompatibleDC(pDC->m_hDC);

  ////绘图并清理
  //if (hBitmap && m_hBkDC)
  //{
  //  ::SelectObject(m_hBkDC, hBitmap);
  //  ::StretchBlt(pDC->m_hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, m_hBkDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
  //  ::DeleteObject(hBitmap);
  //  ::DeleteDC(m_hBkDC);
  //}

  ////这个很重要
  //return TRUE;

  //gdi+的方式
  //Graphics graphics(pDC->m_hDC);

  //Image image(L"image.jpg");
  //TextureBrush tBrush(&image);
  //Pen texturedPen(&tBrush, 10);

  //graphics.DrawLine(&texturedPen, 25, 25, 325, 25);
  //tBrush.SetWrapMode(WrapModeTileFlipXY);
  //graphics.FillRectangle(&tBrush, 25, 100, 300, 200);
}


HBRUSH CDlgDebugDevice::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  在此更改 DC 的任何特性
  if (pWnd == this) {
    return m_brush;
  }
  else if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_MAX)
  {
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));
    if (pWnd->GetDlgCtrlID() == IDC_STATIC)
      return (HBRUSH)::GetStockObject(NULL_BRUSH);
  }
  // TODO:  如果默认的不是所需画笔，则返回另一个画笔
  return hbr;
}

void CDlgDebugDevice::InitGrid(void)
{
  //m_gridStepSet.SetRedraw(FALSE);
 // m_gridStepSet.SetEditable(false);
  m_gridStepSet.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//黄色背景
  m_gridStepSet.SetRowCount(2); //初始为2行
  for (int i = 1; i < 2; i++)
  {
    m_gridStepSet.SetRowHeight(i, 30);
  }
  m_gridStepSet.SetColumnCount(2); //初始化为3列
  m_gridStepSet.SetFixedRowCount(1); //表头为1行
  m_gridStepSet.SetFixedColumnCount(1); //表头为1列
  m_gridStepSet.SetRowHeight(0, 30); //设置各行高         
  //m_gridStepSet.SetColumnWidth(0, 320); //设置0列宽 
  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("位置"));
  Item.row = 0;
  Item.col = 0;
  m_gridStepSet.SetItem(&Item);
  Item.strText.Format(_T("电机步数"));
  Item.row = 0;
  Item.col = 1;
  m_gridStepSet.SetItem(&Item);

  CRect cRect;
  GetDlgItem(IDC_GRID_STEPSET)->GetClientRect(&cRect);
  m_gridStepSet.SetColumnWidth(0, cRect.Width() / 5 * 3); //设置0列宽 
  m_gridStepSet.SetColumnWidth(1, cRect.Width() / 5 * 2); //设置0列宽 
  m_gridStepSet.ExpandLastColumn();
  m_gridStepSet.SetColumnResize(FALSE);
  m_gridStepSet.SetRowResize(FALSE);
  //m_gridStepSet.SetSingleRowSelection(TRUE);
  //SetAutoSizeStyle
  //m_gridStepSet.EnableScrollBar();
 // m_gridStepSet.EnableScrollBarCtrl();
  m_gridStepSet.ShowBar(SB_HORZ, FALSE);
  //m_gridStepSet.SetRedraw(TRUE);
}

void CDlgDebugDevice::UpdateGrid()
{
  //m_gridStepSet.SetRedraw(FALSE);
  int min = 1;
  int max = m_gridStepSet.GetRowCount();
  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
  int sort_type = m_param->user_config.type;
  ASSERT(sort_type < MAX_TYPE_NUM);
  int sort_num = m_param->user_config.type_sort_num[sort_type];


  for (int i = max; i >= min; i--)
  {
    m_gridStepSet.DeleteRow(i);
  }
  int line = 1;
  //添加分档档位
  CString str;
  for (int i = 0; i < sort_num; i++, line++)
  {
    str.Format(_T("第%d档"), i + 1);
    m_gridStepSet.InsertRow(str, -1);
    //m_gridStepSet.SetItemText(line, 0, str);
    str.Format(_T("%d"), m_param->user_config.pos.y_off[sort_type][i]);
    m_gridStepSet.SetItemText(line, 1, str);
  }
  //添加不合格档位
  m_gridStepSet.InsertRow(_T("主测角低"), -1);
  str.Format(_T("%d"), m_param->user_config.pos.primary_n);
  m_gridStepSet.SetItemText(line++, 1, str);

  m_gridStepSet.InsertRow(_T("主测角高"), -1);
  str.Format(_T("%d"), m_param->user_config.pos.primary_p);
  m_gridStepSet.SetItemText(line++, 1, str);

  m_gridStepSet.InsertRow(_T("次测角低"), -1);
  str.Format(_T("%d"), m_param->user_config.pos.secondary_n);
  m_gridStepSet.SetItemText(line++, 1, str);

  m_gridStepSet.InsertRow(_T("次测角高"), -1);
  str.Format(_T("%d"), m_param->user_config.pos.secondary_p);
  m_gridStepSet.SetItemText(line++, 1, str);

  m_gridStepSet.InsertRow(_T("未检出"), -1);
  str.Format(_T("%d"), m_param->user_config.pos.not_detected);
  m_gridStepSet.SetItemText(line++, 1, str);
  //添加其他位置
  m_gridStepSet.InsertRow(_T("X轴放料位置"), -1);
  str.Format(_T("%d"), m_param->user_config.pos.x_off);
  m_gridStepSet.SetItemText(line++, 1, str);
  m_gridStepSet.InsertRow(_T("X轴等待位置"), -1);
  str.Format(_T("%d"), m_param->user_config.pos.x_wait);
  m_gridStepSet.SetItemText(line++, 1, str);
  m_gridStepSet.InsertRow(_T("Y轴等待位置"), -1);
  str.Format(_T("%d"), m_param->user_config.pos.y_wait);
  m_gridStepSet.SetItemText(line++, 1, str);

  m_gridStepSet.ExpandLastColumn();

  //m_gridStepSet.SetRedraw(TRUE);
  //m_gridStepSet.Invalidate();
  m_gridStepSet.Refresh();
}

void CDlgDebugDevice::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
  int col = pItem->iColumn;
  int row = pItem->iRow;
  m_prevEditStr = m_gridStepSet.GetItemText(row, col);
}

void CDlgDebugDevice::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
  int col = pItem->iColumn;
  int row = pItem->iRow;

  CString text = m_gridStepSet.GetItemText(row, col);
  int val = _wtoi(text);

  CString csTemp;
  csTemp.Format(_T("%d"), val);
  if (text != csTemp)
  {
    AfxMessageBox(_T("终端id只能为整数"));
    m_gridStepSet.SetItemText(row, col, m_prevEditStr);
    ::PostMessage(GetDlgItem(IDC_GRID_STEPSET)->GetSafeHwnd(), WM_LBUTTONUP, NULL, 0);
    return;
  }

  //EFG_Param *m_param = &GetMainFrame()->m_efgio.m_efg_param;
  int sort_type = m_param->user_config.type;
  ASSERT(sort_type < MAX_TYPE_NUM);
  int sort_num = m_param->user_config.type_sort_num[sort_type];
  if (row - 1 < sort_num)
  {
    m_param->user_config.pos.y_off[sort_type][row - 1] = val;
  }
  else
  {
    switch (row - sort_num) {
    case 1:
      m_param->user_config.pos.primary_n = val;
      break;
    case 2:
      m_param->user_config.pos.primary_p = val;
      break;
    case 3:
      m_param->user_config.pos.secondary_n = val;
      break;
    case 4:
      m_param->user_config.pos.secondary_p = val;
      break;
    case 5:
      m_param->user_config.pos.not_detected = val;
      break;
    case 6:
      m_param->user_config.pos.x_off = val;
      break;
    case 7:
      m_param->user_config.pos.x_wait = val;
      break;
    case 8:
      m_param->user_config.pos.y_wait = val;
      break;
    default:
      break;
    }
  }
}

void CDlgDebugDevice::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
  POINT p;
  CCellRange sel = m_gridStepSet.GetSelectedCellRange();
  int row = sel.GetMinRow();
  m_gridStepSet.GetCellOrigin(row, 1, &p);
  if (p.y >= 0)
  {
    CRect rect_btn, rect_grid;
    GetDlgItem(IDC_STEP_TEST)->GetWindowRect(&rect_btn);
    ScreenToClient(&rect_btn);
    GetDlgItem(IDC_GRID_STEPSET)->GetWindowRect(&rect_grid);
    ScreenToClient(&rect_grid);

    GetDlgItem(IDC_STEP_TEST)->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    GetDlgItem(IDC_STEP_TEST)->MoveWindow(rect_btn.left, rect_grid.top + p.y, rect_btn.Width(), rect_btn.Height());
    GetDlgItem(IDC_STEP_TEST)->ShowWindow(SW_SHOW);
  }
  else
  {
    GetDlgItem(IDC_STEP_TEST)->ShowWindow(SW_HIDE);
  }
}

void CDlgDebugDevice::OnEndScroll(NMHDR* pNMHDR, LRESULT* pResult)
{
  POINT p;
  CCellRange sel = m_gridStepSet.GetSelectedCellRange();
  int row = sel.GetMinRow();
  m_gridStepSet.GetCellOrigin(row, 1, &p);
  if (p.y >= 0)
  {
    CRect rect_btn, rect_grid;
    GetDlgItem(IDC_STEP_TEST)->GetWindowRect(&rect_btn);
    ScreenToClient(&rect_btn);
    GetDlgItem(IDC_GRID_STEPSET)->GetWindowRect(&rect_grid);
    ScreenToClient(&rect_grid);

    GetDlgItem(IDC_STEP_TEST)->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    GetDlgItem(IDC_STEP_TEST)->MoveWindow(rect_btn.left, rect_grid.top + p.y, rect_btn.Width(), rect_btn.Height());
    GetDlgItem(IDC_STEP_TEST)->ShowWindow(SW_SHOW);
  }
  else
  {
    GetDlgItem(IDC_STEP_TEST)->ShowWindow(SW_HIDE);
  }
}


void CDlgDebugDevice::OnCbnSelchangeCmbType()
{
  // TODO: 在此添加控件通知处理程序代码
  int sel = ((CComboBox*)GetDlgItem(IDC_CMB_TYPE))->GetCurSel();
  ASSERT(sel < MAX_TYPE_NUM);
  m_param->user_config.type = sel;
  SetDlgItemInt(IDC_EDT_SORT_NUM, (UINT)(m_param->user_config.type_sort_num[sel]));
  UpdateGrid();
  UpdateOtherFrameAboutSort();
}


void CDlgDebugDevice::OnEnChangeEdtSortNum()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.type_sort_num[m_param->user_config.type] = GetDlgItemInt(IDC_EDT_SORT_NUM, 0);
  UpdateGrid();
  UpdateOtherFrameAboutSort();
}


void CDlgDebugDevice::OnEnChangeEdtTimeBlow()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.time.blow = GetDlgItemInt(IDC_EDT_TIME_BLOW, 0);
}


void CDlgDebugDevice::OnEnChangeEdtTimeXon()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.time.x_on = GetDlgItemInt(IDC_EDT_TIME_XON, 0);
}


void CDlgDebugDevice::OnEnChangeEdtTimeXoff()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.time.x_off = GetDlgItemInt(IDC_EDT_TIME_XOFF, 0);
}


void CDlgDebugDevice::OnEnChangeEdtTimeYon()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.time.y_on = GetDlgItemInt(IDC_EDT_TIME_YON, 0);
}


void CDlgDebugDevice::OnEnChangeEdtTimeYoff()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.time.y_off = GetDlgItemInt(IDC_EDT_TIME_YOFF, 0);
}


void CDlgDebugDevice::OnEnChangeEdtTimeClean()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
  m_param->user_config.time.clean = GetDlgItemInt(IDC_EDT_TIME_CLEAN, 0);
}


void CDlgDebugDevice::OnBnClickedMotorRun()
{
  // TODO: 在此添加控件通知处理程序代码
  //CEfgIO *io = m_io;

  int sel = ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->GetCurSel();
  int dst = GetDlgItemInt(IDC_MOTOR_DST);

  if (sel < 0)
    return;

  m_io->MotoRun(sel, dst);

  SetDlgItemInt(IDC_MOTOR_SRC, dst);

  switch (sel) {
  case 0:
    m_param->motor.x.pos = dst;
    break;
  case 1:
    m_param->motor.y.pos = dst;
    break;
  case 2:
    m_param->motor.u.pos = dst;
    break;
  case 3:
    m_param->motor.a.pos = dst;
    break;
  case 4:
    m_param->motor.b.pos = dst;
    break;
  case 5:
    m_param->motor.c.pos = dst;
    break;
  case 6:
    m_param->motor.d.pos = dst;
    break;
  default:
    return;
  }
  //switch (sel) {
  //case 0:
  //  m_param->motor.x.dst_pos = dst;
  //  dst = m_param->motor.x.dst_pos - m_param->motor.x.pos;
  //  //设定方向
  //  if (dst > 0)
  //    io->WriteDo(X_DIR, 1);
  //  else
  //    io->WriteDo(X_DIR, 0);
  //  dst = abs(dst);
  //  //设定步数
  //  io->WritePort(X_STEP_LOW, dst);
  //  io->WritePort(X_STEP_HIGH, dst>>8);
  //  //下降沿
  //  io->WriteDo(X_FULL_CURRENT, 0);
  //  io->WriteDo(X_START, 1);
  //  Sleep(1);
  //  io->WriteDo(X_START, 0);
  //  io->WriteDo(X_FULL_CURRENT, 1);
  //  Sleep(1);
  //  while (0!=io->ReadDi(X_END));
  //  m_param->motor.x.pos = m_param->motor.x.dst_pos;
  //  break;
  //case 1:
  //  m_param->motor.y.dst_pos = dst;
  //  dst = m_param->motor.y.dst_pos - m_param->motor.y.pos;
  //  //设定方向
  //  if (dst > 0)
  //    io->WriteDo(Y_DIR, 1);
  //  else
  //    io->WriteDo(Y_DIR, 0);
  //  dst = abs(dst);
  //  //设定步数
  //  io->WritePort(Y_STEP_LOW, dst);
  //  io->WritePort(Y_STEP_HIGH, dst >> 8);
  //  //下降沿
  //  io->WriteDo(Y_FULL_CURRENT, 0);
  //  io->WriteDo(Y_START, 1);
  //  Sleep(1);
  //  io->WriteDo(Y_START, 0);
  //  io->WriteDo(Y_FULL_CURRENT, 1);
  //  Sleep(1);
  //  while (0 != io->ReadDi(Y_END));
  //  m_param->motor.y.pos = m_param->motor.y.dst_pos;
  //  break;
  //case 2:
  //  m_param->motor.u.dst_pos = dst;
  //  dst = m_param->motor.u.dst_pos - m_param->motor.u.pos;
  //  //设定方向
  //  if (dst > 0)
  //    io->WriteDo(U_DIR, 1);
  //  else
  //    io->WriteDo(U_DIR, 0);
  //  dst = abs(dst);
  //  //设定步数
  //  io->WritePort(U_STEP_LOW, dst);
  //  io->WritePort(U_STEP_HIGH, dst >> 8);
  //  //下降沿
  //  io->WriteDo(U_START, 1);
  //  Sleep(1);
  //  io->WriteDo(U_START, 0);
  //  Sleep(1);
  //  while (0 != io->ReadDi(U_END));
  //  m_param->motor.u.pos = m_param->motor.u.dst_pos;
  //  break;
  //default:
  //  return;
  //}

}


void CDlgDebugDevice::OnEnChangeMotorDst()
{
  // TODO:  如果该控件是 RICHEDIT 控件，它将不
  // 发送此通知，除非重写 CDialogEx::OnInitDialog()
  // 函数并调用 CRichEditCtrl().SetEventMask()，
  // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

  // TODO:  在此添加控件通知处理程序代码
}


void CDlgDebugDevice::OnBnClickedMotorZero()
{
  // TODO: 在此添加控件通知处理程序代码
  CEfgIO* io = m_io;

  int sel = ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->GetCurSel();
  io->MotoZero(sel);

  SetDlgItemInt(IDC_MOTOR_SRC, 0);

  switch (sel) {
  case 0:
    m_param->motor.x.pos = 0;
    break;
  case 1:
    m_param->motor.y.pos = 0;
    break;
  case 2:
    m_param->motor.u.pos = 0;
    break;
  case 3:
    m_param->motor.a.pos = 0;
    break;
  case 4:
    m_param->motor.b.pos = 0;
    break;
  case 5:
    m_param->motor.c.pos = 0;
    break;
  case 6:
    m_param->motor.d.pos = 0;
    break;
  default:
    return;
  }
}


void CDlgDebugDevice::OnBnClickedYTip()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_Y_TIP);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(Y_NOZZLE, IO_ON);
  }
  else
  {
    m_io->WriteDo(Y_NOZZLE, IO_OFF);
  }
}


void CDlgDebugDevice::OnBnClickedTipBlow()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_TIP_BLOW);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(BLOW, IO_ON);
  }
  else
  {
    m_io->WriteDo(BLOW, IO_OFF);
  }
}


void CDlgDebugDevice::OnBnClickedTipThree()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_TIP_THREE);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(CLEAN, IO_ON);
  }
  else
  {
    m_io->WriteDo(CLEAN, IO_OFF);
  }
}


void CDlgDebugDevice::OnBnClickedAlert()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_ALERT);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(ALERT, IO_ON);
  }
  else
  {
    m_io->WriteDo(ALERT, IO_OFF);
  }
}


//void CDlgDebugDevice::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//  // TODO: 在此添加消息处理程序代码和/或调用默认值
//
//  CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
//}


void CDlgDebugDevice::OnBnClickedStepTest()
{
  // TODO: 在此添加控件通知处理程序代码
  POINT p;
  CCellRange sel = m_gridStepSet.GetSelectedCellRange();
  int row = sel.GetMinRow();
  CString text = m_gridStepSet.GetItemText(row, 1);
  int val = _wtoi(text);

  int motosel = -1;
  int sort_type = m_param->user_config.type;
  ASSERT(sort_type < MAX_TYPE_NUM);
  int sort_num = m_param->user_config.type_sort_num[sort_type];

  if (row - 1 < sort_num)
  {
    motosel = MOTOR_Y;
  }
  else
  {
    switch (row - sort_num) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      motosel = MOTOR_Y;
      break;
    case 6:
    case 7:
      motosel = MOTOR_X;
      break;
    case 8:
      motosel = MOTOR_Y;
      break;
    default:
      break;
    }
  }
  m_io->MotoZero(motosel);
  Sleep(500);
  m_io->MotoRun(motosel, val);

  /*m_gridStepSet.GetCellOrigin(row, 1, &p);
  if (p.y >= 0)
  {
    CRect rect_btn, rect_grid;
    GetDlgItem(IDC_STEP_TEST)->GetWindowRect(&rect_btn);
    ScreenToClient(&rect_btn);
    GetDlgItem(IDC_GRID_STEPSET)->GetWindowRect(&rect_grid);
    ScreenToClient(&rect_grid);

    GetDlgItem(IDC_STEP_TEST)->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    GetDlgItem(IDC_STEP_TEST)->MoveWindow(rect_btn.left, rect_grid.top + p.y, rect_btn.Width(), rect_btn.Height());
    GetDlgItem(IDC_STEP_TEST)->ShowWindow(SW_SHOW);
  }
  else
  {
    GetDlgItem(IDC_STEP_TEST)->ShowWindow(SW_HIDE);
  }*/

}


void CDlgDebugDevice::OnBnClickedEasySet()
{
  // TODO: 在此添加控件通知处理程序代码
  int sort_type = m_param->user_config.type;
  int sort_num = m_param->user_config.type_sort_num[sort_type];
  int step = m_param->user_config.pos.y_off[sort_type][1] - m_param->user_config.pos.y_off[sort_type][0];
  int tmp = m_param->user_config.pos.y_off[sort_type][1];
  int i = 2;
  while (i < sort_num)
  {
    tmp += step;
    m_param->user_config.pos.y_off[sort_type][i] = tmp;
    i++;
  }
  tmp += step;
  m_param->user_config.pos.primary_n = tmp;
  tmp += step;
  m_param->user_config.pos.primary_p = tmp;
  tmp += step;
  m_param->user_config.pos.secondary_n = tmp;
  tmp += step;
  m_param->user_config.pos.secondary_p = tmp;
  tmp += step;
  m_param->user_config.pos.not_detected = tmp;

  UpdateGrid();
}


void CDlgDebugDevice::OnEndLabelEditUSet(NMHDR* pNMHDR, LRESULT* pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
  int col = pItem->iColumn;
  int row = pItem->iRow;

  CString text = m_gridUSet.GetItemText(row, col);
  int val = _wtoi(text);

  CString csTemp;
  csTemp.Format(_T("%d"), val);
  if (text != csTemp)
  {
    AfxMessageBox(_T("终端id只能为整数"));
    ::PostMessage(GetDlgItem(IDC_GRID_USET)->GetSafeHwnd(), WM_LBUTTONUP, NULL, 0);
    return;
  }


  switch (row)
  {
  case 1:
    switch (col) {
    case 0:
      if (USER_TO_DEG(val) > USER_TO_DEG(m_param->u_auto.degree[1]))
        AfxMessageBox(_T("注意，大于下面角度了"));
      m_param->u_auto.degree[0] = val;
      break;
    case 1:
      if (val > m_param->u_auto.step[1])
        AfxMessageBox(_T("注意，大于下面步数了"));
      m_param->u_auto.step[0] = val;
      break;

    default:
      break;
    }
    break;
  case 2:
    switch (col) {
    case 0:
      if (USER_TO_DEG(val) < USER_TO_DEG(m_param->u_auto.degree[0]))
        AfxMessageBox(_T("注意，小于上面角度了"));
      m_param->u_auto.degree[1] = val;
      break;
    case 1:
      if (val < m_param->u_auto.step[0])
        AfxMessageBox(_T("注意，小于上面步数了"));
      m_param->u_auto.step[1] = val;
      break;

    default:
      break;
    }
    break;
  default:
    break;
  }

}


void CDlgDebugDevice::InitGridUSet(void)
{

  m_gridUSet.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//黄色背景
  m_gridUSet.SetRowCount(3); //初始为2行
  for (int i = 1; i < 3; i++)
  {
    m_gridUSet.SetRowHeight(i, 30);
  }
  m_gridUSet.SetColumnCount(2); //初始化为3列
  m_gridUSet.SetFixedRowCount(1); //表头为1行
  //m_gridUSet.SetFixedColumnCount(1); //表头为1列
  m_gridUSet.SetRowHeight(0, 30); //设置各行高         

  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("角度"));
  Item.row = 0;
  Item.col = 0;
  m_gridUSet.SetItem(&Item);
  Item.strText.Format(_T("电机步数"));
  Item.row = 0;
  Item.col = 1;
  m_gridUSet.SetItem(&Item);

  m_gridUSet.ShowBar(SB_HORZ, FALSE);
  m_gridUSet.ShowBar(SB_VERT, FALSE);

  CRect cRect;
  GetDlgItem(IDC_GRID_USET)->GetClientRect(&cRect);
  m_gridUSet.SetColumnWidth(0, cRect.Width() / 4 * 2); //设置0列宽 
  m_gridUSet.SetColumnWidth(1, cRect.Width() / 4 * 2); //设置0列宽 
  m_gridUSet.ExpandLastColumn();
  m_gridUSet.SetColumnResize(FALSE);
  m_gridUSet.SetRowResize(FALSE);


  CString str;
  str.Format(_T("%d"), m_param->u_auto.degree[0]);
  m_gridUSet.SetItemText(1, 0, str);
  str.Format(_T("%d"), m_param->u_auto.step[0]);
  m_gridUSet.SetItemText(1, 1, str);
  str.Format(_T("%d"), m_param->u_auto.degree[1]);
  m_gridUSet.SetItemText(2, 0, str);
  str.Format(_T("%d"), m_param->u_auto.step[1]);
  m_gridUSet.SetItemText(2, 1, str);

  //m_gridUSet.Refresh();
}


void CDlgDebugDevice::OnBnClickedBtnUsetTestMove()
{
  // TODO: 在此添加控件通知处理程序代码
  int sel = ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->GetCurSel();
  int deg = GetDlgItemInt(IDC_EDT_USET_TEST_DEG);
  m_io->UAutoRun(USER_TO_DEG(deg));
  if (2 == sel)
    SetDlgItemInt(IDC_MOTOR_SRC, m_param->motor.u.pos);
  AfxMessageBox(_T("U轴调整结束"));
}


void CDlgDebugDevice::OnBnClickedAirFlag1()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_AIR_FLAG1);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(AIR_FLAG1, IO_ON);
  }
  else
  {
    m_io->WriteDo(AIR_FLAG1, IO_OFF);
  }
}


void CDlgDebugDevice::OnBnClickedAirFlag2()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_AIR_FLAG2);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(AIR_FLAG2, IO_ON);
  }
  else
  {
    m_io->WriteDo(AIR_FLAG2, IO_OFF);
  }
}


void CDlgDebugDevice::OnBnClickedAbTip()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_AB_TIP);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(AB_NOZZLE, IO_ON);
  }
  else
  {
    m_io->WriteDo(AB_NOZZLE, IO_OFF);
  }
}


void CDlgDebugDevice::OnBnClickedAbCy()
{
  // TODO: 在此添加控件通知处理程序代码
  CButton* pChk = (CButton*)GetDlgItem(IDC_AB_CY);
  int nStat = pChk->GetCheck();
  if (nStat)
  {
    m_io->WriteDo(AB_CYLINDER, IO_ON);
  }
  else
  {
    m_io->WriteDo(AB_CYLINDER, IO_OFF);
  }
}


void CDlgDebugDevice::OnBnClickedMotorShake()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel = ((CComboBox*)GetDlgItem(IDC_MOTOR_SEL))->GetCurSel();
  int dst = GetDlgItemInt(IDC_MOTOR_DST);

  if (sel < 0)
    return;

  m_io->MotoShake(sel, dst);

}
