// DlgCpk.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BER.h"
#include "DlgCpk.h"
#include "afxdialogex.h"

#include "DlgCpkSet.h"
// CDlgCpk �Ի���

IMPLEMENT_DYNAMIC(CDlgCpk, CDialogEx)

	CDlgCpk::CDlgCpk(CCpkLib *cpk, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCpk::IDD, pParent)
{
	m_pCpk = cpk;//new CCpkLib(gstuPathInf.csPathExe);
}

CDlgCpk::~CDlgCpk()
{
	//delete m_pCpk;
	m_pCpk = NULL;
}

void CDlgCpk::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_PLANNED_NO, m_gridPlannedNo);
	DDX_Control(pDX, IDC_GRID_PROCESS_CARD_NO, m_gridProcessCardNo);
}

void CDlgCpk::InitGrid_PlannedNo(void)
{
	m_gridPlannedNo.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//����
	m_gridPlannedNo.SetRowCount(1); //��ʼΪ1��

	m_gridPlannedNo.SetColumnCount(1); //��ʼ��Ϊ1��
	m_gridPlannedNo.SetFixedRowCount(1); //��ͷΪ1��
	m_gridPlannedNo.SetFixedColumnCount(0); //��ͷΪ0��
	m_gridPlannedNo.SetRowHeight(0, 30); //���ø��и�         

	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_WORDBREAK;
	Item.strText.Format(_T("�ƻ���"));
	Item.row = 0;
	Item.col = 0;
	m_gridPlannedNo.SetItem(&Item);

	CRect cRect;
	GetDlgItem(IDC_GRID_PLANNED_NO)->GetClientRect(&cRect);
	m_gridPlannedNo.SetColumnWidth(0, cRect.Width()); //����0�п� 

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
		//ˢ�¿�
		UpdateGrid_ProcessCardNo();
	}
}


BEGIN_MESSAGE_MAP(CDlgCpk, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SET, &CDlgCpk::OnBnClickedBtnSet)
	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_PLANNED_NO, OnSelChanged_PlannedNo)
	ON_BN_CLICKED(IDC_BTN_TEST, &CDlgCpk::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_CALC, &CDlgCpk::OnBnClickedBtnCalc)
	ON_BN_CLICKED(IDC_BTN_TREND_CHART, &CDlgCpk::OnBnClickedBtnTrendChart)
//	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CDlgCpk ��Ϣ�������


void CDlgCpk::OnBnClickedBtnSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgCpkSet dlg(m_pCpk);
	int ret = dlg.DoModal();
}

