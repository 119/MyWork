#include "yx_QianStdAfx.h"

#if USE_AUTORPTSTATION == 1

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-AutoRptStation  ")

//����ֱ��ӦF1��F2��F3��......�������ļ�
char strVFile[9][12] =	{"F100","F200","F300","F400","F500","F600","","",""};

void *G_ThreadAutoRptStation(void *arg)
{
	g_objAuoRptStationNew.P_ThreadAutoRptStation();
}

void G_TmSetCheCi(void *arg, int len)
{
	g_objAuoRptStationNew.P_TmSetCheCi();
}

void G_InLinesTip()
{
	g_objAuoRptStationNew.Show_diaodu("���������ļ��У������U����ϵ�");
}

//////////////////////////////////////////////////////////////////////////
CAutoRptStationNew::CAutoRptStationNew()
{
	m_ucStaCnt = 0;
	m_bytCheCiTmp = 0x0f;
	m_bytCheCi = 0x0f;		//��ʾ������
	m_bytEnbRptSta = 1;		//��������Ӫ��״̬.��ֹ��;��̨��λ����վ����
	m_dwInStaTm = 0;		// ��վʱ��
	m_dwOutStaTm = 0;		// ��վʱ��
	m_usCurZhiTm = 0;
	m_usCurPaoTm = 0;
	m_ucCurLimitSpd  =  100;//����/Сʱ��Ĭ������Ϊ50����������̫С������Ȼ����ٱ���	
	m_ucLstUpDown = 0xff;

	m_objRcvDataMng.InitCfg(10*1024, 30000);
}

CAutoRptStationNew::~CAutoRptStationNew()
{
	
}

int CAutoRptStationNew::init()
{
	// �����߳�
	pthread_t pThreadAutoRptStation;
	if( pthread_create(&pThreadAutoRptStation, NULL, G_ThreadAutoRptStation, NULL) !=0 )
	{
		PRTMSG(MSG_ERR, "Create thread failed!\n");
		perror("");

		return ERR_THREAD;
	}

	return 0;
}

int CAutoRptStationNew::Release()
{

}

