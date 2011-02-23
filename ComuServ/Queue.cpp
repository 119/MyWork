// Queue.cpp: implementation of the CQueue class.
//
//////////////////////////////////////////////////////////////////////
#include "../GlobalShare/ComuServExport.h"
#include "../GlobalShare/GlobFunc.h"
#include "ComuServDef.h"
#include "Queue.h"

#undef MSG_HEAD
#define MSG_HEAD ("ComuServ-Queue")


CQueue::CQueue()
{
	m_ulMemSize = 0;
	m_iShmID = -1;
	m_pShareMem = NULL;
}

CQueue::~CQueue()
{
	//Release();
}

int CQueue::Init( int v_kShm, unsigned long v_ulQueueSize )
{
	int iRet = 0;
	void* pShare = NULL;
	bool bFirstCreate = false;
	
	if( v_kShm <= 0 || 0 == v_ulQueueSize )
	{
		return ERR_PAR;
	}

	m_iShmID = shmget( (key_t)v_kShm, v_ulQueueSize, 0666 | IPC_CREAT | IPC_EXCL );
	if( -1 == m_iShmID )
	{
		m_iShmID = shmget( (key_t)v_kShm, v_ulQueueSize, 0666 | IPC_CREAT );
		if( -1 == m_iShmID )
		{
			iRet = ERR_SHMGET;
			PRTMSG( MSG_ERR, "shmget fail\n" ); 
			goto INIT_END;
		}
		else
		{
			PRTMSG( MSG_DBG, "shmget succ, size:%d\n", int(v_ulQueueSize) ); 
		}
	}
	else
	{
		bFirstCreate = true;
		PRTMSG( MSG_DBG, "shmget succ (first), size:%d\n", int(v_ulQueueSize) ); 
	}

	pShare = shmat( m_iShmID, 0, 0 );
	if( (void*)-1 == pShare )
	{
		iRet = ERR_SHMAT;
		PRTMSG( MSG_ERR, "shmat fail\n" );
		goto INIT_END;
	}
	else
	{
		m_pShareMem = (char*)pShare;
		PRTMSG( MSG_DBG, "shmat succ\n" ); 
	}

	if( bFirstCreate )
	{
		memset( m_pShareMem, 0, v_ulQueueSize );
	}

	m_ulMemSize = v_ulQueueSize;

INIT_END:
	return iRet;
}
 
// void CQueue::Release()
// {
// }

int CQueue::DataPop( void* v_pPopData, DWORD v_dwPopSize, DWORD *v_p_dwPopLen, DWORD *v_p_dwPushSymb )
{
	if( !m_pShareMem || 0 == m_ulMemSize || -1 == m_iShmID ) return ERR_MEMLACK;

	PQUEUEHEAD pHead = * (PQUEUEHEAD*) (m_pShareMem); // ��Ե�ַ
	if( pHead ) pHead = PQUEUEHEAD( (DWORD)pHead + m_pShareMem ); // ���Ե�ַ

	//PQUEUEHEAD pTail = * (PQUEUEHEAD*) (m_pShareMem + 4);
	//XUN_ASSERT_VALID( (pHead && pTail) || (!pHead && !pTail), "" );

	DWORD dwCur = ::GetTickCount();
	bool bEnd = false;
	do 
	{
		// ���Ե�ַ
		if( !pHead ) return ERR_QUEUE_EMPTY;
		if( pHead->m_wDataLen > v_dwPopSize ) return ERR_SPACELACK;

		if( dwCur - pHead->m_dwPushTime <= QUEUEVALID_PERIOD ) // ������δ��ʱ
		{
			memcpy( v_pPopData, (char*)pHead + sizeof(QUEUEHEAD), pHead->m_wDataLen );
			*v_p_dwPopLen = pHead->m_wDataLen;
			*v_p_dwPushSymb = pHead->m_dwPushSymb;
			bEnd = true;
		}
		else
		{
			PRTMSG( MSG_DBG, "DataPop: Release overdue data\n" );
		}

		pHead = pHead->m_pNext; // �޸�ͷָ�� (�õ�������Ե�ַ)
		if( pHead )
		{
			* (PQUEUEHEAD*) (m_pShareMem) = (PQUEUEHEAD)((char*)pHead) ; // ������ָ��, ���������Ե�ַ
			pHead = PQUEUEHEAD((DWORD)pHead + m_pShareMem); // ת��Ϊ���Ե�ַ������ʹ��
		}
		else
		{
			// ����ס�βָ��
			* (PQUEUEHEAD*) (m_pShareMem) = NULL;
			* (PQUEUEHEAD*) (m_pShareMem + 4) = NULL;
		}
	} while( !bEnd );

	return 0;
}

