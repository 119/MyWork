#include "yx_QianStdAfx.h"

#if USE_BLK == 1

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-Blk      ")

int SortStr( const void* v_pElement1, const void* v_pElement2 )
{
	return strcmp( (char*)v_pElement1, (char*)v_pElement2 );
}

int SortBlackPeriod( const void* v_pElement1, const void* v_pElement2 )
{
	tagBlackQueryPeriod *pPeriod1 = (tagBlackQueryPeriod*)v_pElement1;
	tagBlackQueryPeriod *pPeriod2 = (tagBlackQueryPeriod*)v_pElement2;
	
	int iRet = int(pPeriod1->m_bytFromYear) - pPeriod2->m_bytFromYear;
	if( iRet ) return iRet;
	
	iRet = int(pPeriod1->m_bytFromMon) - pPeriod2->m_bytFromMon;
	if( iRet ) return iRet;
	
	iRet = int(pPeriod1->m_bytFromDay) - pPeriod2->m_bytFromDay;
	if( iRet ) return iRet;
	
	iRet = int(pPeriod1->m_bytFromHour) - pPeriod2->m_bytFromHour;
	if( iRet ) return iRet;
	
	return int(pPeriod1->m_bytFromMin) - pPeriod2->m_bytFromMin;
}

void G_TmBlkSave(void *arg, int len)
{
	g_objBlk.P_TmBlkSave();
}

void G_TmBlkSndRes(void *arg, int len)
{
	g_objBlk.P_TmBlkSndRes();
}

void G_TmBlkRcvQuery(void *arg, int len)
{
	g_objBlk.P_TmBlkRcvQuery();
}


CBlk::CBlk()
{
	m_i0e60Len = 0;
	
	memset(m_szBlackPath, 0, sizeof(m_szBlackPath));
	memset(m_szBlackFile, 0, sizeof(m_szBlackFile));
	
	memcpy(m_szBlackPath, BLK_SAVE_PATH, strlen(BLK_SAVE_PATH));

	m_pfBlack = NULL;
	m_iBlkFile = 0;
	
	m_dwLstGpsSaveTm = GetTickCount();
	m_wBlkGpsCt = 0;
}

CBlk::~CBlk()
{
	if( m_pfBlack )
	{
		if( m_wBlkGpsCt > 0 )
		{
			fwrite( m_aryBlackGps, sizeof(m_aryBlackGps[0]) * m_wBlkGpsCt, 1, m_pfBlack );
			m_wBlkGpsCt = 0;
		}
		fclose( m_pfBlack );
		m_pfBlack = NULL;
	}
}

void CBlk::P_TmBlkSave()
{
//	PRTMSG(MSG_DBG, "Blk Save Timer!(100224)\n");

	tagBlackSaveGps objBlackSaveGps;
	int iResult = _GetCurBlackSaveGps( objBlackSaveGps );

	if( !iResult )
	{
		_SaveBlackGps( objBlackSaveGps ); // ��ʱ,�ѱ�֤��ϻ��GPS��������Ч��,���ٶ�>0
	}

	if( ERR_BLACKGPS_INVALID == iResult )
	{
		//PRTMSG(MSG_DBG, "Gps invalid, wait for 5s!\n");
		_SetTimer(&g_objTimerMng, BLACK_TIMER, 5000, G_TmBlkSave ); // ����ϻ��������Ч,����ʱ5�����
	}
	else
	{
		_SetTimer(&g_objTimerMng, BLACK_TIMER, BLACK_MININTERVAL * 1000, G_TmBlkSave ); // (�̶���ϻ�ӱ�����)
	}
}

