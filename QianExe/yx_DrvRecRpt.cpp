#include "yx_QianStdAfx.h"
#include "yx_DrvRecRpt.h"

#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4 || USE_DRVRECRPT == 11

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-DrvRec   ")

const UINT DRVRECWIN_REUPLOAD_INTERVAL = 30000; // ��ʻ��¼�����ش����,ms
const DWORD DRVWINSNDMAXPERIOD_FSTCHKGPRS = DRVRECWIN_REUPLOAD_INTERVAL * 3 + 8000; // ��ʻ��¼�ϴ����֮����û��Ӧ��,��Ҫ��һ�μ��GPRS����,ms
const DWORD DRVWINSNDMAXPERIOD_CHKGPRS = 300000; // ��ʻ��¼�ϴ����֮����û��Ӧ��,��һ�μ��GPRS�������û��Ӧ��,Ҫ��ײ���GPRS�ļ��,ms
const DWORD CHKDRV_INTERVAL = 1000; // ��ʻ��¼�����,1000ms,��Ķ�
const DWORD AVGSPEEDSAVE_INTERVAL = 60000; // �������һ��ƽ���ٶ�,ms

#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 11
const BYTE DRVRECFRAME_DATATYPE = 0x45;
#endif
#if USE_DRVRECRPT == 2
const BYTE DRVRECFRAME_DATATYPE = 0x46;
#endif
#if USE_DRVRECRPT == 3
const BYTE DRVRECFRAME_DATATYPE = 0x47;
#endif
#if USE_DRVRECRPT == 4
const BYTE DRVRECFRAME_DATATYPE = 0x48;
#endif

#pragma pack(4)

struct tagAvgSpeed
{
	float	m_fAvgSpeed; // ��/��
	char	m_szDate[3]; // ������ (�������ڴ�,�ļ��в����ڴ���)
	char	m_szTime[2]; // ʱ��
};

#pragma pack()

void *G_ThreadDrvRecSample(void *arg)
{
	g_objDrvRecRpt.P_ThreadDrvRecSample();
}

void G_TmReRptDrvRecWin(void *arg, int len)
{
	g_objDrvRecRpt.P_TmReRptDrvRecWin();
}

void G_TmStopUploadDrvrec(void *arg, int len)
{
	g_objDrvRecRpt.P_TmStopUploadDrvrec();
}

CDrvRecRpt::CDrvRecRpt()
{
	pthread_mutex_init( &m_MutexDrvRec, NULL );
	
	m_objWaitSndDrvRecMng.InitCfg( QUEUE_WAITSNDDRVREC_SIZE, QUEUE_WAITSNDDRVREC_PERIOD );
	memset( &m_objDrvRecSampleCfg, 0, sizeof(m_objDrvRecSampleCfg) );
	
	m_dwDrvCurWinFstSndTime = GetTickCount();
	m_bDrvCurWinChkGprs = false;
	m_bytCurSndDrvRecFrameNum = 0;
	m_bCurSndNeedAns = false;
	memset( m_aryCurSndDrvRecFrameInfo, 0, sizeof(m_aryCurSndDrvRecFrameInfo) );
}

CDrvRecRpt::~CDrvRecRpt()
{
	pthread_mutex_destroy( &m_MutexDrvRec );
}

int CDrvRecRpt::Init()
{
	int iRet = 0;
	Release();
	
	// ��������ʻ��¼���ݲ����߳�
	m_bStopDrvSample = false;
	m_bEndDrvSample = false;
	m_bForceRptDrv = false;
		
	if( pthread_create(&m_pthreadDrvSample, NULL, G_ThreadDrvRecSample, NULL) != 0 )
	{
		iRet = ERR_THREAD;
		goto INIT_END;
	}
	
	// �ٿ����ض�����ʻ��¼����
	_BeginDrvRecSample( (BOOL)(g_objQianIO.GetDeviceSta() & DEVSTA_ACCVALID) ); // �ú���Ҫ��CVeSta����_Init֮�����
	
INIT_END:
	return iRet;
}

int CDrvRecRpt::Release()
{
	m_bEndDrvSample = true;
	m_bStopDrvSample = true;
}

