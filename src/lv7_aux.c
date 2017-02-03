#include "lv7_aux.h"

#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501
LUALIB_API void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
  luaL_checkstack(L, nup+1, "too many upvalues");
  for (; l->name != NULL; l++) {  /* fill the table with given functions */
    int i;
    lua_pushstring(L, l->name);
    for (i = 0; i < nup; i++)  /* copy upvalues to the top */
      lua_pushvalue(L, -(nup+1));
    lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
    lua_settable(L, -(nup + 3));
  }
  lua_pop(L, nup);  /* remove upvalues */
}
#endif

LUALIB_API void lv7_setmeta(lua_State *L, const char *name) {
  luaL_getmetatable(L, name);
  lua_setmetatable(L, -2);
}

LUALIB_API int lv7_createmeta(lua_State *L, const char *name, const luaL_Reg *methods) {
  if (!luaL_newmetatable(L, name)) {
    return 0;
  }

  lua_pushstring(L, "__index");
  lua_newtable(L);
  lua_pushstring(L, "class");
  lua_pushstring(L, name);
  lua_rawset(L, -3);

  for (; methods->name; methods++) {
    lua_pushstring(L, methods->name);
    lua_pushcfunction(L, methods->func);
    lua_rawset(L, methods->name[0] == '_' ? -5: -3);
  }
  lua_rawset(L, -3);
  lua_pop(L, 1);
  return 1;
}

void lv7_assert(lua_State *L, int cond, const char *msg)  {
  if(!cond) {
    luaL_error(L, msg);
  }
}

void lv7_pushvalue(lua_State *L, struct v7 *v7, v7_val_t value) {
  if(v7_is_string(value)) {
    size_t len;
    const char *str = v7_get_string(v7, &value, &len);
    lua_pushlstring(L, str, len);
  }
  else if(v7_is_boolean(value)) {
    int b = v7_get_bool(v7, value);
    lua_pushboolean(L, b);
  }
  else if(v7_is_number(value)) {
    double d = v7_get_double(v7, value);
    lua_pushnumber(L, d);
  }
  else if(v7_is_null(value) || v7_is_undefined(value)) {
    lua_pushnil(L);
  }
  else if(v7_is_array(v7, value)) {
    unsigned long length = v7_array_length(v7, value);
    v7_val_t v;
    unsigned long i;
    lua_newtable(L);
    for(i = 0; i < length; i++) {
      v = v7_array_get(v7, value, i);
      lv7_pushvalue(L, v7, v);
      lua_rawseti(L,-2,i + 1);
    }
  }
  else if(v7_is_object(value)) {
    void *h = NULL;
    v7_val_t name, val;
    v7_prop_attr_t attrs;
    lua_newtable(L);
    while ((h = v7_next_prop(h, value, &name, &val, &attrs)) != NULL) {
      lv7_pushvalue(L, v7, name);
      lv7_pushvalue(L, v7, val);
      lua_settable(L, -3);
    }
  }
  else {
    luaL_error(L, "unknown type");
  }
}

v7_val_t lv7_makevalue(lua_State *L, int index, struct v7 *v7) {
  v7_val_t val;
  int type = lua_type(L, index);
  size_t vbool; double vnumber;
  const char *vstring;
  size_t vstring_len;
  size_t vtable_len;
  switch(type) {
    case LUA_TBOOLEAN:
      vbool = lua_toboolean(L, index);
      val = v7_mk_boolean(v7, vbool);
      break;
    case LUA_TNUMBER:
      vnumber = lua_tonumber(L, index);
      val = v7_mk_number(v7, vnumber);
      break;
    case LUA_TSTRING:
      vstring = lua_tolstring(L, index, &vstring_len);
      val = v7_mk_string(v7, vstring, vstring_len, 1);
      break;
    case LUA_TNIL:
      val = v7_mk_null();
      break;
    case LUA_TTABLE:
      vtable_len = lua_objlen(L, index);
      //object
      if(vtable_len == 0) {
        val = v7_mk_object(v7);
        void *ud = (void *) 0;
        v7_own(v7, &val);
        lv7_assert(L, v7_get_user_data(v7, val) == NULL, "v7_get_user_data failed");
        v7_set_user_data(v7, val, ud);
        lv7_assert(L, v7_get_user_data(v7, val) == ud, "v7_get_user_data failed");
        lua_pushnil(L);
        while(lua_next(L, -2) != 0) {
          size_t len;
          const char *key = lua_tolstring(L, -2, &len);
          v7_val_t tval = lv7_makevalue(L, -1, v7);
          v7_set(v7, val, key, len, tval);
          lua_pop(L, 1);
        }
      }
      //array
      else {
        val = v7_mk_array(v7);
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
          v7_val_t aval = lv7_makevalue(L, -1, v7);
          v7_array_push(v7, val, aval);
          lua_pop(L, 1);
        }
      }
      break;
  }
  return val;
}

v7_val_t lv7_makeargs(lua_State *L, struct v7 *v7) {
  int argc = lua_gettop(L) + 1;
  int i = 3;
  v7_val_t args = v7_mk_array(v7);
  for(; i < argc; i++) {
    v7_val_t v = lv7_makevalue(L, i, v7);
    v7_array_push(v7, args, v);
  }
  return args;
}

