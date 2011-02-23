#include "yx_QianStdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-Sock     ")

#if USE_LANGUAGE == 0
#define LANG_PAR ("����(")
#define LANG_TELIPPORT ("�ֻ��� IP �˿�")
#define LANG_AREACODE (" �����")
#define LANG_NOSET (")δ����!")
#define LANG_CHKING_U ("���ڼ��U��...")
#endif

#if USE_LANGUAGE == 11
#define LANG_PAR ("Config(")
#define LANG_TELIPPORT ("Tel IP Port")
#define LANG_AREACODE (" Areacode")
#define LANG_NOSET (") no established !")
#define LANG_CHKING_U ("U Disk Searching...")
#endif

int G_Sock_JugSmsSndFrameType(void* v_pElement, void* v_pSymb, DWORD v_dwSymbSize )
{
	if( !v_pElement || !v_pSymb || 2 != v_dwSymbSize )
		return 1;
	else
	{
		if( ((char*)v_pSymb)[0] == ((char*)v_pElement)[3]
			&&
			((char*)v_pSymb)[1] == ((char*)v_pElement)[4]  )
		{
			return 0;
		}
		else return 1;
	}
}

void G_TmTcpLog(void *arg, int len)
{
	g_objSock.TcpLog();
}

void G_TmUdpLog(void *arg, int len)
{
	g_objSock.UdpLog();
}

void G_TmTcpReConn(void *arg, int len)
{
	char buf = 0x01;	// QianExe����SockServExe���������׽���
	
	G_RecordDebugInfo("No link keep, ask for reconnn!");

	DataPush((void*)&buf, 1, DEV_QIAN, DEV_SOCK, LV3);
}

void G_TmShowMsg(void *arg, int len)
{
	static DWORD sta_dwLastShowTm = GetTickCount();
	static bool sta_bFirst = true;
	if( GetTickCount() - sta_dwLastShowTm > 60*1000 || sta_bFirst )	// ��ʾ���Ҫ����1����
	{
		sta_bFirst = false;
		sta_dwLastShowTm = GetTickCount();
	}
	else
		return;

	char tmpbuf[] = {"��½ʧ�ܣ������ֻ���IP���˿��Ƿ�������ȷ��"};
	char buf[100] = {0};
	
	buf[0] = 0x01;
	buf[1] = 0x72;
	buf[2] = 0x01;
	memcpy(buf+3, tmpbuf, sizeof(tmpbuf));

	//DataPush(buf, sizeof(tmpbuf)+3, DEV_QIAN, DEV_DIAODU, LV2);
}

CSock::CSock()
{
	m_bTcpLogged =  false;	
	m_bUdpLogged = false;
#if USE_LIAONING_SANQI == 1
	m_bDVRTcpLoged = false;
#endif
	m_iTcpLogTimers = 0;
	m_iUdpLogTimers = 0;
	m_dwTcpSndTm = m_dwTcpLogTm = GetTickCount();

	memset((void*)m_szTcpRecvFrame, 0, sizeof(m_szTcpRecvFrame) );
	m_dwTcpFrameLen = 0;

	m_obj0155DataMng.InitCfg( 100000, 7200000 );
}

CSock::~CSock()
{

}

void CSock::Init()
{

}