void CBlk::P_TmBlkSndRes()
{
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	int iResult = 0;

	tag2QGprsCfg objGprsCfg;
	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	DWORD dwBegin = GetTickCount();
	char szCurFile[ MAX_PATH ];
	FILE* fp = NULL;
	size_t sizResult;
	char* szBlkFileName = NULL;
	tagBlackSaveGps aryBlackSaveGps[ BLKREADUNIT ];
	tag0e60BlackGps aryBlackGps[ BLKSNDUNIT ];
	tagBlackQueryPeriod* pPeriod;
	DWORD dwCt = 0; // ��ϻ�ӵ���ѭ����
	WORD wNameSize = sizeof(szCurFile) / sizeof(szCurFile[0]);
	BYTE bytYear = 0, bytMon = 0, bytDayNo = 0; // ��ϻ���ļ��������õ�����������Ϣ
	BYTE bytPeriodBeginNo = 0, byt = 0;
	BYTE bytAreaCt = 0; // ��֡�����㷶Χ�����ĺ�ϻ�����ݼ���
	BYTE bytSndCt = 0; // ��֡Ҫ���͵ĺ�ϻ�����ݼ���
	BYTE bytAdvanceEndSymb = 0; // 0,������ǰ����; 1,��ǰ�������ζ�ʱ������; 2,��ǰ����ȫ����ѯ����
	bool bFitArea = false, bFitPeriod = false;

	BYTE bytDataType = 0;
	tag0e60 res;

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL || 30 == USE_PROTOCOL
	const BYTE PKT0E60_BASLEN = 16;
	bytDataType = 0x60;
	memcpy( res.m_szCentGroup, m_objBlackQuerySta.m_szCentGroup, sizeof(res.m_szCentGroup) );
#endif
#if 2 == USE_PROTOCOL
	const BYTE PKT0E60_BASLEN = 6;
	bytDataType = 0x50;
#endif

	// ������ϻ�������ļ����д���
	while( m_objBlackSndSta.m_wDoneFileCount < m_objBlackSndSta.m_wFileTotal )
	{
		// ����ǰ����������ļ�,��λ������Ҫ������ļ�
		szBlkFileName = (char*)(m_objBlackSndSta.m_szBlackFile[m_objBlackSndSta.m_wDoneFileCount]);

		// Ѱ�Ҹú�ϻ���ļ�������ĵ�һ��ʱ�������
		bytPeriodBeginNo = INVALID_NUM_8;
		if( _GetPeriodFromBlkFileName(szBlkFileName, bytYear, bytMon, bytDayNo) )
		{
			for( byt = 0; byt < m_objBlackQuerySta.m_bytPeriodCount; byt ++ )
			{
				pPeriod = m_objBlackQuerySta.m_aryPeriod + byt;
				if( pPeriod->m_bytFromYear > bytYear || pPeriod->m_bytToYear < bytYear )
				{
					continue;
				}
				if( (pPeriod->m_bytFromYear == bytYear && pPeriod->m_bytFromMon > bytMon)
					||
					(pPeriod->m_bytToYear == bytYear && pPeriod->m_bytToMon < bytMon)
				  )
				{
					continue;
				}
				if(
					(pPeriod->m_bytFromYear == bytYear && pPeriod->m_bytFromMon == bytMon && pPeriod->m_bytFromDay / DSK2_ONEFILE_DAYS + 1 > bytDayNo)
					||
					(pPeriod->m_bytToYear == bytYear && pPeriod->m_bytToMon == bytMon && pPeriod->m_bytToDay / DSK2_ONEFILE_DAYS + 1 < bytDayNo)
				  )
				{
					continue;
				}

				bytPeriodBeginNo = byt;
				break;
			}
		}
		else
		{
			// �޷����ļ���������Ϣ��,������
			PRTMSG(MSG_ERR, "can not deal the file:%s\n", szBlkFileName);
			iResult = ERR_FILE;
			continue;
		}

		// ����ʱ�������,��δ�ҵ����ļ��������ʱ�������,����
		if( m_objBlackQuerySta.m_bytPeriodCount > 0 && INVALID_NUM_8 == bytPeriodBeginNo )
		{
			goto BLKONEFILELOOP_END;
		}

		// �ļ���
		memset(szCurFile, 0, sizeof(szCurFile));
		memcpy(szCurFile, m_szBlackPath, strlen(m_szBlackPath));
		memcpy(szCurFile+strlen(m_szBlackPath), szBlkFileName, strlen(szBlkFileName));

		// ���ļ�
		fp = fopen( szCurFile, "rb" );
		if( !fp )
		{
			PRTMSG(MSG_ERR, "Open Blk File To Read Failure: %s\n" , szCurFile);
			iResult = ERR_FILE_FAILED;
			goto BLKONEFILELOOP_END;
		}

		// �����Ѵ���ĵ�
		if( fseek(fp, sizeof(tagBlackSaveGps) * m_objBlackSndSta.m_dwLstFileDoneCt, SEEK_SET) )
		{
			iResult = ERR_FILE_FAILED;
			goto BLKONEFILELOOP_END;
		}

		// ������ȡ��ϻ������,��ȡ���������ĵ�,��֡����,�����·���״̬
		bytSndCt = 0; // ��ʼ�������͵ĺ�ϻ�ӵ���� (��һ֡�˲ŷ���)
		do 
		{
			// ��ȡһ����ϻ�����ݿ�
			sizResult = fread( aryBlackSaveGps, sizeof(aryBlackSaveGps[0]), BLKREADUNIT, fp );
			if( 0 == sizResult )
			{
				iResult = ERR_FILE_FAILED;
				goto BLKONEFILELOOP_END;
			}

			// ���ϻ�ӵ��ж�
			bytAreaCt = 0; // ��ʼ���������������ĺ�ϻ�ӵ����
			for( dwCt = 0; dwCt < sizResult; dwCt ++ )
			{
				// �������������ж�
				bFitArea = false; // ��ʼ��Ϊ�������������
				for( byt = 0; byt < m_objBlackQuerySta.m_bytAreaCount; byt ++ )
				{
					if( JugPtInRect( aryBlackSaveGps[ dwCt ].m_lLon, aryBlackSaveGps[ dwCt ].m_lLat,
						m_objBlackQuerySta.m_aryArea[ byt ].m_lLonMin, m_objBlackQuerySta.m_aryArea[ byt ].m_lLatMin,
						m_objBlackQuerySta.m_aryArea[ byt ].m_lLonMax, m_objBlackQuerySta.m_aryArea[ byt ].m_lLatMax ) )
					{
						bFitArea = true;
						break;
					}
				}

				if( m_objBlackQuerySta.m_bytAreaCount > 0 && !bFitArea ) continue; // ���е���������������,����

				// ����ʱ��������ж� (��ǰ���ѵõ��ĵ�һ�������ʱ���������ʼ)
				bFitPeriod = false; // ��ʼ��Ϊ������ʱ�������
				for( byt = bytPeriodBeginNo; byt < m_objBlackQuerySta.m_bytPeriodCount; byt ++ )
				{
					pPeriod = m_objBlackQuerySta.m_aryPeriod + byt;

					// ���õ㲻����ķ�Χ��,����
					if( aryBlackSaveGps[ dwCt ].m_bytYear < pPeriod->m_bytFromYear
						|| aryBlackSaveGps[ dwCt ].m_bytYear > pPeriod->m_bytToYear )
						continue;

					if( aryBlackSaveGps[ dwCt ].m_bytYear == pPeriod->m_bytFromYear ) // ������ʼ��
					{
						if( aryBlackSaveGps[ dwCt ].m_bytMon < pPeriod->m_bytFromMon ) // ������ʼ��֮ǰ ����
							continue;
						else if( aryBlackSaveGps[ dwCt ].m_bytMon == pPeriod->m_bytFromMon ) // ������ʼ��
						{
							if( aryBlackSaveGps[ dwCt ].m_bytDay < pPeriod->m_bytFromDay ) // ������ʼ��֮ǰ,����
								continue;
							else if( aryBlackSaveGps[ dwCt ].m_bytDay == pPeriod->m_bytFromDay ) // ������ʼ��
							{
								if( aryBlackSaveGps[ dwCt ].m_bytHour < pPeriod->m_bytFromHour ) // ������ʼʱ֮ǰ,����
									continue;
								else if( aryBlackSaveGps[ dwCt ].m_bytHour == pPeriod->m_bytFromHour ) // ������ʼʱ
								{
									if( aryBlackSaveGps[ dwCt ].m_bytMin < pPeriod->m_bytFromMin ) // ������ʼ��֮ǰ,����
										continue;
								}
							}
						}
					}

					if( aryBlackSaveGps[ dwCt ].m_bytYear == pPeriod->m_bytToYear ) // ������ֹ��
					{
						if( aryBlackSaveGps[ dwCt ].m_bytMon > pPeriod->m_bytToMon ) // ������ֹ��֮�� ����
							continue;
						else if( aryBlackSaveGps[ dwCt ].m_bytMon == pPeriod->m_bytToMon ) // ������ֹ��
						{
							if( aryBlackSaveGps[ dwCt ].m_bytDay > pPeriod->m_bytToDay ) // ������ֹ��֮��,����
								continue;
							else if( aryBlackSaveGps[ dwCt ].m_bytDay == pPeriod->m_bytToDay ) // ������ֹ��
							{
								if( aryBlackSaveGps[ dwCt ].m_bytHour > pPeriod->m_bytToHour ) // ������ֹʱ֮��,����
									continue;
								else if( aryBlackSaveGps[ dwCt ].m_bytHour == pPeriod->m_bytToHour ) // ������ֹʱ
								{
									if( aryBlackSaveGps[ dwCt ].m_bytMin > pPeriod->m_bytToMin ) // ������ֹ��֮��,����
										continue;
								}
							}
						}
					}

					bFitPeriod = true;
					break;
				}

				if( m_objBlackQuerySta.m_bytPeriodCount > 0 && !bFitPeriod )
				{
					continue; // ����ʱ���������������,����
				}

				_BlackSaveGpsToBlackGps( aryBlackSaveGps[ dwCt ], aryBlackGps[ bytSndCt ++ ] );
			
				if( bFitArea ) bytAreaCt ++; // �����������������ۼӸ������

				if( bytSndCt >= BLKSNDUNIT ) // �����ϴ��ĺ�ϻ�ӵ���һ֡
				{
					if( m_i0e60Len > 0 ) // ����ǰһ������û���ϴ�
					{
						// �ȷ���ǰһ������
						iResult = g_objSms.MakeSmsFrame( (char*)&m_obj0e60, m_i0e60Len, 0x0e, bytDataType, buf, sizeof(buf), iBufLen );
						if( !iResult )
						{
							iResult = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9,
								DATASYMB_OE60 | ((objGprsCfg.m_bytChannelBkType_2 & 0x40) ? DATASYMB_SMSBACKUP : 0) );
							if( !iResult )
							{
								m_objBlackSndSta.m_dwGpsTotal += m_obj0e60.m_bytGpsCount;
								m_objBlackSndSta.m_bytSndFrameCount ++;
							}
						}

						// ǰһ�����ݻ������
						memset((void*)&m_obj0e60, 0, sizeof(m_obj0e60) );
						m_i0e60Len = 0;
					}

					// �����ϴ�֡������ (��Ϊ��������)
					m_objBlackQuerySta.m_bytUploadFrameCt ++;

					// �ж��ϴ���֡���Ƿ��Ѿ��ﵽ���ֵ
					if( m_objBlackQuerySta.m_bytUploadFrameCt >= m_objBlackQuerySta.m_bytUploadFrameTotal )
					{
						bytAdvanceEndSymb = 2; // Ҫ��ǰ����ȫ����ϻ�Ӳ�ѯ����
						break; // ���������ϻ�����ݴ���
					}

					// ���ɱ�������
					res.m_bytResFrameNo = m_objBlackSndSta.m_bytSndFrameCount + 1;
					res.m_bytGpsCount = BLKSNDUNIT;
					res.m_bytResType = 0x01; // ��Ԥ����Ϊ�ǽ���֡�ĳɹ�Ӧ��
					memcpy( res.m_aryBlackGps, aryBlackGps, sizeof(res.m_aryBlackGps) ); 
					res.m_bytFitAreaCount = bytAreaCt;
					res.m_bytFitTotalHi = 0;
					res.m_bytFitTotalLo = 0;

					// �ݴ汾������(�´η���)
					m_obj0e60 = res;
					m_i0e60Len = PKT0E60_BASLEN + sizeof(tag0e60BlackGps) * BLKSNDUNIT;

					// ���ü���,�Ա��´�ʹ��
					bytSndCt = 0, bytAreaCt = 0;
				}
			}

			// ���±���ϻ���ļ���,����ɵĺ�ϻ�ӵ����
			m_objBlackSndSta.m_dwLstFileDoneCt += dwCt;

			// ���δ������ѻ��Ѵ���ʱ��,����ǰ����
			if( GetTickCount() - dwBegin >= TIMER_MAXPERIOD ) // �����η�����ʱ�Ѿ������������ֵ
			{
				if(bytAdvanceEndSymb != 2)
				{
					PRTMSG(MSG_DBG, "query over time, quit\n");
					bytAdvanceEndSymb = 1; // Ҫ��ǰ�������ζ�ʱ����ϻ�Ӵ���
					break; // ���������ļ�����
				}
			}
		} while( BLKREADUNIT == sizResult && !bytAdvanceEndSymb );

BLKONEFILELOOP_END:
		if( fp )
		{
			fclose( fp );
			fp = NULL;
		}

		if( bytAdvanceEndSymb ) // ������ǰ���� (���������쳣���µ���ǰ����)
			break;
		else
		{
			m_objBlackSndSta.m_wDoneFileCount ++; // �����Ѵ�����ļ���¼
			m_objBlackSndSta.m_dwLstFileDoneCt = 0; // �����ļ�,���ø��ļ��Ѵ���ĵ����
		}
	}

	if( fp ) 
		fclose( fp );

	if( bytSndCt > 0 ) // ���б�������
	{
		if( m_i0e60Len > 0 ) // ����ǰһ�����ݻ�δ�ϴ�
		{
			// ����ǰһ������
			iResult = g_objSms.MakeSmsFrame( (char*)&m_obj0e60, m_i0e60Len, 0x0e, bytDataType, buf, sizeof(buf), iBufLen );
			if( !iResult )
			{
				iResult = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9,
					DATASYMB_OE60 | ((objGprsCfg.m_bytChannelBkType_2 & 0x40) ? DATASYMB_SMSBACKUP : 0) );
				if( !iResult )
				{
					m_objBlackSndSta.m_dwGpsTotal += m_obj0e60.m_bytGpsCount;
					m_objBlackSndSta.m_bytSndFrameCount ++;
				}
			}

			// ǰһ�����ݻ������ (��Ҫ)
			memset((void*)&m_obj0e60, 0, sizeof(m_obj0e60) );
			m_i0e60Len = 0;
		}

		// ���ɱ�������
		res.m_bytResFrameNo = m_objBlackSndSta.m_bytSndFrameCount + 1;
		res.m_bytGpsCount = bytSndCt;
		res.m_bytResType = 0x01; // ��Ԥ����Ϊ�ǽ���֡�ĳɹ�Ӧ��
		memcpy( res.m_aryBlackGps, aryBlackGps, bytSndCt * sizeof(res.m_aryBlackGps[0]) ); 
		res.m_bytFitAreaCount = bytAreaCt;
		res.m_bytFitTotalHi = 0;
		res.m_bytFitTotalLo = 0;

		// �ݴ汾������(�´η���)
		m_obj0e60 = res;
		m_i0e60Len = PKT0E60_BASLEN + sizeof(tag0e60BlackGps) * bytSndCt;

		// ���ü���,�Ա��´�ʹ��
		bytSndCt = 0, bytAreaCt = 0;
	}

	/// **���涼��ᵽ"�ޱ�������"�Ĵ�����** ///

	int iEndType = 0; // 1,��ֹ��2������
	if( m_objBlackSndSta.m_wDoneFileCount >= m_objBlackSndSta.m_wFileTotal || 2 == bytAdvanceEndSymb )
	{
		iEndType = 2;
	}
	else if( 1 == bytAdvanceEndSymb )
	{
		iEndType = 1;
	}

	if(  2 == iEndType ) // ���ǽ�����ѯ
	{
		if( m_i0e60Len > 0 ) // ����ǰһ������ 
		{
			// �޸�Ϊ�ɹ�����֡
			m_obj0e60.m_bytResType = 0x02;
			
			// ����ϻ����������
			m_objBlackSndSta.m_dwGpsTotal += m_obj0e60.m_bytGpsCount;
			if( m_i0e60Len >= 2 ) // (��ȫ���)
			{
				*( (BYTE*)(&m_obj0e60) + m_i0e60Len - 2 ) = BYTE( m_objBlackSndSta.m_dwGpsTotal / 0x7e );
				*( (BYTE*)(&m_obj0e60) + m_i0e60Len - 1 ) = BYTE( m_objBlackSndSta.m_dwGpsTotal % 0x7e );
			}
			
			// �������һ֡����
			iResult = g_objSms.MakeSmsFrame( (char*)&m_obj0e60, m_i0e60Len, 0x0e, bytDataType, buf, sizeof(buf), iBufLen );
			if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9,
				DATASYMB_OE60 | ((objGprsCfg.m_bytChannelBkType_2 & 0x40) ? DATASYMB_SMSBACKUP : 0) );
		}
		else
		{
			// ����ʧ��Ӧ������
			res.m_bytResType = 0x04; // û���������������֡
			
			// ��������
			iResult = g_objSms.MakeSmsFrame( (char*)&res, 11, 0x0e, bytDataType, buf, sizeof(buf), iBufLen );
			if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9, 
				(objGprsCfg.m_bytChannelBkType_2 & 0x40) ? DATASYMB_SMSBACKUP : 0 );
		}

		_KillTimer(&g_objTimerMng, BLACK_SNDRES_TIMER ); // �رն�ʱ��
	}
	else
	{
		_KillTimer(&g_objTimerMng, BLACK_SNDRES_TIMER ); // �رն�ʱ��
		_SetTimer(&g_objTimerMng, BLACK_SNDRES_TIMER, BLACK_SNDINTERVAL, G_TmBlkSndRes ); // �ؿ���ʱ��
	}
}

