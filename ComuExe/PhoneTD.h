 // PhoneTD.h: interface for the CPhoneTD class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PHONETD_H_
#define _PHONETD_H_

enum TD_ATANS
{
	TD_ATANS_OK = 1,
	TD_ATANS_ERROR,
	TD_ATANS_CMEERR,
	TD_ATANS_NOANS,
	TD_ATANS_RCVNOTALL,
	// { �����ں������
	TD_ATANS_NOANSWER,
	TD_ATANS_NOCARRIER,
	TD_ATANS_NODIALTONE,
	TD_ATANS_BUSY,
	TD_ATANS_NOCALL, // ����ͨ����
	// } �����ں������
	TD_ATANS_OTHER
};

enum TD_PHONESTA
{
	TD_PST_INIT = 1,
	TD_PST_IDLE,
	TD_PST_DIALNET,
	TD_PST_WAITDIALNET,
	TD_PST_CUTNET,
	TD_PST_RESET,
	TD_PST_DIALLSN,
	TD_PST_WAITLSNPICK,
	TD_PST_LSNING,
	TD_PST_DIAL,
	TD_PST_WAITPICK,
	TD_PST_HOLD,
	TD_PST_HANGUP,
	TD_PST_RCVRING
};

// enum TD_PINGSTA
// {
// 	TD_PINGST_IDLE = 666,
// 	TD_PINGST_DOING,
// 	TD_PINGST_SUCC,
// 	TD_PINGST_FAIL,
// 	TD_PINGST_CANCEL
// };

enum TD_AUDIOMODE
{
	TD_AUDIOMODE_HANDFREE = 1, // ��Ҫ̫��,�еĽṹ����ֻ��1���ֽڱ���
	TD_AUDIOMODE_EARPHONE,
	TD_AUDIOMODE_LSN,
	TD_AUDIOMODE_DIAL,
	TD_AUDIOMODE_CLOSE
};

// ����ָ��壨�ɻ�������
const DWORD TD_PHTASK_DIALSN		= 0X00000001;
const DWORD TD_PHTASK_DIAL			= 0X00000002;
const DWORD TD_PHTASK_RING			= 0X00000004;
const DWORD TD_PHTASK_SETSMSCENT	= 0X00000008;
const DWORD TD_PHTASK_SNDDTMF		= 0X00000010;
const DWORD TD_PHTASK_PICKUP		= 0X00000020;
const DWORD TD_PHTASK_HANGUP		= 0X00000040;
const DWORD TD_PHTASK_CHGAUDIO		= 0X00000080;
const DWORD TD_PHTASK_CHGVOL		= 0X00000100;
const DWORD TD_PHTASK_PING			= 0x00000200;
const DWORD TD_PHTASK_PINGRPT		= 0x00000400;
const DWORD TD_PHTASK_CUTNET		= 0x00000800;
const DWORD TD_PHTASK_ALERM		= 0x00001000;
const DWORD TD_PHTASK_RESET		= 0x00002000;

// �������ݶ��� (���̷߳���,��Ҫ����)
struct tagTdPhoneTask
{
	DWORD	m_dwTaskSymb; // ȡֵ������Ķ���

//	bool	m_bDialLsn;
	char	m_szDialLsnTel[20];

//	bool	m_bDial;
	char	m_szDialTel[20];
	DWORD	m_dwHoldLmtPrid;
	
//	bool	m_bRing;
//	char	m_szRingTel[20];

//	bool	m_bSetSmsCent;
//	char	m_szSmsCent[19];

//	bool	m_bSndDTMF;
	char	m_szDTMF[60];

//	bool	m_bSndSms;
//	bool	m_bHangUp; // �û����������һ�
	BYTE	m_bytChgAudio; // TD_AUDIOMODE_HANDFREE,�л�������; TD_AUDIOMODE_EARPHONE,�л�������
	int		m_iChgVol; // 1,����; -1,��С

//	bool	m_bPing;
	BYTE	m_bytPingSrc; // 1,QianExe; 2,StreamMedia (��Ĺ�ϵ )

	BYTE	m_bytPingRptSrc;
	BYTE	m_bytPingResult; // 0,�ɹ�; 1,ʧ��; 2,�쳣; 3,��ȡ��

//	bool	m_bCutNet;

	DWORD	m_dwAlertPrid;

	tagTdPhoneTask() { memset(this, 0, sizeof(*this)); }
};


/// { ״̬����ÿһ��״̬��Ӧ��״ֵ̬ (����״̬���߳���ʹ��,���軥�����)
struct tagTdPhoneStaDial
{
	DWORD	m_dwDialBeginTm;
	DWORD	m_dwHoldLmtPrid;
	char	m_szDialTel[20];

	void Init() { memset(this, 0, sizeof(*this)); }
	tagTdPhoneStaDial() { Init(); }
};

struct tagTdPhoneStaHold
{
	DWORD	m_dwHoldBeginTm;
	DWORD	m_dwHoldLmtPrid;
	DWORD	m_dwCallChkTm;

