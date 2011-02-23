#include "yx_common.h"

#undef MSG_HEAD
#define MSG_HEAD ("DvrExe-Sock  ")

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


void G_TmTcpReConn(void *arg, int len)
{
	char buf = 0x01;
	DataPush((void*)&buf, 1, DEV_DVR, DEV_SOCK, LV3);
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

	//DataPush(buf, sizeof(tmpbuf)+3, DEV_DVR, DEV_DIAODU, LV2);
}

CSock::CSock()
{
	m_bTcpLogged =  false;	
	m_iTcpLogTimers = 0;
	m_dwTcpSndTm = m_dwTcpLogTm = GetTickCount();

	memset((void*)m_szTcpRecvFrame, 0, sizeof(m_szTcpRecvFrame) );
	m_dwTcpFrameLen = 0;
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
		_KillTimer(&g_objTimerMng, DVRTCP_LOG_TIMER);

		buf[0] = 0x01;	// ����SockServExe���������׽���	
		DataPush(buf, 1, DEV_DVR, DEV_SOCK, LV3);
		PRTMSG(MSG_NOR, "Tcp Log failed to max timers, request Tcp Reconnect!\n");

		// ������������ʾ
		G_TmShowMsg(NULL, 0);
		
		return;
	}

	m_bTcpLogged = false;
	
#if USE_LIAONING_SANQI == 1	
	buf[0] = 0x07;
	buf[1] = m_bTcpLogged ? 0x01 : 0x00;
	DataPush(buf, 2, DEV_DVR, DEV_QIAN, LV3);
#endif
	_SetTimer(&g_objTimerMng, DVRTCP_LOG_TIMER, 15000, G_TmTcpLog );
	
#if 2 == USE_PROTOCOL
	tag1PComuCfg obj1PComuCfg;
	obj1PComuCfg.Init(CDMA_NAME,CDMA_PASS);
	GetImpCfg( &obj1PComuCfg, sizeof(obj1PComuCfg),	offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	memcpy( m_objGPRS01.m_szUserIdenCode, USERIDENCODE, sizeof(m_objGPRS01.m_szUserIdenCode) );
#endif

	GetSelfTel( m_objGPRS01.m_szParData, sizeof(m_objGPRS01.m_szParData) );
	
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

		G_RecordDebugInfo("Dvr Send Tcp Log Frame" );
		
		DataPush(buf, iLen, DEV_DVR, DEV_SOCK, LV3);
		m_iTcpLogTimers++;
	}
}

