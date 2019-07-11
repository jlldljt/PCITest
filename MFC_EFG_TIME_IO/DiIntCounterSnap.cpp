//特殊功能模块，非基础模块,测量数据采集
//根据具体的硬件接线，实现转盘一圈正弦曲线的一次捕捉

#include "StdAfx.h"
#include "DiIntCounterSnap.h"
#include <math.h>
#include "EfgAlg.h"

#define SIN_CNT_NUM 4
#define CIRCLE_CNT_NUM 2
#define XRAY_CNT_NUM 2

//x光计数，采取查询方式
CWinThread * gTrdXRay = NULL;
UINT XRayMsg(LPVOID pParam)
{
	CDiIntCounterSnap* param = (CDiIntCounterSnap*)pParam;
	int delay = 0;
	if (param->m_device < 0 || !param->m_card)
		return FALSE;
	DWORD tick;
	double fparam1, fparam2;
	param->m_card->ReadDi(2, fparam1, fparam2);
	while (!param->m_counter.start);
	tick = GetTickCount();
	while (param->m_counter.start && param->m_counter.index[1] < XRAY_ONESHOT_NUM) {
		double fparam;
		param->m_card->ReadDi(2, fparam, fparam2);
		if (fparam != fparam1) {
			param->m_counter.flag = 1;//指示已经有电平变换
			tick = GetTickCount();
			fparam1 = fparam;
			//读取计数器
			for (int i = 0; i < XRAY_CNT_NUM; i++) {
				param->m_card->ReadT1(i, fparam, param->m_counter.counter[4+i][param->m_counter.index[1]]);
			}
			param->m_counter.index[1]++;
		}

		if (param->m_counter.flag && GetTickCount() - tick > 1000) {
			break;
		}
	}

	param->m_counter.start--;
	param->m_counter.flag = 0;

	//param->StopDiInt();

	//TODO:计算


	AfxMessageBox(L"xray over");


	return 0;
}

CWinThread * gTrdLaser = NULL;
UINT LaserMsg(LPVOID pParam)
{
	CDiIntCounterSnap* param = (CDiIntCounterSnap*)pParam;
	int delay = 0;
	if (param->m_device < 0 || !param->m_card)
		return FALSE;
	DWORD tick;
	double fparam1, fparam2;
	param->m_card->ReadDi(1, fparam1, fparam2);
	while (!param->m_counter.start);
	tick = GetTickCount();
	while (param->m_counter.start && param->m_counter.index[0] < LASER_SIN_NUM) {
		double fparam,tmpfparam;
		int cnt = 10;
		param->m_card->ReadDi(1, fparam, fparam2);
		while(cnt--);
		param->m_card->ReadDi(1, tmpfparam, fparam2);
		if(tmpfparam == fparam)
			if (fparam != fparam1) {
				param->m_counter.flag = 1;//指示已经有电平变换
				tick = GetTickCount();
				fparam1 = fparam;
				//读取计数器
				if(fparam == 0)//下降沿
				{
					for (int i = 0; i < SIN_CNT_NUM; i++) {
						param->m_card->ReadT0(i+4, fparam, param->m_counter.counter[i][param->m_counter.index[0]]);
					}
					param->m_counter.index[0]++;
				}
			}

			if (param->m_counter.flag && GetTickCount() - tick > 1000) {
				break;
			}
	}

	param->m_counter.start--;
	param->m_counter.flag = 0;

	//param->StopDiInt();

	//TODO:计算


	AfxMessageBox(L"laser over");


	return 0;
}
// 修改
CWinThread * gTrdAll = NULL;
#ifdef USE_EFGV1
unsigned char g_tmp_counter[6][COUNTER_NUM*2]; // 测量时前4个激光，后两个x光
#endif // USE_EFGV1

UINT AllMsg(LPVOID pParam)
{
	if(0 == SetThreadAffinityMask(GetCurrentThread(), 4))
		SetThreadAffinityMask(GetCurrentThread(), 3);

	//SetThreadAffinityMask(GetCurrentThread(), 1);
#define FILTER_NUM 0//滤波 n次
	CDiIntCounterSnap* param = (CDiIntCounterSnap*)pParam;
	//int delay = 0;
	if (param->m_device < 0 || !param->m_card)
		return FALSE;
#ifdef USE_EFGV1
	while (!param->m_counter.start);

  param->m_efgio->StartMeasure(1);
  while (!param->m_efgio->CheckMeasureEnd());

  for (int i = 0; i < 5; i++)// 获取所有计数器数量
  {
    param->m_efgio->GetCntDataNum(i);
  }

  param->m_counter.index[0] = param->m_efgio->m_resultParam.measure.cnt_num[0];// 激光数量

  for (int i = 0; i < 4; i++)
  {
    if(param->m_efgio->m_resultParam.measure.cnt_num[i]+3 < LASER_SIN_NUM)//;// 检验数量是否超标
	{
    int rdlen = param->m_efgio->GetAllCntData(i, (char*)(g_tmp_counter[i]), param->m_efgio->m_resultParam.measure.cnt_num[i]*2+6/*命令头4B+crc2B*/);//获取数据
    //ASSERT(rdlen == param->m_efgio->m_resultParam.measusre.cnt_num[i] * 2 + 6); // 检验接收数量是否一致

    if(param->m_counter.index[0] > param->m_efgio->m_resultParam.measure.cnt_num[i])// 激光数量存最小的
      param->m_counter.index[0] = param->m_efgio->m_resultParam.measure.cnt_num[i];
	}

  }

  if(param->m_efgio->m_resultParam.measure.cnt_num[4]+3 < XRAY_ONESHOT_NUM)//;// 检验数量是否超标
  {
  int rdlen = param->m_efgio->GetAllCntData(4, (char*)(g_tmp_counter[4]), param->m_efgio->m_resultParam.measure.cnt_num[4]*2+6);//获取数据
  //ASSERT(rdlen == param->m_efgio->m_resultParam.measure.cnt_num[4] + 6); // 检验接收数量是否一致
  param->m_counter.index[1] = param->m_efgio->m_resultParam.measure.cnt_num[4];// X光数量
  }
  for (int i = 0; i < 5; i++)//复制到原先的计数器数组中
  {
    for (int j = 0; j < param->m_efgio->m_resultParam.measure.cnt_num[i];j++)
		//param->m_counter.counter[i][j] = g_tmp_counter[i][j+4];
      param->m_counter.counter[i][j] = (g_tmp_counter[i][j*2+4+1]<<8)|(g_tmp_counter[i][j*2+4]);
  }

  

#elif USE_AC6641
	double fparam0=0,tmpfparam0=0, fparam01=0, fparam02=0;
	double fparam1=0,tmpfparam1=0, fparam11=0, fparam12=0;
	double fparam2=0,tmpfparam2=0, fparam21=0, fparam22=0;
	int cnt=0;
	int zero_flag = 0;

	while (!param->m_counter.start);

	double tmp_val=0;
	//  param->m_efgio->WriteDo(XRAY_GATE, IO_ON);

	//param->m_card->WriteDO(XRAY_CTRL_GATE, tmp_val = 1);//guang
	////kaishi
	//param->m_card->ReadDi(XRAY_CNT_GATE, fparam21, fparam22);//x
	//param->m_card->ReadDi(LASER_CNT_GATE, fparam11, fparam12);//激光
	//param->m_card->ReadDi(TURNTABLE_ZERO, fparam01, fparam02);//零位

	////读取计数器
	//for (int i = 0; i < XRAY_CNT_NUM; i++) {
	//	param->m_card->ReadT1(i+ XRAY_CNT_START, fparam22, param->m_counter.counter[SIN_CNT_NUM +i][param->m_counter.index[1]]);
	//}
	//param->m_counter.index[1]++;

	//for (int i = 0; i < SIN_CNT_NUM; i++) {
	//	param->m_card->ReadT0(i+ LASER_CNT_START, fparam12, param->m_counter.counter[i][param->m_counter.index[0]]);
	//}
	//param->m_counter.index[0]++;


	while(1){
		param->m_card->ReadDi(XRAY_CNT_GATE, fparam2, fparam22);
		param->m_card->ReadDi(LASER_CNT_GATE, fparam1, fparam12);
		param->m_card->ReadDi(TURNTABLE_ZERO, fparam0, fparam02);
		for(int i = 0; i < FILTER_NUM; i++)
		{
			cnt = 10;
			while(cnt--);
			param->m_card->ReadDi(XRAY_CNT_GATE, tmpfparam2, fparam22);
			fparam2+=tmpfparam2;
			param->m_card->ReadDi(LASER_CNT_GATE, tmpfparam1, fparam12);
			fparam1+=tmpfparam1;
			param->m_card->ReadDi(TURNTABLE_ZERO, tmpfparam0, fparam02);
			fparam0+=tmpfparam0;
		}
		if (0==fparam0 || FILTER_NUM+1==fparam0) {

			if (fparam0 != fparam01) {
				fparam01 = fparam0;
				//读取计数器
				if(fparam0 == 0)//下降沿
				{
					switch(zero_flag) {
					case 0://一个零位脉冲结束
						zero_flag =1;
						{
							//start 记录第一次数据
							param->m_card->ReadDi(XRAY_CNT_GATE, fparam21, fparam22);//x
							param->m_card->ReadDi(LASER_CNT_GATE, fparam11, fparam12);//激光
							param->m_card->ReadDi(TURNTABLE_ZERO, fparam01, fparam02);//零位

							//读取计数器
							for (int i = 0; i < XRAY_CNT_NUM; i++) {
								param->m_card->ReadT1(i + XRAY_CNT_START, fparam22, param->m_counter.counter[SIN_CNT_NUM + i][param->m_counter.index[1]]);
							}
							param->m_counter.index[1]++;

							for (int i = 0; i < SIN_CNT_NUM; i++) {
								param->m_card->ReadT0(i + LASER_CNT_START, fparam12, param->m_counter.counter[i][param->m_counter.index[0]]);
							}
							param->m_counter.index[0]++;

							continue;
						}
						break;
					case 1://两个零位脉冲结束
						zero_flag = 2;
						break;


					}
				}
			}
		}

		if (1==zero_flag&&(0==fparam2 || FILTER_NUM+1==fparam2) && param->m_counter.index[1] <= XRAY_ONESHOT_NUM) {

			if (fparam2 != fparam21) {
				fparam21 = fparam2;
				//读取计数器
				for (int i = 0; i < XRAY_CNT_NUM; i++) {
					param->m_card->ReadT1(i+ XRAY_CNT_START, fparam22, param->m_counter.counter[SIN_CNT_NUM +i][param->m_counter.index[1]]);
				}
				param->m_counter.index[1]++;
			}
		}

		//TODO:计算
		//

		if (1==zero_flag&&(0==fparam1 || FILTER_NUM+1==fparam1) && param->m_counter.index[0] <= LASER_SIN_NUM) {

			if (fparam1 != fparam11) {
				fparam11 = fparam1;
				//读取计数器
				if(fparam1 == 0)//下降沿
				{
					for (int i = 0; i < SIN_CNT_NUM; i++) {
						param->m_card->ReadT0(i+ LASER_CNT_START, fparam12, param->m_counter.counter[i][param->m_counter.index[0]]);
					}
					param->m_counter.index[0]++;
				}
			}
		}
		//判断超市


		if(2==zero_flag||!param->m_counter.start ||(param->m_counter.index[1] > XRAY_ONESHOT_NUM && param->m_counter.index[0] > LASER_SIN_NUM))
			break;
	}

#endif
	param->m_counter.start=0;
	param->m_counter.flag = 0;

	//param->StopDiInt();

	//TODO:计算
	//	param->m_efgio->WriteDo(XRAY_GATE, IO_OFF);
	//param->m_card->WriteDO(XRAY_CTRL_GATE, tmp_val = 0);//guang
	//AfxMessageBox(L"over");


	return 0;
}

