#ifndef COMUSERV_H_11BGHSWE5567WE
#define COMUSERV_H_11BGHSWE5567WE

#include "../GlobalShare/ComuServExport.h"

#undef MSG_NOR
#undef MSG_DBG
#undef MSG_ERR
#define MSG_NOR		0
#define MSG_DBG		0
#define MSG_ERR		0

enum KEY_SYSV
{
	KEY_SEM_CFG = 1701,
	KEY_SEM_GPS,
	//KEY_SEM_DOG,
	KEY_SEM_IO,
	KEY_SEM_TM,
	
	KEY_SHM_IMPCFG,
	KEY_SHM_SECCFG,
	KEY_SHM_GPS,
	//KEY_SHM_DOG,
	KEY_SHM_IO,
	KEY_SHM_TM,

	KEY_SEM_DIAODU,
	KEY_SEM_LED,
	KEY_SEM_PHONE,
	KEY_SEM_QIAN,
	KEY_SEM_LIU,
	KEY_SEM_DVR,
	KEY_SEM_SOCK,
	KEY_SEM_IOQUE,
	
	KEY_MSG_DIAODU,
	KEY_MSG_LED,
	KEY_MSG_PHONE,
	KEY_MSG_QIAN,
	KEY_MSG_LIU,
	KEY_MSG_DVR,
	KEY_MSG_SOCK,
	KEY_MSG_IOQUE,
	
	KEY_SHM_DIAODU_1,
	KEY_SHM_DIAODU_2,
	KEY_SHM_DIAODU_3,

	KEY_SHM_LED_1,
	KEY_SHM_LED_2,
	KEY_SHM_LED_3,

	KEY_SHM_PHONE_1,
	KEY_SHM_PHONE_2,
	KEY_SHM_PHONE_3,

	KEY_SHM_QIAN_1,
	KEY_SHM_QIAN_2,
	KEY_SHM_QIAN_3,

	KEY_SHM_LIU_1,
	KEY_SHM_LIU_2,
	KEY_SHM_LIU_3,

	KEY_SHM_DVR_1,
	KEY_SHM_DVR_2,
	KEY_SHM_DVR_3,

	KEY_SHM_SOCK_1,
	KEY_SHM_SOCK_2,
	KEY_SHM_SOCK_3,

	KEY_SHM_IOQUE_1,
	KEY_SHM_IOQUE_2,
	KEY_SHM_IOQUE_3,
};

const DWORD		QUEUEVALID_PERIOD = 120000;

struct tagMsgQueInfo
{
	DWORD	m_dwDevSymb;
	int		m_iKeySem;
	int		m_iKeyMsg;

	int		m_iKeyShm1;
	DWORD	m_dwShm1Size; //Ӧ��4096�ı���
	int		m_iKeyShm2;
	DWORD	m_dwShm2Size; //Ӧ��4096�ı���
	int		m_iKeyShm3;
	DWORD	m_dwShm3Size; //Ӧ��4096�ı���
};


#define PAGE(n)	(4096 * n)

// ����Ӧ����һ�����ȼ�����Ϻ�
const tagMsgQueInfo MSGQUE_INFO[] = 
{
	{DEV_SOCK,		KEY_SEM_SOCK,	KEY_MSG_SOCK,	KEY_SHM_SOCK_1,		PAGE(32),	KEY_SHM_SOCK_2,		PAGE(256),	KEY_SHM_SOCK_3,		PAGE(64)},
	{DEV_LED,		KEY_SEM_LED,	KEY_MSG_LED,	KEY_SHM_LED_1,		PAGE(1),	KEY_SHM_LED_2,		PAGE(2),	KEY_SHM_LED_3,		PAGE(1)},
	{DEV_QIAN,		KEY_SEM_QIAN,	KEY_MSG_QIAN,	KEY_SHM_QIAN_1,		PAGE(2),	KEY_SHM_QIAN_2,		PAGE(4),	KEY_SHM_QIAN_3,		PAGE(2)},
	{DEV_DVR,		KEY_SEM_DVR,	KEY_MSG_DVR,	KEY_SHM_DVR_1,		PAGE(1),	KEY_SHM_DVR_2,		PAGE(1),	KEY_SHM_DVR_3,		PAGE(1)},
	{DEV_PHONE,		KEY_SEM_PHONE,	KEY_MSG_PHONE,	KEY_SHM_PHONE_1,	PAGE(1),	KEY_SHM_PHONE_2,	PAGE(4),	KEY_SHM_PHONE_3,	PAGE(2)},
	{DEV_DIAODU,	KEY_SEM_DIAODU,	KEY_MSG_DIAODU,	KEY_SHM_DIAODU_1,	PAGE(2),	KEY_SHM_DIAODU_2,	PAGE(4),	KEY_SHM_DIAODU_3,	PAGE(2)},
	{DEV_UPDATE,	KEY_SEM_LIU,	KEY_MSG_LIU,	KEY_SHM_LIU_1,		PAGE(1),	KEY_SHM_LIU_2,		PAGE(1),	KEY_SHM_LIU_3,		PAGE(1)},
	{DEV_IO,		KEY_SEM_IOQUE,	KEY_MSG_IOQUE,	KEY_SHM_IOQUE_1,	PAGE(1),	KEY_SHM_IOQUE_2,	PAGE(1),	KEY_SHM_IOQUE_3,	PAGE(1)},
};