int CQueue::DataPush( void* v_pPushData, DWORD v_dwPushLen, DWORD v_dwPushSymb )
{
	int iRet = 0;

	if( !m_pShareMem || 0 == m_ulMemSize || -1 == m_iShmID ) return ERR_MEMLACK;
	if( 0 == v_dwPushLen ) return ERR_PAR;

	//PRTMSG( MSG_DBG, "Before DataPush: ShareMem Begin:%8x, ", (unsigned int)m_pShareMem );

	PQUEUEHEAD pHead = NULL, pTail = NULL;
	PQUEUEHEAD pNew = NULL, pNext = NULL;

	// ��ʱ�õ��Ľ�����Ե�ַ
 	pHead = * (PQUEUEHEAD*) (m_pShareMem);
 	pTail = * (PQUEUEHEAD*) (m_pShareMem + 4);

	XUN_ASSERT_VALID( (pHead && pTail) || (!pHead && !pTail), "" );
	
	//PRTMSG( MSG_DBG, "DataPush: xiangdui Head Ptr:%4d, xiangdui Tail Ptr:%4d\n", (int)pHead, (int)pTail );
	//if( pTail ) PRTMSG( MSG_DBG, "Last Element Len:%d\n", pTail->m_wDataLen );

	// ��ʱ�õ��Ĳ��Ǿ��Ե�ַ
	if( pHead ) pHead = (PQUEUEHEAD)( DWORD(pHead) + m_pShareMem );
	if( pTail ) pTail = (PQUEUEHEAD)( DWORD(pTail) + m_pShareMem );

	DWORD dwCirFreeTotal = 0; // ���η���ʱ�ͷſռ���ۻ�
	DWORD dwCur = GetTickCount();

	if( pHead ) // ����ʱΪ�ǿն���
	{
		if( !pTail )
		{
			PRTMSG( MSG_ERR, "DataPush: Head Ptr Vaild but Tail Ptr NULL!\n" );
			return ERR_QUEUE;
		}

		// ���������������ʱӦ��ʹ�õ���ʼ��ַ
		pNew = PQUEUEHEAD( (char*)pTail + sizeof(QUEUEHEAD) + pTail->m_wDataLen ); // �·����ַ,���Ե�ַ
		pNew = PQUEUEHEAD( (char*)pNew + ( (DWORD)pNew % 4 ? 4 - (DWORD)pNew % 4 : 0 ) ); // ȷ���ڴ����

		if( pTail >= pHead ) // ��������˳����� (�����˶��д�ʱֻ��һ��Ԫ�ص������)
		{
			//PRTMSG( MSG_DBG, "1\n");
			goto _QUESUN_DEAL;
		}
		else
		{
			//PRTMSG( MSG_DBG, "2\n");
			goto _QUECIR_DEAL;
		}
	}
	else // ˵����ʱ����Ϊ��
	{
		if( pTail )
		{
			PRTMSG( MSG_ERR, "DataPush: Head Ptr NULL but Tail Ptr Invalid!\n" );
			return ERR_QUEUE;
		}

		goto _QUEEMPTY_DEAL;
	}

_QUESUN_DEAL: // ˳�����
	if( (char*)pNew + sizeof(QUEUEHEAD) + v_dwPushLen <= m_pShareMem + m_ulMemSize ) // ����������󲻻ᳬ���ڴ���Top�߽�
	{
		//PRTMSG( MSG_DBG, "3\n");
		goto _DATAPUSH_END;
	}
	else
	{
		pNew = PQUEUEHEAD( m_pShareMem + 8 ); // ���ܴ�Bottom�߽翪ʼ����,ת�����η��䷽ʽ��һ���ж�,���Ե�ַ
		//PRTMSG( MSG_DBG, "4\n");
		goto _QUECIR_DEAL;
	}

_QUECIR_DEAL: // ���η���
	dwCirFreeTotal = 0;
	while( true )
	{
		// �����ж�Headָ��
		if( (char*)pHead > m_pShareMem + m_ulMemSize )
		{
			iRet = ERR_QUEUE;
			PRTMSG( MSG_ERR, "DataPush: Head Ptr Over Top Side\n" );
			goto _DATAPUSH_END;
		}

		// �����ж��ۻ��ͷſռ��С�Ƿ񳬹����������ڴ��С,��ֹ����ѭ��
		if( dwCirFreeTotal > m_ulMemSize )
		{
			iRet = ERR_QUEUE;
			PRTMSG( MSG_ERR, "DataPush: Cir Alloc Total Free Space Over Space Size\n" );
			goto _DATAPUSH_END;
		}

		// �ж������Ƿ���Ի��η���
		if( (char*)pNew + sizeof(QUEUEHEAD) + v_dwPushLen <= (char*)pHead ) // ������󲻻ᳬ��Headλ��
		{
			//PRTMSG( MSG_DBG, "5\n");
			goto _DATAPUSH_END; // �ѷ���,�ɹ�����ѭ���ж�
		}
		
		// ������󽫳���headλ��,����������

		// �ж�ͷ�����Ƿ����ɾ�����ڳ��ռ�
		if( dwCur <= pHead->m_dwPushTime + QUEUEVALID_PERIOD ) // ��ͷ����δ��ʱ����
		{
			iRet = ERR_QUEUE_FULL; // ����ɾ��,�������޷�������Ԫ��
			goto _DATAPUSH_END; // ����ʧ��,����ѭ���ж�
		}
		else
		{
			PRTMSG( MSG_DBG, "DataPush: Release overdue data\n" );
		}
		
		// ���򼴴�ͷ���ݿ����ͷ�

		// �ͷ�ͷ���ݿռ�
		pNext = pHead->m_pNext; // ��Ե�ַ
		if( pNext ) pNext = (PQUEUEHEAD)( DWORD(pNext) + m_pShareMem ); // ���Ե�ַ
		if( !pNext ) // �����ͷ����һ��Ԫ��,���б�Ϊ�ն���
		{
			if( pTail != pHead )
			{
				iRet = ERR_QUEUE;
				PRTMSG( MSG_ERR, "DataPush: For Last Element, Head Ptr != Tail Ptr\n" );
				goto _DATAPUSH_END;
			}

			pHead = pTail = NULL; // �����ͷβָ��
			goto _QUEEMPTY_DEAL; // תΪ�ն���״̬���� 
		}

		// ���ͷŵĲ������һ��Ԫ��,���ͷź������Ȼ�ǿ�

		// �ж��Ƿ��ͷŵ�����Top�߽������Ԫ��
		if( pNext < pHead )
		{
			// �ȼ����ۻ��ͷſռ�
			dwCirFreeTotal += DWORD( m_pShareMem + m_ulMemSize - (char*)pHead );

			// �ٸ���ͷָ��,���Ե�ַ
			pHead = pNext;

			// Ȼ���жϼ��������Ƿ񳬹�Top�߽�
			if( (char*)pNew + sizeof(QUEUEHEAD) + v_dwPushLen <= m_pShareMem + m_ulMemSize ) // ��������Top�߽�
			{
				//PRTMSG( MSG_DBG, "6\n");
				goto _DATAPUSH_END;
			}
			else
			{
				pNew = PQUEUEHEAD( m_pShareMem + 8 ); // �´δ�Bottom�߽翪ʼ�жϿɷ����
			}
		}
		else
		{
			pHead = pNext; // ����ͷָ��,���Ե�ַ
			dwCirFreeTotal += DWORD( (char*)pNext - (char*)pHead ); // �����ۻ��ͷſռ�
		}
	}

_QUEEMPTY_DEAL: // �ն��з���
	pHead = pTail = NULL; // ȷ��ͷβָ��Ϊ��
	pNew = PQUEUEHEAD( m_pShareMem + 8 ); // �ӱ߽翪ʼ����,���Ե�ַ
	if( (char*)pNew + sizeof(QUEUEHEAD) + v_dwPushLen <= m_pShareMem + m_ulMemSize ) // ������󲻻ᳬ���ڴ���Top�߽�
	{
		pHead = pNew; // ����ֻ����ͷָ�� (βָ�����ͳһ����)
		goto _DATAPUSH_END;
	}
	else
	{
		iRet = ERR_QUEUE_TOOSMALL;
		goto _DATAPUSH_END;
	}

_DATAPUSH_END:
	// ��ǰ�����ɹ�,���������ͳһ��Ԫ�ز������
	if( !iRet )
	{
		// ��Ԫ�ظ�ֵ
		memcpy( (char*)pNew + sizeof(QUEUEHEAD), v_pPushData, v_dwPushLen );
		pNew->m_dwPushSymb = v_dwPushSymb;
		pNew->m_wDataLen = v_dwPushLen;
		pNew->m_dwPushTime = dwCur;
		pNew->m_pNext = NULL;
		
		// ԭβԪ�صĺ���ָ��ָ�򱾴ε���βԪ��, ���������Ե�ַ (��ԭβԪ�ش���ʱ)
		if( pTail ) pTail->m_pNext = PQUEUEHEAD( (char*)pNew - m_pShareMem );
		
		// ����βָ��,���Ե�ַ
		pTail = pNew;
	}

	// �ڹ����ڴ��и���ͷβָ�� (��Ե�ַ)
	*(PQUEUEHEAD*) (m_pShareMem) = PQUEUEHEAD( pHead ? (char*)pHead - m_pShareMem : NULL ); // �޸�ͷָ��
	*(PQUEUEHEAD*) (m_pShareMem + 4) = PQUEUEHEAD( pTail ? (char*)pTail - m_pShareMem : NULL ); // �޸�βָ��

	if( iRet ) PRTMSG( MSG_DBG, "DataPush: Result %d\n", iRet );

	return iRet;
}

int CQueue::DataDel( void* v_pCmpDataFunc )
{
	if( !m_pShareMem || 0 == m_ulMemSize || -1 == m_iShmID ) return ERR_MEMLACK;
	return 0;
}
