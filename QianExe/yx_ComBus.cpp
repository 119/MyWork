#include "yx_QianStdAfx.h"

#if USE_COMBUS == 1

#undef MSG_HEAD 
#define MSG_HEAD	("QianExe-ComBus")

void *G_ThreadComBusRecv(void *arg)
{
	g_objComBus.P_ThreadComBusRecv();
}

void *G_ThreadComBusSend(void *arg)
{
	g_objComBus.P_ThreadComBusSend();
}

//////////////////////////////////////////////////////////////////////////
CComBus::CComBus()
{
	m_iComPort = 0;
	m_bComConnect = false;
}

CComBus::~CComBus()
{

}

int CComBus::Init()
{
	// �򿪴���
	if( !ComOpen() )
	{
		PRTMSG(MSG_ERR, "Open Com failed!\n");
		return ERR_COM;
	}

	// �����շ��߳�
	if( pthread_create(&m_pthreadRecv, NULL, G_ThreadComBusRecv, NULL) != 0
		|| pthread_create(&m_pthreadSend, NULL, G_ThreadComBusSend, NULL) != 0
		)
	{
		ComClose();
		PRTMSG(MSG_ERR, "Create Thread failed!\n");
		return ERR_THREAD;
	}

	PRTMSG(MSG_DBG, "Create ComBus thread succ!\n");

	return 0;
}

int CComBus::Release()
{
	ComClose();

	return 0;
}

bool CComBus::ComOpen()
{
	int iResult;
	struct termios options;

	m_iComPort = open("/dev/ttyAMA0", O_RDWR );

	if( -1 == m_iComPort )
	{
		PRTMSG(MSG_ERR, "Open AMA0 failed!\n");
		perror("");
		return false;
	}
	
	if(tcgetattr(m_iComPort, &options) != 0)
	{
		PRTMSG(MSG_ERR, "GetSerialAttr\n");
		perror("");
		return false;
	}

	options.c_iflag &= ~(IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL|IUCLC|IXON|IXOFF|IXANY); 
	options.c_lflag &= ~(ECHO|ECHONL|ISIG|IEXTEN|ICANON);
	options.c_oflag &= ~OPOST;
	
	cfsetispeed(&options,B9600);	//��չ�в�����9600
	cfsetospeed(&options,B9600);

	if(tcsetattr(m_iComPort, TCSANOW, &options) != 0)   
	{ 
		PRTMSG(MSG_ERR, "Set com Attr\n"); 
		perror("");
		return false;
	}

	PRTMSG(MSG_DBG,"ComBus open Succ!\n"); 

	return true;
}

bool CComBus::ComClose()
{
	if(m_iComPort != -1)
	{
		close(m_iComPort);
		m_iComPort = -1;

		return true;
	}

	return false;
}

int CComBus::ComWrite(char *v_szBuf, int v_iLen)
{
	int iWrited = 0;
	
	iWrited = write(m_iComPort, v_szBuf, v_iLen);

//	PRTMSG(MSG_DBG, "ComBus send one frame!");
//	PrintString(v_szBuf, v_iLen);
	
	return iWrited;
}

void CComBus::P_ThreadComBusRecv()
{
	char szReadBuf[1024] = {0};
	int  iBufLen = sizeof(szReadBuf);
	int  iReadLen = 0;
	int  i = 0;
	
	while(!g_bProgExit) 
	{
		iReadLen = read(m_iComPort, szReadBuf, iBufLen);
		
		if(iReadLen > iBufLen)
		{
			PRTMSG(MSG_ERR,"Com Read Over Buf!\n");
			memset(szReadBuf, 0, sizeof(szReadBuf));
			iReadLen = 0;
			
			continue;
		}		
		
		if( iReadLen > 0 )
		{
//			PRTMSG(MSG_DBG, "ComBus recv data!\n");
//			PrintString(szReadBuf, iReadLen);
			AnalyseComFrame(szReadBuf, iReadLen);
		}
	}
}

