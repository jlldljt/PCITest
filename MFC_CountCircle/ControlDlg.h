#pragma once



// CControlDlg ������ͼ

class CControlDlg : public CFormView
{
	DECLARE_DYNCREATE(CControlDlg)

protected:
	CControlDlg();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CControlDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLDLG };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


