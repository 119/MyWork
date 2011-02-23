#ifndef COMUSERVEXPORT_H_FGJIYE382ASD
#define COMUSERVEXPORT_H_FGJIYE382ASD

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stddef.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "typedef.h"

extern int NETWORK_TYPE;
extern int PHONE_MODTYPE;
extern char USB_DEV_INSMOD[128];
extern char USB_DEV_CHECKPATH_AT[128];
extern char USB_DEV_CHECKPATH_PPP[128];
extern char PPP_DIAL_NUM[128];
extern char PPP_USER_NAME[128];
extern char PPP_PASSWORD[128];
extern DWORD COMMUX_DEVPATH_AT;
extern DWORD COMMUX_DEVPATH_PPP;

#define VEHICLE_M		666
#define VEHICLE_M2		777
#define VEHICLE_V8		888
#define VEHICLE_TYPE VEHICLE_V8

// ���汾��,�ڹ鵵���������ͻ�ʹ�õ������Ҫע��汾�ŵ�����
#define CHK_VER			0	// 0,�����汾; 1,�������汾
#define USE_PROTOCOL	0	// 0,ǧ����ͨ�ð�Э��; 1,ǧ��������������Э��; 2,ǧ������һ������Э��(����˾������); 30,����ͨ�ð�Э��
#define USE_LANGUAGE	0	// 0,��������; 11,Ӣ��
#define SMALL_VER  1	// 1,��С���а汾,�����ļ�ϵͳ�ڣ�0��ͨ�����а汾

#ifdef XUN_DEBUG
#define XUN_ASSERT_VALID( boolval, strval ) { if(!(boolval)) assert(false); }
#else
#define XUN_ASSERT_VALID( boolval, strval ) {}
#endif

#define MSG_NOR		1
#define MSG_DBG		1
#define MSG_ERR		1
#define MSG_HEAD	("")

#define PRTMSG(enb, format, ...) \
if( enb ) \
{ \
	printf( "%s: ", MSG_HEAD ); \
	printf( format, ##__VA_ARGS__ ); \
}

#define PACKED_1 __attribute__( (packed, aligned(1)) )
#define STRUCT_1 struct PACKED_1

#define KEY_SEM_DOG	58588
#define KEY_SHM_DOG	58598

///////// { <1>ͨ�ñ��뿪�� (��������,��Ӧ������Ҫ���������)

// { ��������
#if CHK_VER == 0
#define USE_TELLIMIT	0		// 0,ǧ�������ĺ����������; 1,ǧ�������ĵ绰������; 2,�������ĵ绰������; 3,��һ������绰������
#else
#define USE_TELLIMIT	0
#endif
#define USE_TTS			1		// 1,TTS����֧��; 0,��֧��
#define CP_CHS			936
#define USE_SOCKBINDIP	0		// 0,socket����(��ʽ�汾ʹ��); 1,��GPRS�����IP(�ڵ���ʱʹ��)
#define USE_COMBUS		0		// 0, ��ʹ�ô�����չ��; 1, ʹ�ô�����չ��;	2, ʹ��150TR��ʻ��¼��
#define GPS_TEST		0		// 1, ��GPS���Թ���1; 2,��GPS���Թ���2; 0,�ر�
#define USE_JIDIANQI	0		// 1, ���ռ̵���; 0, �����̵���
#define USE_MENCHI		0		// 1, ʹ���Ŵſ���; 0, ��ʹ���Ŵſ���
#define USE_VIDEO_TCP	1		// 1, ��Ƶָ��ʹ�ö������׽��֣�0����ʹ�ö������׽���
// } ��������

// QianExeʹ��
#define USE_LIAONING_SANQI	0	// 1, ʹ�������ƶ�����Э�飬0, ��ʹ��
#define USE_BLK			1		// 1,֧�ֺ�ϻ��; 0,��֧��
#define USE_AUTORPT		0		// 1,֧�������ϱ�(0154); 0,��֧��
#define USE_METERTYPE	0		// 1,֧��˰��,�Ƽ����복̨֮���ô���ͨ��,�������س�����; 2,֧�ֿ���Ƽ���; 0,��֧��˰��
#define USE_LEDTYPE		0		// 1,ʹ�ò���LED��; 2,ʹ�ô���LED; 3,ʹ�ÿ���LED; 0,��ʹ��LED
#define USE_DRVREC		1		// 1,ʹ��JG2�ɵ���ʻ��¼��ģ��; 0,��ʹ��
//�� ��USE_DRVRECΪ1�����⼸����Ҫ����
#define USE_DRVRECRPT	0		// 1,3745��ʻ��¼�ϴ�; 2,3746; 3,3747; 4,3748; 11,3745_2��; 0,��֧��	(1��4�ݲ�֧��)
#define USE_ACDENT		0		// 1,�¹��ɵ�,ǧ��������Э��; 2,2������Э��; 0,��֧��
#define USE_DRIVERCTRL	0		// 1,ʹ��˾������ҵ��; 0,��ʹ��
// ����USE_DRVRECΪ1�����⼸����Ҫ����
#define USE_OIL			3		// 1,�ͺ�����ʹ��IOֱ�Ӷ�ȡ; 2,ʹ���ͺļ���,��ʹ����һ�㷨; 3,ʹ��150pd02-003�ͺļ���; 0,��֧��
#define USE_REALNAV		0		// 1,����ʵʱָ��(Ŀǰ����2��ϵͳ���õ�); 0,������
#define USE_JIJIA		0		// 1,֧�ּƼ���ģ��(�������ڴ�����IO����,����˰�ع����޹�); 0,��֧��
#define USE_REALPOS		1		// 1,֧��ʵʱ��λģ��; 0,��֧��
#define USE_DISCONNOILELEC 1	// 1,�������Ͷϵ�; 0,������
#define USE_HELPAUTODIAL 0		// 1,����ʱ�Զ��������ĵ������绰���豣֤�����绰������Ч��; 0,������
//#define DIS_SPD_USEIO			// �򿪴˿���,��ʻ��¼ģ��CDrvrecRptDlg�е���̺��ٶ�,ʹ��IO(�������)�źż���
#define UPLOAD_AFTERPHOTOALL 0	// 0,����1�ż��ϴ�1��; 1,ȫ��������ϲſ�ʼ�ϴ�
#define USE_PHOTOUPLD	0		// 0,ʹ�ô��ڷ�ʽ�ϴ�Э��; 1,ʹ��һ�����ϴ�������ƬЭ��
#define USE_PHOTOTHD	1		// 1,���մ���ʹ�õ������߳�; 0,��ʹ��
#define USE_AUTORPTSTATION	1	// 1,���ù����Զ���վ��0��������
#define USE_PLAYSOUNDTYPE	1	// 1,ʹ��TTS�������б�վ��2��ʹ��¼���ļ����б�վ

// ���¿���ҪС��ʹ��
#define USE_EXCEPTRPT	0		// 1,�쳣������ͱ������; 0,��֧��(ע��: �����汾������0)


//	ComuExeʹ�� SIMULATE_GPS:ģ���ܳ�
#define GPS_TYPE REAL_GPS
#define SIMULATE_GPS 1
#define REAL_GPS 2

// ComuExeʹ��
#define NETWORK_YIDONG	66
#define NETWORK_LIANTONG	88
#define NETWORK_GPRS 0
#define NETWORK_CDMA 1
#define NETWORK_TD	2
#define NETWORK_EVDO 3
#define NETWORK_GSM	4
#define NETWORK_WCDMA 5

// { �ֻ�ģ��ѡ�� 
#define PHONE_MODTYPE_TD_LC6311	10
#define PHONE_MODTYPE_TD_DTM6211 11
#define PHONE_MODTYPE_MC703OLD 12
#define PHONE_MODTYPE_MC703NEW 13
#define PHONE_MODTYPE_EM770W 14
#define PHONE_MODTYPE_BENQ 15
#define PHONE_MODTYPE_SIMCOM 16
#define PHONE_MODTYPE_SIM4222 17
#define PHONE_MODTYPE_SIM5218 18
#define PHONE_MODTYPE_MU203 19

#define NETWORK_GPRSTYPE NETWORK_YIDONG	// NETWORK_LIANTONG
//ע��: �����CDMAģ��Ҫѡ��ģ�������
#define CDMA_EM200 1
#define CDMA_CM320 2
#define CDMA_TYPE CDMA_EM200
// } �ֻ�ģ��ѡ��

#define AUTO_PICKUP		0		// 1,��ʾ�����Զ�����; 0, ���粻�Զ�����
#define GPSVALID_FASTFIXTIME 1	// 1,��ʾһ��GPS��Ч������Уʱ��0��������ЧԼ15���Ӻ��Уʱ
#define USE_IRD			0		// 1,��ʾʹ�ú�������,��ʹ���ֱ��͵�����(��ʱӦȷ������TTS); 0,��ʹ�ú�������,ʹ���ֱ��������(����Ӧ)
#define USE_IRD_KAITIAN		1	// 1,��ʾʹ�ÿ���������ۣ�0,��ʹ�ÿ����������
#define PRINT_GPS_DATA		0	// 1,��ʾ�ӵ��Դ��ڴ�ӡGPSԭʼ���ݣ�0, ����ӡ
//#define GPSGSA_ENB			// �Ƿ��ȡGSA��ʽ���� (PDOP��HDOP��VDOP)
#define USE_PING		0		// �Ƿ�ʹ��PING�Ļ��� 1:ʹ�� 0:��ʹ��
#if GPS_TEST && !defined(GPSGSA_ENB)
#define GPSGSA_ENB
#endif

// UpdateExeʹ��
#define WIRELESS_UPDATE_TYPE	0	// 1, ʹ�ó�̨ά�����Ľ���Զ�������� 0��ʹ����ý�����Ľ���Զ������
#define WIRELESS_DOWNLOAD_TYPE	1 // 1, ����ȫ������֡,һ����дFlash	0, ��֡дFlash

// DvrExeʹ��
#define DISK_UMOUNT_REBOOT	1	// 1, SD��δ���ظ�λ�� 0������λ
#define DISK_UCONNECT_INIT	1	// 1, SD��δʶ���ʼ���� 0������ʼ��
#define VIDEO_BLACK_TYPE	1	// 1, ��Ƶ��ϻ���ϴ�ʹ�þ�Э�飬2, ʹ����Э��

#if VEHICLE_TYPE == VEHICLE_M

	#define SYSTEM_VER		"1.01"

#if SMALL_VER == 0

	#define CFGSPEC	("YXYF3_��Ʒ����M-Xǰ;����_2010") // �ֽ���ӦС�������еĴ洢����,��������

#if CHK_VER == 0
	#define SOFT_VER		"yx003-110mw001-001"
#else
	#define SOFT_VER		"t005-110mw001-001"
#endif

#endif

#if SMALL_VER == 1

	#define SOFT_VER		"yx003-110mw001-bkp"
	#define CFGSPEC	("YXYF3_��Ʒ����M-Xǰ;����_2010") // �ֽ���ӦС�������еĴ洢����,��������

#endif

#endif



#if VEHICLE_TYPE == VEHICLE_V8

	#define SYSTEM_VER		"1.08"

#if SMALL_VER == 0

	#define CFGSPEC	("YXYF3_��Ʒ����V8-Xǰ;����_2010") // �ֽ���ӦС�������еĴ洢����,��������

#if CHK_VER == 0
	#define SOFT_VER		"yx01b-110v8t001-001"
#else
	#define SOFT_VER		"t008-110v8w001-001"
#endif

#endif

#if SMALL_VER == 1

	#define CFGSPEC	("YXYF3_��Ʒ����V8-Xǰ;����_2010") // �ֽ���ӦС�������еĴ洢����,��������
	#define SOFT_VER		"yx01b-110v8w001-bkp"

#endif

#endif



#if VEHICLE_TYPE == VEHICLE_M2

	#define SYSTEM_VER		"1.00"

#if SMALL_VER == 0

	#define CFGSPEC	("YXYF3_��Ʒ����M2-Xǰ;����_2010") // �ֽ���ӦС�������еĴ洢����,��������

#if CHK_VER == 0
	#define SOFT_VER		"yx001-110m2w001-001"
#else
	#define SOFT_VER		"t001-110m2w001-001"
#endif

#endif

#if SMALL_VER == 1

	#define CFGSPEC	("YXYF3_��Ʒ����M2-Xǰ;����_2010") // �ֽ���ӦС�������еĴ洢����,��������
	#define SOFT_VER		"yx001-110m2w001-bkp"

#endif

#endif



// ���ַ�������������д���������ڵ�m_szSpecode�ڣ��������������ָ��������ã���Ҫ�����ֻ��š�IP���˿�
#define NEED_RUSEMUCFG	("������ָ���������")

// ���ַ�������������д����Ҫ��������m_szReUpdate�ڣ������������������Ƿ���Ҫ�ٴ�����
#define NEED_REUPDATE	("�����쳣���ñ����ļ�����")

///////// } <1>ͨ�ñ��뿪��

/// { �������ó�ʼ������,������������Ĭ������,�ҽ����ڸ����ö����Init������,��ͬ�İ汾��Ҫע���޸�!
/// !!ע���Ƿ�������Ҫ�޸ĵ�Ĭ������
#if USE_LIAONING_SANQI == 1
#define APN			"cmcwt"				// APN���ã�һ�㲻�øģ�
#endif
#if USE_LIAONING_SANQI == 0
#define APN			"3gnet"				// APN���ã�һ�㲻�øģ�
#endif

#define CDMA_NAME   "card"
#define CDMA_PASS   "card"

#define QTCPIP		"27.115.0.114"		// ǧ����TCP IP��ַ
#define QTCPPORT	9020				// ǧ����TCP�˿�
#if USE_LIAONING_SANQI == 1										//
#define QUDPIP		"10.64.92.80"		// ǧ����UDP IP��ַ
#define QUDPPORT	11004				// ǧ����UDP�˿�
#endif
#if USE_LIAONING_SANQI == 0
#define QUDPIP		"27.115.0.114"		// ǧ����UDP IP��ַ
#define QUDPPORT	3100				// ǧ����UDP�˿�

#endif

#if USE_LIAONING_SANQI == 1	
#define DTCPIP		"211.137.43.183"	// DVR��Ƶ��TCP IP��ַ
#define DTCPPORT	6200				// DVR��Ƶ��TCP�˿�
#endif
#if USE_LIAONING_SANQI == 0
#define DTCPIP		"27.115.0.114"		// DVR��Ƶ��TCP IP��ַ
#define DTCPPORT	6900				// DVR��Ƶ��TCP�˿�
#endif
#define DUDPIP		"59.61.82.170"		// DVR��Ƶ��UDP IP��ַ
#define DUDPPORT	10101				// DVR��Ƶ��UDP�˿�

#define UTCPIP		"59.61.82.173"		// GPSKING���ĵ�������������IP
#define UTCPPORT	12345				// GPSKING���ĵ������������Ķ˿�
#define LUDPIP		"59.61.82.173"		// ��ý��(��Զ����������)UDP IP��ַ
#define LUDPPORT	18000				// ��ý��(��Զ����������)UDP�˿�
			
#define SMSCENTID	""					// �������ĺ�,ע��"+86"���ܶ� // +8613800592500
#define SMSSERVCODE	""					// �ط���
#define CENTID1		0x35		// ����ID(�������)��һ�ֽ�,ע��ʹ��16����
#define CENTID2		0x00		// ����ID(�������)�ڶ��ֽ�,ע��ʹ��16����,0����ת��Ϊ7f

#define USERIDENCODE "111111"	// ������_��һЭ���е�GPRS��½֡�е��û�ʶ����(6�ַ�),ÿ�����ư汾��Ӧ���Լ��̶���ʶ����

#define EXCEP_RPTTEL "15985886953"		// �쳣������ŵ�Ŀ���ֻ���


#define SOFT_VERSION		"05500030100"
#define COMPANY_ID			"1025"
#define GROUP_BILLING_ID	"9000038873"//"9000098732"
/// } �������ó�ʼ������,������������Ĭ������,�ҽ����ڸ����ö����Init������,��ͬ�İ汾��Ҫע���޸�!

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b)) 
#endif
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#define MAX_PATH	255
#define MSG_MAXLEN		1

