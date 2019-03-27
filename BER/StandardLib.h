#pragma once
// #include "CSVFile.h"

class CStdLib {
public:
  CStdLib() : m_no(-1), m_laser(0), m_phi(0){};
  CStdLib(int no, int laser, int phi) : m_no(no), m_laser(laser), m_phi(phi) {};
  ~CStdLib() {};
  int m_no;      //ID
  int m_laser;   //����
  int m_phi;     //����
};

class CStdSeries {
public:
  CStdSeries() : m_no(-1), m_phi_p(0), m_phi_n(0), m_laser_p(0), m_laser_n(0) {};
  CStdSeries(int no, int phi_p, int phi_n, int laser_p, int laser_n)
    : m_no(no), m_phi_p(phi_p), m_phi_n(phi_n), m_laser_p(laser_p), m_laser_n(laser_n) {};
  ~CStdSeries() {};
public:
  int m_no;      //ϵ��ID
  int m_phi_p;   //���
  int m_phi_n;   //���
  int m_laser_p; //���
  int m_laser_n; //���
};

class CStdSet {
public:
  CStdSet() : m_maxd_phi(0), m_maxd_laser(0), m_min_lib_num(0), m_test_cnt(0), m_min_test_num(0), m_password(""),m_type(""){};
  ~CStdSet() {};
public:
  //int m_no;      //ID
  int m_maxd_phi;   //��������ֵ
  int m_maxd_laser;   //��������ֵ
  int m_min_lib_num; //����СƬ�����ĳ������ж����ͨ��Ƭ��
  int m_test_cnt; //�Ա��������
  int m_min_test_num; //��С�Ա�Ƭ��
  CString m_password;
  CString m_type;// �Ա�����
};

typedef struct tagResult{
  double m_avg;
  double m_std;
  double m_std2;
}Result;

class CStdResult {
public:
  CStdResult() 
  { 
    Clear();
  };
  ~CStdResult() {};
public:
  int m_sum;
  Result m_checking_laser, m_checking_phi;
  Result m_checked_laser, m_checked_phi;
  double m_dLaser, m_dPhi;

  void Clear(void)
  {
    m_sum = 0;
    memset(&m_checking_laser, 0, sizeof(Result));
    memset(&m_checking_phi, 0, sizeof(Result));
    memset(&m_checked_laser, 0, sizeof(Result));
    memset(&m_checked_phi, 0, sizeof(Result));
    m_dLaser = 0;
    m_dPhi = 0;
  }
  void AddCheck(Result &check_result, double angle);

  void Add(CStdLib checking, CStdLib checked)
  {
    m_sum++;
    AddCheck(m_checking_laser, checking.m_laser);
    AddCheck(m_checking_phi, checking.m_phi);
    AddCheck(m_checked_laser, checked.m_laser);
    AddCheck(m_checked_phi, checked.m_phi);
    m_dLaser = fabs(m_checking_laser.m_avg - m_checked_laser.m_avg);
    m_dPhi = fabs(m_checking_phi.m_avg - m_checked_phi.m_avg);
  }

};

class CStandardLib
{
public:
  //CStandardLib();
  CStandardLib(LPCTSTR path, int series = 20,int std = 10);
  ~CStandardLib();
public:
  CString m_sqlPath;
  //CCSVFile *m_csvStd;
  // ��׼Ƭϵ��
  CArray<CStdSeries, CStdSeries> m_series;
  // ��ǰ���ص�ϵ��No
  int m_seriesNo;
  // �Ѽ���ϵ�еı�׼Ƭ
  CArray<CStdLib, CStdLib> m_lib;
  // �Ա��ʵ��ֵ
  CArray<CStdLib, CStdLib> m_checked;
  int pass_num;
  // �Ա�ı�׼ֵ
  CArray<CStdLib, CStdLib> m_checking;
  // ����
  CStdSet m_set;
  // �Ա���
  CStdResult m_result;

  // ��ʼ�����ݿ�
  void Init(int series_num, int std_num);

  void Save(void);
  // ��������ϵ�е�array
  void LoadSeries(void);
  // ����seriesNoϵ�еı�׼Ƭ��array
  void LoadLib(int seriesNo);
  // ����ϵ��
  void SetSeries(CStdSeries std);
  // ���õ�ǰϵ�б�׼Ƭ
  void SetStd(CStdLib std);
  // �����Ѳ����ı�׼Ƭ
  void SetStdChecking(CStdLib std);
  CStdLib GetStdChecking(int no);
  void SetStdChecked(CStdLib std);
  CStdLib GetStdChecked(int no);
  // ��������
  void LoadSet(/*int id*/);
  // �������
  BOOL CheckPassword(CString password);
  // ����Ա�����
  void SaveSet(void);
  // ����Ա�
  void ClearCheck();
  UCHAR AddCheck(CStdLib checking, CStdLib checked);
  // ����Ƿ��Ѿ�������
  BOOL IsChecked(int no);
};

