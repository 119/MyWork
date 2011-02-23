/*******************************************************************
*
*   �ļ���:     yx_gpiodrv.h
*   ��Ȩ:       ������Ѹ����ɷ����޹�˾
*   ������:     �θ�    2009��5��11��
*
*   �汾��:     1.0
*   �ļ�����:   HIƽ̨GPIO����ģ��ͷ�ļ�
*
************************�޸ļ�¼1************************************
*
*	�޸���:		������	2009��11��18��
*	�汾�ţ�	1.1
*	
*	�޸�������	������Hi3511ƽ̨��gpio���üĴ����Ķ�д
**********************************************************************/

#ifndef _LINUX_YX_GPIODRV_H_
#define _LINUX_YX_GPIODRV_H_

#include <linux/ioctl.h>
#include <linux/types.h>
//#include <asm/arch/platform.h>
//#include <asm/arch/hardware.h>

// �������������������
#define GPIO_INPUT                      0
#define GPIO_OUTPUT                     1
// �������Ÿߵ͵�ƽ
#define GPIO_PIN_LOW                    0
#define GPIO_PIN_HIGH                   1
// �������Ŵ���ģʽ
#define INT_TRIGGER_EDGE                0               
#define INT_TRIGGER_LEVEL               1
// �������Ŵ�����������
#define DOUBLE_EDGE_TRIGGER             1
#define SINGLE_EDGE_TRIGGER             0
// �������Ŵ�������
#define FALL_EDGE_TRIGGER               0
#define RAISE_EDGE_TRIGGER              1


/*����������ȡGPIO���ŵ�������Ϣ*/
typedef struct {
	__u32 direction;        // ���ŷ���
	__u32 data;             // ���ŵ�ƽ
	__u32 irqenable;        // �ж�ʹ��
	__u32 irq_is;           // �жϴ���ģʽ
	__u32 irq_ibe;          // �жϱ���ģʽ 
	__u32 irq_iev;          // �жϵ�ƽģʽ 
// ����Ϊ���Զ���
	__u32 irq_mis;	      // ��ǰ�ж�״̬			          
} GPIO_INFO_T;

#define	GPIODRV_IOCTL_BASE	'G'

#define GPIOIOC_SETDIRECTION    _IOW(GPIODRV_IOCTL_BASE, 0, int)
#define GPIOIOC_SETPINVALUE     _IOW(GPIODRV_IOCTL_BASE, 1, int)
#define GPIOIOC_SETTRIGGERMODE  _IOW(GPIODRV_IOCTL_BASE, 2, int)
#define GPIOIOC_SETTRIGGERIBE   _IOW(GPIODRV_IOCTL_BASE, 3, int)
#define GPIOIOC_SETTRIGGERIEV   _IOW(GPIODRV_IOCTL_BASE, 4, int)
#define GPIOIOC_SETIRQALL       _IOW(GPIODRV_IOCTL_BASE, 5, int)
#define GPIOIOC_SETCALLBACK     _IOW(GPIODRV_IOCTL_BASE, 6, int)
#define GPIOIOC_ENABLEIRQ       _IO(GPIODRV_IOCTL_BASE,  7)
#define GPIOIOC_DISABLEIRQ      _IO(GPIODRV_IOCTL_BASE,  8)
#define GPIOIOC_GETPININFO      _IOR(GPIODRV_IOCTL_BASE, 9, GPIO_INFO_T)
#define GPIOIOC_GETCALLBACK     _IOR(GPIODRV_IOCTL_BASE, 10, int)

#define YX_GPIO_GET_SC_PERCTRL1   (*(volatile unsigned long *)(IO_ADDRESS(REG_BASE_SCTL)+0x20))
#define YX_GPIO_SET_SC_PERCTRL1(v)  do{ *(volatile unsigned long *)(IO_ADDRESS(REG_BASE_SCTL)+0x20)=v;} while(0)
#define YX_GPIO_SC_PERCTRL1(g,b)      YX_GPIO_SET_SC_PERCTRL1 ((YX_GPIO_GET_SC_PERCTRL1 & (~(1<<(g)))) | ((b)<<(g)))
#endif