// IO�������״̬
struct tagIOSta
{
	DWORD		m_dwTRUMPOpenDevSymb;	// ������Դ�豸
	bool		m_bPhoneAudio;			// �Ƿ��л����ֻ���Ƶ���
};

#if VEHICLE_TYPE == VEHICLE_M
// ˳��Ҫ��ö������IO_SYMB�е�����һ��!
// �ܽű����Ϊ65535,��ʾ��һ��Ԥ��IO,�����κ�����
const tagIOCfg SYSIO_CFG[] = 
{
	// �ܽű��		ȥ��ʱ��	in/out	�ж�����	�жϷ�ʽ	������ƽ/����	�ܽŵ͵�ƽ			�ܽŸߵ�ƽ
	//	ui			ui(ms)		uc(0/1)	b		c(��ƽ1/����2/˫��3)	c		uc					uc
	
	// ����
	{109,			200,		0,		true,			3,				1,		IO_ACC_ON,				IO_ACC_OFF},				// ACC
	{212,			2000,		0,		true,			3,				1,		IO_ALARM_OFF,			IO_ALARM_ON},			// �پ�
	{211,			200,		0,		true,			3,				1,		IO_JIJIA_HEAVY,			IO_JIJIA_LIGHT},		// �������ź�
#if USE_MENCHI == 0
	{213,			200,		0,		true,			3,				1,		IO_FDOOR_OPEN,			IO_FDOOR_CLOSE},		// ǰ��
	{209,			200,		0,		true,			3,				1,		IO_BDOOR_OPEN,			IO_BDOOR_CLOSE},		// ����
#endif
#if USE_MENCHI == 1
	{213,			200,		0,		true,			3,				1,		IO_FDOOR_CLOSE,			IO_FDOOR_OPEN},		// ǰ��
	{209,			200,		0,		true,			3,				1,		IO_BDOOR_CLOSE,			IO_BDOOR_OPEN},		// ����
#endif
	{65535,			200,		0,		true,			3,				0,		IO_JIAOSHA_VALID,		IO_JIAOSHA_INVALID},	// ��ɲ�ź�(��δʵ��)
	{65535,			200,		0,		true,			3,				0,		IO_SHOUSHA_VALID,		IO_SHOUSHA_INVALID},	// ��ɲ�ź�(��δʵ��)
	{216,			200,		0,		true,			3,				1,		IO_PASSENGER1_VALID,	IO_PASSENGER1_INVALID},
	{217,			200,		0,		true,			3,				1,		IO_PASSENGER2_VALID,	IO_PASSENGER2_INVALID},
	{218,			200,		0,		true,			3,				1,		IO_PASSENGER3_VALID,	IO_PASSENGER3_INVALID},
	{206,			200,		0,		true,			3,				1,		IO_HDELELOCK_CLOSE,		IO_HDELELOCK_OPEN},
	
	
	// ���
	{111,			0,			1,		false,			0,				0,		IO_HUBPOW_OFF,			IO_HUBPOW_ON},			// HUB��Դ
	{133,			0,			1,		false,			0,				0,		IO_PHONEPOW_OFF,		IO_PHONEPOW_ON},		// �ֻ�ģ���Դ
	{132,			0,			1,		false,			0,				0,		IO_PHONERST_LO,			IO_PHONERST_HI},		// �ֻ�ģ�鸴λ�ź�
	{215,			0,		    1,		false,		    0,				0,		IO_HARDPOW_OFF,			IO_HARDPOW_ON},			// Ӳ�̵�Դ
	{117,			0,		    1,		false,		    0,				0,		IO_TRUMPPOW_ON,			IO_TRUMPPOW_OFF},		// ���ŵ�Դ
	{112,			0,		    1,		false,		    0,				0,		IO_SCHEDULEPOW_OFF,		IO_SCHEDULEPOW_ON},		// ��������Դ
	{214,			0,		    1,		false,		    0,				0,		IO_VIDICONPOW_OFF,		IO_VIDICONPOW_ON},		// ����ͷ��Դ
#if USE_JIDIANQI == 0
	{210,			0,		    1,		false,		    0,				0,		IO_OILELECCTL_CUT,		IO_OILELECCTL_RESUME},	// ���Ͷϵ�
#endif
#if USE_JIDIANQI == 1
	{210,			0,		    1,		false,		    0,				0,		IO_OILELECCTL_RESUME,		IO_OILELECCTL_CUT},	// ���Ͷϵ�
#endif
	{110,			0,		    1,		false,		    0,				0,		IO_APPLEDPOW_OFF,		IO_APPLEDPOW_ON},		// Ӧ�ó�����ָʾ��
	{208,			0,		    1,		false,		    0,				0,		IO_TOPLEDPOW_OFF,		IO_TOPLEDPOW_ON},		// ���Ƶ�Դ
	{120,			0,		    1,		false,		    0,				0,		IO_AUDIOSEL_PHONE,		IO_AUDIOSEL_PC},		// ��Ƶ�л�ѡ��
	{119,			0,		    1,		false,		    0,				0,		IO_EARPHONESEL_ON,		IO_EARPHONESEL_OFF},	// ����/����ѡ��
	{131,			0,		    1,		false,		    0,				0,		IO_TW2835RST_LO,		IO_TW2835RST_HI},		// TW2835��λ�ź�
	//	{105,			0,			1,		false,			0,				0,		IO_GPSPOW_OFF,			IO_GPSPOW_ON},			// GPS��Դ(��δʵ��)	
	//	{103,			0,		    1,		false,		    0,				0,		IO_SYSLEDPOW_ON,		IO_SYSLEDPOW_OFF},		// ϵͳ����ָʾ��
};
#endif