void CComBus::P_ThreadComBusSend()
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
		dwRecvLen = 0;
		memset(szRecvBuf, 0, sizeof(szRecvBuf));
		
		// �ȴ����ڽ��յ�������
		if( !m_objComBusReadMng.PopData(bytLvl, dwBufSize, dwRecvLen, szRecvBuf, dwPushTm))
		{
			if( dwRecvLen <= dwBufSize )
				DealComBusFrame(szRecvBuf, (int)dwRecvLen);
		}
		
		// ���򴮿ڷ�������
		if( !m_objComBusWorkMng.PopData(bytLvl, dwBufSize, dwRecvLen, szRecvBuf, dwPushTm))
		{
			szSendBuf[1] = 2;	// Э��汾���̶�ȡ2
			szSendBuf[2] = 3;	// �����ţ��̶�ȡ3
			szSendBuf[3] = 0;	// ������ţ��̶�ȡ0
			memcpy(szSendBuf+4, szRecvBuf, dwRecvLen);
			szSendBuf[0] = get_crc((byte*)szSendBuf+1, 3+dwRecvLen);
			iSendLen = 4+dwRecvLen;

			// ת��
			iSendLen = TranData(szSendBuf, iSendLen);
		
 			// ��������֡		
 			ComWrite(szSendBuf, iSendLen);
		}

		usleep(50000);
	}
}

void CComBus::AnalyseComFrame(char *v_szBuf, int v_iLen)
{
	int iRet = 0;
	if( !v_szBuf || v_iLen <= 0 ) 
		return ;
	
	static int i7ECount = 0;
	
	for( int i = 0; i < v_iLen; i ++ )
	{
		if( 0x7e == v_szBuf[ i ] )
		{
			++ i7ECount;
			
			if( 0 == i7ECount % 2 ) // ���õ�һ֡
			{		
				// ��ת��
				m_iComFrameLen = DetranData(m_szComFrameBuf, m_iComFrameLen);

				// ����У��ͣ�У����ȷ��������ն���
				if( check_crc((byte*)m_szComFrameBuf, m_iComFrameLen) )
				{				
					// ������ն���ʱȥ��У���롢Э��汾�������ţ��������
					DWORD dwPacketNum = 0;
					iRet = m_objComBusReadMng.PushData(LV1, (DWORD)(m_iComFrameLen-4), m_szComFrameBuf+4, dwPacketNum);
					if( 0 != iRet )
					{
						PRTMSG(MSG_ERR, "Push one frame failed:%d\n",iRet);
						memset(m_szComFrameBuf, 0, m_iComFrameLen);
						m_iComFrameLen = 0;
					}					
				}
				else
				{
					PRTMSG(MSG_ERR, "ComBus frame check crc err!\n");
					memset(m_szComFrameBuf, 0, m_iComFrameLen);
					m_iComFrameLen = 0;
				}
				
				//����У����ȷ��񣬶�����������0
				memset(m_szComFrameBuf, 0, m_iComFrameLen);
				m_iComFrameLen = 0;
			}
		}
		else
		{
			m_szComFrameBuf[m_iComFrameLen++] = v_szBuf[i];
		}
	}
}

int CComBus::TranData(char *v_szBuf, int v_iLen)
{
	char szNewBuf[1024] = {0};
	int  iNewLen = 0;

	szNewBuf[iNewLen++] = 0x7e;

	for( int i=0; i<v_iLen; i++) 
	{
		switch(v_szBuf[i])
		{
		case 0x7e:
			szNewBuf[iNewLen++] = 0x7d; 
			szNewBuf[iNewLen++] = 0x02;  
			break;

		case 0x7d:
			szNewBuf[iNewLen++] = 0x7d; 
			szNewBuf[iNewLen++] = 0x01; 
			break;

		default:
			szNewBuf[iNewLen++]= v_szBuf[i]; 
			break;
		}
	}

	szNewBuf[iNewLen++] = 0x7e;

	memcpy(v_szBuf, szNewBuf, iNewLen);
	return iNewLen;
}

