local M = {
	co1 = function()
		local x = 0
		while x < 10 do
			x = x + 1
			print("x: " .. x)
			coroutine.yield()
		end
	end,
}

return M
