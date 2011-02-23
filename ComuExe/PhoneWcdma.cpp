// PhoneWcdma.cpp: implementation of the CPhoneWcdma class.
//
//////////////////////////////////////////////////////////////////////

#include "ComuStdAfx.h"


#undef MSG_HEAD
#define MSG_HEAD ("ComuExe-PhoneWcdma:")


#if USE_LANGUAGE == 0
#define LANG_NETSEARCH_FAIL ("������Ѱʧ�ܣ�����SIM��")
#endif

#if USE_LANGUAGE == 11
#define LANG_NETSEARCH_FAIL ("Network search Failure,Please Check SIM Card")
#endif


#define MIN_GPRS_SIGNAL		5				// �ܲ�GPRS����С�ź�ֵ
#define MIN_RESET_TIME		12*3600*1000	// ��С��λʱ��
#define SMS_POP_COUNT		10


static char	MapTable[64]={   
	0x2e,0x2c,0x30,0x31,0x32,0x33,0x34,0x35,	0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,					
	0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,	0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,						
	0x55,0x56,0x57,0x58,0x59,0x5A,0x61,0x62,	0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,	
	0x6B,0x6C,0x6D,0x6E,0x6F,0x70,0x71,0x72,	0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A 
};

static uchar CrcTable[256] = {
	0x00,0x91,0xE3,0x72,0x07,0x96,0xE4,0x75,	0x0E,0x9F,0xED,0x7C,0x09,0x98,0xEA,0x7B,
	0x1C,0x8D,0xFF,0x6E,0x1B,0x8A,0xF8,0x69,	0x12,0x83,0xF1,0x60,0x15,0x84,0xF6,0x67,
	0x38,0xA9,0xDB,0x4A,0x3F,0xAE,0xDC,0x4D,	0x36,0xA7,0xD5,0x44,0x31,0xA0,0xD2,0x43,
	0x24,0xB5,0xC7,0x56,0x23,0xB2,0xC0,0x51,	0x2A,0xBB,0xC9,0x58,0x2D,0xBC,0xCE,0x5F,

	0x70,0xE1,0x93,0x02,0x77,0xE6,0x94,0x05,	0x7E,0xEF,0x9D,0x0C,0x79,0xE8,0x9A,0x0B,
	0x6C,0xFD,0x8F,0x1E,0x6B,0xFA,0x88,0x19,	0x62,0xF3,0x81,0x10,0x65,0xF4,0x86,0x17,
	0x48,0xD9,0xAB,0x3A,0x4F,0xDE,0xAC,0x3D,	0x46,0xD7,0xA5,0x34,0x41,0xD0,0xA2,0x33,
	0x54,0xC5,0xB7,0x26,0x53,0xC2,0xB0,0x21,	0x5A,0xCB,0xB9,0x28,0x5D,0xCC,0xBE,0x2F,

	0xE0,0x71,0x03,0x92,0xE7,0x76,0x04,0x95,	0xEE,0x7F,0x0D,0x9C,0xE9,0x78,0x0A,0x9B,
	0xFC,0x6D,0x1F,0x8E,0xFB,0x6A,0x18,0x89,	0xF2,0x63,0x11,0x80,0xF5,0x64,0x16,0x87,
	0xD8,0x49,0x3B,0xAA,0xDF,0x4E,0x3C,0xAD,	0xD6,0x47,0x35,0xA4,0xD1,0x40,0x32,0xA3,
	0xC4,0x55,0x27,0xB6,0xC3,0x52,0x20,0xB1,	0xCA,0x5B,0x29,0xB8,0xCD,0x5C,0x2E,0xBF,

	0x90,0x01,0x73,0xE2,0x97,0x06,0x74,0xE5,	0x9E,0x0F,0x7D,0xEC,0x99,0x08,0x7A,0xEB,
	0x8C,0x1D,0x6F,0xFE,0x8B,0x1A,0x68,0xF9,	0x82,0x13,0x61,0xF0,0x85,0x14,0x66,0xF7,
	0xA8,0x39,0x4B,0xDA,0xAF,0x3E,0x4C,0xDD,	0xA6,0x37,0x45,0xD4,0xA1,0x30,0x42,0xD3,
	0xB4,0x25,0x57,0xC6,0xB3,0x22,0x50,0xC1,	0xBA,0x2B,0x59,0xC8,0xBD,0x2C,0x5E,0xCF
};

// void G_QueryWcdmaBase()
// {
// 	g_objPhone.P_QueryWcdmaBase();
// }

// void *G_SaveWcdmaData(void *v_pPar)
// {
// 	PRTMSG(MSG_DBG,"Save Wcdma Data ")
// 	g_objPhone.P_SaveWcdmaData();
// }

void G_WCDMA_PppSigHandler(int v_signo)
{
	G_RecordDebugInfo("Dial Net succ!");

	PRTMSG(MSG_NOR,"�յ�ppp���ųɹ��ź�֪ͨ\n");
	g_objPhoneWcdma._PppSigHandler();

	// ����ָʾ��
#if VEHICLE_TYPE == VEHICLE_M
	g_objLightCtrl.SetLightSta(0x03);	// ����
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	g_objLightCtrl2.SetLightSta(0x01);	// ���ųɹ�֪ͨ
#endif
}

unsigned long G_WCDMA_PhoneHdl( void* v_pPar )
{
	if( v_pPar )
		return ((CPhoneWcdma*)v_pPar)->P_PhoneHdl();
	else
		return ERR_THREAD;
}

unsigned long G_WCDMA_OtherHdl( void* v_pPar )
{
	if( v_pPar )
		return ((CPhoneWcdma*)v_pPar)->P_OtherHdl();
	else
		return ERR_THREAD;
}

unsigned long G_WCDMA_ComRcv( void* v_pPar )
{
	if( v_pPar )
		return ((CPhoneWcdma*)v_pPar)->P_ComRcv();
	else
		return ERR_THREAD;
}
//�ݲ�ʵ��ping����
// unsigned long G_WCDMA_Ping(void* v_pPar)
// {
// 	if( v_pPar )
// 		return ((CPhoneWcdma*)v_pPar)->P_Ping();
// 	else
// 		return ERR_THREAD;
// }

bool G_WCDMA_JugNormalAsc( char v_cToJug )
{
	return (v_cToJug >= '0' && v_cToJug <= '9')
		|| (v_cToJug >= 'a' && v_cToJug <= 'z')
		|| (v_cToJug >= 'A' && v_cToJug <= 'Z');
}

int G_WCDMA_CMGS_JugRes(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
	if( strstr(v_szAtRes, "ERROR") )
		return WCDMA_ATANS_ERROR;
	else if( strstr(v_szAtRes, ">") )
		return WCDMA_ATANS_OK;
	else
		return WCDMA_ATANS_NOANSWER;
}

int G_WCDMA_GSN_Jugres(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// AT+CGSN
//
// +GSN:a3daaaeb
//
// OK
	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}
	const char STR_RESSYMB[] = "+GSN:";
	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}

	else if( strstr(v_szAtRes, "OK") )
	{
		char* pAns = strstr(v_szAtRes, STR_RESSYMB);
		if( pAns )
		{
			// Ѱ����ʼ�жϵ�λ��
			pAns += strlen( STR_RESSYMB ); // ����Ӧ���־
			while( 0x20 == *pAns && 0 != *pAns ) // ����֮��Ŀո�
			{
				pAns ++;
			}

			// ��ȡIMEI
			if( *pAns )
			{
				DWORD dwLen = 0;
				while( dwLen + 1 < v_sizResult )
				{
					if( !G_WCDMA_JugNormalAsc(pAns[dwLen]) ) // ֱ������ͨASCII�ַ�
					{
						break;
					}

					((char*)v_pResult)[dwLen] = pAns[dwLen];
					dwLen ++;
				}
				if( dwLen < v_sizResult )
				{
					((char*)v_pResult)[dwLen] = 0;
				}

				return WCDMA_ATANS_OK;
			}
			else
			{
				return WCDMA_ATANS_OTHER;
			}
		}
		else
		{
			return WCDMA_ATANS_OTHER;
		}
	}
	else
	{
		return WCDMA_ATANS_RCVNOTALL;
	}
}

int G_WCDMA_CSQ_Jugres(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// AT+CSQ
// 
// +CSQ: 11,99
// 
// OK

	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}

	const char STR_RESSYMB[] = "+CSQ:";
	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}
	else if( strstr(v_szAtRes, "OK") )
	{
		char* pAns = strstr(v_szAtRes, STR_RESSYMB);
		if( pAns )
		{
			// Ѱ����ʼ�жϵ�λ��
			pAns += strlen( STR_RESSYMB ); // ����Ӧ���־
			while( 0x20 == *pAns && 0 != *pAns ) // ����֮��Ŀո�
			{
				pAns ++;
			}

			// ��ȡ�ź�
			if( *pAns )
			{
				char* pEnd = strstr(pAns, ",");
				if( pEnd )
				{
					size_t sizSingalLen = pEnd - pAns;
					if( sizSingalLen + 1 > v_sizResult )
					{
						return WCDMA_ATANS_OTHER;
					}
					else
					{
						strncpy((char*)v_pResult, pAns, sizSingalLen );
						((char*)v_pResult)[ sizSingalLen ] = 0;
						return WCDMA_ATANS_OK;
					}
				}
				else
				{
					return WCDMA_ATANS_OTHER;
				}
			}
			else
			{
				return WCDMA_ATANS_OTHER;
			}
		}
		else
		{
			return WCDMA_ATANS_OTHER;
		}
	}
	else
	{
		return WCDMA_ATANS_RCVNOTALL;
	}
}

// que �������ĺţ�703at��
int G_WCDMA_CSCA_Query_res(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// AT+CSCA?
// 
// +CSCA: "+8613800592500",145
// 
// OK

	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}

	const char STR_RESSYMB[] = "+CSCA:";
	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}
	else if( strstr(v_szAtRes, "OK") )
	{
		char* pAns = strstr(v_szAtRes, STR_RESSYMB);
		if( pAns )
		{
			pAns += strlen(STR_RESSYMB); // ����Ӧ���־

			// Ѱ����ʼλ��
			char* pBegin = strstr(pAns, "\"");
			if( !pBegin ) return WCDMA_ATANS_OTHER;
			if( pBegin - pAns > 3 ) return WCDMA_ATANS_OTHER;

			// ����λ��
			char* pEnd = strstr(pBegin + 1, "\"");
			if( !pEnd ) return WCDMA_ATANS_OTHER;
			if( pEnd - pBegin > 15 ) return WCDMA_ATANS_OTHER;

			DWORD dwCscaLen = min(DWORD(pEnd - pBegin - 1), v_sizResult - 1);
			memcpy( v_pResult, pBegin + 1, dwCscaLen );
			((char*)v_pResult)[ dwCscaLen ] = 0;
			
			return WCDMA_ATANS_OK;
		}
		else
		{
			return WCDMA_ATANS_OTHER;
		}
	}
	else
	{
		return WCDMA_ATANS_RCVNOTALL;
	}
}

//���������ʾָʾ CLIP 
int G_WCDMA_CLIP_ReadTel(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// RING    
// 
// +CLIP: "05922956773",161,"",,"",0  
//+CLIP:15980856182,0,,,,0 

	int iRet = WCDMA_ATANS_NOANS;
	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}
	
	const char STR_RESSYMB[] = "+CLIP:";
	char* pClipBegin = NULL;

	if( strstr(v_szAtRes, "RING") )
	{
		if( pClipBegin = strstr(v_szAtRes, STR_RESSYMB) )
		{
			pClipBegin += strlen(STR_RESSYMB); // ����Ӧ���־

			// Ѱ����ʼλ��

			char* pBegin = pClipBegin;
			while( 0x20 == *pBegin ) // ����֮��Ŀո�
			{
				pBegin ++;
			}
			if( !pBegin ) return WCDMA_ATANS_OTHER;
			if( pBegin - pClipBegin > 3 ) return WCDMA_ATANS_OTHER;

			// Ѱ����ֹλ��,���ű�־
			char* pEnd = strchr( pBegin + 1, ',' );
			if( !pEnd ) return WCDMA_ATANS_OTHER;
			if( pEnd - pBegin > 20 ) return WCDMA_ATANS_OTHER;
			DWORD dwTelLen = min( DWORD(pEnd - pBegin ), v_sizResult - 1 )-2;
			memcpy( v_pResult, pBegin+1, dwTelLen );
			((char*)v_pResult)[ dwTelLen ] = 0;

			iRet = WCDMA_ATANS_OK;
		}
		else
		{
			iRet = WCDMA_ATANS_RCVNOTALL;	//��������ʾ
		}

		// �ж��Ƿ���DSCIָʾ (��)
	}
	else
	{
		iRet = WCDMA_ATANS_NOANS;
	}
	return iRet;
}

//����״̬��ѯ����AT+CLCC���˴�������ȡ�������,ͨ�����ű�־��ȡ
int G_WCDMA_CLCC_ReadTel(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// at+clcc       
// +CLCC:1,1,4,0,0,05922956774,255,255,255
// 
// OK

	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}
	
	const char STR_RESSYMB[] = "+CLCC:";
	char* pClccBegin = NULL;

	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}
	else if( strstr(v_szAtRes, "OK") )
	{
		if( pClccBegin = strstr(v_szAtRes, STR_RESSYMB) )
		{
			pClccBegin += strlen(STR_RESSYMB); // ����Ӧ���־
			char* pBegin;
			char* pEnd;
			char* pResEnd;
			int i;
			pBegin = pClccBegin;
			for( i=0; i<5; i++)
			{
				// Ѱ����ʼλ��
				pBegin = strchr(pBegin, ',');
				if( !pBegin ) 
				{
					return WCDMA_ATANS_OTHER;
				}
				pBegin++;
			}
			
			while( 0x20 == *pBegin && 0 != *pBegin ) // ����֮��Ŀո�
			{
				pBegin ++;
			}

			// Ѱ����ֹλ��
			pEnd = strchr( pBegin, ',' );
			if( !pEnd ) 
			{
				return WCDMA_ATANS_OTHER;
			}
			pResEnd = strstr(pClccBegin, "\r");
			if( pEnd - pBegin > 20 || pResEnd < pEnd ) 
			{
				return WCDMA_ATANS_OTHER;
			}

			DWORD dwTelLen = min( DWORD(pEnd - pBegin), v_sizResult - 1 ) - 2;
			memcpy( v_pResult, pBegin+1, dwTelLen );
			((char*)v_pResult)[ dwTelLen ] = 0;
			return WCDMA_ATANS_OK;
		}
		else
		{
			return WCDMA_ATANS_NOANS;
		}
	}
	else
	{
		return WCDMA_ATANS_RCVNOTALL;
	}
}

//��ȡ��������
int G_WCDMA_CMGD_SmsList(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
	// AT+CMGD=?      
	// +CMGD:(0,1,2,3,4,5,6,8)  //�����б�        
	// +CMGD:()  //�޶��� 
	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}
	
	const char STR_RESSYMB[] = "+CMGD:";
	char* pCmgdBegin = NULL;
	
	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}

	if( pCmgdBegin = strstr(v_szAtRes, STR_RESSYMB) )
	{
		pCmgdBegin += strlen(STR_RESSYMB); // ����Ӧ���־
		char* pIndex;
		char* pEnd;
		char* pResEnd;
		int i = 0;
		pIndex = pCmgdBegin;

		// Ѱ����ֹλ��
		pEnd = strchr( pCmgdBegin, ')' );
		if( !pEnd ) 
		{
			return WCDMA_ATANS_OTHER;
		}

		pResEnd = strstr(pCmgdBegin, "\r");
		if( pResEnd < pEnd )
		{
			return WCDMA_ATANS_OTHER;
		}

		pIndex = strchr(pCmgdBegin, '(' );
		if ( 1 != (pEnd - pIndex) )
		{
			((char*)v_pResult)[i++] = *(pIndex+1);
		}
		
		do
		{
			pIndex = strchr(pIndex, ',' );
			if( !pIndex || (pIndex > pEnd) ) 
			{
				return WCDMA_ATANS_OK;
			}
			pIndex++;
			
			while( 0x20 == *pIndex && 0 != *pIndex ) // ����֮��Ŀո�
			{
				pIndex ++;
			}
			((char*)v_pResult)[i++] = *pIndex;
		}while(pIndex < pEnd);

		((char*)v_pResult)[i] = 0;
		return WCDMA_ATANS_OK;
	}
	else
	{
		return WCDMA_ATANS_NOANS;
	}

}

//����Ƿ�������״̬�������жϹһ�
int G_WCDMA_CLCCJugIncoming(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
	if( !v_szAtRes )	//	�ж��贫��Ĳ���
	{
		return WCDMA_ATANS_OTHER;
	}
	
	const char STR_RESSYMB[] = "+CLCC:";
	char* pClccBegin = NULL;
	
	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}
	else if( strstr(v_szAtRes, "OK") )
	{
		if( pClccBegin = strstr(v_szAtRes, STR_RESSYMB) )
		{
			pClccBegin += strlen(STR_RESSYMB); // ����Ӧ���־
			char* pTarget = pClccBegin;
			// Ѱ�Ҷ��ű�־

			for(int i=0; i<2; i++ )
			{
				pTarget = strchr(pTarget, ',');
				// Ѱ�Ҷ��ű�־
				if( !pTarget ) 
				{
					return WCDMA_ATANS_OTHER;
				}					
				pTarget++;
			}
			while( 0x20 == *pTarget && 0 != *pTarget ) // ����֮��Ŀո�
			{
				pTarget ++;
			}			
			char* pResEnd = strstr(pClccBegin, "\r");
			if( pResEnd < pTarget ) return WCDMA_ATANS_OTHER;

			if( !pTarget ) return WCDMA_ATANS_OTHER;
			
			if('4' != *pTarget)
			{
				return WCDMA_ATANS_OTHER;
			}
			else
				return WCDMA_ATANS_OK;
		}
		else
		{
			return WCDMA_ATANS_NOANS;
		}
	}
	else
	{
		return WCDMA_ATANS_RCVNOTALL;
	}
}

int G_WCDMA_CLCCGetId(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}
	
	const char STR_RESSYMB[] = "+CLCC:";
	char* pClccBegin = NULL;
	
	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}
	else if( strstr(v_szAtRes, "OK") )
	{
		if( pClccBegin = strstr(v_szAtRes, STR_RESSYMB) )
		{
			pClccBegin += strlen(STR_RESSYMB); // ����Ӧ���־
			char* pTarget = pClccBegin;
			
			while( 0x20 == *pTarget ) // ����֮��Ŀո�
			{
				pTarget ++;
			}
			if( G_WCDMA_JugNormalAsc( *pTarget ) )
			{
				((char*)v_pResult)[0] = *pTarget;
			}

			char* pResEnd = strstr(pClccBegin, "\r");
			if( pResEnd < pTarget ) return WCDMA_ATANS_OTHER;
			
			if( !pTarget ) return WCDMA_ATANS_OTHER;
			return WCDMA_ATANS_OK;
		}
		else
		{
			return WCDMA_ATANS_NOANS;
		}
	}
	else
	{
		return WCDMA_ATANS_RCVNOTALL;
}
}


//��ͨ��ģ�����޷��ж϶Է���û�н����ģ�������Ŀǰ����
//��״̬���������й�GSMģ������Է��������᷵��OK����CDMA������
//û�����Ӧ����ʾ

//���н�ͨ��ͨ��״̬�ϱ�
int G_WCDMA_JugRemotePick(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// ^CONN:<call_x>,<call_type>  

	if( strstr(v_szAtRes, "ERROR") )
		return WCDMA_ATANS_ERROR;
	else if( strstr(v_szAtRes, "NO CARRIER") )
		return WCDMA_ATANS_NOCARRIER;
	else if( strstr(v_szAtRes, "NO DIALTONE") )
		return WCDMA_ATANS_NODIALTONE;
	else if( strstr(v_szAtRes, "BUSY") )
		return WCDMA_ATANS_BUSY;
	else if( strstr(v_szAtRes, "NO ANSWER") )
		return WCDMA_ATANS_NOANSWER;
	else if( strstr(v_szAtRes, "CONN") )
		return WCDMA_ATANS_OK;
	else
		return WCDMA_ATANS_NOANS;
}

//�жϹҶ�,ģ�������ϱ�^CEND:��ʾ�Է��Ҷ�
//^CEND:1,82,29
int G_WCDMA_JugRemoteHangup(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
	if( strstr(v_szAtRes, "CEND:") )
		return WCDMA_ATANS_OK;
	else if( strstr(v_szAtRes, "NO CARRIER") )
		return WCDMA_ATANS_NOCARRIER;
	else if( strstr(v_szAtRes, "NO ERROR") )
		return WCDMA_ATANS_ERROR;
// 	else if( strstr(v_szAtRes, "NO DIALTONE") )
// 		return WCDMA_ATANS_NODIALTONE;
// 	else if( strstr(v_szAtRes, "BUSY") )
// 		return WCDMA_ATANS_BUSY;
// 	else if( strstr(v_szAtRes, "NO ANSWER") )
// 		return WCDMA_ATANS_NOANSWER;
// 	else if( strstr(v_szAtRes, "COLP") )
// 		return WCDMA_ATANS_OK;
// 	else
// 		return WCDMA_ATANS_NOANS;
	else
		return WCDMA_ATANS_OTHER;
}

//����״̬��ѯ����,�˴�������ѯ�Ƿ���ͨ��״̬
//����״̬�᷵��OK
//+CLCC:2,0,0,0,0,15980856182,255,255,255 
int G_WCDMA_CLCC_JugRes(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// +CLCC: 1,0,2,0,0,"1305526111",129                                 
// OK

//	PRTMSG(MSG_DBG, "%s\n", v_szAtRes);

	const char STR_RESSYMB[] = "+CLCC:";

	if( strstr(v_szAtRes, "OK") && strstr(v_szAtRes, STR_RESSYMB) )
	{
		return WCDMA_ATANS_OK;
	}
	else
	{
		return WCDMA_ATANS_NOCALL;
	}
}

//���ŷ����жϣ��յ� > ���ɷ��Ͷ�������
int G_WCDMA_HCMGS_JugRes(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
	if( strstr(v_szAtRes, "ERROR") )
		return WCDMA_ATANS_ERROR;
	else if( strstr(v_szAtRes, ">") )
		return WCDMA_ATANS_OK;
	else
		return WCDMA_ATANS_NOANSWER;
}

//�������ݷ��ͳɹ�/ʧ���ж�
int G_WCDMA_HCMGS_JugSend(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
//OK
//^HCMGSS:31 //���ŷ��ͳɹ�
	const char STR_RESSYMB[] = "^HCMGSS:";
	if( strstr(v_szAtRes, "ERROR") )
		return WCDMA_ATANS_ERROR;
	else if( strstr(v_szAtRes, "OK") || strstr(v_szAtRes, STR_RESSYMB) )
		return WCDMA_ATANS_OK;
	else
		return WCDMA_ATANS_NOANSWER;
}

//�������ݶ�ȡ�ɹ�/ʧ���ж�
int G_WCDMA_HCMGR_JugSend(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
	//OK
	//^HCMGSS:31 //���ŷ��ͳɹ�
	const char STR_RESSYMB[] = "^HCMGR:";
	if( strstr(v_szAtRes, "ERROR") )
		return WCDMA_ATANS_ERROR;
	else if( strstr(v_szAtRes, "OK") || strstr(v_szAtRes, STR_RESSYMB) )
		return WCDMA_ATANS_OK;
	else
		return WCDMA_ATANS_NOANSWER;
}

//Wcdma,R-UIM ����λ��PIN1 ��״̬��ѯ
int G_WCDMA_CPIN_Jugres(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{	
//AT+CPIN?
//+CPIN: READY
	if(strstr(v_szAtRes, "+CPIN"))
	{
		if( strstr(v_szAtRes, "READY") || strstr(v_szAtRes, "OK") )
		{
			return WCDMA_ATANS_OK;
		}
	}
	else
	{
		return WCDMA_ATANS_ERROR;
	}
}

//PPP�����û������ѯ
int G_WCDMA_CPPPCFG_Jugres(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{	
//	AT^PPPCFG?
//	^PPPCFG:"ctnet@mycdma.cn","vnet.mobi"
//	^PPPCFG:"card","card"
	char *pTarget = NULL;
	if(strstr(v_szAtRes, "^PPPCFG:"))
	{
		pTarget = strstr(v_szAtRes, ",");//���ű�־
		if (NULL == pTarget)
		{
			return WCDMA_ATANS_ERROR;
		}
		if( strstr(v_szAtRes, "card") && strstr(pTarget, "card") ||
			strstr(v_szAtRes, "CARD") && strstr(pTarget, "CARD") )
		{
			return WCDMA_ATANS_OK;
		}
	}
	else
	{
		return WCDMA_ATANS_ERROR;
	}
}

int G_WCDMA_BaseCPSI(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// AT+CPSI?
// +CPSI:WCDMA,Online,001-01,0xED2E,WCDMA IMT 2000,0,9,10688,0,6,62,43,45 
// OK

	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}
	
	const char STR_RESSYMB[] = "+CPSI";
	char* pBegin = NULL;
	
	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}
	else if( strstr(v_szAtRes, "OK") )
	{
		pBegin = strstr(v_szAtRes, STR_RESSYMB);
		if(pBegin) 
		{
			char* pEnd;
			char* pResEnd;
			int i;
			
			// Ѱ����ֹλ��
			pResEnd = strstr( pBegin, "OK" );
			if( !pResEnd ) 
			{
				return WCDMA_ATANS_OTHER;
			}
			
			DWORD dwTelLen = min( DWORD(pResEnd - pBegin), v_sizResult - 1 );
			memcpy( v_pResult, pBegin, dwTelLen );
			((char*)v_pResult)[ dwTelLen ] = 0;
			return WCDMA_ATANS_OK;
		}
		else
		{
			return WCDMA_ATANS_NOANS;
		}
	}
	else
	{
		return WCDMA_ATANS_ERROR;
	}
}

int G_WCDMA_BaseCRUS(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
//AT+CRUS
//+CRUS:Active SET,1,2,10663,0,0,16,16,101,1536
//+CRUS:Sync Neighbor SET,2,42,10663,0,0,34,33,109,1536,35,10663,0,0,26,26,106,1536
//+CRUS:Async Neighbor SET,10,11,10663,0,0,0,49,121,0,6,10663,0,0,0,49,121,0,28,10663,0,0,0,49,121,0,247,10663,0,0,0,49,121,0,193,10663,0,0,0,49,121,0,493,10663,0,0,0,49,121,0,485,10663,0,0,0,49,121,0,258,10663,0,0,0,49,121,0,109,10663,0,0,0,49,121,0,226,10663,0,0,38,49,121,1536
//OK

	if( !v_szAtRes || !v_pResult || !v_sizResult )
	{
		return WCDMA_ATANS_OTHER;
	}
	
	const char STR_RESSYMB[] = "+CRUS";
	char* pBegin = NULL;
	
	if( strstr(v_szAtRes, "ERROR") )
	{
		return WCDMA_ATANS_ERROR;
	}
	else if( strstr(v_szAtRes, "OK") )
	{
		pBegin = strstr(v_szAtRes, STR_RESSYMB);
		if(pBegin) 
		{
			char* pEnd;
			char* pResEnd;
			int i;
			
			// Ѱ����ֹλ��
			pResEnd = strstr( pBegin, "OK" );
			if( !pResEnd ) 
			{
				return WCDMA_ATANS_OTHER;
			}
			
			DWORD dwTelLen = min( DWORD(pResEnd - pBegin), v_sizResult - 1 );
			memcpy( v_pResult, pBegin, dwTelLen );
			((char*)v_pResult)[ dwTelLen ] = 0;
			return WCDMA_ATANS_OK;
		}
		else
		{
			return WCDMA_ATANS_NOANS;
		}
	}
	else
	{
		return WCDMA_ATANS_ERROR;
	}
}

int G_WCDMA_CNSMOD_Jugres(char* v_szAtRes, void* v_pResult, size_t v_sizResult )
{
// +CNSMOD: 0,2
// OK
	
	//PRTMSG(MSG_DBG, "%s\n", v_szAtRes);

	static int i = 0;
	char *p = NULL;
	const char STR_RESSYMB[] = "+CNSMOD:";
	char szNetType[20] = {0};

	p = strstr(v_szAtRes, STR_RESSYMB);
	if( NULL != p )
	{
		p = strstr(v_szAtRes, ",");
		p ++;

		switch(*p)
		{
		case '0':
			sprintf(szNetType, "%s", "No Service");
			break;

		case '1':
			sprintf(szNetType, "%s", "GSM");
			break;

		case '2':
			sprintf(szNetType, "%s", "GPRS");
			break;

		case '3':
			sprintf(szNetType, "%s", "EDGE");
			break;

		case '4':
			sprintf(szNetType, "%s", "WCDMA");
			break;

		case '5':
			sprintf(szNetType, "%s", "HSDPA");
			break;

		case '6':
			sprintf(szNetType, "%s", "HSUPA");
			break;

		case '7':
			sprintf(szNetType, "%s", "HSDPA+HSUPA");
			break;

		default:
			sprintf(szNetType, "%s", "Unknown");
			break;
		}

		//PRTMSG(MSG_DBG, "PhoneWcdma Current NetWork Type is %s\n", szNetType);
		G_RecordDebugInfo("PhoneWcdma Current NetWork Type is %s\n", szNetType);

		//if( ++i % 2 == 0)
		//{
		//	char str[50] = {0x01, 0x00};
		//	sprintf(str+2, "%s", szNetType);
		//	DataPush(str, 7, DEV_QIAN, DEV_DVR, LV2);
		//}

// 		{
// 			char str[1024] = {0x01, 0x72, 0x01};
// 			sprintf(str+3, "��ǰ������: %s", szNetType);
// 			DataPush(str, 30, DEV_QIAN, DEV_DIAODU, LV2);
// 		}

		return WCDMA_ATANS_OK;
	}
	else
		return WCDMA_ATANS_ERROR;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhoneWcdma::CPhoneWcdma()
{
	m_iPhonePort = -1;

	m_hThdComRcv = 0;//��ʼ���߳̾��
	m_hThdPhoneHdl = 0;
	m_hThdOtherHdl = 0;
//	m_hThdPing = 0;

	m_objDataMngRcv.InitCfg( 60000, 60000 );
	m_objDataMngSmsSnd.InitCfg(80000, 600000);

// !!ע�����»���״̬��������Ҫmemset,�����и��ԵĹ��캯����ʼ��
// 	memset( &m_objPhoneStaDial, 0, sizeof(m_objPhoneStaDial) );
// 	memset( &m_objPhoneStaDialLsn, 0, sizeof(m_objPhoneStaDialLsn) );
// 	memset( &m_objPhoneStaHold, 0, sizeof(m_objPhoneStaHold) );
// 	memset( &m_objPhoneStaLsning, 0, sizeof(m_objPhoneStaLsning) );
// 	memset( &m_objPhoneStaRing, 0, sizeof(m_objPhoneStaRing) );
// 
// 	memset( &m_objPhoneStaCommon, 0, sizeof(m_objPhoneStaCommon) );

	m_dwClrDogTm = GetTickCount();
	m_iPhoneHdlSta = WCDMA_PST_INIT;
	m_bPhoneNetReady = false;
	m_bNeedDialNet = true;
	m_bToClosePhone = false;
	m_dwDialNetTm = 0;

	m_bSaveExitFlag = false;
	m_bSaveThRunSta = false;
	
	//m_ucCurCltCyc = 2;//at��վ��������
	//m_bGpsChangeInitFlag = false;
	
//	InitPhone();
}

CPhoneWcdma::~CPhoneWcdma()
{
	
}

void CPhoneWcdma::ResetPhone()
{
	tagWcdmaPhoneTask objPhoneTask;
	_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_RESET );
}

// WcdmaӲ����λ��rst��������3s������
// void CPhoneWcdma::_GsmHardReset()
// {
// 	IOSet(IOS_PHONERST, IO_PHONERST_LO, NULL, 0);		//RESET	
// 	sleep(3);//����3s
// 	IOSet(IOS_PHONERST, IO_PHONERST_HI, NULL, 0);		//RESET	
// 	sleep(2);
// }

// WcdmaӲ����λ��ֱ�Ӷϵ����ʱ�����ϵ�
void CPhoneWcdma::_GsmHardReset()
{
 	_GsmPowOff();
 	sleep(5);
 	_GsmPowOn();
}

// Wcdma��λ
void CPhoneWcdma::_GsmReset()
{
 	IOSet(IOS_PHONERST, IO_PHONERST_LO, NULL, 0);
}

//Wcdma�ϵ�
void CPhoneWcdma::_GsmPowOn()
{
	if (PHONE_MODTYPE == PHONE_MODTYPE_EM770W)
	{
		IOSet(IOS_PHONEPOW, IO_PHONEPOW_ON, NULL, 0);
	}
	else if (PHONE_MODTYPE == PHONE_MODTYPE_SIM5218)
	{
		IOSet(IOS_PHONEPOW, IO_PHONEPOW_ON, NULL, 0);

		IOSet(IOS_PHONERST, IO_PHONERST_LO, NULL, 0);
		usleep(200000);
		IOSet(IOS_PHONERST, IO_PHONERST_HI, NULL, 0);
	}
	else if (PHONE_MODTYPE == PHONE_MODTYPE_MU203)
	{
		IOSet(IOS_PHONEPOW, IO_PHONEPOW_ON, NULL, 0);
		
		IOSet(IOS_PHONERST, IO_PHONERST_LO, NULL, 0);
		sleep(1);
		IOSet(IOS_PHONERST, IO_PHONERST_HI, NULL, 0);
		sleep(4);
	}

	PRTMSG(MSG_DBG,"wcdma power on!\n");
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	RenewMemInfo(0x02, 0x02, 0x01, 0x00);
#endif
}

void CPhoneWcdma::_GsmPowOff()
{
	IOSet(IOS_PHONEPOW, IO_PHONEPOW_OFF, NULL, 0);
	PRTMSG(MSG_DBG,"wcdma power off!\n");
}

