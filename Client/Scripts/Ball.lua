local game = {}

local assets = require("Scripts.Assets")

----------------------------------------------------------------
-- World / Game Space
----------------------------------------------------------------
-- Normalized world size: X and Y in [0, 1]
local GAME_W  = 96
local GAME_H  = 54

-- How far in from the top/bottom the play area is
local verticalPadding = 0.01

----------------------------------------------------------------
-- Entities
----------------------------------------------------------------
local ballEntity
local backgroundEntity
local leftPaddle
local rightPaddle
local floorEntity
local ceilingEntity

----------------------------------------------------------------
-- Game Settings
----------------------------------------------------------------
local screenW
local screenH
local paddleSpeed  = 2  -- movement per fixed update (world units)
local ballSpeed    = 2  -- initial ball speed (world units)
local paddleOffset = 0.5  -- distance from left/right edges in world units

-- Sizes
local ballSize  = 128       -- sprite size in pixels (visual only)
local paddleW   = 0.5     -- world units (physics + visual)
local paddleH   = 4     -- world units (physics + visual)

----------------------------------------------------------------
-- Helpers
----------------------------------------------------------------

-- Reset ball to center with random direction
local function ResetBall()
    if not ballEntity or not screenW or not screenH then
        return
    end

    -- Center of normalized world
    Entity.set_global_pos(ballEntity, screenW * 0.5, screenH * 0.5)

    -- Randomize start direction (Left or Right)
    local dirX = (math.random() > 0.5) and -1 or 1

    -- Randomize angle slightly so it's not boring
    local dirY = (math.random() * 2 - 1) * 0.5 -- between -0.5 and 0.5

    -- Apply velocity in world units
    Fysics.set_linear_velocity(ballEntity, dirX * ballSpeed, dirY * ballSpeed)
end

-- Helper to move paddle and clamp to screen
local function UpdatePaddle(entity, inputDir)
    if not entity or not screenH then
        return
    end

    local currentPos = Entity.get_global_pos(entity)
    local px = Mafs.get_vec_x(currentPos)
    local py = Mafs.get_vec_y(currentPos)

    -- Apply movement
    py = py + (inputDir * paddleSpeed)

    -- Clamp to screen top/bottom in WORLD units, with padding
    local halfH   = paddleH * 0.5
    local topY    = verticalPadding + halfH
    local bottomY = screenH - verticalPadding - halfH

    if py < topY then
        py = topY
    end
    if py > bottomY then
        py = bottomY
    end

    -- Apply new position
    Entity.set_global_pos(entity, px, py)

    -- Reset velocity to 0 so physics doesn't drift it
    Fysics.set_linear_velocity(entity, 0, 0)
end

----------------------------------------------------------------
-- Initializers
----------------------------------------------------------------

local function InitBackground()
    backgroundEntity = Entity.create_entity()
    if not backgroundEntity then
        return
    end

    -- Centered in world space (0.5, 0.5)
    Entity.set_global_pos(backgroundEntity, 0, 0)

    -- Visual size in pixels
    Entity.add_sprite_component(backgroundEntity, assets.textures.office, 1920, 1080, 0)
end

local function InitBall()
    ballEntity = Entity.create_entity()
    if not ballEntity then
        return
    end
    Entity.set_global_pos(ballEntity, screenW/2, screenH/2)
    -- Add Sprite
    Entity.add_sprite_component(ballEntity, assets.textures.PimBall, ballSize, ballSize, 100)

    -- Physics: Type 2 (Dynamic) so it bounces
    Entity.add_fysics_component(ballEntity, 2, true) -- true = rotation locked

    -- Box collider in world units
    local ballHalfSize = 0.9
    Fysics.add_box_collider(
        ballEntity,
        ballHalfSize,  -- half-width (world units)
        ballHalfSize,  -- half-height (world units)
        0.5,             -- offset X
        0.5,             -- offset Y
        0,             -- rotation
        false          -- isTrigger
    )

    -- Bouncy material
    Fysics.set_material_properties(ballEntity, 0.0, 1.0)

    -- No gravity for Pong
    Fysics.set_gravity_scale(ballEntity, 0)
