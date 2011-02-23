#include "ComuStdAfx.h"


#undef MSG_HEAD
#define MSG_HEAD ("ComuExe-DiaoDu:")

#if CHK_VER == 1
	#define ALL_PASS	""				//������ͨ������
	#define LIU_PASS	"4312896654"	//��ý�������������
	#define EXP_PASS	"1122334455"	//����ģʽ����(���̲˵���������뼴�������ģʽ)
#else 
	#define ALL_PASS	"4312897765"	//������ͨ������
	#define LIU_PASS	"4312896654"	//��ý�������������
	#define EXP_PASS	"1122334455"	//����ģʽ����(���̲˵���������뼴�������ģʽ)
#endif


#define build_frm(a, b, c)	if(a)	{ strcpy(frm+len, b); len+=strlen(b); }  \
							else	{ strcpy(frm+len, c); len+=strlen(c); }

#define build_frm2(a, b, c, d)	if(a)	{ sprintf(frm+len, "%s:%s,", b, c); len+=strlen(b)+strlen(c)+2; }  \
								else	{ sprintf(frm+len, "%s:%s,", b, d); len+=strlen(b)+strlen(d)+2; }


bool UDiskMount(void *Event)
{
	if( access("/mnt/UDisk/part1/flash", F_OK)!=0 && access("/mnt/UDisk/part1/data/disk", F_OK)==0 )
		return true;
	else
		return false;
}

// void *G_DiaoduReadThread(void *arg)
// {
// 	g_objDiaodu.P_DiaoduReadThread();
// }

void *G_DiaoDuWorkThread(void *arg)
{
	g_objDiaodu.P_DiaoduWorkThread();
}

void *G_ExeUpdateThread(void *arg)
{
	g_objDiaodu.P_ExeUpdateThread();
}

void *G_ImageUpdateThread(void *arg)
{
	g_objDiaodu.P_ImageUpdateThread();
}

void *G_AppUpdateThread(void *arg)
{
	g_objDiaodu.P_AppUpdateThread();
}

void *G_SysUpdateThread(void *arg)
{
	g_objDiaodu.P_SysUpdateThread();
}

void *G_DataOutPutThread(void *arg)
{
	//g_objDiaodu.P_DataOutPutThread();
	g_objDiaodu.P_VideoOutPutThread();
}

void *G_VideoOutPutThread(void *arg)
{
	g_objDiaodu.P_VideoOutPutThread();
}

void *G_IoDetect(void *arg)
{
	g_objDiaodu._IoDetectThread();
}

//////////////////////////////////////////////////////////////////////////
CDiaodu::CDiaodu()
{
//	m_iComPort = 0;
	m_pthreadDiaoduWork = 0;
//	m_pthreadDiaoduRead = 0;

	m_gpsdect = 0;
	m_iodect = 0;

	memset(m_szComFrameBuf,0, sizeof(m_szComFrameBuf));
	m_iComFrameLen = 0;
}

CDiaodu::~CDiaodu()
{
	
}

int CDiaodu::Init()
{
	IOSet(IOS_SCHEDULEPOW, IO_SCHEDULEPOW_ON, NULL, 0);//�������ϵ�

// #if USE_COMBUS == 0		// ��ʹ�ô�����չ�У��򲻴򿪴���
// 	// �򿪴���
// 	if( false == ComOpen() )
// 		return ERR_COM;
// 
// 	// �������Ƚ����߳�
// 	if( pthread_create(&m_pthreadDiaoduRead, NULL, G_DiaoduReadThread, NULL) != 0 )
// 	{
// 		PRTMSG(MSG_ERR, "Create Diaodu read thread failed!\n");
// 		perror(" ");
// 
// 		return ERR_THREAD;
// 	}
// #endif

	// �������ȴ����߳�
	if( pthread_create(&m_pthreadDiaoduWork, NULL, G_DiaoDuWorkThread, NULL) != 0 )
	{
		PRTMSG(MSG_ERR, "Create Diaodu thread failed!\n");
		perror(" ");
		
		return ERR_THREAD;
	}

	return 0;
}

int CDiaodu::Release()
{
// #if USE_COMBUS == 0
// 	ComClose();		// ���ڹر�֮�󣬶����ں��������ش��󣬶��߳��˳�
// #endif
	return 0;
}

// void CDiaodu::P_DiaoduReadThread()
// {
// 	char szReadBuf[1024] = {0};
// 	int  iBufLen = sizeof(szReadBuf);
// 	int  iReadLen = 0;
// 	int  i = 0;
// 	byte bytLvl = 0;
// 	DWORD dwPushTm;
// 
// 	while(!g_bProgExit) 
// 	{
// 		iReadLen = read(m_iComPort, szReadBuf, iBufLen);
// 
// //		PRTMSG(MSG_DBG, "diao recv data\n");
// //		PrintString(szReadBuf, iReadLen);
// 
// 		if(iReadLen > iBufLen)
// 		{
// 			PRTMSG(MSG_ERR,"Com Read Over Buf!\n");
// 			memset(szReadBuf, 0, sizeof(szReadBuf));
// 			iReadLen = 0;
// 
// 			continue;
// 		}		
// 
// 		if( iReadLen > 0 )
// 		{
// 			AnalyseComFrame(szReadBuf, iReadLen);
// 		}
// 	}
// }

void CDiaodu::P_DiaoduWorkThread()
{
	byte  bytLvl = 0;
	char  szRecvBuf[1024] = {0};
	char  szSendBuf[1024] = {0};
	DWORD dwRecvLen = 0;
	int   iSendLen = 0;
	DWORD dwPushTm;
	DWORD dwBufSize = (DWORD)sizeof(szRecvBuf);

	while( !g_bProgExit )
	{
		usleep(50000);

		dwRecvLen = 0;
		memset(szRecvBuf, 0, sizeof(szRecvBuf));

		// �ȴ����ڽ��յ�������
		if( !m_objDiaoduReadMng.PopData(bytLvl, dwBufSize, dwRecvLen, szRecvBuf, dwPushTm))
		{
			//PRTMSG(MSG_DBG, "m_objDiaoduReadMng PopData:");
			//PrintString(szRecvBuf, dwRecvLen);

			if( dwRecvLen <= dwBufSize )
				DealDiaoduFrame(szRecvBuf, (int)dwRecvLen);
		}

		// ����ǰ����ʹ�õ���������continue
		if (g_iTerminalType != 2)
			continue;

		// ���򴮿ڷ�������
		if( !g_objDiaoduOrHandWorkMng.PopData(bytLvl, dwBufSize-1, dwRecvLen, szRecvBuf+1, dwPushTm))
		{
			// ����У���
			szRecvBuf[0] = get_crc((byte*)(szRecvBuf+1), int(dwRecvLen));
			
			iSendLen = 0;
#if USE_COMBUS == 1
			szSendBuf[iSendLen++] = 0xff;	// ʹ�ô�����չ��ʱ���������������ֱ�������Ҫ��QianExe�����CComBusת��
			szSendBuf[iSendLen++] = 0xf3;	// 0xf3��ʾ����չ�е�������3���з���
#endif
#if USE_COMBUS == 2
			szSendBuf[iSendLen++] = 0xff;	// ʹ��150TRʱ���������������ֱ�������Ҫ��QianExe�����CCom150TRת��
			szSendBuf[iSendLen++] = 0xA0;	// 0xA0��ʾ��150TR����չ���ڽ��з���
			szSendBuf[iSendLen++] = 0x01;	// ��չ����1
#endif
			// �����ݽ���ת��(ͷβ��0x7e)
			szSendBuf[iSendLen++] = 0x7e;

			for(DWORD i=0; i<dwRecvLen+1; i++) 
			{
				switch(szRecvBuf[i])
				{
				case 0x7e:	
					szSendBuf[iSendLen++]=0x7d;  
					szSendBuf[iSendLen++]=0x02;  
					break;

				case 0x7d:	
					szSendBuf[iSendLen++]=0x7d; 
					szSendBuf[iSendLen++]=0x01; 
					break;

				default:
					szSendBuf[iSendLen++]=szRecvBuf[i];
					break;
				}
			}
			szSendBuf[iSendLen++] = 0x7e;
			
			// ��������֡		
			g_objComAdjust.ComWrite(szSendBuf, iSendLen);
		}

		// �����ã�1���Ӵ�ӡһ��GPS״̬��Ϣ
// 		static DWORD test = 0;
// 		if(test==0)	test = GetTickCount();
// 		if((GetTickCount()-test)>1000) 
// 		{
// 			test = 0;
// 			tagSatelliteInfo info;
// 			char str[100] = {0};
// 			if(g_objGpsSrc.GetSatellitesInfo(info)) 
// 			{
// 				int cnt = 0;
// 				int i = 0;
// 				for( i=0; i<info.m_bytSatelliteCount; i++) 
// 				{
// 					if(info.m_arySatelliteInfo[i][1]>35)	cnt++;
// 				}
// 				sprintf(str, "NUM:%02d SNR>35:%d ", info.m_bytSatelliteCount, cnt);
// 				for(i=0; i<info.m_bytSatelliteCount; i++) 
// 				{
// 					char tmp[20] = {0};
// 					sprintf(tmp, "%d/%d ", info.m_arySatelliteInfo[i][0], info.m_arySatelliteInfo[i][1]);
// 					if((strlen(str)+strlen(tmp))>=45)	break;
// 					strcat(str, tmp);
// 				}
// 			}
// 
// 			PRTMSG(MSG_DBG, "%s\n", str);
// 		}

		// �ٴ����������Ϣ����
		static DWORD t1 = 0;
		if(t1==0)	t1 = GetTickCount();
		if((GetTickCount()-t1)>2500) 
		{ 
			t1 = 0;
			
			if(g_diskfind) 
			{
				show_diaodu(LANG_CHKING_U);
			}
			
			if(g_SysUpdate) 
			{
				show_diaodu(LANG_NK_UPGRADING);
			}

			if (g_AppUpdate)
			{
				show_diaodu(LANG_SFT_UPGRADING);
			}
			
			if (g_ExeUpdate)
			{
				show_diaodu(LANG_SFT_UPGRADING);
			}
			
			if (g_ImageUpdate)
			{
				show_diaodu(LANG_NK_UPGRADING);
			}
			
			if(g_DataOutPut)
			{
				show_diaodu(LANG_DATAOUTPUT);
			}
			
			if(m_gpsdect) 
			{		// ʵʱ��ʾGPS��ʾ��Ϣ
				static DWORD t2 = 0;
				if(m_gpsdect==1)	{ m_gpsdect = 2; t2 = GetTickCount(); }
				if( (GetTickCount()-t2) > 30*1000 ) 
				{
					t2 = 0;
					m_gpsdect = 0;
					show_diaodu(LANG_GPS_CHKMODE_CLOSE);
				} else 
				{
					tagSatelliteInfo info;
					char str[100] = {0};
					if(g_objGpsSrc.GetSatellitesInfo(info)) 
					{
						int cnt = 0;
						int i = 0;
						for( i=0; i<info.m_bytSatelliteCount; i++) 
						{
							if(info.m_arySatelliteInfo[i][1]>35)	cnt++;
						}
						sprintf(str, "NUM:%02d SNR>35:%d ", info.m_bytSatelliteCount, cnt);
						for(i=0; i<info.m_bytSatelliteCount; i++) 
						{
							char tmp[20] = {0};
							sprintf(tmp, "%d/%d ", info.m_arySatelliteInfo[i][0], info.m_arySatelliteInfo[i][1]);
							if((strlen(str)+strlen(tmp))>=45)	break;
							strcat(str, tmp);
						}
						show_diaodu(str);
					} 
					else 
					{
						show_diaodu(LANG_GPS_NOSIGNAL);
					}
				}	
			}

			if(m_iodect) 
			{	//������ʵʱ���
				static DWORD t2 = 0;
				if(m_iodect==1)		
				{ 
					m_iodect = 2; 
					t2 = GetTickCount();
// 					//��������������߳�,(��IoSta�м��)
// 					pthread_t h_ThIoDetect;
// 					if( 0 != pthread_create(&h_ThIoDetect, NULL, G_IoDetect, NULL) )
// 					{
// 						PRTMSG(MSG_ERR,"����RecvPhoneData���ݽ����߳�ʧ�ܣ�\n");
// 						show_diaodu(LANG_IO_CHKMODE_CLOSE);
// 					}
				}
				if( (GetTickCount()-t2) > 5*60*1000 ) 
				{
					t2 = 0;
					m_iodect = 0;
					show_diaodu(LANG_IO_CHKMODE_CLOSE);
					
					char by = 0x0a;
					DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
				} 
			}
		}
	}
}

