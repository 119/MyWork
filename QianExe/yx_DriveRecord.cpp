#include "yx_QianStdAfx.h"

#if USE_DRVREC == 1

void *G_ThreadDrvRecWork(void *arg)
{
	g_objDriveRecord.P_ThreadDrvRecWork();
}

void G_TmDrvRecTimer(void *arg, int len)
{
	g_objDriveRecord.P_TmDrvRecTimer();
}

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-DriveRecord")

//////////////////////////////////////////////////////////////////////////

CDriveRecord::CDriveRecord()
{
	m_pthreadWork = 0;
	m_bytAreaCode = 0;

	m_bNeedClt = false;
	m_v.init();				//������ʼ��
	m_bLcdLog = false;
	m_bDrvRecQuit = false;
#if USE_LIAONING_SANQI == 1
	TotalMeter = 0;
	Mile = 0;
#endif
}

CDriveRecord::~CDriveRecord()
{

}

int CDriveRecord::Init()
{
	// ��ʼ�������Լ��ļ�
	init_filecfg();

	// ���������߳�
	if( pthread_create(&m_pthreadWork, NULL, G_ThreadDrvRecWork, NULL) != 0)
	{
		PRTMSG(MSG_ERR, "Create DrvRec Thread failed!\n");
		return ERR_THREAD;
	}

	// ע�ᶨʱ��
	_SetTimer(&g_objTimerMng, DRIVERECORD_TIMER, 1000, G_TmDrvRecTimer);

	return 0;
}

int CDriveRecord::ReInit()
{
	HRcdFile objHRcdFile;
	objHRcdFile.init();

	FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb");
	if( NULL != fp) 
	{
		static bool bFirstReInit = true;

		fread(&objHRcdFile, 1, sizeof(objHRcdFile), fp);

		// �����ֱ��������µ�ֵ������
		objHRcdFile.ticks = m_v.ticks;					//��ʱ����(��)
		objHRcdFile.numdrv = m_v.numdrv;				//��ǰ˾�����
		objHRcdFile.stacar = m_v.stacar;				//��ǰ������״̬(ֻ������״̬,�ƶ��;�ֹ)
		objHRcdFile.stacur = m_v.stacur;				//��ǰ����״̬(����״̬)
		objHRcdFile.stapre = m_v.stapre;				//�ϴγ���״̬(����״̬)
		objHRcdFile.run = m_v.run;				//����������ʻʱ��(��)
		objHRcdFile.stop = m_v.stop;			//��������ֹͣʱ��(��)
		objHRcdFile.inputflag = m_v.inputflag;			//˾����½���(����������)
		objHRcdFile.speed = m_v.speed;					//��ǰ�ٶ�(����/ʱ)
		memcpy(objHRcdFile.vector, m_v.vector, 60);		//���60����ٶ�(����/ʱ)
		memcpy(objHRcdFile.average, m_v.average, 15);	//���15���ӵ�ƽ���ٶ�
		objHRcdFile.lstlat = m_v.lstlat;				//���һ����Ч�ľ���
		objHRcdFile.lstlon = m_v.lstlon;				//���һ����Ч��γ��
		objHRcdFile.metercur = m_v.metercur;			//������ʻ�����(m)
		
		if( true == bFirstReInit )
		{
			bFirstReInit = false;
			objHRcdFile.metertotal += m_v.metertotal;		//�����(m)
		}

		memcpy(objHRcdFile.rp_doub, m_v.rp_oil, 5);		//��־�յ� �����ϴ���ʻ���ݵ�Ӧ��
		objHRcdFile.uptime = m_v.uptime;		//�������ϱ���ʱ��
		objHRcdFile.st_clt = m_v.st_clt;		//���ͺ������ϱ��ɼ�״̬
		objHRcdFile.updoub = m_v.updoub;				//�Ƿ���Ҫ�ϱ��ɵ����ݵı�־, 
		objHRcdFile.tickdoub = m_v.tickdoub;			//�¹��ɵ�ͣ��ʱ�����
		memcpy(objHRcdFile.rp_doub, m_v.rp_doub, 2);	//��־�յ� ʵʱ�¹��ɵ��ϴ�Ӧ��
		objHRcdFile.st_log = m_v.st_log;				//˾����½״̬	
		objHRcdFile.prompt = m_v.prompt;				//�Ƿ���������ʾ����
		objHRcdFile.alarmed = m_v.alarmed;		//�Ƿ������Ƿ񱨾����ı�־

		//Ȼ���滻�����ڵ�m_v
		memcpy((void*)&m_v, (void*)&objHRcdFile, sizeof(m_v));
		PRTMSG(MSG_DBG, "m_v renew!\n");
	}
	else
	{
		fp = fopen(DRIVE_RECORD_FILENAME, "wb+");
		if( NULL != fp)
		{	
			fwrite(&objHRcdFile, 1, sizeof(objHRcdFile), fp);
			PRTMSG(MSG_NOR,"Create DrvJg2 Succ!\n");
		}
		else
		{
			PRTMSG(MSG_NOR,"Create DrvJg2 Fail\n");
			perror("");
		}
	}
	
	if( NULL != fp )
		fclose(fp);
}

int CDriveRecord::Release()
{
	flush_headrcd();
}

// ��ʼ�������ļ�
void CDriveRecord::init_filecfg()
{
	// ��ȡ��̨���õ�m_cfg��
	uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
	GetSecCfg(&m_cfg, sizeof(m_cfg), offset, sizeof(m_cfg));
	
	if(123!=m_cfg.inited) // ע�⣬��Ϊ�ж�����ı�־123
	{
		m_cfg.init(); 
		SetSecCfg(&m_cfg, offset, sizeof(m_cfg));
		PRTMSG(MSG_NOR, "Init DrvCfg!\n");
	}

	// ��ȡԽ�����򱨾����ò���
	GetSecCfg((void*)&m_objAreaCfg, sizeof(m_objAreaCfg), offsetof(tagSecondCfg, m_uni2QAreaCfg.m_obj2QAreaCfg), sizeof(m_objAreaCfg));
	
// 	// ��ʼ�������ļ�
// 	FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb");
// 	if(fp) 
// 	{
// 		fread(&m_v, 1, sizeof(m_v), fp);
// 
// 		if( Logined != m_v.st_log ) // ʹ������ʱ�Ϳ�����ʾ˾����½
// 			m_v.st_log = UnLog; 
// 
// 		m_v.st_clt = 0; // 090817 xun add,ʹ������ʱ��ʻ��¼�ϱ�״̬���ܴӳ�ʼ״̬��ʼ
// 	}
// 	else
// 	{
// 		fp = fopen(DRIVE_RECORD_FILENAME, "wb+");
// 		if(fp)
// 			fwrite(&m_v, 1, sizeof(m_v), fp);
// 
// 		PRTMSG(MSG_NOR,"Create DrvJg2 %s!\n", fp ? "Succ" : "Fail");
// 	}
// 
// 	fclose(fp);
}

void CDriveRecord::P_ThreadDrvRecWork()
{
#define TM_DOUB1	100		//����ʽͣ��ʱ��
#define TM_DOUB2	300		//��ʽͣ��ʱ��

	bool cur;
	int i=0;

	// 200����ִ��һ��, ���ڼ�¼�ͱ����¹��ɵ���Ϣ���ϱ��¹��ɵ�
	while( !g_bProgExit && !m_bDrvRecQuit)
	{
		usleep(200000);

		// --------- �����¹��ɵ����� -----------------------
		cur = (m_v.vector[0]>VT_LOW_RUN) && acc_valid();
		
		if(m_v.tickdoub>=TM_DOUB1 && m_v.tickdoub<TM_DOUB2 && cur)	//ͣ����20s-60s֮��
		{	
			m_v.rcddoub.stoptype = 0x01;	//����ʽ�ɵ�
			flush_doubrcd();	
		}

		if(TM_DOUB2==m_v.tickdoub)	//ͣ������60s,���ϱ���־��1
		{	
			m_v.rcddoub.stoptype = 0x02;	//��ʽ�ɵ�
			flush_doubrcd(); 
			if(!m_v.updoub)		m_v.updoub = 1;		//�����ж��ϱ�������
		}		
		
		if(cur)
		{
			m_v.tickdoub = 0;
			static int cnt = 0;
			if(cnt++>=4)	//1�뱣��һ��GPS
			{			
				cnt = 0;
				m_v.rcddoub.set_data(true, get_sensor());
			} 
			else 
			{ 
				m_v.rcddoub.set_data(false, get_sensor());
			}
		} 
		else
		{
			m_v.tickdoub++;		//�ۼ�ͣ��ʱ��
		}

#if USE_PROTOCOL == 2
		continue;		// �����棬ɽ����ͨ��Ŀ���������ϴ��¹��ɵ�����
#endif
		
		// --------- �ϱ��¹��ɵ����� ----------------------
		static enum {
			Idlesse, Sending, WaitReply, 
		} st_snd = Idlesse;
		static ulong index = 0;		//��ǰ��¼���ڼ�¼�ļ��е�����
		static int overtime = 0;	//���ͳ�ʱ����
		static byte frmno = 0;		//֡���
		
		switch(st_snd) 
		{
		case Idlesse:	// ����״̬
			{
				index = 0;
				overtime = 0;

				if(m_v.updoub)	
					st_snd = Sending;
			}
			break;
			
		case Sending:	// ����״̬
			{
				RcdDoub rcd;
				index = get_lstdoub(m_v.updoub, rcd);
				if(0==index)
				{
					PRTMSG(MSG_NOR, "No find lstdoub!\n"); 
					m_v.updoub = 0; 
					st_snd = Idlesse;
					break;
				}
				
				if(++frmno>0x0f)
					frmno = 0;
				
				// ����ǰ������¼
				frm0157 frm = {0};
				frm.trantype = (frmno<<4) | 0x01;
				memcpy(frm.e_time, rcd.times, 6);

				for(i=0; i<10; i++) 
				{
					memcpy(frm.doub[i].lon, rcd.pos[i].lon, 4);
					memcpy(frm.doub[i].lat, rcd.pos[i].lat, 4);
					frm.doub[i].speed = rcd.vector[i*5];
					frm.doub[i].sensor = rcd.sensor[i*5];
				}
				frm.doubtype = rcd.stoptype;
				tcp_send(0x01, 0x57, &frm, sizeof(frm));
				
				// ���ͺ������¼
				frm.trantype = (frmno<<4) | 0x02;
				memcpy(frm.e_time, rcd.times, 6);
				for(i=0; i<10; i++)
				{
					memcpy(frm.doub[i].lon, rcd.pos[i+10].lon, 4);
					memcpy(frm.doub[i].lat, rcd.pos[i+10].lat, 4);
					frm.doub[i].speed = rcd.vector[i*5+50];
					frm.doub[i].sensor = rcd.sensor[i*5+50];
				}
				frm.doubtype = rcd.stoptype;
				tcp_send(0x01, 0x57, &frm, sizeof(frm));
				
				// ���rp_doub
				m_v.rp_doub[0] = 0;
				m_v.rp_doub[1] = 0;
				
				st_snd = WaitReply;
				PRTMSG(MSG_NOR, "Send doub and wait rp_doub(%u)..\n", index);
			}
			break;
			
		case WaitReply:	// �ȴ�����Ӧ��
			{
				bool rcv1 = ( m_v.rp_doub[0] == ((frmno<<4)|0x01) );
				bool rcv2 = ( m_v.rp_doub[1] == ((frmno<<4)|0x02) );

				if( rcv1 && rcv2 )	//�յ�Ӧ��(2֡��Ӧ���յ�)
				{
					PRTMSG(MSG_NOR, "Rcv rp_doub!\n");
					set_doubtype(index-1);

					if(1==m_v.updoub)
						m_v.updoub = 0;

					st_snd = Idlesse; 
					break; 
				}
				
				if(++overtime > 3*300)	//����Ӧ��ʱ!
				{
					PRTMSG(MSG_NOR, "Wait rp_doub overtime!\n");
					m_v.updoub = 0;
					st_snd = Idlesse; 
					break; 
				}
			}
			break;
			
		default:
			break;
		}
	}
}

void CDriveRecord::P_TmDrvRecTimer()
{
	tagGPSData gps(0);
	GetCurGps(&gps, sizeof(gps), GPS_LSTVALID);
	
	m_v.ticks++;
	
	// ���ó���״̬(�ƶ���ֹ)
	set_car_state(gps);
	
	// �������ͳ��
	deal_meter(gps);
	
	// ����Ƿ��ٲ�����
	chk_overspeed();
	
	// ����Ƿ���ٲ�����
	chk_belowspeed();
	
	// ����˾����½
	deal_login();	
	
	// ��������Э��
	deal_center();
	
	// ��������ʹ��¼
	deal_state();
	
	// ����������
	deal_alarm(gps);

	// ����Խ������0F Э�����ͣ�
	deal_area(gps);
	
	// �����ѯ���ݵ��ϴ�
	deal_query();

#if 0 == USE_PROTOCOL	
#if USE_LIAONING_SANQI == 0
	// ���������ϱ���ʻ����(���ͺ�)
	deal_updrv(gps);
#endif
#endif
}

//------------------------------------------------------------------------------
// �������ĵ�Э��
void CDriveRecord::deal_center()
{
	// ��������Э��	
	byte tmp[2048] = {0};
	ulong len = tcprcv.pop(tmp);
	if(len==0 || len>2048)	return;

	// ʵʱ�¹��ɵ�Э��
	if(0x01 == tmp[0])
	{
		for(ulong i=2; i<len; i++)	
		{ 
			if(0x7f==tmp[i])	tmp[i] = 0; 
		}
		switch(tmp[1]) 
		{
			case 0x17:	
				hdl_0117(tmp+2, len-2);		
				break;	//ʵʱ�¹��ɵ��ϴ�Ӧ��		
			case 0x27:	
				hdl_0127(tmp+2, len-2);		
				break;	//����ʽ�ɵ��ϴ�����			
			default:	
				PRTMSG(MSG_NOR, "Rcv unknown frm(%02x,%02x)!\n", tmp[0], tmp[1]);	
				break;
		}
	}

	// ���ͳ��ҵ��Э��
	if(0x09 == tmp[0]) 
	{
		for(ulong i=2; i<len; i++)	
		{ 
			if(0x7f==tmp[i])	
				tmp[i] = 0; 
		}
		switch(tmp[1]) 
		{
			case 0x01:	
				hdl_0901(tmp+2, len-2);		
				break;	//����̲�ѯ����		
			case 0x02:	
				hdl_0902(tmp+2, len-2);		
				break;	//������������			
			case 0x04:	
				hdl_0904(tmp+2, len-2);		
				break;	//��ѯ���360Сʱ�ں�2�����������ʻ�������
			case 0x05:	
				hdl_0905(tmp+2, len-2);		
				break;	//��̼�¼��ѯ
			default:	
				PRTMSG(MSG_NOR, "Rcv unknown frm(%02x,%02x)!\n", tmp[0], tmp[1]);	
				break;
		}
	}

	// �û�����ҵ��Э��
	if(0x10 == tmp[0]) 
	{
		for(ulong i=2; i<len; i++)	
		{ 
			if(0x7f==tmp[i])	
				tmp[i] = 0; 
		}
		switch(tmp[1]) 
		{
			case 0x3e:	
				hdl_103e(tmp+2, len-2);		
				break;	//���ö�ʱ�ϴ���ʻ����		
			default:	
				PRTMSG(MSG_NOR, "Rcv unknown frm(%02x,%02x)!\n", tmp[0], tmp[1]);	
				break;
		}
	}

	// �г���¼��ҵ��Э��
	if(0x36 == tmp[0]) 
	{
		byte tmp2[2048] = {0};
		for(int k=0; k<len; k++)
		{
			if( 0x7f == tmp[k] )	
				tmp[k] = 0;
		}
		int len2 = CodeXToCode8(tmp2, tmp+2, len-2, 7);	//���û����ݽ���7->8��ת��
		switch(tmp[1]) 
		{
			case 0x01:	
				hdl_3601(tmp2, len2);	
				break;	//���ó�����Ϣ����		
			case 0x02:	
				hdl_3602(tmp2, len2);	
				break;	//�������ü�ʻԱ���			
			case 0x03:	
				hdl_3603(tmp2, len2);	
				break;	//����ƣ����ʻʱ��
			case 0x04:	
				hdl_3604(tmp2, len2);	
				break;	//��ѯ�¹��ɵ���������
			case 0x05:	
				hdl_3605(tmp2, len2);	
				break;	//��ѯ���15������ÿ����ƽ���ٶ�����	
			case 0x06:	
				hdl_3606(tmp2, len2);	
				break;	//��ѯ������ʻʱ���¼����
			case 0x11:	
				hdl_3611(tmp2, len2);	
				break;	//���ó��ٲ���
			case 0x13:	
				hdl_3613(tmp2, len2);	
				break;	//�ɼ�˾����Ϣ
			case 0x18:	
				hdl_3618(tmp2, len2);	
				break;
			//case 0x19:
			//	hdl_3619(tmp2, len2);	
			//	break; //���õ��ٲ���
			case 0x23:
				hdl_3623(tmp2, len2);
				break;
			default:	
				PRTMSG(MSG_NOR, "Rcv unknown frm(%02x,%02x)!\n", tmp[0], tmp[1]);	
				break;
		}
	}
	
	// �г���¼�Ǳ���ҵ��Э��
	if(0x37 == tmp[0]) 
	{
		byte tmp2[2048] = {0};
		int len2 = CodeXToCode8(tmp2, tmp+2, len-2, 7);	// ���û����ݽ���7->8��ת��
		switch(tmp[1]) 
		{
#if USE_OIL == 3
			case 0x10:
				g_objOil.Deal3710(tmp2, len2);
				break;
				
			case 0x11:
				g_objOil.Deal3711(tmp2, len2);
				break;
				
			case 0x12:
				g_objOil.Deal3712(tmp2, len2);
				break;
				
			case 0x13:
				g_objOil.Deal3713(tmp2, len2);
				break;
#endif
			case 0x01:	
				hdl_3701(tmp2, len2);	
				break;	// ������ʻʱ�������	
				
			case 0x02:	
				hdl_3702(tmp2, len2);	
				break;	// ���ñ�����������

#if USE_OIL == 3
			case 0x09:	
				hdl_3709(tmp2, len2);	
				break;	// �����ϴ���ʻ����Ӧ��(���ͺ�ֵ)
#else
			case 0x07:	
				hdl_3707(tmp2, len2);	
				break;	// �����ϴ���ʻ����Ӧ��(���ͺ�ֵ)
#endif
			default:	
				PRTMSG(MSG_NOR, "Rcv unknown frm(%02x,%02x)!\n", tmp[0], tmp[1]);	
				break;
		}
	}

	// Խ�籨����������ҵ��
	if(0x0f == tmp[0]) 
	{
		for(ulong i=2; i<len; i++)	
		{ 
			if(0x7f==tmp[i])	
				tmp[i] = 0; 
		}
		switch(tmp[1]) 
		{
			case 0x01:	
				hdl_0f01(tmp+2, len-2);		
				break;	// ��ʼ����Խ��·��	
			case 0x02:	
				hdl_0f02(tmp+2, len-2);		
				break;	// ����Խ��·��	
			case 0x03:	
				hdl_0f03(tmp+2, len-2);		
				break;	// ��������Խ��·��	
			case 0x11:	
				hdl_0f11(tmp+2, len-2);		
				break;	// ���Խ������	
			case 0x12:	
				hdl_0f12(tmp+2, len-2);		
				break;	// ��ʼ����Խ��·��	
			default:	
				PRTMSG(MSG_NOR, "Rcv unknown frm(%02x,%02x)!\n", tmp[0], tmp[1]);	
				break;
		}
	}
}

