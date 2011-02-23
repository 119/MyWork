// DriverCtrl.cpp: implementation of the CDriverCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "yx_QianStdAfx.h"
#include "yx_DriverCtrl.h"

#if USE_DRIVERCTRL == 1

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

const UINT DRIVERLOG_INTERVAL		= 120000; // �ȴ�˾����½�ĳ�ʱʱ��,ms
const UINT ACCOFF_DRVRRELOG_INTERVAL= 20000; // ACC������Ч�೤ʱ���,��Ҫ���½���˾����½,ms

void G_TmDriverLog(void *arg, int len)
{
	g_objDriverCtrl.P_TmDriverLog();
}

void G_TmAccOffDriverReLog(void *arg, int len)
{
	g_objDriverCtrl.P_TmAccOffDriverReLog();
}

void G_TmFstDriverLog(void *arg, int len)
{
	g_objDriverCtrl.P_TmFstDriverLog();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriverCtrl::CDriverCtrl()
{

}

CDriverCtrl::~CDriverCtrl()
{

}


void CDriverCtrl::P_TmDriverLog()
{
	_KillTimer(&g_objTimerMng, DRIVERLOG_TIMER );
	
	g_objMonAlert.SetAlertDrive(true);
	
	g_objCarSta.SavDriverNo( 0xffff );
	g_objCarSta.SavDrvRecSta( false );

	char buf[100] = { 0 };
	buf[0] = 0x01;
	buf[1] = 0x00;
	strcat( buf, "˾������,�������볬ʱ,�벻Ҫ�Ƿ���ʻ!" );
	DataPush( buf, strlen(buf), DEV_QIAN, DEV_DVR, 2 );
	
	//hxd �����ֱ���ʱ��
	memset(buf,0,sizeof(buf));
	buf[0] = char(0x01);
	buf[1] = char(0xf8);
	DataPush( buf, 2, DEV_QIAN, DEV_DIAODU, 2 );
}

void CDriverCtrl::P_TmAccOffDriverReLog()
{
	_KillTimer(&g_objTimerMng, ACCOFF_DRVRRELOG_TIMER );

	g_objMonAlert.SetAlertDrive(false);

	g_objCarSta.SavDriverNo( 0 );
	g_objCarSta.SavDrvRecSta( false );
}

void CDriverCtrl::P_TmFstDriverLog()
{
	// ˾����½�жϴ���
	_KillTimer(&g_objTimerMng, FSTDRIVERLOG_TIMER );
	DriverCtrl( (g_objQianIO.GetDeviceSta() & DEVSTA_ACCVALID) ? true : false );
}

void CDriverCtrl::Init()
{
	// ������ʼ��½��ʱ�� (Ҫ�ȵ�����Ҳ������)
	_SetTimer(&g_objTimerMng, FSTDRIVERLOG_TIMER, 5000, G_TmFstDriverLog );
}

void CDriverCtrl::Release()
{
}

// ˾����½����
void CDriverCtrl::DriverCtrl( bool v_bAccOn )
{
	_KillTimer(&g_objTimerMng, FSTDRIVERLOG_TIMER ); // ��ֹACC�仯����ʾ˾����½��,�������״ε�½

	static BYTE sta_bytSta = 0; // 0,��ʼ̬; 1,ACC ON; 2,ACC OFF

	if( v_bAccOn )
	{
		// ȥ��
		if( 1 == sta_bytSta ) return;
		sta_bytSta = 1;

		// �ر�ACC������Чʹ��˾����Ҫ���µ�½�Ķ�ʱ
		_KillTimer(&g_objTimerMng, ACCOFF_DRVRRELOG_TIMER );

		WORD wCurDriverNo = g_objCarSta.GetDriverNo();
		if( 0 == wCurDriverNo || 0xffff == wCurDriverNo ) // ����ǰû����Ч˾����Ϣ
		{
			// ��ȡ����
			tag2QDriverCfg objDrvrCfg;
			objDrvrCfg.Init();
			::GetSecCfg( &objDrvrCfg, sizeof(objDrvrCfg), offsetof(tagSecondCfg, m_uni2QDriverCfg.m_obj2QDriverCfg),
				sizeof(objDrvrCfg) );

			// �������˾����½֡: 0x62 + ��Ч��Ÿ�����1��������ţ�1������ţ�2������ݴ��볤�ȣ�1������ݴ��롿*N
			char buf[ 500 ];
			memset( buf, 0, sizeof(buf) );
			buf[0] = 0x01;		// Qian�������ȵģ�ǰ�涼Ҫ��0x01
			buf[1] = 0x62;
			DWORD dw = 3;
			WORD wTemp;
			bool bValidDriverCfgExist = false; // �Ƿ���������Ч��˾�����
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
			tag2QDriverPar_QIAN* aryDrvrPar = objDrvrCfg.m_aryDriver;
#endif
#if USE_PROTOCOL == 30
			tag2QDriverPar_KE* aryDrvrPar = objDrvrCfg.m_aryDriver;
#endif
			const size_t DRIVER_MAXCOUNT = sizeof(objDrvrCfg.m_aryDriver) / sizeof(objDrvrCfg.m_aryDriver[0]);
			for( size_t siz = 0; siz < DRIVER_MAXCOUNT; siz ++ )
			{
				// ���ü�¼�Ƿ������Ч˾����Ϣ
				if( aryDrvrPar[siz].m_wDriverNo < 1 ) continue;
				else bValidDriverCfgExist = true;

				// �洢�ռ���
				if( sizeof(buf) < dw + 4 + sizeof(aryDrvrPar[0].m_szIdentID) ) break;

				// ��䷢�͵���������֡
				buf[ dw ] = BYTE(siz); // ���
				wTemp = htons( aryDrvrPar[siz].m_wDriverNo );
				memcpy( buf + dw + 1, &wTemp, 2); // ���
				if( 0 == aryDrvrPar[siz].m_szIdentID[ sizeof(aryDrvrPar[0].m_szIdentID) - 1] )
					wTemp = strlen(aryDrvrPar[siz].m_szIdentID);
				else
					wTemp = sizeof(aryDrvrPar[siz].m_szIdentID);
				if( wTemp > sizeof(aryDrvrPar[siz].m_szIdentID) ) wTemp = sizeof(aryDrvrPar[siz].m_szIdentID);
				buf[ dw + 3 ] = BYTE(wTemp); // ��ݴ��볤��
				memcpy( buf + dw + 4, aryDrvrPar[siz].m_szIdentID, wTemp ); // ��ݴ���

				// ��Ч��Ÿ���
				buf[2] = BYTE(buf[2]) + 1;

				dw += 4 + wTemp;
			}

			// �������в�������Ч��˾����Ϣ,����Ҫ˾����½
			if( !bValidDriverCfgExist ) return;

			// ��������˾����½֡��������
			DataPush(  buf, dw, DEV_QIAN,  DEV_DIAODU, 3 );

			// ������½��ʱ��ʱ��
			_KillTimer(&g_objTimerMng, DRIVERLOG_TIMER );
			_SetTimer(&g_objTimerMng, DRIVERLOG_TIMER, DRIVERLOG_INTERVAL, G_TmDriverLog );

			// ��������˾����½����ʾ
			buf[0] = 0x01;
			buf[1] = 0x00;
			strcat( buf, "˾������,������������ݴ���!" );
			DataPush(  buf, strlen(buf), DEV_QIAN,  DEV_DVR, 2 );
		}
	}
	else
	{
		// ȥ��
		if( 2 == sta_bytSta ) return;
		sta_bytSta = 2;

		_KillTimer(&g_objTimerMng, ACCOFF_DRVRRELOG_TIMER );
		_SetTimer(&g_objTimerMng, ACCOFF_DRVRRELOG_TIMER, ACCOFF_DRVRRELOG_INTERVAL, G_TmAccOffDriverReLog ); // ��ʱҲ��Ϊ��ȥ��
	}
}

/// ���ü�ʻԱ�������: 
// 2��Э��:   �����֣�1�������ø�����1��������ţ�1������ţ�2������ݴ��볤�ȣ�1������ݴ��롿* N
// ǧ����Э��:�����֣�1�������ø�����1��������ţ�1������ţ�1������ݴ��볤�ȣ�1������ݴ��� ����ʻ֤�ų��ȣ�1������ʻ֤�š�* N
int CDriverCtrl::Deal3602( char* v_szSrcHandtel, char* v_szData, DWORD v_dwDataLen )
{
	tag2QDriverCfg objDriverCfg, objDriverCfgBkp;
	int iRet = 0;
	int iBufLen = 0;
	DWORD dw = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytResType = 0;
	BYTE bytSetType;
	BYTE bytSetCount = 0;
	BYTE bytNo = 0;
	BYTE bytLen = 0;
	BYTE byt = 0;

	//if( g_pView ) g_pView->DbgShowDataBuf( "Rcv3602", v_szData, v_dwDataLen );

	// ���ݲ������
	if( v_dwDataLen < 2 )
	{
		iRet = ERR_PAR;
		goto DEAL3602_FAILED;
	}

	// ��ȡԭʼ����
	iRet = ::GetSecCfg( &objDriverCfg, sizeof(objDriverCfg),
		offsetof(tagSecondCfg, m_uni2QDriverCfg.m_obj2QDriverCfg), sizeof(objDriverCfg) );
	if( iRet ) goto DEAL3602_FAILED;
	objDriverCfgBkp = objDriverCfg; // �ȱ���

	// ������
	bytSetType = BYTE( v_szData[0] );
	if( 0x7f == bytSetType ) bytSetType = 0;

	// ���ø��� (�ۺ�2�ͺ�ǧ����Э�飬�����������Ϊ0����ʾֻ�����)
	bytSetCount = BYTE( v_szData[1] );
	if( bytSetCount > sizeof(objDriverCfg.m_aryDriver) / sizeof(objDriverCfg.m_aryDriver[0]) )
	{
		iRet = ERR_PAR;
		goto DEAL3602_FAILED;
	}

	// �ж��Ƿ�Ҫ���ԭ����
	if( 1 == bytSetType )
	{
		memset( &objDriverCfg, 0, sizeof(objDriverCfg) );
	}

	// ��������
	dw = 2;
	for( byt = 0; byt < bytSetCount; byt ++ )
	{
		// �������ȼ��
		if( v_dwDataLen < dw + 4 )
		{
			iRet = ERR_PAR;
			goto DEAL3602_FAILED;
		}

		// ���
		bytNo = BYTE( v_szData[ dw ] );
		if( 0x7f == bytNo ) bytNo = 0;
		if( bytNo >= sizeof(objDriverCfg.m_aryDriver) / sizeof(objDriverCfg.m_aryDriver[0]) )
		{
			iRet = ERR_PAR;
			goto DEAL3602_FAILED;
		}
		dw ++;

		// ���
#if USE_PROTOCOL == 30
		memcpy( &objDriverCfg.m_aryDriver[bytNo].m_wDriverNo, v_szData + dw,
			sizeof(objDriverCfg.m_aryDriver[0].m_wDriverNo) );
		objDriverCfg.m_aryDriver[bytNo].m_wDriverNo = ::ntohs( objDriverCfg.m_aryDriver[bytNo].m_wDriverNo );
		if( 0 == objDriverCfg.m_aryDriver[bytNo].m_wDriverNo || 0xffff == objDriverCfg.m_aryDriver[bytNo].m_wDriverNo )
		{
			iRet = ERR_PAR;
			goto DEAL3602_FAILED;
		}
		dw += 2;
#endif
#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		objDriverCfg.m_aryDriver[bytNo].m_wDriverNo = BYTE( v_szData[dw] );
		if( 0 == objDriverCfg.m_aryDriver[bytNo].m_wDriverNo || objDriverCfg.m_aryDriver[bytNo].m_wDriverNo > 126 )
		{
			iRet = ERR_PAR;
			goto DEAL3602_FAILED;
		}
		dw ++;
#endif

		// ��ݴ��볤�� ���ۺ�2�ͺ�ǧ����Э�飬������󱣴�������ϴ�ĳߴ紦��
		bytLen = BYTE( v_szData[ dw ] );
		if( bytLen < 1 || bytLen > sizeof(objDriverCfg.m_aryDriver[0].m_szIdentID) / sizeof(objDriverCfg.m_aryDriver[0].m_szIdentID[0]) )
		{
			iRet = ERR_PAR;
			goto DEAL3602_FAILED;
		}
		dw ++;

		// �������ȼ��
		if( v_dwDataLen < dw + bytLen )
		{
			iRet = ERR_PAR;
			goto DEAL3602_FAILED;
		}

		// ������ݴ���
		strncpy( objDriverCfg.m_aryDriver[bytNo].m_szIdentID, v_szData + dw, bytLen );
		dw += bytLen;

#if USE_PROTOCOL == 0 || USE_PROTOCOL == 1 || USE_PROTOCOL == 2
		// ��ʻ֤���볤��
		bytLen = BYTE( v_szData[ dw ] );
		if( bytLen > sizeof(objDriverCfg.m_aryDriver[0].m_szDrivingLic) / sizeof(objDriverCfg.m_aryDriver[0].m_szDrivingLic[0]) )
		{
			iRet = ERR_PAR;
			goto DEAL3602_FAILED;
		}
		dw ++;

		// �������ȼ��
		if( v_dwDataLen < dw + bytLen )
		{
			iRet = ERR_PAR;
			goto DEAL3602_FAILED;
		}

		// �����ʻ֤����
		strncpy( objDriverCfg.m_aryDriver[bytNo].m_szDrivingLic, v_szData + dw, bytLen );
		dw += bytLen;
#endif
	}

	// ����Ӧ��
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x36, 0x42, buf, sizeof(buf), iBufLen, CVT_8TO7 );
	if( !iRet )
	{
		// д���ļ�
		if( iRet = ::SetSecCfg( &objDriverCfg, offsetof(tagSecondCfg, m_uni2QDriverCfg.m_obj2QDriverCfg),
			sizeof(objDriverCfg)) )
		{
			goto DEAL3602_FAILED;
		}

		// 080917,��������080899�޸�,ֻ���������ݡ�λ�ò�ѯӦ��Ͷ���ͨ���·����õ�Ӧ���߶���ͨ��
		if( iRet = g_objSock.SOCKSNDSMSDATA(buf, iBufLen, LV12, 0) )
		{
			::SetSecCfg( &objDriverCfgBkp, offsetof(tagSecondCfg, m_uni2QDriverCfg.m_obj2QDriverCfg),
				sizeof(objDriverCfgBkp) );
			goto DEAL3602_FAILED;
		}
		
		// ���������ڵĴ���������Ϊ��˾���ڿ������ѵ���˾����ʱ������룬�ƺ��Ƚ�Σ��
		// ����鵱ǰ�����ú��Ѿ���½��˾����Ϣ�Ƿ�һ�£���һ��������Ҫ���½��
	}
	else
		goto DEAL3602_FAILED;

	return 0;

DEAL3602_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x36, 0x42, buf, sizeof(buf), iBufLen, CVT_8TO7 ); 
	// 080917,��������080899�޸�,ֻ���������ݡ�λ�ò�ѯӦ��Ͷ���ͨ���·����õ�Ӧ���߶���ͨ��
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, 0 );

	return iRet;
}

