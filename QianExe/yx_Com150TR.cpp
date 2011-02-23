#include "yx_QianStdAfx.h"

#if USE_COMBUS == 2

#undef MSG_HEAD 
#define MSG_HEAD	("QianExe-Com150TR")

void *G_ThreadCom150TRRecv(void *arg)
{
	g_objCom150TR.P_ThreadCom150TRRecv();
}

void *G_ThreadCom150TRSend(void *arg)
{
	g_objCom150TR.P_ThreadCom150TRSend();
}

//////////////////////////////////////////////////////////////////////////
CCom150TR::CCom150TR()
{
	m_iComPort = 0;
	m_bComConnect = false;

	m_bytIOSignal = 0;
	m_dwCycleCount = 0;
	pthread_mutex_init(&m_mutexIOSignal, NULL);

	memset(m_szComFrameBuf, 0, sizeof(m_szComFrameBuf));
	m_iComFrameLen = 0;
}

CCom150TR::~CCom150TR()
{
	pthread_mutex_destroy(&m_mutexIOSignal);
}

int CCom150TR::Init()
{
	// �򿪴���
	if( !ComOpen() )
	{
		PRTMSG(MSG_ERR, "Open Com failed!\n");
		return ERR_COM;
	}

	// �����շ��߳�
	if( pthread_create(&m_pthreadRecv, NULL, G_ThreadCom150TRRecv, NULL) != 0
		|| pthread_create(&m_pthreadSend, NULL, G_ThreadCom150TRSend, NULL) != 0
		)
	{
		ComClose();
		PRTMSG(MSG_ERR, "Create Thread failed!\n");
		return ERR_THREAD;
	}

	PRTMSG(MSG_DBG, "Create Com150TR thread succ!\n");

	return 0;
}

int CCom150TR::Release()
{
	ComClose();

	return 0;
}

bool CCom150TR::ComOpen()
{
	int iResult;
	struct termios options;

	m_iComPort = open("/dev/ttyAMA0", O_RDWR );//| O_NONBLOCK);
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

	PRTMSG(MSG_DBG,"Com150TR open Succ!\n"); 

	return true;
}

bool CCom150TR::ComClose()
{
	if(m_iComPort != -1)
	{
		close(m_iComPort);
		m_iComPort = -1;

		return true;
	}

	return false;
}

int CCom150TR::ComWrite(char *v_szBuf, int v_iLen)
{
	int iWrited = 0;
	
	iWrited = write(m_iComPort, v_szBuf, v_iLen);

//   	PRTMSG(MSG_DBG, "Com150TR send one frame: ");
//   	PrintString(v_szBuf, v_iLen);
	
	return iWrited;
}

void CCom150TR::P_ThreadCom150TRRecv()
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
//   			PRTMSG(MSG_DBG, "Com150TR recv data: ");
//   			PrintString(szReadBuf, iReadLen);

			AnalyseComFrame(szReadBuf, iReadLen);
		}
	}
}

