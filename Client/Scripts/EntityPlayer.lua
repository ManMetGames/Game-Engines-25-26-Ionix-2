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
	Entity.add_sprite_component(entity1, assets.textures.FlappyBird, 100, 100, 0)   
	--Entity.add_fysics_component(entity1, 0, false)

	Entity.set_entity_pos(entity1, x, y)
	Entity.add_fysics_component(entity1, enums.bodytype.kinematicBody, true)
	Fysics.add_sprite_collider(entity1, false, 1)

	local hasComp, sprite1 = Entity.try_get_sprite_component(entity1)
	if(hasComp) then
		--Sprite.set_playback_mode(sprite1, 4)
		Sprite.set_columns(sprite1, 1)
	end
end

function EntityPlayer:OnUpdate()
	if entity1 == nil then
		return
	end
	
	vec2 = Fysics.get_linear_velocity(entity1)
	--Entity.set_entity_pos(entity1, 700, 600)
	if Input.get_key_held(Keys.ionix_d) then
		vec2.x = xSpeed
	elseif Input.get_key_held(Keys.ionix_a) then
		vec2.x = -xSpeed
	else
	vec2.x = 0
	end

	if Input.get_key_held(Keys.ionix_w) then
		vec2.y = -ySpeed
	elseif Input.get_key_held(Keys.ionix_s) then
		vec2.y = ySpeed
	else
	vec2.y = 2
	end

	Fysics.set_linear_velocity_v(entity1, vec2)
end
return EntityPlayer
