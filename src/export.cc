#include "export.h"
#include "log.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

extern lua_State	*L;

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

