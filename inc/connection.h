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

typedef struct buffer_s buffer_t;
struct buffer_s {
	char 			*data;
	unsigned int 	sz;
	unsigned int 	idx;	
};

typedef struct connection_s connection_t;
struct connection_s {
	int 				fd;
	unsigned int 		event;
	buffer_t			rb;
	buffer_t			wb;
	conn_event_handle_t rh;
	conn_event_handle_t wh;
	conn_stat_t			stat;
};

connection_t*
connection_new(int fd, unsigned int bs);

int
connection_free(connection_t* c);

#endif
