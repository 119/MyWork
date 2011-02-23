#include "yx_QianStdAfx.h"

#if USE_METERTYPE == 1

#undef MSG_HEAD
#define MSG_HEAD  ("QianExe-MeterMod  ")

DWORD G_JijiaRead( void* v_pPar )
{
	if( !v_pPar ) return 1;
	return ((CMeterMod*)v_pPar)->JijiaRead();
}

DWORD G_JijiaWork( void* v_pPar )
{
	if( !v_pPar ) return 1;
	return ((CMeterMod*)v_pPar)->JijiaWork();
}

DWORD G_InvalidCarry( void* v_pPar )
{
	if( !v_pPar ) return 1;
	return ((CMeterMod*)v_pPar)->InvalidCarry();
}

//====================================================================================

CMeterMod::CMeterMod()
{
	must_up = false;
	m_hJijia = -1;
	m_bCarrySkip = false;
}

CMeterMod::~CMeterMod()
{

}

int CMeterMod::Init()
{
	int iRet;

#if USE_COMBUS == 0		// ʹ�ô�����չ��ʱ�����������߳�
	// �������߳�
	iRet = pthread_create(&m_hthJijiaRead, NULL, (void *(*)(void*))G_JijiaRead, (void *)this);
	if(0 !=iRet)
	{
		PRTMSG(MSG_ERR,"�����Ƽ������ڶ�ȡ�߳�ʧ�ܣ�\n");  
	}
	else
	{
		PRTMSG(MSG_ERR,"�����Ƽ������ڶ�ȡ�̳߳ɹ���\n");
	}
#endif

	// ���������߳�
	iRet = pthread_create(&m_hthJijiaWork, NULL, (void *(*)(void*))G_JijiaWork, (void *)this);
	if(0 !=iRet)
	{
		PRTMSG(MSG_ERR,"�����Ƽ��������߳�ʧ�ܣ�\n");  
	}
	else
	{
		PRTMSG(MSG_ERR,"�����Ƽ��������̳߳ɹ���\n");
	}

#if VEHICLE_TYPE == VEHICLE_M	
	iRet = pthread_create(&m_hInvalidCarry, NULL, (void *(*)(void*))G_InvalidCarry, (void *)this);
	if(0 !=iRet)
	{
		PRTMSG(MSG_ERR,"�����ؿͲ�������߳�ʧ�ܣ�\n");  
	}
	else
	{
		PRTMSG(MSG_ERR,"�������ؿͲ�������̳߳ɹ���\n");
	}
#endif

	return 0;
}

int CMeterMod::Release()
{
	close(m_hJijia);
	m_hJijia = -1;

	return 0;
}

bool CMeterMod::JijiaComOpen()
{
	int i;
	struct termios options;
	const int COMOPEN_TRYMAXTIMES = 5;

	for( i = 0; i < COMOPEN_TRYMAXTIMES; i ++ )
	{
		// �򿪼Ƽ�������
		m_hJijia = open("/dev/ttySIM0", O_RDWR);
//		m_hJijia = open("/dev/ttyAMA1", O_RDWR | O_NONBLOCK);	//testc,������
		if( -1 == m_hJijia )
		{	
			PRTMSG(MSG_ERR, "Open serial error!");
			perror(" ");
			sleep(1);
			continue;
		} 
		else 
		{
			if(tcgetattr(m_hJijia, &options) != 0)
			{
				perror("GetSerialAttr");
				break;
			}

			options.c_iflag &= ~(IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL|IUCLC|IXON|IXOFF|IXANY); 
			options.c_lflag &= ~(ECHO|ECHONL|ISIG|IEXTEN|ICANON);
			options.c_oflag &= ~OPOST;
			
			cfsetispeed(&options,B2400);	//���ò����ʣ����������ò�����Ϊ9600
			cfsetospeed(&options,B2400);
			
			tcsetattr(m_hJijia,TCSANOW,&options);
				
			usleep(500000);
			return true;
		}
	}
	PRTMSG(MSG_ERR,"�򿪼Ƽ�������ʧ��!\n");
	return false;
}

// ���ڶ��߳�
// p: �����ָ��
DWORD CMeterMod::JijiaRead()
{		
	sleep(2);
	
	if(!JijiaComOpen())		return 0;
	
	// ѭ����ȡ���ڲ�����_DealComChar������յ���ÿ���ֽ�
	int i;
	int readed = 0;
	char buf[512] = {0};

	while(!g_bProgExit) 
	{
		readed = 0;
		memset(buf, 0, sizeof(buf));
		readed = read( m_hJijia, buf, 512);
		if(readed>0)
		{
			DealComBuf(buf, readed);
		}
		usleep(50000);
	}
}

