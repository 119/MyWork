#include "yx_QianStdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-Config   ")

// �Ƴ�ĩβ�Ŀո�
void StrRemvBlank( char* v_szBuf, size_t v_sizBuf )
{
	if( v_sizBuf < 1 ) return;
	if( 0x20 != v_szBuf[v_sizBuf - 1] && 0 != v_szBuf[v_sizBuf - 1] ) return;
	v_szBuf[v_sizBuf - 1] = 0;
	char* pBlank = strchr( v_szBuf, 0x20 );
	if( pBlank ) *pBlank = 0;
}

CConfig::CConfig()
{

}

CConfig::~CConfig()
{

}

int CConfig::GetSelfTel(char *v_szBuf, size_t v_sizTel )
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

int CConfig::GetSmsTel( char* v_szTel, size_t v_sizTel )
{
	tag2QServCodeCfg objServCodeCfg;
	
	int iRet;
	iRet = GetSecCfg( &objServCodeCfg, sizeof(objServCodeCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objServCodeCfg) );
	
	if( !iRet )
	{
		if( v_sizTel < sizeof(objServCodeCfg.m_szQianSmsTel) ) return ERR_SPACELACK;
		memset( v_szTel, 0, v_sizTel );
		memcpy( v_szTel, objServCodeCfg.m_szQianSmsTel, sizeof(objServCodeCfg.m_szQianSmsTel) );
		v_szTel[ v_sizTel - 1 ] = 0;
		char* pFind = strchr( v_szTel, 0x20 );
		if( pFind ) *pFind = 0;
	}
	else
	{
		memset( v_szTel, 0, v_sizTel );
	}
	return iRet;
}

// ���ö������ĺ� (Ӧ��ɹ�����) (�������֡ͷ(35))
int CConfig::Deal1001(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen, char* v_szDesHandtel, bool v_bFromSM )
{
// �������ĺ���(22)

	tag1001 req;
	tag1PComuCfg obj1PComuCfg, obj1PComuCfgBkp;
	tag2QServCodeCfg obj2QServCodeCfg;
	char* pBlank = NULL;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	BYTE bytResType = 0;

	// ���ݲ������
	if( sizeof(req) + QIANDOWNFRAME_BASELEN - 1 != v_dwDataLen )
	{
		iRet = ERR_PAR;
		PRTMSG(MSG_ERR, "Deal1001 fail 1!, sizeof(tag1001) = %d\n", sizeof(tag1001));
		goto DEAL1001_FAILED;
	}

	// ��ȡ����
	memcpy( &req, v_szData + QIANDOWNFRAME_BASELEN - 1, sizeof(req) );

	// �ȶ�ȡ,���޸�

	iRet = GetImpCfg( &obj1PComuCfg, sizeof(obj1PComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	if( iRet ) 
		goto DEAL1001_FAILED;

	obj1PComuCfgBkp = obj1PComuCfg; // �ȱ���

	iRet = GetSecCfg( &obj2QServCodeCfg, sizeof(obj2QServCodeCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QServCodeCfg) );
	if( iRet ) 
		goto DEAL1001_FAILED;

	memcpy( obj1PComuCfg.m_szSmsCentID, req.m_szSmsCentID, sizeof(obj1PComuCfg.m_szSmsCentID) );
	StrRemvBlank( obj1PComuCfg.m_szSmsCentID, sizeof(obj1PComuCfg.m_szSmsCentID) );

	// �����޸ı����ֻ���
	memcpy( obj1PComuCfg.m_szTel, v_szDesHandtel, sizeof(obj1PComuCfg.m_szTel) );
	obj1PComuCfg.m_szTel[ sizeof(obj1PComuCfg.m_szTel) - 1 ] = 0;
	pBlank = strchr( obj1PComuCfg.m_szTel, 0x20 );
	if( pBlank ) *pBlank = 0;

	// �����޸�����ID (�������)
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 2
	obj2QServCodeCfg.m_szAreaCode[0] = v_szData[0];
	obj2QServCodeCfg.m_szAreaCode[1] = v_szData[1];
#endif
#if USE_PROTOCOL == 1
	obj2QServCodeCfg.m_szAreaCode[0] = v_szData[2];
	obj2QServCodeCfg.m_szAreaCode[1] = v_szData[3];
#endif

	if( 0x7f == obj2QServCodeCfg.m_szAreaCode[1] ) obj2QServCodeCfg.m_szAreaCode[1] = 0; // ����ʱ7f��ԭΪ0
	
	// д�غͷ���Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x41, buf, sizeof(buf), iBufLen ); 
	if( !iRet )
	{
		// ���������� (���������ֻ���)
		if( iRet = SetImpCfg(&obj1PComuCfg, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg)) )
		{
			goto DEAL1001_FAILED;
		}

		// ����
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );
		if( !iRet )
		{
			SetSecCfg( &obj2QServCodeCfg, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QServCodeCfg) ); // ����д�������

			// ֪ͨ�ײ�������Ч�µĶ������ĺ�
			char buf[] = {0x01, 0x36};
			DataPush(buf, 2, DEV_QIAN, DEV_DIAODU, LV2);
		}
		else // ����(������ʧ��)
		{
			SetImpCfg( &obj1PComuCfgBkp, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfgBkp) ); // �ָ�ԭ���� ((�����ָ��ֻ���))
			goto DEAL1001_FAILED;
		}
	}
	else goto DEAL1001_FAILED;

	return 0;

DEAL1001_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x41, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );

	return iRet;
}

/// ���ñ��������ȡ�˰���ֻ��� (Ӧ��ɹ�����)
int CConfig::Deal1002(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	tag2QServCodeCfg obj2QServCodeCfg, obj2QServCodeCfgBkp;
	tag1002 req;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	BYTE bytResType = 0;

	// ���ݲ������
	if( 15 > v_dwDataLen )
	//if( sizeof(req) != v_dwDataLen )
	{
		iRet = ERR_PAR;
		goto DEAL1002_FAILED;
	}
	memcpy( &req, v_szData, sizeof(req) );

	// ��ȡ->�޸�
	iRet = GetSecCfg( &obj2QServCodeCfg, sizeof(obj2QServCodeCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QServCodeCfg) );
	if( iRet )
		goto DEAL1002_FAILED;
	obj2QServCodeCfgBkp = obj2QServCodeCfg; // �ȱ���

	memcpy( obj2QServCodeCfg.m_szQianSmsTel, req.m_szSmsTel, sizeof(obj2QServCodeCfg.m_szQianSmsTel) );
	StrRemvBlank( obj2QServCodeCfg.m_szQianSmsTel, sizeof(obj2QServCodeCfg.m_szQianSmsTel) );
	
	if (v_dwDataLen>20) 
	{
		memcpy( obj2QServCodeCfg.m_szTaxHandtel, req.m_szTaxHandtel, sizeof(obj2QServCodeCfg.m_szTaxHandtel) );
	}
	if (v_dwDataLen>35) 
	{
		memcpy( obj2QServCodeCfg.m_szDiaoduHandtel, req.m_szDiaoduHandtel, sizeof(obj2QServCodeCfg.m_szDiaoduHandtel) );
	}

	// д�غͷ���Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x42, buf, sizeof(buf), iBufLen); // ��֡
	if( !iRet )
	{
		// ����������
		if( iRet = ::SetSecCfg(&obj2QServCodeCfg, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QServCodeCfg)) )
		{
			goto DEAL1002_FAILED;
		}

		// ����
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
		if( iRet ) // ������ʧ��
		{
			SetSecCfg( &obj2QServCodeCfgBkp, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg),	sizeof(obj2QServCodeCfgBkp) ); // �ָ�ԭ����
			goto DEAL1002_FAILED;
		}
	}
	else goto DEAL1002_FAILED;

	return 0;

DEAL1002_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x42, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );

	return iRet;
}

/// ���ü����绰: �������(1) + ����(15 * N)
int CConfig::Deal1003(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	tag2QServCodeCfg obj2QLsnCfg, obj2QLsnCfgBkp;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	BYTE bytResType = 0;
	BYTE bytTelCount = 0;
	BYTE byt = 0;

	// ���ݲ������
	if( v_dwDataLen < 16 )
	{
		iRet = ERR_PAR;
		goto DEAL1003_FAILED;
	}
	bytTelCount = v_szData[0];
	if( 0 == bytTelCount || bytTelCount > sizeof(obj2QLsnCfg.m_aryLsnHandtel) / sizeof(obj2QLsnCfg.m_aryLsnHandtel[0]) )
	{
		iRet = ERR_PAR;
		goto DEAL1003_FAILED;
	}
	if( 1 + sizeof(obj2QLsnCfg.m_aryLsnHandtel[0]) * bytTelCount != v_dwDataLen )
	{
		iRet = ERR_PAR;
		goto DEAL1003_FAILED;
	}

	// ��ȡ->�޸�
	iRet = GetSecCfg( &obj2QLsnCfg, sizeof(obj2QLsnCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QLsnCfg) );
	if( iRet )
		goto DEAL1003_FAILED;

	obj2QLsnCfgBkp = obj2QLsnCfg; // �ȱ���

	for( byt = 0; byt < bytTelCount; byt ++ )
		memcpy( obj2QLsnCfg.m_aryLsnHandtel + byt, v_szData + 1 + byt * 15, sizeof(obj2QLsnCfg.m_aryLsnHandtel[0]) );
	obj2QLsnCfg.m_bytListenTelCount = bytTelCount;

	// ����Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x43, buf, sizeof(buf), iBufLen );
	if( !iRet )
	{
		// д���ļ�
		iRet = SetSecCfg( &obj2QLsnCfg, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QLsnCfg) );		
		if( iRet )
			goto DEAL1003_FAILED;

		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
		if( iRet )
		{
			SetSecCfg( &obj2QLsnCfgBkp, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QLsnCfgBkp) );
			goto DEAL1003_FAILED;
		}
	}

	return 0;

DEAL1003_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x43, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );

	return iRet;
}