//------------------------------------------------------------------------------	
// ʵʱ�¹��ɵ��ϴ�Ӧ��(0117)
void CDriveRecord::hdl_0117(byte* data, int len)
{
	byte reply = byte(data[0]);
	if( 0x01 == (reply & 0x0f) )	m_v.rp_doub[0] = reply;
	if( 0x02 == (reply & 0x0f) )	m_v.rp_doub[1] = reply;
}

//------------------------------------------------------------------------------	
// ����ʽ�ɵ��ϴ�����(0127)
void CDriveRecord::hdl_0127(byte* data, int len)
{
	m_v.updoub = 2;		//����־λ���𼴿�
}

//------------------------------------------------------------------------------	
// ����̲�ѯ����(0901)
void CDriveRecord::hdl_0901(byte* data, int len)
{
	// ��������̲�ѯ����Ӧ��
	frm0941 frm = {0};
	frm.reply = 0x01;
	memcpy(frm.time, m_v.metertime, 3);
	sprintf(frm.inf, "%u", ulong(m_v.metertotal));
	frm.len = min(127, strlen(frm.inf) );
	tcp_send(0x09, 0x41, &frm, sizeof(frm)-sizeof(frm.inf)+frm.len);

	// �������д�������ļ���
	flush_headrcd();

	PRTMSG(MSG_NOR, "flush_headrcd : hdl_0901\n");
	PRTMSG(MSG_NOR, "Req total meter(%sm) succ!\n", frm.inf);
}

//------------------------------------------------------------------------------	
// ������������(0902)
void CDriveRecord::hdl_0902(byte* data, int len)
{
	// �������̺����ͳ�ƿ�ʼʱ��
	struct tm pCurrentTime;
	G_GetTime(&pCurrentTime);

	m_v.metertime[0] = (byte)(pCurrentTime.tm_year-100);
	m_v.metertime[1] = (byte)(pCurrentTime.tm_mon+1);
	m_v.metertime[2] = (byte)pCurrentTime.tm_mday;
	m_v.metertotal = 0;

	// �������д�������ļ���
	flush_headrcd();

	byte reply = 0x01;
	tcp_send(0x09, 0x42, &reply, 1);

	PRTMSG(MSG_NOR, "flush_headrcd : hdl_0902\n");
	PRTMSG(MSG_NOR, "Clear total meter succ!\n");
}


//------------------------------------------------------------------------------	
// ��ѯ���360Сʱ�ں�2�����������ʻ�������(0904)
void CDriveRecord::hdl_0904(byte* data, int len)
{
	char str1[128] = {0};
	char str2[128] = {0};
	sprintf(str1, "%.0f", m_v.m360.get_meter());
	sprintf(str2, "%.0f", m_v.m2d.get_meter());
	byte len1 = min(126, strlen(str1));
	byte len2 = min(126, strlen(str2));

	int i = 0;
	byte frm[1024] = {0};
	frm[i++] = 0x01;		//Ӧ������
	frm[i++] = len1;		//�����Ϣ����1
	memcpy(frm+i, str1, len1);	//�����Ϣ1
	i += len1;
	frm[i++] = len2;		//�����Ϣ����2
	memcpy(frm+i, str2, len2);	//�����Ϣ2
	i += len2;
	tcp_send(0x09, 0x44, frm, i);
	PRTMSG(MSG_NOR, "Req 360(%sm, %sm) succ!\n", str1, str2);
}


//------------------------------------------------------------------------------	
// ��̼�¼��ѯ(0905)
// ʱ�䷶Χ����(1) + ʱ�䷶Χ(10) + ��ʻԱ��ݱ���(1)
void CDriveRecord::hdl_0905(byte* data, int len)
{
	ulong i=0;

	if(0==m_quemeter.getcount())
	{
		frm0905 *p = (frm0905*)data;
		RcdDrv rcd = {0};
		ulong cnt = m_v.drvcnt;
		ulong pw = m_v.drvpw;
		
		// ���Ҽ�¼,������Ҫ��ļ�¼���������
		FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");
		if(NULL!=fp) 
		{
			for(ulong i=0; i<cnt; i++) 
			{
				if(--pw>=200)	pw = 199;

				fseek(fp, offsetof(RcdFile,drv)+pw*sizeof(RcdDrv), 0);
				fread(&rcd, sizeof(rcd), 1, fp);

				if(chk_drvrcd2(&rcd, p->type, p->s_time, p->e_time, p->numdrv))
				{
					m_quemeter.push((byte*)&rcd, sizeof(rcd));
				}
			}
			fclose(fp);
		}

		if( 0 == m_quemeter.getcount() )
		{
			byte reply = 0x7f;
			tcp_send(0x09, 0x45, &reply, sizeof(reply));
		}

		PRTMSG(MSG_NOR, "Queryed %d meter(%u,%u) rcd!", m_quemeter.getcount(), p->type, p->numdrv);
	} 
	else
	{
		byte reply = 0x7f;
		tcp_send(0x09, 0x45, &reply, sizeof(reply));

		PRTMSG(MSG_NOR, "Meter is query...");
	}
}
//------------------------------------------------------------------------------	
// ���ö�ʱ�ϴ���ʻ����(103e)
// �������ݱ�ʶ(1) + ״̬��ʶ(1) + �ϱ���ʱ��(2) + ��������(2) + ��������(2)
//�������ݱ�ʶ��0x01: �����ķ���GPS����   0x02: �������ķ���GPS����
//״̬��ʶ��    0x01  ��̨�����ƶ�״̬    0x02  ��̨���ھ�ֹ״̬ 
//              0x03  ��̨���ڿճ�״̬    0x04  ��̨�����س�״̬
void CDriveRecord::hdl_103e(byte* data, int len)
{
	frm103e *p = (frm103e*)data;

#if 1	// ���ο��س�״̬����
	if(1!=p->sta_id && 2!=p->sta_id)	return;
#endif

	if(p->sta_id>=1 && p->sta_id<=6) 
	{
		m_cfg.sta_id = p->sta_id;
		m_cfg.uptime[m_cfg.sta_id-1] = p->uptime;
		m_cfg.cltcyc[m_cfg.sta_id-1] = p->collect;

		if (p->send>4) 
		{
			show_diaodu("���������ϱ���������");
			play_tts("���������ϱ���������");
			PRTMSG(MSG_NOR, "CDriveRecord::hdl_103e sndcyc>4 err.\n");
			
			return;
		}
		m_cfg.sndcyc[m_cfg.sta_id-1] = p->send;
	}

	if(0x02==p->up)		m_cfg.sta_id = 0;

	// ��������
	uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
	int ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );	

	PRTMSG(MSG_NOR, "Set updrv %s StaId=%d,CltCyc=%d,SndCyc=%d. \n", ( (1==p->sta_id) ? "AccOn" : "AccOff"),p->sta_id, p->collect, p->send);
}


//------------------------------------------------------------------------------	
// ���ó�����Ϣ����(3601)
// ��������ϵ��(2)+����VIN�ų���(1)+ ����VIN��+
// �������볤��(1)+��������+���Ʒ��೤��(1)+���Ʒ���
void CDriveRecord::hdl_3601(byte* data, int len)
{
	byte len2 = 0;

	memcpy(&m_cfg.factor, data, 2);
	m_cfg.factor = ntohs(m_cfg.factor);
	data += 2;

	len2 = *(data++);
	memcpy(&m_cfg.vin, data, len2 );
	data += len2;

	len2 = *(data++);
	memcpy(&m_cfg.num, data, len2 );
	data += len2;

	len2 = *(data++);
	memcpy(&m_cfg.type, data, len2 );
	data += len2;	
	
	// ��������
	uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
	int ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );

	// �����ķ���Ӧ��
	char reply = (0==ret) ? 0x01 : 0x7f;
	tcp_send(0x36, 0x41, &reply, 1);

	PRTMSG(MSG_NOR, "Set carinfo(%u,%.17s,%.12s,%.12s) %s!\n", m_cfg.factor, 
		m_cfg.vin, m_cfg.num, m_cfg.type, (0==ret) ? "succ" : "fail");	
}


//------------------------------------------------------------------------------
// �������ü�ʻԱ���(3602)
// ������(1)+���ø���(1)+�����(1)+���(1)+
// ��ݴ��볤��(1)+��ݴ���+��ʻ֤�ų���(1)+��ʻ֤�š�* N
void CDriveRecord::hdl_3602(byte* data, int len)
{
	byte *p = data;	
	int cnt = 0;
	int i=0;
	int iMaxAdd = 0;
	BYTE bytLen = 0;
	int ret = 0;

	DrvCfg cfg = m_cfg;

	// ������
	if( *p & 0x01 )
	{
		cfg.total = 0;

		for(int i=0; i<10; i++)	
			cfg.inf[i].valid = false;
	}
	p++;

	if( cfg.total > sizeof(cfg.inf) / sizeof(cfg.inf[0]) )
	{
		cfg.total = 0;
		for(int i=0; i<10; i++)	
			cfg.inf[i].valid = false;
	}

	// ���ø���
	iMaxAdd = sizeof(cfg.inf) / sizeof(cfg.inf[0]) - cfg.total;
	cnt = *p;
	if( cnt > iMaxAdd )
	{
		PRTMSG(MSG_NOR, "Set driver fail! Over Max Size!\n");
		ret = -1;
		goto HDL3602_END;
	}
	p++;

	// ���ø���  
	if(cnt>4)
	{
		PRTMSG(MSG_NOR, "Set driver fail! Set cnt err!\n");
		ret = -1;
		goto HDL3602_END;
	}
	else
	{
		PRTMSG(MSG_NOR, "Set driver cnt = %d\n", cnt);
	}

	while(cnt--)
	{
		// ��ȡ�յļ�¼
		int empty = -1;
		for(; i<10; i++)
		{
			if(false == cfg.inf[i].valid)
			{ 
				empty = i; 
				break; 
			}
		}

		if(-1 == empty)
		{
			PRTMSG(MSG_NOR, "Set driver fail! Cann't get empty rcd!\n");
			ret = -1; 
			goto HDL3602_END;
		}
		
		// ���ռ�¼

		//���
		if(*p > 9)
		{
			PRTMSG(MSG_NOR, "Set driver fail! index err(%u)!\n",*p);
			ret = -1;
			goto HDL3602_END;
		}
		p++;

		//���
		cfg.inf[empty].num = *p;
		if(cfg.inf[empty].num > 126)
		{
			PRTMSG(MSG_NOR, "Set driver fail! num err(%u)!\n", cfg.inf[empty].num);
			ret = -1;
			goto HDL3602_END;
		}
		p++;

		//��ݴ���
		bytLen = *p;
		if( bytLen > 7 )
		{
			PRTMSG(MSG_NOR, "Set driver fail! ID Len err(%u)!\n", bytLen);
			ret = -1;
			goto HDL3602_END;
		}
		p++;
		memset(cfg.inf[empty].ascii, 0, sizeof(cfg.inf[empty].ascii) );
		memcpy(cfg.inf[empty].ascii, p, bytLen );
		p += bytLen;

		//��ʻ֤��
		bytLen = *p;
		if( bytLen > 18 )
		{
			PRTMSG(MSG_NOR, "Set driver fail! Driver Code err(%u)!\n", bytLen);
			ret = -1;
			goto HDL3602_END;
		}
		p++;
		memset(cfg.inf[empty].code, 0, sizeof(cfg.inf[empty].code) );
		memcpy(cfg.inf[empty].code, p, bytLen );		//��ʻ֤��
		p += bytLen;

		cfg.inf[empty].valid = true;
		
		// ���Ӽ���
		cfg.total++;

		// �Ϸ��Լ��
		if( (p-data) > len)
		{
			PRTMSG(MSG_NOR, "Set driver fail! data err(%u)!\n");
			ret = -1;
			goto HDL3602_END;
		}
	}
	
	// ��������
	ret = SetSecCfg(&cfg, offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered), sizeof(cfg) );

HDL3602_END:
	// �����ķ���Ӧ��
	char reply = (0==ret) ? 0x01 : 0x7f;
	tcp_send(0x36, 0x42, &reply, 1);

	if(0==ret)
	{
		m_cfg = cfg;
		PRTMSG(MSG_NOR, "Set driver %s", (0==cnt) ? "finished!" : "...\n");
	}
	else
	{
		PRTMSG(MSG_NOR, "Set driver fail!\n");
	}
}


//------------------------------------------------------------------------------
// ����ƣ����ʻʱ��(3603)
// ƣ����ʻʱ��(1)+˾����Ϣ���ʱ��(1)
// ƣ����ʻʱ�䣺��λΪСʱ����Ϊ0�����ʾ������ƣ����ʻ���
// ˾����Ϣ���ʱ�䣺��λΪ����
void CDriveRecord::hdl_3603(byte* data, int len)
{
	m_cfg.max_tire = *data++;
	m_cfg.min_sleep = *data++;
	
	// ��������
	uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
	int ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );

	// �����ķ���Ӧ��
	char reply = (0==ret) ? 0x01 : 0x7f;
	tcp_send(0x36, 0x43, &reply, 1);

	PRTMSG(MSG_NOR, "Set tire time(%u,%u) %s!\n", m_cfg.max_tire, m_cfg.min_sleep, (0==ret) ? "succ" : "fail");
}


//------------------------------------------------------------------------------
// ��ѯ�¹��ɵ���������(3604)
// ��ѯ��(1)+��ѯʱ��θ���(1)+����ѯʱ��Ρ�+��ѯ��Χ����(1)+��λ�÷�Χ��
// ��ѯʱ��Σ����ʱ��(5)+�յ�ʱ��(5)
// λ�÷�Χ�����½Ǿ��ȡ�γ��(8)+���ϽǾ��ȡ�γ��(8)
void CDriveRecord::hdl_3604(byte* data, int len)
{
	if(0==m_quedoub.getcount())
	{
		// ����Э��
		byte attrib = *data++;
		
		byte timecnt = *data++;
		if(timecnt>10)		return;
		byte time[10][10] = {0};
		memcpy(time, data, timecnt*10);
		data += timecnt*10;
		
		byte regioncnt = *data++;
		if(regioncnt>5)		return;
		byte region[5][16] = {0};
		memcpy(region, data, regioncnt*16);
		data += regioncnt*16;
			
		// ���Ҽ�¼,������Ҫ��ļ�¼���������
		RcdDoub rcd = {0};
		ulong cnt = m_v.doubcnt;
		ulong pw = m_v.doubpw;	
		
		FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");
		if(NULL!=fp)
		{
			for(ulong i=0; i<cnt; i++)
			{
				if(--pw >100)		pw = 99;

				fseek(fp, offsetof(RcdFile,doub)+pw*sizeof(RcdDoub), 0);
				fread(&rcd, sizeof(rcd), 1, fp);
				
				if(chk_doubrcd(&rcd,timecnt,time,regioncnt,region))
				{
					m_quedoub.push((byte*)&rcd, sizeof(rcd));
				}
			}
			fclose(fp);
		}	
		
		if( (0==m_quedoub.getcount()) && (attrib & 0x01) )
		{
			byte reply = 0x7f;
			tcp_send(0x36, 0x44, &reply , sizeof(reply) );
		}

		PRTMSG(MSG_NOR, "Queryed %d doub(%u,%u) rcd!\n", m_quedoub.getcount(), timecnt, regioncnt);
	}
	else 
	{
		byte reply = 0x7f;
		tcp_send(0x36, 0x44, &reply , sizeof(reply) );
		PRTMSG(MSG_NOR, "Doub is query...!\n");
	}
}


//------------------------------------------------------------------------------
// ��ѯ���15������ÿ����ƽ���ٶ�����(3605)
void CDriveRecord::hdl_3605(byte* data, int len)
{
	frm3645 frm = {0};
	frm.reply = 0x01;
	memcpy(frm.average, m_v.average, 15);
	
	tcp_send(0x36, 0x45, &frm, sizeof(frm) );	
	
	PRTMSG(MSG_NOR, "Query 15speed succ!\n");
}


