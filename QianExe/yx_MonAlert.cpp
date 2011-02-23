#include "yx_QianStdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-MonAlert ")

void G_TmMon(void *arg, int len)
{
	g_objMonAlert.P_TmMon();
}

void G_TmAlertInvalid(void *arg, int len)
{
	g_objMonAlert.P_TmAlertInvalid();
}

void G_TmAlertSilent(void *arg, int len)
{
	g_objMonAlert.P_TmAlertSilent();
}

void G_TmAutoReqLsn(void *arg, int len)
{
	g_objMonAlert.P_TmAutoReqLsn();
}

void G_TmAlertInitQuery(void *arg, int len)
{
	g_objMonAlert.P_TmAlertInitQuery();
}

void G_TmAlertTest(void *arg, int len)
{
	g_objMonAlert.P_TmAlertTest();
}

//////////////////////////////////////////////////////////////////////////
CMonAlert::CMonAlert()
{
	m_dwAlertSymb = 0;
	
	m_dwAlertFootTm = 0;
	m_dwAlertPowerOffTm = 0;
	m_dwAlertBrownoutTm = 0;
	m_dwAlertFrontDoorTm = 0;
	m_dwAlertBackDoorTm = 0;
	m_dwAlertOverTurnTm = 0;
	m_dwAlertBumpTm = 0;
	
	m_bytAutoReqLsnCount = 0;
	m_bytAutoReqLsnContBusyCt = 0;
	memset( m_szAutoReqLsnTel, 0, sizeof(m_szAutoReqLsnTel) );
	
	m_dSpdAlermMinSpd = 9999999;
	m_bytSpdAlermPrid = 255;
	m_bytSpdAlermSta = 0; // �����ʼ��Ϊfalse
}

CMonAlert::~CMonAlert()
{

}

void CMonAlert::P_TmMon()
{
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	int iResult = 0;
	
	/// ��û������ݺ���Ϣ
	DWORD dwCur = GetTickCount();
	DWORD dwIntervChk = dwCur - m_objMonStatus.m_dwLstChkTime; // ���ϴζ�ʱ����ʱ��
	tag2QGprsCfg objGprsCfg;
	tagGPSData objGps(0);
	bool bNeedSend = false;

	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg) ,sizeof(objGprsCfg) );
	
	// �ݼ�ʣ����ʱ��
	if( INVALID_NUM_32 != m_objMonStatus.m_dwMonTime )
	{
		if( m_objMonStatus.m_dwMonTime >= dwIntervChk )
			m_objMonStatus.m_dwMonTime -= dwIntervChk;
		else
			m_objMonStatus.m_dwMonTime = 0;
	}
	
	// �޸��ϴμ��ʱ��,�Ա�����ļ��ʱ�䵽�ж�
	m_objMonStatus.m_dwLstChkTime = dwCur;
	
	// ��ȡ��ǰGPS��Ϣ
	iResult = GetCurGPS( objGps, true );
	if( iResult ) goto MONTIMER_END;
	
	/// �ж��Ƿ�Ҫ���ͼ������
	if( m_objMonStatus.m_bInterv )
	{
		DWORD dwIntervSend = dwCur - m_objMonStatus.m_dwLstSendTime; // ���ϴη��ͼ�����ݵ�ʱ��
		if( dwIntervSend >= m_objMonStatus.m_dwMonInterv ) // ����ؼ����
		{
			bNeedSend = true;
		}
	}
	if( m_objMonStatus.m_bSpace && !bNeedSend )
	{
		m_objMonStatus.m_dLstSendDis += G_CalGeoDis2(m_objMonStatus.m_dLstLon, m_objMonStatus.m_dLstLat,
			objGps.longitude, objGps.latitude);
		m_objMonStatus.m_dLstLon = objGps.longitude;
		m_objMonStatus.m_dLstLat = objGps.latitude;
		
		if( m_objMonStatus.m_dLstSendDis >= double( m_objMonStatus.m_dwSpace ) )
		{
			bNeedSend = true;
			m_objMonStatus.m_dLstSendDis = 0;
		}
	}
	
	/// ���ͼ������
	if( bNeedSend )
	{
		_Snd0145( objGps, objGprsCfg );
		
		// ֻҪ�����˼�����ݾ͸��� (��Ҫ)
		m_objMonStatus.m_dwLstSendTime = dwCur; // ʹ�������ж϶�ʱ
		m_objMonStatus.m_dLstSendDis = 0; // ʹ�������ж϶���
		m_objMonStatus.m_dLstLon = objGps.longitude;
		m_objMonStatus.m_dLstLat = objGps.latitude;
		
		// �ݼ�GPS���ݸ���
		if( m_objMonStatus.m_dwGpsCount > 0 ) -- m_objMonStatus.m_dwGpsCount;
	}
	
MONTIMER_END:
	// ��̨��ֹ����ж�
	if( 0 == m_objMonStatus.m_dwMonTime || 0 == m_objMonStatus.m_dwGpsCount ) // �����ʱ�䵽,�����ϴ�GPS��������
	{
		BYTE bytEndType = 0 == m_objMonStatus.m_dwGpsCount ? 0x05 : 0x01;
		
		iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, sizeof(bytEndType), 0x01, 0x44, buf, sizeof(buf), iBufLen);
		if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, // ���ȼ�Ӧ�ͼ������һ��
			(objGprsCfg.m_bytChannelBkType_1 & 0x40) ? DATASYMB_SMSBACKUP : 0 );
				
		_EndMon(); // Ҫ���ں��棬��Ϊ�����ڻ�������״̬������ǰ�浼�º������״̬��д�ġ���ֹ���ԭ�򡱲���ȷ
#if USE_LEDTYPE == 3
		//���LED���Ʊ�����Ϣ
		g_objKTLed.SetLedShow('A',0xff,0);
#endif
	}
	else
	{
		// Լÿ10��,��������һ�ζ�ʱ��
		static BYTE sta_bytCt = 0;
		if( 0 == sta_bytCt ++ % 10 )
		{
			if( m_objMonStatus.m_dwChkInterv < 1000 ) m_objMonStatus.m_dwChkInterv = 1000; // ��ȫ���
			_SetTimer(&g_objTimerMng, MON_TIMER, m_objMonStatus.m_dwChkInterv, G_TmMon );
		}
	}
}

void CMonAlert::P_TmAlertSilent()
{
	_KillTimer(&g_objTimerMng, ALERT_SILENT_TIMER );

	char cResult = 0;

	// �ж��Ƿ񻹴��ڶϵ籨��
	m_dwAlertSymb &= ~ALERT_TYPE_POWOFF;
	//if( g_objIO.QueryOneIOState(IOCTL_GPT_POWDET_STATE, cResult) )
	{
		if( 1 == cResult )
		{
			m_dwAlertSymb |= ALERT_TYPE_POWOFF;
		}
	}

	if( ALERT_TYPE_POWOFF & m_dwAlertSymb )
		SetAlertPowOff( true );
	else if( ALERT_TYPE_POWBROWNOUT & m_dwAlertSymb )
		SetAlertPowBrownout( true );
}

void CMonAlert::P_TmAlertInvalid()
{
	_ClrAlertSta();
}

void CMonAlert::P_TmAutoReqLsn()
{
	_KillTimer(&g_objTimerMng, AUTO_REQLSN_TIMER );
	
	// EVDO��TDģ��ʹ��USB��ͨ�ţ�����ͬʱ��绰�Ͳ���
	if (NETWORK_TYPE == NETWORK_EVDO || NETWORK_TYPE == NETWORK_TD || NETWORK_TYPE == NETWORK_WCDMA)
	{
		if( 0 != _AutoReqLsn() )
		{
			if( m_bytAutoReqLsnCount < AUTO_REQLSN_MAXTIMES ) // �������������δ���޶�����
				_SetTimer(&g_objTimerMng, AUTO_REQLSN_TIMER, AUTO_REQLSN_SHORTINTERVAL, G_TmAutoReqLsn );
		}
	}
	else if (NETWORK_TYPE == NETWORK_GSM)
	{
		if( g_objPhoto.IsPhotoEnd() // �����ս���
			&& // ��������������
				( 
				g_objPhoto.IsUploadPhotoEnd() // ��Ƭ�ϴ�����
				||
				g_objPhoto.IsUploadPhotoBreak() // ���ϴ���Ƭ����ж�
				||
				!g_objSock.IsOnline() // ��GPRSδ����δ��½
				)
			)
		{	
			if( 0 != _AutoReqLsn() )
			{
				if( m_bytAutoReqLsnCount < AUTO_REQLSN_MAXTIMES ) // �������������δ���޶�����
					_SetTimer(&g_objTimerMng, AUTO_REQLSN_TIMER, AUTO_REQLSN_SHORTINTERVAL, G_TmAutoReqLsn );
			}
		}
		else
			_SetTimer(&g_objTimerMng, AUTO_REQLSN_TIMER, 3000, G_TmAutoReqLsn );
	}
}

void CMonAlert::P_TmAlertTest()
{

}

void CMonAlert::P_TmAlertInitQuery()
{
	_KillTimer(&g_objTimerMng, ALERT_INITQUERY_TIMER );
}

void CMonAlert::P_TmAlertSpd()
{
}

int CMonAlert::_BeginMon( double v_dLon, double v_dLat )
{
	// GPSģ���ϵ�
 	g_objQianIO.StopAccChk();

	char buf = 0x04;
	DataPush(&buf, 1, DEV_QIAN, DEV_DIAODU, LV2);

	// �ر������ϱ�
#if USE_AUTORPT == 1
	g_objAutoRpt.EndAutoRpt();
#endif

	// ���¼������,�����ü�س�ʼ״̬
	m_objMonStatus.m_dwLstChkTime = GetTickCount();
	m_objMonStatus.m_dwLstSendTime = m_objMonStatus.m_dwLstChkTime;
	if( 0 == v_dLon || 0 == v_dLat )
	{
		tagGPSData objGps(0);
		GetCurGPS( objGps, true );
		m_objMonStatus.m_dLstLon = objGps.longitude;
		m_objMonStatus.m_dLstLat = objGps.latitude;
	}
	else
	{
		m_objMonStatus.m_dLstLon = v_dLon;
		m_objMonStatus.m_dLstLat = v_dLat;
	}
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	m_objMonStatus.m_dwGpsCount = 0xffffffff;
#endif	

	// ������ض�ʱ
	if( m_objMonStatus.m_bSpace ) m_objMonStatus.m_dwChkInterv = MON_MININTERVAL; // �ж���ʱ,�����Ĭ�ϼ�ؼ��
	else m_objMonStatus.m_dwChkInterv = m_objMonStatus.m_dwMonInterv;
	unsigned int uiTmId = _SetTimer(&g_objTimerMng, MON_TIMER, m_objMonStatus.m_dwChkInterv, G_TmMon );

	PRTMSG(MSG_NOR, "Mon Start, TmId = %d\n", uiTmId );

	return 0;
}

/// ����������ֹ��أ������������ع���ҲҪ���������
int CMonAlert::_EndMon()
{
	// �����ر�GPS��Դ���
	g_objQianIO.StartAccChk( 2000 );

	// �رռ�ض�ʱ
	m_objMonStatus.Init();
	_KillTimer(&g_objTimerMng, MON_TIMER );
	
	// ȡ������ʱ��ֹͨ��
	{
		char buf[2];
		buf[0] = (char)0xf3;
		buf[1] = 0;
		DataPush(buf, 2, DEV_QIAN, DEV_PHONE, LV2);
	}

	// �������״̬
	_ClrAlertSta();

	PRTMSG(MSG_NOR, "Mon Stop!\n" );

#if USE_AUTORPT == 1
	// �������������ϱ�
	g_objAutoRpt.Init();
#endif

	return 0;
}

int CMonAlert::_Snd0145( const tagGPSData &v_objGps, const tag2QGprsCfg &v_objGprsCfg, bool v_bFbidAllowSms /*=false*/ )
{
	int iBufLen = 0;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	tagQianGps objQianGps;
	tag0145 data;
	DWORD dwAlertSta = 0;

	PRTMSG(MSG_DBG, "_Send0145 Alerm data.\n");
	
	// �������б���״̬,���ɱ�������
	dwAlertSta = g_objMonAlert.GetAlertSta();
	//FillAlertSymb( dwAlertSta, data.m_bytAlertHi, data.m_bytAlertLo );
	FillAlertSymb(dwAlertSta, &(data.m_bytAlertHi), &(data.m_bytAlertLo));

	// ���ĸ�������1
	data.m_bytMonCentCount = 1;

	// gps����
	GpsToQianGps( v_objGps, objQianGps );
	memcpy( &(data.m_objQianGps), &objQianGps, sizeof(data.m_objQianGps) );
	
#if 2 == USE_PROTOCOL
	// ��������ֵ
	data.m_szAlertAreaCode[0] = 0;
	data.m_szAlertAreaCode[1] = (char)g_objDriveRecord.m_bytAreaCode;
#endif

	// SMS��װ
	iRet = g_objSms.MakeSmsFrame((char*)&data, sizeof(data), 0x01, 0x45, buf, sizeof(buf), iBufLen);
	if( iRet ) return iRet;

	// ����
	BYTE bytLvl = LV8, bytSndSymb = 0;
	if( !dwAlertSta )
	{
		bytLvl = LV11;
		bytSndSymb = 0;
	}
	else
	{
		bytLvl = LV13;
		bytSndSymb = DATASYMB_SMSBACKUP;
	}
	
	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, bytLvl, bytSndSymb );

	return iRet;
}

