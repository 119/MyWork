#ifndef _YX_DRIVERECORD_H_
#define _YX_DRIVERECORD_H_

#if USE_DRVREC == 1

#define CAR_RUNNING			0
#define CAR_STOP_RUN		1
#define CAR_RUN_STOP		2
#define CAR_STOP			3
#define UnLog				0 
#define Logined				1
#define Inputing			2
#define MAX_DRIVER			10		//���˾����
#define MAX_PROMPT			3		//���˾�����Ѵ���
#define CONTINUE_TIME		3		//��ʻ��¼�������ʻʱ��(Сʱ),���ڴ�ֵ������
#define VT_LOW_RUN			2		//�жϳ���ֹͣ����С�ٶ�(����/ʱ)
#define RUN_MINTIME			5		//�жϳ����ƶ�����Сʱ��(��)
#define STOP_MINTIME		20		//�жϳ���ֹͣ����Сʱ��(��)
#define SND_TYPE			1		// 1,ÿ֡����,��һ�����ڵȴ�Ӧ��   2,����һ�����ڲŷ���;

// ��ʻ��¼�ṹ
STRUCT_1 RcdDrv {
	
	// ˾�����,ȡֵ��Χ0~126
	byte numdrv;
	
	// ���ʱ��;�γ��
	byte s_time[5];
	byte s_lon[4];
	byte s_lat[4];
	
	// �յ�ʱ��;�γ��
	byte e_time[5];
	byte e_lon[4];
	byte e_lat[4];
	
	// ��ʻ���
	ushort space;
	
	void set_start_time()
	{
		struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);
		
		s_time[0]	= (byte)(pCurrentTime.tm_year-100);
		s_time[1]	= (byte)(pCurrentTime.tm_mon+1);
		s_time[2]	= (byte)pCurrentTime.tm_mday;
		s_time[3]	= (byte)pCurrentTime.tm_hour;
		s_time[4]	= (byte)pCurrentTime.tm_min;
	}	
	
	void set_end_time()
	{
		struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);
		
		e_time[0]	= (byte)(pCurrentTime.tm_year-100);
		e_time[1]	= (byte)(pCurrentTime.tm_mon+1);
		e_time[2]	= (byte)pCurrentTime.tm_mday;
		e_time[3]	= (byte)pCurrentTime.tm_hour;
		e_time[4]	= (byte)pCurrentTime.tm_min;
	}	
	
	void set_start_gps()
	{
		tagGPSData gps(0);
		GetCurGps(&gps, sizeof(gps), GPS_LSTVALID);
		
		double db = 60*(gps.latitude-byte(gps.latitude));
		s_lat[0] = byte(gps.latitude);
		s_lat[1] = (byte)db;
		s_lat[2] = byte(long(db*100)%100);
		s_lat[3] = byte(long(db*10000)%100);
		
		db = 60*(gps.longitude-byte(gps.longitude));
		s_lon[0] = byte(gps.longitude);
		s_lon[1] = (byte)db;
		s_lon[2] = byte(long(db*100)%100);
		s_lon[3] = byte(long(db*10000)%100);
	}
	
	void set_end_gps()
	{
		tagGPSData gps(0);
		GetCurGps(&gps, sizeof(gps), GPS_LSTVALID);
		
		double db = 60*(gps.latitude-byte(gps.latitude));
		e_lat[0] = byte(gps.latitude);
		e_lat[1] = (byte)db;
		e_lat[2] = byte(long(db*100)%100);
		e_lat[3] = byte(long(db*10000)%100);
		
		db = 60*(gps.longitude-byte(gps.longitude));
		e_lon[0] = byte(gps.longitude);
		e_lon[1] = (byte)db;
		e_lon[2] = byte(long(db*100)%100);
		e_lon[3] = byte(long(db*10000)%100);
	}
	
};

