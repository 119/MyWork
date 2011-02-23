#include "yx_common.h"

static BYTE g_bytViChn = 0;
static BYTE g_bytVoChn = 0;

bool g_bDeal380A = false;
int g_iDeal380A = 0;
char g_szDeal380A[1024] = {0};

int IDLE_DELAY = 20; //���� 
int CTRL_DELAY = 20; //���� 

tagPhotoInfo g_objPhotoInfo;

char szCurTime[9]="00:00:00";
char szIdleEndTime[9]="00:00:00";
char szRecEndTime[9]="00:00:00";
char szUplEndTime[9]="00:00:00";
char szPicEndTime[9]="00:00:00";
char szTtsEndTime[9]="00:00:00";
char szBlkEndTime[9]="00:00:00";

bool IDLESTATE(void *Event)
{
	if( *((BYTE*)Event)==IDLE )
		return true;
	else
		return false;
}

bool CTRLSTATE(void *Event)
{
	if( *((BYTE*)Event)==CTRL )
		return true;
	else
		return false;
}

bool SLEEPSTATE(void *Event)
{
	if( *((BYTE*)Event)==SLEEP )
		return true;
	else
		return false;
}

void FlushSysState( OSD_REGION v_objRegion[], BOOL v_bRegionOn[], int v_iFlag )
{
#if VEHICLE_TYPE == VEHICLE_M

#if NO_INFO == 0
	int i;
	int iRegionNum;
	int	iLocation[4][2];
	char szContent[50];
	OSD_BITMAP objBitmap;
	
	if( v_iFlag == 0 )
	{
		for(i = 0; i < 4; i++)
		{
			if( v_bRegionOn[i] )
			{
				sprintf(szContent, "%d", i+1);
				if( g_objIdleStart.VPreview[i] )
					strcat(szContent, " Ԥ��");
				else
					strcat(szContent, "     ");
				if( g_objWorkStart.VRecord[i] )
					strcat(szContent, " ¼��");
				else
					strcat(szContent, "     ");
				if( g_objWorkStart.VUpload[i] )
					strcat(szContent, " ���");
				else
					strcat(szContent, "     ");
				strcat(szContent, "\n");
						
				CreateBitmap(&objBitmap, 1, CountCharRow(szContent));							
				DrawCharacter(&objBitmap, szContent, WHITE, 0x000000, 0, 0, FALSE);	
				ShowBitmap(&(v_objRegion[i]), &objBitmap, 0, 0);
			}
		}
	}
	else if( v_iFlag == 1 )
	{
		switch( g_objMvrCfg.VInput.VMode )
		{
			case CIF:
				iLocation[0][0] = 2;
				iLocation[0][1] = 2;
				iLocation[1][0] = 2;
				iLocation[1][1] = 42;
				iLocation[2][0] = 13;
				iLocation[2][1] = 2;
				iLocation[3][0] = 13;
				iLocation[3][1] = 42;
				iRegionNum = 4;
				break;
			case HD1:
				iLocation[0][0] = 2;
				iLocation[0][1] = 2;
				iLocation[1][0] = 13;
				iLocation[1][1] = 2;
				iRegionNum = 2;
				break;
			case D1:
				iLocation[0][0] = 2;
				iLocation[0][1] = 2;
				iRegionNum = 1;
				break;
			default:
				break;
		}
		
		for(i = 0; i < iRegionNum; i++)
		{
			if( !v_bRegionOn[i] )
			{
				sprintf(szContent, "%d", i+1);
				if( g_objIdleStart.VPreview[i] )
					strcat(szContent, " Ԥ��");
				else
					strcat(szContent, "     ");
				if( g_objWorkStart.VRecord[i] )
					strcat(szContent, " ¼��");
				else
					strcat(szContent, "     ");
				if( g_objWorkStart.VUpload[i] )
					strcat(szContent, " ���");
				else
					strcat(szContent, "     ");
				strcat(szContent, "\n");
						
				CreateRegion(&(v_objRegion[i]), 0, iLocation[i][0], iLocation[i][1], 1, CountCharRow(szContent));
				CreateBitmap(&objBitmap, 1, CountCharRow(szContent));							
				DrawCharacter(&objBitmap, szContent, WHITE, 0x000000, 0, 0, FALSE);	
				ShowBitmap(&(v_objRegion[i]), &objBitmap, 0, 0);
				v_bRegionOn[i] = TRUE;
			}
		}
	}
	else if( v_iFlag == -1 )
	{
		for(i = 0; i < 4; i++)
		{
			if( v_bRegionOn[i] )
			{
				DeleteRegion(v_objRegion[i]);
				v_bRegionOn[i] = FALSE;
			}
		}
	}
#endif
	
#endif
}

void FlushShowTime( OSD_REGION *v_objRegion, int v_iCDateTime[], BOOL *v_bRegionOn, int v_iFlag )
{
#if VEHICLE_TYPE == VEHICLE_M

#if NO_INFO == 0
	char szContent[50] = {0};
	OSD_BITMAP objBitmap;
	
	if( v_iFlag == 0 )// ˢ��ʱ����
	{
		if( *v_bRegionOn )
		{
			sprintf(szContent, "%04d-%02d-%02d  %02d:%02d:%02d\n", v_iCDateTime[0], v_iCDateTime[1], v_iCDateTime[2], v_iCDateTime[3], v_iCDateTime[4], v_iCDateTime[5]);
//			CreateBitmap(&objBitmap, 1, 20);							
//			DrawCharacter(&objBitmap, szContent, WHITE, 0x000000, 0, 0, FALSE);	
//			ShowBitmap( v_objRegion, &objBitmap, 0, 0 );
			CreateBitmap(&objBitmap, 1, 20);							
//			DrawCharacter(&objBitmap, szContent, 0x7c00, 0x03e0, 0, 0, FALSE);	
			ShowBitmap( v_objRegion, &objBitmap, 0, 0 );
		}
	}
	else if( v_iFlag == 1 )// ����ʱ����
	{
		if( !(*v_bRegionOn) )
		{
			sprintf(szContent, "%04d-%02d-%02d  %02d:%02d:%02d\n", v_iCDateTime[0], v_iCDateTime[1], v_iCDateTime[2], v_iCDateTime[3], v_iCDateTime[4], v_iCDateTime[5]);
//			CreateRegion( v_objRegion, 0, 1, 20, 1, 20 );
//			CreateBitmap( &objBitmap, 1, 20 );
//			DrawCharacter(&objBitmap, szContent, WHITE, 0x000000, 0, 0, FALSE);
//			ShowBitmap( v_objRegion, &objBitmap, 0, 0 );
			CreateRegion( v_objRegion, 0, 10, 20, 1, 20 );
			CreateBitmap( &objBitmap, 1, 20 );
//			DrawCharacter(&objBitmap, szContent, 0x7c00, 0x03e0, 0, 0, FALSE);
			ShowRegion(*v_objRegion);
			ShowBitmap( v_objRegion, &objBitmap, 0, 0 );

			*v_bRegionOn = TRUE;
		}
	}
	else if( v_iFlag == -1 )// ɾ��ʱ����
	{
		if( *v_bRegionOn )
		{
			DeleteRegion(*v_objRegion);
			*v_bRegionOn = FALSE;
		}
	}
#endif

#endif
}

void RenewRecRptBuf(WORK_EVENT *v_objWorkEvent, byte *v_bytRecRptBuf)
{
	*(v_bytRecRptBuf+9) = 0x00;
	*(v_bytRecRptBuf+10) = 0x00;

	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_IRDA )    *(v_bytRecRptBuf+9) |= 0x10; //ң��ָ��
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_CENT )    *(v_bytRecRptBuf+9) |= 0x40; //����ָ��
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_PERIOD )  *(v_bytRecRptBuf+9) |= 0x20; //ʱ���ź�
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ACCON )   *(v_bytRecRptBuf+9) |= 0x80; //Acc�ź�
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_MOVE )    *(v_bytRecRptBuf+9) |= 0x08; //�˶��ź�
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_OVERLAY ) *(v_bytRecRptBuf+9) |= 0x04; //�ڵ��ź�
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ALERT )   *(v_bytRecRptBuf+9) |= 0x02; //S1�ź�
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ALERT2 )  *(v_bytRecRptBuf+9) |= 0x01; //S2�ź�
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ALERT3 )  *(v_bytRecRptBuf+10) |= 0x80; //S3�ź�
	if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ALERT4 )  *(v_bytRecRptBuf+10) |= 0x40; //S4�ź�
}

void RenewIdleTime( int v_iCurDateTime[], char *v_szIdleEndTime, int v_bFlag )
{
	if( v_bFlag == 0 )
	{
		sprintf( v_szIdleEndTime, "%02d:%02d:%02d", (v_iCurDateTime[3]+(v_iCurDateTime[4]+IDLE_DELAY)/60), ((v_iCurDateTime[4]+IDLE_DELAY)%60), v_iCurDateTime[5] );
	}
	else if( v_bFlag == -1 )
	{
		strcpy(v_szIdleEndTime, "00:00:00");
	}
}

