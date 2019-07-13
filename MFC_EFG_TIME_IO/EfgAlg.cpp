#include "StdAfx.h"
#include "EfgAlg.h"
#include <math.h>

EfgAlg::EfgAlg()
{
}


EfgAlg::~EfgAlg()
{
}

// 一维滤波器信息结构体
typedef  struct{
	double filterValue;  //k-1时刻的滤波值，即是k-1时刻的值
	double kalmanGain;   //   Kalamn增益
	double A;   // x(n)=A*x(n-1)+u(n),u(n)~N(0,Q)
	double H;   // z(n)=H*x(n)+w(n),w(n)~N(0,R)
	double Q;   //预测过程噪声偏差的方差
	double R;   //测量噪声偏差，(系统搭建好以后，通过测量统计实验获得)
	double P;   //估计误差协方差
}  KalmanInfo;
/**
* @brief Init_KalmanInfo   初始化滤波器的初始值
* @param info  滤波器指针
* @param Q 预测噪声方差 由系统外部测定给定
* @param R 测量噪声方差 由系统外部测定给定
*/
void Init_KalmanInfo(KalmanInfo* info, double Q, double R, double filterValue)
{
	info->A = 1;  //标量卡尔曼
	info->H = 15;  //
	info->P = 1.85;  //后验状态估计值误差的方差的初始值（不要为0问题不大）
	info->Q = Q;    //预测（过程）噪声方差 影响收敛速率，可以根据实际需求给出
	info->R = R;    //测量（观测）噪声方差 可以通过实验手段获得
	info->filterValue = filterValue;// 测量的初始值 0 
}
double KalmanFilter(KalmanInfo* kalmanInfo, double lastMeasurement)
{
	//预测下一时刻的值
	double predictValue = kalmanInfo->A* kalmanInfo->filterValue;   //x的先验估计由上一个时间点的后验估计值和输入信息给出，此处需要根据基站高度做一个修改
	
	//求协方差
	kalmanInfo->P = kalmanInfo->A*kalmanInfo->A*kalmanInfo->P + kalmanInfo->Q;  //计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q
	double preValue = kalmanInfo->filterValue;  //记录上次实际坐标的值
 
	//计算kalman增益
	kalmanInfo->kalmanGain = kalmanInfo->P*kalmanInfo->H / (kalmanInfo->P*kalmanInfo->H*kalmanInfo->H + kalmanInfo->R);  //Kg(k)= P(k|k-1) H’ / (H P(k|k-1) H’ + R)
	//修正结果，即计算滤波值
	kalmanInfo->filterValue = predictValue + (lastMeasurement - predictValue)*kalmanInfo->kalmanGain;  //利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
	//更新后验估计
	kalmanInfo->P = (1 - kalmanInfo->kalmanGain*kalmanInfo->H)*kalmanInfo->P;//计算后验均方差  P[n|n]=(1-K[n]*H)*P[n|n-1]
 
	return  kalmanInfo->filterValue;
}

void EfgAlg::KLM(double * yi, int iNum)
{
	KalmanInfo klm;
	Init_KalmanInfo(&klm, 3, 3, yi[iNum-1]);
	for(int i = 0; i < iNum; i++)
	{
		KalmanFilter(&klm, yi[i]);
		yi[i] = klm.filterValue;
	}
	//KalmanFilter(&klm, yi[0]);
	//yi[0] = klm.filterValue;
}


//计算光轴平均值及标准方程
double EfgAlg::Avg(int num, int *th)
{

  double sum = 0;

  for (int i = 0; i< num; i++)//计算原始TH的中间值
  {
    sum += double(th[i]);
  }

  return sum / double(num);
}

//计算光轴平均值及标准方程
double EfgAlg::Avg(int num, double *th)
{

  double sum = 0;

  for (int i = 0; i< num; i++)//计算原始TH的中间值
  {
    sum += th[i];
  }

  return sum / double(num);
}

double EfgAlg::AvgDVal(int num, double *yi, double *fityi)
{
  return fabs(Avg(num, yi) - Avg(num, fityi));
}