#define FLASH_PART4_PATH		("/mnt/Flash/part4/")				// FLASH����4·����
#define DRVRECSTA_FILENAME		("/mnt/SDisk/part2/data/DrvRecSta.dat")	// ��ʻ״̬��¼�ļ���
#define DRIVE_RECORD_FILENAME	("/mnt/SDisk/part2/data/DrvJg2")	// �ɵ���ʻ��¼�Ǳ����ļ�
#define BLK_SAVE_PATH			("/mnt/SDisk/part2/data/Blk/")		// ��ϻ���ļ����·��
#define ACD_SAVE_PATH			("/mnt/SDisk/part2/data/Acd/")		// �¹��ɵ��ļ����·��
#define HANDLE_FILE_PATH		("/mnt/SDisk/part2/data")			// �ֱ������ļ�����Ŀ¼

#if VEHICLE_TYPE == VEHICLE_V8
#define	PHOTO_SAVE_PATH			("/mnt/SDisk/part2/data/")			// ��Ƭ����·��
#define	VIDEO_SAVE_PATH			("/mnt/SDisk/part1/data/")			// ��Ƶ����·��
#endif
#if VEHICLE_TYPE == VEHICLE_M || VEHICLE_TYPE == VEHICLE_M2
#define	PHOTO_SAVE_PATH			("/mnt/HDisk/part2/data/")			// ��Ƭ����·��
#define	VIDEO_SAVE_PATH			("/mnt/HDisk/part1/data/")			// ��Ƶ����·��
#endif

#define LIU_DOWNLOAD_PATH		("/mnt/Flash/part3/Down/")			// ��ý�������ļ��Ĵ��Ŀ¼
#define RPTSTATION_U_PATH		("/mnt/UDisk/part1/")				// �����ļ���U���ϵ�·��
#define	RPTSTATION_SAVE_PATH 	("/mnt/SDisk/part2/data/RptSta/")	// �����ļ�����·��

enum QUEELVL // �����1��ʼ����,����ֻ��16��,Խ��Խ����
{
	LV1 = 1,
	LV2,
	LV3,
	LV4,
	LV5,
	LV6,
	LV7,
	LV8,
	LV9,
	LV10,
	LV11,
	LV12,
	LV13,
	LV14,
	LV15,
	LV16
};

enum ERR_CODE
{
	ERR_SEMGET = 1,
	ERR_SEMCTL,
	ERR_SEMOP,
	
	ERR_SHMGET,
	ERR_SHMAT,
	
	ERR_MSGGET,
	ERR_MSGSND,
	ERR_MSGRCV,
	ERR_MSGSKIPBLOCK,

	ERR_THREAD,
	ERR_READCFGFAILED,

	ERR_MEMLACK,
	ERR_SPACELACK,
	ERR_BUFLACK,
	ERR_PTR,
	ERR_QUEUE_EMPTY,
	ERR_QUEUE_FULL,
	ERR_QUEUE,
	ERR_QUEUE_TOOSMALL,
	ERR_QUEUE_SYMBNOFIND,

	ERR_OPENFAIL,
	ERR_IOCTLFAIL,
	ERR_LSEEKFAIL,
	ERR_WRITEFAIL,
	ERR_READFAIL,

	ERR_SOCKLIB,
	ERR_SOCK,
	ERR_SOCK_TCPFBID,

	ERR_FILEMAP,
	ERR_FILE_FAILED,
	ERR_FILE,

	ERR_COM,
	ERR_IO,
	ERR_PHOTO,
	ERR_CFGBAD,		
	ERR_COMCHK,
	ERR_COMSND,
	ERR_COMSND_UNALL,

	ERR_DLL,
	ERR_PAR,	
	ERR_CFG,
	ERR_DATA_INVALID,
	ERR_BLACKGPS_INVALID,
	ERR_GPS_INVALID,
	ERR_INNERDATA_ERR,
	ERR_STA_ERR,

	ERR_AUTOLSN,
	ERR_AUTOLSN_NOTEL,	

	ERR_CODECVT,

	ERR_LED,
	ERR_LED_QUEE,

	ERR_SMS_DESCODE, // ���ŷ���Ŀ�ķ��ֻ��Ų���ȷ

	ERR_TM,

	ERR_SYSCALL,
	ERR_SIG,
};

enum PHONE_TYPE
{
	PHONE_BENQ = 0,
	PHONE_SIMCOM,
	PHONE_HWCDMA,
	PHONE_DTTD
};

#define VALIDYEAR_BEGIN		2009
#define VALIDMONTH_BEGIN	10 // ��VALIDYEAR_BEGIN�е�month

#define TCP_RECVSIZE	1024
#define TCP_SENDSIZE	60000 //1024
#define UDP_RECVSIZE	8096
#define UDP_SENDSIZE	8096
#define SOCK_MAXSIZE	60000 //1500
#define COMUBUF_SIZE	2048
#define UDPDATA_SIZE	1200 // UDP���ݱ����������ݵ���󳤶�, Ҳ����Ƭ����֡һ�����Ĵ�С�޶�ֵ
// !!ע��,��ֵӦС��1500,��С��SOCKSND_MAXBYTES_PERSEC-QIANUPUDPFRAME_BASELEN

#define MAX_UNSIGNED_64		((unsigned long long)(-1))
#define MAX_UNSIGNED_32		((unsigned long)(-1))
#define MAX_UNSIGNED_16		((unsigned short)(-1))
#define MAX_UNSIGNED_8		((unsigned char)(-1))

const double BELIV_MAXSPEED = 100 * 1.852 / 3.6; // ��λ:��/��

const BYTE GPS_REAL		= 0x01;
const BYTE GPS_LSTVALID	= 0x02;

const WORD PHOTOEVT_FOOTALERM		= 0x8000; // �پ�
const WORD PHOTOEVT_OVERTURNALERM	= 0x4000; // �෭����
const WORD PHOTOEVT_BUMPALERM		= 0x2000; // ��ײ����
const WORD PHOTOEVT_FOOTALERM_BIG	= 0x1000; // �پ�ʱ���ӵĴ���Ƭ,�����������ϴ�
const WORD PHOTOEVT_TRAFFICHELP		= 0x0800; // ��ͨ����
const WORD PHOTOEVT_MEDICALHELP		= 0x0400; // ҽ������
const WORD PHOTOEVT_BOTHERHELP		= 0x0200; // ��������
const WORD PHOTOEVT_OPENDOOR		= 0x0100; // ������
const WORD PHOTOEVT_AREA			= 0x0080; // ��������
const WORD PHOTOEVT_TIRE			= 0x0080; // ƣ�ͼ�ʻ											  //
const WORD PHOTOEVT_ACCON			= 0x0040; // ACC��Ч
const WORD PHOTOEVT_DRIVEALERM		= 0x0020; // �Ƿ�����������������
const WORD PHOTOEVT_OVERSPEED		= 0x0010; // ����
const WORD PHOTOEVT_JIJIADOWN		= 0x0008;// �ճ����س�
const WORD PHOTOEVT_BELOWSPEED		= 0x0004;// ����
const WORD PHOTOEVT_TIMER			= 0x0002; // ��ʱ����
const WORD PHOTOEVT_CENT			= 0x0001; // ����ץ��

const WORD PHOTOUPLOAD_FOOTALERM		= PHOTOEVT_FOOTALERM;
const WORD PHOTOUPLOAD_OVERTURNALERM	= PHOTOEVT_OVERTURNALERM;
const WORD PHOTOUPLOAD_BUMPALERM		= PHOTOEVT_BUMPALERM;
const WORD PHOTOUPLOAD_TRAFFICHELP		= PHOTOEVT_TRAFFICHELP;
const WORD PHOTOUPLOAD_MEDICALHELP		= PHOTOEVT_MEDICALHELP;
const WORD PHOTOUPLOAD_BOTHERHELP		= PHOTOEVT_BOTHERHELP;
const WORD PHOTOUPLOAD_OPENDOOR		= PHOTOEVT_OPENDOOR;
const WORD PHOTOUPLOAD_AREA				= PHOTOEVT_AREA;
const WORD PHOTOUPLOAD_TIRE				= PHOTOEVT_TIRE;
const WORD PHOTOUPLOAD_ACCON			= PHOTOEVT_ACCON;
const WORD PHOTOUPLOAD_DRIVEALERM		= PHOTOEVT_DRIVEALERM;
const WORD PHOTOUPLOAD_OVERSPEED		= PHOTOEVT_OVERSPEED;
const WORD PHOTOUPLOAD_JIJIADOWN		= PHOTOEVT_JIJIADOWN;
const WORD PHOTOUPLOAD_BELOWSPEED		= PHOTOEVT_BELOWSPEED;
const WORD PHOTOUPLOAD_TIMER			= PHOTOEVT_TIMER;
const WORD PHOTOUPLOAD_CENT				= PHOTOEVT_CENT;


/// ���̼�ͨ�Ŷ��д�������, �����ǽ��̻��߳� ���¶���,������Ϊ1��bit�������� !!!
const DWORD DEV_SOCK	= 0x00000001;
const DWORD DEV_LED		= 0x00000002;
const DWORD DEV_QIAN	= 0X00000004;
const DWORD DEV_DVR		= 0x00000008;
const DWORD DEV_PHONE	= 0x00000010;
const DWORD DEV_DIAODU	= 0x00000020;
const DWORD DEV_UPDATE	= 0x00000040;
const DWORD DEV_IO		= 0x00000080; // ����Ҫ���ն���
const DWORD DEV_GPS		= 0x00000100; // ����Ҫ���ն���


/// ���Ź�����
const DWORD DOG_QIAN	= 0X00000001;
const DWORD DOG_COMU	= 0X00000002;
const DWORD DOG_SOCK	= 0X00000004;
const DWORD DOG_IO		= 0X00000008;
const DWORD DOG_UPGD	= 0X00000010;
const DWORD DOG_DVR		= 0X00000020;
const DWORD DOG_DVR_T1	= 0X00000040;
const DWORD DOG_DVR_T2	= 0X00000080;
const DWORD DOG_DVR_T3	= 0X00000100;


#if VEHICLE_TYPE == VEHICLE_M
// ������SYSIO_CFG[]�Ķ���˳��һ��!�ұ����0��ʼ���,ע����ȡֵ��Χ: 0~254 (255����Ϊ�����������ʹ��)
enum IOSymb
{
	// ����
	IOS_ACC = 0,
	IOS_ALARM, //�پ��ź�
	IOS_JIJIA, // ������
	IOS_FDOOR, //ǰ���ź�
	IOS_BDOOR, //�����ź�
	IOS_JIAOSHA,
	IOS_SHOUSHA,
	IOS_PASSENGER1, //�ؿ��ź�1
	IOS_PASSENGER2, //�ؿ��ź�2
	IOS_PASSENGER3, //�ؿ��ź�3
	IOS_HDELELOCK, // ������
	
	
	// ���
	IOS_HUBPOW,
	IOS_PHONEPOW,
	IOS_PHONERST,
	IOS_HARDPOW, //Ӳ�̵�Դ
	IOS_TRUMPPOW, //���ŵ�Դ
	IOS_SCHEDULEPOW, //��������Դ
	IOS_VIDICONPOW, //����ͷ/TW2835��Դ
	IOS_OILELECCTL, //���͵����
	IOS_APPLEDPOW, //������ָʾ��
	IOS_TOPLEDPOW, //���Ƶ�Դ
	IOS_AUDIOSEL, //�ֻ���Ƶ/PC��Ƶѡ��
	IOS_EARPHONESEL, //����/����ѡ��
	IOS_TW2835RST, //TW2835��λ
// 	IOS_GPS,
// 	IOS_SYSLEDPOW, //ϵͳ����ָʾ��
};

// IOӦ�����嶨��,����ⲿIO�ź�,��CPU�ܽ��ź�
enum IO_APP
{
	// ����
	IO_ACC_ON = 1, // �����1��ʼ,��Ϊ�ڲ������õ���unsigned char,��Լ�ռ�
	IO_ACC_OFF,
	IO_ALARM_ON,
	IO_ALARM_OFF,
	IO_JIJIA_HEAVY,
	IO_JIJIA_LIGHT,
	IO_FDOOR_OPEN,
	IO_FDOOR_CLOSE,
	IO_BDOOR_OPEN,
	IO_BDOOR_CLOSE,
	IO_JIAOSHA_VALID,
	IO_JIAOSHA_INVALID,
	IO_SHOUSHA_VALID,
	IO_SHOUSHA_INVALID,
	IO_PASSENGER1_VALID,
	IO_PASSENGER1_INVALID,
	IO_PASSENGER2_VALID,
	IO_PASSENGER2_INVALID,
	IO_PASSENGER3_VALID,
	IO_PASSENGER3_INVALID,
	IO_HDELELOCK_OPEN,
	IO_HDELELOCK_CLOSE,
	
	
	// ���
	IO_HUBPOW_ON,
	IO_HUBPOW_OFF,
	IO_PHONEPOW_ON,
	IO_PHONEPOW_OFF,
	IO_PHONERST_HI,
	IO_PHONERST_LO,
	IO_HARDPOW_ON, 
	IO_HARDPOW_OFF, 
	IO_TRUMPPOW_ON,
	IO_TRUMPPOW_OFF,
	IO_SCHEDULEPOW_ON, 
	IO_SCHEDULEPOW_OFF, 
	IO_VIDICONPOW_ON, 
	IO_VIDICONPOW_OFF, 
	IO_OILELECCTL_CUT, 
	IO_OILELECCTL_RESUME, 
	IO_APPLEDPOW_ON, 
	IO_APPLEDPOW_OFF,
	IO_TOPLEDPOW_ON,
	IO_TOPLEDPOW_OFF,
	IO_AUDIOSEL_PHONE, //ѡ���ֻ���Ƶ
	IO_AUDIOSEL_PC, //ѡ��PC��Ƶ
	IO_EARPHONESEL_ON,//ѡ�����
	IO_EARPHONESEL_OFF,//ѡ������
	IO_TW2835RST_HI,
	IO_TW2835RST_LO,
// 	IO_GPSPOW_ON,
// 	IO_GPSPOW_OFF,
// 	IO_SYSLEDPOW_ON, 
// 	IO_SYSLEDPOW_OFF, 
};
#endif