// �¹��ɵ��¼�ṹ
STRUCT_1 RcdDoub {
	byte stoptype;		//ֹͣ����
	byte numdrv;		//˾�����
	byte times[6];		//��¼ʱ��
	struct Gps {
		byte lon[4];
		byte lat[4];
	} pos[20];			//ǰ20��λ����Ϣ
	byte vector[100];	//ǰ100���ٶ�
	byte sensor[100];	//ǰ100���������ź�
	
	void set_data(bool setgps, byte sens)
	{	
		tagGPSData gps(0);
		GetCurGps(&gps, sizeof(gps), GPS_LSTVALID);
		
		//����ϴ��ٶ��Ѿ�Ϊ0,ͬʱ��ǰ�ٶ�ҲΪ0,���ٲɼ�
		if(0==vector[0] && 0==gps.speed)	return;
		
		int i = 0;
		if(setgps) {
			for(i=19; i>=1; i--) pos[i] = pos[i-1];
			
			double db = 60*(gps.latitude-byte(gps.latitude));
			pos[0].lat[0] = byte(gps.latitude);
			pos[0].lat[1] = (byte)db;
			pos[0].lat[2] = byte(long(db*100)%100);
			pos[0].lat[3] = byte(long(db*10000)%100);
			db = 60*(gps.longitude-byte(gps.longitude));
			pos[0].lon[0] = byte(gps.longitude);
			pos[0].lon[1] = (byte)db;
			pos[0].lon[2] = byte(long(db*100)%100);
			pos[0].lon[3] = byte(long(db*10000)%100);
		}
		for(i=99; i>=1; i--)	vector[i] = vector[i-1];
		vector[0] = byte( ((gps.speed*3600)/1000)/1.852 );
		for(i=99; i>=1; i--)	sensor[i] = sensor[i-1];
		sensor[0] = sens;	
	}
	
	void set_time()
	{
		struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);
		
		times[0] = (byte)(pCurrentTime.tm_year-100);
		times[1] = (byte)(pCurrentTime.tm_mon+1);
		times[2] = (byte)pCurrentTime.tm_mday;
		times[3] = (byte)pCurrentTime.tm_hour ? (byte)pCurrentTime.tm_hour : 0x7f;
		times[4] = (byte)pCurrentTime.tm_min  ? (byte)pCurrentTime.tm_min  : 0x7f;
		times[5] = (byte)pCurrentTime.tm_sec  ? (byte)pCurrentTime.tm_sec  : 0x7f;
	}
};


// 360Сʱ����ʻ������
STRUCT_1 Meter360 {
	double _space;			//��ǰ2��Сʱ���ۼ����
	double space[180];		//360�ڵ��������
	double get_meter() {
		double ret = _space;
		for(int i=0; i<180; i++)	ret += space[i];
		return ret;
	}
};

// 2���������ڵ���ʻ������
STRUCT_1 Meter2Day {
	double _space;			//10�����ڵ��ۼ����
	byte date[3][3];		//����
	double space[3];		//3�����ʻ�����
	double get_meter() {
		return (space[2]+space[1]);
	}
};

