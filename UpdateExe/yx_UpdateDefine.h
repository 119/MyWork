#ifndef _YX_UPDATEDEFINE_H_
#define _YX_UPDATEDEFINE_H_

#if USE_LANGUAGE == 0
#define LANG_NK_OL_UPGRADING ("�ں�����������,�뱣�ֹ����������ĵȺ")
#define LANG_NK_OL_UPGRAD_FILE_LENERR ("�ں���������,�ļ����ȴ���!")
#define LANG_NK_OL_UPGRAD_FILE_NOFIND ("�ں���������,�ļ�û���ҵ�!")
#define LANG_NK_OL_UPGRAD_FILE_OPENFAIL ("�ں���������,�ļ��򿪴���!")
#define LANG_NK_OL_UPGRAD_FILE_READFAIL ("�ں���������,�ļ���ȡ����!")
#define LANG_NK_OL_UPGRAD_TOCREADFAIL ("�ں���������,TOC��ȡʧ��!")
#define LANG_NK_OL_UPGRAD_TOCWRITEFAIL ("�ں���������,TOCд��ʧ��!")
#define LANG_CHKING_U ("���ڼ��U��...")
#define LANG_CHKU_FAIL ("U�̼��ʧ��!")
#define LANG_VIDEOFILE_INPUTTING ("��Ƶ�ļ�������,���Ժ�...")
#define LANG_VIDEOFILE_INPUTSUCC ("��Ƶ�ļ�����ɹ�!ϵͳ��������..")
#define LANG_U_FILEERR ("U�������ļ�ȱʧ����!���������!")
#define LANG_U_VERERR ("U�����ݰ汾����!���������!")
#define LANG_NK_UPGRADING ("�ں�������,�뱣�ֹ����������ĵȺ")
#define LANG_VIDEOFILE_ERR ("U����Ƶ�����ļ��޷�ʶ��!")
#define LANG_NOVIDEO_FUNC ("�ð汾����Ƶ���Թ���!")
#define LANG_EXITPLAYMODE ("�����˳���ǰ����״̬!")
#define LANG_MEDIA_LOGGING ("��ý���½��...")
#define LANG_VOLUME ("��ǰ����")
#define LANG_LED_RESET ("������дLED��,���Ժ�..")
#define LANG_ACC_NEEDVALID ("ACC��Ч!�뽫ACC����Ϊ��Ч!")
#define LANG_ALA1_TEST ("LED���ڲ���")
#define LANG_SUCC ("�ɹ�")
#define LANG_FAIL ("ʧ��")
#endif

#if USE_LANGUAGE == 11
#define LANG_NK_OL_UPGRADING ("Maintain the power,Sys Online Upgrading...")
#define LANG_NK_OL_UPGRAD_FILE_LENERR ("Sys Online Upgrade, File Len Err!")
#define LANG_NK_OL_UPGRAD_FILE_NOFIND ("Sys Online Upgrade, File Find Failure!")
#define LANG_NK_OL_UPGRAD_FILE_OPENFAIL ("Sys Online Upgrade, File Open Failure!")
#define LANG_NK_OL_UPGRAD_FILE_READFAIL ("Sys Online Upgrade, File Read Failure!")
#define LANG_NK_OL_UPGRAD_TOCREADFAIL ("Sys Online Upgrade,Toc Read Fail!")
#define LANG_NK_OL_UPGRAD_TOCWRITEFAIL ("Sys Online Upgrade,Toc Write Fail!")
#define LANG_CHKING_U ("U Disk Searching...")
#define LANG_CHKU_FAIL ("U Disk Search Failure!")
#define LANG_VIDEOFILE_INPUTTING ("Video Files Inputting...")
#define LANG_VIDEOFILE_INPUTSUCC ("Video Files Input Succ, Resetting...")
#define LANG_U_FILEERR ("U Disk Files Damaged!")
#define LANG_U_VERERR ("U Disk Files Ver Err!")
#define LANG_NK_UPGRADING ("Maintain the power supply, Sys Upgrading...")
#define LANG_VIDEOFILE_ERR ("Vedio Files Err!")
#define LANG_NOVIDEO_FUNC ("Not Video Files Inputting Function!")
#define LANG_EXITPLAYMODE ("Please Quit Play State First!")
#define LANG_MEDIA_LOGGING ("Media Center Logging in...")
#define LANG_VOLUME ("Current Volume")
#define LANG_LED_RESET ("Startting Led...")
#define LANG_ACC_NEEDVALID ("ACC Invalid! Make ACC Valid First")
#define LANG_ALA1_TEST ("ALA1 Port Test")
#define LANG_SUCC ("Succ")
#define LANG_FAIL ("Fail")
#endif

