-- Scripts/FlappyBird/Map.lua
local assets = require("Scripts.Assets")
local enums  = require("Scripts.Enums")

local Map = {}

function Map.create_level1()
    -- background
    local Background = Entity.create_entity()
    Entity.add_sprite_component(Background, assets.textures.Background, 1920, 1080, 0)

    --------------------------------------------------
    -- MAIN FLOOR PLATFORM
    --------------------------------------------------
    local floorY      = 550
    local floorWidth  = 900
    local floorHeight = 50

    local tile = Entity.create_entity()
    Entity.set_global_pos(tile, 500, floorY)
    Entity.add_sprite_component(tile, assets.textures.Sand, floorWidth, floorHeight, 1)
    Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)
    Fysics.add_sprite_collider(tile, false, 1)

    --------------------------------------------------
    -- FLOATING PLATFORM
    --------------------------------------------------
    local platform1 = Entity.create_entity()
    Entity.set_global_pos(platform1, 500, 380)
    Entity.add_sprite_component(platform1, assets.textures.Sand, 350, 18, 1)
    Entity.add_fysics_component(platform1, enums.bodytype.staticBody, false)
    Fysics.add_edge_collider(platform1, 20, 30, 50, 30, false)

    local groundLineY = floorY - floorHeight

    return Background, tile, platform1, groundLineY
end

return Map