// ���������߳�
// p: �����ָ��
DWORD CMeterMod::JijiaWork()
{
	sleep(2);
	
	while(!g_bProgExit) 
	{
		_Work();
		usleep(100000);
	}
}

#if VEHICLE_TYPE == VEHICLE_M
DWORD CMeterMod::InvalidCarry()
{
	while(!g_bProgExit)
	{
		_CheckSta();
		usleep(200000);
	}
}

bool CMeterMod::_CheckSta()
{
	int iRet;
	bool bDoorSta;//1:open 0:close
	bool bPsgSta;//1:�п� 0:�޿�
	static bool bPsgStaChange = 0;//1:�ؿ�״̬�仯 0:�ؿ�״̬δ�仯
	static DWORD dwPsgChgStaTime = GetTickCount();//�ؿ�״̬�仯��ʱ
	static DWORD dwPsgStaTime = GetTickCount();//�ؿ�״̬��ʱ
	static DWORD dwRptTime = GetTickCount();
	static unsigned char uszPsg1Sta = IO_PASSENGER1_INVALID;
	static unsigned char uszPsg2Sta = IO_PASSENGER1_INVALID;
	static unsigned char uszPsg3Sta = IO_PASSENGER1_INVALID;
	tagGPSData objGps(0);
	memset( &objGps, 0, sizeof(objGps) );
	unsigned char uszRetAcc,uszRetJijia,uszRetFdoor,uszRetBdoor;
	unsigned char uszRetPsg1,uszRetPsg2,uszRetPsg3;

	tag2QInvCarryCfg objCarryCfg;
	GetSecCfg( &objCarryCfg, sizeof(objCarryCfg), offsetof(tagSecondCfg, m_uni2QInvCarryCfg.m_obj2QInvCarryCfg), sizeof(objCarryCfg) );

	if(0x01 != objCarryCfg.m_bytCarryRpt)
	{
		PRTMSG(MSG_DBG,"CarryRpt disable\n");
		sleep(2);
		return false;
	}

	GetCurGps( &objGps, sizeof(objGps), GPS_REAL ); // ��ȡGPSʵʱ����

	//ACC�ź�
	IOGet((byte)IOS_ACC, &uszRetAcc );
	//ǰ�����ź�
	IOGet((byte)IOS_FDOOR, &uszRetFdoor );
	//�����ź�
	IOGet((byte)IOS_BDOOR, &uszRetBdoor );
	//�ؿ��ź�1
	IOGet((byte)IOS_PASSENGER1, &uszRetPsg1 );
	//�ؿ��ź�2
	IOGet((byte)IOS_PASSENGER2, &uszRetPsg2 );
	//�ؿ��ź�3
	IOGet((byte)IOS_PASSENGER3, &uszRetPsg3 );
	//�Ƽ����ź�
	IOGet((byte)IOS_JIJIA, &uszRetJijia);


	bDoorSta = ( (IO_BDOOR_OPEN==uszRetBdoor)||(IO_FDOOR_OPEN==uszRetFdoor) );//����״̬
	
	bPsgSta = ( (IO_PASSENGER1_VALID==uszRetPsg1) || (IO_PASSENGER2_VALID==uszRetPsg2) ||
		(IO_PASSENGER3_VALID==uszRetPsg3) );//�ؿ�״̬

	if (bPsgSta)
	{
		dwPsgStaTime = GetTickCount();
	}
	if ( (GetTickCount()-dwPsgStaTime)>2*60*1000 )//����2���ӣ�С��dwRptTime�ļ�ʱʱ�䣩�޿ͣ���m_bCarrySkip
	{
		m_bCarrySkip = false;
	}
	//�ؿ�����仯
	if( (uszRetPsg1!=uszPsg1Sta) || (uszRetPsg2!=uszPsg2Sta) || (uszRetPsg3!=uszPsg3Sta) )
	{
		dwPsgChgStaTime = GetTickCount();
		bPsgStaChange = true;
	}
	if( (GetTickCount()-dwPsgChgStaTime) >10000 ) //�ؿ�״̬��Ч����10s���Ա��жϳ���
	{
		bPsgStaChange = false;
	}

	if ( (objGps.speed<5) && bDoorSta && bPsgStaChange )
//	if ( bDoorSta && bPsgStaChange )	//testc
	{
		dwRptTime = GetTickCount();
		m_bCarrySkip = true;
		bPsgStaChange = false;
	}

//	//testc
//  	objGps.speed = 20;
//  	objGps.valid = 'A';
//	PRTMSG(MSG_DBG,"ACC:%d\n bDoorSta:%d\n bPsgStaChange:%d\n JIJIA:%d\n m_bCarrySkip:%d\n",uszRetAcc,bDoorSta,bPsgStaChange,uszRetJijia,m_bCarrySkip);
//	PRTMSG(MSG_DBG,"bPsgSta:%d\n IOS_FDOOR:%d\n IOS_bDOOR:%d\n IOS_PASSENGER1:%d\n IOS_PASSENGER2:%d\n IOS_PASSENGER3:%d\n",bPsgSta,uszRetFdoor,uszRetBdoor,uszRetPsg1,uszRetPsg2,uszRetPsg3);
//	sleep(2);
//	//testc


	if( (true==m_bCarrySkip) && ('A' == objGps.valid) && (objGps.speed>10) && bPsgSta &&
		(IO_JIJIA_LIGHT==uszRetJijia) && (IO_ACC_ON==uszRetAcc) && ((GetTickCount()-dwRptTime)>3*60*1000) )
	{
		_InvCarryRpt(objGps);
		dwRptTime = GetTickCount();
		PRTMSG(MSG_DBG,"invalid carry...\n");
		m_bCarrySkip = false;
	}

	uszPsg1Sta = uszRetPsg1;
	uszPsg2Sta = uszRetPsg2;
	uszPsg3Sta = uszRetPsg3;
}
#endif

