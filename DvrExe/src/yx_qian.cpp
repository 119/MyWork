#include "yx_common.h"

int	gFupdate = -1;
BYTE	gbytTempBuf[10500] = {0};
int	giTempLen = 0;
BYTE	gbytDetranBuf[10500] = {0};
int	giDetranLen = 0;

// ��Ƶ���: ����������1��+���������ͣ�1��+ �������ȣ�1��+ ������N����*  M
int Deal3801(char *v_databuf, int v_datalen)
{
	RenewMemInfo(0x02, 0x02, 0x02, 0x00);
	
	char  szBuf[128] = {0};
	DWORD dwPacketNum = 0;
	
	int iRet;
	int i = 0;
	BYTE byt = 0;
	BYTE bytResType = 1;
	BYTE bytParCt	= 0;
	BYTE bytParNo	= 0;
	BYTE bytParType	= 0;
	BYTE bytParLen = 0;
	BYTE bytChnNo	= 0;
	BYTE m_sendbuf[100] = {0};

	g_objAVUplBuf.VUpload = FALSE;
	g_objAVUplBuf.AUpload = FALSE;

	//������� 0x01������Ƶ��0x02������Ƶ��0x03��������Ƶ
	if( v_databuf[0] & 0x01 )
		g_objAVUplBuf.VUpload = TRUE;
	if( v_databuf[0] & 0x02 )
		g_objAVUplBuf.AUpload = TRUE;
 	
 	if( g_objAVUplBuf.VUpload&&!g_objAVUplBuf.AUpload )
 	{
 		g_objMvrCfg.AVUpload.AStart[0] = FALSE;
 		g_objMvrCfg.AVUpload.AStart[1] = FALSE;
 		g_objMvrCfg.AVUpload.AStart[2] = FALSE;
 		g_objMvrCfg.AVUpload.AStart[3] = FALSE;
 	}
 	else if( !g_objAVUplBuf.VUpload&&g_objAVUplBuf.AUpload )
 	{
 		g_objMvrCfg.AVUpload.VStart[0] = FALSE;
		g_objMvrCfg.AVUpload.VStart[1] = FALSE;
		g_objMvrCfg.AVUpload.VStart[2] = FALSE;
		g_objMvrCfg.AVUpload.VStart[3] = FALSE;
 	}
 	
	// ��Ƶ��������
	if( g_objAVUplBuf.VUpload )
	{
		g_bUploadOnlyISlice = false;
		
// 		// ֪ͨSockServExe�ԽϿ����ʷ���
// 		char buf = 0x07;
// 		DataPush(&buf, 1, DEV_DVR, DEV_SOCK, LV3);

		// ��������
		bytParNo =(BYTE)(v_databuf[1]);
		if( bytParNo < 2)
		{
			bytResType = 2;
			PRTMSG(MSG_ERR, "3801 error 1\n");
			goto DEAL3801_END;
		}

		// ��ѯ����
		byt = 2;
		for(bytParCt = 0; bytParCt < bytParNo; bytParCt++)
		{
			// �����ߴ���
			if( (int)(byt + 2) > v_datalen )
			{
				bytResType = 2;
				PRTMSG(MSG_ERR, "3801 error 2\n");
				goto DEAL3801_END;
			}

			// �������ͺͳ���
			bytParType =(BYTE)( v_databuf[ byt ] );
			bytParLen  = (BYTE)( v_databuf[ byt + 1 ] );
			
			// �����ߴ��ٴμ��
			if( (int)(byt + 2 + bytParLen) > v_datalen )
			{
				bytResType = 2;
				PRTMSG(MSG_ERR, "3801 error 3\n");
				goto DEAL3801_END;
			}

			switch(bytParType)
			{
				case 1: // �ֱ���
						// 0x01��352��288
						// 0x02��176��144
						// 0x03��128��96���ݲ�֧�֣�
					{
// 						if( 1 == v_databuf[byt+2] )
// 							g_objMvrCfg.AVUpload.VMode = CIF;
// 						else if( 2 == v_databuf[byt+2] )
// 							g_objMvrCfg.AVUpload.VMode = QCIF;
// 						else
// 						{
// 							bytResType = 2;
// 							PRTMSG(MSG_ERR, "3801 error 4\n");
// 							goto DEAL3801_END;
// 						}

						g_objMvrCfg.AVUpload.VMode = CIF;
					}
					break;
		
				case 2:	// ͨ���ţ�0000ABCD
						// A��1--����ͨ��3�� 0--������
						// B��1--����ͨ��2�� 0--������
						// C��1--����ͨ��1�� 0--������
						// D��1--����ͨ��0�� 0--������
					{
						bytChnNo = 0x0f & v_databuf[byt+2];

						if( bytChnNo == 0 )
						{
							bytResType = 2;
							PRTMSG(MSG_ERR, "3801 error 5\n");
							goto DEAL3801_END;
						}
#if USE_PROTOCOL == 2
						if( 0x01 & bytChnNo )
							g_objMvrCfg.AVUpload.VStart[0] = TRUE;
						else if( 0x02 & bytChnNo )
							g_objMvrCfg.AVUpload.VStart[1] = TRUE;
						else if( 0x04 & bytChnNo )
							g_objMvrCfg.AVUpload.VStart[2] = TRUE;
						else if( 0x08 & bytChnNo )
							g_objMvrCfg.AVUpload.VStart[3] = TRUE;
#endif
						
#if USE_PROTOCOL == 0
						g_objMvrCfg.AVUpload.VStart[0] = ( (0x01 & bytChnNo) ? TRUE : FALSE );
						g_objMvrCfg.AVUpload.VStart[1] = ( (0x02 & bytChnNo) ? TRUE : FALSE );
						g_objMvrCfg.AVUpload.VStart[2] = ( (0x04 & bytChnNo) ? TRUE : FALSE );
						g_objMvrCfg.AVUpload.VStart[3] = ( (0x08 & bytChnNo) ? TRUE : FALSE );
#endif
					}
					break;
		
				case 3:	// ��������
					{
// 						switch( v_databuf[byt+2] )
// 						{
// 							case 0x00:
// 								g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_0;
// 								break;
// 							case 0x01:
// 								g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_1;
// 								break;
// 							case 0x02:
// 								g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_2;
// 								break;
// 							case 0x03:
// 								g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_3;
// 								break;
// 							case 0x04:
// 								g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_4;
// 								break;
// 							case 0x05:
// 								g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_5;
// 								break;
// 							case 0x06:
// 								g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_6;
// 								break;
// 							case 0x07:
// 								g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_7;
// 								break;
// 							default:
// 								{
// 									bytResType = 2;
// 									PRTMSG(MSG_ERR, "3801 error 6\n");
// 									goto DEAL3801_END;
// 								}
// 								break;
// 						}

						if (NETWORK_TYPE == NETWORK_WCDMA || NETWORK_TYPE == NETWORK_EVDO)
						{
							g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_6;
						}
						else if (NETWORK_TYPE == NETWORK_TD)
						{
							g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_2;
						}

						for(i=1; i<4; i++)
							g_objMvrCfg.AVUpload.VBitrate[i] = g_objMvrCfg.AVUpload.VBitrate[0];
					}
					break;

				case 4: // �ɼ�֡��
					{
						if (NETWORK_TYPE == NETWORK_TD)
						{
							if( 1 <= v_databuf[byt+2] && 25 >= v_databuf[byt+2] )
							{
								g_objMvrCfg.AVUpload.VFramerate[0] = v_databuf[byt+2];
								g_objMvrCfg.AVUpload.VFramerate[1] = v_databuf[byt+2];
								g_objMvrCfg.AVUpload.VFramerate[2] = v_databuf[byt+2];
								g_objMvrCfg.AVUpload.VFramerate[3] = v_databuf[byt+2];
							}
							else	
							{
								bytResType = 2;
								PRTMSG(MSG_ERR, "3801 error 7\n");
								goto DEAL3801_END;
							}
						}
						else if (NETWORK_TYPE == NETWORK_EVDO || NETWORK_TYPE == NETWORK_WCDMA)
						{
							g_objMvrCfg.AVUpload.VFramerate[0] = 10;
							g_objMvrCfg.AVUpload.VFramerate[1] = 10;
							g_objMvrCfg.AVUpload.VFramerate[2] = 10;
							g_objMvrCfg.AVUpload.VFramerate[3] = 10;
						}
					}
					break;
		
				case 5:	
					{
						g_objMvrCfg.AVUpload.UploadSecond = 0;
						
						// ʱ����
						//if(  v_databuf[byt+2] <= 23 && v_databuf[byt+3] <= 59 && v_databuf[byt+4] <= 59 )
						if(1)
						{
							g_objMvrCfg.AVUpload.UploadSecond += 3600*v_databuf[byt+2] + 60*v_databuf[byt+3] + v_databuf[byt+4];
							PRTMSG(MSG_DBG, "video mon time: %d s, hour:%d, min:%d, sec:%d\n", g_objMvrCfg.AVUpload.UploadSecond, v_databuf[byt+2], v_databuf[byt+3], v_databuf[byt+4] );
						}
						else
						{
							bytResType = 2;
							PRTMSG(MSG_ERR, "3801 error 8\n");
							goto DEAL3801_END;
						}

						if(g_objMvrCfg.AVUpload.UploadSecond == 0)
						{
							bytResType = 2;
							PRTMSG(MSG_ERR, "3801 error 9\n");
							goto DEAL3801_END;
						}
					}
					break;
					
				default:
					break;			
			}
			
			byt += 2 + bytParLen; // ׼���´�ѭ��
		}
	}

	// ���ݼ��ͨ�������������ֱ��ʺ�����
	int iChnCnt = (g_objMvrCfg.AVUpload.VStart[0] == TRUE) + (g_objMvrCfg.AVUpload.VStart[1] == TRUE) +
		(g_objMvrCfg.AVUpload.VStart[2] == TRUE) + (g_objMvrCfg.AVUpload.VStart[3] == TRUE);
	
	if( iChnCnt > 3)
	{
		g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_3;
	}
	
	if( (iChnCnt > 1 && NETWORK_TYPE == NETWORK_TD)	|| 
		(iChnCnt > 3 && (NETWORK_TYPE == NETWORK_WCDMA || NETWORK_TYPE == NETWORK_EVDO)) )
	{
		g_objMvrCfg.AVUpload.VMode = QCIF;
	}
	
	// ��Ƶ��������
	if( g_objAVUplBuf.AUpload )
	{
		g_objMvrCfg.AVUpload.AStart[0] = TRUE;

// 		// ��������
// 		bytParNo =(BYTE)(v_databuf[byt]);
// 		
// 		// ��ѯ����
// 		byt++;
// 		for(bytParCt = 0; bytParCt < bytParNo; bytParCt++)
// 		{
// 			// �����ߴ���
// 			if( (int)(byt + 2) > v_datalen )
// 			{
// 				bytResType = 2;
// 				goto DEAL3801_END;
// 			}
// 			
// 			// �������ͺͳ���
// 			bytParType =(BYTE)( v_databuf[ byt ] );
// 			bytParLen  = (BYTE)( v_databuf[ byt + 1 ] );
// 			
// 			// �����ߴ��ٴμ��
// 			if( (int)(byt + 2 + bytParLen) > v_datalen )
// 			{
// 				bytResType = 2;
// 				goto DEAL3801_END;
// 			}
// 			
// 			// ������Ƶͨ���š��������ڶ��ѹ̶���������Բ��������ж�
// 			switch(bytParType)
// 			{
// 				case 1:	// ͨ���ţ�0000ABCD
// 					g_objMvrCfg.AVUpload.AStart[0] = TRUE;
// 					break;
// 				case 2:	// ��Ƶ����
// 					break;
// 				default:
// 					break;
// 			}
// 		}
	}

	// �����Ѿ�������ͨ��������������֡�ʡ�����
	if( FALSE == ResetUploadPara(g_objMvrCfg.AVUpload.VFramerate[0]) )
	{
		bytResType = 3;
		PRTMSG(MSG_ERR, "3801 error 10\n");
		goto DEAL3801_END;
	}

DEAL3801_END:
	byt = 0;
	m_sendbuf[byt++] = 0x01;	// 0x01 ��ʾ�����ķ��͵�Э��֡
	m_sendbuf[byt++] = 0x38;
	m_sendbuf[byt++] = 0x41;
	m_sendbuf[byt++] = bytResType;
	
#if USE_VIDEO_TCP == 0
	DataPush(m_sendbuf, (DWORD)byt, DEV_DVR, DEV_QIAN, LV2);
#endif

#if USE_VIDEO_TCP == 1
	char szbuf[1024] = {0};
	int  ilen = 0;
	g_objSock.MakeSmsFrame((char*)&bytResType, 1, 0x38, 0x41, (char*)szbuf, sizeof(szbuf), ilen);
	g_objSock.SendTcpData(szbuf, ilen);
#endif

	return bytResType;
}