void CPhoneWcdma::ReDialNet()
{
	tagWcdmaPhoneTask objPhoneTask;
	_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_CUTNET );
}

// ����ֵ: 0,��ʾ������; 1,��ʾ��ע������; 2,��ʾ�Ѳ�������
BYTE CPhoneWcdma::GetPhoneState()
{
	switch( m_iPhoneHdlSta )
	{
	case WCDMA_PST_INIT:
	case WCDMA_PST_RESET:
		return 0;
		break;

	default:
		if( m_bPhoneNetReady ) return 2;
		else return 1;
	}
	return 0;
}

int CPhoneWcdma::GetPhoneHdlSta()
{
	return m_iPhoneHdlSta;
}

int CPhoneWcdma::GetPhoneSignal()
{
	tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
	return objPhoneSta.m_iSignal;
}

void CPhoneWcdma::GetPhoneIMEI( char* v_szImei, size_t v_sizImei )
{
	if( v_sizImei < 1 ) return;
	tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
	memset( v_szImei, 0, sizeof(v_sizImei) );
	memcpy( v_szImei, objPhoneSta.m_szGsmIMEI, min(v_sizImei, strlen(objPhoneSta.m_szGsmIMEI)) );
	v_szImei[ v_sizImei - 1 ] = 0;
}

int CPhoneWcdma::GetPhoneVolume()
{
	tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
	return objPhoneSta.m_iAudioOutVol;
}

unsigned long CPhoneWcdma::P_PhoneHdl()
{
	usleep(200000);
	PRTMSG(MSG_DBG,"**********\n");
	usleep(200000);
	m_iPhoneHdlSta = WCDMA_PST_INIT;

	int iResult;

	sleep(5);
	_GsmPowOn();
	sleep(8);
	
	_InitCom();
	
	while(!g_bProgExit) 
	{
		// �ú����ڲ�������
		_PhoneHdl();
	}

	g_bProgExit = true;
	pthread_exit(NULL);

	return 0;
}

//�ֻ�ģ����߳�
unsigned long CPhoneWcdma::P_ComRcv()
{
	int iReaded = 0;
	char szRcv[2048] = {0};
	DWORD dwCurTm = 0;
	DWORD dwPktNum = 0;

	int iResult;

	while(!g_bProgExit)
	{
		iResult = read(m_iPhonePort, szRcv, sizeof(szRcv));	//������
		if( iResult > 0 )
		{
			dwCurTm = GetTickCount();
			
			if( iResult > sizeof(szRcv) )
			{
				PRTMSG(MSG_ERR,"Com Read Over Buf!\n");
				memset(szRcv, 0, sizeof(szRcv));
				continue;
			}

			szRcv[ sizeof(szRcv) - 1 ] = 0;

			//PRTMSG(MSG_DBG, "PCom Rcv:%s\n",szRcv);

			//ע��push��ʱ�򣬲������ַ������������жϣ�unicode��������0x00
			//que lvl 5->1
//			m_objDataMngRcv.PushData( 1, strlen(szRcv), szRcv, dwPktNum, 0, (char*)&dwCurTm, sizeof(dwCurTm) );
			m_objDataMngRcv.PushData( 1, (DWORD)iResult, szRcv, dwPktNum, 0, (char*)&dwCurTm, sizeof(dwCurTm) );

			memset(szRcv, 0, sizeof(szRcv));
		}
		else
		{
			usleep(20000);
		}
	}

	g_bProgExit = true;
	
	return false;
}

unsigned long CPhoneWcdma::P_OtherHdl()
{
	int iResult;

	while(!g_bProgExit)
	{
		_OtherHdl();
		usleep(20000);//que ��ôû��sleep��
	}
	g_bProgExit = true;
	
	return 0;
}
/*
//que�ݲ�ʵ��
unsigned long CPhoneWcdma::P_Ping()
{
	int iResult = 0; // 0,�ɹ�; 1,ʧ��; 2,�����쳣; 3,����ֹ

	HANDLE hIcmp = NULL;
	DWORD ret = 0;
	DWORD ip_serv = 0;
	hostent* phostent = NULL;
	char reply[sizeof(icmp_echo_reply)+8];
	icmp_echo_reply* iep = (icmp_echo_reply*)&reply; 
	iep->RoundTripTime = 0xffffffff;

	hIcmp = IcmpCreateFile();
	if( INVALID_HANDLE_VALUE == hIcmp )
	{
		iResult = 2;
		goto PING_END;
	}

	// ��ȡ����IP ������ping����
	{
		DWORD dwRetVal = 0;
		DWORD ulOutBufLen = sizeof(IP_ADAPTER_INFO);
		IP_ADAPTER_INFO *pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);   
		IP_ADAPTER_INFO *pAdapter;
		if(pAdapterInfo==NULL)
		{
			iResult = 2;
			goto PING_END;
		}
		
		if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)==ERROR_BUFFER_OVERFLOW)
		{   
			free(pAdapterInfo); 
			pAdapterInfo = NULL;
			pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen); 
			if(pAdapterInfo==NULL)
			{
				iResult = 2;
				goto PING_END;
			}
		}  
    
		if( (dwRetVal=GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR )
		{  
			pAdapter = pAdapterInfo; 	
			while(pAdapter)
			{
				if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
					||				
					WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
				{
					iResult = 3;
					goto PING_END;
				}
				char *usbname = strstr(pAdapter->AdapterName, "SC2440 USB Cable");
				char *usbip = strstr(pAdapter->GatewayList.IpAddress.String, "192.168.55.100");
				if(usbname==NULL && usbip==NULL)
				{
					Sleep(2000);

					if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
						||				
						WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
					{
						iResult = 3;
						goto PING_END;
					}

					ip_serv = inet_addr(pAdapter->GatewayList.IpAddress.String);
					ret = IcmpSendEcho(hIcmp,ip_serv,0,0,NULL,reply,sizeof(icmp_echo_reply)+8,2000);
					if(ret!=0)
					{ 
					//	show_msg("ping gateway succ!");
						free(pAdapterInfo); 
						iResult = 0;
						goto PING_END;
					}
				}
				pAdapter = pAdapter->Next;   
			}   			
		}
		free(pAdapterInfo);
	}

	if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
		||				
		WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
	{
		iResult = 3;
		goto PING_END;
	}
	else
	{
		::Sleep( 2000 );
	}

	// ��ȡdns������IP ������ping����
	{
		DWORD dwRetVal = 0;
		DWORD ulOutBufLen = sizeof(FIXED_INFO);
		FIXED_INFO *pFixedInfo = (FIXED_INFO*)malloc(ulOutBufLen);   
		IP_ADDR_STRING *pIPAddr;
		if(pFixedInfo==NULL)
		{
			iResult = 2;
			goto PING_END;
		}
		 
		if(GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
		{   
			free(pFixedInfo);  
			pFixedInfo = NULL;	
			pFixedInfo = (FIXED_INFO*)malloc(ulOutBufLen); 
			if(pFixedInfo==NULL)
			{
				iResult = 2;
				goto PING_END;
			}
		}   
    
		if(dwRetVal=GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) 
		{
			ip_serv = inet_addr(pFixedInfo->DnsServerList.IpAddress.String);
			if(ip_serv != 0x0100007f)
			{
				if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
					||				
					WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
				{
					iResult = 3;
					goto PING_END;
				}
				
				ret = IcmpSendEcho(hIcmp,ip_serv,0,0,NULL,reply,sizeof(icmp_echo_reply)+8,2000);
				if(ret!=0)
				{ 
				//	show_msg("ping dns succ!");
					free(pFixedInfo); 
					iResult = 0;
					goto PING_END;
				}
			}
			pIPAddr = pFixedInfo->DnsServerList.Next;   
			while(pIPAddr)
			{  
				if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
					||				
					WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
				{
					iResult = 3;
					goto PING_END;
				}

				ip_serv = inet_addr(pIPAddr->IpAddress.String);
				if(ip_serv != 0x0100007f)
				{
					Sleep(2000);

					if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
						||				
						WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
					{
						iResult = 3;
						goto PING_END;
					}
					
					ret = IcmpSendEcho(hIcmp,ip_serv,0,0,NULL,reply,sizeof(icmp_echo_reply)+8,2000);
					if(ret!=0)
					{ 
					//	show_msg("ping dns succ!");
						free(pFixedInfo); 
						iResult = 0;
						goto PING_END;
					}
				}
				pIPAddr = pIPAddr->Next; 
			} 
		}  
		free(pFixedInfo);
	}
 
	if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
		||				
		WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
	{
		iResult = 3;
		goto PING_END;
	}
	else
	{
		::Sleep(2000);
	}

	if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
		||				
		WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
	{
		iResult = 3;
		goto PING_END;
	}

	// ��ȡ 163 IP ������ping���� 
	phostent = gethostbyname("www.163.com");
	ip_serv = (phostent==NULL) ? (0) : (*(DWORD*)(*phostent->h_addr_list));
	ret = IcmpSendEcho(hIcmp,ip_serv,0,0,NULL,reply,sizeof(icmp_echo_reply)+8,2000);
	if(ret!=0)
	{ 
		//show_msg("ping 163 succ!");
		iResult = 0;
		goto PING_END;
	}

	if( WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtEnd, 0)
		||				
		WAIT_TIMEOUT == ::WaitForSingleObject(m_hEvtPingExit, 0))
	{
		iResult = 3;
		goto PING_END;
	}

	// ��ȡ Sina IP ������ping���� 
	phostent = gethostbyname("www.sina.com.cn");
	ip_serv = (phostent==NULL) ? (0) : (*(DWORD*)(*phostent->h_addr_list));
	ret = IcmpSendEcho(hIcmp,ip_serv,0,0,NULL,reply,sizeof(icmp_echo_reply)+8,2000);
	if(ret!=0)
	{ 
		//show_msg("ping sina succ!"); 
		iResult = 0;
		goto PING_END;
	}

	iResult = 1;
PING_END:
	if( hIcmp && INVALID_HANDLE_VALUE != hIcmp )
	{
		IcmpCloseHandle( hIcmp );
	}

	if( 0 == iResult )
	{
		show_msg("ping succ!");
	}
	else if( 3 == iResult )
	{
		show_msg("ping is canceled");
	}
	else
	{
		show_msg("ping fail!");

		// Ҫ�����
		tagWcdmaPhoneTask objPhoneTask;
		_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_CUTNET );
	}

	tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
	tagWcdmaPhoneTask objPhoneTask;
	objPhoneTask.m_bytPingResult = iResult;
	objPhoneTask.m_bytPingSrc = objPhoneSta.m_bytPingSrc;
	_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_PINGRPT );

	return 0;
}
*/
int CPhoneWcdma::InitPhone()
{
	int iResult;

	//��ʼ��������
	if((0 != pthread_mutex_init( &m_crisecRcv, NULL)) || (0 != pthread_mutex_init( &m_crisecPhoneTask, NULL)) ||
		(0 != pthread_mutex_init( &m_crisecSta, NULL)) || (0 != pthread_mutex_init( &m_crisecDog, NULL)))
	{
		PRTMSG(MSG_ERR,"��ʼ��������ʧ�ܣ�\n");
		goto _INITPHONE_FAIL;
	}
	
	if(system(USB_DEV_INSMOD) != 0)
	{
		PRTMSG(MSG_ERR,"linux_usbserial.ko ����ʧ�ܣ�\n");
	}
	
// 	sleep(5);
// 	_GsmPowOn();
// 	sleep(8);
// 
// 	iResult = _InitCom();
// 	if( true != iResult ) 
// 	{
// 		G_RecordDebugInfo("PhoneWcdma InitCom Failed!");
// 		goto _INITPHONE_FAIL;
// 	}

	// �����ֻ�ģ�鴮�ڶ�ȡ�߳�
	iResult = pthread_create(&m_hThdComRcv, NULL, (void *(*)(void*))G_WCDMA_ComRcv, (void *)this);
	if(0 !=iResult)
	{
		PRTMSG(MSG_ERR,"�������ڶ�ȡ�߳�ʧ�ܣ�\n");  
		goto _INITPHONE_FAIL;
	}

	// ���� �绰���� �߳�
	iResult = pthread_create(&m_hThdPhoneHdl, NULL, (void *(*)(void*))G_WCDMA_PhoneHdl, (void *)this);
	if(0 !=iResult)
	{
		PRTMSG(MSG_ERR,"�����绰�����߳�ʧ�ܣ�\n");  
		goto _INITPHONE_FAIL;
	}

	// ���� ����� �߳�
	iResult = pthread_create(&m_hThdOtherHdl, NULL, (void *(*)(void*))G_WCDMA_OtherHdl, (void *)this);
	if(0 !=iResult)
	{
		PRTMSG(MSG_ERR,"����������߳�ʧ�ܣ�\n");  
		goto _INITPHONE_FAIL;
	}
	//���øú�����������ݻ��������������gps_distortion_initʱ�����һ��
//	gps_distortion_init(m_GpsChangeBuf);//�ڳ����ʼ��ʱ����

	return true;

_INITPHONE_FAIL:
	ReleasePhone();//ReasePhone�����˳����̱�ʶ
	return ERR_THREAD;
}

void CPhoneWcdma::ReleasePhone()
{
	//�����˳���־
#if CHK_VER == 0	
	g_bProgExit = true;
#endif
	// { ͬ�������ֻ�ģ��ر�,ʹ��״̬��Ѹ���˳��Ǹ�λ״̬���븴λ״̬,�����ڸ�λ״̬����Ȼ�����ùر�
	m_bToClosePhone = true;

	_PhoneCutNet();	
	_CloseCom();

	//���ٻ�����
	pthread_mutex_destroy( &m_crisecRcv );
	pthread_mutex_destroy( &m_crisecPhoneTask );
	pthread_mutex_destroy( &m_crisecSta );
	pthread_mutex_destroy( &m_crisecDog );

	_GsmPowOff();//ģ�鰲ȫ�ػ�
}

int CPhoneWcdma::_InitCom()
{
	int iResult;
	struct termios options;
#if CHK_VER == 0		
	const int COMOPEN_TRYMAXTIMES = 20;
#endif
#if CHK_VER == 1
	const int COMOPEN_TRYMAXTIMES = 5;
#endif
	char szUsbSerialPathName[256] = {0};
	struct dirent **ppUsbSerialNameList;
	int i, j;
	int iUsbCheckOkTimes = 0;

	for( i = 0; i < COMOPEN_TRYMAXTIMES; i ++ )
	{
#if VEHICLE_TYPE == VEHICLE_M
		iResult = access("/sys/bus/usb/drivers/usbserial_generic/1-2.3:1.0", F_OK);
#endif

#if VEHICLE_TYPE == VEHICLE_M2
		iResult = access("/sys/bus/usb/drivers/usbserial_generic/1-1.2:1.0", F_OK);
#endif

#if VEHICLE_TYPE == VEHICLE_V8
		iResult = access("/sys/bus/usb/drivers/usbserial_generic/1-1:1.0", F_OK);
		
		// ������仰�ǲ���USB2.0�����ֻ�ģ��ͨ��
		//iResult = access("/sys/devices/platform/hiusb-hcd.0/usb2/2-1/2-1:1.0", F_OK);
#endif
		
		if( 0 != iResult )
		{
			PRTMSG(MSG_ERR,"no usb device checked\n");
			G_RecordDebugInfo("SIM5218 no usb device checked");
			sleep(2);
			continue;
		}

		//Ŀ¼���ļ���
		//bAlternateSetting   bInterfaceSubClass  interface
		//bInterfaceClass     bNumEndpoints       modalias
		//bInterfaceNumber    bus                 ttyUSB0
		//bInterfaceProtocol  driver
		scandir(USB_DEV_CHECKPATH_AT, &ppUsbSerialNameList, 0, alphasort);
		sprintf(szUsbSerialPathName, "/dev/usb/tts/%c", *(ppUsbSerialNameList[11]->d_name + 6));
		if( 0 != access(szUsbSerialPathName, F_OK) )
		{
			PRTMSG(MSG_ERR,"usb device port check err\n");
			sleep(1);
			continue;
		}

		m_iPhonePort = open(szUsbSerialPathName, O_RDWR);
		if( -1 != m_iPhonePort )
		{
			PRTMSG(MSG_DBG,"Open %s succ!\n",szUsbSerialPathName);
			break;
		}

		PRTMSG(MSG_ERR,"Open %s failed!\n",szUsbSerialPathName);
		sleep(1);
	}

	if( i >= COMOPEN_TRYMAXTIMES )
	{
#if CHK_VER == 0		
		PRTMSG(MSG_ERR,"usb serial open fail to max times,reset system!");
		//g_bProgExit = true;
		G_ResetSystem();
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		g_objDiaodu.show_diaodu("�ֻ�ģ���쳣");
		RenewMemInfo(0x02, 0x00, 0x00, 0x01);
#endif

		return false;
	}	

	if(tcgetattr(m_iPhonePort, &options) != 0)
	{
		perror("GetSerialAttr");
		return false;
	}
	
	options.c_iflag &= ~(IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL|IUCLC|IXON|IXOFF|IXANY); 
	options.c_lflag &= ~(ECHO|ECHONL|ISIG|IEXTEN|ICANON);
	options.c_oflag &= ~OPOST;

//	cfsetispeed(&Opt,B460800);	//���ò�����
//	cfsetospeed(&Opt,B460800);
	
	if (tcsetattr(m_iPhonePort,TCSANOW,&options) != 0)   
	{ 
		perror("SetSerialAttr"); 
		return false;
	}

	PRTMSG(MSG_NOR,"UsbSerial open Succ!\n"); 
	return true;
}

// bool CPhoneWcdma::InitWcdmaFile()
// {
// 	bool bNeedNewFile = false;
// 	char sysVersion[32] = "app-yx001-mvr160vc-001"; //32���ֽڣ���m_objWcdmaFileHead.SysVerһ����
// 	memcpy(	m_objWcdmaFileHead.SysVer , sysVersion , sizeof(m_objWcdmaFileHead.SysVer));
// 	strcpy(	m_objWcdmaFileHead.strFileVer , WCDMA_FILE_VER);
// 	
// 	FILE *fp = fopen(WCDMA_FILE ,"rb");	
// 	if(fp) 
// 	{
// 		fread(&m_objWcdmaFileHead, 1, sizeof(m_objWcdmaFileHead), fp);
// 		if (strcmp(m_objWcdmaFileHead.strFileVer , WCDMA_FILE_VER))// �汾�Ų����
// 		{
// 			PRTMSG(MSG_DBG, "Rd WcdmaFile version Err.");
// 			fclose(fp); 
// 			unlink(WCDMA_FILE_VER);
// 			bNeedNewFile = true;
// 		}
// 		else
// 		{
// 			PRTMSG(MSG_DBG, "WcdmaFile ver=%s",WCDMA_FILE_VER);	
// 			fclose(fp); 
// 			bNeedNewFile = FALSE;
// 		}
// 	} 
// 	else 
// 	{
// 		bNeedNewFile = true;
// 	}	
// 	
// 	if(bNeedNewFile) 
// 	{
// 		fp = NULL;
// 		fp = fopen(WCDMA_FILE, "wb+");
// 		if(ferror(fp))
// 		{
// 			PRTMSG(MSG_ERR,"create WcdmaFile err_2.");
// 		}
// 		fwrite(&m_objWcdmaFileHead, 1, sizeof(m_objWcdmaFileHead), fp);
// 		if(!ferror(fp))
// 		{
// 			PRTMSG(MSG_DBG, "create WcdmaFile:%s.",WCDMA_FILE_VER);
// 		}
// 		if(fp)
// 		{
// 			fclose(fp);
// 		}
// 	}
// 	m_ulWcdmaIndex =	m_objWcdmaFileHead.ulWcdmaIndex;
// 	PRTMSG(MSG_DBG, "m_ulWcdmaIndex=%d" , m_ulWcdmaIndex);
// 	return true;
// }

int CPhoneWcdma::_CloseCom()
{
	if( -1 != m_iPhonePort )
	{
		close( m_iPhonePort );
		m_iPhonePort = -1;
	}

	return 0;
}

int CPhoneWcdma::_ComSnd(void* v_pSnd, DWORD v_dwSndLen )
{
	int iWrited = 0;
	const DWORD SNDMINTNTERVAL = 200; // ms,����̫��
	static DWORD sta_dwLstSnd = 0;
	DWORD dwToLstSnd = GetTickCount() - sta_dwLstSnd;
	if( dwToLstSnd < SNDMINTNTERVAL )
	{
		usleep( (SNDMINTNTERVAL - dwToLstSnd)*1000 );
	}
	iWrited = write(m_iPhonePort, v_pSnd, v_dwSndLen);
	sta_dwLstSnd = GetTickCount();
	if( -1 == iWrited ) return ERR_COMSND;
	if( iWrited != v_dwSndLen ) return ERR_COMSND_UNALL;

	//PRTMSG(MSG_DBG, "PCom Snd:%s\n", v_pSnd);

	return 0;
}

