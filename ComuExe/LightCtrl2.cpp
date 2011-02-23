#include "ComuStdAfx.h"

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2

void *G_ThreadLightCtrl2(void *arg)
{
	g_objLightCtrl2.P_ThreadLightCtrl();
}

#undef MSG_HEAD
#define MSG_HEAD	("Comu-LightCtrl2")

//////////////////////////////////////////////////////////////////////////

CLightCtrl2::CLightCtrl2()
{
	memset(m_szReBootTime, 0, sizeof(m_szReBootTime));
	memset(m_szSwiLinkTime, 0, sizeof(m_szSwiLinkTime));
	memset(m_szDiskLinkTime, 0, sizeof(m_szDiskLinkTime));
	memset(m_szMonLinkTime, 0, sizeof(m_szMonLinkTime));
	
	m_iTelOnCnt = 0;
	memset(m_szTelOnTime, 0, sizeof(m_szTelOnTime));
	memset(m_iTelOnSec, 0, sizeof(m_iTelOnSec));
	m_iTelInitCnt = 0;
	memset(m_szTelInitTime, 0, sizeof(m_szTelInitTime));
	memset(m_iTelInitSec, 0, sizeof(m_iTelInitSec));
	m_iLowSigCnt = 0;
	memset(m_szLowSigTime, 0, sizeof(m_szLowSigTime));
	memset(m_iLowSigSec, 0, sizeof(m_iLowSigSec));
	m_iDialNetCnt = 0;
	memset(m_szDialNetTime, 0, sizeof(m_szDialNetTime));
	memset(m_iDialNetSec, 0, sizeof(m_iDialNetSec));
	m_iTcpConCnt = 0;
	memset(m_szTcpConTime, 0, sizeof(m_szTcpConTime));
	memset(m_iTcpConSec, 0, sizeof(m_iTcpConSec));
	m_iLogInCnt = 0;
	memset(m_szLogInTime, 0, sizeof(m_szLogInTime));
	memset(m_iLogInSec, 0, sizeof(m_iLogInSec));

	m_iDeal3801Cnt = 0;
	memset(m_szDeal3801Time, 0, sizeof(m_szDeal3801Time));
	memset(m_iDeal3801Sec, 0, sizeof(m_iDeal3801Sec));
	m_iVideoMonCnt = 0;
	memset(m_szVideoMonTime, 0, sizeof(m_szVideoMonTime));
	memset(m_iVideoMonSec, 0, sizeof(m_iVideoMonSec));
	
	m_dwDevErrCode = 0;
	m_dwCfgErrCode = 0;
	m_dwTelErrCode = 0;
	m_dwMonErrCode = 0;
	
	m_bytDvrState = 1;
	m_bytTcpState = 0;
	
	m_bNetWork = false;
	m_bNetWork = false;
	m_bFoot = false;
	m_bDial = false;
	m_bytLightSta = Red_ManShan;
	m_pthreadLightCtrl = 0;
}

CLightCtrl2::~CLightCtrl2()
{

}

int CLightCtrl2::Init()
{
	// �������Ƶ��߳�
	if( pthread_create(&m_pthreadLightCtrl, NULL, G_ThreadLightCtrl2, NULL) != 0 )
	{
		PRTMSG(MSG_ERR, "create light thread failed!\n");
		return ERR_THREAD;
	}

	PRTMSG(MSG_ERR, "create light thread succ!\n");
	return 0;
}

int CLightCtrl2::Release()
{

}