void CCom150TR::P_ThreadCom150TRSend()
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
		if( !m_objCom150TRReadMng.PopData(bytLvl, dwBufSize, dwRecvLen, szRecvBuf, dwPushTm))
		{
			if( dwRecvLen <= dwBufSize )
				DealCom150TRFrame(szRecvBuf, (int)dwRecvLen);
		}
		
		// ���򴮿ڷ�������
		if( !m_objCom150TRWorkMng.PopData(bytLvl, dwBufSize, dwRecvLen, szRecvBuf, dwPushTm))
		{
			szSendBuf[1] = 1;	// Э��汾���̶�ȡ1
			szSendBuf[2] = 8;	// �����ţ��̶�ȡ8
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

void CCom150TR::AnalyseComFrame(char *v_szBuf, int v_iLen)
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
					iRet = m_objCom150TRReadMng.PushData(LV1, (DWORD)(m_iComFrameLen-4), m_szComFrameBuf+4, dwPacketNum);
					if( 0 != iRet )
					{
						PRTMSG(MSG_ERR, "Push one frame failed:%d\n",iRet);
						memset(m_szComFrameBuf, 0, m_iComFrameLen);
						m_iComFrameLen = 0;
					}
// 					else
// 					{
// 						PRTMSG(MSG_DBG, "m_objCom150TRReadMng push one frame:");
// 						PrintString(m_szComFrameBuf+4, m_iComFrameLen-4);
// 					}
				}
				else
				{
					PRTMSG(MSG_ERR, "Com150TR frame check crc err!\n");
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

int CCom150TR::TranData(char *v_szBuf, int v_iLen)
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

int CCom150TR::DetranData(char *v_szBuf, int v_iLen)
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

void CCom150TR::DealCom150TRFrame(char *v_szBuf, int v_iLen)
{
	char szSendBuf[100] = {0};
	int  iSendLen = 0;
	DWORD dwPacketNum = 0;

	switch( v_szBuf[0] )
	{
	case 0x01:	//���ں�ע������
		{
			szSendBuf[iSendLen++] = 0x81;		// ע��Ӧ��
			szSendBuf[iSendLen++] = 0x01;		// ֧������
			m_objCom150TRWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
			m_bComConnect = true;

			PRTMSG(MSG_DBG, "Recv Com150TR connect!\n");

			// ע��󣬽��и����ڲ�������
			_SetComPara();		
		}
		break;

	case 0x02:	//���ں�����
		{
			if(m_bComConnect == false)
			{
				PRTMSG(MSG_NOR, "Recv Com150TR link, but is not connect, quit!\n");
				break;
			}
			//PRTMSG(MSG_DBG, "Recv Com150TR link!\n");

			szSendBuf[iSendLen++] = 0x82;		// ����Ӧ��
			m_bComConnect = true;
			m_objCom150TRWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
		}
		break;

	case 0x03:	//���ں���������Ӧ��
		{
			// �������Ӻ��ٴν��и����ڲ�������
			_SetComPara();	
		}
		break;

	case 0x11:	//���ڲ������õ�Ӧ��
		{
			if( v_szBuf[1] == 0x01 )	
			{
				PRTMSG(MSG_DBG, "Com150TR set para succ!\n");

				// �����ϵ�
				szSendBuf[iSendLen++] = 0xB3;
				szSendBuf[iSendLen++] = 0x01;
				szSendBuf[iSendLen++] = 0x01;
				m_objCom150TRWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
			}
			else	//������ʧ�ܣ����ٴ�����
			{
				_SetComPara();
			}
		}
		break;
		
	case 0x21:	// ������չ���ڵ�����
		{
			switch( v_szBuf[1] )
			{
			case 0x01:	// ���������ֱ�ʹ����չ����2
				{
					// ����չ��Ӧ��
					szSendBuf[iSendLen++] = 0xA1;
					m_objCom150TRWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
					
					v_szBuf[1] = 0x00;		// 0x00 ��ʾʹ����չ�У��ӵ������յ�������
					DataPush(v_szBuf+1, v_iLen-1, DEV_QIAN, DEV_DIAODU, LV2);
				}
				break;
			}
		}
		break;

	case 0x30:	// �ź�״ֵ̬
		{
			// ����չ��Ӧ��
			szSendBuf[iSendLen++] = 0xB0;
			m_objCom150TRWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);

			// �����ź�״ֵ̬
			pthread_mutex_lock(&m_mutexIOSignal);
			m_bytIOSignal = v_szBuf[2];

			int iPackCnt = v_szBuf[3];
			for(int i=0; i<iPackCnt; i++)
			{
				m_dwCycleCount += 256*v_szBuf[4+i*2] + v_szBuf[4+i*2+1];
			}
			pthread_mutex_unlock(&m_mutexIOSignal);
		}
		break;

	case 0x31:	// �ź�״̬�仯֪ͨ
		{
			// ����չ��Ӧ��
			szSendBuf[iSendLen++] = 0xB1;
			m_objCom150TRWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);

			//	��ʾ�ı���ź�ID�͸ı�״̬
			//	S ID6 ID5 ID4 ID3 ID2 ID1 ID0
			//	ID0~ID6 --��ʾ�ź�ID����0��ʼ�������Ϊ127;
			//	S --��ʾ�źŸı�״̬, 1��ʾ����Ч��Ϊ��Ч,0��ʾ����Ч��Ϊ��Ч;
			//	ID6~ID0��ֵ����:
			//	00H --ǰ����
			//	01H --�ҵ�
			//	02H --���
			//	03H --��ɲ
			//	04H --��ɲ
			//	05H --ǰ���
			//	06H --����
			//	07H --����

			// �����ź�״ֵ̬
			pthread_mutex_lock(&m_mutexIOSignal);
			bool bValid = v_szBuf[1] & 0x80;
			byte bytIOID = v_szBuf[1] | 0x7f;
			switch( bytIOID )
			{
			case 0x01:
				if( bValid )	m_bytIOSignal |= 0x01;
				else			m_bytIOSignal &= ~(0x01);
				break;

			case 0x02:
				if( bValid )	m_bytIOSignal |= 0x02;
				else			m_bytIOSignal &= ~(0x02);
				break;

			case 0x03:
				if( bValid )	m_bytIOSignal |= 0x04;
				else			m_bytIOSignal &= ~(0x04);
				break;

			case 0x04:
				if( bValid )	m_bytIOSignal |= 0x10;
				else			m_bytIOSignal &= ~(0x10);
				break;

			case 0x05:
				if( bValid )	m_bytIOSignal |= 0x20;
				else			m_bytIOSignal &= ~(0x20);
				break;

			case 0x06:
				if( bValid )	m_bytIOSignal |= 0x40;
				else			m_bytIOSignal &= ~(0x40);
				break;

			case 0x07:
				if( bValid )	m_bytIOSignal |= 0x80;
				else			m_bytIOSignal &= ~(0x80);
				break;

			default:
				break;
			}
			
			pthread_mutex_unlock(&m_mutexIOSignal);
		}
		break;

	case 0x50:	// U��״̬֪ͨ
		{
			if( v_szBuf[1] == 0x00 )
			{
				PRTMSG(MSG_NOR, "150TR Udisk insert\n");
			}
			else
			{
				PRTMSG(MSG_NOR, "150TR Udisk Out\n");
			}
		}

	case 0x20:	// ������չ���ڵ����ݽ���ָʾӦ��
	case 0x32:	// �ź��˲�������������Ӧ��
	case 0x40:	// �������ݵ�������Ӧ��
	case 0x41:	// �������ݽ���ָʾӦ��
		break;
		
	default:
		break;
	}
}

