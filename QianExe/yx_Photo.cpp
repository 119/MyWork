#include "yx_QianStdAfx.h"

void G_TmPhotoTimer(void *arg, int len)
{
	g_objPhoto.P_TmPhotoTimer();
}

void G_TmUploadPhotoWin(void *arg, int len)
{
	g_objPhoto.P_TmUploadPhotoWin();
}

void G_TmUploadPhotoChk(void *arg, int len)
{
	g_objPhoto.P_TmUploadPhotoChk();
}

int SortWord( const void* v_pElement1, const void* v_pElement2 )
{
	return int( *(WORD*)v_pElement1 ) - int( *(WORD*)v_pElement2 );
}

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-Photo")

//////////////////////////////////////////////////////////////////////////

CPhoto::CPhoto()
{
	memset(m_szPhotoPath, 0, sizeof(m_szPhotoPath));
	sprintf(m_szPhotoPath, "%s", PHOTO_SAVE_PATH);

	m_objDataMngShotDone.InitCfg( 20000, 300000 );
}

CPhoto::~CPhoto()
{

}

int CPhoto::Init()
{
	P_TmPhotoTimer();
}

int CPhoto::Release()
{

}

void CPhoto::P_TmPhotoTimer()
{
	tag2QPhotoCfg objPhotoCfg;

	objPhotoCfg.Init();
	GetSecCfg( &objPhotoCfg, sizeof(objPhotoCfg), offsetof(tagSecondCfg, m_uni2QPhotoCfg.m_obj2QPhotoCfg), sizeof(objPhotoCfg) );

	if( objPhotoCfg.m_wPhotoEvtSymb & PHOTOEVT_TIMER )
	{
		AddPhoto( PHOTOEVT_TIMER );
		
		UINT uiInterval = 1000 * (objPhotoCfg.m_objTimerPhotoPar.m_bytIntervHour * 3600 + objPhotoCfg.m_objTimerPhotoPar.m_bytIntervMin * 60 );
	
		//if( uiInterval < 600000 )
		//	uiInterval = 600000;  // ��С����ĳ�10���ӣ���ֹ��������

		_SetTimer(&g_objTimerMng, TIMERPHOTO_TIMER, uiInterval, G_TmPhotoTimer);
	}
}

void CPhoto::P_TmUploadPhotoWin()
{
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	int iResult = 0;
	WORD wFrameNo;

	static WORD sta_wReChkUdpQueTime = 0;
	static bool sta_bUdpQueEmpty = true;
	bool bUploadPause = false;
	
	// ֹͣ�ش��ж�
	if( m_objUploadPhotoSta.m_bytAllPhotoCt >= m_objUploadPhotoSta.m_bytAllPhotoTotal
		|| m_objUploadPhotoSta.m_bytAllPhotoCt >= UPLOADPHOTO_MAXCOUNT )
	{
		PRTMSG(MSG_DBG, "All Photo Upload Succ!\n" );
		goto STOP_UPLOADPHOTO;
	}

	// ����ǰ�ϴ���Ƭ������Ϊ0,��ֹ����Ƭ���ش����� (�������)
	if( 0 == m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex )
	{
		++ m_objUploadPhotoSta.m_bytAllPhotoCt;
		PRTMSG(MSG_DBG, "The Index of the photo to be uploaded is 0, Stop Upload Photo!\n" );
		goto STOP_UPLOADPHOTO;
	}

	// �����ط�����,������������ֹͣ�ϴ�
// 	if( ++ m_objUploadPhotoSta.m_obj4152.m_bytReSndTime > REUPLOADPHOTO_TIME )
// 	{
// 		PRTMSG(MSG_DBG, "Reupload this Photo Win To Max Times!\n" );
// 
// 		// ֪ͨ�ײ���GPRS����״̬
// 		// g_objSock.LetComuChkGPRS();
// 
// 		bUploadPause = true;
// 		goto STOP_UPLOADPHOTO;
// 	}

	++ m_objUploadPhotoSta.m_obj4152.m_bytReSndTime;

	// �����ش�֡
	iResult = g_objSms.MakeSmsFrame( (char*)&m_objUploadPhotoSta.m_obj4152, sizeof(m_objUploadPhotoSta.m_obj4152),
		0x41, 0x52, buf, sizeof(buf), iBufLen );
	if( !iResult )
	{
		iResult = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV8, DATASYMB_415052 ); // ���ȼ�Ҫ��4150һ��
		if( !iResult ) m_objUploadPhotoSta.m_byt4152Snd = 1;
	}

	wFrameNo = m_objUploadPhotoSta.m_obj4152.m_szFrameNo[0] * 0x100 + m_objUploadPhotoSta.m_obj4152.m_szFrameNo[1];
	PRTMSG(MSG_DBG, "Reuploading photo win %d--last packet No %d, the %d times\n", m_objUploadPhotoSta.m_wPhotoWinCt, wFrameNo, m_objUploadPhotoSta.m_obj4152.m_bytReSndTime );

	// ���¶�ʱ
	_SetTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER, UPLOAD_PHOTOWIN_INTERVAL, G_TmUploadPhotoWin );
	return;

STOP_UPLOADPHOTO:
	// ��ֹ�ش� (�ȿ�������������Ƭ,�ֿ��������¸���Ƭ�ϴ�)
	_KillTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );

	if( bUploadPause )
	{
		m_objUploadPhotoSta.m_bUploadBreak = true; // ���Ϊ�ϴ��ж�,Ҳ����ʱ��ֹ

		g_objSock.TcpLog(); // ����TCP��½�Դ������ĵĶϵ����� (ѯ�ʹ��ཨ��)
	}

	PRTMSG(MSG_DBG, "Stop Upload Photo, because Reupload this Win To Max Times, or Failure happened\n" );
}

void CPhoto::P_TmUploadPhotoChk()
{
	// �����ϴ���Ƭ����
	++ m_objUploadPhotoSta.m_bytAllPhotoCt;
	
	// �ж��Ƿ�ȫ����Ƭ�ϴ�����
	if( m_objUploadPhotoSta.m_bytAllPhotoCt >= m_objUploadPhotoSta.m_bytAllPhotoTotal )
	{
		_StopUploadPhoto();
		m_objUploadPhotoSta.m_bytAllPhotoCt = m_objUploadPhotoSta.m_bytAllPhotoTotal = 0; // ����ϴ���Ƭ����������
	}
	else
	{
		// �����ϴ���һ��Ƭ
		PRTMSG(MSG_DBG, "Leave %d Photos To Upload,Next Photo Index:%d\n",
			m_objUploadPhotoSta.m_bytAllPhotoTotal - m_objUploadPhotoSta.m_bytAllPhotoCt,
			m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex );
		
		_KillTimer(&g_objTimerMng, UPLOAD_PHOTOCHK_TIMER );
		_UploadOnePhoto( m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex );
	}
}

int CPhoto::Deal4100( char* v_szData, DWORD v_dwDataLen )
{
	PRTMSG(MSG_NOR, "Rcv Cent 4100 Res\n" );
	
	WORD wResPhotoIndex = 0;
	wResPhotoIndex = BYTE(v_szData[0]) * WORD(0x100) + BYTE( v_szData[1] ); // ��Ƭ����
	
	// ���Ӧ����Ƭ�͵�ǰ�ϴ�����Ƭ�Ƿ���ͬ
	if( wResPhotoIndex != m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex )
	{
		PRTMSG(MSG_NOR, "Deal 4110, But the photo in res isn't the Uploading photo,Exit!", wResPhotoIndex );
		return ERR_PAR;
	}
	
	// ��¼�����ϴ��ɹ�����Ƭ����
	m_objUploadPhotoSta.m_wLstSuccUpPhotoIndex =
		m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex;
	
	// �����ϴ���Ƭ����
	++ m_objUploadPhotoSta.m_bytAllPhotoCt;
	
	// �ж��Ƿ�ȫ����Ƭ�ϴ�����
	if( m_objUploadPhotoSta.m_bytAllPhotoCt >= m_objUploadPhotoSta.m_bytAllPhotoTotal )
	{
		_StopUploadPhoto();
		m_objUploadPhotoSta.m_bytAllPhotoCt = m_objUploadPhotoSta.m_bytAllPhotoTotal = 0; // ����ϴ���Ƭ����������
	}
	else
	{
		// �����ϴ���һ��Ƭ
		PRTMSG(MSG_NOR, "Leave %d Photos To Upload,Next Photo Index:%d\n",
			m_objUploadPhotoSta.m_bytAllPhotoTotal - m_objUploadPhotoSta.m_bytAllPhotoCt,
			m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex );
		
		_KillTimer(&g_objTimerMng, UPLOAD_PHOTOCHK_TIMER );
		_UploadOnePhoto( m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex );
	}
	
	return 0;
}

/// ������Ƭץ������
// ���͸���M(1)+���¼�����ID(1)+��������K(1)��+��������(1)+��������N(1)+����(N)���� K���� M
int CPhoto::Deal4101( char* v_szData, DWORD v_dwDataLen )
{
	int iRet = 0;
	int iBufLen;
	char buf[ SOCK_MAXSIZE ];
	WORD wPhotoIndex = 0;
	BYTE bytChannel = 1;
	BYTE bytInterval = 0x03;
	BYTE bytPhotoTotal = 0;
	BYTE bytSizeType = 2;
	BYTE bytQualityType = 2;
	BYTE bytBright = 8;
	BYTE bytContrast = 8;
	BYTE bytHue = 8;
	BYTE bytBlueSaturation = 8;
	BYTE bytRedSaturation = 8;

	BYTE bytResType = 1;
	BYTE bytParTotal = 0, bytParCt = 0;
	BYTE bytParType = 0, bytParLen = 0;
	BYTE byt = 0;

	/// �����ߴ���
	if( v_dwDataLen < 1 )
	{
		bytResType = 2;
		iRet = ERR_PAR;
		goto DEAL4101_END;
	}

	/// ��������
	bytParTotal = BYTE( v_szData[0] );
	if( bytParTotal < 1 )
	{
		bytResType = 2;
		iRet = ERR_PAR;
		goto DEAL4101_END;
	}

	/// ��ѯ����
	byt = 1;
	for( bytParCt = 0; bytParCt < bytParTotal; bytParCt ++ )
	{
		// �����ߴ���
		if( DWORD(byt + 2) > v_dwDataLen )
		{
			bytResType = 2;
			iRet = ERR_PAR;
			goto DEAL4101_END;
		}

		// �������ͺͳ���
		bytParType = BYTE( v_szData[ byt ] );
		bytParLen = BYTE( v_szData[ byt + 1 ] );

		// �����ߴ��ٴμ��
		if( DWORD(byt + 2 + bytParLen) > v_dwDataLen )
		{
			bytResType = 2;
			iRet = ERR_PAR;
			goto DEAL4101_END;
		}

		switch( bytParType )
		{
		case 1: // �ֱ���
				// 0x01��352��288
				// 0x02��176��144
				// 0x03��720��576
			{
				bytSizeType = BYTE( v_szData[ byt + 2 ] ); // Э��ֵ
				
				// תΪ�ڲ�ʹ��ֵ
				switch( bytSizeType )
				{
					case 1:
						bytSizeType = 2;
						break;
					case 2:
						bytSizeType = 1;
						break;
					default:
						;
				}
			}
			break;

		case 2:	// ��Ƭͨ����
			{
				bytChannel = BYTE( v_szData[ byt + 2 ] ) & 0x0f;
				if( 0 == bytChannel ) bytChannel = 1; // ͨ���ż��
			}
			break;

		case 3: // ͼƬ����
				// 0x01���㶨�����ȼ����ߣ�
				// 0x02���㶨�����ȼ����У�
				// 0x03���㶨�����ȼ����ͣ�
			{
				bytQualityType = BYTE( v_szData[ byt + 2 ] );
				if( bytQualityType < 1 ) bytQualityType = 1;
				if( bytQualityType > 3 ) bytQualityType = 3;
			}
			break;

		case 4: // ����ץ������
			{
				bytPhotoTotal = BYTE( v_szData[ byt + 2 ] );
				if( bytPhotoTotal < 1 || bytPhotoTotal > 10 ) // (��Ҫ)
				{
					bytResType = 2;
					iRet = ERR_PAR;
					goto DEAL4101_END;
				}
			}
			break;

		case 5: // ���ռ��
			bytInterval = BYTE( v_szData[byt + 2] );
			if( bytInterval > 0x3c ) bytInterval = 0x3c;
			else if( bytInterval < 0x03 ) bytInterval = 0x03;
			break;

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		case 6: // ͼ����ǿ����
			{
				//���ȣ�1�����Աȶȣ�1����ɫ����1������ɫ���Ͷȣ�1������ɫ���Ͷȣ�1��
				bytBright = BYTE( v_szData[byt + 2] );
				bytContrast = BYTE( v_szData[byt + 3] );
				bytHue = BYTE( v_szData[byt + 4] );
				bytBlueSaturation = BYTE( v_szData[byt + 5] );
				bytRedSaturation = BYTE( v_szData[byt + 6] );

				if( bytBright > 0x0f ) bytBright = 0x0f;
				else if( bytBright < 1 ) bytBright = 1;
				if( bytContrast > 0x0f ) bytContrast = 0x0f;
				else if( bytContrast < 1 ) bytContrast = 1;
				if( bytHue > 0x0f ) bytHue = 0x0f;
				else if( bytHue < 1 ) bytHue = 1;
				if( bytBlueSaturation > 0x0f ) bytBlueSaturation = 0x0f;
				else if( bytBlueSaturation < 1 ) bytBlueSaturation = 1;
				if( bytRedSaturation > 0x0f ) bytRedSaturation = 0x0f;
				else if( bytRedSaturation < 1 ) bytRedSaturation = 1;
			}
			break;
#endif

		default:
			;
		}

		byt += 2 + bytParLen; // ׼���´�ѭ��
	}

	PRTMSG(MSG_NOR, "Rcv Cent Photo Req!\n");

	// �ϴε����չ��̡����ϴ����̿��ܻ����
	// ��������µ���������������״̬���ϴ�״̬���ش���ʱȫ�����㣬���Ȿ���ٳ���
	_KillTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );
	memset((void*)&m_objPhotoSta, 0, sizeof(m_objPhotoSta));
	memset((void*)&m_objUploadPhotoSta, 0, sizeof(m_objUploadPhotoSta));

	if( m_objPhotoSta.m_bytPhotoCt < m_objPhotoSta.m_bytPhotoTotal )
	{ /// ���ϴε���������û�н���

		// ����"��̨��æ"Ӧ��
		bytResType = 0x05;
		iRet = 0;
	}
	else
	{ /// ����������״̬

		SendPhotoRequest( bytSizeType, bytQualityType, bytBright, bytContrast, bytHue, bytBlueSaturation,
			bytRedSaturation, bytPhotoTotal, bytChannel, bytInterval, PHOTOEVT_CENT );

		iRet = 0;
	}

DEAL4101_END:
	int iRet2 = g_objSms.MakeSmsFrame((char*)&bytResType, 1, 0x41, 0x41, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9 );

	return iRet;
}

