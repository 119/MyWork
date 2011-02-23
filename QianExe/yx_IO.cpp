#include "yx_QianStdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-QianIO   ")

void G_TmAccChk(void *arg, int len)
{
	g_objQianIO.P_TmAccChk();
}

CQianIO::CQianIO()
{

}

CQianIO::~CQianIO()
{

}

int CQianIO::Init()
{
	m_dwMeterComuCycle = 0;
	m_dwDeviceSta = 0;
	m_bPowLowSta = false;
	m_dwTmRcvDCD=0;
	m_dwAccInvalidTm = GetTickCount();

	pthread_mutex_init(&m_MutexDevSta, NULL);
}

int CQianIO::Release()
{
	pthread_mutex_destroy( &m_MutexDevSta );
}

void CQianIO::P_TmAccChk()
{
	_KillTimer(&g_objTimerMng, ACCCHK_TIMER );
	
	unsigned char uszResult;

	if( !IOGet((byte)IOS_ACC, &uszResult) )
	{
		if( IO_ACC_ON == uszResult )
		{
//			PowerOnGps( true );
			SetDevSta( true, DEVSTA_ACCVALID );
		}
		else if( IO_ACC_ON == uszResult )
		{
			DWORD dwCur = GetTickCount();
			if( dwCur - m_dwAccInvalidTm >= ACCCHK_INTERVAL )
			{
//				PowerOnGps( false );
				//�����ֱ�����ʡ��ģʽ
				char buf[] = {0x01, 0x0c, 0x01};
				DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);
			}
			SetDevSta( false, DEVSTA_ACCVALID );
		} 
	}
	
	if( IO_ACC_ON == uszResult )
		_SetTimer(&g_objTimerMng,  ACCCHK_TIMER, ACCCHK_INTERVAL, G_TmAccChk ); // ����ʡ��
	else
		_SetTimer(&g_objTimerMng,  ACCCHK_TIMER, 10000, G_TmAccChk );
}

int CQianIO::DealIOMsg(char *v_szbuf, DWORD v_dwlen, BYTE v_bytSrcSymb)
{
	DWORD dwVeSta = g_objCarSta.GetVeSta(); // �Ȼ�ȡ��ǰ״̬

	switch( v_szbuf[0] )
	{
	case IO_ALARM_ON:
		{
			PRTMSG(MSG_NOR, "IO: Alerm\n" );

#if USE_LIAONING_SANQI == 0
			g_objMonAlert.SetAlertFoot(true);
#endif
#if USE_LIAONING_SANQI == 1
			g_objReport.SetAlert(true, ALERT_TYPE_FOOT, 0);
			g_objLNPhoto.AddPhoto( PHOTOEVT_FOOTALERM );
#endif
		}
		break;

#if CHK_VER == 1
	case IO_ALARM_OFF:
		{
			PRTMSG(MSG_NOR, "IO: Alerm\n" );
			
			g_objMonAlert.P_TmAlertInvalid();// �������汾��������ʱ��ֹͣ�������
		}
		break;
#endif
		
	case IO_ACC_ON:
		{
			PRTMSG(MSG_NOR, "IO: ACC ON\n");

			BYTE buf[] = {0x01, 0xf9, 0x01};
			DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);

			SetDevSta( true, DEVSTA_ACCVALID );

			// ����δ��ʡ����ƣ����迪��GPS��Դ
			//StartAccChk( 3000 );

#if USE_BLK == 1
			g_objBlk.BlkSaveStart();
#endif

#if USE_LEDTYPE == 1
			g_objLedBohai.LedTurnToWork();
#endif
			//g_objMonAlert.SetAlertFrontDoor( true );

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
#if USE_LIAONING_SANQI == 1
			g_objLNPhoto.AddPhoto( PHOTOEVT_ACCON );
#else
			g_objPhoto.AddPhoto( PHOTOEVT_ACCON );
#endif

#endif
#if USE_LIAONING_SANQI == 1
			g_objReport.m_accsta = 0x01;
#endif
		}
		break;

	case IO_ACC_OFF:
		{
			PRTMSG(MSG_NOR, "IO: ACC OFF\n");

			m_dwAccInvalidTm = GetTickCount(); // �ȸ��¿�ʼ��Ч��ʱ��
			
			BYTE buf[] = {0x01, 0xf9, 0x00};	
			DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);

			SetDevSta( false, DEVSTA_ACCVALID );

			// ����δ��ʡ����ƣ����迪��GPS��Դ
			//StartAccChk( ACCCHK_INTERVAL ); 

#if USE_BLK == 1
			g_objBlk.BlkSaveStop();
#endif
			
#if USE_LEDTYPE == 1
			g_objLedBohai.LedTurnToSleep();
#endif
#if USE_LIAONING_SANQI == 1
			g_objReport.m_accsta = 0x00;
#endif
		}
		break;

