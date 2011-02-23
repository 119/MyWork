#ifndef _SMS_H_
#define _SMS_H_

//----------------------------------------------------------------------------------------------------------------
// �û���Ϣ���뷽ʽ
#define SMS_7BIT		0	//ASCI
#define SMS_8BIT		4
#define SMS_UCS2		8

//----------------------------------------------------------------------------------------------------------------
// ����Ϣ�����ṹ������/���빲��
// ���У��ַ�����0��β
struct SM_PARAM {
	char sca[16];			// ����Ϣ�������ĺ���(SMSC��ַ)
	char tpa[16];			// Ŀ������ظ�����(TP-DA��TP-RA)
	char pid;				// �û���ϢЭ���ʶ(TP-PID)
	char dcs;				// �û���Ϣ���뷽ʽ(TP-DCS)
	char scts[16];			// ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
	char ud[161];			// ԭʼ�û���Ϣ(����ǰ�������TP-UD) ��������0��β������Ҫ161
	char type;				// ����Ϣ����
	char index;				// ����Ϣ��ţ��ڶ�ȡʱ�õ�
	size_t udl;				// evdo���Ӷ���Ϣ����

	SM_PARAM() { memset(this, 0, sizeof(*this)); }
};

//////////////////////////////////////////////////////////////////////////
static int bytes_to_string(const uchar *psrc, char *pdst, int srclen);
int string_to_bytes(const char *psrc, uchar *pdst, int srclen);
static int encode_7bit(const char *psrc, uchar *pdst, int srclen);
static int decode_7bit(const uchar *psrc, char *pdst, int srclen);
static int encode_8bit(const char *psrc, uchar *pdst, int srclen);
static int decode_8bit(const uchar *psrc, char *pdst, int srclen);
static size_t encode_ucs2(const char *psrc, uchar *pdst, size_t srclen);
static size_t decode_ucs2(const uchar *psrc, char *pdst, size_t srclen);
static int invert_numbers(const char *psrc, char *pdst, int srclen);
static int serialize_numbers(const char *psrc, char *pdst, int srclen);
bool wcdma_encode_pdu(SM_PARAM *v_pSM,char *v_pBuf,int &v_pLen);
bool wcdma_decode_pdu(const char *psrc, SM_PARAM *pdst);
bool evdo_encode_pdu(SM_PARAM *v_pSM,char *v_pBuf,int &v_pLen);
bool evdo_decode_pdu(const char *psrc, SM_PARAM *pdst);
int decode_pdu(const char *psrc, SM_PARAM *pdst);
int encode_pdu(SM_PARAM *psrc, char *pdst);

#endif	//#define _SMS_H_