void CDrvRecRpt::P_ThreadDrvRecSample()
{
	char buf[SOCK_MAXSIZE];
	DWORD dwBufLen = 0;

	tagDrvSampleDatas objSampleDatas; // �������ʻ���ݼ�,���������ں�����һ����ʻ��¼,Ȼ�����²�����ʻ����
	tagDrvRecSampleCfg objCurSampleCfg, objNewSampleCfg; // ��ʻ���ݲ����ĵ�ǰ���ú�������

#if USE_DRVRECRPT == 2
	tagSampleData_3746 objOneSampleData; // ����һ�β�������ʻ����
	tagSampleData_3746* arySampleData = objSampleDatas.m_uniSampleDatas.m_arySampleData_3746;
#endif
#if USE_DRVRECRPT == 3
	tagSampleData_3747 objOneSampleData; // ����һ�β�������ʻ����
	tagSampleData_3747* arySampleData = objSampleDatas.m_uniSampleDatas.m_arySampleData_3747;
#endif
#if USE_DRVRECRPT == 11
	tagSampleData_3745_KJ2 objOneSampleData; // ����һ�β�������ʻ����
	tagSampleData_3745_KJ2* arySampleData = objSampleDatas.m_uniSampleDatas.m_arySampleData_3745_KJ2;
#endif
	
	tagGPSData gps(0);
	double dLstLon = 0; // �ϴ�ѭ��ʱ��¼�ľ�γ��
	double dLstLat = 0;
	double dDrvDis = 0; // �ϴ�ѭ��->����ѭ������ʻ�����
	double dCurSavSpdDis = 0; // ��������Ҫ����1����ƽ���ٶ�ʱ��1�������ۼ����
	double dMeterPerCycle = 0; // ÿ������������ (�궨ֵ����õ�)
	double dAvgSpeed = 0; // ����ѭ��ʱ���ɵ�ƽ���ٶ�,��/��
	DWORD dwWait = CHKDRV_INTERVAL; // �̵߳ȴ�ʱ��,����Ϊ�ó���ֵ
	DWORD dwCur = GetTickCount(); // ��Ҫ����
	DWORD dwLstSavSpdTm = dwCur; // �ϴα���1����ƽ���ٶȵ�ʱ��
	DWORD dwLstSavRecTm = dwCur; // �ϴβ�����ʻ���ݵ�ʱ��
	DWORD dwDeltTm = 1; // �����ܷ�ֹ��0�쳣
	DWORD dwPacketNum = 0; // ����1����ʻ��¼�����Ͷ��к�õ��Ķ����ڼ�¼����
	DWORD dwLstMeterCycle = 0; // �ϴ�ѭ��ʱ��ȡ�������������
	DWORD dwCurMeterCycle = 0; // ��ǰѭ��ʱ��ȡ�������������
	DWORD dwCyclePerSec = 0; // ��ǰѭ��ʱ��ȡ��ÿ��������
	DWORD dwIoSta = 0;
	BOOL blResult = FALSE;
	WORD wCyclePerKm = 0; // ÿ���������� (�궨ֵ)
	BYTE bytAvgSpeed = 0; // ����ѭ��ʱ���ɵ�ƽ���ٶ�,����/Сʱ
	BYTE bytLoopCt = 0; // ����=0
	BYTE bytSta = 0; // 0,��ʼ̬; 1,����̬; 2,������̬ͣ(��1�μ�⵽); 3,������̬ͣ
	bool bMeterDone = false;
	bool bSpdDone = false;

	// �����ʻ����ʱ���������Ƴ���
	const DWORD AVGSPD_MAXCOUNT = sizeof(objSampleDatas.m_aryAvgSpeed) / sizeof(objSampleDatas.m_aryAvgSpeed[0]);
	const DWORD SAMPLEDATA_MAXCOUNT = sizeof(objSampleDatas.m_uniSampleDatas.m_arySampleData_3746)
		/ sizeof(objSampleDatas.m_uniSampleDatas.m_arySampleData_3746[0]);

	// �߳�ѭ��ǰ�ĳ�ʼ�� (��Ҫ)
	dLstLon = dLstLat = 0;
	bytLoopCt = 0;
	bytSta = 0;
	memset( &objSampleDatas, 0, sizeof(objSampleDatas) );
	{
		// ��ȡ��λǰ�������Ч��γ��
		double dLichen = 0;
		long lLon = 0, lLat = 0;
		g_objCarSta.GetLstValidLonlatAndLichen( lLon, lLat, dLichen );
		dLstLon = lLon / LONLAT_DO_TO_L;
		dLstLat = lLat / LONLAT_DO_TO_L;
#if USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4
		objSampleDatas.m_dDrvDis = dLichen;
#endif
	}

	// �߳�ѭ������
	while( !m_bEndDrvSample ) 
	{
		usleep(dwWait*1000);	// �̶���ѭ�����

		// (1)ÿ��ѭ��ǰ�ĳ�ʼ��
		dwCur = GetTickCount();
		dwDeltTm = 1000; // ��ʼ�������ܷ�ֹ��0�쳣
		bytAvgSpeed = 0;
		memset( &objOneSampleData, 0, sizeof(objOneSampleData) );
		memset( &gps, 0, sizeof(gps) );
		GetCurGps( &gps, sizeof(gps), GPS_REAL ); // ��ȡGPSʵʱ����

		// (��Ҫ) ��ACC��Ч��ǿ����Ϊ��ЧGPS��ʹ�ú��治�������ͳ�ƣ�������Ϊ�´�ѭ��ʱʹ�õġ��ϴ���Чλ�á�
		if( !(g_objQianIO.GetDeviceSta() & DEVSTA_ACCVALID) )
			gps.valid = 'V';
		else
		{
			// ��һ�λ�ȡ����ЧGPSʱ,Ҫ��Flash�м�¼�������Чλ�ý��бȽ�,ƫ��10km����,��Flash�еļ�¼��ʹ��.
			static bool sta_bFstJugLstValid = true;
			if( sta_bFstJugLstValid && 'A' == gps.valid )
			{
				if( G_CalGeoDis2(dLstLon, dLstLat, gps.longitude, gps.latitude) > 10000 )
				{
					dLstLon = dLstLat = 0;
				}

				sta_bFstJugLstValid = false;
			}
		}

		if( 0 == bytSta ) // ���ǳ�ʼ״̬
		{			
			// ����ɵ���ʻ��¼����,���Ǳ����ۻ���ʻ��� (��Ҫ)
			double dDis = objSampleDatas.m_dDrvDis;
			memset( &objSampleDatas, 0, sizeof(objSampleDatas) );
			objSampleDatas.m_dDrvDis = dDis;

			dwLstSavSpdTm = dwLstSavRecTm = dwCur; // ���³�ʼ��������ʱ��
			_GetDrvRecSampleCfg( objNewSampleCfg ); // ����һ�β�������
			objCurSampleCfg = objNewSampleCfg; // ��ǰ����ʹ�����µ�����
		}

		if( 0 == bytLoopCt % 20 ) // ����,ÿ20��
		{
#ifdef DIS_SPD_USEIO
			// ����һ�α궨ֵ (��ʼʱҲ�ڴ˸���)
			GetImpCfg( &wCyclePerKm, sizeof(wCyclePerKm),
				offsetof(tagImportantCfg, m_uni1QBiaodingCfg.m_obj1QBiaodingCfg.m_wCyclePerKm), sizeof(wCyclePerKm) );
			if( wCyclePerKm > 50 )
			{
				dMeterPerCycle = 1000.0 / wCyclePerKm;  // ע�Ᵽ֤wCyclePerKm����0,�Է�ֹ��0�쳣
			}
			else
			{
				dMeterPerCycle = 0;
			}
#endif
			// ���ǲ���̬,�����һ�β�������
			if( 1 == bytSta ) _GetDrvRecSampleCfg( objNewSampleCfg );
		}


		// (2)���µ�ǰ״̬
		if( !m_bStopDrvSample ) // ������Ҫ��ֹ����
		{
			switch( bytSta )
			{
			case 0:
			case 1:
				bytSta = 1; // ����̬
				break;

			case 2: // ��һ����̬ͣ
			case 3: // ��ȫ��̬ͣ
			default:
				bytSta = 0; // ��ʼ̬
			}
		}
		else
		{
			m_bStopDrvSample = false;

			// �ж��Ƿ��һ����ͣ
			switch( bytSta )
			{
			case 2:
			case 3:
				bytSta = 3;
				break;

			default:
				bytSta = 2;
			}
		}


		// (3)�������ϴ�ѭ�������ڵ����
		dDrvDis = 0;
#ifdef DIS_SPD_USEIO
		bMeterDone = false;

		// ��ʹ���������
		blResult = g_objQianIO.IN_QueryMeterCycle( dwCurMeterCycle );
		if( blResult )
		{
			if( dMeterPerCycle > 0.01 ) // ���궨ֵ��Ч
			{
				dDrvDis = (dwCurMeterCycle - dwLstMeterCycle) * dMeterPerCycle;
				bMeterDone = true;
			}
			dwLstMeterCycle = dwCurMeterCycle;
		}
#endif
		
		// ����������ȡʧ��,�ٿ���GPS
		if( !bMeterDone && 'A' == gps.valid && dLstLon > 0.0001 && dLstLat > 0.0001 )
			// ���ϴξ�γ����Ч,�ұ���GPS��Ч(��ʱACC��Ч,��Ϊǰ��Ĵ�����ACC��Чʱ��GPSҲ��Ϊ��Ч��)
		{
			dDrvDis = G_CalGeoDis2( dLstLon, dLstLat, gps.longitude, gps.latitude );
		}


		// ���ڸ����ϴ�λ�� (dLstLon��dLstLat�ڱ���ѭ���ĺ��治����)
		if( 'A' == gps.valid )
			// ��ǰGPS��Ч,��ACC��Чʱ
		{
			dLstLon = gps.longitude;
			dLstLat = gps.latitude;

			static DWORD sta_dwCt = 0;
			if( 0 == ++ sta_dwCt % 600 ) // ACC��Ч��GPS��Ч״̬�£�ÿ10���ӣ�����һ�������Чλ�ã���������ȷ������ۼ�׼ȷ
			{
				g_objCarSta.SavLstValidLonlatAndLichen(
					long(gps.longitude * LONLAT_DO_TO_L),
					long(gps.latitude * LONLAT_DO_TO_L),
					objSampleDatas.m_dDrvDis );
				
				// �����߳����ļ���д�빤��
				QIANMSG msg;
				msg.m_lMsgType = MSG_SAVRECSTA;
				if( msgsnd(g_MsgID, (void*)&msg, MSG_MAXLEN, 0) == -1 )
					perror("send MSG_SAVRECSTA faile:");
				else
					PRTMSG(MSG_DBG, "send MSG_SAVRECSTA\n");
			}
		}


		// (4)�ۼ���ʻ���
		objSampleDatas.m_dDrvDis += dDrvDis;


		// (5)�ж��Ƿ�Ҫ����
		if( 1 == bytSta || 2 == bytSta ) // ���ǲ���̬���ǵ�һ����ͣ����̬
		{
			// (5_1)�ж��Ƿ�ﵽ��������,�ﵽ��Ҫ����1����ʻ����
			dwDeltTm = dwCur - dwLstSavRecTm;
			if( objCurSampleCfg.m_wSampleCycle > 0 // ����Ҫ����
				&&
				dwDeltTm >= objCurSampleCfg.m_wSampleCycle * DWORD(1000) ) // �Ҵﵽ��������
			{
				if( dwDeltTm < 1 ) dwDeltTm = 1; // ��ֹ��0����

				// ����1����ʻ����
				objOneSampleData.m_bytYear = BYTE(gps.nYear % 1000);
				objOneSampleData.m_bytMon = BYTE(gps.nMonth);
				objOneSampleData.m_bytDay = BYTE(gps.nDay);
				objOneSampleData.m_bytHour = BYTE(gps.nHour);
				objOneSampleData.m_bytMin = BYTE(gps.nMinute);
				objOneSampleData.m_bytSec = BYTE(gps.nSecond);
				{
					WORD wCurDriverNo = g_objCarSta.GetDriverNo();
					objOneSampleData.m_szDriverNo[0] = BYTE(wCurDriverNo / 0x100 );
					objOneSampleData.m_szDriverNo[1] = BYTE(wCurDriverNo % 0x100 );
				}
#ifdef DIS_SPD_USEIO
				bSpdDone = false;

				// ��ͨ���������
				if( dMeterPerCycle > 0.01 ) // ���궨ֵ��Ч
				{
					if( g_objQianIO.IN_QueryIOSpeed(dwCyclePerSec) )
					{
						objOneSampleData.m_bytSpeed = BYTE(dwCyclePerSec * dMeterPerCycle * MIMIAO_TO_HAIHOUR);
						bSpdDone = true;
					}
				}
				
				// ʧ����ʹ��GPS
				if( !bSpdDone )
				{
					objOneSampleData.m_bytSpeed = BYTE(gps.speed * MIMIAO_TO_HAIHOUR);
				}
#else
				objOneSampleData.m_bytSpeed = BYTE(gps.speed * MIMIAO_TO_HAIHOUR);
#endif
				objOneSampleData.m_bytDir = BYTE(gps.direction / 3) + 1;

				g_objQianIO.IN_QueryIOSta( dwIoSta );				
				objOneSampleData.m_bytIO = ~BYTE(dwIoSta % 0x100); 

#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 4
				if( g_objCarSta.GetVeSta() & VESTA_HEAVY )
					objOneSampleData.m_bytIO |= 0x08;
				else
					objOneSampleData.m_bytIO &= 0xf7;
#endif
				// ��ȡ״̬��Ϣ
				{
					// Э�鶨��:
					// ״̬�֣�H7 H6 H5 H4 H3 H2 H1 H0
					//		   L7 L6 L5 L4 L3 L2 L1 L0
					// H0��GPRS�������  0������        1����
					// H1��������ײ       0 ����ײ       1 ��ײ
					// H2�������෭       0�޲෭       1 �෭
					// H3��Ƿѹ״̬       0 ��Ƿѹ       1Ƿѹ
					// H4������ϵ�״̬   0 ����δ�ϵ�   1����ϵ�
					// H5�����ٱ���״̬   0�����ٱ���   1�������ٱ���
					// H6���Ƿ�����״̬   0 �޷Ƿ�����   1 �����Ƿ�����
					// H7������
					// 
					// L1��L0��γ������:
					// 0  ������γ  1  ������γ  2  ������γ  3  ������γ
					// L3��L2 GPSģ�鶨λ״̬��
					// 0��δ��λ    1��2D��λ    2��3D��λ   3 ����
					// L5��L4 GPSģ�鹤��״̬��
					// 0������      1��ʡ��       2��ͨѶ�쳣  3 ����
					// L6��L7 ����
					// ����λ��������������Ϊ0��
					DWORD dwAlertSta = g_objMonAlert.GetAlertSta();
					DWORD dwDevSta = g_objQianIO.GetDeviceSta();
					BYTE bytH = 0, bytL = 0;
					if( g_objSock.IsOnline() ) bytH |= 0x01; // �����������
					if( dwAlertSta & ALERT_TYPE_BUMP ) bytH |= 0x02; // ��ײ����
					if( dwAlertSta & ALERT_TYPE_OVERTURN ) bytH |= 0x04; // �෭����
					if( dwAlertSta & ALERT_TYPE_POWBROWNOUT ) bytH |= 0x08; // Ƿѹ״̬
					if( dwAlertSta & ALERT_TYPE_POWOFF ) bytH |= 0x10; // �ϵ�״̬
					if( dwAlertSta & ALERT_TYPE_FOOT ) bytH |= 0x20; // ���ٱ���
					if( dwAlertSta & ALERT_TYPE_DRIVE ) bytH |= 0x40; // �Ƿ���������
					bytL |= 0; // ��ʱ��ǿ��ʹ��"������γ"
					if( '3' == gps.m_cFixType ) bytL |= 0x08; // ��λ����
					else if( '2' == gps.m_cFixType ) bytL |= 0x04;
					else bytL |= 0;
					if( dwDevSta & DEVSTA_GPSON ) bytL |= 0; // GPSģ��״̬
					else bytL |= 0x10;

					objOneSampleData.m_bytSta1 = bytH;
					objOneSampleData.m_bytSta2 = bytL;
				}

				G_Lonlat_DTo4Char( gps.longitude, objOneSampleData.m_szLon);
				G_Lonlat_DTo4Char( gps.latitude, objOneSampleData.m_szLat);

#if USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3
				// Э�鶨��
				// bit7~Bit0: 0��ʾ��Ч��1��ʾ��Ч
				// Bit0: ��ײ���λ
				// Bit1���෭���λ
				// Bit2��Ƿѹ���λ
				// Bit3���ϵ���λ
				// Bit4�����ٰ�ť���λ
				// (������,����) objOneSampleData.m_bytAlertIO = BYTE( ~(dwIoSta >> 9) & 0x1f ); // ������������ֵ�����Э�鶨��,���������ٴ���
				objOneSampleData.m_bytAlertIO = 0;
				if( !(dwIoSta & 0x200) ) objOneSampleData.m_bytAlertIO |= 0x01; //��ײIO
				if( !(dwIoSta & 0x400) ) objOneSampleData.m_bytAlertIO |= 0x02; //�෭IO
				if( !(dwIoSta & 0x800) ) objOneSampleData.m_bytAlertIO |= 0x04; //ǷѹIO
				if( dwIoSta & 0x1000 ) objOneSampleData.m_bytAlertIO |= 0x08; //�ϵ�IO
				if( dwIoSta & 0x2000 ) objOneSampleData.m_bytAlertIO |= 0x10; //����IO
				objOneSampleData.m_bytHeightCount = 0; // ��ʱ���������ش�����
				memset( objOneSampleData.m_aryHeightIO, 0, sizeof(objOneSampleData.m_aryHeightIO) ); // ��������������0
#endif
			
#if USE_DRVRECRPT == 11
				objOneSampleData.m_bytPDop = BYTE(gps.m_bytPDop);
				objOneSampleData.m_bytHDop = BYTE(gps.m_bytHDop);
				objOneSampleData.m_bytVDop = BYTE(gps.m_bytVDop);
#endif

				// ������ʻ����
				if( objSampleDatas.m_bytSampleDataCount < SAMPLEDATA_MAXCOUNT ) // �������
				{
					arySampleData[ objSampleDatas.m_bytSampleDataCount ++ ]
						= objOneSampleData;
				}

				dwLstSavRecTm = dwCur; // �����ϴβ�����ʻ���ݵ�ʱ��
			}

			// (5_2)�ۼ�ƽ���ٶȼ��������ڵ����,���ж��Ƿ�Ҫ����ƽ���ٶ�
			dCurSavSpdDis += dDrvDis;
			dwDeltTm = dwCur - dwLstSavSpdTm;
			if( dwDeltTm >= AVGSPEEDSAVE_INTERVAL ) // �������ϴμ���ƽ���ٶȵ�ʱ��ﵽ�˼�������
			{
				if( dwDeltTm < 1 ) dwDeltTm = 1; // ��ֹ��0����
				
				// ����1����ƽ���ٶȲ�����
				dAvgSpeed = dCurSavSpdDis / (dwDeltTm / 1000.0);
				bytAvgSpeed = BYTE(dAvgSpeed * MIMIAO_TO_HAIHOUR);
				//if( objSampleDatas.m_bytAvgSpeedCount < AVGSPD_MAXCOUNT ) // �������
				{
					objSampleDatas.m_aryAvgSpeed[ objSampleDatas.m_bytAvgSpeedCount ++ ] = bytAvgSpeed;
				}

				dCurSavSpdDis = 0; // ����ƽ���ٶ��ۼ����
				dwLstSavSpdTm = dwCur; // �����ϴ�ƽ���ٶ�����ʱ��
			}
		}


		// (6)�ж��Ƿ�Ҫ����һ����ʻ��¼֡
		if( objCurSampleCfg.m_bytSndCycle > 0 ) // ��Ҫ����
		{
			// �ж��Ƿ�Ҫǿ����֡������
			bool bForceMakeWin =
				2 == bytSta // ��1����ͣ����̬
				||
				m_bForceRptDrv // �� �ⲿ������������
				||
				(objNewSampleCfg.m_wSampleCycle != objCurSampleCfg.m_wSampleCycle
				|| objNewSampleCfg.m_bytSndCycle != objCurSampleCfg.m_bytSndCycle
				|| objNewSampleCfg.m_bytSndWinSize != objCurSampleCfg.m_bytSndWinSize); // �� ����Ҫ��������

			if( m_bForceRptDrv == true ) m_bForceRptDrv = false;

			if( objSampleDatas.m_bytSampleDataCount >= objCurSampleCfg.m_bytSndCycle // �����﷢������
				|| bForceMakeWin ) // ������Ҫǿ����֡������
			{
#if USE_DRVRECRPT == 3 || USE_DRVRECRPT == 11

#if USE_DRVRECRPT == 3
				const double OIL_MVESPD = 1.852 * 2 / 3.6; // ��/��
#endif
#if USE_DRVRECRPT == 11
				const double OIL_MVESPD = 15 / 3.6; // ��/��
#endif
				
				// ��ȡ��ǰ���״̬
				static bool sta_bAccValid = true;
				bool bGpsValid = gps.valid == 'A' ? true : false;
				bool bAccValid = g_objQianIO.GetDeviceSta() & DEVSTA_ACCVALID ? true : false;

				
#if USE_OIL == 1

				// ȷ���ͺ�״̬
#if USE_DRVRECRPT == 3
				// ǧ����Э��: �ͺ�ֵ��״̬ 1 D7��D0����ʾ�ͺĲ���ʱ��״̬ (���¸���״ֻ̬��һλ��Ч)
				// D0��0������ʻ��1�������
				// D1��0��ʾ������1 GPS��λ���ٶȴ���2����/Сʱ
				// D2��0��ʾ��λ��1����λ
				// D3��0��ʾ������1 GPS��λ���ٶ�С��2����/Сʱ
				// D4��0��ʾ������1ͣ����ACC��Ч����ACC��Ч�Ǳ���ȴû���ϵ�
				// D5��0��ʾ������1�ͺĺ�û��
				// D6��0��ʾ������1�ͺ�ֵ�������÷�Χ�ڣ���ֵ��Ч��

				// �����źŲ��ܲɼ�,������
				if( !bAccValid ) objSampleDatas.m_bytOilSta = 0x10;
				else if( !bGpsValid ) objSampleDatas.m_bytOilSta = 0x04;
				else
				{
					if( gps.speed >= OIL_MVESPD ) objSampleDatas.m_bytOilSta = 0x02;
					else objSampleDatas.m_bytOilSta = 0x08;
				}
#endif

#if USE_DRVRECRPT == 11
				// 2��Э��
				// D7��D0����ʾ�ͺĲ���ʱ��״̬
				// 0x01: ��� (�¼�����)
				// 0x02: ��ͨ�磬�Ѷ�λ���ٶ�>15
				// 0x04: ��ͨ�磬δ��λ
				// 0x08: ��ͨ�磬�Ѷ�λ���ٶ�>15
				// 0x10: ������Ч�ģ�δͨ��
				if( !bAccValid ) objSampleDatas.m_bytOilSta = 0x10;
				else if( bAccValid && !sta_bAccValid ) objSampleDatas.m_bytOilSta = 0x01;
				else if( bAccValid && !bGpsValid ) objSampleDatas.m_bytOilSta = 0x04;
				else if( bAccValid && bGpsValid && gps.speed >= OIL_MVESPD ) objSampleDatas.m_bytOilSta = 0x02;
				else if( bAccValid && bGpsValid && gps.speed < OIL_MVESPD ) objSampleDatas.m_bytOilSta = 0x08;
#endif

#endif

				// ��ȡ�ͺ�ֵ
				BOOL blGetOilSucc = FALSE; // ��ʼ��Ϊ��ȡʧ��
// 				if( bAccValid )
// 				{
#if USE_OIL == 1
					blGetOilSucc = g_objQianIO.IN_QueryOil( objSampleDatas.m_bytOilAD, objSampleDatas.m_bytPowAD );
#endif
#if USE_OIL == 2
					blGetOilSucc = g_objOil.GetOilPowAndSta( &objSampleDatas.m_bytOilAD,
						&objSampleDatas.m_bytPowAD, &objSampleDatas.m_bytOilSta );
#endif
// 				}
 				if( blGetOilSucc )
				{
#if USE_OIL == 1
					// ��������״̬
					g_objCarSta.SavOil( objSampleDatas.m_bytOilAD, objSampleDatas.m_bytPowAD, objSampleDatas.m_bytOilSta );
#endif
				}
				else
				{
					// ��ȡ״̬�б�����ϴ���Ч�ͺ�����
					g_objCarSta.GetOil( &objSampleDatas.m_bytOilAD, &objSampleDatas.m_bytPowAD, &objSampleDatas.m_bytOilSta );
				}

				// �����ͺ�����������
				{
					tag1QBiaoding objBd;	
					if( !GetImpCfg( &objBd, sizeof(objBd), offsetof(tagImportantCfg, m_uni1QBiaodingCfg.m_obj1QBiaodingCfg), sizeof(objBd) ) )
					{
						if( objBd.m_bReverse )
						{
							objSampleDatas.m_bytOilAD =  255 - objSampleDatas.m_bytOilAD; // xun��Ҫ��תҲ������ת�ͺ�AD
						}
						
						if(  objBd.m_bFiltrate && (objSampleDatas.m_bytOilAD > objBd.m_bytHiVal || objSampleDatas.m_bytOilAD < objBd.m_bytLoVal) )
						{
							objSampleDatas.m_bytOilSta = 0x40; // ��Χ��Ч
						}
					}
				}

				// ����ACC״̬��¼
				sta_bAccValid = bAccValid;
#endif
				
				// ��ǰ��ʻ�������1����ʻ��¼֡,���뷢�Ͷ���
				if( !_MakeOneDrvrecFrame(objSampleDatas, objCurSampleCfg.m_wSampleCycle, buf, sizeof(buf), dwBufLen) )
					// ����֡�ɹ�
				{
					m_objWaitSndDrvRecMng.PushData(LV8, dwBufLen, buf, dwPacketNum);
					PRTMSG(MSG_DBG, "m_objWaitSndDrvRecMng push one frame\n");
				}
				else // �����ɹ�
				{ // �������� (��������֡,��Ϊ�����ڴ治��,�ٻ���û������;����,�����ǿ�����,Ҳ����Ҫ����)
				}
				// �����Ƿ���֡�ɹ���Ҫ֪ͨ����,�����ϴ���֡�����ڻ�û�в�����,Ҳ������
				
				QIANMSG msg;
				msg.m_lMsgType = MSG_DRVECDTOSND;
				msg.m_uszMsgBuf[0] = (char)(bForceMakeWin ? 1 : 0);
				if( msgsnd(g_MsgID, (void*)&msg, MSG_MAXLEN, 0) == -1 )
					perror("send MSG_DRVECDTOSND faile:");
				else
					PRTMSG(MSG_DBG, "send MSG_DRVECDTOSND\n");

				double dLeaveDis = 0;
#if USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3
				// ���������
				dLeaveDis = objSampleDatas.m_dDrvDis;
#endif
#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 11
				// ������֡ʱ��Ϊ����ԭ���"��ʧ"�����
				dLeaveDis = objSampleDatas.m_dDrvDis - WORD(objSampleDatas.m_dDrvDis);
#endif
				
				// �����ǰ������¼��
				memset( &objSampleDatas, 0, sizeof(objSampleDatas) );
				
				// �ָ������,�򲹳���֡ʱ��ʧ�����
				objSampleDatas.m_dDrvDis = dLeaveDis;

				// ������ʻ��¼֡��,Ҫ���µ�ǰ�Ĳ������� (��Ϊ�������÷����仯ʱ���ȰѾɵ���ʻ����������ʻ��¼֡)
				objCurSampleCfg = objNewSampleCfg;
			}
		}

		// ��ǰ����ʹ�����µ�����,����и��µĻ�
		objCurSampleCfg = objNewSampleCfg;

		// �����ȴ�ʱ�� (Ϊ�˾����ܵ�ȷ����ʱ׼ȷ)
		{
			DWORD dwDelt = GetTickCount() - dwCur;
			if( dwDelt + 200 < CHKDRV_INTERVAL ) // ����Ԥ��200ms
				dwWait = CHKDRV_INTERVAL - dwDelt;
			else
				dwWait = CHKDRV_INTERVAL;
		}

		// ����ѭ������
		++ bytLoopCt;
	}

	// �߳��˳�ǰ,��������Ч��γ�Ⱥ�����̱��浽״̬ģ����
	g_objCarSta.SavLstValidLonlatAndLichen( 
					long(dLstLon * LONLAT_DO_TO_L),
					long(dLstLat * LONLAT_DO_TO_L),
					objSampleDatas.m_dDrvDis );
}

