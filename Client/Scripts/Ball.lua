local ball = {}
local assets = require("Scripts.Assets")

local ballEntity
local Background
local speedX = 6
local speedY = 6
local ballSize = 32
local screenW
local screenH

function ballInit()
    ballEntity = Entity.create_entity()
    Entity.set_entity_pos(ballEntity, screenW / 2, screenH / 2)
    Entity.add_sprite_component(ballEntity, assets.textures.PimBall, 0, 0, 100)
    Entity.add_fysics_component(ballEntity, 2, false)
    Fysics.add_sprite_collider(ballEntity, false)
    print("Ball Initialized at pos" .. Entity.get_entity_pos)
   
end

function BackgroundInnit()
    Background = Entity.create_entity()
    Entity.add_sprite_component(Background, assets.textures.Background, 0, 0, 0)
end
function WallInnit()
    local floor = Entity.create_entity()
    local ceiling = Entity.create_entity()
    local leftWall = Entity.create_entity()
    local rightWall = Entity.create_entity() 
    Entity.set_entity_pos(floor, screenW / 2, screenH + 10)
    Entity.set_entity_pos(ceiling, screenW / 2, -10)
    Entity.set_entity_pos(leftWall, -10, screenH / 2)
    Entity.set_entity_pos(rightWall, screenW + 10, screenH / 2)
    Entity.add_fysics_component(floor, 0, false)
    Entity.add_fysics_component(ceiling, 0, false)
    Entity.add_fysics_component(leftWall, 0, false)
    Entity.add_fysics_component(rightWall, 0, false)
    Fysics.add_sprite_collider(floor, false)
    Fysics.add_sprite_collider(ceiling, false)
    Fysics.add_sprite_collider(leftWall, false)
    Fysics.add_sprite_collider(rightWall, false)

end

function ball:OnStart()
    screenW = Window.get_width()
    screenH = Window.get_height()

    BackgroundInnit()
    WallInnit()
    ballInit()
    Fysics.add_impulse(ballEntity, screenW/2, screenH/2,speedX,speedY)
end

function ball:OnUpdate()
    local vel1 = Fysics.get_linear_velocity(ballEntity)
    local vx = 0
    local vy1 = vel1.y
    Fysics.set_linear_velocity(ballEntity, vx, vy1)
end

return ball