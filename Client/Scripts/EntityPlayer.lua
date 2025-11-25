local EntityPlayer = {}
local entity1
local sprite1
local x = 500
local y = 500
local xSpeed = 5
local ySpeed = 5
local vec2
function EntityPlayer:OnStart()
	local assets = GetModule("Assets")
	entity1 = Entity.create_entity()
	print(Entity.has_sprite_component(entity1))
	print(assets.textures.ball)
	Entity.add_sprite_component(entity1, "ball", 100, 100, 0)   
	Entity.add_fysics_component(entity1, 1, false)
	--Entity.add_fysics_component(entity1, 0, false)
	vec2 = Mafs.vec_2(0,0)

	Entity.set_entity_pos(entity1, x, y)

	local hasComp, sprite1 = Entity.try_get_sprite_component(entity1)
	if(hasComp) then
		Sprite.set_playback_mode(sprite1, 4)
	end
end

function EntityPlayer:OnUpdate()
	if entity1 == nil then
		return
	end
	
	Fysics.set_linear_velocity_v(entity1, vec2)
	print(Fysics.get_linear_velocity(entity1).x)
	print(Fysics.get_pos(entity1).x)
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
	vec2.y = 0
	end
end
return EntityPlayer
