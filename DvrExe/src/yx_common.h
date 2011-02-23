#ifndef _YX_COMMON_H_
#define _YX_COMMON_H_ 

#define _REENTRANT

#include 	<time.h>
#include 	<stdio.h>
#include	<fcntl.h> 
#include	<errno.h>
#include  <netdb.h>
#include 	<stdlib.h>
#include	<string.h>
#include  <signal.h>
#include  <stdarg.h>
#include  <assert.h>
#include	<unistd.h>
#include	<dirent.h>
#include 	<getopt.h>
#include	<termios.h>
#include  <pthread.h>
#include  <arpa/inet.h>
#include  <netinet/in.h>
#include  <asm/page.h>  
#include  <sys/uio.h>
#include 	<sys/vfs.h>
#include  <sys/ipc.h> 
#include  <sys/shm.h>
#include  <sys/sem.h> 
#include	<sys/stat.h>
#include  <sys/mman.h>
#include  <sys/poll.h>
#include  <sys/time.h>
#include  <sys/wait.h>
#include 	<sys/mount.h>
#include	<sys/types.h>
#include  <sys/ioctl.h>
#include  <sys/select.h>
#include  <sys/socket.h>
#include  <linux/fs.h>
#include  <linux/fb.h>
#include  <linux/rtc.h>
#include  <linux/ioctl.h>
#include  <linux/types.h>

#include  "../../GlobalShare/typedef.h"
#include  "../../GlobalShare/GlobFunc.h"
#include  "../../GlobalShare/ComuServExport.h"
#include  "../../GlobalShare/InnerDataMng.h"
#include  "../../GlobalShare/TimerMng.h"
#include  "../../QianExe/yx_QianDefine.h"

#if VEHICLE_TYPE == VEHICLE_M
#include  "ivTTS.h"
#include  "yx_perspm.h"
#include  "yx_tw2835.h"
#include  "hi_common_api.h"
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
#include  "ivTTS.h"
#include  "yx_perspm.h"
#include  "yx_tw2865.h"
#include  "hi_common_api.h"
#include  "usb-statue.h"
#include  "sd_mmc_const.h"
#endif

#undef MSG_HEAD
#define MSG_HEAD ("DvrExe")

#define MSG_HHQ		0

#define NO_VENC		1
#define NO_PREV		1
#define NO_INFO	  1

#define msleep(n) usleep(n*1000)

#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))

//�����¼�����
#define EVENT_TYPE_ERROR			0x00000001  //�쳣
#define EVENT_TYPE_POWOFF			0x00000002  //�ϵ�
#define EVENT_TYPE_ALERT			0x00000004  //����
#define EVENT_TYPE_HELP				0x00000008  //����
#define EVENT_TYPE_PERIOD			0x00000010  //ʱ��
#define EVENT_TYPE_CENT				0x00000020  //����
#define EVENT_TYPE_ACCON			0x00000040	//���
#define EVENT_TYPE_IRDA				0x00000080  //ң��
#define EVENT_TYPE_MOVE				0x00000100  //�ƶ�
#define EVENT_TYPE_OVERLAY		0x00000200  //�ڵ�
#define EVENT_TYPE_TIMEOUT		0x00000400  //��ʱ
#define EVENT_TYPE_ALERT2			0x00000800  //����
#define EVENT_TYPE_ALERT3			0x00001000  //����
#define EVENT_TYPE_ALERT4			0x00002000  //����

////////////////////////////////////////������������/////////////////////////////////////////

extern	CTimerMng g_objTimerMng;

enum DVRTIMERS
{
	TIMER_DIAODU_TIP = 1,
	DVRTCP_LOG_TIMER,
	DVRTCP_RECONN_TIMER,
	A1_REUPLOAD_TIMER,
	C1C3_REUPLOAD_TIMER
};

extern bool g_bIrTest;
extern volatile bool g_bProgExit;
extern int g_iTempState;

/*��Ƶ��ʽ*/
enum{PAL, NTSC};
/*��Ƶ��ʽ*/
enum{CIF=1, QCIF, HD1, D1};

/*�洢��״̬*/
enum
{
	UCON_UMOU_UVAL,//δ���ӡ�δ���ء��޿��ÿռ�
	CON_UMOU_UVAL,//�����ӡ�δ���ء��޿��ÿռ�
	CON_MOU_UVAL,//�����ӡ��ѹ��ء��޿��ÿռ�
	CON_MOU_VAL//�����ӡ��ѹ��ء��п��ÿռ�
};

/*����Ƶ������ʽ*/
enum
{
	TRIG_PER,//ʱ�δ���
	TRIG_EVE,//�¼�����
	TRIG_PER_OR_EVE,//ʱ�λ��¼�����
	TRIG_PER_AND_EVE//ʱ�����¼�����
};

