// CfgMng.cpp: implementation of the CCfgMng class.
//
//////////////////////////////////////////////////////////////////////

#include "CfgMng.h"
#include "ComuServDef.h"
#include "mtd-abi.h"
#include "mtd-user.h"
#include "../GlobalShare/GlobFunc.h"

#undef MSG_HEAD
#define MSG_HEAD ("ComuServ-CfgMng")

#define  BLOCK_6	("/dev/mtd/6")

// ����˳��	ImpCfg,ImpCfg Bkp,SecCfg,SecCfg Bkp (ÿ��������СΪһ�����С,128K,��0x20000)
const DWORD IMPCFG_SIZE = sizeof(tagImportantCfg);
const DWORD SECCFG_SIZE = sizeof(tagSecondCfg);
const DWORD ONECFG_WHOLESIZE = 128 * 1024;
#define CFGCHKSTR ("��Ѹ��Ʒ��������") // �����ı� (�� const char[]�������ͨ����)

CCfgMng::CCfgMng()
{
	m_iSemID = -1;
	m_iShmIDImpCfg = -1;
	m_iShmIDSecCfg = -1;
	m_pMemShareImpCfg = NULL;
	m_pMemShareSecCfg = NULL;
	m_bInit = false;

	_SemCreate(); // ��
	_ShmCreate(); // ��

	_SemP();

	_InitImp();
	_InitSec();

	_SemV();
}

CCfgMng::~CCfgMng()
{
	
}

int CCfgMng::GetImpCfg( void *v_pDes, unsigned int v_uiDesSize, unsigned int v_uiGetBegin, unsigned int v_uiGetSize )
{
	if( !v_pDes ) return ERR_PTR;
	if( v_uiDesSize < v_uiGetSize ) return ERR_SPACELACK;
	if( v_uiGetBegin + v_uiGetSize > sizeof(tagImportantCfg) ) return ERR_PAR;
	
	_SemP();

	memcpy( v_pDes, m_pMemShareImpCfg + v_uiGetBegin, v_uiGetSize );
	
	_SemV();
	
	return 0;
}


int CCfgMng::GetSecCfg( void *v_pDes, unsigned int v_uiDesSize, unsigned int v_uiGetBegin, unsigned int v_uiGetSize )
{
	if( !v_pDes ) return ERR_PTR;
	if( v_uiDesSize < v_uiGetSize ) return ERR_SPACELACK;
	if( v_uiGetBegin + v_uiGetSize > sizeof(tagSecondCfg) ) return ERR_PAR;

	_SemP();

	memcpy( v_pDes, m_pMemShareSecCfg + v_uiGetBegin, v_uiGetSize );
	
	_SemV();

	return 0;
}



int CCfgMng::SetImpCfg( void *v_pSrc, unsigned int v_uiSaveBegin, unsigned int v_uiSaveSize )
{
	if( !v_pSrc ) return ERR_PTR;
	if( v_uiSaveBegin + v_uiSaveSize > sizeof(tagImportantCfg) )
	{
		return ERR_PAR;
	}
	
	int iRet = 0;
	
	tagImportantCfg objImpCfg;
	objImpCfg.InitDefault();

	_SemP();

	memcpy( &objImpCfg, m_pMemShareImpCfg, sizeof(objImpCfg) );
	memcpy( (char*)(&objImpCfg) + v_uiSaveBegin, v_pSrc, v_uiSaveSize );
	
	iRet = _CfgSetImp( &objImpCfg, sizeof(objImpCfg) );
	if( !iRet )
	{
		memcpy( m_pMemShareImpCfg, &objImpCfg, sizeof(objImpCfg) ); // �����ڴ��л���
	}

	_SemV();
	
	return iRet;
}


int CCfgMng::SetSecCfg( void *v_pSrc, unsigned int v_uiSaveBegin, unsigned int v_uiSaveSize )
{
	if( !v_pSrc ) return ERR_PTR;
	if( v_uiSaveBegin + v_uiSaveSize > sizeof(tagSecondCfg) )
	{
		return ERR_PAR;
	}

	int iRet = 0;
	tagSecondCfg objSecCfg;
	objSecCfg.InitDefault();

	_SemP();

	memcpy( &objSecCfg, m_pMemShareSecCfg, sizeof(objSecCfg) );
	memcpy( (char*)(&objSecCfg) + v_uiSaveBegin, v_pSrc, v_uiSaveSize );

	iRet = _CfgSetSec( &objSecCfg, sizeof(objSecCfg) );
	if( !iRet )
	{
		memcpy( m_pMemShareSecCfg, &objSecCfg, sizeof(objSecCfg) ); // �����ڴ��л���
	}

	_SemV();

	return iRet;
}


