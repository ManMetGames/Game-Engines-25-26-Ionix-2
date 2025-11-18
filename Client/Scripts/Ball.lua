local ball = {}
local xSpeed = 2
local ySpeed = 2
local x
local y
function ball:OnStart()
    local entity = Entity.create_entity()
    Entity.add_sprite_component(entity, "PimBall", 512, 512,1)
    Entity.set_entity_position(entity, 100, 100)
end
function ball:onUpdate()
    
    x = x+ xSpeed
    y = y + ySpeed
    Entity.set_entity_position(entity, x, y)
end

return ball