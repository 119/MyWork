#include "yx_QianStdAfx.h"

#if USE_LIAONING_SANQI == 1

void *G_ThreadTransportSend(void *arg)
{
	g_objTransport.P_ThreadSend();
}

void *G_ThreadTransportRecv(void *arg)
{
	g_objTransport.P_ThreadRecv();
}

void *G_ThreadTransportReUpload(void *arg)
{
	g_objTransport.P_ThreadReUpload();
}

void *G_ThreadRecvLongCmd(void *arg)
{
	g_objTransport.P_ThreadRecvLongCmd();
}
//////////////////////////////////////////////////////////////////////////

	#undef MSG_HEAD
	#define MSG_HEAD	("QianExe-LN_Transport")


CLN_Transprot::CLN_Transprot()
{
	m_objRecvDataMng.InitCfg(60*1024, 30*1000);
	m_objSendDataMng.InitCfg(60*1024, 30*1000);
	m_objRecvLCmdDataMng.InitCfg(10*1024, 30*1000);
	m_bytSta = WaitData;
	m_dwSEQ = 0;
	m_bytCurPackNum = 0;
	m_bytPackTotal = 0;
	memset(m_bytCRC, 0, 2);
	memset(m_bLongCmdPackSta, 0, sizeof(m_bLongCmdPackSta));
	memset(m_dwPackLen, 0, sizeof(m_dwPackLen));
// 	memset(m_aryiWaitCheckSEQ, 0, sizeof(m_aryiWaitCheckSEQ));
// 	memset(m_arybValidSEQ, 0, sizeof(m_arybValidSEQ));
// 	m_iSEQCnt = 0;
}

CLN_Transprot::~CLN_Transprot()
{

}

int CLN_Transprot::Init()
{
//	pthread_mutex_init(&m_MutexSEQ, NULL);
	m_objReUploadMng.Init();

	pthread_t threadRecv, threadSend, threadReUpload, threadRecvLongCmd;

	// �����߳�
	if( pthread_create(&threadSend, NULL, G_ThreadTransportSend, NULL) != 0
		||
		pthread_create(&threadRecv, NULL, G_ThreadTransportRecv, NULL) != 0
		||
		pthread_create(&threadReUpload, NULL, G_ThreadTransportReUpload, NULL) != 0
		||
		pthread_create(&threadRecvLongCmd, NULL, G_ThreadRecvLongCmd, NULL) != 0
	  )
	{
		PRTMSG(MSG_ERR, "create Transport thread failed!\n");
	}
}

int CLN_Transprot::Release()
{
//	pthread_mutex_destroy(&m_MutexSEQ);
	m_objReUploadMng.Release();
}


