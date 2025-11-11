local EntityPlayer = {}
local entity1
local entity2
local x = 500
local y = 500
local xSpeed = 10
local ySpeed = 10
function EntityPlayer:OnStart()
    entity1 = Entity.create_entity()
    entity2 = Entity.create_entity()
	
    Texture.add_texture("./Assets/aur naur.jpg", "aur")
    Entity.add_sprite_component(entity1, "aur", 5)
    Entity.add_sprite_component(entity2, "fuck", 3)
    Entity.set_entity_pos(entity1, 200, 400)
    Entity.set_entity_pos(entity2, 600, 400)
end

function EntityPlayer:OnUpdate()
--Entity.set_entity_pos(entity1, x, y)
--if  Input.get_key_held(Keys.ionix_d) then
--   x = x + xSpeed
--end
--if  Input.get_key_held(Keys.ionix_a) then
--   x = x - xSpeed
--end
--if  Input.get_key_held(Keys.ionix_w) then
--   y = y - ySpeed
--end
--if  Input.get_key_held(Keys.ionix_s) then
--   y = y + ySpeed
--end 

    
end
return EntityPlayer