#include "yx_QianStdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-Comu     ")

void G_TmChkDiaodu(void *arg, int len)
{
	g_objComu.P_TmChkDiaodu();
}

CComu::CComu()
{
	m_dwComuSta = 0;
}

CComu::~CComu()
{

}

// �����ն˲�ѯ��½����״̬
int CComu::_DealComu0b( char* v_szData, DWORD v_dwDataLen )
{
	char buf[3];
	buf[0] = 0x01;
	buf[1] = BYTE( 0x0c );

#if USE_LIAONING_SANQI == 0
	buf[2] = g_objSock.IsOnline() ? BYTE(1) : BYTE(2);
#endif
#if USE_LIAONING_SANQI == 1
	buf[2] = g_objSock.m_bDVRTcpLoged && g_objLogin.IsOnline() ? BYTE(1) : BYTE(2);
#endif
	DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);

	m_dwComuSta |= COMUSTA_DIAODUCONN;
	_SetTimer(&g_objTimerMng, CHKDIAODU_TIMER, 30000, G_TmChkDiaodu );
	
	return 0;
}

/// CFG_GPRS����ָ�֪ͨ�������ۣ�
int CComu::_DealComu35( char* v_szData, DWORD v_dwDataLen )
{
	return 0;
}

// ǧ���۶��Ž���֪ͨ
int CComu::_DealComu37( char* v_szData, DWORD v_dwDataLen )
{
	PRTMSG(MSG_NOR, "Rcv ShortMsg Data From Comu\n" );

	DWORD dwPacketNum = 0;
	int iRet = g_objRecvSmsDataMng.PushData( LV10, v_dwDataLen, v_szData, dwPacketNum, DATASYMB_RCVFROMSM );

	if( iRet )	
	{
		PRTMSG(MSG_ERR, "Push ShortMsg Data To Queue Failed, because %d\n", iRet );
	}
	
	return iRet;
}

int CComu::_DealComu41( char* v_szData, DWORD v_dwDataLen )
{
	int iRet = 0;
	if( v_dwDataLen < 1 ) return ERR_PAR;

	switch( BYTE(v_szData[0]) )
	{
	case 1: // ��������
		{
			iRet = g_objMonAlert.SetAlertTest();
			
			if( !iRet )
			{
				char buf[] = { 0x01, 0x42, 0x01 };
				DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);
			}
		}
		break;

	default:
		;
	}

	return iRet;
}

// �ٶȲ�ѯ����
int CComu::_DealComu61( char* v_szData, DWORD v_dwDataLen )
{
	double dSpeed = 0; // ��/��
	BYTE bytAnsType = 0; // Ӧ������
	bool bGetSpd = false;

#ifdef DIS_SPD_USEIO
	DWORD dwCyclePerSec = 0;
	WORD wCyclePerKm = 0;
	GetImpCfg( &wCyclePerKm, sizeof(wCyclePerKm), offsetof(tagImportantCfg, m_uni1QBiaodingCfg.m_obj1QBiaodingCfg.m_wCyclePerKm), sizeof(wCyclePerKm) );
	
	if( wCyclePerKm > 50 && g_objQianIO.IN_QueryIOSpeed(dwCyclePerSec) )
	{
		dSpeed = dwCyclePerSec * (1000.0 / wCyclePerKm); // ע�Ᵽ֤wCyclePerKm����0,�Է�ֹ��0�쳣
		bytAnsType = 1;
		bGetSpd = true;
	}
#endif

	if( !bGetSpd )
	{
		tagGPSData objGps(0);
		objGps.Init();
		g_objMonAlert.GetCurGPS( objGps, FALSE );

		dSpeed = objGps.speed;
		if( 'A' == objGps.valid ) bytAnsType = 1;
		else bytAnsType = 2;
	}

	char buf[16] = {0};
	buf[0] = 0x01;
	buf[1] = 0x60;
	buf[2] = bytAnsType;
	buf[3] = BYTE( dSpeed * MIMIAO_TO_HAIHOUR );
	DataPush(buf, 4, DEV_QIAN, DEV_DIAODU, LV2);

	return 0;
}