void CBlk::P_TmBlkRcvQuery()
{
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;
	int iResult = 0;
	BYTE bytDataType = 0;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	bytDataType = 0x60;
#endif
#if 2 == USE_PROTOCOL
	bytDataType = 0x50;
#endif

	DWORD dwCur = GetTickCount();

	if( dwCur - m_objBlackQuerySta.m_dwLstQueryTime >= BLACKQUERY_MAXINTERVAL // ��2�β�ѯ֡�����ʱ
		&& m_objBlackQuerySta.m_bytReqFrameTotal > m_objBlackQuerySta.m_bytReqFrameCount ) // �Ҳ�ѯ֡δ��������
	{
		tag0e60 res;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
		memcpy( &res.m_szCentGroup, m_objBlackQuerySta.m_szCentGroup, sizeof(res.m_szCentGroup) );
#endif
		res.m_bytResType = 0x03; // ��ѯ֡������

		tag2QGprsCfg objGprsCfg;
		GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

		iResult = g_objSms.MakeSmsFrame( (char*)&res, 11, 0x0e, bytDataType, buf, sizeof(buf), iBufLen );
		if( !iResult ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12,
			(objGprsCfg.m_bytChannelBkType_2 & 0x40) ? DATASYMB_SMSBACKUP : 0 );

		_KillTimer(&g_objTimerMng, BLACK_RCVQUERY_TIMER );
	}
	else // ��2�β�ѯ֡���δ��ʱ (��ѯ֡����������������ﲻ���ܳ���)
	{
		m_objBlackQuerySta.m_dwLstQueryTime = dwCur;
		_SetTimer(&g_objTimerMng, BLACK_RCVQUERY_TIMER, BLACKQUERY_MAXINTERVAL, G_TmBlkRcvQuery );
	}
}

int CBlk::_GetCurBlackSaveGps( tagBlackSaveGps &v_objBlackSaveGps )
{
	static bool sta_bFstValid = true;
	tagGPSData gps(0);
	int iResult = 0;

	// ������ü��ģ��Ļ�ȡGPS�ӿڣ���Ϊ�ýӿ�����Ժ�ϻ��ģ���һЩ����
	iResult = g_objMonAlert.GetCurGPS( gps, false );
	if( iResult )
	{
		memset( (void*)&v_objBlackSaveGps, 0, sizeof(v_objBlackSaveGps) );
		return iResult;
	}
	else if( 'A' != gps.valid )			//GPS��Ч�򷵻ش���
	{
		memset( (void*)&v_objBlackSaveGps, 0, sizeof(v_objBlackSaveGps) );
		return ERR_BLACKGPS_INVALID;
	}
// 	// �״���Ч���뱣�棬������֮�����ٶ�Ϊ0�����豣�棬��ʡ�ռ�
// 	else if( !sta_bFstValid && gps.speed < BLKSAVE_MINSPEED )	
// 	{
// 		memset( (void*)&v_objBlackSaveGps, 0, sizeof(v_objBlackSaveGps) );
// 		return ERR_BLACKGPS_INVALID;
// 	}

	sta_bFstValid = false;
	_GpsToBlackSaveGps( gps, v_objBlackSaveGps );
	
	return 0;
}

void CBlk::Init()
{
	BlkSaveStart();
	return ;

	unsigned char ucCurIOSta = 0;
	IOGet(IOS_ACC, &ucCurIOSta);
	
	if( ucCurIOSta == IO_ACC_ON )
	{
		BlkSaveStart();
	}
	else
	{
		PRTMSG(MSG_NOR, "Acc invalid, can not save blk!\n");
	}
}

