#ifndef _YX_BLACKNEW_H_
#define _YX_BLACKNEW_H_ 

#if VIDEO_BLACK_TYPE == 2

extern int UPLOAD_PACK_SIZE;		// �ϴ�֡�еİ����ݳ���

#define		MAX_UPLOAD_FILE_COUNT		48
#define		UPLOAD_WINUNIT					32
#define		UPLOAD_WINCOUNT					2
#define		UPLOAD_WINSIZE					(UPLOAD_WINUNIT*UPLOAD_WINCOUNT)
//#define		UPLOAD_PACK_SIZE				600		// �ϴ�֡�еİ����ݳ���
#define		UPLOAD_FRAME_SIZE				1100	// �ϴ�֡���ܳ���
#define		C2_HEAD_SIZE					67		// A2֡��֡ͷ��

typedef struct yx_BLACK_QUERY_PARA
{
	BYTE	m_uszChnNo;				// ͨ����
	BYTE	m_uszEventType;		// �¼�����
	char	m_szDate[11];			// ��ѯ����
	char	m_szStartTime[9];	// ��ʼʱ��
	char	m_szEndTime[9];		// ����ʱ��
}BLACK_QUERY_PARA;

typedef struct tagUpLoadFileSta
{
	int 	m_iFileCount;																		// ���ϴ����ļ�����
	char		m_szTempFileName[MAX_UPLOAD_FILE_COUNT][40];		// ���ϴ�����ʱ�ļ������У�����·����
	char		m_szRealFileName[MAX_UPLOAD_FILE_COUNT][80];		// ���ϴ�����ʱ�ļ�����Ӧ����ʽ�ļ�������·����
	HI_U64	m_ulStartOffset[MAX_UPLOAD_FILE_COUNT];					// ��ʱ�ļ�������Ӧ����ʽ�ļ��е���ʼλ��
	HI_U64	m_ulEndOffset[MAX_UPLOAD_FILE_COUNT];						// ��ʱ�ļ�������Ӧ����ʽ�ļ��еĽ���λ��
	BYTE		m_uszFileType;																	// �����ϴ����ļ�������
	HI_U64	m_ulPackNo;																			// �����ϴ��İ����
	HI_U64	m_ulWinNo;																			// �����ϴ��Ĵ������
	HI_U64	m_ulPackTotal;																	// �����ϴ����ļ����ܰ���
	HI_U64	m_ulWinTotal;																		// �����ϴ����ļ����ܴ�����
	int 	m_iBufCount;																		// ��ǰ�����еİ���
	int	m_iBufLen[UPLOAD_WINSIZE];											// �����еĸ����ĳ��ȣ���Ϊ�����ݳ��ȿ��ܲ���ͬ��
	BYTE		m_szPackBuf[UPLOAD_WINSIZE][UPLOAD_FRAME_SIZE]; // ������
}UpLoadFileSta;

extern UpLoadFileSta g_objUploadFileSta;

int  Deal3809(char *v_databuf, int v_datalen);
int  Deal380A(char *v_databuf, int v_datalen);
int  Deal38D1(char *v_databuf, int v_datalen);
int  Deal38D3(char *v_databuf, int v_datalen);

int	 QueryBlackData(BLACK_QUERY_PARA v_objQueryPara, char *v_szStartTime, char *v_szEndTime, int  v_iFileCnt);
void BeginUploadFile();
BOOL UpdateUploadSta(BOOL v_bDeleteFirstFile);

void *G_BlackUploadThread(void* arg);
void Deal_ReUpload(void *arg, int len);

void *G_DealSockData(void *arg);
int StartBlack();
int StopBlack();

#endif

#endif