int CCfgMng::ResumeCfg( bool v_bReservTel, bool v_bReservIP )
{
	int iRet = 0;

	_SemP();

	iRet = _ResumeImpCfg( v_bReservTel, v_bReservIP );
	if(0 == iRet) iRet = _ResumeSecCfg();

	_SemV();

	return iRet;
}

/// ÿ�����������󽫵���һ��
int CCfgMng::_InitImp()
{	
	/// ��ʧ�ܺ���Ŷ�һ��,������ʧ��,�Ժ󶼲�����
	
	int iRet = 0;
	tagImportantCfg objImpCfg;
	tag1QIpCfg obj1QIpCfg[3];

	memset( &objImpCfg, 0, sizeof(objImpCfg) );
	memcpy( &objImpCfg, m_pMemShareImpCfg, sizeof(objImpCfg) );

	if( strncmp(CFGSPEC, objImpCfg.m_szSpecCode, strlen(CFGSPEC)) ) // ���ڴ��е����û�δ��ʼ��
	{
		iRet = _CfgGetImp( &objImpCfg, sizeof(objImpCfg) );
		if( iRet ) // ����ʧ��
		{
			iRet = _CfgGetImp( &objImpCfg, sizeof(objImpCfg) );
		}

		if( iRet || strncmp(CFGSPEC, objImpCfg.m_szSpecCode, strlen(CFGSPEC)) ) // ����ʧ�ܻ�������������
		{
			char buf[100] = { 0 };
			memcpy( buf, objImpCfg.m_szSpecCode, sizeof(CFGSPEC) );
			PRTMSG( MSG_DBG, "To Init Imp Cfg Because %d,%s\n", iRet, buf );

			// ����ԭ�ֻ���
			char szTel[ sizeof(objImpCfg.m_uni1PComuCfg.m_obj1PComuCfg.m_szTel) ];
			memcpy( szTel, objImpCfg.m_uni1PComuCfg.m_obj1PComuCfg.m_szTel, sizeof(szTel) );

			// ����Ҫ�����ֻ��š�IP���˿ڵ�
			bool bSaveIpAndPort = false;
			if( 0 == strncmp(NEED_RUSEMUCFG, objImpCfg.m_szSpecCode, strlen(NEED_RUSEMUCFG)) )
			{
				memcpy((void*)obj1QIpCfg, (void*)objImpCfg.m_uni1QComuCfg.m_ary1QIpCfg, 3*sizeof(tag1QIpCfg));
				bSaveIpAndPort = true;
			}
			
			// ԭ�ֻ��źϷ��Լ��
			bool bValid = true;
			for( DWORD dw = 0; dw < sizeof(szTel); dw ++ )
			{
				if( 0 == szTel[dw] ) break;
				
				if( szTel[dw] > 0x39 || szTel[dw] < 0x30 )
				{
					bValid = false;
					break;
				}
			}
			//if( dw != 11 ) bValid = false; // ������11������,�򲻺Ϸ�


			// �ó�ʼֵ��ʼ���ڴ��е�����
			objImpCfg.InitDefault();

			if( bValid ) // ��ԭ�ֻ��źϷ���ָ�ԭ�ֻ���
			{
				// �ָ�ԭ�ֻ���
				memcpy( &objImpCfg.m_uni1PComuCfg.m_obj1PComuCfg.m_szTel, szTel,
					sizeof(objImpCfg.m_uni1PComuCfg.m_obj1PComuCfg.m_szTel) );
			}

			// �ָ�IP���˿�����
			if( bSaveIpAndPort )
			{
				memcpy((void*)objImpCfg.m_uni1QComuCfg.m_ary1QIpCfg,(void*)obj1QIpCfg,3*sizeof(tag1QIpCfg));
			}

			// д������
			iRet = _CfgSetImp( &objImpCfg, sizeof(objImpCfg) );

			PRTMSG( MSG_DBG, "Init Default ImpCfg!\n" );

			//_SetApnInReg( (char*)&(objImpCfg.m_uni1QComuCfg.m_ary1QIpCfg[0]) ); // ��������Ķ�������,����ʹ��Ĭ������,��Ҫ��APNд��ע���
		}
		else
		{
			PRTMSG( MSG_DBG, "Read ImpCfg Succ!\n" );
		}

		// �����ڴ��л���
		memcpy( m_pMemShareImpCfg, &objImpCfg, sizeof(objImpCfg) );
	}

	PRTMSG( MSG_DBG, "Init ImpCfg End: %s!\n", iRet ? "Fail" : "Succ" );
	
	return iRet;
}