UINT AllMsgPlus(LPVOID pParam)
{
	if(0 == SetThreadAffinityMask(GetCurrentThread(), 4))
		SetThreadAffinityMask(GetCurrentThread(), 3);

	//SetThreadAffinityMask(GetCurrentThread(), 1);
#define FILTER_NUM 0//滤波 n次
	CDiIntCounterSnap* param = (CDiIntCounterSnap*)pParam;
	//int delay = 0;
	if (param->m_device < 0 || !param->m_card)
		return FALSE;
#ifdef USE_EFGV1
	while (!param->m_counter.start);

  param->m_efgio->StartMeasure(1);
  while (!param->m_efgio->CheckMeasureEnd());

  for (int i = 0; i < 5; i++)// 获取所有计数器数量
  {
    param->m_efgio->GetCntDataNum(i);
  }

  param->m_counter.index[0] = param->m_efgio->m_resultParam.measure.cnt_num[0];// 激光数量

  for (int i = 0; i < 4; i++)
  {
    if(param->m_efgio->m_resultParam.measure.cnt_num[i]+3 < LASER_SIN_NUM)//;// 检验数量是否超标
	{
    int rdlen = param->m_efgio->GetAllCntData(i, (char*)(g_tmp_counter[i]), param->m_efgio->m_resultParam.measure.cnt_num[i]*2+6/*命令头4B+crc2B*/);//获取数据
    //ASSERT(rdlen == param->m_efgio->m_resultParam.measusre.cnt_num[i] * 2 + 6); // 检验接收数量是否一致

    if(param->m_counter.index[0] > param->m_efgio->m_resultParam.measure.cnt_num[i])// 激光数量存最小的
      param->m_counter.index[0] = param->m_efgio->m_resultParam.measure.cnt_num[i];
	}

  }

  if(param->m_efgio->m_resultParam.measure.cnt_num[4]+3 < XRAY_ONESHOT_NUM)//;// 检验数量是否超标
  {
  int rdlen = param->m_efgio->GetAllCntData(4, (char*)(g_tmp_counter[4]), param->m_efgio->m_resultParam.measure.cnt_num[4]*2+6);//获取数据
  //ASSERT(rdlen == param->m_efgio->m_resultParam.measure.cnt_num[4] + 6); // 检验接收数量是否一致
  param->m_counter.index[1] = param->m_efgio->m_resultParam.measure.cnt_num[4];// X光数量
  }
  for (int i = 0; i < 5; i++)//复制到原先的计数器数组中
  {
    for (int j = 0; j < param->m_efgio->m_resultParam.measure.cnt_num[i];j++)
		//param->m_counter.counter[i][j] = g_tmp_counter[i][j+4];
      param->m_counter.counter[i][j] = (g_tmp_counter[i][j*2+4+1]<<8)|(g_tmp_counter[i][j*2+4]);
  }
#endif
	param->m_counter.start=0;
	param->m_counter.flag = 0;
	return 0;
}
#ifndef USE_EFGV1
CWinThread * gTrdU = NULL;
UINT UMsg(LPVOID pParam)
{
	CDiIntCounterSnap* param = (CDiIntCounterSnap*)pParam;

	if (param->m_device < 0 || !param->m_card)
		return FALSE;

	double tmp_val;
	param->m_card->WriteDO(XRAY_CTRL_GATE, tmp_val=0);//guang
	param->m_card->WriteDO(U_GATE, tmp_val=0);//gate
	param->m_card->WriteDO(U_EN, tmp_val=0);//en

	while(1)
	{
		//急停
		if(param->m_motor_u.stop)
		{
			param->m_motor_u.dst_step = param->m_motor_u.cur_step;
			param->m_motor_u.state = MOTOR_STOP;
			param->m_motor_u.stop = 0;
			break;
		}

		int dvalue = param->m_motor_u.dst_step - param->m_motor_u.cur_step;

		//控制方向
		if(dvalue < 0) // 反转
		{
			double val = 0;
			param->m_card->WriteDO(U_DIR, val=0);
			param->m_motor_u.cur_step--;
			param->m_motor_u.state = MOTOR_RUN;
		}
		else if(dvalue >0) //正转
		{
			double val = 1;
			param->m_card->WriteDO(U_DIR, val=1);
			param->m_motor_u.cur_step++;
			param->m_motor_u.state = MOTOR_RUN;
		}
		else //停止
		{
			param->m_motor_u.state = MOTOR_STOP;
			break;
		}

		//

		//一个脉冲
		double val = 1;
		double delay = param->m_motor_u.max_speed/2;
		param->m_card->WriteDO(U_OUTP, val=1);
		param->m_card->WriteDO(U_OUTN, val=0);
		//Sleep(delay);
		for(int i = 0; i < 1000000; i++)//400us 100000
		{

		}

		val = 0;
		param->m_card->WriteDO(U_OUTP, val=0);
		param->m_card->WriteDO(U_OUTN, val=1);
		//Sleep(delay);
		for(int i = 0; i < 1000000; i++)
		{

		}
	}

	param->m_card->WriteDO(U_GATE, tmp_val=1);
	AfxMessageBox(L"over");
}
#endif

CWinThread * gTrdCircle = NULL;
UINT CircleMsg(LPVOID pParam)
{
	if(0 == SetThreadAffinityMask(GetCurrentThread(), 2))
		SetThreadAffinityMask(GetCurrentThread(), 2);

	//SetThreadAffinityMask(GetCurrentThread(), 1);
#define FILTER_NUM 4//滤波 n-1次
	CDiIntCounterSnap* param = (CDiIntCounterSnap*)pParam;
	//int delay = 0;
	if (param->m_device < 0 || !param->m_card)
		return FALSE;
	double fparam0,tmpfparam0, fparam01, fparam02;
	double fparam1,tmpfparam1, fparam11, fparam12;
	int cnt;
	int zero_flag = 0;
	param->StartCaptureSin(OUT3_COUNTER, 1, OUT6_COUNTER, 150);
	while (!param->m_counter.start);

	double tmp_val;
	param->m_card->ReadDi(LASER_CNT_GATE, fparam11, fparam12);//激光
	param->m_card->ReadDi(TURNTABLE_ZERO, fparam01, fparam02);//零位

	//读取计数器
	for (int i = 0; i < SIN_CNT_NUM; i++) {
		param->m_card->ReadT0(i+ LASER_CNT_START, fparam12, param->m_counter.counter[i][param->m_counter.index[0]]);
	}
	param->m_counter.index[0]++;


	while(1){
		param->m_card->ReadDi(LASER_CNT_GATE, fparam1, fparam12);
		param->m_card->ReadDi(TURNTABLE_ZERO, fparam0, fparam02);
		for(int i = 0; i < FILTER_NUM; i++)
		{
			cnt = 10;
			while(cnt--);
			param->m_card->ReadDi(LASER_CNT_GATE, tmpfparam1, fparam12);
			fparam1+=tmpfparam1;
			param->m_card->ReadDi(TURNTABLE_ZERO, tmpfparam0, fparam02);
			fparam0+=tmpfparam0;
		}
		if (0==fparam0 || FILTER_NUM+1==fparam0) {

			if (fparam0 != fparam01) {
				fparam01 = fparam0;
				//读取计数器
				if(fparam0 == 0)//下降沿
				{
					switch(zero_flag) {
					case 0:
						zero_flag =1;
						break;
					case 1:
						zero_flag = 2;
						break;


					}
				}
			}
		}

		if (1==zero_flag&&(0==fparam1 || FILTER_NUM+1==fparam1) && param->m_counter.index[0] <= LASER_SIN_NUM) {

			if (fparam1 != fparam11) {
				fparam11 = fparam1;
				//读取计数器
				if(fparam1 == 0)//下降沿
				{
					for (int i = 0; i < SIN_CNT_NUM; i++) {
						param->m_card->ReadT0(i+ LASER_CNT_START, fparam12, param->m_counter.counter[i][param->m_counter.index[0]]);
					}
					param->m_counter.index[0]++;
				}
			}
		}
		//判断超市


		if(2==zero_flag||!param->m_counter.start ||(param->m_counter.index[1] > XRAY_ONESHOT_NUM && param->m_counter.index[0] > LASER_SIN_NUM))
			break;
	}
	param->m_counter.start=0;
	param->m_counter.flag = 0;


	return 0;
}


CDiIntCounterSnap::CDiIntCounterSnap()
{
	m_device = -1;
	m_device = -1;
	m_card = NULL;
	m_card = NULL;
	m_viewBoard = NULL;
	m_efgio = NULL;
	memset(&m_counter, 0, sizeof(m_counter));
	m_channel = -1;
	m_channel = -1;
	m_counter.start = 0;
	m_counter.flag = 0;


	memset(&m_motor_u, 0, sizeof(m_motor_u));
	// TODO:删除
#ifdef  __DEBUG__
	for (int i = 0; i < COUNTER_NUM; i++)
	{
		m_counter.counter[0][i] = 50;
		m_counter.counter[1][i] = 100;
		m_counter.counter[2][i] = 50 + i;
		m_counter.counter[3][i] = 100 + i;
	}


	//TODO：调试用
	//for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
	//{
	//  //sin后面不是角度是弧度，故要转换
	//  //我们是匀速的，每个步进弧度都一致即2pi / XRAY_ONESHOT_NUM
	//  m_counter.counter[0][i] = 110 * sin(i * (2 * PI) / XRAY_ONESHOT_NUM + PI) + 370;
	//}
	int sin_num = LASER_SIN_NUM;
	for (int i = 0; i < sin_num; i++)
	{
		//x laser 
		//m_counter.counter[0][i] = 100 * sin(i * (2 * PI) / sin_num *4+ PI) + 300;
		m_counter.fit[0][i] = 100 * sin(i * (2 * PI) / sin_num + PI) + 300;
	}
#endif //  __DEBUG__
}


