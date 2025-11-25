local ok, test = pcall(require, "Test")

if not ok then
	print("Could not load Test.lua")
	return
end

local coroutines = {}

local co2 = function()
	local y = 10
	while y > 0 do
		y = y - 1
		print("y: " .. y)
		coroutine.yield()
	end
end

table.insert(coroutines, coroutine.create(test.co1))
table.insert(coroutines, coroutine.create(co2))

print(#coroutines)

for i, coro in ipairs(coroutines) do
	while coroutine.status(coro) ~= "dead" do
		coroutine.resume(coro)
	end
	table.remove(coroutines, i)
end