/// ÿ�����������󽫵���һ��
int CCfgMng::_InitSec()
{
	/// ��ʧ�ܺ���Ŷ�һ��,������ʧ��,�Ժ󶼲�����
	int iRet = 0;
	tagSecondCfg objSecCfg;
//	tag2DIpPortPar obj2DIpPortPar;

	memset( &objSecCfg, 0, sizeof(objSecCfg) );
	memcpy( &objSecCfg, m_pMemShareSecCfg, sizeof(objSecCfg) );

	if( strncmp(CFGSPEC, objSecCfg.m_szSpecCode, strlen(CFGSPEC)) ) // ���ڴ��е����û�δ��ʼ��
	{
		iRet = _CfgGetSec( &objSecCfg, sizeof(objSecCfg) );
		if( iRet )
		{
			iRet = _CfgGetSec( &objSecCfg, sizeof(objSecCfg) );
		}

		// 080917,��������080899�޸�,ֻ���������ݡ�λ�ò�ѯӦ��Ͷ���ͨ���·����õ�Ӧ���߶���ͨ��
		// ǿ�ƹرն��ű��ݣ��������
		objSecCfg.m_uni2QGprsCfg.m_obj2QGprsCfg.m_bytChannelBkType_1 = 0;
		objSecCfg.m_uni2QGprsCfg.m_obj2QGprsCfg.m_bytChannelBkType_2 = 0;
		
		if( iRet || strncmp(CFGSPEC, objSecCfg.m_szSpecCode, strlen(CFGSPEC)) ) // ����ʧ�ܻ������ݲ�����
		{
			char buf[100] = { 0 };
			memcpy( buf, objSecCfg.m_szSpecCode, sizeof(CFGSPEC) );
			PRTMSG( MSG_DBG, "To Init Sec Cfg Because %d,%s\n", iRet, buf );

// 			// ����Ҫ�����ֻ��š�IP���˿ڵ�
// 			bool bSaveIpAndPort = false;
// 			if( 0 == strncmp(NEED_RUSEMUCFG, objSecCfg.m_szSpecCode, strlen(NEED_RUSEMUCFG)) )
// 			{
// 				memcpy((void*)&obj2DIpPortPar, (void*)&objSecCfg.m_uni2DDvrCfg.m_obj2DDvrCfg.IpPortPara, sizeof(tag2DIpPortPar));
// 				bSaveIpAndPort = true;
// 			}

			// �ó�ʼֵ��ʼ���ڴ�
			objSecCfg.InitDefault();

// 			if( bSaveIpAndPort )
// 			{
// 				memcpy((void*)&objSecCfg.m_uni2DDvrCfg.m_obj2DDvrCfg.IpPortPara, (void*)&obj2DIpPortPar, sizeof(tag2DIpPortPar));
// 			}

			// д������
			iRet = _CfgSetSec( &objSecCfg, sizeof(objSecCfg) );

			PRTMSG( MSG_DBG, "Init Default SecCfg!\n" );
			//PRTMSG( MSG_DBG, "SecCfg SpecCode: %s\n", objSecCfg.m_szSpecCode );
		}
		else
		{
			PRTMSG( MSG_DBG, "Read SecCfg Succ!\n" );
		}
		
		// �����ڴ��л���
		memcpy( m_pMemShareSecCfg, &objSecCfg, sizeof(objSecCfg) );
	}

	PRTMSG( MSG_DBG, "Init SecCfg End: %s!\n", iRet ? "Fail" : "Succ" );
	
	return iRet;
}

