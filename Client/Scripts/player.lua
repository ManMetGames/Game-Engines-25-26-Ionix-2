local player = {}
local keyCode = SDL_SCANCODE_SPACE

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
		print(Mafs.square_root(25))
	end
end
return player