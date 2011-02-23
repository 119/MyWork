#ifndef _COMUEXEDEF_H_
#define _COMUEXEDEF_H_

#define IOSTA_ACC_ON				0x00000001
#define IOSTA_ALARM_ON				0x00000002
#define IOSTA_JIJIA_HEAVY			0x00000004
#define IOSTA_FDOOR_OPEN			0x00000008
#define IOSTA_BDOOR_OPEN			0x00000010
#define IOSTA_JIAOSHA_VALID			0x00000020
#define IOSTA_SHOUSHA_VALID			0x00000040
#define IOSTA_PASSENGER1_VALID		0x00000080
#define IOSTA_PASSENGER2_VALID		0x00000100
#define IOSTA_PASSENGER3_VALID		0x00000200

#if USE_LANGUAGE == 0
#define LANG_ACC_NEEDVALID ("ACC��Ч!�뽫ACC����Ϊ��Ч!")
#define LANG_IO_CHKMODE_OPEN ("������ʵʱ����Ѵ�! 5���Ӻ�ر�!")
#define LANG_IO_CHKMODE_CLOSE ("������ʵʱ���ģʽ�ѹر�!")
#define LANG_GPS_NOSIGNAL ("��ʱ��ȡ����GPS�ź�!���Ժ�...")
#define LANG_GPS_CHKMODE_OPEN ("GPSʵʱ����Ѵ�! 30���ر�!")
#define LANG_GPS_CHKMODE_CLOSE ("GPSʵʱ���ģʽ�ѹر�!")
#define LANG_CHKING_U ("���ڼ��U��...")
#define LANG_CHKU_SUCC ("U�̼��ɹ�!")
#define LANG_CHKU_FAIL ("U�̼��ʧ��,��U�̻�������̨!")
#define LANG_NK_UPGRADING ("�ں�������,�뱣�ֹ������������ĵȺ�")
#define LANG_NK_UPGRAD_SUCC ("�ں������ɹ�!ϵͳ��������...")
#define LANG_NK_UPGRAD_FILE_LENERR ("�ں�����,�ļ����ȴ���!")
#define LANG_NK_UPGRAD_FILE_NOFIND ("�ں�����,�ļ�û���ҵ�!")
#define LANG_NK_UPGRAD_FILE_OPENFAIL ("�ں�����,�ļ��򿪴���!")
#define LANG_NK_UPGRAD_FILE_READFAIL ("�ں�����,�ļ���ȡ����!")
#define LANG_NK_UPGRAD_TOCREADFAIL ("�ں�����,TOC��ȡʧ��!")
#define LANG_NK_UPGRAD_TOCWRITEFAIL ("�ں�����,TOCд��ʧ��!")
#define LANG_SFT_UPGRAD ("Ӧ���������")
#define LANG_SFT_UPGRADING ("Ӧ�����������...")
#define LANG_DATAOUTPUT ("���ڽ����ݵ���U��...")
#define LANG_SYS_TORESET ("ϵͳ��������...")
#define LANG_SUCC ("�ɹ�")
#define LANG_FAIL ("ʧ��")
#define LANG_ALA1_TEST ("�Ƽ۴��ڲ���")
#define LANG_COM6_TEST ("���ô��ڲ���")
#define LANG_TESTOILCUT_COMPLETE ("���Ͷϵ� �������!")
#define LANG_LOGGED ("�ѵ�½")
#define LANG_NOLOG ("δ��½")
#define LANG_SIGNAL ("�ź�")
#define LANG_ONLINE ("����")
#define LANG_OFFLINE ("����")
#define LANG_VALID ("��Ч")
#define LANG_INVALID ("��Ч")
#define LANG_CONNECT ("����")
#define LANG_DISCONNECT ("�Ͽ�")
#define LANG_NORMAL ("����")
#define LANG_ALERM ("����")
#define LANG_HEAVY ("�س�")
#define LANG_EMPTY ("�ճ�")
#define LANG_RESUMECFG ("�ָ���������")
#define LANG_SMSCENT ("�������ĺ�")
#define LANG_SPECCODE ("�ط���")
#define LANG_IO_METER ("�Ƽ����ź�")
#define LANG_METER_CONNECTION ("�Ƽ�������")
#define LANG_IO_POWDET ("��Դ")
#define LANG_IO_FOOTALERM ("�پ�")
#define LANG_IO_BROWNOUT ("��ѹ")
#define LANG_IO_OIL ("�ͺ�")
#define LANG_IO_OILPOW ("��ѹ")
#define LANG_IO_PULSE ("�������")
#define LANG_IO_SPEED ("�ٶ�")