//��ʹ�ã�ʹ��IoSta�е��ж��źż��
void CDiaodu::_IoDetectThread()
{
	DWORD dwIoSta = g_objIoSta.GetIoSta();
	DWORD dwResult = 0;
	while(g_objDiaodu.m_iodect > 0)
	{
//		dwIoSta = g_objIoSta.GetIoSta();

		//ACC
		dwResult = g_objIoSta.GetIoSta();
		if( (IOSTA_ACC_ON&dwResult) != (IOSTA_ACC_ON&dwIoSta) )
		{
			if (IOSTA_ACC_ON&dwResult)
			{
				dwIoSta |= IOSTA_ACC_ON;
				show_diaodu(LANG_IOCHK_ACCON);
			}
			else
			{
				dwIoSta &= ~IOSTA_ACC_ON;
				show_diaodu(LANG_IOCHK_ACCOFF);
			}
			usleep(300);
		}
		
		//�����ź�
		dwResult = g_objIoSta.GetIoSta();
		if ( (IOSTA_ALARM_ON&dwResult) != (IOSTA_ALARM_ON&dwIoSta) )
		{
			if (IOSTA_ALARM_ON&dwResult)
			{
				dwIoSta |= IOSTA_ALARM_ON;
				show_diaodu(LANG_IOCHK_ALERMFOOT);
			}
			else
			{
				dwIoSta &= ~IOSTA_ALARM_ON;
				show_diaodu(LANG_IOCHK_ALERMFOOT);
			}
			usleep(300);
		}

		//���س�/�Ƽ���
		dwResult = g_objIoSta.GetIoSta();
		if( (IOSTA_JIJIA_HEAVY&dwResult) != (IOSTA_JIJIA_HEAVY&dwIoSta) )
		{
			if (IOSTA_JIJIA_HEAVY&dwResult)
			{
				dwIoSta |= IOSTA_JIJIA_HEAVY;
				show_diaodu(LANG_IOCHK_JIJIA_VALID);
			}
			else
			{
				dwIoSta &= ~IOSTA_JIJIA_HEAVY;
				show_diaodu(LANG_IOCHK_JIJIA_INVALID);
			}
			usleep(300);
		}
		
		//ǰ�����ź�
		dwResult = g_objIoSta.GetIoSta();
		if( (IOSTA_FDOOR_OPEN&dwResult) != (IOSTA_FDOOR_OPEN&dwIoSta) )
		{
			if (IOSTA_FDOOR_OPEN&dwResult)
			{
				dwIoSta |= IOSTA_FDOOR_OPEN;
				show_diaodu(LANG_IOCHK_DOOROPEN);
			}
			else
			{
				dwIoSta &= ~IOSTA_FDOOR_OPEN;
				show_diaodu(LANG_IOCHK_DOORCLOSE);
			}
			usleep(300);
		}
		
		//�����ź�
		dwResult = g_objIoSta.GetIoSta();
		if( (IOSTA_BDOOR_OPEN&dwResult) != (IOSTA_BDOOR_OPEN&dwIoSta) )
		{
			if (IOSTA_BDOOR_OPEN&dwResult)
			{
				dwIoSta |= IOSTA_BDOOR_OPEN;
				show_diaodu(LANG_IOCHK_BACKDOOROPEN);
			}
			else
			{
				dwIoSta &= ~IOSTA_BDOOR_OPEN;
				show_diaodu(LANG_IOCHK_BACKDOORCLOSE);
			}
			usleep(300);
		}
		
		//��ɲ�ź�
		dwResult = g_objIoSta.GetIoSta();
		if ( (IOSTA_JIAOSHA_VALID&dwResult) != (IOSTA_JIAOSHA_VALID&dwIoSta) )
		{
			if (IOSTA_JIAOSHA_VALID&dwResult)
			{
				dwIoSta |= IOSTA_JIAOSHA_VALID;
				show_diaodu(LANG_IOCHK_FOOTBRAKE);
			}
			else
			{
				dwIoSta &= ~IOSTA_JIAOSHA_VALID;
				show_diaodu(LANG_IOCHK_FOOTBRAKE_CANCEL);
			}
			usleep(300);
		}
		
		//��ɲ�ź�
		dwResult = g_objIoSta.GetIoSta();
		if ( (IOSTA_SHOUSHA_VALID&dwResult) != (IOSTA_SHOUSHA_VALID&dwIoSta) )
		{
			if (IOSTA_SHOUSHA_VALID&dwResult)
			{
				dwIoSta |= IOSTA_SHOUSHA_VALID;
				show_diaodu(LANG_IOCHK_HANDBRAKE);
			}
			else
			{
				dwIoSta &= ~IOSTA_SHOUSHA_VALID;
				show_diaodu(LANG_IOCHK_HANDBRAKE_CANCEL);
			}
			usleep(300);
		}

		//�ؿ��ź�1
		dwResult = g_objIoSta.GetIoSta();
		if( (IOSTA_PASSENGER1_VALID&dwResult) != (IOSTA_PASSENGER1_VALID&dwIoSta) )
		{
			if (IOSTA_PASSENGER1_VALID&dwResult)
			{
				dwIoSta |= IOSTA_PASSENGER1_VALID;
				show_diaodu(LANG_IOCHK_PASSENGER1_VALID);
			}
			else
			{
				dwIoSta &= ~IOSTA_PASSENGER1_VALID;
				show_diaodu(LANG_IOCHK_PASSENGER1_INVALID);
			}
			usleep(300);
		}
		
		//�ؿ��ź�2
		dwResult = g_objIoSta.GetIoSta();
		if( (IOSTA_PASSENGER2_VALID& dwResult) != (IOSTA_PASSENGER2_VALID& dwIoSta) )
		{
			if (IOSTA_PASSENGER2_VALID& dwResult)
			{
				dwIoSta |= IOSTA_PASSENGER2_VALID;
				show_diaodu(LANG_IOCHK_PASSENGER2_VALID);
			}
			else
			{
				dwIoSta &= ~IOSTA_PASSENGER2_VALID;
				show_diaodu(LANG_IOCHK_PASSENGER2_INVALID);
			}			
			usleep(300);
		}
		
		//�ؿ��ź�3
		dwResult = g_objIoSta.GetIoSta();
		if( (IOSTA_PASSENGER3_VALID&dwResult) != (IOSTA_PASSENGER3_VALID&dwIoSta) )
		{
			if (IOSTA_PASSENGER3_VALID&dwResult)
			{
				dwIoSta |= IOSTA_PASSENGER3_VALID;
				show_diaodu(LANG_IOCHK_PASSENGER3_VALID);
			}
			else
			{
				dwIoSta &= ~IOSTA_PASSENGER3_VALID;
				show_diaodu(LANG_IOCHK_PASSENGER3_INVALID);
			}
			usleep(300);
		}
	}
}

bool CDiaodu::P_ExeUpdateThread()
{
	char by;
	
	if (true == g_bProgExit)
	{
		return false;
	}
	
	// ����ͬʱ����߳̽��п���
	static bool copying = false;
	if(copying)		return 0;
	copying = true;
	
	bool finded = false;
	char szShowBuf[1024] = {0};
	char str[128] = {0};
	char const szUpdateFile[32] = "exe.bin";
	char diskPathUpfile[128] = {0};	//����·��+�����ļ�
	int len = 0;
	
	// �ȼ��U��
	show_diaodu(LANG_CHKING_U);
	
	g_diskfind = true;
	if( WaitForEvent(UDiskMount, NULL, 15000) )
	{
		g_diskfind = false;
		show_diaodu(LANG_CHKU_FAIL);
		copying = false;
		return false;
	}
	g_diskfind = false;

	//��ʼ�����ļ�
	show_diaodu(LANG_SFT_UPGRADING);
	
	by = 0x03;
	DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
	
	g_ExeUpdate = true;
	
	sprintf(diskPathUpfile, "/mnt/%s/%s", UDISK_UP_PATH, szUpdateFile);

	// �������ļ�exe.bin���Ƶ�part3������Ŀ¼�£�����Ϊ��������
	if( AppIntact(diskPathUpfile) == false || 0 != system("cp -f /mnt/UDisk/part1/exe.bin /mnt/Flash/part3/Down/exe.bin") )
	{
		show_diaodu("����ʧ��");
		copying = false;
		return false;
	}
	PRTMSG(MSG_DBG, "cp exe.bin to part3 succ!\n");

#if VEHICLE_TYPE == VEHICLE_M
	if( G_ExeUpdate(diskPathUpfile, 1) == true )
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	if( G_ExeUpdate(diskPathUpfile, 0) == true )
#endif
	{
		strcpy(str, "exe.bin ");
			
		strcpy(szShowBuf+len, str);
		len += strlen(str);
		finded = true;
		PRTMSG(MSG_DBG, "%s update succ\n", szUpdateFile);

// 		// �����ɹ��󣬽��ָ��������ã���Ҫ�����ֻ��š�IP���˿�
// 		char szbuf[40] = {0};
// 		sprintf(szbuf, "%s", NEED_RUSEMUCFG);
// 		SetImpCfg(szbuf, offsetof(tagImportantCfg, m_szSpecCode), sizeof(szbuf));
// 		SetSecCfg(szbuf, offsetof(tagSecondCfg, m_szSpecCode), sizeof(szbuf));

		DelErrLog();	// �����ɹ������ӷ���5����

		sleep(1);
	}
		
	sleep(2);
	g_ExeUpdate = false;
	
	by = 0x08;
	DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);

	sprintf(str, "����%s!", finded ? LANG_SUCC : LANG_FAIL);
	strcpy(szShowBuf+len, str);
	len += strlen(str);
	szShowBuf[len] = 0;
	sleep(1);
	
	// ����ʧ�ܣ�������
	if( !finded )
	{
		show_diaodu(szShowBuf);
		copying = false;
		return false;
	}
	
	// ��ʾ������������ʾ,ֱ������
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu("%s",LANG_SYS_TORESET );
	sleep(2);

	G_ResetSystem();
}

bool CDiaodu::P_ImageUpdateThread()
{
	char by;
	
	if (true == g_bProgExit)
	{
		return false;
	}
	
	// ����ͬʱ����߳̽��п���
	static bool copying = false;
	if(copying)		return 0;
	copying = true;
	
	bool finded = false;
	char szShowBuf[1024] = {0};
	char str[128] = {0};
	char const szUpdateFile[32] = "image.bin";
	char diskPathUpfile[128] = {0};	//����·��+�����ļ�
	int len = 0;
	
	// �ȼ��U��
	show_diaodu(LANG_CHKING_U);
	
	g_diskfind = true;
	if( WaitForEvent(UDiskMount, NULL, 15000) )
	{
		g_diskfind = false;
		show_diaodu(LANG_CHKU_FAIL);
		copying = false;
		return false;
	}
	g_diskfind = false;

	//��ʼ�����ļ�
	show_diaodu(LANG_NK_UPGRADING);
	
	by = 0x03;
	DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
	
	g_ImageUpdate = true;
	
	sprintf(diskPathUpfile, "/mnt/%s/%s", UDISK_UP_PATH, szUpdateFile);

#if VEHICLE_TYPE == VEHICLE_M
	if( G_ImageUpdate(diskPathUpfile, 1)==true )
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	if( G_ImageUpdate(diskPathUpfile, 0)==true )
#endif
	{
		strcpy(str, "image.bin ");
			
		strcpy(szShowBuf+len, str);
		len += strlen(str);
		finded = true;
		PRTMSG(MSG_DBG, "%s update succ\n", szUpdateFile);

// 		// �����ɹ��󣬽��ָ��������ã���Ҫ�����ֻ��š�IP���˿�
// 		char szbuf[40] = {0};
// 		sprintf(szbuf, "%s", NEED_RUSEMUCFG);
// 		SetImpCfg(szbuf, offsetof(tagImportantCfg, m_szSpecCode), sizeof(szbuf));
// 		SetSecCfg(szbuf, offsetof(tagSecondCfg, m_szSpecCode), sizeof(szbuf));

		DelErrLog();	// �����ɹ������ӷ���5����
		DelVerFile();	// �����ɹ����޸�ϵͳ�汾��
		
		sleep(1);
	}
		
	sleep(2);
	g_ImageUpdate = false;
	
	by = 0x08;
	DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);

	sprintf(str, "����%s!", finded ? LANG_SUCC : LANG_FAIL);
	strcpy(szShowBuf+len, str);
	len += strlen(str);
	szShowBuf[len] = 0;
	sleep(1);
	
	// ����ʧ�ܣ�������
	if( !finded )
	{
		show_diaodu(szShowBuf);
		copying = false;
		return false;
	}
	
	// ��ʾ������������ʾ,ֱ������
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu("%s",LANG_SYS_TORESET );
	sleep(2);

	G_ResetSystem();
}

