#include "worker.h"

static lua_State*
__new_lua_state()
{
	lua_State* L = lua_open();     /* initialize Lua */
    luaL_openlibs(L);   /* load Lua base libraries */
    tolua_export_open(L);

    return L;
}

static worker_t*
__new_worker(worker_handler_t handler)
{
	worker_t* w = malloc(sizeof *w);
	memset(w, 0, sizeof *w);

	pthread_create(&w->tid, NULL, handler, NULL);

	w->L = __new_lua_state();
	w->pid = getpid();

	return w;
}


worker_t*
worker_new(worker_handler_t handler)
{
	return __new_worker(handler);
}

worker_pool_t*
worker_pool_new(unsigned int sz, workder_handler_t handler)
{
	worker_pool_t* wp = malloc(sizeof *wp);
	wp->sz = sz;
	wp->pool = (worker_t**)malloc((sizeof *(wp->pool)) * sz);


	if (wp->pool == NULL) {
		return wp;
	}
	
	for (unsigned int i = 0; i < sz; i++) {
		(*wp->pool + i) = __new_worker(handler);
	}

	return wp;
}
