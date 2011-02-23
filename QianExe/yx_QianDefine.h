#ifndef _YX_QIANDEFINE_H_
#define _YX_QIANDEFINE_H_

enum TIMERSNEW
{
	// CMonAlert
	MON_TIMER = 1,
	AUTO_REQLSN_TIMER,
	ALERT_SILENT_TIMER,
	ALERT_INVALID_TIMER,
	ALERT_TEST_TIMER,
	ALERT_INITQUERY_TIMER,

	// CRealPos
	POS_TIMER,
		
	// CBlk
	BLACK_TIMER,
	BLACK_RCVQUERY_TIMER,
	BLACK_SNDRES_TIMER,
		
	// CAutoRpt
	AUTO_RPT_TIMER,
		
	// CHelp
	AUTODIALHELPTEL_TIMER,
		
	// CComu
	COPYDATA_TIMER,
	CHKDIAODU_TIMER,

	// CSock
	TCP_LOG_TIMER,
	UDP_LOG_TIMER,
	TCP_RECONN_TIMER,
	SHOW_MSG_TIMER,
		
	//CIO
	ACCCHK_TIMER,

	// CDrvRecRpt
	DRVRECWIN_REUPLOAD_TIMER,
	DRVREC_STOPUPLOAD_TIMER,

	// CDriverCtrl
	DRIVERLOG_TIMER,
	ACCOFF_DRVRRELOG_TIMER,
	FSTDRIVERLOG_TIMER,
	
	// CDriveRecord
	DRIVERECORD_TIMER,

	// CAcdent
	ACDUPLOAD_TIMER,
	
	//KTMeterMod
	DOWN_TIMER,
	SETDEVICE_TIMER,
	
	//KTLED
	CLEARFRONT_TIMER,
	CLEARBACK_TIMER,
	SETLEDSTA_TIMER,
	
	//KTDriverCtrl
	RESNDPIC_TIMER,
	RESPIC_TIMER,
	
	//CDiaodu
	SETBESPOKE_TIMER,
	BESPOKENOT_TIMER,

	// CPhoto
	TIMERPHOTO_TIMER,
	UPLOAD_PHOTOWIN_TIMER,
	UPLOAD_PHOTOCHK_TIMER,

	// COil
	AUTO_RPTOIL_TIMER,

	// CAutoRptStation
	SET_CHECI_TIMER,

	// CDownLineFile
	SEND4353_TIMER,

	// CLN_Login
	REGISTER_TIMER,
	LOGIN_TIMER,
	HEART_KEEP_TIMER,
	
	// CLN_Report
	ALERT_REPORT_TIMER,
	AUTO_REPORT_TIMER,
	SPEED_CHECK_TIMER,
	AREA_CHECK_TIMER,
	STARTENG_CHECK_TIMER,
	SPEEDRPT_CHECK_TIMER,
	PARKTIME_CHECK_TIMER,
	OUTLINE_CHECK_TIMER,
	TIRE_CHECK_TIMER,
	MILE_CHECK_TIMER
};

enum ALERM_TYPE_0149
{
	ALERM_0149_1 = 1,	//01	���ٱ���
	ALERM_0149_2,		//02	��ײ����
	ALERM_0149_3,		//03	�෭����
	ALERM_0149_4,		//04	�ϳ�����
	ALERM_0149_5,		//05	����ε���Χ������
	ALERM_0149_6,		//06	ƫ������
	ALERM_0149_7,		//07	�ֶ����ٱ���
	ALERM_0149_8,		//08	ͣ���㱨��
	ALERM_0149_9,		//09	ͣ����ʱ����
	ALERM_0149_10,		//10	�𶯱���(��������෭����)
	ALERM_0149_11,		//11	�Ƿ������ű���
	ALERM_0149_12,		//12	�涨ʱ�������ʻ����
	ALERM_0149_13,		//13	���ٱ���
	ALERM_0149_14,		//14	Խ�籨��
	ALERM_0149_15,		//15	��ѹ����
	ALERM_0149_16,		//16	��·����
	ALERM_0149_17,		//17	���ٱ���
	ALERM_0149_18,		//18	���Ա���
	ALERM_0149_19,		//19	GPS�����쳣����
	ALERM_0149_20,		//20	��̨���Ǳ�ͨ���쳣����
	ALERM_0149_21,		//21	������1����
	ALERM_0149_22,		//22	������2����
	ALERM_0149_23,		//23	������1���ӱ���  Ҳ�Ƕ���
	ALERM_0149_24,		//24	������2���ӱ���  Ҳ�Ƕ���
	ALERM_0149_25,		//25	�Ƿ���������
};

enum DEV_STATE
{
	KT_METER=0x0001,
	KT_LED=0x02,
	KT_TOUCH=0x04,
	KT_PHOTO=0x08,
};

enum SMS_CODECVT
{
	CVT_NONE = 1,
	CVT_8TO6,
	CVT_8TO7
};

enum
{
	LED_CMD_INIT = 66,
	LED_CMD_LIGHT,
	LED_CMD_ALERM,
	LED_CMD_ALERMCANCEL,
	LED_CMD_CARMOVE,
	LED_CMD_CARSILENT
};

enum LEDSTATE
{
	stINIT = 0,
	stLOG,
	stWORK,
	stEXCP,
	stDOWN
};

enum ENU_RPTJUGRESULT	// �����ϱ�ʹ���жϽ��
{
	RPT_YES = 188,
	RPT_CLOSE,		// �ϱ����ر�
	RPT_OUTPRID,	// �����ϱ�ʱ�����
};

enum TRANS_MODE
{
	MODE_A = 1,
	MODE_B,
	MODE_C,
	MODE_D,
	MODE_E
};
const	double		LONLAT_DO_TO_L = 1000000.0;

#define INVALID_NUM_32	0xffffffff
#define INVALID_NUM_16	0xffff
#define INVALID_NUM_8	0Xff

#define LEDALARM_PRID			60	// LED��ʾ������ʱ��,��

// socket���Ͷ�����������
const BYTE DATASYMB_SMSBACKUP = 0x01; // ����ʹ�ö��ű���
const BYTE DATASYMB_USESMS = 0x02; // ǿ��ʹ�ö��ŷ���
const BYTE DATASYMB_415052 = 0x08;
const BYTE DATASYMB_0154 = 0x10;
const BYTE DATASYMB_OE60 = 0x20;
const BYTE DATASYMB_DONETCPFRAME = 0x80;

// sms ��������
const BYTE DATASYMB_RCVFROMSM = 61; // �Ӷ��Ŷ�����SMS��װ������
const BYTE DATASYMB_RCVFROMSOCK = 62; // ��sokcet������SMS��װ����

// Benqģ��
const double BENQ_SOCKSND_DELAYMULTI = 11.2 / 6.0; // �׽��ַ���ʱ�ײ������ĺ�ʱ����
const DWORD BENQ_SOCKSND_MAXBYTES_PERSEC = 900; // ÿ�������͵�����ֽ��� (������������) (Ҫ���ǵײ���������ʱ��Ҫ�����ֽ�)
// SIMCOM300Dģ��
const double SIM300D_SOCKSND_DELAYMULTI = 7.5 / 6.0; // �׽��ַ���ʱ�ײ��������ӳٱ���
const DWORD SIM300D_SOCKSND_MAXBYTES_PERSEC = 1800; // ÿ�������͵�����ֽ��� (������������,) (Ҫ���ǵײ���������ʱ��Ҫ�����ֽ�)

#define TCPSNDFAIL_MAXTIME	2 // TCP����ʧ�ܶ��ٴκ�(��Socketδ����),UDP��TCP�ȷ�
#define UDPSNDBYTES_SNDTCP 200 // UDP�������Ͷ����ֽں�,���ȷ���TCP����

// #define PHOTODISK	ResFlh3
// #define BLKDISK		ResFlh2

//#define DELPHOTO_MAXCOUNT	100

//#define ONEPHOTODIR_DAYS 4 // ÿ������,ÿ�����챣��һ����ƬĿ¼

//#define PHOTO_MAXCOUNT 10 // ÿ������ʱ��������մ��� (��һ�㱣��)
#define PHOTOQUEE_MAXCOUNT 40 // ���մ�����еĴ�С
#define UPLOADPHOTO_MAXCOUNT 50 // ��Ƭ�ϴ��Ļ�����
#define UPLOADPHOTO_WINFRAMETOTAL	16 // �ϴ���Ƭʱ,���ٰ�Ϊһ������
#define REUPLOADPHOTO_TIME 4 // �ش���Ƭ��Ϣ��������

const double BLKSAVE_MINSPEED = 0.5; // ��/��

const double MOVE_MINSPEED_GPS = 0.5; // ��Ϊ���ƶ�����С�ٶ�,��/�� (GPS��ʽ)
const double MOVE_MINSPEED_IO = 0.25; // ��Ϊ���ƶ�����С�ٶ�,��/�� (IO��ʽ)

const double MIMIAO_TO_HAIHOUR = 3.6 / 1.852;

const UINT AUTO_REQLSN_LONGINTERVAL		= 30000; // �������򱨾������绰�ĳ���� (����) (��Ϊ�ײ��ж�ռ�߻��߱��ֻ��Ҷϵ������Ҫ�ȽϾõ�ʱ��)
const UINT AUTO_REQLSN_SHORTINTERVAL	= 5000; // �������򱨾������绰�Ķ̼�� (����) (�ֻ�ģ��æʱ�Ķ̳��Լ��)


const DWORD QUEUE_RCVSMS_SIZE = 10000;
const DWORD QUEUE_TCPSND_SIZE = 200000;
const DWORD QUEUE_UDPSND_SIZE = 200000;
const DWORD QUEUE_ACDENT_SIZE = 10000;
const DWORD QUEUE_WAITSNDDRVREC_SIZE = 500000;

const DWORD QUEUE_RCVSMS_PERIOD = 60000; // ms
const DWORD QUEUE_TCPSND_PERIOD = 180000;
const DWORD QUEUE_UDPSND_PERIOD = 180000;
const DWORD QUEUE_ACDENT_PERIOD	= 600000;
const DWORD QUEUE_WAITSNDDRVREC_PERIOD = 0xfffffff8;

const DWORD	QUEUE_COMRCV_SIZE		= 10000;
const DWORD	QUEUE_COMRCVALID_PERIOD	= 60000;

