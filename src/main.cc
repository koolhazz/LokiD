#include "tolua++.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// using lua
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

/* Exported function */
TOLUA_API int  tolua_interface_open (lua_State* tolua_S);

lua_State		*L;
CMysql 			mysql_handle;
CRedis 			redis_handle;

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

static void
__pidfile()
{
	//int fd = open(PidPath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
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
}

int
main(int argc, char* argv[])
{
	pid_t 	pid;
	char 	log_prefix[50] = {0};

	pid = getpid();		
	
	snprintf(log_prefix, sizeof(log_prefix), "Log_%d", pid);
}