void CAutoRptStationNew::P_ThreadAutoRptStation()
{
	tagGPSData gps(0);
	char iResult = 0;	
	char szTip[300] = { 0 };
	int iLen = 0, i = 0, j = 0;	
	bool bRet = true;
	char szRcv[500] = { 0 };
	DWORD dwRcvLen = 0;
	DWORD dwPushTm = 0;
	BYTE bytLvl = 0;
	bool bLstRcvCmd = false;
	const DWORD STATION_MAXSIZE = sizeof(m_objStations.m_aryStation) / sizeof(m_objStations.m_aryStation[0]);	
	double aryStaDis[STATION_MAXSIZE] = { 0 };			//���ڱ��������վ��֮��ľ���
	float max_direction1 = 0.0, min_direction1 = 0.0;	//���뱨վ�뾶��GPS����Χ
	float max_direction2 = 0.0, min_direction2 = 0.0;
	BYTE bytareaNo = 0; //��վ��ֻ���������ڵļ���վ��ľ���
	char buf[200] = { 0 };
	char buf2[200] = { 0 };

	szTip[0] = 6;

	//�Ƿ����������Զ���վ����
	tag2QRptStationCfg objRptStationcfg;
	GetSecCfg( &objRptStationcfg, sizeof(objRptStationcfg), offsetof(tagSecondCfg, m_uni2QRptStationCfg.m_obj2QRptStationCfg), sizeof(objRptStationcfg) );
	m_bytEnbRptSta = objRptStationcfg.m_bytRptSta;

	if (m_bytEnbRptSta)
	{
		ShowAvTxt("Ӫ��","");
	}
	else 
	{
		ShowAvTxt("��Ӫ��","");
	}

	LoadStations( m_objStations, true, objRptStationcfg.m_iLineNo );

// 	for(int k=0; k<3; k++)
// 	{
// 		printf("m_objStations.m_aryStation[%d].m_iCurNo = %d\n", k, m_objStations.m_aryStation[k].m_iCurNo);
// 		printf("m_objStations.m_aryStation[%d].m_iUniId = %d\n", k, m_objStations.m_aryStation[k].m_iUniId);
// 		printf("m_objStations.m_aryStation[%d].m_bIsPlayed = %d\n", k, m_objStations.m_aryStation[k].m_bIsPlayed);	
// 		printf("m_objStations.m_aryStation[%d].m_lLat = %ld\n", k, m_objStations.m_aryStation[k].m_lLat);
// 		printf("m_objStations.m_aryStation[%d].m_lLon = %ld\n", k, m_objStations.m_aryStation[k].m_lLon);
// 		printf("m_objStations.m_aryStation[%d].m_iRad = %d\n", k, m_objStations.m_aryStation[k].m_iRad);
// 		printf("m_objStations.m_aryStation[%d].m_fdirect = %f\n", k, m_objStations.m_aryStation[k].m_fdirect);
// 		printf("m_objStations.m_aryStation[%d].m_bytStaAttr = %d\n", k, m_objStations.m_aryStation[k].m_bytStaAttr);
// 		printf("m_objStations.m_aryStation[%d].m_usZhiTm = %d\n", k, m_objStations.m_aryStation[k].m_usZhiTm);
// 		printf("m_objStations.m_aryStation[%d].m_usPaoTm = %d\n", k, m_objStations.m_aryStation[k].m_usPaoTm);
// 		printf("m_objStations.m_aryStation[%d].m_bytrecordfile = %d\n", k, m_objStations.m_aryStation[k].m_bytrecordfile);
// 		printf("m_objStations.m_aryStation[%d].m_szrecord:%s\n", k, m_objStations.m_aryStation[k].m_szrecord);
// 		printf("m_objStations.m_aryStation[%d].m_szName:%s\n", k, m_objStations.m_aryStation[k].m_szName);
// 		printf("m_objStations.m_aryStation[%d].m_szPlay:%s\n", k, m_objStations.m_aryStation[k].m_szPlay);
// 	}

	bytareaNo = m_objStations.m_iStaTotal/2;	//��������ʱ������������վ��֮��ľ���
	m_objRptSta.m_iPlayNo = 0;

	// ���Ӷ��ظ���վ�Ĺ���
	int iLastStationNo = 0;
	int tempStationNo = 0;  // ���Ӷ���վԽսվ�������ж�

	ShowToAV(m_objRptSta.m_iPlayNo);

	while( !g_bProgExit )
	{
		if( m_objStations.m_iStaTotal >= STATION_MAXSIZE )
		{
			PRTMSG(MSG_ERR, "Auto Rpt Station Err 100\n" );
			continue;
		}			
		
		bLstRcvCmd = false;
		
		while( !m_objRcvDataMng.PopData(bytLvl, sizeof(szRcv), dwRcvLen, szRcv, dwPushTm) )
		{
			bLstRcvCmd = true;

			if( dwRcvLen < 1 ) goto _POPRCVDATA_NEXT;

			switch( BYTE(szRcv[0]) )
			{		
			case 0x01:
				{
					switch(BYTE(szRcv[1]))
					{
					case 0x01:
					case 0x02:
					case 0x03:
					case 0x04:
					case 0x05:
					case 0x06:
						{
							// (��) �����ض�����
						}
						break;

					case 0x07:
						{
							SetWorkSta();		//ʵ�ʿ����Ұ����������ˣ�		
						}
						break;
						
					case 0x08:
						{
							
						}
						break;

					case 0x09: // �س�/�����л�
						{
							static int sta_iChan = 0;
							if (sta_iChan++%2)
							{
								ShowAvTxt("�س�-�°�","");
								m_bytCheCiTmp = 2; 
							}
							else
							{
								ShowAvTxt("����-�ϰ�","");
								m_bytCheCiTmp = 1;
							}	
							m_bytEnbRptSta = true; // Ϊʲô���￪����վ��Ӧ�ûس��رգ�����������

							_SetTimer(&g_objTimerMng, SET_CHECI_TIMER, 5000,G_TmSetCheCi);	
						}
						break;
					}
				}
				break;

			case 0x02://��ǰ����һվ���������� ,���ͷ
				{
					PRTMSG( MSG_NOR, "Front station!\n");

					if((m_objRptSta.m_iPlayNo >= 0) && (m_objRptSta.m_iPlayNo < m_objStations.m_iStaTotal))
					{
						if(0==m_objRptSta.m_iPlayNo)
						{
							m_objRptSta.m_iPlayNo = (m_objStations.m_iStaTotal-1);
						}
						else
						{
							m_objRptSta.m_iPlayNo -= 1;
						}
					}

					ShowToAV(m_objRptSta.m_iPlayNo);
				}
				break;

			case 0x03://�������һս���Ҽ�ͷ
				{	
					if(m_bytEnbRptSta) // ����Ӫ��״̬
					{
						if((m_objRptSta.m_iPlayNo >= 0) && (m_objRptSta.m_iPlayNo < m_objStations.m_iStaTotal))
						{
							if(m_objRptSta.m_iPlayNo == (m_objStations.m_iStaTotal-1))
							{
								m_objRptSta.m_iPlayNo = 0;
							}
							else
							{
								m_objRptSta.m_iPlayNo += 1;
							}
						}
						ShowToAV(m_objRptSta.m_iPlayNo);
					}
					else		// ���ڷ�Ӫ��״̬
					{
						static int sOutWork = 0; //��Ӫ�˸�״̬�л�
						int offset = 2;

//						if (0 == sOutWork) 
//						{
//							m_bytCheCiTmp = 1; //����
//							ShowAvTxt("����-�ϰ�","");
//						}
//						if (1 == sOutWork) 
//						{
//							m_bytCheCiTmp = 2; //�س�
//							ShowAvTxt("�س�-�°�","");
//						}

						if (2-offset == sOutWork) 
						{
							m_bytCheCiTmp = 3;  //����
							ShowAvTxt("��Ӫ��-����","");
						}
						if (3-offset == sOutWork) 
						{
							m_bytCheCiTmp = 4; //����
							ShowAvTxt("��Ӫ��-����","");
						}
						if (4-offset == sOutWork) 
						{
							m_bytCheCiTmp = 5; //�޳�
							ShowAvTxt("��Ӫ��-�޳�","");
						}
						if (5-offset == sOutWork) 						
						{
							m_bytCheCiTmp = 6; //����
							ShowAvTxt("��Ӫ��-����","");
						}
						if (6-offset == sOutWork) 
						{
							m_bytCheCiTmp = 7; //�ſ�
							ShowAvTxt("��Ӫ��-�ſ�","");
						}
						if (7-offset == sOutWork) 
						{
							m_bytCheCiTmp = 0x0f; //����
							ShowAvTxt("��Ӫ��-����","");
						}

						if(++sOutWork>7-offset)sOutWork=0;
						
						_SetTimer(&g_objTimerMng, SET_CHECI_TIMER, 5000,G_TmSetCheCi);						
					}
				}
				break;

			case 0x04: // �������л�  C��
				{
					if((m_objRptSta.m_iPlayNo >= 0) && (m_objRptSta.m_iPlayNo < m_objStations.m_iStaTotal))
					{
						static int iturn = 0;//��Ϊ�������л�

						if (iturn++%2 == 0)
						{
							m_objRptSta.m_iPlayNo = 0;//���г�վ
						}
						else
						{
							int k;
							for(k=0;k<m_objStations.m_iStaTotal;k++)
							{
								if(!memcmp("����",m_objStations.m_aryStation[k].m_szName,4))
									break;									
							}
							m_objRptSta.m_iPlayNo = k;//���г�վ
						}				
					}

					ShowToAV(m_objRptSta.m_iPlayNo);
				}
				break;

			case 0x07: //�ز��ղŵĵ�վ/��վ��ʾ��0��
				{
					if(!m_bytEnbRptSta)  // ����Ӫ��̬
					{
						Show_diaodu("��Ӫ��״̬,ˡ����վ!");
						break;
					}

					if((m_objRptSta.m_iPlayNo >= 0) && (m_objRptSta.m_iPlayNo < m_objStations.m_iStaTotal))
					{
						m_objRptSta.m_bPlaySound = true;
					}
				}
				break;	

			case 0x08:	//��վ��#��
				{

				}
				break;	
				
			case 0x09:	//��վ��*��
				{

				}
				break;	

			case 0x0b:
				{

				}
				break;

			case 0x0a: // ��������ͷ
				{

				}
				break;

			//������·��
			case 0x21: 
				{
					/////////////////////////////////////////////////////////////
					char frm[20] = {0};	
					frm[0] = (char)0x01;	// 0x01��ʾҪ���͵����������ֱ�
					frm[1] = (char)0x8f;
					frm[2] = (char)0x81;
					frm[3] = (char)0x01;
					DataPush((char*)&frm, strlen(frm), DEV_QIAN, DEV_DIAODU, 2 );

					memset(frm,0,sizeof(frm));				
					memcpy(frm,szRcv+1,dwRcvLen - 1);

					ulong iStaNum = atol(frm+1);
					LoadStations(m_objStations,0,iStaNum); // ��������·
				}
				break;

			case 0x10: 
				{
					char frm[100] = {0};
					DWORD dwBegin = GetTickCount();
					Show_diaodu( "U�̼����..." );
					
					FILE* fp = NULL;
					do
					{
						fp = fopen( SPRINTF(buf, "%s/LineIno.wav", RPTSTATION_U_PATH), "rb" );
						if( fp ) break;

						if( !g_bProgExit )
						{
							sleep(1);
							Show_diaodu( "U�̼����..." );
						}
						else
						{
							break;
						}
					} while( GetTickCount() - dwBegin < 10000 );

					if( !fp )
					{
						Show_diaodu( "��·�ļ�����ʧ��" );
						goto IMPORT_WAV;
					}
					else
					{
						fclose( fp );
					}
					
					frm[0] = char(0x01);
					frm[1] = char(0x8f);
					frm[2] = char(0x90);
					frm[3] = 3;
					DataPush( frm, 4, DEV_QIAN, DEV_DIAODU, LV2 ); // ���ڵ���

					if( access(RPTSTATION_SAVE_PATH, F_OK) != 0 )
					{
						if( 0 == mkdir(RPTSTATION_SAVE_PATH, 777) )
						{
							chmod(RPTSTATION_SAVE_PATH, S_IRWXU|S_IRWXG|S_IRWXO);
						}
						else
						{
							Show_diaodu("������·�ļ�Ŀ¼ʧ��");
							break;
						}
					}

					int iResult = FileCopy( SPRINTF(buf, "%sLineIno.wav", RPTSTATION_U_PATH), SPRINTF(buf2, "%sLineIno.wav", RPTSTATION_SAVE_PATH) );
					frm[3] = iResult > 0 ? 1 : 2;
					DataPush( frm, 4, DEV_QIAN, DEV_DIAODU, 2 ); // ����ɹ�/ʧ��

					if( iResult > 0 )
					{
						Show_diaodu( "��·�ļ�����ɹ�" );
						sleep(1);

						tag2QRptStationCfg cfg;
						memset(&cfg,0,sizeof(cfg));
						GetSecCfg( &cfg, sizeof(cfg), offsetof(tagSecondCfg, m_uni2QRptStationCfg.m_obj2QRptStationCfg), sizeof(cfg) );
						LoadStations( m_objStations, true, cfg.m_iLineNo );

						// ���ñ���״̬
						bytareaNo = m_objStations.m_iStaTotal/2;
						m_objRptSta.m_iPlayNo = 0; 
					}
					else
					{
						Show_diaodu( "��·�ļ�����ʧ��" );
					}
					sleep(1);

IMPORT_WAV:

#if USE_PLAYSOUNDTYPE == 2		//��ʹ��¼����վ�����ʱҲ����¼���ļ�
					Show_diaodu("��ʼ���������ļ�������U��");

					iResult = DirCopyFilter( RPTSTATION_U_PATH, RPTSTATION_SAVE_PATH, ".wav", "LineIno.wav", G_InLinesTip );
					
					if( iResult == 1)
						Show_diaodu( "�����ļ�����ɹ�" );
					else
						Show_diaodu( "�����ļ�����ʧ��" );
#endif
					;
				}
				break;

			case 0x99: // ������·
				{
					int  iSelNo ;
					BYTE bytSelLen = BYTE( szRcv[1] );
					char szSelNo[16] = {0};
					memcpy(szSelNo, szRcv+2, bytSelLen);
					iSelNo = atoi(szSelNo);
					
					LoadStations( m_objStations, true, iSelNo );

					bytareaNo = m_objStations.m_iStaTotal/2;
					m_objRptSta.m_iPlayNo = 0;
				}
				break;

			case 0xf2://�����·��Ĺ�����·�ļ�����
				{
					BOOL blResult = FALSE;

					char szFileName[MAX_PATH] = {0};
					sprintf(szFileName, "/mnt/Flash/part3/Down/Line.dat");
					
					FILE *fp = NULL;
					fp = fopen(szFileName, "rb");
					if( !fp )
					{
						PRTMSG( MSG_ERR, "Can't find %s\n", szFileName );
						break;
					}
					fclose( fp );

					if( access(RPTSTATION_SAVE_PATH, F_OK) != 0 )
					{
						if( 0 == mkdir(RPTSTATION_SAVE_PATH, 777) )
						{
							chmod(RPTSTATION_SAVE_PATH, S_IRWXU|S_IRWXG|S_IRWXO);
						}
						else
						{
							Show_diaodu("������·�ļ�Ŀ¼ʧ��");
							break;
						}
					}

					int iResult = 0;
					SPRINTF(buf, "%s", szFileName);
					SPRINTF(buf2, "%s%s", RPTSTATION_SAVE_PATH, "LineIno.wav");

					// ɾ��ԭ����·�ļ�
					if( access(buf2, F_OK) == 0 )
					{
						unlink(buf2);
					}

#if USE_PLAYSOUNDTYPE == 1
					iResult = FileCopy( buf, buf2 );
#endif
#if USE_PLAYSOUNDTYPE == 2
					iResult = DecompressLineFile( buf );
#endif
					if( iResult )
					{
						Show_diaodu("������·�ļ����سɹ�");
						PRTMSG(MSG_DBG, "������·�ļ����سɹ�\n");
					}
					else
					{
						Show_diaodu("������·�ļ�����ʧ��");
						PRTMSG(MSG_DBG, "������·�ļ�����ʧ��\n");
					}
					
					if( iResult > 0 )
					{					
						tag2QRptStationCfg cfg;
						memset(&cfg,0,sizeof(cfg));
						GetSecCfg( &cfg, sizeof(cfg), offsetof(tagSecondCfg, m_uni2QRptStationCfg.m_obj2QRptStationCfg), sizeof(cfg) );

						LoadStations( m_objStations, true, cfg.m_iLineNo );

						bytareaNo = m_objStations.m_iStaTotal/2;
						m_objRptSta.m_iPlayNo = 0;
					}
				}
				break;
			default:
				break;
			}

_POPRCVDATA_NEXT:
			memset( szRcv, 0, sizeof(szRcv) );
			dwRcvLen = 0;
			break; // һ��Ҫ�˳�ѭ��
		}

		if( bLstRcvCmd )
			usleep(100000);
		else			
			usleep(1000000);		

		if (!m_bytEnbRptSta) 
		{
			continue;
		}
	
		GetCurGps(&gps, sizeof(gps), GPS_REAL);

		//��5�����ڶ�û�б�վ�������¿�ʼ����������վ��֮��ľ���
		static ulong t1 = 0;
		{
			if(0==t1)	t1 = GetTickCount();
			if((GetTickCount()-t1)>ulong(5*60*1000)) 
			{
				bytareaNo = m_objStations.m_iStaTotal/2;
				m_ucLstUpDown = 0xff;
				t1 = 0;
			}
		}

		char resbuf[1024] = {0};
		int  iBufLen;
		int  iRet;

		//���������վ��֮��ľ��룬����������aryStaDis�У�
		//Ϊ��ʡ��������ֻ���������ڵ�bytareaNo��վ��ľ���
		CalculateDis(gps.latitude,gps.longitude,aryStaDis,m_objRptSta.m_iPlayNo,int(bytareaNo));

		byte bytAccSta = 0;
		IOGet(IOS_ACC, &bytAccSta);

		if('A' == gps.valid && ( bytAccSta == IO_ACC_ON ) )
		{
			for(j = m_objRptSta.m_iPlayNo - int(bytareaNo); j < m_objRptSta.m_iPlayNo + int(bytareaNo); j++ ) // �������㣬��վ������Ϊ����ʱ���ܻ�©һվ��
			{
				if( j<0 )
					i = j + m_objStations.m_iStaTotal;
				else if( j>= m_objStations.m_iStaTotal)
					i = j - m_objStations.m_iStaTotal;
				else 
					i = j;

				// ���㷽��ǶȲ����ֵ
				float fDeltDir = gps.direction;
				if( fDeltDir < m_objStations.m_aryStation[i].m_fdirect ) fDeltDir += 360;
				fDeltDir -= m_objStations.m_aryStation[i].m_fdirect;
				if( fDeltDir > 180 ) fDeltDir = 360 - fDeltDir;

				//�ж��Ƿ�Ӹ÷�������վ��뾶���Ҹ�վ��δ��վ
				if( aryStaDis[i] <= m_objStations.m_aryStation[i].m_iRad	//�������˸õ�İ뾶֮��
					&& 
					fDeltDir <= 70 // �ҷ�����·���
					&&
					m_objStations.m_aryStation[i].m_bIsPlayed == false	//�ұ�վδ��վ��
					) 
				{
					// ���Ӷ�ͬһ��վ���վ��վ�ظ����Ĺ���
					if( m_objStations.m_aryStation[i].m_iCurNo != iLastStationNo)
					{
						m_objRptSta.m_iPlayNo = i;

						if( 
							// �����м�վ, �жϵ�ǰվ�Ƿ����ϴε�վ��ͬһ�������ϣ���ֹͣ��վ�㸽������GPSƯ�Ʊ��������վ
							(
								gps.speed<1.8 && 0xff != m_ucLstUpDown && 3 == GetStaAttr(m_objRptSta.m_iPlayNo) 
								&& m_ucLstUpDown != GetUpDownSta(m_objRptSta.m_iPlayNo) 
							)
							||
							(
								//�������վҪ�ٶȴ���һ������ֵ�ű�����ֹ��ͣ���յ�վ�ұ�
								(1 == GetStaAttr(m_objRptSta.m_iPlayNo) && gps.speed<1.2 )
							)

						  ) // �ж�ʲôʱ�����ϴ�����ȴ����վ (������Ư��)��֮����Ȼ���ڱ���վ�Ŀ���
						{
							static int sta_iLastSendNo = 0;

							m_objRptSta.m_bPlaySound = false;

							//hxd Ϊ������ͳ�Ƶ�ͷ��������Ȼ��������վ����������Ҫ�ϴ�������
							if (sta_iLastSendNo != m_objStations.m_aryStation[i].m_iCurNo) 
							{
								SendInOutData(m_objRptSta.m_iPlayNo,2);
							}
							sta_iLastSendNo = m_objStations.m_aryStation[i].m_iCurNo;
						}
						else
						{						
							iLastStationNo = m_objStations.m_aryStation[i].m_iCurNo;
							m_objRptSta.m_bPlaySound = true;
							m_objStations.m_aryStation[i].m_bIsPlayed = true;			
						}
					}
					else
					{
						PRTMSG( MSG_DBG, "sta have play\n" );
						m_objRptSta.m_bPlaySound = false;
					}
						
					if(!m_objRptSta.m_bPlaySound)
						continue;

					//��������ʾ��վ
					ShowToAV(m_objRptSta.m_iPlayNo);
					t1 = GetTickCount();
					bytareaNo = m_objStations.m_iStaTotal/2;
					PRTMSG( MSG_NOR, "Report station %d\n", i );

					// ������վ����Ϊ��δ��վ��
					for(int j=0; j < m_objStations.m_iStaTotal; j++ )
					{
						if(j != i)	m_objStations.m_aryStation[j].m_bIsPlayed = false;
					}

					//��ȡ��ǰ·�εĳ����ٶ�
					m_ucCurLimitSpd = GetStaLimitSpd(m_objRptSta.m_iPlayNo);

					//���͵���վ����
					tag4247 obj4247;
					time_t lCurrentTime;
					struct tm *pCurrentTime;
					time(&lCurrentTime);
					pCurrentTime = localtime(&lCurrentTime);
					obj4247.usLineNo =  m_objStations.m_iLineNo;
					obj4247.usStaNo  =  m_objStations.m_aryStation[i].m_iUniId;
					if(  !GetInOutSta(m_objRptSta.m_iPlayNo) )
					{
						obj4247.bytIsInOut = 0x00;//��վ
					}
					else
					{
						obj4247.bytIsInOut = 0x01;//��վ
					}
					obj4247.usYear   =  (WORD)(pCurrentTime->tm_year + 1900);
					obj4247.bytMon   =  (char)pCurrentTime->tm_mon + 1;
					obj4247.bytDay   =  (char)pCurrentTime->tm_mday;
					obj4247.bytHour  =  (char)pCurrentTime->tm_hour;
					obj4247.bytMin   =  (char)pCurrentTime->tm_min;
					obj4247.bytSec   =  (char)pCurrentTime->tm_sec;
					obj4247.dwMeterTotle  =  0;	//�ݲ���

					iRet = g_objSms.MakeSmsFrame((char*)&obj4247, sizeof(obj4247), 0x42, 0x47, resbuf, sizeof(resbuf), iBufLen, CVT_8TO7);
					if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);

					if ( !GetInOutSta(m_objRptSta.m_iPlayNo) ) // ����ǽ�վ��
					{// ��վ��վ�ж�Ԥ����
						m_dwInStaTm = GetTickCount();
						m_usCurZhiTm = m_objStations.m_aryStation[i].m_usZhiTm;
						m_usCurPaoTm = m_objStations.m_aryStation[i].m_usPaoTm;
						tempStationNo = GetUniStaId(i);  // ���ӶԱ���վԽվվ����ж�
					}
					else
					{
						tag4246 obj4246;
						int dwSpan;  
						m_dwOutStaTm = GetTickCount();
						dwSpan =(int) (m_dwOutStaTm - m_dwInStaTm)/1000 ;
						obj4246.usLineNo =  WORD(m_objStations.m_iLineNo);
						obj4246.usStaNo  =  WORD(m_objStations.m_aryStation[i].m_iCurNo);
						obj4246.bytYear  =  (WORD)(pCurrentTime->tm_year + 1900);
						obj4246.bytMon   =  (char)pCurrentTime->tm_mon + 1;
						obj4246.bytDay   =  (char)pCurrentTime->tm_mday;
						obj4246.bytHour  =  (char)pCurrentTime->tm_hour;
						obj4246.bytMin   =  (char)pCurrentTime->tm_min;
						obj4246.bytSec   =  (char)pCurrentTime->tm_sec;

						if(dwSpan >= 15360)	
						{
							dwSpan = 15360;
						}

						obj4246.bytTmspanMin = (char)(dwSpan/60);
						obj4246.bytTmspanSec = (char)(dwSpan%60);

						if ( (dwSpan > m_usCurZhiTm) && (tempStationNo == GetUniStaId(i))) // ͬһվ��ű�4246
						{
							obj4246.bytAlemType = 0; 

							iRet = g_objSms.MakeSmsFrame((char*)&obj4246, sizeof(obj4246), 0x42, 0x46, resbuf, sizeof(resbuf), iBufLen, CVT_8TO7);
							if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);
						}

						if ( (dwSpan < m_usCurPaoTm) && (tempStationNo == GetUniStaId(i))) 	 // ͬһվ��ű�4246				
						{
							obj4246.bytAlemType = 1;

							iRet = g_objSms.MakeSmsFrame((char*)&obj4246, sizeof(obj4246), 0x42, 0x46, resbuf, sizeof(resbuf), iBufLen, CVT_8TO7);
							if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);
						}
					}
					break;
				}
			}
		}

		bRet = true;
		
		//�Ƿ�Ҫ������������
		if( m_objRptSta.m_bPlaySound )
		{
			m_ucLstUpDown = GetUpDownSta(m_objRptSta.m_iPlayNo);

#if USE_PLAYSOUNDTYPE == 1	//ʹ��TTS������վ
			char szTip[300] = { 0 };
			szTip[0] = 1;
			szTip[1] = 1; // ��ʾ��������

			iLen = strlen(m_objStations.m_aryStation[m_objRptSta.m_iPlayNo].m_szPlay);
			memcpy(szTip+2, m_objStations.m_aryStation[m_objRptSta.m_iPlayNo].m_szPlay, iLen );
			iLen += 2;			
			DataPush( szTip, iLen, DEV_QIAN, DEV_DVR, LV2 );
#endif

#if USE_PLAYSOUNDTYPE == 2	//ʹ��¼���ļ���վ (��)
#endif
			m_objRptSta.m_bPlaySound = false;
		}
	}

}