/// ���ӿڲ��⸴λǧ��������,����λ��ý�������
int CCfgMng::_ResumeImpCfg( bool v_bReservTel, bool v_bReservIP )
{
	int iRet = 0;
	tagImportantCfg objImpCfg;
	char szTel[ sizeof(objImpCfg.m_uni1PComuCfg.m_obj1PComuCfg.m_szTel) ] = { 0 };

	tag1QIpCfg obj1QIpCfg[3];
	tag1LComuCfg obj1LComuCfg;

	// ��ȡ��ǰ����
	memcpy( &objImpCfg, m_pMemShareImpCfg, sizeof(objImpCfg) );

	if( v_bReservTel )
	{
		// ����ԭ�ֻ���
		memcpy( szTel, objImpCfg.m_uni1PComuCfg.m_obj1PComuCfg.m_szTel, sizeof(szTel) );

		// ԭ�ֻ��źϷ��Լ��
		bool bValid = true;
		for( DWORD dw = 0; dw < sizeof(szTel); dw ++ )
		{
			if( 0 == szTel[dw] ) break;

			if( szTel[dw] > 0x39 || szTel[dw] < 0x30 )
			{
				bValid = false;
				break;
			}
		}
		//if( dw != 11 ) bValid = false; // ������11������,�򲻺Ϸ�
		if( !bValid ) memset( szTel, 0, sizeof(szTel) ); // ���Ϸ���ԭ�ֻ�����0
	}

	// ����IP���˿�
	if( v_bReservIP )
	{
		memcpy((void*)obj1QIpCfg, (void*)objImpCfg.m_uni1QComuCfg.m_ary1QIpCfg, 3*sizeof(tag1QIpCfg) );
		memcpy((void*)&obj1LComuCfg, (void*)&objImpCfg.m_uni1LComuCfg.m_obj1LComuCfg, sizeof(tag1LComuCfg));
	}

	// ��ʼ��Ĭ������
	objImpCfg.InitDefault();

	// �ָ�ԭ�ֻ���
	memcpy( &objImpCfg.m_uni1PComuCfg.m_obj1PComuCfg.m_szTel, szTel,
		sizeof(objImpCfg.m_uni1PComuCfg.m_obj1PComuCfg.m_szTel) );

	// �ָ�ԭIP���˿�����
	if( v_bReservIP )
	{
		memcpy((void*)objImpCfg.m_uni1QComuCfg.m_ary1QIpCfg, (void*)obj1QIpCfg, 3*sizeof(tag1QIpCfg) );
		memcpy( (void*)&objImpCfg.m_uni1LComuCfg.m_obj1LComuCfg, (void*)&obj1LComuCfg, sizeof(tag1LComuCfg));
	}

	// д����
	iRet = _CfgSetImp( &objImpCfg, sizeof(objImpCfg) );
	if( !iRet )
	{
		// ���¹����ڵ�����
		memcpy( m_pMemShareImpCfg, &objImpCfg, sizeof(objImpCfg) );

		// д���óɹ���,����Ӧ���µ�APN
		//_SetApnInReg( (char*)&(objImpCfg.m_uni1QComuCfg.m_ary1QIpCfg[0]) );
	}

	return iRet;
}

int CCfgMng::_ResumeSecCfg()
{
	int iRet = 0;
	tagSecondCfg objSecCfg;
	
	// ��ʼ��Ĭ������
	objSecCfg.InitDefault();

	// д����
	iRet = _CfgSetSec( &objSecCfg, sizeof(objSecCfg) );
	if( !iRet )
	{
		// ���¹����ڵ�����
		memcpy( m_pMemShareSecCfg, &objSecCfg, sizeof(objSecCfg) );
	}
	
	return iRet;
}

// bool CCfgMng::_SetApnInReg( const char *const v_szImpCfg )
// {
// 	HKEY hKey;
// 	DWORD dwDisposition;
// 	char szApn[32] = { 0 };
// 
// 	memcpy( szApn, ((tag1QIpCfg*)v_szImpCfg)->m_szApn, sizeof(szApn) );
// 	szApn[ sizeof(szApn) - 1 ] = 0;
// 	char* pBlank = strchr( szApn, 0x20 );
// 	if( pBlank ) *pBlank = 0;
// 
// 	if( strcmp("", szApn) ) // �������е�APN���ǿյ�
// 	{
// 		wchar_t wszApn[32] = { 0 };
// 		wchar_t wszVal[128] = { 0 };
// 
// 		::MultiByteToWideChar(CP_ACP, 0, szApn, strlen(szApn), wszApn, sizeof(wszApn) / sizeof(wszApn[0]) );
// 		wszApn[ sizeof(wszApn) / sizeof(wszApn[0]) - 1 ] = 0;
// 		wsprintf( wszVal, L"AT+CGDCONT=1,\"IP\",\"%s\"<cr>", wszApn );
// 		
// 		LONG LResult = ::RegCreateKeyEx( HKEY_LOCAL_MACHINE, L"ExtModems\\HayesCompat7\\Init", 0, NULL, 0, 0, NULL, &hKey, &dwDisposition );
// 		if( ERROR_SUCCESS == LResult )
// 		{
// 			LResult = ::RegSetValueEx( hKey, L"3", 0, REG_SZ, (BYTE*)wszVal, wcslen(wszVal) * 2 );
// 			//LResult = ::RegSetValueEx( hKey, L"3", 0, REG_SZ, (BYTE*)(L"123"), 6 );
// 			::RegCloseKey(hKey);
// 			::RegCloseKey(HKEY_LOCAL_MACHINE);
// 			return ERROR_SUCCESS == LResult ? true : false;
// 		}
// 	}
// 
// 	return false;
// }

