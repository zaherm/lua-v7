local v7 = require("v7")
print(v7._VERSION)
print(v7.version())
local c, ok, res
c = v7.create()
ok, res = c:exec([[1/2:1]])
assert(ok == v7.SYNTAX_ERROR)

c:exec([[
  function sum(a, b) {
    return a + b;
  }
]])

print(c:exec("sum(1,2)"))

ok, res = c:exec([[
  (function () {
    return "string"
  })();
]])
print(ok, res)

ok, res = c:exec([[
  (function () {
    return false
  })();
]])
print(ok, res)
ok, res = c:exec([[
  (function () {
    return 1;
  })();
]])
print(ok, res)
ok, res = c:exec([[
  (function () {
    return 1.2;
  })();
]])
print(ok, res)

ok, res = c:exec([[
  (function () {
    return null;
  })();
]])
print(ok, res)

ok, res = c:exec([[
  (function () {
    return undefined;
  })();
]])
print(ok, res)

ok, res = c:exec([[
  (function () {
    return ['a', 'r', 'r', 'a', 'y'];
  })();
]])
print("> array")
for k,v in ipairs(res) do print(k,v) end

ok, res = c:exec([[
  (function () {
    return { a: 1, b: [1,2,3], c: { x: 'y' }};
  })();
]])
print("> object")
for k,v in pairs(res) do print(k,v) end

