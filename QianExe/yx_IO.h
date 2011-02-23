#ifndef _YX_IO_H_
#define _YX_IO_H_

void G_TmAccChk(void *arg, int len);

class CQianIO
{
public:
	CQianIO();
	~CQianIO();

	int Init();
	int Release();

	int DealIOMsg(char *v_szbuf, DWORD v_dwlen, BYTE v_bytSrcSymb);

	void P_TmAccChk();

	BOOL PowerOnGps( bool v_bVal );
	void StartAccChk(DWORD v_dwFstInterval);
	void StopAccChk();
	DWORD GetDeviceSta();
	void SetDevSta( bool v_bSet, DWORD v_dwVal );
	void SetLowPowSta( bool v_bVal );
	bool GetLowPowSta();
	void ConnectOilElec();
	void DisconnOilElec();
	
	BOOL IN_QueryIOSta( DWORD &v_dwIoSta );
	BOOL IN_QueryIOSpeed( DWORD &v_dwCyclePerSec );
	BOOL IN_QueryMeterCycle( DWORD &v_dwCycle );
	BOOL IN_QueryOil( BYTE &v_bytOilAD, BYTE &v_bytPowAD );
	
private:
	pthread_mutex_t m_MutexDevSta;
	
	DWORD m_dwMeterComuCycle;	// ��������ۼ�ֵ	
	DWORD m_dwDeviceSta;		// ������һ���Ե�IO�ź�,�����º��޷���ѯ��IO�źŻ��豸����ָ��
	DWORD m_dwPowAlertTm;
	bool m_bPowLowSta;
	DWORD m_dwTmRcvDCD;			//��⵽DCD�źŵ�ʱ��	
	DWORD m_dwAccInvalidTm;
};

// ��ȡʵʱ�Ĵ������ź� (ȫ��������)���貹��)
inline BOOL CQianIO::IN_QueryIOSta( DWORD &v_dwIoSta )
{
	byte bytSta;
	IOGet(IOS_ACC, &bytSta);
	if(bytSta == IO_ACC_ON)
		v_dwIoSta |= 0x00000001;
		
	return TRUE;
}

// ��ü�ʱ�ٶ�ֵ,����/��
inline BOOL CQianIO::IN_QueryIOSpeed( DWORD &v_dwCyclePerSec )
{
	return TRUE;
}

// ��ȡ��̨�����������ۼ����,����ֵ��λ:������
inline BOOL CQianIO::IN_QueryMeterCycle( DWORD &v_dwCycle )
{
	return TRUE;
}

// ��ȡ�ͺ�AD,��Դ��ѹAD (����һ����ȡֵ��Χת��)
inline BOOL CQianIO::IN_QueryOil( BYTE &v_bytOilAD, BYTE &v_bytPowAD )
{
	return TRUE;
}

#endif

