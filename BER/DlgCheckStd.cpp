// DlgCheckStd.cpp : 实现文件
//

#include "stdafx.h"
#include "BER.h"
#include "DlgCheckStd.h"
#include "afxdialogex.h"

#include "DlgPassword.h"
#include "DlgStdSet.h"

// CDlgCheckStd 对话框

IMPLEMENT_DYNAMIC(CDlgCheckStd, CDialogEx)

CDlgCheckStd::CDlgCheckStd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCheckStd::IDD, pParent)
{
  //CList<> list;
  m_stdLib = new CStandardLib(gstuPathInf.csPathExe + _T("\\data.db"));
}

CDlgCheckStd::~CDlgCheckStd()
{
  delete m_stdLib;
}

void CDlgCheckStd::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID_STD_SERIES, m_gridStdSeries);
  DDX_Control(pDX, IDC_GRID_STD_LIB, m_gridStdLib);
  DDX_Control(pDX, IDC_GRID_STD_CHECKED, m_gridStdChecked);
}


BEGIN_MESSAGE_MAP(CDlgCheckStd, CDialogEx)
  ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID_STD_SERIES, OnBeginLabelEdit_StdSeries)
  ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_STD_SERIES, OnEndLabelEdit_StdSeries)
  ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_STD_SERIES, OnSelChanged_StdSeries)
  ON_NOTIFY(GVN_ENDSCROLL, IDC_GRID_STD_SERIES, OnEndScroll_StdSeries)

  ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID_STD_LIB, OnBeginLabelEdit_StdLib)
  ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_STD_LIB, OnEndLabelEdit_StdLib)
  ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_STD_LIB, OnSelChanged_StdLib)
  ON_NOTIFY(GVN_ENDSCROLL, IDC_GRID_STD_LIB, OnEndScroll_StdLib)

  ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID_STD_CHECKED, OnBeginLabelEdit_StdChecked)
  ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_STD_CHECKED, OnEndLabelEdit_StdChecked)
  ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_STD_CHECKED, OnSelChanged_StdChecked)
  ON_NOTIFY(GVN_ENDSCROLL, IDC_GRID_STD_CHECKED, OnEndScroll_StdChecked)

  ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgCheckStd::OnBnClickedBtnSave)
  ON_BN_CLICKED(IDC_BTN_RELOAD, &CDlgCheckStd::OnBnClickedBtnReload)
  ON_BN_CLICKED(IDC_BTN_STD_CHECKING, &CDlgCheckStd::OnBnClickedBtnStdChecking)
  ON_BN_CLICKED(IDC_BTN_SET, &CDlgCheckStd::OnBnClickedBtnSet)
END_MESSAGE_MAP()


// CDlgCheckStd 消息处理程序


BOOL CDlgCheckStd::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  /*CString datapath=_T("data.db");
  SQLiteWrapper sqlite;
  if (sqlite.Open(CW2A(datapath))) {
    AfxMessageBox(_T("数据库打开"));
  }
  else {
    AfxMessageBox(_T("数据库打开失败"));
  }
  std::string sqlcreate =  "CREATE TABLE COMPANY("  \
    "ID INT PRIMARY KEY     NOT NULL," \
    "NAME           TEXT    NOT NULL," \
    "AGE            INT     NOT NULL," \
    "ADDRESS        CHAR(50)," \
    "SALARY         REAL );";
  bool isok = sqlite.Writedb(std::string(sqlcreate));
  std::string sqlinsert = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
    "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
    "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
    "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
    "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
    "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
    "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
    "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
  isok = sqlite.Writedb(std::string(sqlinsert));

  std::string sqlstr = "SELECT * FROM COMPANY where NAME='Mark';";
  SQLiteStatement* stmt = sqlite.Readdb(sqlstr);
  int count;
  if (stmt)
  {
    while (stmt->NextRow()) {
      int id = stmt->ValueInt(0);
      CString str = stmt->ValueString(1);
    }
  }
  delete stmt;
  sqlite.Close();*/

  InitGrid_StdSeries();

  InitGrid_StdLib();

  InitGrid_StdChecked();

  UpdateGrid_StdSeries();

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}