// Ϊ����SEQ��ˮ�Ŷ���ۼӣ��˺���ֻ������P_ThreadSend�е��ã������ط����õ���
int CLN_Transprot::MakeTransportFrame(byte v_bytControl, char v_szUseLoaclSEQ, char *v_szCenterSEQ, char *v_szOptHeadBuf, DWORD v_dwOptHeadLen, char *v_szSrcBuf, DWORD v_dwSrcLen, char *v_szDesBuf, DWORD &v_dwDesLen)
{
	//  �ֽ�0	�ֽ�1	�ֽ�2	�ֽ�3

	//  У��ͣ�2��	�汾�ţ�1��	ͷ���ȣ�1��
	//	��������֣�1��	���ʣ�1��	���ܷ�ʽ��1��	������1��
	//	SEQ��ˮ�ţ�4��
	//	DEV_ID��4�ֽڣ�4��
	//	DEV_ID��4�ֽڣ�4��
	//	��׼ʱ�����4��
	//	�����ͷ����ѡ�0~200��
	//	Ӧ�ò����ݣ��ɱ䣩

	char szBuf[UDP_SENDSIZE] = {0};
	int  iLen = 0;
	static int iLocalSEQ = 0;		// ����̨������λ������SEQ��ˮ�Ž����´�0��ʼ
									// ��̨����ʱ��Լ1�������ң������ݰ��������е�����ʱ�䲻�����֮����������������ˮ���ظ�������

	szBuf[iLen++] = 0x00;						// У�������0
	szBuf[iLen++] = 0x00;
	szBuf[iLen++] = 0x00;						// �汾��
	szBuf[iLen++] = (byte)(v_dwOptHeadLen + 24);// ͷ����
	szBuf[iLen++] = v_bytControl;				// ���������
	szBuf[iLen++] = 0x01;						// ���ʣ�GPRS-UDP
	szBuf[iLen++] = 0x00;						// ���ܷ�ʽ
	szBuf[iLen++] = 0x00;						// ����

	if( v_szUseLoaclSEQ == 1 )					// SEQ��ˮ��
	{
		G_LocalToNet((char*)&iLocalSEQ, szBuf+iLen, 4);
		iLocalSEQ++;

// 		// ������SEQ��ˮ�ű������������ڽ���Ӧ��ʱ�����ж�
// 		pthread_mutex_lock(&m_MutexSEQ);
// 
// 		m_aryiWaitCheckSEQ[m_iSEQCnt] = iLocalSEQ;
// 		m_arybValidSEQ[m_iSEQCnt] = true;
// 
// 		m_iSEQCnt++;
// 		if( m_iSEQCnt >= SEQ_ARY_SIZE )	m_iSEQCnt = 0;
// 
// 		pthread_mutex_unlock(&m_MutexSEQ);
	} else
	{
		memcpy(szBuf+iLen, v_szCenterSEQ, 4);
	}               
	iLen += 4;

	G_GetDEV_ID(szBuf+iLen);					// DEV_ID
	iLen += 8;

	DWORD dwSec = G_GetSecCnt();				// ʱ���
	G_LocalToNet((char*)&dwSec, szBuf+iLen, 4);
	iLen += 4;  

	if( v_dwOptHeadLen != 0 && v_szOptHeadBuf != NULL )		// �����ͷ����ѡ��
	{
		memcpy(szBuf+iLen, v_szOptHeadBuf, v_dwOptHeadLen); 
		iLen += v_dwOptHeadLen;
	}

	if( v_dwSrcLen != 0 && v_szSrcBuf != NULL )		// Ӧ�ò�����
	{
		memcpy(szBuf+iLen, v_szSrcBuf, v_dwSrcLen); 
		iLen += v_dwSrcLen;
	}

	short shCRC = CaculateCRC((byte*)szBuf+2, iLen-2);	// ����CRC16У���
	G_LocalToNet((char*)&shCRC, szBuf, 2);

	memcpy(v_szDesBuf, szBuf, iLen);
	v_dwDesLen = iLen;

	// �����ã���֤CRC16У����Ƿ���ȷ
//	char buf[] = {0x00, 0x00, 0x00, 0x18, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
//		, 0xFF, 0xFF, 0xF1, 0x59, 0x42, 0x01, 0x67, 0x33, 0x13, 0xEB, 0x80, 0x73, 0x41, 0x00, 0x03};
// 	char buf[] = {0x33, 0xC1, 0x00, 0x18, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// 		0xFF, 0xFF, 0xF1, 0x59, 0x42, 0x01, 0x67, 0x33, 0x09, 0x68, 0x34, 0x2F, 0x41, 0x00,
// 		0x01, 0x00, 0x02, 0x74, 0x50, 0x00, 0x01, 0x01, 0x00, 0x03, 0x76, 0x65, 0x72, 0x00,
// 		0x0B, 0x30, 0x35, 0x35, 0x30, 0x30, 0x30, 0x33, 0x30, 0x31, 0x30, 0x30, 0x10, 0x02, 
// 		0x70, 0x49, 0x00, 0x25, 0x00, 0x01, 0x74, 0x00, 0x01, 0x00, 0x00, 0x03, 0x6C, 0x61,
// 		0x74, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x6C, 0x6F, 0x6E, 0x00, 0x04, 
// 		0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x61, 0x6C, 0x74, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x73, 0x00, 0x01,
// 		0x00, 0x00, 0x01, 0x64, 0x00, 0x02, 0x00, 0x03, 0x00, 0x02, 0x6d, 0x65, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00};
// 	short shCRC1 = CaculateCRC((byte*)buf+2, sizeof(buf)-2);
// 	G_LocalToNet((char*)&shCRC1, buf, 2);	
// 	memcpy(v_szDesBuf, buf, sizeof(buf));
//	v_dwDesLen = sizeof(buf);

	return 0;
}

void CLN_Transprot::P_ThreadSend()
{
	char  szPopBuf[UDP_SENDSIZE] = {0};
	DWORD dwPopLen = 0;
	byte  bytLvl = 0;
	DWORD dwPushTm = 0;

	char  szSendBuf[UDP_SENDSIZE] = {0};
	DWORD dwSendLen = 0;
	byte  bytControl = 0;
	DWORD dwSEQ = 0;

	while( !g_bProgExit )
	{
		if( !m_objSendDataMng.PopData(bytLvl, sizeof(szPopBuf), dwPopLen, szPopBuf, dwPushTm) )
		{
			// szPopBuf[0]�Ǵ���ģʽ��szPopBuf[1]�Ǵ�������֣�
			// szPopBuf[2]��ʾʹ������SEQ��ˮ�Ż��ǳ�̨SEQ��ˮ�ţ�3��4��5��6ΪSEQ��ˮ�� dģʽb1Ϊͷ��+ͷ����+Ӧ�ò�����
			if( MODE_C == szPopBuf[0] || MODE_D == szPopBuf[0] || MODE_E == szPopBuf[0] )
			{
				if(0xb1 == (byte)szPopBuf[1] && !MakeTransportFrame((byte)szPopBuf[1], szPopBuf[2], szPopBuf+3, szPopBuf+11, szPopBuf[10], szPopBuf+11+szPopBuf[10], dwPopLen-11-szPopBuf[10], szSendBuf, dwSendLen ) )
				{
					g_objNetWork.SendSocketData(szSendBuf, dwSendLen);
				} else if( !MakeTransportFrame((byte)szPopBuf[1], szPopBuf[2], szPopBuf+3, szPopBuf+7, dwPopLen-7, NULL, 0, szSendBuf, dwSendLen ) )
				{
					g_objNetWork.SendSocketData(szSendBuf, dwSendLen);
				}
			} else
			{
				if( !MakeTransportFrame((byte)szPopBuf[1], szPopBuf[2], szPopBuf+3, NULL, 0, szPopBuf+7, dwPopLen-7, szSendBuf, dwSendLen ) )
				{
					g_objNetWork.SendSocketData(szSendBuf, dwSendLen);
				}
			}

			// ��ȡ��֡���SEQ��ˮ��
			memcpy((void*)&dwSEQ, szSendBuf+8, 4);

			// ��ͬģʽ�Ĵ���㴦��
			switch(szPopBuf[0])
			{
			case MODE_A:
				break;
			case MODE_D:
				if(0xb1 == (byte)szPopBuf[1])
					break;
			case MODE_C:
				if(0xa3 == (byte)szPopBuf[1])
					break;
			case MODE_B:
				m_objReUploadMng.InsertOneNodeToTail(dwSEQ, szSendBuf, dwSendLen, 3, GetTickCount());
				break;      

			default:
				break;
			}
		}

		_SND_ONE_FRAME_END:
		usleep(20000);
	}
}

