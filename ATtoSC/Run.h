#pragma once
#include "afxcmn.h"
#include "afxwin.h"

int RCGVGA(void);//����Ļ
void WaitVGAEnd(void);
UINT TakeThread(LPVOID pParam);//�̺߳���
UINT SortThread(LPVOID pParam);//�̺߳���
UINT SortThreadWithZ(LPVOID pParam);//�̺߳���
UINT Sort2Thread(LPVOID pParam);//�̺߳���
UINT CylinderThread(LPVOID pParam);//�̺߳���
UINT CheckThread(LPVOID lParam);
UINT ScreenThread(LPVOID pParam);//�̺߳���
UINT DisplayThread(LPVOID pParam);//�̺߳���
int SortChip(sortchip* s_sort,int *degree);
int SortChipR1(sortchip* s_sort,int *degree);
void SortAvgStd(unsigned int sum,int *angle,double *avg,double *std,long double *std2);
void AlertCtrl(bool stat);//���ƾ���
void MotorCtrlYToX(int steps);//���Ƶ��
void MotorCtrl(int n,bool zf,int steps,bool send);//����1��2�ŵ��
void MotorCtrl(int n,bool zf,int steps);//����1��2�ŵ��
void CylinderCtrl(bool stat);
void CylinderCtrlDbg(bool stat);
void TubeCtrl(int n,bool stat);//����1��2�Ż�е�۵����쿪��
void SortBlock(int n);//�ֵ�
void SortBlockWithZ(int n);//�ֵ�
void VibrateBlock();//������
void ReturnZero(int n);//����1��2�Ż�е�ۻ���λ
void BlowBlock(UINT time);//����
void StartFT();//efg��ʼ����ź�
void StartFTLong(bool se);


// CRun �Ի���

class CRun : public CDialogEx
{
	DECLARE_DYNAMIC(CRun)

public:
	CRun(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRun();
	CToolTipCtrl m_toolTip;
// �Ի�������
	enum { IDD = IDD_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CWinThread* p_take;
	CWinThread* p_sort;
	CWinThread* p_sort2;
	CWinThread* p_cylinder;
	CWinThread* p_check ;
	CWinThread* p_screen;
	CWinThread* p_display;
	DECLARE_MESSAGE_MAP()
public:
  CCSVFile *m_csvCard;
  CCSVFile *m_csvAllCard;
	afx_msg void OnBnClickedRunStart();
	afx_msg void OnBnClickedRunStop();
	CListCtrl m_runlist;
	void RunListReset();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSortSet();
//	CComboBox m_SortSel1;
	CComboBox m_sortsel1;
	CComboBox m_sortsel2;
	afx_msg void OnCbnSelchangeSortSel1();
	afx_msg void OnCbnSelchangeSortSel2();
	afx_msg void OnBnClickedRunPause();
	afx_msg void OnBnClickedStartFt();
//	int i_edit_bs;
//	afx_msg void OnEnChangeEditbs();
	afx_msg void OnBnClickedBtnStandand();
	afx_msg void OnEnChangeEditTimeX();
	afx_msg void OnBnClickedChkLongSqu();
	afx_msg void OnBnClickedEndFt();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
afx_msg void OnEnChangeEditTimeZf();
afx_msg void OnEnChangeEditTimeX2();
	afx_msg void OnEnChangeEditTimeXf();
afx_msg void OnEnChangeEditTimeX3();
	afx_msg void OnEnChangeEditTimeZ();
	afx_msg void OnEnChangeEditTimeY();
	afx_msg void OnEnChangeEditTimeYf();
	afx_msg void OnEnChangeEditTimeBlow();
	afx_msg void OnBnClickedBtnPrint();
	afx_msg void OnBnClickedBtnStdcheck();
  afx_msg void OnBnClickedBtnCpk();
};
