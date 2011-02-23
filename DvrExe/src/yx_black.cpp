#include "yx_common.h"

#if VIDEO_BLACK_TYPE == 1

int UPLOAD_PACK_SIZE;

pthread_t g_pthreadBlack;
UpLoadFileSta g_objUploadFileSta;
BYTE	g_szA1Buf[100] = {0};
int		g_iA1Len = 0;

// ͨ���ţ�1���� �¼����ͣ�1���� ��ʼ���ڣ�3���� ��ʼʱ�䣨3���� �������ڣ�3��������ʱ�䣨3��
int Deal380A(char *v_databuf, int v_datalen)
{
	int		i = 0;
	BYTE	szResBuf[100] = {0};
	int		iResLen = 0;
	BYTE	szResType = 1;
	char	szTempBuf[3] = {0};
	
	BLACK_QUERY_PARA  objQueryPara;
	char szStartTime[9] = {0};
	char szEndTime[9] = {0};

	int  iFileCount = 0;

	// ����ǰ���ڽ��м�أ���֧�ֺ�ϻ�Ӳ�ѯ
	if(	g_objWorkStart.VUpload[0] || g_objWorkStart.VUpload[1] || g_objWorkStart.VUpload[2] || g_objWorkStart.VUpload[3] )
	{
		szResType = 5;
		goto DEAL380A_END;
	}

	memset((void*)&g_objUploadFileSta, 0, sizeof(g_objUploadFileSta));

	iFileCount = g_objUploadFileSta.m_iFileCount;

	// ����������
	if( v_datalen != 14 )
	{
		PRTMSG(MSG_ERR, "380A uncorrect len\n");
		szResType = 2;
		goto DEAL380A_END;
	}

	// ͨ����
	objQueryPara.m_uszChnNo = v_databuf[0];

	// �¼�����
	objQueryPara.m_uszEventType = v_databuf[1];

	// ���ڣ��������޶�����ѯ����ֻ����ͬһ�죩
	sprintf(objQueryPara.m_szDate, "%04d-%02d-%02d", 2000+v_databuf[2], v_databuf[3], v_databuf[4]);

	// ��ʼʱ��
	sprintf(objQueryPara.m_szStartTime, "%02d:%02d:%02d", v_databuf[5], v_databuf[6], v_databuf[7]);
	sprintf(szStartTime, "%02d:%02d:%02d", v_databuf[5], v_databuf[6], v_databuf[7]);

	// ����ʱ��
	sprintf(objQueryPara.m_szEndTime, "%02d:%02d:%02d", v_databuf[11], v_databuf[12], v_databuf[13]);
	sprintf(szEndTime, "%02d:%02d:%02d", v_databuf[11], v_databuf[12], v_databuf[13]);

	// ��ѯ
	iFileCount = QueryBlackData(objQueryPara, szStartTime, szEndTime, iFileCount);

	if( 0 < iFileCount )
	{
		PRTMSG(MSG_NOR, "Query video black succ, Find %d files\n", iFileCount);

		g_objUploadFileSta.m_iFileCount += iFileCount;
		szResType = 1;
	}
	else if( 0 == iFileCount )
	{
		PRTMSG(MSG_DBG, "Query black failed, No such file\n");
		szResType = 3;
	}
	else
	{
		PRTMSG(MSG_DBG, "Query black failed, unknown reason\n");
		szResType = 4;
	}

	for(i=0; i<g_objUploadFileSta.m_iFileCount; i++)
	{
		PRTMSG(MSG_DBG, "The %d file:\n", i);
		PRTMSG(MSG_DBG, "            TempName:%s\n", g_objUploadFileSta.m_szTempFileName[i]);
		PRTMSG(MSG_DBG, "            RealName:%s\n", g_objUploadFileSta.m_szRealFileName[i]);
		PRTMSG(MSG_DBG, "            Start: %d\n", g_objUploadFileSta.m_ulStartOffset[i]);
		PRTMSG(MSG_DBG, "            End:   %d\n", g_objUploadFileSta.m_ulEndOffset[i]);
	}

DEAL380A_END:
	szResBuf[iResLen++] = 0x01;		// 0x01 ��ʾ�����ķ��͵�����֡

	szResBuf[iResLen++] = 0x38;
	szResBuf[iResLen++] = 0x4A;
	szResBuf[iResLen++] = szResType;
	if( szResType == 1)
	{
		szResBuf[iResLen++] = objQueryPara.m_uszEventType;	// �¼�����
		szResBuf[iResLen++] = v_databuf[2];		// ��
		szResBuf[iResLen++] = v_databuf[3];		// ��
		szResBuf[iResLen++] = v_databuf[4];		// ��
		
		memcpy(szTempBuf,szStartTime,2);
		szResBuf[iResLen++] = (BYTE)(atoi(szTempBuf));	//��ʼʱ

		memcpy(szTempBuf,szStartTime+3,2);
		szResBuf[iResLen++] = (BYTE)(atoi(szTempBuf));	//��ʼ��

		memcpy(szTempBuf,szStartTime+6,2);
		szResBuf[iResLen++] = (BYTE)(atoi(szTempBuf));	//��ʼ��

		szResBuf[iResLen++] = v_databuf[2];		// ��
		szResBuf[iResLen++] = v_databuf[3];		// ��
		szResBuf[iResLen++] = v_databuf[4];		// ��

		memcpy(szTempBuf,szEndTime,2);
		szResBuf[iResLen++] = (BYTE)(atoi(szTempBuf));	//��ֹʱ

		memcpy(szTempBuf,szEndTime+3,2);
		szResBuf[iResLen++] = (BYTE)(atoi(szTempBuf));	//��ֹ��

		memcpy(szTempBuf,szEndTime+6,2);
		szResBuf[iResLen++] = (BYTE)(atoi(szTempBuf));	//��ֹ��

		for(i=iResLen-6; i<iResLen; i++)
			if( 0 == szResBuf[i] )
				szResBuf[i] = 0x7f;
	}
	
#if USE_VIDEO_TCP == 0
	// �Ƶ�QianExe����
// 	PRTMSG(MSG_DBG, "Dvr Push to Qian:");
// 	PrintString((char*)szResBuf, iResLen);
	DataPush(szResBuf, (DWORD)iResLen, DEV_DVR, DEV_QIAN, LV2);
#endif

#if USE_VIDEO_TCP == 1
	char szbuf[1024] = {0};
	int  ilen = 0;
	g_objSock.MakeSmsFrame((char*)szResBuf+3, iResLen-3, 0x38, 0x4A, (char*)szbuf, sizeof(szbuf), ilen);
	g_objSock.SendTcpData(szbuf, ilen);
#endif

	// Ӧ�����ʼ�ϴ���ϻ���ļ�
//	sleep(2);
	BeginUploadFile();
	
	return (int)szResType;
}