double EfgAlg::Std(int num, double *yi, double *fityi)
{
  double sumStd = 0;
  for (int i = 0; i< num; i++)
  {
    sumStd += (double(yi[i]) - fityi[i])*(double(yi[i]) - fityi[i]);
  }
  return sumStd;
}
//return A * sin (w*xi + t) + k
double EfgAlg::SinAwtk(struct tagSinParam sinparam, double xi)
{
  return (sinparam.A*sin(sinparam.w * xi + sinparam.t) + sinparam.k);
}
//安装拟合参数计算出yi
void EfgAlg::CalFitYi(double *fityi, int iNum, struct tagSinParam sinparam)
{
  for (int i = 0; i< iNum; i++)
  {
    fityi[i] = SinAwtk(sinparam, i);
  }
}
//返回整顿后的数组大小
int EfgAlg::SmoothYi(double * yi, int iNum, double limit/*超限阈值*/, int confirmLimitNum/*不足即超限*/,int flag/*超限标记*/)
{
  int outLimitCnt = 0;
  // int cur = 0;
  int limitCur = 0;
  // int limitCnt = 0;
  double d = 0;

  for (int i = 0; i < iNum-1; i++)
  {
    outLimitCnt = i - limitCur;//
    d = fabs(yi[i + 1] - yi[i]);//前后两点差
    if (d < limit && i < iNum - 2)
    {
      continue;
    }
    else // out limit
    {
      if (outLimitCnt < confirmLimitNum)//不确认状态
      {
        for (int j = limitCur; j < i+1; j++)
        {
          yi[j] = flag;
        }
      }
      
      limitCur = i + 1;
    }
  }
  return 0;
}
//代入逼近xi 等间距
int EfgAlg::FitSinBySubstitution(double * yi/*存放原始y值*/, int iNum, double *fityi/*存放拟合后的值*/, struct tagSinParam &sinparam)
{
  double A = 1;
  const double w = 2*PI/ iNum;
  double t = 0;
  double k = Avg(iNum, yi);//计算大致k
  double  ldv_MinDAvg = 0, ldv_TmpDAvg = 0;
  double  ldv_MinDStd = 0, ldv_TmpDStd = 0;
  struct tagSinParam param = { 0 };

  for (int i = 0; i< iNum; i++)//计算大致A
  {
    A = A > fabs(double(yi[i]) - k) ? A : fabs(double(yi[i]) - k);
  }

  param.A = A;
  param.k = k;
  param.t = t;
  param.w = w;

  bool lbv_Chg = 0;
  double ldv_DVal = 0;//迭代步进值，逐渐缩小
  CalFitYi(fityi, iNum, param);
  //置初值
  ldv_MinDAvg = AvgDVal(iNum, yi, fityi);
  ldv_MinDStd = Std(iNum, yi, fityi);
  do
  {
    lbv_Chg = 0;
    
    //t fit
    ldv_DVal = 1;
    for (double tmpt = t + ldv_DVal;; tmpt += ldv_DVal)
    {
      param.A = A;
      param.k = k;
      param.t = tmpt;
      param.w = w;
      CalFitYi(fityi, iNum, param);
      ldv_TmpDStd = Std(iNum, yi, fityi);
      if (ldv_MinDStd > ldv_TmpDStd)
      {
        ldv_MinDStd = ldv_TmpDStd;
        t = tmpt;
        lbv_Chg = 1;

      }
      else
      {
        tmpt = t;
        if (ldv_DVal > 0)
        {
          ldv_DVal *= -0.1;
        }
        else
        {
          ldv_DVal *= -1;
        }

      }

      if (fabs(ldv_DVal) < 0.0000001)
      {
        break;
      }

      if (ldv_DVal>0)
      {
        if (tmpt + ldv_DVal >= 2*PI)
        {
          tmpt = 0;
        }
      }
      else if (ldv_DVal<0)
      {
        if (tmpt + ldv_DVal < 0)
        {
          tmpt = 2*PI;
        }
      }
      else
      {
        AfxMessageBox(_T("ldv_DVal error"));
        return 0;
      }
    }
    
    //k fit
    ldv_DVal = 100;
    for (double tmpk = k + ldv_DVal;; tmpk += ldv_DVal)
    {
      param.A = A;
      param.k = tmpk;
      param.t = t;
      param.w = w;
      CalFitYi(fityi, iNum, param);
      ldv_TmpDStd = Std(iNum, yi, fityi);
      if (ldv_MinDStd > ldv_TmpDStd)
      {
        ldv_MinDStd = ldv_TmpDStd;
        k = tmpk;
        lbv_Chg = 1;

      }
      else
      {
        tmpk = k;
        if (ldv_DVal > 0)
        {
          ldv_DVal *= -0.1;
        }
        else
        {
          ldv_DVal *= -1;
        }

      }

      if (fabs(ldv_DVal) < 0.001)
      {
        break;
      }

    }

    //A fit
    ldv_DVal = 10;
    for (double tmpA = A + ldv_DVal;; tmpA += ldv_DVal)
    {
      param.A = tmpA;
      param.k = k;
      param.t = t;
      param.w = w;
      CalFitYi(fityi, iNum, param);
      ldv_TmpDStd = Std(iNum, yi, fityi);
      /*if (ldv_MinDAvg > ldv_TmpDAvg)
      {
      ldv_MinDAvg = ldv_TmpDAvg;
      A = cntK;
      lbv_Chg = 1;

      }*/
      if (ldv_MinDStd > ldv_TmpDStd)
      {
        ldv_MinDStd = ldv_TmpDStd;
        A = tmpA;
        lbv_Chg = 1;

      }
      else
      {
        tmpA = A;
        if (ldv_DVal > 0)
        {
          ldv_DVal *= -0.1;
        }
        else
        {
          ldv_DVal *= -1;
        }

      }

      if (fabs(ldv_DVal) < 0.001)
      {
        break;
      }

    }


  } while (lbv_Chg);

  sinparam.A = A;
  sinparam.w = w;
  sinparam.t = t;
  sinparam.k = k;

  return 1;
}

//三参数拟合
//参考《四参数正弦曲线拟合的一种收敛算法》
int EfgAlg::FitSinByLeastSquares(double * yi, int iNum, double * fityi, tagSinParam & sinparam, double *ignore_val)
{
	
  double A = 0;
  const double w = 2 * PI / iNum;
  int num = 0;
  double t = 0;
  double k = 0;
  struct tagSinParam param = { 0 };
  double y, a, b;
  double avgy = 0, avga = 0, avgb = 0;
  double sumya = 0, sumyb = 0, sumb = 0, suma = 0, sumab = 0, sumaa = 0, sumbb = 0;
  for (int i = 0; i < iNum; i++)
  {
	if(ignore_val)
	  if(yi[i] == *ignore_val)//临时加，使之跳过
	    continue;

	num++;

    y = yi[i];
    a = cos(w*i);
    b = sin(w*i);
    avgy += y;
    avga += a;
    avgb += b;
    sumya += y*a; 
    sumyb += y*b;
    sumb += b;
    suma += a;
    sumab += a*b;
    sumaa += a*a;
    sumbb += b*b;
  }
  avgy /= num;//iNum; 
  avga /= num;//iNum;
  avgb /= num;//iNum;

  double AN = 0, AD = 0, BN = 0, BD = 0;
  
  AN = (sumya - avgy * suma) / (sumab - avgb*suma) - (sumyb - avgy*sumb) / (sumbb - avgb*sumb);
  AD = (sumaa - avga * suma) / (sumab - avgb*suma) - (sumab - avga*sumb) / (sumbb - avgb*sumb);
  BN = (sumya - avgy * suma) / (sumaa - avga*suma) - (sumyb - avgy*sumb) / (sumab - avga*sumb);
  BD = (sumab - avgb * suma) / (sumaa - avga*suma) - (sumbb - avgb*sumb) / (sumab - avga*sumb);

  double A1 = 0, B1 = 0, C = 0;

  A1 = AN / AD;
  B1 = BN / BD;
  C = avgy - A1*avga - B1*avgb;

  A = sqrt(A1*A1 + B1*B1);
  if (A1 >= 0)
  {
    t = atan(-B1 / A1);
  }
  else
  {
    t = atan(-B1 / A1) + PI;
  }
  k = C;
  //以上拟合函数是cos   cos（3π / 2 + α） = sinα
  t -= 3 * PI / 2;

  param.A = A;
  param.k = k;
  param.t = t;
  param.w = w;
  CalFitYi(fityi, iNum, param);

  sinparam = param;

  return 0;
}
//smooth_width两边各cnt次数
  void EfgAlg::Correct(double * yi, int iNum, int sub)
  {
	  double *tmp_yi=NULL;
	  tmp_yi = new double[iNum];
	  tmp_yi[0] = yi[0]; 
	  double tmp_front = tmp_yi[0];
	  double sub_sum = sub;

	  for(int i = 1 ; i < iNum; i++)
	  {
		  tmp_front = tmp_yi[i-1];
		  if(fabs(yi[i]-tmp_front)<=sub_sum ){
			  sub_sum = sub;
			  tmp_yi[i] = yi[i];
		  }else{
			  sub_sum += sub;
			  tmp_yi[i] = tmp_front;
		  }
	  }

	  memcpy(yi,tmp_yi,sizeof(double)*iNum);
	  
	  delete[] tmp_yi;
  }

