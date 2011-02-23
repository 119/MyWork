#include "yx_QianStdAfx.h"

#if USE_METERTYPE == 2

#undef MSG_HEAD
#define MSG_HEAD  ("QianExe-MeterModKaiTian  ")

void *G_ThreadKTMeterModRecv(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	g_objKTMeter.P_ThreadRecv();
	
	pthread_exit(0);
}

void *G_ThreadKTMeterModWork(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	g_objKTMeter.P_ThreadWork();
	
	pthread_exit(0);
}

void G_TmDownProc(void *arg, int arg_len)
{
	g_objKTMeter.P_TmDownProc();
}

void G_TmSetDeviceProc(void *arg, int arg_len)
{
	g_objKTMeter.P_TmSetDeviceProc();
}

CKTMeterMod::CKTMeterMod()
{
	m_iComPort = -1;
	m_pthreadRecv = 0;
	m_pthreadWork = 0;
	
	m_DownCt = 0;
  m_SendCt  = 0;
	m_SendSta=TAXRECV;
	m_DataType=0;
	m_DataLen=0;
	must_up=false;
	last_pack=false;
	
	m_bEscRecv = false;
	m_bEscWork = false;
	
	pthread_mutex_init(&m_mutexTaxiDataSta, NULL);
	pthread_mutex_init(&m_mutexMeterDataSta, NULL);
}
	
CKTMeterMod:: ~CKTMeterMod()
{
	
}

int CKTMeterMod::Init()
{
#if USE_COMBUS == 0
	// �򿪴���
	if( !_ComOpen() )
	{
		PRTMSG(MSG_ERR, "Open Com failed!\n");
		return ERR_COM;
	}
#endif

	m_wMeterSta=0;
	link=false;
	_CheckTaxiFile();//����Ƿ���δ������ļƼ����ļ�
	if( false==_InitTaxiData() ) 
		return -1;//��ʼ���ͼ��ˢ������
	if( false==_InitMeterData() ) 
		return -1;//��ʼ���ͼ����Ӫ����

	memset(m_DriverInf,0,sizeof(m_DriverInf));
	m_objUp2068.Init();
		
	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg), offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	memcpy(m_DriverInf,objTaxiCfg.m_aryDriverInf,sizeof(m_DriverInf));
	if(objTaxiCfg.m_bytJiJiaEnable==0x01)//��ʼ���Ƽ���״̬
		m_bJijiaEnable=false;
	else 
		m_bJijiaEnable=true;
	//taxfile.renewflag=true;//zzg debug
		
#if USE_COMBUS == 0		// ʹ�ô�����չ��ʱ�����������߳�
	if( pthread_create(&m_pthreadRecv, NULL, G_ThreadKTMeterModRecv, NULL) != 0 )
	{
		Release();
		PRTMSG(MSG_ERR, "Create Recv Thread failed!\n");
		return ERR_THREAD;
	}
#endif

	if( pthread_create(&m_pthreadWork, NULL, G_ThreadKTMeterModWork, NULL) != 0 )
	{
		Release();
		PRTMSG(MSG_ERR, "Create Work Thread failed\n");
		return ERR_THREAD;
	}
	
	return 0;
}

int CKTMeterMod::Release()
{
#if USE_COMBUS == 0
	if( m_pthreadRecv )
	{
		m_bEscRecv = true;
		usleep(100000);
		
		if( WaitForEvent(ThreadExit, &m_pthreadRecv, 3000) )
		{
			pthread_cancel(m_pthreadRecv);
			usleep(100000);
		}

		m_pthreadRecv = 0;
	}
	m_bEscRecv = false;

	_ComClose();
#endif

	if( m_pthreadWork )
	{
		m_bEscWork = true;
		usleep(100000);
		
		if( WaitForEvent(ThreadExit, &m_pthreadWork, 3000) )
		{
			pthread_cancel(m_pthreadWork);
			usleep(100000);
		}

		m_pthreadWork = 0;
	}
	m_bEscWork = false;
	
	pthread_mutex_destroy(&m_mutexTaxiDataSta);
	pthread_mutex_destroy(&m_mutexMeterDataSta);
	
	return 0;
}

void CKTMeterMod::_CheckTaxiFile()
{
	bool resul=false;
	char szpath[100] = {0};
	sprintf( szpath, "%s", TAXIFILEPATH );
	FILE *fp = fopen(szpath, "rb+");
	if(fp!=NULL)
	{
		TaxiFileHead head;
		fread(&head,sizeof(head),1,fp);		
// 		{   //zzg debug
// 			head.curNum=1;
// 			//head.endLen=16;
// 			fseek(fp,0,0);
// 			fwrite(&head,sizeof(head),1,fp); 
//		}
		taxfile.id=head.fileID;
		taxfile.no=head.totalCnt;
		taxfile.endlen=head.endLen;
		m_CurNo=head.curNum;
		if(head.curNum<head.totalCnt)
		{
			taxfile.flag=false;
			_SetTimer(&g_objTimerMng, DOWN_TIMER,10000,G_TmDownProc);
		}
		else 
		{
			taxfile.flag=true;
		}
	}
	if(NULL!=fp) fclose(fp);
}

bool CKTMeterMod::_InitTaxiData()
{
	bool resul=false;
	char szpath[100] = {0};
	sprintf( szpath, "%s", TAXIDATAPATH );
	FILE *fp = fopen(szpath, "rb+");
	if(fp!=NULL)
	{
		//����Ƿ���δ�������ˢ������
		DataHead head;
		fread(&head,sizeof(head),1,fp);
// 	    head.curIndex=94;
//		head.sendCnt=1;
//   	fwrite(&head,sizeof(head),1,fp);
		
		if(head.curIndex>=TOTALDATACNT||head.sendCnt>TOTALDATACNT)
		{//zzg mod 091201
			head.curIndex=0;
			head.sendCnt=0;
			fseek(fp,0,0);
			fwrite(&head,sizeof(head),1,fp);
		}
		else if(head.sendCnt>0)
		{
			printf("Hav Taxi Data:%d",head.sendCnt);
			must_up=true;
		}
		fclose(fp);
		return true;
	}
  fp=fopen(szpath, "wb+");
	if(NULL==fp)	return false;
	DataHead head;
//  head.curIndex=90;
//	head.sendCnt=1;
	if(1==fwrite(&head,sizeof(head),1,fp))
	{
		resul=true;
	}
	if(NULL!=fp) fclose(fp);
	return resul;
}

