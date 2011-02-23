#ifndef _YX_LED_KAITIAN_H_
#define _YX_LED_KAITIAN_H_

#define USE_ALA     0       //ʹ��GPS1��

#define CTEXTLEN	17		//C����Ϣ����
#define DTEXTLEN	17		//D����Ϣ����
#define CLEARBACK_INTERVAL	30*1000	//C����Ϣ��ͣ��ʱ��

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKTLed  
{
public:
	CKTLed();
	virtual ~CKTLed();

	int  Init();
	int	 Release();

	bool ComOpen();
	bool ComClose();
	int  ComWrite(char *v_szBuf, int v_iLen);

	void P_ThreadRecv();
	void P_ThreadWork();

	void P_TmClearFrontProc();
	void P_TmClearBackProc();
	void P_TmSetLedStaProc();
	
	//zzg add
	void SetLedShow(char v_bytType,byte v_bytIndex,UINT v_uiInterval );//����״̬��Ϣ��A,B�ࣩ
	void SetBackText(char v_bytType,byte * v_aryText,byte v_bytCount);//���ú���������Ϣ��C,D�ࣩ
	void DelBackText(char v_bytType);//���������Ϣ

	void DingdengClassA( bool b_bAlarm);
  void DingdengClassB( bool v_bDisp,byte v_bytIndex );
	void DingdengClassC( byte v_bytCmd,byte* v_pContent = NULL );
	void DingdengClassD( byte v_bytCmd,byte v_bytIndex =0,byte* v_pContent =NULL );
	void DingdengClassE();

	CInnerDataMng	m_objReadMng;
	CInnerDataMng m_objWorkMng;

private:
	int	m_iComPort;// ���ھ��		
	pthread_t	m_pthreadRecv;//LED��ȡ�߳�
	pthread_t	m_pthreadWork;//LED�����߳�
	
	bool m_bEscRecv;
	bool m_bEscWork;
	
	char m_cShowType;
	byte m_bytIndex;
	bool m_bConnect;
	
	void _SendLinkKeep();
	void DealLEDStack();// ���д���LED����
  bool Sendfrm(char* v_pData, int v_iLen);
};

#endif