//------------------------------------------------------------------------------
// ��ѯ������ʻʱ���¼����(3606)
// ˾�����(1)+������ʻʱ��(1)
// ˾����ţ���Ϊ0�����ʾ��ѯ����˾�����
// ������ʻʱ�䣺��λΪСʱ
void CDriveRecord::hdl_3606(byte* data, int len)
{
	if( 0 == m_quedrv.getcount() )
	{
		frm3606 *p = (frm3606*)data;
		RcdDrv rcd = {0};
		ulong cnt = m_v.drvcnt;
		ulong pw = m_v.drvpw;
		
		// ���Ҽ�¼,������Ҫ��ļ�¼���������
		FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");
		if(NULL!=fp) 
		{
			for(ulong i=0; i<cnt; i++) 
			{
				if(--pw>=200)		pw = 199;

				fseek(fp, offsetof(RcdFile,drv)+pw*sizeof(RcdDrv), 0);
				fread(&rcd, sizeof(rcd), 1, fp);

				if(chk_drvrcd(&rcd,p->numdrv,p->times))
				{
					m_quedrv.push((byte*)&rcd, sizeof(rcd));
				}
			}

			fclose(fp);
		}

		if( 0 == m_quedrv.getcount() ) 
		{
			byte reply = 0x7f;
			tcp_send(0x36, 0x46, &reply, sizeof(reply));
		}

		PRTMSG(MSG_NOR, "Queryed %d drv(num:%u,%uh) rcd!\n", m_quedrv.getcount(), p->numdrv, p->times);		
	} 
	else 
	{
		byte reply = 0x7f;
		tcp_send(0x36, 0x46, &reply, sizeof(reply));
		PRTMSG(MSG_NOR, "Drv is query...\n");
	}	
}


//------------------------------------------------------------------------------
// ���ó��ٲ���
// �����ٶ�(1) + ���ټ���ʱ��(1)
// �����ٶȣ���λΪ����/Сʱ���ٶ���Ϊ0�����ʾ�����г����ж�
// ���ټ���ʱ�䣺��λΪ�룻��ʾ���ٳ����������ʱ�䣬���ж�Ϊ���١�
void CDriveRecord::hdl_3611(byte* data, int len)
{
	m_cfg.max_speed = *data++;
	m_cfg.min_over = *data++;
	
	// ��������
	uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
	int ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );

	// �����ķ���Ӧ��
	char reply = (0==ret) ? 0x01 : 0x7f;
	tcp_send(0x36, 0x51, &reply, 1);

	PRTMSG(MSG_NOR, "Set overspeed(%usm/h,%us) %s!\n", m_cfg.max_speed, 
		m_cfg.min_over, (0==ret) ? "succ" : "fail");
}


//------------------------------------------------------------------------------
// ���õ��ٲ���
// �����ٶ�(1) + ���ټ���ʱ��(1)
// �����ٶȣ���λΪ����/Сʱ���ٶ���Ϊ255�����ʾ�����е����ж�
// ���ټ���ʱ�䣺��λΪ���ӣ���ʾ���ٳ����������ʱ�䣬���ж�Ϊ���١�
void CDriveRecord::hdl_3619(byte* data, int len)
{
	m_cfg.min_speed = *data++;
	m_cfg.min_below = *data++;
	
	// ��������
	uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
	int ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );

	// �����ķ���Ӧ��
	char reply = (0==ret) ? 0x01 : 0x7f;
	tcp_send(0x36, 0x59, &reply, 1);

	PRTMSG(MSG_NOR, "Set belowspeed(%ukm/h,%um) %s!\n", m_cfg.min_speed, m_cfg.min_below, (0==ret) ? "succ" : "fail");
}

//------------------------------------------------------------------------------
// ������չ����
// ��ǰ�汾��(1B) +����չ����(2B) + ����(2B) + ���ݡ�
//		��ǰ�汾��(1B)����ǰ�汾��Ϊ1��
//		��չ���ͣ�
//				0x0001--���ó�̨�����ٶȣ�
//				�����ٶ�(1B)����λΪ����/Сʱ��
//				����ʱ��(2B)����λΪ���ӣ����ֽ���ǰ���������ٳ������ʱ�䣬���ж�Ϊ���١������ٶ�Ϊ0��ʾֹͣ���ٱ�����

void CDriveRecord::hdl_3623(byte* data, int len)
{
	char	szVer = 0;
	char	sztmpbuf[2] = {0};
	short	shType = 0;

	char	szRes = 1;
	char	szReplyBuf[200] = {0};
	int		iReplyLen = 0;

	// �汾��
	szVer = (char)data[0];

	for(int i=1; i<len; )
	{
		// ��ȡ��չ����
		sztmpbuf[0] = (char)data[i+1];
		sztmpbuf[1] = (char)data[i];
		i += 2;

		memcpy((void*)&shType, sztmpbuf, 2);

		switch( shType )
		{
		case 0x0001:	// ���ٱ�������
			{
				//����������
				if( len != 8 )
				{
					szRes = 0;
					goto _DEAL_3623_END;
				}

				// ���������ֶ�
				i += 2;

				//�����ٶ�
				m_cfg.min_speed = byte(data[i]*1.852);
				i += 1;

				// ����ʱ��
				m_cfg.min_below = 256*data[i] + data[i+1];
				i += 2;

				// ��������
				uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
				int ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );
			}
			break;

		case 0x0002:	// ����Χ������
		case 0x0003:	// ����ͣ��ʱ������
		default:
			break;
		}
	}

_DEAL_3623_END:
	// ������Ӧ��
	szReplyBuf[iReplyLen++] = szVer;		// �汾��
	szReplyBuf[iReplyLen++] = sztmpbuf[0];	// ��չ����
	szReplyBuf[iReplyLen++] = sztmpbuf[1];
	szReplyBuf[iReplyLen++] = szRes;		// Ӧ������
	switch( shType )
	{
	case 0x0001:
		szReplyBuf[iReplyLen++] = 0;		// ��չ��Ϣ���ӳ���Ϊ0
		break;

	case 0x0002:
	case 0x0003:
	default:
		break;
	}

	tcp_send(0x36, 0x63, szReplyBuf, iReplyLen);
	PRTMSG(MSG_NOR, "Set 3623 belowspeed(%ukm/h,%um) %s!\n", m_cfg.min_speed, m_cfg.min_below, (1==szRes) ? "succ" : "fail");
}

//------------------------------------------------------------------------------
// �ɼ�˾����Ϣ
void CDriveRecord::hdl_3613(byte* data, int len)
{
	int i = 0;
	byte frm[1024] = {0};

	int index = get_drvindex(m_v.numdrv);
	
	frm[i++] = byte(m_v.numdrv);	//˾�����
	
	if(-1 != index)
	{
		int asclen = min(7, strlen(m_cfg.inf[index].ascii) );
		frm[i++] = asclen;		//��ݴ��볤��
		memcpy(frm+i, m_cfg.inf[index].ascii, asclen);	//��ݴ���
		i += asclen;

		int codlen = min(18, strlen(m_cfg.inf[index].code) );
		frm[i++] = codlen;		//��ʻ֤�ų���
		memcpy(frm+i, m_cfg.inf[index].code, codlen);	//��ʻ֤��
		i += codlen;
	}
	else 
	{
		frm[i++] = 0;		//��ݴ��볤��
		frm[i++] = 0;		//��ʻ֤�ų���
	}

	tcp_send(0x36, 0x53, frm, i);

	PRTMSG(MSG_NOR, "Collect driver info %s(idx:%d)!\n",(-1!=index) ? "succ" : "fail", index);
}

void CDriveRecord::hdl_3618(byte* data, int len)
{
// ���ڴ�С��1��+ ACCON�������ڣ�2��+ ACCON�������ڣ�1��+ ACCOFF�������ڣ�2��+ ACCOFF�������ڣ�1��

	BYTE bytResType = 0x01;
	uint offset = 0;
	int ret = 0;

	BYTE bytWinSize = data[0];
	if( bytWinSize > 32 )
	{
		bytResType = 0x7f;
		goto HDL3618_END;
	}

	if( 0x7f == data[1] ) data[1] = 0;
	if( 0x7f == data[2] ) data[2] = 0;
	m_cfg.cltcyc[0] = data[1] * 60 + data[2];

	if( 0x7f == data[3] ) data[3] = 0;
	m_cfg.sndcyc[0] = data[3];

	m_cfg.uptime[0] = 0xffff;

	if( 0x7f == data[4] ) data[4] = 0;
	if( 0x7f == data[5] ) data[5] = 0;
	m_cfg.cltcyc[1] = data[4] * 60 + data[5];
	
	if( 0x7f == data[3] ) data[3] = 0;
	m_cfg.sndcyc[1] = data[6];

	m_cfg.uptime[1] = 0xffff;

	m_cfg.sta_id = ( 1 == m_cfg.sta_id) ? 2 : 1;
	
	// ��������
	offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
	ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );
	if( ret ) bytResType = 0x7f;
	
HDL3618_END:
	tcp_send(0x36, 0x58, &bytResType, 1);
}

//------------------------------------------------------------------------------
// ������ʻʱ�������(3701)
// ������(1) + ���ø���(1) + ��ʱ��α��(1) + ʱ���(4)��
void CDriveRecord::hdl_3701(byte* data, int len)
{
	frm3701 *p = (frm3701*)data;

	// �������ʱ�������
	if(p->ctl & 0x01)	memset(m_v.almtime, 0, sizeof(m_v.almtime));

	for(int i=0; i<min(20,p->cnt); i++) 
	{
		byte no = min(19, p->time[i][0]);	//ʱ��α��
		memcpy(&m_v.almtime[no][0], &p->time[i][1], 4);
	}

	byte reply = flush_headrcd() ? 0x01 : 0x7f;
	tcp_send(0x37, 0x41, &reply, 1);

	PRTMSG(MSG_NOR, "Set drvtime(%u) %s!\n", p->cnt, (0x01==reply) ? "succ" : "fail");
}



//------------------------------------------------------------------------------
// ���ñ�����������(3702)
// ֡���(2) + ������(1) + ��������(2) + ��������(1) + �����ٶ�(1) + ��������(2) + 
// ������㾭��(4) + �����γ��(4)����
void CDriveRecord::hdl_3702(byte* data, int len)
{
	static bool setting = false;
	static ushort nextno = 0;
	static int lstcnt = 0;
	static ulong tm_set = GetTickCount();
	static Range rg = {0};
	
	frm37021 *p = (frm37021*)data;
	bool succ = false;
	
	while(1) 
	{
		// �ж��Ƿ�������״̬(���ó�ʱ���˳�����״̬)
		if(setting && (GetTickCount()-tm_set)<10*60*1000) 
		{
			if(nextno!=ntohs(p->frmno))	
			{
				PRTMSG(MSG_ERR, "p->frmno(%u)!=0\n", ntohs(p->frmno)); 
				break; 
			}
			nextno++;	
		} 
		else
		{
			if(0!=ntohs(p->frmno))	
			{ 
				PRTMSG(MSG_ERR, "p->frmno(%u)!=nextno(%u)\n", ntohs(p->frmno), nextno);
				break;
			}
			nextno = 1;
			setting = true;
			tm_set = GetTickCount();
		}
		
		// ֡��Ϊ0,����������Ϊ���,��������������¼
		if((p->ctl&0x02) && (ntohs(p->frmno)==0) ) 
		{ 
			m_v.r_used = 0; 
			m_v.d_used = 0;
			flush_headrcd();
			PRTMSG(MSG_ERR, "Clear all area rcd!\n"); 
		}
		
		// �ж����ý�����־
		if(p->ctl & 0x01)
		{ 
			setting = false;
			succ = true; 
			break; 
		}
		
		// ������������
		if( 0==lstcnt ) 
		{
			rg.areano = ntohs(p->areano);
			rg.attr = p->attr;
			rg.vector = p->vector;
			rg.cnt = ntohs(p->cnt);
			rg.n_start = m_v.d_used;
			rg.n_end = rg.n_start + rg.cnt;

			if(m_v.r_used>=200)	//�������Ƿ�����
			{ 
				PRTMSG(MSG_ERR, "m_v.r_used(%u)>=200", m_v.r_used);
				break; 
			}	

			if(rg.cnt>200)		//���ø����Ƿ�̫��
			{
				PRTMSG(MSG_ERR, "rg.cnt(%u)>200", rg.cnt); 
				break; }	
			
			if(rg.n_end>=800)	//�������Ƿ񲻹���
			{
				PRTMSG(MSG_ERR, "rg.n_end(%u)>=800", rg.n_end);
				break; 
			}		
		} 
		
		// �������洢�����
		FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");
		if(NULL!=fp)
		{
			if(0==lstcnt)
				lstcnt = rg.cnt;

			for(int i=0; i<=lstcnt; i++) //�洢�����
			{	
				int offset = (lstcnt==rg.cnt) ? offsetof(frm37021,dot)+i*8 : offsetof(frm37022,dot)+i*8 ;
				if(offset>=len)
				{ 
					lstcnt = lstcnt - i;
					break;
				}

				fseek(fp, offsetof(RcdFile, dot)+(rg.n_start+rg.cnt-lstcnt+i)*8, 0);
				fwrite(data+offset, 1, 8, fp);
			}

			if(0==lstcnt)	//�洢��������
			{		
				fseek(fp, offsetof(RcdFile, range)+m_v.r_used*sizeof(Range), 0);
				fwrite(&rg, 1, sizeof(rg), fp);
			
				m_v.d_used = rg.n_end;
				if(++m_v.r_used>=200)	m_v.r_used = 199;

				fseek(fp, offsetof(RcdFile, head), 0);
				fwrite(&m_v, 1, sizeof(m_v), fp);
			
				PRTMSG(MSG_ERR, "save area%u(%u-%u) data!\n", m_v.r_used, rg.n_start, rg.n_end);
			}
			fclose(fp);
		} 
		else 
		{
			break;
		}
		
		succ = true;
		break;		//�˴�while����ѭ��,���������˳�
	}
	
	// ��������״̬
	if(!succ || !setting)
	{
		setting = false;
		nextno = 0;
		lstcnt = 0;	
		tm_set = GetTickCount();
		PRTMSG(MSG_ERR, "set alarm area %s!\n", (!succ) ? "fail" : "finish");
	}
	
	// �������ñ�����������Ӧ��
	frm3742 frm = {0};
	frm.reply = succ ? 0x01 : 0x7f;
	frm.frmno = p->frmno;
	tcp_send(0x037, 0x42, &frm, succ ? 1 : 2 );
}


//------------------------------------------------------------------------------
// �����ϴ���ʻ����Ӧ��(���ͺ�ֵ)
// Ӧ������(1) + ֡�������(4)
void CDriveRecord::hdl_3707(byte* data, int len)
{
	if( 0x01 == *data )
	{
		m_v.rp_oil[0] = *data;	
	} 
	else
	{
		memcpy(m_v.rp_oil, data, 5);
	}
}

void CDriveRecord::hdl_3709(byte *data, int len)
{
	if( 0x01 == *data )
	{
		m_v.rp_oil[0] = *data;	
	} 
	else
	{
		memcpy(m_v.rp_oil, data, 5);
	}
}

//------------------------------------------------------------------------------
// ��ʼ����Խ��·��
// ���ñ�־��1��
void CDriveRecord::hdl_0f01(byte* data, int len)
{

}


//------------------------------------------------------------------------------
// ����Խ��·��
// ���������1��+ [Խ��·������1 + ���� + Խ��·������n]
void CDriveRecord::hdl_0f02(byte* data, int len)
{

}

//------------------------------------------------------------------------------
// ��������Խ��·��
// �ܹ��������������1��
void CDriveRecord::hdl_0f03(byte* data, int len)
{

}


//------------------------------------------------------------------------------
// ���Խ������
// ��
void CDriveRecord::hdl_0f11(byte* data, int len)
{
	tag2QAreaCfg objAreaCfg;
	int iret;
	byte bytRes = 0x7f;

	objAreaCfg.Init();

	// д������
	iret = SetSecCfg((void*)&objAreaCfg, offsetof(tagSecondCfg, m_uni2QAreaCfg.m_obj2QAreaCfg), sizeof(objAreaCfg));
	if( !iret )
	{
		bytRes = 0x01;
		memset((void*)&m_objAreaCfg, 0, sizeof(m_objAreaCfg));
	}

	
	tcp_send(0x0f, 0x51, &bytRes, 1);
}


