#include "ComuStdAfx.h"

#undef MSG_HEAD
#define MSG_HEAD ("ComuExe-Sms:")

#define DSTLEN_MAX 200

//----------------------------------------------------------------------------------------------------------------
// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// psrc: Դ�ַ���ָ��
// pdst: Ŀ������ָ��
// srclen: Դ�ַ�������
// ����: Ŀ�����ݳ���
// 100513 xun modify,�����޸ģ���ת���������ַ�ʱ�������� (���ǣ���Ӧ�ó���ת���������ַ��������
int string_to_bytes(const char *psrc, uchar *pdst, int srclen)
{
	int i = 0;
	char *psrcfst = (char*)psrc;
	for( i=0; i<srclen; i+=2)
	{
		if( 0 == *psrc ) break;

		// �����4λ
		if(*psrc>='0' && *psrc<='9')  {
			*pdst = (*psrc - '0') << 4;
		} else {
			*pdst = (*psrc - 'A' + 10) << 4;
		}

		if( i + 1 < srclen && 0 != *(psrc +1) )
		{
			psrc++;

			// �����4λ
			if(*psrc>='0' && *psrc<='9') {
				*pdst |= *psrc - '0';
			} else {
				*pdst |= *psrc - 'A' + 10;
			}

			psrc++;
			pdst++;
		}
		else
		{
			*pdst &= 0xf0;
			i += 2;
			break;
		}
	}

	int iConvertedLen = int(psrc - psrcfst);
	if( iConvertedLen % 2 )
	{
		PRTMSG( MSG_ERR, "string_to_bytes err, converted srclen(%d) is odd!\n", iConvertedLen );
	}

	// ����Ŀ�����ݳ���
	return i / 2;
}

//----------------------------------------------------------------------------------------------------------------
// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// psrc: Դ����ָ��
// pdst: Ŀ���ַ���ָ��
// srclen: Դ���ݳ���
// ����: Ŀ���ַ�������
static int bytes_to_string(const uchar *psrc, char *pdst, int srclen)
{
	const char tab[] = "0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�

	for(int i=0; i<srclen; i++) 
	{
		*pdst++ = tab[*psrc >> 4];		// �����4λ
		*pdst++ = tab[*psrc & 0x0f];	// �����4λ
		psrc++;
	}

	// ����ַ����Ӹ�������
	*pdst = '\0';

	// ����Ŀ���ַ�������
	return srclen * 2;
}

//----------------------------------------------------------------------------------------------------------------
// 7bit����
// psrc: Դ�ַ���ָ��
// pdst: Ŀ����봮ָ��
// srclen: Դ�ַ�������
// ����: Ŀ����봮����
// �˱���ɷ������160���ַ�
static int encode_7bit(const char *psrc, uchar *pdst, int srclen)
{
	int src;		// Դ�ַ����ļ���ֵ
	int dst;		// Ŀ����봮�ļ���ֵ
	int ch;		// ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
	uchar left;	// ��һ�ֽڲ��������
	
	// ����ֵ��ʼ��
	src = 0;
	dst = 0;
	
	// ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
	// ѭ���ô�����̣�ֱ��Դ����������
	// ������鲻��8�ֽڣ�Ҳ����ȷ����
	while(src<srclen)
	{
		// ȡԴ�ַ����ļ���ֵ�����3λ
		ch = src & 7;
		
		// ����Դ����ÿ���ֽ�
		if(ch == 0) {
			// ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
			left = *psrc;
		} else {
			// ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
			*pdst = (*psrc << (8-ch)) | left;
			// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
			left = *psrc >> ch;
			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pdst++;
			dst++;
		}
		
		// �޸�Դ����ָ��ͼ���ֵ
		psrc++;
		src++;
	}
	
	// ����Ŀ�괮����
	return dst;
}