// ����ץ�Ĳ�������
// ���͸���M��1��+���¼�����ID��1��+��������K��1����+�������ͣ�1��+��������N��1��+������N������ K����M
int CPhoto::Deal4102(char* v_szData, DWORD v_dwDataLen )
{
	tag2QPhotoCfg objPhotoCfg, objPhotoCfgBkp;
	tag2QPhotoPar* pPhotoPar = NULL;
	int iRet = 0;
	int iBufLen = 0;
	DWORD dw = 0;
	WORD wPhotoType = 0, wPhotoUpload = 0;
	char buf[SOCK_MAXSIZE];
	BYTE bytResType = 2;
	BYTE bytTypeTotal = 0;
	BYTE bytEvtType = 0;
	BYTE bytParTotal = 0;
	BYTE bytParType = 0;
	BYTE bytParLen = 0;
	BYTE bytEvt = 0, bytPar = 0;

	// ����������ȼ��
	if( v_dwDataLen < 1 )
	{
		iRet = ERR_PAR;
		goto DEAL4102_FAILED;
	}

	// �¼����͸������
	bytTypeTotal = v_szData[ 0 ];
	if( 0 == bytTypeTotal )
	{
		iRet = ERR_PAR;
		goto DEAL4102_FAILED;
	}

	// ��ȡԭʼ����
	iRet = GetSecCfg( &objPhotoCfg, sizeof(objPhotoCfg), offsetof(tagSecondCfg, m_uni2QPhotoCfg.m_obj2QPhotoCfg), sizeof(objPhotoCfg) );
	if( iRet ) goto DEAL4102_FAILED;
	objPhotoCfgBkp = objPhotoCfg; // �ȱ���

	// �����¼����͸�����������
	dw = 1;
	for( bytEvt = 0; bytEvt < bytTypeTotal; bytEvt ++ )
	{
		if( dw + 2 > v_dwDataLen )
		{
			iRet = ERR_PAR;
			goto DEAL4102_FAILED;
		}
		// ��������
		bytParTotal = v_szData[ dw + 1 ];

		// �¼�����
		bytEvtType = v_szData[ dw ];

		// ��ʼ����ʼλ��Ϊ������ʼλ��
		dw += 2;

		// ���ݵ�ǰ�¼�����׼������
		switch( bytEvtType )
		{
#if USE_PROTOCOL == 30
		case 0x10: // ������
#endif

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		case 0x01: // ������
#endif
			wPhotoType = PHOTOEVT_FOOTALERM;// | PHOTOEVT_OVERTURNALERM | PHOTOEVT_BUMPALERM;
			wPhotoUpload = PHOTOUPLOAD_FOOTALERM;// | PHOTOUPLOAD_OVERTURNALERM | PHOTOUPLOAD_BUMPALERM;
			pPhotoPar = &objPhotoCfg.m_objAlermPhotoPar;
			break;

#if USE_PROTOCOL == 30
		case 0x30: // ������
#endif

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		case 0x03: 
#endif
			wPhotoType = PHOTOEVT_OPENDOOR;
			wPhotoUpload = PHOTOUPLOAD_OPENDOOR;
			pPhotoPar = &objPhotoCfg.m_objDoorPhotoPar;
			break;
		
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		case 0x02: // ͨ�� (ͨ����ΪACC���)
#endif

#if USE_PROTOCOL == 30
		case 0x60: // ACC
#endif
			wPhotoType = PHOTOEVT_ACCON;
			wPhotoUpload = PHOTOUPLOAD_ACCON;
			pPhotoPar = &objPhotoCfg.m_objAccPhotoPar;
			break;


#if USE_PROTOCOL == 30
		case 0x50: // ��ʱ����
#endif

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		case 0x06: // 
#endif
			wPhotoType = PHOTOEVT_TIMER;
			wPhotoUpload = PHOTOUPLOAD_TIMER;
			pPhotoPar = &objPhotoCfg.m_objTimerPhotoPar;
			break;

#if USE_JIJIA == 1
		case 0x04: // �Ƽ���
			wPhotoType = PHOTOEVT_JIJIADOWN;
			wPhotoUpload = PHOTOUPLOAD_JIJIADOWN;
			pPhotoPar = &objPhotoCfg.m_objJijiaPhotoPar;
			break;
#endif

#if USE_PROTOCOL == 30
		case 0x20: // ������
			wPhotoType = PHOTOEVT_TRAFFICHELP | PHOTOEVT_MEDICALHELP | PHOTOEVT_BOTHERHELP;
			wPhotoUpload = PHOTOUPLOAD_TRAFFICHELP | PHOTOUPLOAD_MEDICALHELP | PHOTOUPLOAD_BOTHERHELP;
			pPhotoPar = &objPhotoCfg.m_objHelpPhotoPar;
			break;

//		case 0x30: // ���س���
//			wPhotoType = PHOTOEVT_OPENDOOR;
//			wPhotoUpload = PHOTOUPLOAD_OPENDOOR;
//			pPhotoPar = &objPhotoCfg.m_objDoorPhotoPar;
//			break;

		case 0x40: // ��������
			wPhotoType = PHOTOEVT_AREA;
			wPhotoUpload = PHOTOUPLOAD_AREA;
			pPhotoPar = &objPhotoCfg.m_objAreaPhotoPar;
			break;

//		case 0x50: // ��ʱ����
//			wPhotoType = PHOTOEVT_TIMER;
//			wPhotoUpload = PHOTOUPLOAD_TIMER;
//			pPhotoPar = &objPhotoCfg.m_objTimerPhotoPar;
//			break;

//		case 0x00: // ͨ�� (ͨ����ΪACC���)
//		case 0x60: // ACC
//			wPhotoType = PHOTOEVT_ACCON;
//			wPhotoUpload = PHOTOUPLOAD_ACCON;
//			pPhotoPar = &objPhotoCfg.m_objAccPhotoPar;
//			break;
#endif

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		case 0x07:// ��������
			wPhotoType = PHOTOEVT_AREA;
			wPhotoUpload = PHOTOUPLOAD_AREA;
			pPhotoPar = &objPhotoCfg.m_objAreaPhotoPar;
			break;
		case 0x08:// ����
			wPhotoType = PHOTOEVT_OVERSPEED;
			wPhotoUpload = PHOTOUPLOAD_OVERSPEED;
			pPhotoPar = &objPhotoCfg.m_objSpeedPhotoPar;
			break;
		case 0x09:// ����
			wPhotoType = PHOTOEVT_BELOWSPEED;
			wPhotoUpload = PHOTOUPLOAD_BELOWSPEED;
			pPhotoPar = &objPhotoCfg.m_objSpeedPhotoPar;
			break;
#endif

		default:
			iRet = ERR_PAR;
			goto DEAL4102_FAILED;
		}

		// �������¼��µĸ���������������
		for( bytPar = 0; bytPar < bytParTotal; bytPar ++ )
		{
			if( dw >= v_dwDataLen )
			{
				iRet = ERR_PAR;
				goto DEAL4102_FAILED;
			}
			
			switch( v_szData[ dw ] ) // �жϲ�������
			{
			case 0x01: // �ֱ���
				if( dw + 3 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				// �ֱ��ʲ������
				if( v_szData[ dw + 2 ] > 3 || v_szData[ dw + 2 ] < 1 ) v_szData[ dw + 2 ] = 1;

				// ��¼�ֱ���
				if( pPhotoPar )
				{
					pPhotoPar->m_bytSizeType = BYTE( v_szData[ dw + 2 ] );
					
					// �ֱ���תΪ�ڲ�ʹ��ֵ
					switch( pPhotoPar->m_bytSizeType )
					{
						case 1:
							pPhotoPar->m_bytSizeType = 2;
							break;
						case 2:
							pPhotoPar->m_bytSizeType = 1;
							break;
						default:
							;
					}
				}

				// ׼����һѭ��
				dw += 3;
				break;

			case 0x02: // ͨ����
				if( dw + 3 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				// ��¼ͨ����
				if( pPhotoPar )
				{
					pPhotoPar->m_bytChannel = BYTE( v_szData[dw + 2] ) & 0x0f;
					if( 0 == pPhotoPar->m_bytChannel )
						pPhotoPar->m_bytChannel = 1; // ͨ���ż��
				}

				// ׼����һѭ��
				dw += 3;
				break;

			case 0x03: // ��Ƭ����
				if( dw + 3 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				// �����������
				if( v_szData[ dw + 2 ] > 3 ) v_szData[ dw + 2 ] = 3;
				if( v_szData[ dw + 2 ] < 1 ) v_szData[ dw + 2 ] = 1;

				// ��¼��������
				if( pPhotoPar )
					pPhotoPar->m_bytQualityType = BYTE( v_szData[ dw + 2 ] );

				// ׼����һѭ��
				dw += 3;

				break; 
				
			case 0x04: // ����ץ������
				if( dw + 3 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				// ץ���������
				if( v_szData[ dw + 2 ] > 10 ) v_szData[ dw + 2 ] = 10;
				if( v_szData[ dw + 2 ] < 1 ) v_szData[ dw + 2 ] = 1;

				// ��¼ץ������
				if( pPhotoPar )
					pPhotoPar->m_bytTime = BYTE( v_szData[ dw + 2 ] );

				// ׼����һѭ��
				dw += 3;

				break;

#if USE_PROTOCOL == 30
			case 0x05: // ���ռ��
				if( dw + 3 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				if( BYTE(v_szData[dw + 2]) > 0x3c )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				if( pPhotoPar ) pPhotoPar->m_bytInterval = BYTE(v_szData[dw + 2]);
				
				// ׼����һѭ��
				dw += 3;

				break;
#endif

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
			case 0x05: // ������
#endif
#if USE_PROTOCOL == 30
			case 0x07: // ������
#endif
				if( dw + 3 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				// ��¼�¼�����״̬
				if( 0x80 & v_szData[ dw + 2 ] )
					objPhotoCfg.m_wPhotoEvtSymb |= wPhotoType;
				else
					objPhotoCfg.m_wPhotoEvtSymb &= ~wPhotoType;

				// ��¼�����ϱ�״̬
				if( 0x40 & v_szData[ dw + 2 ] )
					objPhotoCfg.m_wPhotoUploadSymb |= wPhotoUpload;
				else
					objPhotoCfg.m_wPhotoUploadSymb &= ~wPhotoUpload;

				// ׼����һѭ��
				dw += 3;

				break;

#if USE_PROTOCOL == 30
			case 0x06:
				if( 0x30 == bytEvtType ) // ���Կ��س���������Ч
				{
					if( pPhotoPar )
						pPhotoPar->m_bytDoorPar = v_szData[ dw + 2 ];
				}
				// ׼����һѭ��
				dw += 3;
				break;
#endif

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
			case 0x06: // ��ǿ����
				if( dw + 7 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				// ���ȣ�1�����Աȶȣ�1����ɫ����1������ɫ���Ͷȣ�1������ɫ���Ͷȣ�1��
				if( pPhotoPar )
				{
					pPhotoPar->m_bytBright = BYTE( v_szData[dw + 2] );
					if( pPhotoPar->m_bytBright > 0x0f ) pPhotoPar->m_bytBright = 0x0f;
					else if( pPhotoPar->m_bytBright < 1 ) pPhotoPar->m_bytBright = 1;
					pPhotoPar->m_bytContrast = BYTE( v_szData[dw + 3] );
					if( pPhotoPar->m_bytContrast > 0x0f ) pPhotoPar->m_bytContrast = 0x0f;
					else if( pPhotoPar->m_bytContrast < 1 ) pPhotoPar->m_bytContrast = 1;
					pPhotoPar->m_bytHue = BYTE( v_szData[dw + 4] );
					if( pPhotoPar->m_bytHue > 0x0f ) pPhotoPar->m_bytHue = 0x0f;
					else if( pPhotoPar->m_bytHue < 1 ) pPhotoPar->m_bytHue = 1;
					pPhotoPar->m_bytBlueSaturation = BYTE( v_szData[dw + 5] );
					if( pPhotoPar->m_bytBlueSaturation > 0x0f ) pPhotoPar->m_bytBlueSaturation = 0x0f;
					else if( pPhotoPar->m_bytBlueSaturation < 1 ) pPhotoPar->m_bytBlueSaturation = 1;
					pPhotoPar->m_bytRedSaturation = BYTE( v_szData[dw + 6] );
					if( pPhotoPar->m_bytRedSaturation > 0x0f ) pPhotoPar->m_bytRedSaturation = 0x0f;
					else if( pPhotoPar->m_bytRedSaturation < 1 ) pPhotoPar->m_bytRedSaturation = 1;
				}

				// ׼����һѭ��
				dw += 7;
				break;

           //xun,081014   // ��ʱ���գ����������޸���� by tl 081218
			case 0x07: // ��ʱ���
				if( dw + 4 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}
				// Сʱ��1��+ ���ӣ�1��
				// Сʱȡֵ:0~23,����ȡֵ:0~59,�����0,ת��Ϊ7f (xun,ѯ�ʺ�־��,081014)
				if( 0x7f == v_szData[dw + 2] )
					v_szData[dw + 2] = 0;
				if( 0x7f == v_szData[dw + 3] )
					v_szData[dw + 3] = 0;

				if( pPhotoPar )
				{
					pPhotoPar->m_bytIntervHour = v_szData[dw + 2] % 24;
					pPhotoPar->m_bytIntervMin = v_szData[dw + 3] % 60;
					// �������ֵ,ȷ����Ϊ0
					if( 0 == pPhotoPar->m_bytIntervHour && 0 == pPhotoPar->m_bytIntervMin )
					{
	//					pPhotoPar->m_bytIntervMin = 1;
    //    �������ܻ�����������Ϊ�����������Ļ���1������һ����Ƭ�����ܻ����������������������
						pPhotoPar->m_bytIntervMin = 10;
					}
				}			
				// ׼����һѭ��
				dw += 4;
				break;

#endif

			default:
				if( dw + 2 > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				bytParLen = v_szData[ dw + 1 ];
				if( dw + 2 + bytParLen > v_dwDataLen )
				{
					iRet = ERR_PAR;
					goto DEAL4102_FAILED;
				}

				// ׼����һѭ��
				dw += 2 + bytParLen;
			}
		}
	}

	// ��֡,����
	bytResType = 1;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x41, 0x42, buf, sizeof(buf), iBufLen );
	if( !iRet )
	{
		// ����������
		if( iRet = SetSecCfg(&objPhotoCfg, offsetof(tagSecondCfg, m_uni2QPhotoCfg.m_obj2QPhotoCfg),
			sizeof(objPhotoCfg)) )
		{
			goto DEAL4102_FAILED;
		}
		else
		{
			if( objPhotoCfg.m_wPhotoEvtSymb & PHOTOEVT_TIMER )
			{
				P_TmPhotoTimer(); // ���̶�ʱ����,�����ڽ�������ʱ��
			}
			else
			{
				_KillTimer(&g_objTimerMng, TIMERPHOTO_TIMER );
			}
		}

		// ����Ӧ��
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12 );
		if( iRet )
		{
			SetSecCfg( &objPhotoCfgBkp, offsetof(tagSecondCfg, m_uni2QPhotoCfg.m_obj2QPhotoCfg), sizeof(objPhotoCfgBkp) ); // �ָ�ԭ����
			goto DEAL4102_FAILED;
		}
	}
	else 
		goto DEAL4102_FAILED;

	return 0;

DEAL4102_FAILED:
	bytResType = 3;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x41, 0x42, buf, sizeof(buf), iBufLen );
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12 );

	return iRet;
}

/// ͨ�����ı����ѯͼƬ��ϻ������ (Ӧ��ɹ�����)
// ��ѯ������1��+��������1��2�� + ���� + ������n��2����
int CPhoto::Deal4103( char* v_szData, DWORD v_dwDataLen )
{
	tagPhotoInfo aryPhotoInfo[ UPLOADPHOTO_MAXCOUNT ];
	int iRet = 0;
	int iBufLen;
	char buf[ SOCK_MAXSIZE ];
	WORD *aryIndex = NULL;
	WORD wPhotoIndex = 0;
	BYTE bytQueryTotal = 0;
	BYTE bytResType = 5;
	BYTE byt = 0;
	bool bInitUpload = true;

	if( m_objPhotoSta.m_bytPhotoCt < m_objPhotoSta.m_bytPhotoTotal // ������δ����
		|| // ��
		(
			m_objUploadPhotoSta.m_bytAllPhotoCt < m_objUploadPhotoSta.m_bytAllPhotoTotal // ��Ƭ�ϴ�δ����
			&&
			!m_objUploadPhotoSta.m_bUploadBreak // ���ϴ�δ��ʱ��ֹ
		)
	  )
	{
		bytResType = 0x03; // ��̨��æ
		iRet = 0;
		goto DEAL4103_FAILED;
	}

	// �������
	if( v_dwDataLen < 1 )
	{
		bytResType = 4; // δ֪ԭ��
		iRet = ERR_PAR;
		goto DEAL4103_FAILED;
	}

	// ��������
	bytQueryTotal = BYTE( v_szData[0] );
	if( 0 == bytQueryTotal || bytQueryTotal > 20 // �����������������
		|| bytQueryTotal > UPLOADPHOTO_MAXCOUNT ) // �򳬹��ڴ滺��ߴ�
	{
		bytResType = 2; // ��ѯ������������
		iRet = ERR_PAR;
		goto DEAL4103_FAILED;
	}

	// �����ټ��
	if( 1 + DWORD(bytQueryTotal * 2) > v_dwDataLen )
	{
		bytResType = 4; // δ֪ԭ��
		iRet = ERR_PAR;
		goto DEAL4103_FAILED;
	}

	// ��ȡҪ�ϴ�����Ƭ����
	memset( aryPhotoInfo, 0, sizeof(aryPhotoInfo) );
	for( byt = 0; byt < bytQueryTotal; byt ++ )
	{
		// ��ȡ��Ƭ����
		if( 0x7f == v_szData[ 1 + 2 * byt ] )
			v_szData[ 1 + 2 * byt ] = 0;
		if( 0x7f == v_szData[ 1 + 2 * byt + 1 ])
			v_szData[ 1 + 2 * byt + 1 ] = 0;

		memcpy( &wPhotoIndex, v_szData + 1 + 2 * byt, 2 );
		
		if( byt < sizeof(aryPhotoInfo) / sizeof(aryPhotoInfo[0]) ) // �����ж�
			aryPhotoInfo[ byt ].m_wPhotoIndex = htons( wPhotoIndex ); // �������ֽ���ָ�
		else
			break;
	}
	bytQueryTotal = byt; // ��������

	// �ϴ�Ӧ��
	bytResType = 1; // �ɹ�
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x41, 0x43, buf, sizeof(buf), iBufLen );
	if( !iRet )
	{
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12 );
		if( !iRet )
		{
			// ���ӵ�Ҫ�ϴ���Ƭ�Ķ�����
			P_AddUploadPhoto( aryPhotoInfo, bytQueryTotal );
		}
		else
		{
			bytResType = 4; // δ֪ԭ��
			goto DEAL4103_FAILED;
		}
	}
	else
	{
		bytResType = 4; // δ֪ԭ��
		goto DEAL4103_FAILED;
	}

	return 0;

DEAL4103_FAILED:
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x41, 0x43, buf, sizeof(buf), iBufLen );
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9 );

	return iRet;
}

