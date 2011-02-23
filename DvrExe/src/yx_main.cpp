#include "yx_common.h"

int main(int argc, char* argv[])
{
	int i;
	int iRet;
	
// 	if((g_objDevFd.Wdg = open("/dev/misc/watchdog", O_WRONLY)) == -1)
// 	{
// 		PRTMSG(MSG_ERR, "open watchdog failed\n");
// 		system("reboot");
// 	}
// 	while(1)
// 	{
// 		write(g_objDevFd.Wdg, "\0", 1);
// 		sleep(1);
// 	}

	ListPhoneInfo();
	
	CrtVerFile();

#if VEHICLE_TYPE == VEHICLE_M
	IOSET(IOS_TRUMPPOW, IO_TRUMPPOW_ON);//�������ŵ�Դ
	IOSET(IOS_EARPHONESEL, IO_EARPHONESEL_OFF);//ѡ������ͨ��
	IOSET(IOS_AUDIOSEL, IO_AUDIOSEL_PC);//ѡ��PC��Ƶ
	IOSET(IOS_HARDPOW, IO_HARDPOW_ON);//����Ӳ�̵�Դ
	IOSET(IOS_HUBPOW, IO_HUBPOW_ON);//����USB��������ܿ���HUB/SD��Դ
	IOSET(IOS_VIDICONPOW, IO_VIDICONPOW_ON, 100);//��������ͷ��Դ	
	IOSET(IOS_TW2835RST, IO_TW2835RST_LO, 100);//��λTW2835
	IOSET(IOS_TW2835RST, IO_TW2835RST_HI, 100);

	INSMOD(KO_HI3510_VS);
	INSMOD(KO_HI3510_I2C);
	INSMOD(KO_HI3510_SIO);
	INSMOD(KO_HI3510_SSP);
//	INSMOD(KO_HI3510_EXT3);

	//INSMOD(KO_HI3510_GPIO);	// GPIOģ����ComuServ������	
	//INSMOD(KO_YX3510_GPIO);

	//INSMOD(KO_YX3510_GPS);	// GPSģ����ComuExe������
	INSMOD(KO_YX3510_IRDA);
	INSMOD(KO_YX3510_TW2835);
	INSMOD(KO_YX3510_PERSPM);
	INSMOD(KO_YX3510_PT7C4311);
	INSMOD(KO_YX3510_SIM_UART);
	
	SetTimeByRtc();//�������ȶ�ȡRTCʱ�䣬����Ϊϵͳʱ��
	g_objTimerMng.Init();
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	SetTimeByRtc();//�������ȶ�ȡRTCʱ�䣬����Ϊϵͳʱ��
	g_objTimerMng.Init();
	
#if CHK_VER == 1
	IOSET(IOS_LCDPOW, IO_LCDPOW_ON);//����LCD����Դ
#else
	IOSET(IOS_LCDPOW, IO_LCDPOW_OFF);//�ر�LCD����Դ
#endif
	//IOSET(IOS_SCHEDULEPOW, IO_SCHEDULEPOW_ON);//�������ȵ�Դ
	IOSET(IOS_TRUMPPOW, IO_TRUMPPOW_OFF);//�رչ��ŵ�Դ
#if USE_DISCONNOILELEC == 0
	IOSET(IOS_OILELECCTL, IO_OILELECCTL_CUT);	// �رռ̵���
#endif
	IOSET(IOS_YUYIN, IO_YUYIN_OFF);	//�ر������л�
	IOSET(IOS_EARPHONESEL, IO_EARPHONESEL_OFF);//ѡ������ͨ��
	IOSET(IOS_AUDIOSEL, IO_AUDIOSEL_PC);//ѡ��PC��Ƶ
#if VEHICLE_TYPE == VEHICLE_V8
	IOSET(IOS_USBPOW, IO_USBPOW_ON);
#endif
#if VEHICLE_TYPE == VEHICLE_M2
	IOSET(IOS_HUBPOW, IO_HUBPOW_ON);
	IOSET(IOS_HARDPOW, IO_HARDPOW_ON);
#endif
	IOSET(IOS_VIDICONPOW, IO_VIDICONPOW_ON, 100);//��������ͷ��Դ	
	IOSET(IOS_TW2865RST, IO_TW2865RST_LO, 100);//��λTW2865
	IOSET(IOS_TW2865RST, IO_TW2865RST_HI, 100);
	
	INSMOD(KO_LINUX_FB);
	
	INSMOD(KO_HI3511_BASE);
	INSMOD(KO_HI3511_SYS);
	INSMOD(KO_HI3511_MMZ);
	INSMOD(KO_HI3511_SIO);
	INSMOD(KO_HI3511_TDE);
	INSMOD(KO_HI3511_VIU);
	INSMOD(KO_HI3511_VOU);
	INSMOD(KO_HI3511_AI);
	INSMOD(KO_HI3511_AO);
	INSMOD(KO_HI3511_DSU);
//	INSMOD(KO_HI3511_MD);
	INSMOD(KO_HI3511_VPP);
	INSMOD(KO_HI3511_CHNL);
	INSMOD(KO_HI3511_VENC);
	INSMOD(KO_HI3511_GROUP);
	INSMOD(KO_HI3511_VDEC);
//	INSMOD(KO_HI3511_MPEG4E);
	INSMOD(KO_HI3511_H264E);
	INSMOD(KO_HI3511_H264D);
	INSMOD(KO_HI3511_JPEGE);
//	INSMOD(KO_HI3511_JPEGD);
	INSMOD(KO_HI3511_FB);
	INSMOD(KO_HI3511_DMAC);
	INSMOD(KO_HI3511_SD);
	sleep(1);
	INSMOD(KO_LINUX_MMCCORE);
	INSMOD(KO_HI3511_MCI);
	INSMOD(KO_LINUX_MMCBLOCK);
	//INSMOD(KO_HI3511_USB);
	
	//	INSMOD(KO_YX3511_GPIO);
	//	INSMOD(KO_YX3511_GPS);	
	//	INSMOD(KO_YX3511_IRDA);	
	//	INSMOD(KO_YX3511_PERSPM);	// ģ���Ѽӵ��ں���
	INSMOD(KO_YX3511_TW2865);
#endif

#if VEHICLE_TYPE == VEHICLE_M
	g_pMenuPage = (unsigned char *)malloc(SCRWIDTH*SCRHIGH*3);
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	g_pMenuPage = (unsigned char *)malloc(SCRWIDTH*SCRHIGH*4);
#endif
	if( g_pMenuPage == NULL )
	{
		PRTMSG(MSG_ERR, "malloc buffer failed\n");
		exit(1);
	}
	
	pthread_mutex_init(&g_AVListMutex, NULL);
	pthread_mutex_init(&g_objAVUplBuf.AVMutex, NULL);

#if VEHICLE_TYPE == VEHICLE_M
	for(i=0; i<4; i++)
	{
		if( !InitBuf(i,25600, 'A') || !InitBuf(i,256000, 'V') )
		{
			PRTMSG(MSG_ERR, "malloc buffer failed\n");
			exit(1);
		}

		g_objVideoDataMng[i].InitCfg(128*1024, 30000);
		g_objAudioDataMng[i].InitCfg(10*1024,  30000);
	}
	if( !InitBuf(0,512000, 'T') )
	{
		PRTMSG(MSG_ERR, "malloc buffer failed\n");
		exit(1);
	}
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	for(i=0; i<4; i++)
	{
		if( !InitBuf(i,102400, 'A') || !InitBuf(i,1024000, 'V') )
		{
			PRTMSG(MSG_ERR, "malloc buffer failed\n");
			exit(1);
		}

		g_objVideoDataMng[i].InitCfg(1024*1024, 30000);
		g_objAudioDataMng[i].InitCfg(128*1024,  30000);
	}
	if( !InitBuf(0,1024000, 'T') )
	{
		PRTMSG(MSG_ERR, "malloc buffer failed\n");
		exit(1);
	}
#endif

	if(
#if VEHICLE_TYPE == VEHICLE_M
		   (g_objDevFd.Tw2835 = open("/dev/misc/TW2835dev", O_RDWR)) == -1 
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		   (g_objDevFd.Tw2865 = open("/dev/misc/tw2865adev", O_RDWR)) == -1 
#endif
		|| (g_objDevFd.Irda = open("/dev/misc/irdadrv", O_RDWR|O_NONBLOCK)) == -1
		|| (g_objDevFd.ASC = fopen("/lib/ASC24", "rb")) == NULL 
		|| (g_objDevFd.HZK = fopen("/lib/HZK24S", "rb")) == NULL 
		|| InitTTS() == -1
		|| GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg)) != 0)
	{
		PRTMSG(MSG_ERR, "system init failed!\n");
		exit(1);
	}
	
	memset(&g_objUploadFileSta, 0 ,sizeof(g_objUploadFileSta));

	int iErrTime[6];
	GetCurDateTime( iErrTime );
	PRTMSG(MSG_DBG, "ϵͳ����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d\n", iErrTime[0], iErrTime[1], iErrTime[2], iErrTime[3], iErrTime[4], iErrTime[5]);

	pthread_t pthDataUpl, pthDiskMng, pthCmdRev, pthSysSwi, pthDealUdp;
	pthread_create(&pthDataUpl, NULL, G_DataUpload, NULL);
	pthread_create(&pthDiskMng, NULL, G_DiskManage, NULL);
	pthread_create(&pthCmdRev, NULL, G_CmdReceive, NULL);
	pthread_create(&pthDealUdp, NULL, G_DealSockData, NULL);
	pthread_create(&pthSysSwi, NULL, G_SysSwitch, NULL);
	
	PRTMSG(MSG_NOR, "DvrExe begin to run!\n");
	
	//DogInit(); // ȷ����һ���жϿ��Ź������־�ܳɹ�

	i = 0;
	while(!g_bProgExit)
	{		
		sleep(1);

		if( i == 15 )
		{
			int iCurDateTime[6];
			GetCurDateTime( iCurDateTime );
			ShowDiaodu("��ǰʱ�䣺%4d-%02d-%02d %02d:%02d:%02d", iCurDateTime[0], iCurDateTime[1], iCurDateTime[2], iCurDateTime[3], iCurDateTime[4], iCurDateTime[5]);
		}

		DogClr( DOG_DVR );

// 		if( i++%8 == 0 )
// 		{
// 			DWORD dwFlag = DogQuery();
// 			
// 			if( 0xffffffff==dwFlag 
// 				||(0xffffffbf==dwFlag && CTRLSTATE(&g_enumSysSta)==true) )//��OSD�˵���������ʱ��״̬���߳��������忴�Ź�
// 				write(g_objDevFd.Wdg, "\0", 1);
// 			else
// 				PRTMSG(MSG_DBG, "dwFlag = %08x\n", dwFlag);
// 		}
// 		else
// 		{
// 			DogClr( DOG_DVR );
// 		}
	}

	sleep(15);

	PRTMSG(MSG_NOR, "DvrExe is now to quit!\n");
}