CDiIntCounterSnap::~CDiIntCounterSnap()
{
	m_device = -1;
	m_device = -1;
	m_card = NULL;
	m_card = NULL;
	m_viewBoard = NULL;
}

void CDiIntCounterSnap::UserFuncLaserSin(void * param)
{
	if (!param)
		return;
	CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
	usrParam->DIIntLaserSin();
}

void CDiIntCounterSnap::UserFuncLaserSin_TMC12A(void * param)
{
	if (!param)
		return;
	CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
	usrParam->DIIntLaserSin_TMC12A();
}

void CDiIntCounterSnap::UserFuncLaserCircle(void * param)
{
	if (!param)
		return;
	CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
	usrParam->DIIntLaserCircle();
}

void CDiIntCounterSnap::UserFuncXRayOneShot(void * param)
{
	if (!param)
		return;
	CDiIntCounterSnap* usrParam = (CDiIntCounterSnap*)param;
	usrParam->DIIntXRayOneShot();
}

void CDiIntCounterSnap::DIIntLaserSin(void)
{
	if (m_device < 0 || !m_card)
		return;
	if (m_counter.start) {
		memset(&m_counter, 0, sizeof(m_counter));

		if (m_viewBoard)
			m_viewBoard->Erase();

#ifndef  __DEBUG__
		for (int i = 0; i < SIN_CNT_NUM; i++) {
			m_card->StopT0(i);
			m_card->StartT0(i, m_device, 0, 0);//out6
		}
#endif
		m_counter.flag = 1;
	}

	if (!m_counter.flag)
		return;

#ifndef  __DEBUG__
	double fparam;
	for (int i = 0; i < SIN_CNT_NUM; i++) {
		m_card->ReadT0(i, m_counter.counter[i][m_counter.index[0]], fparam);
	}
#endif
	if (m_counter.index[0]++ >= LASER_SIN_NUM - 1)
	{
		m_counter.flag = 0;
#ifndef  __DEBUG__
		for (int i = LASER_SIN_NUM - 1; i > 0; i--) {
			for (int j = 0; j < SIN_CNT_NUM; j++) {
				m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
			}
			m_counter.fit[0][i] = (m_counter.counter[1][i] + m_counter.counter[0][i]+m_counter.counter[3][i] + m_counter.counter[2][i])/4;
		}
#endif


#ifdef __DEBUG__
		int sin_num = LASER_SIN_NUM;
		for (int i = 0; i < sin_num; i++)
		{
			//x laser 
			//m_counter.counter[0][i] = 100 * sin(i * (2 * PI) / sin_num *4+ PI) + 300;
			m_counter.fit[0][i] = 100 * sin(i * (2 * PI) / (sin_num-20) + PI) + 300;
		}
#endif // __DEBUG__
		//TODO：调试拟合
		EfgAlg alg;
		struct tagSinParam param;
		alg.FitSinByLeastSquares(m_counter.fit[0], LASER_SIN_NUM, m_counter.fit[1], param);

		if (m_viewBoard) {
			m_viewBoard->DrawLaserSin();

			CString str;
			str.Format(L"yi = %.2f * sin (%.2f * xi + %.2f) + %.2f", param.A, param.w, param.t, param.k);
			m_viewBoard->SetOutStr(str);
			///TODO：调试拟合

			POINT point;

#ifdef __DEBUG__
			for (int i = 0; i < LASER_SIN_NUM; i++)
			{
				point.x = i << 2;
				point.y = m_counter.fit[0][i];
				m_viewBoard->DrawPoint(point, RGB(255, 0, 0));
			}
#endif // __DEBUG__

			for (int i = 0; i < LASER_SIN_NUM; i++)
			{
				point.x = i << 2;
				point.y = m_counter.fit[1][i];
				m_viewBoard->DrawPoint(point);
				point.x += 2;
				m_viewBoard->DrawPoint(point);
			}
			m_viewBoard->Invalidate();
		}

		StopDiInt();
	}
}

//tmc12的由零位触发，不需要标记，理论上
void CDiIntCounterSnap::DIIntLaserSin_TMC12A(void)
{
	if (m_device < 0 || !m_card)
		return;
	if (m_counter.start && m_counter.index[0] < LASER_SIN_NUM) {
#ifndef  __DEBUG__
		double fparam;
		for (int i = 0; i < SIN_CNT_NUM; i++) {
			m_card->ReadT0(i + 4, fparam, m_counter.counter[i][m_counter.index[0]]);
		}
		m_counter.index[0]++;
#endif
	}else if(m_counter.index[0] >= LASER_SIN_NUM){
		m_counter.start = 0;
	}
}


void CDiIntCounterSnap::DIIntLaserCircle(void)
{
	int delay = 0;
	if (m_device < 0 || !m_card)
		return;
	if (m_counter.start) {
		memset(&m_counter, 0, sizeof(m_counter));

		if (m_viewBoard)
			m_viewBoard->Erase();

#ifndef  __DEBUG__
		for (int i = 0; i < CIRCLE_CNT_NUM; i++) {
			m_card->StopT0(i);
			m_card->StartT0(i, m_device, 0, 0);//out6
		}
		m_card->StopT0(m_out);
		m_card->StartT0(m_out, m_device, m_counter.index[0], 0);//指向下一行
#endif
		m_counter.flag = 1;
	}

	if (!m_counter.flag)
		return;

#ifndef  __DEBUG__
	double fparam;
	//读取计数器
	for (int i = 0; i < CIRCLE_CNT_NUM; i++) {
		m_card->ReadT0(i, m_counter.counter[i][m_counter.index[0]], fparam);
	}
#endif
	if (m_counter.index[0]++ >= LASER_CIRCLE_NUM - 1)
	{
		m_counter.flag = 0;
#ifndef  __DEBUG__
		for (int i = LASER_CIRCLE_NUM - 1; i > 0; i--) {
			for (int j = 0; j < CIRCLE_CNT_NUM; j++) {
				m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
			}
		}
#endif
		if (m_viewBoard) {
			m_viewBoard->DrawLaserCircle();
			// m_viewBoard->SetOutStr(L"");
			m_viewBoard->Invalidate();
		}

		StopDiInt();
	}
	m_card->StopT0(m_out);
	m_card->StartT0(m_out, m_device, m_counter.index[0], 0);//指向下一行
}


void CDiIntCounterSnap::DIIntXRayOneShot(void)
{
	int delay = 0;
	if (m_device < 0 || !m_card)
		return;
	if (m_counter.start) {
		memset(&m_counter, 0, sizeof(m_counter));

		if (m_viewBoard)
			m_viewBoard->Erase();

#ifndef  __DEBUG__
		for (int i = 0; i < XRAY_CNT_NUM; i++) {
			m_card->StopT0(i);
			m_card->StartT0(i, m_device, 0, 0);//out6
		}
#endif
		m_counter.flag = 1;
	}

	if (!m_counter.flag)
		return;

	double fparam;
#ifndef  __DEBUG__
	//读取计数器
	for (int i = 0; i < XRAY_CNT_NUM; i++) {
		m_card->ReadT0(i, m_counter.counter[i][m_counter.index[1]], fparam);
	}
#endif
	if (m_counter.index[1]++ >= XRAY_ONESHOT_NUM - 1)
	{
		m_counter.flag = 0;
#ifndef  __DEBUG__
		for (int i = XRAY_ONESHOT_NUM - 1; i > 0; i--) {
			for (int j = 0; j < XRAY_CNT_NUM; j++) {
				m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
			}
		}
		//使用中断信号的话，一个是0计数一个是有计数，直接相加即可
		for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
		{
			for (int j = 0; j < XRAY_CNT_NUM; j++) {
				m_counter.fit[0][i] += m_counter.counter[j][i];
			}
		}
#endif

#ifdef __DEBUG__
		int sin_num = XRAY_ONESHOT_NUM;
		for (int i = 0; i < sin_num; i++)
		{
			//x laser 
			//m_counter.counter[0][i] = 100 * sin(i * (2 * PI) / sin_num *4+ PI) + 300;
			m_counter.fit[0][i] = 100 * sin(i * (2 * PI) / sin_num * 2 + PI) + 300;
		}
#endif // __DEBUG__

		//TODO：调试拟合
		EfgAlg alg;
		// struct tagSinParam param;
		// alg.FitSinBySubstitution(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[1], param);
		// alg.FitSinByLeastSquares(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[2], param);
		alg.ExtractSpike(m_counter.fit[0], XRAY_ONESHOT_NUM, 300, 20, -1);

		if (m_viewBoard) {
			m_viewBoard->DrawXRayOneShot();

			//  m_viewBoard->SetOutStr(L"");
			//#ifdef  __DEBUG__
			//      for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
			//      {
			//        POINT point;
			//        point.x = i;
			//        point.y = m_counter.fit[0][i];
			//        m_viewBoard->DrawPoint(point);
			//      }
			//#endif
			///TODO：调试拟合
			int num = alg.GetSpikesNumber();
			if (num < 10)
			{
				POINT point;
				for (int i = 0; i < num; i++)
				{
					SPIKE spike;
					alg.GetSpike(i, spike);
					point = spike.p;
					m_viewBoard->DrawCircle(point, 10);
				}
			}
			m_viewBoard->Invalidate();
		}

		StopDiInt();
	}

}



////////////////////////////////////////////////////////////////////////
int CDiIntCounterSnap::BindCard(int device, CPCIBase * card, CBoardView *viewBoard, CEfgIO *efgio)
{
	//if (device < 0 || !card)
	//  return -1;

	m_device = device;
	m_card = card;
	m_viewBoard = viewBoard;
	m_efgio = efgio;
	return 0;
}