//------------------------------------------------------------------------------
// ����Խ������
// ���������1�� + [Խ������1 + ���� + Խ������n]
/*
��������	1	�ֽڸ�ʽ���壺01NNNNAA
			NNNN�������ţ�ȡֵ��Χ0000~1111
			AA����λ����
			00-�����ڶ�λ���߼��룩
			01-�����ڶ�λ���߼���
			10-�����ⶨλ���߼��룩
			11-�����ⶨλ���߼���

Hbit1		1	�ֽڸ�ʽ���壺01GDDDDD
			G����Ųο��㾭�ȶȷ������λ
			DDDDD����ž��ȷ���������ȵĶȷ�����ȡֵ��Χ��00000~11111

���ȷ���
������	3	�����֣�1��+ С���֣�2��

Hbit2		1	�ֽڸ�ʽ����Ϊ��010DDDDD
			DDDDD�����γ�ȷ���������ȵĶȷ�����ȡֵ��Χ��00000~11111
γ�ȷ���
������	3	�����֣�1��+ С���֣�2��

�ο��㾭��γ��	8	�ο��㾭�ȣ�4��+ �ο���γ�ȣ�4��

��ע���ο���ȡ��������½ǣ��ο��㾭��γ�Ȳμ���׼GPS�����еľ���γ�ȶ��塣
��ע�������֡�С���ֶ���ɲμ���׼GPS�����еľ���γ�ȶ��塣
*/
void CDriveRecord::hdl_0f12(byte* data, int len)
{
	tag2QAreaCfg objAreaCfg;
	tag0f12Area objOneArea;
	byte bytAreaCnt = 0;
	int  iret = 0;
	byte bytRes = 0x7f;		// Ĭ��Ϊʧ��Ӧ��

	objAreaCfg.Init();
	bytAreaCnt = data[0];

	// ��Χ����1��6
	if( bytAreaCnt > 6 || bytAreaCnt < 1 )
	{
		PRTMSG(MSG_ERR, "Deal0f12 err, Area Cnt uncorrect!\n");
		goto _DEAL0F12_END;
	}

	// �������ȼ��
	if( len < sizeof(objOneArea)*bytAreaCnt + 1 )
	{
		PRTMSG(MSG_ERR, "Deal0f12 err, data len uncorrect!\n");
		goto _DEAL0F12_END;
	}

	for(int i=0; i<bytAreaCnt; i++)
	{
		double dTemp = 0.0;
		BYTE bytTemp = 0;

		memcpy( (void*)&objOneArea, data+1+i*sizeof(objOneArea), sizeof(objOneArea) );

		objAreaCfg.m_aryAreaProCode[i] = objOneArea.m_bytAreaProCode;
				
		// ���¾���
		bytTemp = (0x20 & objOneArea.m_bytLonMix ? 0x80 : 0) + objOneArea.m_bytLonDu;
		if( 0xff == bytTemp ) bytTemp = 0;
		else if( 0xfe == bytTemp ) bytTemp = 0x80;
		if( 0x7f == objOneArea.m_bytLonFenzen ) objOneArea.m_bytLonFenzen = 0;
		if( 0x7f == objOneArea.m_bytLonFenxiao1 ) objOneArea.m_bytLonFenxiao1 = 0;
		if( 0x7f == objOneArea.m_bytLonFenxiao2 ) objOneArea.m_bytLonFenxiao2 = 0;
		dTemp = objOneArea.m_bytLonFenzen + objOneArea.m_bytLonFenxiao1 * 0.01 + objOneArea.m_bytLonFenxiao2 * 0.0001;
		objAreaCfg.m_aryArea[ i ][ 0 ] = long( (bytTemp + dTemp / 60) * LONLAT_DO_TO_L );
				
		// ����γ��
		bytTemp = objOneArea.m_bytLatDu;
		if( 0x7f == bytTemp ) bytTemp = 0;
		if( 0x7f == objOneArea.m_bytLatFenzen ) objOneArea.m_bytLatFenzen = 0;
		if( 0x7f == objOneArea.m_bytLatFenxiao1 ) objOneArea.m_bytLatFenxiao1 = 0;
		if( 0x7f == objOneArea.m_bytLatFenxiao2 ) objOneArea.m_bytLatFenxiao2 = 0;
		dTemp = objOneArea.m_bytLatFenzen + objOneArea.m_bytLatFenxiao1 * 0.01 + objOneArea.m_bytLatFenxiao2 * 0.0001;
		objAreaCfg.m_aryArea[ i ][ 1 ] = long( (bytTemp + dTemp / 60) * LONLAT_DO_TO_L );
		
		// ���Ͼ���
		if( 0x7f == objOneArea.m_bytLonWidFenZhen ) objOneArea.m_bytLonWidFenZhen = 0;
		if( 0x7f == objOneArea.m_bytLonWidFenxiao1 ) objOneArea.m_bytLonWidFenxiao1 = 0;
		if( 0x7f == objOneArea.m_bytLonWidFenxiao2 ) objOneArea.m_bytLonWidFenxiao2 = 0;
		bytTemp = objOneArea.m_bytLonMix & 0x1f;
		dTemp = objOneArea.m_bytLonWidFenZhen + objOneArea.m_bytLonWidFenxiao1 * 0.01 + objOneArea.m_bytLonWidFenxiao2 * 0.0001;
		objAreaCfg.m_aryArea[ i ][ 2 ] = objAreaCfg.m_aryArea[ i ][ 0 ] + long( (bytTemp + dTemp / 60) * LONLAT_DO_TO_L );
		
		// ����γ��
		bytTemp = objOneArea.m_bytLatMix & 0x1f;
		if( 0x7f == objOneArea.m_bytLatHeiFenZhen ) objOneArea.m_bytLatHeiFenZhen = 0;
		if( 0x7f == objOneArea.m_bytLatHeiFenxiao1 ) objOneArea.m_bytLatHeiFenxiao1 = 0;
		if( 0x7f == objOneArea.m_bytLatHeiFenxiao2 ) objOneArea.m_bytLatHeiFenxiao2 = 0;
		dTemp = objOneArea.m_bytLatHeiFenZhen + objOneArea.m_bytLatHeiFenxiao1 * 0.01 + objOneArea.m_bytLatHeiFenxiao2 * 0.0001;
		objAreaCfg.m_aryArea[ i ][ 3 ] = objAreaCfg.m_aryArea[ i ][ 1 ] + long( (bytTemp + dTemp / 60) * LONLAT_DO_TO_L );

		objAreaCfg.m_bytAreaCount ++;

		PRTMSG(MSG_DBG, "i=%d, %ld, %ld, %ld, %ld\n", i, objAreaCfg.m_aryArea[i][0], objAreaCfg.m_aryArea[i][1],objAreaCfg.m_aryArea[i][2], objAreaCfg.m_aryArea[i][3]);
	}

	// д������
	iret = SetSecCfg((void*)&objAreaCfg, offsetof(tagSecondCfg, m_uni2QAreaCfg.m_obj2QAreaCfg), sizeof(objAreaCfg));
	if( !iret )
	{
		bytRes = 0x01;

		// �����µ�����
		memset((void*)&m_objAreaCfg, 0, sizeof(m_objAreaCfg));
		memcpy((void*)&m_objAreaCfg, (void*)&objAreaCfg, sizeof(objAreaCfg));
	}

_DEAL0F12_END:
	tcp_send(0x0f, 0x52, &bytRes, 1);
}


//------------------------------------------------------------------------------
// ����������г���¼�ǵ�Э��
void CDriveRecord::hdl_diaodu(byte* data, int len)
{
	char szbuf[100] = {0};
	szbuf[0] = 0x01;		// ʹ���˴�����չ�У���ʾҪ���͵�������������

	switch(*data) 
	{
	case 0x61: // �����ٶ�
		{
			frm60 frm;
			frm.cmd = 0x60;
			frm.reply = 0x01;
			frm.speed = m_v.vector[0];	//ACC��Ч���ٶ���Ϊ0

			memcpy(szbuf+1, (char*)&frm, sizeof(frm));
			DataPush(szbuf, sizeof(frm)+1, DEV_QIAN, DEV_DIAODU, LV2);
		}
		break;

	case 0x63: // ����˾����¼Ӧ��
		{
			frm63 *p = (frm63*)data;

			if(0x01 == p->reply)	
			{
				m_v.inputflag = htons(p->numdrv);
				PRTMSG(MSG_NOR, "DrvLog m_v.inputflag=%d.\n",	m_v.inputflag);
			}
			else 
				m_v.inputflag = -1;
		}
		break;

	case 0x65: // ����˾�����
		{
			frm64 frm = {0};
			frm.cmd = 0x64;
			frm.reply = 0x01;
			frm.numdrv = htons(ushort(m_v.numdrv));
			int index = get_drvindex(m_v.numdrv);
			frm.asclen = 0;
			if(-1 != index) 
			{
				frm.asclen = min(7, strlen(m_cfg.inf[index].ascii) );
				memcpy(frm.ascii, m_cfg.inf[index].ascii, frm.asclen);
			}

			memcpy(szbuf+1, (char*)&frm, sizeof(frm));
			DataPush(szbuf, sizeof(frm)+1, DEV_QIAN, DEV_DIAODU, LV2);
		}
		break;

	case 0x67: // �������ش���������
		break;

	case 0x69: // ���󴫸���״̬
		break;

	case 0x6B: // �����ı���Ϣ��ʾӦ��
		break;

	case 0x6C: // ˾���ǳ�ָʾӦ��
		break;

	case 0x6D: // ��������ϵ��ָ��
		{
			int i = 0;
			byte frm[10] = {0};
			static ulong pulse = 0;
			frm[i++] = 0x6E;		//��������
			frm[i++] = *(data+1);	//ָ���

			switch(*(data+1))
			{
			case 0x01:	//��ѯ
				{
					PRTMSG(MSG_NOR, "query factor = %u\n", m_cfg.factor);
					
					ushort factor = htons(m_cfg.factor);
					frm[i++] = 0x01;	//Ӧ������
					memcpy(frm+i, &factor, 2);	i += 2;
				}
				break;

			case 0x02:	//����
				{	
					ushort factor = 0;
					memcpy(&factor, data+2, 2);

					PRTMSG(MSG_NOR, "set factor = %u", htons(factor));
				
					m_cfg.factor = htons(factor);
					uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
					int ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );
				
					frm[i++] = 0x01;	//Ӧ������
				}
				break;
				
			case 0x03:	//�������
				{
					g_objQianIO.IN_QueryMeterCycle(pulse);
					PRTMSG(MSG_NOR, "Begin chk pulse(%u)..\n", pulse);
					frm[i++] = 0x01;	//Ӧ������
				}
				break;

			case 0x04:	//ֹͣ���
				{
					ulong pulse2 = 0;
					g_objQianIO.IN_QueryMeterCycle(pulse2);

					PRTMSG(MSG_NOR, "End chk pulse(%u)!", pulse);

					pulse = pulse2 - pulse;
					frm[i++] = 0x01;	//Ӧ������
				}
				break;

			case 0x05:	//�û��������
				{
					byte mile = *(data+2);	//�û����������(����)
				
					//if(mile>0 && mile<=255)	// byte�Ͳ�������жϰ�
					{
						m_cfg.factor = ushort(pulse / mile);
						uint offset = offsetof(tagSecondCfg, m_uniDrvCfg.m_szReservered);
						int ret = SetSecCfg(&m_cfg, offset, sizeof(m_cfg) );
						
						frm[i++] = 0x01;	//Ӧ������
					} 
// 					else 
// 					{
// 						frm[i++] = 0x02;	//Ӧ������
// 					}

					PRTMSG(MSG_NOR, "Result is %u=%u/%u\n", m_cfg.factor, pulse, mile);
				}
				break;
				
			default:
				break;
			}

			memcpy(szbuf+1, (char*)frm, i);
			DataPush(szbuf, i+1, DEV_QIAN, DEV_DIAODU, LV2);
		}
		break;

	case 0x6F: // ��������ָ��
		{
			int i = 0;
			byte frm[10] = {0};
			frm[i++] = 0x6F;		//��������
			frm[i++] = *(data+1);	//ָ���

			switch(*(data+1))
			{
			case 0x01:	//��ѯ
				{
					PRTMSG(MSG_NOR, "query total = %u\n", ulong(m_v.metertotal));
					
					ulong total = htonl(ulong(m_v.metertotal));
					frm[i++] = 0x01;	//Ӧ������
					memcpy(frm+i, &total, 4);	i += 4;
				}
				break;

			case 0x02:	//��0
				{
					m_v.metertotal = 0;
					flush_headrcd();
					frm[i++] = 0x01;	//Ӧ������
				}
				break;

			case 0x03:	//����
				{
					ulong total = 0;
					memcpy(&total, data+2, 4);
					PRTMSG(MSG_NOR, "set total = %u\n", htonl(total));
					
					m_v.metertotal = htonl(total);
					flush_headrcd();
					frm[i++] = 0x01;	//Ӧ������
				}
				break;
				
			default:
				break;
			}

			memcpy(szbuf+1, (char*)frm, i);
			DataPush(szbuf, i+1, DEV_QIAN, DEV_DIAODU, LV2);
		}
		break;

	default:
		PRTMSG(MSG_NOR, "Rcv unknown diaodu(%02X)!\n", *data);
		break;
	}
}

//------------------------------------------------------------------------------
// �Ӽ�¼�ļ��ж������һ��δ�ϴ����¹��ɵ�
// updoub : �¹��ɵ�����,1��ʽ�¹��ɵ����� 2ȫ���ɵ�����
// rd: ������¹��ɵ�ṹָ��
// ����: �ü�¼���ļ��е�λ��(0Ϊû���ҵ���¼)
ulong CDriveRecord::get_lstdoub(byte updoub, RcdDoub &rd)
{
	RcdDoub rcd;
	ulong cnt = m_v.doubcnt;
	ulong pw = m_v.doubpw;
	ulong find = 0;
	
	FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");
	if(NULL!=fp)
	{
		for(ulong i=0; i<cnt; i++) 
		{
			if(--pw >100)		pw = 99;

			fseek(fp, offsetof(RcdFile,doub)+pw*sizeof(RcdDoub), 0);
			fread(&rcd, sizeof(rcd), 1, fp);

			if( 0 == (rcd.stoptype & 0x80) ) //�ü�¼��δ�ϱ���
			{	
				if( (1==updoub && 0x02==rcd.stoptype) || 2==updoub )	//��ʽ�ɵ� || ȫ���ɵ�
					find = pw + 1;
			}

			if(find)
			{ 
				rd = rcd;
				break;
			}
		}

		fclose(fp);
	}
	return find;
}


//------------------------------------------------------------------------------
// ����¼�ļ����¹��ɵ���ϱ���������Ϊ���ϱ�
// pw: ��¼��ָ��
void CDriveRecord::set_doubtype(ulong pw)
{
	RcdDoub rcd;
	FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");

	if(NULL!=fp) 
	{
		fseek(fp, offsetof(RcdFile,doub)+pw*sizeof(RcdDoub), 0);
		fread(&rcd, sizeof(rcd), 1, fp);
		rcd.stoptype = rcd.stoptype | 0x80;
		
		fseek(fp, offsetof(RcdFile,doub)+pw*sizeof(RcdDoub), 0);
		fwrite(&rcd, sizeof(rcd), 1, fp);
		fclose(fp);
	}
}


