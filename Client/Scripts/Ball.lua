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
local leftTopWall
local leftBottomWall
local rightTopWall
local rightBottomWall
local boingEntity  

local screenW
local screenH

-- Movement (pixels per fixed update)
local paddleSpeed = 3
local ballSpeed   = 300 -- pixels per second-ish (we convert to m/s when giving velocity)

-- Sprite sizes (pixels)
local ballSize = 128
local paddleW  = 32
local paddleH  = 300

local previousPaddlePos = {}  -- cache per-entity previous positions in pixels
local FIXED_DT = 1 / 60       -- use your engine’s actual fixed timestep if different

----------------------------------------------------------------
-- Helpers
----------------------------------------------------------------

-- Convert pixels -> meters and meters -> pixels
local function px_to_m(px) return px / PIXELS_PER_METER end
local function m_to_px(m) return m * PIXELS_PER_METER end

-- Center sprite top-left on physics body (physics pos is center in meters)
local scoreLeft  = 0
local scoreRight = 0


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


local function MovePaddleByPixels(entity, deltaPixelsY, deltaPixelsX)
    deltaPixelsX = deltaPixelsX or 0

    -- current physics pos (meters)
    local phys = Fysics.get_pos(entity)
    if not phys then
        return
    end

    local physX = Mafs.get_vec_x(phys)
    local physY = Mafs.get_vec_y(phys)

    local px = m_to_px(physX)
    local py = m_to_px(physY)

    -- store previous pixel position for velocity calculation
    local prev = previousPaddlePos[entity]
    if not prev then
        prev = { x = px, y = py }
        previousPaddlePos[entity] = prev
    end

    -- early‑out: no input → no velocity
    if deltaPixelsX == 0 and deltaPixelsY == 0 then
        -- keep position as is, but make sure velocity is zero
        Fysics.set_linear_velocity(entity, 0, 0)
        -- sync cache so we don't invent motion next frame
        prev.x = px
        prev.y = py
        return
    end

    -- apply input in pixels
    local newPx = px + deltaPixelsX
    local newPy = py + deltaPixelsY

    -- vertical clamp
    local halfH = paddleH * 0.2
    if newPy < halfH then newPy = halfH end
    if newPy > screenH - halfH then newPy = screenH - halfH end

    -- horizontal clamp per side
    local halfW = paddleW * 0.5
    local minX, maxX
    if entity == leftPaddle then
        minX = halfW
        maxX = screenW * 0.5 - halfW
    elseif entity == rightPaddle then
        minX = screenW * 0.5 + halfW
        maxX = screenW - halfW
    else
        minX = halfW
        maxX = screenW - halfW
    end

    if newPx < minX then newPx = minX end
    if newPx > maxX then newPx = maxX end

    -- if clamping means we didn't actually move, kill velocity
    if newPx == prev.x and newPy == prev.y then
        Fysics.set_pos(entity, px_to_m(newPx), px_to_m(newPy))
        Fysics.set_linear_velocity(entity, 0, 0)
        prev.x = newPx
        prev.y = newPy
        return
    end

    -- compute velocity in meters/second from pixel displacement over fixed dt
    local vx_px = (newPx - prev.x) / FIXED_DT
    local vy_px = (newPy - prev.y) / FIXED_DT

    local vx_m = vx_px / PIXELS_PER_METER
    local vy_m = vy_px / PIXELS_PER_METER

    -- write back position (meters)
    Fysics.set_pos(entity, px_to_m(newPx), px_to_m(newPy))

    -- tell physics the paddle’s velocity
    Fysics.set_linear_velocity(entity, vx_m, vy_m)

    -- update cache
    prev.x = newPx
    prev.y = newPy
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
    
    Entity.set_global_pos(ballEntity, screenW * 0.5, screenH * 0.5)

    Entity.add_sprite_component(ballEntity, assets.textures.PimBall, ballSize, ballSize, 100)
    Entity.add_fysics_component(ballEntity, 2, false) -- dynamic

    local halfW = px_to_m(ballSize * 0.5)
    local halfH = px_to_m(ballSize * 0.5)

    -- centered collider (offset 0,0 is body center)
    Fysics.add_circle_collider(ballEntity, halfW, 0.5, 0.5, false)
    --Fysics.add_sprite_collider(ballEntity, false)

    Fysics.set_material_properties(ballEntity, 0.0, 0.8)
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
    -- How far in from the screen edges the arena top/bottom are
    local arenaMarginY = 150          -- tweak to taste
    local wallThicknessPx = 20
    local halfT = px_to_m(wallThicknessPx * 0.5)

    -----------------------------
    -- Top / Bottom arena walls
    -----------------------------
    -- Bottom wall (inside the screen)
    floorEntity = Entity.create_entity()
    Entity.set_global_pos(floorEntity, screenW * 0.5, screenH - arenaMarginY)
    Entity.add_sprite_component(
        floorEntity,
        assets.textures.Wall,
        screenW,
        wallThicknessPx,
        50
    )
    Entity.add_fysics_component(floorEntity, 1, false)
    Fysics.add_box_collider(floorEntity, px_to_m(screenW), halfT, 0, 0, 0, false)

    -- Top wall (inside the screen)
    ceilingEntity = Entity.create_entity()
    Entity.set_global_pos(ceilingEntity, screenW * 0.5, arenaMarginY)
    Entity.add_sprite_component(
        ceilingEntity,
        assets.textures.Wall,
        screenW,
        wallThicknessPx,
        50
    )
    Entity.add_fysics_component(ceilingEntity, 1, false)
    Fysics.add_box_collider(ceilingEntity, px_to_m(screenW), halfT, 0, 0, 0, false)

    local sideWallWidthPx = 30
    local sideWallHalfWm = px_to_m(sideWallWidthPx * 0.5)

    -- Vertical usable arena height (between inner edges of top/bottom walls)
    local arenaHeightPx = screenH - 2 * arenaMarginY - wallThicknessPx

    -- How tall each pillar is inside that arena
    local pillarHeightPx = arenaHeightPx * 0.3   -- 30% top, 30% bottom, 40% gap; tweak to taste
    local pillarHalfHm  = px_to_m(pillarHeightPx * 0.5)

    -- Y positions for pillars so they butt up against the walls
    -- Top pillar sits just below the top wall
    local topPillarCenterY =
        arenaMarginY + (wallThicknessPx * 0.5) + (pillarHeightPx * 0.5)

    -- Bottom pillar sits just above the bottom wall
    local bottomPillarCenterY =
        screenH - arenaMarginY - (wallThicknessPx * 0.5) - (pillarHeightPx * 0.5)

    -- X positions (goal line at the very left/right)
    local leftPillarX  = sideWallWidthPx * 0.5
    local rightPillarX = screenW - sideWallWidthPx * 0.5

    -- LEFT TOP BLOCKER (inside arena)
    leftTopWall = Entity.create_entity()
    Entity.set_global_pos(leftTopWall, leftPillarX, topPillarCenterY)
    Entity.add_sprite_component(
        leftTopWall,
        assets.textures.Wall,
        sideWallWidthPx,
        pillarHeightPx,
        50
    )
    Entity.add_fysics_component(leftTopWall, 1, false)
    Fysics.add_box_collider(leftTopWall, sideWallHalfWm, pillarHalfHm + 2, 0, 0, 0, false)

    -- LEFT BOTTOM BLOCKER (inside arena)
    leftBottomWall = Entity.create_entity()
    Entity.set_global_pos(leftBottomWall, leftPillarX, bottomPillarCenterY)
    Entity.add_sprite_component(
        leftBottomWall,
        assets.textures.Wall,
        sideWallWidthPx,
        pillarHeightPx,
        50
    )
    Entity.add_fysics_component(leftBottomWall, 1, false)
    Fysics.add_box_collider(leftBottomWall, sideWallHalfWm, pillarHalfHm + 2, 0, 0, 0, false)

    -- RIGHT TOP BLOCKER (inside arena)
    rightTopWall = Entity.create_entity()
    Entity.set_global_pos(rightTopWall, rightPillarX, topPillarCenterY)
    Entity.add_sprite_component(
        rightTopWall,
        assets.textures.Wall,
        sideWallWidthPx,
        pillarHeightPx,
        50
    )
    Entity.add_fysics_component(rightTopWall, 1, false)
    Fysics.add_box_collider(rightTopWall, sideWallHalfWm, pillarHalfHm + 2, 0, 0, 0, false)

    -- RIGHT BOTTOM BLOCKER (inside arena)
    rightBottomWall = Entity.create_entity()
    Entity.set_global_pos(rightBottomWall, rightPillarX, bottomPillarCenterY)
    Entity.add_sprite_component(
        rightBottomWall, 
        assets.textures.Wall,
        sideWallWidthPx,
        pillarHeightPx,
        50
    )
    Entity.add_fysics_component(rightBottomWall, 1, false)
    Fysics.add_box_collider(rightBottomWall, sideWallHalfWm, pillarHalfHm + 2, 0, 0, 0, false)