/// ͨ���¼���ʱ��β�ѯͼƬ��ϻ������ (Ӧ��ɹ�����)
// ��ѯʱ��Σ�12��+ ����������1��+ �¼���������(1) +���¼�����1��1��+ �� + �¼�����n��1����
int CPhoto::Deal4104( char* v_szData, DWORD v_dwDataLen )
{
	int iRet = 0, n;
	int iBufLen;
	char buf[ SOCK_MAXSIZE ];
	char sz4144[ 300 ] = { 0 };
	char szPhotoDir[255];
	struct dirent **namelist;
	tag4144PhotoIndex* aryPhotoIndex = NULL;
	DWORD dwBeginDate, dwBeginTime, dwEndDate, dwEndTime, dwCurDate;
	DWORD dwPhotoDate, wPhotoTime;
	WORD wPhotoIndex, wPhotoType, wReqPhotoType;
	BYTE bytFindCount = 0;
	BYTE bytEvtTotal = 0, bytIdxTotal = 0;
	BYTE bytResType = 4;
	BYTE byt = 0;
	BYTE bytYear, bytMon, bytDay;
	
	// �����Ƿ���������
	if( m_objPhotoSta.m_bytPhotoCt < m_objPhotoSta.m_bytPhotoTotal )
	{
		iRet = 0;
		bytResType = 3; // ��̨æ
		goto DEAL4104_END;
	}

	// ����������ȼ��
	if( v_dwDataLen < 14 )
	{
		iRet = ERR_PAR;
		bytResType = 4; // δ֪ԭ��
		goto DEAL4104_END;
	}

	// ʱ���ת��
	if( 0x7f == v_szData[0] ) v_szData[0] = 0;
	if( 0x7f == v_szData[3] ) v_szData[3] = 0;
	if( 0x7f == v_szData[4] ) v_szData[4] = 0;
	if( 0x7f == v_szData[5] ) v_szData[5] = 0;
	if( 0x7f == v_szData[6] ) v_szData[6] = 0;
	if( 0x7f == v_szData[9] ) v_szData[9] = 0;
	if( 0x7f == v_szData[10] ) v_szData[10] = 0;
	if( 0x7f == v_szData[11] ) v_szData[11] = 0;

	// ʱ��β�������
	if( v_szData[1] < 1 || v_szData[1] > 12 || v_szData[2] < 1 || v_szData[2] > 31 // ��ʼ����
		|| v_szData[3] > 23 || v_szData[4] > 59 || v_szData[5] > 59 // ��ʼʱ����
		|| v_szData[7] < 1 || v_szData[7] > 12 || v_szData[8] < 1 || v_szData[8] > 31 // ��ֹ����
		|| v_szData[9] > 23 || v_szData[10] > 59 || v_szData[11] > 59 ) // ��ֹʱ����
	{
		iRet = ERR_PAR;
		bytResType = 4; // δ֪ԭ��
		goto DEAL4104_END;
	}

	// ����ʱ������ֲ���
	dwBeginDate = BYTE(v_szData[0]) * DWORD(10000) + BYTE(v_szData[1]) * DWORD(100) + BYTE(v_szData[2]);
	dwBeginTime = BYTE(v_szData[3]) * DWORD(10000) + BYTE(v_szData[4]) * DWORD(100) + BYTE(v_szData[5]);
	dwEndDate = BYTE(v_szData[6]) * DWORD(10000) + BYTE(v_szData[7]) * DWORD(100) + BYTE(v_szData[8]);
	dwEndTime = BYTE(v_szData[9]) * DWORD(10000) + BYTE(v_szData[10]) * DWORD(100) + BYTE(v_szData[11]);

	// ʱ��β����Ϸ��Լ���
	if( dwEndDate < dwBeginDate || (dwEndDate == dwBeginDate && dwEndTime < dwBeginTime) )
	{
		iRet = ERR_PAR;
		bytResType = 4; // δ֪ԭ��
		goto DEAL4104_END;
	}

	// �����ͼ����������
	bytIdxTotal = BYTE( v_szData[12] );
	if( 0x7f == bytIdxTotal ) bytIdxTotal = 0;

	// ͼ�������������
	if( 0 == bytIdxTotal || bytIdxTotal > 20 )
	{
		iRet = ERR_PAR;
		bytResType = 4; // δ֪ԭ��
		goto DEAL4104_END;
	}

	// ���佫Ҫ�������������
	aryPhotoIndex = new tag4144PhotoIndex[ bytIdxTotal ];
	if( !aryPhotoIndex )
	{
		iRet = ERR_MEMLACK;
		bytResType = 4; // δ֪ԭ��
		goto DEAL4104_END;
	}

	// ��ʼ���ҵ�����������
	bytFindCount = 0;

	// �¼����͸���
	bytEvtTotal = BYTE( v_szData[13] );
	if( 0x7f == bytEvtTotal ) bytEvtTotal = 0; // ��ת��
	if( bytEvtTotal > 10 ) // �����������10��
	{
		iRet = ERR_PAR;
		bytResType = 4;
		goto DEAL4104_END;
	}

	// �¼�����->��Ƭ����
	if( v_dwDataLen < WORD(14 + bytEvtTotal) )
	{
		iRet = ERR_MEMLACK;
		bytResType = 4;
		goto DEAL4104_END;
	}
	if( bytEvtTotal > 0 )
	{
		wReqPhotoType = 0;
		for( byt = 0; byt < bytEvtTotal; byt ++ )
		{
			if( 0x7f == v_szData[14 + byt] ) v_szData[14 + byt] = 0;
			wReqPhotoType |= _EvtTypeToPhotoType( BYTE(v_szData[14 + byt]) );

#if USE_PROTOCOL == 30
			// KJ2��̨,Э��������ץ�ĺ�ͨ�����������һ��,�����ڰ�ͨ�����յ�ͬ��ACC�������.
			if( 0 == v_szData[14 + byt] ) wReqPhotoType |= PHOTOEVT_ACCON;
#endif
		}
	}
	else
	{
		wReqPhotoType = 0xffff;
	}

	// ������ֹʱ���,��Ѱ��Ƭ
	bytFindCount = 0; // ��ʼ���ҵ�����Ƭ����

	for( dwCurDate = dwBeginDate; dwCurDate <= dwEndDate; )
	{
		IN_MakePhotoDirName( dwCurDate, szPhotoDir );

		n = scandir(szPhotoDir, &namelist, 0, alphasort);
		if (n < 0)
			perror("scandir");
		else 
		{
			while(n--) 
			{
				if( 0 == strcmp(".", namelist[n]->d_name) || 0 == strcmp("..", namelist[n]->d_name) )
					continue;

				// ���ļ��������Ƭ����ʱ��
				IN_GetDateTimeFromPhotoName( namelist[n]->d_name, dwPhotoDate, wPhotoTime );
				
				// ������ʼʱ��������
				if( dwPhotoDate < dwBeginDate || (dwPhotoDate == dwBeginDate && wPhotoTime < dwBeginTime) )
					continue;
				
				// ������ֹʱ��,����
				if( dwPhotoDate > dwEndDate || (dwPhotoDate == dwEndDate && wPhotoTime > dwEndTime) )
					continue;
				
				// ���ļ��������Ƭ����
				IN_GetTypeFromPhotoName( namelist[n]->d_name, wPhotoType );
				
				// ��������Ҫ������,����
				if( bytEvtTotal > 0 )
				{
					if( 0 == (wReqPhotoType & wPhotoType) ) continue;
				}
				
				// ���ļ��������Ƭ����
				IN_GetIndexFromPhotoName( namelist[n]->d_name, wPhotoIndex );
				
				// ���뵽�ҵ���������
				aryPhotoIndex[ bytFindCount ].m_bytEvtType = _PhotoTypeToEvtType( wPhotoType );
				wPhotoIndex = htons( wPhotoIndex );
				memcpy( &aryPhotoIndex[ bytFindCount ].m_wIndex, &wPhotoIndex, sizeof(aryPhotoIndex[ 0 ].m_wIndex) );
				aryPhotoIndex[ bytFindCount ].m_szDateTime[0] = char( dwPhotoDate / 10000 );
				aryPhotoIndex[ bytFindCount ].m_szDateTime[1] = char( dwPhotoDate % 10000 / 100 );
				aryPhotoIndex[ bytFindCount ].m_szDateTime[2] = char( dwPhotoDate % 100 );
				aryPhotoIndex[ bytFindCount ].m_szDateTime[3] = char( wPhotoTime / 10000 );
				aryPhotoIndex[ bytFindCount ].m_szDateTime[4] = char( wPhotoTime % 10000 / 100 );
				aryPhotoIndex[ bytFindCount ].m_szDateTime[5] = char( wPhotoTime % 100 );
				
				// �����ҵ��㹻��ͼƬ,����
				if( ++ bytFindCount >= bytIdxTotal )
					goto _FINDPHOTO_END;
			}
		}

		// �����´�����
		bytYear = BYTE(dwCurDate / 10000), bytMon = BYTE(dwCurDate % 10000 / 100), bytDay = BYTE(dwCurDate % 100);
		bytDay ++;
		switch( bytMon )
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if( bytDay > 31 )
			{
				bytDay = 1;
				bytMon ++;
			}
			break;

		case 4:
		case 6:
		case 9:
		case 11:
			if( bytDay > 30 )
			{
				bytDay = 1;
				bytMon ++;
			}
			break;

		case 2:
			if( 0 == bytYear % 4 )
			{
				if( bytDay > 29 )
				{
					bytDay = 1;
					bytMon ++;
				}
			}
			else
			{
				if( bytDay > 28 )
				{
					bytDay = 1;
					bytMon ++;
				}
			}
			break;

		default:
			iRet = ERR_PAR;
			bytResType = 4;
			goto DEAL4104_END;
		}

		if( bytMon > 12 )
		{
			bytMon = 1;
			bytYear ++;
		}

		dwCurDate = bytYear * 10000 + bytMon * 100 + bytDay;
	}

_FINDPHOTO_END:
	if( 0 == bytFindCount )
	{
		iRet = 0;
		bytResType = 2;
		goto DEAL4104_END;
	}

	if( 2 + bytFindCount * 9 > sizeof(sz4144) )
	{
		iRet = ERR_SPACELACK;
		bytResType = 4;
		goto DEAL4104_END;
	}

	bytResType = 1;
	iRet = 0;

DEAL4104_END:
	if( 1 == bytResType )
	{
		// Ӧ�����ͣ�1����+����������1��+���¼����ͣ�1��+�����ţ�2��+��ʼʱ�䣨6����* N��
		sz4144[ 0 ] = 1;
		sz4144[ 1 ] = char( bytFindCount );
		memcpy( sz4144 + 2, aryPhotoIndex, bytFindCount * sizeof(aryPhotoIndex[0]) );
		iRet = g_objSms.MakeSmsFrame(sz4144, 2 + bytFindCount * sizeof(aryPhotoIndex[0]), 0x41, 0x44, buf, sizeof(buf), iBufLen);
		if( !iRet ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9 );
	}
	else
	{
		int iRet2 = g_objSms.MakeSmsFrame((char*)&bytResType, 1, 0x41, 0x44, buf, sizeof(buf), iBufLen);
		if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9 );
	}

	free(namelist);
	if( aryPhotoIndex ) delete[] aryPhotoIndex;

	return iRet;
}

