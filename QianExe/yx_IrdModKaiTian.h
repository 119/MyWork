#ifndef _YX_IRDMOD_KAITIAN_H_
#define _YX_IRDMOD_KAITIAN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKTIrdMod  
{
public:
	CKTIrdMod();
	virtual ~CKTIrdMod();

	int  Init();
	int	 Release();

	bool ComOpen();
	bool ComClose();
	
	void P_ThreadRecv();
	void P_ThreadWork();
	
	void KTIrdStart();
	void KTIrdEnd();
	
	CInnerDataMng	m_objIrdModMng;

private:
	typedef union 
	{
	  UINT irda_recv_code;
	  struct 
	  {
	  	BYTE  irkey_reverse_code;
	    BYTE  irkey_code;
	    WORD 	sys_id_code;
	  }irkey;
	}IRKEY_INFO_T;

	enum IRDSTATE
	{
		stStart = 1,
		stServ,
		stEnd
	};
	
	enum SERLEVEL
	{
		LEVEL1 = 1,//������
		LEVEL2,//������
		LEVEL3,//����
		LEVEL4//������
	};
	
	int	m_iComPort;
	pthread_t m_pthreadRecv; //��ȡ�߳�
	pthread_t m_pthreadWork; //�����߳�
	bool m_bEscRecv;
	bool m_bEscWork;
	
	IRKEY_INFO_T m_objIrda;
	IRDSTATE m_CurSta;
	SERLEVEL m_SerLevel;
};

#endif 
