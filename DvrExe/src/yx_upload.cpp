#include "yx_common.h"

DataUploadSta	g_objDataUploadSta[8];
tag1PComuCfg g_objComuCfg;

int ALONE_VIDEO_LEN;
int MIXAV_VIDEO_LEN;
int AUDIO_LEN;

void *G_DataUpload(void*)
{
	if (NETWORK_TYPE == NETWORK_EVDO || NETWORK_TYPE == NETWORK_WCDMA)
	{
		ALONE_VIDEO_LEN = 1200;
		MIXAV_VIDEO_LEN = 960;
		AUDIO_LEN = 240;
	}
	else if (NETWORK_TYPE == NETWORK_TD)
	{
		ALONE_VIDEO_LEN = 600;
		MIXAV_VIDEO_LEN = 400;
		AUDIO_LEN = 200;
	}

	int 	i = 0, j = 0, k = 0;
	BYTE	bytFlag = 0x1f;
	
	BYTE	bytLvl = 0;
	DWORD	dwPushTm = 0;
	char	szBuf[100*1024] = {0};
	DWORD	dwLen = 0;
	int		iPacketCnt = 0;

	//BYTE	m_videobuf[ALONE_VIDEO_LEN] = {0};
	BYTE	m_videobuf[1200] = {0};
	BYTE	m_audiobuf[2048] = {0};
	int		m_videolen = 0;
	int		m_audiolen = 0;
	
	BYTE	m_tempbuf[1500] = {0};
	BYTE 	m_sendbuf[1500] = {0};
	uint	m_sendlen = 0;
	int		m_templen = 0;
	
	HI_U64	m_WinNo[5] = {0};
	BYTE	m_PackNo[5] = {0};
	HI_U64	m_PackNum[5] = {0};

	int iCurDateTime[6];

	memset( (void*)g_objDataUploadSta, 0, 4*sizeof(DataUploadSta) );

	for(k=0; k<8; k++)
		for(i=0; i<32; i++)
			g_objDataUploadSta[k].m_objDataWin[i].m_ulWinNo = MAX_UNSIGNED_64;

	
	GetImpCfg( &g_objComuCfg, sizeof(g_objComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(g_objComuCfg) );

	//while(g_objWorkStop.AVUpload == FALSE)
	while( !g_bProgExit )
	{
		RenewMemInfoHalfMin(2);
		ClearThreadDog(2);
		
		msleep(20);
		
		for(i=0; i<4; i++)
		{
			// ���ϴ���Ƶ����
			if( !g_objVideoDataMng[i].PopData(bytLvl, (DWORD)sizeof(szBuf), dwLen, (char*)szBuf, dwPushTm) )
			{
				iPacketCnt = (dwLen-1) / ALONE_VIDEO_LEN;
				if( (dwLen-1) - iPacketCnt*ALONE_VIDEO_LEN )	iPacketCnt++;		//������ALONE_VIDEO_LEN����Ҫ�ֳɶ������

				for(j=0; j<iPacketCnt; j++)
				{
					if( j < iPacketCnt-1 )
					{
						memcpy(m_videobuf, (szBuf+1)+j*ALONE_VIDEO_LEN, ALONE_VIDEO_LEN);
						m_videolen = ALONE_VIDEO_LEN;
					}
					else
					{
						memcpy(m_videobuf, (szBuf+1)+j*ALONE_VIDEO_LEN, (dwLen-1) - j*ALONE_VIDEO_LEN);
						m_videolen = (dwLen-1) - j*ALONE_VIDEO_LEN;
					}

					m_audiolen = 0;
						
					m_PackNum[i]++;
					
					// ���¿�ʼ���
					m_templen = 0;
					
					// ��������
					m_tempbuf[m_templen++] = 0x33;
					
					// �ֻ���
					memcpy(m_tempbuf+m_templen, g_objComuCfg.m_szTel, 11);
					m_templen += 11;
					m_tempbuf[m_templen++] = 0x20;
					m_tempbuf[m_templen++] = 0x20;
					m_tempbuf[m_templen++] = 0x20;
					m_tempbuf[m_templen++] = 0x20;
					
					// ͨ����
					m_tempbuf[m_templen++] = (BYTE)i;
					
					// ʱ����������0��
					m_tempbuf[m_templen++] = 0;
					m_tempbuf[m_templen++] = 0;
					
					// ���ڴ�С
					m_tempbuf[m_templen++] = UPLOAD_WIN_SIZE;
					
					// �������
					m_WinNo[i] = m_PackNum[i]/UPLOAD_WIN_SIZE;
					memcpy(m_tempbuf+m_templen, (void*)&m_WinNo[i], 8);
					m_templen += 8;
					
					// �����
					m_PackNo[i] = (BYTE)(m_PackNum[i]%UPLOAD_WIN_SIZE);
					m_tempbuf[m_templen++] = m_PackNo[i];
					
					// ��־λ
					m_tempbuf[m_templen++] = bytFlag;
					
					if( bytFlag )
					{
						// ʱ��
						GetCurDateTime( iCurDateTime );
						
						m_tempbuf[m_templen++] = (byte)(iCurDateTime[0]-2000);
						m_tempbuf[m_templen++] = (byte)iCurDateTime[1];
						m_tempbuf[m_templen++] = (byte)iCurDateTime[2];
						m_tempbuf[m_templen++] = (byte)iCurDateTime[3];
						m_tempbuf[m_templen++] = (byte)iCurDateTime[4];
						m_tempbuf[m_templen++] = (byte)iCurDateTime[5];
						
						// �¼�����
						m_tempbuf[m_templen++] = 0;
						
						// ��׼GPS����
						memcpy( m_tempbuf+m_templen, &g_objQianGps+4, 14);
						m_templen += 14;
						
						// �ֱ���
						m_tempbuf[m_templen++] = (BYTE)g_objMvrCfg.AVUpload.VMode;
						
						// ֡��
						m_tempbuf[m_templen++] = (BYTE)g_objMvrCfg.AVUpload.VFramerate[i];
					}
					
					// ��Ƶ�������ݳ���
					m_tempbuf[m_templen++] = m_videolen/256;
					m_tempbuf[m_templen++] = m_videolen%256;
					
					// ��Ƶ����
					memcpy(m_tempbuf+m_templen, m_videobuf, m_videolen);
					m_templen += m_videolen;
					
					// ��Ƶ�������ݳ���
					m_tempbuf[m_templen++] = m_audiolen/256;
					m_tempbuf[m_templen++] = m_audiolen%256;
					
					// ��Ƶ����
					memcpy(m_tempbuf+m_templen, m_audiobuf, m_audiolen);
					m_templen += m_audiolen;
					
					// �������ݰ����浽���ͻ����У��Ա��ش�
					g_objDataUploadSta[i].m_objDataWin[ g_objDataUploadSta[i].m_iCurWinNo ].m_ulWinNo = m_WinNo[i];
					g_objDataUploadSta[i].m_objDataWin[ g_objDataUploadSta[i].m_iCurWinNo ].m_bISlice[m_PackNo[i]] = (szBuf[0] ? true : false);
					g_objDataUploadSta[i].m_objDataWin[ g_objDataUploadSta[i].m_iCurWinNo ].m_iBufLen[m_PackNo[i]] = m_templen;
					memcpy( g_objDataUploadSta[i].m_objDataWin[ g_objDataUploadSta[i].m_iCurWinNo ].m_uszDataBuf[m_PackNo[i]], m_tempbuf, m_templen);
					if( UPLOAD_WIN_SIZE == (m_PackNo[i]+1) )
					{
						g_objDataUploadSta[i].m_iCurWinNo++;
						if( 32 <= g_objDataUploadSta[i].m_iCurWinNo )
							g_objDataUploadSta[i].m_iCurWinNo = 0;
					}
					
					// �Ƶ�SockServExe����
					DataPush(m_tempbuf, (DWORD)m_templen, DEV_DVR, DEV_SOCK, LV2);
				}
			}
			
			// Ȼ�����ϴ���Ƶ����
			if( g_objAVUplBuf.AUpload && !g_objAudioDataMng[i].PopData(bytLvl, (DWORD)sizeof(szBuf), dwLen, (char*)szBuf, dwPushTm) )
			{
				iPacketCnt = dwLen / ALONE_VIDEO_LEN;
				if( dwLen - iPacketCnt*ALONE_VIDEO_LEN )	iPacketCnt++;		//������ALONE_VIDEO_LEN����Ҫ�ֳɶ������
				
				for(j=0; j<iPacketCnt; j++)
				{
					if( j < iPacketCnt-1 )
					{
						memcpy(m_audiobuf, szBuf+j*ALONE_VIDEO_LEN, ALONE_VIDEO_LEN);
						m_audiolen = ALONE_VIDEO_LEN;
					}
					else
					{
						memcpy(m_audiobuf, szBuf+j*ALONE_VIDEO_LEN, dwLen - j*ALONE_VIDEO_LEN);
						m_audiolen = dwLen - j*ALONE_VIDEO_LEN;
					}

					m_videolen = 0;
				
					m_PackNum[4+i]++;
					
					// ���¿�ʼ���
					m_templen = 0;
					
					// ��������
					m_tempbuf[m_templen++] = 0x33;
					
					// �ֻ���
					memcpy(m_tempbuf+m_templen, g_objComuCfg.m_szTel, 11);
					m_templen += 11;
					m_tempbuf[m_templen++] = 0x20;
					m_tempbuf[m_templen++] = 0x20;
					m_tempbuf[m_templen++] = 0x20;
					m_tempbuf[m_templen++] = 0x20;
					
					// ͨ����
					m_tempbuf[m_templen++] = (BYTE)(0x40+i);
					
					// ʱ��������0
					m_tempbuf[m_templen++] = 0;
					m_tempbuf[m_templen++] = 0;
					
					// ���ڴ�С
					m_tempbuf[m_templen++] = UPLOAD_WIN_SIZE;
					
					// �������
					m_WinNo[4+i] = m_PackNum[4+i]/UPLOAD_WIN_SIZE;
					memcpy(m_tempbuf+m_templen, (void*)&m_WinNo[4+i], 8);
					m_templen += 8;
					
					// �����
					m_PackNo[4+i] = (BYTE)(m_PackNum[4+i]%UPLOAD_WIN_SIZE);
					m_tempbuf[m_templen++] = m_PackNo[4+i];
					
					// ��־λ
					m_tempbuf[m_templen++] = bytFlag;
					
					if( bytFlag )
					{
						// ʱ��
						GetCurDateTime( iCurDateTime );
						
						m_tempbuf[m_templen++] = (byte)(iCurDateTime[0]-2000);
						m_tempbuf[m_templen++] = (byte)iCurDateTime[1];
						m_tempbuf[m_templen++] = (byte)iCurDateTime[2];
						m_tempbuf[m_templen++] = (byte)iCurDateTime[3];
						m_tempbuf[m_templen++] = (byte)iCurDateTime[4];
						m_tempbuf[m_templen++] = (byte)iCurDateTime[5];
						
						// �¼�����
						m_tempbuf[m_templen++] = 0;
						
						// ��׼GPS����
						memcpy( m_tempbuf+m_templen, &g_objQianGps+4, 14);
						m_templen += 14;
						
						// �ֱ���
						m_tempbuf[m_templen++] = (BYTE)g_objMvrCfg.AVUpload.VMode;
						
						// ֡��
						m_tempbuf[m_templen++] = (BYTE)g_objMvrCfg.AVUpload.VFramerate[i];
					}
					
					// ��Ƶ�������ݳ���
					m_tempbuf[m_templen++] = m_videolen/256;
					m_tempbuf[m_templen++] = m_videolen%256;
					
					// ��Ƶ����
					memcpy(m_tempbuf+m_templen, m_videobuf, m_videolen);
					m_templen += m_videolen;
					
					// ��Ƶ�������ݳ���
					m_tempbuf[m_templen++] = m_audiolen/256;
					m_tempbuf[m_templen++] = m_audiolen%256;
					
					// ��Ƶ����
					memcpy(m_tempbuf+m_templen, m_audiobuf, m_audiolen);
					m_templen += m_audiolen;
					
					// �������ݰ����浽���ͻ����У��Ա��ش�
					g_objDataUploadSta[4+i].m_objDataWin[ g_objDataUploadSta[4+i].m_iCurWinNo ].m_ulWinNo = m_WinNo[4+i];
					g_objDataUploadSta[4+i].m_objDataWin[ g_objDataUploadSta[4+i].m_iCurWinNo ].m_bISlice[m_PackNo[4+i]] = false;
					g_objDataUploadSta[4+i].m_objDataWin[ g_objDataUploadSta[4+i].m_iCurWinNo ].m_iBufLen[m_PackNo[4+i]] = m_templen;
					memcpy( g_objDataUploadSta[4+i].m_objDataWin[ g_objDataUploadSta[4+i].m_iCurWinNo ].m_uszDataBuf[m_PackNo[4+i]], m_tempbuf, m_templen);
					if( UPLOAD_WIN_SIZE == (m_PackNo[4+i]+1) )
					{
						g_objDataUploadSta[4+i].m_iCurWinNo++;
						if( 32 <= g_objDataUploadSta[4+i].m_iCurWinNo )
							g_objDataUploadSta[4+i].m_iCurWinNo = 0;
					}
					
					// �Ƶ�SockServExe����
					DataPush(m_tempbuf, (DWORD)m_templen, DEV_DVR, DEV_SOCK, LV2);
				}
			}
		}
	}	
	
	pthread_exit(NULL);
	return NULL;
}

//ͨ���ţ�1�� �� ������ţ�8���� ����״̬��1��
int	Deal3835(char *v_databuf, int v_datalen)
{
	int		i = 0, j = 0;
	BYTE	uszBitFlag = 0;
	BYTE	bytChn = 0;
	BYTE	uszWinSta = 0;
	HI_U64	ulWinNo = 0;

	bytChn = v_databuf[0];
	memcpy( (void*)&ulWinNo, v_databuf+1, 8);
	uszWinSta = v_databuf[9];

	PRTMSG(MSG_DBG, "ReUpload Chn%d the %d win, ", bytChn, ulWinNo);
	printf("WinSta = %02x\n", uszWinSta);

	// �������ݰ���Ҫ�ش�
	if( uszWinSta != 0xFF )
	{
		for(byte k=0; k<4; k++)
		{
			if( k != bytChn )
				continue;

			// �ҵ����ͻ����ڵĶ�Ӧ���ݴ���
			for(i=0; i<32; i++)
			{
				if( ulWinNo == g_objDataUploadSta[k].m_objDataWin[i].m_ulWinNo )
				{
					uszBitFlag = 0x01;
					
					for(j=0; uszBitFlag != 0; uszBitFlag = uszBitFlag << 1,j++)
					{
						if (NETWORK_TYPE == NETWORK_EVDO || NETWORK_TYPE == NETWORK_WCDMA)
						{
							// ��������Ҫ�ش�
							if( !(uszBitFlag & uszWinSta) )
							{
								if( !DataPush(g_objDataUploadSta[k].m_objDataWin[i].m_uszDataBuf[j], (DWORD)g_objDataUploadSta[k].m_objDataWin[i].m_iBufLen[j], DEV_DVR, DEV_SOCK, LV3) )
								{
									PRTMSG(MSG_DBG, "ReUpload %d win ", ulWinNo);
									PRTMSG(MSG_DBG, "the %d pack, len=%d\n", j, (DWORD)g_objDataUploadSta[k].m_objDataWin[i].m_iBufLen[j] );
									//msleep(5);
								}
							}
						}
						else if (NETWORK_TYPE == NETWORK_TD)
						{
							// ��������Ҫ�ش�
							if( !(uszBitFlag & uszWinSta) && g_objDataUploadSta[k].m_objDataWin[i].m_bISlice[j] == true)
							{
								if( !DataPush(g_objDataUploadSta[k].m_objDataWin[i].m_uszDataBuf[j], (DWORD)g_objDataUploadSta[k].m_objDataWin[i].m_iBufLen[j], DEV_DVR, DEV_SOCK, LV3) )
								{
									PRTMSG(MSG_DBG, "ReUpload %d win ", ulWinNo);
									PRTMSG(MSG_DBG, "the %d pack, len=%d\n", j, (DWORD)g_objDataUploadSta[k].m_objDataWin[i].m_iBufLen[j] );
									//msleep(5);
								}
							}
						}
					}
				}
			}
		}
	}
}

// ����ʱ�ӣ�2��+�����ʣ�1��
int	Deal3836(char *v_databuf, int v_datalen)
{
	WORD	usTimeDelay = 0;
	BYTE	uszPackLostRate = 0;

	memcpy((void*)&usTimeDelay, v_databuf, 2);
	uszPackLostRate = v_databuf[2];

	//PRTMSG(MSG_DBG, "usTimeDelay = %d ms, ", usTimeDelay);
	//PRTMSG(MSG_DBG, "uszPackLostRate = %d\n", uszPackLostRate);
}