bool CDiaodu::P_AppUpdateThread()
{
	char by;
	
	if (true == g_bProgExit)
	{
		return false;
	}
	
	// ����ͬʱ����߳̽��п���
	static bool copying = false;
	if(copying)		return 0;
	copying = true;
	
	char *pBegin = NULL;
	char *pEnd = NULL;
	bool finded = false;
	char szShowBuf[1024] = {0};
	char str[128] = {0};
	char const szUpdateFile[][32] = {"QianExe", "ComuExe", "SockServExe", "IOExe", "UpdateExe", "DvrExe", "libComuServ.so"};
	char diskPathUpfile[128] = {0};	//����·��+�����ļ�
	char flashPathUpfile[128] = {0};
	int len = 0;
	int i;
	
	// �ȼ��U��
	show_diaodu(LANG_CHKING_U);
	
	g_diskfind = true;
	if( WaitForEvent(UDiskMount, NULL, 15000) )
	{
		g_diskfind = false;
		show_diaodu(LANG_CHKU_FAIL);
		copying = false;
		return false;
	}
	g_diskfind = false;

	//��ʼ�����ļ�
	show_diaodu(LANG_SFT_UPGRADING);
	
	by = 0x03;
	DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
	
	g_AppUpdate = true;
	for(i=0; i<(sizeof(szUpdateFile)/sizeof(szUpdateFile[0])); i++)
	{
		sprintf(diskPathUpfile, "/mnt/%s/%s", UDISK_UP_PATH, szUpdateFile[i]);
		sprintf(flashPathUpfile, "/mnt/Flash/part5/%s", szUpdateFile[i]);
		
#if VEHICLE_TYPE == VEHICLE_M
		if( G_AppUpdate(diskPathUpfile, flashPathUpfile, 1)==0 )
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		if( G_AppUpdate(diskPathUpfile, flashPathUpfile, 0)==0 )
#endif
		{
// 		�����ɹ��󣬽��ָ��������ã���Ҫ�����ֻ��š�IP���˿�
//		char szbuf[40] = {0};
//		sprintf(szbuf, "%s", NEED_RUSEMUCFG);
//		SetImpCfg(szbuf, offsetof(tagImportantCfg, m_szSpecCode), sizeof(szbuf));
//		SetSecCfg(szbuf, offsetof(tagSecondCfg, m_szSpecCode), sizeof(szbuf));

			DelErrLog();	// �����ɹ������ӷ���5����

			switch(i)
			{
				case 0:
					strcpy(str, "Qian ");
					break;
				case 1:
					strcpy(str, "Comu ");
					break;
				case 2:
					strcpy(str, "Sock ");
					break;
				case 3:
					strcpy(str, "IO ");
					break;
				case 4:
					strcpy(str, "Upd ");
					break;
				case 5:
					strcpy(str, "Dvr ");
					break;
				case 6:
					strcpy(str, "ComuServ ");
					break;
				default:
					break;
			}
			
			strcpy(szShowBuf+len, str);
			len += strlen(str);
			finded = true;
			PRTMSG(MSG_DBG, "%s update succ\n",szUpdateFile[i]);
			sleep(1);
		}
	}
	sleep(2);
	g_AppUpdate = false;
	
	by = 0x08;
	DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);

	sprintf(str, "����%s!", finded ? LANG_SUCC : LANG_FAIL);
	strcpy(szShowBuf+len, str);
	len += strlen(str);
	szShowBuf[len] = 0;
	sleep(1);
	
	// ����ʧ�ܣ�������
	if( !finded )
	{
		show_diaodu(szShowBuf);
		copying = false;
		return false;
	}
	
	// ��ʾ������������ʾ,ֱ������
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu("%s",LANG_SYS_TORESET );
	sleep(2);

	G_ResetSystem();
}

bool CDiaodu::P_SysUpdateThread()
{
	char by;
	
	if (true == g_bProgExit)
	{
		return false;
	}
	
	bool finded = false;
	char szShowBuf[1024] = {0};
	char szCommand[128] = {0};
	char str[128] = {0};
	char szMtdPath[][32] = {BOOT_PART, KERN_PART, ROOT_PART, UPD_PART, APP_PART};
	char const szUpdateFile[][32] = {"boot.bin", "kernel.bin", "root.bin", "update.bin", "app.bin"};
	char diskPathUpfile[128] = {0};	//����·��+�����ļ�
	int len = 0;
	int i;
	
	// �ȼ��U��
	show_diaodu(LANG_CHKING_U);
	
	g_diskfind = true;
	if( WaitForEvent(UDiskMount, NULL, 15000) )
	{
		g_diskfind = false;
		show_diaodu(LANG_CHKU_FAIL);
		return false;
	}
	g_diskfind = false;

	//��ʼ�����ļ�
	show_diaodu(LANG_NK_UPGRADING);
	
	by = 0x03;
	DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
	
	g_SysUpdate = true;
	for(i=0; i<(sizeof(szUpdateFile)/sizeof(szUpdateFile[0])); i++)
	{
		sprintf(diskPathUpfile, "/mnt/%s/%s", UDISK_UP_PATH, szUpdateFile[i]);

#if VEHICLE_TYPE == VEHICLE_M
		if( G_SysUpdate(diskPathUpfile, szMtdPath[i], 1)==0 )
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		if( G_SysUpdate(diskPathUpfile, szMtdPath[i], 0)==0 )
#endif
		{
			if(i == 0 || i == 1 || i == 2)
				DelVerFile();	// �����ɹ����޸�ϵͳ�汾��
			else if(i == 3 || i == 4)
				DelErrLog();	// �����ɹ������ӷ���5����
					
			switch(i)
			{
				case 0:
					strcpy(str, "boot.bin ");
					break;
				case 1:
					strcpy(str, "kernel.bin ");
					break;
				case 2:
					strcpy(str, "root.bin ");
					break;
				case 3:
					strcpy(str, "update.bin ");
					break;
				case 4:
					strcpy(str, "app.bin ");
					break;
				default:
					break;
			}
			
			strcpy(szShowBuf+len, str);
			len += strlen(str);		
			finded = true;
			PRTMSG(MSG_DBG, "%s update succ\n", szUpdateFile[i]); 
			sleep(1);
		}
	}
	sleep(2);
	g_SysUpdate = false;
	
	by = 0x08;
	DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);

	sprintf(str, "����%s!", finded ? LANG_SUCC : LANG_FAIL);
	strcpy(szShowBuf+len, str);
	len += strlen(str);
	szShowBuf[len] = 0;
	sleep(1);
	
	// ����ʧ�ܣ�������
	if( !finded )
	{
		show_diaodu(szShowBuf);
		return false;
	}
	
	// ��ʾ������������ʾ,ֱ������
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu(szShowBuf);
	sleep(3);
	show_diaodu("%s",LANG_SYS_TORESET );
	sleep(2);

	G_ResetSystem();
}

bool CDiaodu::P_DataOutPutThread()
{
	if (true == g_bProgExit)
	{
		return false;
	}
	
	// ����ͬʱ����߳̽��п���
	static bool copying = false;
	if(copying)		return 0;
	copying = true;
	
	// �ȼ��U��
	show_diaodu(LANG_CHKING_U);
	
	bool bdiskfind = true;
	int  i = 0;
	char szDesFilePath[255] = {0};
	char szDiskPathCheck[100] = {0};
	
	sprintf(szDiskPathCheck, "/mnt/%s/flash", UDISK_UP_PATH);
	for (i=0; i<15; i++) 
	{
		if( 0 == access(szDiskPathCheck, F_OK) )
		{
			sleep(1);
			continue;
		}
		sleep(1);
		break;
	}
	
	g_diskfind = false;
	
	if(i>=15) 
	{
		show_diaodu(LANG_CHKU_FAIL);
		copying = false;
		return false;
	}
	
	show_diaodu(LANG_DATAOUTPUT);
	g_DataOutPut = true;
		
	// ����part4������
	if( 0 == access(FLASH_PART4_PATH, F_OK) )
	{ 		
		memset(szDesFilePath, 0, sizeof(szDesFilePath));
		sprintf(szDesFilePath, "/mnt/%s/data/", UDISK_UP_PATH);
		PRTMSG(MSG_DBG, "szDesFilePaht: %s\n", szDesFilePath);
				
		if( 0 != DirCopy(FLASH_PART4_PATH, szDesFilePath) )
		{
			PRTMSG(MSG_DBG, "copy %s succ!\n", FLASH_PART4_PATH);
		}
		else
		{
			PRTMSG(MSG_DBG, "copy %s failed!\n", FLASH_PART4_PATH);
		}
	}
	
	copying = false;
	g_DataOutPut = false;
	show_diaodu("���ݵ������");
}

bool CDiaodu::P_VideoOutPutThread()
{
	if (true == g_bProgExit)
	{
		return false;
	}
	
	// ����ͬʱ����߳̽��п���
	static bool copying = false;
	if(copying)		return 0;
	copying = true;
	
	// �ȼ��U��
	show_diaodu(LANG_CHKING_U);
	
	bool bdiskfind = true;
	int  i = 0;
	char szDesFilePath[255] = {0};
	char szDiskPathCheck[100] = {0};
	
	sprintf(szDiskPathCheck, "/mnt/%s/flash", UDISK_UP_PATH);
	for (i=0; i<15; i++) 
	{
		if( 0 == access(szDiskPathCheck, F_OK) )
		{
			sleep(1);
			continue;
		}
		sleep(1);
		break;
	}
	
	g_diskfind = false;
	
	if(i>=15) 
	{
		show_diaodu(LANG_CHKU_FAIL);
		copying = false;
		return false;
	}
	
	show_diaodu(LANG_DATAOUTPUT);
	g_DataOutPut = true;
	
	// ��Ƭ�ļ�
	if( 0 == access(PHOTO_SAVE_PATH, F_OK) )
	{		
		memset(szDesFilePath, 0, sizeof(szDesFilePath));
		sprintf(szDesFilePath, "/mnt/%s/data/Photo", UDISK_UP_PATH);
		PRTMSG(MSG_DBG, "szDesFilePaht: %s\n", szDesFilePath);
		
		if( 0 != DirCopy(PHOTO_SAVE_PATH, szDesFilePath) )
		{
			PRTMSG(MSG_DBG, "copy %s succ!\n", PHOTO_SAVE_PATH);
		}
		else
		{
			PRTMSG(MSG_DBG, "copy %s failed!\n", PHOTO_SAVE_PATH);
		}
	}
	
	// ��Ƶ�ļ�
	if( 0 == access(VIDEO_SAVE_PATH, F_OK) )
	{ 		
		memset(szDesFilePath, 0, sizeof(szDesFilePath));
		sprintf(szDesFilePath, "/mnt/%s/data/Video/", UDISK_UP_PATH);
		PRTMSG(MSG_DBG, "szDesFilePaht: %s\n", szDesFilePath);
		
		if( 0 != DirCopy(VIDEO_SAVE_PATH, szDesFilePath) )
		{
			PRTMSG(MSG_DBG, "copy %s succ!\n", VIDEO_SAVE_PATH);
		}
		else
		{
			PRTMSG(MSG_DBG, "copy %s failed!\n", VIDEO_SAVE_PATH);
		}
	}
	
	copying = false;
	g_DataOutPut = false;
	show_diaodu("���ݵ������");
}

//-----------------------------------------------------------------------------------------------------------------
// �򿪵������Ĵ���
// ����: �򿪳ɹ�����ʧ��
// bool CDiaodu::ComOpen()
// {
// 	int iResult;
// 	int i;
// 	struct termios options;
// 	const int COMOPEN_TRYMAXTIMES = 5;
// 
// 	for( i = 0; i < COMOPEN_TRYMAXTIMES; i ++ )
// 	{
// //		m_iComPort = open("/dev/ttyAMA0", O_RDWR );//| O_NONBLOCK);
// 		m_iComPort = open("/dev/ttySIM0", O_RDWR );// | O_NONBLOCK);
// 		if( -1 != m_iComPort )
// 		{
// 			PRTMSG(MSG_ERR,"Open diaodu com succ!\n");
// 			break;
// 		}
// 		
// 		PRTMSG(MSG_ERR,"Open diaodu com fail!\n");
// 		sleep(1);
// 	}
// 	
// 	if(tcgetattr(m_iComPort, &options) != 0)
// 	{
// 		perror("GetSerialAttr");
// 		return false;
// 	}
// 	
// 	options.c_iflag &= ~(IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL|IUCLC|IXON|IXOFF|IXANY); 
// 	options.c_lflag &= ~(ECHO|ECHONL|ISIG|IEXTEN|ICANON);
// 	options.c_oflag &= ~OPOST;
// 	
// 	cfsetispeed(&options,B9600);	//���ò����ʣ����������ò�����Ϊ9600
// 	cfsetospeed(&options,B9600);
// 	
// 	if (tcsetattr(m_iComPort,TCSANOW,&options) != 0)   
// 	{ 
// 		perror("Set com Attr"); 
// 		return false;
// 	}
// 	
// 	PRTMSG(MSG_NOR,"Diaodu com open Succ!\n"); 
// 	return true;
// }
// 
// 
// //-----------------------------------------------------------------------------------------------------------------
// // �رմ���
// void CDiaodu::ComClose()
// {
// 	if(-1 != m_iComPort)	
// 	{
// 		close(m_iComPort);
// 		m_iComPort = -1;
// 	}
// }
// 
// //-----------------------------------------------------------------------------------------------------------------
// // д���ں���
// // data: ��д���ݵ�ָ��
// // len:  ��д���ݵĳ���
// // ����: ʵ��д���ֽ��� 
// int CDiaodu::ComWrite(char *v_szBuf, int v_iLen)
// {
// #if USE_COMBUS == 1
// 	DataPush(v_szBuf, (DWORD)v_iLen, DEV_DIAODU, DEV_QIAN, LV2);
// 	return v_iLen;
// #endif
// 
// 	int iWrited = 0;	// ���ڷ��������ݳ���	
// 	iWrited = write(m_iComPort, v_szBuf, v_iLen);
// 	return iWrited;
// }
// 
// void CDiaodu::AnalyseComFrame(char *v_szBuf, int v_iLen)
// {
// 	int iRet = 0;
// 	if( !v_szBuf || v_iLen <= 0 ) 
// 		return ;
// 	
// 	static int i7ECount = 0;
// 	
// 	for( int i = 0; i < v_iLen; i ++ )
// 	{
// 		if( 0x7e == v_szBuf[ i ] )
// 		{
// 			++ i7ECount;
// 			
// 			if( 0 == i7ECount % 2 ) // ���õ�һ֡
// 			{
// 				i7ECount = 0;
// 
// 				// ת��
// 				m_iComFrameLen = DetranData(m_szComFrameBuf, m_iComFrameLen);
// 
// 				// ����У��ͣ�У����ȷ��������ն���
// 				if( check_crc((byte*)m_szComFrameBuf, m_iComFrameLen) )
// 				{	
// 					DWORD dwPacketNum = 0;
// 
// 					iRet = m_objDiaoduReadMng.PushData(LV1, (DWORD)(m_iComFrameLen-1), m_szComFrameBuf+1, dwPacketNum);
// 					if( 0 != iRet )
// 					{
// 						PRTMSG(MSG_ERR, "Push one frame failed:%d\n",iRet);
// 						memset(m_szComFrameBuf, 0, m_iComFrameLen);
// 						m_iComFrameLen = 0;
// 					}					
// 				}
// 				else
// 				{
// 					PRTMSG(MSG_ERR, "Diaodu frame check crc err!\n");
// 					memset(m_szComFrameBuf, 0, m_iComFrameLen);
// 					m_iComFrameLen = 0;
// 				}
// 
// 				//����У����ȷ��񣬶�����������0
// 				memset(m_szComFrameBuf, 0, m_iComFrameLen);
// 				m_iComFrameLen = 0;
// 			}
// 		}
// 		else
// 		{
// 			m_szComFrameBuf[m_iComFrameLen++] = v_szBuf[i];
// 		}
// 	}
// }

