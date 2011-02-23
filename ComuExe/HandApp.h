#ifndef _INC_CHSTAPP_H
#define _INC_CHSTAPP_H

/****�ֱ����ݴ洢����**HXD 090504 *******/ 

#define TB_REC_OFFSET    0 
#define TALK_DAIL_OFFSET 2000
#define TALK_RECV_OFFSET 2600
#define TALK_MISS_OFFSET 3200
#define ATM_REC_OFFSET   4000
#define SM_REC_OFFSET    9000
#define DIANZAO_REC_OFFSET    14000

/**************MainWin**********************************************************/
#define SIZE_TELEDITBUF                         20

/**************VirPhone**********************************************************/
#define SIZE_DTMFBUF							30
//#define SIZE_DPLYBUF							30

#define VIR_FREE								0
#define VIR_CALLING								1
#define VIR_TALKING								2
#define VIR_RINGING								3

#define CHA_HANDSET								2
#define CHA_HANDSFREE							4

/****************CallMan********************************************************/
#define TALK_DIAL								0
#define TALK_RECV								1
#define TALK_MISS								2

#define MAX_TALKREC								10

#define TALKREC_SIZE_TEL						20
#define TALKREC_SIZE_DATE						10
#define TALKREC_SIZE_TIME						10

/****************TelBook********************************************************/
#define MAX_TBREC							    50 //	50  //hxd 090424

#define TBREC_SIZE_NAME							10
#define TBREC_SIZE_TEL							20

#define SIZE_DPLYBUF                            50

#define OP_ADD                                  0
#define OP_MODIFY                               1
#define OP_FIND                                 2

/********************ShortMsg****************************************************/
#define MAX_SMREC								20

#define SMREC_SIZE_TIME							12
#define SMREC_SIZE_TEL							16
#define SMREC_SIZE_DATA							160 //161 hxd 090425 �����Բ��Ľ���޸�

/********************Succor****************************************************/
#define SUCCOR_MEDICAL                          0
#define SUCCOR_TROUBLE                          1
#define SUCCOR_INFORMATION                      2

/********************SetGPRS****************************************************/
#define SIZE_RECVBUF							30
#define TYPE_TCP_PORT1							0
#define TYPE_UDP_PORT1							1
#define TYPE_TCP_IP1							3
#define TYPE_UDP_IP1							4
#define TYPE_TCP_PORT2							5
#define TYPE_UDP_PORT2							6
#define TYPE_TCP_IP2							7
#define TYPE_UDP_IP2							8
#define TYPE_APN								9
#define TYPE_TEL								10
#define TYPE_LISTENTEL							11
#define TYPE_CDMANAME							12
#define TYPE_CDMAPASS							13

/********************SetCGV****************************************************/
#define TYPE_COMPANYID							0
#define TYPE_TERMIVER							1
#define TYPE_GROUPID							2

/********************SysSet****************************************************/
//�Աȶ�
#define MAX_STEP								9
#define INIT_STEP								4
#define VALUE_STEP								3
//������
#define OP_CALLRING                             0

/*****************SaveDataType**************************************************/
#define	TYPE_TB									0
#define TYPE_TK									1
#define TYPE_SM									2									

/******************AtmpMsg******************************************************/
#define MAX_ATMPREC								20

#define ATMPREC_SIZE_DATE						10
#define ATMPREC_SIZE_TIME						10
#define ATMPREC_SIZE_DATA						180

/*******************PhoneCallMsg*****************************************************/
#define MAX_PCREC								MAX_ATMPREC
#define PC_SIZE_DATE							ATMPREC_SIZE_DATE
#define PC_SIZE_TIME							ATMPREC_SIZE_TIME
#define PC_SIZE_DATA							ATMPREC_SIZE_DATA

#define CON_COE									(3600*1000)	//ת��ϵ��

typedef	void   (*RETINFORMER)(INT8U tellen, INT8U *tel, INT8U namelen, INT8U *name);
	   
struct Frm053d {			//����������Ϣ
	char	tel[15];		//�˿���ϵ��ʽ
	char	car_id[15];		//���ƺ�
	char	content[50];	//���˵��
	Frm053d()	{ memset(this, 0, sizeof(*this)); }
};

class CHstApp
{
public: 	
	CHstApp();
	~CHstApp();

	int Init();
	int Release();

	void P_ThreadHandleWork();

	void P_TmDrvRec();
	void P_TmIsOnLine();
	
private:
	enum {
		PowerOn = 0,		//�ϵ�
		Idle,				//����
		PowerOff,			//�ֱ��ϵ�ʱ�ͷ��ֱ�ģ����Դ
	}m_hststate;

	pthread_t		m_pthreadHandleWork;

	static ulong	m_hsttm;	//�ֱ�Уʱʱ��
	static bool		m_IsRefuseDial;	//�Ƿ�ܾ�����绰
	int				m_DialCount;	//����绰�ɹ�����
	static bool		m_IsReturnMainWin;		//�Ƿ��������

	void Init_Menu();

private:
	static tag2QHstCfg		m_HstCfg;
	static INT8U			m_hangupstate;		//�һ�״̬Ӧ��
	INT8U					m_tempbuf[100];
	static bool				m_hstmode;
	static INT8U			m_helptype;			//��������
	PROMPT_STRUCT			m_dianzhaoprompt;	//������ʾ��Ϣ
//	static bool				m_flagdz;			//�ж�Ӧ���ǵ��������ǵ��ٽ������
	static ulong			m_diatm;			//�绰��ͨ���ʱ��
	static EDITOR_STRUCT	EditorEntry_MainWin;  //cyh  �༭����Ĵ�����
	char					m_diasucctm[20];	//���ųɹ�ʱ��
	char					m_recvtm[20];		//����ʱ�̻�δ�ӵ绰δ��ʱ��
	ulong					m_talktm;			//�绰��ͨʱ��
	ulong					m_calltm;			//ͨ��ʱ��
	char					m_strcalltm[10];
//	bool					m_IsmissTel;		//�Ƿ���δ������
//	bool					m_Iscallsucc;		//����绰ʱ�Է��Ƿ��ͨ
	static INT8U			m_state_tel;		//δ�ӻ��ǲ��ŶԷ�δ��
	static INT8U			m_dianzhaostate;	//����״̬
	ulong					m_dianzhaotm;		//�յ�������Ϣʱ��
	ulong					m_qiangdatm;		//����ʱ��
	ulong					m_qiangdasucctm;	//����ɹ�ʱ��
	static bool				m_IsPDkey;			//�Ƿ��а���
	Frm053d					m_frm053d;
	static int				m_testalarmsta;		//��������״̬
	static int				m_msgsta;			//�ǵ�����Ϣ���ǵ�����Ϣ
	static int				m_pickupsta;		//�Զ�/�ֶ�����״̬
	static int				m_gpsdect;			//���GPS�ź�
	static int				m_gprssta;			//���õ�GPRS���� ǧ����/��ý��

	typedef struct {
		INT16U     index;
		void (* entryproc)(void);
	} FUNCENTRY_STRUCT;
	static FUNCENTRY_STRUCT Handler_Key_entry[7];  //cyh  �������£������ܰ����ĺ������������£��ʹ�����7�����������಻����
	static bool FindProcEntry(INT16U index, FUNCENTRY_STRUCT *funcentry, INT16U num);
	
/*************MainWin***********************************************************/
	static KEY_STRUCT		m_curkey;
	static bool				m_KBIsLocked, m_WaitKEY_Star;
	static INT8U			m_TelEditBuf[SIZE_TELEDITBUF];
	ITEMMENU_STRUCT			MENU_MAIN_CHILD[11];  //cyh add���������£�����SET�󼤻��9���˵�
	static ITEMMENU_STRUCT	MENU_MAIN;  //cyh add  �������£�����SET�󼤻�Ĳ˵����������ӵ�9���Ӳ˵������ָ��
	static WINSTACKENTRY_STRUCT WinStackEntry_MainWin;

/*************VirPhone***********************************************************/
	typedef struct {
		INT8U       attrib;
		INT8U       channel;
		INT8U       tellen;
		INT8U       tel[TALKREC_SIZE_TEL];
	} ENTRYTEL_STRUCT;
	static INT8U			m_status_phone;
	static INT8U			m_dtmfbuf[SIZE_DTMFBUF], m_dtmflen, m_dtmfpos;
	static INT8U			m_dplybuf_phone[SIZE_DPLYBUF], m_dplyptr_phone[SIZE_DPLYBUF], m_dplylen_phone;
	static ENTRYTEL_STRUCT	EntryTel;

	static PROMPT_STRUCT	PromptEntry_Phone;
	static WINSTACKENTRY_STRUCT WinStackEntry_Dia;			//���Ŵ���  //cyh ���Ž���Ĵ��ں���
	static WINSTACKENTRY_STRUCT WinStackEntry_DialSuccor;	//�������Ŵ���
	static WINSTACKENTRY_STRUCT WinStackEntry_Ring;			//���細��
	
/*************CallMan***********************************************************/
	//sizeof(TALKREC_STRUCT)=29
	typedef struct {
		INT8U       tellen;
		char		tel[TALKREC_SIZE_TEL];
		char		talktime[10];
		char		date[TALKREC_SIZE_DATE];
		char		time[TALKREC_SIZE_TIME];
	} TALKREC_STRUCT;		//ͨ����¼
	
	typedef struct {
		INT8U   NumTel;
		INT8U   NumSM;
	} HSTMISS_STRUCT;

	static HSTMISS_STRUCT	HstMiss;
	static MENUWIN_STRUCT	SelMenuWinEntry;		
	ITEMMENU_STRUCT			MENU_ERASE_DIAL;		//ɾ���Ѳ��绰
	ITEMMENU_STRUCT			MENU_ERASE_MISS;		//ɾ��δ�ӵ绰
	ITEMMENU_STRUCT			MENU_ERASE_RECV;		//ɾ���ѽӵ绰
	ITEMMENU_STRUCT			MENU_ERASE_ALL;			//ɾ������ͨ����¼
	ITEMMENU_STRUCT			MENU_ERASE_CHILD[4];
	ITEMMENU_STRUCT			MENU_MISS;				//δ������
	ITEMMENU_STRUCT			MENU_DIAL;				//�Ѳ��绰
	ITEMMENU_STRUCT			MENU_RECV;				//�ѽӵ绰
	ITEMMENU_STRUCT			MENU_ERASE;				//ɾ��
	ITEMMENU_STRUCT			MENU_CALLMAN_CHILD[4];
	ITEMMENU_STRUCT			MENU_CALLMAN;			//ͨ����¼
	static INT8U			m_talktype;
	static TALKREC_STRUCT	CurRecord;
	//ֱ�Ӱ���OK���������ͨ����¼�˵�
	ITEMMENU_STRUCT			MENU_OPREC_DIAL;		//����
	ITEMMENU_STRUCT			MENU_OPREC_READ;		//ͨ��ʱ��
	ITEMMENU_STRUCT			MENU_OPREC_ERASE;		//ɾ��
	ITEMMENU_STRUCT			MENU_OPREC_DISPLAY;		//��ʾͨ��ʱ��
	ITEMMENU_STRUCT			MENU_OPREC_SAVE;		//�洢
	ITEMMENU_STRUCT			MENU_OPREC_CHILD_CM[5];
	static ITEMMENU_STRUCT	MENU_OPREC_CM;
	static PROMPT_STRUCT	PromptEntry_CM;
	
