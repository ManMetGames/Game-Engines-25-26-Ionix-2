local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums  = require("Scripts.Enums")
local Projectiles = require("Scripts.FlappyBird.Projectiles")

local Background
local player1
local player2
local x = 300
local jumpCount1 = 0
local jumpCount2 = 0
local tile
local platform1

local grounded1 = false
local grounded2 = false

local maxJumps   = 2
local jumpForce  = -30

local fireCooldown = 0
local fireInterval = 1.0

-- charge / projectile size config (used for UI + size)
local chargeP1 = 0
local chargeP2 = 0
local isChargingP1 = false
local isChargingP2 = false
local maxCharge         = 2.0
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

local groundLineY = 450

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()
    Background = Entity.create_entity()
    Entity.add_sprite_component(Background, assets.textures.Background, 1920, 1080, 0)

    -- PLAYER 1
    player1 = Entity.create_entity()
    Entity.set_global_pos(player1, x, 200)
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(playerSprite1, 1)
    Sprite.set_color(playerSprite1, 255, 255, 0)
    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player1, false, 1)

    -- PLAYER 2
    player2 = Entity.create_entity()
    Entity.set_global_pos(player2, x + 50, 200)
    local playerSprite2 = Entity.add_sprite_component(player2, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(playerSprite2, 1)
    Sprite.set_color(playerSprite2, 0, 0, 255)
    Entity.add_fysics_component(player2, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player2, false, 1)

    local floorY = 500
	tile = Entity.create_entity()
	Entity.set_global_pos(tile, 250, floorY)
    Entity.add_sprite_component(tile, assets.textures.Sand, 500, 50, 1)
	Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)
	Fysics.add_sprite_collider(tile, false, 1)

    platform1 = Entity.create_entity()
	Entity.set_global_pos(platform1, 300, 400)
    Entity.add_sprite_component(platform1, assets.textures.Sand, 200, 10, 1)
	Entity.add_fysics_component(platform1, enums.bodytype.staticBody, false)
	Fysics.add_edge_collider(platform1, 20, 30, 50, 30, false)

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
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    local dt = Mafs.delta_time()

    local vel1 = Fysics.get_linear_velocity(player1)
    local vx1  = Mafs.get_vec_x(vel1)
    local vy1  = Mafs.get_vec_y(vel1)

    local vel2 = Fysics.get_linear_velocity(player2)
    local vx2  = Mafs.get_vec_x(vel2)
    local vy2  = Mafs.get_vec_y(vel2)

    -- pseudo-grounded off Y position
    local pos1 = Entity.get_global_pos(player1)
    local p1y  = Mafs.get_vec_y(pos1)
    grounded1  = (vy1 >= 0 and p1y >= groundLineY)

    local pos2 = Entity.get_global_pos(player2)
    local p2y  = Mafs.get_vec_y(pos2)
    grounded2  = (vy2 >= 0 and p2y >= groundLineY)

    if grounded1 then jumpCount1 = 0 end
    if grounded2 then jumpCount2 = 0 end

    -- double jump
	if Input.get_button_down(0, Buttons.ionix_a) and jumpCount1 < maxJumps then
        jumpCount1 = jumpCount1 + 1
        Fysics.add_force_to_center(player1, 0, jumpForce)
	end

    if Input.get_button_down(1, Buttons.ionix_a) and jumpCount2 < maxJumps then
        jumpCount2 = jumpCount2 + 1
        Fysics.add_force_to_center(player2, 0, jumpForce)
	end
    
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

    -- base movement from stick
    vx1 = 2.5 * stickX1
    vx2 = 2.5 * stickX2

    -- let projectile system override vx via knockback if active
    vx1, vx2 = Projectiles.ApplyKnockback(dt, vx1, vx2)

    Fysics.set_linear_velocity(player1, vx1, vy1)
    Fysics.set_linear_velocity(player2, vx2, vy2)

    -- shooting cooldown
    if fireCooldown > 0 then
        fireCooldown = fireCooldown - dt
        if fireCooldown < 0 then fireCooldown = 0 end
    end

    -- start charging
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

            Projectiles.SpawnProjectile(px, py, lastDirP1, 0, size, 1)

            chargeP1 = 0
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

            Projectiles.SpawnProjectile(px, py, lastDirP2, 0, size, 2)

            chargeP2 = 0
            isChargingP2 = false
            fireCooldown = fireInterval

            Entity.set_global_pos(chargeBarP2, -1000, -1000)
        end
    else
        Entity.set_global_pos(chargeBarP2, -1000, -1000)
    end

    -- finally, move and resolve projectiles & knockback timers
    Projectiles.Update(dt, player1, player2)
end

function ExampleScript:OnCollisionEnter() end
function ExampleScript:OnCollisionExit() end
function ExampleScript:OnTriggerEnter() end
function ExampleScript:OnTriggerExit() end

return ExampleScript