void CDrvRecRpt::P_TmReRptDrvRecWin()
{
	_SndDrvrecWin( true );
}

void CDrvRecRpt::P_TmStopUploadDrvrec()
{
	_KillTimer(&g_objTimerMng, DRVREC_STOPUPLOAD_TIMER );
	_StopDrvRecSample();
}

void CDrvRecRpt::DrvRecToSnd( bool v_bNeedSndWin )
{
	#if DRVREC_SNDTYPE == 1
///////// { ��ʽ1: ����һ�����ڲŷ���
	// ������δ������Ĵ���,���˳��������� (��ʱ���ɶ�ʱ�������ش�)
	if( !_IsCurSndDrvRecWinEmpty() ) return;

	// ������û�����ڷ��͵Ĵ��ڵĴ���,��ʱ�ش���ʱ��Ӧ���Ѿ��ر�


	// ��ȡ����
	tagDrvRecSampleCfg objDrvRecSampleCfg;
	_GetDrvRecSampleCfg( objDrvRecSampleCfg );

	// ���﷢�ʹ����ж�
	bool bNeedSndWin = false;
	if( v_bNeedSndWin ) bNeedSndWin = true; // ��ǿ����Ϊһ�����ڷ���
	else if( m_objWaitSndDrvRecMng.GetPacketCount() >= objDrvRecSampleCfg.m_bytSndWinSize ) bNeedSndWin = true;

	// �����﷢�ʹ���
	if( bNeedSndWin )
	{
		// ����һ���´�������
		_FillCurSndDrvRecWin();

		// ����һ�����ڵ�����
		_SndDrvrecWin( false );
	}
///////// } ��ʽ1: ����һ�����ڲŷ���
#endif


#if DRVREC_SNDTYPE == 2
///////// { ��ʽ2: ÿ֡����,��һ�����ڵȴ�Ӧ��
	static tagDrvRecSampleCfg sta_objDrvRecSampleCfg;
	static BYTE sta_bytWinId = 0;

	if( m_bytCurSndDrvRecFrameNum < 1 ) // ����ʱ��û��δ����ľɴ���,Ҳû�з��͹��µ�֡
	{
		// ˢ������
		_GetDrvRecSampleCfg( sta_objDrvRecSampleCfg );
		m_bCurSndNeedAns = sta_objDrvRecSampleCfg.m_bytSndWinSize > 0;

		// ���ղ���
		_KillTimer(&g_objTimerMng, DRVRECWIN_REUPLOAD_TIMER );
	}
	else if( m_bCurSndNeedAns && m_bytCurSndDrvRecFrameNum >= sta_objDrvRecSampleCfg.m_bytSndWinSize )
		// ����ǰ����Ҫ�ɿ����������ڵȴ�Ӧ��
	{
		return;
	}

	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	char* pSndBuf = NULL;
	DWORD dwLen = 0;
	DWORD dwPushTm = 0;
	int iResult = 0;
	BYTE bytLvl = 0;
	BYTE bytFillNum = 0;

	while( m_bytCurSndDrvRecFrameNum < sta_objDrvRecSampleCfg.m_bytSndWinSize // ����δ����1������
		|| 0 == sta_objDrvRecSampleCfg.m_bytSndWinSize ) // ����Ҫ�ɿ�����
	{
		// ��ȡһ֡����
		if( m_objWaitSndDrvRecMng.PopData( bytLvl, sizeof(buf), dwLen, buf, dwPushTm ) )
		{
			PRTMSG(MSG_DBG, "m_objWaitSndDrvRecMng pop no data\n");
			break;
		}

		if( dwLen < 1 )
			continue;

		pSndBuf = new char[dwLen];
		if( !pSndBuf ) break;
		memcpy( pSndBuf, buf, dwLen );

		// ��ʣ����Ϣ������,���ǿɿ�����Ҫ����,�����������һ֡��Ҫ���ش���ʱ��
		if( m_bCurSndNeedAns ) // ����Ҫ�ɿ�����
		{
			pSndBuf[0] |= 0x42; // �ɿ�����
			pSndBuf[0] |= sta_bytWinId ? 0x08 : 0; // ���ڱ�־ID
			if( m_bytCurSndDrvRecFrameNum + 1 >= sta_objDrvRecSampleCfg.m_bytSndWinSize ) // ���Ǳ��������1֡
			{
				pSndBuf[0] |= 0x01; // ���: ĩ֡ҪӦ��
				sta_bytWinId = ~sta_bytWinId; // �������ڱ�־

				// ���ش���ʱ��
				_SetTimer(&g_objTimerMng, DRVRECWIN_REUPLOAD_TIMER, DRVRECWIN_REUPLOAD_INTERVAL, G_TmReRptDrvRecWin);
				m_dwDrvCurWinFstSndTime = GetTickCount();
				m_bDrvCurWinChkGprs = false;
			}
			pSndBuf[1] = sta_objDrvRecSampleCfg.m_bytSndWinSize; // ���ڴ�С
			pSndBuf[2] = m_bytCurSndDrvRecFrameNum; // ֡���

			// �ɿ�����ʱҪ�����ѷ���֡
			if( m_bytCurSndDrvRecFrameNum < sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0]) )
			{
				m_aryCurSndDrvRecFrameInfo[m_bytCurSndDrvRecFrameNum][0] = dwLen;
				m_aryCurSndDrvRecFrameInfo[m_bytCurSndDrvRecFrameNum][1] = DWORD(pSndBuf);
				++ m_bytCurSndDrvRecFrameNum;
			}
		}
		else
		{
			pSndBuf[0] |= 0x40;
			pSndBuf[1] = 0;
			pSndBuf[2] = 0;
		}

		// �������͸�֡
		iResult = g_objSms.MakeSmsFrame( pSndBuf, int(dwLen), 0x37, DRVRECFRAME_DATATYPE,
			buf, sizeof(buf), iBufLen, CVT_8TO7 );
		if( !iResult )
		{
			iResult = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV8 );
		}
	}