// ��ʻ��¼��ͷ�ļ��ṹ
STRUCT_1 HRcdFile {
	
	ulong ticks;			//��ʱ����(��)
	
	int numdrv;				//��ǰ˾�����
	int stacar;				//��ǰ������״̬(ֻ������״̬,�ƶ��;�ֹ)
	
	int stacur;				//��ǰ����״̬(����״̬)
	int stapre;				//�ϴγ���״̬(����״̬)
	int run;				//����������ʻʱ��(��)
	int stop;				//��������ֹͣʱ��(��)
	
	int inputflag;			//˾����½���(����������)
	
	RcdDrv rcddrv;			//��ǰ����ʻ��¼
	RcdDoub rcddoub;		//��ǰ���¹��ɵ��¼
	
	byte speed;				//��ǰ�ٶ�(����/ʱ)
	byte vector[60];		//���60����ٶ�(����/ʱ)
	byte average[15];		//���15���ӵ�ƽ���ٶ�
	
	double lstlon,lstlat;	//���һ����Ч�ľ�γ��
	double metercur;		//������ʻ�����(m)
	double metertotal;		//�����(m)
	byte metertime[3];		//��ʼ�����ͳ��ʱ��(������)
	
	Meter360 m360;			//360Сʱ��̼�¼
	Meter2Day m2d;			//2����������̼�¼
	
	ulong drvcnt;			//��ʻ��¼����
	ulong drvpw;			//��ʻ��¼дָ��
	ulong doubcnt;			//�¹��ɵ��¼����
	ulong doubpw;			//�¹��ɵ��¼дָ��
	
	byte almtime[20][4];	//��ʻʱ���
	ushort r_used;			//��������ǰ���˶��ٿ�����
	ushort d_used;			//��������ǰ���˶��ٸ���
	
	byte rp_oil[5];			//��־�յ� �����ϴ���ʻ���ݵ�Ӧ��
	ulong uptime;			//�������ϱ���ʱ��
	int st_clt;				//���ͺ������ϱ��ɼ�״̬
	
	byte updoub;			//�Ƿ���Ҫ�ϱ��ɵ����ݵı�־, 
	//1:�ϱ����һ����ʽͣ������ 2:�ϱ�����δ�ϱ���ͣ������
	int tickdoub;			//�¹��ɵ�ͣ��ʱ�����
	byte rp_doub[2];		//��־�յ� ʵʱ�¹��ɵ��ϴ�Ӧ��
	
	
	int st_log;				//˾����½״̬	
	int prompt;				//�Ƿ���������ʾ����
	bool alarmed;			//�Ƿ������Ƿ񱨾����ı�־
	
	void init()
	{
		memset(this, 0, sizeof(*this));
		
		numdrv = 0;
		stacar = CAR_STOP;
		
		stacur = CAR_STOP;
		stapre = CAR_STOP;
		run = 0;
		stop = 0;
		
		inputflag = -2;
		
		memset(vector, 0, 60);
		memset(average, 0, 15);	
		
		struct tm pCurrentTime;
		G_GetTime(&pCurrentTime);

		metertime[0] = (byte)(pCurrentTime.tm_year-100);
		metertime[1] = (byte)(pCurrentTime.tm_mon+1);
		metertime[2] = (byte)pCurrentTime.tm_mday;
		
		prompt = 0;
		alarmed = false;
		st_log = UnLog;
	}
	
};

// ��������
STRUCT_1 Range {
	ushort areano;			//������
	byte attr;				//��������
	byte vector;			//�����ٶ�
	ushort cnt;				//����Ķ������
	ushort n_start;			//��ʼ�����
	ushort n_end;			//��ֹ�����
};

// ��ʻ��¼���ļ��ṹ
STRUCT_1 RcdFile {
	HRcdFile head;
	byte reserved[20*1024-sizeof(HRcdFile)];		//head�ı����ռ�
	
	RcdDrv drv[200];		//��ʻ��¼
	RcdDoub doub[100];		//�¹��ɵ��¼
	
	Range range[200];		//��������
	byte dot[800][8];		//��������
};

//-------------������Э��֡------------------
// ƣ����ʻָʾ
STRUCT_1 frm3647 {
	byte numdrv;		//˾�����
	byte s_time[5];		//��ʼʱ��
	byte s_lon[4];		//��㾭��
	byte s_lat[4];		//���γ��
	byte timed;			//����ʻʱ��(Сʱ)
};

// ��ѯ������ʻʱ���¼����
STRUCT_1 frm3606 {
	byte numdrv;		//˾�����
	byte times;			//������ʻʱ��
};

// ��ѯ������ʻʱ���¼Ӧ��
STRUCT_1 frm3646 {
	byte reply;			//Ӧ������
	byte index;			//��ǰ֡���
	byte count;			//��¼����
	RcdDrv rcd;			//��¼
};

// ��ѯ���15������ÿ����ƽ���ٶ�Ӧ��
STRUCT_1 frm3645 {
	byte reply;			//Ӧ������
	byte average[15];	//ÿ����ƽ���ٶ�(����/Сʱ)
};