//smooth_width两边各cnt次数
  void EfgAlg::Smooth(double * yi, int iNum, int smooth_width, int cnt)
  {
	  double *tmp_yi=NULL;
	  tmp_yi = new double[iNum];
	  for(int j=0;j<cnt;j++)
	  {
	  for(int i = 0 ; i < iNum; i++)
	  {
		  double tmp_sum = yi[i];
		  for (int j = 0; j< smooth_width;j++)
		  {
			  int index_low = i - j;
			  int index_high = i + j;

			  if(index_low < 0)
				  index_low += iNum;

			  if(index_high >= iNum)
				  index_high -= iNum;

			  tmp_sum += (yi[index_low] + yi[index_high]);
		  }
		  tmp_sum /= (smooth_width*2+1);
		  tmp_yi[i] = tmp_sum;
	  }

	  memcpy(yi,tmp_yi,sizeof(double)*iNum);
	  }
	  delete[] tmp_yi;
  }

//////////////////////////////////////////////////X laser///////////////////////////////////////////////////
 //抛物线
  /***********************************************************************************
从txt文件里读取double型XY数据
***********************************************************************************/
int GetXY(const char* FileName, double* X, double* Y, int* Amount)
{
        FILE* File = fopen(FileName, "r");
        if (!File)
                return -1;
        for (*Amount = 0; !feof(File); X++, Y++, (*Amount)++)
                if (2 != fscanf(File, (const char*)"%lf %lf", X, Y))
                        break;
        fclose(File);
        return 0;
}

/***********************************************************************************
从double型XY数据中获取系数矩阵
***********************************************************************************/
int GetK(double* K, const double* X, const double* Y, int Amount)
{
        *(K + 10) = Amount;//第10位即左矩阵右下角即x^0=1，累加完就是Amount
        *K = 0;
        *(K + 1) = 0;
        *(K + 2) = 0;
        *(K + 3) = 0;
        *(K + 6) = 0;
        *(K + 7) = 0;
        *(K + 11) = 0;
        for ( ; Amount; Amount--, X++, Y++)//求和即Σ
        {
                *K += (*X) * (*X) * (*X) * (*X);
                *(K + 1) += (*X) * (*X) * (*X);
                *(K + 2) += (*X) * (*X);
                *(K + 3) += (*X) * (*X) * (*Y);
                *(K + 6) += (*X);
                *(K + 7) += (*X) * (*Y);
                *(K + 11) += (*Y);
        }
        *(K + 4) = *(K + 1);//一样的Σ元素
        *(K + 5) = *(K + 2);
        *(K + 8) = *(K + 2);
        *(K + 9) = *(K + 6);
        return 0;
}

/***********************************************************************************
系数矩阵变换，解矩阵方程
***********************************************************************************/
int TransK(double* K)//应该是消元法把左矩阵化成单位矩阵，则右矩阵(K 3 7 11)即是解
{
        //
        *(K + 5) = (*(K + 5)) * (*K) - (*(K + 4)) * (*(K + 1));
        *(K + 6) = (*(K + 6)) * (*K) - (*(K + 4)) * (*(K + 2));
        *(K + 7) = (*(K + 7)) * (*K) - (*(K + 4)) * (*(K + 3));
        *(K + 4) = 0;
        *(K + 9) = (*(K + 9)) * (*K) - (*(K + 8)) * (*(K + 1));
        *(K + 10) = (*(K + 10)) * (*K) - (*(K + 8)) * (*(K + 2));
        *(K + 11) = (*(K + 11)) * (*K) - (*(K + 8)) * (*(K + 3));
        *(K + 8) = 0;
        //
        //
        *(K + 10) = (*(K + 10)) * (*(K + 5)) - (*(K + 9)) * (*(K + 6));
        *(K + 11) = (*(K + 11)) * (*(K + 5)) - (*(K + 9)) * (*(K + 7));
        *(K + 9) = 0;
        //
        //
        *(K + 5) = (*(K + 5)) * (*(K + 10));
        *(K + 7) = (*(K + 7)) * (*(K + 10)) - (*(K + 6)) * (*(K + 11));
        *(K + 6) = 0;
        //
        //
        *K = (*K) * (*(K + 5));
        *(K + 2) = (*(K + 2)) * (*(K + 5));
        *(K + 3) = (*(K + 3)) * (*(K + 5)) - (*(K + 1)) * (*(K + 7));
        *(K + 1) = 0;
        //
        //
        *K = (*K) * (*(K + 10));
        *(K + 3) = (*(K + 3)) * (*(K + 10)) - (*(K + 2)) * (*(K + 11));
        *(K + 2) = 0;
        //
        //
        if (0 != *(K + 00))
        {
                *(K + 3) /= *(K + 00);
                *K = 1.0;
        }
        if (0 != *(K + 5))
        {
                *(K + 7) /= *(K + 5);
                *(K + 5) = 1.0;
        }
        if (0 != *(K + 10))
        {
                *(K + 11) /= *(K + 10);
                *(K + 10) = 1.0;
        }
        //
        return 0;
}

