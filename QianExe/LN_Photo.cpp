#include "yx_QianStdAfx.h"

#if USE_LIAONING_SANQI == 1

#undef MSG_HEAD
#define MSG_HEAD	("QianExe-LN_Photo ")


void *G_ThreadSendPhoto(void *arg)
{
	g_objLNPhoto.P_ThreadSendPhoto();
}


void G_TmLNPhotoTimer(void *arg, int len)
{
	g_objLNPhoto.P_TmPhotoTimer();
}
//////////////////////////////////////////////////////////////////////////

CLNPhoto::CLNPhoto()
{
	memset(m_szPhotoPath, 0, sizeof(m_szPhotoPath));
	memset(m_bytBlockSta, 0, sizeof(m_bytBlockSta));
	memset(m_aryUpPhotoInfo, 0, sizeof(m_aryUpPhotoInfo));
	m_bytAllPhotoTotal = 0;	// ����Ҫ�ϴ���Ƭ������
	m_bytAllPhotoCt = 0; // �������ϴ���Ƭ�ļ���
	sprintf(m_szPhotoPath, "%s", PHOTO_SAVE_PATH);
	m_bThreadExit = false;
	m_bytSta = Idlesse;
	m_objDataMngShotDone.InitCfg( 20000, 300000 );
	m_objRecvMsg.InitCfg( 10000, 300000 );
}

CLNPhoto::~CLNPhoto()
{

}

int CLNPhoto::Init()
{
	pthread_mutex_init(&m_mutex_photo, NULL);
	P_TmPhotoTimer();
	// �����߳�
	if( pthread_create(&m_pThreadUp, NULL, G_ThreadSendPhoto, NULL) != 0 )
		return ERR_THREAD;

	PRTMSG(MSG_NOR, "Create SendPhoto thread succ!\n");
	return 0;
}

int CLNPhoto::Release()
{
	m_bThreadExit = true;

	return 0;
}


void CLNPhoto::P_TmPhotoTimer()
{
	tag2QLNPhotoCfg objLNPhotoCfg;

	objLNPhotoCfg.Init();
	GetSecCfg( &objLNPhotoCfg, sizeof(objLNPhotoCfg), offsetof(tagSecondCfg, m_uni2QLNPhotoCfg.m_obj2QLNPhotoCfg), sizeof(objLNPhotoCfg) );

	if( (objLNPhotoCfg.m_wPhotoEvtSymb & PHOTOEVT_TIMER)  && true == objLNPhotoCfg.m_objTimerPhotoPar.m_bEnable)
	{
		// ��ȡϵͳ��ǰʱ��
		char szCurTime[6] = {0};
		time_t lCurrentTime;
		struct tm *pCurrentTime;    
		time(&lCurrentTime);
		pCurrentTime = localtime(&lCurrentTime); 

		// UTCʱ��ת�ɱ���ʱ��
		struct tm pTemp;
		memcpy((void*)&pTemp, (void*)pCurrentTime, sizeof(pTemp));
		G_GetLocalTime(&pTemp);
		pCurrentTime = &pTemp;

		szCurTime[0] = (char)(pCurrentTime->tm_year-100);
		szCurTime[1] = (char)(pCurrentTime->tm_mon+1);
		szCurTime[2] = (char)pCurrentTime->tm_mday;
		szCurTime[3] = (char)pCurrentTime->tm_hour;
		szCurTime[4] = (char)pCurrentTime->tm_min;
		szCurTime[5] = (char)pCurrentTime->tm_sec;
		if( strncmp(szCurTime, (char *)objLNPhotoCfg.m_objTimerPhotoPar.m_bytCTakeBeginTime, 6) >= 0
			&& strncmp(szCurTime, (char *)objLNPhotoCfg.m_objTimerPhotoPar.m_bytCTakeEndTime, 6) <= 0)
		{

			AddPhoto( PHOTOEVT_TIMER ); 
			UINT uiInterval = (UINT)objLNPhotoCfg.m_objTimerPhotoPar.m_usInterval * 60;   
			if( uiInterval < 60000 )
				uiInterval = 60000;	 // ��С����ĳ�1���ӣ���ֹ��������

			_SetTimer(&g_objTimerMng, TIMERPHOTO_TIMER, uiInterval, G_TmLNPhotoTimer);
		}
	}
}

void CLNPhoto::AddPhoto( WORD v_wPhotoType )
{
	// ��ȡ����
	tag2QLNPhotoCfg objLNPhotoCfg;
	objLNPhotoCfg.Init();
	GetSecCfg( &objLNPhotoCfg, sizeof(objLNPhotoCfg), offsetof(tagSecondCfg, m_uni2QLNPhotoCfg.m_obj2QLNPhotoCfg), sizeof(objLNPhotoCfg) );

	if( !(objLNPhotoCfg.m_wPhotoEvtSymb & v_wPhotoType) )
		return;

	tag2QLNPhotoPar objLNPhotoPar;
	if( v_wPhotoType & PHOTOEVT_FOOTALERM && true == objLNPhotoCfg.m_objAlermPhotoPar.m_bEnable)
	{
		objLNPhotoPar = objLNPhotoCfg.m_objAlermPhotoPar;
	} else if( v_wPhotoType & PHOTOEVT_OVERTURNALERM && true == objLNPhotoCfg.m_objOverTurnPhotoPar.m_bEnable)
	{
		objLNPhotoPar = objLNPhotoCfg.m_objOverTurnPhotoPar;
	} else if( v_wPhotoType & PHOTOEVT_OPENDOOR  && true == objLNPhotoCfg.m_objDoorPhotoPar.m_bEnable)
	{
		objLNPhotoPar = objLNPhotoCfg.m_objDoorPhotoPar;
	} else if( v_wPhotoType & PHOTOEVT_JIJIADOWN && true == objLNPhotoCfg.m_objJijiaPhotoPar.m_bEnable )
	{
		objLNPhotoPar = objLNPhotoCfg.m_objJijiaPhotoPar;
	} else if( v_wPhotoType & PHOTOEVT_TIMER && true == objLNPhotoCfg.m_objTimerPhotoPar.m_bEnable )
	{
		objLNPhotoPar = objLNPhotoCfg.m_objTimerPhotoPar;
	} else if( v_wPhotoType & PHOTOEVT_ACCON && true == objLNPhotoCfg.m_objAccPhotoPar.m_bEnable )
	{
		objLNPhotoPar = objLNPhotoCfg.m_objAccPhotoPar;
	} else if( v_wPhotoType & PHOTOEVT_TIRE && true == objLNPhotoCfg.m_objTirePhotoPar.m_bEnable )
	{
		objLNPhotoPar = objLNPhotoCfg.m_objTirePhotoPar;
	} else return;

	BYTE bytPhotoTime = objLNPhotoPar.m_bytTime;
	BYTE bytPhotoSizeType = objLNPhotoPar.m_bytSizeType;
	BYTE bytPhotoQualityType = objLNPhotoPar.m_bytQualityType;
	BYTE bytBright = objLNPhotoPar.m_bytBright;
	BYTE bytContrast = objLNPhotoPar.m_bytContrast;
	BYTE bytHue = objLNPhotoPar.m_bytHue;
	BYTE bytBlueSaturation = objLNPhotoPar.m_bytBlueSaturation;
	BYTE bytRedSaturation = objLNPhotoPar.m_bytRedSaturation;
	BYTE bytChannel = objLNPhotoPar.m_bytChannel;
	BYTE bytInterval = 0x03;
	if( 0 == bytPhotoTime )	bytPhotoTime = 1;
	else if( bytPhotoTime > PHOTOQUEE_MAXCOUNT ) bytPhotoTime = PHOTOQUEE_MAXCOUNT;

	// �������յ�����
	if( m_objPhotoSta.m_bytPhotoTotal < PHOTOQUEE_MAXCOUNT )
	{
		// ���������������յ�����
		bytPhotoTime = min(bytPhotoTime, PHOTOQUEE_MAXCOUNT - m_objPhotoSta.m_bytPhotoTotal);

		// ����������
		m_objPhotoSta.m_bytPhotoTotal += bytPhotoTime;
	} else
	{
		return;
	}

	SendPhotoRequest(bytPhotoSizeType, bytPhotoQualityType, bytBright, bytContrast,bytHue,
					 bytBlueSaturation, bytRedSaturation, bytPhotoTime, bytChannel, bytInterval,v_wPhotoType);
}

void CLNPhoto::DealOnePhotoEnd()
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
	_UploadPhotos();
// ��ȫ����Ƭ�����꣬�����������Ϣ
	if( m_objPhotoSta.m_bytPhotoCt == m_objPhotoSta.m_bytPhotoTotal )
		memset((void*)&m_objPhotoSta, 0, sizeof(m_objPhotoSta));

}


void CLNPhoto::_UploadPhotos()
{
	// ���մ���
	if( m_objPhotoSta.m_bytPhotoCt > sizeof(m_objPhotoSta.m_aryPhotoInfo) / sizeof(m_objPhotoSta.m_aryPhotoInfo[0]) )
		m_objPhotoSta.m_bytPhotoCt = sizeof(m_objPhotoSta.m_aryPhotoInfo) / sizeof(m_objPhotoSta.m_aryPhotoInfo[0]);

	// ��ȡ����
	tag2QLNPhotoCfg objLNPhotoCfg;
	objLNPhotoCfg.Init();
	GetSecCfg( &objLNPhotoCfg, sizeof(objLNPhotoCfg), offsetof(tagSecondCfg, m_uni2QLNPhotoCfg.m_obj2QLNPhotoCfg), sizeof(objLNPhotoCfg) );
	WORD wUploadPhotoType = _UploadTypeToPhotoType( objLNPhotoCfg.m_wPhotoUploadSymb ); // ������Ҫ�ϴ�����Ƭ����


	tagPhotoInfo aryPhoto[PHOTOQUEE_MAXCOUNT];
	BYTE bytPhotoCt = 0;
	bool bCurPhotoSnd = false;
	for( BYTE byt = 0; byt < m_objPhotoSta.m_bytPhotoCt; byt ++ )
	{
		if( m_objPhotoSta.m_aryPhotoInfo[ byt ].m_bUploaded ) // ���Ѿ�������ϴ�
			continue;

		m_objPhotoSta.m_aryPhotoInfo[ byt ].m_bUploaded = true;	// ��Ǳ�����Ƭ�Ѿ�������ϴ�

		if(wUploadPhotoType & m_objPhotoSta.m_aryPhotoInfo[byt].m_wPhotoType) // ������Ҫ�����ϴ�����Ƭ���� (�������ӵĴ���Ƭ����))
		{
			bCurPhotoSnd = true;
		} else
		{
			bCurPhotoSnd = false;
		}

		if( bCurPhotoSnd && bytPhotoCt < PHOTOQUEE_MAXCOUNT )
		{
			aryPhoto[ bytPhotoCt ] = m_objPhotoSta.m_aryPhotoInfo[ byt ];
			++ bytPhotoCt;
		}
	}
	// �ϴ���Ƭ
	if( bytPhotoCt > 0 )
	{
		P_AddUploadPhoto( aryPhoto, bytPhotoCt );
	}
}

