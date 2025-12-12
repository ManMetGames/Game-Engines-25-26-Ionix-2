local assets = require("Scripts.Assets")

local Death = {}

Death.minX = -200
Death.maxX = 1920 + 200
Death.minY = -200
Death.maxY = 1080 + 200

Death.visualThickness = 28
Death.visualPadding   = 0

local visuals = { left=nil, right=nil, top=nil, bottom=nil }

function Death.set_bounds(minX, maxX, minY, maxY)
    Death.minX, Death.maxX, Death.minY, Death.maxY = minX, maxX, minY, maxY
end

function Death.create_visuals()
    if visuals.left then return end

    local t = Death.visualThickness
    local pad = Death.visualPadding

    -- Left border
    visuals.left = Entity.create_entity()
    Entity.add_sprite_component(visuals.left, assets.textures.Sand, t, 1080 - pad*2, 2)
    Sprite.set_color(Entity.get_sprite_component(visuals.left), 255, 0, 0)
    Entity.set_global_pos(visuals.left, 0 + pad, 540)

    -- Right border
    visuals.right = Entity.create_entity()
    Entity.add_sprite_component(visuals.right, assets.textures.Sand, t, 1080 - pad*2, 2)
    Sprite.set_color(Entity.get_sprite_component(visuals.right), 255, 0, 0)
    Entity.set_global_pos(visuals.right, 1920 - pad, 540)

    -- Top border
    visuals.top = Entity.create_entity()
    Entity.add_sprite_component(visuals.top, assets.textures.Sand, 1920 - pad*2, t, 2)
    Sprite.set_color(Entity.get_sprite_component(visuals.top), 255, 0, 0)
    Entity.set_global_pos(visuals.top, 960, 0 + pad)

    -- Bottom border
    visuals.bottom = Entity.create_entity()
    Entity.add_sprite_component(visuals.bottom, assets.textures.Sand, 1920 - pad*2, t, 2)
    Sprite.set_color(Entity.get_sprite_component(visuals.bottom), 255, 0, 0)
    Entity.set_global_pos(visuals.bottom, 960, 1080 - pad)
end

function Death.is_out_of_bounds(entity)
    if not entity then return false end
    local p = Entity.get_global_pos(entity)
    local x = Mafs.get_vec_x(p)
    local y = Mafs.get_vec_y(p)

    return (x < Death.minX) or (x > Death.maxX) or (y < Death.minY) or (y > Death.maxY)
end

return Death
