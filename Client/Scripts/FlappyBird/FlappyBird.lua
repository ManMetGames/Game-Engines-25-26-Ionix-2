local ExampleScript = {}

local assets = require("Scripts.Assets")
local enums  = require("Scripts.Enums")

local Map         = require("Scripts.FlappyBird.Map")
local Projectiles = require("Scripts.FlappyBird.Projectiles")
local Shield      = require("Scripts.FlappyBird.Shield")
local Death       = require("Scripts.FlappyBird.Death")

local Background
local player1
local player2
local tile
local platform1

local grounded1 = false
local grounded2 = false
local groundLineY = 500

local spawn1X, spawn1Y = 0, 0
local spawn2X, spawn2Y = 0, 0

local moveSpeed = 2.5

local jumpCount1 = 0
local jumpCount2 = 0
local maxJumps  = 2
local jumpForce = -30

local dashSpeed      = 12
local dashDuration   = 0.15
local dashTapWindow  = 0.25
local timeSinceStart = 0

local dashP1Time, dashP1Dir = 0, 0
local lastStickDirP1 = 0
local lastTapTimeP1  = -1
local lastTapDirP1   = 0

local dashP2Time, dashP2Dir = 0, 0
local lastStickDirP2 = 0
local lastTapTimeP2  = -1
local lastTapDirP2   = 0

local fireCooldown = 0
local fireInterval = 1.0

local chargeP1, chargeP2 = 0, 0
local isChargingP1, isChargingP2 = false, false
local maxCharge = 2.0
local minProjectileSize = 16
local maxProjectileSize = 64

local lastDirP1 = 1
local lastDirP2 = 1

local chargeBarP1
local chargeBarP2
local chargeBarSpriteP1
local chargeBarSpriteP2
local chargeBarWidth   = 60
local chargeBarHeight  = 8
local chargeBarYOffset = -40

local function axis_to_dir(x)
    if x > 0.5 then return 1 end
    if x < -0.5 then return -1 end
    return 0
end

