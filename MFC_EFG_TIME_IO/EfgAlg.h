#pragma once
#define PI 3.1415926


#define USER_TO_SEC(deg) (((int)(deg))/10000*3600+((int)(deg))%10000/100*60+((int)(deg))%100)
#define DEG_TO_USER(deg) (SEC_TO_USER(DEG_TO_SEC(deg)))
#define SEC_TO_USER(sec) (((int)(sec))/3600*10000+((int)(sec))%3600/60*100+((int)(sec))%60)

#define USER_TO_DEG(deg) (((int)(deg))/10000+((int)(deg))%10000/100/60.0+((int)(deg))%100/3600.0)
#define DEG_TO_SEC(deg) ((int)((deg)*3600+0.5))
#define SEC_TO_DEG(sec) ((sec)/3600.0)
//��Ķȷ���
#define S_DEG(sec) ((sec)/3600)
#define S_MIN(sec) ((sec)%3600/60)
#define S_SEC(sec) ((sec)%60)
//�ȵĶȷ���
#define D_DEG(deg) S_DEG((int)((deg)*3600+0.5))
#define D_MIN(deg) S_MIN((int)((deg)*3600+0.5))
#define D_SEC(deg) S_SEC((int)((deg)*3600+0.5))
//�û�����Ķȷ���
#define U_DEG(user) ((user)/10000)
#define U_MIN(user) ((user)%10000/100)
#define U_SEC(user) ((user)%100)


#define DPI 0.0174532922//pi / 180 �Ƕ� ת ������


typedef struct tagSPIKE
{
  POINT p;//����
  LONG  w;//���
  double x;//��עx
} SPIKE, *PSPIKE;



  //yi = A * sin (w*xi + t) + k
  struct tagSinParam {
    double A;
    double w;
    double t;
    double k;
  };

  //struct tagFitParam {
  //  double A,w,t,k,D1,D2, DM,R1;
  //  double A_avg,w_avg,t_avg,k_avg,D1_avg,D2_avg, DM_avg,R1_avg;
  //  double A_std,w_std,t_std,k_std,D1_std,D2_std, DM_std,R1_std;
  //  double A_std2,w_std2,t_std2,k_std2,D1_std2,D2_std2, DM_std2,R1_std2;
  //};


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
  // ��������ע���ǻ���ֵ
  int FitSinByLeastSquares(double * yi/*���ԭʼyֵ*/, int iNum, double *fityi/*�����Ϻ��ֵ*/, struct tagSinParam &sinparam, double *ignore_val = NULL);

  //////////////////////////////////////////////X laser////////////////////////////////////////////////////////////

  //typedef struct tagSpike
  //{
  //  double start;
  //  double end;
  //}Spike, *PSpike;

  CArray<SPIKE, SPIKE&> m_spikes;
  void Correct(double * yi, int iNum, int sub);
  void KLM(double * yi, int iNum);
  void Smooth(double * yi, int iNum, int smooth_width, int cnt);
  // ��ȡ���
  int ExtractSpike(double * yi, int iNum, double threshold, int confirmNum, double ignore);
  // �õ�������
  int GetSpikesNumber();
  // �õ�ĳ����������
  int GetSpike(int number, SPIKE& spike);
  int DelSpike(int number);
  // �������е��x�����ڶ�β���

  int UpdateSpikeX(double* x);
  // �õ��������ĵ�λ,��
  BOOL GetAllSortDegree(int *out_sec, int in_center_sec, int step_sec, int sort_num);
 // void JudegWhichSortDegree(double sec, int *sort_sec, int step_sec, int sort_num, int &which);
  //��
  BOOL GetD1D2DM(double & D1, double & D2, double & DM,double &R1, int pluse_num);
  
  //BOOL UpdateFitParam(int num,struct tagFitParam fit);
  //BOOL GetD1D2DM(double &D1, double &D2, double &DM, int pluse_num, double factor_h, double factor_w);
  BOOL SortSpike(int pluse_num);
  // ����ԭʼ����/ԭʼ����/u��g����
  void CalcDegree0(const double D1, const double D2, const double DM, double & theta0, double & phi0, double &u_g);
  // ������������/���ᣬ��
  void CalcDegree1(double amp, double phase, double r1, double laser_offset, double theta0, double phi0, double &theta1, double &phi1, double dm, double *phase_end = NULL);
  // �����Ч�ǣ���
  void CalcEquAngle(double theta, double phi, double equ_phi, double equ_factor, double & equ);
  void CalcEquSec(int theta0, int phi0, int equ_phi, int equ_factor, int &equ);
  // �ۼƼ���avg std�� std2ʱ�м�ֵ
  void SortAvgStd(const int sum, const double angle, double & avg, double & std, double & std2);


private:
  BOOL SortSpike(double factor_h, double factor_w);
};