///////// } ��ʽ2: ÿ֡����,��һ�����ڵȴ�Ӧ��
#endif
}

void CDrvRecRpt::RecStaSav()
{
	g_objCarSta.SavDrvRecSta( false );
}

void CDrvRecRpt::_BeginDrvRecSample( BOOL v_blAccOn )
{
	// ��ȡ��������
	tag2QDriveRecCfg objDrvRecCfg;
	objDrvRecCfg.Init();
	GetSecCfg( &objDrvRecCfg, sizeof(objDrvRecCfg),
		offsetof(tagSecondCfg, m_uni2QDriveRecCfg.m_obj2QDriveRecCfg), sizeof(objDrvRecCfg) );
	
	// (1)�޸��ڴ��еĲ�����������
	tagDrvRecSampleCfg objDrvSampleCfg;
	WORD wSndPeriod = 1; // (����)
	if( v_blAccOn )
	{
		objDrvSampleCfg.m_wSampleCycle = objDrvRecCfg.m_wOnSampCycle;
		objDrvSampleCfg.m_bytSndCycle = objDrvRecCfg.m_bytOnSndCycle;
		wSndPeriod = objDrvRecCfg.m_wOnPeriod;
	}
	else
	{
		objDrvSampleCfg.m_wSampleCycle = objDrvRecCfg.m_wOffSampCycle;
		objDrvSampleCfg.m_bytSndCycle = objDrvRecCfg.m_bytOffSndCycle;
		wSndPeriod = objDrvRecCfg.m_wOffPeriod;
	}
	objDrvSampleCfg.m_bytSndWinSize = objDrvRecCfg.m_bytSndWinSize;
	_SetDrvRecSampleCfg( objDrvSampleCfg );
	m_bForceRptDrv = true; // ǿ������3745��ʻ��¼֡���ϴ�

	// (2)�ж��Ƿ�Ҫ���в���
	bool bNeedSample = false;
	if( v_blAccOn && (objDrvRecCfg.m_wOnSampCycle > 0 && objDrvRecCfg.m_bytOnSndCycle > 0) )
	{
		bNeedSample = true;
	}
	else if( !v_blAccOn && (objDrvRecCfg.m_wOffSampCycle > 0 && objDrvRecCfg.m_bytOffSndCycle > 0) )
	{
		bNeedSample = true;
	}
	else
	{
		bNeedSample = false;
	}
	
	if( bNeedSample )
	{
		m_bStopDrvSample = false;
		_KillTimer(&g_objTimerMng, DRVREC_STOPUPLOAD_TIMER );
		if( 0xffff != wSndPeriod )
			_SetTimer(&g_objTimerMng, DRVREC_STOPUPLOAD_TIMER, DWORD(wSndPeriod) * 60000, G_TmStopUploadDrvrec);
	}
	else
	{
		m_bStopDrvSample = true;
	}
}