/// ���������л�״̬�Ĵ���
void CAutoRptStationNew::P_TmSetCheCi()
{
	_KillTimer(&g_objTimerMng, SET_CHECI_TIMER);

	char buf[500] = { 0 };
	int iRet;
	int iSendLen;
	char sendBuf[512]={0};

	strcpy( buf, "����Ӫ��״̬�л�����ʧ��!");

	if( !g_objSock.IsOnline() )
	{
		goto _SETCHECI_END;
	}

	m_ucStaCnt++;

	buf[0] = m_ucStaCnt;
	
	//����: ��Ӫ�� 0x01 Ӫ�� 0x02
	if (m_bytEnbRptSta)
	{
		buf[1] = 2;//��ǰ�Ƿ�Ӫ��״̬����ô�л���Ӫ��״̬.
	}
	else
	{
		buf[1] = 1;
	}
	buf[2] = m_bytCheCiTmp;
	
	iRet = g_objSms.MakeSmsFrame(buf, 3 ,0x42,0x60,sendBuf,sizeof(sendBuf),iSendLen);
	if( !iRet ) 
	{
		g_objSock.SOCKSNDSMSDATA( sendBuf, iSendLen, LV12,0);
	}
	else 
	{
		goto _SETCHECI_END;
	}

	strcpy( buf, "����Ӫ��״̬�л�����ɹ�!" );

_SETCHECI_END:
	Show_diaodu( buf );
}

