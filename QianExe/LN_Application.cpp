#include "yx_QianStdAfx.h"

#if USE_LIAONING_SANQI == 1

void *G_ThreadApplicationRecv(void *arg)
{
	g_objApplication.P_ThreadRecv();
}

//////////////////////////////////////////////////////////////////////////

#undef MSG_HEAD
#define MSG_HEAD	("QianExe-LN_App")

CLN_Application::CLN_Application()
{
	m_objRecvDataMng.InitCfg(10*1024, 30*1000);
}

CLN_Application::~CLN_Application()
{

}

int CLN_Application::Init()
{
	//�����߳�
	pthread_t threadRecv;
	if( pthread_create(&threadRecv, NULL, G_ThreadApplicationRecv, NULL) != 0	)
	{
		PRTMSG(MSG_ERR, "create Application thread failed!\n");
	}
}

int CLN_Application::Release()
{

}

int CLN_Application::MakeAppFrame(char v_szCmd, short v_shAction, char *v_szSrcBuf, DWORD v_dwSrcLen, char *v_szDesBuf, DWORD v_dwDesSize, DWORD &v_dwDesLen)
{
	// Ŀ�껺�������ȼ��
	if( v_dwDesSize < v_dwSrcLen + 3 )
		return ERR_BUFLACK;

	v_dwDesLen = 0;
	v_szDesBuf[v_dwDesLen++] = v_szCmd;
	
	G_LocalToNet((char*)&v_shAction, v_szDesBuf+v_dwDesLen, 2);
	v_dwDesLen += 2;

	memcpy(v_szDesBuf+v_dwDesLen, v_szSrcBuf, v_dwSrcLen);
	v_dwDesLen += v_dwSrcLen;

	return 0;
}

int CLN_Application::SendAppFrame(char v_szModeType, char v_szControlByte, bool v_bUseLocalSEQ, DWORD v_dwCenterSEQ, char *v_szData, DWORD v_dwDataLen)
{
	char  szBuf[UDP_SENDSIZE] = {0};
	DWORD dwLen = 0;
	DWORD dwPacket = 0;

	szBuf[dwLen++] = v_szModeType;					// ����ģʽ
	szBuf[dwLen++] = v_szControlByte;				// ������
	szBuf[dwLen++] = (v_bUseLocalSEQ ? 1 : 0);		// �Ƿ�ʹ�ñ���SEQ��ˮ��
	memcpy(szBuf+dwLen, (void*)&v_dwCenterSEQ, 4);	// ������Ӧ��ʱʹ�õ�SEQ��ˮ��
	dwLen += 4;
	memcpy(szBuf+dwLen, v_szData, v_dwDataLen);		// Ӧ�ò�����
	dwLen += v_dwDataLen;

	//������֡���봫���ķ��Ͷ���
	g_objTransport.m_objSendDataMng.PushData(LV2, dwLen, szBuf, dwPacket);
}