#define LANG_IO_FDOOR ("ǰ����")
#define LANG_IO_BDOOR ("����")
#define LANG_IO_LOAD1 ("�ؿ�λ��1")
#define LANG_IO_LOAD2 ("�ؿ�λ��2")
#define LANG_IO_LOAD3 ("�ؿ�λ��3")

#define LANG_IO_QIANYA ("Ƿѹ�ź�")
#define LANG_IO_ELECSPEAKER ("���������ź�")
#define LANG_IO_FDOORALERT ("ǰ�ű����ź�")
#define LANG_IO_BDOORALERT ("���ű����ź�")
#define LANG_IO_JIAOSHA ("��ɲ�ź�")
#define LANG_IO_POWDETM ("��Դ����ź�")
#define LANG_IO_CHEDENG ("�����ź�")
#define LANG_IO_ZHENLING ("�����ź�")

#endif

#if USE_LANGUAGE == 11
#define LANG_ACC_NEEDVALID ("ACC Invalid! Make ACC Valid First")
#define LANG_IO_CHKMODE_OPEN ("IO Check Mode Opened! 5 Minutes Later Closes")
#define LANG_IO_CHKMODE_CLOSE ("IO Check Mode Closed!")
#define LANG_GPS_NOSIGNAL ("No GPS Signal, Please Waiting...")
#define LANG_GPS_CHKMODE_OPEN ("GPS Check Mode Opened! 30 Secs Later Closes!")
#define LANG_GPS_CHKMODE_CLOSE ("GPS Check Mode Closed!")
#define LANG_CHKING_U ("U Disk Searching...")
#define LANG_CHKU_SUCC ("U Disk Search Success!")
#define LANG_CHKU_FAIL ("U Disk Search Failure!")
#define LANG_NK_UPGRADING ("Maintain the power supply, Sys Upgrading...")
#define LANG_NK_UPGRAD_SUCC ("Sys Upgrade Succ, Resetting...")
#define LANG_NK_UPGRAD_FILE_LENERR ("Sys Upgrade, File Len Err!")
#define LANG_NK_UPGRAD_FILE_NOFIND ("Sys Upgrade, File Find Failure!")
#define LANG_NK_UPGRAD_FILE_OPENFAIL ("Sys Upgrade, File Open Failure!")
#define LANG_NK_UPGRAD_FILE_READFAIL ("Sys Upgrade, File Read Failure!")
#define LANG_NK_UPGRAD_TOCREADFAIL ("Sys Upgrade,Toc Read Fail!")
#define LANG_NK_UPGRAD_TOCWRITEFAIL ("Sys Upgrade,Toc Write Fail!")
#define LANG_SFT_UPGRAD ("App Soft Upgrade")
#define LANG_SFT_UPGRADING ("App Upgrading...")
#define LANG_SYS_TORESET ("Sys Resetting...")
#define LANG_DATAOUTPUT ("Data Output...")
#define LANG_SUCC ("Succ")
#define LANG_FAIL ("Fail")
#define LANG_ALA1_TEST ("ALA1 Port Test")
#define LANG_COM6_TEST ("COM6 Port Test")
#define LANG_TESTOILCUT_COMPLETE ("Oil Cut Test Complete!")
#define LANG_LOGGED ("Logged in")
#define LANG_NOLOG ("Logged out")
#define LANG_SIGNAL ("Signal")
#define LANG_ONLINE ("Online")
#define LANG_OFFLINE ("Offline")
#define LANG_VALID ("Valid")
#define LANG_INVALID ("Invalid")
#define LANG_CONNECT ("Connected")
#define LANG_DISCONNECT ("Disconnected")
#define LANG_NORMAL ("Normal")
#define LANG_ALERM ("Alerm")
#define LANG_RESUMECFG ("Resume Initial Config")
#define LANG_SMSCENT ("SmsCent")
#define LANG_SPECCODE ("SpecCode")
#define LANG_IO_METER ("Meter")
#define LANG_METER_CONNECTION ("Meter Connect")
#define LANG_IO_POWDET ("Pow Connect")
#define LANG_IO_FOOTALERM ("Rob Alerm")
#define LANG_IO_BROWNOUT ("Pow Voltage")
#define LANG_HEAVY ("Heavy")
#define LANG_EMPTY ("Empty")
#define LANG_IO_OIL ("Oil")
#define LANG_IO_PULSE ("Meter Pulse")
#define LANG_IO_SPEED ("Speed")

