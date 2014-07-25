#ifndef __EVENT_LOOP_H_
#define __EVENT_LOOP_H_

typedef struct connection_s connection_t;
struct connection_s;

typedef struct worker_s worker_t;
struct worker_s;

typedef int (*event_loop_run_t)(int timeout);
typedef int	(*event_loop_read_t)(connection_t* c);
typedef int (*event_loop_write_t)(connection_t* c);

typedef struct event_loop_s event_loop_t;
struct event_loop_s {
	int 				poller;
	int					to; /* epoll_wait timeout */
	int					enums; /* 每次关注时间的个数 */
	worker_t 			*w;
	event_loop_run_t 	run;
	event_loop_read_t	read;
	event_loop_write_t	write;	
};



#endif