void CCom150TR::_SetComPara()
{
	char szSendBuf[100] = {0};
	int iSendLen = 0;
	DWORD dwPacketNum;

	szSendBuf[iSendLen++] = 0x91;	// ����

	szSendBuf[iSendLen++] = 0x01;	// ��չ����ID

	szSendBuf[iSendLen++] = 0x00;	// ������, 9600
	szSendBuf[iSendLen++] = 0x00;
	szSendBuf[iSendLen++] = 0x25;
	szSendBuf[iSendLen++] = 0x80;

	szSendBuf[iSendLen++] = 0x08;	// ����λ8λ

	szSendBuf[iSendLen++] = 0x00;	// 1λֹͣλ

	szSendBuf[iSendLen++] = 0x00;	// ��У��

	szSendBuf[iSendLen++] = 0x00;	// ͨ��ģʽ���շ�

	szSendBuf[iSendLen++] = 0x02;	// ת�������ż���512��
	szSendBuf[iSendLen++] = 0x00;

	szSendBuf[iSendLen++] = 0x00;	// ת��ʱ���ż� 200ms
	szSendBuf[iSendLen++] = 0xC8;

	m_objCom150TRWorkMng.PushData((BYTE)LV1, (DWORD)iSendLen, (char*)szSendBuf, dwPacketNum);
}

byte CCom150TR::GetIOSignal()
{
	byte bytIOSignal;

	pthread_mutex_lock(&m_mutexIOSignal);
	bytIOSignal = m_bytIOSignal;
	pthread_mutex_unlock(&m_mutexIOSignal);

	return bytIOSignal;
}

DWORD CCom150TR::GetCycleCount()
{
	DWORD dwCycleCount;

	pthread_mutex_lock(&m_mutexIOSignal);
	dwCycleCount = m_dwCycleCount;
	pthread_mutex_unlock(&m_mutexIOSignal);
	
	return dwCycleCount;
}

//--------------------------------------------------------------------------------------------------------------------------
// ���У���(ֱ��ȡ�ۼӺ�)
// buf: ���ݻ�����ָ��
// len: ���ݵĳ���
// ����: У���Ƿ���ȷ
bool CCom150TR::check_crc(const byte *buf, const int len)
{   
	byte check_sum = get_crc( buf+1, len-1 );
	return (buf[0] == check_sum);
}

//--------------------------------------------------------------------------------------------------------------------------
// �õ�У��͵ķ���
byte CCom150TR::get_crc(const byte *buf, const int len)
{   
	byte check_sum = 0;
	for(int i=0; i<len; i++) {
		check_sum += *(buf+i);
	}
	check_sum ^= 0xff;
	
    return check_sum;
}

#endif