	void Init() { memset(this, 0, sizeof(*this)); }
	tagTdPhoneStaHold() { Init(); }
};

struct tagTdPhoneStaDialLsn
{
	DWORD	m_dwDialBeginTm;
	char	m_szDialLsnTel[20];

	void Init() { memset(this, 0, sizeof(*this)); }
	tagTdPhoneStaDialLsn() { Init(); }
};

struct tagTdPhoneStaLsning
{
	DWORD	m_dwCallChkTm;

	void Init() { memset(this, 0, sizeof(*this)); }
	tagTdPhoneStaLsning() { Init(); }
};

struct tagTdPhoneStaRing
{
	DWORD	m_dwHoldLmtPrid;
	BYTE	m_bytCallInType; // �������ͣ�0,������� 1,���������������� 2,��ֹ������� 3,���ķ���绰���� 4,UDP�������
	char	m_szRingTel[20];
	DWORD	m_dwLstChkRingTm;

	void Init()
	{
		memset(this, 0, sizeof(*this));
		m_dwLstChkRingTm = GetTickCount();
	}
	tagTdPhoneStaRing() { Init(); }
};
/// } ״̬����ÿһ��״̬��Ӧ��״ֵ̬


// ͨ��״̬������ (��λ����)
const DWORD TD_PHSTVAR_IMEI		= 0X00000001;
const DWORD TD_PHSTVAR_SING		= 0X00000002;
const DWORD TD_PHSTVAR_ALERT		= 0X00000004;
const DWORD TD_PHSTVAR_NET			= 0X00000008;
const DWORD TD_PHSTVAR_PING		= 0X00000010;
const DWORD TD_PHSTVAR_AUDIOMODE	= 0X00000020;
const DWORD TD_PHSTVAR_AUDIOOUTLVL = 0x00000040;
const DWORD	TD_PHSTVAR_NETTECH		= 0x00000080;

/// { ״̬���ж��״̬����Ҫʹ�õ�״ֵ̬ (���ܶ��̷߳���,��Ҫ����)
struct tagTdPhoneStaCommon
{
	char	m_szGsmIMEI[256];
	int		m_iSignal;
	int		m_iNetTech; // -1,δ֪; 0,GSM; 1,GSM compact; 2,TDD

	DWORD	m_dwAlertBegin;
	DWORD	m_dwAlertPrid;

	int		m_iAudioOutVol; // ͨ��ʱ�����������: 0~5
	BYTE	m_bytCurAudioMode;

	bool	m_bNetConnected;

	BYTE	m_bytPingSrc; // 1,QianExe; 2,StreamMedia (��Ĺ�ϵ )

	void Init()
	{
		memset(this, 0, sizeof(*this));
		m_iAudioOutVol = 2;
		m_iNetTech = -1;
	}

	tagTdPhoneStaCommon()
	{
		Init();
	}
};
/// } ״̬���ж��״̬����Ҫʹ�õ�״ֵ̬

typedef int (*PCHKATRES) (char* v_szAtRes, void* v_pResult, size_t v_sizResult );

class CPhoneTD  
{
public:
	CPhoneTD();
	virtual ~CPhoneTD();
	int InitPhone();
	void ReleasePhone();
	void ResetPhone();
	void ReDialNet();
	BYTE GetPhoneState();
	int GetPhoneHdlSta();
	int GetPhoneSignal();
	void GetPhoneIMEI( char* v_szImei, size_t v_sizImei );
	int GetPhoneVolume();
	int _PppSigHandler();

	bool IsPhoneValid();

protected:
	unsigned long P_PhoneHdl();
	friend unsigned long G_TD_PhoneHdl(void* v_pPar);

	unsigned long P_ComRcv();
	friend unsigned long G_TD_ComRcv(void* v_pPar);

	unsigned long P_OtherHdl();
	friend unsigned long G_TD_OtherHdl(void* v_pPar);

//	unsigned long P_Ping();
	friend unsigned long G_TD_Ping(void* v_pPar);

private:

	int _InitCom();
	int _CloseCom();
	int _OtherHdl();

	void _GsmHardReset();
	void _GsmPowOn();
	void _GsmPowOff();

	// { ����״̬���̵߳���
	int _ComSnd(void* v_pSnd, DWORD v_dwSndLen );
	int _SetAT( DWORD v_dwWaitPerTime, BYTE v_bytTryTimes, DWORD v_dwSuccPausePrid, DWORD v_dwFailPausePrid, void* v_pResult, size_t v_sizResult,
		PCHKATRES v_pChkRes, DWORD v_dwSndLen, char* v_szATCmd, ... );
	int _ReadAT( DWORD v_dwWait, void* v_pResult, size_t v_sizResult, PCHKATRES v_pChkRes );
	int _PhoneDataDeal(char* v_szAtData);
	
