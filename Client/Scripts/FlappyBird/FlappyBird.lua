local ExampleScript = {}
local assets        = require("Scripts.Assets")
local enums         = require("Scripts.Enums")

local Map           = require("Scripts.FlappyBird.Map")
local Death         = require("Scripts.FlappyBird.Death")
local Projectiles   = require("Scripts.FlappyBird.Projectiles")

----------------------------------------------------------
-- PLAYER STATE
----------------------------------------------------------
local player1
local player2

local jumpCount1 = 0
local jumpCount2 = 0

local grounded1   = false
local grounded2   = false
local groundLineY = 500

-- spawn positions for respawn
local spawn1X, spawn1Y = 0, 0
local spawn2X, spawn2Y = 0, 0

----------------------------------------------------------
-- JUMP
----------------------------------------------------------
local maxJumps  = 2
local jumpForce = -30

----------------------------------------------------------
-- DASH (double-tap on left stick)
----------------------------------------------------------
local dashSpeed      = 12
local dashDuration   = 0.15
local dashTapWindow  = 0.25
local timeSinceStart = 0

local dashP1Time, dashP1Dir = 0, 0
local lastStickDirP1, lastTapTimeP1, lastTapDirP1 = 0, -1, 0

local dashP2Time, dashP2Dir = 0, 0
local lastStickDirP2, lastTapTimeP2, lastTapDirP2 = 0, -1, 0

----------------------------------------------------------
-- SHOOTING / CHARGE
----------------------------------------------------------
local fireCooldown = 0
local fireInterval = 1.0

local chargeP1     = 0
local chargeP2     = 0
local isChargingP1 = false
local isChargingP2 = false
local maxCharge    = 2.0

local minProjectileSize = 16
local maxProjectileSize = 64

-- last aim direction: 1 = right, -1 = left
local lastDirP1 = 1
local lastDirP2 = 1

----------------------------------------------------------
-- CHARGE BARS
----------------------------------------------------------
local chargeBarP1
local chargeBarP2
local chargeBarSpriteP1
local chargeBarSpriteP2

local chargeBarWidth   = 60
local chargeBarHeight  = 8
local chargeBarYOffset = -40

----------------------------------------------------------
-- HELPERS
----------------------------------------------------------
local function axis_to_dir(x)
    if x > 0.5 then
        return 1
    elseif x < -0.5 then
        return -1
    else
        return 0
    end
