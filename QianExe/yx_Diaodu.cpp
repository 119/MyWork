#include "yx_QianStdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-Diaodu   ")

void G_TmSetBespokeProc(void *arg, int arg_len)
{
	g_objDiaodu.P_TmSetBespokeProc();
}
void G_TmBespokeNotProc(void *arg, int arg_len)
{
	g_objDiaodu.P_TmBespokeNotProc();
}

CDiaodu::CDiaodu()
{

}

CDiaodu::~CDiaodu()
{

}

/// ���ĵ���ָʾ
int CDiaodu::Deal0501( char* v_szData, DWORD v_dwDataLen )
{
	int iResult; 
	char buf[ 500 ] = { 0 };

	if( 0 == v_dwDataLen ) 
		return ERR_PAR;

	if( v_dwDataLen + 1 > sizeof(buf) )
		return ERR_SPACELACK;
	
	// ���͵������ݵ�������
	buf[ 0 ] = 0x01;
	buf[ 1 ] = BYTE(0x70);
	memcpy( buf + 2, v_szData, v_dwDataLen );
	iResult = DataPush(buf, WORD(v_dwDataLen + 2), DEV_QIAN, DEV_DIAODU, LV2);

#if USE_TTS == 1 && USE_IRD == 0
	memset(buf, 0, sizeof(buf));
	
	int ilen = Code6To8(v_szData, buf, v_dwDataLen, sizeof(buf));

	//����֡��ʽ����
	//����		У����	·����Ϣ	��������	�û�����
	//1 Byte	1 Byte	4 Byte		1 Byte		0~100 Byte

	char  szbuf[1024] = {0};
	DWORD dwlen = 0;
	
	szbuf[dwlen++] = 0x01;	// 0x01��ʾTTS����
	szbuf[dwlen++] = 0x00;	// 0x01��ʾ�������ű�����

	if( buf[6] == 0x02 || buf[6] == 0x08 )
	{
		memcpy(szbuf+dwlen, "�յ�������Ϣ��", strlen("�յ�������Ϣ��"));
		dwlen += strlen("�յ�������Ϣ��");
		memcpy(szbuf+dwlen, buf+10, ilen-10);
		dwlen += (ilen - 10);
	}
	else if( buf[6] == 0x00 || buf[6] == 0x04 )
	{
		memcpy(szbuf+dwlen, "�յ��㲥��Ϣ��", strlen("�յ��㲥��Ϣ��"));
		dwlen += strlen("�յ��㲥��Ϣ��");
		memcpy(szbuf+dwlen, buf+9, ilen-9);
		dwlen += (ilen - 9);
	}	

	iResult = DataPush(szbuf, dwlen, DEV_QIAN, DEV_DVR, LV2);
#endif

	return iResult;
}