	int _PhoneHdl();
	int _PhoneStaHdl_Init();
	int _PhoneStaHdl_Idle( int& v_iNextSta );
	int _PhoneStaHdl_DialLsn( int& v_iNextSta );
	int _PhoneStaHdl_WaitLsnPick( int &v_iNextSta );
	int _PhoneStaHdl_Lsning(int &v_iNextSta );
	int _PhoneStaHdl_Dial( int &v_iNextSta );
	int _PhoneStaHdl_WaitPick( int &v_iNextSta );
	int _PhoneStaHdl_Hold( int &v_iNextSta );
	int _PhoneStaHdl_RcvRing( int& v_iNextSta );
	int _PhoneStaHdl_Hangup( int &v_iNextSta );
	int _PhoneStaHdl_Reset();
	int _PhoneStaHdl_DialNet( int &v_iNextSta );
	int _PhoneStaHdl_WaitDialNet( int &v_iNextSta );
	void _PhoneStaHdl_CutNet( int &v_iNextSta );

	void _PhoneJugSmsSnd();
	void _PhoneJugSmsCentSet();
	void _PhoneJugSmsPingRpt();
	void _PhoneJugAlerm();
	BOOL _PhoneJugReset();
	BOOL _PhoneJugCutNet();

	int _PhoneSingleGet( int *v_p_iSignal );

	int _PhoneAudioModeChg(int v_iChgToMode);
	int _PhoneAudioOutVolChg(int v_iChgDegree);

	int _PhoneSmsSnd( SM_PARAM& v_objSmPar );
	int _PushSndSms(char *to, byte tolen, char *msg, byte msglen, BYTE v_bytSrcType);
	bool sms_deal(SM_PARAM *psm);

	int _PhoneDialNet();
	void _PhoneCutNet();
	int _ForkPppd();
	
	int  convert6to7(char *sptr, char *dptr, int len);
	int  convert6to8(const char* strOldBuf, char* strNewBuf, const int iOldLen);
	bool check_crc1(const byte *buf, const int len);	// ���
	byte get_crc1(const byte *buf, const int len);		// ���
	bool check_crc2(const byte *buf, const int len);	// ֱ��ȡ�ۼӺ�
	byte get_crc2(const byte *buf, const int len);		// ֱ��ȡ�ۼӺ�
	// } ����״̬���̵߳���

	tagTdPhoneTask _PhoneTaskGet( DWORD v_dwToGetTaskSymb );
	void _PhoneTaskSet( const tagTdPhoneTask& v_objPhoneTask, DWORD v_dwToSetTaskSymb );
	void _PhoneTaskClr(DWORD v_dwToGetTaskSymb);
	void _ClearPhoneTask( DWORD v_dwToGetTaskSymb, tagTdPhoneTask& v_objPhoneTask );

	tagTdPhoneStaCommon _PhoneStaCommonGet( DWORD v_dwToClrStaSymb );
	void _PhoneStaCommonSet( const tagTdPhoneStaCommon& v_objPhoneStaCommon, DWORD v_dwToSetStaSymb );
	void _PhoneStaCommonClr(DWORD v_dwToClrStaSymb);
	void _ClearPhoneStaCommon(tagTdPhoneStaCommon& v_objPhoneStaCommon, DWORD v_dwToClrStaSymb);

	byte tel_type(bool callout, char *tel, byte len, ushort *holdtime);
	int JugTelType( char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType );
	int JugTelType2( char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType,
		WORD* v_p_wLmtOutPeriod, WORD* v_p_wLmtInPeriod );
	int JugTelType3(char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType,
		WORD* v_p_wLmtOutPeriod, WORD* v_p_wLmtInPeriod);

private:

	int m_iPhonePort;	//�ֻ�ģ�鴮���豸�ļ����
	
	pthread_t m_hThdComRcv;	//���ڽ����߳̾��
	pthread_t m_hThdPhoneHdl;
	pthread_t m_hThdOtherHdl;
//	pthread_t m_hThdPing;

	//���̷߳��ʻ�����
	pthread_mutex_t 	m_crisecRcv;
	pthread_mutex_t 	m_crisecPhoneTask;
	pthread_mutex_t 	m_crisecSta;
	pthread_mutex_t 	m_crisecDog;

	CInnerDataMng m_objDataMngRcv;
	CInnerDataMng m_objDataMngSmsSnd;

	tagTdPhoneTask m_objPhoneTask;

	// ״̬���е�һ״̬ʹ�õ�״ֵ̬
	tagTdPhoneStaDial m_objPhoneStaDial;
	tagTdPhoneStaDialLsn m_objPhoneStaDialLsn;
	tagTdPhoneStaHold m_objPhoneStaHold;
	tagTdPhoneStaLsning m_objPhoneStaLsning;
	tagTdPhoneStaRing m_objPhoneStaRing;

	// ���״̬�����߳�Ҫʹ�õ�״ֵ̬
	tagTdPhoneStaCommon m_objPhoneStaCommon;

	DWORD m_dwClrDogTm;
	volatile int m_iPhoneHdlSta;
	volatile bool m_bPhoneNetReady;
	volatile bool m_bToClosePhone;
	DWORD m_dwDialNetTm;

};

#endif // #ifndef _PHONETD_H_
