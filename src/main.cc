#include "tolua++.h"
#include "log.h"
#include "worker.h"
#include "call.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

// using lua
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
 
}

/* Exported function */
TOLUA_API int 
tolua_export_open(lua_State* tolua_S);

lua_State		*L;
worker_pool_t	*pool;
bool 			is_daemon = true;

static void 
__binding_cpu(void)
{
	int curr_cpu_max = sysconf(_SC_NPROCESSORS_CONF);
	
	srand(time(NULL));
	
	int num = rand() % curr_cpu_max;

	while(!num) {
		num = rand() % curr_cpu_max;
	}
	
	log_debug("CPU: %d\n", num);
	
	cpu_set_t mask;

	__CPU_ZERO(&mask);

	__CPU_SET(num, &mask);

	sched_setaffinity(0,sizeof(cpu_set_t),&mask);	
}

/*static void
__pidfile()
{
	int fd = open(PidPath, O_CREAT | O_RDWR, 00666);

	if (fd <= 0) {
		log_error("create pidfile failed.");
		return;
	}

	char temp[64] = {0};

	snprintf(temp, 64, "%d", getpid());

	int ret = write(fd, temp, strlen(temp));

	if (ret <= 0) {
		log_error("write pid failed. %d %d", ret, errno);
	}
}*/

static void*
__exec_lua(void* data)
{
	worker_t 	*w;
	lua_State 	*L;

	w = (worker_t*)data;
	L = w->L;

	tolua_export_open(L);
    luaL_dofile(L, "lua/server.lua");    /* load the script */

	int ret = 0;
	if (lua(L, "start", ">d", &ret)) { /* ½øÈë³ÌÐòÖ÷Ñ­»· */
		return (void*)-1;
	}	

	return (void*)0;
}

int
main(int argc, char* argv[])
{
	if (daemon(1, 0) == -1) {
		exit(-1);
	}
	
	pid_t 	pid;
	char 	log_prefix[50] = {0};

	pid = getpid();		
	
	snprintf(log_prefix, sizeof(log_prefix), "Log_%d", pid);
	
	init_log(log_prefix, "./"); /* ³õÊ¼»¯log */

	pool = worker_pool_new(10, __exec_lua);
	if (pool == NULL) {
		log_error("worker pool new failed.");
		return -1;
	}

	for (unsigned int i = 0; i < 10; i++) {
		pthread_join((*(pool->pool + i))->tid, NULL);
	}

	return 0;
}


