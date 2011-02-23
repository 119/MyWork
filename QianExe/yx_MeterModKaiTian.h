#ifndef _METERMOD_KAITIAN_H_
#define _METERMOD_KAITIAN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define UPLOAD_CNT	  5			//һ���ϴ�������Ӫ����
#define TAXIDATASIZE   42        //һ����Ӫ���ݵĴ�С
#define TAXIFILEPACKSIZE  64    //�����ļ���ÿ���Ĵ�С����

#define TOTALDATACNT 200		//��Ӫ���ݺ�ˢ�����ݴ洢����  ˢ������:200*5*42  ��Ӫ����:200*19 


#define ALABUF				1500		//�շ������� 
#define SERIALNUMBER  0x7006	//��̨���
 
#define  WAITPERIOD   120000  // �ȴ�����
#define  DOWNPERIOD   60000   // ����ҵ��ʱ��--zzg debug

#define  MAX_RESEND   5
  
#define TAXRECV       0x00
#define TAXOK         0x01 

#define TAXIFILEPATH				(L"\\ResFlh2\\TaxiFile")//�Ƽ����ļ�
#define TAXIDATAPATH				(L"\\ResFlh2\\TaxiData")//һ��ͨˢ������
#define METERDATAPATH				(L"\\ResFlh2\\MeterData")//������Ӫ����
//��Ӫ���ݲɼ�״̬
#define METERDATA_GPS				0x0001	//λ����Ϣ
#define METERDATA_DATA			0x0002	//������Ϣ
#define METERDATA_SERVALUE	0x0004	//��������
#define METERDATA_COMPLETE	0x0006 //���ݲɼ����

extern Queue g_MeterRD;	     // ���ն�ջ
extern Queue g_MeterSND;	 //���Ͷ�ջ
//extern Queue stack;        //�Ƽ����복̨


class CKTMeterMod  
{
private:
typedef struct TaxiFile
{
  INT8U 	 ChkSum;
  INT8U    id;                      // �����ļ�id
  bool     flag;                    // ���ر�־λ
  bool     renewflag;               // ����״̬λ
  INT16U   no;                     // �����ļ��ܰ���
  INT8U    endlen;                  // ���һ����С
	TaxiFile()
	{
		memset(this, 0, sizeof(*this));
		flag=false;
		renewflag=false;
	}
}TAXFILE_STRUCT;

#pragma pack(push,1)

struct DataHead
{
	int curIndex;  //��ǰ���������ݵ�λ��
	int sendCnt;	 //���������ݵĸ���
	DataHead(){memset(this, 0, sizeof(*this));}
};

struct TaxiFileHead
{
	byte fileID;	//�ļ�ID
	WORD totalCnt;	//�ܰ���
	WORD curNum;	//��ǰ�����
	byte endLen;    //���һ������
	TaxiFileHead(){memset(this, 0, sizeof(*this));}
};

struct frm2025
{
	byte resType;
	byte id;
	byte tolNum[2];
	byte curNum[2];
};

struct Up2027 
{
	byte id[2];		// ����ID
	byte cnt;		// ��Ӫ���ݸ���
	byte type;		// ��Ӫ��������
	byte len;		// ��Ӫ���ݳ���
	byte data[UPLOAD_CNT*TAXIDATASIZE];	// ��Ӫ����
	
	Up2027()
	{
		memset(this, 0, sizeof(*this));
		id[0] = 0x27;
		id[1] = 0x27;
		//cnt = UPLOAD_CNT;
		len = 0;
	}
};

struct MeterData
{
	byte driverInfo[3];//˾����Ϣ
	byte year;//����
	byte month;
	byte day;
	byte inTm[2];//�ϳ�ʱ�䣨ʱ�֣�
	byte outTm[2];//�³�ʱ�䣨ʱ�֣�
	byte heavyTm;//�س�ʱ��
	byte meter[2];//�Ƴ�
	byte time[2];//��ʱ��ʱ�֣�
	byte money[2];//���
	byte emptyMeter[2];//��ʻ���
	MeterData() {memset(this, 0, sizeof(*this));}
};

struct GPSInf
{
	byte date[3];//����
	byte beginTime[2];
	byte beginLat[4];
	byte beginLon[4];
	byte endTime[2];
	byte endLat[4];
	byte endLon[4];
	GPSInf(){memset(this, 0, sizeof(*this));}
};

struct Up2068
{
	GPSInf gpsInf;//GPS��Ϣ
	byte meterDataLen;//��Ӫ���ݳ���
	MeterData meterData;//��Ӫ����
	byte cnt;//�ؿ�����
	byte serValue;//��������
	Up2068(){Init();}
	void Init()
	{
		memset(this,0,sizeof(*this));
		meterDataLen=sizeof(MeterData)+2;//zzg mod 090928
		cnt=1;
	}
};

struct frm4048
{
	byte data[5];//����ʱ��
	byte oldDriver[3];//ԭ˾����Ϣ
	byte curDriverInfo[3];//����˾����Ϣ
	frm4048(){memset(this, 0, sizeof(*this));}
};

#pragma pack(pop)

public:
	CKTMeterMod();
	virtual ~CKTMeterMod();
	