/// ��Ƭ����֡Ӧ��
// ��Ƭ����(2) + �����(2) + Ӧ�����ͣ�1����+ �ش���������1��+ [�����1��2�� +�����2��2��+ ���� +�����n��2��] ��
int CPhoto::Deal4110(char* v_szData, DWORD v_dwDataLen, bool v_bFrom4112 /*=false*/ )
{
	PRTMSG(MSG_NOR, " Rcv Cent 4110 Res\n" );

	WORD wResPhotoIndex = 0;
	WORD wResFrameNo = 0;

	if( v_dwDataLen < 5 ) return ERR_PAR;
	
	if( 0x7f == v_szData[0] ) v_szData[0] = 0;
	if( 0x7f == v_szData[1] ) v_szData[1] = 0;
	if( 0x7f == v_szData[2] ) v_szData[2] = 0;
	if( 0x7f == v_szData[3] ) v_szData[3] = 0;

	wResPhotoIndex = BYTE(v_szData[0]) * WORD(0x100) + BYTE( v_szData[1] ); // ��Ƭ����
	wResFrameNo = BYTE(v_szData[2]) * WORD(0x100) + BYTE( v_szData[3] ); // �����

	// ��鵱ǰ״̬�Ƿ���ȷ
	if( m_objUploadPhotoSta.m_bytAllPhotoCt >= m_objUploadPhotoSta.m_bytAllPhotoTotal
		||
		m_objUploadPhotoSta.m_bytAllPhotoCt >= sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo) / sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) )
	{
		m_objUploadPhotoSta.m_bytAllPhotoCt = m_objUploadPhotoSta.m_bytAllPhotoTotal = 0;
		_ClearUploadPhotoStaForNewPhoto();
		PRTMSG(MSG_ERR, "Deal 4110, But No Photo Is Uploading, Exit!\n" );
		return ERR_PAR;
	}

	// ���Ӧ����Ƭ�͵�ǰ�ϴ�����Ƭ�Ƿ���ͬ
	if( wResPhotoIndex != m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex )
	{
		PRTMSG(MSG_NOR, "Deal 4110, But the photo in res isn't the Uploading photo,Exit!\n", wResPhotoIndex );
		return ERR_PAR;
	}

	if( wResFrameNo / UPLOADPHOTO_WINFRAMETOTAL != m_objUploadPhotoSta.m_wPhotoWinCt ) // ��Ӧ�𴰿ں�״̬��¼�еĲ�ͬ
	{
		PRTMSG(MSG_NOR, "Deal 4110, But the win(last packet %d) isn't the Uploading win,Exit!\n", wResFrameNo );
		return ERR_PAR;
	}

	int iRet = 0;

	if( v_bFrom4112 )
	{
		if( !m_objUploadPhotoSta.m_byt4152Snd ) return 0;
	}

	if( 1 == v_szData[4] ) // �ô��ڳɹ�ȷ��
	{
		PRTMSG(MSG_NOR, "The No. %d Win Uploaded Succ!\n", m_objUploadPhotoSta.m_wPhotoWinCt );

		// ֹͣ�ش���ʱ
		_KillTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );

		// ������ɴ��ڼ���
		++ m_objUploadPhotoSta.m_wPhotoWinCt;

		// Ϊ������´����ϴ����������
		_ClearUploadPhotoStaForNewWin();

		if( DWORD(m_objUploadPhotoSta.m_wPhotoWinCt) * UPLOADPHOTO_WINFRAMETOTAL
			>= DWORD(m_objUploadPhotoSta.m_wPhotoFrameTotal) ) // �������������һ������
		{
			// ��¼�����ϴ��ɹ�����Ƭ����
			m_objUploadPhotoSta.m_wLstSuccUpPhotoIndex =
				m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex;

			// �����ϴ���Ƭ����
			++ m_objUploadPhotoSta.m_bytAllPhotoCt;
			
			// �ж��Ƿ�ȫ����Ƭ�ϴ�����
			if( m_objUploadPhotoSta.m_bytAllPhotoCt >= m_objUploadPhotoSta.m_bytAllPhotoTotal )
			{
				_StopUploadPhoto();
				m_objUploadPhotoSta.m_bytAllPhotoCt = m_objUploadPhotoSta.m_bytAllPhotoTotal = 0; // ����ϴ���Ƭ����������
			}
			else
			{
				// �����ϴ���һ��Ƭ
				PRTMSG(MSG_NOR, "Leave %d Photos To Upload,Next Photo Index:%d\n",
					m_objUploadPhotoSta.m_bytAllPhotoTotal - m_objUploadPhotoSta.m_bytAllPhotoCt,
					m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex );
				
				_ClearUploadPhotoStaForNewPhoto(); // Ϊ����Ƭ�ϴ�����ϴ���Ƭ״̬
				_UploadOnePhotoWin( m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex,
					false, 0, 0, NULL );
			}
		}
		else
		{
			// �ϴ���һ����
			PRTMSG(MSG_NOR, "Continue Upload Next Win\n" );
			_ClearUploadPhotoStaForNewWin();
			_UploadOnePhotoWin( m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex,
				false, m_objUploadPhotoSta.m_wPhotoWinCt * UPLOADPHOTO_WINFRAMETOTAL, 0, NULL );
		}
	}
	else // �����ô�����δ�ɹ��İ�
	{
// 		if( 0 == m_objUploadPhotoSta.m_bytAllPhotoTotal )
// 		{
// 			PRTMSG(MSG_NOR, "���������ش�,����̨δ������Ҫ�ϴ�����Ƭ,������!" );
// 			return ERR_PAR;
// 		}

		if( v_dwDataLen < 7 ) return ERR_PAR;

		if( 0x7f == v_szData[5] ) v_szData[5] = 0;
		BYTE bytFrameTotal = BYTE( v_szData[5] ); // �ش�������

		if( 0 == bytFrameTotal ) return ERR_PAR;

		if( WORD(bytFrameTotal) != m_objUploadPhotoSta.m_wPhotoFrameTotal ) // ������ȫ���ش�
		{
			if( 0 == bytFrameTotal ||
				(bytFrameTotal > UPLOADPHOTO_WINFRAMETOTAL && WORD(bytFrameTotal) != m_objUploadPhotoSta.m_wPhotoFrameTotal ) )
				return ERR_PAR;
			if( 6 + 2 * DWORD(bytFrameTotal) != v_dwDataLen ) return ERR_PAR;

			DWORD dw = 6;
			BYTE byt = 0;
			WORD aryFrameNo[ UPLOADPHOTO_WINFRAMETOTAL ];
			for( byt = 0; byt < bytFrameTotal; byt ++ )
			{
				if( 0x7f == v_szData[ dw ] ) v_szData[ dw ] = 0;
				if( 0x7f == v_szData[ dw + 1 ] ) v_szData[ dw + 1 ] = 0;

				aryFrameNo[ byt ] = BYTE(v_szData[ dw ]) * WORD(0x100) + BYTE(v_szData[ dw + 1 ]);
				dw += 2;
			}

			// ���������
			if( bytFrameTotal > 1 ) 
				qsort( aryFrameNo, bytFrameTotal, sizeof(aryFrameNo[0]), SortWord );

			// ������źϷ���
			if( aryFrameNo[0] / UPLOADPHOTO_WINFRAMETOTAL != aryFrameNo[ byt - 1] / UPLOADPHOTO_WINFRAMETOTAL // ������İ���Ų�����ͬһ����
				|| aryFrameNo[0] / UPLOADPHOTO_WINFRAMETOTAL != m_objUploadPhotoSta.m_wPhotoWinCt ) // ����İ������״̬��¼�в�����ͬһ����
			{
				PRTMSG(MSG_NOR, "Deal 4110, fail_1\n" );
				return ERR_PAR;
			}
			
			// ֹͣ�ش���ʱ
			_KillTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );

			// �ش���Ҫ�İ�
			_UploadOnePhotoWin( m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex,
				true, 0, bytFrameTotal, aryFrameNo );
		}
		else
		{
			// ֹͣ�ش���ʱ
			_KillTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );
			
			PRTMSG(MSG_NOR, "Cent Req Upload All Packets of the Photo\n" );

			// �ش���һ�����ڵİ�
			_UploadOnePhotoWin( m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex,
				false, 0, 0, NULL );
		}
	}
	
	return iRet;
}

/// ��Ƭ����(2) + ����ţ�2����Ӧ�����ͣ�1����+ �ش���������1��+ [�����1��2�� +�����2��2��+ ���� +�����n��2��] ��
int CPhoto::Deal4112( char* v_szData, DWORD v_dwDataLen )
{
	PRTMSG(MSG_NOR, "Rcv Cent 4112 res!\n" );

	return Deal4110( v_szData, v_dwDataLen, true );
}

// ����ϵ�����ͼƬ: ͼ��������2��+ �ش���������1��+[�����1��2�� +�����2��2��+ ���� +�����n��2��]
// �������������������,Ҳ������ϵͳ��λ��,���Բ��������ڴ��е�״̬���ж������Ƿ���Ч
int CPhoto::Deal4113(char* v_szData, DWORD v_dwDataLen )
{
	PRTMSG(MSG_NOR, "Rcv Cent 4113 res!\n" );

	int iRet = 0;
	WORD wResPhotoIndex;

	// �������ȼ��
	if( v_dwDataLen < 3 ) return ERR_PAR;
	
	if( 0x7f == v_szData[0] ) v_szData[0] = 0;
	if( 0x7f == v_szData[1] ) v_szData[1] = 0;
	wResPhotoIndex = BYTE(v_szData[0]) * WORD(0x100) + BYTE( v_szData[1] ); // ��Ƭ����

	// ������Ƭ���ǵ�ǰ���ж��Ƿ��ϴ��ɹ�����Ƭ
	if( m_objUploadPhotoSta.m_bytAllPhotoCt < m_objUploadPhotoSta.m_bytAllPhotoTotal
		&& m_objUploadPhotoSta.m_bytAllPhotoCt < UPLOADPHOTO_MAXCOUNT
		&& (wResPhotoIndex != m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex)
		)
	{
		// Ҫ�����ж��Ƿ�����ƬҪ�ϴ�

		// ֹͣ�ش���ʱ
		_KillTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );

		// ��¼�����ϴ��ɹ�����Ƭ����
		m_objUploadPhotoSta.m_wLstSuccUpPhotoIndex =
			m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex;
		
		// �����ϴ���Ƭ����
		++ m_objUploadPhotoSta.m_bytAllPhotoCt;
		
		// �ж��Ƿ�ȫ����Ƭ�ϴ�����
		if( m_objUploadPhotoSta.m_bytAllPhotoCt >= m_objUploadPhotoSta.m_bytAllPhotoTotal )
		{
			_StopUploadPhoto();
			m_objUploadPhotoSta.m_bytAllPhotoCt = m_objUploadPhotoSta.m_bytAllPhotoTotal = 0; // ����ϴ���Ƭ����������
		}
		else
		{
			// �����ϴ���һ��Ƭ
			PRTMSG(MSG_NOR, "Leave %d Photos To Upload,Next Photo Index:%d\n",
				m_objUploadPhotoSta.m_bytAllPhotoTotal - m_objUploadPhotoSta.m_bytAllPhotoCt,
				m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex );
			
			_ClearUploadPhotoStaForNewPhoto(); // Ϊ����Ƭ�ϴ�����ϴ���Ƭ״̬
			_UploadOnePhotoWin( m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex,
				false, 0, 0, NULL );
		}

		return 0;
	}

	{
		// �ش�������
		BYTE bytFrameTotal = BYTE( v_szData[2] );
		if( 0 == bytFrameTotal || bytFrameTotal > UPLOADPHOTO_WINFRAMETOTAL ) return ERR_PAR;

		// �������ȼ��
		if( 3 + 2 * DWORD(bytFrameTotal) != v_dwDataLen ) return ERR_PAR;

		// ��ȡ�����
		DWORD dw = 3;
		BYTE byt = 0;
		WORD aryFrameNo[ UPLOADPHOTO_WINFRAMETOTAL ];
		for( byt = 0; byt < bytFrameTotal; byt ++ )
		{
			if( 0x7f == v_szData[ dw ] ) v_szData[ dw ] = 0;
			if( 0x7f == v_szData[ dw + 1 ] ) v_szData[ dw + 1 ] = 0;

			aryFrameNo[ byt ] = BYTE(v_szData[ dw ]) * WORD(0x100) + BYTE(v_szData[ dw + 1 ]);
			dw += 2;
		}

		// ���������
		if( bytFrameTotal >= 2 ) 
			qsort( aryFrameNo, bytFrameTotal, sizeof(aryFrameNo[0]), SortWord );

		// ������źϷ���
		if( aryFrameNo[0] / UPLOADPHOTO_WINFRAMETOTAL != aryFrameNo[ byt - 1 ] / UPLOADPHOTO_WINFRAMETOTAL ) // ������İ���Ų�����ͬһ����
			return ERR_PAR;

		// ���ϴ������жϹ���δ���ָ�
		if( m_objUploadPhotoSta.m_bUploadBreak )
		{
			_ClearUploadPhotoStaForNewPhoto();
			m_objUploadPhotoSta.m_bytAllPhotoCt ++; // ��������Ƭ�ϴ�
			if( m_objUploadPhotoSta.m_bytAllPhotoCt >= m_objUploadPhotoSta.m_bytAllPhotoTotal )
				m_objUploadPhotoSta.m_bytAllPhotoCt = m_objUploadPhotoSta.m_bytAllPhotoTotal = 0; // ����ϴ���Ƭ����������
		}
		
		/// �����ϴ���Ƭ״̬
		if( m_objUploadPhotoSta.m_bytAllPhotoCt < m_objUploadPhotoSta.m_bytAllPhotoTotal ) // ����ʱ����Ҫ�ϴ�����Ƭ
		{
			// ����ԭ����
			tagPhotoInfo aryTempIndex[ UPLOADPHOTO_MAXCOUNT ];
			memcpy( aryTempIndex, m_objUploadPhotoSta.m_aryUpPhotoInfo,
			 UPLOADPHOTO_MAXCOUNT * sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );
			 
			// �ش���Ƭ��Ϣ���������һ��λ��
			memset( &(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]), 0, sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );
			m_objUploadPhotoSta.m_aryUpPhotoInfo[ 0 ].m_wPhotoIndex = wResPhotoIndex;
			
			// ��������������δ��ɵ���Ƭ
			for( byt = 1;
				byt < sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo) / sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0])
				&& m_objUploadPhotoSta.m_bytAllPhotoCt < m_objUploadPhotoSta.m_bytAllPhotoTotal;
				 m_objUploadPhotoSta.m_bytAllPhotoCt ++ )
			{
				if( wResPhotoIndex != aryTempIndex[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex )
				{
					m_objUploadPhotoSta.m_aryUpPhotoInfo[ byt ] = aryTempIndex[ m_objUploadPhotoSta.m_bytAllPhotoCt ];
					byt ++;
				}
			}
			
			// �����������ѷ�����
			m_objUploadPhotoSta.m_bytAllPhotoTotal = byt;
			m_objUploadPhotoSta.m_bytAllPhotoCt = 0;
		}
		else
		{
			memset( &(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]), 0, sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );
			m_objUploadPhotoSta.m_aryUpPhotoInfo[ 0 ].m_wPhotoIndex = wResPhotoIndex;
			m_objUploadPhotoSta.m_bytAllPhotoTotal = 1;
			m_objUploadPhotoSta.m_bytAllPhotoCt = 0;
		}
		
		// �ش���Ҫ�İ�
		_ClearUploadPhotoStaForNewPhoto();
		_UploadOnePhotoWin( wResPhotoIndex, true, 0, bytFrameTotal, aryFrameNo );
	}
	
	return iRet;
}

void CPhoto::AddPhoto( WORD v_wPhotoType )
{
	// ��ȡ����
	tag2QPhotoCfg objPhotoCfg;
	objPhotoCfg.Init();
	GetSecCfg( &objPhotoCfg, sizeof(objPhotoCfg), offsetof(tagSecondCfg, m_uni2QPhotoCfg.m_obj2QPhotoCfg), sizeof(objPhotoCfg) );

	if( !(objPhotoCfg.m_wPhotoEvtSymb & v_wPhotoType) )
		return;

	tag2QPhotoPar objPhotoPar;
	if( v_wPhotoType & PHOTOEVT_FOOTALERM || v_wPhotoType & PHOTOEVT_FOOTALERM_BIG )
	{
		objPhotoPar = objPhotoCfg.m_objAlermPhotoPar;
	}
#if USE_PROTOCOL == 30
	else if( v_wPhotoType & (PHOTOEVT_OVERTURNALERM | PHOTOEVT_BUMPALERM | PHOTOEVT_DRIVEALERM) )
	{
		objPhotoPar = objPhotoCfg.m_objAlermPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_OPENDOOR )
	{
		objPhotoPar = objPhotoCfg.m_objDoorPhotoPar;
	}
#endif
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
	else if( v_wPhotoType & PHOTOEVT_JIJIADOWN )
	{
		objPhotoPar = objPhotoCfg.m_objJijiaPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_TIMER )
	{
		objPhotoPar = objPhotoCfg.m_objTimerPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_ACCON )
	{
		objPhotoPar = objPhotoCfg.m_objAccPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_OPENDOOR )
	{
		objPhotoPar = objPhotoCfg.m_objDoorPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_AREA )
	{
		objPhotoPar = objPhotoCfg.m_objAreaPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_OVERSPEED || v_wPhotoType & PHOTOEVT_BELOWSPEED )
	{
		objPhotoPar = objPhotoCfg.m_objSpeedPhotoPar;
	}
#endif
#if USE_PROTOCOL == 30
	else if( v_wPhotoType & (PHOTOEVT_TRAFFICHELP | PHOTOEVT_MEDICALHELP | PHOTOEVT_BOTHERHELP) )
	{
		objPhotoPar = objPhotoCfg.m_objHelpPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_AREA )
	{
		objPhotoPar = objPhotoCfg.m_objAreaPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_OPENDOOR )
	{
		objPhotoPar = objPhotoCfg.m_objDoorPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_TIMER )
	{
		objPhotoPar = objPhotoCfg.m_objTimerPhotoPar;
	}
	else if( v_wPhotoType & PHOTOEVT_ACCON )
	{
		objPhotoPar = objPhotoCfg.m_objAccPhotoPar;
	}
#endif
	else return;

	BYTE bytPhotoTime = objPhotoPar.m_bytTime;
	BYTE bytPhotoSizeType = objPhotoPar.m_bytSizeType;
	BYTE bytPhotoQualityType = objPhotoPar.m_bytQualityType;
	BYTE bytBright = objPhotoPar.m_bytBright;
	BYTE bytContrast = objPhotoPar.m_bytContrast;
	BYTE bytHue = objPhotoPar.m_bytHue;
	BYTE bytBlueSaturation = objPhotoPar.m_bytBlueSaturation;
	BYTE bytRedSaturation = objPhotoPar.m_bytRedSaturation;
	BYTE bytChannel = objPhotoPar.m_bytChannel;
	BYTE bytInterval = objPhotoPar.m_bytInterval;
	if( 0 == bytPhotoTime ) bytPhotoTime = 1;
	else if( bytPhotoTime > PHOTOQUEE_MAXCOUNT ) bytPhotoTime = PHOTOQUEE_MAXCOUNT;

	// ���Ǳ������յĲ��ϴ����еȳߴ���Ƭ�����⴦��
	if( v_wPhotoType & PHOTOEVT_FOOTALERM_BIG )
	{
		bytPhotoTime = 1;
		if( bytPhotoSizeType < 2 ) bytPhotoSizeType = 2;
		if( bytPhotoQualityType > 2 || 0 == bytPhotoQualityType ) bytPhotoQualityType = 2;
		bytChannel |= 3;
	}

	// �������յ�����
	if( m_objPhotoSta.m_bytPhotoTotal < PHOTOQUEE_MAXCOUNT )
	{
		// ���������������յ�����
		bytPhotoTime = min(bytPhotoTime, PHOTOQUEE_MAXCOUNT - m_objPhotoSta.m_bytPhotoTotal);
		
		// ����������
		m_objPhotoSta.m_bytPhotoTotal += bytPhotoTime;
	}
	else
	{
		return;
	}

	SendPhotoRequest(bytPhotoSizeType, bytPhotoQualityType, bytBright, bytContrast,bytHue,
		bytBlueSaturation, bytRedSaturation, bytPhotoTime, bytChannel, bytInterval,v_wPhotoType);
}

