#ifndef LV7_AUX_H
#define LV7_AUX_H
#include "lauxlib.h"
#include "../dep/v7.h"

#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501
LUALIB_API void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup);
#endif

LUALIB_API void lv7_setmeta(lua_State *L, const char *name);
LUALIB_API int lv7_createmeta(lua_State *L, const char *name, const luaL_Reg *methods);

void lv7_assert(lua_State *L, int cond, const char *msg);

void lv7_pushvalue(lua_State *L, struct v7 *v7, v7_val_t value);
#endif

