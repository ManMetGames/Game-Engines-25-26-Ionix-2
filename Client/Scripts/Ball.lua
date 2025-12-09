local game = {}

local assets = require("Scripts.Assets")

----------------------------------------------------------------
-- Pixel-based world with physics in meters (1 m = 100 px)
----------------------------------------------------------------
local PIXELS_PER_METER = 100

local ballEntity
local backgroundEntity
local leftPaddle
local rightPaddle
local floorEntity
local ceilingEntity

local screenW
local screenH

-- Movement (pixels per fixed update)
local paddleSpeed = 30
local ballSpeed   = 300 -- pixels per second-ish (we convert to m/s when giving velocity)

-- Sprite sizes (pixels)
local ballSize = 128
local paddleW  = 32
local paddleH  = 300

----------------------------------------------------------------
-- Helpers
----------------------------------------------------------------

-- Convert pixels -> meters and meters -> pixels
local function px_to_m(px) return px / PIXELS_PER_METER end
local function m_to_px(m) return m * PIXELS_PER_METER end

-- Center sprite top-left on physics body (physics pos is center in meters)


-- Reset ball to center and give random velocity (converted to meters/sec)
local function ResetBall()
    -- Place physics body at center (meters)
    local centerXM = px_to_m(screenW * 0.5)
    local centerYM = px_to_m(screenH * 0.5)
    Fysics.set_pos(ballEntity, centerXM, centerYM)

    -- random direction
    local dirX = (math.random() > 0.5) and -1 or 1
    local dirY = (math.random() * 2 - 1) * 0.5

    -- convert ballSpeed (pixels/sec) -> meters/sec
    local vx = dirX * (ballSpeed / PIXELS_PER_METER)
    local vy = dirY * (ballSpeed / PIXELS_PER_METER)

    Fysics.set_linear_velocity(ballEntity, vx, vy)
end

-- Move a kinematic paddle using pixel delta -> physics pos
local function MovePaddleByPixels(entity, deltaPixelsY)
    -- Read current physics pos (meters) — authoritative
    local phys = Fysics.get_pos(entity)
    if not phys then
        -- fallback: use transform
        local tr = Entity.get_global_pos(entity)
        local tx = Mafs.get_vec_x(tr)
        local ty = Mafs.get_vec_y(tr)
        local newY = ty + deltaPixelsY
        -- clamp in pixels
        local halfH = paddleH * 0.5
        if newY < halfH then newY = halfH end
        if newY > screenH - halfH then newY = screenH - halfH end
        Entity.set_global_pos(entity, tx, newY)
        return
    end

    local physX = Mafs.get_vec_x(phys)
    local physY = Mafs.get_vec_y(phys)

    -- convert to pixels, modify, clamp, convert back
    local px = m_to_px(physX)
    local py = m_to_px(physY)

    py = py + deltaPixelsY

    local halfH = paddleH * 0.2
    if py < halfH then py = halfH end
    if py > screenH - halfH then py = screenH - halfH end

    -- write back to physics in meters
    Fysics.set_pos(entity, px_to_m(px), px_to_m(py))
end

----------------------------------------------------------------
-- Init
----------------------------------------------------------------

local function InitBackground()
    backgroundEntity = Entity.create_entity()
    Entity.set_global_pos(backgroundEntity, 0, 0)
    Entity.add_sprite_component(backgroundEntity, assets.textures.office, 1920, 1080, 0)
end

local function InitBall()
    ballEntity = Entity.create_entity()

    -- **Important**: set entity transform (pixels) first so the FysicsBody uses correct initial pos
    Entity.set_global_pos(ballEntity, screenW * 0.5, screenH * 0.5)

    Entity.add_sprite_component(ballEntity, assets.textures.PimBall, ballSize, ballSize, 100)
    Entity.add_fysics_component(ballEntity, 2, false) -- dynamic

    local halfW = px_to_m(ballSize * 0.5)
    local halfH = px_to_m(ballSize * 0.5)

    -- centered collider (offset 0,0 is body center)
    Fysics.add_circle_collider(ballEntity, halfW, 0.5, 0.5, false)
    --Fysics.add_sprite_collider(ballEntity, false)

    Fysics.set_material_properties(ballEntity, 0.0, 1.1)
    Fysics.set_gravity_scale(ballEntity, 0)