// SetAT�еķ���ֵ����Ҫ���׸ı�
int CPhoneWcdma::_SetAT( DWORD v_dwWaitPerTime, BYTE v_bytTryTimes, void* v_pResult, size_t v_sizResult,
	PCHKATRES v_pChkRes, DWORD v_dwSndLen, char* v_szATCmd, ... )
{
	//int euAns = WCDMA_ATANS_OTHER;
	int iResult = 0;
	int iRet = WCDMA_ATANS_NOANS; // ��ʼ������ֵΪ��Ӧ��
	DWORD dwFstResRcvTm = GetTickCount(); // ��ʼ��Ϊ��ʼʱ��
	DWORD dwAtCmdSndTm = 0;
	DWORD dwAtCmdLen = 0;
	//tagComRcvHdl objComRcvHdl;
	char *pAtResBegin = NULL;
	char *pAtResTotalBegin = NULL;
	DWORD dwAtResTotalLen = 0;
	DWORD dwResRcvTm = 0;
	DWORD dwResLen = 0;
	DWORD dwPushTm = 0;
	DWORD dwCpyLen = 0;
	DWORD dwCur = 0;
	BYTE bytLvl = 0;
	bool bToSndNext = false;
	char szAtCmdData[1024] = {0};
	char szAtResData[1024] = {0};
	char szAtResTotalData[2048] = { 0 };

	if( 0 == v_dwSndLen )
	{
		va_list ap;
		va_start(ap, v_szATCmd);
		vsprintf(szAtCmdData, v_szATCmd, ap);
		va_end(ap);
		strcat(szAtCmdData, "\r\n"); // ���ܶ�

		szAtCmdData[ sizeof(szAtCmdData) - 1 ] = 0;
		dwAtCmdLen = strlen( szAtCmdData );
	}
	else
	{
		dwAtCmdLen = min(v_dwSndLen, sizeof(szAtCmdData));
		memcpy( szAtCmdData, v_szATCmd, dwAtCmdLen );
	}

	pAtResTotalBegin = szAtResTotalData; // ��ʼ��"�ܽ�������ʼָ��"
	for( BYTE byt = 0; byt < v_bytTryTimes; byt ++ )
	{
		// ��������
		iResult = _ComSnd(szAtCmdData, dwAtCmdLen );
		if( iResult )
		{
			usleep(200000);
			continue;
		}
		dwAtCmdSndTm = GetTickCount();
		pAtResBegin = NULL; // ÿ�η��ͺ������ж�Ӧ��������ʼλ��

		// �ȴ�Ӧ��
		bToSndNext = false;
		while( GetTickCount() - dwAtCmdSndTm <= v_dwWaitPerTime )
		{
			usleep(50000);
			// ������ȡӦ��
			DWORD dwPopCumuLen = 0;
			DWORD dwBeforPopTm = GetTickCount();
			while(1)
			{
				memset( szAtResData, 0, sizeof(szAtResData) );
				iResult = m_objDataMngRcv.PopData( bytLvl, sizeof(szAtResData), dwResLen, szAtResData,
					dwPushTm, NULL, (char*)&dwResRcvTm, sizeof(dwResRcvTm) ) ;
				if( 0 == iResult )
				{
					// ��Ӧ������ӵ�"�ܽ�����"��
					if( dwResLen <= sizeof(szAtResData) )
					{
						szAtResData[ sizeof(szAtResData) - 1 ] = 0; // ���մ���
						dwCpyLen = min(dwResLen, sizeof(szAtResTotalData) - dwAtResTotalLen - 1);
						memcpy( szAtResTotalData + dwAtResTotalLen, szAtResData, dwCpyLen);

						// ���ж��Ƿ���Ǳ��������Ӧ��
						//que dwResRcvTm��
						if( !pAtResBegin && dwResRcvTm > dwAtCmdSndTm ) // ��֮ǰû�еõ���ʼӦ��, �ұ���Ӧ��ʱ���������ʱ��
						{
							pAtResBegin = szAtResTotalData + dwAtResTotalLen;
						}

						// ���ۼ��ܽ��������ݳ���
						dwAtResTotalLen += dwCpyLen;

						// �ۼӵõ����ݵ��ܳ���
						dwPopCumuLen += dwResLen;
					}
 
					// ��ֹ��һ���������ƵĶ�ȡ��������
					if( dwPopCumuLen >= 2000 || GetTickCount() - dwBeforPopTm > 6000 ) // (ʱ�����ƻ����Ǳ������,��Ҫ�Ƕ�ȡ�������ƻ���)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}

			// �ж��Ƿ�Ҫ�ȴ���"�ܽ�����"�ڵ�����
			dwCur = GetTickCount();
			if( dwCur - dwFstResRcvTm >= 3000 && pAtResTotalBegin )
			{
				// ����ģ����������������
				_PhoneDataDeal( pAtResTotalBegin );

				// ������Ϻ����״̬ˢ��
				pAtResTotalBegin = szAtResTotalData + dwAtResTotalLen;
 				dwFstResRcvTm = dwCur;
			}

			// ����δ�յ����������Ӧ��,�����´�ѭ��
			if( !pAtResBegin ) continue;
			// �Ա�������Ӧ��Ĵ���
			if( v_pChkRes )
			{
				iResult = v_pChkRes(pAtResBegin, v_pResult, v_sizResult);
			}
			else if( strstr(pAtResBegin, "OK") )
			{
				iResult = WCDMA_ATANS_OK;
			}
			else if( strstr(pAtResBegin, "CME ERROR") )
			{
				iResult = WCDMA_ATANS_CMEERR;
			}
			else if( strstr(pAtResBegin, "ERROR") )
			{
				iResult = WCDMA_ATANS_ERROR;
			}
			else
			{
				iResult = WCDMA_ATANS_RCVNOTALL;
			}

			// ���ݴ����������²�����
			switch( iResult )
			{
				case WCDMA_ATANS_OK:
					iRet = iResult;
					goto _SETAT_END;
					break;
	
				case WCDMA_ATANS_RCVNOTALL:
				case WCDMA_ATANS_NOANS:
					// �봦��ATӦ��ĺ���ע��,������ص������ϵ�Ӧ���򽫼����ȴ�
					break;
	
				case WCDMA_ATANS_CMEERR:
				case WCDMA_ATANS_ERROR:
				case WCDMA_ATANS_OTHER:
				default:
					// �봦��ATӦ��ĺ���ע��,������ص������ϵ�Ӧ�����ٴ�(δ��ʱδ��������ʱ)�������󲢵ȴ�Ӧ��
					bToSndNext = true;
					break;
			}

			if( bToSndNext )
			{
				break;
			}
		}
	}

_SETAT_END:

	// �ж��Ƿ�Ҫ����������"�ܽ�����"����
	if( pAtResTotalBegin )
	{
		// ����ģ����������������
		_PhoneDataDeal( pAtResTotalBegin );
	}

	if( WCDMA_ATANS_OK != iRet )
	{
		szAtCmdData[ strlen(szAtCmdData) - 2 ] = 0; // ȥ���س������ٴ�ӡ
		PRTMSG(MSG_ERR, "SETAT_END: Set %s not return WCDMA_ATANS_OK\n", szAtCmdData );
	}

	return iRet;
}

// ReadAT�еķ���ֵ����Ҫ���׸ı�
int CPhoneWcdma::_ReadAT( DWORD v_dwWait, void* v_pResult, size_t v_sizResult, PCHKATRES v_pChkRes )
{
	if( !v_pChkRes )
	{
		return WCDMA_ATANS_NOANS;
	}

	int iResult = WCDMA_ATANS_NOANS;
	int iRet = WCDMA_ATANS_NOANS; // ��ʼ������ֵΪ��Ӧ��
	DWORD dwResLen = 0;
	DWORD dwPushTm = 0;
	DWORD dwResRcvTm = 0;
	DWORD dwCpyLen = 0;
	DWORD dwCur = 0;
	BYTE bytLvl = 0;
	bool bToSndNext = false;
	char szAtResData[1024] = {0};
	char szAtResTotalData[2048] = { 0 };
	DWORD dwAtResTotalLen = 0;
	DWORD dwBegin = GetTickCount();

	while( GetTickCount() - dwBegin <= v_dwWait )
	{
		usleep(50000);

		// ������ȡAT����
		DWORD dwPopCumuLen = 0;
		DWORD dwBeforPopTm = GetTickCount();
		memset( szAtResTotalData, 0, sizeof(szAtResTotalData) );
		dwAtResTotalLen = 0;
		while(1)
		{			
			memset( szAtResData, 0, sizeof(szAtResData) );
			iResult = m_objDataMngRcv.PopData( bytLvl, sizeof(szAtResData), dwResLen, szAtResData,
				dwPushTm, NULL, (char*)&dwResRcvTm, sizeof(dwResRcvTm) ) ;
			if( 0 == iResult )
			{
				// ��Ӧ������ӵ�"�ܽ�����"��
				if( dwResLen <= sizeof(szAtResData) )
				{
					szAtResData[ sizeof(szAtResData) - 1 ] = 0; // ���մ���
					dwCpyLen = min(dwResLen, sizeof(szAtResTotalData) - dwAtResTotalLen - 1);
					memcpy( szAtResTotalData + dwAtResTotalLen, szAtResData, dwCpyLen);

					// �ۼ��ܽ��������ݳ���
					dwAtResTotalLen += dwCpyLen;

					// �ۼӵõ����ݵ��ܳ���
					dwPopCumuLen += dwResLen;
				}

				// ��ֹ��һ���������ƵĶ�ȡ��������
				if( dwPopCumuLen >= 2000 || GetTickCount() - dwBeforPopTm > 6000 ) // (ʱ�����ƻ����Ǳ������,��Ҫ�Ƕ�ȡ�������ƻ���)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		if( dwPopCumuLen > 0 )
		{
			// ��������Ҫ������
			if( v_pChkRes )
			{
				iResult = v_pChkRes(szAtResTotalData, v_pResult, v_sizResult);
			}
			else
			{
				iResult = WCDMA_ATANS_NOANS;
			}

			// ��������յ�����������
			_PhoneDataDeal( szAtResTotalData );

			// ���ݴ����������²�����
			switch( iResult )
			{
			case WCDMA_ATANS_OK:
				iRet = iResult;
				goto _READAT_END;
				break;

			case WCDMA_ATANS_RCVNOTALL:
			case WCDMA_ATANS_NOANS:
				// �봦��AT���ݵĺ���ע��,������ص������ϵ�Ӧ���򽫼����ȴ�
				iRet = iResult;
				break;

			default:
				// �봦��AT���ݵĺ���ע��,������ص������ϵ�Ӧ�����˳��ȴ�
				iRet = iResult;
				goto _READAT_END;
			}
		}
	}

_READAT_END:
	return iRet;
}

//WCDMA������Ϣ��ѯ
// bool CPhoneWcdma::Handler_AT_CPSI(INT8U *recvbuf, INT16U len, ATCMDACK_T *ATCmdAck)
// {
// //+CPSI:<System Mode>,<Operation Mode>,<MCC>-<MNC>,<LAC>,<Cell ID>,<Frequency Band>,<PSC>,<Freq>,<SSC>,<EC/IO>,<RSCP>,<Qual>,<RxLev>
// //
// //OK
// 
// //+CPSI: WCDMA,Online,460-01,0xD91F,93203593,WCDMA IMT 2000,316,10713,0,21,86,15,29
// //
// //OK
// 
// //	printf("%s\n",recvbuf);
// 
//   INT32U temp;
// 	
//   if(SearchKeyWord(recvbuf, len, "WCDMA,Online")) 
// 	{
// 		temp = SearchDigitalString(recvbuf, len, '-', 1, 10); //MCC
// 		temp = htons(temp);
// 	  ATCmdAck->ackbuf[0] = temp&0xff; 
// 	  ATCmdAck->ackbuf[1] = (temp>>8)&0xff;
// 	  
// 	  temp = SearchDigitalString(recvbuf, len, ',', 3, 10); //MNC
// 		temp = htons(temp);
// 	  ATCmdAck->ackbuf[2] = temp&0xff; 
// 	  ATCmdAck->ackbuf[3] = (temp>>8)&0xff;
// 	  
// 	  temp = SearchDigitalString(recvbuf, len, ',', 4, 16); //LAC
// 		temp = htons(temp);
// 	  ATCmdAck->ackbuf[4] = temp&0xff; 
// 	  ATCmdAck->ackbuf[5] = (temp>>8)&0xff;
// 	  
// 	  temp = SearchDigitalString(recvbuf, len, ',', 5, 10); //Cell ID
// 		temp = htons(temp);
// 	  ATCmdAck->ackbuf[6] = temp&0xff; 
// 	  ATCmdAck->ackbuf[7] = (temp>>8)&0xff;
// 	  ATCmdAck->ackbuf[8] = (temp>>16)&0xff; 
// 	  ATCmdAck->ackbuf[9] = (temp>>24)&0xff;
// 	  
// 	  temp = SearchDigitalString(recvbuf, len, ',', 7, 10); //PSC
// 		temp = htons(temp);
// 	  ATCmdAck->ackbuf[10] = temp&0xff; 
// 	  ATCmdAck->ackbuf[11] = (temp>>8)&0xff;
// 	  
// 	  temp = SearchDigitalString(recvbuf, len, ',', 8, 10); //Freq
// 		temp = htons(temp);
//     ATCmdAck->ackbuf[12] = temp&0xff;
//     ATCmdAck->ackbuf[13] = (temp>>8)&0xff;
//     
//     temp = SearchDigitalString(recvbuf, len, ',', 9, 10); //SSC
// 		temp = htons(temp);
//     ATCmdAck->ackbuf[14] = temp&0xff;
//     ATCmdAck->ackbuf[15] = (temp>>8)&0xff;
//     
//     temp = SearchDigitalString(recvbuf, len, ',', 10, 10); //EC/IO
// 		temp = htons(temp);
// 	  ATCmdAck->ackbuf[16] = temp&0xff; 
// 	  ATCmdAck->ackbuf[17] = (temp>>8)&0xff;
// 	  
// 	  temp = SearchDigitalString(recvbuf, len, ',', 11, 10); //RSCP
// 		temp = htons(temp);
// 	  ATCmdAck->ackbuf[18] = temp&0xff; 
// 	  ATCmdAck->ackbuf[19] = (temp>>8)&0xff;
// 	  
// 	  temp = SearchDigitalString(recvbuf, len, ',', 12, 10); //Qual
// 		temp = htons(temp);
// 	  ATCmdAck->ackbuf[20] = temp&0xff; 
// 	  ATCmdAck->ackbuf[21] = (temp>>8)&0xff;
// 
//     temp = SearchDigitalString(recvbuf, len, '\r', 1, 10); //RxLev
// 		temp = htons(temp);
//     ATCmdAck->ackbuf[22] = temp&0xff;
//     ATCmdAck->ackbuf[23] = (temp>>8)&0xff;    
// 
//     return true;
//   }
// 	else 
// 	{
//     return false;
//   }
// }
// 
// bool CPhoneWcdma::Handler_AT_CRUS(INT8U *recvbuf, INT16U len, ATCMDACK_T *ATCmdAck)
// {
// //+CRUS:Active SET,<ActiveSET Cells Num>[,<ActiveSET Cell1 PSC>,<ActiveSET Cell1 Freq>,<ActiveSET Cell1 SSC>,<ActiveSET Cell1 Sttd>,<ActiveSET Cell1 TotEcio>,<ActiveSET Cell1 Ecio>,<ActiveSET Cell1 Rscp>,<ActiveSET Cell1 WinSize>[...]]
// //
// //+CRUS:Sync Neighbor SET,<SyncSET Cells Num>[,<SyncSET Cell1 PSC>,<SyncSET Cell1 Freq>,<SyncSET Cell1 SSC>,<SyncSET Cell1 Sttd>,<SyncSET Cell1 TotEcio>,<SyncSET Cell1 Ecio>,<SyncSET Cell1 Rscp>,<SyncSET Cell1 WinSize>[...]]
// //
// //+CRUS:Async Neighbor SET,<AsyncSET Cells Num>[,<AsyncSET Cell1 PSC>,<AsyncSET Cell1 Freq>,<AsyncSET Cell1 SSC>,<AsyncSET Cell1 Sttd>,<AsyncSET Cell1 TotEcio>,<AsyncSET Cell1 Ecio>,<AsyncSET Cell1 Rscp>,<AsyncSET Cell1 WinSize>[...]]
// //
// //OK
// 
// //+CRUS: Active SET,1,216,10713,0,0,26,19,85,0,0,1536
// //
// //+CRUS: Sync Neighbor SET,3,256,10713,0,0,31,29,90,1536,178,10713,0,0,11,30,91,1536,186,10713,0,0,24,26,89,1536
// //
// //+CRUS: Async Neighbor SET,16,168,10713,0,0,31,49,121,512,145,10713,0,0,0,49,121,512,153,10713,0,0,0,49,121,512,83,10713,0,0,0,49,121,0,289,10713,0,0,0,49,121,0,297,10713,0,0,0,49,121,0,272,10713,0,0,63,49,121,1536,240,10713,0,0,0,49,121,0,248,10713,0,0,0,49,121,0,225,10713,0,0,0,49,121,0,233,10713,0,0,0,49,121,0,192,10713,0,0,0,49,121,0,170,10713,0,0,0,49,121,0,384,10713,0,0,46,49,121,3072,324,10713,0,0,0,49,121,0,224,10713,0,0,63,49,121,3072
// //
// //OK
// 
// //	printf("%s\n",recvbuf);
// 
//   INT32U temp, Actno=0, Syncno=0, Asyncno=0;
//   INT8U *Actbuf=NULL, *Syncbuf=NULL, *Asyncbuf=NULL;
//   
//   if(SearchKeyWord(recvbuf, len, "+CRUS")) 
// 	{
// 		Actbuf = (INT8U *)strstr((char*)recvbuf,"Active SET");
// 		if( Actbuf!=NULL && *(Actbuf+11)!='N' ) //�Ƿ���ڼ���С��
// 			Actno = SearchDigitalString(Actbuf, len, ',', 2, 10); // ����С������
// 		ATCmdAck->ackbuf[0] = (htons(Actno))&0xff;
//     ATCmdAck->ackbuf[1] = ((htons(Actno))>>8)&0xff; 
//     if(Actno)
// 		{
//       for(int i=0; i<(Actno<6?Actno:6); i++) 
// 			{
//         temp = SearchDigitalString(Actbuf, len, ',', 3+i*10, 10);
//         temp = htons(temp);
//         ATCmdAck->ackbuf[2+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[3+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Actbuf, len, ',', 4+i*10, 10);
//         temp = htons(temp);
//         ATCmdAck->ackbuf[4+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[5+i*16] = (temp>>8)&0xff;
//         
//         temp = SearchDigitalString(Actbuf, len, ',', 5+i*10, 10);
//         temp = htons(temp);
//         ATCmdAck->ackbuf[6+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[7+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Actbuf, len, ',', 6+i*10, 10);
//         temp = htons(temp);
//         ATCmdAck->ackbuf[8+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[9+i*16] = (temp>>8)&0xff;
//         
//         temp = SearchDigitalString(Actbuf, len, ',', 7+i*10, 10);
//         temp = htons(temp);
//         ATCmdAck->ackbuf[10+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[11+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Actbuf, len, ',', 8+i*10, 10);
//         temp = htons(temp);
//         ATCmdAck->ackbuf[12+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[13+i*16] = (temp>>8)&0xff;
//         
//         temp = SearchDigitalString(Actbuf, len, ',', 9+i*10, 10);
//         temp = htons(temp);
//         ATCmdAck->ackbuf[14+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[15+i*16] = (temp>>8)&0xff; 
//         
//         if( i!=Actno-1 && i!=5 )
//         	temp = SearchDigitalString(Actbuf, len, ',', 12+i*10, 10);
//         else
//         	temp = SearchDigitalString(Actbuf, len, '\r', 1, 10);
//         temp = htons(temp);
//         ATCmdAck->ackbuf[16+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[17+i*16] = (temp>>8)&0xff;
//       }
//     }
// 
//     Syncbuf = (INT8U *)strstr((char*)recvbuf,"Sync Neighbor SET");
// 		if( Syncbuf!=NULL && *(Syncbuf+18)!='N' ) //�Ƿ����ͬ������
//     	Syncno = SearchDigitalString(Syncbuf, len, ',', 2, 10); // ͬ����������
//     ATCmdAck->ackbuf[98] = (htons(Syncno))&0xff; 
//     ATCmdAck->ackbuf[99] = ((htons(Syncno))>>8)&0xff; 
//     if(Syncno) 
// 		{
//       for(int i=0; i<(Syncno<6?Syncno:6); i++) 
// 			{
//         temp = SearchDigitalString(Syncbuf, len, ',', 3+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[100+i*16] = temp&0xff; 
//         ATCmdAck->ackbuf[101+i*16] = (temp>>8) & 0xff; 
//         
//         temp = SearchDigitalString(Syncbuf, len, ',', 4+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[102+i*16] = temp&0xff; 
//         ATCmdAck->ackbuf[103+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Syncbuf, len, ',', 5+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[104+i*16] = temp&0xff; 
//         ATCmdAck->ackbuf[105+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Syncbuf, len, ',', 6+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[106+i*16] = temp&0xff; 
//         ATCmdAck->ackbuf[107+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Syncbuf, len, ',', 7+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[108+i*16] = temp&0xff; 
//         ATCmdAck->ackbuf[109+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Syncbuf, len, ',', 8+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[110+i*16] = temp&0xff; 
//         ATCmdAck->ackbuf[111+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Syncbuf, len, ',', 9+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[112+i*16] = temp&0xff; 
//         ATCmdAck->ackbuf[113+i*16] = (temp>>8)&0xff; 
//         
//         if( i!=Syncno-1 && i!=5 )
//         	temp = SearchDigitalString(Syncbuf, len, ',', 10+i*8, 10);
//         else
//         	temp = SearchDigitalString(Syncbuf, len, '\r', 1, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[114+i*16] = temp&0xff; 
//         ATCmdAck->ackbuf[115+i*16] = (temp>>8)&0xff; 
//       }
//     }
//         
//     Asyncbuf = (INT8U *)strstr((char*)recvbuf,"Async Neighbor SET");
//     if( Asyncbuf!=NULL && *(Asyncbuf+19)!='N' ) //�Ƿ�����첽����
//    		Asyncno = SearchDigitalString(Asyncbuf, len, ',', 2, 10); // �첽��������
//     ATCmdAck->ackbuf[196] = (htons(Asyncno))&0xff;
//     ATCmdAck->ackbuf[197] = ((htons(Asyncno))>>8)&0xff;  
//     if(Asyncno) 
// 		{
//       for(int i=0; i<(Asyncno<6?Asyncno:6); i++) 
// 			{
//         temp = SearchDigitalString(Asyncbuf, len, ',', 3+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[198+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[199+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Asyncbuf, len, ',', 4+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[200+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[201+i*16] = (temp>>8)&0xff;
//         
//         temp = SearchDigitalString(Asyncbuf, len, ',', 5+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[202+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[203+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Asyncbuf, len, ',', 6+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[204+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[205+i*16] = (temp>>8)&0xff;    
//         
//         temp = SearchDigitalString(Asyncbuf, len, ',', 7+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[206+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[207+i*16] = (temp>>8)&0xff; 
//         
//         temp = SearchDigitalString(Asyncbuf, len, ',', 8+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[208+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[209+i*16] = (temp>>8)&0xff;    
//         
//         temp = SearchDigitalString(Asyncbuf, len, ',', 9+i*8, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[210+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[211+i*16] = (temp>>8)&0xff; 
//         
//         if( i!=Asyncno-1 && i!=5 )
//         	temp = SearchDigitalString(Asyncbuf, len, ',', 10+i*8, 10);
//         else
//         	temp = SearchDigitalString(Asyncbuf, len, '\r', 1, 10);
// 				temp = htons(temp);
//         ATCmdAck->ackbuf[212+i*16] = temp&0xff;
//         ATCmdAck->ackbuf[213+i*16] = (temp>>8)&0xff;
//       }
//     }
//     return true;
//   } 
// 	else 
// 	{
//     return false;
//   }
// }

// void CPhoneWcdma::SaveWcdmaDataRst()
// {
// 	FILE *fpWcdma = NULL;
// 
// 	BYTE bytLvl = 0;
// 	DWORD dwDataLen;
// 	DWORD dwPushTm;
// 	tagWcdmaBaseData objWcdmaBaseData;
// 
// 	if(m_objWcdmaBaseQue.GetPacketCount() > 0) //������������ݴ���1000����ô��Ҫ�洢�ˡ�
// 	{
// 		fpWcdma = fopen(WCDMA_FILE,"rab+");
// 		if (NULL!=fpWcdma) 
// 		{	
// 			m_objWcdmaFileHead.ulWcdmaIndex = m_ulWcdmaIndex;
// 			fseek(fpWcdma,0,SEEK_SET);
// 			fwrite((byte*)&m_objWcdmaFileHead,sizeof(m_objWcdmaFileHead),1,fpWcdma); 
// 			if(ferror(fpWcdma))
// 			{	
// 				PRTMSG(MSG_DBG, "Write Wcdmafile Head err.");
// 				goto SAVE_ERR;
// 			}
// 
// 			while(m_objWcdmaBaseQue.GetPacketCount())
// 			{
// 				m_objWcdmaBaseQue.PopData(bytLvl,sizeof(objWcdmaBaseData),dwDataLen,(char*)&objWcdmaBaseData,dwPushTm);
// 				fseek(fpWcdma,0,SEEK_END);
// 				fwrite((byte*)&objWcdmaBaseData,sizeof(tagWcdmaBaseData),1,fpWcdma);
// 				if(ferror(fpWcdma))
// 				{	
// 					PRTMSG(MSG_DBG, "Write Wcdmafile Data err.");
// 					goto SAVE_ERR;
// 				}
// 			}
// 		}
// 	}
// 
// 	PRTMSG(MSG_DBG, "SaveWcdmaDataRst ok!!");
// SAVE_ERR:
// 	if(NULL!=fpWcdma) fclose(fpWcdma);
// 	m_bSaveExitFlag = true;
// }

// void CPhoneWcdma::P_SaveWcdmaData()
// {
// 	while(true != m_bSaveExitFlag)
// 	{
// 		m_bSaveThRunSta = true;
// 		FILE *fpWcdma = NULL;
// 		
// 		BYTE bytLvl = 0;
// 		DWORD dwDataLen;
// 		DWORD dwPushTm;
// 		tagWcdmaBaseData objWcdmaBaseData;
// 		tagWcdmaBaseData objDataFst;
// 		if(m_objWcdmaBaseQue.GetPacketCount() >= WCDMA_WRITE_FILE_CNT) //������������ݴ���1000����ô��Ҫ�洢�ˡ�
// 		{
// 			fpWcdma = fopen(WCDMA_FILE,"rab+");
// 			fseek(fpWcdma,sizeof(m_objWcdmaFileHead),SEEK_SET);
// 			if (NULL!=fpWcdma) 
// 			{	
// 				PRTMSG(MSG_DBG, "P_SaveWcdmaData=%d",m_objWcdmaBaseQue.GetPacketCount() );	
// 				if(!feof(fpWcdma)){fread(&objDataFst,sizeof(tagWcdmaBaseData),1,fpWcdma);}
// 				for(int i=0;i<WCDMA_WRITE_FILE_CNT;i++)
// 				{
// 					m_objWcdmaBaseQue.PopData(bytLvl,sizeof(objWcdmaBaseData),dwDataLen,(char*)&objWcdmaBaseData,dwPushTm);
// 					fseek(fpWcdma,0,SEEK_END);
// 					fwrite((byte*)&objWcdmaBaseData,sizeof(tagWcdmaBaseData),1,fpWcdma);
// 					if(ferror(fpWcdma))
// 					{	
// 						PRTMSG(MSG_DBG, "Write Wcdmafile Data err.");
// 						goto SAVE_ERR;
// 					}
// 				}
// 				fseek(fpWcdma, 0, SEEK_END);
// 				ulong fsize = ftell(fpWcdma);
// 				m_objWcdmaFileHead.ulPackNum = (fsize - sizeof(m_objWcdmaFileHead)) / sizeof(objWcdmaBaseData);
// 				m_objWcdmaFileHead.ulWcdmaIndex = m_ulWcdmaIndex;
// 				fseek(fpWcdma,0,SEEK_SET);
// 				fwrite((byte*)&m_objWcdmaFileHead,sizeof(m_objWcdmaFileHead),1,fpWcdma); 
// 				if(ferror(fpWcdma))
// 				{	
// 					PRTMSG(MSG_DBG, "Write Wcdmafile Head err.");
// 					goto SAVE_ERR;
// 				}
// 				PRTMSG(MSG_DBG, "File Obj=%d/%d",m_objWcdmaFileHead.ulPackNum ,m_ulWcdmaIndex - objDataFst.m_ulWcdmaIdx+1 );
// 			}
// 		}
// 		
// SAVE_ERR:
// 		if(NULL!=fpWcdma) 
// 		{
// 			fclose(fpWcdma);
// 		}
// 		m_bSaveExitFlag = true;
// 	}
// 	m_bSaveThRunSta = false;
// }

// void CPhoneWcdma::P_QueryWcdmaBase()
// {
// 	tagGPSData gps(0);
// 	GetCurGps( &gps, sizeof(gps), GPS_REAL ); // ��ȡGPSʵʱ����
// 
// 
// #if RESET_ENABLE_ALL == 1
// 	if ('A'!=gps.valid) //ACC��Ч�Ͳ�������
// 	{
// 		m_bGpsChangeInitFlag = false;
// 		PRTMSG(MSG_DBG, "gps Invalid return.\n");
// 		return;
// 	}	
// 	if( gps.speed*3.6 > 80 )
// 	{
// 		m_ucCurCltCyc = m_objWcdmaCltCyc.i80toupSampCycle;
// 	}
// 	if( gps.speed*3.6 <= 80 && gps.speed*3.6 > 45)
// 	{
// 		m_ucCurCltCyc = m_objWcdmaCltCyc.i45to80SampCycle;
// 	}
// 
// 	if( gps.speed*3.6 <= 45 && gps.speed*3.6 > 10)
// 	{
// 		m_ucCurCltCyc = m_objWcdmaCltCyc.i10to45SampCycle;
// 	}
// 
// 	if( gps.speed*3.6 <= 10)
// 	{
// 		m_ucCurCltCyc = m_objWcdmaCltCyc.i00to10SampCycle;
// 	}
// #endif
// 
// //	gps.longitude = 118.118946;
// //	gps.latitude  =  24.486080;
// //	gps.valid     = 'A';
// 
// 	unsigned char uszIoSta;
// 	IOGet((byte)IOS_ACC, &uszIoSta );
// 	if (IO_ACC_OFF == uszIoSta) 
// 	{
// 		g_bNeedUpLine  = 1;
// 		PRTMSG(MSG_DBG,"acc Invalid return.");
// 		return;
// 	}
// 	else
// 	{
// 		g_bNeedUpLine  = 0;
// 	}
// 
// // 	if ( m_bPhoneNetReady )
// // 	{
// // 		if (!g_bNeedUpLine )
// // 		{	
// // 			PRTMSG(MSG_DBG, "Need stGprsCut 2.");
// // 			tagWcdmaPhoneTask objPhoneTask;
// // 			_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_CUTNET );
// // 		}	
// // 		PRTMSG(MSG_DBG, "QueryWcdma gprson return.");
// // 		return;
// // 	}
// 
// 	if(WCDMA_PST_IDLE!=m_iPhoneHdlSta) 
// 	{
// 		PRTMSG(MSG_DBG, "P_QueryWcdmaBase not stIdle return.\n");
// 		return;
// 	}
// 	tagWcdmaBaseData objWcdmaBaseData;
// 	memset(&objWcdmaBaseData,0,sizeof(tagWcdmaBaseData));
// 
// 	{
// 		time_t  lCurrentTime;
// 		struct tm *pCurrentTime;
// 		time(&lCurrentTime);
// 		pCurrentTime = localtime(&lCurrentTime);
// 		G_GetLocalTime( pCurrentTime );
// 
// 		objWcdmaBaseData.m_bytYear = (byte)(pCurrentTime->tm_year +1900 - 2000);
// 		objWcdmaBaseData.m_bytMon  = (byte)(pCurrentTime->tm_mon+1);//�·�0~~11
// 		objWcdmaBaseData.m_bytDay  = (byte)pCurrentTime->tm_mday;
// 		objWcdmaBaseData.m_bytHour = (byte)pCurrentTime->tm_hour;
// 		objWcdmaBaseData.m_bytMin  = (byte)pCurrentTime->tm_min;
// 		objWcdmaBaseData.m_bytSec  = (byte)pCurrentTime->tm_sec;
// 
// 	//	GetHeight(szData,lheihgt);
// 		int Days = count_days(pCurrentTime->tm_year +1900,pCurrentTime->tm_mon+1,pCurrentTime->tm_mday)
// 			-  count_days(1980,1,6);
// 		unsigned long uiLon = 0, uiLat = 0;
// 		//GPS���α�ת��������һ���ṩ
// 		unsigned long uiValue;
// // 		long buffer[256];//buffer �ӿں����ڲ�ʹ��
// // 		gps_distortion_init(buffer);//�ڳ����ʼ��ʱ����
// 		if ( false == m_bGpsChangeInitFlag )
// 		{
// 			uiValue = gps_wgtochina_lb( m_GpsChangeBuf, (long)0, (unsigned int)(gps.longitude * 3686400.0),
// 				(unsigned int)(gps.latitude * 3686400.0), 0 ,  int(Days/ 7),
// 				(unsigned int)( Days % 7 * 86400 + pCurrentTime->tm_hour * 3600 + pCurrentTime->tm_min * 60 + pCurrentTime->tm_sec ) * 1000, 												&uiLon, &uiLat );
// 			
// 			if (uiValue) 
// 			{
// 				PRTMSG(MSG_DBG, "calc GpsEx1 Err\n");
// 				return;
// 			}
// 			m_bGpsChangeInitFlag = true;
// 		}
// 
// 		uiValue = gps_wgtochina_lb( m_GpsChangeBuf, (long)1, (unsigned int)(gps.longitude * 3686400.0),
// 			(unsigned int)(gps.latitude * 3686400.0), 0 ,  int(Days/ 7),
// 			(unsigned int)( Days % 7 * 86400 + pCurrentTime->tm_hour * 3600 + pCurrentTime->tm_min * 60 + pCurrentTime->tm_sec ) * 1000, 												&uiLon, &uiLat );
// 
// 		if (!uiValue) 
// 		{	
// 			G_Lonlat_DTo4Char( (double)uiLon/3686400, objWcdmaBaseData.m_szLonEx);
// 			G_Lonlat_DTo4Char( (double)uiLat/3686400, objWcdmaBaseData.m_szLatEx);
// 		}
// 		else
// 		{
// 			PRTMSG(MSG_DBG, "calc GpsEx2 Err\n");
// 			return;
// 		}
// 	}
// 
// 	//��̨����״̬��Bit7------0��00TPXYFG�����ֽڱ�ʾ��λ�������£�
// 	//TP��01- �ƶ���  10- ��ͨ��11- ���š�00- ��Ч��(2G)
// 	//XY��01- �ƶ���  10- ��ͨ��11- ���š�00- ��Ч��(3G)
// 	// F��������״̬��1-������  0-�ǽ����� 
// 	// G����λ״̬��  1-��Ч��  0-��Ч��
// #if RESET_ENABLE_ALL == 1
// 	if ('A'==gps.valid) 
// 	{
// 		objWcdmaBaseData.m_bytSta |=  0x01;
// 	}else
// 	{
// 		objWcdmaBaseData.m_bytSta |= ~0x01;
// 	}
// 	objWcdmaBaseData.m_bytSta |=  0x08;//��ͨ
// #else
// 	objWcdmaBaseData.m_bytSta  = 0x08;
// #endif
// 	
// //	objWcdmaBaseData.m_bytYear = BYTE(gps.nYear % 1000);
// //	objWcdmaBaseData.m_bytMon = BYTE(gps.nMonth);
// //	objWcdmaBaseData.m_bytDay = BYTE(gps.nDay);
// //	objWcdmaBaseData.m_bytHour = BYTE(gps.nHour);
// //	objWcdmaBaseData.m_bytMin = BYTE(gps.nMinute);
// //	objWcdmaBaseData.m_bytSec = BYTE(gps.nSecond);
// 
// 	G_Lonlat_DTo4Char( gps.longitude, objWcdmaBaseData.m_szLon);
// 	G_Lonlat_DTo4Char( gps.latitude,  objWcdmaBaseData.m_szLat);
// 		
// 	query_CPSI();
// 	query_CRUS();
// 	
// 	objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t = s_cpsi_t;
// 	objWcdmaBaseData.m_objWcdmaBaseDataEx.m_crus_t = s_crus_t;
// 	objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t.MCC = change_cellid(objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t.MCC);
// 	objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t.MNC = change_cellid(objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t.MNC);
// 	objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t.CellIDLow = change_cellid(objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t.CellIDLow);
// 	objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t.CellIDHigh = change_cellid(objWcdmaBaseData.m_objWcdmaBaseDataEx.m_cpsi_t.CellIDHigh);
// 	
// 	DWORD dwPackNum;
// 	char sndBuf[10240] = {0};
// 	m_ulWcdmaIndex ++;
// 	objWcdmaBaseData.m_ulWcdmaIdx = m_ulWcdmaIndex;
// 	m_objWcdmaBaseQue.PushData(1,sizeof(objWcdmaBaseData),(char*)&objWcdmaBaseData,dwPackNum); //�ŵ�ͷ��
// 	//SetWcdmaBaseData(&objWcdmaBaseData);
// 	sndBuf[0] = 0xf6;//���ӻ�վ����Phone->QianExe
// 	memcpy(sndBuf+1, (const char*)&objWcdmaBaseData, sizeof(objWcdmaBaseData));
// 	DataPush(sndBuf, sizeof(objWcdmaBaseData)+1, DEV_PHONE, DEV_QIAN, 2);	//
// 	PRTMSG(MSG_DBG, "P_QueryWcdmaBase=%d\n",m_ulWcdmaIndex);
// }

// �����ֻ�ģ��״̬���߳���ʹ��
int CPhoneWcdma::_PhoneDataDeal(char* v_szAtData)
{
	tagWcdmaPhoneTask objPhotoTask;
	DWORD dwToSetTaskSymb = 0;

	// �ж��Ƿ��յ�SIM������֪ͨ que�Ƿ���Ҫ�����Ĺ�����������ѯ��


	// �ڿ���״̬��,�ж��Ƿ�������
	if( WCDMA_PST_IDLE == m_iPhoneHdlSta && strstr(v_szAtData, "RING") )
	{
		dwToSetTaskSymb |= WCDMA_PHTASK_RING;
	}

	// ��ͨ��״̬��,�ж��Ƿ�Է��һ�
	if( (WCDMA_PST_HOLD == m_iPhoneHdlSta || WCDMA_PST_LSNING == m_iPhoneHdlSta) && strstr(v_szAtData, "CEND:") )
	{
		PRTMSG(MSG_DBG, "Find Remote Hangup!\n" );
		dwToSetTaskSymb |= WCDMA_PHTASK_HANGUP;
	}

	// ���ָ��
	if( dwToSetTaskSymb )
	{
		_PhoneTaskSet(objPhotoTask, dwToSetTaskSymb);
	}

/*
	// ���Ž��մ���
	{
		char* pBuf = v_szAtData;
		char* pSmsBegin = NULL;
		char* pUData = NULL;
		char* pComma[13] = {NULL};//��n����λ��n:0--12
		char* pTelNum = NULL;
		size_t len;
		DWORD dwPktNum;

		while( pSmsBegin = strstr(pBuf, "^HCMGR:") )
		{
			PRTMSG(MSG_DBG, "!!!!!^HCMGR Msg" );

//			PRTMSG(MSG_DBG, "Rcv HCMT msg:%s\n",pBuf);
			//���룬�꣬�£��գ�ʱ���֣��룬���ԣ���ʽ�����ȣ�ptr��prv��type��tag
			//^HCMT:15980856182,2009,11,12,15,45,11,0,��ʽ1,����3,0,0,0,0
			//456 ,����
			SM_PARAM sm;
			int i;

			pTelNum = strchr(pSmsBegin, ':');
			pTelNum += 1;
			pComma[0] = strchr(pSmsBegin, ',');
			len = pComma[0] - pTelNum;
			memcpy(sm.tpa, pTelNum, len);
		
			for( i=1; i<13; i++)
			{
				// Ѱ����ʼλ��
				pComma[i] = strchr((pComma[i-1]+1), ',');
				if( !pComma[i] ) 
				{
					return WCDMA_ATANS_OTHER;
				}
			}

			//����ʱ��
			sm.scts[0] = *(pComma[0]+3);//��
			sm.scts[1] = *(pComma[0]+4);

			sm.scts[2] = *(pComma[1]+1);//��
			sm.scts[3] = *(pComma[1]+2);

			sm.scts[4] = *(pComma[2]+1);//��
			sm.scts[5] = *(pComma[2]+2);

			sm.scts[6] = *(pComma[3]+1);
			sm.scts[7] = *(pComma[3]+2);

			sm.scts[8] = *(pComma[4]+1);
			sm.scts[9] = *(pComma[4]+2);

			sm.scts[10] = '0';
			sm.scts[11] = '0';
			//����ʱ��

			
			int num = 0;
			sscanf((pComma[7]+1),"%d",&num);
			if (0==num)
			{
				sm.dcs = SMS_7BIT;
			}
			else if(1==num)
			{
				sm.dcs = SMS_7BIT;//QUE ASCI
			}
			else if (6==num)
			{
				sm.dcs = SMS_UCS2;
			}
			else
			{
				PRTMSG(MSG_ERR,"wcdma sms DCS err\n");
				return false;
			}

			//���ų����ж�
			num = 0;
			sscanf((pComma[8]+1),"%d",&num);
				
			if (num>160)    return false;
			sm.udl = (size_t)num;


			pUData = strstr( pSmsBegin, "\r\n" );
			if( !pUData )
			{
				break;
			}
			PRTMSG(MSG_NOR,"sms data:0x%x0x%x0x%x0x%x0x%x0x%x0x%x0x%x0x%x0x%x\n",pUData[0],pUData[1],pUData[2],pUData[3],pUData[4],pUData[5],pUData[6],pUData[7]);
			pUData+=2;
			
			size_t dstLen = wcdma_decode_pdu(pUData, &sm);//��������
			//sms_deal(&sm);
			m_objDataMngSmsRcv.PushData((BYTE)LV1, sizeof(sm), (char*)&sm, dwPktNum);
			sm.udl = dstLen;
			
			pBuf = pUData;
		}
	}
*/

	// ���Ž��մ���
	{
		char* pBuf = v_szAtData;
		char* p = NULL;
		char* pTemp = NULL;
		while( p = strstr(pBuf, "+CMT:") )
		{
			PRTMSG(MSG_DBG, "!!!!!CMT Msg" );
			
			// +CMT:[<alpha>],<length><CR><LF><pdu>
			
			//+CMT: "",60
			//0891683108501935F5240D91683150256111F100089040129001352328621160F389819009987994FE67610054004A004D00450074006D00610038003400367A7A88AD6211
			
			p = strstr( p, "\r\n" );
			if( !p )
			{
				break;
			}
			p+=2;
			
			SM_PARAM sm;
			int iDeLen = decode_pdu(p, &sm);
			sms_deal(&sm);
			
			pBuf = p;
		}
	}

	return true;
}


int CPhoneWcdma::_OtherHdl()
{
	// ���绰�̵߳Ŀ��Ź�(�糬��5����δ�忴�Ź���λϵͳ)
	bool bResetSys = false;
	pthread_mutex_lock( &m_crisecDog );
	unsigned int uiCur = GetTickCount();

	//uiCur���ܻ�С��m_dwClrDogTm������uiCur-m_dwClerDogTm������һ������300000�ĺܴ��ֵ������ϵͳ��λ
	//if( uiCur - m_dwClrDogTm > 300000 ) 

	int iDiff = uiCur - m_dwClrDogTm;	
	if( iDiff > 300000 ) 
	{
//		PRTMSG(MSG_DBG,"other tick:%lu\n", uiCur);
// 		PRTMSG(MSG_DBG,"other DogTm:%lu\n",m_dwClrDogTm);
		bResetSys = true;
	}
	pthread_mutex_unlock( &m_crisecDog );
	if( true == bResetSys )
	{
		PRTMSG(MSG_DBG,"Thread(PhoneHdl) long time block!");

		pthread_mutex_lock( &m_crisecDog );
		m_dwClrDogTm = GetTickCount();
		pthread_mutex_unlock( &m_crisecDog );
		
#if CHK_VER == 0		
		//g_objPhoneWcdma.ReleasePhone();
		G_ResetSystem();
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		g_objDiaodu.show_diaodu("�ֻ�ģ���쳣");
		RenewMemInfo(0x02, 0x00, 0x00, 0x01);
#endif

		return 0;
	}

	BYTE src = 0;
	BYTE bytLvl = 0;
	DWORD cmdlen = 0;
	char cmd[1024] = {0};
	DWORD dwPushTm;
	DWORD dwPktNum;
	if( (0 != g_objPhoneRecvDataMng.PopData(bytLvl, (DWORD)sizeof(cmd), cmdlen, (char*)cmd, dwPushTm, &src)) || (0==cmdlen))
	{
		usleep(50000);
		return 0;
	}

	int iPhoneHdlSta = m_iPhoneHdlSta; // ������΢Υ��һ�¹���,�����������,������ȡһ�����ⲻ��,��Ҫ���þͺ�

	switch(byte(cmd[0]))
	{
		//--------------------------------------------------
	case 0x10:	//����DTMF����
		{
			char ch = cmd[1];
			bool succ = ((ch>='0'&&ch<='9')||(ch>='A'&&ch<='D')||(ch=='*')||(ch=='#'));
			succ = succ && (WCDMA_PST_HOLD == iPhoneHdlSta);
			if(!succ)
			{
				Frm11 frm;
				frm.reply = 0x02;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			else
			{
				tagWcdmaPhoneTask objPhoneTask;
				objPhoneTask.m_szDTMF[0] = ch;
				_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_SNDDTMF );
			}
		}
		break;
		

		//--------------------------------------------------
	case 0x12:	// �������� ���к��볤��(1)+���к���(n)
		{
			Frm12 *pfrm = (Frm12*)cmd;

			// �жϺ�������(0,������� 1,��̨������������ 2,��ֹ�������� 3,���ķ���绰����)
			WORD wHoldLmtPrid = 0;
			byte type = tel_type(true, pfrm->num, pfrm->len, &wHoldLmtPrid);
			PRTMSG(MSG_DBG,"Dial %.15s(%02x, %u)...\n", pfrm->num, type, wHoldLmtPrid);
			type>>=4;
			
			// �Ƿ��ǽ�������
			char num[256] = {0};
			memcpy(num, pfrm->num, pfrm->len);
			bool ishurry =  ( strcmp(num, "110") == 0 ) || ( strcmp(num, "112") == 0 )
							|| ( strcmp(num, "119") == 0 ) ||( strcmp(num, "120") == 0 )
							|| ( strcmp(num, "122") == 0 ) || ( strcmp(num, "10010") == 0 );

			Frm13 frm;
			frm.reply = 0x01;
			if(WCDMA_PST_IDLE != iPhoneHdlSta)
			{
				frm.reply = (WCDMA_PST_INIT == iPhoneHdlSta) ? 0x04 : 0x02; // û������������:�ֻ���æ
			}
			else if( ishurry )
			{
				frm.reply = 0x01; // �����������
			}
			else if(type==2)
			{
				frm.reply = 0x05; // ���������Ƶĺ���
			} 

// 			// �ж��Ƿ��ڱ���״̬��(����������������ķ���绰�������ж�)
// 			if(1!=type && 3!=type) {
// 				if(m_almtime!=0 && (GetTickCount()-m_almstart)<m_almtime ) {
// 					frm.reply = 0x02;
// 				} else {
// 					m_almtime = 0;
// 					m_almstart = 0;
// 				}
// 			}

			if(0x01 == frm.reply) // �ɹ������Ӳ���绰ָ��
			{
				tagWcdmaPhoneTask objPhoneTask;
				memcpy( objPhoneTask.m_szDialTel, pfrm->num, min(sizeof(objPhoneTask) - 1, pfrm->len) );
				_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_DIAL ); // �˴�����Ϊ���û�����绰,���Ǽ����绰
			}
			else // ʧ��������Ӧ��
			{
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
		}
		break;


		//--------------------------------------------------
	case 0x14:	//ժ������
		{
			bool succ = WCDMA_PST_RCVRING == iPhoneHdlSta;
			//succ = succ && (!m_simlack);

			if(!succ)
			{
				Frm15 frm;
				frm.reply = 0x02;	//ʧ��
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			else
			{
				tagWcdmaPhoneTask objPhoneTask;
				_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_PICKUP );
			}
		}
		break;


		//--------------------------------------------------
	case 0x16:	//�һ�����
		{
			bool succ = (iPhoneHdlSta==WCDMA_PST_RCVRING) || (iPhoneHdlSta==WCDMA_PST_DIAL)
				|| (iPhoneHdlSta==WCDMA_PST_WAITPICK)|| (iPhoneHdlSta==WCDMA_PST_HOLD);
				
			if(!succ)
			{
				Frm17 frm;
				frm.reply = 0x02;	//ʧ��
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			else
			{
				PRTMSG(MSG_DBG,"User Hangup!\n");

				tagWcdmaPhoneTask objPhoneTask;
				_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_HANGUP );
			}
		}
		break;


		//--------------------------------------------------
	case 0x18:	//����ָʾӦ��
	case 0x20:	//��ָͨʾӦ��
	case 0x22:	//����ָʾӦ��
		break;


		//--------------------------------------------------
	case 0x24:	//�ź�ǿ������
		{
			tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
			int iSignal = objPhoneSta.m_iSignal;

			Frm25 frm;
			if(iSignal>0 && iSignal<MIN_GPRS_SIGNAL)			frm.level = 1;
			else if(iSignal>=MIN_GPRS_SIGNAL && iSignal<=15)	frm.level = 2;
			else if(iSignal>15 && iSignal<=25)	frm.level = 3;
			else if(iSignal>25 && iSignal<=31)	frm.level = 4;
			else if(iSignal>=100 && iSignal<=199)	frm.level = 5;
			else									frm.level = 0;
			g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
		}
		break;


		//--------------------------------------------------
	case 0x26:	//��������л�����
		{
			bool succ = (iPhoneHdlSta==WCDMA_PST_HOLD);

			if(!succ)
			{
				Frm27 frm;
				frm.reply = 0x02;	//ʧ��
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			else
			{
				tagWcdmaPhoneTask objPhoneTask;
				_PhoneTaskSet(objPhoneTask, WCDMA_PHTASK_CHGAUDIO);
			}
		}
		break;


		//--------------------------------------------------
	case 0x28:	//������������  ���ڷ���(1)  00H���� 01H����
		{
			bool succ = (iPhoneHdlSta==WCDMA_PST_HOLD || iPhoneHdlSta==WCDMA_PST_IDLE);

			if(!succ)
			{
				Frm29 frm;
				frm.reply = 0x02;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			else
			{
				tagWcdmaPhoneTask objPhoneTask;
				Frm28 *pfrm = (Frm28*)cmd;
				objPhoneTask.m_iChgVol = (pfrm->dir==0x00) ? (1) : (-1);	// ���� : ����
				_PhoneTaskSet(objPhoneTask, WCDMA_PHTASK_CHGVOL);
			}
		}
		break;

#if USE_IRD == 0
	case 0x2a:	//ͨ��ģʽ��������
		{
			bool succ = (iPhoneHdlSta==WCDMA_PST_HOLD);

			if(succ)
			{
				tagWcdmaPhoneTask objPhoneTask;
				_PhoneTaskSet(objPhoneTask, WCDMA_PHTASK_CHGAUDIO);
			}

// 			tag2QHstCfg obj2QHstCfg;
// 			GetSecCfg(&obj2QHstCfg,sizeof(obj2QHstCfg), 
// 				offsetof(tagSecondCfg, m_uni2QHstCfg.m_obj2QHstCfg), sizeof(obj2QHstCfg) );
// 			m_telmode = obj2QHstCfg.m_HandsfreeChannel;
// 			m_telmodebk = obj2QHstCfg.m_HandsfreeChannel;
// 			if (!m_telmode) {	//����
// 				// ��������ģʽ
// 				if(!set_audio_mode(1))		{ PRTMSG(MSG_ERR,"set_audio_mode(1) Fail!"); }
// 				else	{	PRTMSG(MSG_DBG,"set_audio_mode(1) Succ!");	}
// 			} else {	//����
// 				// ��������ģʽ
// 				if(!set_audio_mode(0))		{ PRTMSG(MSG_ERR,"set_audio_mode(0) Fail!"); }
// 				else	{	PRTMSG(MSG_DBG,"set_audio_mode(0) Succ!");	}
// 			}
		}
		break;
#endif

		//--------------------------------------------------
	case 0x30:	//���Ͷ���Ϣ����  �����ֻ��ų���(1)+ �����ֻ���(n)+����Ϣ���ݳ���(1)+����Ϣ����(m)
		{
			PRTMSG(MSG_DBG,"User req send sms\n");

			if( true ) //WCDMA_PST_IDLE == iPhoneHdlSta )
			{
				int p = 1;
				byte tellen = cmd[p];		p++;
				if(tellen>15)	break;
				char tel[16] = {0};
				memcpy(tel, cmd+p, tellen);	p+=tellen;
				
				byte msglen = cmd[p];		p++;
				if(msglen>160)	break;
				char msg[160] = {0};
				memcpy(msg, cmd+p, msglen);	p+=msglen;

				
				int iResult = _PushSndSms(tel, tellen, msg, msglen, INNERSYMB_PHONETODIAO);
				if( 1 != iResult )
				{
					Frm31 frm;
					frm.reply = 2;
					g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
				}
			}
			else
			{
				PRTMSG(MSG_DBG,"Not In Idle Sta,Refuse Sending SMS.");

				Frm31 frm;
				frm.reply = 2;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
		}
		break;


		//--------------------------------------------------
	case 0x32:	//���ն���ϢָʾӦ��	Ӧ�����ͣ�01H���ͳɹ� 02H����ʧ��
		{
		}
		break;


		//--------------------------------------------------
	case 0x34:	//�޸���������
		{
			// ��������״̬�������޸�����
			if( WCDMA_PST_IDLE==iPhoneHdlSta)
			{
				PRTMSG(MSG_DBG,"Rcv %s's net wick inform!\n", (0x01==cmd[1]) ? "Liu" : "Qian" );

				tagWcdmaPhoneTask objPhoneTask;
				if(0x01==cmd[1])	objPhoneTask.m_bytPingSrc |= 0x02;
				else				objPhoneTask.m_bytPingSrc |= 0x01;
//				if(!m_simlack)		m_gprsdail = true;

				_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_PING );
			}
		}
		break;

	case 0x36: //���ö������ĺ�
		{
			tagWcdmaPhoneTask objPhoneTask;
			_PhoneTaskSet(objPhoneTask, WCDMA_PHTASK_SETSMSCENT);
		}
		break;


		//--------------------------------------------------
	case 0x38:	//�����ķ��Ͷ�������
		{
			PRTMSG(MSG_DBG,"Other Program req send sms!");
			
			Frm38 *pfrm = (Frm38*)cmd;
			int iResult = _PushSndSms(pfrm->tel, strlen(pfrm->tel), pfrm->msg, strlen(pfrm->msg), 
				0x02 == pfrm->source ? INNERSYMB_PHONETOLIU : INNERSYMB_PHONETOQIAN);

			if( 1 != iResult )
			{
				Frm39 frm;
				frm.reply = 2;
					
				if(2 == pfrm->source)	
					DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_UPDATE, 2);
				else		
					DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);
			}
		}
		break;

		//--------------------------------------------------
	case 0xF1:	//��������绰����
		{
			PRTMSG(MSG_DBG,"Rcv req(dail listen)!\n");

			FrmF1 *pfrm = (FrmF1*)cmd;

			//������������
			bool succ = (pfrm->len!=0);
			if(!succ)
			{
				PRTMSG(MSG_ERR,"listen number is err!\n");

				FrmF2 frm;
				frm.reply = 0x04;	//����ʱ(����QianExe���޴�����)
				DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);
				break;
			}
			
			//���ڼ�����...
			succ = (WCDMA_PST_LSNING == iPhoneHdlSta);
			if(succ)
			{
				PRTMSG(MSG_DBG,"Now is listening...\n");

				FrmF2 frm;
				frm.reply = 0x02;	//���ڼ�����
				DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);
				break;
			}

			//ϵͳæ(�����ǳ�ʱ����) �����ڲ�������绰��
			succ =	(iPhoneHdlSta==WCDMA_PST_INIT) || (iPhoneHdlSta==WCDMA_PST_RESET) || (iPhoneHdlSta==WCDMA_PST_DIALNET)
					|| (iPhoneHdlSta==WCDMA_PST_HANGUP) || (iPhoneHdlSta==WCDMA_PST_DIALLSN) || (iPhoneHdlSta==WCDMA_PST_WAITLSNPICK);
			if(succ)
			{
				PRTMSG(MSG_DBG,"The line is busy!\n");

				FrmF2 frm;
				frm.reply = 0x06;	//��̨æ
				DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);
				break;
			}

			//����״̬(��������,����,ͨ��,�����)
			tagWcdmaPhoneTask objPhoneTask;
			memcpy( objPhoneTask.m_szDialLsnTel, pfrm->num, min(sizeof(objPhoneTask.m_szDialLsnTel) - 1, pfrm->len) );
			if( WCDMA_PST_IDLE == iPhoneHdlSta ) // ����״̬��
			{
				_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_DIALSN );
			}
			else // ����/ͨ��/����״̬
			{
				_PhoneTaskSet( objPhoneTask, WCDMA_PHTASK_HANGUP | WCDMA_PHTASK_DIALSN ); // ����2��ָ��,ʵ��ִ��ʱ�����ȹһ��ٲ����������
			}
		}
		break;

		//--------------------------------------------------
	case 0xF3:	//���뱨��״̬(������������ĵ绰���Զ��ҶϷ���������)
		{
			tagWcdmaPhoneTask objPhotoTask;
			objPhotoTask.m_dwAlertPrid = DWORD(cmd[1])*1000; // 0��ʾ�˳�����
			_PhoneTaskSet(objPhotoTask, WCDMA_PHTASK_ALERM);

// 			tagWcdmaPhoneStaCommon objPhoneStaComm;
// 			objPhoneStaComm.m_dwAlertPrid = DWORD(cmd[1])*1000;
// 			if (0 != objPhoneStaComm.m_dwAlertPrid)
// 			{
// 				objPhoneStaComm.m_dwAlertBegin = GetTickCount();
// 				PRTMSG(MSG_DBG,"Enter alarm state(%d s)!",DWORD(cmd[1]));
// 			}
// 			else
// 			{
// 				objPhoneStaComm.m_dwAlertBegin = 0;
// 				PRTMSG(MSG_DBG,"Exit alarm state!");
// 			}
// 			_PhoneStaCommonSet(objPhoneStaComm, WCDMA_PHSTVAR_ALERT);
		}
		break;

// 	case 0xf9:
// 		{
// 			m_objWcdmaCltCyc.i00to10SampCycle = cmd[1];// ACC OFF ��������,��
// 			m_objWcdmaCltCyc.i10to45SampCycle = cmd[2];// ACC OFF ��������,��
// 			m_objWcdmaCltCyc.i45to80SampCycle = cmd[3];// ACC OFF ��������,��
// 			m_objWcdmaCltCyc.i80toupSampCycle = cmd[4];
// 			PRTMSG(MSG_DBG,"CltPar=%d-%d-%d-%d",cmd[1],cmd[2],cmd[3],cmd[4]);
// 		}
// 		break;
		
	default:
		break;
	}

	return 0;
}