bool CKTMeterMod::_InitMeterData()
{
	bool resul=false;
	char szpath[100] = {0};
	sprintf( szpath, "%s", METERDATAPATH );
	FILE *fp = fopen(szpath, "rb+");
	if(fp!=NULL)
	{
		//����Ƿ���δ�����������
		DataHead head;
		fread(&head,sizeof(head),1,fp);
// 	    head.curIndex=94;
//		head.sendCnt=1;
//   	fwrite(&head,sizeof(head),1,fp);	
		if(head.curIndex>=TOTALDATACNT||head.sendCnt>TOTALDATACNT)
		{//zzg mod 091201
			head.curIndex=0;
			head.sendCnt=0;
			fseek(fp,0,0);
			fwrite(&head,sizeof(head),1,fp);
		}
		else if(head.sendCnt>0)
		{
			printf("Hav Meter Data:%d",head.sendCnt);
			must_up=true;
		}
		fclose(fp);
		return true;
	}
  fp=fopen(szpath, "wb+");
	if(NULL==fp)	return false;
	DataHead head;
//  head.curIndex=90;
//	head.sendCnt=1;
	if(1==fwrite(&head,sizeof(head),1,fp))
	{
		resul=true;
	}
	if(NULL!=fp) fclose(fp);
	return resul;
}

//-----------------------------------------------------------------------------------------------------------------
// �򿪵������Ĵ���
// ����: �򿪳ɹ�����ʧ��
bool CKTMeterMod::_ComOpen()
{
	int i;
	struct termios options;
	const int COMOPEN_TRYMAXTIMES = 5;

	for( i = 0; i < COMOPEN_TRYMAXTIMES; i ++ )
	{
		// �򿪼Ƽ�������
		m_iComPort = open("/dev/ttySIM0", O_RDWR);
		if( -1 == m_iComPort )
		{	
			PRTMSG(MSG_ERR, "Open serial error!");
			perror(" ");
			sleep(1);
			continue;
		} 
		else 
		{
			if(tcgetattr(m_iComPort, &options) != 0)
			{
				perror("GetSerialAttr");
				break;
			}

			options.c_iflag &= ~(IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL|IUCLC|IXON|IXOFF|IXANY); 
			options.c_lflag &= ~(ECHO|ECHONL|ISIG|IEXTEN|ICANON);
			options.c_oflag &= ~OPOST;
			
			cfsetispeed(&options,B19200);	//���ò����ʣ����������ò�����Ϊ9600
			cfsetospeed(&options,B19200);
			
			tcsetattr(m_iComPort,TCSANOW,&options);
				
			usleep(500000);
			return true;
		}
	}
	
	PRTMSG(MSG_ERR,"�򿪼Ƽ�������ʧ��!\n");
	return false;
}

