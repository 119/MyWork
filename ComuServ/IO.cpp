#include "../GlobalShare/ComuServExport.h"
#include "IO.h"
#include "ComuServDef.h"

#if VEHICLE_TYPE == VEHICLE_M
#include "yx_gpio.h"
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
#include "YX_GPIO_API.h"
#endif

#undef MSG_HEAD
#define MSG_HEAD ("ComuServ-IO")

// int IOGet( unsigned char v_ucIOSymb, DWORD *v_p_dwIOSta );
// int IOSet( unsigned char v_ucIOSymb, char v_cIOSta,  void* v_pReserv, unsigned int v_uiReservSiz );
// int IOCfgGet( unsigned char v_ucIOSymb, void *v_pCfg, DWORD v_dwCfgSize );
// int IOIntFuncSet( unsigned char v_ucIOSymb, void* v_pIntFunc );
// 
// // ������SYSIO_CFG[]�Ķ���˳��һ��!�ұ����0��ʼ���
// enum IO_SYMB
// {
// 	IO_ACC = 0,
// };
// 
// // IOӦ�����嶨��,����ⲿIO�ź�,��CPU�ܽ��ź�
// enum IO_APP
// {
// 	IO_ACC_ON = 100,
// };

const char IOINIT_STR[] = "���߲�����,����������,ҹ��������,����֪����";
const size_t IO_MEMSHARE_SIZE = sizeof(tagIOSta) + sizeof(IOINIT_STR);

CIO::CIO()
{
	m_iSemID = -1;
	m_iShmID = -1;
	m_pMemShare = NULL;
//	m_bInit = false;

	_Init();
}

CIO::~CIO()
{
	//YX_GPIO_Release();
}

int CIO::IOGet( unsigned char v_ucIOSymb, unsigned char *v_p_ucIOSta )
{	// ����ֵ: 0,�͵�ƽ; 1,�ߵ�ƽ

/*	_Init();*/

	if( v_ucIOSymb >= sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]) )
	{
		PRTMSG( MSG_ERR, "IOGet %d: IOSymb Over Max Number\n", int(v_ucIOSymb) );
		return ERR_PAR;
	}

	if( 65535 == SYSIO_CFG[v_ucIOSymb].m_uiPinNo ) return ERR_IO;

	if( v_p_ucIOSta )
	{
		//_SemP();

		char cVal = YX_GPIO_GetValue( SYSIO_CFG[v_ucIOSymb].m_uiPinNo );

		//_SemV();

		*v_p_ucIOSta = (cVal == 0 ? SYSIO_CFG[v_ucIOSymb].m_ucPinLowApp : SYSIO_CFG[v_ucIOSymb].m_ucPinHigApp);

		//PRTMSG( MSG_DBG, "IOGet: pin-%3d, val-%d\n", SYSIO_CFG[v_ucIOSymb].m_uiPinNo, cVal );

		return 0;
	}
	else
	{
		return ERR_PTR;
	}
}

