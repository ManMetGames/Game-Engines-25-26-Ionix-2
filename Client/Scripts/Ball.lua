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
    local sprite = Entity.add_sprite_component(ballEntity, "PimBall", 0, 0, 0)
    Entity.add_fysics_component(ballEntity, 2, false)
    print(Entity.get_entity_pos(ballEntity))
    Entity.set_entity_pos(ballEntity, screenW / 2, screenH / 2)
    print(Entity.get_entity_pos(ballEntity))
end

function BackgroundInnit()
    Background = Entity.create_entity()
    Entity.add_sprite_component(Background, "Background", 0, 0, -1)
end

function ball:OnStart()
    screenW = Window.get_width()
    screenH = Window.get_height()
    BackgroundInnit()
    ballInit()
end

function ball:OnUpdate()
    local pos = Entity.get_entity_pos(ballEntity)
    pos.x = pos.x + speedX
    pos.y = pos.y + speedY
    Entity.set_entity_pos(ballEntity, pos.x, pos.y)
end

return ball