bool CKTMeterMod::_ComClose()
{
	if(m_iComPort != -1)
	{
		close(m_iComPort);
		m_iComPort = -1;
		
		return true;
	}
	
	return false;
}
//====================================================================================
// ����Ӵ����յ���ÿ���ַ�,�������������ݲ�����
// ch: ��������ֽ� 	
int CKTMeterMod::_DealComChar(byte * chs,int dataLen)
{
	static byte frm[ALABUF] = {0};
	static int len = 0;
	int delCnt=1;
	byte type=0xff;
	//s_count=1;
	if (len == 0) 
	{ 
		if (chs[0] == 0xAA) 
		{
			frm[0]=0xAA;
			len++;
		}
	} 
	else 
	{
		if ( len == 1 ) 
		{
			type=chs[0];	
		}
		else type=frm[1];
		
		switch( type )
		{    
			case 0x00:	
			case 0x01:
			case 0x02:
			case 0x05:	
			case 0x06:	
			case 0x07:
			case 0x09:
			case 0x0c:
			case 0x0d:
				frm[len++]=chs[0];
				if ( len > 3 ) 
				{
					_DealCom(frm,len);
					len = 0;
				} 
				break;
			case 0x03:	
				frm[len++]=chs[0];
				if ( len > 5 )
				{
					_DealCom(frm,len);
					len = 0;
				}
				break;
			case 0x04://һ��ͨˢ������
				//if ( s_msgLen%50 == 0 )ClearWatchdog(); 
				//frm[len++]=ch; 
				if ( len+dataLen > m_DataLen+2 ) 
				{
					memcpy(frm+len,chs,dataLen);
					len+=dataLen;
					_DealCom(frm,len);
					len = 0;
					delCnt=dataLen;
				} 
				break;	
			case 0x0b://������Ӫ����
				if ( len+dataLen>19+2)
				{
					memcpy(frm+len,chs,dataLen);
					len+=dataLen;
					_DealCom(frm,len);
					len = 0;
					delCnt=dataLen;
				}
				break;
			case 0x08:  
				frm[len++]=chs[0];
				if ( len > 4 ) 
				{
					_DealCom(frm,len);
					len = 0;
				} 
				break;
			case 0x0a:
				frm[len++]=chs[0];
				if ( len > 6 ) 
				{
					_DealCom(frm,len);
					len = 0;
				} 
				break;
			case 0xAA://��ֹ��������0xaa�ĳ���ʹ״̬���쳣
				len=0;
				frm[len++]=0xaa;
				break;
		  default  : //��ֹ��������
			  len=0;
			  break;
	   }
	}
  return delCnt;
}
//====================================================================================
// ����Ƽ�����������
void CKTMeterMod::_DealCom(byte* frm, int len)
{
	// ��֡����У��
	byte crc = 0;
	for(int i=0; i<len-1; i++)
		crc += frm[i];
	if(crc!=frm[len-1])		
	{ 
		PRTMSG(MSG_DBG, " Crc Err:", (char*)&crc, 1);
		byte crcErr=0;
		_ComSend(0xff,&crcErr,1);
		return; 
	}

	switch(frm[1])
	{
		case 0x00:  
			hdl_recv0x00();
     	break;
    case 0x01:  
    	hdl_recv0x01();
      break;
    case 0x02:  
    	hdl_recv0x02();
      break;
    case 0x03:	
    	hdl_recv0x03(frm+2);
      break;
    case 0x04:	
    	hdl_recv0x04(frm+2, len-3);
      break;
    case 0x05:  
    	hdl_recv0x05();
      break;
    case 0x06:	
    	hdl_recv0x06(frm+2);
      break;
    case 0x07:	
    	hdl_recv0x07();
      break;
    case 0x08:	
    	hdl_recv0x08(frm+2);
      break;
    case 0x09:  
    	hdl_recv0x09(frm+2);
      break;
		case 0x0a:	
			hdl_recv0x0a(frm+2, len-3);
			break;
		case 0x0b:	
			hdl_recv0x0b(frm+2, len-3);
			break;
		case 0x0c:	
			hdl_recv0x0c(frm+2);
			break;
		case 0x0d:	
			hdl_recv0x0d(frm+2);
			break;
   	default:	
   		break;
	}
}
/****************************************************************
*   ������:    hdl_recv0x00
*   ��������:  ͬ����̨
*�� ����:      ��
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x00(void)
{
	byte send[3];
    
  send[0] = SERIALNUMBER >> 8;           // ��̨���
  send[1] = SERIALNUMBER & 0x00FF;
  send[2] = 0x00;                        // ����ͨ��
  _ComSend(0x00,send,sizeof(send));
	if(link==false)
	{
		link=true;
		g_objKTDeviceCtrl.SetDeviceSta(KT_METER,true);
		PRTMSG(MSG_NOR, "Meter Connect!\n");
	}
	_KillTimer(&g_objTimerMng, SETDEVICE_TIMER);
	_SetTimer(&g_objTimerMng, SETDEVICE_TIMER,30*1000,G_TmSetDeviceProc);
}

/****************************************************************
*   ������:    hdl_recv0x01
*   ��������:  ��ѯ�¼�
*�� ����:      ��
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x01(void)
{ 
	byte taxflag;
  taxflag = 0x00;
  if (taxfile.flag) 
		taxflag |= 0x02;       // �����ļ�
  if (taxfile.renewflag) 
		taxflag |=0x01;				 // ��������
	if(!m_bJijiaEnable)			// �رռƼ���
		taxflag |=0x04;
  _ComSend(0x01,&taxflag,1);  
}

/****************************************************************
*   ������:    hdl_recv0x02
*   ��������:  ����ͨѶ
*�� ����:      ��
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x02(void)
{
	byte resData=0x00;
	_ComSend(0x02,&resData,1);
}

/****************************************************************
*   ������:    hdl_recv0x03
*   ��������:  ����ˢ����������
*�� ����:      ��������
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x03(byte *data)
{
	WORD_UNION len;
  m_DataType = *data++;
	byte resType=0x00;
  len.bytes.high = *data++;
  len.bytes.low = *data;
  if(taxfile.renewflag && m_DataType==0x01 && len.word<=0x348) 
	{    
		// ɾ�����ɱ�־λ
   	taxfile.renewflag = false;
    //StorePubPara(TXFILEDOWN_); 
  }  
  
  if (len.word >0x348) 
	{   
		// ���ݴ���840�ֽ�ֻ����840
		m_DataLen=0x348;
		resType=0x01; 
  }
	else
	{	
		m_DataLen=len.word;
		last_pack=true;
	}

	_ComSend(0x03,&resType,1);
}

/****************************************************************
*   ������:    hdl_recv0x04
*   ��������:  ˢ�����ݴ洢
*�� ����:      INT8U *data: ��Ӫ����
*              INT8U len  : ��Ӫ���ݳ���
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x04(byte *data, WORD len)
{
	PRTMSG(MSG_NOR, "Deal 0x04 Succ:", (char*)data, 3);

	if (m_SendSta==TAXRECV) 
	{
    if(_FlushTaxiDataRcd(data,len))
		{
			byte resType=0;	
			_ComSend(0x04, &resType, 1);
			m_SendSta=TAXOK;
			if(last_pack)
			{
				//zzg test
				//must_up=true;//--zzg debug
				last_pack=false;
			}
		}
  }
}
/****************************************************************
*   ������:    hdl_recv0x05
*   ��������:  ��ѯ����״̬
*�� ����:      ��
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x05()
{
	byte sndState;
  if (m_SendSta==TAXOK) 
	{
    sndState = 0x03;// �ɹ�
  } 
	else
	{
    if ( !g_objSock.IsOnline() ) 
      sndState = 0x01;           // ������
		else 
			sndState = 0x00;        // ���ڴ�����
  }   
  
  _ComSend(0x05, &sndState, 1);
}

/****************************************************************
*   ������:    hdl_recv0x06
*   ��������:  ɾ�������ļ�
*�� ����:      INT8U *data: ɾ������
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x06(byte *data)//���޸�
{   
	byte resType=0;
	_ComSend(0x06,&resType,1);
	if (*data == 0x02 || *data == 0x04 || *data == 0x06)
	{   // ɾ�������ļ�
		PRTMSG(MSG_NOR, "Rcv Meter 0x06: Del TaxiFile!");
		char szpath[100] = {0};
		sprintf( szpath, "%s", TAXIFILEPATH );
		FILE *fp = fopen(szpath, "rb");
		if(fp!=NULL)
		{
			fclose(fp);
			unlink(szpath);
		}
    taxfile.flag = false;
		m_CurNo=0;
	}
}

/****************************************************************
*   ������:    hdl_recv0x07
*   ��������:  �����ļ�����
*�� ����:      ��
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x07(void)
{
	byte senddatabuf[2];
  WORD_UNION totallenght; 

  totallenght.word = 0x0040*(taxfile.no-1) + taxfile.endlen;
  senddatabuf[1] = totallenght.bytes.low;   
  senddatabuf[0] = totallenght.bytes.high;
	_ComSend(0x07, senddatabuf, sizeof(senddatabuf));
}

/****************************************************************
*   ������:    hdl_recv0x08
*   ��������:  �����ļ�
*�� ����:      INT8U *data: �ڼ���
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x08(byte *data)
{
	WORD_UNION curno;
  byte sendlen;
	int offset=0;
	byte buffer[64+3]={0};
  curno.bytes.high =data[0];
  curno.bytes.low = data[1];
	if (curno.word > taxfile.no)
	{     
	  taxfile.flag = false;
    return;
  } 
	char szpath[100] = { 0 };
	sprintf( szpath, "%s", TAXIFILEPATH );
	FILE *fp = fopen(szpath, "rb+");
	if(NULL==fp)
	{
		return;
	}
  if (curno.word == taxfile.no) // ���һ��
	{    
    sendlen = taxfile.endlen;
    PRTMSG(MSG_NOR, "Snd Last TaxFilePck:%d,%d",curno.word,sendlen);
  } 
	else 
	{
    sendlen = 64;
  } 
	offset=sizeof(TaxiFileHead)+(curno.word-1)*64;
	fseek(fp,offset,0);
	if(fread(buffer+3,sendlen,1,fp))
	{
		buffer[0]=sendlen;
		buffer[1]=data[0];
		buffer[2]=data[1];
		_ComSend(0x08,buffer,3+sendlen);
		PRTMSG(MSG_NOR, "Snd TaxFilePck:%d,%d",curno.word,sendlen);
	}

	if(NULL!=fp) fclose(fp);
}

/****************************************************************
*   ������:    hdl_recv0x09
*   ��������:  ɾ������״̬��־λ����
*�� ����:      ��
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv0x09(byte *data)
{
	byte resType=0;
	_ComSend(0x09,&resType,1); 
  if (*data == 0) 
  {                  
    m_SendSta = TAXRECV; // ɾ������ɹ��ı�־
  }   
}

//���ػ�����˾�������Ϣ
void CKTMeterMod::hdl_recv0x0a(byte *data, WORD Len)
{
	byte resType=0;
	_ComSend(0x0a,&resType,1); 
	byte bytJijiaSta=0;
	byte aryDriverInfo[3]={0};
	bytJijiaSta=data[0];
	memcpy(aryDriverInfo,data+1,sizeof(aryDriverInfo));
	PRTMSG(MSG_NOR, "cur driver:%x%x%x",aryDriverInfo[0],aryDriverInfo[1],aryDriverInfo[2]);
	if(memcmp(aryDriverInfo,m_DriverInf,sizeof(aryDriverInfo)))//���Ӱ�
	{
		frm4048 frm;
		PRTMSG(MSG_NOR, "driver change:%x%x%x-->%x%x%x",m_DriverInf[0],m_DriverInf[1],m_DriverInf[2],aryDriverInfo[0],aryDriverInfo[1],aryDriverInfo[2]);
		//LED��ȡ����ʾ������Ϣ
		g_objKTLed.SetLedShow('B',2,0);
		//�������������Ӱ���Ϣ
		memcpy(frm.oldDriver,m_DriverInf,sizeof(frm.oldDriver));
		memcpy(frm.curDriverInfo,aryDriverInfo,sizeof(frm.oldDriver));
		memcpy(m_DriverInf,aryDriverInfo,sizeof(aryDriverInfo));

		tagTaxiCfg objTaxiCfg;
		memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
		GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
		offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
		memcpy(objTaxiCfg.m_aryDriverInf,m_DriverInf,sizeof(m_DriverInf));
		SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
		//���ͽ�����Ϣ��������
		g_objKTDriverCtrl.SndDrvChgInf(m_DriverInf,3);
		//�������Ľ�����Ϣ
		tagGPSData objGps(0);
		g_objMonAlert.GetCurGPS( objGps, true );
		tagQianGps objQianGps;
		g_objMonAlert.GpsToQianGps( objGps, objQianGps );
		frm.data[0]=objQianGps.m_bytYear;
		frm.data[1]=objQianGps.m_bytMon;
		frm.data[2]=objQianGps.m_bytDay;
		frm.data[3]=objQianGps.m_bytHour;
		frm.data[4]=objQianGps.m_bytMin;
		char buf[200]={0};
		int iRet=0;
		int iBufLen = 0;
		iRet = g_objSms.MakeSmsFrame( (char*)&frm, sizeof(frm), 0x40, 0x48, buf, sizeof(buf), iBufLen );
		if( !iRet ) 
		{
			iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9, 0 );
		}
	}
}
//���䵱����Ӫ����
void CKTMeterMod::hdl_recv0x0b(byte *data, WORD Len)
{
	byte resType=0;
	//���λ����Ϣ
	tagGPSData objGps(0);
	g_objMonAlert.GetCurGPS( objGps, true );
	tagQianGps objQianGps;
	g_objMonAlert.GpsToQianGps( objGps, objQianGps );
	memcpy(m_objUp2068.gpsInf.endTime,(byte*)&objQianGps+4,sizeof(m_objUp2068.gpsInf.endTime));
	memcpy(m_objUp2068.gpsInf.endLat,(byte*)&objQianGps+8,sizeof(m_objUp2068.gpsInf.endLat));
	memcpy(m_objUp2068.gpsInf.endLon,(byte*)&objQianGps+12,sizeof(m_objUp2068.gpsInf.endLon));
	//�����Ӫ����
	memcpy(&(m_objUp2068.meterData),data,sizeof(m_objUp2068.meterData));
	//�ԼƼ����ϴ���������Ϣ���д���
	byte month=m_objUp2068.meterData.day;
  m_objUp2068.meterData.day=m_objUp2068.meterData.month;
	m_objUp2068.meterData.month=month & 0x3f;
	m_wMeterSta |=METERDATA_DATA;
	if(METERDATA_COMPLETE==m_wMeterSta)
	{
		_FlushMeterDataRcd((byte *)&m_objUp2068,sizeof(m_objUp2068));
		m_wMeterSta=0;
		must_up=true;//--zzg debug
	}
	_ComSend(0x0b,&resType,1); 
}
//��ѯ���䵱��Ӫ������
void CKTMeterMod::hdl_recv0x0c(byte *data)
{
	byte resType=0;
	_ComSend(0x0c,&resType,1); 
}
//��ѯ�ؿ�����
void CKTMeterMod::hdl_recv0x0d(byte *data)//δ�յ�������ָ��
{
	byte resType=4;
	_ComSend(0x0d,&resType,1); 
}
//====================================================================================
// ���Ƽ������ڷ�������
bool CKTMeterMod::_ComSend(byte cmd, byte* data, int len)
{	
	if(len<0 && len>500)	return false;
	if(NULL==data)	len = 0;
	static int rcv00Cnt=0;
	
	byte buf[512] = {0};
	int i = 0, j = 0;
	byte crc = 0;		
	
	// ��֡
#if USE_COMBUS == 1	
	buf[i++] = 0xf0;	// 0xf0��ʾ����չ�е�������2����
#endif
	buf[i++] = 0xbb;	// ֡ͷ	
	buf[i++] = cmd;	
	memcpy(buf+i, data, len);
	i += len;
  for(j=0; j<i; j++)	
 	crc += buf[j];	// ����У����
	buf[i++] = crc;		
	
#if USE_COMBUS == 0
		// ����֡���ݷ�������
		ulong writed = 0;
		writed = write(m_iComPort, buf, i);
		return (writed == i) ? true : false;
#endif

#if USE_COMBUS == 1
		DWORD dwPacketNum;
		g_objComBus.m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)i, (char*)buf, dwPacketNum);
		return true;
#endif
}
//====================================================================================
// ������Ӫ���ݵ�״̬��
void CKTMeterMod::_Work()
{
	static int stawork = 0;
	static DWORD upCheckTick=GetTickCount();
// 	byte data[]={0x00,0x00,0xaa};
//	_ComSend(0xAA,data,sizeof(data));

	switch(stawork)
	{
		case 0:	// ����
		{
			if(must_up) 
			{ 
				must_up = false; 
				if(true==_GetTaxiDataRcd())
					stawork = 1;
				if(true==_GetMeterDataRcd())
					stawork = 1;
				upCheckTick=GetTickCount(); 
				break; 
			}
			
			if(GetTickCount()-upCheckTick>30*1000)//30����һ�ζ���
			{
				upCheckTick=GetTickCount();
				//���ˢ�����ݶ���
				if(que_taxi.getcount()>0&&g_objSock.IsOnline())
				{
					stawork = 1;
					break;
				}
				
				//�����Ӫ���ݶ���
				if(que_meter.getcount()>0&&g_objSock.IsOnline())
				{
					stawork = 1;
					break;
				}
			}
			
			// ���TCP���ն���
			static byte tmp[2048] = {0};
			int len = tcprcv.pop(tmp);
			if( len>0 && len<2048 )		
				hdl_cent(tmp[0], tmp+1, len-1);
		}
			break;
			
		case 1:	// �ϴ�
		{
			//����ˢ������
			_SendCashData();
			bool bUp=false;
			
			if(0==que_taxi.getcount())
			{
				if(_GetTaxiDataRcd()) bUp=true;
			}
				
			//������Ӫ����
			_SendMeterData();
			if(0==que_meter.getcount())
			{
				if(_GetMeterDataRcd()) 
					bUp=true;
			}
			
			if(!bUp)	
				stawork=0;
			upCheckTick=GetTickCount();
		}
			break;
			
		default:
			stawork = 0;
			break;
	}
}
//====================================================================================
// �����ķ���TCP����
void CKTMeterMod::_TcpSend(byte trantype, byte datatype, byte *data, int len ,int cvtType)
{
	int ln2 = 0;
	char buf2[2048] = {0};
	int iRet = g_objSms.MakeSmsFrame((char*)data, len, trantype, datatype, buf2, sizeof(buf2),ln2,cvtType);
	if( !iRet ) 
	{
		g_objSock.SOCKSNDSMSDATA( buf2, ln2, LV9 );
	}
}	
//====================================================================================
// �������ĵ�˰��Э��
byte CKTMeterMod::hdl_cent(byte type, byte *data, int len)
{
	byte ret = 0;
	
	if(len<=0 && len>2048)	return ret;
	if(NULL==data)			return ret;
	
// ���û����ݽ���7->8��ת��
//	byte tmp[2048] = {0};
//	CodeXToCode8(tmp, data ,len, 7);
	
	switch(type) 
	{
		case 0x05:
			hdl_recv2005();
			break;
		case 0x06:
			hdl_recv2006();
			break;
		case 0x65:
			hdl_recv2065(data,len);
			break;
		case 0x66:
			hdl_recv2066(data);
			break;
		case 0x67:
			if (data[0] == 0xFF)
			{
				ret=0xFF;
			}
			break;
		case 0x28:
			{
				if(data[0]==0x01)
				{
					ret=0xFF;
				}
			}	
		default:
			break;
	}
	
	return ret;
}

//���üƼ���
void CKTMeterMod::hdl_recv2005()
{
	byte bytRes=0x00;
	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg), offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	m_bJijiaEnable=false;
	objTaxiCfg.m_bytJiJiaEnable=0x01;
	SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	bytRes=0x01;
	_TcpSend(0x20,0x47,&bytRes,1);
}

//���üƼ���
void CKTMeterMod::hdl_recv2006()
{
	byte bytRes=0x00;
	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg), offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	m_bJijiaEnable=true;
	objTaxiCfg.m_bytJiJiaEnable=0x00;
	SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	bytRes=0x01;
	_TcpSend(0x20,0x48,&bytRes,1);
}
/**************************************************************
*   ������:    HdlRecv2066
*   ��������:  ���ղ�������ӿ�
*�� ����:      INT8U *Data: ���������־
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv2066(byte *Data)
{
	PRTMSG(MSG_NOR, "Rcv 2066\n");
	byte bytRes=0x00;
  if (*Data == 0x00) 
	{ 
    if (taxfile.renewflag != true)
		{
      taxfile.renewflag = true;
    }
		bytRes=0xff;
		_TcpSend(0x20,0x26,&bytRes,1); 
	}
}
/****************************************************************
*   ������:    HdlRecv2065
*   ��������:  �����������ؽӿ�
*�� ����:      INT8U *Data: ������������
*              INT16U Len : �����������ݳ���
*   ����ֵ:    ��
***************************************************************/
void CKTMeterMod::hdl_recv2065(byte *Data, WORD Len)
{
  _KillTimer(&g_objTimerMng, DOWN_TIMER); 
  PRTMSG(MSG_NOR, "Rcv 2065\n");
    
	frm2025 frm={0};
	frm.resType=0xff;
  if ( Len>4)
	{
		if(Data[1]==0&&Data[2]==0&&Data[3]==0&&Data[4]==0)//�յ�00����	
      return;
  } 
	if(taxfile.flag)
	{
		PRTMSG(MSG_NOR, "Have Taxi File\n");
	}
  if (!taxfile.flag&&Len!=0 && Len>6)
	{
		_StorageFileData(Data, Len);
	}
  frm.id=taxfile.id;
	frm.tolNum[0]=(taxfile.no>>8)&0x00ff;
	frm.tolNum[1]=taxfile.no&0x00ff;
	frm.curNum[0]=(m_CurNo>>8)&0x00ff;
	frm.curNum[1]=m_CurNo&0x00ff;
	_TcpSend(0x20,0x25,(byte*)&frm,sizeof(frm));
	_SetTimer(&g_objTimerMng, DOWN_TIMER,DOWNPERIOD,G_TmDownProc);
}