void CLNPhoto::P_AddUploadPhoto( tagPhotoInfo* v_aryPhoto, BYTE v_bytPhotoTotal )
{
	pthread_mutex_lock(&m_mutex_photo);
	if( m_bytAllPhotoCt < m_bytAllPhotoTotal ) // ���ϴ��ϴ���δ����
	{

		// �����ʱ��δ������ɵ���Ƭ����
		tagPhotoInfo aryTempIndex[ UPLOADPHOTO_MAXCOUNT ];
		BYTE bytOldTotal = m_bytAllPhotoTotal - m_bytAllPhotoCt; // ��δ���͵�����
		memcpy( aryTempIndex, m_aryUpPhotoInfo + m_bytAllPhotoCt, bytOldTotal * sizeof(aryTempIndex[0]) );

		// δ���͵ľ���Ƭ������ǰ
		if( bytOldTotal > UPLOADPHOTO_MAXCOUNT ) bytOldTotal = UPLOADPHOTO_MAXCOUNT; // ���մ���
		memcpy( m_aryUpPhotoInfo, aryTempIndex, bytOldTotal * sizeof(m_aryUpPhotoInfo[0]) );

		// ����Ƭ�������
		v_bytPhotoTotal = bytOldTotal + v_bytPhotoTotal <= UPLOADPHOTO_MAXCOUNT
						  ? v_bytPhotoTotal : UPLOADPHOTO_MAXCOUNT - bytOldTotal;
		memcpy( m_aryUpPhotoInfo + bytOldTotal, v_aryPhoto, v_bytPhotoTotal * sizeof(m_aryUpPhotoInfo[0]) );

		// �����ܹ�Ҫ������Ƭ������
		m_bytAllPhotoTotal = bytOldTotal + v_bytPhotoTotal;
	}

	else
	{
		// �����״̬,��ʼ����״̬
		//m_objUploadPhotoSta.Init();
		//m_objUploadPhotoSta.m_wPhotoIndex = m_objUploadPhotoSta.m_aryPhotoInfo[0].m_wPhotoIndex;

		// ��������Ƭ����
		if( v_bytPhotoTotal > UPLOADPHOTO_MAXCOUNT ) v_bytPhotoTotal = UPLOADPHOTO_MAXCOUNT; // �������
		memcpy( m_aryUpPhotoInfo, v_aryPhoto, v_bytPhotoTotal * sizeof(m_aryUpPhotoInfo[0]) );

		// �����ܹ�Ҫ������Ƭ������
		m_bytAllPhotoTotal = v_bytPhotoTotal;

		// �ѷ�����Ƭ������0
		m_bytAllPhotoCt = 0;
	}
	pthread_mutex_unlock(&m_mutex_photo);
}


int CLNPhoto::Deal02_0002(char *v_szBuf, DWORD v_dwLen, DWORD v_dwCenterSEQ)
{
	BYTE bytInterval = 0x03;
	BYTE bytPhotoTotal = 0;
	BYTE bytSizeType = 1;
	BYTE bytQualityType = 2;
	BYTE bytBright = 8;
	BYTE bytContrast = 8;
	BYTE bytHue = 8;
	BYTE bytBlueSaturation = 8;
	BYTE bytRedSaturation = 8;
	char m_ExtID[20] = {0};
	char    szReplyBuf[1024] = {0};
	DWORD   dwReplyLen = 0;
	int     iret = 0;
	int     i = 0;
	char    szTempBuf[1024] = {0};
	ushort  dwTempLen = 0;
	char *akvp;
	byte channel;
	AKV_t AKVTemp, *akv;
	akv = &AKVTemp;
	//
	// 	char	szkey[20] = {0};
	//
	// �������
	if( v_dwLen < v_szBuf[0] + 8 )
	{
		PRTMSG(MSG_ERR, "Deal02_0002 err 1, v_dwLen=%d, i=%d\n", v_dwLen, i);
		iret = ERR_PAR;
		goto _DEAL020002_END;
	}
	//
	// ExtID
	memcpy(m_ExtID, v_szBuf + 1, v_szBuf[0]);
	i += v_szBuf[0] + 1;
	if(!AKV_Jugre(&v_szBuf[i], akv, v_dwLen - i))
	{
		PRTMSG(MSG_DBG, "Deal02_0002 AKV 1 err!!!!!\n");
		iret = ERR_PAR;
		goto _DEAL020002_END;
	}
	if( akv->KeyLen != 2 && akv->Key[0] != 'c' && akv->Key[1] != 'h' )
	{
		PRTMSG(MSG_DBG, "Deal02_0002 AKV 2 err!!!!!\n");
		iret = ERR_PAR;
		goto _DEAL020002_END;
	}
	if( akv->Vlen != 1 && akv->Value[0] > 0x0f )
	{
		PRTMSG(MSG_DBG, "Deal02_0002 AKV 3 err!!!!!\n");
		iret = ERR_PAR;
		goto _DEAL020002_END;
	}
	channel = akv->Value[0];
	bytSizeType = 3;
	channel &=  0x0f;
	if( 0 == channel )
		channel = 1; // ͨ���ż��
	// ͼƬ����
	// 0x01���㶨�����ȼ����ߣ�
	// 0x02���㶨�����ȼ����У�
	// 0x03���㶨�����ȼ����ͣ�
	bytQualityType = 1;         
	bytPhotoTotal = 1;          
	bytInterval = 0x03;
	
	//���ȣ�1�����Աȶȣ�1����ɫ����1������ɫ���Ͷȣ�1������ɫ���Ͷȣ�1��
	if( bytBright > 0x0f ) bytBright = 0x0f;
	else if( bytBright < 1 ) bytBright = 1;
	if( bytContrast > 0x0f ) bytContrast = 0x0f;
	else if( bytContrast < 1 ) bytContrast = 1;
	if( bytHue > 0x0f )	bytHue = 0x0f;
	else if( bytHue < 1 ) bytHue = 1;
	if( bytBlueSaturation > 0x0f ) bytBlueSaturation = 0x0f;
	else if( bytBlueSaturation < 1 ) bytBlueSaturation = 1;
	if( bytRedSaturation > 0x0f ) bytRedSaturation = 0x0f;
	else if( bytRedSaturation < 1 )	bytRedSaturation = 1;

	PRTMSG(MSG_NOR, "Rcv Cent Photo Req!\n");

	// �ϴε����չ��̡����ϴ����̿��ܻ����
	// ��������µ���������������״̬���ϴ�״̬���ش���ʱȫ�����㣬���Ȿ���ٳ���
//	_KillTimer(&g_objTimerMng, UPLOAD_PHOTOWIN_TIMER );
	memset((void*)&m_objPhotoSta, 0, sizeof(m_objPhotoSta));
//	memset((void*)&m_objUploadPhotoSta, 0, sizeof(m_objUploadPhotoSta));

	if( m_objPhotoSta.m_bytPhotoCt < m_objPhotoSta.m_bytPhotoTotal )
	{ /// ���ϴε���������û�н���

		// ����"��̨��æ"Ӧ��
		iret = ERR_PAR;
		goto _DEAL020002_END;
	} else
	{ /// ����������״̬

		SendPhotoRequest( bytSizeType, bytQualityType, bytBright, bytContrast, bytHue, bytBlueSaturation,
						  bytRedSaturation, bytPhotoTotal, channel, bytInterval, PHOTOEVT_CENT );
	}

	_DEAL020002_END:
	szTempBuf[dwTempLen++] = v_szBuf[0];
	memcpy(szTempBuf + dwTempLen, v_szBuf + 1, v_szBuf[0]);
	dwTempLen += v_szBuf[0];
	if( iret )
	{
		szTempBuf[dwTempLen++] = 0x00;
	} else
	{
		szTempBuf[dwTempLen++] = 0x01;
	}
	PutAKV_char(szTempBuf + dwTempLen, 0x00, "ch", &channel, 1, &dwTempLen);
	// ������Ӧ��
	if( !g_objApplication.MakeAppFrame(0x02, 0x0002, szTempBuf, dwTempLen, szReplyBuf, sizeof(szReplyBuf), dwReplyLen) )
	{
		g_objApplication.SendAppFrame(MODE_A, 0x80, false, v_dwCenterSEQ, szReplyBuf, dwReplyLen);
	}

	return iret;
}