int CCfgMng::_CfgGetImp( void* v_szGet, size_t v_sizGet )
{
	int iRet = 0;
	int iCfgDri = -1;
	off_t ofstGet = 0;
	char szTmp[ strlen(CFGCHKSTR) ] = { 0 };

	if( v_sizGet > ONECFG_WHOLESIZE )
	{
		iRet = ERR_PAR;
		goto _CFGGETIMP_END;
	}

	// ���ļ�������
	iCfgDri = open( BLOCK_6, O_RDWR );
	if( -1 == iCfgDri )
	{
		iRet = ERR_OPENFAIL;
		goto _CFGGETIMP_END;
	}

	// ��ת����������У���ֽڶ�ȡ��
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE - strlen(CFGCHKSTR), SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		goto _CFGGETIMP_END;
	}

	// ��ȡУ���ֽ�
	if( strlen(CFGCHKSTR) != size_t(read(iCfgDri, szTmp, strlen(CFGCHKSTR))) )
	{
		iRet = ERR_READFAIL;
		goto _CFGGETIMP_END;
	}

	// ��У���ֽڲ���ȷ
	if( strncmp(CFGCHKSTR, szTmp, strlen(CFGCHKSTR)) )
	{
		PRTMSG( MSG_ERR, "_CfgGetImp Main Area ChkSum Err\n" );

		// ��ת���������ı�������У���ֽڶ�ȡ��
		if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE * 2 - strlen(CFGCHKSTR), SEEK_SET) )
		{
			iRet = ERR_LSEEKFAIL;
			goto _CFGGETIMP_END;
		}
		
		// ��ȡУ���ֽ�
		if( strlen(CFGCHKSTR) != size_t(read(iCfgDri, szTmp, strlen(CFGCHKSTR))) )
		{
			iRet = ERR_READFAIL;
			goto _CFGGETIMP_END;
		}

		// �ж�У���ֽ�
		if( strncmp(CFGCHKSTR, szTmp, strlen(CFGCHKSTR)) )
		{
			iRet = ERR_CFG;
			goto _CFGGETIMP_END;
		}

		// ȷ����ȡλ��
		ofstGet = ONECFG_WHOLESIZE;
	}
	else
	{
		// ȷ����ȡλ��
		ofstGet = 0;
	}

	// ��ת����ȡ���ݴ�
	if( -1 == lseek( iCfgDri, ofstGet, SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		goto _CFGGETIMP_END;
	}

	// ��ȡ����
	if( v_sizGet != size_t(read(iCfgDri, v_szGet, v_sizGet)) )
	{
		iRet = ERR_READFAIL;
		goto _CFGGETIMP_END;
	}

_CFGGETIMP_END:
	if( -1 != iCfgDri ) close( iCfgDri );

	if( iRet )
	{
		PRTMSG( MSG_ERR, "_CfgGetImp Fail:%d\n", iRet );
	}

	return iRet;
}

