#include "yx_QianStdAfx.h"

#if USE_LIAONING_SANQI == 1

#undef MSG_HEAD
#define MSG_HEAD ("QianExe-DataMng")

CLN_DataMng::CLN_DataMng()
{
	m_HeadNode = NULL;
	m_TailNode = NULL;
	m_CurNode = NULL;
	m_dwNodeCnt = 0;
}

CLN_DataMng::~CLN_DataMng()
{

}

int CLN_DataMng::Init()
{
	pthread_mutex_init(&m_mutex_node, NULL);
}

int CLN_DataMng::Release()
{
	tagOneNode *objNode = m_HeadNode;
	tagOneNode *objNext;
	
	for(objNode != NULL; ; )
	{
		objNext = objNode->m_next;
		
		// ���ͷ������ڴ�
		free(objNode->m_szDataBuf);
		objNode->m_szDataBuf = NULL;
		
		// ���ͷ������ڵ��ڴ�
		free(objNode);
		objNode = NULL;
		
		objNode = objNext;

		if( objNode->m_next == NULL )	// ���ѵ����������һ���ڵ�
		{
			break;
		}
	}

	m_HeadNode = NULL;
	m_CurNode = NULL;
	
	pthread_mutex_destroy(&m_mutex_node);
}

// ����ǰ�ڵ��Ƿ���Ч
bool CLN_DataMng::IsCurNodeValid()
{
	pthread_mutex_lock(&m_mutex_node);

	if( NULL != m_CurNode )
	{
		pthread_mutex_unlock(&m_mutex_node);
		return true;
	}
	else
	{
		pthread_mutex_unlock(&m_mutex_node);
		return false;
	}
}

// ���´���������ϴη���ʱ��
int CLN_DataMng::UpdateNode()
{
	pthread_mutex_lock(&m_mutex_node);
	
	m_CurNode->m_iTimers++;
	m_CurNode->m_dwLastUploadTm = GetTickCount();

	pthread_mutex_unlock(&m_mutex_node);
}

// �ж��Ƿ�Ҫɾ���ýڵ�
// �ж����ݣ����������������ش�����
bool CLN_DataMng::IsNeedDelCurNode()
{
	pthread_mutex_lock(&m_mutex_node);

	if( (m_CurNode->m_iTimers+1) > m_CurNode->m_iNeedReUploadTimers )
	{
		pthread_mutex_unlock(&m_mutex_node);
		return true;
	}
	else
	{
		pthread_mutex_unlock(&m_mutex_node);
		return false;
	}
}

// ��ȡ��ǰ�ڵ��SEQ��ˮ��
DWORD CLN_DataMng::GetCurNodeSEQ()
{
	pthread_mutex_lock(&m_mutex_node);
	
	DWORD dwSEQ = m_CurNode->m_dwSEQ;

	pthread_mutex_unlock(&m_mutex_node);

	return dwSEQ;
}

// ��ȡ��ǰ�ڵ�����, �ɹ����ػ��峤�ȣ�ʧ�ܷ���0
// ˳������ϴη���ʱ��
int CLN_DataMng::GetCurNodeData(char *v_szBuf, DWORD v_dwBufSize, DWORD v_dwTm)
{
	pthread_mutex_lock(&m_mutex_node);

	m_CurNode->m_dwLastUploadTm = v_dwTm;

	if(v_dwBufSize < m_CurNode->m_dwDataLen)
	{
		pthread_mutex_unlock(&m_mutex_node);
		return 0;
	}
	
	memcpy(v_szBuf, m_CurNode->m_szDataBuf, m_CurNode->m_dwDataLen);
	DWORD dwLen = m_CurNode->m_dwDataLen;
	
	pthread_mutex_unlock(&m_mutex_node);
	
	return dwLen;
}

int CLN_DataMng::GetCurNodeTm()
{
	pthread_mutex_lock(&m_mutex_node);
	
	DWORD dwTm = m_CurNode->m_dwLastUploadTm;
	
	pthread_mutex_unlock(&m_mutex_node);
	
	return dwTm;
}

// ���뵽����ĩβ��ע�⣺�˺����޷����������м䣩
int CLN_DataMng::InsertOneNodeToTail(DWORD v_dwSEQ, char *v_szDataBuf, DWORD v_dwDataLen, int v_iNeedReUploadTimers, int v_dwLastUploadTm)
{
	// �������
	if( v_szDataBuf == NULL || v_dwDataLen == 0)
	{
		PRTMSG(MSG_ERR, "InsertOneNodeToTail failed, Para error!\n");
		return ERR_PAR;
	}

	pthread_mutex_lock(&m_mutex_node);

	// ����һ���½ڵ�
	tagOneNode *objNewNode = (tagOneNode*)malloc( sizeof(tagOneNode) );
	if( objNewNode == NULL)
	{
		PRTMSG(MSG_ERR, "InsertOneNodeToTail failed, malloc error!\n");
		pthread_mutex_unlock(&m_mutex_node);
		return ERR_BUFLACK;
	}

	// ��ֵ
	objNewNode->m_dwSEQ = v_dwSEQ;
	objNewNode->m_szDataBuf = (char*)malloc(v_dwDataLen);
	memcpy(objNewNode->m_szDataBuf, v_szDataBuf, v_dwDataLen);
	objNewNode->m_dwDataLen = v_dwDataLen;
	objNewNode->m_iNeedReUploadTimers = v_iNeedReUploadTimers;
	objNewNode->m_iTimers = 1;		// ����һ���Ժ󣬲��������� ����ѷ��ʹ���Ϊ1
	objNewNode->m_dwLastUploadTm = v_dwLastUploadTm;
	objNewNode->m_next = NULL;
	objNewNode->m_prev = NULL;

	// ����
	objNewNode->m_prev = m_TailNode;
	if(m_dwNodeCnt==0) 
	{ 
		m_HeadNode = objNewNode;
		m_CurNode = m_HeadNode;
	} 
	else 
	{ 
		m_TailNode->m_next = objNewNode; 
	}
	
	objNewNode->m_next = NULL;
	m_TailNode = objNewNode;
	m_dwNodeCnt++;

	// �����ã���ӡ���нڵ���������
// 	{
// 		tagOneNode *objDebugNode = m_HeadNode;
// 		int  iNodeCnt = 0;
// 
// 		while( NULL != objDebugNode )
// 		{
// 			iNodeCnt++;
// 			
// 			char buf[4] = {0};
// 			memcpy(buf, (void*)&objDebugNode->m_dwSEQ, 4);
// 			
// 			PRTMSG(MSG_DBG, "i=%d, dwSEQ=%02x,%02x,%02x,%02x, data: ", iNodeCnt, buf[3],buf[2],buf[1],buf[0]);
// 			PrintString(objDebugNode->m_szDataBuf, objNewNode->m_dwDataLen);
// 
// 			objDebugNode = objDebugNode->m_next;
// 		}
// 	}

	pthread_mutex_unlock(&m_mutex_node);

	return 0;
}