void CLN_Transprot::P_ThreadRecv()
{
	char  szPopBuf[UDP_RECVSIZE] = {0};
	DWORD dwPopLen = 0;
	byte  bytLvl = 0;
	DWORD dwPushTm = 0;
	DWORD dwPacketNum = 0;
	char  szDevID[8] = {0};
	DWORD dwSEQ;
	char  szPushBuf[UDP_RECVSIZE] = {0};
	DWORD dwPushLen = 0;

	while( !g_bProgExit )
	{
		if( !m_objRecvDataMng.PopData(bytLvl, sizeof(szPopBuf), dwPopLen, szPopBuf, dwPushTm) )
		{
			// ���У���
			if( !CheckCRC((byte*)szPopBuf, (int)dwPopLen) )
			{
				PRTMSG(MSG_ERR, "Center Frame check crc err!\n");
				goto _RCV_ONE_FRAME_END;
			}

			// ���汾��
			if( 0 != szPopBuf[2] )
			{
				PRTMSG(MSG_ERR, "Center Frame check version err!\n");
				goto _RCV_ONE_FRAME_END;
			}

			// ���DEV_ID
			G_GetDEV_ID(szDevID);
			if( 0 != memcmp(szDevID, szPopBuf+12, 8) )
			{
				PRTMSG(MSG_ERR, "Center Frame check DEV_ID err!\n");
				PRTMSG(MSG_DBG, "Center DEV_ID: %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x\n", szPopBuf[12], szPopBuf[13], szPopBuf[14], szPopBuf[15], szPopBuf[16], szPopBuf[17], szPopBuf[18], szPopBuf[19]);
				PRTMSG(MSG_DBG, "Local  DEV_ID: %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x\n", szDevID[0], szDevID[1], szDevID[2], szDevID[3], szDevID[4], szDevID[5], szDevID[6], szDevID[7]);
				goto _RCV_ONE_FRAME_END;
			}

			memcpy((void*)&dwSEQ, szPopBuf+8, 4);
			m_objReUploadMng.DeleteSpecialNode(dwSEQ);
			// ��鴫�������
			switch( szPopBuf[4] )
			{
			case 0x00:		// ģʽA��ͨ���ݴ���
			case 0x10:		// ģʽB���ݴ�������
				{
					// ��ȡ��ˮ�ź�Ӧ�ò����ݷ��͵��ϲ���д���
					memcpy(szPushBuf, szPopBuf+8, 4);
					memcpy(szPushBuf+4, szPopBuf+szPopBuf[3], dwPopLen-szPopBuf[3]);
					dwPushLen = 4 + dwPopLen - szPopBuf[3];
					g_objApplication.m_objRecvDataMng.PushData(LV2, dwPushLen, szPushBuf, dwPacketNum);
				}
				break;

			case 0x11:		//ģʽB���ݴ���Ӧ��
				{
					if( 0 == szPopBuf[dwPopLen-1] )	// ����ɹ�
					{
//						DWORD dwSEQ;
//						memcpy((void*)&dwSEQ, szPopBuf+8, 4);
//						m_objReUploadMng.DeleteSpecialNode(dwSEQ);

						// ���б�Ҫ���˴�����Ӧ�ò�֪ͨ�����ƺ�û��Ҫ��
					}
				}
				break;
				//ģʽC���ݴ���
			case 0x20: // ����Ӧ��
				break;
			case 0x21: // �ϵ�����Ӧ��
				{   

					memcpy(szPushBuf, szPopBuf + 24, szPopBuf[3] - 24);
					dwPushLen = szPopBuf[3] - 24;
					Deal_21(szPushBuf, dwPushLen);
				}
				break;
			case 0x22: // ���������ش�Ӧ�ò��������⴦��
				{

					memcpy(szPushBuf, szPopBuf + 24, dwPopLen - 24);
					dwPushLen = dwPopLen - 24;
					Deal_22(szPushBuf, dwPushLen);

				}
				break;
			case 0x23: // ���ս����ѯ
				{

					memcpy(szPushBuf, szPopBuf + 24, szPopBuf[3] - 24);
					dwPushLen = szPopBuf[3] - 24;
					Deal_23(szPushBuf, dwPushLen);
				}
				break;
			case 0x24: // ���ز���Ӧ��
				{
					memcpy(szPushBuf, szPopBuf + 24, szPopBuf[3] - 24);
					dwPushLen = szPopBuf[3] - 24;
					Deal_24(szPushBuf, dwPushLen);
				}
				break;
				//ģʽD���ݴ���
			case 0x30: // �ϴ�����Ӧ��
				{
					memcpy(szPushBuf, szPopBuf + 24, szPopBuf[3] - 24);
					dwPushLen = szPopBuf[3] - 24;
					Deal_30(szPushBuf, dwPushLen);
				}
				break;
			case 0x31: // �ϴ����ձ���
				{
					memcpy(szPushBuf, szPopBuf + 24, szPopBuf[3] - 24);
					dwPushLen = szPopBuf[3] - 24;
					Deal_31(szPushBuf, dwPushLen);
				}
				break;
			case 0x32: // �ϴ����Ӧ��
				{
					memcpy(szPushBuf, szPopBuf + 24, szPopBuf[3] - 24);
					dwPushLen = szPopBuf[3] - 24;
					Deal_32(szPushBuf, dwPushLen);

				}
				break;
			case 0x33: // �ϵ���������
				{
					memcpy(szPushBuf, szPopBuf + 24, szPopBuf[3] - 24);
					dwPushLen = szPopBuf[3] - 24;
					Deal_33(szPushBuf, dwPushLen);
				}
				break;
			case 0x34: // �ϴ�����Ӧ��
				{
					memcpy(szPushBuf, szPopBuf + 24, szPopBuf[3] - 24);
					dwPushLen = szPopBuf[3] - 24;
					Deal_34(szPushBuf, dwPushLen);
				}
				break;
			case 0x41: // ��ָ������
				{
					memcpy(szPushBuf, szPopBuf + 24, dwPopLen - 24);
					dwPushLen = dwPopLen - 24;
					Deal_41(szPopBuf+8, szPushBuf, dwPushLen);
				}
				break;
			case 0x42: // ���ݽ��ս����ѯ
				{
					memcpy(szPushBuf, szPopBuf + 24, dwPopLen - 24);
					dwPushLen = dwPopLen - 24;
					Deal_42(szPopBuf+8, szPushBuf, dwPushLen);
				}
				break;
			default:
				PRTMSG(MSG_ERR, "Center Frame check control err, control=%02x\n", szPopBuf[4]);
				break;
			}
		}

		_RCV_ONE_FRAME_END:
		usleep(20000);
	}
}