int CCfgMng::_CfgSetImp( void* v_szSet, size_t v_sizSetLen )
{
	int iRet = 0;
	int iCfgDri = -1;
	erase_info_t objErase;
	
	if( v_sizSetLen > ONECFG_WHOLESIZE )
	{
		iRet = ERR_PAR;
		goto _CFGSETIMP_END;
	}
	
	// ���ļ�������
	iCfgDri = open( BLOCK_6, O_RDWR );
	if( -1 == iCfgDri )
	{
		iRet = ERR_OPENFAIL;
		printf("open /dev/mtd/6 failed");
		perror("");
		goto _CFGSETIMP_END;
	}

	// ����
	objErase.start = 0;
	objErase.length = ONECFG_WHOLESIZE;
	if( -1 == ioctl(iCfgDri, MEMERASE, &objErase) )
	{
		iRet = ERR_IOCTLFAIL;
		printf("erase /dev/mtd/6 failed");
		perror("");
		goto _CFGSETIMP_END;
	}

	// ��ת��д���ݴ�
	if( -1 == lseek( iCfgDri, 0, SEEK_SET) )
	{
		printf("lseek /dev/mtd/6 failed 1");
		perror("");
		iRet = ERR_LSEEKFAIL;
		goto _CFGSETIMP_END;
	}
	
	// д����
	if( (ssize_t)v_sizSetLen != write(iCfgDri, v_szSet, v_sizSetLen) )
	{
		printf("write /dev/mtd/6 failed");
		perror("");
		iRet = ERR_WRITEFAIL;
		goto _CFGSETIMP_END;
	}
	
	// ��ת��У���ֽڴ�
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE - strlen(CFGCHKSTR), SEEK_SET) )
	{
		printf("lseek /dev/mtd/6 failed 2");
		perror("");
		iRet = ERR_LSEEKFAIL;
		goto _CFGSETIMP_END;
	}

	// д��У���ֽ�
	if( strlen(CFGCHKSTR) != size_t(write(iCfgDri, CFGCHKSTR, strlen(CFGCHKSTR))) )
	{
		printf("lseek /dev/mtd/6 failed 1");
		perror("");
		iRet = ERR_WRITEFAIL;
		goto _CFGSETIMP_END;
	}

	
	// ����������
	objErase.start = ONECFG_WHOLESIZE;
	objErase.length = ONECFG_WHOLESIZE;
	if( -1 == ioctl(iCfgDri, MEMERASE, &objErase) )
	{
		iRet = ERR_IOCTLFAIL;
		goto _CFGSETIMP_END;
	}

	// ��ת��������д���ݴ�
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE, SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		goto _CFGSETIMP_END;
	}
	
	// д���ݵ�������
	if( (ssize_t)v_sizSetLen != write(iCfgDri, v_szSet, v_sizSetLen) )
	{
		iRet = ERR_WRITEFAIL;
		goto _CFGSETIMP_END;
	}
	
	// ��ת��������У���ֽڴ�
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE * 2 - strlen(CFGCHKSTR), SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		goto _CFGSETIMP_END;
	}
			
	// д��У���ֽڵ�������
	if( strlen(CFGCHKSTR) != size_t(write(iCfgDri, CFGCHKSTR, strlen(CFGCHKSTR))) )
	{
		iRet = ERR_WRITEFAIL;
		goto _CFGSETIMP_END;
	}
	
_CFGSETIMP_END:
	if( -1 != iCfgDri ) close( iCfgDri );

	if( iRet )
	{
		PRTMSG( MSG_ERR, "_CfgSetImp Fail:%d\n", iRet );
	}

	return iRet;
}

int CCfgMng::_CfgGetSec( void* v_szGet, size_t v_sizGet )
{
	int iRet = 0;
	int iCfgDri = -1;
	off_t ofstGet = 0;
	char szTmp[ strlen(CFGCHKSTR) ] = { 0 };
	
	if( v_sizGet > ONECFG_WHOLESIZE )
	{
		iRet = ERR_PAR;
		goto _CFGGETSEC_END;
	}
	
	// ���ļ�������
	iCfgDri = open( BLOCK_6, O_RDWR );
	if( -1 == iCfgDri )
	{
		iRet = ERR_OPENFAIL;
		goto _CFGGETSEC_END;
	}
	
	// ��ת����������У���ֽڶ�ȡ��
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE * 3 - strlen(CFGCHKSTR), SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		goto _CFGGETSEC_END;
	}
	
	// ��ȡУ���ֽ�
	if( strlen(CFGCHKSTR) != size_t(read(iCfgDri, szTmp, strlen(CFGCHKSTR))) )
	{
		iRet = ERR_READFAIL;
		goto _CFGGETSEC_END;
	}

// 	{
// 		lseek( iCfgDri, ONECFG_WHOLESIZE * 2, SEEK_SET);
// 
// 		char buf[100] = { 0 };
// 		read( iCfgDri, buf, sizeof(buf) );
// 		PRTMSG( MSG_DBG, "_CfgGetSec SpecCode__: %s\n", buf );
// 	}
	
	// ��У���ֽڲ���ȷ
	if( strncmp(CFGCHKSTR, szTmp, strlen(CFGCHKSTR)) )
	{
		PRTMSG( MSG_ERR, "_CfgGetSec Main Area ChkSum Err\n" );

		// ��ת���������ı�������У���ֽڶ�ȡ��
		if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE * 4 - strlen(CFGCHKSTR), SEEK_SET) )
		{
			iRet = ERR_LSEEKFAIL;
			goto _CFGGETSEC_END;
		}
		
		// ��ȡУ���ֽ�
		if( strlen(CFGCHKSTR) != size_t(read(iCfgDri, szTmp, strlen(CFGCHKSTR))) )
		{
			iRet = ERR_READFAIL;
			goto _CFGGETSEC_END;
		}
		
		// �ж�У���ֽ�
		if( strncmp(CFGCHKSTR, szTmp, strlen(CFGCHKSTR)) )
		{
			iRet = ERR_CFG;
			goto _CFGGETSEC_END;
		}
		
		// ȷ����ȡλ��
		ofstGet = ONECFG_WHOLESIZE * 3;
	}
	else
	{
		// ȷ����ȡλ��
		ofstGet = ONECFG_WHOLESIZE * 2;
	}
	
	// ��ת����ȡ���ݴ�
	if( -1 == lseek( iCfgDri, ofstGet, SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		goto _CFGGETSEC_END;
	}
	
	// ��ȡ����
	if( (ssize_t)v_sizGet != read(iCfgDri, v_szGet, v_sizGet) )
	{
		iRet = ERR_READFAIL;
		goto _CFGGETSEC_END;
	}
	
_CFGGETSEC_END:
	if( -1 != iCfgDri ) close( iCfgDri );

	if( iRet )
	{
		PRTMSG( MSG_ERR, "_CfgGetSec Fail:%d\n", iRet );
	}

	return iRet;
}

