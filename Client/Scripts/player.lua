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

	if Input.get_key_down(44) then
		print("key down")
	end

	if Input.get_key_up(44) then
        print("key up")
    end

	if Input.get_key_held(44) then
        print("key held")
    end


end
return player