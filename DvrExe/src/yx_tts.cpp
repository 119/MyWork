#include "yx_common.h"

pthread_t g_pthreadTTS;

int g_Handle = -1;
char *g_MappedMem = (char *)MAP_FAILED;

/* read resource callback */
ivBool ivCall ReadResCB(ivPointer pParameter, ivPointer pBuffer, ivResAddress iPos, ivResSize nSize )
{
	memcpy(pBuffer, g_MappedMem+iPos, nSize);
	return ivTrue;
}

ivTTSErrID ivCall OutputCB(ivPointer pParameter, ivUInt16 nCode, ivCPointer pcData, ivSize nSize )
{
	/* ������������ȥ���� */
	WriteBuf(0, (BYTE *)pcData, nSize, 'T');
	return ivTTS_ERR_OK;
}

ivTTSErrID ivCall ParamChangeCB(ivPointer pParameter, ivUInt32 nParamID, ivUInt32 nParamValue )
{
	return ivTTS_ERR_OK;
}

ivTTSErrID ivCall ProgressCB(ivPointer pParameter, ivUInt32 iProcPos, ivUInt32 nProcLen )
{
	return ivTTS_ERR_OK;
}

int InitTTS()
{
	g_Handle = open("/lib/Resource.irf", O_RDONLY);
	if( g_Handle==-1 ) goto ERROR;

	g_MappedMem = (char *)mmap(NULL, RESOUSE_SIZE, PROT_READ, MAP_SHARED, g_Handle, 0);
  if( g_MappedMem==MAP_FAILED ) goto ERROR;

  return 0;

 ERROR:
 	PRTMSG(MSG_ERR, "init tts error\n");
 	close(g_Handle);
 	return -1;
}

int ReleaseTTS()
{
	munmap(g_MappedMem, RESOUSE_SIZE);
	close(g_Handle);
	
	g_Handle = -1;
	g_MappedMem = (char *)MAP_FAILED;
}

void MakeTTS(char* v_pText)
{
	ivHTTS	hTTS;
	ivPByte	pHeap;
	ivTTSErrID	ivReturn;
	
	/* ����� */
	pHeap = (ivPByte)malloc(ivTTS_HEAP_SIZE);
	memset(pHeap, 0, ivTTS_HEAP_SIZE);

	/* ��ʼ����Դ�������ж����Դ�������Էְ�*/
	ivTResPackDescExt tResPackDesc;
	tResPackDesc.pCBParam = NULL;
	tResPackDesc.pfnRead = ReadResCB;
	tResPackDesc.pfnMap = NULL;
	tResPackDesc.nSize = 0;

	tResPackDesc.pCacheBlockIndex = NULL;
	tResPackDesc.pCacheBuffer = NULL;
	tResPackDesc.nCacheBlockSize = 0;
	tResPackDesc.nCacheBlockCount = 0;
	tResPackDesc.nCacheBlockExt = 0;
	
	/* ���� TTS ʵ�� */
	ivReturn = ivTTS_Create(&hTTS, (ivPointer)pHeap, ivTTS_HEAP_SIZE, ivNull, (ivPResPackDescExt)&tResPackDesc, (ivSize)1, NULL);
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_OUTPUT_CALLBACK, (ivUInt32)OutputCB);/* ������Ƶ����ص� */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_PARAMCH_CALLBACK, (ivUInt32)ParamChangeCB);/* ���ò����ı�ص� */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_PROGRESS_CALLBACK, (ivUInt32)ProgressCB);/* ���ô�����Ȼص� */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_INPUT_CODEPAGE, ivTTS_CODEPAGE_GBK);/* ���������ı�����ҳ */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_LANGUAGE, ivTTS_LANGUAGE_CHINESE);	/* �������� */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_VOLUME, ivTTS_VOLUME_NORMAL);/* �������� */
	ivReturn = ivTTS_SetParam(hTTS, ivTTS_PARAM_ROLE, ivTTS_ROLE_XIAOFENG);/* ���÷�����Ϊ XIAOFENG */
	ivReturn = ivTTS_SynthText(hTTS, ivText(v_pText), MAX_UNSIGNED_32);/* ���������ı�*/
	
	/* ���ʼ�� */
	ivReturn = ivTTS_Destroy(hTTS);
	
	if( tResPackDesc.pCacheBlockIndex )
		free(tResPackDesc.pCacheBlockIndex);
	
	if( tResPackDesc.pCacheBuffer )
		free(tResPackDesc.pCacheBuffer);
	
	if( pHeap )
		free(pHeap);
}

