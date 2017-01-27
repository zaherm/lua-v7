#include "lv7.h"

lv7* lv7_check(lua_State *L, int index) {
  lv7 *p = (lv7 *) luaL_checkudata(L, index, "v7");
  luaL_argcheck(L, p != NULL && p->v7 != NULL, index, "v7 expected");
  return p;
}

LUALIB_API int lv7_version(lua_State *L) {
  lua_pushstring(L, V7_VERSION);
  return 1;
}

LUALIB_API int lv7_create(lua_State *L) {
  lv7 *p = (lv7 *) lua_newuserdata(L, sizeof(lv7));
  p->v7 = v7_create();
  lv7_setmeta(L, "v7");
  return 1;
}

LUALIB_API int lv7_destroy(lua_State *L) {
  lv7 *p = lv7_check(L, 1);
  v7_destroy(p->v7);
  p->v7 = NULL;
  return 0;
}

LUALIB_API int lv7_exec(lua_State *L) {
  lv7 *p = lv7_check(L, 1);
  const char *code = luaL_checkstring(L, 2);
  enum v7_err rc = V7_OK;
  const char *err;
  v7_val_t result;
  rc = v7_exec(p->v7, code, &result);
  lua_pushinteger(L, rc);
  switch(rc) {
    case V7_OK:
      lv7_pushvalue(L, p->v7, result);
      break;
    case V7_AST_TOO_LARGE:
      lua_pushstring(L, "AST too large");
      break;
    case V7_EXEC_EXCEPTION:
      lua_pushstring(L, "Exception!");
      break;
    case V7_SYNTAX_ERROR:
      err = v7_get_parser_error(p->v7);
      lua_pushstring(L, err);
      break;
    case V7_INTERNAL_ERROR:
      lua_pushstring(L, "Internal Error");
      break;
  }
  return 2;
}

void lv7_pushcodes(lua_State *L) {
  int i;
  for(i = 0; lv7_codes[i].name != NULL; i++) {
    lua_pushinteger(L, lv7_codes[i].value);
    lua_setfield(L, -2, lv7_codes[i].name);
  }
}

LUALIB_API int luaopen_v7(lua_State *L) {
  lua_newtable(L);
  lv7_createmeta(L, "v7", lv7_reg);
  luaL_setfuncs(L, lv7_funcs, 0);

  lua_pushliteral(L, LV7_VERSION);
  lua_setfield(L, -2, "_VERSION");
  lua_pushliteral(L, LV7_COPYRIGHT);
  lua_setfield(L, -2, "_COPYRIGHT");
  lua_pushliteral(L, LV7_DESCRIPTION);
  lua_setfield(L, -2, "_DESCRIPTION");
  lv7_pushcodes(L);
  return 1;
}