// �������
int Deal3802(char *v_databuf, int v_datalen, WORK_EVENT *v_objWorkEvent)
{
	byte bytChnNo;

	if( 0 == v_datalen )
		bytChnNo = 0x0f;
	else
		bytChnNo = 0x0f & v_databuf[0];
	
	if( 0x01 & bytChnNo )
		g_objMvrCfg.AVUpload.VStart[0] = FALSE;
	if( 0x02 & bytChnNo )
		g_objMvrCfg.AVUpload.VStart[1] = FALSE;
	if( 0x04 & bytChnNo )
		g_objMvrCfg.AVUpload.VStart[2] = FALSE;
	if( 0x08 & bytChnNo )
		g_objMvrCfg.AVUpload.VStart[3] = FALSE;
	
	if( !g_objMvrCfg.AVUpload.VStart[0] && !g_objMvrCfg.AVUpload.VStart[1] &&
		!g_objMvrCfg.AVUpload.VStart[2] && !g_objMvrCfg.AVUpload.VStart[3] )
	{
		g_objAVUplBuf.AUpload = FALSE;
		g_objAVUplBuf.VUpload = FALSE;

		// ֪ͨSockServExe�ѷ��ͻ��������
		char buf = 0x0a;
		DataPush((void*)&buf, 1, DEV_DVR, DEV_SOCK, LV2);

		if(g_enumSysSta == WORK)
		{
			v_objWorkEvent->dwUplStopEvt |= EVENT_TYPE_CENT;
		}
	}

	// Ӧ��
	char m_sendbuf[4] = {0};
	int  len = 0;
	m_sendbuf[len++] = 0x01;	// 0x01 ��ʾ�����ķ��͵�����֡
	m_sendbuf[len++] = 0x38;
	m_sendbuf[len++] = 0x42;
	m_sendbuf[len++] = 0x01;
	if(0 != v_datalen)
		m_sendbuf[len++] = v_databuf[0];
	
#if USE_VIDEO_TCP == 0
	DataPush(m_sendbuf, (DWORD)len, DEV_DVR, DEV_QIAN, LV2);
#endif

#if USE_VIDEO_TCP == 1
	char szbuf[1024] = {0};
	int  ilen = 0;
	g_objSock.MakeSmsFrame(m_sendbuf+3, len-3, 0x38, 0x42, (char*)szbuf, sizeof(szbuf), ilen);
	g_objSock.SendTcpData(szbuf, ilen);
#endif

	return 1;
}

