#include "StdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("Monitor-Main")

#define APP_IN_PART2	5
#define APP_IN_PART5	6

volatile bool g_bProgExit = false;

void *G_ThreadClearDog(void *arg)
{
	int iDog = 0;

	if((iDog = open("/dev/misc/watchdog", O_WRONLY)) == -1)
	{
		PRTMSG(MSG_ERR, "open watchdog failed\n");
		system("reboot");
	}

	while(!g_bProgExit)
	{
		write(iDog, "\0", 1);
		sleep(5);
	}
}

int main()
{
	// ����ʱ���������Ź��߳�,һֱι��,ֱ�����߳�֪ͨ�˳�
	pthread_t pthreadDog;
	pthread_create(&pthreadDog, NULL, G_ThreadClearDog, NULL);
	
	g_objDog.DogInit(); // ȷ����һ���жϿ��Ź������־�ܳɹ�
	
	int i, j;
	int	iCnt = 0;		// �������ٴ�δ�幷�Ĵ���
	DWORD	dwFlag = 0;
	
	char *szAppInPart2[APP_IN_PART2] = {"/SockServExe", "/IOExe", "/UpdateExe", "/ComuExe", "/QianExe"};
	char *szAppInPart5[APP_IN_PART5] = {"/mnt/Flash/part5/DvrExe", "/mnt/Flash/part5/SockServExe", "/mnt/Flash/part5/IOExe", "/mnt/Flash/part5/UpdateExe", "/mnt/Flash/part5/QianExe", "/mnt/Flash/part5/ComuExe"};
	
	pid_t RunPid[6] = {0};
	int EndStatus = 0;
	int EndSucc = 0;

	char szPart, szError, szUpdate;
	int iAppInPartx;
	char **pszAppInPartx;
	
	CreateLog();
	
	// ���ErrLog��,��Ĭ�����з���2����
	if(ReadLog(szPart, szError, szUpdate) == -1)
	{
		PRTMSG(MSG_DBG, "ErrLog damage, use default part2\n");
		
		szPart = '2'; szError = '0'; szUpdate = '0';
	}
	
	// ���ErrLog����,��Ĭ�����з���5����
	if((szPart != '2' && szPart != '5') || (szError < '0' || szError > '5') || (szUpdate < '0' || szUpdate > '5'))
	{
		PRTMSG(MSG_DBG, "ErrLog error, use default part5\n");
		
		szPart = '5';	szError = '0'; szUpdate = '0';
		if(WriteLog(szPart, szError, szUpdate) == -1)
		{
			PRTMSG(MSG_DBG, "write errlog fail\n");
			DeleteLog();
			goto EXIT;
		}
	}

ENTRY:
	PRTMSG(MSG_DBG, "Part:%c Error:%c Update:%c\n", szPart, szError, szUpdate);

	// ���������ȫ�����г������δ��������
	if(AllAppExist(szPart) == true && szError <= '8')
	{
		if(szPart == '2')
		{
			PRTMSG(MSG_DBG, "begin to run in part2\n");
			
			DOG_RSTVAL = 0XFFFFFFFF & ~DOG_QIAN & ~DOG_COMU & ~DOG_SOCK & ~DOG_IO & ~DOG_UPGD;// & ~DOG_DVR_T1 & ~DOG_DVR_T2 & ~DOG_DVR_T3;
			iAppInPartx = APP_IN_PART2;
			pszAppInPartx = szAppInPart2;
		}
		else if(szPart == '5')
		{
			PRTMSG(MSG_DBG, "begin to run in part5\n");
			
			DOG_RSTVAL = 0XFFFFFFFF & ~DOG_QIAN & ~DOG_COMU & ~DOG_SOCK & ~DOG_IO & ~DOG_UPGD & ~DOG_DVR & ~DOG_DVR_T1 & ~DOG_DVR_T2 & ~DOG_DVR_T3;
			iAppInPartx = APP_IN_PART5;
			pszAppInPartx = szAppInPart5;
		}
	}
	else
	{
		// ��������ɹ���������5��,���л�������2����
		if(szUpdate == '5')
		{
			PRTMSG(MSG_DBG, "update too many times, switch to part2\n");
			
			szPart = '2';	szError = '0'; szUpdate = '0';
			if(WriteLog(szPart, szError, szUpdate) == -1)
			{
				PRTMSG(MSG_DBG, "write errlog fail\n");
				DeleteLog();
				goto EXIT;
			}
	
			goto ENTRY;
		}
		// ���������������
		else
		{
			PRTMSG(MSG_DBG, "begin to update from part3\n");
						
			for(i = 0; i < 3; i++)
			{
				if(AllAppExist('3') == true)
				{
					if(G_ExeUpdate("/mnt/Flash/part3/Down/exe.bin", 0))
					{
						PRTMSG(MSG_DBG, "update succ, switch to part5\n");
						
						// ��������ɹ�,�л�������5,�����������,������������
						szPart = '5';	szError = '0'; szUpdate++;
						if(WriteLog(szPart, szError, szUpdate) == -1)
						{
							PRTMSG(MSG_DBG, "write errlog fail\n");
							DeleteLog();
							goto EXIT;
						}
	
						goto ENTRY;
					}
				}
							
				sleep(1);
			}
			
			PRTMSG(MSG_DBG, "update fail, switch to part2\n");
			
			// �������ʧ��,�л�������2,�����������,������������
			szPart = '2';	szError = '0'; szUpdate = '0';
			if(WriteLog(szPart, szError, szUpdate) == -1)
			{
				PRTMSG(MSG_DBG, "write errlog fail\n");
				DeleteLog();
				goto EXIT;
			}
	
			goto ENTRY;
		}
	}

	// ��������, ���س���
	for(i = 0; i < iAppInPartx; i++)
	{
		chmod(pszAppInPartx[i], S_IRWXU|S_IRWXG|S_IRWXO);

		if((RunPid[i] = fork()) == -1)
		{
			PRTMSG(MSG_DBG, "fork process %s fail, exit\n", pszAppInPartx[i]);
			
			// ��������ӽ���ʧ��,�����������
			szError++;
			if(WriteLog(szPart, szError, szUpdate) == -1)
			{
				DeleteLog();
			}
			goto EXIT;
		}
		else if(RunPid[i] == 0)
		{
			if(-1 == execl(pszAppInPartx[i], NULL))
			{
				// ����ӽ��̼��س���ʧ��,������
				PRTMSG(MSG_DBG, "execl program %s fail, sleep\n", pszAppInPartx[i]);
				sleep(3600*48); 
			}
		}
	}
	
	while(!g_bProgExit)
	{
		sleep(8);

		dwFlag = g_objDog.DogQuery();

		if(dwFlag == 0xffffffff)
		{
			iCnt = 0;

			// ��������������ʱ�䳬��20����(��1200��,��150��ѭ��),��ʧ�ܴ�������,������������
			static DWORD dwRunTime = 0;
			if(++dwRunTime > 150)
			{
				szError = '0'; szUpdate = '0';
				if(WriteLog(szPart, szError, szUpdate) == -1)
				{
					PRTMSG(MSG_DBG, "write errlog fail\n");
					DeleteLog();
					goto EXIT;
				}
			}
		}
		else
		{
			PRTMSG(MSG_DBG, "dwFlag = %08x\n", dwFlag);

			// ������5��δ��Ч�幷(һ���жϼ��Ϊ8��,5�μ�Ϊ32��)
			if(++iCnt > 5)
			{
				// ����Ƿ����ӽ����˳�
				for(i = 0; i < iAppInPartx; i++)
				{
					if(waitpid(RunPid[i], &EndStatus, WNOHANG) == RunPid[i])
					{
						//�ӽ��������˳�
						if(WIFEXITED(EndStatus) != 0)
						{
							EndSucc++;
						}
					}
				}
				
				// ��������ӽ��̶������˳�,�����������
				if(EndSucc == iAppInPartx)
				{
					szError = '0';
				}
				// ��������������
				else
				{
					szError++;
				}
				
				PRTMSG(MSG_DBG, "%d/%d program end normally\n", EndSucc, iAppInPartx);
				
				if(WriteLog(szPart, szError, szUpdate) == -1)
				{
					DeleteLog();
				}
				goto EXIT;
			}
		}
	}
		
EXIT:
	g_bProgExit = true;
	sleep(3);
	return 0;
}