// ��ѯ�¹��ɵ�����Ӧ��1(�ٶȺͿ������ź�)
STRUCT_1 frm36441 {
	byte reply;			//Ӧ������
	byte index;			//��ǰ֡���
	byte attr;			//�¹��ɵ���������
	byte numdrv;		//˾������
	byte times[6];		//ͣ��ʱ��ʱ��
	byte vecsen[50][2];	//�ٶȺͿ������ź�
};

// ��ѯ�¹��ɵ�����Ӧ��2(λ��)
STRUCT_1 frm36442 {
	byte reply;			//Ӧ������
	byte index;			//��ǰ֡���
	byte attr;			//�¹��ɵ���������
	byte numdrv;		//˾������
	byte times[6];		//ͣ��ʱ��ʱ��
	byte gps[10][8];	//λ��
};

// ����̲�ѯ����Ӧ��
STRUCT_1 frm0941 {
	byte reply;			//Ӧ������
	byte time[3];		//��ʼʱ��
	byte len;			//�����Ϣ����
	char inf[126];		//�����Ϣ
};

// ��̼�¼��ѯ
STRUCT_1 frm0905 {
	byte type;			//ʱ�䷶Χ����
	byte s_time[5];		//��ʼʱ��
	byte e_time[5];		//��ֹʱ��
	byte numdrv;		//˾������
};

// ��̼�¼��ѯӦ��
STRUCT_1 frm0945 {
	byte reply;			//Ӧ������
	byte index;			//��ǰ֡���
	byte count;			//��¼����
	byte numdrv;		//˾������
	byte bit_g4:1;		//HBIT
	byte bit_g3:1;
	byte bit_g2:1;
	byte bit_g1:1;		
	byte bit_g0:4;		
	byte s_time[5];		//���ʱ��
	byte s_lon[4];		//��㾭��
	byte s_lat[4];		//���γ��
	byte e_time[5];		//�յ�ʱ��
	byte e_lon[4];		//�յ㾭��
	byte e_lat[4];		//�յ�γ��
	byte space[2];		//��ʻ���
};

// ������ʻʱ�������
STRUCT_1 frm3701 {
	byte ctl;			//������
	byte cnt;			//���ø���
	byte time[20][5];	//ʱ���
};

// ���ñ�����������1
STRUCT_1 frm37021 {
	ushort frmno;		//֡���
	byte ctl;			//������
	ushort areano;		//������
	byte attr;			//��������
	byte vector;		//�����ٶ�
	ushort cnt;			//��������
	byte dot[200][8];	//�����
};

// ���ñ�����������2
STRUCT_1 frm37022 {
	ushort frmno;		//֡���
	byte ctl;			//������
	byte dot[200][8];	//�����
};

// ���ñ�������Ӧ��
STRUCT_1 frm3742 {
	byte reply;			//Ӧ������
	ushort frmno;		//����֡���
};

// ���ö�ʱ�ϴ���ʻ����
STRUCT_1 frm103e {
	byte up;			//�������ݱ�ʶ
	byte sta_id;		//״̬��ʶ
	ushort uptime;		//�ϱ���ʱ��
	ushort collect;		//��������
	ushort send;		//��������
};

STRUCT_1 Upload3747Inf {
	ushort numdrv;		//˾�����
	byte lat[4];		//γ��
	byte lon[4];		//����
	byte speed;			//�ٶ�
	byte dir;			//����
	byte sensor;		//������״̬
	byte state[2];		//״̬��
	byte sensor2;		//����������״̬
	byte ct_load;		//���ش���������L
	ushort ad_load[4];	//����ADֵ
};