void CKTMeterMod::P_ThreadRecv()
{
	sleep(2);
	ulong readed = 0;
	char buf[ALABUF] = {0};

	while( !g_bProgExit )
	{
		if( m_bEscRecv )
			break;
			
		readed = 0;
		memset(buf, 0, sizeof(buf));
		readed = read( m_iComPort, buf, ALABUF);
		if(readed>0)
		{
// 			if( readed>0 )
// 			{
//				PRTMSG(MSG_DBG, "Rcv From Meter:%s\n", (char*)buf);//zzg debug
//			}

			for(ulong i=0; i<readed;)
			{
				i+=_DealComChar((byte*)buf+i,readed-i);
			}
		}
	}
	
	m_bEscRecv = false;
		
	return;
}
	
void CKTMeterMod::P_ThreadWork()
{
	sleep(10);

	while( !g_bProgExit )
	{
		if( m_bEscWork )
			break;
			
		_Work();
		usleep(50000);
	}
	
	m_bEscWork = false;

	return;
}

void CKTMeterMod::P_PlayTxt(char *fmt)
{
#if USE_TTS == 1
	char str[1024] = {0};
	str[0] = 0x01;		// 0x01��ʾTTS����
	str[1] = 0x00;		// 0x01��ʾ�������ű�����
	strcpy(str+2, fmt);
	DataPush(str, (DWORD)(2+strlen(fmt)), DEV_QIAN, DEV_DVR, LV2);
#endif
}

