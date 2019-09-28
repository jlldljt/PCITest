#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CDevice 对话框

class CDevice : public CDialogEx
{
	DECLARE_DYNAMIC(CDevice)

public:
	CDevice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDevice();
	void AxisMove(char num , int value , bool special);
	void XYMove(int xVal, int yVal);
  int ParamMove(int x, int y, int degree, int pn);
	void StmSE(bool se);
	void StmPause(bool pause);
	void StmReSta();
	void EFGCtrl(bool cw);
	void EFGCut(bool cw);
	void MHCtrl(char num, bool ctrl);
	void SNCtrl(char num, bool ctrl);
	void BlowCtrl(bool ctrl);
	void PosSet(char num , char num1 , int value);
	void AlarmCtrl(bool ctrl);
	void XYPosSet(char num , int valuex , int valuey);
	int XYSend(int valuex , int valuey);
  int DegPNSend(int degree, int pn);
  int StatusGet(int& status);
  int StatusSet(int status);
  int TurnTableStatGet(int& status);
  int TurnTableStatSet(int status);
  int BeltGet(int& status);
  int BeltSet(int status);
  int DefPNGet(int& status);
  int DefPNSet(int status);
  int CutGet(int& status);
  int CutSet(int status);
  int SizeGet(int& status);
  int SizeSet(int status);
	void SortSend(char num);
	bool EasyPosSet();
	void EFGStaSel(char nN);
	int nListSel;//标记点中的list中的编号

  CToolTipCtrl m_tt;

// 对话框数据
	enum { IDD = IDD_DIALOG_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listDevice;
	CEdit m_edtRs232Rcv;
	afx_msg void OnBnClickedBtnComclear();
	afx_msg void OnBnClickedBtnStminit();
	afx_msg void OnBnClickedBtnStmstop();
	afx_msg void OnBnClickedBtnXm();
	afx_msg void OnBnClickedBtnYm();
	afx_msg void OnBnClickedBtnChgm();
	afx_msg void OnBnClickedBtnSortm();
	afx_msg void OnBnClickedBtnRawm();
	afx_msg void OnBnClickedBtnXz();
	afx_msg void OnBnClickedBtnYz();
	afx_msg void OnBnClickedBtnChgz();
	afx_msg void OnBnClickedBtnSortz();
	afx_msg void OnBnClickedBtnRawv();
	afx_msg void OnBnClickedBtnUpz();
	afx_msg void OnBnClickedBtnDownz();
	afx_msg void OnBnClickedBtnXym();
	afx_msg void OnBnClickedBtnEfgtest();
	afx_msg void OnBnClickedBtnEfgdyn();
//	afx_msg void OnBnClickedBtnEfgstart();
	afx_msg void OnBnClickedChkXymh();
	afx_msg void OnBnClickedChkInmh();
	afx_msg void OnBnClickedChkOutmh();
	afx_msg void OnBnClickedChkSortmh();
	afx_msg void OnBnClickedChkXysn();
	afx_msg void OnBnClickedChkInsn();
	afx_msg void OnBnClickedChkOutsn();
	afx_msg void OnBnClickedChkSortsn();
	afx_msg void OnBnClickedBtnXy1set();
	afx_msg void OnBnClickedBtnXy2set();
	afx_msg void OnBnClickedBtnXy3set();
	afx_msg void OnBnClickedBtnSortupset();
	afx_msg void OnBnClickedBtnSortinset();
	afx_msg void OnBnClickedBtnSortoutset();
	afx_msg void OnBnClickedBtnSortdownset();
	afx_msg void OnLvnItemchangedListDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnListset();
	afx_msg void OnBnClickedChkBlow();
	afx_msg void OnBnClickedChkAlarm();
	afx_msg void OnBnClickedBtnClbpos1set();
	afx_msg void OnBnClickedBtnClbpos2set();
	afx_msg void OnBnClickedBtnClbpos3set();
	afx_msg void OnBnClickedBtnCom();
	afx_msg void OnBnClickedBtnListtest();
	afx_msg void OnBnClickedBtnSortuptest();
	afx_msg void OnBnClickedBtnSortintest();
	afx_msg void OnBnClickedBtnSortouttest();
	afx_msg void OnBnClickedBtnSortdowntest();
	afx_msg void OnBnClickedBtnXy1test();
	afx_msg void OnBnClickedBtnXy2test();
	afx_msg void OnBnClickedBtnXy3test();
	afx_msg void OnBnClickedBtnClbpos1test();
	afx_msg void OnBnClickedBtnClbpos2test();
	afx_msg void OnBnClickedBtnClbpos3test();
	afx_msg void OnCbnSelchangeCmbEfgstasel();
	CComboBox m_cmb_efgstasel;
//	afx_msg void OnBnClickedBtnOpenefg();
//	afx_msg void OnBnClickedBtnCloseefg();
	afx_msg void OnBnClickedBtnStmresta();
	afx_msg void OnBnClickedBtnSnchgset();
	afx_msg void OnBnClickedBtnSnchgtest();
	afx_msg void OnBnClickedBtnSnchgm();
  afx_msg void OnEnChangeEdtSnchgx();
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  afx_msg void OnEnChangeEdtXy2y();
//  afx_msg void OnBnClickedBtnPosset();
//  afx_msg void OnBnClickedBtnPosset();
  afx_msg void OnBnClickedBtnXOffsetSet();
//  afx_msg void OnBnClickedBtnYOffsetSet();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedBtnVar7();
  afx_msg void OnBnClickedBtnVar8();
  afx_msg void OnBnClickedBtnVar9();
  afx_msg void OnBnClickedBtnVar10();
};
extern CDevice *g_dlgDevice;