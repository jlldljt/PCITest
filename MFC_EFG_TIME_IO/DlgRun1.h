#pragma once



// CDlgRun1 ������ͼ

class CDlgRun1 : public CFormView
{
	DECLARE_DYNCREATE(CDlgRun1)

protected:
	CDlgRun1();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDlgRun1();

public:
	enum { IDD = IDD_DLG_RUN1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
  CBrush m_brush;
public:
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  virtual void OnInitialUpdate();
};