const UINT MON_MININTERVAL	= 1000; // ���ڶ�����ʱ (����)
const UINT ALERT_SNDDATA_INTERVAL	= 30000;	// Ĭ�ϱ������ͼ�����ݼ�� (����)
const UINT ALERT_SNDDATA_PERIOD		= 180000;	// Ĭ�ϱ������ͼ������ʱ�� (����)
const UINT ALERT_SILENT_PERIOD		= 60000 * 600; // �������ھ�Ĭ״̬��ʱ�� (����)
const UINT ALERT_INVALID_PERIOD		= ALERT_SNDDATA_PERIOD + 5000; // ���������ʱ�䱨���ź���� (����)
const UINT ALERT_TEST_PERIOD		= 70000; // �������Եȴ�ʱ�� (����)
const UINT POS_INTERVAL				= 1000; // ����ʵʱ��λ (����)
const UINT BLACK_MININTERVAL		= 30; // ��ϻ����С������ (�� �ر�ע��!!!!)
const UINT BLACK_SAVEINTERVAL		= 300000; // ��ϻ������ʵ��д���� (����)
const UINT BLACKQUERY_MAXINTERVAL	= 60000; // ��ϻ�Ӳ�ѯ֡�´���������� (����)
const UINT BLACK_SNDINTERVAL		= 1000; // ��ʱ�ϴ���ϻ�����ݵļ�� (����)
//const UINT FOOTALERTVALID_INTERVAL	= 1000;
//const UINT POWALERTVALID_INTERVAL	= 1000;
const UINT FOOTALERTVALID_PERIOD	= 1800; // ��ס���ٱ������ض೤ʱ�����Ϸ������� (����)
const UINT POWALERTVALID_PERIOD		= 2000; // �յ��ϵ��źź�೤ʱ�����Ϸ������� (����)
const UINT ACCCHK_INTERVAL			= 1200000;
const UINT COPYDATA_INTERVAL		= 3000;

const UINT FIRSTPHOTO_INTERVAL		= 2000; // ��һ����Ƭ����ʱ (һ�������ͷ������һ��,xun,070828)
const UINT PHOTO_INTERVAL			= 500; // ���ռ�� (����)
const UINT UPLOAD_PHOTOWIN_INTERVAL	= 10000; // ��Ƭÿ���������ط�������ʱ��� (����) (��ͨ���)
const UINT CENTPHOTORES_PERIOD		= 3000; // һ�����ڷ��ͺ�ȴ�����Ӧ���ʱ�� (����)
const UINT JIJIAPHOTO_MININTERVAL	= 2500; // �Ƽ�����������,���ټ������ʱ�� (����)
const UINT DOORPHOTO_INTERVAL		= 1000; // ��ʱ����Ƿ�Ҫ���йس������յļ�� (ms)

const UINT ONEPHOTO_MAXPERIOD		= 18000; // ��ÿ����Ƭ�����ʱ�� (����)
const UINT ONEPHOTOUPLOAD_MAXPERIOD	= 10000 + UPLOAD_PHOTOWIN_INTERVAL;
const UINT ALERTFSTPHOTO_DELAYPERIOD= 2500; // ��������ʱ����ʱ������,�Ա�֤��һ�����������ܷ��ͳ�ȥ (����)

const UINT ALERTREQLSN_DEALYPERIOD	= 5000; // ���������ʱ�俪ʼ���������������� (����)

const DWORD BELIV_DATE	= 80801; // �˺��������Ϊ����Ч��,���ڱ�����Ƭ��Ŀ¼��
const DWORD UNBLV_DATE	= 10101; // ��������Ч,ͳһ��Ϊ��ֵ,���ڱ�����Ƭ��Ŀ¼��

const DWORD CENTLOCKON_DOORMSGPERIOD = 10000; // ���ܿغ󣬻��յ��೤ʱ��ĳ��ſ����źţ�����

//const DWORD ALERT_FOOT_TM_SHAKE	= 1505;	// �Ӵ���������ʼ,������������ʱ�䣬����
//const DWORD ALERT_TM_ACTIVE		= 182000; // �Ӵ���������ʼ,Ĭ�ϵı����ʱ��,���� (����һ�������ʱֵ)
//const DWORD ALERT_TM_SILENT		= 30 * 60 * 1000 + ALERT_TM_ACTIVE; // �Ӵ���������ʼ,Ĭ�ϵı�����Ĭʱ��

const WORD	PHOTOINDEX_MAX		= 65520; // ��Ƭ��������� (��1��ʼ�ģ�ӦС���������Ч������)
const WORD	PHOTOINDEX_INVALID	= 65535; // ��Ƭ����Ч������

const BYTE	PHOTO_QUALITYHIGH	= 55;
const BYTE	PHOTO_QUALITYMID	= 40;
const BYTE	PHOTO_QUALITYLOW	= 20;

const BYTE ALERT_STOP_PHONE_PRD = 255 ;//��λ��s��0-255

//const BYTE	PHOTO_UPLOAD_TIME	= 40; // ÿ����Ƭ�Ĺ����ϴ���ʱ(��)

#define AUTO_REQLSN_MAXTIMES	3 // ���������������
#if USE_LIAONING_SANQI == 0	
#define POS_CONTCOUNT 20 // ʵʱ��λ����(�ٶȡ���Χ����)�������ٸ�GPS�����������д���

#endif
#if USE_LIAONING_SANQI == 1						 //
#define POS_CONTCOUNT 10 // ʵʱ��λ����(�ٶȡ���Χ����)�������ٸ�GPS�����������д���
#endif
#define SPD_CONTCOUNT 5

#define TIMER_MAXPERIOD	300 // ÿ�ζ�ʱ�������ķѵ��������ʱ��(����),�������´ζ�ʱ���ٴ������磺��ϻ���ϴ����¹��ɵ��ϴ���

// ����״̬����
#define	ALERT_TYPE_FOOT		    0x00000001		// ���ٱ���
#define ALERT_TYPE_POWOFF	    0x00000002		// ����ϵ�
#define ALERT_TYPE_POWBROWNOUT	0x00000004	// ����Ƿѹ
#define ALERT_TYPE_FRONTDOOR	0x00000008		// �Ƿ���ǰ����
#define ALERT_TYPE_OVERSPEED	0x00000010		// ���ٱ���
#define ALERT_TYPE_DRIVE	    0x00000020		// �Ƿ���������
#define ALERT_TYPE_TIME		    0x00000040		// ʱ�������ʻ
#define ALERT_TYPE_RANGE	    0x00000080		// Խ��
#define ALERT_TYPE_RSPEED	    0x00000100		// ������
#define ALERT_TYPE_OVERTURN     0x00000200		// �෭
#define ALERT_TYPE_BUMP		    0x00000400		// ��ײ
#define ALERT_TYPE_BACKDOOR		0x00000800		// �Ƿ�������
#define ALERT_TYPE_BELOWSPEED	0x00001000		// ���ٱ���
												//
//��������ͨ
#define ALERT_TYPE_STARTENG		0x00002000		//��𱨾�
#define ALERT_TYPE_MILE			0x00004000		//��̱���
#define ALERT_TYPE_PARKTIMEOUT	0x00008000		//��ʱͣ��
#define ALERT_TYPE_AREA			0x00010000		//���򱨾�
#define ALERT_TYPE_TIMESPD		0x00020000		//ʱ����ٶ��쳣
#define ALERT_TYPE_OUTLINE		0x00040000		//ƫ������
#define ALERT_TYPE_TIRE			0x00080000		//ƣ�ͱ���
#define ALERT_TYPE_TIMERESPD	0x00100000		//��ʱ�������ٱ���
#define ALERT_TYPE_TIMEOUTLINE	0x00200000		//��ʱ��ƫ������
#define ALERT_TYPE_REPATIMEOUT  0x00400000		//����ͣ����ʱ����
												//
#define ALERT_TYPE_INAREA		0x00800000		//�����򱨾�
#define ALERT_TYPE_OUTAREA		0x01000000		//�����򱨾�
												//
#define ALERT_TYPE_INREGION		0x02000000		//�����ڱ���
#define ALERT_TYPE_OUTREGION	0x04000000		//�����ⱨ��
												//
#define ALERT_TYPE_INRESPD		0x08000000		//�����ڳ���
#define ALERT_TYPE_OUTRESPD		0x10000000		//�����ⳬ��												
#define ALERT_TYPE_INREPATOUT	0x20000000		//�����ڳ�ʱͣ��
#define ALERT_TYPE_OUTREPATOUT	0x40000000		//�����ⳬʱͣ��


#define ALERT_TYPE_TEST			0x80000000		// ��������

// IOģ���е��豸״̬���� (������һ���Ե�IO�ź�,�����º��޷���ѯ��IO�źŻ��豸����ָ��)
#define DEVSTA_LOCKENABLE	0x00000001 // �п�������/û��
#define DEVSTA_ACCVALID		0x00000002 // ACC��Ч/��Ч
#define DEVSTA_GPSON		0x00000004 // GPS�ϵ�/�ص�
#define DEVSTA_JIJIADOWN	0x00000008 // �Ƽ�������/���� (���ڼƳ���ʹ��ʱ)
#define DEVSTA_SYSTEMSLEEP	0x00000010	// ϵͳʡ��

// Comuģ���е�״̬����
#define COMUSTA_DIAODUCONN	0x00000001 // �������Ƿ�ͨ������/�쳣

// ������ʻ״̬ (������û�е�����ģ�����ṩ׼ȷ��ѯ��״̬)
#define VESTA_MOVE		0x00000001 // �ƶ�/��ֹ
#define VESTA_HEAVY		0x00000002 // �س�/�ճ�

#define DISCONNOILELEC_PERIOD 30000 // ���Ͷϵ綯������ʱ�� (����)
#define DISCONNOILELEC_DELAY_PERIOD 10000 // ��ʾ���Ͷϵ����ִ�� (����)

// ��Ƭ�����ļ���
#define CAMINDEXNAME	"Index.dat"

#define BLKREADUNIT		128 // �Ӻ�ϻ���ļ���ÿ�ζ�ȡ���ݵ������� (���ʹ�ö�ȡ���ֽ����Ǵش�С�ı���,һ����ϻ������16�ֽ�)
#define BLKSNDUNIT		5 // ÿ�η��ͺ�ϻ�����ݵ������� (���ǵ����ŵ��ֽ�������,5��������)

#define DSK2_ONEFILE_DAYS	8 // ÿ������,ÿ�����챣��һ����ϻ���ļ����¹��ɵ��ļ� (��2������)

