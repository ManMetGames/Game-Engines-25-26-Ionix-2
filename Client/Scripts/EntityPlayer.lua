local EntityPlayer = {}
local entity1
local sprite1
local x = 500
local y = 100
local xSpeed = 5
local ySpeed = 5
local vec2
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

function EntityPlayer:OnStart()
	entity1 = Entity.create_entity()
	print(Entity.has_sprite_component(entity1))
	print(assets.textures.ball)
	Entity.add_sprite_component(entity1, assets.textures.PacMan, 100, 100, 0)   
	--Entity.add_fysics_component(entity1, 0, false)

	Entity.set_global_pos(entity1, x, y)
	Entity.add_fysics_component(entity1, enums.bodytype.kinematicBody, true)
	Fysics.add_sprite_collider(entity1, false, 1)

	local hasComp, sprite1 = Entity.try_get_sprite_component(entity1)
	if(hasComp) then
		--Sprite.set_playback_mode(sprite1, 4)
		Sprite.set_columns(sprite1, 1)
	end

	Fysics.load_nav_mef(0, 0, 500, 200, 25)
end

function EntityPlayer:OnUpdate()
	if entity1 == nil then
		return
	end
	
	vec2 = Fysics.get_linear_velocity(entity1)
	--Entity.set_entity_pos(entity1, 700, 600)
	if Input.get_key_held(Keys.ionix_d) then
		Mafs.set_vec_x(vec2, xSpeed);
	elseif Input.get_key_held(Keys.ionix_a) then
		Mafs.set_vec_x(vec2, -xSpeed);
	else
	Mafs.set_vec_x(vec2, 0);
	end

	if Input.get_key_held(Keys.ionix_w) then
		Mafs.set_vec_y(vec2, -ySpeed);
	elseif Input.get_key_held(Keys.ionix_s) then
		Mafs.set_vec_y(vec2, ySpeed);
	else
	Mafs.set_vec_y(vec2, 2);
	end

	Fysics.set_linear_velocity_v(entity1, vec2)
end
return EntityPlayer
