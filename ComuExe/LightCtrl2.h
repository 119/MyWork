#ifndef _YX_LIGHTCTRL2_H_
#define _YX_LIGHTCTRL2_H_

enum LIGHTSTA2
{
	Red_KuaiShan,			// ��ƿ�������2�룬��2�룩
	Red_ManShan,			// �����������0.5�룬��4.5�룩	
	Green_KuaiShan,			// �̵ƿ�������2�룬��2�룩
	Green_ManShan,			// �̵���������0.5�룬��4.5�룩
	Red_JiShan,				// ��Ƽ�������0.5�룬��0.5�룩
	Green_JiShan			// �̵Ƽ�������0.5�룬��0.5�룩
};

class CLightCtrl2
{
public:
	CLightCtrl2();
	virtual ~CLightCtrl2();

	int Init();
	int Release();
	void SetLightSta(char v_szSta);
	void P_ThreadLightCtrl();

public:
	CInnerDataMng	m_objRecvCtrlMng;
	CInnerDataMng	m_objMemInfoMng;
	
	char m_szReBootTime[15];
	char m_szSwiLinkTime[15];
	char m_szDiskLinkTime[15];
	char m_szMonLinkTime[15];
	
	char m_szTelOnTime[15];
	int m_iTelOnCnt;
	int m_iTelOnSec[100];
	char m_szTelInitTime[15];
	int m_iTelInitCnt;
	int m_iTelInitSec[100];
	char m_szLowSigTime[15];
	int m_iLowSigCnt;
	int m_iLowSigSec[100];
	char m_szDialNetTime[15];
	int m_iDialNetCnt;
	int m_iDialNetSec[100];
	char m_szTcpConTime[15];
	int m_iTcpConCnt;
	int m_iTcpConSec[100];
	char m_szLogInTime[15];
	int m_iLogInCnt;
	int m_iLogInSec[100];
	
	char m_szDeal3801Time[15];
	int m_iDeal3801Cnt;
	int m_iDeal3801Sec[100];
	char m_szVideoMonTime[15];
	int m_iVideoMonCnt;
	int m_iVideoMonSec[100];
	
	DWORD m_dwDevErrCode;
	DWORD m_dwCfgErrCode;
	DWORD m_dwTelErrCode;
	DWORD m_dwMonErrCode;
	
	BYTE m_bytDvrState;
	BYTE m_bytTcpState;

private:
	byte		m_bytLightSta;
	bool		m_bVideoWork;	// �Ƿ�����Ƶ¼��״̬
	bool		m_bNetWork;		// �Ƿ�����������״̬
	bool		m_bFoot;		// �Ƿ������ٱ���״̬
	bool		m_bDial;		// �Ƿ������粦�Ź�����
	pthread_t	m_pthreadLightCtrl;
};

#endif