void CPhoto::SendPhotoRequest( BYTE v_bytSizeType, BYTE v_bytQualityType, BYTE v_bytBright,
							 BYTE v_bytContrast, BYTE v_bytHue, BYTE v_bytBlueSaturation, BYTE v_bytRedSaturation, 
							 BYTE v_bytPhotoTotal, BYTE v_bytChannel, BYTE v_bytInterval, WORD v_wPhotoType )
{
	memset( &m_objPhotoSta, 0, sizeof(m_objPhotoSta) );
	
	size_t sizTotal = sizeof(m_objPhotoSta.m_aryPhotoInfo) / sizeof(m_objPhotoSta.m_aryPhotoInfo[0]);
	
	if( size_t(v_bytPhotoTotal) >= sizTotal )
		v_bytPhotoTotal = BYTE(sizTotal);

	int iChannelNum = 0;
	// ����������ͷ��������
	if( v_bytChannel & 0x01 ) 
		iChannelNum ++;
	if( v_bytChannel & 0x02 ) 
		iChannelNum ++;
	if( v_bytChannel & 0x04 ) 
		iChannelNum ++;
	if( v_bytChannel & 0x08 ) 
		iChannelNum ++;
	
	m_objPhotoSta.m_bytPhotoTotal = v_bytPhotoTotal * iChannelNum;
	
	tagPhotoInfo objToShot;
	DWORD dwPktNum = 0;
	char buf[100] = {0};
	buf[0] = 0x04;
	//static int k = 0;
	for( BYTE byt = 0; byt < v_bytPhotoTotal; byt ++ )
	{
		objToShot.m_bytSizeType = v_bytSizeType;
		objToShot.m_bytQualityType = v_bytQualityType;
		objToShot.m_bytBright = v_bytBright;
		objToShot.m_bytContrast = v_bytContrast;
		objToShot.m_bytHue = v_bytHue;
		objToShot.m_bytBlueSaturation = v_bytBlueSaturation;
		objToShot.m_bytRedSaturation = v_bytRedSaturation;
		objToShot.m_wPhotoType = v_wPhotoType;
		objToShot.m_bytInterval = v_bytInterval;
		for( int i=0; i<4; i++ )
		{
			objToShot.m_bytChannel = v_bytChannel & (0x01<<i) ;
			if( objToShot.m_bytChannel != 0)
			{
				memcpy(buf+1, (void*)&objToShot, sizeof(objToShot));			
				DataPush(buf, sizeof(objToShot)+1, DEV_QIAN, DEV_DVR, LV2);
			//	PRTMSG(MSG_DBG, "Qian push %d frame!\n", ++k);
			}
		}
	}
}

void CPhoto::DealOnePhotoEnd()
{	
	tagPhotoInfo objPhotoInfo;
	DWORD dwDataLen = 0;
	DWORD dwPushTm = 0;
	BYTE bytLvl = 0;
	
	while( !m_objDataMngShotDone.PopData(bytLvl, sizeof(objPhotoInfo), dwDataLen, (char*)&objPhotoInfo, dwPushTm) )
	{
		if( m_objPhotoSta.m_bytPhotoCt < sizeof(m_objPhotoSta.m_aryPhotoInfo) / sizeof(m_objPhotoSta.m_aryPhotoInfo[0]) )
		{
			m_objPhotoSta.m_aryPhotoInfo[m_objPhotoSta.m_bytPhotoCt] = objPhotoInfo;
			m_objPhotoSta.m_bytPhotoCt ++;
		}
	}
#if UPLOAD_AFTERPHOTOALL == 0
	_UploadPhotos();
#endif


}

void CPhoto::_UploadPhotos()
{
	// ���մ���
	if( m_objPhotoSta.m_bytPhotoCt > sizeof(m_objPhotoSta.m_aryPhotoInfo) / sizeof(m_objPhotoSta.m_aryPhotoInfo[0]) )
		m_objPhotoSta.m_bytPhotoCt = sizeof(m_objPhotoSta.m_aryPhotoInfo) / sizeof(m_objPhotoSta.m_aryPhotoInfo[0]);
	
	// ��ȡ����
	tag2QPhotoCfg objPhotoCfg;
	objPhotoCfg.Init();
	GetSecCfg( &objPhotoCfg, sizeof(objPhotoCfg), offsetof(tagSecondCfg, m_uni2QPhotoCfg.m_obj2QPhotoCfg), sizeof(objPhotoCfg) );
	WORD wUploadPhotoType = _UploadTypeToPhotoType( objPhotoCfg.m_wPhotoUploadSymb ); // ������Ҫ�ϴ�����Ƭ����
	
	// ����Ƭȷ�����ϴ���Ƭ�������ϴ�����
	tagPhotoInfo aryPhoto[PHOTOQUEE_MAXCOUNT];
	tag4144PhotoIndex aryIndex[PHOTOQUEE_MAXCOUNT] = { 0 };
	BYTE bytPhotoCt = 0, bytIndexCt = 0;
	bool bCurPhotoSnd = false;
	for( BYTE byt = 0; byt < m_objPhotoSta.m_bytPhotoCt; byt ++ )
	{
		if( m_objPhotoSta.m_aryPhotoInfo[ byt ].m_bUploaded ) // ���Ѿ�������ϴ�
			continue;
			
		m_objPhotoSta.m_aryPhotoInfo[ byt ].m_bUploaded = true; // ��Ǳ�����Ƭ�Ѿ�������ϴ�
		
		if(  m_objPhotoSta.m_aryPhotoInfo[ byt ].m_wPhotoType & PHOTOEVT_CENT // ����������ץ��
			|| ( !(PHOTOEVT_FOOTALERM_BIG & m_objPhotoSta.m_aryPhotoInfo[byt].m_wPhotoType)
			 &&(wUploadPhotoType & m_objPhotoSta.m_aryPhotoInfo[byt].m_wPhotoType)) // ������Ҫ�����ϴ�����Ƭ���� (�������ӵĴ���Ƭ����))
			)
		{
			bCurPhotoSnd = true;
		}
		else
		{
			bCurPhotoSnd = false;
		}
		
		if( bCurPhotoSnd && bytPhotoCt < PHOTOQUEE_MAXCOUNT )
		{
			aryPhoto[ bytPhotoCt ] = m_objPhotoSta.m_aryPhotoInfo[ byt ];
			++ bytPhotoCt;
		}
		else if( bytIndexCt < PHOTOQUEE_MAXCOUNT )
		{
			aryIndex[ bytIndexCt ].m_bytEvtType
				= _PhotoTypeToEvtType( m_objPhotoSta.m_aryPhotoInfo[ byt ].m_wPhotoType );
			memcpy( aryIndex[ bytIndexCt ].m_szDateTime, m_objPhotoSta.m_aryPhotoInfo[ byt ].m_szDateTime, 6 );
			WORD wIndex = m_objPhotoSta.m_aryPhotoInfo[ byt ].m_wPhotoIndex;
			wIndex = htons( wIndex );
			memcpy( &aryIndex[ bytIndexCt ].m_wIndex, &wIndex, 2 );
			
			++ bytIndexCt;
		}
	}
	
	// �ϴ�����
	if( bytIndexCt > 0 && bytIndexCt <= PHOTOQUEE_MAXCOUNT )
	{
		char sz4145[ 300 ] = { 0 };
		sz4145[0] = bytIndexCt;
		memcpy( sz4145 + 1, aryIndex, bytIndexCt * sizeof(aryIndex[0]) );
		
		char buf[ 200 ] = { 0 };
		int iBufLen = 0;
		
		int iResult = g_objSms.MakeSmsFrame( sz4145, 1 + bytIndexCt * sizeof(aryIndex[0]),
			0x41, 0x45, buf, sizeof(buf), iBufLen );
		if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV8 );
	}
	
	// �ϴ���Ƭ
	if( bytPhotoCt > 0 )
	{
		P_AddUploadPhoto( aryPhoto, bytPhotoCt );
	}
}

void CPhoto::P_AddUploadPhoto( tagPhotoInfo* v_aryPhoto, BYTE v_bytPhotoTotal )
{
	if( m_objUploadPhotoSta.m_bytAllPhotoCt < m_objUploadPhotoSta.m_bytAllPhotoTotal ) // ���ϴ��ϴ���δ����
	{
#if USE_PHOTOUPLD == 0
		// ���ϴ������жϹ���δ���ָ�
		if( m_objUploadPhotoSta.m_bUploadBreak )
		{
			PRTMSG(MSG_DBG, "Last Photo Upload Break\n" );
			_ClearUploadPhotoStaForNewPhoto();

			// �����ʱ��δ���͵���Ƭ����
			tagPhotoInfo aryTempIndex[ UPLOADPHOTO_MAXCOUNT ];
			BYTE bytOldTotal = m_objUploadPhotoSta.m_bytAllPhotoTotal - m_objUploadPhotoSta.m_bytAllPhotoCt; // ��δ���͵�����
			memcpy( aryTempIndex, m_objUploadPhotoSta.m_aryUpPhotoInfo + m_objUploadPhotoSta.m_bytAllPhotoCt,
				bytOldTotal * sizeof(aryTempIndex[0]) );

			// ����Ƭ����ǰ��
			if( v_bytPhotoTotal > UPLOADPHOTO_MAXCOUNT ) v_bytPhotoTotal = UPLOADPHOTO_MAXCOUNT; // ���մ���
			memcpy( m_objUploadPhotoSta.m_aryUpPhotoInfo, v_aryPhoto,
				v_bytPhotoTotal * sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );

			// ����Ƭ���ں���
			bytOldTotal = v_bytPhotoTotal + bytOldTotal <= UPLOADPHOTO_MAXCOUNT
				? bytOldTotal : UPLOADPHOTO_MAXCOUNT - v_bytPhotoTotal;
			memcpy( m_objUploadPhotoSta.m_aryUpPhotoInfo + v_bytPhotoTotal, aryTempIndex,
 				bytOldTotal * sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );

			// �����ܹ�Ҫ������Ƭ������
			m_objUploadPhotoSta.m_bytAllPhotoTotal = v_bytPhotoTotal + bytOldTotal;
		}
		else
		{
			// �����ʱ��δ������ɵ���Ƭ����
			tagPhotoInfo aryTempIndex[ UPLOADPHOTO_MAXCOUNT ];
			BYTE bytOldTotal = m_objUploadPhotoSta.m_bytAllPhotoTotal - m_objUploadPhotoSta.m_bytAllPhotoCt; // ��δ���͵�����
			memcpy( aryTempIndex, m_objUploadPhotoSta.m_aryUpPhotoInfo + m_objUploadPhotoSta.m_bytAllPhotoCt,
				bytOldTotal * sizeof(aryTempIndex[0]) );

			// δ���͵ľ���Ƭ������ǰ
			if( bytOldTotal > UPLOADPHOTO_MAXCOUNT ) bytOldTotal = UPLOADPHOTO_MAXCOUNT; // ���մ���
			memcpy( m_objUploadPhotoSta.m_aryUpPhotoInfo, aryTempIndex,
				bytOldTotal * sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );

			// ����Ƭ�������
			v_bytPhotoTotal = bytOldTotal + v_bytPhotoTotal <= UPLOADPHOTO_MAXCOUNT
				? v_bytPhotoTotal : UPLOADPHOTO_MAXCOUNT - bytOldTotal;
			memcpy( m_objUploadPhotoSta.m_aryUpPhotoInfo + bytOldTotal, v_aryPhoto,
				v_bytPhotoTotal * sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );

			// �����ܹ�Ҫ������Ƭ������
			m_objUploadPhotoSta.m_bytAllPhotoTotal = bytOldTotal + v_bytPhotoTotal;
		}
								
		// �ѷ�����Ƭ������0
		m_objUploadPhotoSta.m_bytAllPhotoCt = 0;
				
		// �������,Ҫ�������ش���Ƭ��ʱ (��Ҫ, ����,�������ĵĳɹ�Ӧ��̨û���յ�,<��̨�ֻ�ģ�鸴λ��>,�����´�����ʱ��Ƭ��������)
		if( m_objUploadPhotoSta.m_bPhotoBeginUpload && m_objUploadPhotoSta.m_byt4152Snd )
		{
			m_objUploadPhotoSta.m_obj4152.m_bytReSndTime = 0;
			_SetTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER, UPLOAD_PHOTOWIN_INTERVAL, G_TmUploadPhotoWin);
			
			PRTMSG(MSG_DBG, "New Upload, but last photo is uploading, Set resend timer\n"); 
		}
		else if( !m_objUploadPhotoSta.m_bPhotoBeginUpload )
		{
			// ���͵�һ����Ƭ��һ������
			_ClearUploadPhotoStaForNewPhoto();
			_UploadOnePhotoWin( m_objUploadPhotoSta.m_aryUpPhotoInfo[0].m_wPhotoIndex, false, 0, 0, NULL );
			
			PRTMSG(MSG_DBG, "New Upload, but last photo hasn't uploaded, upload last photo first\n"); 
		}
#endif

#if USE_PHOTOUPLD == 1
		// �����ʱ��δ���͵���Ƭ����
		tagPhotoInfo aryTempIndex[ UPLOADPHOTO_MAXCOUNT ];
		BYTE bytOldTotal = m_objUploadPhotoSta.m_bytAllPhotoTotal - m_objUploadPhotoSta.m_bytAllPhotoCt; // ��δ���͵�����
		memcpy( aryTempIndex, m_objUploadPhotoSta.m_aryUpPhotoInfo + m_objUploadPhotoSta.m_bytAllPhotoCt,
			bytOldTotal * sizeof(aryTempIndex[0]) );

		// δ���͵ľ���Ƭ������ǰ
		if( bytOldTotal > UPLOADPHOTO_MAXCOUNT ) bytOldTotal = UPLOADPHOTO_MAXCOUNT; // ���մ���
		memcpy( m_objUploadPhotoSta.m_aryUpPhotoInfo, aryTempIndex,
			bytOldTotal * sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );

		// ����Ƭ�������
		v_bytPhotoTotal = bytOldTotal + v_bytPhotoTotal <= UPLOADPHOTO_MAXCOUNT
			? v_bytPhotoTotal : UPLOADPHOTO_MAXCOUNT - bytOldTotal;
		memcpy( m_objUploadPhotoSta.m_aryUpPhotoInfo + bytOldTotal, v_aryPhoto,
			v_bytPhotoTotal * sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );

		// �����ܹ�Ҫ������Ƭ������
		m_objUploadPhotoSta.m_bytAllPhotoTotal = bytOldTotal + v_bytPhotoTotal;
#endif
	}
	else
	{
		// �����״̬,��ʼ����״̬
		m_objUploadPhotoSta.Init();
		//m_objUploadPhotoSta.m_wPhotoIndex = m_objUploadPhotoSta.m_aryPhotoInfo[0].m_wPhotoIndex;
		
		// ��������Ƭ����
		if( v_bytPhotoTotal > UPLOADPHOTO_MAXCOUNT ) v_bytPhotoTotal = UPLOADPHOTO_MAXCOUNT; // �������
		memcpy( m_objUploadPhotoSta.m_aryUpPhotoInfo, v_aryPhoto,
			v_bytPhotoTotal * sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo[0]) );
		
		// �����ܹ�Ҫ������Ƭ������
		m_objUploadPhotoSta.m_bytAllPhotoTotal = v_bytPhotoTotal;
		
		// �ѷ�����Ƭ������0
		m_objUploadPhotoSta.m_bytAllPhotoCt = 0;
		
		printf("\n");
		PRTMSG(MSG_DBG, "New Upload,Total %d,First Index:%d\n",
			m_objUploadPhotoSta.m_bytAllPhotoTotal, m_objUploadPhotoSta.m_aryUpPhotoInfo[0].m_wPhotoIndex );

