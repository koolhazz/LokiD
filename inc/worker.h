#ifndef __WORKER_H_
#define __WORKER_H_

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus 
extern "C" {
#endif
	
typedef struct worker_s worker_t;
struct worker_s {
	lua_State 	*L;
	pthread_t 	tid;
	pid_t		pid;
	int			notify; /* 父线程通信句柄 */
};

typedef struct worker_pool_s worker_pool_t;
struct worker_pool_s {
	unsigned int 	sz;
	worker_t 		**pool; // worker_t* pool[10];  hack struct worker_t* pool[0]
};

typedef void* (*worker_handler_t)(void*);

extern worker_pool_t*
worker_pool_new(unsigned int sz, worker_handler_t handler);

extern worker_t*
worker_new(worker_handler_t handler);


#ifdef __cplusplus
}
#endif

#endif
