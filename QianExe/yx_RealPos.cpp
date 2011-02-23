// RealPos.cpp: implementation of the CRealPos class.
//
//////////////////////////////////////////////////////////////////////

#include "yx_QianStdAfx.h"
#include "yx_RealPos.h"

#if USE_REALPOS == 1

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

void G_TmRealPosChk(void *arg, int len)
{
	g_objRealPos.P_TmRealPosChk();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRealPos::CRealPos()
{
	tag2QRealPosCfg obj2QRealPosCfg;
	obj2QRealPosCfg.Init(); // �ȳ�ʼ��,�ٶ�����
	::GetSecCfg( &obj2QRealPosCfg, sizeof(obj2QRealPosCfg),
		offsetof(tagSecondCfg, m_uni2QRealPosCfg.m_obj2QRealPosCfg), sizeof(obj2QRealPosCfg) );
	m_objPosCond = obj2QRealPosCfg;

	// ������������ļ��� ����Ҫ��
	memset( m_objPosCond.m_aryAreaContCt, 0, sizeof(m_objPosCond.m_aryAreaContCt) );
}

CRealPos::~CRealPos()
{

}

// bool CRealPos::_CutLine(byte *dot1, byte *dot2, double lon,double lat)
// {
//     unsigned int  x1, y1, x2, y2, x0, y0, temp;
//     bool positive;
// 
//     x1 = DegreeToNum(dot1);
//     dot1 += 4;
//     y1 = DegreeToNum(dot1);
//     x2 = DegreeToNum(dot2);
//     dot2 += 4;
//     y2 = DegreeToNum(dot2);
// 
//     x0= lon*60*100;
//    
//     y0 = lat*60*100;
// 
//     if (y0 == y1) {                 /* �������߾�������ζ��㡢���ߺͶ���α�ƽ�е���� */
//         y0++;
//     }
//     if (y0 == y2) {
//         y0++;
//     }
//     if ((y0 > y1 && y0 > y2) || (y0 < y1 && y0 < y2)) return FALSE;
//     if (x0 < x1 && x0 < x2) return FALSE;
//     if (x0 > x1 && x0 > x2) return TRUE;
// 
//     if (y1 > y2) {
//         temp = y1;
//         y1   = y2;
//         y2   = temp;
//         temp = x1;
//         x1   = x2;
//         x2   = temp;
//     }
// 
//     if (x2 >= x1) {                 /* �����(y2 - y1) / (x2 - x1) >= (y0 - y1) / (x0 - x1)����˵��2�߶��ཻ */
//         x2 -= x1;
//         x1  = x0 - x1;
//         positive = TRUE;            /* ����α߳���б��Ϊ�� */
//     } else {
//         x2  = x1 - x2;
//         x1  = x1 - x0;
//         positive = FALSE;           /* ����α߳���б��Ϊ�� */
//     }
//     y2 -= y1;
//     y1  = y0 - y1;
// 
//     if (y2 * x1 >= y1 * x2) {
//         if (positive) {
//             return TRUE;
//         } else {
//             return FALSE;
//         }
//     } else {
//         if (positive) {
//             return FALSE;
//         } else {
//             return TRUE;
//         }
//     }
// }

//�̻��ڳ�������ĳ���������--zzg add
// bool CRealPos::_CheckSanhuan(double lon,double lat)
// {
// 	static byte orgpolydot[][8] = 
// 	{
// 	  {104, 2,73, 6,30,43,69,24},//1
// 	  {104, 6,13,63,30,43,27, 1},//2
// 		{104, 8, 0,75,30,42,86,50},//3
// 		{104, 9,23,25,30,42,43, 0},//4
// 		{104, 9,61,61,30,40,33, 9},//5
// 		{104, 8,95,66,30,38,18,44},//6
// 		{104, 8, 0, 8,30,36,40,24},//7
// 		{104, 7, 9,21,30,35,96,26},//8
// 		{104, 5,61,14,30,35,81,80},//9
// 		{104, 3,86,16,30,36, 2, 5},//10
// 		{104, 2,38,73,30,36,11,89},//11
// 		{103,59,75,57,30,37,92,98},//12
// 		{103,58,87,39,30,39,27,79},//13
// 		{103,59,28,45,30,40,58,55},//14
// 		{104, 0,23,35,30,42,21,71} //15
// 	};
// 	INT16U  i, cut;
//     cut = 0;                                      
//     
//     for (i = 0; i < MAXPOLYDOT;i++) 
// 	{
//         if (i == MAXPOLYDOT - 1) {
//             if (_CutLine((byte*)&orgpolydot[i],(byte*)&orgpolydot[0], lon,lat)) {            	
//                 cut++;
//             }        
//         } else {
//             if (_CutLine((byte*)&orgpolydot[i],(byte*)&orgpolydot[i+1], lon,lat)) {            	
//                 cut++;
//             }
//         }
//     }
// 
//     if (cut % 2) {   	 	
//         return false;/* �������ཻ�ı���Ϊ������˵����ǰ���������� */
//     } 
// 	else 	return true;
// 	
// }

void CRealPos::P_TmRealPosChk()
{
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	int iResult = 0;

	if( 0 == (0x3e & m_objPosCond.m_bytCondStatus) )
	{
		_KillTimer(&g_objTimerMng, POS_TIMER );
		return;
	}

	DWORD dwCur = ::GetTickCount();
	
	static DWORD sta_dwSpdCt = 0;

	BYTE bytHour = 0, bytMin = 0;

	BYTE bytTimeNo = 0, bytAreaCode = 0, bytStaNo = 0;
	bool bTimeValid = true;
	bool bInterv = false, bTime = false, bArea = false, bSpeed = false, bDisconn = false;

	tagGPSData objGps(0);
	iResult = g_objMonAlert.GetCurGPS( objGps, false );
	if( iResult ) goto POSTIMER_END;

	if( 'A' == objGps.valid )
	{
		bytHour = BYTE( objGps.nHour ), bytMin = BYTE( objGps.nMinute );
	}
	else
	{
		struct tm pTm;
		G_GetTime(&pTm);
		
		if( pTm.tm_year + 1900 < VALIDYEAR_BEGIN ) bTimeValid = false;
		else if( VALIDYEAR_BEGIN == pTm.tm_year + 1900 && pTm.tm_mon < VALIDMONTH_BEGIN ) bTimeValid = false;
		bytHour = BYTE( pTm.tm_hour ), bytMin = BYTE( pTm.tm_min );
	}

//			�Ѿ���ComuExe����			
// 			// GPS��Ч��ʹ��GPSʱ�串��ϵͳʱ��
// 			if( 'A' == objGps.valid )
// 			{
// 				// ��ʱ��ֻ�õ�Сʱ�ͷ�
// 				tim.wHour = WORD(objGps.nHour);
// 				tim.wMinute = WORD( objGps.nMinute );
// 			}

	m_objPosCond.m_dLstSendDis +=
		::G_CalGeoDis2( m_objPosCond.m_dLstLon, m_objPosCond.m_dLstLat, objGps.longitude, objGps.latitude );
	m_objPosCond.m_dLstLon = objGps.longitude;
	m_objPosCond.m_dLstLat = objGps.latitude;
	
	if( 0x08 & m_objPosCond.m_bytCondStatus && 'A' == objGps.valid ) // �ٶ����� (��Ϊ���ڳ��ٱ���,���Է�����ǰ��)
	{
		if( objGps.speed >= m_objPosCond.m_fSpeed && m_objPosCond.m_fSpeed > 0 )
		{
			sta_dwSpdCt ++;
			if( POS_CONTCOUNT == sta_dwSpdCt )
			{
				sta_dwSpdCt = 0;
				bSpeed = true;
				goto POSTIMER_END;
			}
		}
		else
		{
			sta_dwSpdCt = 0;
		}
	}

	if( 0x04 & m_objPosCond.m_bytCondStatus && 'A' == objGps.valid ) // �з�Χ���� (��Ϊ����Խ�籨��,���Է���ǰ��)
	{
		size_t sizMax = sizeof(m_objPosCond.m_aryArea) / sizeof(m_objPosCond.m_aryArea[0]);
		long lLon = long( objGps.longitude * LONLAT_DO_TO_L );
		long lLat = long( objGps.latitude * LONLAT_DO_TO_L );
		bool bInArea;
		for( BYTE i = 0; i < m_objPosCond.m_bytAreaCount && i < sizMax; i ++ )
		{
			bInArea = ::JugPtInRect(lLon, lLat, m_objPosCond.m_aryArea[i][0], m_objPosCond.m_aryArea[i][1],
				m_objPosCond.m_aryArea[i][2], m_objPosCond.m_aryArea[i][3]);

			switch( m_objPosCond.m_aryAreaProCode[ i ] & 0x03 )
			{
// 					case 0: // ��������
// 						{
// 							if( !bInArea )
// 							{
// 								bArea = false;
// 								goto AREA_LOOPEND;
// 							}
// 							else
// 							{
// 								bytAreaCode = (m_objPosCond.m_aryAreaProCode[ i ] & 0x3c) >> 2;
// 								bArea = true;
// 								goto POSTIMER_END;
// 							}
// 						}
// 						break;

			case 1: // �����ڻ�
				{
					if( bInArea )
					{
						m_objPosCond.m_aryAreaContCt[ i ] ++;
						if( POS_CONTCOUNT == m_objPosCond.m_aryAreaContCt[ i ] )
						{
							//m_objPosCond.m_aryAreaContCt[ i ]  = 0;
							bytAreaCode = (m_objPosCond.m_aryAreaProCode[ i ] & 0x3c) >> 2;
							bArea = true;
							goto POSTIMER_END;
						}
					}
					else
						m_objPosCond.m_aryAreaContCt[ i ]  = 0;
				}
				break;

// 					case 2: // ��������
// 						{
// 							if( bInArea )
// 							{
// 								bArea = false;
// 								goto AREA_LOOPEND;
// 							}
// 							else
// 							{
// 								bytAreaCode = (m_objPosCond.m_aryAreaProCode[ i ] & 0x3c) >> 2;
// 								bArea = true;
// 								goto POSTIMER_END;
// 							}
// 						}
// 						break;

			case 3: // �������
				{
					if( !bInArea )
					{
						m_objPosCond.m_aryAreaContCt[ i ] ++;
						if( POS_CONTCOUNT == m_objPosCond.m_aryAreaContCt[ i ] )
						{
							//m_objPosCond.m_aryAreaContCt[ i ] = 0;
							bytAreaCode = (m_objPosCond.m_aryAreaProCode[ i ] & 0x3c) >> 2;
							bArea = true;
							goto POSTIMER_END;
						}
					}
					else
						m_objPosCond.m_aryAreaContCt[ i ] = 0;
				}
				break;

			default:
				;
			}
		}
//AREA_LOOPEND:
		;
	}

	if( 0x02 & m_objPosCond.m_bytCondStatus ) // ״̬����
	{
	}

	if( 0x20 & m_objPosCond.m_bytCondStatus ) // �м������
	{
		if( m_objPosCond.m_wTimeInterv > 0
			&& dwCur - m_objPosCond.m_dwLstSendTime >= 1000 * 60 * DWORD(m_objPosCond.m_wTimeInterv) )
		{
			bInterv = true;
			goto POSTIMER_END;
		}
		else if( m_objPosCond.m_wSpaceInterv > 0 
			&& m_objPosCond.m_dLstSendDis > m_objPosCond.m_wSpaceInterv * 1000.0 )
		{
			bInterv = true;
			goto POSTIMER_END;
		}
	}

	if( bTimeValid && 0x10 & m_objPosCond.m_bytCondStatus &&
		(bytMin != m_objPosCond.m_bytLstTimeMin || bytHour != m_objPosCond.m_bytLstTimeHour) ) // ��ʱ������,�ҵ�ǰʱ��δ�жϹ�
	{
		size_t sizMax = sizeof(m_objPosCond.m_aryTime) / sizeof(m_objPosCond.m_aryTime[0]);
		for( BYTE i = 0; i < m_objPosCond.m_bytTimeCount && i < sizMax; i ++ )
		{
			if( bytMin == m_objPosCond.m_aryTime[i][2] && bytHour == m_objPosCond.m_aryTime[i][1] )
			{
				m_objPosCond.m_bytLstTimeHour = bytHour;
				m_objPosCond.m_bytLstTimeMin = bytMin;
				bytTimeNo = m_objPosCond.m_aryTime[ i ][ 0 ];
				bTime = true;
				goto POSTIMER_END;
			}
		}

		// ����ѷ���ʱ��,ʹ��24Сʱ������ٴδ���
		m_objPosCond.m_bytLstTimeMin = m_objPosCond.m_bytLstTimeHour = 0xff;
	}

POSTIMER_END:
	if( bInterv || bArea || bSpeed || bTime || bDisconn )
	{
		BYTE bytConNo = 0;
		BYTE bytType = 0;
		if( bDisconn )
		{
			bytType = 0x44;
		}
		else if( bArea )
		{
			bytConNo = bytAreaCode;
			bytType = 0x43;
	
			g_objMonAlert.SetAlertArea(true);
		}
		else if( bSpeed )
		{
			bytConNo = 0;
			bytType = 0x42;

			char str[1024] = {0x01, 0x00};
			char szTip[] = {"˾�����ѣ����ѳ��٣����������"};
			memcpy(str+2, szTip, strlen(szTip));
			DataPush(str, strlen(szTip)+2, DEV_QIAN, DEV_DVR, LV2);
	
 			g_objMonAlert.SetAlertOverSpd(true);
		}
		else if( bTime )
		{
			bytConNo = bytTimeNo;
			bytType = 0x41;
		}
		else if( bInterv )
		{
			bytConNo = 0;
			bytType = 0x40;
		}
		if( 0 == bytConNo ) bytConNo = 0x7f;

		tag0241 data;
		data.m_bytConNo = bytConNo;
		data.m_bytConType = bytType;

		// �������б���״̬,���ɱ�������
		g_objMonAlert.FillAlertSymb( g_objMonAlert.GetAlertSta(), &data.m_bytAlertHi, &data.m_bytAlertLo );
		
		tag2QGprsCfg objGprsCfg;
		::GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),
			offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );
		tagQianGps objQianGps;
		g_objMonAlert.GpsToQianGps( objGps, objQianGps );
		memcpy( &(data.m_objQianGps), &objQianGps, sizeof(objQianGps) );
		iResult = g_objSms.MakeSmsFrame((char*)&data, sizeof(data), 0x02, 0x41, buf, sizeof(buf), iBufLen);
		if( !iResult )
		{
			iResult = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12,
				(objGprsCfg.m_bytChannelBkType_1 & 0x20) ? DATASYMB_SMSBACKUP : 0 );
			if( !iResult ) // ֻҪ��������,�Ժ�����¿�ʼ�����������ʱ����
			{
				m_objPosCond.m_dLstSendDis = 0;
				m_objPosCond.m_dwLstSendTime = ::GetTickCount();
			}
		}
	}

	static BYTE sta_bytCt = 0;
	if( 0 == sta_bytCt ++ % 10 )
		_SetTimer(&g_objTimerMng, POS_TIMER, POS_INTERVAL, G_TmRealPosChk );
}