/// ����ָ��(��̵���ָ��)
// Э��:���ٷ�Χ(16)(���½�λ��8+���Ͻ�λ��8) λ��(γ��4+����4)
int CDiaodu::Deal053c( char* v_szData, DWORD v_dwDataLen )
{
	int iRet = 0;

	PRTMSG(MSG_DBG, "dianzhao:\n");
	PrintString(v_szData, v_dwDataLen);

	// ���������س������������
	DWORD dwVeSta = g_objCarSta.GetVeSta();
	if( dwVeSta & VESTA_HEAVY )
	{
		PRTMSG(MSG_DBG, "now is heavy, donot deal dianzhao!\n");
		return 0;
	}

	// �ж������������
	if( v_dwDataLen < 16 ) 
	{
		PRTMSG(MSG_DBG, "053c len err!\n");
		return ERR_PAR;
	}

	// ��ȡ��ǰλ��
	tagGPSData objGps(0);
	iRet = g_objMonAlert.GetCurGPS( objGps, false );
	if( iRet ) return iRet;
	if( 'A' != objGps.valid )
	{
		PRTMSG(MSG_DBG, "gps is invalid, donot deal dianzhao\n" );
		return ERR_GPS_INVALID;
	}
	// ��γ������ת��
	for( DWORD dw = 0; dw < 16; dw ++ )
	{
		if( 0x7f == v_szData[dw] )
			v_szData[dw] = 0;
	}

	// �ж��Ƿ����ٳ���Χ��
	long lMinLon = 0, lMinLat = 0, lMaxLon = 0, lMaxLat = 0;
	lMinLat = long( ( BYTE(v_szData[0]) + ( BYTE(v_szData[1]) + BYTE(v_szData[2]) / 100.0 + BYTE(v_szData[3]) / 10000.0 ) / 60.0 ) * LONLAT_DO_TO_L );
	lMinLon = long( ( BYTE(v_szData[4]) + ( BYTE(v_szData[5]) + BYTE(v_szData[6]) / 100.0 + BYTE(v_szData[7]) / 10000.0 ) / 60.0 ) * LONLAT_DO_TO_L );
	lMaxLat = long( ( BYTE(v_szData[8]) + ( BYTE(v_szData[9]) + BYTE(v_szData[10]) / 100.0 + BYTE(v_szData[11]) / 10000.0 ) / 60.0 ) * LONLAT_DO_TO_L );
	lMaxLon = long( ( BYTE(v_szData[12]) + ( BYTE(v_szData[13]) + BYTE(v_szData[14]) / 100.0 + BYTE(v_szData[15]) / 10000.0 ) / 60.0 ) * LONLAT_DO_TO_L );
	if( !JugPtInRect( long(objGps.longitude * LONLAT_DO_TO_L), long(objGps.latitude * LONLAT_DO_TO_L),
		lMinLon, lMinLat, lMaxLon, lMaxLat ) )
	{
		PRTMSG(MSG_DBG, "Not in the dianzhao area! \n");
		PRTMSG(MSG_DBG, "(%d,%d)\n", long(objGps.longitude * LONLAT_DO_TO_L), long(objGps.latitude * LONLAT_DO_TO_L) );
		PRTMSG(MSG_DBG, "(%d,%d)\n", lMinLon, lMinLat );
		PRTMSG(MSG_DBG, "(%d,%d)\n", lMaxLon, lMaxLat );
		return 0;
	}

	// ���͵������� (����������)
	char buf[ 100 ] = { 0 };
	buf[ 0 ] = 0x01;
	buf[ 1 ] = BYTE(0x74);

	iRet = DataPush(buf, 2, DEV_QIAN, DEV_DIAODU, LV2);;

	PRTMSG(MSG_NOR, "Rcv From Cent: Dianzao,Send to Diaodu\n" );

	return iRet;
}

/// ����ȷ��(��ϸ����ָ��)
// �˿͵绰(15)+���ƺ�(15)+������Ϣ(50) 
int CDiaodu::Deal053d( char* v_szData, DWORD v_dwDataLen )
{
	int iRet = 0;
	char szSelfTel[15] = { 0x20 };
	char buf[ 200 ] = { 0 };

	if( v_dwDataLen < 80 ) return ERR_PAR;

	// ���͵������� (�˿͵绰(15)+���ƺ�(15)+������Ϣ(50)+�����ֻ��ţ�15��)
	buf[0] = 0x01;
	buf[1] = BYTE(0x75);
	memcpy( buf + 2, v_szData, 80 );
	iRet = g_objCfg.GetSelfTel( szSelfTel, sizeof(szSelfTel) );
	if( iRet ) return iRet;
	memcpy( buf + 2 + 80, szSelfTel, 15 ); // ���ӱ����ֻ���

	iRet = DataPush(buf, WORD(2 + v_dwDataLen), DEV_QIAN, DEV_DIAODU, LV2);

	if( !iRet )
	{
		// �ɹ����͵���������,��Ҫ��Ӧ�������
		int iBufLen = 0;
		iRet = g_objSms.MakeSmsFrame( NULL, 0, 0x05, 0x7d, buf, sizeof(buf), iBufLen );

		if( !iRet ) iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9, 0 );
	}

#if USE_LEDTYPE == 2
	g_objLedChuangLi.DealEvent(0x03, 'B', 3);
#endif

	PRTMSG(MSG_NOR, "Rcv from Cent:Qiangda queren,send to Diaodu\n" );

	return iRet;
}

