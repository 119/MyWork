#ifndef _YX_UPLOAD_H_
#define _YX_UPLOAD_H_

#define		UPLOAD_WIN_SIZE  8	// ���ڴ�С

typedef struct tagDataWinDow
{
	HI_U64	m_ulWinNo;								// �����ڵĴ��ں�
	bool	m_bISlice[UPLOAD_WIN_SIZE];				// �Ƿ�I֡�ı�־
	int		m_iBufLen[UPLOAD_WIN_SIZE];				// �����ݰ��ĳ���
	BYTE	m_uszDataBuf[UPLOAD_WIN_SIZE][1500];	// ����
}DataWinDow;

typedef struct tagDataUploadSta
{
	int		m_iCurWinNo;				// ��ǰ��Ҫ���Ĵ���
	DataWinDow	m_objDataWin[32];			// ����
}DataUploadSta;

extern DataUploadSta	g_objDataUploadSta[8];
extern tag1PComuCfg g_objComuCfg;

void *G_DataUpload(void*);
int	Deal3835(char *v_databuf, int v_datalen);
int	Deal3836(char *v_databuf, int v_datalen);

#endif