/// �����ն˷��͵�����������
int CComu::_DealComu70( char* v_szData, DWORD v_dwDataLen )
{
	int iRet = 0;
	int iBufLen = 0;
	BYTE bytResType = 0;
	char buf[ 300 ] = { 0 };

	tag2QGprsCfg objGprsCfg;
	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	if( 0 == v_dwDataLen || sizeof(buf) < v_dwDataLen )
	{
		bytResType = 2;
		goto DEALCOMU70_END;
	}
	
	iRet = g_objSms.MakeSmsFrame( v_szData, v_dwDataLen, 0x05, 0x41, buf, sizeof(buf), iBufLen );
	if( iRet )
	{
		bytResType = 2;
		goto DEALCOMU70_END;
	}

	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9,
		(objGprsCfg.m_bytChannelBkType_1 & 0x02) ? DATASYMB_SMSBACKUP : 0 );
	if( iRet )
	{
		bytResType = 2;
		goto DEALCOMU70_END;
	}

	bytResType = 1;

DEALCOMU70_END:
	buf[ 0 ] = 0x01;
	buf[ 1 ] = BYTE( 0x71 );
	buf[ 2 ] = bytResType;
	DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);

	return iRet;
}

/// ������Ϣ(���Ե�����)
// �������Э��: ����������
int CComu::_DealComu7a( char* v_szData, DWORD v_dwDataLen )
{
	int iRet = 0;
	int iBufLen = 0;
	BYTE bytResType = 0;
	char buf[ 200 ] = { 0 };

	tag2QGprsCfg objGprsCfg;
	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );
	
	/// ���͵���Ӧ��(��������Ϣ)������
	// ������Э��: ����������
	iRet = g_objSms.MakeSmsFrame( NULL, 0, 0x05, 0x7c, buf, sizeof(buf), iBufLen, CVT_8TO6 );
	if( iRet )
	{
		bytResType = 0;
		goto DEALCOMU7A_END;
	}
	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9,
		(objGprsCfg.m_bytChannelBkType_1 & 0x02) ? DATASYMB_SMSBACKUP : 0 );
	if( iRet )
	{
		bytResType = 0;
		goto DEALCOMU7A_END;
	}

	PRTMSG(MSG_NOR, "Rcv From Diaodu: Dianzao qiangda, Send To Cent\n" );

	bytResType = 1;

DEALCOMU7A_END:

	// ����ȷ��Ӧ�𵽵�����
	buf[ 0 ] = 0x01;
 	buf[ 1 ] = BYTE( 0x76 );
	buf[ 2 ] = bytResType;
	DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);

	return iRet;
}

/// ���ٽ������(���Ե�����)
// �������Э��: ���ٽ��(1) ���ٽ����0x00��ʧ�ܣ� 0x01���ɹ���
int CComu::_DealComu7b( char* v_szData, DWORD v_dwDataLen )
{	
	int iRet = 0;
	int iBufLen = 0;
	BYTE bytResType = 0;
	char buf[ 200 ] = { 0 };

	tag2QGprsCfg objGprsCfg;
	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	if( 0 == v_dwDataLen || v_dwDataLen < 1 )
	{
		iRet = ERR_PAR;
		goto DEALCOMU7B_END;
	}

	/// ���͵��ٽ������Ӧ������
	// ������Э��: ���ٽ��(1)  ���ٽ����0x00��ʧ�ܣ� 0x01���ɹ���
	iRet = g_objSms.MakeSmsFrame( v_szData, v_dwDataLen, 0x05, 0x7e, buf, sizeof(buf), iBufLen, CVT_8TO6 );
	if( iRet )
	{
		goto DEALCOMU7B_END;
	}

	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9,
		(objGprsCfg.m_bytChannelBkType_1 & 0x02) ? DATASYMB_SMSBACKUP : 0 );
	if( iRet )
	{
		goto DEALCOMU7B_END;
	}

	PRTMSG(MSG_NOR, "Rcv from diaodu:Dianzao result,Send to Cent\n" );

	bytResType = 1;

DEALCOMU7B_END:
	// ����ȷ��Ӧ�𵽵�����
	buf[ 0 ] = 0x01;
	buf[ 1 ] = BYTE( 0x76 );
	buf[ 2 ] = bytResType;
	DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);

		