int CPhoneWcdma::_PhoneHdl()
{
	pthread_mutex_lock( &m_crisecDog );
	m_dwClrDogTm = GetTickCount();
	pthread_mutex_unlock( &m_crisecDog );
	
	DWORD dwSleep = 50000;

	switch( m_iPhoneHdlSta )
	{
		case WCDMA_PST_INIT:
			{
#if VEHICLE_TYPE == VEHICLE_M
				g_objLightCtrl.SetLightSta(0x02);	// ����
#endif
				RenewMemInfo(0x02, 0x02, 0x01, 0x05);
				
				int iResult = _PhoneStaHdl_Init();
				if( !iResult) //�ɹ�
				{
					m_iPhoneHdlSta = WCDMA_PST_IDLE;

#if CHK_VER == 1
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
					g_objLightCtrl2.SetLightSta(0x01);	// �������汾��ʼ���ɹ������̵�
#endif
#endif
				}
				else
				{
					G_RecordDebugInfo("wcdma init failed, phone reset!");
					m_iPhoneHdlSta = WCDMA_PST_RESET;
				}
			}
			break;
	
			//-----------------------------------------------------------------
		case WCDMA_PST_IDLE:	//�绰������...
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_Idle( iNextSta );
				m_iPhoneHdlSta = iNextSta;
			}
			break;
	
		case WCDMA_PST_DIALLSN:	//��������绰
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_DialLsn( iNextSta );
				m_iPhoneHdlSta = iNextSta;
			}
			break;
	
		case WCDMA_PST_WAITLSNPICK:	//�ȴ������绰����
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_WaitLsnPick( iNextSta );
				m_iPhoneHdlSta = iNextSta;
			}
			break;
	
		case WCDMA_PST_LSNING:	//������..
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_Lsning( iNextSta );
				m_iPhoneHdlSta = iNextSta;
	
				if( WCDMA_PST_LSNING == iNextSta )
				{
					// ����Ƿ�Ҫ���Ͷ���
					_PhoneJugSmsSnd();
				}
			}
			break;
	
		case WCDMA_PST_DIAL:	//����绰
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_Dial( iNextSta );
				m_iPhoneHdlSta = iNextSta;
			}
			break;
	
		case WCDMA_PST_WAITPICK:	//�ȴ�ժ��
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_WaitPick( iNextSta );
				m_iPhoneHdlSta = iNextSta;
			}
			break;
	
		case WCDMA_PST_HOLD:	//ͨ����
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_Hold( iNextSta );
				m_iPhoneHdlSta = iNextSta;
	
				if( WCDMA_PST_HOLD == iNextSta ) // ����Ȼ��ͨ��״̬��
				{
					// ����Ƿ�Ҫ���Ͷ���
					_PhoneJugSmsSnd();
				}
			}
			break;
	
		case WCDMA_PST_RCVRING:	//�յ�����..
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_RcvRing( iNextSta );
				if( iNextSta != m_iPhoneHdlSta )
				{
					// ����Ring������ܲ��ϲ���,����Ҫ�ദ�ط����
					_PhoneTaskClr( WCDMA_PHTASK_RING );
				}
				m_iPhoneHdlSta = iNextSta;
			}
			break;
	
		case WCDMA_PST_HANGUP:
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_Hangup( iNextSta );
				m_iPhoneHdlSta = iNextSta;
	
				// ����Ring������ܲ��ϲ���,����Ҫ�ദ�ط����
				_PhoneTaskClr( WCDMA_PHTASK_RING );
			}
			break;
	
		case WCDMA_PST_RESET:
			_PhoneStaHdl_Reset();
			m_iPhoneHdlSta = WCDMA_PST_INIT;
	
			// ����Ring������ܲ��ϲ���,����Ҫ�ദ�ط����
			_PhoneTaskClr( WCDMA_PHTASK_RING );
			break;
	
		case WCDMA_PST_DIALNET:
		{
			int iNextSta = m_iPhoneHdlSta;
			_PhoneStaHdl_DialNet( iNextSta );
			m_iPhoneHdlSta = iNextSta;
		}
		break;
		
		case WCDMA_PST_WAITDIALNET:
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_WaitDialNet( iNextSta );
				m_iPhoneHdlSta = iNextSta;
			}
		break;
	
		case WCDMA_PST_CUTNET:
			{
				int iNextSta = m_iPhoneHdlSta;
				_PhoneStaHdl_CutNet( iNextSta );
				m_iPhoneHdlSta = iNextSta;
			}
			break;
	
		default:
			PRTMSG(MSG_ERR, "Phone Sta Unknown! Reset Phone!\n" );
			m_iPhoneHdlSta = WCDMA_PST_RESET;
	}

	// �ж��Ƿ�Ҫ����/�˳�����״̬
	//que ��ʵ�ʲ���
	_PhoneJugAlerm();

// 	// �ж��Ƿ�Ҫ����"ping�������"
// 	_PhoneJugSmsPingRpt();

	// �ж��Ƿ�Ҫ���븴λ״̬ //que
	if( _PhoneJugReset() ) m_iPhoneHdlSta = WCDMA_PST_RESET;

	if( dwSleep ) usleep(dwSleep);

	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_Init()
{
	PRTMSG(MSG_DBG,"stInit\n");
	G_RecordDebugInfo("PhoneWcdma stInit");
	
	static int sta_iSetAtFail = 0;

	char szResult[2048] = { 0 };
	int iResult = 0;
	char cIndicate = 0;

	// �����Ӧ�е�,��û�д����ֵ,���ѹ�ʱ������ָ��
	_PhoneTaskClr( WCDMA_PHTASK_CHGAUDIO | WCDMA_PHTASK_CHGVOL | WCDMA_PHTASK_CUTNET | WCDMA_PHTASK_DIAL | WCDMA_PHTASK_DIALSN
		           | WCDMA_PHTASK_HANGUP | WCDMA_PHTASK_RING | WCDMA_PHTASK_SNDDTMF | WCDMA_PHTASK_RESET );

	// ״̬���е�һ״̬���еĻ�������ֵͳһ��� (ע�ⲻҪ��memset,Ҫ����Init)
	m_objPhoneStaDial.Init();
	m_objPhoneStaDialLsn.Init();
	m_objPhoneStaHold.Init();
	m_objPhoneStaLsning.Init();
	m_objPhoneStaRing.Init();

	// ͨ�û��������п�������Ľ������
	_PhoneStaCommonClr( WCDMA_PHSTVAR_SING | WCDMA_PHSTVAR_NET );

	// ѡ��ͨ���ر�ģʽ
	_PhoneAudioModeSet( WCDMA_AUDIOMODE_CLOSE );

	// ��usbserial���ھ��Ϊ-1��˵���ѹر�,���´򿪴���
	if( -1 == m_iPhonePort )
	{
		iResult = _InitCom();
		if( false == iResult )
		{
			RenewMemInfo(0x02, 0x00, 0x02, 0x00);
			goto _INITPHONE_FAIL;
		}
	}

	// AT����
	if( WCDMA_ATANS_OK != _SetAT(500, 5, NULL, 0, NULL, 0, "AT") )
	{
		PRTMSG(MSG_NOR,"set at fail!\n");
		G_RecordDebugInfo("PhoneWcdma set at fail!");

		sta_iSetAtFail++;
		if( sta_iSetAtFail >= 3 )
			G_ResetSystem();

		RenewMemInfo(0x02, 0x00, 0x02, 0x01);
		goto _INITPHONE_FAIL;
	}
	else
	{
		sta_iSetAtFail = 0;
	}

	// ��ֹ����
	if( WCDMA_ATANS_OK != _SetAT(500, 5, NULL, 0, NULL, 0, "ATE0") )
	{
		PRTMSG(MSG_NOR,"set ATE0 fail!\n");
		RenewMemInfo(0x02, 0x00, 0x02, 0x02);
		goto _INITPHONE_FAIL;
	}

//	_SetAT(3000, 3, NULL, 0, NULL, 0, "AT+CTA=0");

	// ��ѯIMEI��,AT+CGSN
//	{
//		static bool sta_bGetIMEI = false;
//		if( !sta_bGetIMEI )
//		{
//			tagWcdmaPhoneStaCommon objSta;
//			if( WCDMA_ATANS_OK == _SetAT(1000, 5, objSta.m_szGsmIMEI, sizeof(objSta.m_szGsmIMEI), G_WCDMA_GSN_Jugres, 0, "AT+CGSN") )
//			{
//				objSta.m_szGsmIMEI[ sizeof(objSta.m_szGsmIMEI) - 1 ] = 0;
//				_PhoneStaCommonSet( objSta, WCDMA_PHSTVAR_IMEI );
				
//				sta_bGetIMEI = true;
//			}
//		}
//	}

	sleep(5);
	iResult = _SetAT(3000, 5, NULL, 0, G_WCDMA_CPIN_Jugres, 0, "AT+CPIN?");
	if( WCDMA_ATANS_OK != iResult )	
	{
		RenewMemInfo(0x02, 0x00, 0x02, 0x03);
		G_RecordDebugInfo("PhoneWcdma AT+CPIN? failed!\n");
		goto _INITPHONE_FAIL;
	}

	// Set SMS Service
	_SetAT(500, 5, NULL, 0, NULL, 0, "AT+CSMS=0");
	
	// ���ö��ŵ�PDUģʽ
	_SetAT(600, 5, NULL, 0, NULL, 0, "AT+CMGF=0");
	
	// ���ö��ŵ����ֱ�Ӵ�ģ���ͳ���
	_SetAT(600, 5, NULL, 0, NULL, 0, "AT+CNMI=1,2,0,0,0");
	
	// ���Ŵ洢������
	sleep(5); // AT+CPMS����ǰҪ��Ϣ�£��űȽ����׳ɹ�
 	_SetAT(4000, 2, NULL, 0, NULL, 0, "AT+CPMS=\"ME\",\"ME\",\"ME\"");

// 	//���ö���״̬���棬�������ȼ������뷽ʽ����6ΪUNICODE���룩�����ܼ���
// 	_SetAT(1000, 5, NULL, 0, NULL, 0, "AT^HSMSSS=0,0,6,0");
	
	// �洢������,WCDMA֧�� testc
//	_SetAT(3000, 2, NULL, 0, NULL, 0, "AT+CPMS=\"ME\",\"ME\",\"ME\"");

	// ���ö��ŵ�PDU/textģʽ��WCDMAĿǰ��֧��textģʽ
//	_SetAT(600, 5, NULL, 0, NULL, 0, "AT+CMGF=1");
 
//	_SetAT(600, 5, NULL, 0, NULL, 0, "AT+CNMI?");
//	_SetAT(600, 5, NULL, 0, NULL, 0, "AT+CNMI=1,3,0,0,0");

	//AT^PPPCFG?	��ѯ�����û�������,���ڷǶ�ʧ��������
//	_SetAT(3000, 2, NULL, 0, NULL, 0, "AT^PPPCFG=\"card\",\"card\"");
//	iResult = _SetAT(3000, 2, NULL, 0, G_WCDMA_CPPPCFG_Jugres, 0, "AT^PPPCFG?");
//	if( WCDMA_ATANS_OK != iResult )
//	{
//		_SetAT(3000, 2, NULL, 0, NULL, 0, "AT^PPPCFG=\"CARD\",\"CARD\"");
//		PRTMSG(MSG_DBG, "set ppp dial user name:card,password:card\n");
//	}

	//�����й���
	iResult = _SetAT(100000, 2, NULL, 0, NULL, 0, "AT+CFUN=1");
	if( WCDMA_ATANS_OK != iResult )
	{
		PRTMSG(MSG_DBG, "set AT+CFUN=1 failed\n");
		RenewMemInfo(0x02, 0x00, 0x02, 0x04);
		goto _INITPHONE_FAIL;
	}

	if (PHONE_MODTYPE == PHONE_MODTYPE_SIM5218)
	{
		//ѡ������ģʽ:0Ϊ�Զ�ģʽ
		iResult = _SetAT(60000, 2, NULL, 0, NULL, 0, "AT+COPS=0");
		if( WCDMA_ATANS_OK != iResult )
		{	
			PRTMSG(MSG_DBG, "set AT+COPS=0 failed\n");
			RenewMemInfo(0x02, 0x00, 0x02, 0x05);
			goto _INITPHONE_FAIL;
		}
		
		//�Զ�ѡ������
		iResult = _SetAT(60000, 2, NULL, 0, NULL, 0, "AT+CNMP=2");
		if( WCDMA_ATANS_OK != iResult )
		{	
			PRTMSG(MSG_DBG, "set AT+CNMP=2 failed\n");
			RenewMemInfo(0x02, 0x00, 0x02, 0x06);
			goto _INITPHONE_FAIL;
		}
		
		//ʹ���κ�Ƶ��
		iResult = _SetAT(60000, 2, NULL, 0, NULL, 0, "AT+CNBP=0xFFFFFFFF7FFFFFFF");
		if( WCDMA_ATANS_OK != iResult )
		{	
			PRTMSG(MSG_DBG, "set AT+CNBP=0xFFFFFFFF7FFFFFFF failed\n");
			RenewMemInfo(0x02, 0x00, 0x02, 0x07);
			goto _INITPHONE_FAIL;
		}
	}
	else if (PHONE_MODTYPE == PHONE_MODTYPE_EM770W )
	{
		//ѡ������ģʽ����������Ϊ:2,ϵͳģʽ�ο��Զ�ѡ��;2,����������ο�,��WCDMA��GSM;3FFFFFFF,ѡ���κ�Ƶ��;1,֧������;2,������,CS_PS;
		_SetAT(600, 5, NULL, 0, NULL, 0, "AT%SYSCFG=2,2,3FFFFFFF,1,2");
	}
	else if (PHONE_MODTYPE == PHONE_MODTYPE_MU203)
	{
		_SetAT(600, 5, NULL, 0, NULL, 0, "AT^SYSCFG=2,2,3FFFFFFF,1,2");
	}
	
	//ע������
	if( WCDMA_ATANS_OK != _SetAT(600, 5, NULL, 0, NULL, 0, "AT+CREG=1") )
	{
		PRTMSG(MSG_DBG, "set AT+CREG=1 failed\n");
		G_RecordDebugInfo("PhoneWcdma set AT+CREG=1 failed");
		RenewMemInfo(0x02, 0x00, 0x02, 0x08);
		goto _INITPHONE_FAIL;
	}
	
	sleep(1);

	// ��ȡ�ź�
	memset( szResult, 0, sizeof(szResult) );
	if( WCDMA_ATANS_OK == _SetAT(3000, 2, szResult, sizeof(szResult), G_WCDMA_CSQ_Jugres, 0, "AT+CSQ") )
	{
		tagWcdmaPhoneStaCommon objSta;
		objSta.m_iSignal = atoi( szResult );
		G_RecordDebugInfo("PhoneWcdma first Signal=%d", objSta.m_iSignal);
		_PhoneStaCommonSet( objSta, WCDMA_PHSTVAR_SING );
	}

	PRTMSG(MSG_NOR,"wcdma init succ!\n");
	G_RecordDebugInfo("wcdma init succ!");

	return 0;

_INITPHONE_FAIL:
	return -1;
}

int CPhoneWcdma::_PhoneStaHdl_Idle( int& v_iNextSta )
{
//	PRTMSG(MSG_DBG,"stIdle\n");

	int iRet = 0;
	tagWcdmaPhoneTask objPhoneTask;
	tagWcdmaPhoneStaCommon objPhoneSta;
	static int iGetCSQFailed = 0;
	DWORD dwPktNum;

	// �����Ӧ�е�,��û�д����ֵ,���ѹ�ʱ������ָ��
	_PhoneTaskClr( WCDMA_PHTASK_CHGAUDIO | WCDMA_PHTASK_CHGVOL | WCDMA_PHTASK_HANGUP | WCDMA_PHTASK_SNDDTMF );

	// ����Ƿ���Ҫ�Ͽ�����
	if( _PhoneJugCutNet() )
	{
		v_iNextSta = WCDMA_PST_CUTNET;
		goto _PHONEIDLE_END;
	}

	//����Ƿ�������
	objPhoneTask = _PhoneTaskGet(WCDMA_PHTASK_RING);
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_RING )
	{
		// ����ring״̬ǰ�ȳ�ʼ�����µĻ�������
		m_objPhoneStaRing.Init();

		v_iNextSta = WCDMA_PST_RCVRING;
		goto _PHONEIDLE_END;
	}

	//����Ƿ��м����绰��Ҫ����
	objPhoneTask = _PhoneTaskGet(WCDMA_PHTASK_DIALSN);
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_DIALSN )
	{
		// ������һ״̬ǰ������һ״̬���еĳ�ʼ��������
		memcpy( m_objPhoneStaDialLsn.m_szDialLsnTel, objPhoneTask.m_szDialLsnTel,
			min(sizeof(m_objPhoneStaDialLsn.m_szDialLsnTel), sizeof(objPhoneTask.m_szDialLsnTel)) );
		m_objPhoneStaDialLsn.m_szDialLsnTel[ sizeof(m_objPhoneStaDialLsn.m_szDialLsnTel) - 1 ] = 0;
		m_objPhoneStaDialLsn.m_dwDialBeginTm = GetTickCount();
		
		v_iNextSta = WCDMA_PST_DIALLSN;
		goto _PHONEIDLE_END;
	}

	//����Ƿ��в���绰����
	objPhoneTask = _PhoneTaskGet(WCDMA_PHTASK_DIAL);
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_DIAL )
	{
		// ������һ״̬ǰ������һ״̬���еĳ�ʼ��������
		memcpy( m_objPhoneStaDial.m_szDialTel, objPhoneTask.m_szDialTel,
			min(sizeof(m_objPhoneStaDial.m_szDialTel), sizeof(objPhoneTask.m_szDialTel)) );
		m_objPhoneStaDial.m_szDialTel[ sizeof(m_objPhoneStaDial.m_szDialTel) - 1 ] = 0;
		m_objPhoneStaDial.m_dwDialBeginTm = GetTickCount();
		m_objPhoneStaDial.m_dwHoldLmtPrid = objPhoneTask.m_dwHoldLmtPrid;

		v_iNextSta = WCDMA_PST_DIAL;
		goto _PHONEIDLE_END;
	}

	// que ����Ƿ���Ҫ���ö������ĺ�
//	_PhoneJugSmsCentSet();
	
	// ����Ƿ�Ҫ���Ͷ���
	_PhoneJugSmsSnd();

// 	//����ʱ���1����ж��Ž��ռ��,Ŀǰֻ�ڿ���״̬�½��ն���
// 	static DWORD tmMsg = GetTickCount();
// 	if( GetTickCount()-tmMsg>1*1000 )
// 	{
// 		tmMsg = GetTickCount();
// 		// ����Ƿ��н��ն���
// 		_PhoneJugSmsRcv();
// 	}

	// ping�����ping�Ľ��
	objPhoneTask = _PhoneTaskGet(WCDMA_PHTASK_PING);
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_PING )
	{		
		v_iNextSta = WCDMA_PST_CUTNET;
		goto _PHONEIDLE_END;
	}

	//���ӻ�վ�ɼ�
// 	static unsigned int baseQuerytime = 0;
// 	if(baseQuerytime==0)	baseQuerytime = GetTickCount();
// 	if((GetTickCount()-baseQuerytime) > m_ucCurCltCyc*1000) 
// 	{
// 		baseQuerytime = GetTickCount();
// 		P_QueryWcdmaBase();
// 	}


	//����ʱ���5��(�����źż��ȵ�)
	{
		static DWORD sta_dwLst = GetTickCount();
		if( GetTickCount() - sta_dwLst > 5000 ) 
		{
			sta_dwLst = GetTickCount();

			//_SetAT(1000, 3, NULL, 0, NULL, 0, "AT^SYSINFO");//״̬��ѯ
			//_SetAT(1000, 3, NULL, 0, NULL, 0, "AT^HDRCSQ");//wcdma�źŸ���

			// ��ѯ��ǰ��������������
			_SetAT(1000, 1, NULL, 0, G_WCDMA_CNSMOD_Jugres, 0, "AT+CNSMOD?");

			//��ʱ�����������Ϳ���ָʾ
			Frm23 frm;
			g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);

// 			//��ʱѡ��GSM�����PC��Ƶ������رչ���
// 			_PhoneAudioModeSet( WCDMA_AUDIOMODE_CLOSE ); // ѡ��ͨ���ر�ģʽ

			objPhoneSta = _PhoneStaCommonGet(0);

			char szResult[50] = { 0 };
			int iSignal = 0;
			if( WCDMA_ATANS_OK == _SetAT(2000, 3, szResult, sizeof(szResult), G_WCDMA_CSQ_Jugres, 0, "AT+CSQ") )
			{
				iGetCSQFailed = 0;

				iSignal = atoi( szResult );
				objPhoneSta.m_iSignal = iSignal;
				_PhoneStaCommonSet(objPhoneSta, WCDMA_PHSTVAR_SING);

				//PRTMSG(MSG_DBG,"Signal:%d\n", iSignal);
				G_RecordDebugInfo("PhoneWcdma Signal:%d", iSignal);

				//����Ƿ�ʱ�����ź�
				static DWORD sta_dwLstNoWeakTm = GetTickCount();
				static DWORD sta_dwContWeakTimes = 0; // ����������źŴ���
				if( iSignal > 0 && iSignal < MIN_GPRS_SIGNAL ) 
				{
					PRTMSG(MSG_DBG,"Low signal(%d)!", iSignal);
					sta_dwContWeakTimes ++;

					if( GetTickCount() - sta_dwLstNoWeakTm >= 180000 && sta_dwContWeakTimes > 1 )
					{ //��ʱ�����źţ���ø�λһ��ģ��(3����)
						PRTMSG(MSG_DBG,"Low signal(long time),reset gsm!");

						sta_dwContWeakTimes = 0;
						sta_dwLstNoWeakTm = GetTickCount();
						v_iNextSta = WCDMA_PST_RESET;

						goto _PHONEIDLE_END;
					}
				}
				else
				{
					sta_dwContWeakTimes = 0;
					sta_dwLstNoWeakTm = GetTickCount();
				}

				//����Ƿ�ʱ�����ź�
				static DWORD sta_dwLstSingNormTm = GetTickCount();
				static DWORD sta_dwContNoSingTimes = 0; // ����������źŴ���
				if(0 == iSignal || 99 == iSignal || -1 == iSignal) 
				{
					PRTMSG(MSG_DBG,"No signal(%d)!", iSignal);
					sta_dwContNoSingTimes ++;

					if( GetTickCount() - sta_dwLstSingNormTm >= 60000 && sta_dwContNoSingTimes > 1 )
					{ //��ʱ�����źţ���ø�λһ��ģ��(1����)
						PRTMSG(MSG_DBG,"Low signal(long time),reset gsm!");

						sta_dwContNoSingTimes = 0;
						sta_dwLstSingNormTm = GetTickCount();
						v_iNextSta = WCDMA_PST_RESET;

						goto _PHONEIDLE_END;
					}
				}
				else
				{
					sta_dwContNoSingTimes = 0;
					sta_dwLstSingNormTm = GetTickCount();
				}
			}
			else
			{
				iGetCSQFailed++;
				if( iGetCSQFailed > 2)
				{
#if CHK_VER == 0		
					//g_bProgExit = true;
					g_objPhoneWcdma.ResetPhone();					
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
					g_objDiaodu.show_diaodu("�ֻ�ģ���쳣");
					RenewMemInfo(0x02, 0x00, 0x00, 0x01);
#endif

					return false;
				}
			}
		}

//��������޸�˵��: ���汾������GPRS����
#if CHK_VER == 0
		//����Ƿ���Ҫ��������
		objPhoneSta = _PhoneStaCommonGet(0);
		if( !objPhoneSta.m_bNetConnected )
		{
			v_iNextSta = WCDMA_PST_DIALNET;
			goto _PHONEIDLE_END;
		}

#endif
	}

_PHONEIDLE_END:
	return iRet;
}

int CPhoneWcdma::_PhoneStaHdl_DialLsn( int& v_iNextSta )
{
	if( 0 == m_objPhoneStaDialLsn.m_szDialLsnTel[0] )
	{
		v_iNextSta = WCDMA_PST_IDLE;
		goto _PHONEDIALLSN_END;
	}

	PRTMSG(MSG_NOR,"stDialListen\n");
	PRTMSG(MSG_DBG,"Dial listen(%.255s)...", m_objPhoneStaDialLsn.m_szDialLsnTel);

	//����audioΪ����ģʽ
	_PhoneAudioModeSet(WCDMA_AUDIOMODE_LSN);
	
	//���Ͳ���ָ��
	if( WCDMA_ATANS_OK != _SetAT(4000, 5, NULL, 0, NULL, 0, "ATD%.20s;", m_objPhoneStaDialLsn.m_szDialLsnTel) )
	{
		PRTMSG(MSG_ERR,"Dial fail!\n");

		// ��������Ӧ��
		FrmF2 frm;
		frm.reply = 0x06;	// ��̨�ֻ�ģ��æ
		DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);
		
		v_iNextSta = WCDMA_PST_HANGUP;
		goto _PHONEDIALLSN_END;
	}
	else
	{
		// �˴������WAITLSNPICK״̬�µĻ���������ֱ��ʹ��DIALLSN״̬�µĻ�������
		v_iNextSta = WCDMA_PST_WAITLSNPICK;
		goto _PHONEDIALLSN_END;
	}

_PHONEDIALLSN_END:
	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_WaitLsnPick( int &v_iNextSta )
{
	PRTMSG(MSG_NOR,"stWaitLsnPick\n");

//CDMA��ժ����ʾ���˴���CLCC���ж�
	int iResult = _SetAT(1000, 3, NULL, 0, G_WCDMA_CLCC_JugRes, 0, "AT+CLCC");

	// ����ʹ��AT+CLCC������״̬ (��)

	switch( iResult )
	{
	case WCDMA_ATANS_OK:
		{
			PRTMSG(MSG_DBG,"Dial succ,enter listen!\n");

			// ��������Ӧ��
			FrmF2 frm;
			frm.reply = 0x01;	//�ɹ�	
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			m_objPhoneStaLsning.m_dwCallChkTm = GetTickCount();
			v_iNextSta = WCDMA_PST_LSNING;
		}
		break;

	case WCDMA_ATANS_ERROR:
		{
			PRTMSG(MSG_ERR,"Phone Err,hang up!\n");

			// ��������Ӧ��
			FrmF2 frm;
			frm.reply = 0x03;	//ռ�߻��豸æ	
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_NOCARRIER:
		{
			PRTMSG(MSG_ERR,"No Carrier,hang up!\n");

			// ��������Ӧ��
			FrmF2 frm;
			frm.reply = 0x03;	//ռ�߻��豸æ	
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_NODIALTONE:
		{
			PRTMSG(MSG_ERR,"No Dialtone,hang up!\n");

			// ��������Ӧ��
			FrmF2 frm;
			frm.reply = 0x03;	//ռ�߻��豸æ	
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_BUSY:
		{
			PRTMSG(MSG_ERR,"Phone busy,hang up!\n");

			// ��������Ӧ��
			FrmF2 frm;
			frm.reply = 0x03;	//ռ�߻��豸æ	
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_NOANSWER:
		{
			PRTMSG(MSG_ERR,"Phone No Answer,hang up!\n");

			// ��������Ӧ��
			FrmF2 frm;
			frm.reply = 0x04;	// ����ʱ����Ӧ��
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_NOANS:
	case WCDMA_ATANS_OTHER:
	default:
		if( GetTickCount() - m_objPhoneStaDialLsn.m_dwDialBeginTm > 90000 )
		{
			PRTMSG(MSG_ERR,"Dial Lsn,but wait pick time up,hang up\n");

			// ��������Ӧ��
			FrmF2 frm;
			frm.reply = 0x04;	// ����ʱ����Ӧ��
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);
			
			v_iNextSta = WCDMA_PST_HANGUP;
		}
	}

	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_Lsning(int& v_iNextSta)
{
	PRTMSG(MSG_NOR,"stListening\n");

	// �ж��Ƿ�Ҫ�����һ� (ԭ���Ǽ�⵽NO CARRIER,���Է��һ�)
	tagWcdmaPhoneTask objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_HANGUP );
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_HANGUP )
	{
		v_iNextSta = WCDMA_PST_HANGUP;

		FrmF2 frm;
		frm.reply = 0x05;	//��������	
		DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

		return 0;
	}

	int iResult = _ReadAT( 200, NULL, 0, G_WCDMA_JugRemoteHangup );

	switch( iResult )
	{
		case WCDMA_ATANS_OK:
		{
			PRTMSG(MSG_DBG,"the other side hang up\n");
			
			v_iNextSta = WCDMA_PST_HANGUP;
			
			FrmF2 frm;
			frm.reply = 0x05;	//��������	
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);
		}
		break;

		case WCDMA_ATANS_ERROR:
		case WCDMA_ATANS_NOCARRIER:

		default:
		{
			// ��һ��ʱ�����Ƿ�ͨ����...
			if( GetTickCount() - m_objPhoneStaLsning.m_dwCallChkTm > 10000 )
			{
				m_objPhoneStaLsning.m_dwCallChkTm = GetTickCount();
				if( WCDMA_ATANS_OK != _SetAT(1000, 3, NULL, 0, G_WCDMA_CLCC_JugRes, 0, "AT+CLCC") )
				{
					PRTMSG(MSG_ERR, "Lsning,but chk call fail,hang up\n" );
					v_iNextSta = WCDMA_PST_HANGUP;
				}
			}
		}
	}

	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_Dial( int &v_iNextSta )
{
	DWORD dwPktNum;
	PRTMSG(MSG_NOR,"stDial\n");

	// �ж��Ƿ��ڱ���״̬��
	tagWcdmaPhoneStaCommon objSta = _PhoneStaCommonGet(0);
	if( GetTickCount() - objSta.m_dwAlertBegin < objSta.m_dwAlertPrid )
	{
		PRTMSG(MSG_DBG,"Alert! Stop dial,hang up!\n");

		// ��������Ӧ��
		Frm13 frm;
		frm.reply = 0x02;	
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);	//�ֻ�æ
		
		v_iNextSta = WCDMA_PST_HANGUP;
		return 0;
	}

	if( 0 == m_objPhoneStaDial.m_szDialTel[0] )
	{
		v_iNextSta = WCDMA_PST_IDLE;
		return 0;
	}
		
	// ѡ�񲦺�ģʽ
	_PhoneAudioModeSet( WCDMA_AUDIOMODE_DIAL );
	
// 	// ���ò��ź�Ҫ�ȴ��Է�ժ����ŷ���+COLP��OK(������ʧЧ,����Ҫ��������)
// 	if(!set_at(5, 1, 0, "AT+COLP=1\r\n"))   show_msg("Set COLP Fail!");

	//���Ͳ���ָ��
	if( WCDMA_ATANS_OK != _SetAT(2000, 5, NULL, 0, NULL, 0, "ATD%.20s;", m_objPhoneStaDial.m_szDialTel) )
	{
		PRTMSG(MSG_ERR,"Dial fail!\n");

		// ��������Ӧ��
		Frm13 frm;
		frm.reply = 0x02;	//�ֻ���æ	
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
		
		v_iNextSta = WCDMA_PST_HANGUP;
		goto _PHONEDIAL_END;
	}
	else
	{
		// ��������Ӧ��
		Frm13 frm13;
		frm13.reply = 0x01;	//���ųɹ�
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm13), (char*)&frm13, dwPktNum);

		if(1==g_iTerminalType) //������ֱ�ģʽ
		{
			tag2QHstCfg obj2QHstCfg;
			GetSecCfg(&obj2QHstCfg,sizeof(obj2QHstCfg),
				offsetof(tagSecondCfg, m_uni2QHstCfg.m_obj2QHstCfg), sizeof(obj2QHstCfg) );

			// �ָ�Ĭ������
			if( obj2QHstCfg.m_HandsfreeChannel )
			{
				if(_PhoneAudioModeSet(WCDMA_AUDIOMODE_HANDFREE))
				{
					PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_HANDFREE);
				}
			}
			else
			{
				if(_PhoneAudioModeSet(WCDMA_AUDIOMODE_EARPHONE))
				{
					PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_EARPHONE);
				}
			}
		}
		else
		{
			// ����������Ƶģʽ
			if(_PhoneAudioModeSet(WCDMA_AUDIOMODE_HANDFREE))
			{
				PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_HANDFREE);
			}
		}

		// �˴������WAITPICK״̬�µĻ���������WAITPICK״ֱ̬��ʹ��DIAL״̬�µĻ�������
		v_iNextSta = WCDMA_PST_WAITPICK;
	}

_PHONEDIAL_END:
	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_WaitPick( int &v_iNextSta )
{
	DWORD dwPktNum;

	PRTMSG(MSG_NOR,"stWaitPick\n");

	// �ж��Ƿ��ڱ���״̬��
	tagWcdmaPhoneStaCommon objSta = _PhoneStaCommonGet(0);
	if( GetTickCount() - objSta.m_dwAlertBegin < objSta.m_dwAlertPrid )
	{
		PRTMSG(MSG_DBG,"Alert! Stop waiting pick,hang up!\n");

		// ��������Ӧ��
		Frm13 frm;
		frm.reply = 0x02;	
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);	//�ֻ�æ
		
		v_iNextSta = WCDMA_PST_HANGUP;
		return 0;
	}

	// �ж��Ƿ�Ҫ�����һ�
	tagWcdmaPhoneTask objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_HANGUP );
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_HANGUP )
	{
		v_iNextSta = WCDMA_PST_HANGUP;

		// �һ�����Ӧ��
		Frm17 frm;
		frm.reply = 1;
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);

		return 0;
	}

	char szResult[100] = { 0 };		//��CLCC���ж��Ƿ�Է�ժ��
	int iResult = _SetAT(1000, 3, NULL, 0, G_WCDMA_CLCC_JugRes, 0, "AT+CLCC");

	switch( iResult )
	{
	case WCDMA_ATANS_OK:
		{
			PRTMSG(MSG_DBG,"Dial succ,enter hold!\n");

			Frm21 frm21;		
			g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm21), (char*)&frm21, dwPktNum);	//��ָͨʾ

			if(1==g_iTerminalType) //������ֱ�ģʽ
			{
				tag2QHstCfg obj2QHstCfg;
				GetSecCfg(&obj2QHstCfg,sizeof(obj2QHstCfg),
					offsetof(tagSecondCfg, m_uni2QHstCfg.m_obj2QHstCfg), sizeof(obj2QHstCfg) );

				// �ָ�Ĭ������
				if( obj2QHstCfg.m_HandsfreeChannel )
				{
					if( _PhoneAudioModeSet(WCDMA_AUDIOMODE_HANDFREE) )
					{
						PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_HANDFREE);
					}
				}
				else
				{
					if( _PhoneAudioModeSet(WCDMA_AUDIOMODE_EARPHONE) )
					{
						PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_EARPHONE);
					}
				}
			}
			else
			{
				// ����������Ƶģʽ
				if( _PhoneAudioModeSet(WCDMA_AUDIOMODE_HANDFREE) )
				{
					PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_HANDFREE);
				}
			}

			// ��ʼ��Hold״̬�Ķ��еĻ�������
			m_objPhoneStaHold.m_dwHoldLmtPrid = m_objPhoneStaDial.m_dwHoldLmtPrid;
			m_objPhoneStaHold.m_dwHoldBeginTm = m_objPhoneStaHold.m_dwCallChkTm = GetTickCount();
			v_iNextSta = WCDMA_PST_HOLD;
		}
		break;

	case WCDMA_ATANS_ERROR:
		{
			PRTMSG(MSG_ERR,"Phone Err,hang up!\n");

			// ��������Ӧ��
			Frm13 frm;
			frm.reply = 0x02;	//�ֻ�æ	
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_NOCARRIER:
		{
			PRTMSG(MSG_ERR,"No Carrier,hang up!\n");

			// ��������Ӧ��
			Frm13 frm;
			frm.reply = 0x04; // δ����������
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_NODIALTONE:
		{
			PRTMSG(MSG_ERR,"No Dialtone,hang up!\n");

			// ��������Ӧ��
			Frm13 frm;
			frm.reply = 0x02;	//�ֻ�æ
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_BUSY:
		{
			PRTMSG(MSG_ERR,"Phone busy,hang up!\n");

			// ��������Ӧ��
			Frm13 frm;
			frm.reply = 0x02;	//�ֻ�æ
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_NOANSWER:
		{
			PRTMSG(MSG_ERR,"Phone No Answer,hang up!\n");

			// ��������Ӧ��
			Frm13 frm;
			frm.reply = 0x02;	//�ֻ�æ
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_NOANS:
	case WCDMA_ATANS_OTHER:
	default:
		if( GetTickCount() - m_objPhoneStaDial.m_dwDialBeginTm > 90000 ) // ����ʱ��Ӧ�ùҶ�
		{
			PRTMSG(MSG_ERR,"Dial,but wait pick time up,hang up\n");

			// ��������Ӧ��
			Frm13 frm;
			frm.reply = 0x02;	//�ֻ�æ
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);

			v_iNextSta = WCDMA_PST_HANGUP;
		}
	}

	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_Hold( int &v_iNextSta )
{
	PRTMSG(MSG_NOR,"stHold\n");

	char sCallId;
	int iResult = 0;
	tagWcdmaPhoneTask objPhoneTask;
	tagWcdmaPhoneStaCommon objSta;
	DWORD dwPktNum;

	// �ж��Ƿ��ڱ���״̬��
	objSta = _PhoneStaCommonGet(0);
	if( GetTickCount() - objSta.m_dwAlertBegin < objSta.m_dwAlertPrid )
	{
		PRTMSG(MSG_DBG,"Alert! Stop hold,hang up!\n");
		
		v_iNextSta = WCDMA_PST_HANGUP;
		goto _PHONEHDLHOLD_END;
	}

	// �ж��Ƿ�Ҫ�����һ�
	objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_HANGUP );
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_HANGUP )
	{
		v_iNextSta = WCDMA_PST_HANGUP;

		// �һ�����Ӧ��
		Frm17 frm;
		frm.reply = 1;
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);

		goto _PHONEHDLHOLD_END;
	}

	// ͨ��ʱ������
	if( m_objPhoneStaHold.m_dwHoldLmtPrid &&
		GetTickCount() - m_objPhoneStaHold.m_dwHoldBeginTm >= m_objPhoneStaHold.m_dwHoldLmtPrid )
	{
		PRTMSG(MSG_DBG,"Hold time is limit(%u),hang up!\n", m_objPhoneStaHold.m_dwHoldLmtPrid / 1000 );

		v_iNextSta = WCDMA_PST_HANGUP;
		goto _PHONEHDLHOLD_END;
	}
	
	// ��������л� (�ֱ�ģʽ��)
	objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_CHGAUDIO );
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_CHGAUDIO )
	{
		objSta = _PhoneStaCommonGet(0);

		if( WCDMA_AUDIOMODE_EARPHONE == objSta.m_bytCurAudioMode )
		{
			_PhoneAudioModeSet( WCDMA_AUDIOMODE_HANDFREE );
		}
		else
		{
			_PhoneAudioModeSet( WCDMA_AUDIOMODE_EARPHONE );
		}

		// ����/�����л�Ӧ��
		Frm27 frm;
		frm.reply = 0x01;
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
	}
	// ����DTMF
	objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_SNDDTMF );
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_SNDDTMF )
	{
		//��ѯ����ID��ͨ��AT+CLCC����ѯ��������sCallId��0~6ѭ��ȡֵ
		int iResult = _SetAT( 1000, 1, &sCallId, sizeof(sCallId), G_WCDMA_CLCCGetId, 0, "AT+CLCC" );
		if(WCDMA_ATANS_OK == iResult)
		{
			if('0' <= sCallId && '9' >= sCallId )
			//����dtmfָ��
			//AT^DTMF=<call_x>,<dtmf_digit>[,<on_length>[,<off_length>] ]
			//iResult = _SetAT( strlen(objPhoneTask.m_szDTMF) * 1000, 2, NULL, 0, NULL, 0, "AT^DTMF=%c,%s", sCallId,objPhoneTask.m_szDTMF );
			iResult = _SetAT( strlen(objPhoneTask.m_szDTMF) * 1000, 2, NULL, 0, NULL, 0, "AT+VTS=%s", objPhoneTask.m_szDTMF );
		}
		// DTMF��������Ӧ��
		Frm11 frm;
		frm.reply = WCDMA_ATANS_OK == iResult ? 0x01 : 0x02;
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
	}

	// ��������
	objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_CHGVOL );
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_CHGVOL )
	{
		// ���ߵ���audio scene ? (��)

		iResult = _PhoneAudioOutVolSet( objPhoneTask.m_iChgVol );

		// ������������Ӧ��
		Frm29 frm;
		frm.reply = (!iResult ? 0x01 : 0x02);
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);		
	}

	//CEND
	iResult = _ReadAT( 200, NULL, 0, G_WCDMA_JugRemoteHangup );

	switch( iResult )
	{
	case WCDMA_ATANS_OK:
		{
			PRTMSG(MSG_DBG,"the other side hang up\n");
			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	case WCDMA_ATANS_ERROR:
	case WCDMA_ATANS_NOCARRIER:
		{
			PRTMSG(MSG_ERR,"Holding,but no carrier or err,hang up\n");
			
			v_iNextSta = WCDMA_PST_HANGUP;
		}
		break;

	default:
		{
			// ��һ��ʱ�����Ƿ�ͨ����... (���Ŀ���Ƶ��һ��)
			if( GetTickCount() - m_objPhoneStaHold.m_dwCallChkTm > 10000 )
			{
				m_objPhoneStaHold.m_dwCallChkTm = GetTickCount();
				if( WCDMA_ATANS_OK != _SetAT(1000, 3, NULL, 0, G_WCDMA_CLCC_JugRes, 0, "AT+CLCC") )
				{
					PRTMSG(MSG_ERR,"Holding,but check call fail,hang up\n");
					v_iNextSta = WCDMA_PST_HANGUP;
				}
			}
		}
		;
	}

_PHONEHDLHOLD_END:
	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_RcvRing( int& v_iNextSta )
{
	PRTMSG(MSG_NOR,"stRcvRing\n");

	char szRingTel[23] = { 0 };
	byte type = 0;
	WORD wLmtPrid = 0;
	int iResult = 0;
	DWORD dwClipWait = 500;
	bool bToJugRingActive = true;
	DWORD dwPktNum;

	// ��ȡ�������
	if( 0 == m_objPhoneStaRing.m_szRingTel[0] ) // ��������뻹δ��ȡ��
	{
		dwClipWait = 1700;
	}
	iResult = _ReadAT( dwClipWait, szRingTel, sizeof(szRingTel), G_WCDMA_CLIP_ReadTel );
	if( WCDMA_ATANS_OK != iResult && WCDMA_ATANS_NOANS != iResult && WCDMA_ATANS_RCVNOTALL != iResult )
	{
		// �һ�
		v_iNextSta = WCDMA_PST_HANGUP;
		return 0;
	}
	// �ж��Ƿ��ȡ��ring
	if( WCDMA_ATANS_OK == iResult || WCDMA_ATANS_RCVNOTALL == iResult )
	{
		bToJugRingActive = false; // ��ȡ��������Ϣ����Ҫ�������״̬
	}

	if( 0 == m_objPhoneStaRing.m_szRingTel[0] ) // ��������뻹δ��ȡ��
	{
		// ������봦��
		if( 0 == szRingTel[0] ) // �����δ�clip��ȡ�������ʧ��
		{
			// ������ȡ�������
			_SetAT( 1800, 1, szRingTel, sizeof(szRingTel), G_WCDMA_CLCC_ReadTel, 0, "AT+CLCC" );
		}
		if( strlen(szRingTel) > 2 && strlen(szRingTel) < sizeof(szRingTel) )
		{ // ����ȡ�����������Ч

			type = tel_type(false, szRingTel, strlen(szRingTel), &wLmtPrid );
			type &= 0x0f;

			m_objPhoneStaRing.m_dwHoldLmtPrid = wLmtPrid;
			m_objPhoneStaRing.m_bytCallInType = type;
			memset( m_objPhoneStaRing.m_szRingTel, 0, sizeof(m_objPhoneStaRing.m_szRingTel) );
			memcpy( m_objPhoneStaRing.m_szRingTel, szRingTel, min(sizeof(m_objPhoneStaRing.m_szRingTel) - 1, strlen(szRingTel)) );

			bToJugRingActive = false; // ��ȡ�������������Ҫ�������״̬
		}
	}
	else
	{
		// ������뼰��صĺ������������Ϣʹ�û��������еĻ���ֵ (�����Idle״̬�жϳ���������ʱ��Щ����ֵ��Ҫ�����)

		type = m_objPhoneStaRing.m_bytCallInType;
		wLmtPrid = WORD(m_objPhoneStaRing.m_dwHoldLmtPrid);
		memset( szRingTel, 0, sizeof(szRingTel) );
		memcpy( szRingTel, m_objPhoneStaRing.m_szRingTel, min(sizeof(szRingTel) - 1, strlen(m_objPhoneStaRing.m_szRingTel)) );
	}
	// �ж��Ƿ�Ҫ�������״̬
	{
		DWORD dwCur = GetTickCount();
		if( !bToJugRingActive ) m_objPhoneStaRing.m_dwLstChkRingTm = dwCur; // ������Ҫ�������,�����"�ϴη���������Ϣʱ��"
		if( dwCur - m_objPhoneStaRing.m_dwLstChkRingTm >= 4000 ) // ������һ��ʱ��δ����������Ϣ
		{
			m_objPhoneStaRing.m_dwLstChkRingTm = dwCur;

			// ��������Ƿ�������
			iResult = _SetAT(1000, 2, NULL, 0, G_WCDMA_CLCCJugIncoming, 0, "AT+CLCC");
			if( WCDMA_ATANS_OK != iResult )
			{
				// �һ�
				v_iNextSta = WCDMA_PST_HANGUP;
				return 0;
			}
		}
	}

	PRTMSG(MSG_DBG,"Ring %.15s(%02x, %u)...\n", szRingTel, type, wLmtPrid);

	tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);

	// �������ͣ�0,������� 1,���������������� 2,��ֹ������� 3,���ķ���绰���� 4,UDP�������
	if(type==1) // ����������������
	{
		PRTMSG(MSG_DBG,"Ring(%.25s) listen...\n", szRingTel);

		if( WCDMA_ATANS_OK == _SetAT(1000, 3, NULL, 0, NULL, 0, "ATA") )
		{
			PRTMSG(MSG_DBG,"Auto pick up, Listening...!\n");
			if( 0 == _PhoneAudioModeSet(WCDMA_AUDIOMODE_LSN) )
			{
				objPhoneSta.m_bytCurAudioMode = WCDMA_AUDIOMODE_LSN;
				_PhoneStaCommonSet( objPhoneSta, WCDMA_PHSTVAR_AUDIOMODE );
			}
			
			// ��һ״̬����������ʼ��
			m_objPhoneStaLsning.m_dwCallChkTm = GetTickCount();

			v_iNextSta = WCDMA_PST_LSNING;
		}
		else
		{
			v_iNextSta = WCDMA_PST_HANGUP; // ����������ǹһ�һ�½Ϻ�
		}
	}
	else if(type==4) // UDP�������
	{
		PRTMSG(MSG_DBG,"UDP active(%.25s),hang up!\n", szRingTel);

		Frm35 frm;	//����ָ�֪ͨ֡
		frm.reply = 0x01;	//���ųɹ�
		DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_SOCK, 2);	//֪ͨǧ����(KJ2)UDP�ص�

		v_iNextSta = WCDMA_PST_HANGUP;
	}
	else if( GetTickCount()-objPhoneSta.m_dwAlertBegin < objPhoneSta.m_dwAlertPrid )
	{ // �ж��Ƿ��ڱ���״̬��(�Ǽ����绰��Ҷ�)
		PRTMSG(MSG_DBG,"In Alarm Period,hang up ring(%.25s)!\n", szRingTel);
		v_iNextSta = WCDMA_PST_HANGUP;
	}
	else if( 3 == type ) // ���ķ���绰����
	{
		PRTMSG(MSG_DBG,"Ring(%.25s) CentServ Tel..\n", szRingTel);

		Frm19 frm;
		frm.len = min(strlen(szRingTel), 25);
		memcpy(frm.num, szRingTel, frm.len);
		g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, 2+frm.len, (char*)&frm, dwPktNum);
	}
	else if(type==2) // ��ֹ�������
	{
		PRTMSG(MSG_DBG,"Ring(%.25s) forbid,hang up!\n", szRingTel);
		v_iNextSta = WCDMA_PST_HANGUP;
	}
	else
	{
		PRTMSG(MSG_DBG,"Ring(%.25s)...\n", szRingTel);
	}

	if( WCDMA_PST_RCVRING == v_iNextSta ) // ��ǰ����жϴ���δ�ı�״̬
	{
		tagWcdmaPhoneTask objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_HANGUP | WCDMA_PHTASK_PICKUP );
		if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_HANGUP ) // ��Ҫ�����Ҷ�
		{
			v_iNextSta = WCDMA_PST_HANGUP;

			// �һ�����Ӧ��
			Frm17 frm;
			frm.reply = 1;
			g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
		}
		else if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_PICKUP ) // ���û�Ҫ����
		{
			PRTMSG(MSG_DBG,"User Pick up\n");

			if( WCDMA_ATANS_OK == _SetAT(1000, 3, NULL, 0, NULL, 0, "ATA") )
			{
				PRTMSG(MSG_DBG,"Pick up Succ, Holding..!\n");

				if(1==g_iTerminalType) //������ֱ�ģʽ
				{
					tag2QHstCfg obj2QHstCfg;
					GetSecCfg(&obj2QHstCfg,sizeof(obj2QHstCfg),
						offsetof(tagSecondCfg, m_uni2QHstCfg.m_obj2QHstCfg), sizeof(obj2QHstCfg) );

					// �ָ�Ĭ������
					if( obj2QHstCfg.m_HandsfreeChannel )
					{
						if( _PhoneAudioModeSet(WCDMA_AUDIOMODE_HANDFREE) )
						{
							PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_HANDFREE);
						}
					}
					else
					{
						if( _PhoneAudioModeSet(WCDMA_AUDIOMODE_EARPHONE) )
						{
							PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_EARPHONE);
						}
					}
				}
				else
				{
					// ����������Ƶģʽ
					if( _PhoneAudioModeSet(WCDMA_AUDIOMODE_HANDFREE) )
					{
						PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_HANDFREE);
					}
				}
				
				// ��һ״̬����������ʼ��
				m_objPhoneStaHold.m_dwHoldBeginTm = m_objPhoneStaHold.m_dwCallChkTm = GetTickCount();
				m_objPhoneStaHold.m_dwHoldLmtPrid = m_objPhoneStaRing.m_dwHoldLmtPrid;

				v_iNextSta = WCDMA_PST_HOLD;

				// ժ������Ӧ��
				Frm15 frm;
				frm.reply = 1;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			else
			{
				PRTMSG(MSG_ERR,"Pick up Fail, Hang up\n");
				v_iNextSta = WCDMA_PST_HANGUP; // (Ϊ�������Ҳ���ǹһ�һ�½Ϻ�)

				// ժ������Ӧ��
				Frm15 frm;
				frm.reply = 2;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
		}
		else
		{
			// ��������ָʾ
			Frm19 frm;
			frm.len = min(strlen(szRingTel), 25);
			memcpy(frm.num, szRingTel, frm.len);
			g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, 2+frm.len, (char*)&frm, dwPktNum);
		}
	}

	return 0;
}


int CPhoneWcdma::_PhoneStaHdl_Hangup( int &v_iNextSta )
{
	PRTMSG(MSG_NOR,"stHangup\n");
	DWORD dwPktNum;

	// �����Ӧ�е�,��û�д����ֵ,���ѹ�ʱ������ָ��
	_PhoneTaskClr( WCDMA_PHTASK_CHGAUDIO | WCDMA_PHTASK_CHGVOL | WCDMA_PHTASK_HANGUP | WCDMA_PHTASK_RING | WCDMA_PHTASK_SNDDTMF );

	v_iNextSta = WCDMA_PST_IDLE; // Ԥ�ô�������״ֵ̬

	//que ATH�᲻��Ҷϲ���NET��
	int iResult = _SetAT(15000, 3, NULL, 0, NULL, 0, "AT+CHUP"); // ���Ҷ�����
	if( WCDMA_ATANS_OK != iResult )
	{
		v_iNextSta = WCDMA_PST_RESET;
		// ����return, �������Ĵ����©��		return 0;
	}

	Frm23 frm;
	g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);	// �����������Ϳ���ָʾ

	Frm35 frm2;
	frm2.reply = 0x04;//�Ҷϵ绰��֪ͨsock�������
	DataPush((char*)&frm2, sizeof(frm2), DEV_PHONE, DEV_SOCK, 2);

	// ѡ��ͨ���ر�ģʽ
	_PhoneAudioModeSet( WCDMA_AUDIOMODE_CLOSE );

// 	if(1==g_iTerminalType) //������ֱ�ģʽ
// 	{
// 		tag2QHstCfg obj2QHstCfg;
// 		GetSecCfg(&obj2QHstCfg,sizeof(obj2QHstCfg),
// 			offsetof(tagSecondCfg, m_uni2QHstCfg.m_obj2QHstCfg), sizeof(obj2QHstCfg) );
// 
// 		// �ָ�Ĭ������
// 		if( obj2QHstCfg.m_HandsfreeChannel )
// 		{
// 			if(_PhoneAudioModeSet(WCDMA_AUDIOMODE_HANDFREE))
// 			{
// 				PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_HANDFREE);
// 			}
// 		}
// 		else
// 		{
// 			if(_PhoneAudioModeSet(WCDMA_AUDIOMODE_EARPHONE))
// 			{
// 				PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_EARPHONE);
// 			}
// 		}
// 	}
// 	else
// 	{	
// 		// ����������Ƶģʽ
// 		if(_PhoneAudioModeSet(WCDMA_AUDIOMODE_HANDFREE))
// 		{
// 			PRTMSG(MSG_DBG,"_PhoneAudioModeSet(%d) Fail!\n", WCDMA_AUDIOMODE_HANDFREE);
// 		}
// 	}

	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_Reset()
{
	PRTMSG(MSG_DBG,"Phone Reseting...\n");
	G_RecordDebugInfo("PhoneWcdma Reseting...");

	// ѡ��ͨ���ر�ģʽ
	_PhoneAudioModeSet( WCDMA_AUDIOMODE_CLOSE );

	// ����״̬
	tagWcdmaPhoneStaCommon objPhoneSta;
	objPhoneSta.m_bNetConnected = false;
	_PhoneStaCommonSet( objPhoneSta, WCDMA_PHSTVAR_NET );

	// ����رյ�ͬ������, que ��δʵ��

	// �ֻ�ģ���ʼ����λ��
	char cIndicate = 0x01;
	DataPush((char*)&cIndicate, 1, DEV_PHONE, DEV_LED, 2);

	Frm35 frm;
	frm.reply = 0x03;		// ����Ͽ���(֪ͨǧ���۳���)
	DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_SOCK, 2);

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	g_objLightCtrl2.SetLightSta(0x02);	// ��λ��ͬ������Ͽ�
#endif

	// ����ź�ֵ����������״̬
	_PhoneStaCommonClr( WCDMA_PHSTVAR_SING | WCDMA_PHSTVAR_NET );

	_PhoneCutNet();

	_SetAT(1000, 1, NULL, 0, NULL, 0, "ATH"); // �Ҷ����� (���մ���)
			
			
	//�ر�usbserial
	_CloseCom();

	//ͬ�������ֻ�ģ��ر�
	if( m_bToClosePhone )
	{
		usleep(5000);
		return 0;
	}

	//GSMģ�鸴λ
	_GsmHardReset();	// ģ���Ӳ����λ

	PRTMSG(MSG_DBG,"wcdma wait ready...\n");
	sleep(3);

	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_DialNet( int &v_iNextSta )
{
	PRTMSG(MSG_NOR,"stDialNet\n");
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	RenewMemInfo(0x02, 0x02, 0x01, 0x02);

	tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
	if(objPhoneSta.m_iSignal < 10)
		RenewMemInfo(0x02, 0x02, 0x01, 0x01);
#endif
	
	static BYTE sta_bytDialNetFailTime = 0;
	if( 0 == _PhoneDialNet() ) // �����Ŷ����ɹ�
	{
		m_dwDialNetTm = GetTickCount();
		sta_bytDialNetFailTime = 0;
		v_iNextSta = WCDMA_PST_WAITDIALNET;
	}
	else
	{
		sta_bytDialNetFailTime ++;
		if( sta_bytDialNetFailTime >= 2 ) // ������2�β��Ŷ���ʧ��
		{
			v_iNextSta = WCDMA_PST_RESET;
		}
		else
		{
			sleep(1);
			v_iNextSta = WCDMA_PST_CUTNET;
		}
	}
	
	return 0;
}

