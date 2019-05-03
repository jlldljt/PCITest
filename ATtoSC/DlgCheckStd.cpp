// DlgCheckStd.cpp : 实现文件
//

#include "stdafx.h"
#include "ATtoSC.h"
#include "DlgCheckStd.h"
#include "afxdialogex.h"

#include "DlgPassword.h"
#include "DlgStdSet.h"
#include "Run.h"

// CDlgCheckStd 对话框

IMPLEMENT_DYNAMIC(CDlgCheckStd, CDialogEx)

CDlgCheckStd::CDlgCheckStd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCheckStd::IDD, pParent)
{
  //CList<> list;
  m_stdLib = new CStandardLib(g_path + _T("\\data.db"));
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
  DDX_Control(pDX, IDC_GRID_STD_RESULT, m_gridStdResult);
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
  ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgCheckStd::OnBnClickedBtnClear)
  ON_BN_CLICKED(IDC_BTN_CONFIRM, &CDlgCheckStd::OnBnClickedBtnConfirm)
  ON_BN_CLICKED(IDC_BTN_STD_MODIFY, &CDlgCheckStd::OnBnClickedBtnStdModify)
END_MESSAGE_MAP()


// CDlgCheckStd 消息处理程序


BOOL CDlgCheckStd::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  GetDlgItem(IDC_BTN_STD_MODIFY)->EnableWindow(false);
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

  InitGrid_StdResult();

  UpdateGrid_StdSeries();

  UpdateGrid_StdResult();

  StartFTLong(1); //g_dlgDevice->EFGCut(false);

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
  m_gridStdSeries.SetEditable(FALSE);
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

  //UpdateGrid_StdChecked();

  //UpdateGrid_StdResult();
  OnBnClickedBtnClear();
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
  m_gridStdLib.SetEditable(FALSE);
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
    str.Format(_T("%d"), SEC_TO_USER(m_stdLib->m_checked[i].m_phi));
    m_gridStdChecked.SetItemText(line, 1, str);
    str.Format(_T("%d"), SEC_TO_USER(m_stdLib->m_checked[i].m_laser));
    m_gridStdChecked.SetItemText(line, 2, str);
  }

  m_gridStdChecked.ExpandLastColumn();

  m_gridStdChecked.Refresh();
}

void CDlgCheckStd::ClearGrid_StdChecked(void)
{
  int min = 1;
  int max = m_gridStdChecked.GetRowCount();

  int sort_num = m_stdLib->m_checked.GetCount();

  for (int i = max; i >= min; i--)
  {
    m_gridStdChecked.DeleteRow(i);
  }

  m_gridStdChecked.Refresh();
}
// bool true 不合格 标红
// false 合格
void CDlgCheckStd::AddGrid_StdChecked(CStdLib std, bool laser, bool phi)
{
  int line = -1;

  CString str;

  str.Format(_T("%d"), std.m_no);
  line = m_gridStdChecked.InsertRow(str, -1);
  ASSERT(line != -1);
  str.Format(_T("%d"), SEC_TO_USER(std.m_phi));
  m_gridStdChecked.SetItemText(line, 1, str);
  str.Format(_T("%d"), SEC_TO_USER(std.m_laser));
  m_gridStdChecked.SetItemText(line, 2, str);

  if (laser)
    m_gridStdChecked.GetCell(line, 2)->SetBackClr(RGB(255, 0, 0));
  if (phi)
    m_gridStdChecked.GetCell(line, 1)->SetBackClr(RGB(255, 0, 0));

  m_gridStdChecked.ExpandLastColumn();

  m_gridStdChecked.Refresh();

  UpdateGrid_StdResult();
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

void CDlgCheckStd::InitGrid_StdResult(void)
{
  m_gridStdResult.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//黄色背景
  m_gridStdResult.SetRowCount(1); //初始为2行

  //for (int i = 1; i < 2; i++)
  //{
  //  m_gridStdChecked.SetRowHeight(i, 30);
  //}

  m_gridStdResult.SetColumnCount(3); //初始化为3列
  m_gridStdResult.SetFixedRowCount(1); //表头为1行
  m_gridStdResult.SetFixedColumnCount(1); //表头为1列
  m_gridStdResult.SetRowHeight(0, 30); //设置各行高         

  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("项目"));
  Item.row = 0;
  Item.col = 0;
  m_gridStdResult.SetItem(&Item);

  Item.strText.Format(_T("电"));
  Item.row = 0;
  Item.col = 1;
  m_gridStdResult.SetItem(&Item);

  Item.strText.Format(_T("光"));
  Item.row = 0;
  Item.col = 2;
  m_gridStdResult.SetItem(&Item);

  m_gridStdResult.InsertRow(_T("所选标准片中心"), -1);
  m_gridStdResult.InsertRow(_T("对标结果中心"), -1);
  m_gridStdResult.InsertRow(_T("差值"), -1);

  CRect cRect;
  GetDlgItem(IDC_GRID_STD_LIB)->GetClientRect(&cRect);
  m_gridStdResult.SetColumnWidth(0, cRect.Width() / 3 * 1); //设置0列宽 
  m_gridStdResult.SetColumnWidth(1, cRect.Width() / 3 * 1); //设置0列宽 
  m_gridStdResult.SetColumnWidth(2, cRect.Width() / 3 * 1); //设置0列宽 

  m_gridStdResult.ExpandLastColumn();
  m_gridStdResult.SetColumnResize(FALSE);
  m_gridStdResult.SetRowResize(FALSE);
  m_gridStdResult.SetSingleRowSelection(TRUE);
  m_gridStdResult.SetEditable(FALSE);
  //SetAutoSizeStyle
  //m_gridStdSeries.EnableScrollBar();
 // m_gridStdSeries.EnableScrollBarCtrl();
  m_gridStdResult.ShowBar(SB_HORZ, FALSE);
}