void CDrvRecRpt::_StopDrvRecSample()
{
	m_bStopDrvSample = true;
}

void CDrvRecRpt::_GetDrvRecSampleCfg( tagDrvRecSampleCfg &v_objDrvRecSampleCfg )
{
	pthread_mutex_lock( &m_MutexDrvRec );

	v_objDrvRecSampleCfg = m_objDrvRecSampleCfg;

	pthread_mutex_unlock( &m_MutexDrvRec );
}

void CDrvRecRpt::_SetDrvRecSampleCfg( const tagDrvRecSampleCfg &v_objDrvRecSampleCfg  )
{
	pthread_mutex_lock( &m_MutexDrvRec );

	m_objDrvRecSampleCfg = v_objDrvRecSampleCfg;

	pthread_mutex_unlock( &m_MutexDrvRec );
}

#if DRVREC_SNDTYPE == 1
bool CDrvRecRpt::_IsCurSndDrvRecWinEmpty()
{
	return m_bytCurSndDrvRecFrameNum < 1;
}

// ���һ�����ڵ�����,���ڴ�С��������,������ʵ���ܴӶ����ж�����Ϊ׼,���Ҳ��������ʹ��ڻ������ķ�Χ
// ������ǿɿ�����,��������һ�鷢������,�ڲ��������ͻ������ķ�Χ��,�����ܶ�Ķ�ȡ
void CDrvRecRpt::_FillCurSndDrvRecWin()
{
	// ���ղ���,�������״̬,ʵ���Ͻ���ú���ǰ���ʹ���Ӧ���ǿյĻ��߸ô������ѷ�����ϵ�
	_ClearCurSndDrvRecWin();
	
	// ��ȡ����,���µ�ǰ����״̬
	tagDrvRecSampleCfg objDrvSampleCfg;
	_GetDrvRecSampleCfg( objDrvSampleCfg );
	m_bCurSndNeedAns = objDrvSampleCfg.m_bytSndWinSize > 0 ? true : false;
	
	// ȷ������Ҫ�����֡��(�ɿ�����ʱ����һ������)
	BYTE bytFillNum = 0;
	if( m_bCurSndNeedAns )
	{
		bytFillNum = objDrvSampleCfg.m_bytSndWinSize;
		if( bytFillNum > sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0]) )
			bytFillNum = sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0]); // ���ղ���
	}
	else bytFillNum = 32;
	
	// �����е������������ǰ���ʹ��ڻ�����
	char buf[ SOCK_MAXSIZE ];
	BYTE bytLvl = 0;
	DWORD dwLen = 0;
	DWORD dwPushTm = 0;
	char* pTemp = NULL;
	for( BYTE byt = 0; byt < bytFillNum; byt ++ )
	{
		if( m_objWaitSndDrvRecMng.PopData( bytLvl, sizeof(buf), dwLen, buf, dwPushTm ) ) break;
		if( dwLen < 1 ) continue;
		
		if( byt < sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0]) )
		{
			pTemp = new char[dwLen];
			if( !pTemp ) break;
			
			memcpy( pTemp, buf, dwLen );
			m_aryCurSndDrvRecFrameInfo[byt][0] = dwLen;
			m_aryCurSndDrvRecFrameInfo[byt][1] = DWORD(pTemp);
		}
	}
	
	// ��ǰ���͵���֡������ʵ������֡��
	m_bytCurSndDrvRecFrameNum = byt;
}
#endif