//y = ax^2 + b x + c
/***********************************************************************************
***********************************************************************************/
int Cal(double * yi, int startCur, int endCur, double* ParaA, double* ParaB, double* ParaC)
{
	int len = endCur - startCur /*+ 1*/;
	double *BufferX = new double[len];
	double *BufferY = new double[len];

	for(int i=0;i<len;i++)
	{
		BufferX[i] = i;
		BufferY[i] = yi[startCur + i];
	}

        double ParaK[12];
        int Amount = len;
        //GetXY(FileName, (double*)BufferX, (double*)BufferY, &Amount);
        GetK((double*)ParaK, (const double*)BufferX, (const double*)BufferY, Amount);
        TransK((double*)ParaK);
        *ParaA = ParaK[3];
        *ParaB = ParaK[7];
        *ParaC = ParaK[11];

		delete[] BufferX;
		delete[] BufferY;

        return 0;
}
  
  
// y = c0+c1x+c2x^2
double* GetCoefficent(double* t, double* y, int m, int n)
//最小二乘拟合 t,y分别是x,y值 m:数据个数 最后得到系数在c数组中,n 系数个数
{
  //int n = 3;
  int i, j, k;
  double tki, tkj;
  double* q = new double[n];
  double* c = new double[n];
  double** p = new double* [n];

  for (i = 0; i < n; i++)p[i] = new double[n];
  // init
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)p[i][j] = 0.;
    q[i] = 0.;
  }
  // 建立矩阵
  for (k = 0; k < m; k++)
  {
    tki = 1.;
    for (i = 0; i < n; i++)
    {
      tkj = 1.;

      for (j = 0; j < n; j++) { p[i][j] += tki * tkj; tkj = tkj * t[k]; }

      q[i] += y[k] * tki;
      tki = tki * t[k];
    }
  }
  // 转换成上三角
  for (k = 0; k < n; k++)for (i = k + 1; i < n; i++)
  {
    q[i] = q[i] * p[k][k] - q[k] * p[i][k];
    for (j = n - 1; j >= k; j--)p[i][j] = (p[i][j] * p[k][k] - p[k][j] * p[i][k]);
  }
  // 计算系数
  for (i = n - 1; i > -1; i--)
  {
    c[i] = q[i];
    for (j = n - 1; j > i; j--)c[i] -= c[j] * p[i][j];
    c[i] /= p[i][i];
  }

  for (i = 0; i < n; i++)delete[]p[i];

  delete[]p;
  delete[]q;

  return c;
}
  
  // 提取一般面积处的cur,后期可以考虑抛物线
double GetMidCur(double * yi, int startCur, int endCur)
{
  //多项式
  int num = endCur - startCur;
  double* t = new double[num];
  for (int i = 0; i < num; i++)t[i] = i;
  int n = 5;
  double* c = GetCoefficent(t, yi+ startCur, num, n);

  double max = 0, max_val = 0;
  for (int i = startCur; i < endCur; i++) {
    yi[i] = c[0];

    for (int j = 1; j < n; j++) {
      yi[i] += c[j] * pow(t[i - startCur], j);
    }
    //yi[i] = c[0] + c[1] * t[i- startCur] + c[2] * t[i- startCur] * t[i- startCur];

    if (yi[i] > max_val) {
      max_val = yi[i];
      max = i;

    }
  }
  return max;

	//抛物线
	double ParaA,ParaB,ParaC;
	Cal(yi,startCur,endCur,&ParaA,&ParaB,&ParaC);

  //for (int i = startCur; i < endCur; i++) {
  //  yi[i] = ParaC + ParaB * (i - startCur) + ParaA * (i - startCur) * (i - startCur);
  //}

	return -ParaB/(2*ParaA)+startCur;
	//宽度中间
	//double mid = (startCur+endCur)/2.0 +0.5;
	//return mid;
	//面积中间
	double area=0;
	int i;
	for(i = startCur; i < endCur; i++) 
	{
		area+=yi[i];
	}
	area/=2;
	double tmp_area = 0;
	for(i = startCur; i < endCur; i++)
	{
		tmp_area+=yi[i];
		if(tmp_area>=area)
			break;
	}

	double d1 = area-tmp_area;
	double d2 = area-(tmp_area-yi[i]);

	if(d1<d2)
		return i;
	else
		return i-1;

}


// 提取尖峰
// 指定一个点，超过这个点一定数量的尖峰，认为合格。
// 返回整顿后的数组大小
  // ignore 忽略多少个连续不合格点，即认为合格,故confirmNum需包含不和格数量
//int EfgAlg::ExtractSpike(double * yi, int iNum, double threshold/*阈值*/, int confirmNum/*确认数量*/, double ignore/*忽略值*/)
//{
//  int outLimitCnt = 0;// 超限点计数
//  int ignoreCnt = 0; //忽略点计数181116
//  //int cur = 0;
//  // -1 没有累计到的超限点
//  int limitCur = -1;// 第一个超限点
//  //int limitCnt = 0;
//  double d = 0;
//  int max = 0;// 最大超限点
//  confirmNum += ignore;
//  m_spikes.RemoveAll();
//
//  for (int i = 0; i < iNum; i++)
//  {
//  //  if (ignore == yi[i])
//  //  {
//  //    if (-1 == limitCur)
//  //    {
//  //      ;
//  //    }
//  //    else
//  //    {
//  //      outLimitCnt++;
//		//ignoreCnt++;
//  //    }
//  //    continue;
//  //  }
//
//    d = yi[i] - threshold;
//
//    if (d > 0 && i < iNum-1)
//    {
//      if (-1 == limitCur)
//      {
//        limitCur = i;
//        outLimitCnt = 1;
//        max = i;
//      }
//      else
//      {
//        if (yi[max] < yi[i])
//          max = i;
//
//        outLimitCnt++;
//      }
//	  ignoreCnt = 0;
//    }
//    else//d<=0
//    {
//      if (-1 == limitCur)
//      {
//        ;
//      }
//	  else if (ignore > ignoreCnt)
//      {
//
//        outLimitCnt++;
//		ignoreCnt++;
//      
//        continue;
//      }
//      else if(outLimitCnt >= confirmNum) // 存
//      {
//       // Spike spike = { limitCur, i - 1 };
//		  //181215
//		max =  GetMidCur(yi, limitCur, i-1-ignoreCnt);
//        SPIKE spike;
//        spike.p.x = max;//(limitCur + i - 1) / 2;
//        spike.p.y = yi[max];
//        spike.w = outLimitCnt;
//        m_spikes.Add(spike);
//      }
//      limitCur = -1;
//      outLimitCnt = 0;
//	  ignoreCnt = 0;
//    }
//  }
//
//
//  return 0;
//}

// 提取尖峰
// 指定一个点，超过这个点一定数量的尖峰，认为合格。
// 返回整顿后的数组大小
  // ignore 忽略多少个连续不合格点，即认为合格,故confirmNum需包含不和格数量
