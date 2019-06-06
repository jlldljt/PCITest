#include "StdAfx.h"
#include "StandardLib.h"
#include "ZPJ.h"

//CStandardLib::CStandardLib()
//{
//}

CStandardLib::CStandardLib(LPCTSTR path, int series_num, int std_num)
{
  m_sqlPath = path;

  Init(series_num, std_num);

  LoadSeries();

  LoadSet();
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
      itoa(cnt+1, s1, 10);
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
          itoa(cnt+1, s6, 10);
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

  //������׼��ϵ�б�
  sql = "CREATE TABLE STDSET("  \
    "ID INT PRIMARY KEY     NOT NULL," \
    "�Ա�ѡ��               TEXT," \
    "��������ֵ           INT," \
    "��������ֵ           INT," \
    "����СƬ��             INT," \
    "�Ա��������           INT," \
    "��С�Ա�Ƭ��           INT," \
    "����                   TEXT );";

  isOk = sqlite.Writedb(std::string(sql));
  //��Ӹ�ϵ��
  if (true == isOk)
  {
    sql = "INSERT INTO STDSET (ID,�Ա�ѡ��,��������ֵ,��������ֵ,����СƬ��,�Ա��������,��С�Ա�Ƭ��,����) "  \
      "VALUES (1,'����',30,10,5,2,5 ,'111111');";

    isOk = sqlite.Writedb(std::string(sql));
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

  ClearCheck();

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

  ClearCheck();

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

void CStandardLib::SetStdChecking(CStdLib std)
{
  int count = m_checking.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_checking[i].m_no == std.m_no)
    {
      m_checking[i] = std;
      break;
    }
  }

  if (i >= count)
  {
    m_checking.Add(std);
  }
}
CStdLib CStandardLib::GetStdChecking(int no)
{
  int count = m_checking.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_checking[i].m_no == no)
    {
      return m_checking[i];
    }
  }
}
void CStandardLib::SetStdChecked(CStdLib std)
{
  int count = m_checked.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_checked[i].m_no == std.m_no)
    {
      m_checked[i] = std;
      break;
    }
  }

  if (i >= count)
  {
    m_checked.Add(std);
  }
}

CStdLib CStandardLib::GetStdChecked(int no)
{
  int count = m_checked.GetCount();

  int i;

  for (i = 0; i < count; i++)
  {
    if (m_checked[i].m_no == no)
    {
      return m_checked[i];
    }
  }
}

void CStandardLib::LoadSet()
{
  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }

  std::string sql = "SELECT ��������ֵ,��������ֵ,����СƬ��,�Ա��������,��С�Ա�Ƭ��,����,�Ա�ѡ�� FROM STDSET WHERE ID = 1;";

  bool isOk = sqlite.Writedb(std::string(sql));
  //��Ӹ�ϵ��
  if (true == isOk)
  {
    SQLiteStatement* stmt = sqlite.Readdb(sql);

    if (stmt)
    {
      while (stmt->NextRow())
      {
        m_set.m_maxd_phi = stmt->ValueInt(0);
        m_set.m_maxd_laser = stmt->ValueInt(1);
        m_set.m_min_lib_num = stmt->ValueInt(2);
        m_set.m_test_cnt = stmt->ValueInt(3);
        m_set.m_min_test_num = stmt->ValueInt(4);
        m_set.m_password = stmt->ValueString(5);
        m_set.m_type = stmt->ValueString(6);

      }
    }
    delete stmt;
  }

  sqlite.Close();
}

BOOL CStandardLib::CheckPassword(CString password)
{
  BOOL ret = FALSE;

  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return ret;
  }

  std::string sql = "select ID from STDSET where ���� = '" + std::string(CT2A(password)) + "';";

  bool isOk = sqlite.Writedb(std::string(sql));
  //��Ӹ�ϵ��
  if (true == isOk)
  {
    SQLiteStatement* stmt = sqlite.Readdb(sql);

    if (stmt)
    {
      while (stmt->NextRow())
      {
        if (1 == stmt->ValueInt(0))
        {
          ret = TRUE;
        }
      }
    }
    delete stmt;
  }

  sqlite.Close();

  return ret;
}

void CStandardLib::SaveSet(void)
{
  SQLiteWrapper sqlite;

  if (!sqlite.Open(CW2A(m_sqlPath)))
  {
    return;
  }

  sqlite.Begin();

  char s1[10];
  char s2[10];
  char s3[10];
  char s4[10];
  char s5[10];

  itoa(m_set.m_maxd_phi, s1, 10);
  itoa(m_set.m_maxd_laser, s2, 10);
  itoa(m_set.m_min_lib_num, s3, 10);
  itoa(m_set.m_test_cnt, s4, 10);
  itoa(m_set.m_min_test_num, s5, 10);

  std::string sql = "UPDATE STDSET SET ��������ֵ = " + std::string(s1) + ","\
    + "��������ֵ = " + std::string(s2) + ", "\
    + "����СƬ�� = " + std::string(s3) + ", "\
    + "�Ա�������� = " + std::string(s4) + ", "\
    + "��С�Ա�Ƭ�� = " + std::string(s5) + ", "\
    + "�Ա�ѡ�� = '" + std::string(CT2A(m_set.m_type)) + "', "\
    + "���� = '" + std::string(CT2A(m_set.m_password)) + "' "\
    + "WHERE ID = 1;";

  bool isOk = sqlite.Writedb(std::string(sql));

  if (false == isOk)
  {
    AfxMessageBox(_T("����ʧ��"));
  }
  else
  {
    AfxMessageBox(_T("����ɹ�"));
  }

  sqlite.Commit();

  sqlite.Close();
}

void CStandardLib::ClearCheck()
{
  m_checked.RemoveAll();
  m_checking.RemoveAll();
  ASSERT(m_checked.GetCount()== m_checking.GetCount());
  pass_num = 0;
  m_result.Clear();
}



//����ֵ˵��
// 0x00 ��ʾͨ��
// 0x01 ��ʾlaserδͨ��
// 0x02 ��ʾphiδͨ��
// 0x03 ��ʾ��δͨ��
UCHAR CStandardLib::AddCheck(CStdLib checking, CStdLib checked)
{
  ASSERT(checking.m_no == checked.m_no);
  UCHAR cnt = 0;

  SetStdChecking(checking);
  SetStdChecked(checked);

  ASSERT(m_checking.GetCount() == m_checked.GetCount());

  if (abs(checking.m_laser - checked.m_laser) > m_set.m_maxd_laser)
    cnt |= 0x01;

  if (abs(checking.m_phi - checked.m_phi) > m_set.m_maxd_phi)
    cnt |= 0x02;

  if(m_set.m_type == _T("����"))
  {
    if(!(cnt & 0x01))
      pass_num++; 
  }
  else if(m_set.m_type == _T("����"))
  {
    if (!(cnt & 0x02))
      pass_num++;
  }
      


  m_result.Add(checking,checked);
  return cnt;
}

BOOL CStandardLib::IsChecked(int no)
{
  int count_checking = m_checking.GetCount();
  int count_checked = m_checked.GetCount();

  ASSERT(count_checking == count_checked);

  int count = count_checked;

  int i;

  for (i = 0; i < count; i++)
  {
    ASSERT(m_checking[i].m_no == m_checked[i].m_no);
    if (m_checked[i].m_no == no)
    {
      return TRUE;
    }
  }

  return FALSE;
}

void CStdResult::AddCheck(Result &check_result, double angle)
{
    CalcAvgStd(m_sum, angle, check_result.m_avg, check_result.m_std, check_result.m_std2);
}