int CPhoneWcdma::_PhoneStaHdl_WaitDialNet( int &v_iNextSta )
{
	PRTMSG(MSG_NOR,"stWaitDialNet\n");
	
	if( true == m_bPhoneNetReady )
	{
		//����ָ�֪ͨ֡
		Frm35 frm;
		frm.reply = 0x01;	//���ųɹ�
		DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_SOCK, 2);
		
		/// { ���ֻ��������ص�����ָ��
		// ȡ���Ͽ���������
		_PhoneTaskClr( WCDMA_PHTASK_CUTNET );
		
		// ȡ����ǰ�ͽ�Ҫ���е�ping����
		_PhoneTaskClr( WCDMA_PHTASK_PING );
		
		/// } ���ֻ��������ص�����ָ��
		
		// ����״̬
		tagWcdmaPhoneStaCommon objPhoneSta;
		objPhoneSta.m_bNetConnected = true;
		_PhoneStaCommonSet( objPhoneSta, WCDMA_PHSTVAR_NET );
		
		// �ص�����״̬
		v_iNextSta = WCDMA_PST_IDLE;
		//		v_iNextSta = WCDMA_PST_RESET;//testc
	}
	else
	{
		if ((GetTickCount()-m_dwDialNetTm) > 1*60*1000)
		{
			PRTMSG(MSG_NOR, "Dial net Timeout!\n");

			G_RecordDebugInfo("Dial net Timeout!");
			
			//����Ͽ�֪ͨ֡
			Frm35 frm;
			frm.reply = 0x03;
			DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_SOCK, 2);
			
			// ����ָʾ��
#if VEHICLE_TYPE == VEHICLE_M
			g_objLightCtrl.SetLightSta(0x02);	// �̵ƿ���
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
			g_objLightCtrl2.SetLightSta(0x02);	// ����Ͽ�֪ͨ
#endif
			
			// ����״̬
			tagWcdmaPhoneStaCommon objPhoneSta;
			objPhoneSta.m_bNetConnected = false;
			_PhoneStaCommonSet( objPhoneSta, WCDMA_PHSTVAR_NET );
			
			v_iNextSta = WCDMA_PST_RESET; // ����ʧ�ܾ͸�λģ��
			sleep( 12 ); //�ȴ�һ��ʱ��,�Ա��ֻ�ģ����ʱ����ȡ����(����APN���û��ʱ��������ö���)
		}
		else
		{
			usleep(1000 * 500);
		}
	}
}

void CPhoneWcdma::_PhoneStaHdl_CutNet( int &v_iNextSta )
{
	/// { ���ڶ���������ǰ��
	//����Ͽ�֪ͨ֡
	Frm35 frm;
	frm.reply = 0x03;
	DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_SOCK, 2);

	// ����ָʾ��
#if VEHICLE_TYPE == VEHICLE_M
	g_objLightCtrl.SetLightSta(0x02);	// �̵ƿ���
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	g_objLightCtrl2.SetLightSta(0x02);	// ����Ͽ�֪ͨ
#endif

	// ����״̬
	tagWcdmaPhoneStaCommon objPhoneSta;
	objPhoneSta.m_bNetConnected = false;
	_PhoneStaCommonSet( objPhoneSta, WCDMA_PHSTVAR_NET );
	/// } ���ڶ���������ǰ��

	_PhoneCutNet();

	/// { ���ֻ��������ص�����ָ�� (Ҫ���ڶ��������ĺ���,��Ϊ�Ͽ������ִ����Ҫһ��ʱ��)
	// ȡ���Ͽ���������
	_PhoneTaskClr( WCDMA_PHTASK_CUTNET );

	// ȡ����ǰ�ͽ�Ҫ���е�ping����
	_PhoneTaskClr( WCDMA_PHTASK_PING );

	/// } ���ֻ��������ص�����ָ�� (Ҫ���ڶ��������ĺ���,��Ϊ�Ͽ������ִ����Ҫһ��ʱ��)

	if( WCDMA_ATANS_OK == _SetAT(1000, 3, NULL, 0, NULL, 0, "AT") )
	{
		v_iNextSta = WCDMA_PST_IDLE;
	}
	else
	{
		v_iNextSta = WCDMA_PST_RESET;
	}
}

void CPhoneWcdma::_ClearPhoneTask( DWORD v_dwToGetTaskSymb, tagWcdmaPhoneTask& v_objPhoneTask )
{
	v_objPhoneTask.m_dwTaskSymb &= ~v_dwToGetTaskSymb;

	if( v_dwToGetTaskSymb & WCDMA_PHTASK_DIALSN )
	{
		memset( v_objPhoneTask.m_szDialLsnTel, 0, sizeof(v_objPhoneTask.m_szDialLsnTel) );
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_DIAL )
	{
		memset( v_objPhoneTask.m_szDialTel, 0, sizeof(v_objPhoneTask.m_szDialTel) );
		v_objPhoneTask.m_dwHoldLmtPrid = 0;
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_RING )
	{
		//memset( v_objPhoneTask.m_szRingTel, 0, sizeof(v_objPhoneTask.m_szRingTel) );
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_SETSMSCENT )
	{
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_SNDDTMF )
	{
		memset( v_objPhoneTask.m_szDTMF, 0, sizeof(v_objPhoneTask.m_szDTMF) );
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_HANGUP )
	{
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_PICKUP )
	{
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_CHGAUDIO )
	{
		v_objPhoneTask.m_bytChgAudio = 0;
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_CHGVOL )
	{
		v_objPhoneTask.m_iChgVol = 0;
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_PING )
	{
		v_objPhoneTask.m_bytPingSrc = 0;
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_PINGRPT )
	{
		v_objPhoneTask.m_bytPingResult = 0;
		v_objPhoneTask.m_bytPingRptSrc = 0;
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_CUTNET )
	{
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_ALERM )
	{
		v_objPhoneTask.m_dwAlertPrid = 0;
	}
	if( v_dwToGetTaskSymb & WCDMA_PHTASK_RESET )
	{
	}
}

// ��ȡ�����ͬʱ��Ҫ����������־
tagWcdmaPhoneTask CPhoneWcdma::_PhoneTaskGet(DWORD v_dwToGetTaskSymb)
{
	tagWcdmaPhoneTask objRet;
	
	pthread_mutex_lock( &m_crisecPhoneTask );
	objRet = m_objPhoneTask; // ����䷵��ֵ
	_ClearPhoneTask(v_dwToGetTaskSymb, m_objPhoneTask); // �������Ӧ����
	pthread_mutex_unlock( &m_crisecPhoneTask );

	return objRet;
}

void CPhoneWcdma::_PhoneTaskSet( const tagWcdmaPhoneTask& v_objPhoneTask, DWORD v_dwToSetTaskSymb )
{
	pthread_mutex_lock( &m_crisecPhoneTask );

	// ���������־λ
	m_objPhoneTask.m_dwTaskSymb |= v_dwToSetTaskSymb;
	
	// ���þ�����������
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_DIALSN )
	{
		memcpy( m_objPhoneTask.m_szDialLsnTel, v_objPhoneTask.m_szDialLsnTel, sizeof(m_objPhoneTask.m_szDialLsnTel) );
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_DIAL )
	{
		memcpy( m_objPhoneTask.m_szDialTel, v_objPhoneTask.m_szDialTel, sizeof(m_objPhoneTask.m_szDialTel) );
		m_objPhoneTask.m_dwHoldLmtPrid = v_objPhoneTask.m_dwHoldLmtPrid;
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_RING )
	{
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_SETSMSCENT )
	{
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_SNDDTMF )
	{
		// �����յ����DTMF�����ͬʱ������ģʽ
		if( strlen(m_objPhoneTask.m_szDTMF) + 1 < sizeof(m_objPhoneTask.m_szDTMF) )
		{
			strncat( m_objPhoneTask.m_szDTMF, v_objPhoneTask.m_szDTMF,
				sizeof(m_objPhoneTask.m_szDTMF) - strlen(m_objPhoneTask.m_szDTMF) - 1 );
		}
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_HANGUP )
	{
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_PICKUP )
	{
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_CHGAUDIO )
	{
		m_objPhoneTask.m_bytChgAudio = v_objPhoneTask.m_bytChgAudio;
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_CHGVOL )
	{
		m_objPhoneTask.m_iChgVol = v_objPhoneTask.m_iChgVol;
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_PING )
	{
		m_objPhoneTask.m_bytPingSrc = v_objPhoneTask.m_bytPingSrc;
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_PINGRPT )
	{
		m_objPhoneTask.m_bytPingRptSrc = v_objPhoneTask.m_bytPingRptSrc;
		m_objPhoneTask.m_bytPingResult = v_objPhoneTask.m_bytPingResult;
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_CUTNET )
	{
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_ALERM )
	{
		m_objPhoneTask.m_dwAlertPrid = v_objPhoneTask.m_dwAlertPrid;
	}
	if( v_dwToSetTaskSymb & WCDMA_PHTASK_RESET )
	{
	}

	pthread_mutex_unlock( &m_crisecPhoneTask );
}

void CPhoneWcdma::_PhoneTaskClr(DWORD v_dwToGetTaskSymb)
{
	// �º����ָ��������״̬��־
	pthread_mutex_lock( &m_crisecPhoneTask );
	_ClearPhoneTask(v_dwToGetTaskSymb, m_objPhoneTask);
	pthread_mutex_unlock( &m_crisecPhoneTask );
}

tagWcdmaPhoneStaCommon CPhoneWcdma::_PhoneStaCommonGet( DWORD v_dwToClrStaSymb )
{
	tagWcdmaPhoneStaCommon objRet;
	pthread_mutex_lock( &m_crisecSta );
	objRet = m_objPhoneStaCommon;
	_ClearPhoneStaCommon( m_objPhoneStaCommon, v_dwToClrStaSymb );
	pthread_mutex_unlock( &m_crisecSta );

	return objRet;
}

void CPhoneWcdma::_PhoneStaCommonSet( const tagWcdmaPhoneStaCommon& v_objPhoneStaCommon, DWORD v_dwToSetStaSymb )
{
	pthread_mutex_lock( &m_crisecSta );
	if( v_dwToSetStaSymb & WCDMA_PHSTVAR_IMEI )
	{
		memcpy( m_objPhoneStaCommon.m_szGsmIMEI, v_objPhoneStaCommon.m_szGsmIMEI, sizeof(m_objPhoneStaCommon.m_szGsmIMEI) );
	}

	if( v_dwToSetStaSymb & WCDMA_PHSTVAR_SING )
	{
		m_objPhoneStaCommon.m_iSignal = v_objPhoneStaCommon.m_iSignal;
	}

	if( v_dwToSetStaSymb & WCDMA_PHSTVAR_ALERT )
	{
		m_objPhoneStaCommon.m_dwAlertBegin = v_objPhoneStaCommon.m_dwAlertBegin;
		m_objPhoneStaCommon.m_dwAlertPrid = v_objPhoneStaCommon.m_dwAlertPrid;
	}

	if( v_dwToSetStaSymb & WCDMA_PHSTVAR_NET )
	{
		m_objPhoneStaCommon.m_bNetConnected = v_objPhoneStaCommon.m_bNetConnected;
	}

	if( v_dwToSetStaSymb & WCDMA_PHSTVAR_PING )
	{
		m_objPhoneStaCommon.m_bytPingSrc |= v_objPhoneStaCommon.m_bytPingSrc;
	}

	if( v_dwToSetStaSymb & WCDMA_PHSTVAR_AUDIOMODE )
	{
		m_objPhoneStaCommon.m_bytCurAudioMode = v_objPhoneStaCommon.m_bytCurAudioMode;
	}

	if( v_dwToSetStaSymb & WCDMA_PHSTVAR_AUDIOOUTLVL )
	{
		m_objPhoneStaCommon.m_iAudioOutVol = v_objPhoneStaCommon.m_iAudioOutVol;
	}

	pthread_mutex_unlock( &m_crisecSta );
}

void CPhoneWcdma::_PhoneStaCommonClr(DWORD v_dwToClrStaSymb)
{
	pthread_mutex_lock( &m_crisecSta );

	_ClearPhoneStaCommon( m_objPhoneStaCommon, v_dwToClrStaSymb );

	pthread_mutex_unlock( &m_crisecSta );
}

//	ע�⣺�˴���_PhoneStaCommonGet���Ѿ�ʹ�û�����ʣ���������
void CPhoneWcdma::_ClearPhoneStaCommon(tagWcdmaPhoneStaCommon& v_objPhoneStaCommon, DWORD v_dwToClrStaSymb)
{
	if( v_dwToClrStaSymb & WCDMA_PHSTVAR_IMEI )
	{
		memset( v_objPhoneStaCommon.m_szGsmIMEI, 0, sizeof(v_objPhoneStaCommon.m_szGsmIMEI) );
	}

	if( v_dwToClrStaSymb & WCDMA_PHSTVAR_SING )
	{
		v_objPhoneStaCommon.m_iSignal = 0;
	}

	if( v_dwToClrStaSymb & WCDMA_PHSTVAR_ALERT )
	{
		v_objPhoneStaCommon.m_dwAlertBegin = 0;
		v_objPhoneStaCommon.m_dwAlertPrid = 0;
	}

	if( v_dwToClrStaSymb & WCDMA_PHSTVAR_NET )
	{
		v_objPhoneStaCommon.m_bNetConnected = false;
	}

	if( v_dwToClrStaSymb & WCDMA_PHSTVAR_PING )
	{
		v_objPhoneStaCommon.m_bytPingSrc = 0;
	}

	if( v_dwToClrStaSymb & WCDMA_PHSTVAR_AUDIOMODE )
	{
		v_objPhoneStaCommon.m_bytCurAudioMode = 0;
	}

	if( v_dwToClrStaSymb & WCDMA_PHSTVAR_AUDIOOUTLVL )
	{
		v_objPhoneStaCommon.m_iAudioOutVol = 4;
	}
}

void CPhoneWcdma::_PhoneJugSmsSnd()
{
	DWORD dwLen = 0;
	DWORD dwPushTm = 0;
	BYTE bytLvl = 0;
	BYTE bytSymb = 0;
	BYTE bytSrcType = 0;
	BYTE bytSndTimes = 0;
	BYTE bytSndCt = 0;
	char szSndTel[16] = { 0 };
	char szSndData[200] = { 0 };
	SM_PARAM objSmPar;
	int iResult = 0;
	DWORD dwPktNum;
	while( !m_objDataMngSmsSnd.PopData(bytLvl, sizeof(szSndData), dwLen, szSndData, dwPushTm,
		&bytSymb, szSndTel, sizeof(szSndTel)) )
	{
		if( dwLen > sizeof(szSndData) ) continue;

		bytSndCt ++;
		//pop�Ķ���Ķ��巽ʽ�иı䣬��������淽ʽ
		bytSndTimes = bytSymb >> 4;
		bytSrcType = bytSymb & 0x0f;

		memset( &objSmPar, 0, sizeof(objSmPar) );
		memcpy( objSmPar.tpa, szSndTel, min(sizeof(objSmPar.tpa), sizeof(szSndTel)) );
		memcpy( objSmPar.ud, szSndData, min(sizeof(objSmPar.ud), dwLen) );

		iResult = _PhoneSmsSnd( objSmPar );
		if( WCDMA_ATANS_OK != iResult )
		{
			//PRTMSG(MSG_ERR, "Send SM fail\n" );

			bytSndTimes ++;
			if( bytSndTimes < 2 ) // ����ʧ�ܴ���δ��2��,��ش��ط�
			{
				bytSymb = bytSndTimes << 4 | bytSrcType;
				m_objDataMngSmsSnd.SaveBackData( bytLvl, dwLen, (char*)szSndData, bytSymb, (char*)szSndTel );
			}
			else
			{
				PRTMSG(MSG_ERR,"Sms Snd Fail 2 times, Discard!\n");

				if( INNERSYMB_PHONETODIAO == bytSrcType )
				{
					Frm31 frm;
					frm.reply = 2;
					g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
				}
			}
		}
		else
		{
			if( INNERSYMB_PHONETODIAO == bytSrcType )
			{
				Frm31 frm;
				frm.reply = 1;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
		}

		if( bytSndCt >= 2 ) // ÿ����ೢ�Է���2��,�������´��ٷ���
		{
			break;
		}

		memset( szSndData, 0, sizeof(szSndData) );
		memset(szSndTel, 0, sizeof(szSndTel) );
	}
}

int CPhoneWcdma::_PhoneJugSmsRcv()
{
	_QueryRcvSMS();
}

//����¶���Ϣ���������Ϣ�������ţ����ض���Ϣ������
//ע�⣺����ĵ�ַ��Ҫ�㹻��
int CPhoneWcdma::_QueryRcvSMS()
{
	char szSmsInx[64] = {0};
	int iLen = 0;
	BYTE bytLvl = 0;
	SM_PARAM sm;
	DWORD dwResLen;
	DWORD dwPushTm;
	int i,j;

	_SetAT( 500, 1, szSmsInx, sizeof(szSmsInx), G_WCDMA_CMGD_SmsList, 0, "AT+CMGD=?" );
	iLen = strlen(szSmsInx);

	for (i=0; i<iLen; i++)
	{
		if( WCDMA_ATANS_OK == _SetAT(1000, 5, NULL, 0, G_WCDMA_HCMGR_JugSend, 0, "AT^HCMGR=%c",szSmsInx[i]) ) 
		for(j=0; j<SMS_POP_COUNT; j++)
		{
			if( 0 != m_objDataMngSmsRcv.PopData( bytLvl, (DWORD)sizeof(sm), dwResLen, (char*)&sm, dwPushTm ,NULL, NULL, 0) )
			{
				usleep(200000);
				continue;
			}
			else break;
		}
		if ( j < SMS_POP_COUNT )
		{
			sms_deal(&sm);
			_SetAT(1000, 2, NULL, 0, NULL, 0, "AT+CMGD=%c",szSmsInx[i]); 
		}
	}
	return 0;
}

//que ��֧��
void CPhoneWcdma::_PhoneJugSmsCentSet()
{
	tagWcdmaPhoneTask objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_SETSMSCENT );
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_SETSMSCENT )
	{
		tag1PComuCfg cfg;
		GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) );

		if(strcmp(cfg.m_szSmsCentID, "")!=0)
		{
			// �Ȳ�ѯSIM���ڵĶ������ĺ�
			char szSmsCentID[100] = {0};
			/////nnnnnnn
			_SetAT(5000, 2, szSmsCentID, sizeof(szSmsCentID), G_WCDMA_CSCA_Query_res, 0, "AT+CSCA?");

			if( strcmp(cfg.m_szSmsCentID, szSmsCentID) )
			{
				_SetAT(6000, 2, NULL, 0, NULL, 0, "AT+CSCA=\"%.21s\"", cfg.m_szSmsCentID);
			}
			else
			{
				PRTMSG(MSG_DBG,"SmsCent in cfg is equal to in SimCard !\n");
			}
		}
		else
		{
			PRTMSG(MSG_ERR,"SmsCent in cfg is empty!\n");
		}
	}
}
//	que ping��δʵ��
void CPhoneWcdma::_PhoneJugSmsPingRpt()
{
	tagWcdmaPhoneTask objPhoneTask = _PhoneTaskGet( WCDMA_PHTASK_PINGRPT );
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_PINGRPT )
	{
		if( 0 == objPhoneTask.m_bytPingResult || 1 == objPhoneTask.m_bytPingResult
			|| 2 == objPhoneTask.m_bytPingResult ) // ping�ɹ���ʧ�ܻ��쳣
		{
			Frm35 frm;
			frm.reply = (0 == objPhoneTask.m_bytPingResult ? 2 : 3);
			if(objPhoneTask.m_bytPingSrc & 0x01 )
			{
				DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_QIAN, 2);
			}
			if( objPhoneTask.m_bytPingSrc & 0x02 )
			{
				DataPush((char*)&frm, sizeof(frm), DEV_PHONE, DEV_UPDATE, 2);
			}
		}
		else if( 3 == objPhoneTask.m_bytPingResult ) // ��ping��ȡ��
		{
			// (pingȡ��������֪ͨ)
		}
	}
}

void CPhoneWcdma::_PhoneJugAlerm()
{
	tagWcdmaPhoneTask objPhoneTask = _PhoneTaskGet(WCDMA_PHTASK_ALERM);
	if( objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_ALERM )
	{
		tagWcdmaPhoneStaCommon objPhoneSta;
		objPhoneSta.m_dwAlertBegin = GetTickCount() - 1;
		objPhoneSta.m_dwAlertPrid = objPhoneTask.m_dwAlertPrid;

		_PhoneStaCommonSet(objPhoneSta, WCDMA_PHSTVAR_ALERT);
	}
}

BOOL CPhoneWcdma::_PhoneJugReset()
{
	tagWcdmaPhoneTask objPhoneTask = _PhoneTaskGet(WCDMA_PHTASK_RESET);
	return m_bToClosePhone ? TRUE: (objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_RESET ? TRUE : FALSE);
}

BOOL CPhoneWcdma::_PhoneJugCutNet()
{
	tagWcdmaPhoneTask objPhoneTask = _PhoneTaskGet(WCDMA_PHTASK_CUTNET);
	return objPhoneTask.m_dwTaskSymb & WCDMA_PHTASK_CUTNET ? TRUE : FALSE;
}


int CPhoneWcdma::_PhoneSmsSnd( SM_PARAM& v_objSmPar )
{
/*
	int iRet = 0;
	char smsbuf[512] = {0};
	int smslen;
	int i;
	bool unicode = false;
	for( i=0; i<strlen(v_objSmPar.ud); i++) 
	{
		if((byte)v_objSmPar.ud[i] > 0x80)
		{ 
			unicode=true;  
			break; 
		}
	}
	if(unicode)		v_objSmPar.dcs = SMS_UCS2;
	else			v_objSmPar.dcs = SMS_7BIT;

	wcdma_encode_pdu(&v_objSmPar,smsbuf,smslen);
	PRTMSG(MSG_NOR,"testc smslen: %d\n",smslen);

//	v_objSmPar->tpa[strlen(v_objSmPar.tpa)-1] = 0;
//	char cmd[64] = {0};
	char cmdSMS[1024] = {0};
	int cmdLen = 0;

	char smsEnd[64]={0};//����Ϣ��������ctrl+z����Ӧ0x1a
	switch(v_objSmPar.dcs)
	{
	case SMS_7BIT:
		{
			//���ö���״̬���棬�������ȼ������뷽ʽ����1ΪASCII���룩�����ܼ���
			_SetAT(1000, 5, NULL, 0, NULL, 0, "AT^HSMSSS=0,0,1,0");
			smsEnd[0] = 0x1a;
			smsEnd[1] = '\0';
		}
		break;
		
	case SMS_UCS2:
		{
			//���ö���״̬���棬�������ȼ������뷽ʽ����1ΪASCII���룩�����ܼ���
			_SetAT(1000, 5, NULL, 0, NULL, 0, "AT^HSMSSS=0,0,6,0");
// 			smsEnd[0] = 0x0;//unicode
// 			smsEnd[1] = 0x1a;//���԰��˷�ʽ���Ͳ���
// 			smsEnd[2] = '\0';
			smsEnd[0] = 0x1a;
			smsEnd[1] = '\0';
		}
		break;
		
	case SMS_8BIT:
		{
			//���ö���״̬���棬�������ȼ������뷽ʽ����1ΪASCII���룩�����ܼ���
			_SetAT(1000, 5, NULL, 0, NULL, 0, "AT^HSMSSS=0,0,1,0");
			smsEnd[0] = 0x1a;
			smsEnd[1] = '\0';
		}
		break;
		
	default:
		{
			return false;
		}
		break;
	}

	smsbuf[smslen] = '\r';
//	smsbuf[smslen+1] = '\n';
	//			smsbuf[smslen+2] = 0x0;//0x001a,ctrl+z
	//			smsbuf[smslen+3] = 0x1a;//
	cmdLen = smslen+1;
	//sprintf(cmdSMS,"%s",smsbuf);
	memcpy(cmdSMS,smsbuf,cmdLen);  //unicode������sprintf,��00����

	//AT^HCMGS="�ֻ�����"
	iRet = _SetAT( 20000, 1, NULL, 0, G_WCDMA_HCMGS_JugRes, 0, "AT^HCMGS=\"%s\"\r\n",v_objSmPar.tpa );
	if( WCDMA_ATANS_OK == iRet )
	{
		//���Ͷ������ݣ���Ӧ����Ϣ
		_ComSnd(cmdSMS, size_t(cmdLen) );
		//���Ž�����־ctrl+z
		iRet = _SetAT(1000, 2, NULL, 0, G_WCDMA_HCMGS_JugSend, 0, "%s",smsEnd);
	}

	if( WCDMA_ATANS_OK == iRet )
	{
		PRTMSG(MSG_NOR,"Send SM succ!\n");
	}
	else
	{
		PRTMSG(MSG_NOR,"Send SM fail!\n");
	}
	
	return iRet;
*/
	int iRet = 0;
	char pdu[512] = {0};
	
	int len1 = encode_pdu(&v_objSmPar, pdu);	//����PDU����������PDU��
	if( len1 + 5 < sizeof(pdu) )
	{
		strcpy( pdu + len1, "\x1a");				//��Ctrl-Z����
	}
	
	byte len2 = 0;
	string_to_bytes(pdu, &len2, 2);		//ȡPDU���е�SCA��Ϣ����
	len2++;
	
	// �����еĳ��ȣ�������SCA��Ϣ���ȣ��������ֽڼ�
	int iSmsLen = len1 / 2-1;
	if( iSmsLen <= 0 )
	{
		PRTMSG(MSG_DBG, "send SMS err, iSmsLen = %d\n", iSmsLen);
		return -1;
	}

	iRet = _SetAT( 5000, 1, NULL, 0, G_WCDMA_CMGS_JugRes, 0, "AT+CMGS=%d", iSmsLen );
	PRTMSG( MSG_DBG, "AT+CMGS=%d, result:%d\n", iSmsLen, iRet );

	if( WCDMA_ATANS_OK == iRet )
	{
		iRet = _SetAT( 20000, 1,  NULL, 0, NULL, DWORD(strlen(pdu)), pdu );
	}
	
	if( WCDMA_ATANS_OK == iRet )
	{
		PRTMSG(MSG_NOR,"Send SM succ!\n");
	}
	else
	{
		PRTMSG(MSG_ERR,"Send SM fail!\n");
	}
	
	return iRet;
}

#if 0 == USE_TELLIMIT
int CPhoneWcdma::JugTelType( char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType )
{
	*v_p_cTelType = 0; // ��ʼ�����ֵ
	BYTE bytCtrl = 0;
	BYTE byt1 = 0, byt2 = 0;

	tag2QFbidTelCfg obj2QFbidTelCfg;
	int iRet = GetSecCfg( &obj2QFbidTelCfg, sizeof(obj2QFbidTelCfg), offsetof(tagSecondCfg, m_uni2QFbidTelCfg.m_obj2QFbidTelCfg),
		sizeof(obj2QFbidTelCfg) );
	if( iRet )
	{
		*v_p_cTelType = 0; // ����������
		return iRet;
	}

	tag2QServCodeCfg objListenCfg;
	iRet = GetSecCfg( &objListenCfg, sizeof(objListenCfg), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg),
		sizeof(objListenCfg) );
	if( iRet )
	{
		*v_p_cTelType = 0; // ����������
		return iRet;
	}

	// ��������ж�
	if( NULL == v_szTel || v_ucTelLen > 15 || v_ucTelLen < 3 )
	{
		*v_p_cTelType |= 0x20; // ��ֹ����

		bytCtrl = obj2QFbidTelCfg.m_bytFbidType & BYTE(0x0f);
		if( 0x01 == bytCtrl ) *v_p_cTelType |= 0x02; // ��ֹ�������
		else *v_p_cTelType |= 0; // �������
		return ERR_PAR;
	}

	// ȥ����������β�ո�
	for( unsigned char uc = v_ucTelLen - 1; uc > 0; uc -- )
	{
		if( 0x20 == v_p_cTelType[ uc ] ) v_p_cTelType[ uc ] = 0;
		else break;
	}	
	char szJugTel[16] = { 0 };
	char szCompTel[16] = { 0 };
	memcpy( szJugTel, v_szTel, v_ucTelLen < 14 ? v_ucTelLen : 14 );
	char* pos = strchr( szJugTel, 0x20 );
	if( pos ) *pos = 0;


	// ���������ж�
	memcpy( szCompTel, objListenCfg.m_aryLsnHandtel[0], 15 );
	pos = strchr( szCompTel, 0x20 );
	if( pos ) *pos = 0;
	if( objListenCfg.m_bytListenTelCount > 0 && 0 == strcmp(szJugTel, szCompTel) )
	{
		*v_p_cTelType |= 0x10; // ��̨������������(����)
	}
	else
	{
		bytCtrl = obj2QFbidTelCfg.m_bytFbidType & BYTE(0xf0);

		if( 0 == bytCtrl ) *v_p_cTelType |= 0; // �������
		else if( 0x10 == bytCtrl ) *v_p_cTelType |= 0x20; // ��ֹ��������
		else if( 0x20 == bytCtrl ) // ����������: �����ֺ������ֽ�ֹĳЩ����
		{
			for( byt1 = 0; byt1 < obj2QFbidTelCfg.m_bytAllowOutTelCount; byt1 ++ )
			{
				memcpy( szCompTel, obj2QFbidTelCfg.m_aryAllowOutHandtel[ byt1 ], 15 );
				pos = strchr( szCompTel, 0x20 );
				if( pos ) *pos = 0;

				if( 0 == strncmp(szJugTel, szCompTel, strlen(szCompTel)) ) // �����������������
				{
					for( byt2 = 0; byt2 < obj2QFbidTelCfg.m_bytFbidOutTelCount; byt2 ++ )
					{
						memcpy( szCompTel, obj2QFbidTelCfg.m_aryFbidOutHandtel[ byt2 ], 15 );
						pos = strchr( szCompTel, 0x20 );
						if( pos ) *pos = 0;

						if( 0 == strncmp(szJugTel, szCompTel, strlen(szCompTel)) ) // �������ֽ�ֹ�����ĺ���
							break;
					}

					if( byt2 >= obj2QFbidTelCfg.m_bytFbidOutTelCount ) *v_p_cTelType |= 0; // �������
					else *v_p_cTelType |= 0x20; // ��ֹ����

					break;
				}
			}

			if( byt1 >= obj2QFbidTelCfg.m_bytAllowOutTelCount ) *v_p_cTelType |= 0x20; // ��ֹ����
		}
		else if( 0x30 == bytCtrl ) // ����������: ��ֹ���ֺ�����������ĳЩ����
		{
			for( byt1 = 0; byt1 < obj2QFbidTelCfg.m_bytFbidOutTelCount; byt1 ++ )
			{
				memcpy( szCompTel, obj2QFbidTelCfg.m_aryFbidOutHandtel[ byt1 ], 15 );
				pos = strchr( szCompTel, 0x20 );
				if( pos ) *pos = 0;

				if( 0 == strncmp(szJugTel, szCompTel, strlen(szCompTel)) ) // �����ڽ�ֹ��������
				{
					for( byt2 = 0; byt2 < obj2QFbidTelCfg.m_bytAllowOutTelCount; byt2 ++ )
					{
						memcpy( szCompTel, obj2QFbidTelCfg.m_aryAllowOutHandtel[ byt2 ], 15 );
						pos = strchr( szCompTel, 0x20 );
						if( pos ) *pos = 0;

						if( 0 == strncmp(szJugTel, szCompTel, strlen(szCompTel)) ) // ����������������ĺ���
							break;
					}

					if( byt2 >= obj2QFbidTelCfg.m_bytAllowOutTelCount ) *v_p_cTelType |= 0x20; // ��ֹ����
					else *v_p_cTelType |= 0; // �������

					break;
				}
			}

			if( byt1 >= obj2QFbidTelCfg.m_bytFbidOutTelCount ) *v_p_cTelType |= 0; // �������
		}
	}

	// ���������ж�
	for( byt1 = 0; byt1 < objListenCfg.m_bytListenTelCount; byt1 ++ )
	{
		memcpy( szCompTel, objListenCfg.m_aryLsnHandtel[ byt1 ], 15 );
		pos = strchr( szCompTel, 0x20 );
		if( pos ) *pos = 0;

		if( 0 == strcmp(szJugTel, szCompTel) ) break;
	}
	if( byt1 < objListenCfg.m_bytListenTelCount ) *v_p_cTelType |= 0x01; // ����������������
	else
	{
		bytCtrl = obj2QFbidTelCfg.m_bytFbidType & BYTE(0x0f);

		if( 0 == bytCtrl ) *v_p_cTelType |= 0; // �������
		else if( 0x01 == bytCtrl ) *v_p_cTelType |= 0x02; // ��ֹ�������
		else if( 0x02 == bytCtrl ) // ����������: �����ֺ��뵫�ֽ�ֹĳЩ����
		{
			for( byt1 = 0; byt1 < obj2QFbidTelCfg.m_bytAllowInTelCount; byt1 ++ )
			{
				memcpy( szCompTel, obj2QFbidTelCfg.m_aryAllowInHandtel[ byt1 ], 15 );
				pos = strchr( szCompTel, 0x20 );
				if( pos ) *pos = 0;

				if( 0 == strncmp(szJugTel, szCompTel, strlen(szCompTel)) ) // ����������������
				{
					for( byt2 = 0; byt2 < obj2QFbidTelCfg.m_bytFbidInTelCount; byt2 ++ )
					{
						memcpy( szCompTel, obj2QFbidTelCfg.m_aryFbidInHandtel[ byt2 ], 15 );
						pos = strchr( szCompTel, 0x20 );
						if( pos ) *pos = 0;

						if( 0 == strncmp(szJugTel, szCompTel, strlen(szCompTel)) ) // �������ֽ�ֹ����ĺ���
							break;
					}

					if( byt2 >= obj2QFbidTelCfg.m_bytFbidInTelCount ) *v_p_cTelType |= 0; // �������
					else *v_p_cTelType |= 0x02; // ��ֹ����

					break;
				}
			}

			if( byt1 >= obj2QFbidTelCfg.m_bytAllowInTelCount ) *v_p_cTelType |= 0x02; // ��ֹ����
		}
		else if( 0x03 == bytCtrl ) // ����������: ��ֹ���ֺ��뵫������ĳЩ����
		{
			for( byt1 = 0; byt1 < obj2QFbidTelCfg.m_bytFbidInTelCount; byt1 ++ )
			{
				memcpy( szCompTel, obj2QFbidTelCfg.m_aryFbidInHandtel[ byt1 ], 15 );
				pos = strchr( szCompTel, 0x20 );
				if( pos ) *pos = 0;

				if( 0 == strncmp(szJugTel, szCompTel, strlen(szCompTel)) ) // �����ڽ�ֹ�������
				{
					for( byt2 = 0; byt2 < obj2QFbidTelCfg.m_bytAllowInTelCount; byt2 ++ )
					{
						memcpy( szCompTel, obj2QFbidTelCfg.m_aryAllowInHandtel[ byt2 ], 15 );
						pos = strchr( szCompTel, 0x20 );
						if( pos ) *pos = 0;

						if( 0 == strncmp(szJugTel, szCompTel, strlen(szCompTel)) ) // ���������������ĺ���
							break;
					}

					if( byt2 >= obj2QFbidTelCfg.m_bytAllowInTelCount ) *v_p_cTelType |= 0x02; // ��ֹ����
					else *v_p_cTelType |= 0; // �������

					break;
				}
			}

			if( byt1 >= obj2QFbidTelCfg.m_bytFbidInTelCount ) *v_p_cTelType |= 0; // �������
		}
	}
	
	return 0;
}
#endif