bool CAutoRptStationNew::LoadStations( tagStationsNew &v_objStations, bool v_bReBegin, int v_iSelNo )
{
	bool iRet = false;
	static WORD sta_wSelNo = 0xffff;
	char buf[200] = { 0 };
	tagStationsNew objStationsRead;

	if( v_bReBegin )	sta_wSelNo = 0xffff;

	memset( &m_objLinesHead, 0, sizeof(m_objLinesHead) );

	FILE* fp = fopen( SPRINTF(buf, "%sLineIno.wav", RPTSTATION_SAVE_PATH), "rb" );
	if( fp )
	{
		WORD w = 0;
		WORD wNextNo;
		WORD wCurNo = 0;

		if( 1 != fread( &m_objLinesHead, sizeof(m_objLinesHead), 1, fp ) )
		{
			Show_diaodu("��ȡ��·�ļ�ͷ����");
			goto LOADLINE_END;
		}
		if( m_objLinesHead.m_wLineTotal < 1 )
		{
			Show_diaodu("��·����Ϊ0");
			goto LOADLINE_END;
		}

		wNextNo = sta_wSelNo + 1;
		if( wNextNo >= m_objLinesHead.m_wLineTotal )
			wNextNo = 0;
		
		// ·�߱���
		for( w = 0; w < m_objLinesHead.m_wLineTotal; w ++ )
		{
			wCurNo = wNextNo;

			if( fseek(fp, sizeof(m_objLinesHead) + wNextNo * sizeof(objStationsRead), SEEK_SET) )
			{
				PRTMSG( MSG_NOR, "fseek %d line err!!!\n", wCurNo );
				Show_diaodu("�Ҳ�����վ��·");
				goto LOADLINE_END;
			}

			if( 1 != fread( (void*)&objStationsRead, sizeof(objStationsRead), 1, fp ) )
			{
				PRTMSG( MSG_NOR, "Loadsta read Line err\n");
				Show_diaodu("��ȡ��·����");
				goto LOADLINE_END;
			}

			//�������Ĳ�������0��ô��ѡ���һ��ʹ�ܵ��Ǹ�������Ҫ������Ĳ�������ѡ���.
			if( v_iSelNo > 0 )
			{
				if( v_iSelNo == objStationsRead.m_iLineNo )
				{
					char buf[20] = { 0 };
					buf[0] = 1;
					buf[1] = 0;
					buf[2] = char( v_iSelNo );
					char frm[100] = { 0 };
					int iFrmLen = 0;
					int iResult = g_objSms.MakeSmsFrame( buf, 3, 0x39, 0x43, frm, sizeof(frm), iFrmLen, CVT_8TO7 );
					if( !iResult )
					{
						iResult = g_objSock.SOCKSNDSMSDATA( frm, iFrmLen, LV9 );
					}

					break;
				}
			}
			else
			{
				if( objStationsRead.m_bytLineEnb )
					break;
			}

			wNextNo ++;
			if( wNextNo >= m_objLinesHead.m_wLineTotal )	wNextNo = 0;
		}

		if( w < m_objLinesHead.m_wLineTotal )
		{
			// վ�����
			for ( int i=0; i<objStationsRead.m_iStaTotal; i++ ) 
			{
				if(objStationsRead.m_aryStation[i].m_bytrecordfile > (sizeof(objStationsRead.m_aryStation[i].m_szrecord) / sizeof(objStationsRead.m_aryStation[i].m_szrecord[0])) )
				{
					Show_diaodu("��·�ļ�����,�����ļ�̫��.");
					goto LOADLINE_END;
				}
			}
			v_objStations = objStationsRead;
			sta_wSelNo = wCurNo;

			PRTMSG(MSG_NOR, "�����Զ���վ��·:%d·\n",objStationsRead.m_iLineNo );

			char str[100] = {0};
			sprintf(str, "������·--%d", objStationsRead.m_iLineNo );
			Show_diaodu( str );

			//�ѵ�ǰʹ�õ���·�洢������������
			tag2QRptStationCfg cfg;
			GetSecCfg( &cfg, sizeof(cfg), offsetof(tagSecondCfg, m_uni2QRptStationCfg.m_obj2QRptStationCfg), sizeof(cfg) );
			cfg.m_iLineNo = objStationsRead.m_iLineNo;
			SetSecCfg(&cfg, offsetof(tagSecondCfg, m_uni2QRptStationCfg.m_obj2QRptStationCfg), sizeof(cfg) );

            iRet = true;
		}
		else
		{
			PRTMSG(MSG_NOR, "������·ʧ��,������ѡ����·\n");
			Show_diaodu("������·ʧ�ܣ�������ѡ����·");
			iRet = false; 
		}
	}

	
LOADLINE_END:
	if(fp)
	{
		fclose( fp );
	}
	else
	{
		PRTMSG(MSG_NOR, "Open Line file failed!\n");
	}

	return iRet;
}