void RenewWorkTime( int v_iWorkStyle, int v_iCurDateTime[], char *v_szEndTime, WORK_EVENT *v_objWorkEvent, int v_bFlag )
{
	char szEventEndTime[9] = {0};
	
	if( v_bFlag == 0 )
	{
		switch(v_iWorkStyle)
		{
			case WORK_REC:
			{
				//ң��ָ��
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_IRDA )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", (v_iCurDateTime[3]+(v_iCurDateTime[4]+30)/60), ((v_iCurDateTime[4]+30)%60), v_iCurDateTime[5] );//ң��Ĭ��¼��30����
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_IRDA;
				}
				
				//����ָ��
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_CENT )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(3600*v_iCurDateTime[3] + 60*v_iCurDateTime[4] + v_iCurDateTime[5] + g_objMvrCfg.AVUpload.UploadSecond)/3600, 
									((3600*v_iCurDateTime[3] + 60*v_iCurDateTime[4] + v_iCurDateTime[5] + g_objMvrCfg.AVUpload.UploadSecond)%3600)/60, 
									(3600*v_iCurDateTime[3] + 60*v_iCurDateTime[4] + v_iCurDateTime[5] + g_objMvrCfg.AVUpload.UploadSecond)%60 );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_CENT;
				}
				
				//ʱ���ź�(��ʱ1�룬��ֹ��ʱ)
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_PERIOD )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(3600*v_iCurDateTime[3] + 60*v_iCurDateTime[4] + v_iCurDateTime[5] + 1)/3600, 
									((3600*v_iCurDateTime[3] + 60*v_iCurDateTime[4] + v_iCurDateTime[5] + 1)%3600)/60, 
									(3600*v_iCurDateTime[3] + 60*v_iCurDateTime[4] + v_iCurDateTime[5] + 1)%60 );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_PERIOD;
				}
				
				//Acc�ź�
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ACCON )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(v_iCurDateTime[3] + (v_iCurDateTime[4]+g_objMvrCfg.EventTrig.AccDelay)/60), 
									((v_iCurDateTime[4] + g_objMvrCfg.EventTrig.AccDelay) % 60), 
									v_iCurDateTime[5] );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ACCON;
				}
				
				//�˶��ź�
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_MOVE )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(v_iCurDateTime[3] + (v_iCurDateTime[4] + g_objMvrCfg.EventTrig.MoveDelay) / 60), 
									((v_iCurDateTime[4] + g_objMvrCfg.EventTrig.MoveDelay) % 60), 
									v_iCurDateTime[5] );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_MOVE;
				}
				
				//�ڵ��ź�
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_OVERLAY )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(v_iCurDateTime[3] + (v_iCurDateTime[4] + g_objMvrCfg.EventTrig.OverlayDelay)/60), 
									((v_iCurDateTime[4] + g_objMvrCfg.EventTrig.OverlayDelay) % 60), 
									v_iCurDateTime[5] );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_OVERLAY;
				}
				
				//S1�ź�
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ALERT )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(v_iCurDateTime[3] + (v_iCurDateTime[4] + g_objMvrCfg.EventTrig.S1Delay) / 60), 
									((v_iCurDateTime[4] + g_objMvrCfg.EventTrig.S1Delay) % 60), 
									v_iCurDateTime[5] );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT;
				}
				
				//S2�ź�
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ALERT2 )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(v_iCurDateTime[3] + (v_iCurDateTime[4] + g_objMvrCfg.EventTrig.S2Delay) / 60), 
									((v_iCurDateTime[4] + g_objMvrCfg.EventTrig.S2Delay) % 60), 
									v_iCurDateTime[5] );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT2;
				}
				
				//S3�ź�
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ALERT3 )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(v_iCurDateTime[3] + (v_iCurDateTime[4] + g_objMvrCfg.EventTrig.S3Delay) / 60), 
									((v_iCurDateTime[4] + g_objMvrCfg.EventTrig.S3Delay) % 60), 
									v_iCurDateTime[5] );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT3;
				}
				
				//S4�ź�
				if( v_objWorkEvent->dwRecStartEvt & EVENT_TYPE_ALERT4 )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
									(v_iCurDateTime[3] + (v_iCurDateTime[4] + g_objMvrCfg.EventTrig.S4Delay) / 60), 
									((v_iCurDateTime[4] + g_objMvrCfg.EventTrig.S4Delay) % 60), 
									v_iCurDateTime[5] );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT4;
				}
			}
				break;
			case WORK_UPL:
			{
				if( v_objWorkEvent->dwUplStartEvt & EVENT_TYPE_CENT )
				{
					sprintf( szEventEndTime, "%02d:%02d:%02d", 
						(3600*v_iCurDateTime[3]+60*v_iCurDateTime[4]+v_iCurDateTime[5]+g_objMvrCfg.AVUpload.UploadSecond)/3600, 
						((3600*v_iCurDateTime[3]+60*v_iCurDateTime[4]+v_iCurDateTime[5]+g_objMvrCfg.AVUpload.UploadSecond)%3600)/60, 
						(3600*v_iCurDateTime[3]+60*v_iCurDateTime[4]+v_iCurDateTime[5]+g_objMvrCfg.AVUpload.UploadSecond)%60 );
					if(strcmp(v_szEndTime, szEventEndTime) < 0) strcpy(v_szEndTime, szEventEndTime);
					v_objWorkEvent->dwUplStartEvt &= ~EVENT_TYPE_CENT;
				}
			}
				break;
			case WORK_PIC:
			{
				if( v_objWorkEvent->dwPicStartEvt & EVENT_TYPE_CENT )
				{
					v_objWorkEvent->dwPicStartEvt &= ~EVENT_TYPE_CENT;
				}
			}
				break;
			case WORK_TTS:
			{
				if( v_objWorkEvent->dwTtsStartEvt & EVENT_TYPE_CENT )
				{
					v_objWorkEvent->dwTtsStartEvt &= ~EVENT_TYPE_CENT;
				}
			}
				break;
			case WORK_BLK:
			{
				if( v_objWorkEvent->dwBlkStartEvt & EVENT_TYPE_CENT )
				{
					v_objWorkEvent->dwBlkStartEvt &= ~EVENT_TYPE_CENT;
				}
			}
				break;
			default:
				break;
		}
	}
	else if( v_bFlag == -1 )
	{
		strcpy(v_szEndTime, "00:00:00");
		
		switch(v_iWorkStyle)
		{
			case WORK_REC:
			{
				v_objWorkEvent->dwRecStopEvt = 0;
			}
				break;
			case WORK_UPL:
			{
				v_objWorkEvent->dwUplStopEvt = 0;
			}
				break;
			case WORK_PIC:
			{
				v_objWorkEvent->dwPicStopEvt = 0;
			}
				break;
			default:
				break;
		}
	}
}

void DetectEvent( BOOL *v_bPeriodIgnore, SLEEP_EVENT *v_pSleepEvent, WORK_EVENT *v_pWorkEvent, CTRL_EVENT *v_pCtrlEvent )
{
	int i;
	uint uiStatus;
	int iResult;
	char szBuf[2048] = {0};
	BYTE  bytLvl;
	DWORD dwLen;
	DWORD dwPushTm = 0;
	
	BYTE ucIOState;

	IRKEY_INFO_T 	objIrda = {0};
	
	//���ʱ�δ����¼�
	if(   (g_objMvrCfg.PeriodTrig.PeriodStart[0] && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.StartTime[0]) > 0 && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.EndTime[0]) < 0) 
			||(g_objMvrCfg.PeriodTrig.PeriodStart[1] && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.StartTime[1]) > 0 && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.EndTime[1]) < 0) 
			||(g_objMvrCfg.PeriodTrig.PeriodStart[2] && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.StartTime[2]) > 0 && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.EndTime[2]) < 0) 
			||(g_objMvrCfg.PeriodTrig.PeriodStart[3] && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.StartTime[3]) > 0 && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.EndTime[3]) < 0) 
			||(g_objMvrCfg.PeriodTrig.PeriodStart[4] && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.StartTime[4]) > 0 && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.EndTime[4]) < 0) 
			||(g_objMvrCfg.PeriodTrig.PeriodStart[5] && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.StartTime[5]) > 0 && strcmp(szCurTime, g_objMvrCfg.PeriodTrig.EndTime[5]) < 0) )
	{
		switch(g_enumSysSta)
		{
			case SLEEP:
			{
				v_pSleepEvent->dwStopEvt |= EVENT_TYPE_PERIOD;
			}
				break;
			case IDLE:
			case WORK:
			{
				v_pWorkEvent->dwRecStartEvt |= EVENT_TYPE_PERIOD;
			}
				break;
			default:
				break;
		}
	}
	
	//���Acc�����¼�
	IOGet( IOS_ACC, &ucIOState );
	if( ucIOState == IO_ACC_ON )
	{
		if( g_objMvrCfg.EventTrig.AccStart )
		{
			switch(g_enumSysSta)
			{
				case SLEEP:
				{
					v_pSleepEvent->dwStopEvt |= EVENT_TYPE_ACCON;
				}
				case IDLE:
				case WORK:
				{
					v_pWorkEvent->dwRecStartEvt |= EVENT_TYPE_ACCON;
				}
					break;
				default:
					break;
			}
		}
	}

	//����ƶ����ڵ������¼�
#if VEHICLE_TYPE == VEHICLE_M
	ioctl(g_objDevFd.Tw2835, TW2835_IRQ_STATUS_READ, &uiStatus);
	for(i = 0; i < 4; i++)
	{
		if(((uiStatus >> 24) & (0x08 >> i)) > 0)//�������ͷ״̬
			g_objDevSta.Camera[i] = FALSE;
		else
			g_objDevSta.Camera[i] = TRUE;
	}
	if(((uiStatus >> 16) & 0x0f) > 0)//��⵽�ƶ��ź�
	{
		if( g_objMvrCfg.EventTrig.MoveStart )
		{
			switch(g_enumSysSta)
			{
				case SLEEP:
				{
					v_pSleepEvent->dwStopEvt |= EVENT_TYPE_MOVE;
				}
				case IDLE:
				case WORK:
				{
					v_pWorkEvent->dwRecStartEvt |= EVENT_TYPE_MOVE;
				}
					break;
				default:
					break;
			}
		}
	}
	if(((uiStatus >> 8) & 0x0f) > 0)//��⵽�ڵ��ź�
	{
		if(g_objMvrCfg.EventTrig.OverlayStart)
		{
			switch(g_enumSysSta)
			{
				case SLEEP:
				{
					v_pSleepEvent->dwStopEvt |= EVENT_TYPE_OVERLAY;
				}
				case IDLE:
				case WORK:
				{
					v_pWorkEvent->dwRecStartEvt |= EVENT_TYPE_OVERLAY;
				}
					break;
				default:
					break;
			}
		}
	}
#endif
	
	//��ⱨ�������¼�
	iResult = g_objIoMng.PopData(bytLvl, sizeof(szBuf), dwLen, szBuf, dwPushTm);
	if( !iResult && dwLen == 1 )
	{
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		IOGet( IOS_POWDETM, &ucIOState );
#endif

		if(szBuf[0] == IO_ALARM_ON)
		{
			if( g_objMvrCfg.EventTrig.S1Start )
			{
				switch(g_enumSysSta)
				{
					case SLEEP:
					{
						v_pSleepEvent->dwStopEvt |= EVENT_TYPE_ALERT;
					}
					case IDLE:
					case WORK:
					{
						v_pWorkEvent->dwRecStartEvt |= EVENT_TYPE_ALERT;
					}
						break;
					default:
						break;
				}
			}
		}
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		else if((szBuf[0] == IO_POWDETM_INVALID) || (szBuf[0] == IO_QIANYA_VALID && ucIOState == IO_POWDETM_INVALID))
		{
			switch(g_enumSysSta)
			{
				case IDLE:
				{
					v_pSleepEvent->dwStartEvt |= EVENT_TYPE_POWOFF;
				}
					break;
				case CTRL:
				{
					v_pCtrlEvent->dwStopEvt |= EVENT_TYPE_POWOFF;
					v_pSleepEvent->dwStartEvt |= EVENT_TYPE_POWOFF;
				}
					break;
				case WORK:
				{
					*v_bPeriodIgnore = TRUE;
					v_pWorkEvent->dwRecStopEvt |= EVENT_TYPE_POWOFF;
					v_pWorkEvent->dwUplStopEvt |= EVENT_TYPE_POWOFF;
					v_pSleepEvent->dwStartEvt |= EVENT_TYPE_POWOFF;
				}
					break;
				default:
					break;
			}
		}
#endif
	}
	
	//�������ָ����¼�
	iResult = g_objQianMng.PopData( bytLvl, sizeof(szBuf), dwLen, szBuf, dwPushTm, NULL, NULL, 0 );
	if( !iResult && dwLen <= sizeof(szBuf) )
	{
		DealQianSmsData(szBuf, (int)dwLen, v_pSleepEvent, v_pWorkEvent, v_pCtrlEvent);
	}
	
	//���ң��ָ����¼�
// 	iResult = g_objQianIrdaMng.PopData( bytLvl, sizeof(szBuf), dwLen, szBuf, dwPushTm, NULL, NULL, 0 );
// 	if( !iResult && dwLen == 2 )
// 	{
// 		DealIrdaKey((BYTE)szBuf[0], (BYTE)szBuf[1], v_bPeriodIgnore, v_pSleepEvent, v_pWorkEvent, v_pCtrlEvent);
// 	}
	iResult = read(g_objDevFd.Irda, &objIrda, sizeof(IRKEY_INFO_T));
	if(iResult > 0)
	{
		DealIrdaKey(objIrda.irkey.sys_id_code, objIrda.irkey.irkey_code, v_bPeriodIgnore, v_pSleepEvent, v_pWorkEvent, v_pCtrlEvent);
	}
	
	//������մ����¼�
	iResult = g_objQianPicMng.PopData( bytLvl, sizeof(szBuf), dwLen, szBuf, dwPushTm, NULL, NULL, 0 );
	if( !iResult && dwLen>0 && dwLen <= sizeof(szBuf) )
	{
		DealPicEvent((tagPhotoInfo*)szBuf, v_pSleepEvent, v_pWorkEvent, v_pCtrlEvent);
	}
	
	//���TTS�����¼�
	iResult = g_objQianTTSMng.PopData( bytLvl, sizeof(szBuf), dwLen, szBuf, dwPushTm, NULL, NULL, 0 );
	if( !iResult && dwLen>0 && dwLen<=sizeof(szBuf) )
	{
		PRTMSG(MSG_DBG, "recv tts data:%s, dwLen = %d\n", szBuf, dwLen);

		g_objWorkStop.TTS = FALSE;
		
		//��ֹ�����ϳ�ʱ��������������忴�Ź�
		ClearThreadDog(0);
		MakeTTS(szBuf); // �ϳ�TTS����
		ClearThreadDog(0);
		
		switch(g_enumSysSta)
		{
			case SLEEP:
			{
				v_pSleepEvent->dwStopEvt |= EVENT_TYPE_CENT;
			}
			case IDLE:
			case WORK:
			{
				v_pWorkEvent->dwTtsStartEvt |= EVENT_TYPE_CENT;
			}
				break;
			default:
				break;
		}
	}
	
	//���Tw2865�쳣�����¼�
	iResult = g_objDevErrMng.PopData(bytLvl, sizeof(szBuf), dwLen, szBuf, dwPushTm);
	if( !iResult && dwLen == 1 )
	{
		if(szBuf[0] == 0x01)
		{
			switch(g_enumSysSta)
			{
				case IDLE:
				{
					v_pWorkEvent->dwRecStartEvt = 0;
					v_pWorkEvent->dwUplStartEvt = 0;
					v_pWorkEvent->dwPicStartEvt = 0;
					v_pWorkEvent->dwTtsStartEvt = 0;
					v_pWorkEvent->dwBlkStartEvt = 0;

					v_pSleepEvent->dwStartEvt |= EVENT_TYPE_ERROR;
				}
					break;
				case CTRL:
				{
					v_pCtrlEvent->dwStopEvt |= EVENT_TYPE_ERROR;
					v_pSleepEvent->dwStartEvt |= EVENT_TYPE_ERROR;
				}
					break;
				case WORK:
				{
					v_pWorkEvent->dwRecStartEvt = 0;
					v_pWorkEvent->dwUplStartEvt = 0;
					v_pWorkEvent->dwPicStartEvt = 0;
					v_pWorkEvent->dwTtsStartEvt = 0;
					v_pWorkEvent->dwBlkStartEvt = 0;

					*v_bPeriodIgnore = TRUE;
					v_pWorkEvent->dwRecStopEvt |= EVENT_TYPE_ERROR;
					v_pWorkEvent->dwUplStopEvt |= EVENT_TYPE_ERROR;
					v_pWorkEvent->dwPicStopEvt |= EVENT_TYPE_ERROR;
					v_pWorkEvent->dwTtsStopEvt |= EVENT_TYPE_ERROR;
					v_pWorkEvent->dwBlkStopEvt |= EVENT_TYPE_ERROR;
					v_pSleepEvent->dwStartEvt |= EVENT_TYPE_ERROR;
				}
					break;
				default:
					break;
			}
		}
	}
	
	//�������쳣�����¼�
	if((strcmp("SDisk", g_objMvrCfg.AVRecord.DiskType) == 0 && (g_objUsbSta.SDisk[0] == UCON_UMOU_UVAL || g_objUsbSta.SDisk[0] == CON_UMOU_UVAL || g_objUsbSta.SDisk[1] == UCON_UMOU_UVAL || g_objUsbSta.SDisk[1] == CON_UMOU_UVAL)) 
	 ||(strcmp("HDisk", g_objMvrCfg.AVRecord.DiskType) == 0 && (g_objUsbSta.HDisk[0] == UCON_UMOU_UVAL || g_objUsbSta.HDisk[0] == CON_UMOU_UVAL || g_objUsbSta.HDisk[1] == UCON_UMOU_UVAL || g_objUsbSta.HDisk[1] == CON_UMOU_UVAL)))
	{
		switch(g_enumSysSta)
		{
			case WORK:
			{
				*v_bPeriodIgnore = TRUE;//����ʱ�δ���
				v_pWorkEvent->dwRecStopEvt |= EVENT_TYPE_ERROR;
				v_pWorkEvent->dwPicStopEvt |= EVENT_TYPE_ERROR;
			}
				break;
			default:
				break;
		}
	}
	
	/*��⵽��ʱ, ����ֹͣ¼���¼�*/
	if( (g_objWorkStart.VRecord[0] || g_objWorkStart.VRecord[1] || g_objWorkStart.VRecord[2] || g_objWorkStart.VRecord[3]) 
		&&(strcmp(szCurTime, szRecEndTime)>0 || strcmp(szCurTime, "23:59:59")==0) )
	{
		if( g_iTempState!=1 )//��������ͷ����ʱ
		{
			PRTMSG(MSG_DBG, "timeout stop record, szCurTime:%s, szRecEndTime:%s\n", szCurTime, szRecEndTime);
			v_pWorkEvent->dwRecStopEvt |= EVENT_TYPE_TIMEOUT;
		}
	}
	
	/*��⵽��ʱ, ����ֹͣ����¼�*/
	if(	(g_objWorkStart.VUpload[0] || g_objWorkStart.VUpload[1] || g_objWorkStart.VUpload[2] || g_objWorkStart.VUpload[3] ) 
		&&(strcmp(szCurTime, szUplEndTime) > 0))// || strcmp(szCurTime, "23:59:59") == 0))
	{
		PRTMSG(MSG_DBG, "timeout stop upload, szCurTime:%s, szUplEndTime:%s\n", szCurTime, szUplEndTime);
		v_pWorkEvent->dwUplStopEvt |= EVENT_TYPE_TIMEOUT;
	}
}