void CDlgCheckStd::InitGrid_StdSeries(void)
{
  m_seriesRowSel = -1;

  m_gridStdSeries.SetTextBkColor(RGB(0xCC, 0xFF, 0xCC));//背景
  m_gridStdSeries.SetRowCount(1); //初始为2行

  //for (int i = 1; i < 2; i++)
  //{
  //  m_gridStdSeries.SetRowHeight(i, 30);
  //}

  m_gridStdSeries.SetColumnCount(5); //初始化为5列
  m_gridStdSeries.SetFixedRowCount(1); //表头为1行
  m_gridStdSeries.SetFixedColumnCount(1); //表头为1列
  m_gridStdSeries.SetRowHeight(0, 30); //设置各行高         
  //m_gridStdSeries.SetColumnWidth(0, 320); //设置0列宽 
  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("No"));
  Item.row = 0;
  Item.col = 0;
  m_gridStdSeries.SetItem(&Item);

  Item.strText.Format(_T("电-"));
  Item.row = 0;
  Item.col = 1;
  m_gridStdSeries.SetItem(&Item);

  Item.strText.Format(_T("电+"));
  Item.row = 0;
  Item.col = 2;
  m_gridStdSeries.SetItem(&Item);

  Item.strText.Format(_T("光-"));
  Item.row = 0;
  Item.col = 3;
  m_gridStdSeries.SetItem(&Item);

  Item.strText.Format(_T("光+"));
  Item.row = 0;
  Item.col = 4;
  m_gridStdSeries.SetItem(&Item);

  CRect cRect;
  GetDlgItem(IDC_GRID_STD_SERIES)->GetClientRect(&cRect);
  m_gridStdSeries.SetColumnWidth(0, cRect.Width() / 5 * 1); //设置0列宽 
  m_gridStdSeries.SetColumnWidth(1, cRect.Width() / 5 * 1); //设置0列宽 
  m_gridStdSeries.SetColumnWidth(2, cRect.Width() / 5 * 1); //设置0列宽 
  m_gridStdSeries.SetColumnWidth(3, cRect.Width() / 5 * 1); //设置0列宽 
  m_gridStdSeries.SetColumnWidth(4, cRect.Width() / 5 * 1); //设置0列宽 
  m_gridStdSeries.ExpandLastColumn();
  m_gridStdSeries.SetColumnResize(FALSE);
  m_gridStdSeries.SetRowResize(FALSE);
  //m_gridStdSeries.SetSingleRowSelection(TRUE);
  //SetAutoSizeStyle
  //m_gridStdSeries.EnableScrollBar();
 // m_gridStdSeries.EnableScrollBarCtrl();
  m_gridStdSeries.ShowBar(SB_HORZ, FALSE);
}

void CDlgCheckStd::UpdateGrid_StdSeries(void)
{
  //m_gridStdSeries.SetRedraw(FALSE);
  int min = 1;
  int max = m_gridStdSeries.GetRowCount();

  int sort_num = m_stdLib->m_series.GetCount();

  for (int i = max; i >= min; i--)
  {
    m_gridStdSeries.DeleteRow(i);
  }
  int line = 1;

  CString str;

  for (int i = 0; i < sort_num; i++, line++)
  {
    str.Format(_T("%d"), m_stdLib->m_series[i].m_no);
    m_gridStdSeries.InsertRow(str, -1);
    str.Format(_T("%d"), m_stdLib->m_series[i].m_phi_n);
    m_gridStdSeries.SetItemText(line, 1, str);
    str.Format(_T("%d"), m_stdLib->m_series[i].m_phi_p);
    m_gridStdSeries.SetItemText(line, 2, str);
    str.Format(_T("%d"), m_stdLib->m_series[i].m_laser_n);
    m_gridStdSeries.SetItemText(line, 3, str);
    str.Format(_T("%d"), m_stdLib->m_series[i].m_laser_p);
    m_gridStdSeries.SetItemText(line, 4, str);
  }

  m_gridStdSeries.ExpandLastColumn();

  //m_gridStdSeries.SetRedraw(TRUE);
  m_gridStdSeries.Refresh();
}