int CMonAlert::_Snd0149( const tagGPSData &v_objGps, const tag2QGprsCfg &v_objGprsCfg, bool v_bFbidAllowSms /*=false*/ )
{
	int iBufLen = 0;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	tagQianGps objQianGps;
	tag0149 data;
	DWORD dwAlertSta = 0;

	PRTMSG(MSG_DBG, "_Send0149 Alerm data.\n");
	
	// �������б���״̬,���ɱ�������
	dwAlertSta = g_objMonAlert.GetAlertSta();
	FillAlertSymbFor0149(dwAlertSta, &(data.m_bytAlertHi), &(data.m_bytAlertLo));

	// ���ĸ�������1
	data.m_bytMonCentCount = 1;

	// gps����
	GpsToQianGps( v_objGps, objQianGps );
	memcpy( &(data.m_objQianGps), &objQianGps, sizeof(data.m_objQianGps) );
	
	// SMS��װ
	iRet = g_objSms.MakeSmsFrame((char*)&data, sizeof(data), 0x01, 0x49, buf, sizeof(buf), iBufLen);
	if( iRet ) return iRet;

	// ����
	BYTE bytLvl = LV8, bytSndSymb = 0;
	if( !dwAlertSta )
	{
		bytLvl = LV11;
		bytSndSymb = 0;
	}
	else
	{
		bytLvl = LV13;
		bytSndSymb = DATASYMB_SMSBACKUP;
	}
	
	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, bytLvl, bytSndSymb );

	return iRet;
}

int CMonAlert::_SendOne0149Alert(const tagGPSData &v_objGps, byte v_bytAlertType)
{
	int iBufLen = 0;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	tagQianGps objQianGps;

	char szTempBuf[SOCK_MAXSIZE] = {0};
	int  iTempLen = 0;

	// ���ĸ���
	szTempBuf[iTempLen++] = 1;
	
	// gps����
	GpsToQianGps( v_objGps, objQianGps );
	memcpy( szTempBuf+iTempLen, &objQianGps, sizeof(objQianGps) );
	iTempLen += sizeof(objQianGps);

	// �汾��
	szTempBuf[iTempLen++] = 0x01;
	
	// ��չ��������
	szTempBuf[iTempLen++] = 0x01;

	// �������ͣ�������
	szTempBuf[iTempLen++] = 0x02;

	// ��������
	szTempBuf[iTempLen++] = 0x01;

	// ��������
	szTempBuf[iTempLen++] = v_bytAlertType;
	
	// SMS��װ
	iRet = g_objSms.MakeSmsFrame((char*)szTempBuf, iTempLen, 0x01, 0x49, buf, sizeof(buf), iBufLen);
	if( iRet ) return iRet;
	
	// ����
	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV13, 0 );
	return iRet;
}

void CMonAlert::Init()
{
	_SetTimer(&g_objTimerMng, ALERT_INITQUERY_TIMER, 5000, G_TmAlertInitQuery );
}

// ���ʺ������߳��е���
// ��Ҫ�����߳��л�ȡGPS����ֱ�ӵ���dll�ӿڡ���::GetCurGps
int CMonAlert::GetCurGPS( tagGPSData& v_objGps, bool v_bGetLstValid )
{
	bool bNeedSetBackGps = false;
	int iRet = GetCurGps( &v_objGps, sizeof(v_objGps), v_bGetLstValid ? GPS_LSTVALID : GPS_REAL );

#if USE_BLK == 1
	// ��1��GPS��Чʱ�ı��մ���ɾ����Ч����֮��ĺ�ϻ������
	static bool sta_bDelLaterBlk = false;
	if( !sta_bDelLaterBlk && 'A' == v_objGps.valid )
	{
		g_objBlk.DelLaterBlk( v_objGps );
		sta_bDelLaterBlk = true;
	}

	// ÿ600����Чʱ�ļ�⣺�����ڱ仯ʱ��ɾ��1��֮ǰ�ĺ�ϻ������
	if( 'A' == v_objGps.valid  )
	{
		static DWORD sta_dwGpsValidTime = 0;
		++ sta_dwGpsValidTime;

		if( 599 == sta_dwGpsValidTime % 600 ) // ÿ600��
		{
			static byte bytDay = 0;

			struct tm pCurrentTime;
			G_GetTime(&pCurrentTime);

			if( bytDay != pCurrentTime.tm_mday ) // �����ڱ仯
			{
				g_objBlk.DelOlderBlk( v_objGps );
				bytDay = pCurrentTime.tm_mday;
			}
		}
	}
	
	static bool sta_bGetBlk = false;
	if( !sta_bGetBlk && v_bGetLstValid && (0 == v_objGps.longitude || 0 == v_objGps.latitude) )
	{
		// ����ȡ��GPS������ȫ��Ч(ȫ0),������ʹ���ϴ���Ч���ݴ���,���ȡ���һ����ϻ������
		tagGPSData objGps(0);
		iRet = g_objBlk.GetLatestBlkGps( objGps );
		if( !iRet )
		{
			v_objGps = objGps;
			bNeedSetBackGps = true;
		}
		
		sta_bGetBlk = true; // �����Ƿ������ϻ�����ݣ���ֻ��1��
	}

#endif

#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4 || USE_DRVRECRPT == 11|| USE_DRVRECRPT == 0
	static bool sta_bGetDrvRec = false;
	if( !sta_bGetDrvRec && v_bGetLstValid && (0 == v_objGps.longitude || 0 == v_objGps.latitude) )
	{
		double dLichen;
		long lLon = 0, lLat = 0;
		g_objCarSta.GetLstValidLonlatAndLichen( lLon, lLat, dLichen );
		v_objGps.longitude = double(lLon) / LONLAT_DO_TO_L;
		v_objGps.latitude = double(lLat) / LONLAT_DO_TO_L;
		v_objGps.speed = 0;
		v_objGps.direction = 0;
		v_objGps.valid = 'V';
		
		bNeedSetBackGps = true;
		
		sta_bGetDrvRec = true;
	}
#endif

	if( bNeedSetBackGps )
	{
		SetCurGps( &v_objGps, GPS_REAL | GPS_LSTVALID );
		GetCurGps( &v_objGps, sizeof(v_objGps), GPS_LSTVALID );
	}

	// ACC��Ч����GPS��Чʱ���ٶ�ǿ��Ϊ0
	if( 'A' !=v_objGps.valid )
	{
		v_objGps.speed = 0;
	}

	return 0; // �����Ƿ�ʧ�ܣ������سɹ�����Ϊ�еĵط������GPSʧ�ܣ�������ʧ��Ӧ���ƺ����ã�������Ӧ��
}

int CMonAlert::GetCurQianGps( tagQianGps &v_objQianGps, bool v_bGetLstValid )
{
	tagGPSData gps(0);
	int iResult = GetCurGPS( gps, v_bGetLstValid );
	if( iResult )
	{
		memset( (void*)&v_objQianGps, 0, sizeof(v_objQianGps) );
		v_objQianGps.m_bytMix = 0x60;
		return iResult;
	}

	GpsToQianGps( gps, v_objQianGps );
	return 0; // �����Ƿ�ʧ�ܣ������سɹ�����Ϊ�еĵط������GPSʧ�ܣ�������ʧ��Ӧ���ƺ�����
}