int CIO::IOSet( unsigned char v_ucIOSymb, unsigned char v_ucIOSta, void* v_pReserv, unsigned int v_uiReservSiz )
{
	// ����ֵ: 0,�͵�ƽ; 1,�ߵ�ƽ

	int iRet = 0;

	/*_Init();*/
	
	if( v_ucIOSymb >= sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]) )
	{
		PRTMSG( MSG_ERR, "IOSet %d: IOSymb Over Max Number\n", int(v_ucIOSymb) );
		return ERR_PAR;
	}

	if( 65535 == SYSIO_CFG[v_ucIOSymb].m_uiPinNo ) return ERR_IO;

	int iSetVal = ( v_ucIOSta == SYSIO_CFG[v_ucIOSymb].m_ucPinLowApp ? 0 : (v_ucIOSta == SYSIO_CFG[v_ucIOSymb].m_ucPinHigApp ? 1 : 2) );
	bool bDoSet = true;

	_SemP();

	tagIOSta objIOSta;
	memcpy( &objIOSta, m_pMemShare, sizeof(objIOSta) );
	
	switch( v_ucIOSymb )
	{
	case IOS_TRUMPPOW:
		{
			DWORD dwDevSymb = 0;
			if( v_pReserv && sizeof(dwDevSymb) == v_uiReservSiz )
			{
				memcpy( &dwDevSymb, v_pReserv, sizeof(dwDevSymb) );

				if( char(IO_TRUMPPOW_OFF) == v_ucIOSta ) // ��Ҫ�ع���
				{
					objIOSta.m_dwTRUMPOpenDevSymb &= ~dwDevSymb;
					if( objIOSta.m_dwTRUMPOpenDevSymb )
					{
						bDoSet = false; // �����������豸û�йرչ���,������
					}
				}
				else // Ҫ������
				{
					if( objIOSta.m_dwTRUMPOpenDevSymb )
					{
						//bDoSet = false; // �������Ѿ���,���ظ����� (Ҳ���Բ�����)
					}
					objIOSta.m_dwTRUMPOpenDevSymb |= dwDevSymb;
				}
			}
			else // ��û�д���Ϸ��ĸ��Ӳ���,�򷵻�ʧ��
			{
				bDoSet = false;
				iRet = ERR_PAR;
				PRTMSG( MSG_ERR, "IOSet %d: Reserv Par Shoud Not Null!\n", int(v_ucIOSymb) );
			}
		}
		break;

	case IOS_AUDIOSEL:
		{
			if( char(IO_AUDIOSEL_PHONE) == v_ucIOSta ) // ���л����ֻ���Ƶ���
			{
				objIOSta.m_bPhoneAudio = true;
			}
			else
			{
				objIOSta.m_bPhoneAudio = false;
			}
		}
		break;

	case IOS_EARPHONESEL:
		{
			DWORD dwDevSymb = 0;
			if( v_pReserv && sizeof(dwDevSymb) == v_uiReservSiz )
			{
				memcpy( &dwDevSymb, v_pReserv, sizeof(dwDevSymb) );

				if( objIOSta.m_bPhoneAudio && DEV_PHONE != dwDevSymb )
				{
					bDoSet = false; // �����л����ֻ���Ƶ���,�Ҵ�ʱ����Phone�Ŀ�������,�򲻴���
					PRTMSG( MSG_DBG, "IOSet: In Phone Mode, refuse to switch earphonesel from pc\n" );
				}
			}
			else // ��û�д���Ϸ��ĸ��Ӳ���,�򷵻�ʧ��
			{
				bDoSet = false;
				iRet = ERR_PAR;
				PRTMSG( MSG_ERR, "IOSet %d: Reserv Par Shoud Not Null!\n", int(v_ucIOSymb) );
			}
		}
		break;

	default:
		;
	}

	if( bDoSet )
	{
		YX_GPIO_SetValue( SYSIO_CFG[v_ucIOSymb].m_uiPinNo, iSetVal );
	}

	memcpy( m_pMemShare, &objIOSta, sizeof(objIOSta) );

	_SemV();

	//PRTMSG( MSG_DBG, "IOSet: pin-%3d, val-%d\n", SYSIO_CFG[v_ucIOSymb].m_uiPinNo, iSetVal );

	return iRet;
}

int CIO::IOCfgGet( unsigned char v_ucIOSymb, void *v_pIOCfg, DWORD v_dwCfgSize )
{
/*	_Init();*/

	if( v_ucIOSymb >= sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]) )
	{
		PRTMSG( MSG_ERR, "IOCfgGet %d: IOSymb Over Max Number\n", int(v_ucIOSymb) );
		return ERR_PAR;
	}

	if( v_pIOCfg && v_dwCfgSize >= sizeof(tagIOCfg) )
	{
		memcpy( v_pIOCfg, &SYSIO_CFG[v_ucIOSymb], sizeof(tagIOCfg) );
		PRTMSG( MSG_DBG, "IOCfgGet OK, No.%3d,Pin-%3d,prd-%4d,io-%d,brken-%d,brktyp-%d,brklvl-%d,low-%3d,hig-%3d\n", v_ucIOSymb,
			SYSIO_CFG[v_ucIOSymb].m_uiPinNo, SYSIO_CFG[v_ucIOSymb].m_uiContPrid, SYSIO_CFG[v_ucIOSymb].m_ucInOut,
			SYSIO_CFG[v_ucIOSymb].m_bBreak, SYSIO_CFG[v_ucIOSymb].m_cBreakType, SYSIO_CFG[v_ucIOSymb].m_cBreakPow,
			SYSIO_CFG[v_ucIOSymb].m_ucPinLowApp, SYSIO_CFG[v_ucIOSymb].m_ucPinHigApp );
		return 0;
	}
	else
	{
		PRTMSG( MSG_ERR, "IOCfgGet %d: output size(%d) lack!\n", int(v_ucIOSymb), int(v_dwCfgSize) );
		return ERR_PTR;
	}
}