end

local function InitPaddles()
    -- Create left paddle
    leftPaddle = Entity.create_entity()
    -- position entity transform (pixels) — center of sprite desired
    Entity.set_global_pos(leftPaddle, 50, screenH * 0.5)
    Entity.add_sprite_component(leftPaddle, assets.textures.LaptopBattery, paddleW, paddleH, 100)
    Entity.add_fysics_component(leftPaddle, 1, true) -- kinematic

    local halfWx = px_to_m(paddleW * 0.5)
    local halfHy = px_to_m(paddleH * 0.5)
    Fysics.add_sprite_collider(leftPaddle,false)

    -- Create right paddle
    rightPaddle = Entity.create_entity()
    Entity.set_global_pos(rightPaddle, screenW - 50, screenH * 0.5)
    Entity.add_sprite_component(rightPaddle, assets.textures.LaptopBattery, paddleW, paddleH, 100)
    Entity.add_fysics_component(rightPaddle, 1, true)

    Fysics.add_sprite_collider(rightPaddle,false)
end

local function InitWalls()
    local wallThicknessPx = 20
    local halfT = px_to_m(wallThicknessPx * 0.5)

    -- Bottom wall (place slightly off-screen)
    floorEntity = Entity.create_entity()
    Entity.set_global_pos(floorEntity, screenW * 0.5, screenH + wallThicknessPx * 0.5)
    Entity.add_fysics_component(floorEntity, 1, false)
    Fysics.add_box_collider(floorEntity, px_to_m(screenW), halfT, 0, 0, 0, false)

    -- Top wall
    ceilingEntity = Entity.create_entity()
    Entity.set_global_pos(ceilingEntity, screenW * 0.5, -wallThicknessPx * 0.5)
    Entity.add_fysics_component(ceilingEntity, 1, false)
    Fysics.add_box_collider(ceilingEntity, px_to_m(screenW), halfT, 0, 0, 0, false)
end

----------------------------------------------------------------
-- Game start / fixed update
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
    -- Query controller count *each frame* (avoid stale value)
    local numControllers = Input.get_controller_count()

    local p1Move = 0
    local p2Move = 0

    if numControllers == 0 then
        if Input.get_key_held(Keys.ionix_w) then p1Move = -1 end
        if Input.get_key_held(Keys.ionix_s) then p1Move =  1 end

        if Input.get_key_held(Keys.arrow_up) then p2Move = -1 end
        if Input.get_key_held(Keys.arrow_down) then p2Move =  1 end

    elseif numControllers == 1 then
        -- Player 1 keyboard, player 2 controller
        if Input.get_key_held(Keys.ionix_w) then p1Move = -1 end
        if Input.get_key_held(Keys.ionix_s) then p1Move =  1 end

        local c2 = Input.get_left_stick_y(0)
        if Mafs.abs(c2) > 0.1 then p2Move = c2 end

    else
        local c1 = Input.get_left_stick_y(0)
        if Mafs.abs(c1) > 0.1 then p1Move = c1 end

        local c2 = Input.get_left_stick_y(1)
        if Mafs.abs(c2) > 0.1 then p2Move = c2 end
    end

    -- Convert input (-1..1) into pixel movement per fixed tick
    -- If stick values are fractional, scale by paddleSpeed
    MovePaddleByPixels(leftPaddle,  p1Move * paddleSpeed)
    MovePaddleByPixels(rightPaddle, p2Move * paddleSpeed)

    -- Scoring - check sprite position (pixels)
    local ballPos = Fysics.get_pos(ballEntity) -- meters
    local bx = m_to_px(Mafs.get_vec_x(ballPos))

    if bx < -100 or bx > screenW + 100 then
        ResetBall()
    end
end

return game
