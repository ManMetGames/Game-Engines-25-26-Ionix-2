local ball = {}
local assets = require("Scripts.Assets")

local ballEntity
local Background
local mouseEntity

local speedX = 1
local speedY = -5
local ballSize = 32
local screenW
local screenH

-- Bounciness factor:
-- 1.0  = perfectly elastic (no energy lost)
-- >1.0 = gains a bit of energy on every bounce (very “arcade” / pinball feel)
-- <1.0 = loses energy on every bounce
local BOUNCINESS = 1.05

----------------------------------------------------------------
-- Mouse helper: uses the actual API from InputScripting.cpp
----------------------------------------------------------------
local function getMousePosition()
    -- Uses Input.get_mouse_x() and Input.get_mouse_y() from your engine
    local x = Input.get_mouse_x()
    local y = Input.get_mouse_y()
    -- They should always return numbers, but we guard anyway
    if x == nil or y == nil then
        return -1000, -1000
    end
    return x, y
end

function ballInit()
    ballEntity = Entity.create_entity()
    Entity.set_entity_pos(ballEntity, screenW / 2, screenH / 2)
    Entity.add_sprite_component(ballEntity, assets.textures.PimBall, 64, 64, 100)
    -- 2 = dynamic body in your setup
    Entity.add_fysics_component(ballEntity, 2, false)
    Fysics.add_sprite_collider(ballEntity, false)
    -- Slight gravity so it moves, but not too strong to kill the bouncy effect
    Fysics.set_gravity_scale(ballEntity, 0.05)
end

function BackgroundInnit()
    Background = Entity.create_entity()
    Entity.add_sprite_component(Background, assets.textures.office, 960, 640, 0)
end

-- Create a "cursor body" with collider and sprite that we can move to the mouse
function MouseInit()
    mouseEntity = Entity.create_entity()

    -- Visual + collider size for the mouse "paddle"
    local mouseSize = 32

    -- Start offscreen so it doesn't interfere until we get a real mouse position
    Entity.set_entity_pos(mouseEntity, -1000, -1000)

    -- Visible sprite so you can see the mouse entity.
    -- Layer 200 so it renders above background (0) and ball (100).
    Entity.add_sprite_component(mouseEntity, assets.textures.PimBall, mouseSize, mouseSize, 200)

    -- Static body (0) is fine; we just teleport it each frame
    Entity.add_fysics_component(mouseEntity, 0, false)

    -- Simple box collider around the mouse
    Fysics.add_box_collider(
            mouseEntity,
            mouseSize / 2,
            mouseSize / 2,
            false
    )
end

function WallInnit()
    local floor = Entity.create_entity()
    local ceiling = Entity.create_entity()
    local leftWall = Entity.create_entity()
    local rightWall = Entity.create_entity()

    local wallThickness = 0.1

    -- FLOOR: centered horizontally, just below the bottom edge
    Entity.set_entity_pos(floor, screenW / 2, screenH + wallThickness / 2)
    Entity.add_fysics_component(floor, 0, false)
    Fysics.add_box_collider(
            floor,
            screenW / 2,
            wallThickness / 2,
            false
    )

    -- CEILING: centered horizontally, just above the top edge
    Entity.set_entity_pos(ceiling, screenW / 2, -wallThickness / 2)
    Entity.add_fysics_component(ceiling, 0, false)
    Fysics.add_box_collider(
            ceiling,
            screenW / 2,
            wallThickness / 2,
            false
    )

    -- LEFT WALL: centered vertically, just left of the screen
    Entity.set_entity_pos(leftWall, -wallThickness / 2, screenH / 2)
    Entity.add_fysics_component(leftWall, 0, false)
    Fysics.add_box_collider(
            leftWall,
            wallThickness / 2,
            screenH / 2,
            false
    )

    -- RIGHT WALL: centered vertically, just right of the screen
    Entity.set_entity_pos(rightWall, screenW + wallThickness / 2, screenH / 2)
    Entity.add_fysics_component(rightWall, 0, false)
    Fysics.add_box_collider(
            rightWall,
            wallThickness / 2,
            screenH / 2,
            false
    )
end

function ball:OnStart()
    screenW = Window.get_width()
    screenH = Window.get_height()

    BackgroundInnit()
    WallInnit()
    ballInit()
    MouseInit()

    -- Give it an initial push so you can see the bouncing
    Fysics.set_linear_velocity(ballEntity, speedX, speedY)
end

function ball:OnFixedUpdate()
    local pos = Entity.get_entity_pos(ballEntity)
    local vel = Fysics.get_linear_velocity(ballEntity)

    local vx = vel.x
    local vy = vel.y

    local radius = ballSize / 2

    ------------------------------------------------
    -- Move mouse collider entity to mouse position
    ------------------------------------------------
    local mx, my = getMousePosition()

    -- If mouse is inside the window, move mouse entity to that position.
    -- Otherwise, move it far off-screen so it can't collide with anything.
    if mx >= 0 and mx <= screenW and my >= 0 and my <= screenH then
        Entity.set_entity_pos(mouseEntity, mx, my)
    else
        Entity.set_entity_pos(mouseEntity, -1000, -1000)
    end

    ------------------------------------------------
    -- Screen-edge bouncing (walls, floor, ceiling)
    ------------------------------------------------
    -- Bounce off left/right edges
    if pos.x <= radius then
        pos.x = radius
        vx = -vx * BOUNCINESS
    elseif pos.x >= (screenW - radius) then
        pos.x = screenW - radius
        vx = -vx * BOUNCINESS
    end

    -- Bounce off floor/ceiling
    if pos.y <= radius then
        pos.y = radius
        vy = -vy * BOUNCINESS
    elseif pos.y >= (screenH - radius) then
        pos.y = screenH - radius
        vy = -vy * BOUNCINESS
    end

    -- Apply updated position and velocity
    Entity.set_entity_pos(ballEntity, pos.x, pos.y)
    Fysics.set_linear_velocity(ballEntity, vx, vy)
end

return ball