int EfgAlg::ExtractSpike(double * yi, int iNum, double threshold/*阈值*/, int confirmNum/*确认数量*/, double ignore/*忽略值*/)
{
  int outLimitCnt = 0;// 超限点计数
  int ignoreCnt = 0; //忽略点计数181116
  //int cur = 0;
  // -1 没有累计到的超限点
  int limitCur = -1;// 第一个超限点
  //int limitCnt = 0;
  double d = 0;
  double max = 0;// 最大超限点
  //confirmNum += ignore;
  m_spikes.RemoveAll();
  bool lastOne = false;

  for (int i = 0; /*i < iNum*/; i++)
  {
	  if(i >= iNum &&limitCur!=-1)
	  {
		  i = 0;
		  lastOne = true;
	  }
	  else if(i < iNum && false == lastOne)
	  {
		  ;
	  }
	  else if(true == lastOne)
	  {
		  ;
	  }
	  else
	  {
		  break;
	  }
  //  if (ignore == yi[i])
  //  {
  //    if (-1 == limitCur)
  //    {
  //      ;
  //    }
  //    else
  //    {
  //      outLimitCnt++;
		//ignoreCnt++;
  //    }
  //    continue;
  //  }

    d = yi[i] - threshold;

    if (d > 0 /*&& i < iNum-1*/)
    {
      if (-1 == limitCur)
      {
        limitCur = i;
        outLimitCnt = 1;
        max = i;
      }
      else
      {
        if (yi[int(max)] < yi[i])
          max = i;
        outLimitCnt++;
      }
	  ignoreCnt = 0;
    }
    else//d<=0
    {
      if (-1 == limitCur)
      {
        ;
      }
	  else if (ignore > ignoreCnt)
      {

        outLimitCnt++;
		ignoreCnt++;
      
        continue;
      }
      else if(outLimitCnt >= confirmNum) // 存
      {
       // Spike spike = { limitCur, i - 1 };
		  //181215
		int lastCur = i-1-ignoreCnt;

		if(lastCur <0)
			lastCur=iNum+lastCur;

		double * tmp_yi = new double[outLimitCnt];//防止跨边界
		int point_num  =0;
		if(lastCur >=limitCur)
		{
			memcpy((char*)tmp_yi, (char*)(yi+limitCur), (lastCur-limitCur+1)*sizeof(double));
			point_num+=lastCur-limitCur+1;
		}
		else
		{
			memcpy((char*)tmp_yi, (char*)(yi+limitCur), (iNum - limitCur)*sizeof(double));
			point_num+=iNum - limitCur;
			memcpy((char*)(tmp_yi+iNum - limitCur), (char*)yi, (lastCur+1)*sizeof(double));
			point_num+=lastCur+1;
		}

		//max =  GetMidCur(yi, limitCur, lastCur);
		max =  GetMidCur(tmp_yi, 0, point_num);
    // copy to yi
    if (lastCur >= limitCur)
    {
      memcpy((char*)(yi + limitCur), (char*)tmp_yi, (lastCur - limitCur + 1) * sizeof(double));
      //point_num += lastCur - limitCur + 1;
    }
    else
    {
      memcpy( (char*)(yi + limitCur),(char*)tmp_yi, (iNum - limitCur) * sizeof(double));
      //point_num += iNum - limitCur;
      memcpy( (char*)yi,(char*)(tmp_yi + iNum - limitCur), (lastCur + 1) * sizeof(double));
      //point_num += lastCur + 1;
    }


		max+=limitCur;
		max=max>=iNum?max-iNum:max;

		delete tmp_yi;

		if(lastOne == true)//第二次遍历，查找与第一个点的距离是否太小
		{
			SPIKE tmp =  m_spikes.GetAt(0);

			int d1 = abs(max - tmp.p.x);
			int d2 = iNum - d1;
			int limit= (confirmNum+ignore)*10;
			if(d1< limit|| d2<limit)
				m_spikes.RemoveAt(0);;
			//以处理最后一个点，以下退出循环上
			lastOne= false;
			i = iNum;

		}
		
		if(1/*max>=limitCur && max<=i-1-ignoreCnt*/)
		{
			SPIKE spike;
            spike.x = (max>=0?max:iNum+max);
			spike.p.x = spike.x+0.5;//(limitCur + i - 1) / 2;
			spike.p.y = yi[spike.p.x];
			spike.w = outLimitCnt;
			
			m_spikes.Add(spike);
		}
      }
      limitCur = -1;
      outLimitCnt = 0;
	  ignoreCnt = 0;
    }
  }


  return 0;
}

int EfgAlg::GetSpikesNumber()
{
  return m_spikes.GetCount();
}

int EfgAlg::GetSpike(int number, SPIKE& spike)
{
  spike = m_spikes.GetAt(number);
  return 0;
}
int EfgAlg::DelSpike(int number)
{
  m_spikes.RemoveAt(number);
  return 0;
}
//x4个尖峰的x
int EfgAlg::UpdateSpikeX(double* x)
{
  SPIKE p[4];
  for (int i = 0; i < 4; i++)
  {
    p[i] = m_spikes.GetAt(i);
	p[i].x = x[i];
	p[i].p.x = x[i]+0.5;
  }

  m_spikes.RemoveAll();

  for(int i = 0 ; i < 4; i ++)
  {
	  m_spikes.Add(p[i]);
  }

  return 0;
}
// 根据中心即步长等得出所有档位的档位中心值
BOOL EfgAlg::GetAllSortDegree(int * out_sec, int in_center_sec, int step_sec, int sort_num)
{
  if (!out_sec)
    return FALSE;
  int low_num = sort_num / 2;
  int high_num = sort_num - low_num - 1;

  int high_degree = in_center_sec + high_num * step_sec;
  out_sec[0] = in_center_sec - low_num * step_sec;

  int i = 1;
  while (i < sort_num)
  {
    out_sec[i] = out_sec[i - 1] + step_sec;
    i++;
  }


  return TRUE;
}
// 利用间距计算d1d2dm,最大间距过来的第一个最小间距是D1，然后D2的中间距离零位距离是R1
// 输出单位°
BOOL EfgAlg::GetD1D2DM(double &D1, double &D2, double &DM, double &R1,int pluse_num)
{
  if (!SortSpike(pluse_num))
  {
	D1 = 0;
	D2 = 0;
	DM = 0;
    return FALSE;
  }


  SPIKE p[4];
  for (int i = 0; i < 4; i++)
  {
    p[i] = m_spikes.GetAt(i);
  }

  double d_D1 = p[1].p.x - p[0].p.x;
  double d_D2 = p[3].p.x - p[2].p.x;
  D1 = d_D1 < 0 ? d_D1 + pluse_num : d_D1;
  D2 = d_D2 < 0 ? d_D2 + pluse_num : d_D2;
  double d_DM, d_R1;
  if (D1 <= D2)
  {
	  d_DM = p[2].p.x - p[1].p.x;
	  DM = d_DM < 0 ? d_DM + pluse_num : d_DM;
	  DM += (D1 + D2) / 2;
	  d_R1 = p[2].p.x + D2 / 2;

  }
  else
  {
	  
  D2 = d_D1 < 0 ? d_D1 + pluse_num : d_D1;
  D1 = d_D2 < 0 ? d_D2 + pluse_num : d_D2;
	  d_DM = p[0].p.x - p[3].p.x;
	  DM = d_DM < 0 ? d_DM + pluse_num : d_DM;
	  DM += (D1 + D2) / 2;
	  d_R1 = p[0].p.x + D2 / 2;
  }
	  R1 = d_R1 >= pluse_num ? d_R1 - pluse_num : d_R1;
	  D1 = D1 * 360 / pluse_num;
	  D2 = D2 * 360 / pluse_num;
	  DM = DM * 360 / pluse_num;
	  R1 = R1 * 360 / pluse_num;
  //D2在d1前面，调整
  if (DM > 180)
  {
    return FALSE;
  }
  return TRUE;
}
// 利用高度/宽度计算d1d2dm,factor调节用于较准确的计算最小差值
//BOOL EfgAlg::GetD1D2DM(double &D1, double &D2, double &DM, int pluse_num, double factor_h, double factor_w)
//{
//  if (!SortSpike(factor_h, factor_w))
//    return FALSE;
//
//  SPIKE p[4];
//  for (int i = 0; i < 4; i++)
//  {
//    p[i] = m_spikes.GetAt(i);
//  }
//
//  D1 = double(abs(p[0].p.y - p[1].p.y)) * 360 / pluse_num;
//  D2 = double(abs(p[2].p.y - p[3].p.y)) * 360 / pluse_num;
//  DM = double(abs((p[0].p.y + p[1].p.y) / 2 - (p[2].p.y + p[3].p.y) / 2)) * 360 / pluse_num;
//  //D2在d1前面，调整
//  if (D2 < D1)
//  {
//    double tmp;
//    tmp = D1;
//    D1 = D2;
//    D2 = tmp;
//    DM = 360 - DM;
//  }
//}