//------------------------------------------------------------------------------
// �ڵ���������ʾ��ʾ��Ϣ
// fmt,..: ��ʽ������
void CAutoRptStationNew::Show_diaodu(char *v_szbuf)
{
	char str[1024] = {0x01, 0x72, 0x01};
	memcpy(str+3, v_szbuf, strlen(v_szbuf));
	DataPush(str, strlen(v_szbuf)+3, DEV_QIAN, DEV_DIAODU, LV2);
}


bool CAutoRptStationNew::CalculateDis(double latitude, double longitude, double *aryStaDis, int curNo, int iAreaNum)
{
	int i=0,j=0;
	double dDis = 0;
	if( iAreaNum < 1 ) return false;

	for( i=curNo - iAreaNum; i <= curNo + iAreaNum; i++)
	{// ����ôѭ��������Ϊż��ʱ�������һ��

		if( i<0 ) 
			j = i + m_objStations.m_iStaTotal;
		else if( i>= m_objStations.m_iStaTotal)
			j = i - m_objStations.m_iStaTotal;
		else 
			j = i;

		if( j >= 0 && j < m_objStations.m_iStaTotal )
		{
			dDis = G_CalGeoDis2( longitude, latitude, m_objStations.m_aryStation[j].m_lLon / LONLAT_DO_TO_L, m_objStations.m_aryStation[j].m_lLat / LONLAT_DO_TO_L );
			aryStaDis[j] = dDis;
		}
	}
	return true;
}

// ��ѹ����Ĺ�����·�ļ������������ļ���
int CAutoRptStationNew::DecompressLineFile(char *filename)
{
	FILE *fp = NULL;
	FILE *fp_write = NULL;
	fp = fopen(filename, "rb+");
	if(NULL != fp)
	{
		mkdir( RPTSTATION_SAVE_PATH, 0777 );

		int ifilecount = 0, ifilesize = 0;
		char szFilename[15];
		char szwritefilename[100];
		char *data_buf;

		fread(&ifilecount,4,1,fp);	//��ȡ�ļ�����

		for( int i=0; i < ifilecount; i++)
		{
			fread(szFilename,15,1,fp);	//��ȡ�ļ���
			fread(&ifilesize,4,1,fp);	//��ȡ�ļ���С

			data_buf = (char*)malloc(ifilesize);	
			fread(data_buf,ifilesize,1,fp);	//��ȡ�ļ�����

			sprintf( szwritefilename, "%s%s", RPTSTATION_SAVE_PATH, szFilename );
			fp_write = fopen(szwritefilename,"wb+"); //�򿪣��½����ļ�	

			if( fp_write )
			{
				fwrite(data_buf,ifilesize,1,fp_write);	//д���ļ�
				fclose(fp_write);
				fp_write = NULL;
			}
		}
		fclose(fp);
		return 1;
	}
	return 0;
}

// ��ʾ2����Ϣ�����С����У�
bool CAutoRptStationNew::ShowAvTxt( char* v_szUp,char *v_szDown)
{
	tag8ff1 frm;
	int iUpLen = 0,iDownLen = 0;

	if (!v_szUp||!v_szDown)
	{
		return false;
	}

	iUpLen = byte(strlen(v_szUp));
	iDownLen = byte(strlen(v_szDown));

	if ((iUpLen+iDownLen)>sizeof(frm.m_szContent) - 4 )
	{
		return false;
	}

	frm.m_szContent[0] = iUpLen;
	memcpy(frm.m_szContent+1,v_szUp,iUpLen);
	frm.m_szContent[iUpLen+1] = iDownLen;
	memcpy(frm.m_szContent+iUpLen+2,v_szDown,iDownLen);

	char buf[100] = {0};
	buf[0] = 0x01;	// 0x01��ʾ���͵�������
	memcpy(buf+1, (void*)&frm, iUpLen+iDownLen+4);
	DataPush( (char*)&frm, iUpLen+iDownLen+4+1, DEV_QIAN, DEV_DIAODU, LV2 );

	return true;
}

int CAutoRptStationNew::DealComu8e(char* v_szBuf, DWORD v_dwBufLen )
{
	DWORD dwPacketNum = 0;
	return m_objRcvDataMng.PushData( LV8, v_dwBufLen, v_szBuf, dwPacketNum );
}

