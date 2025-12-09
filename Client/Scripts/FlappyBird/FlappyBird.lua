local ExampleScript = {}
local assets = require("Scripts.Assets")
local enums = require("Scripts.Enums")

local Background
local player1
local player2
local x = 300
local jumpCount1 = 0
local jumpCount2 = 0
local tile
local platform1

-- JUMP SETTINGS
local maxJumps   = 2
local jumpForce  = -30

-- SHOOTING / TIMERS
local fireCooldown = 0
local fireInterval = 1.0   -- delay between shots

-- ============ SIMPLE PROJECTILE SYSTEM ============
local projectiles = {}
local projectileSpeed = 600          -- pixels per second
local projectileSize = 16            -- default size if not charged
local projectileLifetime = 2.0       -- seconds

-- CHARGE DATA
local chargeP1 = 0
local chargeP2 = 0
local isChargingP1 = false
local isChargingP2 = false
local maxCharge = 2.0                -- seconds to reach full charge
local minProjectileSize = 16         -- size at tap
local maxProjectileSize = 64         -- size at full charge

-- LAST AIM DIRECTION MEMORY
local lastDirP1 = 1
local lastDirP2 = 1

-- KNOCKBACK 
local knockbackP1Time = 0
local knockbackP2Time = 0
local knockbackP1Dir = 0
local knockbackP2Dir = 0
local knockbackBaseDuration = 0.25   -- base duration in seconds
local knockbackBaseSpeed    = 2.5    -- base horizontal speed of the push

-- CHARGE BAR VISUALS 
local chargeBarP1
local chargeBarP2
local chargeBarSpriteP1
local chargeBarSpriteP2
local chargeBarWidth  = 60
local chargeBarHeight = 8
local chargeBarYOffset = -40       -- above player

----------------------------------------------------------
-- PROJECTILE HELPERS
----------------------------------------------------------
local function SpawnProjectile(spawnX, spawnY, dirX, dirY, size, owner)
    size = size or projectileSize

    -- normalise direction, default to (1, 0)
    if dirX == 0 and dirY == 0 then
        dirX = 1
        dirY = 0
    else
        local len = math.sqrt(dirX * dirX + dirY * dirY)
        if len > 0 then
            dirX = dirX / len
            dirY = dirY / len
        else
            dirX = 1
            dirY = 0
        end
    end

    local proj = Entity.create_entity()
    local sprite = Entity.add_sprite_component(
        proj,
        assets.textures.Ghast_Tear,
        size,
        size,
        5
    )

    Entity.set_global_pos(proj, spawnX, spawnY)

    local projData = {
        entity = proj,
        sprite = sprite,
        x = spawnX,
        y = spawnY,
        vx = dirX * projectileSpeed,
        vy = dirY * projectileSpeed,
        age = 0,
        size = size,
        owner = owner
    }

    table.insert(projectiles, projData)
end

local function UpdateProjectiles(dt)
    for i = #projectiles, 1, -1 do
        local p = projectiles[i]

        -- move the projectile
        p.x = p.x + p.vx * dt
        p.y = p.y + p.vy * dt
        Entity.set_global_pos(p.entity, p.x, p.y)

        local hit = false
        local size = p.size or projectileSize
        local radiusProj = size * 0.5
        local radiusPlayer = 16 

        -- tap vs held
        local isTapShot = size <= (minProjectileSize + 0.5)
        local sizeScale = math.min(size / minProjectileSize, 2.0)

        -- hit on player1
        if player1 and p.owner ~= 1 then
            local pos = Entity.get_global_pos(player1)
            local px = Mafs.get_vec_x(pos)
            local py = Mafs.get_vec_y(pos)
            local dx = px - p.x
            local dy = py - p.y
            local r = radiusProj + radiusPlayer

            if dx * dx + dy * dy <= r * r then
                if not isTapShot then
                    local dirX = (p.vx >= 0) and 1 or -1
                    knockbackP1Time = knockbackBaseDuration * sizeScale
                    knockbackP1Dir  = dirX * sizeScale
                end
                hit = true
            end
        end

        -- hit on player2
        if not hit and player2 and p.owner ~= 2 then
            local pos = Entity.get_global_pos(player2)
            local px = Mafs.get_vec_x(pos)
            local py = Mafs.get_vec_y(pos)
            local dx = px - p.x
            local dy = py - p.y
            local r = radiusProj + radiusPlayer

            if dx * dx + dy * dy <= r * r then
                if not isTapShot then
                    local dirX = (p.vx >= 0) and 1 or -1
                    knockbackP2Time = knockbackBaseDuration * sizeScale
                    knockbackP2Dir  = dirX * sizeScale
                end
                hit = true
            end
        end

        -- age / destroy
        p.age = p.age + dt
        if hit or p.age > projectileLifetime then
            Entity.destroy_entity(p.entity)
            table.remove(projectiles, i)
        end
    end
