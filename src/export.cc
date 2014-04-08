#include "export.h"
#include "mysql_part.h"
#include "log.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

extern lua_State	*L;
extern CMysql		mysql_handle;

int
connect_mysql(const char* host,
			  const char* user, 
			  const char* password, 
			  const char* dbname, 
			  unsigned int port)
{
	return mysql_handle.connect_mysql(host, user, password, dbname, port);	
}

int
query(const char* mysql)
{
	return mysql_handle.query(mysql);	
}

void 
error(const char* msg)
{
    log_error(msg);
}

void 
info(const char* msg)
{
    log_info(msg);
}

void 
debug(const char* msg)
{
    log_debug(msg);
}