#if USE_PHOTOUPLD == 0		
		// ���͵�һ����Ƭ��һ������
		_ClearUploadPhotoStaForNewPhoto();
		_UploadOnePhotoWin( m_objUploadPhotoSta.m_aryUpPhotoInfo[0].m_wPhotoIndex, false, 0, 0, NULL );
#endif
#if USE_PHOTOUPLD == 1
		_KillTimer(&g_objTimerMng, UPLOAD_PHOTOCHK_TIMER );
		_UploadOnePhoto( m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex );
#endif
	}
}

/// �ϴ�һ����Ƭ: ����һ������,���߷���һ����Ƭ�����˳�,���м���ʧ��,Ҳ�˳�,����ֹͣ�ϴ���Ƭ
/// ��������еİ����,��δת���
int CPhoto::_UploadOnePhotoWin( WORD v_wPhotoIndex, bool v_bSpcyFrame, WORD v_wBeginFrameNo,
							   WORD v_wSpcyFrameTotal, WORD* v_arySpcyFrame )
{
	PRTMSG(MSG_DBG, "Upload One Win of the photo\n" );

	int iRet = 0;
	int iBufLen = 0;
	int iBigFrameLen = 0;
	long lPos = 0;
	char buf[ SOCK_MAXSIZE ];
	char szSendBuf[ SOCK_MAXSIZE ];
	int  iSendLen = 0;
	char szFrame[ UDPDATA_SIZE ]; // ��Ƭ������
	char szBigFrame[ UDPDATA_SIZE + 200 ]; // ��Ƭ֡
	DWORD dwFrameLen = 0;
	DWORD dwSndBytes = 0;
	FILE* fpPhoto = NULL;
	LINUX_HANDLE hPhotoFnd = {0};
	LINUX_HANDLE hDirFnd = {0};
	LINUX_FIND_DATA fdPhoto;
	LINUX_FIND_DATA fdDir;
	char szPhotoName[255] = {0};
	char szPhotoPath[255] = {0};
	char szPhotoIndex[ 32 ] = { 0 };
	DWORD dwPhotoDate = 0;
	WORD wCurFrameNo = 0, wFrameCt = 0, wFrameTotal = 0;
	bool bFillFrame = false; // ѭ�����Ƿ������һ֡�ı�־
	bool bWinEnd = false; // �Ƿ���һ�����ڵı�־
	BYTE byt = 0;
	BYTE bytSndFrameCt = 0;
	bool bReqLiuPause = false;
	char szTempBuf1[255] = {0};
	char szTempBuf2[255] = {0};

	// �������
	if( 0 == v_wPhotoIndex )
	{
		PRTMSG(MSG_ERR, "9\n" );
		iRet = ERR_PAR;
		goto _UPLOADONEPHOTOWIN_FAIL;
	}
	if( v_bSpcyFrame )
	{
		if( !v_wSpcyFrameTotal || !v_arySpcyFrame )
		{
			PRTMSG(MSG_ERR, "10\n" );
			iRet = ERR_PAR;
			goto _UPLOADONEPHOTOWIN_FAIL;
		}
		if( v_wSpcyFrameTotal > UPLOADPHOTO_WINFRAMETOTAL )
		{
			PRTMSG(MSG_ERR, "11\n" );
			iRet = ERR_PAR;
			goto _UPLOADONEPHOTOWIN_FAIL;
		}

		if( v_wSpcyFrameTotal >= 2 )
		{
			// ����������������,��Ϊ�������������ĳ��������Ƭ�����һ��,���ͽ���,����,Ҫ���������������
			qsort( v_arySpcyFrame, v_wSpcyFrameTotal, sizeof(v_arySpcyFrame[0]), SortWord );

			// �����ټ��,�ж�����İ�����Ƿ�ͬ����һ������,ע��!
			if( v_arySpcyFrame[ 0 ] / UPLOADPHOTO_WINFRAMETOTAL
				!= v_arySpcyFrame[ v_wSpcyFrameTotal - 1 ] / UPLOADPHOTO_WINFRAMETOTAL )
			{
				PRTMSG(MSG_ERR, "12\n" );
				iRet = ERR_PAR;
				goto _UPLOADONEPHOTOWIN_FAIL;
			}
		}
	}

	//�ϴ�ǰ����ϴ��жϱ�־
	m_objUploadPhotoSta.m_bUploadBreak = false;

	// ��Ҫ���͵���Ƭ��״̬��¼��һ��,�����״̬��¼������,�Ա�֮���¼��״̬ (��Ҫ)
	if( v_wPhotoIndex != m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex )
		_ClearUploadPhotoStaForNewPhoto();

	// ��ȡ��Ƭ���ڵ������ļ���
	dwPhotoDate = m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_dwPhotoDate;

	// ��Ҫ���͵İ���״̬�еļ�¼����ͬһ����,�򲿷����,�Ա㴫���´���
	if( v_bSpcyFrame )
	{
		if( v_arySpcyFrame[0] / UPLOADPHOTO_WINFRAMETOTAL != m_objUploadPhotoSta.m_wPhotoWinCt )
		{
			_ClearUploadPhotoStaForNewWin(); // �������״̬
			m_objUploadPhotoSta.m_wPhotoWinCt = v_arySpcyFrame[0] / UPLOADPHOTO_WINFRAMETOTAL; // ��������ɴ��ڼ��� (ȷ��״̬��ȷ)
		}
	}
	else
	{
		if( v_wBeginFrameNo / UPLOADPHOTO_WINFRAMETOTAL != m_objUploadPhotoSta.m_wPhotoWinCt )
		{
			_ClearUploadPhotoStaForNewWin(); // �������״̬
			m_objUploadPhotoSta.m_wPhotoWinCt = v_wBeginFrameNo / UPLOADPHOTO_WINFRAMETOTAL; // ��������ɴ��ڼ��� (ȷ��״̬��ȷ)
		}
	}

	/// ׼����ƬĿ¼��
	memcpy(szPhotoPath, m_szPhotoPath, sizeof(m_szPhotoPath));
	sprintf( szPhotoIndex, "%04x", v_wPhotoIndex );

	wFrameCt = 0;
	do
	{
		/// ȷ����ǰҪ���͵İ���� (����Ŵ�0��ʼ���)
		if( v_bSpcyFrame ) // ����ָ��֡ (Ӧ���Ǵ����ط�)
		{
			wCurFrameNo = v_arySpcyFrame[ wFrameCt ];
		}
		else if( v_wPhotoIndex == m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex )
			// ��Ҫ���͵���Ƭ��״̬��¼�е���� (Ӧ����Ҫ������Ƭ���´���)
		{
			wCurFrameNo = v_wBeginFrameNo + wFrameCt;
		}
		else
		{
			// ֻ���ܵ�һ��ѭ��ʱ����
			wCurFrameNo = 0;
		}
		if( INVALID_NUM_16 == wCurFrameNo )
		{
			PRTMSG(MSG_ERR, "13\n" );
			iRet = ERR_PAR;
			goto _UPLOADONEPHOTOWIN_FAIL;
		}

		/// ��䵱ǰҪ���͵İ�����
		bFillFrame = false;
		dwFrameLen = 0;
		if( v_wPhotoIndex == m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_wPhotoIndex )
			// ��״̬�е���Ƭ��Ҫ���͵���Ƭ��ͬ
		{
			// ���������Ƿ����л����֡
			for( byt = 0; byt < UPLOADPHOTO_WINFRAMETOTAL; byt ++ )
			{
				if( wCurFrameNo == m_objUploadPhotoSta.m_aryPhotoFrameNo[ byt ] ) // ���û��������뵱ǰ��Ҫ�İ���ͬ
				{
					// ��ȫ���������Ƿ�Ϸ�
					if( 0 != m_objUploadPhotoSta.m_aryPhotoFrameLen[ byt ]
						&& DWORD(m_objUploadPhotoSta.m_aryPhotoFrameLen[ byt ]) > sizeof(szFrame) )
					{
						// ���ð����Ȳ��Ϸ�,����ð�����
						m_objUploadPhotoSta.m_aryPhotoFrameNo[ byt ] = 0xffff;
						break;
					}

					memcpy( szFrame, m_objUploadPhotoSta.m_aryPhotoFrameBuf[ byt ],
						m_objUploadPhotoSta.m_aryPhotoFrameLen[ byt ] );
					dwFrameLen = m_objUploadPhotoSta.m_aryPhotoFrameLen[ byt ];
					bFillFrame = true;
					
					break;
				}
			}
		}
		
		if( !bFillFrame ) // ��û�����(������״̬�е���Ƭ�͵�ǰҪ������Ƭ��ͬ,Ҳ������״̬�в���Ҫ���Ͱ��Ļ�������),�����Ƭ�ļ��ж�ȡ
		{
			// ��û�д򿪹�,Ѱ�Ҳ���
			// �ȳ��Դ�״̬��¼�е���Ƭ�ļ���
			// (�����¼�е���Ƭ�͵�ǰ��Ƭ��ͬ,��¼�е��ļ����ѱ������,��򿪲�����ʧ��,û��ϵ,����ִ��)
			if( !fpPhoto )
			{
				memcpy(szPhotoName, m_objUploadPhotoSta.m_szPhotoPath, MAX_PATH);
				fpPhoto = fopen( szPhotoName, "rb" );
			}

			// ���򲻿�,������Ƭ����Ŀ¼����Ч�������ƬĿ¼ֱ�Ӵ�
			if( !fpPhoto && dwPhotoDate >= BELIV_DATE )
			{
				char szDir[30];
				sprintf( szDir, "%d/", dwPhotoDate );
				
				hPhotoFnd = FindFirstFile( SPRINTF(szTempBuf1, "%s%s", szPhotoPath, szDir), SPRINTF(szTempBuf2, "%s%s", szPhotoIndex, "_*.jpg"), &fdPhoto, false );
				if( hPhotoFnd.bValid )
				{
					if( !(S_IFDIR & fdPhoto.objFileStat.st_mode) )
					{
						sprintf(szPhotoName, "%s%s%s", szPhotoPath, szDir, fdPhoto.szFileName);// ��¼��ǰ��Ƭ·��
						fpPhoto = fopen( szPhotoName, "rb" );
					}
					memset(&hPhotoFnd, 0, sizeof(LINUX_HANDLE));
				}
			}

			if( !fpPhoto ) // �����Ǵ򲻿�, Ѱ�Ҹ�������Ƭ�ļ�
			{				
				/// ������ƬĿ¼
				hDirFnd = FindFirstFile( szPhotoPath, "*", &fdDir, false );
				if( hDirFnd.bValid )
				{
					do
					{
						// ������Ŀ¼,����
						if( !(S_IFDIR & fdDir.objFileStat.st_mode) ) 
							continue;
						
						// ������Ч����Ŀ¼,����
						if( !strcmp(".", fdDir.szFileName) || !strcmp("..", fdDir.szFileName) )
							continue;
						
						// ������Ƭ�ļ�
						hPhotoFnd = FindFirstFile( SPRINTF(szTempBuf1, "%s%s%s", szPhotoPath, fdDir.szFileName, "/"), SPRINTF(szTempBuf2, "%s%s", szPhotoIndex, "_*.jpg"), &fdPhoto, false );
						if( hPhotoFnd.bValid )
						{
							do
							{
								if( S_IFDIR & fdPhoto.objFileStat.st_mode )
									continue;
								
								// ��¼��ǰ��Ƭ·��
								sprintf(szPhotoName, "%s%s%s%s", szPhotoPath, fdDir.szFileName, "/", fdPhoto.szFileName);
							
								fpPhoto = fopen(szPhotoName, "rb" );	
							} while( !fpPhoto && FindNextFile(&hPhotoFnd, &fdPhoto, false) );
							
							memset(&hPhotoFnd, 0, sizeof(LINUX_HANDLE));
						}
					} while( !fpPhoto && FindNextFile(&hDirFnd, &fdDir, false) );
				
					memset(&hDirFnd, 0, sizeof(LINUX_HANDLE));
				}
			}

			if( !fpPhoto ) // ��û���ҵ�
			{
				PRTMSG( MSG_DBG, "Find Photo File Fail in Disk,Index: %04x\n", szPhotoIndex );
				iRet = ERR_FILE_FAILED;
				goto _UPLOADONEPHOTOWIN_FAIL;
			}
			else if( strcmp(szPhotoName, m_objUploadPhotoSta.m_szPhotoPath) != 0  )
				// ������״̬��¼�е���Ƭ�͵�ǰ��Ƭ��ͬ,��Ҫ��ȡ��ǰ��Ƭ��Ϣ
			{
				// �����ļ���С
				if( fseek(fpPhoto, 0, SEEK_END) )
				{
					PRTMSG( MSG_DBG, "2\n" );
					iRet = ERR_FILE_FAILED;
					goto _UPLOADONEPHOTOWIN_FAIL;
				}
				long lEnd = ftell( fpPhoto );
				if( lEnd <= 0 )
				{
					PRTMSG( MSG_DBG, "14\n" );
					iRet = ERR_FILE_FAILED;
					goto _UPLOADONEPHOTOWIN_FAIL;
				}
				DWORD dwFileSize = DWORD( lEnd );
				if( 0 == dwFileSize )
				{
					PRTMSG( MSG_DBG, "3\n" );
					iRet = ERR_FILE_FAILED;
					goto _UPLOADONEPHOTOWIN_FAIL;
				}

				// ���ļ�����ȡ��Ƭ�����Ϣ
				DWORD dwDate, dwTime;
				long lLon, lLat;
				WORD wPhotoIndex, wPhotoType;
				BYTE bytGpsValid, bytSizeType, bytQualityType, bytChannel;
				IN_GetIndexFromPhotoName( szPhotoName, wPhotoIndex );
				IN_GetDateTimeFromPhotoName( szPhotoName, dwDate, dwTime );
				IN_GetTypeFromPhotoName( szPhotoName, wPhotoType );
				IN_GetLonLatFromPhotoName( szPhotoName, lLon, lLat, bytGpsValid );
				IN_GetChannelSizeQualityFromPhotoName( szPhotoName, bytChannel, bytSizeType, bytQualityType );

				// �ֱ������ͻ�ԭΪЭ�鶨�巽ʽ
				switch( bytSizeType )
				{
				case 1:
					bytSizeType = 2;
					break;
				case 2:
					bytSizeType = 1;
					break;
				case 3:
					//bytSizeType = 1; // �ϴ�˵�ô���Ƭ���еȳߴ���ʾ���ܸ����
					bytSizeType = 3; // ���Ǳ�����ߴ���ʾ������������
					break;
				default:
					bytSizeType = 2;
				}
				
				// ��ȫ����ж�
				if( wPhotoIndex != v_wPhotoIndex )
				{
					PRTMSG( MSG_DBG, "4\n" );
					iRet = ERR_FILE_FAILED;
					goto _UPLOADONEPHOTOWIN_FAIL;
				}
				
				// ��¼��״̬ (�����ظ�д����ͬ��,��Ҫ��,��һ�㲻Ӧ���ִ������)
				m_objUploadPhotoSta.m_dwPhotoSize = dwFileSize; // ��Ƭ�ߴ�
				m_objUploadPhotoSta.m_bytPhotoSizeType = bytSizeType; // �ֱ�������
				m_objUploadPhotoSta.m_bytPhotoQualityType = bytQualityType; // ��������
				m_objUploadPhotoSta.m_wPhotoFrameTotal = WORD( m_objUploadPhotoSta.m_dwPhotoSize
					/ UDPDATA_SIZE + (m_objUploadPhotoSta.m_dwPhotoSize % UDPDATA_SIZE ? 1 : 0) ); // �ܰ���
				_ConvertLonLatFromLongToStr( lLon / LONLAT_DO_TO_L, lLat / LONLAT_DO_TO_L,
					m_objUploadPhotoSta.m_szLonLat, sizeof(m_objUploadPhotoSta.m_szLonLat) ); // ��γ��
				m_objUploadPhotoSta.m_wPhotoType = wPhotoType; // ��Ƭ����
				m_objUploadPhotoSta.m_bytPhotoChannel = bytChannel; // ͨ����
				m_objUploadPhotoSta.m_szDateTime[ 0 ] = BYTE(dwDate / 10000); // ����ʱ��
				m_objUploadPhotoSta.m_szDateTime[ 1 ] = BYTE(dwDate % 10000 / 100);
				m_objUploadPhotoSta.m_szDateTime[ 2 ] = BYTE(dwDate % 100);
				m_objUploadPhotoSta.m_szDateTime[ 3 ] = BYTE(dwTime / 10000);
				m_objUploadPhotoSta.m_szDateTime[ 4 ] = BYTE(dwTime % 10000 / 100);
				m_objUploadPhotoSta.m_szDateTime[ 5 ] = BYTE(dwTime % 100);
				
				// ��Ƭȫ·����
				strncpy( m_objUploadPhotoSta.m_szPhotoPath, szPhotoName,
					sizeof(m_objUploadPhotoSta.m_szPhotoPath) / sizeof(m_objUploadPhotoSta.m_szPhotoPath[0]) );
				m_objUploadPhotoSta.m_szPhotoPath[ sizeof(m_objUploadPhotoSta.m_szPhotoPath) / sizeof(m_objUploadPhotoSta.m_szPhotoPath[0]) - 1 ] = 0;
				
				PRTMSG( MSG_DBG, "Cur Photo File: %s\n", szPhotoName );
				PRTMSG( MSG_DBG, "Photo File Size:%d, Packets Total: %d\n", 
				                 m_objUploadPhotoSta.m_dwPhotoSize, m_objUploadPhotoSta.m_wPhotoFrameTotal );
			}
			

			//////// ֱ�Ӵ���Ƭ�ļ���ȡ��Ƭ���� /////////

			// ��ת����ȡ��
			if( fseek(fpPhoto, wCurFrameNo * UDPDATA_SIZE, SEEK_SET) )
			{
				PRTMSG( MSG_DBG, "5\n" );
				iRet = ERR_FILE_FAILED;
				goto _UPLOADONEPHOTOWIN_FAIL;
			}
			
			// �ж����ڿ��Զ�ȡ�������� (Ҳ�ɴ˵�֪����Ƭ�ô����Ƿ��ϴ����)
			lPos = ftell( fpPhoto );
			if( lPos < 0 || lPos > long(m_objUploadPhotoSta.m_dwPhotoSize) )
			{
				PRTMSG( MSG_DBG, "6\n" );
				iRet = ERR_FILE_FAILED;
				goto _UPLOADONEPHOTOWIN_FAIL;
			}
			dwFrameLen = DWORD(m_objUploadPhotoSta.m_dwPhotoSize - lPos);
			if( dwFrameLen <= UDPDATA_SIZE )
				bWinEnd = true; // ��־��Ƭ�ϴ����,ͬʱ�ô���Ҳ����
			else
				dwFrameLen = UDPDATA_SIZE;
			
			// ��ȡ
			if( 1 != fread(szFrame, dwFrameLen, 1, fpPhoto) )
			{
				PRTMSG( MSG_DBG, "7: Pos = %d, Read bytes = %d, Total = %d\n", lPos, dwFrameLen, m_objUploadPhotoSta.m_dwPhotoSize );
				
				iRet = ERR_FILE_FAILED;
				goto _UPLOADONEPHOTOWIN_FAIL;
			}
			bFillFrame = true;
			
			// ��������ݰ�
			memcpy( m_objUploadPhotoSta.m_aryPhotoFrameBuf[ wFrameCt ], szFrame, dwFrameLen );
			m_objUploadPhotoSta.m_aryPhotoFrameLen[ wFrameCt ] = WORD( dwFrameLen );
			m_objUploadPhotoSta.m_aryPhotoFrameNo[ wFrameCt ] = wCurFrameNo;
		}

		// ��û�гɹ����ð�,�˳�
		if( !bFillFrame )
		{
			PRTMSG(MSG_DBG, "16\n" );
			goto _UPLOADONEPHOTOWIN_FAIL;
		}

		// �����ѷ��Ͱ�����,���ж��Ƿ����ô���
		if( wCurFrameNo + 1 >= m_objUploadPhotoSta.m_wPhotoFrameTotal ) bWinEnd = true;
		wFrameCt ++;
		if( v_wSpcyFrameTotal > 0 && wFrameCt >= v_wSpcyFrameTotal ) bWinEnd = true;
		else if( wFrameCt >= UPLOADPHOTO_WINFRAMETOTAL ) bWinEnd = true;

		// �������ݰ� ��־λ(1) + ��ǰͼ�������ţ�2��+ �ܰ�����2�� + ��ǰ����ţ�2��
		// + ����ʼʱ�䣨6��+ ��ʼ��γ�ȣ�8��+�ֱ��ʣ�1��+ͼ�������ȼ���1��+�¼����ͣ�1��+ ͨ���ţ�1����+���ݳ���(2) + ��������(n)
		szBigFrame[ 0 ] = 0x21 | (bWinEnd ? 0x40 : 0) | (m_objUploadPhotoSta.m_wPhotoType & ~PHOTOEVT_CENT ? 0x80 : 0);
		szBigFrame[ 1 ] = BYTE(v_wPhotoIndex / 0x100);
		szBigFrame[ 2 ] = BYTE(v_wPhotoIndex % 0x100);
		szBigFrame[ 3 ] = m_objUploadPhotoSta.m_wPhotoFrameTotal / 0x100;
		szBigFrame[ 4 ] = m_objUploadPhotoSta.m_wPhotoFrameTotal % 0x100;
		szBigFrame[ 5 ] = wCurFrameNo / 0x100;
		szBigFrame[ 6 ] = wCurFrameNo % 0x100;
		if( 0 == wCurFrameNo ) // ������Ƭ�ĵ�һ������
		{
			memcpy( szBigFrame + 7, m_objUploadPhotoSta.m_szDateTime, 6 );
			memcpy( szBigFrame + 13, m_objUploadPhotoSta.m_szLonLat, 8 );
			szBigFrame[ 21 ] = m_objUploadPhotoSta.m_bytPhotoSizeType;
			szBigFrame[ 22 ] = m_objUploadPhotoSta.m_bytPhotoQualityType;
			szBigFrame[ 23 ] = _PhotoTypeToEvtType( m_objUploadPhotoSta.m_wPhotoType );
			szBigFrame[ 24 ] = 1 == m_objUploadPhotoSta.m_bytPhotoChannel || 2 == m_objUploadPhotoSta.m_bytPhotoChannel 
				|| 4 == m_objUploadPhotoSta.m_bytPhotoChannel || 8 == m_objUploadPhotoSta.m_bytPhotoChannel 
				? m_objUploadPhotoSta.m_bytPhotoChannel : 1;
			iBigFrameLen = 25;
		}
		else
		{
			iBigFrameLen = 7;
		}
		szBigFrame[ iBigFrameLen ++ ] = char( dwFrameLen / 0x100 );
		szBigFrame[ iBigFrameLen ++ ] = char( dwFrameLen % 0x100 );
		if( DWORD(iBigFrameLen) + dwFrameLen > sizeof(szBigFrame) )
		{
			PRTMSG(MSG_DBG, "17\n" );
			iRet = ERR_MEMLACK;
			goto _UPLOADONEPHOTOWIN_FAIL;
		}
		memcpy( szBigFrame + iBigFrameLen, szFrame, dwFrameLen );
		iBigFrameLen += int( dwFrameLen );

		// �������ݰ����Ƶ�SockServExe����
		iRet = g_objSms.MakeSmsFrame( szBigFrame, iBigFrameLen, 0x41, 0x50, buf, sizeof(buf), iBufLen, CVT_NONE, false );
		
		szSendBuf[0] = 0x04;
		iRet = g_objSock.MakeUdpFrame(buf, iBufLen, 0x02, 0x20, szSendBuf+1, sizeof(szSendBuf)-1, iSendLen);
		if( !iRet )
		{
			iRet = DataPush(szSendBuf, iSendLen+1, DEV_QIAN, DEV_SOCK, LV2);
		}

		if( !iRet )
		{
			dwSndBytes += DWORD(iBufLen);
			++ bytSndFrameCt;
			m_objUploadPhotoSta.m_bPhotoBeginUpload = true;
		}

		// ������󱾴��ڵ����һ������������Ϣ,�����ط�����,�����ش���ʱ��
		if( bWinEnd )
		{
			memcpy( &m_objUploadPhotoSta.m_obj4152, szBigFrame, sizeof(m_objUploadPhotoSta.m_obj4152) );
			m_objUploadPhotoSta.m_obj4152.m_bytReSndTime = 0;

			DWORD dwInterval = 5000;
			_KillTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );
			_SetTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER, dwInterval, G_TmUploadPhotoWin ); // �ط���ʱ
		}
	} while( !bWinEnd ); // �����ʹ���δ�������ѭ��

	// �����ͳɹ�
	iRet = 0;

	PRTMSG(MSG_DBG, "This Win of the Photo Pushed to Send!\n" );

	goto _UPLOADONEPHOTOWIN_END;

