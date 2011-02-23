#ifndef _YX_GPIO_H_
#define _YX_GPIO_H_

#include <linux/ioctl.h>
#include <linux/types.h>
#include "yx_system.h"

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
#define SINGLE_EDGE_TRIGGER             0
#define DOUBLE_EDGE_TRIGGER             1
// �������Ŵ�������
#define FALL_EDGE_TRIGGER               0
#define RAISE_EDGE_TRIGGER              1


/*����������ȡGPIO���ŵ�������Ϣ*/
typedef struct 
{
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


void YX_GPIO_Init(void);
void YX_GPIO_SetDirection(INT32U gpio_id, BOOLEAN directions);
BOOLEAN YX_GPIO_GetDirection(INT32U gpio_id);  
void YX_GPIO_SetValue(INT32U gpio_id, BOOLEAN b_on);
BOOLEAN YX_GPIO_GetValue( INT32U gpio_id );
void YX_GPIO_Release(void);
void YX_GPIO_INTEnable(INT32U gpio_id);
void YX_GPIO_INTDisable(INT32U gpio_id);  
void YX_GPIO_INTConfig(INT32U gpio_id, INT32U mode, INT32U edge, INT32U level);
void YX_GPIO_INTSetMode(INT32U gpio_id, INT32U mode);
void YX_GPIO_INTSetEdge(INT32U gpio_id, INT32U edge);
void YX_GPIO_INTSetLevel(INT32U gpio_id, INT32U level);
void YX_GPIO_INTSethandler(INT32U gpio_id, void (*inthandler)(INT32U, INT32U ));

#endif