end

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()
    ------------------------------------------------------
    -- MAP (background + floor + platform + ground Y)
    ------------------------------------------------------
    local bg, tile, platform, newGroundLineY = Map.create_level1()
    groundLineY = newGroundLineY or groundLineY

    local centerX = 960

    ------------------------------------------------------
    -- PLAYER 1
    ------------------------------------------------------
    player1 = Entity.create_entity()
    Entity.set_global_pos(player1, centerX - 40, 200)

    local playerSprite1 = Entity.add_sprite_component(
        player1,
        assets.textures.FlappyBird,
        32,
        32,
        10
    )
    Sprite.set_columns(playerSprite1, 1)
    Sprite.set_color(playerSprite1, 255, 255, 0)

    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player1, false, 1)

    ------------------------------------------------------
    -- PLAYER 2
    ------------------------------------------------------
    player2 = Entity.create_entity()
    Entity.set_global_pos(player2, centerX + 40, 200)

    local playerSprite2 = Entity.add_sprite_component(
        player2,
        assets.textures.FlappyBird,
        32,
        32,
        10
    )
    Sprite.set_columns(playerSprite2, 1)
    Sprite.set_color(playerSprite2, 0, 255, 255)

    Entity.add_fysics_component(player2, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player2, false, 1)

    ------------------------------------------------------
    -- SPAWN POSITIONS
    ------------------------------------------------------
    local p1 = Entity.get_global_pos(player1)
    spawn1X = Mafs.get_vec_x(p1)
    spawn1Y = Mafs.get_vec_y(p1)

    local p2 = Entity.get_global_pos(player2)
    spawn2X = Mafs.get_vec_x(p2)
    spawn2Y = Mafs.get_vec_y(p2)

    ------------------------------------------------------
    -- CHARGE BARS
    ------------------------------------------------------
    chargeBarP1 = Entity.create_entity()
    chargeBarSpriteP1 = Entity.add_sprite_component(
        chargeBarP1,
        assets.textures.Sand,
        chargeBarWidth,
        chargeBarHeight,
        100
    )
    Sprite.set_columns(chargeBarSpriteP1, 1)
    Sprite.set_color(chargeBarSpriteP1, 0, 255, 0)
    Entity.set_global_pos(chargeBarP1, -1000, -1000)

    chargeBarP2 = Entity.create_entity()
    chargeBarSpriteP2 = Entity.add_sprite_component(
        chargeBarP2,
        assets.textures.Sand,
        chargeBarWidth,
        chargeBarHeight,
        100
    )
    Sprite.set_columns(chargeBarSpriteP2, 1)
    Sprite.set_color(chargeBarSpriteP2, 0, 255, 0)
    Entity.set_global_pos(chargeBarP2, -1000, -1000)

    ------------------------------------------------------
    -- DEATH ZONE (Smash-style blast zone)
    ------------------------------------------------------
    Death.set_bounds(-200, 1920 + 200, -200, 1080 + 200)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    local dt = Mafs.delta_time()
    timeSinceStart = timeSinceStart + dt

    local vel1 = Fysics.get_linear_velocity(player1)
    local vx1  = Mafs.get_vec_x(vel1)
    local vy1  = Mafs.get_vec_y(vel1)

    local vel2 = Fysics.get_linear_velocity(player2)
    local vx2  = Mafs.get_vec_x(vel2)
    local vy2  = Mafs.get_vec_y(vel2)

    ------------------------------------------------------
    -- GROUNDED CHECKS (simple y line)
    ------------------------------------------------------
    local pos1 = Entity.get_global_pos(player1)
    local p1y  = Mafs.get_vec_y(pos1)
    grounded1  = (vy1 >= 0 and p1y >= groundLineY)

    local pos2 = Entity.get_global_pos(player2)
    local p2y  = Mafs.get_vec_y(pos2)
    grounded2  = (vy2 >= 0 and p2y >= groundLineY)

    if grounded1 then jumpCount1 = 0 end
    if grounded2 then jumpCount2 = 0 end

    ------------------------------------------------------
    -- JUMP (double jump)
    ------------------------------------------------------
    if Input.get_button_down(0, Buttons.ionix_a) and jumpCount1 < maxJumps then
        jumpCount1 = jumpCount1 + 1
        Fysics.add_force_to_center(player1, 0, jumpForce)
    end

    if Input.get_button_down(1, Buttons.ionix_a) and jumpCount2 < maxJumps then
        jumpCount2 = jumpCount2 + 1
        Fysics.add_force_to_center(player2, 0, jumpForce)
    end

    ------------------------------------------------------
    -- MOVEMENT + AIM
    ------------------------------------------------------
    local stickX1 = Input.get_left_stick_x(0)
    local stickX2 = Input.get_left_stick_x(1)

    if stickX1 > 0.1 then
        lastDirP1 = 1
    elseif stickX1 < -0.1 then
        lastDirP1 = -1
    end

    if stickX2 > 0.1 then
        lastDirP2 = 1
    elseif stickX2 < -0.1 then
        lastDirP2 = -1
    end

    ------------------------------------------------------
    -- DASH DETECTION (double-tap)
    ------------------------------------------------------
    local dir1 = axis_to_dir(stickX1)
    local dir2 = axis_to_dir(stickX2)

    -- P1 dash
    if dir1 ~= 0 and lastStickDirP1 == 0 then
        if lastTapTimeP1 >= 0
            and (timeSinceStart - lastTapTimeP1) <= dashTapWindow
            and dir1 == lastTapDirP1 then

            dashP1Time = dashDuration
            dashP1Dir  = dir1
        end
        lastTapTimeP1 = timeSinceStart
        lastTapDirP1  = dir1
    end
    lastStickDirP1 = dir1

    -- P2 dash
    if dir2 ~= 0 and lastStickDirP2 == 0 then
        if lastTapTimeP2 >= 0
            and (timeSinceStart - lastTapTimeP2) <= dashTapWindow
            and dir2 == lastTapDirP2 then

            dashP2Time = dashDuration
            dashP2Dir  = dir2
        end
        lastTapTimeP2 = timeSinceStart
        lastTapDirP2  = dir2
    end
    lastStickDirP2 = dir2

    ------------------------------------------------------
    -- BASE HORIZONTAL MOVE (stick or dash)
    ------------------------------------------------------
    if dashP1Time > 0 then
        vx1 = dashP1Dir * dashSpeed
        dashP1Time = dashP1Time - dt
        if dashP1Time < 0 then dashP1Time = 0 end
    else
        vx1 = 2.5 * stickX1
    end

    if dashP2Time > 0 then
        vx2 = dashP2Dir * dashSpeed
        dashP2Time = dashP2Time - dt
        if dashP2Time < 0 then dashP2Time = 0 end
    else
        vx2 = 2.5 * stickX2
    end

    ------------------------------------------------------
    -- APPLY KNOCKBACK FROM PROJECTILES
    ------------------------------------------------------
    vx1, vx2 = Projectiles.ApplyKnockback(dt, vx1, vx2)

    ------------------------------------------------------
    -- APPLY VELOCITY
    ------------------------------------------------------
    Fysics.set_linear_velocity(player1, vx1, vy1)
    Fysics.set_linear_velocity(player2, vx2, vy2)

    ------------------------------------------------------
    -- SHOOTING / CHARGE
    ------------------------------------------------------
    if fireCooldown > 0 then
        fireCooldown = fireCooldown - dt
        if fireCooldown < 0 then fireCooldown = 0 end
    end

    -- start charging when B is pressed
    if Input.get_button_down(0, Buttons.ionix_b) and fireCooldown <= 0 and not isChargingP1 then
        isChargingP1 = true
        chargeP1 = 0
    end

    if Input.get_button_down(1, Buttons.ionix_b) and fireCooldown <= 0 and not isChargingP2 then
        isChargingP2 = true
        chargeP2 = 0
    end

    -- P1 charge + bar
    if isChargingP1 then
        if Input.get_button_held(0, Buttons.ionix_b) then
            chargeP1 = math.min(chargeP1 + dt, maxCharge)

            local t = chargeP1 / maxCharge
            if t > 1 then t = 1 end

            local r = math.floor(255 * t)
            local g = math.floor(255 * (1 - t))
            Sprite.set_color(chargeBarSpriteP1, r, g, 0)

            local pos = Entity.get_global_pos(player1)
            local px  = Mafs.get_vec_x(pos)
            local py  = Mafs.get_vec_y(pos)
            Entity.set_global_pos(chargeBarP1, px - chargeBarWidth * 0.5, py + chargeBarYOffset)
        else
            local pos = Entity.get_global_pos(player1)
            local px  = Mafs.get_vec_x(pos)
            local py  = Mafs.get_vec_y(pos)

            local t = chargeP1 / maxCharge
            if t > 1 then t = 1 end
            local size = minProjectileSize + (maxProjectileSize - minProjectileSize) * t

            -- spawn projectile via module
            Projectiles.SpawnProjectile(px, py, lastDirP1, 0, size, 1)

            chargeP1     = 0
            isChargingP1 = false
            fireCooldown = fireInterval

            Entity.set_global_pos(chargeBarP1, -1000, -1000)
        end
    else
        Entity.set_global_pos(chargeBarP1, -1000, -1000)
    end

    -- P2 charge + bar
    if isChargingP2 then
        if Input.get_button_held(1, Buttons.ionix_b) then
            chargeP2 = math.min(chargeP2 + dt, maxCharge)

            local t = chargeP2 / maxCharge
            if t > 1 then t = 1 end

            local r = math.floor(255 * t)
            local g = math.floor(255 * (1 - t))
            Sprite.set_color(chargeBarSpriteP2, r, g, 0)

            local pos = Entity.get_global_pos(player2)
            local px  = Mafs.get_vec_x(pos)
            local py  = Mafs.get_vec_y(pos)
            Entity.set_global_pos(chargeBarP2, px - chargeBarWidth * 0.5, py + chargeBarYOffset)
        else
            local pos = Entity.get_global_pos(player2)
            local px  = Mafs.get_vec_x(pos)
            local py  = Mafs.get_vec_y(pos)

            local t = chargeP2 / maxCharge
            if t > 1 then t = 1 end
            local size = minProjectileSize + (maxProjectileSize - minProjectileSize) * t

            -- spawn projectile via module
            Projectiles.SpawnProjectile(px, py, lastDirP2, 0, size, 2)

            chargeP2     = 0
            isChargingP2 = false
            fireCooldown = fireInterval

            Entity.set_global_pos(chargeBarP2, -1000, -1000)
        end
    else
        Entity.set_global_pos(chargeBarP2, -1000, -1000)
    end

    ------------------------------------------------------
    -- PROJECTILE UPDATE
    ------------------------------------------------------
    Projectiles.Update(dt, player1, player2)

    ------------------------------------------------------
    -- DEATH / RESPAWN CHECKS
    ------------------------------------------------------
    if Death.is_out_of_bounds(player1) then
        Entity.set_global_pos(player1, spawn1X, spawn1Y)
        Fysics.set_linear_velocity(player1, 0, 0)
        jumpCount1 = 0
    end

    if Death.is_out_of_bounds(player2) then
        Entity.set_global_pos(player2, spawn2X, spawn2Y)
        Fysics.set_linear_velocity(player2, 0, 0)
        jumpCount2 = 0
    end
end

----------------------------------------------------------
-- Hooks (unused)
----------------------------------------------------------
function ExampleScript:OnCollisionEnter() end
function ExampleScript:OnCollisionExit()  end
function ExampleScript:OnTriggerEnter()   end
function ExampleScript:OnTriggerExit()    end

return ExampleScript
