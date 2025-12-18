local Test = {}
local Coroutines = GetModule("Coroutines")

function Test:OnStart()
	local co1 = coroutine.create(function()
		while true do
			print("Coroutine Test")
			Coroutines.Yield()
		end
	end)

	Coroutines.AddCoroutine(co1, 3)
end

return Test