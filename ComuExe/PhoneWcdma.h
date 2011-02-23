#ifndef _PHONEWCDMA_H_
#define _PHONEWCDMA_H_

#define WCDMA_FILE "/mnt/Flash/part3/WcdmaLog"
#define QUEUE_WCDMABUF_SIZE 1100000  // 1M�Ļ�������
#define QUEUE_WCDMABUF_PERIOD 0xfffffff8

#if RESET_ENABLE_ALL == 1
#define WCDMA_WRITE_FILE_CNT 200   //���������ж��ٸ����ݵ�ʱ���ȥд�ļ���
#else
#define WCDMA_WRITE_FILE_CNT 10   //���������ж��ٸ����ݵ�ʱ���ȥд�ļ���
#endif

// #define WCDMA_FILE_VER       "20100119"
// typedef STRUCT_1 atAck
// {
//     INT8U       numEC;                                      /* number of END'S characters */
//     INT16U      bufsize;                                    /* ack buffer size */
//     INT8U       ackbuf[512];                                     /* ack buffer */
//     INT8U       cb_index;                                   /* cmd data cb index */
// 	
// 	atAck()
// 	{
// 		memset(this,0,sizeof(*this));
// 	}
// }ATCMDACK_T;

enum WCDMA_ATANS
{
	WCDMA_ATANS_OK = 1,
	WCDMA_ATANS_ERROR,
	WCDMA_ATANS_CMEERR,
	WCDMA_ATANS_NOANS,
	WCDMA_ATANS_RCVNOTALL,
	// { �����ں������
	WCDMA_ATANS_NOANSWER,
	WCDMA_ATANS_NOCARRIER,
	WCDMA_ATANS_NODIALTONE,
	WCDMA_ATANS_BUSY,
	WCDMA_ATANS_NOCALL, // ����ͨ����
	// } �����ں������
	WCDMA_ATANS_OTHER
};

enum WCDMA_PHONESTA
{
	WCDMA_PST_INIT = 1,
	WCDMA_PST_IDLE,
	WCDMA_PST_DIALNET,
	WCDMA_PST_WAITDIALNET,
	WCDMA_PST_CUTNET,
	WCDMA_PST_RESET,
	WCDMA_PST_DIALLSN,
	WCDMA_PST_WAITLSNPICK,
	WCDMA_PST_LSNING,
	WCDMA_PST_DIAL,
	WCDMA_PST_WAITPICK,
	WCDMA_PST_HOLD,
	WCDMA_PST_HANGUP,
	WCDMA_PST_RCVRING
};

// enum WCDMA_PINGSTA
// {
// 	WCDMA_PINGST_IDLE = 666,
// 	WCDMA_PINGST_DOING,
// 	WCDMA_PINGST_SUCC,
// 	WCDMA_PINGST_FAIL,
// 	WCDMA_PINGST_CANCEL
// };

enum WCDMA_AUDIOMODE
{
	WCDMA_AUDIOMODE_HANDFREE = 1, //����// ��Ҫ̫��,�еĽṹ����ֻ��1���ֽڱ���
	WCDMA_AUDIOMODE_EARPHONE,		//����
	WCDMA_AUDIOMODE_LSN,			//����
	WCDMA_AUDIOMODE_DIAL,			//����ģʽ(�Ǽ�������)
	WCDMA_AUDIOMODE_CLOSE			//ͨ���ر�ģʽ
};