int CLNPhoto::Deal02_0003(char *v_szBuf, DWORD v_dwLen, DWORD v_dwCenterSEQ)
{
	char *DEBUGMSG = "Deal02_0003";
	char    szReplyBuf[1024] = {0};
	DWORD   dwReplyLen = 0;
	int     iret = 0;
	int     i = 0, j, limit;
	char    szTempBuf[1024] = {0};
	ushort  dwTempLen = 0;
	char *akvp;
	byte channel, camnum = 0, Event = 0;
	AKV_t AKVTemp, *akv;
	akv = &AKVTemp;
	tag2QLNPhotoPar objLNPhotoPar;
	//
	// 	char	szkey[20] = {0};
	//
	// �������
	if( v_dwLen < v_szBuf[0] + 8 )
	{
		PRTMSG(MSG_ERR, "Deal02_0003 err 1, v_dwLen=%d, i=%d\n", v_dwLen, i);
		iret = ERR_PAR;
		goto _DEAL020003_END;
	}
	//
	// ExtID
	memcpy(objLNPhotoPar.m_ExtID, v_szBuf + 1, v_szBuf[0]);
	i += v_szBuf[0] + 1;

	DWORD fvlen;
	fvlen = v_dwLen - i;
	tag2QLNPhotoCfg objLNPhotoCfg;
	GetSecCfg((void*)&objLNPhotoCfg, sizeof(objLNPhotoCfg), offsetof(tagSecondCfg, m_uni2QLNPhotoCfg.m_obj2QLNPhotoCfg), sizeof(objLNPhotoCfg));
	// 	// ����AKV����
	for(limit = 0; limit < 7; limit++)
	{
		if( i >= v_dwLen )
		{

			break;
		}
		if(!AKV_Jugre(&v_szBuf[i], akv, fvlen))
		{
			PRTMSG(MSG_DBG, "Deal02_0003 AKV 1 err!!!!!\n");
			iret = ERR_PAR;
			goto _DEAL020003_END;
		}
		i += akv->AKVLen;
		switch( Key_Jugres(akv->Key) )
		{
		case 29: //te
			{
				if( !Get_char(akv, &Event, DEBUGMSG ))
				{
					PRTMSG(MSG_DBG, "Photo event type err!\n");
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				if( Event > 0x07 )
				{
					PRTMSG(MSG_DBG, "Photo event type %d err!\n", Event);
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				PRTMSG(MSG_DBG, "Photo event type %d \n", Event);
			}
			break;
		case 30: //rt
			{
				if( !Get_char(akv, &objLNPhotoPar.m_bytSizeType, DEBUGMSG ))
				{
					PRTMSG(MSG_DBG, "Photo size type err!\n");
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				if( objLNPhotoPar.m_bytSizeType > 0x03 )
				{
					PRTMSG(MSG_DBG, "Photo size type %d err!\n", objLNPhotoPar.m_bytSizeType);
					iret    =    ERR_PAR;
					goto _DEAL020003_END;
				}
				PRTMSG(MSG_DBG, "Photo size type %d \n", objLNPhotoPar.m_bytSizeType);
			}
			break;
		case 31: //ch
			{
				if( !Get_char(akv, &objLNPhotoPar.m_bytChannel, DEBUGMSG ))
				{
					PRTMSG(MSG_DBG, "Cam chanel err!\n");
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				if( objLNPhotoPar.m_bytChannel > 0x0f )
				{
					PRTMSG(MSG_DBG, "Cam chanel %02x err!\n", objLNPhotoPar.m_bytChannel);
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				PRTMSG(MSG_DBG, "Cam chanel %02x \n", objLNPhotoPar.m_bytChannel);
			}
			break;
		case 32: //qa
			{
				if( !Get_char(akv, &objLNPhotoPar.m_bytQualityType, DEBUGMSG ))
				{
					PRTMSG(MSG_DBG, "Photo quality err!\n");
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				if( objLNPhotoPar.m_bytQualityType > 0x03 )
				{
					PRTMSG(MSG_DBG, "Photo quality %d err!\n", objLNPhotoPar.m_bytQualityType);
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				PRTMSG(MSG_DBG, "Photo quality %d \n", objLNPhotoPar.m_bytQualityType);
			}
			break;
		case 33: //cm
			{
				if( !Get_char(akv, &objLNPhotoPar.m_CmSta, DEBUGMSG ))
				{
					PRTMSG(MSG_DBG, "Photo cm err!\n");
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				PRTMSG(MSG_DBG, "Photo cm %02x \n", objLNPhotoPar.m_CmSta);
			}
			break;
		case 34: //ibe
			{
				AKV_ibe_t akv_ibe;
				if(!AKV_ibe(akv, &akv_ibe, DEBUGMSG))
				{
					PRTMSG(MSG_DBG, "AKV ibe err!\n");
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				objLNPhotoPar.m_usInterval = akv_ibe.m_usInterval;
				memcpy(objLNPhotoPar.m_bytCTakeBeginTime, akv_ibe.m_bytCTakeBeginTime, 6);
				memcpy(objLNPhotoPar.m_bytCTakeEndTime, akv_ibe.m_bytCTakeEndTime, 6);
				PRTMSG(MSG_DBG, "Start at %d:%d:%d:%d:%d:%d\n", objLNPhotoPar.m_bytCTakeBeginTime[0], objLNPhotoPar.m_bytCTakeBeginTime[1], objLNPhotoPar.m_bytCTakeBeginTime[2], objLNPhotoPar.m_bytCTakeBeginTime[3], objLNPhotoPar.m_bytCTakeBeginTime[4], objLNPhotoPar.m_bytCTakeBeginTime[5]);
				PRTMSG(MSG_DBG, "End at %d:%d:%d:%d:%d:%d\n", objLNPhotoPar.m_bytCTakeEndTime[0], objLNPhotoPar.m_bytCTakeEndTime[1], objLNPhotoPar.m_bytCTakeEndTime[2], objLNPhotoPar.m_bytCTakeEndTime[3], objLNPhotoPar.m_bytCTakeEndTime[4], objLNPhotoPar.m_bytCTakeEndTime[5]);
				PRTMSG(MSG_DBG, "Take a photo per %d min\n", objLNPhotoPar.m_usInterval);
			}
			break;
		case 35: //bo
			{
				AKV_bo_t akv_bo;
				if(!AKV_bo(akv, &akv_bo, DEBUGMSG))
				{
					PRTMSG(MSG_DBG, "AKV bo err!\n");
					iret = ERR_PAR;
					goto _DEAL020003_END;
				}
				objLNPhotoPar.m_bytTTakeBaseTime = akv_bo.m_bytTTakeBaseTime;
				objLNPhotoPar.m_bytTTakeOfTime = akv_bo.m_bytTTakeOfTime;
				memcpy(objLNPhotoPar.m_bytTTakeBeginTime, akv_bo.m_bytTTakeBeginTime, 6);
				memcpy(objLNPhotoPar.m_bytTTakeEndTime, akv_bo.m_bytTTakeEndTime, 6);
				PRTMSG(MSG_DBG, "Start at %d:%d:%d:%d:%d:%d\n", objLNPhotoPar.m_bytTTakeBeginTime[0], objLNPhotoPar.m_bytTTakeBeginTime[1], objLNPhotoPar.m_bytTTakeBeginTime[2], objLNPhotoPar.m_bytTTakeBeginTime[3], objLNPhotoPar.m_bytTTakeBeginTime[4], objLNPhotoPar.m_bytTTakeBeginTime[5]);
				PRTMSG(MSG_DBG, "End at %d:%d:%d:%d:%d:%d\n", objLNPhotoPar.m_bytTTakeEndTime[0], objLNPhotoPar.m_bytTTakeEndTime[1], objLNPhotoPar.m_bytTTakeEndTime[2], objLNPhotoPar.m_bytTTakeEndTime[3], objLNPhotoPar.m_bytTTakeEndTime[4], objLNPhotoPar.m_bytTTakeEndTime[5]);
				PRTMSG(MSG_DBG, "Base time %d min\n", objLNPhotoPar.m_bytTTakeBaseTime);
				PRTMSG(MSG_DBG, "Of time %d min\n", objLNPhotoPar.m_bytTTakeOfTime);
			}
			break;
		default:
			{
				PRTMSG(MSG_DBG, "Deal02_0003 AKV err!\n");
				iret = ERR_PAR;
				goto _DEAL020003_END;
			}
			break;
		}
	}
	objLNPhotoPar.m_bEnable = objLNPhotoPar.m_CmSta & 0x80 ? true : false;
	switch( Event )
	{
	case 0x01:
		{
			if(objLNPhotoPar.m_CmSta & 0x40)
			{
				objLNPhotoCfg.m_wPhotoUploadSymb |= PHOTOUPLOAD_FOOTALERM;
			} else
			{
				objLNPhotoCfg.m_wPhotoUploadSymb &= ~PHOTOUPLOAD_FOOTALERM;
			}
			memcpy(&objLNPhotoCfg.m_objAlermPhotoPar, &objLNPhotoPar, sizeof(objLNPhotoPar)); 
		}
		break;
	case 0x02:
		{
			if(objLNPhotoPar.m_CmSta & 0x40)
			{
				objLNPhotoCfg.m_wPhotoUploadSymb |= PHOTOUPLOAD_ACCON;
			} else
			{
				objLNPhotoCfg.m_wPhotoUploadSymb &= ~PHOTOUPLOAD_ACCON;
			}
			memcpy(&objLNPhotoCfg.m_objAccPhotoPar, &objLNPhotoPar, sizeof(objLNPhotoPar));
		}
		break;
	case 0x03:
		{
			if(objLNPhotoPar.m_CmSta & 0x40)
			{
				objLNPhotoCfg.m_wPhotoUploadSymb |= PHOTOUPLOAD_OPENDOOR;
			} else
			{
				objLNPhotoCfg.m_wPhotoUploadSymb &= ~PHOTOUPLOAD_OPENDOOR;
			}
			memcpy(&objLNPhotoCfg.m_objDoorPhotoPar, &objLNPhotoPar, sizeof(objLNPhotoPar));
		}
		break;

	case 0x04:
		{
			if(objLNPhotoPar.m_CmSta & 0x40)
			{
				objLNPhotoCfg.m_wPhotoUploadSymb |= PHOTOUPLOAD_JIJIADOWN;
			} else
			{
				objLNPhotoCfg.m_wPhotoUploadSymb &= ~PHOTOUPLOAD_JIJIADOWN;
			}
			memcpy(&objLNPhotoCfg.m_objJijiaPhotoPar, &objLNPhotoPar, sizeof(objLNPhotoPar));
		}
		break;
	case 0x05:
		{
			if(objLNPhotoPar.m_CmSta & 0x40)
			{
				objLNPhotoCfg.m_wPhotoUploadSymb |= PHOTOUPLOAD_OVERTURNALERM;
			} else
			{
				objLNPhotoCfg.m_wPhotoUploadSymb &= ~PHOTOUPLOAD_OVERTURNALERM;
			}
			memcpy(&objLNPhotoCfg.m_objOverTurnPhotoPar, &objLNPhotoPar, sizeof(objLNPhotoPar));
		}
		break;
	case 0x06:
		{
			if(objLNPhotoPar.m_CmSta & 0x40)
			{
				objLNPhotoCfg.m_wPhotoUploadSymb |= PHOTOUPLOAD_TIMER;
			} else
			{
				objLNPhotoCfg.m_wPhotoUploadSymb &= ~PHOTOUPLOAD_TIMER;
			}
			memcpy(&objLNPhotoCfg.m_objTimerPhotoPar, &objLNPhotoPar, sizeof(objLNPhotoPar));
		}
		break;
	case 0x07:
		{
			if(objLNPhotoPar.m_CmSta & 0x40)
			{
				objLNPhotoCfg.m_wPhotoUploadSymb |= PHOTOUPLOAD_TIRE;
			} else
			{
				objLNPhotoCfg.m_wPhotoUploadSymb &= ~PHOTOUPLOAD_TIRE;
			}
			memcpy(&objLNPhotoCfg.m_objTirePhotoPar, &objLNPhotoPar, sizeof(objLNPhotoPar));
		}
		break;
	default:
		break;
	}

	SetSecCfg((void*)&objLNPhotoCfg, offsetof(tagSecondCfg, m_uni2QLNPhotoCfg.m_obj2QLNPhotoCfg), sizeof(objLNPhotoCfg));
	_DEAL020003_END:
	szTempBuf[dwTempLen++] = v_szBuf[0];
	memcpy(szTempBuf + dwTempLen, v_szBuf + 1, v_szBuf[0]);
	dwTempLen += v_szBuf[0];
	if( iret )
	{
		szTempBuf[dwTempLen++] = 0x00;
	} else
	{
		szTempBuf[dwTempLen++] = 0x01;
	}
	PutAKV_char(szTempBuf + dwTempLen, 0x00, "ch", &channel, 1, &dwTempLen);
	// ������Ӧ��
	if( !g_objApplication.MakeAppFrame(0x02, 0x0003, szTempBuf, dwTempLen, szReplyBuf, sizeof(szReplyBuf), dwReplyLen) )
	{
		g_objApplication.SendAppFrame(MODE_A, 0x80, false, v_dwCenterSEQ, szReplyBuf, dwReplyLen);
	}

	return iret;
}

int CLNPhoto::Deal04_0001(char *v_szBuf, DWORD v_dwLen, DWORD v_dwCenterSEQ)
{
	char *DEBUGMSG = "Deal04_0001";
	char    szReplyBuf[2048] = {0};
	DWORD   dwReplyLen = 0;
	int     iret = 0, limit, n;
	int     i = 0;
	char    szTempBuf[2048] = {0};
	ushort  dwTempLen = 0;
	char m_ExtID[20] = {0};
	char *akvp;
	DWORD SegNum = 0;
	byte channel, numphoto = 0, matchnum = 0, j;
	AKV_t AKVTemp, *akv;
	akv = &AKVTemp;
	FILE *fpSrc = NULL;
	long szFileSize;
	char szPhotoDir[255];
	char szFileName[255];
	struct dirent **namelist;
	DWORD dwBeginDate, dwBeginTime, dwEndDate, dwEndTime, dwCurDate;
	DWORD dwPhotoDate, wPhotoTime;
	BYTE bytFindCount = 0;
	BYTE bytYear, bytMon, bytDay;
	ushort usVlenp, usakvfnum = 0;
	PhotoTimeSeg objTimeSeg[MAX_TIMESEG_COUNT];
	//
	// 	char	szkey[20] = {0};
	//
	// �������
	if( v_dwLen < v_szBuf[0] + 8 )
	{
		PRTMSG(MSG_ERR, "Deal04_0001 err 1, v_dwLen=%d, i=%d\n", v_dwLen, i);
		iret = ERR_PAR;
		goto _DEAL040001_END;
	}
	//
	// ExtID
	memcpy(m_ExtID, v_szBuf + 1, v_szBuf[0]);
	i += v_szBuf[0] + 1;

	for(limit = 0; limit < 3; limit++)
	{
		if( i >= v_dwLen )
		{

			break;
		}
		if(!AKV_Jugre(&v_szBuf[i], akv, v_dwLen - i))
		{
			PRTMSG(MSG_DBG, "Deal04_0001 AKV 1 err!!!!!\n");
			iret = ERR_PAR;
			goto _DEAL040001_END;
		}
		i += akv->AKVLen;
		switch( Key_Jugres(akv->Key) )
		{
		case 31: //ch
			{
				if( !Get_char(akv, &channel, DEBUGMSG ))
				{
					PRTMSG(MSG_DBG, "Cam chanel err!\n");
					iret = ERR_PAR;
					goto _DEAL040001_END;
				}
				if( channel > 0x0f )
				{
					PRTMSG(MSG_DBG, "Cam chanel %02x err!\n", channel);
					iret = ERR_PAR;
					goto _DEAL040001_END;
				}
				PRTMSG(MSG_DBG, "Cam chanel %02x \n", channel);
			}
			break;
		case 39: //max
			{
				if( !Get_char(akv, &numphoto, DEBUGMSG ))
				{
					PRTMSG(MSG_DBG, "Photo quality err!\n");
					iret = ERR_PAR;
					goto _DEAL040001_END;
				}
				if( numphoto > 100 )
				{
					PRTMSG(MSG_DBG, "Photo num %d err!\n", numphoto);
					iret = ERR_PAR;
					goto _DEAL040001_END;
				}
				PRTMSG(MSG_DBG, "Photo num %d \n", numphoto);
			}
			break;
		case 40: //qT
			{
				DWORD limit1, limit2;
				DWORD Cnt1 = 0, Cnt2 = 0;
				DWORD akvlen = akv->Vlen, timelen;
				akvp = akv->Value;
				for(limit1 = 0; limit1 < MAX_TIMESEG_COUNT; limit1++)
				{
					if( Cnt1 >= akvlen )
					{
						break;
					}
					if(!AKV_Jugre(akvp, akv, akvlen))
					{
						PRTMSG(MSG_DBG, "%s AKV_qT 1 err!!!!!\n", DEBUGMSG);
						iret = ERR_PAR;
						goto _DEAL040001_END;
					}
					Cnt1 += akv->AKVLen;
					akvp = akv->Value;
					SegNum = (byte)akv->Key[0] - 0x31;
					if( 2 == akv->KeyLen )
					{
						SegNum = (akv->Key[0] - 0x30) * 10 + akv->Key[0] - 0x30 - 1;
					}
					if( SegNum >= MAX_TIMESEG_COUNT)
					{
						PRTMSG(MSG_DBG, "%s Time seg %d err!!!!!\n", DEBUGMSG, SegNum);
						iret = ERR_PAR;
						goto _DEAL040001_END;
					}
					timelen = akv->Vlen;
					Cnt2 = 0;
					for(limit2 = 0; limit2 < 2; limit2++)
					{
						if( Cnt2 >= timelen )
						{
							break;
						}
						if(!AKV_Jugre(akvp, akv, timelen))
						{
							PRTMSG(MSG_DBG, "%s AKV_qT 2 err!!!!!\n", DEBUGMSG);
							iret = ERR_PAR;
							goto _DEAL040001_END;
						}
						Cnt2 += akv->AKVLen;
						akvp += akv->AKVLen;
						switch( Key_Jugres(akv->Key) )
						{
						case 16: //��ʼ
							{
								int a = 0;                                        
								if( akv->Vlen != 6 )
								{
									PRTMSG(MSG_DBG, "%s Btime b err!!!!!\n", DEBUGMSG);
									iret = ERR_PAR;
									goto _DEAL040001_END;
								}
								while( akv->Vlen-- )
								{
									objTimeSeg[SegNum].bBegin[a] = akv->Value[a];
									a++;
								}
								PRTMSG(MSG_DBG, "Start at %d:%d:%d:%d:%d:%d\n", objTimeSeg[SegNum].bBegin[0], objTimeSeg[SegNum].bBegin[1], objTimeSeg[SegNum].bBegin[2], objTimeSeg[SegNum].bBegin[3], objTimeSeg[SegNum].bBegin[4], objTimeSeg[SegNum].bBegin[5]);

							}
							break;
						case 17: //����
							{
								int a = 0;
								if( akv->Vlen != 6 )
								{
									PRTMSG(MSG_DBG, "%s Btime e err!!!!!\n", DEBUGMSG);
									iret = ERR_PAR;
									goto _DEAL040001_END;
								}
								while( akv->Vlen-- )
								{
									objTimeSeg[SegNum].bEnd[a] = akv->Value[a];
									a++;
								}
								PRTMSG(MSG_DBG, "End at %d:%d:%d:%d:%d:%d\n", objTimeSeg[SegNum].bEnd[0], objTimeSeg[SegNum].bEnd[1], objTimeSeg[SegNum].bEnd[2], objTimeSeg[SegNum].bEnd[3], objTimeSeg[SegNum].bEnd[4], objTimeSeg[SegNum].bEnd[5]);
							}
							break;
						default:
							{
								PRTMSG(MSG_DBG, "qT err!!!!\n");
								iret = ERR_PAR;
								goto _DEAL040001_END;
							}
							break;
						}
					}
				}
			}
			break;
		default:
			{
				PRTMSG(MSG_DBG, "Deal04_0001 AKV err!\n");
				iret = ERR_PAR;
				goto _DEAL040001_END;
			}
			break;
		}
	}

	_DEAL040001_END:
	bytFindCount = 0; // ��ʼ���ҵ�����Ƭ����
	szTempBuf[dwTempLen++] = v_szBuf[0];
	memcpy(szTempBuf + dwTempLen, v_szBuf + 1, v_szBuf[0]);
	dwTempLen += v_szBuf[0];
	if( iret || m_objPhotoSta.m_bytPhotoCt < m_objPhotoSta.m_bytPhotoTotal)
	{
		szTempBuf[dwTempLen++] = 0x00;
		PutAKV_char(szTempBuf + dwTempLen, 0x00, "ch", &channel, 1, &dwTempLen);
		PutAKV_char(szTempBuf + dwTempLen, 0x00, "cnt", &bytFindCount, 1, &dwTempLen);
	} else
	{
		szTempBuf[dwTempLen++] = 0x01;
		PutAKV_char(szTempBuf + dwTempLen, 0x00, "ch", &channel, 1, &dwTempLen);
		PutAKV_char(szTempBuf + dwTempLen, 0x00, "cnt", &bytFindCount, 1, &dwTempLen);
		szTempBuf[dwTempLen++] = 0x11;
		szTempBuf[dwTempLen++] = 0x05;
		szTempBuf[dwTempLen++] = 'p';
		szTempBuf[dwTempLen++] = 'h';
		szTempBuf[dwTempLen++] = 'o';
		szTempBuf[dwTempLen++] = 't';
		szTempBuf[dwTempLen++] = 'o';
		szTempBuf[dwTempLen++] = 0x00;
		szTempBuf[dwTempLen++] = 0x00;
		usVlenp = dwTempLen - 2;

		// ������ֹʱ���,��Ѱ��Ƭ

		for(SegNum = 0; SegNum < MAX_TIMESEG_COUNT; SegNum++)
		{

// ʱ��β�������
			if( objTimeSeg[SegNum].bBegin[1] > 12 || objTimeSeg[SegNum].bBegin[2] > 31 // ��ʼ����
				|| objTimeSeg[SegNum].bBegin[3] > 23 || objTimeSeg[SegNum].bBegin[4] > 59 || objTimeSeg[SegNum].bBegin[5] > 59	// ��ʼʱ����
				|| objTimeSeg[SegNum].bEnd[1] > 12 || objTimeSeg[SegNum].bEnd[2] > 31 // ��ֹ����
				|| objTimeSeg[SegNum].bEnd[3] > 23 || objTimeSeg[SegNum].bEnd[4] > 59 || objTimeSeg[SegNum].bEnd[5] > 59 )	// ��ֹʱ����
			{
				goto _FINDPHOTO_END;
			}

			// ����ʱ������ֲ���
			dwBeginDate = BYTE(objTimeSeg[SegNum].bBegin[0]) * DWORD(10000) + BYTE(objTimeSeg[SegNum].bBegin[1]) * DWORD(100) + BYTE(objTimeSeg[SegNum].bBegin[2]);
			dwBeginTime = BYTE(objTimeSeg[SegNum].bBegin[3]) * DWORD(10000) + BYTE(objTimeSeg[SegNum].bBegin[4]) * DWORD(100) + BYTE(objTimeSeg[SegNum].bBegin[5]);
			dwEndDate = BYTE(objTimeSeg[SegNum].bEnd[0]) * DWORD(10000) + BYTE(objTimeSeg[SegNum].bEnd[1]) * DWORD(100) + BYTE(objTimeSeg[SegNum].bEnd[2]);
			dwEndTime = BYTE(objTimeSeg[SegNum].bEnd[3]) * DWORD(10000) + BYTE(objTimeSeg[SegNum].bEnd[4]) * DWORD(100) + BYTE(objTimeSeg[SegNum].bEnd[5]);

			// ʱ��β����Ϸ��Լ���
			if( dwEndDate < dwBeginDate || (dwEndDate == dwBeginDate && dwEndTime < dwBeginTime) )
			{
				goto _FINDPHOTO_END;
			}

			for( dwCurDate = dwBeginDate; dwCurDate <= dwEndDate; )
			{
				IN_MakePhotoDirName( dwCurDate, szPhotoDir );
//				PRTMSG(MSG_DBG, "Deal04_0001 photo save path:%s\n", szPhotoDir);
				n = scandir(szPhotoDir, &namelist, 0, alphasort);
				if(n < 0)
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
						sprintf(szFileName, "%s%s", szPhotoDir, namelist[n]->d_name);
						// ���ļ�
						if( (fpSrc=fopen(szFileName, "rb"))==NULL )
						{
							PRTMSG(MSG_ERR, "Open %s fail\n", szFileName);
							continue;
						}

						// �����ļ���С
						fseek(fpSrc, 0, SEEK_END);
						szFileSize = ftell(fpSrc);
						fclose(fpSrc);
						if( szFileSize < 5 * 1024 )
						{
							continue;
						}
						IN_PutFileName(szFileName, dwPhotoDate, wPhotoTime); 
//						sprintf(szFileName + 6, "%s", ".jpg"); 
//						sprintf(szFileName, "TEST.jpg");
//						PRTMSG(MSG_DBG, "Deal04_0001 photo name:%s\n", szFileName);
						PutAKV_char(szTempBuf + dwTempLen, 0x00, "f", (byte *)szFileName, 6, &dwTempLen);
						usakvfnum += 11;

						// �����ҵ��㹻��ͼƬ,����
						if( ++ bytFindCount >= numphoto )
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
					} else
					{
						if( bytDay > 28 )
						{
							bytDay = 1;
							bytMon ++;
						}
					}
					break;

				default:
					{
						goto _FINDPHOTO_END;
					}
					break;
				}

				if( bytMon > 12 )
				{
					bytMon = 1;
					bytYear ++;
				}

				dwCurDate = bytYear * 10000 + bytMon * 100 + bytDay;
			}
		}
		_FINDPHOTO_END:
		szTempBuf[usVlenp] = htons(usakvfnum) & 0xff;
		szTempBuf[usVlenp+1] = (htons(usakvfnum) >> 8) & 0xff;  
		szTempBuf[dwTempLen-usakvfnum-10] = bytFindCount; 
		if( 0 == bytFindCount )
		{
			dwTempLen -= 9;
		}

	}
	// ������Ӧ��
	if( !g_objApplication.MakeAppFrame(0x04, 0x0001, szTempBuf, dwTempLen, szReplyBuf, sizeof(szReplyBuf), dwReplyLen) )
	{
		g_objApplication.SendAppFrame(MODE_A, 0x80, false, v_dwCenterSEQ, szReplyBuf, dwReplyLen);
	}

	return iret;



}

int CLNPhoto::Deal04_0002(char *v_szBuf, DWORD v_dwLen, DWORD v_dwCenterSEQ)
{
	char *DEBUGMSG = "Deal04_0002";
	char    szReplyBuf[MAX_UNSIGNED_16] = {0};
	DWORD   dwReplyLen = 0;
	int     iret = 0, limit;
	int     i = 0;
	char    szTempBuf[MAX_UNSIGNED_16] = {0};
	ushort  dwTempLen = 0;
	char m_ExtID[20] = {0};
	byte Ftime[7] = {0};
	char *akvp;
	AKV_t AKVTemp, *akv;
	akv = &AKVTemp;
	//
	// 	char	szkey[20] = {0};
	//
	// �������
	if( v_dwLen < v_szBuf[0] + 8 )
	{
		PRTMSG(MSG_ERR, "Deal04_0002 err 1, v_dwLen=%d, i=%d\n", v_dwLen, i);
		iret = ERR_PAR;
		goto _DEAL040002_END;
	}
	//
	// ExtID
	memcpy(m_ExtID, v_szBuf + 1, v_szBuf[0]);
	i += v_szBuf[0] + 1;
	Ftime[6] = '\0';
	for(limit = 0; limit < 2; limit++)
	{
		if( i >= v_dwLen )
		{

			break;
		}
		if(!AKV_Jugre(&v_szBuf[i], akv, v_dwLen - i))
		{
			PRTMSG(MSG_DBG, "Deal04_0002 AKV 1 err!!!!!\n");
			iret = ERR_PAR;
			goto _DEAL040002_END;
		}
		i += akv->AKVLen;
		switch( Key_Jugres(akv->Key) )
		{
		case 41: //f
			{
				int a = 0;                                        
				if( akv->Vlen != 6 )
				{
					PRTMSG(MSG_DBG, "%s Btime b err!!!!!\n", DEBUGMSG);
					iret = ERR_PAR;
					goto _DEAL040002_END;
				}
				while( akv->Vlen-- )
				{
					Ftime[a] = akv->Value[a];
					a++;
				}       
				PRTMSG(MSG_DBG, "Filename:%d%d%d%d%d%d\n", Ftime[0], Ftime[1], Ftime[2], Ftime[3], Ftime[4], Ftime[5]);
			}
			break;
		case 42://bN
			{

			}
			break;
		default:
			{
				PRTMSG(MSG_DBG, "Deal04_0002 AKV err!\n");
				iret = ERR_PAR;
				goto _DEAL040002_END;
			}
			break;
		}
	}

	_DEAL040002_END:
	szTempBuf[dwTempLen++] = v_szBuf[0];
	memcpy(szTempBuf + dwTempLen, v_szBuf + 1, v_szBuf[0]);
	dwTempLen += v_szBuf[0];
	if( iret )
	{
		szTempBuf[dwTempLen++] = 0x00;
	} else
	{
		szTempBuf[dwTempLen++] = 0x01;
		PutAKV_char(szTempBuf + dwTempLen, 0x00, "f", (byte *)Ftime, 6, &dwTempLen);
	}
	// �ȸ�����Ӧ��
	if( !g_objApplication.MakeAppFrame(0x04, 0x0002, szTempBuf, dwTempLen, szReplyBuf, sizeof(szReplyBuf), dwReplyLen) )
	{
		g_objApplication.SendAppFrame(MODE_A, 0x80, false, v_dwCenterSEQ, szReplyBuf, dwReplyLen);
	}
//��ʼ�ϴ���Ƭ

	if( iret )
	{
		return iret;
	}
	if(FindFile((char *)Ftime))
	{
//		_UploadOnePhotoPack(m_objUploadPhotoSta.m_wPhotoIndex, 0);
	} else
	{
		PRTMSG(MSG_DBG, "Read file err!\n");
	}
}

int CLNPhoto::Deal42_0004(char *v_szBuf, DWORD v_dwLen, DWORD v_dwCenterSEQ)
{
	if(0x01 == v_szBuf[0])
	{
		PRTMSG(MSG_DBG, "Rec one package succ!\n");
	} else
	{
		PRTMSG(MSG_DBG, "Rec one package fail!\n");
	}
}

void CLNPhoto::SendPhotoRequest( BYTE v_bytSizeType, BYTE v_bytQualityType, BYTE v_bytBright,
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



/// �ϴ�һ����Ƭ: ����һ������,���߷���һ����Ƭ�����˳�,���м���ʧ��,Ҳ�˳�,����ֹͣ�ϴ���Ƭ
/// ��������еİ����,��δת���
int CLNPhoto::_UploadOnePhotoPack( WORD v_wPhotoIndex, WORD v_wBeginFrameNo, bool v_bNeedResp)
{
//	PRTMSG(MSG_DBG, "Upload One Pack of the photo\n" );
	int iret = 0;
	long lPos = 0;
	int  iSendLen = 0;
	char szFrame[ UDPDATA_SIZE ]; // ��Ƭ������
	DWORD szFramLen;
	char szTempBuf[ UDPDATA_SIZE + 200 ]; // 
	DWORD dwTempLen = 0;
	char szReplyBuf[ UDPDATA_SIZE + 200 ]; // 
	DWORD dwReplyLen = 0, dwReadLen;

	DWORD dwSndBytes = 0;
	FILE* fpPhoto = NULL;
	LINUX_HANDLE hPhotoFnd = {0};
	LINUX_HANDLE hDirFnd = {0};
	LINUX_FIND_DATA fdPhoto;
	LINUX_FIND_DATA fdDir;
	char szPhotoName[255] = {0};
	char szPhotoPath[255] = {0};
	char szPhotoIndex[ 32 ] = { 0};
	DWORD dwPhotoDate = 0, dwFrameLen;
	WORD wCurFrameNo = 0, wFrameTotal = 0;
	bool bFillFrame = false; // ѭ�����Ƿ������һ֡�ı�־
	BYTE byt = 0;
	char szTempBuf1[255] = {0};
	char szTempBuf2[255] = {0};

	// �������
	if( 0 == v_wPhotoIndex )
	{
		PRTMSG(MSG_ERR, "9\n" );
		iret = ERR_PAR;
		goto _UPLOADONEPHOTOPACK_END;
	}


	//�ϴ�ǰ����ϴ��жϱ�־
	m_objUploadPhotoSta.m_bUploadBreak = false;

	// ��Ҫ���͵���Ƭ��״̬��¼��һ��,�����״̬��¼������,�Ա�֮���¼��״̬ (��Ҫ)
	if( v_wPhotoIndex != m_objUploadPhotoSta.m_wPhotoIndex)
		_ClearUploadPhotoStaForNewPhoto();

	// ��ȡ��Ƭ���ڵ������ļ���
	dwPhotoDate = BYTE(m_objUploadPhotoSta.m_szDateTime[0]) * DWORD(10000) + BYTE(m_objUploadPhotoSta.m_szDateTime[1]) * DWORD(100) + BYTE(m_objUploadPhotoSta.m_szDateTime[2]);

	/// ׼����ƬĿ¼��
	memcpy(szPhotoPath, m_szPhotoPath, sizeof(m_szPhotoPath));
	sprintf( szPhotoIndex, "%04x", v_wPhotoIndex );

	if( v_wPhotoIndex == m_objUploadPhotoSta.m_wPhotoIndex)
	// ��Ҫ���͵���Ƭ��״̬��¼�е���� (Ӧ����Ҫ������Ƭ���´���)
	{
		wCurFrameNo = v_wBeginFrameNo;
	} else
	{
		// ֻ���ܵ�һ��ѭ��ʱ����
		wCurFrameNo = 0;
	}
	if( INVALID_NUM_16 == wCurFrameNo )
	{
		PRTMSG(MSG_ERR, "13\n" );
		iret = ERR_PAR;
		goto _UPLOADONEPHOTOPACK_END;
	}

	/// ��䵱ǰҪ���͵İ�����
	bFillFrame = false;
	dwFrameLen = 0;
	if( v_wPhotoIndex == m_objUploadPhotoSta.m_wPhotoIndex )
	// ��״̬�е���Ƭ��Ҫ���͵���Ƭ��ͬ
	{
		// ���������Ƿ����л����֡
		for( byt = 0; byt < UPLOADPHOTO_MAXCOUNT; byt ++ )
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
				szFramLen = m_objUploadPhotoSta.m_aryPhotoFrameLen[ byt ];
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
			iret = ERR_PAR;
			goto _UPLOADONEPHOTOPACK_END;
		} else if( strcmp(szPhotoName, m_objUploadPhotoSta.m_szPhotoPath) != 0  )
		// ������״̬��¼�е���Ƭ�͵�ǰ��Ƭ��ͬ,��Ҫ��ȡ��ǰ��Ƭ��Ϣ
		{
			// �����ļ���С
			if( fseek(fpPhoto, 0, SEEK_END) )
			{
				PRTMSG( MSG_DBG, "2\n" );
				iret = ERR_PAR;
				goto _UPLOADONEPHOTOPACK_END;
			}
			long lEnd = ftell( fpPhoto );
			if( lEnd <= 0 )
			{
				PRTMSG( MSG_DBG, "14\n" );
				iret = ERR_FILE_FAILED;
				goto _UPLOADONEPHOTOPACK_END;
			}
			DWORD dwFileSize = DWORD( lEnd );
			if( 0 == dwFileSize )
			{
				PRTMSG( MSG_DBG, "3\n" );
				iret = ERR_FILE_FAILED;
				goto _UPLOADONEPHOTOPACK_END;
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
				iret = ERR_FILE_FAILED;
				goto _UPLOADONEPHOTOPACK_END;
			}
			cmd5.MD5File16(szPhotoName, (byte *)m_objUploadPhotoSta.m_bytMd5);
			memcpy(m_bytMd5, m_objUploadPhotoSta.m_bytMd5, 16);
			// ��¼��״̬ (�����ظ�д����ͬ��,��Ҫ��,��һ�㲻Ӧ���ִ������)
			// ���ļ�����ȡ��Ƭ�����Ϣ
			IN_GetIndexFromPhotoName( szPhotoName, wPhotoIndex );
			IN_GetTypeFromPhotoName( szPhotoName, wPhotoType );
			IN_GetChannelSizeQualityFromPhotoName( szPhotoName, bytChannel, bytSizeType, bytQualityType );

			m_objUploadPhotoSta.m_wPhotoIndex = wPhotoIndex;
			m_objUploadPhotoSta.m_wPhotoType = wPhotoType;
			m_objUploadPhotoSta.m_dwPhotoSize = dwFileSize;	// ��Ƭ�ߴ�
			m_objUploadPhotoSta.m_wPhotoFrameTotal = WORD( m_objUploadPhotoSta.m_dwPhotoSize
														   / UDPDATA_SIZE + (m_objUploadPhotoSta.m_dwPhotoSize % UDPDATA_SIZE ? 1 : 0) ); // �ܰ���
			m_objUploadPhotoSta.m_bytPhotoChannel = bytChannel;	// ͨ����
			m_objUploadPhotoSta.m_szDateTime[ 0 ] = BYTE(dwDate / 10000); // ����ʱ��
			m_objUploadPhotoSta.m_szDateTime[ 1 ] = BYTE(dwDate % 10000 / 100);
			m_objUploadPhotoSta.m_szDateTime[ 2 ] = BYTE(dwDate % 100);
			m_objUploadPhotoSta.m_szDateTime[ 3 ] = BYTE(dwTime / 10000);
			m_objUploadPhotoSta.m_szDateTime[ 4 ] = BYTE(dwTime % 10000 / 100);
			m_objUploadPhotoSta.m_szDateTime[ 5 ] = BYTE(dwTime % 100);
			m_objUploadPhotoSta.m_bPhotoBeginUpload = true;
			m_objUploadPhotoSta.m_CurPackNo = 0;
			// ��Ƭȫ·����
			strncpy( m_objUploadPhotoSta.m_szPhotoPath, szPhotoPath,
					 sizeof(m_objUploadPhotoSta.m_szPhotoPath) / sizeof(m_objUploadPhotoSta.m_szPhotoPath[0]) );
			m_objUploadPhotoSta.m_szPhotoPath[ sizeof(m_objUploadPhotoSta.m_szPhotoPath) / sizeof(m_objUploadPhotoSta.m_szPhotoPath[0]) - 1 ] = 0;

			PRTMSG( MSG_DBG, "Cur Photo File: %s\n", szPhotoName );
			PRTMSG( MSG_DBG, "Photo File Size:%d, Packets Total: %d\n", 
					m_objUploadPhotoSta.m_dwPhotoSize, m_objUploadPhotoSta.m_wPhotoFrameTotal );
			wCurFrameNo = 0;
		}

		//////// ֱ�Ӵ���Ƭ�ļ���ȡ��Ƭ���� /////////
		for( int i = 0; i < UPLOADPHOTO_MAXCOUNT; i++ )
		{
			// ��ת����ȡ��
			if( fseek(fpPhoto, (i+wCurFrameNo) * UDPDATA_SIZE, SEEK_SET) )
			{
				goto _UPLOADONEPHOTOPACK_END;
			}
			dwFrameLen = fread(szFrame, 1, UDPDATA_SIZE, fpPhoto);
			//	PRTMSG( MSG_DBG, "Read %d B!\n", dwFrameLen);
			// ��ȡ
			if( dwFrameLen !=  UDPDATA_SIZE)
			{
				if( feof(fpPhoto) && !ferror(fpPhoto) )
				{
					// ��������ݰ�
					memcpy( m_objUploadPhotoSta.m_aryPhotoFrameBuf[ i ], szFrame, dwFrameLen );
					m_objUploadPhotoSta.m_aryPhotoFrameLen[ i ] = WORD( dwFrameLen );
					m_objUploadPhotoSta.m_aryPhotoFrameNo[ i ] = i+wCurFrameNo;
					bFillFrame = true;
					//	PRTMSG( MSG_DBG, "The last package!\n");
					break;
				} else
				{
					PRTMSG( MSG_DBG, "Read faild!\n");
					memset(&m_objUploadPhotoSta, 0, sizeof(m_objUploadPhotoSta));
					goto _UPLOADONEPHOTOPACK_END;
				}
			}
			//	PRTMSG( MSG_DBG, "Read one package!\n");
			// ��������ݰ�
			memcpy( m_objUploadPhotoSta.m_aryPhotoFrameBuf[ i ], szFrame, dwFrameLen );
			m_objUploadPhotoSta.m_aryPhotoFrameLen[ i ] = WORD( dwFrameLen );
			m_objUploadPhotoSta.m_aryPhotoFrameNo[ i ] = i + wCurFrameNo;
		}

		// ��û�гɹ����ð�,�˳�
		if( !bFillFrame )
		{
			PRTMSG(MSG_DBG, "16\n" );
			iret = ERR_FILE_FAILED;
			goto _UPLOADONEPHOTOPACK_END;
		}
		memcpy( szFrame, m_objUploadPhotoSta.m_aryPhotoFrameBuf[ 0 ],
				m_objUploadPhotoSta.m_aryPhotoFrameLen[ 0 ] );
		szFramLen = m_objUploadPhotoSta.m_aryPhotoFrameLen[ 0 ];
	}
	_UPLOADONEPHOTOPACK_END:
	if( iret )
	{
		return iret;
	}
	szTempBuf[dwTempLen++] = 19;
	memcpy(szTempBuf + dwTempLen, m_objUploadPhotoSta.m_bytMd5, 16);
	dwTempLen += 16;
	szTempBuf[dwTempLen++] = v_bNeedResp ? 0x01 : 0x00;

	szTempBuf[dwTempLen++] = htons(wCurFrameNo) & 0xff;
	szTempBuf[dwTempLen++] = (htons(wCurFrameNo) >> 8) & 0xff;  

	//Ӧ�ò�����
	szTempBuf[dwTempLen++] = 0x42;
	szTempBuf[dwTempLen++] = 0x00;  
	szTempBuf[dwTempLen++] = 0x04;  
	szTempBuf[dwTempLen++] = htons(szFramLen) & 0xff;
	szTempBuf[dwTempLen++] = (htons(szFramLen) >> 8) & 0xff;
	memcpy(szTempBuf + dwTempLen, szFrame, szFramLen);
	dwTempLen += szFramLen;
	// �ȸ�����Ӧ��
	if( !g_objApplication.MakeAppFrame(0x42, 0x0004, szTempBuf, dwTempLen, szReplyBuf, sizeof(szReplyBuf), dwReplyLen) )
	{
		g_objApplication.SendAppFrame(MODE_D, 0xb1, true, 0, szReplyBuf, dwReplyLen);
	}

}

bool CLNPhoto::FindFile(char *v_FileTime)
{
	DWORD dwPhotoDate, dwPhotoTime, dwTarPhotoDate, dwTarPhotoTime, dwFrameLen;
	bool bret = false;
	char szPhotoDir[255];
	char szFileName[255];
	char szPhotoPath[255];
	char szFrame[UDPDATA_SIZE];
	int n; 
	WORD wPhotoIndex, wPhotoType;
	BYTE bytChannel, bytSizeType, bytQualityType;
	FILE* fpPhoto = NULL;
	struct dirent **namelist;
tagPhotoInfo objPhotoInfo;
	if( v_FileTime[1] > 12 || v_FileTime[2] > 31|| v_FileTime[3] > 23 || v_FileTime[4] > 59 || v_FileTime[5] > 59)
	{
		goto _FINDFILE_END;
	}

	// ����ʱ������ֲ���
	dwTarPhotoDate = BYTE(v_FileTime[0]) * DWORD(10000) + BYTE(v_FileTime[1]) * DWORD(100) + BYTE(v_FileTime[2]);
	dwTarPhotoTime = BYTE(v_FileTime[3]) * DWORD(10000) + BYTE(v_FileTime[4]) * DWORD(100) + BYTE(v_FileTime[5]);


	// ʱ��β����Ϸ��Լ���
	if( dwTarPhotoDate < BELIV_DATE )
	{
		goto _FINDFILE_END;
	}

	IN_MakePhotoDirName( dwTarPhotoDate, szPhotoDir );

	n = scandir(szPhotoDir, &namelist, 0, alphasort);
	if(n < 0)
		perror("scandir");
	else
	{
		while(n--)
		{
			if( 0 == strcmp(".", namelist[n]->d_name) || 0 == strcmp("..", namelist[n]->d_name) )
				continue;
			// ���ļ��������Ƭ����ʱ��
			IN_GetDateTimeFromPhotoName( namelist[n]->d_name, dwPhotoDate, dwPhotoTime );
			if( dwPhotoDate != dwTarPhotoDate || dwPhotoTime != dwTarPhotoTime)
				continue;
			sprintf(szPhotoPath, "%s%s", szPhotoDir, namelist[n]->d_name);
//			PRTMSG( MSG_DBG, "Cur Photo File: %s\n", szPhotoPath );
			fpPhoto = fopen(szPhotoPath, "rb" );
			if( !fpPhoto )
			{
				PRTMSG( MSG_DBG, "File open err!\n");
				continue;
			}
// �����ļ���С
			if( fseek(fpPhoto, 0, SEEK_END) )
			{
				PRTMSG( MSG_DBG, "File seek err!\n");
				goto _FINDFILE_END;
			}
			long lEnd = ftell( fpPhoto );
			if( lEnd <= 0 )
			{
				PRTMSG( MSG_DBG, "File tell err!\n");
				goto _FINDFILE_END;
			}
			DWORD dwFileSize = DWORD(lEnd);
			if( dwFileSize < 5 * 1024 )
			{
				fclose(fpPhoto);
				fpPhoto = NULL;
				PRTMSG( MSG_DBG, "File size err!\n");
				continue;
			}

//			cmd5.MD5File16(szPhotoPath, (byte *)m_objUploadPhotoSta.m_bytMd5);
//			memcpy(m_bytMd5, m_objUploadPhotoSta.m_bytMd5, 16);
// ���ļ�����ȡ��Ƭ�����Ϣ
			IN_GetIndexFromPhotoName( namelist[n]->d_name, wPhotoIndex );
#if 0
			IN_GetTypeFromPhotoName( namelist[n]->d_name, wPhotoType );
			IN_GetChannelSizeQualityFromPhotoName( namelist[n]->d_name, bytChannel, bytSizeType, bytQualityType );

			m_objUploadPhotoSta.m_wPhotoIndex = wPhotoIndex;
			m_objUploadPhotoSta.m_wPhotoType = wPhotoType;
			m_objUploadPhotoSta.m_dwPhotoSize = dwFileSize;	// ��Ƭ�ߴ�
			m_objUploadPhotoSta.m_wPhotoFrameTotal = WORD( m_objUploadPhotoSta.m_dwPhotoSize
														   / UDPDATA_SIZE + (m_objUploadPhotoSta.m_dwPhotoSize % UDPDATA_SIZE ? 1 : 0) ); // �ܰ���
			m_objUploadPhotoSta.m_bytPhotoChannel = bytChannel;	// ͨ����
			memcpy(m_objUploadPhotoSta.m_szDateTime, v_FileTime, 6);// ����ʱ��
			m_objUploadPhotoSta.m_bPhotoBeginUpload = true;
			m_objUploadPhotoSta.m_CurPackNo = 0;
			// ��Ƭȫ·����
			strncpy( m_objUploadPhotoSta.m_szPhotoPath, szPhotoPath,
					 sizeof(m_objUploadPhotoSta.m_szPhotoPath) / sizeof(m_objUploadPhotoSta.m_szPhotoPath[0]) );
			m_objUploadPhotoSta.m_szPhotoPath[ sizeof(m_objUploadPhotoSta.m_szPhotoPath) / sizeof(m_objUploadPhotoSta.m_szPhotoPath[0]) - 1 ] = 0;

			PRTMSG( MSG_DBG, "Cur Photo File: %s\n", szPhotoPath );
			PRTMSG( MSG_DBG, "Photo File Size:%d, Packets Total: %d\n", 
					m_objUploadPhotoSta.m_dwPhotoSize, m_objUploadPhotoSta.m_wPhotoFrameTotal );

			//////// ֱ�Ӵ���Ƭ�ļ���ȡ��Ƭ���� /////////

			for( int i = 0; i < UPLOADPHOTO_MAXCOUNT; i++ )
			{
				// ��ת����ȡ��
				if( fseek(fpPhoto, i * UDPDATA_SIZE, SEEK_SET) )
				{
					goto _FINDFILE_END;
				}
				dwFrameLen = fread(szFrame, 1, UDPDATA_SIZE, fpPhoto);
				//	PRTMSG( MSG_DBG, "Read %d B!\n", dwFrameLen);
				// ��ȡ
				if( dwFrameLen !=  UDPDATA_SIZE)
				{
					if( feof(fpPhoto) && !ferror(fpPhoto) )
					{
						// ��������ݰ�
						memcpy( m_objUploadPhotoSta.m_aryPhotoFrameBuf[ i ], szFrame, dwFrameLen );
						m_objUploadPhotoSta.m_aryPhotoFrameLen[ i ] = WORD( dwFrameLen );
						m_objUploadPhotoSta.m_aryPhotoFrameNo[ i ] = i;
						//	PRTMSG( MSG_DBG, "The last package!\n");
						break;
					} else
					{
						PRTMSG( MSG_DBG, "Read faild!\n");
						memset(&m_objUploadPhotoSta, 0, sizeof(m_objUploadPhotoSta));
						goto _FINDFILE_END;
					}
				}
				//	PRTMSG( MSG_DBG, "Read one package!\n");
				// ��������ݰ�
				memcpy( m_objUploadPhotoSta.m_aryPhotoFrameBuf[ i ], szFrame, dwFrameLen );
				m_objUploadPhotoSta.m_aryPhotoFrameLen[ i ] = WORD( dwFrameLen );
				m_objUploadPhotoSta.m_aryPhotoFrameNo[ i ] = i;
			}
#endif
			bret = true;
			break;
		}
	}
	
	objPhotoInfo.m_wPhotoIndex = wPhotoIndex;
	P_AddUploadPhoto(&objPhotoInfo, 1);
	_FINDFILE_END:
	if( fpPhoto )
	{
		fclose(fpPhoto);
	}
	return bret;
}

int CLNPhoto::FillUpload( WORD v_wPhotoIndex)
{
	int iret = 0;
	long lPos = 0;

	FILE* fpPhoto = NULL;
	LINUX_HANDLE hPhotoFnd = {0};
	LINUX_HANDLE hDirFnd = {0};
	LINUX_FIND_DATA fdPhoto;
	LINUX_FIND_DATA fdDir;
	char szPhotoName[255] = {0};
	char szPhotoPath[255] = {0};
	DWORD dwFrameLen;
	WORD wFrameTotal = 0;
	DWORD dwFileSize;
	long lEnd;
	BYTE byt = 0;
	char szTempBuf1[255] = {0};
	char szTempBuf2[255] = {0};
	char szPhotoIndex[32] = {0};
	char szFrame[UDPDATA_SIZE];
	// �������
	if( 0 == v_wPhotoIndex )
	{
		iret = ERR_PAR;
		goto _FILLUPLOAD_END;
	}
	_ClearUploadPhotoStaForNewPhoto();
/// ׼����ƬĿ¼��
	memcpy(szPhotoPath, m_szPhotoPath, sizeof(m_szPhotoPath));
	sprintf( szPhotoIndex, "%04x", v_wPhotoIndex );
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


	if( !fpPhoto ) // ��û���ҵ�
	{
		PRTMSG( MSG_DBG, "Find File Fail in Disk,Index: %04x\n", szPhotoIndex );
		iret = ERR_PAR;
		goto _FILLUPLOAD_END;
	}
	// �����ļ���С
	if( fseek(fpPhoto, 0, SEEK_END) )
	{
		PRTMSG( MSG_DBG, "2\n" );
		iret = ERR_PAR;
		goto _FILLUPLOAD_END;
	}
	lEnd = ftell( fpPhoto );
	if( lEnd <= 0 )
	{
		PRTMSG( MSG_DBG, "14\n" );
		iret = ERR_FILE_FAILED;
		goto _FILLUPLOAD_END;
	}
	dwFileSize = DWORD( lEnd );
	if( 0 == dwFileSize )
	{
		PRTMSG( MSG_DBG, "3\n" );
		iret = ERR_FILE_FAILED;
		goto _FILLUPLOAD_END;
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
		iret = ERR_FILE_FAILED;
		goto _FILLUPLOAD_END;
	}
	cmd5.MD5File16(szPhotoName, (byte *)m_objUploadPhotoSta.m_bytMd5);
	memcpy(m_bytMd5, m_objUploadPhotoSta.m_bytMd5, 16);
	// ��¼��״̬ (�����ظ�д����ͬ��,��Ҫ��,��һ�㲻Ӧ���ִ������)
	// ���ļ�����ȡ��Ƭ�����Ϣ
	IN_GetIndexFromPhotoName( szPhotoName, wPhotoIndex );
	IN_GetTypeFromPhotoName( szPhotoName, wPhotoType );
	IN_GetChannelSizeQualityFromPhotoName( szPhotoName, bytChannel, bytSizeType, bytQualityType );

	m_objUploadPhotoSta.m_wPhotoIndex = wPhotoIndex;
	m_objUploadPhotoSta.m_wPhotoType = wPhotoType;
	m_objUploadPhotoSta.m_dwPhotoSize = dwFileSize;	// ��Ƭ�ߴ�
	m_objUploadPhotoSta.m_wPhotoFrameTotal = WORD( m_objUploadPhotoSta.m_dwPhotoSize
												   / UDPDATA_SIZE + (m_objUploadPhotoSta.m_dwPhotoSize % UDPDATA_SIZE ? 1 : 0) ); // �ܰ���
	m_objUploadPhotoSta.m_bytPhotoChannel = bytChannel;	// ͨ����
	m_objUploadPhotoSta.m_szDateTime[ 0 ] = BYTE(dwDate / 10000); // ����ʱ��
	m_objUploadPhotoSta.m_szDateTime[ 1 ] = BYTE(dwDate % 10000 / 100);
	m_objUploadPhotoSta.m_szDateTime[ 2 ] = BYTE(dwDate % 100);
	m_objUploadPhotoSta.m_szDateTime[ 3 ] = BYTE(dwTime / 10000);
	m_objUploadPhotoSta.m_szDateTime[ 4 ] = BYTE(dwTime % 10000 / 100);
	m_objUploadPhotoSta.m_szDateTime[ 5 ] = BYTE(dwTime % 100);
	m_objUploadPhotoSta.m_bPhotoBeginUpload = true;
	m_objUploadPhotoSta.m_CurPackNo = 0;
	// ��Ƭȫ·����
	strncpy( m_objUploadPhotoSta.m_szPhotoPath, szPhotoPath,
			 sizeof(m_objUploadPhotoSta.m_szPhotoPath) / sizeof(m_objUploadPhotoSta.m_szPhotoPath[0]) );
	m_objUploadPhotoSta.m_szPhotoPath[ sizeof(m_objUploadPhotoSta.m_szPhotoPath) / sizeof(m_objUploadPhotoSta.m_szPhotoPath[0]) - 1 ] = 0;

	PRTMSG( MSG_DBG, "Cur Photo File: %s\n", szPhotoName );
	PRTMSG( MSG_DBG, "Photo File Size:%d, Packets Total: %d\n", 
			m_objUploadPhotoSta.m_dwPhotoSize, m_objUploadPhotoSta.m_wPhotoFrameTotal );

	//////// ֱ�Ӵ���Ƭ�ļ���ȡ��Ƭ���� /////////
	for( int i = 0; i < UPLOADPHOTO_MAXCOUNT; i++ )
	{
		// ��ת����ȡ��
		if( fseek(fpPhoto, i * UDPDATA_SIZE, SEEK_SET) )
		{
			goto _FILLUPLOAD_END;
		}
		dwFrameLen = fread(szFrame, 1, UDPDATA_SIZE, fpPhoto);
		//	PRTMSG( MSG_DBG, "Read %d B!\n", dwFrameLen);
		// ��ȡ
		if( dwFrameLen !=  UDPDATA_SIZE)
		{
			if( feof(fpPhoto) && !ferror(fpPhoto) )
			{
				// ��������ݰ�
				memcpy( m_objUploadPhotoSta.m_aryPhotoFrameBuf[ i ], szFrame, dwFrameLen );
				m_objUploadPhotoSta.m_aryPhotoFrameLen[ i ] = WORD( dwFrameLen );
				m_objUploadPhotoSta.m_aryPhotoFrameNo[ i ] = i;
				//	PRTMSG( MSG_DBG, "The last package!\n");
				break;
			} else
			{
				PRTMSG( MSG_DBG, "Read faild!\n");
				memset(&m_objUploadPhotoSta, 0, sizeof(m_objUploadPhotoSta));
				goto _FILLUPLOAD_END;
			}
		}
		//	PRTMSG( MSG_DBG, "Read one package!\n");
		// ��������ݰ�
		memcpy( m_objUploadPhotoSta.m_aryPhotoFrameBuf[ i ], szFrame, dwFrameLen );
		m_objUploadPhotoSta.m_aryPhotoFrameLen[ i ] = WORD( dwFrameLen );
		m_objUploadPhotoSta.m_aryPhotoFrameNo[ i ] = i;
	}

	_FILLUPLOAD_END:
	if( fpPhoto )
	{
		fclose(fpPhoto);
	}
	return iret;


}

void CLNPhoto::_ClearUploadPhotoStaForNewPhoto()
{
	memset(&m_objUploadPhotoSta, 0, sizeof(m_objUploadPhotoSta));
}

void CLNPhoto::P_ThreadSendPhoto()
{
	DWORD   dwSleepTm = 0;
	int     Cnt = 0, iret;        
	byte    bytLvl = 0;
	DWORD   dwPushTm = 0;
	ushort  usCurPackageNo, usBeginNo, usPackageLen = UDPDATA_SIZE;;
	char szSendBuf[1600] = {0};
	DWORD dwSendLen = 0;
	static byte errcnt = 0;
	char bytRecvBuf[100];
	DWORD dwRecvLen;
	int loopcnt, sendcnt = 0;  
	while( !m_bThreadExit )
	{
		switch( m_bytSta )
		{
		case Idlesse:   
			{
				dwSleepTm = 100000;
				pthread_mutex_lock(&m_mutex_photo);
				if(errcnt > 11)
				{
					m_bytAllPhotoTotal = 0;
					m_bytAllPhotoCt = 0;
					memset(m_aryUpPhotoInfo, 0, sizeof(m_aryUpPhotoInfo));
					errcnt = 0;
				}
				if(m_bytAllPhotoTotal > m_bytAllPhotoCt)	// 
				{
					m_bytSta = DispTask;
				}
				pthread_mutex_unlock(&m_mutex_photo);
			}
			break;

		case DispTask:
			{
				if(true !=  m_objUploadPhotoSta.m_bPhotoBeginUpload )
				{
					if(!FillUpload(m_aryUpPhotoInfo[0].m_wPhotoIndex))
					{
						errcnt++;
						m_bytSta = Idlesse;
						break;
					}
				}
				dwSendLen = 0;
				// ����
				memcpy(szSendBuf, m_bytMd5, 16);
				dwSendLen = 16;
				G_LocalToNet((char*)&m_objUploadPhotoSta.m_dwPhotoSize, szSendBuf+dwSendLen, 4);
				dwSendLen += 4;
				G_LocalToNet((char*)&usPackageLen, szSendBuf+dwSendLen, 2);
				dwSendLen += 2;

				memcpy(szSendBuf+dwSendLen, m_objUploadPhotoSta.m_szDateTime, 6);
				dwSendLen += 6;
				szSendBuf[dwSendLen++] = m_objUploadPhotoSta.m_bytPhotoChannel;
				szSendBuf[dwSendLen++] = (byte)m_objUploadPhotoSta.m_wPhotoType;
				g_objApplication.SendAppFrame(MODE_D, 0xb0, true, 0, szSendBuf, dwSendLen);
				m_bytSta = ReqAttr;

			} 
			break;
		case ReqAttr:
			{
				dwSleepTm = 50000;

				iret = _WaitForAnswer(10000, 0x30, 0x00, bytRecvBuf, sizeof(bytRecvBuf), &dwRecvLen);

				if(iret == 1)
				{
					m_bytSta = SendData;
					usBeginNo = m_objUploadPhotoSta.m_CurPackNo;
					memset(m_bytBlockSta, 0, sizeof(m_bytBlockSta));
					loopcnt = min(m_objUploadPhotoSta.m_wPhotoFrameTotal, 16); 
					sendcnt = 0;
//					PRTMSG(MSG_DBG, "FrameTotal:%d BeginNo:%d loop:%d\n", m_objUploadPhotoSta.m_wPhotoFrameTotal, usBeginNo, loopcnt);
					break;
				}
				errcnt++;
				m_bytSta = Idlesse;
			}
			break;

		case SendData:
			{
				dwSleepTm = 50000;
				int i;
				sendcnt++;          
				for( i = 0; i < loopcnt ; i++ )
				{
					if(0x00 == m_bytBlockSta[i])
					{
						if(!_UploadOnePhotoPack(m_objUploadPhotoSta.m_wPhotoIndex, usBeginNo + i, i==loopcnt-1 ? true : false))
						{	
//							PRTMSG(MSG_DBG, "pack:%d\n", i);
							m_bytBlockSta[i] = 0xff;
							usCurPackageNo = usBeginNo + i;
						}

					}
				}

				iret = _WaitForAnswer(10000, 0x31, 0x01, bytRecvBuf, sizeof(bytRecvBuf), &dwRecvLen);
				if(iret != 4)
				{
					m_bytSta = UpdataRes;
					break;
				}

				m_bytSta = ReportChk;
			}
			break;  

		case ReportChk:	//
			{
				dwSleepTm = 50000;
				dwSendLen = 0;

				// ����
				memcpy(szSendBuf, m_bytMd5, 16);
				dwSendLen = 16;
				G_LocalToNet((char*)&m_objUploadPhotoSta.m_dwPhotoSize, szSendBuf+dwSendLen, 4);
				dwSendLen += 4;
				G_LocalToNet((char*)&usCurPackageNo, szSendBuf+dwSendLen, 2);
				dwSendLen += 2;
				g_objApplication.SendAppFrame(MODE_D, 0xb2, true, 0, szSendBuf, dwSendLen);

				iret = _WaitForAnswer(10000, 0x32, 0x01, bytRecvBuf, sizeof(bytRecvBuf), &dwRecvLen);
				if(iret != 4)
				{
					m_bytSta = UpdataRes;
					break;
				}
				errcnt++;
				if( errcnt > 11 )
				{
					m_bytSta = Idlesse;
				}
			}
			break;

		case UpdataRes:
			{

				dwSleepTm = 50000;
				ushort packno;
				if( dwRecvLen >= 3 )
				{
					packno  = bytRecvBuf[0] * 256 + (byte)bytRecvBuf[1];
					if( 0x01 == iret)
					{
						m_objUploadPhotoSta.m_CurPackNo += min(m_objUploadPhotoSta.m_wPhotoFrameTotal, 16); 
						if( m_objUploadPhotoSta.m_CurPackNo * usPackageLen >= m_objUploadPhotoSta.m_dwPhotoSize )
						{
							dwSendLen = 0;                  
							// ����
							szSendBuf[dwSendLen++] = 0x00;
							g_objApplication.SendAppFrame(MODE_D, 0xb4, true, 0, szSendBuf, dwSendLen);
							m_bytSta = CutComu;
							break;
						}
						loopcnt = min(m_objUploadPhotoSta.m_wPhotoFrameTotal - sendcnt*16, 16); 
						m_bytSta = SendData;
						usBeginNo = m_objUploadPhotoSta.m_CurPackNo;
						memset(m_bytBlockSta, 0, sizeof(m_bytBlockSta));
//						PRTMSG(MSG_DBG, "2 FrameTotal:%d BeginNo:%d loop:%d\n", m_objUploadPhotoSta.m_wPhotoFrameTotal, usBeginNo, loopcnt);
					} else if( 0x02 == iret)
					{
						if( bytRecvBuf[3] > 0 )
						{
							for(int i = 0; i < bytRecvBuf[3]; i++ )
							{
								packno  = bytRecvBuf[i*2] * 256 + (byte)bytRecvBuf[i*2 + 1];
								if( packno >= usBeginNo && packno <= usCurPackageNo )
								{
									m_bytBlockSta[packno-usBeginNo] = 0x00;
								}
							}

							m_bytSta = SendData;
							sendcnt--;
							//	break;                      
						}
					} else if( 0x03 == iret)
					{
						m_bytSta = SendData;
						usBeginNo = m_objUploadPhotoSta.m_CurPackNo;
						memset(m_bytBlockSta, 0, sizeof(m_bytBlockSta));
					}
				}
			}
			break;

		case CutComu:
			{
				iret = _WaitForAnswer(10000, 0x34, 0x00, bytRecvBuf, sizeof(bytRecvBuf), &dwRecvLen);
				if(iret == 1)
				{
					memset(&m_objUploadPhotoSta, 0, sizeof(m_objUploadPhotoSta));

				}
				pthread_mutex_lock(&m_mutex_photo);
				m_bytAllPhotoCt++;
				if( m_bytAllPhotoCt == m_bytAllPhotoTotal )
				{
					m_bytAllPhotoCt = 0;
					m_bytAllPhotoTotal = 0;
				}
				memset(&m_objUploadPhotoSta, 0, sizeof(m_objUploadPhotoSta));
				pthread_mutex_unlock(&m_mutex_photo);

				m_bytSta = Idlesse;
			}
			break;  
		default:
			break;
		}

		usleep(dwSleepTm);
	}
}

// �ȴ�����Ӧ��, ���ͣ�1����Ӧ�𵽣� 2���������� 3�������ճ��� 4--Ӧ��δ��
int  CLNPhoto::_WaitForAnswer(DWORD v_dwWaitTm, char v_szDataType, char v_szResult, char *v_bytRecvBuf, DWORD v_dwBufLen, DWORD *v_dwRecvLen)
{
	DWORD dwBeginWaiTm = GetTickCount();
	byte  bytLvl;
	DWORD dwPushTm;
	char bytTempBuf[100];
	DWORD dwTempLen;

	int   iResult = 4; // ȱʡδӦ��δ��

	while( GetTickCount() - dwBeginWaiTm < v_dwWaitTm )
	{
		// ��m_objRecvMsg��pop�����������Ѿ���У�飬��ȥ��У��͡�����ID
		if( !m_objRecvMsg.PopData(bytLvl, sizeof(bytTempBuf), dwTempLen, bytTempBuf, dwPushTm) )
		{
			if( dwTempLen >  v_dwBufLen)
			{
				iResult = -1;
				goto _WAIT_END;
			}
			// ��ҵ�����ͺ��������Ͷ�ƥ�䣬�յ�Ӧ��
			if( v_szDataType == 0x30 || 0x34 == v_szDataType)
			{
				if(v_szDataType == bytTempBuf[0] && v_szResult == bytTempBuf[1])
				{
					iResult = 1;
					goto _WAIT_END;
				}
			}
			if( v_szDataType == 0x31 || 0x32 == v_szDataType || 0x33 == v_szDataType)
			{
				if( bytTempBuf[0] == v_szDataType)
				{
					memcpy(v_bytRecvBuf, bytTempBuf + 1, dwTempLen - 1);
					*v_dwRecvLen = dwTempLen - 1;
					if( 0x01 == bytTempBuf[3] )
					{
						iResult = 1;
						goto _WAIT_END;
					}
					if( 0x02 == bytTempBuf[3] )
					{
						iResult = 2;
						goto _WAIT_END;
					}
					if( 0x03 == bytTempBuf[3] && 0x33 != bytTempBuf[0])
					{
						iResult = 3;
						goto _WAIT_END;
					}
				}
			}
		} else
		{
			usleep(20000);
		}
	}

	_WAIT_END:
	return iResult;
}

WORD CLNPhoto::_UploadTypeToPhotoType( WORD v_wUploadType )
{
	return v_wUploadType; // ע��: ��Ϊ��Ƭ���ͺ��ϴ����͵Ķ�����ȫһ��,���Բ�������!
}
#endif
