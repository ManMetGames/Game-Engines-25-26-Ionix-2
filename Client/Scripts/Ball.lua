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
    local function MovePaddleByPixels(entity, deltaPixelsY, deltaPixelsX)
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
        px = px + (deltaPixelsX or 0)

        -- vertical clamp
        local halfH = paddleH * 0.2
        if py < halfH then py = halfH end
        if py > screenH - halfH then py = screenH - halfH end

        -- horizontal clamp: each paddle can move only within its half of the screen
        local halfW = paddleW * 0.5
        local minX, maxX
        if entity == leftPaddle then
            minX = halfW
            maxX = screenW * 0.5 - halfW
        elseif entity == rightPaddle then
            minX = screenW * 0.5 + halfW
            maxX = screenW - halfW
        else
            -- default: full screen, just in case
            minX = halfW
            maxX = screenW - halfW
        end

        if px < minX then px = minX end
        if px > maxX then px = maxX end

        -- write back to physics in meters
        Fysics.set_pos(entity, px_to_m(px), px_to_m(py))
    end
----------------------------------------------------------------
-- Init
----------------------------------------------------------------

local function InitBackground()
    backgroundEntity = Entity.create_entity()
    Entity.set_global_pos(backgroundEntity, 0, 0)
    Entity.add_sprite_component(backgroundEntity, assets.textures.office, screenW, screenH, 0)
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

        local p1MoveY = 0
        local p1MoveX = 0
        local p2MoveY = 0
        local p2MoveX = 0

        if numControllers == 0 then
            -- Player 1: W/S for up/down, A/D for left/right
            if Input.get_key_held(Keys.ionix_w) then p1MoveY = -1 end
            if Input.get_key_held(Keys.ionix_s) then p1MoveY =  1 end
            if Input.get_key_held(Keys.ionix_a) then p1MoveX = -1 end
            if Input.get_key_held(Keys.ionix_d) then p1MoveX =  1 end

            -- Player 2: arrow keys
            if Input.get_key_held(Keys.arrow_up)    then p2MoveY = -1 end
            if Input.get_key_held(Keys.arrow_down)  then p2MoveY =  1 end
            if Input.get_key_held(Keys.arrow_left)  then p2MoveX = -1 end
            if Input.get_key_held(Keys.arrow_right) then p2MoveX =  1 end

        elseif numControllers == 1 then
            -- Player 1 keyboard, player 2 controller
                if Input.get_key_held(Keys.ionix_w) then p1MoveY = -1 end
                if Input.get_key_held(Keys.ionix_s) then p1MoveY =  1 end
                if Input.get_key_held(Keys.ionix_a) then p1MoveX = -1 end
                if Input.get_key_held(Keys.ionix_d) then p1MoveX =  1 end
            
                local c2y = Input.get_left_stick_y(0)
                local c2x = Input.get_left_stick_x(0)
                if Mafs.abs(c2y) > 0.1 then p2MoveY = c2y end
                if Mafs.abs(c2x) > 0.1 then p2MoveX = c2x end

        else
            -- Two controllers
                local c1y = Input.get_left_stick_y(0)
                local c1x = Input.get_left_stick_x(0)
                if Mafs.abs(c1y) > 0.1 then p1MoveY = c1y end
                if Mafs.abs(c1x) > 0.1 then p1MoveX = c1x end
            
                local c2y = Input.get_left_stick_y(1)
                local c2x = Input.get_left_stick_x(1)
                if Mafs.abs(c2y) > 0.1 then p2MoveY = c2y end
                if Mafs.abs(c2x) > 0.1 then p2MoveX = c2x end
        end

        -- Convert input (-1..1) into pixel movement per fixed tick
        -- If stick values are fractional, scale by paddleSpeed
        MovePaddleByPixels(leftPaddle,  p1MoveY * paddleSpeed, p1MoveX * paddleSpeed)
        MovePaddleByPixels(rightPaddle, p2MoveY * paddleSpeed, p2MoveX * paddleSpeed)

        -- Scoring - check sprite position (pixels)
        local ballPos = Fysics.get_pos(ballEntity) -- meters
        local bx = m_to_px(Mafs.get_vec_x(ballPos))

        if bx < -100 or bx > screenW + 100 then
            ResetBall()
        end
    end


return game
