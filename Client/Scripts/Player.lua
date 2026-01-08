local player = {}
local keyCode = SDL_SCANCODE_SPACE

local player
local health
local maxHealth = 100
local x = 200
local y =  200

function player:OnStart()
health = maxHealth

Texture.add_texture("./Assets/"playerBlue_walk1", "player")

player = Entity.create_entity()
Entity.set_entity_pos(player, x , y) 

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