// �����ϴ���ʻ����(���ͺ�)
STRUCT_1 frm3747 {
	byte reply;			//Ӧ������
	byte w_size;		//���ʹ��ڴ�С
	byte frmno;			//֡���
	
	ulong meter;		//��ǰ�����
	byte ad_oil;		//�ͺ�ad
	byte vo_oil;		//�ͺĵ�ѹ
	byte st_oil;		//�ͺ�״̬��
	ushort tm_spc;		//���ʱ��
	byte ct_gps;		//GPS���ݸ���
	byte date[3];		//����
	byte time[3];		//ʱ��
	Upload3747Inf inf[4];
	byte ct_avrg;			//1����ƽ���ٶȵ����
	byte avrg[256];			//1������ƽ���ٶ�
	
	int mkfrm() {
		byte tmp = ct_avrg;
		memcpy(&inf[ct_gps], &ct_avrg, tmp+1);
		return ((byte*)&inf[ct_gps] - &reply + tmp + 1);
	}
};

STRUCT_1 Upload3749Inf {
	ushort numdrv;		//˾�����
	byte lat[4];		//γ��
	byte lon[4];		//����
	byte speed;			//�ٶ�
	byte dir;			//����
	byte sensor;		//������״̬
	byte state[2];		//״̬��
	byte sensor2;		//����������״̬
	byte ct_load;		//���ش���������L
	ushort ad_load[4];	//����ADֵ
	byte m_bytOilSta;	 // �ͺ�״̬��
	ushort m_wValidOilAD; //�ͺ�AD
	ushort m_wValidPowtAD; //��ѹAD
};

STRUCT_1 frm3749 {
	byte reply;			//Ӧ������
	byte w_size;		//���ʹ��ڴ�С
	byte frmno;			//֡���
	
	ulong meter;		//��ǰ�����
	ushort tm_spc;		//���ʱ��
	byte ct_gps;		//GPS���ݸ���
	byte date[3];		//����
	byte time[3];		//ʱ��
	byte m_bytKuoZhanSta1;
	byte m_bytKuoZhanSta2;
	Upload3749Inf inf[4];
	byte ct_avrg;			//1����ƽ���ٶȵ����
	byte avrg[256];			//1������ƽ���ٶ�
	
	int mkfrm() {
		byte tmp = ct_avrg;
		memcpy(&inf[ct_gps], &ct_avrg, tmp+1);
		return ((byte*)&inf[ct_gps] - &reply + tmp + 1);
	}
};

// ʵʱ�¹��ɵ��ϴ�
STRUCT_1 frm0157 {
	byte trantype;		//��������
	byte e_time[6];		//ͣ��ʱ��
	struct Doub {
		byte lat[4];		//γ��
		byte lon[4];		//����
		byte speed;			//�ٶ�
		byte sensor;		//������
	} doub[10];			//�¹��ɵ���Ϣ
	byte doubtype;		//�¹��ɵ�����
};


//---------------�������Э��֡----------------
// �ٶ�����Ӧ��
STRUCT_1 frm60 {
	byte cmd;
	byte reply;			//Ӧ������
	byte speed;			//�ٶ�(����/ʱ)
};

// ����˾����¼Ӧ��
STRUCT_1 frm63 {
	byte cmd;
	byte reply;			//Ӧ������
	ushort numdrv;		//˾�����
};

// ����˾�����Ӧ��
STRUCT_1 frm64 {
	byte cmd;
	byte reply;			//Ӧ������
	ushort numdrv;		//���
	byte asclen;		//��ݴ��볤��
	char ascii[7];		//��ݴ���
};

STRUCT_1 tag0f12Area
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

//////////////////////////////////////////////////////////////////////////

class CDriveRecord
{
public:
	CDriveRecord();
	virtual ~CDriveRecord();

	int Init();
	int ReInit();
	int Release();

	void P_ThreadDrvRecWork();
	void P_TmDrvRecTimer();

	bool ChkDriverLog(char *v_pStrDrvNum);
	void hdl_diaodu(byte* data, int len);

	unsigned long GetTotleMeter();

private:
	void init_filecfg();

