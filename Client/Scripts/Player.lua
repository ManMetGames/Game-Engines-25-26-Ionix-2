local player = {}
local keyCode = SDL_SCANCODE_SPACE

local health
local maxHealth = 100


function player:OnStart()
health = maxHealth

end


function player:OnUpdate()
	local enemy = GetModule("enemy")

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