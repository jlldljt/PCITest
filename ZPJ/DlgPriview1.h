#pragma once


// CDlgPriview1 �Ի���

class CDlgPriview1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPriview1)

public:
	CDlgPriview1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPriview1();

  int m_n;
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
};