//----------------------------------------------------------------------------------------------------------------
// 7bit����
// psrc: Դ���봮ָ��
// pdst: Ŀ���ַ���ָ��
// srclen: Դ���봮����
// ����: Ŀ���ַ�������
static int decode_7bit(const uchar *psrc, char *pdst, int srclen)
{
	int src;		// Դ�ַ����ļ���ֵ
	int dst;		// Ŀ����봮�ļ���ֵ
	int bytes;		// ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
	uchar left;		// ��һ�ֽڲ��������
	
	// ����ֵ��ʼ��
	src = 0;
	dst = 0;
	
	// �����ֽ���źͲ������ݳ�ʼ��
	bytes = 0;
	left = 0;
	
	// ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
	// ѭ���ô�����̣�ֱ��Դ���ݱ�������
	// ������鲻��7�ֽڣ�Ҳ����ȷ����
	while( src < srclen )
	{
		// ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
		*pdst = ((*psrc << bytes) | left) & 0x7f;
		// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
		left = *psrc >> (7-bytes);
		// �޸�Ŀ�괮��ָ��ͼ���ֵ
		pdst++;
		dst++;
		// �޸��ֽڼ���ֵ
		bytes++;
		// ����һ������һ���ֽ�
		if(bytes == 7) 
		{
			// ����õ�һ��Ŀ������ֽ�
			*pdst = left;
			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pdst++;
			dst++;
			// �����ֽ���źͲ������ݳ�ʼ��
			bytes = 0;
			left = 0;
		}
		// �޸�Դ����ָ��ͼ���ֵ
		psrc++;
		src++;
	}

	// ����ַ����Ӹ�������
	*pdst = '\0';

	// ����Ŀ�괮����
	return dst;
}

//----------------------------------------------------------------------------------------------------------------
// 8bit����
// psrc: Դ�ַ���ָ��
// pdst: Ŀ����봮ָ��
// srclen: Դ�ַ�������
// ����: Ŀ����봮����
static int encode_8bit(const char *psrc, uchar *pdst, int srclen)
{
	if(srclen<0)	return 0;
	// �򵥸���
	memcpy(pdst, psrc, srclen);

	return srclen;
}

//----------------------------------------------------------------------------------------------------------------
// 8bit����
// psrc: Դ���봮ָ��
// pdst: Ŀ���ַ���ָ��
// srclen: Դ���봮����
// ����: Ŀ���ַ�������
static int decode_8bit(const uchar *psrc, char *pdst, int srclen)
{
	if(srclen<0)	return 0;
	
	// �򵥸���
	memcpy(pdst, psrc, srclen);

	// ����ַ����Ӹ�������
	*pdst = '\0';

	return srclen;
}

//----------------------------------------------------------------------------------------------------------------
// UCS2����
// psrc: Դ�ַ���ָ��
// pdst: Ŀ����봮ָ��
// srclen: Դ�ַ�������
// ����: Ŀ����봮����
// �˱���ɷ������60������
static size_t encode_ucs2(const char *psrc, uchar *pdst, size_t srclen)
{
	size_t iRet;
	size_t dstlen = DSTLEN_MAX;

	iconv_t eConv;
	eConv = iconv_open("UNICODEBIG","GB2312");

	if((iconv_t)(-1) == eConv )
	{
		perror("iconv_open()");
		return 0;
	}
	//que iconv:pdstΪchar
	//���Է���iRet���صĲ���ת���ĳ��ȣ����ǳɹ�����0��ʧ��-1
	iRet = iconv(eConv, &psrc, &srclen, (char **)&pdst, &dstlen);
	if((size_t)(-1) == iRet) 
	{
		perror("iconv()");
		return 0;
	}

	iRet = iconv_close(eConv);
	if ( -1 == iRet ) 
	{
		perror("iconv_close()");
		return 0;
	}
	// ����Ŀ����봮����
	return DSTLEN_MAX - dstlen;	//que size_t->int �Ƿ�������ת����
}