//------------------------------------------------------------------------------
// ���3747��3749Э��֡
#if USE_OIL == 3
void CDriveRecord::fill_frm3749(frm3749 &frm, int c_snd, ushort cltcyc, ushort sndcyc, tagGPSData gps)
{
	if(0==c_snd) {	//�ɼ���һ��GPS��ʱ
		frm.reply = 0;
		frm.w_size = m_cfg.winsize;
		frm.frmno = 0;
		frm.tm_spc = htons(cltcyc);
		frm.ct_gps = byte(sndcyc);

		struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);

		frm.date[0]	= (byte)(pCurrentTime.tm_year-100);
		frm.date[1]	= (byte)(pCurrentTime.tm_mon+1);
		frm.date[2]	= (byte)pCurrentTime.tm_mday;
		frm.time[0]	= (byte)pCurrentTime.tm_hour;
		frm.time[1]	= (byte)pCurrentTime.tm_min;
		frm.time[2] = (byte)pCurrentTime.tm_sec;


		BYTE bytKuozhan = 0;
		if(gps.valid == 'A') bytKuozhan |= 0x01;
		if(g_objOil.bConnect()) bytKuozhan |= 0x10;  //���ͺ����� 
		bytKuozhan |= 0x06;  //���������ݺ���ʻ��¼������
		
		bytKuozhan = 0x1f;	// ��GPS���ݡ���ʻ��¼���ݡ�����״̬���ݡ��������ݡ��ͺ�����
		frm.m_bytKuoZhanSta1 = bytKuozhan;
		frm.m_bytKuoZhanSta2 = 0;
	}
	frm.inf[c_snd].numdrv = htons(ushort(m_v.numdrv));

	double db = 60*(gps.latitude-byte(gps.latitude));
	frm.inf[c_snd].lat[0] = byte(gps.latitude);
	frm.inf[c_snd].lat[1] = (byte)db;
	frm.inf[c_snd].lat[2] = byte(long(db*100)%100);
	frm.inf[c_snd].lat[3] = byte(long(db*10000)%100);
	db = 60*(gps.longitude-byte(gps.longitude));
	frm.inf[c_snd].lon[0] = byte(gps.longitude);
	frm.inf[c_snd].lon[1] = (byte)db;
	frm.inf[c_snd].lon[2] = byte(long(db*100)%100);
	frm.inf[c_snd].lon[3] = byte(long(db*10000)%100);

	frm.inf[c_snd].speed = m_v.vector[0];
	frm.inf[c_snd].dir = byte(gps.direction/3) + 1;
	
	{
		DWORD dwIoSta = 0;
		// ��ȡ��������Ϣ
		g_objQianIO.IN_QueryIOSta( dwIoSta );
		// Э�鶨��: D7��D0������0��ʾ���źţ�1��ʾ���ź�
		//	D7--��ɲ�ź�
		//  D6--��ɲ�ź�
		//  D5--��ת����ź�
		//  D4--��ת����ź�
		//  D3--ǰ�����ź�
		//  D2--�����ź�
		//  D1--���������ź�
		//  D0--ACC�ź�
		frm.inf[c_snd].sensor = BYTE(dwIoSta % 0x100);

		DWORD dwVeSta = g_objCarSta.GetVeSta();
		if(dwVeSta & VESTA_HEAVY)//�س�
		{
			frm.inf[c_snd].sensor |= 0x02;
		}
		else//�ճ�
		{
			frm.inf[c_snd].sensor &= 0xFD;
		}
		// ��ȡ״̬��Ϣ
		
		// Э�鶨��:
		// ״̬�֣�H7 H6 H5 H4 H3 H2 H1 H0
		//		   L7 L6 L5 L4 L3 L2 L1 L0
		// H0��GPRS�������  0������        1����
		// H1��������ײ       0 ����ײ       1 ��ײ
		// H2�������෭       0�޲෭       1 �෭
		// H3��Ƿѹ״̬       0 ��Ƿѹ       1Ƿѹ
		// H4������ϵ�״̬   0 ����δ�ϵ�   1����ϵ�
		// H5�����ٱ���״̬   0�����ٱ���   1�������ٱ���
		// H6���Ƿ�����״̬   0 �޷Ƿ�����   1 �����Ƿ�����
		// H7������
		// 
		// L1��L0��γ������:
		// 0  ������γ  1  ������γ  2  ������γ  3  ������γ
		// L3��L2 GPSģ�鶨λ״̬��
		// 0��δ��λ    1��2D��λ    2��3D��λ   3 ����
		// L5��L4 GPSģ�鹤��״̬��
		// 0������      1��ʡ��       2��ͨѶ�쳣  3 ����
		// L6 ǰ����״̬
    // 0:�� 1����
    // L7 ����״̬
    // 0:�� 1����
    
		// ����λ��������������Ϊ0��
		DWORD dwAlertSta = g_objMonAlert.GetAlertSta();
		DWORD dwDevSta = g_objQianIO.GetDeviceSta();
		BYTE bytH = 0, bytL = 0;
		
		if( g_objSock.IsOnline() ) bytH |= 0x01; // �����������
		if( dwAlertSta & ALERT_TYPE_BUMP ) bytH |= 0x02;
		if( dwAlertSta & ALERT_TYPE_OVERTURN ) bytH |= 0x04;
		if( dwAlertSta & ALERT_TYPE_POWBROWNOUT ) bytH |= 0x08;
		if( dwAlertSta & ALERT_TYPE_POWOFF ) bytH |= 0x10;
		if( dwAlertSta & ALERT_TYPE_FOOT ) bytH |= 0x20;
		if( dwAlertSta & ALERT_TYPE_DRIVE ) bytH |= 0x40;

		bytL |= 0; // ��ʱ��ǿ��ʹ��"������γ"
		if( '3' == gps.m_cFixType ) bytL |= 0x08; // ��λ����
		else if( '2' == gps.m_cFixType ) bytL |= 0x04;
		else bytL |= 0;
		if( dwDevSta & DEVSTA_GPSON ) bytL |= 0; // GPSģ��״̬
		else bytL |= 0x10;
		if( dwAlertSta & ALERT_TYPE_FRONTDOOR ) bytL |= 0x40; // ǰ����״̬
		if( dwAlertSta & ALERT_TYPE_BACKDOOR ) bytL |= 0x80; // ����״̬
		
		frm.inf[c_snd].state[0] = bytH;
		frm.inf[c_snd].state[1] = bytL;

		BYTE byt = 0;
		if( dwAlertSta & ALERT_TYPE_BUMP ) byt |= 0x01;
		if( dwAlertSta & ALERT_TYPE_OVERTURN ) byt |= 0x02;
		if( dwAlertSta & ALERT_TYPE_POWBROWNOUT ) byt |= 0x04;
		if( dwAlertSta & ALERT_TYPE_POWOFF ) byt |= 0x08;
		if( dwAlertSta & ALERT_TYPE_FOOT ) byt |= 0x10;
		if( dwAlertSta & ALERT_TYPE_BELOWSPEED ) byt |= 0x20;
		
		frm.inf[c_snd].sensor2 = byt & 0x3f;
	}

	frm.inf[c_snd].ct_load = 4;
	frm.inf[c_snd].ad_load[0] = 0;
	frm.inf[c_snd].ad_load[1] = 0;
	frm.inf[c_snd].ad_load[2] = 0;
	frm.inf[c_snd].ad_load[3] = 0;

	{
		byte bytRptType = 0;
		byte bytOilSta1 = 0;
		const double OIL_MVESPD1 = 1.852 * 2 / 3.6; // ��/��
		bool bGpsValid1 = gps.valid == 'A' ? true : false;
		bool bAccValid1 = g_objQianIO.GetDeviceSta() & DEVSTA_ACCVALID ? true : false;
	
		byte bytOilSta = 0; 
		if( !bAccValid1 ) bytOilSta  |= 0x10;
		else if( !bGpsValid1 ) bytOilSta |= 0x04;
		else
		{
			if( gps.speed >= OIL_MVESPD1 ) bytOilSta |= 0x02;
			else bytOilSta |= 0x08;
		}  

		if(!g_objOil.bConnect()) bytOilSta |= 0x20; 	//�ͺĺ��Ƿ�����
	
		bool bytGetOilSucc = false;
		ushort ValidOilAD = 0,ValidPowtAD = 0;
		ushort RealOilAD = 0,RealPowAD = 0;

// 		if(bOilStatus)
		{
			bytGetOilSucc = g_objOil.GetOilAndPowAD(bytRptType, bytOilSta1, ValidOilAD, ValidPowtAD);
			
			if(bytGetOilSucc)
			{
				if(bytOilSta1&0x01) bytOilSta |= 0x01;		// ����־
				if(bytOilSta1&0x08) bytOilSta |= 0x80;		// ��©�ͱ�־
				if(bytOilSta1&0x10) bytOilSta |= 0x40;		// ������Χ��־
			}
		}

		frm.inf[c_snd].m_bytOilSta = bytOilSta;
		frm.inf[c_snd].m_wValidOilAD = htons(ValidOilAD);
		frm.inf[c_snd].m_wValidPowtAD = htons(ValidPowtAD);		
	}
	
	// �ﵽ�������������һ֡ʱ���������
	if( c_snd >= (sndcyc-1) ) { 
		frm.meter = htonl(ulong(m_v.metertotal));
	}
}
#else
void CDriveRecord::fill_frm3747(frm3747 &frm, int c_snd, ushort cltcyc, ushort sndcyc, tagGPSData gps)
{
	if(0==c_snd)	//�ɼ���һ��GPS��ʱ
	{
		frm.reply = 0;
		frm.w_size = m_cfg.winsize;
		frm.frmno = 0;
		frm.ad_oil = 0;
		frm.vo_oil = 0;
		frm.st_oil = 0;
		frm.tm_spc = htons(cltcyc);
		frm.ct_gps = byte(sndcyc);

		struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);
		
		frm.date[0]	= (byte)(pCurrentTime.tm_year-100);
		frm.date[1]	= (byte)(pCurrentTime.tm_mon+1);
		frm.date[2]	= (byte)pCurrentTime.tm_mday;
		frm.time[0]	= (byte)pCurrentTime.tm_hour;
		frm.time[1]	= (byte)pCurrentTime.tm_min;
		frm.time[2] = (byte)pCurrentTime.tm_sec;
	}

	frm.inf[c_snd].numdrv = htons(ushort(m_v.numdrv));
	double db = 60*(gps.latitude-byte(gps.latitude));
	frm.inf[c_snd].lat[0] = byte(gps.latitude);
	frm.inf[c_snd].lat[1] = (byte)db;
	frm.inf[c_snd].lat[2] = byte(long(db*100)%100);
	frm.inf[c_snd].lat[3] = byte(long(db*10000)%100);
	db = 60*(gps.longitude-byte(gps.longitude));
	frm.inf[c_snd].lon[0] = byte(gps.longitude);
	frm.inf[c_snd].lon[1] = (byte)db;
	frm.inf[c_snd].lon[2] = byte(long(db*100)%100);
	frm.inf[c_snd].lon[3] = byte(long(db*10000)%100);
	frm.inf[c_snd].speed = m_v.vector[0];
	frm.inf[c_snd].dir = byte(gps.direction/3) + 1;
	
	{
		DWORD dwIoSta = 0;
		// ��ȡ��������Ϣ
		g_objQianIO.IN_QueryIOSta( dwIoSta );
		// Э�鶨��: D7��D0������0��ʾ���źţ�1��ʾ���ź�
		//	D7--��ɲ�ź�
		//  D6--��ɲ�ź�
		//  D5--��ת����ź�
		//  D4--��ת����ź�
		//  D3--ǰ�����ź�
		//  D2--�����ź�
		//  D1--���������ź�
		//  D0--ACC�ź�
		frm.inf[c_snd].sensor = BYTE(dwIoSta % 0x100);
		
		DWORD dwVeSta = g_objCarSta.GetVeSta();
		if(dwVeSta & VESTA_HEAVY)//�س�
		{
			frm.inf[c_snd].sensor |= 0x02;
		}
		else		//�ճ�
		{
			frm.inf[c_snd].sensor &= 0xFD;
		}
		// ��ȡ״̬��Ϣ
		
		// Э�鶨��:
		// ״̬�֣�H7 H6 H5 H4 H3 H2 H1 H0
		//		     L7 L6 L5 L4 L3 L2 L1 L0
		// H0��GPRS�������  0������        1����
		// H1��������ײ       0 ����ײ       1 ��ײ
		// H2�������෭       0�޲෭       1 �෭
		// H3��Ƿѹ״̬       0 ��Ƿѹ       1Ƿѹ
		// H4������ϵ�״̬   0 ����δ�ϵ�   1����ϵ�
		// H5�����ٱ���״̬   0�����ٱ���   1�������ٱ���
		// H6���Ƿ�����״̬   0 �޷Ƿ�����   1 �����Ƿ�����
		// H7������
		// 
		// L1��L0��γ������:
		// 0  ������γ  1  ������γ  2  ������γ  3  ������γ
		// L3��L2 GPSģ�鶨λ״̬��
		// 0��δ��λ    1��2D��λ    2��3D��λ   3 ����
		// L5��L4 GPSģ�鹤��״̬��
		// 0������      1��ʡ��       2��ͨѶ�쳣  3 ����
		// L6 ǰ����״̬
    // 0:�� 1����
    // L7 ����״̬
    // 0:�� 1����

		// ����λ��������������Ϊ0��
		DWORD dwAlertSta = g_objMonAlert.GetAlertSta();
		DWORD dwDevSta = g_objQianIO.GetDeviceSta();
		BYTE bytH = 0, bytL = 0;
		
		if( g_objSock.IsOnline() ) bytH |= 0x01; // �����������
		if( dwAlertSta & ALERT_TYPE_BUMP ) bytH |= 0x02;
		if( dwAlertSta & ALERT_TYPE_OVERTURN ) bytH |= 0x04;
		if( dwAlertSta & ALERT_TYPE_POWBROWNOUT ) bytH |= 0x08;
		if( dwAlertSta & ALERT_TYPE_POWOFF ) bytH |= 0x10;
		if( dwAlertSta & ALERT_TYPE_FOOT ) bytH |= 0x20;
		if( dwAlertSta & ALERT_TYPE_DRIVE ) bytH |= 0x40;

		bytL |= 0; // ��ʱ��ǿ��ʹ��"������γ"
		if( '3' == gps.m_cFixType ) bytL |= 0x08; // ��λ����
		else if( '2' == gps.m_cFixType ) bytL |= 0x04;
		else bytL |= 0;
		if( dwDevSta & DEVSTA_GPSON ) bytL |= 0; // GPSģ��״̬
		else bytL |= 0x10;
		if( dwAlertSta & ALERT_TYPE_FRONTDOOR ) bytL |= 0x40; // ǰ����״̬
		if( dwAlertSta & ALERT_TYPE_BACKDOOR ) bytL |= 0x80; // ����״̬
		
		frm.inf[c_snd].state[0] = bytH;
		frm.inf[c_snd].state[1] = bytL;

		BYTE byt = 0;
		if( dwAlertSta & ALERT_TYPE_BUMP ) byt |= 0x01;
		if( dwAlertSta & ALERT_TYPE_OVERTURN ) byt |= 0x02;
		if( dwAlertSta & ALERT_TYPE_POWBROWNOUT ) byt |= 0x04;
		if( dwAlertSta & ALERT_TYPE_POWOFF ) byt |= 0x08;
		if( dwAlertSta & ALERT_TYPE_FOOT ) byt |= 0x10;
		if( dwAlertSta & ALERT_TYPE_BELOWSPEED ) byt |= 0x20;
		
		frm.inf[c_snd].sensor2 = byt & 0x3f;
	}

	frm.inf[c_snd].ct_load = 4;
	frm.inf[c_snd].ad_load[0] = 0;
	frm.inf[c_snd].ad_load[1] = 0;
	frm.inf[c_snd].ad_load[2] = 0;
	frm.inf[c_snd].ad_load[3] = 0;
	
	// �ﵽ�������������һ֡ʱ���������
	if( c_snd >= (sndcyc-1) )
	{ 
		frm.meter = htonl(ulong(m_v.metertotal));
	}
}
#endif