// ��ѯ�ļ����������ҵ���Ӧʱ��ε��ļ����򷵻����ҵ����ļ�����
int QueryBlackData(	BLACK_QUERY_PARA v_objQueryPara, char *v_szStartTime, char *v_szEndTime, int v_iFileCnt)
{
	int i=0, j=0;
	
	FILE *pListFile = NULL;
	FILE *pIndexFile = NULL;
	
	char szDatePath[255] = {0};
	char szFileName[255] = {0};

	char szFileStartTime[9] = {0};
	char szFileEndTime[9] = {0};
	char szFindStartTime[9] = {0};
	char szFindEndTime[9] = {0};

	int  iVideoStartOffset = 0;
	int  iAudioStartOffset = 0;
	int	iIndexStartOffset = 0;
	int  iVideoEndOffset = 0;
	int  iAudioEndOffset = 0;
	int	iIndexEndOffset = 0;

	FILE_INDEX objAVFileIndex;
	FRAME_INDEX objAVFrameIndex;
	
	for(i=0; i<4; i++)
	{
		j = 0x01<<i;

		// ��Ҫ��ѯ��i��ͨ��
		if( v_objQueryPara.m_uszChnNo & j )
		{
			memset(szDatePath, 0, sizeof(szDatePath));
			sprintf(szDatePath, "/mnt/%s/part1/data/chn%d/%s", g_objMvrCfg.AVRecord.DiskType, i+1, v_objQueryPara.m_szDate);
			
			// �鿴��Ŀ¼�Ƿ����
			if(access(szDatePath, F_OK) != 0)
			{
				PRTMSG(MSG_ERR, "%s is not exist\n", szDatePath);
				continue;
			}

			// �򿪸�Ŀ¼�������ļ�
			memset(szFileName,0, sizeof(szFileName));
			sprintf(szFileName, "%s/%s.list", szDatePath, v_objQueryPara.m_szDate);
			if( (pListFile = fopen(szFileName, "rb+")) == NULL )
			{
				PRTMSG(MSG_ERR, "open %s failed\n", szFileName);
				continue;
			}

			// ��ÿ�������ļ��������ж�
			while( fread(&objAVFileIndex, sizeof(FILE_INDEX), 1, pListFile) )
			{
				sprintf(szFileStartTime, "%s:%s:%s", objAVFileIndex.StartTime[0], objAVFileIndex.StartTime[1], objAVFileIndex.StartTime[2]);
				sprintf(szFileEndTime, "%s:%s:%s", objAVFileIndex.EndTime[0], objAVFileIndex.EndTime[1], objAVFileIndex.EndTime[2]);

				// ���������ļ���ʱ�������Ҫ��ѯ�ĺ�ϻ��ʱ����н���
				if(
					(strcmp(szFileStartTime, v_objQueryPara.m_szStartTime) >= 0 && strcmp(szFileStartTime, v_objQueryPara.m_szEndTime) <= 0) ||
					(strcmp(szFileStartTime, v_objQueryPara.m_szStartTime) <= 0 && strcmp(szFileEndTime, v_objQueryPara.m_szStartTime) >= 0)
			  	)
				{
					iVideoStartOffset = 0;
					iAudioStartOffset = 0;
					iIndexStartOffset = 0;
					iVideoEndOffset = 0;
					iAudioEndOffset = 0;
					iIndexEndOffset = 0;

					// ѡ�񽻼�����
					if(strcmp(szFileStartTime, v_objQueryPara.m_szStartTime) < 0) 
						memcpy(szFileStartTime, v_objQueryPara.m_szStartTime, 9);
					if(strcmp(szFileEndTime, v_objQueryPara.m_szEndTime) > 0)
						memcpy(szFileEndTime, v_objQueryPara.m_szEndTime,9);

					// �������ļ�
					memset(szFileName, 0, sizeof(szFileName));
					sprintf(szFileName, "/mnt/%s/part1/data/%s", g_objMvrCfg.AVRecord.DiskType,objAVFileIndex.IPath);
					if( NULL == (pIndexFile = fopen(szFileName, "rb+")) )
					{
						PRTMSG(MSG_ERR, "open index file failed: \n");
						continue;
					}

					for( j=0; fread(&objAVFrameIndex, 1, sizeof(FRAME_INDEX), pIndexFile); j++)
					{
						// ��Ƶ����Ƶ�����������ڸ����ļ��е���ʼλ��
						if( strcmp(objAVFrameIndex.CurTime, szFileStartTime) == 0 )
						{
							iVideoStartOffset = objAVFrameIndex.VStartOffset;
							iAudioStartOffset = objAVFrameIndex.AStartOffset;
							iIndexStartOffset = j*sizeof(FRAME_INDEX);
						}

						// ����λ��
						if( strcmp(objAVFrameIndex.CurTime, szFileEndTime) == 0 )
						{
							iVideoEndOffset = objAVFrameIndex.VEndOffset;
							iAudioEndOffset = objAVFrameIndex.AEndOffset;
							iIndexEndOffset = j*sizeof(FRAME_INDEX);
							break;
						}
					}

					//if( 0 == iVideoEndOffset || 0 == iAudioEndOffset || 0 == iIndexEndOffset )
					if( 0 == iVideoEndOffset || 0 == iIndexEndOffset )
					{
						if(pIndexFile)
						{
							fclose(pIndexFile);
							pIndexFile = NULL;
						}
						continue;
					}

					//�ر������ļ�
					if(pIndexFile)
					{
						fclose(pIndexFile);
						pIndexFile = NULL;
					}

					// �ѡ�:���ĳɡ�����
					szFileStartTime[2] = szFileStartTime[5] = '-';
					szFileEndTime[2] = szFileEndTime[5] = '-';

					// ��¼�����Ϣ����ʱ�ļ�������ʽ�ļ�������ʼλ�á�����λ�ã�
					// ����Ӧ��ʱ�����������ʱ�ļ���������ʱ�ļ��������ڣ�ֻ�����ϴ�ʱ���õ�����ʱ�ļ���
					sprintf(g_objUploadFileSta.m_szTempFileName[v_iFileCnt], "%d_%s_%s_%s.ind", i, v_objQueryPara.m_szDate, szFileStartTime, szFileEndTime);
					sprintf(g_objUploadFileSta.m_szRealFileName[v_iFileCnt], "/mnt/%s/part1/data/%s", g_objMvrCfg.AVRecord.DiskType, objAVFileIndex.IPath);
					g_objUploadFileSta.m_ulStartOffset[v_iFileCnt] = iIndexStartOffset;
					g_objUploadFileSta.m_ulEndOffset[v_iFileCnt] = iIndexEndOffset;
					v_iFileCnt++;

					sprintf(g_objUploadFileSta.m_szTempFileName[v_iFileCnt], "%d_%s_%s_%s.264", i, v_objQueryPara.m_szDate, szFileStartTime, szFileEndTime);
					sprintf(g_objUploadFileSta.m_szRealFileName[v_iFileCnt], "/mnt/%s/part1/data/%s", g_objMvrCfg.AVRecord.DiskType, objAVFileIndex.VPath);
					g_objUploadFileSta.m_ulStartOffset[v_iFileCnt] = iVideoStartOffset;
					g_objUploadFileSta.m_ulEndOffset[v_iFileCnt] = iVideoEndOffset;
					v_iFileCnt++;

					if(iAudioEndOffset != 0)//����Ƶ�ļ�Ϊ��ʱ���ϴ�
					{
						sprintf(g_objUploadFileSta.m_szTempFileName[v_iFileCnt], "%d_%s_%s_%s.726", i, v_objQueryPara.m_szDate, szFileStartTime, szFileEndTime);
						sprintf(g_objUploadFileSta.m_szRealFileName[v_iFileCnt], "/mnt/%s/part1/data/%s", g_objMvrCfg.AVRecord.DiskType, objAVFileIndex.APath);
						g_objUploadFileSta.m_ulStartOffset[v_iFileCnt] = iAudioStartOffset;
						g_objUploadFileSta.m_ulEndOffset[v_iFileCnt] = iAudioEndOffset;
						v_iFileCnt++;
					}

					// ��������ѯ����ʱ��Σ��������ҵ�����С�Ŀ�ʼʱ������Ľ���ʱ�䣩
					if( strcmp(szFindStartTime, szFileStartTime) < 0 )
						memcpy(szFindStartTime, szFileStartTime, 9);
					if( strcmp(szFileEndTime, szFindEndTime) > 0 )
						memcpy(szFindEndTime, szFileEndTime, 9);

					// �����ҵ��㹻���ļ����򷵻�
					if( v_iFileCnt >= MAX_UPLOAD_FILE_COUNT )
						goto QUERY_BLACK_END;				
				}
			}

			if(pListFile)
			{
				fclose(pListFile);
				pListFile = NULL;
			}
		}
	}

QUERY_BLACK_END:
	
	if( v_iFileCnt > g_objUploadFileSta.m_iFileCount )	// �����ҵ��ļ�
	{
		// ������ѯ����ʱ��η���
		if( strcmp(v_szStartTime, szFindStartTime) < 0 ) 
			memcpy(v_szStartTime, szFindStartTime, 9);
		if( strcmp(v_szEndTime, szFindEndTime) > 0 )
			memcpy(v_szEndTime, szFindEndTime, 9);
	}

	return (v_iFileCnt - g_objUploadFileSta.m_iFileCount );
}

