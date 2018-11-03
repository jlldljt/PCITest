#pragma once
#include <list>
using std::list;

typedef struct SquareInfo{//标准片
	bool debug;//是否dubug模式，画图模式
	int nBMPW,nBMPH;
	int side ;//三个相机的每个晶片的标准周长,考虑保存参考周长
	int area;//标准面积，考虑保存参考面积
	//int pixel_qty[3];//每个边标准像素个数
	double d_len[4];//每个边的边长
	double d_point_x[4];//顶点X坐标
	double d_point_y[4];//顶点Y坐标
	double d_a[4];//每个边的方程
	double d_b[4];
	double d_c[4];
	int sags;//片的缺陷数量sags凹角的数量,crests凸角数量
	int crests;
	bool rcglineok;//标记找线段是否成功
	double d_theta[4];//每个边的角度
	int nN;//保存识别出的数量
	int pnZPX[20],pnZPY[20];//保存一幅图像的中心坐标，总共20个
	int pnLen[20],pnWth[20];//保存一幅图像对应中心坐标片的长宽
	int pnPX[20][4],pnPY[20][4];//保存一幅图像对应点的顶点，按顺时针存放
	int bPN[20];//保存正反面，正面的依据是，按顺时针方向，缺角的30°角先，60°后.1正-1负，0无
	bool bJudgeFeature;//是否匹配特征
	bool bIsLap;//是否存在叠片
}SquareInfo;
typedef struct FeatureRefer{
	int Len,Wth,Dev,Rad;//长、宽、偏差、半径
	double SAg,BAg;//最大最小允许角，对于方形
}FeatureRefer;
typedef struct Vector
{
	double x,y;
}vector,point;

class CIMGRecognition
{
public:
	SquareInfo g_stu_square;
	FeatureRefer stuRef;
	CString m_bmp_file;
	char chPathMain[MAX_PATH];
	char szname[MAX_PATH];//用于存路径
	int biBitCount;
	RGBQUAD *pColorTable1;//颜色表指针//mmy 移入
	
	CIMGRecognition(void);
	~CIMGRecognition(void);
	int readBmp(char* bmpName,unsigned char *&pBmpBuf,int & bmpWidth,int & bmpHeight);
	bool saveBmp(char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD * pColorTable);
	int read_image_fz(unsigned char * &pBmpBuf,int bmpWidth,int bmpHeight,int oprtWth,int oprtDVal, int validDVal);
	int read_image_mul(unsigned char * &pBmpBuf,int * &xsvalue,int * &fbvalue,int * &jgvalue,int *& txvalue,int bmpWidth,int bmpHeight,int threshold,bool debug,bool delNoise);
	int read_image_blank(unsigned char * &pBmpBuf,int * &xsvalue,int * &fbvalue,int * &jgvalue,int *& txvalue,int bmpWidth,int bmpHeight,int threshold);
	int bj_close(int* sz_h, int* sz_l, int sz_num, int zb_h, int zb_l);
	//
	int data_check_fill_bj(unsigned char * &pBmpBuf,int qybn, int fillv, int temp_close, int * temp_bj_h, int * temp_bj_l,int * & xsvalue,int * & fbvalue,int * & jgvalue,int *& txvalue,int bmpWidth,int bmpHeight,int &tmin_h,int &tmax_h,int &tmin_l,int &tmax_l,bool debug);
	void data_bmp_set_color(unsigned char * &pBmpBuf,int biBitCount,  int bmpWidth,int bmpHeight,int temph,int templ,unsigned char R,unsigned char G,unsigned char B);
	int ReadBmpGray(unsigned char * &pBmpBuf,int biBitCount,  int bmpWidth,int bmpHeight,int temph,int templ);
//
	int data_check_feature_circle(unsigned char * &pBmpBuf,int biBitCount,int bmpWidth,int bmpHeight
		, int bj_num,  int * bj_h, int * bj_l,int qybn,int * & xsvalue,int * & fbvalue,int * & jgvalue,int *& txvalue
		,int outpointallowlen,int defectallowlen,int &tmin_h,int &tmax_h,int &tmin_l,int &tmax_l);
	int data_check_feature_circleTMP(unsigned char * &pBmpBuf,int biBitCount,int bmpWidth,int bmpHeight, int bj_num,  int * bj_h, int * bj_l,int qybn,int * & xsvalue,int * & fbvalue,int * & jgvalue,int *& txvalue,int outpointallowlen,int defectallowlen,int &tmin_h,int &tmax_h,int &tmin_l,int &tmax_l);
	int data_check_feature_circle_split(unsigned char * &pBmpBuf,int biBitCount,int bmpWidth,int bmpHeight
		, int bj_num,  int * bj_h, int * bj_l,int * bj_fx,int qybn,int * & xsvalue,int * & fbvalue,int * & jgvalue,int *& txvalue
		,int outpointallowlen,int defectallowlen,int &tmin_h,int &tmax_h,int &tmin_l,int &tmax_l);
	int data_check_feature(unsigned char * &pBmpBuf,int biBitCount,int bmpWidth,int bmpHeight
		, int bj_num,  int * bj_h, int * bj_l,int qybn,int * & xsvalue,int * & fbvalue,int * & jgvalue,int *& txvalue
		,int outpointallowlen,int defectallowlen);
	int data_check_feature_split(unsigned char * &pBmpBuf,int biBitCount,int bmpWidth,int bmpHeight
		, int bj_num,  int * bj_h, int * bj_l,int * bj_fx,int qybn,int * & xsvalue,int * & fbvalue,int * & jgvalue,int *& txvalue
		,int outpointallowlen,int defectallowlen);
	void SortAvg(int sum,int value,double &avg);
	double LenFromP1toP2(double P1X,double P1Y,double P2X,double P2Y);
	double LenFromPtoL(double PX,double PY,double a,double b,double c);
	bool CalcEquationABC(double P1X,double P1Y,double P2X,double P2Y,double &a,double &b,double &c);
	bool InOrOut(double PX,double PY,double*& a,double*& b,double*& c);
	int IsSquare(double smallAg,double bigAg);
	int IsPositiveOrNegative(double a , double b ,  double c , int beginLineNo , int endLineNo);
	int RCGBMP(CDC*pDC,CRect rect,char* bmpName,int outpointallowlen,int defectallowlen,int threshold,bool debug,bool circle,bool bAutoThrd,bool delNoise);
	int RCGBMPSPLIT(CDC*pDC,CRect rect,char* bmpName,int outpointallowlen,int defectallowlen,int threshold,bool debug,bool circle,bool bAutoThrd,bool delNoise);
	int RCGBMPDATASPLIT(CDC*pDC,CRect rect,unsigned char * &pBmpBuf,int outpointallowlen,int defectallowlen,int threshold,bool debug,bool circle,bool bAutoThrd,bool delNoise);
	double VectorAngle(vector v1,vector v2);
	double VectorRadian(vector v1,vector v2);
	double VectorLen(vector v);
	int VectorCW(vector vf,vector vb);
	double Distance(vector v,point p,point vp);
	vector VectorProjection(vector v,vector basev);
};

extern CIMGRecognition gclsImgRcg;