int CDiaodu::DetranData(char *v_szBuf, int v_iLen)
{
	char szbuf[1024];
	int  iNewLen = 0;
	
	for( int i = 0; i < v_iLen; )
	{
		if( 0x7d != v_szBuf[ i ] || i == v_iLen - 1 )
		{
			szbuf[ iNewLen ++ ] = v_szBuf[ i++ ];
		}
		else
		{
			if( 1 == v_szBuf[ i + 1 ] )
			{
				szbuf[ iNewLen ++ ] = 0x7d;
			}
			else if( 2 == v_szBuf[ i + 1 ] )
			{
				szbuf[ iNewLen ++ ] = 0x7e;
			}
			else
			{
				szbuf[ iNewLen ++ ] = v_szBuf[ i ];
				szbuf[ iNewLen ++ ] = v_szBuf[ i + 1 ];
			}
			i += 2;
		}
	}
	
	memcpy( v_szBuf, szbuf, iNewLen );

	return iNewLen;
}

void CDiaodu::DealDiaoduFrame(char *v_szBuf, int v_iLen)
{
	DWORD dwPktNum;
	
	switch (v_szBuf[0])
	{
		case 0x01:	//0x01��ʲô��
			break;
	
			// һ��ҵ�� -----------------------------------------------
		case 0x03:	//����ǰGPSλ������
			{
				GPSDATA gps(0);
				GetCurGps( &gps, sizeof(gps), GPS_LSTVALID);
				
				Frm04 frm;
				frm.gps.valid = (gps.valid=='V') ? (0x60) : (0x40);
	
				frm.gps.lat[0] = (byte)(DWORD)gps.latitude;
				frm.gps.lat[1] = (byte)(((DWORD)(gps.latitude*100))%100);
				frm.gps.lat[2] = (byte)(((DWORD)(gps.latitude*10000))%100);
				frm.gps.lat[3] = (byte)(((DWORD)(gps.latitude*1000000))%100);
				frm.gps.lon[0] = (byte)(DWORD)gps.longitude;
				frm.gps.lon[1] = (byte)(((DWORD)(gps.longitude*100))%100);
				frm.gps.lon[2] = (byte)(((DWORD)(gps.longitude*10000))%100);
				frm.gps.lon[3] = (byte)(((DWORD)(gps.longitude*1000000))%100);
	
				frm.gps.speed = (byte)(gps.speed/1.852);	//ת��Ϊ����/ʱ
				frm.gps.dir = (byte)(int)gps.direction;
	
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			break;
			
		case 0x05:	//���󱾻��ֻ���
			{
				tag1PComuCfg cfg;
				GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) );
				
				char frm[100] = {0};
				frm[0] = 0x06;
				memcpy(frm+1, cfg.m_szTel, min(15,strlen(cfg.m_szTel)) );
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, strlen(frm), (char*)&frm, dwPktNum);
			}
			break;
			
		case 0x07:	//����ǰʱ��
			{
				struct tm pCurrentTime;
				G_GetTime(&pCurrentTime);

				Frm08 frm;
				frm.year = (byte)(pCurrentTime.tm_year +1900 - 2000);
				frm.month = (byte)(pCurrentTime.tm_mon+1);//�·�0~~11
				frm.day = (byte)pCurrentTime.tm_mday;
				frm.hour = (byte)pCurrentTime.tm_hour;
				frm.minute = (byte)pCurrentTime.tm_min;
				frm.second = (byte)pCurrentTime.tm_sec;
	
				//PRTMSG(MSG_DBG, "frm.year = %d, pCurrentTime->tm_year = %d\n", frm.year,pCurrentTime->tm_year);			
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			break;
			
		case 0x09:	//����ǰ���س�״̬
			{	
				Frm0A frm;
				unsigned char uszResult;
#if VEHICLE_TYPE == VEHICLE_M || VEHICLE_TYPE == VEHICLE_V8
				IOGet((byte)IOS_JIJIA, &uszResult);
				frm.full = (IO_JIJIA_HEAVY == uszResult) ? 0x01 : 0x00;
#endif
#if VEHICLE_TYPE == VEHICLE_M2
				IOGet((byte)IOS_BDOOR, &uszResult);
				frm.full = (IO_BDOOR_OPEN == uszResult) ? 0x01 : 0x00;
#endif
	
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			break;
			
		case 0x0B:	//�����ѯ��¼����״̬
			DataPush( v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, LV2);
			break;
			
			
			// �绰ҵ�� -----------------------------------------------
		case 0x10:	//����DTMF����
		case 0x12:	//��������		
		case 0x14:	//ժ������	
		case 0x16:	//�һ�����
		case 0x18:	//����ָʾӦ��
		case 0x20:	//��ָͨʾӦ��
		case 0x22:	//����ָʾӦ��
		case 0x24:	//�ź�ǿ������
		case 0x26:	//��������л�����
		case 0x28:	//������������
			g_objPhoneRecvDataMng.PushData((BYTE)LV1, v_iLen, v_szBuf, dwPktNum);
			break;
			
			// ����Ϣҵ�� -----------------------------------------------
		case 0x30:	//�����Ͷ���Ϣ
			//show_msg( "���Ͷ���Ϣ%x:%60s", pdata[1], pdata+2 );
		case 0x32:	//���ն���ϢָʾӦ��
			g_objPhoneRecvDataMng.PushData((BYTE)LV1, v_iLen, v_szBuf, dwPktNum);
			break;
			
			// �������� -------------------------------------------------
		case 0x41:	//���󱨾�����
		case 0x44:	//�������Խ����������Ӧ��
			DataPush( v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
			
			
			// ����������ѧϰ -------------------------------------------
		case 0x50:	//������������������
		case 0x52:	//����������ѧϰ����
		case 0x54:	//ֹͣ����������ѧϰ����
		case 0x56:	//������ѧϰ�����������Ӧ��
		case 0x58:	//��������������
		case 0x5A:	//ֹͣ��������������
		case 0x5C:	//���������Խ��������Ӧ��
			DataPush( v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
	
			
			// �г���¼�� ------------------------------------------------
		case 0x61:	//�����ٶ�
		case 0x63:	//����˾����¼Ӧ��
		case 0x65:	//����˾�����
		case 0x67:	//�������ش���������
		case 0x69:	//���󴫸���״̬
		case 0x6B:	//�����ı���Ϣ��ʾӦ��
		case 0x6C:	//˾���ǳ�ָʾӦ��
		case 0x6D:	//��������ϵ��ָ��
		case 0x6F:	//��������ָ��
			DataPush( v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
			
			
			// ����ͨ��	--------------------------------------------------
		case 0x70:	//�����͵�������
		case 0x71:	//���͵���ָʾӦ��
			DataPush( v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
			
	/*	case 0x73:	//������ϢTTS��������
			PRTMSG(MSG_DBG,"Diaodu: Rcv 0x73!\n");
			break;*/
			
			// ����ҵ��	--------------------------------------------------
		case 0x7A:	//������Ϣ
		case 0x7B:	//���ٽ������
			DataPush( v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
			
			
			// ��������	--------------------------------------------------
		case 0x80:	//�����ͽ�������
		case 0x82:	//�������Ľ���������ӦӦ��
		case 0x84:	//������ȡ����������
		case 0x86:	//��������ȡ������������ӦӦ��
			{
				DataPush( v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
				break;
			}
		
#if USE_AUTORPTSTATION == 1
		case 0x8e:
			{
				DataPush( v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			}
			break;
#endif
		
		// GPRS��������	--------------------------------------------------
		case 0xA1:	//����GPRS����
			{
				char frm[1024] = {0};
				byte len = 0;
				int p = 0;
				char str[100] = {0};
				
				tag1PComuCfg cfg;
				GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) );
				
				tag1QIpCfg cfg2[2];
				GetImpCfg( &cfg2, sizeof(cfg2), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg2) );
				
				tag1LComuCfg cfg3;
				GetImpCfg( &cfg3, sizeof(cfg3), offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg3) );
				
				frm[p++] = (char)0xA0;	//��������
				frm[p++] = (char)0x01;	//Ӧ������
				
				// �ֻ�����
				frm[p++] = min( 15, strlen(cfg.m_szTel) );		
				memcpy(frm+p, cfg.m_szTel, frm[p-1]);
				p+=frm[p-1];
				
				// APN
				frm[p++] = min( 19, strlen(cfg2[0].m_szApn) );		
				memcpy(frm+p, cfg2[0].m_szApn, frm[p-1]);
				p+=frm[p-1];
				
				// ǧ���� TCP IP 1
				frm[p++] = strlen(inet_ntoa(*(in_addr*)&cfg2[0].m_ulQianTcpIP));
				memcpy(frm+p, inet_ntoa(*(in_addr*)&cfg2[0].m_ulQianTcpIP), frm[p-1]);
				p+=frm[p-1];
				
				// ǧ���� TCP PORT 1
				sprintf(str, "%d", ntohs( cfg2[0].m_usQianTcpPort) );
				frm[p++] = strlen(str);
				memcpy(frm+p, str, frm[p-1]);
				p+=frm[p-1];
				
				// ǧ���� TCP IP 2
				frm[p++] = strlen(inet_ntoa(*(in_addr*)&cfg2[1].m_ulQianTcpIP));
				memcpy(frm+p, inet_ntoa(*(in_addr*)&cfg2[1].m_ulQianTcpIP), frm[p-1]);
				p+=frm[p-1];
				
				// ǧ���� TCP PORT 2
				sprintf(str, "%d", ntohs( cfg2[1].m_usQianTcpPort) );
				frm[p++] = strlen(str);
				memcpy(frm+p, str, frm[p-1]);
				p+=frm[p-1];
				
				// ǧ���� UDP IP 1
				frm[p++] = strlen(inet_ntoa(*(in_addr*)&cfg2[0].m_ulQianUdpIP));
				memcpy(frm+p, inet_ntoa(*(in_addr*)&cfg2[0].m_ulQianUdpIP), frm[p-1]);
				p+=frm[p-1];
				
				// ǧ���� UDP PORT 1
				sprintf(str, "%d", ntohs( cfg2[0].m_usQianUdpPort) );
				frm[p++] = strlen(str);
				memcpy(frm+p, str, frm[p-1]);
				p+=frm[p-1];
				
				// ǧ���� UDP IP 2
				//frm[p++] = strlen(inet_ntoa(*(in_addr*)&cfg2[1].m_ulQianUdpIP));
				//memcpy(frm+p, inet_ntoa(*(in_addr*)&cfg2[1].m_ulQianUdpIP), frm[p-1]);
				frm[p++] = strlen(inet_ntoa(*(in_addr*)&cfg2[0].m_ulVUdpIP)); // ��ʱ��Ϊ��Ƶ�����ϴ�UDP����ʹ��
				memcpy(frm+p, inet_ntoa(*(in_addr*)&cfg2[0].m_ulVUdpIP), frm[p-1]);
				p+=frm[p-1];
				
				// ǧ���� UDP PORT 2
				//sprintf(str, "%d", ntohs( cfg2[1].m_usQianUdpPort) );
				sprintf(str, "%d", ntohs( cfg2[0].m_usVUdpPort) ); // ��ʱ��Ϊ��Ƶ�����ϴ�UDP����ʹ��
				frm[p++] = strlen(str);
				memcpy(frm+p, str, frm[p-1]);
				p+=frm[p-1];
				
				// ��ý�� UDP IP 1
				frm[p++] = strlen(inet_ntoa(*(in_addr*)&cfg3.m_ulLiuIP));
				memcpy(frm+p, inet_ntoa(*(in_addr*)&cfg3.m_ulLiuIP), frm[p-1]);
				p+=frm[p-1];
				
				// ��ý�� UDP PORT 1
				sprintf(str, "%d", ntohs(cfg3.m_usLiuPort) );
				frm[p++] = strlen(str);
				memcpy(frm+p, str, frm[p-1]);
				p+=frm[p-1];
				
				// ��ý�� UDP IP 2
				frm[p++] = strlen(inet_ntoa(*(in_addr*)&cfg3.m_ulLiuIP2));
				memcpy(frm+p, inet_ntoa(*(in_addr*)&cfg3.m_ulLiuIP2), frm[p-1]);
				p+=frm[p-1];
				
				// ��ý�� UDP PORT 2
				sprintf(str, "%d", ntohs(cfg3.m_usLiuPort2) );
				frm[p++] = strlen(str);
				memcpy(frm+p, str, frm[p-1]);
				p+=frm[p-1];
				
				// ǧ��������
				frm[p++] = strlen(ALL_PASS);
				memcpy(frm+p, ALL_PASS, frm[p-1]);
				p+=frm[p-1];
				
				// ��ý������
				frm[p++] = strlen(LIU_PASS);
				memcpy(frm+p, LIU_PASS, frm[p-1]);
				p+=frm[p-1];
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, p, frm, dwPktNum);			
			}
			break;
			
		case 0xA3:	//�����ֻ���������
			{
				char tel[100] = {0};
				memcpy(tel, v_szBuf+2, v_szBuf[1]);
				
				FrmA2 frm;
				
				tag1PComuCfg cfg;
				GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) );
				memset(cfg.m_szTel, 0, sizeof(cfg.m_szTel) );
				memcpy(cfg.m_szTel, tel, min(15,strlen(tel)) );
				
				frm.reply = 0x02;
				if(strlen(tel)!=0) 
				{ 
					if(!SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) ))	
						frm.reply = 0x01; 
				}
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
				
				Frm35 frm1;		//֪ͨQianExe�ֻ�����ı���
				frm1.reply = 0x01;
				DataPush( (char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);

				char buf[2] = {0x01, 0x03};		//֪ͨUpdateExe�ֻ�����ı���
 				DataPush(buf, 2, DEV_DIAODU, DEV_UPDATE, 2);
 				
				tag2DDvrCfg objMvrCfg;
		    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
				memset(objMvrCfg.IpPortPara.m_szTelNum, 0, sizeof(objMvrCfg.IpPortPara.m_szTelNum) );
				memcpy(objMvrCfg.IpPortPara.m_szTelNum, tel, min(15,strlen(tel)) );
				SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
				char by = 0x0c;//֪ͨDvrExe�ֻ�����ı���
				DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
			}
			break;
			
		case 0xA5:	//����APN����
			{
				char apn[100] = {0};
				memcpy(apn, v_szBuf+2, v_szBuf[1]);
				
				FrmA4 frm;
				
				tag1QIpCfg cfg[2];
				GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) );
				memset(cfg[0].m_szApn, 0, sizeof(cfg[0].m_szApn) );
				memcpy(cfg[0].m_szApn, apn, min(19,strlen(apn)) );
				
				frm.reply = 0x02;
				if(strlen(apn)!=0) 
				{ 
					if(!SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ))	
						frm.reply = 0x01; 
				}
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
				
				tag2DDvrCfg objMvrCfg;
		    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
				memset(objMvrCfg.IpPortPara.m_szApn, 0, sizeof(objMvrCfg.IpPortPara.m_szApn) );
				memcpy(objMvrCfg.IpPortPara.m_szApn, apn, min(19,strlen(apn)) );
				SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
				char by = 0x0c;//֪ͨDvrExe APN�ı���
				DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
			}
			break;
			
		case 0xA7:	//����IP��ַ����
			{
				tag1QIpCfg cfg[2];
				GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) );
				
				tag1LComuCfg cfg2;
				GetImpCfg( &cfg2, sizeof(cfg2), offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg2) );
				
				char str[100] = {0};
				memcpy(str, v_szBuf+3, v_szBuf[2]);
				
				FrmA6 frm;
				frm.flag = v_szBuf[1];
				
				if(frm.flag == 0)
				{
					cfg[0].m_ulQianTcpIP = inet_addr(str);
					frm.reply = 0x02;
					if(inet_addr(str)!=0)
					{ 
						if( !SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ) )	
						{
							frm.reply = 0x01; 
							
							Frm35 frm1;		//֪ͨQianExe IP�ı���
							frm1.reply = 0x01;
							DataPush((char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);
							
							tag2DDvrCfg objMvrCfg;
					    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
							objMvrCfg.IpPortPara.m_ulCommIP = inet_addr(str);
							SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));	
							char by = 0x0c;//֪ͨDvrExe IP�ı���
							DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
						}
					}
				} 
				else if(frm.flag == 1)
				{
					cfg[1].m_ulQianTcpIP = inet_addr(str);
					frm.reply = 0x02;
					
					if(inet_addr(str)!=0)
					{ 
						if( !SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ) )	
							frm.reply = 0x01; 
					}
				}
				else if(frm.flag == 2)
				{
					cfg[0].m_ulQianUdpIP = inet_addr(str);
					frm.reply = 0x02;
					
					if(inet_addr(str)!=0)
					{ 
						if( !SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ) )	
						{
							frm.reply = 0x01; 

							Frm35 frm1;		//UDP IP�ı���
							frm1.reply = 0x01;
							DataPush((char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);
							
							tag2DDvrCfg objMvrCfg;
					    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
							objMvrCfg.IpPortPara.m_ulPhotoIP = inet_addr(str);
							SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));	
							char by = 0x0c;//֪ͨDvrExe IP�ı���
							DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
						}
					}					
				} 
				else if(frm.flag  == 3) // �������ϵ�ǧ���� UDP IP 2
				{
					cfg[1].m_ulQianUdpIP = inet_addr(str);
					frm.reply = 0x02;

					// ��ʱʹ�ô����޸�King���ĵ���Ƶ����, �Ժ���õ�������Ҫ���Ӵ������ò˵�
					cfg[0].m_ulVUdpIP = inet_addr(str);
					
					if(inet_addr(str)!=0) 
					{ 
						if( !SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ) )	
						{
							frm.reply = 0x01; 

							Frm35 frm1;		//UDP IP�ı���
							frm1.reply = 0x01;
							DataPush((char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);
							
							tag2DDvrCfg objMvrCfg;
					    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
							objMvrCfg.IpPortPara.m_ulVideoIP = inet_addr(str);
							SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));	
							char by = 0x0c;//֪ͨDvrExe IP�ı���
							DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
						}
					}
				} 
				else if(frm.flag == 4) // ��ý����������IP
				{
					cfg2.m_ulLiuIP = inet_addr(str);
					frm.reply = 0x02;
					if(inet_addr(str)!=0)
					{ 
						if( !SetImpCfg(&cfg2, offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg2) ) )	
						{
							frm.reply = 0x01; 

							char buf[2] = {0x01, 0x03};		//֪ͨUpdateExe IP�ı���
 							DataPush(buf, 2, DEV_DIAODU, DEV_UPDATE, 2);	
 							
 							tag2DDvrCfg objMvrCfg;
					    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
							objMvrCfg.IpPortPara.m_ulUpdateIP = inet_addr(str);
							SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));	
 							char by = 0x0c;//֪ͨDvrExe IP�ı���
							DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
						}
					}		
				} 
				else if(frm.flag == 5) // ����ƽ̨
				{
					cfg2.m_ulLiuIP2 = inet_addr(str);
					frm.reply = 0x02;
					
					if(inet_addr(str)!=0)
					{ 
						if( !SetImpCfg(&cfg2, offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg2) ) )	
						{
							frm.reply = 0x01; 

							//֪ͨQianExe IP�ı��� (��)
						}
					}
				}

				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);	
			}
			break;
			
		case 0xA9:	//���ö˿ں�
			{	
				tag1QIpCfg cfg[2];
				GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) );
				
				tag1LComuCfg cfg2;
				GetImpCfg( &cfg2, sizeof(cfg2), offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg2) );
				
				char str[100] = {0};
				memcpy(str, v_szBuf+3, v_szBuf[2]);
				
				FrmA8 frm;
				frm.flag = v_szBuf[1];
				
				if(frm.flag == 0)
				{
					cfg[0].m_usQianTcpPort = htons( (ushort)atol(str) );
					frm.reply = 0x02;
					
					if(atol(str)!=0)
					{ 
						if( !SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ) )	
						{
							frm.reply = 0x01; 

							Frm35 frm1;		//֪ͨǧ���۶˿ڸı���
							frm1.reply = 0x01;
							DataPush((char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);
							
							tag2DDvrCfg objMvrCfg;
					    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
							objMvrCfg.IpPortPara.m_usCommPort = htons((ushort)atol(str));
							SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));	
							char by = 0x0c;//֪ͨDvrExe �˿ڸı���
							DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
						}
					}					
				} 
				else if(frm.flag == 1) 
				{
					cfg[1].m_usQianTcpPort = htons( (ushort)atol(str) );
					frm.reply = 0x02;
					
					if(atol(str)!=0) 
					{ 
						if( !SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ) )	
							frm.reply = 0x01; 
					}
				} 
				else if(frm.flag == 2) 
				{
					cfg[0].m_usQianUdpPort = htons( (ushort)atol(str) );
					frm.reply = 0x02;
					
					if(atol(str)!=0) 
					{ 
						if( !SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ) )	
						{
							frm.reply = 0x01;

							Frm35 frm1;		//֪ͨǧ���۶˿ڸı���
							frm1.reply = 0x01;
							DataPush((char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);
							
							tag2DDvrCfg objMvrCfg;
					    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
							objMvrCfg.IpPortPara.m_usPhotoPort = htons((ushort)atol(str));
							SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));	
							char by = 0x0c;//֪ͨDvrExe �˿ڸı���
							DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
						}
					}					
				} 
				else if(frm.flag == 3) // �������ϵ�ǧ���� UDP port 2
				{
					cfg[1].m_usQianUdpPort = htons( (ushort)atol(str) );
					frm.reply = 0x02;

					// ��ʱʹ�ô����޸�King���ĵ���Ƶ����, �Ժ���õ�������Ҫ���Ӵ������ò˵�
					cfg[0].m_usVUdpPort = htons( (ushort)atol(str) );

					if(atol(str)!=0)
					{ 
						if( !SetImpCfg(&cfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) ) )	
						{
							frm.reply = 0x01; 

							Frm35 frm1;		//֪ͨǧ���۶˿ڸı���
							frm1.reply = 0x01;
							DataPush((char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);
							
							tag2DDvrCfg objMvrCfg;
					    GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
							objMvrCfg.IpPortPara.m_usVideoPort = htons((ushort)atol(str));
							SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));	
							char by = 0x0c;//֪ͨDvrExe �˿ڸı���
							DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
						}
					}
				} 
				else if(frm.flag == 4) // ��ý����������
				{
					cfg2.m_usLiuPort = htons( (ushort)atol(str) );
					frm.reply = 0x02;
					
					if(atol(str)!=0)
					{ 
						if( !SetImpCfg(&cfg2, offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg2) ) )	
							frm.reply = 0x01; 
					}

					char buf[2] = {0x01, 0x03};		//֪ͨ��ý��
 					DataPush(buf, 2, DEV_DIAODU, DEV_UPDATE, 2);
 					
 					tag2DDvrCfg objMvrCfg;
					GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
					objMvrCfg.IpPortPara.m_usUpdatePort = htons((ushort)atol(str));
					SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));	
 					char by = 0x0c;//֪ͨDvrExe �˿ڸı���
					DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
				} 
				else if(frm.flag == 5) // ����ƽ̨
				{
					cfg2.m_usLiuPort2 = htons( (ushort)atol(str) );
					frm.reply = 0x02;
					if(atol(str)!=0)
					{ 
						if( !SetImpCfg(&cfg2, offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg2) ) )	
							frm.reply = 0x01; 
					}
				}

				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);	
			}
			break;
			
		case 0xAB:	//����GPRS����
			{
				FrmAA frm;
				frm.reply = 0x01;	//�Ѿ�����
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);

				Frm35 frm1;	
				frm1.reply = 0x01;
				DataPush((char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);
			}
			break;
			
		case 0xAD:	//��̨״̬��ѯ
			{
				char frm[1024] = {0};
				int len = 0;
				
				frm[len++] = (char)0xE9;
				frm[len++] = (char)0x00;
				
				GPSDATA gps(0);
				GetCurGps( &gps, sizeof(gps), GPS_LSTVALID );
				
				tag1PComuCfg cfg;
				GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg) );
				
				tag2QServCodeCfg cfg2;
				GetSecCfg( &cfg2, sizeof(cfg2), offsetof(tagSecondCfg, m_uni2QServCodeCfg.m_obj2QServCodeCfg), sizeof(cfg2) );
				
				char str[200] = {0};
				
				sprintf(str, "%s:%.21s,", LANG_SMSCENT, cfg.m_szSmsCentID);
				strcpy(frm+len, str);
				len += strlen(str);
				
				sprintf(str, "%s:%.14s,", LANG_SPECCODE, cfg2.m_szQianSmsTel);
				strcpy(frm+len, str);
				len += strlen(str);
				
				if( NETWORK_TYPE==NETWORK_GSM )
					sprintf(str, "Network:%s,", 2 == g_objPhoneGsm.GetPhoneState() ? LANG_ONLINE : LANG_OFFLINE );
				else if( NETWORK_TYPE==NETWORK_TD )
					sprintf(str, "Network:%s,", 2 == g_objPhoneTd.GetPhoneState() ? LANG_ONLINE : LANG_OFFLINE );
				else if( NETWORK_TYPE==NETWORK_EVDO )
					sprintf(str, "Network:%s,", 2 == g_objPhoneEvdo.GetPhoneState() ? LANG_ONLINE : LANG_OFFLINE );
				else if( NETWORK_TYPE==NETWORK_WCDMA )
					sprintf(str, "Network:%s,", 2 == g_objPhoneWcdma.GetPhoneState() ? LANG_ONLINE : LANG_OFFLINE );
				
				
				strcpy(frm+len, str);
				len += strlen(str);
				
				sprintf(str, "GPS:%s", 'A'==gps.valid ? LANG_VALID : LANG_INVALID );
				strcpy(frm+len, str);
				len += strlen(str);
				
				frm[1] = len - 2;	// ����
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, len, frm, dwPktNum);
			}
			break;
			
			
			// �û�ע��	--------------------------------------------------
		case 0x90:	//�û�ע������
		case 0x92:	//�û�ע������
		case 0x94:	//�յ��û��б�
			DataPush(v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
			
//  		case 0xAF:	//��ý�����������������IP���˿ڲ��������ϵ�ͬһ�ṹ���ڣ�
//  			break;

		case 0xAF:	//���ò����û�������������
			{
				char userName[100] = {0};
				char passWord[100] = {0};
				memcpy(userName, v_szBuf+2, v_szBuf[1]);
				memcpy(passWord, v_szBuf+2+v_szBuf[1]+1, v_szBuf[(2+v_szBuf[1])]);
				
				FrmAE frm;

				tag1PComuCfg objComuCfg;
				GetImpCfg( &objComuCfg, sizeof(objComuCfg),	offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(objComuCfg) );
	
// 				tag1QIpCfg cfg[2];
// 				GetImpCfg( &cfg, sizeof(cfg), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg) );
 				memset(objComuCfg.m_szCdmaUserName, 0, sizeof(objComuCfg.m_szCdmaUserName) );
				memcpy(objComuCfg.m_szCdmaUserName, userName, min(sizeof(objComuCfg.m_szCdmaUserName),strlen(userName)) );
				memset(objComuCfg.m_szCdmaPassWord, 0, sizeof(objComuCfg.m_szCdmaUserName) );
				memcpy(objComuCfg.m_szCdmaPassWord, passWord, min(sizeof(objComuCfg.m_szCdmaUserName),strlen(passWord)) );
				
				frm.reply = 0x02;
				if( (strlen(userName)!=0) && (strlen(passWord)!=0) ) { 
					if(!SetImpCfg(&objComuCfg, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(objComuCfg) ))	
						frm.reply = 0x01; 
				}
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
				if( NETWORK_TYPE==NETWORK_GSM )
					g_objPhoneGsm.ResetPhone();
				else if( NETWORK_TYPE==NETWORK_TD )
					g_objPhoneTd.ResetPhone();
				else if( NETWORK_TYPE==NETWORK_EVDO )
					g_objPhoneEvdo.ResetPhone();
				else if( NETWORK_TYPE==NETWORK_WCDMA )
					g_objPhoneWcdma.ResetPhone();
			}
			break;

		case 0xb1: // ����GSMģ���IMEI����
			{
				char frm[100] = { 0 };
				frm[0] = char(0xb0);
				
				char szImei[30] = { 0 };
				if( NETWORK_TYPE==NETWORK_GSM )
					g_objPhoneGsm.GetPhoneIMEI( szImei, sizeof(szImei) );
				else if( NETWORK_TYPE==NETWORK_TD )
					g_objPhoneTd.GetPhoneIMEI( szImei, sizeof(szImei) );
				else if( NETWORK_TYPE==NETWORK_EVDO )
					g_objPhoneEvdo.GetPhoneIMEI( szImei, sizeof(szImei) );
				else if( NETWORK_TYPE==NETWORK_WCDMA )
					g_objPhoneWcdma.GetPhoneIMEI( szImei, sizeof(szImei) );
				strncpy( frm + 1, szImei, sizeof(frm) - 2 );
				
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, strlen(frm), frm, dwPktNum);
			}
			break;
			
			
			// ����ҵ�� ----------------------------------------------------------
		case 0xC1: // �Ƽ��س��ź���������	�Ƽ��س��ź�����(1) 00 ��ʾ�͵�ƽ   01 ��ʾ�ߵ�ƽ, ���ౣ��
			{
				char frm[ 20 ] = { 0 };
				frm[0] = BYTE(0xc2);
				frm[1] = 1;
				
				if( v_iLen < 3 )
				{
					PRTMSG(MSG_ERR, "Err Frm:%02x\n", BYTE(v_szBuf[0]) );
					goto DEALDIAODU_C1_FAIL;
				}
				else
				{
					if( 0 == v_szBuf[1] )
					{
						PRTMSG(MSG_DBG, "Jijia Valid Cfg: Low\n" );
					}
					else if( 1 == v_szBuf[1] )
					{
						PRTMSG(MSG_DBG, "Jijia Valid Cfg: High\n" );
					}
					else
					{
						PRTMSG(MSG_ERR, "Jijia Valid Cfg: Data Err\n" );
						goto DEALDIAODU_C1_FAIL;
					}
				}
				
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 2, frm, dwPktNum);
				break;
				