void BeginUploadFile()
{
	int	i = 0;
	int	iLen = 0;
	
	int iCurDateTime[6];

	BYTE	szTempBuf[100] = {0};
	int  iTempLen = 0;

	// ����ǰ���ڼ�أ��򷵻�
	if(	g_objWorkStart.VUpload[0] || g_objWorkStart.VUpload[1] || g_objWorkStart.VUpload[2] || g_objWorkStart.VUpload[3] )
		return;

	// �����ϴ��ļ���״̬
	if( FALSE == UpdateUploadSta(FALSE) ) 
		return;

	// ��֡��A1֡��
	szTempBuf[iTempLen++] = 0xA1;

	// �ֻ���
	tag1PComuCfg objComuCfg;
	GetImpCfg( &objComuCfg, sizeof(objComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(objComuCfg) );					
	memcpy(szTempBuf+iTempLen, objComuCfg.m_szTel, 15);
	iTempLen += 15;
	
	//�ļ�����
	szTempBuf[iTempLen++] = g_objUploadFileSta.m_uszFileType;		
	
	//�˴���ϻ���ļ����̶���Ϊ34���ֽڣ����油�ո񣬴���40���ֽ�
	iLen = strlen(g_objUploadFileSta.m_szTempFileName[0]);
	memcpy( szTempBuf+iTempLen, g_objUploadFileSta.m_szTempFileName[0], iLen);	
	iTempLen += iLen;
	szTempBuf[iTempLen++] = ' ';szTempBuf[iTempLen++] = ' ';szTempBuf[iTempLen++] = ' ';	
	szTempBuf[iTempLen++] = ' ';szTempBuf[iTempLen++] = ' ';szTempBuf[iTempLen++] = ' ';
	
	// ʱ�����
	GetCurDateTime( iCurDateTime );
	
	szTempBuf[iTempLen++] = (char)(iCurDateTime[0]-2000);
	szTempBuf[iTempLen++] = (char)iCurDateTime[1];
	szTempBuf[iTempLen++] = (char)iCurDateTime[2];
	szTempBuf[iTempLen++] = (char)iCurDateTime[3];
	szTempBuf[iTempLen++] = (char)iCurDateTime[4];
	szTempBuf[iTempLen++] = (char)iCurDateTime[5];
	
	// �ܰ���
	memcpy(szTempBuf+iTempLen, (void*)&g_objUploadFileSta.m_ulPackTotal, 8);
	iTempLen += 8;
	
	// ���ڴ�С��Ĭ��Ϊ32��
	szTempBuf[iTempLen++] = UPLOAD_WINSIZE;
	
	// �Ƶ�SockServExe����
	DataPush(szTempBuf, (DWORD)iTempLen, DEV_DVR, DEV_SOCK, LV2);

	// ���浽���壬�Ա��ش�
	memcpy(g_szA1Buf, szTempBuf, iTempLen);
	g_iA1Len = iTempLen;

 	// ������ʱ�ش���ʱ��
	_SetTimer(&g_objTimerMng, A1_REUPLOAD_TIMER, 8000, DealA1_ReUpload);
}

void DealA1_ReUpload(void *arg, int len)
{
	PRTMSG(MSG_DBG, "ReUpload A1 frame\n");

	DataPush(g_szA1Buf, (DWORD)g_iA1Len, DEV_DVR, DEV_SOCK, LV2);
}

BOOL UpdateUploadSta( BOOL v_bDeleteFirstFile )
{
	int	i=0;
	FILE	*fp = NULL;
	HI_U64  ulFileSize = 0;
	
	while(1)
	{
		if(g_objUploadFileSta.m_iFileCount <= 0)
			return FALSE;
		
		// ��Ҫɾ����һ���ļ�
		if( v_bDeleteFirstFile )
		{
			g_objUploadFileSta.m_iFileCount--;
			
			if(g_objUploadFileSta.m_iFileCount <= 0)
				return FALSE;
			
			for(i=0; i<g_objUploadFileSta.m_iFileCount; i++)
			{
				memcpy(g_objUploadFileSta.m_szTempFileName[i],g_objUploadFileSta.m_szTempFileName[i+1], sizeof(g_objUploadFileSta.m_szTempFileName[i]) );
				memcpy(g_objUploadFileSta.m_szRealFileName[i],g_objUploadFileSta.m_szRealFileName[i+1], sizeof(g_objUploadFileSta.m_szRealFileName[i]) );
				g_objUploadFileSta.m_ulStartOffset[i] = g_objUploadFileSta.m_ulStartOffset[i+1];
				g_objUploadFileSta.m_ulEndOffset[i] = g_objUploadFileSta.m_ulEndOffset[i+1];
			}
		}
		
		// �򿪵�ǰҪ�ϴ�����ʽ�ļ�	
		fp = fopen(g_objUploadFileSta.m_szRealFileName[0],"rb");
		
		if(NULL != fp)
		{
			// �ж��ļ���С�Ƿ�Ϊ0
			fseek(fp, 0, SEEK_END);
			DWORD dwFileSize = ftell(fp);
			if( 0 == dwFileSize )
			{
				fclose(fp);
				fp = NULL;
				v_bDeleteFirstFile = TRUE;
				continue;
			}
			else
			{
				fseek(fp, 0, SEEK_SET);
			}

			// �ж��ļ�����
			if( NULL != strstr(g_objUploadFileSta.m_szTempFileName[0], ".264") )
				g_objUploadFileSta.m_uszFileType = 1;
			else if( NULL != strstr(g_objUploadFileSta.m_szTempFileName[0], ".726") )
				g_objUploadFileSta.m_uszFileType = 2;
			else if( NULL != strstr(g_objUploadFileSta.m_szTempFileName[0], ".ind") )
				g_objUploadFileSta.m_uszFileType = 3;
			else
			{
				fclose(fp);
				fp = NULL;
				v_bDeleteFirstFile = TRUE;
				continue;
			}

			// ���㵱ǰҪ�ϴ���ʱ�ļ����ܰ������ܴ�����
			ulFileSize = g_objUploadFileSta.m_ulEndOffset[0] - g_objUploadFileSta.m_ulStartOffset[0];

			g_objUploadFileSta.m_ulPackTotal = ulFileSize / UPLOAD_PACK_SIZE;
			if( 0 != ulFileSize%UPLOAD_PACK_SIZE ) 
				g_objUploadFileSta.m_ulPackTotal++;

			g_objUploadFileSta.m_ulWinTotal = g_objUploadFileSta.m_ulPackTotal/UPLOAD_WINSIZE;
			if( 0 != g_objUploadFileSta.m_ulPackTotal%UPLOAD_WINSIZE )
				g_objUploadFileSta.m_ulWinTotal++;

			PRTMSG(MSG_DBG, "WinTotal : %d,", g_objUploadFileSta.m_ulWinTotal);
			PRTMSG(MSG_DBG, "PackTotal: %d\n", g_objUploadFileSta.m_ulPackTotal);

			g_objUploadFileSta.m_ulPackNo = 0;
			g_objUploadFileSta.m_ulWinNo = MAX_UNSIGNED_64;

			g_objUploadFileSta.m_iBufCount = 0;

			fclose(fp);
			fp = NULL;
			return TRUE;
		}
		
		// ���ļ��޷��򿪣���Ӷ�����ɾ�����ļ�
		v_bDeleteFirstFile = TRUE;
		if(NULL != fp)
		{
			fclose(fp);
			fp = NULL;
		}
	}
}

void UploadOneWindow(BOOL v_bReUpload, BOOL v_bAllReUpload, uint v_ReloadSta[UPLOAD_WINCOUNT])
{
	int	i = 0, j = 0, n = 0;
	HI_U64	ulPackNo = 0;
	HI_U64	ulWinNo = 0;
	int		ireadlen = 0;
	uint	u32bitflag[UPLOAD_WINCOUNT];
	BYTE	u8PackToSend = 0;
	BYTE	u8tempbuf[2000] = {0};
	int		s32templen = 0;
	FILE	*fp = NULL;

	for(n = 0; n < UPLOAD_WINCOUNT; n++)
		u32bitflag[n] = 0x00000001;

	// ����ǰ���ڼ�أ��򷵻�
	if(	g_objWorkStart.VUpload[0] && g_objWorkStart.VUpload[1] && g_objWorkStart.VUpload[2] && g_objWorkStart.VUpload[3] )
		return;

	tag1PComuCfg objComuCfg;
	GetImpCfg( &objComuCfg, sizeof(objComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(objComuCfg) );

	PRTMSG(MSG_DBG, "�ϴ��ļ�:%s\n", g_objUploadFileSta.m_szTempFileName[0]);

	//�Ƿ��ش�
	if(v_bReUpload)
	{
		if((g_objUploadFileSta.m_ulWinNo+1) == g_objUploadFileSta.m_ulWinTotal)
		{
			for(n = 0; n < UPLOAD_WINCOUNT; n++)
			{
				for(i = 0; u32bitflag[n] != 0; u32bitflag[n] = u32bitflag[n] << 1,i++)
				{
					if( !(u32bitflag[n] & v_ReloadSta[n]) )
					{
						if(i+n*UPLOAD_WINUNIT+1 > g_objUploadFileSta.m_ulPackTotal- UPLOAD_WINSIZE*g_objUploadFileSta.m_ulWinNo)
						{
							v_ReloadSta[n] |= u32bitflag[n];
						}
					}
				}
				
				u32bitflag[n] = 0x00000001;
			}
		}

		PRTMSG(MSG_DBG, "ReUpload a Window: ");
		for(n = UPLOAD_WINCOUNT-1; n >= 0; n--)
			PRTMSG(MSG_DBG, "%08x", v_ReloadSta[n]);
		PRTMSG(MSG_DBG, "\n");

		// ��ʼ�����ش���
		for(n = 0; n < UPLOAD_WINCOUNT; n++)
		{
			for(i=0; u32bitflag[n] != 0; u32bitflag[n] = u32bitflag[n] << 1,i++)
			{
				// ��������Ҫ�ش�
				if( !(u32bitflag[n] & v_ReloadSta[n]) )
				{
					// ��ȡ��������ݰ�
					memcpy(u8tempbuf, g_objUploadFileSta.m_szPackBuf[i+n*UPLOAD_WINUNIT],g_objUploadFileSta.m_iBufLen[i+n*UPLOAD_WINUNIT]);
					s32templen = g_objUploadFileSta.m_iBufLen[i+n*UPLOAD_WINUNIT];
	
					memcpy((void*)&ulWinNo, u8tempbuf+50+7, 8);
					memcpy((void*)&ulPackNo, u8tempbuf+50+15, 8);
					PRTMSG(MSG_DBG, "ReUpload the WinNo %ld, ", ulWinNo);
					printf("PackNo: %ld\n", ulPackNo);
	
					//�Ƶ�SockServExe����
					DataPush(u8tempbuf, (DWORD)s32templen, DEV_DVR, DEV_SOCK, LV2);
				}
			}
		}
	}
	// �������ش�����˵��Ҫ�����µĴ�������
	else
	{
		PRTMSG(MSG_DBG, "Upload a new Window, WinNo = %d\n", g_objUploadFileSta.m_ulWinNo);

		// �򿪴����ļ�
		fp = fopen(g_objUploadFileSta.m_szRealFileName[0],"rb");
		if(NULL == fp)
			return;
	
		// �жϵ�ǰ��Ҫ���͵Ĵ�������Ƿ��Ǳ��ļ������һ������
		if((g_objUploadFileSta.m_ulWinNo+1) == g_objUploadFileSta.m_ulWinTotal)
		{
			u8PackToSend = g_objUploadFileSta.m_ulPackTotal- UPLOAD_WINSIZE*g_objUploadFileSta.m_ulWinNo;
			
			PRTMSG(MSG_DBG, "Now is the final window, WinTotal = %d",g_objUploadFileSta.m_ulWinTotal);
			PRTMSG(MSG_DBG, "WinNo = %d",g_objUploadFileSta.m_ulWinNo);
			PRTMSG(MSG_DBG, "u8PackToSend = %d\n", u8PackToSend);
		}
		else
			u8PackToSend = UPLOAD_WINSIZE;

		// ���ļ���λ����Ҫ�ϴ���λ��
		fseek(fp, g_objUploadFileSta.m_ulStartOffset[0]+g_objUploadFileSta.m_ulWinNo*UPLOAD_WINSIZE*UPLOAD_PACK_SIZE, SEEK_SET);

		// ��ȡ�ļ����ݡ����͡�����䵽����
		for(i = 0; i < u8PackToSend; i++)
		{
			// ������
			if( (ireadlen = fread(u8tempbuf+A2_HEAD_SIZE, 1, UPLOAD_PACK_SIZE, fp)) < 0 )
				return;

			s32templen = 0;

			u8tempbuf[s32templen++] = 0xA2;

			// �ֻ���
			memcpy(u8tempbuf+s32templen, objComuCfg.m_szTel, 15);
			s32templen += 15;

			// �ļ�����
			u8tempbuf[s32templen++] = g_objUploadFileSta.m_uszFileType;

			// �ļ���
			memcpy(u8tempbuf+s32templen, g_objUploadFileSta.m_szTempFileName[0], 40);
			s32templen += 40;

			// �������
			memcpy(u8tempbuf+s32templen, (void*)&g_objUploadFileSta.m_ulWinNo, 8);
			s32templen += 8;

			// �����
			PRTMSG(MSG_DBG, "WinNo: %ld, ", g_objUploadFileSta.m_ulWinNo);
			printf("PackNo: %ld\n", g_objUploadFileSta.m_ulPackNo);
			memcpy(u8tempbuf+s32templen, (void*)&g_objUploadFileSta.m_ulPackNo, 8);
			g_objUploadFileSta.m_ulPackNo++;
			s32templen += 8;
			
			// ����С
			u8tempbuf[s32templen++] = ireadlen%256;
			u8tempbuf[s32templen++] = ireadlen/256;

			// ���Ƶ������б��ݣ���Ԥ���ش�
			memcpy(g_objUploadFileSta.m_szPackBuf[i], u8tempbuf, ireadlen+A2_HEAD_SIZE);
			g_objUploadFileSta.m_iBufLen[i] = ireadlen+A2_HEAD_SIZE;
			
			//�Ƶ�SockServExe����
			DataPush(u8tempbuf, ireadlen+A2_HEAD_SIZE, DEV_DVR, DEV_SOCK, LV2);
		}

		if(NULL != fp)
		{
			fclose(fp);
			fp = NULL;
		}
	}
}

// �ļ����ͣ�1���� �ļ�����40�����汾�ţ�6���� Ӧ�����ͣ�1��
int Deal38B1(char *v_databuf, int v_datalen)
{
	char szNewFileName[40] = {0};
	memcpy(szNewFileName, v_databuf+1, 34);
	PRTMSG(MSG_DBG, "Recv B1 frame:%s \n", szNewFileName);
	//PrintString(v_databuf+41, v_datalen-41);
	
	int	i = 0;
	char	s8FileName[40] = {0};
	
	memcpy(s8FileName, v_databuf+1, 34);
	
	// ���ļ���һ�£���ر�A1�ش���ʱ��
	if( strcmp(s8FileName, g_objUploadFileSta.m_szTempFileName[0]) == 0 )
	{
		PRTMSG(MSG_NOR, "close A1_ReUpload Timer\n");

		_KillTimer(&g_objTimerMng, A1_REUPLOAD_TIMER);
	}

	return 1;
}

//�ļ����ͣ�1���� �ļ�����40���� ��ǰ������ţ�8���� ����״̬��4��
int Deal38B2(char *v_databuf, int v_datalen)
{
	static int iB2Cnt = 0;

	int		i = 0;
	BOOL	bNeedReUpload = FALSE;
	HI_U64  ulWinNo = 0;
	uint	uiWinSta[UPLOAD_WINCOUNT] = {0};
	char	szFileName[40] = {0};
	void	*thread_result;

	char szNewFileName[40] = {0};
	memcpy(szNewFileName, v_databuf+1, 34);
	PRTMSG(MSG_DBG, "Recv B2 frame:%s ", szNewFileName);
	PrintString(v_databuf+41, v_datalen-41);

	// �жϳ���
	if( v_datalen != 49+4*UPLOAD_WINCOUNT )
	{
		PRTMSG(MSG_ERR, "B2 frame uncorect data len\n");
		return 0;
	}

	// �ж��ļ����Ƿ��������ϴ����ļ���һ��
	memcpy(szFileName, v_databuf+1, 34);
	if( strcmp(szFileName, g_objUploadFileSta.m_szTempFileName[0]) != 0 )
	{
		PRTMSG(MSG_NOR, "Deal38B2 failed, uncorrect file name!\n");
		return 0;
	}

	// �������
	memcpy((void*)&ulWinNo, v_databuf+41, 8);

	// ����״̬
	for(int n = 0; n < UPLOAD_WINCOUNT; n++)
		memcpy((void*)&(uiWinSta[n]), v_databuf+49+4*n,4);

	// �ж��Ƿ���Ҫ�ش�
	if( ulWinNo != g_objUploadFileSta.m_ulWinNo )
		bNeedReUpload = FALSE;
	else
		bNeedReUpload = TRUE;
	
	// �������ش�������´���״̬
	if( FALSE == bNeedReUpload )
	{
		g_objUploadFileSta.m_iBufCount = 0;
		g_objUploadFileSta.m_ulWinNo++;
		
		// ��������Ĵ�����Ų����ڽ�Ҫ�ϴ��Ĵ������
		if( (ulWinNo != g_objUploadFileSta.m_ulWinNo) || ( (ulWinNo+1) > g_objUploadFileSta.m_ulWinTotal) )
		{
			PRTMSG(MSG_DBG, "uncorrect Window No, ");
			PRTMSG(MSG_DBG, "m_ulWinNo = %d," ,g_objUploadFileSta.m_ulWinNo);
			PRTMSG(MSG_DBG, "ulWinNo =%d\n", ulWinNo);

			return 0;
		}
		
		// ������һ������
		UploadOneWindow(FALSE, FALSE, uiWinSta);		
	}
	// �����ش�������������ݰ�
	else
	{
		UploadOneWindow(TRUE, FALSE, uiWinSta);
	}

	return 1;
}

// �ļ����ͣ�1���� �ļ�����40��
int Deal38B3(char *v_databuf, int v_datalen)
{
	char szNewFileName[40] = {0};
	memcpy(szNewFileName, v_databuf+1, 34);
	PRTMSG(MSG_DBG, "Recv B3 frame:%s\n", szNewFileName);
	
	int		i = 0;
	char	s8FileName[40] = {' '};
	BYTE	u8ResType = 0;
	BYTE	s8TempBuf[100] = {0};
	int		s32TempLen = 0;
	BYTE	s8SendBuf[100] = {0};
	int		s32SendLen = 0;

	memcpy(s8FileName, v_databuf+1, 34);

	if( strcmp(s8FileName, g_objUploadFileSta.m_szTempFileName[0]) == 0 )
	{
		u8ResType = 1;
	}
	else
	{
		PRTMSG(MSG_DBG, "Recv B3 frame, but file name is not correct!\n");
		PRTMSG(MSG_DBG, "Center file name: %s\n", s8FileName);
		PRTMSG(MSG_DBG, "Local  file name: %s\n", g_objUploadFileSta.m_szTempFileName[0]);
		return 0;
	}

	// ��֡Ӧ��A3��
	s8TempBuf[s32TempLen++] = 0xA3;

	// �ֻ���
	tag1PComuCfg objComuCfg;
	GetImpCfg( &objComuCfg, sizeof(objComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(objComuCfg) );					
	memcpy(s8TempBuf+s32TempLen, objComuCfg.m_szTel, 15);
	s32TempLen += 15;

	// �ļ�����
	s8TempBuf[s32TempLen++] = g_objUploadFileSta.m_uszFileType;

	// �ļ���
	memcpy(s8TempBuf+s32TempLen, g_objUploadFileSta.m_szTempFileName[0], 40);
	s32TempLen += 40;

	//Ӧ������
	s8TempBuf[s32TempLen++] = u8ResType;

	// ���������־
	s8TempBuf[s32TempLen++] = ( (UpdateUploadSta(TRUE) == TRUE) ? 0 : 1);

	//�Ƶ�SockServExe����
	DataPush(s8TempBuf, (DWORD)s32TempLen, DEV_DVR, DEV_SOCK, LV2);

	sleep(2);

	// Ӧ��֮������һ���ļ�
	BeginUploadFile();

	return 1;
}

void *G_DealSockData(void *arg)
{
	if(NETWORK_TYPE == NETWORK_EVDO || NETWORK_TYPE == NETWORK_WCDMA)
	{
		UPLOAD_PACK_SIZE = 1000;
	}
	else if(NETWORK_TYPE == NETWORK_TD)
	{
		UPLOAD_PACK_SIZE = 600;
	}
	
	int   iResult;
	char  szBuf[2048] = {0};
	BYTE  bytLvl;
	DWORD dwLen;
	DWORD dwPushTm = 0;
	
	while( !g_bProgExit )
	{
		iResult = g_objSockMng.PopData( bytLvl, sizeof(szBuf), dwLen, szBuf, dwPushTm);
		
		if( iResult || !dwLen || dwLen > sizeof(szBuf) )
		{
			msleep(100);
			continue;
		}

		g_objSock.AnalyseSockData(szBuf, dwLen);
	}
}

int StartBlack()
{
	if( !g_objWorkStart.Black )
	{
		PRTMSG(MSG_NOR, "Start Black\n");
		
// 		int iRet = pthread_create(&g_pthreadBlack, NULL, DealUdpSockData, NULL);
// 		if(iRet) 
// 		{
// 			PRTMSG(MSG_ERR, "start black failed: %08x\n", iRet);
// 			return -1;
// 		}
		
		sleep(1);//��ʱһ��ȷ���̳߳ɹ�����
		return 0;
	}

	return -1;
}

int StopBlack()
{
	if( g_objWorkStart.Black )
	{
		PRTMSG(MSG_NOR, "Stop Black\n");
		
		g_objWorkStop.Black = TRUE;
		/*pthread_join(g_pthreadBlack, NULL);*/
		
		PRTMSG(MSG_NOR, "stop black succ\n");
		return 0;
	}
	
	return -1;
}

#endif

