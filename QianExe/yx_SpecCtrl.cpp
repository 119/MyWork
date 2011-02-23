// SpecCtrl.cpp: implementation of the CSpecCtrl class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
//#include "qianexe.h"
#include "yx_QianStdAfx.h"
#include "yx_SpecCtrl.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-SpecCtrl	")
 
#if USE_LANGUAGE == 0
#define LANG_OILTOCUT ("�����ȫ�ж���·,�뿿��ͣ��,��ע�ⰲȫ!")
#define LANG_OILCUT_SUCC ("��·�ѱ��ж�!!")
#define LANG_OILCONN_SUCC ("��·�ѽ�ͨ����")
#endif

#if USE_LANGUAGE == 11
#define LANG_OILTOCUT (" Secs Later Cut Oil Supply, Stop and Keep Safe!")
#define LANG_OILCUT_SUCC ("Oil Supply Cutted !!")
#define LANG_OILCONN_SUCC ("Oil Supply Resumed !!")
#endif


void *G_CloseOilElec(void *arg)
{
	g_objSpecCtrl.DisconnOilElec();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpecCtrl::CSpecCtrl()
{
 	m_hThdCtrl = 0;
 	m_bThdCtrlExit = false;

// 	m_hThdCtrl = NULL;
// 	m_hEvtThdCtrlExit = ::CreateEvent( NULL, TRUE, TRUE, NULL ); // �ֹ���λ(����),��ʼΪ�ź�̬
}

CSpecCtrl::~CSpecCtrl()
{
	// ��Ҫ���������Release;
}

void CSpecCtrl::Init()
{
#if USE_DISCONNOILELEC == 1
	unsigned char ucResult = 0;
	if( !IOGet(IOS_ACC, &ucResult) )
	{
		AccChg( IO_ACC_ON == ucResult );
	}	
#endif
}

void CSpecCtrl::Release()
{
	// ֪ͨ���߳��˳�
	m_bThdCtrlExit = true;
	usleep(100000); // Ϊ������ģ�鷱æ�����߳������˳������߳�sleep�ȴ�һ�½Ϻ�

	// �رն��Ͷϵ��߳�
	if( m_hThdCtrl )
	{
		void* pThdRet;
		int iResult = 0;
		
		DWORD dwCt = 0;
		while( 1 )
		{
			iResult = pthread_kill( m_hThdCtrl, 0);
			if( ESRCH == iResult || EINVAL == iResult )
			{
				break;
			}
			else
			{
				if( dwCt ++ >= 60 ) break;
				usleep( 50000 );
			}
		}
		
		m_hThdCtrl = 0;
	}

// 	// ������Դ
// 	if( m_hEvtThdCtrlExit )
// 	{
// 		::CloseHandle( m_hEvtThdCtrlExit );
// 		m_hEvtThdCtrlExit = NULL;
// 	}
}

/// Զ��ң������ (ҵ��������)
int CSpecCtrl::Deal0601( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen )
{
	char buf[100] = { 0 };
	int iBufLen = 0;
	int iRet = 0;
	BYTE bytResType = 0;
	bool bOilCut = false;
	unsigned char ucResult = 0;

	// �������
	if( 2 != v_dwDataLen )
	{
		iRet = ERR_PAR;
		goto DEAL0601_FAILED;
	}

#if USE_DISCONNOILELEC == 1

	// Ԥ���ɳɹ�Ӧ��֡
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x06, 0x41, buf, sizeof(buf), iBufLen);
	if( iRet ) goto DEAL0601_FAILED;

	// �Ϻ��͵紦��
	if( !(0x40 & v_szData[0]) ) // ��Ҫ�����͵����
	{
		if( !IOGet(IOS_ACC, &ucResult) )
		{
			if( IO_ACC_ON == ucResult)
			{
				if( 0x40 & v_szData[1] ) // ��Ҫ�����ж�
				{
					PRTMSG( MSG_NOR, "Deal0601 - To Disconnect Oil Task\n" );

					if( !m_hThdCtrl || (m_hThdCtrl && ESRCH == pthread_kill(m_hThdCtrl, 0)) ) // ���ر��͵��߳�û������
					{
// 						// �ر��ϴ��߳̾�� (�������)
// 						if( m_hThdCtrl ) ::CloseHandle( m_hThdCtrl );

						// �����߳�����
						int iResult = 0;
						pthread_attr_t objThreadAttr;
						iResult = pthread_attr_init( &objThreadAttr );
						if( iResult )
						{
							PRTMSG( MSG_ERR, "Deal0601 - pthread_attr_init fail %d\n", iResult );
							iRet = ERR_THREAD;
							goto DEAL0601_FAILED;
						}
 						iResult = pthread_attr_setdetachstate( &objThreadAttr, PTHREAD_CREATE_DETACHED ); // ����Ϊ�����߳�
 						if( iResult )
 						{
							PRTMSG( MSG_ERR, "Deal0601 - pthread_setdetachstate fail %d\n", iResult );
							iRet = ERR_THREAD;
							goto DEAL0601_FAILED;
						}

						// ����߳��˳���־
						m_bThdCtrlExit = false;

						// �ؽ����߳�
						iResult = pthread_create( &m_hThdCtrl, &objThreadAttr, G_CloseOilElec, NULL );
						if( iResult || 0 == m_hThdCtrl )
						{
							PRTMSG( MSG_ERR, "Deal0601 - pthread_create fail %d\n", iResult );
							iRet = ERR_THREAD;
							goto DEAL0601_FAILED;
						}

						// �������߳�
// 						::SetThreadPriority( m_hThdCtrl, THREAD_PRIORITY_TIME_CRITICAL );
// 						::ResumeThread( m_hThdCtrl );
					}

					bOilCut = true;
				}
				else // ����,����Ҫ������ͨ
				{
					PRTMSG( MSG_NOR, "Deal0601 - To Connect Oil Task\n" );

					if( m_hThdCtrl && ESRCH != pthread_kill(m_hThdCtrl, 0) ) // ���������йر��͵��߳�
					{
						// �����߳��˳���־
						m_bThdCtrlExit = true;

						// �ȴ��߳��˳�
						DWORD dwCt = 0;
						while( ESRCH != pthread_kill(m_hThdCtrl, 0) )
						{
							if( dwCt ++ > 25 )
							{
								PRTMSG( MSG_ERR, "Deal0601 - Wait CtrlThread Exit Timeout!\n" );
								iRet = ERR_THREAD;
								goto DEAL0601_FAILED;
								break;
							}

							usleep( 200000 );
						}
					}

					// ���ͺϵ�
					g_objQianIO.ConnectOilElec();

					char buf[200] = {0};
					buf[0] = (char)0x01;
					buf[1] = (char)0x72;
					buf[2] = (char)0x02;
					sprintf(buf+3, "%s", LANG_OILCONN_SUCC);
					DataPush( buf, strlen(buf), DEV_QIAN,  DEV_DIAODU, 1 );
					
#if USE_TTS == 1
					memset( buf, 0, sizeof(buf) );
					buf[0] = 0x01;
					buf[1] = 0x01;
					strcpy(buf+2, LANG_OILCONN_SUCC);
					DataPush( buf, strlen(buf) + 2, DEV_QIAN, DEV_DVR, 1 );
#endif

					bOilCut = false;
				}
			}
		}
	}
	else
	{
		PRTMSG( MSG_NOR, "acc invalid ,keep old station\n" );
	}


	// ���Ͷϵ����ñ���
	tagSpecCtrlCfg objCfg;
	objCfg.Init();
	objCfg.m_bOilCut = bOilCut;
	SetSecCfg( &objCfg.m_bOilCut, offsetof(tagSecondCfg, m_uni2QSpecCtrlCfg.m_obj2QSpecCtrlCfg.m_bOilCut),
		sizeof(objCfg.m_bOilCut) );

	// ���ͳɹ�Ӧ��
	// 080917,��������080899�޸�,ֻ���������ݡ�λ�ò�ѯӦ��Ͷ���ͨ���·����õ�Ӧ���߶���ͨ��
	g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV14, 0 );

	return 0;