void CLN_Application::P_ThreadRecv()
{
	char  szPopBuf[UDP_SENDSIZE] = {0};
	DWORD dwPopLen = 0;
	byte  bytLvl = 0;
	DWORD dwPushTm = 0;

	DWORD dwCenterSEQ = 0;

	char  szTemBuf[2] = {0};
	short shAction = 0;
	
	while( !g_bProgExit )
	{
		usleep(100000);
		if( !m_objRecvDataMng.PopData(bytLvl, sizeof(szPopBuf), dwPopLen, szPopBuf, dwPushTm) )
		{
			// SEQ��ˮ��
			memcpy((void*)&dwCenterSEQ, szPopBuf, 4);

			// ��������
			szTemBuf[0] = szPopBuf[6];
			szTemBuf[1] = szPopBuf[5];
			memcpy((void*)&shAction, szTemBuf, 2);

			switch(szPopBuf[4])
			{
			case 0x01:	// �ն˿��ƶ���
				{
					switch(shAction)
					{
					case 0x0001:	// �ն˳�ʼ��
						g_objLNConfig.Deal01_0001(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;

					case 0x0002:	// ����
						g_objLNConfig.Deal01_0002(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;

					case 0x0004:	// ��·����
						g_objLNConfig.Deal01_0004(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;

					case 0x0006:	// ����
						g_objLNConfig.Deal01_0006(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0007:	// GPS����
						g_objLNConfig.Deal01_0007(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					default:
						break;
					}					
				}
				break;

			case 0x02:	// ������ƶ���
				{
					switch(shAction)
					{
					case 0x0002:	// 
						g_objLNPhoto.Deal02_0002(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0003:	// 
						g_objLNPhoto.Deal02_0003(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					default:
						break;
					}					
				}
				break;

			case 0x03:	// �������ݷ���
				break;

			case 0x04:	// �������ݻ�ȡ
				{
					switch(shAction)
					{
					case 0x0001:	// 
						g_objLNPhoto.Deal04_0001(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0002:	// 
						g_objLNPhoto.Deal04_0002(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					default:
						break;
					}					
				}
				break;

			case 0x05:	// �ն˿��ƶ���
				break;

			case 0x06:	// �������Բ�ѯ
				break;

			case 0x07:	// �ն�״̬��ѯ
				break;
				
			case 0x08:	// ����״̬��ѯ
				break;

			case 0x09:	// �ն�����
				{
					switch(shAction)
					{
					case 0x0001:	// �������
						g_objLNConfig.Deal09_0001(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
						
					case 0x0008:	// �����绰��������
						g_objLNConfig.Deal09_0008(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
						
					case 0x0009:	// ������������
						g_objLNConfig.Deal09_0009(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
						
					case 0x0010:	// ���Ӳ�������
						g_objLNConfig.Deal09_0010(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;

					case 0x0011:	// �����������
						g_objLNConfig.Deal09_0011(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
						
					case 0x0012:	// �����������
						g_objLNConfig.Deal09_0012(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
						
					default:
						break;
					}
				}
				break;
				
			case 0x0a:	// �ն����ò�ѯ
				break;

			case 0x0b:	// ��������
				break;
				
			case 0x0c:	// �������ò�ѯ
				break;

			case 0x0d:	// ����λ���ϱ���������
				{
					g_objLNConfig.Deal0D_0000(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
				}
				break;
				
			case 0x0e:	// ����λ���ϱ������Ƴ�
				{
					g_objLNConfig.Deal0E_0000(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
				}
				break;

			case 0x11:	// ���津����������
				{
					switch(shAction)
					{
					case 0x0001: //������������
							g_objLNConfig.Deal11_0001(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;	
					case 0x0002: //��̱�������
							g_objLNConfig.Deal11_0002(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;	
					case 0x0003:	
							g_objLNConfig.Deal11_0003(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0005: //��ʱͣ����������
							g_objLNConfig.Deal11_0005(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0006:	// ���򱨸�����
							g_objLNConfig.Deal11_0006(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0007:	// �ٶ��쳣����
							g_objLNConfig.Deal11_0007(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0008: //ʱ����ٶ��쳣��������
							g_objLNConfig.Deal11_0008(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0009: //ƫ����������
							g_objLNConfig.Deal11_0009(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x000e:	//����Դ�ϵ籨��
							g_objLNConfig.Deal11_000E(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x000f:	// ��𱨸�����
							g_objLNConfig.Deal11_000F(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;	
					case 0x0014: //ƣ�ͱ�������
							g_objLNConfig.Deal11_0014(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0016: //��ʱ�������ٱ�������
							g_objLNConfig.Deal11_0016(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0018: //��ʱ��ƫ����������
							g_objLNConfig.Deal11_0018(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0019: //����ʱͣ����������
							g_objLNConfig.Deal11_0019(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0022:	//�����򱨸�����
							g_objLNConfig.Deal11_0022(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0023: //�����򱨸�����
							g_objLNConfig.Deal11_0023(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0024: //�����ڱ�������
							g_objLNConfig.Deal11_0024(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0025: //�����ⱨ������
							g_objLNConfig.Deal11_0025(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0026: //�����ڳ��ٱ�������
							g_objLNConfig.Deal11_0026(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0027: //�����ⳬ�ٱ�������
							g_objLNConfig.Deal11_0027(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0028: //�����ڳ�ʱͣ����������
							g_objLNConfig.Deal11_0028(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;	
					case 0x0029: //
							g_objLNConfig.Deal11_0029(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;	
					default:
						break;
					}
				}
				break;
				
			case 0x12:	// ���津�������Ƴ�
				{
					g_objLNConfig.Deal12(szPopBuf+5, dwPopLen-5, dwCenterSEQ);
				}
				break;

			case 0x15:	// ���洦��
				{
					g_objLNConfig.Deal15(szPopBuf+5, dwPopLen-5, dwCenterSEQ);

				}
				break;

			case 0x1a:	// ��Ϣ�·�
				break;

			case 0x1b:	// ����
				{
					switch(shAction)
					{
					case 0x0001: // Զ������
						g_objTransport.Deal1B_0001(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					case 0x0002: // Զ����������
						g_objTransport.Deal1B_0002(szPopBuf+7, dwPopLen-7, dwCenterSEQ);
						break;
					default:
						break;
					}
				}
				break;

			case 0x41:	// �ն�
				{
					switch(shAction)
					{
					case 0x0000:	// ��ע��Ӧ��
						g_objLogin.RegisterAnswer( szPopBuf+7, dwPopLen-7 );
						break;

					case 0x0001:	// ��½Ӧ��
						g_objLogin.LoginAnswer( szPopBuf+7, dwPopLen-7 );
						break;

					case 0x0003:	// ��������
						g_objLogin.HeartKeepAnswer( szPopBuf+7, dwPopLen-7 );
						break;

					case 0x0004:	// �Լ�����
						break;

					default:
						break;
					}
				}
				break;
			case 0x42:	// 
				{
					switch(shAction)
					{
						case 0x0004:	// 
						g_objLNPhoto.Deal42_0004(szPopBuf+7, dwPopLen-7, dwCenterSEQ);				
						break;

					default:
						break;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

#endif