/*��Ƶ¼����*/
enum 
{
	BITRATE_REC_1 = 173000,//262144,
	BITRATE_REC_2 = 327680,
	BITRATE_REC_3 = 409600,
	BITRATE_REC_4 = 512000,
	BITRATE_REC_5 = 640000,
	BITRATE_REC_6 = 800000,
	BITRATE_REC_7 = 1000000,
	BITRATE_REC_8 = 1250000,
	BITRATE_REC_9 = 1562500
};

/*��Ƶ�ϴ�����*/
enum
{
	BITRATE_UPL_0 = 16000,
	BITRATE_UPL_1 = 35500,
	BITRATE_UPL_2 = 63000,
	BITRATE_UPL_3 = 90500,
	BITRATE_UPL_4 = 118000,
	BITRATE_UPL_5 = 145500,
	BITRATE_UPL_6 = 173000,
	BITRATE_UPL_7 = 200000
};

enum{ YEAR, MON, DAY, HOUR, MIN, SEC };
enum{ SDISK, HDISK, UDISK };
enum{ PART1=0, PART2, PART3 };
enum{ TOTAL, AVAIL };
enum{ UP, DOWN, LEFT, RIGHT, ENTER, CANCEL };

/*�ļ���ѯ����*/
typedef struct yx_FILE_SEARCH
{
	BOOL  SearchStart[4];
	char	DiskType[10];
	char	CreatDate[3][5];
	char	StartTime[9];
	char	EndTime[9];
}FILE_SEARCH;

typedef struct yx_QianGps
{
	BYTE	m_bytGpsType;
	BYTE	m_bytYear;	
	BYTE	m_bytMon;
	BYTE	m_bytDay;
	BYTE	m_bytHour;
	BYTE	m_bytMin;
	BYTE	m_bytSec;
	BYTE	m_bytMix;	
	BYTE	m_bytLatDu;
	BYTE	m_bytLatFenzen;
	BYTE	m_bytLatFenxiao1;
	BYTE	m_bytLatFenxiao2;
	BYTE	m_bytLonDu;
	BYTE	m_bytLonFenzen;
	BYTE	m_bytLonFenxiao1;
	BYTE	m_bytLonFenxiao2;
	BYTE	m_bytSpeed; 
	BYTE	m_bytDir;
	BYTE	m_bytChk;
}QianGps;

/*����Ƶ�����ļ��ڵ�*/
typedef struct yx_FRAME_INDEX
{
	char		CurTime[9];
	HI_U64 	AStartOffset;
	HI_U64 	AEndOffset;
	HI_U64 	VStartOffset;
	HI_U64 	VEndOffset;
	BOOL	IFrameFlag;
}FRAME_INDEX;
/*����Ƶ�б��ļ��ڵ�*/
typedef struct yx_FILE_INDEX
{
	char	EncChn[2];

	char	CreatDate[3][5];
	char	StartTime[3][3];
	char	EndTime[3][3];

	char  Mode[5];
	char	Normal[5];

	char  Bitrate[8];
	char  Framerate[3];
	
	char  ASize[10];
	char  VSize[10];
	char	APath[50];
	char	VPath[50];
	char	IPath[50];
}FILE_INDEX;

/*���ղ˵����������Ϣ*/
typedef struct yx_MENU_OSD
{
	/*��½*/
	char				LoginPassword[11]; 
	/*�ļ���ѯ��¼������*/  
	char				CreatDate[3][5];
	char				StartTime[3][3];
	char				EndTime[3][3];
	char			  EncChn;
	/*���̲���*/
	char				DiskType[10];
	/*������������*/
	char				SystemDate[3][5];
	char				SystemTime[3][3];
	char				DriverID[13]; 
	char				CarNumber[13];
	/*¼���������*/ 
	uint			  TrigType;
	
	uint			  ViNormal;	
	uint			  VRecMode[4];
	uint			  VUplMode[4];
	uint  		  VRecBps[4];
	uint  		  VUplBps[4];
	char  			VRecFps[4][3];
	char  			VUplFps[4][3];
	BOOL			  APrevEn[4];
	BOOL 		    ARecEn[4];
	BOOL 		    AUplEn[4];
	BOOL			  VPrevEn[4];
	BOOL 		    VRecEn[4];
	BOOL 	 	    VUplEn[4];
	
	char				PeriodSHour[6][3];
	char				PeriodSMin[6][3];
	char				PeriodSSec[6][3];
	char				PeriodEHour[6][3];
	char				PeriodEMin[6][3];
	char				PeriodESec[6][3];
	BOOL 		    PeriodEn[6];
	char				AccDelay[3];
	BOOL			  AccEn;
	char				MoveDelay[3];
	BOOL			  MoveEn;
	char				OverlayDelay[3];
	BOOL			  OverlayEn;
	char				S1Delay[3];
	BOOL			  S1En;
	char				S2Delay[3];
	BOOL			  S2En;
	char				S3Delay[3];
	BOOL			  S3En;
	char				S4Delay[3];
	BOOL			  S4En;
	/*��������*/
	char	      m_szPhoneMod[20];
	char	      m_szTelNum[16];
	char	      m_szApn[20];
	char	      m_ulCommIP[20]; 
	char	      m_ulPhotoIP[20]; 
	char	      m_ulVideoIP[20];
	char	      m_ulUpdateIP[20];
	char	      m_usCommPort[20];
	char	      m_usPhotoPort[20];
	char	      m_usVideoPort[20];
	char	      m_usUpdatePort[20];
	
	/*��������*/
	char				CurPassword[11]; 
	char				NewPassword[2][11];  
	/*��ѯ���������*/	
	BOOL			  FileFix[7];
	int			    FileNum;						//��ѯ������������Ƶ�ļ�����
	int			    FilePageNum;				//��ѯ������������Ƶ�ļ�ҳ��
	int			    FilePageIndex;
	int			    FileIndex[7];
	FILE_INDEX	*AVFileIndex;
}MENU_OSD; 

