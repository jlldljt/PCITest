// DlgCpkSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ZPJ.h"
#include "DlgCpkSet.h"
#include "afxdialogex.h"


// CDlgCpkSet 对话框

IMPLEMENT_DYNAMIC(CDlgCpkSet, CDialogEx)

CDlgCpkSet::CDlgCpkSet(CCpkLib * pLib, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCpkSet::IDD, pParent)
{
  ASSERT(pLib);

  m_cpk = pLib;
}

CDlgCpkSet::~CDlgCpkSet()
{
}

void CDlgCpkSet::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID_CPK_SET, m_gridCpkSet);
}

void CDlgCpkSet::InitGrid_CpkSet(void)
{
  m_gridCpkSet.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//黄色背景
  m_gridCpkSet.SetRowCount(2); //初始为2行

  for (int i = 1; i < 2; i++)
  {
    m_gridCpkSet.SetRowHeight(i, 30);
  }

  m_gridCpkSet.SetColumnCount(2); //初始化为3列
  m_gridCpkSet.SetFixedRowCount(1); //表头为1行
  m_gridCpkSet.SetFixedColumnCount(1); //表头为1列
  m_gridCpkSet.SetRowHeight(0, 30); //设置各行高         

  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("类目"));
  Item.row = 0;
  Item.col = 0;
  m_gridCpkSet.SetItem(&Item);

  Item.strText.Format(_T("值"));
  Item.row = 0;
  Item.col = 1;
  m_gridCpkSet.SetItem(&Item);

  CRect cRect;
  GetDlgItem(IDC_GRID_CPK_SET)->GetClientRect(&cRect);
  m_gridCpkSet.SetColumnWidth(0, cRect.Width() / 2 * 1); //设置0列宽 
  m_gridCpkSet.SetColumnWidth(1, cRect.Width() / 2 * 1); //设置0列宽 

  m_gridCpkSet.ExpandLastColumn();
  m_gridCpkSet.SetColumnResize(FALSE);
  m_gridCpkSet.SetRowResize(FALSE);
  //m_gridStdSet.SetSingleRowSelection(TRUE);
  //m_gridStdSet.SetEditable(FALSE);
  //SetAutoSizeStyle
  //m_gridStdSeries.EnableScrollBar();
 // m_gridStdSeries.EnableScrollBarCtrl();
  m_gridCpkSet.ShowBar(SB_HORZ, FALSE);
}

void CDlgCpkSet::UpdateGrid_CpkSet(void)
{
  int min = 1;

  int max = m_gridCpkSet.GetRowCount();

  for (int i = max; i >= min; i--)
  {
    m_gridCpkSet.DeleteRow(i);
  }

  CString str;

  m_gridCpkSet.InsertRow(_T("CP下限"), -1);
  m_gridCpkSet.InsertRow(_T("CA上限"), -1);
  m_gridCpkSet.InsertRow(_T("CPK下限"), -1);
  m_gridCpkSet.InsertRow(_T("起始数量"), -1);

  str.Format(_T("%.3f"), m_cpk->m_cpkSet.m_min_cp);
  m_gridCpkSet.SetItemText(1, 1, str);
  str.Format(_T("%.3f"), m_cpk->m_cpkSet.m_max_ca);
  m_gridCpkSet.SetItemText(2, 1, str);
  str.Format(_T("%.3f"), m_cpk->m_cpkSet.m_min_cpk);
  m_gridCpkSet.SetItemText(3, 1, str);
  str.Format(_T("%d"), m_cpk->m_cpkSet.m_start_num);
  m_gridCpkSet.SetItemText(4, 1, str);

  m_gridCpkSet.ExpandLastColumn();

  m_gridCpkSet.Refresh();
}

void CDlgCpkSet::OnBeginLabelEdit_CpkSet(NMHDR * pNMHDR, LRESULT * pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
  int col = pItem->iColumn;
  int row = pItem->iRow;
  m_prevEditStr = m_gridCpkSet.GetItemText(row, col);
}

void CDlgCpkSet::OnEndLabelEdit_CpkSet(NMHDR * pNMHDR, LRESULT * pResult)
{
  NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;

  int col = pItem->iColumn;
  int row = pItem->iRow;

  CString text = m_gridCpkSet.GetItemText(row, col);
  double val = _ttof(text);

  CString icsTemp,fcsTemp;
  fcsTemp.Format(_T("%.3f"), val);
  icsTemp.Format(_T("%.0f"), val);

  if (text != fcsTemp&& text != icsTemp)
  {
    AfxMessageBox(_T("只能输入整数和（3位）小数"));
    m_gridCpkSet.SetItemText(row, col, m_prevEditStr);
    ::PostMessage(GetDlgItem(IDC_GRID_CPK_SET)->GetSafeHwnd(), WM_LBUTTONUP, NULL, 0);
    return;
  }

  m_cpk->m_cpkSet.m_min_cp = _ttof(m_gridCpkSet.GetItemText(1, 1));
  m_cpk->m_cpkSet.m_max_ca = _ttof(m_gridCpkSet.GetItemText(2, 1));
  m_cpk->m_cpkSet.m_min_cpk = _ttof(m_gridCpkSet.GetItemText(3, 1));
  m_cpk->m_cpkSet.m_start_num = _ttoi(m_gridCpkSet.GetItemText(4, 1));

  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(TRUE);
}


BEGIN_MESSAGE_MAP(CDlgCpkSet, CDialogEx)
  ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgCpkSet::OnBnClickedBtnSave)
  ON_BN_CLICKED(IDC_BTN_RELOAD, &CDlgCpkSet::OnBnClickedBtnReload)

  ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_GRID_CPK_SET, OnBeginLabelEdit_CpkSet)
  ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID_CPK_SET, OnEndLabelEdit_CpkSet)
END_MESSAGE_MAP()


// CDlgCpkSet 消息处理程序


void CDlgCpkSet::OnBnClickedBtnSave()
{
  // TODO: 在此添加控件通知处理程序代码
  m_cpk->SaveSet();

  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
}


void CDlgCpkSet::OnBnClickedBtnReload()
{
  // TODO: 在此添加控件通知处理程序代码
  UpdateGrid_CpkSet();
}


BOOL CDlgCpkSet::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  在此添加额外的初始化
  InitGrid_CpkSet();

  UpdateGrid_CpkSet();

  return TRUE;  // return TRUE unless you set the focus to a control
                // 异常: OCX 属性页应返回 FALSE
}
