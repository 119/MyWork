#include "yx_common.h"

BYTE g_bytCPageID;/*��ǰҳ����*/
BYTE g_bytCItemID;/*��ǰѡ����*/
BYTE g_bytLItemID_L0Page;/*��ǰҳ��һѡ����*/
BYTE g_bytLItemID_L1Page;/*��һҳ��һѡ����*/
BYTE g_bytLItemID_L2Page;/*�϶�ҳ��һѡ����*/
BYTE g_bytLItemID_L3Page;/*����ҳ��һѡ����*/

MENU_OSD g_objMenuOsd;

MENU_PAGE	g_objMenuPage[] = 
{
	{LANG_PAGE_00, {0,0}, WHITE, BLUE, 0x00, 2, NULL, {0,NULL}},
	{LANG_PAGE_01, {0,0}, WHITE, BLUE, 0x01, 6, NULL, {0,NULL}},
	{LANG_PAGE_02, {0,0}, WHITE, BLUE, 0x02, 4, NULL, {0,NULL}},
	{LANG_PAGE_03, {0,0}, WHITE, BLUE, 0x03, 18,NULL, {0,NULL}},
	{LANG_PAGE_04, {0,0}, WHITE, BLUE, 0x04, 18,NULL, {0,NULL}},
	{LANG_PAGE_05, {0,0}, WHITE, BLUE, 0x05, 2, NULL, {0,NULL}},
	{LANG_PAGE_06, {0,0}, WHITE, BLUE, 0x06, 3, NULL, {0,NULL}},
	{LANG_PAGE_07, {0,0}, WHITE, BLUE, 0x07, 13,NULL, {0,NULL}},
	{LANG_PAGE_08, {0,0}, WHITE, BLUE, 0x08, 13,NULL, {0,NULL}},
	{LANG_PAGE_09, {0,0}, WHITE, BLUE, 0x09, 4, NULL, {0,NULL}},
	{LANG_PAGE_0a, {0,0}, WHITE, BLUE, 0x0a, 4, NULL, {0,NULL}},
	{LANG_PAGE_0b, {0,0}, WHITE, BLUE, 0x0b, 13,NULL, {0,NULL}},
	{LANG_PAGE_0c, {0,0}, WHITE, BLUE, 0x0c, 13,NULL, {0,NULL}},
	{LANG_PAGE_0d, {0,0}, WHITE, BLUE, 0x0d, 3, NULL, {0,NULL}},
	{LANG_PAGE_0e, {0,0}, WHITE, BLUE, 0x0e, 22,NULL, {0,NULL}},
	{LANG_PAGE_0f, {0,0}, WHITE, BLUE, 0x0f, 67,NULL, {0,NULL}},
	{LANG_PAGE_10, {0,0}, WHITE, BLUE, 0x10, 15,NULL, {0,NULL}},
	{LANG_PAGE_11, {0,0}, WHITE, BLUE, 0x11, 12,NULL, {0,NULL}}
};

MENU_ITEM g_objMenuItem[][70] = 
{
	{
		{{0}, {11,24}, WHITE, PURPLE, FALSE, TRUE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, {0}, {0}, {0}, {0}},
		{{0}, {11,36}, WHITE, MODENA, FALSE, TRUE, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, {0}, {0}, {"�������! \n", {14,24}, RED, BLUE}, {0}}
	},

	{
		{{0}, {6,22},  WHITE, MODENA, FALSE, TRUE, 0x01, 0x00, 0x05, 0x01, 0x00, 0x00, 0x02, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {8,22},  WHITE, MODENA, FALSE, TRUE, 0x01, 0x01, 0x00, 0x02, 0x01, 0x01, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {10,22}, WHITE, MODENA, FALSE, TRUE, 0x01, 0x02, 0x01, 0x03, 0x02, 0x02, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {12,22}, WHITE, MODENA, FALSE, TRUE, 0x01, 0x03, 0x02, 0x04, 0x03, 0x03, 0x05, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {14,22}, WHITE, MODENA, FALSE, TRUE, 0x01, 0x04, 0x03, 0x05, 0x04, 0x04, 0x06, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {16,22}, WHITE, MODENA, FALSE, TRUE, 0x01, 0x05, 0x04, 0x00, 0x05, 0x05, 0x07, 0x01, {0}, {0}, {0}, {0}}
	},

	{
		{{0}, {8,22},  WHITE, MODENA, FALSE, TRUE, 0x02, 0x00, 0x03, 0x01, 0x00, 0x00, 0x08, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {10,22}, WHITE, MODENA, FALSE, TRUE, 0x02, 0x01, 0x00, 0x02, 0x01, 0x01, 0x09, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {12,22}, WHITE, MODENA, FALSE, TRUE, 0x02, 0x02, 0x01, 0x03, 0x02, 0x02, 0x11, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {14,22}, WHITE, MODENA, FALSE, TRUE, 0x02, 0x03, 0x02, 0x00, 0x03, 0x03, 0x0a, 0x01, {0}, {0}, {0}, {0}}
	},

	{
		{{0}, {9,16},  WHITE, PURPLE, FALSE, TRUE, 0x03, 0x00, 0x00, 0x06, 0x00, 0x01, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,40},  WHITE, PURPLE, FALSE, TRUE, 0x03, 0x01, 0x01, 0x0b, 0x00, 0x03, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,44},  WHITE, PURPLE, FALSE, TRUE, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,46},  WHITE, PURPLE, FALSE, TRUE, 0x03, 0x03, 0x03, 0x0d, 0x01, 0x05, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,48},  WHITE, PURPLE, FALSE, TRUE, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,50},  WHITE, PURPLE, FALSE, TRUE, 0x03, 0x05, 0x05, 0x0f, 0x03, 0x06, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,16}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x06, 0x00, 0x10, 0x05, 0x08, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,18}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,20}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x08, 0x00, 0x10, 0x06, 0x0a, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,22}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x09, 0x09, 0x09, 0x09, 0x09, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,24}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x0a, 0x00, 0x10, 0x08, 0x0b, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,40}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x0b, 0x01, 0x11, 0x0a, 0x0d, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,44}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,46}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x0d, 0x03, 0x11, 0x0b, 0x0f, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,48}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,50}, WHITE, PURPLE, FALSE, TRUE, 0x03, 0x0f, 0x05, 0x11, 0x0d, 0x10, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {13,16}, WHITE, MODENA, FALSE, TRUE, 0x03, 0x10, 0x06, 0x11, 0x0f, 0x11, 0x03, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {15,48}, WHITE, MODENA, FALSE, TRUE, 0x03, 0x11, 0x0b, 0x11, 0x10, 0x11, 0x0b, 0x01, {0}, {0}, {"�ļ�������! \n", {15,24}, RED, BLUE}, {0}}
	},

	{
		{{0}, {9,16},  WHITE, PURPLE, FALSE, TRUE, 0x04, 0x00, 0x00, 0x06, 0x00, 0x01, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,40},  WHITE, PURPLE, FALSE, TRUE, 0x04, 0x01, 0x01, 0x0b, 0x00, 0x03, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,44},  WHITE, PURPLE, FALSE, TRUE, 0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,46},  WHITE, PURPLE, FALSE, TRUE, 0x04, 0x03, 0x03, 0x0d, 0x01, 0x05, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,48},  WHITE, PURPLE, FALSE, TRUE, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,50},  WHITE, PURPLE, FALSE, TRUE, 0x04, 0x05, 0x05, 0x0f, 0x03, 0x06, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,16}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x06, 0x00, 0x10, 0x05, 0x08, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,18}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x07, 0x07, 0x07, 0x07, 0x07, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,20}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x08, 0x00, 0x10, 0x06, 0x0a, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,22}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x09, 0x09, 0x09, 0x09, 0x09, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,24}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x0a, 0x00, 0x10, 0x08, 0x0b, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,40}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x0b, 0x01, 0x11, 0x0a, 0x0d, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,44}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,46}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x0d, 0x03, 0x11, 0x0b, 0x0f, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,48}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,50}, WHITE, PURPLE, FALSE, TRUE, 0x04, 0x0f, 0x05, 0x11, 0x0d, 0x10, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {13,16}, WHITE, MODENA, FALSE, TRUE, 0x04, 0x10, 0x06, 0x11, 0x0f, 0x11, 0x04, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {15,48}, WHITE, MODENA, FALSE, TRUE, 0x04, 0x11, 0x0b, 0x11, 0x10, 0x11, 0x0c, 0x01, {0}, {0}, {"¼�񲻴���! \n", {15,24}, RED, BLUE}, {0}}
	},

	{
		{{0}, {8,30},  WHITE, PURPLE, FALSE, TRUE, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00, 0x05, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,22}, WHITE, MODENA, FALSE, TRUE, 0x05, 0x01, 0x00, 0x01, 0x01, 0x01, 0x05, 0x01, {"�������ڸ�ʽ��! \n", {18,22}, RED, BLUE}, {"���̸�ʽ���ɹ�! \n", {18,22}, RED, BLUE}, {"���̸�ʽ��ʧ��! \n", {18,22}, RED, BLUE}, {0}}
	},

	{
		{{0}, {4,22},  WHITE, MODENA, FALSE, TRUE, 0x06, 0x00, 0x02, 0x01, 0x00, 0x00, 0x06, 0x01, {0}, {"�ָ��������óɹ�! \n", {9,20}, RED, BLUE}, {"�ָ���������ʧ��! \n", {9,20}, RED, BLUE}, {0}},
		{{0}, {11,24}, WHITE, MODENA, FALSE, TRUE, 0x06, 0x01, 0x00, 0x02, 0x01, 0x01, 0x06, 0x01, {"Ӧ��������,����ϵ�!\n", {15,19}, RED, BLUE}, {"Ӧ�������ɹ�! \n", {15,22}, RED, BLUE}, {"Ӧ������ʧ��! \n", {15,22}, RED, BLUE}, {0}},
		{{0}, {17,24}, WHITE, MODENA, FALSE, TRUE, 0x06, 0x02, 0x01, 0x00, 0x02, 0x02, 0x06, 0x01, {"ϵͳ������,����ϵ�!\n", {21,19}, RED, BLUE}, {"ϵͳ�����ɹ�! \n", {21,22}, RED, BLUE}, {"ϵͳ����ʧ��! \n", {21,22}, RED, BLUE}, {0}}
	},

	{
		{{0}, {4,22},  WHITE, PURPLE, FALSE, TRUE, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,10},  WHITE, PURPLE, FALSE, TRUE, 0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,22},  WHITE, PURPLE, FALSE, TRUE, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,32},  WHITE, PURPLE, FALSE, TRUE, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {9,42},  WHITE, PURPLE, FALSE, TRUE, 0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,10}, WHITE, PURPLE, FALSE, TRUE, 0x07, 0x05, 0x05, 0x05, 0x05, 0x05, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,22}, WHITE, PURPLE, FALSE, TRUE, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,32}, WHITE, PURPLE, FALSE, TRUE, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {11,42}, WHITE, PURPLE, FALSE, TRUE, 0x07, 0x08, 0x08, 0x08, 0x08, 0x08, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {13,10}, WHITE, PURPLE, FALSE, TRUE, 0x07, 0x09, 0x09, 0x09, 0x09, 0x09, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {13,22}, WHITE, PURPLE, FALSE, TRUE, 0x07, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {13,32}, WHITE, PURPLE, FALSE, TRUE, 0x07, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x07, 0x01, {0}, {0}, {0}, {0}},
		{{0}, {13,42}, WHITE, PURPLE, FALSE, TRUE, 0x07, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x07, 0x01, {0}, {0}, {0}, {0}}
	},

	{
		{{0}, {8,28},  WHITE, PURPLE, FALSE, TRUE, 0x08, 0x00, 0x00, 0x05, 0x00, 0x02, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {8,32},  WHITE, PURPLE, FALSE, TRUE, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {8,34},  WHITE, PURPLE, FALSE, TRUE, 0x08, 0x02, 0x02, 0x07, 0x00, 0x04, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {8,36},  WHITE, PURPLE, FALSE, TRUE, 0x08, 0x03, 0x03, 0x03, 0x03, 0x03, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {8,38},  WHITE, PURPLE, FALSE, TRUE, 0x08, 0x04, 0x04, 0x09, 0x02, 0x05, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {10,28}, WHITE, PURPLE, FALSE, TRUE, 0x08, 0x05, 0x00, 0x0a, 0x04, 0x07, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {10,32}, WHITE, PURPLE, FALSE, TRUE, 0x08, 0x06, 0x06, 0x06, 0x06, 0x06, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {10,34}, WHITE, PURPLE, FALSE, TRUE, 0x08, 0x07, 0x02, 0x0a, 0x05, 0x09, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {10,36}, WHITE, PURPLE, FALSE, TRUE, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {10,38}, WHITE, PURPLE, FALSE, TRUE, 0x08, 0x09, 0x04, 0x0a, 0x07, 0x0a, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {12,28}, WHITE, PURPLE, FALSE, TRUE, 0x08, 0x0a, 0x05, 0x0b, 0x0a, 0x0a, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {14,28}, WHITE, PURPLE, FALSE, TRUE, 0x08, 0x0b, 0x0a, 0x0c, 0x0b, 0x0c, 0x08, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {17,44}, WHITE, MODENA, FALSE, TRUE, 0x08, 0x0c, 0x0b, 0x0c, 0x0b, 0x0c, 0x08, 0x02, {0}, {"���óɹ�! \n", {17,24}, RED, BLUE}, {"����ʧ��! \n", {17,24}, RED, BLUE}, {0}}
	},

	{
		{{0}, {8,22},  WHITE, MODENA, FALSE, TRUE, 0x09, 0x00, 0x03, 0x01, 0x00, 0x00, 0x0d, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {10,22}, WHITE, MODENA, FALSE, TRUE, 0x09, 0x01, 0x00, 0x02, 0x01, 0x01, 0x0e, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {12,22}, WHITE, MODENA, FALSE, TRUE, 0x09, 0x02, 0x01, 0x03, 0x02, 0x02, 0x0f, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {14,22}, WHITE, MODENA, FALSE, TRUE, 0x09, 0x03, 0x02, 0x00, 0x03, 0x03, 0x10, 0x02, {0}, {0}, {0}, {0}}
	},
	
	{
		{{0}, {8,30},  WHITE, PURPLE, FALSE, TRUE, 0x0a, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0a, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {10,30}, WHITE, PURPLE, FALSE, TRUE, 0x0a, 0x01, 0x00, 0x02, 0x01, 0x01, 0x0a, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {12,30}, WHITE, PURPLE, FALSE, TRUE, 0x0a, 0x02, 0x01, 0x03, 0x02, 0x02, 0x0a, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {15,40}, WHITE, MODENA, FALSE, TRUE, 0x0a, 0x03, 0x02, 0x03, 0x03, 0x03, 0x0a, 0x02, {0}, {"�������óɹ�! \n", {18,24}, RED, BLUE}, {"��������ʧ��! \n", {18,24}, RED, BLUE}, {0}}
	},
	
	{
		{{0}, {5,2},   WHITE, BLUE,  FALSE, FALSE, 0x0b, 0x00, 0x00, 0x01, 0x00, 0x01, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {7,2},   WHITE, BLUE,  FALSE, FALSE, 0x0b, 0x01, 0x00, 0x02, 0x01, 0x01, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {9,2},   WHITE, BLUE,  FALSE, FALSE, 0x0b, 0x02, 0x01, 0x03, 0x02, 0x02, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {11,2},  WHITE, BLUE,  FALSE, FALSE, 0x0b, 0x03, 0x02, 0x04, 0x03, 0x03, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {13,2},  WHITE, BLUE,  FALSE, FALSE, 0x0b, 0x04, 0x03, 0x05, 0x04, 0x04, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {15,2},  WHITE, BLUE,  FALSE, FALSE, 0x0b, 0x05, 0x04, 0x06, 0x05, 0x05, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {17,2},  WHITE, BLUE,  FALSE, FALSE, 0x0b, 0x06, 0x05, 0x07, 0x06, 0x06, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {20,10}, WHITE, MODENA,FALSE, TRUE,  0x0b, 0x07, 0x06, 0x07, 0x07, 0x08, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {20,16}, WHITE, MODENA,FALSE, TRUE,  0x0b, 0x08, 0x06, 0x08, 0x07, 0x09, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {20,22}, WHITE, MODENA,FALSE, TRUE,  0x0b, 0x09, 0x06, 0x09, 0x08, 0x0a, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {20,28}, WHITE, MODENA,FALSE, TRUE,  0x0b, 0x0a, 0x06, 0x0a, 0x09, 0x0b, 0x0b, 0x03, {0}, {0}, {0}, {0}},
		{{0}, {20,34}, WHITE, MODENA,FALSE, TRUE,  0x0b, 0x0b, 0x06, 0x0b, 0x0a, 0x0b, 0x0b, 0x03, {"���ڵ����ļ�! \n", {22,24}, RED, BLUE}, {"�����ļ��ɹ�! \n", {22,24}, RED, BLUE}, {"�����ļ�ʧ��! \n", {22,24}, RED, BLUE}, {0}},
		{{0}, {20,46}, WHITE, BLUE,  FALSE, TRUE,  0x0b, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0b, 0x03, {0}, {0}, {0}, {0}}
	},
	
	{
		{{0}, {5,10},  WHITE, BLUE,  FALSE, FALSE, 0x0c, 0x00, 0x00, 0x01, 0x00, 0x01, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {7,10},  WHITE, BLUE,  FALSE, FALSE, 0x0c, 0x01, 0x00, 0x02, 0x01, 0x01, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {9,10},  WHITE, BLUE,  FALSE, FALSE, 0x0c, 0x02, 0x01, 0x03, 0x02, 0x02, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {11,10}, WHITE, BLUE,  FALSE, FALSE, 0x0c, 0x03, 0x02, 0x04, 0x03, 0x03, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {13,10}, WHITE, BLUE,  FALSE, FALSE, 0x0c, 0x04, 0x03, 0x05, 0x04, 0x04, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {15,10}, WHITE, BLUE,  FALSE, FALSE, 0x0c, 0x05, 0x04, 0x06, 0x05, 0x05, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {17,10}, WHITE, BLUE,  FALSE, FALSE, 0x0c, 0x06, 0x05, 0x07, 0x06, 0x06, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {20,10}, WHITE, MODENA, FALSE,TRUE,  0x0c, 0x07, 0x06, 0x07, 0x07, 0x08, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {20,16}, WHITE, MODENA, FALSE,TRUE,  0x0c, 0x08, 0x06, 0x08, 0x07, 0x09, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {20,22}, WHITE, MODENA, FALSE,TRUE,  0x0c, 0x09, 0x06, 0x09, 0x08, 0x0a, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {20,28}, WHITE, MODENA, FALSE,TRUE,  0x0c, 0x0a, 0x06, 0x0a, 0x09, 0x0b, 0x0c, 0x04, {0}, {0}, {0}, {0}},
		{{0}, {20,34}, WHITE, MODENA, FALSE,TRUE,  0x0c, 0x0b, 0x06, 0x0b, 0x0a, 0x0b, 0x0c, 0x04, {0}, {0}, {"¼��ط�ʧ��! \n", {22,24}, RED, BLUE}, {0}},
		{{0}, {20,46}, WHITE, BLUE,  FALSE, TRUE,  0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x04, {0}, {0}, {0}, {0}}
	},
	
	{
		{{0}, {6,28},  WHITE, MODENA, FALSE, TRUE, 0x0d, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0d, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,28},  WHITE, MODENA, FALSE, TRUE, 0x0d, 0x01, 0x00, 0x02, 0x01, 0x02, 0x0d, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {12,44}, WHITE, MODENA, FALSE, TRUE, 0x0d, 0x02, 0x01, 0x02, 0x01, 0x02, 0x0d, 0x09, {0}, {"���óɹ�! \n", {12,28}, RED, BLUE}, {"����ʧ��! \n", {12,28}, RED, BLUE}, {0}}
	},
	
	{
		{{0}, {4,18},  WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,14},  WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x01, 0x00, 0x06, 0x01, 0x02, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,24},  WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x02, 0x00, 0x07, 0x01, 0x03, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,32},  WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x03, 0x00, 0x08, 0x02, 0x04, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,40},  WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x04, 0x00, 0x09, 0x03, 0x05, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,48},  WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x05, 0x00, 0x0a, 0x04, 0x06, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,14}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x06, 0x01, 0x0b, 0x05, 0x07, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,24}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x07, 0x02, 0x0c, 0x06, 0x08, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,32}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x08, 0x03, 0x0d, 0x07, 0x09, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,40}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x09, 0x04, 0x0e, 0x08, 0x0a, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,48}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x0a, 0x05, 0x0f, 0x09, 0x0b, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,14}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x0b, 0x06, 0x10, 0x0a, 0x0c, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,24}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x0c, 0x07, 0x11, 0x0b, 0x0d, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,32}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x0d, 0x08, 0x12, 0x0c, 0x0e, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,40}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x0e, 0x09, 0x13, 0x0d, 0x0f, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,48}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x0f, 0x0a, 0x14, 0x0e, 0x10, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,14}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x10, 0x0b, 0x15, 0x0f, 0x11, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,24}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x11, 0x0c, 0x15, 0x10, 0x12, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,32}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x12, 0x0d, 0x15, 0x11, 0x13, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,40}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x13, 0x0e, 0x15, 0x12, 0x14, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,48}, WHITE, PURPLE, FALSE, TRUE, 0x0e, 0x14, 0x0f, 0x15, 0x13, 0x15, 0x0e, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {18,52}, WHITE, MODENA, FALSE, TRUE, 0x0e, 0x15, 0x14, 0x15, 0x14, 0x15, 0x0e, 0x09, {0}, {"���óɹ�! \n", {18,28}, RED, BLUE}, {"����ʧ��! \n", {18,28}, RED, BLUE}, {0}}
	},
	
	{
		{{0}, {5,16},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x00, 0x00, 0x0b, 0x00, 0x02, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,18},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,20},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x02, 0x02, 0x0d, 0x00, 0x04, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,22},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x03, 0x03, 0x03, 0x03, 0x03, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,24},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x04, 0x04, 0x0f, 0x02, 0x05, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,30},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x05, 0x05, 0x10, 0x04, 0x07, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,32},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x06, 0x06, 0x06, 0x06, 0x06, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,34},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x07, 0x07, 0x12, 0x05, 0x09, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,36},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,38},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x09, 0x09, 0x14, 0x07, 0x0a, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,44},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x0a, 0x0a, 0x15, 0x09, 0x0b, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,16},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x0b, 0x00, 0x16, 0x0a, 0x0d, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,18},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,20},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x0d, 0x02, 0x18, 0x0b, 0x0f, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,22},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,24},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x0f, 0x04, 0x1a, 0x0d, 0x10, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,30},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x10, 0x05, 0x1b, 0x0f, 0x12, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,32},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,34},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x12, 0x07, 0x1d, 0x10, 0x14, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,36},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x13, 0x13, 0x13, 0x13, 0x13, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,38},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x14, 0x09, 0x1f, 0x12, 0x15, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,44},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x15, 0x0a, 0x20, 0x14, 0x16, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,16},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x16, 0x0b, 0x21, 0x15, 0x18, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,18},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x17, 0x17, 0x17, 0x17, 0x17, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,20},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x18, 0x0d, 0x23, 0x16, 0x1a, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,22},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x19, 0x19, 0x19, 0x19, 0x19, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,24},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x1a, 0x0f, 0x25, 0x18, 0x1b, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,30},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x1b, 0x10, 0x26, 0x1a, 0x1d, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,32},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,34},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x1d, 0x12, 0x28, 0x1b, 0x1f, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,36},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,38},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x1f, 0x14, 0x2a, 0x1d, 0x20, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,44},  WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x20, 0x15, 0x2b, 0x1f, 0x21, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,16}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x21, 0x16, 0x2c, 0x20, 0x23, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,18}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x22, 0x22, 0x22, 0x22, 0x22, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,20}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x23, 0x18, 0x2e, 0x21, 0x25, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,22}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x24, 0x24, 0x24, 0x24, 0x24, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,24}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x25, 0x1a, 0x30, 0x23, 0x26, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,30}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x26, 0x1b, 0x31, 0x25, 0x28, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,32}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x27, 0x27, 0x27, 0x27, 0x27, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,34}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x28, 0x1d, 0x33, 0x26, 0x2a, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,36}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x29, 0x29, 0x29, 0x29, 0x29, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,38}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x2a, 0x1f, 0x35, 0x28, 0x2b, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,44}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x2b, 0x20, 0x36, 0x2a, 0x2c, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,16}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x2c, 0x21, 0x37, 0x2b, 0x2e, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,18}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,20}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x2e, 0x23, 0x39, 0x2c, 0x30, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,22}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,24}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x30, 0x25, 0x3b, 0x2e, 0x31, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,30}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x31, 0x26, 0x3c, 0x30, 0x33, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,32}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x32, 0x32, 0x32, 0x32, 0x32, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,34}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x33, 0x28, 0x3e, 0x31, 0x35, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,36}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x34, 0x34, 0x34, 0x34, 0x34, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,38}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x35, 0x2a, 0x40, 0x33, 0x36, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,44}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x36, 0x2b, 0x41, 0x35, 0x37, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,16}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x37, 0x2c, 0x37, 0x36, 0x39, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,18}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x38, 0x38, 0x38, 0x38, 0x38, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,20}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x39, 0x2e, 0x39, 0x37, 0x3b, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,22}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x3a, 0x3a, 0x3a, 0x3a, 0x3a, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,24}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x3b, 0x30, 0x3b, 0x39, 0x3c, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,30}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x3c, 0x31, 0x3c, 0x3b, 0x3e, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,32}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,34}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x3e, 0x33, 0x3e, 0x3c, 0x40, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,36}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,38}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x40, 0x35, 0x40, 0x3e, 0x41, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,44}, WHITE, PURPLE, FALSE, TRUE, 0x0f, 0x41, 0x36, 0x42, 0x40, 0x42, 0x0f, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {18,48}, WHITE, MODENA, FALSE, TRUE, 0x0f, 0x42, 0x41, 0x42, 0x42, 0x42, 0x0f, 0x09, {0}, {"���óɹ�! \n", {18,24}, RED, BLUE}, {"����ʧ��! \n", {18,24}, RED, BLUE}, {0}}
	},
	
	{
		{{0}, {5,28},  WHITE, PURPLE, FALSE, TRUE, 0x10, 0x00, 0x00, 0x02, 0x00, 0x01, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {5,38},  WHITE, PURPLE, FALSE, TRUE, 0x10, 0x01, 0x01, 0x03, 0x00, 0x02, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,28},  WHITE, PURPLE, FALSE, TRUE, 0x10, 0x02, 0x00, 0x04, 0x01, 0x03, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {7,38},  WHITE, PURPLE, FALSE, TRUE, 0x10, 0x03, 0x01, 0x05, 0x02, 0x04, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,28},  WHITE, PURPLE, FALSE, TRUE, 0x10, 0x04, 0x02, 0x06, 0x03, 0x05, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {9,38},  WHITE, PURPLE, FALSE, TRUE, 0x10, 0x05, 0x03, 0x07, 0x04, 0x06, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,28}, WHITE, PURPLE, FALSE, TRUE, 0x10, 0x06, 0x04, 0x08, 0x05, 0x07, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {11,38}, WHITE, PURPLE, FALSE, TRUE, 0x10, 0x07, 0x05, 0x09, 0x06, 0x08, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,28}, WHITE, PURPLE, FALSE, TRUE, 0x10, 0x08, 0x06, 0x0a, 0x07, 0x09, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {13,38}, WHITE, PURPLE, FALSE, TRUE, 0x10, 0x09, 0x07, 0x0b, 0x08, 0x0a, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,28}, WHITE, PURPLE, FALSE, TRUE, 0x10, 0x0a, 0x08, 0x0c, 0x09, 0x0b, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {15,38}, WHITE, PURPLE, FALSE, TRUE, 0x10, 0x0b, 0x09, 0x0d, 0x0a, 0x0c, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {17,28}, WHITE, PURPLE, FALSE, TRUE, 0x10, 0x0c, 0x0a, 0x0e, 0x0b, 0x0d, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {17,38}, WHITE, PURPLE, FALSE, TRUE, 0x10, 0x0d, 0x0b, 0x0e, 0x0c, 0x0e, 0x10, 0x09, {0}, {0}, {0}, {0}},
		{{0}, {20,46}, WHITE, MODENA, FALSE, TRUE, 0x10, 0x0e, 0x0d, 0x0e, 0x0d, 0x0e, 0x10, 0x09, {0}, {"���óɹ�! \n", {20,26}, RED, BLUE}, {"����ʧ��! \n", {20,26}, RED, BLUE}, {0}}
	},
	
	{
		{{0}, {1,26},  WHITE, PURPLE, FALSE, TRUE, 0x11, 0x00, 0x00, 0x01, 0x00, 0x00, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {3,26},  WHITE, PURPLE, FALSE, TRUE, 0x11, 0x01, 0x00, 0x02, 0x01, 0x01, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {5,26},  WHITE, PURPLE, FALSE, TRUE, 0x11, 0x02, 0x01, 0x03, 0x02, 0x02, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {7,26},  WHITE, PURPLE, FALSE, TRUE, 0x11, 0x03, 0x02, 0x04, 0x03, 0x03, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {9,26},  WHITE, PURPLE, FALSE, TRUE, 0x11, 0x04, 0x03, 0x05, 0x04, 0x04, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {11,26}, WHITE, PURPLE, FALSE, TRUE, 0x11, 0x05, 0x04, 0x06, 0x05, 0x05, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {13,26}, WHITE, PURPLE, FALSE, TRUE, 0x11, 0x06, 0x05, 0x07, 0x06, 0x06, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {15,26}, WHITE, PURPLE, FALSE, TRUE, 0x11, 0x07, 0x06, 0x08, 0x07, 0x07, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {17,26}, WHITE, PURPLE, FALSE, TRUE, 0x11, 0x08, 0x07, 0x09, 0x08, 0x08, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {19,26}, WHITE, PURPLE, FALSE, TRUE, 0x11, 0x09, 0x08, 0x0a, 0x09, 0x09, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {21,26}, WHITE, PURPLE, FALSE, TRUE, 0x11, 0x0a, 0x09, 0x0b, 0x0a, 0x0a, 0x11, 0x02, {0}, {0}, {0}, {0}},
		{{0}, {22,49}, WHITE, MODENA, FALSE, TRUE, 0x11, 0x0b, 0x0a, 0x0b, 0x0b, 0x0b, 0x11, 0x02, {0}, {"���óɹ�! \n", {22,26}, RED, BLUE}, {"����ʧ��! \n", {22,26}, RED, BLUE}, {0}}
	}
};

