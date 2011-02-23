#include "ComuStdAfx.h"

void *G_ThreadComAdjustRead(void *arg)
{
	g_objComAdjust.P_ThreadComAdjustRead();
}

#undef MSG_HEAD
#define MSG_HEAD	("ComuExe-ComAdjust")
//////////////////////////////////////////////////////////////////////////

CComAdjust::CComAdjust()
{
	m_pthreadRead = 0;
}

CComAdjust::~CComAdjust()
{

}

int CComAdjust::Init()
{
#if USE_COMBUS == 0		// ��ʹ�ô�����չ�У����ڴ˴��򿪴���
	if( !ComOpen() )
	{
		PRTMSG(MSG_ERR, "open com failed!\n");
		perror("");
		return ERR_COM;
	}

	// �����������߳�
	if( pthread_create(&m_pthreadRead, NULL, G_ThreadComAdjustRead, NULL) != 0 )
	{
		PRTMSG(MSG_ERR, "Create com read thread failed!\n");
		perror("");
		
		return ERR_THREAD;
	}
#endif

	pthread_mutex_init(&m_MutexWrite, NULL);

	return 0;
}

int CComAdjust::Release()
{
#if USE_COMBUS == 0
	ComClose();		// ���ڹر�֮�󣬶����ں��������ش��󣬶��߳��˳�
#endif

	pthread_mutex_destroy(&m_MutexWrite);

	return 0;
}

bool CComAdjust::ComOpen()
{
	int iResult;
	int i;
	struct termios options;
	const int COMOPEN_TRYMAXTIMES = 5;
	
	for( i = 0; i < COMOPEN_TRYMAXTIMES; i ++ )
	{
		m_iComPort = open("/dev/ttySIM0", O_RDWR );
		//m_iComPort = open("/dev/ttyAMA0", O_RDWR );

		if( -1 != m_iComPort )
		{
			PRTMSG(MSG_ERR,"Open com succ!\n");
			break;
		}
		
		sleep(1);
	}

	if( -1 == m_iComPort )
	{
		PRTMSG(MSG_ERR,"Open com fail!\n");
		return false;
	}
	
	if(tcgetattr(m_iComPort, &options) != 0)
	{
		perror("GetSerialAttr");
		return false;
	}
	
	options.c_iflag &= ~(IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL|IUCLC|IXON|IXOFF|IXANY); 
	options.c_lflag &= ~(ECHO|ECHONL|ISIG|IEXTEN|ICANON);
	options.c_oflag &= ~OPOST;
	
	cfsetispeed(&options,B9600);	//���ò����ʣ����������ò�����Ϊ9600
	cfsetospeed(&options,B9600);
	
	if (tcsetattr(m_iComPort,TCSANOW,&options) != 0)   
	{ 
		perror("Set com Attr"); 
		return false;
	}
	
	return true;
}

int CComAdjust::ComClose()
{
	if(-1 != m_iComPort)	
	{
		close(m_iComPort);
		m_iComPort = -1;
	}
}

int CComAdjust::ComWrite(char *v_szBuf, int v_iLen)
{
#if USE_COMBUS == 1 || USE_COMBUS == 2
	DataPush(v_szBuf, (DWORD)v_iLen, DEV_DIAODU, DEV_QIAN, LV2);
	return v_iLen;
#endif
	
	pthread_mutex_lock(&m_MutexWrite);

// 	if( 0xa0 == v_szBuf[2] )
// 	{
// 		PRTMSG(MSG_DBG, "com send:");
//    		PrintString(v_szBuf, v_iLen);
// 	}

	int iWrited = 0;	// ���ڷ��������ݳ���	
	iWrited = write(m_iComPort, v_szBuf, v_iLen);

	pthread_mutex_unlock(&m_MutexWrite);

	return iWrited;
}

void CComAdjust::P_ThreadComAdjustRead()
{
	char szReadBuf[1024] = {0};
	int  iBufLen = sizeof(szReadBuf);
	int  iReadLen = 0;
	int  i = 0;
	byte bytLvl = 0;
	DWORD dwPushTm;
	
	while(!g_bProgExit) 
	{
		iReadLen = read(m_iComPort, szReadBuf, iBufLen);
		
//   		PRTMSG(MSG_DBG, "com recv:");
//  		PrintString(szReadBuf, iReadLen);
		
		if(iReadLen > iBufLen)
		{
			PRTMSG(MSG_ERR,"Com Read Over Buf!\n");
			memset(szReadBuf, 0, sizeof(szReadBuf));
			iReadLen = 0;
			
			continue;
		}		
		
		if( iReadLen > 0 )
		{
			AnalyseComFrame(szReadBuf, iReadLen);
		}
	}
}

