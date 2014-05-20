#include "worker.h"
#include <stdlib.h>
#include <string.h>

static lua_State*
__new_lua_state()
{
	lua_State* L = lua_open();     /* initialize Lua */
    luaL_openlibs(L);   /* load Lua base libraries */
    //tolua_export_open(L);

    return L;
}

static worker_t*
__new_worker(worker_handler_t handler)
{
	worker_t* w = malloc(sizeof *w);
	memset(w, 0, sizeof *w);
	w->L = __new_lua_state();
	w->pid = getpid();

	pthread_create(&w->tid, NULL, handler, (void*)w);

	return w;
}

/* 绑定线程绑定cpu */
static void
__affinity(worker_t* w, unsigned int n)
{
	unsigned int cps = sysconf(_SC_NPROCESSORS_ONLN);

	cpu_set_t cpu;

	CPU_ZERO(&cpu);
	CPU_SET(n % cps, &cpu);

	pthread_setaffinity_np(w->tid, sizeof cpu, &cpu);
}

worker_t*
worker_new(worker_handler_t handler)
{
	return __new_worker(handler);
}

worker_pool_t*
worker_pool_new(unsigned int sz, worker_handler_t handler)
{
	worker_pool_t* wp = malloc(sizeof *wp);
	// hack struct worker_pool_t* wp = malloc(sizeof *wp + sizeof(worker_t*) * sz);
	wp->sz = sz;
	wp->pool = (worker_t**)malloc((sizeof *(wp->pool)) * sz);


	if (wp->pool == NULL) {
		return wp;
	}
	
	for (unsigned int i = 0; i < sz; i++) {
		*(wp->pool + i) = __new_worker(handler);
		__affinity(*(wp->pool + i), i + 1);
	}
	
	return wp;
}