void CMonAlert::GpsToQianGps( const tagGPSData &v_objGps, tagQianGps &v_objQianGps )
{
	v_objQianGps.m_bytGpsType = 0x42; // ��׼GPS����

// ǧ����ͨ�ð�,������_����
// 01ASCLGV
// A��GPS������Чλ��0-��Ч��1-��Ч
// L����̨����״̬��0���ճ���1���س�
// S��ʡ��ģʽ��0����ʡ��ģʽ��1��ʡ��ģʽ
// C: ACC״̬�� 0��ACC OFF�� 1 �� ACC ON
// G�������ֶ��ж��ֽڵ����λ
// V���ٶ��ֶ����λ


// ǧ���۹�����_��һ
// �ֽڶ���Ϊ��01A0TTGV
// A��GPS������Чλ��0-��Ч��1-��Ч
// G�������ֶ��ж��ֽڵ����λ
// V���ٶ��ֶ����λ
// TT�����س���ͣҵ״̬ 01-�ճ�  10-�س� 11-����

	v_objQianGps.m_bytMix = 0x40;

	// �ճ�/�س�
	DWORD dwVeSta = g_objCarSta.GetVeSta();
	if( dwVeSta & VESTA_HEAVY )
	{
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
		v_objQianGps.m_bytMix |= 0x04;
#endif
#if 2 == USE_PROTOCOL
		v_objQianGps.m_bytMix &= 0xf3;
		v_objQianGps.m_bytMix |= 0x08;
#endif
	}
	else
	{
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
		v_objQianGps.m_bytMix &= 0xfb;
#endif
#if 2 == USE_PROTOCOL
		v_objQianGps.m_bytMix &= 0xf3;
		v_objQianGps.m_bytMix |= 0x04;
#endif
	}
		
	// ʡ��ģʽ
	if( g_objQianIO.GetDeviceSta() & DEVSTA_SYSTEMSLEEP )
		v_objQianGps.m_bytMix |= 0x10;
	else
		v_objQianGps.m_bytMix &= 0xef;

	// ACC״̬
	byte bytSta = 0;
	IOGet(IOS_ACC, &bytSta);
	if( bytSta == IO_ACC_ON )
		v_objQianGps.m_bytMix |= 0x08;	
	else
		v_objQianGps.m_bytMix &= 0xf7;


	// GPS�Ƿ���Ч
	if( 'A' == v_objGps.valid ) v_objQianGps.m_bytMix &= 0xdf;
	else v_objQianGps.m_bytMix |= 0x20;

	v_objQianGps.m_bytDay = BYTE(v_objGps.nDay);
	v_objQianGps.m_bytMon = BYTE(v_objGps.nMonth);
	v_objQianGps.m_bytYear = BYTE(v_objGps.nYear % 1000);
	v_objQianGps.m_bytSec = BYTE(v_objGps.nSecond);
	v_objQianGps.m_bytMin = BYTE(v_objGps.nMinute);
	v_objQianGps.m_bytHour = BYTE(v_objGps.nHour);
	v_objQianGps.m_bytDir = BYTE(v_objGps.direction / 3 + 1);

	// ����ʱ��->�������� (������Сʱ�ı�)
	if( v_objQianGps.m_bytHour < 8 ) 
		v_objQianGps.m_bytHour += 24;

	v_objQianGps.m_bytHour -= 8;

	// ԭʼγ��
	v_objQianGps.m_bytLatDu = BYTE(v_objGps.latitude);
	double dLatFen = ( v_objGps.latitude - v_objQianGps.m_bytLatDu ) * 60;
	v_objQianGps.m_bytLatFenzen = BYTE(dLatFen);
	WORD wLatFenxiao = WORD( (dLatFen - v_objQianGps.m_bytLatFenzen) * 10000 );
	v_objQianGps.m_bytLatFenxiao1 = wLatFenxiao / 100;
	v_objQianGps.m_bytLatFenxiao2 = wLatFenxiao % 100;

	// ԭʼ����
	v_objQianGps.m_bytLonDu = BYTE(v_objGps.longitude);
	double dLonFen = (v_objGps.longitude - v_objQianGps.m_bytLonDu) * 60;
	v_objQianGps.m_bytLonFenzen = BYTE(dLonFen);
	WORD wLonFenxiao = WORD( (dLonFen - v_objQianGps.m_bytLonFenzen) * 10000 );
	v_objQianGps.m_bytLonFenxiao1 = wLonFenxiao / 100;
	v_objQianGps.m_bytLonFenxiao2 = wLonFenxiao % 100;

	// ԭʼ�ٶ�
	double dSpeed = v_objGps.speed * 3.6 / 1.852; // ת��Ϊ����/Сʱ
	v_objQianGps.m_bytSpeed = BYTE( dSpeed );

	// У���
	{
		BYTE bytSum = 0;
		char szTemp[64];
		int i;

		// γ��
		int iTemp = int( v_objQianGps.m_bytLatDu );
		sprintf( szTemp, "%02d", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		// γ��������
		iTemp = int( v_objQianGps.m_bytLatFenzen );
		sprintf( szTemp, "%02d.", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		// γ��С����_1
		iTemp = int( v_objQianGps.m_bytLatFenxiao1 );
		sprintf( szTemp, "%02d", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		// γ��С����_2
		iTemp = int( v_objQianGps.m_bytLatFenxiao2 );
		sprintf( szTemp, "%02d", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		// ����
		iTemp = int( v_objQianGps.m_bytLonDu );
		sprintf( szTemp, "%03d", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		// ����������
		iTemp = int( v_objQianGps.m_bytLonFenzen );
		sprintf( szTemp, "%02d.", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		// ����С����_1
		iTemp = int( v_objQianGps.m_bytLonFenxiao1 );
		sprintf( szTemp, "%02d", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		// ����С����_2
		iTemp = int( v_objQianGps.m_bytLonFenxiao2 );
		sprintf( szTemp, "%02d", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		// �ٶ�
		iTemp = int( v_objQianGps.m_bytSpeed );
		sprintf( szTemp, "%03d.0", iTemp );
		for( i = 0; i < int(strlen(szTemp)); i++ ) bytSum += BYTE( szTemp[i] );

		bytSum &= 0x7f;
		if( 0 == bytSum ) bytSum = 0x7f;
		v_objQianGps.m_bytChk = bytSum;
	}

	// ת��
	if( 0 == v_objQianGps.m_bytLatDu ) v_objQianGps.m_bytLatDu = 0x7f;
	if( 0 == v_objQianGps.m_bytLatFenzen ) v_objQianGps.m_bytLatFenzen = 0x7f;
	if( 0 == v_objQianGps.m_bytLatFenxiao1 ) v_objQianGps.m_bytLatFenxiao1 = 0x7f;
	if( 0 == v_objQianGps.m_bytLatFenxiao2 ) v_objQianGps.m_bytLatFenxiao2 = 0x7f;
	if( 0 == v_objQianGps.m_bytLonFenzen ) v_objQianGps.m_bytLonFenzen = 0x7f;
	if( 0 == v_objQianGps.m_bytLonFenxiao1 ) v_objQianGps.m_bytLonFenxiao1 = 0x7f;
	if( 0 == v_objQianGps.m_bytLonFenxiao2 ) v_objQianGps.m_bytLonFenxiao2 = 0x7f;
	if( 0 == v_objQianGps.m_bytLonDu ) v_objQianGps.m_bytLonDu = 0xff;
	else if( 0x80 == v_objQianGps.m_bytLonDu ) v_objQianGps.m_bytLonDu = 0xfe;
	if( v_objQianGps.m_bytLonDu & 0x80 ) v_objQianGps.m_bytMix |= 0x02;
	else v_objQianGps.m_bytMix &= 0xfd;
	v_objQianGps.m_bytLonDu &= 0x7f;
	if( 0 == v_objQianGps.m_bytSpeed ) v_objQianGps.m_bytSpeed = 0xff;
	else if( 0x80 == v_objQianGps.m_bytSpeed ) v_objQianGps.m_bytSpeed = 0xfe;
	if( v_objQianGps.m_bytSpeed & 0x80 ) v_objQianGps.m_bytMix |= 0x01;
	else v_objQianGps.m_bytMix &= 0xfe;
	v_objQianGps.m_bytSpeed &= 0x7f;
}

//���貹�䣩
int CMonAlert::DealPhoneAutoReqLsnRes( BYTE v_bytResType )
{
}

/// ִ�б������� (ҵ��������) (�������ظ�����,ֻ���±����Ż���ñ�����)
int CMonAlert::_DealAlert( bool v_bReqLsn, bool v_bSilentTimer )
{
	int iRet = 0;
	
	if (NETWORK_TYPE != NETWORK_CDMA)
	{
		// ����CDMA��̨�������������ܣ���Ϊ�ᵼ��һֱ���ͱ�������
		// ������������
		if( v_bReqLsn )
		{
			_BeginAutoReqLsn( ALERTREQLSN_DEALYPERIOD ); // ��������������,�Ծ�����֤��һ�����������ܷ��ͳ�ȥ
		}
	}

	// �򿪱�����Ĭ��ʱ
	if( v_bSilentTimer )
		_SetTimer(&g_objTimerMng, ALERT_SILENT_TIMER, ALERT_SILENT_PERIOD, G_TmAlertSilent );
		
	// ���������״̬�Ķ�ʱ
	_KillTimer(&g_objTimerMng, ALERT_INVALID_TIMER );
	_SetTimer(&g_objTimerMng, ALERT_INVALID_TIMER, ALERT_INVALID_PERIOD, G_TmAlertInvalid );
	
	// �޸Ķ�ʱ��ز���
	if( !m_objMonStatus.m_bInterv || m_objMonStatus.m_dwMonInterv > ALERT_SNDDATA_INTERVAL )
		m_objMonStatus.m_dwMonInterv = ALERT_SNDDATA_INTERVAL;
	m_objMonStatus.m_bInterv = true;
	if( ALERT_SNDDATA_PERIOD > m_objMonStatus.m_dwMonTime + 2000 )
	{
		m_objMonStatus.m_dwMonTime = ALERT_SNDDATA_PERIOD;
		m_objMonStatus.m_bAlertAuto = true; // ���Ϊ�������������ļ��
	}

	// ���̷��͵�һ����������
	tagGPSData objGps(0);
	tag2QGprsCfg objGprsCfg;
	GetCurGPS( objGps, true );
	objGprsCfg.Init();
 	::GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),
 		offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );
	_Snd0145( objGps, objGprsCfg, true );

	// ��(����)��ض�ʱ
	_BeginMon( 0, 0 );

	return iRet;
}

void CMonAlert::_ClrAlertSta()
{
	// �ر� "�������״̬"��ʱ
//	m_dwAlertSymb = 0;
	m_dwAlertSymb &= 0x00000808;//ǰ����ֻ���ڹس���ʱ���������״̬
	m_objMonStatus.m_bAlertAuto = false;
	_KillTimer(&g_objTimerMng, ALERT_INVALID_TIMER );

	// �ر�����������ʱ
	_KillTimer(&g_objTimerMng, AUTO_REQLSN_TIMER );

#if USE_LEDTYPE == 1	
	char cCmd = LED_CMD_ALERMCANCEL;
	g_objLedBohai.m_objLocalCmdQue.push( (unsigned char*)&cCmd, 1 );
#endif

#if USE_LEDTYPE == 2
	g_objLedChuangLi.DealEvent(0x04, 'A', 0);
#endif

	// ָʾ������
	char szbuf[10] = {0};
	szbuf[0] = 0x02;	// 0x02��ʾ����ָʾ��
	szbuf[1] = 0x06;	// �������ٱ���֪ͨ
	DataPush(szbuf, 2, DEV_QIAN, DEV_DIAODU, LV2);
}

/// ��/�ر����ٱ���
int CMonAlert::SetAlertFoot( const bool v_bSet )
{
	int iRet = 0;
	DWORD dwCur = GetTickCount();

	tag2QAlertCfg objAlertCfg;
	objAlertCfg.Init();
 	GetSecCfg( &objAlertCfg, sizeof(objAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );

	if( v_bSet && !(objAlertCfg.m_bytAlert_1 & 0x20) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_FOOT;
		PRTMSG(MSG_NOR, "Deal FootAlerm, but this func is Disabled! Exit!\n" );
		return 0;
	}

	// ע���ʼ��ֵ
	BYTE bytAlertDealType = 0; // 1,Ҫ�´�����; 2,Ҫ�ر����б�������

	// (ע�⴦��ʱ��˳��)
	if( v_bSet ) // �������ñ���
	{
		if( !(ALERT_TYPE_FOOT & m_dwAlertSymb) ) // ��֮ǰû�иñ���״̬
		{
			bytAlertDealType = 1; // Ҫ�´���ñ���
			
#if USE_LEDTYPE == 3
			//LED ������ʾ����
			g_objKTLed.SetLedShow('A',0,1);
#endif
		}
		else // ���Ѿ��иñ���״̬
		{
			if( dwCur - m_dwAlertFootTm > ALERT_SNDDATA_PERIOD ) // �����������ʱ��
			{
				bytAlertDealType = 1; // Ҫ�´���ñ���
			}
		}

		m_dwAlertSymb |= ALERT_TYPE_FOOT; // ���øñ���״̬
	}

	// �±�������
	if( 1 == bytAlertDealType ) // ��Ҫ�´�����
	{
		PRTMSG(MSG_NOR, "Start FootAlerm\n" );
		
		char buf[16] = { 0 };

		//���ӱ���ʱ��ֹͨ��
		buf[0] = (char) 0xf3;
		buf[1] = (char) ALERT_STOP_PHONE_PRD;
		DataPush(buf, 2, DEV_QIAN, DEV_PHONE, LV2);
		PRTMSG(MSG_NOR, "Deal Foot Alerm,Req Comu handoff phone,and Fbid Phone in %d s\n", ALERT_STOP_PHONE_PRD );

#if USE_LEDTYPE == 1
		char cCmd = LED_CMD_ALERM;
		g_objLedBohai.m_objLocalCmdQue.push( (unsigned char*)&cCmd, 1 );
#endif

#if USE_LEDTYPE == 2
		g_objLedChuangLi.DealEvent(0x03, 'A', 0);
#endif

		// ͳһ��������
		iRet = _DealAlert( true, false ); // (���ϴ��̶�,���ٱ���֮��,��������ٱ�����ƽ)
		
		// ���±�����ʼʱ��
		m_dwAlertFootTm = dwCur;

		// ���ٱ���ʱ��Ƶ�����ϴ�
		char szbuf[100];
// 		int  iBufLen = 0;
// 		byte bytType = 0x00;		// ���ٱ���ʱ����Ƶ�����ϴ�����Ϊ0x00 
// 		int iResult = g_objSms.MakeSmsFrame((char*)&bytType, 1, 0x38, 0x46, szbuf, sizeof(szbuf), iBufLen);
// 		if( !iResult ) g_objSock.SOCKSNDSMSDATA( szbuf, iBufLen, LV12, 0 );

		// ָʾ�ƿ���
		szbuf[0] = 0x02;	// 0x02��ʾ����ָʾ��
		szbuf[1] = 0x05;	// �̵ƿ�����160VC), �� ���ٱ���֪ͨ��V6-60)
		DataPush(szbuf, 2, DEV_QIAN, DEV_DIAODU, LV2);

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
 		g_objPhoto.AddPhoto( PHOTOEVT_FOOTALERM );
#endif
	}

	return 0;
}

/// ��/�رնϵ籨��
int CMonAlert::SetAlertPowOff( const bool v_bSet )
{
	int iRet = 0;
	DWORD dwCur = GetTickCount();

	tag2QAlertCfg objAlertCfg;
 	GetSecCfg( &objAlertCfg, sizeof(objAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );

	if( v_bSet && !(objAlertCfg.m_bytAlert_1 & 0x04) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_POWOFF;
		PRTMSG(MSG_NOR, "Deal Powoff Alerm,but this func is Disabled! Exit!\n" );
		return 0;
	}

	// ע���ʼ��ֵ
	BYTE bytAlertDealType = 0; // 1,Ҫ�´�����; 2,Ҫ�ر����б�������

	// (ע�⴦��ʱ��˳��)
	if( v_bSet ) // �������ñ���
	{
		if( !(ALERT_TYPE_POWOFF & m_dwAlertSymb) ) // ��֮ǰû�иñ���״̬
		{
			bytAlertDealType = 1; // Ҫ�´���ñ���
		}
		else // ���Ѿ��иñ���״̬
		{
			if( dwCur - m_dwAlertPowerOffTm > ALERT_SNDDATA_PERIOD ) // �����������ʱ��
			{
				bytAlertDealType = 1; // Ҫ�´���ñ���
			}
		}

		m_dwAlertSymb |= ALERT_TYPE_POWOFF; // ���øñ���״̬
	}
	else // �����������
	{
		PRTMSG(MSG_NOR, "Stop Powoff Alerm!\n" );
		
		if( m_dwAlertSymb & ALERT_TYPE_POWOFF )
		{
			m_dwAlertSymb &= ~ALERT_TYPE_POWOFF; // ����ñ���״̬
			
			// ���ùرռ�ض�ʱ,��Ϊ���ܱ�������,��ػ�����
			// ���ô������ϱ���ʱ,��Ϊ���ֹͣʱ��������ϱ���ʱ

			if( !m_dwAlertSymb ) // �������û���κα���
				bytAlertDealType = 2; // Ҫ�رձ�������
		}
	}

	// �±�������
	if( 1 == bytAlertDealType ) // ��Ҫ�´�����
	{	
		PRTMSG(MSG_NOR, "Start Powoff Alerm!\n" );

		// ͳһ��������
		iRet = _DealAlert( false, true );

		// ���±�����ʼʱ��
		m_dwAlertPowerOffTm = dwCur;
	}
	else if( 2 == bytAlertDealType ) // ��Ҫ�ر����б���
	{		
		if( m_objMonStatus.m_bAlertAuto )
		{
			_EndMon(); // ���Ǳ���������,�رռ��

			char buf[ SOCK_MAXSIZE ];
			int iBufLen = 0;
			BYTE bytEndType = 0x04; // ��Ϊ�����ź���ֹ
			int iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, 1, 0x01, 0x44, buf, sizeof(buf), iBufLen);
			if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
		}
	}

	return 0;
}

int CMonAlert::SetAlertPowBrownout( const bool v_bSet )
{
	int iRet = 0;
	DWORD dwCur = GetTickCount();

	tag2QAlertCfg objAlertCfg;
 	GetSecCfg( &objAlertCfg, sizeof(objAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );

	if( v_bSet && !(objAlertCfg.m_bytAlert_1 & 0x40) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_POWBROWNOUT;
		PRTMSG(MSG_NOR, "Deal Pow Brownout Alerm, but this func is Disabled! Exit!\n" );
		return 0;
	}

	// ע���ʼ��ֵ
	BYTE bytAlertDealType = 0; // 1,Ҫ�´�����; 2,Ҫ�ر����б�������

	// (ע�⴦��ʱ��˳��)
	if( v_bSet ) // �������ñ���
	{
		if( !(ALERT_TYPE_POWBROWNOUT & m_dwAlertSymb) ) // ��֮ǰû�иñ���״̬
		{
			bytAlertDealType = 1; // Ҫ�´���ñ���
		}
		else // ���Ѿ��иñ���״̬
		{
			if( dwCur - m_dwAlertPowerOffTm > ALERT_SNDDATA_PERIOD ) // �����������ʱ��
			{
				bytAlertDealType = 1; // Ҫ�´���ñ���
			}
		}

		m_dwAlertSymb |= ALERT_TYPE_POWBROWNOUT; // ���øñ���״̬
	}
	else // �����������
	{
		PRTMSG(MSG_NOR, "Stop Pow Brownout Alerm!\n" );
		
		if( m_dwAlertSymb & ALERT_TYPE_POWBROWNOUT )
		{
			m_dwAlertSymb &= ~ALERT_TYPE_POWBROWNOUT; // ����ñ���״̬
			
			// ���ùرռ�ض�ʱ,��Ϊ���ܱ�������,��ػ�����
			// ���ô������ϱ���ʱ,��Ϊ���ֹͣʱ��������ϱ���ʱ

			if( !m_dwAlertSymb ) // �������û���κα���
				bytAlertDealType = 2; // Ҫ�رձ�������
		}
	}

	// �±�������
	if( 1 == bytAlertDealType ) // ��Ҫ�´�����
	{	
		PRTMSG(MSG_NOR, "Start Pow Brownout Alerm!\n" );

		// ͳһ��������
		iRet = _DealAlert( false, true );

		// ���±�����ʼʱ��
		m_dwAlertPowerOffTm = dwCur;
	}
	else if( 2 == bytAlertDealType ) // ��Ҫ�ر����б���
	{		
		// ��ʱ���鷢�ֱ����źŲ�����ʱ��Ȼ��ر� _KillTimer(&g_objTimerMng, ALERT_SILENT_TIMER ); // �رվ�Ĭ������ʱ��

		if( m_objMonStatus.m_bAlertAuto )
		{
			_EndMon(); // ���Ǳ���������,�رռ��

			char buf[ SOCK_MAXSIZE ];
			int iBufLen = 0;
			BYTE bytEndType = 0x04; // ��Ϊ�����ź���ֹ
			int iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, 1, 0x01, 0x44, buf, sizeof(buf), iBufLen);
			if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
		}
	}

	return 0;
}

/// ��/�رշǷ���ǰ���ű���
int CMonAlert::SetAlertFrontDoor( const bool v_bSet )
{
	int iRet = 0;
	DWORD dwCur = GetTickCount();

	tag2QAlertCfg objAlertCfg;
 	GetSecCfg( &objAlertCfg, sizeof(objAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );
	
	tag2QGprsCfg objGprsCfg;
 	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	// ע���ʼ��ֵ
	BYTE bytAlertDealType = 0; // 1,Ҫ�´�����; 2,Ҫ�ر����б�������

	// (ע�⴦��ʱ��˳��)
	if( v_bSet ) // �������ñ���
	{
		if( v_bSet && !(objAlertCfg.m_bytAlert_1 & 0x08) )
		{
			m_dwAlertSymb &= ~ALERT_TYPE_FRONTDOOR;
			PRTMSG(MSG_NOR, "Deal FrontDooropen Alerm, but this func is Disabled, Exit!\n" );
			return 0;
		}

		if( !(ALERT_TYPE_FRONTDOOR & m_dwAlertSymb) ) // ��֮ǰû�иñ���״̬
		{
			bytAlertDealType = 1; // Ҫ�´���ñ���
		}
		else // ���Ѿ��иñ���״̬
		{
			if( dwCur - m_dwAlertFrontDoorTm > ALERT_SNDDATA_PERIOD ) // �����������ʱ��
			{
				bytAlertDealType = 1; // Ҫ�´���ñ���
			}
		}

		m_dwAlertSymb |= ALERT_TYPE_FRONTDOOR; // ���øñ���״̬
	}
	else // �����������
	{
		PRTMSG(MSG_NOR, "Stop FrontDooropen Alerm!\n" );
		
		if( m_dwAlertSymb & ALERT_TYPE_FRONTDOOR )
		{
			m_dwAlertSymb &= ~ALERT_TYPE_FRONTDOOR; // ����ñ���״̬
			
			// ���ùرռ�ض�ʱ,��Ϊ���ܱ�������,��ػ�����
			// ���ô������ϱ���ʱ,��Ϊ���ֹͣʱ��������ϱ���ʱ

			if( !m_dwAlertSymb ) // �������û���κα���
				bytAlertDealType = 2; // Ҫ�رձ�������
		}
	}

	PRTMSG(MSG_NOR, "bytAlertDealType = %d\n", bytAlertDealType);
	
	// �±�������
	if( 1 == bytAlertDealType ) // ��Ҫ�´�����
	{
		PRTMSG(MSG_NOR, "Start FrontDooropen Alerm!\n" );

		// ͳһ��������
		iRet = _DealAlert( false, false );

		// ���±�����ʼʱ��
		m_dwAlertFrontDoorTm = dwCur;
		
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
 		g_objPhoto.AddPhoto( PHOTOEVT_OPENDOOR );
#endif
	}
	else if( 2 == bytAlertDealType ) // ��Ҫ�ر����б���
	{
		if( m_objMonStatus.m_bAlertAuto )
		{
			_EndMon(); // ���Ǳ���������,�رռ��

			char buf[ SOCK_MAXSIZE ];
			int iBufLen = 0;
			BYTE bytEndType = 0x04; // ��Ϊ�����ź���ֹ
			int iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, 1, 0x01, 0x44, buf, sizeof(buf), iBufLen);
			if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
		}
	}

	return 0;
}

/// ��/�رշǷ������ű���
int CMonAlert::SetAlertBackDoor( const bool v_bSet )
{
	int iRet = 0;
	DWORD dwCur = GetTickCount();

	tag2QAlertCfg objAlertCfg;
 	GetSecCfg( &objAlertCfg, sizeof(objAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );
	
	tag2QGprsCfg objGprsCfg;
 	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	// ע���ʼ��ֵ
	BYTE bytAlertDealType = 0; // 1,Ҫ�´�����; 2,Ҫ�ر����б�������

	// (ע�⴦��ʱ��˳��)
	if( v_bSet ) // �������ñ���
	{
		if( v_bSet && !(objAlertCfg.m_bytAlert_1 & 0x08) )
		{
			m_dwAlertSymb &= ~ALERT_TYPE_BACKDOOR;
			PRTMSG(MSG_NOR, "Deal BackDooropen Alerm, but this func is Disabled, Exit!\n" );
			return 0;
		}

		if( !(ALERT_TYPE_BACKDOOR & m_dwAlertSymb) ) // ��֮ǰû�иñ���״̬
		{
			bytAlertDealType = 1; // Ҫ�´���ñ���
		}
		else // ���Ѿ��иñ���״̬
		{
			if( dwCur - m_dwAlertBackDoorTm > ALERT_SNDDATA_PERIOD ) // �����������ʱ��
			{
				bytAlertDealType = 1; // Ҫ�´���ñ���
			}
		}

		m_dwAlertSymb |= ALERT_TYPE_BACKDOOR; // ���øñ���״̬
	}
	else // �����������
	{
		PRTMSG(MSG_NOR, "Stop BackDooropen Alerm!\n" );
		
		if( m_dwAlertSymb & ALERT_TYPE_BACKDOOR )
		{
			m_dwAlertSymb &= ~ALERT_TYPE_BACKDOOR; // ����ñ���״̬
			
			// ���ùرռ�ض�ʱ,��Ϊ���ܱ�������,��ػ�����
			// ���ô������ϱ���ʱ,��Ϊ���ֹͣʱ��������ϱ���ʱ

			if( !m_dwAlertSymb ) // �������û���κα���
				bytAlertDealType = 2; // Ҫ�رձ�������
		}
	}

	// �±�������
	if( 1 == bytAlertDealType ) // ��Ҫ�´�����
	{
		PRTMSG(MSG_NOR, "Start BackDooropen Alerm!\n" );

		// ͳһ��������
		iRet = _DealAlert( false, false );

		// ���±�����ʼʱ��
		m_dwAlertBackDoorTm = dwCur;
		
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
 		g_objPhoto.AddPhoto( PHOTOEVT_OPENDOOR );
#endif
	}
	else if( 2 == bytAlertDealType ) // ��Ҫ�ر����б���
	{
		if( m_objMonStatus.m_bAlertAuto )
		{
			_EndMon(); // ���Ǳ���������,�رռ��

			char buf[ SOCK_MAXSIZE ];
			int iBufLen = 0;
			BYTE bytEndType = 0x04; // ��Ϊ�����ź���ֹ
			int iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, 1, 0x01, 0x44, buf, sizeof(buf), iBufLen);
			if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
		}
	}

	return 0;
}

int CMonAlert::SetAlertOverTurn( const bool v_bSet )
{
	int iRet = 0;
	DWORD dwCur = GetTickCount();

	tag2QAlertCfg objAlertCfg;
 	GetSecCfg( &objAlertCfg, sizeof(objAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );

	if( v_bSet && !(objAlertCfg.m_bytAlert_1 & 0x08) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_OVERTURN;
		PRTMSG(MSG_NOR, "Deal OverTurn Alerm, but this func is Disabled! Exit!\n" );
		return 0;
	}

	// ע���ʼ��ֵ
	BYTE bytAlertDealType = 0; // 1,Ҫ�´�����; 2,Ҫ�ر����б�������

	// (ע�⴦��ʱ��˳��)
	if( v_bSet ) // �������ñ���
	{
		if( !(ALERT_TYPE_OVERTURN & m_dwAlertSymb) ) // ��֮ǰû�иñ���״̬
		{
			bytAlertDealType = 1; // Ҫ�´���ñ���
		}
		else // ���Ѿ��иñ���״̬
		{
			if( dwCur - m_dwAlertOverTurnTm > ALERT_SNDDATA_PERIOD ) // �����������ʱ��
			{
				bytAlertDealType = 1; // Ҫ�´���ñ���
			}
		}

		m_dwAlertSymb |= ALERT_TYPE_OVERTURN; // ���øñ���״̬
	}

	// �±�������
	if( 1 == bytAlertDealType ) // ��Ҫ�´�����
	{
		PRTMSG(MSG_NOR, "Start OverTurn Alerm\n" );

		// ͳһ��������
		iRet = _DealAlert( false, false ); // �෭����֮��,��Ȼ���ڻ��б�����ƽ����,��������ѯʱ��һ���ܲ鵽,�ʲ���Ҫ���
		
		// ���±�����ʼʱ��
		m_dwAlertOverTurnTm = dwCur;
		
		// �෭�������գ�ȥ����
		char szBuf[2] = {0};
		szBuf[0] = 0x04;
		szBuf[1] = 0x02;
		DataPush(szBuf, 2, DEV_QIAN, DEV_DVR, LV2);
	}

	return 0;
}

int CMonAlert::SetAlertBump( const bool v_bSet )
{
	int iRet = 0;
	DWORD dwCur = GetTickCount();

	tag2QAlertCfg objAlertCfg;
 	::GetSecCfg( &objAlertCfg, sizeof(objAlertCfg),
 		offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );

	if( v_bSet && !(objAlertCfg.m_bytAlert_1 & 0x10) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_BUMP;
		PRTMSG(MSG_NOR, "Deal Bump Alerm, but this func is Disabled! Exit!\n" );
		return 0;
	}

	// ע���ʼ��ֵ
	BYTE bytAlertDealType = 0; // 1,Ҫ�´�����; 2,Ҫ�ر����б�������

	// (ע�⴦��ʱ��˳��)
	if( v_bSet ) // �������ñ���
	{
		if( !(ALERT_TYPE_BUMP & m_dwAlertSymb) ) // ��֮ǰû�иñ���״̬
		{
			bytAlertDealType = 1; // Ҫ�´���ñ���
		}
		else // ���Ѿ��иñ���״̬
		{
			if( dwCur - m_dwAlertBumpTm > ALERT_SNDDATA_PERIOD ) // �����������ʱ��
			{
				bytAlertDealType = 1; // Ҫ�´���ñ���
			}
		}

		m_dwAlertSymb |= ALERT_TYPE_BUMP; // ���øñ���״̬
	}

	// �±�������
	if( 1 == bytAlertDealType ) // ��Ҫ�´�����
	{
		PRTMSG(MSG_NOR, "Start Bump Alerm\n" );

		// ͳһ��������
		iRet = _DealAlert( false, false ); // ��ײ����֮��,����Ҫ��ʱ��鱨����ƽ
		
		// ���±�����ʼʱ��
		m_dwAlertBumpTm = dwCur;
		
		// �෭�������գ�ȥ����
		char szBuf[2] = {0};
		szBuf[0] = 0x04;
		szBuf[1] = 0x03;
		DataPush(szBuf, 2, DEV_QIAN, DEV_DVR, LV2);
	}

	return 0;
}

int CMonAlert::SetAlertDrive( const bool v_bSet )
{
	int iRet = 0;

	tag2QAlertCfg objAlertCfg;
 	GetSecCfg( &objAlertCfg, sizeof(objAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );

	if( v_bSet && !(objAlertCfg.m_bytAlert_2 & 0x04) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_DRIVE;
		PRTMSG(MSG_NOR, "Deal Illegal Drive Alerm, but this func is Disabled! Exit!\n" );
		return 0;
	}

	if( v_bSet ) // ��Ҫ��ʼ����
	{
		PRTMSG(MSG_NOR, "Start Illegal Drive Alerm!\n" );

		m_dwAlertSymb |= ALERT_TYPE_DRIVE;

		// ����һ���Ƿ���������
		tagGPSData objGps(0);
		tag2QGprsCfg objGprsCfg;
		GetCurGPS( objGps, true );
		objGprsCfg.Init();
		::GetSecCfg( &objGprsCfg, sizeof(objGprsCfg), offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );
		_SendOne0149Alert(objGps, ALERM_0149_25);
		
		m_dwAlertSymb &= ~ALERT_TYPE_DRIVE;	// ����֮�󣬽���־λ���

// 		// ͳһ��������
// 		iRet = _DealAlert( false, false );
// 		
// 		// �Ƿ�����������������
// 		char szBuf[2] = {0};
// 		szBuf[0] = 0x04;
// 		szBuf[1] = 0x0a;
// 		DataPush(szBuf, 2, DEV_QIAN, DEV_DVR, LV2);
	}
// 	else
// 	{
// 		if( m_dwAlertSymb & ALERT_TYPE_DRIVE )
// 		{
// 			m_dwAlertSymb &= ~ALERT_TYPE_DRIVE;
// 
// 			if( 0 == m_dwAlertSymb && m_objMonStatus.m_bAlertAuto )
// 			{
// 				_EndMon(); // ���Ǳ���������,�رռ��
// 
// 				char buf[ 200 ] = { 0 };
// 				int iBufLen = 0;
// 				BYTE bytEndType = 0x04; // ��Ϊ�����ź���ֹ
// 				int iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, 1, 0x01, 0x44, buf, sizeof(buf), iBufLen);
// 				if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
// 			}
// 		}
// 	}

	return 0;
}

int CMonAlert::SetAlertOverSpd( const bool v_bSet )
{
	int iRet = 0;

	tag2QAlertCfg objAlertCfg;
 	::GetSecCfg( &objAlertCfg, sizeof(objAlertCfg),
 		offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );

	if( v_bSet && !(objAlertCfg.m_bytAlert_2 & 0x20) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_OVERSPEED;
		PRTMSG(MSG_NOR, "Deal OverSpeed Alerm, but this func is Disabled! Exit!\n" );
		return 0;
	}

	if( v_bSet ) // ��Ҫ��ʼ����
	{
		PRTMSG(MSG_NOR, "Start OverSpeed Alerm!\n" );

		m_dwAlertSymb |= ALERT_TYPE_OVERSPEED;

		// ͳһ�����������ٱ�����ͳһ������Ϊֻ��һ�Σ�
		//iRet = _DealAlert( false, false );

		// ����һ�����ٱ���
		tagGPSData objGps(0);
		tag2QGprsCfg objGprsCfg;
		GetCurGPS( objGps, true );
		objGprsCfg.Init();
		::GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),
			offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );
		_Snd0145( objGps, objGprsCfg, true );

		m_dwAlertSymb &= ~ALERT_TYPE_OVERSPEED;	// ���ٱ���֮�󣬽���־λ���
		
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
 		g_objPhoto.AddPhoto( PHOTOEVT_OVERSPEED );
#endif
	}
// 	else
// 	{
// 		if( m_dwAlertSymb & ALERT_TYPE_OVERSPEED )
// 		{
// 			m_dwAlertSymb &= ~ALERT_TYPE_OVERSPEED;
// 
// 			if( 0 == m_dwAlertSymb && m_objMonStatus.m_bAlertAuto )
// 			{
// 				_EndMon(); // ���Ǳ���������,�رռ��
// 
// 				char buf[ 200 ] = { 0 };
// 				int iBufLen = 0;
// 				BYTE bytEndType = 0x04; // ��Ϊ�����ź���ֹ
// 				int iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, 1, 0x01, 0x44, buf, sizeof(buf), iBufLen);
// 				if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
// 			}
// 		}
// 	}

	return 0;
}

int CMonAlert::SetAlertBelowSpd( const bool v_bSet )
{
	int iRet = 0;

	tag2QAlertCfg objAlertCfg;
 	::GetSecCfg( &objAlertCfg, sizeof(objAlertCfg),
 		offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );

	if( v_bSet && !(objAlertCfg.m_bytAlert_2 & 0x04) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_BELOWSPEED;
		PRTMSG(MSG_NOR, "Deal BelowSpeed Alerm, but this func is Disabled! Exit!\n" );
		return 0;
	}

	if( v_bSet ) // ��Ҫ��ʼ����
	{
		PRTMSG(MSG_NOR, "Start BelowSpeed Alerm!\n" );

		m_dwAlertSymb |= ALERT_TYPE_BELOWSPEED;

		// ͳһ�����������ٱ�����ͳһ������Ϊֻ��һ�Σ�
		//iRet = _DealAlert( false, false );

		// ����һ�����ٱ���
		tagGPSData objGps(0);
		tag2QGprsCfg objGprsCfg;
		GetCurGPS( objGps, true );
		objGprsCfg.Init();
		::GetSecCfg( &objGprsCfg, sizeof(objGprsCfg), offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );
		_SendOne0149Alert(objGps, ALERM_0149_1);

		m_dwAlertSymb &= ~ALERT_TYPE_BELOWSPEED;	// ���ٱ���֮�󣬽���־λ���
		
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
 		g_objPhoto.AddPhoto( PHOTOEVT_BELOWSPEED );
#endif
	}
// 	else
// 	{
// 		if( m_dwAlertSymb & ALERT_TYPE_BELOWSPEED )
// 		{
// 			m_dwAlertSymb &= ~ALERT_TYPE_BELOWSPEED;
// 
// 			if( 0 == m_dwAlertSymb && m_objMonStatus.m_bAlertAuto )
// 			{
// 				_EndMon(); // ���Ǳ���������,�رռ��
// 
// 				char buf[ 200 ] = { 0 };
// 				int iBufLen = 0;
// 				BYTE bytEndType = 0x04; // ��Ϊ�����ź���ֹ
// 				int iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, 1, 0x01, 0x44, buf, sizeof(buf), iBufLen);
// 				if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
// 			}
// 		}
// 	}

	return 0;
}

int CMonAlert::SetAlertArea( const bool v_bSet )
{
	int iRet = 0;
	
	tag2QAlertCfg objAlertCfg;
	::GetSecCfg( &objAlertCfg, sizeof(objAlertCfg),
		offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );
	
	if( v_bSet && !(objAlertCfg.m_bytAlert_2 & 0x40) )
	{
		m_dwAlertSymb &= ~ALERT_TYPE_RANGE;
		PRTMSG(MSG_NOR, "Deal Area Alerm, but this func is Disabled! Exit!\n" );
		return 0;
	}
	
	if( v_bSet ) // ��Ҫ��ʼ����
	{
		PRTMSG(MSG_NOR, "Start Area Alerm!\n" );
		
		m_dwAlertSymb |= ALERT_TYPE_RANGE;
		
		// ����һ��Խ�籨��
		tagGPSData objGps(0);
		tag2QGprsCfg objGprsCfg;
		GetCurGPS( objGps, true );
		objGprsCfg.Init();
		::GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),
			offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );
		_Snd0145( objGps, objGprsCfg, true );
		
		m_dwAlertSymb &= ~ALERT_TYPE_RANGE;	// ����֮�󣬽���־λ���

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
 		g_objPhoto.AddPhoto( PHOTOEVT_AREA );
#endif
	}
	
	return 0;
}

/// ��������JG2
int CMonAlert::SetAlertOther(DWORD v_dwType, const bool v_bSet)
{
	if(v_bSet)
	{
		m_dwAlertSymb |= v_dwType;	// ���øñ���״̬
		_DealAlert( false, false );
	}
	else
	{
		if( m_dwAlertSymb & v_dwType )
		{
			m_dwAlertSymb &= ~v_dwType;

			if( 0 == m_dwAlertSymb )  // �������û���κα���
			{
				if( m_objMonStatus.m_bAlertAuto )
				{
					_EndMon(); // ���Ǳ���������,�رռ��

					char buf[ 200 ] = { 0 };
					int iBufLen = 0;
					BYTE bytEndType = 0x04; // ��Ϊ�����ź���ֹ
					int iResult = g_objSms.MakeSmsFrame((char*)&bytEndType, 1, 0x01, 0x44, buf, sizeof(buf), iBufLen);
					if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );
				}
			}
		}
	}

	return 1;
}

int CMonAlert::SetAlertTest()
{
	_KillTimer(&g_objTimerMng, ALERT_TEST_TIMER );
	_SetTimer(&g_objTimerMng, ALERT_TEST_TIMER, ALERT_TEST_PERIOD, G_TmAlertTest );
	m_dwAlertSymb |= ALERT_TYPE_TEST;
	return 0;
}

int CMonAlert::_BeginAutoReqLsn( DWORD v_dwBeginInterval, char* v_szLsnTel/*=NULL*/ )
{
	// ���ü�������
	memset( m_szAutoReqLsnTel, 0, sizeof(m_szAutoReqLsnTel) );
	if( !v_szLsnTel )
	{
		tag2QServCodeCfg objLsnCfg;
		objLsnCfg.Init();
 		GetSecCfg( &objLsnCfg, sizeof(objLsnCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objLsnCfg) );

		strncpy( m_szAutoReqLsnTel, (char*)&(objLsnCfg.m_aryLsnHandtel[0]), sizeof(m_szAutoReqLsnTel) - 1 );
	}
	else
	{
		strncpy( m_szAutoReqLsnTel, v_szLsnTel, sizeof(m_szAutoReqLsnTel) - 1 );
	}
	m_szAutoReqLsnTel[ 14 ] = 0;
	char* pBlank = strchr( m_szAutoReqLsnTel, 0x20 );
	if( pBlank ) *pBlank = 0;

	// �������״̬����������
	m_bytAutoReqLsnCount = 0;
	m_bytAutoReqLsnContBusyCt = 0;
	_KillTimer(&g_objTimerMng, AUTO_REQLSN_TIMER );
	_SetTimer(&g_objTimerMng, AUTO_REQLSN_TIMER, v_dwBeginInterval, G_TmAutoReqLsn ); // ����ʱ��֮����������

	return 0;
}

int CMonAlert::_AutoReqLsn()
{
	// �������
	m_szAutoReqLsnTel[ sizeof(m_szAutoReqLsnTel) - 1 ] = 0;
	WORD wTelLen = WORD( strlen(m_szAutoReqLsnTel) );
	if( wTelLen > 20 || wTelLen < 3 ) return ERR_AUTOLSN_NOTEL;
	
	char buf[ 100 ] = { 0 };
	buf[ 0 ] = BYTE(0xf1);
	buf[ 1 ] = BYTE( wTelLen );
	memcpy( buf + 2, m_szAutoReqLsnTel, wTelLen );

	if( DataPush(buf, 2+wTelLen, DEV_QIAN, DEV_PHONE, LV2) ) 
		return ERR_AUTOLSN;

	return 0;
}

bool CMonAlert::JugFootAlert()
{
	return m_dwAlertSymb & ALERT_TYPE_FOOT ? true : false;
}

DWORD CMonAlert::GetAlertSta()
{
	return m_dwAlertSymb;
}

/*
void CMonAlert::FillAlertSymb( const DWORD v_dwAlertSta, BYTE &v_byt1, BYTE &v_byt2 )
{
// ǧ����ͨ�ð�,������_����
// ���ֽڣ�
// 0	A1	A2	E1	E2	E3	V	1
// ���ֽڣ�
// 0	C	K	S1	S2	S3	T	1
// A1��A2����ӱ�����״̬
// 00������
// 01���𶯱���
// 10���Ƿ��򿪳��ű���
// 11����������
// E1��E2��E3��Խ�籨����״̬�����鱨��״̬�ʺ���Σ�ճ�����
// 000������
// 001���ڹ涨��ʱ���������ʻ
// 010��������ʻ
// 011��ͣ��ʱ�����
// 100��Խ����ʻ
// ������������
// V��Ƿѹ����
// 0������       1��Ƿѹ
// C����·����
//     0������       1����·
// K�����ٱ���
// 0������       1������
// T�����Ա�־λ
// 0������       1������
// S1��S2��S3����������

// ǧ���۹�����_��һ
// ���ֽڣ�
// 0	A1	A2	E1	E2	E3	V	1
// ���ֽڣ�
// 0	C	K	S1	S2	S3	T	1
// A1��A2����ӱ�����״̬
// 00������
// 01���𶯱���
// 10���Ƿ��򿪳��ű���
// 11����������
// E1��Խ����ʻ��־
// 0������
// 1��Խ����ʻ
// E2��������ʻ��־
// 0������
// 1��������ʻ
// E3���ڹ涨ʱ�������ʻ��־
// 0������
// 1���ڹ涨ʱ�������ʻ
// S1���Ƿ�����������־
// 0������
// 1���Ƿ���������
// V��Ƿѹ����
// 0������       1��Ƿѹ
// C����·����
//     0������       1����·
// K�����ٱ���
// 0������       1������
// T�����Ա�־λ
// 0������       1������
// S2��S3����������


	v_byt1 = 1, v_byt2 = 1;

	if( ALERT_TYPE_POWBROWNOUT & v_dwAlertSta ) // Ƿѹ����
		v_byt1 |= 0x02;
	if( ALERT_TYPE_TIME & v_dwAlertSta ) // ʱ�������ʻ����
		v_byt1 |= 0x04;
	if( ALERT_TYPE_RSPEED & v_dwAlertSta ) // �����ٱ���(Խ�粢����)
	{
		v_byt1 |= 0x08;
		v_byt1 |= 0x10;
	}
	if( ALERT_TYPE_RANGE & v_dwAlertSta ) // Խ�籨��
		v_byt1 |= 0x10;
	if( ALERT_TYPE_OVERTURN & v_dwAlertSta ) // �෭����
		v_byt1 |= 0x20;

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
	if( ALERT_TYPE_FRONTDOOR & v_dwAlertSta ) // �Ƿ������ű���
		v_byt1 |= 0x40;
#endif

#if USE_PROTOCOL == 30
	if( ALERT_TYPE_BUMP & v_dwAlertSta ) // ��ײ����
		v_byt1 |= 0x40;
#endif

	if( ALERT_TYPE_DRIVE & v_dwAlertSta ) // �Ƿ���������
		v_byt2 |= 0x10;
	if( ALERT_TYPE_OVERSPEED & v_dwAlertSta ) // ���ٱ���
		v_byt2 |= 0x08;
	if( ALERT_TYPE_FOOT & v_dwAlertSta ) // ���ٱ���
		v_byt2 |= 0x20;
	if( ALERT_TYPE_POWOFF & v_dwAlertSta ) // �ϵ籨��
		v_byt2 |= 0x40;
	if( ALERT_TYPE_TEST & v_dwAlertSta ) // ��������
		v_byt2 |= 0x02;
}
*/

void CMonAlert::FillAlertSymb( const DWORD v_dwAlertSta, BYTE *v_byt1, BYTE *v_byt2 )
{
// ǧ����ͨ�ð�,������_����
// ���ֽڣ�
// 0	A1	A2	E1	E2	E3	V	1
// ���ֽڣ�
// 0	C	K	S1	S2	S3	T	1
// A1��A2����ӱ�����״̬
// 00������
// 01���𶯱���
// 10���Ƿ��򿪳��ű���
// 11����������
// E1��E2��E3��Խ�籨����״̬�����鱨��״̬�ʺ���Σ�ճ�����
// 000������
// 001���ڹ涨��ʱ���������ʻ
// 010��������ʻ
// 011��ͣ��ʱ�����
// 100��Խ����ʻ
// ������������
// V��Ƿѹ����
// 0������       1��Ƿѹ
// C����·����
//     0������       1����·
// K�����ٱ���
// 0������       1������
// T�����Ա�־λ
// 0������       1������
// S1��S2��S3����������

// ǧ���۹�����_��һ
// ���ֽڣ�
// 0	A1	A2	E1	E2	E3	V	1
// ���ֽڣ�
// 0	C	K	S1	S2	S3	T	1
// A1��A2����ӱ�����״̬
// 00������
// 01���𶯱���
// 10���Ƿ��򿪳��ű���
// 11����������
// E1��Խ����ʻ��־
// 0������
// 1��Խ����ʻ
// E2��������ʻ��־
// 0������
// 1��������ʻ
// E3���ڹ涨ʱ�������ʻ��־
// 0������
// 1���ڹ涨ʱ�������ʻ
// S1���Ƿ�����������־
// 0������
// 1���Ƿ���������
// V��Ƿѹ����
// 0������       1��Ƿѹ
// C����·����
//     0������       1����·
// K�����ٱ���
// 0������       1������
// T�����Ա�־λ
// 0������       1������
// S2��S3����������


	*v_byt1 = 1, *v_byt2 = 1;

	if( ALERT_TYPE_POWBROWNOUT & v_dwAlertSta ) // Ƿѹ����
		*v_byt1 |= 0x02;
	if( ALERT_TYPE_TIME & v_dwAlertSta ) // ʱ�������ʻ����
		*v_byt1 |= 0x04;
	if( ALERT_TYPE_RSPEED & v_dwAlertSta ) // �����ٱ���(Խ�粢����)
	{
		*v_byt1 |= 0x08;
		*v_byt1 |= 0x10;
	}
	if( ALERT_TYPE_RANGE & v_dwAlertSta ) // Խ�籨��
		*v_byt1 |= 0x10;
	if( ALERT_TYPE_OVERTURN & v_dwAlertSta ) // �෭����
		*v_byt1 |= 0x20;

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
	if( (ALERT_TYPE_FRONTDOOR & v_dwAlertSta) || (ALERT_TYPE_BACKDOOR & v_dwAlertSta) ) // �Ƿ������ű���
		*v_byt1 |= 0x40;
#endif

#if USE_PROTOCOL == 30
	if( ALERT_TYPE_BUMP & v_dwAlertSta ) // ��ײ����
		*v_byt1 |= 0x40;
#endif

	if( ALERT_TYPE_DRIVE & v_dwAlertSta ) // �Ƿ���������
		*v_byt2 |= 0x10;
	if( ALERT_TYPE_OVERSPEED & v_dwAlertSta ) // ���ٱ���
		*v_byt1 |= 0x08;
	if( ALERT_TYPE_FOOT & v_dwAlertSta ) // ���ٱ���
		*v_byt2 |= 0x20;
	if( ALERT_TYPE_POWOFF & v_dwAlertSta ) // �ϵ籨��
		*v_byt2 |= 0x40;
	if( ALERT_TYPE_TEST & v_dwAlertSta ) // ��������
		*v_byt2 |= 0x02;
}

void CMonAlert::FillAlertSymbFor0149( const DWORD v_dwAlertSta, BYTE *v_byt1, BYTE *v_byt2 )
{
// ǧ����ͨ�ð�,������_����
// ���ֽڣ�
// A15 A14	A13	A12	A11	A10	A9 A8
// ���ֽڣ�
// A7  A6  A5	A4	A3	A2	A1 A0
// A0��A1��
// 00������
// 01����ײ����
// 10���෭����
// A2��
// 1�����ٱ���
// 0������


	*v_byt1 = 0, *v_byt2 = 0;

	if( ALERT_TYPE_BUMP & v_dwAlertSta ) // ��ײ����
		*v_byt2 |= 0x02;
	if( ALERT_TYPE_OVERTURN & v_dwAlertSta ) // �෭����
		*v_byt2 |= 0x01;
	if( ALERT_TYPE_BELOWSPEED & v_dwAlertSta ) // ���ٱ���
		*v_byt2 |= 0x04;
}

bool CMonAlert::IsMonAlert()
{
	return (m_objMonStatus.m_bInterv || m_objMonStatus.m_bSpace) && m_objMonStatus.m_dwMonTime >= 1000;
}

/// ������� ��Ӧ��ɹ����ȣ�
int CMonAlert::Deal0101(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen, bool v_bFromSM, bool v_bSendRes/*=true*/)
{
// ͨ�ð�,������_����
// ���ҵ�����ͣ�1��+ ���ʱ�䣨2��+ ������ڣ�2��+ ������루2��

// ������_��һ
// ���ҵ�����ͣ�1��+ ���ʱ�䣨2��+ ������ڣ�2��+ ������루2��+ �ϴ�GPS����������(2)

	PRTMSG(MSG_NOR, "Rcv Cent Mon Req\n" );
	
	tag0141 res;
	tagMonStatus objMonStatus;
	tagGPSData objGps(0);
	int iRet = 0;
	int iBufLen = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytAnsType = 0x01;

	if( v_dwDataLen < sizeof(tag0101) )
	{
		PRTMSG(MSG_ERR, "Deal0101 error 1, v_dwDatalen = %d, sizeof(tag0101) = %d\n", v_dwDataLen, sizeof(tag0101));
		XUN_ASSERT_VALID( false, "" );
		return ERR_DATA_INVALID;
	}
	
	tag0101 req;
	memcpy( &req, v_szData, sizeof(req) );

	BYTE bytTemp = req.m_bytMonType & 0x18;

	// �������������ü������
	objMonStatus = m_objMonStatus; // ����ԭ������ʼ��
	if( 0 == bytTemp || 0x10 == bytTemp ) // �ж�ʱ
	{
		if( 0x7f == req.m_bytIntervMin ) req.m_bytIntervMin = 0;
		if( 0x7f == req.m_bytIntervSec ) req.m_bytIntervSec = 0;

		if( req.m_bytIntervMin > 126 )
		{
			PRTMSG(MSG_ERR, "Deal0101 error 2\n");
			iRet = ERR_DATA_INVALID;
			goto DEAL0101_FAIL;
		}
		if( req.m_bytIntervSec > 59 )
		{
			PRTMSG(MSG_ERR, "Deal0101 error 3\n");
			iRet = ERR_DATA_INVALID;
			goto DEAL0101_FAIL;
		}

		objMonStatus.m_bInterv = true;
		objMonStatus.m_dwMonInterv = (req.m_bytIntervMin * 60 + req.m_bytIntervSec) * 1000 ;
		if( objMonStatus.m_dwMonInterv < 1000 ) objMonStatus.m_dwMonInterv = 1000; // �������
	}
	else
	{
		objMonStatus.m_bInterv = false;
	}
	if( 0x08 == bytTemp || 0x10 == bytTemp ) // �ж���
	{
		if( 0x7f == req.m_bytSpaceKilo ) req.m_bytSpaceKilo = 0;
		if( 0x7f == req.m_bytSpaceTenmeter ) req.m_bytSpaceTenmeter = 0;

		if( req.m_bytSpaceKilo > 126 )
		{
			PRTMSG(MSG_ERR, "Deal0101 error 4\n");
			iRet = ERR_DATA_INVALID;
			goto DEAL0101_FAIL;
		}
		if( req.m_bytSpaceTenmeter > 99 )
		{
			PRTMSG(MSG_ERR, "Deal0101 error 5\n");
			iRet = ERR_DATA_INVALID;
			goto DEAL0101_FAIL;
		}

		objMonStatus.m_bSpace = true;
		objMonStatus.m_dwSpace = req.m_bytSpaceKilo * 1000 + req.m_bytSpaceTenmeter * 10;
		if( objMonStatus.m_dwSpace < 10 ) objMonStatus.m_dwSpace = 10;
	}
	else
	{
		objMonStatus.m_bSpace = false;
	}
	if( 0x18 == bytTemp ) // ����ж���Ҫ,��ֹ����ȷ�ļ�ز���,���²�ͣ���쳣��ش���
	{
		PRTMSG(MSG_ERR, "Deal0101 error 6\n");
		iRet = ERR_DATA_INVALID;
		goto DEAL0101_FAIL; // ��Ч�ĸ��·�ʽ
	}

	// ���ʱ��
	if( 0x7f == req.m_bytTimeHour ) req.m_bytTimeHour = 0;
	if( 0x7f == req.m_bytTimeMin ) req.m_bytTimeMin = 0;
	if( 0 == req.m_bytTimeHour && 0 == req.m_bytTimeMin )
	{
		objMonStatus.m_dwMonTime = INVALID_NUM_32; // (ע��Ŷ)
		m_objMonStatus.m_bAlertAuto = false;
	}
	else
	{
		DWORD dwMonTime = (req.m_bytTimeHour * DWORD(60) + req.m_bytTimeMin) * 60 * 1000;
		if( dwMonTime > objMonStatus.m_dwMonTime ) // ���¼��ʱ�䳬����ԭ�еļ��ʱ��(�����Ǳ���ʱ��)
			m_objMonStatus.m_bAlertAuto = false;
		objMonStatus.m_dwMonTime = dwMonTime;
	}

#if 2 == USE_PROTOCOL
	// GPS���ݸ���
	if( 0x7f == req.m_bytUpGpsHund ) req.m_bytUpGpsHund = 0;
	if( 0x7f == req.m_bytUpGpsCount ) req.m_bytUpGpsCount = 0;
	if( 0 == req.m_bytUpGpsHund && 0 == req.m_bytUpGpsCount )
		objMonStatus.m_dwGpsCount = 0xffffffff; // (ע��Ŷ)
	else
	{
		if( req.m_bytUpGpsHund > 126 || req.m_bytUpGpsCount > 99 )
		{
			PRTMSG(MSG_ERR, "Deal0101 error 7\n");
			iRet = ERR_DATA_INVALID;
			goto DEAL0101_FAIL;
		}
		objMonStatus.m_dwGpsCount = req.m_bytUpGpsHund * 100 + req.m_bytUpGpsCount;
	}
#endif

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	objMonStatus.m_dwGpsCount = 0xffffffff;
#endif

	// Ŀ���ֻ���
	if( 0x20 != v_szSrcHandtel[0] && 0 != v_szSrcHandtel[0] && '0' != v_szSrcHandtel[0] )
		memcpy( objMonStatus.m_szMonHandTel, v_szSrcHandtel, sizeof(objMonStatus.m_szMonHandTel) );

	// ���ͼ�سɹ�Ӧ��
	if( v_bSendRes )
	{
		tagQianGps objQianGps;

		res.m_bytAnsType = 0x01;
		iRet = GetCurGPS( objGps, true );
		if( iRet ) 
			goto DEAL0101_FAIL;

		GpsToQianGps( objGps, objQianGps );
		memcpy( &(res.m_objQianGps), &objQianGps, sizeof(res.m_objQianGps) );

		iRet = g_objSms.MakeSmsFrame( (char*)&res, sizeof(res), 0x01, 0x41, buf, sizeof(buf), iBufLen );		
		if( iRet ) 
			goto DEAL0101_FAIL;

		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV13, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );		
		if( iRet )
			goto DEAL0101_FAIL;
	}

	m_objMonStatus = objMonStatus; // ��ز������µ��ڴ��еĳ�פ״̬
	_BeginMon( objGps.longitude, objGps.latitude );

	return 0;

DEAL0101_FAIL:
	// ���ͼ��ʧ��Ӧ��
	if( v_bSendRes )
	{
		bytAnsType = 0x7f;
		int iRet2 = g_objSms.MakeSmsFrame((char*)&bytAnsType, 1, 0x01, 0x41, buf, sizeof(buf), iBufLen);
		if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );
	}

	return iRet;
}

/// ������� (��������)
int CMonAlert::Deal0102(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen, bool v_bFromSM)
{
	PRTMSG(MSG_NOR, "Rcv Cent Stop Mon Req\n" );
	
	int iRet = 0;
	int iBufLen = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytResType = 1;
	iRet = g_objSms.MakeSmsFrame((char*)&bytResType, 1, 0x01, 0x42, buf, sizeof(buf), iBufLen);
	if( !iRet ) 
	{
		g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );
	}
	
	_EndMon();
	
	PRTMSG(MSG_NOR, "Stop Mon Succ\n" );
	
	return iRet;
}

/// �޸ļ�ز��� ��ҵ�������ȣ�
int CMonAlert::Deal0103(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen)
{
	PRTMSG(MSG_NOR, "Rcv Cent Modify Mon Par Req\n" );
	
	if( sizeof(tag0103) != v_dwDataLen ) return ERR_DATA_INVALID;

	tag2QGprsCfg objGprsCfg;
 	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	tag0103 req;
	memcpy( &req, v_szData, sizeof(req) );

	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	BYTE bytResType;

	int iRet = Deal0101( v_szSrcHandtel, (char*)&(req.m_objO101), sizeof(req.m_objO101), false );
	if( !iRet )
	{
		bytResType = 0x01;
		iRet = g_objSms.MakeSmsFrame((char*)&bytResType, 1, 0x01, 0x43, buf, sizeof(buf), iBufLen);
		if( !iRet ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, (objGprsCfg.m_bytChannelBkType_1 & 0x40) ? DATASYMB_SMSBACKUP : 0 );
	}
	else goto DEAL0103_FAILED;
		
	PRTMSG(MSG_NOR, "Modify mon par Succ\n" );

	return 0;

DEAL0103_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame((char*)&bytResType, 1, 0x01, 0x43, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12,
		(objGprsCfg.m_bytChannelBkType_1 & 0x40) ? DATASYMB_SMSBACKUP : 0 );
	return iRet;
}

/// ��������ָʾ ��ҵ�������ȣ�
int CMonAlert::Deal0105(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen)
{
	int iRet = 0;
	int iBufLen = 0;
	char buf[ SOCK_MAXSIZE ];

	bool bNeedSendRes = false; // ע���ʼ����ֵ��false
	bool bDealSucc = false; // ע���ʼ����ֵ��false

	tag2QGprsCfg objGprsCfg;
 	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	if( v_dwDataLen < 1 ) return ERR_DATA_INVALID;

	switch( BYTE(*v_szData) )
	{
	case 0x01: // Ĭ�ϴ���
		{
			PRTMSG(MSG_NOR, "Rcv Cent Alerm Res: Default\n" );

			if( m_dwAlertSymb & ALERT_TYPE_TEST )
			{
				m_dwAlertSymb &= ~ALERT_TYPE_TEST; // ����������Ա��
				_KillTimer(&g_objTimerMng, ALERT_TEST_TIMER );

				// ���Ͳ��Գɹ�֪ͨ��������
				char buf[3] = { 0x01, 0x43, 0x01 };
				DataPush(buf, sizeof(buf), DEV_QIAN, DEV_DIAODU, LV2);

				_EndMon(); // �رռ��
			}
			
			bDealSucc = true;
		}
		break;

	case 0x02: // ��������
		{
			PRTMSG(MSG_NOR, "Rcv Cent Alerm Res: Stop Alerm\n" );
			
			bNeedSendRes = true;

			if( m_objMonStatus.m_bAlertAuto ) // ���Ǳ���������,�رռ��
				_EndMon();
			else // ����,���������־ (����Լ���)
				_ClrAlertSta();
			bDealSucc = true;
		}
		break;

	case 0x03: // �޸ļ�ز���
		{
			PRTMSG(MSG_NOR, "Rcv Cent Alerm Res: Modify Mon Par\n" );
			
			bNeedSendRes = true;

			if( sizeof(tag0105) != v_dwDataLen )
				break;

			tag0105 req;
			memcpy( &req, v_szData, sizeof(req) );
			iRet = Deal0101( v_szSrcHandtel, (char*)&(req.m_obj0101), sizeof(req.m_obj0101), false );
			if( !iRet ) bDealSucc = true;
		}
		break;
		
	default:
		;
	}

	// �յ���������ָʾ,��رվ�Ĭ������ʱ
	_KillTimer(&g_objTimerMng, ALERT_SILENT_TIMER );

	if( bDealSucc )
	{
		BYTE bytAnsType = 0x01;

		iRet = g_objSms.MakeSmsFrame( (char*)&bytAnsType, 1, 0x01, 0x52, buf, sizeof(buf), iBufLen );
		if( !iRet ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, DATASYMB_SMSBACKUP );
	}
	else goto DEAL0105_FAILED;
		
	PRTMSG(MSG_NOR, "Deal Cent Alerm Res Succ\n" );

	return 0;

DEAL0105_FAILED:
	BYTE bytAnsType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytAnsType, 1, 0x01, 0x52, buf, sizeof(buf), iBufLen );
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, DATASYMB_SMSBACKUP );

	return iRet;
}

