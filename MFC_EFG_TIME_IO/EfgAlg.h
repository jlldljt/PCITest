#pragma once
#define PI 3.1415926


#define USER_TO_SEC(deg) ((deg)/10000*3600+(deg)%10000/100*60+(deg)%100)
#define DEG_TO_USER(deg) (SEC_TO_USER(DEG_TO_SEC(deg)))
#define SEC_TO_USER(sec) ((sec)/3600*10000+(sec)%3600/60*100+(sec)%60)

#define USER_TO_DEG(deg) ((deg)/10000+(deg)%10000/100/60.0+(deg)%100/3600.0)
#define DEG_TO_SEC(deg) ((int)((deg)*3600))
#define SEC_TO_DEG(sec) ((sec)/3600.0)
//秒的度分秒
#define S_DEG(sec) ((sec)/3600)
#define S_MIN(sec) ((sec)%3600/60)
#define S_SEC(sec) ((sec)%60)
//度的度分秒
#define D_DEG(deg) S_DEG((int)((deg)*3600))
#define D_MIN(deg) S_MIN((int)((deg)*3600))
#define D_SEC(deg) S_SEC((int)((deg)*3600))
//用户输入的度分秒
#define U_DEG(user) ((user)/10000)
#define U_MIN(user) ((user)%10000/100)
#define U_SEC(user) ((user)%100)


#define DPI 0.0174532922//pi / 180 角度 转 弧度用


typedef struct tagSPIKE
{
  POINT p;//坐标
  LONG  w;//宽度
} SPIKE, *PSPIKE;



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
  // 三参数，注意是弧度值
  int FitSinByLeastSquares(double * yi/*存放原始y值*/, int iNum, double *fityi/*存放拟合后的值*/, struct tagSinParam &sinparam);

  //////////////////////////////////////////////X laser////////////////////////////////////////////////////////////

  //typedef struct tagSpike
  //{
  //  double start;
  //  double end;
  //}Spike, *PSpike;

  CArray<SPIKE, SPIKE&> m_spikes;
  void Correct(double * yi, int iNum, int sub);
  void Smooth(double * yi, int iNum, int smooth_width, int cnt);
  // 提取尖峰
  int ExtractSpike(double * yi, int iNum, double threshold, int confirmNum, double ignore);
  // 得到尖峰个数
  int GetSpikesNumber();
  // 得到某个尖峰的坐标
  int GetSpike(int number, SPIKE& spike);
  // 得到各个档的档位,秒
  BOOL GetAllSortDegree(int *out_sec, int in_center_sec, int step_sec, int sort_num);
 // void JudegWhichSortDegree(double sec, int *sort_sec, int step_sec, int sort_num, int &which);
  //°
  BOOL GetD1D2DM(double & D1, double & D2, double & DM,double &R1, int pluse_num);
  //BOOL GetD1D2DM(double &D1, double &D2, double &DM, int pluse_num, double factor_h, double factor_w);
  BOOL SortSpike(int pluse_num);
  // 计算原始光轴/原始电轴/u轴g，°
  void CalcDegree0(const double D1, const double D2, const double DM, double & theta0, double & phi0, double &u_g);
  // 计算修正光轴/电轴，°
  void CalcDegree1(double amp, double phase, double r1, double laser_offset, double theta0, double phi0, double &theta1, double &phi1);
  // 计算等效角，°
  void CalcEquAngle(double theta0, double phi0, double equ_phi, double equ_factor, double & equ);
  // 累计计算avg std， std2时中间值
  void SortAvgStd(const int sum, const double angle, double & avg, double & std, double & std2);


private:
  BOOL SortSpike(double factor_h, double factor_w);
};