#if VEHICLE_TYPE == VEHICLE_V8
// ˳��Ҫ��ö������IO_SYMB�е�����һ��!
// �ܽű����Ϊ65535,��ʾ��һ��Ԥ��IO,�����κ�����
const tagIOCfg SYSIO_CFG[] = 
{
	// �ܽű��		ȥ��ʱ��	in/out	�ж�����	�жϷ�ʽ	������ƽ/����	�ܽŵ͵�ƽ			�ܽŸߵ�ƽ
	//	ui			ui(ms)		uc(0/1)	b		c(��ƽ1/����2/˫��3)	c		uc					uc
	
	// ����
	{247,			200,		0,		true,			3,				1,		IO_ACC_ON,				IO_ACC_OFF},				// ACC
	{221,			15000,		0,		true,			3,				1,		IO_QIANYA_VALID,		IO_QIANYA_INVALID},			// Ƿѹ
#if USE_MENCHI == 0
	{237,			200,		0,		true,			3,				1,		IO_ELECSPEAKER_VALID,	IO_ELECSPEAKER_INVALID},	// ���������ź�
	{238,			200,		0,		true,			3,				1,		IO_FDOOR_OPEN,			IO_FDOOR_CLOSE},		// ǰ��
#endif
#if USE_MENCHI == 1
	{237,			200,		0,		true,			3,				1,		IO_ELECSPEAKER_INVALID,	IO_ELECSPEAKER_VALID},	// ���������ź�
	{238,			200,		0,		true,			3,				1,		IO_FDOOR_CLOSE,			IO_FDOOR_OPEN},		// ǰ��
#endif
	{239,			2000,		0,		true,			3,				1,		IO_ALARM_OFF,			IO_ALARM_ON},			// �پ�
	{240,			8000,		0,		true,			3,				1,		IO_POWDETM_VALID,		IO_POWDETM_INVALID},
	{242,			200,		0,		true,			3,				1,		IO_JIJIA_HEAVY,			IO_JIJIA_LIGHT},
	{248,			200,		0,		true,			3,				1,		IO_CHEDENG_VALID,		IO_CHEDENG_INVALID},
	{122,			200,		0,		true,			3,				1,		IO_ZHENLING_VALID,		IO_ZHENLING_INVALID},
//	{146,			200,		0,		true,			3,				1,		IO_SPIDO_CARD_EXIST,	IO_SPIDO_CARD_NOTEXIST},
//	{144,			200,		0,		true,			3,				1,		IO_SPICK_CARD_NOTPROTECT,IO_SPICK_CARD_PROTECT},	
	
	// ���
	{119,			0,			1,		false,			0,				0,		IO_SYSLEDPOW_RED_OFF,	IO_SYSLEDPOW_RED_ON},	// ����ָʾ�ƣ��죩
	{121,			0,			1,		false,			0,				0,		IO_SYSLEDPOW_GREEN_OFF,	IO_SYSLEDPOW_GREEN_ON},	// ����ָʾ�ƣ��̣�
	{250,			0,			1,		false,			0,				0,		IO_SCHEDULEPOW_OFF,		IO_SCHEDULEPOW_ON},		// ��������Դ 
	{145,			0,		    1,		false,		    0,				0,		IO_TRUMPPOW_ON,			IO_TRUMPPOW_OFF},		// ��Ƶ���ŵ�Դ
	{253,			0,		    1,		false,		    0,				0,		IO_EARPHONESEL_ON,		IO_EARPHONESEL_OFF},	// ����/����ѡ��
	{254,			0,		    1,		false,		    0,				0,		IO_AUDIOSEL_PHONE,		IO_AUDIOSEL_PC},		// ��Ƶ�л�ѡ��
	{256,			0,		    1,		false,		    0,				0,		IO_PHONERST_LO,			IO_PHONERST_HI},		// �ֻ�ģ�鸴λ�ź�
	{255,			0,		    1,		false,		    0,				0,		IO_PHONEPOW_OFF,		IO_PHONEPOW_ON},		// �ֻ�ģ���Դ
	{241,			0,		    1,		false,		    0,				0,		IO_YUYIN_OFF,			IO_YUYIN_ON},			// �����л�����
#if USE_JIDIANQI == 0
	{251,			0,		    1,		false,		    0,				0,		IO_OILELECCTL_CUT,		IO_OILELECCTL_RESUME},	// ���͵����
#endif
#if USE_JIDIANQI == 1
	{251,			0,		    1,		false,		    0,				0,		IO_OILELECCTL_RESUME,		IO_OILELECCTL_CUT},	// ���͵����
#endif
	{252,			0,		    1,		false,		    0,				0,		IO_VIDICONPOW_OFF,		IO_VIDICONPOW_ON},		// ����ͷ/TW2865��Դ
	{257,			0,		    1,		false,		    0,				0,		IO_TW2865RST_LO,		IO_TW2865RST_HI},		// TW2865��λ
	{219,			0,		    1,		false,		    0,				0,		IO_LCDPOW_OFF,			IO_LCDPOW_ON},			// LCD��Դ����
	{130,			0,			1,		false,			0,				0,		IO_USBPOW_OFF,			IO_USBPOW_ON},			// USB2.0��Դ
};
#endif