// ���ú�������: ���ÿ����ֽڣ�1��+ �绰������1��+ [ �绰�������ԣ�1��+ �绰���봮 ]  x  N
int CConfig::Deal1008( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
#if USE_TELLIMIT == 0
	tag2QFbidTelCfg obj2QFbidTelCfg, obj2QFbidTelCfgBkp;
	int iRet = 0;
	int iBufLen = 0;
	DWORD dw = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytResType = 0;
	BYTE bytSetCtrl = 0;
	BYTE bytTelCount = 0;
	BYTE bytTelLen = 0;
	BYTE byt = 0;

	// ���ݲ������
	if( v_dwDataLen < 2 )
	{
		iRet = ERR_PAR;
		goto DEAL1008_FAILED;
	}

	// ��ȡԭʼ����
	iRet = GetSecCfg( &obj2QFbidTelCfg, sizeof(obj2QFbidTelCfg), offsetof(tagSecondCfg, m_uni2QFbidTelCfg.m_obj2QFbidTelCfg), sizeof(obj2QFbidTelCfg) );
	if( iRet )
		goto DEAL1008_FAILED;

	obj2QFbidTelCfgBkp = obj2QFbidTelCfg; // �ȱ���

	// ������֡��ȡ������Ϣ
	bytSetCtrl = BYTE( v_szData[0] );
	bytTelCount = BYTE( v_szData[1] );
	if( 0x7f == bytTelCount ) bytTelCount = 0;

	// ���ݻ�����Ϣ�ټ��
	if( bytTelCount > 16 )
	{
		iRet = ERR_PAR;
		goto DEAL1008_FAILED;
	}

	// �ж��Ƿ�Ҫ��ԭ���ý������
	if( 0x20 & bytSetCtrl ) // ��Ҫ�����������
	{
		memset( obj2QFbidTelCfg.m_aryFbidOutHandtel, 0, sizeof(obj2QFbidTelCfg.m_aryFbidOutHandtel) );
		memset( obj2QFbidTelCfg.m_aryAllowOutHandtel, 0, sizeof(obj2QFbidTelCfg.m_aryAllowOutHandtel) );
		obj2QFbidTelCfg.m_bytFbidOutTelCount = 0, obj2QFbidTelCfg.m_bytAllowOutTelCount = 0;
	}
	if( 0x10 & bytSetCtrl ) // ��Ҫ�����������
	{
		memset( obj2QFbidTelCfg.m_aryFbidInHandtel, 0, sizeof(obj2QFbidTelCfg.m_aryFbidInHandtel) );
		memset( obj2QFbidTelCfg.m_aryAllowInHandtel, 0, sizeof(obj2QFbidTelCfg.m_aryAllowInHandtel) );
		obj2QFbidTelCfg.m_bytFbidInTelCount = 0, obj2QFbidTelCfg.m_bytAllowInTelCount = 0;
	}

	// ��������
	dw = 2;
	for( byt = 0; byt < bytTelCount; byt ++ )
	{
		// �������ȼ��
		if( dw >= v_dwDataLen )
		{
			iRet = ERR_PAR;
			goto DEAL1008_FAILED;
		}

		// ��ȡ���볤��
		bytTelLen = BYTE(v_szData[dw] & 0x0f);

		// �������ȼ��
		if( dw + 1 + bytTelLen > v_dwDataLen || bytTelLen > 15 || bytTelLen < 1 )
		{
			iRet = ERR_PAR;
			goto DEAL1008_FAILED;
		}

		// ��¼�ú���
		switch( v_szData[dw] & 0x30 )
		{
		case 0: // �������
			if( obj2QFbidTelCfg.m_bytAllowOutTelCount < sizeof(obj2QFbidTelCfg.m_aryAllowOutHandtel) / sizeof(obj2QFbidTelCfg.m_aryAllowOutHandtel[0]) )
			{
				memset( obj2QFbidTelCfg.m_aryAllowOutHandtel + obj2QFbidTelCfg.m_bytAllowOutTelCount,
					0x20, sizeof(obj2QFbidTelCfg.m_aryAllowOutHandtel[0]) );
				memcpy( obj2QFbidTelCfg.m_aryAllowOutHandtel + obj2QFbidTelCfg.m_bytAllowOutTelCount,
					v_szData + dw + 1, bytTelLen );
				obj2QFbidTelCfg.m_bytAllowOutTelCount ++;
			}
			break;

		case 0x10: // ��ֹ����
			if( obj2QFbidTelCfg.m_bytFbidOutTelCount < sizeof(obj2QFbidTelCfg.m_aryFbidOutHandtel) / sizeof(obj2QFbidTelCfg.m_aryFbidOutHandtel[0]) )
			{
				memset( obj2QFbidTelCfg.m_aryFbidOutHandtel + obj2QFbidTelCfg.m_bytFbidOutTelCount,
					0x20, sizeof(obj2QFbidTelCfg.m_aryFbidOutHandtel[0]) );
				memcpy( obj2QFbidTelCfg.m_aryFbidOutHandtel + obj2QFbidTelCfg.m_bytFbidOutTelCount,
					v_szData + dw + 1, bytTelLen );
				obj2QFbidTelCfg.m_bytFbidOutTelCount ++;
			}
			break;

		case 0x20: // �������
			if( obj2QFbidTelCfg.m_bytAllowInTelCount < sizeof(obj2QFbidTelCfg.m_aryAllowInHandtel) / sizeof(obj2QFbidTelCfg.m_aryAllowInHandtel[0]) )
			{
				memset( obj2QFbidTelCfg.m_aryAllowInHandtel + obj2QFbidTelCfg.m_bytAllowInTelCount,
					0x20, sizeof(obj2QFbidTelCfg.m_aryAllowInHandtel[0]) );
				memcpy( obj2QFbidTelCfg.m_aryAllowInHandtel + obj2QFbidTelCfg.m_bytAllowInTelCount,
					v_szData + dw + 1, bytTelLen );
				obj2QFbidTelCfg.m_bytAllowInTelCount ++;
			}
			break;

		case 0x30: // ��ֹ����
			if( obj2QFbidTelCfg.m_bytFbidInTelCount < sizeof(obj2QFbidTelCfg.m_aryFbidInHandtel) / sizeof(obj2QFbidTelCfg.m_aryFbidInHandtel[0]) )
			{
				memset( obj2QFbidTelCfg.m_aryFbidInHandtel + obj2QFbidTelCfg.m_bytFbidInTelCount,
					0x20, sizeof(obj2QFbidTelCfg.m_aryFbidInHandtel[0]) );
				memcpy( obj2QFbidTelCfg.m_aryFbidInHandtel + obj2QFbidTelCfg.m_bytFbidInTelCount,
					v_szData + dw + 1, bytTelLen );
				obj2QFbidTelCfg.m_bytFbidInTelCount ++;
			}
			break;

		default:
			;
		}

		dw += 1 + DWORD(bytTelLen); // ׼��������һ����
	}
	obj2QFbidTelCfg.m_bytFbidType = BYTE( (bytSetCtrl & 0x03) + (bytSetCtrl & 0x0c) * 4 ); // ������������

	// ����Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x48, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// д���ļ�
		if( iRet = SetSecCfg(&obj2QFbidTelCfg, offsetof(tagSecondCfg, m_uni2QFbidTelCfg.m_obj2QFbidTelCfg),	sizeof(obj2QFbidTelCfg)) )
		{
			goto DEAL1008_FAILED;
		}

		if( iRet = g_objSock.SOCKSNDSMSDATA(buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL) )
		{
			SetSecCfg( &obj2QFbidTelCfgBkp, offsetof(tagSecondCfg, m_uni2QFbidTelCfg.m_obj2QFbidTelCfg), sizeof(obj2QFbidTelCfgBkp) );
			goto DEAL1008_FAILED;
		}
	}
	else
		goto DEAL1008_FAILED;

	return 0;

DEAL1008_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x48, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );

	return iRet;

#else
	return 0;
#endif
}

/// ��������� ��Ӧ��ɹ����ȣ�
int CConfig::Deal1011( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	tag2QServCodeCfg objServCodeCfg, objServCodeCfgBkp;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	BYTE bytResType = 0;

	// ���ݲ������
	if( 2 != v_dwDataLen )
	{
		iRet = ERR_PAR;
		goto DEAL1011_FAILED;
	}

	// ��ȡ->�޸�
	iRet = GetSecCfg( &objServCodeCfg, sizeof(objServCodeCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objServCodeCfg) );
	if( iRet )
		goto DEAL1011_FAILED;

	objServCodeCfgBkp = objServCodeCfg; // �ȱ���

	memcpy( objServCodeCfg.m_szAreaCode, v_szData, sizeof(objServCodeCfg.m_szAreaCode) );
	
	// ����ʱ7f��ԭΪ0
	if( 0x7f == objServCodeCfg.m_szAreaCode[1] )
		objServCodeCfg.m_szAreaCode[1] = 0; 

	// д�غͷ���Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x51, buf, sizeof(buf), iBufLen); 
	if( !iRet )
	{
		if( iRet = SetSecCfg(&objServCodeCfg, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objServCodeCfg)) )
		{
			goto DEAL1011_FAILED;
		}
		
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL ); // ����
		if( iRet ) // ������ʧ��
		{
			SetSecCfg( &objServCodeCfgBkp, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objServCodeCfgBkp) ); // �ָ�ԭ����
			goto DEAL1011_FAILED;
		}
	}
	else goto DEAL1011_FAILED;

	return 0;

DEAL1011_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x51, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );

	return iRet;
}