void CDiaodu::Init()
{
	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
	offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	if(objTaxiCfg.m_aryBespokeInf[0]!=0xff&&objTaxiCfg.m_aryBespokeInf[1]!=0xff)
	{
		printf("Begin Check Bespoke:%d:%d",objTaxiCfg.m_aryBespokeInf[0],objTaxiCfg.m_aryBespokeInf[1]);
		if(objTaxiCfg.m_aryBespokeInf[0]==0xee&&objTaxiCfg.m_aryBespokeInf[1]==0xee)
		{
			
			//������ʾԤԼ״̬
			_KillTimer(&g_objTimerMng, SETBESPOKE_TIMER);
#if USE_LEDTYPE == 3
			g_objKTLed.SetLedShow('B',0,30*60*1000);
#endif
			//10���ӽ���һ������
			P_TmBespokeNotProc();
			_SetTimer(&g_objTimerMng, BESPOKENOT_TIMER,10*60*1000,G_TmBespokeNotProc);

		}
		else if(objTaxiCfg.m_aryBespokeInf[0]<=24&&objTaxiCfg.m_aryBespokeInf[1]<=60&&
			objTaxiCfg.m_aryBespokeInf[0]>0&&objTaxiCfg.m_aryBespokeInf[1]>0)
		{
				//һ���ּ��һ��ԤԼ����
			_SetTimer(&g_objTimerMng, SETBESPOKE_TIMER,60000,G_TmSetBespokeProc);
		}
		else 
		{
			objTaxiCfg.m_aryBespokeInf[0]=0xff;
			objTaxiCfg.m_aryBespokeInf[1]=0xff;
			SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
		}
	}
}

void CDiaodu::LedTest()
{
	byte ledTest[]={0x39,0x42 ,0x30 ,0x42 ,0x32 ,0x44 ,0x45 ,0x41 ,0x43 ,0x44 ,0x30 ,0x39
		,0x33 ,0x34 ,0x33 ,0x42 ,0x33 };
	
	byte ledTest2[]={0x39 ,0x24 ,0x4d ,0x50 ,0xe9 ,0x51 ,0xf4 ,0x9d ,0xde ,0x67 ,0x3f ,0x4e ,0xba 
		,0xee ,0x84 ,0x18 ,0x61 };
	printf("rcv led Test ");

#if USE_LEDTYPE == 3
	g_objKTLed.SetBackText('D',ledTest2,1);
#endif
}

