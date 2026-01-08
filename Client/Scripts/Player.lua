local player = {}
local keyCode = SDL_SCANCODE_SPACE

local player
local health
local maxHealth = 100
local x = 200
local y =  200
local attackDamage = 10

function player:OnStart()
health = maxHealth

Texture.add_texture("./Assets/"playerBlue_walk1", "player")

player = Entity.create_entity()
Entity.set_entity_pos(player, x , y) 
playerSprite = Entity.add_sprite_component(player, "walk" , 75, 75 ,0)
Sprite.set_playback_mode(playerSprite, 4) 

Entity.add_fysics_component(player, 2 , true) 
Fysics.add_sprite_collider(player, true)

end

function player:attack()
 if Input.get_mouse_button_down() then
	enemy.health - attackDamage = enemy.dmgHealth
	enemy.dmgHealth = enemy.health
 end

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