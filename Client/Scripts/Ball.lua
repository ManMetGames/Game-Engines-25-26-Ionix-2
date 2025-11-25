local ball = {}

local ballEntity
local Background
local speedX = 6
local speedY = 6
local ballSize = 32
local screenW
local screenH

function ballInit()
    print(screenH, screenW)
    ballEntity = Entity.create_entity()
    local sprite = Entity.add_sprite_component(ballEntity, "PimBall", ballSize, ballSize, 0)
    Entity.add_fysics_component(ballEntity, 2, false)
    Fysics.add_circle_collider(ballEntity, ballSize / 2, 0, 0, false)
    print (Entity.get_entity_pos(ballEntity))
    Entity.set_entity_pos(ballEntity, screenW / 2, screenH / 2)
    Fysics.set_pos(ballEntity, screenW / 2, screenH / 2)
    print (Entity.get_entity_pos(ballEntity))
end

function wallInnit()
end
function BackgroundInnit()
    Background = Entity.create_entity()
    local sprite = Entity.add_sprite_component(Background, "Background", screenW, screenH, -1)
end
function ball:OnStart()
    screenW = Window.get_width()
    screenH = Window.get_height()
    
    BackgroundInnit()
    ballInit()
    wallInnit()
end

function ball:OnUpdate()
    Fysics.add_force(ballEntity, speedX, speedY)
end

return ball