#define LANG_IO_FDOOR ("Front Door")
#define LANG_IO_BDOOR ("Back Door")
#define LANG_IO_LOAD1 ("Load Passenger Seat 1")
#define LANG_IO_LOAD2 ("Load Passenger Seat 2")
#define LANG_IO_LOAD3 ("Load Passenger Seat 3")
#endif



#if USE_LANGUAGE == 0
#define LANG_IOCHK_LIGHT ("��⵽ ���� �źű仯")
#define LANG_IOCHK_SPEAKER ("��⵽ �������� �źű仯")
#define LANG_IOCHK_CENTLOCK_OFF ("��⵽ �п����� �źű仯")
#define LANG_IOCHK_CENTLOCK_ON ("��⵽ �п����� �źű仯")
#define LANG_IOCHK_ALERM_LED ("��⵽ ����ָʾ�� �źű仯")
#define LANG_IOCHK_ALERMFOOT ("��⵽ ���ٱ��� �źű仯")
#define LANG_IOCHK_ACCON ("��⵽ ACC��Ч �źű仯")
#define LANG_IOCHK_ACCOFF ("��⵽ ACC��Ч �źű仯")
#define LANG_IOCHK_METERON ("��⵽ �Ƴ���Ч �źű仯")
#define LANG_IOCHK_METEROFF ("��⵽ �Ƴ���Ч �źű仯")
#define LANG_IOCHK_ALERM_POWDETON ("��⵽ �ϵ籨�� �źű仯")
#define LANG_IOCHK_ALERM_POWDETOFF ("��⵽ ��ƽ���� �źű仯")
#define LANG_IOCHK_DOOROPEN ("��⵽ ǰ���Ŵ� �źű仯")
#define LANG_IOCHK_DOORCLOSE ("��⵽ ǰ���Źر� �źű仯")
#define LANG_IOCHK_DCDHIGH ("��⵽ DCD�� �źű仯")
#define LANG_IOCHK_DCDLOW ("��⵽ DCD�� �źű仯")
#define LANG_IOCHK_RINGHIGH ("��⵽ RING�� �źű仯")
#define LANG_IOCHK_RINGLOW ("��⵽ RING�� �źű仯")
#define LANG_IOCHK_ALERMFOOT_CANCEL ("��⵽ ���ٱ���ȡ�� �źű仯")
#define LANG_IOCHK_BROWNOUT ("��⵽ Ƿѹ �źű仯")
#define LANG_IOCHK_OVERTURN ("��⵽ �෭ �źű仯")
#define LANG_IOCHK_OVERTURN_CANCEL ("��⵽ �෭ȡ�� �źű仯")
#define LANG_IOCHK_HANDBRAKE ("��⵽ ��ɲ �źű仯")
#define LANG_IOCHK_HANDBRAKE_CANCEL ("��⵽ ��ɲȡ�� �źű仯")
#define LANG_IOCHK_COLLISIONG ("��⵽ ��ײ �źű仯")
#define LANG_IOCHK_COLLISIONG_CANCEL ("��⵽ ��ײȡ�� �źű仯")
#define LANG_IOCHK_FOOTBRAKE ("��⵽ ��ɲ �źű仯")
#define LANG_IOCHK_FOOTBRAKE_CANCEL ("��⵽ ��ɲȡ�� �źű仯")
#define LANG_IOCHK_BACKDOOROPEN ("��⵽ ���Ŵ� �źű仯")
#define LANG_IOCHK_BACKDOORCLOSE ("��⵽ ���Źر� �źű仯")
#define LANG_IOCHK_LEFTLIGHT_ON ("��⵽ ��ת����� �źű仯")
#define LANG_IOCHK_LEFTLIGHT_OFF ("��⵽ ��ת����� �źű仯")
#define LANG_IOCHK_RIGHTLIGHT_ON ("��⵽ ��ת����� �źű仯")
#define LANG_IOCHK_RIGHTLIGHT_OFF ("��⵽ ��ת����� �źű仯")
#define LANG_IOCHK_FRONTLIGHT_ON ("��⵽ ǰ����� �źű仯")
#define LANG_IOCHK_FRONTLIGHT_OFF ("��⵽ ǰ����� �źű仯")
#define LANG_IOCHK_JIJIA_VALID ("��⵽ �Ƽ�����Ч �ź���Ч")
#define LANG_IOCHK_JIJIA_INVALID ("��⵽ �Ƽ�����Ч �ź���Ч")
#define LANG_IOCHK_PASSENGER1_VALID ("��⵽ �ؿ�1 ��Ч �źű仯")
#define LANG_IOCHK_PASSENGER1_INVALID ("��⵽ �ؿ�1 ��Ч �źű仯")
#define LANG_IOCHK_PASSENGER2_VALID ("��⵽ �ؿ�2 ��Ч �źű仯")
#define LANG_IOCHK_PASSENGER2_INVALID ("��⵽ �ؿ�2 ��Ч �źű仯")
#define LANG_IOCHK_PASSENGER3_VALID ("��⵽ �ؿ�3 ��Ч �ź���Ч")
#define LANG_IOCHK_PASSENGER3_INVALID ("��⵽ �ؿ�3 ��Ч �ź���Ч")
#define LANG_IOCHK_QIANYA_VALID ("��⵽ Ƿѹ ��Ч �źű仯")
#define LANG_IOCHK_QIANYA_INVALID ("��⵽ Ƿѹ ��Ч �źű仯")
#define LANG_IOCHK_ELECSPEAKER_VALID ("��⵽ �������� ��Ч �źű仯")
#define LANG_IOCHK_ELECSPEAKER_INVALID ("��⵽ �������� ��Ч �źű仯")
#define LANG_IOCHK_POWDETM_VALID ("��⵽ ��Դ��� ��Ч �źű仯")
#define LANG_IOCHK_POWDETM_INVALID ("��⵽ ��Դ��� ��Ч �źű仯")
#define LANG_IOCHK_CHEDENG_VALID ("��⵽ ���� ��Ч �źű仯")
#define LANG_IOCHK_CHEDENG_INVALID ("��⵽ ���� ��Ч �źű仯")
#define LANG_IOCHK_ZHENLING_VALID ("��⵽ ���� ��Ч �źű仯")
#define LANG_IOCHK_ZHENLING_INVALID ("��⵽ ���� ��Ч �źű仯")
#define LANG_IOCHK_FDOORALERT_VALID ("��⵽ ǰ�ű��� ��Ч �źű仯")
#define LANG_IOCHK_FDOORALERT_INVALID ("��⵽ ǰ�ű��� ��Ч �źű仯")
#define LANG_IOCHK_BDOORALERT_VALID ("��⵽ ���ű��� ��Ч �źű仯")
#define LANG_IOCHK_BDOORALERT_INVALID ("��⵽ ���ű��� ��Ч �źű仯")
#endif	//USE_LANGUAGE == 0