void CDlgCheckStd::UpdateGrid_StdResult(void)
{
  CString str;

  str.Format(_T("%d"), SEC_TO_USER(m_stdLib->m_result.m_checking_phi.m_avg));
  m_gridStdResult.SetItemText(1, 1, str); 
  str.Format(_T("%d"), SEC_TO_USER(m_stdLib->m_result.m_checking_laser.m_avg));
  m_gridStdResult.SetItemText(1, 2, str);
  str.Format(_T("%d"), SEC_TO_USER(m_stdLib->m_result.m_checked_phi.m_avg));
  m_gridStdResult.SetItemText(2, 1, str);
  str.Format(_T("%d"), SEC_TO_USER(m_stdLib->m_result.m_checked_laser.m_avg));
  m_gridStdResult.SetItemText(2, 2, str);
  str.Format(_T("%d"), SEC_TO_USER(m_stdLib->m_result.m_dPhi));
  m_gridStdResult.SetItemText(3, 1, str);
  str.Format(_T("%d"), SEC_TO_USER(m_stdLib->m_result.m_dLaser));
  m_gridStdResult.SetItemText(3, 2, str);

  if (m_stdLib->m_result.m_dLaser > m_stdLib->m_set.m_maxd_laser)
      m_gridStdResult.GetCell(3, 2)->SetBackClr(RGB(255, 0, 0));
  else
	  m_gridStdResult.GetCell(3, 2)->SetBackClr(RGB(255, 255, 255));

  if (m_stdLib->m_result.m_dPhi> m_stdLib->m_set.m_maxd_phi)
      m_gridStdResult.GetCell(3, 1)->SetBackClr(RGB(255, 0, 0));
  else
	  m_gridStdResult.GetCell(3, 1)->SetBackClr(RGB(255, 255, 255));

  m_gridStdResult.ExpandLastColumn();

  m_gridStdResult.Refresh();
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

  //UpdateGrid_StdChecked();
}


void CDlgCheckStd::OnBnClickedBtnStdChecking()
{
  // TODO: 在此添加控件通知处理程序代码
  CStdLib checked, checking;

  CCellRange sel = m_gridStdLib.GetSelectedCellRange();

  int num = m_gridStdLib.GetRowCount();

  int row = sel.GetMinRow();

  checking.m_no = _wtoi(m_gridStdLib.GetItemText(row, 0));
  checking.m_phi = USER_TO_SEC(_wtoi(m_gridStdLib.GetItemText(row, 1)));
  checking.m_laser = USER_TO_SEC(_wtoi(m_gridStdLib.GetItemText(row, 2)));

  if (m_stdLib->IsChecked(checking.m_no))
  {
	  AfxMessageBox(_T("该片已经测过了"));
    return;
  }


  if (row <= 0 && row >= num /*|| checking.m_phi == 0 || checking.m_laser == 0*/)
    return;

  CStdResult result;

  //g_dlgDevice->EFGCut(true);
  StartFTLong(1);

  for (int i = 0; i < m_stdLib->m_set.m_test_cnt; i++)
  {
    //gstuRun.chStmCmd &= ~(1 << 0);
	//gstuRun.chStmCmd0 = 0;
    //g_dlgDevice->EFGCtrl(1);

#if 1
    // 等待测量完成
	  WaitVGAEnd();
	//int cnt = 0;
 //   while (!gstuRun.chStmCmd0)
 //   {
 //     Sleep(100);
	//  //if(cnt++ > 100)
	//  //{
	//	 // //AfxMessageBox(_T("该片已经测过了"),MB_OKCANCEL);
	//	 // g_dlgDevice->EFGCut(true);
	//  //}
 //   }

	//if(cnt > 100)
 //     g_dlgDevice->EFGCut(false);

    // 读屏幕
    if (RCGVGA())
    {
		int* degree = degree;//g_dlgScreen->degree;

        checked.m_no = checking.m_no;
        checked.m_laser = (degree[0] * 10 + degree[1]) * 3600 + (degree[2] * 10 + degree[3]) * 60 + (degree[4] * 10 + degree[5])/* + g_sort.R1t[g_sort.R1Num]*/;//光轴//_0614
        checked.m_phi = (degree[6] * 10 + degree[7]) * 3600 + (degree[8] * 10 + degree[9]) * 60 + (degree[10] * 10 + degree[11])/* + g_sort.R1e[g_sort.R1Num]*/;//电轴

      

    }
    else
    {
      AfxMessageBox(_T("读屏失败！"));
    }
#else
    checked = checking;

#endif

    result.Add(checking, checked);
	Sleep(100);
  }
  
  //g_dlgDevice->EFGCtrl(false);
  StartFTLong(0);

  checking.m_laser = result.m_checking_laser.m_avg;
  checking.m_phi = result.m_checking_phi.m_avg;
  checked.m_laser = result.m_checked_laser.m_avg;
  checked.m_phi = result.m_checked_phi.m_avg;


    UCHAR ret = m_stdLib->AddCheck(checking, checked);

    AddGrid_StdChecked(checked, ret&0x01, ret&0x02);
    




}


