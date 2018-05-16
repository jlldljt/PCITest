#pragma once
#define PI 3.1415926
  //yi = A * sin (w*xi + t) + k
  struct tagSinParam {
    double A;
    double w;
    double t;
    double k;
  };
class EfgAlg
{
public:
  EfgAlg();
  ~EfgAlg();

private:

  double Avg(int num, int * th);

  double Avg(int num, double * th);

  double AvgDVal(int num, double * yi, double * fityi);

  double Std(int num, double * yi, double * fityi);

  double SinAwtk(tagSinParam sinparam, double xi);

  void CalFitYi(double * fityi, int iNum, tagSinParam sinparam);

  void SmoothYi(double * yi, int iNum, double limit);

public:
  //�涨һ��sin���ڣ���w��֪�����w���涨���޷������������������
  int FitSinBySubstitution(double * yi/*���ԭʼyֵ*/, int iNum, double *fityi/*�����Ϻ��ֵ*/, struct tagSinParam &sinparam);
  int FitSinBySubstitution(double * yi/*���ԭʼyֵ*/, int iNum, double *fityi/*�����Ϻ��ֵ*/, struct tagSinParam &sinparam);



};