//-----------------------------------------------------------------------------------------------------------------
void ShowDialog(MENU_ITEM *pCurMenuItem, uint TopLevel, BYTE DialogType)
{
	OSD_BITMAP Bitmap;
	MENU_DIALOG lMenuDialog;
	
	switch(DialogType)
	{
		case DLG_INFO:
			lMenuDialog = pCurMenuItem->InfoDialog;
			break;
		case DLG_SUCC:
			lMenuDialog = pCurMenuItem->SuccDialog;
			break;
		case DLG_ERR:
			lMenuDialog = pCurMenuItem->ErrDialog;
			break;
		default:
			break;
	}
	
	CreateRegion(&(pCurMenuItem->Region), TopLevel, lMenuDialog.Location[0], lMenuDialog.Location[1], 1, CountCharRow(lMenuDialog.Content), 1);
	CreateBitmap(&Bitmap, 1, CountCharRow(lMenuDialog.Content));							
	DrawCharacter(&Bitmap, lMenuDialog.Content, lMenuDialog.CharColor, lMenuDialog.BackColor, 0, 0, FALSE);	
	ShowBitmap(&(pCurMenuItem->Region), &Bitmap, 0, 0);
}

MENU_ITEM *GetItem()
{
	return &(g_objMenuPage[g_bytCPageID].MenuItem[g_bytCItemID]);	
}

void FlushItem( char *v_szDestContent, int v_iDestContentLen, char *v_szSrcContent, int v_iSrcContentLen, char *v_szContentFormat, DWORD v_ulSrcContent )
{
	int i;
	
	if( !v_iDestContentLen )
	{
		for( i = strlen(v_szDestContent)-2; i >= 0; i-- )
		{
			if( v_szDestContent[i] != ' ' )
			{
				v_szDestContent[i] = ' ';
				break;
			}
		}
	}
	else
	{
		memset(v_szDestContent, 0, v_iDestContentLen);
		
		for(i = 0; i < v_iSrcContentLen; i++)
		{
			strcat(v_szDestContent, v_szSrcContent);
		}
		
		if(v_szContentFormat)
		{
			sprintf(v_szDestContent+v_iSrcContentLen, v_szContentFormat, v_ulSrcContent);
		}
			
		while( strlen(v_szDestContent) < v_iDestContentLen )
		{
			strcat(v_szDestContent, " ");
		}
		
		strcat(v_szDestContent, "\n");
	}
}

/*��ʼ���˵�ѡ��*/
void Init_PX_IX(MENU_ITEM *v_pMenuItem)
{
	switch(v_pMenuItem->Pid)
	{
		case 0x00:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					memset(g_objMenuOsd.LoginPassword, 0, sizeof(g_objMenuOsd.LoginPassword));
					FlushItem(v_pMenuItem->Content, 10, NULL, 0, NULL, 0);
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"��½");
					break;
				default:
					break;
			}
		}
			break;
		case 0x01:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"¼  ��  ��  ��");
					break;
				case 0x03:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
					break;
				case 0x04:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
					break;
				case 0x05:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ϵ  ͳ  ��  Ϣ");
					break;
				default:
					break;
			}
		}
			break;
		case 0x02:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"¼  ��  ��  ��");
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
					break;
				case 0x03:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x03:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					g_objMenuOsd.EncChn = 'A';
					FlushItem(v_pMenuItem->Content, 10, " ", 1, "%s", (DWORD)"����ͨ��");
					break;
				case 0x01:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.CreatDate[0], "%04d", iCurDateTime[0]);
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%04d", (DWORD)iCurDateTime[0]);
				}
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"-");
					break;
				case 0x03:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.CreatDate[1], "%02d", iCurDateTime[1]);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%02d", (DWORD)iCurDateTime[1]);
				}
					break;
				case 0x04:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"-");
					break;
				case 0x05:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.CreatDate[2], "%02d", iCurDateTime[2]);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%02d", (DWORD)iCurDateTime[2]);
				}
					break;
				case 0x06:
					strcpy(g_objMenuOsd.StartTime[0], "00"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"00");
					break;
				case 0x07:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x08:
					strcpy(g_objMenuOsd.StartTime[1], "00"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"00");
					break;
				case 0x09:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x0a:
					strcpy(g_objMenuOsd.StartTime[2], "00"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"00"); 
					break;
				case 0x0b:
					strcpy(g_objMenuOsd.EndTime[0], "23"); 
					FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"23"); 
					break;
				case 0x0c:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x0d:
					strcpy(g_objMenuOsd.EndTime[1], "59"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"59");
					break;
				case 0x0e:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x0f:
					strcpy(g_objMenuOsd.EndTime[2], "59"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"59"); 
					break;
				case 0x10:
					strcpy(g_objMenuOsd.DiskType, "SDisk"); 
					FlushItem(v_pMenuItem->Content, 10, " ", 2, "%s", (DWORD)"SD  ��"); 
					break;
				case 0x11:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"��ѯ"); 
					break;
				default:
					break;
			}
		}
			break;
		case 0x04:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					g_objMenuOsd.EncChn = 'A';
					FlushItem(v_pMenuItem->Content, 10, " ", 1, "%s", (DWORD)"����ͨ��"); 
					break;
				case 0x01:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.CreatDate[0], "%04d", iCurDateTime[0]);
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%04d", (DWORD)iCurDateTime[0]); 
				}
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"-");
					break;
				case 0x03:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.CreatDate[1], "%02d", iCurDateTime[1]);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%02d", (DWORD)iCurDateTime[1]);
				}
					break;
				case 0x04:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"-");
					break;
				case 0x05:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.CreatDate[2], "%02d", iCurDateTime[2]);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%02d", (DWORD)iCurDateTime[2]);
				}
					break;
				case 0x06:
					strcpy(g_objMenuOsd.StartTime[0], "00"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"00"); 
					break;
				case 0x07:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x08:
					strcpy(g_objMenuOsd.StartTime[1], "00"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"00"); 
					break;
				case 0x09:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x0a:
					strcpy(g_objMenuOsd.StartTime[2], "00"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"00"); 
					break;
				case 0x0b:
					strcpy(g_objMenuOsd.EndTime[0], "23"); 
					FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"23");
					break;
				case 0x0c:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x0d:
					strcpy(g_objMenuOsd.EndTime[1], "59"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"59");
					break;
				case 0x0e:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x0f:
					strcpy(g_objMenuOsd.EndTime[2], "59"); 
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"59");
					break;
				case 0x10:
					strcpy(g_objMenuOsd.DiskType, "SDisk");
					FlushItem(v_pMenuItem->Content, 10, " ", 2, "%s", (DWORD)"SD  ��");
					break;
				case 0x11:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					break;
				default:
					break;
			}
		}
			break;
		case 0x05:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					strcpy(g_objMenuOsd.DiskType, "SDisk");
					FlushItem(v_pMenuItem->Content, 6, NULL, 0, "%s", (DWORD)"SD  ��");
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 14, " ", 2, "%s", (DWORD)"���̸�ʽ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x06:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					FlushItem(v_pMenuItem->Content, 12, NULL, 0, "%s", (DWORD)"�ָ���������");
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 8, NULL, 0, "%s", (DWORD)"Ӧ������");
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 8, NULL, 0, "%s", (DWORD)"ϵͳ����");
					break;
				default:
					break;
			}
		}
			break;
		case 0x07:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
				{
					char szSoftVer[64] = {0};
					ChkSoftVer(szSoftVer);

					char szSysVer[64] = {0};
					ReadVerFile(szSysVer);

					char szVer[30] = {0};
					sprintf(szVer, "%s-%s", szSoftVer, szSysVer);
					FlushItem(v_pMenuItem->Content, 28, NULL, 0, "%s", (DWORD)szVer);
				}
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 8, " ", 2, "%s", (DWORD)"SD��");
					break;
				case 0x02:
				{
					char szFileSystem[10] = {0};
					GetDiskSystem(SDISK, PART1, szFileSystem);
					FlushItem(v_pMenuItem->Content, 6, NULL, 0, "%s", (DWORD)szFileSystem);
				}
					break;
				case 0x03:
				{
					char szDiskSize[10] = {0};
					GetDiskSize(SDISK, PART1, szDiskSize, NULL, NULL);
					FlushItem(v_pMenuItem->Content, 6, NULL, 0, "%s", (DWORD)szDiskSize);
				}
					break;
				case 0x04:
				{
					char szDiskSize[10] = {0};
					GetDiskSize(SDISK, PART1, NULL, NULL, szDiskSize);
					FlushItem(v_pMenuItem->Content, 8, NULL, 0, "%s", (DWORD)szDiskSize);
				}
					break;
				case 0x05:
					FlushItem(v_pMenuItem->Content, 8, " ", 2, "%s", (DWORD)"Ӳ��");
					break;
				case 0x06:
				{
					char szFileSystem[10] = {0};
					GetDiskSystem(HDISK, PART1, szFileSystem);
					FlushItem(v_pMenuItem->Content, 6, NULL, 0, "%s", (DWORD)szFileSystem);
				}
					break;
				case 0x07:
				{
					char szDiskSize[10] = {0};
					GetDiskSize(HDISK, PART1, szDiskSize, NULL, NULL);
					FlushItem(v_pMenuItem->Content, 6, NULL, 0, "%s", (DWORD)szDiskSize);
				}
					break;
				case 0x08:
				{
					char szDiskSize[10] = {0};
					GetDiskSize(HDISK, PART1, NULL, NULL, szDiskSize);
					FlushItem(v_pMenuItem->Content, 8, NULL, 0, "%s", (DWORD)szDiskSize);
				}
					break;
				case 0x09:
					FlushItem(v_pMenuItem->Content, 8, " ", 2, "%s", (DWORD)"U ��");
					break;
				case 0x0a:
				{
					char szFileSystem[10] = {0};
					GetDiskSystem(UDISK, PART1, szFileSystem);
					FlushItem(v_pMenuItem->Content, 6, NULL, 0, "%s", (DWORD)szFileSystem);
				}
					break;
				case 0x0b:
				{
					char szDiskSize[10] = {0};
					GetDiskSize(UDISK, PART1, szDiskSize, NULL, NULL);
					FlushItem(v_pMenuItem->Content, 6, NULL, 0, "%s", (DWORD)szDiskSize);
				}
					break;
				case 0x0c:
				{
					char szDiskSize[10] = {0};
					GetDiskSize(UDISK, PART1, NULL, NULL, szDiskSize);
					FlushItem(v_pMenuItem->Content, 8, NULL, 0, "%s", (DWORD)szDiskSize);
				}
					break;
				default:
					break;
			}
		}
			break;
		case 0x08:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.SystemDate[0], "%04d", iCurDateTime[0]);
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%04d", (DWORD)iCurDateTime[0]);
				}
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"-");
					break;
				case 0x02:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.SystemDate[1], "%02d", iCurDateTime[1]);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%02d", (DWORD)iCurDateTime[1]);
				}
					break;
				case 0x03:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)"-");
					break;
				case 0x04:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.SystemDate[2], "%02d", iCurDateTime[2]);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%02d", (DWORD)iCurDateTime[2]);
				}
					break;
				case 0x05:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.SystemTime[0], "%02d", iCurDateTime[3]);
					FlushItem(v_pMenuItem->Content, 4, " ", 2, "%02d", (DWORD)iCurDateTime[3]);
				}
					break;
				case 0x06:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x07:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.SystemTime[1], "%02d", iCurDateTime[4]);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%02d", (DWORD)iCurDateTime[4]);
				}
					break;
				case 0x08:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":");
					break;
				case 0x09:
				{
					int iCurDateTime[6];
					GetCurDateTime( iCurDateTime );
					sprintf(g_objMenuOsd.SystemTime[2], "%02d", iCurDateTime[5]);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%02d", (DWORD)iCurDateTime[5]);
				}
					break;
				case 0x0a:
					strcpy(g_objMenuOsd.DriverID, g_objMvrCfg.DeviceInfo.DriverID);
					FlushItem(v_pMenuItem->Content, 12, NULL, 0, "%s", (DWORD)g_objMvrCfg.DeviceInfo.DriverID);
					break;
				case 0x0b:
					strcpy(g_objMenuOsd.CarNumber, g_objMvrCfg.DeviceInfo.CarNumber);
					FlushItem(v_pMenuItem->Content, 12, NULL, 0, "%s", (DWORD)g_objMvrCfg.DeviceInfo.CarNumber);
					break;
				case 0x0c:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ȷ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x09:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"¼  ��  ģ  ʽ");
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ͨ  ��  ��  ��");
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ʱ  ��  ��  ��");
					break;
				case 0x03:
					FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x0a:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					memset(g_objMenuOsd.CurPassword, 0, sizeof(g_objMenuOsd.CurPassword));
					FlushItem(v_pMenuItem->Content, 10, NULL, 0, NULL, 0);
					break;
				case 0x01:
					memset(g_objMenuOsd.NewPassword[0], 0, sizeof(g_objMenuOsd.NewPassword[0]));
					FlushItem(v_pMenuItem->Content, 10, NULL, 0, NULL, 0);
					break;
				case 0x02:
					memset(g_objMenuOsd.NewPassword[1], 0, sizeof(g_objMenuOsd.NewPassword[1]));
					FlushItem(v_pMenuItem->Content, 10, NULL, 0, NULL, 0);
					break;
				case 0x03:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ȷ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x0b:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					FlushItem(v_pMenuItem->Content, 56, NULL, 0, NULL, 0);
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 56, NULL, 0, NULL, 0);
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 56, NULL, 0, NULL, 0);
					break;
				case 0x03:
					FlushItem(v_pMenuItem->Content, 56, NULL, 0, NULL, 0);
					break;
				case 0x04:
					FlushItem(v_pMenuItem->Content, 56, NULL, 0, NULL, 0);
					break;
				case 0x05:
					FlushItem(v_pMenuItem->Content, 56, NULL, 0, NULL, 0);
					break;
				case 0x06:
					FlushItem(v_pMenuItem->Content, 56, NULL, 0, NULL, 0);
					break;
				case 0x07:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"��ҳ");
					break;
				case 0x08:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"��ҳ");
					break;
				case 0x09:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"��ҳ");
					break;
				case 0x0a:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ĩҳ");
					break;
				case 0x0b:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					break;
				case 0x0c:
					FlushItem(v_pMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"   0 /   0");
					break;
				default:
					break;
			}
		}
			break;
		case 0x0c:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					FlushItem(v_pMenuItem->Content, 40, NULL, 0, NULL, 0);
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 40, NULL, 0, NULL, 0);
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 40, NULL, 0, NULL, 0); 
					break;
				case 0x03:
					FlushItem(v_pMenuItem->Content, 40, NULL, 0, NULL, 0);
					break;
				case 0x04:
					FlushItem(v_pMenuItem->Content, 40, NULL, 0, NULL, 0);
					break;
				case 0x05:
					FlushItem(v_pMenuItem->Content, 40, NULL, 0, NULL, 0);
					break;
				case 0x06:
					FlushItem(v_pMenuItem->Content, 40, NULL, 0, NULL, 0);
					break;
				case 0x07:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"��ҳ");
					break;
				case 0x08:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"��ҳ");
					break;
				case 0x09:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"��ҳ");
					break;
				case 0x0a:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ĩҳ");
					break;
				case 0x0b:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ط�");
					break;
				case 0x0c:
					FlushItem(v_pMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"   0 /   0");
					break;
				default:
					break;
			}
		}
			break;
		case 0x0d:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					switch(g_objMvrCfg.TrigPara.TrigType)
					{
						case TRIG_PER:
							g_objMenuOsd.TrigType = TRIG_PER;
							FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ʱ  ��  ��  ��");
							break;
						case TRIG_EVE:
							g_objMenuOsd.TrigType = TRIG_EVE;
							FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
							break;
						case TRIG_PER_AND_EVE:
							g_objMenuOsd.TrigType = TRIG_PER_AND_EVE;
							FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ʱ�����¼�����");
							break;
						case TRIG_PER_OR_EVE:
							g_objMenuOsd.TrigType = TRIG_PER_OR_EVE;
							FlushItem(v_pMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ʱ�λ��¼�����");
							break;
						default:
							break;
					}
					break;
				case 0x01:
					if(strcmp("SDisk", g_objMvrCfg.AVRecord.DiskType) == 0)
					{
						strcpy(g_objMenuOsd.DiskType, "SDisk");
						FlushItem(v_pMenuItem->Content, 14, " ", 4, "%s", (DWORD)"SD  ��");
					}
					else if(strcmp("HDisk", g_objMvrCfg.AVRecord.DiskType) == 0)
					{
						strcpy(g_objMenuOsd.DiskType, "HDisk");
						FlushItem(v_pMenuItem->Content, 14, " ", 4, "%s", (DWORD)"Ӳ  ��");
					}
					break;
				case 0x02:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ȷ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x0e:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					if(g_objMvrCfg.VInput.VNormal == PAL)
					{
						g_objMenuOsd.ViNormal = PAL;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"PAL");
					}
					else
					{
						g_objMenuOsd.ViNormal = NTSC;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"NTSC");
					}
					break;
				case 0x01:
#if VEHICLE_TYPE == VEHICLE_M
					switch(g_objMvrCfg.VInput.VMode)
					{
						case CIF:
							g_objMenuOsd.VRecMode[0] = CIF;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
							break;
						case HD1:
							g_objMenuOsd.VRecMode[0] = HD1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
							break;
						case D1:
							g_objMenuOsd.VRecMode[0] = D1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
							break;
						default:
							break;
					}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
					switch(g_objMvrCfg.AVRecord.VMode[0])
					{
						case CIF:
							g_objMenuOsd.VRecMode[0] = CIF;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
							break;
						case HD1:
							g_objMenuOsd.VRecMode[0] = HD1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
							break;
						case D1:
							g_objMenuOsd.VRecMode[0] = D1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
							break;
						default:
							break;
					}
#endif
					break;
				case 0x02:
					switch(g_objMvrCfg.AVRecord.VBitrate[0])
					{
						case BITRATE_REC_1:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_1;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"1");
							break;
						case BITRATE_REC_2:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_2;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"2");
							break;
						case BITRATE_REC_3:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_3;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"3");
							break;
						case BITRATE_REC_4:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_4;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"4");
							break;
						case BITRATE_REC_5:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_5;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"5");
							break;
						case BITRATE_REC_6:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_6;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"6");
							break;
						case BITRATE_REC_7:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_7;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"7");
							break;
						case BITRATE_REC_8:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_8;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"8");
							break;
						case BITRATE_REC_9:
							g_objMenuOsd.VRecBps[0] = BITRATE_REC_9;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"9");
							break;
						default:
							break;
					}
					break;
				case 0x03:
					sprintf(g_objMenuOsd.VRecFps[0], "%d", g_objMvrCfg.AVRecord.VFramerate[0]);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.AVRecord.VFramerate[0]);
					break;
				case 0x04:
					if( g_objMvrCfg.AVRecord.VStart[0] )
					{
						g_objMenuOsd.VRecEn[0] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.VRecEn[0] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x05:
					if( g_objMvrCfg.VPreview.VStart[0] )
					{
						g_objMenuOsd.VPrevEn[0] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.VPrevEn[0] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x06:
#if VEHICLE_TYPE == VEHICLE_M
					switch( g_objMvrCfg.VInput.VMode )
					{
						case CIF:
							g_objMenuOsd.VRecMode[1] = CIF;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
							break;
						case HD1:
							g_objMenuOsd.VRecMode[1] = HD1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
							break;
						default:
							break;
					}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
					switch(g_objMvrCfg.AVRecord.VMode[1])
					{
						case CIF:
							g_objMenuOsd.VRecMode[1] = CIF;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
							break;
						case HD1:
							g_objMenuOsd.VRecMode[1] = HD1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
							break;
						case D1:
							g_objMenuOsd.VRecMode[1] = D1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
							break;
						default:
							break;
					}
#endif
					break;
				case 0x07:
					switch( g_objMvrCfg.AVRecord.VBitrate[1] )
					{
						case BITRATE_REC_1:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_1;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"1");
							break;
						case BITRATE_REC_2:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_2;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"2");
							break;
						case BITRATE_REC_3:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_3;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"3");
							break;
						case BITRATE_REC_4:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_4;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"4");
							break;
						case BITRATE_REC_5:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_5;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"5");
							break;
						case BITRATE_REC_6:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_6;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"6");
							break;
						case BITRATE_REC_7:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_7;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"7");
							break;
						case BITRATE_REC_8:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_8;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"8");
							break;
						case BITRATE_REC_9:
							g_objMenuOsd.VRecBps[1] = BITRATE_REC_9;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"9");
							break;
						default:
							break;
					}
					break;
				case 0x08:
					sprintf(g_objMenuOsd.VRecFps[1], "%d", g_objMvrCfg.AVRecord.VFramerate[1]);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.AVRecord.VFramerate[1]);
					break;
				case 0x09:
					if( g_objMvrCfg.AVRecord.VStart[1] )
					{
						g_objMenuOsd.VRecEn[1] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.VRecEn[1] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x0a:
					if( g_objMvrCfg.VPreview.VStart[1] )
					{
						g_objMenuOsd.VPrevEn[1] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.VPrevEn[1] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x0b:
#if VEHICLE_TYPE == VEHICLE_M
					g_objMenuOsd.VRecMode[2] = CIF;
					FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
					switch(g_objMvrCfg.AVRecord.VMode[2])
					{
						case CIF:
							g_objMenuOsd.VRecMode[2] = CIF;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
							break;
						case HD1:
							g_objMenuOsd.VRecMode[2] = HD1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
							break;
						case D1:
							g_objMenuOsd.VRecMode[2] = D1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
							break;
						default:
							break;
					}
#endif
					break;
				case 0x0c:
					switch( g_objMvrCfg.AVRecord.VBitrate[2] )
					{
						case BITRATE_REC_1:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_1;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"1");
							break;
						case BITRATE_REC_2:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_2;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"2");
							break;
						case BITRATE_REC_3:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_3;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"3");
							break;
						case BITRATE_REC_4:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_4;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"4");
							break;
						case BITRATE_REC_5:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_5;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"5");
							break;
						case BITRATE_REC_6:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_6;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"6");
							break;
						case BITRATE_REC_7:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_7;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"7");
							break;
						case BITRATE_REC_8:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_8;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"8");
							break;
						case BITRATE_REC_9:
							g_objMenuOsd.VRecBps[2] = BITRATE_REC_9;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"9");
							break;
						default:
							break;
					}
					break;
				case 0x0d:
					sprintf(g_objMenuOsd.VRecFps[2], "%d", g_objMvrCfg.AVRecord.VFramerate[2]);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.AVRecord.VFramerate[2]);
					break;
				case 0x0e:
					if( g_objMvrCfg.AVRecord.VStart[2] )
					{
						g_objMenuOsd.VRecEn[2] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.VRecEn[2] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x0f:
					if( g_objMvrCfg.VPreview.VStart[2] )
					{
						g_objMenuOsd.VPrevEn[2] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.VPrevEn[2] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x10:
#if VEHICLE_TYPE == VEHICLE_M
					g_objMenuOsd.VRecMode[3] = CIF;
					FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
					switch(g_objMvrCfg.AVRecord.VMode[3])
					{
						case CIF:
							g_objMenuOsd.VRecMode[3] = CIF;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
							break;
						case HD1:
							g_objMenuOsd.VRecMode[3] = HD1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
							break;
						case D1:
							g_objMenuOsd.VRecMode[3] = D1;
							FlushItem(v_pMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
							break;
						default:
							break;
					}
#endif
					break;
				case 0x11:
					switch( g_objMvrCfg.AVRecord.VBitrate[3] )
					{
						case BITRATE_REC_1:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_1;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"1");
							break;
						case BITRATE_REC_2:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_2;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"2");
							break;
						case BITRATE_REC_3:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_3;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"3");
							break;
						case BITRATE_REC_4:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_4;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"4");
							break;
						case BITRATE_REC_5:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_5;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"5");
							break;
						case BITRATE_REC_6:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_6;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"6");
							break;
						case BITRATE_REC_7:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_7;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"7");
							break;
						case BITRATE_REC_8:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_8;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"8");
							break;
						case BITRATE_REC_9:
							g_objMenuOsd.VRecBps[3] = BITRATE_REC_9;
							FlushItem(v_pMenuItem->Content, 4, " ", 2, "%s", (DWORD)"9");
							break;
						default:
							break;
					}
					break;
				case 0x12:
					sprintf(g_objMenuOsd.VRecFps[3], "%d", g_objMvrCfg.AVRecord.VFramerate[3]);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.AVRecord.VFramerate[3]);
					break;
				case 0x13:
					if( g_objMvrCfg.AVRecord.VStart[3] )
					{
						g_objMenuOsd.VRecEn[3] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.VRecEn[3] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x14:
					if( g_objMvrCfg.VPreview.VStart[3] )
					{
						g_objMenuOsd.VPrevEn[3] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.VPrevEn[3] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x15:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ȷ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x0f:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					strncpy(g_objMenuOsd.PeriodSHour[0], g_objMvrCfg.PeriodTrig.StartTime[0], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[0]);
					break;
				case 0x01:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x02:
					strncpy(g_objMenuOsd.PeriodSMin[0], g_objMvrCfg.PeriodTrig.StartTime[0]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[0]);
					break;
				case 0x03:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x04:
					strncpy(g_objMenuOsd.PeriodSSec[0], g_objMvrCfg.PeriodTrig.StartTime[0]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[0]);
					break;
				case 0x05:
					strncpy(g_objMenuOsd.PeriodEHour[0], g_objMvrCfg.PeriodTrig.EndTime[0], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[0]);
					break;
				case 0x06:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x07:
					strncpy(g_objMenuOsd.PeriodEMin[0], g_objMvrCfg.PeriodTrig.EndTime[0]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[0]);
					break;
				case 0x08:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x09:
					strncpy(g_objMenuOsd.PeriodESec[0], g_objMvrCfg.PeriodTrig.EndTime[0]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[0]);
					break;
				case 0x0a:
					if( g_objMvrCfg.PeriodTrig.PeriodStart[0] )
					{
						g_objMenuOsd.PeriodEn[0] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.PeriodEn[0] = FALSE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x0b:
					strncpy(g_objMenuOsd.PeriodSHour[1], g_objMvrCfg.PeriodTrig.StartTime[1], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[1]);
					break;
				case 0x0c:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x0d:
					strncpy(g_objMenuOsd.PeriodSMin[1], g_objMvrCfg.PeriodTrig.StartTime[1]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[1]);
					break;
				case 0x0e:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x0f:
					strncpy(g_objMenuOsd.PeriodSSec[1], g_objMvrCfg.PeriodTrig.StartTime[1]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[1]);
					break;
				case 0x10:
					strncpy(g_objMenuOsd.PeriodEHour[1], g_objMvrCfg.PeriodTrig.EndTime[1], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[1]);
					break;
				case 0x11:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x12:
					strncpy(g_objMenuOsd.PeriodEMin[1], g_objMvrCfg.PeriodTrig.EndTime[1]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[1]);
					break;
				case 0x13:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x14:
					strncpy(g_objMenuOsd.PeriodESec[1], g_objMvrCfg.PeriodTrig.EndTime[1]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[1]);
					break;
				case 0x15:
					if( g_objMvrCfg.PeriodTrig.PeriodStart[1] )
					{
						g_objMenuOsd.PeriodEn[1] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.PeriodEn[1] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x16:
					strncpy(g_objMenuOsd.PeriodSHour[2], g_objMvrCfg.PeriodTrig.StartTime[2], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[2]);
					break;
				case 0x17:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x18:
					strncpy(g_objMenuOsd.PeriodSMin[2], g_objMvrCfg.PeriodTrig.StartTime[2]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[2]);
					break;
				case 0x19:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x1a:
					strncpy(g_objMenuOsd.PeriodSSec[2], g_objMvrCfg.PeriodTrig.StartTime[2]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[2]);
					break;
				case 0x1b:
					strncpy(g_objMenuOsd.PeriodEHour[2], g_objMvrCfg.PeriodTrig.EndTime[2], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[2]);
					break;
				case 0x1c:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x1d:
					strncpy(g_objMenuOsd.PeriodEMin[2], g_objMvrCfg.PeriodTrig.EndTime[2]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[2]);
					break;
				case 0x1e:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x1f:
					strncpy(g_objMenuOsd.PeriodESec[2], g_objMvrCfg.PeriodTrig.EndTime[2]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[2]);
					break;
				case 0x20:
					if( g_objMvrCfg.PeriodTrig.PeriodStart[2] )
					{
						g_objMenuOsd.PeriodEn[2] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.PeriodEn[2] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x21:
					strncpy(g_objMenuOsd.PeriodSHour[3], g_objMvrCfg.PeriodTrig.StartTime[3], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[3]);
					break;
				case 0x22:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x23:
					strncpy(g_objMenuOsd.PeriodSMin[3], g_objMvrCfg.PeriodTrig.StartTime[3]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[3]);
					break;
				case 0x24:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x25:
					strncpy(g_objMenuOsd.PeriodSSec[3], g_objMvrCfg.PeriodTrig.StartTime[3]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[3]);
					break;
				case 0x26:
					strncpy(g_objMenuOsd.PeriodEHour[3], g_objMvrCfg.PeriodTrig.EndTime[3], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[3]);
					break;
				case 0x27:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x28:
					strncpy(g_objMenuOsd.PeriodEMin[3], g_objMvrCfg.PeriodTrig.EndTime[3]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[3]);
					break;
				case 0x29:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x2a:
					strncpy(g_objMenuOsd.PeriodESec[3], g_objMvrCfg.PeriodTrig.EndTime[3]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[3]);
					break;
				case 0x2b:
					if( g_objMvrCfg.PeriodTrig.PeriodStart[3] )
					{
						g_objMenuOsd.PeriodEn[3] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.PeriodEn[3] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x2c:
					strncpy(g_objMenuOsd.PeriodSHour[4], g_objMvrCfg.PeriodTrig.StartTime[4], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[4]);
					break;
				case 0x2d:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x2e:
					strncpy(g_objMenuOsd.PeriodSMin[4], g_objMvrCfg.PeriodTrig.StartTime[4]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[4]);
					break;
				case 0x2f:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x30:
					strncpy(g_objMenuOsd.PeriodSSec[4], g_objMvrCfg.PeriodTrig.StartTime[4]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[4]);
					break;
				case 0x31:
					strncpy(g_objMenuOsd.PeriodEHour[4], g_objMvrCfg.PeriodTrig.EndTime[4], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[4]);
					break;
				case 0x32:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x33:
					strncpy(g_objMenuOsd.PeriodEMin[4], g_objMvrCfg.PeriodTrig.EndTime[4]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[4]);
					break;
				case 0x34:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x35:
					strncpy(g_objMenuOsd.PeriodESec[4], g_objMvrCfg.PeriodTrig.EndTime[4]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[4]);
					break;
				case 0x36:
					if( g_objMvrCfg.PeriodTrig.PeriodStart[4] )
					{
						g_objMenuOsd.PeriodEn[4] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.PeriodEn[4] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x37:
					strncpy(g_objMenuOsd.PeriodSHour[5], g_objMvrCfg.PeriodTrig.StartTime[5], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[5]);
					break;
				case 0x38:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x39:
					strncpy(g_objMenuOsd.PeriodSMin[5], g_objMvrCfg.PeriodTrig.StartTime[5]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[5]);
					break;
				case 0x3a:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x3b:
					strncpy(g_objMenuOsd.PeriodSSec[5], g_objMvrCfg.PeriodTrig.StartTime[5]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[5]);
					break;
				case 0x3c:
					strncpy(g_objMenuOsd.PeriodEHour[5], g_objMvrCfg.PeriodTrig.EndTime[5], 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[5]);
					break;
				case 0x3d:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x3e:
					strncpy(g_objMenuOsd.PeriodEMin[5], g_objMvrCfg.PeriodTrig.EndTime[5]+3, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[5]);
					break;
				case 0x3f:
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)":"); 
					break;
				case 0x40:
					strncpy(g_objMenuOsd.PeriodESec[5], g_objMvrCfg.PeriodTrig.EndTime[5]+6, 2);
					FlushItem(v_pMenuItem->Content, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[5]);
					break;
				case 0x41:
					if( g_objMvrCfg.PeriodTrig.PeriodStart[5] )
					{
						g_objMenuOsd.PeriodEn[5] = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.PeriodEn[5] = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x42:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ȷ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x10:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					sprintf(g_objMenuOsd.AccDelay, "%d", g_objMvrCfg.EventTrig.AccDelay);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.EventTrig.AccDelay);
					break;
				case 0x01:
					if( g_objMvrCfg.EventTrig.AccStart )
					{
						g_objMenuOsd.AccEn = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.AccEn = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x02:
					sprintf(g_objMenuOsd.MoveDelay, "%d", g_objMvrCfg.EventTrig.MoveDelay);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.EventTrig.MoveDelay);
					break;
				case 0x03:
					if( g_objMvrCfg.EventTrig.MoveStart )
					{
						g_objMenuOsd.MoveEn = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.MoveEn = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x04:
					sprintf(g_objMenuOsd.OverlayDelay, "%d", g_objMvrCfg.EventTrig.OverlayDelay);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.EventTrig.OverlayDelay);
					break;
				case 0x05:
					if( g_objMvrCfg.EventTrig.OverlayStart )
					{
						g_objMenuOsd.OverlayEn = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.OverlayEn = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x06:
					sprintf(g_objMenuOsd.S1Delay, "%d", g_objMvrCfg.EventTrig.S1Delay);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.EventTrig.S1Delay);
					break;
				case 0x07:
					if( g_objMvrCfg.EventTrig.S1Start )
					{
						g_objMenuOsd.S1En = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.S1En = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x08:
					sprintf(g_objMenuOsd.S2Delay, "%d", g_objMvrCfg.EventTrig.S2Delay);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.EventTrig.S2Delay);
					break;
				case 0x09:
					if( g_objMvrCfg.EventTrig.S2Start )
					{
						g_objMenuOsd.S2En = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.S2En = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x0a:
					sprintf(g_objMenuOsd.S3Delay, "%d", g_objMvrCfg.EventTrig.S3Delay);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.EventTrig.S3Delay);
					break;
				case 0x0b:
					if( g_objMvrCfg.EventTrig.S3Start )
					{
						g_objMenuOsd.S3En = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.S3En = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x0c:
					sprintf(g_objMenuOsd.S4Delay, "%d", g_objMvrCfg.EventTrig.S4Delay);
					FlushItem(v_pMenuItem->Content, 4, " ", 1, "%-2d", (DWORD)g_objMvrCfg.EventTrig.S4Delay);
					break;
				case 0x0d:
					if( g_objMvrCfg.EventTrig.S4Start )
					{
						g_objMenuOsd.S4En = TRUE;
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
					}
					else
					{
						g_objMenuOsd.S4En = FALSE;	
						FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
					}
					break;
				case 0x0e:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ȷ��");
					break;
				default:
					break;
			}
		}
			break;
		case 0x11:
		{
			switch(v_pMenuItem->Iid)
			{
				case 0x00:
					strcpy(g_objMenuOsd.m_szPhoneMod, g_objMvrCfg.IpPortPara.m_szPhoneMod); 
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%s", (DWORD)g_objMvrCfg.IpPortPara.m_szPhoneMod);
					break;
				case 0x01:
					strcpy(g_objMenuOsd.m_szTelNum, g_objMvrCfg.IpPortPara.m_szTelNum); 
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%s", (DWORD)g_objMvrCfg.IpPortPara.m_szTelNum);
					break;
				case 0x02:
					strcpy(g_objMenuOsd.m_szApn, g_objMvrCfg.IpPortPara.m_szApn); 
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%s", (DWORD)g_objMvrCfg.IpPortPara.m_szApn);
					break;
				case 0x03:
				{
					struct in_addr in = {g_objMvrCfg.IpPortPara.m_ulCommIP};
					strcpy(g_objMenuOsd.m_ulCommIP, inet_ntoa(in)); 
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%s", (DWORD)inet_ntoa(in));
				}
					break;
				case 0x04:
					sprintf(g_objMenuOsd.m_usCommPort, "%d", ntohs(g_objMvrCfg.IpPortPara.m_usCommPort));
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%-19d", (DWORD)ntohs(g_objMvrCfg.IpPortPara.m_usCommPort));
					break;
				case 0x05:
				{
					struct in_addr in = {g_objMvrCfg.IpPortPara.m_ulPhotoIP};
					strcpy(g_objMenuOsd.m_ulPhotoIP, inet_ntoa(in)); 
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%s", (DWORD)inet_ntoa(in));
				}
					break;
				case 0x06:
					sprintf(g_objMenuOsd.m_usPhotoPort, "%d", ntohs(g_objMvrCfg.IpPortPara.m_usPhotoPort));
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%-19d", (DWORD)ntohs(g_objMvrCfg.IpPortPara.m_usPhotoPort));
					break;
				case 0x07:
				{
					struct in_addr in = {g_objMvrCfg.IpPortPara.m_ulVideoIP};
					strcpy(g_objMenuOsd.m_ulVideoIP, inet_ntoa(in)); 
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%s", (DWORD)inet_ntoa(in));
				}
					break;
				case 0x08:
					sprintf(g_objMenuOsd.m_usVideoPort, "%d", ntohs(g_objMvrCfg.IpPortPara.m_usVideoPort));
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%-19d", (DWORD)ntohs(g_objMvrCfg.IpPortPara.m_usVideoPort));
					break;
				case 0x09:
				{
					struct in_addr in = {g_objMvrCfg.IpPortPara.m_ulUpdateIP};
					strcpy(g_objMenuOsd.m_ulUpdateIP, inet_ntoa(in)); 
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%s", (DWORD)inet_ntoa(in));
				}
					break;
				case 0x0a:
					sprintf(g_objMenuOsd.m_usUpdatePort, "%d", ntohs(g_objMvrCfg.IpPortPara.m_usUpdatePort));
					FlushItem(v_pMenuItem->Content, 19, NULL, 0, "%-19d", (DWORD)ntohs(g_objMvrCfg.IpPortPara.m_usUpdatePort));
					break;
				case 0x0b:
					FlushItem(v_pMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"ȷ��");
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

void InitItem(MENU_ITEM *v_pCurMenuItem)
{
	Init_PX_IX(v_pCurMenuItem);
}

void ShowItem(MENU_PAGE *pCurMenuPage, MENU_ITEM *pCurMenuItem, uint CharColor, uint BackColor)
{
	OSD_BITMAP Bitmap;
	
	if( pCurMenuItem->Exist )
	{
		CreateBitmap(&Bitmap, 1, CountCharRow(pCurMenuItem->Content));	
		DrawCharacter(&Bitmap, pCurMenuItem->Content, CharColor, BackColor, 0, 0, FALSE);		
		ShowBitmap(&(pCurMenuPage->Region), &Bitmap, pCurMenuItem->Location[0], pCurMenuItem->Location[1]);
	}
}

void HideItem(MENU_ITEM *v_pCurMenuItem)
{
	DeleteRegion(v_pCurMenuItem->Region);
}

MENU_PAGE *GetPage()
{
	return &(g_objMenuPage[g_bytCPageID]);
}

/*��ʼ���˵�ҳ��*/
void Init_PX(MENU_PAGE *v_pMenuPage)
{
	int i;
	
	v_pMenuPage->MenuItem = (MENU_ITEM*)malloc((v_pMenuPage->Num)*sizeof(MENU_ITEM));
	
	for(i = 0; i < v_pMenuPage->Num; i++)
	{
		v_pMenuPage->MenuItem[i] = g_objMenuItem[v_pMenuPage->Pid][i];
		Init_PX_IX(&(v_pMenuPage->MenuItem[i]));
	}
}

void InitPage(MENU_PAGE *v_pCurMenuPage)
{
	Init_PX(v_pCurMenuPage);
}

void ShowPage(MENU_PAGE *pCurMenuPage, uint TopLevel, WORD Line, WORD Row)
{
	int i;
	OSD_BITMAP Bitmap;

	CreateRegion(&(pCurMenuPage->Region), TopLevel, pCurMenuPage->Location[0], pCurMenuPage->Location[1], Line, Row, 0);
	CreateBitmap(&Bitmap, Line, Row);
	DrawCharacter(&Bitmap, pCurMenuPage->Content, pCurMenuPage->CharColor, pCurMenuPage->BackColor, 0, 0, TRUE);	

	for(i = 0; i < pCurMenuPage->Num; i++)
	{
		if( pCurMenuPage->MenuItem[i].Exist )
		{									
			DrawCharacter(&Bitmap, pCurMenuPage->MenuItem[i].Content, pCurMenuPage->MenuItem[i].CharColor, pCurMenuPage->MenuItem[i].BackColor, pCurMenuPage->MenuItem[i].Location[0], pCurMenuPage->MenuItem[i].Location[1], FALSE);
		}
	}
	//ShowRegion(pCurMenuPage->Region);
	ShowBitmap(&(pCurMenuPage->Region), &Bitmap, 0, 0);
}

void HidePage(MENU_PAGE *v_pCurMenuPage)
{
	if( v_pCurMenuPage->MenuItem )
	{
		free(v_pCurMenuPage->MenuItem);
		v_pCurMenuPage->MenuItem = NULL;
	}
	
	DeleteRegion(v_pCurMenuPage->Region);
}

BYTE GetNextID( MENU_ITEM *v_pCurMenuItem, int v_iKeyStyle )
{
	BYTE bytNextID;
	
	switch( v_iKeyStyle )
	{
		case UP:
			bytNextID = v_pCurMenuItem->Uid;
			break;
		case DOWN:
			bytNextID = v_pCurMenuItem->Did;
			break;
		case LEFT:
			bytNextID = v_pCurMenuItem->Lid;
			break;
		case RIGHT:
			bytNextID = v_pCurMenuItem->Rid;
			break;
		case ENTER:
			bytNextID = v_pCurMenuItem->Eid;	
			break;
		case CANCEL:
			bytNextID = v_pCurMenuItem->Cid;	
			break;
		default:
			break;
	}
	
	return bytNextID;
}

void DealNumInput( MENU_PAGE *v_pCMenuPage, MENU_ITEM *v_pCMenuItem, char *v_pNumber, char *v_pMenuOsd, int v_iMenuOsdLen, int v_iDItemLen, char *v_szSItem, int v_iSItemLen, char *v_szItemFormat, DWORD v_ulSItem )
{
	if( v_pCMenuItem->Fix )
	{
		if( strlen( v_pMenuOsd ) < v_iMenuOsdLen )
		{
			strcat( v_pMenuOsd, v_pNumber);
			FlushItem(v_pCMenuItem->Content, v_iDItemLen, v_szSItem, v_iSItemLen, v_szItemFormat, (DWORD)v_ulSItem);
			ShowItem(v_pCMenuPage, v_pCMenuItem, BLACK, ORCHID);
		}
	}
}

//���㿪����ͨ�����ֱ��ʶ�Ӧ֡��
EN_MODE_FRAME CountEnModeFrame()
{
	EN_MODE_FRAME objEnModeFrame = {0};
	
	for(int i=0; i<4; i++)
	{
		if( g_objMenuOsd.VRecEn[i] )
		{
			switch( g_objMenuOsd.VRecMode[i] )
			{
				case CIF:
					objEnModeFrame.CIFFrame += atoi(g_objMenuOsd.VRecFps[i]);
					break;
				case HD1:
					objEnModeFrame.HD1Frame += atoi(g_objMenuOsd.VRecFps[i]);
					break;
				case D1:
					objEnModeFrame.D1Frame += atoi(g_objMenuOsd.VRecFps[i]);
					break;
			}
		}
	}
	
	return objEnModeFrame;
}

bool CanSwitchNormal()
{
	EN_MODE_FRAME objEnModeFrame = CountEnModeFrame();
	
	switch( g_objMenuOsd.ViNormal )
	{
		case PAL://PAL->NTSC
		{
			if( (4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame) <= 240 )
				return true;
		}
			break;
		case NTSC://NTSC->PAL
		{
			if( (4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame) <= 200 )
				return true;
		}
			break;
	}

	return false;
}

bool CanSwitchMode(int v_iChn)
{
	EN_MODE_FRAME objEnModeFrame = CountEnModeFrame();
	
	switch( g_objMenuOsd.VRecMode[v_iChn] )
	{
		case CIF://CIF->HD1
		{
			if( !g_objMenuOsd.VRecEn[v_iChn] || (4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame+atoi(g_objMenuOsd.VRecFps[v_iChn])) <= (g_objMenuOsd.ViNormal==PAL?200:240) )
				return true;
		}
			break;
		case HD1://HD1->D1
		{
			if( !g_objMenuOsd.VRecEn[v_iChn] || (4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame+2*atoi(g_objMenuOsd.VRecFps[v_iChn])) <= (g_objMenuOsd.ViNormal==PAL?200:240) )
				return true;
		}
			break;
		case D1://D1->CIF
		{
			if( !g_objMenuOsd.VRecEn[v_iChn] || (4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame-3*atoi(g_objMenuOsd.VRecFps[v_iChn])) <= (g_objMenuOsd.ViNormal==PAL?200:240) )
				return true;
		}
			break;
	}

	return false;
}

bool CanSwitchFrame(int v_iChn)
{
	EN_MODE_FRAME objEnModeFrame = CountEnModeFrame();
	
	if( !g_objMenuOsd.VRecEn[v_iChn] || (4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame) <= (g_objMenuOsd.ViNormal==PAL?200:240) )
		return true;
	else
		return false;
}

bool CanEnableRecord(int v_iChn)
{
	EN_MODE_FRAME objEnModeFrame = CountEnModeFrame();
	
	if( !g_objMenuOsd.VRecEn[v_iChn] )//�ر�->����
	{
		switch(g_objMenuOsd.VRecMode[v_iChn])
		{
			case CIF:
			{
				if( 4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame+atoi(g_objMenuOsd.VRecFps[v_iChn]) > (g_objMenuOsd.ViNormal==PAL?200:240) )
					return false;
			}
				break;
			case HD1:
			{
				if( 4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame+2*atoi(g_objMenuOsd.VRecFps[v_iChn]) > (g_objMenuOsd.ViNormal==PAL?200:240) )
					return false;
			}
				break;
			case D1:
			{
				if( 4*objEnModeFrame.D1Frame+2*objEnModeFrame.HD1Frame+objEnModeFrame.CIFFrame+4*atoi(g_objMenuOsd.VRecFps[v_iChn]) > (g_objMenuOsd.ViNormal==PAL?200:240) )
					return false;
			}
				break;
		}
	}
	
	return true;
}
//-----------------------------------------------------------------------------------------------------------------

/*ң�ذ�������*/
int DealKeyLogin()
{
	memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
	
	MENU_PAGE *pCurMenuPage;
	MENU_ITEM *pCurMenuItem;
	
	g_bytCPageID = 0x00;				
	pCurMenuPage = GetPage();
	InitPage(pCurMenuPage);
	ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);

	g_bytCItemID = 0x00;
	pCurMenuItem = GetItem();
	ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
	
	g_bytLItemID_L0Page = 0x00;
	g_bytLItemID_L1Page = 0x00;
	g_bytLItemID_L2Page = 0x00;
	g_bytLItemID_L3Page = 0x00;
}

int DealKeyLock()
{
	MENU_PAGE *pCurMenuPage = GetPage();
	MENU_ITEM *pCurMenuItem = GetItem();	
		
	HideItem( pCurMenuItem );
	HidePage( pCurMenuPage );
}

int DealKeyDirect( int v_iKeyStyle )
{
	BYTE bytCurItemID;
	MENU_PAGE *pCurMenuPage = GetPage();
	MENU_ITEM *pCurMenuItem = GetItem();	
	
	bytCurItemID = GetNextID( pCurMenuItem, v_iKeyStyle );
	
	if( !pCurMenuItem->Fix && bytCurItemID!=g_bytCItemID )
	{
		ShowItem(pCurMenuPage, pCurMenuItem, pCurMenuItem->CharColor, pCurMenuItem->BackColor);//��ǰѡ����ʾԭɫ

		g_bytLItemID_L0Page = g_bytCItemID;
		g_bytCItemID = bytCurItemID;
		pCurMenuItem = GetItem();//ָ����һ��ѡ��
		
		while( !pCurMenuItem->Exist )
		{
			bytCurItemID = GetNextID( pCurMenuItem, v_iKeyStyle );
			
			if( bytCurItemID == g_bytCItemID )
				g_bytCItemID = g_bytLItemID_L0Page;
			else
				g_bytCItemID = bytCurItemID;
			
			pCurMenuItem = GetItem();
		}

		ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);//��ǰѡ����ʾ��ɫ
	}
}

int DealKeyNumber(char *pNumber)
{
		MENU_PAGE *pCurMenuPage = GetPage();
		MENU_ITEM *pCurMenuItem = GetItem();

		switch( g_bytCPageID )
		{
			case 0x00://ϵͳ��½
			{
				switch( g_bytCItemID )
				{
					case 0x00:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.LoginPassword, 10, 10, "*", strlen(g_objMenuOsd.LoginPassword)+1, NULL, 0 );
						break;
					default:
						break;
				}
			}
				break;
			case 0x03://�ļ���ѯ
			{
				switch( g_bytCItemID )
				{
					case 0x01:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.CreatDate[0], 4, 4, NULL, 0, "%s", (DWORD)g_objMenuOsd.CreatDate[0] );
						break;
					case 0x03:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.CreatDate[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.CreatDate[1] );
						break;
					case 0x05:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.CreatDate[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.CreatDate[2] );
						break;
					case 0x06:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.StartTime[0], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.StartTime[0] );
						break;
					case 0x08:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.StartTime[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.StartTime[1] );
						break;
					case 0x0a:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.StartTime[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.StartTime[2] );
						break;
					case 0x0b:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.EndTime[0], 2, 4, " ", 2, "%s", (DWORD)g_objMenuOsd.EndTime[0] );
						break;
					case 0x0d:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.EndTime[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.EndTime[1] );
						break;
					case 0x0f:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.EndTime[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.EndTime[2] );
						break;
					default:
						break;
				}
			}
				break;
			case 0x04://¼������
			{
				switch( g_bytCItemID )
				{
					case 0x01:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.CreatDate[0], 4, 4, NULL, 0, "%s", (DWORD)g_objMenuOsd.CreatDate[0] );
						break;
					case 0x03:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.CreatDate[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.CreatDate[1] );
						break;
					case 0x05:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.CreatDate[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.CreatDate[2] );
						break;
					case 0x06:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.StartTime[0], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.StartTime[0] );
						break;
					case 0x08:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.StartTime[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.StartTime[1] );
						break;
					case 0x0a:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.StartTime[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.StartTime[2] );
						break;
					case 0x0b:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.EndTime[0], 2, 4, " ", 2, "%s", (DWORD)g_objMenuOsd.EndTime[0] );
						break;
					case 0x0d:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.EndTime[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.EndTime[1] );
						break;
					case 0x0f:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.EndTime[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.EndTime[2] );
						break;
					default:
						break;
				}
			}
				break;
			case 0x08://������������
			{
				switch( g_bytCItemID )
				{
					case 0x00:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.SystemDate[0], 4, 4, NULL, 0, "%s", (DWORD)g_objMenuOsd.SystemDate[0] );
						break;
					case 0x02:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.SystemDate[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.SystemDate[1] );
						break;
					case 0x04:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.SystemDate[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.SystemDate[2] );
						break;
					case 0x05:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.SystemTime[0], 2, 4, " ", 2, "%s", (DWORD)g_objMenuOsd.SystemTime[0] );
						break;
					case 0x07:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.SystemTime[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.SystemTime[1] );
						break;
					case 0x09:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.SystemTime[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.SystemTime[2] );
						break;
					case 0x0a:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.DriverID, 12, 12, NULL, 0, "%s", (DWORD)g_objMenuOsd.DriverID );
						break;
					case 0x0b:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.CarNumber, 12, 12, NULL, 0, "%s", (DWORD)g_objMenuOsd.CarNumber );
						break;
					default:
						break;
				}
			}
				break;
			case 0x0a://��������
			{
				switch( g_bytCItemID )
				{
					case 0x00:/*��ǰ����*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.CurPassword, 10, 10, "*", strlen(g_objMenuOsd.CurPassword)+1, NULL, 0 );
						break;
					case 0x01:/*������*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.NewPassword[0], 10, 10, "*", strlen(g_objMenuOsd.NewPassword[0])+1, NULL, 0 );
						break;
					case 0x02:/*ȷ��������*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.NewPassword[1], 10, 10, "*", strlen(g_objMenuOsd.NewPassword[1])+1, NULL, 0 );
						break;
					default:
						break;
				}
			}
				break;
			case 0x0e://¼���������
			{
				switch( g_bytCItemID )
				{
					case 0x03:/*1ͨ��֡��*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.VRecFps[0], 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.VRecFps[0] );
						break;
					case 0x08:/*2ͨ��֡��*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.VRecFps[1], 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.VRecFps[1] );
						break;
					case 0x0d:/*3ͨ��֡��*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.VRecFps[2], 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.VRecFps[2] );
						break;
					case 0x12:/*4ͨ��֡��*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.VRecFps[3], 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.VRecFps[3] );
						break;
					default:
						break;
				}
			}
				break;
			case 0x0f://ʱ�δ�����������
			{
				switch( g_bytCItemID )
				{
					case 0x00:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSHour[0], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[0] );
						break;
					case 0x02:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSMin[0], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[0] );
						break;
					case 0x04:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSSec[0], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[0] );
						break;
					case 0x05:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEHour[0], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[0] );
						break;
					case 0x07:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEMin[0], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[0] );
						break;
					case 0x09:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodESec[0], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[0] );
						break;
					case 0x0b:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSHour[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[1] );
						break;
					case 0x0d:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSMin[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[1] );
						break;
					case 0x0f:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSSec[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[1] );
						break;
					case 0x10:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEHour[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[1] );
						break;
					case 0x12:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEMin[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[1] );
						break;
					case 0x14:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodESec[1], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[1] );
						break;
					case 0x16:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSHour[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[2] );
						break;
					case 0x18:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSMin[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[2] );
						break;
					case 0x1a:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSSec[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[2] );
						break;
					case 0x1b:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEHour[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[2] );
						break;
					case 0x1d:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEMin[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[2] );
						break;
					case 0x1f:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodESec[2], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[2] );
						break;
					case 0x21:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSHour[3], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[3] );
						break;
					case 0x23:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSMin[3], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[3] );
						break;
					case 0x25:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSSec[3], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[3] );
						break;
					case 0x26:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEHour[3], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[3] );
						break;
					case 0x28:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEMin[3], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[3] );
						break;
					case 0x2a:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodESec[3], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[3] );
						break;
					case 0x2c:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSHour[4], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[4] );
						break;
					case 0x2e:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSMin[4], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[4] );
						break;
					case 0x30:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSSec[4], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[4] );
						break;
					case 0x31:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEHour[4], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[4] );
						break;
					case 0x33:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEMin[4], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[4] );
						break;
					case 0x35:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodESec[4], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[4] );
						break;
					case 0x37:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSHour[5], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSHour[5] );
						break;
					case 0x39:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSMin[5], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSMin[5] );
						break;
					case 0x3b:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodSSec[5], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodSSec[5] );
						break;
					case 0x3c:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEHour[5], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEHour[5] );
						break;
					case 0x3e:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodEMin[5], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodEMin[5] );
						break;
					case 0x40:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.PeriodESec[5], 2, 2, NULL, 0, "%s", (DWORD)g_objMenuOsd.PeriodESec[5] );
						break;
					default:
						break;
				}
			}
				break;
			case 0x10://�¼�������������
			{
				switch( g_bytCItemID )
				{
					case 0x00:/*�����ʱ*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.AccDelay, 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.AccDelay );
						break;
					case 0x02:/*�˶���ʱ*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.MoveDelay, 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.MoveDelay );
						break;
					case 0x04:/*�ڵ���ʱ*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.OverlayDelay, 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.OverlayDelay );
						break;
					case 0x06:/*S1��ʱ*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.S1Delay, 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.S1Delay );
						break;
					case 0x08:/*S2��ʱ*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.S2Delay, 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.S2Delay );
						break;
					case 0x0a:/*S3��ʱ*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.S3Delay, 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.S3Delay );
						break;
					case 0x0c:/*S4��ʱ*/
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.S4Delay, 2, 4, " ", 1, "%s", (DWORD)g_objMenuOsd.S4Delay );
						break;
					default:
						break;
				}
			}
				break;
			case 0x11:
			{
				switch( g_bytCItemID )
				{
					case 0x01:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_szTelNum, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_szTelNum );
						break;
					case 0x02:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_szApn, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_szApn );
						break;
					case 0x03:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_ulCommIP, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_ulCommIP );
						break;
					case 0x04:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_usCommPort, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_usCommPort );
						break;
					case 0x05:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_ulPhotoIP, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_ulPhotoIP );
						break;
					case 0x06:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_usPhotoPort, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_usPhotoPort );
						break;
					case 0x07:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_ulVideoIP, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_ulVideoIP );
						break;
					case 0x08:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_usVideoPort, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_usVideoPort );
						break;
					case 0x09:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_ulUpdateIP, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_ulUpdateIP );
						break;
					case 0x0a:
						DealNumInput( pCurMenuPage, pCurMenuItem, pNumber, g_objMenuOsd.m_usUpdatePort, 19, 19, NULL, 0, "%s", (DWORD)g_objMenuOsd.m_usUpdatePort );
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

