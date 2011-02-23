#include "yx_QianStdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-Sms      ")

CSms::CSms()
{

}

CSms::~CSms()
{

}

// SMS��װ��ע�⣺�ɹ�ִ�к�v_iDesLen�Ÿ�ֵ������v_iDesLen���ı�
int CSms::MakeSmsFrame( char* v_szSrcData, int v_iSrcLen, BYTE v_bytTranType, BYTE v_bytDataType, char* v_szDesData, int v_iDesSize, int &v_iDesLen, int v_iCvtType /*= CVT_NONE*/, bool v_bTcp/*=true*/ )
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
		g_objCfg.GetSelfTel( szTel, sizeof(szTel) );

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

int CSms::DealSmsFrame( char* v_szBuf, int v_iBufLen, char* v_szVeTel, size_t v_sizTel, bool v_bFromSM )
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
	
	// ��������
	if( 0x10 != bytTranType || (0x11 != bytDataType && 0x01 != bytDataType) )
	{

	}
	
	// �������֡��Ŀ���ֻ���
	if( 0x10 != bytTranType || 0x01 != bytDataType ) // ���������ö������ĺ�
	{
		if( v_szVeTel && v_szVeTel[0] && strncmp(szDesTelInFrm, v_szVeTel, v_sizTel) )
		{
			return ERR_PAR;
		}
	}
	
	// ���У���
	byte bytSum = get_crc_sms((byte*)v_szBuf, v_iBufLen - 1);
	if( bytSum != BYTE( v_szBuf[v_iBufLen - 1] ) )
	{
		PRTMSG(MSG_DBG, "get_crc_sms Err!\n");
		return ERR_PAR;
	}
	
	// ת�����������
	bool bInputWholePar = false;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	if( 0x10 == bytTranType && 0x01 == bytDataType ) // �������ö������ĺ�,����������֡
		bInputWholePar = true;
#endif
#if 2 == USE_PROTOCOL
	if( 0x10 == bytTranType && 0x3f == bytDataType ) // �������ö������ĺ�,����������֡
		bInputWholePar = true;
#endif
	if( bInputWholePar )
	{
		_AnalyzeSmsFrame( szSrcTelInFrm, szDesTelInFrm, bytTranType, bytDataType, v_szBuf, v_iBufLen - 1, v_bFromSM );
	}
	else
	{
		_AnalyzeSmsFrame( szSrcTelInFrm, szDesTelInFrm, bytTranType, bytDataType, v_szBuf + QIANDOWNFRAME_BASELEN - 1, v_iBufLen - QIANDOWNFRAME_BASELEN, v_bFromSM );
	}
	
	return 0;
}

// ��Ƶ��صĽ����������뻹�Ƿ���QianExe����ֱ�Ӵ������
bool CSms::_Deal3805(char *v_szData, DWORD v_dwDataLen)
{
	int  iRet = 0;
	int  iBufLen;
	BYTE bytResType = 2;
	char buf[100] = {0};
	unsigned long ulDvrUdpIP;
	unsigned short usDvrUdpPort;
	
	// �Ƚ��г��ȼ���
	if( 21 != v_dwDataLen )
	{
		iRet = ERR_PAR;
		PRTMSG(MSG_ERR, "Deal3805 failed! 1\n");
		bytResType = 2;
		goto _DEAL3805_END;
	}
	
	// ������IP��ַ�Ͷ˿�
	switch(v_szData[0])
	{
	case 0x01:
	case 0x02:
	case 0x03:
		{
			// ֪ͨSockServExe����IP�Ͷ˿�
			buf[0] = 0x02;		// QianExe֪ͨSockServExe��Ƶ��ؽ�����IP�Ͷ˿�
			memcpy(buf+1, v_szData+1, v_dwDataLen-1);
			DataPush(buf, v_dwDataLen, DEV_QIAN, DEV_SOCK, LV2);

			bytResType = 0x01;
		}
		break;
		
	default:
		break;
		
	}
	
_DEAL3805_END:
	int iRet2 = g_objSms.MakeSmsFrame((char*)&bytResType, 1, 0x38, 0x45, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9 );
	
	return ( bytResType == 1 ? true : false);
}

