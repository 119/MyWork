#include "yx_QianStdAfx.h"
#include "yx_AutoRpt.h"

#if USE_AUTORPT == 1

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-AutoRpt  ")

void G_TmAutoRpt(void *arg, int len)
{
	g_objAutoRpt.P_TmAutoRpt();
}

CAutoRpt::CAutoRpt()
{

}

CAutoRpt::~CAutoRpt()
{

}

void CAutoRpt::Init()
{
	// �ж��Ƿ�Ҫ���������ϱ�
//	PRTMSG(MSG_NOR, "Init AutoRpt\n" );

	DWORD dwVeSta = g_objCarSta.GetVeSta(); // ����Ҫ����CIO������ʼ��֮�����

	tagGPSData objGps(0);
	g_objMonAlert.GetCurGPS( objGps, false );
	ENU_RPTJUGRESULT enuResult = _JugCanRpt( objGps );
	if( RPT_YES != enuResult )
	{
		//PRTMSG(MSG_NOR, "can not Rpt!\n");
		EndAutoRpt();
		return;
	}

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	// �ƶ�/��ֹ�ϱ�
	if( dwVeSta & VESTA_MOVE )
		BeginAutoRpt( 1 ); // �ƶ�
	else // ��GPS��Ч״̬Ҳ��Ϊ��ֹ״̬�ϱ�	if ( 2 == gps.m_bytMoveType )
		BeginAutoRpt( 2 ); // ��ֹ

	// �ճ�/�س��ϱ�
	
	if( dwVeSta & VESTA_HEAVY )
		BeginAutoRpt(4); // �س�
	else
		BeginAutoRpt(3); // �ճ�
#endif

#if 2 == USE_PROTOCOL
	if( dwVeSta & VESTA_MOVE )
	{
		if( dwVeSta & VESTA_HEAVY )
			BeginAutoRpt( 1 ); // �ƶ��س�
		else
			BeginAutoRpt( 3 ); // �ƶ��ճ�
	}
	else
	{
		if( dwVeSta & VESTA_HEAVY )
			BeginAutoRpt( 4 ); // ��ֹ�س�
		else
			BeginAutoRpt( 2 ); // ��ֹ�ճ�
	}
#endif
}

void CAutoRpt::BeginAutoRpt( DWORD v_dwNewSta )
{
	// �����ڼ�أ��������ϱ�
	if( g_objMonAlert.IsMonAlert() )
	{
		PRTMSG(MSG_DBG, "Now is Monitoring, AutoRpt quit\n");
		return;
	}
	// ��ȡ����
	tag2QGprsCfg objGprsCfg;
	objGprsCfg.Init();
	GetSecCfg(&objGprsCfg, sizeof(objGprsCfg), offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg));

	tagMonStatus objAutoRptSta;
	tag2QGpsUpPar objCfgPar;
	BYTE bytMonType;

	// ����Ҫ�����ϱ�
	if( 1 != objGprsCfg.m_bytGpsUpType )
	{
		EndAutoRpt();
		return;
	}

	objAutoRptSta.m_dwMonInterv = 0xffffffff;
	bool bModfySta = false; // �Ƿ�����˲���
	for( BYTE byt = 0; byt < objGprsCfg.m_bytGpsUpParCount; byt ++ )
	{
		objCfgPar = objGprsCfg.m_aryGpsUpPar[ byt ];

		// �����Ǹ���״̬�������ϴ�����
		if( v_dwNewSta != objCfgPar.m_bytStaType )
		{
			PRTMSG(MSG_DBG, "this AutoRpt type is ignore!\n");
			continue;
		}

		// ��ʱ�����ϱ�����
		bytMonType = 0x18 & objCfgPar.m_bytMonType;

		// ��ʱ�ϱ�����
		if( 0 == bytMonType || 0x10 == bytMonType ) // ���ж�ʱ�ϱ�
		{
			objAutoRptSta.m_bInterv = true;
			if( DWORD(objCfgPar.m_wMonInterval) * 1000 < objAutoRptSta.m_dwMonInterv )
			{
				objAutoRptSta.m_dwMonInterv = DWORD(objCfgPar.m_wMonInterval) * 1000;
				if( objAutoRptSta.m_dwMonInterv < 1000 ) objAutoRptSta.m_dwMonInterv = 1000;
			}
		}
		else
		{
			objAutoRptSta.m_bInterv = false;
		}

		// �����ϱ�����
		if( 0x08 == bytMonType || 0x10 == bytMonType ) // ���ж����ϱ�
		{
			objAutoRptSta.m_bSpace = true;
			objAutoRptSta.m_dwSpace = DWORD(objCfgPar.m_wMonSpace) * 10;
			if( objAutoRptSta.m_dwSpace < 10 ) objAutoRptSta.m_dwSpace = 10;
		}
		else
		{
			objAutoRptSta.m_bSpace = false;
		}

		// �ϱ�ʱ��
		if( INVALID_NUM_16 == objCfgPar.m_wMonTime )
			objAutoRptSta.m_dwMonTime = INVALID_NUM_32;
		else
			objAutoRptSta.m_dwMonTime = DWORD(objCfgPar.m_wMonTime) * 60000;

		bModfySta = true;
		break;
	}

	// �ϱ�״̬��ʼ��
	if( bModfySta ) // �������������ϱ�����
	{
		EndAutoRpt(); // ����ǰ�����

		tagGPSData objGps(0);
		g_objMonAlert.GetCurGPS( objGps, true );

		// �����ϱ���������
		objAutoRptSta.m_dLstLon = objGps.longitude;
		objAutoRptSta.m_dLstLat = objGps.latitude;
		objAutoRptSta.m_dLstSendDis = 0;
		objAutoRptSta.m_dwLstChkTime = objAutoRptSta.m_dwLstSendTime = GetTickCount();
		if( objAutoRptSta.m_bSpace )
			objAutoRptSta.m_dwChkInterv = 1000;
		else
			objAutoRptSta.m_dwChkInterv = objAutoRptSta.m_dwMonInterv;

		// �����ڴ��е�����
		m_objAutoRptSta = objAutoRptSta;

		// ��/���������ϱ���ʱ
		if( objAutoRptSta.m_bInterv || objAutoRptSta.m_bSpace )
		{
			PRTMSG(MSG_NOR, "AutoRpt Begin, Type is %d\n", v_dwNewSta );
			_Snd0154( objGps ); // ��һ֡���̷���
			_SetTimer(&g_objTimerMng,  AUTO_RPT_TIMER, m_objAutoRptSta.m_dwChkInterv, G_TmAutoRpt );
		}
	}
}