void CBlk::BlkSaveStart()
{
	if( access(m_szBlackPath, F_OK) != 0 )
	{
		struct stat lDirStat;
		if( 0 == mkdir(m_szBlackPath, lDirStat.st_mode) )
		{
			chown(m_szBlackPath, lDirStat.st_uid, lDirStat.st_gid);
		}
		else
		{
			perror("Blk mkdir failed: ");
			return;
		}
	}

	tag2QBlkCfg objBlkCfg;
	objBlkCfg.Init();
	GetSecCfg( &objBlkCfg, sizeof(objBlkCfg),	offsetof(tagSecondCfg, m_uni2QBlkCfg.m_obj2QBlkCfg), sizeof(objBlkCfg) );

	if( objBlkCfg.m_dwBlackChkInterv < BLACK_MININTERVAL )
		objBlkCfg.m_dwBlackChkInterv = BLACK_MININTERVAL;

	_SetTimer(&g_objTimerMng, BLACK_TIMER, BLACK_MININTERVAL * 1000, G_TmBlkSave ); // �̶���ϻ�Ӽ��
	P_TmBlkSave();

	PRTMSG(MSG_NOR, "Blk Save Timer Open\n" );
}

void CBlk::BlkSaveStop()
{
	if( NULL != m_pfBlack )
	{
		fflush(m_pfBlack);
		fclose(m_pfBlack);
	}
	_KillTimer(&g_objTimerMng, BLACK_TIMER );

	PRTMSG(MSG_NOR, "Blk Save Timer Close\n" );
}

// �������ڲ����ٵ���
int CBlk::GetLatestBlkGps( tagGPSData& v_objGps )
{
	int iRet = 0;
	char szFileName[MAX_PATH] = {0};

	if( m_wBlkGpsCt > 0 && m_pfBlack )
	{
		fwrite( m_aryBlackGps, sizeof(m_aryBlackGps[0]) * m_wBlkGpsCt, 1, m_pfBlack );
		fclose( m_pfBlack );
		m_pfBlack = NULL;

		m_dwLstGpsSaveTm = GetTickCount();
		m_wBlkGpsCt = 0;
	}

	struct dirent **namelist = NULL;
	int n = 0;
	
	n = scandir(m_szBlackPath, &namelist, 0, alphasort);
	if (n > 0)
	{
		char szCompareName[10] = {0};
		char szLatestFile[ MAX_PATH ] = { 0 };
		char szSubLatestFile[ MAX_PATH ] = { 0 };
		memcpy(szLatestFile, "0000_0", strlen("0000_0"));

		while(n--)
		{
			if( 0 == strcmp(".", namelist[n]->d_name) || 0 == strcmp("..", namelist[n]->d_name) )
				continue;	
			if( 14 != strlen(namelist[n]->d_name) )
				continue;

			memcpy(szCompareName, namelist[n]->d_name + 4, 6);
			if( strcmp(szLatestFile, szCompareName) < 0 )
			{
				memcpy( szSubLatestFile, szLatestFile, 6 );
				szSubLatestFile[ 6 ] = 0;
				
				memcpy( szLatestFile, namelist[n]->d_name + 4, 6 );
				szLatestFile[ 6 ] = 0;
			}
		} 

		sprintf(szFileName, "%sBlk_%s.dat", m_szBlackPath,szLatestFile);
		FILE* fp = fopen( szFileName, "rb" );
		if( fp )
		{
			tagBlackSaveGps objBlkSaveGps;
			if( fseek(fp, long(sizeof(tagBlackSaveGps)) * -1, SEEK_END) )
			{
				fclose(fp);
				iRet = ERR_FILE;
				goto _GETSUBLATESTFILE;
			}
			if( 1 != fread(&objBlkSaveGps, sizeof(objBlkSaveGps), 1, fp) )
			{
				fclose(fp);
				iRet = ERR_FILE;
				goto _GETSUBLATESTFILE;
			}

			fclose( fp );

			_BlackSaveGpsToGps( objBlkSaveGps, v_objGps );
			v_objGps.valid = 'V';
			v_objGps.direction = 0;
			v_objGps.speed = 0;
			SetCurGps( &v_objGps, GPS_REAL | GPS_LSTVALID );
			GetCurGps( &v_objGps, sizeof(v_objGps), GPS_LSTVALID );
		}
		else
		{
			iRet = ERR_FILE_FAILED;
			goto _GETSUBLATESTFILE;
		}

		goto _GETCURGPS_END;

_GETSUBLATESTFILE: // �������µĺ�ϻ���ļ���СΪ0���߶�ȡ����,���Դ�ʱҪʹ�ô��µ��ļ�
		sprintf(szFileName, "%sBlk_%s.dat", m_szBlackPath, szSubLatestFile);
		fp = fopen( szFileName, "rb" );
		if( fp )
		{
			tagBlackSaveGps objBlkSaveGps;
			if( fseek(fp, long(sizeof(tagBlackSaveGps)) * -1, SEEK_END) )
			{
				fclose(fp);
				iRet = ERR_FILE;
				goto _GETCURGPS_END;
			}
			if( 1 != fread(&objBlkSaveGps, sizeof(objBlkSaveGps), 1, fp) )
			{
				fclose(fp);
				iRet = ERR_FILE;
				goto _GETCURGPS_END;
			}

			fclose( fp );

			_BlackSaveGpsToGps( objBlkSaveGps, v_objGps );
			v_objGps.valid = 'V';
			v_objGps.direction = 0;
			v_objGps.speed = 0;
			SetCurGps( &v_objGps, GPS_REAL | GPS_LSTVALID );
			GetCurGps( &v_objGps, sizeof(v_objGps), GPS_LSTVALID );
		}
		else
		{
			iRet = ERR_FILE_FAILED;
		}
	}
	else
	{
		iRet = ERR_FILE_FAILED;
	}

_GETCURGPS_END:
	if(NULL != namelist) 
		free(namelist);
	return iRet;
}

void CBlk::SaveCurBlkGps()
{
	P_TmBlkSave();

	// ����ĺ�ϻ������д�����
	if( m_pfBlack )
	{
		if( m_wBlkGpsCt > 0 )
		{
			fwrite( m_aryBlackGps, sizeof(m_aryBlackGps[0]) * m_wBlkGpsCt, 1, m_pfBlack );
			
			m_dwLstGpsSaveTm = GetTickCount();
 			m_wBlkGpsCt = 0;
		}
		else
		{
			fflush( m_pfBlack );
		}

		fclose( m_pfBlack );
		m_pfBlack = NULL;
	}
}

// ����ʱ,Ӧ��֤��ϻ��GPS��������Ч��,���ٶ�>0
int CBlk::_SaveBlackGps( const tagBlackSaveGps &v_objBlackSaveGps )
{
	int iRet = 0;
	DWORD dwCur = 0;
	char szFile[MAX_PATH];
	int iDaysNo = v_objBlackSaveGps.m_bytDay / DSK2_ONEFILE_DAYS + 1;
	sprintf( szFile, "%sBlk_%02d%02d_%d.dat", m_szBlackPath, v_objBlackSaveGps.m_bytYear, v_objBlackSaveGps.m_bytMon, iDaysNo );

	// ����ǰ��Ҫ�����µĺ�ϻ���ļ�,����ԭ��û�д򿪺�ϻ���ļ����
	// ����ǰ����ĺ�ϻ������д���ļ�(����еĻ�),Ȼ��ر��ļ�(����򿪵Ļ�),��ɾ���ɵĺ�ϻ���ļ�
	if( strcmp(m_szBlackFile, szFile) != 0 || !m_pfBlack )
	{
		if( m_wBlkGpsCt > 0 && m_pfBlack )
		{
			fwrite( m_aryBlackGps, sizeof(m_aryBlackGps[0]) * m_wBlkGpsCt, 1, m_pfBlack );
			m_dwLstGpsSaveTm = GetTickCount();
			m_wBlkGpsCt = 0;
		}
		if( m_pfBlack )
		{
			fclose( m_pfBlack );
			m_pfBlack = NULL;
		}

		//g_objFlashPart4Mng.DelOldFile();
	}

	// ����ϻ���ļ������Ч,�򴴽���ǰ�ĺ�ϻ���ļ�
	if( !m_pfBlack )
	{
		m_pfBlack = fopen( szFile, "ab" );
		if( m_pfBlack )
		{
			memcpy(m_szBlackFile, szFile, sizeof(szFile));
		}
		else
		{
			memset(m_szBlackFile, 0 ,sizeof(m_szBlackFile));
			iRet = ERR_FILE_FAILED;
			perror("open blk file error:");
			goto _SAVEBLKGPS_END;
		}
	}
	
	if( m_wBlkGpsCt < sizeof(m_aryBlackGps) / sizeof(m_aryBlackGps[0]) )
	{
		m_aryBlackGps[ m_wBlkGpsCt ++ ] = v_objBlackSaveGps;
	}
	
	dwCur = GetTickCount();
	if( dwCur - m_dwLstGpsSaveTm < BLACK_SAVEINTERVAL && m_wBlkGpsCt < sizeof(m_aryBlackGps) / sizeof(m_aryBlackGps[0]) )
	{
		iRet = 0;
		goto _SAVEBLKGPS_END;
	}

	if( m_pfBlack ) 
	{
		PRTMSG(MSG_DBG, "Begin black write Tm= %d\n", GetTickCount());
		fwrite( m_aryBlackGps, sizeof(m_aryBlackGps[0]) * m_wBlkGpsCt, 1, m_pfBlack );
		fclose( m_pfBlack );
		PRTMSG(MSG_DBG, "End black write Tm= %d\n", GetTickCount());
		m_pfBlack = NULL;
		
		m_dwLstGpsSaveTm = GetTickCount();
		m_wBlkGpsCt = 0;
	}
	else
	{
		iRet = ERR_FILE_FAILED;
	}

_SAVEBLKGPS_END:
	return iRet;
}