#if USE_LANGUAGE == 11
#define LANG_IOCHK_LIGHT ("IO: Light On")
#define LANG_IOCHK_SPEAKER ("IO: Electronic Speaker")
#define LANG_IOCHK_CENTLOCK_OFF ("IO: Central Locks Off")
#define LANG_IOCHK_CENTLOCK_ON ("IO: Central Locks On")
#define LANG_IOCHK_ALERM_LED ("IO: Alerm Led")
#define LANG_IOCHK_ALERMFOOT ("IO: Foot Alerm")
#define LANG_IOCHK_ACCON ("IO: ACC On")
#define LANG_IOCHK_ACCOFF ("IO: ACC Off")
#define LANG_IOCHK_METERON ("IO: Meter On")
#define LANG_IOCHK_METEROFF ("IO: Meter Off")
#define LANG_IOCHK_ALERM_POWDETON ("IO: Power Failure")
#define LANG_IOCHK_ALERM_POWDETOFF ("IO: Power Resume")
#define LANG_IOCHK_DOOROPEN ("IO: Front Door Open")
#define LANG_IOCHK_DOORCLOSE ("IO: Front Door Close")
#define LANG_IOCHK_DCDHIGH ("IO: DCD High")
#define LANG_IOCHK_DCDLOW ("IO: DCD Low")
#define LANG_IOCHK_RINGHIGH ("IO: RING High")
#define LANG_IOCHK_RINGLOW ("IO: RING Low")
#define LANG_IOCHK_ALERMFOOT_CANCEL ("IO: Foot Alerm Cancel")
#define LANG_IOCHK_BROWNOUT ("IO: Power Brownout")
#define LANG_IOCHK_OVERTURN ("IO: Overturn")
#define LANG_IOCHK_OVERTURN_CANCEL ("IO: Overturn Cancel")
#define LANG_IOCHK_HANDBRAKE ("IO: Hand Brake")
#define LANG_IOCHK_HANDBRAKE_CANCEL ("IO: Hand Brake Cancel")
#define LANG_IOCHK_COLLISIONG ("IO: Collision")
#define LANG_IOCHK_COLLISIONG_CANCEL ("IO: Collision Cancel")
#define LANG_IOCHK_FOOTBRAKE ("IO: Foot Brake")
#define LANG_IOCHK_FOOTBRAKE_CANCEL ("IO: Foot Brake Cancel")
#define LANG_IOCHK_BACKDOOROPEN ("IO: Back Door Open")
#define LANG_IOCHK_BACKDOORCLOSE ("IO: Back Door Close")
#define LANG_IOCHK_LEFTLIGHT_ON ("IO: Left Light On")
#define LANG_IOCHK_LEFTLIGHT_OFF ("IO: Left Light Off")
#define LANG_IOCHK_RIGHTLIGHT_ON ("IO: Right Light On")
#define LANG_IOCHK_RIGHTLIGHT_OFF ("IO: Right Light Off")
#define LANG_IOCHK_FRONTLIGHT_ON ("IO: Front Light On")
#define LANG_IOCHK_FRONTLIGHT_OFF ("IO: Front Light Off")
#define LANG_IOCHK_JIJIA_VALID ("IO: jijia valid")
#define LANG_IOCHK_JIJIA_INVALID ("IO: jijia invalid")
#define LANG_IOCHK_PASSENGER1_VALID ("IO: Passager load 1 valid")
#define LANG_IOCHK_PASSENGER1_INVALID ("IO: Passager load 1 invalid")
#define LANG_IOCHK_PASSENGER2_VALID ("IO: Passager load 2 valid")
#define LANG_IOCHK_PASSENGER2_INVALID ("IO: Passager load 2 invalid")
#define LANG_IOCHK_PASSENGER3_VALID ("IO: Passager load 3 valid")
#define LANG_IOCHK_PASSENGER3_INVALID ("IO: Passager load 3 invalid")
#endif	//USE_LANGUAGE == 11

