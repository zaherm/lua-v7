#ifndef LV7_H
#define LV7_H

#include "../dep/v7.h"
#include "lv7_aux.h"

#define LV7_VERSION "lua-v7 0.0.1"
#define LV7_COPYRIGHT "Copyright (c) 2017, Zaher Marzuq"
#define LV7_DESCRIPTION "v7 binding for Lua"


typedef struct {
  struct v7 *v7;
} lv7;

static struct {
  const char *name;
  int value;
} lv7_codes[] = {
  { "OK", V7_OK },
  { "SYNTAX_ERROR", V7_SYNTAX_ERROR },
  { "EXEC_EXCEPTION", V7_EXEC_EXCEPTION },
  { "AST_TOO_LARGE", V7_AST_TOO_LARGE },
  { NULL, 0 }
};

LUALIB_API int lv7_version(lua_State *L);
LUALIB_API int lv7_create(lua_State *L);
LUALIB_API int lv7_destroy(lua_State *L);
LUALIB_API int lv7_exec(lua_State *L);
LUALIB_API int lv7_apply(lua_State *L);

void lv7_pushcodes(lua_State *L);

static const luaL_Reg lv7_reg[] = {
  { "__gc", lv7_destroy },
  { "exec", lv7_exec },
  { "apply", lv7_apply },
  { "destroy", lv7_destroy },
  {NULL, NULL}
};

static const luaL_Reg lv7_funcs[] = {
  { "version", lv7_version },
  { "create", lv7_create },
  { NULL, NULL }
};

#endif