//------------------------------------------------------------------------------
// ���������ϱ���ʻ����(���ͺ�)
void CDriveRecord::deal_updrv(tagGPSData gps)
{
	static LinkLst link;		//��ʱ��Ųɼ����ݵ�������
	
	// ---------- ����ɼ���ʻ���ݵ�״̬�� --------------
#if USE_OIL == 3
	static frm3749 frm = {0};
#else
	static frm3747 frm = {0};
#endif

	static int c_snd = 0;		//��ǰ֡�ɼ����ڼ���GPS��
	static byte sta_idlst = 0;	//��һ�ε�״̬��ʶ

	// ���ݳ���״̬ȷ��״̬��ʶ
	sta_idlst = m_cfg.sta_id;
	//car_move��Ϊacc_valid������׼ȷ����ֹ���Ե�ʱ��ACC�����ϱ�����û�и��£��Ӷ���Ϊ��̨������.
	if(0x01==m_cfg.sta_id || 0x02==m_cfg.sta_id)	m_cfg.sta_id = acc_valid() ? 0x01 : 0x02;
#if 0
	if(0x01==m_cfg.sta_id || 0x02==m_cfg.sta_id)	m_cfg.sta_id = car_move() ? 0x01 : 0x02;
#endif
	if(0x03==m_cfg.sta_id || 0x04==m_cfg.sta_id)	m_cfg.sta_id = !car_load() ? 0x03 : 0x04;
	if(0x05==m_cfg.sta_id || 0x06==m_cfg.sta_id)	m_cfg.sta_id = acc_valid() ? 0x05 : 0x06;

	// ���״̬��ʶ�з����ı�(��Ϊ��),�����ؿ���
	// (ÿ�����������ϱ�����Ҳ������״̬��ʶ�����ı�,��Ϊ������
	// ����hdl_103e,ʹm_cfg.sta_id��ֵ�����仯)
	if(sta_idlst!=m_cfg.sta_id)		
		m_v.st_clt = 0;
	if(0==m_cfg.sta_id)				
		m_v.st_clt = 0;

	switch(m_v.st_clt) 
	{
		case 0:	// ����״̬
		{
			if(m_cfg.sta_id)
			{			
				c_snd = 0;
				m_v.uptime = 0;
				memset(&frm, 0, sizeof(frm));
				m_v.st_clt = 1;
			}
		}
			break;
		
		case 1:	// �ɼ�״̬
		{
			ushort cltcyc = m_cfg.cltcyc[m_cfg.sta_id-1];
			ushort sndcyc = m_cfg.sndcyc[m_cfg.sta_id-1];
			ushort uptime = m_cfg.uptime[m_cfg.sta_id-1];

			if (sndcyc>4)
			{
				PRTMSG(MSG_NOR, "deal_updrv sndcyc>4 err.\n");
				break;
			}
			
			if(0==cltcyc || 0==sndcyc) 
			{ 
				m_v.st_clt = 0; 
				break; 
			}

			if( 0==(m_v.ticks%cltcyc) || m_bNeedClt ) 
			{
#if USE_OIL == 3
				fill_frm3749(frm, c_snd, cltcyc, sndcyc, gps);
#else
				fill_frm3747(frm, c_snd, cltcyc, sndcyc, gps);
#endif
				if( ++c_snd >= sndcyc || m_bNeedClt ) 
				{
					if(m_bNeedClt)
					{
						m_bNeedClt = false;
					}
					c_snd = 0;
					//if(false==link.append((byte*)&frm, sizeof(frm)))//zzg mod ��ֹ�б������µ��������Ӳ���ȥ
					if(link.getcount()>=1024)//zzg mod ��ֹ�б������µ��������Ӳ���ȥ��1024��������������
					{
						link.gohead();
						int i=0;
						for( i=0; i<m_cfg.winsize; i++)	link.next();
						for( i=0; i<m_cfg.winsize; i++)	link.remove();//����ڶ������ڣ�����Ĵ��ڻ��Զ�Ų����			
					}
					link.append((byte*)&frm, sizeof(frm));
					memset(&frm, 0, sizeof(frm));
				}

				if( 0xffff!=uptime && (m_v.uptime/60)>=uptime )
				{
					PRTMSG(MSG_NOR, "Uptime(%u) is over!\n", uptime);
					m_v.st_clt = 2;
					break;
				}
			}

			if(0xffff!=uptime)
				m_v.uptime++;
		}
			break;

		case 2:	// �ɼ�����״̬(�ɼ���ʱ����)
			break;

		default:
			m_v.st_clt = 0;
			break;
	}

	// ---------- �����ϱ���ʻ���ݵ�״̬�� --------------
	static enum 
	{ 
		ChkLink, 
		Sending, 
		WaitReply, 
		ReSending,
	} st_snd = ChkLink;			//�ɼ��Լ��ϱ���ʻ���ݵ�״̬��
	static bool winid = false;	//���ڱ�ʶ(ʶ��ͬ�Ĵ���)
	static int overtime = 0;	//���ͺ�ȴ�����Ӧ��ʱ����	

#if SND_TYPE == 1// ��֡���ݷ��ͣ���һ������Ҫ��Ӧ��
	// ����״̬��
	static unsigned long ucSendCnt = 0;
	static unsigned long ulLastLinkCnt = 0;

	switch(st_snd) 
	{
		case ChkLink:	// ���������Ƿ���������Ҫ����
		{
			unsigned long tempCnt = link.getcount();

			if( ( (tempCnt >= 1) && (tempCnt != ulLastLinkCnt)  ) || tempCnt > m_cfg.winsize  )
			{
				st_snd = Sending; 

				if (0==(ucSendCnt%m_cfg.winsize) )
				{
					winid = !winid;
				}
			}
			ulLastLinkCnt = tempCnt;
		}
			break;

		case Sending:	// ����״̬
		{
			ulong tmp = 0;

#if USE_OIL == 3
			frm3749 frm = {0};
#else
			frm3747 frm = {0};
#endif
			
			link.gohead();

			int iLoop = (ucSendCnt%m_cfg.winsize) ;
			for(int kk=0 ; kk< iLoop ; kk++)
				link.next();

			link.getdata((byte*)&frm);

			PRTMSG(MSG_NOR, "snd frm3747 Tm=%02d:%02d:%02d.iLoop=%d.\n",frm.time[0],frm.time[1],frm.time[2],iLoop);
			
			frm.reply = ( (ucSendCnt+1) % m_cfg.winsize ) ? 0x42 : 0x43;
			frm.reply = winid ? (frm.reply & ~0x08) : (frm.reply | 0x08);	
			frm.frmno = byte(ucSendCnt%m_cfg.winsize );
			
			int ln = frm.mkfrm();

#if USE_OIL == 3
			tcp_send(0x37, 0x49, &frm, ln);
#else
			tcp_send(0x37, 0x47, &frm, ln);
#endif
			
			if ( 0==( (ucSendCnt+1) % m_cfg.winsize ) )
			{
				overtime = 0;
				memset(m_v.rp_oil, 0xff, 5);
				st_snd = WaitReply;
			}
			else
			{
				st_snd = ChkLink; 
			}
			ucSendCnt++;
		}
			break;

		case ReSending:	// �ط�״̬
		{
			ulong tmp = 0;

#if USE_OIL == 3
			frm3749 frm = {0};
#else
			frm3747 frm = {0};
#endif
			memcpy(&tmp, &m_v.rp_oil[1], 4);

			tmp = htonl(tmp);	//�ֽ�����Ҫת��
			tmp &= (~(0xffffffff<<m_cfg.winsize));		//������λ��0

			link.gohead();

			for(int i=0; i<m_cfg.winsize; i++) 
			{
				if(tmp & 0x00000001) 
				{
					link.getdata((byte*)&frm);
					frm.reply = (tmp>>1) ? 0x42 : 0x43;		//(tmp>>1)Ϊ0,���ʾ�����һ֡
					frm.reply = winid ? (frm.reply & ~0x08) : (frm.reply | 0x08);
					frm.frmno = byte(i);
				
					int ln = frm.mkfrm();
#if USE_OIL == 3
					tcp_send(0x37, 0x49, &frm, ln);
#else
					tcp_send(0x37, 0x47, &frm, ln);
#endif
				}
				tmp >>= 1;
				link.next();
			}

			overtime = 0;
			memset(m_v.rp_oil, 0xff, 5);
			st_snd = WaitReply;
		
			PRTMSG(MSG_NOR, "ReSend(%u/%u) and wait rp_oil...\n", m_cfg.winsize, link.getcount());
		}
			break;

		case WaitReply:		// �ȴ�����Ӧ��״̬
		{
			if(0x00 == m_v.rp_oil[0])	//�ô�������δ��ȫ
			{		
				st_snd = ReSending; 
				PRTMSG(MSG_NOR, "Rcv rp_oil(part)!\n");
				break; 
			}
			if(0x01 == m_v.rp_oil[0])	//�ô������Ľ������
			{		
				link.gohead();
				for(int i=0; i<m_cfg.winsize; i++)	link.remove(); 
				ucSendCnt = ucSendCnt - m_cfg.winsize;	
				if (ucSendCnt<0) ucSendCnt=0;
				PRTMSG(MSG_NOR, "Rcv rp_oil(all)!\n");	
				st_snd = ChkLink; 		
				break; 
			}
			if(++overtime > 3*60)		//������Ӧ��,��ʱ!
			{					
				if( g_objSock.IsOnline() ) //����������û��Ҫ�����ط�������ռ�ù����TCP���пռ�// zzg mod
				{
					PRTMSG(MSG_NOR, "Wait rp_oil overtime!\n");//������Ӧ��,��ʱ!
					st_snd = ChkLink; 
				}
				else
				{
					overtime=0;
				}
				break; 
			}
		}
			break;

		default:
			st_snd = ChkLink;
			break;
	}	
#endif


#if SND_TYPE == 2// ��һ�������ٷ���
	// ����״̬��
	switch(st_snd) 
	{
		case ChkLink:	// ���������Ƿ���������Ҫ����
		{
			if(link.getcount() >= m_cfg.winsize) 
			{ 
				winid = !winid; 
				memset(m_v.rp_oil, 0xff, 5);
				st_snd = Sending; 
				break; 
			}
		}
			break;

		case Sending:	// ����״̬
		{
			ulong tmp = 0;
			frm3747 frm = {0};
			memcpy(&tmp, &m_v.rp_oil[1], 4);

			tmp = htonl(tmp);	//�ֽ�����Ҫת��
			tmp &= (~(0xffffffff<<m_cfg.winsize));		//������λ��0

			link.gohead();//һֱ������������Ǹ����ڵ�����

			for(int i=0; i<m_cfg.winsize; i++)
			{
				if(tmp & 0x00000001) 
				{
					link.getdata((byte*)&frm);
					
					frm.reply = (tmp>>1) ? 0x42 : 0x43;		//(tmp>>1)Ϊ0,���ʾ�����һ֡
					frm.reply = winid ? (frm.reply & ~0x08) : (frm.reply | 0x08);
					frm.frmno = byte(i);
				
					int ln = frm.mkfrm();
					tcp_send(0x37, 0x47, &frm, ln);
				}

				tmp >>= 1;
				link.next();
			}
			overtime = 0;
			memset(m_v.rp_oil, 0xff, 5);
			st_snd = WaitReply;
		
			PRTMSG(MSG_NOR, "Send(%u/%u) and wait rp_oil...\n", m_cfg.winsize, link.getcount());
		}
			break;

		case WaitReply:		// �ȴ�����Ӧ��״̬
		{
			if(0x00 == m_v.rp_oil[0])	//�ô�������δ��ȫ
			{		
				st_snd = Sending; 
				PRTMSG(MSG_NOR, "Rcv rp_oil(part)!\n");
				break; 
			}
			if(0x01 == m_v.rp_oil[0])	//�ô������Ľ������
			{		
				link.gohead();
				for(int i=0; i<m_cfg.winsize; i++)	link.remove(); 
				PRTMSG(MSG_NOR, "Rcv rp_oil(all)!\n");
				st_snd = ChkLink; 
				break; 
			}
			if(++overtime > 3*60) 		//������Ӧ��,��ʱ!
			{		
				PRTMSG(MSG_NOR, "Wait rp_oil overtime!\n");
				st_snd = ChkLink; 
				break; 
			}
		}
			break;

		default:
			st_snd = ChkLink;
			break;
	}
#endif
}


//------------------------------------------------------------------------------
// ���ڴ����ѯ���ݵĶ�ʱ�ϴ�����
// (ÿ�뷢��һ�����ݰ�,����ͬʱ���ʹ��������ݱ���������)
void CDriveRecord::deal_query()
{
	// �ϴ��¹��ɵ�����
	if( 0 == m_v.ticks%4 ) 
	{
		static byte index = 0;

		if(0 != m_quedoub.getcount()) 
		{
			RcdDoub rcd = {0};
			m_quedoub.pop((byte*)&rcd);
			send_3644(&rcd, index, m_quedoub.getcount() ? 0x01 : 0x02 );
			index++;
		} 
		else 
		{ 
			index = 0; 
		}
	}

	// �ϴ���ʻ��¼����
	if( 0 == m_v.ticks%1 ) 
	{
		static byte index = 0;
		if(0!=m_quedrv.getcount()) 
		{
			RcdDrv rcd = {0};
			m_quedrv.pop((byte*)&rcd);
			frm3646 frm = {0};

			frm.reply = m_quedrv.getcount() ? 0x01 : 0x02 ;
			frm.index = index++;
			frm.count = 1;
			frm.rcd = rcd;
			frm.rcd.space = htons(frm.rcd.space);

			tcp_send(0x36, 0x46, &frm, sizeof(frm) );
		} 
		else 
		{ 
			index = 0;
		}
	}

	// �ϴ���̼�¼����
	if( 0 == m_v.ticks%1 )
	{
		static byte index = 0;

		if(0!=m_quemeter.getcount())
		{
			RcdDrv rcd = {0};
			m_quemeter.pop((byte*)&rcd);
			frm0945 frm = {0};

			frm.reply = m_quemeter.getcount() ? 0x01 : 0x02 ;
			frm.index = index;
			frm.count = 1;
			frm.numdrv = rcd.numdrv;
			frm.bit_g0 = 0x04;
			frm.bit_g1 = rcd.s_lon[0] & 0x80;
			frm.bit_g2 = rcd.s_lon[0] & 0x40;
			rcd.s_lon[0] &= 0x3f;
			frm.bit_g3 = rcd.e_lon[0] & 0x80;
			frm.bit_g4 = rcd.e_lon[0] & 0x40;
			rcd.e_lon[0] &= 0x3f;
			memcpy(frm.s_time, rcd.s_time, 5);
			memcpy(frm.s_lon, rcd.s_lon, 4);
			memcpy(frm.e_time, rcd.e_time, 5);
			memcpy(frm.s_lat, rcd.s_lat, 4);
			frm.space[0] = byte(rcd.space/100);
			frm.space[1] = byte(rcd.space%100);

			tcp_send(0x09, 0x45, &frm, sizeof(frm) );
		}
		else
		{ 
			index = 0; 
		}
	}
}


//------------------------------------------------------------------------------
// ���ڴ�����ʱ��κͱ�������
void CDriveRecord::deal_alarm(tagGPSData gps)
{
	// 30��ɨ��һ��
	if(0 != (m_v.ticks%30))		return;

	// ------ ɨ�豨����ʻʱ��� -----------------------------
	// �ж��Ƿ��ڹ涨ʱ�������ʻ(ֻ�������е�ʱ�������ʻ,����Ϊ����)
	if( (CAR_RUNNING==m_v.stacar) || acc_valid() )
	{
		struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);

		byte curtime[2] = { (byte)pCurrentTime.tm_hour, (byte)pCurrentTime.tm_min };
		
		bool outtime = true;
		int invalid = 0;

		for(int i=0; i<20; i++)
		{
			int r0 = memcmp(&m_v.almtime[i][0], &m_v.almtime[i][2], 2);
		
			if(r0>=0)	
			{ 
				invalid++;
				continue; 
			}
			
			int r1 = memcmp(curtime, &m_v.almtime[i][0], 2);
			int r2 = memcmp(curtime, &m_v.almtime[i][2], 2);
			
			if(r1>=0 && r2<=0)
			{ 
				outtime = false; 
				break;
			}
		}

		if(invalid>=20)	
			outtime = false;

		set_alarm(ALERT_TYPE_TIME, outtime);	
	} 
	else
	{
		set_alarm(ALERT_TYPE_TIME, false);
	}

	// ------- ɨ�豨����ʻ���� -------------------------------
	FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");
	if(NULL==fp)
		return;
	
	Range rg;
	int cut = 0;
	bool outrange = false;
	bool overspeed = false;
	byte dot0[8], dot1[8], dot2[8];
	
	double db = 60*(gps.longitude-byte(gps.longitude));
	dot0[0] = byte(gps.longitude);
	dot0[1] = (byte)db;
	dot0[2] = byte(long(db*100)%100);
	dot0[3] = byte(long(db*10000)%100);
	db = 60*(gps.latitude-byte(gps.latitude));
	dot0[4] = byte(gps.latitude);
	dot0[5] = (byte)db;
	dot0[6] = byte(long(db*100)%100);
	dot0[7] = byte(long(db*10000)%100);	
	
	for(int i=0; i<m_v.r_used; i++) 
	{
		fseek(fp, offsetof(RcdFile, range)+i*sizeof(Range), 0);
		fread(&rg, 1, sizeof(rg), fp);
	
		for (int j = rg.n_start;;) 
		{
			if(j >= rg.n_end)
				break;

			fseek(fp, offsetof(RcdFile, dot)+j*8, 0);
			fread(dot1, 1, 8, fp);

			if(dot1[4] & 0x80)
			{ 
				j++;
				continue;
			}
			j++;
			fread(dot2, 1, 8, fp);

			if(dot2[4] & 0x80)	//γ�ȷ��������λΪ1����ʾ����ε����1������
			{
				dot2[4] &= ~0x80; 
				j++; 
			}	

			if(cut_line(dot1, dot2, dot0))
				cut++;
		}

		outrange = false;

		if(cut % 2) {        //�������ཻ�ı���Ϊ������˵����ǰ����������
			if((rg.attr & 0x03) == 0x00)		outrange = true;
		} else {             //�������ཻ�ı���Ϊż����˵����ǰ����������
			if((rg.attr & 0x03) == 0x01)		outrange = true;
		}
		if(outrange)		break;
	}
	fclose(fp);
	
	if(outrange) 
	{
		if(rg.vector>0 && m_v.vector[0]>rg.vector)
			overspeed = true;
		
		if(overspeed) 
		{
			set_alarm(ALERT_TYPE_RSPEED, true);
		} 
		else 
		{
			set_alarm(ALERT_TYPE_RSPEED, false);
			set_alarm(ALERT_TYPE_RANGE, true);
		}
	} 
	else
	{
		set_alarm(ALERT_TYPE_RANGE, false);
		set_alarm(ALERT_TYPE_RSPEED, false);
	}
}

//------------------------------------------------------------------------------
// ���ڴ���Խ�籨����OF Э�����ͣ�
void CDriveRecord::deal_area(tagGPSData gps)
{
	size_t sizMax = sizeof(m_objAreaCfg.m_aryArea) / sizeof(m_objAreaCfg.m_aryArea[0]);
	long lLon = long( gps.longitude * LONLAT_DO_TO_L );
	long lLat = long( gps.latitude * LONLAT_DO_TO_L );
	bool bInArea;
	bool bAlerm = false;

	for( BYTE i = 0; i < m_objAreaCfg.m_bytAreaCount && i < sizMax; i ++ )
	{
		bInArea = JugPtInRect(lLon, lLat, m_objAreaCfg.m_aryArea[i][0], m_objAreaCfg.m_aryArea[i][1],
			m_objAreaCfg.m_aryArea[i][2], m_objAreaCfg.m_aryArea[i][3]);
		
		switch( m_objAreaCfg.m_aryAreaProCode[ i ] & 0x03 )
		{
// 		case 0: // ��������
// 			{
// 				if( !bInArea )
// 				{
// 					bArea = false;
// 					goto AREA_LOOPEND;
// 				}
// 				else
// 				{
// 					bytAreaCode = (m_objAreaCfg.m_aryAreaProCode[ i ] & 0x3c) >> 2;
// 					bArea = true;
// 					goto _DEAL_AREA_END;
// 				}
// 			}
//			break;
		
		case 1: // �����ڻ�
			{
				if( bInArea )
				{
					m_objAreaCfg.m_aryAreaContCt[ i ] ++;
					if( POS_CONTCOUNT == m_objAreaCfg.m_aryAreaContCt[ i ] )
					{
						m_bytAreaCode = (m_objAreaCfg.m_aryAreaProCode[ i ] & 0x3c) >> 2;
						bAlerm = true;
						goto _DEAL_AREA_END;
					}
				}
				else
					m_objAreaCfg.m_aryAreaContCt[ i ]  = 0;
			}
			break;
			
// 		case 2: // ��������
// 			{
// 				if( bInArea )
// 				{
// 					bArea = false;
// 					goto AREA_LOOPEND;
// 				}
// 				else
// 				{
// 					bytAreaCode = (m_objAreaCfg.m_aryAreaProCode[ i ] & 0x3c) >> 2;
// 					bArea = true;
// 					goto _DEAL_AREA_END;
// 				}
// 			}
// 			break;
			
		case 3: // �������
			{
				if( !bInArea )
				{
					m_objAreaCfg.m_aryAreaContCt[ i ] ++;
					if( POS_CONTCOUNT == m_objAreaCfg.m_aryAreaContCt[ i ] )
					{
						m_bytAreaCode = (m_objAreaCfg.m_aryAreaProCode[ i ] & 0x3c) >> 2;
						bAlerm = true;
						goto _DEAL_AREA_END;
					}
				}
				else
					m_objAreaCfg.m_aryAreaContCt[ i ] = 0;
			}
			break;
			
		default:
			;
		}
	}

_DEAL_AREA_END:
	if( bAlerm )
	{
		g_objMonAlert.SetAlertArea(true);

		// ����֮������
		for( BYTE i = 0; i < m_objAreaCfg.m_bytAreaCount && i < sizMax; i ++ )
			m_objAreaCfg.m_aryAreaContCt[ i ] = 0;
	}
}