#if USE_LEDTYPE == 2
		g_objLedChuangLi.DealEvent(0x04, 'B', 3);	// ���۵��ٳɹ���񣬶�ȥ��LED���ġ����١���ʾ
#endif

	return iRet;
}


// ��������̨���ͽ�������:	�������ͣ�1��
// �������ͣ�01000TTT
// TTT����������
// 000-ҽ������
// 001-��ͨ����
// 010-����
// ������Ч
//  
// int CComu::_DealComu80( char* v_szData, DWORD v_dwDataLen )
// {
// 	return g_objHelp.DealComu80(v_szData, v_dwDataLen);
// }

// �����յ���̨ת��������������Ӧ�󣬸���̨Ӧ��: Ӧ�����ͣ�1��01H-��ȷ,02H-����
int CComu::_DealComu82( char* v_szData, DWORD v_dwDataLen )
{
	return 0;
}

// ��������̨����ȡ������	�������ͣ�1��
int CComu::_DealComu84( char* v_szData, DWORD v_dwDataLen )
{
	return 0;
}

// �����յ���̨ת��������ȡ��������Ӧ�󣬸���̨Ӧ��: Ӧ�����ͣ�1��01H-��ȷ,02H-����
int CComu::_DealComu86( char* v_szData, DWORD v_dwDataLen )
{
	return 0;
}

/// ����ͷ�������ȥ����
int CComu::_DealComuE0( char* v_szData, DWORD v_dwDataLen )
{
	return 0;
}

/// ���ݵ�������ȥ����
int CComu::_DealComuE2()
{
	return 0;
}

/// ���򴰿��л�����ȥ����
int CComu::_DealComuEE( char* v_szData, DWORD v_dwDataLen )
{
	switch(v_szData[0])
	{
	case 0xdd:
		g_objCarSta.ResumeDrvSta();
		break;

	default:
		break;
	}
	return 0;
}

/// ��̨״̬�仯֪ͨ
int CComu::_DealComuF0( char* v_szData, DWORD v_dwDataLen )
{
	if( 1 != v_dwDataLen ) return ERR_PAR;
	static bool sta_bFst = true;

	DWORD dwVeSta = g_objCarSta.GetVeSta();
	switch( BYTE(v_szData[0]) ) 
	{
	case 0x01: // �����ƶ�
		{
			if( sta_bFst || !(VESTA_MOVE & dwVeSta) )
			{
				PRTMSG(MSG_NOR, "Rcv Vehicle Move Msg\n" );

				dwVeSta |= VESTA_MOVE; // �ȡ���������״̬
				g_objCarSta.SetVeSta( dwVeSta );
				
#if USE_AUTORPT == 1
				//g_objAutoRpt.InitAutoRpt(); // �١������³�ʼ�������ϱ�
				g_objAutoRpt.BeginAutoRpt(1);
#endif

				sta_bFst = false;

#if USE_LEDTYPE == 1
				//070903 hong �����˶�״̬�仯�������ƶ���LED����
				if (g_objLedBohai.m_bytLedCfg & 0x02)
				{
					g_objLedBohai.m_bytVeSta = 2;
					g_objLedBohai.light();
					PRTMSG(MSG_NOR, "Light on LED\n");
				}
#endif
			}
		}
		break;
		
	case 0x02: // ���澲ֹ
		{
			if( sta_bFst || (VESTA_MOVE & dwVeSta) )
			{
				PRTMSG(MSG_NOR, "Rcv Vehicle Stop Msg\n" );

				dwVeSta &= ~VESTA_MOVE; // �ȡ���������״̬
				g_objCarSta.SetVeSta( dwVeSta );
				
#if USE_AUTORPT == 1
				//g_objAutoRpt.InitAutoRpt(); // �١������³�ʼ�������ϱ�
				g_objAutoRpt.BeginAutoRpt(2);
#endif

				sta_bFst = false;

#if USE_LEDTYPE == 1
				//070903 hong �����˶�״̬�仯��������ֹ��LED��ʱ����
				if (g_objLedBohai.m_bytLedCfg&0x02)
				{
					g_objLedBohai.m_bytVeSta = 1;
					g_objLedBohai.m_dwVeStaTick = GetTickCount();
					PRTMSG(MSG_NOR, "Black LED when LED is on stall\n");
				}
#endif
			}
		}
		break;
		
	default:
		;
	}

	return 0;
}

