local EntityPlayer = {}
local entity1
local sprite1
local x = 500
local y = 500
local xSpeed = 10
local ySpeed = 10
local t = 10
function EntityPlayer:OnStart()
	local assets = GetModule("Assets")
	entity1 = Entity.create_entity()
	print(Entity.has_sprite_component(entity1))
	print(assets.textures.ball)
	Entity.add_sprite_component(entity1, "ball", 100, 100, 0)
	Entity.set_entity_pos(entity1, x, y)
	--sprite1 = Entity.get_sprite_component(entity1)
	
	local hasComp, sprite1 = Entity.try_get_sprite_component(entity1)
	if(hasComp) then
		Sprite.set_playback_mode(sprite1, 4)
end
	--Sprite.set_playback_mode(sprite1, 3)
end

function EntityPlayer:OnUpdate()
	if entity1 == nil then
		return
	end
	
	-- x = Mafs.lerp(x, 700, t)	
	-- t = Mafs.delta_time() * t

	

	Entity.set_entity_pos(entity1, x, y)

	if Input.get_key_held(Keys.ionix_d) then
		x = x + xSpeed
	end
	if Input.get_key_held(Keys.ionix_a) then
		x = x - xSpeed
	end
	if Input.get_key_held(Keys.ionix_w) then
		y = y - ySpeed
	end
	if Input.get_key_held(Keys.ionix_s) then
		y = y + ySpeed
	end
end
return EntityPlayer