end
local function InitAudio()
    boingEntity = Entity.create_entity()
    Entity.add_audio_component(boingEntity, "Boing", false)
end

local function PlayBoing()
    if not boingEntity then return end
    AudioComponent.play(boingEntity)
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
        InitAudio()
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
        if  Fysics.col(ballEntity, leftPaddle)
                or Fysics.col(ballEntity, rightPaddle)
                or Fysics.col(ballEntity, floorEntity)
                or Fysics.col(ballEntity, ceilingEntity)
                or Fysics.col(ballEntity, leftTopWall)
                or Fysics.col(ballEntity, leftBottomWall)
                or Fysics.col(ballEntity, rightTopWall)
                or Fysics.col(ballEntity, rightBottomWall)
            then
                PlayBoing()
            end

        
        -- Scoring - check sprite position (pixels)
            local ballPos = Fysics.get_pos(ballEntity) -- meters
            local bx = m_to_px(Mafs.get_vec_x(ballPos))
        
            -- NOTE:
            -- Only crossing fully off the left/right edges counts as a goal.
            -- The side blockers physically prevent using the top/bottom corners as goals.
        if bx < -100 then
            -- Right player scores
            scoreRight = scoreRight + 1
            ResetBall()

        elseif bx > screenW + 100 then
            -- Left player scores
            scoreLeft = scoreLeft + 1
            ResetBall()
        end
        -- Draw score UI
        local scoreText = tostring(scoreLeft) .. "  :  " .. tostring(scoreRight)

        -- Centered at top
        UI.draw_label(
                scoreText,
                200,            -- width
                60,             -- height
                screenW * 0.5 - 100,  -- x (centered)
                20,             -- y (top margin)
                ""              -- font (empty = default)
        )-- Draw score UI
        local scoreText = tostring(scoreLeft) .. "  :  " .. tostring(scoreRight)

        -- Centered at top
        UI.draw_label(
                scoreText,
                200,            -- width
                60,             -- height
                screenW * 0.5 - 100,  -- x (centered)
                20,             -- y (top margin)
                ""              -- font (empty = default)
        )
    end


return game
