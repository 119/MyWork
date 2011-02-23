#ifndef _YX_DOWNLOAD_H_
#define _YX_DOWNLOAD_H_

void *G_ThreadDownLoad(void *arg);

#define MAX_FRAME_LEN		1400

class CDownLoad
{
public:
	CDownLoad();
	~CDownLoad();

	int Init();
	int Release();

	void P_ThreadDownLoad();
	void P_TmRetran();
	void AnalyseRecvData(char *v_szRecvBuf, DWORD v_dwRecvLen);
	void SendSockData();

private:
	void _SendOneFrame(char v_szTranType, char v_szDataType, char *v_szFrameBuf, int v_iFrameLen, bool v_bNeedRetran, DWORD v_dwRetranTm);
	int  _WaitForAnswer(DWORD v_dwWaitTm, char v_szTranType, char v_szDataType);

	bool _DeleteFile(DWORD v_dwID);
	bool _RenewFileLst();
	int  _ConvertBlockSta(byte *v_bytSta_in, byte *v_bytSta_out, ushort v_usCurBlockSize);

	byte _GetCrc(const byte *v_bytCarId, const int v_iLen);
	void _GetCarId(ulong &v_ulCarId);
	bool _SetTimeToRtc(struct tm* v_SetTime);

public:
	bool	m_bNetFined;		// �����Ƿ�ͨ
	bool	m_bLoginNow;		// �Ƿ�������½
	bool 	m_bRtcTimeChecked;     //RTCУʱ��־
	bool  m_bRtcNeedChk;		//�Ƿ���Ҫ��ʱtrue:��Ҫ false:����Ҫ


#if WIRELESS_UPDATE_TYPE == 1
	bool	m_bNeedUpdate;		// ʹ��ά�����Ľ���Զ���������Ƿ�Ҫ�����ı�־
#endif

private:
	pthread_t m_pThreadDown;
	bool	m_bThreadExit;		// �߳��˳���־
	byte	m_bytSta;			// ״̬��
	ulong	m_ulCarId;			// ����ID

	bool	m_bReturnIdl;		// �Ƿ񷵻ؿ���״̬

	CInnerDataMng	m_objRecvMsg;				// ���ն���
	CInnerDataMng	m_objSendMsg;				// ���Ͷ���
	byte	m_bytRecvBuf[MAX_FRAME_LEN + 10];	// ��ǰ����֡����
	DWORD	m_dwRecvLen;						// ��ǰ����֡����
	byte	m_bytSendBuf[MAX_FRAME_LEN + 10];	// ��ǰ����֡����
	DWORD	m_dwSendLen;						// ��ǰ����֡����

	DWORD	m_dwLastLoginTm;	// �ϴε�½ʱ��

	FileList	m_objFileList;	// �ļ������б�
	DWORD	m_dwSoftId;
	DWORD	m_dwLineId;

	DWORD	m_dwFileId;			// ��ǰ�������ص��ļ�ID
	DWORD	m_dwFileSize;		// ��ǰ�������ص��ļ���С
	DWORD	m_dwDownSize;		// �����ش�С
	DWORD	m_dwDownFrameCt;	// ������֡��
	byte	m_bytMd5[32];		// ��ǰ�������صļ��ܹ���md5ֵ
	byte	m_bytFlstNo;		// ��ǰ�����ļ����ļ��б��е�λ��
	byte	m_bytFileType;
	bool	m_bMd5ok;			// У����
	
	// ��ǰ����
	byte	*m_pAllDownBuf;			// �ļ�����ָ��
	byte	*m_pDownBuf;			// ָ��
	ushort	m_usBlockSize;		// ���С
	ushort	m_usFrameSize;		// ֡��С
	byte	m_bytBlockSta[1024];// ��״̬
	ushort	m_usBlockNo;		// �����
	
	// ����
	bool	m_sended;			//�Ƿ�������ı�־
	DWORD	m_timeout;			//���ս�Ŀ���ݳ�ʱʱ���־
	int		m_informed;			//����������ϵĴ���
};

#endif
