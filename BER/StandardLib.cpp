#include "StdAfx.h"
#include "StandardLib.h"

//CStandardLib::CStandardLib()
//{
//}

CStandardLib::CStandardLib(LPCTSTR path, int series_num, int std_num)
{
  m_sqlPath = path;

  Init(series_num, std_num);

  LoadSeries();
}


CStandardLib::~CStandardLib()
{
}

void CStandardLib::Init(int series_num, int std_num)
{
  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    exit(0);
  }
  //������׼��ϵ�б�
  std::string sql = "CREATE TABLE SERIES("  \
    "ID INT PRIMARY KEY     NOT NULL," \
    "���           INT," \
    "���           INT," \
    "���       INT," \
    "���        INT );";

  bool isOk = sqlite.Writedb(std::string(sql));
  //��Ӹ�ϵ��
  if (true == isOk)
  {
    int cnt = 0;

    for (cnt = 0; cnt < series_num; cnt++)
    {
      char s1[10];
      char s2[10];
      char s3[10];
      char s4[10];
      char s5[10];
      itoa(cnt, s1, 10);
      itoa(0, s2, 10);
      itoa(0, s3, 10);
      itoa(0, s4, 10);
      itoa(0, s5, 10);
      sql = "INSERT INTO SERIES (ID,���,���,���,���) "  \
        "VALUES ("\
        + std::string(s1) + ","\
        + std::string(s2) + "," \
        + std::string(s3) + "," \
        + std::string(s4) + "," \
        + std::string(s5) + " );";

      isOk = sqlite.Writedb(std::string(sql));

      if (false == isOk)
        continue;
      /////////////////////////////////////////
      //������׼���
      sql = "CREATE TABLE STDS"\
        + std::string(s1) + "("  \
        "ID INT PRIMARY KEY     NOT NULL," \
        "��           INT," \
        "��           INT );";

      isOk = sqlite.Writedb(std::string(sql));

      if (false == isOk)
        continue;
      //���10��ϵ��
      if (true == isOk)
      {
        int cnt = 0;

        for (cnt = 0; cnt < std_num; cnt++)
        {
          char s6[10];
          char s7[10];
          char s8[10];
          itoa(cnt, s6, 10);
          itoa(0, s7, 10);
          itoa(0, s8, 10);
          sql =
            "INSERT INTO STDS"\
            + std::string(s1) + "(ID,��,��) "  \
            "VALUES ("\
            + std::string(s6) + ","\
            + std::string(s7) + "," \
            + std::string(s8) + " );";

          isOk = sqlite.Writedb(std::string(sql));
        }
      }
    }
  }


  sqlite.Close();
}

void CStandardLib::Save(void)
{
  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }

  sqlite.Begin();
  // ����series��
  int count = m_series.GetCount();

  int i = 0;

  for (i = 0; i < count; i++)
  {
    char s1[10];
    char s2[10];
    char s3[10];
    char s4[10];
    char s5[10];
    itoa(m_series[i].m_no, s1, 10);
    itoa(m_series[i].m_phi_n, s2, 10);
    itoa(m_series[i].m_phi_p, s3, 10);
    itoa(m_series[i].m_laser_n, s4, 10);
    itoa(m_series[i].m_laser_p, s5, 10);
    
    std::string sql = "UPDATE SERIES SET ��� = " + std::string(s2) + ","\
      + "��� = " + std::string(s3) + ","\
      + "��� = " + std::string(s4) + ","\
      + "��� = " + std::string(s5) + " "\
      + "WHERE ID = " + std::string(s1) + ";";

    bool isOk = sqlite.Writedb(std::string(sql));

    if (false == isOk)
    {
      sql = "INSERT INTO SERIES (ID,���,���,���,���) "  \
        "VALUES ("\
        + std::string(s1) + ","\
        + std::string(s2) + "," \
        + std::string(s3) + "," \
        + std::string(s4) + "," \
        + std::string(s5) + " );";

      bool isOk = sqlite.Writedb(std::string(sql));
    }

  }
  // ����m_seriesNo��Ƭ��
  if (m_seriesNo >= 0)
  {
    count = m_lib.GetCount();

    i = 0;

    for (i = 0; i < count; i++)
    {
      char s0[10];
      char s1[10];
      char s2[10];
      char s3[10];
      itoa(m_seriesNo, s0, 10);
      itoa(m_lib[i].m_no, s1, 10);
      itoa(m_lib[i].m_phi, s2, 10);
      itoa(m_lib[i].m_laser, s3, 10);

      std::string sql = "UPDATE STDS" + std::string(s0) + " SET "\
        + "�� = " + std::string(s2) + ","\
        + "�� = " + std::string(s3) + " "\
        + "WHERE ID = " + std::string(s1) + ";";

      bool isOk = sqlite.Writedb(std::string(sql));

      if (false == isOk)
      {
        sql =
          "INSERT INTO STDS"\
          + std::string(s0) + " (ID,��,��) "  \
          "VALUES ("\
          + std::string(s1) + ","\
          + std::string(s2) + "," \
          + std::string(s3) + " );";

        isOk = sqlite.Writedb(std::string(sql));
      }
    }
  }
  sqlite.Commit();

  sqlite.Close();
}

//����series
void CStandardLib::LoadSeries(void)
{
  m_seriesNo = -1;

  m_series.RemoveAll();

  m_checked.RemoveAll();

  m_lib.RemoveAll();

  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }
  //��ѯ
  std::string sql = "SELECT ID,���,���,���,��� FROM SERIES ORDER BY ID;";

  bool isOk = sqlite.Writedb(std::string(sql));
  //��Ӹ�ϵ��
  if (true == isOk)
  {
    SQLiteStatement* stmt = sqlite.Readdb(sql);
    int count;

    if (stmt)
    {
      while (stmt->NextRow())
      {
        CStdSeries series;
        series.m_no = stmt->ValueInt(0);
        series.m_phi_n = stmt->ValueInt(1);
        series.m_phi_p = stmt->ValueInt(2);
        series.m_laser_n = stmt->ValueInt(3);
        series.m_laser_p = stmt->ValueInt(4);
        m_series.Add(series);
      }
    }
    delete stmt;
  }
  sqlite.Close();
}

void CStandardLib::LoadLib(int seriesNo)
{
  m_seriesNo = seriesNo;

  m_checked.RemoveAll();

  m_lib.RemoveAll();

  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }

  char s[10];

  itoa(seriesNo, s, 10);

  std::string sql = "SELECT ID,��,�� FROM STDS" + std::string(s) + " ORDER BY ID;";

  bool isOk = sqlite.Writedb(std::string(sql));
  //��Ӹ�ϵ��
  if (true == isOk)
  {
    SQLiteStatement* stmt = sqlite.Readdb(sql);
    int count;

    if (stmt)
    {
      while (stmt->NextRow())
      {
        CStdLib lib;
        lib.m_no = stmt->ValueInt(0);
        lib.m_phi = stmt->ValueInt(1);
        lib.m_laser = stmt->ValueInt(2);
        m_lib.Add(lib);
      }
    }
    delete stmt;
  }

  sqlite.Close();
}

void CStandardLib::SetSeries(CStdSeries series)
{
  int count = m_series.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_series[i].m_no == series.m_no)
    {
      m_series[i] = series;
      break;
    }
  }

  if (i >= count)
  {
    m_series.Add(series);
  }
}

void CStandardLib::SetStd(CStdLib std)
{
  int count = m_lib.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_lib[i].m_no == std.m_no)
    {
      m_lib[i] = std;
      break;
    }
  }

  if (i >= count)
  {
    m_lib.Add(std);
  }
}