#if VEHICLE_TYPE == VEHICLE_M || VEHICLE_TYPE == VEHICLE_V8
	case IO_JIJIA_HEAVY:
		{
			PRTMSG(MSG_NOR, "IO: Jijia Valid\n" );
			
//			dwVeSta |= VESTA_HEAVY;
			dwVeSta &= ~VESTA_HEAVY;
			g_objCarSta.SetVeSta( dwVeSta );
			
			//���س�תΪ�ճ���LED������ʾ'����'
#if USE_LEDTYPE == 3
			g_objKTLed.SetLedShow('B',1,30*1000);
#endif
			//�������ۿ�ʼ����
			g_objKTIrdMod.KTIrdStart();

#if USE_JIJIA == 1
			g_objJijia.BeginJijia( false );
#endif

#if USE_METERTYPE == 2
			//֪ͨ��������Ϊ�ճ�
			char buf2[2];
			buf2[0] = 0xdd;
			buf2[1] = 0x02;
			DataPush(buf2, sizeof(buf2), DEV_QIAN, DEV_DIAODU, 2);
#endif
		}
		break;

	case IO_JIJIA_LIGHT:
		{
			PRTMSG(MSG_NOR, "IO: Jijia InValid\n" );

#if USE_METERTYPE == 2			
			//�ճ�ת�س�����ʼ��¼��Ӫ����λ����Ϣ
			g_objKTMeter.BeginMeterRcd();
#endif
//			dwVeSta |= ~VESTA_HEAVY;
			dwVeSta |= VESTA_HEAVY;		
			g_objCarSta.SetVeSta( dwVeSta );

#if USE_JIJIA == 1
//			g_objJijia.BeginJijia( false );
			g_objJijia.BeginJijia( true );//zzg mod
#endif

#if USE_METERTYPE == 2
			//֪ͨ��������Ϊ�س�
			char buf2[2];
			buf2[0] = 0xdd;
			buf2[1] = 0x01;
			DataPush(buf2, sizeof(buf2), DEV_QIAN, DEV_DIAODU, 2);
#endif
		}
		break;
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	case IO_POWDETM_INVALID:		// �������Դ
		{
			PRTMSG(MSG_NOR, "IO: Power off\n" );
#if USE_LIAONING_SANQI == 1
			g_objReport.SetAlert(true, ALERT_TYPE_POWOFF, 0);
#else
			g_objMonAlert.SetAlertPowOff( true );
#endif
		}
		break;

	case IO_POWDETM_VALID:			// �����Դ����
		{
			PRTMSG(MSG_NOR, "IO: Power on\n" );
			g_objMonAlert.SetAlertPowOff( false );
		}
		break;

	case IO_QIANYA_VALID:			// Ƿѹ
		{
			// �ȼ���Ƿ�ϵ磬���Ƕϵ磬�򲻽���Ƿѹ������ֱ�ӽ��жϵ籨��
			byte bytSta;
			IOGet(IOS_POWDETM, &bytSta);
			
			if( bytSta == IO_POWDETM_VALID )
			{
				// �ټ���Ƿ�Ϩ����δϨ���򲻽���Ƿѹ����
				byte bytSta1;
				IOGet(IOS_ACC, &bytSta1);
				
				if( bytSta1 == IO_ACC_OFF )
				{
					PRTMSG(MSG_NOR, "IO: Pow Brownout!\n" );
					g_objMonAlert.SetAlertPowBrownout( true );
				}
			}
			else
			{
				PRTMSG(MSG_NOR, "But Power off!\n" );
#if USE_LIAONING_SANQI == 1
			g_objReport.SetAlert(true, ALERT_TYPE_POWOFF, 0);
#else
				g_objMonAlert.SetAlertPowOff( true );
#endif
			}
		}
		break;

	case IO_QIANYA_INVALID:			// ��ѹ����
		{
			PRTMSG(MSG_NOR, "IO: Pow Brownout cancel\n" );
			g_objMonAlert.SetAlertPowBrownout( false );
#if USE_LIAONING_SANQI == 1
			g_objReport.SetAlert(true, ALERT_TYPE_POWOFF, 0);
#else
			g_objMonAlert.SetAlertPowOff( false );
#endif				
		}
		break;
	case IO_FDOOR_OPEN:
		{
			g_objMonAlert.SetAlertFrontDoor(true);
			g_objDriveRecord.m_bNeedClt = true;
		}
		break;
	case IO_FDOOR_CLOSE:
		{
			g_objMonAlert.SetAlertFrontDoor(false);
			g_objDriveRecord.m_bNeedClt = true;
		}
		break;