void CLightCtrl2::P_ThreadLightCtrl()
{
	int iSleepTime = 1000;
	static int sta_iManShanCnt = 0;
	static int sta_iKuaiShanCnt = 0;

	BYTE  bytLvl = 0;
	char  szbuf[10] = {0};
	DWORD dwLen = 0;
	DWORD dwPushTm;

	RenewMemInfo(0x02, 0x02, 0x00, 0x00);

	sleep(2);
	
	while( !g_bProgExit )
	{
		if( !m_objMemInfoMng.PopData(bytLvl, sizeof(szbuf), dwLen, szbuf, dwPushTm) )
		{
			if( 3 == dwLen )
			{
				switch( szbuf[0] )
				{
					case 0x00:		//�����쳣������(ÿ���쳣���֧��32��������)
						{
							switch( szbuf[1] )
							{
								case 0x00:		//оƬ����
									m_dwDevErrCode |= (0x00000001<<szbuf[2]);
									break;
								case 0x01:		//���ò���
									m_dwCfgErrCode |= (0x00000001<<szbuf[2]);
									break;
								case 0x02:		//�ֻ�ģ��
									m_dwTelErrCode |= (0x00000001<<szbuf[2]);
									break;
								case 0x03:		//��Ƶ���
									m_dwMonErrCode |= (0x00000001<<szbuf[2]);
									break;
								default:
									break;
							}
						}
						break;
					
					case 0x01:		//�豸����״̬��Ϣ
						{
							switch( szbuf[1] )
							{
								case 0x00:		//DvrExe״̬
									m_bytDvrState = szbuf[2];
									break;
								case 0x01:		//Qian Tcp״̬
									m_bytTcpState = szbuf[2];
									break;
								default:
									break;
							}
						}
						break;
						
					case 0x02:		//ͳ��ҵ�����ʱ�䡢����
						{
							switch( szbuf[1] )
							{
								case 0x00:
									{
										switch( szbuf[2] )
										{
											case 0x00:		//���һ�ο���ʱ��
												RecTimeAndCnt(m_szReBootTime);
												break;
											case 0x01:		//���һ��G_SysSwitch�߳��幷ʱ��
												RecTimeAndCnt(m_szSwiLinkTime);
												break;
											case 0x02:		//���һ��G_DiskManage�߳��幷ʱ��
												RecTimeAndCnt(m_szDiskLinkTime);
												break;
											case 0x03:		//���һ��G_DataUpload�߳��幷ʱ��
												RecTimeAndCnt(m_szMonLinkTime);
												break;
											default:
												break;
										}
									}
									break;
								case 0x01:
									{
										switch( szbuf[2] )
										{
											case 0x00:		//���һ���ֻ�ģ���ϵ�ʱ���ǰһСʱ���ϵ����
												RecTimeAndCnt(m_szTelOnTime, &m_iTelOnCnt, m_iTelOnSec, 3600);
												break;
											case 0x01:		//���һ�����ź�ʱ���ǰһСʱ�����źŴ���
												RecTimeAndCnt(m_szLowSigTime, &m_iLowSigCnt, m_iLowSigSec, 3600);
												break;
											case 0x02:		//���һ�β���ʱ���ǰһСʱ�ڲ��Ŵ���
												RecTimeAndCnt(m_szDialNetTime, &m_iDialNetCnt, m_iDialNetSec, 3600);
												break;
											case 0x03:		//���һ������ʱ���ǰ����������Ӵ���
												RecTimeAndCnt(m_szTcpConTime, &m_iTcpConCnt, m_iTcpConSec, 300);
												break;
											case 0x04:		//���һ�ε�½ʱ���ǰ������ڵ�½����
												RecTimeAndCnt(m_szLogInTime, &m_iLogInCnt, m_iLogInSec, 300);
												break;
											case 0x05:		//���һ���ֻ�ģ���ʼ��ʱ���ǰһСʱ�ڳ�ʼ������
												RecTimeAndCnt(m_szTelInitTime, &m_iTelInitCnt, m_iTelInitSec, 3600);
												break;
											default:
												break;
										}
									}
									break;
								case 0x02:
									{
										switch( szbuf[2] )
										{
											case 0x00:		//���һ���յ���Ƶ���ָ��ʱ�����������յ�����
												RecTimeAndCnt(m_szDeal3801Time, &m_iDeal3801Cnt, m_iDeal3801Sec, 300);
												break;
											case 0x01:		//���һ��������Ƶ���ʱ���������ڵ�½����
												RecTimeAndCnt(m_szVideoMonTime, &m_iVideoMonCnt, m_iVideoMonSec, 300);
												break;
											default:
												break;
										}
									}
									break;
							}
						}
	
					default:
						break;				
				}
			}
		}
		
		if( !m_objRecvCtrlMng.PopData(bytLvl, sizeof(szbuf), dwLen, szbuf, dwPushTm) )
		{
			if( 1 == dwLen )
			{
				switch( szbuf[0] )
				{
					case 0x01:		// ���ųɹ�֪ͨ
						m_bNetWork = true;
						m_bDial = false;	// ���粦�ųɹ������߲���ʧ�ܣ������˳����Ź���
						break;
						
					case 0x02:		// �Ͽ�����֪ͨ
						m_bNetWork = false;
						m_bDial = false;	// ���粦�ųɹ������߲���ʧ�ܣ������˳����Ź���
						break;
						
					case 0x03:		// ��ʼ¼��֪ͨ
						m_bVideoWork = true;
						break;
	
					case 0x04:		// ֹͣ¼��֪ͨ
						m_bVideoWork = false;
						break;
	
					case 0x05:		// ���ٱ���֪ͨ
						m_bFoot = true;
						break;
						
					case 0x06:		// ��������֪ͨ
						m_bFoot = false;
						break;
	
					case 0x07:		// ��ʼ����֪ͨ
						m_bDial = true;
						break;
						
					default:
						break;
				}
			}
		}

		// ���ߺ�¼��ָʾλ����ͼ���
		if( m_bNetWork && m_bVideoWork )	m_bytLightSta = Green_KuaiShan;
		if( m_bNetWork && !m_bVideoWork )	m_bytLightSta = Green_ManShan;
		if( !m_bNetWork && m_bVideoWork )	m_bytLightSta = Red_KuaiShan;
		if( !m_bNetWork && !m_bVideoWork )	m_bytLightSta = Red_ManShan;

		// ����ָʾλ���м伶��
		if( m_bDial )	m_bytLightSta = Green_JiShan;

		// ���ٱ���λ����߼���
		if( m_bFoot )	m_bytLightSta = Red_JiShan;
		
#if CHK_VER == 1
		if( m_dwDevErrCode ) m_bytLightSta = Red_JiShan;
#endif

		switch( m_bytLightSta )
		{
		case Red_ManShan:	// �������
			{
				//�ر��̵�
				IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_OFF, NULL, 0);

				sta_iManShanCnt++;
				if( sta_iManShanCnt % 10 == 0 )
					IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_ON, NULL, 0);
				else
					IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_OFF, NULL, 0);
				
				iSleepTime = 500;
			}
			break;
			
		case Red_KuaiShan:	// ��ƿ���
			{
				//�ر��̵�
				IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_OFF, NULL, 0);

				if( sta_iKuaiShanCnt++ % 2 == 0 )
					IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_ON, NULL, 0);
				else
					IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_OFF, NULL, 0);

				iSleepTime = 2000;
			}
			break;
			
		case Green_ManShan:	// �̵�����
			{
				//�رպ��
				IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_OFF, NULL, 0);
				
				sta_iManShanCnt++;
				if( sta_iManShanCnt % 10 == 0 )
					IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_ON, NULL, 0);
				else
					IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_OFF, NULL, 0);
				
				iSleepTime = 500;
			}
			break;
			
		case Green_KuaiShan:	// �̵ƿ���
			{
				//�رպ��
				IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_OFF, NULL, 0);
				
				if( sta_iKuaiShanCnt++ % 2 == 0 )
					IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_ON, NULL, 0);
				else
					IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_OFF, NULL, 0);
				
				iSleepTime = 2000;
			}
			break;

		case Red_JiShan:
			{
				//�ر��̵�
				IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_OFF, NULL, 0);
				
				if( sta_iKuaiShanCnt++ % 2 == 0 )
					IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_ON, NULL, 0);
				else
					IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_OFF, NULL, 0);
				
				iSleepTime = 500;
			}
			break;
			
		case Green_JiShan:
			{
				//�رպ��
				IOSet(IOS_SYSLEDPOW_RED, IO_SYSLEDPOW_RED_OFF, NULL, 0);
				
				if( sta_iKuaiShanCnt++ % 2 == 0 )
					IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_ON, NULL, 0);
				else
					IOSet(IOS_SYSLEDPOW_GREEN, IO_SYSLEDPOW_GREEN_OFF, NULL, 0);
				
				iSleepTime = 500;
			}
			break;

		default:
			iSleepTime = 1000;
			break;
		}

		usleep(iSleepTime*1000);
	}
}

void CLightCtrl2::SetLightSta(char v_szSta)
{
	DWORD	dwPackNum;
	m_objRecvCtrlMng.PushData(LV1, 1, &v_szSta, dwPackNum);
}

#endif