#if 2 == USE_TELLIMIT
int CPhoneWcdma::JugTelType2( char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType,
	WORD* v_p_wLmtOutPeriod, WORD* v_p_wLmtInPeriod )
{
	*v_p_cTelType = 0x22; // ��ʼ�����ֵΪ��ֹ�������
	*v_p_wLmtOutPeriod = * v_p_wLmtInPeriod = 0; // ��ʼ��Ϊ����ʱ

	int iRet = 0;
	char* pos = NULL;
	bool bSkipOut = false, bSkipIn = false;
	BYTE byt = 0;
	char szJugTel[16] = { 0 };
	char szCompTel[16] = { 0 };
	
	// ��������ж�
	if( NULL == v_szTel || v_ucTelLen > 15 || v_ucTelLen < 3 )
	{
		*v_p_cTelType |= 0x22; // ��ֹ�������
		return ERR_PAR;
	}

	// ȥ����������β�ո�
	memcpy( szJugTel, v_szTel, v_ucTelLen < sizeof(szJugTel) ? v_ucTelLen : sizeof(szJugTel) );
	szJugTel[ sizeof(szJugTel) - 1 ] = 0;
	pos = strchr( szJugTel, 0x20 );
	if( pos ) *pos = 0;

	// ��ȡ����
	tag2QCallCfg objCallCfg;
	objCallCfg.Init();
	tag2QTelBookPar* pTelBook = NULL;
	iRet = GetSecCfg( &objCallCfg, sizeof(objCallCfg),
		offsetof(tagSecondCfg, m_uni2QCallCfg.m_obj2QCallCfg), sizeof(objCallCfg) );

	tag2QServCodeCfg objServCodeCfg;
	objServCodeCfg.Init();
	iRet = GetSecCfg( &objServCodeCfg, sizeof(objServCodeCfg),
		offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objServCodeCfg) );

// 	tag2QListenCfg objListenCfg;
// 	objListenCfg.Init();
// 	iRet = GetSecCfg( &objListenCfg, sizeof(objListenCfg), offsetof(tagSecondCfg, m_uni2QListenCfg.m_obj2QListenCfg),
// 		sizeof(objListenCfg) );

// 	tag2QCentServTelCfg objCentTelCfg;
// 	objCentTelCfg.Init();
// 	iRet = GetSecCfg( &objCentTelCfg, sizeof(objCentTelCfg),
// 		offsetof(tagSecondCfg, m_uni2QCentServTelCfg.m_obj2QCentServTelCfg), sizeof(objCentTelCfg) );
// 
// 	tag2QUdpAtvTelCfg objUdpActCfg;
// 	objUdpActCfg.Init();
// 	iRet = GetSecCfg( &objUdpActCfg, sizeof(objUdpActCfg),
// 		offsetof(tagSecondCfg, m_uni2QUdpAtvTelCfg.m_obj2QUdpAtvTelCfg), sizeof(objUdpActCfg) );

	
	// (1)����,���������ж�

	// �������������ж� (����)
	memset( szCompTel, 0, sizeof(szCompTel) );
	memcpy( szCompTel, objServCodeCfg.m_aryLsnHandtel[0],
		sizeof(objServCodeCfg.m_aryLsnHandtel[0]) < sizeof(szCompTel)
		? sizeof(objServCodeCfg.m_aryLsnHandtel[0])
		: sizeof(szCompTel) );
	szCompTel[ sizeof(szCompTel) - 1 ] = 0;
	pos = strchr( szCompTel, 0x20 );
	if( pos ) *pos = 0;
	if( objServCodeCfg.m_bytListenTelCount > 0 && 0 == strcmp(szJugTel, szCompTel) )
	{
		*v_p_cTelType = *v_p_cTelType & 0x0f | 0x10; // �޸�Ϊ��̨������������(����)
		bSkipOut = true;
	}

	// ���ļ��������ж� (����)
	for( byt = 0; byt < objServCodeCfg.m_bytListenTelCount; byt ++ )
	{
		memset( szCompTel, 0, sizeof(szCompTel) );
		memcpy( szCompTel, objServCodeCfg.m_aryLsnHandtel[ byt ],
			sizeof(objServCodeCfg.m_aryLsnHandtel[0]) < sizeof(szCompTel)
			? sizeof(objServCodeCfg.m_aryLsnHandtel[0])
			: sizeof(szCompTel) );
		szCompTel[ sizeof(szCompTel) - 1 ] = 0;
		pos = strchr( szCompTel, 0x20 );
		if( pos ) *pos = 0;

		if( 0 == strcmp(szJugTel, szCompTel) )
		{
			*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x01; // �޸�Ϊ����������������(����)
			bSkipIn = true;
			break;
		}
	}


	// (2)���ķ���绰�����ж�
	for( byt = 0; byt < objServCodeCfg.m_bytCentServTelCount; byt ++ )
	{
		memset( szCompTel, 0, sizeof(szCompTel) );
		memcpy( szCompTel, objServCodeCfg.m_aryCentServTel[ byt ],
			sizeof(objServCodeCfg.m_aryCentServTel[0]) < sizeof(szCompTel)
			? sizeof(objServCodeCfg.m_aryCentServTel[0])
			: sizeof(szCompTel) );
		szCompTel[ sizeof(szCompTel) - 1 ] = 0;
		pos = strchr( szCompTel, 0x20 );
		if( pos ) *pos = 0;

		if( 0 == strcmp(szJugTel, szCompTel) )
		{
			if( !bSkipOut )
			{
				*v_p_cTelType = *v_p_cTelType & 0x0f | 0x30; // �޸�Ϊ���ķ������(����)
				bSkipOut = true;
			}
			if( !bSkipIn )
			{
				*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x03; // �޸�Ϊ���ķ������(����)
				bSkipIn = true;
			}
			break;
		}
	}


	// (3)UDP����绰�����ж� (���漰����)
	for( byt = 0; byt < objServCodeCfg.m_bytUdpAtvTelCount; byt ++ )
	{
		memset( szCompTel, 0, sizeof(szCompTel) );
		memcpy( szCompTel, objServCodeCfg.m_aryUdpAtvTel[ byt ],
			sizeof(objServCodeCfg.m_aryUdpAtvTel[0]) < sizeof(szCompTel)
			? sizeof(objServCodeCfg.m_aryUdpAtvTel[0])
			: sizeof(szCompTel) );
		szCompTel[ sizeof(szCompTel) - 1 ] = 0;
		pos = strchr( szCompTel, 0x20 );
		if( pos ) *pos = 0;

		if( 0 == strcmp(szJugTel, szCompTel) )
		{
			if( !bSkipIn )
			{
				*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x04; // �޸�ΪUDP�������(����)
				bSkipIn = true;
			}
			break;
		}
	}

	
	// (4)���,���뱾�ж�

	// ���ͳһ�����˺�������
	if( 0 == (objCallCfg.m_bytSetType & 0x0c) && !bSkipOut ) // ��ͳһ����Ϊ�������
	{
		*v_p_cTelType = *v_p_cTelType & 0x0f | 0;
		bSkipOut = true;
	}
	else if( 0x04 == (objCallCfg.m_bytSetType & 0x0c) && !bSkipOut ) // ��ͳһ����Ϊ��ֹ����
	{
		*v_p_cTelType = *v_p_cTelType & 0x0f | 0x20;
		bSkipOut = true;
	}
	if( 0 == (objCallCfg.m_bytSetType & 0x03) && !bSkipIn  ) // ��ͳһ����Ϊ�������
	{
		*v_p_cTelType = *v_p_cTelType & 0xf0 | 0;
		bSkipIn = true;
	}
	else if( 0x01 == (objCallCfg.m_bytSetType & 0x03) && !bSkipIn ) // ��ͳһ����Ϊ��ֹ����
	{
		*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x02;
		bSkipIn = true;
	}

	// ���������ж�
	for( byt = 0; byt < sizeof(objCallCfg.m_aryTelBook) / sizeof(objCallCfg.m_aryTelBook[0]); byt ++ )
	{
		pTelBook = objCallCfg.m_aryTelBook + byt;
		memset( szCompTel, 0, sizeof(szCompTel) );
		memcpy( szCompTel, pTelBook->m_szTel,
			sizeof(pTelBook->m_szTel) < sizeof(szCompTel) ? sizeof(pTelBook->m_szTel) : sizeof(szCompTel) );
		szCompTel[ sizeof(szCompTel) - 1 ] = 0;
		pos = strchr( szCompTel, 0x20 );
		if( pos ) *pos = 0;

		if( !strcmp(szJugTel, szCompTel) )
		{
			switch( pTelBook->m_bytType )
			{
			case 0x01: // ��������룬�������
				if( !bSkipOut )
				{
					*v_p_cTelType = *v_p_cTelType & 0x0f | 0;
					*v_p_wLmtOutPeriod = pTelBook->m_wLmtPeriod;
				}
				if( !bSkipIn )
				{
					*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x02; 
					*v_p_wLmtInPeriod = pTelBook->m_wLmtPeriod;
				}
				break;

			case 0x02: // ������룬�������
				if( !bSkipOut )
				{
					*v_p_cTelType = *v_p_cTelType & 0x0f | 0x20;
					*v_p_wLmtOutPeriod = pTelBook->m_wLmtPeriod;
				}
				if( !bSkipIn )
				{
					*v_p_cTelType = *v_p_cTelType & 0xf0 | 0;
					*v_p_wLmtInPeriod = pTelBook->m_wLmtPeriod;
				}
				break;

			case 0x03: // �������ͺ���
				if( !bSkipOut )
				{
					*v_p_cTelType = *v_p_cTelType & 0x0f | 0;
					*v_p_wLmtOutPeriod = pTelBook->m_wLmtPeriod;
				}
				if( !bSkipIn )
				{
					*v_p_cTelType = *v_p_cTelType & 0xf0 | 0;
					*v_p_wLmtInPeriod = pTelBook->m_wLmtPeriod;
				}
				break;

			case 0: // ���������ͺ���
			default:
				if( !bSkipOut )
				{
					*v_p_cTelType = *v_p_cTelType & 0x0f | 0x20;
					*v_p_wLmtOutPeriod = pTelBook->m_wLmtPeriod;
				}
				if( !bSkipIn )
				{
					*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x02;
					*v_p_wLmtInPeriod = pTelBook->m_wLmtPeriod;
				}
			}

			break;
		}
	}

	return 0;
}
#endif


