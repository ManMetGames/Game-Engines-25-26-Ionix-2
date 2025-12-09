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

-- SHOOTING / TIMERS
local fireCooldown = 0
local fireInterval = 0.25

-- every 30 seconds auto fire
local autoFireTimer = 0
local autoFireInterval = 30.0

-- ============ SIMPLE PROJECTILE SYSTEM ============
local projectiles = {}
local projectileSpeed = 600          -- pixels per second
local projectileSize = 16
local projectileLifetime = 2.0       -- seconds

local function SpawnProjectile(spawnX, spawnY, dirX, dirY)
    -- normalise direction, default to (1, 0) if zero
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
    Entity.add_sprite_component(
        proj,
        assets.textures.Ghast_Tear,
        projectileSize,
        projectileSize,
        5
    )

    -- initial transform
    Entity.set_global_pos(proj, spawnX, spawnY)

    local projData = {
        entity = proj,
        x = spawnX,
        y = spawnY,
        vx = dirX * projectileSpeed,
        vy = dirY * projectileSpeed,
        age = 0
    }

    table.insert(projectiles, projData)
end

local function UpdateProjectiles(dt)
    for i = #projectiles, 1, -1 do
        local p = projectiles[i]

        -- move
        p.x = p.x + p.vx * dt
        p.y = p.y + p.vy * dt
        Entity.set_global_pos(p.entity, p.x, p.y)

        -- age
        p.age = p.age + dt
        if p.age > projectileLifetime then
            -- destroy projectile entity and remove from list
            Entity.destroy_entity(p.entity)
            table.remove(projectiles, i)
        end
    end
end

