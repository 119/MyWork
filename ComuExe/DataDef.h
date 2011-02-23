#ifndef _DATADEF_H_
#define _DATADEF_H_

#define MIN_GPRS_SIGNAL		5				// �ܲ�GPRS����С�ź�ֵ

#pragma pack(push,1)

	struct Frm10 {
		byte type;
		char dtmf;
		Frm10() { type = 0x10; }
	};

	struct Frm11 {
		byte type;
		char reply;
		Frm11() { type = 0x11; }
	};

	struct Frm12 {
		byte type;
		byte len;
		char num[256];
		Frm12() { memset(this, 0, sizeof(*this));  type = 0x12; }
	};

	struct Frm13 {
		byte type;
		char reply;
		Frm13() { type = 0x13; }
	};

	struct Frm14 {
		byte type;
		Frm14() { type = 0x14; }
	};

	struct Frm15 {
		byte type;
		char reply;
		Frm15() { type = 0x15; }
	};

	struct Frm16 {
		byte type;
		Frm16() { type = 0x16; }
	};

	struct Frm17 {
		byte type;
		char reply;
		Frm17() { type = 0x17; }
	};

	struct Frm18 {
		byte type;
		Frm18() { type = 0x18; }
	};

	struct Frm19 {
		byte type;
		byte len;
		char num[256];
		Frm19() { memset(this, 0, sizeof(*this));  type = 0x19; }
	};
	
	struct Frm20 {
		byte type;
		char dtmf;
		Frm20() { type = 0x20; }
	};

	struct Frm21 {
		byte type;
		Frm21() { type = 0x21; }
	};

	struct Frm22 {
		byte type;
		Frm22() { type = 0x22; }
	};

	struct Frm23 {
		byte type;
		Frm23() { type = 0x23; }
	};

	struct Frm24 {
		byte type;
		Frm24() { type = 0x24; }
	};

	struct Frm25 {
		byte type;
		byte level;
		Frm25() { type = 0x25; }
	};
	
	struct Frm26 {
		byte type;
		Frm26() { type = 0x26; }
	};

	struct Frm27 {
		byte type;
		byte reply;
		Frm27() { type = 0x27; }
	};

	struct Frm28 {
		byte type;
		byte dir;
		Frm28() { type = 0x28; }
	};

	struct Frm29 {
		byte type;
		byte reply;
		Frm29() { type = 0x29; }
	};

	struct Frm31 {
		byte type;
		byte reply;
		Frm31() { type = 0x31; }
	};

	struct Frm32 {
		byte type;
		byte reply;
		Frm32() { type = 0x32; }
	};

	// ����״̬֪ͨ 
	struct Frm35 {
		byte type;
		byte reply;		//1:����ָ�(���ųɹ�)  2:����ָ�(Ping�ɹ�)   3:����Ͽ���
		Frm35() { type = 0x35; }
	};

	struct Frm38 {
		byte type;
		byte source;
		char tel[16];
		char msg[160];
		Frm38() { memset(this, 0, sizeof(*this));  type = 0x38; }
	};

	struct Frm39 {
		byte type;
		byte reply;
		Frm39() { type = 0x39; }
	};

	struct FrmF1 {
		byte type;
		byte len;
		char num[256];
		FrmF1() { memset(this, 0, sizeof(*this));  type = 0xF1; }
	};

	struct FrmF2 {
		byte type;
		byte reply;
		FrmF2() { type = 0xF2; reply = 0x00; }
	};

	struct FrmF3 {
		byte type;
		byte reply;
		FrmF3() { type = 0xF3; reply = 0; }
	};

	struct FrmFF {		// add by zyq 090824
		byte type;
		byte iSignal;
		FrmFF() { type = 0xff; iSignal = 0;}
	};

	struct UserSM {
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

	struct UserSM2 {
		char pass[10];		// ����
		char cmd[6];		// ����
	};
	
	//--------���ں�������------------------------
	// �绰��¼
	struct Book {
		byte idx;		//������
		char tel[15];	//�绰����
		char name[20];	//����
		byte limit;		//Ȩ������
		byte tmhold[2];	//ͨ��ʱ��
	};	

	// �绰��������
	struct BookCfg {
		Book book[30];		//�绰��¼	
		char tel[5][15];	//�绰����
	};
	//-----------------------------------------------

#pragma pack(pop)

	
#if USE_LANGUAGE == 0
#define LANG_IOCHK_LIGHT ("��⵽ ���� �źű仯")
#define LANG_IOCHK_SPEAKER ("��⵽ �������� �źű仯")
#define LANG_IOCHK_CENTLOCK_OFF ("��⵽ �п����� �źű仯")
#define LANG_IOCHK_CENTLOCK_ON ("��⵽ �п����� �źű仯")
#define LANG_IOCHK_ALERM_LED ("��⵽ ����ָʾ�� �źű仯")
#define LANG_IOCHK_ALERMFOOT ("��⵽ ���ٱ��� �źű仯")
#define LANG_IOCHK_ACCON ("��⵽ ACC��Ч �źű仯")
#define LANG_IOCHK_ACCOFF ("��⵽ ACC ��Ч �źű仯")
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
#define LANG_IOCHK_JIJIA_VALID ("��⵽ �Ƽ����ź���Ч")
#define LANG_IOCHK_PASSENGER1_VALID ("��⵽ �ؿ�1 �źű仯")
#define LANG_IOCHK_PASSENGER2_VALID ("��⵽ �ؿ�2 �źű仯")
#define LANG_IOCHK_PASSENGER3_VALID ("��⵽ �ؿ�3 �ź���Ч")
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
#define LANG_IOCHK_PASSENGER1_VALID ("IO: Passager load 1 valid")
#define LANG_IOCHK_PASSENGER2_VALID ("IO: Passager load 2 valid")
#define LANG_IOCHK_PASSENGER3_VALID ("IO: Passager load 3 valid")
#endif	//USE_LANGUAGE == 11


#endif	//#define _DATADEF_H_