/// ��������绰Ӧ��
int CComu::_DealComuF2( char* v_szData, DWORD v_dwDataLen )
{
	if( v_dwDataLen < 1 ) return ERR_PAR;

	return g_objMonAlert.DealPhoneAutoReqLsnRes( BYTE(v_szData[0]) );
}

//��ȥ����
int CComu::_DealComuF3( char* v_szData, DWORD v_dwDataLen )
{
	return 0;
}

void CComu::P_TmChkDiaodu()
{
	m_dwComuSta &= ~COMUSTA_DIAODUCONN;
}

DWORD CComu::GetComuSta()
{
	return m_dwComuSta;
}

void CComu::DealComuMsg(char *v_szbuf, DWORD v_dwlen, BYTE  v_bytSrcSymb)
{
	switch( v_bytSrcSymb )
	{
	case DATASYMB_DIAODUTOQIAN:
		switch( BYTE(v_szbuf[0]) )
		{
		case 0x0b: // ��ѯ��½����״̬
			_DealComu0b( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x35: // GPRS����״̬֪ͨ (�޸����ú��֪ͨ)
			_DealComu35( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x41: // �������Ժ��޳�
			_DealComu41( v_szbuf + 1, v_dwlen - 1 );
			break;

#if USE_DRIVERCTRL == 1
		case 0x63: // ����˾����¼Ӧ��
			g_objDriverCtrl.DealComu63( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x65: // ����˾�����
			g_objDriverCtrl.DealComu65( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x6C: // ˾���ǳ�ָʾӦ��
			g_objDriverCtrl.DealComu6c( v_szbuf + 1, v_dwlen - 1 );
			break;
#endif
			
			
#if USE_DRVREC == 1	// JG2�г���¼�ǵ�����Э��
		case 0x61: // �����ٶ�
		case 0x63: // ����˾����¼Ӧ��
		case 0x65: // ����˾�����
		case 0x67: // �������ش���������
		case 0x69: // ���󴫸���״̬
		case 0x6B: // �����ı���Ϣ��ʾӦ��
		case 0x6C: // ˾���ǳ�ָʾӦ��
		case 0x6D: // ��������ϵ��ָ��
		case 0x6F: // ��������ָ��
			g_objDriveRecord.hdl_diaodu((byte*)v_szbuf, v_dwlen);
			break;
#endif

#if USE_OIL == 3
		case 0xC1:
		case 0xC3:
		case 0xC4:
		case 0x51:
			g_objOil.DealDiaoDu(v_szbuf + 1, v_dwlen - 1);
			break;
#endif
			
#if USE_DRVREC == 0
		case 0x61: // �����ٶ�
			_DealComu61( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x6D: // ��������ϵ��ָ��
			g_objCfg.DealComu6d( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x67: // �������ش���������
		case 0x69: // ���󴫸���״̬
		case 0x6B: // �����ı���Ϣ��ʾӦ��
			break;
#endif
			
		case 0x70: // �����͵�������
			_DealComu70( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x7a: // ������Ϣ(���Ե�����)
			_DealComu7a( v_szbuf + 1, v_dwlen - 1 );
			//�����µ��ٹ���
			//g_objDiaodu.Send0550();
			break;
			
		case 0x7b: // ���ٽ������(���Ե�����)
			_DealComu7b( v_szbuf + 1, v_dwlen - 1 );
			//g_objDiaodu.DealComu7B(v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x80: // ����������
			g_objHelp.DealComu80( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x82: // �յ�����������Ӧ��Ӧ��
			_DealComu82( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x84: // �����ͳ�������
			_DealComu84( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x86: // �յ����ĳ���������Ӧ��
			_DealComu86( v_szbuf + 1, v_dwlen - 1 );
			break;

#if USE_AUTORPTSTATION == 1
		case 0x8e: // ��վ��ҵ��
			g_objAuoRptStationNew.DealComu8e( v_szbuf + 1, v_dwlen - 1 );
			break;
#endif
			
		case 0xe0: // ����ͷ����ָʾ
			_DealComuE0( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0xe2: // ���ݵ�������
			_DealComuE2();
			break;
			
		case 0xee: // �л����Ƶ�
			_DealComuEE( v_szbuf + 1, v_dwlen - 1);
			break;

		//���촥����Э��
		case 0x3c:
			g_objDiaodu.DealComu3C(v_szbuf + 1, v_dwlen - 1);
			break;

#if USE_METERTYPE == 2
		case 0x48:
			g_objKTMeter.DealComu48(v_szbuf + 1, v_dwlen - 1);
			break;
		case 0x4a:
			g_objKTMeter.DealComu4A(v_szbuf + 1, v_dwlen - 1);
			break;
#endif

		case 0x73://������Ϣ����ָʾӦ��
			g_objDiaodu.DealComu73(v_szbuf + 1, v_dwlen - 1);
			break;
		case 0xe9://�豸���--Ӧ����ComuExe�е�GPS��GPRS״̬���ʴ�����ʱתΪE9
			g_objKTDeviceCtrl.DealComuAD(v_szbuf, v_dwlen);
			break;
		case 0xd5://���ӷ���֤��������Ӧ��
			g_objKTDriverCtrl.DealComuD5(v_szbuf + 1, v_dwlen - 1);
			break;
		case 0xd7://������Ƭ�����Ӧ��
			g_objKTDriverCtrl.DealComuD7(v_szbuf + 1, v_dwlen - 1);
			break;
		case 0xd9://��Ƭ���ݴ���Ӧ��
			g_objKTDriverCtrl.DealComuD9(v_szbuf + 1, v_dwlen - 1);
			break;
		case 0xda://������ӷ���֤
			g_objKTDriverCtrl.DealComuDA(v_szbuf + 1, v_dwlen - 1);
			break;
		case 0xdb://���󽻽Ӱ�
			g_objKTDriverCtrl.DealComuDB(v_szbuf + 1, v_dwlen - 1);
				break;

		case 0xfe:	//ComuExe֪ͨҪ����root���˳��������Flash����
			{
				_KillTimer(&g_objTimerMng, BLACK_TIMER);
				_KillTimer(&g_objTimerMng, DRIVERECORD_TIMER);
				g_objDriveRecord.m_bDrvRecQuit = true;
			}
			break;

#if USE_COMBUS == 1
		case 0xff:	// ʹ�ô�����չ�У�����������ֱ���������ʱ��������CComBusת��
			{
				DWORD dwPacketNum;
				g_objComBus.m_objComBusWorkMng.PushData((BYTE)LV1, v_dwlen-1, v_szbuf+1, dwPacketNum);
			}
			break;
#endif

#if USE_COMBUS == 2
		case 0xff:	// ʹ��150TR��ʻ��¼�ǣ�����������ֱ���������ʱ��������CCom150TRת��
			{
				DWORD dwPacketNum;
				g_objCom150TR.m_objCom150TRWorkMng.PushData((BYTE)LV1, v_dwlen-1, v_szbuf+1, dwPacketNum);
			}
			break;
#endif

		default:
			break;
		}
	break;
		
	case DATASYMB_PHONETOQIAN:
		switch( BYTE(v_szbuf[0]) )
		{				
		case 0x35: // GPRS����״̬֪ͨ
			_DealComu35( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0x37: // ���ն���֪ͨ
			_DealComu37( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0xf2: // ��������绰Ӧ��
			_DealComuF2( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		case 0xf3: // �ֻ�ģ������֪ͨ
			_DealComuF3( v_szbuf + 1, v_dwlen - 1 );
			break;
			
		default:
			break;
		}
		break;
		
	case DATASYMB_GPSTOQIAN:
		switch( BYTE(v_szbuf[0]) )
		{
		case 0xf0: // ����״̬֪ͨ
			_DealComuF0( v_szbuf + 1, v_dwlen - 1 );
			break;

		case 0xf7:	// GPS����ʡ��֪ͨ
			PRTMSG(MSG_DBG, "Gps into sleep!\n");
			g_objQianIO.SetDevSta( false, DEVSTA_GPSON );
			break;

		case 0xf8:	// GPS�˳�ʡ��֪ͨ
			PRTMSG(MSG_DBG, "Gps out sleep!\n");
			g_objQianIO.SetDevSta( true, DEVSTA_GPSON );
			break;
				
		default:
			break;
		}
		break;
			
	default:
			break;
	}
}