	TALKREC_STRUCT			m_calllog;			//ͨ����¼
	
/*************TelBook***********************************************************/
	ITEMMENU_STRUCT			MENU_ADD;				//����
	ITEMMENU_STRUCT			MENU_FIND;				//����
	ITEMMENU_STRUCT			MENU_LIST;				//���
	ITEMMENU_STRUCT			MENU_DelALL;			//ɾ������
	ITEMMENU_STRUCT			MENU_Browse;			//�洢״��

	ITEMMENU_STRUCT			MENU_TELBOOK_CHILD[5];
	ITEMMENU_STRUCT			MENU_TELBOOK;			//�绰��
	//ÿ���绰��¼��Ӧ���Ӳ˵�
	ITEMMENU_STRUCT			MENU_DIAL_TB;			//����
	ITEMMENU_STRUCT			MENU_DEL_TB;			//ɾ��
	ITEMMENU_STRUCT			MENU_MODIFY_TB;			//�޸�
	ITEMMENU_STRUCT			MENU_OPREC_CHILD_TB[3];
	static ITEMMENU_STRUCT	MENU_OPREC_TB;

	//sizeof(TBREC_STRUCT)=32
	typedef struct {
		INT8U       namelen;
		INT8U       name[TBREC_SIZE_NAME];
		INT8U       tellen;
		INT8U       tel[TBREC_SIZE_TEL];
	} TBREC_STRUCT;
	static TBREC_STRUCT		currecord, readrecord;
	static INT16S			m_selrecord_tb;
	static INT8U			m_op_type_tb;  //cyh add: �Ե绰���Ĳ�����ʽ�����ӣ��޸ģ�����
	static char				m_dplybuf[SIZE_DPLYBUF];
	static MENUWIN_STRUCT	ListTBEntry;
	static RETINFORMER	RetInformer;		

	static BOOL             m_bDrvHaveLog;//hxd 090428

/*************SaveData***********************************************************/
	struct SAVEDATAINDEX_STRUCT
	{
		INT8U	num_TB;             // �绰������
		INT8U	num_TK_dia;        // �Ѳ��绰
		INT8U	num_TK_rec;        // �ѽӵ绰
		INT8U	num_TK_mis;         // δ�ӵ绰
		INT8U	num_Atmp;			 //������Ϣ
		INT8U	num_SM;             // ���Ÿ���
		INT8U	num_PC;				//���ٸ���
		INT8U	flag_TB[MAX_TBREC];	//�����绰��¼��λ���Ƿ�ʹ�õı��,1:��ռ��,0:δռ��
		INT8U	flag_TK_dia[MAX_TALKREC];
		INT8U	flag_TK_rec[MAX_TALKREC];
		INT8U	flag_TK_mis[MAX_TALKREC];
		INT8U	flag_Atmp[MAX_ATMPREC];
		INT8U	flag_SM[MAX_SMREC];
		INT8U	flag_PC[MAX_PCREC];	//���ٸ���
		SAVEDATAINDEX_STRUCT() { init(); }
		void init()
		{
			memset(this, 0, sizeof(*this));
		}
	};

/**************ShortMsg**********************************************************/
	ITEMMENU_STRUCT			MENU_READSM;			//�յ�����Ϣ
	ITEMMENU_STRUCT			MENU_WRITESM;			//д��Ϣ
	ITEMMENU_STRUCT			MENU_DELALL;			//ɾ��������Ϣ
	ITEMMENU_STRUCT			MENU_SHORTMSG_CHILD[3];
	ITEMMENU_STRUCT			MENU_SHORTMSG;			//����Ϣ

	ITEMMENU_STRUCT			MENU_WRITEBACK;			//�ظ�
	ITEMMENU_STRUCT			MENU_TRANSMIT;			//ת��
	ITEMMENU_STRUCT			MENU_ERASE_SM;			//ɾ��
	ITEMMENU_STRUCT			MENU_SAVE_SM;			//����
	ITEMMENU_STRUCT			MENU_DIAL_SM;			//����
	ITEMMENU_STRUCT			MENU_OPSM_CHILD[5];		
	static ITEMMENU_STRUCT	MENU_OPSM;				
	static MENUWIN_STRUCT	MENU_LISTSM;			//�г�����Ϣ

