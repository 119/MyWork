#include "yx_QianStdAfx.h"

LinkLst::LinkLst()
{
	count = 0;
	head = tail = curnode = NULL;
	
	pthread_mutex_init(&cs, NULL);
}

LinkLst::~LinkLst()
{
	Node *node = head;
	Node *next;
	while(node!=tail) {
		next = node->next;
		if(node)	{ free(node); node=NULL; }
		node = next;
	}
	if(tail)	{ free(tail); tail=NULL; }
	count = 0;
	head = tail = curnode = NULL;

	pthread_mutex_destroy(&cs);
}

//������β���һ����������
//data: ����ָ��
//len: ���ݳ���
bool LinkLst::append(byte* data, ulong len)
{
	if(data==NULL || len==0)	return false;
	
	enter_cs(cs);
	if(count>MAX_COUNT)		{ leave_cs(cs); return false; }
	Node *node = (Node*)malloc(sizeof(Node)+len);
	if(node==NULL)		{ leave_cs(cs); return false; }
	node->len = len;
	memcpy((byte*)node+sizeof(Node), data, len);
	
	node->prev = tail;
	if(count==0) { head = node; curnode = head; } 
	else { tail->next = node; }
	
	node->next = NULL;
	tail = node;
	count++;
	
	leave_cs(cs);
	
	return true;
}

//�޸ĵ�ǰ��������
//data: ����ָ��
//len: ���ݳ���	
bool LinkLst::modify(byte* data, ulong len)
{
	enter_cs(cs);
	if(count==0)	{ leave_cs(cs); return false; }
	if(curnode==NULL)	{ leave_cs(cs); return false; }
	
	Node *node = (Node*)malloc(sizeof(Node)+len);
	if(node==NULL)	{ leave_cs(cs); return false; }
	
	Node *prev = curnode->prev;
	Node *next = curnode->next;
	
	node->len = len;
	node->prev = prev;
	node->next = next;
	if(prev!=NULL)		prev->next = node;
	if(next!=NULL)		next->prev = node;
	
	memcpy((byte*)node+sizeof(Node), data, len);
	
	if(head==curnode)	head = node;
	if(tail==curnode)	tail = node;
	
	free(curnode);
	curnode = NULL;
	curnode = node;
	
	leave_cs(cs);
	return true;
}

//�ڵ�ǰ����ǰ���һ����������
//data: ����ָ��
//len: ���ݳ���
bool LinkLst::insertb(byte* data, ulong len)
{
	enter_cs(cs);
	
	if(count>MAX_COUNT)		{ leave_cs(cs); return false; }
	if(count==0)			{ leave_cs(cs); return false; }
	if(curnode==NULL)		{ leave_cs(cs); return false; }
	
	Node *node = (Node*)malloc(sizeof(Node)+len);
	if(node==NULL)	{ leave_cs(cs); return false; }
	node->len = len;
	memcpy((byte*)node+sizeof(Node), data, len);
	
	node->next = curnode;
	node->prev = curnode->prev;
	if(curnode->prev==NULL) { head = node; } 
	else { curnode->prev->next = node; }
	curnode->prev = node;
	count++;
	leave_cs(cs);
	return true;
}

//�ڵ�ǰ��������һ����������
//data: ����ָ��
//len: ���ݳ���
bool LinkLst::inserta(byte* data, ulong len)
{
	enter_cs(cs);
	
	if(count>MAX_COUNT)		{ leave_cs(cs); return false; }
	if(count==0)			{ leave_cs(cs); return false; }
	if(curnode==NULL)		{ leave_cs(cs); return false; }
	
	Node *node = (Node*)malloc(sizeof(Node)+len);
	if(node==NULL)	{ leave_cs(cs); return false; }
	node->len = len;
	memcpy((byte*)node+sizeof(Node), data, len);
	
	node->next = curnode->next;
	node->prev = curnode;
	if(curnode->next==NULL) { tail = node; }
	else { curnode->next->prev = node; }
	curnode->next = node;
	count++;
	leave_cs(cs);
	return true;
}

//�Ƴ���ǰ��������
bool LinkLst::remove()
{
	enter_cs(cs);
	
	if(count==0)	{ leave_cs(cs); return false; }
	if(curnode==NULL)	{ leave_cs(cs); return false; }
	
	Node *prevnode, *nextnode, *cur;
	prevnode = curnode->prev;
	nextnode = curnode->next;
	
	if(prevnode==NULL) { head = nextnode; cur = head; } 
	else { prevnode->next = nextnode; cur = nextnode; }
	
	if(nextnode==NULL) { tail = prevnode; cur = tail; } 
	else { nextnode->prev = prevnode; cur = nextnode; }
	
	free(curnode);
	curnode = NULL;
	curnode = cur;
	count--;
	
	leave_cs(cs);
	return true;
}

//�ƶ�����ָ�뵽����ͷ
bool LinkLst::gohead()
{
	enter_cs(cs);
	if(count!=0 && curnode!=NULL) { curnode = head; leave_cs(cs); return true; } 
	else { leave_cs(cs); return false; }
}

//�ƶ�����ָ�뵽����β	
bool LinkLst::gotail()
{
	enter_cs(cs);
	if(count!=0 && curnode!=NULL) { curnode = tail; leave_cs(cs); return true; } 
	else { leave_cs(cs); return false; }
}

//�ƶ�����ָ�뵽��һ����
bool LinkLst::next()
{
	enter_cs(cs);
	if(count==0)		{ leave_cs(cs); return false; }
	if(curnode==NULL)	{ leave_cs(cs); return false; }
	if(curnode->next==NULL)		{ leave_cs(cs); return false; }
	curnode = curnode->next;
	leave_cs(cs);
	return true;	 
}

//�ƶ�����ָ�뵽��һ����	
bool LinkLst::prev()
{
	enter_cs(cs);
	if(count==0)		{ leave_cs(cs); return false; }
	if(curnode==NULL)	{ leave_cs(cs); return false; }
	if(curnode->prev==NULL)		{ leave_cs(cs); return false; }
	curnode = curnode->prev;
	leave_cs(cs);
	return true;
}

//��õ�ǰ��������
//out: �������ָ��
//����: �����������
ulong LinkLst::getdata(byte* out)
{
	if(out==NULL)	return 0;
	enter_cs(cs);
	if(count==0)		{ leave_cs(cs); return 0; }
	if(curnode==NULL)	{ leave_cs(cs); return 0; }
	
	int len = curnode->len;
	memcpy(out, (byte*)curnode+sizeof(Node), len);
	leave_cs(cs);
	return len;
}

//����������
//����: �������
ulong LinkLst::getcount()
{
	enter_cs(cs);
	ulong cnt = count;
	leave_cs(cs);
	return cnt;
}

//�����������
void LinkLst::reset()
{
	enter_cs(cs);
	Node *node = head;
	Node *next;
	while(node!=tail) {
		next = node->next;
		if(node) { free(node); node=NULL; }
		node = next;
	}
	if(tail)	{ free(tail); tail=NULL; }
	count = 0;
	head = tail = curnode = NULL;
	leave_cs(cs);
}