// �޸ļ�ز���
int Deal3803(char *v_databuf, int v_datalen)
{
	return 0;

	int  byt = 0;
	int  iret = 0;
	BYTE bytResType = 1;
	BYTE m_sendbuf[100] = {0};

	PrintString(v_databuf, v_datalen);

	// ���ݵ�ǰ��������ͨ����������������ĵ���֡��
	int iChnNum = 0;
	for(byt=0; byt<4; byt++)
	{
		if( g_objMvrCfg.AVUpload.VStart[byt] )
			iChnNum++;
	}
	if( iChnNum == 4 )		// ����������·�ϴ�����ǿ��֡��Ϊ1
	{
		v_databuf[0] = 1;	
	}
	else if(iChnNum == 3)	// ����������·�ϴ��������֡��Ϊ2
	{
		if( v_databuf[0] > 2 )	v_databuf[0] = 2;
	}
	else if(iChnNum == 2)	// ����������·�ϴ��������֡��Ϊ5
	{
		if( v_databuf[0] > 5 )	v_databuf[0] = 5;
	}


	// ����ǰ�����ϴ����򷵻ش���Ӧ��
	if(	 g_objMvrCfg.AVUpload.VStart[0] == FALSE 
		&& g_objMvrCfg.AVUpload.VStart[1] == FALSE
		&& g_objMvrCfg.AVUpload.VStart[2] == FALSE
		&& g_objMvrCfg.AVUpload.VStart[3] == FALSE
		)
	{
		bytResType = 3;
		PRTMSG(MSG_ERR, "Now is not Monitoring!\n");
		goto DEAL3803_END;
	}

	// ����������
	if( v_datalen != 2)
	{
		bytResType = 2;
		PRTMSG(MSG_ERR, "3803 Para error\n");
		goto DEAL3803_END;
	}

// 	// ֡��
// 	if( 1 <= v_databuf[0] && 25 >= v_databuf[0] )
// 	{
// 		g_objMvrCfg.AVUpload.VFramerate[0]
// 		= g_objMvrCfg.AVUpload.VFramerate[1]
// 		= g_objMvrCfg.AVUpload.VFramerate[2]
// 		= g_objMvrCfg.AVUpload.VFramerate[3]
// 		= v_databuf[0];
// 	}
// 	else
// 	{
// 		PRTMSG(MSG_ERR, "3803 FrameRate error\n");
// 		bytResType = 2;
// 		goto DEAL3803_END;
// 	}
// 
// 	// ����
// 	switch( v_databuf[1] )
// 	{
// 		case 0x00:
// 			g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_0;
// 			break;
// 		case 0x01:
// 			g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_1;
// 			break;
// 		case 0x02:
// 			g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_2;
// 			break;
// 		case 0x03:
// 			g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_3;
// 			break;
// 		case 0x04:
// 			g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_4;
// 			break;
// 		case 0x05:
// 			g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_5;
// 			break;
// 		case 0x06:
// 			g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_6;
// 			break;
// 		case 0x07:
// 			g_objMvrCfg.AVUpload.VBitrate[0] = BITRATE_UPL_7;
// 			break;
// 		default:
// 			{
// 				bytResType = 2;
// 				PRTMSG(MSG_ERR, "3803 BitRate error\n");
// 				goto DEAL3803_END;
// 			}
// 			break;
// 	}

	if( FALSE == ResetUploadPara(v_databuf[0]) )
	{
		bytResType = 0x03;
		PRTMSG(MSG_ERR, "3803 Reset Para error\n");
		goto DEAL3803_END;
	}

DEAL3803_END:
// 	byt = 0;
// 	m_sendbuf[byt++] = 0x01;
// 	m_sendbuf[byt++] = 0x38;
// 	m_sendbuf[byt++] = 0x43;
// 	m_sendbuf[byt++] = bytResType;
// 
// 	DataPush(m_sendbuf, (DWORD)byt, DEV_DVR, DEV_QIAN, LV2);

	return 1;
}