// ����GPRS��ʼ���� ��Ӧ��ɹ����ȣ�
int CConfig::Deal103b( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
// ǧ����ͨ�ð�,������_����
// ����������1��+����������1��1��+�������ȣ�1��+������n������N

// ǧ���۹�����_��һ
// �û�ʶ���루6������������N��1�������������ͣ�1������������n��1����������n������N

	tag2QGprsCfg objGprsCfg, objGprsCfgBkp;
	int iRet = 0;
	int iBufLen = 0;
	DWORD dw = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytResType = 0;
	BYTE bytParTotal = 0;
	BYTE bytParLen = 0;
	BYTE byt = 0;
	bool bChgAutoRpt = false;

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	const DWORD BASELEN_103B = 1;
#endif
#if 2 == USE_PROTOCOL
	const DWORD BASELEN_103B = 7;
#endif

	PRTMSG(MSG_DBG, "Deal 103b\n");

	// ��ȡԭʼ����
	iRet = GetSecCfg( &objGprsCfg, sizeof(objGprsCfg), offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );
	if( iRet )
	{
		//RenewMemInfo(0x02, 0x00, 0x01, 0x00);
		goto DEAL103B_FAILED;
	}

	objGprsCfgBkp = objGprsCfg; // �ȱ���

#if 2 == USE_PROTOCOL
	// �Ƚ�ʶ����
	if( memcmp(v_szData, USERIDENCODE, strlen(USERIDENCODE)) )
	{
		PRTMSG(MSG_DBG, "Deal 103b failed 1!\n");
		iRet = ERR_PAR;
		goto DEAL103B_FAILED;
	}
#endif

	// �������
	if( v_dwDataLen < BASELEN_103B )
	{
		PRTMSG(MSG_DBG, "Deal 103b failed 2!\n");
		iRet = ERR_PAR;
		//RenewMemInfo(0x02, 0x00, 0x01, 0x01);
		goto DEAL103B_FAILED;
	}
	bytParTotal = BYTE( v_szData[BASELEN_103B - 1] ); // ��������
	if( 0 == bytParTotal || bytParTotal > 4 )
	{
		PRTMSG(MSG_DBG, "Deal 103b failed 3!\n");
		iRet = ERR_PAR;
		//RenewMemInfo(0x02, 0x00, 0x01, 0x02);
		goto DEAL103B_FAILED;
	}

	// ��֡,��������
	dw = BASELEN_103B;
	for( byt = 0; byt < bytParTotal; byt ++ )
	{
		if( dw + 2 >= v_dwDataLen )
		{
			PRTMSG(MSG_DBG, "Deal 103b failed 4!\n");
			iRet = ERR_PAR;
			//RenewMemInfo(0x02, 0x00, 0x01, 0x03);
			goto DEAL103B_FAILED;
		}

		bytParLen = BYTE( v_szData[ dw + 1 ] );
		if( dw + 2 + bytParLen > v_dwDataLen )
		{
			PRTMSG(MSG_DBG, "Deal 103b failed 5!\n");
			iRet = ERR_PAR;
			//RenewMemInfo(0x02, 0x00, 0x01, 0x04);
			goto DEAL103B_FAILED;
		}

		switch( BYTE(v_szData[dw]) )
		{
		case 0x01: // CFG_GPRS���߷�ʽ
			{
				if( 1 != bytParLen )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 6!\n");
					iRet = ERR_PAR;
					//RenewMemInfo(0x02, 0x00, 0x01, 0x05);
					goto DEAL103B_FAILED;
				}
				BYTE bytOnLineType = BYTE( v_szData[dw + 2] );
				if( 0x7f == bytOnLineType ) bytOnLineType = 0;
				if( bytOnLineType > 3 )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 7!\n");
					iRet = ERR_PAR;
					//RenewMemInfo(0x02, 0x00, 0x01, 0x6);
					goto DEAL103B_FAILED;
				}
				objGprsCfg.m_bytGprsOnlineType = bytOnLineType;
			}
			break;

		case 0x02: // �ϴ�GPS���ݲ���
			{
				// ǧ����ͨ�ð棬������_����
				// �������ݱ�ʶ��1��+ ״̬������1��
				// + [ ״̬��ʶ��1��+���ҵ�����ͣ�1��+ ���ʱ�䣨2��+ ������ڣ�2��+ ������루2�� ]  x  N

				// ǧ����ͨ�ð棬������_��һ
				// �������ݱ�ʶ��1��+ ״̬������1��+ �ϴ�����ʱ��Σ�4��
				// + [ ״̬��ʶ��1��+���ҵ�����ͣ�1��+ ���ʱ�䣨2��+ ������ڣ�2��+ ������루2�� ]  x  N

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
				const DWORD GPRSRPTPAR_BASELEN = 2;
#endif
#if 2 == USE_PROTOCOL
				const DWORD GPRSRPTPAR_BASELEN = 6;
#endif

				// ʲôʱ�����µ��ϴ�GPS����,��ʲôʱ������ɵ�GPS�ϴ����� 
				// ����Ϊ������ֻ����1֡�����,����ֻҪ��������,�Ϳ���������ɵ�,�Ա�������趨��ֵ
				memset( objGprsCfg.m_aryGpsUpPar, 0, sizeof(objGprsCfg.m_aryGpsUpPar) );
				objGprsCfg.m_bytGpsUpParCount = 0;
				objGprsCfg.m_bytGpsUpType = 0;
				
				if( bytParLen <= GPRSRPTPAR_BASELEN )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 8!\n");
					iRet = ERR_PAR;
					//RenewMemInfo(0x02, 0x00, 0x01, 0x07);
					goto DEAL103B_FAILED;
				}
				
				// �������ݱ�־
				objGprsCfg.m_bytGpsUpType = BYTE( v_szData[dw + 2] );
				
				// ��֡�а�����״̬����
				BYTE bytStaCount = BYTE( v_szData[dw + 3] );
				if( 0 == bytStaCount || bytStaCount > 4 )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 9!\n");
					iRet = ERR_PAR;
					//RenewMemInfo(0x02, 0x00, 0x01, 0x08);
					goto DEAL103B_FAILED;
				}
				if( GPRSRPTPAR_BASELEN + 8 * bytStaCount != bytParLen )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 10!\n");
					iRet = ERR_PAR;
					//RenewMemInfo(0x02, 0x00, 0x01, 0x09);
					goto DEAL103B_FAILED;
				}

#if 2 == USE_PROTOCOL
				// ��ֹʱ���
				objGprsCfg.m_bytRptBeginHour = BYTE( v_szData[dw + 4] );
				objGprsCfg.m_bytRptBeginMin = BYTE( v_szData[dw + 5] );
				objGprsCfg.m_bytRptEndHour = BYTE( v_szData[dw + 6] );
				objGprsCfg.m_bytRptEndMin = BYTE( v_szData[dw + 7] );
				if( 0x7f == objGprsCfg.m_bytRptBeginHour )
				{
					objGprsCfg.m_bytRptBeginHour = 0;
				}
				if( 0x7f == objGprsCfg.m_bytRptBeginMin )
				{
					objGprsCfg.m_bytRptBeginMin = 0;
				}
				if( 0x7f == objGprsCfg.m_bytRptEndHour )
				{
					objGprsCfg.m_bytRptEndHour = 0;
				}
				if( 0x7f == objGprsCfg.m_bytRptEndMin )
				{
					objGprsCfg.m_bytRptEndMin = 0;
				}
				if( objGprsCfg.m_bytRptBeginHour > 23 )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 11!\n");
					iRet = ERR_PAR;
					goto DEAL103B_FAILED;
				}
				if( objGprsCfg.m_bytRptBeginMin > 59 )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 12!\n");
					iRet = ERR_PAR;
					goto DEAL103B_FAILED;
				}
				if( objGprsCfg.m_bytRptEndHour > 23 )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 13!\n");
					iRet = ERR_PAR;
					goto DEAL103B_FAILED;
				}
				if( objGprsCfg.m_bytRptEndMin > 59 )
				{
					PRTMSG(MSG_DBG, "Deal 103b failed 14!\n");
					iRet = ERR_PAR;
					goto DEAL103B_FAILED;
				}
				if( objGprsCfg.m_bytRptEndHour < objGprsCfg.m_bytRptBeginHour )
				{
					iRet = ERR_PAR;
					PRTMSG(MSG_DBG, "Deal 103b failed 15!\n");
					goto DEAL103B_FAILED;
				}
				if( objGprsCfg.m_bytRptBeginHour == objGprsCfg.m_bytRptEndHour 
					&&
					objGprsCfg.m_bytRptEndMin < objGprsCfg.m_bytRptBeginMin )
				{
					iRet = ERR_PAR;
					PRTMSG(MSG_DBG, "Deal 103b failed 16!\n");
					goto DEAL103B_FAILED;
				}