const DWORD DISK2_RESERVE	 = 512000;//DISK2_CUSIZE * 500;
const DWORD DISK3_RESERVE	 = 5120000;//DISK3_CUSIZE * 2500;

// ��ϻ������ļ���С(һ����ϻ������16Bytes,30�뱣��һ��,8��һ���ļ�): 16 * 2 * 60 * 24 * 8 = 368,640
const DWORD BLK_MAXSIZE		= 16 * 20 * 60 * 24 * DSK2_ONEFILE_DAYS; // 8��һ���ļ�=3,686,400
const DWORD PHOTO_MAXSIZE	= 204800;
const DWORD ACD_MAXSIZE = 376 * 1000 * DSK2_ONEFILE_DAYS; // 8��һ���ļ�: 3,008,000

const BYTE DATASYMB_DIAODUTOQIAN = 0x01;
const BYTE DATASYMB_PHONETOQIAN = 0x02;
const BYTE DATASYMB_GPSTOQIAN = 0x04;


//-----------------------------------------------------------------------
//LED���
#define MAILCNT_BOHAI		252					//������������
#define RENEWMENUTICK		30*60*1000			//30*60*1000 //���½�Ŀ����ʱ
#define EXP1TICKMAX			20*1000				//��ͨ�쳣�ָ���ʱ,��
#define DOWNWAITTICK		10*60*1000			//�ش����ض�ʱ
#define UPDDERRTICK			30*60*1000			//��������ʱ
#define ERRTICK				10*60*1000			//LEDͨ�Ŵ���

#define PKT_BOHAI			512					//�����������ݰ���С
#define RESEND_BOHAI		4*1024/PKT_BOHAI	//�����ش�����
#define UPDTCNT_BOHAI		64*1024/PKT_BOHAI	//�������������ܰ���
#define VERCNT_BOHAI		8					//�����汾����

#define ALABUF				1500				//�շ�������

#define DOWNTIME1			01					//��̨���������Ŀ��ʱ���1
#define DOWNTIME2			02					//��̨���������Ŀ��ʱ���2

#define LOG_MAX				8000				//LED�����־���������				

//------------------------------------------------------------------------
//LED���뿪��
//#define SAVE_LEDDATA							//�Ƿ��¼LED��������
//#define LEDTIMEDOWN							//�Ƿ���ж�ʱ��������Ŀ������
//#define DELAYLEDTIME							//�Ƿ�ACC OFFʱ�Ľ�Ŀ������������

enum MSG_TYPE
{ 
	MSG_SAVRECSTA = 1,
	MSG_DRVECDTOSND,
	MSG_TCPTOSEND,
	MSG_PROGQUIT,
	MSG_IOEVENT,
	MSG_PHOTODONE,
};

typedef struct tagMsg
{
	long int m_lMsgType;
	unsigned char m_uszMsgBuf[MSG_MAXLEN];
	
	tagMsg() { memset( this ,0, sizeof(*this) ); }
}QIANMSG;

#pragma pack(1)	// !!!!!�ر�ע���ڴ�1�ֽڶ���������������, ��Ҫ�ǿ��ܲ���ֱ�ӷ��ʽṹ���ڵĳ�Ա!!!!!

/// { �׽��ֲ�ֱ�Ӵ���

// ��̨->����


// ����->��̨

/// } �׽��ֲ�ֱ�Ӵ���


/// { ҵ��㴦��
// GPS��������(1)+�ο�����(3)+GPS����(n)+GPS����У����(1)
// ��׼GPS����: Time(3)+Hbit(1)+Latitude(4)+Longitude(4)+Vector(1)+Direction(1)
STRUCT_1 tagQianGps // �Ѷ���
{
	// �����Ǳ���ʱ��,ʱ���Ǹ�������ʱ��
	BYTE		m_bytGpsType;
	BYTE		m_bytYear; // 2λ���ֱ�ʾ��ֵ
	BYTE		m_bytMon;
	BYTE		m_bytDay;
	BYTE		m_bytHour;
	BYTE		m_bytMin;
	BYTE		m_bytSec;
	BYTE		m_bytMix;	// 01AS0LGV: 
							// A��GPS������Чλ��0-��Ч��1-��Ч
							// L����̨����״̬��0���ճ���1���س�
							// S��ʡ��ģʽ��0����ʡ��ģʽ��1��ʡ��ģʽ
							// G�������ֶ��ж��ֽڵ����λ
							// V���ٶ��ֶ����λ
	BYTE		m_bytLatDu;
	BYTE		m_bytLatFenzen;
	BYTE		m_bytLatFenxiao1;
	BYTE		m_bytLatFenxiao2;
	BYTE		m_bytLonDu;
	BYTE		m_bytLonFenzen;
	BYTE		m_bytLonFenxiao1;
	BYTE		m_bytLonFenxiao2;
	BYTE		m_bytSpeed; // ����/Сʱ
	BYTE		m_bytDir;
	BYTE		m_bytChk;

	tagQianGps()
	{
		memset( this, 0, sizeof(*this) );
	}
};

// ��ϻ��GPS: ����(3)+ʱ��(3)+Hbit(1)+γ��(4)+����(4)+�ٶ�(1)
STRUCT_1 tag0e60BlackGps // �Ѷ���,16 Bytes
{
	// ���ں�ʱ�̾��Ǳ���ʱ��
	BYTE		m_bytYear; // 2λ���ֱ�ʾ��ֵ
	BYTE		m_bytMon;
	BYTE		m_bytDay;
	BYTE		m_bytHour;
	BYTE		m_bytMin;
	BYTE		m_bytSec;
	BYTE		m_bytMix;
	BYTE		m_bytLatDu;
	BYTE		m_bytLatFenzen;
	BYTE		m_bytLatFenxiao1;
	BYTE		m_bytLatFenxiao2;
	BYTE		m_bytLonDu;
	BYTE		m_bytLonFenzen;
	BYTE		m_bytLonFenxiao1;
	BYTE		m_bytLonFenxiao2;
	BYTE		m_bytSpeed; // ����/Сʱ
#if 2 == USE_PROTOCOL
	BYTE		m_bytIOSta; // �ݲ�ʹ��
	BYTE		m_bytHeavyIO; // �ݲ�ʹ��
#endif

	tag0e60BlackGps() { memset( this, 0, sizeof(*this) ); }
};

STRUCT_1 tag1007Interv
{
	BYTE		m_bytGpsType;
	BYTE		m_bytHourInterv;
	BYTE		m_bytMinInterv;
	BYTE		m_bytHunKilo;
	BYTE		m_bytKilo;
};

STRUCT_1 tag1007Time
{
	BYTE		m_bytNo;
	BYTE		m_bytHour;
	BYTE		m_bytMin;
};

STRUCT_1 tag1007Area
{
	BYTE		m_bytAreaProCode;
	BYTE		m_bytLonMix;
	BYTE		m_bytLonWidFenZhen;
	BYTE		m_bytLonWidFenxiao1;
	BYTE		m_bytLonWidFenxiao2;
	BYTE		m_bytLatMix;
	BYTE		m_bytLatHeiFenZhen;
	BYTE		m_bytLatHeiFenxiao1;
	BYTE		m_bytLatHeiFenxiao2;
	BYTE		m_bytLonDu;
	BYTE		m_bytLonFenzen;
	BYTE		m_bytLonFenxiao1;
	BYTE		m_bytLonFenxiao2;
	BYTE		m_bytLatDu;
	BYTE		m_bytLatFenzen;
	BYTE		m_bytLatFenxiao1;
	BYTE		m_bytLatFenxiao2;
};

// ��Ӧ��Э�飺״̬��ʶ��1�������ҵ�����ͣ�1���� ���ʱ�䣨2���� ������ڣ�2���� ������루2��
STRUCT_1 tag103bMonPar
{
	BYTE	m_bytStaType;
	BYTE	m_bytMonType;
	BYTE	m_bytMonTimeHour;
	BYTE	m_bytMonTimeMin;
	BYTE	m_bytMonIntervMin;
	BYTE	m_bytMonIntervSec;
	BYTE	m_bytMonSpaceKilo;
	BYTE	m_bytMonSpaceTenmeter;
};

// ��ʻ��¼ (�ڴ���ʹ��,ͬʱ����Ϊ�ļ��������ݵĸ�ʽ,��˾�������HDOP�Ķ��������ֿ���ΪЭ���еĲ�������)
// �ýṹ������ö�ʹ�õ��ֽڵ�����,��ΪЭ����ص����ݶ���Ҫ��1�ֽڶ���
// �ýṹ��Ķ��������׸ı�,��Ϊ�Գ������ڿ�������ʱ���㿽��λ�úͿ�����Сʱ��Ӱ��
STRUCT_1 tagSampleData_3745_KJ2
{
	BYTE	m_bytYear;
	BYTE	m_bytMon;
	BYTE	m_bytDay;
	BYTE	m_bytHour;
	BYTE	m_bytMin;
	BYTE	m_bytSec;
	char	m_szDriverNo[2]; // ˾�����,htons( WORD )
	char	m_szLat[4];
	char	m_szLon[4];
	BYTE	m_bytSpeed; // ��ʱ�ٶ�,����/Сʱ
	BYTE	m_bytDir;
	BYTE	m_bytIO;
	BYTE	m_bytSta1;
	BYTE	m_bytSta2;
	BYTE	m_bytPDop; // GPS������ӡ�10,��ͬ
	BYTE	m_bytVDop;
	BYTE	m_bytHDop;
};

STRUCT_1 tagSampleData_3746
{
	BYTE	m_bytYear;
	BYTE	m_bytMon;
	BYTE	m_bytDay;
	BYTE	m_bytHour;
	BYTE	m_bytMin;
	BYTE	m_bytSec;
	char	m_szDriverNo[2]; // ˾�����,htons( WORD )
	char	m_szLat[4];
	char	m_szLon[4];
	BYTE	m_bytSpeed; // ��ʱ�ٶ�,����/Сʱ
	BYTE	m_bytDir;
	BYTE	m_bytIO;
	BYTE	m_bytSta1;
	BYTE	m_bytSta2;
	BYTE	m_bytAlertIO;
	BYTE	m_bytHeightCount; // ���ش���������,����=0
	WORD	m_aryHeightIO[4]; // ���ش���������,���޸���,ʱ��ʱ,��Ϊ�ڴ����Ҫ��,����ʹ��memcpy,��Ҫ��=��ֵ!!!
};

