/*******************************************************************
*
*   �ļ���:     YX_GPIO_API.h
*   ��Ȩ:       ������Ѹ����ɷ����޹�˾
*   ������:     �θ�    2009��5��18��
*
*   �汾��:     1.0
*   �ļ�����:   HIƽ̨GPIOӦ�ýӿ�
*
*******************************************************************/
#ifndef _LINUX_YX_GPIO_API_H_
#define _LINUX_YX_GPIO_API_H_

#include "yx_gpiodrv.h"
#include "yx_system.h"

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