void CComAdjust::AnalyseComFrame(char *v_szBuf, int v_iLen)
{
	int iRet = 0;
	if( !v_szBuf || v_iLen <= 0 ) 
		return ;

	DWORD dwPacketNum = 0;	
	static int i7ECount = 0;
	static int iFFCount = 0;
	
	for( int i = 0; i < v_iLen; i ++ )
	{
		if( 0x7e == v_szBuf[ i ] )
		{
			++ i7ECount;
			
			if( 0 == i7ECount % 2 ) // ���õ�һ֡������������
			{
				if( 0 != m_iComFrameLen)
				{
					i7ECount = 0;
				
					// ��ת��
					m_iComFrameLen = DetranData(m_szComFrameBuf, m_iComFrameLen);
					
					// ����У��ͣ�У����ȷ��������ն���
					if( check_crc((byte*)m_szComFrameBuf, m_iComFrameLen) )
					{						
						g_iTerminalType = 2;
						iRet = g_objDiaodu.m_objDiaoduReadMng.PushData(LV1, (DWORD)(m_iComFrameLen-1), m_szComFrameBuf+1, dwPacketNum);
						if( 0 != iRet )
						{
							PRTMSG(MSG_ERR, "Push one frame failed:%d\n",iRet);
							memset(m_szComFrameBuf, 0, m_iComFrameLen);
							m_iComFrameLen = 0;
						}
// 						else
// 						{
// 							PRTMSG(MSG_DBG, "push one frame:");
// 							PrintString(m_szComFrameBuf, m_iComFrameLen);
// 						}
					}
					else
					{
						PRTMSG(MSG_ERR, "Diaodu frame check crc err: ");
						PrintString(m_szComFrameBuf, m_iComFrameLen);
						memset(m_szComFrameBuf, 0, m_iComFrameLen);
						m_iComFrameLen = 0;
					}
				}	
					
				//����У����ȷ��񣬶�����������0
				memset(m_szComFrameBuf, 0, m_iComFrameLen);
				m_iComFrameLen = 0;
			}
		}
		else if( 0xFF == v_szBuf[i] && 0 == i7ECount%2 )
		{
			++iFFCount;

			if( iFFCount%2 == 0 )	// �յ�һ֡�ֱ�������
			{
				if( 0 != m_iComFrameLen)
				{
					iFFCount = 0;

					g_iTerminalType = 1;

					// �Ƶ��ֱ��Ľ��ն���
					iRet = g_hst.m_objHandleReadMng.PushData(LV1, (DWORD)m_iComFrameLen, m_szComFrameBuf, dwPacketNum);

					if( 0 != iRet )
					{
						PRTMSG(MSG_ERR, "Push one frame failed:%d\n",iRet);
						memset(m_szComFrameBuf, 0, m_iComFrameLen);
						m_iComFrameLen = 0;
					}
// 					else
// 					{
// 						PRTMSG(MSG_DBG, "push one frame:");
// 						PrintString(m_szComFrameBuf, m_iComFrameLen);
// 					}
				}

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

int CComAdjust::DetranData(char *v_szBuf, int v_iLen)
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

//--------------------------------------------------------------------------------------------------------------------------
// ���У���(ֱ��ȡ�ۼӺ�)
// buf: ���ݻ�����ָ��
// len: ���ݵĳ���
// ����: У���Ƿ���ȷ
bool CComAdjust::check_crc(const byte *buf, const int len)
{   
	byte check_sum = get_crc( buf+1, len-1 );
	return (buf[0]==check_sum);
}

//--------------------------------------------------------------------------------------------------------------------------
// �õ�У���(ֱ��ȡ�ۼӺ�)
// buf: ���ݻ�����ָ��
// len: ���ݵĳ���
// ����: ���ݵ�У���
byte CComAdjust::get_crc(const byte *buf, const int len)
{   
	byte check_sum = 0;
	for(int i=0; i<len; i++) {
		check_sum += *(buf+i);
	}
    return check_sum;
}