	//sizeof(SMREC_STRUCT)=190
	typedef struct {
		INT8U		unread;
		INT8U       tellen;
		INT8U       tel[SMREC_SIZE_TEL];
		INT8U       time[SMREC_SIZE_TIME];
		INT8U       datalen;
		INT8U       data[SMREC_SIZE_DATA];
	} SMREC_STRUCT;
	typedef struct  
	{
		byte		cmd;
		int			len0;		//�绰���볤��
		char		tel[16];	//�绰����
		int			len1;		//�������ݳ���
		char		info[160];	//��������
	} SMS_STRUCT;
	static INT16S			m_selrecord_sm;
	static SMREC_STRUCT		CurRecord_SM, EditRecord;
	SMREC_STRUCT			m_smrec;
	static INT8U			m_storebuf[300];
	static EDITOR_STRUCT	TelEditorEntry;

/*************SetGPRS***********************************************************/
	ITEMMENU_STRUCT			MENU_SETGPRS;			//����GPRS����
	static INT8U			m_recvbuf[SIZE_RECVBUF];
	static EDITOR_STRUCT	EditorEntry;
	typedef struct {
		INT8U ip;
		INT8U port;
		INT8U apn;
		INT8U tel;
		INT8U listentel;
		INT8U cdmaname;
		INT8U cdmapass;
	}SETGPRS_STRUCT;	//����GPRS���ݽṹ
	static SETGPRS_STRUCT	set_gprs;
	ITEMMENU_STRUCT			MENU_TCPPORT1;			//����ǧ����TCP�˿�1
	ITEMMENU_STRUCT			MENU_UDPPORT1;			//����ǧ����UDP�˿�1
	ITEMMENU_STRUCT			MENU_TCPIP1;			//����ǧ����TCPIP1
	ITEMMENU_STRUCT			MENU_UDPIP1;			//����ǧ����UDPIP1
	ITEMMENU_STRUCT			MENU_TCPPORT2;			//����ǧ����TCP�˿�2
	ITEMMENU_STRUCT			MENU_UDPPORT2;			//����ǧ����UDP�˿�2
	ITEMMENU_STRUCT			MENU_TCPIP2;			//����ǧ����TCPIP2
	ITEMMENU_STRUCT			MENU_UDPIP2;			//����ǧ����UDPIP2
	ITEMMENU_STRUCT			MENU_APN;				//����APN
	ITEMMENU_STRUCT			MENU_TEL;				//���ñ����ֻ���
	ITEMMENU_STRUCT			MENU_LISTENTEL;			//���ü����绰����
	ITEMMENU_STRUCT			MENU_ACTIVATE;			//����GPRS
	ITEMMENU_STRUCT			MENU_CDMANAME;          //����CDMA�û���
	ITEMMENU_STRUCT			MENU_CDMAPASS;			//����CDMA����
//	ITEMMENU_STRUCT			MENU_SET_CHILD[9];	    // hxd 090713	
//add lzy 101026
	ITEMMENU_STRUCT			MENU_TERMIVER;
	ITEMMENU_STRUCT			MENU_SETCOMP;
	ITEMMENU_STRUCT			MENU_SETGROUPID;

//	
#if USE_LIAONING_SANQI == 1													//
	ITEMMENU_STRUCT			MENU_SET_CHILD[16];	    	
#else
	ITEMMENU_STRUCT			MENU_SET_CHILD[13];		// hxd 090713
#endif																
	static ITEMMENU_STRUCT  MENU_SET;				//����GPRS����
	ITEMMENU_STRUCT			MENU_LIP;				//������ý��IP
	ITEMMENU_STRUCT			MENU_LPORT;				//������ý��˿�
	ITEMMENU_STRUCT			MENU_SET_LCHILD[2];
	static ITEMMENU_STRUCT	MENU_SETL;				//������ý�����				

/****************SysSet********************************************************/
	ITEMMENU_STRUCT			MENU_VERSION;			//�汾˵��
	ITEMMENU_STRUCT			MENU_SYSSET_CHILD[6];
	ITEMMENU_STRUCT			MENU_SYSSET;			//ϵͳ����
	//�Աȶȵ���	
	static INT8U			m_curvalue;				//�Աȶȵ�ǰֵ
	static WINSTACKENTRY_STRUCT WinStackEntry_AD;	//���ڶԱȶ����
	ITEMMENU_STRUCT			MENU_ADCONST;			//���ڶԱȶ�
	//ѡ������
	static INT8U			m_optype;				//��ǰ������
	ITEMMENU_STRUCT			MENU_CALLRING;			//��������
	ITEMMENU_STRUCT			MENU_SMRING;			//��������
	ITEMMENU_STRUCT			MENU_SELRING_CHILD[2];
	ITEMMENU_STRUCT			MENU_SELRING;			//����������
	static MENUWIN_STRUCT	MenuWinEntry ;
	//���ð�����
	ITEMMENU_STRUCT			MENU_SETKEYPT;			//���ð�����
	ITEMMENU_STRUCT			MENU_OPEN;				//�򿪰�����
	ITEMMENU_STRUCT			MENU_CLOSE;				//�رհ�����
	ITEMMENU_STRUCT			MENU_SELOPTION_CHILD[2];
	static ITEMMENU_STRUCT	MENU_SELOPTION;
	//���ý�����ʽ
	ITEMMENU_STRUCT			MENU_SETPICK;			//���ý�����ʽ
	ITEMMENU_STRUCT			MENU_AUTO;				//�Զ�����
	ITEMMENU_STRUCT			MENU_MANUAL;			//�ֶ�����
	ITEMMENU_STRUCT			MENU_SELPICK_CHILD[2];
	static ITEMMENU_STRUCT  MENU_SELPICK;
	//����ȱʡģʽ
	ITEMMENU_STRUCT			MENU_SETCHA;			//����ȱʡģʽ
	ITEMMENU_STRUCT			MENU_AUTO_CHA;			//����ģʽ
	ITEMMENU_STRUCT			MENU_MANUAL_CHA;		//�ֱ�ģʽ
	ITEMMENU_STRUCT			MENU_SELCHA_CHILD[2];
	static ITEMMENU_STRUCT  MENU_SELCHA;
		
/*************ParaChk***********************************************************/
	ITEMMENU_STRUCT			MENU_CENTERNO;			//��ѯ�����ط���
	ITEMMENU_STRUCT			MENU_GPRSSTATUS;		//��ѯGPRS״̬
	ITEMMENU_STRUCT			MENU_GPSSTATUS;			//��ѯGPS״̬
	ITEMMENU_STRUCT			MENU_PARACHK_CHILD[3];
	ITEMMENU_STRUCT			MENU_PARACHK;			//��ѯ����״̬
	static PROMPT_STRUCT	ParaPrompt;				//��ѯ������ʾ��Ϣ

/*****************************************************************************/
//wxl
    ITEMMENU_STRUCT			MENU_DATAREQUEST;       //��������������������
    ITEMMENU_STRUCT			MENU_BIAODINGSTART;		//�������б궨��ʼָʾ
	ITEMMENU_STRUCT			MENU_GRADUATIONBD;      // ACCON �̶ȱ궨
	ITEMMENU_STRUCT			MENU_GRADUATIONBDOFF;      //ACCOFF �̶ȱ궨
	ITEMMENU_STRUCT			MENU_ZEROBD;			// 0�̶ȱ궨
	ITEMMENU_STRUCT			MENU_QUERTERBD;			//ACCON 1/4�̶ȱ궨
	ITEMMENU_STRUCT			MENU_HALFBD;			// 1/2�̶ȱ궨
	ITEMMENU_STRUCT			MENU_THREEFOURBD;		// 3/4�̶ȱ궨
	ITEMMENU_STRUCT			MENU_FULLBD;			// ���̶ȱ궨
	ITEMMENU_STRUCT			MENU_ZEROBDOFF;			// ACCOFF 0�̶ȱ궨
	ITEMMENU_STRUCT			MENU_QUERTERBDOFF;			// 1/4�̶ȱ궨
	ITEMMENU_STRUCT			MENU_HALFBDOFF;			// 1/2�̶ȱ궨
	ITEMMENU_STRUCT			MENU_THREEFOURBDOFF;		// 3/4�̶ȱ궨
	ITEMMENU_STRUCT			MENU_FULLBDOFF;			// ���̶ȱ궨
	ITEMMENU_STRUCT			MENU_BIAODINGEND;		//�궨���ָʾ
	ITEMMENU_STRUCT			MENU_BIAODING;			//�̶ȱ궨
	ITEMMENU_STRUCT			MENU_BIAODINGTOCENTER;	//�����ķ����������б궨��������
    ITEMMENU_STRUCT			MENU_OILCHK_CHILD[5];
	ITEMMENU_STRUCT			MENU_GRADUATIONBD_CHILD[5];
	ITEMMENU_STRUCT			MENU_GRADUATIONBDOFF_CHILD[5];
	ITEMMENU_STRUCT			MENU_BIAODING_CHILD[2];  
	ITEMMENU_STRUCT			MENU_OILCHK;            //�������в�ѯ
    static PROMPT_STRUCT    OilPrompt;				//�������в�ѯ��ʾ��Ϣ
	
//wxl

/*****************************************************************************/
//hhq
    ITEMMENU_STRUCT			MENU_CHN1VIEW;       //Ԥ��1ͨ��
    ITEMMENU_STRUCT			MENU_CHN2VIEW;		 //Ԥ��2ͨ��
	ITEMMENU_STRUCT			MENU_CHN3VIEW;       //Ԥ��3ͨ��
	ITEMMENU_STRUCT			MENU_CHN4VIEW;       //Ԥ��4ͨ��
	ITEMMENU_STRUCT			MENU_CHNAVIEW;       //Ԥ��ȫͨ��
	ITEMMENU_STRUCT			MENU_VIDEOVIEW_CHILD[5];
	ITEMMENU_STRUCT			MENU_VIDEOVIEW;		 //Ԥ��ͨ��
//hhq

/***************AtmpMsg*********************************************************/
	typedef struct {
		char		date[ATMPREC_SIZE_DATE];
		char		time[ATMPREC_SIZE_TIME];
		INT8U       datalen;
		char		data[ATMPREC_SIZE_DATA];
	} ATMPREC_STRUCT;		//������Ϣ�ṹ
	static ATMPREC_STRUCT	CurRec_Atmp;			//��ǰ������Ϣ��ʾ
	static ATMPREC_STRUCT   m_tagDiaoduMsg;//cyh add:add  ��ֹ������Ϣ�������Ϣ�໥���ǡ�����е���ʱ��������һ��������Ϣ���鷳�ˡ�����
	PROMPT_STRUCT			AtmpPrompt;
	ATMPREC_STRUCT			m_atmprec;
	ITEMMENU_STRUCT			MENU_ATMPMSG;			//������Ϣ
	static MENUWIN_STRUCT	MENU_LISTATMP;			//��������Ϣ�б�
	static MENUWIN_STRUCT	MENU_READCONTENT;		//��������Ϣ����

/***************PhoneCallMsg*********************************************************/
//	typedef struct  {
//		char		date[PC_SIZE_DATE];
//		char		time[PC_SIZE_TIME];
//		INT8U       datalen;
//		char		data[PC_SIZE_DATA];
//	} PHONECALL_STRUCT;		//������Ϣ�ṹ
	static ATMPREC_STRUCT	CurRec_PC;
//	PROMPT_STRUCT			PcPrompt;
//	ATMPREC_STRUCT			m_pcrec;
	ITEMMENU_STRUCT			MENU_PCMSG;				//������Ϣ
	static MENUWIN_STRUCT	MENU_LISTPC;			//��������Ϣ�б�
	static MENUWIN_STRUCT	MENU_READCONTENT_PC;	//��������Ϣ����

/***************ProjectMenu*********************************************************/
	static ITEMMENU_STRUCT	MENU_PROJECT;
	ITEMMENU_STRUCT			MENU_PROJECTACCESS;	
	ITEMMENU_STRUCT			MENU_PROJECT_CHILD[14];
	ITEMMENU_STRUCT			MENU_UPDATESYS;			//U��ϵͳ����
	ITEMMENU_STRUCT			MENU_RESUMESET;			//�ָ���������
	ITEMMENU_STRUCT			MENU_FORMATSDISK;		//��ʽ��SD��
	ITEMMENU_STRUCT			MENU_UPDATEAPP;			//U��Ӧ������
	ITEMMENU_STRUCT			MENU_TESTCUTOIL;		//���Զ��Ͷϵ�
	ITEMMENU_STRUCT			MENU_INPUTVIDEO;		//��Ƶ�ļ����뵼��
	ITEMMENU_STRUCT			MENU_UDATAOUTPUT;		//U�����ݵ���
	ITEMMENU_STRUCT			MENU_TESTPCSOUND;		//PC��Ƶ����
	ITEMMENU_STRUCT			MENU_TESTCAM;			//����ͷ1���
	ITEMMENU_STRUCT			MENU_CHKSENSORSTA;		//��ѯ������״̬
	ITEMMENU_STRUCT			MENU_TESTALARM;			//��������
	ITEMMENU_STRUCT			MENU_CHKLED;			//LED�����
	ITEMMENU_STRUCT			MENU_CHKGPS;			//���GPS�ź�
	ITEMMENU_STRUCT			MENU_CHKLED_CHILD[4];	
	ITEMMENU_STRUCT			MENU_TESTCAM_CHILD[4];
	ITEMMENU_STRUCT			MENU_CHKLED_TIME;		//��LEDУʱ
	ITEMMENU_STRUCT			MENU_CHKLED_SHOW;		//��LED����ʹ��Ϣ
	ITEMMENU_STRUCT			MENU_CHKLED_BLAC;		//���ͺ���ָ���LED
	ITEMMENU_STRUCT			MENU_CHKLED_LIGH;		//��������ָ���LED
	ITEMMENU_STRUCT			MENU_TESTCAM1;			//����ͷ2���
	ITEMMENU_STRUCT			MENU_TESTCAM2;			//����ͷ2���
	ITEMMENU_STRUCT			MENU_TESTCAM3;			//����ͷ3���
	ITEMMENU_STRUCT			MENU_TESTCAM4;			//����ͷ4���
	ITEMMENU_STRUCT			MENU_LOGINLIU;
	static PROMPT_STRUCT	ProjectPrompt;

/***********DiaoZhao*************************************************************/
	static void HdlDianZhao();
	static void HdlQiangDa();

/************TestAlarm************************************************************/
	static void HdlTestAlarm();

/*************MainWin***********************************************************/
	static void DisplayScreen(void);
	static void SuspendProc_MainWin(void);
	static void ResumeProc_MainWin(void);
	static void InitProc_MainWin(void);
	static void HdlKeyProc_MainWin(void);
	static void Handler_KEY_ACK(void);
	static void Handler_KEY_FCN(void);
	static void Handler_KEY_UP(void);
	static void Handler_KEY_DOWN(void);
	static void Handler_KEY_SUCCOR(void);
	static void ExitTelEditor(void);
	static void Handler_KEY_DIGITAL(void);
	static void    ActivateMainWin(void);
	void    InitMainWin(void);

/***************VirPhone*********************************************************/
	static bool Display_DTMF(void);
	static void AddDtmfChar(INT8U ch);
	static void DelDtmfChar(void);
	static void ModifyDtmfChar(INT8U ch);
	static bool HaveDtmfChar(void);
	static void InitDtmfBuf(void);
	static void GetDisplayPara(void);
	static void Display_COMMON(INT8U textlen, char *textptr, INT8U tellen, INT8U *tel);
	static void Display_CALLING(void);
	static void Display_CALLING_INIT(void);
	static void Display_TALKING(void);
	static void Display_RINGING(void);
	static void Display_VOLUME(void);
	static void Display_CHANNEL(void);
	static void Status_Phone(void);
	static void ExitVirPhone(char *str, INT8U time);
	static void InitProc_Dial(void);
	static void InitProc_Ring(void);
	static void DestroyProc_Phone(void);
	static void Handler_CALLING(INT8U phonestatus);
	static void Handler_TALKING(INT8U phonestatus);
	static void Handler_RINGING(INT8U phonestatus);
	static void HdlEventProc(INT8U phonestatus);
	static void HdlKeyProc_Phone(void);
	static void    ActivateVirPhone_Dial(bool findsk, INT8U tellen, INT8U *tel);
	void    ActivateVirPhone_DialSuccor(INT8U tellen, INT8U *tel);
//	void    ActivateVirPhone_Ring(void);
//	void    HangupVirPhone(void);
//	void    ResetVirPhone(void);
//	void    InitVirPhone(void);
//	void    InformVirPhone(INT8U phonestatus);
//	bool	VirPhoneIsActivated(void);

/*************CallMan***********************************************************/
	static void OPREC_DIAL(void);
	static void OPREC_READ(void);
	static void ACK_OP_ERASE(void);
	static void OPREC_ERASE(void);
	static void OPREC_DISPLAY(void);
	static void OPREC_SAVE(void);
	static void Handler_KEY_ACK_CallMan(void);
	static INT16U MaxRecordProc_CallMan(void);
	static INT16U ReadRecordProc_CallMan(INT16S numrec, INT8U *ptr);
	static void SelTalkProc(void);
	static void ACK_DelTalkProc(void);
	static void DelTalkProc(void);
	static void ListMissTel(void);
	static void ExitPromptReadMissTel(void);
	static void    ActivateDialTalkRec(void);
	static bool PromptReadMissTel(void);
	//hxd 090428
	static void InputDrvLog(void);
	static void ACK_InputDrvNum(void);//������˾����½ģ��Ĵ�����
	static void ACK_InputDrvNum_AD(void);//���ŵ�˾����½ģ��Ĵ�����
	static void ACK_ExitDrvPro(void);

/*************TelBook***********************************************************/
	static void AddRec(void);
	static void ACK_AddRec(void);
	static void ModifyRec(void);
	static void ACK_ModifyRec(void);
	static void ExitPhoneEditor(void);
	static void ActivatePhoneEditor(void);
	static void ExitNameEditor(void);
	static void ActivateNameEditor(void);
	static void ActivateNameEditor_Md(void);
	static void AddTB(void);
	static void ACK_DelProc(void);
	static void DelProc(void);
	static void ModifyProc(void);
	static void DialProc(void);
	static void Handler_KEY_ACK_TelBook(void);
	static void Handler_KEY_ACK_RET(void);
	static INT16U MaxRecordProc_TelBook(void);
	static INT16U InitRecordProc_TelBook(void);
	static INT16U ReadRecordProc_TelBook(INT16S numrec, INT8U *ptr);
	static void ListTB_Init(INT16U initrecord);
	static void FindTB(void);
	static void ACK_DelAllTB(void);
	static void DelAllTB(void);
	static void BrowseMemory(void);
	static void    ListTB(void);
	static bool	ListTB_RET(void Informer(INT8U tellen, INT8U *tel, INT8U namelen, INT8U *name));
	static void    SaveToTB(INT8U *tel, INT8U tellen);

/*************SaveData***********************************************************/
	static bool AddTBRec(TBREC_STRUCT *recptr);
	void AddToTB(TBREC_STRUCT *recptr);
	static bool ModifyTBRec (INT16U numrec, TBREC_STRUCT *recptr);
	static void DelTBRec(INT16U numrec);
	static INT16S GetTBRecByName_FULL(INT8U len, INT8U *name);
	static INT16U GetTBNameByTel(INT8U *ptr, INT8U len, INT8U *tel);
	static INT16U GetTBRec(INT16U numrec, TBREC_STRUCT *recptr);
	static INT16U GetTBItem(void);
	static void DelDB_TB(void);
	static INT16U GetTBBlankItem(void);
	static void DelTalkRecord(INT8U talktype, INT16U numrec);
	static INT16U GetTalkRecord(INT8U talktype, INT16U numrec, TALKREC_STRUCT *recptr);
	static INT16U GetNumTalkRecord(INT8U talktype);
	static void DelAllTalkRecord(INT8U talktype);
	void SaveTKToFile(INT8U talktype, TALKREC_STRUCT *recptr);
	static bool SaveHstConfig();
	//ShortMsg
	static void AddSMRec(SMREC_STRUCT *smrec);
	static void DelSMRec(INT16U numrec);
	static INT16U GetSMRec(INT16U numrec, SMREC_STRUCT *ptr);
	static void DelAllSMRec(void);

/****************ShortMsg********************************************************/
	static void ACK_EraseSM(void);
	static void EraseSM(void);
	static void TransmitSM(void);
	static void WriteBackSM(void);
	static void SaveTel(void);
	static void DialTel_SM(void);
	static void Handler_KEY_ACK_CONTENT(void);
	static INT16U MaxRecordProc_CONTENT(void);
	static INT16U ReadRecordProc_CONTENT(INT16S numrec, INT8U *ptr);
	static void Handler_KEY_ACK_SM(void);
	static INT16U MaxRecordProc_SM(void);
	static INT16U ReadRecordProc_SM(INT16S numrec, INT8U *ptr);
	static void ReadSM(void);
	static void SendInformer(INT8U result);
	static void GetTelFromTB(INT8U tellen, INT8U *tel, INT8U namelen, INT8U *name);
	static void ExitTelEditor_SM(void);
	static void AckSMEditor(void);
	static INT16U CurLenProc(INT16U len, INT8U *ptr);
	static INT16U MaxLenProc(INT16U len, INT8U *ptr);
	static void WriteSM(void);
	static void ReadSMSTel(void);
	static void ACK_DelAll(void);
	static void DelAll(void);
	static void    ActivateReadShorMsgMenu(void);
	static void CreateSMEditor(void);
	static void CreateTelEditor(void);
	static INT8U HaveUnReadSMRec(void);
	static void ClearSMRecUnReadFlag(INT16U numrec);
	static INT16U GetSMRecItem(void);
	static INT8U SMRecUnRead(INT16U numrec);
	static INT8U GetSMRecTel(INT16U numrec, INT8U *ptr);
	void SendSMToCustomer();

/****************Succor********************************************************/
	static void PromptResult(INT8U type, INT8U result);
	static void Informer_Medical(INT8U result);
	static void Informer_Trouble(INT8U result);
	static void Informer_Information(INT8U result);
	static void StartSuccor(INT8U type);

/****************SetGPRS********************************************************/
	static void Set_GPRS_PARA(INT8U type, INT8U len, INT8U *ptr);
	