// �������������ϴ���ͨ�������ʡ�֡��
BOOL ResetUploadPara(byte v_bytISlice)
{
	int iret = 0, i = 0;

	for(i = 0; i < 4; i++)
	{
		if( g_objWorkStart.VUpload[i] && g_objMvrCfg.AVUpload.VStart[i] )
		{
#if VEHICLE_TYPE == VEHICLE_M
			iret = HI_VENC_SetSingleAttr(2*i+1, VENC_TARGET_FRAMERATE, g_objMvrCfg.AVUpload.VFramerate[i]);
			if(iret) return FALSE;
			iret = HI_VENC_SetSingleAttr(2*i+1, VENC_TARGET_BITRATE, g_objMvrCfg.AVUpload.VBitrate[i]);
			if(iret) return FALSE;
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
			VENC_ATTR_H264_S objH264Attr;
			VENC_CHN_ATTR_S objChnAttr;

			objH264Attr.u32Priority = 0;//ͨ�����ȼ�
			objH264Attr.bMainStream = HI_TRUE;//��̬����,����������ʶ(1:������;0:������)
			objH264Attr.bVIField = HI_TRUE;//��̬����,����ͼ���֡����־(1:��;0:֡)
			objH264Attr.bField = HI_FALSE;//��̬����,֡������ģʽ(1:������;0:֡����)
			objH264Attr.bCBR = HI_TRUE;//��̬����,VBR/CBRģʽ(1:CBR;0:VBR)
			objH264Attr.u32PicLevel = 3;//(CBR:���ʲ�����Χ,0~5,0:��SDK������п�������,1~5:��Ӧ���ʲ�����Χ�ֱ�Ϊ10%~50%;VBR:ͼ��������ȼ�,0~5,ֵԽСͼ������Խ��)
			objH264Attr.u32MaxDelay = 10;//��̬����,����ӳ�(��֡Ϊ��λ)
			objH264Attr.bByFrame = HI_TRUE;//��̬����,֡/��ģʽ��ȡ����(1:��֡��ȡ;0:������ȡ)
			objH264Attr.u32ViFramerate = 25;
			objH264Attr.u32PicWidth = ( g_objMvrCfg.AVUpload.VMode == CIF ? 352 : 176);
			objH264Attr.u32PicHeight = ( g_objMvrCfg.AVUpload.VMode == CIF ? 288 : 144);
			objH264Attr.u32BufSize = ( g_objMvrCfg.AVUpload.VMode == CIF ? 352*288*2*2 : 176*144*2*2);
			objH264Attr.u32TargetFramerate = g_objMvrCfg.AVUpload.VFramerate[i];//��̬����,Ŀ��֡��(P��:0~25;N��:0~30)
			objH264Attr.u32Bitrate = g_objMvrCfg.AVUpload.VBitrate[i]/1000;//��̬����,Ŀ������(CBR)/�������(VBR);16K~20M
			//objH264Attr.u32Gop = 10*objH264Attr.u32TargetFramerate;//��̬����,I֡���(0~1000,��֡Ϊ��λ)

			if( v_bytISlice == 10 || v_bytISlice == 5)
			{
				if (NETWORK_TYPE == NETWORK_EVDO || NETWORK_TYPE == NETWORK_WCDMA)
				{
					objH264Attr.u32TargetFramerate = 10;
					objH264Attr.u32Gop = 100;
				}			
				else if (NETWORK_TYPE == NETWORK_TD)
				{
					objH264Attr.u32TargetFramerate = v_bytISlice;
					objH264Attr.u32Gop = 20*v_bytISlice;
				}
				g_bUploadOnlyISlice = false;
				
// 				// ֪ͨSockServExe�ԽϿ����ʷ���
// 				char buf = 0x07;
// 				DataPush(&buf, 1, DEV_DVR, DEV_SOCK, LV3);
			}
			else if(v_bytISlice == 2 || v_bytISlice == 1)
			{
				objH264Attr.u32TargetFramerate = 10;
				objH264Attr.u32Gop = 30;
				
				g_bUploadOnlyISlice = true;
		
// 				// ֪ͨSockServExe�Խ������ʷ���
// 				char buf = 0x08;
// 				DataPush(&buf, 1, DEV_DVR, DEV_SOCK, LV3);
			}

			objChnAttr.enType = PT_H264;
			objChnAttr.pValue = (HI_VOID *)&objH264Attr;

			iret = HI_MPI_VENC_SetChnAttr(2*i+1, &objChnAttr);
			if(iret) 
			{
				PRTMSG(MSG_DBG, "HI_MPI_VENC_SetChnAttr Err, iChn = %d, iret = %08x\n", i, iret);
				return FALSE;
			}
			else
			{
				PRTMSG(MSG_DBG, "HI_MPI_VENC_SetChnAttr succ!\n" );
			}

#endif
		}
	}

	return TRUE;
}