/// λ�ò�ѯ���� (Ӧ��ɹ�����)
int CMonAlert::Deal0111( char* v_szSrcHandtel, bool v_bFromSM )
{
	tagQianGps objQianGps;
	tag0151 res;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	int iRet = 0;

	tag2QGprsCfg objGprsCfg;
	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg), offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	iRet = GetCurQianGps( objQianGps, true );
	if( !iRet )
	{
		res.m_bytAnsType = 0x01;
		memcpy( &(res.m_objQianGps), &objQianGps, sizeof(res.m_objQianGps) );
		iRet = g_objSms.MakeSmsFrame((char*)&res, sizeof(res), 0x01, 0x51, buf, sizeof(buf), iBufLen);
		if( !iRet )
		{
			iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12,
 				(objGprsCfg.m_bytChannelBkType_1 & 0x10) ? DATASYMB_SMSBACKUP : 0 );
			iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );
			if( iRet ) 
				goto DEAL0111_FAILED;

			// GPSģ���ϵ� (��ʵֻ���Ժ���λ�ò�ѯ,�ſ����յ���ЧGPS)
 			//g_objQianIO.PowerOnGps( true );
 			//g_objQianIO.StartAccChk( 300000 );
		}
		else goto DEAL0111_FAILED;
	}
	else goto DEAL0111_FAILED;

	return 0;