#if VEHICLE_TYPE == VEHICLE_V8
// ������SYSIO_CFG[]�Ķ���˳��һ��!�ұ����0��ʼ���,ע����ȡֵ��Χ: 0~254 (255����Ϊ�����������ʹ��)
enum IOSymb
{
	// ����
	IOS_ACC = 0,
	IOS_QIANYA,			// Ƿѹ�ź�
	IOS_ELECSPEAKER,	// ���������ź�
	IOS_FDOOR,			// ǰ���ź�
	IOS_ALARM,			// �پ��ź�
	IOS_POWDETM,		// ��Դ�������ź�
	IOS_JIJIA,			// �������ź�
	IOS_CHEDENG,		// ���������ź�
	IOS_ZHENLING,		// �����ź�
//	IOS_SPIDO,			// SD������ź�
//	IOS_SPICK,			// SD��д�����ź�
		
	// ���
	IOS_SYSLEDPOW_RED,	// ����ָʾ�ƣ��죩
	IOS_SYSLEDPOW_GREEN,// ����ָʾ�ƣ��̣�
	IOS_SCHEDULEPOW,	// ��������Դ
	IOS_TRUMPPOW,		// ��Ƶ���ŵ�Դ
	IOS_EARPHONESEL,	// ����/����ѡ��
	IOS_AUDIOSEL,		// �ֻ���Ƶ/PC��Ƶѡ��
	IOS_PHONERST,		// �ֻ�ģ�鸴λ
	IOS_PHONEPOW,		// �ֻ�ģ���Դ
	IOS_YUYIN,			// �����л�����
	IOS_OILELECCTL,		// ���͵����
	IOS_VIDICONPOW,		// ����ͷ/TW2835��Դ
	IOS_TW2865RST,		// TW2865��λ
	IOS_LCDPOW,			// LCD��Դ����
	IOS_USBPOW,			// USB2.0��Դ
};

// IOӦ�����嶨��,����ⲿIO�ź�,��CPU�ܽ��ź�
enum IO_APP
{
	// ����
	IO_ACC_ON = 1, // �����1��ʼ,��Ϊ�ڲ������õ���unsigned char,��Լ�ռ�
	IO_ACC_OFF,
	IO_QIANYA_VALID,
	IO_QIANYA_INVALID,
	IO_ELECSPEAKER_VALID,
	IO_ELECSPEAKER_INVALID,
	IO_FDOOR_OPEN,
	IO_FDOOR_CLOSE,
	IO_ALARM_ON,
	IO_ALARM_OFF,
	IO_POWDETM_VALID,
	IO_POWDETM_INVALID,
	IO_JIJIA_HEAVY,
	IO_JIJIA_LIGHT,
	IO_CHEDENG_VALID,
	IO_CHEDENG_INVALID,
	IO_ZHENLING_VALID,
	IO_ZHENLING_INVALID,
//	IO_SPIDO_CARD_EXIST,
//	IO_SPIDO_CARD_NOTEXIST,
//	IO_SPICK_CARD_PROTECT,
//	IO_SPICK_CARD_NOTPROTECT,
			
	// ���
	IO_SYSLEDPOW_RED_ON,
	IO_SYSLEDPOW_RED_OFF,
	IO_SYSLEDPOW_GREEN_ON,
	IO_SYSLEDPOW_GREEN_OFF,
	IO_SCHEDULEPOW_ON, 
	IO_SCHEDULEPOW_OFF, 
	IO_TRUMPPOW_ON,
	IO_TRUMPPOW_OFF,
	IO_EARPHONESEL_ON,	// ѡ�����
	IO_EARPHONESEL_OFF,	// ѡ������
	IO_AUDIOSEL_PHONE,	// ѡ���ֻ���Ƶ
	IO_AUDIOSEL_PC,		// ѡ��PC��Ƶ
	IO_PHONERST_HI,
	IO_PHONERST_LO,
	IO_PHONEPOW_ON,
	IO_PHONEPOW_OFF,
	IO_YUYIN_ON,
	IO_YUYIN_OFF,
	IO_OILELECCTL_CUT, 
	IO_OILELECCTL_RESUME,
	IO_VIDICONPOW_ON, 
	IO_VIDICONPOW_OFF,
	IO_TW2865RST_HI,
	IO_TW2865RST_LO,
	IO_LCDPOW_ON,
	IO_LCDPOW_OFF,
	IO_USBPOW_ON,
	IO_USBPOW_OFF,
};
#endif

#if VEHICLE_TYPE == VEHICLE_M2
// ������SYSIO_CFG[]�Ķ���˳��һ��!�ұ����0��ʼ���,ע����ȡֵ��Χ: 0~254 (255����Ϊ�����������ʹ��)
enum IOSymb
{
	// ����
	IOS_ACC = 0,
	IOS_QIANYA,			// Ƿѹ�ź�
	IOS_FDOOR_ALERT,	// ǰ�ű���
	IOS_FDOOR,			// ǰ���ź�
	IOS_BDOOR_ALERT,	// ���ű���
	IOS_BDOOR,			// �����ź�
	IOS_ALARM,			// �پ��ź�
	IOS_JIAOSHA,		// ��ɲ�ź�
	IOS_POWDETM,		// ��Դ�������ź�
	IOS_CHEDENG,		// ���������ź�
	IOS_ZHENLING,		// �����ź�
//	IOS_SPIDO,			// SD������ź�
//	IOS_SPICK,			// SD��д�����ź�
	
	// ���
	IOS_SYSLEDPOW_RED,	// ����ָʾ�ƣ��죩
	IOS_SYSLEDPOW_GREEN,// ����ָʾ�ƣ��̣�
	IOS_SCHEDULEPOW,	// ��������Դ
	IOS_TRUMPPOW,		// ��Ƶ���ŵ�Դ
	IOS_EARPHONESEL,	// ����/����ѡ��
	IOS_AUDIOSEL,		// �ֻ���Ƶ/PC��Ƶѡ��
	IOS_PHONERST,		// �ֻ�ģ�鸴λ
	IOS_PHONEPOW,		// �ֻ�ģ���Դ
	IOS_YUYIN,			// �����л�����
	IOS_JIDIANQI,		// ���ü̵�������
	IOS_OILELECCTL,		// ���͵����
	IOS_VIDICONPOW,		// ����ͷ/TW2835��Դ
	IOS_TW2865RST,		// TW2865��λ
	IOS_LCDPOW,			// LCD��Դ����
//	IOS_USBPOW,			// USB2.0��Դ
	IOS_HUBPOW,			// HUB��Դ����
	IOS_HARDPOW,		// Ӳ�̵�Դ����
};

// IOӦ�����嶨��,����ⲿIO�ź�,��CPU�ܽ��ź�
enum IO_APP
{
	// ����
	IO_ACC_ON = 1, // �����1��ʼ,��Ϊ�ڲ������õ���unsigned char,��Լ�ռ�
	IO_ACC_OFF,
	IO_QIANYA_VALID,
	IO_QIANYA_INVALID,
	IO_FDOORALERT_VALID,
	IO_FDOORALERT_INVALID,
	IO_FDOOR_OPEN,
	IO_FDOOR_CLOSE,
	IO_BDOORALERT_VALID,
	IO_BDOORALERT_INVALID,
	IO_BDOOR_OPEN,
	IO_BDOOR_CLOSE,
	IO_ALARM_ON,
	IO_ALARM_OFF,
	IO_JIAOSHA_VALID,
	IO_JIAOSHA_INVALID,
	IO_POWDETM_VALID,
	IO_POWDETM_INVALID,
	IO_CHEDENG_VALID,
	IO_CHEDENG_INVALID,
	IO_ZHENLING_VALID,
	IO_ZHENLING_INVALID,
	//	IO_SPIDO_CARD_EXIST,
	//	IO_SPIDO_CARD_NOTEXIST,
	//	IO_SPICK_CARD_PROTECT,
	//	IO_SPICK_CARD_NOTPROTECT,
	
	// ���
	IO_SYSLEDPOW_RED_ON,
	IO_SYSLEDPOW_RED_OFF,
	IO_SYSLEDPOW_GREEN_ON,
	IO_SYSLEDPOW_GREEN_OFF,
	IO_SCHEDULEPOW_ON, 
	IO_SCHEDULEPOW_OFF, 
	IO_TRUMPPOW_ON,
	IO_TRUMPPOW_OFF,
	IO_EARPHONESEL_ON,	// ѡ�����
	IO_EARPHONESEL_OFF,	// ѡ������
	IO_AUDIOSEL_PHONE,	// ѡ���ֻ���Ƶ
	IO_AUDIOSEL_PC,		// ѡ��PC��Ƶ
	IO_PHONERST_HI,
	IO_PHONERST_LO,
	IO_PHONEPOW_ON,
	IO_PHONEPOW_OFF,
	IO_YUYIN_ON,
	IO_YUYIN_OFF,
	IO_JIDIANQI_ON,
	IO_JIDIANQI_OFF,
	IO_OILELECCTL_CUT, 
	IO_OILELECCTL_RESUME,
	IO_VIDICONPOW_ON, 
	IO_VIDICONPOW_OFF,
	IO_TW2865RST_HI,
	IO_TW2865RST_LO,
	IO_LCDPOW_ON,
	IO_LCDPOW_OFF,
//	IO_USBPOW_ON,
//	IO_USBPOW_OFF,
	IO_HUBPOW_ON,
	IO_HUBPOW_OFF,
	IO_HARDPOW_ON,
	IO_HARDPOW_OFF,
};
#endif

struct tagIOCfg
{
	unsigned int m_uiPinNo; // �ܽű��
	unsigned int m_uiContPrid; // �ж��źų���ʱ��(ʹ���ж�ʱ��������),ȥ����,ms
	unsigned char m_ucInOut;	// 0,����; 1,���
	bool m_bBreak;	// true,ʹ���ж�(����ʱ��������); false,��ֹ�ж�
	char m_cBreakType; // �жϷ�ʽ(ʹ���ж�ʱ��������): 1,��ƽ����; 2,�����ش���; 3,˫���ش���
	char m_cBreakPow; // �жϴ�����ƽ(�ڵ�ƽ�����͵����ش���ʱ��������): 0, �͵�ƽ���½��ش���; 1,�ߵ�ƽ�������ش���
	unsigned char m_ucPinLowApp; // �ܽŵ͵�ƽ��Ӧ������
	unsigned char m_ucPinHigApp; // �ܽŸߵ�ƽ��Ӧ������
};

#define TCP_MAX_LOG_TIMERS	3	// TCP���������½ʧ�ܴ���

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 30
#define TCPFRAME_BASELEN	3	// TCP����֡��������,����ͷβ7e���û�����
#define UDPFRAME_BASELEN	3	// UDP����֡��������,�����û�����
#define QIANDOWNFRAME_BASELEN	36	// ���е�SMS����֡��������,����У���
#define QIANUPTCPFRAME_BASELEN		6	// ���е�TCP SMS����֡��������,����У���
#define QIANUPUDPFRAME_BASELEN		21	// ���е�UDP SMS����֡��������,����У���
#endif

#if USE_PROTOCOL == 2
#define TCPFRAME_BASELEN	4	// TCP����֡��������,����ͷβ7e���û�����
#define UDPFRAME_BASELEN	4	// UDP����֡��������,�����û�����
#define QIANDOWNFRAME_BASELEN	37	// ���е�SMS����֡��������,����У���
#define QIANUPTCPFRAME_BASELEN		22	// ���е�TCP SMS����֡��������,����У���
#define QIANUPUDPFRAME_BASELEN		22	// ���е�UDP SMS����֡��������,����У���
#endif

STRUCT_1 tagGPRS01
{
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	BYTE		m_bytParCount;		// �̶�Ϊ1
	BYTE		m_bytParType_1;		// �̶�Ϊ0x01
	BYTE		m_bytParLen_1;		// �̶�Ϊ15
	char		m_szParData[15];	// �̶�Ϊ�ֻ���,���治�㲹�ո�
	BYTE		m_bytParType_2;
	BYTE		m_bytParLen_2;
	BYTE		m_bytVersion;
	
	tagGPRS01()
	{
		m_bytParCount = 2;
		m_bytParType_1 = 1;
		m_bytParLen_1 = 15;
		m_bytParType_2 = 2;
		m_bytParLen_2 = 1;
		m_bytVersion = 0x31;
	}
#endif
	
#if 2 == USE_PROTOCOL
	//�û�ʶ���루6��	����������1��	����1�� �ͣ�1��	����1���ȣ�1��	����1���ݣ�n��	����2���ͣ�1��	����2���ȣ�1��	����2���ݣ�n��	����
	char		m_szUserIdenCode[6];
	BYTE		m_bytParCount;		// �̶�Ϊ1
	BYTE		m_bytParType_1;		// �̶�Ϊ0x01
	BYTE		m_bytParLen_1;		// �̶�Ϊ15
	char		m_szParData[15];	// �̶�Ϊ�ֻ���,���治�㲹�ո�
	
	tagGPRS01()
	{
		memset( this, 0, sizeof(*this) );
		m_bytParCount = 1;
		m_bytParType_1 = 1;
		m_bytParLen_1 = 15;
	}
#endif
};


#pragma pack(8)

