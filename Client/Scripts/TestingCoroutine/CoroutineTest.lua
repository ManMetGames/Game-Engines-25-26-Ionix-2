local CoroutineTest = {}
local Coroute = GetModule("CoModusOperandi")

function CoroutineTest:OnStart()
	local co1 = coroutine.create(function()
		while true do
			print("Coroutine Test")
			Coroute:Yield()
		end
	end)

	Coroute:AddCoroutine(co1, 3)
end

return CoroutineTest