// ɾ��ָ��SEQ��ˮ�ŵĽڵ�
int CLN_DataMng::DeleteSpecialNode(DWORD v_dwSEQ)
{
	bool bFind = false;

	pthread_mutex_lock(&m_mutex_node);

	tagOneNode *objJudgeNode = m_HeadNode;

	// Ѱ�ҽڵ�
	while( NULL != objJudgeNode )
	{
		if( v_dwSEQ == objJudgeNode->m_dwSEQ )
		{
			m_CurNode = objJudgeNode;
			
			bFind = true;
			break;
		}
		else
		{
			objJudgeNode = objJudgeNode->m_next;
		}
	}

	// �����ҵ�SEQ��ˮ����ͬ�Ľڵ�
	if( bFind )
	{
		tagOneNode *objNextNode;
		tagOneNode *objPrevNode;
		tagOneNode *objCurNode;
		
		objCurNode  = m_CurNode;
		objPrevNode = m_CurNode->m_prev;
		objNextNode = m_CurNode->m_next;

		if(objPrevNode == NULL) { m_HeadNode = objNextNode; objCurNode = m_HeadNode; } 
		else { objPrevNode->m_next = objNextNode; objCurNode = objNextNode; }
		
		if(objNextNode == NULL) { m_TailNode = objPrevNode; objCurNode = m_TailNode; } 
		else { objNextNode->m_prev = objPrevNode; objCurNode = objNextNode; }
		
		free(m_CurNode->m_szDataBuf);	// ���ͷ������ڴ�
		free(m_CurNode);				// ���ͷ������ڵ��ڴ�
		m_CurNode = NULL;
		m_CurNode = objCurNode;
		m_dwNodeCnt--;

		//PRTMSG(MSG_DBG, "delete one node succ!\n");

		pthread_mutex_unlock(&m_mutex_node);
		return 0;
	}

	pthread_mutex_unlock(&m_mutex_node);
	return ERR_PAR;
}

// ɾ����ǰ�ڵ�
int CLN_DataMng::DeleteCurNode()
{
	if( m_CurNode == NULL )
		return ERR_PAR;

	pthread_mutex_lock(&m_mutex_node);

	tagOneNode *objNextNode;
	tagOneNode *objPrevNode;
	tagOneNode *objCurNode;

	objCurNode  = m_CurNode;
	objPrevNode = m_CurNode->m_prev;
	objNextNode = m_CurNode->m_next;

	if(objPrevNode == NULL) { m_HeadNode = objNextNode; objCurNode = m_HeadNode; } 
	else { objPrevNode->m_next = objNextNode; objCurNode = objNextNode; }
	
	if(objNextNode == NULL) { m_TailNode = objPrevNode; objCurNode = m_TailNode; } 
	else { objNextNode->m_prev = objPrevNode; objCurNode = objNextNode; }

	free(m_CurNode->m_szDataBuf);	// ���ͷ������ڴ�
	free(m_CurNode);				// ���ͷ������ڵ��ڴ�

	m_CurNode = NULL;
	m_CurNode = objCurNode;
	m_dwNodeCnt--;

	pthread_mutex_unlock(&m_mutex_node);

	return 0;
}

// ָ����һ�ڵ�
int CLN_DataMng::GotoNextNode()
{
	pthread_mutex_lock(&m_mutex_node);

	if( NULL != m_CurNode && NULL != m_CurNode->m_next )	// ����ǰ�ڵ㲻���������һ���ڵ�
	{
		m_CurNode = m_CurNode->m_next;

		pthread_mutex_unlock(&m_mutex_node);
		return 0;
	}
	else									// ������������һ���ڵ�
	{
		m_CurNode = m_HeadNode;				// ����ǰ�ڵ�ָ������ͷ��������Ҫ����ʧ��

		pthread_mutex_unlock(&m_mutex_node);
		return ERR_PAR;
	}
}

// �ѵ�ǰ�ڵ�ָ��ָ������ͷ��
int CLN_DataMng::GotoHeadNode()
{
	pthread_mutex_lock(&m_mutex_node);

	m_CurNode = m_HeadNode;

	pthread_mutex_unlock(&m_mutex_node);

	return 0;
}
#endif