typedef struct tagGPSData
{
	double		latitude;         //γ��0��90
	double		longitude;        //����0��180
	float		speed;           //�ٶȣ���/��
	float		direction;        //����0��360
	float		mag_variation;    //�ű�Ƕȣ�0��180
	int			nYear; // 4�����ֱ�ʾ��ֵ,��2007,������7;
	int			nMonth;
	int			nDay;
	int			nHour;
	int			nMinute;
	int			nSecond;
	char		valid;             //��λ�����Ƿ���Ч
	BYTE		la_hemi;          //γ�Ȱ���'N':������'S':�ϰ���
    BYTE		lo_hemi;           //���Ȱ���'E':������'W':������
	BYTE		mag_direction;    //�ű䷽��E:����W:��
	BYTE		m_bytMoveType; // 1,�ƶ�; 2,��ֹ
	BYTE		m_bytPDop; // GPS������ӡ�10,��ͬ
	BYTE		m_bytVDop;
	BYTE		m_bytHDop;
	char		m_cFixType; // ��λ����: '1'-δ��λ; '2'-2D��λ; '3'-3D��λ
	
	tagGPSData(char c)
	{
		Init();
	}
	tagGPSData()
	{
		Init();
	}
	void Init()
	{
		// ��ʼ����ͬ,dll�Ĵ�����ܲ�ͬ,ע��!
		memset( this, 0, sizeof(*this) );
		nYear = VALIDYEAR_BEGIN, nMonth = VALIDMONTH_BEGIN, nDay = 15;
		valid = 'V', m_cFixType = '1';
	}
}GPSDATA;

#pragma pack()


#pragma pack(8)

/// ��������:
// (1)������Ҫ���õ�,ǰ׺��"1";����һ�����õ�,ǰ׺��"2"
// (2)��������,ǰ׺��"P";��ý������,ǰ׺��"L";ǧ��������,ǰ׺��"Q"
// (3)һ�����ö���,��"Cfg"��β; �������ö���,��"Par"��β

/// ���µĵ绰����,��������˵��,�������ǲ��ո�

struct tag1PComuCfg
{
	char		m_szDialCode[33];
	char		m_szSmsCentID[22];	// �������ĺ�,������0
	char		m_szTel[16];		// ��̨�ֻ���,��NULL��β
	int			m_iRegSimFlow;		// ע��SIM������
	byte		m_PhoneVol;			// �绰����
//	char		m_szUserIdentCode[6]; // �û�ʶ����	
	
	char        m_szCdmaUserName[50];
	char        m_szCdmaPassWord[30];

	char		m_szTernimalID[8];	// ��½ά������ʱ���ƶ�̨ID��

#if USE_LIAONING_SANQI == 1
	bool		m_bRegsiter;			// �Ƿ�����������ע��
	int			m_iHeartKeepInterval;	// �������
#endif

	void Init(const char *v_szCdmaUserName , const char *v_szCdmaPassWord)
	{
		memset( this, 0, sizeof(*this) );
		strncpy( m_szSmsCentID, SMSCENTID, sizeof(m_szSmsCentID) );
		m_szSmsCentID[ sizeof(m_szSmsCentID) - 1 ] = 0;
		strncpy( m_szCdmaUserName , v_szCdmaUserName, sizeof(m_szCdmaUserName) );
		m_szCdmaUserName[sizeof(m_szCdmaUserName) -1] = 0;
		strncpy( m_szCdmaPassWord , v_szCdmaPassWord, sizeof(m_szCdmaPassWord) );
		m_szCdmaPassWord[sizeof(m_szCdmaPassWord) -1] = 0;
		m_iRegSimFlow = 1;
		m_PhoneVol = 3;		//0-5

#if USE_LIAONING_SANQI == 1
		m_bRegsiter = false;
		m_iHeartKeepInterval = 20;//60;
#endif
	}
};

struct tag1LComuCfg
{
	unsigned long m_ulLiuIP; // ��ý����������IP
	unsigned long m_ulLiuIP2; // ά������ƽ̨����IP
	unsigned short m_usLiuPort;
	unsigned short m_usLiuPort2;
	char	m_szLiuSmsTel[15];
	char	m_szVeIDKey[20]; // ʵ���Ѳ���,�ѵ����������
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );
		m_ulLiuIP = inet_addr(LUDPIP); 
		m_usLiuPort = htons(LUDPPORT);
	}
};

struct tag1LMediaCfg
{
	BYTE		m_bytPlayTimes; // ��ý�岥�Ŵ���
	//unsigned long m_aryDriverID[5]; // ����ע��˾��ID
	void Init()
	{
		memset( this, 0, sizeof(*this) );
		m_bytPlayTimes = 1;
	}
};

// ע�⣺��tag1QIpCfg�ṹ�����ϵ���ָ��������õĹؼ����⣬�������ӻ���ٳ�Ա������������������������
struct tag1QIpCfg
{
	unsigned long m_ulQianTcpIP; // (inet_addr֮���,��ͬ)
	unsigned long m_ulVUdpIP; // ��Ƶ�ϴ�UDP IP
	unsigned long m_ulQianUdpIP;
	unsigned short m_usQianTcpPort; // (htons֮��ģ���ͬ) 
	unsigned short m_usVUdpPort;
	unsigned short m_usQianUdpPort;
	char		m_szApn[20]; // ��NULL��β

	void Init( unsigned long v_ulQianTcpIP, unsigned short v_usQianTcpPort,
				unsigned long v_ulVUdpIP, unsigned short v_usVUdpPort,
				unsigned long v_ulQianUdpIP, unsigned short v_usQianUdpPort,
		const char *const v_szApn )
	{
		memset(this, 0, sizeof(*this));
		strncpy( m_szApn, v_szApn, sizeof(m_szApn) - 1 );

		m_ulQianTcpIP = v_ulQianTcpIP, m_usQianTcpPort = v_usQianTcpPort;
		m_ulVUdpIP = v_ulVUdpIP,   m_usVUdpPort = v_usVUdpPort;
		m_ulQianUdpIP = v_ulQianUdpIP, m_usQianUdpPort = v_usQianUdpPort;
	}
};

struct tag1QBiaoding
{
	WORD		m_wCyclePerKm; // ÿ����������
	//HXD 081120 �ͺ���صĲ�����
	BYTE		m_bReverse;
	BYTE	    m_bFiltrate;
	BYTE	    m_bytHiVal;
	BYTE		m_bytLoVal;

	void Init()
	{
		memset( this, 0, sizeof(*this) );
	}
};

//�ͺı궨
struct tag1QOilBiaodingCfg
{
	unsigned short	   m_aryONOilBiaoding[5][2];
	unsigned short	   m_aryOFFOilBiaoding[5][2];
	
	void Init()
	{
		memset( this, 0, sizeof(*this));
	}
};

// ����20070914����,��Ϊ������Ӧ����ҵ����зֿ�,����ÿ���СӦ��40��������
struct tagImportantCfg
{
	char		m_szSpecCode[40];

	// ����ͨ�Ų���
	union
	{
		char		m_szReserved[160];
		tag1PComuCfg m_obj1PComuCfg;
	} m_uni1PComuCfg;

	// ��ý��ͨ�Ų���
	union
	{
		char		m_szReserved[160];
		tag1LComuCfg m_obj1LComuCfg;
	} m_uni1LComuCfg;

	// ��ý�岥������
	union
	{
		char		m_szReserved[120];
		tag1LMediaCfg m_obj1LMediaCfg;
	} m_uni1LMediaCfg;

	// ǧ����IP
	union
	{
		char		m_szReserved[200];
		tag1QIpCfg	m_ary1QIpCfg[3];
	} m_uni1QComuCfg;

	// �ͺı궨ֵ
	union
	{
		char		m_szReserved[80];
		tag1QOilBiaodingCfg	m_obj1QOilBiaodingCfg;
	} m_uni1QOilBiaodingCfg;

	// IO���ݷ���ı궨ֵ
	union
	{
		char		m_szReserved[80];
		tag1QBiaoding m_obj1QBiaodingCfg;
	} m_uni1QBiaodingCfg;

	char		m_szReUpdate[40];

	void InitDefault()
	{
		memset(this, 0, sizeof(*this));
		strncpy( m_szSpecCode, CFGSPEC, sizeof(m_szSpecCode) - 1 );
		strncpy( m_szReUpdate, NEED_REUPDATE, sizeof(m_szReUpdate) - 1 );

		m_uni1PComuCfg.m_obj1PComuCfg.Init(CDMA_NAME,CDMA_PASS);
		m_uni1LComuCfg.m_obj1LComuCfg.Init();
		m_uni1LMediaCfg.m_obj1LMediaCfg.Init();
		m_uni1QComuCfg.m_ary1QIpCfg[0].Init( inet_addr(QTCPIP), htons(QTCPPORT),
											inet_addr(LUDPIP), htons(LUDPPORT),
											inet_addr(QUDPIP), htons(QUDPPORT),	APN);

		m_uni1QComuCfg.m_ary1QIpCfg[1].Init( inet_addr(DTCPIP), htons(DTCPPORT),
											inet_addr(LUDPIP), htons(LUDPPORT),
											inet_addr(DUDPIP), htons(DUDPPORT),	APN);

		m_uni1QBiaodingCfg.m_obj1QBiaodingCfg.Init();
		m_uni1QOilBiaodingCfg.m_obj1QOilBiaodingCfg.Init();
	}
};

struct tag2LMediaCfg
{
	DWORD		m_dwSysVolume; // ϵͳ����
	byte        m_bytInvPhoto; //090611 �Ƿ�Ҫ�ߵ���Ƭ
	void Init()
	{
		memset(this, 0, sizeof(*this));
		m_dwSysVolume = 75;
	}
};

struct tag2QBlkCfg
{
	DWORD			m_dwBlackChkInterv; // ��λ:��

	void Init()
	{
		memset(this, 0, sizeof(*this));
		m_dwBlackChkInterv = 30; // ��ȫ���
	}
};

struct tag2QRealPosCfg // 432
{
	double	m_dLstLon;
	double	m_dLstLat;
	double	m_dLstSendDis; // �ϴη�������֮���ۻ���ʻ�ľ��� (��)

	float	m_fSpeed; // ��/��, 0��ʾ��Ч

	DWORD	m_dwLstSendTime; // �ϴη���ʱ��

	long	m_aryArea[16][4]; // ÿ����Χ������: ���¾��ȡ�����γ�ȡ����Ͼ��ȡ�����γ�� (����10��-6�η���)
	DWORD	m_aryAreaContCt[16]; // ���������ļ��� (Ԫ�ظ���������һ��)
	BYTE	m_aryAreaProCode[16]; // ����Χ�ı�� (Ԫ�ظ���������һ��)
	BYTE	m_bytAreaCount;

	BYTE	m_bytGpsType;
	WORD	m_wSpaceInterv; // ����,0��ʾ��Ч
	WORD	m_wTimeInterv; // ����,0��ʾ��Ч

	char	m_szSendToHandtel[15];

	BYTE	m_aryTime[10][3]; // ÿ��ʱ��������: ���,ʱ,��
	BYTE	m_bytTimeCount;

	BYTE	m_aryStatus[6];
	// �̶�Ϊ6��, BYTE	m_bytStaCount; 

	BYTE	m_bytCondStatus; // 0	1	I	T	V	A	S	0
						//	�ɣ��ر�/�������������0-�رգ�1-����
						//	�ԣ��ر�/����ʱ��������0-�رգ�1-����
						//	�֣��ر�/�����ٶ�������0-�رգ�1-����
						//	�����ر�/������Χ������0-�رգ�1-����
						//	�ӣ��ر�/����״̬������0-�رգ�1-����

	BYTE	m_bytLstTimeHour; // �ϴη���ʱ��������ʱ��,��ʼ��Ϊ��Чֵ����0xff,
	BYTE	m_bytLstTimeMin;

	void Init()
	{
		memset(this, 0, sizeof(*this));
		m_bytLstTimeHour = m_bytLstTimeMin = 0xff;
		m_wTimeInterv = 0, m_wSpaceInterv = 0, m_bytCondStatus = 0x40;
	}
};

// Խ���������ã���ʵ��ʵʱ��λ�еķ�Χ����һ����
// ��Ϊ�˲���ʵʱ��λ��ͻ���˴�������һ�����壬���ڡ�Խ�籨����������ҵ��ҵ�����ͣ�0FH����
struct tag2QAreaCfg	
{	
	BYTE	m_bytAreaCount;
	BYTE	m_aryAreaProCode[6];	// ����Χ�ı�� 
	long	m_aryArea[6][4];		// ÿ����Χ������: ���¾��ȡ�����γ�ȡ����Ͼ��ȡ�����γ�� (����10��-6�η���)
	DWORD	m_aryAreaContCt[6];		// ���������ļ��� (Ԫ�ظ���������һ��)
	BYTE	m_aryReserve[3];		// ����3���ֽڣ����ڶ���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );
	}
};

struct tag2QGpsUpPar // �������ļ���
{
	WORD		m_wMonSpace; // ��ض������ ����λ��10�ף�
	WORD		m_wMonTime; // ���ʱ�� �����ӣ���0xffff��ʾ���ü��
	WORD		m_wMonInterval; // ��ؼ�� ���룩������Ϊ1
	BYTE		m_bytStaType; // ע��: ����Э�����״̬����
	BYTE		m_bytMonType; // Э���ж���ļ������

	void Init()
	{
		memset( this, 0, sizeof(*this) );
		m_wMonInterval = 0xffff;
		m_wMonSpace = 0xffff;
	}
};

struct tag2QGprsCfg	// 42
{
	tag2QGpsUpPar	m_aryGpsUpPar[4];
	BYTE		m_bytGprsOnlineType; // ���߷�ʽ
	BYTE		m_bytOnlineTip; // ���߷�ʽ��ʾ
	BYTE		m_bytGpsUpType; // GPS�����ϴ���ʶ 1,��������0154֡��2,����������
	BYTE		m_bytGpsUpParCount; // GPS���ݲ�������
	BYTE		m_bytChannelBkType_1; // ���ű�������1
		// 0	H1	H2	H3	H4	H5	H6	1
		// H1: ���, H2: ʵʱ��λ, H3: λ�ò�ѯ, H4: ����, H5: ����, H6: ����
	BYTE		m_bytChannelBkType_2; // ���ű�������2
		// 0	L1	L2	L3	L4	L5	L6	1
		// L1: ��ϻ��, L2: ˰�ش���

	BYTE		m_bytRptBeginHour;	// �����ϱ�ʱ���,����ʼʱ��==����ʱ��,���ʾ��ʱ�������
	BYTE		m_bytRptBeginMin;
	BYTE		m_bytRptEndHour;
	BYTE		m_bytRptEndMin;

	void Init()
	{
		memset( this, 0, sizeof(*this) );

		// �ر����ж��ű��ݣ����ű��ݹ������ڳ�����̶�����ע�������޸Ķ��ű��ݵ�ָ��Ҫ����
		m_bytChannelBkType_1 = m_bytChannelBkType_2 = 0;
		
		m_bytGpsUpType = 2;
		for( unsigned int ui = 0; ui < sizeof(m_aryGpsUpPar) / sizeof(m_aryGpsUpPar[0]); ui ++ )
			m_aryGpsUpPar[ui].Init();
	}
};

