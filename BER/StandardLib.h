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
};