/*�豸������*/
typedef struct yx_DEVICE_FD
{
	FILE   	*ASC;
	FILE   	*HZK;
	
	int	Wdg;
	int	Usb;
	int	Irda;
	int	Tw2835;
	int	Tw2865;
	int	Perspm;
}DEVICE_FD;

/*USB�豸״̬*/
typedef struct yx_USB_STATE
{
	BYTE	SDisk[2];
	BYTE	HDisk[2];
	BYTE	UDisk[2];
}USB_STATE;

/*USB�豸ж��*/
typedef struct yx_USB_UMOUNT
{
	BOOL	SDisk[2];
	BOOL	HDisk[2];
	BOOL	UDisk[2];
}USB_UMOUNT;

/*ϵͳ״̬*/
typedef enum yx_SYSTEM_STATE
{
	SLEEP, 
	IDLE, 
	WORK, 
	CTRL
}SYSTEM_STATE;

enum{ WORK_REC, WORK_UPL, WORK_PIC, WORK_TTS, WORK_BLK };

/*�豸����״̬*/
typedef struct yx_WORK_START
{
	BOOL ARecord[4];
	BOOL VRecord[4];
	BOOL AUpload[4];
	BOOL VUpload[4];
	BOOL Picture;
	BOOL TTS;
	BOOL Black;
}WORK_START;

typedef struct yx_IDLE_START
{
	BOOL APreview;
	BOOL VPreview[4];
}IDLE_START;

typedef struct yx_CTRL_START
{
	BOOL APlayback[4];
	BOOL VPlayback[4];
}CTRL_START;

/*ֹͣ�豸����*/
typedef struct yx_WORK_STOP
{
	BOOL AVRecord;
	BOOL AVUpload;
	BOOL Picture;
	BOOL TTS;
	BOOL Black;
}WORK_STOP;

typedef struct yx_IDLE_STOP
{
	BOOL APreview;
	BOOL VPreview;
}IDLE_STOP;

typedef struct yx_CTRL_STOP
{
	BOOL APlayback;
	BOOL VPlayback;
	BOOL AVPlayback;
}CTRL_STOP;

/*оƬ״̬*/
typedef struct yx_DEVICE_STATE
{
	BOOL	Camera[4];
}DEVICE_STATE;

/*ʡ��״̬�����¼�*/
typedef struct yx_SLEEP_EVENT
{
	DWORD dwStartEvt;
	DWORD dwStopEvt;
}SLEEP_EVENT;

/*����״̬�����¼�*/
typedef struct yx_CTRL_EVENT
{
	DWORD dwStartEvt;
	DWORD dwStopEvt;
}CTRL_EVENT;

/*����״̬�����¼�*/
typedef struct yx_WORK_EVENT
{
	DWORD dwRecStartEvt;
	DWORD dwUplStartEvt;
	DWORD dwPicStartEvt;
	DWORD dwTtsStartEvt;
	DWORD dwBlkStartEvt;
	
	DWORD dwRecStopEvt;
	DWORD dwUplStopEvt;
	DWORD dwPicStopEvt;
	DWORD dwTtsStopEvt;
	DWORD dwBlkStopEvt;
}WORK_EVENT;

/*�ط�����*/
typedef enum yx_PLAYBACK_FOLD
{
	MM1, MM2, MM4, MM8, //���˱���
 	MP1, MP2, MP4, MP8,	//�������
 	DP1,								//��������
 	DP2, DP4, DP8,			//���ű���
}PLAYBACK_FOLD;

