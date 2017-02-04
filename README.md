lua-v7
====
Lua binding for v7 (https://github.com/cesanta/v7)

Example
===

```lua
local v7 = require("v7")
local ctx = v7.create()

local ok, res = ctx:exec([[
  print("example");
]])
assert(ok == v7.OK)
```

Doc
===

Codes
==
  * v7.OK
  * v7.EXCEPTION
  * v7.SYNTAX_ERROR
  * v7.AST_TOO_LARGE

v7.create()
==
Create a v7 context.
return _v7_ instance.

v7.version()
==
Return the v7 version as a string.

code, result = v7:apply(func [, args])
==
Invoke a global function in the v7 context.
  * params:
    * **func** (string) global function name
    * **args** (any) optional arguments, can be any lua type (except _function_)
  * return:
    * **code** (int) execution code
    * **result** (any) Lua value (automatic conversion)

**Example**

```lua
local v7 = require("v7")
local ctx = v7.create()
-- add a global function
local ccode, cresult = ctx:exec([[
  function sum(a, b) {
    return a + b;
  }
  function keys(obj) {
    var result = [];
    for(var k in obj) {
      result.push(k);
    }
    return result;
  }
]])
assert(ccode == v7.OK)
local acode, aresult = ctx:apply("sum", 1, 2)
assert(acode == v7.OK)
assert(aresult == 3)

local t = { key1 = "value1", key2 = "value2" }
local kcode, keys = ctx:apply("keys", t);
assert(kcode == v7.OK)
for _, k in pairs(keys) do
  print(k)
  assert(t[k])
end

```

