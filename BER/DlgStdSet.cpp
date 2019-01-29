// DlgStdSet.cpp : 实现文件
//

#include "stdafx.h"
#include "BER.h"
#include "DlgStdSet.h"
#include "afxdialogex.h"


// CDlgStdSet 对话框

IMPLEMENT_DYNAMIC(CDlgStdSet, CDialogEx)

CDlgStdSet::CDlgStdSet(CStandardLib * pLib, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStdSet::IDD, pParent)
{
	ASSERT(pLib);
  
	m_pLib = pLib;
}

CDlgStdSet::~CDlgStdSet()
{
}

void CDlgStdSet::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID_STD_SET, m_gridStdSet);
}

void CDlgStdSet::InitGrid_StdSet(void)
{
  m_gridStdSet.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//黄色背景
  m_gridStdSet.SetRowCount(2); //初始为2行

  for (int i = 1; i < 2; i++)
  {
    m_gridStdSet.SetRowHeight(i, 30);
  }

  m_gridStdSet.SetColumnCount(2); //初始化为3列
  m_gridStdSet.SetFixedRowCount(1); //表头为1行
  m_gridStdSet.SetFixedColumnCount(1); //表头为1列
  m_gridStdSet.SetRowHeight(0, 30); //设置各行高         

  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("类目"));
  Item.row = 0;
  Item.col = 0;
  m_gridStdSet.SetItem(&Item);

  Item.strText.Format(_T("值"));
  Item.row = 0;
  Item.col = 1;
  m_gridStdSet.SetItem(&Item);

  CRect cRect;
  GetDlgItem(IDC_GRID_STD_SET)->GetClientRect(&cRect);
  m_gridStdSet.SetColumnWidth(0, cRect.Width() / 2 * 1); //设置0列宽 
  m_gridStdSet.SetColumnWidth(1, cRect.Width() / 2 * 1); //设置0列宽 

  m_gridStdSet.ExpandLastColumn();
  m_gridStdSet.SetColumnResize(FALSE);
  m_gridStdSet.SetRowResize(FALSE);
  //m_gridStdSet.SetSingleRowSelection(TRUE);
  //m_gridStdSet.SetEditable(FALSE);
  //SetAutoSizeStyle
  //m_gridStdSeries.EnableScrollBar();
 // m_gridStdSeries.EnableScrollBarCtrl();
  m_gridStdSet.ShowBar(SB_HORZ, FALSE);
}

void CDlgStdSet::UpdateGrid_StdSet(void)
{
  int min = 1;

  int max = m_gridStdSet.GetRowCount();

 m_pLib->m_set;

  for (int i = max; i >= min; i--)
  {
    m_gridStdSet.DeleteRow(i);
  }

  CString str;

  m_gridStdSet.InsertRow(_T("电轴最大差值"), -1);
  m_gridStdSet.InsertRow(_T("光轴最大差值"), -1);
  m_gridStdSet.InsertRow(_T("库最小片数"), -1);
  m_gridStdSet.InsertRow(_T("对标测量次数"), -1);
  m_gridStdSet.InsertRow(_T("最小对标片数"), -1);
  m_gridStdSet.InsertRow(_T("密码"), -1);

  str.Format(_T("%d"), m_pLib->m_set.m_maxd_phi);
  m_gridStdSet.SetItemText(1, 1, str);
  str.Format(_T("%d"), m_pLib->m_set.m_maxd_laser);
  m_gridStdSet.SetItemText(2, 1, str);
  str.Format(_T("%d"), m_pLib->m_set.m_min_lib_num);
  m_gridStdSet.SetItemText(3, 1, str);
  str.Format(_T("%d"), m_pLib->m_set.m_test_cnt);
  m_gridStdSet.SetItemText(4, 1, str);
  str.Format(_T("%d"), m_pLib->m_set.m_min_test_num);
  m_gridStdSet.SetItemText(5, 1, str);
  m_gridStdSet.SetItemText(6, 1, m_pLib->m_set.m_password);

  m_gridStdSet.ExpandLastColumn();

  m_gridStdSet.Refresh();
}

void CDlgStdSet::OnBeginLabelEdit_StdSet(NMHDR * pNMHDR, LRESULT * pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
  int col = pItem->iColumn;
  int row = pItem->iRow;
  m_prevEditStr = m_gridStdSet.GetItemText(row, col);
}

void CDlgStdSet::OnEndLabelEdit_StdSet(NMHDR * pNMHDR, LRESULT * pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;

  int col = pItem->iColumn;
  int row = pItem->iRow;

  CString text = m_gridStdSet.GetItemText(row, col);
  int val = _wtoi(text);

  CString csTemp;
  csTemp.Format(_T("%d"), val);

  if (text != csTemp && row != 6)
  {
    AfxMessageBox(_T("只能输入整数"));
    m_gridStdSet.SetItemText(row, col, m_prevEditStr);
    ::PostMessage(GetDlgItem(IDC_GRID_STD_LIB)->GetSafeHwnd(), WM_LBUTTONUP, NULL, 0);
    return;
  }

  m_pLib->m_set.m_maxd_phi = _wtoi(m_gridStdSet.GetItemText(1, 1));
  m_pLib->m_set.m_maxd_laser = _wtoi(m_gridStdSet.GetItemText(2, 1));
  m_pLib->m_set.m_min_lib_num = _wtoi(m_gridStdSet.GetItemText(3, 1));
  m_pLib->m_set.m_test_cnt = _wtoi(m_gridStdSet.GetItemText(4, 1));
  m_pLib->m_set.m_min_test_num = _wtoi(m_gridStdSet.GetItemText(5, 1));
  m_pLib->m_set.m_password = m_gridStdSet.GetItemText(6, 1);

  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
}

void CDlgStdSet::OnSelChanged_StdSet(NMHDR * pNMHDR, LRESULT * pResult)
{
}

void CDlgStdSet::OnEndScroll_StdSet(NMHDR * pNMHDR, LRESULT * pResult)
{
}


BEGIN_MESSAGE_MAP(CDlgStdSet, CDialogEx)
  ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgStdSet::OnBnClickedBtnSave)
  ON_BN_CLICKED(IDC_BTN_RELOAD, &CDlgStdSet::OnBnClickedBtnReload)

  ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID_STD_SET, OnBeginLabelEdit_StdSet)
  ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_STD_SET, OnEndLabelEdit_StdSet)
  ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_STD_SET, OnSelChanged_StdSet)
  ON_NOTIFY(GVN_ENDSCROLL, IDC_GRID_STD_SET, OnEndScroll_StdSet)

END_MESSAGE_MAP()


// CDlgStdSet 消息处理程序


void CDlgStdSet::OnBnClickedBtnSave()
{
  // TODO: 在此添加控件通知处理程序代码
  m_pLib->SaveSet();

  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
}

void CDlgStdSet::OnBnClickedBtnReload()
{
  // TODO: 在此添加控件通知处理程序代码
  UpdateGrid_StdSet();
}


BOOL CDlgStdSet::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  InitGrid_StdSet();

  UpdateGrid_StdSet();

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}