#if 1==USE_TELLIMIT
//--------------------------------------------------------------------------------------------------------------------------
// ���ݵ绰���жϵ绰����
int CPhoneWcdma::JugTelType3(char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType,
	WORD* v_p_wLmtOutPeriod, WORD* v_p_wLmtInPeriod)
{
	*v_p_cTelType = 0x22; // ��ʼ�����ֵΪ��ֹ�������
	*v_p_wLmtOutPeriod = * v_p_wLmtInPeriod = 0; // ��ʼ��Ϊ����ʱ

	int iRet = 0;
	char* pos = NULL;
	bool bSkipOut = false, bSkipIn = false;
	BYTE byt = 0;
	char szJugTel[16] = { 0 };
	char szCompTel[16] = { 0 };
	

	// ��������ж�
	if( NULL == v_szTel || v_ucTelLen > 15 || v_ucTelLen < 3 )
	{
		*v_p_cTelType |= 0x22; // ��ֹ�������
		return ERR_PAR;
	}

	// ȥ����������β�ո�
	memcpy( szJugTel, v_szTel, v_ucTelLen < sizeof(szJugTel) ? v_ucTelLen : sizeof(szJugTel) );
	szJugTel[ sizeof(szJugTel) - 1 ] = 0;
	pos = strchr( szJugTel, 0x20 );
	if( pos ) *pos = 0;

	// ��ȡ����
// 	tag2QCallCfg objCallCfg;
// 	objCallCfg.Init();
// 	tag2QTelBookPar* pTelBook = NULL;
// 	iRet = GetSecCfg( &objCallCfg, sizeof(objCallCfg),
// 		offsetof(tagSecondCfg, m_uni2QCallCfg.m_obj2QCallCfg), sizeof(objCallCfg) );

	tag2QServCodeCfg objServCodeCfg;
	objServCodeCfg.Init();
	iRet = GetSecCfg( &objServCodeCfg, sizeof(objServCodeCfg),
		offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(objServCodeCfg) );
	
	BookCfg cfg = {0};
	uint offset = offsetof(tagSecondCfg, m_uniBookCfg.m_szReservered);
	GetSecCfg(&cfg, sizeof(cfg), offset, sizeof(cfg) );

// 	tag2QServCodeCfg cfg2 = {0};
// 	offset = offsetof(tagSecondCfg,m_uni2QServCodeCfg.m_obj2QServCodeCfg);
// 	GetSecCfg( &cfg2, sizeof(cfg2), offset, sizeof(cfg2) );

	// (1)����,���������ж�

	// �������������ж� (����)
	memset( szCompTel, 0, sizeof(szCompTel) );
	memcpy( szCompTel, objServCodeCfg.m_aryLsnHandtel[0],
		sizeof(objServCodeCfg.m_aryLsnHandtel[0]) < sizeof(szCompTel)
		? sizeof(objServCodeCfg.m_aryLsnHandtel[0])
		: sizeof(szCompTel) );
	szCompTel[ sizeof(szCompTel) - 1 ] = 0;
	pos = strchr( szCompTel, 0x20 );
	if( pos ) *pos = 0;
	if( objServCodeCfg.m_bytListenTelCount > 0 && 0 == strcmp(szJugTel, szCompTel) )
	{
		*v_p_cTelType = *v_p_cTelType & 0x0f | 0x10; // �޸�Ϊ��̨������������(����)
		bSkipOut = true;
	}

	// ���ļ��������ж� (����)
	for( byt = 0; byt < objServCodeCfg.m_bytListenTelCount; byt ++ )
	{
		memset( szCompTel, 0, sizeof(szCompTel) );
		memcpy( szCompTel, objServCodeCfg.m_aryLsnHandtel[ byt ],
			sizeof(objServCodeCfg.m_aryLsnHandtel[0]) < sizeof(szCompTel)
			? sizeof(objServCodeCfg.m_aryLsnHandtel[0])
			: sizeof(szCompTel) );
		szCompTel[ sizeof(szCompTel) - 1 ] = 0;
		pos = strchr( szCompTel, 0x20 );
		if( pos ) *pos = 0;

		if( 0 == strcmp(szJugTel, szCompTel) )
		{
			*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x01; // �޸�Ϊ����������������(����)
			bSkipIn = true;
			break;
		}
	}

// 	// �ȼ���Ƿ��Ǽ����绰
// 	for(int i=0; i<min(8,cfg2.m_bytListenTelCount); i++) {
// 		if( 0 == memcmp(&cfg2.m_aryLsnHandtel[i][0], tel, len) ) {
// 			*type = 0x11;
// 			*outtime = 0;
// 			*intime = 0;			
// 			return true;
// 		}
// 	}

	// (2)���ķ���绰�����ж�
	for( byt = 0; byt < sizeof(cfg.tel) / sizeof(cfg.tel[0]); byt ++ )
	{
		memset( szCompTel, 0, sizeof(szCompTel) );
		memcpy( szCompTel, cfg.tel[byt],
			sizeof(cfg.tel[0]) < sizeof(szCompTel) ? sizeof(cfg.tel[0]) : sizeof(szCompTel) );
		szCompTel[ sizeof(szCompTel) - 1 ] = 0;
		pos = strchr( szCompTel, 0x20 );
		if( pos ) *pos = 0;

		if( 0 == strcmp(szJugTel, szCompTel) )
		{
			if( !bSkipOut )
			{
				*v_p_cTelType = *v_p_cTelType & 0x0f | 0x30; // �޸�Ϊ���ķ������(����)
				bSkipOut = true;
			}
			if( !bSkipIn )
			{
				*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x03; // �޸�Ϊ���ķ������(����)
				bSkipIn = true;
			}
			break;
		}
	}

// 	// �ȼ�����ķ������绰��
// 	for(i=0; i<5; i++) {
// 		if( 0 == memcmp(&cfg.tel[i][0], tel, len) ) {
// 			*type = 0x33;
// 			*outtime = 0;
// 			*intime = 0;			
// 			return true;
// 		}
// 	}

// 
// 	// (3)UDP����绰�����ж� (���漰����)
// 	for( byt = 0; byt < objServCodeCfg.m_bytUdpAtvTelCount; byt ++ )
// 	{
// 		memset( szCompTel, 0, sizeof(szCompTel) );
// 		memcpy( szCompTel, objServCodeCfg.m_aryUdpAtvTel[ byt ],
// 			sizeof(objServCodeCfg.m_aryUdpAtvTel[0]) < sizeof(szCompTel)
// 			? sizeof(objServCodeCfg.m_aryUdpAtvTel[0])
// 			: sizeof(szCompTel) );
// 		szCompTel[ sizeof(szCompTel) - 1 ] = 0;
// 		pos = strchr( szCompTel, 0x20 );
// 		if( pos ) *pos = 0;
// 
// 		if( 0 == strcmp(szJugTel, szCompTel) )
// 		{
// 			if( !bSkipIn )
// 			{
// 				*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x04; // �޸�ΪUDP�������(����)
// 				bSkipIn = true;
// 			}
// 			break;
// 		}
// 	}

	
	// (3)���,���뱾�ж�

// 	// ���ͳһ�����˺�������
// 	if( 0 == (objCallCfg.m_bytSetType & 0x0c) && !bSkipOut ) // ��ͳһ����Ϊ�������
// 	{
// 		*v_p_cTelType = *v_p_cTelType & 0x0f | 0;
// 		bSkipOut = true;
// 	}
// 	else if( 0x04 == (objCallCfg.m_bytSetType & 0x0c) && !bSkipOut ) // ��ͳһ����Ϊ��ֹ����
// 	{
// 		*v_p_cTelType = *v_p_cTelType & 0x0f | 0x20;
// 		bSkipOut = true;
// 	}
// 	if( 0 == (objCallCfg.m_bytSetType & 0x03) && !bSkipIn  ) // ��ͳһ����Ϊ�������
// 	{
// 		*v_p_cTelType = *v_p_cTelType & 0xf0 | 0;
// 		bSkipIn = true;
// 	}
// 	else if( 0x01 == (objCallCfg.m_bytSetType & 0x03) && !bSkipIn ) // ��ͳһ����Ϊ��ֹ����
// 	{
// 		*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x02;
// 		bSkipIn = true;
// 	}

	// ���������ж�
	for( byt = 0; byt < sizeof(cfg.book) / sizeof(cfg.book[0]); byt ++ )
	{
		memset( szCompTel, 0, sizeof(szCompTel) );
		memcpy( szCompTel, cfg.book[byt].tel,
			sizeof(cfg.book[0].tel) < sizeof(szCompTel) ? sizeof(cfg.book[0].tel) : sizeof(szCompTel) );
		szCompTel[ sizeof(szCompTel) - 1 ] = 0;
		pos = strchr( szCompTel, 0x20 );
		if( pos ) *pos = 0;

		if( !strcmp(szJugTel, szCompTel) )
		{
			switch( cfg.book[byt].limit )
			{
			case 0x01: // ��������룬�������
				if( !bSkipOut )
				{
					*v_p_cTelType = *v_p_cTelType & 0x0f | 0;
					*v_p_wLmtOutPeriod = WORD(cfg.book[byt].tmhold[0]*60) + cfg.book[byt].tmhold[1];
				}
				if( !bSkipIn )
				{
					*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x02; 
					*v_p_wLmtInPeriod = WORD(cfg.book[byt].tmhold[0]*60) + cfg.book[byt].tmhold[1];
				}
				break;

			case 0x02: // ������룬�������
				if( !bSkipOut )
				{
					*v_p_cTelType = *v_p_cTelType & 0x0f | 0x20;
					*v_p_wLmtOutPeriod = WORD(cfg.book[byt].tmhold[0]*60) + cfg.book[byt].tmhold[1];
				}
				if( !bSkipIn )
				{
					*v_p_cTelType = *v_p_cTelType & 0xf0 | 0;
					*v_p_wLmtInPeriod = WORD(cfg.book[byt].tmhold[0]*60) + cfg.book[byt].tmhold[1];
				}
				break;

			case 0x03: // �������ͺ���
				if( !bSkipOut )
				{
					*v_p_cTelType = *v_p_cTelType & 0x0f | 0;
					*v_p_wLmtOutPeriod = WORD(cfg.book[byt].tmhold[0]*60) + cfg.book[byt].tmhold[1];
				}
				if( !bSkipIn )
				{
					*v_p_cTelType = *v_p_cTelType & 0xf0 | 0;
					*v_p_wLmtInPeriod = WORD(cfg.book[byt].tmhold[0]*60) + cfg.book[byt].tmhold[1];
				}
				break;

			case 0: // ���������ͺ���
			default:
				if( !bSkipOut )
				{
					*v_p_cTelType = *v_p_cTelType & 0x0f | 0x20;
					*v_p_wLmtOutPeriod = WORD(cfg.book[byt].tmhold[0]*60) + cfg.book[byt].tmhold[1];
				}
				if( !bSkipIn )
				{
					*v_p_cTelType = *v_p_cTelType & 0xf0 | 0x02;
					*v_p_wLmtInPeriod = WORD(cfg.book[byt].tmhold[0]*60) + cfg.book[byt].tmhold[1];
				}
			}

			break;
		}
	}

	return 0;

// 	// ������ͨ�绰��
// 	for(i=0; i<30; i++) {
// 		if( 0 == memcmp(cfg.book[i].tel, tel, len) ) {
// 			if(0==cfg.book[i].limit)		*type = 0x22;
// 			else if(1==cfg.book[i].limit)	*type = 0x02;
// 			else if(2==cfg.book[i].limit)	*type = 0x20;
// 			else							*type = 0x00;
// 			*outtime = *intime = cfg.book[i].tmhold[0]*60 + cfg.book[i].tmhold[1];
// 			return true;
// 		}
// 	}
// 
// 	*type = 0x22; // ʲô���붼���ǣ����ֹ�������
// 	*outtime = 0;
// 	*intime = 0;
// 	return false;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// �жϵ绰��������
// callout: true �����绰, false ����绰
// tel: �绰����
// len: �绰���볤��
// holdtime: ���ص�ͨ��ʱ��
// ����: �绰�������� A1 A2 A3 A4 B1 B2 B3 B4
// A1 A2 A3 A4�����������ͣ�0,�������; 1,��̨������������; 2,��ֹ��������; 3,���ķ���绰����;
// B1 B2 B3 B4�����������ͣ�0,�������; 1,����������������; 2,��ֹ�������; 3,���ķ���绰����; 4,UDP�������
byte CPhoneWcdma::tel_type(bool callout, char *tel, byte len, ushort *holdtime)
{
	byte type = 0;
#if 0==USE_TELLIMIT
	JugTelType(tel, len, &type);
	*holdtime = 0;
#endif
			
#if 2==USE_TELLIMIT
	ushort outtime = 0, intime = 0;
	JugTelType2(tel, len, &type, &outtime, &intime);
	*holdtime = callout ? outtime : intime;
#endif

#if 1==USE_TELLIMIT
	ushort outtime = 0, intime = 0;
	JugTelType3(tel, len, &type, &outtime, &intime);
	*holdtime = callout ? outtime : intime;
#endif

	return type;
}

// que DVR��Ƶģʽ����
// ������Ƶģʽ 
// mode: ��Ƶģʽ	0 ����
//					1 ����
//					2 ����	
// ����: �����Ƿ�ɹ�
int CPhoneWcdma::_PhoneAudioModeSet(int v_iChgToMode)
{
	DWORD dwIoSetSymb = DEV_PHONE;
	switch( v_iChgToMode )
	{
	case WCDMA_AUDIOMODE_HANDFREE: //����ģʽ
		{
			//AT+CLVL,�����������
			//////nnnnnnn
			int iResult = _SetAT(5000, 3, NULL, 0, NULL, 0, "AT+CLVL=3" );
			if( WCDMA_ATANS_OK != iResult )
			{
				return -1;
			}

			iResult = _PhoneAudioOutVolSet(0);
			if( iResult ) return -1;

			PRTMSG(MSG_DBG,"Audio Mode switch to handfree!\n");
			
 

			IOSet(IOS_EARPHONESEL, IO_EARPHONESEL_OFF, &dwIoSetSymb, sizeof(dwIoSetSymb));
			IOSet(IOS_AUDIOSEL, IO_AUDIOSEL_PHONE, NULL, 0);
			IOSet(IOS_TRUMPPOW, IO_TRUMPPOW_ON, &dwIoSetSymb, sizeof(dwIoSetSymb));
		}
		break;

	case WCDMA_AUDIOMODE_EARPHONE: //����ģʽ
		{
			int iResult = _SetAT( 5000, 3, NULL, 0, NULL, 0, "AT+CLVL=3" );
			if( WCDMA_ATANS_OK != iResult )
			{
				return -1;
			}

			iResult = _PhoneAudioOutVolSet(0);
			if( iResult ) return -1;

			PRTMSG(MSG_DBG,"Audio Mode switch to earphone!\n");

			IOSet(IOS_EARPHONESEL, IO_EARPHONESEL_ON, &dwIoSetSymb, sizeof(dwIoSetSymb));
			IOSet(IOS_AUDIOSEL, IO_AUDIOSEL_PHONE, NULL, 0);
			IOSet(IOS_TRUMPPOW, IO_TRUMPPOW_ON, &dwIoSetSymb, sizeof(dwIoSetSymb));
	
		}
		break;

	case WCDMA_AUDIOMODE_LSN: //����ģʽ
		{
			PRTMSG(MSG_DBG,"Audio Mode switch to lsn!\n");

			IOSet(IOS_TRUMPPOW, IO_TRUMPPOW_OFF, &dwIoSetSymb, sizeof(dwIoSetSymb));
			IOSet(IOS_AUDIOSEL, IO_AUDIOSEL_PHONE, NULL, 0);
			IOSet(IOS_EARPHONESEL, IO_EARPHONESEL_OFF, &dwIoSetSymb, sizeof(dwIoSetSymb));

// Ӳ���ϲ�֧�ֶ��ֻ�ģ�������˫ͨ���л�,����ͨ���ֻ�ģ�����Χ��·ͨ���л�ʵ�ֵ�
// 			int iResult = _SetAT( 5000, 3, NULL, 0, NULL, 0, "AT^DAUDO=2,5" );
// 			if( WCDMA_ATANS_OK != iResult )
// 			{
// 				return -1;
// 			}
// 
// 			iResult = _SetAT( 5000, 3, NULL, 0, NULL, 0, "AT^DAUDI=1,2" );
// 			if( WCDMA_ATANS_OK != iResult )
// 			{
// 				return -1;
// 			}

// 		if(!_SetAT(5, 1, 0, "AT$AUVOL=0\r\n"))			return false;	// vol ����
// 		if(!_SetAT(5, 1, 0, "AT$AUGAIN=1,9\r\n"))		return false;	// mic ����
		}
		break;

	case WCDMA_AUDIOMODE_DIAL: // ����ģʽ(�Ǽ�������)
		{
			PRTMSG(MSG_DBG,"Audio Mode switch to dial!\n");

			// (�����������Ტ��ָ��)

			IOSet(IOS_AUDIOSEL, IO_AUDIOSEL_PHONE, NULL, 0);
			IOSet(IOS_TRUMPPOW, IO_TRUMPPOW_ON, &dwIoSetSymb, sizeof(dwIoSetSymb));
		}
		break;

	case WCDMA_AUDIOMODE_CLOSE: // ͨ���ر�ģʽ
		{
			PRTMSG(MSG_DBG,"Audio Mode switch to close!\n");

			IOSet(IOS_TRUMPPOW, IO_TRUMPPOW_OFF, &dwIoSetSymb, sizeof(dwIoSetSymb));
			IOSet(IOS_AUDIOSEL, IO_AUDIOSEL_PC, NULL, 0);
			// (�����������Ტ��ָ��)
		}
		break;

	default:
		return -1; // ����return
	}

	tagWcdmaPhoneStaCommon objSta = _PhoneStaCommonGet(0);
	objSta.m_bytCurAudioMode = v_iChgToMode;
	_PhoneStaCommonSet( objSta, WCDMA_PHSTVAR_AUDIOMODE);

	return 0;
}

int CPhoneWcdma::_PhoneAudioOutVolSet(int v_iChgDegree)
{
	tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
	
	objPhoneSta.m_iAudioOutVol += v_iChgDegree;
	if( objPhoneSta.m_iAudioOutVol < 0 ) objPhoneSta.m_iAudioOutVol = 0;
	if( objPhoneSta.m_iAudioOutVol > 4 ) objPhoneSta.m_iAudioOutVol = 4;
//m_iAudioOutVol==0ʱ��Ϊ��������ע��
	int iResult = _SetAT( 2500, 2, NULL, 0, NULL, 0, "AT+CLVL=%d", objPhoneSta.m_iAudioOutVol );

	if( WCDMA_ATANS_OK == iResult )
	{
		_PhoneStaCommonSet( objPhoneSta, WCDMA_PHSTVAR_AUDIOOUTLVL );
		return 0;
	}
	else
	{
		return -1;
	}
}

//-----------------------------------------------------------------------------------------------------------------
// ���Ͷ���(�������͵Ķ���ѹ�뷢�Ͷ���)
// to: Ŀ���ֻ����ַ���ָ��
// tolen: Ŀ���ֻ����ַ�������
// msg: ������������ָ��
// msglen: �����������ݳ���
// ����: 1 �ɹ�
//		 2 ʧ��
//		 3 ��������̫��
int CPhoneWcdma::_PushSndSms(char *to, byte tolen, char *msg, byte msglen, BYTE v_bytSrcType)
{
	// �ж϶�����ַ�������
//	if(m_simlack)								{ return 2; }
	if(tolen==0 || tolen>15 || msglen==0)		{ return 2;	}
	if(msglen>160)								{ return 3; }

	//ȥ������ǰ��"+"
	char tel[16] = {0};
	if(to[0]=='+')		memcpy(tel, to+1, min(tolen-1,16));
	else 				memcpy(tel, to, min(tolen,16));

	//�ں���ǰ��"86"
	if(tel[0]!='8' && tel[1]!='6')
	{
		char tmp[16] = {0};
		tmp[0] = '8';	
		tmp[1] = '6';
		memcpy(tmp+2, tel, min(strlen(tel),16));
		memset(tel, 0, 16);
		memcpy(tel, tmp, min(strlen(tmp),16));
	}
	tel[ sizeof(tel) - 1 ] = 0;

	// �����Ŵ��뷢�Ͷ���
	DWORD dwPktNum = 0;
	//que lvl 5->1
	int iResult = m_objDataMngSmsSnd.PushData(1, msglen, msg, dwPktNum, v_bytSrcType, tel, strlen(tel) );

	return iResult ? 2 : 1;

// 	uchar tomsg[16+161] = {0};
// 	memcpy(tomsg, tel, 16);
// 	memcpy(tomsg+16, msg, min(msglen,161));
// 	if(m_stack.push(tomsg, 16+161))	return 1;
// 	else							return 2;
}

//-----------------------------------------------------------------------------------------------------------------
// ������ŵĺ���
// psm: ���ŵĽṹָ��
bool CPhoneWcdma::sms_deal(SM_PARAM *psm)
{
	// ��δ�����Ų�����
	if( 0 != psm->type )
	{
		return true;
	}

	byte len1 = min(strlen(psm->tpa),16);
	byte len2 = min(strlen(psm->ud),161);
	if( 0 == len1 )	
	{ 
		PRTMSG(MSG_ERR,"Err's SM: tpa=0\n"); 
		return false; 
	}
	if( 0 ==len2 )	
	{ 
		PRTMSG(MSG_ERR,"Err's SM: ud=0\n"); 
		return false; 
	}

	char buf[1024] = {0};
	ushort buflen = 0;
	unsigned char uszResult;

	// ����6ת8,Ȼ�����У����ж��Ƿ�����ý�����
	buflen = min(convert6to8(psm->ud, buf, len2),1024);
	if(buflen>0) {
		if(check_crc1((uchar*)buf, buflen))	{
			PRTMSG(MSG_DBG,"Rcv Liu SM!\n");
			buf[0] = 0x37;	//��������
			DataPush(buf, buflen+1, DEV_PHONE, DEV_UPDATE, 2);	//������ý��
			return true;
		}
	}

	// ����6ת7,Ȼ�����У����ж��Ƿ���ǧ���۶���
	memset(buf, 0, 1024);
	buflen = min(convert6to7(psm->ud, buf+1, len2),1024);
	if(buflen>0) {
		if(check_crc2((uchar*)(buf+1), buflen))	{
			PRTMSG(MSG_DBG,"Rcv Qian SM!!\n");
			buf[0] = 0x37;	//��������
			DataPush(buf, buflen+1, DEV_PHONE, DEV_QIAN, 2);	//����ǧ����
			return true;
		}
	}

	// ����Ƿ�����ͨ���ö���(ͨ���ֻ�ֱ�����õĶ���)
	UserSM *pusm = (UserSM*)psm->ud;
	if( pusm->add1=='+' && pusm->add2=='+' && pusm->add3=='+' && pusm->add4=='+' && pusm->add5=='+' && pusm->add6=='+' && pusm->add7=='+'
		&& pusm->add8 =='+' && pusm->add9=='+' && pusm->add10=='+' && pusm->add11=='+' ) 
	{
		PRTMSG(MSG_DBG, "Rcv Cfg SM!\n");

		tag1PComuCfg cfg;
		GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) );
		tag1QIpCfg cfg2[2];
		GetImpCfg( &cfg2, sizeof(cfg2), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg2) );
		tag1LComuCfg cfg3;
		GetImpCfg( &cfg3, sizeof(cfg3), offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg3) );

		// ����ֻ���
		memset(cfg.m_szTel, 0, sizeof(cfg.m_szTel) );
		memcpy(cfg.m_szTel, pusm->mytel, sizeof(pusm->mytel));

		// ���ǧ���۲���
		DWORD ipScan[4] = {0};
		DWORD ip = 0;
		sscanf(pusm->ip1, "%03d%03d%03d%03d", ipScan, ipScan+1, ipScan+2, ipScan+3);

		ip = ipScan[0] | ipScan[1]<<8 | ipScan[2]<<16 | ipScan[3]<<24;
		cfg2[0].m_ulQianTcpIP = ip;
		cfg2[0].m_usQianTcpPort = htons( (ushort)atol(pusm->port1) );

		sscanf(pusm->ip2, "%03d%03d%03d%03d", ipScan, ipScan+1, ipScan+2, ipScan+3);
		ip = ipScan[0] | (ipScan[1]<<8) | (ipScan[2]<<16 | (ipScan[3]<<24) );
		cfg2[0].m_ulQianUdpIP = ip;
		cfg2[0].m_usQianUdpPort = htons( (ushort)atol(pusm->port2) );

		// ���DVR����
		sscanf(pusm->ip3, "%03d%03d%03d%03d", ipScan, ipScan+1, ipScan+2, ipScan+3);
		ip = ipScan[0] | (ipScan[1]<<8) | (ipScan[2]<<16 | (ipScan[3]<<24) );
		cfg2[1].m_ulQianTcpIP = ip;
		cfg2[1].m_usQianTcpPort = htons( (ushort)atol(pusm->port3) );

		// �����ý�����
		sscanf(pusm->ip4, "%03d%03d%03d%03d", ipScan, ipScan+1, ipScan+2, ipScan+3);
		ip = ipScan[0] | (ipScan[1]<<8) | (ipScan[2]<<16 | (ipScan[3]<<24) );
		cfg3.m_ulLiuIP = ip;
		cfg3.m_usLiuPort = htons( (ushort)atol(pusm->port4) );

		// ���ά�����Ĳ���
		sscanf(pusm->ip5, "%03d%03d%03d%03d", ipScan, ipScan+1, ipScan+2, ipScan+3);
		ip = ipScan[0] | (ipScan[1]<<8) | (ipScan[2]<<16 | (ipScan[3]<<24) );
		cfg3.m_ulLiuIP2 = ip;
		cfg3.m_usLiuPort2 = htons( (ushort)atol(pusm->port5) );
		
		// ���APN
		memset(cfg2[0].m_szApn, 0, sizeof(cfg2[0].m_szApn) );
		memcpy(cfg2[0].m_szApn, pusm->apn, strlen(pusm->apn));

		// ��������
		bool succ = true;
		if(SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) ))		succ = false;
		if(SetImpCfg(&cfg2, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg2) ))		succ = false;
		if(SetImpCfg(&cfg3, offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg3) ))		succ = false;


		// ֪ͨ��ý���ǧ���۲����б仯
		char frm1 = 0x43;	
		DataPush(&frm1, 1, DEV_PHONE, DEV_UPDATE, 2);
		Frm35 frm2;			
		frm2.reply = 0x01;
		DataPush((char*)&frm2, sizeof(frm2), DEV_PHONE, DEV_SOCK, 2);
		
		//֪ͨDvrExe�ֻ�����ı���
		char by = 0x0c;
		DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);

		// ���ֻ���������Ӧ��
		char msg[1024] = {0};

		// ��ӳ�̨�ֻ��ź�����汾
		sprintf(msg, "Set %.15s Succ! Ver:", cfg.m_szTel);
		GetAppVer( msg+strlen(msg), sizeof(msg)-strlen(msg) - 200 );

		in_addr addr1, addr2, addr3, addr4, addr5;
		addr1.s_addr = cfg2[0].m_ulQianTcpIP;
		addr2.s_addr = cfg2[0].m_ulQianUdpIP;
		addr3.s_addr = cfg2[1].m_ulQianTcpIP;
		addr4.s_addr = cfg3.m_ulLiuIP;
		addr5.s_addr = cfg3.m_ulLiuIP2;

		char ip1[32] = {0};
		char ip2[32] = {0};
		char ip3[32] = {0};
		char ip4[32] = {0};
		char ip5[32] = {0};

		strcpy( ip1, inet_ntoa(addr1) );
		strcpy( ip2, inet_ntoa(addr2) );
		strcpy( ip3, inet_ntoa(addr3) );
		strcpy( ip4, inet_ntoa(addr4) );
		strcpy( ip5, inet_ntoa(addr5) );

		// ���ǧ���ۺ���ý���IP�Ͷ˿�
		sprintf(msg+strlen(msg), " Q:%s,%d;%s,%d;%s,%d", ip1, ntohs(cfg2[0].m_usQianTcpPort),
			ip2, ntohs(cfg2[0].m_usQianUdpPort), ip3, ntohs(cfg2[1].m_usQianTcpPort) );
		sprintf( msg+strlen(msg), " L:%s,%d", ip4, ntohs(cfg3.m_usLiuPort) );
		sprintf( msg+strlen(msg), " M:%s,%d", ip5, ntohs(cfg3.m_usLiuPort2) );
		sprintf(msg+strlen(msg), " A:%s", cfg2[0].m_szApn);

		_PushSndSms( psm->tpa, strlen(psm->tpa), msg, min(strlen(msg), 160), 0 );

		// ���GSM�źš�GPRS���������ACC״̬��GPS��λ���
		memset( msg, 0, sizeof(msg) );
		tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
		GPSDATA gps(0);
		GetCurGps( &gps, sizeof(gps), GPS_LSTVALID );
		sprintf(msg+strlen(msg), " Sig:%d", objPhoneSta.m_iSignal);
		sprintf(msg+strlen(msg), " NET:%s", objPhoneSta.m_bNetConnected ? "on" : "off");
		IOGet((byte)IOS_ACC,&uszResult);
		sprintf(msg+strlen(msg), " Acc:%s", IO_ACC_ON == uszResult ? "on" : "off");
		sprintf(msg+strlen(msg), " Gps:%c", gps.valid);
		
		_PushSndSms( psm->tpa, strlen(psm->tpa), msg, min(strlen(msg), 160), 0 );	//Ŀǰ strlen(msg)==149
		return true;
	}

	UserSM2 *pusm2 = (UserSM2*)psm->ud;

	// ����Ƿ�����ͨ��ѯ����(ͨ���ֻ�ֱ�Ӳ�ѯ�Ķ���)
	if( 0==strncmp(pusm2->pass,"4312896654",10) && 0==strncmp(pusm2->cmd,"+query",6) ) 
	{
		PRTMSG(MSG_DBG, "Rcv Query SM!!\n");

		tag1PComuCfg cfg;
		GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) );
		tag1QIpCfg cfg2[2];
		GetImpCfg( &cfg2, sizeof(cfg2), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg2) );
		tag1LComuCfg cfg3;
		GetImpCfg( &cfg3, sizeof(cfg3), offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg3) );

		char msg[1024] = {0};

		// ��ӳ�̨�ֻ��ź�����汾
		sprintf(msg, "%.15s Ver:", cfg.m_szTel);
		GetAppVer( msg+strlen(msg), sizeof(msg)-strlen(msg) - 200);

		in_addr addr1, addr2, addr3, addr4, addr5;
		addr1.s_addr = cfg2[0].m_ulQianTcpIP;
		addr2.s_addr = cfg2[0].m_ulQianUdpIP;
		addr3.s_addr = cfg2[1].m_ulQianTcpIP;
		addr4.s_addr = cfg3.m_ulLiuIP;
		addr5.s_addr = cfg3.m_ulLiuIP2;

		char ip1[32] = {0};
		char ip2[32] = {0};
		char ip3[32] = {0};
		char ip4[32] = {0};
		char ip5[32] = {0};

		strcpy( ip1, inet_ntoa(addr1) );
		strcpy( ip2, inet_ntoa(addr2) );
		strcpy( ip3, inet_ntoa(addr3) );
		strcpy( ip4, inet_ntoa(addr4) );
		strcpy( ip5, inet_ntoa(addr5) );
		
		// ���ǧ���ۺ���ý���IP�Ͷ˿�
		sprintf(msg+strlen(msg), " Q:%s,%d;%s,%d;%s,%d", ip1, ntohs(cfg2[0].m_usQianTcpPort),
			ip2, ntohs(cfg2[0].m_usQianUdpPort), ip3, ntohs(cfg2[1].m_usQianTcpPort) );
		sprintf(msg+strlen(msg), " L:%s,%d", ip4, ntohs(cfg3.m_usLiuPort));
		sprintf(msg+strlen(msg), " M:%s,%d", ip5, ntohs(cfg3.m_usLiuPort2));
		sprintf(msg+strlen(msg), " A:%s", cfg2[0].m_szApn);

		_PushSndSms( psm->tpa, strlen(psm->tpa), msg, min(strlen(msg), 160), 0 );

		char tpa[50] = { 0 };
		memcpy( tpa, psm->tpa, min( strlen(psm->tpa), 25) );
		PRTMSG( MSG_NOR, "Send Query Res Msg to %s: \"%s\"\n", tpa, msg );
		
		return true;
	}
	
	// ����Ƿ���״̬��ѯ����(ͨ���ֻ���ѯ״̬�Ķ���)
	if( 0==strncmp(pusm2->pass,"4312896654",10) && 0==strncmp(pusm2->cmd,"+state",6) ) 
	{
		PRTMSG(MSG_DBG, "Rcv State SM!!\n");

		char msg[1024] = {0};

		// ���GSM�źš�GPRS���������ACC״̬��GPS��λ���
		tagWcdmaPhoneStaCommon objPhoneSta = _PhoneStaCommonGet(0);
		sprintf(msg+strlen(msg), " Sig:%d", objPhoneSta.m_iSignal);
		sprintf(msg+strlen(msg), " Net:%s", objPhoneSta.m_bNetConnected ? "on" : "off");
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		sprintf(msg+strlen(msg), " Tcp:%s", g_objLightCtrl2.m_bytTcpState ? "on" : "off");
#endif
		IOGet((byte)IOS_ACC,&uszResult);
		sprintf(msg+strlen(msg), " Acc:%s", IO_ACC_ON == uszResult ? "on" : "off");
		GPSDATA gps(0);
		GetCurGps( &gps, sizeof(gps), GPS_LSTVALID );
		sprintf(msg+strlen(msg), " Gps:%c", gps.valid);	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		IOGet((byte)IOS_POWDETM, &uszResult);
		sprintf(msg+strlen(msg), " Pow:%s", IO_POWDETM_VALID == uszResult ? "on" : "off");	
		IOGet((byte)IOS_QIANYA, &uszResult);
		sprintf(msg+strlen(msg), " Vol:%s", IO_QIANYA_INVALID == uszResult ? "on" : "off");	
		sprintf(msg+strlen(msg), " Sys:%s", g_objLightCtrl2.m_bytDvrState == 0x00 ? "sleep" : 
																						(g_objLightCtrl2.m_bytDvrState == 0x01 ? "idle" : 
																							(g_objLightCtrl2.m_bytDvrState == 0x02 ? "work" : 
																								(g_objLightCtrl2.m_bytDvrState == 0x03 ? "ctrl" : "idle"))));
			
		//���GPSģ�顢�ֻ�ģ�顢TW2865ģ�顢SD��״̬
		sprintf(msg+strlen(msg), " DevErr:0x%08x", g_objLightCtrl2.m_dwDevErrCode);
		sprintf(msg+strlen(msg), " CfgErr:0x%08x", g_objLightCtrl2.m_dwCfgErrCode);
		sprintf(msg+strlen(msg), " TelErr:0x%08x", g_objLightCtrl2.m_dwTelErrCode);
		sprintf(msg+strlen(msg), " MonErr:0x%08x", g_objLightCtrl2.m_dwMonErrCode);
			
		_PushSndSms( psm->tpa, strlen(psm->tpa), msg, min(strlen(msg), 160), 0 );
		
		memset( msg, 0, sizeof(msg) );
		sprintf(msg+strlen(msg), " ReBoot:%s", g_objLightCtrl2.m_szReBootTime);
		sprintf(msg+strlen(msg), " SwiLink:%s", g_objLightCtrl2.m_szSwiLinkTime);
		sprintf(msg+strlen(msg), " DiskLink:%s", g_objLightCtrl2.m_szDiskLinkTime);
		sprintf(msg+strlen(msg), " MonLink:%s", g_objLightCtrl2.m_szMonLinkTime);
		sprintf(msg+strlen(msg), " Deal3801:%s(%d)", g_objLightCtrl2.m_szDeal3801Time, g_objLightCtrl2.m_iDeal3801Cnt);
		sprintf(msg+strlen(msg), " VideoMon:%s(%d)", g_objLightCtrl2.m_szVideoMonTime, g_objLightCtrl2.m_iVideoMonCnt);
		
		_PushSndSms( psm->tpa, strlen(psm->tpa), msg, min(strlen(msg), 160), 0 );
		
		memset( msg, 0, sizeof(msg) );
		sprintf(msg+strlen(msg), " TelOn:%s(%d)", g_objLightCtrl2.m_szTelOnTime, g_objLightCtrl2.m_iTelOnCnt);
		sprintf(msg+strlen(msg), " TelInit:%s(%d)", g_objLightCtrl2.m_szTelInitTime, g_objLightCtrl2.m_iTelInitCnt);
		sprintf(msg+strlen(msg), " LowSig:%s(%d)", g_objLightCtrl2.m_szLowSigTime, g_objLightCtrl2.m_iLowSigCnt);
		sprintf(msg+strlen(msg), " DialNet:%s(%d)", g_objLightCtrl2.m_szDialNetTime, g_objLightCtrl2.m_iDialNetCnt);
		sprintf(msg+strlen(msg), " TcpCon:%s(%d)", g_objLightCtrl2.m_szTcpConTime, g_objLightCtrl2.m_iTcpConCnt);
		sprintf(msg+strlen(msg), " LogIn:%s(%d)", g_objLightCtrl2.m_szLogInTime, g_objLightCtrl2.m_iLogInCnt);

#endif
// 		// ��ӳ�̨ID����Կ
// 		char str[21] = {0};
// 		GetDevID(str, sizeof(str)); 
// 		sprintf(msg+strlen(msg), " Id:%d", *(ulong*)str);
// 		sprintf(msg+strlen(msg), " Key:%.16s", str+4);
		
		_PushSndSms( psm->tpa, strlen(psm->tpa), msg, min(strlen(msg), 160), 0 );

		char tpa[50] = { 0 };
		memcpy( tpa, psm->tpa, min( strlen(psm->tpa), 25) );
		PRTMSG( MSG_NOR, "Send State Res Msg to %s: \"%s\"\n", tpa, msg );
		
		return true;
	}

	// ����Ƿ��Ǹ�λϵͳ�Ķ���(ͨ���ֻ���λ�Ķ���)
	if( 0==strncmp(pusm2->pass,"4312896654",10) && 0==strncmp(pusm2->cmd,"+reset",6) ) 
	{
		PRTMSG(MSG_DBG,"Rcv SysReset SM!!\n");
		G_ResetSystem();//���߳��ڲ�����λ���˳����̣�
		return true;
	}

	// ����Ƿ�ָ��������õĶ���
	if( 0==strncmp(pusm2->pass,"4312896654",10) && 0==strncmp(pusm2->cmd,"+resume",7) ) 
	{
		PRTMSG(MSG_DBG,"Rcv ResumeCfg SM!!\n");
		
		// ���ֻ���������Ӧ��
		char msg[1024] = {0};

		if( 0 == ResumeCfg(true, true) )
		{		
			g_objDiaodu.show_diaodu("�ֻ����Żָ��������óɹ���");
			
			// ֪ͨ��ý���ǧ���۲����б仯
			char frm1 = 0x43;	
			DataPush(&frm1, 1, DEV_PHONE, DEV_UPDATE, 2);
			Frm35 frm2;			
			frm2.reply = 0x01;
			DataPush((char*)&frm2, sizeof(frm2), DEV_PHONE, DEV_SOCK, 2);
			
			//֪ͨDvrExe�ֻ�����ı���
			char by = 0x0c;
			DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
			
			tag1PComuCfg cfg;
			GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) );
			tag1QIpCfg cfg2[2];
			GetImpCfg( &cfg2, sizeof(cfg2), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg2) );
			tag1LComuCfg cfg3;
			GetImpCfg( &cfg3, sizeof(cfg3), offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg3) );

			// ��ӳ�̨�ֻ��ź�����汾
			sprintf(msg, "ResumeCfg %.15s Succ! Ver:", cfg.m_szTel);
			GetAppVer( msg+strlen(msg), sizeof(msg)-strlen(msg) - 200 );
			
			in_addr addr1, addr2, addr3, addr4, addr5;
			addr1.s_addr = cfg2[0].m_ulQianTcpIP;
			addr2.s_addr = cfg2[0].m_ulQianUdpIP;
			addr3.s_addr = cfg2[1].m_ulQianTcpIP;
			addr4.s_addr = cfg3.m_ulLiuIP;
			addr5.s_addr = cfg3.m_ulLiuIP2;
			
			char ip1[32] = {0};
			char ip2[32] = {0};
			char ip3[32] = {0};
			char ip4[32] = {0};
			char ip5[32] = {0};
			
			strcpy( ip1, inet_ntoa(addr1) );
			strcpy( ip2, inet_ntoa(addr2) );
			strcpy( ip3, inet_ntoa(addr3) );
			strcpy( ip4, inet_ntoa(addr4) );
			strcpy( ip5, inet_ntoa(addr5) );
			
			// ���ǧ���ۺ���ý���IP�Ͷ˿�
			sprintf(msg+strlen(msg), " Q:%s,%d;%s,%d;%s,%d", ip1, ntohs(cfg2[0].m_usQianTcpPort),
				ip2, ntohs(cfg2[0].m_usQianUdpPort), ip3, ntohs(cfg2[1].m_usQianTcpPort) );
			sprintf( msg+strlen(msg), " L:%s,%d", ip4, ntohs(cfg3.m_usLiuPort) );
			sprintf( msg+strlen(msg), " M:%s,%d", ip5, ntohs(cfg3.m_usLiuPort2) );
			sprintf(msg+strlen(msg), " A:%s", cfg2[0].m_szApn);		
		}
		else
		{
			sprintf(msg, "ResumeCfg Failed!");
		}

		_PushSndSms( psm->tpa, strlen(psm->tpa), msg, min(strlen(msg), 160), 0 );

		return true;
	}

	// ������,����Ϊ���û�����,����������
	PRTMSG(MSG_DBG,"User's SM!\n");
	char frm[1024] = {0};
	int p = 0;
	DWORD dwPktNum;
	frm[p] = 0x33;					p++;		//���ն���Ϣָʾ
	if( psm->tpa[0] == '8' && psm->tpa[1] == '6' )
	{
		frm[p] = len1-2;					p++;		//�����ֻ����볤��
		memcpy(frm+p, psm->tpa+2, len1-2);	p+=len1-2;	//�����ֻ�����
	}
	else if( psm->tpa[0] == '+' && psm->tpa[1] == '8' && psm->tpa[2] == '6' )
	{
		frm[p] = len1-3;					p++;
		memcpy(frm+p, psm->tpa+3, len1-3);	p+=len1-3;
	}
	else
	{
		frm[p] = len1;					p++;
		memcpy(frm+p, psm->tpa, len1);	p+=len1;
	}
	memcpy(frm+p, psm->scts, 12);	p+=12;		//����ʱ��
	frm[p] = len2;					p++;		//����Ϣ���ݳ���
	memcpy(frm+p, psm->ud, len2);	p+=len2;	//����Ϣ����
	g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, p, frm, dwPktNum);	//����������

	return true;
}

//�ֻ�ģ�鲦������
int CPhoneWcdma::_PhoneDialNet()
{	
	int iRet;
	if (SIG_ERR == G_Signal(SIGUSR1, G_WCDMA_PppSigHandler) )
	{
		PRTMSG(MSG_ERR,"failed to set the SIGUSR1 handler function for ppp dial\n");
		return ERR_SIG;
	}
	
	// ��ȡAPN
	char szApn[100] = { 0 };
	tag1QIpCfg objIpCfg;
	int iResult = GetImpCfg( &objIpCfg, sizeof(objIpCfg),
		offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg[0]),sizeof(objIpCfg) );
	if( !iResult )
	{
		strncpy( szApn, objIpCfg.m_szApn, min(strlen(objIpCfg.m_szApn), sizeof(szApn) - 1) );
	}
	else
	{
		strcpy( szApn, "CMNET" );
	}

	_SetAT( 1000, 3, NULL, 0, NULL, 0, "AT+CGDCONT=1,\"IP\",\"%s\"", szApn );
	//_SetAT( 1000, 3, NULL, 0, NULL, 0, "AT+CGDCONT=1,\"IP\",\"CMNET\"" );
	
	if (0 != _ForkPppd())
	{
		PRTMSG(MSG_NOR,"failed to fork pppd!\n");
		return ERR_THREAD;
	}
	
	return 0;
}

//�������Ž���pppd
int CPhoneWcdma::_ForkPppd()
{
	int iRet;
	pid_t pidPppd;
	char szUsbSerialPathName[256] = {0};
	struct dirent **ppUsbSerialNameList;
	scandir(USB_DEV_CHECKPATH_PPP, &ppUsbSerialNameList, 0, alphasort);
	sprintf(szUsbSerialPathName, "/dev/usb/tts/%c", *(ppUsbSerialNameList[11]->d_name + 6));
	if( 0 != access(szUsbSerialPathName, F_OK) )
	{
		PRTMSG(MSG_ERR,"usb device port for ppp check err\n");
		return ERR_COM;
	}
	pidPppd = fork();
	switch(pidPppd)
	{
		case -1:
			PRTMSG(MSG_ERR,"failed to fork a new process\n");
			return ERR_THREAD;
			
		case 0:
			iRet = execl("/etc/ppp/ppp-on", "ppp-on",szUsbSerialPathName, PPP_DIAL_NUM, PPP_USER_NAME, PPP_PASSWORD, NULL);
			if (-1 == iRet)
			{
				PRTMSG(MSG_ERR,"failed to execl a new program\n");
				return ERR_THREAD;		
			}
			break;
			
		default:
			PRTMSG(MSG_NOR,"fork a new process succ\n");

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
			g_objLightCtrl2.SetLightSta(0x07);	// ��ʼ����֪ͨ
#endif
			sleep(5);

			break;
	}
	
	return 0;
}

int CPhoneWcdma::_PppSigHandler()
{
	m_bPhoneNetReady = true;
}


void CPhoneWcdma::_PhoneCutNet()
{	
	m_bPhoneNetReady = false;
	m_bNeedDialNet = true;
	PRTMSG(MSG_NOR,"Cut Net...\n");
	system("killall pppd");	//kill pppd���Ž��̣������ظ�����
	sleep(10);
}

//-----------------------------------------------------------------------------------------------------------------
// ����ǧ�������ĵĶ�����Ҫ����6ת7(SMS)
// sptr: Դ�ַ���ָ��
// dptr: Ŀ���ַ���ָ��
// len: Դ�ַ�������
// ����: Ŀ���ַ�������
int CPhoneWcdma::convert6to7(char *sptr, char *dptr, int len)
{
	static byte mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    byte i, j, k, stemp, dtemp;
    int rlen;
    
    rlen  = 0;
    i     = 0;
    dtemp = 0;
    j = 7;

    for (;;) {
        if (j == 0) {
            j = 7;
            *dptr++ = dtemp;
            rlen++;
            dtemp = 0;
            if (len == 0) break;
        }
        if (i == 0) {
            if (len == 0) {
                rlen = 0;
                break;
            }
            
            char data = *sptr++;
            stemp = 0xff;
			for(k=0; k< sizeof(MapTable); k++) {
				if(data == MapTable[k])		{ stemp = k ; break; }
			}
			
            if (stemp == 0xff) {
                rlen = 0;
                break;
            } else {
                i = 6;
                len--;
            }
        }
        if(stemp & mask[i-1])		dtemp |= mask[j-1];
        i--;
        j--;
    }
    return rlen;
}


//-----------------------------------------------------------------------------------------------------------------
//������ý�����ĵĶ�����Ҫ����6ת8(SMS)
// strOldBuf: Դ�ַ���ָ��
// strNewBuf: Ŀ���ַ���ָ��
// iOldLen: Դ�ַ�������
// ����: Ŀ���ַ�������
int CPhoneWcdma::convert6to8(const char* strOldBuf,char* strNewBuf,const int iOldLen)
{
	int		i=0;	//��ת�����ַ�������
	int		k=0;	//��ת�����ַ��±�
	int		l=8;	//Ҫ��Ҫ�ı�����
	int		iNewLen;
	byte	strOldBuf1[500];

	if((DWORD)iOldLen>500)	return 0;

	// ���ת��
	memcpy( strOldBuf1, strOldBuf, iOldLen);
	for( i=0; i<iOldLen; i++ )
	{
		for( l=0; l<64; l++ )
		{
			if( strOldBuf1[i]==MapTable[l] )
			{
				strOldBuf1[i] = l;
				break;
			}
		}
	}
		
	l=8;k=0;
	for(i=0;i<iOldLen;i++)
	{
		strNewBuf[i]=0;
		strOldBuf1[i]=((unsigned char)strOldBuf1[i] & 0x3f) << 2;
	}
	for(i=0;i<iOldLen;i++)
	{
		if(l<6)
		{
			strNewBuf[k]=(unsigned char)strNewBuf[k] | ((unsigned char)strOldBuf1[i] >> (8-l));
			k++;
			strNewBuf[k]=(unsigned char)strOldBuf1[i] << l;
			l=8-(6-l);
		}else if(l==6)
		{
			strNewBuf[k]=(unsigned char)strNewBuf[k] | ((unsigned char)strOldBuf1[i] >> (8-l));
			k++;
			l=8;
		}else
		{
			strNewBuf[k]=(unsigned char)strNewBuf[k] | (unsigned char)strOldBuf1[i];
			l -= 6;
		}
	}
	iNewLen=k;
	
	return iNewLen;	
}

//-----------------------------------------------------------------------------------------------------------------
// �õ�У���(���)
// buf: �ַ���ָ��
// len: �ַ�������
// ����: У���
byte CPhoneWcdma::get_crc1(const byte *buf, const int len) 
{   
	int	j = 0;
	byte sum = 0xff;

	for( int i=len; i>0; i-- ) {
		sum = CrcTable[sum ^ buf[j++]];
	}
	return (0xff-sum);
}


//--------------------------------------------------------------------------------------------------------------------------
// �õ�У���(ֱ��ȡ�ۼӺ�)
// buf: �ַ���ָ��
// len: �ַ�������
// ����: У���
byte CPhoneWcdma::get_crc2(const byte *buf, const int len)
{   
	byte bytRet;
	bytRet = get_crc_sms( buf, len );
	return bytRet;
}

//--------------------------------------------------------------------------------------------------------------------------
// ���У���(���)
// buf: ��У���ַ���ָ��
// len: ��У���ַ�������
// ����: У����Ƿ���ȷ
bool CPhoneWcdma::check_crc1(const byte *buf, const int len) 
{   
	byte sum = get_crc1( buf+1, len-1 );
	if( buf[0]==sum )
		return true;
	else 
		return false;
}


//--------------------------------------------------------------------------------------------------------------------------
// ���У���(ֱ��ȡ�ۼӺ�)
// buf: ��У���ַ���ָ��
// len: ��У���ַ�������
// ����: У����Ƿ���ȷ
bool CPhoneWcdma::check_crc2(const byte *buf, const int len)
{   
	byte sum = get_crc2( buf, len-1 );
	if( buf[len-1]==sum )		
		return true;
	else						
		return false;
}

// bool CPhoneWcdma::query_CPSI()
// {
// // AT+CPSI?
// //+CPSI: WCDMA,Online,460-01,0xD91F,93203593,WCDMA IMT 2000,316,10713,0,21,86,15,29
// 
// //OK
// 
// 	char szAns[1024] = {0};
// 	ATCMDACK_T CPSI_ans;
// 
// 	_SetAT( 3000, 3, szAns, sizeof(szAns), G_WCDMA_BaseCPSI, 0, "AT+CPSI?" );
// 
// 	if (strlen(szAns)>strlen("+CPSI"))
// 	{
// 		Handler_AT_CPSI((INT8U *)szAns, strlen(szAns), &CPSI_ans);			
// 		memcpy((INT8U *)&s_cpsi_t, CPSI_ans.ackbuf, sizeof(ATDATA_CPSI_T));
// 
// 		return true;
// 	}
// 	else
// 	{
// //		PRTMSG(MSG_DBG, "CPSI buf err\n");
// 		return false;
// 	}
// }
// 
// bool CPhoneWcdma::query_CRUS()
// {
// //AT+CRUS
// //+CRUS: Active SET,1,316,10713,0,0,28,21,86,0,0,1536
// 
// //+CRUS: Sync Neighbor SET,7,291,10713,0,0,0,32,92,1536,256,10713,0,0,31,30,91,1536,170,10713,0,0,63,32,92,1536,178,10713,0,0,11,43,97,1536,186,10713,0,0,24,24,88,1536,324,10713,0,0,0,49,100,1536,233,10713,0,0,0,24,88,1536
// 
// //+CRUS: Async Neighbor SET,18,432,10713,0,0,0,49,121,0,75,10713,0,0,0,49,121,0,83,10713,0,0,0,49,121,0,299,10713,0,0,35,49,121,512,264,10713,0,0,0,49,121,0,105,10713,0,0,31,49,121,512,129,10713,0,0,0,49,121,512,240,10713,0,0,0,49,121,512,225,10713,0,0,0,49,121,0,234,10713,0,0,46,49,121,3072,24,10713,0,0,0,49,121,0,32,10713,0,0,63,49,121,1536,40,10713,0,0,0,49,121,0,128,10713,0,0,0,49,121,0,136,10713,0,0,0,49,121,0,120,10713,0,0,0,49,121,0,218,10713,0,0,0,49,121,0,226,10713,0,0,0,49,121,0
// 
// //OK
// 
// 	char szAns[10240] = {0};
// 	ATCMDACK_T CRUS_ans;
// 
// 	_SetAT( 3000, 3, szAns, sizeof(szAns), G_WCDMA_BaseCRUS, 0, "AT+CRUS" );
// 
// 	if (strlen(szAns)>strlen("+CRUS"))
// 	{
// 		Handler_AT_CRUS((INT8U *)szAns,strlen(szAns),&CRUS_ans);			
// 		memcpy((INT8U *)&s_crus_t, CRUS_ans.ackbuf, sizeof(ATDATA_CRUS_T));
// 
// 		return true;
// 	}
// 	else
// 	{
// //		PRTMSG(MSG_DBG, "CRUS buf err\n");
// 		return false;
// 	}
// }