int CComBus::DetranData(char *v_szBuf, int v_iLen)
{
	char szbuf[1024];
	int  iNewLen = 0;
	
	for( int i = 0; i < v_iLen ; )
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

void CComBus::DealComBusFrame(char *v_szBuf, int v_iLen)
{
	char szSendBuf[100] = {0};
	int  iSendLen = 0;
	DWORD dwPacketNum = 0;

	switch( v_szBuf[0] )
	{
	case 0x01:	//���ں�ע������
		{
			szSendBuf[iSendLen++] = 0x81;		// ע��Ӧ��
			m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
			m_bComConnect = true;

			PRTMSG(MSG_DBG, "Recv ComBus connect!\n");

			// ע��󣬽��и����ڲ�������
			_SetComPara();		
		}
		break;

	case 0x02:	//���ں�����
		{
			if(m_bComConnect == false)
			{
				PRTMSG(MSG_NOR, "Recv ComBus link, but is not connect, quit!\n");
				break;
			}
			//PRTMSG(MSG_DBG, "Recv ComBus link!\n");

			szSendBuf[iSendLen++] = 0x82;		// ����Ӧ��
			m_bComConnect = true;
			m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
		}
		break;

	case 0x03:	//���ں���������Ӧ��
		{
			// �������Ӻ��ٴν��и����ڲ�������
			_SetComPara();	
		}
		break;

	case 0x43:	//���ڲ������õ�Ӧ��
		{
			if( v_szBuf[1] == 0x00 )	//������ʧ�ܣ����ٴ�����
				_SetComPara();
			else
				PRTMSG(MSG_DBG, "ComBus set para succ!\n");
		}
		break;
		
	case 0x70:	//������2���Ƽ�����
		{
			// Ӧ��
			szSendBuf[iSendLen++] = 0xf0;
			m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
#if USE_METERTYPE == 1			
			g_objMeter.DealComBuf(v_szBuf+1, v_iLen-1);
#endif
		}
		break;
		
	case 0x72:	//������3����������
		{
			// Ӧ��
			szSendBuf[iSendLen++] = 0xf2;
			m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
			
			v_szBuf[0] = 0x00;		// 0x00 ��ʾʹ����չ�У��ӵ������յ�������
			DataPush(v_szBuf, v_iLen, DEV_QIAN, DEV_DIAODU, LV2);
		}
		break;
		
	case 0x74:
		break;
	
	case 0x76:	//ģ�⴮��2��LED���ƣ�
		{
			// Ӧ��
			szSendBuf[iSendLen++] = 0xf7;
			m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);

#if USE_LEDTYPE == 1
			g_objLedBohai.DealComData_V1_2(v_szBuf+1, v_iLen-1);
#endif

#if USE_LEDTYPE == 2
			if( v_iLen - 1 == 1)
				g_objLedChuangLi.m_objReadMng.PushData((BYTE)LV1, 1, (char*)v_szBuf+1, dwPacketNum);
#endif
		}
		break;

	case 0x78:
	case 0x7a:
	case 0x71://��չ��������2�յ�����Ӧ��	
	case 0x73://��չ��������3�յ�����Ӧ��
	case 0x75://��չ��ģ�⴮��1�յ�����Ӧ��	
	case 0x79://��չ��ģ�⴮��3�յ�����Ӧ��
	default:
		break;
	}
}

void CComBus::_SetComPara()
{
	char szSendBuf[100] = {0};
	int iSendLen = 0;
	DWORD dwPacketNum;

	szSendBuf[iSendLen++] = 0xC3;	// ����
	szSendBuf[iSendLen++] = 0x1f;	// ����������3��ģ�⴮��2��ģ�⴮��1

	szSendBuf[iSendLen++] = 0x02;	// ������2������, 2400
	szSendBuf[iSendLen++] = 0x01;	// ������2����λ��8λ
	szSendBuf[iSendLen++] = 0x01;	// ������2ֹͣλ��1λ
	szSendBuf[iSendLen++] = 0x01;	// ������2У��λ����У��
	
	szSendBuf[iSendLen++] = 0x04;	// ������3������, 9600
	szSendBuf[iSendLen++] = 0x01;	// ������3����λ��8λ
	szSendBuf[iSendLen++] = 0x01;	// ������3ֹͣλ��1λ
	szSendBuf[iSendLen++] = 0x01;	// ������3У��λ����У��

	szSendBuf[iSendLen++] = 0x02;	// ģ�⴮��1������, 2400
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��1����λ��8λ
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��1ֹͣλ��1λ
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��1У��λ����У��

	szSendBuf[iSendLen++] = 0x04;	// ģ�⴮��2������, 9600
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��2����λ��8λ
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��2ֹͣλ��1λ
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��2У��λ����У��
	
	szSendBuf[iSendLen++] = 0x02;	// ģ�⴮��3������, 2400
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��3����λ��8λ
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��3ֹͣλ��1λ
	szSendBuf[iSendLen++] = 0x01;	// ģ�⴮��3У��λ����У��

	m_objComBusWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
}

//--------------------------------------------------------------------------------------------------------------------------
// ���У���(ֱ��ȡ�ۼӺ�)
// buf: ���ݻ�����ָ��
// len: ���ݵĳ���
// ����: У���Ƿ���ȷ
bool CComBus::check_crc(const byte *buf, const int len)
{   
	byte check_sum = get_crc( buf+1, len-1 );
	return (buf[0] == check_sum);
}

//--------------------------------------------------------------------------------------------------------------------------
// �õ�У��͵ķ���
byte CComBus::get_crc(const byte *buf, const int len)
{   
	byte check_sum = 0;
	for(int i=0; i<len; i++) {
		check_sum += *(buf+i);
	}
	check_sum ^= 0xff;
	
    return check_sum;
}

#endif