void CDlgCheckStd::OnBeginLabelEdit_StdSeries(NMHDR * pNMHDR, LRESULT * pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
  int col = pItem->iColumn;
  int row = pItem->iRow;
  m_prevEditStr = m_gridStdSeries.GetItemText(row, col);
}

void CDlgCheckStd::OnEndLabelEdit_StdSeries(NMHDR * pNMHDR, LRESULT * pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;

  int col = pItem->iColumn;
  int row = pItem->iRow;

  CString text = m_gridStdSeries.GetItemText(row, col);
  int val = _wtoi(text);

  CString csTemp;
  csTemp.Format(_T("%d"), val);

  if (text != csTemp)
  {
    AfxMessageBox(_T("只能输入整数"));
    m_gridStdSeries.SetItemText(row, col, m_prevEditStr);
    ::PostMessage(GetDlgItem(IDC_GRID_STD_SERIES)->GetSafeHwnd(), WM_LBUTTONUP, NULL, 0);
    return;
  }
  
  CStdSeries series;

  series.m_no = _wtoi(m_gridStdSeries.GetItemText(row, 0));
  series.m_phi_n = _wtoi(m_gridStdSeries.GetItemText(row, 1));
  series.m_phi_p = _wtoi(m_gridStdSeries.GetItemText(row, 2));
  series.m_laser_n = _wtoi(m_gridStdSeries.GetItemText(row, 3));
  series.m_laser_p = _wtoi(m_gridStdSeries.GetItemText(row, 4));

  m_stdLib->SetSeries(series);

  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);

}

void CDlgCheckStd::OnSelChanged_StdSeries(NMHDR * pNMHDR, LRESULT * pResult)
{
  POINT p;

  CCellRange sel = m_gridStdSeries.GetSelectedCellRange();

  int row = sel.GetMinRow();

  if (row == m_seriesRowSel)
    return;

  m_seriesRowSel = row;

  CString text = m_gridStdSeries.GetItemText(row, 0);

  int id = _wtoi(text);

  m_stdLib->LoadLib(id);
  //刷新库
  UpdateGrid_StdLib();
}

void CDlgCheckStd::OnEndScroll_StdSeries(NMHDR * pNMHDR, LRESULT * pResult)
{
}

void CDlgCheckStd::InitGrid_StdLib(void)
{
  m_gridStdLib.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//黄色背景
  m_gridStdLib.SetRowCount(1); //初始为2行

  //for (int i = 1; i < 2; i++)
  //{
  //  m_gridStdLib.SetRowHeight(i, 30);
  //}

  m_gridStdLib.SetColumnCount(3); //初始化为3列
  m_gridStdLib.SetFixedRowCount(1); //表头为1行
  m_gridStdLib.SetFixedColumnCount(1); //表头为1列
  m_gridStdLib.SetRowHeight(0, 30); //设置各行高         

  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("No"));
  Item.row = 0;
  Item.col = 0;
  m_gridStdLib.SetItem(&Item);

  Item.strText.Format(_T("电"));
  Item.row = 0;
  Item.col = 1;
  m_gridStdLib.SetItem(&Item);

  Item.strText.Format(_T("光"));
  Item.row = 0;
  Item.col = 2;
  m_gridStdLib.SetItem(&Item);


  CRect cRect;
  GetDlgItem(IDC_GRID_STD_LIB)->GetClientRect(&cRect);
  m_gridStdLib.SetColumnWidth(0, cRect.Width() / 3 * 1); //设置0列宽 
  m_gridStdLib.SetColumnWidth(1, cRect.Width() / 3 * 1); //设置0列宽 
  m_gridStdLib.SetColumnWidth(2, cRect.Width() / 3 * 1); //设置0列宽 

  m_gridStdLib.ExpandLastColumn();
  m_gridStdLib.SetColumnResize(FALSE);
  m_gridStdLib.SetRowResize(FALSE);
  //m_gridStdSeries.SetSingleRowSelection(TRUE);
  //SetAutoSizeStyle
  //m_gridStdSeries.EnableScrollBar();
 // m_gridStdSeries.EnableScrollBarCtrl();
  m_gridStdLib.ShowBar(SB_HORZ, FALSE);
}