void CDiaodu::DzTest()
{
	printf("diaodu test");
	static int iTick=0;
	switch(iTick)
	{
		case 0:
			{
				frm0510 frm;
				frm.type=0x01;
				frm.preTime[0]=0x13;
				frm.preTime[1]=0x01;
				
				sprintf(frm.fromAdd,"%s","������Ѹ����");
				sprintf(frm.endAdd,"%s","�ɷ����޹�˾");

				char buf1[ 200 ] = { 0 };
				buf1[0]=0x01;
				buf1[1]=0x74;
				memcpy( buf1 + 2, &frm, sizeof(frm) );
				DataPush(buf1, (DWORD)(sizeof(frm)+2), DEV_QIAN, DEV_DIAODU, LV2);

				char buf2[100] = { 0 };
				buf2[0] = 6;
				if(0x01==frm.type)
				{
					strcat( buf2, "��ע��,��ԤԼ��Ϣ������" );
					DataPush(buf2, (DWORD)(strlen(buf2) + 1), DEV_QIAN, DEV_LED, LV2);
				}
				else if(0x00==frm.type)
				{
					strcat( buf2, "��ע��,�е�����Ϣ������" );
					DataPush(buf2, (DWORD)(strlen(buf2) + 1), DEV_QIAN, DEV_LED, LV2);
				}

			}
			iTick++;
			break;
		case 1:
			{
				frm0511 frm;
				frm.type=0x01;
				frm.preTime[0]=0x13;
				frm.preTime[1]=0x01;
				frm.earlyTime=0x01;
			
				sprintf(frm.tel,"%s","12345678901");
				sprintf(frm.name,"%s","������");
				sprintf(frm.fromAdd,"%s","�ɶ�����");
				sprintf(frm.endAdd,"%s","ͨ��ʵҵ");
				char buf1[ 200 ] = { 0 };
				buf1[0]=0x01;
				buf1[1]=0x75;
				memcpy( buf1 + 2, &frm, sizeof(frm) );
				DataPush(buf1, (DWORD)(sizeof(frm)+2), DEV_QIAN, DEV_DIAODU, LV2);

				switch(frm.type)
				{
					case 0x00://��ͨ����
						{
#if USE_LEDTYPE == 3
							g_objKTLed.SetLedShow('B',3,30*60*1000);
#endif
							char buf[100] = { 0 };
							buf[0] = 6;
							strcat( buf, "����ɹ�,��ע�������Ϣ" );
							DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);
						}
						break;
					case 0x01://ԤԼ����
						if(frm.earlyTime>0)
						{
							UINT interval=frm.earlyTime*60*1000;
							_SetTimer(&g_objTimerMng, SETBESPOKE_TIMER,interval,G_TmSetBespokeProc);
							char buf[100] = { 0 };
							buf[0] = 6;
							strcat( buf, "����ɹ�,��ע�������Ϣ" );
							DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);
						}	
						break;
					case 0x02://����
						{
#if USE_LEDTYPE == 3
							g_objKTLed.SetLedShow('B',4,30*60*1000);
#endif
							char buf[100] = { 0 };
							buf[0] = 6;
							strcat( buf, "��ע��,�㳵�ѱ�����,�����ǰ��ָ���ص���Ϳ���" );
							DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);
						}
						break;
					default:
						break;
				}

			}
			iTick++;
			break;
		default:
			iTick=0;
			break;
	}
	


}
//����һ����������
int CDiaodu::DealComu3C( char* v_szData, DWORD v_dwDataLen )
{
	printf("Rcv Comu3c");
	Frm12 frm;
	if(v_szData[0]==0x04)//�����绰
	{
		frm.len=3;
		sprintf(frm.num,"%s","110");
	}
	else
	{
		frm.len=11;
		//frm.len=5;
		sprintf(frm.num,"%s","02885120180");
		//sprintf(frm.num,"%s","10086");
	}	
	DataPush((char*)(&frm), (DWORD)(sizeof(frm)), DEV_QIAN, DEV_PHONE, LV2);
	return 0;

}
//�������ȷ��
int CDiaodu::DealComu7B( char* v_szData, DWORD v_dwDataLen )
{
	int iRet = 0;
	int iBufLen = 0;
	char buf[ 200 ] = { 0 };
	bool bUpResult=true;
	byte bytCurHour=0;
	byte bytCurMin=0;		
	tagGPSData objGps(0);
	if( 0 == v_dwDataLen || v_dwDataLen < 1 )
	{
		iRet = ERR_PAR;
		goto DEALCOMU7B_END;
	}
	printf("RcvComu7B");
	//����Ƿ���ԤԼʱ�䣬����û��ԤԼʱ������Ӧ��
	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
	offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	if((objTaxiCfg.m_aryBespokeInf[0]!=0xff&&objTaxiCfg.m_aryBespokeInf[1]!=0xff)&&
		(objTaxiCfg.m_aryBespokeInf[0]!=0xee&&objTaxiCfg.m_aryBespokeInf[1]!=0xee))

	{
		
		//��ȡGPS����
		g_objMonAlert.GetCurGPS( objGps, true );
		bytCurHour=(byte)objGps.nHour;
		bytCurMin=(byte)objGps.nMinute;
		if(bytCurHour<objTaxiCfg.m_aryBespokeInf[0])
			bUpResult=false;
		else if(bytCurHour==objTaxiCfg.m_aryBespokeInf[0])
		{
			if(bytCurMin<objTaxiCfg.m_aryBespokeInf[1])
				bUpResult=false;

		}
		if(!bUpResult)
		{
			char buf[100] = { 0 };
			buf[0] = 6;
			strcat( buf, "δ��ԤԼʱ��,����ȷ��" );
			DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);
			return 0;
		}
		
	}
	
	/// ���͵��ٽ������Ӧ������
	// ������Э��: ���ٽ��(1)  ���ٽ����0x00��ʧ�ܣ� 0x01���ɹ���
	iRet = g_objSms.MakeSmsFrame( v_szData, v_dwDataLen, 0x05, 0x7e, buf, sizeof(buf), iBufLen, CVT_8TO6 );
	if( iRet )
	{
		goto DEALCOMU7B_END;
	}

	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9);
	if( iRet )
	{
		goto DEALCOMU7B_END;
	}

	PRTMSG(MSG_NOR, "Rcv from diaodu:Dianzao result,Send to Cent\n");

