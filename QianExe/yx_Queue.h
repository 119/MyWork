#ifndef _YX_QUEUE_H_
#define _YX_QUEUE_H_

// �Ƚ��ȳ�������

#define MAX_ITEM 	1024	// ���Ķ��и���

class Queue
{
public:
	Queue();
	~Queue();

	bool push(unsigned char *data, ulong len);
	ulong pop(unsigned char *data);
	int getcount();
	void reset();
	
private:
	
	typedef struct {
		unsigned char *data;
		ulong len;
	} Item;
	
	Item lst[MAX_ITEM];
	int p1,p2;
	pthread_mutex_t cs;
};

#endif