int CAutoRpt::EndAutoRpt()
{
	m_objAutoRptSta.m_bInterv = false;
	m_objAutoRptSta.m_bSpace = false;
	_KillTimer(&g_objTimerMng, AUTO_RPT_TIMER );
	
	//PRTMSG(MSG_NOR, "AutoRpt Stop\n" );
	return 0;
}

void CAutoRpt::P_TmAutoRpt()
{
	/// ��û������ݺ���Ϣ
	DWORD dwCur = GetTickCount();
	DWORD dwIntervChk = dwCur - m_objAutoRptSta.m_dwLstChkTime; // ���ϴζ�ʱ����ʱ��
	tagGPSData objGps(0);
	bool bNeedSend = false;
	int iResult = 0;
	
	// �ݼ�ʣ����ʱ��
	if( INVALID_NUM_32 != m_objAutoRptSta.m_dwMonTime )
	{
		if( m_objAutoRptSta.m_dwMonTime >= dwIntervChk )
			m_objAutoRptSta.m_dwMonTime -= dwIntervChk;
		else
			m_objAutoRptSta.m_dwMonTime = 0;
	}
	
	// �޸��ϴμ��ʱ��,�Ա�����ļ��ʱ�䵽�ж�
	m_objAutoRptSta.m_dwLstChkTime = dwCur;
	
	// ��ȡ��ǰGPS��Ϣ
	iResult = g_objMonAlert.GetCurGPS( objGps, true );
	if( iResult ) goto AUTORPTTIMER_END;
	
	/// �ж��Ƿ�Ҫ���������ϱ�����
	if( m_objAutoRptSta.m_bInterv )
	{
		DWORD dwIntervSend = dwCur - m_objAutoRptSta.m_dwLstSendTime; // ���ϴη��ͼ�����ݵ�ʱ��
		if( dwIntervSend >= m_objAutoRptSta.m_dwMonInterv ) // ����ؼ����
		{
			bNeedSend = true;
		}
	}
	if( m_objAutoRptSta.m_bSpace && !bNeedSend )
	{
		m_objAutoRptSta.m_dLstSendDis += G_CalGeoDis2(m_objAutoRptSta.m_dLstLon, m_objAutoRptSta.m_dLstLat,
			objGps.longitude, objGps.latitude);
		m_objAutoRptSta.m_dLstLon = objGps.longitude;
		m_objAutoRptSta.m_dLstLat = objGps.latitude;
		
		if( m_objAutoRptSta.m_dLstSendDis >= double( m_objAutoRptSta.m_dwSpace ) )
		{
			bNeedSend = true;
			m_objAutoRptSta.m_dLstSendDis = 0;
		}
	}
	
	/// ���������ϱ�����
	if( bNeedSend )
	{
		if( !_Snd0154(objGps) )
		{
			// ֻҪ�����˼�����ݾ͸��� (��Ҫ)
			m_objAutoRptSta.m_dwLstSendTime = dwCur; // ʹ�������ж϶�ʱ
			m_objAutoRptSta.m_dLstSendDis = 0; // ʹ�������ж϶���
			m_objAutoRptSta.m_dLstLon = objGps.longitude;
			m_objAutoRptSta.m_dLstLat = objGps.latitude;
		}
	}
	
AUTORPTTIMER_END:
	// ��̨��ֹ�����ϱ��ж�
	ENU_RPTJUGRESULT enuResult = _JugCanRpt( objGps );
	if( 0 == m_objAutoRptSta.m_dwMonTime || RPT_YES != enuResult )
	{
		EndAutoRpt();
	}
	else
	{
		static BYTE sta_bytCt = 0;
		if( 0 == sta_bytCt ++ % 10 ) // ÿ10������1�ζ�ʱ
		{
			if( m_objAutoRptSta.m_dwChkInterv < 1000 ) m_objAutoRptSta.m_dwChkInterv = 1000; // ��ȫ���
			_SetTimer(&g_objTimerMng, AUTO_RPT_TIMER, m_objAutoRptSta.m_dwChkInterv, G_TmAutoRpt );
		}
	}
}