#endif

				tag103e obj103e; // ת������ʻ��¼ģ���ģ������֡
				BYTE byt103eSymb = 0; // ģ��֡��־

				// ����״̬������
				DWORD dwPos = dw + 2 + GPRSRPTPAR_BASELEN;
				BYTE bytMaxCount = sizeof(objGprsCfg.m_aryGpsUpPar) / sizeof(objGprsCfg.m_aryGpsUpPar[0]);
				BYTE bytParNo = objGprsCfg.m_bytGpsUpParCount;
				for( byt = 0; byt < bytStaCount && bytParNo < bytMaxCount; byt ++ )
				{
					// ״̬��ʶ
					objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_bytStaType = BYTE(v_szData[dwPos]);

					// ���ҵ������
					objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_bytMonType = BYTE( v_szData[dwPos + 1] );

					// ���ʱ��
					if( 0x7f == v_szData[dwPos + 2] ) v_szData[dwPos + 2] = 0;
					if( 0x7f == v_szData[dwPos + 3] ) v_szData[dwPos + 3] = 0;
					if( BYTE(v_szData[dwPos + 2]) > 126 || BYTE(v_szData[dwPos + 3]) > 59 )
					{
						PRTMSG(MSG_DBG, "Deal 103b failed 17!\n");
						iRet = ERR_PAR;
						//RenewMemInfo(0x02, 0x00, 0x01, 0x0a);
						goto DEAL103B_FAILED;
					}
					objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonTime
						= BYTE(v_szData[dwPos + 2]) * WORD(60) + BYTE(v_szData[dwPos + 3]);
					if( 0 == objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonTime )
						objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonTime = 0xffff; // ���ü��

					BYTE bytMonSimpType = 0x18 & objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_bytMonType; // ��ط�ʽ

#if USE_DRVREC == 1	
					// ��ʱ���
					if( 0 == bytMonSimpType || 0x10 == bytMonSimpType )
					{
						if( 0x7f == v_szData[dwPos + 4] ) v_szData[dwPos + 4] = 0;
						if( 0x7f == v_szData[dwPos + 5] ) v_szData[dwPos + 5] = 0;
						if( BYTE(v_szData[dwPos + 4]) > 126 || BYTE(v_szData[dwPos + 5]) > 59 )
						{
							PRTMSG(MSG_DBG, "Deal 103b failed 18!\n");
							iRet = ERR_PAR;
							//RenewMemInfo(0x02, 0x00, 0x01, 0x0b);
							goto DEAL103B_FAILED;
						}
						objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonInterval
							= BYTE(v_szData[dwPos + 4]) * WORD(60) + BYTE(v_szData[dwPos + 5]);
					}

					// ����ط�ʽΪ��ʱ���ʱ����Ϊ��ʻ��¼�ϴ�
					if( 0 == bytMonSimpType ) // ��ʹ��JG2����ʻ��¼����ʱ����ʱ�������ϱ�����֡��Ϊ��ʻ��¼�ϴ�������
					{
						if( 0x7f == v_szData[dwPos + 6] ) v_szData[dwPos + 6] = 0;
						if( 0x7f == v_szData[dwPos + 7] ) v_szData[dwPos + 7] = 0;
						if( BYTE(v_szData[dwPos + 6]) > 126 || BYTE(v_szData[dwPos + 7]) > 99 )
						{
							PRTMSG(MSG_DBG, "Deal 103b failed 19!\n");
							iRet = ERR_PAR;
							//RenewMemInfo(0x02, 0x00, 0x01, 0x0c);
							goto DEAL103B_FAILED;
						}
						objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonSpace
							= BYTE(v_szData[dwPos + 6]) * WORD(100) + BYTE(v_szData[dwPos + 7]);
					}
#else
					// ����ʱ����ʱ������ʱ���
					if( 0 == bytMonSimpType || 0x10 == bytMonSimpType )
					{
						if( 0x7f == v_szData[dwPos + 4] ) v_szData[dwPos + 4] = 0;
						if( 0x7f == v_szData[dwPos + 5] ) v_szData[dwPos + 5] = 0;
						if( BYTE(v_szData[dwPos + 4]) > 126 || BYTE(v_szData[dwPos + 5]) > 59 )
						{
							PRTMSG(MSG_DBG, "Deal 103b failed 20!\n");
							iRet = ERR_PAR;
							goto DEAL103B_FAILED;
						}
						objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonInterval
							= BYTE(v_szData[dwPos + 4]) * WORD(60) + BYTE(v_szData[dwPos + 5]);
						if( 0 == objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonInterval )
						{
							// ��֤��ؼ������Ϊ1��

							//iRet = ERR_PAR;
							//goto DEAL103B_FAILED;

							objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonInterval = 1;
						}
					}
					
					// ��������������������
					if( 0x08 == bytMonSimpType || 0x10 == bytMonSimpType )
					{
						if( 0x7f == v_szData[dwPos + 6] ) v_szData[dwPos + 6] = 0;
						if( 0x7f == v_szData[dwPos + 7] ) v_szData[dwPos + 7] = 0;
						if( BYTE(v_szData[dwPos + 6]) > 126 || BYTE(v_szData[dwPos + 7]) > 99 )
						{
							PRTMSG(MSG_DBG, "Deal 103b failed 21!\n");
							iRet = ERR_PAR;
							goto DEAL103B_FAILED;
						}
						objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonSpace
							= BYTE(v_szData[dwPos + 6]) * WORD(100) + BYTE(v_szData[dwPos + 7]);
						if( 0 == objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonSpace )
						{
							// ��֤��ض�������Ϊ10��

							//iRet = ERR_PAR;
							//goto DEAL103B_FAILED;

							objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonSpace = 1;
						}
					}
#endif

					if( 0x18 == bytMonSimpType ) // ����ж���Ҫ,��ֹ����ȷ�ļ�ز���,���²�ͣ���쳣��ش���
					{
						PRTMSG(MSG_DBG, "Deal 103b failed 22!\n");
						iRet = ERR_PAR;
						//RenewMemInfo(0x02, 0x00, 0x01, 0x0d);
						goto DEAL103B_FAILED;
					}
					
#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4 // ���������ϱ���������֮��
					if( 0 == bytMonSimpType )	// ���ҽ�����ʱ���·�ʽʱ
					{
						// ����Բ������ò�����Ҳ����ԭ���Ķ���������жϴ���
						if( 0x7f == v_szData[dwPos + 7] ) v_szData[dwPos + 7] = 0;
						if( BYTE(v_szData[dwPos + 7]) > 4 )
						{
							PRTMSG(MSG_DBG, "Deal 103b failed 23!\n");
							iRet = ERR_PAR;
							goto DEAL103B_FAILED;
						}
						
						if( 1 == objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_bytStaType ) // �������
						{
							obj103e.m_bytOnSndHour = BYTE(v_szData[dwPos + 2]);
							obj103e.m_bytOnSndMin = BYTE(v_szData[dwPos + 3]); // �ϱ�ʱ��
							obj103e.m_bytOnSampCycleMin = BYTE(v_szData[dwPos + 4]); 
							obj103e.m_bytOnSampCycleSec = BYTE(v_szData[dwPos + 5]); //ʱ����->��������
							obj103e.m_bytOnSndCycle = BYTE(v_szData[dwPos + 7]); // ������->��������

							byt103eSymb |= 0x01;
						}
						else if( 2 == objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_bytStaType ) // Ϩ������
						{
							obj103e.m_bytOffSndHour = BYTE(v_szData[dwPos + 2]);
							obj103e.m_bytOffSndMin = BYTE(v_szData[dwPos + 3]); // �ϱ�ʱ��
							obj103e.m_bytOffSampCycleMin = BYTE(v_szData[dwPos + 4]); 
							obj103e.m_bytOffSampCycleSec = BYTE(v_szData[dwPos + 5]); //ʱ����->��������
							obj103e.m_bytOffSndCycle = BYTE(v_szData[dwPos + 7]); // ������->��������

							byt103eSymb |= 0x02;
						}						
					}
#endif

#if USE_DRVREC == 1 // ���������ϱ���������֮��
					if( 0 == bytMonSimpType )	// ���ҽ�����ʱ���·�ʽʱ
					{
						unsigned char sz103e[32] = { 0 };
						sz103e[0] = 0x10;
						sz103e[1] = 0x3e;
						sz103e[2] = objGprsCfg.m_bytGpsUpType;	//�������ݱ�ʶ
						sz103e[3] = objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_bytStaType;	//״̬��ʶ
						sz103e[4] = BYTE(objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonTime % 256);
						sz103e[5] = BYTE(objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonTime / 256);
						sz103e[6] = BYTE(objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonInterval % 256);	//ʱ����->��������
						sz103e[7] = BYTE(objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonInterval / 256);
						if( 0 == sz103e[6] && 0 == sz103e[7] ) sz103e[6] = 1;

						//------
						if( 0x7f == v_szData[dwPos + 6] ) v_szData[dwPos + 6] = 0;
						if( 0x7f == v_szData[dwPos + 7] ) v_szData[dwPos + 7] = 0;
						if( BYTE(v_szData[dwPos + 6]) > 126 || BYTE(v_szData[dwPos + 7]) > 99 )
						{
							PRTMSG(MSG_DBG, "Deal 103b failed 24!\n");
							iRet = ERR_PAR;
							//RenewMemInfo(0x02, 0x00, 0x01, 0x0e);
							goto DEAL103B_FAILED;
						}
						objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonSpace
							= BYTE(v_szData[dwPos + 6]) * WORD(100) + BYTE(v_szData[dwPos + 7]);
// 						if( 0 == objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonSpace )
// 						{
// 							PRTMSG(MSG_DBG, "Deal 103b failed 25!\n");
// 							iRet = ERR_PAR;
// 							goto DEAL103B_FAILED; // ��֤��ض�������Ϊ10��
// 						}
						//---------

						sz103e[8] = BYTE(objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonSpace % 256); //������->��������
						sz103e[9] = BYTE(objGprsCfg.m_aryGpsUpPar[ bytParNo ].m_wMonSpace / 256);
						if( 0 == sz103e[8] && 0 == sz103e[9] ) sz103e[8] = 1;
						g_objDriveRecord.tcprcv.push(sz103e, 10);
					}
#endif
					// ����״̬����
					objGprsCfg.m_bytGpsUpParCount ++;
					bytParNo ++;

					// ׼���´�ѭ��
					dwPos += 8;
				}
				
				bChgAutoRpt = true;

#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4 // ���������ϱ���������֮��
				// ǧ����ϵͳ,��103b֡��Ϊ��ʻ��¼�ϱ�����֡ʹ��,����ģ��һ֡103e����ʻ��¼�ϱ�ģ��
				if( (byt103eSymb & 0x03) == 0x03 )
				{
					obj103e.m_bytSndWinSize = 5; // �̶����ڴ�С

					iRet = g_objDrvRecRpt.Deal103e( v_szSrcHandtel, (char*)&obj103e, sizeof(obj103e) );
					if( iRet )
						goto DEAL103B_FAILED;
				}
#endif
			}
			break;

		case 0x03: // ���߷�ʽ��ʾ
			{
				if( 1 != bytParLen )
				{
					iRet = ERR_PAR;
					//RenewMemInfo(0x02, 0x00, 0x01, 0x0f);
					goto DEAL103B_FAILED;
				}
				BYTE bytTipType = BYTE( v_szData[dw + 2] );
				if( 0 == bytTipType || bytTipType > 2 )
				{
					iRet = ERR_PAR;
					//RenewMemInfo(0x02, 0x00, 0x01, 0x10);
					goto DEAL103B_FAILED;
				}
				objGprsCfg.m_bytOnlineTip = bytTipType;
			}
			break;

		case 0x04: // ����ͨ��
			{
				// 080917,��������080899�޸�,ֻ���������ݡ�λ�ò�ѯӦ��Ͷ���ͨ���·����õ�Ӧ���߶���ͨ��
				// ���ű��ݹ������ڴ�����̶������������ζ��������ö��ű��ݵĴ���
				
				objGprsCfg.m_bytChannelBkType_1 = objGprsCfg.m_bytChannelBkType_2 = 0; // ǿ�ƹرն��ű��ݣ��������
			}
			break;

		default:
			break;
		}

		dw += 2 + bytParLen; // ׼���´�ѭ��
	}

	// ��֡
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x7b, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// ����������
		if( iRet = SetSecCfg(&objGprsCfg, offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg)) )
		{
			//RenewMemInfo(0x02, 0x00, 0x01, 0x11);
			goto DEAL103B_FAILED;
		}

		// ����Ӧ��
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
		if( !iRet )
		{
#if USE_AUTORPT == 1
			if( bChgAutoRpt )
			{				
				// ����Ӧ���µ������ϱ�����
				g_objAutoRpt.Init();
			}
#endif
		}
		else
		{
			SetSecCfg( &objGprsCfgBkp, offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfgBkp) ); // �ָ�ԭ����
			//RenewMemInfo(0x02, 0x00, 0x01, 0x12);
			goto DEAL103B_FAILED;
		}
	}
	else 
	{
		//RenewMemInfo(0x02, 0x00, 0x01, 0x13);
		goto DEAL103B_FAILED;
	}

	return 0;

