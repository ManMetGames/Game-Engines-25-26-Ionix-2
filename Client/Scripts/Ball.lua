local ball = {}

local ballEntity
local speedX = 6
local speedY = 6
local ballSize = 32
local screenW
local screenH

function ballInit()
    print(screenH, screenW)
    ballEntity = Entity.create_entity()
    Entity.set_entity_pos(ballEntity, screenW / 2, screenH / 2)
    local sprite = Entity.add_sprite_component(ballEntity, "PimBall", ballSize, ballSize, 0)
    Entity.add_fysics_component(ballEntity, 2, false)
    Fysics.add_sprite_collider(ballEntity, false)
end
function wallInnit()
    
end

function ball:OnStart()
     screenW = Window.get_width()
     screenH = Window.get_height()
    ballInit()
    wallInnit() 
end
function ball:OnUpdate()
    
end

return ball