//ʹ��write����д��ϻ�����ݣ�������
/*
int CBlk::_SaveBlackGps( const tagBlackSaveGps &v_objBlackSaveGps )
{
	int iRet = 0;
	DWORD dwCur = 0;
	char szFile[MAX_PATH];
	int iDaysNo = v_objBlackSaveGps.m_bytDay / DSK2_ONEFILE_DAYS + 1;
	sprintf( szFile, "%sBlk_%02d%02d_%d.dat", m_szBlackPath, v_objBlackSaveGps.m_bytYear, v_objBlackSaveGps.m_bytMon, iDaysNo );
	
	// ����ǰ��Ҫ�����µĺ�ϻ���ļ�,����ԭ��û�д򿪺�ϻ���ļ����
	// ����ǰ����ĺ�ϻ������д���ļ�(����еĻ�),Ȼ��ر��ļ�(����򿪵Ļ�),��ɾ���ɵĺ�ϻ���ļ�
	if( strcmp(m_szBlackFile, szFile) != 0 || !m_iBlkFile )
	{
		if( m_wBlkGpsCt > 0 && m_iBlkFile )
		{
			write(m_iBlkFile, m_aryBlackGps, sizeof(m_aryBlackGps[0]) * m_wBlkGpsCt);
			m_dwLstGpsSaveTm = GetTickCount();
			m_wBlkGpsCt = 0;
		}
		if( m_iBlkFile )
		{
			close( m_iBlkFile );
			m_iBlkFile = 0;
		}
	}
	
	// ����ϻ���ļ������Ч,�򴴽���ǰ�ĺ�ϻ���ļ�
	if( !m_iBlkFile )
	{
		m_iBlkFile = open( szFile, O_RDWR );
		if( m_iBlkFile )
		{
			memcpy(m_szBlackFile, szFile, sizeof(szFile));
		}
		else
		{
			memset(m_szBlackFile, 0 ,sizeof(m_szBlackFile));
			iRet = ERR_FILE_FAILED;
			perror("open blk file error:");
			goto _SAVEBLKGPS_END;
		}
	}
	
	if( m_wBlkGpsCt < sizeof(m_aryBlackGps) / sizeof(m_aryBlackGps[0]) )
	{
		m_aryBlackGps[ m_wBlkGpsCt ++ ] = v_objBlackSaveGps;
	}
	
	dwCur = GetTickCount();
	if( dwCur - m_dwLstGpsSaveTm < BLACK_SAVEINTERVAL && m_wBlkGpsCt < sizeof(m_aryBlackGps) / sizeof(m_aryBlackGps[0]) )
	{
		iRet = 0;
		goto _SAVEBLKGPS_END;
	}
	
	if( m_iBlkFile ) 
	{
		PRTMSG(MSG_DBG, "begin blk write = %d\n", GetTickCount());
		write(m_iBlkFile,  m_aryBlackGps, sizeof(m_aryBlackGps[0]) * m_wBlkGpsCt);
		close( m_iBlkFile );
		PRTMSG(MSG_DBG, "end blk write = %d\n", GetTickCount());
		m_iBlkFile = 0;
		
		m_dwLstGpsSaveTm = GetTickCount();
		m_wBlkGpsCt = 0;
	}
	else
	{
		iRet = ERR_FILE_FAILED;
	}
	
_SAVEBLKGPS_END:
	return iRet;
}
*/

// ɾ��ָ��GPSʱ��֮��ĺ�ϻ������
void CBlk::DelLaterBlk( const tagGPSData& v_objGps )
{
	if( 'A' != v_objGps.valid ) return;
	
	char szNowFile[32];
	char szCommand[100];
	struct dirent **namelist;
    int n;
	char szTempbuf[4];
	
	sprintf( szNowFile, "Blk_%02d%02d_%d.dat", v_objGps.nYear % 1000, v_objGps.nMonth, v_objGps.nDay / DSK2_ONEFILE_DAYS + 1 );
	
	n = scandir(m_szBlackPath, &namelist, 0, alphasort); // ����ֵ�����0��
    if (n < 0)
        perror("scandir");
	else 
    {
        while(n--) 
        {
			memcpy(szTempbuf, namelist[n]->d_name, 4);

			if( 0 != strcmp("Blk_", szTempbuf) || 0 == strcmp(".", namelist[n]->d_name) || 0 == strcmp("..", namelist[n]->d_name) )
				continue;
			
			if( 0 > strcmp(szNowFile, namelist[n]->d_name) )
			{
				sprintf(szCommand, "rm %s%s", m_szBlackPath, namelist[n]->d_name);
				system(szCommand);
				PRTMSG(MSG_DBG, "Delete file %s%s\n", m_szBlackPath, namelist[n]->d_name);
			}
            free(namelist[n]);
        }
        free(namelist);
    }
}

// ɾ��ָ��GPSʱ��֮ǰ1��ĺ�ϻ������
void CBlk::DelOlderBlk( const tagGPSData& v_objGps )
{
	if( 'A' != v_objGps.valid ) return;
	
	// ת��Ϊ1��ǰ��GPSʱ��
	tagGPSData objGpsJug = v_objGps;
	objGpsJug.nYear --;
	if( objGpsJug.nYear < 2008 )
		objGpsJug.nYear = 2008;
	
	char szCommand[100];
	struct dirent **namelist;
    int n;
	char szTempbuf[4];
	
	char szJugFile[32];
	sprintf( szJugFile, "Blk_%02d%02d_%d.dat", objGpsJug.nYear % 1000, objGpsJug.nMonth, objGpsJug.nDay / DSK2_ONEFILE_DAYS );
	
	n = scandir(m_szBlackPath, &namelist, 0, alphasort);
    if (n < 0)
        perror("scandir");
	else 
    {
        while(n--) 
        {
			memcpy(szTempbuf, namelist[n]->d_name, 4);
			
			if( 0 != strcmp("Blk_", szTempbuf) || 0 == strcmp(".", namelist[n]->d_name) || 0 == strcmp("..", namelist[n]->d_name) )
				continue;
			
			if( 0 < strcmp(szJugFile, namelist[n]->d_name) )
			{
				sprintf(szCommand, "rm %s%s", m_szBlackPath, namelist[n]->d_name);
				system(szCommand);
				PRTMSG(MSG_DBG, "Delete file %s%s\n", m_szBlackPath, namelist[n]->d_name);
			}
            free(namelist[n]);
        }
        free(namelist);
    }
}

void CBlk::_GpsToBlackSaveGps( const tagGPSData &v_objGps, tagBlackSaveGps &v_objBlackSaveGps )
{
	DWORD dwVeSta = g_objCarSta.GetVeSta();

	v_objBlackSaveGps.m_bytDay = BYTE(v_objGps.nDay);
	v_objBlackSaveGps.m_bytMon = BYTE(v_objGps.nMonth);
	v_objBlackSaveGps.m_bytYear = BYTE(v_objGps.nYear % 1000);
	v_objBlackSaveGps.m_bytSec = BYTE(v_objGps.nSecond);
	v_objBlackSaveGps.m_bytMin = BYTE(v_objGps.nMinute);
	v_objBlackSaveGps.m_bytHour = BYTE(v_objGps.nHour);
	
	v_objBlackSaveGps.m_bytMix = 0x40;

	// �ճ�/�س�
	if( dwVeSta & VESTA_HEAVY )
		v_objBlackSaveGps.m_bytMix |= 0x04;
	else
		v_objBlackSaveGps.m_bytMix &= 0xfb;

	// ʡ��/��ʡ��
 	if( g_objQianIO.GetDeviceSta() & DEVSTA_SYSTEMSLEEP )
 		v_objBlackSaveGps.m_bytMix |= 0x10;
 	else
 		v_objBlackSaveGps.m_bytMix &= 0xef;

	// ACC״̬
	if( g_objQianIO.GetDeviceSta() & DEVSTA_ACCVALID )
		v_objBlackSaveGps.m_bytMix |= 0x08;	
	else
		v_objBlackSaveGps.m_bytMix &= 0xf7;

	// GPS�Ƿ���Ч
	if( 'A' == v_objGps.valid ) v_objBlackSaveGps.m_bytMix &= 0xdf;
	else v_objBlackSaveGps.m_bytMix |= 0x20;

	v_objBlackSaveGps.m_lLon = long(v_objGps.longitude * LONLAT_DO_TO_L);
	v_objBlackSaveGps.m_lLat = long(v_objGps.latitude * LONLAT_DO_TO_L);

	double dSpeed = v_objGps.speed * 3.6 / 1.852; // ת��Ϊ����/Сʱ
	v_objBlackSaveGps.m_bytSpeed = BYTE( dSpeed );
}