DEAL0111_FAILED:
	BYTE bytAnsType = 0x7f;

	int iRet2 = g_objSms.MakeSmsFrame((char*)&bytAnsType, 1, 0x01, 0x51, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12,
		(objGprsCfg.m_bytChannelBkType_1 & 0x10) ? DATASYMB_SMSBACKUP : 0 );
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );
	
	return iRet;
}


// ָ��������� ������_��һ
int CMonAlert::Deal0720( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen )
{
// �绰���봮��15��

	int iRet = 0;
	char szLsnTel[20] = { 0 };
	char buf[300] = { 0 };
	int iBufLen = 0;
	char* pStop = NULL;
	BYTE bytResType = 1;

	if( v_dwDataLen < 15 )
	{
		iRet = ERR_PAR;
		goto DEAL0720_FAILED;
	}

	strncpy( szLsnTel, v_szData, 15 );
	szLsnTel[ sizeof(szLsnTel) - 1 ] = 0;
	pStop = strchr( szLsnTel, 0x20 );
	if( pStop ) *pStop = 0;
	iRet = _BeginAutoReqLsn( 3000, szLsnTel );
	if( iRet ) goto DEAL0720_FAILED;

	bytResType = 1;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x07, 0x60, buf, sizeof(buf), iBufLen );
	if( !iRet )
	{
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, DATASYMB_SMSBACKUP, v_szSrcHandtel );
	}

	return 0;