function ExampleScript:OnStart()
    Background, tile, platform1, groundLineY = Map.create_level1()

    local centerX = 960

    player1 = Entity.create_entity()
    Entity.set_global_pos(player1, centerX - 40, 200)
    local p1Sprite = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(p1Sprite, 1)
    Sprite.set_color(p1Sprite, 255, 255, 0)
    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player1, false, 1)

    player2 = Entity.create_entity()
    Entity.set_global_pos(player2, centerX + 40, 200)
    local p2Sprite = Entity.add_sprite_component(player2, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(p2Sprite, 1)
    Sprite.set_color(p2Sprite, 0, 255, 255)
    Entity.add_fysics_component(player2, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player2, false, 1)


    do
        local p1 = Entity.get_global_pos(player1)
        spawn1X = Mafs.get_vec_x(p1)
        spawn1Y = Mafs.get_vec_y(p1)

        local p2 = Entity.get_global_pos(player2)
        spawn2X = Mafs.get_vec_x(p2)
        spawn2Y = Mafs.get_vec_y(p2)
    end

    Shield.Init(player1, player2)

    Death.set_bounds(-200, 1920 + 200, -200, 1080 + 200)
    Death.create_visuals()

    chargeBarP1 = Entity.create_entity()
    chargeBarSpriteP1 = Entity.add_sprite_component(chargeBarP1, assets.textures.Sand, chargeBarWidth, chargeBarHeight, 100)
    Sprite.set_columns(chargeBarSpriteP1, 1)
    Sprite.set_color(chargeBarSpriteP1, 0, 255, 0)
    Entity.set_global_pos(chargeBarP1, -1000, -1000)

    chargeBarP2 = Entity.create_entity()
    chargeBarSpriteP2 = Entity.add_sprite_component(chargeBarP2, assets.textures.Sand, chargeBarWidth, chargeBarHeight, 100)
    Sprite.set_columns(chargeBarSpriteP2, 1)
    Sprite.set_color(chargeBarSpriteP2, 0, 255, 0)
    Entity.set_global_pos(chargeBarP2, -1000, -1000)
end

function ExampleScript:OnUpdate()
    local dt = Mafs.delta_time()
    timeSinceStart = timeSinceStart + dt

    --------------------------------------------------
    -- SHIELD INPUT 
    --------------------------------------------------
    local lt1 = Input.get_left_trigger(0)
    local rt1 = Input.get_right_trigger(0)
    local lt2 = Input.get_left_trigger(1)
    local rt2 = Input.get_right_trigger(1)

    local shieldHeldP1 = (lt1 >= Shield.triggerThreshold) or (rt1 >= Shield.triggerThreshold)
    local shieldHeldP2 = (lt2 >= Shield.triggerThreshold) or (rt2 >= Shield.triggerThreshold)

    Shield.SetHeld(1, shieldHeldP1)
    Shield.SetHeld(2, shieldHeldP2)
    Shield.Update(dt, player1, player2)

    --------------------------------------------------
    -- VELOCITIES
    --------------------------------------------------
    local vel1 = Fysics.get_linear_velocity(player1)
    local vx1  = Mafs.get_vec_x(vel1)
    local vy1  = Mafs.get_vec_y(vel1)

    local vel2 = Fysics.get_linear_velocity(player2)
    local vx2  = Mafs.get_vec_x(vel2)
    local vy2  = Mafs.get_vec_y(vel2)

    --------------------------------------------------
    -- GROUNDED
    --------------------------------------------------
    local pos1 = Entity.get_global_pos(player1)
    local p1y  = Mafs.get_vec_y(pos1)
    grounded1  = (vy1 >= 0 and p1y >= groundLineY)

    local pos2 = Entity.get_global_pos(player2)
    local p2y  = Mafs.get_vec_y(pos2)
    grounded2  = (vy2 >= 0 and p2y >= groundLineY)

    if grounded1 then jumpCount1 = 0 end
    if grounded2 then jumpCount2 = 0 end

    --------------------------------------------------
    -- JUMP 
    --------------------------------------------------
    if Input.get_button_down(0, Buttons.ionix_a) and jumpCount1 < maxJumps then
        jumpCount1 = jumpCount1 + 1
        Fysics.add_force_to_center(player1, 0, jumpForce)
    end

    if Input.get_button_down(1, Buttons.ionix_a) and jumpCount2 < maxJumps then
        jumpCount2 = jumpCount2 + 1
        Fysics.add_force_to_center(player2, 0, jumpForce)
    end

    --------------------------------------------------
    -- MOVEMENT INPUT
    --------------------------------------------------
    local stickX1 = Input.get_left_stick_x(0)
    local stickX2 = Input.get_left_stick_x(1)

    if stickX1 > 0.1 then lastDirP1 = 1 elseif stickX1 < -0.1 then lastDirP1 = -1 end
    if stickX2 > 0.1 then lastDirP2 = 1 elseif stickX2 < -0.1 then lastDirP2 = -1 end

    --------------------------------------------------
    -- DASH DETECTION 
    --------------------------------------------------
    local dir1 = axis_to_dir(stickX1)
    local dir2 = axis_to_dir(stickX2)

    if not Shield.IsActive(1) then
        if dir1 ~= 0 and lastStickDirP1 == 0 then
            if lastTapTimeP1 >= 0 and (timeSinceStart - lastTapTimeP1) <= dashTapWindow and dir1 == lastTapDirP1 then
                dashP1Time = dashDuration
                dashP1Dir  = dir1
            end
            lastTapTimeP1 = timeSinceStart
            lastTapDirP1  = dir1
        end
        lastStickDirP1 = dir1
    else
        dashP1Time = 0
        lastStickDirP1 = dir1
    end

    if not Shield.IsActive(2) then
        if dir2 ~= 0 and lastStickDirP2 == 0 then
            if lastTapTimeP2 >= 0 and (timeSinceStart - lastTapTimeP2) <= dashTapWindow and dir2 == lastTapDirP2 then
                dashP2Time = dashDuration
                dashP2Dir  = dir2
            end
            lastTapTimeP2 = timeSinceStart
            lastTapDirP2  = dir2
        end
        lastStickDirP2 = dir2
    else
        dashP2Time = 0
        lastStickDirP2 = dir2
    end

    --------------------------------------------------
    -- BASE MOVE 
    --------------------------------------------------
    if Shield.IsActive(1) then
        vx1 = 0
    else
        vx1 = moveSpeed * stickX1
    end

    if Shield.IsActive(2) then
        vx2 = 0
    else
        vx2 = moveSpeed * stickX2
    end

    --------------------------------------------------
    -- APPLY DASH 
    --------------------------------------------------
    if dashP1Time > 0 and not Shield.IsActive(1) then
        vx1 = dashP1Dir * dashSpeed
        dashP1Time = dashP1Time - dt
        if dashP1Time < 0 then dashP1Time = 0 end
    end

    if dashP2Time > 0 and not Shield.IsActive(2) then
        vx2 = dashP2Dir * dashSpeed
        dashP2Time = dashP2Time - dt
        if dashP2Time < 0 then dashP2Time = 0 end
    end

    --------------------------------------------------
    -- KNOCKBACK overrides everything
    --------------------------------------------------
    vx1, vx2 = Projectiles.ApplyKnockback(dt, vx1, vx2)

    Fysics.set_linear_velocity(player1, vx1, vy1)
    Fysics.set_linear_velocity(player2, vx2, vy2)

    --------------------------------------------------
    -- SHOOTING 
    --------------------------------------------------
    if Shield.IsActive(1) then isChargingP1 = false end
    if Shield.IsActive(2) then isChargingP2 = false end

    if fireCooldown > 0 then
        fireCooldown = fireCooldown - dt
        if fireCooldown < 0 then fireCooldown = 0 end
    end

    -- start charging (B)
    if Input.get_button_down(0, Buttons.ionix_b) and fireCooldown <= 0 and not isChargingP1 and not Shield.IsActive(1) then
        isChargingP1 = true
        chargeP1 = 0
    end
    if Input.get_button_down(1, Buttons.ionix_b) and fireCooldown <= 0 and not isChargingP2 and not Shield.IsActive(2) then
        isChargingP2 = true
        chargeP2 = 0
    end

    -- P1 charge/release
    if isChargingP1 then
        if Input.get_button_held(0, Buttons.ionix_b) then
            chargeP1 = math.min(chargeP1 + dt, maxCharge)

            local t = chargeP1 / maxCharge
            if t > 1 then t = 1 end
            local r = math.floor(255 * t)
            local g = math.floor(255 * (1 - t))
            Sprite.set_color(chargeBarSpriteP1, r, g, 0)

            local p = Entity.get_global_pos(player1)
            Entity.set_global_pos(chargeBarP1, Mafs.get_vec_x(p) - chargeBarWidth*0.5, Mafs.get_vec_y(p) + chargeBarYOffset)
        else
            local p = Entity.get_global_pos(player1)
            local t = chargeP1 / maxCharge
            if t > 1 then t = 1 end
            local size = minProjectileSize + (maxProjectileSize - minProjectileSize) * t

            Projectiles.SpawnProjectile(Mafs.get_vec_x(p), Mafs.get_vec_y(p), lastDirP1, 0, size, 1)

            isChargingP1 = false
            chargeP1 = 0
            fireCooldown = fireInterval
            Entity.set_global_pos(chargeBarP1, -1000, -1000)
        end
    else
        Entity.set_global_pos(chargeBarP1, -1000, -1000)
    end

    -- P2 charge/release
    if isChargingP2 then
        if Input.get_button_held(1, Buttons.ionix_b) then
            chargeP2 = math.min(chargeP2 + dt, maxCharge)

            local t = chargeP2 / maxCharge
            if t > 1 then t = 1 end
            local r = math.floor(255 * t)
            local g = math.floor(255 * (1 - t))
            Sprite.set_color(chargeBarSpriteP2, r, g, 0)

            local p = Entity.get_global_pos(player2)
            Entity.set_global_pos(chargeBarP2, Mafs.get_vec_x(p) - chargeBarWidth*0.5, Mafs.get_vec_y(p) + chargeBarYOffset)
        else
            local p = Entity.get_global_pos(player2)
            local t = chargeP2 / maxCharge
            if t > 1 then t = 1 end
            local size = minProjectileSize + (maxProjectileSize - minProjectileSize) * t

            Projectiles.SpawnProjectile(Mafs.get_vec_x(p), Mafs.get_vec_y(p), lastDirP2, 0, size, 2)

            isChargingP2 = false
            chargeP2 = 0
            fireCooldown = fireInterval
            Entity.set_global_pos(chargeBarP2, -1000, -1000)
        end
    else
        Entity.set_global_pos(chargeBarP2, -1000, -1000)
    end

    --------------------------------------------------
    -- PROJECTILES UPDATE 
    --------------------------------------------------
    Projectiles.Update(dt, player1, player2, Shield)

    --------------------------------------------------
    -- DEATH ZONE / RESPAWN
    --------------------------------------------------
    if Death.is_out_of_bounds(player1) then
        Entity.set_global_pos(player1, spawn1X, spawn1Y)
        Fysics.set_linear_velocity(player1, 0, 0)
        jumpCount1 = 0
        Shield.Reset(1)
    end

    if Death.is_out_of_bounds(player2) then
        Entity.set_global_pos(player2, spawn2X, spawn2Y)
        Fysics.set_linear_velocity(player2, 0, 0)
        jumpCount2 = 0
        Shield.Reset(2)
    end
end

function ExampleScript:OnCollisionEnter() end
function ExampleScript:OnCollisionExit()  end
function ExampleScript:OnTriggerEnter()   end
function ExampleScript:OnTriggerExit()    end

return ExampleScript