void SelectEvent( BOOL *v_bPeriodIgnore, SLEEP_EVENT *v_pSleepEvent, WORK_EVENT *v_pWorkEvent, CTRL_EVENT *v_pCtrlEvent )
{
	if((strcmp("SDisk", g_objMvrCfg.AVRecord.DiskType) == 0 && (g_objUsbSta.SDisk[0] == UCON_UMOU_UVAL || g_objUsbSta.SDisk[0] == CON_UMOU_UVAL || g_objUsbSta.SDisk[1] == UCON_UMOU_UVAL || g_objUsbSta.SDisk[1] == CON_UMOU_UVAL)) 
	 ||(strcmp("HDisk", g_objMvrCfg.AVRecord.DiskType) == 0 && (g_objUsbSta.HDisk[0] == UCON_UMOU_UVAL || g_objUsbSta.HDisk[0] == CON_UMOU_UVAL || g_objUsbSta.HDisk[1] == UCON_UMOU_UVAL || g_objUsbSta.HDisk[1] == CON_UMOU_UVAL)))
	{
		switch(g_enumSysSta)
		{
			case IDLE:
			case WORK:
			{
				v_pWorkEvent->dwRecStartEvt = 0;
				v_pWorkEvent->dwPicStartEvt = 0;
				g_objPhoto.m_objDataMngToShot.Release();
			}
				break;
			default:
				break;
		}
	}
	
	switch( g_objMvrCfg.TrigPara.TrigType )
	{
		case TRIG_PER:/*ʱ�δ���*/
		{
			v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ACCON;
			v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_MOVE;
			v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_OVERLAY;
			v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ALERT;
			v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ALERT2;
			v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ALERT3;
			v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ALERT4;
								
			if( *v_bPeriodIgnore ) 
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_PERIOD;
			v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ACCON;
			v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_MOVE;
			v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_OVERLAY;
			v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT;
			v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT2;
			v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT3;
			v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT4;
		}
			break;
			
		case TRIG_EVE:/*�¼�����*/
		{
			v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_PERIOD;	
			v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_PERIOD;
		}
			break;
			
		case TRIG_PER_AND_EVE:/*ʱ�κ��¼�����*/
		{
			if( !(v_pSleepEvent->dwStopEvt & EVENT_TYPE_PERIOD) )
			{
				v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ACCON;
				v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_MOVE;
				v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_OVERLAY;
				v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ALERT;
				v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ALERT2;
				v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ALERT3;
				v_pSleepEvent->dwStopEvt &= ~EVENT_TYPE_ALERT4;
			}
								
			if( !(v_pWorkEvent->dwRecStartEvt & EVENT_TYPE_PERIOD) )
			{
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ACCON;
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_MOVE;
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_OVERLAY;
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT;
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT2;
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT3;
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_ALERT4;
			}
		}
			break;
			
		case TRIG_PER_OR_EVE:/*ʱ�λ��¼�����*/
		{
			if( *v_bPeriodIgnore ) 
				v_pWorkEvent->dwRecStartEvt &= ~EVENT_TYPE_PERIOD;
		}
			break;
			
		default:
			break;
	}
}

void Init( int v_iSysState, SLEEP_EVENT *v_objSleepEvent, CTRL_EVENT *v_objCtrlEvent, WORK_EVENT *v_objWorkEvent )
{
	switch(v_iSysState)
	{
		case SLEEP:
		{
			StopVPrev();
			StopVo();
			StopVdec();
			StopAPrev();
			StopAo();
			StopAdec();
			StopVi();
			StopVencChn();
			StopVenc();
			StopAi();
			StopAencChn();
			StopAenc();
			MPP_Exit();
			
			//UmountDisk(SDISK, 3000);
			
#if VEHICLE_TYPE == VEHICLE_M
			IOSET( IOS_VIDICONPOW, IO_VIDICONPOW_OFF );//�ر�����ͷ��Դ	
			IOSET( IOS_HUBPOW, IO_HUBPOW_ON );//����HUB/SD��Դ
			//IOSET( IOS_HARDPOW, IO_HARDPOW_OFF );//�ر�Ӳ�̵�Դ
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
			IOSET( IOS_VIDICONPOW, IO_VIDICONPOW_OFF );//�ر�����ͷ��Դ	
#if VEHICLE_TYPE == VEHICLE_V8
			IOSET(IOS_USBPOW, IO_USBPOW_OFF);
#endif
#if VEHICLE_TYPE == VEHICLE_M2
			//IOSET(IOS_HUBPOW, IO_HUBPOW_OFF);
			IOSET(IOS_HARDPOW, IO_HARDPOW_OFF);
#endif
			IOSET(IOS_LCDPOW, IO_LCDPOW_OFF);
			IOSET(IOS_TRUMPPOW, IO_TRUMPPOW_OFF);
			IOSET(IOS_SCHEDULEPOW, IO_SCHEDULEPOW_OFF);
			sleep(2);
			close(g_objDevFd.Tw2865);
			RMMOD(KO_YX3511_TW2865);
			RMMOD(KO_HI3511_USB);
#endif

			v_objSleepEvent->dwStartEvt = 0;
			v_objCtrlEvent->dwStartEvt = 0;
			v_objWorkEvent->dwRecStartEvt = 0;
			v_objWorkEvent->dwUplStartEvt = 0;
			v_objWorkEvent->dwPicStartEvt = 0;
			v_objWorkEvent->dwTtsStartEvt = 0;
			v_objWorkEvent->dwBlkStartEvt = 0;

			//SetCpuSlow();/*cpu����slowģʽ*/
			SetClkDisable();/*�ر�ʱ��*/	
		}
			break;
		case IDLE:
		{
			MPP_Exit();
			MPP_Init();
	
			StartAenc();
			StartAi();
			StartAencChn();
 			StartVenc();
 			StartVi();
 			StartVencChn();
			StartAdec();
			StartAo();
 			StartVdec();
 			StartVo();

 			StartVPrev();
		}
			break;
		case CTRL:
		{
			IOSET( IOS_TRUMPPOW, IO_TRUMPPOW_ON );//�������ŵ�Դ
			IOSET( IOS_EARPHONESEL, IO_EARPHONESEL_OFF );//ѡ������ͨ��
			IOSET( IOS_AUDIOSEL, IO_AUDIOSEL_PC);//ѡ��PC��Ƶ
								
			StopVPrev();
			StopVo();
			StartVo();	
			DealKeyLogin();
								
			v_objSleepEvent->dwStartEvt = 0;
			v_objCtrlEvent->dwStartEvt = 0;
			v_objWorkEvent->dwRecStartEvt = 0;
			v_objWorkEvent->dwUplStartEvt = 0;
			v_objWorkEvent->dwPicStartEvt = 0;
			v_objWorkEvent->dwTtsStartEvt = 0;
			v_objWorkEvent->dwBlkStartEvt = 0;
		}
			break;
		case WORK:
		{
			v_objSleepEvent->dwStartEvt = 0;
			v_objCtrlEvent->dwStartEvt = 0;
		}
			break;
		default:
			break;
	}
}