// ˾����½Ӧ��
int CDriverCtrl::DealComu63( char* v_szData, DWORD v_dwDataLen )
{
	if( v_dwDataLen < 1 ) return ERR_PAR;

	if( 1 != v_szData[0] ) // ������ǳɹ�Ӧ��
	{
		g_objCarSta.SavDriverNo( 0xffff );
		g_objMonAlert.SetAlertDrive(true);

		// ��������˾����½����ʾ
		char buf[100] = { 0 };
		buf[0] = 0x01;
		buf[1] = 0x00;
		strcat( buf, "˾������,������ݴ��벻��ȷ!" );
		DataPush( buf, strlen(buf), DEV_QIAN, DEV_DVR, 2 );
	}
	else if( v_dwDataLen < 3 ) // ����ǳɹ�Ӧ��,�����������������
	{
		g_objCarSta.SavDriverNo( 0xffff );
		g_objMonAlert.SetAlertDrive(true);

		char buf[100] = { 0 };
		buf[0] = 0x01;
		buf[1] = 0x00;
		strcat( buf, "˾������,������ݴ��벻��ȷ!" );
		DataPush( buf, strlen(buf), DEV_QIAN, DEV_DVR, 2 );
	}
	else
	{
		_KillTimer(&g_objTimerMng, DRIVERLOG_TIMER );
		WORD wDriverNo;
		memcpy( &wDriverNo, v_szData + 1, sizeof(wDriverNo) );
		wDriverNo = ntohs( wDriverNo );
		g_objCarSta.SavDriverNo( wDriverNo );
		g_objMonAlert.SetAlertDrive(false);

		char buf[100] = { 0 };
		buf[0] = 0x01;
		buf[1] = 0x00;
		strcat( buf, "˾������,��֤ͨ��,ף����;���!" );
		DataPush( buf, strlen(buf), DEV_QIAN, DEV_DVR, 2 );
	}

	g_objCarSta.SavDrvRecSta( false );

	return 0;
}