void *PlayTTS(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	int	iRet;
	
	IOSET( IOS_TRUMPPOW, IO_TRUMPPOW_ON );//�������ŵ�Դ
	IOSET( IOS_EARPHONESEL, IO_EARPHONESEL_OFF );//ѡ������ͨ��
	IOSET( IOS_AUDIOSEL, IO_AUDIOSEL_PC );//ѡ��PC��Ƶ
	
	msleep(200);
	
#if VEHICLE_TYPE == VEHICLE_M
	BYTE TBuf[960] = {0};
	ADEC_CHN iAdecChn = -1;
	AUDIO_RAWDATA_S objAudioRawData = {AUDIO_BIT_WIDTH_16, AUDIO_SAMPLE_RATE_8, 0, 0, (short *)TBuf, (uint)sizeof(TBuf)};
	
	if( !g_objCtrlStart.APlayback[0] )
	{
		iRet = HI_ADEC_CreateCH(0, AUDIO_CODEC_FORMAT_ADPCM , HI_TRUE , &iAdecChn); 
		if(iRet)
		{
			PRTMSG(MSG_ERR, "create audio playback chn1 failed: %08x\n", iRet);
			goto TTS_PLAY_STOP;
		}
		
		iRet = HI_ADEC_StartCH(iAdecChn, 0); 
		if(iRet) 
		{
			PRTMSG(MSG_ERR, "start audio playback chn1 failed: %08x\n", iRet);
			goto TTS_PLAY_STOP;
		}
	}
	else
	{
		g_objPlaybackCtrl.APause = TRUE;// ��ͣ��Ƶ�ط�
	}
#endif
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	BYTE TBuf[960] = {0};
	AUDIO_FRAME_S objAudioRawData;
	objAudioRawData.enBitwidth = AUDIO_BIT_WIDTH_16;
	objAudioRawData.enSoundmode = AUDIO_SOUND_MODE_MOMO;
	objAudioRawData.u64TimeStamp = 0;
	objAudioRawData.u32Seq = 0;
	objAudioRawData.u32Len = sizeof(TBuf);
	
	if( !g_objCtrlStart.APlayback[0] )
	{
		SwitchAo(AUDIO_SAMPLE_RATE_8000);
		
		ADEC_ATTR_ADPCM_S objAdpcmAttr;
		ADEC_CHN_ATTR_S objChnAttr;
		
		objAdpcmAttr.enADPCMType = ADPCM_TYPE_IMA;
		objChnAttr.enType = PT_ADPCMA;
		objChnAttr.u32BufSize = 8;
		objChnAttr.enMode = ADEC_MODE_STREAM;
		objChnAttr.pValue = &objAdpcmAttr;
		
		iRet = HI_MPI_ADEC_CreateChn(0, &objChnAttr);
		if(iRet)
		{
			PRTMSG(MSG_ERR, "create audio playback chn1 failed: %08x\n", iRet);
			goto TTS_PLAY_STOP;
		}
	}
	else
	{
		g_objPlaybackCtrl.APause = TRUE;// ��ͣ��Ƶ�ط�
		sleep(1);
		SwitchAo(AUDIO_SAMPLE_RATE_8000);
	}
#endif
	
	g_objWorkStart.TTS = TRUE;
	
	while( !g_objWorkStop.TTS && ReadBuf(0, TBuf, sizeof(TBuf), 'T') )
	{
#if VEHICLE_TYPE == VEHICLE_M
		HI_ADEC_SendRawData(0, &objAudioRawData);//����TTS����
#endif
		
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		memcpy(objAudioRawData.aData, TBuf, sizeof(TBuf));
		HI_MPI_AO_SendFrame(0, 0, &objAudioRawData, HI_IO_BLOCK);
#endif

		memset(TBuf, 0, sizeof(TBuf));
	}

#if VEHICLE_TYPE == VEHICLE_M
	HI_ADEC_SendRawData(0, &objAudioRawData);//����TTS����
#endif
		
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	memcpy(objAudioRawData.aData, TBuf, sizeof(TBuf));
	HI_MPI_AO_SendFrame(0, 0, &objAudioRawData, HI_IO_BLOCK);
#endif
	
TTS_PLAY_STOP:
	
	g_objWorkStop.TTS = FALSE;
	WriteBuf(0, NULL, 0, 'T');//���TTS����
	
	sleep(3);//�ȴ����һ���ֲ�����Źرչ��ŵ�Դ
	
	IOSET( IOS_TRUMPPOW, IO_TRUMPPOW_OFF );//�رչ��ŵ�Դ
	IOSET( IOS_EARPHONESEL, IO_EARPHONESEL_ON );//ѡ�����ͨ��
	IOSET( IOS_AUDIOSEL, IO_AUDIOSEL_PHONE );//ѡ���ֻ���Ƶ
	
#if VEHICLE_TYPE == VEHICLE_M
	if( !g_objCtrlStart.APlayback[0] )
	{
		//�ر���Ƶ����ͨ��
		HI_ADEC_StopCH(iAdecChn); 
		HI_ADEC_DestroyCH(iAdecChn, HI_TRUE); 
	}
	else
	{
		g_objPlaybackCtrl.APause = FALSE;
	}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	if( !g_objCtrlStart.APlayback[0] )
	{
		//�ر���Ƶ����ͨ��
		HI_MPI_ADEC_DestroyChn(0);
		SwitchAo(AUDIO_SAMPLE_RATE_32000);
	}
	else
	{
		SwitchAo(AUDIO_SAMPLE_RATE_32000);
		sleep(1);
		g_objPlaybackCtrl.APause = FALSE;
	}
#endif
	
	g_objWorkStart.TTS = FALSE;
	
	pthread_exit(0);
}

