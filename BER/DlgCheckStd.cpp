// DlgCheckStd.cpp : 实现文件
//

#include "stdafx.h"
#include "BER.h"
#include "DlgCheckStd.h"
#include "afxdialogex.h"




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

  ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgCheckStd::OnBnClickedBtnSave)
  ON_BN_CLICKED(IDC_BTN_RELOAD, &CDlgCheckStd::OnBnClickedBtnReload)
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

  UpdateGrid_StdSeries();

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}

void CDlgCheckStd::InitGrid_StdSeries(void)
{
  m_seriesRowSel = -1;

  m_gridStdSeries.SetTextBkColor(RGB(0xCC, 0xFF, 0xCC));//背景
  m_gridStdSeries.SetRowCount(2); //初始为2行

  for (int i = 1; i < 2; i++)
  {
    m_gridStdSeries.SetRowHeight(i, 30);
  }

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
  m_gridStdLib.SetRowCount(2); //初始为2行

  for (int i = 1; i < 2; i++)
  {
    m_gridStdLib.SetRowHeight(i, 30);
  }

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
}
