// DlgCpk.cpp : 实现文件
//

#include "stdafx.h"
#include "BER.h"
#include "DlgCpk.h"
#include "afxdialogex.h"

#include "DlgCpkSet.h"
// CDlgCpk 对话框

IMPLEMENT_DYNAMIC(CDlgCpk, CDialogEx)

	CDlgCpk::CDlgCpk(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCpk::IDD, pParent)
{
	m_pCpk = new CCpkLib(gstuPathInf.csPathExe);
}

CDlgCpk::~CDlgCpk()
{
	delete m_pCpk;
}

void CDlgCpk::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_PLANNED_NO, m_gridPlannedNo);
	DDX_Control(pDX, IDC_GRID_PROCESS_CARD_NO, m_gridProcessCardNo);
}

void CDlgCpk::InitGrid_PlannedNo(void)
{
	m_gridPlannedNo.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//背景
	m_gridPlannedNo.SetRowCount(1); //初始为1行

	m_gridPlannedNo.SetColumnCount(1); //初始化为1列
	m_gridPlannedNo.SetFixedRowCount(1); //表头为1行
	m_gridPlannedNo.SetFixedColumnCount(0); //表头为0列
	m_gridPlannedNo.SetRowHeight(0, 30); //设置各行高         

	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_WORDBREAK;
	Item.strText.Format(_T("计划号"));
	Item.row = 0;
	Item.col = 0;
	m_gridPlannedNo.SetItem(&Item);

	CRect cRect;
	GetDlgItem(IDC_GRID_PLANNED_NO)->GetClientRect(&cRect);
	m_gridPlannedNo.SetColumnWidth(0, cRect.Width()); //设置0列宽 

	m_gridPlannedNo.ExpandLastColumn();
	m_gridPlannedNo.SetColumnResize(FALSE);
	m_gridPlannedNo.SetRowResize(FALSE);
	m_gridPlannedNo.SetSingleRowSelection(TRUE);
	m_gridPlannedNo.SetEditable(FALSE);
	//SetAutoSizeStyle
	//m_gridStdSeries.EnableScrollBar();
	// m_gridStdSeries.EnableScrollBarCtrl();
	m_gridPlannedNo.ShowBar(SB_HORZ, FALSE);
}

void CDlgCpk::UpdateGrid_PlannedNo(void)
{
	int min = 1;
	int max = m_gridPlannedNo.GetRowCount();

	int sort_num = m_pCpk->m_arrPlannedNo.GetCount();

	for (int i = max; i >= min; i--)
	{
		m_gridPlannedNo.DeleteRow(i);
	}
	int line = 1;

	CString str;

	for (int i = 0; i < sort_num; i++, line++)
	{
		m_gridPlannedNo.InsertRow(m_pCpk->m_arrPlannedNo[i], -1);
	}

	m_gridPlannedNo.ExpandLastColumn();

	m_gridPlannedNo.Refresh();
}

void CDlgCpk::OnSelChanged_PlannedNo(NMHDR * pNMHDR, LRESULT * pResult)
{
	CCellRange sel = m_gridPlannedNo.GetSelectedCellRange();

	int row = sel.GetMinRow();

	CString text = m_gridPlannedNo.GetItemText(row, 0);

	if (m_pCpk->LoadProcessCardNo(text))
	{
		//刷新库
		UpdateGrid_ProcessCardNo();
	}
}


BEGIN_MESSAGE_MAP(CDlgCpk, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SET, &CDlgCpk::OnBnClickedBtnSet)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_PLANNED_NO, OnSelChanged_PlannedNo)
	ON_BN_CLICKED(IDC_BTN_TEST, &CDlgCpk::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CDlgCpk 消息处理程序


void CDlgCpk::OnBnClickedBtnSet()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgCpkSet dlg(m_pCpk);
	int ret = dlg.DoModal();
}

