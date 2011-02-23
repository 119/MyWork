#ifndef _YX_LN_UPDATE_H_
#define _YX_LN_UPDATE_H_

void *G_ThreadLNDownLoad(void *arg);

#define MAX_FRAME_LEN		1400

class CLNDownLoad
{
public:
	CLNDownLoad();
	~CLNDownLoad();

	int Init();
	int Release();
	void ClearPar();
	void P_ThreadDownLoad();
	bool _DeleteFile(uchar *md5);
	bool _RenewFileLst();
private:
	int  _WaitForAnswer(DWORD v_dwWaitTm, char v_szDataType, char v_szResult);

public:
	bool	m_bNetFined;		// �����Ƿ�ͨ
	bool	m_bNeedUpdate;	
	DWORD	m_dwFileSize;		// ��ǰ�������ص��ļ���С
	DWORD	m_dwDownSize;		// �����ش�С
								//
	byte	m_bytMd5[16];		// ��ǰ�������صļ��ܹ���md5ֵ

	ushort	m_ulPackageLen;
	byte	m_bytFileType;
	FileList	m_objFileList;	// �ļ������б�
	CInnerDataMng	m_objRecvMsg;				// ���ն���
private:
	pthread_t m_pThreadDown;
	bool	m_bThreadExit;		// �߳��˳���־
	byte	m_bytSta;			// ״̬��

	bool	m_bReturnIdl;		// �Ƿ񷵻ؿ���״̬


//	CInnerDataMng	m_objSendMsg;				// ���Ͷ���
	byte	m_bytRecvBuf[MAX_FRAME_LEN + 10];	// ��ǰ����֡����
	DWORD	m_dwRecvLen;						// ��ǰ����֡����
	byte	m_bytSendBuf[MAX_FRAME_LEN + 10];	// ��ǰ����֡����
	DWORD	m_dwSendLen;						// ��ǰ����֡����


	
	byte	m_bytFlstNo;		// ��ǰ�����ļ����ļ��б��е�λ��
								
	
	bool	m_bMd5ok;			// У����
	
	// ��ǰ����
	byte	*m_pDownBuf;			// ָ��
	byte	m_bytBlockSta[31];// ��״̬
	ushort	m_usPackageNo;
	DWORD	m_dwCurpackLen;
	ushort 	m_usEndPackageNo;
	
	// ����
	uint	m_timeout;			//���ս�Ŀ���ݳ�ʱʱ���־

};

#endif

