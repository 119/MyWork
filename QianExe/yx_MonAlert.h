#ifndef _YX_MONALERT_H_
#define _YX_MONALERT_H_

void G_TmMon(void *arg, int len);
void G_TmAlertInvalid(void *arg, int len);
void G_TmAlertSilent(void *arg, int len);
void G_TmAutoReqLsn(void *arg, int len);
void G_TmAlertInitQuery(void *arg, int len);
void G_TmAlertTest(void *arg, int len);

class CMonAlert  
{
public:
	CMonAlert();
	virtual ~CMonAlert();
	
	void Init();
	
	int DealPhoneAutoReqLsnRes( BYTE v_bytResType );
	bool JugFootAlert();
	DWORD GetAlertSta();
//	void FillAlertSymb( const DWORD v_dwAlertSta, BYTE &v_byt1, BYTE &v_byt2 );
	void FillAlertSymb( const DWORD v_dwAlertSta, BYTE *v_byt1, BYTE *v_byt2 );
	void FillAlertSymbFor0149( const DWORD v_dwAlertSta, BYTE *v_byt1, BYTE *v_byt2 );
	bool IsMonAlert();
	
	int GetCurGPS( tagGPSData& v_objGps, bool v_bGetLstValid );
	int GetCurQianGps( tagQianGps &v_objQianGps, bool v_bGetLstValid );
	void GpsToQianGps( const tagGPSData &v_objGps, tagQianGps &v_objQianGps );
	
	int SetAlertFoot( const bool v_bSet );
	int SetAlertPowOff( const bool v_bSet );
	int SetAlertPowBrownout( const bool v_bSet );
	int SetAlertFrontDoor( const bool v_bSet );
	int SetAlertBackDoor( const bool v_bSet );
	int SetAlertOverTurn( const bool v_bSet );
	int SetAlertBump( const bool v_bSet );
	int SetAlertDrive( const bool v_bSet ); // �Ƿ���������
	int SetAlertOverSpd( const bool v_bSet );
	int SetAlertBelowSpd( const bool v_bSet );
	int SetAlertArea( const bool v_bSet );	// Խ�����򱨾�
	int SetAlertOther(DWORD v_dwType, const bool v_bSet);
	int SetAlertTest();
	
	int Deal0136( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen );
	int Deal103d( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM );
	int Deal101a( char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen );
	int Deal1009( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM );
	int Deal0720( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen );
	int Deal0723( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen );
	int Deal0111( char* v_szSrcHandtel, bool v_bFromSM );
	int Deal0105( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen );
	int Deal0103( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen );
	int Deal0102( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM );
	int Deal0101( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen, bool v_bFromSM, bool v_bSendRes = true );
	
	void P_TmMon();
	void P_TmAlertSilent();
	void P_TmAlertInvalid();
	void P_TmAutoReqLsn();
	void P_TmAlertTest();
	void P_TmAlertInitQuery();
	void P_TmAlertSpd();

private:
	int  _EndMon();
	int  _BeginMon( double v_dLon, double v_dLat );
	
	int  _Snd0145( const tagGPSData &v_objGps, const tag2QGprsCfg &v_objGprsCfg, bool v_bFbidAllowSms = false );
	int  _Snd0149( const tagGPSData &v_objGps, const tag2QGprsCfg &v_objGprsCfg, bool v_bFbidAllowSms = false );
	int  _SendOne0149Alert(const tagGPSData &v_objGps, byte v_bytAlertType);

	int  _DealAlert( bool v_bReqLsn, bool v_bSilentTimer );
	void _ClrAlertSta();
	
	int  _BeginAutoReqLsn( DWORD v_dwBeginInterval, char* v_szLsnTel = NULL );
	int  _AutoReqLsn();
	 
private:
	tagMonStatus	m_objMonStatus;
	
	DWORD m_dwAlertSymb;
	
	DWORD m_dwAlertFootTm;
	DWORD m_dwAlertPowerOffTm;
	DWORD m_dwAlertBrownoutTm;
	DWORD m_dwAlertFrontDoorTm;
	DWORD m_dwAlertBackDoorTm;
	DWORD m_dwAlertOverTurnTm;
	DWORD m_dwAlertBumpTm;
	
	double m_dSpdAlermMinSpd; // ���ٱ�����ֵ (��/��)
	BYTE m_bytSpdAlermPrid; // ���ٳ���ʱ�� (��)
	BYTE m_bytSpdAlermSta; // 0,δ����; 1,���ֳ���; 2,�ѱ���
	
	//	DWORD m_dwReqLsnTm;
	char	m_szAutoReqLsnTel[ 20 ]; // ��NULL��β
	BYTE	m_bytAutoReqLsnCount; // ������������ļ���
	BYTE	m_bytAutoReqLsnContBusyCt; // ��������ʱ�����յ����豸æ��Ӧ��Ĵ���
};

#endif


