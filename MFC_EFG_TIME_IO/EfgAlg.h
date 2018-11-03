#pragma once
#define PI 3.1415926


#define USER_TO_SEC(deg) ((deg)/10000*3600+(deg)%10000/100*60+(deg)%100)
//秒的度分秒
#define S_DEG(sec) ((sec)/3600)
#define S_MIN(sec) ((sec)%3600/60)
#define S_SEC(sec) ((sec)%60)
//用户输入的度分秒
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
  // 规定一个sin周期，即w已知，如果w不规定，无法代出结果，不收敛了
  int FitSinBySubstitution(double * yi/*存放原始y值*/, int iNum, double *fityi/*存放拟合后的值*/, struct tagSinParam &sinparam);
  // 三参数
  int FitSinByLeastSquares(double * yi/*存放原始y值*/, int iNum, double *fityi/*存放拟合后的值*/, struct tagSinParam &sinparam);

  //////////////////////////////////////////////X laser////////////////////////////////////////////////////////////

  //typedef struct tagSpike
  //{
  //  double start;
  //  double end;
  //}Spike, *PSpike;

  CArray<POINT, POINT&> m_spikes;
  // 提取尖峰
  int ExtractSpike(double * yi, int iNum, double limit, int confirmLimitNum, double ignore);
  // 得到尖峰个数
  int GetSpikesNumber();
  // 得到某个尖峰的坐标
  int GetSpike(int number, POINT& spike);
  BOOL GetAllSortDegree(int *out_sec, int in_center_sec, int step_sec, int sort_num);
};

