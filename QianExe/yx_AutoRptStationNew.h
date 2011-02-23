#ifndef _YX_AUTORPTSTATION_H_
#define _YX_AUTORPTSTATION_H_

#define PACKED_8 __attribute__( (packed, aligned(8)) )
#define _STRUCT_8 struct PACKED_8

#define PACKED_2 __attribute__( (packed, aligned(2)) )
#define _STRUCT_2 struct PACKED_2

struct tagReportStateNew
{
	bool	m_bPlaySound;		// �Ƿ�Ҫ������������
	int  	m_iPlayNo;			// ��������������վ���
	
	tagReportStateNew() 
	{
		memset(this, 0, sizeof(*this)); 
	}
};

struct tagOneStationNew	// վ����� 
{
	int		m_iCurNo;			// ��վ����
	int     m_iUniId;			// վ��ͳһ���
	bool	m_bIsPlayed;		// ��վ�Ƿ��Ѿ�����
	long	m_lLat;				// վ��γ��
	long	m_lLon;				// վ�㾭��
	int		m_iRad;				// ��վ��İ뾶
	float	m_fdirect;			// ��վ��GPS����
	BYTE    m_bytStaAttr;		// վ������: 0x01��ʼ��վ����վ�� 0x02���յ�վ ����վ�� 0x03����;վ 0x04������
	BYTE            m_bytSpeed;        
	unsigned short  m_usZhiTm;
	unsigned short  m_usPaoTm;	
	BYTE 	m_bytrecordfile;	// ��Ҫ���ŵĳ���¼���ļ��ĸ���
	char	m_szrecord[5][11];	// ��Ҫ���ŵĳ���¼���ļ�������
	
	char	m_szPlay[200];		// վ�㲥������	
	char	m_szName[30];		// վ�� (������0,�����1���ֽڱ���Ϊ0)
};

struct tagStationsNew
{
	tagOneStationNew m_aryStation[400];		// ����վ��
	int m_iStaTotal;						// վ������ (��������������)
	int  m_iLineNo;		     				// ��·���,�����ı������ֱ�ӹ�ϵ
	BYTE m_bytLineEnb;						// ��·�Ƿ�ʹ�ܣ�0--��ʹ�ܣ�1��ʹ��
	char m_szLineNoName[100];				// ��·��,����"�ô� 29 ���԰��������"�� (������0,�����1���ֽڱ���Ϊ0)
	
	tagStationsNew() {	memset(this, 0, sizeof(*this));	}
};

struct tagLineHeadNew
{
	WORD	m_wLineTotal; // ��·����
	WORD	m_wLineSelNo; // ������·���
	char ver[16];
	tagLineHeadNew() {	memset(this, 0, sizeof(*this));	}
};

class CAutoRptStationNew
{
public:
	CAutoRptStationNew();
	~CAutoRptStationNew();

	int init();
	int Release();	

	void P_ThreadAutoRptStation();	
	void P_TmSetCheCi();

	bool LoadStations( tagStationsNew &v_objStations, bool v_bReBegin, int v_bytSelNo );
	
	void SendInOutData(int v_iSta, byte v_bytSym);
	void StopPlayCurSta();
	
	int DealComu8e( char* v_szBuf, DWORD v_dwBufLen );
	int DecompressLineFile(char *filename);
	bool ShowAvTxt( char* v_szUp,char *v_szDown);
	void Show_diaodu(char *v_szbuf);
	ushort GetLineId();
	//BOOL DeleteDirectory(const CString szDir);
	BYTE GetCheCi();
	byte GetCurLimitSpd();
	bool ShowToAV( DWORD PlayNo );
	bool CenterLoadSta(ushort v_usLine);
	BYTE GetUpDownSta2();
	void SetWorkSta();
	
	int GetUniStaId(int v_iCurId);
	byte GetAutoRptSta();
	void SetAutoRptSta(byte m_bytSet);
	//��������ĳ����·
	void hdl_4216(byte* buf, int len);
	//����������վ���ܣ��ó�̨�Ƿ���������վ����
	void hdl_4217(byte* buf, int len);
	
	//Ӧ����(1) + ��ţ�1��+���ͣ�1�� �� [ ��Ӫ�����ͣ�1��]
	void hdl_4220(byte* buf, int len);
	//��������һ���������绰�ĺ��롣
	void hdl_4219(byte* buf, int len);

	int  Snd4254( const tagGPSData &v_objGps );
	
	//���Ĳ�ѯ��̨��·���
	void hdl_4218();
	void ShowLineVer();
	void ShowLineAV();
	
private:
	byte m_ucStaCnt;
	byte  m_bytEnbRptSta;		//Ӫ��-��Ӫ��״̬
	byte  m_bytCheCi;			//��������
	byte  m_bytCheCiTmp;		//��������
	DWORD m_dwInStaTm;			// ��վʱ��
	DWORD m_dwOutStaTm;			// ��վʱ��
	WORD  m_usCurZhiTm;
	WORD  m_usCurPaoTm;
	byte  m_ucLstUpDown;
	byte  m_ucCurLimitSpd;

	CInnerDataMng m_objRcvDataMng;
	
	tagStationsNew m_objStations;
	tagReportStateNew m_objRptSta;		//��ǰ��վ״̬
	tagLineHeadNew m_objLinesHead;
	
	bool CalculateDis(double latitude,	double longitude, double *dwStaDis, int curNo, int bytareaNo);
	// 	void Playsound();//add by kzy 20091019
	
	byte GetUpDownSta(int v_iCurId);
	
	//��ȡ����վ��״̬, 0: ��վ�� 1����վ
	bool GetInOutSta(int v_iCurId);
	
	byte GetStaLimitSpd(int v_iCurId);

	//��ȡվ�������:ʼ��վ/�յ�վ/�м�վ/����/����/���
	byte GetStaAttr(int v_iCurId);
};

#endif

