local player = {}
local vec3 = Mafs.vector3(50, 40, 80)
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
		print(Mafs.vector3_z(vec3))
	end
end
return player