void CLN_Transprot::P_ThreadReUpload()
{
	int   i = 0;
	char  szBuf[UDP_SENDSIZE] = {0};
	DWORD dwLen = 0;

	DWORD dwSEQ = 0;
	char  szTempBuf[4] = {0};

	while( !g_bProgExit )
	{
		i = 0;

		while( m_objReUploadMng.IsCurNodeValid() )
		{
			// ���ϴη���ʱ�̾����ʱ�ѳ���10��
			if( GetTickCount() - m_objReUploadMng.GetCurNodeTm() > 10*1000 )
			{
				// �жϸýڵ��Ƿ���Ҫɾ��
				if( m_objReUploadMng.IsNeedDelCurNode() )
				{
					// ���б�Ҫ����ɾ��ǰ����Ӧ�ò�֪ͨ���������ƺ�û��Ҫ��

					// ɾ��
					m_objReUploadMng.DeleteCurNode();
				} else
				{
					// ��ȡ�ڵ�����
					if( ( dwLen = m_objReUploadMng.GetCurNodeData(szBuf, sizeof(szBuf), GetTickCount() ) ) != 0  )
					{
						g_objNetWork.SendSocketData(szBuf, dwLen);
						//					PRTMSG(MSG_ERR, "resend!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
						// �����ѷ��ʹ������ϴη���ʱ��
						m_objReUploadMng.UpdateNode();
					} else
					{
						PRTMSG(MSG_DBG, "Get node data failed!\n");
					}
				}
			}
			_NEXT_NODE:
			// ���ѵ�������β������GotoNextNode�����ط�0�����򽫷���0
			if( m_objReUploadMng.GotoNextNode() )
				goto _ONE_REUPLOAD_LOOP_END;

			i++;
			if(i > 100)		// ÿ��ѭ������ж�100���ڵ�
				goto _ONE_REUPLOAD_LOOP_END;
		}

		_ONE_REUPLOAD_LOOP_END:
		usleep(200000);
	}
}