// ��ѯOSD����
int Deal380b(char *v_databuf, int v_datalen)
{
	int  i;
	int  byt = 0;
	BYTE 	m_sendbuf[100] = {0};
	struct  rtc_time objRtcTime;
	tag2DDvrCfg objMvrCfg;
	
	if(GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg)) != 0)
		return 0;

	m_sendbuf[byt++] = 0x01;	// 0x01��ʾ�����ķ��͵�����֡

	m_sendbuf[byt++] = 0x38;
	m_sendbuf[byt++] = 0x4b;
	m_sendbuf[byt++] = v_databuf[0];
	
	if(v_databuf[0] & 0x10) 
	{
		if(GetRtcTime(&objRtcTime) != 0)
			return 0;

		m_sendbuf[byt++] = objRtcTime.tm_year-100;
		m_sendbuf[byt++] = objRtcTime.tm_mon+1;
		m_sendbuf[byt++] = objRtcTime.tm_mday;
		m_sendbuf[byt++] = objRtcTime.tm_hour;
		m_sendbuf[byt++] = objRtcTime.tm_min;
		m_sendbuf[byt++] = objRtcTime.tm_sec;
	}
	
	if(v_databuf[0] & 0x08) 
	{
		m_sendbuf[byt++] = objMvrCfg.TrigPara.TrigType;
		m_sendbuf[byt++] = strcmp("HDisk", objMvrCfg.AVRecord.DiskType) == 0 ? 0x00 : 0x01;
	}
	
	if(v_databuf[0] & 0x04) 
	{
		m_sendbuf[byt++] = objMvrCfg.VInput.VNormal;
		m_sendbuf[byt++] = objMvrCfg.VInput.VMode;
		
		for(i = 0; i < 4; i++)
		{
			switch(objMvrCfg.AVRecord.VBitrate[i])
			{
				case BITRATE_REC_1:
					m_sendbuf[byt++] = 0x00;
					break;
				case BITRATE_REC_2:
					m_sendbuf[byt++] = 0x01;
					break;
				case BITRATE_REC_3:
					m_sendbuf[byt++] = 0x02;
					break;
				case BITRATE_REC_4:
					m_sendbuf[byt++] = 0x03;
					break;
				case BITRATE_REC_5:
					m_sendbuf[byt++] = 0x04;
					break;
				case BITRATE_REC_6:
					m_sendbuf[byt++] = 0x05;
					break;
				case BITRATE_REC_7:
					m_sendbuf[byt++] = 0x06;
					break;
				case BITRATE_REC_8:
					m_sendbuf[byt++] = 0x07;
					break;
				case BITRATE_REC_9:
					m_sendbuf[byt++] = 0x08;
					break;
				default:
					break;
			}
				
			m_sendbuf[byt++] = objMvrCfg.AVRecord.VFramerate[i];
			m_sendbuf[byt++] = objMvrCfg.AVRecord.VStart[i];
			m_sendbuf[byt++] = objMvrCfg.VPreview.VStart[i];
		}
	}
	
	if(v_databuf[0] & 0x02) 
	{
		for(i = 0; i < 6; i++)
		{
			objMvrCfg.PeriodTrig.StartTime[i][2] = 0;
			m_sendbuf[byt++] = atoi(objMvrCfg.PeriodTrig.StartTime[i]);
			objMvrCfg.PeriodTrig.StartTime[i][5] = 0;
			m_sendbuf[byt++] = atoi(objMvrCfg.PeriodTrig.StartTime[i]+3);
			m_sendbuf[byt++] = atoi(objMvrCfg.PeriodTrig.StartTime[i]+6);
			
			objMvrCfg.PeriodTrig.EndTime[i][2] = 0;
			m_sendbuf[byt++] = atoi(objMvrCfg.PeriodTrig.EndTime[i]);
			objMvrCfg.PeriodTrig.EndTime[i][5] = 0;
			m_sendbuf[byt++] = atoi(objMvrCfg.PeriodTrig.EndTime[i]+3);
			m_sendbuf[byt++] = atoi(objMvrCfg.PeriodTrig.EndTime[i]+6);
			
			m_sendbuf[byt++] = objMvrCfg.PeriodTrig.PeriodStart[i];
		}
	}
	
	if(v_databuf[0] & 0x01) 
	{
		m_sendbuf[byt++] = objMvrCfg.EventTrig.AccDelay;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.AccStart;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.MoveDelay;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.MoveStart;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.OverlayDelay;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.OverlayStart;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.S1Delay;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.S1Start;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.S2Delay;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.S2Start;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.S3Delay;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.S3Start;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.S4Delay;
		m_sendbuf[byt++] = objMvrCfg.EventTrig.S4Start;
	}

