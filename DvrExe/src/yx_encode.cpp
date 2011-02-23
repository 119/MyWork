#include "yx_common.h"

pthread_t g_pthreadRec;
pthread_t g_pthreadUpl;

int MPP_Init()
{
	int iRet;
	
	PRTMSG(MSG_NOR, "Init Media Process Platform\n");
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
  VB_CONF_S objVbConf = {0};
	MPP_SYS_CONF_S objSysConf = {0};

	objVbConf.u32MaxPoolCnt = 64;
	objVbConf.astCommPool[0].u32BlkSize = 768*576*2;//4D1
	objVbConf.astCommPool[0].u32BlkCnt = 8*4;	
	objVbConf.astCommPool[1].u32BlkSize = 768*288*2;//4HD1
	objVbConf.astCommPool[1].u32BlkCnt = 8*4;	
	objSysConf.u32AlignWidth = 64;  

  iRet = HI_MPI_VB_SetConf(&objVbConf);
  if(iRet) goto ERROR;

  iRet = HI_MPI_VB_Init();
  if(iRet) goto ERROR;

  iRet = HI_MPI_SYS_SetConf(&objSysConf);
  if(iRet) goto ERROR;

  iRet = HI_MPI_SYS_Init();
  if(iRet) goto ERROR;

#endif

	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "init media process platform failed: 0x%08x\n", iRet);
	return iRet;
}

int MPP_Exit()
{
	int iRet;
	
	PRTMSG(MSG_NOR, "Exit Media Process Platform\n");

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	iRet = HI_MPI_SYS_Exit();
	if(iRet) goto ERROR;
	
	iRet = HI_MPI_VB_Exit();
	if(iRet) goto ERROR;
#endif

	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "exit media process platform failed: 0x%08x\n", iRet);
	return iRet;	
}

int StartVenc()
{
	int iRet;
	
	PRTMSG(MSG_NOR, "Start Video Encode Device\n");

#if VEHICLE_TYPE == VEHICLE_M
	VENC_INIT_S objVencInit;
	objVencInit.enVencType = VENC_FORMAT_H264;
	objVencInit.reserved = 0;
	
	iRet = HI_VENC_Open(&objVencInit);
	if(iRet) goto ERROR;
#endif

	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "start video encode device failed: 0x%08x\n", iRet);
	return iRet;
}

int StopVenc()
{
	int iRet;
	
	PRTMSG(MSG_NOR, "Stop Video Encode Device\n");
	
#if VEHICLE_TYPE == VEHICLE_M
	iRet = HI_VENC_Close();
	if(iRet) goto ERROR;
#endif

	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "stop video encode device failed: 0x%08x\n", iRet);
	return iRet;	
}

int ViADCfg(uint v_uimode, uint v_uinormal)
{
#if VEHICLE_TYPE == VEHICLE_M
	int iRet;
	int val = 0;
	
	ioctl(g_objDevFd.Tw2835, TW2835_SETNORM, &v_uinormal);
	
	switch(v_uimode)
	{
		case CIF:
			iRet = ioctl(g_objDevFd.Tw2835,TW2835_SET_4CIF, &val);
			if(iRet) goto ERROR;
			break;
		case HD1:
			iRet = ioctl(g_objDevFd.Tw2835,TW2835_SET_2HALF_D1, &val);
			if(iRet) goto ERROR;
			break;
		case D1:  
			iRet = ioctl(g_objDevFd.Tw2835,TW2835_SET_1D1, &val);
			if(iRet) goto ERROR;
			break;
		default:
			iRet = -1;
			goto ERROR;
			break;
	}
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	int i;
	int iRet;
	tw2865_set_2d1 objVideoMode;
	tw2865_set_videomode objVideoNormal;
	tw2865_set_videooutput objVideoOutput;

	for(i=0; i<=3; i++)
	{
		objVideoNormal.ch = i;
		objVideoNormal.mode = v_uinormal==PAL ? TW2865_PAL : TW2865_NTSC;
		msleep(10);
		iRet = ioctl(g_objDevFd.Tw2865, TW2865CMD_SET_VIDEO_MODE, &objVideoNormal);
		if(iRet) goto ERROR;
	}
	
	objVideoMode.vdn = 1;
	objVideoMode.ch1 = 1;
	objVideoMode.ch2 = 3;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_SET_2_D1, &objVideoMode);
	if(iRet) goto ERROR;
	objVideoMode.vdn = 2;
	objVideoMode.ch1 = 0;
	objVideoMode.ch2 = 2;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_SET_2_D1, &objVideoMode);
	if(iRet) goto ERROR;
		
	//ѡ��ģ����Ƶ���
	objVideoOutput.PlaybackOrLoop = 0;
	objVideoOutput.channel = 0;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_VIDEO_OUT, &objVideoOutput);
	if(iRet) goto ERROR;
#endif
	
	return 0;
	
ERROR:
	return iRet;
}

int ViPubCfg(uint v_uimode, uint v_uinormal)
{
	int iRet;
	
#if VEHICLE_TYPE == VEHICLE_M
	VI_ATTR_S objPubAttr;

	iRet = HI_VI_GetPublicAttr(&objPubAttr);
	if(iRet) goto ERROR; 

	/*����¼�������豸��������*/
	objPubAttr.enMaster = VIDEO_CONTROL_MODE_SLAVER;
	if(v_uinormal == NTSC)
		objPubAttr.enInputNorm = VIDEO_ENCODING_MODE_NTSC;
	else
		objPubAttr.enInputNorm = VIDEO_ENCODING_MODE_PAL;
	objPubAttr.enInputMode = VI_MODE_BT656 ;
	objPubAttr.u32CapSel = VI_CAP_BOTH;
	objPubAttr.u32Bufsize = 6*1024*1024;
	objPubAttr.bDownScaling = HI_FALSE;
	
	switch(v_uimode)
	{
		case CIF:
			objPubAttr.bDownScaling = HI_FALSE;
			objPubAttr.u32InputOption = VI_IA_L24;    
			break;
		case HD1:
			objPubAttr.bDownScaling = HI_FALSE;
			objPubAttr.u32InputOption = VI_IA_L12; 
			break;
		case D1:
			objPubAttr.bDownScaling = HI_FALSE;
			objPubAttr.u32InputOption = VI_IA_L11; 
			break;
		default:
			break;
  }

	iRet = HI_VI_SetPublicAttr(&objPubAttr);
	if(iRet) goto ERROR; 
	
	iRet = HI_VI_EnableVI();
	if(iRet) goto ERROR; 
#endif
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	int i;
	VI_PUB_ATTR_S objPubAttr;
	
	objPubAttr.enInputMode = VI_MODE_BT656;
	objPubAttr.enWorkMode = VI_WORK_MODE_2D1;
	objPubAttr.u32AdType = AD_OTHERS;
	objPubAttr.enViNorm = v_uinormal==PAL ? VIDEO_ENCODING_MODE_PAL : VIDEO_ENCODING_MODE_NTSC;
	objPubAttr.stRect.s32X = 0;
	objPubAttr.stRect.s32Y = 0;
	objPubAttr.stRect.u32Width = 720;
	objPubAttr.stRect.u32Height = 576;
	
	//VI�豸ֻʹ��2��3
  for(i=2; i<=3; i++)
  {            
    iRet = HI_MPI_VI_SetPubAttr(i, &objPubAttr);
    if(iRet) goto ERROR;
    	
    iRet = HI_MPI_VI_Enable(i);
    if(iRet) goto ERROR;
  }
#endif
  
  return 0;
  
ERROR:
	return iRet;
}

