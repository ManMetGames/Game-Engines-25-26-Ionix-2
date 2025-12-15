local game = {}

local assets = require("Scripts.Assets")

----------------------------------------------------------------
-- Constants
----------------------------------------------------------------
local PIXELS_PER_METER = 100
local FIXED_DT = 1 / 60

local paddleSpeed = 6
local ballSpeed   = 300

local ballSize = 128
local paddleW  = 32
local paddleH  = 300

----------------------------------------------------------------
-- Screen
----------------------------------------------------------------
local screenW, screenH

----------------------------------------------------------------
-- Arena
----------------------------------------------------------------
local arenaTopMargin    = 10
local arenaBottomMargin = 10
local wallThickness    = 20
local sideWallWidth    = 30

----------------------------------------------------------------
-- Entities
----------------------------------------------------------------
local background
local ball
local leftPaddle
local rightPaddle
local ceiling
local floor
local leftWall
local rightWall

----------------------------------------------------------------
-- Shared input state
----------------------------------------------------------------
local input_p1x, input_p1y = 0, 0
local input_p2x, input_p2y = 0, 0

----------------------------------------------------------------
-- Helpers
----------------------------------------------------------------
local function px_to_m(px) return px / PIXELS_PER_METER end
local function Clamp(v, min, max)
    if v < min then return min end
    if v > max then return max end
    return v
end

----------------------------------------------------------------
-- Ball
----------------------------------------------------------------
local function ResetBall()
    local cx = screenW * 0.5 - ballSize * 0.5
    local cy = screenH * 0.5 - ballSize * 0.5

    -- Move the ENTITY (this controls rendering)
    Entity.set_global_pos(ball, cx, cy)

    -- Sync physics to entity (meters)
    Fysics.set_pos(
            ball,
            px_to_m(cx + ballSize * 0.5),
            px_to_m(cy + ballSize * 0.5)
    )

    local dirX = (math.random() > 0.5) and -1 or 1
    local dirY = math.random() * 2 - 1

    Fysics.set_linear_velocity(
            ball,
            dirX * (ballSpeed / PIXELS_PER_METER),
            dirY * (ballSpeed / PIXELS_PER_METER)
    )
end

----------------------------------------------------------------
-- Paddle movement (non-physics, screen-space)
----------------------------------------------------------------
local function MovePaddle(entity, dx, dy)
    if not entity then return end

    dx = dx or 0
    dy = dy or 0

    local pos = Entity.get_global_pos(entity)
    if not pos then return end

    local x = Mafs.get_vec_x(pos) + dx
    local y = Mafs.get_vec_y(pos) + dy

    local minY = arenaTopMargin + wallThickness
    local maxY = screenH - arenaBottomMargin - paddleH - wallThickness
    y = Clamp(y, minY, maxY)

    local minX, maxX
    if entity == leftPaddle then
        minX = sideWallWidth
        maxX = screenW * 0.5 - paddleW
    else
        minX = screenW * 0.5
        maxX = screenW - sideWallWidth - paddleW
    end
    x = Clamp(x, minX, maxX)

    Entity.set_global_pos(entity, x, y)
end

----------------------------------------------------------------
-- Init
----------------------------------------------------------------
local function InitBackground()
    background = Entity.create_entity()
    Entity.set_global_pos(background, 0, 0)
    Entity.add_sprite_component(background, assets.textures.office, screenW, screenH, 0)
end

local function InitBall()
    ball = Entity.create_entity()

    Entity.add_sprite_component(
            ball,
            assets.textures.PimBall,
            ballSize,
            ballSize,
            10
    )

    Entity.add_fysics_component(ball, 2, false)

    Fysics.add_circle_collider(
            ball,
            px_to_m(ballSize * 0.5),
            0, 0,
            false
    )

    Fysics.set_gravity_scale(ball, 0)
    Fysics.set_pos(
            ball,
            px_to_m(screenW * 0.5),
            px_to_m(screenH * 0.5)
    )
end