void CKTMeterMod::P_TmDownProc()
{
	//StartTmr(s_downtmr, DOWNPERIOD);
  if (taxfile.no == 0) 
	{
    _KillTimer(&g_objTimerMng, DOWN_TIMER);
    return;
  }
  if (m_DownCt < MAX_RESEND) 
		m_DownCt++;
  else 
	{
    //StartTmr(s_downtmr, MINUTE, 30);    // �ӳٶ���ش����޸�
    m_DownCt = 0;
		_KillTimer(&g_objTimerMng, DOWN_TIMER);
    return;
  } 
	hdl_recv2065(0, 0);
}

void CKTMeterMod::P_TmSetDeviceProc()
{
	DWORD dwVeSta=g_objCarSta.GetVeSta();
	if(dwVeSta & VESTA_HEAVY)
	{
		PRTMSG(MSG_NOR, "SetMeterSta But Car Is HEAVY\n");
	}
	else //�ճ�
	{
		link=false;
		g_objKTDeviceCtrl.SetDeviceSta(KT_METER,false);
		PRTMSG(MSG_NOR, "Meter Disconnect\n");
		_KillTimer(&g_objTimerMng, SETDEVICE_TIMER);
	}
}
/****************************************************************
*   ������:    _StorageFileData
*   ��������:  �洢��������
*�� ����:      INT8U *Data: ����ָ��
*              INT16U Len : ���ݳ���
*   ����ֵ:    ״̬
***************************************************************/
bool CKTMeterMod::_StorageFileData(byte *Data, WORD Len)
{   
  //WORD sectorno, offsetno;
  WORD_UNION fileno,filecurno;
  fileno.bytes.high = Data[1];
  fileno.bytes.low = Data[2];
  filecurno.bytes.high = Data[3];
  filecurno.bytes.low = Data[4];
  PRTMSG(MSG_NOR, "fileno:%d,filecur:%d,m_curno:%d\n",fileno.word,filecurno.word,m_CurNo);
  if(fileno.word==0||filecurno.word!=m_CurNo) 
  	return false;
	char szpath[100] = { 0 };
	sprintf( szpath, "%s", TAXIFILEPATH );
	FILE *fp =NULL;
	TaxiFileHead head;
  if ( filecurno.word == 0 ) 
	{  
		// ��һ��
		fp=fopen(szpath, "rb+");
		if(fp!=NULL)
		{
			fclose(fp);
			unlink(szpath);
		}
		fp=fopen(szpath, "wb+");
		if(NULL==fp)	
			return false;
	
	  m_CurNo = 0;	    
	  taxfile.id = Data[0];
    taxfile.no = fileno.word;
		head.fileID=taxfile.id;
		head.totalCnt=taxfile.no;
		head.curNum=0;
		PRTMSG(MSG_NOR, "Begin Rcv TaxiFile,ID:%d,Tol:%d\n",taxfile.id,taxfile.no);
		fwrite(&head,sizeof(head),1,fp);
  }
	else
	{
		fp = fopen(szpath, "rb+");
		if(fp==NULL) return false;
		fseek(fp,0,0);
		fread(&head,sizeof(head),1,fp);
	}
	PRTMSG(MSG_NOR, "Rcv TaxiFile:%d\n",filecurno.word);
	if(head.fileID!=taxfile.id||head.totalCnt!=taxfile.no||head.curNum!=filecurno.word)
	{ 	
		if(NULL!=fp) fclose(fp); 
		return false;
	}
  if (filecurno.word == (taxfile.no-1))
	{   
		// ���һ��
		PRTMSG(MSG_NOR, "Rcv TaxiFile Complete\n");
		taxfile.flag = true;
    taxfile.endlen = Data[6];
		head.endLen=Data[6];
		
	  //StorePubPara(TXFILEDOWN_);
  } 
	int offset=sizeof(head)+head.curNum*TAXIFILEPACKSIZE;
  fseek(fp,offset,0);
	fwrite(Data+7,Len-7,1,fp);
	m_CurNo=filecurno.word+1;
	head.curNum++;
	fseek(fp,0,0);
	fwrite(&head,sizeof(head),1,fp);
	if(NULL!=fp) fclose(fp);
	return true;
}
//====================================================================================
bool CKTMeterMod::_GetTaxiDataRcd()
{
	pthread_mutex_lock(&m_mutexTaxiDataSta);
	bool result=false;
	DataHead head;
	int cnt=0;
	char szpath[100] = {0};
	sprintf( szpath, "%s", TAXIDATAPATH );
	FILE *fp =fopen(szpath, "rb+");
	if(NULL==fp)	goto LEAVECS;
	fread(&head,sizeof(head),1,fp);
	if(head.sendCnt>0&&head.curIndex>=0)
	{
		int curIndex=head.curIndex;
		while(cnt<head.sendCnt)
		{
			fseek(fp,sizeof(head)+curIndex*sizeof(Up2027),0);
			Up2027 taxiData;
			if(!fread(&taxiData,sizeof(taxiData),1,fp)) 
			{//����ȡʧ�ܳ�ʼ���ļ�״̬--zzg mod 091201
				head.curIndex=0;
				head.sendCnt=0;
				fseek(fp,0,0);
				fwrite(&head,sizeof(head),1,fp);
				break;
			}
			if(false==que_taxi.push((byte*)&taxiData, sizeof(taxiData)-sizeof(taxiData.data)+taxiData.len)) break;
			curIndex++;
			cnt++;
		}
		if(cnt>0)result=true;//zzg mod 091201
	}
	PRTMSG(MSG_NOR, "get taxi--cur:%d; sendCnt:%d\n",head.curIndex,head.sendCnt);//zzg debug
LEAVECS:
	if(NULL!=fp) fclose(fp);
	pthread_mutex_unlock(&m_mutexTaxiDataSta);
	return result;
}