// ����IO���жϳ�ʼ��,��IO.exe���ñ��ӿ����, �������v_ucIOSymb����ʹ������ֵ0xff
int CIO::IOIntInit( unsigned char v_ucIOSymb, void* v_pIntFunc )
{
	int iRet = 0;

	if( v_ucIOSymb >= sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]) && 0xff != v_ucIOSymb )
	{
		PRTMSG( MSG_ERR, "IOIntInit %d: IOSymb Over Max Number\n", int(v_ucIOSymb) );
		return ERR_PAR;
	}

	_SemP();

	if( 0xff == v_ucIOSymb )
	{
		DWORD dwIOTotal = sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]);
		for( DWORD dw = 0; dw < dwIOTotal; dw ++ )
		{
			if( 65535 == SYSIO_CFG[dw].m_uiPinNo || 0 != SYSIO_CFG[dw].m_ucInOut || !SYSIO_CFG[dw].m_bBreak ) // ���Ǳ���IO����,��������IO,�������ж�
			{
				continue; // ����
			}

			iRet = _IntSet( char(dw), v_pIntFunc );
			if( iRet ) break;
		}
	}
	else
	{
		if( 65535 == SYSIO_CFG[v_ucIOSymb].m_uiPinNo
			|| 0 != SYSIO_CFG[v_ucIOSymb].m_ucInOut
			|| !SYSIO_CFG[v_ucIOSymb].m_bBreak ) // ���Ǳ���IO����,��������IO,�������ж�
		{
			iRet = ERR_PAR;
		}
		else
		{
			iRet = _IntSet( v_ucIOSymb, v_pIntFunc );
		}
	}

	_SemV();
	
	return iRet;
}

// int CIO::IOIntFuncSet( unsigned char v_ucIOSymb, void* v_pIntFunc )
// {
// /*	_Init();*/
// 	
// 	if( v_ucIOSymb >= sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]) )
// 	{
// 		return ERR_PAR;
// 	}
// 	
// 	if( v_pIntFunc )
// 	{
// 		if( 0xff == v_ucIOSymb )
// 		{
// 			DWORD dwIOTotal = sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]);
// 			for( DWORD dw = 0; dw < dwIOTotal; dw ++ )
// 			{
// 				if( 65535 == SYSIO_CFG[dw].m_uiPinNo ) continue; // ���Ǳ���IO����������
// 
// 				YX_GPIO_INTSethandler( SYSIO_CFG[dw].m_uiPinNo, (void (*)(INT32U, INT32U))v_pIntFunc );
// 			}
// 		}
// 		else
// 		{
// 			YX_GPIO_INTSethandler( SYSIO_CFG[v_ucIOSymb].m_uiPinNo, (void (*)(INT32U, INT32U))v_pIntFunc );
// 		}
// 		PRTMSG( MSG_DBG, "Set IO(%d), Int Func: %08x\n", v_ucIOSymb, int(v_pIntFunc) );
// 		return 0;
// 	}
// 	else
// 	{
// 		return ERR_PTR;
// 	}
// }