// ����ָ��壨�ɻ�������
const DWORD WCDMA_PHTASK_DIALSN		  = 0X00000001;	
const DWORD WCDMA_PHTASK_DIAL			  = 0X00000002;
const DWORD WCDMA_PHTASK_RING			  = 0X00000004;
const DWORD WCDMA_PHTASK_SETSMSCENT	= 0X00000008;
const DWORD WCDMA_PHTASK_SNDDTMF		= 0X00000010;
const DWORD WCDMA_PHTASK_PICKUP		  = 0X00000020;
const DWORD WCDMA_PHTASK_HANGUP		  = 0X00000040;
const DWORD WCDMA_PHTASK_CHGAUDIO		= 0X00000080;	//��������л�
const DWORD WCDMA_PHTASK_CHGVOL		  = 0X00000100;	//��������
const DWORD WCDMA_PHTASK_PING			  = 0x00000200;
const DWORD WCDMA_PHTASK_PINGRPT		= 0x00000400;
const DWORD WCDMA_PHTASK_CUTNET		  = 0x00000800;
const DWORD WCDMA_PHTASK_ALERM		  = 0x00001000;	//����
const DWORD WCDMA_PHTASK_RESET		  = 0x00002000;

// �������ݶ��� (���̷߳���,��Ҫ����)
struct tagWcdmaPhoneTask
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
	BYTE	m_bytChgAudio; // WCDMA_AUDIOMODE_HANDFREE,�л�������; WCDMA_AUDIOMODE_EARPHONE,�л�������
	int		m_iChgVol; // 1,����; -1,��С

//	bool	m_bPing;
	BYTE	m_bytPingSrc; // 1,QianExe; 2,StreamMedia (��Ĺ�ϵ )

	BYTE	m_bytPingRptSrc;
	BYTE	m_bytPingResult; // 0,�ɹ�; 1,ʧ��; 2,�쳣; 3,��ȡ��

//	bool	m_bCutNet;

//	DWORD	m_dwAlertBegin;
	DWORD	m_dwAlertPrid;

	tagWcdmaPhoneTask() { memset(this, 0, sizeof(*this)); }
};


/// { ״̬����ÿһ��״̬��Ӧ��״ֵ̬ (����״̬���߳���ʹ��,���軥�����)
struct tagWcdmaPhoneStaDial
{
	DWORD	m_dwDialBeginTm;
	DWORD	m_dwHoldLmtPrid;
	char	m_szDialTel[20];

	void Init() { memset(this, 0, sizeof(*this)); }
	tagWcdmaPhoneStaDial() { Init(); }
};

struct tagWcdmaPhoneStaHold
{
	DWORD	m_dwHoldBeginTm;
	DWORD	m_dwHoldLmtPrid;
	DWORD	m_dwCallChkTm;

	void Init() { memset(this, 0, sizeof(*this)); }
	tagWcdmaPhoneStaHold() { Init(); }
};

struct tagWcdmaPhoneStaDialLsn
{
	DWORD	m_dwDialBeginTm;
	char	m_szDialLsnTel[20];

	void Init() { memset(this, 0, sizeof(*this)); }
	tagWcdmaPhoneStaDialLsn() { Init(); }
};

struct tagWcdmaPhoneStaLsning
{
	DWORD	m_dwCallChkTm;

	void Init() { memset(this, 0, sizeof(*this)); }
	tagWcdmaPhoneStaLsning() { Init(); }
};

struct tagWcdmaPhoneStaRing
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
	tagWcdmaPhoneStaRing() { Init(); }
};
/// } ״̬����ÿһ��״̬��Ӧ��״ֵ̬


// ͨ��״̬������ (��λ����)
const DWORD WCDMA_PHSTVAR_IMEI		= 0X00000001;
const DWORD WCDMA_PHSTVAR_SING		= 0X00000002;
const DWORD WCDMA_PHSTVAR_ALERT		= 0X00000004;
const DWORD WCDMA_PHSTVAR_NET			= 0X00000008;
const DWORD WCDMA_PHSTVAR_PING		= 0X00000010;
const DWORD WCDMA_PHSTVAR_AUDIOMODE	= 0X00000020;
const DWORD WCDMA_PHSTVAR_AUDIOOUTLVL = 0x00000040;

/// { ״̬���ж��״̬����Ҫʹ�õ�״ֵ̬ (���ܶ��̷߳���,��Ҫ����)
struct tagWcdmaPhoneStaCommon
{
	char	m_szGsmIMEI[128];
	int		m_iSignal;

