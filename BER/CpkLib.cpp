#include "StdAfx.h"
#include "CpkLib.h"
#include "BER.h"

CCpkLib::CCpkLib(LPCTSTR exePath)
{
	m_exePath = exePath;

	CFileFind find;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  

	if (!find.FindFile(m_exePath + _T("\\CPK\\*.*")))
	{
		if (!CreateDirectory(m_exePath + _T("\\CPK\\"), NULL))
		{
			AfxMessageBox(_T("CPK目录创建失败！"));
			exit(0);
		}
	}

	m_iniPath = m_exePath + _T("\\CPK\\CpkSet.ini");

	m_cpkPath = m_exePath + _T("\\CPK\\");

	m_csv_plannedNo = NULL;

	m_csv_processCardNo = NULL;

	Init();
	find.Close();
}


CCpkLib::~CCpkLib()
{
	//if (m_csv_plannedNo)
	//{
	//	delete m_csv_plannedNo;
	//	m_csv_plannedNo = NULL;
	//}

	//if (m_csv_processCardNo)
	//{
	//	delete m_csv_processCardNo;
	//	m_csv_processCardNo = NULL;
	//}
	CloseCsv();
}

void CCpkLib::LoadPlannedNo(void)
{
	CFileFind finder;
	CString strPlan, strdir;
	strdir = m_cpkPath + _T("*.*");
	BOOL b_finded = (BOOL)finder.FindFile(strdir);
	while (b_finded)
	{
		b_finded = (BOOL)finder.FindNextFile();
		if (finder.IsDots())  continue;
		strPlan = finder.GetFileName();
		if (finder.IsDirectory())
		{
			m_arrPlannedNo.Add(strPlan);
		}
		else
		{
			continue;
		}
	}
	finder.Close();
}

BOOL CCpkLib::LoadProcessCardNo(CString plannedNo)
{
	CloseCsv();

	CString path;

	path = m_cpkPath +plannedNo+ _T("\\")+plannedNo+ _T(".csv");

	CCSVFile *csv_plannedNo = new CCSVFile(path, CCSVFile::modeRead);

  CProcessCard processCard;

	if (csv_plannedNo)
	{
		CStringArray arr;
		m_arrProcessCardNo.RemoveAll();
		csv_plannedNo->SeekToBegin();
		while(csv_plannedNo->ReadData(arr))
		{
      processCard.Reset();
			//if(arr.GetCount()==PLANNED_CSV_ROW_MAX){
			//	processCard.no = arr[PLANNED_CSV_ROW_NO];
   //   
			//	if(processCard.no == _T("流程卡"))
			//		continue;

			//	processCard.axis = arr[PLANNED_CSV_ROW_AXIS];
			//	processCard.center = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_CENTER]));
			//	processCard.usl = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_USL]));
			//	processCard.lsl = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_LSL]));
			//	processCard.avg = _ttof(arr[PLANNED_CSV_ROW_AVG]);
			//	processCard.std = _ttof(arr[PLANNED_CSV_ROW_STD]);
			//	processCard.cp = _ttof(arr[PLANNED_CSV_ROW_CP]);
			//	processCard.ca = _ttof(arr[PLANNED_CSV_ROW_CA]);
			//	processCard.cpk = _ttof(arr[PLANNED_CSV_ROW_CPK]);
			//	m_arrProcessCardNo.Add(processCard);
			//}
      if (arr.GetCount() > PLANNED_CSV_ROW_NO) 
        processCard.no = arr[PLANNED_CSV_ROW_NO];

        if (processCard.no == _T("流程卡") || processCard.no == _T(""))
          continue;

      if (arr.GetCount() > PLANNED_CSV_ROW_AXIS)
        processCard.axis = arr[PLANNED_CSV_ROW_AXIS];
      if (arr.GetCount() > PLANNED_CSV_ROW_CENTER)
        processCard.center = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_CENTER]));
      if (arr.GetCount() > PLANNED_CSV_ROW_USL)
        processCard.usl = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_USL]));
      if (arr.GetCount() > PLANNED_CSV_ROW_LSL)
        processCard.lsl = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_LSL]));
      if (arr.GetCount() > PLANNED_CSV_ROW_AVG)
        processCard.avg = _ttof(arr[PLANNED_CSV_ROW_AVG]);
      if (arr.GetCount() > PLANNED_CSV_ROW_STD)
        processCard.std = _ttof(arr[PLANNED_CSV_ROW_STD]);
      if (arr.GetCount() > PLANNED_CSV_ROW_CP)
        processCard.cp = _ttof(arr[PLANNED_CSV_ROW_CP]);
      if (arr.GetCount() > PLANNED_CSV_ROW_CA)
        processCard.ca = _ttof(arr[PLANNED_CSV_ROW_CA]);
      if (arr.GetCount() > PLANNED_CSV_ROW_CPK)
        processCard.cpk = _ttof(arr[PLANNED_CSV_ROW_CPK]);
        
        m_arrProcessCardNo.Add(processCard);
      
		}



		// 
		// 

		/*m_gridProcessCardNo.InsertRow(m_pCpk->m_arrProcessCardNo[i].no, -1);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].center);
		m_gridProcessCardNo.SetItemText(line, 1, str);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].usl);
		m_gridProcessCardNo.SetItemText(line, 2, str);
		str.Format(_T("%d"), m_pCpk->m_arrProcessCardNo[i].lsl);
		m_gridProcessCardNo.SetItemText(line, 3, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].avg);
		m_gridProcessCardNo.SetItemText(line, 4, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].std);
		m_gridProcessCardNo.SetItemText(line, 5, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].cp);
		m_gridProcessCardNo.SetItemText(line, 6, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].ca);
		m_gridProcessCardNo.SetItemText(line, 7, str);
		str.Format(_T("%.3f"), m_pCpk->m_arrProcessCardNo[i].cpk);
		m_gridProcessCardNo.SetItemText(line, 8, str);*/
		delete csv_plannedNo;

		csv_plannedNo = NULL;

	}

	return TRUE;
}