local function InitPaddles()
    leftPaddle = Entity.create_entity()
    Entity.set_global_pos(leftPaddle, sideWallWidth + 10, screenH * 0.5 - paddleH * 0.5)
    Entity.add_sprite_component(leftPaddle, assets.textures.LaptopBattery, paddleW, paddleH, 10)

    rightPaddle = Entity.create_entity()
    Entity.set_global_pos(
            rightPaddle,
            screenW - sideWallWidth - paddleW - 10,
            screenH * 0.5 - paddleH * 0.5
    )
    Entity.add_sprite_component(rightPaddle, assets.textures.LaptopBattery, paddleW, paddleH, 10)
end

local function InitWalls()
    -- Top wall
    ceiling = Entity.create_entity()
    Entity.set_global_pos(ceiling, 0, arenaTopMargin)
    Entity.add_sprite_component(ceiling, assets.textures.Wall, screenW, wallThickness, 5)
    Entity.add_fysics_component(ceiling, 0, false)
    Fysics.add_box_collider(
            ceiling,
            px_to_m(screenW),
            px_to_m(wallThickness * 0.5),
            0, 0, 0, false
    )

    -- Bottom wall
    floor = Entity.create_entity()
    Entity.set_global_pos(
            floor,
            0,
            screenH - arenaBottomMargin - wallThickness
    )
    Entity.add_sprite_component(floor, assets.textures.Wall, screenW, wallThickness, 5)
    Entity.add_fysics_component(floor, 0, false)
    Fysics.add_box_collider(
            floor,
            px_to_m(screenW),
            px_to_m(wallThickness * 0.5),
            0, 0, 0, false
    )

    -- Left wall
    leftWall = Entity.create_entity()
    Entity.set_global_pos(leftWall, 0, arenaTopMargin + wallThickness)
    Entity.add_sprite_component(
            leftWall,
            assets.textures.Wall,
            sideWallWidth,
            screenH - arenaTopMargin - arenaBottomMargin - wallThickness * 2,
            5
    )
    Entity.add_fysics_component(leftWall, 0, false)
    Fysics.add_box_collider(
            leftWall,
            px_to_m(sideWallWidth * 0.5),
            px_to_m((screenH - arenaTopMargin - arenaBottomMargin) * 0.5),
            0, 0, 0, false
    )

    -- Right wall
    rightWall = Entity.create_entity()
    Entity.set_global_pos(
            rightWall,
            screenW - sideWallWidth,
            arenaTopMargin + wallThickness
    )
    Entity.add_sprite_component(
            rightWall,
            assets.textures.Wall,
            sideWallWidth,
            screenH - arenaTopMargin - arenaBottomMargin - wallThickness * 2,
            5
    )
    Entity.add_fysics_component(rightWall, 0, false)
    Fysics.add_box_collider(
            rightWall,
            px_to_m(sideWallWidth * 0.5),
            px_to_m((screenH - arenaTopMargin - arenaBottomMargin) * 0.5),
            0, 0, 0, false
    )
end

----------------------------------------------------------------
-- Lifecycle
----------------------------------------------------------------
function game:OnStart()
    screenW = Window.get_width()
    screenH = Window.get_height()

    InitBackground()
    InitWalls()
    InitBall()
    InitPaddles()
    ResetBall()
end

function game:OnUpdate()
    input_p1x, input_p1y = 0, 0
    input_p2x, input_p2y = 0, 0

    if Input.get_key_held(Keys.ionix_w) then input_p1y = -paddleSpeed end
    if Input.get_key_held(Keys.ionix_s) then input_p1y =  paddleSpeed end
    if Input.get_key_held(Keys.ionix_a) then input_p1x = -paddleSpeed end
    if Input.get_key_held(Keys.ionix_d) then input_p1x =  paddleSpeed end

    if Input.get_key_held(Keys.arrow_up)    then input_p2y = -paddleSpeed end
    if Input.get_key_held(Keys.arrow_down)  then input_p2y =  paddleSpeed end
    if Input.get_key_held(Keys.arrow_left)  then input_p2x = -paddleSpeed end
    if Input.get_key_held(Keys.arrow_right) then input_p2x =  paddleSpeed end
end

function game:OnFixedUpdate()
    MovePaddle(leftPaddle,  input_p1x, input_p1y)
    MovePaddle(rightPaddle, input_p2x, input_p2y)
end

return game