	DWORD	m_dwAlertBegin;
	DWORD	m_dwAlertPrid;

	int		m_iAudioOutVol; // ͨ��ʱ�����������: 0~5
	BYTE	m_bytCurAudioMode;

	bool	m_bNetConnected;

	BYTE	m_bytPingSrc; // 1,QianExe; 2,StreamMedia (��Ĺ�ϵ )

	void Init()
	{
		memset(this, 0, sizeof(*this));
		m_iAudioOutVol = 4;
	}

	tagWcdmaPhoneStaCommon()
	{
		Init();
	}
};
/// } ״̬���ж��״̬����Ҫʹ�õ�״ֵ̬

typedef int ( *PCHKATRES) (char* v_szAtRes, void* v_pResult, size_t v_sizResult );

class CPhoneWcdma
{
public:
	CPhoneWcdma();
	virtual ~CPhoneWcdma();
	void ResetPhone();
	void ReDialNet();
	BYTE GetPhoneState();
	int GetPhoneHdlSta();
	int GetPhoneSignal();
	void GetPhoneIMEI( char* v_szImei, size_t v_sizImei );
	int GetPhoneVolume();
	int InitPhone();
	void ReleasePhone();
	
	int _PppSigHandler();

protected:
	unsigned long P_PhoneHdl();
	friend unsigned long G_WCDMA_PhoneHdl(void* v_pPar);

	unsigned long P_ComRcv();
	friend unsigned long G_WCDMA_ComRcv(void* v_pPar);

	unsigned long P_OtherHdl();
	friend unsigned long G_WCDMA_OtherHdl(void* v_pPar);

//	unsigned long P_Ping();
	friend unsigned long G_WCDMA_Ping(void* v_pPar);

private:

	void _GsmHardReset();
	void _GsmReset();
	void _GsmPowOn();
	void _GsmPowOff();
	int _InitCom();
	int _CloseCom();
	int _OtherHdl();

