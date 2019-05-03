#pragma once


// CStaticDraw

class CStaticDraw //: public CStatic
{
	//DECLARE_DYNAMIC(CStaticDraw)
public:
	bool AddLine(COLORREF crColor, int nWidth);//����һ����

	bool AddLineEx(COLORREF crColor, int nWidth, int* pnVal);

	bool AddPoint(COLORREF crColor, int nWidth, POINTF pt);
	//����һ����
	int GetLineNum(void);//�õ����ڵ�������

	bool SetBG(COLORREF crBg, COLORREF crLine, int nGridX, int nGridY, bool show);//���ñ�������

	bool SetLineVal(int nLineNo, int* pnVal);//����ĳ���ߵĵ�ǰ��ֵ

	bool SetScrollPixel(int nPixel);//���þ�����������

	bool SetUpdateTime(int nTime);//���ø������ڣ�ms

	bool UpdateManual(void);//�ֶ�ˢ�´���

	bool SetRange(int nX, int nY);//����x��y���ֵ��nx��ʱû��
private:
	enum 
	{
		DRAW_LINE = 1,
	};
	// ����ͼ��
	struct stLine 
	{
		COLORREF clr;//��ɫ
		int* val;//��ǰֵy����,ָ���ⲿ����
		int preVal;//ǰһ��ֵy����
		int w;//�߿�
	};
	CList<stLine, stLine&> lsLine;//����ÿ���ߵ���Ϣ
	// ����ͼ��
	struct stPoint 
	{
		COLORREF clr;//��ɫ
		POINTF pt;//��ǰֵy����,ָ���ⲿ����
		int w;//�߿�
	};
	CArray<stPoint, stPoint&> arrPoint;//����ÿ�������Ϣ

	int scrollX;//��ǰ����x����
	int scrollPixel;//ÿ��x+����
	int updateTime;//����ʱ��ˢ��һ��

	COLORREF bgClr;//������ɫ
	COLORREF bgLineClr;//������ɫ
	int bgXLineNum;//����x���������
	int bgYLineNum;//����y���������
	bool showBgLine;//�Ƿ���ʾ����
	//��ͼ��dc
	CDC memDC;
	CBitmap memBM;
	CDC bgDC;
	CBitmap bgBM;
	CDC lineDC;
	CBitmap lineBM;
	CDC pointDC;
	CBitmap pointBM;
	CRect m_rcPoint;
	int m_showPercent;
	//���崰��dc
	CDC  *thisDC;
	CRect thisRect;
	CWnd *m_pWndParent;
	CWnd *m_pWnd;

	int rangeX;
	int rangeY;

public:
	CStaticDraw(CWnd * pWnd);
	virtual ~CStaticDraw();

protected:
	//DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	//virtual void PreSubclassWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void DrawPoint(void);

	virtual BOOL DestroyWindow();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, CRect rcCreate);

	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