void CDlgCpk::InitGrid_ProcessCardNo(void)
{
	m_gridProcessCardNo.SetTextBkColor(RGB(0xFF, 0xFF, 0xCC));//����
	m_gridProcessCardNo.SetRowCount(1); //��ʼΪ1��

	m_gridProcessCardNo.SetColumnCount(10); //��ʼ��Ϊ9��
	m_gridProcessCardNo.SetFixedRowCount(1); //��ͷΪ1��
	m_gridProcessCardNo.SetFixedColumnCount(1); //��ͷΪ1��
	m_gridProcessCardNo.SetRowHeight(0, 30); //���ø��и�         

	GV_ITEM Item;
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_WORDBREAK;
	Item.strText.Format(_T("No"));
	Item.row = 0;
	Item.col = 0;
	m_gridProcessCardNo.SetItem(&Item); 
	Item.strText.Format(_T("���̿�"));
	Item.col = 1;
	m_gridProcessCardNo.SetItem(&Item); 
	Item.strText.Format(_T("����"));
	Item.col = 2;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("����"));
	Item.col = 3;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("����"));
	Item.col = 4;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("��ֵ"));
	Item.col = 5;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("ɢ��"));
	Item.col = 6;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("CP"));
	Item.col = 7;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("CA"));
	Item.col = 8;
	m_gridProcessCardNo.SetItem(&Item);
	Item.strText.Format(_T("CPK"));
	Item.col = 9;
	m_gridProcessCardNo.SetItem(&Item);

	CRect cRect;
	GetDlgItem(IDC_GRID_PROCESS_CARD_NO)->GetClientRect(&cRect);
	m_gridProcessCardNo.SetColumnWidth(0, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(1, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(2, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(3, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(4, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(5, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(6, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(7, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(8, cRect.Width() / 10 * 2); //����0�п� 
	m_gridProcessCardNo.SetColumnWidth(9, cRect.Width() / 10 * 2); //����0�п� 

	m_gridProcessCardNo.ExpandLastColumn();
	m_gridProcessCardNo.SetColumnResize(FALSE);
	m_gridProcessCardNo.SetRowResize(FALSE);
	m_gridProcessCardNo.SetSingleRowSelection(TRUE);
	m_gridProcessCardNo.SetEditable(FALSE);
	//SetAutoSizeStyle
	//m_gridStdSeries.EnableScrollBar();
	// m_gridStdSeries.EnableScrollBarCtrl();
	//m_gridProcessCardNo.ShowBar(SB_HORZ, FALSE);
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
		str.Format(_T("%d"), i);
		m_gridProcessCardNo.InsertRow(str, -1);
		m_gridProcessCardNo.SetItemText(line, 1, m_pCpk->m_arrProcessCardNo[i].no);//(m_pCpk->m_arrProcessCardNo[i].no, -1);
		str.Format(_T("%d"), SEC_TO_USER(m_pCpk->m_arrProcessCardNo[i].center));
		m_gridProcessCardNo.SetItemText(line, 2, str);
		str.Format(_T("%d"), SEC_TO_USER(m_pCpk->m_arrProcessCardNo[i].usl));
		m_gridProcessCardNo.SetItemText(line, 3, str);
		str.Format(_T("%d"), SEC_TO_USER(m_pCpk->m_arrProcessCardNo[i].lsl));
		m_gridProcessCardNo.SetItemText(line, 4, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].avg);
		m_gridProcessCardNo.SetItemText(line, 5, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].std);
		m_gridProcessCardNo.SetItemText(line, 6, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].cp);
		m_gridProcessCardNo.SetItemText(line, 7, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].ca);
		m_gridProcessCardNo.SetItemText(line, 8, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].cpk);
		m_gridProcessCardNo.SetItemText(line, 9, str);
	}

	m_gridProcessCardNo.ExpandLastColumn();

	m_gridProcessCardNo.Refresh();
}


BOOL CDlgCpk::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pCpk->LoadPlannedNo();

	InitGrid_PlannedNo();

	InitGrid_ProcessCardNo();

	UpdateGrid_PlannedNo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgCpk::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString test1,test2;
	GetDlgItemText(IDC_EDT_TEST1, test1);
	GetDlgItemText(IDC_EDT_TEST2, test2);

	if (!m_pCpk->OpenProcessCard(test1, test2))
	{
		AfxMessageBox(_T("�������̿��д����Ƿ��ظ�"));
		return;
	}

	CProcessCard processCard;
	processCard.avg = 100;
	processCard.ca = 1;
	processCard.center = 120;
	processCard.cp = 1;
	processCard.cpk = 1;
	processCard.axis = "����";
	processCard.lsl = 60;
	processCard.no = test2;
	processCard.std = 100;
	processCard.usl = 180;

	//for (int i = 0; i < 1; i++)
	//{
		m_pCpk->AddToPlannedCsv(processCard);
		m_pCpk->AddCpkToPlannedCsv(processCard);
	//}


	CProcessData processData;
	processData.equal = 120;
	processData.laser = 60;
	processData.laser0 = 60;
	processData.no = 2;
	processData.phi = 60;
	processData.phi0 = 60;
	processData.sort = 8;

	for (int i = 0; i < 3; i++)
	{
		m_pCpk->AddToProcessCardCsv(processData);
	}
}


void CDlgCpk::OnBnClickedBtnCalc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CCellRange sel = m_gridPlannedNo.GetSelectedCellRange();

	//int row = sel.GetMinRow();

	//CString text = m_gridPlannedNo.GetItemText(row, 0);

	//m_arrProcessCardNo;
}


#include "TrendChart.h"

void CDlgCpk::OnBnClickedBtnTrendChart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCellRange sel = m_gridPlannedNo.GetSelectedCellRange();

	int row = sel.GetMinRow();

	CString text = m_gridPlannedNo.GetItemText(row, 0);

	CArray<double, double> arrCpk;

  if (text == _T(""))
  {
    AfxMessageBox(_T("��ѡ��ƻ���"));
    return;
  }
#if 0
	for(int i = 0;i<50;i++)
	{
		double r = rand()%3 + (double(rand()%100))/100;
		arrCpk.Add(r);
	}
#else
	m_pCpk->CalcTrend(text, arrCpk);
#endif
	CTrendChart trend(arrCpk);

	trend.Show();

}


//BOOL CDlgCpk::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
//}