int CIO::_Init()
{
//	if( m_bInit ) return 0;

	int iRet = 0;
	
	iRet = _SemCreate();
	if( iRet ) return iRet;

	iRet = _ShmCreate();
	if( iRet ) return iRet;
	
	/// ���´���ȷ������δ��ʼ��ʱ,һ���ܽ��г�ʼ�� (���ײ�������ڲ�����ʧ����û�취)

	{
	//	sleep(1);
	//	IOSet( IOS_OILELECCTL, IO_OILELECCTL_CUT, NULL, 0 );
	}

	bool bNeedInit = false;
	_SemP();

	if( strncmp(m_pMemShare + sizeof(tagIOCfg), IOINIT_STR, strlen(IOINIT_STR)) )
	{
		bNeedInit = true;
		memcpy( m_pMemShare + sizeof(tagIOCfg), IOINIT_STR, sizeof(IOINIT_STR) );
	}

	if( bNeedInit )
	{
#if VEHICLE_TYPE == VEHICLE_M
 		system( "insmod /dvs/extdrv/hi3510_gpio.ko");
		system( "insmod /dvs/extdrv/yx3510_gpio.ko");
#endif
	}

	YX_GPIO_Init();
	
	if( bNeedInit )
	{
		PRTMSG( MSG_DBG, "GPIO Init\n" );

		// ��ʼ��IO����
		DWORD dwIOTotal = sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]);
		char szMsg[ 300 ] = { 0 };
		char buf[ 50 ] = { 0 };
		for( DWORD dw = 0; dw < dwIOTotal; dw ++ )
		{
			if( 65535 == SYSIO_CFG[dw].m_uiPinNo ) continue; // ���Ǳ���IO����������

			memset( szMsg, 0, sizeof(szMsg) );
			sprintf(buf, "GPIO No.%2d: Pin:%3d,", int(dw), SYSIO_CFG[dw].m_uiPinNo );
			strcat( szMsg, buf );

			switch( SYSIO_CFG[dw].m_ucInOut )
			{
			case 0: // ����
				YX_GPIO_SetDirection( SYSIO_CFG[dw].m_uiPinNo, 0 );
				sprintf(buf, "Set Dir-%d,", SYSIO_CFG[dw].m_ucInOut );
				strcat( szMsg, buf );

				if( !SYSIO_CFG[dw].m_bBreak ) // �������ж�
				{
					YX_GPIO_INTDisable( SYSIO_CFG[dw].m_uiPinNo ); // ���ж�
 					strcat( szMsg, "Dsb Brk," );
				}
				break;

			case 1: // ���
				YX_GPIO_SetDirection( SYSIO_CFG[dw].m_uiPinNo, 1 );
				sprintf(buf, "Set Dir-%d,", SYSIO_CFG[dw].m_ucInOut );
				strcat( szMsg, buf );
				break;

			default:
				;
			}

			PRTMSG( MSG_DBG, "%s\n", szMsg );
		}
// 		m_bInit = true;
	}

	_SemV();
	
	return iRet;
}