#if USE_VIDEO_TCP == 0
	DataPush(m_sendbuf, (DWORD)byt, DEV_DVR, DEV_QIAN, LV2);
#endif

#if USE_VIDEO_TCP == 1
	char szbuf[1024] = {0};
	int  ilen = 0;
	g_objSock.MakeSmsFrame((char*)m_sendbuf+3, byt-3, 0x38, 0x4b, (char*)szbuf, sizeof(szbuf), ilen);
	g_objSock.SendTcpData(szbuf, ilen);
#endif

	return 1;
}

// �޸�OSD����
int Deal380c(char *v_databuf, int v_datalen)
{
	int  i;
	int  byt = 1;
	int  iret = 0;
	BYTE 	bytResType = 1;
	BYTE 	m_sendbuf[100] = {0};
	struct  rtc_time objRtcTime;
	tag2DDvrCfg objMvrCfg;
	
	if(GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg)) != 0)
	{
		bytResType = 3;
		goto DEAL380c_END;
	}

	PRTMSG(MSG_DBG, "Deal 380C: ");
	PrintString(v_databuf, v_datalen);

	if(v_databuf[0] & 0x10) 
	{
		if(GetRtcTime(&objRtcTime) != 0)
		{
			bytResType = 3;
			goto DEAL380c_END;
		}

		if(v_databuf[byt] <= 99)
		{
			objRtcTime.tm_year = v_databuf[byt++]+2000;
		}
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}

		if(1 <= v_databuf[byt] && v_databuf[byt] <= 12)
			objRtcTime.tm_mon = v_databuf[byt++]-1;
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
			
		if(1 <= v_databuf[byt] && v_databuf[byt] <= 31)
			objRtcTime.tm_mday = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(v_databuf[byt] <= 23)
			objRtcTime.tm_hour = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
			
		if(v_databuf[byt] <= 59)
			objRtcTime.tm_min = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
			
		if(v_databuf[byt] <= 59)
			objRtcTime.tm_sec = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}

		if( SetRtcTime(&objRtcTime) != 0 )
		{
			bytResType = 3;
			goto DEAL380c_END;
		}
	}
	
	if(v_databuf[0] & 0x08) 
	{
		if(v_databuf[byt] <= 3)
			objMvrCfg.TrigPara.TrigType = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}

		switch(v_databuf[byt++])
		{
			case 0x00:
				strcpy(objMvrCfg.AVRecord.DiskType, "HDisk");
				break;
			case 0x01:
				strcpy(objMvrCfg.AVRecord.DiskType, "SDisk");
				break;
			default:
				bytResType = 2;
				goto DEAL380c_END;
				break;
		}
	}
	
	if(v_databuf[0] & 0x04) 
	{
		if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
			objMvrCfg.VInput.VNormal = objMvrCfg.VOutput.VNormal = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if( 1 <= v_databuf[byt] && v_databuf[byt] <= 4)
#if VEHICLE_TYPE == VEHICLE_M
			objMvrCfg.VInput.VMode = objMvrCfg.VOutput.VMode = objMvrCfg.AVRecord.VMode = v_databuf[byt++];
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
			objMvrCfg.AVRecord.VMode[0] = objMvrCfg.AVRecord.VMode[1] = objMvrCfg.AVRecord.VMode[2] = objMvrCfg.AVRecord.VMode[3] = v_databuf[byt++];
#endif
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		for(i = 0; i < 4; i++)
		{
			switch(v_databuf[byt++])
			{
				case 0x00:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_1;
					break;
				case 0x01:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_2;
					break;
				case 0x02:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_3;
					break;
				case 0x03:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_4;
					break;
				case 0x04:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_5;
					break;
				case 0x05:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_6;
					break;
				case 0x06:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_7;
					break;
				case 0x07:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_8;
					break;
				case 0x08:
					objMvrCfg.AVRecord.VBitrate[i] = BITRATE_REC_9;
					break;
				default:
					bytResType = 2;
					goto DEAL380c_END;
					break;
			}
				
			if(1 <= v_databuf[byt] && v_databuf[byt] <= 20)
				objMvrCfg.AVRecord.VFramerate[i] = v_databuf[byt++];
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
			
			if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
				objMvrCfg.AVRecord.VStart[i] = (BOOL)v_databuf[byt++];
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
			
			if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
				objMvrCfg.VPreview.VStart[i] = (BOOL)v_databuf[byt++];
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
		}
	}
	
	if(v_databuf[0] & 0x02) 
	{
		for(i = 0; i < 6; i++)
		{
			if(v_databuf[byt] <= 23)
				sprintf(objMvrCfg.PeriodTrig.StartTime[i], "%02d:", v_databuf[byt++]);
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
			
			if(v_databuf[byt] <= 59)
				sprintf(objMvrCfg.PeriodTrig.StartTime[i]+3, "%02d:", v_databuf[byt++]);
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
			
			if(v_databuf[byt] <= 59)
				sprintf(objMvrCfg.PeriodTrig.StartTime[i]+6, "%02d", v_databuf[byt++]);
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
				
			if(v_databuf[byt] <= 23)
				sprintf(objMvrCfg.PeriodTrig.EndTime[i], "%02d:", v_databuf[byt++]);
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
			
			if(v_databuf[byt] <= 59)
				sprintf(objMvrCfg.PeriodTrig.EndTime[i]+3, "%02d:", v_databuf[byt++]);
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
			
			if(v_databuf[byt] <= 59)
				sprintf(objMvrCfg.PeriodTrig.EndTime[i]+6, "%02d", v_databuf[byt++]);
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
			
			if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
				objMvrCfg.PeriodTrig.PeriodStart[i] = (BOOL)v_databuf[byt++];
			else
			{
				bytResType = 2;
				goto DEAL380c_END;
			}
		}
	}
	
	if(v_databuf[0] & 0x01) 
	{
		if(1 <= v_databuf[byt] && v_databuf[byt] <= 60)
			objMvrCfg.EventTrig.AccDelay = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
			objMvrCfg.EventTrig.AccStart = (BOOL)v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
			
		if(1 <= v_databuf[byt] && v_databuf[byt] <= 60)
			objMvrCfg.EventTrig.MoveDelay = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
			objMvrCfg.EventTrig.MoveStart = (BOOL)v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(1 <= v_databuf[byt] && v_databuf[byt] <= 60)
			objMvrCfg.EventTrig.OverlayDelay = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
			objMvrCfg.EventTrig.OverlayStart = (BOOL)v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(1 <= v_databuf[byt] && v_databuf[byt] <= 60)
			objMvrCfg.EventTrig.S1Delay = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
			objMvrCfg.EventTrig.S1Start = (BOOL)v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(1 <= v_databuf[byt] && v_databuf[byt] <= 60)
			objMvrCfg.EventTrig.S2Delay = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
			objMvrCfg.EventTrig.S2Start = (BOOL)v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(1 <= v_databuf[byt] && v_databuf[byt] <= 60)
			objMvrCfg.EventTrig.S3Delay = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
			objMvrCfg.EventTrig.S3Start = (BOOL)v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(1 <= v_databuf[byt] && v_databuf[byt] <= 60)
			objMvrCfg.EventTrig.S4Delay = v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
		
		if(v_databuf[byt] == 0 || v_databuf[byt] == 1)
			objMvrCfg.EventTrig.S4Start = (BOOL)v_databuf[byt++];
		else
		{
			bytResType = 2;
			goto DEAL380c_END;
		}
	}
	
	if(SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg)) != 0)
	{
		char by = 0x0b;
		DataPush(&by, 1, DEV_DIAODU, DEV_DVR, LV1);		
								
		bytResType = 3;
		goto DEAL380c_END;
	}