void CBlk::_BlackSaveGpsToGps( const tagBlackSaveGps &v_objBlkSaveGps, tagGPSData &v_objGps )
{
	v_objGps.nYear = v_objBlkSaveGps.m_bytYear + 2000;
	v_objGps.nMonth = v_objBlkSaveGps.m_bytMon;
	v_objGps.nDay = v_objBlkSaveGps.m_bytDay;
	v_objGps.nHour = v_objBlkSaveGps.m_bytHour;
	v_objGps.nMinute = v_objBlkSaveGps.m_bytMin;
	v_objGps.nSecond = v_objBlkSaveGps.m_bytSec;
	v_objGps.longitude = v_objBlkSaveGps.m_lLon / LONLAT_DO_TO_L;
	v_objGps.latitude = v_objBlkSaveGps.m_lLat / LONLAT_DO_TO_L;
	v_objGps.speed = float( v_objBlkSaveGps.m_bytSpeed * 1.852 / 3.6 );
	v_objGps.direction = 0;
}

void CBlk::_BlackSaveGpsToBlackGps( tagBlackSaveGps &v_objBlackSaveGps, tag0e60BlackGps &v_objBlackGps )
{
	v_objBlackGps.m_bytMix = v_objBlackSaveGps.m_bytMix;
	
	v_objBlackGps.m_bytDay = v_objBlackSaveGps.m_bytDay;
	v_objBlackGps.m_bytMon = v_objBlackSaveGps.m_bytMon;
	v_objBlackGps.m_bytYear = v_objBlackSaveGps.m_bytYear;
	v_objBlackGps.m_bytSec = v_objBlackSaveGps.m_bytSec;
	v_objBlackGps.m_bytMin = v_objBlackSaveGps.m_bytMin;
	v_objBlackGps.m_bytHour = v_objBlackSaveGps.m_bytHour;
	v_objBlackGps.m_bytSpeed = v_objBlackSaveGps.m_bytSpeed;

	// γ��
	double dLat = v_objBlackSaveGps.m_lLat / LONLAT_DO_TO_L;
	v_objBlackGps.m_bytLatDu = BYTE(dLat);
	double dLatFen = ( dLat - v_objBlackGps.m_bytLatDu ) * 60;
	v_objBlackGps.m_bytLatFenzen = BYTE(dLatFen);
	WORD wLatFenxiao = WORD( (dLatFen - v_objBlackGps.m_bytLatFenzen) * 10000 );
	v_objBlackGps.m_bytLatFenxiao1 = wLatFenxiao / 100;
	v_objBlackGps.m_bytLatFenxiao2 = wLatFenxiao % 100;

	// ����
	double dLon = v_objBlackSaveGps.m_lLon / LONLAT_DO_TO_L;
	v_objBlackGps.m_bytLonDu = BYTE(dLon);
	double dLonFen = (dLon - v_objBlackGps.m_bytLonDu) * 60;
	v_objBlackGps.m_bytLonFenzen = BYTE(dLonFen);
	WORD wLonFenxiao = WORD( (dLonFen - v_objBlackGps.m_bytLonFenzen) * 10000 );
	v_objBlackGps.m_bytLonFenxiao1 = wLonFenxiao / 100;
	v_objBlackGps.m_bytLonFenxiao2 = wLonFenxiao % 100;

	// ת��
	if( 0 == v_objBlackGps.m_bytLatDu ) v_objBlackGps.m_bytLatDu = 0x7f;
	if( 0 == v_objBlackGps.m_bytLatFenzen ) v_objBlackGps.m_bytLatFenzen = 0x7f;
	if( 0 == v_objBlackGps.m_bytLatFenxiao1 ) v_objBlackGps.m_bytLatFenxiao1 = 0x7f;
	if( 0 == v_objBlackGps.m_bytLatFenxiao2 ) v_objBlackGps.m_bytLatFenxiao2 = 0x7f;
	if( 0 == v_objBlackGps.m_bytLonFenzen ) v_objBlackGps.m_bytLonFenzen = 0x7f;
	if( 0 == v_objBlackGps.m_bytLonFenxiao1 ) v_objBlackGps.m_bytLonFenxiao1 = 0x7f;
	if( 0 == v_objBlackGps.m_bytLonFenxiao2 ) v_objBlackGps.m_bytLonFenxiao2 = 0x7f;
	if( 0 == v_objBlackGps.m_bytLonDu ) v_objBlackGps.m_bytLonDu = 0xff;
	else if( 0x80 == v_objBlackGps.m_bytLonDu ) v_objBlackGps.m_bytLonDu = 0xfe;
	if( v_objBlackGps.m_bytLonDu & 0x80 ) v_objBlackGps.m_bytMix |= 0x02;
	else v_objBlackGps.m_bytMix &= 0xfd;
	v_objBlackGps.m_bytLonDu &= 0x7f;
	if( 0 == v_objBlackGps.m_bytSpeed ) v_objBlackGps.m_bytSpeed = 0xff;
	else if( 0x80 == v_objBlackGps.m_bytSpeed ) v_objBlackGps.m_bytSpeed = 0xfe;
	if( v_objBlackGps.m_bytSpeed & 0x80 ) v_objBlackGps.m_bytMix |= 0x01;
	else v_objBlackGps.m_bytMix &= 0xfe;
	v_objBlackGps.m_bytSpeed &= 0x7f;
}

bool CBlk::_GetPeriodFromBlkFileName( char* v_szBlkFileName, BYTE& v_bytYear, BYTE& v_bytMon, BYTE& v_bytDayNo )
{
	if( 14 != strlen(v_szBlkFileName) ) return false;
	
	char buf[ 20 ] = { 0 };

	memcpy( buf, v_szBlkFileName + 4, 2 );
	buf[ 2 ] = 0;
	v_bytYear = BYTE( atoi( buf ));
	if( 0 == v_bytYear || v_bytYear > 50 ) return false;

	memcpy( buf, v_szBlkFileName + 6, 2 );
	v_bytMon = BYTE( atoi( buf ));
	if( 0 == v_bytMon || v_bytMon > 12 ) return false;

	buf[ 0 ] = v_szBlkFileName[ 9 ];
	buf[ 1 ] = 0;
	v_bytDayNo = BYTE( atoi( buf ));
	if( v_bytDayNo < 1 || v_bytDayNo > 31 / DSK2_ONEFILE_DAYS + 1 ) return false;

	return true;
}

