local EntityPlayer = {}
local entity1
local floorBox
local sprite1
local x = 200
local y = 300
local xSpeed = 10
local ySpeed = 11
local t = 10
local ok

function EntityPlayer:OnStart()
	-- Create a player entity and add a dynamic rigidbody with box collider to it.
	player = Entity.create_entity()
	Entity.add_sprite_component(player, "ball", 100, 100, 0)

	Entity.add_fysics_component(player, 2, false) -- dynamic / non-trigger
	Fysics.add_box_collider(player, 1, 1, 0, 0, 0, true)
	Entity.set_entity_pos(player, x, y)	
	
	theFloor = Entity.create_entity()
	Entity.add_sprite_component(theFloor, "ball", 100, 100, 0)

	Entity.add_fysics_component(theFloor, 2, false) -- dynamic / non-trigger
	Fysics.add_box_collider(theFloor, 1, 1, 0, 0, 0, true)
	Entity.set_entity_pos(theFloor, x + 100, y)	
	-- multiple entity issue

	
	--sprite1 = Entity.get_sprite_component(entity1)
	--Sprite.set_playback_mode(sprite1, 3)
end

function EntityPlayer:OnUpdate()
	if entity1 == nil then
		return
	end
	
	-- x = Mafs.lerp(x, 700, t)	
	-- t = Mafs.delta_time() * t
	
	if Input.get_key_down(Keys.ionix_space) then
		Fysics.add_impulse_to_center(entity1, 0, -15)
		--x = x + xSpeed
	end
	
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
	
	--Entity.set_entity_pos(entity1, x, y)
end
return EntityPlayer