#endif

DEAL0601_FAILED:

	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x06, 0x41, buf, sizeof(buf), iBufLen);
	// 080917,��������080899�޸�,ֻ���������ݡ�λ�ò�ѯӦ��Ͷ���ͨ���·����õ�Ӧ���߶���ͨ��
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV15, 0 );

	return iRet;
}

void CSpecCtrl::AccChg(bool v_bOn )
{
#if USE_DISCONNOILELEC == 1
	if( v_bOn ) // ��ACC��Ч
	{
		tagSpecCtrlCfg objCfg;
		objCfg.Init();
		::GetSecCfg( &objCfg, sizeof(objCfg), offsetof(tagSecondCfg, m_uni2QSpecCtrlCfg.m_obj2QSpecCtrlCfg),
			sizeof(objCfg) );

		if( objCfg.m_bOilCut )
		{
			g_objQianIO.DisconnOilElec();
		}
		else
		{
			g_objQianIO.ConnectOilElec();
		}
	}
	else
	{
		g_objQianIO.ConnectOilElec();
	}
#endif
}

// ���߳����еĺ��������̰߳�ȫ�ģ�����Ҳ���������̹߳�������
void CSpecCtrl::DisconnOilElec()
{
	DWORD dwBegin = GetTickCount();
	DWORD dwCur = dwBegin;
	DWORD dwLeave = 30;
	WORD wCt = 0;
	bool bDisconn = true;
	BYTE bytSta = 0; // 0,��ʼ̬; 1,��ʾ̬; 2,ִ��̬

	char frm72[200] = {0};
	frm72[0] = (char)0x01;
	frm72[1] = (char)0x72;
	frm72[2] = (char)0x02;

	char frmTTS[200] = { 0 };
	frmTTS[0] = (char)0x01;
	frmTTS[1] = (char)0x01;

	pthread_setcancelstate( PTHREAD_CANCEL_DISABLE, 0 ); // ����ȡ����Ϣ

	while( !m_bThdCtrlExit )
	{
		switch( bytSta )
		{
		case 0: // ��ʼ̬
#if USE_TTS == 1
			sprintf( frmTTS+2, "%d����ж���·���뿿��ͣ����", DISCONNOILELEC_PERIOD / 1000 );
			DataPush( frmTTS, strlen(frmTTS), DEV_QIAN, DEV_DVR, 1 ); 
			sleep(10);
			dwBegin += 10000;
#endif			
			sprintf(frm72+3, "%d%s", DISCONNOILELEC_PERIOD / 1000, LANG_OILTOCUT );
			DataPush( frm72, strlen(frm72), DEV_QIAN, DEV_DIAODU, 1 );

			bytSta = 2; // ת��ִ��̬
			break;

		case 1: // ��ʾ̬
			{
				if( GetTickCount() - dwBegin >= 14000 ) // ���ӵ�ʱ����Ϊ�˼��㲥����ʾ��ʱ��
				{
					dwBegin = GetTickCount(); // ������ʼʱ��
					bytSta = 2; // ת��ִ��̬
				}
				else
				{
					usleep( 200000 );
				}
			}
			break;

		case 2: // ִ��̬
			{
				if( bDisconn )
				{
					dwCur = GetTickCount();

					if( 0 == ++wCt%10 )
					{
						if( dwCur-dwBegin<DISCONNOILELEC_PERIOD )
						{
							dwLeave = (DISCONNOILELEC_PERIOD - (dwCur - dwBegin)) / 1000;
							if(dwLeave)
							{
#if USE_TTS == 1
								sprintf(frmTTS+2, "%d����ж���·���뿿��ͣ����", dwLeave );
 								DataPush( frmTTS, strlen(frmTTS), DEV_QIAN, DEV_DVR, 1 );
 								sleep(10);
 								dwCur += 10000;
								dwBegin += 10000;
#endif								
								sprintf(frm72+3, "%d%s", dwLeave, LANG_OILTOCUT );
								DataPush( frm72, strlen(frm72), DEV_QIAN, DEV_DIAODU, 1 );
							}
						}
						else
						{
							dwLeave = 0;
						}
					}

					/// �������ر�ע�⣺���Ͷϵ綯����Ϊʵ��ֻ��һ��,���ⷴ��������ɼ̵�����
					//g_objIO.DisconnOilElec();
					if( dwCur - dwBegin >= DISCONNOILELEC_PERIOD )
					{
						g_objQianIO.DisconnOilElec();
						goto DISCONNOILELEC_END;
					}
					usleep( 700000 );
				}
				else
				{
					/// �������ر�ע�⣺���Ͷϵ綯����Ϊʵ��ֻ��һ��,���ⷴ��������ɼ̵�����
					//g_objIO.ConnectOilElec();
					usleep( 300000 );
				}

				bDisconn = !bDisconn;
			}
			break;

		default:
			g_objQianIO.ConnectOilElec();
			goto DISCONNOILELEC_END;
		}
	}

DISCONNOILELEC_END:
	if( m_bThdCtrlExit ) // ���Ǳ���ֹ�߳�
	{
		g_objQianIO.ConnectOilElec();
	}
	else
	{
		sprintf(frm72+3, "%s", LANG_OILCUT_SUCC );
		DataPush( frm72, strlen(frm72), DEV_QIAN,  DEV_DIAODU, 1 );

#if USE_TTS == 1
		strcpy(frmTTS+2, LANG_OILCUT_SUCC);
		DataPush( frmTTS, strlen(frmTTS), DEV_QIAN, DEV_DVR, 1 );
#endif
	}
}
