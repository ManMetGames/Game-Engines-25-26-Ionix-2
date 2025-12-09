local Peggle = {}

local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

local ball

function Peggle:OnStart()
    ball = Entity.create_entity()
    Entity.add_sprite_component(ball, assets.textures.PimBall, 50, 50, 0)
end

return Peggle