DEALCOMU7B_END:
	//������ٴ�����
	if(v_szData[0]==0x01) DzCheck(true);
	else DzCheck(false);
	return iRet;

}
//���촥��������ָʾӦ��
int CDiaodu::DealComu73( char* v_szData, DWORD v_dwDataLen )
{
	printf("RcvComu73:%d",v_szData[0]);
	int iRet = 0;
	int iBufLen = 0;
	BYTE bytResType = 0;
	char buf[ 300 ] = { 0 };
	char res[100]={0};
	int  resLen=0;
	
	if(v_szData[0]==0x01)
	{
		memcpy(m_objDiaoduInfo.m_szData,"����",5);

	}
	else memcpy(m_objDiaoduInfo.m_szData,"�ܾ�",5);
	m_objDiaoduInfo.m_bytLen=6+2+5;
	m_objDiaoduInfo.m_bytDataLen=5;
	m_objDiaoduInfo.m_bytDataType=0x03;
	resLen=7+2+m_objDiaoduInfo.m_bytDataLen;

	memcpy(res,&m_objDiaoduInfo,resLen);
	byte crc=0;
	for(int j=2; j<resLen; j++)	
 		crc += buf[j];	// ����У����
	buf[1] = crc;		

	iRet = g_objSms.MakeSmsFrame(res, resLen, 0x05, 0x41, buf, sizeof(buf), iBufLen,CVT_8TO6 );
	if( !iRet )
	iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9);
	return iRet;

}
//ԤԼ����--zzg add
int CDiaodu::Deal0510( char* v_szData, DWORD v_dwDataLen )
{
	if(v_dwDataLen<sizeof(frm0510)) return ERR_PAR;

	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
	offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	if(objTaxiCfg.m_aryBespokeInf[0]!=0xff&&objTaxiCfg.m_aryBespokeInf[1]!=0xff)//��ԤԼ
	{
		printf("Rcv 0510,But Hav Bespoke");
		return ERR_PAR;
	}
	printf("Rcv 0510");
	memcpy(&m_objDzInfo,v_szData,sizeof(m_objDzInfo));
    //_Send0550();

	char buf1[ 200 ] = { 0 };
	buf1[0]=0x01;
	buf1[1]=0x74;
	memcpy( buf1 + 2, v_szData, v_dwDataLen );
	DataPush(buf1, (DWORD)(v_dwDataLen+2), DEV_QIAN, DEV_DIAODU, LV2);

	char buf2[100] = { 0 };
	buf2[0] = 6;
	if(0x01==m_objDzInfo.type)
	{
		strcat( buf2, "��ע��,��ԤԼ��Ϣ������" );
		DataPush(buf2, (DWORD)(strlen(buf2) + 1), DEV_QIAN, DEV_LED, LV2);
	}
	else if(0x00==m_objDzInfo.type)
	{
		strcat( buf2, "��ע��,�е�����Ϣ������" );
		DataPush(buf2, (DWORD)(strlen(buf2) + 1), DEV_QIAN, DEV_LED, LV2);
	}
}

