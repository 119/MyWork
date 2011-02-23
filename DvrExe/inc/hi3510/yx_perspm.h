/*******************************************************************
*
*   �ļ���:     yx_perspm.h
*   ��Ȩ:       ������Ѹ����ɷ����޹�˾
*   ������:     �θ�    2009��5��20��
*
*   �汾��:     1.0
*   �ļ�����:   HIƽ̨����Χ��Դ����ģ��ͷ�ļ�
*
*******************************************************************/
#ifndef _LINUX_YX_PERSPM_H_
#define _LINUX_YX_PERSPM_H_

#include <linux/ioctl.h>
#include <linux/types.h>

#define	PERSPM_IOCTL_BASE	'P'

#define PERSPMIOC_SETCLKENABLE      _IOW(PERSPM_IOCTL_BASE, 50, int)
#define PERSPMIOC_SETCLKDISABLE     _IOW(PERSPM_IOCTL_BASE, 51, int)
#define PERSPMIOC_GETCLKSTATE       _IOR(PERSPM_IOCTL_BASE, 52, int)

#define PERSPMIOC_GETSYSINFO        _IOR(PERSPM_IOCTL_BASE, 53, int)
#define PERSPMIOC_DDRC_SETDISABLE   _IO(PERSPM_IOCTL_BASE, 54)
#define PERSPMIOC_SETSYSSTATUS      _IOW(PERSPM_IOCTL_BASE, 55, int)
#define PERSPMIOC_SETSYSRESET       _IO(PERSPM_IOCTL_BASE, 56)

#endif