int ViChnCfg(uint v_uimode, uint v_uinormal)
{
#if VEHICLE_TYPE == VEHICLE_M
  int iRet = 0;
	VI_CH_ATTR_S objChnAttr;
  
	if (v_uinormal == PAL)
	{
		switch (v_uimode)
		{
			case CIF:
			{
				iRet = HI_VI_GetCHAttr(0, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 352;
				objChnAttr.u32Height = 288;
				objChnAttr.u32CapSel = VI_CAP_TOP;
				iRet = HI_VI_SetCHAttr(0, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(0);
				if(iRet) goto ERROR;
				
				iRet = HI_VI_GetCHAttr(1, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 352;
				objChnAttr.u32Height = 288;
				objChnAttr.u32CapSel = VI_CAP_TOP;
				iRet = HI_VI_SetCHAttr(1, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(1);
				if(iRet) goto ERROR;
				
				iRet = HI_VI_GetCHAttr(2, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 352;
				objChnAttr.u32Height = 288;
				objChnAttr.u32CapSel = VI_CAP_BOTTOM;
				iRet = HI_VI_SetCHAttr(2, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(2);
				if(iRet) goto ERROR;
				
				iRet = HI_VI_GetCHAttr(3, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 352;
				objChnAttr.u32Height = 288;
				objChnAttr.u32CapSel = VI_CAP_BOTTOM;
				iRet = HI_VI_SetCHAttr(3, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(3);
				if(iRet) goto ERROR;
      }
				break;
          
			case HD1:
			{
				iRet = HI_VI_GetCHAttr(0, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 720;
				objChnAttr.u32Height = 288;
				objChnAttr.u32CapSel = VI_CAP_TOP;
				iRet = HI_VI_SetCHAttr(0, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(0);
				if(iRet) goto ERROR;
				
				iRet = HI_VI_GetCHAttr(1, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 720;
				objChnAttr.u32Height = 288;
				objChnAttr.u32CapSel = VI_CAP_BOTTOM;
				iRet = HI_VI_SetCHAttr(1, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(1);
				if(iRet) goto ERROR;
      }
				break;
			
			case D1:
			{
				iRet = HI_VI_GetCHAttr(0, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 720;
				objChnAttr.u32Height = 288;
				objChnAttr.u32CapSel = VI_CAP_BOTH;
				iRet = HI_VI_SetCHAttr(0, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(0);
				if(iRet) goto ERROR;
      }
				break;
            
			default: 
				break;
		}
	}
	else
	{
		switch (v_uimode)
		{			
			case CIF:
			{
				iRet = HI_VI_GetCHAttr(0, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 352;
				objChnAttr.u32Height = 240;
				objChnAttr.u32CapSel = VI_CAP_TOP;
				iRet = HI_VI_SetCHAttr(0, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(0);
				if(iRet) goto ERROR;
				
				iRet = HI_VI_GetCHAttr(1, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 352;
				objChnAttr.u32Height = 240;
				objChnAttr.u32CapSel = VI_CAP_TOP;
				iRet = HI_VI_SetCHAttr(1, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(1);
				if(iRet) goto ERROR;
				
				iRet = HI_VI_GetCHAttr(2, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 352;
				objChnAttr.u32Height = 240;
				objChnAttr.u32CapSel = VI_CAP_BOTTOM;
				iRet = HI_VI_SetCHAttr(2, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(2);
				if(iRet) goto ERROR;
				
				iRet = HI_VI_GetCHAttr(3, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 352;
				objChnAttr.u32Height = 240;
				objChnAttr.u32CapSel = VI_CAP_BOTTOM;
				iRet = HI_VI_SetCHAttr(3, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(3);
				if(iRet) goto ERROR;
      }
				break;
            
			case HD1:
			{
				iRet = HI_VI_GetCHAttr(0, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 720;
				objChnAttr.u32Height = 240;
				objChnAttr.u32CapSel = VI_CAP_TOP;
				iRet = HI_VI_SetCHAttr(0, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(0);
				if(iRet) goto ERROR;
				
				iRet = HI_VI_GetCHAttr(1, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 720;
				objChnAttr.u32Height = 240;
				objChnAttr.u32CapSel = VI_CAP_BOTTOM;
				iRet = HI_VI_SetCHAttr(1, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(1);
				if(iRet) goto ERROR;
      }
				break;
			
			case D1:
			{
				iRet = HI_VI_GetCHAttr(0, &objChnAttr); 
  			if(iRet) goto ERROR;
  			  
				objChnAttr.u32StartX = 0;
				objChnAttr.u32StartY = 0;
				objChnAttr.u32Width  = 720;
				objChnAttr.u32Height = 240;
				objChnAttr.u32CapSel = VI_CAP_BOTH;
				iRet = HI_VI_SetCHAttr(0, &objChnAttr);
				if(iRet) goto ERROR;
  			  
  			iRet = HI_VI_EnableVIChn(0);
				if(iRet) goto ERROR;
      }
				break;
            
			default: 
			break;
		} 
	}
#endif
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	int i,j;
  int iRet;
  VI_CHN_ATTR_S objChnAttr={0};
  
	objChnAttr.stCapRect.s32X = 0;
	objChnAttr.stCapRect.s32Y = 0;
	objChnAttr.stCapRect.u32Width = 720;
	objChnAttr.stCapRect.u32Height = v_uinormal==PAL ? 288 : 240;	
	objChnAttr.enCapSel = VI_CAPSEL_BOTH;
	objChnAttr.bDownScale = HI_FALSE;
	objChnAttr.bHighPri = HI_FALSE;
	objChnAttr.bChromaResample = HI_FALSE;
	objChnAttr.enViPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
	
	for(i=2; i<=3; i++)//VI�豸ֻʹ��2��3
	{
		for(j=0; j<=1; j++)//2D1ģʽ��ÿ��VI�豸��ӦVIͨ��0��1
		{
		  iRet = HI_MPI_VI_SetChnAttr(i, j, &objChnAttr);
			if(iRet) goto ERROR;
			    	                  
			iRet = HI_MPI_VI_EnableChn(i, j);
			if(iRet) goto ERROR;
		}
	}
#endif
	
	return 0;
	
ERROR:
	return iRet;
}

int StartVi()
{
	PRTMSG(MSG_NOR, "Start Video Input Channel\n");

	int i;
	int iRet;
	
	for(i = 0; i < 3; i++)
	{
		iRet = ViADCfg(g_objMvrCfg.VInput.VMode, g_objMvrCfg.VInput.VNormal);
		if(iRet)
		{
			ShowDiaodu("����Ƶģ���쳣");
			RenewMemInfo(0x02, 0x00, 0x00, 0x02);
			
			msleep(100);
		}
		else
			break;
	}

	iRet = ViPubCfg(g_objMvrCfg.VInput.VMode, g_objMvrCfg.VInput.VNormal);
	if(iRet) goto ERROR;

	iRet = ViChnCfg(g_objMvrCfg.VInput.VMode, g_objMvrCfg.VInput.VNormal);
	if(iRet) goto ERROR;

	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "start video input channel failed: 0x%08x\n", iRet);
	return iRet;
}

int StopVi()
{
	PRTMSG(MSG_NOR, "Stop Video Input Channel\n");

	int	iRet;
	
#if VEHICLE_TYPE == VEHICLE_M
	int i;
	
	for(i=0; i<4; i++ )
	{
		iRet = HI_VI_DisableOverlay(i);
		if(iRet) goto ERROR;
	}
    
	for(i=0; i<4; i++ )
	{
		iRet = HI_VI_DisableVIChn(i);
		if(iRet) goto ERROR;
	}

	iRet = HI_VI_DisableVI();
	if(iRet) goto ERROR;
#endif
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	int i,j;
	
	for(i=2; i<=3; i++)
	{
		for(j=0; j<=1; j++)
		{           
		  iRet = HI_MPI_VI_DisableChn(i, j);
		  if(iRet) goto ERROR;
		}
		
		iRet = HI_MPI_VI_Disable(i);
	  if(iRet) goto ERROR;
	}
#endif

	return 0;

ERROR:
	PRTMSG(MSG_ERR, "stop video input channel failed: 0x%08x\n", iRet);
	return iRet;
}

int StartVencChn()
{
	int i,j,k;
	int iRet;
	int iGroup;
	
	PRTMSG(MSG_NOR, "Start Video Encode Channel\n");

#if VEHICLE_TYPE == VEHICLE_M

#if NO_VENC == 0
	VENC_CHN iVencChn;
	
	for(i=0; i<=7; i++)
	{
		iRet = HI_VENC_CreateCH(i/2, &iVencChn);
		if(iRet) goto ERROR;
	}
#endif

#endif
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	for(i=2; i<=3; i++)
	{
		for(j=0; j<=1; j++)
		{
			for(k=0; k<=2; k++)
			{
				if(k==2)
					iGroup = 2*i+j+4;
				else
					iGroup = 4*(i-2)+2*j+k;
					
				iRet = HI_MPI_VENC_CreateGroup(iGroup);
				if(iRet) goto ERROR;
				
				switch(iGroup)
				{
					case 0:
					case 1:
					case 8:
						iRet = HI_MPI_VENC_BindInput(iGroup, 2, 0);
						break;
					case 2:
					case 3:
					case 9:
						iRet = HI_MPI_VENC_BindInput(iGroup, 3, 0);
						break;
					case 4:
					case 5:
					case 10:
						iRet = HI_MPI_VENC_BindInput(iGroup, 2, 1);
						break;
					case 6:
					case 7:
					case 11:
						iRet = HI_MPI_VENC_BindInput(iGroup, 3, 1);
						break;
					default:
						break;
				}
				if(iRet) goto ERROR;
			}
		}
	}
#endif
	
	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "start video encode channel failed: 0x%08x\n", iRet);
	return iRet;
}

int StopVencChn()
{
	int i;
	int iRet;
	
	PRTMSG(MSG_NOR, "Stop Video Encode Channel\n");

#if VEHICLE_TYPE == VEHICLE_M

#if NO_VENC == 0
	for(i=0; i<=7; i++)
	{
		iRet = HI_VENC_DestroyCH(i, HI_TRUE);
		if(iRet) goto ERROR;
	}
#endif
	
#endif
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	for(i=0; i<=11; i++)
	{
		iRet = HI_MPI_VENC_UnbindInput(i);
		if(iRet) goto ERROR;
				
		iRet =HI_MPI_VENC_DestroyGroup(i);
		if(iRet) goto ERROR;
	}
#endif
	
	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "stop video encode channel failed: 0x%08x\n", iRet);
	return iRet;
}

int StartAenc()
{
	PRTMSG(MSG_NOR, "Start Audio Encode Device\n");
	
	int iRet;
	
#if VEHICLE_TYPE == VEHICLE_M
	iRet = HI_AENC_Open(); 
	if(iRet) goto ERROR;
#endif

	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "start audio encode device failed: 0x%08x\n", iRet);
	return iRet;
}

int StopAenc()
{
	PRTMSG(MSG_NOR, "Stop Audio Encode Device\n");

	int iRet;
	
#if VEHICLE_TYPE == VEHICLE_M
	iRet = HI_AENC_Close();
	if(iRet) goto ERROR;
#endif

	return 0;

ERROR:
	PRTMSG(MSG_ERR, "stop audio encode device failed: 0x%08x\n", iRet);
	return iRet;	
}

int AiADCfg()
{
	int iRet;
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	int val;
	tw2865_set_audiosamplarate enumAudioRate;
	tw2865_set_audiooutput objAudioOutput;
	
	//������Ƶ����Ƶ��
	enumAudioRate = ASR_32K;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865, TW2865CMD_SET_ADA_SAMPLERATE, &enumAudioRate);
	if(iRet) goto ERROR;
	
	//������Ƶ���λ��
	val = SET_16_BITWIDTH;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_SET_ADA_BITWIDTH, &val);
	if(iRet) goto ERROR;
	
	//����Ƶ�������ͨ����
	val = 4;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_SET_AUDIO_OUTPUT, &val);
	if(iRet) goto ERROR;
	
	//��������������·������
	val = 3;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865, TW2865CMD_SET_CHANNEL_SEQUENCE, &val);
  if(iRet) goto ERROR;
	
	//ѡ��ģ����Ƶ���
	objAudioOutput.PlaybackOrLoop = 0;
	objAudioOutput.channel = 0;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_AUDIO_OUTPUT, &objAudioOutput);
	if(iRet) goto ERROR;
	
	//������Ƶ��������
	val = 15;
	msleep(10);
	iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_SET_AIGAIN, &val);
	if(iRet) goto ERROR;

	//������Ƶ�������
	msleep(10);
	SetSysVolume(false, 0);
#endif
	
	return 0;
	
ERROR:
	return iRet;
}

int AiPubCfg()
{
  int iRet;
  
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	int i;
	AIO_ATTR_S objPubAttr;
  
  objPubAttr.enSamplerate = AUDIO_SAMPLE_RATE_32000;//��̬����,��Ƶ������(��ģʽ�²�������)
  objPubAttr.enBitwidth = AUDIO_BIT_WIDTH_16;//��̬����,��Ƶ��������(��ģʽ�±������ƵAD/DA�Ĳ�������ƥ��)
  objPubAttr.enWorkmode = AIO_MODE_I2S_SLAVE;//��̬����,��Ƶ�����������ģʽ
  objPubAttr.enSoundmode = AUDIO_SOUND_MODE_MOMO;//��̬����,��Ƶ����ģʽ
  objPubAttr.u32EXFlag = 1;//��̬����,8bit��16bit��չ��־(8bit����ʱ��Ч;0:����չ;1:��չ)
  objPubAttr.u32FrmNum = 30;//��̬����,����֡��Ŀ,1~10
  objPubAttr.u32PtNumPerFrm = 480;//��̬����,ÿ֡�Ĳ��������,G711��G726��ADPCM_DVI4����ʱȡֵΪ80��160��240��320��480��ADPCM_IMA����ʱȡֵΪ81��161��241��321��481��AMR����ʱֻ֧��160
  
  iRet = HI_MPI_AI_SetPubAttr(1, &objPubAttr);
  if(iRet) goto ERROR;
	
  iRet = HI_MPI_AI_Enable(1);
  if(iRet) goto ERROR;
#endif

  return 0;
  
ERROR:
	return iRet;
}

int AiChnCfg()
{
	int i;
  int iRet;
  
#if VEHICLE_TYPE == VEHICLE_M
	AUDIO_CH_ATTR_S objChnAttr;
  
	for(i=0; i<4; i++)
	{
		iRet = HI_AENC_GetAIAttr(i+2, &objChnAttr); 
		if(iRet) goto ERROR;

		objChnAttr.enSampleRate = AUDIO_SAMPLE_RATE_8;
		objChnAttr.enBitWidth 	= AUDIO_BIT_WIDTH_16;
		objChnAttr.enMode 			= AUDIO_MODE_I2S_SLAVE;

		iRet = HI_AENC_SetAIAttr(i+2, &objChnAttr);
		if(iRet) goto ERROR;
	}
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	for(i=0; i<=3; i++)
	{
		iRet = HI_MPI_AI_EnableChn(1, i);
		if(iRet) goto ERROR;   
	} 
#endif
	
	return 0;
	
ERROR:
	return iRet;
}

int StartAi()
{
	PRTMSG(MSG_NOR, "Start Audio Input Channel\n");

	int i;
	int iRet;

#if VEHICLE_TYPE == VEHICLE_M
	iRet = AiChnCfg();
	if(iRet) goto ERROR;
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	for(i = 0; i < 3; i++)
	{
		iRet = AiADCfg();
		if(iRet)	
		{
			ShowDiaodu("����Ƶģ���쳣");
			RenewMemInfo(0x02, 0x00, 0x00, 0x02);
			
			msleep(100);
		}
		else
			break;
	}

	iRet = AiPubCfg();
	if(iRet) goto ERROR;

	iRet = AiChnCfg();
	if(iRet) goto ERROR;

#endif

	return 0;
	
ERROR:
	PRTMSG(MSG_ERR, "start audio input channel failed: 0x%08x\n", iRet);
	return iRet;
}

int StopAi()
{
	int i;
	int	iRet;
	
	PRTMSG(MSG_NOR, "Stop Audio Input Channel\n");
	
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	for(i=0; i<=3; i++)
	{
		iRet = HI_MPI_AI_DisableChn(1, i);
		if(iRet) goto ERROR;
	}

	iRet = HI_MPI_AI_Disable(1);
	if(iRet) goto ERROR;
#endif

	return 0;

ERROR:
	PRTMSG(MSG_ERR, "stop audio input channel failed: %08x\n", iRet);
	return iRet;
}

int StartAencChn()
{
	PRTMSG(MSG_NOR, "Start Audio Encode Channel\n");

	int i;
	int iRet;
#if VEHICLE_TYPE == VEHICLE_M
	AENC_CHN iAencChn;
	
	for(i=0; i<=1; i++)
	{
		iRet = HI_AENC_CreateCH(i+2, AUDIO_CODEC_FORMAT_G726, HI_TRUE, &iAencChn);
		if(iRet) goto ERROR;
	}
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	AENC_ATTR_G726_S objAencG726;
	AENC_CHN_ATTR_S objAencAttr;
	
 	objAencAttr.enType = PT_G726;//��̬����,����Э������
 	objAencAttr.u32BufSize = 8;//��̬����,���뻺���С
  objAencAttr.pValue = &objAencG726;//��̬����,Э������ָ��
  
#if CHK_VER == 1
  for(i=0; i<=1; i++)
  {
  	objAencG726.enG726bps = MEDIA_G726_16K; //G.726Э������
	  iRet = HI_MPI_AENC_CreateChn(i, &objAencAttr);
	  if(iRet) goto ERROR;
  }
#else
  objAencG726.enG726bps = MEDIA_G726_16K; //G.726Э������
  iRet = HI_MPI_AENC_CreateChn(0, &objAencAttr);
  if(iRet) goto ERROR;

	objAencG726.enG726bps = G726_16K; //G.726Э������
	iRet = HI_MPI_AENC_CreateChn(1, &objAencAttr);
  if(iRet) goto ERROR;
#endif

#endif

	return 0;

ERROR:
	PRTMSG(MSG_ERR, "start audio encode channel failed: %08x\n", iRet);
  return iRet;
}

int StopAencChn()
{
	int i;
	int iRet;
	
	PRTMSG(MSG_NOR, "Stop Audio Encode Channel\n");

#if VEHICLE_TYPE == VEHICLE_M
	for(i=0; i<=1; i++)
	{
		iRet = HI_AENC_DestroyCH(i+2, HI_TRUE);
		if(iRet) goto ERROR;
	}
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	for(i=0; i<=1; i++)
	{
		iRet = HI_MPI_AENC_DestroyChn(i);
		if(iRet) goto ERROR;
	}
#endif
	
	return 0;

ERROR:
	PRTMSG(MSG_ERR, "stop audio encode channel failed: 0x%08x\n", iRet);
	return iRet;
}

int StartVPrev()
{
	int iRet;
	
	if((!g_objIdleStart.VPreview[0]&&!g_objIdleStart.VPreview[1]&&!g_objIdleStart.VPreview[2]&&!g_objIdleStart.VPreview[3])
	 &&(g_objMvrCfg.VPreview.VStart[0]||g_objMvrCfg.VPreview.VStart[1]||g_objMvrCfg.VPreview.VStart[2]||g_objMvrCfg.VPreview.VStart[3]))
	{
		PRTMSG(MSG_NOR, "Start Video Preview\n");

		int i,j;
		
#if VEHICLE_TYPE == VEHICLE_M

#if NO_PREV == 0
		for(i=0; i<4; i++)
		{
			if(g_objMvrCfg.VPreview.VStart[i])
			{
				iRet = HI_VI_EnVi2Vo(i, i, TRUE);
    		if(iRet) goto ERROR;
			
				g_objIdleStart.VPreview[i] = TRUE;
			}
		}
#endif
		
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
    for(i=2; i<=3; i++)
		{
			for(j=0; j<=1; j++)
			{
				if(g_objMvrCfg.VPreview.VStart[2*(i-2)+j])
				{
					switch(2*(i-2)+j)
					{
						case 0:
							iRet = HI_MPI_VI_BindOutput(2, 0, 0);
							break;
						case 1:
							iRet = HI_MPI_VI_BindOutput(3, 0, 1);
							break;
						case 2:
							iRet = HI_MPI_VI_BindOutput(2, 1, 2);
							break;
						case 3:
							iRet = HI_MPI_VI_BindOutput(3, 1, 3);
							break;
						default:
							break;
					}
					
					if(iRet) goto ERROR;
					g_objIdleStart.VPreview[2*(i-2)+j] = TRUE;
				}
			}
		}
#endif

		return 0;
	}
	
	return -1;
	
ERROR:
	PRTMSG(MSG_ERR, "start video preview failed: 0x%08x\n", iRet);
	return iRet;
}

int StopVPrev()
{
	int iRet;
	
	if(g_objIdleStart.VPreview[0]||g_objIdleStart.VPreview[1]||g_objIdleStart.VPreview[2]||g_objIdleStart.VPreview[3])
	{
		PRTMSG(MSG_NOR, "Stop Video Preview\n");

#if VEHICLE_TYPE == VEHICLE_M

#if NO_PREV == 0
		int i;
		
		for(i = 0; i < 4; i ++)
		{
			if(g_objIdleStart.VPreview[i])
			{
				iRet = HI_VI_DisVi2Vo(i, i, TRUE);
				if(iRet) goto ERROR;
			
				g_objIdleStart.VPreview[i] = FALSE; 
			}
		}
#endif
		
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		int i,j;
		int iRet;
    
    for(i=2; i<=3; i++)
		{
			for(j=0; j<=1; j++)
			{
				if(g_objIdleStart.VPreview[2*(i-2)+j])
				{
					switch(2*(i-2)+j)
					{
						case 0:
							iRet = HI_MPI_VI_UnBindOutput(2, 0, 0);
							break;
						case 1:
							iRet = HI_MPI_VI_UnBindOutput(3, 0, 1);
							break;
						case 2:
							iRet = HI_MPI_VI_UnBindOutput(2, 1, 2);
							break;
						case 3:
							iRet = HI_MPI_VI_UnBindOutput(3, 1, 3);
							break;
						default:
							break;
					}
					if(iRet) goto ERROR;
					
					g_objIdleStart.VPreview[2*(i-2)+j] = FALSE;
				}
			}
		}
#endif

		return 0;
	}

	return -1;
	
ERROR:
	PRTMSG(MSG_ERR, "stop video preview failed: %08x\n", iRet);
	return iRet;
}

int SwitchVPrev(int v_iViChn, int v_iVoChn, int v_iFlag)
{
	int iRet;
	int i;
	
	switch(v_iFlag)
	{
		case -1:
		{
			if(g_objIdleStart.VPreview[0]||g_objIdleStart.VPreview[1]||g_objIdleStart.VPreview[2]||g_objIdleStart.VPreview[3])
			{
				PRTMSG(MSG_NOR, "Switch Video Preview\n");
		
#if VEHICLE_TYPE == VEHICLE_M
				if(g_objIdleStart.VPreview[v_iViChn])
				{
					iRet = HI_VI_DisVi2Vo(v_iViChn, v_iVoChn, TRUE);
					if(iRet) goto ERROR;
					
					g_objIdleStart.VPreview[v_iViChn] = FALSE; 
				}
#endif
		
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
				if(g_objIdleStart.VPreview[v_iViChn])
				{
					switch(v_iViChn)
					{
						case 0:
							iRet = HI_MPI_VI_UnBindOutput(2, 0, v_iVoChn);
							break;
						case 1:
							iRet = HI_MPI_VI_UnBindOutput(3, 0, v_iVoChn);
							break;
						case 2:
							iRet = HI_MPI_VI_UnBindOutput(2, 1, v_iVoChn);
							break;
						case 3:
							iRet = HI_MPI_VI_UnBindOutput(3, 1, v_iVoChn);
							break;
						default:
							break;
					}
					if(iRet) goto ERROR;
							
					g_objIdleStart.VPreview[v_iViChn] = FALSE;
				}
#endif
				return 0;
			}
		
			return -1;
		}
			break;
		case 1:
		{
			if((!g_objIdleStart.VPreview[0]&&!g_objIdleStart.VPreview[1]&&!g_objIdleStart.VPreview[2]&&!g_objIdleStart.VPreview[3]))
			{
				PRTMSG(MSG_NOR, "Switch Video Preview\n");
		
#if VEHICLE_TYPE == VEHICLE_M
				iRet = HI_VI_EnVi2Vo(v_iViChn, v_iVoChn, TRUE);
		    if(iRet) goto ERROR;
					
				g_objIdleStart.VPreview[v_iViChn] = TRUE;
#endif
		
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
				switch(v_iViChn)
				{
					case 0:
						iRet = HI_MPI_VI_BindOutput(2, 0, v_iVoChn);
						break;
					case 1:
						iRet = HI_MPI_VI_BindOutput(3, 0, v_iVoChn);
						break;
					case 2:
						iRet = HI_MPI_VI_BindOutput(2, 1, v_iVoChn);
						break;
					case 3:
						iRet = HI_MPI_VI_BindOutput(3, 1, v_iVoChn);
						break;
					default:
						break;
				}
				if(iRet) goto ERROR;
					
				g_objIdleStart.VPreview[v_iViChn] = TRUE;
#endif
				return 0;
			}
			
			return -1;
		}
			break;
		default:
			return -1;
	}
	
ERROR:
	PRTMSG(MSG_ERR, "switch video preview failed: 0x%08x\n", iRet);
	return iRet;
}

int CtrlVideoView(BYTE v_bytVvChn)
{
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2

	int i, j, k;
	int iRet;
	tagVideoOutputCfg v_objVideoOutputCfg;

	// ֹͣ���е�Ԥ��ͨ��
	for(i = 2; i <= 3; i++)
	{
		for(j = 0; j <= 1; j++)
		{
			k = i-2+2*j;
			
			if(g_objIdleStart.VPreview[k])
			{
				iRet = HI_MPI_VI_UnBindOutput(i, j, k);
				if(iRet) goto ERROR;

				g_objIdleStart.VPreview[k] = FALSE;
			}
		}
	}

	// �л���Ƶ���ͨ��
	switch(v_bytVvChn)
	{
		// ��ͨ��Ԥ��
		case 0x01:
		case 0x02:
		case 0x04:
		case 0x08:
			v_objVideoOutputCfg.bytVoMode = D1;
			break;
		// ˫ͨ��Ԥ��
		case 0x03:
		case 0x05:
		case 0x06:
		case 0x09:
		case 0x0a:
		case 0x0c:
			v_objVideoOutputCfg.bytVoMode = HD1;
			break;
		// ����ͨ��Ԥ��
		default:
			v_objVideoOutputCfg.bytVoMode = CIF;
			break;
	}
	v_objVideoOutputCfg.bytVoNormal = PAL;
	v_objVideoOutputCfg.bytVoChn = v_bytVvChn;
	iRet = CtrlVideoOutput(v_objVideoOutputCfg);
	if(iRet) goto ERROR;

	// ������Ҫ��Ԥ��ͨ��
	for(i = 2; i <= 3; i++)
	{
		for(j = 0; j <= 1; j++)
		{
			k = i-2+2*j;
			
			if(!g_objIdleStart.VPreview[k])
			{
				if(v_bytVvChn & (0x01 << k))
				{
					iRet = HI_MPI_VI_BindOutput(i, j, k);
					if(iRet) goto ERROR;
	
					g_objIdleStart.VPreview[k] = TRUE;
				}
			}
		}
	}
	
	return 0;

ERROR:
	PRTMSG(MSG_ERR, "ctrl video view failed: 0x%08x\n", iRet);
	return iRet;
	
#endif
}

int StartAPrev()
{
	PRTMSG(MSG_NOR, "Start Audio Preview\n");

	int iRet;
	
#if VEHICLE_TYPE == VEHICLE_M

#if NO_PREV == 0
	int val = 0;
	iRet = ioctl(g_objDevFd.Tw2835, TW2835_AOUT_MIXED, &val);
	if(iRet) goto ERROR;
	
	g_objIdleStart.APreview = TRUE;
#endif
	
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	tw2865_set_audiooutput objAudioOutput;
	
	objAudioOutput.PlaybackOrLoop = 1;
	objAudioOutput.channel = 0;
	iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_AUDIO_OUTPUT, &objAudioOutput);
	if(iRet) goto ERROR;
	
	g_objIdleStart.APreview = TRUE;
#endif

	return 0;

ERROR:
	PRTMSG(MSG_ERR, "start audio preview failed: %08x\n", iRet);
	return iRet;
}

int StopAPrev()
{
	int iRet;
	
	if(g_objIdleStart.APreview)
	{
	  PRTMSG(MSG_NOR, "Stop Audio Preview\n");

#if VEHICLE_TYPE == VEHICLE_M

#if NO_PREV == 0
		int val = 0;
		iRet = ioctl(g_objDevFd.Tw2835, TW2835_AOUT_PB, &val);
		if(iRet) goto ERROR;
		
		g_objIdleStart.APreview = FALSE;
#endif
		
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
		tw2865_set_audiooutput objAudioOutput;
		
		objAudioOutput.PlaybackOrLoop = 0;
		iRet = ioctl(g_objDevFd.Tw2865,TW2865CMD_AUDIO_OUTPUT, &objAudioOutput);
		if(iRet) goto ERROR;
		
		g_objIdleStart.APreview = FALSE;
#endif

		return 0;
	}
	
	return -1;
	
ERROR:
	PRTMSG(MSG_ERR, "stop audio preview failed: %08x\n", iRet);
	return iRet;
}

void *AVRec(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	int iErrTime[6];
	
	DWORD dwVFileTick[4] = {0};
	DWORD dwAFileTick[4] = {0};
	DWORD dwIndexTick[4] = {0};
	
	BYTE bytLampBuf[2];
	
	BYTE byt = 0;
	
	struct statfs objSDiskBuf, objHDiskBuf;
	
	int	iRet;
	int	i, j, k, m, n;
	
	int iChnNoVi[4] = {0};
	
#if VEHICLE_TYPE == VEHICLE_M
	VENC_CHN iVencChn;
	AENC_STREAM_S  objAencStream;
	VENC_STREAM_S  objVencStream;
	VENC_CONFIG_H264_S 	objVencCfg; 
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	AUDIO_FRAME_S   objAiFrame;
	AUDIO_STREAM_S  objAencStream;
	VENC_CHN_STAT_S objVencStat;
	VENC_STREAM_S  	objVencStream;	
	VENC_ATTR_H264_S objH264Attr;
	VENC_CHN_ATTR_S objChnAttr;
#endif
	
	FILE 	*pAVListFile[4] = {NULL};
	FILE  *pAVIndexFile[4] = {NULL};
	FILE  *pVFile[4] = {NULL};
	FILE  *pAFile[4] = {NULL};
	
	FILE_INDEX objAVFileIndex[4];
	FRAME_INDEX objAVFrameIndex[4];
	HI_U64 lVOffset[4] = {0};
	HI_U64 lAOffset[4] = {0};
	uint uiVWriteLen = 0;
	uint uiAWriteLen = 0;
	
	char  szPathName[255]	= {0};
	char  szAVListName[4][255] = {0};
	char  szAVIndexName[4][255] = {0};
	char  szVFileName[4][255] = {0};
	char  szAFileName[4][255] = {0};	
	
	int iCurDateTime[6];
	int iCrtSec, iCurSec;

	char szbuf[1024] = {0};
	int  ilen = 0;

	iCrtSec = iCurSec = GetCurDateTime(iCurDateTime);
	
#if VEHICLE_TYPE == VEHICLE_M
	iRet = HI_VENC_SetStreamMode(VENC_STREAM_FRAME);
	if(iRet) 
	{
		PRTMSG(MSG_ERR, "set stream mode failed: %08x\n", iRet);
		goto AVENC_RECORD_STOP;
	}
	
	iRet = HI_VENC_SetStreamBufLen((g_objMvrCfg.VInput.VMode==CIF ? (512*1024) : (g_objMvrCfg.VInput.VMode==HD1 ? (2*512*1024) : (4*512*1024))));
	if(iRet) 
	{
		PRTMSG(MSG_ERR, "set stream buf failed: %08x\n", iRet);
		goto AVENC_RECORD_STOP;
	}
	
	for(i=0; i<(g_objMvrCfg.VInput.VMode==CIF ? 4: (g_objMvrCfg.VInput.VMode==HD1 ? 2 : 1)); i++)
	{
		/*������Ƶ����ͨ��*/
		if( g_objMvrCfg.AVRecord.VStart[i] )
		{
#if NO_VENC == 1
			iRet = HI_VENC_CreateCH(i, &iVencChn);
			if(iRet) 
			{
				PRTMSG(MSG_ERR, "start video encode chn%d failed: 0x%08x\n", i+1, iRet);
				goto AVENC_RECORD_STOP;
			}
#endif

			iRet = HI_VENC_GetAttr(2*i, &objVencCfg); 
			if(iRet) 
			{
				PRTMSG(MSG_ERR, "get video record chn%d attr failed: 0x%08x\n", i+1, iRet);
				goto AVENC_RECORD_STOP;
			}
		
			objVencCfg.u32TargetBitrate = g_objMvrCfg.AVRecord.VBitrate[i];
			objVencCfg.u32TargetFramerate = g_objMvrCfg.AVRecord.VFramerate[i];
			objVencCfg.u32PreferFrameRate = 1;
			objVencCfg.u32Gop = 25;

			iRet = HI_VENC_SetAttr(2*i, &objVencCfg); 
			if(iRet) 
			{ 
				PRTMSG(MSG_ERR, "set video record chn%d attr failed: %08x\n", i+1, iRet);
				goto AVENC_RECORD_STOP;
			}

			iRet = HI_VENC_StartCH(2*i, 0);
			if(iRet)
			{
				PRTMSG(MSG_ERR, "start video record chn%d failed: %08x\n", i+1, iRet);
				goto AVENC_RECORD_STOP;
			}

			g_objWorkStart.VRecord[i] = TRUE;
			PRTMSG(MSG_NOR, "start video record chn%d succ\n", i+1);
									
			/*������Ƶ����ͨ��*/
			if( !g_objWorkStart.ARecord[0] && !g_objWorkStart.ARecord[1] && !g_objWorkStart.ARecord[2] && !g_objWorkStart.ARecord[3] )
			{
        iRet = HI_AENC_EnableANR(2, 0);
				if(iRet)
				{
					PRTMSG(MSG_ERR, "enable audio record chn%d ANR failed: %08x\n", i+1, iRet);
					goto AVENC_RECORD_STOP;
				}
				
// 				iRet = HI_AENC_EnableAEC(2, 0);
// 				if(iRet)
// 				{
// 					PRTMSG(MSG_ERR, "enable audio record chn%d AEC failed: %08x\n", i+1, iRet);
// 					goto AVENC_RECORD_STOP;
// 				}

				iRet = HI_AENC_StartCH(2, 0);//��Ƶ������õ������ͨ��
				if(iRet)
				{
					PRTMSG(MSG_ERR, "start audio record chn%d failed: %08x\n", i+1, iRet);
					goto AVENC_RECORD_STOP;
				}

#if CHK_VER == 1
				if( i==0||i==1 )
#endif
				{
					g_objWorkStart.ARecord[i] = TRUE;
					PRTMSG(MSG_NOR, "start audio record chn%d succ\n", i+1);
				}
			}
		}
	}
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	for(i=0; i<4; i++)
	{
		/*������Ƶ����ͨ��*/
		if( g_objMvrCfg.AVRecord.VStart[i] )
		{
			objChnAttr.enType = PT_H264;
			objChnAttr.pValue = (HI_VOID *)&objH264Attr;
			
			objH264Attr.u32Priority = 0;//ͨ�����ȼ�
			objH264Attr.bMainStream = HI_TRUE;//��̬����,����������ʶ(1:������;0:������)
			objH264Attr.bVIField = HI_TRUE;//��̬����,����ͼ���֡����־(1:��;0:֡)
			objH264Attr.bField = HI_FALSE;//��̬����,֡������ģʽ(1:������;0:֡����)
			objH264Attr.bCBR = HI_TRUE;//��̬����,VBR/CBRģʽ(1:CBR;0:VBR)
			objH264Attr.u32PicLevel = 3;//(CBR:���ʲ�����Χ,0~5,0:��SDK������п�������,1~5:��Ӧ���ʲ�����Χ�ֱ�Ϊ10%~50%;VBR:ͼ��������ȼ�,0~5,ֵԽСͼ������Խ��)
			objH264Attr.u32MaxDelay = 10;//��̬����,����ӳ�(��֡Ϊ��λ)
			objH264Attr.bByFrame = HI_TRUE;//��̬����,֡/��ģʽ��ȡ����(1:��֡��ȡ;0:������ȡ)
			objH264Attr.u32TargetFramerate = g_objMvrCfg.AVRecord.VFramerate[i];//��̬����,Ŀ��֡��(P��:0~25;N��:0~30)
			objH264Attr.u32Bitrate = g_objMvrCfg.AVRecord.VBitrate[i]/1000;//��̬����,Ŀ������(CBR)/�������(VBR);16K~20M
			objH264Attr.u32Gop = g_objMvrCfg.AVRecord.VFramerate[i];//25;//��̬����,I֡���(0~1000,��֡Ϊ��λ)

			if(g_objMvrCfg.VInput.VNormal == PAL)
			{
				objH264Attr.u32ViFramerate = 25;//��̬����,VI�����֡��(ԭʼ֡��,P��:0~25;N��:0~30)
					
				switch(g_objMvrCfg.AVRecord.VMode[i])
				{
					case CIF:
						objH264Attr.u32PicWidth = 352;//��̬����,����ͼ����(160~2048)
						objH264Attr.u32PicHeight = 288;//��̬����,����ͼ��߶�(112~1536)
						objH264Attr.u32BufSize = 352*288*2*2;//��̬����,����buffer��С(��СֵΪ���ԭʼͼ�������ڴ��1.5��,��D1Ϊ��,��СֵΪ704*576*1.5*1.5,���ֵ������)
						break;
					case HD1:
						objH264Attr.u32PicWidth = 720;
						objH264Attr.u32PicHeight = 288;
						objH264Attr.u32BufSize = 720*288*2*2;
						break;
					case D1:
						objH264Attr.u32PicWidth = 720;
						objH264Attr.u32PicHeight = 576;
						objH264Attr.u32BufSize = 720*576*2*2;
						break;
					default://Ĭ��CIF
						objH264Attr.u32PicWidth = 352;
						objH264Attr.u32PicHeight = 288;
						objH264Attr.u32BufSize = 352*288*2*2;
						break;
				}
			}
			else
			{
				objH264Attr.u32ViFramerate = 30;

				switch(g_objMvrCfg.AVRecord.VMode[i])
				{
					case CIF:
						objH264Attr.u32PicWidth = 352;
						objH264Attr.u32PicHeight = 240;
						objH264Attr.u32BufSize = 352*240*2*2;
						break;
		      case HD1:
						objH264Attr.u32PicWidth = 720;
						objH264Attr.u32PicHeight = 240;
						objH264Attr.u32BufSize = 720*240*2*2;
						break;
					case D1:
						objH264Attr.u32PicWidth = 720;
						objH264Attr.u32PicHeight = 480;
						objH264Attr.u32BufSize = 720*480*2*2;
						break;
					default: 
						objH264Attr.u32PicWidth = 352;
						objH264Attr.u32PicHeight = 240;
						objH264Attr.u32BufSize = 352*240*2*2;
						break;
				}     
			}
							
			iRet = HI_MPI_VENC_CreateChn(2*i, &objChnAttr, HI_NULL);
			if(iRet)
			{
				PRTMSG(MSG_ERR, "create video record chn%d failed: 0x%08x\n", i+1, iRet);
				goto AVENC_RECORD_STOP;
			}
				
			iRet = HI_MPI_VENC_RegisterChn(2*i, 2*i);
			if(iRet)
			{
				PRTMSG(MSG_ERR, "register video record chn%d failed: 0x%08x\n", i+1, iRet);
				goto AVENC_RECORD_STOP;
			}

			iRet = HI_MPI_VENC_StartRecvPic(2*i);
			if(iRet)
			{
				PRTMSG(MSG_ERR, "start video record chn%d failed: 0x%08x\n", i+1, iRet);
				goto AVENC_RECORD_STOP;
			}

			g_objWorkStart.VRecord[i] = TRUE;
			PRTMSG(MSG_NOR, "start video record chn%d succ\n", i+1);
									
			/*������Ƶ����ͨ��*/
			if( !g_objWorkStart.ARecord[0] && !g_objWorkStart.ARecord[1] && !g_objWorkStart.ARecord[2] && !g_objWorkStart.ARecord[3] )
			{
#if CHK_VER == 1
				if( i==0||i==1 )
#endif
				{
					g_objWorkStart.ARecord[i] = TRUE;
					PRTMSG(MSG_NOR, "start audio record chn%d succ\n", i+1);
				}
			}
		}
	}
#endif

	while(1)
	{
		/*�ϱ�¼��������Ϣ*/
		g_bytRecRptBuf[0] = 0x01;	// 0x01 ��ʾ�����ķ��͵�����֡
		g_bytRecRptBuf[1] = 0x38;
		g_bytRecRptBuf[2] = 0x47;
		g_bytRecRptBuf[3] = iCurDateTime[0]-2000;
		g_bytRecRptBuf[4] = iCurDateTime[1];
		g_bytRecRptBuf[5] = iCurDateTime[2];
		g_bytRecRptBuf[6] = iCurDateTime[3];
		g_bytRecRptBuf[7] = iCurDateTime[4];
		g_bytRecRptBuf[8] = iCurDateTime[5];
		/*��¼����ͷ״̬��Ϣ*/
		g_bytRecRptBuf[11] = 0x00;
		for(i = 0; i < 4; i++)
		{
			if(g_objDevSta.Camera[i])
			{
				g_bytRecRptBuf[11] |= (0x08>>i);
			}
		}
		/*��¼�洢������Ϣ*/
		if(strcmp("SDisk", g_objMvrCfg.AVRecord.DiskType) == 0)
		{
			memset(&objSDiskBuf, 0, sizeof(objSDiskBuf));
			statfs("/mnt/SDisk/part1", &objSDiskBuf);
			g_bytRecRptBuf[12] = 0x00;
			g_bytRecRptBuf[13] = (BYTE)(1-objSDiskBuf.f_bavail/(double)objSDiskBuf.f_blocks)*100;
		}
		else if(strcmp("HDisk", g_objMvrCfg.AVRecord.DiskType) == 0)
		{
			memset(&objHDiskBuf, 0, sizeof(objHDiskBuf));
			statfs("/mnt/HDisk/part1", &objHDiskBuf);
			g_bytRecRptBuf[12] = 0x01;
			g_bytRecRptBuf[13] = (BYTE)(1-objHDiskBuf.f_bavail/(double)objHDiskBuf.f_blocks)*100;
		}	

#if USE_VIDEO_TCP == 0
		DataPush(g_bytRecRptBuf, 14, DEV_DVR, DEV_QIAN, LV2);
#endif

#if USE_VIDEO_TCP == 1
		g_objSock.MakeSmsFrame((char*)g_bytRecRptBuf+3, 11, 0x38, 0x47, (char*)szbuf, sizeof(szbuf), ilen);
		g_objSock.SendTcpData(szbuf, ilen);
#endif
		
		bytLampBuf[0] = 0x02;
		bytLampBuf[1] = 0x03;	// ��ʼ¼��֪ͨ
		DataPush(bytLampBuf, 2, DEV_DVR, DEV_DIAODU, LV2);
	
		for(i = 0; i < 4; i++)
		{
			if(g_objWorkStart.VRecord[i])
			{
				/*�����洢Ŀ¼*/
				sprintf(szPathName, "/mnt/%s/part1/data/chn%d/%04d-%02d-%02d/", g_objMvrCfg.AVRecord.DiskType, i+1, iCurDateTime[0], iCurDateTime[1], iCurDateTime[2]);
				for(k = 22; k < strlen(szPathName); k++) // ��Ҫ����dataĿ¼
				{
					if(szPathName[k] == '/')
					{
						szPathName[k] = 0;
						if(access(szPathName, F_OK) == 0)
						{
							chmod(szPathName, S_IRWXU|S_IRWXG|S_IRWXO);// ��Ŀ¼�ķ���Ȩ������Ϊ�����˿ɶ�����д����ִ��
//							chown(szPathName, 0, 0);// ��Ŀ¼�������ߺ�������Ϊroot
						}
						else
						{   
							if(mkdir(szPathName, 0777) == 0)   
							{   
//								chown(szPathName, 0, 0);
								chmod(szPathName, S_IRWXU|S_IRWXG|S_IRWXO);
							}
							else
							{
								PRTMSG(MSG_ERR, "create chn%d dir failed\n", i+1);
								goto AVENC_RECORD_STOP;
							}
						} 
						szPathName[k] = '/';   
					}   
				}

				/*�����б��ļ�*/
				sprintf(szAVListName[i], "%s%04d-%02d-%02d.list", szPathName, iCurDateTime[0], iCurDateTime[1], iCurDateTime[2]);
				if((pAVListFile[i] = fopen(szAVListName[i], "a+b")) != NULL)
				{
//					chown(szAVListName[i], 0, 0);
					chmod(szAVListName[i], S_IRWXU|S_IRWXG|S_IRWXO);
				}
				else
				{
					PRTMSG(MSG_ERR, "create chn%d list file failed\n", i+1);
					goto AVENC_RECORD_STOP;
				}
				fclose(pAVListFile[i]);
				pAVListFile[i] = NULL;
				
				//������Ƶ�ļ�
				sprintf(szVFileName[i], "%s%02d-%02d-%02d.264", szPathName, iCurDateTime[3], iCurDateTime[4], iCurDateTime[5]);
				strcpy(szVTest[i], szVFileName[i]);
				
				if((pVFile[i] = fopen(szVFileName[i], "w+b")) != NULL )
				{
//					chown(szVFileName[i], 0, 0);
					chmod(szVFileName[i], S_IRWXU|S_IRWXG|S_IRWXO);
				}
				else
				{
					PRTMSG(MSG_ERR, "create chn%d video file failed\n", i+1);
					goto AVENC_RECORD_STOP;
				}
				lVOffset[i] = 0;	
				setvbuf(pVFile[i], (char*)g_objAVRecBuf.VBuf[i], _IOFBF, 256000);
				
				//������Ƶ�ļ�
				if( g_objWorkStart.ARecord[i] )
				{
					sprintf(szAFileName[i], "%s%02d-%02d-%02d.726", szPathName, iCurDateTime[3], iCurDateTime[4], iCurDateTime[5]);
					strcpy(szATest[i], szAFileName[i]);

					if((pAFile[i] = fopen(szAFileName[i], "w+b")) != NULL )
					{
//						chown(szAFileName[i], 0, 0);
						chmod(szAFileName[i], S_IRWXU|S_IRWXG|S_IRWXO);
					}
					else
					{
						PRTMSG(MSG_ERR, "create chn%d audio file failed\n", i+1);
						goto AVENC_RECORD_STOP;
					}
					lAOffset[i] = 0;
					setvbuf(pAFile[i], (char*)g_objAVRecBuf.ABuf[i], _IOFBF, 25600);
				}
				
				//���������ļ�
   			sprintf(szAVIndexName[i], "%s%02d-%02d-%02d.index", szPathName, iCurDateTime[3], iCurDateTime[4], iCurDateTime[5]);
   			strcpy(szITest[i], szAVIndexName[i]);

				if((pAVIndexFile[i] = fopen(szAVIndexName[i], "w+b")) != NULL )
				{
//					chown(szAVIndexName[i], 0, 0);
					chmod(szAVIndexName[i], S_IRWXU|S_IRWXG|S_IRWXO);
				}
				else
				{
					PRTMSG(MSG_ERR, "create chn%d index file failed\n", i+1);
					goto AVENC_RECORD_STOP;
				}
				
				dwVFileTick[i] = dwAFileTick[i] = dwIndexTick[i] = GetTickCount();
				
				/*��¼����Ƶ�ļ���Ϣ*/
				memset(&objAVFileIndex[i], 0, sizeof(FILE_INDEX));
				sprintf(objAVFileIndex[i].EncChn, "%d", i+1);
				sprintf(objAVFileIndex[i].CreatDate[0], "%04d", iCurDateTime[0]);
				sprintf(objAVFileIndex[i].CreatDate[1], "%02d", iCurDateTime[1]);
				sprintf(objAVFileIndex[i].CreatDate[2], "%02d", iCurDateTime[2]);
				sprintf(objAVFileIndex[i].StartTime[0], "%02d", iCurDateTime[3]);
				sprintf(objAVFileIndex[i].StartTime[1], "%02d", iCurDateTime[4]);
				sprintf(objAVFileIndex[i].StartTime[2], "%02d", iCurDateTime[5]);
#if VEHICLE_TYPE == VEHICLE_M
				sprintf(objAVFileIndex[i].Mode, "%s", g_objMvrCfg.VInput.VMode == CIF ? "CIF" : (g_objMvrCfg.VInput.VMode == HD1 ? "HD1" : (g_objMvrCfg.VInput.VMode == D1 ? "D1 " : "???")));
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
				sprintf(objAVFileIndex[i].Mode, "%s", g_objMvrCfg.AVRecord.VMode[i] == CIF ? "CIF" : (g_objMvrCfg.AVRecord.VMode[i] == HD1 ? "HD1" : (g_objMvrCfg.AVRecord.VMode[i] == D1 ? "D1 " : "???")));
#endif
				sprintf(objAVFileIndex[i].Normal, "%s", g_objMvrCfg.VInput.VNormal == PAL ? "PAL" : "NTSC");
				sprintf(objAVFileIndex[i].Bitrate, "%7d", g_objMvrCfg.AVRecord.VBitrate[i]);
				sprintf(objAVFileIndex[i].Framerate, "%2d", g_objMvrCfg.AVRecord.VFramerate[i]);
				sprintf(objAVFileIndex[i].VPath, "%s", szVFileName[i]+22);//��¼��·����chn��ʼ
				sprintf(objAVFileIndex[i].APath, "%s", szAFileName[i]+22);
				sprintf(objAVFileIndex[i].IPath, "%s", szAVIndexName[i]+22);
			}
		}
	  
		while(1)
		{
			for(i = 0; i < 4; i++)
			{
				if( g_objWorkStart.VRecord[i] )
				{
	        /*��¼����Ƶ֡��Ϣ*/
					memset(&objAVFrameIndex[i], 0, sizeof(FRAME_INDEX));
					sprintf(objAVFrameIndex[i].CurTime, "%02d:%02d:%02d",	iCurDateTime[3], iCurDateTime[4], iCurDateTime[5]);
					objAVFrameIndex[i].AStartOffset = lAOffset[i];
					objAVFrameIndex[i].VStartOffset = lVOffset[i];

#if VEHICLE_TYPE == VEHICLE_M
					/*������ʽ��ȡ��Ƶ֡*/
					iRet = HI_VENC_GetStream(2*i, &objVencStream, HI_TRUE);
					if(iRet)
					{
						PRTMSG(MSG_ERR, "get record chn%d video stream failed: %08x\n", i+1, iRet);
		        goto AVENC_RECORD_STOP;
					}
		
					/*������Ƶ֡���ļ���*/			
					for(m=0; m<objVencStream.u32DataNum; m++)
					{
						uiVWriteLen = fwrite(objVencStream.struData[m].pAddr, objVencStream.struData[m].u32Len, 1, pVFile[i]);					
						lVOffset[i] += uiVWriteLen * objVencStream.struData[m].u32Len;
							
						if(uiVWriteLen != 1)
						{
							PRTMSG(MSG_ERR, "write record chn%d video file failed\n", i+1);
							goto AVENC_RECORD_STOP;
						}
					}
	        	
					objAVFrameIndex[i].VEndOffset = lVOffset[i];
	        objAVFrameIndex[i].IFrameFlag = (BOOL)objVencStream.struDataInfo.u16IFrameFlag;
	        	
	        /*�ͷ���Ƶ֡����*/
					iRet = HI_VENC_ReleaseStream(2*i, &objVencStream);            
					if(iRet)
					{
						PRTMSG(MSG_ERR, "release record chn%d video stream failed: %08x\n", i+1, iRet);
						goto AVENC_RECORD_STOP;
					}
	
					/*��¼��Ƶ�ļ���Ϣ*/
					sprintf(objAVFileIndex[i].VSize, "%-3d", lVOffset[i]/1048576);
					
					/*������Ƶ֡���ļ���*/
	        if( g_objWorkStart.ARecord[i] )
	        {
	        	for(j=0; j<5; j++)//ÿ�ɼ�һ֡��Ƶ��ͬʱ�ɼ���֡��Ƶ
	        	{
		        	/*������ʽ��ȡ��Ƶ֡*/
#if CHK_VER == 1
							iRet = HI_AENC_GetStream(i+2, &objAencStream);
#else
							iRet = HI_AENC_GetStream(2, &objAencStream);
#endif
							if(iRet)
							{
								PRTMSG(MSG_ERR, "get record chn%d audio stream failed: %08x\n", i+1, iRet);
			          goto AVENC_RECORD_STOP;
							}
								
		          uiAWriteLen = fwrite(objAencStream.ps16Addr, objAencStream.u32Len, 1, pAFile[i]);
		          lAOffset[i] += uiAWriteLen * objAencStream.u32Len;
		            
		          if(uiAWriteLen != 1)
							{
								PRTMSG(MSG_ERR, "write record chn%d audio file failed\n", i+1);
								goto AVENC_RECORD_STOP;
							}
		        	
							objAVFrameIndex[i].AEndOffset = lAOffset[i];
							
							/*�ͷ���Ƶ֡����*/
							iRet = HI_AENC_ReleaseStream(2, &objAencStream);            
							if(iRet)
							{
								PRTMSG(MSG_ERR, "release record chn%d audio stream failed: %08x\n", i+1, iRet);
								goto AVENC_RECORD_STOP;
							}
						}
	
						/*��¼��Ƶ�ļ���Ϣ*/
						sprintf(objAVFileIndex[i].ASize, "%3d", lAOffset[i]/1048576);
	        }
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
					/*������ʽ��ȡ��Ƶ֡*/
					memset(&objVencStat, 0, sizeof(objVencStat));

					iRet = HI_MPI_VENC_Query(2*i, &objVencStat);
					if(iRet) 
					{
						PRTMSG(MSG_ERR, "query record chn%d video stream failed: 0x%08x\n", i+1, iRet);
		        goto AVENC_RECORD_STOP;
					}

					if( !objVencStat.u32CurPacks )
					{
						iChnNoVi[i]++;
						
						//PRTMSG(MSG_ERR, "chn%d no video input\n", i+1);
						
						if(iChnNoVi[i] >= 25)
						{
							char cDevErr = 0x01;
							DWORD dwPacketNum = 0;
							g_objDevErrMng.PushData( 1, 1, &cDevErr, dwPacketNum);
							
							PRTMSG(MSG_ERR, "chn%d no video input for 25 frames, reboot tw2865\n", i+1);
			        goto AVENC_RECORD_STOP;
						}
						else
						{
							msleep(20);
							continue;
						}
					}
					else
					{
						iChnNoVi[i] = 0;
						
						//PRTMSG(MSG_ERR, "chn%d have video input\n", i+1);
					}
						
					memset(&objVencStream, 0, sizeof(objVencStream));
					objVencStream.u32PackCount = objVencStat.u32CurPacks;
					objVencStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S)*objVencStat.u32CurPacks);
					if(objVencStream.pstPack==NULL)
					{
						PRTMSG(MSG_ERR, "malloc stream pack err\n");
		        goto AVENC_RECORD_STOP;
					}

					iRet = HI_MPI_VENC_GetStream(2*i, &objVencStream, HI_IO_BLOCK);
					if(iRet)
					{
						PRTMSG(MSG_ERR, "get record chn%d video stream failed: 0x%08x\n", i+1, iRet);
		        free(objVencStream.pstPack);
		        goto AVENC_RECORD_STOP;
					}
					
					for( m=0; m<objVencStream.u32PackCount; m++ )
					{
						uiVWriteLen = fwrite(objVencStream.pstPack[m].pu8Addr[0], objVencStream.pstPack[m].u32Len[0], 1, pVFile[i]);					
						lVOffset[i] += uiVWriteLen * objVencStream.pstPack[m].u32Len[0];
						if(objVencStream.pstPack[m].u32Len[1] > 0)
						{
							uiVWriteLen = fwrite(objVencStream.pstPack[m].pu8Addr[1], objVencStream.pstPack[m].u32Len[1], 1, pVFile[i]);					
							lVOffset[i] += uiVWriteLen * objVencStream.pstPack[m].u32Len[1];
						}
						if(uiVWriteLen != 1)
						{
							GetCurDateTime( iErrTime );		
		  				PRTMSG(MSG_DBG, "д��Ƶ����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d\n", iErrTime[0], iErrTime[1], iErrTime[2], iErrTime[3], iErrTime[4], iErrTime[5]);
		  
							PRTMSG(MSG_ERR, "write record chn%d video file failed\n", i+1);
							free(objVencStream.pstPack);
							goto AVENC_RECORD_STOP;
						}
					}
					
					if(FileWritable(szVFileName[i], dwVFileTick[i]) == false)
					{
						PRTMSG(MSG_ERR, "Video file size is zero in 5 minutes\n");
						free(objVencStream.pstPack);
						goto AVENC_RECORD_STOP;
					}
					
					objAVFrameIndex[i].VEndOffset = lVOffset[i];
	        objAVFrameIndex[i].IFrameFlag = objVencStream.pstPack[0].DataType.enH264EType==H264E_NALU_PSLICE?FALSE:TRUE;
					
					/*�ͷ���Ƶ֡����*/
					iRet = HI_MPI_VENC_ReleaseStream(2*i, &objVencStream);            
					if(iRet)
					{
						PRTMSG(MSG_ERR, "release record chn%d video stream failed: %08x\n", i+1, iRet);
						free(objVencStream.pstPack);
						goto AVENC_RECORD_STOP;
					}
					
					free(objVencStream.pstPack);
					
					/*��¼��Ƶ�ļ���Ϣ*/
					sprintf(objAVFileIndex[i].VSize, "%-3d", lVOffset[i]/1048576);
					
					if( g_objWorkStart.ARecord[i] )
					{
						for(j=0; j<5; j++)//ÿ����һ֡��Ƶʱ��Ӧ������֡��Ƶ
						{
							//������ʽ��ȡ��Ƶ֡
#if CHK_VER == 1
							iRet = HI_MPI_AI_GetFrame(1, i, &objAiFrame, NULL, HI_IO_NOBLOCK);
#else
							iRet = HI_MPI_AI_GetFrame(1, 0, &objAiFrame, NULL, HI_IO_NOBLOCK);
#endif
							if(iRet) continue;
//							{
//								PRTMSG(MSG_ERR, "get audio frame from ai failed: 0x%08x\n", iRet);
//								goto AVENC_RECORD_STOP;
//							}
							
			        iRet = HI_MPI_AENC_SendFrame(0, &objAiFrame, NULL);//������ñ���ͨ��0
			        if(iRet)
							{
								PRTMSG(MSG_ERR, "send audio frame to aenc failed: 0x%08x\n", iRet);
								goto AVENC_RECORD_STOP;
							}
							
			        iRet = HI_MPI_AENC_GetStream(0, &objAencStream);
			        if(iRet)
							{
								PRTMSG(MSG_ERR, "get stream from aenc failed: 0x%08x\n", iRet);
								goto AVENC_RECORD_STOP;
							}
							
							uiAWriteLen = fwrite(objAencStream.pStream, objAencStream.u32Len, 1, pAFile[i]);
		          lAOffset[i] += uiAWriteLen * objAencStream.u32Len;
		            
		          if(uiAWriteLen != 1)
							{
								GetCurDateTime( iErrTime );		
			  				PRTMSG(MSG_DBG, "д��Ƶ����ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d\n", iErrTime[0], iErrTime[1], iErrTime[2], iErrTime[3], iErrTime[4], iErrTime[5]);
			  
								PRTMSG(MSG_ERR, "write record chn%d audio file failed\n", i+1);
								goto AVENC_RECORD_STOP;
							}
							
							if(FileWritable(szAFileName[i], dwAFileTick[i]) == false)
							{
								PRTMSG(MSG_ERR, "Audio file size is zero in 5 minutes\n");
								goto AVENC_RECORD_STOP;
							}
		        	
							objAVFrameIndex[i].AEndOffset = lAOffset[i];
							
							//�ͷ���Ƶ֡����
							iRet = HI_MPI_AENC_ReleaseStream(0, &objAencStream);            
							if(iRet)
							{
								PRTMSG(MSG_ERR, "release record chn%d audio stream failed: 0x%08x\n", i+1, iRet);
								goto AVENC_RECORD_STOP;
							}
						}
					}
#endif
	        if(1 != fwrite(&objAVFrameIndex[i], sizeof(FRAME_INDEX), 1, pAVIndexFile[i]))
	        {
						GetCurDateTime( iErrTime );		
			  		PRTMSG(MSG_DBG, "д��������ʱ�䣺%04d-%02d-%02d %02d:%02d:%02d\n", iErrTime[0], iErrTime[1], iErrTime[2], iErrTime[3], iErrTime[4], iErrTime[5]);
			  
	        	PRTMSG(MSG_ERR, "write record chn%d index file failed\n", i+1);
	        	goto AVENC_RECORD_STOP;
	        }
	        
	        if(FileWritable(szAVIndexName[i], dwIndexTick[i]) == false)
					{
						PRTMSG(MSG_ERR, "Index file size is zero in 5 minutes\n");
						goto AVENC_RECORD_STOP;
					}
	        
					sprintf(objAVFileIndex[i].EndTime[0], "%02d", iCurDateTime[3]);
					sprintf(objAVFileIndex[i].EndTime[1], "%02d", iCurDateTime[4]);
					sprintf(objAVFileIndex[i].EndTime[2], "%02d", iCurDateTime[5]);
				}
			}

			if( g_objWorkStop.AVRecord )
			{
				AVENC_RECORD_STOP:

				for(i = 0; i < 4; i++)
				{
					if( g_objWorkStart.VRecord[i] )
					{
#if VEHICLE_TYPE == VEHICLE_M
						iRet = HI_VENC_StopCH(2*i); 
						
#if NO_VENC == 1
						iRet = HI_VENC_DestroyCH(2*i, HI_TRUE);
#endif

#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
						iRet = HI_MPI_VENC_StopRecvPic(2*i);
						iRet = HI_MPI_VENC_UnRegisterChn(2*i);
						iRet = HI_MPI_VENC_DestroyChn(2*i);
#endif
						if(iRet)
						{
							PRTMSG(MSG_ERR, "stop video record chn%d failed: 0x%08x\n", i+1, iRet);
						}

						if(pVFile[i])    
						{
							fclose(pVFile[i]);
							pVFile[i] = NULL;
						}
						g_objWorkStart.VRecord[i] = FALSE;

						if( g_objWorkStart.ARecord[i] )
						{
#if VEHICLE_TYPE == VEHICLE_M
							iRet = HI_AENC_StopCH(2); 
#endif
							if(iRet)
							{
								PRTMSG(MSG_ERR, "stop audio record chn%d failed: 0x%08x\n", i+1, iRet);
							}

							if(pAFile[i])    
							{
								fclose(pAFile[i]);
								pAFile[i] = NULL;
							}
							g_objWorkStart.ARecord[i] = FALSE;
						}
						
						if(pAVIndexFile[i])    
						{
							fclose(pAVIndexFile[i]);
							pAVIndexFile[i] = NULL;    
						}
						
						if( strlen(objAVFileIndex[i].EndTime[0]) && strlen(objAVFileIndex[i].EndTime[1]) && strlen(objAVFileIndex[i].EndTime[2]) )
						{
							FlushAVList(szAVListName[i], &objAVFileIndex[i], NULL, 1);
						}
					}
				}

				if( !g_objWorkStart.ARecord[0]&&!g_objWorkStart.ARecord[1]&&!g_objWorkStart.ARecord[2]&&!g_objWorkStart.ARecord[3] 
					&&!g_objWorkStart.VRecord[0]&&!g_objWorkStart.VRecord[1]&&!g_objWorkStart.VRecord[2]&&!g_objWorkStart.VRecord[3] )
				{
					g_bytRecRptBuf[0] = 0x01;	// 0x01 ��ʾ�����ķ��͵�����֡

					g_bytRecRptBuf[1] = 0x38;
					g_bytRecRptBuf[2] = 0x48;
					/*��¼¼��ֹͣʱ��*/
					g_bytRecRptBuf[9] = iCurDateTime[0]-2000;
					g_bytRecRptBuf[10] = iCurDateTime[1];
					g_bytRecRptBuf[11] = iCurDateTime[2];
					g_bytRecRptBuf[12] = iCurDateTime[3];
					g_bytRecRptBuf[13] = iCurDateTime[4];
					g_bytRecRptBuf[14] = iCurDateTime[5];
	
					sprintf((char*)g_bytRecRptBuf+16, "%02d-%02d-%02d.264", iCurDateTime[3], iCurDateTime[4], iCurDateTime[5]);
					byt = 28;
					for(i = 0; i < 4; i++)
					{
						g_bytRecRptBuf[byt++] = lVOffset[i]/16777216;
						g_bytRecRptBuf[byt++] = (lVOffset[i]-g_bytRecRptBuf[byt-1]*16777216)/65536;
						g_bytRecRptBuf[byt++] = (lVOffset[i]-g_bytRecRptBuf[byt-2]*16777216-g_bytRecRptBuf[byt-1]*65536)/256;
						g_bytRecRptBuf[byt++] = (lVOffset[i]-g_bytRecRptBuf[byt-3]*16777216-g_bytRecRptBuf[byt-2]*65536-g_bytRecRptBuf[byt-1]*256);
					}
					
					if(g_bytRecRptBuf[15] == 0x00)//����ֹͣ
					{
#if USE_VIDEO_TCP == 0
						DataPush(g_bytRecRptBuf, 15, DEV_DVR, DEV_QIAN, LV2);
#endif
#if USE_VIDEO_TCP == 1
						g_objSock.MakeSmsFrame((char*)g_bytRecRptBuf+3, 12, 0x38, 0x48, (char*)szbuf, sizeof(szbuf), ilen);
						g_objSock.SendTcpData(szbuf, ilen);
#endif
					}
					else
					{
#if USE_VIDEO_TCP == 0
						DataPush(g_bytRecRptBuf, byt, DEV_DVR, DEV_QIAN, LV2);
#endif

#if USE_VIDEO_TCP == 1
						g_objSock.MakeSmsFrame((char*)g_bytRecRptBuf+3, 12, 0x38, 0x48, (char*)szbuf, sizeof(szbuf), ilen);
						g_objSock.SendTcpData(szbuf, ilen);
#endif
					}

					memset(g_bytRecRptBuf, 0, sizeof(g_bytRecRptBuf));
					
					bytLampBuf[0] = 0x02;
					bytLampBuf[1] = 0x04;	// ֹͣ¼��֪ͨ
					DataPush(bytLampBuf, 2, DEV_DVR, DEV_DIAODU, LV2);

					g_objWorkStop.AVRecord = FALSE;
					pthread_exit(0);
				}
			}
		  
		  iCurSec = GetCurDateTime(iCurDateTime); 
			
			/*ÿ��Сʱ���һ����Ƶ�ļ�*/
			if(iCurSec - iCrtSec >= PACKINT)
			{
				for(i = 0; i < 4; i++)
				{
					if(pVFile[i])
					{
						fclose(pVFile[i]);
						pVFile[i] = NULL;
#if VEHICLE_TYPE == VEHICLE_M
						HI_VENC_RequestIFrame(2*i);
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
						HI_MPI_VENC_RequestIDR(2*i);
#endif
					}
					
					if(pAFile[i])
					{
						fclose(pAFile[i]);
						pAFile[i] = NULL;         
					}
					
					if(pAVIndexFile[i])    
					{
						fclose(pAVIndexFile[i]);
						pAVIndexFile[i] = NULL;      
					}
					
					FlushAVList(szAVListName[i], &objAVFileIndex[i], NULL, 1);
				}

				g_bytRecRptBuf[0] = 0x01;		// 0x01��ʾ�����ķ��͵�����֡

				g_bytRecRptBuf[1]  = 0x38;
				g_bytRecRptBuf[2]  = 0x48;
				/*��¼¼��ֹͣʱ��*/
				g_bytRecRptBuf[9]  = iCurDateTime[0]-2000;
				g_bytRecRptBuf[10] = iCurDateTime[1];
				g_bytRecRptBuf[11] = iCurDateTime[2];
				g_bytRecRptBuf[12] = iCurDateTime[3];
				g_bytRecRptBuf[13] = iCurDateTime[4];
				g_bytRecRptBuf[14] = iCurDateTime[5];
				g_bytRecRptBuf[15] = 0x01;
				sprintf((char*)g_bytRecRptBuf+16, "%02d-%02d-%02d.264", iCurDateTime[3], iCurDateTime[4], iCurDateTime[5]);
				byt = 28;
				for(i = 0; i < 4; i++)
				{
					g_bytRecRptBuf[byt++] = lVOffset[i]/16777216;
					g_bytRecRptBuf[byt++] = (lVOffset[i]-g_bytRecRptBuf[byt-1]*16777216)/65536;
					g_bytRecRptBuf[byt++] = (lVOffset[i]-g_bytRecRptBuf[byt-2]*16777216-g_bytRecRptBuf[byt-1]*65536)/256;
					g_bytRecRptBuf[byt++] = (lVOffset[i]-g_bytRecRptBuf[byt-3]*16777216-g_bytRecRptBuf[byt-2]*65536-g_bytRecRptBuf[byt-1]*256);
				}
#if USE_VIDEO_TCP == 0
				DataPush(g_bytRecRptBuf, byt, DEV_DVR, DEV_QIAN, LV2);
#endif
#if USE_VIDEO_TCP == 1
				g_objSock.MakeSmsFrame((char*)g_bytRecRptBuf+3, byt-3, 0x38, 0x48, (char*)szbuf, sizeof(szbuf), ilen);
				g_objSock.SendTcpData(szbuf, ilen);
#endif

				memset(g_bytRecRptBuf, 0, sizeof(g_bytRecRptBuf));
				
				iCrtSec = iCurSec;
				break;  
			}
		} 					 
	}
}

int StartAVRec()
{
	if(( !g_objWorkStart.ARecord[0] && !g_objWorkStart.ARecord[1] && !g_objWorkStart.ARecord[2] && !g_objWorkStart.ARecord[3] 
		 &&!g_objWorkStart.VRecord[0] && !g_objWorkStart.VRecord[1] && !g_objWorkStart.VRecord[2] && !g_objWorkStart.VRecord[3] ) 
	 &&( g_objMvrCfg.AVRecord.VStart[0] || g_objMvrCfg.AVRecord.VStart[1] || g_objMvrCfg.AVRecord.VStart[2] || g_objMvrCfg.AVRecord.VStart[3] ))
	{
		PRTMSG(MSG_NOR, "Start AV Record\n");
		
		int iRet = pthread_create(&g_pthreadRec, NULL, AVRec, (void*)(&g_objMvrCfg.AVRecord));
		if(iRet) 
		{
			PRTMSG(MSG_ERR, "start av record failed: 0x%08x\n", iRet);
			return -1;
		}
		
		sleep(3);//��ʱ����ȷ��¼���̳߳ɹ�����
		return 0;
	}

	return -1;
}

int StopAVRec()
{
	if( g_objWorkStart.ARecord[0]||g_objWorkStart.ARecord[1]||g_objWorkStart.ARecord[2]||g_objWorkStart.ARecord[3] 
		||g_objWorkStart.VRecord[0]||g_objWorkStart.VRecord[1]||g_objWorkStart.VRecord[2]||g_objWorkStart.VRecord[3] )
	{
		PRTMSG(MSG_NOR, "Stop AV Record\n");
		
		g_objWorkStop.AVRecord = TRUE;
		pthread_join(g_pthreadRec, NULL);
		
		PRTMSG(MSG_NOR, "stop av record succ\n");
		return 0;
	}
	
	return -1;
}

int SwitchAVRec(int v_iAVRecChn)
{
	static struct tag2DAVRecPar objAVRecord = g_objMvrCfg.AVRecord;
	
	int i;
	
	if(v_iAVRecChn==-1)
	{
		g_objMvrCfg.AVRecord = objAVRecord;
	}
	else
	{
		for(i=0; i<=3; i++)
		{
			if(v_iAVRecChn==i)
				g_objMvrCfg.AVRecord.VStart[i] = TRUE;
			else
				g_objMvrCfg.AVRecord.VStart[i] = FALSE;
		}
	}
}

void *AVUpl(void *arg)
{
	int	iRet;
	int	i, j;
	char szFrameBuf[100*1024] = {0};
	int  iFrameLen = 0;
	DWORD dwPacketNum = 0;

	char szSendBuf[1024] = {0};
	int  iSendlen = 0;

#if VEHICLE_TYPE == VEHICLE_M
	VENC_CHN iVencChn;
	AENC_STREAM_S  			objAencStream;
	VENC_STREAM_S  			objVencStream;	
	VENC_CONFIG_H264_S 	objVencCfg; 
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
	AUDIO_FRAME_S   objAiFrame;
	AUDIO_STREAM_S  objAencStream;
	VENC_CHN_STAT_S objVencStat;
	VENC_STREAM_S  	objVencStream;	
	VENC_ATTR_H264_S objH264Attr;
	VENC_CHN_ATTR_S objChnAttr;
#endif
  	  
	while(1)
	{				
		for(i=0; i<4; i ++)
		{
#if VEHICLE_TYPE == VEHICLE_M
			if(g_objMvrCfg.AVUpload.VStart[i] && !g_objWorkStart.VUpload[i])
			{
#if NO_VENC == 1
//				if( !g_objWorkStart.VRecord[i] )
//				{
//					PRTMSG(MSG_ERR, "video record chn%d not start\n", i+1);
//					goto AVENC_UPLOAD_STOP;
//				}

				iRet = HI_VENC_CreateCH(i, &iVencChn);
				if(iRet) 
				{
					PRTMSG(MSG_ERR, "start video upload chn%d failed: 0x%08x\n", i+1, iRet);
					goto AVENC_UPLOAD_STOP;
				}
#endif
				iRet = HI_VENC_GetAttr(2*i+1, &objVencCfg); 
				if(iRet) 
				{
					PRTMSG(MSG_ERR, "get video upload chn%d attr failed: %08x\n", i+1, iRet);
					goto AVENC_UPLOAD_STOP;
				}
		
				if(g_objMvrCfg.VInput.VNormal == PAL)
				{
					switch(g_objMvrCfg.AVUpload.VMode)
					{
						case QCIF:
							objVencCfg.u32PicWidth = 176; 
							objVencCfg.u32PicHeight = 144; 
							objVencCfg.u32TargetBitrate = g_objMvrCfg.AVUpload.VBitrate[i]; 
							objVencCfg.u32TargetFramerate = g_objMvrCfg.AVUpload.VFramerate[i];
							objVencCfg.u32CapSel = (i >= 2) ? VI_CAP_BOTTOM : VI_CAP_TOP;
							objVencCfg.u32Gop = 250;
						break;
					
						case CIF:
							objVencCfg.u32PicWidth = 352; 
							objVencCfg.u32PicHeight = 288; 
							objVencCfg.u32TargetBitrate = g_objMvrCfg.AVUpload.VBitrate[i]; 
							objVencCfg.u32TargetFramerate = g_objMvrCfg.AVUpload.VFramerate[i];
							objVencCfg.u32CapSel = (i >= 2) ? VI_CAP_BOTTOM : VI_CAP_TOP;
							objVencCfg.u32Gop = 250;
						break;
						
						case HD1:
							objVencCfg.u32PicWidth = 720; 
							objVencCfg.u32PicHeight = 288; 
							objVencCfg.u32TargetBitrate = g_objMvrCfg.AVUpload.VBitrate[i]; 
							objVencCfg.u32TargetFramerate = g_objMvrCfg.AVUpload.VFramerate[i];
							objVencCfg.u32CapSel = VI_CAP_BOTH;
							objVencCfg.u32Gop = 250;
						break;
		            
						default: 
						break;
					}
				}
				else
				{
					switch(g_objMvrCfg.AVUpload.VMode)
					{
						case QCIF:
							objVencCfg.u32PicWidth = 176; 
							objVencCfg.u32PicHeight = 120; 
							objVencCfg.u32TargetBitrate = g_objMvrCfg.AVUpload.VBitrate[i]; 
							objVencCfg.u32TargetFramerate = g_objMvrCfg.AVUpload.VFramerate[i];
							objVencCfg.u32CapSel = (i >= 2) ? VI_CAP_BOTTOM : VI_CAP_TOP;
							objVencCfg.u32Gop = 250;
						break;
						
						case CIF:
							objVencCfg.u32PicWidth = 352; 
							objVencCfg.u32PicHeight = 240; 
							objVencCfg.u32TargetBitrate = g_objMvrCfg.AVUpload.VBitrate[i]; 
							objVencCfg.u32TargetFramerate = g_objMvrCfg.AVUpload.VFramerate[i];
							objVencCfg.u32CapSel = (i >= 2) ? VI_CAP_BOTTOM : VI_CAP_TOP;
							objVencCfg.u32Gop = 250;
						break;
		            
		        case HD1:
							objVencCfg.u32PicWidth = 720; 
							objVencCfg.u32PicHeight = 240; 
							objVencCfg.u32TargetBitrate = g_objMvrCfg.AVUpload.VBitrate[i]; 
							objVencCfg.u32TargetFramerate = g_objMvrCfg.AVUpload.VFramerate[i];
							objVencCfg.u32CapSel = VI_CAP_BOTH;
							objVencCfg.u32Gop = 250;
						break;
						
						default: 
						break;
					}     
				}
		
				iRet = HI_VENC_SetAttr(2*i+1, &objVencCfg); 
				if(iRet) 
				{ 
					PRTMSG(MSG_ERR, "set video upload chn %d attr failed: %08x\n", i+1, iRet);
					goto AVENC_UPLOAD_STOP;
				}						
		
				iRet = HI_VENC_StartCH(2*i+1, 0);
				if(iRet)
				{
					PRTMSG(MSG_ERR, "start video upload chn %d failed: %08x\n", i+1, iRet);
					goto AVENC_UPLOAD_STOP;
				}
				
				g_objWorkStart.VUpload[i] = TRUE;
				PRTMSG(MSG_NOR, "start video upload chn%d succ\n", i+1);
			
				/*������Ƶ����ͨ��*/
				if( !g_objWorkStart.AUpload[0] && !g_objWorkStart.AUpload[1] && !g_objWorkStart.AUpload[2] && !g_objWorkStart.AUpload[3] )
				{
					iRet = HI_AENC_EnableANR(3, 0);
					if(iRet)
					{
						PRTMSG(MSG_ERR, "enable audio upload chn%d ANR failed: %08x\n", i+1, iRet);
						goto AVENC_UPLOAD_STOP;
					}
					
// 					iRet = HI_AENC_EnableAEC(3, 0);
// 					if(iRet)
// 					{
// 						PRTMSG(MSG_ERR, "enable audio upload chn%d AEC failed: %08x\n", i+1, iRet);
// 						goto AVENC_UPLOAD_STOP;
// 					}
				
					iRet = HI_AENC_StartCH(3, 0);
					if(iRet)
					{
						PRTMSG(MSG_ERR, "start audio upload chn%d failed: %08x\n", i+1, iRet);
						goto AVENC_UPLOAD_STOP;
					}
					
					g_objWorkStart.AUpload[i] = TRUE;
					PRTMSG(MSG_NOR, "start audio upload chn%d succ\n", i+1);
				}
			}
			
			if( g_objWorkStart.VUpload[i] )
			{	
				/*������ʽ��ȡ��Ƶ֡*/
				iRet = HI_VENC_GetStream(2*i+1, &objVencStream, HI_TRUE);
				if(iRet)
				{
					PRTMSG(MSG_ERR, "get upload chn%d video stream failed: %08x\n", i+1, iRet);
					goto AVENC_UPLOAD_STOP;
				}
				
// 				for( j = 0; j < objVencStream.u32DataNum; j ++ )
// 				{   
// 					WriteBuf(i, (BYTE*)objVencStream.struData[j].pAddr, objVencStream.struData[j].u32Len, 'V');
// 				}

				iFrameLen = 0;
				for( j = 0; j < objVencStream.u32DataNum; j ++ )
				{   
					memcpy(szFrameBuf+iFrameLen, (BYTE*)objVencStream.struData[j].pAddr, objVencStream.struData[j].u32Len);
					iFrameLen += objVencStream.struData[j].u32Len;
				}
				g_objVideoDataMng[i].PushData(LV2, (DWORD)iFrameLen, szFrameBuf, dwPacketNum);

				
				/*�ͷ���Ƶ֡����*/
				iRet = HI_VENC_ReleaseStream(2*i+1, &objVencStream);            
				if(iRet)
				{
					PRTMSG(MSG_ERR, "release upload chn%d video stream failed: %08x\n", i+1, iRet);
					goto AVENC_UPLOAD_STOP;
				}
				
				if( g_objWorkStart.AUpload[i] )
				{
					iFrameLen = 0;

					for(j = 0; j < 5; j++)
					{
						/*������ʽ��ȡ��Ƶ֡*/
						iRet = HI_AENC_GetStream(3, &objAencStream);
						if(iRet)
						{
							PRTMSG(MSG_ERR, "get upload chn%d audio stream failed: %08x\n", i+1, iRet);
							goto AVENC_UPLOAD_STOP;
						}
						
						//WriteBuf(i, (BYTE*)objAencStream.ps16Addr, objAencStream.u32Len, 'A');
						memcpy(szFrameBuf+iFrameLen, (BYTE*)objAencStream.ps16Addr, objAencStream.u32Len);
						iFrameLen += objAencStream.u32Len;
						
						/*�ͷ���Ƶ֡����*/
						iRet = HI_AENC_ReleaseStream(3, &objAencStream);            
						if(iRet)
						{
							PRTMSG(MSG_ERR, "release upload chn%d audio stream failed: %08x\n", i+1, iRet);
							goto AVENC_UPLOAD_STOP;
						}
					}

					g_objAudioDataMng[i].PushData(LV2, (DWORD)iFrameLen, szFrameBuf, dwPacketNum);
				}
			}
#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
			if(g_objMvrCfg.AVUpload.VStart[i] && !g_objWorkStart.VUpload[i])
			{
				PRTMSG(MSG_DBG, "������Ƶ�ϴ�ͨ��%d\n", i+1);
				
//				if( !g_objWorkStart.VRecord[i] )
//				{
//					PRTMSG(MSG_ERR, "video record chn%d not start\n", i+1);
//					goto AVENC_UPLOAD_STOP;
//				}

				objChnAttr.enType = PT_H264;
				objChnAttr.pValue = (HI_VOID *)&objH264Attr;
				
				objH264Attr.u32Priority = 0;//ͨ�����ȼ�
				objH264Attr.bMainStream = HI_TRUE;//��̬����,����������ʶ(1:������;0:������)
				objH264Attr.bVIField = HI_TRUE;//��̬����,����ͼ���֡����־(1:��;0:֡)
				objH264Attr.bField = HI_FALSE;//��̬����,֡������ģʽ(1:������;0:֡����)
				objH264Attr.bCBR = HI_TRUE;//��̬����,VBR/CBRģʽ(1:CBR;0:VBR)
				objH264Attr.u32PicLevel = 3;//(CBR:���ʲ�����Χ,0~5,0:��SDK������п�������,1~5:��Ӧ���ʲ�����Χ�ֱ�Ϊ10%~50%;VBR:ͼ��������ȼ�,0~5,ֵԽСͼ������Խ��)
				objH264Attr.u32MaxDelay = 10;//��̬����,����ӳ�(��֡Ϊ��λ)
				objH264Attr.bByFrame = HI_TRUE;//��̬����,֡/��ģʽ��ȡ����(1:��֡��ȡ;0:������ȡ)
				objH264Attr.u32TargetFramerate = g_objMvrCfg.AVUpload.VFramerate[i];//��̬����,Ŀ��֡��(P��:0~25;N��:0~30)
				objH264Attr.u32Bitrate = g_objMvrCfg.AVUpload.VBitrate[i]/1000;//��̬����,Ŀ������(CBR)/�������(VBR);16K~20M
				objH264Attr.u32Gop = 10*objH264Attr.u32TargetFramerate;//��̬����,I֡���(0~1000,��֡Ϊ��λ)
	
				if(g_objMvrCfg.VInput.VNormal == PAL)
				{
					objH264Attr.u32ViFramerate = 25;
					
					switch(g_objMvrCfg.AVUpload.VMode)
					{
						case QCIF:
							objH264Attr.u32PicWidth = 176;
							objH264Attr.u32PicHeight = 144;
							objH264Attr.u32BufSize = 176*144*2*2;
							break;
						case CIF:
							objH264Attr.u32PicWidth = 352;
							objH264Attr.u32PicHeight = 288;
							objH264Attr.u32BufSize = 352*288*2*2;
							break;
						case HD1:
							objH264Attr.u32PicWidth = 720;
							objH264Attr.u32PicHeight = 288;
							objH264Attr.u32BufSize = 720*288*2*2;
							break;
						default: 
							objH264Attr.u32PicWidth = 176;
							objH264Attr.u32PicHeight = 144;
							objH264Attr.u32BufSize = 176*144*2*2;
							break;
					}
				}
				else
				{
					objH264Attr.u32ViFramerate = 30;

					switch(g_objMvrCfg.AVUpload.VMode)
					{
						case QCIF:
							objH264Attr.u32PicWidth = 176;
							objH264Attr.u32PicHeight = 120;
							objH264Attr.u32BufSize = 176*120*2*2;
							break;
						case CIF:
							objH264Attr.u32PicWidth = 352;
							objH264Attr.u32PicHeight = 240;
							objH264Attr.u32BufSize = 352*240*2*2;
							break;
		        case HD1:
							objH264Attr.u32PicWidth = 720;
							objH264Attr.u32PicHeight = 240;
							objH264Attr.u32BufSize = 720*240*2*2;
							break;
						default: 
							objH264Attr.u32PicWidth = 176;
							objH264Attr.u32PicHeight = 120;
							objH264Attr.u32BufSize = 176*120*2*2;
							break;
					}     
				}
								
				iRet = HI_MPI_VENC_CreateChn(2*i+1, &objChnAttr, HI_NULL);
				if(iRet)
				{
					PRTMSG(MSG_ERR, "create video upload chn%d failed: 0x%08x\n", i+1, iRet);
					RenewMemInfo(0x02, 0x00, 0x03, 0x02);
					goto AVENC_UPLOAD_STOP;
				}
					
				iRet = HI_MPI_VENC_RegisterChn(2*i+1, 2*i+1);
				if(iRet)
				{
					PRTMSG(MSG_ERR, "register video upload chn%d failed: 0x%08x\n", i+1, iRet);
					RenewMemInfo(0x02, 0x00, 0x03, 0x03);
					goto AVENC_UPLOAD_STOP;
				}

				iRet = HI_MPI_VENC_StartRecvPic(2*i+1);
				if(iRet)
				{
					PRTMSG(MSG_ERR, "start video upload chn%d failed: 0x%08x\n", i+1, iRet);
					RenewMemInfo(0x02, 0x00, 0x03, 0x04);
					goto AVENC_UPLOAD_STOP;
				}
				
				g_objWorkStart.VUpload[i] = TRUE;
				PRTMSG(MSG_NOR, "start video upload chn%d succ\n", i+1);
			}

			/*������Ƶ����ͨ��*/
			if(g_objMvrCfg.AVUpload.AStart[i] && !g_objWorkStart.AUpload[i])
			{
				PRTMSG(MSG_DBG, "������Ƶ�ϴ�ͨ��%d\n", i+1);
				
				g_objWorkStart.AUpload[i] = TRUE;
				PRTMSG(MSG_NOR, "start audio upload chn%d succ\n", i+1);
			}
			
			if(!g_objMvrCfg.AVUpload.VStart[i] && g_objWorkStart.VUpload[i])
			{
				PRTMSG(MSG_DBG, "�ر���Ƶ�ϴ�ͨ��%d\n", i+1);
				
				//�����Ƶ���ݻ�����
				if(g_objAVUplBuf.VMemBuf[i].Buf)
				{
					g_objAVUplBuf.VMemBuf[i].Read = g_objAVUplBuf.VMemBuf[i].Write = g_objAVUplBuf.VMemBuf[i].Head;
					g_objAVUplBuf.VMemBuf[i].SameCircle = TRUE;
				}
				
				//�����Ƶ���ݻ������
				g_objVideoDataMng[i].Release();
				
				//֪ͨSockServExe�����Ƶ��������
				BYTE bytBuf[2] = {0x09, (BYTE)i};
				DataPush(bytBuf, 2, DEV_DVR, DEV_SOCK, LV2);

				iRet = HI_MPI_VENC_StopRecvPic(2*i+1);
				iRet = HI_MPI_VENC_UnRegisterChn(2*i+1);
				iRet = HI_MPI_VENC_DestroyChn(2*i+1);

				if(iRet)
				{
					PRTMSG(MSG_ERR, "stop video upload chn%d failed: 0x%08x\n", i+1, iRet);
				}
				
				g_objWorkStart.VUpload[i] = FALSE;
			}
			
			if(!g_objMvrCfg.AVUpload.AStart[i] && g_objWorkStart.AUpload[i])
			{
				PRTMSG(MSG_DBG, "�ر���Ƶ�ϴ�ͨ��%d\n", i+1);
				
				//�����Ƶ���ݻ�����
				if(g_objAVUplBuf.AMemBuf[i].Buf)
				{
					g_objAVUplBuf.AMemBuf[i].Read = g_objAVUplBuf.AMemBuf[i].Write = g_objAVUplBuf.AMemBuf[i].Head;
					g_objAVUplBuf.AMemBuf[i].SameCircle = TRUE;
				}
				
				//�����Ƶ���ݻ������
				g_objAudioDataMng[i].Release();
				
				//֪ͨSockServExe�����Ƶ��������
				BYTE bytBuf[2] = {0x09, (BYTE)(40+i)};
				DataPush(bytBuf, 2, DEV_DVR, DEV_SOCK, LV2);
				
				g_objWorkStart.AUpload[i] = FALSE;
			}
			
			if( g_objWorkStart.VUpload[i] )
			{
				memset(&objVencStat, 0, sizeof(objVencStat));

				iRet = HI_MPI_VENC_Query(2*i+1, &objVencStat);
				if(iRet) 
				{
					PRTMSG(MSG_ERR, "query upload chn%d video stream failed: 0x%08x\n", i+1, iRet);
					RenewMemInfo(0x02, 0x00, 0x03, 0x05);
					goto AVENC_UPLOAD_STOP;
				}

				if( !objVencStat.u32CurPacks )
				{
					msleep(20);
					continue;
				}
						
				memset(&objVencStream, 0, sizeof(objVencStream));
				objVencStream.u32PackCount = objVencStat.u32CurPacks;
				objVencStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S)*objVencStat.u32CurPacks);
				if(objVencStream.pstPack==NULL)	
				{
					PRTMSG(MSG_ERR, "malloc stream pack err\n");
					RenewMemInfo(0x02, 0x00, 0x03, 0x06);
					goto AVENC_UPLOAD_STOP;
				}
		
				/*������ʽ��ȡ��Ƶ֡*/
				iRet = HI_MPI_VENC_GetStream(2*i+1, &objVencStream, HI_IO_BLOCK);
				if(iRet)
				{
					PRTMSG(MSG_ERR, "get upload chn%d video stream failed: 0x%08x\n", i+1, iRet);
					free(objVencStream.pstPack);
					RenewMemInfo(0x02, 0x00, 0x03, 0x07);
					goto AVENC_UPLOAD_STOP;
				}
				
// 				for( j=0; j<objVencStream.u32PackCount; j ++ )
// 				{
// 					WriteBuf(i, (BYTE*)objVencStream.pstPack[j].pu8Addr[0], objVencStream.pstPack[j].u32Len[0], 'V');
// 
// 					if(objVencStream.pstPack[i].u32Len[1] > 0)
// 					{
// 						WriteBuf(i, (BYTE*)objVencStream.pstPack[j].pu8Addr[1], objVencStream.pstPack[j].u32Len[1], 'V');
// 					}
// 				}

				iFrameLen = 0;
				szFrameBuf[iFrameLen++] = (objVencStream.pstPack[0].DataType.enH264EType == H264E_NALU_PSLICE ? 0 : 1);
				for( j=0; j<objVencStream.u32PackCount; j ++ )
				{
					memcpy(szFrameBuf+iFrameLen, (BYTE*)objVencStream.pstPack[j].pu8Addr[0], objVencStream.pstPack[j].u32Len[0]);
					iFrameLen += objVencStream.pstPack[j].u32Len[0];
					
					if(objVencStream.pstPack[j].u32Len[1] > 0)
					{
						memcpy(szFrameBuf+iFrameLen, (BYTE*)objVencStream.pstPack[j].pu8Addr[1], objVencStream.pstPack[j].u32Len[1]);
						iFrameLen += objVencStream.pstPack[j].u32Len[1];
					}
				}

				if( g_bUploadOnlyISlice )
				{
					if( szFrameBuf[0] == 1 )
						g_objVideoDataMng[i].PushData(LV2, (DWORD)iFrameLen, szFrameBuf, dwPacketNum);
				}
				else
				{
					g_objVideoDataMng[i].PushData(LV2, (DWORD)iFrameLen, szFrameBuf, dwPacketNum);
				}

				//fwrite(szFrameBuf, iFrameLen, 1, fp);
 				//fflush(fp);


				/*�ͷ���Ƶ֡����*/
				iRet = HI_MPI_VENC_ReleaseStream(2*i+1, &objVencStream);            
				if(iRet)
				{
					PRTMSG(MSG_ERR, "release upload chn%d video stream failed: 0x%08x\n", i+1, iRet);
					free(objVencStream.pstPack);
					RenewMemInfo(0x02, 0x00, 0x03, 0x08);
					goto AVENC_UPLOAD_STOP;
				}
				
				free(objVencStream.pstPack);
			}
			
			if( g_objWorkStart.AUpload[i] )
			{
				iFrameLen = 0;

				for(j = 0; j < 5; j++)//ÿ����һ֡��Ƶʱ��Ӧ������֡��Ƶ(����ֵ)
				{
					/*������ʽ��ȡ��Ƶ֡*/
					iRet = HI_MPI_AI_GetFrame(1, 1, &objAiFrame, NULL, HI_IO_BLOCK);
					if(iRet)
 					{
 						PRTMSG(MSG_ERR, "get audio frame from ai failed: 0x%08x\n", iRet);
 						RenewMemInfo(0x02, 0x00, 0x03, 0x09);
 						goto AVENC_UPLOAD_STOP;
 					}
						
					iRet = HI_MPI_AENC_SendFrame(1, &objAiFrame, NULL);//�ϴ����ñ���ͨ��1
					if(iRet)
					{
						PRTMSG(MSG_ERR, "send audio frame to aenc failed: 0x%08x\n", iRet);
						RenewMemInfo(0x02, 0x00, 0x03, 0x0a);
						goto AVENC_UPLOAD_STOP;
					}
						
					iRet = HI_MPI_AENC_GetStream(1, &objAencStream);
					if(iRet)
					{
						PRTMSG(MSG_ERR, "get stream from aenc failed: 0x%08x\n", iRet);
						RenewMemInfo(0x02, 0x00, 0x03, 0x0b);
						goto AVENC_UPLOAD_STOP;
					}
						
					//WriteBuf(i, (BYTE*)objAencStream.pStream, objAencStream.u32Len, 'A');
					//PRTMSG(MSG_NOR, "%02x %02x %02x %02x\n", objAencStream.pStream[0], objAencStream.pStream[1], objAencStream.pStream[2], objAencStream.pStream[3]);
					memcpy(szFrameBuf+iFrameLen, (BYTE*)objAencStream.pStream, objAencStream.u32Len);
					iFrameLen += objAencStream.u32Len;
						
					/*�ͷ���Ƶ֡����*/
					iRet = HI_MPI_AENC_ReleaseStream(1, &objAencStream);            
					if(iRet)
					{
						PRTMSG(MSG_ERR, "release upload chn%d audio stream failed: 0x%08x\n", i+1, iRet);
						RenewMemInfo(0x02, 0x00, 0x03, 0x0c);
						goto AVENC_UPLOAD_STOP;
					}
				}

				g_objAudioDataMng[i].PushData(LV2, (DWORD)iFrameLen, szFrameBuf, dwPacketNum);
			}
#endif
		}

		if(g_objWorkStop.AVUpload)
		{
AVENC_UPLOAD_STOP:

			for(i=0; i<4; i++)
			{
				if(g_objWorkStart.VUpload[i])
				{
					//�����Ƶ���ݻ�����
					if(g_objAVUplBuf.VMemBuf[i].Buf)
					{
						g_objAVUplBuf.VMemBuf[i].Read = g_objAVUplBuf.VMemBuf[i].Write = g_objAVUplBuf.VMemBuf[i].Head;
						g_objAVUplBuf.VMemBuf[i].SameCircle = TRUE;
					}
					
					//�����Ƶ���ݻ������
					g_objVideoDataMng[i].Release();
					
					//֪ͨSockServExe�����Ƶ��������
					BYTE bytBuf[2] = {0x09, (BYTE)i};
					DataPush(bytBuf, 2, DEV_DVR, DEV_SOCK, LV2);
					
#if VEHICLE_TYPE == VEHICLE_M
					iRet = HI_VENC_StopCH(2*i+1);  

#if NO_VENC == 1
					iRet = HI_VENC_DestroyCH(2*i+1, HI_TRUE);
#endif

#endif

#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
					iRet = HI_MPI_VENC_StopRecvPic(2*i+1);
					iRet = HI_MPI_VENC_UnRegisterChn(2*i+1);
					iRet = HI_MPI_VENC_DestroyChn(2*i+1);
#endif
					if(iRet)
					{
						PRTMSG(MSG_ERR, "stop video upload chn%d failed: 0x%08x\n", i+1, iRet);
					}
					
					g_objWorkStart.VUpload[i] = FALSE;
					
#if VEHICLE_TYPE == VEHICLE_M
					if(g_objWorkStart.AUpload[i])
					{
						//�����Ƶ���ݻ�����
						if(g_objAVUplBuf.AMemBuf[i].Buf)
						{
							g_objAVUplBuf.AMemBuf[i].Read = g_objAVUplBuf.AMemBuf[i].Write = g_objAVUplBuf.AMemBuf[i].Head;
							g_objAVUplBuf.AMemBuf[i].SameCircle = TRUE;
						}
						HI_AENC_StopCH(3);
						g_objWorkStart.AUpload[i] = FALSE;
					}
#endif
				}
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
				if(g_objWorkStart.AUpload[i])
				{
					//�����Ƶ���ݻ�����
					if(g_objAVUplBuf.AMemBuf[i].Buf)
					{
						g_objAVUplBuf.AMemBuf[i].Read = g_objAVUplBuf.AMemBuf[i].Write = g_objAVUplBuf.AMemBuf[i].Head;
						g_objAVUplBuf.AMemBuf[i].SameCircle = TRUE;
					}
					
					//�����Ƶ���ݻ������
					g_objAudioDataMng[i].Release();
					
					//֪ͨSockServExe�����Ƶ��������
					BYTE bytBuf[2] = {0x09, (BYTE)(40+i)};
					DataPush(bytBuf, 2, DEV_DVR, DEV_SOCK, LV2);
					
					g_objWorkStart.AUpload[i] = FALSE;
				}
#endif
			}
		}

		if(	!g_objWorkStart.AUpload[0] && !g_objWorkStart.AUpload[1] && !g_objWorkStart.AUpload[2] && !g_objWorkStart.AUpload[3] 
			&&!g_objWorkStart.VUpload[0] && !g_objWorkStart.VUpload[1] && !g_objWorkStart.VUpload[2] && !g_objWorkStart.VUpload[3] )
		{
			char szbuf[10] = {0};
			
			if(g_bytUplRptBuf[1]==0x42 || g_bytUplRptBuf[1]==0x44)//ֹͣ¼���ϴ��ɹ�
			{
				szbuf[0] = 0x01;		// 0x01 ��ʾ�����ķ��͵�����֡
				szbuf[1] = 0x38;
				szbuf[2] = g_bytUplRptBuf[1];
				szbuf[3] = 0x01;
			}
			else//����¼���ϴ�ʧ��
			{
				szbuf[0] = 0x01;		// 0x01 ��ʾ�����ķ��͵�����֡
				szbuf[1] = 0x38;
				szbuf[2] = 0x41;
				szbuf[3] = 0x03;
			}

			if( 0x42 != g_bytUplRptBuf[1] )
			{
#if USE_VIDEO_TCP == 0
				DataPush(szbuf, 4, DEV_DVR, DEV_QIAN, LV2);
#endif
#if USE_VIDEO_TCP == 1
				g_objSock.MakeSmsFrame(szbuf+3, 1, 0x38, g_bytUplRptBuf[1], (char*)szSendBuf, sizeof(szSendBuf), iSendlen);
				g_objSock.SendTcpData(szSendBuf, iSendlen);
#endif
			}

			memset(g_bytUplRptBuf, 0, sizeof(g_bytUplRptBuf));
			
			g_objWorkStop.AVUpload = FALSE;
			pthread_exit(0); 
		}
	}
}

int StartAVUpl()
{
	if(( !g_objWorkStart.AUpload[0] && !g_objWorkStart.AUpload[1] && !g_objWorkStart.AUpload[2] && !g_objWorkStart.AUpload[3] 
		 &&!g_objWorkStart.VUpload[0] && !g_objWorkStart.VUpload[1] && !g_objWorkStart.VUpload[2] && !g_objWorkStart.VUpload[3] )
	 &&(  g_objMvrCfg.AVUpload.AStart[0] || g_objMvrCfg.AVUpload.AStart[1] || g_objMvrCfg.AVUpload.AStart[2] || g_objMvrCfg.AVUpload.AStart[3]
	 	 ||	g_objMvrCfg.AVUpload.VStart[0] || g_objMvrCfg.AVUpload.VStart[1] || g_objMvrCfg.AVUpload.VStart[2] || g_objMvrCfg.AVUpload.VStart[3] ) )
	{
		PRTMSG(MSG_NOR, "Start Video Upload\n");

		int iRet = pthread_create(&g_pthreadUpl, NULL, AVUpl, (void*)(&g_objMvrCfg.AVUpload));
		if(iRet) 
		{
			PRTMSG(MSG_ERR, "start video upload failed: 0x%08x\n", iRet);
			RenewMemInfo(0x02, 0x00, 0x03, 0x01);
			G_ResetSystem();
			return -1;
		}
		
		sleep(3);//��ʱ����ȷ��¼���̳߳ɹ�����
		return 0;
	}  
	
	RenewMemInfo(0x02, 0x00, 0x03, 0x00);
	return -1;
}

int StopAVUpl()
{
	if( g_objWorkStart.AUpload[0] || g_objWorkStart.AUpload[1] || g_objWorkStart.AUpload[2] || g_objWorkStart.AUpload[3] 
		||g_objWorkStart.VUpload[0] || g_objWorkStart.VUpload[1] || g_objWorkStart.VUpload[2] || g_objWorkStart.VUpload[3] )
	{
		PRTMSG(MSG_NOR, "Stop Video Upload\n");

		g_objWorkStop.AVUpload = TRUE;
		pthread_join(g_pthreadUpl, NULL);
		
		PRTMSG(MSG_NOR, "stop video upload succ\n");
		return 0;
	}

	return -1;
}

int AVTest(int v_iChn)
{
	if( strcmp("SDisk", g_objMvrCfg.AVRecord.DiskType)==0 )
	{
		if( g_objUsbSta.SDisk[0]!=CON_MOU_VAL || g_objUsbSta.SDisk[1]!=CON_MOU_VAL )
		{
			ShowDiaodu("SD��δʶ��,�޷���������ͷ��⣡");	
			return -1;
		}
	}
	else if( strcmp("HDisk", g_objMvrCfg.AVRecord.DiskType)==0 )
	{
		if( g_objUsbSta.HDisk[0]!=CON_MOU_VAL || g_objUsbSta.HDisk[1]!=CON_MOU_VAL )
		{
			ShowDiaodu("Ӳ��δʶ��,�޷���������ͷ��⣡");	
			return -1;
		}
	}
	
	int i = v_iChn;
	tag2DVOPar objVOutput = {D1, PAL, {0}};

	// ���ʱ�����ʼӴ󣬼����ٻָ�ԭ������
	UINT iBitRate = g_objMvrCfg.AVRecord.VBitrate[i];
	g_objMvrCfg.AVRecord.VBitrate[i] = 800000;

	ShowDiaodu("����ͷ¼����");
	
	StopVPrev();
	SwitchVo(objVOutput);
	SwitchVPrev(i, 0, 1);
	
	SwitchAVRec(i);
	StartAVRec();
	sleep(15);
	StopAVRec();
	SwitchAVRec(-1);
	//sync();
	sleep(3);
	SwitchVPrev(i, 0, -1);
	
	ShowDiaodu("����ͷ�طż��");
	
	IOSET( IOS_TRUMPPOW, IO_TRUMPPOW_ON );
	IOSET( IOS_EARPHONESEL, IO_EARPHONESEL_OFF );
	IOSET( IOS_AUDIOSEL, IO_AUDIOSEL_PC );
	
	SwitchAVPlay(i);
	StartAVPlay();
	sleep(15);
	StopAVPlay();
	
	IOSET( IOS_TRUMPPOW, IO_TRUMPPOW_OFF );
	IOSET( IOS_EARPHONESEL, IO_EARPHONESEL_ON );
	IOSET( IOS_AUDIOSEL, IO_AUDIOSEL_PHONE );
	
	// �����ٻָ�ԭ������
	g_objMvrCfg.AVRecord.VBitrate[i] = iBitRate;

	SwitchVo(g_objMvrCfg.VOutput);
	StartVPrev();
	
	return 0;
}