//----------------------------------------------------------------------------------------------------------------
// UCS2����
// psrc: Դ���봮ָ��
// pdst: Ŀ���ַ���ָ��
// srclen: Դ���봮����
// ����: �ֽ��������Ƿ����ַ���
static size_t decode_ucs2(const uchar *psrc, char *pdst, size_t srclen)
{
	size_t iRet;
	size_t dstlen = DSTLEN_MAX;			// UNICODE���ַ���Ŀ
//	const uchar* pTempSrc = psrc;
	char* pTempDst = pdst;

	iconv_t dConv;
	dConv = iconv_open("GB2312","UNICODEBIG");//GB18030?
	if((iconv_t)(-1) == dConv ) 
	{
		perror("iconv_open()");
		return 0;
	}

	if (*(psrc+1)==0 && srclen > 6 )
	{
		psrc = psrc+6;//��ʾΪ������
		srclen -= 6;
	}

	do
	{
		//psrc��pdstָ����iconv�л��Զ�����,�ֱ���תԴ��Ŀ��ת���ĳ���
		//���Է��ֲ���unicode������ַ��޷�ת�����确-����0x20 0x14�����˴�����ת�����ַ��ã����
		iRet = iconv(dConv, (const char**)&psrc, &srclen, &pdst, &dstlen);
		if((size_t)(-1) == iRet)
		{
			perror("iconv()");
			//return 0;
			if ( srclen>=2 )
			{
				PRTMSG(MSG_DBG, "iconv transfer err data:%x %x", *psrc, *(psrc+1) );
				psrc+=2;
// 				*(pdst++) = 0xff;
// 				*(pdst++) = 0x1f;
 				*(pdst++) = 0xa1;
				*(pdst++) = 0xaa;//δʶ����-����
				srclen-=2;
				dstlen-=2;
			}
			
			continue;
		}
	}
	while( srclen>=2 );

	iRet = iconv_close(dConv);
	if ( -1 == iRet ) 
	{
		perror("iconv_close()");
		return 0;
	}

	// ����ַ����Ӹ�������
	pTempDst[DSTLEN_MAX - dstlen] = '\0';
	// ����Ŀ����봮����
	return DSTLEN_MAX - dstlen;	//que size_t->int �Ƿ�������ת����
}

//----------------------------------------------------------------------------------------------------------------
// ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��
// �磺"8613851872468" --> "683158812764F8"
// psrc: Դ�ַ���ָ��
// pdst: Ŀ���ַ���ָ��
// srclen: Դ�ַ�������
// ����: Ŀ���ַ�������
static int invert_numbers(const char *psrc, char *pdst, int srclen)
{
	int dstlen;		// Ŀ���ַ�������
	char ch;		// ���ڱ���һ���ַ�

	// ���ƴ�����
	dstlen = srclen;

	// �����ߵ�
	for(int i=0; i<srclen; i+=2)
	{
		ch = *psrc++;		// �����ȳ��ֵ��ַ�
		*pdst++ = *psrc++;	// ���ƺ���ֵ��ַ�
		*pdst++ = ch;		// �����ȳ��ֵ��ַ�
	}

	// Դ��������������
	if(srclen & 1)
	{
		*(pdst-2) = 'F';	// ��'F'
		dstlen++;			// Ŀ�괮���ȼ�1
	}

	// ����ַ����Ӹ�������
	*pdst = '\0';

	// ����Ŀ���ַ�������
	return dstlen;
}

//----------------------------------------------------------------------------------------------------------------
// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"683158812764F8" --> "8613851872468"
// psrc: Դ�ַ���ָ��
// pdst: Ŀ���ַ���ָ��
// srclen: Դ�ַ�������
// ����: Ŀ���ַ�������
static int serialize_numbers(const char *psrc, char *pdst, int srclen)
{
	int dstlen;			// Ŀ���ַ�������
	char ch;			// ���ڱ���һ���ַ�

	// ���ƴ�����
	dstlen = srclen;

	// �����ߵ�
	for(int i=0; i<srclen;i+=2)
	{
		ch = *psrc++;		// �����ȳ��ֵ��ַ�
		*pdst++ = *psrc++;	// ���ƺ���ֵ��ַ�
		*pdst++ = ch;		// �����ȳ��ֵ��ַ�
	}

	// �����ַ���'F'��
	if(*(pdst-1) == 'F')
	{
		pdst--;
		dstlen--;		// Ŀ���ַ������ȼ�1
	}

	// ����ַ����Ӹ�������
	*pdst = '\0';

	// ����Ŀ���ַ�������
	return dstlen;
}