typedef struct tagSampleData_3746 tagSampleData_3747;

STRUCT_1 tag4144PhotoIndex
{
	BYTE	m_bytEvtType;
	WORD	m_wIndex;
	char	m_szDateTime[6];
};

/// ��������
STRUCT_1 tag0501Data {
	BYTE	m_bytLen;			//���ݳ���
	BYTE	m_bytCheck;			//У���
	BYTE	m_szRouter[4];		//·����Ϣ
	BYTE	m_bytDataType;		//�������� 02Ϊ����ָʾ
	char	m_szData[200];		//�û�����
	tag0501Data()	{ memset(this, 0, sizeof(*this)); }
};


STRUCT_1 tag0101
{
	BYTE	m_bytMonType;
	BYTE	m_bytTimeHour;
	BYTE	m_bytTimeMin;
	BYTE	m_bytIntervMin;
	BYTE	m_bytIntervSec;
	BYTE	m_bytSpaceKilo;
	BYTE	m_bytSpaceTenmeter;
#if 2 == USE_PROTOCOL
	BYTE	m_bytUpGpsHund;
	BYTE	m_bytUpGpsCount;
#endif
};

STRUCT_1 tag0103
{
#if QIANTYPE_USE == QIANTYPE_JG2 || QIANTYPE_USE == QIANTYPE_JG3 || QIANTYPE_USE == QIANTYPE_VM1 || QIANTYPE_USE == QIANTYPE_V7
	char	m_szHandtel[15];
#endif
	tag0101	m_objO101;
};

// struct tag0104
// {
// 	BYTE	m_bytAnsType;
// };

STRUCT_1 tag0105
{
	BYTE	m_bytCmdType;
	tag0101	m_obj0101;
};

STRUCT_1 tag0141
{
	BYTE	m_bytAnsType;
	tagQianGps	m_objQianGps;
};

// struct tag0142
// {
// 	BYTE	m_bytAnsType;
// };
// 
// struct tag0143
// {
// 	BYTE	m_bytAnsType;
// };
// 
// struct tag0144
// {
// 	BYTE	m_bytEndType;
// };

STRUCT_1 tag0145
{
	BYTE	m_bytAlertHi;
	BYTE	m_bytAlertLo;
	BYTE	m_bytMonCentCount;
	tagQianGps	m_objQianGps;
#if 2 == USE_PROTOCOL
	char		m_szAlertAreaCode[2];		
#endif

	tag0145()
	{
		memset( this, 0, sizeof(*this) );
#if 2 == USE_PROTOCOL
		m_szAlertAreaCode[0] = m_szAlertAreaCode[1] = 0x7f; // Ĭ��������Ϊȫ0
#endif
	}
};
typedef struct tag0145 tag0154;

STRUCT_1 tag0149
{
	BYTE	m_bytAlertHi;
	BYTE	m_bytAlertLo;
	BYTE	m_bytMonCentCount;
	tagQianGps	m_objQianGps;

	tag0149()
	{
		memset( this, 0, sizeof(*this) );
	}
};

STRUCT_1 tag0151
{
	BYTE	m_bytAnsType;
	tagQianGps	m_objQianGps;
//	char	m_szStatus[2];

	tag0151()
	{
		memset(this, 0, sizeof(*this));
		//m_szStatus[0] = m_szStatus[1] = 1;
	}
};

// ����鳵����
STRUCT_1 tag0136
{
	BYTE	m_bytQueryIndex;		//��ѯ������
	BYTE	m_szMonId[8];			//����ն�ID
	BYTE	m_bytZoneCount;			//�������
	
	struct tagZone {				
		BYTE	hbit1;
		BYTE	lat1[4];
		BYTE	lon1[4];
		BYTE	hbit2;
		BYTE	lat2[4];
		BYTE	lon2[4];
	} m_Zone[256];					//����

};

#if 2 == USE_PROTOCOL
STRUCT_1 tag0156
{
	BYTE	m_bytAlertHi;
	BYTE	m_bytAlertLo;
	BYTE	m_bytMonCentCount;
	tagQianGps	m_objQianGps;
	BYTE	m_bytDriverNo;
	char	m_szAlermAreaCode[2];

	tag0156()
	{
		memset( this, 0, sizeof(*this) );
		m_bytDriverNo = 0x7f;
		m_szAlermAreaCode[0] = m_szAlermAreaCode[1] = 0x7f;
	}
};
#endif

// ����鳵����Ӧ��
STRUCT_1 tag0176
{
	BYTE	m_bytQueryIndex;		//��ѯ������
	BYTE	m_szMonId[8];			//����ն�ID
	BYTE	m_bytZoneCount;			//�������
	
	BYTE	m_szCarStat[2];			//����״̬
	tagQianGps m_objQianGps;		//��׼GPS
};

STRUCT_1 tag0241
{
	BYTE	m_bytConType;
	BYTE	m_bytConNo;
	BYTE	m_bytAlertHi;
	BYTE	m_bytAlertLo;
	tagQianGps m_objQianGps;

	tag0241() { memset(this, 0, sizeof(*this)); }
};

STRUCT_1 tag0341
{
	BYTE	m_bytHelpType;
	tagQianGps m_objQianGps;
};

STRUCT_1 tag0e20 // ����������壬���ܰ�ͨ����ʽʹ��
{
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	char	m_szCentGroup[10];
#endif
	BYTE	m_bytMaxFrameCount;
	BYTE	m_bytPeriodCount; // ���ֶ�ǰ(�������ֶ�)��ͨ����ʽʹ��
	BYTE	m_szPeriod[ 10 * 5 ];
	BYTE	m_bytAreaCount;
	BYTE	m_szArea[ 16 * 20 ];
	BYTE	m_bytReqFrameTotal;
	BYTE	m_bytReqFrameNo; // ��1��ʼ���

	tag0e20() { memset(this, 0, sizeof(*this)); }
};

typedef struct tag0e20 tag0e10;

STRUCT_1 tag0e21
{
	char	m_szCentGroup[10];
};

struct tag0e60 // ����������壬���ܰ�ͨ����ʽʹ��
{
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	char	m_szCentGroup[10];
#endif
	BYTE	m_bytResType;
	BYTE	m_bytResFrameNo; // Ҳ��1��ʼ��� (����Э��û��˵)
	BYTE	m_bytGpsCount; // ���ֶ�ǰ(�������ֶ�)��ͨ����ʽʹ��
	tag0e60BlackGps	m_aryBlackGps[BLKSNDUNIT];
	BYTE	m_bytFitAreaCount;
	BYTE	m_bytFitTotalHi;
	BYTE	m_bytFitTotalLo;

	tag0e60() { memset(this, 0, sizeof(*this)); }
};

typedef struct tag0e60 tag0e50;

STRUCT_1 tag0e61
{
	char	m_szCentGroup[10];
	BYTE	m_bytResType;
};

STRUCT_1 tag1001
{
	char	m_szSmsCentID[22];	// �������ĺ�,������ո�
};

STRUCT_1 tag1002
{
	char	m_szSmsTel[15];
	char	m_szTaxHandtel[15];
	char	m_szDiaoduHandtel[15];
};

STRUCT_1 tag1003 // ���ߴ綨��
{
	BYTE	m_bytLsnTelCount;
	char	m_aryLsnHandtel[5][15];
};

STRUCT_1 tag1007
{
	char	m_szHandTel[15];
	BYTE	m_aryControl[2];
	BYTE	m_bytCondCount;
	char	m_szCond[ 17 * 30 ];
};

STRUCT_1 tag1008 // ���ߴ綨��
{
	BYTE	m_bytSetCtrl;
	BYTE	m_bytTelCount;
	char	m_szTelInfo[16*16];
};

STRUCT_1 tag1012
{
	BYTE	m_bytMin;
	BYTE	m_bytSec;
};

// struct tag1042
// {
// 	BYTE	m_bytAnsType;
// };

STRUCT_1 tag103e
{
	BYTE	m_bytSndWinSize; // ���ڴ�С
#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4
	BYTE	m_bytOnSndHour; // ����ʱ�� (h)
	BYTE	m_bytOnSndMin; // ����ʱ�� (m)
#endif
	BYTE	m_bytOnSampCycleMin; // ACC ON ��������,����
	BYTE	m_bytOnSampCycleSec; // ACC ON ��������,��
	BYTE	m_bytOnSndCycle; // ACC ON,��������,��
#if USE_DRVRECRPT == 1 || USE_DRVRECRPT == 2 || USE_DRVRECRPT == 3 || USE_DRVRECRPT == 4
	BYTE	m_bytOffSndHour; // ����ʱ�� (h)
	BYTE	m_bytOffSndMin; // ����ʱ�� (m)
#endif
	BYTE	m_bytOffSampCycleMin; // ACC OFF ��������,����
	BYTE	m_bytOffSampCycleSec; // ACC OFF ��������,��
	BYTE	m_bytOffSndCycle; // ACC OFF,��������,��

	tag103e() { memset( this, 0, sizeof(*this) );}
};

STRUCT_1 tag103f
{
	char	m_szSmsCentID[22];	// �������ĺ�,������ո�
	char	m_szUserIdentCode[6]; // �û�ʶ����	
};

STRUCT_1 tag105a
{
	BYTE	m_bytSta_1;
	BYTE	m_bytSta_2;
#if 2 == USE_PROTOCOL || 30 == USE_PROTOCOL
	BYTE	m_bytIO; // ������״̬
#endif
	tagQianGps objQianGps;

	tag105a() { memset(this, 0, sizeof(*this)); }
};

STRUCT_1 tag107c
{
	BYTE	m_bytIPNo;
	BYTE	m_bytResType;
};

STRUCT_1 tag3341
{
	BYTE	m_bytAnsType;
	BYTE	m_bytFrameNo;
};

STRUCT_1 tag3644
{
	BYTE	m_bytRestype;
	BYTE	m_bytFrameNo;
	BYTE	m_bytType;
#if USE_ACDENT == 1
	BYTE	m_bytDriverNo;
#endif
#if USE_ACDENT == 2
	char	m_szDriverNo[2]; // htons( WORD )
#endif
	char	m_szStopTime[6];
	union
	{
		BYTE	m_arySpeedIO[50][2]; // (1�ֽ�speed + 1�ֽ�IO) * 50
		BYTE	m_aryLonlat[10][8]; // (4�ֽ�Lon + 4�ֽ�Lat) * 10
	} m_uniAcdentData;
};