bool CKTMeterMod::_GetMeterDataRcd()
{
	pthread_mutex_lock(&m_mutexMeterDataSta);
	bool result=false;
	DataHead head;
	int cnt=0;
	char szpath[100] = {0};
	sprintf(szpath, "%s", METERDATAPATH);
	FILE *fp =fopen(szpath, "rb+");
	if(NULL==fp)	goto LEAVECS;
	fread(&head,sizeof(head),1,fp);
	if(head.sendCnt>0&&head.curIndex>=0)
	{
		int curIndex=head.curIndex;
		while(cnt<head.sendCnt)
		{
			fseek(fp,sizeof(head)+curIndex*sizeof(Up2068),0);
			Up2068 up2028;
			if(!fread(&up2028,sizeof(up2028),1,fp)) 
			{//����ȡʧ�ܳ�ʼ���ļ�״̬--zzg mod 091201
				head.curIndex=0;
				head.sendCnt=0;
				fseek(fp,0,0);
				fwrite(&head,sizeof(head),1,fp);
				break;
			}
			if(false==que_meter.push((byte*)&up2028, sizeof(up2028))) break;
			curIndex++;
			cnt++;
		}
		if(cnt>0)result=true;//zzg mod 091201
	}
	PRTMSG(MSG_NOR, "get meter--cur:%d; sendCnt:%d\n",head.curIndex,head.sendCnt);//zzg debug
LEAVECS:
	if(NULL!=fp) fclose(fp);
	pthread_mutex_unlock(&m_mutexMeterDataSta);
	return result;
}

