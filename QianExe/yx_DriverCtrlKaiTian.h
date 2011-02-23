#ifndef _YX_DRVCTRL_KAITIAN_H_
#define _YX_DRVCTRL_KAITIAN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DRIVERPICPATH	L"\\Resflh2\\DriverPIC.jpg"
#define RCVPICPACKSIZE		1024//��Ƭ����ÿ����С
#define SNDPICPACKSIZE		512//��Ƭ����ÿ����С

class CKTDriverCtrl  
{
public:
#pragma pack(push,1)
	struct frm4004
	{
		byte driverID[3];//˾�������Ϣ
		char driverName[10];//˾������
		byte level;//����ȼ�
		char copName[20];//��˾����	
		frm4004(){memset(this,0,sizeof(*this));}
	};
	
	struct frm4005
	{
		byte driverID[3];//˾�������Ϣ
		WORD totalPack;//�ܰ���
		byte winSize;//���ڴ�С
		frm4005(){memset(this,0,sizeof(*this));}
	};
	
	struct frm4006
	{
		byte driverID[3];//˾�������Ϣ
		byte flag;//��־λ
		WORD packNum;//����
		WORD packSize;//����С
		byte data[1024];//������
		frm4006(){memset(this,0,sizeof(*this));}
	};
	
	struct frm4045
	{
		byte driverID[3];//˾�������Ϣ
		byte resType;
		frm4045(){memset(this,0,sizeof(*this));}
	};
	
	struct frm4046
	{
		byte driverID[3];//˾�������Ϣ
		byte curWin;//��ǰ�������
		byte winSta[2];//����״̬
		frm4046(){memset(this,0,sizeof(*this));}
	};
	
	//������Ƭ����
	struct frmD6
	{
		byte type;
		byte driverID[3];//˾�������Ϣ
		WORD packNum;//�ܰ���	
		frmD6(){ memset(this,0,sizeof(*this));type=0xD6;}
	};
	
	//��Ƭ����
	struct frmD8
	{
		byte type;
		WORD curPack;
		WORD dataLen;
		byte data[SNDPICPACKSIZE];
		frmD8(){ memset(this,0,sizeof(*this));type=0xD8;}
	};
	
	//���Ӱ�����
	struct frmDC
	{
		byte type;
		byte driverID[3];//��ǰ˾�������Ϣ
		frmDC(){ memset(this,0,sizeof(*this));type=0xDC;}
	};
	
	//���ӷ���֤��Ϣ
	struct frmD4
	{
		byte type;
		frm4004 info;
		frmD4(){ memset(this,0,sizeof(*this));type=0xD4;}
	};
#pragma pack(pop)

	struct PICRCVINFO
	{
		byte driverID[3];//˾�������Ϣ
		WORD totalPack;//�ܰ���
		WORD curPack;//��ǰ��
		WORD curWin;//��ǰ�������
		byte winSize;//���ڴ�С
		byte winNum;//��������
		byte lastWinSize;//���һ�����ڴ�С
		byte winSta[2];//����״̬
		byte rcvSta;//����״̬0��ʾδ������ϣ�1��ʾ�������
		PICRCVINFO(){memset(this,0,sizeof(*this));}
	};
	
	struct PICSNDINFO
	{
		byte driverID[3];//˾�������Ϣ
		WORD packNum;//�ܰ���
		WORD curPack;//��ǰ��
		byte sendSta;//����״̬1��ʾδ�����꣬2��ʾ�ѷ�����
		PICSNDINFO(){memset(this,0,sizeof(*this));}
	};

  bool m_bSendPicWork;
  bool m_bSendPicExit;
  pthread_t	m_pthreadSendPic;//������Ƭ�߳�

private:
	PICRCVINFO m_objPicRcvInfo;
	PICSNDINFO m_objPicSndInfo;
	//byte m_bytSendPackNum;//��ǰ���Ͱ����
	int m_iResPicTime;
	
public:
	CKTDriverCtrl();
	virtual ~CKTDriverCtrl();
  void Init();
  void Release();
  
  void P_TmReSendPicProc();
	void P_TmResPicProc();
  void P_ThreadSendPic();
  
	int DealComuD5( char* v_szData, DWORD v_dwDataLen );
	int DealComuD7( char* v_szData, DWORD v_dwDataLen );
	int DealComuD9( char* v_szData, DWORD v_dwDataLen );
	int DealComuDA( char* v_szData, DWORD v_dwDataLen );
	int DealComuDB( char* v_szData, DWORD v_dwDataLen );
	int Deal4004( char* v_szData, DWORD v_dwDataLen );
	int Deal4005( char* v_szData, DWORD v_dwDataLen );
	int Deal4006( char* v_szData, DWORD v_dwDataLen );
	int Deal4008( char* v_szData, DWORD v_dwDataLen );
	
	void ClearSendThd();
	bool SendPic();
	void BeginSendPic();
	void SndDrvChgInf(byte* v_aryODriverID,byte v_bytLen);
	
	void PicTest();//��Ƭ�������
	void DriverInfoTest();//������ӷ���֤��Ϣ����
	void DrvInfoChgTest();//���Ӱ����

private:
	WORD _GetPicFileSize();
	void _ReqDriverInfo(char* v_szDriverID,DWORD v_dwDataLen);
	bool _ReqSendPic();//������
	void _Send4046();
};

#endif
