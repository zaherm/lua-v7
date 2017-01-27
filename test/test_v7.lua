local v7 = require("v7")
print(v7._VERSION)
print(v7.version())

local c = v7.create()
local ok, res = c:exec([[
  sum([1, 1]);
]])

print(ok, res)
ok, res = c:exec([[1/2:1]])

print(ok, res)
