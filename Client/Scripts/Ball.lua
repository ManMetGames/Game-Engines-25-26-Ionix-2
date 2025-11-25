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

    Entity.add_sprite_component(ballEntity, assets.textures.PimBall, 0, 0, 0)
    Entity.set_entity_pos(ballEntity, screenW / 2, screenH / 2)
    Entity.add_fysics_component(ballEntity, 2, false)
    Fysics.add_circle_collider(ballEntity, ballSize / 2, 0, 0, false)
end

function BackgroundInnit()
    Background = Entity.create_entity()
    Entity.add_sprite_component(Background, assets.textures.Background, 0, 0, -1)
end

function ball:OnStart()
    screenW = Window.get_width()
    screenH = Window.get_height()

    BackgroundInnit()
    ballInit()
    Fysics.add_impulse(ballEntity, screenW/2, screenH/2,speedX,speedY)
end

function ball:OnUpdate()
end

return ball