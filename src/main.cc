#include "tolua++.h"
#include "log.h"

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
#include "call.h"
}

/* Exported function */
TOLUA_API int 
tolua_export_open(lua_State* tolua_S);

lua_State		*L;

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

bool is_daemon = true;

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
	
	init_log(log_prefix, "./"); /* 初始化log */

	__binding_cpu();

	L = lua_open();     /* initialize Lua */
    luaL_openlibs(L);   /* load Lua base libraries */
    tolua_export_open(L);
    luaL_dofile(L, "lua/server.lua");    /* load the script */

	int ret = 0;
	if (lua("start", ">d", &ret)) { /* 进入程序主循环 */
		return -1;
	}
}