//�޸��ļ�ͷ���ͼ�¼��0Ϊˢ�����ݣ�1Ϊ��Ӫ����
bool CKTMeterMod::_FlushDataHead(byte type,WORD Cnt)
{
	if(0==type)
		pthread_mutex_lock(&m_mutexTaxiDataSta);
	else
		pthread_mutex_lock(&m_mutexMeterDataSta);

	bool result=false;
	DataHead head;
	char szpath[100] = {0};
	if(0==type) 
		sprintf( szpath, "%s", TAXIDATAPATH );
	else 
		sprintf( szpath, "%s", METERDATAPATH );

	FILE *fp=fopen(szpath, "rb+");
	if(NULL==fp)	
		goto LEAVECS;
	
	fread(&head,sizeof(head),1,fp);
	if(head.sendCnt-Cnt>0)
	{
		head.sendCnt-=Cnt;
		head.curIndex=(head.curIndex+Cnt)%TOTALDATACNT;//zzg mod 091201
	}
	else
	{
		head.sendCnt=0;
		head.curIndex=0;
	}
	fseek(fp,0,0);
	fwrite(&head,sizeof(head),1,fp);
	
	if(0==type)
	{
		PRTMSG(MSG_DBG, "taxi data--curIndex:%d, sendCnt:%d\n",head.curIndex,head.sendCnt);
	}
	else
	{
		PRTMSG(MSG_DBG, "meter data--curIndex:%d, sendCnt:%d\n",head.curIndex,head.sendCnt);
	}
	result=true;
LEAVECS:
	if(NULL!=fp) 
		fclose(fp);
	if(0==type)
		pthread_mutex_unlock(&m_mutexTaxiDataSta);
	else
		pthread_mutex_unlock(&m_mutexMeterDataSta);
	return result;
}

bool CKTMeterMod::_FlushTaxiDataRcd(byte *Data,WORD Len)
{
	pthread_mutex_lock(&m_mutexTaxiDataSta);
	bool result=false;
	int dataSize=UPLOAD_CNT*TAXIDATASIZE;
	int lastData=Len;
	DataHead head;
	int rcdIndex=0;
	int offset=0;
	int rcdCnt=0;
	char szpath[100] = {0};
	sprintf( szpath, "%s", TAXIDATAPATH );
	FILE *fp =fopen(szpath, "rb+");
	if(NULL==fp)	goto LEAVECS;
	fread(&head,sizeof(head),1,fp);
	rcdIndex=(head.curIndex+head.sendCnt)%TOTALDATACNT;
	fseek(fp,sizeof(head)+rcdIndex*sizeof(Up2027),0);
	while(lastData>0||offset>Len)
	{
		if(rcdIndex>=TOTALDATACNT)
		{	
			rcdIndex=0;//���ļ�ͷ��ʼ�洢 
		}
		
		if(rcdIndex==head.curIndex&&head.sendCnt>0) 
		{ 
			break;//�����ļ���������
		}
		
		Up2027 taxidata;
		taxidata.type=m_DataType;
		if(lastData-dataSize>=0)
		{
			taxidata.len=dataSize;
		}
		else
		{
			taxidata.len=lastData;
		}
		taxidata.cnt=taxidata.len/TAXIDATASIZE;
		memcpy(taxidata.data,Data+offset,taxidata.len);
		offset+=taxidata.len;
		fwrite(&taxidata,sizeof(taxidata),1,fp);
		lastData-=dataSize;
		rcdCnt++;
		rcdIndex++;
	}
	if(lastData<=0) result=true;
	head.sendCnt+=rcdCnt;
	fseek(fp,0,0);
	fwrite(&head,sizeof(head),1,fp);
	PRTMSG(MSG_NOR, "cur:%d; sendCnt:%d\n",head.curIndex,head.sendCnt);//zzg debug
LEAVECS:
	if(NULL!=fp) fclose(fp);
	pthread_mutex_unlock(&m_mutexTaxiDataSta);
	return result;
}