int CIO::_IntSet( unsigned char v_ucIOSymb, void* v_pIntFunc )
{
	if( v_ucIOSymb >= sizeof(SYSIO_CFG) / sizeof(SYSIO_CFG[0]) || !v_pIntFunc )
	{
		return ERR_PAR;
	}

	// ��ʼ������IO���жϿ���
	char szMsg[ 300 ] = { 0 };
	char buf[ 80 ] = { 0 };
	
	sprintf(buf, "GPIO No.%2d: Pin:%3d,", int(v_ucIOSymb), SYSIO_CFG[v_ucIOSymb].m_uiPinNo );
	strcat( szMsg, buf );
	
	switch( SYSIO_CFG[v_ucIOSymb].m_cBreakType )
	{
	case 1: // ��ƽ����
		YX_GPIO_INTConfig( SYSIO_CFG[v_ucIOSymb].m_uiPinNo, INT_TRIGGER_LEVEL, 0, SYSIO_CFG[v_ucIOSymb].m_cBreakPow );
		sprintf(buf, "Set BrkTyp-%d,", SYSIO_CFG[v_ucIOSymb].m_cBreakType );
		strcat( szMsg, buf );
		break;
		
	case 2: // ���ش���
		YX_GPIO_INTConfig( SYSIO_CFG[v_ucIOSymb].m_uiPinNo, INT_TRIGGER_EDGE, SINGLE_EDGE_TRIGGER, SYSIO_CFG[v_ucIOSymb].m_cBreakPow );
		sprintf(buf, "Set BrkTyp-%d,", SYSIO_CFG[v_ucIOSymb].m_cBreakType );
		strcat( szMsg, buf );
		break;
		
	case 3: // ˫�ش���
		YX_GPIO_INTConfig( SYSIO_CFG[v_ucIOSymb].m_uiPinNo, INT_TRIGGER_EDGE, DOUBLE_EDGE_TRIGGER, 0 );
		sprintf(buf, "Set BrkTyp-%d,", SYSIO_CFG[v_ucIOSymb].m_cBreakType );
		strcat( szMsg, buf );
		break;
		
	default:
		;
	}

	YX_GPIO_INTSethandler( SYSIO_CFG[v_ucIOSymb].m_uiPinNo, (void (*)(INT32U, INT32U))v_pIntFunc ); // �����жϻص�����
	YX_GPIO_INTEnable( SYSIO_CFG[v_ucIOSymb].m_uiPinNo ); // ���ж�
	sprintf( buf, "Int Func: %08x, Enb Brk\n", int(v_pIntFunc) );
	strcat( szMsg, buf );
	PRTMSG( MSG_DBG, szMsg );

	return 0;
}

int CIO::_SemV()
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

int CIO::_SemP()
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

int CIO::_SemCreate()
{
	if( -1 == m_iSemID )
	{
		bool bFstCreate = false;
		
		m_iSemID = semget( (key_t)KEY_SEM_IO, 1, 0666 | IPC_CREAT | IPC_EXCL );
		if( -1 == m_iSemID )
		{
			m_iSemID = semget( (key_t)KEY_SEM_IO, 1, 0666 | IPC_CREAT );
			if( -1 == m_iSemID )
			{
				PRTMSG( MSG_ERR, "sem get fail\n" );
				return ERR_SEMGET;
			}
			else
			{
				PRTMSG( MSG_DBG, "sem get Succ\n" );
			}
		}
		else
		{
			PRTMSG( MSG_DBG, "sem get succ(first)\n" );
			bFstCreate = true;
		}
		
		if( bFstCreate )
		{
			semun sem_union;
			sem_union.val = 1;
			if( -1 == semctl( m_iSemID, 0, SETVAL, sem_union ) )
			{
				PRTMSG( MSG_ERR, "semctl SETVAL fail\n" );
				return ERR_SEMCTL;
			}
			else
			{
				PRTMSG( MSG_DBG, "semctl SETVAL succ\n" );
			}
		}
	}

	return 0;
}

int CIO::_ShmCreate()
{
	bool bFstCreate = false;

	if( -1 == m_iShmID )
	{
		m_iShmID = shmget( (key_t)KEY_SHM_IO, IO_MEMSHARE_SIZE, 0666 | IPC_CREAT | IPC_EXCL );
		if( -1 == m_iShmID )
		{
			m_iShmID = shmget( (key_t)KEY_SHM_IO, IO_MEMSHARE_SIZE, 0666 | IPC_CREAT );
			if( -1 == m_iShmID )
			{
				PRTMSG( MSG_ERR, "shm get fail\n" );
				return ERR_SHMGET;
			}
			else
			{
				PRTMSG( MSG_DBG, "shm get succ\n" );
			}
		}
		else
		{
			bFstCreate = true;
			PRTMSG( MSG_DBG, "shm get succ(first)\n" );
		}
	}

	if( !m_pMemShare )
	{
		void *pShare = shmat( m_iShmID, 0, 0 );
		if( (void*)-1 != pShare )
		{
			PRTMSG( MSG_DBG, "shmat succ\n" );
			m_pMemShare = (char*)pShare;
			if( bFstCreate )
			{
				memset( pShare, 0, IO_MEMSHARE_SIZE );
			}
		}
		else
		{
			PRTMSG( MSG_ERR, "shmat fail\n" );
			return ERR_SHMAT;
		}
	}

	return 0;
}