end

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()
    Background = Entity.create_entity()
    Entity.add_sprite_component(Background, assets.textures.Background, 1920, 1080, 0)

    player1 = Entity.create_entity()
    Entity.set_global_pos(player1, x, 200)
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(playerSprite1, 1)
    Sprite.set_color(playerSprite1, 255, 255, 0)   -- P1 yellow
    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player1, false, 1)

    player2 = Entity.create_entity()
    Entity.set_global_pos(player2, x + 50, 200)
    local playerSprite2 = Entity.add_sprite_component(player2, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(playerSprite2, 1)
    Sprite.set_color(playerSprite2, 0, 255, 255)   -- P2 cyan
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
    local vx1 = Mafs.get_vec_x(vel1)
    local vy1 = Mafs.get_vec_y(vel1)

    local vel2 = Fysics.get_linear_velocity(player2)
    local vx2 = Mafs.get_vec_x(vel2)
    local vy2 = Mafs.get_vec_y(vel2)

    -- grounded checks every frame (resets jump counts)
    local grounded1 = Fysics.col(player1, tile) or Fysics.col(player1, platform1)
    local grounded2 = Fysics.col(player2, tile) or Fysics.col(player2, platform1)

    if grounded1 and vy1 >= 0 then
        jumpCount1 = 0
    end

    if grounded2 and vy2 >= 0 then
        jumpCount2 = 0
    end

    -- double jump: maxJumps per air-time
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

    if knockbackP1Time > 0 then
        local t = knockbackP1Time / knockbackBaseDuration
        vx1 = knockbackP1Dir * knockbackBaseSpeed * t
        knockbackP1Time = knockbackP1Time - dt
        if knockbackP1Time < 0 then knockbackP1Time = 0 end
    else
        vx1 = 2.5 * stickX1
    end

    if knockbackP2Time > 0 then
        local t = knockbackP2Time / knockbackBaseDuration
        vx2 = knockbackP2Dir * knockbackBaseSpeed * t
        knockbackP2Time = knockbackP2Time - dt
        if knockbackP2Time < 0 then knockbackP2Time = 0 end
    else
        vx2 = 2.5 * stickX2
    end

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

            local pos = Entity.get_global_pos(player1)
            local px = Mafs.get_vec_x(pos)
            local py = Mafs.get_vec_y(pos)

            local t = chargeP1 / maxCharge
            if t > 1 then t = 1 end

            local r = math.floor(255 * t)
            local g = math.floor(255 * (1 - t))
            Sprite.set_color(chargeBarSpriteP1, r, g, 0)

            Entity.set_global_pos(chargeBarP1, px - chargeBarWidth * 0.5, py + chargeBarYOffset)
        else
            local pos = Entity.get_global_pos(player1)
            local px = Mafs.get_vec_x(pos)
            local py = Mafs.get_vec_y(pos)

            local t = chargeP1 / maxCharge
            if t > 1 then t = 1 end
            local size = minProjectileSize + (maxProjectileSize - minProjectileSize) * t

            SpawnProjectile(px, py, lastDirP1, 0, size, 1)

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

            local pos = Entity.get_global_pos(player2)
            local px = Mafs.get_vec_x(pos)
            local py = Mafs.get_vec_y(pos)

            local t = chargeP2 / maxCharge
            if t > 1 then t = 1 end

            local r = math.floor(255 * t)
            local g = math.floor(255 * (1 - t))
            Sprite.set_color(chargeBarSpriteP2, r, g, 0)

            Entity.set_global_pos(chargeBarP2, px - chargeBarWidth * 0.5, py + chargeBarYOffset)
        else
            local pos = Entity.get_global_pos(player2)
            local px = Mafs.get_vec_x(pos)
            local py = Mafs.get_vec_y(pos)

            local t = chargeP2 / maxCharge
            if t > 1 then t = 1 end
            local size = minProjectileSize + (maxProjectileSize - minProjectileSize) * t

            SpawnProjectile(px, py, lastDirP2, 0, size, 2)

            chargeP2 = 0
            isChargingP2 = false
            fireCooldown = fireInterval

            Entity.set_global_pos(chargeBarP2, -1000, -1000)
        end
    else
        Entity.set_global_pos(chargeBarP2, -1000, -1000)
    end

    UpdateProjectiles(dt)
end

----------------------------------------------------------
-- Collisions
----------------------------------------------------------
function ExampleScript:OnCollisionEnter()
end

function ExampleScript:OnTriggerEnter() end
function ExampleScript:OnTriggerExit() end
function ExampleScript:OnCollisionExit() end

return ExampleScript