void CDrvRecRpt::_ClearCurSndDrvRecWin()
{
	for( DWORD dw = 0; dw < sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0]); dw ++ )
	{
		delete[] (char*) (m_aryCurSndDrvRecFrameInfo[dw][1]);
	}
	memset( m_aryCurSndDrvRecFrameInfo, 0, sizeof(m_aryCurSndDrvRecFrameInfo) );
	m_bytCurSndDrvRecFrameNum = 0;
	m_bCurSndNeedAns = false;
}

int CDrvRecRpt::_MakeOneDrvrecFrame( const tagDrvSampleDatas &v_objDrvSampleDatas, const WORD v_wSampleCycle,
			char *const v_szBuf, const size_t v_sizBuf, DWORD &v_dwLen )
{
	DWORD dw = 0;
	DWORD dwTemp = 0;
	WORD wTemp = 0;
	BYTE byt = 0;
	BYTE bytOilSta = 0;

	// Ԥ���
	memset( v_szBuf, 0, v_sizBuf );
	v_dwLen = 0;

	// �ж��Ƿ������ʻ����
	if( v_objDrvSampleDatas.m_bytSampleDataCount < 1 )
	{
		PRTMSG(MSG_ERR, "no SampleData\n");
		return ERR_PAR;
	}
	
	// ����ռ���
	if( v_sizBuf < 1000 ) // ����Ϊһ�������ϵļ���ֵ
	{
		return ERR_SPACELACK;
	}

	
	// ����һ֡ (�����ͺı��,Ȼ�����̿�ʼ��ֵ��ǰ�������ֵ�ڷ���ʱ��ֵ)

	// ���
#if USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4
	dwTemp = htonl( DWORD(v_objDrvSampleDatas.m_dDrvDis) );
	memcpy( v_szBuf + 3, &dwTemp, 4); // �����
	dw = 7;
#endif
#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 11
	wTemp = htons( WORD(v_objDrvSampleDatas.m_dDrvDis) );
	memcpy( v_szBuf + 3, &wTemp, 2); // ���
	dw = 5;
#endif

	// �ͺ�
#if USE_DRVRECRPT == 3 || USE_DRVRECRPT == 11
	bytOilSta = v_objDrvSampleDatas.m_bytOilSta;
#if USE_DRVRECRPT == 11
	v_szBuf[ 0 ] |= char( 0x80 ); // �ͺı�־λ��1
#endif
	v_szBuf[ dw ++ ] = char( v_objDrvSampleDatas.m_bytOilAD );
	v_szBuf[ dw ++ ] = char( v_objDrvSampleDatas.m_bytPowAD );
	v_szBuf[ dw ++ ] = char( bytOilSta );
#endif

	// �������
	wTemp = htons( v_wSampleCycle );
	memcpy( v_szBuf + dw, &wTemp, 2 );
	dw += 2;

	// ��ʻ���ݸ���
	v_szBuf[ dw ++ ] = char( v_objDrvSampleDatas.m_bytSampleDataCount );
	
	// ��ʻ����
	for( byt = 0; byt < v_objDrvSampleDatas.m_bytSampleDataCount; byt ++ )
	{
#if USE_DRVRECRPT == 2
		// ������С
		dwTemp = sizeof(tagSampleData_3746);

		// ȥ������������ֶ�
		if( v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_bytHeightCount
			>
			sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_aryHeightIO)
			/
			sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_aryHeightIO[0]) )
		{ // �������
			memset( (char*)&v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_bytHeightCount, 1,
				sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_bytHeightCount) );
		}
		dwTemp -= sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_aryHeightIO)
			-
			v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_bytHeightCount
			* sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_aryHeightIO[0]);

		// ���뷢�ͻ���
		if( byt > 0 )
		{
			dwTemp -= offsetof(tagSampleData_3746, m_szDriverNo); // �ǵ�һ��,ȥ������ʱ��
			memcpy( v_szBuf + dw, v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_szDriverNo, dwTemp );
			dw += dwTemp;
		}
		else
		{
			// ��һ���㺬����ʱ��
			memcpy( v_szBuf + dw, v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746 + byt, dwTemp );
			dw += dwTemp;
		}
#endif