/// ��ϻ�Ӳ�ѯ����
int CBlk::Deal0e20( char* v_szData, DWORD v_dwDataLen )
{
// ǧ����ͨ�ð棬������_����
// ������ķ����(10)+���������֡������1��+ ��ѯʱ��θ�����1��
// + [ ��ѯʱ���(10) ] x N +��ѯλ�÷�Χ����(1)
// + [ λ�÷�Χ(16)] x N +��ѯ������֡����1��+��ǰ֡��ţ�1��

// ǧ���۹�����_��һ
// ���������֡������1��+ ��ѯʱ��θ�����1��+ [ ��ѯʱ���1 + ���� + ��ѯʱ���n]

	char buf[ 200 ] = { 0 };
	int iBufLen = 0;
	DWORD dwCt = 0;
	BYTE bytPeriCount = 0, bytAreaCount = 0;
	BYTE bytReqFrameNo = 0;
	BYTE byt = 0;
	int iRet = 0;
	tag0e20 req;
	DWORD dwBaseNo = 0;
	DWORD dwBaseLen = 0;

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	dwBaseNo = 10;
	dwBaseLen = 15;
#endif
#if 2 == USE_PROTOCOL
	dwBaseNo = 0;
	dwBaseLen = 2;
#endif

	// �������ȼ��
	if( v_dwDataLen > sizeof(tag0e20) || v_dwDataLen < dwBaseLen )
		return ERR_DATA_INVALID; // ���������ݳ��������ܸ���,����С����С���ܸ���
	memcpy( &req, v_szData, v_dwDataLen );

#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	/// ���������ķ����
	if( strncmp(req.m_szCentGroup, m_objBlackQuerySta.m_szCentGroup, sizeof(req.m_szCentGroup)) )
	{
		memset((void*)&m_objBlackQuerySta, 0, sizeof(m_objBlackQuerySta) ); // �����µķ��飬�����ԭ�еĲ�ѯ״̬
		memcpy( m_objBlackQuerySta.m_szCentGroup, req.m_szCentGroup, sizeof(m_objBlackQuerySta.m_szCentGroup) );
	}
	memset((void*)&m_objBlackQuerySta, 0, sizeof(m_objBlackQuerySta) ); // �����ԭ�еĲ�ѯ״̬
#endif

#if 2 == USE_PROTOCOL
	memset((void*)&m_objBlackQuerySta, 0, sizeof(m_objBlackQuerySta) ); // �����ԭ�еĲ�ѯ״̬
#endif

	// ��ȡ����
	tag2QGprsCfg objGprsCfg;
	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	// �رպ�ϻ�����ݷ��Ͷ�ʱ (ȷ���յ���ϻ�Ӳ�ѯ֡��,�ر����ڽ��еĺ�ϻ�����ݷ���)
	_KillTimer(&g_objTimerMng, BLACK_SNDRES_TIMER );

	dwCt = dwBaseNo;

	/// �����ϴ���֡��,������ϴ�����
	m_objBlackQuerySta.m_bytUploadFrameTotal = BYTE( v_szData[dwCt] );
	m_objBlackQuerySta.m_bytUploadFrameCt = 0;
	if( m_objBlackQuerySta.m_bytUploadFrameTotal < 1 ) m_objBlackQuerySta.m_bytUploadFrameTotal = 1;
	if( m_objBlackQuerySta.m_bytUploadFrameTotal > 100 ) m_objBlackQuerySta.m_bytUploadFrameTotal = 100;

	/// ����ʱ�������
	bytPeriCount = req.m_bytPeriodCount;
	if( 0x7f == bytPeriCount ) bytPeriCount = 0;
	if( bytPeriCount > 5 )
	{
		iRet = ERR_DATA_INVALID;
		goto DEAL0E20_FAILED;
	}
	dwCt += 2 + bytPeriCount * 10;

	// ʱ�����������ȷ����,�ж�֡�Ƿ�����
	if( dwCt > v_dwDataLen )
	{
		iRet = ERR_DATA_INVALID;
		goto DEAL0E20_FAILED;
	}
	
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	/// ������������
	bytAreaCount = BYTE( v_szData[dwCt] );
	if( 0x7f == bytAreaCount ) bytAreaCount = 0;
	if( bytAreaCount > 20 )
	{
		iRet = ERR_DATA_INVALID;
		goto DEAL0E20_FAILED;
	}
	dwCt += 1 + 16 * bytAreaCount;
	if( dwCt + 2 != v_dwDataLen ) // ������������ȷ����,�ж�֡�Ƿ�����
	{
		iRet = ERR_DATA_INVALID;
		goto DEAL0E20_FAILED;
	}

	// ��ѯ֡��֡��
	m_objBlackQuerySta.m_bytReqFrameTotal = BYTE( v_szData[dwCt] );

	// ��ѯ֡���
	bytReqFrameNo = BYTE(v_szData[dwCt + 1]);
#endif

#if 2 == USE_PROTOCOL
	// ������������
	bytAreaCount = 0;
	
	// ��ѯ֡��֡��
	m_objBlackQuerySta.m_bytReqFrameTotal = 1;

	// ��ѯ֡���
	bytReqFrameNo = 1;
#endif

	{
		if( bytReqFrameNo <= sizeof(m_objBlackQuerySta.m_aryReqFrameSymb) && bytReqFrameNo > 0 ) // ��֤����Ƿ������ڴ�
		{
			if( 0 == m_objBlackQuerySta.m_aryReqFrameSymb[ bytReqFrameNo - 1 ] ) // ����֡δ�յ���
			{
				m_objBlackQuerySta.m_bytReqFrameCount ++;
				m_objBlackQuerySta.m_aryReqFrameSymb[ bytReqFrameNo - 1 ] = 1; // �Ժ󽫲����ظ������֡

				/// ����ʱ�������
				if( bytPeriCount > 0 )
				{
					BYTE bytDoneCount = m_objBlackQuerySta.m_bytPeriodCount; // �ѱ����ʱ��θ���
					BYTE bytTotal = BYTE(sizeof(m_objBlackQuerySta.m_aryPeriod) / sizeof(m_objBlackQuerySta.m_aryPeriod[0])); // �ɱ����ʱ�������
					if( bytTotal < bytDoneCount )
					{
						iRet = ERR_INNERDATA_ERR;
						goto DEAL0E20_FAILED;
					}
					BYTE bytCanDealCount = bytTotal - bytDoneCount;
					bytCanDealCount = bytCanDealCount > bytPeriCount ? bytPeriCount : bytCanDealCount; // ���λ����Ա����ʱ��θ���

					if( bytCanDealCount > 0 )
					{
						for( byt = 0; byt < bytCanDealCount; byt ++ )
						{
							memcpy( &(m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ]),
								v_szData + dwBaseNo + 2 + byt * 10, sizeof(m_objBlackQuerySta.m_aryPeriod[0]) );

							// 07.08.01 bug,û�н�7fת��Ϊ0,��ɺ�����ҵ�������ʱ����������ݵĴ���,modified by xun
							if( 0x7f == m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ].m_bytFromHour )
								m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ].m_bytFromHour = 0;
							if( 0x7f == m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ].m_bytFromMin )
								m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ].m_bytFromMin = 0;
							if( 0x7f == m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ].m_bytToHour )
								m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ].m_bytToHour = 0;
							if( 0x7f == m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ].m_bytToMin )
								m_objBlackQuerySta.m_aryPeriod[ bytDoneCount + byt ].m_bytToMin = 0;
						}
					}

					m_objBlackQuerySta.m_bytPeriodCount += bytCanDealCount; // ����ʱ��μ���
				}

				/// ������������
				if( bytAreaCount > 0 )
				{
					BYTE bytDoneCount = m_objBlackQuerySta.m_bytAreaCount; // �ѱ�����������
					BYTE bytTotal = BYTE(sizeof(m_objBlackQuerySta.m_aryArea) / sizeof(m_objBlackQuerySta.m_aryArea[0])); // �ɱ������������
					if( bytTotal < bytDoneCount )
					{
						iRet = ERR_INNERDATA_ERR;
						goto DEAL0E20_FAILED;
					}
					BYTE bytCanDealCount = bytTotal - bytDoneCount;
					bytCanDealCount = bytCanDealCount > bytAreaCount ? bytAreaCount : bytCanDealCount; // ���λ��ɱ�����������

					if( bytCanDealCount > 0 )
					{
						char* szArea;
						for( byt = 0; byt < bytCanDealCount; byt ++ )
						{
							szArea = v_szData + dwBaseNo + 2 + bytPeriCount * 10 + 1 + byt * 16;
							m_objBlackQuerySta.m_aryArea[ bytDoneCount + byt ].m_lLatMin
								= long( ( szArea[0] + ( szArea[1] + (szArea[2]*0.01 + szArea[3]*0.0001) ) / 60.0 ) * LONLAT_DO_TO_L );
							m_objBlackQuerySta.m_aryArea[ bytDoneCount + byt ].m_lLonMin
								= long( ( szArea[4] + ( szArea[5] + (szArea[6]*0.01 + szArea[7]*0.0001) ) / 60.0 ) * LONLAT_DO_TO_L );
							m_objBlackQuerySta.m_aryArea[ bytDoneCount + byt ].m_lLatMax
								= long( ( szArea[8] + ( szArea[9] + (szArea[10]*0.01 + szArea[11]*0.0001) ) / 60.0 ) * LONLAT_DO_TO_L );
							m_objBlackQuerySta.m_aryArea[ bytDoneCount + byt ].m_lLonMax
								= long( ( szArea[12] + ( szArea[13] + (szArea[14]*0.01 + szArea[15]*0.0001) ) / 60.0 ) * LONLAT_DO_TO_L );
						}
					}

					m_objBlackQuerySta.m_bytAreaCount += bytCanDealCount; // ����������������
				}
			}
		}
	}

	if( m_objBlackQuerySta.m_bytReqFrameTotal <= m_objBlackQuerySta.m_bytReqFrameCount ) // ����ѯ֡�ѽ������
	{
		// �رս��պ�ϻ�Ӳ�ѯ������ʱ
		_KillTimer(&g_objTimerMng, BLACK_RCVQUERY_TIMER );
		
		// ����ɵķ���״̬��Ϣ
		memset((void*)&m_objBlackSndSta, 0, sizeof(m_objBlackSndSta) );
		memset((void*)&m_obj0e60, 0, sizeof(m_obj0e60) );
		m_i0e60Len = 0;

		// ��ȡ��ϻ���ļ���
		struct dirent **namelist;
		int n = 0;
		
		n = scandir(m_szBlackPath, &namelist, 0, alphasort);
		if (n < 0)
		{
			perror("scandir");
			iRet = ERR_FILE;
			goto DEAL0E20_FAILED;
		}
		else 
		{
			WORD wFileCount = 0;
			WORD wFileMaxTotal = sizeof(m_objBlackSndSta.m_szBlackFile) / sizeof(m_objBlackSndSta.m_szBlackFile[0]) ;
			
			while(n--) 
			{
				if( 0 == strcmp(".", namelist[n]->d_name) || 0 == strcmp("..", namelist[n]->d_name) ) continue;	
				
				if( 14 != strlen(namelist[n]->d_name) )
					continue;
				
				if( wFileCount >= wFileMaxTotal )
					break;
				
				memcpy(m_objBlackSndSta.m_szBlackFile[wFileCount], namelist[n]->d_name, strlen(namelist[n]->d_name) );

				wFileCount++;
			}
			free(namelist);
			
			// ��¼��ϻ���ļ�����
			m_objBlackSndSta.m_wFileTotal = wFileCount > wFileMaxTotal ? wFileMaxTotal : wFileCount; 
		}

		// ����ʱ�������ļ���
		qsort( m_objBlackSndSta.m_szBlackFile, m_objBlackSndSta.m_wFileTotal, 
			sizeof(m_objBlackSndSta.m_szBlackFile[0]), SortStr );

		// ������ʼʱ������ʱ�������
		if( m_objBlackQuerySta.m_bytPeriodCount >= 2 )
		{
			qsort( m_objBlackQuerySta.m_aryPeriod, m_objBlackQuerySta.m_bytPeriodCount,
				sizeof(m_objBlackQuerySta.m_aryPeriod[0]), SortBlackPeriod );
		}

		// ��ǰ�ĺ�ϻ������,�ӻ��汣�浽������
		if( m_pfBlack )
		{
			if( m_wBlkGpsCt > 0 )
			{
				fwrite( m_aryBlackGps, sizeof(m_aryBlackGps[0]) * m_wBlkGpsCt, 1, m_pfBlack );
				fclose( m_pfBlack );
				m_pfBlack = NULL;
				
				m_dwLstGpsSaveTm = GetTickCount();
				m_wBlkGpsCt = 0;
			}
			else
				fflush( m_pfBlack );
		}

		// �򿪺�ϻ�����ݷ��Ͷ�ʱ
		_SetTimer(&g_objTimerMng, BLACK_SNDRES_TIMER, BLACK_SNDINTERVAL, G_TmBlkSndRes );
	}
	else
	{
		// ���ý��պ�ϻ�Ӳ�ѯ������ʱ
		_SetTimer(&g_objTimerMng, BLACK_RCVQUERY_TIMER, BLACKQUERY_MAXINTERVAL, G_TmBlkRcvQuery );
	}

	return 0;

