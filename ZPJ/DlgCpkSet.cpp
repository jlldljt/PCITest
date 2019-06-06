// DlgCpkSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZPJ.h"
#include "DlgCpkSet.h"
#include "afxdialogex.h"


// CDlgCpkSet �Ի���

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
  m_gridCpkSet.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//��ɫ����
  m_gridCpkSet.SetRowCount(2); //��ʼΪ2��

  for (int i = 1; i < 2; i++)
  {
    m_gridCpkSet.SetRowHeight(i, 30);
  }

  m_gridCpkSet.SetColumnCount(2); //��ʼ��Ϊ3��
  m_gridCpkSet.SetFixedRowCount(1); //��ͷΪ1��
  m_gridCpkSet.SetFixedColumnCount(1); //��ͷΪ1��
  m_gridCpkSet.SetRowHeight(0, 30); //���ø��и�         

  GV_ITEM Item;
  Item.mask = GVIF_TEXT | GVIF_FORMAT;
  Item.nFormat = DT_CENTER | DT_WORDBREAK;
  Item.strText.Format(_T("��Ŀ"));
  Item.row = 0;
  Item.col = 0;
  m_gridCpkSet.SetItem(&Item);

  Item.strText.Format(_T("ֵ"));
  Item.row = 0;
  Item.col = 1;
  m_gridCpkSet.SetItem(&Item);

  CRect cRect;
  GetDlgItem(IDC_GRID_CPK_SET)->GetClientRect(&cRect);
  m_gridCpkSet.SetColumnWidth(0, cRect.Width() / 2 * 1); //����0�п� 
  m_gridCpkSet.SetColumnWidth(1, cRect.Width() / 2 * 1); //����0�п� 

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

  m_gridCpkSet.InsertRow(_T("CP����"), -1);
  m_gridCpkSet.InsertRow(_T("CA����"), -1);
  m_gridCpkSet.InsertRow(_T("CPK����"), -1);
  m_gridCpkSet.InsertRow(_T("��ʼ����"), -1);

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
    AfxMessageBox(_T("ֻ�����������ͣ�3λ��С��"));
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


// CDlgCpkSet ��Ϣ�������


void CDlgCpkSet::OnBnClickedBtnSave()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  m_cpk->SaveSet();

  GetDlgItem(IDC_BTN_SAVE)->EnableWindow(FALSE);
}


void CDlgCpkSet::OnBnClickedBtnReload()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  UpdateGrid_CpkSet();
}


BOOL CDlgCpkSet::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  �ڴ���Ӷ���ĳ�ʼ��
  InitGrid_CpkSet();

  UpdateGrid_CpkSet();

  return TRUE;  // return TRUE unless you set the focus to a control
                // �쳣: OCX ����ҳӦ���� FALSE
}