void Release( int v_iSysState, SLEEP_EVENT *v_objSleepEvent, CTRL_EVENT *v_objCtrlEvent, WORK_EVENT *v_objWorkEvent )
{
	switch(v_iSysState)
	{
		case SLEEP:
		{
//			SetCpuNormal();/*cpu����normalģʽ*/
			SetClkEnable();/*����ʱ��*/
			
#if VEHICLE_TYPE == VEHICLE_M
			IOSET(IOS_VIDICONPOW, IO_VIDICONPOW_ON, 100);//��������ͷ��Դ
			IOSET(IOS_TW2835RST, IO_TW2835RST_LO, 100);//��λTW2835
			IOSET(IOS_TW2835RST, IO_TW2835RST_HI, 100);
			IOSET(IOS_HUBPOW, IO_HUBPOW_ON);//����HUB/SD��Դ
			IOSET(IOS_HARDPOW, IO_HARDPOW_ON);//����Ӳ�̵�Դ
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
			IOSET(IOS_VIDICONPOW, IO_VIDICONPOW_ON, 100);//��������ͷ��Դ
			IOSET(IOS_TW2865RST, IO_TW2865RST_LO, 100);//��λTW2865
			IOSET(IOS_TW2865RST, IO_TW2865RST_HI, 100);
#if VEHICLE_TYPE == VEHICLE_V8
			IOSET(IOS_USBPOW, IO_USBPOW_ON, 100);
#endif
#if VEHICLE_TYPE == VEHICLE_M2
			IOSET(IOS_HUBPOW, IO_HUBPOW_ON, 100);
			IOSET(IOS_HARDPOW, IO_HARDPOW_ON, 100);
#endif
			IOSET(IOS_LCDPOW, IO_LCDPOW_ON);
			IOSET(IOS_SCHEDULEPOW, IO_SCHEDULEPOW_ON);
			sleep(2);
			INSMOD(KO_HI3511_USB);
			INSMOD(KO_YX3511_TW2865, 100);
			g_objDevFd.Tw2865 = open("/dev/misc/tw2865adev", O_RDWR);
#endif

			//MountDisk(SDISK);
			
			MPP_Init();
			StartAenc();
			StartAi();
			StartAencChn();
			StartVenc();
			StartVi();
			StartVencChn();
			StartAdec();
			StartAo();
			StartVdec();
			StartVo();	
			StartVPrev();
			
			/*�������ʡ��״̬����¼�*/
			v_objSleepEvent->dwStopEvt	= 0;
		}
			break;
		case IDLE:
		{
			StopVPrev();
			StopVo();
			StopVdec();
			StopAPrev();
			StopAo();
			StopAdec();
			StopVi();
			StopVencChn();
			StopVenc();
			StopAi();
			StopAencChn();
			StopAenc();
						
			UmountDisk(SDISK);
			UmountDisk(HDISK);
			UmountDisk(UDISK);
		}
			break;
		case CTRL:
		{
			DealKeyLock();
						
			StopAVPlay();
			StopVo();
			StopVdec();
			StopVi();
			StopVencChn();
			StopVenc();
			sleep(1);
			StartVenc();
			StartVencChn();
			StartVi();
			StartVdec();
			StartVo();	
		  StartVPrev();
								
			IOSET(IOS_TRUMPPOW, IO_TRUMPPOW_OFF);//�رչ��ŵ�Դ
			IOSET(IOS_EARPHONESEL, IO_EARPHONESEL_ON);//ѡ�����ͨ��
			IOSET(IOS_AUDIOSEL, IO_AUDIOSEL_PHONE);//ѡ���ֻ���Ƶ
								
			/*������в���״̬����¼�*/
			v_objCtrlEvent->dwStopEvt = 0;
		}
			break;
		case WORK:
		{
			/*������й���״̬����¼�*/
			v_objWorkEvent->dwRecStopEvt = 0;
			v_objWorkEvent->dwUplStopEvt = 0;
			v_objWorkEvent->dwPicStopEvt = 0;
			v_objWorkEvent->dwTtsStopEvt = 0;
			v_objWorkEvent->dwBlkStartEvt = 0;
		}
			break;
		default:
			break;
	}
}