//�������ϱ��ؿͲ������
void CMeterMod::_InvCarryRpt(const tagGPSData &v_objGps)
{
	tagQianGps objQianGps;
	g_objMonAlert.GpsToQianGps( v_objGps, objQianGps );
	_Snd0159( objQianGps );
}
void CMeterMod::_Snd0159( tagQianGps &v_objQianGps )
{
	int iRet;
	int iBufLen = 0;
	char buf[100] = { 0 };
	PRTMSG(MSG_DBG,"send 1059 frame\n");
//	iRet = g_objSms.MakeSmsFrame((char*)&v_objQianGps, sizeof(v_objQianGps), 0x01, 0x59, buf, sizeof(buf), iBufLen);
	iRet = g_objSms.MakeSmsFrame((char*)&v_objQianGps, 19, 0x01, 0x59, buf, sizeof(buf), iBufLen);
	if( !iRet ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, DATASYMB_SMSBACKUP );
}


int CMeterMod::Deal1015( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	tag2QInvCarryCfg obj2QCarryCfg,obj2QCarryCfgBkp;
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	BYTE bytResType = 0;
	
	// ���ݲ������
	if( 1 != v_dwDataLen )
	{
		PRTMSG(MSG_DBG,"inv carry para err\n");
		iRet = ERR_PAR;
		goto DEAL1015_FAILED;
	}
	PRTMSG(MSG_DBG,"rcv invCarry cfg\n");
	// ��ȡ->�޸�
	iRet = GetSecCfg( &obj2QCarryCfg, sizeof(obj2QCarryCfg), offsetof(tagSecondCfg, m_uni2QInvCarryCfg.m_obj2QInvCarryCfg), sizeof(obj2QCarryCfg) );
	if( iRet ) goto DEAL1015_FAILED;
	obj2QCarryCfgBkp = obj2QCarryCfg; // �ȱ���
	obj2QCarryCfg.m_bytCarryRpt = BYTE(v_szData[0]);
	// д�غͷ���Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x45, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		if( 0 != SetSecCfg(&obj2QCarryCfg, offsetof(tagSecondCfg, m_uni2QInvCarryCfg.m_obj2QInvCarryCfg), sizeof(obj2QCarryCfg)) )
		{
			goto DEAL1015_FAILED;
		}
		
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL ); // ����
		if( iRet ) // ������ʧ��
		{
			SetSecCfg( &obj2QCarryCfgBkp, offsetof(tagSecondCfg, m_uni2QInvCarryCfg.m_obj2QInvCarryCfg), sizeof(obj2QCarryCfgBkp) ); // �ָ�ԭ����
			goto DEAL1015_FAILED;
		}
	}
	else goto DEAL1015_FAILED;
	
	return 0;
	