#define BENQ		0
#define SIMCOM		1
#define DTTD		2
#define EVDO		3

#define DATA_UNIT_BUFF		1			//GPS����
#define GPS_BUFSIZE			1024
#define MIN_GPRS_SIGNAL		5			// �ܲ�GPRS����С�ź�ֵ

#define MAX_DEAL_BUF_SIZE	10240

#define KEYPATH				("/mnt/Flash/part3/for_key")  //һ��·����
#define SHM_SIZE			4096
#define GPS_PATH			("/SiRFNavIIIdemo")

#define SPEED_STOP		0.5 // ���ڸ��ٶ���Ϊ��ֹ (��/��)
#define SPEED_MOVE		1.5 // ���ڸ��ٶ���Ϊ���ƶ� (��/��) (SPEED_MOVE Ӧ���� SPEED_STOP)
#define SPEED_COUNT		10 // ȷ�Ͼ�ֹ/�ƶ�,��Ҫ�������ٴ��ж�

enum ENUGPSSTA
{
	stInit = 1,
	stReset,
	stGetData,
	stWaitKill,
	stSleep
};

enum TIMERS
{
	// CHandset
	HST_CONVERT_TIMER = 1,
	
	// CHandApp
	DRV_LOG_TIMER
};


const DWORD TIMEBELIV_GPSAPERIOD = 15 * 60000; // ��λ:����, GPS��Ч������ú�,ʱ�����
const DWORD TIMEUNBLV_GPSVMINPERIOD = 30000; // ��λ:����, GPS��Ч������ú�,ʱ�䲻����

//comu�����ڲ����ݶ��У�phone-diaodu
const BYTE INNERSYMB_PHONETODIAO = 0x01;
const BYTE INNERSYMB_PHONETOQIAN = 0x02;
const BYTE INNERSYMB_PHONETOLIU = 0x03;

#pragma pack(4)

typedef struct tagComPar
{
	UINT	uiPort;			//���ں�
	DWORD	dwBaudrate;		//������
	DWORD	dwByteSize;	//�ֽ�λ��
	DWORD	dwParity;		//��ż��־
	DWORD	dwStopBits;	//ֹͣλ����־
	DWORD	dwReadIntervalTimeout;
	DWORD	dwReadTotalTimeoutMultiplier;
	DWORD	dwReadTotalTimeoutConstant;
}COMPAR;

typedef struct tagSatelliteInfo
{
	BYTE	m_bytSatelliteCount;
	BYTE	m_arySatelliteInfo[32][2]; // ����ID(1~32) + �����(0~99)
	tagSatelliteInfo() { memset(this, 0, sizeof(*this)); }
}SATELLITEINFO;