DEAL0E20_FAILED:
	tag0e60 res;
	BYTE bytDataType = 0;
#if 0 == USE_PROTOCOL || 1 == USE_PROTOCOL
	memcpy( &res.m_szCentGroup, m_objBlackQuerySta.m_szCentGroup, sizeof(res.m_szCentGroup) );
	bytDataType = 0x60;
#endif
#if 2 == USE_PROTOCOL
	bytDataType = 0x50;
#endif
	res.m_bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame((char*)&res, 11, 0x0e, bytDataType, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12,
		(objGprsCfg.m_bytChannelBkType_2 & 0x40) ? DATASYMB_SMSBACKUP : 0 );
	return iRet;
}

/// ҵ��������
int CBlk::Deal0e21( char* v_szData, DWORD v_dwDataLen )
{
	char buf[ SOCK_MAXSIZE ];
	int iBufLen = 0;

	tag2QGprsCfg objGprsCfg;
	GetSecCfg( &objGprsCfg, sizeof(objGprsCfg),	offsetof(tagSecondCfg, m_uni2QGprsCfg.m_obj2QGprsCfg), sizeof(objGprsCfg) );

	tag0e61 res;
	memcpy( &res.m_szCentGroup, v_szData, sizeof(res.m_szCentGroup) );
	res.m_bytResType = 0x01;
	int iRet = g_objSms.MakeSmsFrame((char*)&res, sizeof(res), 0x0e, 0x61, buf, sizeof(buf), iBufLen);
	if( iRet ) return iRet;
	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12,
		(objGprsCfg.m_bytChannelBkType_2 & 0x40) ? DATASYMB_SMSBACKUP : 0 );
	if( iRet ) return iRet;

	memset((void*)&m_objBlackQuerySta, 0, sizeof(m_objBlackQuerySta) );
	memset((void*)&m_objBlackSndSta, 0, sizeof(m_objBlackSndSta) );
	memset((void*) &m_obj0e60, 0, sizeof(m_obj0e60) );
	m_i0e60Len = 0;

	_KillTimer(&g_objTimerMng, BLACK_SNDRES_TIMER );
	_KillTimer(&g_objTimerMng, BLACK_RCVQUERY_TIMER );

	g_objTcpSendMng.DelSpecData( DATASYMB_OE60 ); // ɾ�������ﻹδ���͵ĺ�ϻ������

	return 0;
}

/// ��ϻ�Ӽ������ ��Ӧ��ɹ����ȣ�
int CBlk::Deal1012( char* v_szSrcHandtel, char *v_szData, DWORD v_dwDataLen, bool v_bFromSM )
{
	tag2QBlkCfg objBlkCfg, objBlkCfgBkp;
	tag1012 req;
	int iRet = 0;
	int iBufLen = 0;
	char buf[ SOCK_MAXSIZE ];
	BYTE bytResType;

	// �������
	if( sizeof(tag1012) != v_dwDataLen )
	{
		iRet = ERR_DATA_INVALID;
		goto DEAL1012_FAILED;
	}

	// ��ȡԭʼ����
	iRet = GetSecCfg( &objBlkCfg, sizeof(objBlkCfg), offsetof(tagSecondCfg, m_uni2QBlkCfg.m_obj2QBlkCfg), sizeof(objBlkCfg) );
	if( iRet ) goto DEAL1012_FAILED;
	objBlkCfgBkp = objBlkCfg; // �ȱ���

	// ��֡,��������
	memcpy( &req, v_szData, sizeof(req) );
	if( 0x7f == req.m_bytMin ) req.m_bytMin = 0;
	if( 0x7f == req.m_bytSec ) req.m_bytSec = 0;
	objBlkCfg.m_dwBlackChkInterv = req.m_bytMin * 60 + req.m_bytSec;
	if( objBlkCfg.m_dwBlackChkInterv < BLACK_MININTERVAL )
		objBlkCfg.m_dwBlackChkInterv = BLACK_MININTERVAL;

	// ��֡
	bytResType = 0x01;
	iRet = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x52, buf, sizeof(buf), iBufLen);
	if( !iRet )
	{
		// ����������
		if( iRet = SetSecCfg(&objBlkCfg, offsetof(tagSecondCfg,m_uni2QBlkCfg.m_obj2QBlkCfg),
			sizeof(objBlkCfg)) )
		{
			goto DEAL1012_FAILED;
		}

		// ����Ӧ��
		iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );
		if( !iRet )
		{
			SaveCurBlkGps();
		}
		else
		{
			SetSecCfg(&objBlkCfgBkp, offsetof(tagSecondCfg,m_uni2QBlkCfg.m_obj2QBlkCfg), sizeof(objBlkCfgBkp)); // �ָ�ԭ����
			goto DEAL1012_FAILED;
		}
	}
	else goto DEAL1012_FAILED;

	return 0;

DEAL1012_FAILED:
	bytResType = 0x7f;
	int iRet2 = g_objSms.MakeSmsFrame( (char*)&bytResType, 1, 0x10, 0x52, buf, sizeof(buf), iBufLen);
	if( !iRet2 ) g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV12, v_bFromSM ? DATASYMB_USESMS : 0, v_bFromSM ? v_szSrcHandtel : NULL );

	return iRet;
}

#endif