DEAL103B_FAILED:
	bytResType = 0x02;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x7b, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );

	return iRet;
}

/// ����IP��ַ
// ����(1)+ ����(1)+ [ ���(1)+APN(20)+ TCP_IP(15)+ TCP_PORT(5)+ VUDP_IP(15)+VUDP_PORT(5)+ BUDP_IP(15)+BUDP_PORT(5) ] �� N
int CConfig::Deal103c( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	int iRet = 0;

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL	
	tag1QIpCfg objIpCfg[3];
	tag1QIpCfg objIpCfgBkp[3];
	tag107c res;
	int iBufLen = 0;
	DWORD dw = 0;
	char* pBlank = NULL;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytParTotal = 0;
	BYTE bytPro = 0;
	BYTE bytIPNo = 0;
	BYTE byt = 0;
	bool bOneIpSetSucc = false;

	// �������
	if( v_dwDataLen <= 2 )
	{
		return ERR_PAR;
	}
	bytPro = v_szData[0];
	bytParTotal = v_szData[1];
	if( 0 == bytParTotal || bytParTotal > 3 )
	{
		return ERR_PAR;
	}

	// �ж��Ƿ�Ҫ���ԭ����
	if( 0x7f == bytPro ) bytPro = 0;
	if( 0 == bytPro )
	{
// 		memset( buf, 0, sizeof(buf) );
// 		::SaveOneQianIpCfg( 0, buf, 80 );
// 		::SaveOneQianIpCfg( 1, buf, 80 );
// 		::SaveOneQianIpCfg( 2, buf, 80 );
	}

	// ��֡,��������
	dw = 2;
	for( byt = 0; byt < bytParTotal; byt ++, dw += 81 )
	{
		// �������
		if( dw + 81 > v_dwDataLen )
		{
			iRet = ERR_PAR;
			continue;
		}

		// ���
		bytIPNo = v_szData[ dw ];
		if( 0x7f == bytIPNo ) bytIPNo = 0;
		if( bytIPNo > 2 )
		{
			iRet = ERR_PAR;
			continue;
		}
		if( bytIPNo > 1 ) continue; // ��ʱֻ����2��IP����

		// Ԥ��֡ (������ǰ)
		res.m_bytIPNo = bytIPNo;
		if( 0 == res.m_bytIPNo ) res.m_bytIPNo = 0x7f;

		// ��ȡԭʼ����
		iRet = GetImpCfg( (void*)objIpCfg, sizeof(tag1QIpCfg)*3, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg),sizeof(tag1QIpCfg)*3 );
		if( iRet ) 
			goto _ONEIP_FAILED;

		//objIpCfgBkp = objIpCfg; // �ȱ���
		memcpy((void*)objIpCfgBkp, (void*)objIpCfg, sizeof(tag1QIpCfg)*3);

		// APN
		memcpy( objIpCfg[bytIPNo].m_szApn, v_szData + dw + 1, sizeof(objIpCfg[bytIPNo].m_szApn) );
		objIpCfg[bytIPNo].m_szApn[ sizeof(objIpCfg[bytIPNo].m_szApn) - 1 ] = 0;
		pBlank = strchr( objIpCfg[bytIPNo].m_szApn, 0x20 );
		if( pBlank ) *pBlank = 0;
		
		// TCP IP
		memset( buf, 0, sizeof(buf) );
		strncpy( buf, v_szData + dw + 21, 15 );
		objIpCfg[bytIPNo].m_ulQianTcpIP = inet_addr( buf );

		// TCP Port
		memset( buf, 0, sizeof(buf) );
		strncpy( buf, v_szData + dw + 36, 5 );
		objIpCfg[bytIPNo].m_usQianTcpPort = htons( (short)(atoi(buf)) );

		// VUDP IP
		memset( buf, 0, sizeof(buf) );
		strncpy( buf, v_szData + dw + 41, 15 );
		objIpCfg[bytIPNo].m_ulVUdpIP = inet_addr( buf );

		// VUDP Port
		memset( buf, 0, sizeof(buf) );
		strncpy( buf, v_szData + dw + 56, 5 );
		objIpCfg[bytIPNo].m_usVUdpPort = htons( (short)(atoi(buf)) );

		// BUDP IP
		memset( buf, 0, sizeof(buf) );
		strncpy( buf, v_szData + dw + 61, 15 );
		objIpCfg[bytIPNo].m_ulQianUdpIP = inet_addr( buf );

		// BUDP Port
		memset( buf, 0, sizeof(buf) );
		strncpy( buf, v_szData + dw + 76, 5 );
		objIpCfg[bytIPNo].m_usQianUdpPort = htons( (short)(atoi(buf)) );

		// ���ɳɹ�Ӧ��
		res.m_bytResType = 0x01;
		iRet = g_objSms.MakeSmsFrame( (char*)&res, sizeof(res), 0x10, 0x7c, buf, sizeof(buf), iBufLen);
		if( iRet ) goto _ONEIP_FAILED;
	
		// ����������
		iRet = SetImpCfg( (void*)objIpCfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(tag1QIpCfg)*3 );
		if( iRet ) goto _ONEIP_FAILED;

		// ����Ӧ��
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0 , v_bFromSM ? v_szSrcHandtel : NULL);
		if( iRet )
		{
			// ������ʧ��,�ָ�ԭ����
			SetImpCfg( (void*)objIpCfgBkp, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(tag1QIpCfg)*3 );
			goto _ONEIP_FAILED;
		}

		bOneIpSetSucc = true;
		continue;

_ONEIP_FAILED:
		res.m_bytResType = 0x02;
		iRet = g_objSms.MakeSmsFrame((char*)&res, sizeof(res), 0x10, 0x7c, buf, sizeof(buf), iBufLen);
		if( !iRet ) iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
	}

	if( bOneIpSetSucc ) //������ip�Ͷ˿�
	{
		char buf = 0x01;	// QianExe����SockServExe���������׽���		
		DataPush((void*)&buf, 1, DEV_QIAN, DEV_SOCK, LV3);
	}
#endif

#if 2 == USE_PROTOCOL
// �û�ʶ���루6������������N��1�������������ͣ�1������������n��1����������n������N
// ��������	����				����	����
// 0x01		APN����						�ƶ��ṩ��APN�������磺CMnet���򳤶���5
// 0x02		Tcp��IP�Ͷ˿�		20		TCP_IP��15���� TCP_PORT��5��ע��ʹ���ַ�������ʽ�·�
// 0x03		Tcp��һ����IP�Ͷ˿�	20		TCP_IP��15���� TCP_PORT��5��ע��ʹ���ַ�������ʽ�·�
// 0x04		Tcp�ڶ�����IP�Ͷ˿�	20		TCP_IP��15���� TCP_PORT��5��ע��ʹ���ַ�������ʽ�·�
// 0x05		UDP ʹ�õ�IP�Ͷ˿�	20		TCP_IP��15���� TCP_PORT��5��ע��ʹ���ַ�������ʽ�·�
// 0x06		��ƵUDPʹ�õ�IP�Ͷ˿�	20	TCP_IP��15���� TCP_PORT��5��ע��ʹ���ַ�������ʽ�·�

	char buf[200] = { 0 };
	char szPkt[100] = { 0 };
	int iBufLen = 0;
	BYTE bytParCount = 0;
	BYTE byt = 0;
	BYTE bytParType = 0;
	BYTE bytParLen = 0;
	DWORD dw = 0;
	tag1PComuCfg obj1PComuCfg;
	tag1QIpCfg obj1QIpCfg;
	obj1PComuCfg.Init(CDMA_NAME,CDMA_PASS);

	if( v_dwDataLen < 7 )
	{
		return ERR_PAR;	
	}
	
	// Ԥ����Ӧ���������ʧ��Ӧ��
	szPkt[0] = v_szData[6];
	memset( szPkt + 1, 2, sizeof(szPkt) - 1 );

	bytParCount = BYTE( v_szData[6] );
	if( bytParCount > 5 || bytParCount < 1 )
	{
		bytParCount = 1;
		goto DEAL103C_END;
	}

	// �Ƚ�ʶ����
	if( memcmp(v_szData, USERIDENCODE, strlen(USERIDENCODE)) )
	{
		iRet = ERR_PAR;
		goto DEAL103C_END;
	}