// ������Ϣ
// typedef struct tagSatelliteInfo
// {
// 	BYTE	m_bytSatelliteCount;
// 	BYTE	m_arySatelliteInfo[32][2]; // ����ID(1~32) + �����(0~99)
// 	tagSatelliteInfo() { memset(this, 0, sizeof(*this)); }
// }SATELLITEINFO;

typedef struct itemmenu{
	INT8U       Attrib;					//cyh add: �˵�������
	char        ContentStr[20];
	void        (*EntryProc)(void);		//cyh add������˵�ǰ������ִ�еĺ����� 
	INT8U       NumChild;				//cyh add �м����˵���
	itemmenu	*ChildItem;				//cyh ָ������˵���������ָ��
} ITEMMENU_STRUCT;						//cyh add:����˵����������

typedef struct menuwin_st{
	INT8U   WinAttrib;
	void    (*Handler_KEY_ACK)(void);
	void    (*Handler_KEY_NAK)(void);
	void    (*Handler_KEY_FCN)(void);
	void    (*Handler_KEY_SEL)(void);
	INT16U  (*StoreProc)(INT8U *);				//cyh add:����ǰ�Ĳ˵��ṹ������ItemMenu���浽��ָ��ַ�����������ص��ϲ�˵�ʱ��ԭ�������ݲŲ��ᶪʧ
	INT16U  (*MaxRecordProc)(void);				//cyh add: ��ȡ��ǰ������Ӳ˵���
	INT16U  (*InitRecordProc)(void);			//cyh add: ���ص�ǰ�˵�������ģʽ????���ǳ�ʼ�������ڽ�������ʾ�Ĳ˵�????
	INT16U  (*ReadRecordProc)(INT16S, INT8U *);	//cyh add:��ȡ��ǰ�˵��µ�����(����?)��������������ռ���ֽ���. ��һ��������ʾ�˵��������ţ��ڶ���������ʾ�������ݵ�ָ�룬���� ��ȡ���ļ�¼ ��ռ���ֽ���
} MENUWIN_STRUCT;								//cyh add: ����˵�������ݵĹ��ܺ���

typedef struct prompt_st{
	INT8U   Mode;
	int   LiveTime;					/* UNIT: SECOND */
	INT8U  *TextPtr;
	INT16U  TextLen;
	void (*ExitProc)(void);
	void (*ExitKeyProc)(void);		//cyh add��ʱʱ�䵽���߰���OK��ʱ �Ĵ�����
} PROMPT_STRUCT;

typedef struct editor_st
{
	INT8U   Mode;
	INT16U  MinLen;
	INT16U  MaxLen;
	INT16U  LabelLen;
	char   *LabelPtr;
	INT16U  InitLen;
	INT8U  *InitPtr;
	INT8U  *StorePtr;
	void    (*ExitProc)(void);
	void    (*AckProc)(void);
	INT16U  (*CurLenProc)(INT16U len, INT8U *ptr);
	INT16U  (*MinLenProc)(INT16U len, INT8U *ptr);
	INT16U  (*MaxLenProc)(INT16U len, INT8U *ptr);
	bool	(*FilterProc)(INT8U  *ch);
} EDITOR_STRUCT;

typedef struct winstackentry_st{
	void    (*InitProc)(void);
	void    (*DestroyProc)(void);
	void    (*SuspendProc)(void);
	void    (*ResumeProc)(void);
	void    (*HdlKeyProc)(void);
} WINSTACKENTRY_STRUCT;

typedef struct
{
	char        pinyin[7];
	const char *PY_mb;
}PY_INDEX;


typedef struct key_st{
	INT8U   attrib;
	INT8U   key;
} KEY_STRUCT;

//GPSЭ������
#define DATA_LENGTH 150
#define GSV_DATA_LEN 512
struct data_unit
{
	char gga_data[ DATA_LENGTH];//for gga format.
	char rmc_data[ DATA_LENGTH];//for rmc format.
	char gsa_data[ DATA_LENGTH];//for gsa format.
	char vtg_data[ DATA_LENGTH];//for vtg format.
	
	char gsv_data[GSV_DATA_LEN];//for gsv format.
};//the  basic data uint.

//GPSЭ������
struct gps_data_share
{
	int flag;//this flag is used to show that whether the share_data is initialized or not.
	data_unit combine_data[DATA_UNIT_BUFF];//data_unit buffer space used to store data of different format. 
	struct timeval time;
#if VEHICLE_TYPE == VEHICLE_M
	unsigned n1_time;//GetTickTimeʱ���
#endif
	struct timezone time_zone;
	
	gps_data_share() 
	{ 
		memset(this, 0, sizeof(*this));
	}
};//the gps data controller