void CDlgCheckStd::OnBnClickedBtnSet()
{
  // TODO: 在此添加控件通知处理程序代码
  CDlgPassword dlg(m_stdLib);
  int ret = dlg.DoModal();

  if (IDOK == ret)
  {
	  GetDlgItem(IDC_BTN_STD_MODIFY)->EnableWindow(true);
	  m_gridStdLib.SetEditable(TRUE);
	  m_gridStdSeries.SetEditable(TRUE);
    CDlgStdSet set(m_stdLib);
    ret = set.DoModal();
  }
  else
  {
    AfxMessageBox(_T("密码错误"));
  }
}


void CDlgCheckStd::OnBnClickedBtnClear()
{
  // TODO: 在此添加控件通知处理程序代码
  m_stdLib->ClearCheck();
  //ClearGrid_StdChecked();
  UpdateGrid_StdChecked();
  UpdateGrid_StdResult();
}


void CDlgCheckStd::OnBnClickedBtnConfirm()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_stdLib->m_checked.GetCount() < m_stdLib->m_set.m_min_test_num)
	{
		AfxMessageBox(_T("对标片数不够！"));
		return;
	}
	CString msg;
	// 记录对标数据
	CString str, str_tmp;
	CTime time;
	time = CTime::GetCurrentTime();
	str += time.Format("\r\n%Y/%m/%d");
	str += time.Format("  %H:%M:%S");
	str += time.Format("\r\n编号\t标准电轴\t标准光轴\t实测电轴\t实测光轴");
	str += time.Format("\r\n--------------------------------------------------------------");
	if(m_stdLib->m_checking.GetCount() == m_stdLib->m_checked.GetCount())
	{
		for(int i = 0; i < m_stdLib->m_checked.GetCount(); i++)
		{
			CStdLib checked = m_stdLib->m_checked[i];
			CStdLib checking = m_stdLib->GetStdChecking(checked.m_no);
			if(checking.m_no == checked.m_no)
			{
				str_tmp.Format(_T("\r\n%d\t%d\t%d\t%d\t%d\t")
					, checking.m_no, SEC_TO_USER(checking.m_phi), SEC_TO_USER(checking.m_laser)
					, SEC_TO_USER(checked.m_phi), SEC_TO_USER(checked.m_laser));
				str += str_tmp;
			}
		}


	}

	if (m_stdLib->m_result.m_dLaser <= m_stdLib->m_set.m_maxd_laser
		&& m_stdLib->m_result.m_dPhi <= m_stdLib->m_set.m_maxd_phi
		&& m_stdLib->pass_num >= m_stdLib->m_set.m_min_lib_num)
	{
		// 通过
		str += time.Format("\r\n========================通过==================================\r\n");

		gclsTxt.TXTAddStr(g_path + _T("\\对标记录.txt"),str);

		AfxMessageBox(_T("通过！"));

		EndDialog(101);
	}
	else
	{
		str += time.Format("\r\n========================未通过================================\r\n");

		gclsTxt.TXTAddStr(g_path + _T("\\对标记录.txt"),str);

		AfxMessageBox(_T("未通过！"));
	}
}


void CDlgCheckStd::OnBnClickedBtnStdModify()
{
  // TODO: 在此添加控件通知处理程序代码
  CCellRange sel = m_gridStdChecked.GetSelectedCellRange();

  int row = sel.GetMinRow();
 
  int num = m_gridStdLib.GetRowCount();
 
  if (row <= 0 && row >= num)
    return;

  int no = _ttoi(m_gridStdChecked.GetItemText(row, 0));

  for (int i = 1; i < num; i++)
  {
    int tmp = _ttoi(m_gridStdLib.GetItemText(i, 0));
    
    if (tmp == no)
    {
      m_gridStdLib.SetItemText(i,1,m_gridStdChecked.GetItemText(row, 1));
      m_gridStdLib.SetItemText(i,2,m_gridStdChecked.GetItemText(row, 2));

	  CStdLib std;

	  std.m_no = _wtoi(m_gridStdChecked.GetItemText(row, 0));
	  std.m_phi = _wtoi(m_gridStdChecked.GetItemText(row, 1));
	  std.m_laser = _wtoi(m_gridStdChecked.GetItemText(row, 2));

	  m_stdLib->SetStd(std);

	  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);

	  break;
    }
  }



}