_UPLOADONEPHOTOWIN_FAIL:
	_StopUploadPhoto();
	PRTMSG(MSG_ERR, "Upload this Win of the photo Fail, err code:%d\n", iRet );

_UPLOADONEPHOTOWIN_END:
	if( fpPhoto ) 
		fclose( fpPhoto );

	PRTMSG(MSG_NOR, "Sended this win, Packets Num: %d\n", bytSndFrameCt );

	return iRet;
}

int CPhoto::_UploadOnePhoto( WORD v_wPhotoIndex )
{
	PRTMSG(MSG_NOR, "Upload One photo\n" );

	char szTempBuf1[1024];
	char szTempBuf2[1024];

	int iRet = 0;
	FILE* fpPhoto = NULL;
	LINUX_HANDLE hPhotoFnd = {0};
	LINUX_HANDLE hDirFnd = {0};
	LINUX_FIND_DATA fdPhoto;
	LINUX_FIND_DATA fdDir;
	char szPhotoPath[255] = {0};
	char szPhotoName[255] = {0};
	DWORD dwPhotoDate = 0;
	DWORD dwFileSize = 0;
	DWORD dwReadLen = 0;
	int iBufLen = 0;
	char szPhotoIndex[ 32 ] = { 0 };
	char szPhoto[ TCP_SENDSIZE ] = { 0 };
	char buf[ SOCK_MAXSIZE ] = { 0 };

	// ��ȡ��Ƭ���ڵ������ļ���
	dwPhotoDate = m_objUploadPhotoSta.m_aryUpPhotoInfo[ m_objUploadPhotoSta.m_bytAllPhotoCt ].m_dwPhotoDate;

	/// ׼����ƬĿ¼��
	memcpy(szPhotoPath, m_szPhotoPath, sizeof(m_szPhotoPath));
	sprintf( szPhotoIndex, "%04x", v_wPhotoIndex );

	// ���򲻿�,������Ƭ����Ŀ¼����Ч�������ƬĿ¼ֱ�Ӵ�
	if( !fpPhoto && dwPhotoDate >= BELIV_DATE )
	{
		char szDir[30];
		sprintf( szDir, "%d/", dwPhotoDate );
 
		hPhotoFnd = FindFirstFile( SPRINTF(szTempBuf1, "%s%s", szPhotoPath, szDir), SPRINTF(szTempBuf2, "%s%s", szPhotoIndex, "_*.jpg"), &fdPhoto, false );
		if( hPhotoFnd.bValid )
		{
			if( !(S_IFDIR & fdPhoto.objFileStat.st_mode) )
			{
				// ��¼��ǰ��Ƭ·��
				sprintf(szPhotoName, "%s%s%s", szPhotoPath, szDir, fdPhoto.szFileName);
				fpPhoto = fopen( szPhotoName, "rb" );
			}
			memset(&hPhotoFnd, 0, sizeof(LINUX_HANDLE));
		}
	}

	if( !fpPhoto ) // �����Ǵ򲻿�, Ѱ�Ҹ�������Ƭ�ļ�
	{				
		/// ������ƬĿ¼
		hDirFnd = FindFirstFile( szPhotoPath, "*", &fdDir, false );
		if( hDirFnd.bValid )
		{
			do
			{
				// ������Ŀ¼,����
				if( !(S_IFDIR & fdDir.objFileStat.st_mode) ) continue;
				
				// ������Ч����Ŀ¼,����
				if( !strcmp(".", fdDir.szFileName) || !strcmp("..", fdDir.szFileName) ) continue;
				
				// ������Ƭ�ļ�
				hPhotoFnd = FindFirstFile( SPRINTF(szTempBuf1, "%s%s%s", szPhotoPath, fdDir.szFileName, "/"), SPRINTF(szTempBuf2, "%s%s", szPhotoIndex, "_*.jpg"), &fdPhoto, false );
				if( hPhotoFnd.bValid )
				{
					do
					{
						if( S_IFDIR & fdPhoto.objFileStat.st_mode ) continue;
						
						sprintf(szPhotoName, "%s%s%s%s", szPhotoPath, fdDir.szFileName, "/", fdPhoto.szFileName);// ��¼��ǰ��Ƭ·��
						fpPhoto = fopen(szPhotoName, "rb" );
						
					} while( !fpPhoto && FindNextFile(&hPhotoFnd, &fdPhoto, false) );
				
					memset(&hPhotoFnd, 0, sizeof(LINUX_HANDLE));
				}
			} while( !fpPhoto && FindNextFile(&hDirFnd, &fdDir, false) );
		
			memset(&hDirFnd, 0, sizeof(LINUX_HANDLE));
		}
	}

	if( !fpPhoto ) // ��û���ҵ�
	{
		PRTMSG( MSG_DBG, "Find Photo File Fail in Disk,Index: %04x\n", szPhotoIndex );
		iRet = ERR_FILE_FAILED;
		goto _UPLOADONEPHOTO_FAIL;
	}
	// 	else if( strcmp(strPhotoName, m_objUploadPhotoSta.m_wszPhotoPath) != 0 )
	// 		// ������״̬��¼�е���Ƭ�͵�ǰ��Ƭ��ͬ,��Ҫ��ȡ��ǰ��Ƭ��Ϣ
	{
		// �����ļ���С
		if( fseek(fpPhoto, 0, SEEK_END) )
		{
			PRTMSG( MSG_DBG, "2\n" );
			iRet = ERR_FILE_FAILED;
			goto _UPLOADONEPHOTO_FAIL;
		}
		long lEnd = ftell( fpPhoto );
		if( lEnd <= 0 )
		{
			PRTMSG( MSG_DBG, "14\n" );
			iRet = ERR_FILE_FAILED;
			goto _UPLOADONEPHOTO_FAIL;
		}
		dwFileSize = DWORD( lEnd );
		if( 0 == dwFileSize )
		{
			PRTMSG( MSG_DBG, "3\n" );
			iRet = ERR_FILE_FAILED;
			goto _UPLOADONEPHOTO_FAIL;
		}
		
		// ���ļ�����ȡ��Ƭ�����Ϣ
		DWORD dwDate, dwTime;
		long lLon, lLat;
		WORD wPhotoIndex, wPhotoType;
		BYTE bytGpsValid, bytSizeType, bytQualityType, bytChannel;
		IN_GetIndexFromPhotoName( szPhotoName, wPhotoIndex );
		IN_GetDateTimeFromPhotoName( szPhotoName, dwDate, dwTime );
		IN_GetTypeFromPhotoName( szPhotoName, wPhotoType );
		IN_GetLonLatFromPhotoName( szPhotoName, lLon, lLat, bytGpsValid );
		IN_GetChannelSizeQualityFromPhotoName( szPhotoName, bytChannel, bytSizeType, bytQualityType );
		
		// �ֱ������ͻ�ԭΪЭ�鶨�巽ʽ
		switch( bytSizeType )
		{
		case 1:
			bytSizeType = 2;
			break;
		case 2:
			bytSizeType = 1;
			break;
		case 3:
			//bytSizeType = 1; // �ϴ�˵�ô���Ƭ���еȳߴ���ʾ���ܸ����
			bytSizeType = 3; // ���Ǳ�����ߴ���ʾ������������
			break;
		default:
			bytSizeType = 2;
		}
		
		// ��ȫ����ж�
		if( wPhotoIndex != v_wPhotoIndex )
		{
			PRTMSG( MSG_DBG, "4\n" );
			iRet = ERR_FILE_FAILED;
			goto _UPLOADONEPHOTO_FAIL;
		}
		
		// ��¼��״̬ (�����ظ�д����ͬ��,��Ҫ��,��һ�㲻Ӧ���ִ������)
		m_objUploadPhotoSta.m_dwPhotoSize = dwFileSize; // ��Ƭ�ߴ�
		m_objUploadPhotoSta.m_bytPhotoSizeType = bytSizeType; // �ֱ�������
		m_objUploadPhotoSta.m_bytPhotoQualityType = bytQualityType; // ��������
		m_objUploadPhotoSta.m_wPhotoFrameTotal = WORD( m_objUploadPhotoSta.m_dwPhotoSize
			/ UDPDATA_SIZE + (m_objUploadPhotoSta.m_dwPhotoSize % UDPDATA_SIZE ? 1 : 0) ); // �ܰ���
		_ConvertLonLatFromLongToStr( lLon / LONLAT_DO_TO_L, lLat / LONLAT_DO_TO_L,
			m_objUploadPhotoSta.m_szLonLat, sizeof(m_objUploadPhotoSta.m_szLonLat) ); // ��γ��
		m_objUploadPhotoSta.m_wPhotoType = wPhotoType; // ��Ƭ����
		m_objUploadPhotoSta.m_bytPhotoChannel = bytChannel; // ͨ����
		m_objUploadPhotoSta.m_szDateTime[ 0 ] = BYTE(dwDate / 10000); // ����ʱ��
		m_objUploadPhotoSta.m_szDateTime[ 1 ] = BYTE(dwDate % 10000 / 100);
		m_objUploadPhotoSta.m_szDateTime[ 2 ] = BYTE(dwDate % 100);
		m_objUploadPhotoSta.m_szDateTime[ 3 ] = BYTE(dwTime / 10000);
		m_objUploadPhotoSta.m_szDateTime[ 4 ] = BYTE(dwTime % 10000 / 100);
		m_objUploadPhotoSta.m_szDateTime[ 5 ] = BYTE(dwTime % 100);

		PRTMSG( MSG_DBG, "Cur Photo File: %s\n", szPhotoName );
		PRTMSG( MSG_DBG, "Photo File Size:%d, Packets Total: %d\n", 
			m_objUploadPhotoSta.m_dwPhotoSize, m_objUploadPhotoSta.m_wPhotoFrameTotal );
	}

	// ʱ�䣨6��+ γ�ȣ�4��+ ���ȣ�4��+�ֱ��ʣ�1��+ͼ�������ȼ���1��+�¼����ͣ�1��+ ͨ���ţ�1��+�����ţ�2��+���ݳ��ȣ�4��+���ݣ�n��
#define PHOTOHEAD_LEN	24

	// ��ת����ʼλ��
	if( fseek(fpPhoto, 0, SEEK_SET) )
	{
		PRTMSG( MSG_DBG, "5\n" );
		iRet = ERR_FILE_FAILED;
		goto _UPLOADONEPHOTO_FAIL;
	}
	
	// ��ȡ
	if( dwFileSize + PHOTOHEAD_LEN > sizeof(szPhoto) )
	{
		iRet = ERR_BUFLACK;
		PRTMSG( MSG_DBG, "6\n" );
		goto _UPLOADONEPHOTO_FAIL;
	}
	dwReadLen = fread(szPhoto + PHOTOHEAD_LEN, dwFileSize, 1, fpPhoto);
	if( 1 != dwReadLen )
	{
		PRTMSG( MSG_DBG, "7: Read bytes != FileSize(%d)\n", dwFileSize );
		
		iRet = ERR_FILE_FAILED;
		goto _UPLOADONEPHOTO_FAIL;
	}
	
	memcpy( szPhoto, m_objUploadPhotoSta.m_szDateTime, 6 );
	memcpy( szPhoto + 6, m_objUploadPhotoSta.m_szLonLat + 4, 4 );
	memcpy( szPhoto + 10, m_objUploadPhotoSta.m_szLonLat, 4 );
	szPhoto[ 14 ] = m_objUploadPhotoSta.m_bytPhotoSizeType;
	szPhoto[ 15 ] = m_objUploadPhotoSta.m_bytPhotoQualityType;
	szPhoto[ 16 ] = _PhotoTypeToEvtType( m_objUploadPhotoSta.m_wPhotoType );
	szPhoto[ 17 ] = 1 == m_objUploadPhotoSta.m_bytPhotoChannel || 2 == m_objUploadPhotoSta.m_bytPhotoChannel 
		|| 4 == m_objUploadPhotoSta.m_bytPhotoChannel || 8 == m_objUploadPhotoSta.m_bytPhotoChannel 
		? m_objUploadPhotoSta.m_bytPhotoChannel : 1;
	szPhoto[ 18 ] = BYTE(v_wPhotoIndex / 0x100);
	szPhoto[ 19 ] = BYTE(v_wPhotoIndex % 0x100);
	szPhoto[ 20 ] = char( dwFileSize / 0x1000000 );
	szPhoto[ 21 ] = char( dwFileSize % 0x1000000 / 0x10000 );
	szPhoto[ 22 ] = char( dwFileSize % 0x10000 / 0x100 );
	szPhoto[ 23 ] = char( dwFileSize % 0x100 );

	// �������ݰ�
	iRet = g_objSms.MakeSmsFrame( szPhoto, PHOTOHEAD_LEN + dwFileSize, 0x41, 0x40, buf, sizeof(buf), iBufLen, CVT_NONE, true );
	if( !iRet )
	{
		iRet = g_objSock.TcpSendSmsData( buf, iBufLen, LV8, DATASYMB_415052 );
	}

	iRet = 0;

	PRTMSG(MSG_NOR, "This Photo Pushed to Send!\n" );

	_SetTimer( &g_objTimerMng, UPLOAD_PHOTOCHK_TIMER, 60000, G_TmUploadPhotoChk ); // �ط���ʱ

	goto _UPLOADONEPHOTO_END;

_UPLOADONEPHOTO_FAIL:
	_StopUploadPhoto();
	PRTMSG(MSG_NOR, "Upload this photo Fail, err code:%d", iRet );
	
_UPLOADONEPHOTO_END:
	if( fpPhoto ) fclose( fpPhoto );
	return iRet;
}