#pragma pack()

#pragma pack(1)

STRUCT_1 Frm10 {
	byte type;
	char dtmf;
	Frm10() { type = 0x10; }
};

STRUCT_1 Frm11 {
	byte type;
	char reply;
	Frm11() { type = 0x11; }
};

STRUCT_1 Frm12 {
	byte type;
	byte len;
	char num[256];
	Frm12() { memset(this, 0, sizeof(*this));  type = 0x12; }
};

STRUCT_1 Frm13 {
	byte type;
	char reply;
	Frm13() { type = 0x13; }
};

STRUCT_1 Frm14 {
	byte type;
	Frm14() { type = 0x14; }
};

STRUCT_1 Frm15 {
	byte type;
	char reply;
	Frm15() { type = 0x15; }
};

STRUCT_1 Frm16 {
	byte type;
	Frm16() { type = 0x16; }
};

STRUCT_1 Frm17 {
	byte type;
	char reply;
	Frm17() { type = 0x17; }
};

STRUCT_1 Frm18 {
	byte type;
	Frm18() { type = 0x18; }
};

STRUCT_1 Frm19 {
	byte type;
	byte len;
	char num[256];
	Frm19() { memset(this, 0, sizeof(*this));  type = 0x19; }
};

STRUCT_1 Frm20 {
	byte type;
	char dtmf;
	Frm20() { type = 0x20; }
};

STRUCT_1 Frm21 {
	byte type;
	Frm21() { type = 0x21; }
};

STRUCT_1 Frm22 {
	byte type;
	Frm22() { type = 0x22; }
};

STRUCT_1 Frm23 {
	byte type;
	Frm23() { type = 0x23; }
};

STRUCT_1 Frm24 {
	byte type;
	Frm24() { type = 0x24; }
};

STRUCT_1 Frm25 {
	byte type;
	byte level;
	Frm25() { type = 0x25; }
};

STRUCT_1 Frm26 {
	byte type;
	Frm26() { type = 0x26; }
};

STRUCT_1 Frm27 {
	byte type;
	byte reply;
	Frm27() { type = 0x27; }
};

STRUCT_1 Frm28 {
	byte type;
	byte dir;
	Frm28() { type = 0x28; }
};

STRUCT_1 Frm29 {
	byte type;
	byte reply;
	Frm29() { type = 0x29; }
};

STRUCT_1 Frm31 {
	byte type;
	byte reply;
	Frm31() { type = 0x31; }
};

STRUCT_1 Frm32 {
	byte type;
	byte reply;
	Frm32() { type = 0x32; }
};

// ����״̬֪ͨ 
STRUCT_1 Frm35 {
	byte type;
	byte reply;		//1:����ָ�(���ųɹ�)  2:����ָ�(Ping�ɹ�)   3:����Ͽ���	   4:֪ͨsock�������
	Frm35() { type = 0x35; }
};

STRUCT_1 Frm38 {
	byte type;
	byte source;
	char tel[16];
	char msg[160];
	Frm38() { memset(this, 0, sizeof(*this));  type = 0x38; }
};

STRUCT_1 Frm39 {
	byte type;
	byte reply;
	Frm39() { type = 0x39; }
};

STRUCT_1 FrmF1 {
	byte type;
	byte len;
	char num[256];
	FrmF1() { memset(this, 0, sizeof(*this));  type = 0xF1; }
};

STRUCT_1 FrmF2 {
	byte type;
	byte reply;
	FrmF2() { type = 0xF2; reply = 0x00; }
};

STRUCT_1 FrmF3 {
	byte type;
	byte reply;
	FrmF3() { type = 0xF3; reply = 0; }
};

STRUCT_1 FrmFF {	
	byte type;
	byte iSignal;
	FrmFF() { type = 0xff; iSignal = 0;}
};

STRUCT_1 DataFrame {
		int data_len;
		byte data[1024];
	};

STRUCT_1 GpsInfo {
	byte valid;		//�ֽڶ���Ϊ��01A00000   A��GPS������Чλ��0-��Ч��1-��Ч
	byte lat[4];		//γ��
	byte lon[4];		//����
	byte speed;		//�ٶ�
	byte dir;		//����
};

STRUCT_1 Frm04 {
	byte type;
	GpsInfo gps;
	Frm04() { type = 0x04; }
};


STRUCT_1 Frm08 {
	byte type;
	
	byte hour;
	byte minute;
	byte second;
	
	byte year;
	byte month;
	byte day;
	Frm08() { type = 0x08; }
};