struct tag2QServCodeCfg
{
	char		m_szAreaCode[2]; // �����(����ID)
	char		m_szQianSmsTel[15]; // �����ط���(�����ط���),������0
	char		m_szTaxHandtel[15]; // ˰���ط���
	char		m_szDiaoduHandtel[15]; // �����ط���
	char		m_szHelpTel[15]; // ��������
	
	BYTE		m_bytListenTelCount;
	char		m_aryLsnHandtel[8][15]; // ����������������ĵ绰 (Э���ﶨ�������Ϊ5��)

	BYTE		m_bytCentServTelCount;
	char		m_aryCentServTel[5][15]; // ���ķ���绰

	BYTE		m_bytUdpAtvTelCount; // UDP����绰����
	char		m_aryUdpAtvTel[10][15];

	void Init()
	{
		memset( this, 0, sizeof(*this) );
		m_szAreaCode[0] = CENTID1;
		m_szAreaCode[1] = CENTID2;
		strncpy( m_szQianSmsTel, SMSSERVCODE, sizeof(m_szQianSmsTel) );
		m_szQianSmsTel[ sizeof(m_szQianSmsTel) - 1 ] = 0;
	}
};

struct tag2QListenCfg
{
	BYTE		m_bytListenTelCount;
	char		m_aryLsnHandtel[8][15]; // ����������������ĵ绰 (Э���ﶨ�������Ϊ5��)

	void Init()
	{
		memset( this, 0, sizeof(*this) );
	}
};

struct tag2QFbidTelCfg
{
	BYTE		m_bytFbidInTelCount;
	BYTE		m_bytAllowInTelCount;
	BYTE		m_bytFbidOutTelCount;
	BYTE		m_bytAllowOutTelCount;
	BYTE		m_bytFbidType; // ��Э����Ķ��巽ʽһ��
	char		m_aryFbidInHandtel[12][15]; // ��ֹ����ĵ绰 (Э���ﶨ�������Ϊ8��)
	char		m_aryAllowInHandtel[12][15]; // �������ĵ绰
	char		m_aryFbidOutHandtel[12][15]; // ��ֹ�����ĵ绰
	char		m_aryAllowOutHandtel[12][15]; // ��������ĵ绰
		// ��4λ,0�������к���,1��ֹ���к���,2�����ֺ������ֽ�ֹĳЩ����,3��ֹ���ֺ���,��������ĳЩ����
		// ��4λ,0�������к���,1��ֹ���к���,2�����ֺ��뵫�ֽ�ֹĳЩ����,3��ֹ���ֺ���,��������ĳЩ����

	void Init()
	{
		memset( this, 0, sizeof(*this) );
	}
};

struct tag2QReportPar
{
	char	m_szRID[20];
	bool	m_bEnable;
	byte	m_bytPriority;
	ushort	m_usCnt;
	ushort	m_usInterval;
	
	void Init(char *v_szRID, bool v_bEnable, byte v_bytPriority, ushort v_dwCnt, ushort v_dwInterval)
	{
		strncpy(m_szRID, v_szRID, sizeof(m_szRID)-1);
		m_bEnable = v_bEnable;
		m_bytPriority = v_bytPriority;
		m_usCnt = v_dwCnt;
		m_usInterval = v_dwInterval;
	}
};

struct tag2QReportCfg
{
	tag2QReportPar	m_objFootRptPar;
	tag2QReportPar	m_objDoorRptPar;
	tag2QReportPar	m_objPowOffRptPar;
	tag2QReportPar	m_objAccRptPar;
	tag2QReportPar	m_objOverSpdRptPar;
	tag2QReportPar	m_objBelowSpdPrtPar;
	tag2QReportPar	m_objMilePrtPar;
//	tag2QReportPar  m_objStopTimePrtPar;
	tag2QReportPar  m_objStartEngPrtPar;
	tag2QReportPar  m_objTirePrtPar;
	
	void Init()
	{
		m_objFootRptPar.Init("r001", true, 0x00, 5, 30);
		m_objDoorRptPar.Init("r001", false, 0x00, 1, 20);
		m_objPowOffRptPar.Init("r001", false, 0x00, 1, 20);
		m_objAccRptPar.Init("r001", true, 0x00, 1, 20);
		m_objOverSpdRptPar.Init("r001", false, 0x00, 1, 20);
		m_objBelowSpdPrtPar.Init("r001", false, 0x00, 1, 20);
		m_objMilePrtPar.Init("r001", false, 0x00, 1, 20);
//		m_objStopTimePrtPar.Init("r001", false, 0x00, 1, 20*1000);
		m_objStartEngPrtPar.Init("r001", false, 0x00, 1, 20);
	}
};

#define MAX_AUTORPT_CNT	20

struct tag2QAutoRptCfg
{
	bool	m_bNeedReport[MAX_AUTORPT_CNT];
	char	m_szId[MAX_AUTORPT_CNT][20];
	char	m_szBeginTime[MAX_AUTORPT_CNT][6];
	char	m_szIntervalType[MAX_AUTORPT_CNT];
	DWORD	m_dwLastRptValue[MAX_AUTORPT_CNT];
	DWORD	m_dwIntervalValue[MAX_AUTORPT_CNT];
	char	m_szEndType[MAX_AUTORPT_CNT];
	char	m_szEndTime[MAX_AUTORPT_CNT][6];
	DWORD	m_dwReportCnt[MAX_AUTORPT_CNT];
	DWORD	m_dwMaxCnt[MAX_AUTORPT_CNT];
	
	void Init()
	{
		memset(this, 0, sizeof(*this));
		
		for(int i=0; i<MAX_AUTORPT_CNT; i++)
		{
			m_bNeedReport[i] = false;
		}
	}
};

#define MAX_AREA_COUNT	8	// �����������10������10	
#define MAX_TIME_COUNT	6		// ÿ�����������������6��ʱ���

struct tag2QAreaRptPar
{
	bool	m_bEnable;							// �Ƿ�ʹ��
	char	m_szRID[20];
	ulong	m_ulIndex;
	byte	m_bytPriority;
	ushort  m_usCnt;
	ushort	m_usInterval;
	byte	m_bytAreaAttr;						// �������ԣ�01h--������02h--������03h--�����ڣ�04h--�����⡣
	bool	m_bTimeEnable[MAX_TIME_COUNT];		// ʱ����Ƿ���Ч�������������5��ʱ��Σ�
	char	m_szBeginTime[MAX_TIME_COUNT][3];	// ʱ��η�Χ
	char	m_szEndTime[MAX_TIME_COUNT][3];
	long	m_lMaxLat;							// ����Χ������ֻ֧�־�������
	long	m_lMinLat;
	long	m_lMaxLon;
	long	m_lMinLon;

	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct tag2QAreaRptCfg
{
	tag2QAreaRptPar	m_objArea[MAX_AREA_COUNT];
	tag2QAreaRptPar	m_objInArea[MAX_AREA_COUNT];
	tag2QAreaRptPar	m_objOutArea[MAX_AREA_COUNT];
	tag2QAreaRptPar	m_objInRegion[MAX_AREA_COUNT];
	tag2QAreaRptPar	m_objOutRegion[MAX_AREA_COUNT];
	
	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct tag2QTimeSpdRptPar
{
	bool	m_bEnable;							// �Ƿ�ʹ��
	char	m_szRID[20];
	ulong	m_ulIndex;
	byte	m_bytspd;
	byte	m_bytPriority;
	ushort	m_usTimeval;
	bool	m_bTimeEnable[MAX_TIME_COUNT];		// ʱ����Ƿ���Ч�������������5��ʱ��Σ�
	char	m_szBeginTime[MAX_TIME_COUNT][3];	// ʱ��η�Χ
	char	m_szEndTime[MAX_TIME_COUNT][3];

	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};
#define MAX_TIMESPD_COUNT 1
struct tag2QTimeSpdRptCfg
{
	tag2QTimeSpdRptPar	m_TimeSpd[MAX_TIMESPD_COUNT];

	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct tag2QAreaSpdRptPar
{
	bool	m_bEnable;							// �Ƿ�ʹ��
	char	m_szRID[20];
	ulong	m_ulIndex;
	byte	m_bytPriority;
	ushort  m_usCnt;
	ushort	m_usInterval;
	byte	m_bytAreaAttr;						// �������ԣ�01h--������02h--������03h--�����ڣ�04h--�����⡣
	byte 	m_bytSpeed;
	ushort 	m_usTimeval;											//
	bool	m_bTimeEnable[MAX_TIME_COUNT];		// ʱ����Ƿ���Ч�������������5��ʱ��Σ�
	byte	m_szBeginTime[MAX_TIME_COUNT][3];	// ʱ��η�Χ
	byte	m_szEndTime[MAX_TIME_COUNT][3];
	long	m_lMaxLat;							// ����Χ������ֻ֧�־�������
	long	m_lMinLat;
	long	m_lMaxLon;
	long	m_lMinLon;

	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct tag2QAreaSpdRptCfg
{
	tag2QAreaSpdRptPar	m_objAreaSpd[MAX_AREA_COUNT];
	tag2QAreaSpdRptPar	m_objInRegionSpd[MAX_AREA_COUNT];
	tag2QAreaSpdRptPar	m_objOutRegionSpd[MAX_AREA_COUNT];
	tag2QAreaSpdRptPar	m_TimeSpd[MAX_TIMESPD_COUNT];
	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct tag2QAreaParkTimeRptPar
{
	bool	m_bEnable;							// �Ƿ�ʹ��
	char	m_szRID[20];
	ulong	m_ulIndex;
	byte	m_bytPriority;
	ushort  m_usCnt;
	ushort	m_usInterval;
	byte	m_bytAreaAttr;						// �������ԣ�01h--������02h--������03h--�����ڣ�04h--�����⡣
	ushort 	m_usParkTime;											//
	bool	m_bTimeEnable[MAX_TIME_COUNT];		// ʱ����Ƿ���Ч�������������5��ʱ��Σ�
	char	m_szBeginTime[MAX_TIME_COUNT][3];	// ʱ��η�Χ
	char	m_szEndTime[MAX_TIME_COUNT][3];
	long	m_lMaxLat;							// ����Χ������ֻ֧�־�������
	long	m_lMinLat;
	long	m_lMaxLon;
	long	m_lMinLon;

	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct tag2QAreaParkTimeRptCfg
{
	tag2QAreaParkTimeRptPar	m_objAreaParkTime[MAX_AREA_COUNT];
	tag2QAreaParkTimeRptPar	m_objInRegionParkTime[MAX_AREA_COUNT];
	tag2QAreaParkTimeRptPar	m_objOutRegionParkTime[MAX_AREA_COUNT];
	tag2QAreaParkTimeRptPar	m_objParkTime;
	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

#define MAX_POINT_COUNT 100
#define MAX_LINE_COUNT  5
struct tag2QLineRptPar
{
	bool	m_bEnable;							// �Ƿ�ʹ��
	char	m_szRID[20];
	ulong	m_ulIndex;
	byte	m_bytPriority;
	ushort  m_usCnt;
	ushort	m_usInterval;
	ushort 	m_usOutval;											//ƫ����뵥λ��
	bool	m_bTimeEnable[MAX_TIME_COUNT];		// ʱ����Ƿ���Ч�������������5��ʱ��Σ�
	byte	m_szBeginTime[MAX_TIME_COUNT][3];	// ʱ��η�Χ
	byte	m_szEndTime[MAX_TIME_COUNT][3];
	bool	m_bPointEnable[MAX_POINT_COUNT];
	long	m_lLat[MAX_POINT_COUNT];							
	long	m_lLon[MAX_POINT_COUNT];


	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct tag2QLineRptCfg
{
	tag2QLineRptPar	m_objLine[MAX_LINE_COUNT];
	tag2QLineRptPar	m_objTimeLine[MAX_LINE_COUNT];

	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};

struct tag2QLNPhotoPar
{

	bool m_bEnable;
	char m_ExtID[20];
//	byte m_EventType;
	BYTE		m_bytSizeType; // ���շֱ���
	BYTE		m_bytChannel; // ͨ���ֽڵ���λ��ʾC4C3C2C1��"1"��Ч��"0"��Ч
	BYTE		m_bytQualityType; // �������� (��Э�鶨����ͬ)
	byte m_CmSta; //�ֽڸ���λC7C6��
                   //C7��0���رմ�����ץ��, 1������
                   //C6��0������Ҫ�����ϴ� 1����Ҫ
	ushort		m_usInterval; // ���ռ��,s
	byte	m_bytCTakeBeginTime[6]; //ץ����ʼ
	byte    m_bytCTakeEndTime[6];

	byte	m_bytTTakeBeginTime[6]; //ƣ����ʼ
	byte    m_bytTTakeEndTime[6];

	byte	m_bytTTakeBaseTime; //ƣ��ץ����ʼ
	byte    m_bytTTakeOfTime;


	BYTE		m_bytTime; // ���մ���
	BYTE		m_bytBright; // ����
	BYTE		m_bytContrast; // �Աȶ�
	BYTE		m_bytHue; // ɫ��
	BYTE		m_bytBlueSaturation; // ������
	BYTE		m_bytRedSaturation; // �챥��
	char		m_szReserve[7];

	void Init( BYTE v_bytTime, BYTE v_bytSizeType, BYTE v_bytQualityType, BYTE v_bytChannel, bool v_EnShot)
	{
		memset( this, 0, sizeof(*this) );
		m_bytBright = m_bytContrast = m_bytHue = m_bytBlueSaturation = m_bytRedSaturation = 8;
		m_bytTime = v_bytTime;
		m_bytSizeType = v_bytSizeType;
		m_bytQualityType = v_bytQualityType;
		m_bytChannel = v_bytChannel;
		m_bEnable = v_EnShot;
	}
};

struct tag2QLNPhotoCfg
{

	WORD		m_wPhotoEvtSymb; // ���մ���״̬�� (��QianExeDef.h�еĹ涨)
	WORD		m_wPhotoUploadSymb; // ���������ϴ�״̬�� (��QianExeDef.h�еĹ涨)

	tag2QLNPhotoPar	m_objAlermPhotoPar; // ������������
	tag2QLNPhotoPar	m_objAccPhotoPar; // ACC�仯����
	tag2QLNPhotoPar	m_objDoorPhotoPar; // ���س�������								  //									
	tag2QLNPhotoPar	m_objJijiaPhotoPar; // �ճ�/�س��仯ʱ����������
	tag2QLNPhotoPar	m_objOverTurnPhotoPar; // �෭��������
	tag2QLNPhotoPar	m_objTimerPhotoPar; // ��ʱ����
	tag2QLNPhotoPar	m_objTirePhotoPar; // ƣ�ͼ�ʻ����

	void Init()
	{
		memset( this, 0, sizeof(*this));
		m_wPhotoEvtSymb = PHOTOEVT_FOOTALERM | PHOTOEVT_OPENDOOR | PHOTOEVT_TIRE | PHOTOEVT_ACCON | PHOTOEVT_JIJIADOWN | PHOTOEVT_OVERTURNALERM | PHOTOEVT_TIMER;
//		m_wPhotoUploadSymb = PHOTOUPLOAD_FOOTALERM;
		m_objAlermPhotoPar.Init( 1, 3, 1, 0x0f , false); // v_bytTime, v_bytSizeType, v_bytQualityType, v_bytChannel
		m_objJijiaPhotoPar.Init( 1, 2, 2, 0x0f , false);
		m_objTirePhotoPar.Init( 1, 2, 2, 0x0f , false);
		m_objDoorPhotoPar.Init( 1, 2, 2, 0x0f , false);
		m_objTirePhotoPar.Init( 1, 2, 2, 0x0f , false);
		m_objOverTurnPhotoPar.Init( 1, 2, 2, 0x0f, false);
		m_objAccPhotoPar.Init( 1, 2, 2, 0x0f, false);
	}
};

struct tag2QcgvCfg
{
	char m_temiver[11];
	byte m_termiverlen;
	char m_companyid[10];
	byte m_companyidlen;
	char m_groupid[21];
	byte m_groupidlen;
	void Init()
	{
		m_termiverlen = strlen(SOFT_VERSION);
		memcpy(m_temiver, SOFT_VERSION, m_termiverlen);
		m_companyidlen = strlen(COMPANY_ID);
		memcpy(m_companyid, COMPANY_ID, m_companyidlen);
		m_groupidlen = strlen(GROUP_BILLING_ID);
		memcpy(m_groupid, GROUP_BILLING_ID, m_groupidlen);
	}
};

struct tag2QPhotoPar
{
	BYTE		m_bytTime; // ���մ���
	BYTE		m_bytSizeType; // ���շֱ��� (��Э�鶨�岻ͬ)
							// 1,��С
							// 2,�е�
							// 3,���
	BYTE		m_bytQualityType; // �������� (��Э�鶨����ͬ)
							// 0x01���㶨�����ȼ����ߣ�
							// 0x02���㶨�����ȼ����У���Ĭ�ϣ�
							// 0x03���㶨�����ȼ����ͣ�
	BYTE		m_bytChannel; // ͨ�� 0x01,ͨ��1; 0x02,ͨ��2; 0x03,ͨ��1��ͨ��2
	BYTE		m_bytInterval; // ���ռ��,s
	BYTE		m_bytBright; // ����
	BYTE		m_bytContrast; // �Աȶ�
	BYTE		m_bytHue; // ɫ��
	BYTE		m_bytBlueSaturation; // ������
	BYTE		m_bytRedSaturation; // �챥��
	BYTE		m_bytDoorPar; // �����ڹس�������,����λ��ʾ�ٶ����ƣ�0-15������/Сʱ��,����λ��ʾ���ٶȳ���ʱ�䣺0-15�����ӣ�
	//090611 hxd
	BYTE		m_bytIntervHour; // ���ռ����Сʱ��JG2��Сʱ�ͷ���ȫ0,��Ϊ����ʱ����
	BYTE		m_bytIntervMin; // ���ռ��,����(JG2)
	char		m_szReserve[7];

	void Init( BYTE v_bytTime, BYTE v_bytSizeType, BYTE v_bytQualityType, BYTE v_bytChannel )
	{
		memset( this, 0, sizeof(*this) );
		m_bytBright = m_bytContrast = m_bytHue = m_bytBlueSaturation = m_bytRedSaturation = 8;
		m_bytTime = v_bytTime;
		m_bytSizeType = v_bytSizeType;
		m_bytQualityType = v_bytQualityType;
		m_bytChannel = v_bytChannel;
	}
};

struct tag2QAlertCfg
{
	// ��������
	BYTE		m_bytAlert_1; // 0	S1	S2	S3	S4	S5	S6	1
		// JG2��JG3��VM1Э�飺
		// S1��0-�ر�Ƿѹ����            1-����Ƿѹ����
		// S2��0-�ر����ٱ���            1-�������ٱ���
		// S3��0-�ر��𶯱���            1-�����𶯱���
		// S4��0-�رշǷ��򿪳��ű���    1-����Ƿ��򿪳��ű���
		// S5��0-�رն�·����            1-�����·����
		// S6������

		// KJ2Э�飺
		// S1��0-�ر�Ƿѹ����            1-����Ƿѹ���� (Э����û��,����ΪKJ-01���Ա������д˹���,���Բ���)
		// S2��0-�ر����ٱ���            1-�������ٱ���
		// S3��0-�ر���ײ����            1-������ײ����
		// S4��0-�رղ෭����            1-����෭����
		// S5��0-�رն�·����            1-�����·���� (Э����û��,����ΪKJ-01���Ա������д˹���,���Բ���)

	BYTE		m_bytAlert_2; // 0	A1	A2	A3	A4	A5	A6	1
		// JG2��JG3��VM1Э�飺
		// A1��0-�ر�Խ�籨��            1-����Խ�籨��
		// A2��0-�رճ��ٱ���            1-�����ٱ���
		// A3��0-�ر�ͣ����������        1-����ͣ��ʱ���������
		// A4��0-�ر�ʱ��μ�鱨��      1-����ʱ��μ�鱨��
		// A5��0-�رյ��ٱ���            1-������ٱ���
		// A6, ��������

		// KJ2Э�飺
		// A5��0-�رշǷ���������        1-����Ƿ���������


	BYTE		m_bytAlermSpd; // ���ٱ����ٶ� (����/Сʱ)
	BYTE		m_bytSpdAlermPrid; // ���ٱ����ĳ��ٳ���ʱ�� (��)

#if USE_LIAONING_SANQI == 1
	BYTE		m_bytOverSpd;		// ���ٱ����ٶ� (����/Сʱ)
	DWORD		m_dwOverPrid;		// ���ٱ����ĳ��ٳ���ʱ�� (��)
	BYTE		m_bytBelowSpd;		// ���ٱ����ٶ� (����/Сʱ��
	DWORD		m_dwBelowPrid;		// ���ٱ����ĵ��ٳ���ʱ�䣨�룩
	//��𱨸�
	BYTE		m_bytAccSta;
	//��̱���
	ushort		m_usTotalMile;
	//ͣ������
//	ushort		m_usParkTime;
	//ƣ�ͼ�ʻ����
	ushort		m_usDriveTime;
	ushort		m_usRestTime;
	//�������
	ushort		m_usInterval;
	ushort		m_usTimes;
	ushort		m_usAlertDelay;
#endif

	void Init()
	{
		// Ĭ�ϱ���ȫ����
		m_bytAlert_1 = 0x7f;
		m_bytAlert_2 = 0x7f;

		// ���ٱ�������Ĭ����Ϊ������
		m_bytAlermSpd = 0xff;
		m_bytSpdAlermPrid = 0xff;

#if USE_LIAONING_SANQI == 1
		m_bytOverSpd = 0;
		m_dwOverPrid = 0;
		m_bytBelowSpd = 0;
		m_dwBelowPrid = 0;
		m_bytAccSta = 1;
		m_usTotalMile = 0;
		m_usDriveTime = 0;
		m_usRestTime = 0;
		m_usInterval = 60;
		m_usTimes = 10;
		m_usAlertDelay = 10;
//		m_usParkTime = 0;
#endif
	}
};

// ��ʻ��¼���������
// (����������Ϊ0, ��ʾ������; ����������Ϊ0,��ʾ������; �����ڴ�СΪ0,��ʾ����Ҫȷ�ϲ���Ҫ�ɿ�����)
struct tag2QDriveRecCfg
{
	WORD	m_wOnPeriod;	// ACC ON ����ʱ�� (����) (ffff��ʾ���÷���) (ǧ����Э��ʱʹ��)
	WORD	m_wOffPeriod;	// ACC OFF ����ʱ�� (����) (ffff��ʾ���÷���) (ǧ����Э��ʱʹ��)
	WORD	m_wOnSampCycle; // ACC ON ��������,��
	WORD	m_wOffSampCycle; // ACC OFF ��������,��
	BYTE	m_bytSndWinSize; // ���ʹ��ڴ�С
	BYTE	m_bytOnSndCycle; // ACC ON,��������,��
	BYTE	m_bytOffSndCycle; // ACC OFF,��������,��

	void Init()
	{
		memset( this, 0, sizeof(*this) );
#if USE_PROTOCOL == 30
		m_wOnPeriod = 0xffff;
		m_wOffPeriod = 0xffff;
		m_wOnSampCycle = 15;
		m_wOffSampCycle = 30;
		m_bytSndWinSize = 5;
		m_bytOnSndCycle = 2;
		m_bytOffSndCycle = 1;
#endif

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		m_wOnPeriod = 0xffff;
		m_wOffPeriod = 0xffff;
		m_wOnSampCycle = 60;
		m_wOffSampCycle = 1800;
		m_bytSndWinSize = 4;
		m_bytOnSndCycle = 1;
		m_bytOffSndCycle = 1;
#endif
	}
};

// �绰����Ŀ����
struct tag2QTelBookPar
{
	WORD	m_wLmtPeriod; // ͨ��ʱ��(����),0��ʾ����ʱ
	BYTE	m_bytType; // Ȩ������,ͬЭ�鶨��,��
		// 00H  ���������ͺ���
		// 01H  ��������룬�������
		// 02H  ������룬�������
		// 03H  �������ͺ���
	char	m_szTel[15]; // ���Ϊ��,��ʾ�����绰��δ����
	char	m_szName[20];
};

// �绰������
struct tag2QCallCfg
{
	BYTE	m_bytSetType; // ������
						// �ֽڶ������£�
						// 0	1	CL	CR	D	D	R	R
						// 
						// CL��1-������е绰�����ã� 0-����֮ǰ�绰������
						// DD�������绰����
						// 00���������к����绰
						// 01����ֹ���к����绰
						// 10���绰������
						// 11������
						// RR������绰����
						// 00���������к���绰
						// 01����ֹ���к���绰
						// 10���绰������ 
						// 11������
	tag2QTelBookPar	m_aryTelBook[30];

	void Init()
	{
		memset( this, 0, sizeof(*this) ); // �������к������
	}
};

struct tag2QCallListCfg
{
	byte	m_bytCtrl;
	int		m_iTelCnt;			// �������
	char	m_szTel[30][15];	// �绰����
	
	void Init()
	{
		memset( this, 0, sizeof(*this));
		m_bytCtrl = 0x02;		// Ĭ��Ϊ����������
	}
	
};


// ���ķ���绰��������
struct tag2QCentServTelCfg
{
	BYTE	m_bytTelCount;
	char	m_aryCentServTel[5][15];

	void Init() { memset( this, 0, sizeof(*this) ); }
};

// UDP����绰��������
struct tag2QUdpAtvTelCfg
{
	BYTE	m_bytTelCount;
	char	m_aryUdpAtvTel[10][15];

	void Init()
	{
		memset(this, 0, sizeof(*this));
		//strcpy( m_aryUdpAtvTel[0], "05922956773" );
	}
};

// ��ʻԱ��Ϣ (ǧ����ϵͳ)
struct tag2QDriverPar_QIAN
{
	WORD	m_wDriverNo; // ��� 0~126, 0��ʾ����ϢΪ�� (ʵ��ֻ��1���ֽ�)
	char	m_szIdentID[7]; // ��ݴ���,���㲹0,Ҳ����ȫ����
	char	m_szDrivingLic[18]; // ��ʻ֤��,���㲹0,Ҳ����ȫ����
};

// ��ʻԱ��Ϣ (2��ϵͳ)
struct tag2QDriverPar_KE
{
	WORD	m_wDriverNo; // ��� 0~65534, 0��ʾ����ϢΪ��
	char	m_szIdentID[8]; // ��ݴ���, �����0��Ҳ����ȫ����
};

// ��ʻԱ�������
struct tag2QDriverCfg
{
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
	tag2QDriverPar_QIAN	m_aryDriver[10];
#endif

#if USE_PROTOCOL == 30
	tag2QDriverPar_KE	m_aryDriver[10];
#endif

	void Init() { memset(this, 0, sizeof(*this)); }
};

// ʵʱָ��������Ŀ (�պ�4�ֽڶ���,�����׵���)
struct tag2QRealNavPar
{
	long		m_lCentLon;
	long		m_lCentLat;
	WORD		m_wRadius; // ��
	BYTE		m_bytSpeed; // �����ٶ�,����/Сʱ,0��ʾ������
	BYTE		m_bytDir; // ��,ʵ�ʶ���/3
	BYTE		m_bytType; // ����
		// 000TD000 (����Э��)
        //T��0--��ͨ����
        //   1--�������ͣ���ʾ�����⣩��Ҫ����ʵʱָ�������ģ�����ֻ�����ٶ����ƣ�
        //D��0--�����ж��г�����
        //   1--���ж��г�����
	char		m_szTip[51]; // ��ʾ����,������0,�ұض���NULL��β,��ΪЭ���ﶨ�����50�ֽ�,���ֶ����ñ�Э�����1���ֽ�
};


// ʵʱָ������
struct tag2QRealNavCfg
{
	tag2QRealNavPar		m_aryRealNavPar[50];

	void Init() { memset(this, 0, sizeof(*this)); }
};


// ��������
struct tag2QPhotoCfg
{
	WORD		m_wPhotoEvtSymb; // ���մ���״̬�� (��QianExeDef.h�еĹ涨)
	WORD		m_wPhotoUploadSymb; // ���������ϴ�״̬�� (��QianExeDef.h�еĹ涨)

	tag2QPhotoPar	m_objAlermPhotoPar; // ������������
	tag2QPhotoPar	m_objJijiaPhotoPar; // �ճ�/�س��仯ʱ����������
	tag2QPhotoPar	m_objHelpPhotoPar; // ������������
	tag2QPhotoPar	m_objDoorPhotoPar; // ���س�������
	tag2QPhotoPar	m_objAreaPhotoPar; // ������������
	tag2QPhotoPar	m_objTimerPhotoPar; // ��ʱ����
	tag2QPhotoPar	m_objAccPhotoPar; // ACC�仯����
	tag2QPhotoPar	m_objSpeedPhotoPar; // ����/��������

	void Init()
	{
		memset( this, 0, sizeof(*this));
		m_wPhotoEvtSymb = PHOTOEVT_FOOTALERM | PHOTOEVT_OPENDOOR | PHOTOEVT_AREA | PHOTOEVT_OVERSPEED | PHOTOEVT_BELOWSPEED;
		m_wPhotoUploadSymb = PHOTOUPLOAD_FOOTALERM | PHOTOUPLOAD_OPENDOOR | PHOTOUPLOAD_AREA | PHOTOUPLOAD_OVERSPEED | PHOTOUPLOAD_BELOWSPEED;
		m_objAlermPhotoPar.Init( 1, 2, 2, 0x0f ); // v_bytTime, v_bytSizeType, v_bytQualityType, v_bytChannel
		m_objJijiaPhotoPar.Init( 1, 2, 2, 0x0f );
		m_objHelpPhotoPar.Init( 1, 2, 2, 0x0f );
		m_objDoorPhotoPar.Init( 1, 2, 1, 0x0f );
		m_objAreaPhotoPar.Init( 1, 2, 2, 0x0f );
		m_objTimerPhotoPar.Init( 1, 2, 1, 0x0f );
		m_objAccPhotoPar.Init( 1, 2, 1, 0x0f );
		m_objSpeedPhotoPar.Init( 1, 2, 2, 0x0f );
	}
};

//LED���� 07.9.26 hong
struct tag2QLedCfg 
{
	unsigned short m_usLedParamID;  //LED����ID
	unsigned short m_usTURNID;		//LEDת����ϢID add by kzy 20090819
	unsigned short m_usNTICID;		//LED֪ͨ��ϢID add by kzy 20090819
	BYTE           m_bytLedPwrWaitTm; //LED�ϵ�ȴ�ʱ��
	BYTE           m_bytLedConfig;   //LED���ܶ��� 07.9.12 hong // 0 A1 A2 A3 A4 A5 A6 0
	//A6: 1: ��̨�ƶ���ֹʱ��LED���������ڿ��� 0�������п���
	//A5: 1:LED��ͨ�Ŵ���ʱ����ֹ���ع��     0��ͨ�Ŵ���ʱ��ֹ���ع��
	//A4��1������MD5                          0��������MD5
	unsigned short m_usLedBlkWaitTm; //������ֹ������ȴ�ʱ�䣬��λ����,Ĭ��Ϊ2����
	//#ifdef DELAYLEDTIME
	//	unsigned short m_usLedMenuID;   //��Ŀ��ID 07.9.30
	//#endif
	char			m_szLedVer[64]; // LED����汾��
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );
		
		m_usLedParamID = 0;
		m_bytLedPwrWaitTm = 5;
		m_bytLedConfig = 0;
		m_usLedBlkWaitTm = 60;
		m_usNTICID = 0;
		m_usTURNID = 0;
		
		//#ifdef DELAYLEDTIME
		//		m_usLedMenuID = 0;
		//#endif
	}
};


//�ֱ����� caoyi 07.11.29
struct tag2QHstCfg{
    char		m_bytcallring;
    char		m_bytsmring;
    bool		m_HandsfreeChannel;
    bool		m_EnableKeyPrompt;
    bool		m_EnableAutoPickup;
	//char		m_bytListenTel[3];	//�����绰�Ĵ���  //cyh add:del
	char		m_szListenTel[20];	//cyh add:add
	void Init()
	{
		memset( this, 0, sizeof(*this) );
		
		m_HandsfreeChannel = false;
		m_EnableKeyPrompt = true;
		m_EnableAutoPickup = true;
	}
};

//�ͺļ������� wxl 09.05.05
struct tag2QOilCfg
{
	BYTE	m_SetType;
	ushort	m_usRptPeriod;
	BYTE	m_bytPowerCyc; //��ʱ��������
	ushort	m_usADMin; //��СADֵ����
	ushort	m_usADMax;
	ushort	m_usACCONAddOilYUZHI;//ACCON������ֵ
	ushort	m_usACCONLeakOilYUZHI;
	ushort	m_usACCOFFAddOilYUZHI;
	ushort	m_usACCOFFLeakOilYUZHI;
	ushort	m_usTimeToCheckOil;
	BYTE	m_bytNumToCheckOil;//24Сʱ��©�ͼ�����

// 	void init()
// 	{
// 		memset(this, 0, sizeof(*this));
// 		m_usACCONLeakOilYUZHI = 250;
// 		m_usACCONLeakOilYUZHI = 250;
// 		m_usACCOFFAddOilYUZHI = 150;
// 		m_usACCOFFLeakOilYUZHI = 150;
// 		m_usTimeToCheckOil = 0x001E;
// 		m_bytNumToCheckOil = 5;
// 	}
};

#undef MAX_DRIVER
#define MAX_DRIVER 10

// ��ʻ����, 090817 xun move here
struct DrvCfg 
{
	byte inited;		//�Ƿ��ʼ�����ı�־
	
	ushort factor;		//��������ϵ��(ÿ�����������)
	char vin[17];		//����VIN�ţ�ռ��1��17���ֽ�
	char num[12];		//�������룺ռ��1��12���ֽ�
	char type[12];		//���Ʒ��ࣺռ��1��12���ֽ�	
	
	byte total;			//˾������
	byte max_tire;		//���õ����ƣ��ʱ��(Сʱ)
	byte min_sleep;		//���õ���С��Ϣʱ��(����)
	
	struct Driver 
	{		
		bool valid;			//��Ч��־λ
		byte num;			//˾����ţ�ȡֵ��Χ0~126
		char ascii[7];		//��ݴ���(1~7)
		char code[18];		//��ʻ֤��(1~18)
	} inf[MAX_DRIVER];
	
	byte max_speed;			//�����ٶ�(����/ʱ)
	byte min_over;			//���ټ���ʱ��(��)
	
	byte  min_speed;		//�����ٶ�(����/ʱ)
	short min_below;		//���ټ���ʱ��(����)
	
	byte winsize;		//���ڴ�С
	byte sta_id;		//״̬��ʶ
	ushort uptime[6];	//�ϱ���ʱ��
	ushort cltcyc[6];	//��������
	ushort sndcyc[6];	//��������
	
	void init() 
	{
		memset(this, 0, sizeof(*this));
		inited = 123; // ��ʼ����־���Ƚ����⣩
		winsize = 4;
		
		// 090229 xun: ����Ĭ�ϲ���
		uptime[0] = 0xffff; // �ƶ� (ʵ�ʴ���ʱ��ΪACC ON)
		cltcyc[0] = 60;
		sndcyc[0] = 1;
		uptime[1] = 0xffff; // ��ֹ (ʵ�ʴ���ʱ��ΪACC OFF)
		cltcyc[1] = 600;
		sndcyc[1] = 1;
		sta_id = 2; // 090817 xun add
		
		max_speed = 0;
		min_over = 0;
		min_speed = 255;
		min_below = 0;
	}
};

struct tagSpecCtrlCfg
{
	bool			m_bOilCut;

	void Init()
	{
		m_bOilCut = false;
	}
};

struct tag2QInvCarryCfg
{
	byte	m_bytCarryRpt;
	
	void Init()
	{
		m_bytCarryRpt = 0x01;//�ϱ�
	}
};

//zzg add
//������⳵������Ϣ
struct tagTaxiCfg
{
	byte m_aryDriverInf[3];//��ʻԱ�����Ϣ
	byte m_aryBespokeInf[2];//ԤԼ������Ϣ
	byte m_bytJiJiaEnable;//�Ƽ����Ƿ��� 0:���� 1:�ر�
	void Init()
	{
		memset( this, 0, sizeof(*this) );
		m_aryBespokeInf[0]=0xff;
		m_aryBespokeInf[1]=0xff;
	}
};

struct tag2QRptStationCfg
{
	byte	m_bytRptSta;	// �Ƿ����������Զ���վ
	int		m_iLineNo;		// ���õ���·��
	
	void Init()
	{
		m_iLineNo = 0;
		m_bytRptSta = 1;
	}
};

struct tag2QLineVersionCfg
{
	char strVer[6]; // ��λ:��
	
	void Init()
	{
		memset(this, 0, sizeof(*this));
	}
};


//--------------------����ΪDvrExe����--------------------//

//����
struct tag2DPwdPar
{
	char	AdminPassWord[11];
	char	UserPassWord[11];
	char	m_szReserved[26]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

	  strcpy(AdminPassWord, "4312896654");
	  strcpy(UserPassWord, "4312897765");
	}
};

//�豸��Ϣ
struct tag2DDevInfoPar
{
	char	DriverID[13]; 
	char	CarNumber[13];
	char	m_szReserved[22]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

	  strcpy(DriverID, "000000000000");
	  strcpy(CarNumber, "000000000000");
	}
};

//������Ϣ
struct tag2DIpPortPar
{
	char	m_szPhoneMod[20]; //�ֻ�ģ��
	char	m_szTelNum[16]; // ��̨�ֻ���,��NULL��β
	char	m_szApn[20]; // ��NULL��β
	unsigned long m_ulCommIP; //ͨ��IP
	unsigned long m_ulPhotoIP; //��ƬIP
	unsigned long m_ulVideoIP; //��ƵIP
	unsigned long m_ulUpdateIP; //����IP
	unsigned short m_usCommPort; //ͨ�Ŷ˿�
	unsigned short m_usPhotoPort; //��Ƭ�˿�
	unsigned short m_usVideoPort; //��Ƶ�˿�
	unsigned short m_usUpdatePort; //�����˿�

	char	m_szReserved[16]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���

	void Init()
	{
		memset( this, 0, sizeof(*this) );

		strcpy(m_szPhoneMod, "LC6311");  	strcpy(m_szTelNum, "");
		strncpy(m_szApn, APN, sizeof(m_szApn)-1);
		m_ulCommIP = inet_addr(QTCPIP);
	  m_ulPhotoIP = inet_addr(QUDPIP);
	  m_ulVideoIP = inet_addr(DUDPIP);
	  m_ulUpdateIP = inet_addr(LUDPIP);
	  m_usCommPort = htons(QTCPPORT);
	  m_usPhotoPort = htons(QUDPPORT);
	  m_usVideoPort = htons(DUDPPORT);
	  m_usUpdatePort = htons(LUDPPORT);
	}
};

//����Ƶ��������
struct tag2DTrigPar 
{
	UINT TrigType;
	char m_szReserved[20]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		TrigType = 1;//TRIG_EVE;
	}
};

//ʱ�δ�������
struct tag2DPerTrigPar
{
	BOOL	PeriodStart[6];
	char	StartTime[6][9];
	char	EndTime[6][9];
	char	m_szReserved[60]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		int i;
		for(i = 0; i < 6; i++)
		{
			PeriodStart[i] = FALSE;
			strcpy(StartTime[i], "00:00:00");
			strcpy(EndTime[i], "23:59:59");
		}
	}
};

//�¼���������
struct tag2DEveTrigPar
{
	BOOL	AccStart, MoveStart, OverlayStart, S1Start, S2Start, S3Start, S4Start;
	UINT	AccDelay, MoveDelay, OverlayDelay, S1Delay, S2Delay, S3Delay, S4Delay;
	char	m_szReserved[40]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		AccStart = TRUE;
		MoveStart = OverlayStart = S1Start = S2Start = S3Start = S4Start = FALSE;
		AccDelay = MoveDelay = OverlayDelay = S1Delay = S2Delay = S3Delay = S4Delay = 20;
	}
};

//��Ƶ�������
struct tag2DVIPar
{
	UINT	VMode;
	UINT	VNormal;	
	char	m_szReserved[16]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

#if VEHICLE_TYPE == VEHICLE_M
		VMode = 1;//CIF;
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		VMode = 4;//D1;
#endif
		VNormal = 0;//PAL;
	}
};