int DealKeyEnter()
{
		int i;
		MENU_PAGE *pCurMenuPage = GetPage();
		MENU_ITEM *pCurMenuItem = GetItem();
		
		switch(g_bytCPageID)
		{
			case 0x00:/*ϵͳ��½*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*���������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.LoginPassword, 0, sizeof(g_objMenuOsd.LoginPassword));
								FlushItem(pCurMenuItem->Content, 10, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
		
						case 0x01:/*��½��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( strcmp(g_objMvrCfg.PasswordPara.UserPassWord, g_objMenuOsd.LoginPassword)==0 ||
										strcmp(g_objMvrCfg.PasswordPara.AdminPassWord, g_objMenuOsd.LoginPassword)==0 )/*��½�ɹ�*/
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);//ɾ����ǰҳ��

									g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);//��ʾ��һ��ҳ��
									
									g_bytCItemID = 0x00;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);//��һѡ����ʾ��ɫ
								}
								else/*��½ʧ��*/
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
									pCurMenuItem->Fix = TRUE;
								}
							}
						}
							break;
						default:
							break;
					}
			}
				break;
			case 0x01:
			{
					switch(g_bytCItemID)
					{
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
						case 0x04:
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
								
								g_bytLItemID_L1Page = g_bytCItemID;
								g_bytCItemID = 0x00;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
		
						case 0x05:
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);						
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
								
								g_bytLItemID_L1Page = g_bytCItemID;
								g_bytCItemID = 0x00;
								pCurMenuItem = GetItem();
							}
						}
							break;
		
						default:
							break;
					}
			}
				break;
			case 0x02:
			{
					switch(g_bytCItemID)
					{
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);

								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
								
								g_bytLItemID_L2Page = g_bytLItemID_L1Page;
								g_bytLItemID_L1Page = g_bytCItemID;
								g_bytCItemID = 0x00;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x03:/*�ļ���ѯ*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*ͨ��ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								switch(g_objMenuOsd.EncChn)
								{
									case '1':
										g_objMenuOsd.EncChn = '2';
										FlushItem(pCurMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"ͨ  ��  2");
										break;
									case '2':
										g_objMenuOsd.EncChn = '3';
										FlushItem(pCurMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"ͨ  ��  3");
										break;
									case '3':
										g_objMenuOsd.EncChn = '4';
										FlushItem(pCurMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"ͨ  ��  4");
										break;
									case '4':
										g_objMenuOsd.EncChn = 'A';
										FlushItem(pCurMenuItem->Content, 10, " ", 1, "%s", (DWORD)"����ͨ��");
										break;
									case 'A':
										g_objMenuOsd.EncChn = '1';
										FlushItem(pCurMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"ͨ  ��  1");
										break;
									default:
										break;
								}
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x01:/*��ѯ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.CreatDate[0], 0, sizeof(g_objMenuOsd.CreatDate[0]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if( strlen(g_objMenuOsd.CreatDate[0])!=4 || atoi(g_objMenuOsd.CreatDate[0])<1970 )
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x03:/*��ѯ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.CreatDate[1], 0, sizeof(g_objMenuOsd.CreatDate[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.CreatDate[1]) != 2 || atoi(g_objMenuOsd.CreatDate[1]) < 1 || atoi(g_objMenuOsd.CreatDate[1]) > 12)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x05:/*��ѯ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.CreatDate[2], 0, sizeof(g_objMenuOsd.CreatDate[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.CreatDate[2]) != 2 || atoi(g_objMenuOsd.CreatDate[2]) < 1 || atoi(g_objMenuOsd.CreatDate[2]) > 31)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x06:/*��ѯ��ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.StartTime[0], 0, sizeof(g_objMenuOsd.StartTime[0]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.StartTime[0]) != 2 || atoi(g_objMenuOsd.StartTime[0]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x08:/*��ѯ��ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.StartTime[1], 0, sizeof(g_objMenuOsd.StartTime[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.StartTime[1]) != 2 || atoi(g_objMenuOsd.StartTime[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0a:/*��ѯ��ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.StartTime[2], 0, sizeof(g_objMenuOsd.StartTime[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.StartTime[2]) != 2 || atoi(g_objMenuOsd.StartTime[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0b:/*��ѯ����ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.EndTime[0], 0, sizeof(g_objMenuOsd.EndTime[0]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.EndTime[0]) != 2 || atoi(g_objMenuOsd.EndTime[0]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0d:/*��ѯ������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.EndTime[1], 0, sizeof(g_objMenuOsd.EndTime[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.EndTime[1]) != 2 || atoi(g_objMenuOsd.EndTime[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0f:/*��ѯ������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.EndTime[2], 0, sizeof(g_objMenuOsd.EndTime[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.EndTime[2]) != 2 || atoi(g_objMenuOsd.EndTime[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x10:/*����ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if(strcmp("SDisk", g_objMenuOsd.DiskType) == 0)
								{
									strcpy(g_objMenuOsd.DiskType, "HDisk");
									FlushItem(pCurMenuItem->Content, 10, " ", 2, "%s", (DWORD)"Ӳ  ��");
								}
								else if(strcmp("HDisk", g_objMenuOsd.DiskType) == 0)
								{
									strcpy(g_objMenuOsd.DiskType, "SDisk");
									FlushItem(pCurMenuItem->Content, 10, " ", 2, "%s", (DWORD)"SD  ��");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x11:/*��ѯ*/
						{
							if( !pCurMenuItem->Fix )
							{
								int i, j;
								FILE_SEARCH objFileSearch = {FALSE};
			
								switch(g_objMenuOsd.EncChn)
								{
									case '1':
										objFileSearch.SearchStart[0] = TRUE;
										break;
									case '2':
										objFileSearch.SearchStart[1] = TRUE;
										break;
									case '3':
										objFileSearch.SearchStart[2] = TRUE;
										break;
									case '4':
										objFileSearch.SearchStart[3] = TRUE;
										break;
									case 'A':
										for(i = 0; i < 4; i++)
										{
											objFileSearch.SearchStart[i] = TRUE;
										}
										break;
									default:
										break;
								}
								
								strcpy(objFileSearch.DiskType, g_objMenuOsd.DiskType);
								strcpy(objFileSearch.CreatDate[0], g_objMenuOsd.CreatDate[0]);
								strcpy(objFileSearch.CreatDate[1], g_objMenuOsd.CreatDate[1]);
								strcpy(objFileSearch.CreatDate[2], g_objMenuOsd.CreatDate[2]);
								sprintf(objFileSearch.StartTime, "%s:%s:%s", g_objMenuOsd.StartTime[0], g_objMenuOsd.StartTime[1], g_objMenuOsd.StartTime[2]);
								sprintf(objFileSearch.EndTime, "%s:%s:%s", g_objMenuOsd.EndTime[0], g_objMenuOsd.EndTime[1], g_objMenuOsd.EndTime[2]);

								g_objMenuOsd.FileNum = FileSearch(&(g_objMenuOsd.AVFileIndex), objFileSearch);
								g_objMenuOsd.FilePageNum = (g_objMenuOsd.FileNum%7==0) ? (g_objMenuOsd.FileNum/7) : (g_objMenuOsd.FileNum/7+1);
								g_objMenuOsd.FilePageIndex = 1;
								
								if(g_objMenuOsd.FileNum > 0)/*��ѯ�ɹ�*/
								{
									HidePage(pCurMenuPage);
						
									g_bytCPageID = GetNextID(pCurMenuItem, ENTER);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
		
									for(i = 0; i < 7; i++)
									{
										j = 7*(g_objMenuOsd.FilePageIndex-1)+i;	
		
										if(j < g_objMenuOsd.FileNum)
											g_objMenuOsd.FileIndex[i] = j;
										else
											break;
		
										sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														"   %s   %s-%s-%s   %s:%s:%s   %s:%s:%s   %s   %s   %sM\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2],
													 	g_objMenuOsd.AVFileIndex[j].Framerate,
													 	g_objMenuOsd.AVFileIndex[j].Mode,
													 	g_objMenuOsd.AVFileIndex[j].VSize
													 );
										pCurMenuPage->MenuItem[i].Exist = TRUE;
									}
									sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
		
									/*��ʾ��ѯ���*/
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
									
									g_bytLItemID_L2Page = g_bytLItemID_L1Page;
									g_bytLItemID_L1Page = g_bytCItemID;
									g_bytCItemID = 0x09;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								}
								else/*��ѯʧ��*/
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
									pCurMenuItem->Fix = TRUE;
								}
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x04:/*¼������*/
			{
					switch(g_bytCItemID)
					{		
						case 0x00:/*ͨ��ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								switch(g_objMenuOsd.EncChn)
								{
									case '1':
										g_objMenuOsd.EncChn = '2';
										FlushItem(pCurMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"ͨ  ��  2");
										break;
									case '2':
										g_objMenuOsd.EncChn = '3';
										FlushItem(pCurMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"ͨ  ��  3");
										break;
									case '3':
										g_objMenuOsd.EncChn = '4';
										FlushItem(pCurMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"ͨ  ��  4");
										break;
									case '4':
										g_objMenuOsd.EncChn = 'A';
										FlushItem(pCurMenuItem->Content, 10, " ", 1, "%s", (DWORD)"����ͨ��");
										break;
									case 'A':
										g_objMenuOsd.EncChn = '1';
										FlushItem(pCurMenuItem->Content, 10, NULL, 0, "%s", (DWORD)"ͨ  ��  1");
										break;
									default:
										break;
								}
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
								
						case 0x01:/*������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.CreatDate[0], 0, sizeof(g_objMenuOsd.CreatDate[0]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.CreatDate[0]) != 4 || atoi(g_objMenuOsd.CreatDate[0]) < 1970)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x03:/*������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.CreatDate[1], 0, sizeof(g_objMenuOsd.CreatDate[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.CreatDate[1]) != 2 || atoi(g_objMenuOsd.CreatDate[1]) < 1 || atoi(g_objMenuOsd.CreatDate[1]) > 12)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x05:/*������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.CreatDate[2], 0, sizeof(g_objMenuOsd.CreatDate[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.CreatDate[2]) != 2 || atoi(g_objMenuOsd.CreatDate[2]) < 1 || atoi(g_objMenuOsd.CreatDate[2]) > 31)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x06:/*������ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.StartTime[0], 0, sizeof(g_objMenuOsd.StartTime[0]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.StartTime[0]) != 2 || atoi(g_objMenuOsd.StartTime[0]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x08:/*������ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.StartTime[1], 0, sizeof(g_objMenuOsd.StartTime[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.StartTime[1]) != 2 || atoi(g_objMenuOsd.StartTime[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0a:/*������ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.StartTime[2], 0, sizeof(g_objMenuOsd.StartTime[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.StartTime[2]) != 2 || atoi(g_objMenuOsd.StartTime[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0b:/*��������ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.EndTime[0], 0, sizeof(g_objMenuOsd.EndTime[0]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.EndTime[0]) != 2 || atoi(g_objMenuOsd.EndTime[0]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0d:/*����������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.EndTime[1], 0, sizeof(g_objMenuOsd.EndTime[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.EndTime[1]) != 2 || atoi(g_objMenuOsd.EndTime[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0f:/*����������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.EndTime[2], 0, sizeof(g_objMenuOsd.EndTime[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.EndTime[2]) != 2 || atoi(g_objMenuOsd.EndTime[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x10:/*����ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if(strcmp("SDisk", g_objMenuOsd.DiskType) == 0)
								{
									strcpy(g_objMenuOsd.DiskType, "HDisk");
									FlushItem(pCurMenuItem->Content, 10, " ", 2, "%s", (DWORD)"Ӳ  ��");
								}
								else if(strcmp("HDisk", g_objMenuOsd.DiskType) == 0)
								{
									strcpy(g_objMenuOsd.DiskType, "SDisk");
									FlushItem(pCurMenuItem->Content, 10, " ", 2, "%s", (DWORD)"SD  ��");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x11:/*����*/
						{
							if( !pCurMenuItem->Fix )
							{
								int i, j;
								FILE_SEARCH objFileSearch = {FALSE};
			
								switch(g_objMenuOsd.EncChn)
								{
									case '1':
										objFileSearch.SearchStart[0] = TRUE;
										break;
									case '2':
										objFileSearch.SearchStart[1] = TRUE;
										break;
									case '3':
										objFileSearch.SearchStart[2] = TRUE;
										break;
									case '4':
										objFileSearch.SearchStart[3] = TRUE;
										break;
									case 'A':
										for(i = 0; i < 4; i++)
										{
											objFileSearch.SearchStart[i] = TRUE;
										}
										break;
									default:
										break;
								}
								
								strcpy(objFileSearch.DiskType, g_objMenuOsd.DiskType);
								strcpy(objFileSearch.CreatDate[0], g_objMenuOsd.CreatDate[0]);
								strcpy(objFileSearch.CreatDate[1], g_objMenuOsd.CreatDate[1]);
								strcpy(objFileSearch.CreatDate[2], g_objMenuOsd.CreatDate[2]);
								sprintf(objFileSearch.StartTime, "%s:%s:%s", g_objMenuOsd.StartTime[0], g_objMenuOsd.StartTime[1], g_objMenuOsd.StartTime[2]);
								sprintf(objFileSearch.EndTime, "%s:%s:%s", g_objMenuOsd.EndTime[0], g_objMenuOsd.EndTime[1], g_objMenuOsd.EndTime[2]);
									
								g_objMenuOsd.FileNum = FileSearch(&(g_objMenuOsd.AVFileIndex), objFileSearch);
								g_objMenuOsd.FilePageNum = (g_objMenuOsd.FileNum%7==0) ? (g_objMenuOsd.FileNum/7) : (g_objMenuOsd.FileNum/7+1);
								g_objMenuOsd.FilePageIndex = 1;
								
								if(g_objMenuOsd.FileNum > 0)/*�����ɹ�*/
								{
									HidePage(pCurMenuPage);
						
									g_bytCPageID = GetNextID(pCurMenuItem, ENTER);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
		
									for(i = 0; i < 7; i++)
									{
										j = 7*(g_objMenuOsd.FilePageIndex-1)+i;	
		
										if(j < g_objMenuOsd.FileNum)
											g_objMenuOsd.FileIndex[i] = j;
										else
											break;
		
										sprintf(pCurMenuPage->MenuItem[i].Content, 
														" %s   %s-%s-%s   %s:%s:%s    %s:%s:%s\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2]);
										pCurMenuPage->MenuItem[i].Exist = TRUE;
									}
									sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
		
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);//��ʾ�������
									
									g_bytLItemID_L2Page = g_bytLItemID_L1Page;
									g_bytLItemID_L1Page = g_bytCItemID;
									g_bytCItemID = 0x09;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								}
								else/*����ʧ��*/
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
									pCurMenuItem->Fix = TRUE;
								}
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x05:/*���̲���*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*����ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if(strcmp("SDisk", g_objMenuOsd.DiskType) == 0)
								{
									strcpy(g_objMenuOsd.DiskType, "HDisk");
									FlushItem(pCurMenuItem->Content, 6, NULL, 0, "%s", (DWORD)"Ӳ  ��");
								}
								else if(strcmp("HDisk", g_objMenuOsd.DiskType) == 0)
								{
									strcpy(g_objMenuOsd.DiskType, "SDisk");
									FlushItem(pCurMenuItem->Content, 6, NULL, 0, "%s", (DWORD)"SD  ��");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x01:/*������ʽ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_INFO);

								if( DiskFormat(g_objMenuOsd.DiskType)==0 )
								{
									HideItem(pCurMenuItem);
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
								}
								else
								{
									HideItem(pCurMenuItem);
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
								}
							}
						}
							break;
							
						default:
							break;	
					}
			}
				break;
			case 0x06:/*��������*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*�ָ���������*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								
								if(ResumeCfg(false,false)==0)
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
								}
								else
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
							}
						}
							break;
							
						case 0x01:/*Ӧ������*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_INFO);

								if( access("/mnt/UDisk/part1/exe.bin", F_OK)==0 ? ExeUpdate() : AppUpdate() )
								{
									HideItem(pCurMenuItem);
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									
									sleep(3);
									
									G_ResetSystem();
								}
								else
								{
									HideItem(pCurMenuItem);
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
								}
							}
						}
							break;
							
						case 0x02:/*ϵͳ����*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_INFO);
								
								if( access("/mnt/UDisk/part1/image.bin", F_OK)==0 ? ImageUpdate() : SysUpdate() )
								{
									HideItem(pCurMenuItem);
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									
									sleep(3);
									
									G_ResetSystem();
								}
								else
								{
									HideItem(pCurMenuItem);
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
								}
							}
						}
							break;
							
						default:
							break;	
					}
			}
				break;
			case 0x08:/*��������*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.SystemDate[0], 0, sizeof(g_objMenuOsd.SystemDate[0]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.SystemDate[0]) != 4 || atoi(g_objMenuOsd.SystemDate[0]) < 2010)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x02:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.SystemDate[1], 0, sizeof(g_objMenuOsd.SystemDate[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.SystemDate[1]) != 2 || atoi(g_objMenuOsd.SystemDate[1]) < 1 || atoi(g_objMenuOsd.SystemDate[1]) > 12)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x04:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.SystemDate[2], 0, sizeof(g_objMenuOsd.SystemDate[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.SystemDate[2]) != 2 || atoi(g_objMenuOsd.SystemDate[2]) < 1 || atoi(g_objMenuOsd.SystemDate[2]) > 31)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x05:/*ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.SystemTime[0], 0, sizeof(g_objMenuOsd.SystemTime[0]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.SystemTime[0]) != 2 || atoi(g_objMenuOsd.SystemTime[0]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x07:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.SystemTime[1], 0, sizeof(g_objMenuOsd.SystemTime[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.SystemTime[1]) != 2 || atoi(g_objMenuOsd.SystemTime[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x09:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.SystemTime[2], 0, sizeof(g_objMenuOsd.SystemTime[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.SystemTime[2]) != 2 || atoi(g_objMenuOsd.SystemTime[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0a:/*˾�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.DriverID, 0, sizeof(g_objMenuOsd.DriverID));
								FlushItem(pCurMenuItem->Content, 12, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.DriverID) != 12)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0b:/*���ƺ���*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.CarNumber, 0, sizeof(g_objMenuOsd.CarNumber));
								FlushItem(pCurMenuItem->Content, 12, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.CarNumber) != 12)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
		
						case 0x0c:/*ȷ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								
								tag2DDvrCfg objMvrCfg;
								GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));			
								strcpy(objMvrCfg.DeviceInfo.DriverID, g_objMenuOsd.DriverID);
								strcpy(objMvrCfg.DeviceInfo.CarNumber, g_objMenuOsd.CarNumber);					
								
								struct rtc_time objRtcTime = 
								{
									atoi(g_objMenuOsd.SystemTime[2]), atoi(g_objMenuOsd.SystemTime[1]), atoi(g_objMenuOsd.SystemTime[0]),
									atoi(g_objMenuOsd.SystemDate[2]), atoi(g_objMenuOsd.SystemDate[1])-1, atoi(g_objMenuOsd.SystemDate[0]),
									0, 0, 0
								};
								
								if(SetRtcTime(&objRtcTime)==0 && SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg))==0)
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									
									SetTimeByRtc();
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
								}
								else
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x09:
			{
					switch( g_bytCItemID )
					{
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
								
								g_bytLItemID_L3Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L1Page;
								g_bytLItemID_L1Page = g_bytCItemID;
								g_bytCItemID = 0x00;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x0a:/*��������*/
			{
					switch( g_bytCItemID )
					{
						case 0x00:/*��ǰ����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.CurPassword, 0, sizeof(g_objMenuOsd.CurPassword));
								FlushItem(pCurMenuItem->Content, 10, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x01:/*������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.NewPassword[0], 0, sizeof(g_objMenuOsd.NewPassword[0]));
								FlushItem(pCurMenuItem->Content, 10, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x02:/*ȷ��������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.NewPassword[1], 0, sizeof(g_objMenuOsd.NewPassword[1]));
								FlushItem(pCurMenuItem->Content, 10, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
		
						case 0x03:/*ȷ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								
								/*��������*/
								int iRet;
								
								tag2DDvrCfg objMvrCfg;
		            GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								if( strcmp(objMvrCfg.PasswordPara.UserPassWord, g_objMenuOsd.CurPassword)==0 &&
									 	strcmp(g_objMenuOsd.NewPassword[0], g_objMenuOsd.NewPassword[1])==0)
								{
									strcpy(objMvrCfg.PasswordPara.UserPassWord, g_objMenuOsd.NewPassword[0]);
									iRet = SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								}
								else
									iRet = -1;
							
								if(iRet==0)
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
								}
								else
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x0b:/*��ѯ���*/
			{
					switch( g_bytCItemID )
					{
						case 0x00:
						{
							if( !g_objMenuOsd.FileFix[0] )
							{
								g_objMenuOsd.FileFix[0] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[0] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x01:
						{
							if( !g_objMenuOsd.FileFix[1] )
							{
								g_objMenuOsd.FileFix[1] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[1] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x02:
						{
							if( !g_objMenuOsd.FileFix[2] )
							{
								g_objMenuOsd.FileFix[2] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[2] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x03:
						{
							if( !g_objMenuOsd.FileFix[3] )
							{
								g_objMenuOsd.FileFix[3] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[3] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x04:
						{
							if( !g_objMenuOsd.FileFix[4] )
							{
								g_objMenuOsd.FileFix[4] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[4] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x05:
						{
							if( !g_objMenuOsd.FileFix[5] )
							{
								g_objMenuOsd.FileFix[5] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[5] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x06:
						{
							if( !g_objMenuOsd.FileFix[6] )
							{
								g_objMenuOsd.FileFix[6] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[6] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x07:/*��ҳ*/
						{
							if( !pCurMenuItem->Fix && g_objMenuOsd.FilePageIndex != 1)
							{
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
								pCurMenuPage = GetPage();	
								InitPage(pCurMenuPage);
								
								memset(g_objMenuOsd.FileIndex, 0, sizeof(g_objMenuOsd.FileIndex));
								memset(g_objMenuOsd.FileFix, 0, sizeof(g_objMenuOsd.FileFix));
								g_objMenuOsd.FilePageIndex = 1;
								
								int i, j;
								
								for(i = 0; i < 7; i++)
								{
									j = 7*(g_objMenuOsd.FilePageIndex-1)+i;	
		
									if(j < g_objMenuOsd.FileNum)
										g_objMenuOsd.FileIndex[i] = j;
									else
										break;
		
									sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														"   %s   %s-%s-%s   %s:%s:%s   %s:%s:%s   %s   %s   %sM\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2],
													 	g_objMenuOsd.AVFileIndex[j].Framerate,
													 	g_objMenuOsd.AVFileIndex[j].Mode,
													 	g_objMenuOsd.AVFileIndex[j].VSize
													 );
									pCurMenuPage->MenuItem[i].Exist = TRUE;
								}		
								sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
				
								ShowPage(pCurMenuPage, 0, PAGE_LINE, PAGE_ROW);	
									
								g_bytCItemID = 0x09;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
		
						case 0x08:/*��ҳ*/
						{
							if( !pCurMenuItem->Fix && g_objMenuOsd.FilePageIndex > 1)
							{
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
								pCurMenuPage = GetPage();	
								InitPage(pCurMenuPage);
								
								memset(g_objMenuOsd.FileIndex, 0, sizeof(g_objMenuOsd.FileIndex));
								memset(g_objMenuOsd.FileFix, 0, sizeof(g_objMenuOsd.FileFix));
								g_objMenuOsd.FilePageIndex -= 1;
								
								int i, j;
								
								for(i = 0; i < 7; i++)
								{
									j = 7 * (g_objMenuOsd.FilePageIndex - 1) + i;	
		
									if(j < g_objMenuOsd.FileNum)
										g_objMenuOsd.FileIndex[i] = j;
									else
										break;
		
									sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														"   %s   %s-%s-%s   %s:%s:%s   %s:%s:%s   %s   %s   %sM\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2],
													 	g_objMenuOsd.AVFileIndex[j].Framerate,
													 	g_objMenuOsd.AVFileIndex[j].Mode,
													 	g_objMenuOsd.AVFileIndex[j].VSize
													 );
									pCurMenuPage->MenuItem[i].Exist = TRUE;
								}		
								sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
				
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);	
									
								g_bytCItemID = 0x08;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x09:/*��ҳ*/
						{
							if( !pCurMenuItem->Fix && g_objMenuOsd.FilePageIndex < g_objMenuOsd.FilePageNum)
							{
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
								pCurMenuPage = GetPage();	
								InitPage(pCurMenuPage);
								
								memset(g_objMenuOsd.FileIndex, 0, sizeof(g_objMenuOsd.FileIndex));
								memset(g_objMenuOsd.FileFix, 0, sizeof(g_objMenuOsd.FileFix));
								g_objMenuOsd.FilePageIndex += 1;
								
								int i, j;
								
								for(i = 0; i < 7; i++)
								{
									j = 7*(g_objMenuOsd.FilePageIndex-1)+i;	
		
									if(j < g_objMenuOsd.FileNum)
										g_objMenuOsd.FileIndex[i] = j;
									else
										break;
		
									sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														"   %s   %s-%s-%s   %s:%s:%s   %s:%s:%s   %s   %s   %sM\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2],
													 	g_objMenuOsd.AVFileIndex[j].Framerate,
													 	g_objMenuOsd.AVFileIndex[j].Mode,
													 	g_objMenuOsd.AVFileIndex[j].VSize
													 );
									pCurMenuPage->MenuItem[i].Exist = TRUE;
								}		
								sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
				
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);	
									
								g_bytCItemID = 0x09;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0a:/*ĩҳ*/
						{
							if( !pCurMenuItem->Fix && g_objMenuOsd.FilePageIndex != g_objMenuOsd.FilePageNum)
							{
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
								pCurMenuPage = GetPage();	
								InitPage(pCurMenuPage);
								
								memset(g_objMenuOsd.FileIndex, 0, sizeof(g_objMenuOsd.FileIndex));
								memset(g_objMenuOsd.FileFix, 0, sizeof(g_objMenuOsd.FileFix));
								g_objMenuOsd.FilePageIndex = g_objMenuOsd.FilePageNum;
								
								int i, j;
								
								for(i = 0; i < 7; i++)
								{
									j = 7*(g_objMenuOsd.FilePageIndex-1)+i;	
		
									if(j < g_objMenuOsd.FileNum)
										g_objMenuOsd.FileIndex[i] = j;
									else
										break;
		
									sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														"   %s   %s-%s-%s   %s:%s:%s   %s:%s:%s   %s   %s   %sM\n",
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2],
													 	g_objMenuOsd.AVFileIndex[j].Framerate,
													 	g_objMenuOsd.AVFileIndex[j].Mode,
													 	g_objMenuOsd.AVFileIndex[j].VSize
													 );
									pCurMenuPage->MenuItem[i].Exist = TRUE;
								}		
								sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
				
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);	
									
								g_bytCItemID = 0x08;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0b:/*����*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								
								if( VALID(&g_objUsbSta.UDisk[0]) 
									&&( g_objMenuOsd.FileFix[0] || g_objMenuOsd.FileFix[1] || g_objMenuOsd.FileFix[2] || g_objMenuOsd.FileFix[3] || g_objMenuOsd.FileFix[4] || g_objMenuOsd.FileFix[5] || g_objMenuOsd.FileFix[6] ) 
									)
								{
									int i, j;
									char szSFilePath[255], szTFilePath[255];
									
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_INFO);
								
									for(i = 0; i < 7; i++)
									{
										if( g_objMenuOsd.FileFix[i] )
										{
											j = g_objMenuOsd.FileIndex[i];

											sprintf(szSFilePath, "/mnt/%s/part1/data/%s", g_objMenuOsd.DiskType, g_objMenuOsd.AVFileIndex[j].VPath);
											sprintf(  szTFilePath, 
																"/mnt/UDisk/part1/data/chn%s_%s.%s.%s_%s.%s.%s_%s.%s.%s.264", 
																g_objMenuOsd.AVFileIndex[j].EncChn,
																g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
																g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2], 
																g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2] );
											
											PRTMSG(MSG_DBG, "copy file %s\n", szSFilePath);
											if(FileCopy(szSFilePath, szTFilePath) <= 0)
											{
												HideItem(pCurMenuItem);
												ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
												return 0;
											}

//											sprintf(szSFilePath, "/mnt/%s/part1/data/%s", g_objMenuOsd.DiskType, g_objMenuOsd.AVFileIndex[j].APath);
//											sprintf(szTFilePath, "/mnt/UDisk/part1/data/%c%c%c%c_%s", g_objMenuOsd.AVFileIndex[j].APath+16);
											
//											PRTMSG(MSG_DBG, "copy file %s\n", szSFilePath);
//											if(FileCopy(szSFilePath, szTFilePath) <= 0)
//											{
//												HideItem(pCurMenuItem);
//												ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
//												return 0;
//											}
										}
									}
									
									HideItem(pCurMenuItem);
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
								}
								else
								{
									HideItem(pCurMenuItem);
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
								}
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x0c:/*�������*/
			{
					switch( g_bytCItemID )
					{
						case 0x00:
						{
							if( !g_objMenuOsd.FileFix[0] )
							{
								g_objMenuOsd.FileFix[0] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[0] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x01:
						{
							if( !g_objMenuOsd.FileFix[1] )
							{
								g_objMenuOsd.FileFix[1] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[1] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x02:
						{
							if( !g_objMenuOsd.FileFix[2] )
							{
								g_objMenuOsd.FileFix[2] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[2] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x03:
						{
							if( !g_objMenuOsd.FileFix[3] )
							{
								g_objMenuOsd.FileFix[3] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[3] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x04:
						{
							if( !g_objMenuOsd.FileFix[4] )
							{
								g_objMenuOsd.FileFix[4] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[4] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x05:
						{
							if( !g_objMenuOsd.FileFix[5] )
							{
								g_objMenuOsd.FileFix[5] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[5] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x06:
						{
							if( !g_objMenuOsd.FileFix[6] )
							{
								g_objMenuOsd.FileFix[6] = TRUE;
								pCurMenuItem->BackColor = RED;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, RED);
							}
							else
							{
								g_objMenuOsd.FileFix[6] = FALSE;
								pCurMenuItem->BackColor = BLUE;
								ShowItem(pCurMenuPage, pCurMenuItem, WHITE, BLUE);
							}
						}
							break;
							
						case 0x07:/*��ҳ*/
						{
							if( !pCurMenuItem->Fix && g_objMenuOsd.FilePageIndex != 1)
							{
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
								pCurMenuPage = GetPage();	
								InitPage(pCurMenuPage);
								
								memset(g_objMenuOsd.FileIndex, 0, sizeof(g_objMenuOsd.FileIndex));
								memset(g_objMenuOsd.FileFix, 0, sizeof(g_objMenuOsd.FileFix));
								g_objMenuOsd.FilePageIndex = 1;
		
								int i, j;
								for(i = 0; i < 7; i++)
								{
									j = 7*(g_objMenuOsd.FilePageIndex-1)+i;	
		
									if(j < g_objMenuOsd.FileNum)
										g_objMenuOsd.FileIndex[i] = j;
									else
										break;
		
									sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														" %s   %s-%s-%s   %s:%s:%s    %s:%s:%s\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2]
													 );
									pCurMenuPage->MenuItem[i].Exist = TRUE;
								}		
								sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
		
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);	
									
								g_bytCItemID = 0x09;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
		
						case 0x08:/*��ҳ*/
						{
							if( !pCurMenuItem->Fix  && g_objMenuOsd.FilePageIndex > 1)
							{
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
								pCurMenuPage = GetPage();	
								InitPage(pCurMenuPage);
								
								memset(g_objMenuOsd.FileIndex, 0, sizeof(g_objMenuOsd.FileIndex));
								memset(g_objMenuOsd.FileFix, 0, sizeof(g_objMenuOsd.FileFix));
								g_objMenuOsd.FilePageIndex -= 1;
		
								int i, j;
								for(i = 0; i < 7; i++)
								{
									j = 7 * (g_objMenuOsd.FilePageIndex - 1) + i;	
		
									if(j < g_objMenuOsd.FileNum)
										g_objMenuOsd.FileIndex[i] = j;
									else
										break;
		
									sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														" %s   %s-%s-%s   %s:%s:%s    %s:%s:%s\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2]
													 );
									pCurMenuPage->MenuItem[i].Exist = TRUE;
								}		
								sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
		
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);	
									
								g_bytCItemID = 0x08;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x09:/*��ҳ*/
						{
							if( !pCurMenuItem->Fix && g_objMenuOsd.FilePageIndex < g_objMenuOsd.FilePageNum)
							{
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
								pCurMenuPage = GetPage();	
								InitPage(pCurMenuPage);
								
								memset(g_objMenuOsd.FileIndex, 0, sizeof(g_objMenuOsd.FileIndex));
								memset(g_objMenuOsd.FileFix, 0, sizeof(g_objMenuOsd.FileFix));
								g_objMenuOsd.FilePageIndex += 1;
		
								int i, j;
								for(i = 0; i < 7; i++)
								{
									j = 7 * (g_objMenuOsd.FilePageIndex - 1) + i;	
		
									if(j < g_objMenuOsd.FileNum)
										g_objMenuOsd.FileIndex[i] = j;
									else
										break;
		
									sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														" %s   %s-%s-%s   %s:%s:%s    %s:%s:%s\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2]
													 );
									pCurMenuPage->MenuItem[i].Exist = TRUE;
								}		
								sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
		
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);	
									
								g_bytCItemID = 0x09;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0a:/*ĩҳ*/
						{
							if( !pCurMenuItem->Fix && g_objMenuOsd.FilePageIndex != g_objMenuOsd.FilePageNum)
							{
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, ENTER);
								pCurMenuPage = GetPage();	
								InitPage(pCurMenuPage);
								
								memset(g_objMenuOsd.FileIndex, 0, sizeof(g_objMenuOsd.FileIndex));
								memset(g_objMenuOsd.FileFix, 0, sizeof(g_objMenuOsd.FileFix));
								g_objMenuOsd.FilePageIndex = g_objMenuOsd.FilePageNum;
		
								int i, j;
								for(i = 0; i < 7; i++)
								{
									j = 7 * (g_objMenuOsd.FilePageIndex - 1) + i;	
		
									if(j < g_objMenuOsd.FileNum)
										g_objMenuOsd.FileIndex[i] = j;
									else
										break;
		
									sprintf(
														pCurMenuPage->MenuItem[i].Content, 
														" %s   %s-%s-%s   %s:%s:%s    %s:%s:%s\n", 
														g_objMenuOsd.AVFileIndex[j].EncChn, 
														g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2], 
														g_objMenuOsd.AVFileIndex[j].StartTime[0], g_objMenuOsd.AVFileIndex[j].StartTime[1], g_objMenuOsd.AVFileIndex[j].StartTime[2],
														g_objMenuOsd.AVFileIndex[j].EndTime[0], g_objMenuOsd.AVFileIndex[j].EndTime[1], g_objMenuOsd.AVFileIndex[j].EndTime[2]
													 );
									pCurMenuPage->MenuItem[i].Exist = TRUE;
								}		
								sprintf(pCurMenuPage->MenuItem[12].Content, "%4d /%4d\n", g_objMenuOsd.FilePageIndex, g_objMenuOsd.FilePageNum);
		
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);	
									
								g_bytCItemID = 0x08;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
		
						case 0x0b:/*�ط�*/
						{
							if( !pCurMenuItem->Fix )
							{
								int i, j;
								tag2DVOPar	objVOutput;
		
								for(i = 0; i < 7; i++)
								{
									if(g_objMenuOsd.FileFix[i])
									{
										j = g_objMenuOsd.FileIndex[i];
										g_objMvrCfg.AVPlayback.DecNum = 1;
										g_objMvrCfg.AVPlayback.VStart[0] = TRUE;
										g_objMvrCfg.AVPlayback.VFramerate[0] = atoi(g_objMenuOsd.AVFileIndex[j].Framerate);
										strcpy(g_objMvrCfg.AVPlayback.EncChn[0], g_objMenuOsd.AVFileIndex[j].EncChn);
										sprintf(g_objMvrCfg.AVPlayback.CreatDate, "%s-%s-%s", g_objMenuOsd.AVFileIndex[j].CreatDate[0], g_objMenuOsd.AVFileIndex[j].CreatDate[1], g_objMenuOsd.AVFileIndex[j].CreatDate[2]);
										sprintf(g_objMvrCfg.AVPlayback.VFilePath[0], "/mnt/%s/part1/data/%s", g_objMenuOsd.DiskType, g_objMenuOsd.AVFileIndex[j].VPath);
										sprintf(g_objMvrCfg.AVPlayback.IFilePath[0], "/mnt/%s/part1/data/%s", g_objMenuOsd.DiskType, g_objMenuOsd.AVFileIndex[j].IPath);
												
										if(strlen(g_objMenuOsd.AVFileIndex[j].APath) > 0)
										{
											g_objMvrCfg.AVPlayback.AStart[0] = TRUE;
											sprintf(g_objMvrCfg.AVPlayback.AFilePath[0], "/mnt/%s/part1/data/%s", g_objMenuOsd.DiskType, g_objMenuOsd.AVFileIndex[j].APath);
										}
										
										objVOutput.VNormal = PAL;
										objVOutput.VMode = D1;
										
										HideRegion(pCurMenuPage->Region);
										pCurMenuItem->Fix = TRUE;
														
										SwitchVo(objVOutput);
										StartAVPlay();
										break;
									}
								}
							}
						}
							break;
		
						default:
							break;
					}
			}
				break;
			case 0x0d:/*������ʽ����*/
			{
					switch( g_bytCItemID )
					{
						case 0x00:/*������ʽѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								switch(g_objMenuOsd.TrigType)
								{
									case TRIG_PER:
										g_objMenuOsd.TrigType = TRIG_EVE;
										FlushItem(pCurMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"��  ��  ��  ��");
										break;
									case TRIG_EVE:
										g_objMenuOsd.TrigType = TRIG_PER_AND_EVE;
										FlushItem(pCurMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ʱ�����¼�����");
										break;
									case TRIG_PER_AND_EVE:
										g_objMenuOsd.TrigType = TRIG_PER_OR_EVE;
										FlushItem(pCurMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ʱ�λ��¼�����");
										break;
									case TRIG_PER_OR_EVE:
										g_objMenuOsd.TrigType = TRIG_PER;
										FlushItem(pCurMenuItem->Content, 14, NULL, 0, "%s", (DWORD)"ʱ  ��  ��  ��");
										break;
									default:
										break;
								}
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x01:/*�洢����ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if(strcmp("SDisk", g_objMenuOsd.DiskType) == 0)
								{
									strcpy(g_objMenuOsd.DiskType, "HDisk");
									FlushItem(pCurMenuItem->Content, 14, " ", 4, "%s", (DWORD)"Ӳ  ��");
								}
								else if(strcmp("HDisk", g_objMenuOsd.DiskType) == 0)
								{
									strcpy(g_objMenuOsd.DiskType, "SDisk");
									FlushItem(pCurMenuItem->Content, 14, " ", 4, "%s", (DWORD)"SD  ��");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						
						case 0x02:/*ȷ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
											
								tag2DDvrCfg objMvrCfg;
		            GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								objMvrCfg.TrigPara.TrigType = g_objMenuOsd.TrigType;
								strcpy(objMvrCfg.AVRecord.DiskType, g_objMenuOsd.DiskType);
		
								int iRet = SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								if(iRet==0)
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
								}
								else
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x0e:/*¼������*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*��Ƶ��ʽ*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								if(g_objMenuOsd.ViNormal==PAL)
								{
									g_objMenuOsd.ViNormal = NTSC;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"NTSC");
								}
								else
								{
									g_objMenuOsd.ViNormal = PAL;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"PAL");
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanSwitchNormal() )
								{
									if(g_objMenuOsd.ViNormal==PAL)
									{
										g_objMenuOsd.ViNormal = NTSC;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"NTSC");
									}
									else
									{
										g_objMenuOsd.ViNormal = PAL;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"PAL");
									}
								}
#endif
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x01:/*0ͨ���ֱ���*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								switch(g_objMenuOsd.VRecMode[0])
								{
									case CIF:
										g_objMenuOsd.VRecMode[0] = HD1;
										FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
										break;
									case HD1:
										g_objMenuOsd.VRecMode[0] = D1;
										FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
										break;
									case D1:
										g_objMenuOsd.VRecMode[0] = CIF;
										FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
										break;
									default:
										break;
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanSwitchMode(0) )
								{
									switch(g_objMenuOsd.VRecMode[0])
									{
										case CIF:
											g_objMenuOsd.VRecMode[0] = HD1;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
											break;
										case HD1:
											g_objMenuOsd.VRecMode[0] = D1;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
											break;
										case D1:
											g_objMenuOsd.VRecMode[0] = CIF;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
											break;
										default:
											break;
									}
								}
#endif
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
						
						case 0x02:/*0ͨ������*/
						{
							if( !pCurMenuItem->Fix )
							{
								switch( g_objMenuOsd.VRecBps[0] )
								{
									case BITRATE_REC_1:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_2;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"2");
										break;
									case BITRATE_REC_2:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_3;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"3");
										break;
									case BITRATE_REC_3:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_4;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"4");
										break;
									case BITRATE_REC_4:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_5;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"5");
										break;
									case BITRATE_REC_5:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_6;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"6");
										break;
									case BITRATE_REC_6:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_7;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"7");
										break;
									case BITRATE_REC_7:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_8;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"8");
										break;
									case BITRATE_REC_8:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_9;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"9");
										break;
									case BITRATE_REC_9:
										g_objMenuOsd.VRecBps[0] = BITRATE_REC_1;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"1");
										break;
									default:
										break;
								}
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;	
							
						case 0x03:/*0ͨ��֡��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.VRecFps[0], 0, sizeof(g_objMenuOsd.VRecFps[0]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
#if VEHICLE_TYPE == VEHICLE_M
								if(atoi(g_objMenuOsd.VRecFps[0])<1 || atoi(g_objMenuOsd.VRecFps[0])>(g_objMenuOsd.ViNormal==PAL ? 20 : 25))
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if(CanSwitchFrame(0)==false || atoi(g_objMenuOsd.VRecFps[0])<1 || atoi(g_objMenuOsd.VRecFps[0])>(g_objMenuOsd.ViNormal==PAL ? 25 : 30))
#endif
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;	

						case 0x04:/*0ͨ��¼��*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								if( g_objMenuOsd.VRecEn[0] )
								{
									g_objMenuOsd.VRecEn[0] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.VRecEn[0] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanEnableRecord(0) )
								{
									if( g_objMenuOsd.VRecEn[0] )
									{
										g_objMenuOsd.VRecEn[0] = FALSE;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
									}
									else
									{
										g_objMenuOsd.VRecEn[0] = TRUE;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
									}
								}
#endif
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x05:/*0ͨ��Ԥ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.VPrevEn[0] )
								{
									g_objMenuOsd.VPrevEn[0] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.VPrevEn[0] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;	
							
						case 0x06:/*1ͨ���ֱ���*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								switch(g_objMenuOsd.VRecMode[1])
								{
									case CIF:
										g_objMenuOsd.VRecMode[1] = HD1;
										FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
										break;
									case HD1:
										g_objMenuOsd.VRecMode[1] = CIF;
										FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
										break;
									default:
										break;
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanSwitchMode(1) )
								{
									switch(g_objMenuOsd.VRecMode[1])
									{
										case CIF:
											g_objMenuOsd.VRecMode[1] = HD1;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
											break;
										case HD1:
											g_objMenuOsd.VRecMode[1] = D1;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
											break;
										case D1:
											g_objMenuOsd.VRecMode[1] = CIF;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
											break;
										default:
											break;
									}
								}
#endif
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x07:/*1ͨ������*/
						{
							if( !pCurMenuItem->Fix )
							{
								switch( g_objMenuOsd.VRecBps[1] )
								{
									case BITRATE_REC_1:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_2;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"2");
										break;
									case BITRATE_REC_2:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_3;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"3");
										break;
									case BITRATE_REC_3:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_4;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"4");
										break;
									case BITRATE_REC_4:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_5;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"5");
										break;
									case BITRATE_REC_5:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_6;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"6");
										break;
									case BITRATE_REC_6:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_7;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"7");
										break;
									case BITRATE_REC_7:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_8;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"8");
										break;
									case BITRATE_REC_8:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_9;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"9");
										break;
									case BITRATE_REC_9:
										g_objMenuOsd.VRecBps[1] = BITRATE_REC_1;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"1");
										break;
									default:
										break;
								}
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
						
						case 0x08:/*1ͨ��֡��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.VRecFps[1], 0, sizeof(g_objMenuOsd.VRecFps[1]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
#if VEHICLE_TYPE == VEHICLE_M
								if(atoi(g_objMenuOsd.VRecFps[1]) < 1 || atoi(g_objMenuOsd.VRecFps[1]) > (g_objMenuOsd.ViNormal == PAL ? 20 : 25))
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if(CanSwitchFrame(1)==false || atoi(g_objMenuOsd.VRecFps[1]) < 1 || atoi(g_objMenuOsd.VRecFps[1]) > (g_objMenuOsd.ViNormal == PAL ? 25 : 30))
#endif
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;	

						case 0x09:/*1ͨ��¼��*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								if( g_objMenuOsd.VRecEn[1] )
								{
									g_objMenuOsd.VRecEn[1] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.VRecEn[1] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanEnableRecord(1) )
								{
									if( g_objMenuOsd.VRecEn[1] )
									{
										g_objMenuOsd.VRecEn[1] = FALSE;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
									}
									else
									{
										g_objMenuOsd.VRecEn[1] = TRUE;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
									}
								}
#endif
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0a:/*1ͨ��Ԥ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.VPrevEn[1] )
								{
									g_objMenuOsd.VPrevEn[1] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.VPrevEn[1] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
					
						case 0x0b:/*2ͨ���ֱ���*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								if(g_objMenuOsd.VRecMode[2] == CIF)
								{
									g_objMenuOsd.VRecMode[2] = CIF;
									FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanSwitchMode(2) )
								{
									switch(g_objMenuOsd.VRecMode[2])
									{
										case CIF:
											g_objMenuOsd.VRecMode[2] = HD1;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
											break;
										case HD1:
											g_objMenuOsd.VRecMode[2] = D1;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
											break;
										case D1:
											g_objMenuOsd.VRecMode[2] = CIF;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
											break;
										default:
											break;
									}
								}
#endif
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
					
						case 0x0c:/*2ͨ������*/
						{
							if( !pCurMenuItem->Fix )
							{
								switch(g_objMenuOsd.VRecBps[2])
								{
									case BITRATE_REC_1:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_2;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"2");
										break;
									case BITRATE_REC_2:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_3;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"3");
										break;
									case BITRATE_REC_3:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_4;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"4");
										break;
									case BITRATE_REC_4:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_5;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"5");
										break;
									case BITRATE_REC_5:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_6;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"6");
										break;
									case BITRATE_REC_6:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_7;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"7");
										break;
									case BITRATE_REC_7:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_8;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"8");
										break;
									case BITRATE_REC_8:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_9;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"9");
										break;
									case BITRATE_REC_9:
										g_objMenuOsd.VRecBps[2] = BITRATE_REC_1;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"1");
										break;
									default:
										break;
								}
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0d:/*2ͨ��֡��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.VRecFps[2], 0, sizeof(g_objMenuOsd.VRecFps[2]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
#if VEHICLE_TYPE == VEHICLE_M
								if(atoi(g_objMenuOsd.VRecFps[2]) < 1 || atoi(g_objMenuOsd.VRecFps[2]) > (g_objMenuOsd.ViNormal == PAL ? 20 : 25))
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if(CanSwitchFrame(2)==false || atoi(g_objMenuOsd.VRecFps[2]) < 1 || atoi(g_objMenuOsd.VRecFps[2]) > (g_objMenuOsd.ViNormal == PAL ? 25 : 30))
#endif
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;	

						case 0x0e:/*2ͨ��¼��*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								if( g_objMenuOsd.VRecEn[2] )
								{
									g_objMenuOsd.VRecEn[2] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.VRecEn[2] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanEnableRecord(2) )
								{
									if( g_objMenuOsd.VRecEn[2] )
									{
										g_objMenuOsd.VRecEn[2] = FALSE;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
									}
									else
									{
										g_objMenuOsd.VRecEn[2] = TRUE;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
									}
								}
#endif
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0f:/*2ͨ��Ԥ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.VPrevEn[2] )
								{
									g_objMenuOsd.VPrevEn[2] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.VPrevEn[2] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x10:/*3ͨ���ֱ���*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								if(g_objMenuOsd.VRecMode[3] == CIF)
								{
									g_objMenuOsd.VRecMode[3] = CIF;
									FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanSwitchMode(3) )
								{
									switch(g_objMenuOsd.VRecMode[3])
									{
										case CIF:
											g_objMenuOsd.VRecMode[3] = HD1;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"HD1");
											break;
										case HD1:
											g_objMenuOsd.VRecMode[3] = D1;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"D1");
											break;
										case D1:
											g_objMenuOsd.VRecMode[3] = CIF;
											FlushItem(pCurMenuItem->Content, 6, " ", 2, "%s", (DWORD)"CIF");
											break;
										default:
											break;
									}
								}
#endif
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x11:/*3ͨ������*/
						{
							if( !pCurMenuItem->Fix )
							{
								switch(g_objMenuOsd.VRecBps[3])
								{
									case BITRATE_REC_1:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_2;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"2");
										break;
									case BITRATE_REC_2:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_3;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"3");
										break;
									case BITRATE_REC_3:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_4;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"4");
										break;
									case BITRATE_REC_4:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_5;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"5");
										break;
									case BITRATE_REC_5:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_6;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"6");
										break;
									case BITRATE_REC_6:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_7;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"7");
										break;
									case BITRATE_REC_7:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_8;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"8");
										break;
									case BITRATE_REC_8:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_9;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"9");
										break;
									case BITRATE_REC_9:
										g_objMenuOsd.VRecBps[3] = BITRATE_REC_1;
										FlushItem(pCurMenuItem->Content, 4, " ", 2, "%s", (DWORD)"1");
										break;
									default:
										break;
								}
								
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x12:/*3ͨ��֡��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.VRecFps[3], 0, sizeof(g_objMenuOsd.VRecFps[3]));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
#if VEHICLE_TYPE == VEHICLE_M
								if(atoi(g_objMenuOsd.VRecFps[3]) < 1 || atoi(g_objMenuOsd.VRecFps[3]) > (g_objMenuOsd.ViNormal == PAL ? 20 : 25))
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if(CanSwitchFrame(3)==false || atoi(g_objMenuOsd.VRecFps[3]) < 1 || atoi(g_objMenuOsd.VRecFps[3]) > (g_objMenuOsd.ViNormal == PAL ? 25 : 30))
#endif
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;	

						case 0x13:/*3ͨ��¼��*/
						{
							if( !pCurMenuItem->Fix )
							{
#if VEHICLE_TYPE == VEHICLE_M
								if( g_objMenuOsd.VRecEn[3] )
								{
									g_objMenuOsd.VRecEn[3] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.VRecEn[3] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
#endif
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
								if( CanEnableRecord(3) )
								{
									if( g_objMenuOsd.VRecEn[3] )
									{
										g_objMenuOsd.VRecEn[3] = FALSE;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
									}
									else
									{
										g_objMenuOsd.VRecEn[3] = TRUE;
										FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
									}
								}
#endif
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x14:/*3ͨ��Ԥ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.VPrevEn[3] )
								{
									g_objMenuOsd.VPrevEn[3] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.VPrevEn[3] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;

						case 0x15:/*ȷ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								
								tag2DDvrCfg objMvrCfg;
		            GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								
								if(g_objMenuOsd.ViNormal==PAL)
									objMvrCfg.VInput.VNormal = objMvrCfg.VOutput.VNormal = PAL;
								else
									objMvrCfg.VInput.VNormal = objMvrCfg.VOutput.VNormal = NTSC;

#if VEHICLE_TYPE == VEHICLE_M
								objMvrCfg.VInput.VMode = objMvrCfg.VOutput.VMode = objMvrCfg.AVRecord.VMode = g_objMenuOsd.VRecMode[0];
#endif

								for(i = 0; i < 4; i++)
								{
#if VEHICLE_TYPE == VEHICLE_V8 || VEHICLE_TYPE == VEHICLE_M2
									objMvrCfg.AVRecord.VMode[i] = g_objMenuOsd.VRecMode[i];
#endif
									objMvrCfg.VPreview.VStart[i] = g_objMenuOsd.VPrevEn[i];
									objMvrCfg.AVRecord.VStart[i] = g_objMenuOsd.VRecEn[i];
									objMvrCfg.AVRecord.VBitrate[i] = g_objMenuOsd.VRecBps[i];
									objMvrCfg.AVRecord.VFramerate[i] = atoi(g_objMenuOsd.VRecFps[i]);
								}
		
								int iRet = SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								if(iRet==0)
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
								}
								else
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x0f:/*ʱ������*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*0ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSHour[0], 0, sizeof(g_objMenuOsd.PeriodSHour[0]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSHour[0]) != 2 || atoi(g_objMenuOsd.PeriodSHour[0]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x02:/*0ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSMin[0], 0, sizeof(g_objMenuOsd.PeriodSMin[0]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSMin[0]) != 2 || atoi(g_objMenuOsd.PeriodSMin[0]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x04:/*0ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSSec[0], 0, sizeof(g_objMenuOsd.PeriodSSec[0]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSSec[0]) != 2 || atoi(g_objMenuOsd.PeriodSSec[0]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x05:/*0ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEHour[0], 0, sizeof(g_objMenuOsd.PeriodEHour[0]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else 
							{
								if(strlen(g_objMenuOsd.PeriodEHour[0]) != 2 || atoi(g_objMenuOsd.PeriodEHour[0]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x07:/*0ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEMin[0], 0, sizeof(g_objMenuOsd.PeriodEMin[0]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEMin[0]) != 2 || atoi(g_objMenuOsd.PeriodEMin[0]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x09:/*0ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodESec[0], 0, sizeof(g_objMenuOsd.PeriodESec[0]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodESec[0]) != 2 || atoi(g_objMenuOsd.PeriodESec[0]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0a:/*0ͨ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.PeriodEn[0] )
								{
									g_objMenuOsd.PeriodEn[0] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.PeriodEn[0] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0b:/*1ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSHour[1], 0, sizeof(g_objMenuOsd.PeriodSHour[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSHour[1]) != 2 || atoi(g_objMenuOsd.PeriodSHour[1]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0d:/*1ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSMin[1], 0, sizeof(g_objMenuOsd.PeriodSMin[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSMin[1]) != 2 || atoi(g_objMenuOsd.PeriodSMin[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0f:/*1ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSSec[1], 0, sizeof(g_objMenuOsd.PeriodSSec[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSSec[1]) != 2 || atoi(g_objMenuOsd.PeriodSSec[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x10:/*1ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEHour[1], 0, sizeof(g_objMenuOsd.PeriodEHour[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEHour[1]) != 2 || atoi(g_objMenuOsd.PeriodEHour[1]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x12:/*1ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEMin[1], 0, sizeof(g_objMenuOsd.PeriodEMin[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEMin[1]) != 2 || atoi(g_objMenuOsd.PeriodEMin[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x14:/*1ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodESec[1], 0, sizeof(g_objMenuOsd.PeriodESec[1]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodESec[1]) != 2 || atoi(g_objMenuOsd.PeriodESec[1]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x15:/*1ͨ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.PeriodEn[1] )
								{
									g_objMenuOsd.PeriodEn[1] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.PeriodEn[1] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
						
						case 0x16:/*2ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSHour[2], 0, sizeof(g_objMenuOsd.PeriodSHour[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSHour[2]) != 2 || atoi(g_objMenuOsd.PeriodSHour[2]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x18:/*2ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSMin[2], 0, sizeof(g_objMenuOsd.PeriodSMin[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSMin[2]) != 2 || atoi(g_objMenuOsd.PeriodSMin[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x1a:/*2ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSSec[2], 0, sizeof(g_objMenuOsd.PeriodSSec[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSSec[2]) != 2 || atoi(g_objMenuOsd.PeriodSSec[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x1b:/*2ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEHour[2], 0, sizeof(g_objMenuOsd.PeriodEHour[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEHour[2]) != 2 || atoi(g_objMenuOsd.PeriodEHour[2]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x1d:/*2ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEMin[2], 0, sizeof(g_objMenuOsd.PeriodEMin[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEMin[2]) != 2 || atoi(g_objMenuOsd.PeriodEMin[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x1f:/*2ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodESec[2], 0, sizeof(g_objMenuOsd.PeriodESec[2]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodESec[2]) != 2 || atoi(g_objMenuOsd.PeriodESec[2]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x20:/*2ͨ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.PeriodEn[2] )
								{
									g_objMenuOsd.PeriodEn[2] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.PeriodEn[2] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x21:/*3ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSHour[3], 0, sizeof(g_objMenuOsd.PeriodSHour[3]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSHour[3]) != 2 || atoi(g_objMenuOsd.PeriodSHour[3]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x23:/*3ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSMin[3], 0, sizeof(g_objMenuOsd.PeriodSMin[3]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSMin[3]) != 2 || atoi(g_objMenuOsd.PeriodSMin[3]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x25:/*3ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSSec[3], 0, sizeof(g_objMenuOsd.PeriodSSec[3]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSSec[3]) != 2 || atoi(g_objMenuOsd.PeriodSSec[3]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x26:/*3ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEHour[3], 0, sizeof(g_objMenuOsd.PeriodEHour[3]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEHour[3]) != 2 || atoi(g_objMenuOsd.PeriodEHour[3]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x28:/*3ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEMin[3], 0, sizeof(g_objMenuOsd.PeriodEMin[3]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEMin[3]) != 2 || atoi(g_objMenuOsd.PeriodEMin[3]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x2a:/*3ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodESec[3], 0, sizeof(g_objMenuOsd.PeriodESec[3]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodESec[3]) != 2 || atoi(g_objMenuOsd.PeriodESec[3]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x2b:/*3ͨ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.PeriodEn[3] )
								{
									g_objMenuOsd.PeriodEn[3] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.PeriodEn[3] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x2c:/*4ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSHour[4], 0, sizeof(g_objMenuOsd.PeriodSHour[4]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSHour[4]) != 2 || atoi(g_objMenuOsd.PeriodSHour[4]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x2e:/*4ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSMin[4], 0, sizeof(g_objMenuOsd.PeriodSMin[4]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSMin[4]) != 2 || atoi(g_objMenuOsd.PeriodSMin[4]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x30:/*4ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSSec[4], 0, sizeof(g_objMenuOsd.PeriodSSec[4]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSSec[4]) != 2 || atoi(g_objMenuOsd.PeriodSSec[4]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x31:/*4ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEHour[4], 0, sizeof(g_objMenuOsd.PeriodEHour[4]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEHour[4]) != 2 || atoi(g_objMenuOsd.PeriodEHour[4]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x33:/*4ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEMin[4], 0, sizeof(g_objMenuOsd.PeriodEMin[4]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEMin[4]) != 2 || atoi(g_objMenuOsd.PeriodEMin[4]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x35:/*4ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodESec[4], 0, sizeof(g_objMenuOsd.PeriodESec[4]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodESec[4]) != 2 || atoi(g_objMenuOsd.PeriodESec[4]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x36:/*4ͨ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.PeriodEn[4] )
								{
									g_objMenuOsd.PeriodEn[4] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else 
								{
									g_objMenuOsd.PeriodEn[4] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x37:/*5ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSHour[5], 0, sizeof(g_objMenuOsd.PeriodSHour[5]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else 
							{
								if(strlen(g_objMenuOsd.PeriodSHour[5]) != 2 || atoi(g_objMenuOsd.PeriodSHour[5]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x39:/*5ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSMin[5], 0, sizeof(g_objMenuOsd.PeriodSMin[5]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else 
							{
								if(strlen(g_objMenuOsd.PeriodSMin[5]) != 2 || atoi(g_objMenuOsd.PeriodSMin[5]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x3b:/*5ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodSSec[5], 0, sizeof(g_objMenuOsd.PeriodSSec[5]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodSSec[5]) != 2 || atoi(g_objMenuOsd.PeriodSSec[5]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x3c:/*5ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEHour[5], 0, sizeof(g_objMenuOsd.PeriodEHour[5]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEHour[5]) != 2 || atoi(g_objMenuOsd.PeriodEHour[5]) > 23)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x3e:/*5ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodEMin[5], 0, sizeof(g_objMenuOsd.PeriodEMin[5]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodEMin[5]) != 2 || atoi(g_objMenuOsd.PeriodEMin[5]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x40:/*5ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.PeriodESec[5], 0, sizeof(g_objMenuOsd.PeriodESec[5]));
								FlushItem(pCurMenuItem->Content, 2, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.PeriodESec[5]) != 2 || atoi(g_objMenuOsd.PeriodESec[5]) > 59)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x41:/*5ͨ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if(g_objMenuOsd.PeriodEn[5] == TRUE)
								{
									g_objMenuOsd.PeriodEn[5] = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.PeriodEn[5] = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
						
						case 0x42:/*ȷ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								
								tag2DDvrCfg objMvrCfg;
		            GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								
								for(i = 0; i < 6; i++)
								{
									objMvrCfg.PeriodTrig.PeriodStart[i] = g_objMenuOsd.PeriodEn[i];
									sprintf( objMvrCfg.PeriodTrig.StartTime[i], "%s:%s:%s", g_objMenuOsd.PeriodSHour[i], g_objMenuOsd.PeriodSMin[i], g_objMenuOsd.PeriodSSec[i]);
									sprintf( objMvrCfg.PeriodTrig.EndTime[i], "%s:%s:%s", g_objMenuOsd.PeriodEHour[i], g_objMenuOsd.PeriodEMin[i], g_objMenuOsd.PeriodESec[i]);
								}
		
								int iRet = SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								if(iRet==0)
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
								}
								else
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x10:/*�¼�����*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*����¼���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.AccDelay, 0, sizeof(g_objMenuOsd.AccDelay));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.AccDelay) < 1 || atoi(g_objMenuOsd.AccDelay) > 60)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x01:/*����¼�ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.AccEn )
								{
									g_objMenuOsd.AccEn = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.AccEn = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x02:/*�˶��¼���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.MoveDelay, 0, sizeof(g_objMenuOsd.MoveDelay));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.MoveDelay) < 1 || atoi(g_objMenuOsd.MoveDelay) > 60)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x03:/*�˶��¼�ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.MoveEn )
								{
									g_objMenuOsd.MoveEn = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.MoveEn = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x04:/*�ڵ��¼���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.OverlayDelay, 0, sizeof(g_objMenuOsd.OverlayDelay));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.OverlayDelay) < 1 || atoi(g_objMenuOsd.OverlayDelay) > 60)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x05:/*�ڵ��¼�ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.OverlayEn )
								{
									g_objMenuOsd.OverlayEn = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.OverlayEn = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x06:/*S0��ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.S1Delay, 0, sizeof(g_objMenuOsd.S1Delay));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.S1Delay) < 1 || atoi(g_objMenuOsd.S1Delay) > 60)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
		
						case 0x07:/*S0ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.S1En )
								{
									g_objMenuOsd.S1En = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.S1En = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
						
						case 0x08:/*S1��ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.S2Delay, 0, sizeof(g_objMenuOsd.S2Delay));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.S2Delay) < 1 || atoi(g_objMenuOsd.S2Delay) > 60)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
		
						case 0x09:/*S1ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.S2En )
								{
									g_objMenuOsd.S2En = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.S2En = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0a:/*S2��ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.S3Delay, 0, sizeof(g_objMenuOsd.S3Delay));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.S3Delay) < 1 || atoi(g_objMenuOsd.S3Delay) > 60)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
		
						case 0x0b:/*S2ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.S3En )
								{
									g_objMenuOsd.S3En = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.S3En = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x0c:/*S3��ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.S4Delay, 0, sizeof(g_objMenuOsd.S4Delay));
								FlushItem(pCurMenuItem->Content, 4, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.S4Delay) < 1 || atoi(g_objMenuOsd.S4Delay) > 60)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
		
						case 0x0d:/*S3ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if( g_objMenuOsd.S4En )
								{
									g_objMenuOsd.S4En = FALSE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"�ر�");
								}
								else
								{
									g_objMenuOsd.S4En = TRUE;
									FlushItem(pCurMenuItem->Content, 4, NULL, 0, "%s", (DWORD)"����");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
						
						case 0x0e:/*ȷ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
											
								tag2DDvrCfg objMvrCfg;
		            GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								
								objMvrCfg.EventTrig.AccDelay = atoi(g_objMenuOsd.AccDelay);
								objMvrCfg.EventTrig.AccStart = g_objMenuOsd.AccEn;
								objMvrCfg.EventTrig.MoveDelay = atoi(g_objMenuOsd.MoveDelay);
								objMvrCfg.EventTrig.MoveStart = g_objMenuOsd.MoveEn;
								objMvrCfg.EventTrig.OverlayDelay = atoi(g_objMenuOsd.OverlayDelay);
								objMvrCfg.EventTrig.OverlayStart = g_objMenuOsd.OverlayEn;
								objMvrCfg.EventTrig.S1Delay = atoi(g_objMenuOsd.S1Delay);
								objMvrCfg.EventTrig.S1Start = g_objMenuOsd.S1En;
								objMvrCfg.EventTrig.S2Delay = atoi(g_objMenuOsd.S2Delay);
								objMvrCfg.EventTrig.S2Start = g_objMenuOsd.S2En;
								objMvrCfg.EventTrig.S3Delay = atoi(g_objMenuOsd.S3Delay);
								objMvrCfg.EventTrig.S3Start = g_objMenuOsd.S3En;
								objMvrCfg.EventTrig.S4Delay = atoi(g_objMenuOsd.S4Delay);
								objMvrCfg.EventTrig.S4Start = g_objMenuOsd.S4En;
		
								int iRet = SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								if(iRet==0)
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
								}
								else
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
							}
						}
							break;
							
						default:
							break;
					}
			}
				break;
			case 0x11:/*��������*/
			{
					switch(g_bytCItemID)
					{
						case 0x00:/*�ֻ�ģ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								if(strcmp("LC6311", g_objMenuOsd.m_szPhoneMod) == 0)
								{
									strcpy(g_objMenuOsd.m_szPhoneMod, "MC703OLD");
									FlushItem(pCurMenuItem->Content, 19, NULL, 0, "%s", (DWORD)"MC703OLD");
								}
								else if(strcmp("MC703OLD", g_objMenuOsd.m_szPhoneMod) == 0)
								{
									strcpy(g_objMenuOsd.m_szPhoneMod, "MC703NEW");
									FlushItem(pCurMenuItem->Content, 19, NULL, 0, "%s", (DWORD)"MC703NEW");
								}
								else if(strcmp("MC703NEW", g_objMenuOsd.m_szPhoneMod) == 0)
								{
									strcpy(g_objMenuOsd.m_szPhoneMod, "SIM5218");
									FlushItem(pCurMenuItem->Content, 19, NULL, 0, "%s", (DWORD)"SIM5218");
								}
								else if(strcmp("SIM5218", g_objMenuOsd.m_szPhoneMod) == 0)
								{
									strcpy(g_objMenuOsd.m_szPhoneMod, "LC6311");
									FlushItem(pCurMenuItem->Content, 19, NULL, 0, "%s", (DWORD)"LC6311");
								}
														
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						case 0x01:/*��������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_szTelNum, 0, sizeof(g_objMenuOsd.m_szTelNum));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_szTelNum) < 1 || strlen(g_objMenuOsd.m_szTelNum) > 11)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
						
						case 0x02:/*APN*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_szApn, 0, sizeof(g_objMenuOsd.m_szApn));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_szApn) < 1 || strlen(g_objMenuOsd.m_szApn) > 19)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
						
						case 0x03:/*ͨ��IP*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_ulCommIP, 0, sizeof(g_objMenuOsd.m_ulCommIP));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_ulCommIP) < 7 || strlen(g_objMenuOsd.m_ulCommIP) > 15)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x04:/*ͨ�Ŷ˿�*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_usCommPort, 0, sizeof(g_objMenuOsd.m_usCommPort));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_usCommPort) < 1 || atoi(g_objMenuOsd.m_usCommPort) > 65535)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x05:/*��ƬIP*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_ulPhotoIP, 0, sizeof(g_objMenuOsd.m_ulPhotoIP));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_ulPhotoIP) < 7 || strlen(g_objMenuOsd.m_ulPhotoIP) > 15)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x06:/*��Ƭ�˿�*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_usPhotoPort, 0, sizeof(g_objMenuOsd.m_usPhotoPort));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_usPhotoPort) < 1 || atoi(g_objMenuOsd.m_usPhotoPort) > 65535)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x07:/*��ƵIP*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_ulVideoIP, 0, sizeof(g_objMenuOsd.m_ulVideoIP));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_ulVideoIP) < 7 || strlen(g_objMenuOsd.m_ulVideoIP) > 15)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x08:/*��Ƶ�˿�*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_usVideoPort, 0, sizeof(g_objMenuOsd.m_usVideoPort));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_usVideoPort) < 1 || atoi(g_objMenuOsd.m_usVideoPort) > 65535)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x09:/*����IP*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_ulUpdateIP, 0, sizeof(g_objMenuOsd.m_ulUpdateIP));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_ulUpdateIP) < 7 || strlen(g_objMenuOsd.m_ulUpdateIP) > 15)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
							
						case 0x0a:/*�����˿�*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(g_objMenuOsd.m_usUpdatePort, 0, sizeof(g_objMenuOsd.m_usUpdatePort));
								FlushItem(pCurMenuItem->Content, 19, NULL, 0, NULL, 0);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								pCurMenuItem->Fix = TRUE;
							}
							else
							{
								if(strlen(g_objMenuOsd.m_usUpdatePort) < 1 || atoi(g_objMenuOsd.m_usUpdatePort) > 65535)
									InitItem(pCurMenuItem);
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								pCurMenuItem->Fix = FALSE;
							}
						}
							break;
		
						case 0x0b:/*ȷ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								pCurMenuItem->Fix = TRUE;
								//����Dvr����
								tag2DDvrCfg objMvrCfg;
		            GetSecCfg(&objMvrCfg, sizeof(objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg));
								strcpy(objMvrCfg.IpPortPara.m_szPhoneMod, g_objMenuOsd.m_szPhoneMod);
								strcpy(objMvrCfg.IpPortPara.m_szTelNum, g_objMenuOsd.m_szTelNum);
								strcpy(objMvrCfg.IpPortPara.m_szApn, g_objMenuOsd.m_szApn);
								objMvrCfg.IpPortPara.m_ulCommIP = inet_addr(g_objMenuOsd.m_ulCommIP);
								objMvrCfg.IpPortPara.m_ulPhotoIP = inet_addr(g_objMenuOsd.m_ulPhotoIP);
								objMvrCfg.IpPortPara.m_ulVideoIP = inet_addr(g_objMenuOsd.m_ulVideoIP);
								objMvrCfg.IpPortPara.m_ulUpdateIP = inet_addr(g_objMenuOsd.m_ulUpdateIP);
								objMvrCfg.IpPortPara.m_usCommPort = htons(atoi(g_objMenuOsd.m_usCommPort));
								objMvrCfg.IpPortPara.m_usPhotoPort = htons(atoi(g_objMenuOsd.m_usPhotoPort));
								objMvrCfg.IpPortPara.m_usVideoPort = htons(atoi(g_objMenuOsd.m_usVideoPort));
								objMvrCfg.IpPortPara.m_usUpdatePort = htons(atoi(g_objMenuOsd.m_usUpdatePort));
								//����Comu����
								tag1PComuCfg cfg0;
								GetImpCfg( &cfg0, sizeof(cfg0), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg0) );
								tag1QIpCfg cfg1[2];
								GetImpCfg( &cfg1, sizeof(cfg1), offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg1) );
								tag1LComuCfg cfg2;
								GetImpCfg( &cfg2, sizeof(cfg2), offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg2) );
								memset(cfg0.m_szTel, 0, sizeof(cfg0.m_szTel) );
								memcpy(cfg0.m_szTel, g_objMenuOsd.m_szTelNum, min(15,strlen(g_objMenuOsd.m_szTelNum)) );
								memset(cfg1[0].m_szApn, 0, sizeof(cfg1[0].m_szApn) );
								memcpy(cfg1[0].m_szApn, g_objMenuOsd.m_szApn, min(19,strlen(g_objMenuOsd.m_szApn)) );
								cfg1[0].m_ulQianTcpIP = inet_addr(g_objMenuOsd.m_ulCommIP);
								cfg1[0].m_ulQianUdpIP = inet_addr(g_objMenuOsd.m_ulPhotoIP);
								cfg1[0].m_ulVUdpIP = inet_addr(g_objMenuOsd.m_ulVideoIP);
								cfg2.m_ulLiuIP = inet_addr(g_objMenuOsd.m_ulUpdateIP);
								cfg1[0].m_usQianTcpPort = htons(atoi(g_objMenuOsd.m_usCommPort));
								cfg1[0].m_usQianUdpPort = htons(atoi(g_objMenuOsd.m_usPhotoPort));
								cfg1[0].m_usVUdpPort = htons(atoi(g_objMenuOsd.m_usVideoPort));
								cfg2.m_usLiuPort = htons(atoi(g_objMenuOsd.m_usUpdatePort));
									
								if( SetSecCfg(&objMvrCfg, offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(objMvrCfg))==0 
								 && SetImpCfg(&cfg0, offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(cfg0))==0 
								 && SetImpCfg(&cfg1, offsetof(tagImportantCfg, m_uni1QComuCfg.m_ary1QIpCfg), sizeof(cfg1))==0 
								 && SetImpCfg(&cfg2, offsetof(tagImportantCfg, m_uni1LComuCfg.m_obj1LComuCfg), sizeof(cfg2))==0 )
								{
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_SUCC);
									GetSecCfg(&g_objMvrCfg, sizeof(g_objMvrCfg), offsetof(tagSecondCfg, m_uni2DDvrCfg.m_obj2DDvrCfg), sizeof(g_objMvrCfg));
									
									Frm35 frm1;
									frm1.reply = 0x01;
									DataPush((char*)&frm1, sizeof(frm1), DEV_DIAODU, DEV_SOCK, 2);

									char buf[2] = {0x01, 0x03};
					 				DataPush(buf, 2, DEV_DIAODU, DEV_UPDATE, 2);
					 				
					 				//����Ƶ�ϴ��߳����¶�ȡ�ֻ���
									GetImpCfg( &g_objComuCfg, sizeof(g_objComuCfg), offsetof(tagImportantCfg, m_uni1PComuCfg.m_obj1PComuCfg), sizeof(g_objComuCfg) );
								}
								else
									ShowDialog(pCurMenuItem, DLG_LEVEL, DLG_ERR);
							}
						}
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

int DealKeyCancel()
{
		int i;
		MENU_PAGE *pCurMenuPage = GetPage();
		MENU_ITEM *pCurMenuItem = GetItem();	
		
		switch(g_bytCPageID)
		{
			case 0x00:/*ϵͳ��½*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*���������*/
						{
							if( pCurMenuItem->Fix )
							{
								if((i = strlen(g_objMenuOsd.LoginPassword)) > 0)
								{
									g_objMenuOsd.LoginPassword[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
		
						case 0x01:/*��½��*/
						{
							if( pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
		
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
		
								g_bytCItemID = 0x00;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
							
						default:
							break;
					}
				}
				break;
			case 0x01:
				{
					switch(g_bytCItemID)
					{
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
						case 0x04:
						case 0x05:
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								pCurMenuPage = GetPage();
								HidePage(pCurMenuPage);
								
								g_bytCPageID = 0x00;				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);

								g_bytCItemID = 0x00;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L0Page = 0x00;
								g_bytLItemID_L1Page = 0x00;
								g_bytLItemID_L2Page = 0x00;
								g_bytLItemID_L3Page = 0x00;
							}
						}
							break;
							
						default:
							break;
					}
				}
				break;
			case 0x02:
				{
					switch(g_bytCItemID)
					{
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
		
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
							
						default:
							break;
					}
				}
				break;
			case 0x03:/*�ļ���ѯ*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*ͨ��ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
						
						case 0x01:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.CreatDate[0])) > 0)
								{
									g_objMenuOsd.CreatDate[0][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x03:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.CreatDate[1])) > 0)
								{
									g_objMenuOsd.CreatDate[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x05:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.CreatDate[2])) > 0)
								{
									g_objMenuOsd.CreatDate[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x06:/*ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.StartTime[0])) > 0)
								{
									g_objMenuOsd.StartTime[0][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x08:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.StartTime[1])) > 0)
								{
									g_objMenuOsd.StartTime[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0a:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.StartTime[2])) > 0)
								{
									g_objMenuOsd.StartTime[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0b:/*ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.EndTime[0])) > 0)
								{
									g_objMenuOsd.EndTime[0][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0d:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.EndTime[1])) > 0)
								{
									g_objMenuOsd.EndTime[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0f:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.EndTime[2])) > 0)
								{
									g_objMenuOsd.EndTime[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x10:/*����ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
							
						case 0x11:/*��ѯ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
								
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
		
								g_bytCItemID = 0x00;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
					
						default:
							break;
					}
				}
				break;
			case 0x04:/*¼������*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*ͨ��ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
		
						case 0x01:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.CreatDate[0])) > 0)
								{
									g_objMenuOsd.CreatDate[0][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x03:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.CreatDate[1])) > 0)
								{
									g_objMenuOsd.CreatDate[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x05:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.CreatDate[2])) > 0)
								{
									g_objMenuOsd.CreatDate[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x06:/*ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.StartTime[0])) > 0)
								{
									g_objMenuOsd.StartTime[0][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x08:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.StartTime[1])) > 0)
								{
									g_objMenuOsd.StartTime[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0a:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.StartTime[2])) > 0)
								{
									g_objMenuOsd.StartTime[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0b:/*ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.EndTime[0])) > 0)
								{
									g_objMenuOsd.EndTime[0][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0d:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.EndTime[1])) > 0)
								{
									g_objMenuOsd.EndTime[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0f:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.EndTime[2])) > 0)
								{
									g_objMenuOsd.EndTime[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
						
						case 0x10:/*����ѡ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
							
						case 0x11:/*����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
							else
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
								
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
		
								g_bytCItemID = 0x00;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
							}
						}
							break;
					
						default:
							break;
					}
				}
				break;
			case 0x05:/*���̲���*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
		
						case 0x01:/*������ʽ��*/
						{
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
		
						default:
							break;
					}
				}
				break;
			case 0x06:/*��������*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:
						case 0x01:
						case 0x02:
						{
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
		
						default:
							break;
					}
				}
				break;
			case 0x07:/*ϵͳ��Ϣ*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
						case 0x04:
						case 0x05:
						case 0x06:
						case 0x07:
						case 0x08:
						case 0x09:
						case 0x0a:
						case 0x0b:
						case 0x0c:
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
								HidePage(pCurMenuPage);
							
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
								g_bytLItemID_L1Page = 0x00;
							}
						}
							break;
		
						default:
							break;
					}
				}
				break;
			case 0x08:/*��������*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
								HidePage(pCurMenuPage);
							
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.SystemDate[0])) > 0)
								{
									g_objMenuOsd.SystemDate[0][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x02:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
								HidePage(pCurMenuPage);
							
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.SystemDate[1])) > 0)
								{
									g_objMenuOsd.SystemDate[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x04:/*��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
								HidePage(pCurMenuPage);
							
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.SystemDate[2])) > 0)
								{
									g_objMenuOsd.SystemDate[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x05:/*ʱ*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.SystemTime[0])) > 0)
									{
										g_objMenuOsd.SystemTime[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x07:/*��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.SystemTime[1])) > 0)
									{
										g_objMenuOsd.SystemTime[1][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x09:/*��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.SystemTime[2])) > 0)
									{
										g_objMenuOsd.SystemTime[2][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x0a:/*˾�����*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.DriverID)) > 0)
									{
										g_objMenuOsd.DriverID[i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x0b:/*���ƺ���*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.CarNumber)) > 0)
									{
										g_objMenuOsd.CarNumber[i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
		
						case 0x0c:/*ȷ��*/
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							break;
					
						default:
							break;
					}
				}
				break;
			case 0x09:
				{
					switch(g_bytCItemID)
					{
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
			
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
							}
							break;
							
						default:
							break;
					}
				}
				break;
			case 0x0a:/*��������*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*��ǰ����*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.CurPassword)) > 0)
									{
										g_objMenuOsd.CurPassword[i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x01:/*������*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.NewPassword[0])) > 0)
									{
										g_objMenuOsd.NewPassword[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x02:/*ȷ��������*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.NewPassword[1])) > 0)
									{
										g_objMenuOsd.NewPassword[1][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
		
						case 0x03:/*ȷ��*/
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							break;
					
						default:
							break;
					}
				}
				break;
			case 0x0b:/*��ѯ���*/
				{
					switch(g_bytCItemID)
					{
						case 0x0b:
							{
								if( !pCurMenuItem->Fix )
								{
									free(g_objMenuOsd.AVFileIndex);
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									HideItem(pCurMenuItem);
									pCurMenuItem->Fix = FALSE;
									
									for(i = 0; i < 7; i++)
									{
										if( g_objMenuOsd.FileFix[i] )
										{
											g_objMenuOsd.FileFix[i] = FALSE;
											pCurMenuPage->MenuItem[i].BackColor = BLUE;
											ShowItem(pCurMenuPage, &(pCurMenuPage->MenuItem[i]), WHITE, BLUE);
										}
									}
								}
							}
							break;
					
						default:
							{
								free(g_objMenuOsd.AVFileIndex);
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								HidePage(pCurMenuPage);
							
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);

								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							break;
					}
				}
				break;
			case 0x0c:/*�������*/
				{
					switch(g_bytCItemID)
					{
						case 0x0b:
							{
								if( !pCurMenuItem->Fix )
								{
									free(g_objMenuOsd.AVFileIndex);
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = 0x00;
								}
								else
								{
									StopAVPlay();
									StopVo();
									StartVo();	
									ShowRegion(pCurMenuPage->Region);
									
									pCurMenuItem->Fix = FALSE;
								}
							}
							break;
					
						default:
							{
								free(g_objMenuOsd.AVFileIndex);
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								HidePage(pCurMenuPage);
							
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							break;
					}
				}
				break;
			case 0x0d:/*������ʽ����*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:
						case 0x01:
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
							}
							break;
							
						case 0x02:/*ȷ��*/
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							break;
					
						default:
							break;
					}
				}
				break;
			case 0x0e:/*ͨ������*/
				{
					switch(g_bytCItemID)
					{
						case 0x03:/*0ͨ��֡��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.VRecFps[0])) > 0)
									{
										g_objMenuOsd.VRecFps[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
		
						case 0x08:/*1ͨ��֡��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.VRecFps[1])) > 0)
									{
										g_objMenuOsd.VRecFps[1][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;

						case 0x0d:/*2ͨ��֡��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.VRecFps[2])) > 0)
									{
										g_objMenuOsd.VRecFps[2][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;

						case 0x12:/*3ͨ��֡��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.VRecFps[3])) > 0)
									{
										g_objMenuOsd.VRecFps[3][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;

						case 0x15:/*ȷ��*/
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							break;

						default:
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
							}
							break;
					}
				}
				break;
			case 0x0f:/*ʱ������*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*0ʱ�ο�ʼʱ*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.PeriodSHour[0])) > 0)
									{
										g_objMenuOsd.PeriodSHour[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x02:/*0ʱ�ο�ʼ��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.PeriodSMin[0])) > 0)
									{
										g_objMenuOsd.PeriodSMin[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x04:/*0ʱ�ο�ʼ��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.PeriodSSec[0])) > 0)
									{
										g_objMenuOsd.PeriodSSec[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x05:/*0ʱ�ν���ʱ*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.PeriodEHour[0])) > 0)
									{
										g_objMenuOsd.PeriodEHour[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x07:/*0ʱ�ν�����*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.PeriodEMin[0])) > 0)
									{
										g_objMenuOsd.PeriodEMin[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x09:/*0ʱ�ν�����*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
								else
								{
									if((i = strlen(g_objMenuOsd.PeriodESec[0])) > 0)
									{
										g_objMenuOsd.PeriodESec[0][i - 1] = 0;
										FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
										ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
									}
								}
							}
							break;
							
						case 0x0a:/*0ʱ��ʹ��*/
							{
								if( !pCurMenuItem->Fix )
								{
									memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
									
									HidePage(pCurMenuPage);
							
									g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
									pCurMenuPage = GetPage();
									InitPage(pCurMenuPage);
									ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
						
									g_bytCItemID = g_bytLItemID_L1Page;
									pCurMenuItem = GetItem();
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
									
									g_bytLItemID_L1Page = g_bytLItemID_L2Page;
									g_bytLItemID_L2Page = g_bytLItemID_L3Page;
									g_bytLItemID_L3Page = 0x00;
								}
							}
							break;
						
						case 0x0b:/*1ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSHour[1])) > 0)
								{
									g_objMenuOsd.PeriodSHour[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);						
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0d:/*1ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSMin[1])) > 0)
								{
									g_objMenuOsd.PeriodSMin[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0f:/*1ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSSec[1])) > 0)
								{
									g_objMenuOsd.PeriodSSec[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x10:/*1ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEHour[1])) > 0)
								{
									g_objMenuOsd.PeriodEHour[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x12:/*1ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEMin[1])) > 0)
								{
									g_objMenuOsd.PeriodEMin[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x14:/*1ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodESec[1])) > 0)
								{
									g_objMenuOsd.PeriodESec[1][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x15:/*1ʱ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
						}
							break;
							
						case 0x16:/*2ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSHour[2])) > 0)
								{
									g_objMenuOsd.PeriodSHour[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x18:/*2ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSMin[2])) > 0)
								{
									g_objMenuOsd.PeriodSMin[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x1a:/*2ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSSec[2])) > 0)
								{
									g_objMenuOsd.PeriodSSec[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x1b:/*1ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEHour[2])) > 0)
								{
									g_objMenuOsd.PeriodEHour[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x1d:/*2ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEMin[2])) > 0)
								{
									g_objMenuOsd.PeriodEMin[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x1f:/*2ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodESec[2])) > 0)
								{
									g_objMenuOsd.PeriodESec[2][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x20:/*2ʱ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
						}
							break;
							
						case 0x21:/*3ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSHour[3])) > 0)
								{
									g_objMenuOsd.PeriodSHour[3][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x23:/*3ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSMin[3])) > 0)
								{
									g_objMenuOsd.PeriodSMin[3][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x25:/*3ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSSec[3])) > 0)
								{
									g_objMenuOsd.PeriodSSec[3][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x26:/*3ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEHour[3])) > 0)
								{
									g_objMenuOsd.PeriodEHour[3][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x28:/*3ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEMin[3])) > 0)
								{
									g_objMenuOsd.PeriodEMin[3][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x2a:/*3ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodESec[3])) > 0)
								{
									g_objMenuOsd.PeriodESec[3][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x2b:/*3ʱ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
						}
							break;
							
						case 0x2c:/*4ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSHour[4])) > 0)
								{
									g_objMenuOsd.PeriodSHour[4][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x2e:/*4ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSMin[4])) > 0)
								{
									g_objMenuOsd.PeriodSMin[4][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x30:/*4ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSSec[4])) > 0)
								{
									g_objMenuOsd.PeriodSSec[4][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x31:/*4ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEHour[4])) > 0)
								{
									g_objMenuOsd.PeriodEHour[4][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x33:/*4ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEMin[4])) > 0)
								{
									g_objMenuOsd.PeriodEMin[4][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x35:/*4ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodESec[4])) > 0)
								{
									g_objMenuOsd.PeriodESec[4][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x36:/*4ʱ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
						}
							break;
							
						case 0x37:/*5ʱ�ο�ʼʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSHour[5])) > 0)
								{
									g_objMenuOsd.PeriodSHour[5][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x39:/*5ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSMin[5])) > 0)
								{
									g_objMenuOsd.PeriodSMin[5][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x3b:/*5ʱ�ο�ʼ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodSSec[5])) > 0)
								{
									g_objMenuOsd.PeriodSSec[5][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x3c:/*5ʱ�ν���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEHour[5])) > 0)
								{
									g_objMenuOsd.PeriodEHour[5][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x3e:/*5ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodEMin[5])) > 0)
								{
									g_objMenuOsd.PeriodEMin[5][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x40:/*5ʱ�ν�����*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.PeriodESec[5])) > 0)
								{
									g_objMenuOsd.PeriodESec[5][i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x41:/*5ʱ��ʹ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
						}
							break;
							
						case 0x42:/*ȷ��*/
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							break;
					
						default:
							break;
					}
				}
				break;
			case 0x10:/*�¼�����*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*����¼���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.AccDelay)) > 0)
								{
									g_objMenuOsd.AccDelay[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x02:/*�˶��¼���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.MoveDelay)) > 0)
								{
									g_objMenuOsd.MoveDelay[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x04:/*�ڵ��¼���ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.OverlayDelay)) > 0)
								{
									g_objMenuOsd.OverlayDelay[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x06:/*S0��ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.S1Delay)) > 0)
								{
									g_objMenuOsd.S1Delay[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x08:/*S1��ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.S2Delay)) > 0)
								{
									g_objMenuOsd.S2Delay[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0a:/*S2��ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.S3Delay)) > 0)
								{
									g_objMenuOsd.S3Delay[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0c:/*S3��ʱ*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.S4Delay)) > 0)
								{
									g_objMenuOsd.S4Delay[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x01:
						case 0x03:
						case 0x05:
						case 0x07:
						case 0x09:
						case 0x0b:
						case 0x0d:
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
						}
							break;
						
						case 0x0e:/*ȷ��*/
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = g_bytLItemID_L3Page;
								g_bytLItemID_L3Page = 0x00;
							}
							break;
					
						default:
							break;
					}
				}
				break;
			case 0x11:/*��������*/
				{
					switch(g_bytCItemID)
					{
						case 0x00:/*�ֻ�ģ��*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
						}
							break;
							
						case 0x01:/*��������*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_szTelNum)) > 0)
								{
									g_objMenuOsd.m_szTelNum[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x02:/*APN*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_szApn)) > 0)
								{
									g_objMenuOsd.m_szApn[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x03:/*IP*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_ulCommIP)) > 0)
								{
									g_objMenuOsd.m_ulCommIP[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x04:/*�˿ں�*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_usCommPort)) > 0)
								{
									g_objMenuOsd.m_usCommPort[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x05:/*IP*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_ulPhotoIP)) > 0)
								{
									g_objMenuOsd.m_ulPhotoIP[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x06:/*�˿ں�*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_usPhotoPort)) > 0)
								{
									g_objMenuOsd.m_usPhotoPort[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x07:/*IP*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_ulVideoIP)) > 0)
								{
									g_objMenuOsd.m_ulVideoIP[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x08:/*�˿ں�*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_usVideoPort)) > 0)
								{
									g_objMenuOsd.m_usVideoPort[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x09:/*IP*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_ulUpdateIP)) > 0)
								{
									g_objMenuOsd.m_ulUpdateIP[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
							
						case 0x0a:/*�˿ں�*/
						{
							if( !pCurMenuItem->Fix )
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
							else
							{
								if((i = strlen(g_objMenuOsd.m_usUpdatePort)) > 0)
								{
									g_objMenuOsd.m_usUpdatePort[i - 1] = 0;
									FlushItem(pCurMenuItem->Content, 0, NULL, 0, NULL, 0);
									ShowItem(pCurMenuPage, pCurMenuItem, BLACK, ORCHID);
								}
							}
						}
							break;
		
						case 0x0b:/*ȷ��*/
							{
								memset(&g_objMenuOsd, 0, sizeof(g_objMenuOsd));
								
								HideItem(pCurMenuItem);
								HidePage(pCurMenuPage);
						
								g_bytCPageID = GetNextID(pCurMenuItem, CANCEL);				
								pCurMenuPage = GetPage();
								InitPage(pCurMenuPage);
								ShowPage(pCurMenuPage, PAGE_LEVEL, PAGE_LINE, PAGE_ROW);
					
								g_bytCItemID = g_bytLItemID_L1Page;
								pCurMenuItem = GetItem();
								ShowItem(pCurMenuPage, pCurMenuItem, BLACK, YELLOW);
								
								g_bytLItemID_L1Page = g_bytLItemID_L2Page;
								g_bytLItemID_L2Page = 0x00;
							}
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