// 	// �޸��û�ʶ���
// 	iRet = ::GetImpCfg( &obj1PComuCfg, sizeof(obj1PComuCfg),
// 		offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
// 	if( iRet ) goto DEAL103C_END;
// 	memcpy( obj1PComuCfg.m_szUserIdentCode, v_szData, sizeof(obj1PComuCfg.m_szUserIdentCode) );
// 	iRet = ::SetImpCfg( &obj1PComuCfg, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
// 	if( iRet )
// 	{
// 		goto DEAL103C_END;
// 	}
	
	// �޸�IP�Ȳ���
	dw = 7;
	for( byt = 0; byt < bytParCount; byt ++ )
	{
		if( v_dwDataLen < dw + 2 )
			goto DEAL103C_END;

		bytParType = v_szData[ dw ];
		bytParLen = v_szData[ dw + 1 ];

		if( v_dwDataLen < dw + 2 + bytParLen )
			goto DEAL103C_END;

		switch( bytParType )
		{
		case 1: // APN
			if( bytParLen >= sizeof(obj1QIpCfg.m_szApn) )
				goto DEAL103C_END;

			iRet = GetImpCfg( &obj1QIpCfg, sizeof(obj1QIpCfg), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]), sizeof(obj1QIpCfg) );
			if( iRet ) goto DEAL103C_END;

			memset( obj1QIpCfg.m_szApn, 0, sizeof(obj1QIpCfg.m_szApn) ); // ȷ��NULL��β
			memcpy( obj1QIpCfg.m_szApn, v_szData + dw + 2, bytParLen );

			iRet = SetImpCfg( &obj1QIpCfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]), sizeof(obj1QIpCfg) );
			if( iRet ) goto DEAL103C_END;

			break;

		case 2: // Tcp IP �˿�
			if( 20 != bytParLen )
				goto DEAL103C_END;

			iRet = GetImpCfg( &obj1QIpCfg, sizeof(obj1QIpCfg), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]), sizeof(obj1QIpCfg) );
			if( iRet ) goto DEAL103C_END;

			memset( buf, 0, sizeof(buf) );
			memcpy( buf, v_szData + dw + 2, 15 );
			obj1QIpCfg.m_ulQianTcpIP = inet_addr( buf );
			
			memset( buf, 0, sizeof(buf) );
			memcpy( buf, v_szData + dw + 17, 5 );
			obj1QIpCfg.m_usQianTcpPort = htons( (unsigned short)(atoi(buf)) );

			iRet = SetImpCfg( &obj1QIpCfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]), sizeof(obj1QIpCfg) );
			if( iRet ) goto DEAL103C_END;

			break;

		case 3: // Tcp����1 IP �˿� (�ݲ�����)
			if( 20 != bytParLen )
				goto DEAL103C_END;
			break;

		case 4: // Tcp����2 IP �˿� (�ݲ�����)
			if( 20 != bytParLen )
				goto DEAL103C_END;
			break;

		case 5: // Udp IP �˿�
			if( 20 != bytParLen )
				goto DEAL103C_END;

			iRet = GetImpCfg( &obj1QIpCfg, sizeof(obj1QIpCfg), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]), sizeof(obj1QIpCfg) );
			if( iRet ) goto DEAL103C_END;

			memset( buf, 0, sizeof(buf) );
			memcpy( buf, v_szData + dw + 2, 15 );
			obj1QIpCfg.m_ulQianUdpIP = inet_addr( buf );
			
			memset( buf, 0, sizeof(buf) );
			memcpy( buf, v_szData + dw + 17, 5 );
			obj1QIpCfg.m_usQianUdpPort = htons( (unsigned short)(atoi(buf)) );

			iRet = SetImpCfg( &obj1QIpCfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]), sizeof(obj1QIpCfg) );
			if( iRet ) goto DEAL103C_END;
			
			break;

		case 6: // ��ƵUdp IP �˿�
			if( 20 != bytParLen )
				goto DEAL103C_END;
			
			iRet = GetImpCfg( &obj1QIpCfg, sizeof(obj1QIpCfg), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]), sizeof(obj1QIpCfg) );
			if( iRet ) goto DEAL103C_END;
			
			memset( buf, 0, sizeof(buf) );
			memcpy( buf, v_szData + dw + 2, 15 );
			obj1QIpCfg.m_ulVUdpIP = inet_addr( buf );
			
			memset( buf, 0, sizeof(buf) );
			memcpy( buf, v_szData + dw + 17, 5 );
			obj1QIpCfg.m_usVUdpPort = htons( (unsigned short)(atoi(buf)) );
			
			iRet = SetImpCfg( &obj1QIpCfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]), sizeof(obj1QIpCfg) );
			if( iRet ) goto DEAL103C_END;
			
			break;

		default:
			;
		}

		szPkt[ 1 + byt ] = 1; // �ò����ɹ������־

		dw += 2 + bytParLen;
	}

DEAL103C_END:

	// �����в��������óɹ����������µ�IP�Ͷ˿�
	int i=1;
	for(i=1; i<szPkt[0]; i++)
	{
		if( 0x02 == szPkt[i] )
			break;
	}
	if( i == szPkt[0] )
	{
		char buf = 0x01;	// QianExe����SockServExe���������׽���		
		DataPush((void*)&buf, 1, DEV_QIAN, DEV_SOCK, LV3);
	}

	memset( buf, 0, sizeof(buf) );
	iRet = g_objSms.MakeSmsFrame( szPkt, int(bytParCount) + 1, 0x10, 0x7c, buf, sizeof(buf), iBufLen );
	if( !iRet )
	{
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
	}
	
#endif

	return iRet;
}

#if 2 == USE_PROTOCOL
int CConfig::Deal103e( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	int iRet = 0;

	iRet = ResumeCfg( true, false );
	
	char buf[100] = { 0 };
	int iBufLen = 0;
	BYTE bytResult = iRet ? 0x7f : 0;

	iRet = g_objSms.MakeSmsFrame( (char*)&bytResult, sizeof(bytResult), 0x10, 0x7e, buf, sizeof(buf), iBufLen );
	if( !iRet )
	{
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
	}

	return iRet;
}

int CConfig::Deal103f( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, char* v_szDesHandtel, bool v_bFromSM )
{
// �������ĺ���(22)+�û�ʶ����(6)

	tag103f req;
	tag1PComuCfg obj1PComuCfg, obj1PComuCfgBkp;
	tag2QServCodeCfg obj2QServCodeCfg;
	char* pBlank = NULL;
	int iRet = 0;
	char buf[ 200 ] = { 0 };
	int iBufLen = 0;
	BYTE bytResType = 0;

	// ���ݲ������
	if( sizeof(req) + QIANDOWNFRAME_BASELEN - 1 != v_dwDataLen )
	{
		iRet = ERR_PAR;
		goto DEAL103F_FAILED;
	}

	// ��ȡ���ò���
	memcpy( &req, v_szData + QIANDOWNFRAME_BASELEN - 1, sizeof(req) );

	// �Ƚ�ʶ����
	if( memcmp(req.m_szUserIdentCode, USERIDENCODE, strlen(USERIDENCODE)) )
	{
		iRet = ERR_PAR;
		goto DEAL103F_FAILED;
	}

	// �ȶ�ȡ,���޸�

	iRet = GetImpCfg( &obj1PComuCfg, sizeof(obj1PComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	if( iRet ) goto DEAL103F_FAILED;
	obj1PComuCfgBkp = obj1PComuCfg; // �ȱ���

	iRet = GetSecCfg( &obj2QServCodeCfg, sizeof(obj2QServCodeCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QServCodeCfg) );
	if( iRet ) goto DEAL103F_FAILED;
	// ������һ������

	// �޸Ķ������ĺ�
	memcpy( obj1PComuCfg.m_szSmsCentID, req.m_szSmsCentID, sizeof(obj1PComuCfg.m_szSmsCentID) );
	StrRemvBlank( obj1PComuCfg.m_szSmsCentID, sizeof(obj1PComuCfg.m_szSmsCentID) );

// 	// �����޸��û�ʶ���
// 	memcpy( obj1PComuCfg.m_szUserIdentCode, req.m_szUserIdentCode, sizeof(obj1PComuCfg.m_szUserIdentCode) );

	// �����޸ı����ֻ���
	memcpy( obj1PComuCfg.m_szTel, v_szDesHandtel, sizeof(obj1PComuCfg.m_szTel) );
	obj1PComuCfg.m_szTel[ sizeof(obj1PComuCfg.m_szTel) - 1 ] = 0;
	pBlank = strchr( obj1PComuCfg.m_szTel, 0x20 );
	if( pBlank ) *pBlank = 0;

	// �����޸�����ID (�������)
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 2
	obj2QServCodeCfg.m_szAreaCode[0] = v_szData[0];
	obj2QServCodeCfg.m_szAreaCode[1] = v_szData[1];
#endif
#if USE_PROTOCOL == 1
	obj2QServCodeCfg.m_szAreaCode[0] = v_szData[2];
	obj2QServCodeCfg.m_szAreaCode[1] = v_szData[3];
#endif

	if( 0x7f == obj2QServCodeCfg.m_szAreaCode[1] ) obj2QServCodeCfg.m_szAreaCode[1] = 0; // ����ʱ7f��ԭΪ0
	
	// д�غͷ���Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x7f, buf, sizeof(buf), iBufLen ); 
	if( !iRet )
	{
		// ���������� (���������ֻ��š��û�ʶ���)
		if( iRet = SetImpCfg(&obj1PComuCfg, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg)) )
		{
			goto DEAL103F_FAILED;
		}

		// ����
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );
		if( !iRet )
		{
			SetSecCfg( &obj2QServCodeCfg, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(obj2QServCodeCfg) ); // ����д�������

			// ֪ͨ�ײ�������Ч�µĶ������ĺ�
			char buf[2] = {0x01, 0x36};
			DataPush(buf, 2, DEV_QIAN, DEV_DIAODU, LV2);
		}
		else // ����(������ʧ��)
		{
			SetImpCfg( &obj1PComuCfgBkp, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfgBkp) ); // �ָ�ԭ���� ((�����ָ��ֻ���))
			goto DEAL103F_FAILED;
		}
	}
	else goto DEAL103F_FAILED;

	return 0;

DEAL103F_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x7f, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_szSrcHandtel );

	return iRet;
}
#endif