DEALDIAODU_C1_FAIL:
				frm[1] = 0;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 2, frm, dwPktNum);
			}
			break;
			
		case 0xC3: // �п��ź���������	�п��ź����(1)+�п��ź�����(1)
			// �ź����: 01��ʾ�п���Ч�źţ�02��ʾ�п�ȡ���źţ���������
			// �ź�����: 00 ��ʾ�͵�ƽ   01 ��ʾ�ߵ�ƽ, ���ౣ��
			{
				char frm[ 20 ] = { 0 };
				frm[0] = BYTE(0xc4);
				frm[1] = v_szBuf[1];
				frm[2] = 1;
				
				if( v_iLen < 4 )
				{
					PRTMSG(MSG_ERR, "Err Frm:%02x", BYTE(v_szBuf[0]) );
					goto DEALDIAODU_C3_FAIL;
				}
				else
				{
					if( 1 == v_szBuf[1] )
					{
						if( 0 == v_szBuf[2] )
						{
							PRTMSG(MSG_DBG, "CentCtrl Valid Cfg: Low\n" );
						}
						else if( 1 == v_szBuf[2] )
						{
							PRTMSG(MSG_DBG, "CentCtrl Valid Cfg: High\n" );
						}
						else
						{
							PRTMSG(MSG_ERR, "CentCtrl Valid Cfg: Data Err\n" );
							goto DEALDIAODU_C3_FAIL;
						}
						
					}
					else if( 2 == v_szBuf[1] )
					{
						if( 0 == v_szBuf[2] )
						{
							PRTMSG(MSG_DBG, "CentCtrl Invalid Cfg: Low\n" );
						}
						else if( 1 == v_szBuf[2] )
						{
							PRTMSG(MSG_DBG, "CentCtrl Invalid Cfg: High\n" );
						}
						else
						{
							PRTMSG(MSG_ERR, "CentCtrl Invalid Cfg: Data Err\n" );
							goto DEALDIAODU_C3_FAIL;
						}
					}
					else
					{
						PRTMSG(MSG_ERR, "Err CentCtrl Frm: %02x\n", BYTE(v_szBuf[1]) );
						goto DEALDIAODU_C3_FAIL;
					}
				}
				
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 3, frm, dwPktNum);
				break;
				