void CRealPos::Init()
{
	m_objPosCond.m_bytLstTimeHour = m_objPosCond.m_bytLstTimeMin = 0xff;
	m_objPosCond.m_dLstLon = m_objPosCond.m_dLstLat = 0;
	m_objPosCond.m_dwLstSendTime = ::GetTickCount() - 0xfffffff; // ȷ����ʼ�������������ʵʱ��λ����
	
	// ������������ļ��� ����Ҫ��
	memset( m_objPosCond.m_aryAreaContCt, 0, sizeof(m_objPosCond.m_aryAreaContCt) );
	
	m_iSanHuanCt=0;//ʻ�������Ĵ���--zzg add
	
//	if( 0x3e & m_objPosCond.m_bytCondStatus )--����ʱ�亯�������ж�,zzg mod
		_SetTimer(&g_objTimerMng, POS_TIMER, POS_INTERVAL, G_TmRealPosChk );
}

void CRealPos::Release()
{
}

tag2QRealPosCfg CRealPos::GetRealPosCond()
{
	return m_objPosCond;
}

void CRealPos::SetRealPosCond( const tag2QRealPosCfg& v_objRealPosCond )
{
	m_objPosCond = v_objRealPosCond;
}

void CRealPos::BeginRealPosChk()
{
	_SetTimer(&g_objTimerMng, POS_TIMER, POS_INTERVAL, G_TmRealPosChk );
}