/// ���õ绰���뱾:
int CConfig::Deal3301( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen )
{
#if 2 == USE_TELLIMIT
// ����ͨ�ð�Э��
// ֡����(1)+������(1) + ���ø�����1�������30����
// +�������ţ�1��+ �绰���루15��+ ������10��+ Ȩ�����ã�1��+ ͨ��ʱ����2����*N
	tag2QCallCfg objCallCfg, objCallCfgBkp;
	tag3341 res;
	int iRet = 0;
	int iBufLen = 0;
	DWORD dw = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytSetCtrl = 0;
	BYTE bytSetCount = 0;
	BYTE byt = 0;

	res.m_bytFrameNo = 0x7d; // ��ʼһ��������ֵ,�ܿ�0��7f,7e

	// ���ݲ������
	if( v_dwDataLen < 1 )
	{
		iRet = ERR_PAR;
		goto DEAL3301_FAILED;
	}
	res.m_bytFrameNo = v_szData[0]; // ��������Ӧ���֡����

	// ��ȡԭʼ����
	iRet = GetSecCfg( &objCallCfg, sizeof(objCallCfg), offsetof(tagSecondCfg, m_uni2QCallCfg.m_obj2QCallCfg), sizeof(objCallCfg) );
	if( iRet ) goto DEAL3301_FAILED;
	objCallCfgBkp = objCallCfg; // �ȱ���

	// �������
	if( v_dwDataLen < 2 )
	{
		iRet = ERR_PAR;
		goto DEAL3301_FAILED;
	}

	// ������֡��ȡ������Ϣ
	bytSetCtrl = BYTE( v_szData[1] ); // ������

	// ���ȼ��
	if( 0x08 == (bytSetCtrl & 0x0c) || 0x02 == (bytSetCtrl & 0x03) ) // �����������е绰������
	{
		if( v_dwDataLen < 3 )
		{
			iRet = ERR_PAR;
			goto DEAL3301_FAILED;
		}
	}

	// ���ø���
	bytSetCount = BYTE( v_szData[2] );
	if( 0x7f == bytSetCount ) bytSetCount = 0;
	if( bytSetCount > sizeof(objCallCfg.m_aryTelBook) / sizeof(objCallCfg.m_aryTelBook[0]) )
	{
		iRet = ERR_PAR;
		goto DEAL3301_FAILED;
	}

	// ���ȼ��
	if( v_dwDataLen < 3 + 29 * DWORD(bytSetCount) )
	{
		iRet = ERR_PAR;
		goto DEAL3301_FAILED;
	}

	// �ж��Ƿ�Ҫ��ԭ���ý������
	if( 0x20 & bytSetCtrl )
	{
		memset( objCallCfg.m_aryTelBook, 0, sizeof(objCallCfg.m_aryTelBook) );
	}

	// ��������
	objCallCfg.m_bytSetType = bytSetCtrl & 0x0f;
	dw = 3;
	for( byt = 0; byt < bytSetCount; byt ++ )
	{
		BYTE bytTelIndex = BYTE( v_szData[dw] ); // �绰������
		if( 0 == bytTelIndex || bytTelIndex > sizeof(objCallCfg.m_aryTelBook) / sizeof(objCallCfg.m_aryTelBook[0]) )
		{
			iRet = ERR_PAR;
			goto DEAL3301_FAILED;
		}

		memcpy( objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_szTel, v_szData + dw + 1,
			sizeof(objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_szTel) ); // �绰����
		memcpy( objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_szName, v_szData + dw + 16,
			min(sizeof(objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_szName), 10) ); // ����
		objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_bytType = BYTE( v_szData[dw + 26] ); // Ȩ��

		// ͨ����ʱ
		if( 0x7f == v_szData[dw + 27] ) v_szData[dw + 27] = 0;
		if( 0x7f == v_szData[dw + 28] ) v_szData[dw + 28] = 0;
		if( BYTE(v_szData[dw + 27]) >= 0x7f || BYTE(v_szData[dw + 28]) > 59 )
		{
			iRet = ERR_PAR;
			goto DEAL3301_FAILED;
		}
		objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_wLmtPeriod = BYTE( v_szData[dw + 27] ) * 60 + BYTE( v_szData[dw + 28] );

		dw += 29;
	}

	// ����Ӧ��
	res.m_bytAnsType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&res, sizeof(res), 0x33, 0x41, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// д���ļ�
		if( iRet = SetSecCfg( &objCallCfg, offsetof(tagSecondCfg, m_uni2QCallCfg.m_obj2QCallCfg), sizeof(objCallCfg)) )
		{
			goto DEAL3301_FAILED;
		}

		if( iRet = g_objSock.SOCKSNDSMSDATA(buf, iBufLen, LV12, 0) )
		{
			SetSecCfg( &objCallCfgBkp, offsetof(tagSecondCfg, m_uni2QCallCfg.m_obj2QCallCfg), sizeof(objCallCfgBkp) );
			goto DEAL3301_FAILED;
		}
	}
	else
		goto DEAL3301_FAILED;

	return 0;

DEAL3301_FAILED:
	res.m_bytAnsType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&res, sizeof(res), 0x33, 0x41, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );

	return iRet;

#endif
	
#if 3 == USE_TELLIMIT
// ������_��һ
// ֡����(1)+���ø�����1�������30����+�������ţ�1��+ �绰���루15��+ ������20��+ Ȩ�����ã�1��+ ͨ��ʱ����2����*N
	tag2QCallCfg objCallCfg, objCallCfgBkp;
	tag3341 res;
	int iRet = 0;
	int iBufLen = 0;
	DWORD dw = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytSetCtrl = 0;
	BYTE bytSetCount = 0;
	BYTE byt = 0;

	res.m_bytFrameNo = 0x7d; // ��ʼһ��������ֵ,�ܿ�0��7f,7e

	// ���ݲ������
	if( v_dwDataLen < 1 )
	{
		iRet = ERR_PAR;
		goto DEAL3301_FAILED;
	}
	res.m_bytFrameNo = v_szData[0]; // ��������Ӧ���֡����

	// ��ȡԭʼ����
	iRet = GetSecCfg( &objCallCfg, sizeof(objCallCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objCallCfg) );
	if( iRet ) goto DEAL3301_FAILED;
	objCallCfgBkp = objCallCfg; // �ȱ���

	// �������
	if( v_dwDataLen < 2 )
	{
		iRet = ERR_PAR;
		goto DEAL3301_FAILED;
	}

	// ���ø���
	bytSetCount = BYTE( v_szData[1] );
	if( 0x7f == bytSetCount ) bytSetCount = 0;
	if( bytSetCount > sizeof(objCallCfg.m_aryTelBook) / sizeof(objCallCfg.m_aryTelBook[0]) )
	{
		iRet = ERR_PAR;
		goto DEAL3301_FAILED;
	}

	// ���ȼ��
	if( v_dwDataLen < 2 + 39 * DWORD(bytSetCount) )
	{
		iRet = ERR_PAR;
		goto DEAL3301_FAILED;
	}

	// �ж��Ƿ�Ҫ��ԭ���ý������
	if( 0x20 & bytSetCtrl )
	{
		memset( objCallCfg.m_aryTelBook, 0, sizeof(objCallCfg.m_aryTelBook) );
	}

	// ��������
	objCallCfg.m_bytSetType = bytSetCtrl & 0x0f;
	dw = 2;
	for( byt = 0; byt < bytSetCount; byt ++ )
	{
		BYTE bytTelIndex = BYTE( v_szData[dw] ); // �绰������
		if( 0 == bytTelIndex || bytTelIndex > sizeof(objCallCfg.m_aryTelBook) / sizeof(objCallCfg.m_aryTelBook[0]) )
		{
			iRet = ERR_PAR;
			goto DEAL3301_FAILED;
		}

		memcpy( objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_szTel, v_szData + dw + 1,
			sizeof(objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_szTel) ); // �绰����
		memcpy( objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_szName, v_szData + dw + 16,
			min(sizeof(objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_szName), 20) ); // ����
		objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_bytType = BYTE( v_szData[dw + 36] ); // Ȩ��

		// ͨ����ʱ
		if( 0x7f == v_szData[dw + 37] ) v_szData[dw + 37] = 0;
		if( 0x7f == v_szData[dw + 38] ) v_szData[dw + 38] = 0;
		if( BYTE(v_szData[dw + 37]) >= 0x7f || BYTE(v_szData[dw + 38]) > 59 )
		{
			iRet = ERR_PAR;
			goto DEAL3301_FAILED;
		}
		objCallCfg.m_aryTelBook[ bytTelIndex - 1 ].m_wLmtPeriod
			= BYTE( v_szData[dw + 37] ) * 60 + BYTE( v_szData[dw + 38] );

		dw += 39;
	}

	// ����Ӧ��
	res.m_bytAnsType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&res, sizeof(res), 0x33, 0x41, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// д���ļ�
		if( iRet = SetSecCfg( &objCallCfg, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objCallCfg)) )
		{
			goto DEAL3301_FAILED;
		}

		if( iRet = g_objSock.SOCKSNDSMSDATA(buf, iBufLen, LV12, 0) )
		{
			SetSecCfg( &objCallCfgBkp, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objCallCfgBkp) );
			goto DEAL3301_FAILED;
		}
	}
	else
		goto DEAL3301_FAILED;

	return 0;

DEAL3301_FAILED:
	res.m_bytAnsType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&res, sizeof(res), 0x33, 0x41, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );

	return iRet;
#endif

	return 0;
}

