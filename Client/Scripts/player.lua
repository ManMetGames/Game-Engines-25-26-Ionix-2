local player = {}
local vec2 = Mafs.vector2(50, 40)
function player:OnStart()
	print("player start ran.")
end


function player:OnUpdate()
	local enemy = GetModule("enemy")

	--[[if enemy then
		print("Enemy health: " .. enemy.health)
	end
	print("player update ran.")--]]
	
	if Input.get_key_down(Keys.ionix_space) then
		print(Mafs.vector2_x(vec2))
	end
end
return player