DEAL1015_FAILED:
	bytResType = 0x0;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x45, buf, sizeof(buf), iBufLen); 
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV11, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
	return iRet;
}

int CMeterMod::Deal0119( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
//	m_bCarrySkip = true;
	return 0;
}

void CMeterMod::DealComBuf(char *v_szBuf, int v_iLen)
{
	int i=0;
	for( i=0; i<v_iLen; i++)
	{
		_DealComChar(v_szBuf[i]);
	}
}

// ����Ӵ����յ���ÿ���ַ�,�������������ݲ����������
// ch: ��������ֽ� 	
void CMeterMod::_DealComChar(byte ch)
{
	static byte frm[512] = {0};
	static int len = 0;
	static bool rcv_bf = false;
	static bool rcv_02 = false;
	static bool rcv_03 = false;
	static int sta = 0;
	
	
	if(0xbf==ch && !rcv_bf)		{ rcv_bf = true; return; }
	
	if(rcv_bf)	{
		rcv_bf = false;
		if(ch==0x02)	rcv_02 = true;
		if(ch==0x03)	rcv_03 = true;
		if(ch==0x06)	return;
		if(ch==0x15)	return;
	}
	
	switch(sta) 
	{
	case 0:
		len = 0;
		if(rcv_02)	{ rcv_02 = false; sta = 1; }
		break;
		
	case 1:
		if(rcv_03) {
			rcv_03 = false;
			_DealCom(frm, len);
			sta = 0;
		} else {
			frm[len++] = ch;
		}
		break;
		
	default:
		len = 0;
		rcv_bf = false;
		rcv_02 = false;
		rcv_03 = false;
		sta = 0;
		break;
	}
}

//====================================================================================
// ����Ƽ�����������
void CMeterMod::_DealCom(byte* frm, int len)
{
	// ��֡����У��
	byte crc = 0;
	for(int i=0; i<len-1; i++)	crc ^= frm[i];
	if(crc!=frm[len-1])		
	{ 
		PRTMSG(MSG_ERR,"Meter Crc Err\n");
		return; 
	}
	
	// ���Ƽ���ACK
	_ComSend(0);
	
	// ���³�̨��Ƽ������һ�����ӵ�ʱ��
	char tmp = 0x10;
	DataPush( &tmp, 1, DEV_QIAN,DEV_QIAN, 2);
	
	// У����ȷ
	switch(frm[0])
	{
	case 0x30:	// �س�����
		PRTMSG(MSG_DBG,"RcvMeter: �س�����\n");
		_SetFull(&ptif);
		break;
		
	case 0x32:	// ͣ������
		PRTMSG(MSG_DBG,"RcvMeter: ͣ������\n");
		_SetEmpty(&ptif);
		break;
		
	case 0x34:	// �ճ�����
		PRTMSG(MSG_DBG,"RcvMeter: �ճ�����\n");
		_SetEmpty(&ptif);
		break;
		
	case 0x36:	// �ϴ�Ӫ����������
		PRTMSG(MSG_DBG,"RcvMeter: �ϴ�Ӫ����������\n");
		{
			// 30���,�ճ�ʱ�����Ϳճ�ָ��,ֱ�ӷ����ϴ�Ӫ����������
			// ����������ģ��ճ�ָ��
			_SetEmpty(&ptif);
			
			// ���Up2022Э��֡
			Up2022 up;
			memcpy(up.ptif, &ptif, 23);
			memcpy(up.data, frm+1, 60);

// 			printf("testc data:");
// 			for (int i=0;i<60;i++)
// 			{
// 				printf("%x ",up.data[i]);
// 			}
//			printf("\n");
			
			// ���û����ݽ���8ת7
			byte tmp[2048] = {0};

// 			printf("testc up date :");
// 			byte test[2048];
// 			memcpy(test,&up,sizeof(up));
// 			for (int i=0;i<sizeof(up);i++)
// 			{
// 				printf("%x ",test[i]);
// 			}
//			printf("\n");

			int tmplen = Code8ToCodeX(tmp, (byte*)&up, sizeof(up), 7);
			
			// ������ѹ���ϴ����в������ϴ���־
			que_up.push(tmp, tmplen);
			must_up = true;
		}
		break;
		
	case 0x38:	// �Ƽ�������
		PRTMSG(MSG_DBG,"RcvMeter: �Ƽ�������\n");
		break;
		
	case 0x3A:	// �Ƽ�����½
		PRTMSG(MSG_DBG,"RcvMeter: �Ƽ�����½\n");
		break;
		
	case 0x3C:	// �Ƽ�����ǩ
		PRTMSG(MSG_DBG,"RcvMeter: �Ƽ�����ǩ\n");
		break;
		
	case 0x3E:	// ά������
		PRTMSG(MSG_DBG,"RcvMeter: ά������\n");
		break;
		
	case 0x40:	// �����ϴ�����
		PRTMSG(MSG_DBG,"RcvMeter: �����ϴ�����\n");
		break;
		
	default:
		break;
	}
	
}