/*�طſ���*/
typedef	struct yx_PLAYBACK_CTRL
{
	BOOL VSlow;//����
	BOOL VFrame;//֡��
	BOOL APause;//��Ƶ��ͣ
	BOOL AVPause;//����Ƶ��ͣ
}PLAYBACK_CTRL;

typedef	struct yx_MEM_BUF
{
	BYTE *Buf;
	BYTE *Head;// ͷָ��
	BYTE *Tail;// βָ��
	BYTE *Read;// ��ָ��
	BYTE *Write;// дָ��
	DWORD BufSize;// �������ܳ���
	BOOL SameCircle;// ��ָ���дָ���Ƿ���ͬһȦ��
	pthread_mutex_t Mutex;	
}MEM_BUF;

/*�����ϴ���ز���*/
typedef	struct yx_AV_UPLOAD_BUF
{
	pthread_mutex_t	AVMutex;
	
	BOOL VUpload;
	MEM_BUF VMemBuf[4];
	
	BOOL AUpload;
	MEM_BUF AMemBuf[4];
}AV_UPLOAD_BUF;

/*���ݴ洢��ز���*/
typedef	struct yx_AV_RECORD_BUF
{
	BYTE *VBuf[4];
	BYTE *ABuf[4];
}AV_RECORD_BUF;

/*�����ϳ���ز���*/
typedef	struct yx_TTS_PLAY_BUF
{
	BOOL	TStop;
	MEM_BUF TMemBuf;
}TTS_PLAY_BUF;


#include "yx_osd.h" 
#include "yx_tts.h"
#include "yx_qian.h"
#include "yx_irda.h" 
#include "yx_menu.h"
#include "yx_disk.h"
#include "yx_black.h"
#include "yx_black_new.h"
#include "yx_photo.h"
#include "yx_encode.h"
#include "yx_decode.h"
#include "yx_config.h"
#include "yx_upload.h"
#include "yx_monitor.h"
#include "yx_Sock.h"


/////////////////////////////////////ȫ�ֱ������� ///////////////////////////////////////////
extern char szATest[4][255];
extern char szVTest[4][255];
extern char szITest[4][255];

extern unsigned char 	*g_pMenuPage;
extern tag2DDvrCfg 		g_objMvrCfg;
extern DEVICE_FD 		g_objDevFd;
extern USB_STATE 		g_objUsbSta;
extern USB_UMOUNT 		g_objUsbUmount;
extern SYSTEM_STATE		g_enumSysSta;
extern WORK_START		g_objWorkStart;
extern IDLE_START		g_objIdleStart;
extern CTRL_START		g_objCtrlStart;
extern WORK_STOP    	g_objWorkStop;
extern IDLE_STOP    	g_objIdleStop;
extern CTRL_STOP    	g_objCtrlStop;
extern DEVICE_STATE		g_objDevSta;
extern PLAYBACK_FOLD	g_objPlaybackFold;
extern PLAYBACK_CTRL  	g_objPlaybackCtrl;
extern QianGps			g_objQianGps;
extern AV_UPLOAD_BUF	g_objAVUplBuf;
extern AV_RECORD_BUF	g_objAVRecBuf;
extern BYTE 			g_bytRecRptBuf[100];
extern BYTE 			g_bytUplRptBuf[100];
extern TTS_PLAY_BUF     g_objTTSPlayBuf;

extern pthread_mutex_t  g_AVListMutex;

extern CInnerDataMng	g_objIoMng;
extern CInnerDataMng	g_objSockMng;
extern CInnerDataMng	g_objQianMng;
extern CInnerDataMng	g_objQianTTSMng;
extern CInnerDataMng	g_objQianPicMng;
extern CInnerDataMng	g_objQianIrdaMng;

extern CInnerDataMng	g_objDevErrMng;

extern CInnerDataMng	g_objVideoDataMng[4];
extern CInnerDataMng	g_objAudioDataMng[4];

extern CPhoto	g_objPhoto;
extern CSock	g_objSock;

extern bool	g_bUploadOnlyISlice;

bool SetTimeByRtc();
int GetCurDateTime( int v_iCDateTime[] );
int SetSysVolume(bool bSet, char szSet);

int MakeUsbFrame(BYTE *InBuf, int InLen, BYTE *OutBuf);
BOOL InitBuf(int i, int size, BYTE data, BYTE type='R');
DWORD ReadBuf(int i, BYTE *buf, DWORD len, BYTE type);
BOOL WriteBuf(int i, BYTE *buf, DWORD len, BYTE type);

int SecondCount( int v_iLastTime, int v_iDelayTime );

void ShowDiaodu(char *fmt, ...);
void ShowDiaoduForScreens(char *fmt, ...);

void ClearThreadDog(int v_iThreadId);
void RenewMemInfoHalfMin(int v_iId);

bool FileWritable(char *v_szFile, DWORD &v_dwTick);

#endif