//ɾ����·�ļ������е������ļ�
// BOOL CAutoRptStationNew::DeleteDirectory(const CString szDir)
// {
// 	CString szFindDir=szDir;
// 	if(szFindDir.Right(1)!="\\"){szFindDir+="\\"; }
// 	szFindDir+="*.*";
// 
// 	WIN32_FIND_DATA fd;
// 	HANDLE hFind;
// 	hFind=FindFirstFile(szFindDir,&fd);
// 	if(hFind!=INVALID_HANDLE_VALUE)
// 	{
// 		do{
// 			if(fd.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
// 			{ 
// 				DeleteDirectory(szDir+"\\"+fd.cFileName); //it must be directory,get into
// 			}else{ 
// 				if(DeleteFile(szDir+"\\"+fd.cFileName)==FALSE)
// 				{
// 					g_objDiaodu.show_diaodu("ɾ����·�ļ�ʧ��");
// 					return FALSE;
// 				}//it is file ,delete it
// 			}
// 		}while(FindNextFile(hFind,&fd));
// 	}
// 	//if you donot close the handle,the next step of Removing Directory would failed
// 	CloseHandle(hFind);
// 	//the root directory must be empty ,so remove it
// 	if(RemoveDirectory(szDir)==FALSE)
// 	{	
// 		g_objDiaodu.show_diaodu("ɾ����·Ŀ¼ʧ��");
// 		return FALSE;
// 	}
// 	return TRUE;
// }


// ��ʾ��վ״̬
bool CAutoRptStationNew::ShowToAV( DWORD PlayNo )
{
	tag8ff1 frm;
	char	*pSymbol = NULL;
	char	name[20] ={0};
	char	*pName = NULL;
	int		iNLen = 0;
	char	szTemp[80] = { 0 };
	BYTE	bytLen = 2;

	pSymbol = strchr( m_objStations.m_aryStation[PlayNo].m_szName, '-' );

	if(pSymbol==NULL)
	{
		return false;
	}

	pSymbol +=1;
	pName = pSymbol;
	pSymbol = strchr( pName, '-' );
	if(pSymbol==NULL)
	{
		return false;
	}

	iNLen = pSymbol - pName;
	memcpy(name,pName,iNLen);

	if( !memcmp("��վ",pSymbol+1,4) )
	{
		sprintf(szTemp,"����:%s",name);
		frm.m_szContent[0] = BYTE(strlen(szTemp));
		strcpy(frm.m_szContent+1,szTemp);
		bytLen += 1 + BYTE( strlen(szTemp) );
		pSymbol = strchr( m_objStations.m_aryStation[PlayNo-1].m_szName, '-' );
		if(pSymbol==NULL)
		{
			return false;
		}
		pSymbol +=1;
		pName = pSymbol;
		pSymbol = strchr( pName, '-' );
		if(pSymbol==NULL)
		{
			return false;
		}
		iNLen = pSymbol - pName;
		memset(name,0,sizeof(name));
		memcpy(name,pName,iNLen);
		sprintf(szTemp,"��վ:%s",name);
		frm.m_szContent[ strlen(frm.m_szContent) ] = BYTE( strlen(szTemp) );
		strcat( frm.m_szContent, szTemp );
		bytLen += 1 + BYTE( strlen(szTemp) );
	}
	else
	{
		sprintf(szTemp,"�뿪:%s",name);
		frm.m_szContent[0] = BYTE(strlen(szTemp));
		strcpy(frm.m_szContent+1,szTemp);
		bytLen += 1 + BYTE( strlen(szTemp) );
		pSymbol = strchr( m_objStations.m_aryStation[PlayNo+1].m_szName, '-' );
		if(pSymbol==NULL)
		{
			return false;
		}
		pSymbol +=1;
		pName = pSymbol;
		pSymbol = strchr( pName, '-' );
		if(pSymbol==NULL)
		{
			return false;
		}
		iNLen = pSymbol - pName;
		memset(name,0,sizeof(name));
		memcpy(name,pName,iNLen);
		sprintf(szTemp,"��վ:%s",name);
		frm.m_szContent[ strlen(frm.m_szContent) ] = BYTE( strlen(szTemp) );
		strcat( frm.m_szContent, szTemp );
		bytLen += 1 + BYTE( strlen(szTemp) );
	}

	char buf[100] = {0};
	buf[0] = 0x01;
	memcpy(buf+1, (void*)&frm, bytLen);
	DataPush( buf, bytLen+1, DEV_QIAN, DEV_DIAODU, 2 );

	return true;
}

void CAutoRptStationNew::SendInOutData(int v_iSta, byte v_bytSym)
{
	//���͵���վ����
	int iRet;
	int iBufLen;
	byte dataType;
	char resbuf[2048];
	tag4247 obj4247;
	
	dataType = 0x47;
	if (1 == v_bytSym) 	dataType = 0x47;
	if (2 == v_bytSym)  dataType = 0x74;

	struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);

	obj4247.usLineNo =  m_objStations.m_iLineNo;
	obj4247.usStaNo  =  m_objStations.m_aryStation[v_iSta].m_iUniId;

	if(  !GetInOutSta(v_iSta) )
	{
		obj4247.bytIsInOut = 0x00;//��վ
	}
	else
	{
		obj4247.bytIsInOut = 0x01;//��վ
	}

	obj4247.usYear   =  (WORD)(pCurrentTime.tm_year);
	obj4247.bytMon   =  (char)pCurrentTime.tm_mon + 1;
	obj4247.bytDay   =  (char)pCurrentTime.tm_mday;
	obj4247.bytHour  =  (char)pCurrentTime.tm_hour;
	obj4247.bytMin   =  (char)pCurrentTime.tm_min;
	obj4247.bytSec   =  (char)pCurrentTime.tm_sec;
	obj4247.dwMeterTotle  =  0; // �ݲ����

	iRet = g_objSms.MakeSmsFrame((char*)&obj4247, sizeof(obj4247), 0x42, dataType, resbuf, sizeof(resbuf), iBufLen, CVT_8TO7);
	if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);	
	
}

 // ͣס�������� ������
// void CAutoRptStationNew::StopPlayCurSta()
// {
// 	PlaySound( NULL, NULL, 0 );	
// 	byte cmd[10] = {0};
// 	cmd[0]=0x17;
// 	PushDataToPhone(SRC_QIAN,3,1,(char*)cmd);
// }

ushort CAutoRptStationNew::GetLineId()
{
	return m_objStations.m_iLineNo;
}

BYTE CAutoRptStationNew::GetCheCi()
{
	return m_bytCheCi;
}

byte CAutoRptStationNew::GetCurLimitSpd()
{
	return m_ucCurLimitSpd;
}

bool CAutoRptStationNew::CenterLoadSta(ushort v_usLine)
{
	v_usLine = htons(v_usLine);
	PRTMSG( MSG_NOR, "CenterLoadSta %d\n",v_usLine);

	return LoadStations( m_objStations, true, v_usLine );
}

BYTE CAutoRptStationNew::GetUpDownSta2()
{
	if (0x00 == m_ucLstUpDown) return 0x02;
	if (0x01 == m_ucLstUpDown) return 0x01;
	if (0xff == m_ucLstUpDown) return 0x00;
	return 0;
}

void CAutoRptStationNew::SetWorkSta()
{
	static int sWork = 0; //Ӫ�˷�Ӫ���л�
	if ( !(sWork++%2) )
	{
		m_bytEnbRptSta = false;
		ShowAvTxt("��Ӫ��","����");
	}
	else
	{
		m_bytEnbRptSta = true;
		ShowAvTxt("Ӫ��","");
	}

	//�ո��л���������������δ֪��Ū�������Ϳ�����.
	m_bytCheCiTmp = 0x0f ;//����
	_SetTimer(&g_objTimerMng, SET_CHECI_TIMER, 5000,G_TmSetCheCi);	
}

