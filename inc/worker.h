#ifndef __WORKER_H_
#define __WORKER_H_

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <pthread.h>

typedef struct worker_s worker_t;
struct worker_s {
	lua_State 	*L;
	pthread_t 	tid;
	pid_t		pid;
};

typedef struct worker_pool_s worker_pool_t;
struct worker_pool_s {
	unsigned int 	sz;
	worker_t 		**pool;
};

typedef void* (*worker_handler_t)(void*);

extern worker_pool_t*
worker_pool_new(unsigned int sz = 2, workder_handler_t handler);

extern worker_t*
worker_new(worker_handler_t handler);



extern 

#endif