//------------------------------------------------------------------------------
// ���ڴ������ͳ��ҵ��
void CDriveRecord::deal_meter(tagGPSData gps)
{
	// ÿ����ͳ��һ����ʻ���
	double dis = 0;
	static ulong plslst = 0;
	ulong plscur = 0;

// 	plscur = g_objComBus.GetCycleCount();
// 
// 	if( plscur>plslst && 0!=m_cfg.factor )	//�������������ź�,���������ͳ�����
// 	{		
// 		dis = (plscur-plslst)/m_cfg.factor;
// 		ulong speed = 0;
// 		g_objQianIO.IN_QueryIOSpeed(speed);
// 		gps.speed = float((speed/m_cfg.factor)*1000); 
// 	}
//	else	//������GPS�ź�ͳ��
	{	
		if( 0==m_v.lstlon || 0==m_v.lstlat || !acc_valid() )
			dis = 0;
		else
			dis = G_CalGeoDis2(m_v.lstlon, m_v.lstlat, gps.longitude, gps.latitude);

		// �������У��ϵ��������ֵ��
		if( gps.speed > 22.0 )
		{
			dis *= 1.01;
		}
		else
		{
			dis *= 1.02;
		}
	}
	
	if (dis>100) 
	{
        dis = 0;
	}

	// ��Թ�ɽ�����������GPS��Ч������������ͳ�������µ�����
	// 1) ���״η���GPS��Ч������֮ǰ10���ڵ���Ч�ٶȵ�ƽ��ֵ����ΪGPS��Ч���ڵĳ����ٶ�V��
	// 2) ����Ч�ָ���Чʱ��������Чʱ�䣬����Чʱ��С��10���ӣ������ += V * ��Чʱ��
	// 3) ����Чʱ�䳬��10���ӣ����账��
	static bool bFirstInValid = true;
	static byte bytVector = 0;
	static DWORD dwInValidCount = 0;

	byte bytsta = 0;
	IOGet(IOS_ACC, &bytsta);

	if( IO_ACC_ON == bytsta )
	{
		if( 'V' == gps.valid && true == bFirstInValid )
		{
			bFirstInValid = false;
			
			// ��¼�״���Чʱ��
			dwInValidCount = GetTickCount();
			
			// ��¼ǰ10���ƽ���ٶ�
			int iVector = 0;
			for(int k=0; k<10; k++)		iVector += m_v.vector[k];
			bytVector = (byte)( iVector * 1.852 / 3.6 / 10 );
		}
		if( 'A' == gps.valid && false == bFirstInValid )
		{
			bFirstInValid = true;
			
			// ����Чʱ��С��10����
			DWORD dwCount = GetTickCount() - dwInValidCount;
			if( dwCount < 10 * 60 * 1000 )
			{
				dis = bytVector * dwCount / 1000;
			}
		}
	}
	else
	{
		bFirstInValid = true;
		bytVector = 0;
		dwInValidCount = 0;
	}
	

	plslst = plscur;
	m_v.lstlon = gps.longitude;
	m_v.lstlat = gps.latitude;
	m_v.metercur += dis;
	m_v.metertotal += dis;
	m_v.m360._space += dis;
	m_v.m2d._space += dis;

#if USE_LIAONING_SANQI == 1
	TotalMeter = (ushort)(m_v.metertotal/1000);
	Mile = (ulong)(m_v.metertotal/1000);
#endif
	// ͳ���ٶ�
	for(int i=59; i>=1; i--)	m_v.vector[i] = m_v.vector[i-1];
	m_v.vector[0] = byte( ((gps.speed*3600)/1000)/1.852 );
	m_v.speed = byte( ((gps.speed*3600)/1000) );
	
	//090531
	if ('A'!=gps.valid) 
	{
		m_v.vector[0] = 0;
		m_v.speed	 = 0;
	}
	
	//����Ӽ�¼һ��,��ֹ��̨��λ�������ͳ��������
	if( 0 == (m_v.ticks%300) )
	{
		flush_headrcd();
	}
	// 10�����ۼ�һ��2���������ڵ����
	if( 0 == (m_v.ticks%600) ) 
	{
		struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);

		byte date[3] = { (byte)(pCurrentTime.tm_year-100), (byte)(pCurrentTime.tm_mon+1), (byte)pCurrentTime.tm_mday };
		
		if( memcmp(date, &m_v.m2d.date[2][0], 3) >0 )
		{
			memcpy(&m_v.m2d.date[0][0], &m_v.m2d.date[1][0], 3);
			memcpy(&m_v.m2d.date[1][0], &m_v.m2d.date[2][0], 3);
			m_v.m2d.space[0] = m_v.m2d.space[1];
			m_v.m2d.space[1] = m_v.m2d.space[2];
			memcpy(&m_v.m2d.date[2][0], date, 3);
			m_v.m2d.space[2] = m_v.m2d._space;
		}
		else
		{
			m_v.m2d.space[2] += m_v.m2d._space;
		}

		m_v.m2d._space = 0;		//����2����������̼���
	}	
	
	// 2��Сʱ��¼һ��360���� 
	if( 0 == (m_v.ticks%7200) )
	{
		for(int i=180-1; i>=1; i--)	
			m_v.m360.space[i] = m_v.m360.space[i-1];

		m_v.m360.space[0] = m_v.m360._space;
		m_v.m360._space = 0;	//����2Сʱ��̼���
		
		flush_headrcd();
		PRTMSG(MSG_NOR, "flush_headrcd : sav m360\n");
	}
}
   

//------------------------------------------------------------------------------
// ���ڴ��������ʻ��״̬��
void CDriveRecord::deal_state()
{
	if( 0 != (m_v.ticks%60) )	return;	

//	static int prompt = 0;				//����ʾ����
//	static bool alarmed = false;		//�Ƿ񱨾����ı�־

	// ��ó���״̬(�ƶ���ֹ)
	m_v.stacur = ((CAR_RUNNING==m_v.stacar) && acc_valid()) ? CAR_RUNNING : CAR_STOP;
	
	// ������״̬�ֽ�Ϊ����״̬
	int sta_tmp = m_v.stacur;
	if(m_v.stapre != m_v.stacur) {
		m_v.stacur = (CAR_RUNNING==m_v.stacur) ? CAR_STOP_RUN : CAR_RUN_STOP;
	}
	m_v.stapre = sta_tmp;

	// ����ƽ���ٶ�
	int average = 0, i=0;
	for(i=0; i<60; i++)	
		average += m_v.vector[i];
	average = average/60;

	for(i=14; i>=1; i--)
		m_v.average[i] = m_v.average[i-1];
	m_v.average[0] = average;

	if(CAR_RUN_STOP == m_v.stacur)
	{
		flush_headrcd();	//ͣ��ʱ�洢
		PRTMSG(MSG_NOR, "flush_headrcd : sav average\n");
	}
	
	// ���˾���Ƿ��и���
	if(m_v.numdrv != m_v.rcddrv.numdrv)
	{
		PRTMSG(MSG_NOR, "Driver switch(%u->%u)!\n", m_v.rcddrv.numdrv, m_v.numdrv);
		m_v.run = 0;
		m_v.stop = 0;
		m_v.rcddrv.numdrv = m_v.numdrv;
	}

	switch(m_v.stacur) 
	{
	case CAR_RUNNING:
		{
			m_v.run++;

			if(chk_overtired(m_v.run))
			{
				if(!m_v.alarmed)
				{
					m_v.alarmed = true;
					alarm_overtired(m_v.run);
				}

				if(m_v.prompt < MAX_PROMPT) 
				{
					m_v.prompt++;
					play_tts("˾����������ƣ����ʻ,��ע����Ϣ!");
					PRTMSG(MSG_NOR, "Tire drv(%uh,%umin)!", m_cfg.max_tire, m_cfg.min_sleep);

					// �������Ҳ��ʾ
					char tmpbuf[] = {"˾����������ƣ����ʻ,��ע����Ϣ!"};
					char buf[100] = {0};
					
					buf[0] = 0x01;
					buf[1] = 0x72;
					buf[2] = 0x01;
					memcpy(buf+3, tmpbuf, sizeof(tmpbuf));
					
					DataPush(buf, sizeof(tmpbuf)+3, DEV_QIAN, DEV_DIAODU, LV2);
				}
			}
		}
		break;
		
	case CAR_STOP_RUN:
		{
			if(m_v.stacur!=m_v.stapre)
				PRTMSG(MSG_NOR, "CAR_STOP_RUN");
			
			if(m_v.run > 0) 
			{
				m_v.run += m_v.stop;
			} 
			else
			{
				m_v.rcddrv.numdrv = m_v.numdrv;
				m_v.rcddrv.set_start_gps();
				m_v.rcddrv.set_start_time();
				m_v.metercur = 0;		//����ϴ������
			}
			m_v.stop = 0;
		}
		break;
		
	case CAR_RUN_STOP:
		{
			if(m_v.stacur!=m_v.stapre)
				PRTMSG(MSG_NOR, "CAR_RUN_STOP\n");
			m_v.stop++;
		}
		break;
		
	case CAR_STOP:
		{
			if(m_v.run > 0) 
			{
				m_v.stop++;
				if(m_v.stop >= m_cfg.min_sleep) 
				{
					if( (m_v.run/60) >= CONTINUE_TIME )	
						flush_drvrcd();	//д��1����ʻ��¼	
					
					m_v.run = 0;
					m_v.stop = 0;
					m_v.prompt = 0;
					m_v.alarmed = false;
				}
			}
			else 
			{
				m_v.stop = 0;
			}
		}	
		break;
		
	default:
		m_v.stacur = CAR_STOP;
		break;	
	}
}

//------------------------------------------------------------------------------
// ���ڴ���˾����½ҵ��
void CDriveRecord::deal_login()
{
	static int sta_iLstSta = -1;
	if( sta_iLstSta != m_v.st_log )
	{
		PRTMSG(MSG_NOR, "Driver Sta: %d\n", m_v.st_log );
		sta_iLstSta = m_v.st_log;
	}

	switch(m_v.st_log) 
	{
	case UnLog:
		{
			m_v.numdrv = 0;
			m_v.inputflag = -2;
			
			if(acc_valid()) 
			{
				if(0==m_cfg.total) 
				{
					m_v.st_log = Logined;
					PRTMSG(MSG_NOR, "Driver login succ(Guest)!\n");
				} 
				else 
				{
					req_login();
					m_v.st_log = Inputing;
					PRTMSG(MSG_NOR, "Please input drvcode!\n");
					play_tts("˾������,������������ݴ���!");
				}
			}
		}
		break;
		
	case Inputing:
		{
			static int cnt = 0;
			if(cnt++>120 || (-1==m_v.inputflag) ) 
			{
				char buf[16];
				cnt = 0;
				m_v.st_log = UnLog;

				PRTMSG(MSG_NOR, "Driver login fail!\n");
				g_objMonAlert.SetAlertDrive(true);
				play_tts("˾������,������������,�벻Ҫ�Ƿ���ʻ!");

				memset(buf,0,sizeof(buf));
				buf[0] = (char)0x01;
				buf[1] = (char)0xf8;
				DataPush(buf, 2, DEV_QIAN, DEV_DIAODU, LV2);
			} 
			else if(m_v.inputflag>=0) 
			{
				cnt = 0; 
				m_v.st_log = Logined; 
				m_v.numdrv = m_v.inputflag;

				PRTMSG(MSG_NOR, "Diaodu: Login succ\n");
				
				g_objMonAlert.SetAlertDrive(false);
				flush_headrcd(); // ��������˾����½״̬
			
				play_tts("˾������,��֤ͨ��,ף����;���!");
			} 
			else if(!acc_valid()) 
			{
				m_v.numdrv = 0;
				m_v.st_log = UnLog;

				req_unlog();
				g_objMonAlert.SetAlertDrive(false);
				set_alarm(ALERT_TYPE_DRIVE, false);
			}
		}
		break;	
					
	case Logined:
		{
			if(!acc_valid())
			{
				if( m_v.numdrv != 0)
					play_tts("˾������,�����˳���½��");

				m_v.numdrv = 0;
				m_v.st_log = UnLog;
				
				req_unlog();
				flush_headrcd(); // ��������˾��δ��½״̬
				
				PRTMSG(MSG_NOR, "Driver Logout!\n");
			}
		}
		break;

	default:
		break;
	}
}


//------------------------------------------------------------------------------
// ����Ƿ��г���
// speed: �ٶ�,��λ����/ʱ
bool CDriveRecord::chk_overspeed()
{
	static byte tm_over = 0;
	static bool prompt = false;
	byte speed = m_v.vector[0];	

	if( 0 == m_cfg.max_speed )
	{ 
		tm_over = 0; 
		prompt = false; 
		return false;
	}
	
	if( speed > m_cfg.max_speed ) 
	{
		if(tm_over<255)	
			tm_over++;
	}
	else 
	{
		tm_over = 0;
	}

	if( tm_over > m_cfg.min_over )
	{
		if(!prompt)
		{
			PRTMSG(MSG_NOR, "Overspeed(%usm/h,%us)!!\n", m_cfg.max_speed, m_cfg.min_over);
			play_tts("���ѳ���,���������!");

			g_objMonAlert.SetAlertOverSpd(true);
		}
		prompt = true;
		return true;
	}
	else
	{
		prompt = false;
		return false;
	}
}