	// Э�鴦����
	void deal_center();
	void hdl_0117(byte* data, int len);
	void hdl_0127(byte* data, int len);	
	void hdl_0901(byte* data, int len);
	void hdl_0902(byte* data, int len);
	void hdl_0904(byte* data, int len);
	void hdl_0905(byte* data, int len);	
	void hdl_103e(byte* data, int len);	
	void hdl_3601(byte* data, int len);
	void hdl_3602(byte* data, int len);
	void hdl_3603(byte* data, int len);
	void hdl_3604(byte* data, int len);
	void hdl_3605(byte* data, int len);
	void hdl_3606(byte* data, int len);
	void hdl_3611(byte* data, int len);
	void hdl_3613(byte* data, int len);
	void hdl_3618(byte* data, int len);	
	void hdl_3619(byte* data, int len);	
	void hdl_3623(byte* data, int len);	
	void hdl_3701(byte* data, int len);
	void hdl_3702(byte* data, int len);
	void hdl_3707(byte* data, int len);
	void hdl_3709(byte *data, int len);
	void hdl_0f01(byte* data, int len);
	void hdl_0f02(byte* data, int len);	
	void hdl_0f03(byte* data, int len);
	void hdl_0f11(byte* data, int len);
	void hdl_0f12(byte* data, int len);
	
	// ������ʱ����
	void deal_login();
	void deal_state();
	void deal_meter(tagGPSData gps);
	void deal_alarm(tagGPSData gps);
	void deal_area(tagGPSData gps);		// Խ������0F Э�����ͣ�
	void deal_updrv(tagGPSData gps);
	void deal_query();
	
	bool chk_doubrcd(RcdDoub *rcd, byte timecnt, byte time[10][10], byte regioncnt, byte region[5][16]);
	bool chk_drvrcd(RcdDrv *rcd, byte numdrv, byte times);
	bool chk_drvrcd2(RcdDrv *rcd,byte type, byte *s_time, byte *e_time, byte numdrv);
	void send_3644(RcdDoub *rcd, byte index, byte reply);
	
#if USE_OIL == 3
	void fill_frm3749(frm3749 &frm, int c_snd, ushort cltcyc, ushort sndcyc, tagGPSData gps);
#else
	void fill_frm3747(frm3747 &frm, int c_snd, ushort cltcyc, ushort sndcyc, tagGPSData gps);
#endif
	
	bool flush_headrcd();
	bool flush_doubrcd();
	bool flush_drvrcd();
	
	void set_car_state(tagGPSData gps);
	bool chk_overspeed();
	bool chk_belowspeed();
	bool chk_overtired(int minutes);
	bool alarm_overtired(int minutes);
	
	int get_minutes(int year, int month, int day, int hour, int minute, 
		int year2, int month2, int day2, int hour2, int minute2 );
	
	void tcp_send(byte trantype, byte datatype, void *data, int len);
	void req_login();
	void req_unlog();
	int  get_drvindex(int numdrv);
	
	bool cut_line(byte *dot1, byte *dot2, byte *dot0);
	
	ulong get_lstdoub(byte updoub, RcdDoub &rd);
	void set_doubtype(ulong pw);
	
	void set_alarm(ulong type, bool set);
	bool acc_valid();
	bool car_load();
	bool car_move();
	byte get_sensor();
	void show_diaodu(char *v_szbuf);
	void play_tts(char *v_szbuf);

public:		
	Queue tcprcv;			//TCP���ն�ջ	
	bool m_bLcdLog;			//�Ƿ����LCD��־
	bool m_bNeedClt; 
	byte m_bytAreaCode;
	bool m_bDrvRecQuit;		// ����root����ʱ��Ҫ�˳��¹��ɵ�ɼ��߳�

#if USE_LIAONING_SANQI == 1
	ushort TotalMeter;
	ulong  Mile;
#endif
	HRcdFile m_v;			//������Ա����
private:
	pthread_t m_pthreadWork;

	DrvCfg m_cfg;			//��ʻ����

	tag2QAreaCfg m_objAreaCfg;
	
	Queue m_quedoub;
	Queue m_quedrv;
	Queue m_quemeter;
	
	
};

#endif

#endif





