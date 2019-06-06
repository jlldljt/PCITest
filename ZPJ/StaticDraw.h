#pragma once


// CStaticDraw

class CStaticDraw //: public CStatic
{
	//DECLARE_DYNAMIC(CStaticDraw)
public:
	bool AddLine(COLORREF crColor, int nWidth);//增加一条线

	bool AddLineEx(COLORREF crColor, int nWidth, int* pnVal);

	bool AddPoint(COLORREF crColor, int nWidth, POINTF pt);
	//增加一条线
	int GetLineNum(void);//得到现在的线条数

	bool SetBG(COLORREF crBg, COLORREF crLine, int nGridX, int nGridY, bool show);//设置背景网格

	bool SetLineVal(int nLineNo, int* pnVal);//设置某条线的当前点值

	bool SetScrollPixel(int nPixel);//设置卷动步长（像素

	bool SetUpdateTime(int nTime);//设置更新周期（ms

	bool UpdateManual(void);//手动刷新窗口

	bool SetRange(int nX, int nY);//设置x，y最大值，nx暂时没用
private:
	enum 
	{
		DRAW_LINE = 1,
	};
	// 滚动图用
	struct stLine 
	{
		COLORREF clr;//颜色
		int* val;//当前值y坐标,指向外部变量
		int preVal;//前一个值y坐标
		int w;//线宽
	};
	CList<stLine, stLine&> lsLine;//保存每条线的信息
	// 折线图用
	struct stPoint 
	{
		COLORREF clr;//颜色
		POINTF pt;//当前值y坐标,指向外部变量
		int w;//线宽
	};
	CArray<stPoint, stPoint&> arrPoint;//保存每个点的信息

	int scrollX;//当前画的x坐标
	int scrollPixel;//每次x+多少
	int updateTime;//多少时间刷新一次

	COLORREF bgClr;//背景颜色
	COLORREF bgLineClr;//网格颜色
	int bgXLineNum;//网格x轴的线条数
	int bgYLineNum;//网格y轴的线条数
	bool showBgLine;//是否显示网格
	//绘图用dc
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
	//本体窗口dc
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