int CCfgMng::_CfgSetSec( void* v_szSet, size_t v_sizSetLen )
{
	DWORD dw1 = GetTickCount();
	DWORD dw2, dw3;

	int iRet = 0;
	int iCfgDri = -1;
	erase_info_t objErase;
	
	if( v_sizSetLen > ONECFG_WHOLESIZE )
	{
		iRet = ERR_PAR;
		goto _CFGSETSEC_END;
	}
	
	// ���ļ�������
	iCfgDri = open( BLOCK_6, O_RDWR );
	if( -1 == iCfgDri )
	{
		iRet = ERR_OPENFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x01);
		goto _CFGSETSEC_END;
	}
	
	// ����
	objErase.start = ONECFG_WHOLESIZE * 2;
	objErase.length = ONECFG_WHOLESIZE;
	if( -1 == ioctl(iCfgDri, MEMERASE, &objErase) )
	{
		iRet = ERR_IOCTLFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x02);
		goto _CFGSETSEC_END;
	}
	
	// ��ת��д���ݴ�
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE * 2, SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x03);
		goto _CFGSETSEC_END;
	}
	
	// д����
	if( (ssize_t)v_sizSetLen != write(iCfgDri, v_szSet, v_sizSetLen) )
	{
		iRet = ERR_WRITEFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x04);
		goto _CFGSETSEC_END;
	}
	
	// ��ת��У���ֽڴ�
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE * 3 - strlen(CFGCHKSTR), SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x05);
		goto _CFGSETSEC_END;
	}
	
	// д��У���ֽ�
	if( strlen(CFGCHKSTR) != size_t(write(iCfgDri, CFGCHKSTR, strlen(CFGCHKSTR))) )
	{
		iRet = ERR_WRITEFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x06);
		goto _CFGSETSEC_END;
	}

	dw2 = GetTickCount();
	
	
	// ����������
	objErase.start = ONECFG_WHOLESIZE * 3;
	objErase.length = ONECFG_WHOLESIZE;
	if( -1 == ioctl(iCfgDri, MEMERASE, &objErase) )
	{
		iRet = ERR_IOCTLFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x07);
		goto _CFGSETSEC_END;
	}
	
	// ��ת��������д���ݴ�
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE * 3, SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x08);
		goto _CFGSETSEC_END;
	}
	
	// д���ݵ�������
	if( (ssize_t)v_sizSetLen != write(iCfgDri, v_szSet, v_sizSetLen) )
	{
		iRet = ERR_WRITEFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x09);
		goto _CFGSETSEC_END;
	}
	
	// ��ת��������У���ֽڴ�
	if( -1 == lseek( iCfgDri, ONECFG_WHOLESIZE * 4 - strlen(CFGCHKSTR), SEEK_SET) )
	{
		iRet = ERR_LSEEKFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x0a);
		goto _CFGSETSEC_END;
	}
	
	// д��У���ֽڵ�������
	if( strlen(CFGCHKSTR) != size_t(write(iCfgDri, CFGCHKSTR, strlen(CFGCHKSTR))) )
	{
		iRet = ERR_WRITEFAIL;
		RenewMemInfo(0x02, 0x00, 0x01, 0x0b);
		goto _CFGSETSEC_END;
	}

	dw3 = GetTickCount();

	//PRTMSG( MSG_DBG, "_CfgSetSec SpecCode__: %s\n", v_szSet );
	