DEAL380c_END:
	byt = 0;
	m_sendbuf[byt++] = 0x01;

	m_sendbuf[byt++] = 0x38;
	m_sendbuf[byt++] = 0x4c;
	m_sendbuf[byt++] = bytResType;

#if USE_VIDEO_TCP == 0
	DataPush(m_sendbuf, (DWORD)byt, DEV_DVR, DEV_QIAN, LV2);
#endif

#if USE_VIDEO_TCP == 1
	char szbuf[1024] = {0};
	int  ilen = 0;
	g_objSock.MakeSmsFrame((char*)&bytResType, 1, 0x38, 0x4c, (char*)szbuf, sizeof(szbuf), ilen);
	g_objSock.SendTcpData(szbuf, ilen);
#endif
	
	return 1;
}

void DealQianSmsData(char *v_szBuf, int v_iLen, SLEEP_EVENT *v_objSleepEvent, WORK_EVENT *v_objWorkEvent, CTRL_EVENT *v_objCtrlEvent)
{
	int iRet;
	
	for( int i = 1; i < v_iLen; i++)
	{
		if( v_szBuf[i] == 0x7f )
			v_szBuf[i] = 0;
	}

	switch( v_szBuf[0] )
	{
		case 0x01:	// ��Ƶ�������
			{
				if( Deal3801(v_szBuf+1, v_iLen-1) == 1 )
				{
					switch(g_enumSysSta)
					{
						case SLEEP:
						{
							v_objSleepEvent->dwStopEvt |= EVENT_TYPE_CENT;
						}
						case IDLE:
						case WORK:
						{
	//						v_objWorkEvent->dwRecStartEvt |= EVENT_TYPE_CENT;
							v_objWorkEvent->dwUplStartEvt |= EVENT_TYPE_CENT;
						}
							break;
						default:
							break;
					}		
				}
			}
			break;

		case 0x02:	// �����������
			{
				Deal3802(v_szBuf+1, v_iLen-1, v_objWorkEvent);			
			}
			break;

		case 0x03:	// �޸ļ�ز���
			{
				Deal3803(v_szBuf+1, v_iLen-1);
			}
			break;

// 		case 0x35:
// 			{
// 				Deal3835(v_szBuf+1, v_iLen-1);
// 			}
// 			break;

#if VIDEO_BLACK_TYPE == 2
		case 0x09:
			{
				Deal3809(v_szBuf+1, v_iLen-1);
			}
			break;
#endif

		case 0x0A:	// ��ϻ���ϴ�����
			{
				g_bDeal380A = true;
				g_iDeal380A = v_iLen-1;
				memcpy(g_szDeal380A, v_szBuf+1, v_iLen-1);
			}
			break;

		case 0x0b:	// ��ѯOSD����
			{
				Deal380b(v_szBuf+1, v_iLen-1);
			}
			break;

		case 0x0c:	// �޸�OSD����
			{
				Deal380c(v_szBuf+1, v_iLen-1);
			}
			break;		

		default:
			break;
	}
}