int CAutoRptStationNew::GetUniStaId(int v_iCurId)
{
	return m_objStations.m_aryStation[v_iCurId].m_iUniId;
}

byte CAutoRptStationNew::GetAutoRptSta()
{
	return m_bytEnbRptSta;
}

void CAutoRptStationNew::SetAutoRptSta(byte m_bytSet)
{
	tag2QRptStationCfg cfg;
	GetSecCfg( &cfg, sizeof(cfg), offsetof(tagSecondCfg, m_uni2QRptStationCfg.m_obj2QRptStationCfg), sizeof(cfg) );
	cfg.m_bytRptSta = m_bytSet;
	SetSecCfg( &cfg, offsetof(tagSecondCfg, m_uni2QRptStationCfg.m_obj2QRptStationCfg), sizeof(cfg) );

	PRTMSG( MSG_NOR, "m_bytEnbRptSta=%d",m_bytSet);
}
	
//��������ĳ����·
void CAutoRptStationNew::hdl_4216(byte* buf, int len)
{	
	tag4216 *obj4216;
	obj4216 = (tag4216*)(buf);

	tag4256 obj4256;
	obj4256.m_usLindId = obj4216->m_usLindId;

	if(CenterLoadSta(obj4216->m_usLindId))
	{
		obj4256.m_bytRes = 1;
	}
	else
	{
		obj4256.m_bytRes = 0;
	}

	int iBufLen, iRet;
	char resbuf[1024] ={0};	

	iRet = g_objSms.MakeSmsFrame((char*)&obj4256, sizeof(obj4256), 0x42, 0x56, resbuf, sizeof(resbuf), iBufLen);
	if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);
}
	
//����������վ���ܣ��ó�̨�Ƿ���������վ����
void CAutoRptStationNew::hdl_4217(byte* buf, int len)	
{
	if (!buf)
		return;

	m_bytEnbRptSta = buf[0]-1;
	m_bytCheCi = buf[1];
	SetAutoRptSta(m_bytEnbRptSta);

	if(m_bytEnbRptSta)
	{
		ShowAvTxt("Ӫ��","");
		if(1== m_bytCheCi)	ShowAvTxt("����-�ϰ�","");
		if(2== m_bytCheCi)	ShowAvTxt("�س�-�°�","");
	}
	else
	{
		if(1== m_bytCheCi)	ShowAvTxt("����-�ϰ�","");
		if(2== m_bytCheCi)	ShowAvTxt("�س�-�°�","");
		if(3== m_bytCheCi)	ShowAvTxt("��Ӫ��-����","");
		if(4== m_bytCheCi)	ShowAvTxt("��Ӫ��-����","");
		if(5== m_bytCheCi)	ShowAvTxt("��Ӫ��-�޳�","");
		if(6== m_bytCheCi)	ShowAvTxt("��Ӫ��-����","");
		if(7== m_bytCheCi)	ShowAvTxt("��Ӫ��-�ſ�","");
		if(0x0f == m_bytCheCi)ShowAvTxt("��Ӫ��-����","");
	}

	char res = 1;
	int iBufLen, iRet;
	char resbuf[1024] ={0};

	tagGPSData objGps(0);
	g_objMonAlert.GetCurGPS( objGps, true );
	Snd4254(objGps);

	iRet = g_objSms.MakeSmsFrame(&res, sizeof(res), 0x42, 0x57, resbuf, sizeof(resbuf), iBufLen);
	if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);
}

//Ӧ����(1) + ��ţ�1��+���ͣ�1�� �� [ ��Ӫ�����ͣ�1��]
void CAutoRptStationNew::hdl_4220(byte* buf, int len)	
{
	if (!buf) return;

	if (!buf[0])
	{
		Show_diaodu("���ľܾ������Ӫ״̬�л�����");
	}

	if (m_ucStaCnt != buf[1]) 
	{
		Show_diaodu("�Ǳ�����Ӫ״̬�л�����");
		return;
	}

	if (2 == buf[2]) 
		m_bytEnbRptSta = true; // ������վ

	if (1 == buf[2])
	{
		m_bytCheCi = buf[3];
		m_bytEnbRptSta = false;
	}

	if(m_bytEnbRptSta)
	{
		ShowAvTxt("Ӫ��","");
		if(1== m_bytCheCi)	ShowAvTxt("����-�ϰ�","");
		if(2== m_bytCheCi)	ShowAvTxt("�س�-�°�","");
	}
	else
	{
		if(1== m_bytCheCi)	ShowAvTxt("����-�ϰ�","");
		if(2== m_bytCheCi)	ShowAvTxt("�س�-�°�","");
		if(3== m_bytCheCi)	ShowAvTxt("��Ӫ��-����","");
		if(4== m_bytCheCi)	ShowAvTxt("��Ӫ��-����","");
		if(5== m_bytCheCi)	ShowAvTxt("��Ӫ��-�޳�","");
		if(6== m_bytCheCi)	ShowAvTxt("��Ӫ��-����","");
		if(7== m_bytCheCi)	ShowAvTxt("��Ӫ��-�ſ�","");
		if(0x0f == m_bytCheCi)ShowAvTxt("��Ӫ��-����","");
	}

	tagGPSData objGps(0);
	g_objMonAlert.GetCurGPS( objGps, true );
	Snd4254(objGps);
}

//��������һ���������绰�ĺ��롣
void CAutoRptStationNew::hdl_4219(byte* buf, int len)	
{
// 	if (!buf)return;
// 
// 	PRTMSG( MSG_DBG, "hdl_4219 cnt=%d,dataLen=%d\n",buf[0],len);
// 
// 	char telTmp[20] = {0};
// 	int TelCnt = buf[0];
// 	char res = 1;
// 	int iBufLen, iRet;
// 	char resbuf[1024] ={0};
// 
// 	if (len<TelCnt*20+1)
// 	{
// 		PRTMSG( MSG_ERR, "hdl_4219 data len err\n" );
// 		return;
// 	}
// 
// 	tag2QMDailCfg cfg;
// 	::GetSecCfg( &cfg, sizeof(cfg),offsetof(tagSecondCfg, m_uni2QMDailCfg.m_obj2QMDailCfg), sizeof(cfg) );
// 
// 	if (TelCnt>sizeof(cfg.m_szMultiDailTel)/sizeof(cfg.m_szMultiDailTel[0])){
// 		if (g_pView) g_pView->DbgShowStr("hdl_4219 Num Cnt err.");return;
// 	}
// 
// 	memset(&cfg,0,sizeof(cfg));
// 	for(int i = 0;i<TelCnt; i++)
// 	{
// 		memset(telTmp,0,sizeof(telTmp));
// 		memcpy(telTmp, buf+1+i*20, sizeof(telTmp));
// 		char *pos = strchr(telTmp,0x20);
// 		*pos = 0;
// 		if (g_pView) g_pView->DbgShowStr("hdl_4219 telTmp=%s",telTmp);
// 		memcpy(cfg.m_szMultiDailTel[i],telTmp,min(sizeof(cfg.m_szMultiDailTel[0]),sizeof(telTmp)) );
// 	}
// 
// 	if(::SetSecCfg( &cfg,offsetof(tagSecondCfg, m_uni2QMDailCfg.m_obj2QMDailCfg), sizeof(cfg) )) {
// 		res = 2;
// 	}
// 
// 	iRet = g_objSms.MakeSmsFrame(&res, sizeof(res), 0x42, 0x59, resbuf, sizeof(resbuf), iBufLen,NULL);
// 	if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);
}

