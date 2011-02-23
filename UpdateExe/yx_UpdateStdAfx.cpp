#include "yx_UpdateStdAfx.h"

#define BOOT_PART	  ("/dev/mtd/0")
#define KERN_PART	  ("/dev/mtd/1")
#define ROOT_PART	  ("/dev/mtd/2")
#define UPDATE_DIR	("/mnt/Flash/part3/Temp/")

int g_iMsgId;
CTimerMng g_objTimerMng;

CDownLoad g_objDownLoad;

CSock g_objUpdateSock;

// bool G_SoftUpdate(byte v_bytFlstNo, char *v_szFileName, int v_iFlag)
// {
// 	byte bytFileCnt = 0;
// 	bool bUpdateSucc = false;	// ȱʡΪ����ʧ��
// 	
// 	FILE *fpSrc = NULL;
// 	FILE *fpDes = NULL;
// 	size_t iFileSize = 0;
// 	char *szBuf = NULL;
// 	char szDesFileToDir[40] = {0};
// 	char szDesFileToName[40] = {0};
// 	char szDesFileFromPath[80] = {0};
// 	char szDesFileToPath[80] = {0};
// 
// 	if( (fpSrc = fopen(v_szFileName, "rb+"))==NULL )
// 	{
// 		PRTMSG(MSG_ERR, "Open updfile failed:");
// 		perror(" ");
// 		goto _UPDATE_END;
// 	}
// 	
// 	// ���ڴ�������ļ�����ʱ�ļ���
// 	if( access(UPDATE_DIR, F_OK) != 0 )
// 	{
// 		mkdir(UPDATE_DIR, 777);
// 		chmod(UPDATE_DIR, S_IRWXU|S_IRWXG|S_IRWXO);
// 	}
// 	
// 	switch(v_bytFlstNo)
// 	{
// 		case SOFT:
// 		{
// 			// ���н������ѹ������ʽ���ļ�������1�������ļ�Ŀ¼(40) + �ļ�����(40) + �ļ�����(4) + �ļ�����(n)��* N
// 			if( 1 != fread((void*)&bytFileCnt, 1, 1, fpSrc) )	// �ļ�����
// 				goto _UPDATE_END;
// 			
// 			for(int i = 0; i < bytFileCnt; i++)
// 			{
// 				if( 40 != fread(szDesFileToDir, 1, 40, fpSrc) || 40 != fread(szDesFileToName, 1, 40, fpSrc) || 4 != fread((void*)&iFileSize, 1, 4, fpSrc) )
// 					goto _UPDATE_END;
// 				
// 				if( NULL == (szBuf = (char*)malloc(iFileSize)) )
// 				{
// 					PRTMSG(MSG_ERR, "Malloc buffer failed");
// 					goto _UPDATE_END;
// 				}
// 				
// 				if( iFileSize != fread(szBuf, 1, iFileSize, fpSrc) )
// 				{
// 					PRTMSG(MSG_ERR, "Read updfile failed");
// 					goto _UPDATE_END;
// 				}
// 				
// 				sprintf(szDesFileFromPath, "%s%s", UPDATE_DIR, szDesFileToName);
// 				sprintf(szDesFileToPath, "%s%s", szDesFileToDir, szDesFileToName);
// 				
// 				if( v_iFlag==0 )//Memory����
// 				{
// 					if( unlink(szDesFileToPath)==-1 )
// 					{
// 						PRTMSG(MSG_ERR, "Delete %s fail\n", szDesFileToPath);
// 					}
// 			
// 					if( (fpDes = fopen(szDesFileToPath, "wb+"))==NULL )
// 					{
// 						PRTMSG(MSG_ERR, "Open %s failed", szDesFileToPath);
// 						goto _UPDATE_END;
// 					}
// 					
// 					if( iFileSize != fwrite(szBuf, 1, iFileSize, fpDes) )
// 					{
// 						PRTMSG(MSG_ERR, "Write %s failed", szDesFileToPath);
// 						goto _UPDATE_END;
// 					}
// 					
// 					if(szBuf)
// 					{
// 						free(szBuf);
// 						szBuf = NULL;
// 					}
// 					
// 					if(fpDes)
// 					{
// 						fclose(fpDes);
// 						fpDes = NULL;
// 					}
// 				}
// 				else if( v_iFlag==1 )//Flash����
// 				{
// 					if( (fpDes=fopen(szDesFileFromPath, "wb+"))==NULL )
// 					{
// 						PRTMSG(MSG_ERR, "Open %s failed", szDesFileFromPath);
// 						goto _UPDATE_END;
// 					}
// 					
// 					if( iFileSize != fwrite(szBuf, 1, iFileSize, fpDes) )
// 					{
// 						PRTMSG(MSG_ERR, "Write %s failed", szDesFileFromPath);
// 						goto _UPDATE_END;
// 					}
// 					
// 					if(szBuf)
// 					{
// 						free(szBuf);
// 						szBuf = NULL;
// 					}
// 					
// 					if(fpDes)
// 					{
// 						fclose(fpDes);
// 						fpDes = NULL;
// 					}
// 					
// 					if( unlink(szDesFileToPath)==-1 )
// 					{
// 						PRTMSG(MSG_ERR, "Delete %s fail\n", szDesFileToPath);
// 					}
// 					
// 					// �滻ԭ�ļ�
// 					if( SYSTEM("cp -f %s %s", szDesFileFromPath, szDesFileToPath)!=0 )
// 					{
// 						PRTMSG(MSG_ERR, "Copy %s failed", szDesFileToPath);
// 						goto _UPDATE_END;
// 					}
// 				}
// 				
// 				// ����Ȩ��
// 				if( chmod(szDesFileToPath, S_IRWXU|S_IRWXG|S_IRWXO) )
// 				{
// 					PRTMSG(MSG_ERR, "Chmod %s failed", szDesFileToPath);
// 					goto _UPDATE_END;
// 				}
// 
// 				if( unlink(szDesFileFromPath)==-1 )
// 				{
// 					PRTMSG(MSG_ERR, "Delete %s fail\n", szDesFileFromPath);
// 				}
// 			}
// 		}
// 			break;
// 	
// 		default:
// 			break;
// 	}
// 	
// 	bUpdateSucc = true;
// 	
// _UPDATE_END:
// 	if(szBuf)
// 	{
// 		free(szBuf);
// 		szBuf = NULL;
// 	}
// 	
// 	if(fpSrc)
// 	{
// 		fclose(fpSrc);
// 		fpSrc = NULL;
// 	}
// 	
// 	if(fpDes)
// 	{
// 		fclose(fpDes);
// 		fpDes = NULL;
// 	}
// 	
// 	unlink(szDesFileFromPath);
// 
// 	// ��ɾ������Դ�ļ�
// 	//unlink(v_szFileName);
// 	
// 	return bUpdateSucc;
// }