void CCpkLib::Init(void)
{
	CFileFind findini;

	BOOL ifFind = findini.FindFile(m_iniPath);

	if (!ifFind)
	{
		WritePrivateProfileString(_T("CPK设定"), _T("CP下限"), _T("1.0"), m_iniPath);
		WritePrivateProfileString(_T("CPK设定"), _T("CA上限"), _T("0.5"), m_iniPath);
		WritePrivateProfileString(_T("CPK设定"), _T("CPK下限"), _T("1.0"), m_iniPath);
		WritePrivateProfileString(_T("CPK设定"), _T("起始数量"), _T("20"), m_iniPath);
	}

	TCHAR ret[100] = { 0 };
	GetPrivateProfileString(_T("CPK设定"), _T("CP下限"), _T(""), ret, sizeof(ret), m_iniPath);
	m_cpkSet.m_min_cp = _ttof(ret);
	GetPrivateProfileString(_T("CPK设定"), _T("CA上限"), _T(""), ret, sizeof(ret), m_iniPath);
	m_cpkSet.m_max_ca = _ttof(ret);
	GetPrivateProfileString(_T("CPK设定"), _T("CPK下限"), _T(""), ret, sizeof(ret), m_iniPath);
	m_cpkSet.m_min_cpk = _ttof(ret);
	GetPrivateProfileString(_T("CPK设定"), _T("起始数量"), _T(""), ret, sizeof(ret), m_iniPath);
	m_cpkSet.m_start_num = _ttoi(ret);
	findini.Close();
}

void CCpkLib::SaveSet(void)
{
	CString str;

	str.Format(_T("%.3f"), m_cpkSet.m_min_cp);
	WritePrivateProfileString(_T("CPK设定"), _T("CP下限"), str, m_iniPath);
	str.Format(_T("%.3f"), m_cpkSet.m_max_ca);
	WritePrivateProfileString(_T("CPK设定"), _T("CA上限"), str, m_iniPath);
	str.Format(_T("%.3f"), m_cpkSet.m_min_cpk);
	WritePrivateProfileString(_T("CPK设定"), _T("CPK下限"), str, m_iniPath);
	str.Format(_T("%d"), m_cpkSet.m_start_num);
	WritePrivateProfileString(_T("CPK设定"), _T("起始数量"), str, m_iniPath);
}
void CCpkLib::CloseCsv()
{
	if (m_csv_plannedNo)
	{
		delete m_csv_plannedNo;
		m_csv_plannedNo = NULL;
	}

	if (m_csv_processCardNo)
	{
		delete m_csv_processCardNo;
		m_csv_processCardNo = NULL;
	}
}