//====================================================================================
// ���Ƽ������ڷ�������
bool CMeterMod::_ComSend(byte cmd, byte* data, int len)
{
	if(0==cmd) 
	{
#if USE_COMBUS == 0
		byte tmp[] = { 0xbf, 0x06 };
		ulong writed = 0;
		writed = write(m_hJijia, tmp, 2);
		return (writed == 2) ? true : false;
#endif

#if USE_COMBUS == 1
		DWORD dwPacketSymb;
		char buf[] = {0xf0, 0xbf, 0x06 };
		g_objComBus.m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)sizeof(buf), buf, dwPacketSymb);
		return true;
#endif
	} 
	else 
	{
		
		if(len<0 && len>500)	return false;
		if(NULL==data)	len = 0;
		
		byte buf[512] = {0};
		int i = 0, j = 0;
		byte crc = 0;
		
		// ��֡
#if USE_COMBUS == 1	
		buf[i++] = 0xf0;	// 0xf0��ʾ����չ�е�������2����
#endif
		buf[i++] = 0xbf;	// ֡ͷ	
		buf[i++] = 0x02;
		
		buf[i++] = cmd;		
		memcpy(buf+i, data, len);
		i += len;
		for(j=2; j<i; j++)	crc ^= buf[j];	// ����У����
		buf[i++] = crc;	
		
		buf[i++] = 0xbf;	// ֡β
		buf[i++] = 0x03;

#if USE_COMBUS == 0
		// ����֡���ݷ�������
		ulong writed = 0;
		writed = write(m_hJijia, buf, i);
		return (writed == i) ? true : false;
#endif

#if USE_COMBUS == 1
		DWORD dwPacketNum;
		g_objComBus.m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)i, (char*)buf, dwPacketNum);
		return true;
#endif
	}
}	

//====================================================================================
// �����ķ���TCP����
void CMeterMod::_TcpSend(byte trantype, byte datatype, byte *data, int len)
{
	int ln2 = 0;
	char buf2[2048] = {0};
//que �����
//	PRTMSG(MSG_DBG,"jijia tcp send\n");
	int iRet = g_objSms.MakeSmsFrame((char*)data, len, trantype, datatype, buf2, sizeof(buf2), ln2);
	if( !iRet ) g_objSock.SOCKSNDSMSDATA( buf2, ln2, LV9 );
}


//====================================================================================
// ������Ӫ���ݵ�״̬��
void CMeterMod::_Work()
{
	static int stawork = 0;
	
	switch(stawork)
	{
	case 0:	// ����
		{
			if(must_up) { must_up = false; stawork = 1; break; }
			
			// ���TCP���ն���
			static byte tmp[2048] = {0};
			int len = tcprcv.pop(tmp);
			if( len>0 && len<2048 )		_DealCentData(tmp[0], tmp+1, len-1);
		}
		break;
		
	case 1:	// �ϴ�
		{
//			PRTMSG(MSG_DBG,"Start to upload cash data!\n");
			
			byte buf[2048] = {0};
			ulong len = 0;
			
			while( len = que_up.pop(buf) ) 
			{
				_TcpSend(0x20, 0x22, buf, len);
				if(!_ChkAck(0x62)) 
				{
					que_up.push(buf, len);
					break;
				}
			}
			stawork = 0;
		}
		break;
		
	default:
		stawork = 0;
		break;
	}
	
}	