void CSock::TcpLog()
{
	char buf[ 200 ] = { 0 };
	int iLen = 0;

	// TCP������½ʧ�ܴ����ﵽ����
	if(m_iTcpLogTimers > TCP_MAX_LOG_TIMERS)
	{
		m_iTcpLogTimers = 0;
		_KillTimer(&g_objTimerMng, TCP_LOG_TIMER);

		buf[0] = 0x01;	// QianExe����SockServExe���������׽���	
		DataPush(buf, 1, DEV_QIAN, DEV_SOCK, LV3);
		PRTMSG(MSG_NOR, "Qian Tcp Log failed to max timers, request Tcp Reconnect!\n");

		// ������������ʾ
		G_TmShowMsg(NULL, 0);
		
		return;
	}

	m_bTcpLogged = false;
	
	_SetTimer(&g_objTimerMng, TCP_LOG_TIMER, 15000, G_TmTcpLog );
	
#if 2 == USE_PROTOCOL
	tag1PComuCfg obj1PComuCfg;
	obj1PComuCfg.Init(CDMA_NAME,CDMA_PASS);
	GetImpCfg( &obj1PComuCfg, sizeof(obj1PComuCfg),	offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	memcpy( m_objGPRS01.m_szUserIdenCode, USERIDENCODE, sizeof(m_objGPRS01.m_szUserIdenCode) );
#endif

	g_objCfg.GetSelfTel( m_objGPRS01.m_szParData, sizeof(m_objGPRS01.m_szParData) );
	
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	const BYTE GPRSLOGFRM_TYPE = 0x11;
	const BYTE GPRSLOGFRM_GRADE = 0x10;
#endif
#if 2 == USE_PROTOCOL
	const BYTE GPRSLOGFRM_TYPE = 0x01;
	const BYTE GPRSLOGFRM_GRADE = 0x04;
#endif
	if( !_MakeTcpFrame( (char*)&m_objGPRS01, sizeof(m_objGPRS01), GPRSLOGFRM_TYPE, GPRSLOGFRM_GRADE, buf, sizeof(buf), iLen ) )
	{
		PRTMSG(MSG_NOR, "Send Tcp Log Frame\n" );

		G_RecordDebugInfo("Qian Send Tcp Log Frame");

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		RenewMemInfo(0x02, 0x02, 0x01, 0x04);
#endif

		// �������Ƶ�ҵ��֧���Ӳ���з���
		DataPush(buf, iLen, DEV_QIAN, DEV_SOCK, LV3);
		m_iTcpLogTimers++;
	}
}

void CSock::UdpLog()
{
	m_iUdpLogTimers++;
	if( m_iUdpLogTimers > 3 )	// udp������½���ɹ��������Σ����������²���
	{
		m_iUdpLogTimers = 0;
		_KillTimer(&g_objTimerMng, UDP_LOG_TIMER);
		
		PRTMSG(MSG_NOR, "Phot udp log failed to max timers! Ask ComuExe to queck net!\n");
		
		// ����ComuExe���²���
		char buf[] = {0x34, 0x01};
		DataPush((void*)&buf, 2, DEV_SOCK, DEV_PHONE, LV3);
		
		m_bTcpLogged = false;	
		m_bUdpLogged = false;
		
		// �������������ʾ
		G_TmShowMsg(NULL, 0);		
		return;
	}
	
	PRTMSG(MSG_NOR, "Send Udp Log Frame\n");

	G_RecordDebugInfo("Qian Send Udp Log Frame");
	
	// ��֡����
	char buf[100] = {0};
	int  ilen = 0;
	tag1PComuCfg obj1PComuCfg;
	GetImpCfg( (void*)&obj1PComuCfg, sizeof(obj1PComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	
	// ͨ�ð�Э��,������Э��_����
	// У��		���ȼ�	����	�û�����
	// 1 Byte	1 Byte	1 Byte	n Byte
	
	// ������Э��_��һ
	// �汾�ţ�1��	У��(1)	���ȼ�(1)	����(1)	����(n)
	
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	buf[ 1 ] = 0x10;	// ���ȼ�
	buf[ 2 ] = 0x11;	// ����
	buf[3] = 0x02;		// ��������
	buf[4] = 0x01;		// �������ͣ��ֻ���
	buf[5] = 0x0f;		// �������ȣ��ֻ��ų���Ϊ15
	memcpy(buf+6, obj1PComuCfg.m_szTel, 15);
	buf[21] = 0x02;		// �������ͣ��汾��
	buf[22] = 0x01;		// �������ȣ��汾�ų���Ϊ1
	buf[23] = 0x31;		// �汾��Ϊ0x31
	buf[0] = get_crc_gprs((byte*)(buf+1), 23);	//У���
	
	ilen = 24;
#endif
	
#if USE_PROTOCOL == 2			
	buf[0] = 0x03;	// �汾��
	buf[1] = 0x00;	// У��ͣ�����0
	buf[2] = 0x04;	// ���ȼ�
	buf[3] = 0x01;	// ����
	buf[4] = 0x31;	// �û�ʶ����
	buf[5] = 0x31;
	buf[6] = 0x31;
	buf[7] = 0x31;
	buf[8] = 0x31;
	buf[9] = 0x31;
	buf[10] = 0x01;	// ��������
	buf[11] = 0x01;	// �������ͣ��ֻ���
	buf[12] = 0x0f;	// �������ȣ��ֻ��ų���Ϊ15
	memcpy(buf+13, obj1PComuCfg.m_szTel, 15);
	buf[1] = get_crc_gprs((byte*)buf+1, 27);
	
	ilen = 28;
#endif
	
	char szSendbuf[100] = {0};
	szSendbuf[0] = 0x04;
	memcpy(szSendbuf+1, buf, ilen);
	DataPush(szSendbuf, ilen+1, DEV_QIAN, DEV_SOCK, LV2);
}

void CSock::_TcpKeep( BYTE v_bytNextInterval )
{
	if( !m_bTcpLogged ) return;
	
	PRTMSG(MSG_NOR, "Rcv Cent Tcp Link Keep Req: %d m\n", v_bytNextInterval );

	G_RecordDebugInfo("Qian Rcv Cent Link Keep Req: %d m", v_bytNextInterval );
	
	char buf[ 100 ] = { 0 };
	int iLen = 0;
	
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	char* pData = NULL;
	int iDataLen = 0;
	const BYTE FRM04_GRADE = 0x10;
#endif
	
#if 2 == USE_PROTOCOL
	char cAnsType = 1;
	char* pData = &cAnsType;
	int iDataLen = sizeof(cAnsType);
	const BYTE FRM04_GRADE = 0x04;
#endif
	
	if( !_MakeTcpFrame( pData, iDataLen, 0x04, FRM04_GRADE, buf, sizeof(buf), iLen ) )
	{
		// �������Ƶ�ҵ��֧���Ӳ���з���
		DataPush(buf, iLen, DEV_QIAN, DEV_SOCK, LV2);
	}

// 	// ������
// 	char szbuf = 0x01;
// 	g_objSms.MakeSmsFrame(&szbuf, 1, 0x38, 0x34, buf, sizeof(buf), iLen);
// 	g_objSock.SOCKSNDSMSDATA( buf, iLen, LV9 );

 	_SetTimer(&g_objTimerMng, TCP_RECONN_TIMER, 4*v_bytNextInterval*60000 + 50000, G_TmTcpReConn);
}

int CSock::AnalyseSockData( const char* v_szRecv, const int v_iRecvLen )
{
	if( !v_szRecv || v_iRecvLen <= 0 ) return 0;

	// ������Э��֡
	if( 0x01 == v_szRecv[0] )
	{
		switch(v_szRecv[1])
		{
		case 0x01:		// SoceServExe֪ͨ����������
			{
#if USE_LIAONING_SANQI == 0
				TcpLog();
#endif
#if USE_LIAONING_SANQI == 1
				g_objLogin.Init();
#endif
			}
			break;
			
		case 0x02:		// SoceServExe֪ͨ�ѶϿ�����
			{
				_KillTimer(&g_objTimerMng,TCP_LOG_TIMER);
				m_bTcpLogged = false;
				m_bUdpLogged = false;
			}
			break;

		case 0x03:		// SoceServExe�����������������ʾ
			{
				G_TmShowMsg(NULL, 0);
				_SetTimer(&g_objTimerMng, SHOW_MSG_TIMER, 30000, G_TmShowMsg);
			}
			break;
			
		default:
			break;
		}

		return 0;
	}

	// ����TCPЭ��֡
	else if( 0x00 == v_szRecv[0] )
	{
		int i7ECount = 0;
		if( m_dwTcpFrameLen > 0 ) i7ECount = 1;
		
		int iBegin = 1;
		for( int i = 1; i < v_iRecvLen; i ++ )
		{
			if( 0x7e == v_szRecv[ i ] )
			{
				++ i7ECount;
				
				if( 0 == i7ECount % 2 ) // ���õ�һ֡
				{
					if( m_dwTcpFrameLen + (i - iBegin + 1) <= sizeof(m_szTcpRecvFrame) )
					{
						memcpy( m_szTcpRecvFrame + m_dwTcpFrameLen, v_szRecv + iBegin, i - iBegin + 1 );
						m_dwTcpFrameLen += (DWORD)(i - iBegin + 1);
						
						if( m_dwTcpFrameLen <= 2 )
						{
							++ i7ECount;
							iBegin = i;
							memset((void*)m_szTcpRecvFrame, 0, sizeof(m_szTcpRecvFrame) );
							m_dwTcpFrameLen = 0;
							continue;
						}
						else
						{
							// ��ת��
							_DeTranData( m_szTcpRecvFrame, m_dwTcpFrameLen );
							
							// ����У���
							if( !_ChkSum( m_szTcpRecvFrame, m_dwTcpFrameLen) )
							{
								PRTMSG(MSG_DBG, "_ChkSum Err!\n");
								memset((void*)m_szTcpRecvFrame, 0, sizeof(m_szTcpRecvFrame) );
								m_dwTcpFrameLen = 0;
								continue;
							}
							
							// Э������ʹ���
							_DealTcpData( m_szTcpRecvFrame, m_dwTcpFrameLen );
							
							memset((void*)m_szTcpRecvFrame, 0, sizeof(m_szTcpRecvFrame) );
							m_dwTcpFrameLen = 0;
						}
					}
					else
					{
						XUN_ASSERT_VALID( false, "" );
						memset((void*)m_szTcpRecvFrame, 0, sizeof(m_szTcpRecvFrame) );
						m_dwTcpFrameLen = 0;
					}
				}
				else
				{
					iBegin = i;
				}
			}
		}
		
		if( 0 != i7ECount % 2 ) // ������в�����֡����
		{
			if( m_dwTcpFrameLen + (v_iRecvLen - iBegin) < sizeof(m_szTcpRecvFrame) )
			{
				memcpy( m_szTcpRecvFrame + m_dwTcpFrameLen, v_szRecv + iBegin, v_iRecvLen - iBegin );
				m_dwTcpFrameLen += (DWORD)(v_iRecvLen - iBegin);
			}
			else
			{
				XUN_ASSERT_VALID( false, "" );
				memset((void*)m_szTcpRecvFrame, 0, sizeof(m_szTcpRecvFrame) );
				m_dwTcpFrameLen = 0;
				return 1;
			}
		}
	}
	// ����UDPЭ��֡
	else if( 0x02 == v_szRecv[0] )
	{
#if USE_LIAONING_SANQI == 0
		DealUdpFrame((char*)v_szRecv+1, (int)v_iRecvLen-1);
#endif
#if USE_LIAONING_SANQI == 1
		g_objNetWork.DealSocketData((char*)v_szRecv+1, (DWORD)v_iRecvLen-1);
#endif
	}
	
	return 0;
}

int CSock::DealUdpFrame(char *v_szRecv, const int v_iRecvLen)
{
	if( !v_szRecv || v_iRecvLen <= 0 ) return 0;
	
	// ����У���
	if( !_ChkSum( (char*)v_szRecv, v_iRecvLen) )
	{
		PRTMSG(MSG_NOR, "Rcv Udp Frame, but Chksum Err!\n" );
		return ERR_PAR;
	}
		
	// Э������ʹ���
	BYTE bytFrmType = 0;
	BYTE bytGrade = 0;
	DWORD dwDataPos = 0;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	dwDataPos = 3;
	bytGrade = BYTE( v_szRecv[ 1 ] );
	bytFrmType = BYTE( v_szRecv[ 2 ] );
#endif
#if 2 == USE_PROTOCOL
	dwDataPos = 4;
	bytGrade = BYTE( v_szRecv[ 2 ] );
	bytFrmType = BYTE( v_szRecv[ 3 ] );
#endif
	
	switch( bytFrmType )
	{
	case 0x81:
	case 0x91: // �ƶ�̨��½Ӧ��
		{			
			if( 1 == v_szRecv[ dwDataPos ] )
			{
				m_bUdpLogged = true;
				PRTMSG(MSG_NOR, "UDP Logged Succ!\n" );

				G_RecordDebugInfo("Qian Udp Log Succ");
				
				_KillTimer(&g_objTimerMng, UDP_LOG_TIMER );
				m_iUdpLogTimers = 0;
			}
			else
				PRTMSG(MSG_NOR, "UDP Logged Fail, Continue Logging...\n" );
		}
		break;	
		
	default:
		break;
	}
		
	return 0;
}

// 7D+1->7D,7D+2->7E,ͬʱȥ֡ͷβ7e
void CSock::_DeTranData(char *v_szBuf, DWORD &v_dwBufLen)
{
	XUN_ASSERT_VALID( (v_dwBufLen >= 7), "" );
	XUN_ASSERT_VALID( (0x7e == v_szBuf[0] && 0x7e == v_szBuf[v_dwBufLen - 1]), "" );
	
	char buf[ SOCK_MAXSIZE ];
	DWORD dwLen = 0;
	for( int i = 1; i < (int)(v_dwBufLen) - 1; )
	{
		if( 0x7d != v_szBuf[ i ] || i == (int)(v_dwBufLen) - 2 )
		{
			buf[ dwLen ++ ] = v_szBuf[ i++ ];
		}
		else
		{
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
			if( 1 == v_szBuf[ i + 1 ] )
#endif
#if 2 == USE_PROTOCOL
				if( 0 == v_szBuf[ i + 1 ] )
#endif
				{
					buf[ dwLen ++ ] = 0x7d;
				}
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
				else if( 2 == v_szBuf[ i + 1 ] )
#endif
#if 2 == USE_PROTOCOL
					else if( 1 == v_szBuf[ i + 1 ] )
#endif
				{
					buf[ dwLen ++ ] = 0x7e;
				}
				else
				{
					buf[ dwLen ++ ] = v_szBuf[ i ];
					buf[ dwLen ++ ] = v_szBuf[ i + 1 ];
				}
				i += 2;
		}
	}
	
	memcpy( v_szBuf, buf, dwLen );
	v_dwBufLen = dwLen;
}

// 7D+0->7D,7D+1->7E,ȥ֡ͷβ7e֮��,���У���
bool CSock::_ChkSum(char *v_szBuf, DWORD v_dwBufLen)
{
	XUN_ASSERT_VALID( (v_dwBufLen >= 5), "" );
	
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	byte sum = get_crc_gprs((byte*)(v_szBuf+1), v_dwBufLen-1);

	return sum == (BYTE)( v_szBuf[ 0 ] );
#endif

#if 2 == USE_PROTOCOL
	byte sum = get_crc_gprs((byte*)v_szBuf, v_dwBufLen);

	return sum == (BYTE)( v_szBuf[ 1 ] );
#endif
}

/// 7D+1->7D,7D+2->7E,ȥ֡ͷβ7e֮��,��û����ȫ�⿪GPRS��װ
void CSock::_DealTcpData( char* v_szBuf, DWORD v_dwBufLen )
{
	if( v_dwBufLen < TCPFRAME_BASELEN )
	{
		XUN_ASSERT_VALID( false, "" );
		return;
	}

	DWORD dwDataPos = 0;
	BYTE bytFrmType = 0;
	BYTE bytGrade = 0;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	dwDataPos = 3;
	bytGrade = BYTE( v_szBuf[ 1 ] );
	bytFrmType = BYTE( v_szBuf[ 2 ] );
#endif
#if 2 == USE_PROTOCOL
	dwDataPos = 4;
	bytGrade = BYTE( v_szBuf[ 2 ] );
	bytFrmType = BYTE( v_szBuf[ 3 ] );
#endif
	
	switch( bytFrmType )
	{
	case 0x81:
	case 0x91: // �ƶ�̨��½Ӧ��
		{
			if( 1 == v_szBuf[ dwDataPos ] ) // ���ǳɹ�Ӧ��
			{
				PRTMSG(MSG_NOR,  "TCP Logged Succ!\n" );

				G_RecordDebugInfo("Qian Tcp Log Succ");

				_KillTimer(&g_objTimerMng, TCP_LOG_TIMER );
				m_bTcpLogged = true;

				DWORD dwCur = GetTickCount();

				m_dwTcpLogTm = dwCur; // ȷ��֮�����½��е�½��ʱ�ж�
				m_iTcpLogTimers = 0;

				// ���״�TCP��½�ɹ�,������GPRS�ϱ����
				static bool sta_bFstLogged = true;
				if( sta_bFstLogged )
				{
#if USE_AUTORPT == 1
					g_objAutoRpt.Init();
#endif
					sta_bFstLogged = false;
				}

				char buf = 0x03;	// QianExe֪ͨSockServExe��½�ɹ�	
				DataPush((void*)&buf, 1, DEV_QIAN, DEV_SOCK, LV3);

				// �ر���ʾ
				_KillTimer(&g_objTimerMng, SHOW_MSG_TIMER);

				// ��ֹ��һTCP��½��ĵ�һ����·ά��û���յ�
				_SetTimer(&g_objTimerMng, TCP_RECONN_TIMER, 180000, G_TmTcpReConn); 

#if 0 == USE_PROTOCOL	
				// ���͵�½֡����������½��ʱ��ʱ��
 				UdpLog();
 				_SetTimer(&g_objTimerMng, UDP_LOG_TIMER, 20000, G_TmUdpLog);
#endif
				QIANMSG msg;
				msg.m_lMsgType = MSG_TCPTOSEND;
				msgsnd(g_MsgID, (void*)&msg, MSG_MAXLEN, 0);
			}
			else
			{
				PRTMSG(MSG_NOR, "TCP Logged Fail\n" );
				m_iTcpLogTimers = 0;

				// ͨ����������ʾ
				G_TmShowMsg(NULL, 0);
			}
		}
		break;

	case 0x82: // �ƶ�̨���ݴ���Ӧ��
		{
			if( 0x02 == v_szBuf[ dwDataPos ] ) // ���ƶ�̨δ��½
			{
				m_dwTcpLogTm = GetTickCount(); // �ȳ�ʼ��ʱ��,ȷ�����½��е�½��ʱ�ж�
				TcpLog();
			}
		}
		break;

	case 0x83: // ҵ������
		{
			_DealGprsTran( bytGrade, v_szBuf + dwDataPos, v_dwBufLen - TCPFRAME_BASELEN );
		}
		break;

	case 0x84: // ǰ�û���·̽ѯ
		{
			_TcpKeep( BYTE(v_szBuf[ dwDataPos ]) );
		}
		break;

	default:
		XUN_ASSERT_VALID( false, "" );
	}
}


/// ����GPRS�����е�ҵ��֡ (������������Ѿ��⿪GPRS��װ��)
void CSock::_DealGprsTran( BYTE v_bytGrade, char* v_szBuf, DWORD v_dwBufLen )
{
	DWORD dwPacketNum = 0;

	BYTE bytLvl = _GradeToLvl( v_bytGrade );
	g_objRecvSmsDataMng.PushData( bytLvl, v_dwBufLen, v_szBuf, dwPacketNum, DATASYMB_RCVFROMSOCK );
}

int CSock::_MakeTcpFrame( char* v_szSrcData, int v_iSrcLen, BYTE v_bytType, BYTE v_bytGrade, char* v_szDesData, int v_iDesSize, int &v_iDesLen )
{
	// ͨ�ð�,������_����
	// ��־λ	У��	���ȼ�	����	�û�����	��־λ
	// 1 Byte	1 Byte	1 Byte	1 Byte	n Byte		1 Byte
	
	// ������_��һ
	// 0x7E	�汾�ţ�1��	У��(1)	���ȼ�(1)	����(1)	����(n)	0x7E	
	
	char buf[ SOCK_MAXSIZE ] = { 0 };
	int iLen = 0;
	
	if( v_iSrcLen > sizeof(buf) - TCPFRAME_BASELEN )
	{
		XUN_ASSERT_VALID( false, "" );
		return ERR_DATA_INVALID;
	}
	
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	// ��֡
	buf[ 1 ] = char( v_bytGrade ); // ���ȼ�
	buf[ 2 ] = char( v_bytType ); // ����
	memcpy( buf + 3, v_szSrcData, v_iSrcLen );
	iLen = 3 + v_iSrcLen;
	
	// ����У���
	buf[0] = char( get_crc_gprs((byte*)(buf+1), iLen-1) );
#endif
	
#if 2 == USE_PROTOCOL
	// ��֡
	buf[ 0 ] = 3; // �汾��
	buf[ 2 ] = char( v_bytGrade ); // ���ȼ�
	buf[ 3 ] = char( v_bytType ); // ����
	memcpy( buf + 4, v_szSrcData, v_iSrcLen );
	iLen = 4 + v_iSrcLen;
	
	// ����У���
	buf[1] = char( get_crc_gprs((byte*)(buf), iLen) );
#endif
	
	// ת��,��ͷβ7e
	memset( (void*)v_szDesData, 0, v_iDesSize );
	v_iDesLen = 0;
	v_szDesData[ v_iDesLen ++ ] = 0x7e;
	for( int i = 0; i < iLen; i ++ )
	{
		if( 0x7d != buf[ i ] && 0x7e != buf[ i ] )
		{
			if( v_iDesLen >= v_iDesSize )
			{
				XUN_ASSERT_VALID( false, "" );
				return ERR_BUFLACK;
			}
			v_szDesData[ v_iDesLen ++ ] = buf[ i ];
		}
		else
		{
			if( v_iDesLen >= v_iDesSize - 1 )
			{
				XUN_ASSERT_VALID( false, "" );
				return ERR_BUFLACK;
			}
			v_szDesData[ v_iDesLen ++ ] = 0x7d;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
			v_szDesData[ v_iDesLen ++ ] = buf[ i ] - 0x7d + 1;
#endif
#if 2 == USE_PROTOCOL
			v_szDesData[ v_iDesLen ++ ] = buf[ i ] - 0x7d;
#endif
		}
	}
	if( v_iDesLen >= v_iDesSize - 1 )
	{
		XUN_ASSERT_VALID( false, "" );
		return ERR_BUFLACK;
	}
	v_szDesData[ v_iDesLen ++ ] = 0x7e;
	
	// �������
	if( v_iDesLen > v_iDesSize )
	{
		XUN_ASSERT_VALID( false, "" );
		return ERR_BUFLACK;
	}
	
	return 0;
}

/// ��������UDPЭ��֡,��UDP��װ
int CSock::MakeUdpFrame( char* v_szSrcData, int v_iSrcLen, BYTE v_bytType, BYTE v_bytGrade, char* v_szDesData, int v_iDesSize, int &v_iDesLen )
{
	// ͨ�ð�Э��,������Э��_����
	// У��		���ȼ�	����	�û�����
	// 1 Byte	1 Byte	1 Byte	n Byte
	
	// ������Э��_��һ
	// �汾�ţ�1��	У��(1)	���ȼ�(1)	����(1)	����(n)
	
	char buf[ SOCK_MAXSIZE ] = { 0 };
	int iLen = 0;
	
	if( v_iSrcLen > sizeof(buf) - UDPFRAME_BASELEN )
	{
		XUN_ASSERT_VALID( false, "" );
		return ERR_DATA_INVALID;
	}
	
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	// ��֡
	buf[ 1 ] = char( v_bytGrade );
	buf[ 2 ] = char( v_bytType );
	memcpy( buf + 3, v_szSrcData, v_iSrcLen );
	iLen = 3 + v_iSrcLen;
	
	// ����У���
	buf[0] = char( get_crc_gprs((byte*)(buf+1), iLen-1) );

	// �������
	if( iLen > v_iDesSize )
	{
		XUN_ASSERT_VALID( false, "" );
		return ERR_BUFLACK;
	}
	memcpy( v_szDesData, buf, iLen );
	v_iDesLen = iLen;
	
	return 0;
#endif
	
#if 2 == USE_PROTOCOL
	// ��֡
	buf[ 0 ] = 3; // �汾��
	buf[ 2 ] = char( v_bytGrade );
	buf[ 3 ] = char( v_bytType );
	memcpy( buf + 4, v_szSrcData, v_iSrcLen );
	iLen = 4 + v_iSrcLen;
	
	// ����У���
	buf[1] = char( get_crc_gprs((byte*)buf, iLen) );

	// ת��,��ͷβ7e
	memset( (void*)v_szDesData, 0, v_iDesSize );
	v_iDesLen = 0;
	v_szDesData[ v_iDesLen ++ ] = 0x7e;
	for( int i = 0; i < iLen; i ++ )
	{
		if( 0x7d != buf[ i ] && 0x7e != buf[ i ] )
		{
			if( v_iDesLen >= v_iDesSize )
			{
				XUN_ASSERT_VALID( false, "" );
				return ERR_BUFLACK;
			}
			v_szDesData[ v_iDesLen ++ ] = buf[ i ];
		}
		else
		{
			if( v_iDesLen >= v_iDesSize - 1 )
			{
				XUN_ASSERT_VALID( false, "" );
				return ERR_BUFLACK;
			}
			v_szDesData[ v_iDesLen ++ ] = 0x7d;
			v_szDesData[ v_iDesLen ++ ] = buf[ i ] - 0x7d;
		}
	}
	if( v_iDesLen >= v_iDesSize - 1 )
	{
		XUN_ASSERT_VALID( false, "" );
		return ERR_BUFLACK;
	}
	v_szDesData[ v_iDesLen ++ ] = 0x7e;
	
	// �������
	if( v_iDesLen > v_iDesSize )
	{
		XUN_ASSERT_VALID( false, "" );
		return ERR_BUFLACK;
	}	
	return 0;
#endif
}

int CSock::TcpSendSmsData(char *v_szBuf, int v_iBufLen, BYTE v_bytLvl, BYTE v_bytSymb /*= 0*/, char* v_szSmsTel /*=NULL*/ )
{
	int iResult = 0;
	char szSendBuf[TCP_SENDSIZE] = {0};
	int  iSendLen = 0;
	BYTE bytGrade = 0;

	// ��ǿ��ʹ�ö��ŷ��͵�,������ʹ�ö��ŷ���
	if( DATASYMB_USESMS & v_bytSymb )
	{
		iResult = _SmsSndSockData( v_szBuf, DWORD(v_iBufLen), true, v_szSmsTel, 15 );
		if( ERR_SMS_DESCODE == iResult )
		{
			v_bytSymb &= ~DATASYMB_USESMS;
			v_bytSymb &= ~DATASYMB_SMSBACKUP;
		}
		else if( !iResult )
		{
			return iResult;
		}
	}

	// �������뷢�Ͷ���(���ڲ������GPRS֡,�ٷ���ʱ����֡)
	DWORD dwPacketNum = 0;
	iResult = g_objTcpSendMng.PushData( v_bytLvl, v_iBufLen, v_szBuf, dwPacketNum, v_bytSymb, v_szSmsTel, 15 );

	if( iResult ) 
		return iResult;
	else
	{
		if( !IsOnline() )
		{
			_TcpBufTurnSms(); // TCP�����������ת�ɶ��ŷ���
			_TcpBufSave0155(); // TCP�������0154����תΪä���������ݱ���
		}
		else
		{
			QIANMSG msg;
			msg.m_lMsgType = MSG_TCPTOSEND;
			msgsnd(g_MsgID, (void*)&msg, MSG_MAXLEN, 0);
		}
	}

	return 0;
}

void CSock::SendTcpBufData()
{
	char szOrignBuf[ TCP_SENDSIZE ] = { 0 }, szNewBuf[ TCP_SENDSIZE ] = { 0 }, szReserve[64] = { 0 };
	DWORD dwOrignLen;
	DWORD dwPushTm = 0;
	int dwNewLen;
	int iResult;
	BYTE bytLvl;
	BYTE bytSymb = 0; // Ҫ��ʼ��
	BYTE bytGrade;
	bool b0155Data = false;

	do 
	{
		memset( szReserve, 0, sizeof(szReserve) );

		iResult = g_objTcpSendMng.PopData( bytLvl, sizeof(szOrignBuf), dwOrignLen, szOrignBuf, dwPushTm,
			&bytSymb, szReserve, sizeof(szReserve) );
		
		if( iResult )
		{
			iResult = m_obj0155DataMng.PopData( bytLvl, sizeof(szOrignBuf), dwOrignLen, szOrignBuf, dwPushTm,
				&bytSymb, szReserve, sizeof(szReserve) );
			if( iResult ) break;
			else b0155Data = true;
		}
		else b0155Data = false;

		if( !dwOrignLen || dwOrignLen > sizeof(szOrignBuf) )
			continue;

		if( !(DATASYMB_DONETCPFRAME & bytSymb) )
		{
			bytGrade = _LvlToGrade( bytLvl );
			iResult = _MakeTcpFrame(szOrignBuf, int(dwOrignLen), 0x02, bytGrade, szNewBuf, sizeof(szNewBuf), dwNewLen);
			if( iResult ) continue;
		}
		else
		{
			dwNewLen = dwOrignLen < sizeof(szNewBuf) ? dwOrignLen : sizeof(szNewBuf);
			memcpy( szNewBuf, szOrignBuf, dwNewLen );
		}

		iResult == DataPush(szNewBuf, dwNewLen, DEV_QIAN, DEV_SOCK, LV2);
	}while( iResult == 0 ); 
}

void CSock::_TcpBufTurnSms()
{
	CInnerDataMng objTcpDataMng( QUEUE_TCPSND_SIZE, QUEUE_TCPSND_PERIOD );
	int iResult = 0;
	DWORD dwBufLen = 0;
	DWORD dwPushTm = 0;
	DWORD dwPacketNum = 0;
	char buf[ SOCK_MAXSIZE ];
	char szReserv[ 16 ] = { 0 };
	BYTE bytLvl = 0, bytSymb = 0;
	
	bool bLevData = false;
	do
	{
		memset( buf, 0, sizeof(buf) );
		memset( szReserv, 0, sizeof(szReserv) );
		iResult = g_objTcpSendMng.PopData( bytLvl, sizeof(buf), dwBufLen, buf, dwPushTm, &bytSymb, szReserv, sizeof(szReserv) );
		
		if( iResult )
			break;

		if( !dwBufLen || dwBufLen > sizeof(buf) )
			break;
		
		if( !(DATASYMB_SMSBACKUP & bytSymb) && !(DATASYMB_USESMS & bytSymb) )
		{
			objTcpDataMng.PushData( bytLvl, dwBufLen, buf, dwPacketNum, bytSymb, szReserv, sizeof(szReserv) );
			bLevData = true;
		}
		else
		{
			iResult = _SmsSndSockData( buf, dwBufLen, true, szReserv, sizeof(szReserv) );
			if( ERR_SMS_DESCODE == iResult )
			{
				objTcpDataMng.PushData( bytLvl, dwBufLen, buf, dwPacketNum, 
					(bytSymb & ~DATASYMB_SMSBACKUP) & ~DATASYMB_USESMS, szReserv, sizeof(szReserv) );
				bLevData = true;
			}
		}
	} while( !iResult );
	
	if( bLevData ) g_objTcpSendMng = objTcpDataMng;
}

void CSock::_TcpBufSave0155()
{
	DWORD dwBufLen = 0, dwPushTm, dwPacketNum = 0;
	BYTE bytLvl = 0;
	char buf[200] = { 0 };
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	const char szCmp[] = {0x01, 0x54};
#endif
#if 2 == USE_PROTOCOL
	const char szCmp[] = {0x01, 0x56};
#endif
	
	while( !g_objTcpSendMng.PopSymbData(G_Sock_JugSmsSndFrameType, (void*)szCmp, sizeof(szCmp), bytLvl, sizeof(buf), dwBufLen, buf, dwPushTm) )
	{
		if( dwBufLen > QIANUPTCPFRAME_BASELEN )
		{
#if 0 == USE_PROTOCOL
			buf[ 4 ] = 0x55; // �޸�Ϊä����������
#endif
#if 1 == USE_PROTOCOL
			buf[ 1 ] = 0x55; // �޸�Ϊä����������
#endif
#if 2 == USE_PROTOCOL
			buf[ 5 ] = 0x55; // �޸�Ϊä����������
			dwBufLen -= 3; // תΪä����������ʱȥ��һЩ�ֶ�
#endif			
			buf[ dwBufLen - 1 ] = char( get_crc_sms((unsigned char*)buf, dwBufLen - 1) ); // ���¼���У���
			
			m_obj0155DataMng.PushData( bytLvl, dwBufLen, buf, dwPacketNum );
		}
	}
}

int CSock::_SmsSndSockData( char* v_szBuf, DWORD v_dwBufLen, bool v_bFromTcp, char* v_szDesCode, DWORD v_dwDesCodeLen )
{
	// 0x38 + ����ģ�飨1����Ŀ���ֻ��Ż��ط��ţ�16�����������ݣ�n��
	// ע1:����ģ��Ϊ1,���ʾ��ǧ����ģ��
	// ����Ϊ2,���ʾ����ý��ģ������
	// ע2��Ŀ���ֻ����ط��ţ���0��β������ո�
	
	int iResult = 0;
	bool bNeedAddSelfTel = false;
	char szSnd[ 400 ] = { 0 };
	char szTmp[ 300 ] = { 0 };
	DWORD dwSndLen = 0;
	DWORD dwTmpLen = 0;
	bool bUseSmsCodeInCfg = false; // �����ʼ��Ϊfalse
	szSnd[ 0 ] = 0x38;
	szSnd[ 1 ] = 1;
	
	// �����շ��ط���
	if( !v_szDesCode || v_dwDesCodeLen < 1 || v_dwDesCodeLen > 16 )
		// ���������޷�ȡ�ý��շ��ط���,�����ط��Ų������Ȳ���ȷ,��ʹ�������е��ط���
	{
		bUseSmsCodeInCfg = true;
	}
	else
	{
		// ���������е��ط���
		memcpy( szSnd + 2, v_szDesCode, v_dwDesCodeLen );
		
		// ���ɶ���Ӧ��ʱ��Ŀ���ֻ���
		szSnd[ 2 + 16 - 1 ] = 0;
		char* pBlank = strchr( szSnd + 2, 0x20 );
		if( pBlank ) *pBlank = 0;
		
		if( strlen(szSnd + 2) < 1 )
		{
			bUseSmsCodeInCfg = true;
		}
	}
	if( bUseSmsCodeInCfg )
	{
		g_objCfg.GetSmsTel( szSnd + 2, 16 ); // ���ز�������NULL��β��
	}
	if( 0 == szSnd[2] || 0x20 == szSnd[2] || 0 != szSnd[17] )
	{ // ����������ط�����Ч
		return ERR_SMS_DESCODE;
	}
	
	// ���淢������,׼������
	if( v_dwBufLen > sizeof(szTmp) )
		return ERR_BUFLACK;
	memcpy( szTmp, v_szBuf, v_dwBufLen );
	dwTmpLen = v_dwBufLen;
	
	// ����TCP����,���ӱ����ֻ���
	if( v_bFromTcp )
		bNeedAddSelfTel = true;
	else
		bNeedAddSelfTel = false;
	if( bNeedAddSelfTel )
	{
		iResult = _AddSelfTel( szTmp, dwTmpLen, sizeof(szTmp) ); // tcp����û�м��ֻ���,תΪ���ŷ���ʱҪ����
		if( iResult ) return iResult;
	}
	
	// 7->6
	dwSndLen = Code7To6( szSnd + 18, szTmp, dwTmpLen, sizeof(szSnd) - 18 );
	if( dwSndLen > 0 )
	{
		// �ϳɺõ��������뷢�Ͷ���
		iResult = DataPush(szSnd, dwSndLen+18, DEV_QIAN, DEV_PHONE, LV2);
		if(iResult != 0) return iResult;
	}
	
	return 0;
}

int CSock::_AddSelfTel( char* v_szBuf, DWORD &v_dwLen, size_t v_sizBuf )
{
	if( v_dwLen < QIANUPTCPFRAME_BASELEN ) return ERR_PAR;
	if( v_dwLen + 15 > v_sizBuf ) return ERR_SPACELACK;
	
	DWORD dwTelAddPos = 0;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	dwTelAddPos = 5;
#endif
#if 2 == USE_PROTOCOL
	dwTelAddPos = 6;
#endif
	
	for( DWORD dw = v_dwLen - 1; dw >= dwTelAddPos; dw -- )
		v_szBuf[ dw + 15 ] = v_szBuf[ dw ];
	
	v_dwLen += 15;
	int iRet = g_objCfg.GetSelfTel( v_szBuf + dwTelAddPos, 15 );
	
	if( !iRet )
	{
		// ����SMS֡��У���
		v_szBuf[ v_dwLen - 1 ] = char( get_crc_sms( (BYTE*)v_szBuf, v_dwLen - 1 ) );
	}
	return iRet;
}


bool CSock::IsOnline()
{
	//return (bool)(m_bTcpLogged & m_bUdpLogged);
	return m_bTcpLogged;
}

void CSock::LetComuChkGPRS()
{
	static DWORD sta_dwReqTm = 0;
	DWORD dwCur = GetTickCount();
	if( 0 == sta_dwReqTm || dwCur - sta_dwReqTm >= 2500 )
	{
		PRTMSG(MSG_NOR, "Req Comu Chk net\n" );
		char buf[] = {0x34, 0x02};
		DataPush(buf, 2, DEV_QIAN, DEV_PHONE, LV2);
		sta_dwReqTm = dwCur;
	}
}



