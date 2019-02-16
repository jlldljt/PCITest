#pragma once
#include "StandardLib.h"

// CDlgCheckStd �Ի���


class CDlgCheckStd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCheckStd)

public:
	CDlgCheckStd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCheckStd();

// �Ի�������
	enum { IDD = IDD_DLG_CHECKSTD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();

  CStandardLib *m_stdLib;

  CString m_prevEditStr;
  //��׼ϵ�е��б�
  CGridCtrl m_gridStdSeries;  
  int m_seriesRowSel;
  void InitGrid_StdSeries(void);
  void UpdateGrid_StdSeries(void);
  void OnBeginLabelEdit_StdSeries(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit_StdSeries(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged_StdSeries(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll_StdSeries(NMHDR * pNMHDR, LRESULT * pResult);
  //��׼Ƭ���б�
  CGridCtrl m_gridStdLib;
  void InitGrid_StdLib(void);
  void UpdateGrid_StdLib(void);
  void OnBeginLabelEdit_StdLib(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit_StdLib(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged_StdLib(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll_StdLib(NMHDR * pNMHDR, LRESULT * pResult);
  // checked
  CGridCtrl m_gridStdChecked;
  void InitGrid_StdChecked(void);
  void UpdateGrid_StdChecked(void);
  void ClearGrid_StdChecked(void);
  void AddGrid_StdChecked(CStdLib std, bool laser, bool phi);
  void OnBeginLabelEdit_StdChecked(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit_StdChecked(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged_StdChecked(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll_StdChecked(NMHDR * pNMHDR, LRESULT * pResult);
  //result
  CGridCtrl m_gridStdResult;
  void InitGrid_StdResult(void);
  void UpdateGrid_StdResult(void);
  void OnBeginLabelEdit_StdResult(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndLabelEdit_StdResult(NMHDR * pNMHDR, LRESULT * pResult);
  void OnSelChanged_StdResult(NMHDR * pNMHDR, LRESULT * pResult);
  void OnEndScroll_StdResult(NMHDR * pNMHDR, LRESULT * pResult);
public:
  afx_msg void OnBnClickedBtnSave();
  afx_msg void OnBnClickedBtnReload();
  afx_msg void OnBnClickedBtnStdChecking();
  afx_msg void OnBnClickedBtnSet();
  afx_msg void OnBnClickedBtnClear();
  afx_msg void OnBnClickedBtnConfirm();
  afx_msg void OnBnClickedBtnStdModify();
};