int CLN_Transprot::Deal1B_0001(char *v_szBuf, DWORD v_dwLen, DWORD v_dwCenterSEQ)
{
	int packagelen, datalen, md5len;
	byte fupdate = 0xff, updatetype = 0xff; 
	char ver[12], md5[16] = {0};
	char    szReplyBuf[256] = {0};
	DWORD   dwReplyLen = 0;
	int iret = 0, limit;
	int i = 0;
	char szTempBuf[256] = {0};
	ushort  sTempLen = 0, sleftlen;
	char *akvp;
	AKV_t AKVTemp, *akv;
	akv = &AKVTemp;
	if( v_dwLen <= 0 )
	{
		PRTMSG(MSG_DBG, "Update msg len err!!!\n");
		iret = ERR_PAR;
		goto _Deal1B_0001_END;
	}
	akvp = v_szBuf;
	sleftlen = v_dwLen;
	for(limit = 0; limit < 6; limit++)
	{
		if( i >= v_dwLen )
		{
			break;
		}
		if( !AKV_Jugre(akvp, akv, sleftlen) )
		{
			PRTMSG(MSG_DBG, "AKV err!!!!!\n");
			iret = ERR_PAR;
			goto _Deal1B_0001_END;
		}
		i += akv->AKVLen;
		sleftlen -= akv->AKVLen;
		akvp += akv->AKVLen;
		switch( Key_Jugres(akv->Key))
		{
		case 18: // ������ʾ 01��ǿ������ 02���������� 03�������������
			{
				if( 1 != akv->Vlen || akv->Value[0] > 0x03)
				{
					PRTMSG(MSG_DBG, "Update flag err!!\n");
					iret = ERR_PAR;
					goto _Deal1B_0001_END;
				}
				fupdate = akv->Value[0];
				PRTMSG(MSG_DBG, "Update flag %d\n", fupdate);
			}
			break;
		case 19: // �������� 01������ϵͳ 02��Ӧ�ó��� 03��LED
			{
				if( 1 != akv->Vlen || akv->Value[0] > 0x03)
				{
					PRTMSG(MSG_DBG, "Update type err!!\n");
					iret = ERR_PAR;
					goto _Deal1B_0001_END;
				}
				updatetype = (byte)akv->Value[0];
				PRTMSG(MSG_DBG, "Update type %d\n", updatetype);
			}
			break;
		case 20: // Ŀ��汾��Ϣ
			{
				if( akv->Vlen > 11 )
				{
					PRTMSG(MSG_DBG, "Ver too long!\n");
					iret = ERR_PAR;
					goto _Deal1B_0001_END;
				}
				memcpy(ver, akv->Value, akv->Vlen);
				ver[akv->Vlen] = '\0';
				PRTMSG(MSG_DBG, "Target ver:%s\n", ver);
			}
			break;
		case 21: // MD5ֵ
			{
				if( akv->Vlen > 16 )
				{
					PRTMSG(MSG_DBG, "MD5 too long!\n");
					iret = ERR_PAR;
					goto _Deal1B_0001_END;
				}
				md5len = akv->Vlen;
				memcpy(md5, akv->Value, akv->Vlen);
			}
			break;
		case 22: // ��������С
			{
				int a = 0;
				if( 0 == akv->Vlen || akv->Vlen > 4)
				{
					PRTMSG(MSG_DBG, "Data len err!!\n");
					iret = ERR_PAR;
					goto _Deal1B_0001_END;
				}
				datalen = (byte)akv->Value[a];
				while( --akv->Vlen)
				{
					datalen = (datalen << 8) + (byte)akv->Value[++a];
				}
				PRTMSG(MSG_DBG, "Data len %d\n", datalen);
			}
			break;
		case 23: //������
			{
				int a = 0;
				if( 0 == akv->Vlen || akv->Vlen > 2)
				{
					PRTMSG(MSG_DBG, "Package len err!!\n");
					iret = ERR_PAR;
					goto _Deal1B_0001_END;
				}
				packagelen = (byte)akv->Value[a];
				while( --akv->Vlen)
				{
					packagelen = (packagelen << 8) + (byte)akv->Value[++a];
				}
				PRTMSG(MSG_DBG, "Package len %d\n", packagelen);
			}
			break;
		default:
			break;
		}
	}
	tag2QcgvCfg objtag2QcgvCfg;
	GetSecCfg(&objtag2QcgvCfg, sizeof(objtag2QcgvCfg), offsetof(tagSecondCfg, m_uni2QcgvCfg.m_obj2QcgvCfg), sizeof(objtag2QcgvCfg));
	memcpy(g_objLNDownLoad.m_bytMd5, md5, 16);
	g_objLNDownLoad.m_dwFileSize = datalen;
	g_objLNDownLoad.m_ulPackageLen = packagelen;
	g_objLNDownLoad.m_bytFileType = updatetype;
	if( 0x03 == fupdate)
	{ //���֮ǰ�������������ݺ�����״̬

		g_objLNDownLoad._DeleteFile(g_objLNDownLoad.m_bytMd5);
		g_objLNDownLoad.m_objFileList.m_objFile[updatetype].m_bExist = false;
		g_objLNDownLoad.m_objFileList.m_objFile[updatetype].m_ulDownSize = 0;
		g_objLNDownLoad.m_objFileList.m_objFile[updatetype].m_ulFileSize = 0;
		g_objLNDownLoad.m_objFileList.m_objFile[updatetype].m_ulPackLen = 0;
		memset(g_objLNDownLoad.m_objFileList.m_objFile[updatetype].m_bytMd5, 0, 16);
		g_objLNDownLoad._RenewFileLst();
		g_objLNDownLoad.m_dwFileSize = 0;
		g_objLNDownLoad.m_ulPackageLen = 0;
		g_objLNDownLoad.m_bytFileType = ~0;
		memset(g_objLNDownLoad.m_bytMd5, 0, 16);

	}
	if( 0x01 != fupdate && (0 == strlen(ver) || strncmp(ver, objtag2QcgvCfg.m_temiver, strlen(ver)) <= 0) )
	{//03h--�ܾ�����
		PRTMSG(MSG_DBG, "Current ver newer than target!!\n");
		szTempBuf[sTempLen++] = 0x03;
		goto _Deal1B_0001_END2;
	}
// ��Ҫ����
	g_objLNDownLoad.m_bNeedUpdate = true;
	_Deal1B_0001_END:
	if( iret )
	{
		szTempBuf[sTempLen++] = 0x02;
	} else
	{
		szTempBuf[sTempLen++] = 0x01;
	}
	_Deal1B_0001_END2:
	if( !g_objApplication.MakeAppFrame(0x1b, 0x0001, szTempBuf, sTempLen, szReplyBuf, sizeof(szReplyBuf), dwReplyLen) )
	{
		g_objApplication.SendAppFrame(MODE_A, 0x80, false, v_dwCenterSEQ, szReplyBuf, dwReplyLen);
	}
	return iret;

}

