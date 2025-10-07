local player = {}

function player:OnStart()
	print("player start ran.")
end


function player:OnUpdate()
	local enemy = GetModule("enemy")

	if enemy then
		print("Enemy health: " .. enemy.health)
	end
	print("player update ran.")
end
return player