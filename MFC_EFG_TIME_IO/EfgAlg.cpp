#include "StdAfx.h"
#include "EfgAlg.h"
#include <math.h>

EfgAlg::EfgAlg()
{
}


EfgAlg::~EfgAlg()
{
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
  int max = 0;// 最大超限点
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
      else if(outLimitCnt >= confirmNum) // 存
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
  //D2在d1前面，调整
  if (D2 < D1 || DM > 180)
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
    int d = p1.p.x - p2.p.x;
    dval[i] = d < 0 ? d + pluse_num : d;
    if (dval[i] < dval[max_i])//SC < ;AT >
      max_i = i;//找到最小因子的一对的起始序号
  }

  for (int i = 0; i < max_i; i++)//把前面的滚到后面
  {
    SPIKE spike;
    spike = m_spikes.GetAt(i);
    m_spikes.RemoveAt(i);
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
  double s = 4 / 3 * (h * h + h * k + k * k) + l * l / 1.21;
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
  double p = cos_bn * sin(theta) + cos_rn * cos(theta)*cos(phi);
  double q = cos_rn * (-cos(theta)) + cos_an * sin(theta);
  double r = cos_an * cos(theta)*cos(phi) + cos_bn * (-cos(theta));
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
*/
void EfgAlg::CalcDegree1(double amp, double phase,double r1, double laser_offset,double theta0, double phi0, double &theta1, double &phi1)
{
  double x2nd_l = CalcX2ndL1(theta0, phi0);
  //转换到弧度
  double am = amp * DPI;
  double PHASE = phase * DPI;
  double R1 = r1 * DPI;
  double laserOffset = laser_offset * DPI;
  double theta = theta0 * DPI;
  double phi = phi0 * DPI;
  double X2ndL = x2nd_l * DPI;

  double phase1 = R1 + X2ndL - PHASE + 270 - laserOffset;
  //计算测量时的晶片表面和测量基准面交线的方向余弦
  double nx = cos(phase1)*cos(phi) + sin(phase1)*sin(theta)*sin(phi);
  double ny = cos(phase1)*sin(phi) - sin(phase1)*sin(theta)*cos(phi);
  double nz = sin(phase1)*cos(theta);
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
  double cos_a0 = c11 * cos_a1st + c12 * cos_b1st + c13 * cos_r1st;
  double cos_b0 = c21 * cos_a1st + c22 * cos_b1st + c23 * cos_r1st;
  double cos_r0 = c31 * cos_a1st + c32 * cos_b1st + c33 * cos_r1st;
  double a0 = acos(cos_a0);
  double b0 = acos(cos_b0);
  double r0 = acos(cos_r0);
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