DEAL0720_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x07, 0x60, buf, sizeof(buf), iBufLen );
	if( !iRet2 ) iRet2 = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, DATASYMB_SMSBACKUP );

	return iRet;
}

// ָ��������� ǧ����ͨ�ð�,������_����
int CMonAlert::Deal0723( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen )
{
// �绰���볤�ȣ�1��+ �绰���루N��15��

	char szLsnTel[20] = { 0 };
	char buf[300];
	int iBufLen = 0;
	int iRet = 0;
	BYTE bytResType = 1;
	BYTE bytTelLen = 0;

	if( v_dwDataLen < 2 )
	{
		iRet = ERR_PAR;
		goto DEAL0723_FAILED;
	}

	bytTelLen = v_szData[0];
	if( bytTelLen > 15 || bytTelLen < 1 || bytTelLen > sizeof(szLsnTel) )
	{
		iRet = ERR_PAR;
		goto DEAL0723_FAILED;
	}

	if( v_dwDataLen < DWORD(1 + bytTelLen) )
	{
		iRet = ERR_PAR;
		goto DEAL0723_FAILED;
	}

	strncpy( szLsnTel, v_szData + 1, bytTelLen );
	szLsnTel[ sizeof(szLsnTel) - 1 ] = 0;
	iRet = _BeginAutoReqLsn( 3000, szLsnTel );
	if( iRet ) goto DEAL0723_FAILED;

	bytResType = 1;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x07, 0x63, buf, sizeof(buf), iBufLen );
	if( !iRet )
	{
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, DATASYMB_SMSBACKUP, v_szSrcHandtel );
	}

	return 0;

