#include "yx_QianStdAfx.h"

#if USE_LIAONING_SANQI == 1

#undef MSG_HEAD
#define MSG_HEAD	("QianExe-LN_Login ")

void G_TmLogin(void *arg, int len)
{
	g_objLogin.Login();
}

void G_TmRegister(void *arg, int len)
{
	g_objLogin.Register();
}

void G_TmHeartKeep(void *arg, int len)
{
	g_objLogin.HeartKeep();
}

//////////////////////////////////////////////////////////////////////////

CLN_Login::CLN_Login()
{
	m_dwLoginSucc = 0;
	m_dwLoginTimers = 0;
	m_dwRegisterTimers = 0;
	
	a = 0;
	b = 0;

	m_dwSendHeartKeepTimers = 0;
}

CLN_Login::~CLN_Login()
{

}

bool CLN_Login::IsOnline()
{
// 	PRTMSG(MSG_DBG, "---------------------m_dwLoginSucc = %d\n", m_dwLoginSucc);
// 	PRTMSG(MSG_DBG, "---------------------m_dwLoginTimers = %d\n", m_dwLoginTimers);
// 	PRTMSG(MSG_DBG, "---------------------m_dwRegisterTimers = %d\n", m_dwRegisterTimers);
// 	PRTMSG(MSG_DBG, "---------------------m_dwSendHeartKeepTimers = %d\n", m_dwSendHeartKeepTimers);

	if( m_dwLoginSucc )
		return true;
	else
		return false;
}

