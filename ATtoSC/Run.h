#pragma once
#include "afxcmn.h"
#include "afxwin.h"

int RCGVGA(void);//读屏幕
void WaitVGAEnd(void);
UINT TakeThread(LPVOID pParam);//线程函数
UINT SortThread(LPVOID pParam);//线程函数
UINT SortThreadWithZ(LPVOID pParam);//线程函数
UINT Sort2Thread(LPVOID pParam);//线程函数
UINT CylinderThread(LPVOID pParam);//线程函数
UINT CheckThread(LPVOID lParam);
UINT ScreenThread(LPVOID pParam);//线程函数
UINT DisplayThread(LPVOID pParam);//线程函数
int SortChip(sortchip* s_sort,int *degree);
int SortChipR1(sortchip* s_sort,int *degree);
void SortAvgStd(unsigned int sum,int *angle,double *avg,double *std,long double *std2);
void AlertCtrl(bool stat);//控制警报
void MotorCtrlYToX(int steps);//控制电机
void MotorCtrl(int n,bool zf,int steps,bool send);//控制1、2号电机
void MotorCtrl(int n,bool zf,int steps);//控制1、2号电机
void CylinderCtrl(bool stat);
void CylinderCtrlDbg(bool stat);
void TubeCtrl(int n,bool stat);//控制1、2号机械臂的气嘴开关
void SortBlock(int n);//分档
void SortBlockWithZ(int n);//分档
void VibrateBlock();//震料器
void ReturnZero(int n);//控制1、2号机械臂回零位
void BlowBlock(UINT time);//吹气
void StartFT();//efg开始检测信号
void StartFTLong(bool se);


// CRun 对话框

class CRun : public CDialogEx
{
	DECLARE_DYNAMIC(CRun)

public:
	CRun(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRun();
	CToolTipCtrl m_toolTip;
// 对话框数据
	enum { IDD = IDD_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