/// �������ķ���绰����: ������1�������绰���루15������N
int CConfig::Deal3302( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen )
{
#if 2 == USE_TELLIMIT || 3 == USE_TELLIMIT
	tag2QServCodeCfg objCentServTelCfg, objCentServTelCfgBkp;
	int iRet = 0;
	int iBufLen = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytResType = 0;
	BYTE bytTelCount = 0;
	BYTE bytCanCount = 0;

	// ���ݲ������
	if( v_dwDataLen < 1 )
	{
		iRet = ERR_PAR;
		goto DEAL3302_FAILED;
	}

	// ��ȡԭʼ����
	iRet = GetSecCfg( &objCentServTelCfg, sizeof(objCentServTelCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objCentServTelCfg) );
	if( iRet ) goto DEAL3302_FAILED;

	objCentServTelCfgBkp = objCentServTelCfg; // �ȱ���

	// �������
	bytTelCount = BYTE( v_szData[0] );
	if( 0x7f == bytTelCount ) bytTelCount = 0;
	if( bytTelCount > sizeof(objCentServTelCfg.m_aryCentServTel) / sizeof(objCentServTelCfg.m_aryCentServTel[0]) )
	{
		iRet = ERR_PAR;
		goto DEAL3302_FAILED;
	}

	// ���ȼ��
	if( v_dwDataLen < 1 + 15 * DWORD(bytTelCount) )
	{
		iRet = ERR_PAR;
		goto DEAL3302_FAILED;
	}

	// �ж��Ƿ�Ҫ���ԭ����
	if( 0 == bytTelCount ) memset( &objCentServTelCfg, 0, sizeof(objCentServTelCfg) );

	// ��������
	if( objCentServTelCfg.m_bytCentServTelCount > sizeof(objCentServTelCfg.m_aryCentServTel) / sizeof(objCentServTelCfg.m_aryCentServTel[0]) )
		objCentServTelCfg.m_bytCentServTelCount = sizeof(objCentServTelCfg.m_aryCentServTel) / sizeof(objCentServTelCfg.m_aryCentServTel[0]); // ���մ���
	bytCanCount = sizeof(objCentServTelCfg.m_aryCentServTel) / sizeof(objCentServTelCfg.m_aryCentServTel[0]) - objCentServTelCfg.m_bytCentServTelCount;
	if( bytCanCount > bytTelCount ) bytCanCount = bytTelCount; // ʵ�ʿ��ٴ���ĺ������
	memcpy( objCentServTelCfg.m_aryCentServTel + objCentServTelCfg.m_bytCentServTelCount,
		v_szData + 1, bytCanCount * sizeof(objCentServTelCfg.m_aryCentServTel[0]) );
	objCentServTelCfg.m_bytCentServTelCount += bytCanCount;

	// ����Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x33, 0x42, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// д���ļ�
		if( iRet = SetSecCfg( &objCentServTelCfg, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objCentServTelCfg)) )
		{
			goto DEAL3302_FAILED;
		}

		if( iRet = g_objSock.SOCKSNDSMSDATA(buf, iBufLen, LV12, 0) )
		{
			SetSecCfg( &objCentServTelCfgBkp, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objCentServTelCfgBkp) );
			goto DEAL3302_FAILED;
		}
	}
	else
		goto DEAL3302_FAILED;

	return 0;

DEAL3302_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x33, 0x42, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );

	return iRet;

#else
	return 0;
#endif
}

/// ����UDP�������: �������ԣ�1��+ �绰����N��1�������10���� + �����볤�ȣ�1�� + �绰���봮��1~15������ N
int CConfig::Deal3304( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen )
{
#if QIANTYPE_USE == QIANTYPE_KJ2
	tag2QServCodeCfg objUdpAtvTelCfg, objUdpAtvTelCfgBkp;
	int iRet = 0;
	int iBufLen = 0;
	DWORD dw = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytResType = 0;
	BYTE bytSetType;
	BYTE bytTelCount = 0;
	BYTE bytCanCount = 0;
	BYTE bytTelLen = 0;
	BYTE byt = 0;

	// ���ݲ������
	if( v_dwDataLen < 2 )
	{
		iRet = ERR_PAR;
		goto DEAL3304_FAILED;
	}

	// ��ȡԭʼ����
	iRet = GetSecCfg( &objUdpAtvTelCfg, sizeof(objUdpAtvTelCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objUdpAtvTelCfg) );
	if( iRet ) goto DEAL3304_FAILED;
	objUdpAtvTelCfgBkp = objUdpAtvTelCfg; // �ȱ���

	// ��������
	bytSetType = BYTE( v_szData[0] );

	// �������
	bytTelCount = BYTE( v_szData[1] );
	if( 0x7f == bytTelCount ) bytTelCount = 0;
	if( bytTelCount > sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel) / sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel[0]) )
	{
		iRet = ERR_PAR;
		goto DEAL3304_FAILED;
	}

	// �ж��Ƿ�Ҫ���ԭ����
	if( 1 == bytSetType )
	memset( &objUdpAtvTelCfg, 0, sizeof(objUdpAtvTelCfg) );

	// ʵ�ʿ��ٴ���ĺ������
	if( objUdpAtvTelCfg.m_bytUdpAtvTelCount > sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel) / sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel[0]) )
		objUdpAtvTelCfg.m_bytUdpAtvTelCount = sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel) / sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel[0]); // ���մ���
	bytCanCount = sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel) / sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel[0]) - objUdpAtvTelCfg.m_bytUdpAtvTelCount;
	if( bytCanCount > bytTelCount ) bytCanCount = bytTelCount;

	// ��������
	dw = 2;
	for( byt = 0; byt < bytCanCount; byt ++ )
	{
		// �������ȼ��
		if( v_dwDataLen < dw + 2 )
		{
			iRet = ERR_PAR;
			goto DEAL3304_FAILED;
		}

		// ���볤��
		bytTelLen = BYTE( v_szData[ dw ] & 0x0f );
		if( bytTelLen < 1 || bytTelLen > sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel[0]) / sizeof(objUdpAtvTelCfg.m_aryUdpAtvTel[0][0]) )
		{
			iRet = ERR_PAR;
			goto DEAL3304_FAILED;
		}

		// �������ȼ��
		if( v_dwDataLen < dw + 1 + bytTelLen )
		{
			iRet = ERR_PAR;
			goto DEAL3304_FAILED;
		}

		// �������
		memcpy( objUdpAtvTelCfg.m_aryUdpAtvTel + objUdpAtvTelCfg.m_bytUdpAtvTelCount, v_szData + dw + 1, bytTelLen );
		objUdpAtvTelCfg.m_bytUdpAtvTelCount ++;

		dw += 1 + bytTelLen;
	}

	// ����Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x33, 0x44, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// д���ļ�
		if( iRet = SetSecCfg( &objUdpAtvTelCfg, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objUdpAtvTelCfg)) )
		{
			goto DEAL3304_FAILED;
		}

		if( iRet = g_objSock.SOCKSNDSMSDATA(buf, iBufLen, LV12, 0) )
		{
			SetSecCfg( &objUdpAtvTelCfgBkp, offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objUdpAtvTelCfgBkp) );
			goto DEAL3304_FAILED;
		}
	}
	else
		goto DEAL3304_FAILED;

	return 0;

DEAL3304_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x33, 0x44, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );

	return iRet;

#else
	return 0;
#endif
}

// ��������ϵ��ָ��
int CConfig::DealComu6d( char* v_szData, DWORD v_dwDataLen )
{
// ����
// ָ�����ͣ�1��
// 01H ��ѯ��
// 02H ���� + �趨ֵ��2���� �趨ֵ:ÿ����������
// 03H ������⣻
// 04H ֹͣ��⣻
// 05H �û�������� �� ���ֵ��1�� ���ֵ��λ������

// Ӧ��
// ָ��ţ�1��
// 01H ��ѯӦ�� �� Ӧ�����ͣ�1�� �� ��ѯ�����2����
// 02H ����Ӧ�� �� Ӧ�����ͣ�1����
// 03H �������Ӧ�� �� Ӧ�����ͣ�1����
// 04H �������Ӧ�� �� Ӧ�����ͣ�1����
// 05H �����Ӧ�� �� Ӧ�����ͣ�1����
// Ӧ�����ͣ�01H-�ɹ�
//           02H-ʧ��

// 	char buf[64];
// 	buf[0] = 0x6e;
// 	buf[1] = v_szData[0];
// 
// 	if( v_dwDataLen < 1 ) return ERR_PAR;
// 
// 	switch( BYTE(v_szData[0]) )
// 	{
// 	case 1: // ��ѯ
// 		{
// 			WORD wCyclePerKm = 0;
// 			GetImpCfg( &wCyclePerKm, sizeof(wCyclePerKm), offsetof(tagImportantCfg, m_uni1QBiaodingCfg.m_obj1QBiaodingCfg.m_wCyclePerKm), sizeof(wCyclePerKm) );
// 			buf[2] = 1;
// 			buf[3] = wCyclePerKm / 0x100;
// 			buf[4] = wCyclePerKm % 0x100;
// 			DataPush(buf, 5, DEV_QIAN, DEV_DIAODU, LV2);
// 		}		
// 		break;
// 
// 	case 2: // ����
// 		{
// 			if( v_dwDataLen < 3 ) return ERR_PAR;
// 
// 			WORD wCyclePerKm = 0;
// 			wCyclePerKm = BYTE(v_szData[1]) * 0x100 + BYTE(v_szData[2]);
// 
// 			int iResult = SetImpCfg( &wCyclePerKm, offsetof(tagImportantCfg, m_uni1QBiaodingCfg.m_obj1QBiaodingCfg.m_wCyclePerKm), sizeof(wCyclePerKm) );
// 			if( !iResult ) buf[2] = 1;
// 			else buf[2] = 2;
// 
// 			DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);
// 		}
// 		break;
// 
// 	case 3: // �������
// 		{
// 			m_bytBiaodinSta = 1;
// 			
// 			g_objQianIO.IN_QueryMeterCycle( m_dwBiaodinBeginCycle );
// 			buf[2] = 1;
// 			DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);
// 		}
// 		break;
// 
// 	case 4: // �������
// 		{
// 			if( 1 != m_bytBiaodinSta )
// 				buf[2] = 2;
// 			else
// 			{
// 				m_bytBiaodinSta = 2;
// 				g_objQianIO.IN_QueryMeterCycle( m_dwBiaodinEndCycle );
// 				buf[2] = 1;
// 			}
// 
// 			DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);
// 		}
// 		break;
// 
// 	case 5: // �궨
// 		{
// 			if( v_dwDataLen < 2 ) return ERR_PAR;
// 
// 			if( 2 != m_bytBiaodinSta )
// 			{
// 				buf[2] = 2;
// 			}
// 			else if( 0 == v_szData[1] ) // ��ֹ��0�쳣
// 			{
// 				buf[2] = 2;
// 			}
// 			else
// 			{
// 				WORD wCyclePerKm = WORD( double(m_dwBiaodinEndCycle - m_dwBiaodinBeginCycle) / BYTE(v_szData[1]) );
// 				int iResult = SetImpCfg( &wCyclePerKm, offsetof(tagImportantCfg, m_uni1QBiaodingCfg.m_obj1QBiaodingCfg.m_wCyclePerKm), sizeof(wCyclePerKm) );
// 				if( !iResult ) buf[2] = 1;
// 				else buf[2] = 2;
// 			}
// 
// 			DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);
// 		}
// 		break;
// 
// 	default:
// 		;
// 	}

	return 0;
}

void CConfig::GetVer( char* v_szTel, size_t v_sizTel )
{
	
}
