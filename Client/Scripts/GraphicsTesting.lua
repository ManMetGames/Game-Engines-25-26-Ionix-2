local GraphicsTesting = {}
local entity1
local x = 500
local y = 500
local xSpeed = 10
local ySpeed = 10
function GraphicsTesting:OnStart()
    entity1 = Entity.create_entity()
    Texture.add_texture("./Assets/aur naur.jpg", "aur")
    Entity.add_sprite_component(entity1, "aur", 0)
    
end


function GraphicsTesting:OnUpdate()
Entity.set_entity_pos(entity1, x, y)
if  Input.get_key_held(Keys.ionix_d) then
   x = x + xSpeed
end
if  Input.get_key_held(Keys.ionix_a) then
   x = x - xSpeed
end
if  Input.get_key_held(Keys.ionix_w) then
   y = y - ySpeed
end
if  Input.get_key_held(Keys.ionix_s) then
   y = y + ySpeed
end

    
end
return GraphicsTesting