_CFGSETSEC_END:
	if( -1 != iCfgDri ) close( iCfgDri );

	if( iRet )
	{
		PRTMSG( MSG_ERR, "_CfgSetSec Fail:%d\n", iRet );
	}
	else
	{
		PRTMSG( MSG_DBG, "_CfgSetSec cost1:%d, cost2:%d\n", int(dw2-dw1), int(dw3-dw2) );
	}

	return iRet;
}

int CCfgMng::_SemCreate()
{
	int iRet = 0;
	
	bool bFstCreate = false;
	m_iSemID = semget( (key_t)KEY_SEM_CFG, 1, 0666 | IPC_CREAT | IPC_EXCL );
	if( -1 == m_iSemID )
	{
		m_iSemID = semget( (key_t)KEY_SEM_CFG, 1, 0666 | IPC_CREAT );
		if( -1 == m_iSemID )
		{
			iRet = ERR_SEMGET;
			PRTMSG( MSG_ERR, "semget fail\n" );
			goto _SEM_CREATE_END;
		}
		else
		{
			PRTMSG( MSG_DBG, "semget succ\n" );
		}
	}
	else
	{
		bFstCreate = true;
		PRTMSG( MSG_DBG, "semget succ(first)\n" );
	}
	
	if( bFstCreate )
	{
		semun sem_union;
		sem_union.val = 1;
		if( -1 == semctl( m_iSemID, 0, SETVAL, sem_union ) )
		{
			iRet = ERR_SEMCTL;
			PRTMSG( MSG_ERR, "semctl SETVAL fail\n" );
			goto _SEM_CREATE_END;
		}
		else
		{
			PRTMSG( MSG_DBG, "semctl SETVAL succ\n" );
		}
	}
	
_SEM_CREATE_END:
	return iRet;
}

int CCfgMng::_SemV()
{
	sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	
	if( -1 == semop(m_iSemID, &sem_b, 1) )
	{
		PRTMSG( MSG_ERR, "semop V falied\n");
		return ERR_SEMOP;
	}
	
	return 0;
}

int CCfgMng::_SemP()
{
	sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;
	
	if( -1 == semop(m_iSemID, &sem_b, 1) )
	{
		PRTMSG( MSG_ERR, "semop P falied\n");
		return ERR_SEMOP;
	}
	
	return 0;
}

int CCfgMng::_ShmCreate()
{
	int iRet = 0;
	void* pShare = NULL;

	// ������Ҫ�����������ڴ�
	m_iShmIDImpCfg = shmget( (key_t)KEY_SHM_IMPCFG, sizeof(tagImportantCfg), 0666 | IPC_CREAT );
	if( -1 == m_iShmIDImpCfg )
	{
		iRet = ERR_SHMGET;
		PRTMSG( MSG_ERR, "impcfg shmget fail\n" ); 
		goto _SHM_CREATE_END;
	}
	else
	{
		PRTMSG( MSG_DBG, "impcfg shmget succ\n" ); 
	}
	
	// �ڴ�ӳ����Ҫ������ (����Ҫ�������ʼ���ڴ�)
	pShare = shmat( m_iShmIDImpCfg, 0, 0 );
	if( (void*)-1 == pShare )
	{
		iRet = ERR_SHMAT;
		PRTMSG( MSG_ERR, "impcfg shmat fail\n" );
		goto _SHM_CREATE_END;
	}
	else
	{
		m_pMemShareImpCfg = (char*)pShare;
		PRTMSG( MSG_DBG, "impcfg shmat succ\n" );
	}


	// ����һ�������������ڴ�
	m_iShmIDSecCfg = shmget( (key_t)KEY_SHM_SECCFG, sizeof(tagSecondCfg), 0666 | IPC_CREAT );
	if( -1 == m_iShmIDSecCfg )
	{
		iRet = ERR_SHMGET;
		PRTMSG( MSG_ERR, "seccfg shmget fail\n" ); 
		goto _SHM_CREATE_END;
	}
	else
	{
		PRTMSG( MSG_DBG, "seccfg shmget succ\n" ); 
	}
	
	// �ڴ�ӳ����Ҫ������ (����Ҫ�������ʼ���ڴ�)
	pShare = shmat( m_iShmIDSecCfg, 0, 0 );
	if( (void*)-1 == pShare )
	{
		iRet = ERR_SHMAT;
		PRTMSG( MSG_ERR, "seccfg shmat fail\n" );
		goto _SHM_CREATE_END;
	}
	else
	{
		m_pMemShareSecCfg = (char*)pShare;
		PRTMSG( MSG_DBG, "seccfg shmat succ\n" );
	}
	
_SHM_CREATE_END:
	return iRet;
}