void CSock::_TcpKeep( BYTE v_bytNextInterval )
{
	if( !m_bTcpLogged ) return;
	
	PRTMSG(MSG_NOR, "Rcv Cent Tcp Link Keep Req: %d m\n", v_bytNextInterval );

	G_RecordDebugInfo("Dvr Tcp Rcv Cent Link Keep Req: %d m", v_bytNextInterval );
	
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
		DataPush(buf, iLen, DEV_DVR, DEV_SOCK, LV2);
	}

 	_SetTimer(&g_objTimerMng, DVRTCP_RECONN_TIMER, 4*v_bytNextInterval*60000 + 50000, G_TmTcpReConn);
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
				TcpLog();				
			}
			break;
			
		case 0x02:		// SoceServExe֪ͨ�ѶϿ�����
			{
				_KillTimer(&g_objTimerMng,DVRTCP_LOG_TIMER);
				m_bTcpLogged = false;
#if USE_LIAONING_SANQI == 1	
				char buf[2] = {0};
	buf[0] = 0x07;
	buf[1] = m_bTcpLogged ? 0x01 : 0x00;
	DataPush(buf, 2, DEV_DVR, DEV_QIAN, LV3);
#endif
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
								PRTMSG(MSG_NOR, "Dvr Tcp ChkSum err!\n");
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
		switch( v_szRecv[1] )
		{
#if VIDEO_BLACK_TYPE == 1
		case 0xB1:		// ��ϻ���ļ����俪ʼ֡Ӧ��
			Deal38B1((char*)v_szRecv+2, (int)v_iRecvLen-2);
			break;
			
		case 0xB2:		// ��ϻ���ļ����ݴ�������
			Deal38B2((char*)v_szRecv+2, (int)v_iRecvLen-2);
			break;
			
		case 0xB3:		// ��ϻ���ļ��������֡
			Deal38B3((char*)v_szRecv+2, (int)v_iRecvLen-2);								
			break;
#endif
			
#if VIDEO_BLACK_TYPE == 2
		case 0xD1:
			Deal38D1((char*)v_szRecv+2, (int)v_iRecvLen-2);
			break;
			
		case 0xD3:
			Deal38D3((char*)v_szRecv+2, (int)v_iRecvLen-2);
			break;
#endif
			
		case 0x03:
			Deal3803((char*)v_szRecv+2, (int)v_iRecvLen-2);
			break;
			
		case 0x35:
			Deal3835((char*)v_szRecv+2, (int)v_iRecvLen-2);
			break;
			
		case 0x36:
			Deal3836((char*)v_szRecv+2, (int)v_iRecvLen-2);
			break;
			
		default:
			break;
		}
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
			if( v_dwBufLen > 6 )	// �����ȴ���6��˵���ǵ�һ�ε�½Ӧ�𣬺������IP��ַ�Ͷ˿�
			{
				if( 1 == v_szBuf[ dwDataPos ] ) // ���ǳɹ�Ӧ��
				{
					PRTMSG(MSG_NOR,  "TCP First Logged Succ!\n" );

					G_RecordDebugInfo("Dvr Tcp First Logged Succ!" );
					
					_KillTimer(&g_objTimerMng, DVRTCP_LOG_TIMER );					
					_KillTimer(&g_objTimerMng, SHOW_MSG_TIMER);

					// ��ȡIP�˿ڣ��͵�SockServExe
					char buf[ 200 ] = { 0 };					
					buf[0] = 0x05;	// ����SockServExe���������׽��֣��Ҹ�����IP�˿�
					memcpy(buf+1, v_szBuf+dwDataPos+1, v_dwBufLen-dwDataPos-1);
					DataPush(buf, v_dwBufLen-dwDataPos, DEV_DVR, DEV_SOCK, LV3);
				}
				else
				{
					PRTMSG(MSG_NOR, "TCP First Logged Fail\n" );
					m_iTcpLogTimers = 0;
					
					// ͨ����������ʾ
					G_TmShowMsg(NULL, 0);
				}
			}
			else					// �����ǵڶ��ε�½���Ӧ��
			{
				if( 1 == v_szBuf[ dwDataPos ] ) // ���ǳɹ�Ӧ��
				{
					PRTMSG(MSG_NOR,  "TCP Second Logged Succ!\n" );

					G_RecordDebugInfo("Dvr Tcp Second Logged Succ!" );
					
					_KillTimer(&g_objTimerMng, DVRTCP_LOG_TIMER );
					m_bTcpLogged = true;
					
#if USE_LIAONING_SANQI == 1	
					char szbuf[ 2 ] = { 0 };
	szbuf[0] = 0x07;
	szbuf[1] = m_bTcpLogged ? 0x01 : 0x00;
	DataPush(szbuf, 2, DEV_DVR, DEV_QIAN, LV3);
#endif
					DWORD dwCur = GetTickCount();
					
					m_dwTcpLogTm = dwCur; // ȷ��֮�����½��е�½��ʱ�ж�
					m_iTcpLogTimers = 0;
					
					char buf = 0x03;	// ֪ͨSockServExe��½�ɹ�	
					DataPush((void*)&buf, 1, DEV_DVR, DEV_SOCK, LV3);
					
					// �ر���ʾ
					_KillTimer(&g_objTimerMng, SHOW_MSG_TIMER);
					
					// ��ֹ��һTCP��½��ĵ�һ����·ά��û���յ�
					_SetTimer(&g_objTimerMng, DVRTCP_RECONN_TIMER, 180000, G_TmTcpReConn); 
				}
				else
				{
					PRTMSG(MSG_NOR, "TCP Second Logged Fail\n" );
					m_iTcpLogTimers = 0;
					
					// ͨ����������ʾ
					G_TmShowMsg(NULL, 0);
				}
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
	char  szTel[15] = { 0 };
	GetSelfTel( szTel, sizeof(szTel) );

	DealSmsFrame(v_szBuf, v_dwBufLen, szTel, sizeof(szTel));
}

int CSock::DealSmsFrame( char* v_szBuf, int v_iBufLen, char* v_szVeTel, size_t v_sizTel)
{	
	// ������ݳ���
	if( v_iBufLen < QIANDOWNFRAME_BASELEN )
	{		
		XUN_ASSERT_VALID( false, "" );
		return ERR_PAR;
	}
	
	char* szDesTelInFrm = v_szBuf;
	char* szSrcTelInFrm = v_szBuf;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	szDesTelInFrm = v_szBuf + 5;
	szSrcTelInFrm = v_szBuf + 20;
#endif
#if 2 == USE_PROTOCOL
	szDesTelInFrm = v_szBuf + 6;
	szSrcTelInFrm = v_szBuf + 21;
#endif
	
	BYTE bytTranType, bytDataType;
#if 0 == USE_PROTOCOL || 30 == USE_PROTOCOL
	bytTranType = BYTE( v_szBuf[3] );
	bytDataType = BYTE( v_szBuf[4] );
#endif
#if 1 == USE_PROTOCOL
	bytTranType = BYTE( v_szBuf[0] );
	bytDataType = BYTE( v_szBuf[1] );
#endif
#if 2 == USE_PROTOCOL
	bytTranType = BYTE( v_szBuf[4] );
	bytDataType = BYTE( v_szBuf[5] );
#endif
	
	// �������֡��Ŀ���ֻ���
	if( 0x10 != bytTranType || 0x01 != bytDataType ) // ���������ö������ĺ�
	{
		if( v_szVeTel && v_szVeTel[0] && strncmp(szDesTelInFrm, v_szVeTel, v_sizTel) )
		{
			PRTMSG(MSG_DBG, "DesTel is uncorrect!\n");
			PRTMSG(MSG_DBG, "szDesTelInFrm is:%s\n", szDesTelInFrm);
			PRTMSG(MSG_DBG, "v_szVeTel is:%s\n", v_szVeTel);
			return ERR_PAR;
		}
	}
	
	// ���У���
	byte bytSum = get_crc_sms((byte*)v_szBuf, v_iBufLen - 1);
	if( bytSum != BYTE( v_szBuf[v_iBufLen - 1] ) )
	{
		PRTMSG(MSG_DBG, "get_crc_sms err!\n");
		return ERR_PAR;
	}
	
	// ת�����������
	AnalyzeSmsFrame( szSrcTelInFrm, szDesTelInFrm, bytTranType, bytDataType, v_szBuf + QIANDOWNFRAME_BASELEN - 1, v_iBufLen - QIANDOWNFRAME_BASELEN);
	
	return 0;
}


void CSock::AnalyzeSmsFrame( char* v_szSrcHandtel, char* v_szDesHandtel, BYTE v_bytTranType, BYTE v_bytDataType, char* v_szData, DWORD v_dwDataLen)
{
	char buf[2048] = {0};
	DWORD dwPacketNum;

	switch( v_bytTranType )
	{
	case 0x38:
		{
			buf[0] = (char)v_bytDataType;
			memcpy(buf+1, v_szData, v_dwDataLen);

			g_objQianMng.PushData(LV2, v_dwDataLen+1, buf, dwPacketNum);
		}
		break;

	default:
		break;
	}
}

// SMS��װ��ע�⣺�ɹ�ִ�к�v_iDesLen�Ÿ�ֵ������v_iDesLen���ı�
int CSock::MakeSmsFrame( char* v_szSrcData, int v_iSrcLen, BYTE v_bytTranType, BYTE v_bytDataType, char* v_szDesData, int v_iDesSize, int &v_iDesLen, int v_iCvtType /*= CVT_NONE*/, bool v_bTcp/*=true*/ )
{
// �������ݸ�ʽ�������ն� --�� �������ģ�

// ͨ�ð�,������_����
//	�����	�汾��	ҵ������	��������	��̨�ֻ���			�û�����	У����
//	2 Byte	1 Byte	1 Byte		1 Byte		15(UDP�Ͷ��Ų���)	 n Byte		1 Byte

// ������_��һ
// �����	�汾��	ҵ������	��������	��̨�ֻ���			�û�����	У����
// 2�ֽ�	2�ֽ�	1�ֽ�		1�ֽ�		15�ֽ�(����)		n�ֽ�		1�ֽ�

	int iSrcLen = 0, iLen = 0;
	bool bNeedAddTel = false;
	char szSrc[ SOCK_MAXSIZE ] = { 0 };
	char buf[ SOCK_MAXSIZE ] = { 0 };

	// �û����ݱ���ת������
	switch( v_iCvtType )
	{
	case CVT_8TO6:
		iSrcLen = (int)(Code8To6( szSrc, v_szSrcData, v_iSrcLen, sizeof(szSrc) ) );
		break;

	case CVT_8TO7:
		iSrcLen = Code8To7( v_szSrcData, szSrc, v_iSrcLen, sizeof(szSrc) );
		break;

	default:
		if( sizeof(szSrc) < v_iSrcLen ) return ERR_BUFLACK;
		memcpy( szSrc, v_szSrcData, v_iSrcLen );
		iSrcLen = v_iSrcLen;
	}

	if( v_bTcp )
	{
		if( iSrcLen > sizeof(buf) - QIANUPTCPFRAME_BASELEN )
		{
			XUN_ASSERT_VALID( false, "" );
			return ERR_BUFLACK;
		}
	}
	else
	{
		if( iSrcLen > sizeof(buf) - QIANUPUDPFRAME_BASELEN )
		{
			XUN_ASSERT_VALID( false, "" );
			return ERR_BUFLACK;
		}
	}

	/// ���¿�ʼ��֡
	tag2QServCodeCfg objServCodeCfg;
	GetSecCfg( &objServCodeCfg, sizeof(objServCodeCfg),	offsetof(tagSecondCfg, m_uni2QServCodeCfg), sizeof(objServCodeCfg) );

#if 0 == USE_PROTOCOL
	memcpy( buf, objServCodeCfg.m_szAreaCode, 2 ); // �����
	if( v_bTcp && 0 == buf[0] ) buf[0] = 0x7f; // ��������ŵ�ת�巽ʽ
	else if( !v_bTcp && 0x7f == buf[0] ) buf[0] = 0;
	if( v_bTcp && 0 == buf[1] ) buf[1] = 0x7f; // ��������ŵ�ת�巽ʽ
	else if( !v_bTcp && 0x7f == buf[1] ) buf[1] = 0;
	buf[ 2 ] = 0x31; // �汾��
	buf[ 3 ] = (char)v_bytTranType; // ҵ������
	buf[ 4 ] = (char)v_bytDataType; // ��������
	iLen = 5;
#endif

#if 1 == USE_PROTOCOL
	memcpy( buf + 2, objServCodeCfg.m_szAreaCode, 2 ); // �����
	if( v_bTcp && 0 == buf[2] ) buf[2] = 0x7f; // ��������ŵ�ת�巽ʽ
	else if( !v_bTcp && 0x7f == buf[2] ) buf[2] = 0;
	if( v_bTcp && 0 == buf[3] ) buf[3] = 0x7f; // ��������ŵ�ת�巽ʽ
	else if( !v_bTcp && 0x7f == buf[3] ) buf[3] = 0;
	buf[ 4 ] = 0x31; // �汾��
	buf[ 0 ] = (char)v_bytTranType; // ҵ������
	buf[ 1 ] = (char)v_bytDataType; // ��������
	iLen = 5;
#endif

#if 2 == USE_PROTOCOL
	memcpy( buf, objServCodeCfg.m_szAreaCode, 2 ); // �����
	if( v_bTcp && 0 == buf[0] ) buf[0] = 0x7f; // ��������ŵ�ת�巽ʽ
	else if( !v_bTcp && 0x7f == buf[0] ) buf[0] = 0;
	if( v_bTcp && 0 == buf[1] ) buf[1] = 0x7f; // ��������ŵ�ת�巽ʽ
	else if( !v_bTcp && 0x7f == buf[1] ) buf[1] = 0;
	buf[ 2 ] = 0x10; // �汾��
	buf[ 3 ] = 0x70;
	buf[ 4 ] = (char)v_bytTranType; // ҵ������
	buf[ 5 ] = (char)v_bytDataType; // ��������
	iLen = 6;
#endif

	if( v_bTcp )
		bNeedAddTel = false;
	else
		bNeedAddTel = true;

#if USE_PROTOCOL == 2
	bNeedAddTel = true;
#endif

	if( !bNeedAddTel )
	{
		memcpy( buf + iLen, szSrc, iSrcLen ); // �û�����
		iLen += iSrcLen;
	}
	else
	{
		char szTel[ 15 ] = { 0 };
		memset( szTel, 0x20, sizeof(szTel) );
		GetSelfTel( szTel, sizeof(szTel) );

		memcpy( buf + iLen, szTel, 15 );
		memcpy( buf + iLen + 15, szSrc, iSrcLen ); // �û�����

		iLen += 15 + iSrcLen;
	}

	// ����У���
	buf[ iLen ] = (char)( get_crc_sms((byte*)buf, iLen) );

	// ��װ�󳤶�
	iLen ++;

	// �������
	if( iLen > v_iDesSize )
	{
		XUN_ASSERT_VALID( false, "" );
		return ERR_BUFLACK;
	}
	memcpy( v_szDesData, buf, iLen );
	v_iDesLen = iLen;
	
	return 0;
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

void CSock::SendTcpData(char *v_szBuf, int v_iLen)
{
	if( !m_bTcpLogged )
		return;
	
	char  szSendBuf[TCP_SENDSIZE] = {0};
	int   iSendLen = 0;

	if( !_MakeTcpFrame(v_szBuf, v_iLen, (byte)0x02, (byte)0x10, (char*)szSendBuf, (int)sizeof(szSendBuf), iSendLen ) )
	{
		// �������Ƶ�ҵ��֧���Ӳ���з���
		DataPush((void*)szSendBuf, (DWORD)iSendLen, DEV_DVR, DEV_SOCK, LV2);
	}
}

bool CSock::IsOnline()
{
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

int CSock::GetSelfTel(char *v_szBuf, size_t v_sizTel )
{
	unsigned int uiLen = 0;
	tag1PComuCfg obj1PComuCfg;
	
	GetImpCfg( &obj1PComuCfg, sizeof(obj1PComuCfg),	offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	
	memset( v_szBuf, 0x20, v_sizTel );
	memcpy( v_szBuf, obj1PComuCfg.m_szTel, strlen(obj1PComuCfg.m_szTel) < v_sizTel ? strlen(obj1PComuCfg.m_szTel) : v_sizTel );
	
	if( 0x20 == v_szBuf[0] || 0x30 == v_szBuf[0] || !strcmp("", v_szBuf) )
	{
		PRTMSG(MSG_NOR, "Get SelfTel, but Incorrect or Empty!\n" );
	}
	
	return 0;
}



