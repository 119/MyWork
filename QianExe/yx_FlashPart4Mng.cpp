#include "yx_QianStdAfx.h"
#include 	<sys/vfs.h>

#define DELOLD_PERTIME		4
#define PART4_MAX_USAGE		2.5*1024*1024		// ����ļ��ܺʹ�С2.5M

#undef MSG_HEAD
#define MSG_HEAD	("QianExe-FlashMng")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlashPart4Mng::CFlashPart4Mng()
{
	
}

CFlashPart4Mng::~CFlashPart4Mng()
{
	
}

void CFlashPart4Mng::Init()
{
//	ChkFlash();
	DelOldFile();
}

void CFlashPart4Mng::DelOldFile()
{
	struct statfs objSta;
	LINUX_HANDLE hFind = {0};
	LINUX_FIND_DATA fd;
	double dUsage = 0;
	DWORD dwTotalSize = 0;
	char szOldestFile[MAX_PATH] = {0};
	char szFileName[MAX_PATH] = {0};

	for(int i=0; i<DELOLD_PERTIME; i++)
	{
		// ��ȡ�����ļ��Ĵ�С�ܺ�
		dwTotalSize = GetToTalFileSize();
		PRTMSG(MSG_DBG, "part4 total file size: %d\n", dwTotalSize);
		if( dwTotalSize < PART4_MAX_USAGE )
		{
			PRTMSG(MSG_DBG, "Don not need to Delete file!\n");
			break;
		}
		
		// ��������ļ�
		memset(szOldestFile, 0, sizeof(szOldestFile));
		sprintf(szOldestFile, "xxx_5012_8.dat");

		hFind = FindFirstFile( FLASH_PART4_PATH, "*.*", &fd, false);
		if( hFind.bValid )
		{
			for(;;)
			{
				// �ļ�����ǰ�ĸ��ַ�
				memset(szFileName, 0, sizeof(szFileName));
				memcpy(szFileName, fd.szFileName, 4);

				if( 0 != strcmp(".", fd.szFileName)						// ����Ŀ¼
					&& 0 != strcmp("..", fd.szFileName)					// �ϼ�Ŀ¼
					&& 0 != strcmp("DrvJg2", fd.szFileName)				// �ɵ���ʻ��¼�ǵ��ļ�
					&& 0 != strcmp(DRVRECSTA_FILENAME, fd.szFileName)	// ��ʻ״̬��¼�ļ�
					&& 0 != strcmp("HandsetData.dat", fd.szFileName)	// �ֱ������ļ�
					&& 0 != strcmp("Blk_", szFileName)					// ��ϻ���ļ�
					&& 0 != strcmp("Acd_", szFileName)					// �¹��ɵ��ļ�
					)
				{
					memset(szFileName, 0, sizeof(szFileName));
					sprintf(szFileName, "rm -f -r %s%s/", FLASH_PART4_PATH, fd.szFileName);
					
					if( !system(szFileName) )
					{
						PRTMSG(MSG_NOR, "Delete Part4 file: %s succ!\n", szFileName);
					}
					else
					{
						PRTMSG(MSG_NOR, "Delete Part4 file: %s failed!\n", szFileName);
						perror("");
					}

					goto _NEXT_FILE;
				}
				
				// ���Ǻ�ϻ���ļ������¹��ɵ��ļ�
				if( !strcmp("Blk_", szFileName) || !strcmp("Acd_", szFileName)) 
				{
					if( strlen(fd.szFileName) != 14 ||	// �������ļ������Ȳ����ϵ��ļ�
						strcmp(".dat", fd.szFileName + strlen(fd.szFileName) - 4 ))		// ���ļ�����׺����
					{
						// ɾ���ļ�Ȼ������	
						memset(szFileName, 0, sizeof(szFileName));
						sprintf(szFileName, "%s%s", FLASH_PART4_PATH, fd.szFileName);
						unlink(szFileName);
						PRTMSG(MSG_NOR, "Delete Part4 file: %s\n", szFileName);
						
						continue;
					}
					else
					{
						if( strcmp(fd.szFileName + 4, szOldestFile + 4) < 0 ) // ����ǰ�ļ�����
						{
							memset(szOldestFile, 0, sizeof(szOldestFile));
							memcpy(szOldestFile, fd.szFileName, sizeof(fd.szFileName));
						}
					}
				}
_NEXT_FILE:
				if( !FindNextFile(&hFind, &fd, false) )
				{
					//PRTMSG(MSG_DBG, "Find no more file!\n");
					break;
				}
			} 

			memset(&hFind, 0, sizeof(LINUX_HANDLE));

			// ɾ������ļ�
			memset(szFileName, 0, sizeof(szFileName));
			sprintf(szFileName, "%s%s", FLASH_PART4_PATH, szOldestFile);
			unlink(szFileName);
			PRTMSG(MSG_NOR, "Delete Part4 file: %s\n", szFileName);
		}
		else
		{
			PRTMSG(MSG_DBG, "file:%s, hFind invalid!\n", fd.szFileName);
			break;
		}
	}

	memset(&hFind, 0, sizeof(LINUX_HANDLE));
}