STRUCT_1 tag3743
{
	BYTE	m_bytResType;
	BYTE	m_bytAreaNo;
};

STRUCT_1 tag3744
{
	BYTE	m_bytAreaNo;
	tagQianGps m_objQianGps;
};

STRUCT_1 tag4152
{
	BYTE	m_bytSymb;
	char	m_szIndex[2];
	char	m_szFrameTotal[2];
	char	m_szFrameNo[2];
	BYTE	m_bytReSndTime;
};

// �绰��¼
STRUCT_1 Book {
	byte idx;		//������
	char tel[15];	//�绰����
	char name[20];	//����
	byte limit;		//Ȩ������
	byte tmhold[2];	//ͨ��ʱ��
};	

// ���õ绰���뱾����
STRUCT_1 frm3301 {
	byte frmno;			//֡����
	byte cnt;			//���ø���
	Book book[30];		//�绰��¼		
};

// ���õ绰���뱾Ӧ��
STRUCT_1 frm3341 {
	byte reply;			//Ӧ������
	byte frmno;			//֡����
};

// �������ķ���绰����
STRUCT_1 frm3302 {
	byte cnt;			//����
	char tel[5][15];	//�绰����
};

// �������ķ���绰����Ӧ��
STRUCT_1 frm3342 {
	byte reply;			//Ӧ������
};

// �绰��������
STRUCT_1 BookCfg {
	Book book[30];		//�绰��¼	
	char tel[5][15];	//�绰����
};

STRUCT_1 tag4216
{
	WORD m_usLindId;
	
	tag4216(){memset(this,0,sizeof(*this));}
};

STRUCT_1 tag4256
{
	WORD m_usLindId;
	byte m_bytRes;
	
	tag4256(){memset(this,0,sizeof(*this));}
};

STRUCT_1  tag4246 // ��վ��վ����
{
	WORD	usLineNo;
	WORD	usStaNo;
	WORD	bytYear;
	byte	bytMon;
	byte	bytDay;
	byte	bytHour;
	byte	bytMin;
	byte	bytSec;
	byte	bytAlemType;
	//		DWORD	usTmspan;
	byte    bytTmspanMin;
	byte    bytTmspanSec;
	
	tag4246()
	{
		memset(this,0,sizeof(*this) );
	}
}; 

STRUCT_1  tag4247 // ����վ����
{
	WORD	usLineNo;
	WORD	usStaNo;
	WORD	usYear;
	byte	bytMon;
	byte	bytDay;
	byte	bytHour;
	byte	bytMin;
	byte	bytSec;
	byte	bytIsInOut;
	DWORD	dwMeterTotle;
	
	tag4247()
	{
		memset(this,0,sizeof(*this) );
	}
}; 

STRUCT_1  tag4274 // ����վ����
{
	WORD	usLineNo;
	WORD	usStaNo;
	WORD	usYear;
	byte	bytMon;
	byte	bytDay;
	byte	bytHour;
	byte	bytMin;
	byte	bytSec;
	byte	bytIsInOut;
	DWORD	dwMeterTotle;
	
	tag4274()
	{
		memset(this,0,sizeof(*this) );
	}
}; 

STRUCT_1  tag4248 // ��������
{
	WORD	usLineNo;
	WORD	usStaNo;
	byte	bytYear;
	byte	bytMon;
	byte	bytDay;
	byte	bytHour;
	byte	bytMin;
	byte	bytSec;
	ushort  usFup;
	ushort  usFdw;
	ushort  usBup;
	ushort  usBdw;
	
	tag4248()
	{
		memset(this,0,sizeof(*this) );
	}
}; 

STRUCT_1 tag8ff1
{
	BYTE	m_bytTranType;
	BYTE	m_bytDataType;
	char	m_szContent[200];
	
	tag8ff1()
	{
		memset( this, 0, sizeof(*this));
		m_bytTranType = 0x8f, m_bytDataType = 0xf1;
	}
};

STRUCT_1 tag4254
{
	BYTE	m_bytAlertHi;
	BYTE	m_bytAlertLo;
	BYTE	m_bytMonCentCount;
	tagQianGps	m_objQianGps;
	ushort  m_usLineId;  //	��ǰ��·��.
	// A7 A6 A5 A4 A3 A2 A1 A0
	//A1A0:01:���� 10 ���� 00 ��ȷ��, A3: ��û�п��������ϱ�
	BYTE    m_bytBusSta; //��������
	//��������(1):
	//0x01: ����  0x02: �س� 0x03: ���� 0x04 ���� 0x05: �޳�  0x06: ���� 0x07: �ſ� 0x08 ����
	BYTE    m_bytReserve;   //˾�����
	ulong   m_dwMeter;    // �����
	BYTE    m_bytCheCi;
	//���µ��ֶη������
	byte    m_bytSatInUse;//��ʹ�õ�������
	byte    m_bytHDOP;
	ulong   m_ulDriverId; //˾�����
	
	tag4254()
	{
		memset( this, 0, sizeof(*this) );
	}
};

STRUCT_1  tag4310
{
	char  strVersion[6];
	
	tag4310(){memset(this,0,sizeof(*this));}
}; 

STRUCT_1  tag4312
{
	char  strVersion[6];
	DWORD dwPackCnt;
	byte  usWinsize;
	char  m_md5[32];
	
	tag4312(){memset(this,0,sizeof(*this));}
}; 

STRUCT_1  tag4313
{
	byte  bNeedAns;
	DWORD ulPackIndex;
	WORD  usPackSize;//�������һ���������İ���С����1K.
	byte  aryLineDat[1024];
	
	tag4313(){memset(this,0,sizeof(*this));}
}; 

STRUCT_1  tag4350
{
	char  strVersion[6];
	byte  bSucc;
	
	tag4350(){memset(this,0,sizeof(*this));}
}; 

STRUCT_1  tag4352
{
	char  strVersion[6];
	byte  ucFlag;
	
	tag4352(){memset(this,0,sizeof(*this));}
}; 

STRUCT_1  tag4353
{
	char  strVersion[6];
	DWORD ulWinIndex;
	DWORD ulWinStatus;
	
	tag4353(){memset(this,0,sizeof(*this));}
	};

/// } ҵ��㴦��

#pragma pack()


#pragma pack(4)

struct tagMonStatus
{
	double	m_dLstLon; // �ϴμ��ʱ�ľ�γ��
	double	m_dLstLat;

	double	m_dLstSendDis; // �ϴη��ͼ������֮���ۻ���ʻ�ľ��� (��)
	
	DWORD	m_dwLstSendTime; // �ϴη��ͼ�����ݵ�ʱ�� (����)

	DWORD	m_dwChkInterv; // ����� (����), ��ض�ʱ����ʱ�䶨ʱ��������ж������,ʹ��Ĭ��ֵ,����,ʹ�ø�ֵ

	DWORD	m_dwMonTime; // ʣ����ʱ�� (����, 0xffffffff��ʾ���ü��)
	DWORD	m_dwLstChkTime; // �ϴν����жϵ�ʱ�� (����)

	DWORD	m_dwSpace; // �����ؼ�� (��)
	DWORD	m_dwMonInterv; // ��ʱ��ؼ�� (����)

	DWORD	m_dwGpsCount; // ʣ������ (0xffffffff,��ʾ��������)
	char	m_szMonHandTel[15]; // �������Ҫ�������ֻ��� (�߶���ͨ��ʱ��Ч) (��ʱ����)
	bool	m_bInterv; // �Ƿ�ʱ
	bool	m_bSpace; // �Ƿ񶨾�
	bool	m_bAlertAuto; // �Ƿ���Ϊ����

	tagMonStatus()
	{
		Init();
	}

	void Init()
	{
		memset(this, 0, sizeof(*this));
		memset( m_szMonHandTel, 0x20, sizeof(m_szMonHandTel) );
		m_dwChkInterv = m_dwMonInterv = 0xffffffff; // ��ȫ�����
		m_dwMonTime = 0;
		m_dwGpsCount = 0;
	}
};

// ��Э���еĸ�ʽ��ͬ
struct tagBlackQueryPeriod
{
	BYTE	m_bytFromYear;
	BYTE	m_bytFromMon;
	BYTE	m_bytFromDay;
	BYTE	m_bytFromHour;
	BYTE	m_bytFromMin;
	BYTE	m_bytToYear;
	BYTE	m_bytToMon;
	BYTE	m_bytToDay;
	BYTE	m_bytToHour;
	BYTE	m_bytToMin;
};

struct tagBlackQueryArea
{
	long	m_lLonMin;
	long	m_lLatMin;
	long	m_lLonMax;
	long	m_lLatMax;
};

struct tagBlackQuerySta
{
	tagBlackQueryArea m_aryArea[ 20 ];
	tagBlackQueryPeriod	m_aryPeriod[ 20 ];

	DWORD	m_dwLstQueryTime; // �ϴν��ղ�ѯ֡��ʱ�� (����)

	BYTE	m_bytPeriodCount; // �ѱ����ʱ�����������
	BYTE	m_bytAreaCount; // �ѱ����������������

	char	m_szCentGroup[10]; // ���ķ����
	BYTE	m_bytUploadFrameTotal; // ����ϴ�֡��
	BYTE	m_bytUploadFrameCt; // �ϴ�֡������

	BYTE	m_bytReqFrameTotal; // ����֡����
	BYTE	m_bytReqFrameCount; // ��ǰ���յ�������֡����
	char	m_aryReqFrameSymb[ 40 ]; // ���յ���֡�Ƿ��ظ��ı�־ (Ԫ�ظ���=����2������Ԫ������)

	tagBlackQuerySta() { memset(this, 0, sizeof(*this)); }
};

struct tagBlackSndSta
{
	DWORD	m_dwGpsTotal; // ��ϻ�����ݵ�����
	DWORD	m_dwLstFileDoneCt; // �ϴη���ʱ����ĺ�ϻ���ļ�,�Ѿ���ɵĺ�ϻ�ӵ���� (���ļ��ڵ�һ�����������ĵ㿪ʼ����)
	WORD	m_wFileTotal; // ��ϻ���ļ�����
	WORD	m_wDoneFileCount; // �Ѵ���ĺ�ϻ���ļ��ļ���
	char	m_szBlackFile[60][20]; // ��ϻ���ļ���,��NULL��β
	BYTE	m_bytSndFrameCount; // ���Ϸ��ĺ�ϻ��֡����

	tagBlackSndSta() { memset( this, 0, sizeof(*this) ); }
};

