#ifndef _YX_DRVRECRPT_H_
#define _YX_DRVRECRPT_H_

#define DRVREC_SNDTYPE		2	// 1,����һ�����ڲŷ���; 2,ÿ֡����,��һ�����ڵȴ�Ӧ��

void *G_ThreadDrvRecSample(void *arg);
void  G_TmReRptDrvRecWin(void *arg, int len);
void  G_TmStopUploadDrvrec(void *arg, int len);

class CDrvRecRpt
{
public:
	CDrvRecRpt();
	~CDrvRecRpt();

	int Init();
	int Release();

	int Deal103e(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen );
	int Deal3705(char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen );

	void P_ThreadDrvRecSample();
	void P_TmReRptDrvRecWin();
	void P_TmStopUploadDrvrec();

	void DrvRecToSnd(bool v_bNeedSndWin);
	void RecStaSav();
	void DealAccChg( BOOL v_blAccOn );

private:
	void _BeginDrvRecSample( BOOL v_blAccOn );
	void _StopDrvRecSample();
	void _GetDrvRecSampleCfg( tagDrvRecSampleCfg &v_objDrvRecSampleCfg );
	void _SetDrvRecSampleCfg( const tagDrvRecSampleCfg &v_objDrvRecSampleCfg  );
	
#if DRVREC_SNDTYPE == 1
	bool _IsCurSndDrvRecWinEmpty();
	void _FillCurSndDrvRecWin();
#endif
	void _ClearCurSndDrvRecWin();
	int  _MakeOneDrvrecFrame( const tagDrvSampleDatas &v_objDrvSampleDatas, const WORD v_wSampleCycle,
		char *const v_szBuf, const size_t v_sizBuf, DWORD &v_dwLen );
	void _SndDrvrecWin(bool v_bReSnd);

private:
	// ��ʻ��¼�����߳����
	pthread_t m_pthreadDrvSample;
	bool m_bEndDrvSample;
	bool m_bStopDrvSample;
	bool m_bForceRptDrv;
	pthread_mutex_t m_MutexDrvRec;
	
	// ��ʻ��¼��������
	tagDrvRecSampleCfg m_objDrvRecSampleCfg;	// ��Ҫ�������
	CInnerDataMng m_objWaitSndDrvRecMng;		// ��Ҫ�������,���������ṩ�˻�����ʵķ���
	DWORD m_aryCurSndDrvRecFrameInfo[32][2];	// ��ǰ���ʹ��ڵĸ�֡��Ϣ����: ��1��Ԫ����֡����; ��2��Ԫ����֡������ָ��
	DWORD m_dwDrvCurWinFstSndTime;				// ��ǰ�����״η���ʱ��ʱ��
	bool m_bDrvCurWinChkGprs;					// ��ǰ�����ش���ʱ������ײ���GPRS
	BYTE m_bytCurSndDrvRecFrameNum;				// ��ǰ���ڵ�֡��
	bool m_bCurSndNeedAns;						// ��ǰ���ʹ����Ƿ�Ҫ�ɿ�����
};


#endif // #ifndef _YX_DRVRECRPT_H_

