#pragma once
#define PI 3.1415926


#define USER_TO_SEC(deg) ((deg)/10000*3600+(deg)%10000/100*60+(deg)%100)
//��Ķȷ���
#define S_DEG(sec) ((sec)/3600)
#define S_MIN(sec) ((sec)%3600/60)
#define S_SEC(sec) ((sec)%60)
//�û�����Ķȷ���
#define U_DEG(user) ((user)/10000)
#define U_MIN(user) ((user)%10000/100)
#define U_SEC(user) ((user)%100)


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

  int SmoothYi(double * yi, int iNum, double limit, int confirmLimitNum, int flag = -1);

public:
  // �涨һ��sin���ڣ���w��֪�����w���涨���޷������������������
  int FitSinBySubstitution(double * yi/*���ԭʼyֵ*/, int iNum, double *fityi/*�����Ϻ��ֵ*/, struct tagSinParam &sinparam);
  // ������
  int FitSinByLeastSquares(double * yi/*���ԭʼyֵ*/, int iNum, double *fityi/*�����Ϻ��ֵ*/, struct tagSinParam &sinparam);

  //////////////////////////////////////////////X laser////////////////////////////////////////////////////////////

  //typedef struct tagSpike
  //{
  //  double start;
  //  double end;
  //}Spike, *PSpike;

  CArray<POINT, POINT&> m_spikes;
  // ��ȡ���
  int ExtractSpike(double * yi, int iNum, double limit, int confirmLimitNum, double ignore);
  // �õ�������
  int GetSpikesNumber();
  // �õ�ĳ����������
  int GetSpike(int number, POINT& spike);
  BOOL GetAllSortDegree(int *out_sec, int in_center_sec, int step_sec, int sort_num);
};