void *G_CmdReceive(void *arg)
{
	int   iResult;
	char  szBuf[2048] = {0};
	BYTE  bytLvl;
	DWORD dwPopLen;
	DWORD dwPushSymb;
	DWORD dwPacketNum = 0;
	
	int iTempState;
	char buf[256] = {0};
	buf[0] = 0x01;
	buf[1] = 0x72;
	buf[2] = 0x01;

	iResult = DataWaitPop(DEV_DVR);
	while( !iResult )
	{
		if( g_bProgExit )  
		{ 
			break; 
		} 
		
		memset(szBuf, 0, sizeof(szBuf));
		
		while( !DataPop( szBuf, sizeof(szBuf), &dwPopLen, &dwPushSymb, DEV_DVR, &bytLvl ) )
		{
			switch(dwPushSymb)
			{
				case DEV_IO:
					{
						switch(szBuf[0])
						{
							case IO_ALARM_ON:
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
							case IO_QIANYA_VALID:
							case IO_POWDETM_INVALID:
#endif
							{
								g_objIoMng.PushData( 1, 1, szBuf, dwPacketNum);
							}
								break;
							default:
								break;
						}
					}
					break;
					
				case DEV_SOCK:
					{
						g_objSockMng.PushData( bytLvl, dwPopLen, szBuf, dwPacketNum);
					}
					break;
					
				case DEV_QIAN:
					{
						switch( szBuf[0] )
						{
							case 0x01:		// TTS��������
							{
								if( szBuf[1] == 0x01 )	// ������ֹ��ǰTTS����
								{
									StopTTS();
									g_objQianTTSMng.Release();
								}

								g_objQianTTSMng.PushData( bytLvl, dwPopLen-2, szBuf+2, dwPacketNum );
							}
								break;
							case 0x02:		// ǧ��������ָ��
							{
								g_objQianMng.PushData( bytLvl, dwPopLen-1, szBuf+1, dwPacketNum );
							}
								break;
							case 0x03:		// ����ң�ذ�������
							{
								g_objQianIrdaMng.PushData( bytLvl, dwPopLen-1, szBuf+1, dwPacketNum );
							}	
								break;
							case 0x04:		// �����¼�����
							{
								g_objQianPicMng.PushData( bytLvl, dwPopLen-1, szBuf+1, dwPacketNum );
							}	
								break;
							default:
								break;
						}
					}
					break;

				case DEV_DIAODU:
					{
						buf[3] = '\0';

						switch( szBuf[0] )
						{
							case 0x01:	// ϵͳ��������
							{
								SetSysVolume( true, szBuf[1] );
							}
								break;

							case 0x02:  // ���ڡ�SD����Ӳ�̡�U�̼��
							{
#if VEHICLE_TYPE == VEHICLE_M
								ShowDiaodu("SD��:%s;Ӳ��:%s;U��:%s", DiskTest("SDisk")==0 ? "��Ч" : "��Ч", DiskTest("HDisk")==0 ? "��Ч" : "��Ч", DiskTest("UDisk")==0 ? "��Ч" : "��Ч");
#endif
#if VEHICLE_TYPE == VEHICLE_V8
								ShowDiaodu("����1:%s;����2:%s;SD��:%s;U��:%s", 
														TtyTest("/dev/ttyAMA0")==0 ? "��Ч" : "��Ч", TtyTest("/dev/ttySIM1")==0 ? "��Ч" : "��Ч",
														DiskTest("SDisk")==0 ? "��Ч" : "��Ч", DiskTest("UDisk")==0 ? "��Ч" : "��Ч");
#endif
#if VEHICLE_TYPE == VEHICLE_M2
								ShowDiaodu("����1:%s;����2:%s;SD��:%s;U��:%s;Ӳ��:%s", 
									TtyTest("/dev/ttyAMA0")==0 ? "��Ч" : "��Ч", TtyTest("/dev/ttySIM1")==0 ? "��Ч" : "��Ч",
									DiskTest("SDisk")==0 ? "��Ч" : "��Ч", DiskTest("UDisk")==0 ? "��Ч" : "��Ч", DiskTest("HDisk")==0 ? "��Ч" : "��Ч");
#endif
							}
								break;

							case 0x03://ϵͳ������Ӧ������ʱ���ֿ���״̬
							{
								iTempState = g_iTempState;
								g_iTempState = 1;
							}
								break;

							case 0x04: // PC��Ƶ���
							{
								iTempState = g_iTempState;
								g_iTempState = 1;
								if( WaitForEvent(IDLESTATE, &g_enumSysSta, 10000)==0 )
								{
									PRTMSG(MSG_NOR, "Start TTS Test\n");
									TTSTest("����TTS��������");
								}
								g_iTempState = iTempState;
							}
								break;

							case 0x05: // ¼��¼����
							{
								iTempState = g_iTempState;
								g_iTempState = 1;
								if( WaitForEvent(IDLESTATE, &g_enumSysSta, 10000)==0 )
								{
									PRTMSG(MSG_NOR, "Start AV Test\n");
									AVTest((int)szBuf[1]);
								}
								g_iTempState = iTempState;
							}
								break;

							case 0x06://��ʽ��SD����Ӳ��
							{
								sleep(1);
								iTempState = g_iTempState;

								if(strcmp("SDisk", g_objMvrCfg.AVRecord.DiskType) == 0)
								{
									g_iTempState = 2;
									sleep(5);
									ShowDiaodu("��ʽ��SD��%s", (WaitForEvent(IDLESTATE, &g_enumSysSta, 10000)==0 && DiskFormat("SDisk")==0) ? "�ɹ�" : "ʧ��");
								}
								else if(strcmp("HDisk", g_objMvrCfg.AVRecord.DiskType) == 0)
								{
									g_iTempState = 3;
									sleep(5);
									ShowDiaodu("��ʽ��Ӳ��%s", (WaitForEvent(IDLESTATE, &g_enumSysSta, 10000)==0 && DiskFormat("HDisk")==0) ? "�ɹ�" : "ʧ��");
								}

								g_iTempState = iTempState;
							}
								break;

							case 0x07:		//������SD����Ӳ��״̬
							{
								struct statfs objDiskBuf;
								double dDiskUsage;
								char szTempBuf[100] = {0};
								
								if(strcmp("SDisk", g_objMvrCfg.AVRecord.DiskType) == 0)
								{
									strcat(buf, "SD����");

									for(int i=0; i<2; i++)
									{
										if( MOUNT(&g_objUsbSta.SDisk[i]) )
										{
											memset(&objDiskBuf, 0, sizeof(objDiskBuf));
											statfs(SPRINTF(szTempBuf, "/mnt/SDisk/part%d", i+1), &objDiskBuf);
											dDiskUsage = 1 - objDiskBuf.f_bavail/(double)objDiskBuf.f_blocks;
												
											memset(szTempBuf, 0, sizeof(szTempBuf));
											sprintf(szTempBuf, "����%dʹ����%.3f%% ", i+1, 100*dDiskUsage);
											strcat(buf, szTempBuf);	
										}
										else
										{
											memset(szTempBuf, 0, sizeof(szTempBuf));
											sprintf(szTempBuf, "����%dδ���� ", i+1);
											strcat(buf, szTempBuf);		
										}
									}
								}
								else if(strcmp("HDisk", g_objMvrCfg.AVRecord.DiskType) == 0)
								{
									strcat(buf, "Ӳ�̣�");

									for(int i=0; i<2; i++)
									{
										if( MOUNT(&g_objUsbSta.HDisk[i]) )
										{
											memset(&objDiskBuf, 0, sizeof(objDiskBuf));
											statfs(SPRINTF(szTempBuf, "/mnt/HDisk/part%d", i+1), &objDiskBuf);
											dDiskUsage = 1 - objDiskBuf.f_bavail/(double)objDiskBuf.f_blocks;
												
											memset(szTempBuf, 0, sizeof(szTempBuf));
											sprintf(szTempBuf, "����%dʹ����%.3f%% ", i+1, 100*dDiskUsage);
											strcat(buf, szTempBuf);
										}
										else
										{
											memset(szTempBuf, 0, sizeof(szTempBuf));
											sprintf(szTempBuf, "����%dδ���� ", i+1);
											strcat(buf, szTempBuf);
										}
									}
								}
								
								DataPush(buf, strlen(buf), DEV_DVR, DEV_DIAODU, LV2);									
							}
								break;

							case 0x08://�˳�����״̬
							{
								g_iTempState = iTempState;
							}
								break;
								
							case 0x09://��ʼ������
							{
								g_bIrTest = true;
							}
								break;
							
							case 0x0a://ֹͣ������
							{
								g_bIrTest = false;
							}
								break;
								
							case 0x0b://ComuExe֪ͨ�ѻָ���������
							{
								iTempState = g_iTempState;
								g_iTempState = 1;
								if( WaitForEvent(IDLESTATE, &g_enumSysSta, 20000)==0 )
								{
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
									StopVPrev();
									StopVo();
									StopVdec();
									StopVi();
									StopVencChn();
									StopVenc();
									sleep(1);
									StartVenc();
									StartVencChn();
									StartVi();
									StartVdec();
									StartVo();	
								  StartVPrev();
								}
								g_iTempState = iTempState;
							}
								break;
							case 0x0c://ComuExe֪ͨ�ֻ��š�APN��IP���˿�����
							{
								iTempState = g_iTempState;
								g_iTempState = 1;
								if( WaitForEvent(IDLESTATE, &g_enumSysSta, 20000)==0 )
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
								g_iTempState = iTempState;
								
								//����Ƶ�ϴ��߳����¶�ȡ�ֻ���
								GetImpCfg( &g_objComuCfg, sizeof(g_objComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(g_objComuCfg) );
							}
								break;
							case 0x0d://�л�Ԥ��ͨ��
							{
								tag2DVOPar objVOutput;
								
								SwitchVPrev(g_bytViChn, g_bytVoChn, -1);
								StopVPrev();
						
								g_bytViChn = szBuf[1];
								if(g_bytViChn == 0x0a)	// �Ƿ�����ȫͨ��Ԥ��
								{
									g_bytVoChn = 0x0a;
									
									objVOutput.VMode = CIF;
									SwitchVo(objVOutput);
									StartVPrev();
								}
								else
								{
									g_bytVoChn = 0x00;
									
									objVOutput.VMode = D1;
									SwitchVo(objVOutput);
									SwitchVPrev(g_bytViChn, g_bytVoChn, 1);
								}
							}
								break;
							case 0x0e:	//������ƵԤ���л��ӿ�
							{
								CtrlVideoView(szBuf[1]);
							}
								break;

							case 0x99:	// ������
								{
									// 3809��ѯʱ�������: ͨ���ţ�1��+ ��ʼ���ڣ�3��
									//char buf[4] = {0x01, 0x0b, 0x01, 0x1a};
									//char buf = 0x01;
									//Deal3809(&buf, 1);
									
// 									// 380a��ϻ���ϴ�����ͨ���ţ�1��+ �¼����ͣ�1��+ ��ʼ���ڣ�3��+ ��ʼʱ�䣨3��+ �������ڣ�3�� + ����ʱ�䣨3��
// 									char buf[] = {0x04, 0x00, 0x0b, 0x01, 0x1a, 0x07, 0x0a, 0x00, 0x0b, 0x01, 0x1a, 0x07, 0x0b, 0x00};
// 									Deal380A(buf, sizeof(buf));
								}
// 
// 							case 0x9a:	// ������
// 								{
// 									// D1Ӧ���ļ����ͣ�1���� �ļ�����40���� ʱ����ţ�6���� Ӧ�����ͣ�1��
// 									char buf[] = {0x03, 0x30, 0x5f, 0x32, 0x30, 0x31, 0x31, 0x2d, 0x30, 0x31, 0x2d, 0x32, 0x36, 0x5f, 0x30, 0x37, 0x2d, 0x31, 0x30, 0x2d, 0x30, 0x30, 0x5f, 0x30, 0x37, 0x2d, 0x31, 0x31, 0x2d, 0x30, 0x30, 0x2e, 0x69, 0x6e, 0x64, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x0b, 0x01, 0x1a, 0x0d, 0x33, 0x01};
// 									Deal38D1(buf, sizeof(buf));
// 								}
// 								break;
// 
// 							case 0x9b:	// ������
// 								{
// 									// D3Ӧ���ļ����ͣ�1���� �ļ�����40���� Ӧ�����ͣ�1��+ �������ش��İ�����(8) + ����ţ�8��* M��
// 									char buf[] = {0x03, 0x30, 0x5f, 0x32, 0x30, 0x31, 0x31, 0x2d, 0x30, 0x31, 0x2d, 0x32, 0x36, 0x5f, 0x30, 0x37, 0x2d, 0x31, 0x30, 0x2d, 0x30, 0x30, 0x5f, 0x30, 0x37, 0x2d, 0x31, 0x31, 0x2d, 0x30, 0x30, 0x2e, 0x69, 0x6e, 0x64, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01};
// 									Deal38D3(buf, sizeof(buf));
// 								}
// 								break;
								
							default:
								break;
						}
					}
					break;

				default:
					break;
			}
		}
		
		iResult = DataWaitPop(DEV_DVR);
	}

	// �����߳��˳��������������˳�
	g_bProgExit = true;
}

void *G_SysSwitch(void *arg)
{
	BOOL bTimeRegionOn = FALSE;
	BOOL bChnRegionOn[4] = { FALSE };
	OSD_REGION objTimeRegion;
	OSD_REGION objChnRegion[4];

	BOOL bPeriodIgnore = FALSE;
	
	SLEEP_EVENT objSleepEvent = {FALSE};
	CTRL_EVENT objCtrlEvent  = {FALSE};
	WORK_EVENT objWorkEvent  = {FALSE};

	int iCurDateTime[6];  
	int lLastSec, lCurSec;

	bool bNewIdle = true;		// �Ƿ����½������״̬	
	DWORD dwIdleBeginTm = GetTickCount();	// ϵͳ�������״̬�Ŀ�ʼʱ��

	/*�����������״̬*/
	Init( IDLE, NULL, NULL, NULL );
 	
 	g_objPhoto.Init();

	lLastSec = lCurSec = GetCurDateTime( iCurDateTime );
	
	FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, 1 );
	FlushSysState( objChnRegion, bChnRegionOn, 1 );
	
	RenewIdleTime( iCurDateTime, szIdleEndTime, 0 );
	
	sprintf( szCurTime, "%02d:%02d:%02d", iCurDateTime[3], iCurDateTime[4], iCurDateTime[5] );
	
	while(1)
	{
		RenewMemInfoHalfMin(0);
		ClearThreadDog(0);
		
		DetectEvent( &bPeriodIgnore, &objSleepEvent, &objWorkEvent, &objCtrlEvent );//��ⴥ���¼�
		SelectEvent( &bPeriodIgnore, &objSleepEvent, &objWorkEvent, &objCtrlEvent );//���ݴ�������ѡ�񴥷��¼�
/*----------------------------------------------------------------------------------------------------------------*/
		//��Ƶ��ϻ�Ӳ�ѯʱ��������ֹͣ¼��ʹ��д���ļ��б�
		if(g_bDeal380A)
		{
			StopAVRec();
			sleep(3);
			Deal380A(g_szDeal380A, g_iDeal380A);
		}
		
		//���ݴ����¼�ת��ϵͳ״̬
		switch( g_enumSysSta )
		{
			case SLEEP:/*ʡ��״̬�¼����*/
				{
					/*��⵽ֹͣʡ��״̬�¼�, ����ʡ��״̬�������״̬*/
					if(	g_iTempState || g_bProgExit || objSleepEvent.dwStopEvt )
					{
						//�������������豸
						if( objSleepEvent.dwStopEvt & EVENT_TYPE_IRDA )
						{
							G_ResetSystem();
						}
						
						Release( SLEEP, &objSleepEvent, NULL, NULL );
						RenewIdleTime( iCurDateTime, szIdleEndTime, 0 );/*��¼����״̬����ʱ��*/

						FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, 1 );
						FlushSysState( objChnRegion, bChnRegionOn, 1 );
						
						g_enumSysSta = IDLE;
						PRTMSG(MSG_NOR, "System In Idle\n");
						
						RenewMemInfo(0x02, 0x01, 0x00, 0x01);

						// ֪ͨQianExe��ϵͳ�����ʡ��״̬
						char buf = 0x04;
						DataPush(&buf, 1, DEV_DVR, DEV_QIAN, LV1);
					}
				}
				break;
			case IDLE:/*����״̬�¼����*/
				{
					if( g_iTempState==1 )
					{
						bNewIdle = true;
						break;
					}
					else if( g_iTempState==2 )
					{
						ClearThreadDog(0);
						
						bNewIdle = true;
						ShowDiaodu("���ڸ�ʽ��SD��,���Ժ�...");
						sleep(3);
						break;
					}
					else if( g_iTempState==3 )
					{
						ClearThreadDog(0);
						
						bNewIdle = true;
						ShowDiaodu("���ڸ�ʽ��Ӳ��,���Ժ�...");
						sleep(3);
						break;
					}

					if( true == bNewIdle )
					{
						bNewIdle = false;
						dwIdleBeginTm = GetTickCount();
					}
					if( GetTickCount() - dwIdleBeginTm > IDLE_DELAY*60*1000 )
					{
						//��ԭΪĬ�ϵ�20����ʡ��ʱ��
						IDLE_DELAY = 20;
							
						BYTE ucIOState;
						IOGet( IOS_ACC, &ucIOState );
						if( ucIOState==IO_ACC_OFF )
						{
							//ACC��Чʱ����ʡ��״̬
							PRTMSG(MSG_NOR, "timeout start sleep\n");
							objSleepEvent.dwStartEvt |= EVENT_TYPE_TIMEOUT;
						}
						else
						{
							//ACC��Чʱ���¼�ʱ
							bNewIdle = true;
						}
					}
					
					/*����ź��˳�����*/
					if( g_bProgExit )
					{
						FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, -1 );
						FlushSysState( objChnRegion, bChnRegionOn, -1 );
						
						Release( IDLE, NULL, NULL, NULL );
						
						sleep(2);
						return NULL;
					}
					/*��⵽����ʡ��״̬�¼�, ���ӿ���״̬����ʡ��״̬*/
					else if( objSleepEvent.dwStartEvt )
					{
						FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, -1 );
						FlushSysState( objChnRegion, bChnRegionOn, -1 );
						
						Init( SLEEP, &objSleepEvent, &objCtrlEvent, &objWorkEvent );
						RenewIdleTime( iCurDateTime, szIdleEndTime, -1 );/*��տ���״̬����ʱ��*/
						
						g_enumSysSta = SLEEP;	
						bNewIdle = true;
						PRTMSG(MSG_NOR, "System In Sleep\n");
						
						RenewMemInfo(0x02, 0x01, 0x00, 0x00);

						// ֪ͨQianExe��ϵͳ����ʡ��״̬
						char buf = 0x03;
						DataPush(&buf, 1, DEV_DVR, DEV_QIAN, LV1);
					}
					/*��⵽��������״̬�¼�, ���ӿ���״̬�������״̬*/
					else if( objCtrlEvent.dwStartEvt )
					{
						FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, -1 );
						FlushSysState( objChnRegion, bChnRegionOn, -1 );
						
						Init( CTRL, &objSleepEvent, &objCtrlEvent, &objWorkEvent );
						RenewIdleTime( iCurDateTime, szIdleEndTime, -1 );/*��տ���״̬����ʱ��*/

						g_enumSysSta = CTRL;
						bNewIdle = true;
						PRTMSG(MSG_NOR, "System In Ctrl\n");
						
						RenewMemInfo(0x02, 0x01, 0x00, 0x03);
					}
					/*��⵽��������״̬�¼�, ���ӿ���״̬���빤��״̬*/
					else if( objWorkEvent.dwRecStartEvt ||objWorkEvent.dwUplStartEvt ||objWorkEvent.dwPicStartEvt ||objWorkEvent.dwTtsStartEvt ||objWorkEvent.dwBlkStartEvt )
					{
						Init( WORK, &objSleepEvent, &objCtrlEvent, &objWorkEvent );
						RenewIdleTime( iCurDateTime, szIdleEndTime, -1 );/*��տ���״̬����ʱ��*/
							
						g_enumSysSta = WORK;
						bNewIdle = true;
						PRTMSG(MSG_NOR, "System In Work\n");
						
						RenewMemInfo(0x02, 0x01, 0x00, 0x02);
					}
				}
				break;
			case CTRL:/*����״̬�¼����*/
				{
					/*��⵽ֹͣ����״̬�¼�, ���Ӳ���״̬�������״̬*/
					if(g_iTempState || g_bProgExit || objCtrlEvent.dwStopEvt)
					{
						Release( CTRL, NULL, &objCtrlEvent, NULL );
						RenewIdleTime( iCurDateTime, szIdleEndTime, 0 );/*��¼����״̬����ʱ��*/
						bPeriodIgnore = FALSE;/*���ʱ�δ�������*/
						
						FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, 1 );
						FlushSysState( objChnRegion, bChnRegionOn, 1 );
						
						g_enumSysSta = IDLE;
						PRTMSG(MSG_NOR, "System In Idle\n");
						
						RenewMemInfo(0x02, 0x01, 0x00, 0x01);
					}
				}
				break;
			case WORK:/*����״̬�¼����*/
				{
					/*��⵽����¼���¼�, ������¼��*/
					if( objWorkEvent.dwRecStartEvt )
					{
						if( StartAVRec() == 0 )
						{
							FlushSysState( objChnRegion, bChnRegionOn, 0 );
							RenewRecRptBuf( &objWorkEvent, g_bytRecRptBuf );
							bPeriodIgnore = FALSE;/*�������ʱ�δ�������������״̬�¼�*/
						}
						GetCurDateTime( iCurDateTime );
					  RenewWorkTime( WORK_REC, iCurDateTime, szRecEndTime, &objWorkEvent, 0 );
					}
					
					/*��⵽ֹͣ¼���¼�, ��ֹͣ¼��*/
					if( g_iTempState || g_bProgExit || objWorkEvent.dwRecStopEvt )
					{
						//����ǳ�ʱ�˳�¼��ʱ��������ʡ��״̬
						if(objWorkEvent.dwRecStopEvt & EVENT_TYPE_TIMEOUT)
							IDLE_DELAY = 0;
						
						/*��¼ͣ¼��������*/
						if((objWorkEvent.dwRecStopEvt & EVENT_TYPE_ERROR)||(objWorkEvent.dwRecStopEvt & EVENT_TYPE_POWOFF))
							g_bytRecRptBuf[13] = 0x05;
						else if(objWorkEvent.dwRecStopEvt & EVENT_TYPE_IRDA)
							g_bytRecRptBuf[13] = 0x04;
						else if(objWorkEvent.dwRecStopEvt & EVENT_TYPE_CENT)
							g_bytRecRptBuf[13] = 0x03;
						else if(objWorkEvent.dwRecStopEvt & EVENT_TYPE_TIMEOUT)
							g_bytRecRptBuf[13] = 0x02;

						if( StopAVRec() == 0 )
						{
							FlushSysState( objChnRegion, bChnRegionOn, 0 );
							sleep(1);// �������ʱ����
						}
						RenewWorkTime( WORK_REC, iCurDateTime, szRecEndTime, &objWorkEvent, -1 );
					}
/*----------------------------------------------------------------------------------------------------------------*/
					/*��⵽��������¼�, ���������*/
					if( !g_bDeal380A && objWorkEvent.dwUplStartEvt )
					{
						RenewMemInfo(0x02, 0x02, 0x02, 0x01);
							
//						FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, -1 );
// 						FlushSysState( objChnRegion, bChnRegionOn, -1 );

//						StopVPrev();	
						if( StartAVUpl() == 0 )
							FlushSysState( objChnRegion, bChnRegionOn, 0 );
						RenewWorkTime( WORK_UPL, iCurDateTime, szUplEndTime, &objWorkEvent, 0 );
					}
					
					/*��⵽ֹͣ����¼�, ��ֹͣ���*/
					if( g_bDeal380A || g_iTempState || g_bProgExit || objWorkEvent.dwUplStopEvt )
					{
						if(objWorkEvent.dwUplStopEvt & EVENT_TYPE_CENT)
							g_bytUplRptBuf[1] = 0x42;
						else 
							g_bytUplRptBuf[1] = 0x44;
						
						if( StopAVUpl() == 0 )
							FlushSysState( objChnRegion, bChnRegionOn, 0 );

						RenewWorkTime( WORK_UPL, iCurDateTime, szUplEndTime, &objWorkEvent, -1 );
//						StartVPrev();

//						FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, 1 );
//						FlushSysState( objChnRegion, bChnRegionOn, 1 );
					}
/*----------------------------------------------------------------------------------------------------------------*/
					/*��⵽���������¼�, ����������*/
					if( objWorkEvent.dwPicStartEvt )
					{
						if( StartPicture() == 0 )
							FlushSysState( objChnRegion, bChnRegionOn, 0 );

						RenewWorkTime( WORK_PIC, iCurDateTime, szPicEndTime, &objWorkEvent, 0 );
					}

					/*��⵽ֹͣ�����¼�, ��ֹͣ����*/
					if(g_iTempState || g_bProgExit || objWorkEvent.dwPicStopEvt)
					{
						if( StopPicture() == 0 )
							FlushSysState( objChnRegion, bChnRegionOn, 0 );
						RenewWorkTime( WORK_PIC, iCurDateTime, szPicEndTime, &objWorkEvent, -1 );
					}
/*----------------------------------------------------------------------------------------------------------------*/
					/*��⵽����TTS�¼�, ������TTS*/
					if(objWorkEvent.dwTtsStartEvt)
					{
						if( StartTTS() == 0 )
							FlushSysState( objChnRegion, bChnRegionOn, 0 );
						RenewWorkTime( WORK_TTS, iCurDateTime, szTtsEndTime, &objWorkEvent, 0 );
					}

					/*��⵽ֹͣTTS�¼�, ��ֹͣTTS*/
					if(g_iTempState || g_bProgExit || objWorkEvent.dwTtsStopEvt)
					{
						if( StopTTS() == 0 )
							FlushSysState( objChnRegion, bChnRegionOn, 0 );
						RenewWorkTime( WORK_TTS, iCurDateTime, szTtsEndTime, &objWorkEvent, -1 );
					}
/*----------------------------------------------------------------------------------------------------------------*/
					/*��⵽������ϻ���¼�, ��������ϻ��*/
					if(objWorkEvent.dwBlkStartEvt)
					{
						if( StartBlack() == 0 )
							FlushSysState( objChnRegion, bChnRegionOn, 0 );
						RenewWorkTime( WORK_BLK, iCurDateTime, szBlkEndTime, &objWorkEvent, 0 );
					}

					/*��⵽ֹͣ��ϻ���¼�, ��ֹͣ��ϻ��*/
					if(g_iTempState || g_bProgExit || objWorkEvent.dwBlkStopEvt)
					{
						if( StopBlack() == 0 )
							FlushSysState( objChnRegion, bChnRegionOn, 0 );
						RenewWorkTime( WORK_BLK, iCurDateTime, szBlkEndTime, &objWorkEvent, -1 );
					}
/*----------------------------------------------------------------------------------------------------------------*/
					/*�ӹ���״̬�������״̬*/
					if( !g_objWorkStart.ARecord[0] && !g_objWorkStart.ARecord[1] && !g_objWorkStart.ARecord[2] && !g_objWorkStart.ARecord[3] 
						&&!g_objWorkStart.VRecord[0] && !g_objWorkStart.VRecord[1] && !g_objWorkStart.VRecord[2] && !g_objWorkStart.VRecord[3] 
						&&!g_objWorkStart.AUpload[0] && !g_objWorkStart.AUpload[1] && !g_objWorkStart.AUpload[2] && !g_objWorkStart.AUpload[3] 
						&&!g_objWorkStart.VUpload[0] && !g_objWorkStart.VUpload[1] && !g_objWorkStart.VUpload[2] && !g_objWorkStart.VUpload[3]
						&&!g_objWorkStart.Picture
						&&!g_objWorkStart.TTS
						&&!g_objWorkStart.Black )
					{
						Release( WORK, NULL, NULL, &objWorkEvent );
						GetCurDateTime( iCurDateTime );
						RenewIdleTime( iCurDateTime, szIdleEndTime, 0 );/*��¼����״̬����ʱ��*/
						
						/*��չ���״̬����ʱ��*/
						RenewWorkTime( WORK_UPL, iCurDateTime, szUplEndTime, &objWorkEvent, -1 );
						RenewWorkTime( WORK_REC, iCurDateTime, szRecEndTime, &objWorkEvent, -1 );
						RenewWorkTime( WORK_PIC, iCurDateTime, szPicEndTime, &objWorkEvent, -1 );
						RenewWorkTime( WORK_TTS, iCurDateTime, szTtsEndTime, &objWorkEvent, -1 );
						RenewWorkTime( WORK_BLK, iCurDateTime, szBlkEndTime, &objWorkEvent, -1 );
						
						FlushSysState( objChnRegion, bChnRegionOn, 0 );
						
						g_enumSysSta = IDLE;
						PRTMSG(MSG_NOR, "System In Idle\n");
						
						RenewMemInfo(0x02, 0x01, 0x00, 0x01);
					}
				}
				break;
			default:
				break;
		}
		
		if(g_bDeal380A)
		{
			g_bDeal380A = false;
		}
/*----------------------------------------------------------------------------------------------------------------*/
		msleep(50);
		
		lLastSec = lCurSec;
		lCurSec = GetCurDateTime( iCurDateTime );
		
		//ˢ����ʾʱ��
		if( lLastSec < lCurSec )
			FlushShowTime( &objTimeRegion, iCurDateTime, &bTimeRegionOn, 0 );

		sprintf( szCurTime, "%02d:%02d:%02d", iCurDateTime[3], iCurDateTime[4], iCurDateTime[5] );
	}
}
