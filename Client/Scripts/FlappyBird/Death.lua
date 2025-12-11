-- Scripts/FlappyBird/Death.lua
local Death = {}

local bounds = {
    left   = -200,
    right  = 2120,
    top    = -200,
    bottom = 1280
}

local visuals = {}

function Death.set_bounds(left, right, top, bottom)
    bounds.left   = left
    bounds.right  = right
    bounds.top    = top
    bounds.bottom = bottom

    -- LEFT WALL
    visuals.left = Entity.create_entity()
    Entity.set_global_pos(visuals.left, bounds.left, 540)
    Entity.add_sprite_component(visuals.left, "Assets/Textures/white.png", 10, 1200, 200)
    Sprite.set_color(Entity.get_sprite_component(visuals.left), 255, 0, 0)

    -- RIGHT WALL
    visuals.right = Entity.create_entity()
    Entity.set_global_pos(visuals.right, bounds.right, 540)
    Entity.add_sprite_component(visuals.right, "Assets/Textures/white.png", 10, 1200, 200)
    Sprite.set_color(Entity.get_sprite_component(visuals.right), 255, 0, 0)

    -- TOP WALL
    visuals.top = Entity.create_entity()
    Entity.set_global_pos(visuals.top, 960, bounds.top)
    Entity.add_sprite_component(visuals.top, "Assets/Textures/white.png", 2200, 10, 200)
    Sprite.set_color(Entity.get_sprite_component(visuals.top), 255, 0, 0)

    -- BOTTOM WALL
    visuals.bottom = Entity.create_entity()
    Entity.set_global_pos(visuals.bottom, 960, bounds.bottom)
    Entity.add_sprite_component(visuals.bottom, "Assets/Textures/white.png", 2200, 10, 200)
    Sprite.set_color(Entity.get_sprite_component(visuals.bottom), 255, 0, 0)
end

function Death.is_out_of_bounds(entity)
    if not entity then return false end

    local pos = Entity.get_global_pos(entity)
    local x   = Mafs.get_vec_x(pos)
    local y   = Mafs.get_vec_y(pos)

    return (
        x < bounds.left or
        x > bounds.right or
        y < bounds.top or
        y > bounds.bottom
    )
end

return Death