bool CKTMeterMod::_FlushMeterDataRcd(byte *Data,WORD Len)
{
	pthread_mutex_lock(&m_mutexMeterDataSta);
	PRTMSG(MSG_NOR, "FlushMeterDataRcd\n");
	bool result=false;
	DataHead head;
	int rcdIndex=0;
	int offset=0;
	int rcdCnt=0;
	char szpath[100] = {0};
	sprintf( szpath, "%s", METERDATAPATH );
	FILE *fp =fopen(szpath, "rb+");
	if(NULL==fp)	goto LEAVECS;
	fread(&head,sizeof(head),1,fp);
	rcdIndex=(head.curIndex+head.sendCnt)%TOTALDATACNT;
	fseek(fp,sizeof(head)+rcdIndex*sizeof(MeterData),0);

	if(rcdIndex>=TOTALDATACNT){	rcdIndex=0;}//���ļ�ͷ��ʼ�洢 
	if(rcdIndex==head.curIndex&&head.sendCnt>0) //�����ļ���������
	{ 
		PRTMSG(MSG_NOR, "meterdata file full\n");
		goto LEAVECS;
	}
	fwrite(Data,Len,1,fp);
	rcdCnt++;
	head.sendCnt+=rcdCnt;
	fseek(fp,0,0);
	fwrite(&head,sizeof(head),1,fp);
	result=true;
	PRTMSG(MSG_NOR, "meter--cur:%d; sendCnt:%d\n",head.curIndex,head.sendCnt);//zzg debug
LEAVECS:
	if(NULL!=fp) fclose(fp);
	pthread_mutex_unlock(&m_mutexMeterDataSta);
	return result;
}

// ��Ⲣ�ȴ�����Ӧ��
bool CKTMeterMod::_ChkAck(byte datatype)
{
	byte tmp[2048] = {0};
	ulong len = 0;
	
	int left = 400;		// ���ȴ�40����
	while(left>0) 
	{
		len = tcprcv.pop(tmp);
		if(len>0) 
		{
			byte ret = hdl_cent(tmp[0], tmp+1, len-1);
			if(tmp[0]==datatype && ret==0xff) 	break;
		}
		left--;
		usleep(100000);
	}
	
	PRTMSG(MSG_NOR, "_ChkAck %s\n", (left>0) ? "Succ!" : "Fail!");
	return (left>0) ? true : false;
}	

void CKTMeterMod::_SendMeterData()
{
	PRTMSG(MSG_NOR, "Start to upload Meter data\n");
	byte buf[2048] = {0};
	ulong len = 0;
	int mSndCnt=0;
	PRTMSG(MSG_NOR, "que_meter.getcount:%d\n",que_meter.getcount());
	while( que_meter.getcount()>0&&(len = que_meter.pop(buf)) ) 
	{
		_TcpSend(0x20, 0x68, buf, len);
		if(!_ChkAck(0x28)) 
		{
			que_meter.push(buf, len);
			break;
		}
		
		mSndCnt++;
		if(mSndCnt>=10)//ÿ10�κ���޸ļ�¼����ֹδ���공̨��������ʧ
		{
			if(true==_FlushDataHead(1,mSndCnt)) mSndCnt=0;
		}
	}
	if(mSndCnt>0)_FlushDataHead(1,mSndCnt);
}

void CKTMeterMod::_SendCashData()
{
	PRTMSG(MSG_NOR, "Start to upload cash data\n");
	byte buf[2048] = {0};
	ulong len = 0;
	int tSndCnt=0;
	PRTMSG(MSG_NOR, "que_taxi.getcount:%d\n",que_taxi.getcount());
	while( que_taxi.getcount()>0&&(len = que_taxi.pop(buf)) ) 
	{
		_TcpSend(0x20, 0x27, buf, len);
		if(!_ChkAck(0x67)) 
		{
			que_taxi.push(buf, len);
			break;
		}
		
		tSndCnt++;
		if(tSndCnt>=10)//ÿ10�κ���޸ļ�¼����ֹδ���공̨��������ʧ
		{
			if(true==_FlushDataHead(0,tSndCnt)) tSndCnt=0;
		}
	}
	if(tSndCnt>0)_FlushDataHead(0,tSndCnt);	
}

void CKTMeterMod::Test()
{
	byte data[]={0x00,0x00,0xaa};
	_ComSend(0xAA,data,sizeof(data));
	PRTMSG(MSG_NOR, "com send\n");
}

void CKTMeterMod::BeginMeterRcd()
{
	//�����Ӫ��Ϣ
	m_objUp2068.Init();
	m_wMeterSta=0;
	//��ȡGPS����
	tagGPSData objGps(0);
	g_objMonAlert.GetCurGPS( objGps, true );
	tagQianGps objQianGps;
	g_objMonAlert.GpsToQianGps( objGps, objQianGps );
	//���λ����Ϣ
	memcpy(m_objUp2068.gpsInf.date,(byte*)&objQianGps+1,sizeof(m_objUp2068.gpsInf.date));
	memcpy(m_objUp2068.gpsInf.beginTime,(byte*)&objQianGps+4,sizeof(m_objUp2068.gpsInf.beginTime));
	memcpy(m_objUp2068.gpsInf.beginLat,(byte*)&objQianGps+8,sizeof(m_objUp2068.gpsInf.beginLat));
	memcpy(m_objUp2068.gpsInf.beginLon,(byte*)&objQianGps+12,sizeof(m_objUp2068.gpsInf.beginLon));

	P_PlayTxt("�������ؿ�����");

	//�����ؿ�����������������
}

void CKTMeterMod::SetSerValue(int v_iValue)
{
	PRTMSG(MSG_NOR, "Set SerValue:%d\n",v_iValue);
	if(v_iValue<2||v_iValue>5) v_iValue=3;
	m_objUp2068.serValue=(byte)v_iValue;
	m_wMeterSta|=METERDATA_SERVALUE;
	if(METERDATA_COMPLETE==m_wMeterSta)
	{
		_FlushMeterDataRcd((byte *)&m_objUp2068,sizeof(m_objUp2068));
		m_wMeterSta=0;
		must_up=true;
	}
	//��������Ӧ��
	char buf[3];
	buf[0]=0x01;
	buf[1]=0x4b;
	buf[2]=0x01;
	DataPush(buf, 3, DEV_QIAN, DEV_DIAODU, LV2);
}

//�ؿ�����Ӧ��
int CKTMeterMod::DealComu48( char* v_szData, DWORD v_dwDataLen )
{
	PRTMSG(MSG_NOR, "Rcv Comu48:%d\n",v_szData[0]);
	if(v_szData[0]<1||v_szData[0]>4) m_objUp2068.cnt=1;
	m_objUp2068.cnt=v_szData[0];
	return 0;
}

//��������Ӧ��
int CKTMeterMod::DealComu4A( char* v_szData, DWORD v_dwDataLen )
{
	PRTMSG(MSG_NOR, "Rcv Comu4A:%d\n",v_szData[0]);
	if(v_szData[0]<2||v_szData[0]>5) SetSerValue(3);
	SetSerValue(v_szData[0]);

	P_PlayTxt("��л��������!");
	return 0;
}

#endif