int CDiIntCounterSnap::StartDiIntLaserSin(int channel)
{
	if (m_device < 0 || !m_card)
		return -1;
	PCIType type = m_card->m_type;
	//m_card->StopDi(channel);
	switch (type) {
	case PCI1780U:
		if (m_card->StartDi(channel, m_device, 0, 0, UserFuncLaserSin, this)) {
			m_channel = channel;
			return 0;
		}
		break;
	case TMC12A:
		//if (m_card->StartDi(channel, m_device, 0, 0, UserFuncLaserSin_TMC12A, this)) {
		//  m_channel = channel;
		//  return 0;
		//}
		gTrdLaser = AfxBeginThread(LaserMsg, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		gTrdLaser->m_bAutoDelete = TRUE;
		gTrdLaser->ResumeThread();
		m_channel = channel;
		break;
	}



	return -1;
}

int CDiIntCounterSnap::StartDiIntLaserCircle(int channel)
{
	gTrdAll = AfxBeginThread(CircleMsg, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	gTrdAll->m_bAutoDelete = TRUE;
	gTrdAll->ResumeThread();
	m_channel = channel;

	return -1;
	/*if (m_device < 0 || !m_card)
	return -1;

	PCIType type = m_card->m_type;

	switch (type) {
	case PCI1780U:
	if (m_card->StartDi(channel, m_device, 0, 0, UserFuncLaserCircle, this)) {
	m_channel = channel;
	return 0;
	}
	break;
	case TMC12A:
	break;
	}

	return -1;*/

}

int CDiIntCounterSnap::StartDiIntXRayOneShot(int channel)
{
	if (m_device < 0 || !m_card)
		return -1;

	PCIType type = m_card->m_type;

	switch (type) {
	case PCI1780U:
		if (m_card->StartDi(channel, m_device, 0, 0, UserFuncXRayOneShot, this)) {
			m_channel = channel;
			return 0;
		}
		break;
	case TMC12A:
		gTrdXRay = AfxBeginThread(XRayMsg, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		gTrdXRay->m_bAutoDelete = TRUE;
		gTrdXRay->ResumeThread();
		m_channel = channel;
		break;
	}

	return -1;
}

int CDiIntCounterSnap::StartXRayAndLaser(int channel){

	gTrdAll = AfxBeginThread(AllMsg, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	gTrdAll->m_bAutoDelete = TRUE;
	gTrdAll->ResumeThread();
	m_channel = channel;

	return -1;
}

//int CDiIntCounterSnap::StartDiIntMeasure()
//{
//  if (!m_card_add || !m_card)
//    return -1;
//  if (m_card->StartDi(0, m_device, 0, 0, UserFuncXRayOneShot, this)) {
//    return 0;
//  }
//  if (m_card_add->StartDi(0, m_device, 0, 0, UserFuncLaserSin, this)) {
//    return 0;
//  }
//  return -1;
//}

int CDiIntCounterSnap::StartCaptureSin(int no1, double delay1/*out3*/, int no2, double delay2/*out6*/)
{
	if (m_device < 0 || !m_card || m_counter.start)
		return -1;

	m_card->StopT0(no1);//out3
	m_card->StopT0(no2);//out6


#ifndef  __DEBUG__
	//counter
	PCIType type = m_card->m_type;

	switch (type) {
	case PCI1780U:
		m_card->StartT0(no1, m_device, delay1, 0);//out3
		m_card->StartT0(no2, m_device, delay2, 0);//out6

		for (int i = 0; i < SIN_CNT_NUM; i++) {
			m_card->StopT0(i);
			m_card->StartT0(i, m_device, 0, 0);//out6
		}
		break;
	case TMC12A:
		m_card->StartT0(no1, m_device, 5, delay1);//out3

		m_card->StartT0(no2, m_device, 5, delay2);//out6

		//无法充值成65535，直接不重启，判断越界情况
		// for (int i = 4; i < 4 + SIN_CNT_NUM; i++) {
		//   if(FALSE == m_card->RestartT0(i)) {
		// AfxMessageBox(L"restart t0 error");
		//}
		// }
		break;
	}
#endif

	//memset(&m_counter, 0, sizeof(m_counter));
	//if (m_viewBoard)
	//  m_viewBoard->Erase();

	//m_counter.start = 1;
	m_counter.flag = 0;

	return 0;
}

int CDiIntCounterSnap::StartCaptureCircle(int out)
{
	if (m_device < 0 || !m_card)
		return -1;
	m_out = out;

	//memset(&m_counter, 0, sizeof(m_counter));

	//if (m_viewBoard)
	//  m_viewBoard->Erase();


	m_counter.start = 1;
	return 0;
}
int CDiIntCounterSnap::StartCaptureXRayOneShot()
{
	if (m_device < 0 || !m_card || m_counter.start)
		return -1;

	//memset(&m_counter, 0, sizeof(m_counter));

#ifndef  __DEBUG__
	//counter
	PCIType type = m_card->m_type;

	switch (type) {
	case PCI1780U:
		for (int i = 0; i < XRAY_CNT_NUM; i++) {
			m_card->StopT0(i);
			m_card->StartT0(i, m_device, 0, 0);//out6
		}
		break;
	case TMC12A:
		//for (int i=0; i < XRAY_CNT_NUM; i++) {
		//  m_card->RestartT1(i+2);
		//}
		break;
	}
#endif

	//if (m_viewBoard)
	//  m_viewBoard->Erase();


	//m_counter.start = 1;
	m_counter.flag = 0;
	return 0;
}



int CDiIntCounterSnap::StartMeasure()
{
	// TODO: 发开启转盘零位计数器，上升沿触发脉冲
	memset(&m_counter, 0, sizeof(m_counter));

	if (m_viewBoard)
		m_viewBoard->Erase();


	if (m_channel < 0 || !m_card || m_counter.start)
		return -1;
	//double val= 1;
	//m_card->WriteDO(1, val);
	//Sleep(10);
	//val = 0;
	m_counter.start = 2;
	//m_card->WriteDO(1, val);
	return 0;
}
/* calculate the Period and Freq array value， fill the Period value into the Period register during the TImer interrupt.

* calculate the acceleraTIon procedure ， a totally 1000 elements array.

* parameter fre［］： point to the array that keeps the freq value.

* period［］： point to the array that keeps the TImer period value.

* len： the procedure of acceleraTIon length.it is best thing to set the float number， some compile software maybe transfer error if set it as a int

* fre_max： maximum speed， frequency vale.

* fre_min： start minimum speed， frequency vale. mind ： 10000000/65535 = 152， so fre_min can‘t less than 152.

* flexible： flexible value. adjust the S curves

*/

void CalculateSModelLine(float fre[], unsigned short period[], float len, float fre_max, float fre_min, float flexible)
{

	int i = 0;

	float deno;

	float melo;

	float delt = fre_max - fre_min;

	for (; i < len; i++)
	{

		melo = flexible * (i - len / 2) / (len / 2);

		deno = 1.0 / (1 + expf(-melo)); //expf is a library function of exponential（e）

		fre[i] = delt * deno + fre_min;

		period[i] = (unsigned short)(10000000.0 / fre[i]); // 10000000 is the timer driver frequency

	}

	return;

}
int CDiIntCounterSnap::TestS()
{
#define LEN 600
	float fre[LEN] = { 0 };
	unsigned short period[LEN] = { 0 };
	float fre_max = 500;
	float fre_min = 152;
	float flexible = 8;

	CalculateSModelLine(fre, period, LEN, fre_max, fre_min, flexible);

	POINT point;
	m_viewBoard->Erase();
	for (int i = 0; i < LEN; i++)
	{
		point.x = i;
		point.y = fre[i];
		m_viewBoard->DrawPoint(point, RGB(255, 255, 255));

		point.x = i;
		point.y = period[i] / 100;
		m_viewBoard->DrawPoint(point, RGB(255, 255, 0));
	}
	m_viewBoard->SetOutStr(_T("0"), 0, 0);
	m_viewBoard->SetOutStr(_T("100"), 0, 100);
	m_viewBoard->SetOutStr(_T("200"), 0, 200);
	m_viewBoard->SetOutStr(_T("300"), 0, 300);
	m_viewBoard->SetOutStr(_T("400"), 0, 400);
	m_viewBoard->Invalidate();
	return 0;
}

int CDiIntCounterSnap::CheckStart()
{
	return m_counter.start;

}
#define LASER_CNT_START_INDEX 0
int CDiIntCounterSnap::LaserFit()
{
	if (m_channel < 0 || !m_card || m_counter.start || m_counter.index[0] > LASER_SIN_NUM+1)
		return -1;

	memset(m_counter.tmp_counter, 0, sizeof(m_counter.tmp_counter));
#ifndef  __DEBUG__
	for (int i = m_counter.index[0] - 2; i > 0; i--) {
		for (int j = 0; j < SIN_CNT_NUM; j++) {
			/*if(m_counter.counter[LASER_CNT_START_INDEX+j][i] >=m_counter.counter[LASER_CNT_START_INDEX+j][i+1])
				m_counter.tmp_counter[LASER_CNT_START_INDEX+j][i] = m_counter.counter[LASER_CNT_START_INDEX+j][i] - m_counter.counter[LASER_CNT_START_INDEX+j][i+1];
			else
				m_counter.tmp_counter[LASER_CNT_START_INDEX+j][i] = 65536 - m_counter.counter[LASER_CNT_START_INDEX+j][i+1]+m_counter.counter[LASER_CNT_START_INDEX+j][i];
			*/
			double dec = m_counter.counter[LASER_CNT_START_INDEX+j][i];//fabs(m_counter.counter[LASER_CNT_START_INDEX+j][i] -m_counter.counter[LASER_CNT_START_INDEX+j][i+1]);
			m_counter.tmp_counter[LASER_CNT_START_INDEX+j][i] = dec;//dec > 32768 ? 65536 - dec : dec;
		}
	}
	//for (int j = 0; j < SIN_CNT_NUM; j++) {
	//    m_counter.tmp_counter[LASER_CNT_START_INDEX+j][0] = 65535-m_counter.counter[LASER_CNT_START_INDEX+j][0];
	//  }
	double ignore_val = -1;
	for (int i = m_counter.index[0] - 1; i >= 0; i--) {
		//for (int j = 0; j < SIN_CNT_NUM; j++) {
		//  m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
		//}
		double m1 = (m_counter.tmp_counter[1][i] + m_counter.tmp_counter[0][i]) / 2;
		double m2 = (m_counter.tmp_counter[3][i] + m_counter.tmp_counter[2][i]) / 2;
		
		double d1 = (m_counter.tmp_counter[1][i] - m_counter.tmp_counter[0][i]);
		double d2 = (m_counter.tmp_counter[3][i] - m_counter.tmp_counter[2][i]);

		if(fabs(m1 -m2)>min(m1,m2)/2 || d1 <= 0 || d2 <= 0||m1>1000||m2>1000)
			m_counter.fit[0][i] = ignore_val;
		else
			m_counter.fit[0][i] = (m_counter.tmp_counter[1][i] + m_counter.tmp_counter[0][i] + m_counter.tmp_counter[3][i] + m_counter.tmp_counter[2][i]) / 4;
	}
#endif


	int sin_num = m_counter.index[0]-1;
#ifdef __DEBUG__
	for (int i = 0; i < sin_num; i++)
	{
		//x laser 
		//m_counter.counter[0][i] = 100 * sin(i * (2 * PI) / sin_num *4+ PI) + 300;
		if(m_counter.fit[0][i] != ignore_val)
			m_counter.fit[0][i] = 100 * sin(i * (2 * PI) / (sin_num - 20) + PI) + 300;
	}
#endif // __DEBUG__
	//TODO：调试拟合
	EfgAlg alg;
	struct tagSinParam param;
	alg.FitSinByLeastSquares(m_counter.fit[0], m_counter.index[0]-1, m_counter.fit[1], param, &ignore_val);
	//efg的激光图是上下颠倒的
	m_efgio->m_resultParam.measure.A=param.A;
	m_efgio->m_resultParam.measure.w=param.w;
	m_efgio->m_resultParam.measure.t=param.t;
	m_efgio->m_resultParam.measure.k=param.k;
	m_efgio->m_resultParam.measure.l_num=sin_num;


	//if(param.A > 100)
	//	param.A = param.A;

	if (m_viewBoard) {
		m_viewBoard->Erase();
		m_viewBoard->DrawLaserSin();

		CString str;
		str.Format(L"yi = %.2f * sin (%.2f * xi + %.2f) + %.2f\r\n num:%d", param.A, param.w, param.t, param.k,sin_num);
		m_viewBoard->SetOutStr(str, 10,500);//m_viewBoard->SetOutStr(str);
		double AMPL = atan(param.A * m_efgio->m_configParam.laser.factor_a / m_efgio->m_configParam.laser.factor_l) / DPI;
		double PHASE = - param.t / param.w * /*2 * PI*/360 / ( sin_num);  // 零位到sin的起始位置的角度，注意原始efg的图是上下颠倒的
		str.Format(L"AMPL = %.2f °%d \"  PHASE = %.2f ° ",AMPL ,DEG_TO_USER(AMPL), PHASE);
		m_viewBoard->SetOutStr(str, 10,600);
		///TODO：调试拟合

		POINT point;
		double time_x = ((double)WND_WIDTH) / (sin_num);
#ifdef __DEBUG__
		for (int i = 0; i < sin_num; i++)
		{
			point.x = i << 2;
			point.y = m_counter.fit[0][i];
			m_viewBoard->DrawPoint(point, RGB(255, 0, 0));
		}
#endif // __DEBUG__

		for (int i = 0; i < sin_num; i++)
		{
			point.x = i * time_x+1;
			point.y = m_counter.fit[1][i];
			m_viewBoard->DrawPoint(point);
			point.x += time_x/2;
			m_viewBoard->DrawPoint(point);
		}
		m_viewBoard->Invalidate();
	}
	return 0;
}

int CDiIntCounterSnap::LaserFit(int n)
{
	if (m_channel < 0 || !m_card || m_counter.start || m_counter.index[0] > LASER_SIN_NUM+1)
		return -1;

	memset(m_counter.tmp_counter, 0, sizeof(m_counter.tmp_counter));
#ifndef  __DEBUG__
	for (int i = m_counter.index[0] - 2; i > 0; i--) {
		for (int j = 0; j < SIN_CNT_NUM; j++) {
			/*if(m_counter.counter[LASER_CNT_START_INDEX+j][i] >=m_counter.counter[LASER_CNT_START_INDEX+j][i+1])
				m_counter.tmp_counter[LASER_CNT_START_INDEX+j][i] = m_counter.counter[LASER_CNT_START_INDEX+j][i] - m_counter.counter[LASER_CNT_START_INDEX+j][i+1];
			else
				m_counter.tmp_counter[LASER_CNT_START_INDEX+j][i] = 65536 - m_counter.counter[LASER_CNT_START_INDEX+j][i+1]+m_counter.counter[LASER_CNT_START_INDEX+j][i];
			*/
			double dec = m_counter.counter[LASER_CNT_START_INDEX+j][i];//fabs(m_counter.counter[LASER_CNT_START_INDEX+j][i] -m_counter.counter[LASER_CNT_START_INDEX+j][i+1]);
			m_counter.tmp_counter[LASER_CNT_START_INDEX+j][i] = dec;//dec > 32768 ? 65536 - dec : dec;
		}
	}
	//for (int j = 0; j < SIN_CNT_NUM; j++) {
	//    m_counter.tmp_counter[LASER_CNT_START_INDEX+j][0] = 65535-m_counter.counter[LASER_CNT_START_INDEX+j][0];
	//  }
	double ignore_val = -1;
	for (int i = m_counter.index[0] - 1; i >= 0; i--) {
		//for (int j = 0; j < SIN_CNT_NUM; j++) {
		//  m_counter.counter[j][i] -= m_counter.counter[j][i - 1];
		//}
		double m1 = (m_counter.tmp_counter[1][i] + m_counter.tmp_counter[0][i]) / 2;
		double m2 = (m_counter.tmp_counter[3][i] + m_counter.tmp_counter[2][i]) / 2;
		
		double d1 = (m_counter.tmp_counter[1][i] - m_counter.tmp_counter[0][i]);
		double d2 = (m_counter.tmp_counter[3][i] - m_counter.tmp_counter[2][i]);

		if(fabs(m1 -m2)>min(m1,m2)/2 || d1 <= 0 || d2 <= 0||m1>1000||m2>1000)
			m_counter.fit[0][i] = ignore_val;
		else
			m_counter.fit[0][i] = (m_counter.tmp_counter[1][i] + m_counter.tmp_counter[0][i] + m_counter.tmp_counter[3][i] + m_counter.tmp_counter[2][i]) / 4;
	}
#endif


	int sin_num = m_counter.index[0]-1;
#ifdef __DEBUG__
	for (int i = 0; i < sin_num; i++)
	{
		//x laser 
		//m_counter.counter[0][i] = 100 * sin(i * (2 * PI) / sin_num *4+ PI) + 300;
		if(m_counter.fit[0][i] != ignore_val)
			m_counter.fit[0][i] = 100 * sin(i * (2 * PI) / (sin_num - 20) + PI) + 300;
	}
#endif // __DEBUG__
	//TODO：调试拟合
	EfgAlg alg;
	struct tagSinParam param;
	alg.FitSinByLeastSquares(m_counter.fit[0], m_counter.index[0]-1, m_counter.fit[1], param, &ignore_val);

	//关键参数平均值
	static double A=0, t=0,w = 0;
	if(n == 1 )
	{
		A=param.A;
		t=param.t;
		w=param.w;
		
	}
	else
	{//avg = (avg + xi / (n - 1)) / (n / (n - 1));//求平均值
		A= (A * (n - 1.0) + param.A ) / n ;
		t= (t * (n - 1.0) + param.t ) / n ;
		w= (w * (n - 1.0) + param.w ) / n ;
	}
	//更新
	param.A=A;
	param.t=t;
	param.w=w;


	//efg的激光图是上下颠倒的
	m_efgio->m_resultParam.measure.A=param.A;
	m_efgio->m_resultParam.measure.w=param.w;
	m_efgio->m_resultParam.measure.t=param.t;
	m_efgio->m_resultParam.measure.k=param.k;
	m_efgio->m_resultParam.measure.l_num=sin_num;


	//if(param.A > 100)
	//	param.A = param.A;

	if (m_viewBoard) {
		m_viewBoard->Erase();
		m_viewBoard->DrawLaserSin();

		CString str;
		str.Format(L"yi = %.2f * sin (%.2f * xi + %.2f) + %.2f\r\n num:%d", param.A, param.w, param.t, param.k,sin_num);
		m_viewBoard->SetOutStr(str, 10,500);//m_viewBoard->SetOutStr(str);
		double AMPL = atan(param.A * m_efgio->m_configParam.laser.factor_a / m_efgio->m_configParam.laser.factor_l) / DPI;
		double PHASE = - param.t / param.w * /*2 * PI*/360 / ( sin_num);  // 零位到sin的起始位置的角度，注意原始efg的图是上下颠倒的
		str.Format(L"AMPL = %.2f °%d \"  PHASE = %.2f ° ",AMPL ,DEG_TO_USER(AMPL), PHASE);
		m_viewBoard->SetOutStr(str, 10,600);
		///TODO：调试拟合

		POINT point;
		double time_x = ((double)WND_WIDTH) / (sin_num);
#ifdef __DEBUG__
		for (int i = 0; i < sin_num; i++)
		{
			point.x = i << 2;
			point.y = m_counter.fit[0][i];
			m_viewBoard->DrawPoint(point, RGB(255, 0, 0));
		}
#endif // __DEBUG__

		for (int i = 0; i < sin_num; i++)
		{
			point.x = i * time_x+1;
			point.y = m_counter.fit[1][i];
			m_viewBoard->DrawPoint(point);
			point.x += time_x/2;
			m_viewBoard->DrawPoint(point);
		}
		m_viewBoard->Invalidate();
	}
	return 0;
}

#define XRAY_CNT_START_INDEX 4
int CDiIntCounterSnap::XrayFit()
{




#ifdef USE_EFGV1
	if (!m_card || m_counter.start || m_counter.index[1] > XRAY_ONESHOT_NUM|| m_counter.index[1] <0)
		return -1;
	memcpy(m_counter.tmp_counter[XRAY_CNT_START_INDEX], m_counter.counter[XRAY_CNT_START_INDEX], sizeof(double)*m_counter.index[1]);
	memcpy(m_counter.fit[0], m_counter.counter[XRAY_CNT_START_INDEX], sizeof(double)*m_counter.index[1]);
	int sin_num = (m_counter.index[1]-1)/*/SMALL*/;
#else
#ifndef  __DEBUG__
	if (!m_card || m_counter.start || m_counter.index[1] > XRAY_ONESHOT_NUM+1|| m_counter.index[1] <0)
		return -1;
	memset(m_counter.tmp_counter, 0, sizeof(m_counter.tmp_counter));
	memset(m_counter.fit, 0, sizeof(m_counter.fit));

	//计数器值的差，即每个脉冲下的计数值
	//for (int i = m_counter.index[1] - 2; i >= 0; i--) {
	//	for (int j = 0; j < XRAY_CNT_NUM; j++) {
	//		if(m_counter.counter[XRAY_CNT_START_INDEX+j][i] <= m_counter.counter[XRAY_CNT_START_INDEX+j][i+1])
	//			m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][i] = m_counter.counter[XRAY_CNT_START_INDEX+j][i+1] - m_counter.counter[XRAY_CNT_START_INDEX+j][i];
	//		else
	//
	//			m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][i] = 65536- m_counter.counter[XRAY_CNT_START_INDEX+j][i]+m_counter.counter[XRAY_CNT_START_INDEX+j][i+1] ;
	//	}
	//	}
	for (int i = m_counter.index[1] - 2; i >= 0; i--) {
		for (int j = 0; j < XRAY_CNT_NUM; j++) {
			//if(m_counter.counter[XRAY_CNT_START_INDEX+j][i] >= m_counter.counter[XRAY_CNT_START_INDEX+j][i+1])
			//	m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][i] = m_counter.counter[XRAY_CNT_START_INDEX+j][i] - m_counter.counter[XRAY_CNT_START_INDEX+j][i+1];
			//else
			//	m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][i] = 65536- m_counter.counter[XRAY_CNT_START_INDEX+j][i+1]+m_counter.counter[XRAY_CNT_START_INDEX+j][i] ;
			//
			//由于这个计数卡，本来是递减的，结果偶尔会出来一次+1，原因不明，考虑到实际计数值肯定＜65536的一半，故如下
			double dec = fabs(m_counter.counter[XRAY_CNT_START_INDEX+j][i] - m_counter.counter[XRAY_CNT_START_INDEX+j][i+1]);
			m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][i] = dec > 32768 ? 65536 - dec : dec;
			//if(m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][i]>300)
			//	continue;
		}
	}
	//for (int j = 0; j < XRAY_CNT_NUM; j++) {
	//    m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][0] = 65535-m_counter.counter[XRAY_CNT_START_INDEX+j][0];
	//  }
	//使用中断信号的话，一个是0计数一个是有计数，直接相加即可,相加不可，应该时本应0计数下的少量计数要加到本计数器后

	//比大小，确认哪个是真实的计数值
	double tmp_sum[XRAY_CNT_NUM] = { 0,0 };

	for (int i = 0; i < m_counter.index[1] - 1; i+=2)
	{
		for (int j = 0; j < XRAY_CNT_NUM; j++)
		{
			tmp_sum[j] += m_counter.tmp_counter[XRAY_CNT_START_INDEX + j][i];
		}
	}

	int i, j;
	//if (m_counter.tmp_counter[XRAY_CNT_START_INDEX][0] > m_counter.tmp_counter[XRAY_CNT_START_INDEX+1][0])
	if (tmp_sum[0] > tmp_sum[1])
	{
		i = 1;
		j = 0;
	}
	else
	{
		i = 0;
		j = 1;
	}


	for (; i <  m_counter.index[1]-2; i+=2)
	{
		m_counter.tmp_counter[XRAY_CNT_START_INDEX][i+1] += m_counter.tmp_counter[XRAY_CNT_START_INDEX][i];
		m_counter.tmp_counter[XRAY_CNT_START_INDEX][i] = 0;
	}
	for (; j <  m_counter.index[1]-2; j+=2)
	{
		m_counter.tmp_counter[XRAY_CNT_START_INDEX+1][j+1] += m_counter.tmp_counter[XRAY_CNT_START_INDEX+1][j];
		m_counter.tmp_counter[XRAY_CNT_START_INDEX+1][j] = 0;
	}

	for (int i = 0; i <  m_counter.index[1]-1; i++)
	{
		for (int j = 0; j < XRAY_CNT_NUM; j++) {
			m_counter.fit[0][i] += m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][i];
		}
	}
	//yasuo
	//for (int i = 0; i <  m_counter.index[1]-1; i++)
	//{
	//	for (int j = 0; j < XRAY_CNT_NUM; j++) {
	//		m_counter.fit[0][i/*/SMALL*/] += m_counter.tmp_counter[XRAY_CNT_START_INDEX+j][i];
	//	}
	//}
#endif

	int sin_num = (m_counter.index[1]-1)/*/SMALL*/;
	/*
	for (int i = 0; i <  sin_num; i++)
	{
	m_counter.fit[0][i]/=SMALL/2;
	}
	*/
#ifdef __DEBUG__
	for (int i = 0; i < sin_num; i++)
	{
		//x laser 
		//m_counter.counter[0][i] = 100 * sin(i * (2 * PI) / sin_num *4+ PI) + 300;
		m_counter.fit[0][i] = 100 * sin(i * (2 * PI) / sin_num * 2 + PI) + 300;
	}
#endif // __DEBUG__
#endif
	//TODO：调试拟合
	EfgAlg alg;
#if 1 // 用于记录这些点
	CFile m_file;
	m_file.Open(_T("xraypoint.txt"),CFile::modeCreate | CFile::modeWrite | CFile::shareDenyRead);
	m_file.Seek(0,CFile::end);
	CString str;
	double avg = 0, std = 0, std2 = 0;
	for (int i = 0; i < sin_num; i++)
	{
		str.Format(_T("%.3f\r\n"),m_counter.fit[0][i]);
		m_file.Write(CStringA(str).GetBuffer(),CStringA(str).GetLength());

		alg.SortAvgStd(
			i+1,
			m_counter.fit[0][i],
			avg,
			std,
			std2
			);
	}

	m_file.Flush();
	m_file.Close();
#endif
	// struct tagSinParam param;
	// alg.FitSinBySubstitution(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[1], param);
	// alg.FitSinByLeastSquares(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[2], param);

	//alg.Correct(m_counter.fit[0], sin_num,3);//平滑
	if(sin_num <=0)
		return -1;
	alg.KLM(m_counter.fit[0], sin_num);
	
	alg.Smooth(m_counter.fit[0], sin_num,m_efgio->m_configParam.xray.factor_w,m_efgio->m_configParam.xray.factor_h);//平滑
	alg.ExtractSpike(m_counter.fit[0], sin_num,// 100, 3, -1);
		m_efgio->m_configParam.xray.threshold,
		m_efgio->m_configParam.xray.confirmNum,
		m_efgio->m_configParam.xray.ignore);

	//转盘脉冲数
	m_efgio->m_resultParam.measure.pluse_num = sin_num;//m_counter.index[1] - 1;
	m_efgio->m_resultParam.measure.pluse_cnt++;

	if(m_efgio->m_resultParam.measure.pluse_num > m_efgio->m_resultParam.measure.max_pluse_num)
		m_efgio->m_resultParam.measure.max_pluse_num = m_efgio->m_resultParam.measure.pluse_num;
	if(m_efgio->m_resultParam.measure.pluse_num < m_efgio->m_resultParam.measure.min_pluse_num)
		m_efgio->m_resultParam.measure.min_pluse_num = m_efgio->m_resultParam.measure.pluse_num;

	alg.GetD1D2DM(
		m_efgio->m_resultParam.measure.D1,
		m_efgio->m_resultParam.measure.D2,
		m_efgio->m_resultParam.measure.DM,
		m_efgio->m_resultParam.measure.R1,
		m_efgio->m_resultParam.measure.pluse_num/*/SMALL*/
		);

	alg.CalcDegree0(
		m_efgio->m_resultParam.measure.D1,
		m_efgio->m_resultParam.measure.D2,
		m_efgio->m_resultParam.measure.DM,
		m_efgio->m_resultParam.measure.cur_theta0,
		m_efgio->m_resultParam.measure.cur_phi0,
		m_efgio->m_resultParam.measure.u_g
		);


	if (m_viewBoard) {
		m_viewBoard->Erase();
		m_viewBoard->DrawXRayOneShot();
		CString str;

		str.Format(L"静态散差:%.3f 静态平均:%.3f", 
			std,
			avg);

		m_viewBoard->SetOutStr(str,10,470);

		str.Format(L"theta0:%.3f phi0:%.3f ug:%.3f", 
			m_efgio->m_resultParam.measure.cur_theta0,
			m_efgio->m_resultParam.measure.cur_phi0,
			m_efgio->m_resultParam.measure.u_g);

		m_viewBoard->SetOutStr(str,10,510);

		str.Format(L"num:%.0f D1:%.3f D2:%.3f DM:%.3f R1:%.3f", 
			m_efgio->m_resultParam.measure.pluse_num,//m_counter.index[1]-1,
			m_efgio->m_resultParam.measure.D1,
			m_efgio->m_resultParam.measure.D2,
			m_efgio->m_resultParam.measure.DM,
			m_efgio->m_resultParam.measure.R1);

		m_viewBoard->SetOutStr(str,10,550);

		str.Format(L"最小转盘脉冲:%.0f 最大转盘脉冲:%.0f 次数:%d", 
			m_efgio->m_resultParam.measure.min_pluse_num,
			m_efgio->m_resultParam.measure.max_pluse_num,
			m_efgio->m_resultParam.measure.pluse_cnt);

		m_viewBoard->SetOutStr(str,10,590);

		//  m_viewBoard->SetOutStr(L"");
		//#ifdef  __DEBUG__
		//      for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
		//      {
		//        POINT point;
		//        point.x = i;
		//        point.y = m_counter.fit[0][i];
		//        m_viewBoard->DrawPoint(point);
		//      }
		//#endif
		///TODO：调试拟合
		double times = 1.0*(sin_num/*m_counter.index[1]-1*/)/WND_WIDTH;
		//if(times<1)
		//	times=1;
		int num = alg.GetSpikesNumber();
		if (num < 10)
		{
			POINT point;
			for (int i = 0; i < num; i++)
			{
				SPIKE spike;
				alg.GetSpike(i, spike);
				point = spike.p;
				point.x/=times;
				point.y*=XRAY_Y_TIMES;
				m_viewBoard->DrawCircle(point, 10);
				CString str;
				str.Format(_T("No:%d X:%d Y:%d W:%d"), i, spike.p.x, spike.p.y, spike.w);
				m_viewBoard->SetOutStr(str, point.x, 250+50*i);
			}
		}

		for (int i = 0; i < sin_num/*(m_counter.index[1]-1)*/; i+=1/*3*/)
		{
			POINT point;
			point.x=i/times;
			point.y=m_efgio->m_configParam.xray.threshold*XRAY_Y_TIMES;
			m_viewBoard->DrawPoint(point,RGB(100,100,0));

		}



		m_viewBoard->Invalidate();
	}

	//StopDiInt();
	return 0;
}
int CDiIntCounterSnap::XrayFit(int n)
{




#ifdef USE_EFGV1
	if (!m_card || m_counter.start || m_counter.index[1] > XRAY_ONESHOT_NUM|| m_counter.index[1] <0)
		return -1;
	memcpy(m_counter.tmp_counter[XRAY_CNT_START_INDEX], m_counter.counter[XRAY_CNT_START_INDEX], sizeof(double)*m_counter.index[1]);
	memcpy(m_counter.fit[0], m_counter.counter[XRAY_CNT_START_INDEX], sizeof(double)*m_counter.index[1]);
	int sin_num = (m_counter.index[1]-1)/*/SMALL*/;

#endif
	//TODO：调试拟合
	EfgAlg alg;
#if 1 // 用于记录这些点
	CFile m_file;
	m_file.Open(_T("xraypoint.txt"),CFile::modeCreate | CFile::modeWrite | CFile::shareDenyRead);
	m_file.Seek(0,CFile::end);
	CString str;
	double avg = 0, std = 0, std2 = 0;
	for (int i = 0; i < sin_num; i++)
	{
		str.Format(_T("%.3f\r\n"),m_counter.fit[0][i]);
		m_file.Write(CStringA(str).GetBuffer(),CStringA(str).GetLength());

		alg.SortAvgStd(
			i+1,
			m_counter.fit[0][i],
			avg,
			std,
			std2
			);
	}

	m_file.Flush();
	m_file.Close();
#endif
	// struct tagSinParam param;
	// alg.FitSinBySubstitution(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[1], param);
	// alg.FitSinByLeastSquares(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[2], param);

	//alg.Correct(m_counter.fit[0], sin_num,3);//平滑
	if(sin_num <=0)
		return -1;
	alg.KLM(m_counter.fit[0], sin_num);

	//动态确定阀值
	if(n == 1){
	for (int i = 0; i < sin_num; i++)
	{
		alg.SortAvgStd(
			i+1,
			m_counter.fit[0][i],
			avg,
			std,
			std2
			);
	}
	if(m_efgio->m_configParam.xray.threshold < avg+0.5)
		m_efgio->m_configParam.xray.threshold = avg/**1.5*/+0.5;
	}

	alg.Smooth(m_counter.fit[0], sin_num,m_efgio->m_configParam.xray.factor_w,m_efgio->m_configParam.xray.factor_h);//平滑
	alg.ExtractSpike(m_counter.fit[0], sin_num,// 100, 3, -1);
		m_efgio->m_configParam.xray.threshold,
		m_efgio->m_configParam.xray.confirmNum,
		m_efgio->m_configParam.xray.ignore);

	//算关键参数的平均值并更新
	SPIKE cur;
	int spike_num = 0;
	static double x[4] = {0};
	
	spike_num = alg.GetSpikesNumber();
				
	if(spike_num > 12)//尖峰数量异常
		return -1;

	while(spike_num>4)
	{
		int min = 0;
		int min_val=10000000;

		for(int i = 0; i < spike_num; i++)
		{
			alg.GetSpike(i, cur);
			if(min == i || min_val > cur.p.y)
			{
				min_val = cur.p.y;
				min = i;
			}

			if(cur.p.y > 500)//值异常
				return -1;
		}
		alg.DelSpike(min);
	    spike_num = alg.GetSpikesNumber();
	}

	////算尖峰的平均
	//if(spike_num==4)
	//{

	//	for(int i = 0; i < 4; i++)
	//	{
	//		alg.GetSpike(i, cur);
	//		if(n == 1 )
	//			x[i] = cur.x;
	//		else
	//			x[i] = (x[i]*(n - 1.0) + cur.x ) / n;
	//			//x[i] += (x[i] + cur.x / (n - 1.0)) / (n / (n - 1.0));
	//	}
	////更新
	//alg.UpdateSpikeX(x);
	//}

	//
	//转盘脉冲数
	m_efgio->m_resultParam.measure.pluse_num = sin_num;//m_counter.index[1] - 1;
	m_efgio->m_resultParam.measure.pluse_cnt++;

	if(m_efgio->m_resultParam.measure.pluse_num > m_efgio->m_resultParam.measure.max_pluse_num)
		m_efgio->m_resultParam.measure.max_pluse_num = m_efgio->m_resultParam.measure.pluse_num;
	if(m_efgio->m_resultParam.measure.pluse_num < m_efgio->m_resultParam.measure.min_pluse_num)
		m_efgio->m_resultParam.measure.min_pluse_num = m_efgio->m_resultParam.measure.pluse_num;

	alg.GetD1D2DM(
		m_efgio->m_resultParam.measure.D1,
		m_efgio->m_resultParam.measure.D2,
		m_efgio->m_resultParam.measure.DM,
		m_efgio->m_resultParam.measure.R1,
		m_efgio->m_resultParam.measure.pluse_num/*/SMALL*/
		);
	//算D1D2DM平均值并更新
	static double D1=0, D2=0, DM=0, R1=0;
	
	
	if(n == 1 ){
		D1 = m_efgio->m_resultParam.measure.D1;
		D2 = m_efgio->m_resultParam.measure.D2;
		DM = m_efgio->m_resultParam.measure.DM;
		R1 = m_efgio->m_resultParam.measure.R1;
	}else{
		D1 = (D1*(n - 1.0) + m_efgio->m_resultParam.measure.D1 ) / n;
		D2 = (D2*(n - 1.0) + m_efgio->m_resultParam.measure.D2 ) / n;
		DM = (DM*(n - 1.0) + m_efgio->m_resultParam.measure.DM ) / n;
		R1 = (R1*(n - 1.0) + m_efgio->m_resultParam.measure.R1 ) / n;
	}
	//更新
	m_efgio->m_resultParam.measure.D1 = D1;
	m_efgio->m_resultParam.measure.D2 = D2;
	m_efgio->m_resultParam.measure.DM = DM;
	m_efgio->m_resultParam.measure.R1 = R1;
	///////

	alg.CalcDegree0(
		m_efgio->m_resultParam.measure.D1,
		m_efgio->m_resultParam.measure.D2,
		m_efgio->m_resultParam.measure.DM,
		m_efgio->m_resultParam.measure.cur_theta0,
		m_efgio->m_resultParam.measure.cur_phi0,
		m_efgio->m_resultParam.measure.u_g
		);


	if (m_viewBoard) {
		m_viewBoard->Erase();
		m_viewBoard->DrawXRayOneShot();
		CString str;

		str.Format(L"静态散差:%.3f 静态平均:%.3f", 
			std,
			avg);

		m_viewBoard->SetOutStr(str,10,470);

		str.Format(L"theta0:%.3f phi0:%.3f ug:%.3f", 
			m_efgio->m_resultParam.measure.cur_theta0,
			m_efgio->m_resultParam.measure.cur_phi0,
			m_efgio->m_resultParam.measure.u_g);

		m_viewBoard->SetOutStr(str,10,510);

		str.Format(L"num:%.0f D1:%.3f D2:%.3f DM:%.3f R1:%.3f", 
			m_efgio->m_resultParam.measure.pluse_num,//m_counter.index[1]-1,
			m_efgio->m_resultParam.measure.D1,
			m_efgio->m_resultParam.measure.D2,
			m_efgio->m_resultParam.measure.DM,
			m_efgio->m_resultParam.measure.R1);

		m_viewBoard->SetOutStr(str,10,550);

		str.Format(L"最小转盘脉冲:%.0f 最大转盘脉冲:%.0f 次数:%d", 
			m_efgio->m_resultParam.measure.min_pluse_num,
			m_efgio->m_resultParam.measure.max_pluse_num,
			m_efgio->m_resultParam.measure.pluse_cnt);

		m_viewBoard->SetOutStr(str,10,590);

		//  m_viewBoard->SetOutStr(L"");
		//#ifdef  __DEBUG__
		//      for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
		//      {
		//        POINT point;
		//        point.x = i;
		//        point.y = m_counter.fit[0][i];
		//        m_viewBoard->DrawPoint(point);
		//      }
		//#endif
		///TODO：调试拟合
		double times = 1.0*(sin_num/*m_counter.index[1]-1*/)/WND_WIDTH;
		//if(times<1)
		//	times=1;
		int num = alg.GetSpikesNumber();
		if (num < 10)
		{
			POINT point;
			for (int i = 0; i < num; i++)
			{
				SPIKE spike;
				alg.GetSpike(i, spike);
				point = spike.p;
				point.x/=times;
				point.y*=XRAY_Y_TIMES;
				m_viewBoard->DrawCircle(point, 10);
				CString str;
				str.Format(_T("No:%d X:%d Y:%d W:%d"), i, spike.p.x, spike.p.y, spike.w);
				m_viewBoard->SetOutStr(str, point.x, 250+50*i);
			}
		}

		for (int i = 0; i < sin_num/*(m_counter.index[1]-1)*/; i+=1/*3*/)
		{
			POINT point;
			point.x=i/times;
			point.y=m_efgio->m_configParam.xray.threshold*XRAY_Y_TIMES;
			m_viewBoard->DrawPoint(point,RGB(100,100,0));

		}



		m_viewBoard->Invalidate();
	}

	//StopDiInt();
	return 0;
}

int CDiIntCounterSnap::XrayFitTest(int n)
{




#ifdef USE_EFGV1
  //if (!m_card || m_counter.start || m_counter.index[1] > XRAY_ONESHOT_NUM || m_counter.index[1] < 0)
  //  return -1;
  //memcpy(m_counter.tmp_counter[XRAY_CNT_START_INDEX], m_counter.counter[XRAY_CNT_START_INDEX], sizeof(double) * m_counter.index[1]);
  //memcpy(m_counter.fit[0], m_counter.counter[XRAY_CNT_START_INDEX], sizeof(double) * m_counter.index[1]);
  int sin_num = 0;

#endif
  //TODO：调试拟合
  EfgAlg alg;
#if 1 // 用于记录这些点
  CStdioFile m_file;
  m_file.Open(_T("xraypoint2.txt"), CFile::modeCreate | CFile::modeRead| CFile::modeNoTruncate);
  m_file.Seek(0, CFile::begin);
  CString str;
  BOOL ret;
  double avg = 0, std = 0, std2 = 0;
  do
  {/*
    str.Format(_T("%.3f\r\n"), m_counter.fit[0][i]);
    m_file.Write(CStringA(str).GetBuffer(), CStringA(str).GetLength());*/
    ret = m_file.ReadString(str);
    if(ret)
    m_counter.fit[0][sin_num++] = _wtof(str.GetBuffer());
    str.ReleaseBuffer();/*
    alg.SortAvgStd(
      i + 1,
      m_counter.fit[0][i],
      avg,
      std,
      std2
    );*/
  } while (ret);
  m_counter.index[1] = sin_num + 1;
  m_file.Flush();
  m_file.Close();

  if (m_viewBoard) {
    m_viewBoard->Erase();
    m_viewBoard->DrawXRayOneShot();
  }
#endif
  // struct tagSinParam param;
  // alg.FitSinBySubstitution(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[1], param);
  // alg.FitSinByLeastSquares(m_counter.counter[0], XRAY_ONESHOT_NUM, m_counter.counter[2], param);

  //alg.Correct(m_counter.fit[0], sin_num,3);//平滑
  if (sin_num <= 0)
    return -1;
  //alg.KLM(m_counter.fit[0], sin_num);

  ////动态确定阀值
  //if (n == 1) {
  //  for (int i = 0; i < sin_num; i++)
  //  {
  //    alg.SortAvgStd(
  //      i + 1,
  //      m_counter.fit[0][i],
  //      avg,
  //      std,
  //      std2
  //    );
  //  }
  //  if (m_efgio->m_configParam.xray.threshold < avg + 0.5)
  //    m_efgio->m_configParam.xray.threshold = avg/**1.5*/ + 0.5;
  //}

  //alg.Smooth(m_counter.fit[0], sin_num, m_efgio->m_configParam.xray.factor_w, m_efgio->m_configParam.xray.factor_h);//平滑
  //alg.ExtractSpike(m_counter.fit[0], sin_num,// 100, 3, -1);
  //  m_efgio->m_configParam.xray.threshold,
  //  m_efgio->m_configParam.xray.confirmNum,
  //  m_efgio->m_configParam.xray.ignore);

  alg.ExtractSpike(m_counter.fit[0], sin_num,// 100, 3, -1);
    15,
    30,
    25);

  //算关键参数的平均值并更新
  SPIKE cur;
  int spike_num = 0;
  static double x[4] = { 0 };

  spike_num = alg.GetSpikesNumber();

  if (spike_num > 12)//尖峰数量异常
    return -1;

  while (spike_num > 4)
  {
    int min = 0;
    int min_val = 10000000;

    for (int i = 0; i < spike_num; i++)
    {
      alg.GetSpike(i, cur);
      if (min == i || min_val > cur.p.y)
      {
        min_val = cur.p.y;
        min = i;
      }

      if (cur.p.y > 500)//值异常
        return -1;
    }
    alg.DelSpike(min);
    spike_num = alg.GetSpikesNumber();
  }

  //////算尖峰的平均
  ////if(spike_num==4)
  ////{

  ////	for(int i = 0; i < 4; i++)
  ////	{
  ////		alg.GetSpike(i, cur);
  ////		if(n == 1 )
  ////			x[i] = cur.x;
  ////		else
  ////			x[i] = (x[i]*(n - 1.0) + cur.x ) / n;
  ////			//x[i] += (x[i] + cur.x / (n - 1.0)) / (n / (n - 1.0));
  ////	}
  //////更新
  ////alg.UpdateSpikeX(x);
  ////}

  ////
  ////转盘脉冲数
  //m_efgio->m_resultParam.measure.pluse_num = sin_num;//m_counter.index[1] - 1;
  //m_efgio->m_resultParam.measure.pluse_cnt++;

  //if (m_efgio->m_resultParam.measure.pluse_num > m_efgio->m_resultParam.measure.max_pluse_num)
  //  m_efgio->m_resultParam.measure.max_pluse_num = m_efgio->m_resultParam.measure.pluse_num;
  //if (m_efgio->m_resultParam.measure.pluse_num < m_efgio->m_resultParam.measure.min_pluse_num)
  //  m_efgio->m_resultParam.measure.min_pluse_num = m_efgio->m_resultParam.measure.pluse_num;

  //alg.GetD1D2DM(
  //  m_efgio->m_resultParam.measure.D1,
  //  m_efgio->m_resultParam.measure.D2,
  //  m_efgio->m_resultParam.measure.DM,
  //  m_efgio->m_resultParam.measure.R1,
  //  m_efgio->m_resultParam.measure.pluse_num/*/SMALL*/
  //);
  ////算D1D2DM平均值并更新
  //static double D1 = 0, D2 = 0, DM = 0, R1 = 0;


  //if (n == 1) {
  //  D1 = m_efgio->m_resultParam.measure.D1;
  //  D2 = m_efgio->m_resultParam.measure.D2;
  //  DM = m_efgio->m_resultParam.measure.DM;
  //  R1 = m_efgio->m_resultParam.measure.R1;
  //}
  //else {
  //  D1 = (D1 * (n - 1.0) + m_efgio->m_resultParam.measure.D1) / n;
  //  D2 = (D2 * (n - 1.0) + m_efgio->m_resultParam.measure.D2) / n;
  //  DM = (DM * (n - 1.0) + m_efgio->m_resultParam.measure.DM) / n;
  //  R1 = (R1 * (n - 1.0) + m_efgio->m_resultParam.measure.R1) / n;
  //}
  ////更新
  //m_efgio->m_resultParam.measure.D1 = D1;
  //m_efgio->m_resultParam.measure.D2 = D2;
  //m_efgio->m_resultParam.measure.DM = DM;
  //m_efgio->m_resultParam.measure.R1 = R1;
  /////////

  //alg.CalcDegree0(
  //  m_efgio->m_resultParam.measure.D1,
  //  m_efgio->m_resultParam.measure.D2,
  //  m_efgio->m_resultParam.measure.DM,
  //  m_efgio->m_resultParam.measure.cur_theta0,
  //  m_efgio->m_resultParam.measure.cur_phi0,
  //  m_efgio->m_resultParam.measure.u_g
  //);


  if (m_viewBoard) {
   // m_viewBoard->Erase();
   // m_viewBoard->DrawXRayOneShot();
    CString str;

    //str.Format(L"静态散差:%.3f 静态平均:%.3f",
    //  std,
    //  avg);

    //m_viewBoard->SetOutStr(str, 10, 470);

    //str.Format(L"theta0:%.3f phi0:%.3f ug:%.3f",
    //  m_efgio->m_resultParam.measure.cur_theta0,
    //  m_efgio->m_resultParam.measure.cur_phi0,
    //  m_efgio->m_resultParam.measure.u_g);

    //m_viewBoard->SetOutStr(str, 10, 510);

    //str.Format(L"num:%.0f D1:%.3f D2:%.3f DM:%.3f R1:%.3f",
    //  m_efgio->m_resultParam.measure.pluse_num,//m_counter.index[1]-1,
    //  m_efgio->m_resultParam.measure.D1,
    //  m_efgio->m_resultParam.measure.D2,
    //  m_efgio->m_resultParam.measure.DM,
    //  m_efgio->m_resultParam.measure.R1);

    //m_viewBoard->SetOutStr(str, 10, 550);

    //str.Format(L"最小转盘脉冲:%.0f 最大转盘脉冲:%.0f 次数:%d",
    //  m_efgio->m_resultParam.measure.min_pluse_num,
    //  m_efgio->m_resultParam.measure.max_pluse_num,
    //  m_efgio->m_resultParam.measure.pluse_cnt);

    //m_viewBoard->SetOutStr(str, 10, 590);

    ////  m_viewBoard->SetOutStr(L"");
    ////#ifdef  __DEBUG__
    ////      for (int i = 0; i < XRAY_ONESHOT_NUM; i++)
    ////      {
    ////        POINT point;
    ////        point.x = i;
    ////        point.y = m_counter.fit[0][i];
    ////        m_viewBoard->DrawPoint(point);
    ////      }
    ////#endif
    ///TODO：调试拟合
    double times = 1.0 * (sin_num/*m_counter.index[1]-1*/) / WND_WIDTH;
    //if(times<1)
    //	times=1;
    int num = alg.GetSpikesNumber();
    if (num < 10)
    {
      POINT point;
      for (int i = 0; i < num; i++)
      {
        SPIKE spike;
        alg.GetSpike(i, spike);
        point = spike.p;
        point.x /= times;
        point.y *= XRAY_Y_TIMES;
        m_viewBoard->DrawCircle(point, 10);
        CString str;
        str.Format(_T("No:%d X:%d Y:%d W:%d"), i, spike.p.x, spike.p.y, spike.w);
        m_viewBoard->SetOutStr(str, point.x, 250 + 50 * i);
      }
    }

    for (int i = 0; i < sin_num/*(m_counter.index[1]-1)*/; i += 1/*3*/)
    {
      POINT point;
      point.x = i / times;
      point.y = m_counter.fit[0][i] * XRAY_Y_TIMES;
      m_viewBoard->DrawPoint(point, RGB(100, 100, 0));

    }



    m_viewBoard->Invalidate();
  }

  //StopDiInt();
  return 0;
}
////todo 不完善， ° ″，单位没统一
//int CDiIntCounterSnap::CalcResult()
//{
//  //CDlgRun1* dlg_run = ((CDlgRun1*)GetMainFrame()->m_userFrame->m_splitUser.GetPane(0, 0));
//  if (-1 == LaserFit())
//    return -1;
//  //if(m_viewBoard)
//  //  m_viewBoard->DrawToDC(dlg_run->m_laserdc, dlg_run->m_preview_rect);
//
//  if (-1 == XrayFit())
//    return -1;
//  //if (m_viewBoard)
//  //  m_viewBoard->DrawToDC(dlg_run->m_xraydc, dlg_run->m_preview_rect);
//
//  EfgAlg alg;
//
//  struct tagSinParam sin_param = {
//    m_efgio->m_resultParam.measure.A,
//    m_efgio->m_resultParam.measure.w,
//    m_efgio->m_resultParam.measure.t,
//    m_efgio->m_resultParam.measure.k
//  };
//  //计算光轴电轴
//  alg.CalcDegree1(
//    m_efgio->m_resultParam.measure.cur_laser0,
//    m_efgio->m_resultParam.measure.cur_phi0,
//    sin_param,
//    m_efgio->m_resultParam.measure.cur_laser1,
//    m_efgio->m_resultParam.measure.cur_phi1
//    );
//  //计算等效角
//  alg.CalcEquAngle(
//    m_efgio->m_resultParam.measure.cur_laser0,
//    m_efgio->m_resultParam.measure.cur_phi0,
//    USER_TO_DEG(m_efgio->m_configParam.user_config.measure.equivalent_angle.phi),
//    m_efgio->m_configParam.user_config.measure.equivalent_angle.factor/1000.0,
//    m_efgio->m_resultParam.measure.cur_equ
//  );
//
//  // 累计
//  m_efgio->m_resultParam.measure.num++;
//
//  // 累计平均值，散差
//  alg.SortAvgStd(
//    m_efgio->m_resultParam.measure.num,
//    m_efgio->m_resultParam.measure.cur_laser0,
//    m_efgio->m_resultParam.measure.avg_laser0,
//    m_efgio->m_resultParam.measure.std_laser0,
//    m_efgio->m_resultParam.measure.std2_laser0
//  );
//  alg.SortAvgStd(
//    m_efgio->m_resultParam.measure.num,
//    m_efgio->m_resultParam.measure.cur_phi0,
//    m_efgio->m_resultParam.measure.avg_phi0,
//    m_efgio->m_resultParam.measure.std_phi0,
//    m_efgio->m_resultParam.measure.std2_phi0
//  );
//  alg.SortAvgStd(
//    m_efgio->m_resultParam.measure.num,
//    m_efgio->m_resultParam.measure.cur_laser1,
//    m_efgio->m_resultParam.measure.avg_laser1,
//    m_efgio->m_resultParam.measure.std_laser1,
//    m_efgio->m_resultParam.measure.std2_laser1
//  );
//  alg.SortAvgStd(
//    m_efgio->m_resultParam.measure.num,
//    m_efgio->m_resultParam.measure.cur_phi1,
//    m_efgio->m_resultParam.measure.avg_phi1,
//    m_efgio->m_resultParam.measure.std_phi1,
//    m_efgio->m_resultParam.measure.std2_phi1
//  );
//  alg.SortAvgStd(
//    m_efgio->m_resultParam.measure.num,
//    m_efgio->m_resultParam.measure.cur_equ,
//    m_efgio->m_resultParam.measure.avg_equ,
//    m_efgio->m_resultParam.measure.std_equ,
//    m_efgio->m_resultParam.measure.std2_equ
//  );
//  //计算档位，在别的地方，或许在efgio
//  return 0;
//}

int CDiIntCounterSnap::StopDiInt()
{
	if (m_channel < 0 || !m_card)
		return -1;
	if (m_card->StopDi(m_channel)) {
		m_channel = -1;
		return 0;
	}
	return -1;
}

#ifndef USE_EFGV1
int CDiIntCounterSnap::StartURunTrd(double step, double acc, double speed)
{
	// TODO: 发开启转盘零位计数器，上升沿触发脉冲
	m_motor_u.dst_step = step;
	m_motor_u.max_acc = acc;
	m_motor_u.max_speed = speed;


	if (!m_card ||m_motor_u.state != MOTOR_STOP)
		return -1;



	gTrdU = AfxBeginThread(UMsg, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	gTrdU->m_bAutoDelete = TRUE;
	gTrdU->ResumeThread();


	return 0;
}
#endif