// 将spike点排序，间距最长的右侧点排在第一,则01 d1，23 d2
// 根据经验D1<D2,D1+D2=90°，dm<180°
BOOL EfgAlg::SortSpike(int pluse_num)
{
  if (GetSpikesNumber() != 4)
    return FALSE;

  double dval[4] = { 0 };
  int max_i = 0;;

  for (int i = 0; i < 4; i++)
  {
    int j = i + 1 > 3 ? 0 : i + 1;

    SPIKE p1 = m_spikes.GetAt(i);
    SPIKE p2 = m_spikes.GetAt(j);
    int d = p2.p.x - p1.p.x;
    dval[i] = d < 0 ? d + pluse_num : d;
    if (dval[i] > dval[max_i])//SC > ;AT <
      max_i = i;//找到最小因子的一对的起始序号
  }
  //max_i=max_i + 1 > 3 ? 0 : max_i + 1;
  for (int i = 0; i <= max_i; i++)//把前面的滚到后面
  {
    SPIKE spike;
    spike = m_spikes.GetAt(0);
    m_spikes.RemoveAt(0);
    m_spikes.Add(spike);
  }
  return TRUE;
}

// 将spike点排序，01一对，23一对
// h方向 w方向累计差最小的是一对
BOOL EfgAlg::SortSpike(double factor_h, double factor_w)
{
  if (GetSpikesNumber() != 4)
    return FALSE;

  double dval[4] = { 0 };
  int tmp_i = 0;;

  for (int i = 0; i < 4; i++) 
  {
    int j = i + 1 > 3 ? 0 : i + 1;

    SPIKE p1 = m_spikes.GetAt(i);
    SPIKE p2 = m_spikes.GetAt(j);
    int dh = abs(p1.p.y - p2.p.y);
    int dw = abs(p1.w - p2.w);
    dval[i] = dh * factor_h + dw * factor_w;
    if (dval[i] < dval[tmp_i])
      tmp_i = i;//找到最小因子的一对的起始序号
  }

  for (int i = 0; i < tmp_i; i++)//把前面的滚到后面
  {
    SPIKE spike;
    spike = m_spikes.GetAt(i);
    m_spikes.RemoveAt(i);
    m_spikes.Add(spike);
  }
  return 0;
}

//计算dleta2的函数,返回弧度，输入参数：delta1,两个原子面夹角，DM
double cal_dleta2(double dleta1, double dleta3, double dm) {
  double tempd1, tempd2, tempd, tempa;
  double cosd1, cosd2, cosd3, cosdm, sind1;
  cosd1 = cos(dleta1*DPI);
  cosd3 = cos(dleta3*DPI);
  cosdm = cos(dm*DPI);
  sind1 = sin(dleta1*DPI);
  tempa = cosdm * cosdm*sind1*sind1 + cosd1 * cosd1;
  tempd = cosd3 * cosd3*cosd1*cosd1 - tempa * (cosd3*cosd3 - cosdm * cosdm*sind1*sind1);
  tempd1 = (cosd3*cosd1 + sqrt(tempd)) / tempa;
  tempd2 = (cosd3*cosd1 - sqrt(tempd)) / tempa;
  if (tempd1>tempd2)
    return acos(tempd1) / DPI;
  else
    return acos(tempd2) / DPI;
}
//计算g值，返回：g值，单位为°；输入参数：衍射角（°），原子面的法线和表面的法线的夹角，绕晶片法线的旋转角度
double cal_gvalue(double thetaB, double delta, double phi) {
  double cosb, cosd, cosp, sind, sinp;
  double tempb, tempd, tempa;
  double tempg1, tempg2;
  cosb = cos(thetaB*DPI);
  cosd = cos(delta*DPI);
  cosp = cos(phi*DPI);
  sind = sin(delta*DPI);
  sinp = sin(phi*DPI);
  tempd = cosb * cosb - sind * sind*cosp*cosp;
  tempa = cosd * cosd + sind * sind*sinp*sinp;
  tempb = 4.0*tempd*cosd*cosd - 4.0*tempa*(tempd - sind * sind*sinp*sinp);
  tempg1 = acos((sqrt(tempd)*2.0*cosd + sqrt(tempb)) / 2.0 / tempa) / DPI;
  tempg2 = acos((sqrt(tempd)*2.0*cosd - sqrt(tempb)) / 2.0 / tempa) / DPI;
  if (abs(tempg1 - 47.0)>abs(tempg2 - 47.0))   //返回更接近47的数值
    return tempg2;
  else
    return tempg1;
}
//根据delta、g和旋转角度计算衍射角余弦值的平方
//输入参数：
double cal_thetab(double delta, double g, double phi) {
  double sind, cosd, sing, cosg, sinp, cosp;
  double tempb;
  sind = sin(delta*DPI);
  cosd = cos(delta*DPI);
  sing = sin(g*DPI);
  cosg = cos(g*DPI);
  sinp = sin(phi*DPI);
  cosp = cos(phi*DPI);
  tempb = sind * sind*cosp*cosp + (cosg*cosd - sing * sind*sinp)*(cosg*cosd - sing * sind*sinp);
  //tempb=acos(sqrt(tempb))/DPI;
  return tempb;
}
//根据delta1和delta2计算theta角度
//double

