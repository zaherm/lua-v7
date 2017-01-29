local v7 = require("v7")
print(v7._VERSION)
print(v7.version())
local c, ok, res
c = v7.create()
ok, res = c:exec([[1/2:1]])
assert(ok == v7.SYNTAX_ERROR)

function printtable(tbl)
  for k,v in pairs(tbl) do
    print(k, v)
  end
end

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

c:exec([[
var Lua = {
  func: function() {
    return Math.random();
  }
};
function gLua() {
  return Lua.func();
}
]])
ok, res = c:apply("gLua")
if ok == v7.EXEC_EXCEPTION then
  printtable(res)
  print("EXCEPTION", res.message)
else
  print(ok, res)
end

c:exec([[
  function test() {
    print(JSON.stringify(arguments));
    return true;
  }

  function printString(str) {
    print("str: " + str);
    return;
  }

  function add(a, b) {
    return a + b;
  }
]])

ok, res = c:apply("test", 1, 1.2, true, false, nil, "string")
print(ok, res)
ok, res = c:apply("test", { 1, 2, 3 })
print(ok, res)
ok, res = c:apply("test", { a = "a_value", b = "b_value", c = "c_value" })
print(ok, res)
-- nested object
ok, res = c:apply("test", { a = 1, b = { c =  { d = { e = { "a" } } }  } })
print(ok, res)

ok, res = c:apply("add", 1, 2)
print("res = ", res)

ok, res = c:exec([[
  (function() {
    var s = Socket.connect("google.com", 80);
s.send("GET / HTTP/1.0\n\n");
var reply = s.recv();
  })();
]])
print(res.message)