int StartTTS()
{
	if( !g_objWorkStart.TTS )
	{
		PRTMSG(MSG_NOR, "Start TTS\n");
		
		int iRet = pthread_create(&g_pthreadTTS, NULL, PlayTTS, NULL);
		if(iRet) 
		{
			PRTMSG(MSG_ERR, "start tts failed: %08x\n", iRet);
			return -1;
		}
		
		sleep(1);//��ʱһ��ȷ���̳߳ɹ�����
		return 0;
	}
	
	return -1;
}

int StopTTS()
{
	if( g_objWorkStart.TTS )
	{
		PRTMSG(MSG_NOR, "Stop TTS\n");
		
		g_objWorkStop.TTS = TRUE;
		pthread_join(g_pthreadTTS, NULL);
		
		PRTMSG(MSG_NOR, "stop tts succ\n");
		return 0;
	}
	
	return -1;
}


int TTSTest(char* v_pText)
{
	int	iRet;
	
	IOSET( IOS_TRUMPPOW, IO_TRUMPPOW_ON );//�������ŵ�Դ
	IOSET( IOS_EARPHONESEL, IO_EARPHONESEL_OFF );//ѡ������ͨ��
	IOSET( IOS_AUDIOSEL, IO_AUDIOSEL_PC );//ѡ��PC��Ƶ
	
	msleep(100);
	
	MakeTTS(v_pText);
	
#if VEHICLE_TYPE == VEHICLE_M
	BYTE TBuf[960] = {0};
	ADEC_CHN iAdecChn = -1;
	AUDIO_RAWDATA_S objAudioRawData = {AUDIO_BIT_WIDTH_16, AUDIO_SAMPLE_RATE_8, 0, 0, (short *)TBuf, (uint)sizeof(TBuf)};
	
	iRet = HI_ADEC_CreateCH(0, AUDIO_CODEC_FORMAT_ADPCM , HI_TRUE , &iAdecChn); 
	if(iRet)
	{
		PRTMSG(MSG_ERR, "create audio playback chn1 failed: %08x\n", iRet);
		goto TTS_PLAY_STOP;
	}
		
	iRet = HI_ADEC_StartCH(iAdecChn, 0); 
	if(iRet) 
	{
		PRTMSG(MSG_ERR, "start audio playback chn1 failed: %08x\n", iRet);
		goto TTS_PLAY_STOP;
	}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	BYTE TBuf[960] = {0};
	AUDIO_FRAME_S objAudioRawData;
	objAudioRawData.enBitwidth = AUDIO_BIT_WIDTH_16;
	objAudioRawData.enSoundmode = AUDIO_SOUND_MODE_MOMO;
	objAudioRawData.u64TimeStamp = 0;
	objAudioRawData.u32Seq = 0;
	objAudioRawData.u32Len = sizeof(TBuf);
	
	ADEC_ATTR_ADPCM_S objAdpcmAttr;
	ADEC_CHN_ATTR_S objChnAttr;
	objAdpcmAttr.enADPCMType = ADPCM_TYPE_IMA;
	objChnAttr.enType = PT_ADPCMA;
	objChnAttr.u32BufSize = 8;
	objChnAttr.enMode = ADEC_MODE_STREAM;
	objChnAttr.pValue = &objAdpcmAttr;
	
	SwitchAo(AUDIO_SAMPLE_RATE_8000);
	
	iRet = HI_MPI_ADEC_CreateChn(0, &objChnAttr);
	if(iRet)
	{
		PRTMSG(MSG_ERR, "create audio playback chn1 failed: %08x\n", iRet);
		goto TTS_PLAY_STOP;
	}
#endif
	
	while( ReadBuf(0, TBuf, sizeof(TBuf), 'T') )
	{
#if VEHICLE_TYPE == VEHICLE_M
		HI_ADEC_SendRawData(0, &objAudioRawData);//����TTS����
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		memcpy(objAudioRawData.aData, TBuf, sizeof(TBuf));
		HI_MPI_AO_SendFrame(0, 0, &objAudioRawData, HI_IO_BLOCK);
#endif
		memset(TBuf, 0, sizeof(TBuf));
	}
	
#if VEHICLE_TYPE == VEHICLE_M
	HI_ADEC_SendRawData(0, &objAudioRawData);//����TTS����
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	memcpy(objAudioRawData.aData, TBuf, sizeof(TBuf));
	HI_MPI_AO_SendFrame(0, 0, &objAudioRawData, HI_IO_BLOCK);
#endif
	
TTS_PLAY_STOP:
	WriteBuf(0, NULL, 0, 'T');//���TTS����
	
	sleep(3);//�ȴ����һ���ֲ�����Źرչ��ŵ�Դ
	
	IOSET( IOS_TRUMPPOW, IO_TRUMPPOW_OFF );//�رչ��ŵ�Դ
	IOSET( IOS_EARPHONESEL, IO_EARPHONESEL_ON );//ѡ�����ͨ��
	IOSET( IOS_AUDIOSEL, IO_AUDIOSEL_PHONE );//ѡ���ֻ���Ƶ
	
#if VEHICLE_TYPE == VEHICLE_M
		
	HI_ADEC_StopCH(iAdecChn); //�ر���Ƶ����ͨ��
	HI_ADEC_DestroyCH(iAdecChn, HI_TRUE); 
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	HI_MPI_ADEC_DestroyChn(0);//�ر���Ƶ����ͨ��
	SwitchAo(AUDIO_SAMPLE_RATE_32000);
#endif
}