//v_pLen:�����v_pBuf�������ֽ�������Ҫ����ΪUCS2����ʱ������strlen�����ַ���
bool evdo_encode_pdu(SM_PARAM *v_pSM,char *v_pBuf,int &v_pLen)
{
	
	if (NULL==v_pSM || NULL==v_pBuf)
	{
		return false;
	}
	int iUdLen = strlen(v_pSM->ud);
	if( iUdLen > sizeof(v_pSM->ud) - 1 ) iUdLen = sizeof(v_pSM->ud) - 1;

// 	/// ��ʱ���Է��ͺ�0�ַ�����
// 	{
// 		for( int i = iUdLen; i > 5; i ++ )
// 		{
// 			v_pSM->ud[i] = v_pSM->ud[i-1];
// 		}
// 		v_pSM->ud[5] = 0;
// 		iUdLen ++;
// 	}

	switch(v_pSM->dcs)
	{
	case SMS_7BIT:
		{
			//v_pLen = encode_7bit(v_pSM->ud,(uchar*)v_pBuf,iUdLen + 1);
			memcpy( v_pBuf, v_pSM->ud, iUdLen );
			v_pLen = iUdLen;
		}
		break;
		
	case SMS_UCS2:
		{
			//que ΪʲôҪ��uchar��ʽ��
			v_pLen = encode_ucs2(v_pSM->ud,(uchar*)v_pBuf,iUdLen);
		}
		break;
		
	case SMS_8BIT:
		{
			v_pLen = encode_8bit(v_pSM->ud,(uchar*)v_pBuf,iUdLen);
		}
		break;
		
	default:
		{
			PRTMSG(MSG_ERR,"UnSupported Character Set");
			return false;
		}
		
	}
	
	return true;
}

//psrc:ָ����ŵ��û�����
bool evdo_decode_pdu(const char *psrc, SM_PARAM *pdst)//que ���յ���Ϣ���Ȳ����������
{
	if (NULL==psrc || NULL==pdst)
	{
		return false;
	}

	size_t dstLen;
	uchar buf[256] = {0};
	memcpy(buf,psrc,pdst->udl);
	
	switch(pdst->dcs)
	{
	case SMS_7BIT:
		{
			//pdst->UDL = cdma_decode_7bit(buf,pdst->UD,pdst->UDL);
			memcpy(pdst->ud,psrc,pdst->udl);
			dstLen = pdst->udl;
		}
		break;
		
	case SMS_UCS2:
		{
//			dstLen = decode_ucs2(buf,pdst->ud,pdst->udl*2);//���յ���Ϣ����
			dstLen = decode_ucs2(buf,pdst->ud,pdst->udl);//���յ���Ϣ����
		}
		break;
		
	case SMS_8BIT:
		{
			dstLen = decode_8bit(buf,pdst->ud,pdst->udl);
		}
		break;
		
	default:
		PRTMSG(MSG_ERR,"rcv unknown format SM\n");
		break;
	}
	
	return dstLen;
}


//v_pLen:�����v_pBuf�������ֽ�������Ҫ����ΪUCS2����ʱ������strlen�����ַ���
bool wcdma_encode_pdu(SM_PARAM *v_pSM,char *v_pBuf,int &v_pLen)
{
	
	if (NULL==v_pSM || NULL==v_pBuf)
	{
		return false;
	}
	
	switch(v_pSM->dcs)
	{
	case SMS_7BIT:
		{
			v_pLen = encode_7bit(v_pSM->ud,(uchar*)v_pBuf,strlen(v_pSM->ud));
		}
		break;
		
	case SMS_UCS2:
		{
			//que ΪʲôҪ��uchar��ʽ��
			v_pLen = encode_ucs2(v_pSM->ud,(uchar*)v_pBuf,strlen(v_pSM->ud));
		}
		break;
		
	case SMS_8BIT:
		{
			v_pLen = encode_8bit(v_pSM->ud,(uchar*)v_pBuf,strlen(v_pSM->ud));
		}
		break;
		
	default:
		{
			PRTMSG(MSG_ERR,"UnSupported Character Set");
			return false;
		}
		
	}
	
	return true;
}