void CDlgCpk::InitGrid_ProcessCardNo(void)
{
	m_gridProcessCardNo.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//背景
	m_gridProcessCardNo.SetRowCount(1); //初始为1行

	m_gridProcessCardNo.SetColumnCount(9); //初始化为9列
	m_gridProcessCardNo.SetFixedRowCount(1); //表头为1行
	m_gridProcessCardNo.SetFixedColumnCount(1); //表头为1列
	m_gridProcessCardNo.SetRowHeight(0, 30); //设置各行高         

	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_WORDBREAK;
	Item.strText.Format(_T("流程卡"));
	Item.row = 0;
	Item.col = 0;
	m_gridProcessCardNo.SetItem(&Item); 
	Item.strText.Format(_T("中心"));
	Item.col = 1;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("上限"));
	Item.col = 2;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("下限"));
	Item.col = 3;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("均值"));
	Item.col = 4;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("散差"));
	Item.col = 5;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("CP"));
	Item.col = 6;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("CA"));
	Item.col = 7;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("CPK"));
	Item.col = 8;
	m_gridProcessCardNo.SetItem(&Item);

	CRect cRect;
	GetDlgItem(IDC_GRID_PROCESS_CARD_NO)->GetClientRect(&cRect);
	m_gridProcessCardNo.SetColumnWidth(0, cRect.Width() / 10 * 2); //设置0列宽 
	m_gridProcessCardNo.SetColumnWidth(1, cRect.Width() / 10 * 1); //设置0列宽 
	m_gridProcessCardNo.SetColumnWidth(2, cRect.Width() / 10 * 1); //设置0列宽 
	m_gridProcessCardNo.SetColumnWidth(3, cRect.Width() / 10 * 1); //设置0列宽 
	m_gridProcessCardNo.SetColumnWidth(4, cRect.Width() / 10 * 1); //设置0列宽 
	m_gridProcessCardNo.SetColumnWidth(5, cRect.Width() / 10 * 1); //设置0列宽 
	m_gridProcessCardNo.SetColumnWidth(6, cRect.Width() / 10 * 1); //设置0列宽 
	m_gridProcessCardNo.SetColumnWidth(7, cRect.Width() / 10 * 1); //设置0列宽 
	m_gridProcessCardNo.SetColumnWidth(8, cRect.Width() / 10 * 1); //设置0列宽 

	m_gridProcessCardNo.ExpandLastColumn();
	m_gridProcessCardNo.SetColumnResize(FALSE);
	m_gridProcessCardNo.SetRowResize(FALSE);
	m_gridProcessCardNo.SetSingleRowSelection(TRUE);
	m_gridProcessCardNo.SetEditable(FALSE);
	//SetAutoSizeStyle
	//m_gridStdSeries.EnableScrollBar();
	// m_gridStdSeries.EnableScrollBarCtrl();
	m_gridProcessCardNo.ShowBar(SB_HORZ, FALSE);
}

void CDlgCpk::UpdateGrid_ProcessCardNo(void)
{
	int min = 1;
	int max = m_gridProcessCardNo.GetRowCount();

	int sort_num = m_pCpk->m_arrProcessCardNo.GetCount();

	for (int i = max; i >= min; i--)
	{
		m_gridProcessCardNo.DeleteRow(i);
	}
	int line = 1;

	CString str;

	for (int i = 0; i < sort_num; i++, line++)
	{
		m_gridProcessCardNo.InsertRow(m_pCpk->m_arrProcessCardNo[i].no, -1);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].center);
		m_gridProcessCardNo.SetItemText(line, 1, str);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].usl);
		m_gridProcessCardNo.SetItemText(line, 2, str);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].lsl);
		m_gridProcessCardNo.SetItemText(line, 3, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].avg);
		m_gridProcessCardNo.SetItemText(line, 4, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].std);
		m_gridProcessCardNo.SetItemText(line, 5, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].cp);
		m_gridProcessCardNo.SetItemText(line, 6, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].ca);
		m_gridProcessCardNo.SetItemText(line, 7, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].cpk);
		m_gridProcessCardNo.SetItemText(line, 8, str);
	}

	m_gridProcessCardNo.ExpandLastColumn();

	m_gridProcessCardNo.Refresh();
}


BOOL CDlgCpk::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pCpk->LoadPlannedNo();

	InitGrid_PlannedNo();

	InitGrid_ProcessCardNo();

	UpdateGrid_PlannedNo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCpk::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CString test1,test2;
	GetDlgItemText(IDC_EDT_TEST1, test1);
	GetDlgItemText(IDC_EDT_TEST2, test2);

	if (!m_pCpk->OpenProcessCard(test1, test2))
	{
		AfxMessageBox(_T("创建流程卡有错误，是否重复"));
		return;
	}

	CProcessCard processCard;
	processCard.avg = 100;
	processCard.ca = 1;
	processCard.center = 100;
	processCard.cp = 1;
	processCard.cpk = 1;
	processCard.axis = "光轴";
	processCard.lsl = 1;
	processCard.no = "12345";
	processCard.std = 100;
	processCard.usl = 5;

	//for (int i = 0; i < 1; i++)
	//{
		m_pCpk->AddToPlannedCsv(processCard);
		m_pCpk->AddCpkToPlannedCsv(processCard);
	//}


	CProcessData processData;
	processData.equal = 100;
	processData.laser = 100;
	processData.laser0 = 100;
	processData.no = 2;
	processData.phi = 100;
	processData.phi0 = 100;
	processData.sort = 8;

	for (int i = 0; i < 3; i++)
	{
		m_pCpk->AddToProcessCardCsv(processData);
	}
}