#if USE_DRVRECRPT == 3
		// ������С
		dwTemp = sizeof(tagSampleData_3747);

		// ȥ������������ֶ�
		if( v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3747[byt].m_bytHeightCount
			>
			sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3747[byt].m_aryHeightIO)
			/
			sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3747[byt].m_aryHeightIO[0]) )
		{ // �������
			memset( (char*)&v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3747[byt].m_bytHeightCount, 1,
				sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3747[byt].m_bytHeightCount) );
		}
		dwTemp -= sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3747[byt].m_aryHeightIO)
			-
			v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3747[byt].m_bytHeightCount
			* sizeof(v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3747[byt].m_aryHeightIO[0]);

		// ���뷢�ͻ���
		if( byt > 0 )
		{
			dwTemp -= offsetof(tagSampleData_3747, m_szDriverNo); // �ǵ�һ��,ȥ������ʱ��
			memcpy( v_szBuf + dw, v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746[byt].m_szDriverNo, dwTemp );
			dw += dwTemp;
		}
		else
		{
			// ��һ���㺬����ʱ��
			memcpy( v_szBuf + dw, v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3746 + byt, dwTemp );
			dw += dwTemp;
		}
#endif

#if USE_DRVRECRPT == 11
		// ������С
		dwTemp = sizeof(tagSampleData_3745_KJ2);

		// ���뷢�ͻ���
		if( byt > 0 )
		{
			dwTemp -= offsetof(tagSampleData_3745_KJ2, m_szDriverNo); // �ǵ�һ��,ȥ������ʱ��
			memcpy( v_szBuf + dw, v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3745_KJ2[byt].m_szDriverNo,
				dwTemp );
			dw += dwTemp;
		}
		else
		{
			// ��һ���㺬����ʱ��
			memcpy( v_szBuf + dw, v_objDrvSampleDatas.m_uniSampleDatas.m_arySampleData_3745_KJ2 + byt, dwTemp );
			dw += dwTemp;
		}
#endif
	}

	v_szBuf[ dw ++ ] = char( v_objDrvSampleDatas.m_bytAvgSpeedCount ); // ƽ���ٶȸ���

	// ƽ���ٶ�
	for( byt = 0; byt < v_objDrvSampleDatas.m_bytAvgSpeedCount; byt ++ )
	{
		memcpy( v_szBuf + dw, v_objDrvSampleDatas.m_aryAvgSpeed, sizeof(v_objDrvSampleDatas.m_aryAvgSpeed[0]) );
		dw += sizeof(v_objDrvSampleDatas.m_aryAvgSpeed[0]);
	}
	
	v_dwLen = dw;

	return 0;
}

void CDrvRecRpt::_SndDrvrecWin(bool v_bReSnd)
{
	static BYTE sta_bytWinId = 0;
	
	int iResult = 0;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	DWORD dwLen = 0;
	char* pSndBuf = NULL;
	BYTE byt = 0;

	if( !v_bReSnd ) // ��������ش�(�����ϴ�һ���´���,�����յ�3705Ӧ��������ϴ�����֡)
	{
		for( byt = 0; byt < m_bytCurSndDrvRecFrameNum; byt ++ )
		{
			// ��ȡһ֡
			dwLen = m_aryCurSndDrvRecFrameInfo[byt][0];
			pSndBuf = (char*)(m_aryCurSndDrvRecFrameInfo[byt][1]);
			if( dwLen < 1 || !pSndBuf )
			{
				PRTMSG(MSG_ERR, "Snd 3745 Win, find a empty frame\n" );
				continue;
			}

			// ����ǰ��ʣ����Ϣ������ (���������,����Ϊ��ʱ�����ڵ�֡��ȷ��)
			if( m_bCurSndNeedAns ) // ����Ҫ�ɿ�����
			{
				pSndBuf[0] |= 0x42; // ��: �ɿ�����
				pSndBuf[0] |= sta_bytWinId ? 0x08 : 0; // ��: ���ڱ�־ID
				if( byt + 1 == m_bytCurSndDrvRecFrameNum ) // ���Ǵ������1֡
				{
					pSndBuf[0] |= 0x01; // ���: ĩ֡ҪӦ��
					sta_bytWinId = ~sta_bytWinId; // �������ڱ�־
				}

				pSndBuf[1] = m_bytCurSndDrvRecFrameNum; // ���ڴ�С
				pSndBuf[2] = byt; // ֡���
			}
			else
			{
				pSndBuf[0] |= 0x40;
				pSndBuf[1] = 0;
				pSndBuf[2] = 0;
			}

			// ���͸�֡
			iResult = g_objSms.MakeSmsFrame( pSndBuf, int(dwLen), 0x37, DRVRECFRAME_DATATYPE, buf, sizeof(buf), iBufLen, CVT_8TO7 );
			if( !iResult )
			{
				iResult = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV8 );
			}
		}

		// ����Ҫ�ɿ�����,�����ٷ�����һ֡
		if( m_bCurSndNeedAns && byt > 0 )
		{
			// ���ش���ʱ��
			_KillTimer(&g_objTimerMng, DRVRECWIN_REUPLOAD_TIMER );
			_SetTimer(&g_objTimerMng, DRVRECWIN_REUPLOAD_TIMER, DRVRECWIN_REUPLOAD_INTERVAL, G_TmReRptDrvRecWin);

			m_dwDrvCurWinFstSndTime = GetTickCount();
			m_bDrvCurWinChkGprs = false;
		}
		else
		{
			// ������Ͼ������ǰ���͵ļ�¼���� 
			_ClearCurSndDrvRecWin();

			// �ر��ش���ʱ��,�������
			_KillTimer(&g_objTimerMng, DRVRECWIN_REUPLOAD_TIMER );
		}
	}
	else // ����,�����ش�
	{
		if( m_bCurSndNeedAns && m_bytCurSndDrvRecFrameNum > 0 ) // ����Ҫ�ɿ�����,�ҵ�ǰ�����л���֡
		{
			if( m_bytCurSndDrvRecFrameNum < sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0])
				&& m_bytCurSndDrvRecFrameNum > 0 ) // �������
			{
				dwLen = m_aryCurSndDrvRecFrameInfo[m_bytCurSndDrvRecFrameNum - 1][0];
				pSndBuf = (char*)(m_aryCurSndDrvRecFrameInfo[m_bytCurSndDrvRecFrameNum - 1][1]);
				if( dwLen < 1 || !pSndBuf )
				{
					PRTMSG(MSG_ERR, "Resnd last 3745 of a win, but a empty frame\n" );
					return;
				}

				// ���͸ô������1֡
				iResult = g_objSms.MakeSmsFrame( pSndBuf, int(dwLen), 0x37, DRVRECFRAME_DATATYPE, buf, sizeof(buf), iBufLen, CVT_8TO7 );
				if( !iResult )
				{
					iResult = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV8 );
				}
			}

			// �ж��Ƿ�Ҫ�ײ�������
			DWORD dwCur = GetTickCount();
			if( !m_bDrvCurWinChkGprs && dwCur - m_dwDrvCurWinFstSndTime >= DRVWINSNDMAXPERIOD_FSTCHKGPRS ) // ���ô��ڵ�һ�μ��
			{
				g_objSock.LetComuChkGPRS();
				m_dwDrvCurWinFstSndTime = GetTickCount();
				m_bDrvCurWinChkGprs = true;
			}
			else if( m_bDrvCurWinChkGprs && dwCur - m_dwDrvCurWinFstSndTime >= DRVWINSNDMAXPERIOD_CHKGPRS ) // ���ô��ڷǵ�һ�μ��
			{
				g_objSock.LetComuChkGPRS();
				m_dwDrvCurWinFstSndTime = GetTickCount();
			}
		}
		else
		{
			// �ر��ش���ʱ��,�������
			_KillTimer(&g_objTimerMng, DRVRECWIN_REUPLOAD_TIMER );
		}
	}
}

void CDrvRecRpt::DealAccChg( BOOL v_blAccOn )
{
	if( !v_blAccOn )
	{
		char buf[100];
		buf[0] = 0x01;
		buf[1] = 0x00;
		strcat( buf, "ACC�Ͽ�" );
		DataPush(buf, strlen(buf), DEV_QIAN, DEV_DVR, LV2);
	}

	_BeginDrvRecSample( v_blAccOn );
}

