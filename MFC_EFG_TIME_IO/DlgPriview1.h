#pragma once


#include "DlgCamera.h"
// CDlgPriview1 �Ի���

class CDlgPriview1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPriview1)

public:
	CDlgPriview1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPriview1();

  int m_n;
  CRectTracker m_RectTracker;
  BOOL bDraw;
  CCamera* m_p;//����
// �Ի�������
	enum { IDD = IDD_DLG_PRIVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
  afx_msg
    void Process();
  void OnLButtonDown(UINT nFlags, CPoint point);
  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void OnDestroy();
};