void CDlgCheckStd::UpdateGrid_StdLib(void)
{
  int min = 1;
  int max = m_gridStdLib.GetRowCount();

  int sort_num = m_stdLib->m_lib.GetCount();

  for (int i = max; i >= min; i--)
  {
    m_gridStdLib.DeleteRow(i);
  }
  int line = 1;

  CString str;

  for (int i = 0; i < sort_num; i++, line++)
  {
    str.Format(_T("%d"), m_stdLib->m_lib[i].m_no);
    m_gridStdLib.InsertRow(str, -1);
    str.Format(_T("%d"), m_stdLib->m_lib[i].m_phi);
    m_gridStdLib.SetItemText(line, 1, str);
    str.Format(_T("%d"), m_stdLib->m_lib[i].m_laser);
    m_gridStdLib.SetItemText(line, 2, str);
  }

  m_gridStdLib.ExpandLastColumn();

  m_gridStdLib.Refresh();
}

void CDlgCheckStd::OnBeginLabelEdit_StdLib(NMHDR * pNMHDR, LRESULT * pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
  int col = pItem->iColumn;
  int row = pItem->iRow;
  m_prevEditStr = m_gridStdLib.GetItemText(row, col);
}

void CDlgCheckStd::OnEndLabelEdit_StdLib(NMHDR * pNMHDR, LRESULT * pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;

  int col = pItem->iColumn;
  int row = pItem->iRow;

  CString text = m_gridStdLib.GetItemText(row, col);
  int val = _wtoi(text);

  CString csTemp;
  csTemp.Format(_T("%d"), val);

  if (text != csTemp)
  {
    AfxMessageBox(_T("只能输入整数"));
    m_gridStdLib.SetItemText(row, col, m_prevEditStr);
    ::PostMessage(GetDlgItem(IDC_GRID_STD_LIB)->GetSafeHwnd(), WM_LBUTTONUP, NULL, 0);
    return;
  }

  CStdLib std;

  std.m_no = _wtoi(m_gridStdLib.GetItemText(row, 0));
  std.m_phi = _wtoi(m_gridStdLib.GetItemText(row, 1));
  std.m_laser = _wtoi(m_gridStdLib.GetItemText(row, 2));

  m_stdLib->SetStd(std);

  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
}

void CDlgCheckStd::OnSelChanged_StdLib(NMHDR * pNMHDR, LRESULT * pResult)
{
}

void CDlgCheckStd::OnEndScroll_StdLib(NMHDR * pNMHDR, LRESULT * pResult)
{
}

void CDlgCheckStd::InitGrid_StdChecked(void)
{
  m_gridStdChecked.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//黄色背景
  m_gridStdChecked.SetRowCount(1); //初始为2行

  //for (int i = 1; i < 2; i++)
  //{
  //  m_gridStdChecked.SetRowHeight(i, 30);
  //}

  m_gridStdChecked.SetColumnCount(3); //初始化为3列
  m_gridStdChecked.SetFixedRowCount(1); //表头为1行
  m_gridStdChecked.SetFixedColumnCount(1); //表头为1列
  m_gridStdChecked.SetRowHeight(0, 30); //设置各行高         

  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("No"));
  Item.row = 0;
  Item.col = 0;
  m_gridStdChecked.SetItem(&Item);

  Item.strText.Format(_T("电"));
  Item.row = 0;
  Item.col = 1;
  m_gridStdChecked.SetItem(&Item);

  Item.strText.Format(_T("光"));
  Item.row = 0;
  Item.col = 2;
  m_gridStdChecked.SetItem(&Item);


  CRect cRect;
  GetDlgItem(IDC_GRID_STD_LIB)->GetClientRect(&cRect);
  m_gridStdChecked.SetColumnWidth(0, cRect.Width() / 3 * 1); //设置0列宽 
  m_gridStdChecked.SetColumnWidth(1, cRect.Width() / 3 * 1); //设置0列宽 
  m_gridStdChecked.SetColumnWidth(2, cRect.Width() / 3 * 1); //设置0列宽 

  m_gridStdChecked.ExpandLastColumn();
  m_gridStdChecked.SetColumnResize(FALSE);
  m_gridStdChecked.SetRowResize(FALSE);
  m_gridStdChecked.SetSingleRowSelection(TRUE);
  m_gridStdChecked.SetEditable(FALSE);
  //SetAutoSizeStyle
  //m_gridStdSeries.EnableScrollBar();
 // m_gridStdSeries.EnableScrollBarCtrl();
  m_gridStdChecked.ShowBar(SB_HORZ, FALSE);
}