//ȷ������
void CDiaodu::Send0550()
{
	printf("Snd 0550");

	char buf2[100] = { 0 };
	buf2[0] = 6;
	strcat( buf2, "����ȷ������,��ȴ�������" );
	DataPush(buf2, (DWORD)(strlen(buf2) + 1), DEV_QIAN, DEV_LED, LV2);

	char buf[200]={0};
	int iRet=0;
	int iBufLen = 0;
	iRet = g_objSms.MakeSmsFrame( NULL, 0, 0x05, 0x50, buf, sizeof(buf), iBufLen );
	if( !iRet ) iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9, 0 );
}
//���ٽ��Ӧ��
void CDiaodu::DzCheck(bool v_bResult)
{
	//�ر�ԤԼ��������
	_KillTimer(&g_objTimerMng, SETBESPOKE_TIMER);
	_KillTimer(&g_objTimerMng, BESPOKENOT_TIMER);

#if USE_LEDTYPE == 3
	//�ر�B����Ϣ
	g_objKTLed.SetLedShow('B',0xff,0);
#endif

	//������ʾ
	char buf[100] = { 0 };
	buf[0] = 6;
	if(v_bResult)
	strcat( buf, "�ɹ��ӵ�����,���н���" );
	else strcat( buf, "δ�ӵ�����,���н���" );
	DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);

	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
	offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	objTaxiCfg.m_aryBespokeInf[0]=0xff;
	objTaxiCfg.m_aryBespokeInf[1]=0xff;
	SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));

}
//����ȷ��--zzg add
int CDiaodu::Deal0511( char* v_szData, DWORD v_dwDataLen )
{
	if(v_dwDataLen<sizeof(frm0511)) return ERR_PAR;
	
	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
	offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	if(objTaxiCfg.m_aryBespokeInf[0]!=0xff&&objTaxiCfg.m_aryBespokeInf[1]!=0xff)//��ԤԼ
	{
		printf("Rcv 0511,But Hav Bespoke");
		return ERR_PAR;
	}
		

	printf("Rcv 0511");
	char temp[ 200 ] = { 0 };
	temp[0]=0x01;
	temp[1]=0x75;
	memcpy( temp + 2, v_szData, v_dwDataLen );
	DataPush(temp, (DWORD)(v_dwDataLen+2), DEV_QIAN, DEV_DIAODU, LV2);

	char buf[200]={0};
	int iRet=0;
	frm0511 frm;
	memcpy(&frm,v_szData,sizeof(frm));

	switch(frm.type)
	{
		case 0x00://��ͨ����
			{
#if USE_LEDTYPE == 3
				g_objKTLed.SetLedShow('B',3,30*60*1000);
#endif

				char buf[100] = { 0 };
				buf[0] = 6;
				strcat( buf, "����ɹ�,��ע�������Ϣ" );
				DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);
				//ȡ��ԤԼ��¼
// 				tagTaxiCfg objTaxiCfg;
// 				memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
// 				GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
// 				offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
// 				objTaxiCfg.m_aryBespokeInf[0]=0xff;
// 				objTaxiCfg.m_aryBespokeInf[1]=0xff;
// 
// 				SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
			}
			break;
		case 0x01://ԤԼ����
			if(frm.earlyTime>0)
			{
				//UINT interval=frm.earlyTime*60*1000;
				//_SetTimer(&g_objTimerMng, SETBESPOKE_TIMER,interval,G_TmSetBespokeProc);

				char buf[100] = { 0 };
				buf[0] = 6;
				strcat( buf, "����ɹ�,��ע�������Ϣ" );
				DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);
				byte earlyTime=frm.earlyTime;
				byte bespokeTime[2]={0};
				memcpy(bespokeTime,frm.preTime,sizeof(bespokeTime));
				bespokeTime[0]-=earlyTime/60;
				if(earlyTime%60>bespokeTime[1])
				{
					bespokeTime[0]--;
					bespokeTime[1]=60-(earlyTime%60-bespokeTime[1]);
				}
				else bespokeTime[1]-=earlyTime%60;
				printf("Save Bespoke Time:%d:%d",bespokeTime[0],bespokeTime[1]);

				tagTaxiCfg objTaxiCfg;
				memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
				GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
				offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
				memcpy(objTaxiCfg.m_aryBespokeInf,bespokeTime,sizeof(objTaxiCfg.m_aryBespokeInf));
				SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
				//һ���ּ��һ��ԤԼ����
				_KillTimer(&g_objTimerMng, SETBESPOKE_TIMER);
				_KillTimer(&g_objTimerMng, BESPOKENOT_TIMER);
				_SetTimer(&g_objTimerMng, SETBESPOKE_TIMER,60*1000,G_TmSetBespokeProc);
			}	
			break;
		case 0x02://����
			{
#if USE_LEDTYPE == 3
				g_objKTLed.SetLedShow('B',4,30*60*1000);
#endif
				char buf[100] = { 0 };
				buf[0] = 6;
				strcat( buf, "��ע��,�㳵�ѱ�����,�����ǰ��ָ���ص���Ϳ���" );
				DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);
				//ȡ��ԤԼ��¼
// 				tagTaxiCfg objTaxiCfg;
//				memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
// 				GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
// 				offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
// 				objTaxiCfg.m_aryBespokeInf[0]=0xff;
// 				objTaxiCfg.m_aryBespokeInf[1]=0xff;
// 				SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
			}
			break;
		default:
			break;
	}
	int iBufLen = 0;
	iRet = g_objSms.MakeSmsFrame( NULL, 0, 0x05, 0x51, buf, sizeof(buf), iBufLen );
	if( !iRet ) iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9, 0 );
	return iRet;

}