	int Init();
	int Release();
	
	void P_ThreadRecv();
	void P_ThreadWork();
	
	void P_PlayTxt(char *fmt);
	int DealComu48( char* v_szData, DWORD v_dwDataLen );
	int DealComu4A( char* v_szData, DWORD v_dwDataLen );
	
	void BeginMeterRcd();
	void SetSerValue(int v_iValue);
	void Test();
	
	void P_TmDownProc();
	void P_TmSetDeviceProc();

private:
	int m_iComPort;		// ���ھ��
	pthread_t m_pthreadRecv;
	pthread_t m_pthreadWork;
	bool m_bEscRecv;
	bool m_bEscWork;
	
	pthread_mutex_t m_mutexTaxiDataSta;//ˢ�������ļ���д����
	pthread_mutex_t m_mutexMeterDataSta;//��Ӫ�����ļ���д����
	TAXFILE_STRUCT   taxfile;

	Queue que_taxi;		// ˢ��������ʱ��Ŷ���
	Queue que_meter;	// ��Ӫ������ʱ��Ŷ���
	bool must_up;		// �����ϴ���־
	bool last_pack;		// �Ƽ������һ��ˢ������
	bool link;//�Ƽ����Ƿ���������

	Up2068 m_objUp2068;

	bool m_bJijiaEnable;//�Ƽ����Ƿ�����
	byte m_DriverInf[3];
	byte m_DownCt;
  byte m_SendCt;
  byte m_DataType;
	byte m_SendSta;
	WORD m_DataLen;
	WORD m_wMeterSta;//��Ӫ����״̬
	WORD m_CurNo;

	typedef union 
	{
		WORD  word;
		struct 
		{
			byte  low;
			byte  high;
		} bytes;
	} WORD_UNION;

public:
	Queue tcprcv;	// TCP���ն�ջ

private:
	int  _DealComChar(byte * chs,int len);
	void _DealCom(byte* frm, int len);
	bool _ComOpen();
	bool _ComClose();
	bool _ComSend(byte cmd, byte* data=NULL, int len=0);

	void _Work();
	void _TcpSend(byte trantype, byte datatype, byte *data, int len,int cvtType=CVT_NONE);
	bool _ChkAck(byte datatype);
	
	byte hdl_cent(byte type, byte *data, int dataLen);

  void hdl_recv0x00();
	void hdl_recv0x01();
	void hdl_recv0x02();
	void hdl_recv0x03(byte *data);
	void hdl_recv0x04(byte *data, WORD len);
	void hdl_recv0x05();
	void hdl_recv0x06(byte *data);
	void hdl_recv0x07();
	void hdl_recv0x08(byte *data);
	void hdl_recv0x09(byte *data);
	void hdl_recv0x0a(byte *data, WORD Len);
	void hdl_recv0x0b(byte *data, WORD Len);
	void hdl_recv0x0c(byte *data);
	void hdl_recv0x0d(byte *data);
  void hdl_recv2065(byte *Data, WORD Len);
	void hdl_recv2066(byte *Data);
	void hdl_recv2005();//���üƼ���
	void hdl_recv2006();//���üƼ���

	bool _InitTaxiData();
	bool _InitMeterData();
	void _CheckTaxiFile();

	bool _FlushDataHead(byte type,WORD Cnt);
	bool _FlushTaxiDataRcd(byte *Data,WORD Len);
	bool _FlushMeterDataRcd(byte *Data,WORD Len);

	void _SendMeterData();//�ϴ���Ӫ����
	void _SendCashData();//�ϴ�ˢ������

	bool _StorageFileData(byte *Data, WORD Len);
	bool _GetTaxiDataRcd();
	bool _GetMeterDataRcd();
};

#endif 