//====================================================================================
// ��Ⲣ�ȴ�����Ӧ��
bool CMeterMod::_ChkAck(byte datatype)
{
	byte tmp[2048] = {0};
	ulong len = 0;
	
	int left = 400;		// ���ȴ�40����
	while(left>0) 
	{
		len = tcprcv.pop(tmp);
		if(len>0) 
		{
			byte ret = _DealCentData(tmp[0], tmp+1, len-1);
			if(tmp[0]==datatype && ret==0xff) 	break;
		}
		left--;
		usleep(100000);
	}
	PRTMSG(MSG_DBG,"_ChkAck %s\n", (left>0) ? "Succ!!!" : "Fail!!!!!!!!!" );
	return (left>0) ? true : false;
}	


//====================================================================================
// �������ĵ�˰��Э��
byte CMeterMod::_DealCentData(byte type, byte *data, int len)
{
	byte ret = 0;
	
	if(len<=0 && len>2048)	return ret;
	if(NULL==data)			return ret;
	
	// ���û����ݽ���7->8��ת��
	byte tmp[2048] = {0};
	CodeXToCode8(tmp, data ,len, 7);
	
	switch(type) 
	{
	case 0x61:
		break;
		
	case 0x62:
		ret = tmp[0];
		break;
		
	case 0x63:
		break;
		
	default:
		break;
	}
	
	return ret;
}

//====================================================================================
// �������ĵ�˰��Э��
void CMeterMod::_SetFull(PointInfo *p)
{
	struct tm pCurrentTime;
	G_GetTime(&pCurrentTime);
	
	p->date[0] = (byte)(pCurrentTime.tm_year +1900 - 2000);
	p->date[1] = (byte)(pCurrentTime.tm_mon+1);//�·�0~11
	p->date[2] = (byte)pCurrentTime.tm_mday;
	p->s_tm[0] = (byte)pCurrentTime.tm_hour;
	p->s_tm[1] = (byte)pCurrentTime.tm_min;

	tagGPSData gps(0);
	GetCurGps(&gps, sizeof(gps), GPS_LSTVALID);
	
	double db = 60*(gps.latitude-byte(gps.latitude));
	p->s_lat[0] = byte(gps.latitude);
	p->s_lat[1] = (byte)db;
	p->s_lat[2] = byte(long(db*100)%100);
	p->s_lat[3] = byte(long(db*10000)%100);
	
	db = 60*(gps.longitude-byte(gps.longitude));
	p->s_lon[0] = byte(gps.longitude);
	p->s_lon[1] = (byte)db;
	p->s_lon[2] = byte(long(db*100)%100);
	p->s_lon[3] = byte(long(db*10000)%100);

	// �ճ����س�ʱ��Ƶ�����ϴ�
	char szbuf[100];
	int  iBufLen = 0;
	byte bytType = 0x01;		// ��Ƶ�����ϴ�����Ϊ0x01 
	int iResult = g_objSms.MakeSmsFrame((char*)&bytType, 1, 0x38, 0x46, szbuf, sizeof(szbuf), iBufLen);
	if( !iResult ) g_objSock.SOCKSNDSMSDATA( szbuf, iBufLen, LV12, 0 );
		
	// ģ���س��¼�
	//que �����
//	::PostMessage( g_hWndMain, WM_IORCV, WPARAM(EVENT_JIJIA_VALID), 0 );
}


//====================================================================================
// �������ĵ�˰��Э��
void CMeterMod::_SetEmpty(PointInfo *p)
{
	struct tm pCurrentTime;
	G_GetTime(&pCurrentTime);
	
	p->e_tm[0] = (byte)pCurrentTime.tm_hour;
	p->e_tm[1] = (byte)pCurrentTime.tm_min;	
	
	tagGPSData gps(0);
	//testc
//	GetCurGps(&gps, sizeof(gps), GPS_LSTVALID);
	GetCurGps(&gps, sizeof(gps), GPS_LSTVALID);
	double db = 60*(gps.latitude-byte(gps.latitude));
	p->e_lat[0] = byte(gps.latitude);
	p->e_lat[1] = (byte)db;
	p->e_lat[2] = byte(long(db*100)%100);
	p->e_lat[3] = byte(long(db*10000)%100);
	
	db = 60*(gps.longitude-byte(gps.longitude));
	p->e_lon[0] = byte(gps.longitude);
	p->e_lon[1] = (byte)db;
	p->e_lon[2] = byte(long(db*100)%100);
	p->e_lon[3] = byte(long(db*10000)%100);
	
	// ģ���ᳵ�¼�
	//�����
//	::PostMessage( g_hWndMain, WM_IORCV, WPARAM(EVENT_JIJIA_INVALID), 0 );
}

#endif