//计算原始光轴/原始电轴/u轴g ，单位都是°
void EfgAlg::CalcDegree0(const double D1, const  double D2, const double DM, double &theta0, double &phi0, double &u_g)
{
	if(D1==0||D2==0||DM==0)
	{
		theta0=0;
		phi0=0;
		u_g=0;
		return ;
	}
  // TODO: 在此添加控件通知处理程序代码
  double h1 = -1;
  double k1 = 3;
  double l1 = 1;
  double h2 = -1;
  double k2 = 3;
  double l2 = 3;
  double ca = 1.09973485621048;   //晶胞常数比例,通常按照1.1
  double cosyzm11, cosyzm12, cosyzm13, cosyzm21, cosyzm22, cosyzm23, cosdl3;
  double temps1, temps2, tdelta2, tg, tb1, tb2;
  double delta3 = 25.1872615573149;  //两个原子面的法线的夹角，单位为°
  double delta1 = 17.10;   //delta的起始值
  double d1, d2, dm;
  double sintheta, sinphi, theta, phi1, phi2;
  double tvalue, temp, tdelta1, cosdelta1, cosdelta2;
  double wa, wb, wc, tempw, sinphi1, sinphi2;
  CString str;
  //CString strD1, strD2, strDM;
  //GetDlgItem(D1_EDIT1)->GetWindowText(strD1);
  //GetDlgItem(D1_EDIT2)->GetWindowText(strD2);
  //GetDlgItem(D1_EDIT3)->GetWindowText(strDM);
  //原子面1方向余弦
  temps1 = sqrt(4.0 / 3.0*(h1*h1 + h1 * k1 + k1 * k1) + l1 * l1 / ca / ca);
  cosyzm11 = h1 / temps1;
  cosyzm12 = (h1 + 2.0*k1) / sqrt(3.0) / temps1;
  cosyzm13 = l1 / ca / temps1;
  //原子面2方向余弦
  temps2 = sqrt(4.0 / 3.0*(h1*h2 + h2 * k2 + k2 * k2) + l2 * l2 / ca / ca);
  cosyzm21 = h2 / temps2;
  cosyzm22 = (h2 + 2.0*k2) / sqrt(3.0) / temps2;
  cosyzm23 = l2 / ca / temps2;
  //原子面1和原子面2的夹角的余弦
  cosdl3 = cosyzm11 * cosyzm21 + cosyzm12 * cosyzm22 + cosyzm13 * cosyzm23;
  //	temp=cal_dleta2(17.9781922,delta3,138.1541);    //根据一个delta计算另一个delta

  //	temp=cal_gvalue(29.977282,17.9781922,283.1136);   //计算g值
  //根据g值和delta值计算，差值
  //	temp=cal_thetab(17.9781922,47.5971,283.1136);   //计算衍射角
  //对delta1进行循环，
  //d1 = _wtof(strD1.GetBuffer(0)) / 2 + 270.0;
  //d2 = _wtof(strD2.GetBuffer(0)) / 2 + 270.0;
  //dm = _wtof(strDM.GetBuffer(0));
  d1 = D1 / 2 + 270.0;
  d2 = D2 / 2 + 270.0;
  dm = DM;
  tvalue = 1e-5;
  tdelta1 = 0.1;
  while (true) {
    delta1 += tdelta1;   //设定一个原子面法线和晶片表面法线的夹角值
    tdelta2 = cal_dleta2(delta1, delta3, dm);  //计算delta2值
    tg = cal_gvalue(29.977282, delta1, d1);   //计算g值
    tb1 = cal_thetab(delta1, tg, d1);   //计算衍射角的余弦值的平方
    tb2 = cal_thetab(tdelta2, tg, d2);   //计算衍射角的余弦值的平方
    temp = temps2 * temps2*(1 - tb1) - temps1 * temps1*(1 - tb2);
    //if(temp>0) tvalue=temp;
    if (temp<0) {
      tdelta1 = -0.00001;
    }
    if (tdelta1<0 && temp>0) break;
    if (delta1>18.5) break;
  }
  //计算切割角度theta和phi
  cosdelta1 = cos(delta1*DPI);
  cosdelta2 = cos(tdelta2*DPI);
  sintheta = ((cosyzm13*cosdelta2 - cosyzm23 * cosdelta1)*(cosyzm21*cosyzm12 - cosyzm11 * cosyzm22) / (cosyzm21*cosyzm13 - cosyzm11 * cosyzm23) - (cosyzm11 + cosyzm12)*cosdelta2 + (cosyzm21 + cosyzm22)*cosdelta1) / (cosyzm13*cosyzm21 - cosyzm23 * cosyzm11 + cosyzm13 * cosyzm22 - cosyzm23 * cosyzm12);
  theta = asin(sintheta);   //单位弧度
                            //计算PHi角度 wa sinw2+wb sinw+c=0
  tempw = cosdelta1 - cosyzm13 * sintheta;
  wa = (cosyzm11*cosyzm11 + cosyzm12 * cosyzm12)*cos(theta)*cos(theta);
  wb = 2 * tempw*cosyzm11*cos(theta);
  wc = tempw * tempw - cosyzm12 * cosyzm12*cos(theta)*cos(theta);
  sinphi1 = (-wb - sqrt(wb*wb - 4 * wa*wc)) / 2 / wa;
  sinphi2 = (-wb + sqrt(wb*wb - 4 * wa*wc)) / 2 / wa;
  phi1 = asin(sinphi1);
  phi2 = asin(sinphi2);
  //此次需要判断phi角度，19°06′以上还是以下，IT需要操作人员输入确定
  //str.Format(_T("%lf"), phi2);
  //MessageBox(str, _T("Tip"));
  //str.Format(_T("%lf"), theta / DPI);
  //GetDlgItem(IDC_EDIT2)->SetWindowText(str);
  //str.Format(_T("%lf"), phi2 / DPI);
  //GetDlgItem(IDC_EDIT3)->SetWindowText(str);
  //str.Format(_T("%lf"), tg);
  //GetDlgItem(IDC_EDIT4)->SetWindowText(str);
  theta0 = theta / DPI;
  phi0 = phi2 / DPI;
  u_g = tg;
}