bool wcdma_decode_pdu(const char *psrc, SM_PARAM *pdst)//que ���յ���Ϣ���Ȳ����������
{
	if (NULL==psrc || NULL==pdst)
	{
		return false;
	}
	
	size_t dstLen;
	uchar buf[256] = {0};
	memcpy(buf,psrc,pdst->udl);
	
	switch(pdst->dcs)
	{
	case SMS_7BIT:
		{
			//pdst->UDL = cdma_decode_7bit(buf,pdst->UD,pdst->UDL);
			memcpy(pdst->ud,psrc,pdst->udl);
			dstLen = pdst->udl;
		}
		break;
		
	case SMS_UCS2:
		{
			//			dstLen = decode_ucs2(buf,pdst->ud,pdst->udl*2);//���յ���Ϣ����
			dstLen = decode_ucs2(buf,pdst->ud,pdst->udl);//���յ���Ϣ����
		}
		break;
		
	case SMS_8BIT:
		{
			dstLen = decode_8bit(buf,pdst->ud,pdst->udl);
		}
		break;
		
	default:
		PRTMSG(MSG_ERR,"rcv unknown format SM\n");
		break;
	}
	
	return dstLen;
}

//----------------------------------------------------------------------------------------------------------------
// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// psrc: ԴPDU����ָ��
// pdst: Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int encode_pdu(SM_PARAM *psrc, char *pdst)
{
	int len=0;			// �ڲ��õĴ�����
	int dstlen=0;			// Ŀ��PDU������
	uchar buf[256]={0};	// �ڲ��õĻ�����

	// SMSC��ַ��Ϣ��(dkm del)
//	len = strlen(psrc->sca);	// SMSC��ַ�ַ����ĳ���	
//	buf[0] = (char)((len & 1) == 0 ? len : len + 1) / 2 + 1;	// SMSC��ַ��Ϣ����
//	buf[1] = 0x91;		// �̶�: �ù��ʸ�ʽ����
//	dstlen = bytes_to_string(buf, pdst, 2);		// ת��2���ֽڵ�Ŀ��PDU��
//	dstlen += invert_numbers(psrc->sca, &pdst[dstlen], len);	// ת��SMSC���뵽Ŀ��PDU��

	// TPDU�λ���������Ŀ���ַ��
	len = strlen(psrc->tpa);		// TP-DA��ַ�ַ����ĳ���

	buf[0] = 0x00;					// (dkm add)
	buf[1] = 0x11;					// �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
	buf[2] = 0x00;					// TP-MR=0
	buf[3] = (char)len;				// Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
	buf[4] = 0x91;					// �̶�: �ù��ʸ�ʽ����
	dstlen = bytes_to_string(buf, pdst, 5);		// ת��4���ֽڵ�Ŀ��PDU��
	dstlen += invert_numbers(psrc->tpa, &pdst[dstlen], len);	// ת��TP-DA��Ŀ��PDU��

	// ȷ���û���Ϣ�ı��뷽ʽ (dkm add)
	bool unicode = false;
	for(size_t i=0; i<strlen(psrc->ud); i++) {
		if((byte)psrc->ud[i] >= 0x80)	  { unicode=true;  break; }
	}
	if(unicode)		psrc->dcs = SMS_UCS2;
	else			psrc->dcs = SMS_7BIT;

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	len = strlen(psrc->ud);		// �û���Ϣ�ַ����ĳ���
// 	{
//		/// ��ʱ�����ַ�0�Ƿ���Է�����
// 		for( size_t i = len; i > 5; i -- )
// 		{
// 			psrc->ud[i] = psrc->ud[i - 1];
// 		}
// 		psrc->ud[5] = 0;
// 		len ++;
// 	}
	if( len > sizeof(psrc->ud) - 1 ) len = sizeof(psrc->ud) - 1;
	buf[0] = psrc->pid;			// Э���ʶ(TP-PID)
	buf[1] = psrc->dcs;			// �û���Ϣ���뷽ʽ(TP-DCS)
	buf[2] = 0;					// ��Ч��(TP-VP)Ϊ5����
	if(psrc->dcs == SMS_7BIT)	
	{
		// 7-bit���뷽ʽ
		buf[3] = len;			// ����ǰ����
		len = encode_7bit(psrc->ud, &buf[4], len+1) + 4;	// ת��TP-DA��Ŀ��PDU�� (ת������Ҫ��һ��������0��������ܻᶪʧ����)
	}
	else if(psrc->dcs == SMS_UCS2)
	{
		// UCS2���뷽ʽ
		buf[3] = encode_ucs2(psrc->ud, &buf[4], len);	// ת��TP-DA��Ŀ��PDU��
		len = buf[3] + 4;		// len���ڸö����ݳ���
	}
	else
	{
		// 8-bit���뷽ʽ
		buf[3] = encode_8bit(psrc->ud, &buf[4], len);	// ת��TP-DA��Ŀ��PDU��
		len = buf[3] + 4;		// len���ڸö����ݳ���
	}
	dstlen += bytes_to_string(buf, &pdst[dstlen], len);		// ת���ö����ݵ�Ŀ��PDU��
	
	// ����Ŀ���ַ�������
	return dstlen;
}