void CDlgCheckStd::UpdateGrid_StdChecked(void)
{
  int min = 1;
  int max = m_gridStdChecked.GetRowCount();

  int sort_num = m_stdLib->m_checked.GetCount();

  for (int i = max; i >= min; i--)
  {
    m_gridStdChecked.DeleteRow(i);
  }
  int line = 1;

  CString str;

  for (int i = 0; i < sort_num; i++, line++)
  {
    str.Format(_T("%d"), m_stdLib->m_checked[i].m_no);
    m_gridStdChecked.InsertRow(str, -1);
    str.Format(_T("%d"), m_stdLib->m_checked[i].m_phi);
    m_gridStdChecked.SetItemText(line, 1, str);
    str.Format(_T("%d"), m_stdLib->m_checked[i].m_laser);
    m_gridStdChecked.SetItemText(line, 2, str);
  }

  m_gridStdChecked.ExpandLastColumn();

  m_gridStdChecked.Refresh();
}

void CDlgCheckStd::OnBeginLabelEdit_StdChecked(NMHDR * pNMHDR, LRESULT * pResult)
{
}

void CDlgCheckStd::OnEndLabelEdit_StdChecked(NMHDR * pNMHDR, LRESULT * pResult)
{
}

void CDlgCheckStd::OnSelChanged_StdChecked(NMHDR * pNMHDR, LRESULT * pResult)
{
}

void CDlgCheckStd::OnEndScroll_StdChecked(NMHDR * pNMHDR, LRESULT * pResult)
{
}


void CDlgCheckStd::OnBnClickedBtnSave()
{
  // TODO: 在此添加控件通知处理程序代码
  m_stdLib->Save();

  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
}


void CDlgCheckStd::OnBnClickedBtnReload()
{
  // TODO: 在此添加控件通知处理程序代码
  UpdateGrid_StdSeries();

  UpdateGrid_StdLib();

  UpdateGrid_StdChecked();
}


