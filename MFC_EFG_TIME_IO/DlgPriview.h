#pragma once

#include "DlgCamera.h"

// CDlgPriview �Ի���

class CDlgPriview : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPriview)

public:
	CDlgPriview(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPriview();
  CCamera* m_p;//����
// �Ի�������
	enum { IDD = IDD_DLG_PRIVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  virtual BOOL OnInitDialog();
//  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void OnPaint();
};