int CAutoRpt::_Snd0154( const tagGPSData &v_objGps )
{
//#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	tag0154 objSnd;
//#endif
//#if 2 == USE_PROTOCOL
//	tag0156 objSnd;
//#endif
	tagQianGps objQianGps;
	char buf[ 500 ] = { 0 };
	int iBufLen = 0;
	int iRet = 0;
	BYTE bytDataType;

	// �������б���״̬,���ɱ�������
//	g_objMonAlert.FillAlertSymb( g_objMonAlert.GetAlertSta(), objSnd.m_bytAlertHi, objSnd.m_bytAlertLo );
	g_objMonAlert.FillAlertSymb( g_objMonAlert.GetAlertSta(), &(objSnd.m_bytAlertHi), &(objSnd.m_bytAlertLo) );

	objSnd.m_bytMonCentCount = 1; // ���ĸ�������1
	g_objMonAlert.GpsToQianGps( v_objGps, objQianGps );
	memcpy( &(objSnd.m_objQianGps), &objQianGps, sizeof(objSnd.m_objQianGps) );

//#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	bytDataType = 0x54;
//#endif

//#if 2 == USE_PROTOCOL
//	bytDataType = 0x56;		// ˾����źͱ�������ֵ (��)
//#endif

	iRet = g_objSms.MakeSmsFrame((char*)&objSnd, sizeof(objSnd), 0x01, bytDataType, buf, sizeof(buf), iBufLen);
	if( iRet ) 
		return iRet;

	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9, DATASYMB_0154 );
	if( iRet ) 
		return iRet;

	return iRet;
}

// �ж��Ƿ�����ϱ�
ENU_RPTJUGRESULT CAutoRpt::_JugCanRpt( const tagGPSData& v_objGps )
{
	int iResult = 0;
	tag2QGprsCfg objGprsCfg;
	objGprsCfg.Init();
	
	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	// �ϱ���־�ж�
	if( 1 != objGprsCfg.m_bytGpsUpType )
	{
		return RPT_CLOSE;
	}

	// �ϱ�ʱ����ж�
	BYTE bytCurHour = 0, bytCurMin = 0;
	if( objGprsCfg.m_bytRptBeginHour != objGprsCfg.m_bytRptEndHour
		|| objGprsCfg.m_bytRptBeginMin != objGprsCfg.m_bytRptEndMin )
	{
		if( 'A' == v_objGps.valid )
		{
			bytCurHour = BYTE( v_objGps.nHour );
			bytCurMin = BYTE( v_objGps.nMinute );
		}
		else
		{
			struct tm pCurrentTime;
			G_GetTime(&pCurrentTime);

			bytCurHour = BYTE( pCurrentTime.tm_hour );
			bytCurMin = BYTE( pCurrentTime.tm_min );
		}

		if( bytCurHour > objGprsCfg.m_bytRptEndHour
			||
			bytCurHour < objGprsCfg.m_bytRptBeginHour
			)
		{
			return RPT_OUTPRID;
		}
		else if(
			(bytCurHour == objGprsCfg.m_bytRptBeginHour && bytCurMin < objGprsCfg.m_bytRptBeginMin)
			||
			(bytCurHour == objGprsCfg.m_bytRptEndHour && bytCurMin > objGprsCfg.m_bytRptEndMin)
			)
		{
			return RPT_OUTPRID;
		}
	}

	return RPT_YES;
}

#endif