void CRealPos::EndRealPosChk()
{
	_KillTimer(&g_objTimerMng, POS_TIMER );
}

/// ����ʵʱ��λ ��Ӧ��ɹ����ȣ�
// ʵʱ��λĿ���ֻ��ţ�15�� + ���ÿ����֣�2��+ ����������1��+ [ ��λ�������ͣ�1��+ ��λ���� ] x N
int CRealPos::Deal1007( char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	int iRet = 0;
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	
	if( v_dwDataLen < 18 )
	{
		iRet = ERR_DATA_INVALID;
		goto DEAL1007_FAILED;
	}

	{
		tag2QRealPosCfg objPosCond = g_objRealPos.GetRealPosCond(); // �̳е�ǰʵʱ��λ������״̬��Ϣ
		tag2QRealPosCfg objPosCondBkp = objPosCond; // ����
		tag1007 obj1007;

		// ׼����������֡
		memcpy( &obj1007, v_szData, v_dwDataLen );
		if( 0x7f == obj1007.m_bytCondCount ) obj1007.m_bytCondCount = 0;
		
		BYTE bytCond1 = obj1007.m_aryControl[ 0 ]; // �����ֽ�1
		BYTE bytCondCount = obj1007.m_bytCondCount; // ��������

		// ����ж�
		if( 0x20 & bytCond1 ) // ���
		{
			objPosCond.m_wTimeInterv = 0;
			objPosCond.m_wSpaceInterv = 0;
		}
		if( 0x10 & bytCond1 ) // ʱ��
		{
			objPosCond.m_bytTimeCount = 0;
			memset( objPosCond.m_aryTime, 0, sizeof(objPosCond.m_aryTime) );
		}
		if( 0x08 & bytCond1 ) // �ٶ�
		{
			objPosCond.m_fSpeed = 0;
		}
		if( 0x04 & bytCond1 ) // ��Χ
		{
			objPosCond.m_bytAreaCount = 0;
			memset( objPosCond.m_aryArea, 0, sizeof(objPosCond.m_aryArea) );
			memset( objPosCond.m_aryAreaProCode, 0, sizeof(objPosCond.m_aryAreaProCode) );
			memset( objPosCond.m_aryAreaContCt, 0, sizeof(objPosCond.m_aryAreaContCt) );
		}
		if( 0x02 & bytCond1 ) // ״̬
		{
			memset( objPosCond.m_aryStatus, 0, sizeof(objPosCond.m_aryStatus) );
		}

		// ��������
		int i = 0;
		BYTE ct = 0;
		BYTE bytCondType;
		objPosCond.m_bytCondStatus = obj1007.m_aryControl[ 1 ];
		for( i = 0, ct = 0; ct < obj1007.m_bytCondCount; ct ++  )
		{
			if( i >= int(v_dwDataLen - 18) )
			{
				iRet = ERR_DATA_INVALID;
				goto DEAL1007_FAILED;
			}
			bytCondType = BYTE( obj1007.m_szCond[ i ++ ] );

			switch( bytCondType & 0x07 )
			{
			case 0: // ���
				{
					if( i + sizeof(tag1007Interv) > int(v_dwDataLen - 18) )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					tag1007Interv objInterv;
					memcpy( &objInterv, obj1007.m_szCond + i, sizeof(tag1007Interv) );
					i += sizeof(tag1007Interv);

					if( 0x7f == objInterv.m_bytHourInterv ) objInterv.m_bytHourInterv = 0;
					if( 0x7f == objInterv.m_bytMinInterv ) objInterv.m_bytMinInterv = 0;
					if( 0x7f == objInterv.m_bytHunKilo ) objInterv.m_bytHunKilo = 0;
					if( 0x7f == objInterv.m_bytKilo ) objInterv.m_bytKilo = 0;

					if( objInterv.m_bytHourInterv > 126 || objInterv.m_bytMinInterv > 59 )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					objPosCond.m_dwLstSendTime = ::GetTickCount();
					objPosCond.m_wTimeInterv = WORD(objInterv.m_bytHourInterv) * 60 + objInterv.m_bytMinInterv;

					if( objInterv.m_bytHunKilo > 100 || objInterv.m_bytKilo > 99 )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					objPosCond.m_wSpaceInterv = WORD(objInterv.m_bytHunKilo) * 100 + objInterv.m_bytKilo;
				}
				break;

			case 1: // ʱ��
				{
					if( i + sizeof(tag1007Time) > int(v_dwDataLen - 18) )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					tag1007Time objTime;
					memcpy( &objTime, obj1007.m_szCond + i, sizeof(tag1007Time) );
					i += sizeof(tag1007Time);

					if( 0x7f == objTime.m_bytNo ) objTime.m_bytNo = 0;
					if( 0x7f == objTime.m_bytHour ) objTime.m_bytHour = 0;
					if( 0x7f == objTime.m_bytMin ) objTime.m_bytMin = 0;

					if( objTime.m_bytHour > 23 || objTime.m_bytMin > 59 )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					if( objPosCond.m_bytTimeCount >= sizeof(objPosCond.m_aryTime) / sizeof(objPosCond.m_aryTime[0]) )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					BYTE byt = objPosCond.m_bytTimeCount;
					objPosCond.m_aryTime[ byt ][ 0 ] = objTime.m_bytNo;
					objPosCond.m_aryTime[ byt ][ 1 ] = objTime.m_bytHour;
					objPosCond.m_aryTime[ byt ][ 2 ] = objTime.m_bytMin;
					objPosCond.m_bytTimeCount ++;
				}
				break;

			case 2: // �ٶ�
				{
					if( i + 1 > int(v_dwDataLen - 18) )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					BYTE bytSpeed = BYTE(obj1007.m_szCond[ i ]);
					i += 1;

					if( 0x7f == bytSpeed ) bytSpeed = 0;

					if( bytSpeed > 126 )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					objPosCond.m_fSpeed = float( bytSpeed * 1.852 / 3.6 );
				}
				break;

			case 3: // ��Χ
				{
					if( i + sizeof(tag1007Area) > int(v_dwDataLen - 18) )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					tag1007Area objArea;
					memcpy( &objArea, obj1007.m_szCond + i, sizeof(tag1007Area) );
					i += sizeof(tag1007Area);

					if( objPosCond.m_bytAreaCount >= sizeof(objPosCond.m_aryArea) / sizeof(objPosCond.m_aryArea[0]) )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					BYTE byt = objPosCond.m_bytAreaCount;

					objPosCond.m_aryAreaProCode[ byt ] = objArea.m_bytAreaProCode;
					{
						double dTemp;
						BYTE bytTemp;
						
						// ���¾���
						bytTemp = (0x20 & objArea.m_bytLonMix ? 0x80 : 0) + objArea.m_bytLonDu;
						if( 0xff == bytTemp ) bytTemp = 0;
						else if( 0xfe == bytTemp ) bytTemp = 0x80;
						if( 0x7f == objArea.m_bytLonFenzen ) objArea.m_bytLonFenzen = 0;
						if( 0x7f == objArea.m_bytLonFenxiao1 ) objArea.m_bytLonFenxiao1 = 0;
						if( 0x7f == objArea.m_bytLonFenxiao2 ) objArea.m_bytLonFenxiao2 = 0;
						dTemp = objArea.m_bytLonFenzen + objArea.m_bytLonFenxiao1 * 0.01 + objArea.m_bytLonFenxiao2 * 0.0001;
						objPosCond.m_aryArea[ byt ][ 0 ] = long( (bytTemp + dTemp / 60) * LONLAT_DO_TO_L );

						// ����γ��
						bytTemp = objArea.m_bytLatDu;
						if( 0x7f == bytTemp ) bytTemp = 0;
						if( 0x7f == objArea.m_bytLatFenzen ) objArea.m_bytLatFenzen = 0;
						if( 0x7f == objArea.m_bytLatFenxiao1 ) objArea.m_bytLatFenxiao1 = 0;
						if( 0x7f == objArea.m_bytLatFenxiao2 ) objArea.m_bytLatFenxiao2 = 0;
						dTemp = objArea.m_bytLatFenzen + objArea.m_bytLatFenxiao1 * 0.01 + objArea.m_bytLatFenxiao2 * 0.0001;
						objPosCond.m_aryArea[ byt ][ 1 ] = long( (bytTemp + dTemp / 60) * LONLAT_DO_TO_L );

						// ���Ͼ���
						if( 0x7f == objArea.m_bytLonWidFenZhen ) objArea.m_bytLonWidFenZhen = 0;
						if( 0x7f == objArea.m_bytLonWidFenxiao1 ) objArea.m_bytLonWidFenxiao1 = 0;
						if( 0x7f == objArea.m_bytLonWidFenxiao2 ) objArea.m_bytLonWidFenxiao2 = 0;
						bytTemp = objArea.m_bytLonMix & 0x1f;
						dTemp = objArea.m_bytLonWidFenZhen + objArea.m_bytLonWidFenxiao1 * 0.01 + objArea.m_bytLonWidFenxiao2 * 0.0001;
						objPosCond.m_aryArea[ byt ][ 2 ] = objPosCond.m_aryArea[ byt ][ 0 ] + long( (bytTemp + dTemp / 60) * LONLAT_DO_TO_L );

						// ����γ��
						bytTemp = objArea.m_bytLatMix & 0x1f;
						if( 0x7f == objArea.m_bytLatHeiFenZhen ) objArea.m_bytLatHeiFenZhen = 0;
						if( 0x7f == objArea.m_bytLatHeiFenxiao1 ) objArea.m_bytLatHeiFenxiao1 = 0;
						if( 0x7f == objArea.m_bytLatHeiFenxiao2 ) objArea.m_bytLatHeiFenxiao2 = 0;
						dTemp = objArea.m_bytLatHeiFenZhen + objArea.m_bytLatHeiFenxiao1 * 0.01 + objArea.m_bytLatHeiFenxiao2 * 0.0001;
						objPosCond.m_aryArea[ byt ][ 3 ] = objPosCond.m_aryArea[ byt ][ 1 ] + long( (bytTemp + dTemp / 60) * LONLAT_DO_TO_L );
					}
					objPosCond.m_bytAreaCount ++;
				}
				break;

			case 4: // ״̬
				{
					if( i + 6 > int(v_dwDataLen - 18) )
					{
						iRet = ERR_DATA_INVALID;
						goto DEAL1007_FAILED;
					}
					memcpy( objPosCond.m_aryStatus, obj1007.m_szCond + i, 6 );
					i += 6;
				}
				break;
				
			default:
				;
			}
		}

		BYTE bytResType = 0x01;
		iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x47, buf, sizeof(buf), iBufLen); // ��֡
		if( !iRet )
		{
			// ����������
			if( iRet = ::SetSecCfg(&objPosCond, offsetof(tagSecondCfg, m_uni2QRealPosCfg.m_obj2QRealPosCfg),
				sizeof(objPosCond)) )
			{
				goto DEAL1007_FAILED;
			}

			// ����
			iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
			if( !iRet )
			{
				g_objRealPos.SetRealPosCond( objPosCond ); // �ȸ��±������еı���
				
				if( !(0x3e & objPosCond.m_bytCondStatus) ) // ��ȫ�������ر�
					g_objRealPos.EndRealPosChk();
				else
					g_objRealPos.BeginRealPosChk();
			}
			else // ����(������ʧ��)
			{
				::SetSecCfg( &objPosCondBkp, offsetof(tagSecondCfg, m_uni2QRealPosCfg.m_obj2QRealPosCfg),
					sizeof(objPosCondBkp) ); // �ָ�ԭ����
				goto DEAL1007_FAILED;
			}
		}
		else goto DEAL1007_FAILED;
	}

	return 0;

DEAL1007_FAILED:
	BYTE bytResType = 0x7f;
	int iRet2 = 0;
	iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x47, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );

	return iRet;
}

#endif