int CLN_Transprot::Deal1B_0002(char *v_szBuf, DWORD v_dwLen, DWORD v_dwCenterSEQ)
{

}

int CLN_Transprot::Deal_21(char *v_szBuf, DWORD v_dwLen)
{
	char szBuf[100] = {0};
	szBuf[0] = 0x21;
	if( v_dwLen != 17 )
	{
		PRTMSG(MSG_DBG, "0x21 Head len err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNDownLoad.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x21 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	szBuf[1] = v_szBuf[16];
	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNDownLoad.m_objRecvMsg.PushData((byte)LV1, 2, szBuf, dwPacketNum);
}

int CLN_Transprot::Deal_22(char *v_szBuf, DWORD v_dwLen)
{
	char  szBuf[UDP_RECVSIZE] = {0};
	char *szBufp;
	DWORD dwPushLen = 0;
	DWORD dwdatalen;
	int cnt, iret = 0;
	char *akvp;
	AKV_t AKVTemp, *akv;
	akv = &AKVTemp;

	if( v_dwLen < 18 )
	{
		PRTMSG(MSG_DBG, "0x22 MSG err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNDownLoad.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x22 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	szBuf[0] = 0x22;
	memcpy(szBuf + 1, v_szBuf + 16, 2);
	dwPushLen = 3;
	szBufp = v_szBuf + 21;

	dwdatalen = (byte)szBufp[0];

	cnt = 1;
	for( ; cnt < 4 ; cnt++)
	{
		dwdatalen = (dwdatalen << 8) + (byte)szBufp[cnt];
	}
//	PRTMSG(MSG_DBG, "Rcv: %d Data len %d\n", v_dwLen, dwdatalen);
	if( dwdatalen > v_dwLen - 21 || dwdatalen > UDP_RECVSIZE)
	{
		PRTMSG(MSG_DBG, "Rcv: %d Data len %d err!!!\n", v_dwLen, dwdatalen);
		iret = ERR_PAR;
		return iret;
	}
	memcpy(szBuf + dwPushLen, szBufp + 4, dwdatalen);
	dwPushLen += dwdatalen;
	akvp = szBufp + dwdatalen + 4;
//	PrintString(akvp, 24);
	if( v_dwLen > dwdatalen + 25)
	{
		if( !AKV_Jugre(akvp, akv, v_dwLen - dwdatalen - 25) )
		{
			PRTMSG(MSG_DBG, "AKV err!!!!!\n");
			iret = ERR_PAR;
			return iret;
		}
		if( akv->Key[0] == 't' && akv->Key[1] == 'P' )
		{
			if(g_objLNDownLoad.m_bytFileType != akv->Value[0])
			{
				PRTMSG(MSG_DBG, "Data type err!!!!!\n");
				iret = ERR_PAR;
				return iret;
			}
		}

	}

	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNDownLoad.m_objRecvMsg.PushData((byte)LV3, dwPushLen, szBuf, dwPacketNum);

}



int CLN_Transprot::Deal_23(char *v_szBuf, DWORD v_dwLen)
{
	char szBuf[100] = {0};
	szBuf[0] = 0x23;
	if( v_dwLen != 22 )
	{
		PRTMSG(MSG_DBG, "0x23 Head len err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNDownLoad.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x23 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	memcpy(szBuf + 1, v_szBuf + 16, 6);
	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNDownLoad.m_objRecvMsg.PushData((byte)LV3, 7, szBuf, dwPacketNum);
}

int CLN_Transprot::Deal_24(char *v_szBuf, DWORD v_dwLen)
{
	char szBuf[100] = {0};
	szBuf[0] = 0x24;
	if( v_dwLen != 17 )
	{
		PRTMSG(MSG_DBG, "0x24 Head len err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNDownLoad.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x24 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	szBuf[1] = v_szBuf[16];
	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNDownLoad.m_objRecvMsg.PushData((byte)LV1, 2, szBuf, dwPacketNum);
}



int CLN_Transprot::Deal_30(char *v_szBuf, DWORD v_dwLen)
{
	char szBuf[100] = {0};
	szBuf[0] = 0x30;
	if( v_dwLen != 17 )
	{
		PRTMSG(MSG_DBG, "0x30 Head len err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNPhoto.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x30 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	szBuf[1] = v_szBuf[16];
	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNPhoto.m_objRecvMsg.PushData((byte)LV1, 2, szBuf, dwPacketNum);
}

int CLN_Transprot::Deal_31(char *v_szBuf, DWORD v_dwLen)
{
	char szBuf[100] = {0};
	szBuf[0] = 0x31;
	if( v_dwLen <= 17 )
	{
		PRTMSG(MSG_DBG, "0x31 Head len err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNPhoto.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x31 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	memcpy(szBuf + 1, v_szBuf + 16, v_dwLen - 16);
	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNPhoto.m_objRecvMsg.PushData((byte)LV1, v_dwLen - 15, szBuf, dwPacketNum);
}

int CLN_Transprot::Deal_32(char *v_szBuf, DWORD v_dwLen)
{
	char szBuf[100] = {0};
	szBuf[0] = 0x32;
	if( v_dwLen <= 17 )
	{
		PRTMSG(MSG_DBG, "0x32 Head len err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNPhoto.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x32 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	memcpy(szBuf + 1, v_szBuf + 16, v_dwLen - 16);
	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNPhoto.m_objRecvMsg.PushData((byte)LV1, v_dwLen - 15, szBuf, dwPacketNum);

}



int CLN_Transprot::Deal_33(char *v_szBuf, DWORD v_dwLen)
{
	char szBuf[100] = {0};
	szBuf[0] = 0x33;
	if( v_dwLen <= 17 )
	{
		PRTMSG(MSG_DBG, "0x33 Head len err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNPhoto.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x33 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	memcpy(szBuf + 1, v_szBuf + 16, v_dwLen - 16);
	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNPhoto.m_objRecvMsg.PushData((byte)LV1, v_dwLen - 15, szBuf, dwPacketNum);
}

int CLN_Transprot::Deal_34(char *v_szBuf, DWORD v_dwLen)
{
	char szBuf[100] = {0};
	szBuf[0] = 0x34;
	if( v_dwLen != 17 )
	{
		PRTMSG(MSG_DBG, "0x34 Head len err!! %d\n", v_dwLen);
		return -1;
	}
	if(0 != memcmp(g_objLNPhoto.m_bytMd5, v_szBuf, 16))
	{
		PRTMSG(MSG_DBG, "0x34 MD5 err!! %d\n", v_dwLen);    
		return -1;
	}
	szBuf[1] = v_szBuf[16];
	// ������ն���
	DWORD dwPacketNum = 0;
	g_objLNPhoto.m_objRecvMsg.PushData((byte)LV1, 2, szBuf, dwPacketNum);
}

int CLN_Transprot::Deal_41(char *szSEQ, char *v_szBuf, DWORD v_dwLen)
{
	char szTempBuf[2048] = {0};
	DWORD dwTempLen = 0, dwPacketNum;
	if(v_dwLen > 1030 || v_dwLen <= 6)
	{
		PRTMSG(MSG_DBG, "0x41 len err!! %d\n", v_dwLen);
		return -1;
	}
	szTempBuf[0] = 0x41;
	memcpy(szTempBuf + 1, szSEQ, 4);
	dwTempLen = 5;
	memcpy(szTempBuf + dwTempLen, v_szBuf, v_dwLen);
	dwTempLen += v_dwLen;
	m_objRecvLCmdDataMng.PushData(LV3, dwTempLen, szTempBuf, dwPacketNum);
}

int CLN_Transprot::Deal_42(char *szSEQ, char *v_szBuf, DWORD v_dwLen)
{
	char szTempBuf[20] = {0};
	DWORD dwTempLen = 0, dwPacketNum;
	szTempBuf[0] = 0x42;
	memcpy(szTempBuf + 1, szSEQ, 4);
	dwTempLen = 5;
	m_objRecvLCmdDataMng.PushData(LV3, dwTempLen, szTempBuf, dwPacketNum);
}
void CLN_Transprot::P_ThreadRecvLongCmd()
{
	char szRecBuf[2048] = {0};
	DWORD dwRecLen, dwSleepTm, dwSEQ;
	char szSendBuf[50] = {0};
	char szAppBuf[MAX_LONGCMD_PACK*1024];
	DWORD dwSendLen;
	byte bytPackTotal = 0, bLvl;
	bool bfirstpack = true, bRcving = false;
	DWORD dwBufPtr, dwPacketNum, dwWaitTm, dwPushTm, dwRcvlen = 0;
	ushort uscmdlen, usSendNum = 0;
	dwWaitTm = GetTickCount();
	while( !g_bProgExit )
	{
		switch(m_bytSta)
		{
		case WaitData:
			{
				int b;
				dwSleepTm = 50000;
				if( !m_objRecvLCmdDataMng.PopData(bLvl, sizeof(szRecBuf), dwRecLen, szRecBuf, dwPushTm) )
				{
					dwSleepTm = 0;
					bRcving = true;
					usSendNum = 0;
					dwBufPtr = 1;
					b = 4;
					dwSEQ = (byte)szRecBuf[dwBufPtr];
					while( --b )
					{
						dwSEQ = (dwSEQ << 8) + (byte)szRecBuf[++dwBufPtr];
					}
					++dwBufPtr;
					dwWaitTm = GetTickCount();
					switch( szRecBuf[0] )
					{
					case 0x41:
						{               
							bytPackTotal = (byte)szRecBuf[dwBufPtr++];
//							PRTMSG(MSG_DBG, "0x41 data total pack:%d\n", bytPackTotal);
							m_bytCurPackNum = (byte)szRecBuf[dwBufPtr++];
//							PRTMSG(MSG_DBG, "0x41 data current pack:%d\n", m_bytCurPackNum);
							if( (bytPackTotal != m_bytCurPackNum + 1 && dwRecLen - 11 != 1024))
							{
								PRTMSG(MSG_DBG, "0x41 datalen != 1024 err!! %d\n", dwRecLen - 11);
								break;
							}
							if( bytPackTotal > MAX_LONGCMD_PACK  || (bytPackTotal < m_bytCurPackNum + 1))
							{
								szSendBuf[0] = 0x05;
								dwSendLen = 1;
								m_bytSta = EndRcv;
								break;
							}
							uscmdlen = szRecBuf[dwBufPtr] * 256 + (byte)szRecBuf[dwBufPtr + 1];
							dwBufPtr += 2;

							if( true == bfirstpack)
							{
								m_dwSEQ = dwSEQ;
								bfirstpack = false;
								m_bytPackTotal = bytPackTotal;
								memcpy(m_bytCRC, szRecBuf + dwBufPtr, 2);
							}
							dwBufPtr += 2;
							if( m_dwSEQ != dwSEQ )
							{
								szSendBuf[0] = 0x04;
								dwSendLen = 1;
								g_objApplication.SendAppFrame(MODE_E, 0xc2, false, dwSEQ, szSendBuf, dwSendLen);
								break;
							}
							m_bytSta = RcvData;
						}
						break;
					case 0x42:
						{
							m_bytSta = TranResult;
						}
						break;
					default:
						break;
					}
				}
				if(bRcving)
				{
					if( usSendNum >= 3 )
					{
						usSendNum = 0;
						m_bytSta = EndRcv;
					}
					if( GetTickCount() - dwWaitTm > 10000 && usSendNum < 3)
					{
						usSendNum++;
						dwWaitTm = GetTickCount();
						m_bytSta = TranResult;
					}
				}

			}
			break;
		case RcvData:
			{
				memcpy(m_bytLongCmdBuf[m_bytCurPackNum], szRecBuf + dwBufPtr, dwRecLen - 11);
				m_dwPackLen[m_bytCurPackNum] = dwRecLen - 11;
				m_bLongCmdPackSta[m_bytCurPackNum] = true;
				m_bytSta = WaitData;
			}
			break;
		case TranResult:
			{
				int i, a = 2;
				dwSendLen = 1;
				dwRcvlen = 0;
				for(i = 0; i < m_bytPackTotal; i++  )
				{
					if( !m_bLongCmdPackSta[i] )
					{
						szSendBuf[1]++;
						szSendBuf[a++] = i;
					}
					dwRcvlen += m_dwPackLen[i];
				}
				if( szSendBuf[1] )
				{
					szSendBuf[0] = 0x02;
					dwSendLen += szSendBuf[1] + 1;
					m_bytSta = WaitData;    
					g_objApplication.SendAppFrame(MODE_E, 0xc2, false, dwSEQ, szSendBuf, dwSendLen);
					break;
				}
				if( 0 == m_dwSEQ )
				{
					bfirstpack = true;
					bRcving =false;
					usSendNum = 0;
					m_bytCurPackNum = 0;
					m_bytPackTotal = 0;
					memset(m_bytCRC, 0, 2);
					memset(m_bLongCmdPackSta, 0, sizeof(m_bLongCmdPackSta));
					m_bytSta = WaitData;
					break;
				}
				szSendBuf[0] = 0x01;
				short shCRC = CaculateCRC((byte*)m_bytLongCmdBuf, dwRcvlen);	// ����CRC16У���
				if( m_bytCRC[0] != ((shCRC >> 8) & 0xff) || m_bytCRC[1] != (shCRC & 0xff))
				{
					szSendBuf[0] = 0x03;
					dwSendLen = 1;
					m_bytSta = EndRcv;
					break;
				}
				G_LocalToNet((char*)&dwSEQ, szAppBuf, 4);
				memcpy(szAppBuf + 4, m_bytLongCmdBuf, dwRcvlen);
				g_objApplication.m_objRecvDataMng.PushData(LV2, dwRcvlen + 4, szAppBuf, dwPacketNum);
				m_bytSta = EndRcv;  
			}
			break;
		case EndRcv:
			{
				bfirstpack = true;
				bRcving =false;
				usSendNum = 0;
				m_dwSEQ = 0;
				m_bytCurPackNum = 0;
				m_bytPackTotal = 0;
				memset(m_bytCRC, 0, 2);
				memset(m_bLongCmdPackSta, 0, sizeof(m_bLongCmdPackSta));
				g_objApplication.SendAppFrame(MODE_E, 0xc2, false, dwSEQ, szSendBuf, dwSendLen);
				m_bytSta = WaitData;
			}
			break;
		default:
			break;
		}
		usleep(dwSleepTm);
	}
}
#endif