// ���ݰ���ȥ��CFG_GPRS��װ��SMS��װ��,������ֻ�����һ��15�ֽڳ��ȷ�NULL���������ַ�����
void CSms::_AnalyzeSmsFrame( char* v_szSrcHandtel, char* v_szDesHandtel, BYTE v_bytTranType,
							BYTE v_bytDataType, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	char szCvt[ SOCK_MAXSIZE ];
	DWORD dwCvtLen = 0;
	
	switch( v_bytTranType )
	{
	case 0x06: // Զ��ң�� �����ش���ע�������ǰ��,���������ں���ʱǰ��break�ᱻ��ִ�У�
		switch( v_bytDataType )
		{
		case 0x01:
			g_objSpecCtrl.Deal0601( v_szSrcHandtel, v_szData, v_dwDataLen );
			break;
			
		default:
			;
		}
		break;

	case 0x01: // ���/λ�ò�ѯ/��������
		{
			switch( v_bytDataType )
			{
			case 0x01: // �������
				g_objMonAlert.Deal0101( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
				break;
				
			case 0x02: // �����������
				g_objMonAlert.Deal0102( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
				break;
				
			case 0x03: // �޸ļ�ز���
				g_objMonAlert.Deal0103( v_szSrcHandtel, v_szData, v_dwDataLen );
				break;
				
			case 0x04: // ��ֹ���Ӧ��
				break;
				
			case 0x05: // ��������ָʾ
				g_objMonAlert.Deal0105( v_szSrcHandtel, v_szData, v_dwDataLen );
				break;
				
			case 0x11: // λ�ò�ѯ
				g_objMonAlert.Deal0111( v_szSrcHandtel, v_bFromSM );
				break;
				
#if USE_METERTYPE == 1				
			case 0x19:	// �ؿͲ����Ӧ��
				g_objMeter.Deal0119( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
				break;
#endif

			case 0x36: // ����鳵
				g_objMonAlert.Deal0136( v_szSrcHandtel, v_szData, v_dwDataLen );
				break;
				
#if USE_DRVREC == 1
			case 0x17:	// ʵʱ�¹��ɵ��ϴ�Ӧ�� 
			case 0x27:	// ����ʽ�ɵ��ϴ����� 
				{
					// �����յ�������ѹ��g_drvrcd��tcprcv������
					byte tmp[2048] = {0};
					tmp[0] = v_bytTranType;
					tmp[1] = v_bytDataType;
					memcpy(tmp+2, v_szData, min(2046, v_dwDataLen));
					g_objDriveRecord.tcprcv.push(tmp, min(2046, v_dwDataLen)+2);
				}
				break;
#endif
				
			default:
				;
			}
		}
		break;

	case 0x03: // ����ҵ��
		switch( v_bytDataType )
		{
		case 0x01: // ����Ӧ��
			g_objHelp.Deal0301( v_szData, v_dwDataLen );
			break;
			
		default:
			;
		}
		break;

	case 0x05: // ���ȵ���
		switch( v_bytDataType )
		{
		case 0x01: // ���ĵ���ָʾ
			g_objDiaodu.Deal0501( v_szData, v_dwDataLen );
			break;
			
		//zzg add
		case 0x10:
			g_objDiaodu.Deal0510( v_szData, v_dwDataLen );
			break;
		case 0x11:
			g_objDiaodu.Deal0511( v_szData, v_dwDataLen );
			break;
		
		case 0x3c:	// ����ָ��(��̵���ָ��)
			{
				dwCvtLen = DWORD( Code6To8(v_szData, szCvt, int(v_dwDataLen), sizeof(szCvt)) );
				if( dwCvtLen > v_dwDataLen || dwCvtLen > sizeof(szCvt) ) break;
				PRTMSG(MSG_DBG, "Deal 053c dianzhao!\n");
				g_objDiaodu.Deal053c( szCvt, dwCvtLen );
			}
			break;
			
		case 0x3d:	// ����ȷ��(��ϸ����ָ��)
			{
				dwCvtLen = DWORD( Code6To8(v_szData, szCvt, int(v_dwDataLen), sizeof(szCvt)) );
				if( dwCvtLen > v_dwDataLen || dwCvtLen > sizeof(szCvt) ) break;
				
				g_objDiaodu.Deal053d( szCvt, dwCvtLen );
			}
			break;
			
		default:
			;
		}
		break;

	case 0x07: // ����ҵ��
		switch( v_bytDataType )
		{
		case 0x20: // ��������
			g_objMonAlert.Deal0720( v_szSrcHandtel, v_szData, v_dwDataLen );
			break;
				
		case 0x23: // ��������
			g_objMonAlert.Deal0723( v_szSrcHandtel, v_szData, v_dwDataLen );
			break;
				
		default:
			;
		}
		break;

	case 0x0e: // ��ϻ�Ӳ�ѯ
		switch( v_bytDataType )
		{
#if QIANTYPE_USE == QIANTYPE_JG2 || QIANTYPE_USE == QIANTYPE_V7
		case 0x01: // ������������ (������ô������)
			g_objHelp.Deal0e01( v_szSrcHandtel, v_szData, v_dwDataLen );
			break;
#endif
#if USE_BLK == 1
		case 0x10: // ǧ���۹�����_��һ
		case 0x20: // ��ϻ�Ӳ�ѯ����
			g_objBlk.Deal0e20( v_szData, v_dwDataLen );
			break;
			
		case 0x21: // ֹͣ��ϻ�Ӳ�ѯ����
			g_objBlk.Deal0e21( v_szData, v_dwDataLen );
			break;
#endif
		default:
			;
		}
		break;

#if USE_LEDTYPE == 1
	case 0x39:
		g_objLedBohai.Deal0x39(v_bytDataType, v_szData, v_dwDataLen);
		break;
#endif

#if USE_LEDTYPE == 2
	case 0x42:
		g_objLedChuangLi.Deal0x42(v_bytDataType, v_szData, v_dwDataLen);
		break;
#endif

	case 0x10: // ��������
		switch( v_bytDataType )
		{
		case 0x01: // �������ĺ�
			g_objCfg.Deal1001( v_szSrcHandtel, v_szData, v_dwDataLen, v_szDesHandtel, v_bFromSM );
			break;
			
		case 0x02: // ���ñ��������ȡ�˰���ֻ���
			g_objCfg.Deal1002( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
		case 0x03: // ���ü����绰
			g_objCfg.Deal1003( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
#if USE_REALPOS == 1
		case 0x07: // ����ʵʱ��λ
			g_objRealPos.Deal1007( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
#endif
			
#if USE_TELLIMIT == 0
		case 0x08: // ���ú�������
			g_objCfg.Deal1008( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
#endif
			
		case 0x09: // ��������
			g_objMonAlert.Deal1009( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
		case 0x0b: // ������������
			g_objHelp.Deal100b( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
		case 0x11: // ���������
			g_objCfg.Deal1011( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
#if USE_BLK == 1
		case 0x12: // ��ϻ�Ӽ������
			g_objBlk.Deal1012( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
#endif

#if USE_METERTYPE == 1
		case 0x15: // �ؿͲ����
			g_objMeter.Deal1015( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
#endif

		case 0x1a: // ״̬��ѯ
			g_objMonAlert.Deal101a( v_szSrcHandtel, v_szData, v_dwDataLen );
			break;
			
		case 0x3b: // ����CFG_GPRS��ʼ����
			g_objCfg.Deal103b( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
		case 0x3c: // ����IP��ַ
			g_objCfg.Deal103c( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
		case 0x3d: // ���ٱ�������
			g_objMonAlert.Deal103d( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
#if 2 == USE_PROTOCOL
		case 0x3e: // �ָ�ȱʡ����
			g_objCfg.Deal103e( v_szSrcHandtel, v_szData, v_dwDataLen, v_bFromSM );
			break;
			
		case 0x3f: // ���ö������ĺź��û�ʶ����
			g_objCfg.Deal103f( v_szSrcHandtel, v_szData, v_dwDataLen, v_szDesHandtel, v_bFromSM );
			break;
#endif
			
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4 || USE_DRVRECRPT == 11
		case 0x3e: // ���������ϴ���ʻ����
			g_objDrvRecRpt.Deal103e(v_szSrcHandtel, v_szData, v_dwDataLen );
			break;
#endif
#endif
			
		default:
			break;
		}
		break;

#if USE_TELLIMIT == 1
		case 0x33:	// ��������ҵ��
			{
				g_objPhoneBook.hdl_cent(v_bytDataType, v_szData, v_dwDataLen);	
			}
			break;		
#endif

#if USE_TELLIMIT == 2
		case 0x33: // ���д���ҵ��
			switch( v_bytDataType )
			{
			case 0x01:
				g_objCfg.Deal3301( v_szSrcHandtel, v_szData, v_dwDataLen );
				break;
				
			case 0x02:
				g_objCfg.Deal3302( v_szSrcHandtel, v_szData, v_dwDataLen );
				break;
				
			case 0x04:
				g_objCfg.Deal3304( v_szSrcHandtel, v_szData, v_dwDataLen );
				break;
				
			default:
				;
			}
			break;
#endif
			
#if 3 == USE_TELLIMIT
			case 0x33:
				switch( v_bytDataType )
				{
				case 0x01:
					g_objCfg.Deal3301( v_szSrcHandtel, v_szData, v_dwDataLen );
					break;
					
				case 0x02:
					g_objCfg.Deal3302( v_szSrcHandtel, v_szData, v_dwDataLen );
					break;
					
				default:
					;
				}
				break;
#endif

#if USE_DRVREC == 1
	case 0x09:	// ���ͳ��ҵ�� 
	case 0x36:	// �г���¼��ҵ�� 
	case 0x37:	// �г���¼�Ǳ���ҵ��
	case 0x0f:	// Խ�籨����������ҵ��
		{
			// �����յ�������ѹ��g_drvrcd��tcprcv������
			byte tmp[2048] = {0};
			tmp[0] = v_bytTranType;
			tmp[1] = v_bytDataType;
			memcpy(tmp+2, v_szData, min(2046, v_dwDataLen));

			g_objDriveRecord.tcprcv.push(tmp, min(2046, v_dwDataLen)+2);
		}
		break;
#endif

#if USE_DRVREC == 0
	case 0x36: // ��ʻ��¼��ҵ��1
		{
			dwCvtLen = DWORD( Code7To8(v_szData, szCvt, int(v_dwDataLen), sizeof(szCvt)) );
			if( dwCvtLen > v_dwDataLen || dwCvtLen > sizeof(szCvt) ) break;
			
			switch( v_bytDataType )
			{
#if USE_DRIVERCTRL == 1
			case 0x02: // ��ʻԱ�����������
				g_objDriverCtrl.Deal3602( v_szSrcHandtel, szCvt, dwCvtLen );
				break;
#endif
				
#if USE_ACDENT == 1 || USE_ACDENT == 2
			case 0x04: // �¹��ɵ��ѯ����
				g_objAcdent.Deal3604( v_szSrcHandtel, szCvt, dwCvtLen );
				break;
#endif
				
			default:
				;
			}
		}
		break;
#endif

	case 0x38:	// DVR���ҵ�񣬷��͵�DvrExe���д���
		{
			if( 0x05 == v_bytDataType )
			{
				_Deal3805(v_szData, v_dwDataLen);
			}
			else
			{
				char buf[1024] = {0};
				buf[0] = 0x02;		// 0x02 ��ʾQianExe��DvrExeת��������ָ��
				buf[1] = v_bytDataType;
				memcpy(buf+2, v_szData, v_dwDataLen);
				DataPush(buf, v_dwDataLen+2, DEV_QIAN, DEV_DVR, LV2);
			}			
		}
		break;

	case 0x20:	// ˰��Э��
		{
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
			// �����յ�������ѹ��g_meter��tcprcv������
			byte tmp[2048] = {0};
			tmp[0] = v_bytDataType;
			memcpy(tmp+1, v_szData, min(2047, v_dwDataLen));
#if USE_METERTYPE == 1
			g_objMeter.tcprcv.push(tmp, min(2047, v_dwDataLen)+1);
#endif

#if USE_METERTYPE == 2
			g_objKTMeter.tcprcv.push(tmp,min(2047, v_dwDataLen)+1);
#endif
		}
		break;

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	case 0x41:	// ����ҵ��
		{
			switch(v_bytDataType)
			{
			case 0x00:
				g_objPhoto.Deal4100(v_szData, v_dwDataLen);
				break;

			case 0x01:	// ����ץ������
				g_objPhoto.Deal4101(v_szData, v_dwDataLen);
				break;

			case 0x02:	// ����ץ�Ĳ�������
				g_objPhoto.Deal4102(v_szData, v_dwDataLen);
				break;

			case 0x03:	// ͨ�����ı����ѯͼƬ��ϻ��
				g_objPhoto.Deal4103(v_szData, v_dwDataLen);
				break;

			case 0x04:	// ͨ��ʱ��κ��¼���ѯ��ϻ����Ƭ����
				g_objPhoto.Deal4104(v_szData, v_dwDataLen);
				break;

			case 0x10:	// ͼƬ���ݴ���Ӧ��
				g_objPhoto.Deal4110(v_szData, v_dwDataLen);
				break;

			case 0x12:	// ��ǰ���ڳ�ʱ�ش�Ӧ��
				g_objPhoto.Deal4112(v_szData, v_dwDataLen);
				break;

			case 0x13:	// ����ϵ�����ͼƬ
				g_objPhoto.Deal4113(v_szData, v_dwDataLen);
				break;
			}
		}
		break;
#endif

    case 0x40://����˾���������
		{
			switch( v_bytDataType )
			{
				case 0x04:
					g_objKTDriverCtrl.Deal4004(v_szData, v_dwDataLen);
					break;
				case 0x05:
					g_objKTDriverCtrl.Deal4005(v_szData, v_dwDataLen);
					break;
				case 0x06:
					g_objKTDriverCtrl.Deal4006(v_szData, v_dwDataLen);
					break;
				case 0x08:
					g_objKTDriverCtrl.Deal4008(v_szData, v_dwDataLen);
					break;
				default:
					break;
			}
		}
		break;

#if USE_LEDTYPE == 3
	case 0x42://����LED�������
		{
			switch( v_bytDataType )
			{
				case 0x08:
					g_objDiaodu.Deal4208(v_szData, v_dwDataLen);
					break;
				default:
					break;
			}
		}
#endif

#if USE_AUTORPTSTATION == 1
	// �����Զ���վ��ص�ҵ��
	case 0x42:
		{
			switch( v_bytDataType )
			{
			case 0x16:
				g_objAuoRptStationNew.hdl_4216((byte*)v_szData,v_dwDataLen);
				break;
				
			case 0x17:
				g_objAuoRptStationNew.hdl_4217((byte*)v_szData,v_dwDataLen);
				break;
				
			case 0x18:
				g_objAuoRptStationNew.hdl_4218();
				break;
				
			case 0x19:
				g_objAuoRptStationNew.hdl_4219((byte*)v_szData,v_dwDataLen);
				break;
				
			case 0x20:
				g_objAuoRptStationNew.hdl_4220((byte*)v_szData,v_dwDataLen);
				break;
				
			default:
				break;
			}
		}
		break;
				
	case 0x43:
		{
			switch( v_bytDataType )
			{
			case 0x12:
				g_objDownLine.Deal4312(v_szData,v_dwDataLen);
				break;

			case 0x13:
				g_objDownLine.Deal4313(v_szData,v_dwDataLen);
				break;

			case 0x10:
				g_objDownLine.Deal4310(v_szData,v_dwDataLen);
				break;

			default:
				break;
			}
		}
		break;
#endif

	default:
		break;
	}
}


