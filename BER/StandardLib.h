#pragma once
#include "CSVFile.h"

class CStdLib {
public:
  CStdLib() : m_no(0), m_laser(0), m_phi(0){};
  CStdLib(int no, int laser, int phi) : m_no(no), m_laser(laser), m_phi(phi) {};
  ~CStdLib() {};
  int m_no;      //ID
  int m_laser;   //����
  int m_phi;     //����
};

class CStdSeries {
public:
  CStdSeries() : m_no(0), m_phi_p(0), m_phi_n(0), m_laser_p(0), m_laser_n(0) {};
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
  CStdSet() : m_maxd_phi(0), m_maxd_laser(0), m_min_lib_num(0), m_test_cnt(0), m_min_test_num(0) {};
  ~CStdSet() {};
public:
  //int m_no;      //ID
  int m_maxd_phi;   //��������ֵ
  int m_maxd_laser;   //��������ֵ
  int m_min_lib_num; //����СƬ��
  int m_test_cnt; //�Ա��������
  int m_min_test_num; //��С�Ա�Ƭ��
  CString m_password;
};

class CStandardLib
{
public:
  //CStandardLib();
  CStandardLib(LPCTSTR path, int series = 10,int std = 50);
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
  // �Ա���ı�׼Ƭ
  CArray<CStdLib, CStdLib> m_checked;
  // ����
  CStdSet m_set;

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
  void SetStdChecked(CStdLib std);
  // ��������
  void LoadSet(/*int id*/);
  // �������
  BOOL CheckPassword(CString password);
  // ����Ա�����
  void SaveSet(void);
};