void CDlgCheckStd::OnBnClickedBtnStdChecking()
{
  // TODO: 在此添加控件通知处理程序代码
  BOOL bFlag = 0;
  int nValue = GetDlgItemInt(IDC_EDT_DYNNUM, &bFlag, 0);
  if (0 == bFlag)
  {
    AfxMessageBox(_T("error number"));
    return;
  }
  for (int i = 0; i < nValue; i++)
  {
    gstuRun.chStmCmd &= ~(1 << 0);
    g_dlgDevice->EFGCtrl(1);
#ifndef DEBUG_COM
    // 等待测量完成
    while (!(gstuRun.chStmCmd & (1 << 0)))
    {
      Sleep(1);
    }
#endif
    // 读屏幕
    if (g_dlgScreen->RCGVGA())
    {
      CalcEquAngle(g_dlgScreen->degree);//DATE 180421
      gstuSort.sortsn = SortChip(&gstuSort, g_dlgScreen->degree);//得到档位值
      if (gstuSort.sortsn < 0)
        gstuSort.sortsn = 29;

      g_dlgDevice->SortSend(char(gstuSort.sortsn));
      CString _degree;
      _degree.Format(_T("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n"), gstuTrdStat._N, g_dlgScreen->degree[0] * 100000 + g_dlgScreen->degree[1] * 10000 + g_dlgScreen->degree[2] * 1000 + g_dlgScreen->degree[3] * 100 + g_dlgScreen->degree[4] * 10 + g_dlgScreen->degree[5],
        g_dlgScreen->degree[6] * 100000 + g_dlgScreen->degree[7] * 10000 + g_dlgScreen->degree[8] * 1000 + g_dlgScreen->degree[9] * 100 + g_dlgScreen->degree[10] * 10 + g_dlgScreen->degree[11],
        g_dlgScreen->degree[12] * 100000 + g_dlgScreen->degree[13] * 10000 + g_dlgScreen->degree[14] * 1000 + g_dlgScreen->degree[15] * 100 + g_dlgScreen->degree[16] * 10 + g_dlgScreen->degree[17],
        g_dlgScreen->degree[18] * 100000 + g_dlgScreen->degree[19] * 10000 + g_dlgScreen->degree[20] * 1000 + g_dlgScreen->degree[21] * 100 + g_dlgScreen->degree[22] * 10 + g_dlgScreen->degree[23],
        g_dlgScreen->degree[24] * 100000 + g_dlgScreen->degree[25] * 10000 + g_dlgScreen->degree[26] * 1000 + g_dlgScreen->degree[27] * 100 + g_dlgScreen->degree[28] * 10 + g_dlgScreen->degree[29],
        gstuSort.sortsn);

      gclsTxt.TXTAddStr(gstuPathInf.csPathTxt, _degree);

      //流程卡记录：by mmy 171115
      if (csv)
      {
        // CCSVFile csv(gstuPathInf.csPathExe + _T("\\data\\") + card + _T(".csv"), CCSVFile::modeWrite);
        CStringArray arr;
        CString str;
        str.Format(_T("%d"), gstuTrdStat._N);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[0] * 100000 + g_dlgScreen->degree[1] * 10000 + g_dlgScreen->degree[2] * 1000 + g_dlgScreen->degree[3] * 100 + g_dlgScreen->degree[4] * 10 + g_dlgScreen->degree[5]);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[6] * 100000 + g_dlgScreen->degree[7] * 10000 + g_dlgScreen->degree[8] * 1000 + g_dlgScreen->degree[9] * 100 + g_dlgScreen->degree[10] * 10 + g_dlgScreen->degree[11]);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[12] * 100000 + g_dlgScreen->degree[13] * 10000 + g_dlgScreen->degree[14] * 1000 + g_dlgScreen->degree[15] * 100 + g_dlgScreen->degree[16] * 10 + g_dlgScreen->degree[17]);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[18] * 100000 + g_dlgScreen->degree[19] * 10000 + g_dlgScreen->degree[20] * 1000 + g_dlgScreen->degree[21] * 100 + g_dlgScreen->degree[22] * 10 + g_dlgScreen->degree[23]);
        arr.Add(str);
        str.Format(_T("%d"), g_dlgScreen->degree[24] * 100000 + g_dlgScreen->degree[25] * 10000 + g_dlgScreen->degree[26] * 1000 + g_dlgScreen->degree[27] * 100 + g_dlgScreen->degree[28] * 10 + g_dlgScreen->degree[29]);
        arr.Add(str);
        str.Format(_T("%d"), gstuSort.sortsn);
        arr.Add(str);

        csv->WriteData(arr);

        arr.RemoveAll();
      }


      gstuRun.unSort[2] = gstuRun.unSort[1];
      gstuRun.unSort[1] = gstuRun.unSort[0];
      gstuRun.unSort[0] = gstuSort.sortsn + 1;
      QueryPerformanceCounter(&l_lgint_end);
      gstuRun.dRT = double(l_lgint_end.QuadPart - l_lgint_start.QuadPart) / l_lgint_freq.QuadPart;
      l_lgint_start.QuadPart = l_lgint_end.QuadPart;
      gstuTrdStat._N++;
    }
  }
  AfxMessageBox(_T("动态检测完成"));





}


void CDlgCheckStd::OnBnClickedBtnSet()
{
  // TODO: 在此添加控件通知处理程序代码
  CDlgPassword dlg(m_stdLib);
  int ret = dlg.DoModal();

  if (IDOK == ret)
  {
    CDlgStdSet set(m_stdLib);
    ret = set.DoModal();
  }
  else
  {
    AfxMessageBox(_T("密码错误"));
  }
}
