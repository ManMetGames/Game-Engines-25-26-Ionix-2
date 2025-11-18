local ball = {}
local xSpeed = 2
local ySpeed = 2
local x
local y
local screenWidth
local screenHeight
function ball:OnStart()
    screenWidth = Window.get_width()
    screenHeight = Window.get_height()
    local entity = Entity.create_entity()
    print(screenWidth .. " " .. screenHeight)
    Entity.add_sprite_component(entity, "PimBall", 32, 32,255)
    x = screenWidth / 2
    y = screenHeight / 2
    Entity.set_entity_pos(entity, x, y)
end
function ball:onUpdate()
    
    if y >= screenHeight-32 then
        ySpeed = -ySpeed
    elseif  y <= 32 then
        ySpeed = ySpeed
    else
        error("lmao wtf are you doing (error in Ball.lua, Y position out of bounds)")
    end
    y = y + ySpeed
    Entity.set_entity_pos(entity, x, y)
end

return ball