DEALDIAODU_C3_FAIL:
				frm[2] = 0;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 3, frm, dwPktNum);
			}
			break;
			
		case 0xC5: // �������ź���������	�������ź�����(1) 00 ��ʾ�͵�ƽ   01 ��ʾ�ߵ�ƽ, ���ౣ��
			{
				char frm[ 20 ] = { 0 };
				frm[0] = BYTE(0xc6);
				frm[1] = 1;
				
				if( v_iLen < 3 )
				{
					PRTMSG(MSG_ERR, "Err Frm:%02x\n", BYTE(v_szBuf[0]) );
					goto DEALDIAODU_C5_FAIL;
				}
				else
				{
					if( 0 == v_szBuf[1] )
					{
						PRTMSG(MSG_DBG, "Door Valid Cfg: Low\n" );
					}
					else if( 1 == v_szBuf[1] )
					{
						PRTMSG(MSG_DBG, "Door Valid Cfg: High\n" );
					}
					else
					{
						PRTMSG(MSG_ERR, "Door Valid Cfg: Data Err\n" );
						goto DEALDIAODU_C5_FAIL;
					}
				}
				
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 2, frm, dwPktNum);
				break;
				
DEALDIAODU_C5_FAIL:
				frm[1] = 0;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 2, frm, dwPktNum);
			}
			break;
			
		case 0xC7: // ��ɲ�ź���������	��ɲ�ź�����(1) 00 ��ʾ�͵�ƽ   01 ��ʾ�ߵ�ƽ, ���ౣ��
			{
				char frm[ 20 ] = { 0 };
				frm[0] = BYTE(0xc8);
				frm[1] = 1;
				
				if( v_iLen < 3 )
				{
					PRTMSG(MSG_ERR, "Err Frm:%02x\n", BYTE(v_szBuf[0]) );
					goto DEALDIAODU_C7_FAIL;
				}
				else
				{
					if( 0 == v_szBuf[1] )
					{
						PRTMSG(MSG_DBG, "Shousha Valid Cfg: Low\n" );
					}
					else if( 1 == v_szBuf[1] )
					{
						PRTMSG(MSG_DBG, "Shousha Valid Cfg: High\n" );
					}
					else
					{
						PRTMSG(MSG_ERR, "Shousha Valid Cfg: Data Err\n" );
						goto DEALDIAODU_C7_FAIL;
					}
				}
				
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 2, frm, dwPktNum);
				break;
				
DEALDIAODU_C7_FAIL:
				frm[1] = 0;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 2, frm, dwPktNum);
			}
			break;
			
		case 0xC9: // ��ɲ�ź���������	��ɲ�ź�����(1) 00 ��ʾ�͵�ƽ   01 ��ʾ�ߵ�ƽ, ���ౣ��
			{
				char frm[ 20 ] = { 0 };
				frm[0] = BYTE(0xca);
				frm[1] = 1;
				
				if( v_iLen < 3 )
				{
					PRTMSG(MSG_ERR, "Err Frm:%02x\n", BYTE(v_szBuf[0]) );
					goto DEALDIAODU_C9_FAIL;
				}
				else
				{
					if( 0 == v_szBuf[1] )
					{
						PRTMSG(MSG_DBG, "Jiaosha Valid Cfg: Low\n" );
					}
					else if( 1 == v_szBuf[1] )
					{
						PRTMSG(MSG_DBG, "Jiaosha Valid Cfg: High\n" );
					}
					else
					{
						PRTMSG(MSG_ERR, "Jiaosha Valid Cfg: Data Err\n" );
						goto DEALDIAODU_C9_FAIL;
					}
				}
				
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 2, frm, dwPktNum);
				break;
				
