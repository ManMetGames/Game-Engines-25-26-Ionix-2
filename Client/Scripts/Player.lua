local player = {}
local keyCode = SDL_SCANCODE_SPACE

local player
local health
local maxHealth = 100
local x = 200
local y =  200
local attackDamage = 10

function player:OnStart()
health = maxHealth --Sets max health of the player on launch 

Texture.add_texture("./Assets/"playerBlue_walk1", "player")

--creates player entity
player = Entity.create_entity() 
Entity.set_entity_pos(player, x , y) 
playerSprite = Entity.add_sprite_component(player, "walk" , 75, 75 ,0)
Sprite.set_playback_mode(playerSprite, 4) 

--adds collision to player
Entity.add_fysics_component(player, 2 , true) 
Fysics.add_sprite_collider(player, true)

end

--allows player to attack the enemy
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