//计算∠X"L
//单位 度
//X1st = X'
//X2nd = X"
double CalcX2ndL1(const double theta0, const double phi0)
{
  //原子面1方向余弦
  double theta = theta0 * DPI;//角度转换到弧度
  double phi = phi0 * DPI;
  double h = -1;
  double k = 3;
  double l = 3;
  double s = 4.0 / 3 * (h * h + h * k + k * k) + l * l / 1.21;
  //原子面方向余弦
  double cos_an = h / sqrt(s);
  double cos_bn = (h + 2 * k) / sqrt(3 * s);
  double cos_rn = l / (1.1*sqrt(s));
  //测量面方向余弦
  double cos_a1st = -cos(theta)*sin(phi);
  double cos_b1st = cos(theta)*cos(phi);
  double cos_r1st = sin(theta);
  //原子面1与测量面的交线 L1
  //L1的方向数
  //double p = cos_bn * sin(theta) + cos_rn * cos(theta)*cos(phi);//excel b34
  //double q = cos_rn * (-cos(theta)) + cos_an * sin(theta);
  //double r = cos_an * cos(theta)*cos(phi) + cos_bn * (-cos(theta));

  double p = cos_bn * cos_r1st - cos_rn * cos_b1st;//excel b34
  double q = cos_rn * cos_a1st - cos_an * cos_r1st;
  double r = cos_an * cos_b1st - cos_bn * cos_a1st;

  //X2nd的方向数
  double px = cos(phi);
  double qx = sin(phi);
  double rx = 0;
  //L1和X"轴的夹角，∠X"L
  double x2nd_l = acos((p * px + q * qx + r * rx) / 
    (sqrt(p * p + q * q + r * r) * sqrt(px * px + qx * qx + rx * rx)));
  return x2nd_l / DPI;
}
/*
amp	激光测量的修正角度（正弦拟合的幅度值）
      amp = atan (A * factor_a / factor_l) 转换成°
PHASE	相位值（正弦拟合的相位值 + 180）
      sin 的 x∈（0，iNum）转换到 x∈（0，2 * pi）,w就是1，故phase = sinparam.t转换成°
R1	测量结果中的R1值（原子面和晶片表面交线相对于零位的偏移量）
LaserOffset	设置的激光相对偏移角度（激光入射光线和零位的夹角）
θ	晶片表面相对的θ角度（没有修正）
φ	同上类似

单位统一为°
dm 》 180 正面

*/
void EfgAlg::CalcDegree1(double amp, double phase,double r1, double laser_offset,double theta0, double phi0, double &theta1, double &phi1, double dm)
{
	if(theta0==0||phi0==0)
	{
		theta1=0;
		phi1=0;
		return ;
	}

  double x2nd_l = CalcX2ndL1(theta0, phi0);

  double phase1 = ((dm > 180)?1:-1)*(-90 - laser_offset - phase+ r1) + x2nd_l;

  

  //转换到弧度
  double am = amp * DPI;
  double PHASE = phase * DPI;
  double R1 = r1 * DPI;
  double laserOffset = laser_offset * DPI;
  double theta = theta0 * DPI;
  double phi = phi0 * DPI;
  double X2ndL = x2nd_l * DPI;
  //double DM = dm * DPI;

  double PHASE1 = phase1 * DPI;//excel h10

  //计算测量时的晶片表面和测量基准面交线的方向余弦
  double nx = cos(PHASE1)*cos(phi) + sin(PHASE1)*sin(theta)*sin(phi);
  double ny = cos(PHASE1)*sin(phi) - sin(PHASE1)*sin(theta)*cos(phi);
  double nz = sin(PHASE1)*cos(theta);
  //变换矩阵
  double c11 = nx * nx*(1 - cos(am)) + cos(am);
  double c12 = nx * ny*(1 - cos(am)) + nz * sin(am);
  double c13 = nx * nz*(1 - cos(am)) - ny * sin(am);
  double c21 = nx * ny*(1 - cos(am)) - nz * sin(am);
  double c22 = ny * ny*(1 - cos(am)) + cos(am);
  double c23 = ny * nz*(1 - cos(am)) + nx * sin(am);
  double c31 = nx * nz*(1 - cos(am)) + ny * sin(am);
  double c32 = ny * nz*(1 - cos(am)) - nx * sin(am);
  double c33 = nz * nz*(1 - cos(am)) + cos(am);
  //测量面的方向余弦
  double cos_a1st = -cos(theta)*sin(phi);
  double cos_b1st = cos(theta)*cos(phi);
  double cos_r1st = sin(theta);
  //计算基准面的方向余弦
  double a0 = c11 * cos_a1st + c12 * cos_b1st + c13 * cos_r1st;
  double b0 = c21 * cos_a1st + c22 * cos_b1st + c23 * cos_r1st;
  double r0 = c31 * cos_a1st + c32 * cos_b1st + c33 * cos_r1st;
  /*double a0 = acos(cos_a0);
  double b0 = acos(cos_b0);
  double r0 = acos(cos_r0);*/
  // 计算修正后的θ和φ
  theta1 = asin(r0) / DPI;
  phi1 = acos(b0 / sqrt(1 - r0 * r0)) / DPI;
}

//单位必须一致，比如°
void EfgAlg::CalcEquAngle(double theta0, double phi0, double equ_phi, double equ_factor, double &equ)
{
  // 等效角 = 原始光轴 + （原始电轴 - 等效角电轴参数）/等效角因子
  if (equ_factor&&theta0&&phi0&&equ_phi) {
    equ = theta0 + double(phi0 - equ_phi)/* * 1000 *// equ_factor + 0.5;
  }
  else {
    equ = 0;
  }

  return;
}

void EfgAlg::SortAvgStd(const int sum, const double angle, double &avg, double &std, double &std2)
{
  double n = sum;

  double xi = angle;
  //double avg;
  if (1 == n)
  {
    avg = xi;//平均值
    std2 = xi * xi;//标准偏差中间值
  }
  if (n > 1)
  {
    avg = (avg + xi / (n - 1)) / (n / (n - 1));//求平均值
    std2 = (std2 + xi * xi / (n - 1)) / (n / (n - 1));//求标准差中间值
    std = sqrt(abs(std2 - avg * avg)*n / (n - 1));//求标准差
  }
}