DEALDIAODU_C9_FAIL:
				frm[1] = 0;
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV2, 2, frm, dwPktNum);
			}
			break;
			
		case 0xCD:	//�������ն˰汾��
			{
				char szSoftVer[64] = {0};
				ChkSoftVer(szSoftVer);

				char szSysVer[64] = {0};
				ReadVerFile(szSysVer);

				FrmCC frm;
				sprintf(frm.ver, "%s-%s", szSoftVer, szSysVer);
				
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
			}
			break;
			
		case 0xCF:	//̽ѯ�����ն�Ӧ��
			DataPush(v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
			
			
			// ����ͨ��ҵ�� ----------------------------------------------------------
		case 0xD1:	//���͹�������Ӧ��
		case 0xD3:	//�����͹�����Ӧ
			DataPush(v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
			
			// ����ͷ����ҵ�� -------------------------------------------------------
		case 0xE0:	//����ͷ����ָʾ
			{
				char buf[2] = {0x05, v_szBuf[1]-1};
				DataPush(buf, 2, DEV_DIAODU, DEV_DVR, LV1);
			}
			break;
			
			// ��Ƶ�ļ�����ҵ�� -------------------------------------------------------
		case 0xE1:	//��Ƶ�ļ���������
			{
				char szBuf[2] = {0x8e, 0x10};
				DataPush(szBuf, 2, DEV_DIAODU, DEV_QIAN, LV2);				
			}
			break;
			
			// ���ݵ���ҵ�� -------------------------------------------------------
		case 0xE2:	//���ݵ�������
			{
				pthread_t thDataOutPut;
				pthread_create(&thDataOutPut, NULL, G_DataOutPutThread, (void *)this);
			}
			break;			
			
			// ��̨ID��Կҵ�� ---------------------------------------------------------
 		case 0xE4:	//д��̨ID����Կ  ��̨ID(4)+��̨��Կ(16)+IP��ַ(4)+�˿�(2)+����IP��ַ(4)+���ö˿�(2)
 			break;	//���ݲ�֧�֣�


		case 0xE6:	//��ѯ��̨ID����Կ
 			break;	//���ݲ�֧�֣�			
			

		case 0xEA:	//U��ϵͳ����
			pthread_t thSysUpdate;
			if( access("/mnt/UDisk/part1/image.bin", F_OK) == 0 )
				pthread_create(&thSysUpdate, NULL, G_ImageUpdateThread, (void *)this);
			else
				pthread_create(&thSysUpdate, NULL, G_SysUpdateThread, (void *)this);
			break;
			
			
		case 0xEC:	//���빤�̲˵�����
			{
				char szPhoneMode[100] = {0};
				
				char pass[100] = {0};
				memcpy(pass, v_szBuf+1, v_iLen-1);
				
				static bool debuging = false;
				static bool bSetLine = false;
				static bool bVideoDebug = false;

				// �����Զ���վ��·��
				if( ((v_iLen-2)<16) && bSetLine )
				{
					bSetLine = false;

					int iLen = v_iLen-1;
					BYTE *szBuf = new BYTE[iLen+3];

					szBuf[0] = 0x8e;
					szBuf[1] = 0x99;
					szBuf[2] = iLen;

					for(int i=0;i<iLen;i++)
					{			
						szBuf[3+i] = v_szBuf[1+i];
					}	
					
					DataPush(szBuf, 3+iLen, DEV_DIAODU, DEV_QIAN, LV2);

					delete szBuf; 
					szBuf=NULL;

					//show_diaodu("��·��:%s", v_szBuf+1);

					break;
				}
				
				if(strcmp(pass, ALL_PASS)==0) 
				{
					FrmED frm;
					frm.reply = 0x00;
					g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);
					break;					
				} 
				else if(strcmp(pass, EXP_PASS)==0) // �������ģʽ
				{					
					debuging = true;

					//break;
				} 
				else if(strcmp(pass, "1")==0 && debuging)
				{
					// ֪ͨDvrExe����SD��״̬��ѯ�����������������ʾ
					char buf = 0x07;
					DataPush(&buf, 1, DEV_DIAODU, DEV_DVR, LV1);

					break;
				} 
				else if(strcmp(pass, "2")==0 && debuging)	// ��Ƶ�ļ�����
				{
					pthread_t thDataOutPut;
					pthread_create(&thDataOutPut, NULL, G_VideoOutPutThread, (void *)this);
					
					break;
				}
				else if(strcmp(pass, "3")==0 && debuging)	// ��λ�ֻ�ģ��
				{
					if( NETWORK_TYPE==NETWORK_GSM )
						g_objPhoneGsm.ResetPhone();
					else if( NETWORK_TYPE==NETWORK_TD )
						g_objPhoneTd.ResetPhone();
					else if( NETWORK_TYPE==NETWORK_EVDO )
						g_objPhoneEvdo.ResetPhone();
					else if( NETWORK_TYPE==NETWORK_WCDMA )
						g_objPhoneWcdma.ResetPhone();
					
					break;
				} 
				else if(strcmp(pass, "4")==0 && debuging) // ��ý��������½
				{			
					char buf[2] = {0x01, 0x03};
					DataPush(buf, 2, DEV_DIAODU, DEV_UPDATE, 2);			
					break;
				} 				
				else if(strcmp(pass, "5")==0 && debuging) // ϵͳ��λ
				{						
					G_ResetSystem();	
					break;
				}
				else if(strcmp(pass, "116688")==0 ) // ѡ��վ��·
				{						
					bSetLine = true;
					show_diaodu("��������·��");
					break;
				}
				else if(strcmp(pass, CHOOSE_LC6311_PASS)==0)
				{
					strcpy(szPhoneMode, "LC6311");
				}
				else if(strcmp(pass, CHOOSE_MC703OLD_PASS)==0)
				{
					strcpy(szPhoneMode, "MC703OLD");
				}
				else if(strcmp(pass, CHOOSE_MC703NEW_PASS)==0)
				{
					strcpy(szPhoneMode, "MC703NEW");
				}
				else if(strcmp(pass, CHOOSE_SIM5218_PASS)==0)
				{
					strcpy(szPhoneMode, "SIM5218");
				}
				else if(strcmp(pass, "12345")==0) // ������Ƶ����ģʽ
				{
					show_diaodu("������Ƶ����ģʽ");
					bVideoDebug = true;
					break;
				}
				else if(strcmp(pass, "1")==0 && bVideoDebug) // ����ͨ��1
				{
					show_diaodu("����ͨ��1");

					char buf[] = {0x00,0x7e,0x1b,0x20,0x83,0x68,0x7f,0x00,0x38,0x01,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x20,0x20,0x20,0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x20,0x20,0x20,0x20,0x01,0x05,0x01,0x01,0x02,0x02,0x01,0x01,0x03,0x01,0x7f,0x04,0x01,0x0a,0x05,0x03,0x7f,0x0a,0x7f,0x7f,0x7f,0x7b,0x7e};
					DataPush(buf, sizeof(buf), DEV_SOCK, DEV_QIAN, LV2);
					
					break;
				}
				else if(strcmp(pass, "2")==0 && bVideoDebug) // ����ͨ��1,2
				{
					show_diaodu("����ͨ��1,2");

					char buf[] = {0x00,0x7e,0x15,0x20,0x83,0x68,0x7f,0x00,0x38,0x01,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x20,0x20,0x20,0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x20,0x20,0x20,0x20,0x01,0x05,0x01,0x01,0x02,0x02,0x01,0x03,0x03,0x01,0x7f,0x04,0x01,0x05,0x05,0x03,0x7f,0x0a,0x7f,0x7f,0x7f,0x78,0x7e};
					DataPush(buf, sizeof(buf), DEV_SOCK, DEV_QIAN, LV2);

					break;
				}
				else if(strcmp(pass, "3")==0 && bVideoDebug) // ����ͨ��1,2,3
				{
					show_diaodu("����ͨ��1,2,3");

					char buf[] = {0x00,0x7e,0x17,0x20,0x83,0x68,0x7f,0x00,0x38,0x01,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x20,0x20,0x20,0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x20,0x20,0x20,0x20,0x01,0x05,0x01,0x01,0x02,0x02,0x01,0x07,0x03,0x01,0x7f,0x04,0x01,0x02,0x05,0x03,0x7f,0x0a,0x7f,0x7f,0x7f,0x79,0x7e};
					DataPush(buf, sizeof(buf), DEV_SOCK, DEV_QIAN, LV2);

					break;
				}
				else if(strcmp(pass, "4")==0 && bVideoDebug) // ����ͨ��1,2,3,4
				{
					show_diaodu("����ͨ��1,2,3,4");

					char buf[] = {0x00,0x7e,0xa5,0x20,0x83,0x68,0x7f,0x00,0x38,0x01,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x20,0x20,0x20,0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x20,0x20,0x20,0x20,0x01,0x05,0x01,0x01,0x02,0x02,0x01,0x0f,0x03,0x01,0x7f,0x04,0x01,0x01,0x05,0x03,0x7f,0x0a,0x7f,0x7f,0x7f,0x01,0x7e};
					DataPush(buf, sizeof(buf), DEV_SOCK, DEV_QIAN, LV2);

					break;
				}
				else if(strcmp(pass, "5")==0 && bVideoDebug) // �������
				{
					show_diaodu("�������");
					char buf[] = {0x00,0x7e,0x3c,0x20,0x83,0x68,0x7f,0x00,0x38,0x02,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x20,0x20,0x20,0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x20,0x20,0x20,0x20,0x4c,0x7e};
					DataPush(buf, sizeof(buf), DEV_SOCK, DEV_QIAN, LV2);
					break;
				}
				
				if( strlen(szPhoneMode)>0 )
				{
					tag2DDvrCfg objMvrCfg;
					GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
					strcpy(objMvrCfg.IpPortPara.m_szPhoneMod, szPhoneMode);
					
					if( SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg))==0 )
					{
						show_diaodu("����ֻ�ģ��ɹ�,�豸��������Ч");
						break;
					}
				}
				
				// ��ʾ"�������"					
				FrmED frm;
				frm.reply = 0x01;	//�������
				g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, sizeof(frm), (char*)&frm, dwPktNum);				
			} 
			break;			
			
		case 0xEE:	//���̲˵����Ӳ˵�����
			{
				FrmEE *pfrm = (FrmEE*)v_szBuf;
				switch(pfrm->flag) 
				{
				case 0x00:	//������״̬��ѯ
					{
						char frm[1024] = {0};
						int len = 0;
						
						frm[len++] = (char)0xE9;
						frm[len++] = (char)0x00;
						
						GPSDATA gps(0);
						GetCurGps( &gps, sizeof(gps), GPS_LSTVALID );
						
						unsigned char uszResult;
						IOGet((byte)IOS_ACC, &uszResult );
						build_frm2((IO_ACC_ON == uszResult), "ACC", LANG_VALID, LANG_INVALID);
						
#if VEHICLE_TYPE == VEHICLE_M	
						//���س�/�Ƽ���
						IOGet((byte)IOS_JIJIA, &uszResult );
						build_frm2((IO_JIJIA_HEAVY == uszResult),	LANG_IO_METER, LANG_HEAVY, LANG_EMPTY);
						
						//ǰ�����ź�
						IOGet((byte)IOS_FDOOR, &uszResult );
						build_frm2((IO_FDOOR_OPEN == uszResult),	LANG_IO_FDOOR, LANG_VALID, LANG_INVALID);
						
						//�����ź�
						IOGet((byte)IOS_BDOOR, &uszResult );
						build_frm2((IO_BDOOR_OPEN == uszResult),	LANG_IO_BDOOR, LANG_VALID, LANG_INVALID);
						
						//�ؿ��ź�1
						IOGet((byte)IOS_PASSENGER1, &uszResult );
						build_frm2((IO_PASSENGER1_VALID == uszResult),	LANG_IO_LOAD1, LANG_VALID, LANG_INVALID);
						
						//�ؿ��ź�2
						IOGet((byte)IOS_PASSENGER2, &uszResult );
						build_frm2((IO_PASSENGER2_VALID == uszResult),	LANG_IO_LOAD2, LANG_VALID, LANG_INVALID);
						
						//�ؿ��ź�3
						IOGet((byte)IOS_PASSENGER3, &uszResult );
						build_frm2((IO_PASSENGER3_VALID == uszResult),	LANG_IO_LOAD3, LANG_VALID, LANG_INVALID);
#endif
						
#if VEHICLE_TYPE == VEHICLE_V8
						//���س�/�Ƽ���
						IOGet((byte)IOS_JIJIA, &uszResult );
						build_frm2((IO_JIJIA_HEAVY == uszResult),	LANG_IO_METER, LANG_HEAVY, LANG_EMPTY);
						
						//ǰ�����ź�
						IOGet((byte)IOS_FDOOR, &uszResult );
						build_frm2((IO_FDOOR_OPEN == uszResult),	LANG_IO_FDOOR, LANG_VALID, LANG_INVALID);
						
						// Ƿѹ�ź�
						IOGet((byte)IOS_QIANYA, &uszResult );
						build_frm2((IO_QIANYA_VALID == uszResult), LANG_IO_QIANYA, LANG_VALID, LANG_INVALID);
						
						// ���������ź�
						IOGet((byte)IOS_ELECSPEAKER, &uszResult );
						build_frm2((IO_ELECSPEAKER_VALID == uszResult),	LANG_IO_ELECSPEAKER, LANG_VALID, LANG_INVALID);
						
						// ��Դ�������ź�
						IOGet((byte)IOS_POWDETM, &uszResult );
						build_frm2((IO_POWDETM_VALID == uszResult),	LANG_IO_POWDETM, LANG_VALID, LANG_INVALID);
						
						// ���������ź�
						IOGet((byte)IOS_CHEDENG, &uszResult );
						build_frm2((IO_CHEDENG_VALID == uszResult),	LANG_IO_CHEDENG, LANG_VALID, LANG_INVALID);
						
						// �����ź�
						IOGet((byte)IOS_ZHENLING, &uszResult );
						build_frm2((IO_ZHENLING_VALID == uszResult), LANG_IO_ZHENLING, LANG_VALID, LANG_INVALID);
#endif
						
#if VEHICLE_TYPE == VEHICLE_M2
						// Ƿѹ�ź�
						IOGet((byte)IOS_QIANYA, &uszResult );
						build_frm2((IO_QIANYA_VALID == uszResult), LANG_IO_QIANYA, LANG_VALID, LANG_INVALID);
						
						// ǰ�ű����ź�
						IOGet((byte)IOS_FDOOR_ALERT, &uszResult );
						build_frm2((IO_FDOORALERT_VALID == uszResult),	LANG_IO_FDOORALERT, LANG_VALID, LANG_INVALID);
						
						//ǰ�����ź�
						IOGet((byte)IOS_FDOOR, &uszResult );
						build_frm2((IO_FDOOR_OPEN == uszResult),	LANG_IO_FDOOR, LANG_VALID, LANG_INVALID);
						
						// ���ű����ź�
						IOGet((byte)IOS_BDOOR_ALERT, &uszResult );
						build_frm2((IO_BDOORALERT_VALID == uszResult),	LANG_IO_BDOORALERT, LANG_VALID, LANG_INVALID);
						
						//�����ź�
						IOGet((byte)IOS_BDOOR, &uszResult );
						build_frm2((IO_BDOOR_OPEN == uszResult),	LANG_IO_BDOOR, LANG_VALID, LANG_INVALID);
						
						// ��Դ�������ź�
						IOGet((byte)IOS_POWDETM, &uszResult );
						build_frm2((IO_POWDETM_VALID == uszResult),	LANG_IO_POWDETM, LANG_VALID, LANG_INVALID);
						
						// ��ɲ�ź�
						IOGet((byte)IOS_JIAOSHA, &uszResult );
						build_frm2((IO_JIAOSHA_VALID == uszResult),	LANG_IO_JIAOSHA, LANG_VALID, LANG_INVALID);
						
						// ���������ź�
						IOGet((byte)IOS_CHEDENG, &uszResult );
						build_frm2((IO_CHEDENG_VALID == uszResult),	LANG_IO_CHEDENG, LANG_VALID, LANG_INVALID);
						
						// �����ź�
						IOGet((byte)IOS_ZHENLING, &uszResult );
						build_frm2((IO_ZHENLING_VALID == uszResult), LANG_IO_ZHENLING, LANG_VALID, LANG_INVALID);
#endif					
						
						bool meter_link = ( int(GetTickCount()-g_last_heartbeat) <20*1000 ) ? true : false ;
						build_frm2(meter_link, LANG_METER_CONNECTION, LANG_CONNECT,	LANG_DISCONNECT);
						
						
						//��Դ��ƽ��⣬��֧��
						
						//����
						IOGet((byte)IOS_ALARM, &uszResult );
						build_frm2(IO_ALARM_OFF == uszResult, LANG_IO_FOOTALERM, LANG_NORMAL, LANG_ALERM);
						
						//Ƿѹ�źţ��ݲ�֧��
						
						char str[100] = {0};
#if USE_OIL == 3
						sprintf(str, "%s AD:%u,", LANG_IO_OIL, g_usOilAD);
#endif
						strcpy(frm+len, str); 
						len += strlen(str);

						DWORD speed = 0;
						DWORD tick;
						//tick = G_GetMeterTick(&speed);���貹�䣩
						sprintf(str, "%s:%u,%s:%u", LANG_IO_PULSE, tick, LANG_IO_SPEED, speed);
						strcpy(frm+len, str); 
						len += strlen(str);
						
						frm[1] = len - 2;
						g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, len, frm, dwPktNum);
						
					}
					break;
					
				case 0x01:	//������ʵʱ���
					{
						m_iodect = 1;
						show_diaodu(LANG_IO_CHKMODE_OPEN);
						
						char by = 0x09;
						DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
					}
					break;
					
				case 0x02:	//PC��Ƶ����
					{			
						show_diaodu("��ʼ��⣡");
						
						char by = 0x04;
						DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
					}
					break;
					
				case 0x03:	//ϵͳ������1
					{
						char buf[] = {0x01, 0x01};
						DataPush(buf, 2, DEV_DIAODU, DEV_DVR, LV1);
					}
					break;
					
				case 0x04:	//ϵͳ������1
					{
						char buf[] = {0x01, 0x02};
						DataPush(buf, 2, DEV_DIAODU, DEV_DVR, LV1);
					}
					break;
					
				case 0x05:	//����GPS����
					{
						unsigned char uszResult;

						if(!IOGet(IOS_ACC, &uszResult))
						{
							if(IO_ACC_ON == uszResult) // ACC��Ч
							{	
								m_gpsdect = 1;
#if CHK_VER == 0
								show_diaodu(LANG_GPS_CHKMODE_OPEN);	
#endif						
							}
							else
							{
								show_diaodu(LANG_ACC_NEEDVALID);	
							}
						}
					}
					break;
					
				case 0x06:	//���ڼ��
					{
						show_diaodu("��ʼ��⣡");

						char by = 0x02;
						DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
					}
					break;
					
				case 0x07:	//���Ͷϵ����
					{
						show_diaodu("��ʼ��⣡");
						
						IOSet(IOS_OILELECCTL, IO_OILELECCTL_CUT, NULL, 0);
#if VEHICLE_TYPE == VEHICLE_M
						IOSet(IOS_TOPLEDPOW, IO_TOPLEDPOW_OFF, NULL, 0);
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
						IOSet(IOS_YUYIN, IO_YUYIN_ON, NULL, 0);
#endif
						sleep(1);
						IOSet(IOS_OILELECCTL, IO_OILELECCTL_RESUME, NULL, 0);
#if VEHICLE_TYPE == VEHICLE_M
						IOSet(IOS_TOPLEDPOW, IO_TOPLEDPOW_ON, NULL, 0);
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
						IOSet(IOS_YUYIN, IO_YUYIN_OFF, NULL, 0);
#endif
					}
					break;
					
				case 0x08:	//GSM���
					{
						char str[100] = {0};
						BYTE bytSta;

						if( NETWORK_TYPE==NETWORK_GSM )
							bytSta = g_objPhoneGsm.GetPhoneState();
						else if( NETWORK_TYPE==NETWORK_TD )
							bytSta = g_objPhoneTd.GetPhoneState();
						else if( NETWORK_TYPE==NETWORK_EVDO )
							bytSta = g_objPhoneEvdo.GetPhoneState();
						else if( NETWORK_TYPE==NETWORK_WCDMA )
							bytSta = g_objPhoneWcdma.GetPhoneState();
						
						strcat(str, "Net Reg:");
						strcat(str, 1 == bytSta || 2 == bytSta ? LANG_LOGGED : LANG_NOLOG );
						if( NETWORK_TYPE==NETWORK_GSM )
							sprintf(str+strlen(str), " %s:%d ", LANG_SIGNAL, g_objPhoneGsm.GetPhoneSignal());
						else if( NETWORK_TYPE==NETWORK_TD )
							sprintf(str+strlen(str), " %s:%d ", LANG_SIGNAL, g_objPhoneTd.GetPhoneSignal());
						else if( NETWORK_TYPE==NETWORK_EVDO )
							sprintf(str+strlen(str), " %s:%d ", LANG_SIGNAL, g_objPhoneEvdo.GetPhoneSignal());
						else if( NETWORK_TYPE==NETWORK_WCDMA )
							sprintf(str+strlen(str), " %s:%d ", LANG_SIGNAL, g_objPhoneWcdma.GetPhoneSignal());

						strcat(str, "Net:" );
						strcat(str, 2 == bytSta ? LANG_ONLINE : LANG_OFFLINE );
						
						show_diaodu(str);
					}
					break;
					
				case 0x09:	//��ý��������½
					{			
						char buf[2] = {0x01, 0x03};	
 						DataPush(buf, 2, DEV_DIAODU, DEV_UPDATE, 2);
					}
					break;
					
				case 0x0C:	//��дLED��, ��Ϊ��ʽ��SD������Ӳ��
					{
						char by = 0x06;
						DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);
					}
					break;
					
				case 0x0d:	//�ָ���������
					{
						// ֪ͨQianExe
						BYTE buf[2];
						buf[0] = 0xee;
						buf[1] = 0x0d;
						DataPush((char*)buf, 2, DEV_DIAODU, DEV_QIAN, 2);
						if(0==ResumeCfg(false, false))
						{
							//֪ͨDvrExe
							char by = 0x0b;
							DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
							show_diaodu("%s %s!", LANG_RESUMECFG, LANG_SUCC);
						}
						else
							show_diaodu("%s %s!", LANG_RESUMECFG, LANG_FAIL);
					}
					break;
					
				case 0x0f:	//Ӧ�ÿ���
					{
						pthread_t thCopy;

						if( access("/mnt/UDisk/part1/exe.bin", F_OK) == 0 )
							pthread_create(&thCopy, NULL, G_ExeUpdateThread, (void *)this);
						else
							pthread_create(&thCopy, NULL, G_AppUpdateThread, (void *)this);
					}
					break;
					
				case 0x10:	//��Ƶ���ԣ���֧�֣�
					{
					}
					break;
					
				default:
					PRTMSG(MSG_DBG,"Project menu(%02X)!\n", pfrm->flag);
					break;
				}
			}
			break;

			//zzg add
			//���촥����ͨ��Э��
			//��QianExe�ﴦ��
			//		case 0x3c://һ������
			// 			{
			// 				show_msg("Rcv Comu3c");
			// 				Frm12 frm;
			// 				frm.len=5;
			// 				sprintf(frm.num,"%s","10086");
			//              DataPush((char*)(&frm), sizeof(frm), DEV_DIAODU, DEV_PHONE, 2);
			// 			}
			//			break;
		case 0x96:
			{
				//show_msg("Rcv Comu96");
				char res[2];
				res[0]=0x97;
				res[1]=0x02;
				DataPush(res, 2, DEV_DIAODU, DEV_DIAODU, 2);
			}			
			break;

		case 0x98:
			{
				//show_msg("Rcv Comu98");
				char res=0x99;
				DataPush(&res, 1, DEV_DIAODU, DEV_DIAODU, 2);
			}	
			break;

