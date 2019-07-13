#include "StdAfx.h"
#include "EfgAlg.h"
#include <math.h>

EfgAlg::EfgAlg()
{
}


EfgAlg::~EfgAlg()
{
}

// һά�˲�����Ϣ�ṹ��
typedef  struct{
	double filterValue;  //k-1ʱ�̵��˲�ֵ������k-1ʱ�̵�ֵ
	double kalmanGain;   //   Kalamn����
	double A;   // x(n)=A*x(n-1)+u(n),u(n)~N(0,Q)
	double H;   // z(n)=H*x(n)+w(n),w(n)~N(0,R)
	double Q;   //Ԥ���������ƫ��ķ���
	double R;   //��������ƫ�(ϵͳ����Ժ�ͨ������ͳ��ʵ����)
	double P;   //�������Э����
}  KalmanInfo;
/**
* @brief Init_KalmanInfo   ��ʼ���˲����ĳ�ʼֵ
* @param info  �˲���ָ��
* @param Q Ԥ���������� ��ϵͳ�ⲿ�ⶨ����
* @param R ������������ ��ϵͳ�ⲿ�ⶨ����
*/
void Init_KalmanInfo(KalmanInfo* info, double Q, double R, double filterValue)
{
	info->A = 1;  //����������
	info->H = 15;  //
	info->P = 1.85;  //����״̬����ֵ���ķ���ĳ�ʼֵ����ҪΪ0���ⲻ��
	info->Q = Q;    //Ԥ�⣨���̣��������� Ӱ���������ʣ����Ը���ʵ���������
	info->R = R;    //�������۲⣩�������� ����ͨ��ʵ���ֶλ��
	info->filterValue = filterValue;// �����ĳ�ʼֵ 0 
}
double KalmanFilter(KalmanInfo* kalmanInfo, double lastMeasurement)
{
	//Ԥ����һʱ�̵�ֵ
	double predictValue = kalmanInfo->A* kalmanInfo->filterValue;   //x�������������һ��ʱ���ĺ������ֵ��������Ϣ�������˴���Ҫ���ݻ�վ�߶���һ���޸�
	
	//��Э����
	kalmanInfo->P = kalmanInfo->A*kalmanInfo->A*kalmanInfo->P + kalmanInfo->Q;  //������������� p(n|n-1)=A^2*p(n-1|n-1)+q
	double preValue = kalmanInfo->filterValue;  //��¼�ϴ�ʵ�������ֵ
 
	//����kalman����
	kalmanInfo->kalmanGain = kalmanInfo->P*kalmanInfo->H / (kalmanInfo->P*kalmanInfo->H*kalmanInfo->H + kalmanInfo->R);  //Kg(k)= P(k|k-1) H�� / (H P(k|k-1) H�� + R)
	//����������������˲�ֵ
	kalmanInfo->filterValue = predictValue + (lastMeasurement - predictValue)*kalmanInfo->kalmanGain;  //���ò������Ϣ���ƶ�x(t)�Ĺ��ƣ�����������ƣ����ֵҲ�������  X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1))
	//���º������
	kalmanInfo->P = (1 - kalmanInfo->kalmanGain*kalmanInfo->H)*kalmanInfo->P;//������������  P[n|n]=(1-K[n]*H)*P[n|n-1]
 
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


//�������ƽ��ֵ����׼����
double EfgAlg::Avg(int num, int *th)
{

  double sum = 0;

  for (int i = 0; i< num; i++)//����ԭʼTH���м�ֵ
  {
    sum += double(th[i]);
  }

  return sum / double(num);
}

//�������ƽ��ֵ����׼����
double EfgAlg::Avg(int num, double *th)
{

  double sum = 0;

  for (int i = 0; i< num; i++)//����ԭʼTH���м�ֵ
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
//��װ��ϲ��������yi
void EfgAlg::CalFitYi(double *fityi, int iNum, struct tagSinParam sinparam)
{
  for (int i = 0; i< iNum; i++)
  {
    fityi[i] = SinAwtk(sinparam, i);
  }
}
//�������ٺ�������С
int EfgAlg::SmoothYi(double * yi, int iNum, double limit/*������ֵ*/, int confirmLimitNum/*���㼴����*/,int flag/*���ޱ��*/)
{
  int outLimitCnt = 0;
  // int cur = 0;
  int limitCur = 0;
  // int limitCnt = 0;
  double d = 0;

  for (int i = 0; i < iNum-1; i++)
  {
    outLimitCnt = i - limitCur;//
    d = fabs(yi[i + 1] - yi[i]);//ǰ�������
    if (d < limit && i < iNum - 2)
    {
      continue;
    }
    else // out limit
    {
      if (outLimitCnt < confirmLimitNum)//��ȷ��״̬
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
//����ƽ�xi �ȼ��
int EfgAlg::FitSinBySubstitution(double * yi/*���ԭʼyֵ*/, int iNum, double *fityi/*�����Ϻ��ֵ*/, struct tagSinParam &sinparam)
{
  double A = 1;
  const double w = 2*PI/ iNum;
  double t = 0;
  double k = Avg(iNum, yi);//�������k
  double  ldv_MinDAvg = 0, ldv_TmpDAvg = 0;
  double  ldv_MinDStd = 0, ldv_TmpDStd = 0;
  struct tagSinParam param = { 0 };

  for (int i = 0; i< iNum; i++)//�������A
  {
    A = A > fabs(double(yi[i]) - k) ? A : fabs(double(yi[i]) - k);
  }

  param.A = A;
  param.k = k;
  param.t = t;
  param.w = w;

  bool lbv_Chg = 0;
  double ldv_DVal = 0;//��������ֵ������С
  CalFitYi(fityi, iNum, param);
  //�ó�ֵ
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

//���������
//�ο����Ĳ�������������ϵ�һ�������㷨��
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
	  if(yi[i] == *ignore_val)//��ʱ�ӣ�ʹ֮����
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
  //������Ϻ�����cos   cos��3�� / 2 + ���� = sin��
  t -= 3 * PI / 2;

  param.A = A;
  param.k = k;
  param.t = t;
  param.w = w;
  CalFitYi(fityi, iNum, param);

  sinparam = param;

  return 0;
}
//smooth_width���߸�cnt����
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

//smooth_width���߸�cnt����
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
 //������
  /***********************************************************************************
��txt�ļ����ȡdouble��XY����
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
��double��XY�����л�ȡϵ������
***********************************************************************************/
int GetK(double* K, const double* X, const double* Y, int Amount)
{
        *(K + 10) = Amount;//��10λ����������½Ǽ�x^0=1���ۼ������Amount
        *K = 0;
        *(K + 1) = 0;
        *(K + 2) = 0;
        *(K + 3) = 0;
        *(K + 6) = 0;
        *(K + 7) = 0;
        *(K + 11) = 0;
        for ( ; Amount; Amount--, X++, Y++)//��ͼ���
        {
                *K += (*X) * (*X) * (*X) * (*X);
                *(K + 1) += (*X) * (*X) * (*X);
                *(K + 2) += (*X) * (*X);
                *(K + 3) += (*X) * (*X) * (*Y);
                *(K + 6) += (*X);
                *(K + 7) += (*X) * (*Y);
                *(K + 11) += (*Y);
        }
        *(K + 4) = *(K + 1);//һ���Ħ�Ԫ��
        *(K + 5) = *(K + 2);
        *(K + 8) = *(K + 2);
        *(K + 9) = *(K + 6);
        return 0;
}

/***********************************************************************************
ϵ������任������󷽳�
***********************************************************************************/
int TransK(double* K)//Ӧ������Ԫ��������󻯳ɵ�λ�������Ҿ���(K 3 7 11)���ǽ�
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
//��С������� t,y�ֱ���x,yֵ m:���ݸ��� ���õ�ϵ����c������,n ϵ������
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
  // ��������
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
  // ת����������
  for (k = 0; k < n; k++)for (i = k + 1; i < n; i++)
  {
    q[i] = q[i] * p[k][k] - q[k] * p[i][k];
    for (j = n - 1; j >= k; j--)p[i][j] = (p[i][j] * p[k][k] - p[k][j] * p[i][k]);
  }
  // ����ϵ��
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
  
  // ��ȡһ���������cur,���ڿ��Կ���������
double GetMidCur(double * yi, int startCur, int endCur)
{
  //����ʽ
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

	//������
	double ParaA,ParaB,ParaC;
	Cal(yi,startCur,endCur,&ParaA,&ParaB,&ParaC);

  //for (int i = startCur; i < endCur; i++) {
  //  yi[i] = ParaC + ParaB * (i - startCur) + ParaA * (i - startCur) * (i - startCur);
  //}

	return -ParaB/(2*ParaA)+startCur;
	//����м�
	//double mid = (startCur+endCur)/2.0 +0.5;
	//return mid;
	//����м�
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


// ��ȡ���
// ָ��һ���㣬���������һ�������ļ�壬��Ϊ�ϸ�
// �������ٺ�������С
  // ignore ���Զ��ٸ��������ϸ�㣬����Ϊ�ϸ�,��confirmNum��������͸�����
//int EfgAlg::ExtractSpike(double * yi, int iNum, double threshold/*��ֵ*/, int confirmNum/*ȷ������*/, double ignore/*����ֵ*/)
//{
//  int outLimitCnt = 0;// ���޵����
//  int ignoreCnt = 0; //���Ե����181116
//  //int cur = 0;
//  // -1 û���ۼƵ��ĳ��޵�
//  int limitCur = -1;// ��һ�����޵�
//  //int limitCnt = 0;
//  double d = 0;
//  int max = 0;// ����޵�
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
//      else if(outLimitCnt >= confirmNum) // ��
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

// ��ȡ���
// ָ��һ���㣬���������һ�������ļ�壬��Ϊ�ϸ�
// �������ٺ�������С
  // ignore ���Զ��ٸ��������ϸ�㣬����Ϊ�ϸ�,��confirmNum��������͸�����
int EfgAlg::ExtractSpike(double * yi, int iNum, double threshold/*��ֵ*/, int confirmNum/*ȷ������*/, double ignore/*����ֵ*/)
{
  int outLimitCnt = 0;// ���޵����
  int ignoreCnt = 0; //���Ե����181116
  //int cur = 0;
  // -1 û���ۼƵ��ĳ��޵�
  int limitCur = -1;// ��һ�����޵�
  //int limitCnt = 0;
  double d = 0;
  double max = 0;// ����޵�
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
      else if(outLimitCnt >= confirmNum) // ��
      {
       // Spike spike = { limitCur, i - 1 };
		  //181215
		int lastCur = i-1-ignoreCnt;

		if(lastCur <0)
			lastCur=iNum+lastCur;

		double * tmp_yi = new double[outLimitCnt];//��ֹ��߽�
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

		if(lastOne == true)//�ڶ��α������������һ����ľ����Ƿ�̫С
		{
			SPIKE tmp =  m_spikes.GetAt(0);

			int d1 = abs(max - tmp.p.x);
			int d2 = iNum - d1;
			int limit= (confirmNum+ignore)*10;
			if(d1< limit|| d2<limit)
				m_spikes.RemoveAt(0);;
			//�Դ������һ���㣬�����˳�ѭ����
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
//x4������x
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
// �������ļ������ȵó����е�λ�ĵ�λ����ֵ
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
// ���ü�����d1d2dm,���������ĵ�һ����С�����D1��Ȼ��D2���м������λ������R1
// �����λ��
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
  //D2��d1ǰ�棬����
  if (DM > 180)
  {
    return FALSE;
  }
  return TRUE;
}
// ���ø߶�/��ȼ���d1d2dm,factor�������ڽ�׼ȷ�ļ�����С��ֵ
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
//  //D2��d1ǰ�棬����
//  if (D2 < D1)
//  {
//    double tmp;
//    tmp = D1;
//    D1 = D2;
//    D2 = tmp;
//    DM = 360 - DM;
//  }
//}

// ��spike�����򣬼������Ҳ�����ڵ�һ,��01 d1��23 d2
// ���ݾ���D1<D2,D1+D2=90�㣬dm<180��
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
      max_i = i;//�ҵ���С���ӵ�һ�Ե���ʼ���
  }
  //max_i=max_i + 1 > 3 ? 0 : max_i + 1;
  for (int i = 0; i <= max_i; i++)//��ǰ��Ĺ�������
  {
    SPIKE spike;
    spike = m_spikes.GetAt(0);
    m_spikes.RemoveAt(0);
    m_spikes.Add(spike);
  }
  return TRUE;
}

// ��spike������01һ�ԣ�23һ��
// h���� w�����ۼƲ���С����һ��
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
      tmp_i = i;//�ҵ���С���ӵ�һ�Ե���ʼ���
  }

  for (int i = 0; i < tmp_i; i++)//��ǰ��Ĺ�������
  {
    SPIKE spike;
    spike = m_spikes.GetAt(i);
    m_spikes.RemoveAt(i);
    m_spikes.Add(spike);
  }
  return 0;
}

//����dleta2�ĺ���,���ػ��ȣ����������delta1,����ԭ����нǣ�DM
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
//����gֵ�����أ�gֵ����λΪ�㣻�������������ǣ��㣩��ԭ����ķ��ߺͱ���ķ��ߵļнǣ��ƾ�Ƭ���ߵ���ת�Ƕ�
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
  if (abs(tempg1 - 47.0)>abs(tempg2 - 47.0))   //���ظ��ӽ�47����ֵ
    return tempg2;
  else
    return tempg1;
}
//����delta��g����ת�Ƕȼ������������ֵ��ƽ��
//���������
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
//����delta1��delta2����theta�Ƕ�
//double

//����ԭʼ����/ԭʼ����/u��g ����λ���ǡ�
void EfgAlg::CalcDegree0(const double D1, const  double D2, const double DM, double &theta0, double &phi0, double &u_g)
{
	if(D1==0||D2==0||DM==0)
	{
		theta0=0;
		phi0=0;
		u_g=0;
		return ;
	}
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  double h1 = -1;
  double k1 = 3;
  double l1 = 1;
  double h2 = -1;
  double k2 = 3;
  double l2 = 3;
  double ca = 1.09973485621048;   //������������,ͨ������1.1
  double cosyzm11, cosyzm12, cosyzm13, cosyzm21, cosyzm22, cosyzm23, cosdl3;
  double temps1, temps2, tdelta2, tg, tb1, tb2;
  double delta3 = 25.1872615573149;  //����ԭ����ķ��ߵļнǣ���λΪ��
  double delta1 = 17.10;   //delta����ʼֵ
  double d1, d2, dm;
  double sintheta, sinphi, theta, phi1, phi2;
  double tvalue, temp, tdelta1, cosdelta1, cosdelta2;
  double wa, wb, wc, tempw, sinphi1, sinphi2;
  CString str;
  //CString strD1, strD2, strDM;
  //GetDlgItem(D1_EDIT1)->GetWindowText(strD1);
  //GetDlgItem(D1_EDIT2)->GetWindowText(strD2);
  //GetDlgItem(D1_EDIT3)->GetWindowText(strDM);
  //ԭ����1��������
  temps1 = sqrt(4.0 / 3.0*(h1*h1 + h1 * k1 + k1 * k1) + l1 * l1 / ca / ca);
  cosyzm11 = h1 / temps1;
  cosyzm12 = (h1 + 2.0*k1) / sqrt(3.0) / temps1;
  cosyzm13 = l1 / ca / temps1;
  //ԭ����2��������
  temps2 = sqrt(4.0 / 3.0*(h1*h2 + h2 * k2 + k2 * k2) + l2 * l2 / ca / ca);
  cosyzm21 = h2 / temps2;
  cosyzm22 = (h2 + 2.0*k2) / sqrt(3.0) / temps2;
  cosyzm23 = l2 / ca / temps2;
  //ԭ����1��ԭ����2�ļнǵ�����
  cosdl3 = cosyzm11 * cosyzm21 + cosyzm12 * cosyzm22 + cosyzm13 * cosyzm23;
  //	temp=cal_dleta2(17.9781922,delta3,138.1541);    //����һ��delta������һ��delta

  //	temp=cal_gvalue(29.977282,17.9781922,283.1136);   //����gֵ
  //����gֵ��deltaֵ���㣬��ֵ
  //	temp=cal_thetab(17.9781922,47.5971,283.1136);   //���������
  //��delta1����ѭ����
  //d1 = _wtof(strD1.GetBuffer(0)) / 2 + 270.0;
  //d2 = _wtof(strD2.GetBuffer(0)) / 2 + 270.0;
  //dm = _wtof(strDM.GetBuffer(0));
  d1 = D1 / 2 + 270.0;
  d2 = D2 / 2 + 270.0;
  dm = DM;
  tvalue = 1e-5;
  tdelta1 = 0.1;
  while (true) {
    delta1 += tdelta1;   //�趨һ��ԭ���淨�ߺ;�Ƭ���淨�ߵļн�ֵ
    tdelta2 = cal_dleta2(delta1, delta3, dm);  //����delta2ֵ
    tg = cal_gvalue(29.977282, delta1, d1);   //����gֵ
    tb1 = cal_thetab(delta1, tg, d1);   //��������ǵ�����ֵ��ƽ��
    tb2 = cal_thetab(tdelta2, tg, d2);   //��������ǵ�����ֵ��ƽ��
    temp = temps2 * temps2*(1 - tb1) - temps1 * temps1*(1 - tb2);
    //if(temp>0) tvalue=temp;
    if (temp<0) {
      tdelta1 = -0.00001;
    }
    if (tdelta1<0 && temp>0) break;
    if (delta1>18.5) break;
  }
  //�����и�Ƕ�theta��phi
  cosdelta1 = cos(delta1*DPI);
  cosdelta2 = cos(tdelta2*DPI);
  sintheta = ((cosyzm13*cosdelta2 - cosyzm23 * cosdelta1)*(cosyzm21*cosyzm12 - cosyzm11 * cosyzm22) / (cosyzm21*cosyzm13 - cosyzm11 * cosyzm23) - (cosyzm11 + cosyzm12)*cosdelta2 + (cosyzm21 + cosyzm22)*cosdelta1) / (cosyzm13*cosyzm21 - cosyzm23 * cosyzm11 + cosyzm13 * cosyzm22 - cosyzm23 * cosyzm12);
  theta = asin(sintheta);   //��λ����
                            //����PHi�Ƕ� wa sinw2+wb sinw+c=0
  tempw = cosdelta1 - cosyzm13 * sintheta;
  wa = (cosyzm11*cosyzm11 + cosyzm12 * cosyzm12)*cos(theta)*cos(theta);
  wb = 2 * tempw*cosyzm11*cos(theta);
  wc = tempw * tempw - cosyzm12 * cosyzm12*cos(theta)*cos(theta);
  sinphi1 = (-wb - sqrt(wb*wb - 4 * wa*wc)) / 2 / wa;
  sinphi2 = (-wb + sqrt(wb*wb - 4 * wa*wc)) / 2 / wa;
  phi1 = asin(sinphi1);
  phi2 = asin(sinphi2);
  //�˴���Ҫ�ж�phi�Ƕȣ�19��06�����ϻ������£�IT��Ҫ������Ա����ȷ��
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

//�����X"L
//��λ ��
//X1st = X'
//X2nd = X"
double CalcX2ndL1(const double theta0, const double phi0)
{
  //ԭ����1��������
  double theta = theta0 * DPI;//�Ƕ�ת��������
  double phi = phi0 * DPI;
  double h = -1;
  double k = 3;
  double l = 3;
  double s = 4.0 / 3 * (h * h + h * k + k * k) + l * l / 1.21;
  //ԭ���淽������
  double cos_an = h / sqrt(s);
  double cos_bn = (h + 2 * k) / sqrt(3 * s);
  double cos_rn = l / (1.1*sqrt(s));
  //�����淽������
  double cos_a1st = -cos(theta)*sin(phi);
  double cos_b1st = cos(theta)*cos(phi);
  double cos_r1st = sin(theta);
  //ԭ����1�������Ľ��� L1
  //L1�ķ�����
  //double p = cos_bn * sin(theta) + cos_rn * cos(theta)*cos(phi);//excel b34
  //double q = cos_rn * (-cos(theta)) + cos_an * sin(theta);
  //double r = cos_an * cos(theta)*cos(phi) + cos_bn * (-cos(theta));

  double p = cos_bn * cos_r1st - cos_rn * cos_b1st;//excel b34
  double q = cos_rn * cos_a1st - cos_an * cos_r1st;
  double r = cos_an * cos_b1st - cos_bn * cos_a1st;

  //X2nd�ķ�����
  double px = cos(phi);
  double qx = sin(phi);
  double rx = 0;
  //L1��X"��ļнǣ���X"L
  double x2nd_l = acos((p * px + q * qx + r * rx) / 
    (sqrt(p * p + q * q + r * r) * sqrt(px * px + qx * qx + rx * rx)));
  return x2nd_l / DPI;
}
/*
amp	��������������Ƕȣ�������ϵķ���ֵ��
      amp = atan (A * factor_a / factor_l) ת���ɡ�
PHASE	��λֵ��������ϵ���λֵ + 180��
      sin �� x�ʣ�0��iNum��ת���� x�ʣ�0��2 * pi��,w����1����phase = sinparam.tת���ɡ�
R1	��������е�R1ֵ��ԭ����;�Ƭ���潻���������λ��ƫ������
LaserOffset	���õļ������ƫ�ƽǶȣ�����������ߺ���λ�ļнǣ�
��	��Ƭ������ԵĦȽǶȣ�û��������
��	ͬ������

��λͳһΪ��
dm �� 180 ����

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

  

  //ת��������
  double am = amp * DPI;
  double PHASE = phase * DPI;
  double R1 = r1 * DPI;
  double laserOffset = laser_offset * DPI;
  double theta = theta0 * DPI;
  double phi = phi0 * DPI;
  double X2ndL = x2nd_l * DPI;
  //double DM = dm * DPI;

  double PHASE1 = phase1 * DPI;//excel h10

  //�������ʱ�ľ�Ƭ����Ͳ�����׼�潻�ߵķ�������
  double nx = cos(PHASE1)*cos(phi) + sin(PHASE1)*sin(theta)*sin(phi);
  double ny = cos(PHASE1)*sin(phi) - sin(PHASE1)*sin(theta)*cos(phi);
  double nz = sin(PHASE1)*cos(theta);
  //�任����
  double c11 = nx * nx*(1 - cos(am)) + cos(am);
  double c12 = nx * ny*(1 - cos(am)) + nz * sin(am);
  double c13 = nx * nz*(1 - cos(am)) - ny * sin(am);
  double c21 = nx * ny*(1 - cos(am)) - nz * sin(am);
  double c22 = ny * ny*(1 - cos(am)) + cos(am);
  double c23 = ny * nz*(1 - cos(am)) + nx * sin(am);
  double c31 = nx * nz*(1 - cos(am)) + ny * sin(am);
  double c32 = ny * nz*(1 - cos(am)) - nx * sin(am);
  double c33 = nz * nz*(1 - cos(am)) + cos(am);
  //������ķ�������
  double cos_a1st = -cos(theta)*sin(phi);
  double cos_b1st = cos(theta)*cos(phi);
  double cos_r1st = sin(theta);
  //�����׼��ķ�������
  double a0 = c11 * cos_a1st + c12 * cos_b1st + c13 * cos_r1st;
  double b0 = c21 * cos_a1st + c22 * cos_b1st + c23 * cos_r1st;
  double r0 = c31 * cos_a1st + c32 * cos_b1st + c33 * cos_r1st;
  /*double a0 = acos(cos_a0);
  double b0 = acos(cos_b0);
  double r0 = acos(cos_r0);*/
  // ����������ĦȺͦ�
  theta1 = asin(r0) / DPI;
  phi1 = acos(b0 / sqrt(1 - r0 * r0)) / DPI;
}

//��λ����һ�£������
void EfgAlg::CalcEquAngle(double theta0, double phi0, double equ_phi, double equ_factor, double &equ)
{
  // ��Ч�� = ԭʼ���� + ��ԭʼ���� - ��Ч�ǵ��������/��Ч������
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
    avg = xi;//ƽ��ֵ
    std2 = xi * xi;//��׼ƫ���м�ֵ
  }
  if (n > 1)
  {
    avg = (avg + xi / (n - 1)) / (n / (n - 1));//��ƽ��ֵ
    std2 = (std2 + xi * xi / (n - 1)) / (n / (n - 1));//���׼���м�ֵ
    std = sqrt(abs(std2 - avg * avg)*n / (n - 1));//���׼��
  }
}