#if VEHICLE_TYPE == VEHICLE_V8
	case IO_ELECSPEAKER_VALID://��Ϊ������
#endif
#if VEHICLE_TYPE == VEHICLE_M2
	case IO_BDOOR_OPEN:
#endif
		{
			g_objMonAlert.SetAlertBackDoor(true);
			g_objDriveRecord.m_bNeedClt = true;
			
		}
		break;
#if VEHICLE_TYPE == VEHICLE_V8
	case IO_ELECSPEAKER_INVALID://��Ϊ�غ���
#endif
#if VEHICLE_TYPE == VEHICLE_M2
	case IO_BDOOR_CLOSE:
#endif
		{
			g_objMonAlert.SetAlertBackDoor(false);
			g_objDriveRecord.m_bNeedClt = true;
		}
		break;
		
#endif
		
	default:	
		break;
	}
}

BOOL CQianIO::PowerOnGps( bool v_bVal )
{
/*
	static BYTE sta_bytLstSta = 0; // 1,��ʾGPS���ϵ�; 2,��ʾ�ѹص�

	if( !v_bVal && 2 != sta_bytLstSta ) // ��Ҫ�ص�,��GPSû�йع���
	{
		IOSet(IOS_GPS, IO_GPSPOW_ON, NULL, 0);
		sta_bytLstSta = 2;

		PRTMSG(MSG_NOR, "Power off GPS\n" );
		
		SetDevSta( false, DEVSTA_GPSON );
	}
	else if( v_bVal && 1 != sta_bytLstSta ) // ��Ҫ�ϵ�,��GPSû���Ϲ���
	{
		IOSet(IOS_GPS, IO_GPSPOW_OFF, NULL, 0);
		sta_bytLstSta = 1;

		PRTMSG(MSG_NOR, "Power on GPS" );
		
		SetDevSta( true, DEVSTA_GPSON );
	}
	
	return TRUE;
*/
}

void CQianIO::StartAccChk(DWORD v_dwFstInterval)
{
	_KillTimer(&g_objTimerMng, ACCCHK_TIMER );
	_SetTimer(&g_objTimerMng, ACCCHK_TIMER, v_dwFstInterval, G_TmAccChk );
}

void CQianIO::StopAccChk()
{
	_KillTimer(&g_objTimerMng, ACCCHK_TIMER );
}

DWORD CQianIO::GetDeviceSta()
{
	DWORD dwRet = 0;
	
	pthread_mutex_lock( &m_MutexDevSta );
	
	dwRet = m_dwDeviceSta;
	
	pthread_mutex_unlock( &m_MutexDevSta );
	
	return dwRet;
}

void CQianIO::SetDevSta( bool v_bSet, DWORD v_dwVal )
{
	pthread_mutex_lock( &m_MutexDevSta );
	
	if( v_bSet ) m_dwDeviceSta |= v_dwVal;
	else m_dwDeviceSta &= ~v_dwVal;
	
	pthread_mutex_unlock( &m_MutexDevSta );
}

bool CQianIO::GetLowPowSta()
{
	bool bVal = false;
	
	pthread_mutex_lock( &m_MutexDevSta );
	
	bVal = m_bPowLowSta;
	
	pthread_mutex_unlock( &m_MutexDevSta );
	
	return bVal;
}

void CQianIO::SetLowPowSta( bool v_bVal )
{
	pthread_mutex_lock( &m_MutexDevSta );
	
	m_bPowLowSta = v_bVal ;
	
	pthread_mutex_unlock( &m_MutexDevSta );
}

void CQianIO::ConnectOilElec()
{
	IOSet(IOS_OILELECCTL, IO_OILELECCTL_RESUME, NULL, 0);
}

void CQianIO::DisconnOilElec()
{
	IOSet(IOS_OILELECCTL, IO_OILELECCTL_CUT, NULL, 0);
}



