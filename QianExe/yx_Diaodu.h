#ifndef _YX_DIAODU_H_
#define _YX_DIAODU_H_

class CDiaodu
{
public:
	CDiaodu();
	virtual ~CDiaodu();

	int Deal0501( char* v_szData, DWORD v_dwDataLen );
	int Deal053c( char* v_szData, DWORD v_dwDataLen );
	int Deal053d( char* v_szData, DWORD v_dwDataLen );

	//zzg add
	int Deal0510( char* v_szData, DWORD v_dwDataLen );
	int Deal0511( char* v_szData, DWORD v_dwDataLen );
	int DealComu73( char* v_szData, DWORD v_dwDataLen );
	int DealComu7B( char* v_szData, DWORD v_dwDataLen );
	int DealComu3C( char* v_szData, DWORD v_dwDataLen );
	
	void Init();
	
	int Deal4208( char* v_szData, DWORD v_dwDataLen );
	
	void Send0550();
	void DzCheck(bool v_bResult);
	
	void DzTest();
	void LedTest();
private:
	struct frm0510
	{
		byte type;//��������
		byte preTime[2];//ԤԼʱ��
		char fromAdd[20];//�ٳ��˵ص�
		char endAdd[20];//����ص�	
		frm0510(){memset(this,0,sizeof(*this));}
	};
	struct frm0511
	{
		byte type;//��������
		byte preTime[2];//ԤԼʱ��
		byte earlyTime;//��ǰʱ��
		char tel[15];//��ϵ�˵绰
		char name[10];//��ϵ������
		char fromAdd[20];//�ٳ��˵ص�
		char endAdd[20];//����ص�
		frm0511(){memset(this,0,sizeof(*this));}
		
	};
	//����ָʾӦ��
#pragma pack(push,1)
	struct Frm12 {
		byte type;
		byte len;
		char num[256];
		Frm12() { memset(this, 0, sizeof(*this));  type = 0x12; }
	};
	struct frm0541
	{
		BYTE	m_bytLen; //���ݳ���
		BYTE	m_bytCheck; //У���
		BYTE	m_szRouter[4];//·����Ϣ
		BYTE	m_bytDataType;//�������� 02Ϊ����ָʾ
		BYTE	m_bytEvtType;//�¼�����
		BYTE	m_bytDataLen;//���ݳ���
		char	m_szData[200];//��������
		frm0541(){memset(this,0,sizeof(*this));}
		
	};
#pragma pack(pop)
	
	frm0510 m_objDzInfo;//ԤԼ��Ϣ
	frm0541 m_objDiaoduInfo;//���ڴ�ų���ָʾ��Ϣ
	
	public:
		void P_TmSetBespokeProc();
		void P_TmBespokeNotProc();
		friend void G_TmSetBespokeProc();
		friend void G_TmBespokeNotProc();
};

#endif


