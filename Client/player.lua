local player = { }

function player:OnStart()
	print("Player OnStart Ran")
end

function player:OnUpdate()
	local enemy = GetModule("enemy")
		
	if enemy then
		print("Enemy health: " .. enemy.health)
	end

	print("Player OnUpdate Ran")
end

return player