	// { ����״̬���̵߳���
	int _ComSnd(void* v_pSnd, DWORD v_dwSndLen );
	int _SetAT( DWORD v_dwWaitPerTime, BYTE v_bytTryTimes, void* v_pResult, size_t v_sizResult,
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
	int _PhoneJugSmsRcv();
	void _PhoneJugSmsCentSet();
	void _PhoneJugSmsPingRpt();
	void _PhoneJugAlerm();
	BOOL _PhoneJugReset();
	BOOL _PhoneJugCutNet();

	int _PhoneAudioModeSet(int v_iChgToMode);
	int _PhoneAudioOutVolSet(int v_iChgDegree);

	int _PhoneSmsSnd( SM_PARAM& v_objSmPar );
	int _PushSndSms(char *to, byte tolen, char *msg, byte msglen, BYTE v_bytSrcType);
	bool sms_deal(SM_PARAM *psm);
	int _QueryRcvSMS();

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

	tagWcdmaPhoneTask _PhoneTaskGet( DWORD v_dwToGetTaskSymb );
	void _PhoneTaskSet( const tagWcdmaPhoneTask& v_objPhoneTask, DWORD v_dwToSetTaskSymb );
	void _PhoneTaskClr(DWORD v_dwToGetTaskSymb);
	void _ClearPhoneTask( DWORD v_dwToGetTaskSymb, tagWcdmaPhoneTask& v_objPhoneTask );

	tagWcdmaPhoneStaCommon _PhoneStaCommonGet( DWORD v_dwToClrStaSymb );
	void _PhoneStaCommonSet( const tagWcdmaPhoneStaCommon& v_objPhoneStaCommon, DWORD v_dwToSetStaSymb );
	void _PhoneStaCommonClr(DWORD v_dwToClrStaSymb);
	void _ClearPhoneStaCommon(tagWcdmaPhoneStaCommon& v_objPhoneStaCommon, DWORD v_dwToClrStaSymb);

	byte tel_type(bool callout, char *tel, byte len, ushort *holdtime);
	int JugTelType( char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType );
	int JugTelType2( char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType,
		WORD* v_p_wLmtOutPeriod, WORD* v_p_wLmtInPeriod );
	int JugTelType3(char* v_szTel, unsigned char v_ucTelLen, unsigned char* v_p_cTelType,
		WORD* v_p_wLmtOutPeriod, WORD* v_p_wLmtInPeriod);

private:
	//��վ�ɼ����
	bool m_bSaveExitFlag;
	volatile bool m_bSaveThRunSta;//���̰߳�ȫ

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
	CInnerDataMng m_objDataMngSmsRcv;

	tagWcdmaPhoneTask m_objPhoneTask;

	// ״̬���е�һ״̬ʹ�õ�״ֵ̬
	tagWcdmaPhoneStaDial m_objPhoneStaDial;
	tagWcdmaPhoneStaDialLsn m_objPhoneStaDialLsn;
	tagWcdmaPhoneStaHold m_objPhoneStaHold;
	tagWcdmaPhoneStaLsning m_objPhoneStaLsning;
	tagWcdmaPhoneStaRing m_objPhoneStaRing;

	// ���״̬�����߳�Ҫʹ�õ�״ֵ̬
	tagWcdmaPhoneStaCommon m_objPhoneStaCommon;

	DWORD m_dwClrDogTm;
	volatile int m_iPhoneHdlSta;
	volatile bool m_bPhoneNetReady;
	volatile bool m_bToClosePhone;
	volatile bool m_bNeedDialNet;
	DWORD m_dwDialNetTm;

	// private://��վ�ɼ����
// 	long m_GpsChangeBuf[64];
// 	bool m_bGpsChangeInitFlag;//gps�α��ʼ����־����gps��Ч�����¶�λ������£����������wg_flag0��ʼ�����ٽ����α�
// 	int m_ucCurCltCyc;
// 	unsigned long m_ulWcdmaIndex;
// 	INT16U s_sqm;
// 	ATDATA_CPSI_T s_cpsi_t;
// 	ATDATA_CRUS_T s_crus_t;
// 	
// 	CInnerDataMng   m_objWcdmaBaseQue;
// 	WCDMAFILE_HEAD   m_objWcdmaFileHead;
// 	byte m_bytServeType;
// 	ATCltCyc m_objWcdmaCltCyc;
// 	void _KillTimer(UINT v_uiTmID);
// 	UINT _SetTimer(UINT v_uiTmID, UINT v_uiInterval);
	
//	bool InitWcdmaFile();
//public:
//	void P_QueryWcdmaBase();
//	void P_SaveWcdmaData();
//	void SaveWcdmaDataRst();
//	bool g_bNeedUpLine;

//	bool query_CPSI();
//	bool query_CRUS();
//	bool Handler_AT_CPSI(INT8U *recvbuf, INT16U len, ATCMDACK_T *ATCmdAck);
//	bool Handler_AT_CRUS(INT8U *recvbuf, INT16U len, ATCMDACK_T *ATCmdAck);
// 
// 	INT8U CharToHex(INT8U schar)
// 	{
// 		if (schar >= '0' && schar <= '9') 
// 			return (schar - '0');
// 		else if (schar>='A' && schar<='F') 
// 			return (schar - 'A' + 0x0A);
// 		else if (schar>='a' && schar<='f') 
// 			return (schar - 'a' + 0x0A);
// 		else 
// 			return 0;
// 	}
// 
// 	BOOLEAN SearchKeyWord(INT8U *ptr, INT16U maxlen, char *sptr)
// 	{
// 		char *tmpptr = NULL;
// 
// 		if (*sptr == 0) return FALSE;
// 		tmpptr = sptr;
// 		while(maxlen > 0) 
// 		{
// 			if (*ptr == *tmpptr) 
// 			{
// 				if (*(tmpptr+1) == 0) 
// 					return TRUE;
// 			} 
// 			else 
// 			{
// 				if(tmpptr != sptr)
// 				{
// 				   tmpptr = sptr;
// 				   continue;
// 				}   
// 				tmpptr = sptr-1; //-1��Ϊtmpptr++������
// 			}
// 			ptr++;
// 			tmpptr++;
// 			maxlen--;
// 		}
// 		return FALSE;
// 	}
// 	
// 	INT32U SearchDigitalString(INT8U *ptr, INT16U maxlen, INT8U flagchar, INT8U numflag, INT8U base)
// 	{
// 		BOOLEAN find = FALSE;
// 		INT32U  result;
// 		INT8U *bptr = NULL, *eptr = NULL;
//     
// 		for(;;) 
// 		{
// 			if (maxlen == 0)
// 			{
// 				find = FALSE;
// 				break;
// 			}
// 			if (*ptr == flagchar) 
// 			{
// 				numflag--;
// 				if (numflag == 0) 
// 				{
// 					eptr = ptr;
// 					break;
// 				}
// 			}
// 			if((*ptr>='0'&&*ptr<='9')||(*ptr>='A'&&*ptr<='F')||(*ptr>='a'&&*ptr<='f'))
// 			{
// 				if (!find) 
// 				{
// 					find = TRUE;
// 					bptr = ptr;
// 				}
// 			} 
// 			else 
// 			{
// 				find = FALSE;
// 			}
// 			ptr++;
// 			maxlen--;
// 		}
//     
// 		if (find)
// 		{
// 			result = 0;
// 			for(;;) 
// 			{
// 				result = result*base + CharToHex(*bptr++);
// 				if (bptr == eptr) 
// 					break;
// 			}
// 		} 
// 		else 
// 		{
// 			result = 0xffff;
// 		}
// 		return result;
// 	}
// 
// 
// 	INT16U FindCharPos(INT8U *sptr, char findchar, INT8U numchar, INT16U maxlen)
// 	{
// 		INT16U pos;
//     
// 		if (maxlen == 0) 
// 			return 0;
// 		pos = 0;
// 		for (;;) 
// 		{
// 			if (*sptr++ == findchar) 
// 			{
// 				if (numchar == 0) 
// 					break;
// 				else 
// 					numchar--;
// 			}
// 			maxlen--;
// 			pos++;
// 			if (maxlen == 0) 
// 				break;
// 		}
// 		return pos;
// 	}
// 
// 	void Prase_DecimalandPoint(INT8U *ptr, INT16U maxlen, INT8U amply, INT16S *eptr)
// 	{
// 		BOOLEAN find_d, find_p, decimal;
// 		INT16S result;
//     
// 		find_d  = FALSE;
// 		find_p  = FALSE;
// 		decimal = FALSE;
// 		result  = 0;
//     
// 		for (;;) 
// 		{
// 			if ((0 == maxlen) || (0 == amply))  break; // �ַ�������, ��Ŵ����
// 
// 			if ('-' == *ptr) 
// 			{
// 				if (FALSE == decimal) 
// 				{
// 					decimal = TRUE;
// 					ptr++;
// 					maxlen--;
// 					continue;
// 				} 
// 				else 
// 					break;   
// 			}
//         
// 			if ('0' <= *ptr && '9' >= *ptr) 
// 			{
// 				if (FALSE == find_d) 
// 					find_d = TRUE;
// 				else if (TRUE == find_p) 
// 					amply--;
// 				result = result*10 + CharToHex(*ptr);
// 			} 
// 			else if ('.' == *ptr) 
// 			{
// 				if ((TRUE == find_d) && (FALSE == find_p)) 
// 					find_p = TRUE;
// 				else 
// 					break; 
// 			}
// 			ptr++;
// 			maxlen--;
// 		}
// 		if (FALSE == decimal) 
// 			*eptr = result;
// 		else 
// 			*eptr = 0 - result;
// 		return;
// 	}
};

#endif	//#ifndef _PHONEWCDMA_H_
