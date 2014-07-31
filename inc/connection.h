#ifndef __CONNECTION_H_
#define __CONNECTION_H_

#include <sys/epoll.h>

typedef int (*conn_event_handle_t)(connection_t*);

enum conn_stat_t {
	connected;
	header; /* ���հ�ͷ */
	body;  /* ���հ��� */
	done; /* ������� */	
};

struct connection_s {
	int 				fd;
	unsigned int 		event;
	buffer_t			rb;
	buffer_t			wb;
	conn_event_handle_t rh;
	conn_event_handle_t wh;
	conn_stat_t			stat;
};

#endif
