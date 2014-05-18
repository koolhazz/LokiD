#ifndef __CALL_H_
#define __CALL_H_

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef __cplusplus
extern "C" {
#endif 	

	
extern int
lua(lua_State* L, const char *func, const char *sig, ...);

#ifdef __cplusplus
}
#endif

#endif