DWORD CFlashPart4Mng::GetToTalFileSize()
{
	LINUX_FIND_DATA objFind;
	LINUX_HANDLE hFind = {0};
	DWORD dwTotalSize = 0;
	char  szFileName[MAX_PATH] = {0};
	
	FILE *fp = NULL;
	DWORD dwFileSize = 0;

	// ͳ�������ļ��Ĵ�С
	hFind = FindFirstFile(FLASH_PART4_PATH, "*.*", &objFind, false);
	if( hFind.bValid )
	{
		for(;;)
		{
			memset(szFileName, 0, sizeof(szFileName));
			sprintf(szFileName, "%s%s", FLASH_PART4_PATH, objFind.szFileName);
			fp = fopen(szFileName, "rb");

			if( fp != NULL )
			{
				fseek(fp, 0, SEEK_END);
				dwFileSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				dwTotalSize += dwFileSize;
				//PRTMSG(MSG_DBG, "file name: %s, file size:%d\n", objFind.szFileName, dwFileSize);
				fclose(fp);
			}
			else
			{
				PRTMSG(MSG_DBG, "open file failed: %s\n", objFind.szFileName);
				perror("");
			}

			if( !FindNextFile(&hFind, &objFind, false) )
			{
				//PRTMSG(MSG_DBG, "Find no more file!\n");
				break;
			}
		}
		
		memset(&hFind, 0, sizeof(LINUX_HANDLE));
	}

	return dwTotalSize;
}

void CFlashPart4Mng::ChkFlash()
{
	LINUX_FIND_DATA objFind;
	LINUX_HANDLE hFind = {0};
	char szFileName[MAX_PATH] = {0};

	int iResult = 0;

	PRTMSG(MSG_DBG, "Flash part4 Checking..." );
	
	// �����ļ����ж��Ƿ��в��������ļ�,����ͳ�������ļ��Ĵ�С
	hFind = FindFirstFile(FLASH_PART4_PATH, "*.*", &objFind, false);
	if( hFind.bValid )
	{
		do
		{
			if( S_IFDIR & objFind.objFileStat.st_mode )  // ����������Ŀ¼
			{
				iResult = 1;
				goto _ERASE_PART4;
			}

			// �ɵ���ʻ��¼�ļ�
			if( !strcmp("DrvJg2", objFind.szFileName) ) continue;

			// ��ʻ״̬��¼�ļ�
			if( !strcmp(DRVRECSTA_FILENAME, objFind.szFileName) ) continue;

			// �ֱ������ļ�
			if( !strcmp("HandsetData.dat", objFind.szFileName) ) continue;

			bool bInvalidFile = true;

			// { �¹��ɵ��ļ��ͺ�ϻ���ļ����
			if( 14 != strlen(objFind.szFileName) ) // ���ļ������Ȳ���
			{
				iResult = 3;
				goto _ERASE_PART4;
			}

			if( strcmp(".dat", objFind.szFileName + strlen(objFind.szFileName) - 4 ) ) // ���¹��ɵ��ļ�����׺����
			{
				iResult = 4;
				goto _ERASE_PART4;
			}

			memset(szFileName, 0, sizeof(szFileName));
			memcpy(szFileName, objFind.szFileName, 4);
			if( !strcmp("Blk_", szFileName) ) // ���Ǻ�ϻ���ļ�����ͷ
			{
				if( objFind.objFileStat.st_size > BLK_MAXSIZE ) // ���ļ�̫��
				{
					iResult = 5;
					goto _ERASE_PART4;
				}
				else
					bInvalidFile = false;
			}

			if( !strcmp("Acd_", objFind.szFileName) ) // �����¹��ɵ��ļ�����ͷ
			{
				if( objFind.objFileStat.st_size > ACD_MAXSIZE ) // ���ļ�̫��
				{
					iResult = 6;
					goto _ERASE_PART4;
				}
				else
					bInvalidFile = false;
			}
			// } �¹��ɵ��ļ��ͺ�ϻ���ļ����


			if( bInvalidFile )
			{
				iResult = 7;
				goto _ERASE_PART4;
			}

		} while( FindNextFile(&hFind, &objFind, false) );
		
		memset(&hFind, 0, sizeof(LINUX_HANDLE));
	}

	PRTMSG(MSG_DBG, "Flash part4 Check Done, OK!\n" );

	return;

_ERASE_PART4:
	memset(&hFind, 0, sizeof(LINUX_HANDLE));

	PRTMSG(MSG_NOR, "Prepare erase Flash part4, because %d", iResult );

	ChangMtdRW("/dev/mtd/4");
	EraseMtd("/dev/mtd/4", NULL, 3*1024*1024);

	return;
}