//------------------------------------------------------------------------------
// ����Ƿ��е���
// speed: �ٶ�,��λ����/ʱ
bool CDriveRecord::chk_belowspeed()
{
	static int tm_below = 0;
	byte speed = byte(m_v.vector[0]*1.852);	
	
	if( 255 == m_cfg.min_speed )
	{
		tm_below = 0; 
		return false;
	}
	//�ٶ�Ϊ��ʱ�����е��ٱ���
	if( 0 == speed )
	{
		tm_below = 0; 
		return false;
	}
	
	if( speed < m_cfg.min_speed ) 
	{
		tm_below++;
	}
	else 
	{
		tm_below = 0;
	}

	if( tm_below > m_cfg.min_below*60 )
	{
		PRTMSG(MSG_NOR, "Belowspeed(%ukm/h,%um)!!\n", m_cfg.min_speed, m_cfg.min_below);
		g_objMonAlert.SetAlertBelowSpd(true);

		tm_below = 0;
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------------------------------------------------------------
// �����������˾����¼����
void CDriveRecord::req_login()
{
	int i = 0;
	char buf[1024] = {0};
	
	buf[i++] = 0x01;	// ʹ�ô�����չ�У�0x01��ʾҪ���͵�������������
	buf[i++] = 0x62;	//����˾����¼
	buf[i++] = m_cfg.total;
	for(byte j=0; j<10; j++)
	{
		if(m_cfg.inf[j].valid)
		{
			buf[i++] = j;
			buf[i++] = m_cfg.inf[j].num/0xff;
			buf[i++] = m_cfg.inf[j].num%0xff;
			
			byte len = min(strlen(m_cfg.inf[j].ascii), 7);
			buf[i++] = len;
			memcpy(buf+i, m_cfg.inf[j].ascii, len);
			i += len;
		}
	}

	DataPush(buf, i, DEV_QIAN, DEV_DIAODU, LV2);
}


//------------------------------------------------------------------------------
// �����������˾��ȡ����¼ָʾ
void CDriveRecord::req_unlog()
{
	char buf[2] = {0x01, 0x6c};//˾��ȡ����¼ָʾ
	DataPush(buf, 2, DEV_QIAN, DEV_DIAODU, LV2);
}


//------------------------------------------------------------------------------
// �õ�˾��������
int CDriveRecord::get_drvindex(int numdrv)
{
	if(0==numdrv)
		return -1;

	for(int i=0; i<MAX_DRIVER; i++) 
	{
		if(m_cfg.inf[i].valid)
		{
			if(m_cfg.inf[i].num == numdrv)
				return i;
		}
	}

	return -1;
}

//------------------------------------------------------------------------------
// ����¹��ɵ��¼�Ƿ���ϲ�ѯ��׼
bool CDriveRecord::chk_doubrcd(RcdDoub *rcd, byte timecnt, byte time[10][10], byte regioncnt, byte region[5][16])
{
	int i=0;

	// ���ͣ��ʱ���Ƿ��ڲ�ѯʱ�����
	for(i=0; i<timecnt; i++)
	{
		int r1 = memcmp(rcd->times, &time[i][0], 5);
		int r2 = memcmp(rcd->times, &time[i][5], 5);
	
		if(r1>=0 && r2<=0)
			break;

		if((timecnt-1)==i)
			return false;
	}
	
	// ���λ���Ƿ��ڲ�ѯ��λ�÷�Χ��
	for(i=0; i<regioncnt; i++) 
	{
		int r1 = memcmp(rcd->pos[0].lon, &region[i][0], 4);
		int r2 = memcmp(rcd->pos[0].lat, &region[i][4], 4);
		int r3 = memcmp(rcd->pos[0].lon, &region[i][8], 4);
		int r4 = memcmp(rcd->pos[0].lat, &region[i][12], 4);
		
		if(r1>=0 && r2>=0 && r3<=0 && r4<=0)
			break;

		if((regioncnt-1)==i)
			return false;
	}
	
	return true;
}


//------------------------------------------------------------------------------
// �����ʻ��¼�Ƿ���ϲ�ѯ��׼(���ÿ����¼����ʻʱ���Ƿ���ڲ�ѯʱ��ֵ)
bool CDriveRecord::chk_drvrcd(RcdDrv *rcd, byte numdrv, byte times)
{
	struct tm pCurrentTime;
	G_GetTime(&pCurrentTime);
	
	// ����Ƿ�Ϊ2���������ڵļ�¼
	int interval = count_days((pCurrentTime.tm_year+1900), pCurrentTime.tm_mon+1, pCurrentTime.tm_mday) 
			- count_days(rcd->s_time[0]+2000, rcd->s_time[1], rcd->s_time[2]);
	if(interval >= 2)
		return false;
	
	// ���˾������Ƿ�һ��(numdrvΪ0�򲻽�����֤)
	if( (rcd->numdrv!=numdrv) && (numdrv!=0) )
		return false;
	
	// �����ʻʱ���Ƿ񳬹���ѯ��ʱ��
	interval = get_minutes(rcd->s_time[0]+2000, rcd->s_time[1], rcd->s_time[2], rcd->s_time[3], rcd->s_time[4],
		rcd->e_time[0]+2000, rcd->e_time[1], rcd->e_time[2], rcd->e_time[3], rcd->e_time[4]);
	if(times*60 > interval )
		return false;
	
	return true;
}


//------------------------------------------------------------------------------
// �����ʻ��¼�Ƿ������̲�ѯ��׼
bool CDriveRecord::chk_drvrcd2(RcdDrv *rcd, byte type, byte *s_time, byte *e_time, byte numdrv)
{
	// ���˾������Ƿ�һ��(numdrvΪ0�򲻽�����֤)
	if( (rcd->numdrv!=numdrv) && (numdrv!=0) )
		return false;

	switch(type) 
	{
	case 0x01:	//�����ϳ�ʱ���ѯ
		{
			int r1 = memcmp(rcd->s_time, s_time, 5);
			int r2 = memcmp(rcd->s_time, e_time, 5);
		
			if(r1>=0 && r2<=0)
				return true;
		}
		break;

	case 0x02:	//�����³�ʱ���ѯ
		{
			int r1 = memcmp(rcd->e_time, s_time, 5);
			int r2 = memcmp(rcd->e_time, e_time, 5);
			
			if(r1>=0 && r2<=0)
				return true;
		}
		break;

	case 0x03:	//ͬʱ�����ϳ�ʱ����³�ʱ��
		{
			int r1 = memcmp(rcd->e_time, s_time, 5);
			int r2 = memcmp(rcd->e_time, e_time, 5);
			int r3 = memcmp(rcd->e_time, s_time, 5);
			int r4 = memcmp(rcd->e_time, e_time, 5);

			if( (r1>=0 && r2<=0) && (r3>=0 && r4<=0) )
				return true;
		}
		break;

	case 0x04:	//�����ϳ�ʱ����³�ʱ��
		{
			int r1 = memcmp(rcd->e_time, s_time, 5);
			int r2 = memcmp(rcd->e_time, e_time, 5);
			int r3 = memcmp(rcd->e_time, s_time, 5);
			int r4 = memcmp(rcd->e_time, e_time, 5);

			if( (r1>=0 && r2<=0) || (r3>=0 && r4<=0) )
				return true;
		}
		break;

	default:	
		break;
	}
	
	return false;
}

//------------------------------------------------------------------------------
// ���� ��ѯ�¹��ɵ�����Ӧ��
void CDriveRecord::send_3644(RcdDoub *rcd, byte index, byte reply)
{
	int j=0;

	// ����Я��������״̬������֡
	frm36441 frm1 = {0};

	frm1.reply = 0x01;
	frm1.index = index;
	frm1.attr = 0x00;
	frm1.numdrv = rcd->numdrv;
	memcpy(frm1.times, rcd->times, 6);

	for(j=0; j<50; j++)
	{
		frm1.vecsen[j][0] = rcd->vector[j];
		frm1.vecsen[j][1] = rcd->sensor[j];
	}

	tcp_send(0x36, 0x44, &frm1, sizeof(frm1) );
		
	memset(&frm1, 0, sizeof(frm1));
	frm1.reply = 0x01;
	frm1.index = index;
	frm1.attr = 0x01;
	frm1.numdrv = rcd->numdrv;
	memcpy(frm1.times, rcd->times, 6);

	for(j=0; j<50; j++) 
	{
		frm1.vecsen[j][0] = rcd->vector[j+50];
		frm1.vecsen[j][1] = rcd->sensor[j+50];
	}

	tcp_send(0x36, 0x44, &frm1, sizeof(frm1) );
		
	// ����Я����λ����Ϣ������֡
	frm36442 frm2 = {0};
	frm2.reply = 0x01;
	frm2.index = index;
	frm2.attr = 0x02;
	frm2.numdrv = rcd->numdrv;
	memcpy(frm2.times, rcd->times, 6);
	
	for(j=0; j<10; j++) 
	{
		memcpy(&frm2.gps[j][0], &rcd->pos[j], 8);
	}

	tcp_send(0x36, 0x44, &frm2, sizeof(frm2) );
		
	memset(&frm2, 0, sizeof(frm2));
	frm2.reply = reply;
	frm2.index = index;
	frm2.attr = 0x03;
	frm2.numdrv = rcd->numdrv;
	memcpy(frm2.times, rcd->times, 6);
	
	for(j=0; j<5; j++)
	{
		memcpy(&frm2.gps[j][0], &rcd->pos[j+10], 8);
	}

	tcp_send(0x36, 0x44, &frm2, sizeof(frm2) );	
}


//------------------------------------------------------------------------------
// ���ļ�ͷ�ṹд���ļ�����
bool CDriveRecord::flush_headrcd()
{
	FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");
	if(NULL==fp)
		return false;
	
	fseek(fp, offsetof(RcdFile,head), 0);
	fwrite(&m_v, 1, sizeof(m_v), fp);
	
	fclose(fp);
	
	return true;
}


//------------------------------------------------------------------------------
// д��һ����ʻ��¼ 
bool CDriveRecord::flush_drvrcd()
{
	m_v.rcddrv.set_end_gps();
	m_v.rcddrv.set_end_time();
	m_v.rcddrv.space = ushort(m_v.metercur/1000);
	m_v.metercur = 0;	//������������
	
	FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");

	if(fp==NULL) 
		return false;

	fseek(fp, offsetof(RcdFile,drv)+sizeof(RcdDrv)*m_v.drvpw, 0);
	fwrite(&m_v.rcddrv, sizeof(m_v.rcddrv), 1, fp);
	
	if(++m_v.drvcnt >200 )
		m_v.drvcnt = 200;
	if(++m_v.drvpw >=200 )
		m_v.drvpw = 0;

	fseek(fp, offsetof(RcdFile,head), 0);
	fwrite(&m_v, sizeof(m_v), 1, fp);
	fclose(fp);

	PRTMSG(MSG_NOR, "flush_drvrcd, pw=%u, cnt=%u\n", m_v.drvpw, m_v.drvcnt);

	return true;
}


//------------------------------------------------------------------------------
// ��һ���¹��ɵ��¼д���ļ�
bool CDriveRecord::flush_doubrcd()
{	
	m_v.rcddoub.set_time();
	m_v.rcddoub.numdrv = m_v.numdrv;
	
	FILE *fp = fopen(DRIVE_RECORD_FILENAME, "rb+");
	if(NULL==fp)
		return false;

	fseek(fp, offsetof(RcdFile,doub)+sizeof(RcdDoub)*m_v.doubpw, 0);
	fwrite(&m_v.rcddoub, sizeof(m_v.rcddoub), 1, fp);
	
	if(++m_v.doubcnt >100 )
		m_v.doubcnt = 100;

	if(++m_v.doubpw >=100 )
		m_v.doubpw = 0;

	fseek(fp, offsetof(RcdFile,head), 0);
	fwrite(&m_v, sizeof(m_v), 1, fp);
	fclose(fp);
	
	PRTMSG(MSG_NOR, "flush_doubrcd, pw=%u, cnt=%u\n", m_v.doubpw, m_v.doubcnt);

	return true;
}


//------------------------------------------------------------------------------
// �����ٶ����ó�̨����ʻ״̬
// vector: �ٶ�,��λ����/ʱ
void CDriveRecord::set_car_state(tagGPSData gps)
{
	// �����ٶ�ȷ����̨״̬(�ƶ���ֹ)
	static int cnt = 0;
	if(CAR_STOP == m_v.stacar)
	{
		cnt = (VT_LOW_RUN>=m_v.vector[0]) ? 0 : ++cnt ;
		
		if(cnt > RUN_MINTIME) 
		{
			PRTMSG(MSG_NOR, "Car is moving(%f)!\n", m_v.metertotal);
			m_v.stacar = CAR_RUNNING;
			cnt = 0;
		}
	}
	else if(CAR_RUNNING == m_v.stacar)
	{
		cnt = (VT_LOW_RUN>=m_v.vector[0]) ? ++cnt : 0 ;
		
		if(cnt > STOP_MINTIME) 
		{
			PRTMSG(MSG_NOR, "Car is silent(%f)!\n", m_v.metertotal);
			m_v.stacar = CAR_STOP;
			cnt = 0;
		}
	} 
	else 
	{
		m_v.stacar = CAR_STOP;
	}
}


//------------------------------------------------------------------------------
// ��������ʻʱ���Ƿ񳬹����趨��ƣ�ͼ�ʻʱ��
bool CDriveRecord::chk_overtired(int minutes)
{
	if(0 == m_cfg.max_tire)	
		return false;
	if((minutes/60) >= m_cfg.max_tire)
		return true;
	else
		return false;
}


//------------------------------------------------------------------------------
// ƣ����ʻָʾ
// ˾�����(1)+��ʼʱ��(5)+��㾭��(4)+���γ��(4)+����ʻʱ��(1)
// ��ʼʱ�䣺��ʽΪ�ꡢ�¡��ա�ʱ����
// ����ʻʱ�䣺��λΪСʱ
bool CDriveRecord::alarm_overtired(int minutes)
{
	frm3647 frm;

	frm.numdrv = byte(m_v.numdrv);
	memcpy(frm.s_time, m_v.rcddrv.s_time, 5);
	memcpy(frm.s_lon, m_v.rcddrv.s_lon, 4);
	memcpy(frm.s_lat, m_v.rcddrv.s_lat, 4);
	frm.timed = byte(minutes/60);

	tcp_send(0x36, 0x47, &frm, sizeof(frm) );
	
	PRTMSG(MSG_NOR, "Snd tire to center(%dmin)!\n", minutes);
	return true;
}



//------------------------------------------------------------------------------	
// ��������ʱ��֮��ķ�����	
int CDriveRecord::get_minutes(int year, int month, int day, int hour, int minute, 
				int year2, int month2, int day2, int hour2, int minute2 )
{
	int cnt1 = count_days(year, month, day);
	int cnt2 = count_days(year2 , month2, day2);
	
	int cnt3 = 24 * 60 - (hour * 60 + minute);
	int cnt4 = hour2 * 60 + minute2;
	
	int cnt5 = (cnt3 + cnt4) + (cnt2 - cnt1 - 1) * 24 * 60 ;
	
	return cnt5;
}


//------------------------------------------------------------------------------
// �����ķ���TCP����
void CDriveRecord::tcp_send(byte trantype, byte datatype, void *data, int len)
{
#if 0
	char str[100] = {0};
	sprintf(str, "\ntcp_send(%02x,%02x)", trantype, datatype);
	prtbuf(str, (byte*)data, len);
#endif

	int ln = 0;
	byte buf[2048] = {0};

	// ֱ�ӿ�����buf
	if(0x01==trantype || 0x0f == trantype)
	{
		memcpy(buf, data, len);	
		ln = len;
	}

	// ���ͳ��Э����Ҫ��0ת��Ϊ0x7f 
	if(0x09==trantype)
	{
		memcpy(buf, data, len);
		for(int i=0; i<len; i++)
		{
			if(0 == buf[i])	
				buf[i] = 0x7f;
		}

		ln = len;
	}
	
	// �г���¼�� �Լ� �������� ��Ҫ����8ת7
	if(0x36==trantype || 0x37==trantype)
	{
		ln = Code8ToCodeX(buf, (byte*)data, len, 7);
	}

	int ln2 = 0;
	char buf2[2048] = {0};
	int iRet = g_objSms.MakeSmsFrame((char*)buf, ln, trantype, datatype, buf2, sizeof(buf2), ln2);
	if( !iRet ) g_objSock.SOCKSNDSMSDATA( buf2, ln2, LV9 );
}



//------------------------------------------------------------------------------
// ������Ƿ���������
bool CDriveRecord::cut_line(byte *dot1, byte *dot2, byte *dot0)
{
#define DegreeToNum(p)		( ((p)[0]*60 + (p)[1]) * 100 + (p)[2] )

	ulong  x1, y1, x2, y2, x0, y0, temp;
    bool positive;

    x1 = DegreeToNum(dot1);
    y1 = DegreeToNum(dot1+4);
    x2 = DegreeToNum(dot2);
    y2 = DegreeToNum(dot2+4);
    x0 = DegreeToNum(dot0);
    y0 = DegreeToNum(dot0+4);

    if (y0 == y1)	// �������߾�������ζ��㡢���ߺͶ���α�ƽ�е���� 
	{                 
        y0++;
    }
    if (y0 == y2)
	{
        y0++;
    }
    if ((y0 > y1 && y0 > y2) || (y0 < y1 && y0 < y2))
		return false;

    if (x0 < x1 && x0 < x2)
		return false;

    if (x0 > x1 && x0 > x2)
		return true;

    if (y1 > y2) 
	{
        temp = y1;
        y1   = y2;
        y2   = temp;
        temp = x1;
        x1   = x2;
        x2   = temp;
    }

    if (x2 >= x1) /* �����(y2 - y1) / (x2 - x1) >= (y0 - y1) / (x0 - x1)����˵��2�߶��ཻ */
	{                 
        x2 -= x1;
        x1  = x0 - x1;
        positive = true;	/* ����α߳���б��Ϊ�� */
    } 
	else
	{
        x2  = x1 - x2;
        x1  = x1 - x0;
        positive = false;	/* ����α߳���б��Ϊ�� */
    }
    y2 -= y1;
    y1  = y0 - y1;

    if (y2 * x1 >= y1 * x2)
	{
        if (positive) 
		{
            return true;
        }
		else 
		{
            return false;
        }
    }
	else
	{
        if (positive)
		{
            return false;
        } 
		else
		{
            return true;
        }
    }
}

bool CDriveRecord::ChkDriverLog(char *v_pStrDrvNum)
{
	if (!v_pStrDrvNum)
		return 0;

	int i;
	for (i = 0;i<MAX_DRIVER;i++) 
	{
		if (!strncmp(v_pStrDrvNum, m_cfg.inf[i].ascii,min(7,strlen(v_pStrDrvNum)))) 
		{
			m_v.numdrv = m_cfg.inf[i].num;
			return true;
		}
	}

	m_v.numdrv = -1;
	return false;
}

//------------------------------------------------------------------------------
// ���ͻ��������
void CDriveRecord::set_alarm(ulong type, bool set)
{
	if(ALERT_TYPE_DRIVE==type) 
	{
		static bool seted = false;

		if(!seted && set) 
		{ 
			seted = true; 
			g_objMonAlert.SetAlertOther(type, set); 
		
			PRTMSG(MSG_NOR, "Snd Acc alarm!\n"); 
		}

		if(seted && !set) 
		{ 
			seted = false; 
			g_objMonAlert.SetAlertOther(type, set); 
		
			PRTMSG(MSG_NOR, "Cancel Acc alarm!\n"); 
		}	
	}

	if(ALERT_TYPE_TIME==type) 
	{
		static bool seted = false;

		if(!seted && set)
		{ 
			seted = true; 
			g_objMonAlert.SetAlertOther(type, set); 
		
			PRTMSG(MSG_NOR, "Snd Time alarm!!\n"); 
		}
		if(seted && !set)
		{ 
			seted = false; 
			g_objMonAlert.SetAlertOther(type, set); 
		
			PRTMSG(MSG_NOR, "Cancel Time alarm!\n"); 
		}	
	}
	
	if(ALERT_TYPE_RANGE==type) 
	{
		static bool seted = false;

		if(!seted && set) 
		{ 
			seted = true; 
			g_objMonAlert.SetAlertOther(type, set); 
			play_tts("˾����������Խ��!");
			show_diaodu("˾����������Խ��");
		
			PRTMSG(MSG_NOR, "Snd Area alarm!!\n"); 
		}
		if(seted && !set) 
		{ 
			seted = false; 
			g_objMonAlert.SetAlertOther(type, set); 
		
			PRTMSG(MSG_NOR, "Cancel Area alarm!"); 
		}	
	}
	
	if(ALERT_TYPE_RSPEED==type) 
	{
		static bool seted = false;
		if(!seted && set)
		{ 
			seted = true; 
			g_objMonAlert.SetAlertOther(type, set); 
			play_tts("˾����������Խ�粢����,�����!");
			show_diaodu("˾����������Խ�粢����,�����!");
		
			PRTMSG(MSG_NOR, "Snd AreaSpeed alarm!!\n"); 
		}
		if(seted && !set) 
		{ 
			seted = false; 
			g_objMonAlert.SetAlertOther(type, set); 

			PRTMSG(MSG_NOR, "Cancel AreaSpeed alarm!\n"); 
		}	
	}
}


//------------------------------------------------------------------------------
// ��ѯACC״̬
bool CDriveRecord::acc_valid()
{
	byte bytSta;
	IOGet(IOS_ACC, &bytSta);

	return ( bytSta == IO_ACC_ON ? true : false );
}


//------------------------------------------------------------------------------
// ��ѯ���س�״̬
bool CDriveRecord::car_load()
{
	return true;
}


//------------------------------------------------------------------------------
// ��ѯ������ֹ�ƶ�״̬
bool CDriveRecord::car_move()
{
	return (m_v.stacar==CAR_RUNNING);
}


//------------------------------------------------------------------------------
// ��ѯ�����źŵ�״̬
byte CDriveRecord::get_sensor()
{
// 	�����źţ�D7 D6 D5 D4 D3 D2 D1 D0
// 	����D7��D0��0��ʾ���źţ�1��ʾ���ź�
// 		D7--��ɲ�ź�
// 		D6--��ɲ�ź�
// 		D5--��ת����ź�
// 		D4--��ת����ź�
// 		D3--ǰ�����ź�
// 		D2--�����ź�
// 		D1--���������ź�
// 		D0--����
	
// 	ulong sensor = 0;
// 	g_objQianIO.IN_QueryIOSta(sensor);
// 	return ~byte(sensor);
	
	byte sensor = 0, bytIOSignal = 0;
#if USE_COMBUS == 2	
	sensor = g_objCom150TR.GetIOSignal();
#endif
	if( sensor & 0x10 ) bytIOSignal |= 0x80;	// ��ɲ
	if( sensor & 0x08 ) bytIOSignal |= 0x40;	// ��ɲ
	if( sensor & 0x04 ) bytIOSignal |= 0x20;	// ���
	if( sensor & 0x02 ) bytIOSignal |= 0x10;	// �ҵ�
	if( sensor & 0x01 ) bytIOSignal |= 0x08;	// ǰ��
	if( sensor & 0x80 ) bytIOSignal |= 0x04;	// ����
	// ���������źţ��ݿգ�
	
	return bytIOSignal;
}


//------------------------------------------------------------------------------
// �ڵ���������ʾ��ʾ��Ϣ
// fmt,..: ��ʽ������
void CDriveRecord::show_diaodu(char *v_szbuf)
{
	char str[1024] = {0x01, 0x72, 0x01};
	memcpy(str+3, v_szbuf, strlen(v_szbuf));
	DataPush(str, strlen(v_szbuf)+3, DEV_QIAN, DEV_DIAODU, LV2);
}


//------------------------------------------------------------------------------
// TTS�������Žӿ�
void CDriveRecord::play_tts(char *v_szbuf)
{
	char str[1024] = {0x01, 0x00};
	memcpy(str+2, v_szbuf, strlen(v_szbuf));
	DataPush(str, strlen(v_szbuf)+2, DEV_QIAN, DEV_DVR, LV2);
}

unsigned long CDriveRecord::GetTotleMeter()
{
	return (unsigned long)m_v.metertotal;
}

#endif



