#ifndef __CALL_H_
#define __CALL_H_

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
	
extern int
lua(lua_State* L, const char *func, const char *sig, ...);

#endif