//��Ƶ�������
struct tag2DVOPar
{
	UINT  VMode;
	UINT  VNormal;
	char	m_szReserved[16]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		VMode = 1;//CIF;
		//VMode = 4;//D1;
		VNormal = 0;//PAL;
	}
};

//��ƵԤ������
struct tag2DVPrevPar
{
	BOOL  VStart[4];
	char	m_szReserved[16]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		int i;
		for(i = 0; i < 4; i++)
		{
			VStart[i] = TRUE;
		}
	}
};

//����Ƶ����¼�����
struct tag2DAVRecPar
{
#if VEHICLE_TYPE == VEHICLE_M
	BOOL  AStart[4];
	BOOL  VStart[4];
	UINT 	VMode;
	UINT 	VBitrate[4];
	UINT 	VFramerate[4];
	char  DiskType[10];
	char	m_szReserved[34]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		strcpy(DiskType, "HDisk");
		VMode = 1;//CIF;
		
		int i;
		for(i = 0; i < 4; i++)
		{
			AStart[i] = TRUE;
			VStart[i] = TRUE;
			VBitrate[i] = 409600;//BITRATE_REC_3;
			VFramerate[i] = 20;
		}
 		AStart[2] = FALSE;
 		VStart[2] = FALSE;
 		AStart[3] = FALSE;
 		VStart[3] = FALSE;
	}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	BOOL  AStart[4];
	BOOL  VStart[4];
	UINT 	VMode[4];
	UINT 	VBitrate[4];
	UINT 	VFramerate[4];
	char  DiskType[10];
	char	m_szReserved[22]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );
#if VEHICLE_TYPE == VEHICLE_V8
		strcpy(DiskType, "SDisk");
#endif
#if VEHICLE_TYPE == VEHICLE_M2
		strcpy(DiskType, "HDisk");
#endif
		
		int i;
		for(i = 0; i < 4; i++)
		{
			VMode[i] = 1;//CIF;
			AStart[i] = TRUE;
			VStart[i] = TRUE;
			VBitrate[i] = 173000;
			VFramerate[i] = 25;
		}
	}
#endif
};

//����Ƶ�����ϴ�����
struct tag2DAVUpdPar
{
	BOOL  AStart[4];
	BOOL  VStart[4];
	UINT 	VMode;
	UINT 	VBitrate[4];
	UINT 	VFramerate[4];
	int 	UploadType;
	int 	UploadSecond;
	char	m_szReserved[36]; // ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		VMode = 1;//CIF;
		UploadType = 0;
		UploadSecond = 300;
		
		int i;
		for(i = 0; i < 4; i++)
		{
			AStart[i] = FALSE;
			VStart[i] = FALSE;
			VBitrate[i] = 16000;//BITRATE_UPL_0;
			VFramerate[i] = 10;
		}
	}
};

//����Ƶ����طŲ���
struct tag2DAVPbPar
{
	BOOL  AStart[4];
	BOOL  VStart[4];
	UINT	DecNum;
	UINT  VFramerate[4];
	char	CreatDate[11];
	char	EncChn[4][2];
	char	AFilePath[4][50];
	char	VFilePath[4][50];
	char	IFilePath[4][50];
	byte	m_bytSysVolume;		// ϵͳ����
	char	m_szReserved[72];	// ����Ԥ���ռ�,��ʹ�������ṹ��ߴ�Ϊ8�ı���
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		int i;
		for(i = 0; i < 4; i++)
		{
			AStart[i] = FALSE;
			VStart[i] = FALSE;
			VFramerate[i] = 0;
			memset(EncChn[i], 0, sizeof(EncChn[i]));
			memset(AFilePath[i], 0, sizeof(AFilePath[i]));
			memset(VFilePath[i], 0, sizeof(VFilePath[i]));
			memset(IFilePath[i], 0, sizeof(IFilePath[i]));
		}
		DecNum = 1;
		memset(CreatDate, 0, sizeof(CreatDate));