// ����˾�����
int CDriverCtrl::DealComu65( char* v_szData, DWORD v_dwDataLen )
{
	tag2QDriverCfg objDriverCfg;
	objDriverCfg.Init();

	// ��ȡԭʼ����
	::GetSecCfg( &objDriverCfg, sizeof(objDriverCfg),
		offsetof(tagSecondCfg, m_uni2QDriverCfg.m_obj2QDriverCfg), sizeof(objDriverCfg) );

	// ���ҵ�ǰ˾����ŵ�ID
	BYTE bytAnsType = 2;
	char szID[ sizeof(objDriverCfg.m_aryDriver[0].m_szIdentID) ];
	memset( szID, sizeof(szID), 0 );
	WORD wCurDriverNo = g_objCarSta.GetDriverNo();
	if( wCurDriverNo > 0 && wCurDriverNo < 0xffff )
	{
		for( size_t i = 0; i < sizeof(objDriverCfg.m_aryDriver) / sizeof(objDriverCfg.m_aryDriver[0]); i ++ )
		{
			if( objDriverCfg.m_aryDriver[i].m_wDriverNo == wCurDriverNo )
			{
				memcpy( szID, objDriverCfg.m_aryDriver[i].m_szIdentID, sizeof(szID) );
				bytAnsType = 1;
				break;
			}
		}
	}
	else
	{
		bytAnsType = 1;
	}

	char buf[ 64 ] = { 0 };
	buf[0] = 0x64;
	buf[1] = bytAnsType;
	buf[2] = wCurDriverNo / 0x100;
	buf[3] = wCurDriverNo % 0x100;
	buf[4] = BYTE( 0 == szID[sizeof(szID) - 1] ? strlen(szID) : sizeof(szID) );
	if( BYTE(buf[4]) < 8 ) memcpy( buf + 5, szID, BYTE(buf[4]) );
	DataPush( buf, 5 + BYTE(buf[4]), DEV_QIAN, DEV_DIAODU, 2 );

	return 0;
}

// ˾��ȡ����½Ӧ��
int CDriverCtrl::DealComu6c( char* v_szData, DWORD v_dwDataLen )
{
	return 0;
}

#endif
