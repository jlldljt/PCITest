#pragma once
#include <list>
using std::list;

typedef struct SquareInfo{//��׼Ƭ
	bool debug;//�Ƿ�dubugģʽ����ͼģʽ
	int nBMPW,nBMPH;
	int side ;//���������ÿ����Ƭ�ı�׼�ܳ�,���Ǳ���ο��ܳ�
	int area;//��׼��������Ǳ���ο����
	//int pixel_qty[3];//ÿ���߱�׼���ظ���
	double d_len[4];//ÿ���ߵı߳�
	double d_point_x[4];//����X����
	double d_point_y[4];//����Y����
	double d_a[4];//ÿ���ߵķ���
	double d_b[4];
	double d_c[4];
	int sags;//Ƭ��ȱ������sags���ǵ�����,crests͹������
	int crests;
	bool rcglineok;//������߶��Ƿ�ɹ�
	double d_theta[4];//ÿ���ߵĽǶ�
	int nN;//����ʶ���������
	int pnZPX[20],pnZPY[20];//����һ��ͼ����������꣬�ܹ�20��
	int pnLen[20],pnWth[20];//����һ��ͼ���Ӧ��������Ƭ�ĳ���
	int pnPX[20][4],pnPY[20][4];//����һ��ͼ���Ӧ��Ķ��㣬��˳ʱ����
	int bPN[20];//���������棬����������ǣ���˳ʱ�뷽��ȱ�ǵ�30����ȣ�60���.1��-1����0��
	bool bJudgeFeature;//�Ƿ�ƥ������
	bool bIsLap;//�Ƿ���ڵ�Ƭ
}SquareInfo;
typedef struct FeatureRefer{
	int Len,Wth,Dev,Rad;//������ƫ��뾶
	double SAg,BAg;//�����С����ǣ����ڷ���
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
	char szname[MAX_PATH];//���ڴ�·��
	int biBitCount;
	RGBQUAD *pColorTable1;//��ɫ��ָ��//mmy ����
	
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