BOOL CCpkLib::OpenProcessCard(CString plannedNo, CString ProcessCardNo)
{
	CString path_plannedNo = m_cpkPath + plannedNo;

	CFileFind findini;

	if (!findini.FindFile(path_plannedNo + _T("\\*.*")))
	{
		if (!CreateDirectory(path_plannedNo + _T("\\"), NULL))
		{
			AfxMessageBox(_T("计划号目录创建失败！"));
			findini.Close();
			return FALSE;
		}
	}

	CString path = path_plannedNo + _T("\\") + plannedNo + _T(".csv");

	BOOL ifFind = findini.FindFile(path);

	TRY
	{
		if (m_csv_plannedNo)
		{
			delete m_csv_plannedNo;
			m_csv_plannedNo = NULL;
		}

		if (!m_csv_plannedNo)
			m_csv_plannedNo = new CCSVFile(path, CCSVFile::modeWrite);
		else
			AfxMessageBox(_T("计划号csv打开失败"));
	}
	CATCH(CFileException, e)
	{
		CString str;
		e->GetErrorMessage(str.GetBufferSetLength(256), 256);
		str.ReleaseBufferSetLength(256);
		AfxMessageBox(str);
		findini.Close();
		return FALSE;
	}
	END_CATCH

		if (!ifFind)  //ini创建，仅此一次
		{
			if (m_csv_plannedNo) {
				CStringArray arr;
				CString str;
				arr.Add(_T("Data"));
				arr.Add(_T("流程卡"));
				arr.Add(_T("所测轴"));
				arr.Add(_T("中心"));
				arr.Add(_T("上限"));
				arr.Add(_T("下限"));
				arr.Add(_T("均值"));
				arr.Add(_T("散差"));
				arr.Add(_T("CP"));
				arr.Add(_T("CA"));
				arr.Add(_T("CPK"));
				m_csv_plannedNo->WriteData(arr);
				arr.RemoveAll();

			}
		}

		path = path_plannedNo + _T("\\") + ProcessCardNo + _T(".csv");

		ifFind = findini.FindFile(path);


		TRY
		{
			if (m_csv_processCardNo)
			{
				delete m_csv_processCardNo;
				m_csv_processCardNo = NULL;
			}
			if (!m_csv_processCardNo)
				m_csv_processCardNo = new CCSVFile(path, CCSVFile::modeWrite);
			else
				AfxMessageBox(_T("流程卡csv打开失败"));
		}
		CATCH(CFileException, e)
		{
			CString str;
			e->GetErrorMessage(str.GetBufferSetLength(256), 256);
			str.ReleaseBufferSetLength(256);
			AfxMessageBox(str);
			findini.Close();
			return FALSE;
		}
		END_CATCH
			findini.Close();
		if (!ifFind)  //ini创建，仅此一次
		{

			if (m_csv_processCardNo) {
				CStringArray arr;
				CString str;
				arr.Add(_T("No"));
				arr.Add(_T("光轴"));
				arr.Add(_T("电轴"));
				arr.Add(_T("等效角"));
				arr.Add(_T("原始光轴"));
				arr.Add(_T("原始电轴"));
				arr.Add(_T("档位"));
				m_csv_processCardNo->WriteData(arr);
				arr.RemoveAll();
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
}

BOOL CCpkLib::AddToPlannedCsv(CProcessCard processCard)
{
	if (m_csv_plannedNo) {
		CStringArray arr;
		CString str;
		char ch[30];
		GetData(ch);
		arr.Add(CA2T(ch));        // DATA
		arr.Add(processCard.no);  //流程卡
		arr.Add(processCard.axis);
		str.Format(_T("%d"), SEC_TO_USER(processCard.center));
		arr.Add(str);
		str.Format(_T("%d"), SEC_TO_USER(processCard.usl));
		arr.Add(str);
		str.Format(_T("%d"), SEC_TO_USER(processCard.lsl));
		arr.Add(str);
		/*
		str.Format(_T("%.3f"), processCard.avg);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.std);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.cp);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.ca);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.cpk);
		arr.Add(str);*/
		m_csv_plannedNo->WriteData(arr);
		arr.RemoveAll();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCpkLib::AddCpkToPlannedCsv(CProcessCard processCard)
{
	if (m_csv_plannedNo) 
	{
		CStringArray arr;
		CString str;
		//char ch[30];
		//GetData(ch);
		//arr.Add(CA2T(ch));        // DATA
		//arr.Add(processCard.no);  //流程卡
		//arr.Add(processCard.axis);
		//str.Format(_T("%d"), processCard.center);
		//arr.Add(str);
		//str.Format(_T("%d"), processCard.usl);
		//arr.Add(str);
		//str.Format(_T("%d"), processCard.lsl);
		//arr.Add(str);
		str.Format(_T("%.3f"), processCard.avg);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.std);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.cp);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.ca);
		arr.Add(str);
		str.Format(_T("%.3f"), processCard.cpk);
		arr.Add(str);
		m_csv_plannedNo->WriteDataWithoutReturn(arr);
		arr.RemoveAll();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCpkLib::AddToProcessCardCsv(CProcessData processData)
{
	if (m_csv_processCardNo) 
	{
		CStringArray arr;
		CString str;
		str.Format(_T("%d"), processData.no);
		arr.Add(str);
		str.Format(_T("%d"), SEC_TO_USER(processData.laser));
		arr.Add(str);
		str.Format(_T("%d"), SEC_TO_USER(processData.phi));
		arr.Add(str);
		str.Format(_T("%d"), SEC_TO_USER(processData.equal));
		arr.Add(str);
		str.Format(_T("%d"), SEC_TO_USER(processData.laser0));
		arr.Add(str);
		str.Format(_T("%d"), SEC_TO_USER(processData.phi0));
		arr.Add(str);
		str.Format(_T("%d"), processData.sort);
		arr.Add(str);
		m_csv_processCardNo->WriteData(arr);
		arr.RemoveAll();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CCpkLib::GetData(char* data)
{
	time_t l_time;
	l_time = time(NULL);
	struct tm *l_struct_tm;
	l_struct_tm = localtime(&l_time);
	strftime(data, 30, "%Y-%m-%d--%H-%M-%S", l_struct_tm);
	return 1;
}

BOOL CCpkLib::CalcCpk(CString plannedNo, CString ProcessCardNo, CProcessCard & processCard)
{
	// 加载计划号。
	CString path;
	path = m_cpkPath + plannedNo + _T("\\") + plannedNo + _T(".csv");

	CCSVFile *csv = new CCSVFile(path, CCSVFile::modeRead);

	if (csv)
	{
		CStringArray arr;
		csv->SeekToBegin();
		while (csv->ReadData(arr))
		{
			if (arr.GetCount() > 5 ) {
				processCard.no = arr[PLANNED_CSV_ROW_NO];

				if (processCard.no == ProcessCardNo)
				{

					processCard.axis = arr[PLANNED_CSV_ROW_AXIS];
					processCard.center = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_CENTER]));
					processCard.usl = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_USL]));
					processCard.lsl = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_LSL]));
					processCard.avg = 0;//_ttof(arr[PLANNED_CSV_ROW_AVG]);
					processCard.std = 0;//_ttof(arr[PLANNED_CSV_ROW_STD]);
					processCard.cp = 0;//_ttof(arr[PLANNED_CSV_ROW_CP]);
					processCard.ca = 0;//_ttof(arr[PLANNED_CSV_ROW_CA]);
					processCard.cpk =0;// _ttof(arr[PLANNED_CSV_ROW_CPK]);

					break;
				}
			}
		}

		delete csv;

		csv = NULL;

	}
	else
	{
		return FALSE;
	}
	// 加载流程卡，计算CPK
	path = m_cpkPath + plannedNo + _T("\\") + ProcessCardNo + _T(".csv");

	csv = new CCSVFile(path, CCSVFile::modeRead);

	CProcessData data;

	int num = 0;

	if (csv)
	{
		CStringArray arr;
		csv->SeekToBegin();

		double avg=0,std=0,std2=0;

		while (csv->ReadData(arr))
		{
			if (arr.GetCount() == PROCESS_CSV_ROW_MAX) {

				data.no = _ttoi(arr[PROCESS_CSV_ROW_NO]);
				data.laser = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_LASER]));
				data.phi = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_PHI]));
				data.equal = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_EQUAL]));
				data.laser0 = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_LASER0]));
				data.phi0 = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_PHI0]));
				data.sort = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_POS]));

				double angle = 0;

				if (processCard.axis == "光轴") 
				{
					angle = data.laser;
				}
				else if(processCard.axis == "电轴")
				{
					angle = data.phi;
				}
				else if(processCard.axis == "等效角度")
				{
					angle = data.equal;
				}
				else
				{
					AfxMessageBox(_T("所测角不对"));
				}
				
				if(angle>=processCard.lsl&&angle<processCard.usl)
				{
					num++;
					CalcAvgStd(num, angle, avg, std, std2);
				}

			}
		}
		delete csv;

		csv = NULL;

		processCard.avg = avg;//_ttof(arr[PLANNED_CSV_ROW_AVG]);
		processCard.std = std;//_ttof(arr[PLANNED_CSV_ROW_STD]);

		processCard.ca = (processCard.avg-processCard.center)*2/(processCard.usl-processCard.lsl);
		processCard.cp = (processCard.usl-processCard.lsl)/6/processCard.std;
		processCard.cpk = processCard.cp * fabs(1-processCard.ca);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCpkLib::CalcCpk( CProcessCard & processCard)
{
	ASSERT(m_csv_plannedNo && m_csv_processCardNo);
	// 加载计划号。

	CCSVFile *csv = m_csv_plannedNo;

	if (csv)
	{
		CStringArray arr;
		csv->SeekToBegin();
		// 遍历到ed
		while (csv->ReadData(arr))
		{
			if (arr.GetCount() > 5 ) {
				processCard.no = arr[PLANNED_CSV_ROW_NO];

				//if (processCard.no == ProcessCardNo)
				{

					processCard.axis = arr[PLANNED_CSV_ROW_AXIS];
					processCard.center = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_CENTER]));
					processCard.usl = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_USL]));
					processCard.lsl = USER_TO_SEC(_ttoi(arr[PLANNED_CSV_ROW_LSL]));
					processCard.avg = 0;//_ttof(arr[PLANNED_CSV_ROW_AVG]);
					processCard.std = 0;//_ttof(arr[PLANNED_CSV_ROW_STD]);
					processCard.cp = 0;//_ttof(arr[PLANNED_CSV_ROW_CP]);
					processCard.ca = 0;//_ttof(arr[PLANNED_CSV_ROW_CA]);
					processCard.cpk =0;// _ttof(arr[PLANNED_CSV_ROW_CPK]);

					//break;
				}
			}
		}
	}
	// 加载流程卡，计算CPK
	csv = m_csv_processCardNo;

	CProcessData data;

	int num = 0;

	if (csv)
	{
		CStringArray arr;
		csv->SeekToBegin();

		double avg=0,std=0,std2=0;

		while (csv->ReadData(arr))
		{
			if (arr.GetCount() == PROCESS_CSV_ROW_MAX) {

				data.no = _ttoi(arr[PROCESS_CSV_ROW_NO]);
				data.laser = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_LASER]));
				data.phi = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_PHI]));
				data.equal = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_EQUAL]));
				data.laser0 = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_LASER0]));
				data.phi0 = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_PHI0]));
				data.sort = USER_TO_SEC(_ttoi(arr[PROCESS_CSV_ROW_POS]));

				double angle = 0;

				if (processCard.axis == "光轴") 
				{
					angle = data.laser;
				}
				else if(processCard.axis == "电轴")
				{
					angle = data.phi;
				}
				else if(processCard.axis == "等效角度")
				{
					angle = data.equal;
				}
				else
				{
					AfxMessageBox(_T("所测角不对"));
				}

				if(angle>=processCard.lsl&&angle<processCard.usl&&data.sort<24)
				{
					num++;
					CalcAvgStd(num, angle, avg, std, std2);
				}

			}
		}
		processCard.avg = avg;//_ttof(arr[PLANNED_CSV_ROW_AVG]);
		processCard.std = std;//_ttof(arr[PLANNED_CSV_ROW_STD]);

		processCard.ca = (processCard.avg-processCard.center)*2/(processCard.usl-processCard.lsl);
		processCard.cp = (processCard.usl-processCard.lsl)/6/processCard.std;
		processCard.cpk = processCard.cp * fabs(1-processCard.ca);
		processCard.num = num;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CCpkLib::CalcTrend(CString plannedNo/*, CString ProcessCardNo*/, CArray<double, double> &arrCpk)
{
	// 加载计划号。
	CString path;
	path = m_cpkPath + plannedNo + _T("\\") + plannedNo + _T(".csv");

	CCSVFile *csv = new CCSVFile(path, CCSVFile::modeRead);

	arrCpk.RemoveAll();

	if (csv)
	{
		csv->SeekToBegin();
		CStringArray arr;
		while (csv->ReadData(arr))
		{
			if (arr.GetCount() > 5 ) {

				CProcessCard  processCard;

				processCard.no = arr[PLANNED_CSV_ROW_NO];

        if (processCard.no != _T("流程卡"))
        {

          CalcCpk(plannedNo, processCard.no, processCard);

          arrCpk.Add(processCard.cpk);

        }
			}
		}

		delete csv;

		csv = NULL;

	}
	else
	{
		return FALSE;
	}

	return TRUE;
}