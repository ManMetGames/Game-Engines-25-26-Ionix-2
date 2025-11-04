local EntityPlayer = {}
local entity1
local x = 500
local y = 500
local xSpeed = 10
local ySpeed = 10
function EntityPlayer:OnStart()
	entity1 = Entity.create_entity()
	Entity.add_sprite_component(entity1, "ball", 0)
end

function EntityPlayer:OnUpdate()
	if entity1 == nil then
		return
	end
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