STRUCT_1 Frm0A {
	byte type;
	byte full;
	Frm0A() { type = 0x0a; }
};

STRUCT_1 Frm60 {
	byte type;
	byte reply;
	byte speed;
	Frm60() { type = 0x60; }
};

STRUCT_1 Frm64 {
	byte type;
	byte reply;
	ushort num;
	byte id_len;
	byte id[8];
	Frm64() { type = 0x64; }
};

STRUCT_1 FrmA2 {
	byte type;
	byte reply;
	FrmA2() { type = 0xA2; }
};

STRUCT_1 FrmA4 {
	byte type;
	byte reply;
	FrmA4() { type = 0xA4; }
};

STRUCT_1 FrmA6 {
	byte type;
	byte flag;
	byte reply;
	FrmA6() { type = 0xA6; }
};

STRUCT_1 FrmA8 {
	byte type;
	byte flag;
	byte reply;
	FrmA8() { type = 0xA8; }
};

STRUCT_1 FrmAA {
	byte type;
	byte reply;
	FrmAA() { type = 0xAA; }
};

// STRUCT_1 FrmAE {
// 	byte type;
// 	byte reply;
// 	byte info[100];
// 	FrmAE() { type = 0xAE; }
// };
STRUCT_1 FrmAE {
	byte type;
	byte reply;
	FrmAE() { type = 0xAE; }
};


STRUCT_1 FrmCC {
	byte type;
	char ver[100];
	FrmCC() { memset(this, 0, sizeof(*this)); type = 0xCC; }
};

STRUCT_1 FrmE4 {
	byte type;
	DWORD id_car;
	char key_car[16];
	DWORD ip;
	ushort port;
	DWORD ip2;
	ushort port2;
	FrmE4() { type = 0xE4; }
};

STRUCT_1 FrmE5 {
	byte type;
	byte reply;
	DWORD id;
	FrmE5() { type = 0xE5; }
};

STRUCT_1 FrmE7 {
	byte type;
	byte reply;
	DWORD id_car;
	char key_car[16];
	DWORD ip;
	ushort port;
	DWORD ip2;
	ushort port2;
	FrmE7() { type = 0xE7; }
};

STRUCT_1 FrmEB {
	byte type;
	byte reply;
	FrmEB() { type = 0xEB; }
};

STRUCT_1 FrmED {
	byte type;
	byte reply;
	FrmED() { type = 0xED; }
};	

STRUCT_1 FrmEE {
	byte type;
	byte flag;
	FrmEE() { type = 0xEE; }
};

STRUCT_1 FrmOil{
	byte type;
	byte flag;
};

STRUCT_1 FrmC1{
	byte type;
	byte flag;
	FrmC1(){ type = 0xC1; }
};

STRUCT_1 UserSM {
	char mytel[11];		//�ֻ�����
	
	char add1;
	char ip1[12];		//ǧ����TCP IP
	
	char add2;
	char port1[5];		//ǧ����TCP Port
	
	char add3;
	char ip2[12];		//ǧ����UDP IP
	
	char add4;
	char port2[5];		//ǧ����UDP Port
	
	char add5;
	char ip3[12];		//��Ƶ�ϴ�UDP IP
	
	char add6;
	char port3[5];		//��Ƶ�ϴ�UDP Port
	
	char add7;
	char ip4[12];		//��ý��UDP IP
	
	char add8;
	char port4[5];		//��ý��UDP Port
	
	char add9;
	char ip5[12];		//����ƽ̨TCP IP
	
	char add10;
	char port5[5];		//����ƽ̨TCP Port
	
	char add11;
#if USE_LIAONING_SANQI == 0
		char apn[20];		//APN
#endif
#if USE_LIAONING_SANQI == 1
		char apn[41]; //�����żƷѺ��� �����ֺţ���Ϊ�ָ���
#endif
};

STRUCT_1 UserSM2 {
	char pass[10];		// ����
	char cmd[6];		// ����
};

//--------���ں�������------------------------
// �绰��¼
STRUCT_1 Book {
	byte idx;		//������
	char tel[15];	//�绰����
	char name[20];	//����
	byte limit;		//Ȩ������
	byte tmhold[2];	//ͨ��ʱ��
};	

// �绰��������
STRUCT_1 BookCfg {
	Book book[30];		//�绰��¼	
	char tel[5][15];	//�绰����
};
//-----------------------------------------------

#pragma pack()

void (*G_Signal(int signo, void(* func)(int v_signo)))(int);

#endif	//#define _COMUEXEDEF_H_