		m_bytSysVolume = 15;
	}
};

//DVR���ò���
struct tag2DDvrCfg
{
	tag2DPwdPar	    PasswordPara;
	tag2DDevInfoPar	DeviceInfo;
	tag2DIpPortPar	IpPortPara;
	tag2DTrigPar		TrigPara;
	tag2DPerTrigPar	PeriodTrig;
	tag2DEveTrigPar EventTrig;
	tag2DVIPar			VInput;
	tag2DVOPar 			VOutput;
	tag2DVPrevPar		VPreview;
	tag2DAVRecPar		AVRecord;
	tag2DAVUpdPar		AVUpload;
	tag2DAVPbPar		AVPlayback;
	
	void Init()
	{
		memset( this, 0, sizeof(*this) );

		PasswordPara.Init();
		DeviceInfo.Init();
		IpPortPara.Init();
		TrigPara.Init();
		PeriodTrig.Init();
		EventTrig.Init();
		VInput.Init();
	 	VOutput.Init();
		VPreview.Init();
		AVRecord.Init();
		AVUpload.Init();
		AVPlayback.Init();
	}
};

//--------------------����ΪDvrExe����--------------------//


// ����20070914����,��Ϊ������Ӧ����ҵ����зֿ�,����ÿ���СӦ��80��������
struct tagSecondCfg// ע��ṹ���ڴ����
{
	char		m_szSpecCode[80];

	// ��ý������
	union
	{
		char		m_szReserved[80];
		tag2LMediaCfg m_obj2LMediaCfg;
	} m_uni2LMediaCfg;

	// ��ϻ�ӱ�������
	union
	{
		char		m_szReservered[80];
		tag2QBlkCfg	m_obj2QBlkCfg;
	} m_uni2QBlkCfg;

#if USE_LIAONING_SANQI == 1
	// �����ƶ����ڱ�������
	union
	{
		char		m_szReservered[480];
		tag2QReportCfg	m_obj2QReportCfg;
	}m_uni2QReportCfg;
	
	// �����ƶ����������ϱ�����
	union
	{
		char		m_szReservered[1200];
		tag2QAutoRptCfg	m_obj2QAutoRptCfg;
	}m_uni2QAutoRptCfg;

	// �����ƶ�������������
	union
	{
		char		m_szReservered[960];
		tag2QAreaRptCfg	m_obj2QAreaRptCfg;
	}m_uni2QAreaRptCfg;
	// �����ƶ�����ʱ����ٶ��쳣����  �ϲ��������ٶ��쳣��
//	union
//	{
//		char 		m_szReservered[80];
//		tag2QTimeSpdRptCfg m_obj2QTimeSpdRptCfg;
//	}m_uni2QTimeSpdRptCfg;

	// �����ƶ����������ٶ��쳣����
	union
	{
		char 		m_szReservered[960];
		tag2QAreaSpdRptCfg m_obj2QAreaSpdRptCfg;
	}m_uni2QAreaSpdRptCfg;

	// �����ƶ���������ͣ����ʱ
	union
	{
		char 		m_szReservered[480];
		tag2QAreaParkTimeRptCfg m_obj2QAreaParkTimeRptCfg;
	}m_uni2QAreaParkTimeRptCfg;
	 // �����ƶ�����ƫ������
	union
	{
		char 		m_szReservered[10720];
		tag2QLineRptCfg m_obj2QLineRptCfg;
	}m_uni2QLineRptCfg;
	//�����ƶ�������������
	union
	{
		char 		m_szReservered[480];
		tag2QLNPhotoCfg m_obj2QLNPhotoCfg;
	}m_uni2QLNPhotoCfg;

	//�����ƶ����ڳ���ID �ն˰汾 ���żƷѺ�
	union
	{
		char 		m_szReservered[80];
		tag2QcgvCfg m_obj2QcgvCfg;
	}m_uni2QcgvCfg;
#endif

	// ��������
	union
	{
		char		m_szReservered[80];
		tag2QAlertCfg	m_obj2QAlertCfg;
	} m_uni2QAlertCfg;

	// ʵʱ��λ��������
	union
	{
		char		m_szReservered[560];
		tag2QRealPosCfg	m_obj2QRealPosCfg;
	} m_uni2QRealPosCfg;

	// GPRS��������
	union
	{
		char		m_szReservered[160];
		tag2QGprsCfg	m_obj2QGprsCfg;
	} m_uni2QGprsCfg;

	// ���ķ����������
	union 
	{
		char		m_szReservered[560];
		tag2QServCodeCfg	m_obj2QServCodeCfg;
	} m_uni2QServCodeCfg;

	// ��������
	union
	{
		char		m_szReservered[480];
		tag2QPhotoCfg	m_obj2QPhotoCfg;
	} m_uni2QPhotoCfg;
	
	// LED����
	union
	{
		char		m_szReservered[160];
		tag2QLedCfg m_obj2QLedCfg;
	}m_uni2QLedCfg;
	
	//�ֱ�����
	union
	{
		char		m_szReservered[80];
		tag2QHstCfg	m_obj2QHstCfg;
	}m_uni2QHstCfg;

	// ���������������
	union
	{
		char		m_szReservered[880];
		tag2QFbidTelCfg m_obj2QFbidTelCfg;
	} m_uni2QFbidTelCfg;
	
	// �绰������
	union
	{
		char		m_szReservered[960];
		tag2QCallCfg	m_obj2QCallCfg;
	} m_uni2QCallCfg;

#if USE_LIAONING_SANQI == 1
	union
	{
		char		m_szReservered[480];
		tag2QCallListCfg m_obj2QCallListCfg;
	}m_uni2QCallListCfg;
#endif

	// ��ʻ��¼�ϱ�����
	union
	{
		char		m_szReservered[80];
		tag2QDriveRecCfg	m_obj2QDriveRecCfg;
	} m_uni2QDriveRecCfg;

	// ��ʻԱ�������
	union
	{
		char		m_szReservered[480];
		tag2QDriverCfg		m_obj2QDriverCfg;
	} m_uni2QDriverCfg;

	// ʵʱָ������
	union
	{
		char		m_szReservered[3200];
		tag2QRealNavCfg		m_obj2QRealNavCfg;
	} m_uni2QRealNavCfg;
	

	/// { JG2�ɵ���ʽ��¼��ģ������
	// �绰�� (JG2)
	union
	{
		char		m_szReservered[1600];	//�Ѿ�ʹ��1245
		
	} m_uniBookCfg;

// 	union
// 	{
// 		char		m_szReservered[160];
// 		tag2QOilCfg	m_obj2QOilCfg;
// 	}m_uni2QOilCfg;
	
	// �г���¼�� (JG2)
	union
	{
		char		m_szReservered[480];	//�Ѿ�ʹ��318	
	} m_uniDrvCfg;
	/// } JG2�ɵ���ʽ��¼��ģ������

	union
	{
		char		m_szReservered[80];
		tagSpecCtrlCfg m_obj2QSpecCtrlCfg;
	} m_uni2QSpecCtrlCfg;

	//�ؿͲ����
	union
	{
		char		m_szReservered[80];
		tag2QInvCarryCfg m_obj2QInvCarryCfg;
	} m_uni2QInvCarryCfg;

	// Dvr����
	union
	{
		tag2DDvrCfg m_obj2DDvrCfg;
		char		m_szReservered[1760]; //�Ѿ�ʹ��1530����
	}m_uni2DDvrCfg;
	
	// ���⳵��ʻԱ�����Ϣ
	union
	{
		char		m_szReservered[80];
		tagTaxiCfg m_obj2QTaxiCfg;
	} m_uni2QTaxiCfg;

	// Խ�籨������������Ϣ
	union
	{
		char		m_szReservered[80];
		tag2QAreaCfg m_obj2QAreaCfg;
	} m_uni2QAreaCfg;

	// �Զ���վ
	union
	{
		char		m_szReservered[80];
		tag2QRptStationCfg m_obj2QRptStationCfg;
	} m_uni2QRptStationCfg;

	union
	{
		char		m_szReserved[80];
		tag2QLineVersionCfg m_obj2QLineVersionCfg;
	} m_uni2QLineVersionCfg;

	void InitDefault()
	{
		memset(this, 0, sizeof(*this));
		strncpy( m_szSpecCode, CFGSPEC, sizeof(m_szSpecCode) - 1 );

		m_uni2LMediaCfg.m_obj2LMediaCfg.Init();
		m_uni2QBlkCfg.m_obj2QBlkCfg.Init();		
		m_uni2QAlertCfg.m_obj2QAlertCfg.Init();
		m_uni2QRealPosCfg.m_obj2QRealPosCfg.Init();
		m_uni2QGprsCfg.m_obj2QGprsCfg.Init();
		m_uni2QServCodeCfg.m_obj2QServCodeCfg.Init();
		m_uni2QPhotoCfg.m_obj2QPhotoCfg.Init();
		m_uni2QLedCfg.m_obj2QLedCfg.Init();
		m_uni2QHstCfg.m_obj2QHstCfg.Init();
		m_uni2QFbidTelCfg.m_obj2QFbidTelCfg.Init();
		m_uni2QCallCfg.m_obj2QCallCfg.Init();	
		m_uni2QDriveRecCfg.m_obj2QDriveRecCfg.Init();
		m_uni2QDriverCfg.m_obj2QDriverCfg.Init();
		m_uni2QRealNavCfg.m_obj2QRealNavCfg.Init();
		m_uni2QSpecCtrlCfg.m_obj2QSpecCtrlCfg.Init();
		m_uni2QInvCarryCfg.m_obj2QInvCarryCfg.Init();
		m_uni2QAreaCfg.m_obj2QAreaCfg.Init();
		m_uni2QRptStationCfg.m_obj2QRptStationCfg.Init();
		m_uni2QLineVersionCfg.m_obj2QLineVersionCfg.Init();

#if USE_LIAONING_SANQI == 1
		m_uni2QReportCfg.m_obj2QReportCfg.Init();
		m_uni2QAutoRptCfg.m_obj2QAutoRptCfg.Init();
		m_uni2QAreaRptCfg.m_obj2QAreaRptCfg.Init();
		m_uni2QCallListCfg.m_obj2QCallListCfg.Init();
//		m_uni2QTimeSpdRptCfg.m_obj2QTimeSpdRptCfg.Init();
		m_uni2QAreaSpdRptCfg.m_obj2QAreaSpdRptCfg.Init();
		m_uni2QAreaParkTimeRptCfg.m_obj2QAreaParkTimeRptCfg.Init();
		m_uni2QLineRptCfg.m_obj2QLineRptCfg.Init();
		m_uni2QcgvCfg.m_obj2QcgvCfg.Init();
		m_uni2QLNPhotoCfg.m_obj2QLNPhotoCfg.Init();
#endif
 		memset(m_uniBookCfg.m_szReservered, 0, sizeof(m_uniBookCfg.m_szReservered));
 		memset(m_uniDrvCfg.m_szReservered, 0, sizeof(m_uniDrvCfg.m_szReservered));

		// 090817 xun add
		DrvCfg objDrvCfg;
		objDrvCfg.init();
		memcpy( m_uniDrvCfg.m_szReservered, &objDrvCfg, sizeof(objDrvCfg) );

		m_uni2DDvrCfg.m_obj2DDvrCfg.Init();
		
		m_uni2QTaxiCfg.m_obj2QTaxiCfg.Init();//zzg add
	}
};

#pragma pack()

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

int DataPush( void* v_pPushData, DWORD v_dwPushLen, DWORD v_dwPushSymb, DWORD v_dwPopSymb, BYTE v_bytLvl ); // �˷�����ͬʱָ�����PopSymb,�����ն���
int DataPop( void* v_pPopData, DWORD v_dwPopSize, DWORD *v_p_dwPopLen, DWORD *v_p_dwPushSymb, DWORD v_dwPopSymb, BYTE *v_p_bytLvl );
int DataDel( DWORD v_dwPopSymb, void* v_pCmpDataFunc );
int DataWaitPop( DWORD v_dwPopSymb );
int DataSkipWait( DWORD v_dwPopSymb );

int GetImpCfg( void *v_pDes, unsigned int v_uiDesSize, unsigned int v_uiGetBegin, unsigned int v_uiGetSize );
int SetImpCfg( void *v_pSrc, unsigned int v_uiSaveBegin, unsigned int v_uiSaveSize );
int GetSecCfg( void *v_pDes, unsigned int v_uiDesSize, unsigned int v_uiGetBegin, unsigned int v_uiGetSize );
int SetSecCfg( void *v_pSrc, unsigned int v_uiSaveBegin, unsigned int v_uiSaveSize );
int ResumeCfg( bool v_bReservTel, bool v_bReservIP );

int IOGet( unsigned char v_ucIOSymb, unsigned char *v_p_ucIOSta );
int IOSet( unsigned char v_ucIOSymb, unsigned char v_ucIOSta,  void* v_pReserv, unsigned int v_uiReservSiz );
int IOCfgGet( unsigned char v_ucIOSymb, void *v_pCfg, DWORD v_dwCfgSize );
int IOIntInit( unsigned char v_ucIOSymb, void* v_pIntFunc ); // ��v_ucIOSymbֵΪ255,���ʾ����ȫ������IO���ж�
//int IOIntRelease();
//int IOIntFuncSet( unsigned char v_ucIOSymb, void* v_pIntFunc ); // ��v_ucIOSymbֵΪ255,���ʾ����ȫ��IO�Ļص�����

int GetCurGps( void* v_pGps, const unsigned int v_uiSize, BYTE v_bytGetType );
int SetCurGps( const void* v_pGps, BYTE v_bytSetType );

bool GetDevID( char* v_szDes, unsigned int v_uiDesSize );
bool GetAppVer( char* v_szDes, unsigned int v_uiDesSize );

int CrtVerFile();
int DelVerFile();
int ReadVerFile(char *v_pszVersion);
void ChkSoftVer(char *v_pszVersion);

int SetCurTime( void* v_pTmSet ); // �������Ӧ��tm���Ͷ����ָ�� (UTCʱ��)
unsigned int GetTickCount(); // ����ֵ: ��1970.1.1�賿������ʱ��,ms

void DogClr( DWORD v_dwSymb ); // ���������ö�Ӧ�Ŀ��Ź������־
DWORD DogQuery(); // ��ѯ���Ź�ֵ(��ѯ��λ���н��̿��Ź������־)
void DogInit(); // ��ʼ�����Ź�ֵ(����bit��1�Ա��һ�β�ѯ)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
