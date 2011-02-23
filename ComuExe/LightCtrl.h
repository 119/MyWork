#ifndef _YX_LIGHTCTRL_H_
#define _YX_LIGHTCTRL_H_

enum LIGHTSTA
{
	ChangLiang = 1,		// ����
	KuaiShan,			// ��������0.5�룬��0.5�룩
	ManShan				// ��������2�룬��2�룩	
};

class CLightCtrl
{
public:
	CLightCtrl();
	virtual ~CLightCtrl();

	int Init();
	int Release();
	void SetLightSta(char v_szSta);
	void P_ThreadLightCtrl();

public:
	CInnerDataMng	m_objRecvCtrlMng;

private:
	byte		m_bytLightSta;
	pthread_t	m_pthreadLightCtrl;
};

#endif