DEAL0723_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x07, 0x63, buf, sizeof(buf), iBufLen );
	if( !iRet2 ) iRet2 = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, DATASYMB_SMSBACKUP );

	return iRet;
}


/// �������� ��Ӧ��ɹ����ȣ�
int CMonAlert::Deal1009( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	tag2QAlertCfg obj2QAlertCfg, obj2QAlertCfgBkp;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	BYTE bytResType = 0;

	// ���ݲ������
	if( 2 != v_dwDataLen )
	{
		iRet = ERR_PAR;
		goto DEAL1009_FAILED;
	}

	// ��ȡ->�޸�
	iRet = GetSecCfg( &obj2QAlertCfg, sizeof(obj2QAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(obj2QAlertCfg) );
	if( iRet ) goto DEAL1009_FAILED;
	obj2QAlertCfgBkp = obj2QAlertCfg; // �ȱ���
	obj2QAlertCfg.m_bytAlert_1 = BYTE(v_szData[0] & 0x7f);
	obj2QAlertCfg.m_bytAlert_2 = BYTE(v_szData[1] & 0x7f);

	// д�غͷ���Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x49, buf, sizeof(buf), iBufLen); 
	if( !iRet )
	{
		if( iRet = SetSecCfg(&obj2QAlertCfg, offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(obj2QAlertCfg)) )
		{
			goto DEAL1009_FAILED;
		}
		
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL ); // ����
		if( iRet ) // ������ʧ��
		{
			SetSecCfg( &obj2QAlertCfgBkp, offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(obj2QAlertCfgBkp) ); // �ָ�ԭ����
			goto DEAL1009_FAILED;
		}
	}
	else goto DEAL1009_FAILED;

	return 0;

DEAL1009_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x49, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );

	return iRet;
}