----------------------------------------------------------
-- OnStart
----------------------------------------------------------
function ExampleScript:OnStart()

    ------------------------------------------------------
	-- Background Texture
	------------------------------------------------------
    Background = Entity.create_entity()
    Entity.add_sprite_component(Background, assets.textures.Background, 960, 640, 0)

    ------------------------------------------------------
    -- Create player1
    ------------------------------------------------------
    player1 = Entity.create_entity()
    Entity.set_global_pos(player1, x, 200)
	
    local playerSprite1 = Entity.add_sprite_component(player1, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(playerSprite1, 1)

    Entity.add_fysics_component(player1, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player1, false, 1)

    ------------------------------------------------------
    -- Create player2
    ------------------------------------------------------
    player2 = Entity.create_entity()
    Entity.set_global_pos(player2, x + 50, 200)
	
    local playerSprite2 = Entity.add_sprite_component(player2, assets.textures.FlappyBird, 32, 32, 10)
    Sprite.set_columns(playerSprite2, 1)

    Entity.add_fysics_component(player2, enums.bodytype.dynamicBody, true)
    Fysics.add_sprite_collider(player2, false, 1)

    local floorY = 500
    
	------------------------------------------------------
	-- Ground tile
	------------------------------------------------------
	tile = Entity.create_entity()
	Entity.set_global_pos(tile, 250, floorY)
    local tileSprite = Entity.add_sprite_component(tile, assets.textures.Sand, 500, 50, 1)
    Sprite.set_columns(tileSprite, 1)

	Entity.add_fysics_component(tile, enums.bodytype.staticBody, false)
	Fysics.add_sprite_collider(tile, false, 1)

    ------------------------------------------------------
    -- Platform
    ------------------------------------------------------
    platform1 = Entity.create_entity()
	Entity.set_global_pos(platform1, 300, 400)
    local platformSprite = Entity.add_sprite_component(platform1, assets.textures.Sand, 500, 50, 1)
    Sprite.set_columns(platformSprite, 1)

	Entity.add_fysics_component(platform1, enums.bodytype.staticBody, false)
	Fysics.add_edge_collider(platform1, 20, 30, 50, 30, false)
end

----------------------------------------------------------
-- OnUpdate
----------------------------------------------------------
function ExampleScript:OnUpdate()
    local dt = Mafs.delta_time()

    -- get current velocity for player1
    local vel1 = Fysics.get_linear_velocity(player1)
    local vx1 = Mafs.get_vec_x(vel1)
    local vy1 = Mafs.get_vec_y(vel1)

    -- get current velocity for player2
    local vel2 = Fysics.get_linear_velocity(player2)
    local vx2 = Mafs.get_vec_x(vel2)
    local vy2 = Mafs.get_vec_y(vel2)

    --------------------------------------------------
    -- JUMPING
    --------------------------------------------------
	if Input.get_button_down(0, Buttons.ionix_a) then
        jumpCount1 = jumpCount1 + 1
        Fysics.add_force_to_center(player1, 0, -30 / jumpCount1)
	end
    if Input.get_button_down(1, Buttons.ionix_a) and jumpCount2 <= 1 then
        jumpCount2 = jumpCount2 + 1
        Fysics.add_force_to_center(player2, 0, -30 / jumpCount2)
	end
    
    -- removed coin destroy (coin not defined)
    -- if Input.get_key_down(Keys.ionix_m) then
    --     Entity.destroy_entity(coin)
    -- end

    --------------------------------------------------
    -- HORIZONTAL MOVEMENT
    --------------------------------------------------
    if Input.get_left_stick_x(0) then
        vx1 = 2.5 * Input.get_left_stick_x(0)
    else
        vx1 = 0
    end
	
    if Input.get_left_stick_x(1) then
        vx2 = 2.5 * Input.get_left_stick_x(1)
    else
        vx2 = 0
    end

    Fysics.set_linear_velocity(player1, vx1, vy1)
    Fysics.set_linear_velocity(player2, vx2, vy2)

    --------------------------------------------------
    -- (MANUAL) SHOOT WITH B BUTTON + LEFT STICK AIM
    --------------------------------------------------

    -- shared cooldown timer
    if fireCooldown > 0 then
        fireCooldown = fireCooldown - dt
        if fireCooldown < 0 then fireCooldown = 0 end
    end

    -- player1 shoot (controller 0, B button)
    if Input.get_button_down(0, Buttons.ionix_b) and fireCooldown <= 0 then
        local p1pos = Entity.get_global_pos(player1)
        local p1x = Mafs.get_vec_x(p1pos)
        local p1y = Mafs.get_vec_y(p1pos)

        local dirX = Input.get_left_stick_x(0)
        local dirY = Input.get_left_stick_y(0)

        SpawnProjectile(p1x, p1y, dirX, dirY)
        fireCooldown = fireInterval
    end

    -- player2 shoot (controller 1, B button)
    if Input.get_button_down(1, Buttons.ionix_b) and fireCooldown <= 0 then
        local p2pos = Entity.get_global_pos(player2)
        local p2x = Mafs.get_vec_x(p2pos)
        local p2y = Mafs.get_vec_y(p2pos)

        local dirX = Input.get_left_stick_x(1)
        local dirY = Input.get_left_stick_y(1)

        SpawnProjectile(p2x, p2y, dirX, dirY)
        fireCooldown = fireInterval
    end

    --------------------------------------------------
    -- AUTO-FIRE PROJECTILE EVERY 30 SECONDS
    --------------------------------------------------
    autoFireTimer = autoFireTimer + dt
    if autoFireTimer >= autoFireInterval then
        local p1pos = Entity.get_global_pos(player1)
        local p1x = Mafs.get_vec_x(p1pos)
        local p1y = Mafs.get_vec_y(p1pos)

        local p2pos = Entity.get_global_pos(player2)
        local p2x = Mafs.get_vec_x(p2pos)
        local p2y = Mafs.get_vec_y(p2pos)

        -- auto-fire straight right
        SpawnProjectile(p1x + 16, p1y, 1, 0)
        SpawnProjectile(p2x + 16, p2y, 1, 0)

        autoFireTimer = autoFireTimer - autoFireInterval
    end

    --------------------------------------------------
    -- UPDATE PROJECTILES
    --------------------------------------------------
    UpdateProjectiles(dt)
end

----------------------------------------------------------
-- Collisions
----------------------------------------------------------
function ExampleScript:OnCollisionEnter()
    if Fysics.col(player1, tile) or Fysics.col(player1, platform1) then
        jumpCount1 = 0
        print("player1 grounded")
    end
    if Fysics.col(player2, tile) or Fysics.col(player2, platform1) then
        jumpCount2 = 0
        print("player2 grounded")
    end
end

function ExampleScript:OnTriggerEnter()
end

function ExampleScript:OnTriggerExit()
end

function ExampleScript:OnCollisionExit()
end

return ExampleScript