struct tagBlackSaveGps
{
	// ���ں�ʱ�̾��Ǳ���ʱ��
	long		m_lLon;
	long		m_lLat;
	BYTE		m_bytHour; // *** { ��������,ע�����,Ŀ������ "������ʱ"���һ��DWORD�����бȽ�***
	BYTE		m_bytDay;
	BYTE		m_bytMon;
	BYTE		m_bytYear; // 2λ���ֱ�ʾ��ֵ *** } ��������,ע�����,Ŀ������ "������ʱ"���һ��DWORD���Է���Ƚ�***
	BYTE		m_bytSec;
	BYTE		m_bytMin;
	BYTE		m_bytSpeed; // ����/Сʱ
	BYTE		m_bytMix;
#if 2 == USE_PROTOCOL
	BYTE		m_bytIOSta; // ��ʱ��ʹ��
	BYTE		m_bytHeavyIO; // ��ʱ��ʹ��
#endif

	tagBlackSaveGps() { memset( this, 0, sizeof(*this) ); }
};

// struct tagPosCond
// {
// 	double	m_dLstLon;
// 	double	m_dLstLat;
// 	double	m_dLstSendDis; // ��
// 
// 	float	m_fSpeed; // ��/��, 0��ʾ��Ч
// 
// 	DWORD	m_dwLstSendTime; // �ϴη���ʱ��
// 
// 	long	m_aryArea[16][4]; // ÿ����Χ������: ���¾��ȡ�����γ�ȡ����Ͼ��ȡ�����γ�� (����10��-6�η���)
// 	DWORD	m_aryAreaContCt[16]; // ���������ļ��� (Ԫ�ظ���������һ��)
// 	BYTE	m_aryAreaProCode[16]; // ����Χ�ı�� (Ԫ�ظ���������һ��)
// 	BYTE	m_bytAreaCount;
// 
// 	BYTE	m_bytGpsType;
// 	WORD	m_wSpaceInterv; // ����,0��ʾ��Ч
// 	WORD	m_wTimeInterv; // ����,0��ʾ��Ч
// 
// 	BYTE	m_aryTime[10][3]; // ÿ��ʱ��������: ���,ʱ,��
// 	BYTE	m_bytTimeCount;
// 
// 	BYTE	m_aryStatus[6];
// 	// �̶�Ϊ6��, BYTE	m_bytStaCount; 
// 
// 	BYTE	m_bytLstTimeHour; // �ϴη���ʱ��������ʱ��,��ʼ��Ϊ��Чֵ����0xff,
// 	BYTE	m_bytLstTimeMin;
// 
// 	BYTE	m_bytCondStatus; // 0	1	I	T	V	A	S	0
// 						//	�ɣ��ر�/�������������0-�رգ�1-����
// 						//	�ԣ��ر�/����ʱ��������0-�رգ�1-����
// 						//	�֣��ر�/�����ٶ�������0-�رգ�1-����
// 						//	�����ر�/������Χ������0-�رգ�1-����
// 						//	�ӣ��ر�/����״̬������0-�رգ�1-����
// 
// 	char	m_szSendToHandtel[15];
// 
// 	tagPosCond() { Init(); }
// 	void Init()
// 	{
// 		memset(this, 0, sizeof(*this));
// 		m_bytLstTimeHour = m_bytLstTimeMin = 0xff;
// 		m_wTimeInterv = 0, m_wSpaceInterv = 0, m_bytCondStatus = 0x40;
// 	}
// };

// struct tagIPCfg
// {
// 	unsigned long m_ulTcpIP; // (inet_addr֮���,��ͬ)
// 	unsigned long m_ulVUdpIP;
// 	unsigned long m_ulBUdpIP;
// 	unsigned short m_usTcpPort; // (htons֮��ģ���ͬ) 
// 	unsigned short m_usVUdpPort;
// 	unsigned short m_usBUdpPort;
// 	char		m_szApn[20];
// 	BYTE		m_bytValid; // 0,��ʾ��Ч; ��0,��ʾ��Ч
// 
// 	tagIPCfg() { memset(this, 0, sizeof(*this)); }
// };

// struct tagGPSUpPar // �������ļ���
// {
// 	WORD		m_wMonSpace; // ��ض������ ����λ��10�ף�
// 	WORD		m_wMonTime; // ���ʱ�� �����ӣ���0xffff��ʾ���ü��
// 	WORD		m_wMonInterval; // ��ؼ�� ���룩������Ϊ1
// 	BYTE		m_bytStaType; // ע��: ����Э�����״̬����
// 	BYTE		m_bytMonType; // Э���ж���ļ������
// };

struct tagPhotoInfo
{
	DWORD		m_dwPhotoDate; // ��Ƭ���ڣ������0����ʾ��Ƭδ֪ (�����պ��ϴ�ʱ,���Կ����ҵ�����Ƭ����Ŀ¼)
	WORD		m_wPhotoIndex; // ��Ƭ����,��1��ʼ���
	WORD		m_wPhotoType;
	BYTE		m_bytSizeType; // �ֱ��� (��Э�鶨��Ĳ�ͬ)
	BYTE		m_bytQualityType; // �������� (��Э�鶨����ͬ)
	BYTE		m_bytBright; // ����
	BYTE		m_bytContrast; // �Աȶ�
	BYTE		m_bytHue; // ɫ��
	BYTE		m_bytBlueSaturation; // ������
	BYTE		m_bytRedSaturation; // �챥��
	BYTE		m_bytChannel;		// ����ͨ�� (add by dkm)
	BYTE		m_bytInterval; // �´����ռ��,s
	BYTE		m_bytDoorPar; // �����ڹس�������,����λ��ʾ�ٶ����ƣ�0-15������/Сʱ��,����λ��ʾ���ٶȳ���ʱ�䣺0-15�����ӣ�
	char		m_szDateTime[6];
	bool		m_bUploaded; // �Ƿ��Ѿ��ϴ����������ϴ�������

	tagPhotoInfo() { memset(this, 0, sizeof(*this)); }
};

struct tagPhotoSta // ����״̬��Ϣ
{
	tagPhotoInfo m_aryPhotoInfo[ PHOTOQUEE_MAXCOUNT ];
	
	BYTE	m_bytPhotoTotal; // ��ǰ���յ���Ҫ��Ƭ����
	BYTE	m_bytPhotoCt; // ��ǰ���յ�������Ƭ����
	
	tagPhotoSta() { memset(this, 0, sizeof(*this)); }
};

struct tagUploadPhotoSta
{
	tagPhotoInfo m_aryUpPhotoInfo[ UPLOADPHOTO_MAXCOUNT ]; // ����Ҫ�ϴ���Ƭ����Ϣ���� (ֻʹ����Ƭ����)
	BYTE	m_bytAllPhotoTotal; // ����Ҫ�ϴ���Ƭ������
	BYTE	m_bytAllPhotoCt; // �������ϴ���Ƭ�ļ���

	DWORD	m_dwPhotoSize; // ��ǰ���ϴ���Ƭ���ļ���С
	WORD	m_wPhotoFrameTotal; // ��ǰ���ϴ���Ƭ���ܰ���
	//WORD	m_wPhotoIndex; // ��ǰ���ϴ���Ƭ����Ƭ����,��1��ʼ���
	WORD	m_wPhotoWinCt; // ��ǰ���ϴ���Ƭ������ɵĴ��ڼ��� (���������ط���)
	char    m_szPhotoPath[ MAX_PATH ]; // ��ǰ���ϴ���Ƭ��ȫ·����,��NULL��β
	char	m_szDateTime[6]; // ��ǰ���ϴ���Ƭ������ʱ��
	char	m_szLonLat[8]; //��ǰ���ϴ���Ƭ�ľ�γ��
	WORD	m_wPhotoType; // ��ǰ���ϴ���Ƭ���¼�����
	WORD	m_wLstSuccUpPhotoIndex; // ���һ���ϴ��ɹ�����Ƭ����
	// ��ǰ�ϴ���Ƭ��֡���ݻ��� (����δȷ�ϴ����ڰ�����,����)
	WORD	m_aryPhotoFrameNo[ UPLOADPHOTO_WINFRAMETOTAL ]; // �������,��0��ʼ���,0��ת��, ��=0xffff,˵����λ�û���������Ч
	WORD	m_aryPhotoFrameLen[ UPLOADPHOTO_WINFRAMETOTAL ]; // �����������,һ��<=UDPDATA_SIZE
	char	m_aryPhotoFrameBuf[ UPLOADPHOTO_WINFRAMETOTAL ][ UDPDATA_SIZE ]; // ��������

	BYTE	m_bytPhotoSizeType; // ��ǰ���ϴ���Ƭ�ķֱ������� (��Э�鶨����ͬ)
	BYTE	m_bytPhotoQualityType; // ��ǰ���ϴ���Ƭ���������� (��Э�鶨����ͬ)
	BYTE	m_bytPhotoChannel; // ��ǰ���ϴ���Ƭ������ͷͨ���� (��Э�鶨����ͬ)
	bool	m_bPhotoBeginUpload; // ��ǰ���ϴ���Ƭ�Ƿ�����������ϴ�

	bool	m_bUploadBreak; // �����Ƿ��ж��� (��ָ�ϴ���ʱ)

	tag4152 m_obj4152;
	BYTE	m_byt4152Snd; // 0,��ʾ������δ������4152; ��0,��ʾ�������ѷ���4152
	
	tagUploadPhotoSta() { Init(); }
	void Init()
	{
		memset( this, 0, sizeof(*this) );
		memset( m_aryPhotoFrameNo, 0xff, sizeof(m_aryPhotoFrameNo) );
	}
};

struct tagLNUploadPhotoSta
{
	WORD   m_wPhotoIndex;
	WORD   m_CurPackNo;
	DWORD   m_dwPhotoSize; // ��ǰ���ϴ���Ƭ���ļ���С
	WORD    m_wPhotoFrameTotal;	// ��ǰ���ϴ���Ƭ���ܰ���
	WORD    m_wPhotoWinCt; // ��ǰ���ϴ���Ƭ������ɵĴ��ڼ��� (���������ط���)
	char    m_szPhotoPath[ MAX_PATH ]; // ��ǰ���ϴ���Ƭ��ȫ·����,��NULL��β
	char    m_szDateTime[6]; // ��ǰ���ϴ���Ƭ������ʱ��
	char    m_bytMd5[16];
	byte    m_bytPhotoChannel;
	WORD    m_wPhotoType; // ��ǰ���ϴ���Ƭ���¼�����
	// ��ǰ�ϴ���Ƭ��֡���ݻ��� (����δȷ�ϴ����ڰ�����,����)
	WORD    m_aryPhotoFrameNo[ UPLOADPHOTO_MAXCOUNT ];	// �������,��0��ʼ���,0��ת��, ��=0xffff,˵����λ�û���������Ч
	WORD    m_aryPhotoFrameLen[ UPLOADPHOTO_MAXCOUNT ]; // �����������,һ��<=UDPDATA_SIZE
	char    m_aryPhotoFrameBuf[ UPLOADPHOTO_MAXCOUNT ][ UDPDATA_SIZE ]; // ��������

	bool    m_bPhotoBeginUpload; // ��ǰ���ϴ���Ƭ�Ƿ�����������ϴ�

	bool    m_bUploadBreak;	// �����Ƿ��ж��� (��ָ�ϴ���ʱ)

	tagLNUploadPhotoSta() { Init();}
	void Init()
	{
		memset( this, 0, sizeof(*this) );
		memset( m_aryPhotoFrameNo, 0xff, sizeof(m_aryPhotoFrameNo) );
	}
};

// ÿ���¼���¹��ɵ�����
struct tagAcdentSec
{
	char	m_szLon[4]; // ��Э�龭γ�ȶ���,����Lon��LatҪ����һ��,���ڷ���ʱ��������֡
	char	m_szLat[4];
	BYTE	m_arySpeed[5]; // ����/Сʱ
	BYTE	m_aryIOSta[5]; // IO״̬

	tagAcdentSec()
	{
		memset( this, 0, sizeof(*this) );
	}
};

// �¹��ɵ��¼ (�ļ���Ҳʹ��)
struct tagAcdentRec
{
	long			m_lStopLon;
	long			m_lStopLat;
	BYTE			m_bytStopHour; // { ������˳��,����ʹ��������ʱΪһ��4�ֽ�����,���ڱȽ�,���ҿ������ڴ����
	BYTE			m_bytStopDay;
	BYTE			m_bytStopMon;
	BYTE			m_bytStopYear; // }
	WORD			m_wDriverNo; // ˾�����,0~ffff,0��ʾ�հ��û�,ffff��ʾ�쳣�û�
	BYTE			m_bytStopMin;
	BYTE			m_bytStopSec;
	tagAcdentSec	m_aryAcdentSec[20]; // ͣ��ǰ20s������

	tagAcdentRec()
	{
		memset( this, 0, sizeof(*this) );
	}
};

/// �¹��ɵ��ѯʱ������� (�ڴ���ʹ��)
struct tagAcdentQueryPeriod
{
	BYTE			m_bytBeginHour; // { ������˳��,����ʹ��������ʱΪһ��4�ֽ�����,���ڱȽ�,���ҿ������ڴ����
	BYTE			m_bytBeginDay;
	BYTE			m_bytBeginMon;
	BYTE			m_bytBeginYear; // }
	BYTE			m_bytEndHour; // { ������˳��,����ʹ��������ʱΪһ��4�ֽ�����,���ڱȽ�,���ҿ������ڴ����
	BYTE			m_bytEndDay;
	BYTE			m_bytEndMon;
	BYTE			m_bytEndYear; // }
	WORD			m_wBeginMin;
	WORD			m_wEndMin;

	tagAcdentQueryPeriod() { memset(this, 0, sizeof(*this)); }
};

/// �¹��ɵ��ѯ��Χ���� (�ڴ���ʹ��)
struct tagAcdentQueryArea
{
	long	m_lLonMin;
	long	m_lLatMin;
	long	m_lLonMax;
	long	m_lLatMax;
};

struct tagAcdentQuerySta
{
	DWORD	m_dwLstFileDoneCt; // �ϴη���ʱ������ļ�,�Ѿ���ɵļ�¼���� (���ļ�����ʼ��¼��ʼ����)
	WORD	m_wFileTotal; // �ļ�����
	WORD	m_wDoneFileCount; // �Ѵ�����ļ��ļ���
	char	m_aryAcdentFile[60][20]; // �¹��ɵ��ļ��б�,����NULL��β
	BYTE	m_bytFindAcdCt; // �ҵ��ķ����������¹��ɵ��¼�ļ���
	bool	m_bFindAcd; // �Ƿ������ҵ�����������������
};

struct tagDrvSampleDatas
{
	double	m_dDrvDis; // ���ݲ�ͬ��Э��: (1)���ϴη��ͼ�¼ʱ��ʻ��·��; (2)�ۻ���ʻ�������.  ��λ(��)
	union
	{
 		tagSampleData_3745_KJ2 m_arySampleData_3745_KJ2[100]; // �������ʻ��¼��
		tagSampleData_3746 m_arySampleData_3746[100]; // �������ʻ��¼��
		tagSampleData_3747 m_arySampleData_3747[100];
	} m_uniSampleDatas;
 	BYTE	m_bytSampleDataCount; // �������ʻ��¼����
	BYTE	m_bytAvgSpeedCount; // �����ƽ���ٶȸ���
	BYTE	m_aryAvgSpeed[300]; // �����ƽ���ٶȼ�,�������ܳ��������Ա���ܱ��ķ�Χ,����/Сʱ
	BYTE	m_bytOilAD;
	BYTE	m_bytPowAD;
	BYTE	m_bytOilSta; // D7��D0����ʾ�ͺĲ���ʱ��״̬
				// ǧ����Э��: �ͺ�ֵ��״̬ 1 D7��D0����ʾ�ͺĲ���ʱ��״̬ (���¸���״ֻ̬��һλ��Ч)
				// D0��0������ʻ��1�������
				// D1��0��ʾ������1 GPS��λ���ٶȴ���2����/Сʱ
				// D2��0��ʾ��λ��1����λ
				// D3��0��ʾ������1 GPS��λ���ٶ�С��2����/Сʱ
				// D4��0��ʾ������1ͣ����ACC��Ч����ACC��Ч�Ǳ���ȴû���ϵ�
				// D5��0��ʾ������1�ͺĺ�û��
				// D6��0��ʾ������1�ͺ�ֵ�������÷�Χ�ڣ���ֵ��Ч��

				// 2��Э��
				// 0x01: ��� (�¼�����)
				// 0x02: ��ͨ�磬�Ѷ�λ���ٶ�>15
				// 0x04: ��ͨ�磬δ��λ
				// 0x08: ��ͨ�磬�Ѷ�λ���ٶ�<15
				// 0x10: δͨ��

	tagDrvSampleDatas() { memset( this, 0, sizeof(this) ); }
};

struct tagDrvRecSampleCfg
{
	WORD	m_wSampleCycle; // ��������,s
	BYTE	m_bytSndCycle; // ��������,��
	BYTE	m_bytSndWinSize; // ���ʹ��ڴ�С

	tagDrvRecSampleCfg() { memset( this, 0, sizeof(this) ); }
};

struct tagDrvRecSta
{
	double	m_dTotalLichen; // �����,��
	long	m_lLon;	// ���1����Ч�ľ�γ��,�������ͳ��
	long	m_lLat;
	WORD	m_wCurDriverNo; // ����˾�����
	bool	m_bBeforeReset; // �Ƿ�λǰ�����
	BYTE	m_bytChkSum; // У��� (�ۼӺ�ȡ��) (Ҫ�������)
	BYTE	m_bytOilAD; // �����ͺ�AD
	BYTE	m_bytOilSta; // �����ͺ�״̬
	BYTE	m_bytPowAD; // ���ĵ�ԴAD

	tagDrvRecSta() { memset( this, 0, sizeof(*this) ); }
};

#pragma pack()

typedef union {
	ushort     word;
	struct {
		BYTE  low;
		BYTE  high;
	} bytes;
}WORD_UNION;

typedef union{
	DWORD dword;
	struct  
	{
		BYTE b0;
		BYTE b1;
		BYTE b2;
		BYTE b3;
	}bytes;
}DWORD_UNION;

#pragma pack(push,1)
struct tagCurTime 
{
	BYTE		nYear;				// 4�����ֱ�ʾ��ֵ,��2007,������7;
	BYTE		nMonth;
	BYTE		nDay;
	BYTE        nWeekDay;			//����
	BYTE		nHour;
	BYTE		nMinute;
	BYTE		nSecond;
};

//QLED���ݶ�
struct tagQLEDStat
{
	BYTE		symb;				//���ر�־ 0δ����״̬ 1����״̬
	BYTE		lighten;			//������ 0����״̬��1����״̬
	BYTE		char_Excp;			//�ֿ��쳣0��������1���쳣�������������ֿ�
	BYTE		set_Excp;			//�����쳣0��������1���쳣������������
	ushort		badPt_Ct;			//������ ˫�ֽ�
	tagCurTime	curLedTime;			//LEDʱ��
	BYTE		mail_Ct;			//LED��Ϣ����
	BYTE		mode;				//0 ת����Ϣ��1�������ţ�2�㲥���ţ�3���ż�ʱ��Ϣ��4����
	ushort		curID;				//���ڲ��ŵ���ϢID ��Ϊ0��˵����Ļ��û����Ϣ������Ĭ�ϱ���
	BYTE		curIerm;			//���ڲ�����Ϣ����ʾ��ID
	
	//���������������ϢID����(1)����ϢID(2)*n
	BYTE		wait_dwnID;			//�ȴ����ص���Ϣ����
	ushort		ary_dwnID[MAILCNT_BOHAI];
	
	//LEDʱ���Ƿ�ΪGPSʱ��
	BYTE		gpsTime;			//1:�� 0:��
	bool		comuErr;			//��LED��ͨ�ų����־λ 1������ 0������
	//���ڲ��ŵļ�ʱ��Ϣ��
	tagQLEDStat(){Init();}
	void Init()
	{
		memset(this,0,sizeof(*this));
	}
};

struct tagLedUpdtStat 
{
	BYTE		ver_Cnt;			//�汾����
	char		ver[16];			//�汾
	BYTE		pkt_Cnt;			//�ܰ���
	bool		updating;			//��������
	BYTE		packet;				//��������İ�
};