//LED�����Զ�����Ϣ����(C��D ��)
int CDiaodu::Deal4208( char* v_szData, DWORD v_dwDataLen )
{
	if(v_dwDataLen<19 || (v_szData[1]*17)>v_dwDataLen-2) return ERR_PAR;
	char buf[200]={0};
	int iRet=0;
	char bResult=0x01;
	switch(v_szData[0])
	{
		case 0x01://��ʱ��Ϣ
			{
				printf("rcv led c class text ");
#if USE_LEDTYPE == 3
				g_objKTLed.SetBackText('C',(byte*)(v_szData+2),1);
#endif
			}
			break;

		case 0x02://������Ϣ
			{
				printf("rcv led d class text ");
#if USE_LEDTYPE == 3
				g_objKTLed.SetBackText('D',(byte*)(v_szData+2),v_szData[1]);
#endif
			}
			break;

		default:
			bResult=0x02;
			break;
	}

	int iBufLen = 0;
	iRet = g_objSms.MakeSmsFrame( &bResult, 1, 0x42, 0x48, buf, sizeof(buf), iBufLen );
	if( !iRet ) iRet = g_objSock.SOCKSNDSMSDATA( buf, iBufLen, LV9, 0 );
	return iRet;
}

void CDiaodu::P_TmSetBespokeProc()
{
	//_KillTimer(&g_objTimerMng, SETBESPOKE_TIMER);
	
	bool bSetBespoke=false;
	tagTaxiCfg objTaxiCfg;
	memset(&objTaxiCfg,0,sizeof(objTaxiCfg));
	GetSecCfg(&objTaxiCfg,sizeof(objTaxiCfg),
	offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));

	
	byte bytCurHour=0;
	byte bytCurMin=0;
	//��ȡGPS����
	tagGPSData objGps(0);
	g_objMonAlert.GetCurGPS( objGps, true );
	bytCurHour=(byte)objGps.nHour;
	bytCurMin=(byte)objGps.nMinute;
	//tagQianGps objQianGps;
	//g_objMonAlert.GpsToQianGps( objGps, objQianGps );
	printf("Bespoke:%d,%d;CurTim:%d,%d",objTaxiCfg.m_aryBespokeInf[0],
		objTaxiCfg.m_aryBespokeInf[1],bytCurHour,bytCurMin);
	if(objTaxiCfg.m_aryBespokeInf[0]>=24)
	{
		_KillTimer(&g_objTimerMng, SETBESPOKE_TIMER);
		objTaxiCfg.m_aryBespokeInf[0]=0xff;
		objTaxiCfg.m_aryBespokeInf[1]=0xff;
		SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
	}
	if(bytCurHour>=objTaxiCfg.m_aryBespokeInf[0])
	{
		if(bytCurHour==objTaxiCfg.m_aryBespokeInf[0])
		{
			if(bytCurMin>=objTaxiCfg.m_aryBespokeInf[1])	bSetBespoke=true;
		}
		else bSetBespoke=true;
	}
   if(bSetBespoke)
   {
		objTaxiCfg.m_aryBespokeInf[0]=0xee;//�ѽ���ԤԼ״̬
		objTaxiCfg.m_aryBespokeInf[1]=0xee;
		SetSecCfg(&objTaxiCfg,offsetof(tagSecondCfg, m_uni2QTaxiCfg.m_obj2QTaxiCfg), sizeof(objTaxiCfg));
		//������ʾԤԼ״̬
		_KillTimer(&g_objTimerMng, SETBESPOKE_TIMER);

#if USE_LEDTYPE == 3
		g_objKTLed.SetLedShow('B',0,30*60*1000);
#endif
		//10���ӽ���һ������
		P_TmBespokeNotProc();
		_SetTimer(&g_objTimerMng, BESPOKENOT_TIMER,10*60*1000,G_TmBespokeNotProc);
   }
	

}
void CDiaodu::P_TmBespokeNotProc()
{
	//_KillTimer(&g_objTimerMng, BESPOKENOT_TIMER);
	char buf[100] = { 0 };
	buf[0] = 6;
	strcat( buf, "�㳵�ѱ�ԤԼ���밴ʱǰ�����Ϳ���" );
	DataPush(buf, (DWORD)(strlen(buf) + 1), DEV_QIAN, DEV_LED, LV2);

#if USE_LEDTYPE == 3	
	//LED����ˢ��ԤԼ��ʾ
	g_objKTLed.SetLedShow('B',0,30*60*1000);
#endif

}