// ������ʻ��¼����
// ǧ����Э��(�ڲ�ģ��Э��): ���ڴ�С��1����������ʱ��(2)+�������ڣ�2�����������ڣ�1������ACC ON����������ʱ��(2)+�������ڣ�2�����������ڣ�1������ACC OFF��
// KJ2Э��: ���ڴ�С��1�������������ڣ�2�����������ڣ�1������ACC ON�������������ڣ�2�����������ڣ�1������ACC OFF��
int CDrvRecRpt::Deal103e(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen )
{
	tag2QDriveRecCfg objDrvRecCfg, objDrvRecCfgBkp;
	tag103e req;
	int iRet = 0;
	int iBufLen = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytResType;

	// �������
	if( sizeof(tag103e) != v_dwDataLen )
	{
		iRet = ERR_DATA_INVALID;
		goto DEAL103e_FAILED;
	}

	// ��ȡԭʼ����
	iRet = GetSecCfg( &objDrvRecCfg, sizeof(objDrvRecCfg),
		offsetof(tagSecondCfg, m_uni2QDriveRecCfg.m_obj2QDriveRecCfg), sizeof(objDrvRecCfg) );
	if( iRet && ERR_IO != iRet ) goto DEAL103e_FAILED;
	else iRet = 0; // ������������
	objDrvRecCfgBkp = objDrvRecCfg; // �ȱ���

	// ��֡,��������
	memcpy( &req, v_szData, sizeof(req) );
	if( 0x7f == req.m_bytSndWinSize ) req.m_bytSndWinSize = 0;
	if( 0x7f == req.m_bytOnSampCycleMin ) req.m_bytOnSampCycleMin = 0;
	if( 0x7f == req.m_bytOnSampCycleSec ) req.m_bytOnSampCycleSec = 0;
	if( 0x7f == req.m_bytOnSndCycle ) req.m_bytOnSndCycle = 0;
	if( 0x7f == req.m_bytOffSampCycleMin ) req.m_bytOffSampCycleMin = 0;
	if( 0x7f == req.m_bytOffSampCycleSec ) req.m_bytOffSampCycleSec = 0;
	if( 0x7f == req.m_bytOffSndCycle ) req.m_bytOffSndCycle = 0;

	objDrvRecCfg.m_bytSndWinSize = req.m_bytSndWinSize;
	objDrvRecCfg.m_bytOnSndCycle = req.m_bytOnSndCycle;
	objDrvRecCfg.m_bytOffSndCycle = req.m_bytOffSndCycle;
	objDrvRecCfg.m_wOnSampCycle = req.m_bytOnSampCycleMin * 60 + req.m_bytOnSampCycleSec;
	objDrvRecCfg.m_wOffSampCycle = req.m_bytOffSampCycleMin * 60 + req.m_bytOffSampCycleSec;

#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4
	if( 0x7f == req.m_bytOnSndHour ) req.m_bytOnSndHour = 0;
	if( 0x7f == req.m_bytOnSndMin ) req.m_bytOnSndMin = 0;
	if( 0x7f == req.m_bytOffSndHour ) req.m_bytOffSndHour = 0;
	if( 0x7f == req.m_bytOffSndMin ) req.m_bytOffSndMin = 0;

	objDrvRecCfg.m_wOnPeriod = WORD(req.m_bytOnSndHour) * 60 + req.m_bytOnSndMin;
	objDrvRecCfg.m_wOffPeriod = WORD(req.m_bytOffSndHour) * 60 + req.m_bytOffSndMin;
	
	if( 0 == objDrvRecCfg.m_wOnPeriod ) objDrvRecCfg.m_wOnPeriod = 0xffff; // ��ʾ�����ϱ�
	if( 0 == objDrvRecCfg.m_wOffPeriod ) objDrvRecCfg.m_wOffPeriod = 0xffff;
#endif

#if USE_DRVRECRPT == 11
	objDrvRecCfg.m_wOffPeriod = objDrvRecCfg.m_wOnPeriod = 0xffff; // ��ʾ�����ϱ�
#endif

	if( objDrvRecCfg.m_bytSndWinSize > 32
		|| objDrvRecCfg.m_bytSndWinSize > sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0]) )
		// �����ʹ��ڳ���Э�鶨��򻺴�����Χ
	{
		iRet = ERR_DATA_INVALID;
		goto DEAL103e_FAILED;
	}

	// ��֡
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x7e, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// ����������
		iRet = SetSecCfg( &objDrvRecCfg, offsetof(tagSecondCfg, m_uni2QDriveRecCfg.m_obj2QDriveRecCfg), sizeof(objDrvRecCfg) );
		if( iRet ) goto DEAL103e_FAILED;

		// ����Ӧ��
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
		if( !iRet )
		{
			// ʹ���µ����ô�����ʻ��¼
			_BeginDrvRecSample( (BOOL)(g_objQianIO.GetDeviceSta() & DEVSTA_ACCVALID) );
		}
		else
		{
			// �ָ�ԭ����
			SetSecCfg( &objDrvRecCfgBkp, offsetof(tagSecondCfg, m_uni2QDriveRecCfg.m_obj2QDriveRecCfg), sizeof(objDrvRecCfgBkp) );
			
			goto DEAL103e_FAILED;
		}
	}
	else goto DEAL103e_FAILED;

	return 0;

DEAL103e_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x7e, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );

	return iRet;
}

// ��ʻ�����ϴ�Ӧ��: Ӧ�����ͣ�1����֡������루4��
int CDrvRecRpt::Deal3705(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen )
{
	BYTE byt = 0;

	if( v_dwDataLen < 1 ) return ERR_PAR;

	if( 1 == v_szData[0] ) // ���ǳɹ�Ӧ��
	{
		// ������ɴ�������
		_ClearCurSndDrvRecWin();

		// �ټ����ϴ�
		DrvRecToSnd(0);

		return 0;
	}
	else if( 0 == v_szData[0] ) // ������Ҫ�ش�Ӧ��
	{
		if( v_dwDataLen < 5 ) return ERR_PAR;

		// ��ȡ�ش�֡�������
		DWORD dwRcvCode = 0;
		memcpy( &dwRcvCode, v_szData + 1, sizeof(dwRcvCode) );
		dwRcvCode = ntohl( dwRcvCode );

		// Ѱ����Ҫ�����һ֡���
		DWORD dwSymb = 1;
		BYTE bytLstFrameNo = 0xff;
		for( byt = 0; byt < sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0])
			&& byt < m_bytCurSndDrvRecFrameNum; byt ++ )
		{
			if( dwSymb & dwRcvCode ) // ������ŵ�֡δ�ɹ�����
				bytLstFrameNo = byt;
			dwSymb = dwSymb << 1;
		}
		if( bytLstFrameNo >= m_bytCurSndDrvRecFrameNum ) return ERR_PAR;

		// �ش�����
		char buf[ SOCK_MAXSIZE ];
		int iBufLen = 0;
		int iResult = 0;
		DWORD dwLen = 0;
		char* pSndBuf = NULL;
		char cTemp = 0;
		dwSymb = 1;
		for( byt = 0; byt < sizeof(m_aryCurSndDrvRecFrameInfo) / sizeof(m_aryCurSndDrvRecFrameInfo[0])
			&& byt < m_bytCurSndDrvRecFrameNum; byt ++ )
		{
			if( dwSymb & dwRcvCode ) // ������ŵ�֡δ�ɹ�����
			{
				dwLen = m_aryCurSndDrvRecFrameInfo[byt][0];
				pSndBuf = (char*)(m_aryCurSndDrvRecFrameInfo[byt][1]);

				if( dwLen < 1 || !pSndBuf )
				{
					PRTMSG(MSG_ERR, "Deal3705,resend 3745 frame,find an empty frame\n" );
					continue;
				}

				// ��������һ֡,����ǰ��һ���ֽڵ�Ӧ���־λ�޸�ΪҪӦ��
				if( bytLstFrameNo == byt )
				{
					cTemp = pSndBuf[0];
					pSndBuf[0] |= 1;
				}

				iResult = g_objSms.MakeSmsFrame( pSndBuf, int(dwLen), 0x37, DRVRECFRAME_DATATYPE,
					buf, sizeof(buf), iBufLen, CVT_8TO7 );
				if( !iResult ) iResult = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV8 );

				// ��������һ֡,��������һ���ֽ�Ҫ�ָ�Ϊԭֵ
				if( bytLstFrameNo == byt )
				{
					pSndBuf[0] = cTemp;
				}
			}

			dwSymb = dwSymb << 1;
		}

		return iResult;
	}
	else
		return ERR_PAR;
}

#endif //#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4 || USE_DRVRECRPT == 11


