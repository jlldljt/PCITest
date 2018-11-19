#include "StdAfx.h"
#include "EfgAlg.h"
#include <math.h>

EfgAlg::EfgAlg()
{
}


EfgAlg::~EfgAlg()
{
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
int EfgAlg::FitSinByLeastSquares(double * yi, int iNum, double * fityi, tagSinParam & sinparam)
{
  double A = 0;
  const double w = 2 * PI / iNum;
  double t = 0;
  double k = 0;
  struct tagSinParam param = { 0 };
  double y, a, b;
  double avgy = 0, avga = 0, avgb = 0;
  double sumya = 0, sumyb = 0, sumb = 0, suma = 0, sumab = 0, sumaa = 0, sumbb = 0;
  for (int i = 0; i < iNum; i++)
  {
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
  avgy /= iNum; 
  avga /= iNum;
  avgb /= iNum;

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
  int max = 0;// ����޵�
  confirmNum += ignore;
  m_spikes.RemoveAll();

  for (int i = 0; i < iNum; i++)
  {
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

    if (d > 0 && i < iNum-1)
    {
      if (-1 == limitCur)
      {
        limitCur = i;
        outLimitCnt = 1;
        max = i;
      }
      else
      {
        if (yi[max] < yi[i])
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
        SPIKE spike;
        spike.p.x = max;//(limitCur + i - 1) / 2;
        spike.p.y = yi[max];
        spike.w = outLimitCnt;
        m_spikes.Add(spike);
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
  double d_DM = p[2].p.x - p[1].p.x;
  D1 = d_D1 < 0 ? d_D1 + pluse_num : d_D1;
  D2 = d_D2 < 0 ? d_D2 + pluse_num : d_D2;
  DM = d_DM < 0 ? d_DM + pluse_num : d_DM;
  DM += (D1 + D2) / 2;
  double d_R1 = p[2].p.x + D2 / 2;
  R1 = d_R1 >= pluse_num ? d_R1 - pluse_num : d_R1;
  D1 = D1 * 360 / pluse_num;
  D2 = D2 * 360 / pluse_num;
  DM = DM * 360 / pluse_num;
  R1 = R1 * 360 / pluse_num;
  //D2��d1ǰ�棬����
  if (D2 < D1 || DM > 180)
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
    int d = p1.p.x - p2.p.x;
    dval[i] = d < 0 ? d + pluse_num : d;
    if (dval[i] < dval[max_i])//SC < ;AT >
      max_i = i;//�ҵ���С���ӵ�һ�Ե���ʼ���
  }

  for (int i = 0; i < max_i; i++)//��ǰ��Ĺ�������
  {
    SPIKE spike;
    spike = m_spikes.GetAt(i);
    m_spikes.RemoveAt(i);
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
  double s = 4 / 3 * (h * h + h * k + k * k) + l * l / 1.21;
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
  double p = cos_bn * sin(theta) + cos_rn * cos(theta)*cos(phi);
  double q = cos_rn * (-cos(theta)) + cos_an * sin(theta);
  double r = cos_an * cos(theta)*cos(phi) + cos_bn * (-cos(theta));
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
*/
void EfgAlg::CalcDegree1(double amp, double phase,double r1, double laser_offset,double theta0, double phi0, double &theta1, double &phi1)
{
  double x2nd_l = CalcX2ndL1(theta0, phi0);
  //ת��������
  double am = amp * DPI;
  double PHASE = phase * DPI;
  double R1 = r1 * DPI;
  double laserOffset = laser_offset * DPI;
  double theta = theta0 * DPI;
  double phi = phi0 * DPI;
  double X2ndL = x2nd_l * DPI;

  double phase1 = R1 + X2ndL - PHASE + 270 - laserOffset;
  //�������ʱ�ľ�Ƭ����Ͳ�����׼�潻�ߵķ�������
  double nx = cos(phase1)*cos(phi) + sin(phase1)*sin(theta)*sin(phi);
  double ny = cos(phase1)*sin(phi) - sin(phase1)*sin(theta)*cos(phi);
  double nz = sin(phase1)*cos(theta);
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
  double cos_a0 = c11 * cos_a1st + c12 * cos_b1st + c13 * cos_r1st;
  double cos_b0 = c21 * cos_a1st + c22 * cos_b1st + c23 * cos_r1st;
  double cos_r0 = c31 * cos_a1st + c32 * cos_b1st + c33 * cos_r1st;
  double a0 = acos(cos_a0);
  double b0 = acos(cos_b0);
  double r0 = acos(cos_r0);
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