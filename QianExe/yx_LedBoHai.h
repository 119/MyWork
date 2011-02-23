#ifndef _YX_LED_H_
#define _YX_LED_H_

void *G_ThreadLedRead(void* arg);
void *G_ThreadLedwork(void* arg);

class CLedBoHai
{
public:
	CLedBoHai();
	~CLedBoHai();

	int  Init();
	int  Release();
	bool ComOpen();
	bool ComClose();

	void P_ThreadLedRead();
	void P_ThreadLedWork();	
	int  Deal0x39(BYTE v_bytDataType,char *v_szData, DWORD v_dwDataLen);
	bool DealCenterMsg();
	bool SendCenterMsg(BYTE cmd, char* param, int len);

	void LedTurnToWork();
	void LedTurnToSleep();

	void DealComData_V1_2(char *v_szRecv, int v_iRecvLen );
	void DealComData_V2_1(char *v_szRecv, int v_iRecvLen );
	void DealComData_V2_2(char *v_szRecv, int v_iRecvLen );
	int  SendComData_V1_2(char* cmd, char* param, int len);
	int  SendComData_V2_1(char* cmd, char* param, int len);
	int  SendComData_V2_2(char* cmd, char* param, int len);
	bool SndCmdWaitRes( char* m_szCmd, char* v_szCmdCont, int v_iCmdContLen,  BYTE v_bytSndTimes, DWORD v_dwWaitPridPerTime, char* v_szRes = NULL, int v_iResSize = 0, int* v_p_iResLen = NULL );

	int  GetCurTime(tagCurTime& v_time);
	void AdjustTime();
	bool SaveLedConfig();	
	void SendMailID();
	void EnableLedAlarm();
	void CancelLedAlarm();
	
	bool insert(char* param, int len);
	bool delete_all();
	bool delete_msg(char* param, int len);
	bool display(char* param, int len);
	bool exit_disp();
	int  list_all(char* outbuf);
	int  query_id(ushort id, char* outbuf);
	bool show(char* param, int len);
	bool exit_show();
	int  query_time(char* outbuf);
	bool set_time(char* param, int len);
	bool set_wait(BYTE sec);
	bool alarm(char* param, int len);
	bool exit_alarm();
	int  query_ver(char* outbuf);
	int  query_state(char * outbuf);
	bool bright_ctrl(char* param, int len);
	bool black();
	bool light();
	bool reset();
	bool set_led(char* param, int len);
	byte updd(char* param, int len);
	bool pwon();
	bool optm(char* param, int len);	
	
	int LedTest( char* v_szData, DWORD v_dwDataLen );
	void PrintData( int v_iMsgType, char* v_pData, int v_iDataLen );

	void _DeTranData(char *v_szBuf, int &v_iBufLen);

#if SAVE_LEDDATA == 1
	void save_log(char *v_szData, int len);
#endif
	
	void build_table16( unsigned short aPoly );
	unsigned short CRC_16( unsigned char * aData, unsigned long aSize );
	
public:
	Queue		m_objCenterCmdQue;		// ���ķ��͵�����
	Queue		m_objLocalCmdQue;		// ��̨���͵�����
//	Queue		m_objReadComQue;		// ���ڷ��ص�����
	CInnerDataMng	m_objReadComMng;

	BYTE		m_bytVeSta;			// ����״̬��0��ʼ����1��ֹ֪ͨ��2�ƶ�֪ͨ
	DWORD		m_dwVeStaTick;		// ����״̬ʱ��
	BYTE		m_bytLedCfg;		// LED����

private:
	HANDLE		m_hComPort;			// ���ھ��
	pthread_t	m_pthreadRead;		//LED��ȡ�߳�
	pthread_t	m_pthreadWork;		//LED�����߳�

	bool		m_bEvtExit;
	bool		m_bEvtWork;

	char		m_szRecvFrameBuf[256];
	int			m_iRecvFrameLen;

	BYTE		m_bytCurSta;		// ��ǰ����״̬

	tagLedMenu	m_objLedMenu;		//led��Ŀ��
	tagQLEDStat m_objLedStat;		//led����״̬
	tagCurTime	m_objCurtime;		//GPSʱ��
	tagMail		m_objMail;			//LED����������	
	tagLedUpdtStat	m_objUpSta;		//����״̬	
	
	bool		m_bAccValid;
	bool		m_bAccTurnOn;		// ��ʾACC���Ķ����� ����ʾACC��״̬
	int			m_iAccSta;			// ACC��״̬
	DWORD		m_dwAccToOnTm;

	char		m_szUpgrdReqBuf[100];
	int			m_iUpgrdReqBufLen;
	DWORD		m_dwReNewTick;			//���½�Ŀ��ʱ��
	DWORD		m_dwUpgrdSndTm;			// ���һ��������������������ʱ��
	WORD		m_wUpgrdSndTimes;		// �������������Ĵ���
	BYTE		m_bytPowerOnWaitTm;		// LED�ϵ�ȴ�ʱ�� 
	ushort		m_usBlackWaitTm;		// �����ȴ�ʱ��

	ushort		m_usToSavLedSetID;
	ushort		m_usTURNID;				// ת����ϢID
	ushort		m_usNTICID;				// ֪ͨ��ϢID

	bool		m_md5ok;				// У����
	char		m_szCarKey[16];			// ��̨��Կ
	uchar		m_md5[32];				// ���ܹ���md5ֵ
	CMd5		cmd5;					// ��ȡ�����ļ�ʵ�ʵ�md5ֵ	

	ushort		m_cnCRC_CCITT;
	ulong		table_CRC[256];

#if SAVE_LEDDATA == 1
	int			m_logtype;		//��־����,1:��̨����LED;2:LED������̨;3:��̨����ǰ�û�;4:ǰ�û�������̨
#endif
};


#endif	// #ifndef _YX_LED_H_