end

local function InitPaddles()
    -- === Left Paddle (Player 1) ===
    leftPaddle = Entity.create_entity()
    if not leftPaddle then
        return
    end

    Entity.set_global_pos(leftPaddle, paddleOffset, screenH * 0.5)

    -- Visual sprite scaled to world units (renderer interprets size appropriately)
    Entity.add_sprite_component(leftPaddle, assets.textures.PimBall, paddleW, paddleH, 100)

    -- Physics: Type 1 (Kinematic)
    Entity.add_fysics_component(leftPaddle, 1, true)
    Fysics.add_box_collider(leftPaddle, paddleW * 0.5, paddleH * 0.5, 0, 0, 0, false)

    -- === Right Paddle (Player 2) ===
    rightPaddle = Entity.create_entity()
    if not rightPaddle then
        return
    end

    Entity.set_global_pos(rightPaddle, screenW - paddleOffset, screenH * 0.5)
    Entity.add_sprite_component(rightPaddle, assets.textures.PimBall, paddleW, paddleH, 100)
    Entity.add_fysics_component(rightPaddle, 1, true)
    Fysics.add_box_collider(rightPaddle, paddleW * 0.5, paddleH * 0.5, 0, 0, 0, false)
end

local function InitWalls()
    -- Top and Bottom walls in normalized space
    local wallThickness = 0.01

    local playTop    = verticalPadding
    local playBottom = screenH - verticalPadding

    -- Floor (bottom wall)
    floorEntity = Entity.create_entity()
    if floorEntity then
        local floorY = playBottom + wallThickness * 0.5
        Entity.set_global_pos(floorEntity, screenW * 0.5, floorY)
        Entity.add_fysics_component(floorEntity, 1, false)
        Fysics.add_box_collider(floorEntity, screenW * 0.5, wallThickness * 0.5, 0, 0, 0, false)
    end

    -- Ceiling (top wall)
    ceilingEntity = Entity.create_entity()
    if ceilingEntity then
        local ceilY = playTop - wallThickness * 0.5
        Entity.set_global_pos(ceilingEntity, screenW * 0.5, ceilY)
        Entity.add_fysics_component(ceilingEntity, 1, false)
        Fysics.add_box_collider(ceilingEntity, screenW * 0.5, wallThickness * 0.5, 0, 0, 0, false)
    end
end

----------------------------------------------------------------
-- Game Loop
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

function game:OnFixedUpdate()
    ------------------------------------------------
    -- 1. Handle Player 1 Input (Left Paddle)
    ------------------------------------------------
    local p1MoveY = 0

    if Input.get_key_held(Keys.ionix_w) then
        p1MoveY = -1
    elseif Input.get_key_held(Keys.ionix_s) then
        p1MoveY = 1
    end

    local c1Stick = Input.get_left_stick_y(0)
    if Mafs.abs(c1Stick) > 0.1 then
        p1MoveY = c1Stick
    end

    ------------------------------------------------
    -- 2. Handle Player 2 Input (Right Paddle)
    ------------------------------------------------
    local p2MoveY = 0

    local c2Stick = Input.get_left_stick_y(1)
    if Mafs.abs(c2Stick) > 0.1 then
        p2MoveY = c2Stick
    end

    ------------------------------------------------
    -- 3. Move Paddles (Kinematic Movement)
    ------------------------------------------------
    UpdatePaddle(leftPaddle,  p1MoveY)
    UpdatePaddle(rightPaddle, p2MoveY)

    ------------------------------------------------
    -- 4. Scoring Logic (Reset Ball)
    ------------------------------------------------
    if not ballEntity then
        return
    end

    local ballPos = Entity.get_global_pos(ballEntity)
    local bx = Mafs.get_vec_x(ballPos)

    -- Left/right off-screen in normalized space
    if bx < -0.1 then
        ResetBall()
    end

    if bx > screenW + 0.1 then
        ResetBall()
    end

    local _bVel = Fysics.get_linear_velocity(ballEntity)
end

return game