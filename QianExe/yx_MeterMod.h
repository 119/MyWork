#ifndef _METERMOD_H_
#define _METERMOD_H_

class CMeterMod
{
public:

	CMeterMod();
	virtual ~CMeterMod();

private:

#pragma pack(push,1)

	struct PointInfo
	{
		byte date[3];	// ����
		byte s_tm[2];	// ���ʱ��
		byte s_lat[4];	// ���γ��
		byte s_lon[4];	// ��㾭��
		byte e_tm[2];	// �յ�ʱ��
		byte e_lat[4];	// �յ�γ��
		byte e_lon[4];	// �յ㾭��
	
		PointInfo() 	{ memset(this, 0, sizeof(*this)); }
	};
	
	
	struct Up2022 
	{
		byte num;		// ���I
		byte id[2];		// ����ID
		byte cnt;		// ��Ӫ���ݸ���
		byte ptif[23];	// ����յ�λ����Ϣ
		byte len;		// ��Ӫ���ݳ���
		byte data[60];	// ��Ӫ����
		
		Up2022()
		{
			memset(this, 0, sizeof(*this));
			num = 1;
			id[0] = 0x01;
			id[1] = 0x02;
			cnt = 1;
			len = 60;
		}
	};
	
	
	
#pragma pack(pop)

private:
	pthread_t m_hInvalidCarry;
	pthread_t m_hthJijiaRead;
	pthread_t m_hthJijiaWork;
	int m_hJijia;		// ���ھ��

	PointInfo ptif;		// ����յ㶨λ��Ϣ
	Queue que_up;		// ��Ӫ������ʱ��Ŷ���
	bool must_up;		// ��Ӫ�����ϴ���־
	volatile bool m_bCarrySkip;//�ؿͲ����״̬�ؼ�

public:
	Queue tcprcv;	// TCP���ն�ջ	
	
public:
	int  Init();
	int  Release();
	
	bool JijiaComOpen();
	
	DWORD JijiaRead();
	DWORD JijiaWork();
	DWORD InvalidCarry();
	int Deal1015( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM );
	int Deal0119( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM );
	void DealComBuf(char *v_szBuf, int v_iLen);
	
private:
	void _DealComChar(byte ch);
	void _DealCom(byte* frm, int len);
	bool _ComSend(byte cmd, byte* data=NULL, int len=0);
	void _TcpSend(byte trantype, byte datatype, byte *data, int len);
	void _Work();
	bool _ChkAck(byte datatype);
	byte _DealCentData(byte type, byte *data, int len);
	void _SetFull(PointInfo *p);
	void _SetEmpty(PointInfo *p);

	bool _CheckSta();
	void _InvCarryRpt(const tagGPSData &v_objGps);
	void _Snd0159( tagQianGps &v_objQianGps );

};


#endif	//#define _METERMOD_H_