void CPhoto::_StopUploadPhoto()
{
	// ����ϴ���Ƭ��Ϣ
	memset( m_objUploadPhotoSta.m_aryUpPhotoInfo, 0, sizeof(m_objUploadPhotoSta.m_aryUpPhotoInfo) );
	m_objUploadPhotoSta.m_bytAllPhotoTotal = 0;

	// ��ȫ����Ƭ�����꣬�����������Ϣ
	if( m_objPhotoSta.m_bytPhotoCt == m_objPhotoSta.m_bytPhotoTotal )
		memset((void*)&m_objPhotoSta, 0, sizeof(m_objPhotoSta));
	
	// Ϊ����ϴ�����Ƭ�������״̬��Ϣ
	_ClearUploadPhotoStaForNewPhoto();
	
	// ֹͣ�ش���ʱ
	_KillTimer( &g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );
	
	// ֹͣ��Ƭ�ϴ�Ӧ���鶨ʱ
	_KillTimer( &g_objTimerMng, UPLOAD_PHOTOCHK_TIMER );
	
	PRTMSG( MSG_DBG, "Stop Upload Photo!\n" );
}

/// Ϊ���µĴ��ڻ��µ���Ƭ,��״̬��Ϣ���������
void CPhoto::_ClearUploadPhotoStaForNewWin()
{
	// ��������ط�����
	memset( &m_objUploadPhotoSta.m_obj4152, 0, sizeof(m_objUploadPhotoSta.m_obj4152) );
	
	// ���״̬��¼�еĻ������Ϣ,ʹ���Ժ��ط�ʱ�����󷢲���ȷ�İ�
	memset( m_objUploadPhotoSta.m_aryPhotoFrameNo, 0xff, sizeof(m_objUploadPhotoSta.m_aryPhotoFrameNo) );
	
	// ����ط���־
	m_objUploadPhotoSta.m_byt4152Snd = 0;
	
	// ����ط�����
	m_objUploadPhotoSta.m_obj4152.m_bytReSndTime = 0;
	
	m_objUploadPhotoSta.m_bUploadBreak = false;
}

void CPhoto::_ClearUploadPhotoStaForNewPhoto()
{
	m_objUploadPhotoSta.m_dwPhotoSize = 0; // ��ǰ���ϴ���Ƭ���ļ���С
	m_objUploadPhotoSta.m_wPhotoFrameTotal = 0; // ��ǰ���ϴ���Ƭ���ܰ���
	m_objUploadPhotoSta.m_wPhotoWinCt = 0; // ��ǰ���ϴ���Ƭ������ɵĴ��ڼ���
	
	m_objUploadPhotoSta.m_szPhotoPath[0] = 0; // ��ǰ���ϴ���Ƭ��ȫ·����
	memset( m_objUploadPhotoSta.m_szDateTime, 0, sizeof(m_objUploadPhotoSta.m_szDateTime) ); // ��ǰ���ϴ���Ƭ������ʱ��
	memset( m_objUploadPhotoSta.m_szLonLat, 0, sizeof(m_objUploadPhotoSta.m_szLonLat) ); //��ǰ���ϴ���Ƭ�ľ�γ��
	m_objUploadPhotoSta.m_wPhotoType = 0; // ��ǰ���ϴ���Ƭ���¼�����
	memset( &m_objUploadPhotoSta.m_obj4152, 0, sizeof(m_objUploadPhotoSta.m_obj4152) ); // ��������ط�����
	m_objUploadPhotoSta.m_byt4152Snd = 0; // ����ط���־
	m_objUploadPhotoSta.m_obj4152.m_bytReSndTime = 0; // ����ط�����
	m_objUploadPhotoSta.m_bPhotoBeginUpload = false;
	
	m_objUploadPhotoSta.m_bUploadBreak = false;
	
	// ��ǰ�ϴ���Ƭ��֡���ݻ��� (����δȷ�ϴ����ڰ�����,����)
	memset( m_objUploadPhotoSta.m_aryPhotoFrameNo, 0xff, sizeof(m_objUploadPhotoSta.m_aryPhotoFrameNo) ); // �������,��0��ʼ���,0��ת��, ��=0xffff,˵����λ�û���������Ч
}

bool CPhoto::IsPhotoEnd()
{
	bool bRet = m_objPhotoSta.m_bytPhotoCt >= m_objPhotoSta.m_bytPhotoTotal;
	return bRet;
}

bool CPhoto::IsUploadPhotoEnd()
{
	bool bRet = m_objUploadPhotoSta.m_bytAllPhotoCt >= m_objUploadPhotoSta.m_bytAllPhotoTotal;
	return bRet;
}

bool CPhoto::IsUploadPhotoBreak()
{
	bool bRet = m_objUploadPhotoSta.m_bUploadBreak;
	return bRet;
}

void CPhoto::_ConvertLonLatFromLongToStr( double v_dLon, double v_dLat, char* v_szLonLat, size_t v_sizStr )
{
	if( v_sizStr < 8 ) return;
	
	v_szLonLat[ 0 ] = BYTE( v_dLon );
	double dFen = (v_dLon - BYTE(v_szLonLat[0]) ) * 60;
	v_szLonLat[ 1 ] = BYTE( dFen );
	WORD wFenxiao = WORD( (dFen - BYTE(v_szLonLat[1]) ) * 10000 );
	v_szLonLat[ 2 ] = wFenxiao / 100;
	v_szLonLat[ 3 ] = wFenxiao % 100;
	
	v_szLonLat[ 4 ] = BYTE( v_dLat );
	dFen = (v_dLat - BYTE(v_szLonLat[4]) ) * 60;
	v_szLonLat[ 5 ] = BYTE( dFen );
	wFenxiao = WORD( (dFen - BYTE(v_szLonLat[5]) ) * 10000 );
	v_szLonLat[ 6 ] = wFenxiao / 100;
	v_szLonLat[ 7 ] = wFenxiao % 100;
}

WORD CPhoto::_EvtTypeToPhotoType( BYTE v_bytEvtType )
{
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
/*
0x01���������������ٱ����Ͳ෭������
0x02�����ACC����
0x03��������
0x04���ճ���>�س� / �س���>�ճ�
*/
	switch( v_bytEvtType )
	{
	case 0x01: // ����
		return PHOTOEVT_FOOTALERM;

	case 0x02: //hxd 080816
		return PHOTOEVT_ACCON;

	case 0x03://hxd 080816 
		return PHOTOEVT_OPENDOOR;

	case 0x04: // �ճ�->�س�
		return PHOTOEVT_JIJIADOWN;

	case 0x06://hxd 080816
		return PHOTOEVT_TIMER;

	case 0x07: //��������
		return PHOTOEVT_AREA;
		
	case 0x08: //����
		return PHOTOEVT_OVERSPEED;
		
	case 0x09: //����
		return PHOTOEVT_BELOWSPEED;
		
	case 0x0: // ����ץ��
		return PHOTOEVT_CENT;
		
	case 0xff: // ȫ������
		return 0xffff;

	default:
		return 0;
	}
#endif

#if USE_PROTOCOL == 30
/*
0x00����������ץ���¼�
0x11�����ٱ���
0x12���෭����
0x13����ײ����
0x21����ͨ�¹�
0x22��ҽ������
0x23����������
0x31���س���
0x41����������
0x51����ʱ����
0x61��ACC�������
*/
	switch( v_bytEvtType )
	{
	case 0x11: // �پ�
		return PHOTOEVT_FOOTALERM;

	case 0x12: // �෭����
		return PHOTOEVT_OVERTURNALERM;

	case 0x13: // ��ײ����
		return PHOTOEVT_BUMPALERM;

	case 0x21: // ��ͨ����
		return PHOTOEVT_TRAFFICHELP;

	case 0x22: // ҽ������
		return PHOTOEVT_MEDICALHELP;

	case 0x23: // ��������
		return PHOTOEVT_BOTHERHELP;

	case 0x41: // ��������
		return PHOTOEVT_AREA;

	case 0x31: // ����������
		return PHOTOEVT_OPENDOOR;

	case 0x51: // ��ʱ����
		return PHOTOEVT_TIMER;

	case 0x61: // ACC���
		return PHOTOEVT_ACCON;

	case 0: // ����ץ��
		return PHOTOEVT_CENT;

	case 0xff: // ȫ������
		return 0xffff;

	default:
		return 0;
	}
#endif
}

BYTE CPhoto::_PhotoTypeToEvtType( WORD v_wPhotoType )
{
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
	if( v_wPhotoType & PHOTOEVT_FOOTALERM )
		return 0x01;

	else if( v_wPhotoType & PHOTOEVT_ACCON )
		return 0x02;

	else if( v_wPhotoType & PHOTOEVT_OPENDOOR )
		return 0x03;

	else if( v_wPhotoType & PHOTOEVT_JIJIADOWN )
		return 0x04;

	else if( v_wPhotoType & PHOTOEVT_TIMER )
		return 0x06;
	
	else if( v_wPhotoType & PHOTOEVT_AREA )
		return 0x07;
		
	else if( v_wPhotoType & PHOTOEVT_OVERSPEED )
		return 0x08;
		
	else if( v_wPhotoType & PHOTOEVT_BELOWSPEED )
		return 0x09;
		
	else if( v_wPhotoType & PHOTOEVT_CENT )
		return 0;
		
	else
		return 0;
#endif

#if USE_PROTOCOL == 30
	if( v_wPhotoType & PHOTOEVT_FOOTALERM )
		return 0x11;

	else if( v_wPhotoType & PHOTOEVT_OVERTURNALERM )
		return 0x12;

	else if( v_wPhotoType & PHOTOEVT_BUMPALERM )
		return 0x13;

	else if( v_wPhotoType & PHOTOEVT_TRAFFICHELP )
		return 0x21;

	else if( v_wPhotoType & PHOTOEVT_MEDICALHELP )
		return 0x22;
	
	else if( v_wPhotoType & PHOTOEVT_BOTHERHELP )
		return 0x23;
	
	else if( v_wPhotoType & PHOTOEVT_AREA )
		return 0x41;

	else if( v_wPhotoType & PHOTOEVT_OPENDOOR )
		return 0x31;
	
	else if( v_wPhotoType & PHOTOEVT_ACCON )
		return 0x61;
	
	else if( v_wPhotoType & PHOTOEVT_TIMER )
		return 0x51;
	
	else if( v_wPhotoType & PHOTOEVT_CENT )
		return 0;
	
	else
		return 0;
#endif
}

/// �����ϴ�����,�õ���Ӧ����Ƭ����
WORD CPhoto::_UploadTypeToPhotoType( WORD v_wUploadType )
{
	return v_wUploadType; // ע��: ��Ϊ��Ƭ���ͺ��ϴ����͵Ķ�����ȫһ��,���Բ�������!
}

