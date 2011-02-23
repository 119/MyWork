#ifndef _YX_INNERDATAMNG_H_
#define _YX_INNERDATAMNG_H_

#pragma pack(4)

typedef struct tagInnerData
{
	tagInnerData*	m_pNext;
	tagInnerData*	m_pPre;
	void*			m_pData;
	DWORD			m_dwDataLen;
	DWORD			m_dwPushTm;
	WORD			m_wReserve;
	BYTE			m_bytSymb;
	BYTE			m_bytLvl;
	char			m_szReserve[16]; // ʵ�ʳ��Ȳ���ʱ��0��β
	
	tagInnerData() { memset(this, 0, sizeof(*this)); }
	~tagInnerData() { if(m_pData) free(m_pData); }
}INNERDATA;

#pragma pack()

typedef int (*JUGSYMBDATAFUNC)(void* v_pElement, void* v_pSymb, DWORD v_dwSymbSize );

class CInnerDataMng  
{
public:
	void InitCfg( DWORD v_dwSpaceSize, DWORD v_dwInvalidPeriod );
	void GetCfg( DWORD& v_dwSpaceSize, DWORD& v_dwInvalidPeriod );
	void Release();
	bool IsEmpty();
	DWORD GetPacketCount();
	CInnerDataMng& operator =( CInnerDataMng &v_objInnerDataMng);
	int DelSpecData( BYTE v_bytSymb );
	bool IsSpecDataExist( BYTE v_bytSymb );
	int DelOldData();
	int SaveBackData( BYTE v_bytLvl, DWORD v_dwDataLen, char *v_pDataBuf, BYTE v_bytSymb, char* v_szReserv );
	int PushData( BYTE v_bytLvl, DWORD v_dwDataLen, char *v_pDataBuf, DWORD &v_dwPacketNum, BYTE v_bytSymb = 0,
		char* v_szReserv = NULL, size_t v_sizReserv = 0 );
	int PopData(BYTE &v_bytLvl, DWORD v_dwBufSize, DWORD &v_dwDataLen, char * v_pDataBuf,
		DWORD &v_dwPushTm, BYTE* v_p_bytSymb = NULL, char* v_szReserv = NULL, size_t v_sizReserv = 0 );
	int PopSymbData( JUGSYMBDATAFUNC v_pJugSymbFunc, void* v_pSymb, DWORD v_dwSymbSize, BYTE &v_bytLvl,
		DWORD v_dwBufSize, DWORD &v_dwDataLen, char * v_pDataBuf, DWORD &v_dwPushTm,
		BYTE* v_p_bytSymb = NULL, char* v_szReserv = NULL, size_t v_sizReserv = 0 );
	int DelSymbData( JUGSYMBDATAFUNC v_pJugSymbFunc, void* v_pSymb, DWORD v_dwSymbSize );
	
	CInnerDataMng( DWORD v_dwSpaceSize = 10000, DWORD v_dwInvalidPeriod = 1000000 );
	virtual ~CInnerDataMng();
	
private:
	DWORD			m_dwInvalidPeriod; // ��λ:����
	DWORD			m_dwSpaceSize; // ���洢�ռ�
	DWORD			m_dwSpaceUse; // ��ǰʹ�õĿռ�
	DWORD			m_dwPacketNum; // ��ǰ��������ݰ�����
	pthread_mutex_t m_DataMutex;

	tagInnerData*	m_pHead;
	tagInnerData*	m_pTail;
};

inline CInnerDataMng& CInnerDataMng::operator =( CInnerDataMng &v_objInnerDataMng)
{

	DWORD dwSpaceSize = 10000, dwInvalidPeriod = 60000;
	v_objInnerDataMng.GetCfg( dwSpaceSize, dwInvalidPeriod );
	
	Release();
	InitCfg( dwSpaceSize, dwInvalidPeriod );
	
	DWORD dwLen = 0;
	DWORD dwPushTm;
	DWORD dwPacketNum = 0;
	char buf[ 400 ] = { 0 };
	int iRet = 0;
	BYTE bytLvl, bytSymb;
	do
	{
		iRet = v_objInnerDataMng.PopData( bytLvl, sizeof(buf), dwLen, buf, dwPushTm, &bytSymb );
		if( !iRet ) iRet = PushData( bytLvl, dwLen, buf, dwPacketNum, bytSymb );
	} while( !iRet );
	
	return *this;

}

#endif