int CLN_Login::Init()
{
	// �жϳ�̨�Ƿ�����������ע��
	tag1PComuCfg obj1PComuCfg;
	GetImpCfg( (void*)&obj1PComuCfg, sizeof(obj1PComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );

	if( obj1PComuCfg.m_bRegsiter )	// ������ע��������͵�½֡
	{
		Login();
	}
	else							// ��δ��ע�ᣬ������ע��֡
	{
		Register();
	}

	return 0;
}

void CLN_Login::Register()
{
	tag1PComuCfg obj1PComuCfg;
	GetImpCfg( &obj1PComuCfg, sizeof(obj1PComuCfg),	offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	if( 0x20 == obj1PComuCfg.m_szTel[0] || 0x30 == obj1PComuCfg.m_szTel[0] || !strcmp("", obj1PComuCfg.m_szTel) )
	{
		PRTMSG(MSG_NOR, "Get SelfTel, but Incorrect or Empty, Quit Register!\n" );
		return;
	}

	m_dwRegisterTimers++;
	
	// ע���ش���10��δ�յ�Ӧ������ComuExe���²���
	if( m_dwRegisterTimers >= 50 )
	{
		PRTMSG(MSG_NOR, "Send Register to max timers, Ask ComuExe to queck net!\n");
		char buf[] = {0x34, 0x01};
		DataPush((void*)&buf, 2, DEV_SOCK, DEV_PHONE, LV3);
		
		m_dwRegisterTimers = 0;
		_KillTimer(&g_objTimerMng, REGISTER_TIMER);

		return;
	}

	char  szBuf[200] = {0};
	DWORD dwLen = 0;

	char  szTempBuf[100] = {0};
	DWORD dwTempLen = 0;

	szBuf[dwLen++] = 0x41;	// ��������

	szBuf[dwLen++] = 0x00;	// ������ע��
	szBuf[dwLen++] = 0x00;

	szBuf[dwLen++] = 0x00;	// Attr1
	szBuf[dwLen++] = 0x03;	// K_len1
	szBuf[dwLen++] = 'v';	// Key1: ver �ն˰汾�ţ�M��3��+R��3��+V��4��+S��1��
	szBuf[dwLen++] = 'e';
	szBuf[dwLen++] = 'r';
	szBuf[dwLen++] = 0x00;	// V_len1
	szBuf[dwLen++] = dwTempLen = GetVer(szTempBuf);
	memcpy(szBuf+dwLen, szTempBuf, dwTempLen);
	dwLen += dwTempLen;

	szBuf[dwLen++] = 0x00;	// Attr2
	szBuf[dwLen++] = 0x03;	// K_len2
	szBuf[dwLen++] = 'v';	// Key2: ven ���̱��
	szBuf[dwLen++] = 'e';
	szBuf[dwLen++] = 'n';
	szBuf[dwLen++] = 0x00;	// V_len2
	szBuf[dwLen++] = dwTempLen = GetVen(szTempBuf);
	memcpy(szBuf+dwLen, szTempBuf, dwTempLen);
	dwLen += dwTempLen;

	szBuf[dwLen++] = 0x00;	// Attr3
	szBuf[dwLen++] = 0x05;	// K_len3
	szBuf[dwLen++] = 'g';	// Key3: group ���żƷѺ�
	szBuf[dwLen++] = 'r';
	szBuf[dwLen++] = 'o';
	szBuf[dwLen++] = 'u';
	szBuf[dwLen++] = 'p';
	szBuf[dwLen++] = 0x00;	// V_len3
	szBuf[dwLen++] = dwTempLen = GetGroup(szTempBuf);
	memcpy(szBuf+dwLen, szTempBuf, dwTempLen);
	dwLen += dwTempLen;
	
	// ����
	g_objApplication.SendAppFrame(MODE_A, 0x80, true, 0, szBuf, dwLen);
	PRTMSG(MSG_NOR, "Send Register Frame!\n");
	
	_SetTimer(&g_objTimerMng, REGISTER_TIMER, 10000, G_TmRegister);
}


void CLN_Login::RegisterAnswer(char *v_szBuf, DWORD v_dwLen)
{
	if( v_dwLen != 1 )
	{
		PRTMSG(MSG_ERR, "RegisterAnswer len err, v_dwLen = %d\n", v_dwLen);
		return;
	}
	
	switch( v_szBuf[0] )
	{
	case 0x01:		// ע��ɹ�
		{
			PRTMSG(MSG_NOR, "Register succ!\n");

			tag1PComuCfg obj1PComuCfg;
			GetImpCfg( (void*)&obj1PComuCfg, sizeof(obj1PComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
			obj1PComuCfg.m_bRegsiter = true;
			SetImpCfg( (void*)&obj1PComuCfg, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );

			m_dwRegisterTimers = 0;
			
			_KillTimer(&g_objTimerMng, REGISTER_TIMER);
			
			// ���͵�½֡
			Login();
		}
		break;
		
	case 0x00:		// ע��ʧ�ܣ�����ע��
		{
			PRTMSG(MSG_NOR, "Register failed, continue register!\n");
		}
		break;
		
	default:
		break;
	}
}

void CLN_Login::Login()
{
	tag1PComuCfg obj1PComuCfg;
	GetImpCfg( &obj1PComuCfg, sizeof(obj1PComuCfg),	offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	if( 0x20 == obj1PComuCfg.m_szTel[0] || 0x30 == obj1PComuCfg.m_szTel[0] || !strcmp("", obj1PComuCfg.m_szTel) )
	{
		PRTMSG(MSG_NOR, "Get SelfTel, but Incorrect or Empty, Quit Login!\n" );
		return;
	}

	m_dwLoginTimers++;
	
	// ��½�ش���10��δ�յ�Ӧ������ComuExe���²���
	if( m_dwLoginTimers >= 50 )
	{
		PRTMSG(MSG_NOR, "Send Login to max timers, Ask ComuExe to queck net!\n");
		char buf[] = {0x34, 0x01};
		DataPush((void*)&buf, 2, DEV_SOCK, DEV_PHONE, LV3);
		
		m_dwLoginTimers = 0;
		_KillTimer(&g_objTimerMng, LOGIN_TIMER);
	
		return;
	}

	char  szBuf[200] = {0};
	DWORD dwLen = 0;
	
	char  szTempBuf[100] = {0};
	DWORD dwTempLen = 0;
	
	szBuf[dwLen++] = 0x41;	// ��������
	
	szBuf[dwLen++] = 0x00;	// ��������½
	szBuf[dwLen++] = 0x01;
	
	// �ն�����
	szBuf[dwLen++] = 0x00;	// Attr1
	szBuf[dwLen++] = 0x02;	// K_len1
	szBuf[dwLen++] = 't';	// Key1: tP �ն�����
	szBuf[dwLen++] = 'P';
	szBuf[dwLen++] = 0x00;	// V_len1
	szBuf[dwLen++] = 0x01;
	szBuf[dwLen++] = 0x01;	// 01h����Э�飬02h����Э�飬03h������Э�飬04h��VG3Э��

	// �ն˰汾��
	szBuf[dwLen++] = 0x00;	// Attr2
	szBuf[dwLen++] = 0x03;	// K_len2
	szBuf[dwLen++] = 'v';	// Key2: ver �ն˰汾�ţ�M��3��+R��3��+V��4��+S��1��
	szBuf[dwLen++] = 'e';
	szBuf[dwLen++] = 'r';
	szBuf[dwLen++] = 0x00;	// V_len2
	szBuf[dwLen++] = dwTempLen = GetVer(szTempBuf);
	memcpy(szBuf+dwLen, szTempBuf, dwTempLen);
	dwLen += dwTempLen;

	// λ����Ϣ���ٶȡ��������
	dwTempLen = G_GetPoint(szTempBuf);	
	memcpy(szBuf+dwLen, szTempBuf, dwTempLen);
	dwLen += dwTempLen;
	
	// ����
	g_objApplication.SendAppFrame(MODE_A, 0x80, true, 0, szBuf, dwLen);
	PRTMSG(MSG_NOR, "Send Login Frame!\n");
	_KillTimer(&g_objTimerMng, HEART_KEEP_TIMER);
	_SetTimer(&g_objTimerMng, LOGIN_TIMER, 10000, G_TmLogin);
}

void CLN_Login::LoginAnswer(char *v_szBuf, DWORD v_dwLen)
{
	if( v_dwLen != 1 )
	{
		PRTMSG(MSG_ERR, "LoginAnswer len err, v_dwLen = %d\n", v_dwLen);
		return;
	}
	
	switch( v_szBuf[0] )
	{
	case 0x01:		// ��½�ɹ�
		{
			PRTMSG(MSG_NOR, "Login succ!\n");
			g_objLNDownLoad.m_bNetFined = true;
			m_dwLoginTimers = 0;
			m_dwLoginSucc = 1;
			
			_KillTimer(&g_objTimerMng, LOGIN_TIMER);
			
			// ��������ά��֡
			HeartKeep();
		}
		break;
		
	case 0x00:		// ��½ʧ�ܣ�������½
		{
			PRTMSG(MSG_NOR, "Login failed, continue Login!\n");
		}
		break;
		
	default:
		break;
	}
}


void CLN_Login::HeartKeep()
{
	m_dwSendHeartKeepTimers++;

	// ���������ش���9��δ�յ�����Ӧ������ComuExe���²���
	if( m_dwSendHeartKeepTimers >= 9 )
	{
		PRTMSG(MSG_NOR, "Send HeartKeep to max timers, Ask ComuExe to queck net!\n");
		char buf[] = {0x34, 0x01};
		DataPush((void*)&buf, 2, DEV_SOCK, DEV_PHONE, LV3);
		g_objLNDownLoad.m_bNetFined = false;
		m_dwSendHeartKeepTimers = 0;
		m_dwLoginSucc = 0;
		_KillTimer(&g_objTimerMng, HEART_KEEP_TIMER);

		return;
	}

	char  szBuf[100] = {0};
	DWORD dwLen = 0;
	
	szBuf[dwLen++] = 0x41;	// ��������
	
	szBuf[dwLen++] = 0x00;	// ����������
	szBuf[dwLen++] = 0x03;
	
	// ����
	g_objApplication.SendAppFrame(MODE_A, 0x80, true, 0, szBuf, dwLen);
	PRTMSG(MSG_NOR, "Send HeartKeep Frame!\n");
	
	// ������ʱ��
	tag1PComuCfg obj1PComuCfg;
	GetImpCfg( (void*)&obj1PComuCfg, sizeof(obj1PComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(obj1PComuCfg) );
	_SetTimer(&g_objTimerMng, HEART_KEEP_TIMER, obj1PComuCfg.m_iHeartKeepInterval*1000, G_TmHeartKeep );
}

void CLN_Login::HeartKeepAnswer(char *v_szBuf, DWORD v_dwLen)
{
	switch( v_szBuf[0] )
	{
	case 0x01:		// ����Ӧ��ɹ�
		{
			PRTMSG(MSG_NOR, "Recv Center HeartKeep Answer!\n");
			
			m_dwSendHeartKeepTimers = 0;
		}
		break;
		
	case 0x00:		// ����Ӧ��ʧ��
		{
			PRTMSG(MSG_NOR, "Center HeartKeep Answer failed!\n");
		}
		break;
		
	default:
		break;
	}
}

// ��ȡ�ն˰汾��
int CLN_Login::GetVer(char *v_szBuf)
{
	tag2QcgvCfg cfg_cgv;
	GetSecCfg( &cfg_cgv, sizeof(cfg_cgv), offsetof(tagSecondCfg, m_uni2QcgvCfg.m_obj2QcgvCfg), sizeof(cfg_cgv) );
	memcpy(v_szBuf, cfg_cgv.m_temiver, cfg_cgv.m_termiverlen);
	v_szBuf[cfg_cgv.m_termiverlen] = '\0';
	PRTMSG(MSG_DBG, "Ver:%s\n", v_szBuf);
	return cfg_cgv.m_termiverlen;
}

// ��ȡ����ID
int CLN_Login::GetVen(char *v_szBuf)
{
	tag2QcgvCfg cfg_cgv;
	GetSecCfg( &cfg_cgv, sizeof(cfg_cgv), offsetof(tagSecondCfg, m_uni2QcgvCfg.m_obj2QcgvCfg), sizeof(cfg_cgv) );
	memcpy(v_szBuf, cfg_cgv.m_companyid, cfg_cgv.m_companyidlen);
	v_szBuf[cfg_cgv.m_companyidlen] = '\0';
	PRTMSG(MSG_DBG, "Man ID:%s\n", v_szBuf);
	return cfg_cgv.m_companyidlen;
}

// ��ȡ���żƷѺ�
int CLN_Login::GetGroup(char *v_szBuf)
{
	tag2QcgvCfg cfg_cgv;
	GetSecCfg( &cfg_cgv, sizeof(cfg_cgv), offsetof(tagSecondCfg, m_uni2QcgvCfg.m_obj2QcgvCfg), sizeof(cfg_cgv) );
	memcpy(v_szBuf, cfg_cgv.m_groupid, cfg_cgv.m_groupidlen);
	v_szBuf[cfg_cgv.m_groupidlen] = '\0';
	PRTMSG(MSG_DBG, "Group ID:%s\n", v_szBuf);
	return cfg_cgv.m_groupidlen;
}

#endif