//���Ĳ�ѯ��̨��·���
void CAutoRptStationNew::hdl_4218()	
{
	int iBufLen, iRet;
	char resbuf[1024] ={0};
	int i;
	tagStationsNew objStaRead;
	byte res[512];

	//�ɹ���־(1)+��·�汾��(16) + ��ǰ������·ID(2) + ��·����(1)+��������·ID����2*N��
	res[0] = 0x01;//�ɹ�

	for(i=0;i<sizeof(m_objLinesHead.ver);i++)
	{
		res[i+1]=0x20;
	}

	memcpy(res+1,m_objLinesHead.ver,strlen(m_objLinesHead.ver));

	res[17] = GetLineId()/256;
	res[18] = GetLineId()%256;
	res[19] = (byte)m_objLinesHead.m_wLineTotal;

	char buf[200] = { 0 };
	FILE* fp = fopen( SPRINTF(buf, "%sLineIno.wav", RPTSTATION_SAVE_PATH), "rb" );
	if (!fp)
	{	
		PRTMSG( MSG_ERR, "hdl_4218 fopen err\n");
		goto LOADLINE_END;
	}

	for( i = 0; i < m_objLinesHead.m_wLineTotal; i ++ )
	{
		if( fseek(fp, sizeof(m_objLinesHead) + i * sizeof(objStaRead), SEEK_SET) )
		{
			PRTMSG( MSG_ERR, "fseek %d line err!!!\n",i);
			goto LOADLINE_END;
		}

		if( 1 != fread( &objStaRead, sizeof(objStaRead), 1, fp ) )
		{
			PRTMSG( MSG_ERR, "Loadsta read Line err!!!\n");
			goto LOADLINE_END;
		}
		res[20+2*i]   = (ushort)(objStaRead.m_iLineNo)/256;
		res[20+2*i+1] = (ushort)(objStaRead.m_iLineNo)%256;
	}	

	if(fp)
		fclose( fp );
	
	memset(resbuf,0,sizeof(resbuf));
	iRet = g_objSms.MakeSmsFrame((char*)res, 20+2*m_objLinesHead.m_wLineTotal, 0x42, 0x58, resbuf, sizeof(resbuf), iBufLen);
	if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);	

	return;

LOADLINE_END:
	if(fp)
		fclose( fp );
	
	memset(res,0,sizeof(res));
	res[0] = 0x7f;
	iRet = g_objSms.MakeSmsFrame((char*)res, 1, 0x42, 0x58, resbuf, sizeof(resbuf), iBufLen);
	if( !iRet ) g_objSock.SOCKSNDSMSDATA( resbuf, iBufLen, LV12,0);	

	return;
}

int CAutoRptStationNew::Snd4254( const tagGPSData &v_objGps )
{
	tag4254 objSnd;
	tagQianGps objQianGps;
	char buf[ 500 ] = { 0 };
	int iBufLen = 0;
	int iRet = 0;
	BYTE bytTransType;  //ҵ������
	BYTE bytDataType;   //��������
	
	// �������б���״̬,���ɱ�������
	g_objMonAlert.FillAlertSymb( g_objMonAlert.GetAlertSta(), &objSnd.m_bytAlertHi, &objSnd.m_bytAlertLo );
	
	objSnd.m_bytMonCentCount = 1; // ���ĸ�������1
	g_objMonAlert.GpsToQianGps( v_objGps, objQianGps );
	memcpy( &(objSnd.m_objQianGps), &objQianGps, sizeof(objSnd.m_objQianGps) );
	
	objSnd.m_usLineId  = htons(GetLineId());//�����·��û�м��سɹ��Ļ���ô����0.
	//״̬�֣�A7 A6 A5 A4 A3 A2 A1 A0
	//A1A0: 0x00δ֪ 0x01 ���� 0x02 ����
	//A2: �Ƿ���Ӫ��״̬
	objSnd.m_bytBusSta  =  GetUpDownSta2();
	objSnd.m_bytBusSta |= (GetAutoRptSta() << 2);//״̬��
	
	//��������(1):0x01: ����  0x02: �س� 0x03: ���� 0x04 ����,0x05: �޳�  0x06: ���� 0x07: �ſ� 0x0f ����
	objSnd.m_bytCheCi = GetCheCi();	//��������
	
	objSnd.m_dwMeter = htonl(g_objDriveRecord.GetTotleMeter()); //�����
	
	//objSnd.m_bytSatInUse = v_objGps.m_bytSateInUseCnt;
	objSnd.m_bytSatInUse = 4;

	objSnd.m_bytHDOP     = v_objGps.m_bytHDop;
	
	//objSnd.m_ulDriverId  = htonl(g_objDriveRecord.GetDrvNum());
	objSnd.m_ulDriverId = 0;
	
	PRTMSG(MSG_DBG, "S%02d:%02d:%02d,Sta=%02X,HL=%02X%02X,BUS=%02x/%02x,gps=%02x%02x\n",  
		objQianGps.m_bytHour+8,objQianGps.m_bytMin,objQianGps.m_bytSec,
		objQianGps.m_bytMix, objSnd.m_bytAlertHi, objSnd.m_bytAlertLo,
		objSnd.m_bytBusSta,  objSnd.m_bytCheCi,
		objSnd.m_bytSatInUse,objSnd.m_bytHDOP);
	
	bytTransType = 0x42;
	bytDataType  = 0x54;
	
	iRet = g_objSms.MakeSmsFrame((char*)&objSnd, sizeof(objSnd), bytTransType, bytDataType, buf, sizeof(buf), iBufLen);
	if( iRet ) return iRet;

	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9, DATASYMB_0154 );
	if( iRet ) return iRet;
}

void CAutoRptStationNew::ShowLineVer()
{
	int i;
	tagStationsNew objStaRead;

	char buf[200] = { 0 };
	FILE* fp = fopen( SPRINTF(buf, "%sLineIno.wav", RPTSTATION_SAVE_PATH), "rb" );
	
	if (!fp)
	{
		Show_diaodu("��·�ļ�������");
		return;
	}

	char szVer[100] = {0};
	char szVar[16]  = {0};
	sprintf(szVer,"�汾��:%s,����(%d):",m_objLinesHead.ver,m_objLinesHead.m_wLineTotal);

	for( i = 0; i < m_objLinesHead.m_wLineTotal; i ++ )
	{
		if( fseek(fp, sizeof(m_objLinesHead) + i * sizeof(objStaRead), SEEK_SET) )
		{
			PRTMSG(MSG_ERR, "fseek %d line err!\n",i);
			Show_diaodu("�Ҳ�����·");
			goto LOADLINE_END;
		}

		if( 1 != fread( &objStaRead, sizeof(objStaRead), 1, fp ) )
		{
			PRTMSG(MSG_ERR, "Loadsta read Line err!\n");
			Show_diaodu("��ȡ��·����");
			goto LOADLINE_END;
		}

		memset(szVar,0,sizeof(szVar));
		sprintf(szVar,"%d ",objStaRead.m_iLineNo);
		strcat(szVer,szVar);
	}

	Show_diaodu(szVer);

LOADLINE_END:
	if(fp)fclose( fp );
}

void CAutoRptStationNew::ShowLineAV()
{
	tag2QRptStationCfg cfg;
	GetSecCfg( &cfg, sizeof(cfg), offsetof(tagSecondCfg, m_uni2QRptStationCfg.m_obj2QRptStationCfg), sizeof(cfg) );
	
	char str[100] = {0};
	sprintf(str,"��·:%d(Cfg)/%ld", cfg.m_iLineNo, m_objStations.m_iLineNo);
	Show_diaodu(str);	
}


byte CAutoRptStationNew::GetUpDownSta(int v_iCurId)
{
	if(!memcmp("����",m_objStations.m_aryStation[v_iCurId].m_szName,4))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


//��ȡ����վ��״̬, 0: ��վ�� 1����վ
bool CAutoRptStationNew::GetInOutSta(int v_iCurId)
{
	if(strstr(m_objStations.m_aryStation[v_iCurId].m_szName,"��վ"))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

byte CAutoRptStationNew::GetStaLimitSpd(int v_iCurId)
{
	return m_objStations.m_aryStation[v_iCurId].m_bytSpeed;
}

//��ȡվ�������:ʼ��վ/�յ�վ/�м�վ/����/����/���
byte CAutoRptStationNew::GetStaAttr(int v_iCurId)
{
	return m_objStations.m_aryStation[v_iCurId].m_bytStaAttr;
}

#endif