/// ״̬��ѯ
int CMonAlert::Deal101a(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen )
{
// ǧ����ͨ�ð�,������_����
// ״̬�֣�2��+ ��׼GPS����
// ���״̬��
// ����VM1��JG2��JG3Э�鶨��
// Bit  15~8: 0	1	T1	T2	T3	T4	T5	0
// Bit  7~0:  0	1	D1	D2	D3	D4	0	0
// T1-������������λ��      1�����ã�0��δ����
// T2-���Ⱥ�������λ��      1�����ã�0��δ����
// T3-˰�غ�������λ��      1�����ã�0��δ����
// T4-�������ĺ�������λ��  1�����ã�0��δ����
// T5-���ٱ�������λ��      1������0����ֹ
// T6-GPSģ��״̬			1������  0���쳣
// D1-������ն�ͨѶ״̬λ��1��������0���쳣
// D2-˰��ʱ������λ��      1��������0���쳣
// D3-������������λ��      1��������0���쳣
// D4-��ϻ�Ӳɼ�ʱ������λ��1��������0���쳣

// KJ2Э��ͨ�ð�
// ״̬�֣�2��+����״̬��1��+ GPS����
// ״̬��
// Bit  15~8: 0	1 T1 T2	1 T4 T5	T6
// Bit  7~0:  0	1  1  1	1  1 T7	 1
// T1-������������λ��      1�����ã�0��δ����
// T2�����Ⱥ�������λ��      1�����ã�0��δ����
// T4-�������ĺ�������λ��  1�����ã�0��δ����
// T5-���ٱ�������λ��      1������0����ֹ
// T6-GPSģ��״̬λ��      1��������0���쳣 (�ǹص���?)
// T7-������״̬λ��        1��������0���쳣
// ��������


// ǧ���۹�����_��һ
// ״̬�֣�2��+����״̬��1��+ GPS����
// ״̬��:
// Bit  15~8: 0	1 T1  1	1 T4 T5	T6
// Bit  7~0:  0	1  1  1	1  1 T7	 1
// T1-������������λ��      1�����ã�0��δ����
// T4-�������ĺ�������λ��  1�����ã�0��δ����
// T5-���ٱ�������λ��      1������0����ֹ
// T6-GPSģ��״̬λ��      1��������0���쳣
// T7-������״̬λ��        1��������0���쳣


	int iRet = 0;
	int iBufLen = 0;
	tag105a res;
	tagQianGps objQianGps;
	char buf[ 500 ];

	memset( &res, 0, sizeof(res) );
	res.m_bytSta_1 = 0x40; // ע��,����������Ϊ���汾�����2λ����01
	res.m_bytSta_2 = 0x40; // ע��,����������Ϊ���汾�����2λ����01

	{
		tag2QServCodeCfg objServCodeCfg;
		GetSecCfg( &objServCodeCfg, sizeof(objServCodeCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objServCodeCfg) );
		
		if( objServCodeCfg.m_szQianSmsTel[0] && 0x20 != objServCodeCfg.m_szQianSmsTel[0] )
			res.m_bytSta_1 |= 0x20; // ��������(�ط���)����
	
		if( objServCodeCfg.m_szDiaoduHandtel[0] && 0x20 != objServCodeCfg.m_szDiaoduHandtel[0] )
			res.m_bytSta_1 |= 0x10; // ���Ⱥ�������

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
		if( objServCodeCfg.m_szTaxHandtel[0] && 0x20 != objServCodeCfg.m_szTaxHandtel[0] ) res.m_bytSta_1 |= 0x08; // ˰�غ�������
#endif
	}
	{
		tag1PComuCfg objComuCfg;
		GetImpCfg( &objComuCfg, sizeof(objComuCfg),	offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(objComuCfg) );
		
		if( objComuCfg.m_szSmsCentID[0] && 0x20 != objComuCfg.m_szSmsCentID[0] )
			res.m_bytSta_1 |= 0x04; // �������ĺ�����
	}

	{
		tag2QAlertCfg objAlertCfg;
		GetSecCfg( &objAlertCfg, sizeof(objAlertCfg), offsetof(tagSecondCfg, m_uni2QAlertCfg.m_obj2QAlertCfg), sizeof(objAlertCfg) );
	
		if( objAlertCfg.m_bytAlert_1 & 0x20 ) 
			res.m_bytSta_1 |= 0x02; // ���ٱ�������λ
	}

	{
		if( g_objQianIO.GetDeviceSta() & DEVSTA_GPSON ) res.m_bytSta_1 |= 0x01; // GPSģ��״̬
	}

	{
		DWORD dwComuSta;
		dwComuSta = g_objComu.GetComuSta();
#if 2 == USE_PROTOCOL || 30 == USE_PROTOCOL
		if( dwComuSta & COMUSTA_DIAODUCONN ) res.m_bytSta_2 |= 0x02; // ������״̬
#endif
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
		if( dwComuSta & COMUSTA_DIAODUCONN ) res.m_bytSta_2 |= 0x20; // ������״̬
#endif

#if USE_BLK == 1 && (0 == USE_PROTOCOL || 1 == USE_PROTOCOL)
		res.m_bytSta_2 |= 0x04; // ��ϻ�Ӳɼ�����̶�,ֻҪ������ϻ�ӹ���
#endif
	}
	
#if 30 == USE_PROTOCOL
	// ������״̬
	// 0	FB	HB	LL	RL	FD	BD	A
	// A -ACC�ź�״̬,			1��ON�� 0��OFF
	// BD-����״̬��			1����Ч��0����Ч
	// FD��ǰ����״̬��          1����Ч��0����Ч
	// RL����ת��ƣ�            1����Ч��0����Ч
	// LL����ת��ƣ�            1����Ч��0����Ч
	// HB����ɲ��                1����Ч��0����Ч
	// FB����ɲ��                1����Ч��0����Ч
	{
		DWORD dwIoSta = 0;
		if( g_objQianIO.IN_QueryIOSta(dwIoSta) )
		{
			if( !(dwIoSta & 0x00000001) ) res.m_bytIO |= 0x01;
			if( !(dwIoSta & 0x00000004) ) res.m_bytIO |= 0x02;
			if( !(dwIoSta & 0x00000008) ) res.m_bytIO |= 0x04;
			if( !(dwIoSta & 0x00000010) ) res.m_bytIO |= 0x08;
			if( !(dwIoSta & 0x00000020) ) res.m_bytIO |= 0x10;
			if( !(dwIoSta & 0x00000040) ) res.m_bytIO |= 0x20;
			if( !(dwIoSta & 0x00000080) ) res.m_bytIO |= 0x40;
		}
	}
#endif

#if 2 == USE_PROTOCOL
	// ������״̬
	// Bit   7       ->             0
	// 0	1	0	0	0	0	D	A
	// A-ACC�ź�״̬,			1��ON�� 0��OFF
	// D-����״̬��				1���رգ�0����

	{
		DWORD dwIoSta = 0;
		if( g_objQianIO.IN_QueryIOSta(dwIoSta) )
		{
			if( !(dwIoSta & 0x00000001) ) res.m_bytIO |= 0x01;
			if( dwIoSta & 0x00000008 ) res.m_bytIO |= 0x02;
		}
	}
#endif

	// ��׼GPS����
	iRet = GetCurQianGps( res.objQianGps, true );
	if( iRet ) goto DEAL101A_FAILED;
	else memcpy( &res.objQianGps, &objQianGps, sizeof(res.objQianGps) );

	iRet = g_objSms.MakeSmsFrame( (char*)&res, sizeof(res), 0x10, 0x5a, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// ����Ӧ��
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, DATASYMB_SMSBACKUP );
		if( iRet ) goto DEAL101A_FAILED;
	}
	else goto DEAL101A_FAILED;

	return 0;

DEAL101A_FAILED:
	return iRet;
}

// ���ٱ�������
int CMonAlert::Deal103d( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
// ������Э��_��һ
// �����ٶ�	1	��λ������/Сʱ, ȡֵ��Χ��0��7EH��0ת���7FH
// ��������ʱ��	1	��λ����
	
	int iRet = 0;

#if 2 == USE_PROTOCOL

	DrvCfg objDrvCfg;

	BYTE bytResType = 0;
	BYTE bytAlermSpd = 0;
	BYTE bytAlermPrid = 0;
	char buf[100] = { 0 };
	int iBufLen = 0;

	if( v_dwDataLen < 2 )
	{
		iRet = ERR_PAR;
		goto DEAL103D_FAILED;
	}

	bytAlermSpd = BYTE( v_szData[0] );
	if( bytAlermSpd > 0x7f )
	{
		iRet = ERR_PAR;
		goto DEAL103D_FAILED;
	}
	if( 0x7f == bytAlermSpd ) bytAlermSpd = 0;

	bytAlermPrid = BYTE( v_szData[1] );

	iRet = GetSecCfg( &objDrvCfg, sizeof(objDrvCfg), offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered), sizeof(objDrvCfg) );
	if( iRet ) goto DEAL103D_FAILED;

	objDrvCfg.max_speed = bytAlermSpd;
	objDrvCfg.min_over	= bytAlermPrid;

	iRet = SetSecCfg( &objDrvCfg, offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered), sizeof(objDrvCfg) );
	if( iRet ) goto DEAL103D_FAILED;

	bytResType = 1;
	goto DEAL103D_END;

DEAL103D_FAILED:
	bytResType = 0x7f;

DEAL103D_END:
	if( !g_objSms.MakeSmsFrame((char*)&bytResType, sizeof(bytResType), 0x10, 0x7d, buf, sizeof(buf), iBufLen) )
	{
		g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
	}

#endif

	return iRet;
}


///����鳵
// ��ѯ�����ţ�1��+ ����ն�ID��8�������������1��+������18������N
int CMonAlert::Deal0136( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen )
{
	tag0176 res;
	tag0136 *p = (tag0136*)v_szData;
	char buf[500] = {0};
	int iBufLen = 0;
	int i, r1, r2, r3, r4;
	bool in_zone = false;
	int iRet = GetCurQianGps( res.m_objQianGps, true );
	if( iRet ) goto DEAL0136_FAILED;

	// �жϵ�ǰ��γ���Ƿ���������������
	for(i=0; i<p->m_bytZoneCount; i++) 
	{
		r1 = memcmp(p->m_Zone[i].lat1, &res.m_objQianGps.m_bytLatDu, 4);
		r2 = memcmp(p->m_Zone[i].lat2, &res.m_objQianGps.m_bytLatDu, 4);

		// ���������λ�Żض��ֽ��У������ж�
		if(p->m_Zone[i].hbit1 & 0x02)	p->m_Zone[i].lon1[0] |= 0x80;
		if(p->m_Zone[i].hbit2 & 0x02)	p->m_Zone[i].lon2[0] |= 0x80;
		if(res.m_objQianGps.m_bytMix & 0x02)	res.m_objQianGps.m_bytLonDu |= 0x80; 

		r3 = memcmp(p->m_Zone[i].lon1, &res.m_objQianGps.m_bytLonDu, 4);
		r4 = memcmp(p->m_Zone[i].lon2, &res.m_objQianGps.m_bytLonDu, 4);

		if( r1<=0 && r2>=0 && r3<=0 && r4>=0 )	{ in_zone = true; break; }
	}
	
	if(in_zone) 
	{
		memcpy(&res.m_bytQueryIndex, &p->m_bytQueryIndex, 10);
		res.m_objQianGps.m_bytLonDu &= ~0x80;	//��վ��ȵĶ��ֽ����λ

		iRet = g_objSms.MakeSmsFrame( (char*)&res, sizeof(res), 0x01, 0x76, buf, sizeof(buf), iBufLen);
		if( !iRet ) {
			// ����Ӧ��
			iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, DATASYMB_SMSBACKUP );
			if( iRet ) goto DEAL0136_FAILED;
		} else goto DEAL0136_FAILED;
	} 

	return 0;

DEAL0136_FAILED:
	return iRet;
}


