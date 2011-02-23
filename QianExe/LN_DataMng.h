#ifndef _LN_DATAMNG_H_
#define _LN_DATAMNG_H_

class CLN_DataMng
{
public:
	CLN_DataMng();
	virtual ~CLN_DataMng();

public:
	int Init();
	int Release();

	bool  IsCurNodeValid();
	bool  IsNeedDelCurNode();
	int   UpdateNode();
	DWORD GetCurNodeSEQ();
	int   GetCurNodeData(char *v_szBuf, DWORD v_dwBufSize, DWORD v_dwTm);
	int   GetCurNodeTm();

	int InsertOneNodeToTail(DWORD v_dwSEQ, char *v_szDataBuf, DWORD v_dwDataLen, int v_iNeedReUploadTimers, int v_dwLastUploadTm);
	int DeleteSpecialNode(DWORD v_dwSEQ);
	int DeleteCurNode();
	int GotoNextNode();
	int GotoHeadNode();

private:
	struct tagOneNode
	{
		struct tagOneNode *m_next;			// ��һ�ڵ�ָ��
		struct tagOneNode *m_prev;			// ��һ�ڵ�ָ��
		DWORD	m_dwSEQ;					// SEQ��ˮ��
		char	*m_szDataBuf;				// ����ָ��
		DWORD	m_dwDataLen;				// ���ݳ���
		int		m_iNeedReUploadTimers;		// ����Ҫ�ش������ش���������
		int		m_iTimers;					// �Ѵ������
		DWORD	m_dwLastUploadTm;			// �ϴδ���ʱ��
	};

	tagOneNode	*m_HeadNode;
	tagOneNode	*m_TailNode;
	tagOneNode	*m_CurNode;
	DWORD		m_dwNodeCnt;
	
	pthread_mutex_t m_mutex_node;
};

#endif