//LED����
struct tagLedParam 
{
	ushort		m_usParamID;		//����ID��0--65535
	BYTE		m_bytLightenSt;		//���������ƣ�1������ 2������
	BYTE		m_bytBrigType;		//���ȵ��ڷ�ʽ : 0�̶����ȵ��ڡ�1��ʱ����ں� 2����������
	BYTE		m_bytStaticVl;		//�̶�����ֵ: 1~10����
	WORD		m_aryTimeSeg[4];	//��ʱ�����: ������4��ʱ��� HH:MM ��ȷ���֣� HH * 60 + MM �� 
	BYTE		m_aryTimeSegVl[4];	//��ʱ����� ��Ӧֵ
	BYTE		m_aryEnvVl[8];		//���������� 8��
	
	tagLedParam()
	{
		Init();
	}

	void Init()
	{
		memset(this,0,sizeof(*this));
	}

	void Clear()
	{
		memset(this+sizeof(unsigned short),0,sizeof(*this)-sizeof(unsigned short));//����ID����ʼ��
	}
};

//�㲥��Ϣ
struct tagLedDISPMsg 
{
	BYTE		m_bytLvl;			//���ż��𣺵��ֽڣ�0һ�㣬1����
	BYTE		m_bytExitWay;		//�˳���ʽ��0ֻͨ���˳�ָ���˳���1�㲥����ʱ�䣬2�㲥����ʱ�䣬3�㲥ѭ������
	DWORD		m_wEndTm;			//����ʱ�䣺HH:MM ��ȷ���֣� HH * 60 + MM ��//���ֽڣ���λ�롣����Ϊ��λ���� 08:30:05(hh:mm:ss) Ӧ�����:(8*60+30)*60+5=30605(0x0000778D)
	ushort		m_usPlayTm;			//����ʱ��  ��λ��
	BYTE		m_bytCircleCnt;		//ѭ������
	BYTE		m_bytDISPIDCt;      //�㲥��Ϣ����
	ushort		m_aryDISPID[MAILCNT_BOHAI];  //�㲥��ϢID��

	tagLedDISPMsg()
	{
		Init();
	}

	void Init()
	{
		memset(this,0,sizeof(*this));
	}
};

struct tagLedShowItemPar				//��ʾ������ṹ  
{
	BYTE		m_bytType;				//���ͣ�0���֣�3ΪͼƬ����������
	BYTE		m_bytColor;				//��ɫ��0��ɫ��1��ɫ��2��ɫ��3���ƣ�4ѭ����5�����6���죬7���̣�8����
	BYTE		m_bytFont;				//���壺���뷽ʽ��0-���壬16��1-���壬16����2-����_GB2312,16��3-��Բ��16
	BYTE		m_bytWinX1;				//����X1�����Ͻ�x���꣬1��λ = 8���ص�
	BYTE		m_bytWinY1;				//����Y1�����Ͻ�y���꣬1��λ = 8���ص�
	BYTE		m_bytWinX2;				//����X2�����½�x���꣬1��λ = 8���ص�
	BYTE		m_bytWinY2;				//����Y2�����½�y���꣬1��λ = 8���ص�
	ushort		m_usStayTm;				//˫�ֽڣ�ÿ��ʾ����LED���ϵ�ͣ��ʱ��
	BYTE		m_bytShowWay;			//��ʾ��ʽ��0-����      1-�����    2-�Ϲ���    3-���ƽ�    4-���ƽ� 
	//			5-���ƽ�    6-���ƽ�    7-���ҶԽ�  8-���¶Խ�  9-�����    
	//			10-�ҷ���   11-�����   12-�ҹ���   13-���Ϲ��� 14-���Ϲ��� 
	//			15-���¹��� 16-���¹��� 17-������˸ 18-������˸ 19-�������� 
	//			20-�������� 21-������   22-������   23-���Ҷ��� 24-������   
	//			25-���Ҷ   26-�Ұ�Ҷ   27-����β   28-����β   29-����β   
	//			30-����β   31-���ٲ�   32-���ٲ�   33-����ѩ   34-����ѩ   
	//			35-��˸���� 36-���״��� 37-��չ���� 38-����     39-����     
	//			40-�������� 41-�޺�����
	BYTE		m_bytShowType;			//��ʾ��ʽ��δ���ţ���Ϊ0
	BYTE		m_bytRunSpeed;			//�����ٶȣ�����ʾ�����ʾ��ʽΪ1�����������2���Ϲ�����ʱ����ʾ��Ĺ����ٶ�
	BYTE		m_bytUpInter;			//�Ϲ��м�ࣺ����ʾ�����ʾ��ʽΪ2���Ϲ��������Ϲ��м��
	//		BYTE		m_bytLeftInter;			//������������ʾ�����ʾ��ʽΪ1�����������ÿ�����ַ��������Ļ��ÿ8��������Ϊ1
	BYTE		m_bytLeftStayWay;		//���ͣ����ʽ�����ֽڡ�0ÿ��ͣ����1���һ��ͣ��
	BYTE		m_bytCircleCnt;			//ѭ������
	BYTE		m_bytReservered[2];		//���������ֽ�
	ushort		m_usShowInfoCt;			//��ʾ�������ֽ�����˫�ֽ�
	BYTE		m_bytShowInfo[20];		//��ʾ������

	tagLedShowItemPar()
	{
		memset(this, 0, sizeof(*this));	
	}
};

struct tagLedShowMsg				//��ʱ��Ϣ�ṹ 
{
	BYTE		m_bytLvl;				//���ż���
	BYTE		m_bytExitWay;			//�˳���ʽ��0ֻͨ���˳�ָ���˳���1�㲥����ʱ�䣬2�㲥����ʱ�䣬3�㲥ѭ������
	DWORD		m_wEndTm;				//����ʱ�䣺���ֽڣ���λ�롣����Ϊ��λ���� 08:30:05(hh:mm:ss) Ӧ�����:(8*60+30)*60+5=30605(0x0000778D)
	ushort		m_usPlayTm;				//����ʱ�䣺˫�ֽڣ���λ��
	BYTE		m_bytCircleCnt;			//ѭ������
	BYTE		m_bytReservered[4];		//�����ֽ�
	BYTE		m_bytShowCt;			//��ʾ������
	tagLedShowItemPar	m_tagShowPar[2];//��ʾ��ṹ����
	
	tagLedShowMsg()	
	{
		memset(this, 0, sizeof(*this));
	}
};

//LED��Ŀ��
struct tagLedMenu 
{
	ushort		m_usMenuID;			//��Ŀ��ID 0--65535
	tagLedParam m_LedParam;        //LED��������
	BYTE		m_bytConfig;       /*
									�����֣�1��
									������0ABCDEFG��
									G��0������ԭ�Ƚ�Ŀ��
									1�����ԭ�Ƚ�Ŀ��
									F��0���޵㲥��ȡ���㲥
									1�����㲥��Ϣ
									E��0���޼�ʱ������Ϣ����ȡ����ʱ����
									1������ʱ������Ϣ
								   */
	BYTE		m_bytMailIDCnt;      //��Ŀ������ϢID��������LED������ID�Աȼ��ɻ���������ص���ϢID������������
	ushort		m_aryMailID[MAILCNT_BOHAI];  //����ID���� ����125 
	tagLedDISPMsg m_LedDispMsg;     //�㲥��Ϣ	
	tagLedMenu(){Init();}
								   
	void Init()
	{
		memset(this,0,sizeof(*this));
	}
								   
	//���������ֽ���
	void hton()
	{
		for (int i=0;i<m_bytMailIDCnt;i++)
		{
			m_aryMailID[i] = htons(m_aryMailID[i]);
		}
	}
};

struct tagMail 
{
	BYTE		m_bytMailIDCnt;			//�������ø�������
	ushort		m_aryMailID[MAILCNT_BOHAI];  //����ID���� ����125
	
	tagMail()
	{
		Init();
	}

	void Init()
	{
		memset(this,0,sizeof(*this));
	}

	//���������ֽ���
	void hton()
	{
		for (int i=0;i<m_bytMailIDCnt;i++)
		{
			m_aryMailID[i] = htons(m_aryMailID[i]);
		}
	}
};

//���ؽ�Ŀ��Ӧ���ʽ
struct tagLedMenuAns 
{
	ushort		m_usMenuID;			//��Ŀ��ID 0--65535
	ushort		m_usParamID;		//����ID��0--65535
	ushort      m_usTURNID;			//ת����ϢID
	ushort      m_usNTICID;			//֪ͨ��ϢID
	BYTE		m_bytConfig;       /*
								   �����֣�1��
								   ������0ABCDEFG��
								   G��0������ԭ�Ƚ�Ŀ��
								   1�����ԭ�Ƚ�Ŀ��
								   F��0���޵㲥��ȡ���㲥
								   1�����㲥��Ϣ
								   E��0���޼�ʱ������Ϣ����ȡ����ʱ����
								   1������ʱ������Ϣ
									*/
	BYTE		m_bytDownIdCnt;		//�·�ID����
									
	tagLedMenuAns()
	{
		Init();
	}
	
	void Init()
	{
		memset(this,0,sizeof(*this));
	}	
};

struct Frm47 
{
	byte	protype;	//Э������
	char	chktype;	//�������
	char	restype;	//Ӧ������
	Frm47()		{ protype = 0x47; }
};

#pragma pack(pop)

#if SAVE_LEDDATA == 1
//�洢��־�Ľṹ
struct LogHead {
	int cnt;		//��־����
	int write;		//д��־ָ��
};
#endif

enum DOWNSTA{
	Idlesse = 1,
	DispTask,
	ReqAttr,
	WaitData,	
	RcvData,
	ChkBuf,	
	ChkMd5,	
	TranResult,	
	SoftUpdate,
	SendData,	
	ReportChk,
	UpdataRes,	
	CutComu,
	EndRcv
};

enum FILE_TYPE
{
	SOFT,
	ROOT,
	KERN,
	BOOT,
	LINE
};
#define FILE_TYPE_COUNT 5
//-----------------------------------------------------------------------
// �����ļ������б�
struct __attribute__(packed,aligned (1)) FileAttr {
	bool	m_bExist;		// �ļ��Ƿ����
	ulong	m_ulID;			// �ļ�ID
	ulong	m_ulFileSize;	// �ļ���С
	byte	m_bytMd5[32];	// MD5ֵ	
	ulong	m_ulDownSize;	// ������
	ushort	m_ulPackLen;	// ʹ�õ����ݰ���С
};

struct __attribute__(packed,aligned (1)) FileList {
	FileAttr	m_objFile[FILE_TYPE_COUNT];		//�ļ�����
	ulong		m_ulVer;						//�ļ��б�汾

	FileList() { memset(this, 0, sizeof(*this));}
};
#endif