static byte CrcTable[256] = {
	0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75,  0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B,
	0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69,  0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67,
	0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D,  0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43,
	0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51,  0x2A, 0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F,
		
	0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05,  0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B,
	0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19,  0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17,
	0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D,  0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33,
	0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21,  0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F,
		
	0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95,  0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B,
	0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89,  0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87,
	0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD,  0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3,
	0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1,  0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF,
		
	0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5,  0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB,
	0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9,  0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7,
	0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD,  0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3,
	0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1,  0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF
};

enum TIMERS
{
	RETRAN_TIMER = 1,
	UPDATE_TCP_LOG_TIMER
};

const DWORD	RETRAN_INTERVAL	= 3000;		// �ش����
const DWORD WAIT_ANS_TIME = 10000;		// �ȴ�����Ӧ��ʱ��

enum UPDATEMSG_TYPE
{ 
	MSG_SOCK_SEND = 1,
	MSG_PROGQUIT
};

typedef struct tagMsg
{
	long int m_lMsgType;
	unsigned char m_uszMsgBuf[MSG_MAXLEN];
	
	tagMsg() { memset( this ,0, sizeof(*this) ); }
}UPDATEMSG;

enum DOWNSTA{
	Idlesse = 1,
	NetWick,
	Logining,
	ChkDiskExp,	
	ReqTask,
	DispTask,
	ReqAttr,
	ChkFree,
	ReqDown,
	WaitData,	
	RcvData,
	ChkBuf,	
	ChkMd5,		
	TranResult,
	SoftUpdate,
	LineFile
};

enum FILE_TYPE
{
	SOFT,
	ROOT,
	KERN,
	BOOT,
	LINE
};

#define FILE_TYPE_COUNT		5

//Э��汾��
#define PROTOCOL_VER	0x09060800		// �޶������ն˰汾�ŵ�Э��(1281)����չ�˰汾���ֶγ���,ȥ��������ID

#define LOGIN_INTERVAL	30*60*1000		// ��̨�����½ʱ��
#define MAX_RCVTIMEOUT	60*1000			// ���ս�Ŀ���ݵ����ʱ
#define MAX_WAITFINE	5*60*1000		// �ȴ�����ָ����ʱ��
#define MAX_RELOGIN		3				// ���µ�½��������

//#pragma pack(push,1)//���ֽ��Ͷ��뷽ʽ

//-----------------------------------------------------------------------
// ��̨��½����
struct __attribute__(packed,aligned (1)) Frm0181 {
	byte	m_bytTel[6];	//��½�ֻ���
	byte	m_bytDate[6];	//��½ʱ��
	ulong	m_ulProVer;		//Э��汾��

	Frm0181() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// ��̨��½Ӧ��
struct __attribute__(packed,aligned (1)) Frm0101 {
	byte	m_bytType;		//Ӧ������
	byte	m_bytYear;
	byte	m_bytMonth;
	byte	m_bytDay;
	byte	m_bytHour;
	byte	m_bytMinute;
	byte	m_bytSecond;

	Frm0101() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// ��̨��������
struct __attribute__(packed,aligned (1)) Frm1101 {
	byte	m_bytCenter[16];	// �����ط���
	ulong	m_ulIp;				// IP
	ushort	m_usPort;			// �˿�
	ulong	m_ulBakIp;			// ����IP
	ushort	m_ulBakPort;		// ���ö˿�

	Frm1101() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// ��������
struct __attribute__(packed,aligned (1)) Frm1281 {
	byte	m_bytCurSft[20];	//��ǰ����汾
	byte	m_bytCurSys[15];	//��ǰϵͳ�汾
	ulong	m_ulSysStat;		//ϵͳ״̬	

	Frm1281() {memset(this, 0, sizeof(*this));}
};

//----------------------------------------------------------------------
// ��������Ӧ��
struct __attribute__(packed,aligned (1)) Frm1201 {
	byte	m_bytRegFlag;		// ��̨ע���־
	ulong	m_ulVerLst;			// ��ͨ��Ŀ���汾��
	ulong	m_ulSoftID;			// Ӧ�����������ID
	ulong	m_ulTxtAdID;		// ���ֹ��ID
	ulong	m_ulLcdTxtID;		// ��Ļ���ֽ�ĿID
	ulong	m_ulSysID;			// ϵͳ�ں�����ID
	byte	m_bytCycPlayCnt;	// ��Ƶѭ�����Ŵ���
	byte	m_bytSysVol;		// ϵͳ������С(0-100)
	ulong	m_aryDriverID[5];	// ˾��ID
	ulong	m_ulLineID;			// ��·�ļ�ID

	Frm1201() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// ���������ͨ��Ŀ��
struct __attribute__(packed,aligned (1)) Frm1282 {
	byte	m_bytPackNo;			//�����

	Frm1282() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// ��̨������ͨ��Ŀ��Ӧ��
struct __attribute__(packed,aligned (1)) Frm1202 {
	ulong	m_ulVerLst;			// �汾��
	byte	m_bytTotalFrame;	// ��֡��
	byte	m_bytCurFrameNo;	// ��ǰ֡��
	byte	m_bytZoneNo;		// �����

	Frm1202() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// ��ȡ�ļ���������
struct __attribute__(packed,aligned (1)) Frm2181 {
	byte m_bytType;			// ��Ŀ����
	ulong m_ulID;			// ��ĿID

	Frm2181() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// ��ȡ�ļ���ϢӦ��
struct __attribute__(packed,aligned (1)) Frm2101 {

	struct point {
		ulong m_ulLon;
		ulong m_ulLat;
	};

	ulong	m_ulID;				//��Ŀ���
	byte	m_bytFlag;			//��־
	ulong	m_ulFileSize;		//�ļ���С
	byte	m_bytMd5[32];		//MD5ֵ
	byte	m_bytFileType;		//�ļ�����
	byte	m_bytStartTm[3];	//��ʼʱ��
	byte	m_bytEndTm[3];		//��ֹʱ��
	byte	m_bytPointCnt;		//��γ�ȸ���
	point	m_aryPos[10];		//��γ������

	Frm2101() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// �ļ���������
struct __attribute__(packed,aligned (1)) Frm2182 {
	ulong m_ulID;				// ��ĿID
	ulong m_ulBeginLoation;		// ��ʼλ��

	Frm2182() {memset(this, 0, sizeof(*this));}
};


//-----------------------------------------------------------------------
// �ļ�����Ӧ��
struct __attribute__(packed,aligned (1)) Frm2102 {
	ulong	m_ulID;			// ��ĿID
	byte	m_bytFlag;		// ��־
	ushort	m_usBlockSize;	// ���С
	ushort	m_usFrameSize;	// ֡��С

	Frm2102() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// �ļ���������
struct __attribute__(packed,aligned (1)) Frm2183 {
	ushort	m_usBlockNo;			// ��ǰ�����
	byte	m_bytBlockSta[128];		// ��״̬

	Frm2183() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// �ļ�����֡
struct __attribute__(packed,aligned (1)) Frm2103 {
	ushort	m_usBlockNo;			// �����
	ushort	m_usFrameID;			// ֡ID
	byte	m_bytFrameBuf[1400-4];	// ֡����

	Frm2103() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// �ļ�����״̬��ѯ
struct __attribute__(packed,aligned (1)) Frm2104 {
	byte	m_bytQueryNo;		// ��ѯ���
	ushort	m_usBlockNo;		// �����

	Frm2104() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// �ļ�����״̬Ӧ��
struct __attribute__(packed,aligned (1)) Frm2184 {
	byte	m_bytQueryNo;		// ��ѯ���
	ushort	m_usBlockNo;		// �����
	byte	m_bytBlockSta[128];	// ��״̬

	Frm2184() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// �ļ�������
struct __attribute__(packed,aligned (1)) Frm2185 {
	byte	m_bytType;		// ��Ŀ����
	ulong	m_ulID;			// ��ĿID
	byte	m_bytFlag;		// �����־

	Frm2185() {memset(this, 0, sizeof(*this));}
};

//-----------------------------------------------------------------------
// �������֪ͨ֡
struct __attribute__(packed,aligned (1)) Frm34 {
	byte	m_bytType;
	byte	m_bytFrom;

	Frm34() { m_bytType = 0x34; m_bytFrom = 0x01; }
};

//-----------------------------------------------------------------------
// �����ķ��Ͷ�������
struct __attribute__(packed,aligned (1)) Frm38 {
	byte m_bytType;
	byte m_bytSrc;
	char m_szTel[16];
	char m_szMsg[160];

	Frm38() { memset(this, 0, sizeof(*this));  m_bytType = 0x38; }
};

//-----------------------------------------------------------------------
// �����ķ��Ͷ�������Ӧ��
struct __attribute__(packed,aligned (1)) Frm39 {
	byte	m_bytType;
	byte	m_bytReply;
	Frm39() { memset(this, 0, sizeof(*this)); m_bytType = 0x39; }
};

//-----------------------------------------------------------------------
// �����ļ������б�
struct __attribute__(packed,aligned (1)) FileAttr {
	bool	m_bExist;		// �ļ��Ƿ����
	ulong	m_ulID;			// �ļ�ID
	ulong	m_ulFileSize;	// �ļ���С
	byte	m_bytMd5[32];	// MD5ֵ	
	ulong	m_ulDownSize;	// ������
};

struct __attribute__(packed,aligned (1)) FileList {
	FileAttr	m_objFile[FILE_TYPE_COUNT];		//�ļ�����
	ulong		m_ulVer;						//�ļ��б�汾

	FileList() { memset(this, 0, sizeof(*this));}
};

//#pragma pack(pop)	//�ָ�ԭ���Ķ��뷽ʽ

#endif