//----------------------------------------------------------------------------------------------------------------
// PDU���룬���ڽ��ա��Ķ�����Ϣ
// psrc: ԴPDU��ָ��
// pdst: Ŀ��PDU����ָ��
// ����: �û���Ϣ������
int decode_pdu(const char *psrc, SM_PARAM *pdst)
{
	int dstlen = 0;			// Ŀ��PDU������
	uchar tmp;			// �ڲ��õ���ʱ�ֽڱ���
	uchar buf[256] = { 0 };		// �ڲ��õĻ�����

	// SMSC��ַ��Ϣ��
	string_to_bytes(psrc, &tmp, 2);	// ȡ����
	tmp = (tmp - 1) * 2;	// SMSC���봮����
	psrc += 4;			// ָ����ƣ�������SMSC��ַ��ʽ
	serialize_numbers(psrc, pdst->sca, tmp);	// ת��SMSC���뵽Ŀ��PDU��
	psrc += tmp;		// ָ�����

	// TPDU�λ����������ظ���ַ��
	string_to_bytes(psrc, &tmp, 2);	// ȡ��������
	psrc += 2;		// ָ�����

	//if(tmp & 0x04)
	if(tmp)
	{
		// �����ظ���ַ��ȡ�ظ���ַ��Ϣ
		string_to_bytes(psrc, &tmp, 2);	// ȡ����
		if(tmp & 1) tmp += 1;	// ������ż��
		psrc += 4;			// ָ����ƣ������˻ظ���ַ(TP-RA)��ʽ
		serialize_numbers(psrc, pdst->tpa, tmp);	// ȡTP-RA����
		psrc += tmp;		// ָ�����
		PRTMSG(MSG_DBG, "SM from:%.16s", pdst->tpa);
	}

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	string_to_bytes(psrc, (uchar*)&pdst->pid, 2);	// ȡЭ���ʶ(TP-PID)
	psrc += 2;		// ָ�����
	string_to_bytes(psrc, (uchar*)&pdst->dcs, 2);	// ȡ���뷽ʽ(TP-DCS)
	psrc += 2;		// ָ�����
	serialize_numbers(psrc, pdst->scts, 14);		// ����ʱ����ַ���(TP_SCTS) 
	psrc += 14;		// ָ�����
	string_to_bytes(psrc, &tmp, 2);	// �û���Ϣ����(TP-UDL)
	psrc += 2;		// ָ�����
	
	if( (pdst->dcs&0x0c)/4==0 )		
	{
		// 7-bit����
		dstlen = string_to_bytes(psrc, buf, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);	// ��ʽת��
		decode_7bit(buf, pdst->ud, dstlen);	// ת����TP-DU
		dstlen = tmp;
	}
	else if( (pdst->dcs&0x0c)/4==2 )
	{
		// UCS2����
		dstlen = string_to_bytes(psrc, buf, tmp * 2);	// ��ʽת��
// 		PRTMSG(MSG_DBG, "testc sms ud:");
// 		PrintString((char*)buf,dstlen);
		memset(pdst->ud,0,sizeof(pdst->ud));
		dstlen = decode_ucs2(buf, pdst->ud, dstlen);	// ת����TP-DU
	}
	else if( (pdst->dcs&0x0c)/4==1 )
	{
		// 8-bit����
		dstlen = string_to_bytes(psrc, buf, tmp * 2);	// ��ʽת��
		dstlen = decode_8bit(buf, pdst->ud, dstlen);	// ת����TP-DU
	}

	// ����Ŀ���ַ�������
	return dstlen;
}