	static void SetTCPIP1(void);		
	static void SetUDPIP1(void);
	static void SetTCPPort1(void);	
	static void SetUDPPort1(void);
	static void ACK_SetTCPIP1(void);
	static void ACK_SetUDPIP1(void);
	static void ACK_SetTCPPort1(void);
	static void ACK_SetUDPPort1(void);

	static void SetTCPIP2(void);		
	static void SetUDPIP2(void);
	static void SetTCPPort2(void);	
	static void SetUDPPort2(void);
	static void ACK_SetTCPIP2(void);
	static void ACK_SetUDPIP2(void);
	static void ACK_SetTCPPort2(void);
	static void ACK_SetUDPPort2(void);

	static bool CheckIPValid(INT8U len);

	static bool FilterProc(INT8U *ch);
	static void ACK_SetAPN(void);
	static void SetAPN(void);
	static void ACK_SetTel(void);
	static void SetTel(void);
	static void ACK_SetListenTel(void);
	static void SetListenTel(void);
	static void SetCdmaName(void);
	static void SetCdmaPass(void);
	static void ACK_SetCdmaName(void);
	static void ACK_SetCdmaPass(void);
	static void Activate(void);
	static void ACK_InputPassword(void);
	static void InputPassword(void);
	static void SetLiuIP();
	static void SetLiuPort();

/****************SysSet********************************************************/
	static void ReadVersion(void);
	//�Աȶȵ���
	static void DisplayProc_AD(void);
	static void TurnUp(void);
	static void TurnDown(void);
	static void HdlKeyProc_AD(void);
	static void ActivateADConst(void);
	void    InitADConst(void);
	static void    AdjustContrast(void);
	//������
	static void Handler_KEY_ACK_Ring(void);
	static void Handler_KEY_NAK_Ring(void);
	static void Handler_KEY_SEL_Ring(void);
	static INT16U MaxRecordProc_Ring(void);
	static INT16U InitRecordProc_Ring(void);
	static INT16U ReadRecordProc_Ring(INT16S numrec, INT8U *ptr);
	static void SelRing(void);
	//���ð�����
	static INT16U InitRecordProc_KeyPT(void);
	static void StoreProc_KeyPT(void);
	static void ActivateSelOptionMenu(void);
	//���ý�����ʽ
	static INT16U InitRecordProc_Pick(void);
	static void StoreProc_Pick(void);
	static void ActivateSelOptionMenu_Pick(void);
	//����ȱʡģʽ
	static INT16U InitRecordProc_Cha(void);
	static void StoreProc_Cha(void);
	static void ActivateSelOptionMenu_Cha(void);

/****************ParaChk********************************************************/
	static void GetCenterNo(void);
	static void GetGPRSStatus(void);
	static void GetGPSStatus(void);

/*****************AtmpMsg*******************************************************/
	static void AddATMPRec(ATMPREC_STRUCT *atmprec);
	static INT16U GetATMPRec(INT16U numrec, ATMPREC_STRUCT *ptr);
	static INT16U GetATMPRecItem(void);
	static INT16U MaxRecordProc_CONTENT_Atmp(void);
	static INT16U ReadRecordProc_CONTENT_Atmp(INT16S numrec, INT8U *ptr);
	static void Handler_KEY_ACK_Atmp(void);
	static INT16U MaxRecordProc_Atmp(void);
	static INT16U ReadRecordProc_Atmp(INT16S numrec, INT8U *ptr);
	static void ReadATMP(void);

/*****************PhoneCallMsg*******************************************************/
//	static void AddPCRec(ATMPREC_STRUCT *atmprec);
//	static INT16U GetPCRec(INT16U numrec, ATMPREC_STRUCT *ptr);
//	static INT16U GetPCRecItem(void);
	static INT16U MaxRecordProc_CONTENT_PC(void);
	static INT16U ReadRecordProc_CONTENT_PC(INT16S numrec, INT8U *ptr);
	static void Handler_KEY_ACK_PC(void);
	static INT16U MaxRecordProc_PC(void);
	static INT16U ReadRecordProc_PC(INT16S numrec, INT8U *ptr);
	static void ReadPC(void);

/*****************Oil*******************************************************/
	static void SetQuerterBDOFF();
	static void SetHalfBDOFF();
	static void SetThreeFourBDOFF();
	static void SetFullBDOFF();
	static void SetZeroBDOFF();
	static void SetFullBD();
	static void SetThreeFourBD();
	static void SetHalfBD();
	static void SetQuerterBD();
	static void SetZeroBD();
	static void SndBDData();
	static void BiaoDEnd();
	static void BiaoDStart();
	static void SndOilDataDesire();
	
/*****************VideoView*******************************************************/
	static void SwhChn1View();
	static void SwhChn2View();
	static void SwhChn3View();
	static void SwhChn4View();
	static void SwhChnAllView();

/*****************ProjectMenu*******************************************************/
	static void UpdateApp();
	static void ResumeSet();
	static void FormatSDisk();
	static void UpdateSys();
	static void TestOilCut();
	static void InputVideo();
	static void OutputUData();
	static void PCSoundTest();
	static void TestCAM1();
	static void TestCAM2();
	static void TestCAM3();
	static void TestCAM4();
	static void ChkSensorState();
	static void TestAlarm();
	static void ChkLED();
	static void ChkGPS();

	
	static void Settemiver();
	static void ACK_Settemiver();
	static void Setcompanyid();
	static void ACK_Setcompanyid();
	static void Setgroupid();
	static void ACK_Setgroupid();
	static void Set_CGV_PARA(INT8U type, INT8U len, INT8U *ptr);
	static void Loginliu();

	static void InputPasswordToProject();
	static void ACK_InputPasswordProject();

public:	//AuxFunc
	static void DelTBRecByName(char *v_pName);
	static int CheckNameExist(byte v_byLen,char *v_pName);
	void    ActivateVirPhone_Ring(void);

	static INT8U HexToChar(INT8U sbyte);
	static INT8U DecToAscii(INT8U *dptr, INT16U data, INT8U reflen);
	static INT8U FormatYear(INT8U *ptr, INT8U year, INT8U month, INT8U day);
	static INT8U FormatTime(INT8U *ptr, INT8U hour, INT8U minute, INT8U second);
	static bool SearchGBCode(INT8U *ptr, INT16U len);
	static INT8U ASCIIToGsmCode(INT8U incode);
	static INT16U TestGBLen(INT8U *ptr, INT16U len);
};


#endif