// 		case 0xad:
// 			{
// 				char frm[8] = {0};
// 				int len = 0;
// 				
// 				frm[len++] = (char)0xE9;
// 				frm[len++] = (char)0x00;
// 				GPSDATA gps(0);
// 				GetCurGps( &gps, sizeof(gps), GPS_LSTVALID );
// 				//GPS״̬
// 				if(gps.valid == 'A') 
// 					frm[len++]=0x01;
// 				else 
// 					frm[len++]=0x02;
// 				//GPRS״̬
// 				if(g_objPhone.GetPhoneState())
// 					frm[len++]=0x01;
// 				else 
// 					frm[len++]=0x02;
// 				DataPush(frm, 8, DEV_DIAODU, DEV_QIAN, 2);
// 			}
// 			break;

		case 0x3c:
		case 0x48:
		case 0x4a:
		case 0x73:
		case 0xd5:
		case 0xd7:		
		case 0xd9:
			//show_msg("Rcv ComuD9");
		case 0xda:
		case 0xdb:
			DataPush(v_szBuf, v_iLen, DEV_DIAODU, DEV_QIAN, 2);
			break;
			
		case 0x60:// ¼��Ч������->��ѯ�豸״̬
			{
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
				show_diaodu("GPS:%s;%s:%s;TW2865:%s", 
												g_objLightCtrl2.m_dwDevErrCode&0x00000001 ? "�쳣" : "����",
												NETWORK_TYPE == NETWORK_TD?"LC6311":(NETWORK_TYPE == NETWORK_EVDO?"MC703":(NETWORK_TYPE == NETWORK_WCDMA?"SIM5218":"OTHER")),
												g_objLightCtrl2.m_dwDevErrCode&0x00000002 ? "�쳣" : "����",
												g_objLightCtrl2.m_dwDevErrCode&0x00000004 ? "�쳣" : "����");
#endif
			}
			break;
		
		default:
			break;
	}
}

//�ڵ���������ʾ��ʾ��Ϣ
//fmt,..: ��ʽ������
void CDiaodu::show_diaodu(char *fmt, ...)
{
	int iRet;
	DWORD dwPktNum;
	va_list ap;
	
	va_start(ap, fmt);
	char str[1024] = {0x72, 0x01};
	vsprintf(str+2, fmt, ap);
	va_end(ap);
	
	iRet = g_objDiaoduOrHandWorkMng.PushData((BYTE)LV1, min(47,strlen(str)), str, dwPktNum);
}


//--------------------------------------------------------------------------------------------------------------------------
// ���У���(ֱ��ȡ�ۼӺ�)
// buf: ���ݻ�����ָ��
// len: ���ݵĳ���
// ����: У���Ƿ���ȷ
bool CDiaodu::check_crc(const byte *buf, const int len)
{   
	byte check_sum = get_crc( buf+1, len-1 );
	return (buf[0]==check_sum);
}

//--------------------------------------------------------------------------------------------------------------------------
// �õ�У���(ֱ��ȡ�ۼӺ�)
// buf: ���ݻ�����ָ��
// len: ���ݵĳ���
// ����: ���ݵ�У���
byte CDiaodu::get_crc(const byte *buf, const int len)
{   
	byte check_sum = 0;
	for(int i=0; i<len; i++) {
		check_sum += *(buf+i);
	}
    return check_sum;
}