#if VEHICLE_TYPE == VEHICLE_M2
// ˳��Ҫ��ö������IO_SYMB�е�����һ��!
// �ܽű����Ϊ65535,��ʾ��һ��Ԥ��IO,�����κ�����
const tagIOCfg SYSIO_CFG[] = 
{
	// �ܽű��		ȥ��ʱ��	in/out	�ж�����	�жϷ�ʽ	������ƽ/����	�ܽŵ͵�ƽ			�ܽŸߵ�ƽ
	//	ui			ui(ms)		uc(0/1)	b		c(��ƽ1/����2/˫��3)	c		uc					uc
	
	// ����
	{247,			200,		0,		true,			3,				1,		IO_ACC_ON,				IO_ACC_OFF},				// ACC
	{221,			15000,		0,		true,			3,				1,		IO_QIANYA_VALID,		IO_QIANYA_INVALID},			// Ƿѹ
#if USE_MENCHI == 0
	{237,			200,		0,		true,			3,				1,		IO_FDOORALERT_VALID,	IO_FDOORALERT_INVALID},		// ǰ�ű���
	{238,			200,		0,		true,			3,				1,		IO_FDOOR_OPEN,			IO_FDOOR_CLOSE},			// ǰ�ż��
	{147,			200,		0,		true,			3,				1,		IO_BDOORALERT_VALID,	IO_BDOORALERT_INVALID},		// ���ű���
	{242,			200,		0,		true,			3,				1,		IO_BDOOR_OPEN,			IO_BDOOR_CLOSE},
#endif
#if USE_MENCHI == 1
	{237,			200,		0,		true,			3,				1,		IO_FDOORALERT_INVALID,	IO_FDOORALERT_VALID},		// ǰ�ű���
	{238,			200,		0,		true,			3,				1,		IO_FDOOR_CLOSE,			IO_FDOOR_OPEN},				// ǰ�ż��
	{147,			200,		0,		true,			3,				1,		IO_BDOORALERT_INVALID,	IO_BDOORALERT_VALID},		// ���ű���
	{242,			200,		0,		true,			3,				1,		IO_BDOOR_CLOSE,			IO_BDOOR_OPEN},
#endif
	{239,			2000,		0,		true,			3,				1,		IO_ALARM_OFF,			IO_ALARM_ON},				// �پ�
	{146,			2000,		0,		true,			3,				1,		IO_JIAOSHA_VALID,		IO_JIAOSHA_INVALID},		// ��ɲ�ź�
	{240,			8000,		0,		true,			3,				1,		IO_POWDETM_VALID,		IO_POWDETM_INVALID},
	{248,			200,		0,		true,			3,				1,		IO_CHEDENG_VALID,		IO_CHEDENG_INVALID},
	{122,			200,		0,		true,			3,				1,		IO_ZHENLING_VALID,		IO_ZHENLING_INVALID},
	
	// ���
	{119,			0,			1,		false,			0,				0,		IO_SYSLEDPOW_RED_OFF,	IO_SYSLEDPOW_RED_ON},	// ����ָʾ�ƣ��죩
	{121,			0,			1,		false,			0,				0,		IO_SYSLEDPOW_GREEN_OFF,	IO_SYSLEDPOW_GREEN_ON},	// ����ָʾ�ƣ��̣�
	{250,			0,			1,		false,			0,				0,		IO_SCHEDULEPOW_OFF,		IO_SCHEDULEPOW_ON},		// ��������Դ 
	{145,			0,		    1,		false,		    0,				0,		IO_TRUMPPOW_ON,			IO_TRUMPPOW_OFF},		// ��Ƶ���ŵ�Դ
	{253,			0,		    1,		false,		    0,				0,		IO_EARPHONESEL_ON,		IO_EARPHONESEL_OFF},	// ����/����ѡ��
	{254,			0,		    1,		false,		    0,				0,		IO_AUDIOSEL_PHONE,		IO_AUDIOSEL_PC},		// ��Ƶ�л�ѡ��
	{256,			0,		    1,		false,		    0,				0,		IO_PHONERST_LO,			IO_PHONERST_HI},		// �ֻ�ģ�鸴λ�ź�
	{255,			0,		    1,		false,		    0,				0,		IO_PHONEPOW_OFF,		IO_PHONEPOW_ON},		// �ֻ�ģ���Դ
	{241,			0,		    1,		false,		    0,				0,		IO_YUYIN_OFF,			IO_YUYIN_ON},			// �����л�����
	{144,			0,		    1,		false,		    0,				0,		IO_JIDIANQI_OFF,		IO_JIDIANQI_ON},		// �����л�����
#if JIDIANQI_TYPE == 0
	{251,			0,		    1,		false,		    0,				0,		IO_OILELECCTL_CUT,		IO_OILELECCTL_RESUME},	// ���͵����
#endif
#if JIDIANQI_TYPE == 1
	{251,			0,		    1,		false,		    0,				0,		IO_OILELECCTL_RESUME,	IO_OILELECCTL_CUT},	// ���͵����
#endif
	{252,			0,		    1,		false,		    0,				0,		IO_VIDICONPOW_OFF,		IO_VIDICONPOW_ON},		// ����ͷ/TW2865��Դ
	{257,			0,		    1,		false,		    0,				0,		IO_TW2865RST_LO,		IO_TW2865RST_HI},		// TW2865��λ
	{219,			0,		    1,		false,		    0,				0,		IO_LCDPOW_OFF,			IO_LCDPOW_ON},			// LCD��Դ����
	//{130,			0,			1,		false,			0,				0,		IO_USBPOW_OFF,			IO_USBPOW_ON},			// USB2.0��Դ
	{152,			0,			1,		false,			0,				0,		IO_HUBPOW_OFF,			IO_HUBPOW_ON},			// HUB��Դ
	{151,			0,			1,		false,			0,				0,		IO_HARDPOW_OFF,			IO_HARDPOW_ON},			// Ӳ�̵�Դ
};
#endif

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux specific) */
};

struct msgst
{
	long int msg_type;
	char buf[1];
};

#pragma pack(4)
typedef struct tagQueueHead
{
	tagQueueHead* m_pNext; // ��¼������,��ָ��(�����Ե�ַ)����Ϊ��Ե�ַ, ��Ϊ��Ubuntu�Ϸ��ֲ�ͬ����ӳ��ͬһ�����ڴ�ʱ,��ʼ��ַ������ͬ
							// �ر��,NULL�Ա�ʾ�յ�ַ
	DWORD		m_dwPushTime;
	DWORD		m_dwPushSymb; // ��Դ��־
	WORD		m_wDataLen;

	tagQueueHead() { memset( this, 0, sizeof(*this) ); }
	~tagQueueHead() {}
}QUEUEHEAD, *PQUEUEHEAD;
#pragma pack()

#endif
